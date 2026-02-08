#if !defined(AFX_INDIOBJ_H__D0E54135_E8CE_4C8F_981B_4EF8704EBD16__INCLUDED_)
#define AFX_INDIOBJ_H__D0E54135_E8CE_4C8F_981B_4EF8704EBD16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIndiBase;
class CIndiHorizon;
class CIndiDisp;
class CIndiObj : public CDialog
{
public:
	CIndiObj(CWnd* pParent = NULL);
	void SetScreen(struct _graph* pGraph, bool bEach = false, bool bTitle = true);

	//{{AFX_DATA(CIndiObj)
	enum { IDD = IDD_INDI_OBJ };
	CButton	m_stIndi;
	CLineListBox	m_list;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CIndiObj)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CIndiObj)
	afx_msg void OnClickedRaBasic();
	afx_msg void OnClickedRaHorizonline();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClickedRaLine();
	afx_msg void OnSetFocusList();
	//}}AFX_MSG
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	void DelAllDlg(CWnd* pWnd = NULL);	
	UINT CheckRadio(int nRadio = 0);
	void CreateColor(char kind, struct _line* line);
	CRect		m_rcFrame;

	struct	_graph*		m_pGraph;
	struct	_indiInfo*	m_pIndi;
	CHART_INFO*		m_pChart;

	

	CIndiBase*	m_pBaseDlg;
	CIndiHorizon*	m_pHoriDlg;
	CIndiDisp*	m_pDispDlg;
	bool		m_bEach;

	bool		m_bDestroy;
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_INDIOBJ_H__D0E54135_E8CE_4C8F_981B_4EF8704EBD16__INCLUDED_)
