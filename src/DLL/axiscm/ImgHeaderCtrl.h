#if !defined(AFX_IMGHEADERCTRL_H__F2B5E864_A057_4160_ADC5_063936691C69__INCLUDED_)
#define AFX_IMGHEADERCTRL_H__F2B5E864_A057_4160_ADC5_063936691C69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImgHeaderCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImgHeaderCtrl window

class CImgHeaderCtrl : public CHeaderCtrl
{
// Construction
public:
	CImgHeaderCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgHeaderCtrl)
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImgHeaderCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImgHeaderCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	CBitmap* m_bmpStart, * m_bmpSpan, * m_bmpEnd;
public:
	void SetImgBitmap(CBitmap* bmpStart, CBitmap* bmpSpan, CBitmap* bmpEnd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGHEADERCTRL_H__F2B5E864_A057_4160_ADC5_063936691C69__INCLUDED_)
