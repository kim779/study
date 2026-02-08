#if !defined(AFX_SETAIDDLG_H__76419B35_B2CA_4800_96A7_519398C2B463__INCLUDED_)
#define AFX_SETAIDDLG_H__76419B35_B2CA_4800_96A7_519398C2B463__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetAidDlg.h : header file
//

#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CSetAidDlg dialog

class CSetAidDlg : public CDialog
{
// Construction
public:
	//CSetAidDlg(class CMyTool* pMyTool, char* pcInfo, CWnd* pParent = NULL);   // standard constructor
	CSetAidDlg(char* pcInfo, CWnd*, CWnd* pParent = NULL);   // standard constructor

public:
	

private:
	int	m_iPnAidInfoCount;
	
	CWnd*	m_pwndView;

	LOGFONT	m_logfont;
	LOGFONT	m_logfontS;

	CAxisGDlgApp*	m_pApp;

	class CMyTool*	m_pMyTool;
	class CExGrid	*m_pUsedGrid;

	struct _pnAidInfo*	m_pPnAidInfo;
	struct _aidConfig*	m_paidConfig;

	CMap<int, int, class CPnAidInfo*, class CPnAidInfo*>	m_mapPnAidInfo;

public:

private:
	void	InitTree();
	void	InitUsedGrid();
	void	InitBasic();
	void	RemoveBasic();

	void	AddUsedGrid();
	void	AddCell(class CfxGrid *pGrid, int iRow, int iCol, CString strText, LPARAM lParam);
	int	GetLastIndex();
	void	AddNewLine();

	void	ChangeRow(int iUpRow, int iDownRow);

public:
// Dialog Data
	//{{AFX_DATA(CSetAidDlg)
	enum { IDD = IDD_DIALOG_SET_AID };
	CTreeCtrl	m_tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetAidDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetAidDlg)
	virtual void OnOK();
	afx_msg void OnButtonDefault();
	virtual BOOL OnInitDialog();
	afx_msg void OnItemexpandedTreeIndicator(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedTreeIndicator(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonDelAll();
	afx_msg void OnDblclkTreeIndicator(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETAIDDLG_H__76419B35_B2CA_4800_96A7_519398C2B463__INCLUDED_)
