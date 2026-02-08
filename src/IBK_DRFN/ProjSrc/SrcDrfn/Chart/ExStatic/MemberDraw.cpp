// MemberDraw.cpp: implementation of the CMemberDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExStatic.h"
#include "MemberDraw.h"

#include "../../inc/ISkinColorMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMemberDraw* gThis;

void ChangeSkinTable()
{
	if(!gThis->m_bUseSkinTable)
		return;

	gThis->SetSkinColor();
	gThis->m_pStatic->Invalidate();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemberDraw::CMemberDraw(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin) : CMember(lpControl, bUseDefSkin)
{
	m_lpszText = NULL;
	m_hIcon = NULL;
	m_sizeImage = CSize(0, 0);
	
	SetSkinColorManager(pISkinColorManager);
	SetDefaultColor();

	m_pStatic = (CStatic_Friendly*)lpControl;

	gThis = this;
}

CMemberDraw::~CMemberDraw()
{
	if(m_lpszText)
	{
		::free(m_lpszText);
		m_lpszText = NULL;
	}
}

void CMemberDraw::SetDefaultColor()
{
	m_nOutLine = 1;
	m_nBackground = 1;
	m_nText = 2;

	SetSkinColor();
}

void CMemberDraw::SetSkinColor()
{
	m_bUseSkinTable = TRUE;

	m_clrOutLine = RGB(0,0,0);
	m_clrBackground = RGB(222,222,222);
	m_clrText = RGB(0,0,0);
	//m_clrOutLine = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLine) : m_pISkinColorManager->GetColor(m_nOutLine);
	//m_clrBackground = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nBackground) : m_pISkinColorManager->GetColor(m_nBackground);
	//m_clrText = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nText) : m_pISkinColorManager->GetColor(m_nText);
}

void CMemberDraw::SetMemberDraw(COLORREF clrOutLine, COLORREF clrBackground, COLORREF clrText)
{
	m_bUseSkinTable = FALSE;

	m_clrOutLine = clrOutLine;
	m_clrBackground = clrBackground;
	m_clrText = clrText;
}

void CMemberDraw::SetMemberIndex(UINT nOutLine, UINT nBackground, UINT nText)
{
	m_nOutLine = nOutLine;
	m_nBackground = nBackground;
	m_nText = nText;

	SetSkinColor();
}

void CMemberDraw::SetText(LPCTSTR lpszText)
{
	if(m_lpszText)
		::free(m_lpszText);

	m_lpszText = ::strdup(lpszText);
}

FpChangeSkinNotify CMemberDraw::GetNotifySkinTableFunction()
{
	return ChangeSkinTable;
}
