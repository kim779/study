#if !defined(AFX_LOADFRAMEDLG_H__0237A7C7_8F1D_4C1B_AE58_6C9EF569D0F5__INCLUDED_)
#define AFX_LOADFRAMEDLG_H__0237A7C7_8F1D_4C1B_AE58_6C9EF569D0F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoadFrameDlg : public CDialog
{
public:
	CLoadFrameDlg(CWnd* pMain, char* pOutput, int nSize, CWnd* pParent = NULL);

	//{{AFX_DATA(CLoadFrameDlg)
	enum { IDD = IDD_LOAD_FRAME };
	CAxListCtrl	m_list;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CLoadFrameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


protected:
	//{{AFX_MSG(CLoadFrameDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	char*	m_pOutput;
	int	m_nSize;
	CWnd*	m_pMain;
	CString	m_rootPath;
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_LOADFRAMEDLG_H__0237A7C7_8F1D_4C1B_AE58_6C9EF569D0F5__INCLUDED_)
