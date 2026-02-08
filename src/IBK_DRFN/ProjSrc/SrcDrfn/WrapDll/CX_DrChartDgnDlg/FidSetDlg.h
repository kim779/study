#if !defined(AFX_FIDSETDLG_H__64B1CDDD_3774_4C25_9322_A4C848EEE56E__INCLUDED_)
#define AFX_FIDSETDLG_H__64B1CDDD_3774_4C25_9322_A4C848EEE56E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FidSetDlg.h : header file
//


#include "AddQueryDlg.h"

#include "../../DrCommon/DataClassLib/FidQuery.h"

#include "common/FidQueryGrid.h"
#include "common/MenuXP.h"


/////////////////////////////////////////////////////////////////////////////
// CFidSetDlg dialog

class CFidSetDlg : public CDialog
{
// Construction
public:
	CFidSetDlg(CWnd *pParent);
	virtual	~CFidSetDlg();

	void		CopyArrQry(CArrFidQuery* pArr);
	int			GetQueryAt(CString strName);

	void		SetFIDQry(CArrFidQuery* pArr);
	void		GetFIDQry(CArrFidQuery* pArr);

public:
	CString			m_strFile;
	BOOL			m_bDirectSend;
	BOOL			m_bUseEstEge;		// 운영자를 위한...
	CFidQuery		m_outQuery;

	CFidQueryGrid	m_GridIn, m_GridOut, *m_pSelGrid;
	CMenuXP*		m_pSubMenu;
	CMenuXP*		m_pSubMenu2;

//	CFidPack		m_query;
	CArrFidQuery	m_arrQuery;

	int				m_nSelIndex;
	CFidQuery		m_selQry;
	CFidPack		*m_pSelFidPack;

	void		EnableControl();

	void		OnSubMenu(UINT nID);
	void		OnNodeAdd(int nType);
	void		OnItemAdd(int nType);
	void		OnMenuDelete();

	void		QueryDataToGrid(CFidPack* pFidPack, CFidQueryGrid* pGrid);
	BOOL		GridToQueryData(CFidPack* pFidPack, CFidQueryGrid* pGrid);
	int			MakeQueryData(CFidBlock *pPBlock, CSuperGridCtrl::CTreeItem *pPItem);
	void		LoadQueryData(CFidBlock *pPBlock, CSuperGridCtrl::CTreeItem *pPItem);

public:
	void		ReSizeCtrl();
	void		ResetQueryData();

	void		SaveCurrentQry();

// Dialog Data
	//{{AFX_DATA(CFidSetDlg)
	enum { IDD = IDD_FIDSETDLG };
	CEdit	m_editRealSymbol;
	CButton	m_btnFidOpen;
	CButton	m_btnFidSaveAs;
	CButton	m_grpFIDQuery;
	CListCtrl	m_listQuery;
	CEdit	m_editFIDTrCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFidSetDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFidSetDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnAddfidquery();
	afx_msg void OnBtnDelfidquery();
	afx_msg void OnBtnRename();
	afx_msg void OnItemchangedListFidquery(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnFidsaveas();
	afx_msg void OnBtnFidopen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIDSETDLG_H__64B1CDDD_3774_4C25_9322_A4C848EEE56E__INCLUDED_)
