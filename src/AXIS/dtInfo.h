#if !defined(AFX_DTINFO_H__A1FEF8B5_F578_4F6F_AD66_BC45BF47D913__INCLUDED_)
#define AFX_DTINFO_H__A1FEF8B5_F578_4F6F_AD66_BC45BF47D913__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

class CDtInfo : public CDialog
{
public:
	CDtInfo(CWnd* pParent);
	//{{AFX_DATA(CDtInfo)
	enum { IDD = IDD_DTINFO };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDtInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDtInfo)
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	void	OnDraw(CDC* pDC);
	void	DrawBack(CDC* pDC);
	void	DrawFrame(CDC* pDC);
	void	DrawData(CDC* pDC);

	CWnd*	m_mainframe;
	int	m_width;
	int	m_height;
	int	m_nRowHeight;

	CRect	m_baseRc;
	CStringArray	m_arData;
	CRect	m_rect;	// my rect
	CFont	m_font;
	bool	m_bDoctor;
	
public:
	void	ChangePalette();
	void	ChangeData();
	void	ShowSlide(CRect);
	void	HideSlide();
	void	SetData(CString dat);
	void	SetDataColor(CString dat, COLORREF clrHeader, COLORREF clrCell);
	void	StopSlide();	
	void	GetSize(int& width, int& height) { width = m_width, height = m_height; }
	void	SetGubn(bool bDoctor) { m_bDoctor = bDoctor;	}
protected:
	void	Slide();
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_DTINFO_H__A1FEF8B5_F578_4F6F_AD66_BC45BF47D913__INCLUDED_)
