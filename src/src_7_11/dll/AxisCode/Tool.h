// Draw.h: interface for the CTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAW_H__2741F618_6709_4254_8301_136C1F11C07A__INCLUDED_)
#define AFX_DRAW_H__2741F618_6709_4254_8301_136C1F11C07A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CTool
{
public:
	void DeleteDC();
	CTool();
	virtual ~CTool();

	void SetDC(CDC *pDC){ m_pDC = pDC; };
	CDC* GetDC(){ return m_pDC; }
	void FrameRect(CRect rc, COLORREF color, COLORREF frame = RGB(0,0,0), BOOL bline = FALSE);
	void FrameRect2(CRect rc, COLORREF color, COLORREF frame = RGB(0,0,0), COLORREF frame2 = RGB(0,0,0), BOOL bline = FALSE);
	void DrawCell(CRect rc, COLORREF lcolor = RGB(0,0,0));
	void WriteText(CString szText, CRect rc,COLORREF fcolor = RGB(0,0,0), UINT nFormat = DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	BOOL ScrollDC( int dx, int dy, LPCRECT lpRectScroll = NULL, LPCRECT lpRectClip = NULL, CRgn* pRgnUpdate = NULL, LPRECT lpRectUpdate = NULL);

protected:
	
	CDC		*m_pDC;
	CFont		m_Font;

};

#endif // !defined(AFX_DRAW_H__2741F618_6709_4254_8301_136C1F11C07A__INCLUDED_)
