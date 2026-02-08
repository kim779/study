#if !defined(AFX_INDIHORIZON_H__0C13028E_CD0D_44F3_91E7_C27976FBE830__INCLUDED_)
#define AFX_INDIHORIZON_H__0C13028E_CD0D_44F3_91E7_C27976FBE830__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIndiHorizon : public CDialog
{
public:
	CIndiHorizon(CWnd* pParent = NULL);
public:


protected:
	struct	_graph*		m_pGraph;
	struct	_indiInfo*	m_pIndi;
	
	struct _listtemp
	{
		float	value;
		struct	_line	line;
		BYTE	btFillOver;		// added by schnag 2006.03.23
	};
	CIHArray<struct _listtemp*, struct _listtemp*>	m_arListinfo;


public:
	void	SetScreen(struct _graph* pGraph);
	void	SaveAll();	

protected:
	//void	SetInfo(struct _line* pline, BYTE btFillOver);
	void	SetInfo(struct _listtemp* plt);
	void	RefreshCheck();
	void	SetCheckBit(int nIndex, bool bCheck);
	CString	MakeString(CString szString);
	CString	MakeString(double fVal);


public:
	//{{AFX_DATA(CIndiHorizon)
	enum { IDD = IDD_INDI_HORI1 };
	CComboBox	m_cbFillOver;
	CaxdoubleEdit	m_input;
	CLineListBox	m_list;
	CWidthCtrl	m_width;
	CKindCtrl	m_kind;
	CColorCtrl	m_color;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CIndiHorizon)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CIndiHorizon)
	afx_msg void OnClickedBtAdd();
	afx_msg void OnClickedBtModify();
	afx_msg void OnClickedBtDel();
	afx_msg void OnClickedBtDelall();
	afx_msg void OnClickedChkGoUserline();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnControls(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeComboOver();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_INDIHORIZON_H__0C13028E_CD0D_44F3_91E7_C27976FBE830__INCLUDED_)
