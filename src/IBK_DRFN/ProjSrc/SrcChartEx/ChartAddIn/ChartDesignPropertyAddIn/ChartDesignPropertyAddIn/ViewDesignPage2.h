#if !defined(AFX_VIEWDESIGNPAGE2_H__BBFDAC84_0A97_47C8_B996_7C8197ADA539__INCLUDED_)
#define AFX_VIEWDESIGNPAGE2_H__BBFDAC84_0A97_47C8_B996_7C8197ADA539__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDesignPage2.h : header file
//

#include "resource.h"
#include "DlgToolTip.h"
#include "NoSystemMenuEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CViewDesignPage2 dialog

interface IPropertyMap;
class CViewDesignPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CViewDesignPage2)

// Construction
public:
	CViewDesignPage2();
	~CViewDesignPage2();

	//Set Received Value From ocx
	void SetProperty( IPropertyMap *p_pIPropertyMap);

// Dialog Data
	//{{AFX_DATA(CViewDesignPage2)
	enum { IDD = IDD_PROPPAGE_VIEWDESIGN2 };
	CButton	m_ChartRButtonMenu_Bt;
	CNoSystemMenuEdit	m_UserAcceptBlock_Edit;
	CNoSystemMenuEdit	m_Indicatorsave_Edit;
	CNoSystemMenuEdit	m_EnvironMent_Edit;
	CString	m_strUserEnvironmentSaveFileName;
	CString	m_strInsertedIndicatorSaveFileName;
	int		m_nUserInsertedBlockCount;
	int		m_nRButtonMenuType;
	BOOL	m_bHide;
	BOOL	m_bInputRequire;
	BOOL	m_bChartRButtonMenu;
	BOOL	m_bGraphDragandDrop;
	BOOL	m_bUseShortKey;
	BOOL	m_bShowWarningMessageBox;
	BOOL	m_bControlBlockSize;
	BOOL	m_bBlockMoveDelete;
	BOOL	m_bSelectTitle;
	BOOL	m_bSelectGraph;
	BOOL	m_bUseScaleRButtonMenu;
	int		m_nGraphRButtonType;
	BOOL	m_bIsEnvironmentFile;
	BOOL	m_bIsIndicondFile;
	BOOL	m_bUseSkin;
	CString	m_strSaveMapName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CViewDesignPage2)
	public:
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CViewDesignPage2)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeUserinsertedblockcount();
	afx_msg void OnChartrbuttonmenu();
	afx_msg void OnIndicondcheck();
	afx_msg void OnEnvironmentcheck();
	afx_msg void OnUseSkin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString MakeFileName(CString& strFileName);
	//Create tooltip object and Add text to StringArray 
	void CreateToolTip();

	void SetProperty_FileName(const CString& strItemValue, CString& strItem, BOOL& bCheckItem);
	void EnableWindow(const CString& strItem, CEdit& editItem);
	void EnableWindow(const int nID, const bool bEnable);
	void EnableWindow_SaveOption(const bool bEnable);

private:
	const CString FILETYPE;
	//tooltip
	CDlgToolTip		m_toolTip;

// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
protected:
	IPropertyMap *	m_pIPropertyMap;

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
public:
	void	SetOcxHwnd( HWND p_hOcxWnd);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDESIGNPAGE2_H__BBFDAC84_0A97_47C8_B996_7C8197ADA539__INCLUDED_)
