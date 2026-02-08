#if !defined(AFX_DYNAMICBITMAPLOADBUTTON_H__22A058DF_2857_4C95_9E89_1B081AF8A4CD__INCLUDED_)
#define AFX_DYNAMICBITMAPLOADBUTTON_H__22A058DF_2857_4C95_9E89_1B081AF8A4CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DynamicBitmapLoadButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDynamicBitmapLoadButton window

class CDynamicBitmapLoadButton : public CButton
{
// Attributes
public:
	CDynamicBitmapLoadButton();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynamicBitmapLoadButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	// Generated message map functions
protected:
	//{{AFX_MSG(CDynamicBitmapLoadButton)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


public:
	bool GetToggleState();
	void SetToggleState(const bool bToggle);
	void Load(LPCSTR upImageFileName, LPCSTR downImageFileName = NULL);
	void SizeToContent();

private:
	enum BUTTONTYPE
	{
		Button_Up,
		Button_Down
	};
	bool m_bToggle;

	BUTTONTYPE m_TYPEButton;
	CString m_upImageFileName, m_downImageFileName;

	void GetBitmap(CBitmap* bitmap, const CString& imageFileName);
	void SizeToContent(const CString& imageFileName);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNAMICBITMAPLOADBUTTON_H__22A058DF_2857_4C95_9E89_1B081AF8A4CD__INCLUDED_)
