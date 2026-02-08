#if !defined(AFX_MATCHVALEDITDLG_H__3CBD7A24_A6A7_4549_BC1D_6D1A427F362E__INCLUDED_)
#define AFX_MATCHVALEDITDLG_H__3CBD7A24_A6A7_4549_BC1D_6D1A427F362E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MathValEditDlg.h : header file
//

#include "resource.h"
#include "../../../SrcSite/control/fx_misc/fxImgButton.h"
/////////////////////////////////////////////////////////////////////////////
// CMatchValEditDlg dialog

class CGrid_Comm;
class CMatchValEditDlg : public CDialog
{
public:
	CMatchValEditDlg(CWnd* pParent = NULL);   // standard constructor

	CWnd *m_pwndMain;

private:
  CFont m_fontDefault, m_fontBold;
  CBrush m_brushBk;
  CBrush m_brusEdit;

public:
	CGrid_Comm		*m_pgridSource;
	CGrid_Comm		*m_pgridDest;
	
	CfxImgButton	m_btnLeft;
	CfxImgButton	m_btnLeftAll;
	CfxImgButton	m_btnRight;
	CfxImgButton	m_btnRightAll;

	CStatic			m_ctrlPosBar;
	CfxImgButton	m_btnUp;
	CfxImgButton	m_btnDown;
	CfxImgButton	m_btnTop;
	CfxImgButton	m_btnBottom;

	CStatic			m_ctrlLogicTitle;
	CEdit			m_editLogic;
	CfxImgButton	m_btnExp;

	CStatic			m_ctrlComment;

	CfxImgButton	m_btnOk;
	CfxImgButton	m_btnCancel;

	CList <HeadFixInfo, HeadFixInfo&>			m_listFixHeadInfo;
	CList <MatchInfo, MatchInfo&>					*m_plistMatchInfo;

private:
	BOOL m_bExpLogic;
	CStringArray		m_saOpt;

private:
	void OnSize();
	void OnSizeTopBar(CRect reClient);
	void OnSizeMoveBar(CRect reClient);
	void OnSizeMakeBar(CRect reClient);

  int GetBtnWidth(int nTextLen) { return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen; }

	int SetData_ValView(int nOffset);

	void SetSyncMatchInfo(CList <MatchInfo, MatchInfo&>	*plistSource,
													CList <MatchInfo, MatchInfo&>	*plistDest);

	void SetDest_Insert(int nSelectRow);
	void SetDest_Delete(int nSelectRow);

	int GetComparisonRow_Source(CString strFiled);

	CString GetMatchLogic();
  int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);


// Dialog Data
	//{{AFX_DATA(CMatchValEditDlg)
	enum { IDD = IDD_MATCHVALEDIT_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMatchValEditDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMatchValEditDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	afx_msg void OnBtnOk();
	afx_msg void OnBtnCancel();

	afx_msg void OnBtnLeft();
	afx_msg void OnBtnLeftAll();
	afx_msg void OnBtnRight();
	afx_msg void OnBtnRightAll();

	afx_msg void OnBtnUp();
	afx_msg void OnBtnTop();
	afx_msg void OnBtnDown();
	afx_msg void OnBtnBottom();

	afx_msg void OnBtnExp();

	//afx_msg void OnNotifyGrid(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNotifyGrid(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MATHVALEDITDLG_H__3CBD7A24_A6A7_4549_BC1D_6D1A427F362E__INCLUDED_)
