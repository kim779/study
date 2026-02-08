// MemberDraw.cpp: implementation of the CMemberDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExButton.h"
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
	if(!gThis->m_nOutLine)
		return;

	gThis->SetSkinColor();
	gThis->m_pButton->Invalidate();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemberDraw::CMemberDraw(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin) : CMember(lpControl, bUseDefSkin)
{
	m_pButton = (CButton_Friendly*)lpControl;
	m_lpszText = NULL;

	m_hIcon = m_hIconPushed = NULL;
	m_sizeImage = CSize(0, 0);

	m_bUseToggleMode = m_bPushed = FALSE;
	
	SetSkinColorManager(pISkinColorManager);
	SetDefaultColor();

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
	m_nOutLine = 33;
	m_nOutLineTopLeft = 34;
	m_nInLineTopLeft = 35;
	m_nInLineBottomRight = 36;
	m_nOutLineBottomRight = 37;
	m_nBackground = 38;
	m_nText = 2;

	m_nOutLineOver = 41;
	m_nOutLineTopLeftOver = 42;
	m_nInLineTopLeftOver = 43;
	m_nInLineBottomRightOver = 44;
	m_nOutLineBottomRightOver = 45;
	m_nBackgroundOver = 46;
	m_nTextOver = 2;

	m_nOutLineDown = 49;
	m_nOutLineTopLeftDown = 50;
	m_nInLineTopLeftDown = 51;
	m_nInLineBottomRightDown = 52;
	m_nOutLineBottomRightDown = 53;
	m_nBackgroundDown = 54;
	m_nTextDown = 2;

	m_nOutLineDis = 178;
	m_nOutLineTopLeftDis = 179;
	m_nInLineTopLeftDis = 180;
	m_nInLineBottomRightDis = 181;
	m_nOutLineBottomRightDis = 182;
	m_nBackgroundDis = 183;
	m_nTextDis = 2;

	SetSkinColor();
}

void CMemberDraw::SetAccentColor()
{
	m_nOutLine = 57;
	m_nOutLineTopLeft = 58;
	m_nInLineTopLeft = 59;
	m_nInLineBottomRight = 60;
	m_nOutLineBottomRight = 61;
	m_nBackground = 62;
	m_nText = 2;

	m_nOutLineOver = 65;
	m_nOutLineTopLeftOver = 66;
	m_nInLineTopLeftOver = 67;
	m_nInLineBottomRightOver = 68;
	m_nOutLineBottomRightOver = 69;
	m_nBackgroundOver = 70;
	m_nTextOver = 2;

	m_nOutLineDown = 73;
	m_nOutLineTopLeftDown = 74;
	m_nInLineTopLeftDown = 75;
	m_nInLineBottomRightDown = 76;
	m_nOutLineBottomRightDown = 77;
	m_nBackgroundDown = 78;
	m_nTextDown = 2;

	m_nOutLineDis = 178;
	m_nOutLineTopLeftDis = 179;
	m_nInLineTopLeftDis = 180;
	m_nInLineBottomRightDis = 181;
	m_nOutLineBottomRightDis = 182;
	m_nBackgroundDis = 183;
	m_nTextDis = 2;

	SetSkinColor();
}

void CMemberDraw::SetTabColor()
{
	m_nOutLine = 81;
	m_nOutLineTopLeft = 82;
	m_nInLineTopLeft = 83;
	m_nInLineBottomRight = 84;
	m_nOutLineBottomRight = 85;
	m_nBackground = 86;
	m_nText = 2;

	m_nOutLineOver = 90;
	m_nOutLineTopLeftOver = 91;
	m_nInLineTopLeftOver = 92;
	m_nInLineBottomRightOver = 93;
	m_nOutLineBottomRightOver = 94;
	m_nBackgroundOver = 95;
	m_nTextOver = 2;

	m_nOutLineDown = 98;
	m_nOutLineTopLeftDown = 99;
	m_nInLineTopLeftDown = 100;
	m_nInLineBottomRightDown = 101;
	m_nOutLineBottomRightDown = 102;
	m_nBackgroundDown = 103;
	m_nTextDown = 2;

	m_nOutLineDis = 178;
	m_nOutLineTopLeftDis = 179;
	m_nInLineTopLeftDis = 180;
	m_nInLineBottomRightDis = 181;
	m_nOutLineBottomRightDis = 182;
	m_nBackgroundDis = 183;
	m_nTextDis = 2;

	SetSkinColor();
}

void CMemberDraw::SetBuyColor()
{
	m_nOutLine = 106;
	m_nOutLineTopLeft = 107;
	m_nInLineTopLeft = 108;
	m_nInLineBottomRight = 109;
	m_nOutLineBottomRight = 110;
	m_nBackground = 111;
	m_nText = 2;

	m_nOutLineOver = 114;
	m_nOutLineTopLeftOver = 115;
	m_nInLineTopLeftOver = 116;
	m_nInLineBottomRightOver = 117;
	m_nOutLineBottomRightOver = 118;
	m_nBackgroundOver = 119;
	m_nTextOver = 2;

	m_nOutLineDown = 122;
	m_nOutLineTopLeftDown = 123;
	m_nInLineTopLeftDown = 124;
	m_nInLineBottomRightDown = 125;
	m_nOutLineBottomRightDown = 126;
	m_nBackgroundDown = 127;
	m_nTextDown = 2;

	m_nOutLineDis = 178;
	m_nOutLineTopLeftDis = 179;
	m_nInLineTopLeftDis = 180;
	m_nInLineBottomRightDis = 181;
	m_nOutLineBottomRightDis = 182;
	m_nBackgroundDis = 183;
	m_nTextDis = 2;

	SetSkinColor();
}

void CMemberDraw::SetSellColor()
{
	m_nOutLine = 130;
	m_nOutLineTopLeft = 131;
	m_nInLineTopLeft = 132;
	m_nInLineBottomRight = 133;
	m_nOutLineBottomRight = 134;
	m_nBackground = 135;
	m_nText = 2;

	m_nOutLineOver = 138;
	m_nOutLineTopLeftOver = 139;
	m_nInLineTopLeftOver = 140;
	m_nInLineBottomRightOver = 141;
	m_nOutLineBottomRightOver = 142;
	m_nBackgroundOver = 143;
	m_nTextOver = 2;

	m_nOutLineDown = 146;
	m_nOutLineTopLeftDown = 147;
	m_nInLineTopLeftDown = 148;
	m_nInLineBottomRightDown = 149;
	m_nOutLineBottomRightDown = 150;
	m_nBackgroundDown = 151;
	m_nTextDown = 2;

	m_nOutLineDis = 178;
	m_nOutLineTopLeftDis = 179;
	m_nInLineTopLeftDis = 180;
	m_nInLineBottomRightDis = 181;
	m_nOutLineBottomRightDis = 182;
	m_nBackgroundDis = 183;
	m_nTextDis = 2;

	SetSkinColor();
}

void CMemberDraw::SetModifyColor()
{
	m_nOutLine = 154;
	m_nOutLineTopLeft = 155;
	m_nInLineTopLeft = 156;
	m_nInLineBottomRight = 157;
	m_nOutLineBottomRight = 158;
	m_nBackground = 159;
	m_nText = 2;

	m_nOutLineOver = 162;
	m_nOutLineTopLeftOver = 163;
	m_nInLineTopLeftOver = 164;
	m_nInLineBottomRightOver = 165;
	m_nOutLineBottomRightOver = 166;
	m_nBackgroundOver = 167;
	m_nTextOver = 2;

	m_nOutLineDown = 170;
	m_nOutLineTopLeftDown = 171;
	m_nInLineTopLeftDown = 172;
	m_nInLineBottomRightDown = 173;
	m_nOutLineBottomRightDown = 174;
	m_nBackgroundDown = 175;
	m_nTextDown = 2;

	m_nOutLineDis = 178;
	m_nOutLineTopLeftDis = 179;
	m_nInLineTopLeftDis = 180;
	m_nInLineBottomRightDis = 181;
	m_nOutLineBottomRightDis = 182;
	m_nBackgroundDis = 183;
	m_nTextDis = 2;

	SetSkinColor();
}

void CMemberDraw::SetCancelColor()
{
	m_nOutLine = 199;
	m_nOutLineTopLeft = 200;
	m_nInLineTopLeft = 201;
	m_nInLineBottomRight = 202;
	m_nOutLineBottomRight = 203;
	m_nBackground = 204;
	m_nText = 2;

	m_nOutLineOver = 207;
	m_nOutLineTopLeftOver = 208;
	m_nInLineTopLeftOver = 209;
	m_nInLineBottomRightOver = 210;
	m_nOutLineBottomRightOver = 211;
	m_nBackgroundOver = 212;
	m_nTextOver = 2;

	m_nOutLineDown = 215;
	m_nOutLineTopLeftDown = 216;
	m_nInLineTopLeftDown = 217;
	m_nInLineBottomRightDown = 218;
	m_nOutLineBottomRightDown = 219;
	m_nBackgroundDown = 220;
	m_nTextDown = 2;

	m_nOutLineDis = 178;
	m_nOutLineTopLeftDis = 179;
	m_nInLineTopLeftDis = 180;
	m_nInLineBottomRightDis = 181;
	m_nOutLineBottomRightDis = 182;
	m_nBackgroundDis = 183;
	m_nTextDis = 2;

	SetSkinColor();
}

void CMemberDraw::SetSkinColor()
{
	m_clrOutLine = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLine) : m_pISkinColorManager->GetColor(m_nOutLine);
	m_clrOutLineTopLeft = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLineTopLeft) : m_pISkinColorManager->GetColor(m_nOutLineTopLeft);
	m_clrInLineTopLeft = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nInLineTopLeft) : m_pISkinColorManager->GetColor(m_nInLineTopLeft);
	m_clrInLineBottomRight = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nInLineBottomRight) : m_pISkinColorManager->GetColor(m_nInLineBottomRight);
	m_clrOutLineBottomRight = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLineBottomRight) : m_pISkinColorManager->GetColor(m_nOutLineBottomRight);
	m_clrBackground = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nBackground) : m_pISkinColorManager->GetColor(m_nBackground);
	m_clrText = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nText) : m_pISkinColorManager->GetColor(m_nText);
	
	m_clrOutLineOver = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLineOver) : m_pISkinColorManager->GetColor(m_nOutLineOver);
	m_clrOutLineTopLeftOver = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLineTopLeftOver) : m_pISkinColorManager->GetColor(m_nOutLineTopLeftOver);
	m_clrInLineTopLeftOver = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nInLineTopLeftOver) : m_pISkinColorManager->GetColor(m_nInLineTopLeftOver);
	m_clrInLineBottomRightOver = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nInLineBottomRightOver) : m_pISkinColorManager->GetColor(m_nInLineBottomRightOver);
	m_clrOutLineBottomRightOver = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLineBottomRightOver) : m_pISkinColorManager->GetColor(m_nOutLineBottomRightOver);
	m_clrBackgroundOver = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nBackgroundOver) : m_pISkinColorManager->GetColor(m_nBackgroundOver);
	m_clrTextOver = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nTextOver) : m_pISkinColorManager->GetColor(m_nTextOver);

	m_clrOutLineDown = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLineDown) : m_pISkinColorManager->GetColor(m_nOutLineDown);
	m_clrOutLineTopLeftDown = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLineTopLeftDown) : m_pISkinColorManager->GetColor(m_nOutLineTopLeftDown);
	m_clrInLineTopLeftDown = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nInLineTopLeftDown) : m_pISkinColorManager->GetColor(m_nInLineTopLeftDown);
	m_clrInLineBottomRightDown = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nInLineBottomRightDown) : m_pISkinColorManager->GetColor(m_nInLineBottomRightDown);
	m_clrOutLineBottomRightDown = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLineBottomRightDown) : m_pISkinColorManager->GetColor(m_nOutLineBottomRightDown);
	m_clrBackgroundDown = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nBackgroundDown) : m_pISkinColorManager->GetColor(m_nBackgroundDown);
	m_clrTextDown = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nTextDown) : m_pISkinColorManager->GetColor(m_nTextDown);

	m_clrOutLineDis = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLineDis) : m_pISkinColorManager->GetColor(m_nOutLineDis);
	m_clrOutLineTopLeftDis = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLineTopLeftDis) : m_pISkinColorManager->GetColor(m_nOutLineTopLeftDis);
	m_clrInLineTopLeftDis = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nInLineTopLeftDis) : m_pISkinColorManager->GetColor(m_nInLineTopLeftDis);
	m_clrInLineBottomRightDis = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nInLineBottomRightDis) : m_pISkinColorManager->GetColor(m_nInLineBottomRightDis);
	m_clrOutLineBottomRightDis = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nOutLineBottomRightDis) : m_pISkinColorManager->GetColor(m_nOutLineBottomRightDis);
	m_clrBackgroundDis = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nBackgroundDis) : m_pISkinColorManager->GetColor(m_nBackgroundDis);
//	m_clrTextDis = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nTextDis) : m_pISkinColorManager->GetColor(m_nTextDis);
	m_clrTextDis = ::GetSysColor(COLOR_GRAYTEXT);
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

LPCTSTR CMemberDraw::GetText()
{
	if(m_lpszText)
		return m_lpszText;

	return _T("");
}
