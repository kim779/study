#if !defined(AFX_EXGROUPBOX_H__EA5CC1B6_E691_4F4E_A6F7_9B602459A75F__INCLUDED_)
#define AFX_EXGROUPBOX_H__EA5CC1B6_E691_4F4E_A6F7_9B602459A75F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExGroupbox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExGroupbox window

class CExGroupbox : public CButton
{
	DECLARE_DYNAMIC(CExGroupbox);
// Construction
public:
	CExGroupbox();	
// Attributes
public:

// Operations
public:
   virtual CExGroupbox& SetAlignment(DWORD dwType);
   virtual CExGroupbox&	SetBorderColor(COLORREF clrBorder);
   virtual CExGroupbox&	SetCatptionTextColor(COLORREF clrText);
   virtual CExGroupbox& SetBackgroundColor(COLORREF clrBKClient);
   virtual CExGroupbox&	SetBackgroundColor(COLORREF clrBKTilte,  COLORREF clrBKClient);

    
   virtual CExGroupbox& SetText(LPCTSTR lpszTitle);
   virtual CExGroupbox& SetFontBold(BOOL bBold);
   virtual CExGroupbox& SetFontName(const CString& strFont, BYTE byCharSet = ANSI_CHARSET);
   virtual CExGroupbox& SetFontUnderline(BOOL bSet);
   virtual CExGroupbox& SetFontItalic(BOOL bSet);
   virtual CExGroupbox& SetFontSize(int nSize);
   virtual CExGroupbox& SetFont(LOGFONT lf);
   virtual CExGroupbox& SetUserFont(CFont* fontdata);
 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExGroupbox)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExGroupbox();

	// Generated message map functions
protected:
	void ReconstructFont();
	void UpdateSurface();
	//{{AFX_MSG(CExGroupbox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CString		m_strTitle;

	COLORREF	m_clrBorder;
	COLORREF	m_clrTitleBackground;    
	COLORREF	m_clrClientBackground;
	COLORREF	m_clrTitleText;

	DWORD		m_dwAlignment;  
	LOGFONT		m_lf;
	CFont		m_font;
private:
	

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXGROUPBOX_H__EA5CC1B6_E691_4F4E_A6F7_9B602459A75F__INCLUDED_)
