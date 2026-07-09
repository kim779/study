// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__428255AA_45FC_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_CHILDFRM_H__428255AA_45FC_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"
#include "mapform.h"
#include "awCommon/FormItem.h"

#include "PopFrame.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame()	{}
	CChildFrame(class CMainFrame* frame, class CAxisDraw* draw, CString dir, struct _block* block);

// Attributes
public:
	struct	_mapH		m_mapH;
	CFormItem		*m_pFormItem;
	CMainFrame*	m_mainFRM;
	CChildView	m_view;		// view for the client area of the frame.
	long		m_myKey;
	struct	_block*	m_block;

	BOOL		m_bSDI;

	CPopFrame	*m_pPopFrame;

	int		*m_pDisplayMode;
protected:
	CWnd		*m_pVirtualWnd;

// Operations
public:
	void	SetFontSize(int point, WORD style = FS_NORM, CString name = _T(""));
	void	SetPalette(class CAxisPalette* palette);
	void	SetVisible(bool showFORM);
	void	SetNew(bool newWindow);

	void	OnDrop(CPoint pt);

	bool	Copy();
	void	Paste();
	BOOL	IsCopyEnable();
	BOOL	IsEditEnable(UINT cmdID);
	void	DrawForm(UINT cmdID);
	void	AlignForms(UINT cmdID);
	int	EditForm(UINT cmdID);
	bool	DeleteFiles();

	CString	getBookmarkSrc(int idx);
	int	getBMCount();
	void	setBookMark(CString src);
	void	setSelectForm(int idx, bool bMulti = false);
	int	getSelectedForm();

protected:
	void	getGroupNum(int nGroup, CArray <int, int> &arr);
	void	getTabNum(struct _formR *tabN, CArray <int, int> &arr);
	char*	CopyCell(char* idx);
	void	CopyForm(_formR *targetR, _formR *sourceR);
	CRect	getFormRect(int idx);
	CRect	GroupRect(struct _formR* nGroup);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Attributes
protected:
	CString	m_dir;
	bool	m_delete;
	CAxisDraw*	m_draw;
	CStringArray	m_bookmarkArr, m_undoArr, m_redoArr;

// Implementation
public:
	void OnActivateMe();
	void ReloadColors();
	void FreePopFrame();
	void DeleteUndoMap();
	void DeleteSelForms();
	void LoadProcedures();
	void changeKind();
	void SaveProcedures();
	bool canRedo();
	bool canUndo();
	CString	popRedo(bool isAll = false);
	CString	popUndo();
	void	pushRedo(CString src);
	void	pushUndo(CString src);
	void	Cut();
	void	killFocus();
	virtual ~CChildFrame();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnClose();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg LRESULT HandleSetText(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__428255AA_45FC_11D4_A024_00001CD7F9BE__INCLUDED_)
