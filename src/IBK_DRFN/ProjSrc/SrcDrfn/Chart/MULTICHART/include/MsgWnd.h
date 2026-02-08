////////////////////////////////////////////////////////////////////////////////
// 01. 작  성  자      : {이준옥}
// 02. 작 성 일 자     : {1998.10.03 ~ }
// 03. 소       속     : {(주)두리정보통신}
////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_MSGWND_H__D9DD2682_EFC5_11D1_ADE0_00001C1C698B__INCLUDED_)
#define AFX_MSGWND_H__D9DD2682_EFC5_11D1_ADE0_00001C1C698B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MsgWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageWindow window

class CMessageWindow : public CWnd
{
// Construction
public:
	CMessageWindow();

// Attributes
public:
	CComboBox		m_combobox;
	CButton			m_button;
	CButton			m_button1;
	CButton			m_button2;
	CButton			m_button3;	// Back
	CButton			m_button4;	// ForWard

	CFont			m_Font;

	CDWordArray		m_ErrOppArray;

// Operations
public:
	BOOL	CreateMessageWnd(CWnd * pWnd);
	int		GetWndHeight();

	void	AddMessage(LPSTR lpstr, UINT errOpp=0);
	int		GetMessage(CString &Message);
	void	DelMessage();

	void	ResizeMessageWnd(UINT nType, int cx, int cy);
	void	MoveMessageWnd(CRect rect);

	// { Config 정보
	int		m_nMaxLine;
	void	SetMaxLine(int _nLine) { m_nMaxLine=_nLine; }
	void	CheckMaxLines();

	CString	m_szCfgFile;
	void	SetConfigFile(LPCSTR _szFile);
	// }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageWindow)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMessageWindow();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMessageWindow)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg void OnClickedButton(UINT nID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGWND_H__D9DD2682_EFC5_11D1_ADE0_00001C1C698B__INCLUDED_)
