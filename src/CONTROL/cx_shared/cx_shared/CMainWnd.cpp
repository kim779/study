// CMainWnd.cpp: 구현 파일
//

#include "pch.h"
#include "cx_shared.h"
#include "CMainWnd.h"
#include "../AXIS/axMsg.hxx"
#include "../H/axisfire.h"
#include "../H/TickStore.h"

// CMainWnd

IMPLEMENT_DYNAMIC(CMainWnd, CWnd)

std::unordered_map<std::string, int> g_dllcodeToIndex;
TickSnapshot g_dlltickSlots[MAX_SLOT];
int g_dllnextIndex = 0;
std::shared_mutex  g_dllcodeMapLock;
std::vector<int>  g_dlldirtySlots;  // ← 추가
std::mutex        g_dlldirtyMtx;    // ← 추가

CMainWnd::CMainWnd(CWnd* pParent, void* pParam)
{
	int ilen = strlen((char*)pParam);
	
	CString stmp, shkey, smkey;
	stmp.Format("%s", (char*)pParam);
	shkey = stmp.Left(stmp.Find("|"));
	smkey = stmp.Mid(stmp.Find("|") + 1);

	FileLog(shkey);
	FileLog(smkey);

	m_hkey = std::make_unique<char[]>(shkey.GetLength() + 1);
	memcpy(m_hkey.get(), (char*)pParam, shkey.GetLength());

	m_mkey = std::make_unique<char[]>(smkey.GetLength() + 1);
	memcpy(m_mkey.get(), (char*)pParam, smkey.GetLength());

	m_pWizard = pParent;
}

CMainWnd::~CMainWnd()
{
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()



// CMainWnd 메시지 처리기

void CMainWnd::StartWorkerThread()
{
	m_workerRunning = true;
	m_workerThread = std::thread(&CMainWnd::WorkerThreadFunc, this);
}

void CMainWnd::StopWorkerThread()
{
	m_workerRunning = false;
	if (m_workerThread.joinable())
		m_workerThread.join();
}

void CMainWnd::WorkerThreadFunc()
{
	while (m_workerRunning)
	{
		SafeAlertItem* item = nullptr;
		{
			std::lock_guard<std::mutex> lock(m_alertMutex);
			if (!m_alertQueue.empty())
			{
				item = m_alertQueue.front();
				m_alertQueue.pop();

				// 같은 종목이 큐에 또 있으면 최신값으로 교체하고 이전꺼 버림
				while (!m_alertQueue.empty() &&
					strcmp(m_alertQueue.front()->code, item->code) == 0)
				{
					delete item;
					item = m_alertQueue.front();
					m_alertQueue.pop();
				}
			}
		}

		int idx = EnsureSlotIndexForCode(item->code);
		if (idx >= 0)
		{
			UpdateSnapshotFromAlert(g_dlltickSlots[idx], item);
			DLL_PushDirtySlot(idx);  // ← API로
		}
		delete item;
	}
}

int CMainWnd::EnsureSlotIndexForCode(const char* code)
{
	if (!code || !code[0])
		return -1;

	// 읽기로 먼저 확인
	{
		std::shared_lock<std::shared_mutex> readLock(g_dllcodeMapLock);
		auto it = g_dllcodeToIndex.find(code);
		if (it != g_dllcodeToIndex.end())
			return it->second;
	}

	// 없으면 쓰기락으로 새로 할당
	std::unique_lock<std::shared_mutex> writeLock(g_dllcodeMapLock);

	// double-check (락 잡는 사이 다른 스레드가 먼저 넣었을 수 있음)
	auto it = g_dllcodeToIndex.find(code);
	if (it != g_dllcodeToIndex.end())
		return it->second;

	if (g_dllnextIndex >= MAX_SLOT)
		return -1;

	int idx = g_dllnextIndex++;
	g_dllcodeToIndex.emplace(code, idx);
	CopyZ(g_dlltickSlots[idx].code, sizeof(g_dlltickSlots[idx].code), code);

	CString slog;
	slog.Format("[EnsureSlot] 신규등록 code=[%s] idx=[%d] 총갯수=[%d]\n",
		code, idx, (int)g_dllcodeToIndex.size());
	OutputDebugString(slog);

	return idx;
}

DLL_API int DLL_EnsureSlotIndex(const char* code)
{
	CMainWnd* pFrame = (CMainWnd*)AfxGetMainWnd();
	if (!pFrame)
		return -1;

	return pFrame->EnsureSlotIndexForCode(code);
}

DLL_API const TickSnapshot* DLL_GetTickSlots()
{
	return g_dlltickSlots;
}

DLL_API void DLL_PushDirtySlot(int idx)
{
	std::lock_guard<std::mutex> lk(g_dlldirtyMtx);
	g_dlldirtySlots.push_back(idx);
}

DLL_API int DLL_SwapDirtySlots(int* outBuf, int bufSize)
{
	std::vector<int> dirty;
	{
		std::lock_guard<std::mutex> lk(g_dlldirtyMtx);
		dirty.swap(g_dlldirtySlots);
	}
	int count = min((int)dirty.size(), bufSize);
	memcpy(outBuf, dirty.data(), count * sizeof(int));
	return count;
}

void CMainWnd::DumpAllSlots()
{
	DWORD now = GetTickCount();
	DWORD elapsed = (m_tickCountStart > 0) ? (now - m_tickCountStart) : 1;
	float elapsedSec = elapsed / 1000.0f;
	if (elapsedSec < 0.001f) elapsedSec = 0.001f;

	std::vector<std::pair<std::string, int>> sorted(m_tickCount.begin(), m_tickCount.end());
	std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) {
		return a.second > b.second;
		});

	CString slog;
	slog.Format("[TopTickers][dll] 경과=[%.1f초] 총종목=[%d] ==========\n",
		elapsedSec, (int)sorted.size());
	OutputDebugString(slog);

	int rank = 1;
	for (const auto& item : sorted)
	{
		float perSec = item.second / elapsedSec;

		// g_tickSlots에서 현재가/거래량/등락률 읽기
		const char* curr = "";
		const char* volume = "";
		const char* rate = "";

		{
			std::shared_lock<std::shared_mutex> readLock(g_dllcodeMapLock);
			auto it = g_dllcodeToIndex.find(item.first);
			if (it != g_dllcodeToIndex.end())
			{
				const TickSnapshot& slot = g_dlltickSlots[it->second];
				// seqlock - 쓰기중이면 스킵
				int seq = slot.seq.load(std::memory_order_acquire);
				if (!(seq & 1))
				{
					if (slot.valid[23]) curr = slot.values[23];  // 현재가
					if (slot.valid[27]) volume = slot.values[27];  // 거래량
					if (slot.valid[33]) rate = slot.values[33];  // 등락률
				}
			}
		}

		slog.Format("[TopTickers][dll][%3d위] code=[%-10s] 수신=[%d회/%.1f초당] 현재가=[%s] 거래량=[%s] 등락률=[%s]\n",
			rank++, item.first.c_str(), item.second, perSec,
			curr, volume, rate);
		OutputDebugString(slog);

		if (rank > 30) break;
	}

	int totalCnt = 0;
	for (const auto& p : m_tickCount) totalCnt += p.second;
	slog.Format("[TopTickers][dll] 전체수신=[%d회] 전체초당=[%.1f회/초] ====END====\n",
		totalCnt, totalCnt / elapsedSec);
	OutputDebugString(slog);
}

long CMainWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CString slog;
	switch (LOBYTE(LOWORD(wParam)))
	{
		case MMSG_SHARED_DUMP:
		{
			DumpAllSlots();
		}
		break;
	    case DLL_ALERT:
		{
			struct _alertR* alertR = (struct _alertR*)lParam;
			slog.Format("[shared] code = [%s] ", alertR->code);
			OutputDebugString(slog);

			// 덤프용 카운터
			if (m_tickCountStart == 0)
				m_tickCountStart = GetTickCount();

			const char* code = (const char*)(LPCTSTR)alertR->code;

			if (!code || !code[0]) 
				return 0;

			m_tickCount[std::string(code)]++;

			// 큐 사이즈 체크 - new 하기 전에
			{
				std::lock_guard<std::mutex> lock(m_alertMutex);
				if (m_alertQueue.size() > 500)
					return 0;
			}

			// UI 스레드에서 포인터 역참조 + 값 복사
			SafeAlertItem* item = new SafeAlertItem{};
			CopyZ(item->code, CODE_LEN, code);
			item->ts_ms = GetTickCount();
			memset(item->hasValue, 0, sizeof(item->hasValue));

			for (int ii = alertR->size - 1; ii >= 0; ii--)
			{
				if (alertR->ptr[ii] == 0) continue;
				const PTR_T* data = reinterpret_cast<const PTR_T*>(alertR->ptr[ii]);

				for (int jj = 0; jj < MAX_RTS_INDEX; jj++)
				{
					if (data[jj] == 0) continue;
					const char* val = reinterpret_cast<const char*>(data[jj]);
					if (val && val[0])
					{
						CopyZ(item->values[jj], SYMBOL_STR_LEN, val);
						item->hasValue[jj] = true;
					}
				}
			}

			std::lock_guard<std::mutex> lock(m_alertMutex);
			m_alertQueue.push(item);

		}
			break;
		case MMSG_SHARED_REGWND:
		{
			CWnd* pwnd = (CWnd*)lParam;
			m_ShMemory->AddHandle(pwnd->m_hWnd);
		}
		break;
		case MMSG_SHARED_BROADCAST:
		{
			CString sval;
			sval.Format("%s", (char*)lParam);
			m_ShMemory->SendTrigger(sval);
		}
		break;
		case MMSG_SHARED_CTRLDESTROY:
		{
			CWnd* pwnd = (CWnd*)lParam;
			m_ShMemory->RemoveHandle(pwnd->m_hWnd);
		}
		break;
		case MMSG_SHARED_GETHANDLECNT:
		{
			return (int)m_ShMemory->GetHandleCount();
		}
		case MMSG_SHARED_PROCDLL:
		{//메인으로 받은 장운영메시지를 플랫폼을 통해 화면에 브로드케스팅
			CString sval;
			sval.Format("%s", (char*)lParam);
			int key = HIWORD(wParam);
			key = HIBYTE(LOWORD(wParam));
			m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(HIWORD(wParam), HIBYTE(LOWORD(wParam)))), (LPARAM)(LPCTSTR)sval);
	
			return 1;
		}
	}
	return true;
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	DWORD processID = GetCurrentProcessId();
	m_ShMemory = std::make_unique<CShMemory>();
	CString sSHMENAME;
	sSHMENAME.Format("%s", (char*)m_hkey.get());
	m_ShMemory->remainSHMEMNAME = sSHMENAME;
	m_ShMemory->InitSharedMemory(this->m_hWnd);
	//m_ShMemory->AddHandle(this->m_hWnd);

	m_SmMemory = std::make_unique<CShMemory>();
	sSHMENAME.Format("%s", (char*)m_mkey.get());
	m_SmMemory->remainSHMEMNAME = sSHMENAME;
	m_SmMemory->InitSharedMemory(this->m_hWnd);
	//m_SmMemory->AddHandle(this->m_hWnd);
	if(Axis_IsMainRTS())
		StartWorkerThread();
	FileMove();
	return 0;
}


void CMainWnd::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void   CMainWnd::FileMove()
{
	m_sName = Variant(nameCC, "");
	CString strVal, strMovePath, strtmp, strfile, strOriPath, stmp, stemp;

	char sbuf[1024]{};
	GetModuleFileName(nullptr, sbuf, 260);
	m_sRoot.Format("%s", sbuf);
	m_sRoot.TrimRight();
	m_sRoot = m_sRoot.Left(m_sRoot.Find("exe"));

	char buff[128];
	CString iniConf = m_sRoot + "tab\\AXISFILE.ini";
	GetPrivateProfileString("move", "cnt", "", buff, sizeof(buff), iniConf);
	stmp.Format("%s", buff);
	int cnt = atoi(stmp);

	// --- 기존 히스토리 읽기
	char histBuff[2048]{};
	GetPrivateProfileStringA("history", "file", "", histBuff, sizeof(histBuff), CT2CA(iniConf));
	CString strHistory(histBuff);

	for (int ii = 0; ii < cnt; ii++)
	{
		stmp.Format("%d", ii);

		GetPrivateProfileString("move", stmp, "", buff, sizeof(buff), iniConf);
		strVal.Format("%s", buff);
		strVal.TrimRight();

		//ex) gex:mapc2_30-39.xml;icss\finddata\map     다운로드위치:파일명 ; 옮길위치
		strtmp = Parser(strVal, ";");
		strtmp.TrimRight();
		stmp = Parser(strtmp, ":");
		strtmp.TrimRight();
		strfile = strtmp;
		strOriPath.Format("%s%s\\%s", m_sRoot, stmp, strfile);

		stemp = strVal;
		stemp.TrimRight();
		strMovePath.Format("%s%s", m_sRoot, stemp);

		CFileFind ff;
		if (!ff.FindFile(strMovePath))
			::CreateDirectory(strMovePath, NULL);

		strMovePath.Format("%s%s\\%s", m_sRoot, stemp, strfile);

		CFileFind cfFind;
		BOOL bFind = cfFind.FindFile(strOriPath);

		if (bFind)
		{
			//	if (strHistory.Find(strfile) != -1)
			//		continue; // 이미 처리됨

			CopyFile(strOriPath, strMovePath, FALSE);
			DeleteFile(strOriPath);

			//// 기존 히스토리 읽기
			//char histBuff[1024]{};
			//GetPrivateProfileStringA("history", "file", "", histBuff, sizeof(histBuff), CT2CA(iniConf));

			//CString strHistory(histBuff);
			//if (!strHistory.IsEmpty())
			//	strHistory += ";";

			//// 파일명 추가
			//strHistory += strfile;

			//// 다시 기록
			//WritePrivateProfileStringA("history", "file", CT2CA(strHistory), CT2CA(iniConf));
		}
	}


	int readL = 0;
	char readB[64]{};
	CString sfile;
	sfile.Format("%s\\tab\\AXISENC.ini", m_sRoot);
	m_slog.Format("%s\t%s", "", "");
	WritePrivateProfileString("ABROAD", "auth", m_slog, sfile);
}

CString CMainWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char* dta = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

CString CMainWnd::Parser(CString& srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}