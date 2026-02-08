#if !defined(AFX_PSCONFIGDLG_H__8EE70D36_472C_4263_BBC3_170B9E204261__INCLUDED_)
#define AFX_PSCONFIGDLG_H__8EE70D36_472C_4263_BBC3_170B9E204261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PSConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPSConfigDlg dialog

#include "control/BitmapItemComboBox.h"
#include "Control/ColorButton.h"

static TCHAR* szType[] = 
{
	_T("0"),	_T("1"), 	_T("2"),	_T("3"),	_T("4"),	_T("5"),
	_T("6"),	_T("7"),	_T("8"),	_T("9"),	_T("10"),	_T("11"),
	_T("12"),	_T("13"),	_T("14"),	_T("15"),	_T("16"),	_T("17"),
	_T("18"),	_T("19"),	_T("20"),	_T("21"),	_T("22"),	_T("23"),
	_T("24")
};


class CPSConfigDlg : public CDialog
{
// Construction
public:
	int m_nDateType;
	COLORREF m_clrLineColor;
	CPSConfigDlg(CWnd* pParent = NULL);   // standard constructor

	void AddLineType(CComboBox* pCombo, int Type);
	void Initial_ThicknessComboBox();
	void Initial_StyleComboBox();
	void Initial_ColorBtn();

// Dialog Data
	//{{AFX_DATA(CPSConfigDlg)
	enum { IDD = IDD_DLG_PS };
	CDateTimeCtrl	m_ctlEDate;
	CDateTimeCtrl	m_ctlSDate;
	CBitmapItemComboBox m_LineThickness;
	CBitmapItemComboBox	m_LineStyle;
	CColorButton m_ColorPicker;
	CTime	m_SDate;
	CTime	m_EDate;
	double	m_SPrice;
	double	m_EPrice;
	BOOL	m_bSEPrice;
	BOOL	m_bEEPrice;
	BOOL	m_bHPrice;
	BOOL	m_bLPrice;
	BOOL	m_bMPrice;
	BOOL	m_bHAmount;
	BOOL	m_bLAmount;
	BOOL	m_bMAmount;
	BOOL	m_bPCandle;
	BOOL	m_bMCandle;
	BOOL	m_bPMRatio;
	BOOL	m_bAllChange;
	//}}AFX_DATA

	int m_nLineStyle;
	int m_nLineThickness;
	int m_nTextAlignment;
	int m_nCnt;
	long m_lSDate;
	long m_lEDate;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPSConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPSConfigDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTextAlignmentLeft();
	afx_msg void OnTextAlignmentRight();
	afx_msg void OnColorPick();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSCONFIGDLG_H__8EE70D36_472C_4263_BBC3_170B9E204261__INCLUDED_)
