#if !defined(AFX_COLORBUTTON_H__B7CD7C1C_27F3_4A00_96A8_F61786AE4724__INCLUDED_)
#define AFX_COLORBUTTON_H__B7CD7C1C_27F3_4A00_96A8_F61786AE4724__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorButton window

class CColorButton : public CButton
{
// Construction
public:
	CColorButton();
 	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetBtnText();
	void	SetBtnText(const char* p_pBtnText);
	virtual ~CColorButton();

	void SetColor(const COLORREF& color);
	COLORREF GetColor(){ return m_color;}
	

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF	m_color;
	CString		m_strBtnText;  //버튼에 표시할 텍스트
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBUTTON_H__B7CD7C1C_27F3_4A00_96A8_F61786AE4724__INCLUDED_)
