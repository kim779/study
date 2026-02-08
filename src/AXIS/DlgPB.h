#if !defined(AFX_DLGPB_H__44321150_7590_4D9C_B2B1_F76767918F28__INCLUDED_)
#define AFX_DLGPB_H__44321150_7590_4D9C_B2B1_F76767918F28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPB.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPB dialog
#include "MainFrm.h"

class CDlgPB : public CDialog
{
// Construction
public:
	CDlgPB(CWnd* pParent = NULL);   // standard constructor
	CMainFrame* m_pMain;
// Dialog Data
	//{{AFX_DATA(CDlgPB)
	enum { IDD = IDD_DLGPB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPB)

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPB)
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:

	void	OnDraw(CDC* pDC);
	void	DrawBack(CDC* pDC);
	void	DrawFrame(CDC* pDC);
	void	DrawData(CDC* pDC);

	int	m_width;
	int	m_height;
	int	m_nRowHeight;

	bool  m_bshow{};

	CRect	m_baseRc;
	CStringArray	m_arData;
	CRect	m_rect;	
	CFont	m_font;

public:
	void	ChangePalette();
	void	ChangeData();
	void	ShowPBSlide(CRect);
	void	HideSlide();
	void	SetData(CString dat, int kind);
	void	SetDataColor(CString dat, COLORREF clrHeader, COLORREF clrCell);
	void	StopSlide();	
	void	GetSize(int& width, int& height) { width = m_width, height = m_height; }
protected:
	void	Slide();
private:
	CBitmap*	m_bmp;
	CSize		m_bmpSize;
	BOOL		m_stretch;
	void DrawBitmap(CDC* pDC);
	void DrawTitle(CDC* pDC);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPB_H__44321150_7590_4D9C_B2B1_F76767918F28__INCLUDED_)
