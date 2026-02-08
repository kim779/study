#if !defined(AFX_MANAGEINFO_H__E80553AE_FA74_4431_B7A4_93ABE1939F19__INCLUDED_)
#define AFX_MANAGEINFO_H__E80553AE_FA74_4431_B7A4_93ABE1939F19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ManageInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CManageInfo dialog

class CManageInfo : public CDialog
{
// Construction
public:
	CManageInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CManageInfo)
	enum { IDD = IDD_MNGINFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManageInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CManageInfo)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	void	OnDraw(CDC* pDC);
	void	DrawBack(CDC* pDC);
	void	DrawFrame(CDC* pDC);
	void	DrawData(CDC* pDC);

	int	m_width;
	int	m_height;
	int	m_nKind;
	int	m_nRowHeight;

	CRect	m_baseRc;
	CStringArray	m_arData;
	CRect	m_rect;	// my rect
	CFont	m_font;
	
public:
	void	ChangePalette();
	void	ChangeData();
	void	ShowSlide(CRect);
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

public:
	int m_x{}, m_y{};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGEINFO_H__E80553AE_FA74_4431_B7A4_93ABE1939F19__INCLUDED_)
