
#pragma once

#include "h/mapvar.h"
//#include "../dll/Object/fmObject.h"

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
	class	mapForm*	m_mapH;

	int	m_selF;
	int	m_stdF;
	int	m_nSeqTabOdr;
	int	m_nSeqIoOdr;
	int	m_nIoOdr;
	bool	m_move;
	bool	m_bSelect;
	bool	m_bLBtnDown;
	bool	m_bRedraw;
	bool	m_bFirstDrawing;
	bool	m_bLayoutMode;
	bool	m_bBKLine;
	int	m_iBKLineIdx;

	int	m_display;

	CWnd*	m_pEmulateWnd;
	class	CVBScriptEdit*	m_pSCEdit;
	//CObArray		m_formObArry;
	int	m_nCursor;
	class	CFmCtrl* m_FmCtrl;
	class	CFmAlignBar* m_FmAlignBar;
	class	CiForm*		m_piForm;
	CObArray m_ArryFormObj;
	CArray<CRect, CRect> m_aryBKLine;
	CRect m_LineTextRC;

protected:
	int	m_nGap;
	bool	m_showFORM;
	bool	m_new;
//	CWnd*	m_pChildFrame;

	int	m_emulateKey;

	class	CAxisPalette*	m_palette;
	class	CAxisDraw*	m_draw;

	//CObArray	m_toolOBs;
	//class	CTlBase* m_tool;
	
	CString m_selTool;
	bool	m_drawing;
	bool	m_selected;
	
	CBitmap*	m_pBitmap;
	CPoint	m_dragPnt, m_RBDPnt;
	int	m_Previewkey;

	CString	m_sImgPath;	// 배경이미지 경로
	HBITMAP	m_hImgBack;	// 배경이미지 HBITMAP

	CStringArray m_arLayout;
	struct	_Layout* m_pOriLayout;

	CString m_sPreLayoutName;

protected:

// 구현입니다.
public:
	virtual ~CChildView();
;
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void	Init(class mapForm* mapForm, CAxisDraw* draw, CAxisPalette* palette, CiForm* piForm, CString dir);
	void	DrawForm(int cmdID);
	void	AlignForms(int cmdID);
	void	RedrawView();
	void	ChangeKind();
	void	SortIoOrder();
	void	ObjectRemove();
	void	ObejctRemoveAll();
	void	OnDrop(int kind, CPoint pt);
	void	OnTRLayoutDrop(int kind, CPoint pt);
	void	Emulate();
	CString	GetInitSymbol(CString sSymbolList = _T(""), char kind = NULL);
	CString	GetControlName(int kind, int type);
	bool	IsInTab(CPoint pnt, int &formN, int &selectedN);
	void	SetLine(int nLine);
	void	InvalidateRect2(int idx, int iFormLoad = 0);
	void	ShowVScroll(class CformProp* Prop);
	void	SetGap(int nGap);
	void	SetNew(bool newW)	{ m_new = newW; }
	void	ShowForm(bool show);
	COLORREF GetRGB(int color);
	int	GetHitCursor();
	void	SelObj(int idx, bool bClear = true);
	void	SelAddObj(int idx, bool bMulti);
	bool	isValidTabOrder(class CformProp * Prop);
	bool	isValidIOOrder(class CformProp* Prop);
	int		deleteForm(int idx);
	int		removeForm(int idx);
	void	swapPage(int idx, int first, int second);
	void	swapCell(int idx, int first, int second);
	void	cellPropCopy(class cellOb* dstProp, class cellOb* srcProp);
	void	UnSelBKLine();
	void	SetLayoutInfo(CString sKey, CString sPreKey);
	CString	AddObject(CString strInOut, CString strName, CPoint *pt, int iGap, CString strData = "");
	CString	Parser(CString &srcstr, CString substr);
	CString	getInitSymbol(CformProp* form, CString syms);
	void	SetOriginalLayout();
protected:
	char*	getRoot();
	int	getDefaultValue(CString sSection, CString sKey, int nDefault);
	void	loadAutoList();
	void	extMenu();
	void	columnFit();
	CString	getItemSymbol(CString sSymbol);
	void	selectColumn();
	void	addCol();
	void	addPage(int nPage);
	void	addItem();
	bool	isInPage(int idx);
	void	drawBackground(CDC *pDC);
	void	drawBitmap(CDC *pDC);
	void	drawMap(CDC *pDC);
	bool	drawImage(CDC *pDC);
	CString	parse(CString &srcstr, CString substr);
	void	deleteSelPage(int idx);
	CRect	getFormRect(int idx);
	void	keyArrowLeft();
	void	keyArrowRight();
	void	keyArrowUp();
	void	keyArrowDown();
	void	setGroup();
	void	releaseGroup();
	int	getTableForm(CPoint point);
	void	addColumn(bool is_default, int grid, class Csymbol* symbol);
	void	addForm(bool is_default, Csymbol* symbol, CPoint pt, CformProp* Prop = NULL, int idx = 0);
	void	showMenu();
	void	sendToFirst();
	void	sendToEnd();
	void	delBKLine();
	void	moveBKLine(DWORD arrow);
	void	UpdateLineText();
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnObjMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewCommand(UINT cmdID);
	afx_msg LRESULT OnTransfer(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLayoutCommand(UINT cmdID);
	afx_msg void OnLayoutModeCommand(UINT cmdID);

	DECLARE_MESSAGE_MAP()
};


