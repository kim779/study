#if !defined(AFX_DBAR1_H__A26A0521_6588_48AB_9C8A_D38A41CEA252__INCLUDED_)
#define AFX_DBAR1_H__A26A0521_6588_48AB_9C8A_D38A41CEA252__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dbar1.h : header file
//
#include <afxtempl.h>
#include "usedefine.h"

#define	IMG_MAX		3
#define	IMG_BACKGROUND	0
#define	IMG_TOOLIMAGE	1
#define	IMG_TOOLEXPAND	2

#define TM_TOPBANNER_ROTATE		1000
#define TM_STAFF_TOPBANNER_ROTATE		1001
#define TM_TOP10_RATATE		1002
#define TM_CHOKNTOAK_ROTATE		1003
#define TM_TOP10			1004

#define MAX_FUNC	9 + 1 
/////////////////////////////////////////////////////////////////////////////
// CDbar1 windows
class AFX_EXT_CLASS CDbar1 : public CDialogBar
{
// Construction
public:
	CDbar1(bool (*callback)(int, WPARAM, LPARAM));

// Attributes
protected:
	int		m_itemW;
	int		m_height;
	int		m_type;

	int		m_nFunc;
	int		m_bFunc;
	int		m_FuncType;
	CArray <int, int>	m_arFunc;

	int		m_vMode;
	int		m_vsN;

	COLORREF	m_color;

	CRect		m_bRc;

	CFont		m_font;
	CString		m_dept;

#ifdef DF_USESTL_DBAR1
	std::unique_ptr<class CXcom> m_xcom;
	std::unique_ptr<class CNButton> m_searchS;
	std::unique_ptr<class CNButton> m_tacnt;
	std::unique_ptr<class CNButton> m_func[MAX_FUNC];
	std::shared_ptr<CNButton> btnHOB;
#ifndef DF_MK_CAPTION
	std::shared_ptr<CNButton> btnAI;
#endif
	std::unique_ptr<class CXcomex> m_xcomex;

#else 
	class CXcom* m_xcom;
	class CNButton* m_searchS;
	class CNButton* m_tacnt;
	class CNButton* m_func[MAX_FUNC];
	class CXcomex* m_xcomex;
#endif
	
	
	class CNButton* m_hide{};
	class CNButton* m_register{};
	

	class CHBand* m_band{};
	class CSDIBand* m_sband{};
	class CSVband* m_vband{};
	class CTMenu* m_menu{};
	
	// for Banner
	HINSTANCE m_hHtmlInst;
	CWnd* m_banner{};
	CWnd* m_staffbanner{};
	CWnd* m_top10{};
	CNButton *m_btnBanner{};
	CNButton *m_btnStaffBanner{};
	CNButton *m_btnTop10{};

#ifdef DF_MK_CAPTION
	class CNButton* m_btKRX{};
	class CNButton* m_btNXT{};
	class CNButton* m_btMKSELECT{};
#endif
//	CWnd *m_ChokNTalk;
#ifndef DF_USESTL_DBAR1
	CNButton *m_btnHOB;
#endif
	CWnd *m_HOB{};
#ifndef DF_MK_CAPTION
	CWnd* m_AI{};
#endif
	HBITMAP m_bitmapEvent;
	BOOL	m_bUseBanner;
	CStringArray m_arrTOP10;

	bool	(*m_axiscall)(int, WPARAM, LPARAM);

// Operations
public:
	void	init();
	void	make_Ctrl();
	void	setDept(CString dept);
	void	Change_Skin(CString skinName);
	void	set_Background(COLORREF color);
	void	set_Barheight(int height);
	void	set_Maps(CString mapname = _T(""));
	void	set_Code(CString code);
	void	set_List(const CStringArray& array);
	void	add_History(CString dat);
	void	set_MenuInfo(class CTMenu* menu)	{ 
		CString slog;
		slog.Format("[axissm][Dbar1][set_MenuInfo] menu=[%p]\n", menu);
		OutputDebugString(slog);
		m_menu = menu;
	}
	void	Refresh();
	void	RefreshFunc();
	CString	screenInfo();
	CPoint	GetPos(int index);
	void	AddItem(CString dat);
	void	set_Owner();
	bool	IsInputFocus();
	void	SetBandType(int type);
	void	SetPosition();
	void	SetHome();
	void	SetTOP10(const CStringArray &arr);
	void	PBMsgFromMain(CString strdata = ""); //khs
#ifdef DF_MK_CAPTION
	void SetShowAIBtn(int Kgubn, int Ngubn = 0);
#else
	void SetShowAIBtn(bool bshow,int Ngubn = 0);
#endif
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbar1)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDbar1();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	// Generated message map functions
protected:
	CRect	get_GripperRect();

	void	draw_BackGround(CDC* pDC);
	void	draw_Button(bool push = false);
	void	draw_BandTerm(CDC* pDC, int x);
	
	void	load_BandItem();
	void	change_Group(int index);
	bool	IsList(CString key);
	void	ShowMenu();
	void	SetBandType();
	CString Parser(CString &srcstr, CString substr);
	CString GetNewChart(CString old);
	BOOL	GetOldChart(CString section);
	void	RotateBanner();
	void	SetEventBitmap(int index);

	//{{AFX_MSG(CDbar1)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg	LONG OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CBitmap* m_bmpBand;
	CBitmap* m_bmpHome;

	int			  m_currEvent;
	int			  m_currStaffEvent;
	int			  m_currTOP10;

	int m_xcomX, m_funcX, m_registerX;

	bool m_bRotate;
	bool m_bStaffRotate;
	int  m_nRCnt;
	int	 m_nRTime;
	int  m_nStaffRCnt;
	int  m_nStaffRTime;
	int  m_nTOPCount;
	int  m_nTOPTime;
	int  m_nChokRotate;
	int m_showAI{};
public:
	//CRect tool_icon;
	CString m_slog{};
	int GetHeight();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBAR1_H__A26A0521_6588_48AB_9C8A_D38A41CEA252__INCLUDED_)
