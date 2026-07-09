// AccSave.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "AccSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccSave dialog


CAccSave::CAccSave(CWnd* pParent /*=NULL*/)
	: CDialog(CAccSave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccSave)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_parent = pParent;
}


void CAccSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccSave)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccSave, CDialog)
	//{{AFX_MSG_MAP(CAccSave)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SAVEACC, OnSaveACC)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccSave message handlers
LPARAM CAccSave::OnSaveACC(WPARAM wp, LPARAM lp)
{
	if (!m_parent)	m_parent = GetParent();
	return m_parent->SendMessage(WM_SAVEACC, wp, lp);
}

BOOL CAccSave::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_parent->SendMessage(WM_SAVEACC, 1, 0);
	CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAccSave::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
