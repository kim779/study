// Connect.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "Connect.h"
#include "axmsg.hxx"
#include "setup.h"
#include "afxwin.h"
#include "progressDlg.h"
#include "mainfrm.h"
#include "invalidid.h"
#include <exdisp.h>
#include "comdef.h"
#include "../h/axisvar.h"
#include "DlgModaless.h"

#define	INPUTMAX	8
#define	GAP		3

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 키보드 보안의 GetPassword사용 여부(define시 사용)
//#define	KEYPASS

/////////////////////////////////////////////////////////////////////////////
// CConnect dialog

CConnect::CConnect(CWnd* pParent /*=NULL*/)
	: CDialog(CConnect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnect)
	m_user = _T("");
	m_pass = _T("");
	m_cPass = _T("");
	//}}AFX_DATA_INIT

	m_frame    = (CMainFrame *)pParent;
	m_mode     = SM_NO;
	m_progress = (CProgressDlg *) NULL;
		
	m_rate		= 0;
	m_totalCNT	= 0;
	m_curCNT	= 0;
	m_totalSIZE	= 0.0;
	m_curSIZE	= 0.0;

	m_auto		= false;
	m_skinShow	= false;
	m_framerun	= false;
	m_bitmap	= (HBITMAP) NULL;
	m_staff		= FALSE;

	//m_brBkgnd = NULL;
}

CConnect::CConnect(CWnd* pParent, CString home, CString userID)
	: CDialog(CConnect::IDD, pParent)
{
	m_frame = (CMainFrame *)pParent;
	m_home  = home;
	m_user  = userID;
	m_pass  = _T("");

	m_staff		= FALSE;
	m_mode		= SM_NO;
	m_progress	= (CProgressDlg *) NULL;
	m_rate		= 0;
	m_totalCNT	= 0;
	m_curCNT	= 0;
	m_totalSIZE	= 0.0;
	m_curSIZE	= 0.0;
	
	m_auto		= false;
	m_skinShow	= false;
	m_framerun	= false;
	m_bWeb1		= false;
	m_bWeb2		= false;
	m_bitmap	= (HBITMAP) NULL;

	CString	fontName;
	fontName.LoadString(ST_TEXT_OK);
	m_bFont.CreateFont(	11,                        // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,	           // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				DEFAULT_CHARSET,           // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				fontName);                 // lpszFacename
}

CConnect::~CConnect()
{
	if (m_progress)
	{
		m_progress->DestroyWindow();
		delete m_progress;
	}
	
	m_bFont.DeleteObject();
	if (m_bitmap)	DeleteObject(m_bitmap);
}

void CConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnect)
	DDX_Control(pDX, IDC_DONLYSISE, m_ckSISEONLY);
	DDX_Control(pDX, IDC_DSAVEPASS, m_ckSAVEPASS);
	DDX_Control(pDX, IDC_DSAVEID, m_ckSAVEID);
	DDX_Control(pDX, IDC_DBITMAP, m_static);
	DDX_Text(pDX, IDC_DUSER, m_user);
	DDV_MaxChars(pDX, m_user, 30);
	DDX_Text(pDX, IDC_DPASS, m_pass);
	DDX_Text(pDX, IDC_DCPASS, m_cPass);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_DSAVEPW, m_ckSavePw);
	DDX_Control(pDX, IDC_DFILEDOWN, m_ckFileDown);
	DDX_Control(pDX, IDC_DAUTOCONNECT, m_ckAutoConnect);
	//  DDX_Control(pDX, IDC_DRUNX, m_btnclose);
	DDX_Control(pDX, IDC_DUSER, m_edituser);
	DDX_Control(pDX, IDC_EDIT1, m_edithan);
}

BEGIN_MESSAGE_MAP(CConnect, CDialog)
	//{{AFX_MSG_MAP(CConnect)
	ON_BN_CLICKED(IDC_DRUN, OnRun)
	ON_BN_CLICKED(IDC_DEXIT, OnMember)
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DSETUP, OnMemberModify)
	ON_BN_CLICKED(IDC_DSAVEID, OnSaveID)
	ON_BN_CLICKED(IDC_DRUNX, OnExit)
	ON_BN_CLICKED(IDC_DONLYSISE, OnOnlysise)
	ON_BN_CLICKED(IDC_BN_CA, OnWorkCall)
	ON_BN_CLICKED(IDC_BN_BREAK, OnRemote)
	ON_BN_CLICKED(IDC_BN_HELP, OnMsgLogin)
	ON_BN_CLICKED(IDC_BN_CENTER, OnUniKey)
	ON_BN_CLICKED(IDC_BN_IDLOST, OnIDPassFind)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STOP, OnStop)
	ON_BN_CLICKED(IDC_DAUTOCONNECT, &CConnect::OnBnClickedDautoconnect)
	ON_BN_CLICKED(IDC_BN_MINI, &CConnect::OnSetup)
	ON_BN_CLICKED(IDC_DMSG, &CConnect::OnBnClickedDmsg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_DSAVEPW, &CConnect::OnBnClickedDsavepw)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_EN_SETFOCUS(IDC_DUSER, &CConnect::OnSetfocusDuser)
	ON_EN_KILLFOCUS(IDC_DUSER, &CConnect::OnKillfocusDuser)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnect message handlers

BOOL CConnect::OnInitDialog() 
{
	AfxEnableControlContainer();
	CDialog::OnInitDialog();

	CWinApp* app = AfxGetApp();
	m_brBkgnd.CreateSolidBrush(RGB(22,31,44));
	m_brBkgnd1.CreateSolidBrush(RGB(27,35,48));

//	HIMC himc = ImmGetContext(GetSafeHwnd());
//	ImmSetConversionStatus(himc, IME_CMODE_NATIVE, IME_SMODE_CONVERSATION);
//	ImmReleaseContext(GetSafeHwnd(), himc); 

#ifndef _DEBUG
//	InfoRSC();
#endif
	/*
	DWORD	dwConversion, dwSentence;
	HIMC hImc = ImmGetContext(this->m_hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
		ImmSetConversionStatus(hImc, dwConversion|IME_CMODE_HANGEUL, dwSentence);
	*/

	initButtons();  
	((CEdit*)GetDlgItem(IDC_DPASS))->SetLimitText(16);
	((CEdit*)GetDlgItem(IDC_DCPASS))->SetLimitText(30);

//	if (m_staff)
//		initStaff();
	
	CString	imgN;
	imgN.Format("%s\\%s\\axis.bmp", m_home, IMAGEDIR);

	m_bitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			imgN,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	
	if (m_bitmap)
	{
		m_skinShow = true;
		MakeShape();
	}
	else
		invisibleButton();

	m_static.SetBitmap(m_bitmap);
	initLogin();

	m_progress = new CProgressDlg(NULL, m_home);
	m_progress->Create(IDD_PROGRESS, this);
	m_progress->ShowWindow(SW_HIDE);
	
	m_bnGUIDE.SetTextColor(RGB(255, 255, 255));
	m_bnGUIDE.SetFont(&m_bFont);
	
	m_cust	   = app->GetProfileInt(WORKSTATION, "cust", 0)   ? TRUE : FALSE;
	m_saveID   = app->GetProfileInt(WORKSTATION, "SaveID", 0)   ? TRUE : FALSE;
	m_savePass = app->GetProfileInt(WORKSTATION, "SavePass", 0) ? TRUE : FALSE;
	m_onlysise = app->GetProfileInt(WORKSTATION, "OnlySise", 0) ? TRUE : FALSE;
	m_bAutologin = app->GetProfileInt(WORKSTATION, "AutoLogin", 0) ? TRUE : FALSE;

	if (!IsexistHangul(m_user))
		((CEdit *)GetDlgItem(IDC_DUSER))->SetPasswordChar(m_saveID ? '*' : 0);

	if (!m_auto && !m_saveID)	m_user.Empty();

	m_by = app->GetProfileInt(INFORMATION, CONNECTBY, byTCP);

	CenterWindow(GetDesktopWindow());
	
	if (m_saveID)	
	{
		((CButton *) GetDlgItem(IDC_DSAVEID))->SetCheck(TRUE);
		GetDlgItem(IDC_DUSER)->SetWindowText(m_user);
	}

	if (m_savePass)	
	{
		((CButton *) GetDlgItem(IDC_DSAVEPW))->SetCheck(TRUE);
		GetDlgItem(IDC_DPASS)->SetWindowText(GetdecPassword());
	}
	else
		((CButton *) GetDlgItem(IDC_DSAVEPW))->SetCheck(FALSE);

	if(m_bAutologin)
		((CButton *) GetDlgItem(IDC_DAUTOCONNECT))->SetCheck(TRUE);
	else
		((CButton *) GetDlgItem(IDC_DAUTOCONNECT))->SetCheck(FALSE);

	UpdateData(FALSE);

	if (m_bAutologin)
	{
		if (!m_user.IsEmpty() && m_pass.IsEmpty())
			GotoDlgCtrl(GetDlgItem(IDC_DPASS));
		else	
			GotoDlgCtrl(GetDlgItem(IDC_DRUN));
	
		GetDlgItem(IDC_DPASS)->SetWindowText(GetdecPassword());
		PostMessage(WM_COMMAND, IDC_DRUN);
		return FALSE;
	}

	if (!m_user.IsEmpty() && m_pass.IsEmpty())
	{
		GotoDlgCtrl(GetDlgItem(IDC_DPASS));
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConnect::initStaff()
{
	UINT	ids[] = {IDC_DONLYSISE, IDC_DSAVEPASS, IDC_DCPASS };
	int	ncnt = sizeof(ids) / sizeof(UINT);

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		GetDlgItem(ids[ii])->ShowWindow(SW_HIDE);	
	}
}

void CConnect::OnRun() 
{

#ifdef _DEBUG 
//	CString strdata;
//	GetDlgItem(IDC_DPASS)->GetWindowText(strdata);
//	if(strdata.IsEmpty())
//		GetDlgItem(IDC_DPASS)->SetWindowText("swadm!10");
#endif
	if(m_frame->m_bServerError == true)
	{
		OnMsgLogin();
		return;
	}

	if (((CAxisApp *)AfxGetApp())->m_progK != PK_BUSINESS)
	{
		CString	pass, id;

		GetDlgItem(IDC_DUSER)->GetWindowText(id);
		/*
		if (id.GetLength() < 4)
		{
			SetGuide("ID를 입력하세요");
			GetDlgItem(IDC_DUSER)->SetWindowText("");
			GetDlgItem(IDC_DUSER)->SetFocus();
			return;
		}
		*/

		GetDlgItem(IDC_DPASS)->GetWindowText(pass);
		/*
		if (pass.GetLength() < 4)
		{
			SetGuide("비밀번호를 입력하세요");
			GetDlgItem(IDC_DPASS)->SetWindowText("");
			GetDlgItem(IDC_DPASS)->SetFocus();
			return;
		}
		*/
	}

	UpdateData(TRUE);

	if (!m_bnRUN.IsEnable())	
		return;
	
	Save();
	SetStatus(SM_RSM);

	/*  //프로그램 다시 받기
	if (((CButton*)GetDlgItem(IDC_PROGRAM))->GetCheck())
	{
		CString strInfoASIS;
		strInfoASIS.Format("%s\\%s\\InfoAXIS", m_home, TABDIR);
		DeleteFile(strInfoASIS);
	}
	*/  //리소스 다시 받기 
	if (((CButton*)GetDlgItem(IDC_DFILEDOWN))->GetCheck())
	{
		CString strInfoRSC;
		strInfoRSC.Format("%s\\%s\\InfoRSC", m_home, TABDIR);
		DeleteFile(strInfoRSC);
		((CButton*)GetDlgItem(IDC_DFILEDOWN))->SetCheck(false);
	}

	CString id;
	GetDlgItem(IDC_DUSER)->GetWindowText(id);
	TRIM(id);
	if(m_frame->IsUUidCreated(id))  
		m_frame->PostMessage(WM_AXIS, MAKEWPARAM(axSTART, 1));

	m_bnRUN.EnableWindow(false);
}

void CConnect::OnExit() 
{
	m_frame->PostMessage(WM_CLOSE);
	EndDialog(IDCANCEL);
}

int CConnect::urlCheck() 
{
	return false;
	CString	file;
	char	buf[1024];

	file.Format("%s\\%s\\web.ini", m_home, TABDIR);
	GetPrivateProfileString("web", "checkurl", "", buf, sizeof(buf), file);

	CString	url = buf;
	url.TrimRight();
	if (url.IsEmpty())	
		return true;
	ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);

	return false;
}

void CConnect::OnMember() 
{
	CString	file;
	char	buf[1024];

	file.Format("%s\\%s\\web.ini", m_home, TABDIR);
	GetPrivateProfileString("web", "member", "", buf, sizeof(buf), file);

	CString	url = buf;
	url.TrimRight();
	if (url.IsEmpty())	return;
	ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);
}

void CConnect::OnLButtonDown(UINT nFlags, CPoint point) 
{
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

CString CConnect::GetUserID()
{
	if (IsWindow(m_hWnd))
		UpdateData(TRUE);

	return m_user;
}

CString CConnect::GetdecPassword()
{
	CWinApp* app = AfxGetApp();
	m_pass =m_frame->Onl_seeddec(app->GetProfileString(WORKSTATION, _T("PW")));
	return m_pass;
}

CString CConnect::GetPassword()
{
	CString passS = m_pass;
	if (IsWindow(m_hWnd))
		UpdateData(TRUE);

	return passS;
}

CString CConnect::GetCPass()
{
	if (IsWindow(m_hWnd))
		UpdateData(TRUE);

	CString cpassS = m_cPass;

	return cpassS;
}

void CConnect::SetUserID(CString user)
{
	m_user = user;
	if (IsWindow(m_hWnd))
		UpdateData(FALSE);
}

void CConnect::SetPassword(CString pass)
{
	m_pass = pass;
	m_auto = true;
	if (IsWindow(m_hWnd))
		UpdateData(FALSE);
}

void CConnect::SetCPass(CString pass)
{
	m_cPass = pass;
	if (IsWindow(m_hWnd))
		UpdateData(FALSE);
}

void CConnect::Save()
{
	CWinApp* app = AfxGetApp();
	if (((CButton *) GetDlgItem(IDC_DSAVEID))->GetCheck() == BST_CHECKED)
		m_saveID = TRUE;
	else	
		m_saveID = FALSE;

	if (((CButton *) GetDlgItem(IDC_DSAVEPW))->GetCheck() == BST_CHECKED)
		m_savePass = TRUE;
	else	
		m_savePass = FALSE;

	if (((CButton *) GetDlgItem(IDC_DAUTOCONNECT))->GetCheck() == BST_CHECKED)
		m_bAutologin = TRUE;
	else	
		m_bAutologin = FALSE;

	app->WriteProfileInt(WORKSTATION, "SaveID", m_saveID ? 1 : 0);
	app->WriteProfileInt(WORKSTATION, "SavePass", m_savePass ? 1 : 0);
	app->WriteProfileInt(WORKSTATION, "OnlySise", m_onlysise ? 1 : 0);

	app->WriteProfileString(WORKSTATION, SIGNONID, GetUserID() + "|");
	app->WriteProfileString(WORKSTATION, "LOGINID", GetUserID());
	app->WriteProfileInt(WORKSTATION, "AutoLogin", m_bAutologin ? 1 : 0);

	if(m_savePass) 
		app->WriteProfileString(WORKSTATION, "PW",  m_frame->Onl_seedenc(GetPassword()));
	else
		app->WriteProfileString(WORKSTATION, "PW",  _T(""));
}

void CConnect::SetStatus(int mode)
{
	m_mode = mode;
	if (!IsWindow(m_hWnd))
		return;
	int nnn;
	switch (mode)
	{
	case SM_EXIT:
		GetDlgItem(IDC_DUSER)->EnableWindow(TRUE);
		GetDlgItem(IDC_DPASS)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCPASS)->EnableWindow(TRUE);

		m_bnRUN.EnableWindow(true);
		m_bnEXIT.SetEnable(true);
		m_bnSETUP.SetEnable(false);
		break;
	case SM_RSM:
		GetDlgItem(IDC_DUSER)->EnableWindow(FALSE);
		GetDlgItem(IDC_DPASS)->EnableWindow(FALSE);
		GetDlgItem(IDC_DCPASS)->EnableWindow(FALSE);

		m_bnRUN.EnableWindow(false);
		m_bnEXIT.SetEnable(true);
		m_bnSETUP.SetEnable(false);
		break;
	case SM_WSH:
		GetDlgItem(IDC_DUSER)->EnableWindow(TRUE);
		GetDlgItem(IDC_DPASS)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCPASS)->EnableWindow(TRUE);

		m_bnRUN.EnableWindow(false);
		m_bnEXIT.SetEnable(true);
		m_bnSETUP.SetEnable(true);

		nnn =  m_bnRUN.IsEnable();
		break;
	case SM_NO:
	default:
		GetDlgItem(IDC_DUSER)->EnableWindow(TRUE);
		GetDlgItem(IDC_DPASS)->EnableWindow(TRUE);
		GetDlgItem(IDC_DCPASS)->EnableWindow(TRUE);

		m_bnRUN.EnableWindow(true);
		m_bnEXIT.SetEnable(true);
		m_bnSETUP.SetEnable(true);
		break;
	}
}

void CConnect::SetChoice(BOOL choice)
{
	if (IsWindow(m_hWnd))
	{
		GetDlgItem(IDC_DUSER)->EnableWindow(choice);
		GetDlgItem(IDC_DPASS)->EnableWindow(choice);
		GetDlgItem(IDC_DCPASS)->EnableWindow(choice);

		m_bnRUN.EnableWindow(choice  ? true : false);
		m_bnEXIT.SetEnable(choice ? true : false);
	}
}

void CConnect::SetGuide(CString msg, bool bMsgbox)
{
	if (IsWindow(m_hWnd))
	{
		m_bnGUIDE.SetText(msg);
		if(bMsgbox)
			::MessageBox(m_hWnd, msg, "K-Bond", MB_ICONINFORMATION);	
	}

}

void CConnect::SetGuide(CString msg, CString detail)
{
	if (IsWindow(m_hWnd))
	{
		msg += '\t';
		msg += detail;
	}
}

void CConnect::ClearPassword()
{
	if (IsWindow(m_hWnd))
	{
		m_pass = _T("");
		GetDlgItem(IDC_DPASS)->SetWindowText(m_pass);
		GetDlgItem(IDC_DPASS)->SetFocus();
	}
}

void CConnect::SetTotal(CString total)
{
	m_progress->SetTotal(total);
}

void CConnect::SetProgress(bool onoff)
{
	if (!IsWindow(m_hWnd))	return;

	if (onoff)
	{
		if (!m_progress->IsWindowVisible())
		{
			m_progress->ShowWindow(SW_SHOW);
			ShowWindow(SW_HIDE);
		}
	}
	else
	{
		if (m_progress->IsWindowVisible())
		{
			m_progress->ShowWindow(SW_HIDE);
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
		}
	}
}

void CConnect::SetProgress(CString detail, int rate, BOOL error)
{
	if (!IsWindow(m_hWnd))	return;

	if (m_progress->IsWindowVisible())
		m_progress->SetProgress(detail, rate, error);
}

void CConnect::OnClose() 
{
	if (m_framerun)
		EndDialog(IDOK);
	else	EndDialog(IDCANCEL);

	CDialog::OnClose();
}

void CConnect::OnMemberModify() 
{
	CString	file;
	char	buf[1024];

	file.Format("%s\\%s\\web.ini", m_home, TABDIR);
	GetPrivateProfileString("web", "membermodify", "", buf, sizeof(buf), file);

	CString	url = buf;
	url.TrimRight();
	if (url.IsEmpty())	return;
	ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);
}

void CConnect::OnBnClickedDsavepw()
{
	
}

void CConnect::OnSaveID() 
{ 
	UpdateData(TRUE);

	if (((CButton *) GetDlgItem(IDC_DSAVEID))->GetCheck() == BST_CHECKED)
		m_saveID = TRUE;
	else	
		m_saveID = FALSE;

	if (!IsexistHangul(m_user))
		((CEdit *)GetDlgItem(IDC_DUSER))->SetPasswordChar(m_saveID ? '*' : 0);
	GetDlgItem(IDC_DUSER)->SetWindowText(m_user);
}

void CConnect::invisibleButton()
{
	GetDlgItem(IDC_DSTATIC1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DSTATIC2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DSTATIC3)->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_DRUN)->ModifyStyle(BS_OWNERDRAW, 0);
	GetDlgItem(IDC_DEXIT)->ModifyStyle(BS_OWNERDRAW, 0);
	GetDlgItem(IDC_DSETUP)->ModifyStyle(BS_OWNERDRAW, 0);
}

void CConnect::initButtons()
{	
	CWnd*	item;
	CRect	wRc;
	CString	path, path_, path__,  text, imgName;
	int	idcX[] = { IDC_DRUN, IDC_DEXIT, IDC_DSETUP, IDC_DMSG, IDC_BN_CA, IDC_BN_BREAK, IDC_BN_MINI,
				IDC_BN_CENTER, IDC_BN_HELP, IDC_BN_IDLOST, IDC_DRUNX };

	imgName = _T("AXIS");
	path.Format("%s/%s/%s.bmp",     m_home, IMAGEDIR, imgName);
	path_.Format("%s/%s/%s_.bmp",   m_home, IMAGEDIR, imgName);
	path__.Format("%s/%s/%s__.bmp", m_home, IMAGEDIR, imgName);
	
	for (int ii = 0; ii < sizeof(idcX) / sizeof(int); ii++)
	{
		item = GetDlgItem(idcX[ii]);
		item->GetWindowRect(&wRc);
		ScreenToClient(&wRc);
		switch (idcX[ii])
		{
		case IDC_DRUN:
			if (m_bnRUN)	continue;
			VERIFY(m_bnRUN.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_DEXIT:
			if (m_bnEXIT)	continue;
			VERIFY(m_bnEXIT.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_DSETUP:
			if (m_bnSETUP)	continue;
			VERIFY(m_bnSETUP.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_DMSG:
			if (m_bnGUIDE) continue;	 	
			VERIFY(m_bnGUIDE.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_BN_CA:
			if (m_bnCA)	continue;
			VERIFY(m_bnCA.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_BN_BREAK:
			if (m_bnBREAK)	continue;
			VERIFY(m_bnBREAK.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_BN_MINI:
			if (m_bnMINI)	continue;
			VERIFY(m_bnMINI.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_BN_CENTER:
			if (m_bnCENTER)	continue;
			VERIFY(m_bnCENTER.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_BN_HELP:
			if (m_bnHELP)	continue;
			VERIFY(m_bnHELP.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_BN_IDLOST:
			if (m_bnIDLOST)	continue;
			VERIFY(m_bnIDLOST.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		case IDC_DRUNX:
			if (m_btnclose)	continue;
			VERIFY(m_btnclose.Attach(idcX[ii], this, wRc, path, path_, path__));	break;
		}
	}
	
	m_bnMINI.SetEBcolor(RGB(34,39,38));
	m_bnCA.SetEBcolor(RGB(34,39,38));
	m_bnBREAK.SetEBcolor(RGB(34,39,38));
	m_bnHELP.SetEBcolor(RGB(34,39,38)); 
	m_bnEXIT.SetEBcolor(RGB(44,47,53));
	m_bnSETUP.SetEBcolor(RGB(44,47,53));
	m_bnIDLOST.SetEBcolor(RGB(44,47,53));
	m_bnCENTER.SetEBcolor(RGB(44,47,53));
	m_bnGUIDE.SetEBcolor(RGB(44,47,53));
}

HRGN CConnect::DIBToRgn(HBITMAP hBmp, COLORREF BkColor, BOOL Direct)
{
	// use to return the handle of the HGRN
  	HRGN hRgn = NULL;					
	#define MAX_ALLOC_RECTS  100
	//the difference of the color
	COLORREF  Tolerance=BkColor;//0x00101010;
	int	r = GetRValue(0x111111);
	int	g = GetGValue(0x111111);
	int	b = GetBValue(0x111111);
	if (hBmp)
	{
		//creat the dib to save the dc
		HDC hMemDC = CreateCompatibleDC(NULL);		
		if (hMemDC)
		{
			BITMAP bm;
			//get the info of the bitmap
			GetObject(hBmp, sizeof(bm), &bm);	

			BITMAPINFOHEADER BmpInfoh = {					//the struct of the bitmap
					sizeof(BITMAPINFOHEADER),			// biSize
					bm.bmWidth,					// biWidth;
					bm.bmHeight,					// biHeight;
					1,						// biPlanes;
					32,						// biBitCount
					BI_RGB,						// biCompression;
					0,						// biSizeImage;
					0,						// biXPelsPerMeter;
					0,						// biYPelsPerMeter;
					0,						// biClrUsed;
					0						// biClrImportant;
			};
			//design a void point to point to the bitmap
			LPVOID pBit32; 
			//creat a DIB
			HBITMAP hDib32 = CreateDIBSection(hMemDC, 
					(BITMAPINFO *)&BmpInfoh, 
					DIB_RGB_COLORS, &pBit32, NULL, 0);
			if (hDib32)
			{
				//copy dib to DC
				HBITMAP hOldib32 = (HBITMAP)SelectObject(hMemDC, hDib32);
				// create a DC to save orgin bitmap
				HDC hDC = CreateCompatibleDC(hMemDC);
				if (hDC)
				{
					BITMAP bm32;
					// get the new 34 bit Dib size
					GetObject(hDib32, sizeof(bm32), &bm32);
					//make sure the 32Dib's every line pilex's is 4 's times
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;
					//copy the orginal dib to DC
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					//copy dib to memory DC
					BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);
					DWORD MaxRects = MAX_ALLOC_RECTS;
					SYSTEM_INFO  Sysinfo;
					//get memory size
					GetSystemInfo(&Sysinfo);
					//make a stack which can chang big
					//alloct memory
					HANDLE hRcData=HeapCreate(HEAP_GENERATE_EXCEPTIONS,Sysinfo.dwPageSize, 0);
					RGNDATA * pRcData=(RGNDATA*)HeapAlloc(hRcData,HEAP_ZERO_MEMORY,
						sizeof(RGNDATAHEADER)+sizeof(RECT)*MaxRects);
					 //fill the the RGNDATA struck
					pRcData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pRcData->rdh.iType = RDH_RECTANGLES;
					pRcData->rdh.nCount = pRcData->rdh.nRgnSize = 0;
					SetRect(&pRcData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
		             		BYTE hr,hg,hb,lr,lg,lb;
					switch(BkColor)
					{
					case RGB(255,255,255):	//if the bkcolor is white
						hr = GetRValue(BkColor);
						hg = GetGValue(BkColor);
						hb = GetBValue(BkColor);
						lr = min(0xff, hr - GetRValue(Tolerance));
						lg = min(0xff, hg - GetGValue(Tolerance));
						lb = min(0xff, hb - GetBValue(Tolerance));
						break;
					case RGB(0,0,0):	//if the bkcolor is black
						lr = GetRValue(BkColor);
						lg = GetGValue(BkColor);
						lb = GetBValue(BkColor);
						hr = min(0xff, lr + GetRValue(Tolerance));
						hg = min(0xff, lg + GetGValue(Tolerance));
						hb = min(0xff, lb + GetBValue(Tolerance));
						break;
					default:		//if the bkcolor is other color
						Tolerance=0x111111;
						lr =max(0, GetRValue(BkColor)-GetRValue(Tolerance));
						lg = max(0,GetGValue(BkColor)-GetGValue(Tolerance));
						lb = max(0,GetBValue(BkColor)-GetBValue(Tolerance));
						hr=min(0xff,GetRValue(BkColor)+GetRValue(Tolerance));
						hg=min(0xff,GetGValue(BkColor)+GetGValue(Tolerance));
						hb=min(0xff,GetBValue(BkColor)+GetBValue(Tolerance));
						break;
					}
					// Get the bit point and do the search
					BYTE *pBits = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++)
					{
						for (int x = 0; x < bm.bmWidth; x++)
						{
							int x0 = x;
							DWORD *pColor = (DWORD *)pBits + x;
							BYTE dr,dg,db;
							while (x < bm.bmWidth)
							{
								dr=GetRValue(*pColor);
								dg=GetGValue(*pColor);
								db=GetBValue(*pColor);

								if ((dr>= lr && dr<= hr)&&(dg>=lg&&dg<=hg)&&(db>=lb&&db<=hb))
								{
									if(Direct)
										break;
									else
									{
										pColor++;
										x++;
									}
							  	}
								else if(Direct)
								{
									pColor++;
									x++;
								}
								else
									break;

							}
							if (x > x0)
							{
								if (pRcData->rdh.nCount >= MaxRects)
								{
									MaxRects += MAX_ALLOC_RECTS;
									//re alloc the stack
									pRcData=(RGNDATA*)HeapReAlloc(
									hRcData,HEAP_ZERO_MEMORY,pRcData, 
									sizeof(RGNDATAHEADER)+sizeof(RECT)*MaxRects);
								}
								RECT *pr = (RECT *)&pRcData->Buffer;
								SetRect(&pr[pRcData->rdh.nCount], x0, y, x, y+1);
								pRcData->rdh.rcBound.left = x0;
								pRcData->rdh.rcBound.top = y;
								pRcData->rdh.rcBound.right = x;
								pRcData->rdh.rcBound.bottom = y+1;
								pRcData->rdh.nCount++;

								if (pRcData->rdh.nCount == 3000)
								{	
									HRGN tmphRgn = ExtCreateRegion(NULL,
									sizeof(RGNDATAHEADER) + (sizeof(RECT) * MaxRects),
									pRcData);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, tmphRgn, RGN_OR);
										DeleteObject(tmphRgn);
									}
									else
										hRgn = tmphRgn;
									pRcData->rdh.nCount = 0;
									SetRect(&pRcData->rdh.rcBound, 
									MAXLONG, MAXLONG, 0, 0);
								}
							}
						}

						// search next line
						pBits -= bm32.bmWidthBytes;
					}
					HRGN tmphRgn = ExtCreateRegion(NULL, 
							sizeof(RGNDATAHEADER) + (sizeof(RECT) * MaxRects), pRcData);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, tmphRgn, RGN_OR);
						DeleteObject(tmphRgn);
					}
					else
						hRgn = tmphRgn;
					// make a rect ,use this rect xor to the  BkColor
					//then we can get the rect we want
					if(!Direct)
					{
						HRGN hRect=CreateRectRgn(0,0,bm.bmWidth,bm.bmHeight);
				                        if(hRect)
						{
							CombineRgn(hRgn,hRgn,hRect,RGN_XOR);
							DeleteObject(hRect);
						}
					    else
							return NULL;
					}
					//release the memory
					HeapFree(hRcData,HEAP_NO_SERIALIZE,pRcData);
					SelectObject(hDC, holdBmp);
					DeleteDC(hDC);
					DeleteObject(holdBmp);
				}
				SelectObject(hMemDC,hOldib32);
				DeleteDC(hMemDC);
				DeleteObject(hOldib32);
				DeleteObject(hDib32);
			}
			else
				DeleteDC(hMemDC);
		}
	}
	return hRgn;
}

void CConnect::InfoRSC()
{
	char		mapN[32], ver[32], vkey[32], size[32], path[32], buf[256];
	FILE		*fp, *wfp;
	HANDLE		hFind;
	CString		writefile, readfile, searchfile;
	WIN32_FIND_DATA FindFileData;

	writefile.Format("%s\\tab\\infoRSC", m_home);
	readfile.Format("%s\\tab\\infoRSC_", m_home);
	CopyFile(writefile, readfile, TRUE);

	errno_t err;
	err = fopen_s(&fp, readfile, "r");
	if (err)	return;

	err = fopen_s(&wfp, writefile, "w+");


	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		scanf_s(buf, "%s %s %s %s %s", mapN, ver, vkey, size, path);
		searchfile.Format("%s\\%s\\%s", m_home, path, mapN);
		hFind = FindFirstFile(searchfile, &FindFileData);
		
		if (hFind != INVALID_HANDLE_VALUE)
			fprintf(wfp, buf);
	}

	fclose(wfp);
	fclose(fp);
	DeleteFile(readfile);
}

CWnd* CConnect::GetWnd(UINT nRes)
{
	return GetDlgItem(nRes);
}

void CConnect::OnOnlysise() 
{
	if (((CButton*) GetDlgItem(IDC_DONLYSISE))->GetCheck() == BST_CHECKED)
	{
#ifdef NDEBUG
		CString	msg;
		msg = "시세/계좌조회만 가능하고 주문 및 이체등은\n불가하도록 로그인 하는 기능입니다.";
		::MessageBox(m_hWnd, msg, "시세/계좌 전용 접속", MB_ICONINFORMATION);
#endif
		GetDlgItem(IDC_DCPASS)->EnableWindow(FALSE);
	}
	else	GetDlgItem(IDC_DCPASS)->EnableWindow(TRUE);	
}

void CConnect::initLogin()
{
	m_ckSAVEID.SetBitmaps(IDB_CHECK_ON, RGB(255, 0, 255), IDB_CHECK_OFF, RGB(255, 0, 255));
	m_ckSAVEID.DrawTransparent();

	m_ckSAVEPASS.SetBitmaps(IDB_CHECK_ON, RGB(255, 0, 255), IDB_CHECK_OFF, RGB(255, 0, 255));
	m_ckSAVEPASS.DrawTransparent();

	m_ckSISEONLY.SetBitmaps(IDB_CHECK_ON, RGB(255, 0, 255), IDB_CHECK_OFF, RGB(255, 0, 255));
	m_ckSISEONLY.DrawTransparent();

	m_ckSavePw.SetBitmaps(IDB_CHECK_ON, RGB(255, 0, 255), IDB_CHECK_OFF, RGB(255, 0, 255));
	m_ckSavePw.DrawTransparent();

	m_ckFileDown.SetBitmaps(IDB_CHECK_ON, RGB(255, 0, 255), IDB_CHECK_OFF, RGB(255, 0, 255));
	m_ckFileDown.DrawTransparent();

	m_ckAutoConnect.SetBitmaps(IDB_CHECK_ON, RGB(255, 0, 255), IDB_CHECK_OFF, RGB(255, 0, 255));
	m_ckAutoConnect.DrawTransparent();


	for (int ii = 0; ii <= 5; ii++)
	{
		m_ckSAVEID.SetColor(ii, RGB(0, 0, 0));
		m_ckSAVEPASS.SetColor(ii, RGB(0, 0, 0));
		m_ckSISEONLY.SetColor(ii, RGB(0, 0, 0));
	}

	m_bnGUIDE.SetText("");
	m_bnGUIDE.SetAlign(DT_CENTER);
}

void CConnect::MakeShape()
{
	HRGN hWndRgn = DIBToRgn(m_bitmap, RGB(255, 0, 255), FALSE);
	if(hWndRgn)
	{
		SetWindowRgn(hWndRgn,TRUE);
		::DeleteObject(hWndRgn);
	}
}

void CConnect::OnMsgLogin() 
{
	m_frame->SendMsgToMessanger(1);
	CString path,data, id;
	path.Format(_T("%s\\%s"), m_home, _T("KBondMessenger\\KBondMessenger.exe"));
	data.Format(_T("BTS|%d %s|%s %s"),AfxGetMainWnd()->m_hWnd, GetUserID(), GetPassword(), _T("N"));  //m_user, m_name
	ShellExecute(AfxGetMainWnd()->m_hWnd, "open",  path, data, NULL, SW_SHOWNORMAL);
}

void CConnect::OnRemote() 
{
	CString	file;
	char	buf[1024];

	file.Format("%s\\%s\\web.ini", m_home, TABDIR);
	GetPrivateProfileString("web", "Remote", "", buf, sizeof(buf), file);

	CString	url = buf;
	url.TrimRight();
	if (url.IsEmpty())	return;
	ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);
}

void CConnect::OnWorkCall() 
{
	CString	file;
	char	buf[1024];

	file.Format("%s\\%s\\web.ini", m_home, TABDIR);
	GetPrivateProfileString("web", "workcall", "", buf, sizeof(buf), file);

	CString	url = buf;
	url.TrimRight();
	if (url.IsEmpty())	return;
	ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);
}

void CConnect::OnUniKey() 
{
	CString	file;
	char	buf[1024];

	file.Format("%s\\%s\\web.ini", m_home, TABDIR);
	GetPrivateProfileString("web", "unikey", "", buf, sizeof(buf), file);

	CString	url = buf;
	url.TrimRight();
	if (url.IsEmpty())	return;
	ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);
}

void CConnect::OnIDPassFind() 
{
	CString	file;
	char	buf[1024];

	file.Format("%s\\%s\\web.ini", m_home, TABDIR);
	GetPrivateProfileString("web", "idpass", "", buf, sizeof(buf), file);

	CString	url = buf;
	url.TrimRight();
	if (url.IsEmpty())	return;
	ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);
}

BOOL CConnect::IsexistHangul(CString id)
{
	int	len = id.GetLength();
	for (int ii = 0; ii < len; ii++)
	{
		if (id.GetAt(ii) & 0x80)
			return TRUE;
	}
	return FALSE;
}

void CConnect::useKeyProtect(bool bSet)
{

}

int CConnect::valid_usid(CString usid)
{
        int     ii, dlen = usid.GetLength();
        char    w_usid[16], emsg[256];

        /*** 고객ID 중복 check는 제외 */
        memset(w_usid, 0x00, sizeof(w_usid));
        dlen = (int)strlen(usid);

        /* 자릿수 4 ~ 8 */
        if (dlen < 4)
        {
                sprintf_s(emsg, "고객ID는 4 ~ 8 자리만 가능합니다!");
                return -1;
        }

        /* 한글, 특수문자, 스페이스 사용 불가 */
        for (ii = 0; ii < dlen; ii++)
        {
                if (!isalnum(usid[ii]))
                        break;
                w_usid[ii] = usid[ii];
        }

        if (ii != dlen)
        {
                sprintf_s(emsg, "고객ID는 한글, 특수문자, 스페이스 사용이 불가합니다.!");
                return -1;
        }

        /* ID 첫문자는 반드시 영문자 */	// 사원번호때문에 체크 하지 않음
	if (!isalpha(w_usid[0]) && m_by != byTCP)
        {
                sprintf_s(emsg, "고객ID 첫문자는 영문자만 가능합니다!");
                return -1;
        }

        /* 사용이 불가능한 ID */
	if (m_by == byTCP)
	{
		CString xid[] = {"gilson", "sonnim", "guest", "" };
		
		for (ii = 0; xid[ii] != ""; ii++)
		{
			if (xid[ii].CompareNoCase(usid) == 0)
			{
				sprintf_s(emsg, "사용이 불가능한 고객ID입니다!");
				return -1;
			}
		}
	}
	else
	{
		CString xid[] = {"gilson", "sonnim", "guest", "root", "hitc", "" };
		
		for (ii = 0; xid[ii] != ""; ii++)
		{
			if (xid[ii].CompareNoCase(usid) == 0)
			{
				sprintf_s(emsg, "사용이 불가능한 고객ID입니다!");
				return -1;
			}
		}		
	}
	return 0;
}

void CConnect::OnDestroy()
{
	CDialog::OnDestroy();
}

BOOL CConnect::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		//if (pMsg->wParam == VK_PROCESSKEY)  //이걸 하면 한글키 변환이 안되서 영문만 입력된다
		//	return TRUE;
		if(pMsg->wParam == VK_RETURN)
		{
			OnRun();
			return TRUE;
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CConnect::OnStop(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CConnect::OnBnClickedDautoconnect()
{   //자동로그인
	/*
	if(((CButton *) GetDlgItem(IDC_DAUTOCONNECT))->GetCheck())
		((CButton *) GetDlgItem(IDC_DSAVEID))->SetCheck(TRUE);
	else
		((CButton *) GetDlgItem(IDC_DSAVEID))->SetCheck(FALSE);
	*/
}

void CConnect::OnSetup()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//return;
	CWinApp* app = AfxGetApp();
	CDlgModaless dlg;
	dlg.m_home = m_home;
	if (dlg.DoModal() == IDCANCEL)
		return;

	CStringArray arr;
	dlg.GetResultArr(arr);

	if(arr.GetCount() > 0)
	{
		if(arr.GetAt(0) == "1")  //프로그램 다시 받기
		{
			CString strInfoAXIS;
			strInfoAXIS.Format("%s\\%s\\InfoAXIS", m_home, TABDIR);
			DeleteFile(strInfoAXIS);
		}
		
		if(arr.GetAt(1) == "1")   //SDI 모드 사용하기
			app->WriteProfileInt(INFORMATION, "SDI", 1);
		else
			app->WriteProfileInt(INFORMATION, "SDI", 0);
	}
}

void CConnect::OnBnClickedBnHelp2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CConnect::OnBnClickedDmsg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}

HBRUSH CConnect::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO:  여기서 DC의 특성을 변경합니다.
	UINT nRes = pWnd->GetDlgCtrlID();
	if(nRes == IDC_DUSER )
	{
		hbr = (HBRUSH)m_brBkgnd; 
		pDC->SetBkColor(RGB(22,31,44));
		pDC->SetTextColor(RGB(255,255,255));
	}
	else if( nRes == IDC_DPASS) 
	{
		hbr = (HBRUSH)m_brBkgnd1; 
		pDC->SetBkColor(RGB(27,35,48));
		pDC->SetTextColor(RGB(255,255,255));
	}
	return hbr;
}


void CConnect::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
}



BOOL CConnect::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	  RECT  rect;
	  GetClientRect (&rect);
	  pDC->FillSolidRect (&rect, RGB(47,52,59));
	  return 1;
	//return CDialog::OnEraseBkgnd(pDC);
}


void CConnect::OnSetfocusDuser()
{	
	/*
	DWORD	dwConversion,dwSentence;
	HIMC hImc = ImmGetContext(this->m_hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		m_Conversion = dwConversion;
		m_Sentence = dwSentence;
		dwConversion = IME_CMODE_HANGEUL;
		::ImmSetConversionStatus(hImc, dwConversion, IME_SMODE_NONE);
		::ImmReleaseContext(GetSafeHwnd(), hImc); 
	}
	*/
}


void CConnect::OnKillfocusDuser()
{
	/*
	DWORD	dwConversion, dwSentence;

	HIMC hImc = ImmGetContext(this->m_hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		if (dwConversion & IME_CMODE_HANGEUL)
			dwConversion -= IME_CMODE_HANGEUL;
		ImmSetConversionStatus(hImc, IME_CMODE_ALPHANUMERIC, IME_SMODE_NONE);
		ImmReleaseContext(this->m_hWnd, hImc);
	}
	*/
	/*
	DWORD	dwConversion,dwSentence;

	HIMC hImc = ImmGetContext(m_edituser.m_hWnd);
	if ( ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		if (m_Conversion != dwConversion)
		{
			dwConversion = m_Conversion;
			dwSentence = m_Sentence;
			ImmSetConversionStatus(hImc, dwConversion, dwSentence);
		}
	}	
	*/
}
