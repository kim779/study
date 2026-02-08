#if !defined(AFX_SUBFILTERDLG_H__465B34B5_8C32_4545_A459_5D404308C7DE__INCLUDED_)
#define AFX_SUBFILTERDLG_H__465B34B5_8C32_4545_A459_5D404308C7DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FilterDlg.h : header file
//

#include "resource.h"
#include "./AddControl/SetGrid.h"

#include "../DrCommon/Dr_Control/DrBtnCtrl.H"
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
/////////////////////////////////////////////////////////////////////////////
// CSubFilterDlg dialog

class CSubFilterDlg : public CDialog
{
	// Construction
public:
	CFont m_fontDefault;
	CBrush  m_brushStatic;
	
	CSubFilterDlg(CWnd* pParent, CString strType, 
		CList <SubFilterInfo, SubFilterInfo&>	*plistSubFilterInfo, CWnd* pMainWnd);   // standard constructor
	
	CWnd* m_pwndParent;
	CString m_strType;
	CList <SubFilterInfo, SubFilterInfo&>	*m_plistSubFilterInfo;
	
	BOOL Create(CWnd* pParentWnd, UINT nID);

	CWnd*		m_pMainWnd;
public:
	CButton		m_btnFilterGroup;
	CSetGrid	m_gridSet;
	
	CComboBox	  m_comboTime;
	CStatic		  m_ctrlTime;
	CEdit		  m_editVal;
	CStatic		  m_ctrlVal;
	
	CfxImgButton	m_btnModify;
	CfxImgButton	m_btnDelete;
	CfxImgButton	m_btnOk;
	CfxImgButton	m_btnCancel;
	
	CStatic		m_ctrlInfo;
	
	
public:
	void OnSize_Set(CRect reClient);
	void SetInit();
	
	void SetShow(CString strType, CList <SubFilterInfo, SubFilterInfo&>	*plistSubFilterInfo);
	
	// Dialog Data
	//{{AFX_DATA(CSubFilterDlg)
	enum { IDD = IDD_DLG_SUBFILTER };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubFilterDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CSubFilterDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	
	afx_msg void OnBtnModify();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnOk();
	afx_msg void OnBtnCancel();
	
	//afx_msg void OnNotify_Grid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotify_Grid(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUBFILTERDLG_H__465B34B5_8C32_4545_A459_5D404308C7DE__INCLUDED_)
