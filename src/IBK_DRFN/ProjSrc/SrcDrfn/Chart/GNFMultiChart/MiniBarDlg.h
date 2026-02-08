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


class CMiniBarDlg : public CInputBarDlg
{
// Construction
public:
	CMiniBarDlg(CWnd* pParent = NULL);   // standard constructor
	void SetType(BOOL bSendChartMng = TRUE); // UpdateBy m_chType(틱(0)/분(1)/일(2)/주(3)/월(4)) and m_nUnitPos
//	void OnBtperiod();
	void SetAllPosition(int nType);
// Dialog Data
	//{{AFX_DATA(CMiniBarDlg)
	enum { IDD = IDD_MINIBAR };
//	CILButton	m_btChart;
//	CILButton	m_btIndex;
//	//CILButton	m_btAddIndex;
//	CILButton	m_chkShowScale;
//	CILButton	m_btYUnit;

	CILButton	m_btnMoveToLeftRight;
	CILButton	m_btnDayWeekMonth;
	CILButton	m_btnMinSec;
	CILButton	m_btnTick;
	CILButton	m_btnSel_DayWeekMonth;
	CILButton	m_btnSel_MinSec;
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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
	afx_msg void OnSelchangeCbmarket();
//	afx_msg void OnBtnChangeunit();
	afx_msg void OnMoveToLeftRight();
	afx_msg void OnBtnDayWeekMonth();
	afx_msg void OnBtnMinSec();
	afx_msg void OnBtnTick();
	afx_msg void OnBtn_Sel_DayWeekMonth();
	afx_msg void OnBtn_Sel_MinSec();
//	afx_msg void OnBtperiod();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINIBARDLG_H__3CD11890_5BD0_4DBA_B909_F94DEDA6EA30__INCLUDED_)
