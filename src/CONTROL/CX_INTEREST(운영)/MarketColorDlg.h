//#include "../../fb/XTrScr/XTGlobal.h"     
//#include "../../fb/XTrScr/XTColorPicker.h"

#pragma once
// MarketColorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMarketColorDlg dialog

class CMarketColorDlg : public CDialog
{
// Construction
public:
	CMarketColorDlg(CWnd* pParent = nullptr);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMarketColorDlg)
	enum { IDD = IDD_MARKET_COLOR };
/*@@Zeta
	CXTColorPicker	m_ctlKospiColor; 
	CXTColorPicker	m_ctlKosdaqColor; 
*/
	CButton	m_cbApply;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarketColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	BOOL m_bMKClrApply{};
	COLORREF m_clrKospi{};
	COLORREF m_clrKosdaq{};

// Operators
private:
	void EnableConrol(BOOL bEnable);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMarketColorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckApply();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};