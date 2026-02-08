#if !defined(AFX_IMAGECOMBOBOX_H__98E9A5AE_1323_45DE_AA53_3F155C2301ED__INCLUDED_)
#define AFX_IMAGECOMBOBOX_H__98E9A5AE_1323_45DE_AA53_3F155C2301ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageComboBox window

class CImageComboBox : public CComboBox
{
// Construction
public:
	CImageComboBox();
	virtual ~CImageComboBox();

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageComboBox)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void SetToolTipText(const CString& strText);
	void SetToolTipText(const CString& strText, const bool bActivate);

	CBitmap& GetBitmap();

private:
	CString m_strToolTipText;
	CToolTipCtrl m_ToolTip;

	CBitmap	m_bitmap;

	void InitToolTip();
	void AddToolInTooltip(const CString& strText, const bool bActivate);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGECOMBOBOX_H__98E9A5AE_1323_45DE_AA53_3F155C2301ED__INCLUDED_)
