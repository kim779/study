#pragma once

#include "AxisExt.h"

class CCodeCtrl : public CWnd, public CAxisExt
{
// Construction
public:
	CCodeCtrl(CWnd *pParent, LPCSTR type, int key);
	CWnd *m_pCtrl;
	CString GetCode() { return GetCtrlProperty("Data"); }
	void    SetCode(LPCSTR code) { SetCtrlProperty("Data", _variant_t(code)); }
	CString GetName();

protected:
	void CreateControl();
	void ErrorBox( LPCSTR msg );
	void ChangeFont( LPCSTR font, int point );
	CString GetCtrlProperty( LPCSTR prop_name );
	void SetCtrlProperty( _bstr_t prop_name, _variant_t &var );
	
protected:
	LPCSTR m_type;
	int m_key;
	HINSTANCE m_hInst;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser( WPARAM wParam, LPARAM lParam );
};

/////////////////////////////////////////////////////////////////////////////

