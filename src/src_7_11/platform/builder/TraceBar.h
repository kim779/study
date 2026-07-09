// TraceBar.h: interface for the CTraceBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRACEBAR_H__1034B21F_3D17_43BC_84A3_4911626A4367__INCLUDED_)
#define AFX_TRACEBAR_H__1034B21F_3D17_43BC_84A3_4911626A4367__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdAfx.h"
#include "mapform.h"
#include "Components/ResultEditCtrl.h"

class CTraceBar : public CSizingControlBarG  
{
public:
	CTraceBar();
	virtual ~CTraceBar();
public:
public:
	CString Parser(CString &srcstr, CString substr);
	void BarTextClear();
	void FixedAddText(CString txt);
	void AddText(CString txt);
	CResultEditCtrl		m_resultEdit;
	CWnd			*m_pWizard;
protected:
private:
	bool		m_btextpos;
	long		m_ltextpos;
protected:
	CFont		m_font;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

		// Generated message map functions
protected:
	//{{AFX_MSG(CScriptBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnTrace(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCRIPTBAR_H__1034B21F_3D17_43BC_84A3_4911626A4367__INCLUDED_)
