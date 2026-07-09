// ChildView.h : interface of the CChildView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDVIEW_H__428255AC_45FC_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_CHILDVIEW_H__428255AC_45FC_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChildView window

#include <afxtempl.h>
#include "awCommon/FormItem.h"
#include "Components/crysedit_src/VBScriptEdit.h"

class CShow;
class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
	struct	_mapH*		m_mapH;
	CFormItem		*m_pFormItem;
	struct	_block*		m_block;

	class	CAxisPalette*	m_palette;
	class	CAxisDraw*	m_draw;
	int	m_selF, m_stdF;
	int	m_nGap, m_nTabOdr, m_nSeqTabOdr;
	int	m_nIoOdr, m_nSeqIoOdr;
	bool	m_showFORM;
	bool	m_new;
	bool	m_move, m_bSelect, m_bLBtnDown, m_bRedraw, m_bFirstDrawing;
	CString m_root, m_rtnStr;
	CVBScriptEdit	*m_pSCEdit;

	CWnd		*m_pPreView;
	CWnd		*m_pChildFrame;

	int		*m_pDisplayMode;

protected:
	CObArray	m_toolOBs;
	class	CTOOL*	m_tool;
	bool		m_drawing, m_selected;
	CString		m_dir;
	CMap	<int, int, CShow*, CShow*>	m_forms;
	int		m_nCursor;
	CBitmap		*m_pBitmap;
	CPoint		m_dragPnt, m_RBDPnt;
	int		m_Previewkey;


	// new added

	CString	m_sImgPath;	// 배경이미지 경로
	HBITMAP	m_hImgBack;	// 배경이미지 HBITMAP
	int	m_szWidth;	// 배경이미지 폭
	int	m_szHeight;	// 배경이미지 높이
	bool	drawImage(CDC *pDC);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	int getDefaultValue(CString sSection, CString sKey, int nDefault);
	bool isValidTabOrder(int idx);
	bool isValidIOOrder(int idx);
	void setLine(int nLine);
	void RedrawView();
	long OnMessage(WPARAM wParam, LPARAM lParam);
	void changeKind();
	void LoadAutoList();
	void OnExtMenu();
	void OnColumnFit();
	CString getItemSymbol(CString sSymbol);
	void OnSelectColumn();
	void	addCol();
	void	addPage(int nPage);
	void	OnAddItem();
	void	MoveForm(int idx, int mx, int my, bool bSize = false);
	void	SetBlockForm(int value, int type);
	void	sortIoOrder();
	void	sortTabOrder();
	CRect	SizeForm(int mx, int my);
	CRect	MoveForm(int mx, int my, bool bSize = false);
	void	setBlockTab(struct _formR *tabN, int mx, int my, bool bSize = false);
	void	setBlockGroup(int nGroup, int mx, int my, bool bSize = false);
	bool	IsInPage(int idx);
	void	drawBackground(CDC *pDC);
	void	drawBitmap(CDC *pDC);
	void	drawMap(CDC *pDC);
	void	Preview();
	bool	isInTab(CPoint pnt, int &formN, int &selectedN);
	CString	getControlName(int kind, int type);
	void	InvalidateRect2(int idx);
	void	SetGap(int nGap);
	void	deleteFORMs();
	int	deleteForm(int idx);
	CRect	GroupRect(struct _formR* nGroup);
	void	Init(struct _mapH* mapH, struct _block* block, CString dir);
	void	DrawForm(int cmdID);
	void	AlignForms(int cmdID);
	CString	getInitSymbol(CString sSymbolList = "");
	void	SetObject(int index);
	void	OnDrop(CPoint pt);

	virtual ~CChildView();

	// Generated message map functions
protected:
	CString	Parser(CString &srcstr, CString substr);
	void	deleteSelPage(int idx);
	bool	setTabRgn(CDC *pDC, int idx, int nMode);
	void	reDrawSelChange(int oldF, int selF);
	CRect	getFormRect(int idx);
	void	OnDisGrouping();
	void	OnGroupMake();
	void	spaceEqual(int std, long pos, long maxV, int count, bool horizontal = true);
	CRect	getRect(int fN, int flag = 0);
	void	keyArrowLeft();
	void	keyArrowRight();
	void	keyArrowUp();
	void	keyArrowDown();
	void	showScripts();
	int	getTableForm(CPoint point);
	bool	movable(CPoint point);
	bool	optimumSize(CPoint point);
	void	addColumn(int grid, class Csymbol* symbol);
	void	addForm(Csymbol* symbol, CPoint pt, struct _formR *tabN = NULL, int idx = 0);
	void	ShowMenu();
	bool	selectable(CPoint point);
	void	OnRsendtofirst();
	void	OnRsendtoend();

	//{{AFX_MSG(CChildView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg void OnViewCommand(UINT cmdID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDVIEW_H__428255AC_45FC_11D4_A024_00001CD7F9BE__INCLUDED_)
