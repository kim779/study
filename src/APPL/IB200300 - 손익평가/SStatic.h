#pragma once
// SStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSStatic window

class CSStatic : public CStatic
{
// Construction
public:

// Attributes
public:
	COLORREF	m_clrBk{};
	COLORREF	m_clrText{};
	COLORREF	m_clrEdge{};
	bool		m_bRight{};
// Operations
public:
	inline void SetBkColor(COLORREF bk){
		m_clrBk = bk;
	}
	inline void SetEdgeColor(COLORREF clr){
		m_clrEdge = clr;
	}
	inline void SetTextColor(COLORREF clr){
		m_clrText = clr;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSStatic)
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	//{{AFX_MSG(CSStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

