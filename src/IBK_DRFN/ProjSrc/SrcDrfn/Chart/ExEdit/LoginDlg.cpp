// LoginDlg.cpp : implementation file
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

//****************************************************************************
// INCLUDE HAEDER
//****************************************************************************
#include "stdafx.h"
#include <imm.h>
#include <direct.h>
#include "LoginDlg.h"
#include "UserInfo.h"
#include "WndVersion.h"	
#include "../ToolKit/Registry/Registry.h"

//****************************************************************************
// DEFINE MACRO
//****************************************************************************
#define			FILE_IPDAT			"\\Data\\IP.dat"
#define			FILE_MAININI		"\\Data\\Main.ini"
#define			IMG_LOGGIN		    "\\Login_building.bmp"

#define			TYPE_STAFF			-1	// WinnetPro
#define			TYPE_CLIENT			 2	// WinnetProMini
#define			TYPE_JANGOJOHO		-2	// 잔고조회기
#define			TYPE_EXTRA			-3
#define			TYPE_UNKNOW			 0	//Unknown

// 로그인 종류
#define		    NORMAL_INPUT		0x50			// 일반적인 로그인
#define			NEWREG_INPUT		0x58			// 신규 가입고객
#define			GUEST_INPUT			0x57			// guest
#define			SISEONLY_INPUT		0x59			// 시세전용

//****************************************************************************
// REGISTER MESSAGE
//****************************************************************************
const UINT RMSG_KEYMESSAGE = ::RegisterWindowMessage("RMSG_KEYMESSAGE");

//****************************************************************************
// GLOBAL FUNCTION
//****************************************************************************
int gGetProgrameType()
{
	CRegistry registry;
	registry.SetRootKey(HKEY_CURRENT_USER);
	registry.SetKey("Software\\Dongbu Securities\\WinNetNew\\Setup", FALSE);

	CString m_szUserType = registry.ReadString("Type", "Client");

	if(m_szUserType.CompareNoCase("Staff")==0)		return TYPE_STAFF;
	if(m_szUserType.CompareNoCase("Client")==0)		return TYPE_CLIENT;
	if(m_szUserType.CompareNoCase("JangoJoho")==0)	return TYPE_JANGOJOHO;
	if(m_szUserType.CompareNoCase("Extra")==0)		return TYPE_EXTRA;
	return TYPE_UNKNOW;
}


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


CLoginDlg::CLoginDlg(HWND hSockDlg, void* pBuffer, void* pUserInfo, LPCTSTR szRootPath, int nSetupType,CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent),
	m_hSocket(NULL),
	m_bVerCheckCompleted(FALSE)
{
	//{{AFX_DATA_INIT(CLoginDlg)
	m_szID		= _T("");
	m_szPassWd	= _T("");
	//}}AFX_DATA_INIT

//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nSetupType = nSetupType;
	m_szRootPath = szRootPath;

	m_boldFont.CreatePointFont (90,_T("굴림체"));
	LOGFONT logFont;
	m_boldFont.GetLogFont (&logFont);
	m_boldFont.DeleteObject();

	logFont.lfWeight = FW_BOLD;
	m_boldFont.CreateFontIndirect(&logFont);


	m_szIni.Format("%s%s%s", m_szRootPath, _T("\\Data\\"), DEFMAININI);
	m_szDat.Format("%s%s%s", m_szRootPath, _T("\\Data\\"), "main.dat");
	m_szLoginFile = _T("");
	m_szCmd = _T("general");

	// {{ 파라미터를 체크한다. 
	char chBuf[7+28+1] = {0,};
	char chTmp[30] = {0,};
	LPSTR lpUser = (LPSTR)pUserInfo;
	memcpy(chBuf, lpUser, 36);
	
	memcpy(chTmp, chBuf, 7);

	if (strcmp(chTmp, "noversion")==0)
	{
		m_szCmd = _T("noversion");
		m_bVerCheckCompleted = TRUE;
	}
	else if((strcmp(chTmp, "version")==0) || (strcmp(chTmp, "restart")==0))
	{	// 버전 또는 재시작에 의해 실행 
		if (strcmp(chTmp, "version")==0)		m_szCmd = _T("version");
		else									m_szCmd = _T("restart");
		memset(chTmp, 0, 30);
		memcpy(chTmp, chBuf+7, 20);
		m_szID = chTmp;
		m_szID.TrimRight();
		memset(chTmp, 0, 30);
		memcpy(chTmp, chBuf+27, 8);
		m_szPassWd = chTmp;
		m_szPassWd.TrimRight();
	}
	else if (strcmp(chTmp, "noversi")==0)
	{
		m_szCmd = _T("noversi");
		m_bVerCheckCompleted = TRUE;
	}

//	FindStyle();

	m_hSocketDlg = hSockDlg;
	m_pBuffer    = pBuffer;
	
	m_bUseCert   = TRUE;
	m_bSiseOnly  = FALSE;
	m_bAutoPass  = FALSE;
	m_bIDSave	 = FALSE;

	m_nIDType		= -1;
	m_nUserGB		= 0;
	m_nMethod		= 0;
	m_nAuthSkip		= 0;
	m_nProgrameType = 0;
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Text(pDX, IDC_EDT_LOGINID, m_szID);
	DDX_Text(pDX, IDC_EDT_PASSWD,  m_szPassWd);
	DDX_Control(pDX, IDC_BTN_SETTING, m_btnSetting );
	DDX_Control(pDX, IDOK,			  m_btnConnect);
	DDX_Control(pDX, IDCANCEL,		  m_btnExit );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_WM_COPYDATA()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	SetImeMode(IME_CMODE_ROMAN);

	LoadImages();
	MoveCtrls();

/*
	LRESULT lResult = InstallKeyHook();
	if (lResult != KH_OK)
	{
		AfxMessageBox("초기화에 실패하였습니다.");
		return TRUE;
	}

	m_entry.nMessage = RMSG_KEYMESSAGE; // Our message ID
	m_entry.hCallWnd = m_hWnd; // Send message to this window
	m_entry.hHookWnd = 0; // Capture key-strokes occurred in any windows
	m_entry.iCombKeys = 0; // combination keys don't matter
	m_entry.iIndicators = 0; // Caps-lock, Num-lock, Scroll-lock on/off states do not matter
	m_entry.iKeyEvent = 0; // Capture all key events
	m_entry.iMinVKCode = 0x00; // Capture all keys
	m_entry.iMaxVKCode = 0xff;

	// Add the entry to the hook
	lResult = AddKeyEntry(&m_entry);
	if (lResult != KH_OK)
	{
		AfxMessageBox("등록에 실패하였습니다.");
		return TRUE;
	}
*/

	if(!LoadDlgData()) {
		EndDialog(IDCANCEL);
		return FALSE;
	}

	InitControl();

	if(m_szCmd.CompareNoCase("Version")==0) {	// 버전처리된 경우엔 로긴화면을 다시 보여주지 않음 
		ShowWindow(SW_MINIMIZE);	
		OnConnectVersion();
	}
	else if (m_szCmd.CompareNoCase("Restart")==0)
	{
		ShowWindow(SW_MINIMIZE);	
		OnConnect();	
	}

	GetDlgItem(IDC_EDT_LOGINID)->SetWindowText ( m_szRealID );
	GetDlgItem(IDC_EDT_PASSWD )->SetFocus();
	GetDlgItem(IDC_EDT_CERTPWD)->EnableWindow (FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CLoginDlg::SetImeMode(DWORD mode)
{
	//수정내용 : 로그인시 한글 입력 모드를 영문으로 변경
	HIMC hIMC;
	DWORD dwConv, dwSent;
	DWORD dwTemp;

	hIMC = ImmGetContext(GetSafeHwnd());
	ImmSetOpenStatus(hIMC, TRUE);
	ImmGetConversionStatus(hIMC, &dwConv, &dwSent);
	dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
	dwTemp |= mode;

	dwConv = dwTemp;
	ImmSetConversionStatus(hIMC, dwConv, dwSent);
	ImmReleaseContext(GetSafeHwnd(), hIMC);
}


BOOL CLoginDlg::LoadDlgData()
{
	CFileFind find;
	if(find.FindFile(m_szIni)==FALSE) {
		CString szErr;
		szErr.Format("실행에 필요한 파일[%s]을 찾을 수 없습니다.", m_szIni);
		AfxMessageBox(szErr);
		return FALSE;
	}

	m_nMethod = ::GetPrivateProfileInt("CONNECT", "METHOD", 0, m_szIni);

	if(m_szRealID.GetLength()==0) {			// 버전에 의해 실행된 경우 ID를 다시 얻을 필요가 없다. 
		CString szPath;
		szPath.Format("%s%s", m_szRootPath, _T("\\Data\\"));
		CUserInfo userInfo(szPath);
		userInfo.GetUserID(m_szRealID);
	}

	::GetPrivateProfileString("CALL", "LOGINMSG",
		"이용자 ID와 비밀번호를 입력하시기 바랍니다.", 
		m_szMessage.GetBuffer(MAX_PATH),
		MAX_PATH ,
		m_szDat);
	m_szMessage.ReleaseBuffer();

	CString szIPData;
	szIPData.Format("%s\\Data\\IP.dat", m_szRootPath);
	m_nAuthSkip = GetPrivateProfileInt("OPTION", "AUTH", 0, szIPData);

	return TRUE;
}


void CLoginDlg::OnConnect() 
{
	UpdateData(TRUE);

	int nTestMode = ::GetPrivateProfileInt("DEBUG", "TestMode", 0, m_szIni);
	if(nTestMode==111)
	{
//		BTNSTATE(FALSE);
		m_bVerCheckCompleted=TRUE;
		TestSocket();
		TestMode();
		return;
	}

	// 사용자의 입력을 체크한다. 아이디, 비밀번호
	if(!CheckAndSaveUserinfo()) 
	{
		return;
	}
	
	// 버튼 상태를 계속 보이게 한다.
//	BTNSTATE(FALSE);
		
	if(m_nMethod==1) 
	{ // 전화접속인경우
		return;
	}

	SetConnectServer();
	
	CString tmp_IsVerPath;
	tmp_IsVerPath.Format("%s%s%s", m_szRootPath, _T("\\Data\\"), _T("main.ini"));
	int tmp_IsVer = ::GetPrivateProfileInt(_T("CONFIG"), _T("VERSION"), 1, tmp_IsVerPath);
	if (tmp_IsVer != 0)
	{
		if(VersionCheck()==VER_OK) {
			m_bVerCheckCompleted=TRUE;
			CreateSocket();
			SetupUserFile();
		}
	}
	else
	{
		m_bVerCheckCompleted=TRUE;
		CreateSocket();
		SetupUserFile();
	}

	WriteLoginInfo();
//	BTNSTATE(TRUE);
}

void CLoginDlg::TestSocket()
{
	char buffer[28] = {0,};
	memcpy(buffer, m_szRealID, 20);
	memcpy(buffer+20, m_szPassWd, 8);
	
	LRESULT lRet = ::SendMessage(m_hSocketDlg, UDM_SOCK_CREATE, 0, (LPARAM)(LPSTR)buffer);
		
	char chRet[6] = {0,};
	memcpy(chRet, (LPSTR)(LPVOID)lRet, 6);
	CString szRet = chRet;
	if(szRet.GetLength()==0)	{ return; }
	
	CString szTemp = szRet.Left(1);
	int nResult =  atoi(szTemp);
	if(nResult==1) {
		szRet.Delete(0);
		HWND hSock = (HWND)(atol((LPCTSTR)szRet));
	}
}

void CLoginDlg::TestMode()
{
	char aID[20]={0,},
		aPasswd[9]={0,},
		aLevel[33]={0,},
		aIP[15]={0,},
		aGrade[4]={0,},
		aAcctList[200]={0,};
	int nAcctCount=0;

	::GetPrivateProfileString("TestMode", "ID", "tuja", aID, sizeof(aID), m_szIni);
	::GetPrivateProfileString("TestMode", "PASSWD", "tuja", aPasswd, sizeof(aPasswd), m_szIni);
	::GetPrivateProfileString("TestMode", "LEVEL", "", aLevel, sizeof(aLevel), m_szIni);
	::GetPrivateProfileString("CONNECT", "IP2", "", aIP, sizeof(aIP), m_szIni);
	::GetPrivateProfileString("TestMode", "GRADE", "", aGrade, sizeof(aGrade), m_szIni);
	::GetPrivateProfileString("TestMode", "ACCTLIST", "", aAcctList, sizeof(aAcctList), m_szIni);
	nAcctCount = ::GetPrivateProfileInt("TestMode", "ACCTCOUNT", 0, m_szIni);

	int nSize = nAcctCount * GEJA_SIZE;
	int nTotal = sizeof(STUSERINFO) + nSize +10;
	char*  LoginList   = new char[nTotal];
	STUSERINFO* pUInfo = (STUSERINFO*)LoginList;

	memset(pUInfo, 0x00, nTotal);
	memcpy(pUInfo->ID, aID, min(strlen(aID), 20));
	memcpy(pUInfo->Passwd, aPasswd, min(strlen(aPasswd), 8));
	memcpy(pUInfo->Level, aLevel, 32);  // Level 부터 AccCount까지 메모리복사 
	sprintf(pUInfo->AccCount, "%03.3d", nAcctCount);
	memcpy(pUInfo->AccList, aAcctList, nSize);	
	memcpy((LPSTR)m_pBuffer, pUInfo, nTotal);
	delete pUInfo;

	EndDialog(IDOK);
}

BOOL CLoginDlg::CheckAndSaveUserinfo()
{
	UpdateData(TRUE);

	// Temp
/*
	BOOL bHTSGuest = IsDlgButtonChecked(IDC_CHECK_NEWREG);

	CString szMsg;
	CString szID = m_szRealID;
	if(szID.GetLength()==0) {
		GetDlgItem(IDC_EDT_LOGINID)->SetFocus();
		if(bHTSGuest)
			ShowBallonTip(IDC_ID, _T("계좌번호를 입력하세요"));
		else
			ShowBallonTip(IDC_ID, _T("아이디를 입력하세요"));
		return FALSE;
	}
	else if(m_szPassWd.GetLength()==0) {
		if(bHTSGuest==FALSE)
		{
			if(szID.CompareNoCase(STRING_GUEST)!=0)
			{
				GetDlgItem(IDC_PASSWD)->SetFocus();
				ShowBallonTip(IDC_PASSWD, _T("비밀번호를 입력하세요"));
				return FALSE;
			}
		}
	}

*/
	CString szLoginID;
	GetDlgItem(IDC_EDT_LOGINID)->GetWindowText(szLoginID);

	szLoginID.TrimLeft();
	szLoginID.TrimRight();
	if ( szLoginID.IsEmpty() )
	{
		AfxMessageBox("로긴 아이디를 입력해 주십시요.");
		return FALSE;
	}
	else if ( szLoginID != m_szRealID )
	{
		m_szRealID = szLoginID;
	}

	CString szPath;
	szPath.Format("%s%s", m_szRootPath, _T("\\Data\\"));
	CUserInfo userInfo(szPath);
	userInfo.SetUserID(m_szRealID);

	return TRUE;
}


void CLoginDlg::SetConnectServer()
{
	CString szIPDat, szMainIni, szIP, szPort, szVPort, szIPB;	
	CString szSection;
	szIPDat		= m_szRootPath + FILE_IPDAT;
	szMainIni	= m_szRootPath + FILE_MAININI;

	int nLanType = GetPrivateProfileInt("CONNECT", "LANTYPE", 0, szMainIni);

	if (m_nSetupType == 0)
	{
		int nUserGB = GetPrivateProfileInt("CONNECT", "USERGB", 0, szMainIni);

		if (nUserGB == 0)
		{
			m_nUserGB = nUserGB;

			//Temp
//			int nIDType = ((CButton*)GetDlgItem(IDC_CHECK_HTSID))->GetCheck();
			int nIDType = 0;
			if (nIDType == 0)
			{
				m_nIDType = 1;
				m_bUseCert = FALSE;
				m_bAutoPass = FALSE;
				m_bSiseOnly = FALSE;
				if (nLanType == 0)
				{
					szSection = "WinTop_직원(사내)";
				}
				else if (nLanType == 1)
				{
					szSection = "WinTop_직원(데이터센터)";
				}
				else if (nLanType == 2)
				{
					szSection = "WinTop_시세(외부)";
				}
				else 
				{
					szSection = "WinTop_직원(사내)";
				}
				//szSection = "WinTop_직원(사내)";
			}
			else
			{
				m_nIDType = 2;
				m_bUseCert = TRUE;
				m_bAutoPass = TRUE;
				m_bSiseOnly = FALSE;
				szSection = "WinNet_사이버룸";
				if (nLanType == 0)
				{
					szSection = "WinNet_사이버룸";
				}
				else if (nLanType == 1)
				{
					szSection = "WinNet_사이버룸(데이터센터)";
				}
				else if (nLanType == 2)
				{
					szSection = "WinNet_사이버룸(데이터센터)";
				}
				else
				{
					szSection = "WinNet_사이버룸";
				}

			}
		}
		//else if (szUserGB.CompareNoCase("1") == 0)
		else if (nUserGB == 1)
		{
			m_nUserGB = nUserGB;
			m_nIDType = 2;
			m_bUseCert = TRUE;
			m_bAutoPass = TRUE;
			m_bSiseOnly = TRUE;
			szSection = "WinNet_사이버룸";
			if (nLanType == 0)
			{
				szSection = "WinNet_사이버룸";
			}
			else if (nLanType == 1)
			{
				szSection = "WinNet_사이버룸(데이터센터)";
			}
			else if (nLanType == 2)
			{
				szSection = "WinNet_사이버룸(데이터센터)";
			}
			else
			{
				szSection = "WinNet_사이버룸";
			}
		}
		//else if (szUserGB.CompareNoCase("2") == 0)
		else if (nUserGB == 2)
		{
			m_nUserGB = nUserGB;
			m_nIDType = 1;
			m_bUseCert = FALSE;
			m_bAutoPass = FALSE;
			m_bSiseOnly = TRUE;
			szSection = "WinTop_시세(외부)";
			if (nLanType == 0)
			{
				szSection = "WinTop_시세(외부)";
			}
			else if (nLanType == 1)
			{
				szSection = "WinTop_직원(데이터센터)";
			}
			else if (nLanType == 2)
			{
				szSection = "WinTop_시세(외부)";
			}
			else
			{
				szSection = "WinTop_시세(외부)";
			}
		}
	}
	else if (m_nSetupType == 1)
	{
		m_nIDType = 2;
		szSection = "WinNet_우수고객";
		if (nLanType == 0)
		{
			szSection = "WinNet_우수고객";
		}
		else if (nLanType == 1)
		{
			szSection = "WinNet_우수고객(데이터센터)";
		}
		else
		{
			szSection = "WinNet_우수고객";
		}
	}
	else if (m_nSetupType == 2)
	{
		m_nUserGB = 0;
		m_nIDType = 1;
		m_bUseCert = FALSE;
		m_bAutoPass = FALSE;
		m_bSiseOnly = FALSE;
		szSection = "WinTop_직원(사내)";
		if (nLanType == 0)
		{
			szSection = "WinTop_직원(사내)";
		}
		else if (nLanType == 1)
		{
			szSection = "WinTop_직원(데이터센터)";
		}
		else if (nLanType == 2)
		{
			szSection = "WinTop_시세(외부)";
		}
		else
		{
			szSection = "WinTop_직원(데이터센터)";
		}
	}

	GetPrivateProfileString(szSection, "IP", "", szIP.GetBuffer(MAX_PATH), MAX_PATH, szIPDat);
	szIP.ReleaseBuffer();
	GetPrivateProfileString(szSection, "IPB", "", szIPB.GetBuffer(MAX_PATH), MAX_PATH, szIPDat);
	szIPB.ReleaseBuffer();
	GetPrivateProfileString(szSection, "PORT01", "", szPort.GetBuffer(MAX_PATH), MAX_PATH, szIPDat);
	szPort.ReleaseBuffer();
	GetPrivateProfileString(szSection, "VPORT01", "", szVPort.GetBuffer(MAX_PATH), MAX_PATH, szIPDat);
	szVPort.ReleaseBuffer();

	szSection = "CONNECT";
	WritePrivateProfileString(szSection, "IP1", szIP, szMainIni);
	WritePrivateProfileString(szSection, "IP2", szIP, szMainIni);
	WritePrivateProfileString(szSection, "IPB", szIPB, szMainIni);
	WritePrivateProfileString(szSection, "PORT", szPort, szMainIni);
	WritePrivateProfileString(szSection, "VPORT", szVPort, szMainIni);
}


int CLoginDlg::VersionCheck()
{
	int nChk = ::GetPrivateProfileInt("DH_DEBUG", "VERSION", 0, "HANARO.INI");
	if(nChk==100)
		return VER_OK;

#ifdef _DEBUG
	return VER_OK;
#endif

	if(m_bVerCheckCompleted)
		return VER_OK;

	char buffer[28] = {0,};
	memcpy(buffer, m_szRealID, 20);
	memcpy(buffer+20, m_szPassWd, 8);

	CWndVersion version(this);
	int nResult = version.VersionCheck(buffer);

	switch(nResult) {
		case VER_OK:
			break;
		case VER_READY:
			break;
		case VER_TIMEOUT:
		case VER_EXECERROR:
		case VER_ERROREXIT:
		case VER_USERCANCEL:
			SendMessage(WM_COMMAND, IDCANCEL);
			break;
		case VER_RESTART:
			EndDialog(IDCANCEL);
			break;
	}

	return nResult;
}


void CLoginDlg::CreateSocket()
{
	if(!m_hSocket)
	{
		char buffer[28] = {0,};
		memset(buffer , 0x20, 28);
		memcpy(buffer, m_szRealID, 20);
		memcpy(buffer+20, m_szPassWd, 8);

		LRESULT lRet = ::SendMessage(m_hSocketDlg, UDM_SOCK_CREATE, 0, (LPARAM)(LPSTR)buffer);

		//////////////////////////////////////////////////////////////////////////
		// 소켓 관련 내부 IP / 외부 IP 인지를 체크 한다 (접속 서버 체크)
		CString szClientIP;
		LRESULT lRetGetIP = ::SendMessage(m_hSocketDlg, UDM_GET_IPADDRESS, 0, (LPARAM)&szClientIP);
		if(lRetGetIP)
		{
			if(!ClientIPCheck(szClientIP))
			{
				CString szMSG;
				szMSG.Format("Client IP : %s \r\n환경설정이 잘 못 되었습니다.",szClientIP);
				AfxMessageBox(szMSG);
				::SendMessage(m_hSocketDlg, UDM_SOCK_DISCONNECT, 0, 0);
				return ;
			}
		}

		STSOCKRETURN stRet;
		memcpy(&stRet, (LPVOID)lRet, sizeof(STSOCKRETURN));

		CString szMsg;
		int nResult = stRet.m_nResult;
		if(nResult==SOCKET_OK)					// 정상처리
		{
			m_hSocket = stRet.m_hSock;
			if(!::IsWindow(m_hSocket))
			{
				AfxMessageBox("Socket Error!");
			}
			LoginTRSend(m_hSocket);
		}
		else if(nResult==SOCKET_TIMEOUT)		// 시간초과
		{
			szMsg.LoadString(IDS_SOCKET_TIMEOUT);
			ShowEndMessage(szMsg);
		}
		else if(nResult==SOCKET_SOCKCREATEERR)	// 소켓 생성 오류
		{
			szMsg.LoadString(IDS_SOCKET_SOCKCREATEERR);
			ShowEndMessage(szMsg);
		}
		else if(nResult==SOCKET_CONNECTERRROR)	// 접속 실패
		{
			szMsg.LoadString(IDS_SOCKET_CONNECTERRROR);
			ShowEndMessage(szMsg);
		}
		else if(nResult==SOCKET_RECEIVEQUIT)	// 접속 취소 
		{
			szMsg.LoadString(IDS_SOCKET_RECEIVEQUIT);
			ShowEndMessage(szMsg);
		}
		else									// 기타 오류
		{
			szMsg.LoadString(IDS_SOCKET_UNKNOWNERR);
			ShowEndMessage(szMsg);
		}
	}
	else
	{
		LoginTRSend(m_hSocket);	
	}
}

void CLoginDlg::SetupUserFile()
{
	CString szUserFile;
	szUserFile.Format("%s%s%s", m_szRootPath, _T("\\Data\\"), DEF_USERSELECT);

	int nLastUserType = ::GetPrivateProfileInt("UserType", "LastUserType", 0, szUserFile);
}


void CLoginDlg::WriteLoginInfo()
{
	UpdateData(TRUE);

	CFileFind ffind;
	CString UserFoler;
	UserFoler = m_szRootPath + "\\Usr";
	if(ffind.FindFile(UserFoler)==0)
	{
		_mkdir(UserFoler);
	}

/*
	UserFoler += "\\"+m_szRealID;
	if(ffind.FindFile(UserFoler)==0)
	{
		_mkdir(UserFoler);
	}
*/
	
	char chBuf[3] = {0,};
	
	::WritePrivateProfileString("CONNECT", "ID", m_szRealID, m_szIni);
	_itoa((int)m_bIDSave, chBuf, 10);
	::WritePrivateProfileString("IDSAVE", "SAVE", (LPCTSTR)(LPSTR)chBuf, m_szLoginFile);

	if (m_nSetupType == 1)
	{
/*		temp
		m_bUseCert = ((CButton*)GetDlgItem(IDC_CHECK_USECERT))->GetCheck();
		m_bSiseOnly = ((CButton*)GetDlgItem(IDC_CHECK_SISEONLY))->GetCheck();
		m_bAutoPass = ((CButton*)GetDlgItem(IDC_CHECK_AUTOPASS))->GetCheck();
*/
	}

	::WritePrivateProfileString("CERITIFICATION", "SISEONLY", m_bSiseOnly ? "1" : "0", m_szLoginFile);
	::WritePrivateProfileString("CERITIFICATION", "AUTOPASS", m_bAutoPass ? "1" : "0", m_szLoginFile);

/*   temp
	//Guest일 경우에는 공인인증서부분이 Check가 되어 있더라도 작동되지 않게
	BOOL bHTSGuest = IsDlgButtonChecked(IDC_CHECK_NEWREG);
	if(bHTSGuest>0)
	{
		 m_bUseCert = FALSE;
		 m_bAutoPass = FALSE;
	}
*/
}

//////////////////////////////////////////////////////////////////////////
// 클라이언트의 ip 를 체크하여 현재의 환경설정 사항과 맞지 않으면 소켓을 
// Close 시키고 Return 한다.
// 1. Staff :	-. 사내 인지(L1 or L3) 사외(L4) 인지를 파악한다.
//				-. 사이버룸(L3)인지를 파악한다.
//				-. Hts(L3) / 직원(L1) / 콜센터(L1) 접속인지를 파악한다.
//
// 2. Client :  -. (L2)
//
// 3. Extra  :	-. 사내인경우 ip를 체크하여 자동로그인 한다.(L1)
//				-. 외부의 경우(L4)
//				-. 20040330 수정: Extra 일경우는 Client ip 를 체크하지 않는다.
BOOL CLoginDlg::ClientIPCheck(CString szClientIP)
{

	return TRUE;
	CString szLanType , szUserGB;

	// 접속 타입(L1 ~ L4)
	GetPrivateProfileString("CONNECT", "LAN", "" , szLanType.GetBuffer(50), 50, m_szIni);
	szLanType.ReleaseBuffer();

	// 사용자 구분 (szUserGB : 0(Staff), 1(HTS), 2(Cyber Jijum), 3(CyberRoom), 4(Extra))
	GetPrivateProfileString("CONNECT", "USERGB", "" , szUserGB.GetBuffer(50), 50, m_szIni);
	szUserGB.ReleaseBuffer();

	int nCheckState = GetPrivateProfileInt("CONNECT", "CHECKIP", 1 , m_szIni);

	if(!nCheckState)
		return TRUE;

	CString szIP_A,szIP_B,szIP_C,szIP_D;
	int ipIndex = szClientIP.Find('.');
	szIP_A = szClientIP.Left(ipIndex);
		szClientIP = szClientIP.Mid(ipIndex+1);
		ipIndex	   = szClientIP.Find('.');
	szIP_B = szClientIP.Left(ipIndex);
		szClientIP = szClientIP.Mid(ipIndex+1);
		ipIndex	   = szClientIP.Find('.');
	szIP_C = szClientIP.Left(ipIndex);
		szClientIP = szClientIP.Mid(ipIndex+1);
		//ipIndex    = szClientIP.Find('.');
	szIP_D = szClientIP;
	
	//////////////////////////////////////////////////////////////////////////
	// 내부 IP 인지를 체크한다. 
	// 내부 ip 는 ip가 "192" 로 시작하는 ip로 가정한다.
	BOOL bInnerClient;

#ifdef _BDS_TEST_

		bInnerClient = FALSE;
#else

	if(szIP_A == "192")
		bInnerClient = TRUE;
	else 
		bInnerClient = FALSE;
#endif

	if(m_nProgrameType == TYPE_STAFF)
	{
		if(bInnerClient)	// 사내일 경우
		{
			// 1. 사이버 룸인지를 판단한다.(192.10.xx.40 ~ 69)
			int nIP_D = atoi(szIP_D);
			if(szIP_B == "10" && (nIP_D >= 40 && nIP_D <= 69))
			{
				if(szLanType != "L3")
					return FALSE;
			}
			// 2. 사이버 룸이 아니면 사용자 구분값에 의해 판단한다.
			else
			{
				// 사용자 구분(szUserGB : 0(Staff), 1(HTS), 2(Cyber Jijum), 3(CyberRoom), 4(Extra))
				if(szUserGB == "0" )
				{
					if(szLanType != "L1")
						return FALSE;					
				}
				else if(szUserGB == "1")
				{
					if(szLanType != "L3")
						return FALSE;
				}
				else if(szUserGB == "2")
				{
					if(szLanType != "L1")
						return FALSE;
				}
				else
					return FALSE;

			}
		}
		else				// 사외일 경우
		{
			if(szLanType != "L4")
				return FALSE;
		}
		
	}
	else if(m_nProgrameType ==TYPE_CLIENT )
	{
		if(szLanType != "L2")
			return FALSE;
	}
	else if(m_nProgrameType == TYPE_EXTRA )
	{
		if(bInnerClient)
		{
			// 1. 내부 ip 인경우에는 지점 ip를 체크한다.
			CString szIP_CFirst = szIP_C.Left(1);
			int nIP_D = atoi(szIP_D); 
			if(szIP_B == "10" && szIP_CFirst == "2" && (nIP_D >= 71 && nIP_D <= 129))
			{
				if(szLanType != "L1")
					return FALSE;					
				else
					return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			if(szLanType != "L4")
				return FALSE;	
		}

	}

	return TRUE;
	
}

///////////////////////////////////////////////////////////////////////////////
// 만든이             : 이준옥(Jun-Ok, Lee)
// Email              : alzioyes@hotmail.com
// 만든 날짜          : 2003/2/3
// 함수 이름          : CLoginDlg::LoginTRSend
// 리턴 타입          : void
// 파라미터
//		HWND hSocket   : 
// 함수 설명          : IDC_CHECK_NEWREG 체크되어 있으면 계좌번호를 넘기고
//						체크되어있지 않은 경우에는 "guest"일때와 아닐때를 구분처리한다.
//						
///////////////////////////////////////////////////////////////////////////////
void CLoginDlg::LoginTRSend(HWND hSocket)
{
	CString szID = m_szRealID;
	TRLoginData loginData;
	memset(&loginData, 0x20, TRLoginDataSize);

/*
	// {{ TRLOGINDATA Packet 작성 
	BOOL bHTSGuest = IsDlgButtonChecked(IDC_CHECK_NEWREG);;

	if(bHTSGuest)	// 계좌번호 입력시..
	{
		CheckDlgButton(IDC_CHECK_SISEONLY, 1);
		CheckDlgButton(IDC_CHECK_USECERT, 0);
		szID.Remove('-');
 		loginData.cmd = NEWREG_INPUT;	// 'X'
	}
	else 
	{
		if(szID.CompareNoCase(STRING_GUEST)==0)
		{
			CheckDlgButton(IDC_CHECK_SISEONLY, 1);
			CheckDlgButton(IDC_CHECK_USECERT, 0);
			loginData.cmd = GUEST_INPUT;	// 'W'
		}
		else
		{
			if (IsDlgButtonChecked(IDC_CHECK_SISEONLY))
				loginData.cmd = SISEONLY_INPUT;	
			else
				loginData.cmd = NORMAL_INPUT;	
		}
	}
*/

	// Temp
	loginData.cmd = NORMAL_INPUT;


	memcpy(loginData.userid, szID, szID.GetLength());
	memcpy(loginData.passwd, m_szPassWd, m_szPassWd.GetLength());

	int nUserType = m_nIDType;
	TCHAR cUserType = (nUserType==2)? '2' : '1';
	loginData.userType[0] = cUserType;


	Send2Server(hSocket, "08000", &loginData, TRLoginDataSize);
}

void CLoginDlg::ShowEndMessage(CString _szMsg)
{
	if(_szMsg.GetLength()>0) {
		AfxMessageBox(_szMsg);
		EndDialog(IDCANCEL);	
	}
}

short CLoginDlg::Send2Server(HWND hSock, LPCSTR _szTr, LPVOID szData, int nLen)
{
	if(!hSock) return 0;

	int nSize = sizeof(STTYPE400) + nLen + sizeof(char);

	STTYPE400* pTrData = (STTYPE400*)malloc(nSize+1); 
	memset(pTrData, 0x20, nSize);

	pTrData->bEnc = FALSE;
	pTrData->nLen = nSize;
	pTrData->trHeader.WindowID = (long)m_hWnd;
	memcpy(pTrData->trHeader.TRCode, _szTr, 5);
	memcpy(pTrData->trHeader.MsgCode, "000000", 6);
	memcpy(pTrData->trHeader.Message, "", 0);
	memcpy(pTrData->trHeader.CursorPos, "  ", 2);

	memcpy(pTrData->trHeader.Message2Len, "   ", 3);

	char aTmp[4]={0,};
	sprintf(aTmp, "%-03.3d", 0);
	memcpy(pTrData->trHeader.APDataLen, aTmp, 3);

	if(nLen>0)
		memcpy(pTrData->trHeader.TRData, szData, nLen);
	pTrData->trHeader.TRData[nLen] = 0;

	BOOL bRet = RealSend(hSock, (void*)pTrData, (LPARAM)nSize);

	free(pTrData);

	if(bRet==TRUE) return 0;

	return -5;
}

BOOL CLoginDlg::RealSend(HWND hWnd, LPVOID pData, int nLen)
{
	STTYPE400* prData = (STTYPE400*) pData;

	COPYDATASTRUCT cds;
	cds.dwData = (DWORD) TYPE400;
	cds.cbData = nLen + sizeof(DWORD);
	cds.lpData = prData;

	if(::IsWindow(hWnd)) {
		::SendMessage(hWnd, WM_COPYDATA, 0,(LPARAM)&cds);
	}

	return TRUE;
}

void CLoginDlg::OnOK() 
{
	OnConnect();	

//	CDialog::OnOK();
}

BOOL CLoginDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	STTRHEADER* prData = (STTRHEADER*)pCopyDataStruct->lpData;
	char cType = prData->TRData[0];
//	WriteDebug(prData->TRData);
	if(cType==NULL)
	{
		AfxMessageBox("인증에 실패했습니다.");
		EndDialog(IDCANCEL);
		return CDialog::OnCopyData(pWnd, pCopyDataStruct);
	}

	CString tmp;
	tmp.Format("%c", cType);
	int nType = atoi((LPCTSTR)tmp);

	//switch(nType)
	switch(cType)
	{
	case '0':		// 성공
		{
		STLOGINSUCCESS* pData = (STLOGINSUCCESS*)prData->TRData;
		if (!CheckIPAuth(pData->RecentData))
		{
			return CDialog::OnCopyData(pWnd, pCopyDataStruct);;
		}
		MakeUserData(pData, pCopyDataStruct->dwData);
		EndDialog(IDOK);
		}
		break;

	case '1':		// ID 실패 
		::MessageBox(NULL, "등록되지 않은 ID 입니다. 다시 입력하십시요\n\n불편사항은 고객만족센터[1588-4200] 또는 전산센터로 문의하시기 바랍니다.", "알 림 ", MB_ICONWARNING|MB_OK);
		GetDlgItem(IDC_EDT_LOGINID)->SetFocus();
		EndDialog(IDCANCEL);
		break;
	case '2':		// PASSWD 실패
		{
			char cCount = prData->TRData[1];
			if(cCount=='0')
			{
				if(gGetProgrameType()<0)
				{
					tmp.Format("접속비밀번호는 온라인 증권거래의 안정성을 확보하고\n\
	고객정보 보호를 위하여 5회이상 연속 오류 입력시 접속이 불가능 해집니다.\n\n\
	불편사항은 고객만족센터[1588-4200] 또는 전산센터로 문의하시기 바랍니다.");
				}
				else
				{
					tmp.Format("접속비밀번호는 온라인 증권거래의 안정성을 확보하고\n\
	고객정보 보호를 위하여 5회이상 연 속 오류 입력시 접속이 불가능 해집니다.\n\n\
	불편사항은 고객만족센처[1588-4200] 또는 전산센터로 문의하시기 바랍니다.");
				}
				::MessageBox(NULL, tmp,	"비밀번호 오류", MB_OK);
				EndDialog(IDCANCEL);
			}
			else if(cCount=='1')
			{
				char cCnt = prData->TRData[2];
				tmp.Format("접속비밀번호는 온라인 증권거래의 안정성을 확보하고\n\
고객정보 보호를 위하여 5회이상 연속 오류 입력시 접속이 불가능 해집니다.\n\n\
접속비밀번호가 %c회 틀렸습니다. 확인후 입력하시기 바랍니다.\n\n\
불편사항은 고객만족센처[1588-4200] 또는 전산센터로 문의하시기 바랍니다.", cCnt);
				::MessageBox(NULL, tmp, "비밀번호 오류", MB_OK);
			}
		}
		break;
	case '3':
		{
			tmp = "접속 서버를 확인하여 주십시요";
			AfxMessageBox(tmp);
			GetDlgItem(IDC_EDT_LOGINID)->SetFocus();
		}
		break;
	case '4': // 기타 실패
		EndDialog(IDCANCEL);
		break;
	case '5':	//계좌 없음
		tmp = "입력하신 계좌번호가 존재하지 않습니다.\n계좌번호를 확인하시고 다시 입력해주시기 바랍니다";
		AfxMessageBox(tmp);
		GetDlgItem(IDC_EDT_LOGINID)->SetFocus();
		break;
	case '6':
		tmp = "입력하신 계좌비밀번호가 맞지 않습니다.\n계좌비밀번호를 확인하시고 다시 입력해주시기 바랍니다";
		AfxMessageBox(tmp);
		EndDialog(IDCANCEL);
		break;	
	case '7':		// 2003.07.09 이남경, 비밀번호 만료일 에러 처리
		{
/* temp
			CDlgPassPresentation dlg(m_hSocket, m_szRealID, m_szPassWd, prData->TRData);
			int nRet = dlg.DoModal();
			if (nRet == IDOK)
			{
				STLOGINSUCCESS* pData = (STLOGINSUCCESS*)prData->TRData;
				if (!CheckIPAuth(pData->RecentData))
					return CDlgRegion::OnCopyData(pWnd, pCopyDataStruct);

				MakeUserData(pData, pCopyDataStruct->dwData);
				EndDialog(IDOK);
			}
			else if (nRet == ID_RELOGIN)
			{
				CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDT_PASSWD);
				pEdit->SetWindowText("");
				pEdit->SetFocus();
			}
			else
			{
				tmp = "메인 실행이 취소되었습니다. 다시 실행하여 주십시요";
				AfxMessageBox(tmp);
				EndDialog(IDCANCEL);
			}
*/
			tmp = "메인 실행이 취소되었습니다. 다시 실행하여 주십시요";
			AfxMessageBox(tmp);
			EndDialog(IDCANCEL);
		}
		break;
	case 'X':	// 동일고객 동일아이디
	case 'Y':	// 동일고객 상이아이디
	case 'Z':	// 상이고객 동일아이디
		{
/*  temp
			CString szTemp;
			szTemp.Format("%c", cType);
			CDlgWebHelper helper;
			helper.DlgWeb_DoModal("http://www.winnet.co.kr/hts/winnetpro/merge.html", "SendTR",
				m_szRealID, "", szTemp, 1, 750, 650, 1, this);
*/
		tmp = "메인 실행이 취소되었습니다. 다시 실행하여 주십시요";
		AfxMessageBox(tmp);
		EndDialog(IDCANCEL);
		}
		break;
	default:
		break;
	}
	
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}


BOOL CLoginDlg::CheckIPAuth(char* szAuth)
{
	if (m_nAuthSkip == 1)
		return TRUE;

	if (m_nSetupType == 2)
	{
		CString szClientIP;
		LRESULT lRetGetIP = ::SendMessage(m_hSocketDlg, UDM_GET_IPADDRESS, 0, (LPARAM)&szClientIP);
		CString szIP_A,szIP_B,szIP_C,szIP_D;
		int ipIndex = szClientIP.Find('.');
		szIP_A = szClientIP.Left(ipIndex);
			szClientIP = szClientIP.Mid(ipIndex+1);
			ipIndex	   = szClientIP.Find('.');
		szIP_B = szClientIP.Left(ipIndex);
			szClientIP = szClientIP.Mid(ipIndex+1);
			ipIndex	   = szClientIP.Find('.');
		szIP_C = szClientIP.Left(ipIndex);
			szClientIP = szClientIP.Mid(ipIndex+1);
			//ipIndex    = szClientIP.Find('.');
		szIP_D = szClientIP;
		
		BOOL bInnerClient;
	
		if(szIP_A == "192")
			bInnerClient = TRUE;
		else 
			bInnerClient = FALSE;

		if(bInnerClient)
		{
			// 1. 내부 ip 인경우에는 지점 ip를 체크한다.
			CString szIP_CFirst = szIP_C.Left(1);
			int nIP_D = atoi(szIP_D); 
			if(szIP_B == "10" && szIP_CFirst == "2" && (nIP_D >= 70 && nIP_D <= 129))
			{
				//if(szLanType != "L1")
				//	return FALSE;					
				//else
					return TRUE;
			}
			else
			{
				AfxMessageBox("객장용은 지정된 IP에서만 실행이 됩니다.\nIP를 확인하시거나 직원용을 설치하여 주십시요.");
				return FALSE;
			}
		}
		else
		{
			//if(szLanType != "L4")
				AfxMessageBox("객장용은 지정된 IP에서만 실행이 됩니다.\nIP를 확인하시거나 직원용을 설치하여 주십시요.");
				return FALSE;	
		}
	}
	else if (m_nSetupType == 0)
	{
		if (m_nIDType == 1)
		{
			if (szAuth[0] == '1')
				return TRUE;
			else
			{
				AfxMessageBox("지금 사용하시는 IP에서는 직원용으로 접속하실수 없습니다.\n직원이 아니실 경우, 고객용을 설치하시기 바랍니다.");
				return FALSE;
			}
		}
		else if (m_nIDType == 2)
		{
			if (szAuth[1] == '1')
				return TRUE;
			else
			{
				AfxMessageBox("지금 사용하시는 IP에서는 Hts ID로 접속하실 수 없습니다.");
				return FALSE;
			}
		}
	}
	else if (m_nSetupType == 1)
		return TRUE;

	AfxMessageBox("어떤 권한도 설정되지 않았습니다.\n직원이 아니시면 고객용을 설치하시기 바랍니다.");
	return FALSE;
}


void CLoginDlg::MakeUserData(STLOGINSUCCESS* pLoginData, int nBufLen)
{
#ifdef _DEBUG
	char aIP[15+1]={0,};
	memcpy(aIP, pLoginData->IPAddr, 15);
#endif
	CString tmp;

	char  szCount[3+1] = {0,};
	memcpy(szCount, pLoginData->AccCount, 3);
	int nAcctCount = atoi(szCount);

	int nSize = nAcctCount * GEJA_SIZE;		// GEJA_SIZE : 32byte

	int nTotal = sizeof(STUSERINFO) + nSize +10;
	char*  AccList	= new char[nTotal];
	STUSERINFO* pUInfo = (STUSERINFO*)AccList;
	memset(pUInfo, 0x20, nTotal);

	memcpy(pUInfo->ID, m_szRealID, min(m_szRealID.GetLength(), 20));
	memcpy(pUInfo->Passwd, m_szPassWd, min(m_szPassWd.GetLength(), 8));
	memcpy(pUInfo->RecentDate, pLoginData->RecentData, 8);
	memcpy(pUInfo->Level, pLoginData->Level, 32);
	memcpy(pUInfo->IPAddr, pLoginData->IPAddr, 15);
	memcpy(pUInfo->Grade, pLoginData->Grade, 3);
	memcpy(pUInfo->Name, pLoginData->Name, 20);
	memcpy(pUInfo->Gigum, pLoginData->Gigum, 3);
	memcpy(pUInfo->Gigum, pLoginData->Gigum, 3);
	memcpy(pUInfo->OpenMap, pLoginData->OpenMap, 42);
	memcpy(pUInfo->AccCount, pLoginData->AccCount, 3);
	memcpy(pUInfo->AccList, pLoginData->AccList, nSize);
	memcpy((LPSTR)m_pBuffer, pUInfo, nTotal);

	delete pUInfo;

////////////{{  다중접속시행시 필요..
//	char DivPITerm = pLoginData->DivPITerm[0];
//	char szIPAddressInDB[16];
//	if(sizeof(pLoginData->IpAddress)<=sizeof(szIPAddressInDB))
//	{
//		memcpy(szIPAddressInDB,pLoginData->IpAddress,sizeof(pLoginData->IpAddress));
//	}
//	if(DivPITerm == '1'&& m_strIPAdress != szIPAddressInDB)
//	{
//		// 다중접속..
//		CString	str;
//		str.Format("다른 단말기에서 사용중입니다. 사용중인 단말기의 접속을 끊고 새로 접속하겠습니까?");
//		int nResult = MessageBox(str,NULL,MB_YESNO|MB_ICONSTOP);
//		if(nResult == IDOK)
//		{
//			MultiConnectTRSend();
//		}
//		else
//		{
//		}
//	}
////////////}}

	// Login_xxx.usr이 없는 경우 자동 생성 
/*
	CString szPath;
	szPath.Format("%s%s", m_szRootPath, _T("\\User\\"));
	CString szLogin;
	//szLogin.Format("%s\\Login_%s.usr", szPath, m_szRealID);
	szLogin.Format("%s%s\\Login.usr", szPath, m_szRealID);
	CString szData;
	szData.Format("%5.5s", pLoginData->OpenMap);
	szData.TrimLeft(); szData.TrimRight();
	::WritePrivateProfileString("MAINSTART", "OPENMAP", szData, szLogin);
*/
}

void CLoginDlg::OnConnectVersion() 
{
	ShowWindow(SW_MINIMIZE);

	SetConnectServer();

	CreateSocket();		

	WriteLoginInfo();
}


void CLoginDlg::InitControl()
{
	int nType = gGetProgrameType();

	switch (nType)
	{
	case TYPE_STAFF     : SetWindowText("직원용 로긴");		break;
	case TYPE_CLIENT    : SetWindowText("사용자용 로긴");	break;
	case TYPE_JANGOJOHO : SetWindowText("잔고조회용 로긴"); break;
	case TYPE_EXTRA     : SetWindowText("기타");		    break;
	}
}

void CLoginDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Get the client rect.
	CRect rcClient;

	CFont * pOldFont = NULL;

	GetClientRect( &rcClient );

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
//	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&m_LoginBackBitMap.m_bitmap);

	// copy the memory device context back into the original DC via BitBlt().
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);

	// cleanup resources.
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

void CLoginDlg::LoadImages()
{
	CString szImgPath = m_szRootPath + "\\Image";
	CString str;

	str = szImgPath + IMG_LOGGIN;
	m_LoginBackBitMap.LoadBMPImage(str);

	{
		CBitmap bitmap;

		m_LoginBtnImgList.DeleteImageList();
		m_LoginBtnImgList.Create(49, 22, ILC_COLORDDB, 1, 1);

		str = szImgPath + "\\Login_설정_on.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_설정_down.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_설정_over.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_접속_on.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_접속_down.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_접속_over.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_종료_on.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_종료_down.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();

		str = szImgPath + "\\Login_종료_over.bmp";
		bitmap.Attach((HBITMAP)LoadImage(NULL, str, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE) );
		m_LoginBtnImgList.Add(&bitmap, RGB(255,0,0));
		bitmap.DeleteObject();
	}

	m_btnSetting.FP_vSetImage(&m_LoginBtnImgList, 0, 1, -1, 2 );
	m_btnConnect.FP_vSetImage(&m_LoginBtnImgList, 3, 4, -1, 5 );
	m_btnExit.FP_vSetImage   (&m_LoginBtnImgList, 6, 7, -1, 8 );
}

void CLoginDlg::MoveCtrls()
{
	SetWindowPos(NULL, 0, 0, 574, 376, SWP_NOZORDER | SWP_NOMOVE);
	GetDlgItem(IDC_EDT_LOGINID)->SetWindowPos(NULL, 390, 126, 90, 20, SWP_NOZORDER );
	GetDlgItem(IDC_EDT_PASSWD )->SetWindowPos(NULL, 390, 148, 90, 20, SWP_NOZORDER );
	GetDlgItem(IDC_EDT_CERTPWD)->SetWindowPos(NULL, 390, 170, 90, 20, SWP_NOZORDER );

	GetDlgItem(IDC_CHK_SAVE    )->SetWindowPos(NULL, 491, 128, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
	GetDlgItem(IDC_CHK_SISEONLY)->SetWindowPos(NULL, 335, 198, 0, 0, SWP_NOZORDER | SWP_NOSIZE );

	m_btnConnect.SetWindowPos(NULL,335, 231, 49, 22, SWP_NOZORDER );
	m_btnSetting.SetWindowPos(NULL,390, 231, 49, 22, SWP_NOZORDER );
	m_btnExit.SetWindowPos(NULL,445, 231, 49, 22, SWP_NOZORDER );
}


HBRUSH CLoginDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor )
{
	if ( pWnd->GetDlgCtrlID() == IDC_CHK_SAVE || pWnd->GetDlgCtrlID() == IDC_CHK_SISEONLY)
		pDC->SetBkMode(TRANSPARENT);

	return CDialog::OnCtlColor( pDC, pWnd, nCtlColor );
}

UINT CLoginDlg::OnNcHitTest( CPoint point )
{
	CRect  rt;
	GetWindowRect(&rt);

	if( rt.PtInRect(point) )
	{
		return HTCAPTION;
	}

	return CDialog::OnNcHitTest(point);
}