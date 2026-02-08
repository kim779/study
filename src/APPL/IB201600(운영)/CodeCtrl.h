#pragma once

#include "AxisExt.h"

typedef enum { 
	TK_PIBO3002      =   1,
	TK_PIBOFCHG      =   2,
	TK_PIBOFJGO      =   3,
	TK_PIBOFODR      =   4,
	TK_SDLDQ003      =   5,
	TK_SACMT207      =   6,
	TK_SONBQ101      =   7,
	TK_CODE_CTRL     = 100,
	TK_NOTIFY_CTRL   = 200,
	TK_ACCOUNT_CTRL1 = 254,
	TK_ACCOUNT_CTRL2 = 255,
} TR_KEY;

class CCodeCtrl : public CWnd, public CAxisExt
{
// Construction
public:
	CCodeCtrl(CWnd *pParent, LPCSTR type, int key);
	CWnd *m_pCtrl;
	CString GetCode() { return GetCtrlProperty("Data"); }
	void    SetCode(LPCSTR code) { SetCtrlProperty("Data", _variant_t(code)); }
	CString GetName();
	void SetHistory(CString his);

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

