// axiscore.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "axiscore.h"
#include "axiscoreDlg.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	PK_DEV		'D'	// DEFAULT DEV
#define	PK_BUSINESS	'B'	// 업무계
#define	PK_HTS		'H'	// 콜센터/관리자
				// 시세전용/객장용
				// 계좌, 시세전용
				// 일반사용자


#define	CORE_KEY	"KTB투자증권 FONIX2"

/////////////////////////////////////////////////////////////////////////////
// CAxiscoreApp

BEGIN_MESSAGE_MAP(CAxiscoreApp, CWinApp)
	//{{AFX_MSG_MAP(CAxiscoreApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxiscoreApp construction

CAxiscoreApp::CAxiscoreApp()
{
	m_regKey = CORE_KEY;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAxiscoreApp object

CAxiscoreApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAxiscoreApp initialization

BOOL CAxiscoreApp::InitInstance()
{
	SetRegistryKey(m_regKey);

	CString	string;
	char	wb[1024];
	BOOL	fBool;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	string = GetProfileString(ENVIRONMENT, ROOTDIR);
	if (string.IsEmpty())
	{
		::MessageBox(NULL, "설치프로그램 오류입니다.\n전산실로 문의 바랍니다.", "프로그램오류", MB_ICONWARNING);
		return FALSE;
	}

	CString strPathSM, strPathSetup;
	char cFilePath[MAX_PATH];
	char* pFileName;

	/*
	// LoadLibrary 시의 AXISSM.DLL 경로 획득
	strPathSM.Format("%s", "AXISSM.DLL");
	SearchPath(NULL, strPathSM, NULL, MAX_PATH, cFilePath, &pFileName);
	// LoadLibrary 시의 AXISETUP.DLL 경로 획득
	strPathSetup.Format("%s", "AXISETUP.DLL");
	SearchPath(NULL, strPathSetup, NULL, MAX_PATH, cFilePath, &pFileName);
	*/
	sprintf(wb, "%s\\exe\\axis.exe", string);
	
	/*
	if (GetFileVersion(wb) != GetFileVersion(strPathSM))
	{
		::MessageBox(NULL, "프로그램 정보가 일치하지 않습니다.\n전산실로 문의 바랍니다.", "프로그램오류", MB_ICONWARNING);
		return FALSE;
	}

	if (GetFileVersion(wb) != GetFileVersion(strPathSetup))
	{
		::MessageBox(NULL, "프로그램 정보가 일치하지 않습니다.\n전산실로 문의 바랍니다.", "프로그램오류", MB_ICONWARNING);
		return FALSE;
	}
	*/
	string.Format(" %s /k \"%s\" /n %s /p %c", m_lpCmdLine, m_regKey, m_pszExeName, PK_HTS);
	//string.Format(" %s /k \"%s\" /n %s /p %c /i sports01 74100", m_lpCmdLine, m_regKey, m_pszExeName, PK_HTS); // 키오스크용

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	fBool = CreateProcess(  wb,					// application name
				(char *)string.operator LPCTSTR(),	// command line
				NULL,					// process attribute
				NULL,					// thread attribute
				FALSE,					// is inherit handle
				0,					// creation flags
				NULL,					// environment
				NULL,					// current directory
				&si,					// STARTUPINFO
				&pi);					// PROCESS_INFORMATION

	return FALSE;
}

void CAxiscoreApp::write_Regkey()
{
	DWORD	value;
	char	buf[1024];
	CString	home, tmpS;
	// ENVIRONMENT sectioon
	value = GetCurrentDirectory(sizeof(buf), buf);
	home = CString(buf, value);
	tmpS = home.Mid(home.GetLength() - 4);
	if (!tmpS.Compare("\\exe"))
		home = home.Left(home.GetLength() -4);

	WriteProfileString(ENVIRONMENT, ROOTDIR, home);
}

CString CAxiscoreApp::GetFileVersion(CString strPath)
{
	DWORD dwSize = GetFileVersionInfoSize( (char *)strPath.operator LPCTSTR() , 0 );
	char * buffer = new char[dwSize];
	memset( buffer, 0, dwSize);
	
	GetFileVersionInfo( (char *)strPath.operator LPCTSTR(), 0, dwSize, buffer); 
	
	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;
	
	UINT cbTranslate;
	UINT dwBytes;
	LPBYTE lpBuffer;
	CString strSub, strReturn;

	
	VerQueryValue(buffer, 
		TEXT("\\VarFileInfo\\Translation"),
		(LPVOID*)&lpTranslate,
		&cbTranslate);
	
	strSub.Format("\\StringFileInfo\\%04x%04x\\Comments", 
                lpTranslate[0].wLanguage,
                lpTranslate[0].wCodePage);
	
	VerQueryValue(buffer, 
		(LPTSTR)(LPCTSTR)strSub, 
		(LPVOID*)&lpBuffer, 
		&dwBytes); 
	
	// 여기서 dwBytes 가 0 이라면 lpBuffer 는 trash 이므로 error 처리 필요
	strReturn = lpBuffer;
	
	delete [] buffer;
	
	return strReturn;
}