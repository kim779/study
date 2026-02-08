#pragma once
// ContractGrid.h : header file
//
#include <afxtempl.h>
enum { clr_UpIndex = 0, clr_NormalIndex, clr_DnIndex};

class CMatch {
public:
	CString price;
	CString contract;
	int	cp;	// price color index(0,1,2)
	int	cc;	// contract color index(0,1,2)

	CMatch(const char* p, const char* q, int cp, int cc): price(p), contract(q), cp(cp), cc(cc) {}
};

/////////////////////////////////////////////////////////////////////////////
// CContractGrid window

class CContractGrid : public CWnd
{
// Construction
public:
	CContractGrid(CWnd *pWizard, CWnd *pParent);
	virtual ~CContractGrid();

// Attributes
public:
	std::unique_ptr<class CContractScroll>	m_pScroll;

// Operations
public:
	void	Init(int DisplayCnt) { m_iDispCnt = DisplayCnt; }
	void	Clear();
	void	SetDisplay(int display) { m_iDispCnt = display; }
	void	ChangeDisplay(int display);
	void	SetHalfLineInfo(int iHalf) { m_iHalfInfo = iHalf; }
	void	InsertData(const char* price, const char* contract, int cp, int cc);
	void	AddData(const char* price, const char* contract, int cp, int cc);
	void	AutoUpdate();
	void	SetFontx(CFont* pFont);
	void	PaletteGrid(COLORREF upColor, COLORREF downColor, COLORREF normalColor,
		COLORREF lineColor, COLORREF gridColor1st, COLORREF gridColor2st);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContractGrid)
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
protected:
	void	Resize();
	void	AutoScroll();
	void	UpdateScroll(int pos, int end, int day, int total);
	void	DrawData(CDC* pDC);

	//{{AFX_MSG(CContractGrid)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg long OnVScroll(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	class CCX_FODepthApp	*m_pApp;
	CWnd	*m_pWizard, *m_pParent;
	//typedef CTypedPtrList<CPtrList, CMatch*> m_match;
	CList <std::shared_ptr<CMatch>, std::shared_ptr<CMatch>> m_match;

	COLORREF m_pColor[3];	// 상승,보합,하락
	COLORREF m_colorUp, m_colorDown, m_colorNormal;
	COLORREF m_colorLine, m_colorGrid1st, m_colorGrid2st;
	CFont	*m_pFont;
	
	CRect	m_rcData;
	int	m_SCRpos, m_SCRday, m_SCRend, m_SCRtotal;	// 데이터 시작위치, 그려지는개수, 끝위치(m_SCRpos+m_SCRday), 전체개수
	int	m_iDispCnt;	// 보여지는 데이터 개수(5 or 10)
	int	m_iHalfLine;	// 중앙 세로선
	int	m_iHalfInfo;
	BOOL	m_showTitle;

	void	SetShowTitle(BOOL showTitle)
	{
		if (m_showTitle != showTitle)
		{
			m_showTitle = showTitle;
			Invalidate();
		}
	}

	int	GetCount() const 	{ return m_match.GetCount();	}
};