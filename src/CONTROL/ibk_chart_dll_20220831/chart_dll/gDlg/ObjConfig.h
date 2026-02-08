#if !defined(AFX_OBJCONFIG_H__15E0BA7C_01DA_4BD4_8FDB_D8D835C732E7__INCLUDED_)
#define AFX_OBJCONFIG_H__15E0BA7C_01DA_4BD4_8FDB_D8D835C732E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObjConfig : public CDialog
{
public:
	CObjConfig(CWnd* pMain, COLORREF tkCOLOR, char* envinfo, char* graphtotal, char* graphinfo, CWnd* pParent = NULL);

	//{{AFX_DATA(CObjConfig)
	enum { IDD = IDD_EACH_GRAPH };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CObjConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CObjConfig)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnOk();
	afx_msg void OnBtDefault();
	afx_msg void OnBtCustomSave();
	afx_msg void OnBtCustomLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CWnd*	m_pMain;
	CWnd*	m_pChild;
	struct	_graph*	m_pGraph;
	struct	_graph	m_tmpGraph;
	char*	m_pEnv;
	COLORREF	m_tkCOLOR;
	struct	_graph*	m_pBong;
	CMap<int, int, struct _graph*, struct _graph*>	m_mapInfo;
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_OBJCONFIG_H__15E0BA7C_01DA_4BD4_8FDB_D8D835C732E7__INCLUDED_)
