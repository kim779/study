// MemberCheckBox.cpp: implementation of the CMemberCheckBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExButton.h"
#include "MemberCheckBox.h"
#include "../../inc/ISkinColorMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//void ChangeSkinTable()
//{
//	gThis->SetSkinColor();
//	gThis->m_pStatic->Invalidate();
//}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemberCheckBox::CMemberCheckBox(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin) : CMember(lpControl, bUseDefSkin)
{
	m_pButton = (CCheckBox_Friendly*)lpControl;
	SetSkinColorManager(pISkinColorManager);

	m_brush = NULL;
	SetDefaultColor();
}

CMemberCheckBox::~CMemberCheckBox()
{
	if(m_brush)
		::DeleteObject(m_brush);
}

void CMemberCheckBox::SetColor(COLORREF clrBackground, COLORREF clrText)
{
	m_bUseSkinTable = FALSE;

	m_clrBackground = clrBackground;
	m_clrText = clrText;

	if(m_brush)
		::DeleteObject(m_brush);

	m_brush = ::CreateSolidBrush(m_clrBackground);
}

void CMemberCheckBox::SetDefaultColor()
{
	m_bUseSkinTable = TRUE;
	m_nBackground = 1;
	m_nText = 2;

	SetSkinColor();
}

void CMemberCheckBox::SetSkinTable(UINT nBackground, UINT nText)
{
	m_bUseSkinTable = TRUE;
	m_nBackground = nBackground;
	m_nText = nText;

	SetSkinColor();
}

void CMemberCheckBox::SetSkinColor()
{
	m_clrBackground = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nBackground) : m_pISkinColorManager->GetColor(m_nBackground);
	m_clrText = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nText) : m_pISkinColorManager->GetColor(m_nText);

	if(m_brush)
		::DeleteObject(m_brush);

	m_brush = ::CreateSolidBrush(m_clrBackground);
}
