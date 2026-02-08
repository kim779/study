// DlgAutoSave.cpp : implementation file
//

#include "stdafx.h"
#include "ib202200.h"
#include "DlgAutoSave.h"

#include "GroupWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgAutoSave dialog

#define TM_LOADTIMEOUT 9889
#define TM_LOADING   9888

DlgAutoSave::DlgAutoSave(CWnd* pParent /*=nullptr*/)
	: CDialog(DlgAutoSave::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgAutoSave)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_parent = (CGroupWnd*)pParent;
	m_strUploading = ".";
	m_istep = 0;
//	(CGroupWnd*)m_parent->SendUini();
}


void DlgAutoSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgAutoSave)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgAutoSave, CDialog)
	//{{AFX_MSG_MAP(DlgAutoSave)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgAutoSave message handlers

void DlgAutoSave::OnOK() 
{
	// TODO: Add extra validation here
	if(m_istep == 0)
	{
		Upload();
		return;
	}
	CDialog::OnOK();
}
void DlgAutoSave::UploadFinsh()
{
	CWnd* pwnd = GetDlgItem(IDC_UPLOAD);
	if(pwnd)
	{
		CString strdata;
		strdata = "관심종목 저장완료 하였습니다.";
		strdata += m_strUploading;
		pwnd->SetWindowText(strdata);
	}
	m_istep = 1;
	KillTimer(TM_LOADTIMEOUT);
	KillTimer(TM_LOADING);
	GetDlgItem(IDOK)->SetWindowText("확인");
	GetDlgItem(IDOK)->EnableWindow(true);
	CDialog::OnOK();
}


void DlgAutoSave::Upload()
{
	CWnd* pwnd = GetDlgItem(IDC_UPLOAD);
	if(pwnd)
	{
		CString strdata;
		strdata = "관심종목 내역을 저장중입니다";
		strdata += m_strUploading;
		pwnd->SetWindowText(strdata);
	}
	
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	m_parent->SendMessage(WM_MANAGE, MK_CLOSEAUTOSAVE, 0);
	SetTimer(TM_LOADTIMEOUT, 5000, nullptr);
	SetTimer(TM_LOADING, 1000, nullptr);
}

BOOL DlgAutoSave::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


LONG DlgAutoSave::OnManage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case 0x99:
		{
			KillTimer(TM_LOADTIMEOUT);
			UploadFinsh();
		}
		break;	
	}

	return 0;
}

void DlgAutoSave::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
		case TM_LOADTIMEOUT:
			{
				UploadFinsh();
			}
		break;
		case TM_LOADING:
			{
				m_strUploading += ".";
				if(m_strUploading.GetLength() >=5)
					m_strUploading = ".";

				CWnd* pwnd = GetDlgItem(IDC_UPLOAD);
				if(pwnd)
				{
					CString strdata;
					strdata = "관심종목 내역을 저장중입니다";
					strdata += m_strUploading;
					pwnd->SetWindowText(strdata);
				}
				Invalidate();
			}
		break;
	
	}

	CDialog::OnTimer(nIDEvent);
}

void DlgAutoSave::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	

	// Do not call CDialog::OnPaint() for painting messages
}

void DlgAutoSave::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL DlgAutoSave::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
			case VK_RETURN:
				{
					if(m_istep == 0)
					{
						Upload();
						return true;
					}
				}
				break;
			case VK_ESCAPE :
						return true;
					break;	
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
