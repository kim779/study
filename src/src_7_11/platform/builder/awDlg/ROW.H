#if !defined(AFX_ROW_H__B94158C1_5D63_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_ROW_H__B94158C1_5D63_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// row.h : header file
//

#include "resource.h"
#include "../form_w.h"
#include "../mapvar.h"
#include "../awCommon/xbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CRow dialog

class CRow : public CDialog
{
// Construction
public:
	CRow(CWnd* pParent, struct _repR* repR, class CAxisPalette* pal, DWORD attr, bool common = false);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRow)
	enum { IDD = IDD_TABLECELL };
	CString	m_float;
	CString	m_hpRGB;
	CString	m_htRGB;
	CString	m_len;
	CString	m_pRGB;
	CString	m_head;
	CString	m_symbol;
	CString	m_tRGB;
	BOOL	m_visible;
	CString	m_domino;
	CString	m_reference;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
public:
	DWORD	m_attr;
	struct	_repR	m_repR;

protected:
	struct	_repR*	m_pRepR;
	CAxisPalette*	m_palette;
	CXButton	m_row[MAX_COLUMN];
	int	m_sel;
	bool	m_common;
	bool	m_first;
	CWnd*	m_parent;

// Implementation
protected:
	void	setRow(int id, bool save = true);
	void	setRow();
	void	saveRow();
	void	applyCommonRow(int prop);

	// Generated message map functions
	//{{AFX_MSG(CRow)
	virtual BOOL OnInitDialog();
	afx_msg void OnInsert();
	afx_msg void OnDelete();
	afx_msg void OnChangeColor();
	afx_msg void OnEdit();
	afx_msg void OnAttr();
	afx_msg void OnHPRGB();
	afx_msg void OnHTRGB();
	afx_msg void OnPRGB();
	afx_msg void OnTRGB();
	afx_msg void OnSelChangeDataType();
	afx_msg void OnRow01();
	afx_msg void OnRow02();
	afx_msg void OnRow03();
	afx_msg void OnRow04();
	afx_msg void OnRow05();
	afx_msg void OnRow06();
	afx_msg void OnRow07();
	afx_msg void OnRow08();
	afx_msg void OnRow09();
	afx_msg void OnRow10();
	afx_msg void OnRow11();
	afx_msg void OnRow12();
	afx_msg void OnRow13();
	afx_msg void OnRow14();
	afx_msg void OnRow15();
	afx_msg void OnRow16();
	afx_msg void OnRow17();
	afx_msg void OnRow18();
	afx_msg void OnRow19();
	afx_msg void OnRow20();
	afx_msg void OnRow21();
	afx_msg void OnRow22();
	afx_msg void OnRow23();
	afx_msg void OnRow24();
	afx_msg void OnRow25();
	afx_msg void OnRow26();
	afx_msg void OnRow27();
	afx_msg void OnRow28();
	afx_msg void OnRow29();
	afx_msg void OnRow30();
	afx_msg void OnRow31();
	afx_msg void OnRow32();
	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnDominoCheck();
	afx_msg void OnReferenceCheck();
	afx_msg void OnShiftLeft();
	afx_msg void OnShiftRight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROW_H__B94158C1_5D63_11D4_A024_00001CD7F9BE__INCLUDED_)
