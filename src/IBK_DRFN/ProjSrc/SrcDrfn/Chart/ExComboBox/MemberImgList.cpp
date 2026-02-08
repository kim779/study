// MemberImgList.cpp: implementation of the CMemberImgList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExComboBox.h"
#include "MemberImgList.h"

#include "../../inc/ISkinColorMng.h"
#define CLR_FONT_DEFAULT RGB(0,0,1)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemberImgList::CMemberImgList(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin) : CMember(lpControl, bUseDefSkin)
{
	m_pComboBox = (CComboBox_Friendly*)lpControl;
	m_bLBtnDown = FALSE;

	SetSkinColorManager(pISkinColorManager);
	SetDefaultColor();
}

CMemberImgList::~CMemberImgList()
{

}

void CMemberImgList::SetDefaultColor()
{
	m_nOutLine = 97;
	m_nBackground = 0;
	m_nText = 2;

	SetSkinColor();
}

void CMemberImgList::SetSkinColor()
{
	m_clrOutLine = CLR_FONT_DEFAULT;
	m_clrBackground = RGB(222,222,222);
	m_clrText = RGB(0,0,0);

	//m_clrOutLine = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLine) : m_pISkinColorManager->GetColor(m_nOutLine);
	//m_clrBackground = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nBackground) : m_pISkinColorManager->GetColor(m_nBackground);
	//m_clrText = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nText) : m_pISkinColorManager->GetColor(m_nText);
}
