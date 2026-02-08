#if !defined(AFX_COLORBUTTON_H__0692E02A_C56D_48D0_B823_DD8B993F4EA5__INCLUDED_)
#define AFX_COLORBUTTON_H__0692E02A_C56D_48D0_B823_DD8B993F4EA5__INCLUDED_

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
	CColorButton(const COLORREF& color);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorButton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


public:
	void SetColor(const COLORREF& color);
	COLORREF GetColor() const;

private:
	COLORREF m_color;

	COLORREF GetComplementaryColor(const COLORREF& color) const;
	int GetComplementaryColor(const int nColor) const;

	void DrawRectangle(CDC* pDC);
	void CColorButton::DrawText(CDC* pDC, const CString& strText);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBUTTON_H__0692E02A_C56D_48D0_B823_DD8B993F4EA5__INCLUDED_)
