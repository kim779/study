// MoveDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "amRm.h"
#include "MoveDlg.h"
#include "afxdialogex.h"

#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMoveDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMoveDlg, CDialog)

CMoveDlg::CMoveDlg(CWnd* pParent, int kind)
	: CDialog(CMoveDlg::IDD, pParent)
{
	m_kind = kind;
	if (m_kind != kindFORMB) m_kind += 1;

	m_index = 0;
	m_files.RemoveAll();
}

CMoveDlg::~CMoveDlg()
{
}

void CMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
}


BEGIN_MESSAGE_MAP(CMoveDlg, CDialog)
	ON_BN_CLICKED(IDC_RUN, &CMoveDlg::OnClickedRun)
	ON_BN_CLICKED(IDC_MOVE, &CMoveDlg::OnClickedMove)
END_MESSAGE_MAP()


// CMoveDlg 메시지 처리기입니다.


BOOL CMoveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	int	nLen;
	TCHAR	wb[1024];
	GetCurrentDirectory(sizeof(wb), wb);
	CString	path; path.Format("%s/%s", wb, "workshop.ini");
	
	nLen = GetPrivateProfileString(ENVIRONMENT, ROOTDIR, _T(""), wb, sizeof(wb), path);
	CopyMemory(m_root, wb, nLen); m_root[nLen] = 0x00;

	sprintf_s(m_adbpath, "%s/%s/%s", m_root, "adb", "adb.exe");

	strcpy_s(m_activity, "com.winix.axMobileApp/com.winix.app.WinixActivity");
	nLen = GetPrivateProfileString("MOBILE", "RUN", _T(""), wb, sizeof(wb), path);
	if (nLen > 0)
	{
		CopyMemory(m_activity, wb, nLen); m_activity[nLen] = 0x00;
	}

	strcpy_s(m_runpath, "/sdcard/WinixMobile");
	nLen = GetPrivateProfileString("MOBILE", "FORMPATH", _T(""), wb, sizeof(wb), path);
	if (nLen > 0)
	{
		CopyMemory(m_runpath, wb, nLen); m_runpath[nLen] = 0x00;
	}

	m_progress.SetRange(0, 100);
	m_progress.SetPos(0);

	CString	guide = _T("");
	switch (m_kind)
	{
	case kindFORMB:	guide = _T("Click [Move] : RUNTIME_XML files is moving");	break;
	case kindIMAGE:	guide = _T("Click [Move] : IMAGE files is moving");		break;
	case kindTRLY:	guide = _T("Click [Move] : TRLAYOUT files is moving");		break;
	case kindINFO:	guide = _T("Click [Move] : TAB files is moving");		break;
	}

	guideMsg(guide);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMoveDlg::OnClickedMove()
{
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;
	BOOL			bRc;
	DWORD			exitcode;
	CString			string, file, folder;

	guideMsg("");

	switch (m_kind)
	{
	case kindFORMB:	folder = _T("form");	break;
	case kindIMAGE:	folder = _T("images");	break;
	case kindTRLY:	folder = _T("trlayout");break;
	case kindINFO:	folder = _T("tab");	break;
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	m_index = 0; guideMsg("Moving.....");
	while (m_index < m_files.GetSize())
	{
		file = m_files.GetAt(m_index++);
		string.Format("\"%s\" push \"%s\" %s/%s", m_adbpath, file, m_runpath, folder);

		bRc = CreateProcess(NULL, (LPSTR)string.operator LPCSTR(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		if (!bRc)
		{
			guideMsg("화면복사 오류. 연결상태를 확인요망.");
			return;
		}

		WaitForSingleObject(pi.hProcess, INFINITE);

		exitcode = 0; GetExitCodeProcess(pi.hProcess, &exitcode);
		if (exitcode)
		{
			guideMsg("화면복사 오류. 연결상태를 확인요망");
			break;
		}
		m_progress.SetPos((m_index / m_files.GetSize()) * 100);
	}

	string.Format("\"%s\" shell sync", m_adbpath);
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	bRc = CreateProcess(NULL, (LPSTR)string.operator LPCSTR(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (!bRc)
	{
		TRACE("errno = [%s]\n", GetLastError());
		AfxMessageBox("화면 Sync 오류");
		return;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	if (!exitcode)
	{
		guideMsg("Moving completed...");
		(GetDlgItem(IDCANCEL))->SetWindowText("OK");
	}
}

void CMoveDlg::OnClickedRun()
{
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;
	BOOL			bRc;
	CString			string;

	guideMsg("");

	string.Format("\"%s\" shell am start -n %s", m_adbpath, m_activity);
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	bRc = CreateProcess(NULL, (LPSTR)string.operator LPCSTR(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (!bRc)
	{
		guideMsg("실행 오류. 연결상태 확인요망"); return;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD exitcode = 0; GetExitCodeProcess(pi.hProcess, &exitcode);
	if (exitcode) guideMsg("실행 오류. 연결상태 확인요망");
}

void CMoveDlg::guideMsg(CString string)
{
	GetDlgItem(IDC_GUIDE)->SetWindowText(string);
}
