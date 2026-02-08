#pragma once

#include "Common.h"
#include "grid/GridCtrl.h"
#include "AxisExt.h"

class CSiseWnd : public CGridCtrl, public CAxisExt
{
// Construction
public:
	CSiseWnd(CWnd *pParent);
	BOOL Create(const RECT& rect, CWnd* parent, UINT nID, DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE , DWORD dwExStyle = 0);
	void SetData(SiseData *psd);
	void SetRealData(RealData *rp);
	void SetRealData( DWORD* data );
	void Clear();
	void ChangeTheme();
protected:
	void SetSiseGrid();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSiseWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSiseWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSiseWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

