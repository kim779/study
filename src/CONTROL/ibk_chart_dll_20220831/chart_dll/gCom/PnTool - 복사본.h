#if !defined(AFX_PNTOOL_H__4C525F82_2580_4653_A363_CCDD14ED468A__INCLUDED_)
#define AFX_PNTOOL_H__4C525F82_2580_4653_A363_CCDD14ED468A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnTool.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPnTool window

#include "afxtempl.h"
/*
class _toolINFO	
{
public:
	int	btnState;
	CRect	rect;
	int	value;
	CString	text;
};
*/
class CPnTool : public CWnd
{
// Construction
public:
	CPnTool(CWnd *pwndView, CWnd* pwndGView, CFont* pFont, int ctrlID, char* info);

// Attributes
public:

private:
	bool		m_bPopupFailed;
	bool		m_bCapture;
	bool		m_bLDown;
	bool		m_bPattern;

	int		m_iToolSave;
	int		m_iMoveSave;
	int		m_iSelLine;
	int		m_iSelFigure;
	int		m_iSelArrow;
	int		m_iSelGigan;
	int		m_iSelFibo;
	int		m_iSelGann;
	int		m_iSelTri;
	int		m_iSelWidth;
	int		m_iToolCount;


	BOOL		m_bMouseIN;
//	COLORREF	m_rgbBG;

	CAxisGComApp*	m_pApp;
	CWnd*		m_pwndView;
	CWnd*		m_pwndGView;

	CFont*		m_pfont;
	CImageList*	m_pImageList;
	CImageList*	m_pImageList2;
	
	CString		m_strComment;
	CString		m_strMapName;


	class CPnToolTip*	m_pToolTip;

	enum enumPosi { eNoPosi, eLeft, eTop, eRight, eBottom }	m_ePosi;
	enum	{btnNOT = 0, btnMOVE, btnDOWN, btnUP};

	CMap <int, int, class CPnToolInfo*, class CPnToolInfo*> m_mapPnToolInfo;


protected:
	int	Init();
	void	InitTool();
	
	void	DrawTool(CDC *pDC);
	
	void	DeleteAll();
	void	AddControlClass(int iKey, CRect rectTool, int iKind, CString strText, int iIndex);
	int	SearchPointInTool(CPoint pt);
	//void	BtnDraw(CDC* pDC, int selectID, int imgID, _toolINFO* pInfoTOOL, CImageList* pImage);
	void	BtnDraw(CDC* pDC, int selectID, int imgID, class CPnToolInfo* pPnToolInfo, CImageList* pImage);
	void	SetMouseBtn(int toolID, bool bRelease = false);

	CString	GetPosComment(CPoint pt);
	void	mouseLeaveAction(UINT nFlags, CPoint point);


	bool	Load();
	bool	Save();

	void	ToolConfig();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnTool)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnTool();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnTool)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT	OnToolEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void	OnMenuCheck(UINT nID);
	DECLARE_MESSAGE_MAP()


private:
	BOOL	m_patternUP, m_patternDN;
public:
	void	SetPatternUP(BOOL pattern);
	void	SetPatternDN(BOOL pattern);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNTOOL_H__4C525F82_2580_4653_A363_CCDD14ED468A__INCLUDED_)
