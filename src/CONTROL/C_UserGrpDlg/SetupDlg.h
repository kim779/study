#pragma once
// SetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetupDlg dialog
#include "Controls.h"

class CSetupDlg : public CDialog
{
// Construction
public:
	CSetupDlg(CString sConfig, CWnd* pParent = NULL);   // standard constructor

	void	InitCtrl();
	BOOL	AddListHeader(CListCtrl& listCtrl, LPCTSTR strItem, int nItem,
				int nWidth = 50,
				int nSubItem = -1,
				int nMask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM,
				int nFmt = LVCFMT_LEFT);
	BOOL	AddListItem(CListCtrl& listCtrl, int nItem,int nSubItem, LPCTSTR strItem);
	int	GetSelectedGrpItem();
	void	AddGraph(int nItem);
	void	SelectGraph();
	int	GetGrpKind(CString sGrp);
	CString	GetGrpName(int kind);
	int	GetTickKind(CString sTick);
	CString	GetTickName(int kind);
	int	GetIndexValue(CString sIndex);
	int	GetIndexIndex(int kind);
	
	void	LoadGrpInfo();
	bool	SaveGrpInfo();

	CString	m_sConfig;
// Dialog Data
	//{{AFX_DATA(CSetupDlg)
	enum { IDD = IDD_SETUP };
	CButton	m_EnvDataItemRts;
	CButton	m_EnvTickVertical;
	CButton	m_EnvDataPreview;
	CButton	m_EnvDataReverse;
	CEdit	m_GrpOptRegion;
	CComboBox	m_EnvDataIndex;
	CEdit	m_EnvDataShow;
	CEdit	m_EnvDataReq;
	CButton	m_GrpOptNameR;
	CButton	m_GrpOptMinMax;
	CButton	m_EnvTickMinMax;
	CButton	m_EnvTickVDraw;
	CButton	m_EnvTickHDraw;
	CButton	m_EnvTickCtrl;
	CButton	m_BtnModify;
	CButton	m_EnvTickBottom;
	CButton	m_EnvTickRight;
	CButton	m_EnvTickLeft;
	CComboBox	m_GrpKind;
	CComboBox	m_GrpTick;
	CComboBox	m_GrpRegion;
	CListCtrl	m_GrpList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetupDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAdd();
	afx_msg void OnMod();
	afx_msg void OnDel();
	afx_msg void OnDblclkGrpList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAssign();
	afx_msg void OnDataItemrts();
	afx_msg void OnShowall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CColorCtrl	m_GrpColor;
	CKindCtrl	m_GrpStyle;
	CWidthCtrl	m_GrpWidth;

};
