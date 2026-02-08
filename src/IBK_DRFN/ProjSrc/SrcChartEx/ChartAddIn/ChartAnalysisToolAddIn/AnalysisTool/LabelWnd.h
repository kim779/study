#if !defined(AFX_LABELWND_H__C9556CE4_0A07_4D4A_8F18_9ED345194DD3__INCLUDED_)
#define AFX_LABELWND_H__C9556CE4_0A07_4D4A_8F18_9ED345194DD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LabelWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLabelWnd window

class CLabelWnd
{
// Construction
public:
	CLabelWnd();
	virtual ~CLabelWnd();

	enum { LEFT, RIGHT, TOP,BOTTOM };	

// Operations
public:
// Implementation
public:
	void RestoreRect(CRect rect, CDC* pDC, CDC *pBackDC);
	void ShowWnd(BOOL bShow, CDC* pDC, CDC* pBackDC);	
	BOOL CreateLabel(int type, COLORREF colText, COLORREF colBk);
	void SetColor(COLORREF colText, COLORREF colBackground);
	void SetType(int type);
	void MoveLabelWindow(int x, int y, int width = 0, int height = 0);		
	void SetLabel(TCHAR *pChLabel, UINT nFormat, CDC* pDC,BOOL bVerticalLabel = FALSE);
	void SetLabelFont(CFont* pFont);
	void SetLabelRect(CRect rect)
	{	m_rect.CopyRect(rect);					}
	void SetVertCharWidth(int nVerCharWidth)
	{	 m_nVertCharWidth = nVerCharWidth;		}
	void SetVertTextSize(int nVertTextSize)
	{	 m_nVertTextSize = nVertTextSize;		}
// Attributes	
private:
	BOOL m_bVeticalLabel;
	CSize m_sizeLabel;
	int	m_nVertTextSize;
	int m_nVertCharWidth;
	CFont font;
	CString m_strLabel;
	int m_type;
	int m_nFormat;
	int m_height;
	int m_width;
	COLORREF m_colText;
	COLORREF m_colBackground;

	CRect m_rect;
	CRect m_OldRect;

	// Generated message map functions
protected:

	void Paint(CDC* pDC, CDC* pBackDC);


};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LABELWND_H__C9556CE4_0A07_4D4A_8F18_9ED345194DD3__INCLUDED_)
