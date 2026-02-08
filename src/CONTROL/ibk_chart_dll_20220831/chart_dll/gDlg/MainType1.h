#if !defined(AFX_MAINTYPE1_H__05EDCEFC_8764_420A_A0B2_E3D31DC51478__INCLUDED_)
#define AFX_MAINTYPE1_H__05EDCEFC_8764_420A_A0B2_E3D31DC51478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainType1 : public CDialog
{
public:
	CMainType1(CWnd* pParent = NULL);   

protected:
	WORD	m_wUnionKind;

public:
	void SetScreen(struct _envInfo* penvinfo, struct _graph* pGraph);
	WORD	GetUnion()	{ return m_wUnionKind; };
public:
	//{{AFX_DATA(CMainType1)
	enum { IDD = IDD_MAIN_TYPE1 };
	
	CColorCtrl	m_color;
	CComboBox	m_cbValue;
	CSpinCtrl	m_spin5;
	CSpinCtrl	m_spin4;
	CSpinCtrl	m_spin3;
	CSpinCtrl	m_spin2;
	CSpinCtrl	m_spin1;
	CLineListBox	m_list;
	CWidthCtrl	m_width;
	CKindCtrl	m_kind;
	//}}AFX_DATA
	

	//{{AFX_VIRTUAL(CMainType1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    	
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CMainType1)		
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeCbValue();
	afx_msg void OnChkFill();
	afx_msg void OnSelchangeComboUnion();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	struct _graph*	m_pGraph;	
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_MAINTYPE1_H__05EDCEFC_8764_420A_A0B2o_E3D31DC51478__INCLUDED_)
