#if !defined(AFX_MINIBARDLG_H__3CD11890_5BD0_4DBA_B909_F94DEDA6EA30__INCLUDED_)
#define AFX_MINIBARDLG_H__3CD11890_5BD0_4DBA_B909_F94DEDA6EA30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MiniBarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMiniBarDlg dialog
#include "InputBarDlg.h"

class CStdDialog;
class CMiniBarDlg : public CInputBarDlg
{
// Construction
public:
	CMiniBarDlg(CWnd* pParent = NULL);   // standard constructor
	void SetType(BOOL bSendChartMng = TRUE); // UpdateBy m_chType(틱(0)/분(1)/일(2)/주(3)/월(4)) and m_nUnitPos
//	void OnBtperiod();
	void SetAllPosition(int nType);

	virtual void CreatedChartMng();
	virtual void RecievedChartMng();
	virtual void MoveCodeInputCtrl();

// Dialog Data
	//{{AFX_DATA(CMiniBarDlg)
	enum { IDD = IDD_MINIBAR };
	CILButton	m_btnInitail;
	CILButton	m_btnReuest;
	CILButton	m_btnCodeSync;
	CILButton	m_btnLayout2;
	CILButton	m_btnLayout1;
	CILButton	m_btnAllDay;
	CILButton	m_btnUpDown;
	CILButton	m_btnOneDay;
	CILButton	m_btnLowHigh;
	CILButton	m_btnMoveToLeftRight;
	CILButton	m_btnDayWeekMonth;
	CILButton	m_btnMinSec;
	CILButton	m_btnTick;
	CILButton	m_btnSel_DayWeekMonth;
	CILButton	m_btnSel_MinSec;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// m_nYUnintType 변수에 현재 차트 Y축 눈금 단위 구분값을 저장한다.
//	enum {WON_TICK, PERCENT_TICK, DOLLAR_TICK }; //원 단위, 백분율 단위, 달러 단위
//	UINT m_nYUnitType; 
	UINT	m_nMoveDirection;	// 0: None, 1:Left, 2:Right 
	UINT	GetMoveDirection() { return m_nMoveDirection; }
	void	SetMoveDirection(UINT nMoveDirection) { m_nMoveDirection = nMoveDirection; }
	void	MoveToLeftRight(UINT nMoveDirection);

	virtual void Init();
	virtual void SaveIniValue();
	virtual void SetCodeName(int nMarketType, LPCSTR szCode, LPCSTR szName);

//	void SetYUnitType();
//	//virtual void LoadIniValue();
//	//virtual void SaveIniValue();
//	void OnChkshowscale2();
//	void OnBtchart();
//	void OnBtindex();
//	void OnBtaddindex();
	// Generated message map functions
	//{{AFX_MSG(CMiniBarDlg)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeCbmarket();
	afx_msg void OnBtnAllday();
	afx_msg void OnBtnUpdown();
	afx_msg void OnBtnOneday();
	afx_msg void OnBtnLowhigh();
	afx_msg void OnChkType1();
	afx_msg void OnChkType2();
	afx_msg void OnChkType3();
	afx_msg void OnChkCodesync();
	afx_msg void OnBtnRequest();
	afx_msg void OnBtinitial();
	//}}AFX_MSG
	afx_msg void OnMoveToLeftRight();
	afx_msg void OnBtnDayWeekMonth();
	afx_msg void OnBtnMinSec();
	afx_msg void OnBtnTick();
	afx_msg void OnBtn_Sel_DayWeekMonth();
	afx_msg void OnBtn_Sel_MinSec();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINIBARDLG_H__3CD11890_5BD0_4DBA_B909_F94DEDA6EA30__INCLUDED_)
