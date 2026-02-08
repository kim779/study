#if !defined(AFX_SAVEFRAMEDLG_H__49D02885_065F_40EF_BA70_1A66C0A344E3__INCLUDED_)
#define AFX_SAVEFRAMEDLG_H__49D02885_065F_40EF_BA70_1A66C0A344E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSaveFrameDlg : public CDialog
{
public:
	CSaveFrameDlg(CWnd* pMain, char *pName, char *fList, char *Path, CWnd* pParent = NULL);

	int	GetState() { return m_nState; }

	//{{AFX_DATA(CSaveFrameDlg)
	enum { IDD = IDD_SAVE_FRAME };
	CEdit	m_FileName;
	CListBox	m_list;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CSaveFrameDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CSaveFrameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtSave();
	afx_msg void OnBtDel();
	afx_msg void OnBtLoad();
	afx_msg void OnDblclkList();
	afx_msg void OnSelchangeList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	char*		m_pName;
	CString		m_Path;
	CString		m_extention;
	CString		m_identity;
	CStringArray	m_arFileList;
	int		m_nSize;
	CWnd*		m_pMain;
	int		m_nWidth;
	CStringArray	m_arDelList;
	int		m_nState;		// 

	void	SetEditFocus();	
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_SAVEFRAMEDLG_H__49D02885_065F_40EF_BA70_1A66C0A344E3__INCLUDED_)
