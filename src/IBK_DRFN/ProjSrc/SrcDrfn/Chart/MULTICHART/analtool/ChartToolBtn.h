#if !defined(AFX_TOOLBTN_H__35B2421C_A9B0_11D4_BA6F_0080C7358274__INCLUDED_)
#define AFX_TOOLBTN_H__35B2421C_A9B0_11D4_BA6F_0080C7358274__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChartToolBtn window
#include "../../chart_common/BlockBaseEnum.h"

class CChartToolBtn : public CWnd
{
// Construction
public:
	CChartToolBtn();

// Attributes
public:
	CPoint	CreateBtn(CWnd *pParent, CFont *pFont, CPoint ptStart, int nID);
	BOOL	CreateBtn(CWnd *pParent, CFont *pFont, int nID);
	CPoint	Replace(CPoint ptStart);
	CPoint	ReplaceSize(CPoint ptStart, BOOL bShow=TRUE);
	void	CalcRect(CPoint ptStart, CString strTitle, CRect &rect);
	void	DrawGradation(CDC *pdc);
	void	DrawBMP(CDC *pdc);
	void	DrawRound(CDC *pdc);
	void	DrawBackBmp(CDC *pdc);
	void	DrawBackImageList(CDC *pdc);

	// 2002. 8. 30 Hedges
	void    SetValueBtn(CToolOptionInfo::TOOLOPTION tooltype, BOOL bToggle, CBitmap *pNormalBmp, CBitmap *pUpBmp, CBitmap *pDownBmp);
	// end
	void	SetValueBtn(int nAnalyType, int nMessage, BOOL bToggle, CSize *sz = NULL, CSize *szBackBmp = NULL, 
						CBitmap *pNormalBmp = NULL,	CBitmap *pUpBmp = NULL, CBitmap *pDownBmp = NULL);

	void	DeleteBtn();
	int		GetHeight();
	void	CalcText(CPoint *pt, char *cTitle);
	void	SetToggle(BOOL bToggle, int nCurToggle);
	int		GetToggle();
	void	ReleaseBtn();

	void	SetValueBtn(CToolOptionInfo::TOOLOPTION tooltype, BOOL bToggle, CImageList* pImageList, int nNormalIndex, int nUpIndex, int nDownIndex);
	void	SetToggleBtnFromOutside();

// Operations
public:
	// 2002. 8. 30 Hedges
	CToolOptionInfo::TOOLOPTION		m_ToolOption;
	// end
	CWnd	   *m_pParent;
	CString 	m_strTitle;
	CString		m_strTR;
	CString		m_strTrName;
	CPoint		m_ptStart;
	CPoint		m_ptEnd;
	int			m_nID;
	int			m_nFHeight;
	int			m_nFWidth;

	int			m_nBtnWidth;
	int			m_nBtnHeigth;
	int			m_nBeState;

	BOOL		m_bFixedSz;
	CSize		m_FixedSz;
	CSize		m_BmpSz;
	CBitmap		*m_pBmp;

	CRect		m_rcRect;
	int			m_nBtDown;
	BOOL		m_bDowned;
	CFont	   *m_pFont;
	BOOL		m_bMenuBtn;
	CRect		m_rect;

	CSize		m_BackBmpSz;
	CBitmap	   *m_pUpBmp;
	CBitmap	   *m_pDownBmp;
	CBitmap    *m_pNormalBmp;
	int		    m_nCurrToggle;
	BOOL	    m_bToggle;
	int		    m_nAnalyType;
	int			m_nMessage;

	int		m_nUp;
	int		m_nDown;
	int		m_nNormal;
	CImageList	*m_pImageList;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartToolBtn)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChartToolBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChartToolBtn)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBTN_H__35B2421C_A9B0_11D4_BA6F_0080C7358274__INCLUDED_)
