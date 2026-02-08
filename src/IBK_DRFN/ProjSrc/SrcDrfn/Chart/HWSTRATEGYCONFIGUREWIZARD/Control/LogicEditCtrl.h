#if !defined(AFX_LOGICEDITCTRL_H__54EBEF41_3EDB_4B29_AADA_56E75EAC2FEB__INCLUDED_)
#define AFX_LOGICEDITCTRL_H__54EBEF41_3EDB_4B29_AADA_56E75EAC2FEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogicEditCtrl.h : header file
//
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CLogicEditCtrl window


class CElementInLogic
{
public:
	CElementInLogic() 
	{
		Init();
	}
	void Init()
	{
		m_nType = 0;
		m_bTRUE = TRUE;
		m_nStartChar = 0;
		m_nEndChar = 0;
		m_strValue = "";
	}
	int m_nType;
	CString m_strValue;
	BOOL	m_bTRUE;
	long m_nStartChar;
	long m_nEndChar;
};

class CLogicEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CLogicEditCtrl();
	void AddElement(char chElement);
	void SwapElement(char chElement1, char chElement2);
	void DeleteElement(char chElement);
	void ChangeElement(BOOL bTRUE);
	void ChangeOperation(LPCTSTR lpOperation);
	void Parenthesize();
	void Unparenthesize();
	void DeleteAllElement();
	CString GetLogic();
	void SetLogic(LPCTSTR lpLogic);

	
// Attributes
public:
private:
	void UpdateLogic();
	CArray <CElementInLogic,CElementInLogic&> m_arrayElement;
	CFont     m_fontDefault;
	char	  m_chLastElement;
	int		 m_nBeginSelected;
	int		 m_nEndSelected;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogicEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLogicEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLogicEditCtrl)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGICEDITCTRL_H__54EBEF41_3EDB_4B29_AADA_56E75EAC2FEB__INCLUDED_)
