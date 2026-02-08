#if !defined(AFX_BITMAPITEMCOMBOBOX_H__B2C8A82C_B707_47DF_BAE3_307971D3BFDC__INCLUDED_)
#define AFX_BITMAPITEMCOMBOBOX_H__B2C8A82C_B707_47DF_BAE3_307971D3BFDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BitmapItemComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitmapItemComboBox window

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Comments		: 
// Using way	: 
// See			: class CComobBox
//-----------------------------------------------------------------------------
class CBitmapItemComboBox : public CComboBox
{
// Construction
public:
	CBitmapItemComboBox();

// Attributes
public:
	CBitmap	m_bitmap;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapItemComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBitmapItemComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBitmapItemComboBox)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPITEMCOMBOBOX_H__B2C8A82C_B707_47DF_BAE3_307971D3BFDC__INCLUDED_)
