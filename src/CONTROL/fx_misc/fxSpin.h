#pragma once


class CSpinEdit;
class CSpinCtrl;

class AFX_EXT_CLASS CfxSpin : public CStatic
{
public:
	CfxSpin(CFont* pFont = nullptr);
	virtual ~CfxSpin();

	//{{AFX_VIRTUAL(CfxSpin)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CfxSpin)
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnEditChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSpinChange(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

public:
	BOOL	Create(DWORD attr, DWORD spType, const RECT& rect, CWnd* pParentWnd, UINT nID);
	void	Init(DWORD attr, DWORD spType, CWnd* pParentWnd);
	void	SetAttribute(DWORD attr);
	void	SetSpinType(DWORD remove, DWORD add);
	void	SetColors(COLORREF crBack, COLORREF crFont, bool redraw = true);

	void	SetText(CString text);	
	CString	GetText();
	void	SetRangeEx(double lower, double upper);
	void	GetRangeEx(double &lower, double &upper);
	void	SetDelta(double delta, bool redraw = true);
	double	GetDelta();
	void	SetPos(double nPos);
	double	GetPos();
	void	SetDecimalPlaces(int digit = 0, bool redraw = true);
	void	SetDirection(BOOL nFlag = TRUE);
	void	SetLimitText(int len);
private:
	void	createControls();

public:
	std::unique_ptr<CSpinEdit> m_pEdit;
private:
	CWnd*	m_parent{};
	DWORD	m_spType{};
	DWORD	m_attr{};

	CFont*	   m_pFont{};
	std::unique_ptr<CSpinCtrl> m_pSpin;
};


class CSpinEdit : public CEdit
{
public:
	CSpinEdit(CWnd* pWnd, DWORD attr);
	virtual ~CSpinEdit();
	//{{AFX_VIRTUAL(CSpinEdit)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSpinEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnChange();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void	SetUseColor(bool bColor);
	void	SetColors(COLORREF clrBACK, COLORREF clrFORE, bool redraw = true);
	void	SetRange(double lower, double upper);
	void	SetText(CString text, bool redraw = true);
	CString	GetText();
	void	SetAtrribute(DWORD attr, bool redraw = true);
	void	SetDecimalPlaces(int digit);

private:
	void	formatComma(CString& text);
	void	commaText();
private:
	CWnd*		m_pWnd{};
	DWORD		m_attr{};
	COLORREF	m_clrFORE{};
	COLORREF	m_clrBACK{};
	CBrush		m_brBACK{};
	bool		m_bColor{};
	double		m_upper{};
	double		m_lower{};
	int		m_digit{};
	bool		m_bFocus{};
};

class CSpinCtrl : public CSpinButtonCtrl
{
public:
	CSpinCtrl(CWnd* pWnd, DWORD spType, DWORD attr);
	virtual ~CSpinCtrl();
	//{{AFX_VIRTUAL(CSpinCtrl)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSpinCtrl)
	afx_msg void OnDeltapos(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	SetPos(double nPos);
	double	GetPos();
	void	SetSpinType(DWORD spType, bool redraw = true);
	void	SetAttribute(DWORD attr, bool redraw = true);
	void	SetDirection(int direction);
	void	SetDecimalPlaces(int digit);
	double	GetDelta();
	void	SetDelta(double delta);
	
private:
	void	formatComma(CString& text);
	void	checkDelta(BOOL nFlag);
private:
	CWnd	*m_pWnd{};
	DWORD	m_spType{};
	DWORD	m_attr{};

	int	m_direction{};
	int	m_digit{};
	double	m_delta{};
};

