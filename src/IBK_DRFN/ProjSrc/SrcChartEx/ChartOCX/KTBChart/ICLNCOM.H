// iclncom.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CiLineComboBox window
#ifndef __iclncom_h__
#define __iclncom_h__

class CiLineComboBox : public CComboBox
{
// Construction
public:
	CiLineComboBox();

// Attributes
public:
	CBitmap	m_bitmap;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CiLineComboBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CiLineComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CiLineComboBox)
	afx_msg void OnDropdown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


#endif //  __iclncom_h__
