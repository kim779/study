// ExDrawButton.cpp : implementation file
//

#include "stdafx.h"
#include "../../Inc/ExDialog.h"
#include "../../Inc/ExDrawButton.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExDrawButton

CExDrawButton::CExDrawButton(BOOL bUseDefSkin/* = FALSE*/)
{
	m_pLoader = new CLoaderDrawButton(this, bUseDefSkin);
	m_posChangedSkin = NULL;
}

CExDrawButton::~CExDrawButton()
{
	delete m_pLoader;
}


BEGIN_MESSAGE_MAP(CExDrawButton, CExButton)
	//{{AFX_MSG_MAP(CExDrawButton)
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExDrawButton message handlers
long CExDrawButton::ActionControl(LPVOID lpAction)
{
	return m_pLoader->ActionControl(lpAction);
}

BOOL CExDrawButton::SetIconImage(CSize size, HICON hIcon, HICON hIconPushed/* = NULL*/, UINT nIconAlign/* = CBTN_ICON_NORMAL*/)
{
	return m_pLoader->SetIconImage(size, hIcon, hIconPushed, nIconAlign);
}

void CExDrawButton::SetText(LPCTSTR lpszText)
{
	m_pLoader->SetText(lpszText);
}

void CExDrawButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	m_pLoader->DrawItemButton(lpDrawItemStruct);
}

void CExDrawButton::OnDestroy() 
{
//	if(m_posChangedSkin)
//	{
//		((CExDialog*)GetParent())->RemoveChangeSkinNotifyFunction(m_posChangedSkin);
//		m_posChangedSkin = NULL;
//	}

	CExButton::OnDestroy();	
}

BOOL CExDrawButton::SetButtonColor(UINT nButton/* = 0*/)
{
	return m_pLoader->SetDrawButton(nButton);
}

void CExDrawButton::PreSubclassWindow() 
{
	CExButton::PreSubclassWindow();
	
	m_pLoader->PreSubclassWindow();
}

LPCTSTR CExDrawButton::GetText()
{
	return m_pLoader->GetText();
}

void CExDrawButton::UseToggleMode(BOOL bUse/* = TRUE*/)
{
	m_pLoader->UseToggleMode(bUse);
}

BOOL CExDrawButton::GetToggleState()
{
	return m_pLoader->GetToggleState();
}

void CExDrawButton::SetToggleState(BOOL bPush)
{
	m_pLoader->SetToggleState(bPush);
}

void CExDrawButton::ReleasePushed()
{
	m_pLoader->ReleasePushed();
}

BOOL CExDrawButton::OnClicked() 
{
	m_pLoader->OnClicked();

	return FALSE;
}

void CExDrawButton::AddToggleGroup(CExDrawButton* pButton, ...)
{
	va_list varToggle;
	va_start(varToggle, pButton);
	
	CExDrawButton* pAddButton = pButton;

	while(pAddButton)
	{
		m_pLoader->AddToggleGroup(pAddButton->m_pLoader->m_lpPointer);
		pAddButton = va_arg(varToggle, CExDrawButton*);
	}

	va_end(varToggle);
}