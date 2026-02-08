#if !defined(AFX_INDIOBJMA_H__BB2C1DC8_04C3_45E0_8B58_76444E32D3DE__INCLUDED_)
#define AFX_INDIOBJMA_H__BB2C1DC8_04C3_45E0_8B58_76444E32D3DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIndiObjMA : public CDialog
{
public:
	CIndiObjMA(CWnd* pParent = NULL);
	void SetScreen(struct _graph* graph, bool bEach = false, bool bTitle = true);

protected:
	bool			m_bEach;
	struct	_graph*		m_pGraph;
	struct	_indiInfo*	m_pIndi;
	CHART_INFO*		m_pChart;
	

protected:
	void SetColorInfo(float value, struct _line* line);


public:
	//{{AFX_DATA(CIndiObjMA)
	enum { IDD = IDD_INDI_OBJ_MA };
	CStatic	m_stcValue;
	CComboBox	m_cbValue;
	CComboBox	m_cbType;
	CSpinCtrl	m_period;
	CWidthCtrl	m_width;
	CKindCtrl	m_kind;
	CColorCtrl	m_color;
	CLineListBox	m_list;
	CButton	m_stIndi;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CIndiObjMA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CIndiObjMA)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();	
	afx_msg void OnSelchangeCbMatype();
	afx_msg void OnSelchangeCbMavalue();
	//}}AFX_MSG
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_INDIOBJMA_H__BB2C1DC8_04C3_45E0_8B58_76444E32D3DE__INCLUDED_)
