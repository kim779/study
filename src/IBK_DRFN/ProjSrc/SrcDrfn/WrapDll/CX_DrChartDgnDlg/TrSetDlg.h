#if !defined(AFX_TRSETDLG_H__C4DC0915_5EAC_43CF_ACB8_BAD13138821E__INCLUDED_)
#define AFX_TRSETDLG_H__C4DC0915_5EAC_43CF_ACB8_BAD13138821E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrSetDlg.h : header file
//

#include "AddQueryDlg.h"

#include "../../DrCommon/DataClassLib/TrQuery.h"

#include "common/QueryGrid.h"
#include "common/MenuXP.h"

/////////////////////////////////////////////////////////////////////////////
// CTrSetDlg dialog

class CTrSetDlg : public CDialog
{
// Construction
public:
	CTrSetDlg(CWnd* pParent);   // standard constructor
	virtual	~CTrSetDlg();

	void		CopyArrQry(CArrTrQuery* pArr);
	int			GetQueryAt(CString strName);

	void		GetTRQry(CArrTrQuery* pArr);
	void		SetTRQry(CArrTrQuery* pArr);

public:
	CString			m_strFile;
	BOOL			m_bDirectSend;

	CQueryGrid		m_GridIn, m_GridOut, *m_pSelGrid;
	CMenuXP*		m_pSubMenu;
	CMenuXP*		m_pSubMenu2;

	CArrTrQuery		m_arrQuery;

	int				m_nSelIndex;
	CTrQuery		m_selQry;
	CTrPack			*m_pSelTrPack;

	void		EnableControl();

	void		OnSubMenu(UINT nID);
	void		OnNodeAdd(int nType);
	void		OnItemAdd(int nType);
	void		OnMenuDelete();

	void		QueryDataToGrid(CTrPack* pTrPack, CQueryGrid* pGrid);
	BOOL		GridToQueryData(CTrPack* pTrPack, CQueryGrid* pGrid);
	int			MakeQueryData(CTrBlock *pPBlock, CSuperGridCtrl::CTreeItem *pPItem);
	void		LoadQueryData(CTrBlock *pPBlock, CSuperGridCtrl::CTreeItem *pPItem);

public:
	void		ReSizeCtrl();
	void		ResetQueryData();

	void		SaveCurrentQry();

public:
// Dialog Data
	//{{AFX_DATA(CTrSetDlg)
	enum { IDD = IDD_TRSETDLG };
	CEdit	m_editRealSymbol;
	CButton	m_grpTRQuery;
	CListCtrl	m_listQuery;
	CButton	m_chkDirectQry;
	CEdit	m_editTRCode;
	BOOL	m_bDirectQry;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrSetDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrSetDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCheckDirectquery();
	afx_msg void OnBtnAddtrquery();
	afx_msg void OnBtnDeltrquery();
	afx_msg void OnBtnRename();
	afx_msg void OnItemchangedListTrquery(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRSETDLG_H__C4DC0915_5EAC_43CF_ACB8_BAD13138821E__INCLUDED_)
