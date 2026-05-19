
#pragma once

#include "h/mapvar.h"

// WM_USER의 세부 메시지
#define	getCOLOR	0x01
#define	getROOT		0x02

//class CChildView : public CView
class CChildView : public CWnd
{
//protected: // serialization에서만 만들어집니다.
public:
	CChildView();
	DECLARE_DYNCREATE(CChildView)

// 특성입니다.
public:
	struct	_mapH*	m_mapH;
	struct	_block*	m_block;

	int	m_selF;
	int	m_stdF;
	int	m_nTabOdr;
	int	m_nSeqTabOdr;
	int	m_nIoOdr;
	bool	m_move;
	bool	m_bSelect;
	bool	m_bLBtnDown;
	bool	m_bRedraw;
	bool	m_bFirstDrawing;
	bool	m_bEmulRun;
	bool	m_bLayoutMode;
	bool	m_bLine;
	int	m_display;

	CWnd*	m_pEmulateWnd;
	class	CVBScriptEdit*	m_pSCEdit;

protected:
	int	m_nGap;
	int	m_nSeqIoOdr;
	bool	m_showFORM;
	bool	m_new;
//	CWnd*	m_pChildFrame;

	int	m_emulateKey;

	class	CFormItem*	m_formItem;
	class	CAmPalette*	m_palette;
	class	CAmDraw*	m_draw;

	CObArray	m_toolOBs;
	class	CTlBase* m_tool;
	bool	m_drawing;
	bool	m_selected;
	CMap	<int, int, class CShowObject*, CShowObject*>	m_forms;
	CStringArray m_arLayout;
	struct	_Layout* m_pOriLayout;
	int	m_nCursor;
	CBitmap*	m_pBitmap;
	CPoint	m_dragPnt, m_RBDPnt;
	int	m_Previewkey;

	CString	m_sImgPath;	// 배경이미지 경로
	HBITMAP	m_hImgBack;	// 배경이미지 HBITMAP

	CString m_sPreLayoutName;

private:
	CString	m_param;

protected:


// 구현입니다.
public:
	virtual ~CChildView();

	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void	Init(struct _mapH* mapH, CAmDraw* draw, CAmPalette* palette, struct _block* block, CString dir);
	void	DrawForm(int cmdID, CString param);
	void	AlignForms(int cmdID);
	void	RedrawView();
	void	ChangeKind();
	void	SortIoOrder();
	void	SortTabOrder();
	void	DeleteFORMs();
	void	OnDrop(int kind, CPoint pt);
	void	Emulate();
	CString	GetInitSymbol(CString sSymbolList = _T(""));
	CString	GetControlName(int kind, int type);
	void	SetLine(int nLine);
	void	InvalidateRect2(int idx);
	void	SetGap(int nGap);
	void	SetNew(bool newW)	{ m_new = newW; }
	void	ShowForm(bool show);
	COLORREF GetRGB(int color);
	void	DefaultLayout();

protected:
	char*	getRoot();
	int	getDefaultValue(CString sSection, CString sKey, int nDefault);
	bool	isValidTabOrder(int idx);
	bool	isValidIOOrder(int idx);
	void	loadAutoList();
	void	extMenu();
	void	columnFit();
	CString	getItemSymbol(CString sSymbol);
	void	selectColumn();
	void	addCol();
	void	addPage(int nPage);
	void	addItem();
	int	deleteForm(int idx);
	void	moveForm(int idx, int mx, int my, bool bSize = false);
	CRect	moveForm(int mx, int my, bool bSize = false);
	CRect	sizeForm(int mx, int my);
	void	setBlockForm(int value, int type);
	void	setBlockGroup(int nGroup, int mx, int my, bool bSize = false);
	CRect	groupRect(struct _formR* nGroup);
	void	setObject(int index);
	void	drawBackground(CDC *pDC);
	void	drawBitmap(CDC *pDC);
	void	drawMap(CDC *pDC);
	bool	drawImage(CDC *pDC);
	CString	parse(CString &srcstr, CString substr);
	void	deleteSelPage(int idx);
	void	redrawSelChange(int oldF, int selF);
	CRect	getFormRect(int idx);
	void	spaceEqual(int std, long pos, long maxV, int count, bool horizontal = true);
	CRect	getRect(int fN, int flag = 0);
	void	keyArrowLeft();
	void	keyArrowRight();
	void	keyArrowUp();
	void	keyArrowDown();
	void	setGroup();
	void	releaseGroup();
	int	getTableForm(CPoint point);
	bool	movable(CPoint point);
	void	addColumn(bool is_default, int grid, class Csymbol* symbol);
	void	addForm(bool is_default, Csymbol* symbol, CPoint pt, struct _formR *tabN = NULL, int idx = 0);
	void	showMenu();
	bool	selectable(CPoint point);
	void	sendToFirst();
	void	sendToEnd();	
	void	SetDefaultLayout();
	bool	IsLayoutMode();
	

// 생성된 메시지 맵 함수
protected:

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);

	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewCommand(UINT cmdID);
	afx_msg void OnLayoutCommand(UINT cmdID);
	afx_msg void OnLayoutModeCommand(UINT cmdID);	

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnNcHitTest(CPoint point);
};


