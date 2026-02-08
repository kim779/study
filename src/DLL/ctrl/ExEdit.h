#if !defined(AFX_EXEDIT_H__9CA36A5D_484B_11D5_941A_00001CD7EE63__INCLUDED_)
#define AFX_EXEDIT_H__9CA36A5D_484B_11D5_941A_00001CD7EE63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

class AFX_EXT_CLASS CExEdit : public CEdit
{
public:
	CExEdit();
	virtual ~CExEdit();
	//{{AFX_VIRTUAL(CExEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg LRESULT OnEditError(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CExEdit(const CExEdit& d);
	CExEdit operator=(const CExEdit& d);
	static	CExEdit* CreateInPlace(CWnd* pWnd, CRect rect, 
		int row, int col, int limit, long style = WS_VISIBLE|WS_CHILD|ES_RIGHT|WS_BORDER);

	void	SetPlace(int row, int col);
	void	GetPlace(int& row, int& col);
	void	SetNumber(bool isNum = true);
	void	SetPoint(bool bCheck, bool isPoint = true, bool bZero = false);
	void	SetPointEx(bool bCheck, bool isPoint = true, bool isThous = true);
	
	void	MoveControl(CRect& rect);
	void	SetFixText(CString fixS);
	void	SetText(CString str);
	CString	GetText();

	// ½Ã¼¼Æ÷Âø
	void	SetTextColor(COLORREF rgbText = CLR_DEFAULT);
	void	SetBkColor(COLORREF rgbBk = CLR_DEFAULT);
	void	SetUpdn(bool isUpdn = true);
	void	SetDiff(bool isDiff = true);

protected:
	bool	checkPoint(CString& tmpS);

private:
	bool	m_bZero;
	bool	m_isNum;
	bool	m_isThous;
	bool	m_isPoint;
	bool	m_isUpdn;
	bool	m_isDiff;
	bool	m_bNot;
	bool	m_bHipon;
	int	m_row;
	int	m_col;
	CString	m_fix;
	CString	m_sText;
	bool	m_bCheck;
	// ½Ã¼¼Æ÷Âø
	COLORREF	m_rgbText;
	COLORREF	m_rgbBk;
	bool		m_bColor;
	CBrush		m_brush;
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_EXEDIT_H__9CA36A5D_484B_11D5_941A_00001CD7EE63__INCLUDED_)
