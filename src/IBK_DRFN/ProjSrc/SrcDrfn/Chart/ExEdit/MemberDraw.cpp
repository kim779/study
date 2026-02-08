// MemberDraw.cpp: implementation of the CMemberDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExEdit.h"
#include "MemberDraw.h"

#include "../../inc/ISkinColorMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemberDraw::CMemberDraw(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin) : CMember(lpControl, bUseDefSkin)
{
	m_pEdit = (CEdit_Friendly*)lpControl;

	SetSkinColorManager(pISkinColorManager);
	SetDefualtSkin();

	m_bNumericType = FALSE;
}

CMemberDraw::~CMemberDraw()
{

}

void CMemberDraw::SetDefualtSkin()
{
	m_nLine = 31;
	m_nText = 2;
	m_nBackground = 0;

	SetSkinColor();
}

void CMemberDraw::SetSkinColor()
{
	m_clrLine = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nLine) : m_pISkinColorManager->GetColor(m_nLine);
	m_clrText = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nText) : m_pISkinColorManager->GetColor(m_nText);
	m_clrBackground = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nBackground) : m_pISkinColorManager->GetColor(m_nBackground);
}