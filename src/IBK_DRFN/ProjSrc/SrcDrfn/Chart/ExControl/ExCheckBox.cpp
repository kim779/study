// ExCheckBox.cpp : implementation file
//

#include "stdafx.h"
#include "../../Inc/ExCheckBox.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CExCheckBox, CButton)

CExCheckBox::CExCheckBox(BOOL bUseDefSkin/* = FALSE*/)
{
	m_pLoader = new CLoaderCheckBoxButton(this, bUseDefSkin);
}
  
CExCheckBox::~CExCheckBox()
{
	delete m_pLoader;
}

// CExCheckBox message handlers
BEGIN_MESSAGE_MAP(CExCheckBox, CButton)
	//{{AFX_MSG_MAP(CExCheckBox)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

void CExCheckBox::SetColor(COLORREF clrText, COLORREF clrBackground)
{
	m_pLoader->SetColor(clrText, clrBackground);
}

void CExCheckBox::SetSkinString(LPCTSTR lpszText, LPCTSTR lpszBackground)
{
	m_pLoader->SetSkinString(lpszText, lpszBackground);
}

void CExCheckBox::SetSkinIndex(UINT nText, UINT nBackground)
{
	m_pLoader->SetSkinIndex(nText, nBackground);
}

HBRUSH CExCheckBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return m_pLoader->CtlColor(pDC, nCtlColor);
}
