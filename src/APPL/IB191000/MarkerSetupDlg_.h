//#include "../../control/Dx_ColorPicker/ColorPickerWnd.h"
#include "../../fb/XTrScr/XTGlobal.h"
#include "../../fb/XTrScr/XTColorPicker.h"

#pragma once
// MarkerSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMarkerSetupDlg dialog

class CMarkerSetupDlg : public CDialog
{
// Construction
public:
	CMarkerSetupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMarkerSetupDlg)
	enum { IDD = IDD_MARKER_SETUP };
	CXTColorPicker	m_ctlTxtColor;
	CXTColorPicker	m_ctlBkgColor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarkerSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Attributes
private:
	HMODULE m_hLib;
	CWnd*	m_pWnd;

// Operators
public:
	void LoadCtrlModule();
	void FreeCtrlModule();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMarkerSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

