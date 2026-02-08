#if !defined(AFX_ECONCLUSION_H__C111BCCD_7AFF_4E92_8661_2A1EB7971CA4__INCLUDED_)
#define AFX_ECONCLUSION_H__C111BCCD_7AFF_4E92_8661_2A1EB7971CA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EConclusion.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEConclusion dialog
#include "useMaindefine.h"
class CEConclusion : public CDialog
{
// Construction
public:
	CEConclusion(CWnd* pParent = NULL);   // standard constructor
	~CEConclusion();

protected:
	int	m_width{};
	int	m_height{};

	CRect	m_baseRc;

#ifdef DF_USE_CPLUS17
	std::unique_ptr<class CNTable> m_table;
#else
	class CNTable*	m_table;
#endif

// Dialog Data
	//{{AFX_DATA(CEConclusion)
	enum { IDD = IDD_CONCLUSION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEConclusion)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void	Init();
	void	ChangePalette();
	void	ChangeData();
	void	ShowSlide(CRect);
	void	HideSlide();
	void	SetData(CString dat, bool bFail = false);
	void	ChangeTitle(int index = -1, CString title =_T(""));
	void	ChangeTitleK(int kind);
	void	StopSlide();	
protected:
	void	Slide();
	// Generated message map functions
	//{{AFX_MSG(CEConclusion)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ECONCLUSION_H__C111BCCD_7AFF_4E92_8661_2A1EB7971CA4__INCLUDED_)
