#pragma once
// MapWnd.h : header file
//
#include <Afxtempl.h>
#include "data.h"
#include "../../h/axisfire.h"
#include "PopWnd.h"
#include "TransButton.h"

/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd();

// Attributes
public:
	CWnd		*m_pParent;

private:
	CString		m_strRoot;
	CRect		m_clientRt;
	CRect		m_rcUpRect, m_rcDnRect;
	
	CFont		*m_pFont;
	CFont		*m_pFontB;
	int			m_nFontSize{};
	//class CfxStatic	*m_pBorder;
	std::unique_ptr<CTransButton> m_pKospiBtn;
	std::unique_ptr<CTransButton> m_pKostarBtn;
	CCriticalSection	m_section;
	BOOL		m_bMouseClicked{};
	int		m_upperBound{}, m_lowerBound{};
	CUIntArray	m_listUpper, m_listLower;

	struct _outbound	m_data {};
	COLORREF	m_crBack{}, m_crBackIn{};
	CBrush		m_br;

	CMap<CString, LPCTSTR, CRect, CRect>	m_mapItem;	// code, rect

	std::unique_ptr<CPopWnd> m_pPopwnd;
	std::unique_ptr<CBitmap> m_pBitmap;
	CString		m_time;
	CString		m_mouseDat;
	CString		m_strPal;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	//}}AFX_VIRTUAL

// Implementation
public: 
	virtual ~CMapWnd();

private:
	void DrawBitmap(CDC *pDC, CRect rc, CString strImage);
	void DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);
	BOOL m_bDraw;
	
	CFont*	RequestFont(CString strName, int iPoint, BOOL bItalic = FALSE, int iBold = 0);
	
	CString	Variant(int comm, CString data);
	COLORREF GetIndexColor(int index);
	CBitmap* GetAxBitmap(CString path);
	void	FrameSRect(CDC *pDC, CRect rt, COLORREF solid, COLORREF frame);
	void	drawBitmap();
	void	Draw(CDC *pDC);
	void	SendTR(CString name, CString data);
	LOGFONT setFont(int size, bool bold);
	CPen*	GetPen(COLORREF color);
	void	SetResize(int nFontSize);
	void	makeMapItem();
	CString	GetPalette();

	// Generated message map functions
protected:
	afx_msg void OnKospiClick();
	afx_msg void OnKostarClick();
	//{{AFX_MSG(CMapWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

