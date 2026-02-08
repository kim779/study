#if !defined(AFX_TERMINATOR_H__BB45DC72_12C2_4803_A817_83860126A50A__INCLUDED_)
#define AFX_TERMINATOR_H__BB45DC72_12C2_4803_A817_83860126A50A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Terminator.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTerminator window

class CTerminator : public CStatic
{
// Construction
public:
	CTerminator();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTerminator)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTerminator();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTerminator)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void Draw(CDC* dc);
};

#define default_width 2
#define default_height 16

#include <afxtempl.h>
typedef CTypedPtrList<CPtrList, CTerminator*> CTerminatorList;

void AddNewTerminator(CTerminatorList& list, CWnd* parent);
void ClearTerminator(CTerminatorList& list);
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TERMINATOR_H__BB45DC72_12C2_4803_A817_83860126A50A__INCLUDED_)
