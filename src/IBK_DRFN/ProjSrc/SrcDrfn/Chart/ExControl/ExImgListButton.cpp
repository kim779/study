// ExImgListButton.cpp : implementation file
//

#include "stdafx.h"
#include "../../Inc/ExImgListButton.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExImgListButton
long CExImgListButton::ActionControl(LPVOID lpAction)
{
	return m_pLoader->ActionControl(lpAction);
}

CExImgListButton::CExImgListButton()
{
	m_pLoader = new CLoaderImgListButton(this);
}

CExImgListButton::~CExImgListButton()
{
	delete m_pLoader;
}


BEGIN_MESSAGE_MAP(CExImgListButton, CExButton)
	//{{AFX_MSG_MAP(CExImgListButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExImgListButton message handlers
void CExImgListButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	m_pLoader->DrawItem(lpDrawItemStruct);
}

BOOL CExImgListButton::SetImageList(CImageList* pImageList, int nNormal, int nDown, int nDisable, int nOver, LPCTSTR lpszText/* = NULL*/, COLORREF clrText/* = 0x000000*/, COLORREF clrTextDis/* = 0x000000*/)
{
	return m_pLoader->SetImageList(pImageList, nNormal, nDown, nDisable, nOver, lpszText, clrText, clrTextDis);
}
	
void CExImgListButton::SetText(LPCTSTR lpszText)
{
	m_pLoader->SetText(lpszText);
}

LPCTSTR CExImgListButton::GetText()
{
	return m_pLoader->GetText();
}