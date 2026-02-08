#if !defined(AFX_MULTIFODIALOG_H__99ECE68E_7594_4D7F_9C1F_DA88FE40EC8C__INCLUDED_)
#define AFX_MULTIFODIALOG_H__99ECE68E_7594_4D7F_9C1F_DA88FE40EC8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <list>
#include <algorithm>
#include <functional>

#include "../../control/fx_grid/fxGrid.h"
#include "axiscode.h"
#include "user.h"

using namespace std;

class CMultiFODialog : public CDialog
{
// Construction
public:
	CMultiFODialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMultiFODialog)
	enum { IDD = IDD_DIALOG_IB300400 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiFODialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CString GetData();
protected:
	CString m_code, m_name;
	vector<FCODE> m_fjcodes;
	vector<CString> m_month;
	vector<struct ojcode> m_ojcodes;
protected:
	afx_msg void OnFGridLeftUp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFGridCellChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOGridLeftUp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOGridCellChanged(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	CfxGrid m_fgrid, m_ogrid;
protected:
	void InitFGrid();
	void InitOGrid(int midx);
	void loadFJCode();
	void loadOJCode();
	int GetOptionCount(int midx);
	int GetSelectCount();

// Implementation
protected:
	void OnOK();
	void OnCancel();

	// Generated message map functions
	//{{AFX_MSG(CMultiFODialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioMon1();
	afx_msg void OnRadioMon2();
	afx_msg void OnRadioMon3();
	afx_msg void OnRadioMon4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIFODIALOG_H__99ECE68E_7594_4D7F_9C1F_DA88FE40EC8C__INCLUDED_)
