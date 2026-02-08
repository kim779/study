#pragma once

#include "../../h/axisfire.h"
#include "Common.h"
#include "AxisExt.h"

class CAccountCtrl : public CWnd, public CAxisExt
{
public:
	CAccountCtrl(CWnd *m_pParent, LPCSTR type, int key);
	CString GetNumber();
	CString GetName();
	CString GetPassword();
	void ChangeFont(LPCSTR font, int point);
	void Trigger(LPCSTR accno);
	void CreateControl();
protected:
	void ErrorBox(LPCSTR msg);
	CString GetCtrlProperty(LPCSTR property);
public:
	CString m_type;
	int m_key;
	CWnd *m_pCtrl;
	HINSTANCE m_hInst;
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccountCtrl)
	//}}AFX_VIRTUAL

public:
	virtual ~CAccountCtrl();

protected:
	//{{AFX_MSG(CAccountCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////

