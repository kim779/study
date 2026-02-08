#if !defined(AFX_GRAPHSTYLECOMBO_H__9CDA6008_3F5B_46E2_BEA6_A73AB290CAF9__INCLUDED_)
#define AFX_GRAPHSTYLECOMBO_H__9CDA6008_3F5B_46E2_BEA6_A73AB290CAF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphStyleCombo.h : header file
// 그래프타입설정 콤보 클래스

#include "ImageComboBox.h"
#include "BlockBaseData.h"
#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CGraphStyleCombo window
static TCHAR* chType[] = 
{
	_T("0"),	_T("1"), 	_T("2"),	_T("3"),	_T("4"),	_T("5")
};

class CToolBarDlg;

class CGraphStyleCombo : public CImageComboBox
{
// Construction
public:
	CGraphStyleCombo();

// Attributes
public:
private:
	CToolBarDlg* m_pParent;
// Operations
public:
//	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void SetParentWnd(CToolBarDlg* pParent){ m_pParent = pParent; };
	void AddLineType(int nType);
	void SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphStyleCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphStyleCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphStyleCombo)
	afx_msg void OnSelchange();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHSTYLECOMBO_H__9CDA6008_3F5B_46E2_BEA6_A73AB290CAF9__INCLUDED_)
