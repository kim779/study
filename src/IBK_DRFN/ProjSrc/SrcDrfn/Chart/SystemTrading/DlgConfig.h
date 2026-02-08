#if !defined(AFX_DLGCONFIG_H__908E2923_5207_4B69_9F96_347E1090B57B__INCLUDED_)
#define AFX_DLGCONFIG_H__908E2923_5207_4B69_9F96_347E1090B57B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgConfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx class

class CTabCtrlEx : public CXTTabCtrl
{
	DECLARE_DYNAMIC(CTabCtrlEx)

// Construction / destruction
public:

	// Constructs a CTabCtrlEx object.
	CTabCtrlEx();

	// Destroys a CTabCtrlEx object, handles cleanup and de-allocation.
	virtual ~CTabCtrlEx();

// Member variables
protected:

// Member functions
public:

	//{{AFX_VIRTUAL(CTabCtrlEx)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTabCtrlEx)
	afx_msg void OnCloseTab();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDlgConfig dialog

class CDlgEditorConfig;
class CDlgConfig : public CDialog
{
// Construction
public:
	CDlgConfig(UINT nScriptType, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgConfig)
	enum { IDD = IDD_DLG_CONFIG };
	CTabCtrlEx	m_tabCtrl;
	//}}AFX_DATA

	CImageList		m_imageList;
	UINT			m_nScriptType;

	CDlgEditorConfig*	m_pDlgTradeStation;
	CDlgEditorConfig*	m_pDlgYesTrader;

	CFont*			m_pFont;
	
	void	ForceRefreshTabCtrl();
	void	CreateDlgEditor();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgConfig)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONFIG_H__908E2923_5207_4B69_9F96_347E1090B57B__INCLUDED_)
