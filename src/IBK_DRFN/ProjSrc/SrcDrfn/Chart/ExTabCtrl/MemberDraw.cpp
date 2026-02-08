// MemberDraw.cpp: implementation of the CMemberDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExTabCtrl.h"
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

CMemberDraw::CMemberDraw(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin) : CMember(lpControl, bUseDefSkin),
		m_nPrevPointedItem(-1),m_bOverTab(false),
		m_nActiveItem(-1),m_bFloatPaint(false),
		m_rectUpDn(0,0,0,0),m_rctPage(0,0,0,0),
		m_bAutoRaising(false),m_bHasFocus(false),
		m_hPenShadow(NULL), 
		m_hPenItemShadow(NULL), 
		m_hMenuFont(NULL), 
		m_hBoldFont(NULL),
		m_hPenLight(NULL),
		m_bInvalidatePage(FALSE)
{
	m_pTabCtrl = (CTabCtrl_Friendly*)lpControl;

	SetSkinColorManager(pISkinColorManager);
	SetDefualtSkin();
	m_bGradation = FALSE;
}

CMemberDraw::~CMemberDraw()
{
	if(m_hPenShadow)
		::DeleteObject(m_hPenShadow);
	if(m_hPenLight)
		::DeleteObject(m_hPenLight);
	if(m_hPenItemShadow)
		::DeleteObject(m_hPenItemShadow);
	if(m_hMenuFont)
		::DeleteObject(m_hMenuFont);
	if(m_hBoldFont)
		::DeleteObject(m_hBoldFont);

	m_listForgCtrl.RemoveAll();
}


void CMemberDraw::SetDefualtSkin()
{
	m_nNormal = 20;
	m_nText = 2;
	m_nSelect = 34;
	m_nLine = 97;
	m_nBackground = 1;

	SetSkinColor();
}

void CMemberDraw::SetSkinColor()
{
//	m_clrNormal = RGB(0,0,255);
//	m_clrNormal = RGB(193,193,193);		//RGB(251,252,252);
	m_clrNormal = RGB(189,189,189);		//RGB(251,252,252);
	m_clrText = RGB(255,0,0);
	m_clrSelect = RGB(193,180,130);
	m_clrLine = RGB(189,189,189);		//RGB(0,0,0);
	m_clrBackground = RGB(213,213,213);	// RGB(222,222,222);
	//m_clrNormal = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nNormal) : m_pISkinColorManager->GetColor(m_nNormal);
	//m_clrText = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nText) : m_pISkinColorManager->GetColor(m_nText);
	//m_clrSelect = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nSelect) : m_pISkinColorManager->GetColor(m_nSelect);
	//m_clrLine = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nLine) : m_pISkinColorManager->GetColor(m_nLine);
	//m_clrBackground = m_bUseOnlyDefaultSkinTable ? m_pISkinColorManager->GetDefaultColor(m_nBackground) : m_pISkinColorManager->GetColor(m_nBackground);
}

void CMemberDraw::AddForeCtrl(CWnd* pCtrl)
{
	POSITION pos = m_listForgCtrl.Find(pCtrl);
	if(!pos)
		m_listForgCtrl.AddTail(pCtrl);
}

void CMemberDraw::RemoveForeCtrl(CWnd* pCtrl)
{
	POSITION pos = m_listForgCtrl.Find(pCtrl);
	if(pos)
		m_listForgCtrl.RemoveAt(pos);
}

void CMemberDraw::SetTabColor(COLORREF clrBkgrnd, COLORREF clrActive, COLORREF clrInactive)
{
	m_clrBackground = clrBkgrnd;
	m_clrSelect = clrActive;
	m_clrNormal = clrInactive;
}

void CMemberDraw::SetSTTabColor(long lSTTabColor)
{
	STTabColor* pSTTabColor = (STTabColor*)lSTTabColor;
	if(pSTTabColor)
	{
		m_bGradation = TRUE;
		m_clrTabSelText = pSTTabColor->clrTabSelText;
		m_clrTabNorText = pSTTabColor->clrTabNorText;
		m_clrTabBkgrnd = pSTTabColor->clrTabBkgrnd;
		m_clrTabSelGDStt = pSTTabColor->clrTabSelGDStt;
		m_clrTabSelGDEnd = pSTTabColor->clrTabSelGDEnd;
		m_clrTabNorGDStt = pSTTabColor->clrTabNorGDStt;
		m_clrTabNorGDEnd = pSTTabColor->clrTabNorGDEnd;
	}
	else
		m_bGradation = FALSE;
}