#if !defined(AFX_NEWSSETWND_H__90CDC6CC_B7F8_4256_BAD3_70B7CE836A99__INCLUDED_)
#define AFX_NEWSSETWND_H__90CDC6CC_B7F8_4256_BAD3_70B7CE836A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewsSetWnd.h : header file
//

//#include "Dr_Control/DrBtnCtrl.h"
#include "../../DrCommon/Dr_Control/DrBtnCtrl.H"
#include "../../DrCommon/Dr_Control/DrInfoTip.h"

//#include "./AddControl/KeywordGrid.h"

#include "./AddControl/Grid_Comm.h"
#include "./AddControl/SetSignalGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CNewsSetWnd window

class CNewsSetWnd : public CWnd
{
// Construction
public:
	CNewsSetWnd();
	virtual ~CNewsSetWnd();

  BOOL m_bReg_UserKeyword;    //# Change Samsung! (2007.1 : Danny)

private:
  CFont m_fontDefault, m_fontBold;
  CBrush  m_brushStatic;
  CBrush  m_brushSubTitlePoint;

  CString m_strInfoTip;
  int m_nTipMaxLength;

	int m_nRow_FindKeyword;
	CUIntArray m_iaRow_FindKeyword;

public:
  CStatic		      m_ctrlLeftTitlePoint;
	CStatic		      m_ctrlLeftTitle;
	//CKeywordGrid		m_gridKeyword;
  CGrid_Comm      m_gridKeyword;

	CStatic		      m_ctrlFind;
	CEdit			      m_editFind;
	CDrBtnCtrl		  m_btnFind;
	CDrBtnCtrl		  m_btnNext;
	CDrBtnCtrl		  m_btnPre;

	CDrBtnCtrl		  m_btnAdd;
	CDrBtnCtrl		  m_btnDelete;
  CDrInfoTip      m_tipInfo;
	CDrBtnCtrl		  m_btnTip;

	CDrBtnCtrl		  m_btnLeft;
	CDrBtnCtrl		  m_btnLeftAll;
	CDrBtnCtrl		  m_btnRight;
	CDrBtnCtrl		  m_btnRightAll;

  CStatic		m_ctrlRightTitlePoint;
	CStatic		m_ctrlRightTitle;
	CStatic		m_ctrlAddCnt;
	
	CSetSignalGrid		m_gridSetSignal;

	CList <KeywordData, KeywordData&>			m_listUserData;
	CMapStringToPtr	m_mapSignalSet;
	BOOL m_bShow_PF_CTL;

public:
	BOOL Create(LPCTSTR lpszWindowName, CWnd* pParentWnd, UINT nID);

	void SetInit();
  void SetInit_SetVal();

  void SetDrewBox(CPaintDC *pDC, CRect reData);

	void OnSize_Left(CRect reClient);
	void OnSize_Move(CRect reClient);
	void OnSize_Right(CRect reClient);
  int GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }

	void SetLoadFile();
	void SetLoad_KeyWord(int nType, CString strFilePath);

	CString GetSignalDataToKey(SetSignalData	*pstSetSignalData);
  
	void SetAdd_SetSignalData(KeywordData stKeywordData, BOOL bMsg = TRUE);
	void GetData_DefultSignal(KeywordData stKeywordData, SetSignalData	&stSetSignalData);
  void SetDelete_SetSignalData(int nRow);

	POSITION GetUserKeywordPos(CString strName_S, CString strName_V);

	void SetSaveFile();
	void SetClear_SignalSet(CMapStringToPtr *pmapSignalSet);

  int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = PROGRAM_TITLE, UINT nType = MB_OK);

  void OnPF_Contorl_Move();
  void PlatFormBtnNewsAddAll();
  void PlatFormBtnNewsAdd(); 
  void PlatFormBtnNewsRemoveAll();
  void PlatFormBtnNewsRemove();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewsSetWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:


	// Generated message map functions
protected:
	//{{AFX_MSG(CNewsSetWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	afx_msg void OnBtnLeft();
	afx_msg void OnBtnLeftAll();
	afx_msg void OnBtnRight();
	afx_msg void OnBtnRightAll();

	afx_msg void OnBtnFind();
	afx_msg void OnBtnPre();
	afx_msg void OnBtnNext();

	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnTip();

	//afx_msg void OnNotify_Grid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotify_Grid(WPARAM wParam, LPARAM lParam);
	//afx_msg void OnNotify_SignalGrid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotify_SignalGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWSSETWND_H__90CDC6CC_B7F8_4256_BAD3_70B7CE836A99__INCLUDED_)
