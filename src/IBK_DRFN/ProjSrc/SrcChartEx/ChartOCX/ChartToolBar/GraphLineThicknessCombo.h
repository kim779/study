#if !defined(AFX_GRAPHLINETHICKNESSCOMBO_H__03134344_D385_4901_A71A_ACD010FCEF38__INCLUDED_)
#define AFX_GRAPHLINETHICKNESSCOMBO_H__03134344_D385_4901_A71A_ACD010FCEF38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphLineThicknessCombo.h : header file
// 그래프 선 굵기 설정 클래스.

#include "ImageComboBox.h"
#include "Resource.h"

/////////////////////////////////////////////////////////////////////////////
// CGraphLineThicknessCombo window
static TCHAR* szTypeLine[] = 
{
	_T("0"),	_T("1"), 	_T("2"),	_T("3"),	_T("4"),	_T("5")
};

class CToolBarDlg;

class CGraphLineThicknessCombo : public CImageComboBox
{
// Construction
public:
	CGraphLineThicknessCombo();

// Attributes
public:
private:
	CToolBarDlg* m_pParent;	// GetParent()가 잘못된 포인터를 리턴하는경우가 ScrollBar에서
							// 생겨서(원인파악안됨T_T) 멤버변수에 직접 부모윈도우포인터설정.


// Operations
public:
	void SetParentWnd(CToolBarDlg* pParent){ m_pParent = pParent; };	
	void SetLineThickness(int nThickness);
	void AddLineType(int nType);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphLineThicknessCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphLineThicknessCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphLineThicknessCombo)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSelchange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHLINETHICKNESSCOMBO_H__03134344_D385_4901_A71A_ACD010FCEF38__INCLUDED_)
