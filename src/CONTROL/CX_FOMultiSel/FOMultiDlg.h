#pragma once

#include <stdlib.h>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>

#include "../../dll/AXISCODE-款侩/AxisCode.h"   //test
#include "../../dll/AXISCODE-款侩/user.h"
#include "../../control/fx_grid/fxGrid.h"
#include "btnst.h"

using namespace std;

#define DEFAULT_JQTY	"1"
#define CHK_MARK	"↓"
#define DEL_COLUMN	"|"
#define DEL_ROW		"#"

#define K200		0
#define K200MINI	1
#define K200WEEK	2

class CFOMultiDlg : public CDialog
{
// Construction
public:
	CFOMultiDlg(LPCTSTR root, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFOMultiDlg)
	enum { IDD = IDD_SELECT_DIALOG };
	CButtonST	m_ctlButtonMiniKospi200;
	CButtonST	m_ctlButtonKospi200;
	CButtonST	m_ctlButtonWeek;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFOMultiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CString GetData();

protected:
	int	m_oidx;
	CString m_root;
	vector <FCODE>		m_fjcodes;
	vector <CString>	m_month;
	vector <struct ojcode>	m_ojcodes;

protected:
	CfxGrid	m_fgrid, m_ogrid;
	int	m_igubn;		// 20190923 困努府 可记

protected:
	void	loadFJCode();
	void	loadOJCode();
	void	loadWOJCode();
	void	loadMFCode();		// 2015.07.06 KSJ mini kospi200 急拱
	void	loadMOCode();		// 2015.07.06 KSJ mini kospi200 可记
	void	InitFGrid();
	void	InitOGrid(int midx);
	int	GetOptionCount(int midx);
	void	InitData();		// 2015.07.06 KSJ 

protected:
	afx_msg void OnFGridLeftUp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnFGridCellChanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOGridLeftUp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOGridCellChanged(NMHDR *pNMHDR, LRESULT *pResult);

	// Generated message map functions
	//{{AFX_MSG(CFOMultiDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnRadioMon1();
	afx_msg void OnRadioMon2();
	afx_msg void OnRadioMon3();
	afx_msg void OnRadioMon4();
	afx_msg void OnRadioMon5();
	afx_msg void OnRadioMon6();
	afx_msg void OnRadioMon7();
	afx_msg void OnRadioMon8();
	afx_msg void OnRadioMon9();
	afx_msg void OnRadioMon10();
	afx_msg void OnRadioMon11();
	afx_msg void OnButtonKospi200();
	afx_msg void OnButtonMiniKospi200();
	afx_msg void OnBtnWeek();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
