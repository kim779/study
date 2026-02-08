#if !defined(AFX_GRAPHSETTINGBUTTON_H__4E8441C2_69B8_4411_9149_95BFCE5E050C__INCLUDED_)
#define AFX_GRAPHSETTINGBUTTON_H__4E8441C2_69B8_4411_9149_95BFCE5E050C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphSettingButton.h : header file
//

#include "DlgGraphSetting.h"	// Added by ClassView
#include "GraphColorCollection.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CGraphSettingButton window
class CDlgGraphSetting;
class CGraphSettingButton : public CButton
{
// Construction
public:
	CGraphSettingButton(int nIndex, CWnd* pDlgChartWnd);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphSettingButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphSettingButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphSettingButton)
	afx_msg void OnClicked();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CDlgGraphSetting* m_pDlgGraphSetting;
	int m_nIndex;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHSETTINGBUTTON_H__4E8441C2_69B8_4411_9149_95BFCE5E050C__INCLUDED_)
