// CDlgSTD.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlgSTD.h"
#include "afxdialogex.h"
#include <future>
          
// CDlgSTD 대화 상자





IMPLEMENT_DYNAMIC(CDlgSTD, CDialogEx)

CString _slog {};

class _clFnctionPt
{
public:
	_clFnctionPt(CString sDll)
	{
		m_instance = LoadLibrary(sDll);
		if (!m_instance)
		{
			CString msg;
			msg.Format(_T("Failed to load DLL: %s"), sDll);
			AfxMessageBox(msg);
		}
	}

	HINSTANCE m_instance;

	template <typename Fn>
	//템플릿한다
	Fn func(LPCTSTR lpname)
	{
		return reinterpret_cast<Fn>(GetProcAddress(m_instance, lpname));
	}

	void callAuse_MessageBox(std::wstring sval)
	{
		using MSGBOXFUNC = int (WINAPI*)(HWND, LPCWSTR, LPCWSTR, UINT);
		LPCWSTR wstr = sval.c_str();
		MSGBOXFUNC fn = func<MSGBOXFUNC>("MessageBoxW");
		if (fn)
		{
			fn(NULL, wstr, L"Dynamic Call", MB_OK | MB_ICONINFORMATION);
		}

	
	}
};

CString  readFile_ayncTd(CString strfile)
{
	char readB[1024]{};

	GetPrivateProfileString("SCREEN", "UNFLESH", "test", readB, sizeof(readB), strfile);

	CString sval;
	sval.Format("%s", readB);
	sval.TrimRight();
	return sval;

}

CDlgSTD::CDlgSTD(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_STD, pParent)
{

}

CDlgSTD::~CDlgSTD()
{
	m_bThreadEnd = FALSE;
	if(future.valid())
		future.wait();
	if (m_future.valid()) {
		m_future.get();  // 결과는 무시하지만 비동기 작업이 끝날 때까지 기다림
	}

	std::cout << "After get, valid = " << std::boolalpha << m_future.valid() << std::endl;

	if (m_future.valid()) {
		try {
			m_future.get();  // 예외가 발생했으면 여기서 받아냄
		}
		catch (...) {
			// 예외 무시 or 로깅
		}
	}
//	if(m_future.valid())
}

void CDlgSTD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSTD, CDialogEx)
	ON_BN_CLICKED(IDC_MSG, &CDlgSTD::OnBnClickedMsg)
	ON_BN_CLICKED(IDC_TEST, &CDlgSTD::OnTest)
	ON_BN_CLICKED(IDC_FUNC_TEMP, &CDlgSTD::OnBnClickedFuncTemp)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DRAW_THREAD, &CDlgSTD::OnBnClickedDrawThread)
	ON_CBN_SELCHANGE(IDC_FUNC_CB, &CDlgSTD::OnCbnSelchangeFuncCb)
	ON_BN_CLICKED(IDC_DRAW_STOP, &CDlgSTD::OnBnClickedDrawStop)
	ON_BN_CLICKED(IDC_QUE_START, &CDlgSTD::OnBnClickedQueStart)
	ON_BN_CLICKED(IDC_CHK_VM, &CDlgSTD::OnBnClickedChkVm)
END_MESSAGE_MAP()


void AFXAPI _Msg(const TCHAR* fmt = _T(""), ...)
{
	CString	tmpstr, str;

	va_list	valist;
	va_start(valist, fmt);
	tmpstr.FormatV(fmt, valist);
	va_end(valist);
	const ULONGLONG end = GetTickCount64();

	std::async(launch::deferred, [&]() {
		if (tmpstr == _T(""))
		{
			str = CString(_T("<Debug>")) + CString(_LINE);
			g_begin = end;
		}
		else
		{
			//const CTime time = CTime::GetCurrentTime();
			CString sTime;

			sTime.Format(_T("[%lld ms]"), gsl::narrow_cast<LONG64>(end - g_begin));
			str = _T("<Debug>") + sTime + tmpstr;
		}

		if ((end - g_begin) > 10)
			OutputDebugString(str + _T("\n"));

		}).get();
};

BOOL CDlgSTD::OnInitDialog()
{
	CString stmp;
	stmp = "[THREAD][OnInitDialog]";
	OutputDebugString(stmp);


	CDialogEx::OnInitDialog();
	m_toolTip.Create(this);
	CWnd* pButton = GetDlgItem(IDC_MSG);
	m_toolTip.AddTool(pButton, _T("_async 스레드와 멤버변수future <--  미래의 결과 사용(OnBnClickedMsg)"));

	pButton = GetDlgItem(IDC_FUNC_TEMP);
	m_toolTip.AddTool(pButton, _T("함수 실행 스레드(OnBnClickedFuncTemp)"));

	pButton = GetDlgItem(IDC_DRAW_THREAD);
	m_toolTip.AddTool(pButton, _T("그리기 스레드(OnBnClickedDrawThread)"));
	
	

	m_toolTip.Activate(TRUE);
	((CWnd*)GetDlgItem(IDC_STATIC_PAINT))->ShowWindow(SW_HIDE);



	_map.emplace(std::make_pair<CString, int>("FUTURE", FUTURE));
	_map.emplace(std::make_pair<CString, int>("REAL", REAL));
	_map.emplace(std::make_pair<CString, int>("REDUCE", REDUCE));
	_map.emplace(std::make_pair<CString, int>("FOREACH", FOREACH));
	_map.emplace(std::make_pair<CString, int>("ANY_OF", ANY_OF));
	_map.emplace(std::make_pair<CString, int>("READFILE_THREAD", READFILE_THREAD));
	_map.emplace(std::make_pair<CString, int>("MAP_RETURN", MAP_RETURN));
	_map.emplace(std::make_pair<CString, int>("LOADLIBFUC", LOADLIBFUC));
	


	m_cb = (CComboBox*)(GetDlgItem(IDC_FUNC_CB)); 
	m_cb->AddString("FUTURE");	
	m_cb->AddString("REAL");
	m_cb->AddString("REDUCE");
	m_cb->AddString("FOREACH");
	m_cb->AddString("ANY_OF");
	m_cb->AddString("READFILE_THREAD");
	m_cb->AddString("MAP_RETURN");
	m_cb->AddString("LOADLIBFUC");


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDlgSTD::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_toolTip.m_hWnd)
		m_toolTip.RelayEvent(pMsg);
	return CDialogEx::PreTranslateMessage(pMsg);
}
ULONGLONG m_sum{};
void testsum(ULONGLONG val)
{
	//std::vector<int> vv(m_nGroup);
	std::vector<ULONGLONG> vv(3);

	for_each(std::execution::par, vv.begin(), vv.end(), [&](ULONGLONG val) {
			m_sum += val;
		});

}

class _csTest
{
public:
	_csTest();
	_csTest(int isec, int itick) {
		isecound = isec;
		tick = itick;
	};
	~_csTest() {
		isecound = 0;
		tick  =  0;
	}
	int isecound;
	int tick;
};

void CDlgSTD::OnTest()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString stext;
	m_cb->GetWindowText(stext);

	int icbsel = _map[stext];
	auto at = _map.find(stext);
	
	
	switch (icbsel)
	{
		case FUTURE:
		{
		  int result = m_future.get();

			std::cout << "After get, valid = " << std::boolalpha << m_future.valid() << std::endl;
			auto at = m_future.valid();

			m_slog.Format("[THREAD][FUTURE]<%d>  future get = [%d]", __LINE__ ,result);
			OutputDebugString(m_slog);
		}
		break;
		case REAL:
		{
			ULONGLONG gt = GetTickCount64();
			std::async([&, gt] {
				testsum(gt);
				});
		}
		break;
		case REDUCE:
		{
			std::vector<int> data(1000000, 2); // 100만 개의 1
			std::vector<ULONGLONG> vv = { 1,2,3 ,4,5,6,7};
		

			int ival = 0;
			for_each(std::execution::par, vv.begin(), vv.end(), [&](int ii) {
				if (ii % 2 == 0)
					ival += ii;
				}
				
			);

			ival = 0;

			std::vector<int> result(vv.size());
			std::transform(std::execution::par, vv.begin(), vv.end(), result.begin(), [](int ii) {
				if (ii % 2 == 0)
					return ii;
				else
					return 0;
				});

		}
		break;
		case FOREACH:
		{
			//먼저 구조체
			struct stTest
			{
				int inum;
				int igubn;
				CString strtmp;
				std::map<int, CString> IS_map;
			};
			
			stTest items[] = {
			{1, 1, "KRX" , {{1, "111"}, {2, "222"}}},
			{2, 2, "NXT" , {{3, "333"}, {4, "444"}}},
			{3, 4, "통합" , {{5, "555"}, {6, "6666"}}}
			};

			for(auto& item : items)
			{
				if (item.inum % 2 == 1)
				{
					if (!item.IS_map.empty())
					{
						auto it = item.IS_map.begin();
						if (it->first % 2 == 1)
						{
							TRACE("\r\n" + it->second);
							TRACE("\r\n" + std::get<1>(*it));
						}
					}
				}
			}
		}
		break;
		case ANY_OF:
		{
			//우선 arr 하나 만든다
			std::array<int, 100> arr;
			arr = { 1,2,3,4,5 };

			//다른 벡터를 만들어서
			std::vector<int> vt;
			vt.emplace_back(9);
			vt.emplace_back(8);
			vt.emplace_back(7);
			vt.emplace_back(6);
			vt.emplace_back(5);

			bool ret = std::any_of(vt.begin(), vt.end(), [&arr](int val) {
				return std::any_of(arr.begin(), arr.end(), [&val ](int num)
					{
						if (val == num)
							return true;
						else
							return false;
					});
				});

		}
		break;
		case READFILE_THREAD:
		{
	//std::async(launch::deferred, [&]() {
			std::future<CString> fut;
			fut = std::async(launch::async, [this]()  -> CString{
				return readFile_ayncTd("testfile.ini");
				});
			m_slog.Format("%s", fut.get());
			AfxMessageBox(m_slog);
		}
		break;
		case MAP_RETURN:
		{
			unordered_map<int, _csTest> umap;
			int ival = GetTickCount();
			m_slog.Format("%d", ival);

			std::atomic<bool> m_bThreadEnd;
			m_bThreadEnd.store(true);

			std::future<int> fut;
			fut = std::async(std::launch::async, [&]() -> int {
				while (m_bThreadEnd.load())
				{
					const CTime time = CTime::GetCurrentTime();
					int isec = time.GetSecond();
					int itick = (int)GetTickCount();
					auto [it, bret] = umap.emplace(isec, _csTest(isec, (int)itick));
		
					if (bret)
					{
						m_slog.Format("\r\n!!!  sec=[%d]  tick=[%d]", it->first, it->second.isecound, it->second.tick);
						OutputDebugString(m_slog);
					}
					else
					{
						if (  itick - it->second.tick > 10)
						{
							it->second.isecound = isec;
							it->second.tick = itick;
						}
						m_slog.Format("\r\n @@@ sec=[%d]  tick=[%d]  diff=[%d]", it->first, it->second.isecound, it->second.tick,
							it->second.tick - itick);
						OutputDebugString(m_slog);
					}

					Sleep(500);
					
				}
				return 11;
			});

		}
		break;
		case LOADLIBFUC:
		{
			CString strval;
			strval = "123";
			std::wstring ws = CT2W(strval);


			_clFnctionPt user32(_T("user32.dll"));
			user32.callAuse_MessageBox(ws);
		}
		break;
	}
	

	OutputDebugString(m_slog);
}



void CDlgSTD::OnBnClickedMsg()
{
	m_bThreadEnd = TRUE;
	_iCnt = 0;

	std::future<int> fut;

	fut = std::async(std::launch::async, [&]() -> int {
		while (m_bThreadEnd)
		{
			_iCnt += 1;
			m_slog.Format("\r\n%d", _iCnt);
			OutputDebugString(m_slog);
			if (_iCnt > 100)
			{
				m_bThreadEnd = false;
				return 11;
			}
			Sleep(10);
		}
		});

	int ival = fut.get();
	CString str;
}


//template <class _Fty, class... _ArgTypes>
//_NODISCARD future<_Invoke_result_t<decay_t<_Fty>, decay_t<_ArgTypes>...>> async(_Fty&& _Fnarg, _ArgTypes&&... _Args) {
//	// manages a callable object launched with default policy
//	return _STD async(launch::async, _STD forward<_Fty>(_Fnarg), _STD forward<_ArgTypes>(_Args)...);
//}

#include <chrono>
#include <type_traits>


int add(int a, int b) {
	cout << "작업 스레드 id: " << this_thread::get_id() << endl;
	this_thread::sleep_for(chrono::seconds(1));

	_slog.Format("[THREAD][함수포인터?] 작업 스레드 id: [%d] a+ b = [%d] ", this_thread::get_id(), a + b);
	OutputDebugString(_slog);
	return a + b;

}


template <class _Fty, class... _ArgTypes>
_NODISCARD future<_Invoke_result_t<decay_t<_Fty>, decay_t<_ArgTypes>...>> async1(_Fty&& _Fnarg, _ArgTypes&&... _Args) {
	// manages a callable object launched with default policy
	return _STD async(launch::async, _STD forward<_Fty>(_Fnarg), _STD forward<_ArgTypes>(_Args)...);
}


//testcode 위에거 따라하기
template <class aa, class... bb>
_NODISCARD future < _Invoke_result_t<decay_t< aa>, decay_t< bb>...>> async2(aa&& _aa, bb&&... _bb)
{
	return std::async(launch::async, std::forward<aa>(_aa), std::forward<bb>(_bb)...);
}

void CDlgSTD::OnBnClickedFuncTemp()
{
	//using AddFunc = int(*)(int, int);
	m_future = async2(add, 10, 20);

	//cout << "비동기 작업 실행 중..." << endl;

	//// 결과 받아오기 (get 호출 시까지 기다림)
	int result = m_future.get();

	m_slog.Format("[THREAD][FUTURE]<%d>  future get = [%d]", __LINE__,result);
	OutputDebugString(m_slog);
}

void CDlgSTD::OnPaint()
{
	//IDC_STATIC_PAINT
	CPaintDC dc(this);
	
	while (true)
	{
		std::shared_ptr<_csDrawData> data;
		std::lock_guard<std::mutex> lock(g_DrawMutex);

		if (g_DrawQueue.empty())
			break;

		data = g_DrawQueue.front();
		g_DrawQueue.pop();

		if (data)
		{
			// 색상 채우기
			CBrush brush(data->frgb);
			dc.FillRect(&data->_rect, &brush);

			// 문자열 출력
			dc.SetBkMode(TRANSPARENT);
			dc.TextOut(data->_rect.left, data->_rect.top, data->_sData);
		}
	}
	



}

void CDlgSTD::OnBnClickedDrawThread()
{
	atmc_StopFlag = false;

	_fut = std::async(std::launch::async, [this]() -> bool {
		srand((unsigned)time(nullptr));
		while (!atmc_StopFlag)
		{
			auto data = std::make_shared<_csDrawData>();
			DWORD tick = GetTickCount();
			data->_sData.Format(_T("Tick: %d"), g_DrawQueue.size());

			data->frgb = RGB(rand() % 256, rand() % 256, rand() % 256);

			CRect rec;
			((CWnd*)GetDlgItem(IDC_STATIC_PAINT))->GetWindowRect(rec);
			ScreenToClient(&rec);
			data->_parentRect = rec;

			int w = 50 + rand() % 100;
			int h = 30 + rand() % 60;

			int x = data->_parentRect.left + rand() % max(1, (data->_parentRect.Width() - w));
			int y = data->_parentRect.top + rand() % max(1, (data->_parentRect.Height() - h));

			data->_rect = CRect(x, y, x + w, y + h);

			std::lock_guard<std::mutex> lock(g_DrawMutex);
			g_DrawQueue.push(data);
	
			this->InvalidateRect(nullptr, FALSE); // WM_PAINT 대신 안전하게
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			// 또는 std::this_thread::yield();  // 0~1ms 정도만 쉼
		}
		return true;
		});


}

void CDlgSTD::OnBnClickedDrawStop()
{
	atmc_StopFlag.store(true);
	int ret{};
	ret = _fut.get();
}

void CDlgSTD::OnCbnSelchangeFuncCb()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CDlgSTD::OnBnClickedQueStart()
{
	int numThreads = 4;
	for (int ii = 0; ii < numThreads; ii++)
	{
		CWinThread* pThread = AfxBeginThread((AFX_THREADPROC)ThreadRunner<CDlgSTD>::Run, (LPVOID)this, THREAD_PRIORITY_HIGHEST);
		m_threads.push_back(pThread);
	}
}

void CDlgSTD::CheckVectorMap()
{
	struct _intersx
	{
		char	gubn = '0';		// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수	
		CString code;			// 종목코드
		CString name;			// 종목명
		CString	xprc;			// 보유단가
		CString	xnum;			// 보유수량
		CString	xupnum;			// 상장 주식수
		char	bookmark = '0';		// 북마크
		char	futureGubn{};		// 신용매매 구분
		double  creditPrc{};		// 신용금액
		double	maeipPrc{};		// 매입금액

		void copy(void* item);

		void operator=(struct _intersx& ref)
		{
			code = ref.code;
			name = ref.name;
			gubn = ref.gubn;
			xprc = ref.xprc;
			xnum = ref.xnum;
			xupnum = ref.xupnum;
			bookmark = ref.bookmark;
			futureGubn = ref.futureGubn;
			creditPrc = ref.creditPrc;
			maeipPrc = ref.maeipPrc;
		}

		void empty()
		{
			code.Empty();
			name.Empty();
			gubn = '0';
			xprc.Empty();
			xnum.Empty();
			xupnum.Empty();
			bookmark = '0';
			futureGubn = '0';
			creditPrc = 0;
			maeipPrc = 0;
		}
	};


	std::map<CString, int>	_mapSymbol;

	CString stmp;
	stmp = "tset";
	auto at = _mapSymbol.emplace(std::make_pair(stmp, 1));
	
	






	std::map<CString, int> _symbol;
	std::map<int, CString> _GroupName;
	std::map<CWnd*, bool> _memoWnd;


	std::vector<std::shared_ptr<struct _intersx>> _vInters;
	//10개는 먼가 생성하고 남은 10개는 꽁으로
	
	int ii = 0;
	for (ii; ii < 5; ii++)
	{
		auto& at = _vInters.emplace_back(std::move(std::make_shared<struct _intersx>()));
	
	}

	for (ii; ii < 10; ii++)
	{
		//_vInters.emplace_back(std::move(std::make_shared<struct _intersx>()));
	}


	std::vector<std::shared_ptr<struct _intersx>> _arrBaseInters;
	std::vector<std::shared_ptr<struct _intersx>> vInters;
	std::pair<int, std::vector<std::shared_ptr<struct _intersx>>> WriteFileSumtoEachGroup(UINT curruntGroup);


	/*std::vector<std::unique_ptr<_efopitem>> _ETFitem;
	std::vector<std::unique_ptr<_efoptema>> _ETFtema;
	std::vector<std::unique_ptr<_efopfore>> _ETFfore;
	std::vector<std::unique_ptr<_efopcode>> _ETFcode;*/
}


void CDlgSTD::OnBnClickedChkVm()
{
	CheckVectorMap();
}
