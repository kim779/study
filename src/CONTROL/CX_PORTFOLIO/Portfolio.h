#if !defined(AFX_PORTFOLIO_H__FA831DA3_08FE_4BA8_B207_F27CDD88A718__INCLUDED_)
#define AFX_PORTFOLIO_H__FA831DA3_08FE_4BA8_B207_F27CDD88A718__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Portfolio.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPortfolio window

class CPortfolio : public CWnd
{
// Construction
public:
	CPortfolio(CWnd* parent = nullptr, struct _param* param = nullptr);
	virtual ~CPortfolio();

// Attributes
public:

protected:
	CWnd*	m_parent;
	CString	m_name;
	int	m_key;
	CString	m_user;

private:
	std::unique_ptr<char[]>	m_datb;
	int	m_datl;
	int	m_trkey;
	BOOL	m_done;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortfolio)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool	sendTR(CString trc, char* datb, int datl, int key);

	// Generated message map functions
protected:
	//{{AFX_MSG(CPortfolio)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg	long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CPortfolio)
	afx_msg void _SetUserName(LPCTSTR sUser);			// 관심그룹 정보를 가져올 User명 설정
	afx_msg BSTR _LoadGroupNames();					// Group명 가져오기
	afx_msg BSTR _LoadCodeList(LPCTSTR sGroup);			// 관심그룹의 종목코드 가져오기
	afx_msg long _GetGroupCnt();					// 관심그룹 개수 반환
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTFOLIO_H__FA831DA3_08FE_4BA8_B207_F27CDD88A718__INCLUDED_)
