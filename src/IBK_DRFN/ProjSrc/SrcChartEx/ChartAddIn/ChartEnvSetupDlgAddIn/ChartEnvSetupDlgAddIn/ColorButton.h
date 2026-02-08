#if !defined(AFX_COLORBUTTON_H__914BCC72_68CF_46B0_AAA6_71AE9F5BA061__INCLUDED_)
#define AFX_COLORBUTTON_H__914BCC72_68CF_46B0_AAA6_71AE9F5BA061__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorButton window

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 10
// Comments		: 색깔을 설정하는 Button
// Using way	: 
// See			: class CButton
//-----------------------------------------------------------------------------
class CColorButton : public CButton
{
// Construction
public:
	CColorButton();
	virtual ~CColorButton();

// Attribute
private:
	COLORREF m_color;
	CString m_strText;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL


// Implementation
public:
	COLORREF GetColor( void );
	void SetColor( const COLORREF &color );
	void SetText( CString szBtnText );

private:
	COLORREF GetComplementaryColor( COLORREF &color );		//보색을 얻는다.


	// Generated message map functions
protected:
	//{{AFX_MSG(CColorButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBUTTON_H__914BCC72_68CF_46B0_AAA6_71AE9F5BA061__INCLUDED_)
