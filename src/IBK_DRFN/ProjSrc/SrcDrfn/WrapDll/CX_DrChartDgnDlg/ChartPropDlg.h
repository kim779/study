//{{AFX_INCLUDES()
#include "../../DrCommon/DrChartOcx.h"
//#include "SMSChart.h"
//}}AFX_INCLUDES
#if !defined(AFX_CHARTPROPDLG_H__5D895DD8_B3FE_4D47_9784_338BD0255DDF__INCLUDED_)
#define AFX_CHARTPROPDLG_H__5D895DD8_B3FE_4D47_9784_338BD0255DDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChartPropDlg.h : header file
//

#include "../common/WinixConverter.h"

/////////////////////////////////////////////////////////////////////////////
// CChartPropDlg dialog

class CChartPropDlg : public CDialog
{
// Construction
public:
	CChartPropDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChartPropDlg)
	enum { IDD = IDD_CHARTPROPDLG };
	CButton	m_ChkDirect;
	CStatic	m_stcCntTitle;
	CButton	m_btnDataSet;
	CEdit	m_editRowCnt;
	int		m_nTRType;
	BOOL	m_nSortType;
	BOOL	m_bDirectSend;
	CStatic	m_stcGridSymbol;
	CEdit	m_editGridSymbol;
	CComboBox	m_cbChartType;
	//}}AFX_DATA
	//int		m_nFID;

	//CDRChartOcx		m_chart;
	CDRChartOcx		*m_pChart;
	HWND			m_hCtrlWnd;
	CString			m_strMapPath;
	CString			m_strFileName;

	CWinixConverter	m_converter;

//	CTrPack			m_query;
//	CStringList		m_propList;

public:
	CString			GetPath()	{ return m_strMapPath;	};
	CString			GetFileName()	{ return m_strFileName;	};
	void			SetFileName(CString strFileName)	{ if (strFileName.GetLength() > 0) m_strFileName = strFileName;	};
	void			SetMapPath(CString strMap)	{ m_strMapPath = strMap;	};
	BOOL			SetAllPath(CString strPath);
	BOOL			SetAllPropertiesToOCX();

	void			SaveFile();
	void			LoadFile();

	void			ShowFIDCtls();
	void			CheckEnableOfChartSet();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChartPropDlg)
	afx_msg void OnBtnDataset();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnRadioFid();
	afx_msg void OnRadioTr();
	afx_msg void OnDatasort();
	afx_msg void OnChkdirect();
	afx_msg void OnSelchangeCmbChartType();
	afx_msg void OnDrsaveChartset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTPROPDLG_H__5D895DD8_B3FE_4D47_9784_338BD0255DDF__INCLUDED_)
