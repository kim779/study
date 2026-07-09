// CodeEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "fx_edit.h"
#include "CodeEdit.h"

#include "Controlwnd.h"
#define TM_ONETOONE 9999
#define TM_USERABOUT 9998

// CCodeEdit

IMPLEMENT_DYNAMIC(CCodeEdit, CEdit)

CCodeEdit::CCodeEdit(CControlwnd *pwnd)
{
	m_bServerError = false;
}

CCodeEdit::~CCodeEdit()
{
}


BEGIN_MESSAGE_MAP(CCodeEdit, CEdit)
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CCodeEdit 메시지 처리기입니다.

void CCodeEdit::ShellexcuteLogin()
{
	/*
	CControlwnd* pWnd = (CControlwnd*)GetParent();
	CString path,data, pass, tmps;
	path.Format(_T("%s\\%s"), pWnd->m_home, _T("KBondMessenger\\KBondMessenger.exe"));
	pass = 	m_pass;
	m_crypt.Decrypt((BYTE*)pass.operator LPCSTR(), pass.GetLength());

	CString	file, key, tmpS;
	char	buf[1024];
	file.Format("%s\\%s\\%s\\%s", pWnd->m_home, "user",  pWnd->m_sUserName, "usersetup.ini");
	GetPrivateProfileString("messanger", "hwnd", "", buf, sizeof(buf), file);
	tmps.Format(_T("%s"), buf);
	tmps.TrimRight();
	tmps.TrimLeft();

	if(tmps.IsEmpty())
		return;

	int ihwnd = atoi(tmps);
	data.Format(_T("BTS|%d %s|%s %s"), ihwnd, pWnd->m_sUserID, m_crypt.m_outB, _T("Y"));
	//		
	//ShellExecute(this->m_hWnd, "open",  path, data, NULL, SW_SHOWNORMAL);
	*/
	AfxGetMainWnd()->SendMessage(WM_USER+500, 9894, (LPARAM)0);
}

void CCodeEdit::JustLogin()
{
	/*
	CControlwnd* pWnd = (CControlwnd*)GetParent();
	CString tmp, temps;
	struct _st
	{
		char id[32];
		char pass[32];
		char type[32];
	};

	CString path,data, pass;
	struct _st  *pst = new struct _st;
	memset(pst,0, sizeof(struct _st));
	tmp.Format(_T("t%s"), pWnd->m_sUserID);
	memcpy(&pst->id[0], (char*)tmp.operator LPCSTR(), tmp.GetLength());
	pass = 	m_pass;
	m_crypt.Decrypt((BYTE*)pass.operator LPCSTR(), pass.GetLength());
	tmp.Format(_T("tt%s"), m_crypt.m_outB);
	memcpy(&pst->pass[0], (char*)tmp.operator LPCSTR(), tmp.GetLength());
			
	if(m_bServerError)  //장애로그인 N
		tmp.Format(_T("ttt%s"), "N");  
	else
		tmp.Format(_T("ttt%s"), "Y");

	memcpy(pst->type, (char*)tmp.operator LPCSTR(), tmp.GetLength());
				
	COPYDATASTRUCT tiplogin;
	tiplogin.cbData = 96;
	tiplogin.dwData = 28673;
	tiplogin.lpData = pst;

	HWND hwn = ::FindWindow("TfrmMain", NULL);
	
	::SendMessage(hwn, WM_COPYDATA, (WPARAM)this->m_hWnd , LPARAM(&tiplogin));	
	*/
	AfxGetMainWnd()->SendMessage(WM_USER+500, 9893, (LPARAM)0);
}


bool CCodeEdit::CheckMessangerLogin()
{
	CControlwnd* pWnd = (CControlwnd*)GetParent();
	CString	file, key, tmpS;
	char	buf[1024];
	file.Format("%s\\%s\\%s\\%s", pWnd->m_home, "user",  pWnd->m_sUserName, "usersetup.ini");
	GetPrivateProfileString("messanger", "connect", "", buf, sizeof(buf), file);
	
	tmpS.Format(_T("%s"), buf);
	tmpS.TrimLeft();
	tmpS.TrimRight();

	if(tmpS.IsEmpty())
		return false;
	else
	{
		if(tmpS == "N" || tmpS == "1" || tmpS == "2")
			return false;
	}
/*
	file.Format("%s\\%s\\%s\\%s", pWnd->m_home, "user",  pWnd->m_sUserName, "usersetup.ini");
	GetPrivateProfileString("messanger", "Serer_error", "", buf, sizeof(buf), file);
	
	tmpS.Format(_T("%s"), buf);
	tmpS.TrimLeft();
	tmpS.TrimRight();
	if(tmpS == "Y")
		m_bServerError = true;
	else
		m_bServerError = false;
*/
	return true;
}

void CCodeEdit::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString tmpS;
	GetWindowText(tmpS);
	if(tmpS.IsEmpty())
		return;

	CMenu	menu;
	CPoint	pt;
	GetCursorPos(&pt);

	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, 1000, "1:1대화");
	menu.AppendMenu(MF_STRING, 1000 + 1, "사용자정보");


	int menuID = menu.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, pt.x, pt.y, this);
	switch(menuID)
	{
		case 1000:
		{
			CControlwnd* pWnd = (CControlwnd*)GetParent();
			CString senddata;
			CString strname, strid;
			strname = pWnd->m_strMsgName;
			strname = strname.Left((strname.ReverseFind(' ')));
			senddata.Format(_T("%s(%s)"), strname, pWnd->m_strMsgUser);
	
			WCHAR* data = new WCHAR[senddata.GetLength() + 2];
			memset(data, 0x00, senddata.GetLength()*2 + 2);
		
			MultiByteToWideChar(CP_ACP, 0, (LPSTR)senddata.operator LPCSTR(), -1, data, senddata.GetLength()*2 );

			COPYDATASTRUCT tip;
			tip.cbData = wcslen(data)*2 + 2;
			tip.dwData = 38495;
			tip.lpData = data;

			HWND hwn = ::FindWindow("TfrmMain", NULL);
			if(hwn == NULL)
			{
				ShellexcuteLogin();
				m_imsgcnt = 0;
				SetTimer(TM_ONETOONE, 2000, NULL);
			}
			else
			{
				if(!CheckMessangerLogin())
					JustLogin();
				Sleep(200);
				::SendMessage(hwn, WM_COPYDATA, (WPARAM)this->m_hWnd , LPARAM(&tip));	

				::PostMessage(hwn, WM_USER + 1003, 0 , 0);	
			}
		}
		break;
		case 1001:
		{
	
			CString user;
			CControlwnd* pWnd = (CControlwnd*)GetParent();
			user = pWnd->m_strMsgUser;
			WCHAR* data = new WCHAR[user.GetLength() + 2];
			memset(data, 0x00, user.GetLength()*2 + 2);
			MultiByteToWideChar(CP_ACP, 0, (LPSTR)user.operator LPCSTR(), -1, data, user.GetLength()*2);
	
			COPYDATASTRUCT tip;
			tip.cbData = wcslen(data)*2 + 2;
			tip.dwData = 38497;
			tip.lpData = data;

			HWND hwn = ::FindWindow("TfrmMain", NULL); 
			if(hwn == NULL)
			{
				ShellexcuteLogin();
				m_imsgcnt = 0;
				SetTimer(TM_USERABOUT, 2000, NULL);
			}
			else
			{
				if(!CheckMessangerLogin())
					JustLogin();
				Sleep(200);
				::SendMessage(hwn, WM_COPYDATA, (WPARAM)this->m_hWnd , LPARAM(&tip));	
				
				::PostMessage(hwn, WM_USER + 1003, 0 , 0);	
			}
		}
		break;
	}
}


int CCodeEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	TCHAR chrFileName[500];
	GetModuleFileName(NULL, chrFileName, 500);
	CString strFilePath;
	strFilePath.Format(_T("%s"), chrFileName);
	strFilePath = strFilePath.Left((strFilePath.ReverseFind('\\')));
	strFilePath = strFilePath.Left((strFilePath.ReverseFind('\\')));
	strFilePath = strFilePath.Right(strFilePath.GetLength() - strFilePath.ReverseFind('\\') - 1);

	CString strpath;
	HKEY	hSoftKey = NULL;
	char	sBuff[32];
	CString serv;
	DWORD	dSize = sizeof(sBuff) - 1;

#ifdef _DEBUG
	strpath.Format("Software\\%s\\AXIS Workstation V03.00\\Workstation", "TBOARD_WORK");
#else
	strpath.Format("Software\\%s\\AXIS Workstation V03.00\\Workstation", "KBond");
#endif

	if(RegOpenKeyEx(HKEY_CURRENT_USER, strpath
		, 0, KEY_READ,&hSoftKey) == ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hSoftKey, "PW", 0, NULL, (LPBYTE)sBuff, &dSize) == ERROR_SUCCESS)
		{
			RegCloseKey(hSoftKey);
			m_pass.Format(_T("%s"), sBuff);
			m_pass.TrimRight();
			m_pass.TrimLeft();
		}
	}

	return 0;
}


void CCodeEdit::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
		case TM_ONETOONE:
			{			
				CControlwnd* pWnd = (CControlwnd*)GetParent();
				CString senddata;
				senddata.Format(_T("%s(%s)"), pWnd->m_strMsgName, pWnd->m_strMsgUser);

				WCHAR* data = new WCHAR[senddata.GetLength() + 2];
				memset(data, 0, senddata.GetLength()*2 + 2);
				MultiByteToWideChar(CP_ACP, 0, (LPSTR)senddata.operator LPCSTR(), -1, data, senddata.GetLength()*2);

				COPYDATASTRUCT tip;
				tip.cbData = wcslen(data)*2 + 2;
				tip.dwData = 38495;
				tip.lpData = data;

				HWND hwn = ::FindWindow("TfrmMain", NULL);
				if(CheckMessangerLogin())
				{
					KillTimer(TM_ONETOONE);
					::SendMessage(hwn, WM_COPYDATA, (WPARAM)this->m_hWnd , LPARAM(&tip));	
					::PostMessage(hwn, WM_USER + 1003, 0 , 0);	
				}
				else
				{
					m_imsgcnt++;
					if(m_imsgcnt == 3)
						KillTimer(TM_ONETOONE);
				}
			}
			break;
		case TM_USERABOUT:
			{	
				CString user;
				CControlwnd* pWnd = (CControlwnd*)GetParent();
				user = pWnd->m_strMsgUser;
				WCHAR* data = new WCHAR[user.GetLength() + 2];
				memset(data, 0, user.GetLength()*2 + 2);
				MultiByteToWideChar(CP_ACP, 0, (LPSTR)user.operator LPCSTR(), -1, data, user.GetLength()*2);
	
				COPYDATASTRUCT tip;
				tip.cbData = wcslen(data)*2 + 2;
				tip.dwData = 38497;
				tip.lpData = data;

				HWND hwn = ::FindWindow("TfrmMain", NULL); 
				if(CheckMessangerLogin())
				{
					KillTimer(TM_USERABOUT);
					::SendMessage(hwn, WM_COPYDATA, (WPARAM)this->m_hWnd , LPARAM(&tip));	
					::PostMessage(hwn, WM_USER + 1003, 0 , 0);	
				}
				else
				{
					m_imsgcnt++;
					if(m_imsgcnt == 3)
						KillTimer(TM_USERABOUT);
				}
			}
			break;
	}
	CEdit::OnTimer(nIDEvent);
}
