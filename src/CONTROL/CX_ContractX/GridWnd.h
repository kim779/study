#pragma once
// GridWnd.h : header file
//
#include <afxtempl.h>

class CMatch
{
public:
	CString price;
	CString volume;

	CMatch(const char* p, const char* q): price(p), volume(q) {}
};

/////////////////////////////////////////////////////////////////////////////
// CGridWnd window

class CGridWnd : public CWnd
{
// Construction
public:
	CGridWnd(CWnd *pWizard, CWnd *pParent);

// Attributes
public:
	std::unique_ptr<class CExScrollBar>	m_pScroll;

// Operations
public:
	void	Resize();
	void	SetDisplay(int display)	{ m_iDispCnt = display; }
	void	SetFontx(CFont *pFont);

	void	AutoScroll();
	void	Clear();
	void	SetTextColor(COLORREF upColor, COLORREF downColor, COLORREF normalColor, COLORREF lineColor);
	void	UpdateScroll(int pos, int end, int day, int total);
	void	DrawData(CDC *pDC);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGridWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGridWnd)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg long OnVScroll(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	class CCX_ContractXApp	*m_pApp;
	CWnd	*m_pWizard, *m_pParent;

	COLORREF m_colorUp, m_colorDown, m_colorNormal, m_colorLine;
	CFont	*m_pFont;
	//**CStringArray	m_arPrice, m_arVolume;	// data
	
	CRect	m_rcData;
	int	m_SCRpos, m_SCRday, m_SCRend, m_SCRtotal;	// 데이터 시작위치, 그려지는개수, 끝위치(m_SCRpos+m_SCRday), 전체개수
	int	m_iDispCnt;					// 보여지는 데이터 개수(5 or 10)
	int	m_iHalfLine;					// 중앙 세로선

//** macho add
private:
	BOOL	m_showTitle;
	void	SetShowTitle(BOOL showTitle)
	{
		if (m_showTitle != showTitle)
		{
			m_showTitle = showTitle;
			Invalidate();
		}
	}

	int	m_col1, m_col2; 

//	typedef CTypedPtrList<CPtrList, CMatch*> m_match;
	CList <std::shared_ptr<CMatch>, std::shared_ptr<CMatch>> m_match;

	int	GetCount() const	{ return m_match.GetCount();	}

public:
	void	SetColInfo(int col1, int col2);
	void	InsertData(const char* price, const char* volume);
	void	AddData(const char* price, const char* volume);
	void	AutoUpdate();
};
