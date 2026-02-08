#pragma once
// ControlWnd.h : header file
//
#include "InfoPopup.h"
#include "jinfo.h"
#include "axwnd.h"

/////////////////////////////////////////////////////////////////////////////
// CSymbolWnd window

class CSymbolWnd : public CAxWnd
{
// Construction
public:
	CSymbolWnd(CWnd *pWnd);
	virtual ~CSymbolWnd();


// Attributes
public:
	bool		m_capture, m_bCheck, m_bMouseOver;
	std::unique_ptr<CInfoPopup>	m_pPopup;
	CString		m_sData, m_rtnStr;
	CString		m_sRate;
	BOOL		m_bTracking, m_bHover;
	
	COLORREF	m_boxColor, m_boxColorHover, m_bgColor, m_bgColorHover;

	CString		m_sCode;
	CString		m_sName;
	bool		m_ujongsise;
	bool		m_memoexist;
	CBitmap*	m_bitmap;
	CBitmap*	m_pBitmap;

	CString		m_szMarket;
	CString		m_szUpjong;
	CString		m_szDanger;

private:
	std::unique_ptr<CToolTipCtrl>	m_pTipCtrl;
	std::unique_ptr<Chjinfo>	m_hjinfo;
	std::unique_ptr<Cejinfo>	m_ejinfo;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolWnd)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void    loadGubn(struct _jHeader* pinfo);
	CString Variant(int comm, CString data);
	void	WriteAlert(CString str);
	void	WriteAlertx(struct _alertR* alertR);
	CString	Parser(CString &srcstr, CString substr);
	void	drawTriangle(CDC *pDC, CRect rc, COLORREF clr);
	void	drawIndicator1(CDC* dc, CRect rect, COLORREF clr);
	void	drawIndicator2(CDC* dc, CRect rect, COLORREF clr);
	void	drawBitmap(CDC *pDC, CRect rect, CBitmap *pBitmap);
//	void	SetParam(struct _param *pParam);
	LOGFONT	setFont(CString fName, int fsize, int style);
	COLORREF GetIndexColor(int index);
	bool	GetCheck();
	void	SetCheck(bool bCheck);
	BOOL	GetState();
	bool	checkMemo(CString sKey);
	void	loadBitmap(CString jgub, CString kind, CString jrat);
	void	Send(CString szCode);

	CPen*	getAxPen(COLORREF clr, int width, int style);
	CBrush* getAxBrush(COLORREF clr);
	CFont*  getAxFont(CString fName, int point, int style);
	CBitmap* getBitmap(CString path);
	void	Clear();


	void SetColor(COLORREF boxColor, COLORREF boxColorHover, COLORREF bgColor, COLORREF bgColorHover)
	{ m_boxColor = boxColor; m_boxColorHover= boxColorHover; m_bgColor = bgColor; m_bgColorHover = bgColorHover; }

private:
	void CreateTooltip(CString str);
	void SetDataTooltip();


	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
protected:
	//{{AFX_MSG(CSymbolWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};