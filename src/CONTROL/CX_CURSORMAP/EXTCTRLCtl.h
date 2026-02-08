#pragma once

// EXTCTRLCtl.h : Declaration of the CEXTCTRLCtrl ActiveX Control class.

#include "../../h/param.h"
/////////////////////////////////////////////////////////////////////////////
// CEXTCTRLCtrl : See EXTCTRLCtl.cpp for implementation.

class CEXTCTRLCtrl : public COleControl
{
	DECLARE_DYNCREATE(CEXTCTRLCtrl)

	// Constructor & Operators
public:
	CEXTCTRLCtrl();
protected:
	CString	MakeOptions();
	void	ParseOptions();
private:
	int	OpenView(int type, CString data);
	CString	Variant(int comm, CString data);
	BOOL	SendTR(CString sTR, BYTE type, CString data, int key = -1);
	COLORREF GetIndexColor(int nIndex);


	// Attributes
public:
	CWnd		*m_pParent;		// parent view
	CParam		m_Param;		// param info class
	BOOL		m_bParentDetach;	// is parent detach
	BOOL		m_bOleLock;		// because AfxOleUnlockApp() error when create by axCreate
protected:
private:
	CString		m_sRtn;			// Return string member variable

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEXTCTRLCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CEXTCTRLCtrl();

	DECLARE_OLECREATE_EX(CEXTCTRLCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CEXTCTRLCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CEXTCTRLCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CEXTCTRLCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CEXTCTRLCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnInitExtControl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWizard(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CEXTCTRLCtrl)
	afx_msg BSTR GetProperties();
	afx_msg void SetProperties(LPCTSTR sOptions);
	afx_msg void SetParams(long pParams);
	afx_msg BOOL IsInvokeAllowed(long);
	afx_msg void SetCursorMap(LPCTSTR sKey, long left, long top, long right, long bottom);
	afx_msg void DeleteAllCursorMap();
	afx_msg void DeleteCursorMap(LPCTSTR sKey);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CEXTCTRLCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	void SetParam(struct _param *pParam);
	enum {
	//{{AFX_DISP_ID(CEXTCTRLCtrl)
	dispidGetProperties = 1L,
	dispidSetProperties = 2L,
	dispidSetParams = 3L,
	dispidIsInvokeAllowed = 4L,
	dispidSetCursorMap = 5L,
	dispidDeleteAllCursorMap = 6L,
	dispidDeleteCursorMap = 7L,
	//}}AFX_DISP_ID
	};
};