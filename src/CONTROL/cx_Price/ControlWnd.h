#pragma once
// ControlWnd.h : header file
//

#include "PricePopup.h"

class CParam
{
public:
	int		key=0;			// screen key
	CString		name;			// control symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point=0;		// font point
	int		style=0;		// font style
	DWORD		tRGB=0;			// text color index
	DWORD		pRGB=0;			// paint color index
	CString		options;		// properties
};
/////////////////////////////////////////////////////////////////////////////
// CControlWnd window

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();
	virtual ~CControlWnd();

// Attributes
public:
	CWnd*		m_pParent;
	CParam		m_Param;
	CString		m_sRoot, m_sUserID;
	CString		m_rtnStr;
	bool		m_capture;
	BOOL		m_bHover, m_bTracking, m_bRsvdMode;
	HBITMAP		m_hBitmap, m_hBitmap_dn, m_hBitmap_hv;
	bool		m_bNoMaxMin;

	int		m_kind, m_type, m_nKind, m_nType;
	CString		m_sImagePath;
	CString		m_sText, m_sSymbol, m_sTxCode, m_sCode, m_sRTSCode;
	double		m_dCurr, m_dMax, m_dMin, m_dJgga;
	bool		m_bKospi, m_bLhGd, m_bCfo;

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	std::unique_ptr<CToolTipCtrl>	m_pToolTip;
	std::unique_ptr<CPricePopup>	m_pPopup;
	BOOL		m_bDragMode;
	CString		m_sTooltip;
	int		m_nPrice1, m_nPrice2;
	BOOL		m_bNewUnit;		// 2023 호가단위개선

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	void	SetParam(struct _param *pParam);
	COLORREF GetIndexColor(int index);
	BOOL	GetDragMode();

private:
	// 예약주문용을 위해 추가함 2009.08.24 by LKM
	// Local계산용 및 서버 조회용 -> 현재 서버에서 조회하도록 세팅됨.
	double	m_dshga, m_dhhga;

	BOOL	IsRsvdKind(int type);
	CFont*	getAxFont(CString fName, int point, int style);
	CBitmap* getBitmap(CString path);
	void	setKind(char cGubn, int& kind);
	CString	OptionParser(CString sOption, CString sKey);
	char	getCharKind();
	void	popPrice(CRect rc);
	void	SendTR(CString name, CString data, BYTE type, int key);
	void	SetButtonImages();
	CString Variant(int comm, CString data);
	void	WriteAlert(CString str);
	void	WriteAlertx(DWORD* ptr);
	CString	Parser(CString& srcstr, CString substr);
	CSize	BitmapSize(HBITMAP hBitmap);

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg	void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMessage1(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg BSTR GetJCode();
	afx_msg void SetJCode(LPCTSTR lpszNewValue);
	afx_msg short GetType();
	afx_msg void SetType(short nNewValue);
	afx_msg BOOL GetEnable();
	afx_msg void SetEnable(BOOL bNewValue);
	afx_msg BOOL GetVisible();
	afx_msg void SetVisible(BOOL bNewValue);
	afx_msg BSTR GetProperties();
	afx_msg void SetProperties(LPCTSTR sParam);
	afx_msg void SetValues(LPCTSTR str);
	afx_msg BOOL SendCode(LPCTSTR sCode);
	afx_msg void SetReserveMode(BOOL bReserveMode);
	afx_msg void SetDragMode(BOOL flag);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};