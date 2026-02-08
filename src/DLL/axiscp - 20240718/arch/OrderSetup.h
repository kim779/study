#if !defined(AFX_ORDERSETUP_H__CF77E845_8DC3_4AFE_9757_D1B3D763A5A6__INCLUDED_)
#define AFX_ORDERSETUP_H__CF77E845_8DC3_4AFE_9757_D1B3D763A5A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OrderSetup.h : header file
//

#include "SetupDialog.h"
#include "resource.h"

#define	MAXSOUND	7
#define IDX_MDORDER	0
#define IDX_MSORDER	1
#define IDX_JJORDER	2
#define IDX_CSORDER	3
#define IDX_MDCON	4
#define IDX_MSCON	5
#define IDX_REFUSAL	6

// conclusion
#define	CON_NONE	0x00
#define	CON_POPUP	0x01
#define	CON_MSN		0x02
#define	CON_SOUND	0x04
#define	CON_ALL		CON_POPUP|CON_MSN|CON_SOUND

#define BOX_LEFT	0x01
#define BOX_RIGHT	0x02
#define BOX_TOP		0x04
#define BOX_BOTTOM	0x08
#define BOX_ALL		(BOX_LEFT|BOX_RIGHT|BOX_TOP|BOX_BOTTOM)

/////////////////////////////////////////////////////////////////////////////
// COrderSetup dialog

class AFX_EXT_CLASS COrderSetup : public CSetupDialog
{
// Construction
public:
	COrderSetup(CWnd* pParent = NULL);   // standard constructor

	virtual void	ApplySetup();
	virtual void	DefaultSetup();

// Dialog Data
	//{{AFX_DATA(COrderSetup)
	enum { IDD = IDD_ORDER_SETUP };
	UINT	m_ckcount;
	UINT	m_autohide;
	//}}AFX_DATA
protected:
	CString	m_waveF[MAXSOUND];
	DWORD	m_status;
	CRect	m_rect;
	void	DrawBox(CDC* pDC, CRect rect, COLORREF clrLine = RGB(0, 0, 0), int opt = BOX_ALL, COLORREF clrInner = RGB(255, 255, 255), bool bFill = false);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderSetup)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	loadInfo();
	void	saveInfo();
	void	SearchFile(int idx);
	void	Soundplay(int idx);
	void	EnableConclusion(BOOL bEnable);
	void	EnableSound(BOOL bEnable);
	// Generated message map functions
	//{{AFX_MSG(COrderSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnCheSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHideSpin(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDERSETUP_H__CF77E845_8DC3_4AFE_9757_D1B3D763A5A6__INCLUDED_)
