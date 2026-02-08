#pragma once
// PopWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPopWnd window

class CPopWnd : public CWnd
{
// Construction
public:
	CPopWnd();

// Attributes
public:

// Operations
public:
	std::unique_ptr<CBitmap> m_pBitmap;
	CCriticalSection	m_section;

	double			m_jisu;

// Implementation
public:
	void	drawBitmap();
	void	SetData(double jisu);
	virtual ~CPopWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPopWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

