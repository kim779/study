#if !defined(AFX_DBAR1_H__A26A0521_6588_48AB_9C8A_D38A41CEA252__INCLUDED_)
#define AFX_DBAR1_H__A26A0521_6588_48AB_9C8A_D38A41CEA252__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dbar1.h : header file
//


#define	IMG_MAX		3
#define	IMG_BACKGROUND	0
#define	IMG_TOOLIMAGE	1
#define	IMG_TOOLEXPAND	2

/////////////////////////////////////////////////////////////////////////////
// CDbar1 window

class AFX_EXT_CLASS CDbar1 : public CDialogBar
{
// Construction
public:
	CDbar1(bool (*callback)(int, WPARAM, LPARAM), CString home);

// Attributes
protected:
	int		m_itemW;
	int		m_height;
	int		m_type;
	bool		m_defaultColor;
	int		m_maxfunc;
	COLORREF	m_bk;
	COLORREF	m_color;

	CString		m_home;
	CString		m_user;
	CRect		m_bRc;

	CFont		m_font;
	CBitmap		m_img[IMG_MAX];

	class CXcom*	m_xcom;
	class CXcomex*	m_xcomex;


	class CNButton*	m_hide;
	class CNButton*	m_searchS;
	class CNButton*	m_register;
	class CNButton*	m_zero;

	class CHBand*	m_band;
	class CTMenu*	m_menu;
	class CSVband*	m_Vband;

	class CfxSpin*	m_CSPIN;		// by NMH

	class CNButton*	m_func[5];
	class CNButton*	m_btnBanner;

	CObArray	m_tickers;	// ticker
	bool	(*m_axiscall)(int, WPARAM, LPARAM);
// Operations
public:
	void	init();
	void	make_Ctrl();
	void	Change_Skin(CString skinName);
	void	set_Background(COLORREF color);
	void	set_Barheight(int height);
	void	SetHome();
	void	set_Maps(CString mapname = _T(""));
	//void	set_Code(CString code);
	void	set_List(CStringArray& array);
	void	add_History(CString dat);
	void	set_MenuInfo(class CTMenu* menu)	{ m_menu = menu; }
	void	Refresh();
	CString	screenInfo();
	CPoint	GetPos(int index);
	void	AddItem(CString dat);
	void	set_Owner();
	bool	IsInputFocus();
	void	SetBandType(int type);

	void	Update(int type, char* data);
	void	Update(int type, CStringArray& items);
	void	SetsbConfig(int type, DWORD item, bool beep, CString file);
	void	drawingColor(COLORREF bkGround);
	void	Initialize_Ticker();

	void	SetSDI();
	void	SetMDI();

	void	SetPalette();
	void	ResetPalette();
	CString Parser(CString &srcstr, CString substr);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbar1)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDbar1();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	// Generated message map functions
protected:
	CRect	get_GripperRect();

	void	draw_Gripper(CDC* pDC);
	void	draw_BackGround(CDC* pDC);
	void	draw_Seperate(CDC* pDC, CRect rc);
	void	draw_Button(bool push = false);
	void	load_BandItem();
	void	change_Group(int index);
	bool	IsList(CString key);
	void	ShowMenu();
	void	SetPosition();
	void	SetBandType();

	//{{AFX_MSG(CDbar1)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg	LRESULT OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBAR1_H__A26A0521_6588_48AB_9C8A_D38A41CEA252__INCLUDED_)
