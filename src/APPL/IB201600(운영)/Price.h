#pragma once
// Price.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrice window

#include "AxisExt.h"

class CPrice : public CWnd, public CAxisExt
{
// Construction
public:
	CPrice(CWnd* wizard);

// Attributes
public:
	CString		m_sRoot, m_rtnStr;
	HINSTANCE	m_hPriceDll;
	//CWnd*		m_pParent;
	CWnd*		m_pPrice;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrice)
	//}}AFX_VIRTUAL

// Implementation
public:
	void BtnClick();
	void SetValues(CString str);
	void createPrice(int Col,int Row);
	CString GetCode() { return GetCtrlProperty("JCode"); }
	void    SetCode(LPCSTR code) { SetCtrlProperty("JCode", _variant_t(code)); }

	virtual ~CPrice();

protected:
	CString GetCtrlProperty( LPCSTR prop_name );
	void SetCtrlProperty( _bstr_t prop_name, _variant_t &var );

	// Generated message map functions
protected:
	//{{AFX_MSG(CPrice)
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	long OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

