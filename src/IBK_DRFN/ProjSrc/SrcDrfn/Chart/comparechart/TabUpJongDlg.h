#if !defined(AFX_TABUPJONGDLG_H__59B7FDA2_F96E_4C65_9303_0771F8CE6D24__INCLUDED_)
#define AFX_TABUPJONGDLG_H__59B7FDA2_F96E_4C65_9303_0771F8CE6D24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabUpJongDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabUpJongDlg dialog
#include "TabDlg.h"
#include "./control/VirtualListCtrl.h"
#include "../../inc/IMasterDataMng.h"
#include "../chart_common/ILButton.h"
//#include "../../COMMON_BASE/baseInfo/BaseInfo.h" //2005.06.15 add by sy, nam
//#include "../../../DrMLib/DrMiddleLib.h"//05.09.27

class CStdDialog;
class CTabUpJongDlg : public CTabDlg
{
// Construction
public:
	CTabUpJongDlg(CWnd* pParent = NULL);   // standard constructor
	enum {UPALL, UPKOSPI, UPKOSDAQ};	
	int m_nOption;
	//------------------------------------------------------------------------
	//Author	:	Sang Yun Nam
	//Date		:	2005.06.15
	//Comment	:
	//------------------------------------------------------------------------
	//CDrMLib_CBaseInfo	m_BaseInfo;
	CString		m_strCode;
	CString		m_strCodeFromHanhwa;
	void		InitBaseInfo(int nType); //CodeInput
	void		SetKeyCodeChange(WPARAM wParam, LPARAM lParam);	

	
	IMasterDataManagerLast* m_pDataManager;
	//------------------------------------------------------------------------
// Dialog Data
	//{{AFX_DATA(CTabUpJongDlg)
	enum { IDD = IDD_TAB_UPJONG };
	CVirtualListCtrl	m_listUpJongCode;
	CILButton	m_BtnKospi;
	CILButton	m_BtnK200;
	CILButton	m_BtnKosdaq;
//	CILButton	m_BtnKonex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabUpJongDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void fnRemovelistStockCode();

	// Generated message map functions
	//{{AFX_MSG(CTabUpJongDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChkKospi();
	afx_msg void OnChkKosdaq();
//	afx_msg void OnChkKonex();
	afx_msg void OnChkK200();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABUPJONGDLG_H__59B7FDA2_F96E_4C65_9303_0771F8CE6D24__INCLUDED_)
