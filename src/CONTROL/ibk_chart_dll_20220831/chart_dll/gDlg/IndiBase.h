#if !defined(AFX_INDIBASE_H__F317A459_DDF0_49B8_B9EC_3309C66E1CB2__INCLUDED_)
#define AFX_INDIBASE_H__F317A459_DDF0_49B8_B9EC_3309C66E1CB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIndiBase : public CDialog
{
public:
	CIndiBase(CWnd* pParent = NULL);
	void SetScreen(struct _indiInfo* pIndi, struct _graph* pGraph);

	//{{AFX_DATA(CIndiBase)
	enum { IDD = IDD_INDI_BASE };
	CColorCtrl	m_color7;
	CColorCtrl	m_color6;
	CColorCtrl	m_color5;
	CColorCtrl	m_color4;
	CColorCtrl	m_color3;
	CColorCtrl	m_color2;
	CColorCtrl	m_color1;
	CSpinCtrl	m_spin7;
	CSpinCtrl	m_spin6;
	CSpinCtrl	m_spin5;
	CSpinCtrl	m_spin4;
	CSpinCtrl	m_spin3;
	CSpinCtrl	m_spin2;
	CSpinCtrl	m_spin1;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CIndiBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CIndiBase)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CStatic				m_stContent[7];
	CSpinCtrl*			m_spinContent[7];
	CComboBox			m_cbContent[7];
	CColorCtrl*			m_apcolor[7];
	
	struct	_graph*			m_pGraph;
	struct	_indiInfo*		m_pIndi;
	bool				m_bBig;
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_INDIBASE_H__F317A459_DDF0_49B8_B9EC_3309C66E1CB2__INCLUDED_)
