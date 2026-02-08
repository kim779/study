#if !defined(AFX_DFONTPROPPAGE_H__087D008E_06E3_4504_AA4E_390A3D62C907__INCLUDED_)
#define AFX_DFONTPROPPAGE_H__087D008E_06E3_4504_AA4E_390A3D62C907__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DFontPropPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDFontPropPage : Property page dialog

interface IPropertyMap;
class CDFontPropPage : public CDialog
{

public:
	CDFontPropPage(CWnd* pOcxWnd, CWnd* pParent = NULL);

	CString			mt_strlfFaceName;
	int				mt_nlfHeight;
	BOOL			mt_bBold;
	BOOL			mt_bCancelLine;
	BOOL			mt_bItaly;
	BOOL			mt_bUnderLine;
public:
	void			DrawPreview();
	void			FT_vSaveFont();
	void			FT_vInitFont(const CString& initStr);


// Dialog Data
	//{{AFX_DATA(CDFontPropPage)
	enum { IDD = IDD_PROPPAGE_FONT };
	CButton	m_checkUnderLine;
	CButton	m_checkItaly;
	CButton	m_checkCancelLine;
	CButton	m_checkBold;
	CComboBox	m_cbFontList;
	CComboBox	m_cbFontSize;
	CStatic	m_FontPreview;
	CString	m_FontProp;
	//}}AFX_DATA

// Implementation
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDFontPropPage)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support
	//}}AFX_VIRTUAL


// Message maps
protected:
	//{{AFX_MSG(CDFontPropPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnFontBold();
	afx_msg void OnFontCancelline();
	afx_msg void OnFontItaly();
	afx_msg void OnFontUnderline();
	afx_msg void OnSelchangeComboFontlist();
	afx_msg void OnSelchangeComboFontsize();
	afx_msg void OnEditchangeComboFontsize();
	afx_msg void OnEditchangeComboFontlist();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pOcxWnd;

// (2006/7/5 - Seung-Won, Bae) Use IPropertyMap
protected:
	IPropertyMap *	m_pIPropertyMap;
public:
	void SetProperty( IPropertyMap *p_pIPropertyMap);

// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
protected:
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DFONTPROPPAGE_H__087D008E_06E3_4504_AA4E_390A3D62C907__INCLUDED_)
