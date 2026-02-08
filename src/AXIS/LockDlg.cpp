// LockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "LockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLockDlg dialog
#define WD_LOCKPASS			(WM_USER+0x8002)

CLockDlg::CLockDlg(CWnd* pParent, CString user, CString pass)
	: CDialog(CLockDlg::IDD, pParent)
{
	m_user = user;
	m_pass = pass;
	m_wizard = NULL;
	//{{AFX_DATA_INIT(CLockDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLockDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLockDlg, CDialog)
	//{{AFX_MSG_MAP(CLockDlg)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLockDlg message handlers

BOOL CLockDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

#ifdef DF_USE_CPLUS17
	m_changer = std::make_unique<CControlChanger>(this);
#else
	m_changer = new CControlChanger(this);
#endif


	if (!m_user.IsEmpty())
	{
		GetDlgItem(IDC_USER)->SetWindowText(m_user);
		NextDlgCtrl();
	}

// 	if(m_wizard != NULL)
// 	{
// 		GetDlgItem(IDC_LPASS)->ShowWindow(SW_HIDE);
// 		GetDlgItem(IDC_STATIC_PASS)->ShowWindow(SW_HIDE);
// 	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLockDlg::OnOK() 
{
	CString	user, pass;
	
	GetDlgItem(IDC_USER)->GetWindowText(user);
	GetDlgItem(IDC_LPASS)->GetWindowText(pass);

	if (m_user.Compare(user))
	{
		Axis::MessageBox(this, "로그인 사용자로만 잠금을 풀 수 있습니다", MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_USER));
	}
	else if (m_pass.Compare(pass))
	{
		m_iPassErrorCnt++;
		if (m_iPassErrorCnt == 5) //비밀번호 5회 오류
		{
			Axis::MessageBox(this, "비밀번호 입력 5회 오류입니다. HTS를 종료합니다.", MB_OK | MB_ICONEXCLAMATION);
			ShowWindow(SW_HIDE);
			GetParent()->SendMessage(WD_LOCKPASS, 1, 0);
			return;
		}
		CString stmp;
		stmp.Format(" 비밀번호를 입력 %d회 오류입니다. \r\n 5회 이상 비밀번호가 맞지 않는 경우 HTS 종료합니다", m_iPassErrorCnt);
		Axis::MessageBox(this, stmp, MB_OK | MB_ICONEXCLAMATION);
		GotoDlgCtrl(GetDlgItem(IDC_LPASS));
	}
	else
		CDialog::OnOK();

// 	if(m_wizard == NULL)
// 	{
// 		if (m_user.Compare(user))
// 		{
// 			Axis::MessageBox(this, "로그인 사용자로만 잠금을 풀 수 있습니다", MB_OK | MB_ICONEXCLAMATION);
// 			GotoDlgCtrl(GetDlgItem(IDC_USER));
// 		}
// 		else if (m_pass.Compare(pass))
// 		{
// 			Axis::MessageBox(this, "비밀번호를 정확히 입력하십시오.", MB_OK | MB_ICONEXCLAMATION);
// 			GotoDlgCtrl(GetDlgItem(IDC_LPASS));
// 		}
// 		else
// 			CDialog::OnOK();
// 	}
// 	else
// 	{
// 		char ca[6776];
// 		int ret;
// 		
// 		memset(ca,' ',6776);
// 		
// 		//CopyMemory(ca,m_axConnect->GetCPass(),m_axConnect->GetCPass().GetLength());
// 		
// 		m_wizard->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_I4, (void *)&ret,
// 			(BYTE *)(VTS_I4 VTS_I4), MAKELONG(caFULL, 1),ca);
// 
// 		if(ret == 0)
// 		{
// 			CString sCert(ca,sizeof(ca));
// 			sCert.TrimRight();
// 			
// 			CString sCSize = sCert.Mid(0,5);
// 			CString sDSize = sCert.Mid(5,5);
// 			
// 			int nCSize = atoi(sCSize);
// 			int nDSize = atoi(sDSize);
// 			
// 			CString sCAFull = sCert.Mid(10,nCSize);
// 			CString sDN = sCert.Mid(10 + nCSize,nDSize);
// 
// 			if(m_pass != sDN)
// 			{
// 				Axis::MessageBox(this, "로그인 사용자의 인증서로만 잠금을 풀 수 있습니다.", MB_OK | MB_ICONEXCLAMATION);
// 				return;
// 			}
// 			
// 			CDialog::OnOK();
// 		}
// 		else
// 		{
// 			Axis::MessageBox(this, "인증서를 올바르게 선택하고 비밀번호를 정확히 입력해 주세요.", MB_OK | MB_ICONEXCLAMATION);
// 		}
// 	}
}

void CLockDlg::OnCancel() 
{
	return;
}

BOOL CLockDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN))
	{
		if (GetDlgItem(IDC_LPASS)->m_hWnd == pMsg->hwnd)
			OnOK();
		else if (GetDlgItem(IDC_USER)->m_hWnd == pMsg->hwnd)
		{
			pMsg->wParam = VK_TAB;
			return CDialog::PreTranslateMessage(pMsg);
		}
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CLockDlg::OnEraseBkgnd(CDC* pDC) 
{
	if (Axis::dialogBgColor > 0)
	{
		CRect rc;
		GetClientRect(&rc);
		
		CBrush* oldBrush = pDC->SelectObject(&Axis::dialogBgBrush);

		pDC->PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(oldBrush);

		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

HBRUSH CLockDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CLockDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
#ifndef DF_USE_CPLUS17
	delete m_changer;	
#endif
}

void CLockDlg::SetWizard( CWnd* wizard )
{
	m_wizard = wizard;
}
