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
#include "usedefine.h"
class CSVband : public CWnd
{
// Construction
public:
	CSVband(bool (*callback)(int, WPARAM, LPARAM), bool bSDI = false);

// Attributes
protected:
	int			m_pushN;
	bool		m_bshape;
	bool		m_bSDI;
	int			m_vsN;
	int			m_type;
	
	COLORREF	m_bk;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

	int			m_mode;

#ifdef DF_USESTL_SVBAND
	std::unique_ptr<class CNButton> m_pChange;
#else
	class CNButton*	m_pChange;
#endif

// Operations
public:
	CSize	GetBandWidth();
	void	make_Ctrl(int mode);
	void	change_Skin(CString skinName);
	void	set_Background(COLORREF color);
	void	draw_Seperate(CDC* pDC, CRect rc);
	void	SetBandType(int type);
	int		GetBandMode();

protected:	
	void	loadPalette();
	void	move_Ctrl();

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
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg	LONG OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CBitmap* m_bmpBand;
	CBitmap* m_bmpLeft;
	CBitmap* m_bmpMiddle;
	CBitmap* m_bmpRight;
	int m_bmLWidth,m_bmMWidth,m_bmRWidth, m_bmHeight;
	int m_bmBandWidth, m_bmBandHeight;
public:
	int GetWidth();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SVBAND_H__2C02A04A_21AA_47A2_A303_8AB6418D3501__INCLUDED_)
