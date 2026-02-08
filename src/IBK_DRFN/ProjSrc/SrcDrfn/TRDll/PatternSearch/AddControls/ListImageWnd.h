#if !defined(AFX_LISTIMAGEWND_H__D062C2BB_8FB7_416F_884F_44650337A7A5__INCLUDED_)
#define AFX_LISTIMAGEWND_H__D062C2BB_8FB7_416F_884F_44650337A7A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListImageWnd.h : header file
//
#define IMAGELISTCONTROL_CLASSNAME    _T("MFCIMAGELISTCTRL")  // Window class name
#define DDX_IMGLISTControl(pDX, nIDC, rControl)  DDX_Control(pDX, nIDC, rControl)     

const UINT RMSG_DELETE_ITEM	=	::RegisterWindowMessage(_T("RMSG_DELETE_ITEM"));
const UINT RMSG_GET_TIPTEXT	=	::RegisterWindowMessage(_T("RMSG_GET_TIPTEXT"));
/////////////////////////////////////////////////////////////////////////////
// CListImageWnd window
#include "../../../DrCommon/Dr_Control/DrInfoTip.h"

class CListImageWnd : public CWnd
{

// Construction
public:
	CListImageWnd();
	BOOL Create(CRect rcWindow, CWnd* pParent, DWORD dwStyle, int nID);
	BOOL RegisterWindowClass();

// Attributes
public:
	CImageList m_ImageList;

private:
	CWnd* m_pParent;
	CDrInfoTip m_tipInfo;
	int	m_nLeftMargin;
	int m_nTopMargin;

	int m_nViewWidth;
	int m_nViewHeight;
	int m_nHScrollPos;
	int m_nHPageSize;
	int m_nVScrollPos;
	int m_nVPageSize;

	int m_nImageCountPerRow;
	int m_nImageWidth;
	int m_nImageHeight;
	int m_nXGap;
	int m_nYGap;
	int m_nScreenWidth;
	int m_nScreenHeight;

//	int m_nImageSelected;
	BOOL m_bAutoHorzAlign;
	BOOL m_bVertScroll;
	BOOL m_bUseDeleteMenu;
	BOOL m_bShowToolTip;

	int m_nCurImgIndex;
	CFont* m_fontDefault;

	CString m_strToolTip;

// Operations
public:
	void AddImage(CBitmap bitmap);
	void SetImageSpace(int nX, int nY);
	void SetMargin(int nX, int nY);
	void SetUseDeleteMenu(BOOL bUse);
	void SetImageData(CImageList& imgList, int nImageCntPerRow, int nImageWidth, int nImageHeight);
	CPoint GetImagePos(int nIndex);
	void Refresh();
	void ResetScrollInfo();
	void SetDefaultFont(CFont* pFont){ m_fontDefault = pFont; }
	void CreateTooltip();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListImageWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListImageWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListImageWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	//}}AFX_MSG
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTIMAGEWND_H__D062C2BB_8FB7_416F_884F_44650337A7A5__INCLUDED_)
