
// ChildFrm.h : CChildFrame 클래스의 인터페이스
//

#pragma once

#include "ChildView.h"
#include "h/mapform.h"

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame()	{}
	CChildFrame(class CMainFrame* frame, CString curdir);

// 특성입니다.
public:
	class	mapForm		m_mapH;
	CMainFrame*		m_mainFrm;
	CChildView	m_view;
	int		m_CloneCnt;
	class CfmOrder* m_OrderWnd;

protected:
	int	m_display;
	class	CAxisDraw*	m_draw;
	class	CAxisPalette*	m_palette;
	class	CiForm*		m_piForm;
	CString	m_dir;
	bool	m_delete;
	CStringArray	m_bookmarks;
	CStringArray	m_undos;
	CStringArray	m_redos;
// 작업입니다.
public:
	BOOL	Create(CString wName, CRect rect, CAxisDraw* draw, CAxisPalette* palette, CiForm* piForm, int display);

	void	SetFontSize(int point, WORD style = fsNORMAL, CString name = _T(""));
	void	SetVisible(bool showFORM);
	void	SetNew(bool newWindow);

	void	OnDrop(int kind, CPoint pt);
	void	OnTRLayOutDrop(int kind, CPoint pt);

	bool	Copy();
	void	Paste();
	void	Cut();
	bool	MultiCopy();
	BOOL	IsCopyEnable();
	BOOL	IsEditEnable(UINT cmdID);
	void	DrawForm(UINT cmdID);
	void	AlignForms(UINT cmdID);
	int	EditForm(UINT cmdID);
	bool	DeleteFiles();

	CString	GetBookmarkSrc(int idx);
	int	GetBookmarkCount();
	void	SetBookmark(CString src);
	void	SetSelectForm(int idx, bool bMulti = false);
	int	GetSelectedForm();
	bool	IsLayoutMode();

	void	ReloadColors();
	void	DeleteUndoMap();
	void	DeleteSelForms();
	void	LoadProcedures();
	void	ChangeKind();
	void	SaveProcedures();

	BOOL	IsRedoEnable();
	BOOL	IsUndoEnable();
	CString	PopRedo(bool isAll = false);
	CString	PopUndo();
	void	PushRedo(CString src);
	void	PushUndo(CString src);
	void	ReleaseFocus();

	void	OnColumnEdit();

	void	DrawOrderNumber(DWORD dwOrderKind);
	void	ClearOrderNumber();
	bool	CheckOrder(CPoint point);

// 재정의입니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 생성된 메시지 맵 함수
protected:
	void	onActivate();
	void	getGroupNum(CformProp* Group, CArray <int, int> &arr);
	CRect	getFormRect(int idx);
	CRect	groupRect(CformProp* group);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
};
