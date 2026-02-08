// CMainWnd.cpp: 구현 파일
//

#include "pch.h"
#include "cx_shared.h"
#include "CMainWnd.h"
#include "../AXIS/axMsg.hxx"
#include "../H/axisfire.h"

// CMainWnd

IMPLEMENT_DYNAMIC(CMainWnd, CWnd)

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


long CMainWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
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