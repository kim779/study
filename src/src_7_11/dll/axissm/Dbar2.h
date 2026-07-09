#if !defined(AFX_DBAR2_H__782CEE65_A03D_4ECE_81AF_A7E71341D9D3__INCLUDED_)
#define AFX_DBAR2_H__782CEE65_A03D_4ECE_81AF_A7E71341D9D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dbar2.h : header file
//
#define	MODE_COOL	0
#define	MODE_OVER	1
#define	MODE_DOWN	2

#define FUNCTIONBASE	10000
#define GROUPBASE	1000
#define FONTBASE	2000

#include <afxtempl.h>
#include "AxBitmap.h"

//#include <Afxcoll.h>
class _button
{
public:
	int		key;
	bool		active;
	CString		mapN;
	CString		dispN;
	CString		tooltip;
};
/////////////////////////////////////////////////////////////////////////////
// CDbar2 window

class AFX_EXT_CLASS CDbar2 : public CDialogBar
{
// Construction
public:
	CDbar2(bool (*callback)(int, WPARAM, LPARAM), CString home);

// Attributes
protected:
	int		m_vsN;
	int		m_buttonW;
	int		m_buttonH;
	int		m_height;
	int		m_tipN;
	int		m_hindex;
	int		m_actionN;
	bool		m_defaultColor;
	bool		m_close;

	CRect		m_bandRc;
	CRect		m_expandRc;

	CString		m_home;
	COLORREF	m_bk;

	CFont		m_font;
	CFont		m_bFont;
	CAxBitmap		m_img[3];
	CToolTipCtrl*	m_ToolTip;

	class CNButton*	m_hide;
	class CSVband*	m_band;

	bool	m_mouseIN;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);
	CArray	<class _button*, class _button* >	m_arButton[6];
	_button*	m_deleteBtn;

// Operations
public:
	void	init();
	void	make_Ctrl();
	void	Change_Skin(CString skinName);
	void	set_Barheight(int height);
	void	set_Background(COLORREF color);
	// _button function define
	void	ToggleImage(bool start = true);
	void	add_Button(int key, CString mapN, class CTMenu* menu);
	void	add_Button(int key, CString mapN, class CTMenu* menu, int vsN);
	void	del_Button(int key = -1, int vsN = -1);
	void	change_Button(int key = 0);
	void	change_ButtonText(int key, CString text, int vsN = -1);
	void	change_Info(int key, CString mapN, class CTMenu* menu);
	void	change_VirtualScreen(int virtualScreenN);
	void	get_list(CArray <class _button*, class _button* > &ary) { ary.Copy(m_arButton[m_vsN]); }
	int		getActiveKey();
	bool	ExceptMap(_button* button);
	
	void	SetSDI();
	void	SetMDI();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbar2)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDbar2();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);

	// Generated message map functions
protected:
	CRect	GetButtonRect(int index = 0);
	CRect	get_GripperRect();
	void	draw_Back(CDC* pDC);
	void	draw_Gripper(CDC* pDC);
	void	draw_Button(CDC* pDC = NULL);
	void	draw_Img(CDC* pDC, CRect iRc, bool bActive = false);
	void	draw_Expand(CDC* pDC = NULL, int mode = MODE_COOL);
	void	draw_Seperate(CDC* pDC, CRect rc);
	void	move_Band();
	_button* GetActiveButton();

	void	Inactive_Button(int vsN = -1);
	void	SetInfo(CPoint);
	void	InitToolTip();
	void	SetToolTipText(CString text, BOOL bActivate = TRUE);
	void	MakeTooltipString(int vsN);

	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CDbar2)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG
	afx_msg	LRESULT OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBAR2_H__782CEE65_A03D_4ECE_81AF_A7E71341D9D3__INCLUDED_)
