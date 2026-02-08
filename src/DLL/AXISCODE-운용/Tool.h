// Draw.h: interface for the CTool class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

//#include "myfont.h"
typedef struct Data
{
	BOOL		flag{};
	BOOL		atm{};
	CString		code;
	CString		name;
}DATA;

class CTool
{
public:
	void DeleteDC();
	CTool();
	virtual ~CTool();

	void SetDC(CDC *pDC){ m_pDC = pDC; };
	CDC* GetDC(){ return m_pDC; }
	CFont* GetFont(){ return &m_Font; }
	void FrameRect(CRect rc, COLORREF color, COLORREF frame = RGB(0,0,0), BOOL bline = FALSE);
	void DrawCell(CRect rc, COLORREF lcolor = RGB(0,0,0));
	void WriteText(CString szText, CRect rc,COLORREF fcolor = RGB(0,0,0), UINT nFormat = DT_SINGLELINE|DT_VCENTER|DT_CENTER);
//	CMyFont* GetFont(){ return &m_Font; };
	BOOL ScrollDC( int dx, int dy, LPCRECT lpRectScroll = nullptr, LPCRECT lpRectClip = nullptr, CRgn* pRgnUpdate = nullptr, LPRECT lpRectUpdate = nullptr);

protected:
	
	CDC* m_pDC{};
	CFont		m_Font;
};


