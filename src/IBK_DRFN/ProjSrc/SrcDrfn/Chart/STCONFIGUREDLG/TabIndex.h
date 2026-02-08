#if !defined(AFX_TABINDEX_H__12DF8124_B613_454F_B4B5_F8DEA1F07D4D__INCLUDED_)
#define AFX_TABINDEX_H__12DF8124_B613_454F_B4B5_F8DEA1F07D4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabIndex.h : header file
//
#include <afxtempl.h>
#include "Resource.h"
#include "RscDlg.h"
#include "Control/ColorButton.h"
#include "Control/BitmapItemComboBox.h"
#include "Control/XListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTabIndex dialog

class CIndexData
{
public:
	CIndexData()
	{
		Init();
	}
	void Init()
	{
		m_clrIndex = RGB(0,0,0);
		m_lType = 0;
		m_lShape = 0;
		m_lThick = 0;
		m_nPos = 0;
	}
	COLORREF m_clrIndex;
	long m_lType;
	long m_lShape;
	long m_lThick;
	long m_nPos;
};

class CTabIndex : public CRscDlg
{
// Construction
public:
	CTabIndex(CWnd* pParent = NULL);   // standard constructor
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}

// Dialog Data
	//{{AFX_DATA(CTabIndex)
	enum { IDD = IDD_DLG_TAB_INDEX };
	CStatic	m_stThick;
	CStatic	m_stShape;
	CButton	m_chkSaveAsDefault;
	CColorButton	m_btnGraphClr;
	CXListCtrl	m_listGraph;
	CComboBox	m_cbGraphType;
	CBitmapItemComboBox	m_cbGraphThick;
	CBitmapItemComboBox	m_cbGraphShape;
	CColorButton	m_btnClrBear;
	CColorButton	m_btnClrBull;
	//}}AFX_DATA
private:
	void InitComboBox();
	void InitColumn();
//	void AddLineType(CComboBox* pCombo, int Type);
	void OnBtnClr(CColorButton &rColorButton);
	void LoadInfo();
	BOOL SaveInfo();
	void RemoveInfo();
	CArray<CIndexData,CIndexData> m_arrayIndexData;
	COLORREF m_crBk;
	CBrush*	m_pBrBkColor;
	CWnd*	m_pDlgSTOption;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabIndex)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnClrBull();
	afx_msg void OnBtnCltBear();
	afx_msg void OnBtnGraphClr();
	afx_msg void OnItemchangedListGraph(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCbGraphShape();
	afx_msg void OnSelchangeCbGraphThick();
	afx_msg void OnSelchangeCbGraphType();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABINDEX_H__12DF8124_B613_454F_B4B5_F8DEA1F07D4D__INCLUDED_)
