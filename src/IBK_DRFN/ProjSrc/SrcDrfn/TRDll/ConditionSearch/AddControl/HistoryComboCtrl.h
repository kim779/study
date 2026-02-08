#if !defined(AFX_HISTORYCOMBOCTRL_H__D5B48D19_0396_448C_B269_128B81F699DD__INCLUDED_)
#define AFX_HISTORYCOMBOCTRL_H__D5B48D19_0396_448C_B269_128B81F699DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistoryCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRetEdit window
class CRetEdit : public CEdit
{
	// Construction
public:
	CRetEdit();
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRetEdit)
public:
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CRetEdit();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CRetEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};
//
/////////////////////////////////////////////////////////////////////////////
// CHistoryComboCtrl window
class CHistoryComboCtrl : public CComboBox
{
// Construction
public:
	CHistoryComboCtrl();
	virtual ~CHistoryComboCtrl();

	BOOL Create(CWnd* pParentWnd, 
							UINT nID, 
							LPCTSTR strHistoryFilePath,
							LPCTSTR strTagName);

	void SetAddString(CString strText);
	CStringArray* GetComboStringArray();
	CString		 GetComboString();

private :
	CString				m_strFileRoot;
	CString				m_strTagName;
	CString				m_strDefList;
	CStringArray		m_arrDefList;

	CRetEdit	m_nREdit;
		
	void LoadHistoryFile();
	void WriteHistoryFile();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistoryComboCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:
	//{{AFX_MSG(CHistoryComboCtrl)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

///////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTORYCOMBOCTRL_H__D5B48D19_0396_448C_B269_128B81F699DD__INCLUDED_)
