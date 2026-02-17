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

/////////////////////////////////////////////////////////////////////////////
// CAxisverApp

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

	CCommLine cmdInfo;
	ParseCommandLine(cmdInfo);
}

void CAxisverApp::updateObjectAXIS()
{
	if (m_axis != (HWND) NULL)
	{
		Sleep(500);
		::SendMessage(m_axis, WM_CLOSE, 0, 0);
		Sleep(500);
	}

	if (!getDownDirFileName())
	{
		AfxMessageBox("컴포넌트를 업데이트 할 수 없습니다.");
		return;
	}

	int	fileN, step, index;
	CString	dests, news, tmps, text;
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
	if (IsWindow(m_axis))
	{
		Sleep(500);
		::PostMessage(m_axis, WM_CLOSE, 0, 0);
		WaitForSingleObject(m_axis, 3000);
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
