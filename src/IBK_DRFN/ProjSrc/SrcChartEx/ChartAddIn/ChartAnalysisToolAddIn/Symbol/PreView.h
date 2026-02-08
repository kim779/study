#if !defined(AFX_PREVIEW_H__65E615EF_F902_4AF1_BA9E_C78EE987F3C7__INCLUDED_)
#define AFX_PREVIEW_H__65E615EF_F902_4AF1_BA9E_C78EE987F3C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreView.h : header file
//

#include "SymbolBaseData.h"
/////////////////////////////////////////////////////////////////////////////
// CPreView window

class CPropertiesData_text;
class CPreView : public CStatic
{
// Construction
public:
	CPreView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPreView();

	void SetBackgroundColor(const COLORREF& color);
	void SetPropertiesData(CPropertiesData_text* pProperties);

	CSymbolBaseData::TOOLTYPE GetSymbolType();
	void ModifySymbolType(const CSymbolBaseData::TOOLTYPE symboltool);

// Generated message map functions
protected:
	//{{AFX_MSG(CPreView)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CRect m_rcThis;
	CString m_text;
	CSymbolBaseData::TOOLTYPE m_eSymbolTool;
	CPropertiesData_text* m_pProperties;
	COLORREF m_backgroundColor;//sy 2005.03.30.

	LOGFONT GetLogFont() const;
	COLORREF GetTextColor() const;
	COLORREF GetColor() const;
	CSymbolBaseData::SIZETYPE GetSizeType() const;

	void Draw(CDC* pDC);
	CSize GetOneTextSize(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEW_H__65E615EF_F902_4AF1_BA9E_C78EE987F3C7__INCLUDED_)
