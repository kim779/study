#pragma once
// TooltipDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTooltipDlg dialog
#include "MyBitmap.h"
class CTooltipDlg : public CDialog
{
// Construction
public:
	CTooltipDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTooltipDlg)
	enum { IDD = IDD_DLG_TOOLTIP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTooltipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTooltipDlg)
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CRect	m_rect;
	CRect	m_rcBitmap;
	CBrush	m_brush;
	CBrush	m_brushBack;
	CPen	m_pen;
	CRgn	m_rgn;
	CRgn	m_rgnDraw;
	CMyBitmap	m_bitmap;

};

