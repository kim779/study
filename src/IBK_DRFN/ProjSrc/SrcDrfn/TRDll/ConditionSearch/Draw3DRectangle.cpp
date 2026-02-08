// Draw3DRectangle.cpp: implementation of the CDraw3DRectangle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "MakeDrawLine.h"
#include "Draw3DRectangle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#define MGN_LEFT	3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDraw3DRectangle::CDraw3DRectangle()
{
	m_strText.Empty();
	m_rcWnd.SetRectEmpty();
	m_pFont = NULL;
	m_bOuterLine = TRUE;
	m_bTransParent = FALSE;
	m_b3D = TRUE;
	m_bGradient = FALSE;
	m_bMouseUp = FALSE;
	m_bFrame = FALSE;
	m_nScroll = 0;
	m_nScrollX = 0;
	m_nScrollY = 0;
	m_nAlign = DT_LEFT|DT_SINGLELINE|DT_VCENTER;
	m_clrBack = RGB(255, 255, 255);
	m_clrText = RGB(0, 0, 0);
}

CDraw3DRectangle::~CDraw3DRectangle()
{
	delete m_pFont;
}


//####################################################################
// CDraw3DRectangle Member Functions
//####################################################################
// Set Text
void CDraw3DRectangle::SetText(char *szText)
{
	// Scroll 좌표 설정
	switch(m_nScroll)
	{
		case 1 : m_nScrollX = (short)m_rcWnd.left;  m_nScrollY = (short)m_rcWnd.bottom;  break;	// 위로
		case 2 : m_nScrollX = (short)m_rcWnd.left;  m_nScrollY = (short)(m_rcWnd.top-m_rcWnd.Height());  break;	// 아래로
		case 3 : m_nScrollX = (short)m_rcWnd.right;  m_nScrollY = (short)m_rcWnd.top;  break;	// 좌로
		case 4 : m_nScrollX = (short)(m_rcWnd.left-m_rcWnd.Width());  m_nScrollY = (short)m_rcWnd.top;  break;	// 우로
	}

	m_strText = szText;
//	InvalidateRect(&m_rcWnd);
}

// Auto Scroll 설정
void CDraw3DRectangle::SetScroll(short nScroll)
{
	m_nScroll = nScroll;
//	if(m_nScroll) SetTimer(100, 1, NULL);
}

// Output Draw
void CDraw3DRectangle::DrawOutput(CDC *pDC)
{
	if(!m_pFont) return;
	CFont *pOldFont = pDC->SelectObject(m_pFont);

	COLORREF clrOldText = pDC->SetTextColor(m_clrText);

	CRect rc = m_rcWnd;
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	// 외곽선 그리구
	if(!m_bTransParent) pDC->FillSolidRect(rc, m_clrBack);
	if(m_bFrame)
	{
		pDC->Draw3dRect(rc, ::GetSysColor(COLOR_3DHILIGHT), ::GetSysColor(COLOR_3DSHADOW));
		rc.DeflateRect(1, 1);
		pDC->Draw3dRect(rc, ::GetSysColor(COLOR_3DHILIGHT), ::GetSysColor(COLOR_3DDKSHADOW));
		rc.DeflateRect(2, 2);
		
//		pDC->Draw3dRect(rc, ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_3DHILIGHT));
//		rc.DeflateRect(1, 1);
		pDC->Draw3dRect(rc, ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_3DHILIGHT));
	}
	else if(m_bOuterLine) DrawRectangle(pDC, rc);
	if(m_bGradient) DrawGradient(pDC, m_clrBack);
	if(m_b3D)
	{
		pDC->Draw3dRect(rc, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DHILIGHT));
		rc.DeflateRect(1, 1);
		pDC->Draw3dRect(rc, ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_3DFACE));
	}
	// 좌우 마진
	if(m_nAlign & DT_RIGHT) rc.right -= MGN_LEFT;
	else if(m_nAlign & DT_CENTER);
	else rc.left += MGN_LEFT;
	// Scroll인 경우에는 영역을 따로 설정
	switch(m_nScroll)
	{
		case 1 : case 2 : rc.bottom = m_nScrollY + rc.Height();  rc.top = m_nScrollY;  break;
		case 3 : case 4 : rc.right = m_nScrollX + rc.Width();  rc.left = m_nScrollX;  break;
	}
	// Text 표시
	pDC->DrawText(m_strText, rc, m_nAlign);

	pDC->SetBkMode(nOldMode);
	pDC->SetTextColor(clrOldText);
	pDC->SelectObject(pOldFont);
}

// 외곽선 그리기
void CDraw3DRectangle::DrawRectangle(CDC *pDC, CRect rc)
{
	pDC->MoveTo(rc.left,    rc.top);       pDC->LineTo(rc.right-1, rc.top);
	pDC->MoveTo(rc.right-1, rc.top);       pDC->LineTo(rc.right-1, rc.bottom-1);
	pDC->MoveTo(rc.right-1, rc.bottom-1);  pDC->LineTo(rc.left,    rc.bottom-1);
	pDC->MoveTo(rc.left,    rc.bottom-1);  pDC->LineTo(rc.left,    rc.top);
}

void CDraw3DRectangle::DrawTitleFrame(CDC* pDC)
{
	CDC MemDC;
	if(!MemDC.CreateCompatibleDC(pDC)) return;
	CBitmap NewBitmap, *pOldBitmap = NULL;
	if(!NewBitmap.CreateCompatibleBitmap(pDC, m_rcWnd.Width(), m_rcWnd.Height())) return;
	pOldBitmap = MemDC.SelectObject(&NewBitmap);

	DrawOutput(&MemDC);		// Output 그리기

	pDC->BitBlt(0, 0, m_rcWnd.Width(), m_rcWnd.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
}

// Text 정렬 상태 설정
void CDraw3DRectangle::SetAlign(short nAlign)
{
//	switch(nAlign)
//	{
//		case TALIGN_LEFT   : m_nAlign = DT_LEFT|DT_SINGLELINE|DT_VCENTER;    break;
//		case TALIGN_RIGHT  : m_nAlign = DT_RIGHT|DT_SINGLELINE|DT_VCENTER;   break;
//		case TALIGN_CENTER : m_nAlign = DT_CENTER|DT_SINGLELINE|DT_VCENTER;  break;
//	}
}

void CDraw3DRectangle::SetFontStyle()
{
	m_pFont = new CFont;

	LOGFONT	LogFont;
	LogFont.lfHeight	=	12;    
	LogFont.lfWidth	=	0;    
	LogFont.lfEscapement=	0;    
	LogFont.lfOrientation=	0; 
	LogFont.lfWeight	=	400;
	LogFont.lfItalic	=	0;    
	LogFont.lfUnderline	=	0;        
	LogFont.lfStrikeOut	=	0;     
	LogFont.lfCharSet	=	129;
	LogFont.lfOutPrecision	=	3;        
	LogFont.lfClipPrecision	=	2;     
	LogFont.lfQuality	=	1;        
	LogFont.lfPitchAndFamily=	49;        
	strcpy( LogFont.lfFaceName, (const char*)_T("굴림체") );
	m_pFont->CreateFontIndirect( &LogFont );
}

// Gradation 그리기
void CDraw3DRectangle::DrawGradient(CDC *pDC, COLORREF clrStart)
{
	CRect rc = m_rcWnd;
	pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNFACE), RGB(0, 0, 0));
	rc.DeflateRect(1, 1);
	pDC->Draw3dRect(rc, RGB(255, 255, 255), ::GetSysColor(COLOR_BTNFACE));
	rc.DeflateRect(1, 1);

	COLORREF clrEnd = RGB(255, 255, 255);

	int r = GetRValue(clrEnd) - GetRValue(clrStart);
	int g = GetGValue(clrEnd) - GetGValue(clrStart);
	int b = GetBValue(clrEnd) - GetBValue(clrStart);
	int Max = max(abs(r), max(abs(g), abs(b)));

	float fStep = (float)rc.Height()/(float)Max;
	float rStep = r / (float)Max;
	float gStep = g / (float)Max;
	float bStep = b / (float)Max;

	r = GetRValue(clrStart);
	g = GetGValue(clrStart);
	b = GetBValue(clrStart);

	CRect rcTmp = rc;
	for(int i = 0; i < Max; i++)
	{
		::SetRect(&rc, rc.left, rcTmp.top, rc.right, rcTmp.bottom-int(fStep*i));
		pDC->FillSolidRect(&rc, RGB(r+rStep*i, g+gStep*i, b+bStep*i));
	}	
}



