#if !defined(AFX_TABSUBINDICATORSETUPCTRL_H__5C774AEC_1751_425E_9BA7_764CB6B3E074__INCLUDED_)
#define AFX_TABSUBINDICATORSETUPCTRL_H__5C774AEC_1751_425E_9BA7_764CB6B3E074__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabSubIndicatorSetUpCtrl.h : header file
//

enum LOAD_TYPE{ DLG_ENV_SETUP_ALL, GRAPH_SETTING };

/////////////////////////////////////////////////////////////////////////////
// CTabSubIndicatorSetUpCtrl window

class CTabParentDlg
{
public:
	virtual	void ShowChildDialog( BOOL bShow) = 0;					// ChildDlg 보여주기 유무.
};

class CTabSubIndicatorSetUpCtrl : public CTabCtrl
{
// Construction
public:
	CTabSubIndicatorSetUpCtrl();

// Attributes
public:
	int m_nTotalTabPages;		// 전체 TabPage 개수 
	int m_nCurTabPage;			// 현재 TabPage

private:
	CDialog *m_aTabPages[2];

	CString m_strIndicatorName;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabSubIndicatorSetUpCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTabSubIndicatorSetUpCtrl();

	CTabParentDlg *m_pParentDlg;		// 지표설정 Dlg

	void Init( IChartOCX *p_pIChartOCX, CTabParentDlg *pParentDlg);
	
	CWnd *GetSubTab( int nSubTabIdx );

	void ReadIndicatorExpFile( void );


	CString GetSubGraphName( void );
	void SetSubGraphName( CString strIndicatorName );


private:
	void CreateTabPage( IChartOCX *p_pIChartOCX);
	void SetTabPagePosition( void );

	BOOL ChangeTabPage( void );


	// Generated message map functions
protected:
	//{{AFX_MSG(CTabSubIndicatorSetUpCtrl)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABSUBINDICATORSETUPCTRL_H__5C774AEC_1751_425E_9BA7_764CB6B3E074__INCLUDED_)
