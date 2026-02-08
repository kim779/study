#include "DPhonePad.h"
#include "stdafx.h"
#include "CX_NATINGTEL.h"
#include "MainWnd.h"
#include <atlbase.h>
/////////////////////////////////////////////////////////////////////////////
// CMainWnd

void DoEvents(int nMilliSec)
{
	MSG	msg{};
	
#if defined(_WINDOWS)
	
	const	int	nLoop = nMilliSec / 10;
	for (int i = 0; i <= nLoop; i++)
	{
		for (int n = 0; n < 16 && PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ); n++)	//이녀석이 10 MilliSec 정도 대기하는거 같음
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			i++;
		}
		
		if (nMilliSec > 0)
			Sleep(1);
	}
	
#endif
}

CMainWnd::CMainWnd(CWnd* parent)
{
	EnableAutomation();

	m_pWizard = parent;
	m_instance = nullptr;
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_FUNCTION(CMainWnd, "Request", Request, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "Load", Load, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "ExitPhonePad", ExitPhonePad, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "StopRequest", StopRequest, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetStatus", GetStatus, VT_I2, VTS_NONE)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CMainWnd, "SendMSgToMain", dispidSendMSgToMain, SendMSgToMain, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "KillProcess", dispidKillProcess, KillProcess, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {9159A58B-E75F-42F8-9CB3-F97F543BF749}
static const IID IID_IMainWnd =
{ 0x9159a58b, 0xe75f, 0x42f8, { 0x9c, 0xb3, 0xf9, 0x7f, 0x54, 0x3b, 0xf7, 0x49 } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

CString bstrToCString(BSTR bstr)
{
	USES_CONVERSION;		//convert를 위한 매크로

	char*	pbstr = nullptr;
	pbstr = OLE2A(bstr);		//bstr을 char로 convert
	return CString(pbstr);
} 

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BSTR CMainWnd::Request() 
{
	CString strResult;
	CString string;
	//const	short	nSinarioNum = 12, nTerminated = 1;
	const	short	nSinarioNum = 6, nTerminated = 1;
	short	nRtn = 0;
	
	if (m_instance == nullptr)
		return strResult.AllocSysString();

	const	int	nState = GetState();

	switch (nState)
	{
	case 2:
		string.Format("Set_Msg\t%s", "폰패드가 준비되지 않았습니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;

	case 3:
		string.Format("Set_Msg\t%s", "Call Man 드라이버가 설치되지 않았습니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;

	case 4:
		string.Format("Set_Msg\t%s", "Call Man 시나리오 프로그램이 실행되지 않았습니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;

	case 5:
		string.Format("Set_Msg\t%s", "이전 명령이 실행중입니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;

	case 9:
		string.Format("Set_Msg\t%s", "기타 이유로 실행할 수 없습니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;

	default:break;
	}

	if (nState != 1)
		return strResult.AllocSysString();
	
	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	//BSTR CallRequestPasswdFuncB(short nCntDigits, short nTerminated, short nSinarioNum, short nTimeout, short &nRtn)
	//설명 : 비밀번호 요청(응답이 리턴되기까지 기다림)
	//인자 : nCntDigits - 비밀번호 자릿수, 종료문자(*, #) 
	//		 nTerminated - 종료문자(*) 입력 여부(1 : 입력, 0 : 입력안함)
	//		 nSinarioNum - 시나리오 번호
	//		 nTimeout - 입력 대기 초
	//		 nRtn - 실행 여부(1: 정상, 0 : 비정상)
	//리턴값 : 메세지(비밀번호, 입력 결과 등)
	//-----------------------------------------------------------------------------------------------------------------------------------------------------
	
	typedef BSTR (__stdcall *CallRequestPasswdFuncB)(short, short, short, short, short *); 
	CallRequestPasswdFuncB pCallRequestPasswdFuncB;
	
	VERIFY(pCallRequestPasswdFuncB = (CallRequestPasswdFuncB)GetProcAddress(m_instance, "CallRequestPasswdFuncB"));

	if (pCallRequestPasswdFuncB == NULL)
	{
		//AfxMessageBox("CallRequestPasswdFuncB 함수 호출 실패");
		string.Format("Set_Msg\t%s", "CallRequestPasswdFuncB 함수 호출 실패");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		return strResult.AllocSysString();
	}
	
	//시나리오번호(6 : 비밀번호, 7 : 계좌번호, 1 : 주민번호, 2 : 테스트)
	
	//종료문자(*, #) 입력 여부(1 : 입력, 0 : 입력안함)

	string.Format("Set_Msg\t%s", "비밀번호 입력을 대기중이니 잠시만 기다려주십시요.");
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
	
	BSTR	bstrMsg = (pCallRequestPasswdFuncB)(4, nTerminated, nSinarioNum, 10, &nRtn);
	if (nRtn)
	{
		strResult = bstrToCString(bstrMsg);

		string.Format("Set_Msg\t%s", "비밀번호 입력에 성공했습니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
	}
	else
	{
		//AfxMessageBox("비밀번호 입력에 실패했습니다.");
		string.Format("Set_Msg\t%s", "비밀번호 입력에 실패했습니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
	}

	//AfxMessageBox(strResult);

	return strResult.AllocSysString();
}

int CMainWnd::GetState()
{
	short	nRtn = 0;
	CString strProcess = _T("");
	
	if (m_instance == nullptr)
		return -1;
	
	//-------------------------------------------------------------------------------------------------
	//BSTR CallGetPhonePadStateFuncB(BSTR bstrProcess, short &nRtn)
	//설명 : 폰패드 상태 받기
	//인자 : bstrProcess - 폰패드 시나리오 프로그램명(PhonePad)
	//					   작업관리자에 출력된 EXE명이 짤려있을 수 있으므로 확장자는 제외시킴
	//		 nRtn - 폰패드 상태 응답코드
	//				1 : 정상
	//				2 : DEVICE UNREADY
	//	 			3 : 폰패드 드라이버 설치 무	
	//				4 : 폰패드 시나리오 프로그램 실행중이 아님
	//	 			9 : 기타
	//리턴값 : 메세지
	//-------------------------------------------------------------------------------------------------
	
	typedef BSTR (__stdcall *CallGetPhonePadStateFuncB)(BSTR, short *); 
	CallGetPhonePadStateFuncB pCallGetPhonePadStateFuncB;
	
	VERIFY(pCallGetPhonePadStateFuncB = (CallGetPhonePadStateFuncB)GetProcAddress(m_instance, "CallGetPhonePadStateFuncB"));
	CString string;

	if (pCallGetPhonePadStateFuncB == NULL)
	{
		//AfxMessageBox("CallGetPhonePadStateFuncB 함수 호출 실패");
		string.Format("Set_Msg\t%s", "CallGetPhonePadStateFuncB 함수 호출 실패");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		return -1;
	}
	
	strProcess = "PhonePad";
	BSTR bstrMsg = (pCallGetPhonePadStateFuncB)(strProcess.AllocSysString(), &nRtn);

	return nRtn;
}

void CMainWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
// 	short nRtn = 0;
// 
// 	typedef BSTR (__stdcall *CallStopPasswdFuncB)(short *);
// 	CallStopPasswdFuncB pCallStopPasswdFuncB;
// 	
// 	VERIFY(pCallStopPasswdFuncB = (CallStopPasswdFuncB)GetProcAddress(m_instance, "CallStopPasswdFuncB"));
// 	
// 	BSTR bstrStop = (pCallStopPasswdFuncB)(&nRtn);
// 
// 	if (nRtn == 1)
// 	{
// 		typedef BSTR (__stdcall *CallExitPhonePadFuncB)(BSTR, short *); 
// 		CallExitPhonePadFuncB pCallExitPhonePadFuncB;
// 		
// 		VERIFY(pCallExitPhonePadFuncB = (CallExitPhonePadFuncB)GetProcAddress(m_instance, "CallExitPhonePadFuncB"));
// 		
// 		if (pCallExitPhonePadFuncB == NULL) {
// 			//AfxMessageBox("CallExitPhonePadFuncB 함수 호출 실패");
// 			return;
// 		}
// 		
// 		CString strProcess = "PhonePad";
// 		BSTR bstrMsg = (pCallExitPhonePadFuncB)(strProcess.AllocSysString(), &nRtn);
// 		
// 		if (m_instance)
// 		{
// 			FreeLibrary(m_instance);
// 			m_instance = NULL;
// 		}
// 	}
}

BOOL CMainWnd::Load() 
{
	CString string;

	m_PhonePadPath = _T("C:\\PhonePad\\PhonePad.exe");

	//원래 DPhonepad.dll이지만 기존에 아주 오랜 옛날 버젼이 Dphonepad.dll이 이미 올라가 있어
	//자꾸 exe 폴더에 있는 구버젼을 로드함.그래서 임의로 DXPHONEPAD.DLL로 이름변경
	//C:\\Program Files (x86)\\IBKS_STAFF\\IBK hot Trading(직원용)\\dev\\DXphonepad.dll
	m_instance = LoadLibrary("DXphonepad.dll");
	if (m_instance == nullptr)
	{
		//AfxMessageBox("DPHONEPAD.DLL 로드에 실패했습니다.");
		string.Format("Set_Msg\t%s", "DPHONEPAD.DLL 로드에 실패했습니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		return FALSE;
	}

	//-------------------------------------------------------------------------------------------------
	//BSTR CallSetSiteCodeFuncB(BSTR bstrSiteCode, short &nRtn)
	//설명 : 사이트코드 설정
	//인자 : bstrSiteCode - 사이트코드(TYS)
	//		 nRtn - 설정 결과(1: 정상, 0 : 비정상)
	//리턴값 : 설정 결과 메세지
	//-------------------------------------------------------------------------------------------------
	short	nRtn = 0;
	CString strSiteCode = "TYS";

	typedef BSTR (__stdcall *CallSetSiteCodeFuncB)(BSTR, short *);
	CallSetSiteCodeFuncB pCallSetSiteCodeFuncB;
	
	VERIFY(pCallSetSiteCodeFuncB = (CallSetSiteCodeFuncB)GetProcAddress(m_instance, "CallSetSiteCodeFuncB"));
	
	if (pCallSetSiteCodeFuncB == NULL)
	{
		//AfxMessageBox("CallSetSiteCodeFuncB 함수 호출 실패");
		string.Format("Set_Msg\t%s", "CallSetSiteCodeFuncB 함수 호출 실패");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		return FALSE;
	}
	
	BSTR	bstrMsg = (pCallSetSiteCodeFuncB)(strSiteCode.AllocSysString(), &nRtn);
	if (nRtn == 0)
	{
		//AfxMessageBox("네이팅텔 폰패드 사이트코드 설정에 실패 했습니다.");
		string.Format("Set_Msg\t%s", "네이팅텔 폰패드 사이트코드 설정에 실패 했습니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		return FALSE;
	}
	
	typedef BSTR (__stdcall *CallExecPhonePadFuncB)(BSTR, short *);
	CallExecPhonePadFuncB pCallExecPhonePadFuncB;
	
	VERIFY(pCallExecPhonePadFuncB = (CallExecPhonePadFuncB)GetProcAddress(m_instance, "CallExecPhonePadFuncB"));
	
	if (pCallExecPhonePadFuncB == NULL)
	{
		//AfxMessageBox("CallExecPhonePadFuncB 함수 호출 실패");
		string.Format("Set_Msg\t%s", "CallExecPhonePadFuncB 함수 호출 실패");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		return FALSE;
	}
	
	bstrMsg = (pCallExecPhonePadFuncB)(m_PhonePadPath.AllocSysString(), &nRtn);
	
	switch (nRtn)
	{
	case 1:
		string.Format("Set_Msg\t%s", "폰패드가 준비 되었습니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;
	case 2:
		//AfxMessageBox("CALL MAN 드라이버 설치가 되어있지 않습니다.");
		string.Format("Set_Msg\t%s", "CALL MAN 드라이버 설치가 되어있지 않습니다.2");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;
	case 3:
		//AfxMessageBox("CALL MAN 프로그램이 존재하지 않습니다.");
		string.Format("Set_Msg\t%s", "CALL MAN 프로그램이 존재하지 않습니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;
	case 4:
		//AfxMessageBox("CALL MAN 프로그램이 실행에 실패했습니다.");
		string.Format("Set_Msg\t%s", "CALL MAN 프로그램이 실행에 실패했습니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
		break;
	default:break;
	}

	return (nRtn == 1) ? TRUE : FALSE;
}

void CMainWnd::OnClose() 
{	
	CWnd::OnClose();
}

void CMainWnd::ExitPhonePad() 
{
	short	nRtn = 0;

	if (GetState() == 5)
	{
		typedef BSTR (__stdcall *CallStopPasswdFuncB)(short *);
		CallStopPasswdFuncB pCallStopPasswdFuncB;
		
		VERIFY(pCallStopPasswdFuncB = (CallStopPasswdFuncB)GetProcAddress(m_instance, "CallStopPasswdFuncB"));
		
		BSTR bstrStop = (pCallStopPasswdFuncB)(&nRtn);

		DoEvents(1100);
	}

	typedef BSTR (__stdcall *CallExitPhonePadFuncB)(BSTR, short *); 
	CallExitPhonePadFuncB pCallExitPhonePadFuncB;
	
	VERIFY(pCallExitPhonePadFuncB = (CallExitPhonePadFuncB)GetProcAddress(m_instance, "CallExitPhonePadFuncB"));
	
	if (pCallExitPhonePadFuncB == nullptr)
	{
		AfxMessageBox("CallExitPhonePadFuncB 함수 호출 실패");
		return;
	}
	
	CString strProcess = "PhonePad";
	BSTR bstrMsg = (pCallExitPhonePadFuncB)(strProcess.AllocSysString(), &nRtn);
	
	DoEvents(1100);
	
	if (m_instance)
	{
		FreeLibrary(m_instance);
		m_instance = nullptr;
	}
}

void CMainWnd::StopRequest() 
{
	short	nRtn = 0;
	CString string;
	
	typedef BSTR (__stdcall *CallStopPasswdFuncB)(short *);
	CallStopPasswdFuncB pCallStopPasswdFuncB;
	
	string.Format("Set_Msg\t%s", "인증요청의 중단을 요청합니다.");
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());

	VERIFY(pCallStopPasswdFuncB = (CallStopPasswdFuncB)GetProcAddress(m_instance, "CallStopPasswdFuncB"));
	
	BSTR	bstrStop = (pCallStopPasswdFuncB)(&nRtn);
	if (nRtn == 1)
	{
		string.Format("Set_Msg\t%s", "인증요청을 중단하였습니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
	}
	else
	{
		string.Format("Set_Msg\t%s", "인증요청의 중단에 실패했습니다.");
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());
	}
}

short CMainWnd::GetStatus() 
{
	return GetState();
}

CString CMainWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

void CMainWnd::SendMSgToMain(BSTR strPass)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString stmp, stemp;
	stmp.Format("%s", strPass);

	stemp.Format("[Nating Tell] %s\n", stmp);
	OutputDebugString(stemp);

	CString title;

	CString strdata, name;
	name = Variant(nameCC, "");

	int	readL = 0;
	char	readB[1024];
	CString userD;
	userD.Format("%s\\%s\\%s\\%s.ini", Variant(homeCC, ""), "user", name, name);
	readL = GetPrivateProfileString("MODE", "main", "", readB, sizeof(readB), userD);

	if (readL <= 0)
		return ;

	CString sWnd;
	sWnd.Format("%s", readB);
	int ddata = atoi(sWnd);
	HWND hWnd = (HWND)ddata;
	CWnd* pwnd = CWnd::FromHandle(hWnd);

	bool bret = pwnd->SendMessage(WM_USER + 0x7001, 0, (LPARAM)stmp.operator LPCTSTR());
	stmp.Format("[%d][%x] %s", bret, pwnd, userD);
}

#include <TlHelp32.h>
void CMainWnd::KillProcess(BSTR strProcess)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString processname;
	processname.Format("%s", strProcess);
	processname.TrimRight();

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe32 = { 0, };

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		OutputDebugString(_T("Error checking process"));
		CloseHandle(hProcessSnap);
		return ;
	}

	do {
		CString progS(pe32.szExeFile);

		if (progS.Find(processname) > -1)
		{
		//	if (pe32.th32ProcessID == processId)
			{
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);

				if (hProcess)
				{
					TerminateProcess(hProcess, -1);
					CloseHandle(hProcess);

					return ;
				}
			}
		}
	} while (Process32Next(hProcessSnap, &pe32));


	CloseHandle(hProcessSnap);

	return;
}
