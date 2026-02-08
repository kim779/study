#if !defined(AFX_DLGGLASS_H__B6492C82_BDFE_11D6_8777_00E07D8FB0D6__INCLUDED_)
#define AFX_DLGGLASS_H__B6492C82_BDFE_11D6_8777_00E07D8FB0D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGlass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGlass dialog

class CDlgGlass : public CDialog
{
// Construction
public:
	CDlgGlass(CWnd* pParent = NULL);   // standard constructor
	~CDlgGlass();

	void MakeBitmap();
	void OnDraw(CDC* pdc, const CBitmap& bitmap);

// Dialog Data
	//{{AFX_DATA(CDlgGlass)
	enum { IDD = IDD_MAGNIFYINGGLASS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGlass)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGlass)
	afx_msg void OnPaint();
	afx_msg void OnMove(int x, int y);
	virtual BOOL OnInitDialog();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMagnify2();
	afx_msg void OnMagnify5();
	afx_msg void OnMagnify3();
	afx_msg void OnMagnify4();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnClosemagnify();
	afx_msg void OnUpdateMagnify3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMagnify4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMagnify5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMagnify2(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
//	afx_msg UINT OnNcHitTest(CPoint point);
//	afx_msg void OnNcMouseMove( UINT nHitTest, CPoint point );
//	afx_msg void OnNcLButtonDown( UINT nHitTest, CPoint point);
//	afx_msg void OnNcLButtonUp( UINT nHitTest, CPoint point);
	DECLARE_MESSAGE_MAP()
private:
	int m_nCount;
	bool	m_bAfterInit;
	CPoint	m_ptStart;
	CRect	m_rctPos;
	SYSTEMTIME	m_stMouseTime;

	CWnd* m_pParent;
	CWnd * m_wndDesk;		// 바탕화면
	CDC m_memDC;			// 메모리 DC
	CBitmap m_Bitmap;	// 메모리 비트맵
	CBitmap* m_pOldBitmap;

	CPoint m_pt;
	CRect m_rect;
	CPoint m_ptScreen;		// SCREEN의 크기
	CPoint m_ptMove;		// 윈도우가 옮겨진 좌표(왼쪽 모서리)

	void Save_Desktop();

	bool m_bClicked;		// 윈도우 옮길때 마우스 클릭 여부
	int m_nMultiple;	// 배율
	int m_nScrRate;		// 스크린 가로세로 비율

protected:
	long	GetTimeGap( SYSTEMTIME *p_stTo, SYSTEMTIME *p_stFrom);
	int		m_DrawID;
	void	DoMouseMove(CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGLASS_H__B6492C82_BDFE_11D6_8777_00E07D8FB0D6__INCLUDED_)
