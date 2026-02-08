#if !defined(AFX_ESETUP_H__29E7354C_D95B_423E_B7D6_DAFC02EE41E2__INCLUDED_)
#define AFX_ESETUP_H__29E7354C_D95B_423E_B7D6_DAFC02EE41E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// esetup.h : header file
//

#include "SetupDialog.h"
#include "resource.h"
#include "LBSpinButtonCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CESetup dialog

class AFX_EXT_CLASS CESetup : public CSetupDialog
{
// Construction
public:
	void draw_groupBox(CPaintDC* pdc, int ids);
	CFont m_font;
	CBitmap* m_bmpCombo;
	CBitmap* m_bmpSpin;
	CESetup(CWnd* pParent = NULL);   // standard constructor

	virtual void	ApplySetup();
	virtual void	DefaultSetup();

// Dialog Data
	//{{AFX_DATA(CESetup)
	enum { IDD = IDD_ESETUP };
	CSpinButtonCtrl	m_spin2;
	CSpinButtonCtrl	m_spin1;
	
	CEdit	m_lEdit;
	CEdit	m_sEdit;
	UINT	m_Scnt;
	UINT	m_idletime;
	//}}AFX_DATA
protected:
	//class CTMenu* m_menu;

	int	m_childCnt;
	int	m_fontN;
	int	m_childPos;
	int	m_openMap;
	int m_screenNew;
	int m_titleChange;
	int	m_popupAcc;
	int	m_unflesh;
	int	m_msgPos;
	int	m_maintab;
	int	m_msgbox;
	int m_startscreen; // 시작 화면 보일 것 인지 여부 
	int m_srchtype;
	int m_pbSlide;		//촉앤토크 슬라이드 팝업 보일지 말지

	CString	m_usermapN;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CESetup)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	load_info();
	void	save_info();
	void	load_workspace();
	void	save_workspace();
	// Generated message map functions
	//{{AFX_MSG(CESetup)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnChkHotkey();
	//}}AFX_MSG
	afx_msg void OnDeltaposSpincontrol1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpincontrol2(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ESETUP_H__29E7354C_D95B_423E_B7D6_DAFC02EE41E2__INCLUDED_)
