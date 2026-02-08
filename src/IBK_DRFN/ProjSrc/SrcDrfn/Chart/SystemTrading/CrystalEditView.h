////////////////////////////////////////////////////////////////////////////
//	Interface of the CCrystalEditView class, a part of Crystal Edit - syntax
//	coloring text editor.
//
//	You are free to use or modify this code to the following restrictions:
//	- Acknowledge me somewhere in your about box, simple "Parts of code by.."
//	will be enough. If you can't (or don't want to), contact me personally.
//	- LEAVE THIS HEADER INTACT
////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYSTALEDITVIEW_H__0A901F02_2C2F_11D5_975A_00D0B71DFCCD__INCLUDED_)
#define AFX_CRYSTALEDITVIEW_H__0A901F02_2C2F_11D5_975A_00D0B71DFCCD__INCLUDED_

#include "CEDef.h"
#include "CrystalTextView.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEditDropTargetImpl;

/////////////////////////////////////////////////////////////////////////////
// CCrystalEditView view

class CRYSEDIT_CLASS_DECL CCrystalEditView : public CCrystalTextView
{
protected:
	CCrystalEditView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCrystalEditView)

// Attributes
private:
	BOOL	m_bOvrMode;
	BOOL	m_bDropPosVisible;
	CPoint	m_ptSavedCaretPos;
	CPoint	m_ptDropPos;
	BOOL	m_bSelectionPushed;
	CPoint	m_ptSavedSelStart, m_ptSavedSelEnd;
	BOOL	m_bAutoIndent;
	BOOL	m_bDisableBSAtSOL;						// Disable BS At Start Of Line

protected:
	CEditDropTargetImpl *m_pDropTarget;

public:

// Operations
public:
	BOOL	GetAutoIndent() const; 
	void	SetAutoIndent(BOOL bAutoIndent);
	void	SetDisableBSAtSOL(BOOL bDisableBSAtSOL);
	BOOL	GetDisableBSAtSOL() const;
	BOOL	GetOverwriteMode() const;
	void	SetOverwriteMode(BOOL bOvrMode = TRUE);
	void	ShowDropIndicator(const CPoint &point);
	void	HideDropIndicator();
	BOOL	DoDropText(COleDataObject *pDataObject, const CPoint &ptClient);
	void	DoDragScroll(const CPoint &point);
	BOOL	ReplaceSelection(LPCTSTR pszNewText);

	virtual BOOL QueryEditable();
	virtual void UpdateView(CCrystalTextView *pSource, CUpdateContext *pContext, DWORD dwFlags, int nLineIndex = -1);
	virtual void OnEditOperation(int nAction, LPCTSTR pszText);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrystalEditView)
	//}}AFX_VIRTUAL

private:
	BOOL	DeleteCurrentSelection();
	
protected:
	virtual DROPEFFECT GetDropEffect();
	virtual void OnDropSource(DROPEFFECT de);
	virtual void ResetView();
	void	Paste();
	void	Cut();

// Implementation
protected:
	virtual ~CCrystalEditView();

// Generated message map functions
protected:
	//{{AFX_MSG(CCrystalEditView)
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditDelete();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditDeleteBack();
	afx_msg void OnEditUntab();
	afx_msg void OnEditTab();
	afx_msg void OnEditSwitchOvrmode();
	afx_msg void OnUpdateEditSwitchOvrmode(CCmdUI* pCmdUI);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnEditReplace();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditRedo();
	//}}AFX_MSG
	afx_msg void OnUpdateIndicatorCol(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorOvr(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorRead(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	// LeeYong, 2001.6.29
	virtual void	DisplayIMECompStr(LPSTR lpCompStr, LPSTR lpCompAttr);
	virtual void	DisplayIMERestStr(LPSTR lpRestStr);
};

#if !(defined(CE_FROM_DLL) || defined(CE_DLL_BUILD))
#include "CrystalEditView.inl"
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRYSTALEDITVIEW_H__0A901F02_2C2F_11D5_975A_00D0B71DFCCD__INCLUDED_)
