// ExComboBox.h: interface for the CExComboBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCOMBOBOX_H__41B80433_4033_42F6_A579_471D1B967BE0__INCLUDED_)
#define AFX_EXCOMBOBOX_H__41B80433_4033_42F6_A579_471D1B967BE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoaderComboBox;
class CExComboBox : public CComboBox  
{
	DECLARE_DYNCREATE(CExComboBox);
// Construction
public:
	CExComboBox(BOOL bUseDefSkin = FALSE);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	CLoaderComboBox*		m_pLoader;

// Implementation
public:
	virtual ~CExComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExComboBox)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_EXCOMBOBOX_H__41B80433_4033_42F6_A579_471D1B967BE0__INCLUDED_)
