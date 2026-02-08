// ExCheckGroupBox.cpp : implementation file
//

#include "stdafx.h"
#include "../../Inc/ExCheckGroupBox.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_CHECKBOX		0xFFFE
#define	LEFT_OFFSET		12
#define	CHECKBOX_SIZE	16

// Data exchange for this control
void AFXAPI DDX_GroupCheck(CDataExchange* pDX, int nIDC, int& value)
{
	// This works because DDX_Check sends BM_GETCHECK and BM_SETCHECK to CGroupCheckBox
	// CGroupCheckBox then receives those messages and processes them.

	DDX_Check(pDX, nIDC, value);
}

/////////////////////////////////////////////////////////////////////////////
// CExCheckGroupBox

CExCheckGroupBox::CExCheckGroupBox()
{
	m_pLoader = new CLoaderCheckGroupBoxButton(this);
	m_pLoader->InitCheckGroupBox();
}

CExCheckGroupBox::~CExCheckGroupBox()
{
	delete m_pLoader;
}


BEGIN_MESSAGE_MAP(CExCheckGroupBox, CButton)
	//{{AFX_MSG_MAP(CExCheckGroupBox)
	ON_WM_SETFOCUS()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_CHECKBOX , OnClicked)
	ON_MESSAGE(BM_GETCHECK, OnGetCheck)
	ON_MESSAGE(BM_SETCHECK, OnSetCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExCheckGroupBox message handlers
void CExCheckGroupBox::OnClicked() 
{
	m_pLoader->OnClicked_CheckGroupBox();
}

void CExCheckGroupBox::SetStyle(TouchStyle nStyle)
{
	m_pLoader->SetStyle_CheckGroupBox(nStyle);
}

void CExCheckGroupBox::PreSubclassWindow() 
{
	m_pLoader->PreSubclassWindow_CheckGroupBox();
}

void CExCheckGroupBox::SetCheck(int nCheck)
{
	m_pLoader->SetCheck_CheckGroupBox(nCheck);
}

int CExCheckGroupBox::GetCheck()
{
	return m_pLoader->GetCheck_CheckGroupBox();
}

void CExCheckGroupBox::OnSetFocus(CWnd* pOldWnd) 
{
	m_pLoader->OnSetFocus_CheckGroupBox(pOldWnd);
}

LRESULT CExCheckGroupBox::OnGetCheck(WPARAM wParam, LPARAM lParam)
{
	return m_pLoader->OnGetCheck_CheckGroupBox(wParam, lParam);
}

LRESULT CExCheckGroupBox::OnSetCheck(WPARAM wParam, LPARAM lParam)
{
	return m_pLoader->OnSetCheck_CheckGroupBox(wParam, lParam);
}

void CExCheckGroupBox::OnEnable(BOOL bEnable) 
{
	m_pLoader->OnEnable_CheckGroupBox(bEnable);
}
