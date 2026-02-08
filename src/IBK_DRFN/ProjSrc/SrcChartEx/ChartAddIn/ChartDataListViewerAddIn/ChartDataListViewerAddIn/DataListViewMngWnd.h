#if !defined(AFX_DATALISTVIEWMNGWND_H__65D07B6B_13E3_4F5B_9CC2_991FEE8B6E11__INCLUDED_)
#define AFX_DATALISTVIEWMNGWND_H__65D07B6B_13E3_4F5B_9CC2_991FEE8B6E11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataListViewMngWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataListViewMngWnd window


class CDlgDataListViewer;
class CChartDataListViewerAddInImp;


class CDataListViewMngWnd : public CWnd
{
// Construction
public:
	CDataListViewMngWnd();

// Attributes
public:

	BOOL	m_bIsUpdateReady;

	CString	m_strClassName;

	CDlgDataListViewer *	m_pDataListViewer;

	CChartDataListViewerAddInImp *	m_pAddInImp;

// Operations
public:

	
	BOOL IsUpdateReady();

	void SetAddInImp(CChartDataListViewerAddInImp *	pAddInImp);

	CChartDataListViewerAddInImp *	GetAddInImp();


public:

	void Update();

	void RealUpdate();

	void CreateDataListView();	

	void DestroyDataListViewer();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataListViewMngWnd)
	public:
	virtual BOOL Create(CWnd* pParentWnd, UINT nID);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDataListViewMngWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDataListViewMngWnd)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATALISTVIEWMNGWND_H__65D07B6B_13E3_4F5B_9CC2_991FEE8B6E11__INCLUDED_)
