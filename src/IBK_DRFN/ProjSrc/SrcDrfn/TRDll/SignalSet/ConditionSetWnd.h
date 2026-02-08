#if !defined(AFX_CONDITIONSETWND_H__2A4704C0_CD4B_44AA_9989_5757A12647A7__INCLUDED_)
#define AFX_CONDITIONSETWND_H__2A4704C0_CD4B_44AA_9989_5757A12647A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConditionSetWnd.h : header file
//
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
#include "./AddControl/SetSignalGrid.h"
#include "./AddControl/ConditionTreeCtrl.h"
#include "./AddControl/HelpEditCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CConditionSetWnd window

class CConditionSetWnd : public CWnd
{
	// Construction
public:
	CConditionSetWnd();
	virtual ~CConditionSetWnd();
	
private :
	int m_nType;
	CFont m_fontDefault, m_fontBold;
	
	CBrush  m_brushStatic;
	CBrush  m_brushSubTitlePoint;
	
public:
	COLORREF	m_clrMainSkin;

	CStatic		m_ctrlLeftTitlePoint;
	CStatic		m_ctrlLeftTitle;
	
	CConditionTreeCtrl	m_treeCondition;
	
	CfxImgButton	m_btnLinkSrc;
	CfxImgButton	m_btnTip;
	
	CfxImgButton	m_btnLeft;
	CfxImgButton	m_btnLeftAll;
	CfxImgButton	m_btnRight;
	CfxImgButton	m_btnRightAll;
	
	CStatic		m_ctrlRightTitlePoint;
	CStatic		m_ctrlRightTitle;
	CStatic		m_ctrlAddCnt;
	
	CSetSignalGrid		m_gridSetSignal;
	CStatic		m_ctrlNote;
	
	CList <ConditionDataGroup, ConditionDataGroup&> m_listConditionData;
	CMapStringToString m_mapConditionHelp;
	CMapStringToPtr	m_mapSignalSet;

	BOOL m_bShow_PF_CTL;

	BOOL			m_bHelpShowFlag;
	CfxImgButton	m_btnHelpShow;
	CHelpEditCtrl	m_editHelp;

public:
	void SetInit();
	void SetInit_SetVal();
	
	void OnSize_Left(CRect reClient);
	void OnSize_Move(CRect reClient);
	void OnSize_Right(CRect reClient);
	
	void SetDrewBox(CPaintDC *pDC, CRect reData);
	void SetDrewBox(CDC *pDC, CRect reData);
	
	BOOL Create(LPCTSTR lpszWindowName, int nType, CWnd* pParentWnd, UINT nID);
	
	CString GetSignalDataToKey(SetSignalData	*pstSetSignalData);
	
	void SetLoadFile();
	BOOL SetLoadFile_Data(const char* pData, 
		CString &strGroup, ConditionData	&stConditionData);
	
	POSITION GetFind_ConditionGroup(CString strGroup, ConditionDataGroup &stConditionDataGroup);
	
	void SetAdd_SetSignalData(ConditionData	stConditionData, BOOL bMsg = TRUE);
	void GetData_DefultSignal(ConditionData stConditionData, SetSignalData	&stSetSignalData);
	
	void SetDelete_SetSignalData(int nRow, BOOL bMsg = TRUE);
	
	void SetClear_SignalSet(CMapStringToPtr *pmapSignalSet);
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = PROGRAM_TITLE, UINT nType = MB_OK);

	void OnPF_Contorl_Move();
	void PlatFormBtnAdd();
	void PlatFormBtnRemoveAll();
	void PlatFormBtnRemove();

	void ChangeSkinColor(COLORREF clrBkgnd);
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);

	void SetLoadHelp();
	BOOL SetLoadHelp_Data(const char* pData, CString &strKey, CString &strExplanation);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConditionSetWnd)
	
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CConditionSetWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	
	afx_msg void OnBtnLeft();
	afx_msg void OnBtnLeftAll();
	afx_msg void OnBtnRight();
	afx_msg void OnBtnRightAll();
	afx_msg void OnBtnShowHelp();

	//afx_msg void OnNotify_Tree(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotify_Tree(WPARAM wParam, LPARAM lParam);
	//afx_msg void OnNotify_SignalGrid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotify_SignalGrid(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONDITIONSETWND_H__2A4704C0_CD4B_44AA_9989_5757A12647A7__INCLUDED_)
