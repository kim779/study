#if !defined(AFX_EXCHECKBOX_H__F3C74CED_752C_41F9_AF81_888FBCAD9C62__INCLUDED_)
#define AFX_EXCHECKBOX_H__F3C74CED_752C_41F9_AF81_888FBCAD9C62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExCheckBox.h : header file
//

#include "ExButton.h"

/////////////////////////////////////////////////////////////////////////////
// CExCheckBox window
class CLoaderCheckBoxButton;
class CExCheckBox : public CButton
{
	DECLARE_DYNCREATE(CExCheckBox);
public:
	CExCheckBox(BOOL bUseDefSkin = FALSE);
	virtual ~CExCheckBox();

	//-----------------------------------------------------------------------------
	// Author		: ojtaso 	Date : 2006/1/9
	// Return		: void
	// Paramter
	//		COLORREF clrText : 
	// Comments		: 
	//-----------------------------------------------------------------------------
	void	SetColor(COLORREF clrText, COLORREF clrBackground);
	void	SetSkinString(LPCTSTR lpszText, LPCTSTR lpszBackground);
	void	SetSkinIndex(UINT nText, UINT nBackground);

protected:
	CLoaderCheckBoxButton*		m_pLoader;

	//{{AFX_VIRTUAL(CExCheckBox)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExCheckBox)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCHECKBOX_H__F3C74CED_752C_41F9_AF81_888FBCAD9C62__INCLUDED_)
