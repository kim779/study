
// ChildFrm.h : CChildFrame 클래스의 인터페이스
//

#pragma once

#include "ChildView.h"
#include "h/mapform.h"
#include "amCom/formItem.h"

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame()	{}
	CChildFrame(class CMainFrame* frame, CString curdir, struct _block* block);

// 특성입니다.
public:
	struct	_mapH		m_mapH;
	class	CFormItem*	m_formItem;
	CMainFrame*		m_mainFrm;
	CChildView		m_view;
	struct	_block*		m_block;

	bool			m_bEmulRun;
	bool			m_bSubChild;

protected:
	int	m_display;
	class	CAmDraw*	m_draw;
	class	CAmPalette*	m_palette;
	CString	m_dir;
	bool	m_delete;
	CStringArray	m_bookmarks;
// updateXX__2014
	CPtrArray	m_undos;
	CPtrArray	m_redos;
//	CStringArray	m_undos;
//	CStringArray	m_redos;

// 작업입니다.
public:
	BOOL	Create(CString wName, CRect rect, CAmDraw* draw, CAmPalette* palette, int display);

	void	SetFontSize(int point, WORD style = FS_NORM, CString name = _T(""));
	void	SetVisible(bool showFORM);
	void	SetNew(bool newWindow);

	void	OnDrop(int kind, CPoint pt);

	bool	Copy();
	void	Paste();
	void	Cut();
	BOOL	IsCopyEnable();
	BOOL	IsEditEnable(UINT cmdID);
	void	DrawForm(UINT cmdID, CString param);
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

	BOOL	PopRedo(bool isAll = false);
	BOOL	PopUndo(bool bflag = true);
	void	PushRedo(class CUndoRedo* urdo = NULL);
	void	PushUndo(class CUndoRedo* urdo = NULL);
	void	ReleaseFocus();
	void	OnColumnEdit();

	


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
	void	getGroupNum(int nGroup, CArray <int, int> &arr);
	char*	copyCell(char* idx);
	void	copyForm(_formR *targetR, _formR *sourceR);
	CRect	getFormRect(int idx);
	CRect	groupRect(struct _formR* nGroup);

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
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
