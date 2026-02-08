#if !defined(AFX_COLORSETDLG_H__B3B9380F_843F_4FFE_8E10_3BF5BBB6210E__INCLUDED_)
#define AFX_COLORSETDLG_H__B3B9380F_843F_4FFE_8E10_3BF5BBB6210E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorSetDlg.h : header file
//
#include "colourpicker.h"
#include "DlgToolTip.h"
/////////////////////////////////////////////////////////////////////////////
// CColorSetDlg dialog

interface IPropertyMap;
class CColorSetDlg : public CDialog
{
// Construction
public:
	//Create tooltip object and Add text to StringArray 
	void CreateToolTip();
	//Set Received Value From ocx
	void SetProperty( IPropertyMap *p_pIPropertyMap);
	CColorSetDlg( HWND p_hOcxWnd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CColorSetDlg)
	enum { IDD = IDD_COLORSET };
	CColourPicker	m_Lattice;
	CColourPicker	m_NumericalInquiryColor;
	CColourPicker	m_clrVerticalScaleLine;
	CColourPicker	m_TwoLine;
	CColourPicker	m_ThreeLine;
	CColourPicker	m_SixLIne;
	CColourPicker	m_ScaleTextColor;
	CColourPicker	m_OneLine;
	CColourPicker	m_GridColor;
	CColourPicker	m_FourLine;
	CColourPicker	m_FiveLine;
	CColourPicker	m_BlockColor;
	CColourPicker	m_BlockBorderColor;
	CColourPicker	m_AnalyzeToolColor;
	CColourPicker	m_BackBorderColor;
	CColourPicker	m_BackGroundColor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorSetDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CColorSetDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//this function call when push this Dlg's OK to get SettingColor.
	void GetStringColorOfButton();
	//init tooltip's String.
	void StringArrayAdd( UINT ControlID, UINT uMsgID);
	//init Color of Button.
	void InitColortoButtonbyPicker();
	//ÅøÆÁ
	CDlgToolTip	m_toolTip;

	CString m_strBackGroundColor;
	CString	m_strScaleTextColor;
	CString	m_strOneLine;
	CString	m_strTwoLine;
	CString	m_strThreeLine;
	CString	m_strFourLine;
	CString	m_strFiveLine;
	CString	m_strSixLIne;
	CString m_strGridColor;
	CString	m_strBlockColor;
	CString m_strLattice;
	CString	m_strBlockBorderColor;
	CString	m_strAnalyzeToolColor;
	CString	m_strBackBorderColor;
	CString	m_strVerticalScaleLine;
	CString	m_strNumericalInquiry;

// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
protected:
	IPropertyMap *	m_pIPropertyMap;

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSETDLG_H__B3B9380F_843F_4FFE_8E10_3BF5BBB6210E__INCLUDED_)
