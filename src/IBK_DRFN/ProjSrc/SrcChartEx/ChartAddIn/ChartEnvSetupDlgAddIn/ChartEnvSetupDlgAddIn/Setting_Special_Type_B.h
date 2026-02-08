#if !defined(AFX_SETTING_SPECIAL_TYPE_B_H__6247BC89_362A_4420_8B9E_8B4F117DCDCB__INCLUDED_)
#define AFX_SETTING_SPECIAL_TYPE_B_H__6247BC89_362A_4420_8B9E_8B4F117DCDCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting_Special_Type_B.h : header file
//


#include "BaseDialog.h"
#include "ColorButton.h"


/////////////////////////////////////////////////////////////////////////////
// CSetting_Special_Type_B dialog
class CSetting_Special_Type_B : public CBaseDialog
{
// Construction
public:
	CSetting_Special_Type_B( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL, CWnd *pTopWnd = NULL);   // standard constructor
	virtual ~CSetting_Special_Type_B();

	virtual void Initial(IGraphInfo *p_pIGraphInfo);
	// 20081014 박동관 >>
	virtual BOOL OnApply(IGraphInfo *p_pIGraphInfo);
	//virtual void OnApply(IGraphInfo *p_pIGraphInfo);
	// 20081014 박동관 <<

// Dialog Data
	//{{AFX_DATA(CSetting_Special_Type_B)
	enum { IDD = IDD_SPECIAL_TYPE_B };
	CBitmapItemComboBox	m_cmbThickness;
	CColorButton	m_btnColorBtn;
	CString	m_strBlockChange;
	//}}AFX_DATA


// Attribute
private:
	int m_nSubGraphIndex;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting_Special_Type_B)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting_Special_Type_B)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnColor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_SPECIAL_TYPE_B_H__6247BC89_362A_4420_8B9E_8B4F117DCDCB__INCLUDED_)
