#pragma once
// ListEdit.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CListEdit window
class CSListCtrl1;

class CListEdit : public CEdit
{
// Construction
public:
	CListEdit();
	CListEdit(CSListCtrl1* pList, int iItem, int iSubItem, CString sInitText);

// Attributes
public:
	CString m_szNumber;
	int		m_nMaxDigit{};
	int		m_nDigit{};
	double	m_RealVal{};

// Operations
public:
	void Clear();
	void SetNumText(CString str);
	inline int GetIntVal(){return (int)m_RealVal;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	void		SetListText();

private:
	CSListCtrl1* m_pListCtrl{};
	int			m_Item{};
	int			m_SubItem{};
	CString		m_InitText;
	BOOL		m_bEscape{};

};

