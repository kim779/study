// Draw3DRectangle.h: interface for the CDraw3DRectangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAW3DRECTANGLE_H__B4148023_98CF_440F_A884_2B8B9F18607B__INCLUDED_)
#define AFX_DRAW3DRECTANGLE_H__B4148023_98CF_440F_A884_2B8B9F18607B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDraw3DRectangle  
{
public:
	CDraw3DRectangle();
	virtual ~CDraw3DRectangle();

//Attributes
public:
	CRect			m_rcWnd;			// Output 영역
	CFont*		m_pFont;			// Font
	CString		m_strText;		// Text

	BOOL			m_bOuterLine;	// 외곽선 그림 유/무
	BOOL			m_bTransParent;// TransParent
	BOOL			m_b3D;			// 3D 표시
	BOOL			m_bGradient;	// 그라데이션
	BOOL			m_bMouseUp;		// Mouse Up시 Event
	BOOL			m_bFrame;		// Frame 표시
	
	short			m_nScroll;		// 글자 Scroll
	short			m_nAlign;		// 수평 정렬 상태
	short			m_nScrollX;		// Scroll시 X 좌표
	short			m_nScrollY;		// Scroll시 Y 좌표
	COLORREF		m_clrBack;		// 배경색
	COLORREF		m_clrText;		// 전경색


//Member Functions
public:
	void	DrawGradient(CDC *pDC, COLORREF clrStart);
	void  DrawTitleFrame(CDC* pDC);
	void	DrawOutput(CDC *pDC);
	void	DrawRectangle(CDC *pDC, CRect rc);

	void  SetFontStyle();
	void	SetScroll(short nScroll);	
	void	SetText(char *szText);

	char	*GetText() { return (LPSTR)(LPCTSTR)m_strText; }
	void	SetBackColor(COLORREF clrBack) { m_clrBack = clrBack; }
	void	SetFlatOutput(BOOL bFlatOutput) { m_b3D = bFlatOutput; }
	void	SetFrameType() { m_bFrame = TRUE; }
	void	SetGradation(BOOL bGradient = TRUE) { m_bGradient = bGradient; }
	void	SetOuterLine(BOOL bDraw = TRUE) { m_bOuterLine = bDraw; }
	void	SetTextColor(COLORREF clrText) { m_clrText = clrText; }
	void	SetTransParent(BOOL bTrans = TRUE) { m_bTransParent = bTrans; }

// Not Use
	void	SetAlign(short nType);

};

#endif // !defined(AFX_DRAW3DRECTANGLE_H__B4148023_98CF_440F_A884_2B8B9F18607B__INCLUDED_)
