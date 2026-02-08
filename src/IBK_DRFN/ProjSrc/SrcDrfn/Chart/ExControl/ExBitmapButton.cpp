// ExBitmapButton.cpp : implementation file
//

#include "stdafx.h"
#include "../../Inc/ExBitmapButton.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExBitmapButton

CExBitmapButton::CExBitmapButton()
{
	m_pLoader = new CLoaderBitmapButton(this);
}

CExBitmapButton::~CExBitmapButton()
{
	delete m_pLoader;
}


BEGIN_MESSAGE_MAP(CExBitmapButton, CExButton)
	//{{AFX_MSG_MAP(CExBitmapButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExBitmapButton message handlers
long CExBitmapButton::ActionControl(LPVOID lpAction)
{
	return m_pLoader->ActionControl(lpAction);
}

BOOL CExBitmapButton::SetBitmapFile(LPCTSTR lpszUp, LPCTSTR lpszDown, LPCTSTR lpszDis/* = NULL*/)
{
	return m_pLoader->SetBitmapFile(lpszUp, lpszDown, lpszDis);
}

void CExBitmapButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	m_pLoader->DrawItem(lpDrawItemStruct);
}
