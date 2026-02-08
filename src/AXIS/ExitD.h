#if !defined(AFX_EXITD_H__85031079_33CA_442C_BE78_899DCD325805__INCLUDED_)
#define AFX_EXITD_H__85031079_33CA_442C_BE78_899DCD325805__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExitD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExitD dialog
#include "BmpButton.h"
#include "ShapeButton.h"
#define MAX_SHAPE_EX	8

class CExitD : public CDialog
{
// Construction
public:
	void RotateBanner();
	int m_currEvent{};
	void SetEventBitmap(int index);
	CExitD(CWnd* pParent = NULL, BOOL bNew = FALSE);   // standard constructor
	~CExitD();

	void	SetTime(SYSTEMTIME time) { m_stime = time; };
// Dialog Data
	//{{AFX_DATA(CExitD)
	enum { IDD = IDD_DT_EXIT, IDD_NEW = IDD_NEW_DT_EXIT };
	CStatic		m_static;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExitD)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL	m_bNew;
	CFont	m_font, m_fontBold;

	CBitmap* m_bitmap;
	CStatic* m_bmpEvent;
	
	HBITMAP m_bitmapEvent;
	CBmpButton* m_btnOK;
	CBmpButton* m_btnCancel;
	CBmpButton* CreateBmpButton(UINT buttonID, UINT buttonBitmap);

	CShapeButton* m_shapeButtons[MAX_SHAPE_EX]{};
	CShapeButton* CreateShapeButton(UINT buttonID, const char* bmpFileName, CRect rc);
	void		  ReleaseShapeButton();
	void		  OnShapeButton(int index);

	SYSTEMTIME	m_stime{};
	SYSTEMTIME	m_etime{};

	SYSTEMTIME	GetUseTime();

	BOOL	m_bUseNewLogin;
	//void		visibleButton();

	// Generated message map functions
	//{{AFX_MSG(CExitD)
	virtual BOOL OnInitDialog();
	virtual void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXITD_H__85031079_33CA_442C_BE78_899DCD325805__INCLUDED_)
