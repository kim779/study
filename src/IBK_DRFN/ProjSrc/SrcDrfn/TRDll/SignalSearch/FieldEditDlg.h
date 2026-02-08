#if !defined(AFX_FIELDEDITDLG_H__083AD66C_9A20_4BCA_868B_B41FE339414E__INCLUDED_)
#define AFX_FIELDEDITDLG_H__083AD66C_9A20_4BCA_868B_B41FE339414E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FieldEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFieldEditDlg dialog

#include "resource.h"
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "../../DrCommon/Dr_Control/DrTabCtrl.h"

#include "./AddControls/ExEditCtrl.h"

class CGrid_Field;
class CFieldEditDlg : public CDialog
{
public:
	CFieldEditDlg(CWnd* pParent, int nMoveFieldCnt);

	CWnd	*m_pwndMain;
	int m_nMoveFieldCnt;

private:
	CFont m_fontDefault;
	CBrush m_brushBk;
	
public:
	CGrid_Field*	m_pgridSource;
	CGrid_Field*	m_pgridDest;

	CfxImgButton	m_btnUp;
	CfxImgButton	m_btnDown;
	CfxImgButton	m_btnTop;
	CfxImgButton	m_btnBottom;

	CDrTabCtrl*		m_pTab;
	CRect			m_rtTab;

	CStatic			m_ctrlTitle;
	//CExEditCtrl	    m_editFixCol;
	CEdit			m_editFixCol;
	CSpinButtonCtrl	m_btnSpin;

	CfxImgButton	m_btnSetInit;

	CfxImgButton	m_btnOk;
	CfxImgButton	m_btnApply;
	CfxImgButton	m_btnCancel;

	CString			m_strVersion;

	ST_TABCOLOR		m_stTabColor;
	COLORREF		m_clrBkgnd;

	BOOL			m_bInsertState;
	BOOL			m_bRedraw;
	
	CList <HeadFixInfo, HeadFixInfo&>		*m_plistFixHeadUser;
	CList <HeadInfoType, HeadInfoType&>		*m_plistFixHeadSys;
	CList <HeadInfoType, HeadInfoType&>		*m_plistSelectedHeader;
	CList <HeadFixInfo, HeadFixInfo&>		*m_plistTempHeader;

	int m_nGridSourceRow;
	//Member Functions
public:
	void OnSizePosBar(CRect reClient);
	int GetBtnWidth(int nTextLen);

	BOOL SetSaveXmlFile(CString strFile);

	int GetRowField(CString strField, int nFieldID);
	BOOL GetPosField(CString strFieldID, int &nFindRow, int &nFindCol);

	void SetDest_Insert(int nSelectRow, int nCol);
	void SetSpin();

	void ChangeSkinColor(COLORREF clrBkgnd, ST_TABCOLOR stTabColor);

	void Debug_DataView(CStringArray *psaData);

	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);

	// Dialog Data
	//{{AFX_DATA(CFieldEditDlg)
	enum { IDD = IDD_FIELDEDIT_DLG };
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFieldEditDlg)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFieldEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	afx_msg void OnBtnUp();
	afx_msg void OnBtnTop();
	afx_msg void OnBtnDown();
	afx_msg void OnBtnBottom();

	afx_msg void OnBtnSetInit();


	afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBtnOk();
	afx_msg void OnBtnApply();
	afx_msg void OnBtnCancel();

	afx_msg void OnKillFocusFixColEdit();

	afx_msg void OnChangeTab(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg LRESULT OnNotifyGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIELDEDITDLG_H__083AD66C_9A20_4BCA_868B_B41FE339414E__INCLUDED_)
