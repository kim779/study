#if !defined(AFX_BLOCKSETDLG_H__BDF15A87_3B30_4AD0_A238_9A2720525ED0__INCLUDED_)
#define AFX_BLOCKSETDLG_H__BDF15A87_3B30_4AD0_A238_9A2720525ED0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlockSetDlg.h : header file
//
#include "DlgToolTip.h"
#include "NoSystemMenuEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CBlockSetDlg dialog
interface IPropertyMap;
class CBlockSetDlg : public CDialog
{
// Construction
public:
	//Set Received Value From ocx
	void SetProperty( IPropertyMap *p_pIPropertyMap);
	CBlockSetDlg( HWND p_hOcxWnd, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBlockSetDlg)
	enum { IDD = IDD_BLOCKSET1 };
	CNoSystemMenuEdit	m_GapEdit;
	CNoSystemMenuEdit	m_ThicknessEdit;
	CListCtrl	m_BlockShape2;
	CListCtrl	m_BlockShape;
	int		m_nBlockRowCount;
	int		m_nBlockColumnCount;
	int		m_nBlockBorderWidth;
	int		m_nBlockGap;
	int		m_nBlockRow2Count;
	int		m_nBlockColumn2Count;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlockSetDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBlockSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkBlockshape(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnClickBlockshape2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnChangeBlockgap();
	afx_msg void OnClickBlockshape(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeRow();
	afx_msg void OnChangeColumn();
	afx_msg void OnChangeColumn2();
	afx_msg void OnChangeRow2();
	afx_msg void OnKillfocusBlockshape(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusBlockshape2(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//imageList 
	CImageList m_ImageList;  
	CImageList m_ImageList1;
	CImageList m_ImageList2;
	CImageList m_ImageList3;

	//save index of previous Block listCtrl item.
	int m_nPreIndex;
	//save index of previous BlockSection listCtrl item.
	int m_nPreStyleBlockSection;

	//save selected index of BlockSection listCtrl
	int m_nStyleBlockSection;
	//save selected index of Block listCtrl
	int m_nStyleBlock;

	//tooltip 
	CDlgToolTip	m_toolTip;

private:
	//블럭의 Row, Column 값이 잘못 입력되었을때 처리한다.
	void TreateWrongInputData( UINT uMessage, const int  nReplaceData, int* nTarget);
	//Row, Column, 두배로 사이즈를 Default 값으로 변경한다.
	void UpdateColumnandRowbyDefaultValue();
	//init listCtrl
	void InitListCtrl(CListCtrl* pList, UINT nIDs[], int num);

	//Create tooltip object and Add text to StringArray 
	void CreateToolTip();

	//Enable Row, Column Edit by Block Type
	void SetEnableWindow();

	void ChangeDataOftwofoldBlockEdit(int* nBlockCount, int* ntwofoldBlockCount, int nID);

// (2006/7/5 - Seung-Won, Bae) Manage Property Map for Update
protected:
	IPropertyMap *	m_pIPropertyMap;

// (2008/1/22 - Seung-Won, Bae) for Multi-Language
protected:
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLOCKSETDLG_H__BDF15A87_3B30_4AD0_A238_9A2720525ED0__INCLUDED_)
