#if !defined(AFX_NPROGRESSCTRL_H__F5CE3F6E_50A8_4F22_96AB_BB3F38A5ECF3__INCLUDED_)
#define AFX_NPROGRESSCTRL_H__F5CE3F6E_50A8_4F22_96AB_BB3F38A5ECF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NProgressCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNProgressCtrl window

class CNProgressCtrl : public CProgressCtrl
{
// Construction
public:
	CNProgressCtrl();
	virtual ~CNProgressCtrl();

// Attributes
public:

// Operations
public:
	void	SetImage(CString imgName, CString back);
	void	SetRange(int nLower, int nUpper);
	int	SetStep(int nStep);
	int	SetPos(int nPos);
	int	StepIt();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNProgressCtrl)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
protected:
	int	m_nLower;
	int	m_nUpper;
	int	m_nRange;

	int	m_nStep;
	int	m_nPos;

	CBitmap	m_img;
	CBitmap	m_back;
	HBITMAP	m_hBitmap;			// Handle to bitmap
	DWORD	m_dwWidth;			// Width of bitmap
	DWORD	m_dwHeight;			// Height of bitmap

	void	CalcRange();

	void	drawBackground(CDC* pDC);
	void	drawProgress(CDC* pDC);
	//{{AFX_MSG(CNProgressCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NPROGRESSCTRL_H__F5CE3F6E_50A8_4F22_96AB_BB3F38A5ECF3__INCLUDED_)
