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

#include "pclock.h"
#include "enbitmap.h"
#include <afxtempl.h>
//#include <Afxcoll.h>
#include "pptooltip/PPTooltip.h"
#include "UseStopPane.h"

#include "Terminator.h"

class _button
{
public:
	int		key{};
	bool		active{};
	CString		mapN;
	CString		dispN;
	CString		tooltip;
	int		state{};		// cej
};

class CNButton;
class CTerminator;
/////////////////////////////////////////////////////////////////////////////
// CDbar2 window
class AFX_EXT_CLASS CDbar2 : public CDialogBar
{
// Construction
public:
	CDbar2(bool (*callback)(int, WPARAM, LPARAM));
	CImageList*	m_pDragImage;

// Attributes
protected:
	BOOL		m_bBottom;
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

	COLORREF	m_bk;

	CFont		m_font;
	CFont		m_bFont;
	//CEnBitmap	m_img[3];
	CBitmap*	m_img[7]{};

#ifdef DF_USESTL_DBAR2
	std::unique_ptr<CToolTipCtrl>m_ToolTip;
	std::unique_ptr<CUseStopPane>m_pStopPane;
#else
	CToolTipCtrl* m_ToolTip;
	CUseStopPane* m_pStopPane;
#endif

	CPClock		*m_pClock{};
	BOOL		m_bMDIMode;
	CPPToolTip	m_tip[2];

	BOOL		m_bDragEnter;
	int		m_nDragItem;
	int		m_nDragIndex;

	//class CSVband*	m_band;

	bool	m_mouseIN;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);
	//CArray	<class _button*, class _button* >	m_arButton[6];
public:
#ifdef DF_USESTL_DBAR2
	CArray	<std::shared_ptr<class _button>, std::shared_ptr<class _button>>	m_arButton[6];
#else
	CArray	<class _button*, class _button* >	m_arButton[6];
#endif

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
#ifdef DF_USESTL_DBAR2
	void	get_list(CArray <class _button*, class _button* >& ary) { 
		for (int ii = 0; ii < m_vsN; ii++)
		{
			if (!m_arButton[ii].IsEmpty())
				ary.Add(m_arButton->GetAt(ii).get());
		}
	}
#else 
	void	get_list(CArray <class _button*, class _button* >& ary) { ary.Copy(m_arButton[m_vsN]); }
#endif
	
	int	getActiveKey();
	bool	ExceptMap(_button* button);
	void	SetMDIMode(BOOL bMDI);
	int		GetSelectItem(CPoint pt);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDbar2)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetRunningStop(int nKind, BOOL bRunning);
	void SetUseStop(int nKind, BOOL bUse);
	LRESULT OnTipLink(WPARAM wParam, LPARAM lParam);
	void ShowInformation();
	void ImageToggle();
	void RepositionCtrls();
	void DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight);
	virtual ~CDbar2();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	void change_Key(int OldKey, int Key);

	// Generated message map functions
protected:
	CRect	GetButtonRect(int index = 0);
	CRect	get_GripperRect();
	void	draw_Back(CDC* pDC);
	void	draw_Gripper(CDC* pDC);
	void	draw_Button(CDC* pDC = NULL);
	void	draw_Img(CDC* pDC, CRect iRc, bool bActive = false);
	void	draw_Expand(CDC* pDC = NULL, int mode = MODE_COOL);
	void	move_Band();
	_button* GetActiveButton();

	void	Inactive_Button(int vsN = -1);
	void	SetInfo(CPoint);
	void	InitToolTip();
	void	SetToolTipText(CString text, BOOL bActivate = TRUE);
	void	draw_rect(CDC *pDC, CRect iRc,bool bSide);

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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg	LONG OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	// cej
	CRect	getCloseRect(CRect bRc);
	void	drawCloseButton(CDC *pDC, CRect xRc, _button *button);
	void	changeOver(CPoint pt);
	void	drawItem(CDC* pDC, int idx, int mode);

	CRect		m_xRc;
	_button*	m_xBtn;
	_button*	m_deleteBtn;
	int		m_menuID;
	enum { MS_NOT = 0, MS_OVER, MS_DOWN }; 

//** macho add begin
private:
	//CNButton* m_skin;
	CNButton* m_closeAll;
	CNButton* m_moveBar;

	class CSkinDlg*	m_dlg;
	CNButton* CreateNButton(unsigned int id, const char* title, const char* button, BOOL eachButton = TRUE);

	CTerminatorList m_tlist;
	BOOL DrawMaskedBitmap(CDC* pDC, int x, int y, CBitmap* bitmap, int maskcolor, int width, int height);

	CSize m_expandSize;
	BOOL  m_expanded;
	bool	m_pushON;
public:
	CString GetCurrentSkinName();
	void change_Palette(CString skinN);

	void CloseAllChildFrames();
	CRect GetScrButtonPos(int key);

	int GetHeight();
	void MoveBar(int mb);


//** macho add end
};

#define MB_TOP		0
#define MB_BOTTOM	1

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBAR2_H__782CEE65_A03D_4ECE_81AF_A7E71341D9D3__INCLUDED_)
