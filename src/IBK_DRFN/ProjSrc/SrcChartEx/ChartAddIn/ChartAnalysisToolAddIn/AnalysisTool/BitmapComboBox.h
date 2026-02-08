#if !defined(AFX_BITMAPCOMBOBOX_H__4C2BFA6F_3A82_47B3_A77E_F18302EF80D5__INCLUDED_)
#define AFX_BITMAPCOMBOBOX_H__4C2BFA6F_3A82_47B3_A77E_F18302EF80D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BitmapComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBitmapComboBox window

class CBitmapComboBox : public CComboBox
{
// Construction
public:
	CBitmapComboBox();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapComboBox)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual int GetCount() const;
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CBitmapComboBox)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void LoadBitmap(const int nBitmapResource, const unsigned int nCount);

private:
	int m_nCount;
	CBitmap	m_bitmap;

	void InitializeData();
	void SetCount(const unsigned int nCount);
	int GetItemHight() const;

	void DrawItem(CDC* pDC, const CRect& rect, const int nItemID);
	void DrawSelectedItem(CDC* pDC, const CRect& rect, const bool bSelected);
	void DrawFrame(CDC* pDC, const CRect& rect, const COLORREF& color);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPCOMBOBOX_H__4C2BFA6F_3A82_47B3_A77E_F18302EF80D5__INCLUDED_)
