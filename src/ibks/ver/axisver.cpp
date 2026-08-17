// axisver.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "axisver.h"
#include "update.h"
#include "zip.h"

#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "../../H/axislog.h"
/////////////////////////////////////////////////////////////////////////////
// CAxisverApp

#include <RestartManager.h>
#pragma comment(lib, "Rstrtmgr.lib")

// 지정 파일을 사용 중인 프로세스들을 로그로 남긴다.
void LogProcessesUsingFile(LPCSTR filePath)
{
	CString slog;
	DWORD dwSession = 0;
	WCHAR szSessionKey[CCH_RM_SESSION_KEY + 1] = { 0 };

	DWORD rc = RmStartSession(&dwSession, 0, szSessionKey);
	if (rc != ERROR_SUCCESS)
	{
		slog.Format("[VER] RmStartSession fail(%d)\n", rc);
		OutputDebugString(slog);
		return;
	}

	// RM API는 유니코드 경로를 받는다. CP949 → UTF-16 변환.
	WCHAR wPath[MAX_PATH] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, filePath, -1, wPath, MAX_PATH);
	LPCWSTR pFiles[1] = { wPath };

	rc = RmRegisterResources(dwSession, 1, pFiles, 0, NULL, 0, NULL);
	if (rc != ERROR_SUCCESS)
	{
		slog.Format("[VER] RmRegisterResources fail(%d)\n", rc);
		OutputDebugString(slog);
		RmEndSession(dwSession);
		return;
	}

	UINT nProcInfoNeeded = 0, nProcInfo = 0;
	DWORD dwReason = 0;

	// 먼저 필요한 개수를 조회 (nProcInfo=0 으로 호출)
	rc = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, NULL, &dwReason);

	if (rc == ERROR_MORE_DATA && nProcInfoNeeded > 0)
	{
		RM_PROCESS_INFO* pInfo = new RM_PROCESS_INFO[nProcInfoNeeded];
		nProcInfo = nProcInfoNeeded;

		rc = RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, pInfo, &dwReason);
		if (rc == ERROR_SUCCESS)
		{
			for (UINT i = 0; i < nProcInfo; i++)
			{
				// 프로세스 이름은 UTF-16 → CP949 로 변환해서 로그
				char szName[256] = { 0 };
				WideCharToMultiByte(CP_ACP, 0,
					pInfo[i].strAppName, -1, szName, sizeof(szName), NULL, NULL);

				DWORD pid = pInfo[i].Process.dwProcessId;
				slog.Format("[VER] locked by: %s (pid=%d)\n", szName, pid);
				OutputDebugString(slog);
				FileLog(slog);
			}
		}
		delete[] pInfo;
	}

	RmEndSession(dwSession);
}

static BOOL WaitAxisExit(HWND hAxis, DWORD pid, DWORD timeoutMs)
{
	CString slog;

	// PID 우선: /x 로 AXIS가 자기 PID를 직접 넘겨준 경우, 창(hAxis) 상태와 무관하게
	// 바로 프로세스 핸들을 열어 대기한다. SendMessage(WM_CLOSE)가 리턴하는 순간 이미
	// 창은 파괴돼있는(IsWindow=FALSE) 경우가 흔한데, 창 소멸과 프로세스(DLL 언로드,
	// 소켓/COM 정리) 완료는 별개 타이밍이라 hAxis만으로는 이 구간을 놓친다.
	HANDLE hProc = (pid != 0) ? ::OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, FALSE, pid) : NULL;

	if (hProc == NULL)
	{
		// 구버전 호출(=/x 없이 실행됨) 등 PID를 못 받은 경우의 폴백 - 기존 창 폴링 로직 유지
		if (!::IsWindow(hAxis))
			return TRUE;   // 이미 창이 없으면 종료된 것으로 간주

		DWORD waited = 0;
		while (::IsWindow(hAxis) && waited < timeoutMs)
		{
			::Sleep(100);
			waited += 100;

			slog.Format("[VER] axis waited =[%d] timeoutMs=[%d]", waited, timeoutMs);
			OutputDebugString(slog);
		}
		return !::IsWindow(hAxis);
	}

	DWORD wr = ::WaitForSingleObject(hProc, timeoutMs);
	BOOL exited = TRUE;
	if (wr == WAIT_TIMEOUT)
	{
		// 정상 종료가 시간 내 안 끝남 → 마지막 수단으로 강제 종료
		slog.Format("[VER] axis exit timeout. terminating pid=%d\n", pid);
		OutputDebugString(slog);
		::TerminateProcess(hProc, 0);
		DWORD wr2 = ::WaitForSingleObject(hProc, 3000);   // 강제 종료 반영 대기
		exited = (wr2 == WAIT_OBJECT_0);
		if (!exited)
		{
			slog.Format("[VER] axis force-kill wait FAILED pid=%d\n", pid);
			OutputDebugString(slog);
		}
	}
	else
	{
		slog.Format("[VER] WaitForSingleObject no timeout  pid=%d\n", pid);
		OutputDebugString(slog);
	}
	::CloseHandle(hProc);
	return exited;
}

BEGIN_MESSAGE_MAP(CAxisverApp, CWinApp)
	//{{AFX_MSG_MAP(CAxisverApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAxisverApp construction

CAxisverApp::CAxisverApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAxisverApp object

CAxisverApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAxisverApp initialization

BOOL CAxisverApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	parsingCommandStatus();

	switch (m_argument)
	{
	case argACT::argMOVE:
	case argACT::argCLASS:
	case argACT::argAPNAME:
	case argACT::argDIR:
		updateObjectAXIS();
		break;

	case argACT::argUSER:
	case argACT::argRETRY:
		retryAXIS();
		break;

	default:
		break;
	}

	return FALSE;
}

void CAxisverApp::parsingCommandStatus()
{
	m_argument = argACT::argNONE;
	m_user.Empty();
	m_pass.Empty();
	m_params.Empty();
	m_keys.Empty();
	m_axis = (HWND) NULL;
	m_axisPid = 0;

	CCommLine cmdInfo;
	ParseCommandLine(cmdInfo);

	CString slog;
	slog.Format("[VER] axis =  [%x] pid=[%d]\n", m_axis, m_axisPid);
	OutputDebugString(slog);
}

void CAxisverApp::updateObjectAXIS()
{
	if (m_axis != (HWND) NULL || m_axisPid != 0)
	{
		Sleep(500);
		if (m_axis != (HWND) NULL)
			::SendMessage(m_axis, WM_CLOSE, 0, 0);
		if (!WaitAxisExit(m_axis, m_axisPid, 3000))
		{
			CString slog;
			slog.Format("[VER] WaitAxisExit FAILED pid=[%d] - proceeding to copy anyway\n", m_axisPid);
			OutputDebugString(slog);
		}
	}

	if (!getDownDirFileName())
	{
		AfxMessageBox("컴포넌트를 업데이트 할 수 없습니다.");
		return;
	}

	int	fileN, step, index;
	CString	dests, news, tmps, text, slog;
	CMapStringToString refs;
	CZip	zip;

	fileN = m_files.GetSize();

	CUpdate* update = new CUpdate(m_root);
	update->Create(CUpdate::IDD);
	update->ShowWindow(SW_SHOW);
	update->UpdateWindow();
	update->m_progress.SetRange(0, 100);
	if (fileN)
		step = 100 / fileN;
	else
		step = 100;

	update->m_progress.SetStep(step);
	update->m_progress.SetPos(0);
	Sleep(1000);

	for (int ii = 0; ii < fileN; ii++)
	{
		tmps = m_files.GetAt(ii);
		if (tmps.IsEmpty())
			continue;

		news.Format(_T("%s\\%s\\%s\\%s"), m_root.GetString(), RUNDIR, AXISDIR, tmps.GetString());
		tmps.MakeUpper();

		dests.Format(_T("%s\\%s\\%s"), m_root.GetString(), (tmps.Find(".INI") < 0) ? RUNDIR : TABDIR, tmps.GetString());
		index = tmps.Find('.');
		if (index != -1)
			tmps = tmps.Left(index);
		tmps = "Component/" + tmps;

		text = news + ".DZ";
		if (zip.Decompress((char *)news.operator LPCTSTR(), (char *)text.operator LPCTSTR()))
			DeleteFile(news);
		else
			text = news;

		for ( ;; )
		{
			if (CopyFile(text, dests, FALSE))
				break;

			DWORD err = GetLastError();
			slog.Format("[VER] CopyFile fail(%d): %s -> %s\n", err, (LPCSTR)text, (LPCSTR)dests);
			OutputDebugString(slog);

			if (err == ERROR_SHARING_VIOLATION)   // 32
				LogProcessesUsingFile(dests);     // 누가 잡고 있는지 로그

			AfxMessageBox(tmps + "을 사용중입니다. 프로그램을 종료하십시오.", MB_OK|MB_ICONINFORMATION|MB_SYSTEMMODAL);
		}
		DeleteFile(text);
		update->m_progress.OffsetPos(step);
		Sleep(500);
	}

	dests.Format(_T("%s\\%s\\%s"), m_root.GetString(), RUNDIR, AXISDIR);
	::RemoveDirectory(dests);

	update->DestroyWindow();
	delete update;

	dests.Format(_T("%s\\%s\\%s"), m_root.GetString(), RUNDIR, m_apname.IsEmpty() ? "AXIS.EXE" : m_apname.GetString());
	tmps.Format(_T(" %s /i %s %s"), m_params.GetString(), m_user.GetString(), m_pass.GetString());
	if (!m_keys.IsEmpty())
	{
		text.Format(_T(" /k %s"), m_keys.GetString());
		tmps += text;
	}

	BOOL	fBool;
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	Sleep(1000);
	fBool = CreateProcess(dests,				// application name
				(char *)tmps.operator LPCTSTR(),// command line
				NULL,				// process attribute
				NULL,				// thread attribute
				FALSE,				// is inherit handle
				0,				// creation flags
				NULL,				// environment
				NULL,				// current directory
				&si,				// STARTUPINFO
				&pi);				// PROCESS_INFORMATION

	if (!fBool || !pi.hProcess)
		AfxMessageBox("Can't Update Runtime Program.\n\nReinstall Program.");
}

void CAxisverApp::retryAXIS()
{
	if (IsWindow(m_axis) || m_axisPid != 0)
	{
		Sleep(500);
		if (IsWindow(m_axis))
			::PostMessage(m_axis, WM_CLOSE, 0, 0);
		WaitAxisExit(m_axis, m_axisPid, 10000);
	}

	CString	tmps, dests, text;

	tmps.Empty();
	dests.Format(_T("%s\\%s\\%s"), m_root.GetString(), RUNDIR, m_apname.IsEmpty() ? "AXIS.EXE" : m_apname.GetString());
	if (m_argument == argACT::argRETRY)
		tmps.Format(_T(" %s /i %s %s"), m_params.GetString(), m_user.GetString(), m_pass.GetString());
	else
		tmps.Format(_T(" %s"), m_params.GetString());
	if (!m_keys.IsEmpty())
	{
		text.Format(_T(" /k %s"), m_keys.GetString());
		tmps += text;
	}

	BOOL	fBool;
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	si.cb          = sizeof(STARTUPINFO);
	si.dwFlags     = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;

	Sleep(1000);
	fBool = CreateProcess(dests,				// application name
				(char *)tmps.operator LPCTSTR(),// command line
				NULL,				// process attribute
				NULL,				// thread attribute
				FALSE,				// is inherit handle
				0,				// creation flags
				NULL,				// environment
				NULL,				// current directory
				&si,				// STARTUPINFO
				&pi);				// PROCESS_INFORMATION
}

BOOL CAxisverApp::getDownDirFileName()
{
	CString	path;
	HANDLE	hFile;
	BOOL	success = FALSE;
	WIN32_FIND_DATA	findfiledata;
	
	path.Format(_T("%s\\%s\\%s"), m_root.GetString(), RUNDIR, AXISDIR);

	BOOL  rc = ::RemoveDirectory(path);
	DWORD error = GetLastError();
	if (rc || (!rc && (error == ERROR_PATH_NOT_FOUND || error == ERROR_FILE_NOT_FOUND)))
	{
		m_files.RemoveAll();
		return TRUE;
	}

	m_files.RemoveAll();
	path.Format(_T("%s\\%s\\%s\\*.*"), m_root.GetString(), RUNDIR, AXISDIR);
	hFile = ::FindFirstFile(path.operator LPCTSTR(), &findfiledata);
	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	if (!(findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		m_files.Add(findfiledata.cFileName);

	while (true)
	{
		success = ::FindNextFile(hFile, &findfiledata);
		if (!success)
		{
			::FindClose(hFile);
			break;
		}
		if (!(findfiledata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			m_files.Add(findfiledata.cFileName);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//
//	CCommLine
//

CCommLine::CCommLine()
{
}

CCommLine::~CCommLine()
{
}

void CCommLine::ParseParam(LPCSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	CAxisverApp* app = (CAxisverApp *)AfxGetApp();
	CString	tmps     = lpszParam;

	if (bFlag)
	{
		if (!tmps.CompareNoCase("u"))
			app->m_argument = CAxisverApp::argACT::argMOVE;
		else if (!tmps.CompareNoCase("c"))
			app->m_argument = CAxisverApp::argACT::argCLASS;
		else if (!tmps.CompareNoCase("a"))
			app->m_argument = CAxisverApp::argACT::argAPNAME;
		else if (!tmps.CompareNoCase("d"))
			app->m_argument = CAxisverApp::argACT::argDIR;
		else if (!tmps.CompareNoCase("i"))
			app->m_argument = CAxisverApp::argACT::argUSER;
		else if (!tmps.CompareNoCase("r"))
			app->m_argument = CAxisverApp::argACT::argRETRY;
		else if (!tmps.CompareNoCase("k"))
			app->m_argument = CAxisverApp::argACT::argMANAGER;
		else if (!tmps.CompareNoCase("x"))
			app->m_argument = CAxisverApp::argACT::argPID;
		else
		{
			app->m_params += " /";
			app->m_params += tmps;
			app->m_argument = CAxisverApp::argACT::argRSV;
		}
		return;
	}

	switch (app->m_argument)
	{
	case CAxisverApp::argACT::argCLASS:
		app->m_axis = (HWND)atoi(tmps);
		return;
	case CAxisverApp::argACT::argAPNAME:
		app->m_apname = tmps;
		return;
	case CAxisverApp::argACT::argMOVE:
		break;
	case CAxisverApp::argACT::argDIR:
		app->m_root = tmps;
		return;
	case CAxisverApp::argACT::argNONE:
		if (!tmps.CompareNoCase("Move"))
			app->m_argument = CAxisverApp::argACT::argMOVE;
		return;
	case CAxisverApp::argACT::argMANAGER:
		app->m_keys = tmps;
		return;
	case CAxisverApp::argACT::argPID:
		app->m_axisPid = (DWORD)atoi(tmps);
		return;
	case CAxisverApp::argACT::argRSV:
		app->m_params += ' ';
		app->m_params += tmps;
	default:
		return;
	}

	if (app->m_user.IsEmpty())
		app->m_user = tmps;
	else if (app->m_pass.IsEmpty())
		app->m_pass = tmps;
}
