#if !defined(AFX_RGBBUTTON_H__AE3FE2FC_7B21_438A_A74B_22A1AE65C79D__INCLUDED_)
#define AFX_RGBBUTTON_H__AE3FE2FC_7B21_438A_A74B_22A1AE65C79D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RGBButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRGBButton window
class CLeftBarDlg;
class CRGBButton : public CButton
{
// Construction
public:
	CRGBButton();

// Attributes
public:
	CToolTipCtrl	m_ToolTip;			// 툴팁
	BOOL			m_bTracking;		// Botton_RGB 영역 내 존재시 마우스 Mouse Position Tracking 시작 (Now Tracking - TRUE / End Tracking - FALSE)
	BOOL			m_bHover;			// Hovering 상태 (In - TRUE / Out - FALSE)
	
	COLORREF	m_clrBorder;
	COLORREF	m_clrBackGround;
	COLORREF	m_clrBackGround_Sel;
	COLORREF	m_clrText;

	BOOL		m_bHasBorder;
	BOOL		m_bIsRoundRect;

	CFont*		m_pFont;

	CString		m_szText;
	BOOL		m_bSideBarRunning;

// Operations
public:
	// 툴팁
	void InitToolTip();
	void SetTooltipText(int nId, BOOL bActivate);
	void SetTooltipText(LPCTSTR lpszText, BOOL bActivate);

	void DrawButtonRect(CDC* pDC, CRect rcClient, UINT nItemState);
	void DrawButtonText(CDC* pDC, CRect rcClient, UINT nItemState);

	void SetBorderColor(COLORREF clrBorder);
	void SetBackGroundColor(COLORREF clrBackGround, COLORREF clrBackGround_Sel);
	void SetTextColor(COLORREF clrText);

	void SetFont(CFont* pFont)	{ m_pFont = pFont; }
	void SetButtonText(LPCTSTR szText);

	// SideBar
	CLeftBarDlg* m_pSideBar;
	void SetSideBar(CLeftBarDlg* pSideBar);
	void StartSideBar();
	void EndSideBar();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRGBButton)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRGBButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRGBButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT	OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RGBBUTTON_H__AE3FE2FC_7B21_438A_A74B_22A1AE65C79D__INCLUDED_)

