#if !defined(AFX_DLGBLOCKHORIZONTALSCALE_H__5C4D92F5_6E3E_4141_9B5D_1D08C3431CC6__INCLUDED_)
#define AFX_DLGBLOCKHORIZONTALSCALE_H__5C4D92F5_6E3E_4141_9B5D_1D08C3431CC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgBlockHorizontalScale.h : header file
//
#include "DlgToolTip.h"
#include "NoSystemMenuCombo.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgBlockHorizontalScale dialog

class CChartLink;
class CDlgBlockHorizontalScale : public CDialog
{
// Construction
public:
	void SetAllItemInfoOfPacketPage(const CString &strAllItemInfoOfPacketPage);
	CDlgBlockHorizontalScale( CChartLink *pItem, HWND p_hOcxWnd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgBlockHorizontalScale)
	enum { IDD = IDD_DLGBLOCKSCALE };
	CNoSystemMenuCombo	m_ScaleDataComboBox;
	CNoSystemMenuCombo	m_DateCompartComboBox;
	CNoSystemMenuCombo	m_TypeTimeCompartComboBox;
	CNoSystemMenuCombo	m_TextCombo;
	int		m_nHorizonScalePos;
	CString	m_strDateCompartType;
	CString	m_StrTypeTimeCompartType;
	int		m_nStyleHorizonScale;
	CString	m_strScaleData;
	int		m_nScaleCompartType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgBlockHorizontalScale)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	
	// Generated message map functions
	//{{AFX_MSG(CDlgBlockHorizontalScale)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeComboHorizonsaclestyle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_strAllItemInfoOfPacketPage;

	void AddStringToScaleDataComboBox();
	void SetEnableWindowToTextCombo(const int& nIndex);
	CChartLink* m_pItem;
	//ÅøÆÁ
	CDlgToolTip		m_toolTip;
	void CreateToolTip();

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBLOCKHORIZONTALSCALE_H__5C4D92F5_6E3E_4141_9B5D_1D08C3431CC6__INCLUDED_)
