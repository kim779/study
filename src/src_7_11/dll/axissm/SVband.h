#if !defined(AFX_SVBAND_H__2C02A04A_21AA_47A2_A303_8AB6418D3501__INCLUDED_)
#define AFX_SVBAND_H__2C02A04A_21AA_47A2_A303_8AB6418D3501__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SVband.h : header file
//
#include <afxtempl.h> 

#define HILIGHT		::GetSysColor(COLOR_BTNHILIGHT)
#define	SHADOW		RGB(155, 155, 155)
#define	IPOS_LEFT	1
#define	IPOS_RIGHT	2

#define	IMG_NONETOGGLE	0x00
#define	IMG_NEWSTOGGLE	0x01
#define	IMG_CONTOGGLE	0x02
//#define	MAX		5
/////////////////////////////////////////////////////////////////////////////
// CSVband window

class AFX_EXT_CLASS CSVband : public CWnd
{
// Construction
public:
	CSVband(bool (*callback)(int, WPARAM, LPARAM), CString home);

// Attributes
protected:
	int		m_skinN;
	int		m_pushN;
	bool	m_bshape;
	int		m_vsN;
	
	CString		m_home;
	COLORREF	m_bk;
	CStringArray	m_ary;
	CArray <DWORD, DWORD>	m_aryColor;
	CArray <DWORD, DWORD>	m_aryLine;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

	int		m_mode;
	class CSkinDlg*	m_dlg;
	class CPalButton*	m_pPal;
	class CNButton*	m_pFunc[4];
	class CNButton*	m_pVS[4];

// Operations
public:
	CSize	GetBandWidth();
	void	make_Ctrl();
	void	changeSkin(CString skinName);
	void	set_Background(COLORREF color);
	void	draw_Seperate(CDC* pDC, CRect rc);
	void	change_VirtualScreen(int virtualScreenN);
	void	SetVSToolTip(int vs, CString tip);

protected:	
	void	loadPalette();
	void	move_Ctrl();
	void	set_ButtonImg(int vsN);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSVband)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSVband();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSVband)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg	LRESULT OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SVBAND_H__2C02A04A_21AA_47A2_A303_8AB6418D3501__INCLUDED_)
