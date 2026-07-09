#pragma once

#include "resource.h"
// CDSetupDlg dialog
#include "SetupDialog.h"
#include "XListCtrl.h"

class AFX_EXT_CLASS CDSetupDlg : public CSetupDialog
{
//	DECLARE_DYNAMIC(CDSetupDlg)

public:
	CDSetupDlg(CWnd* pParent = NULL);   // standard constructor
	CDSetupDlg(class CTMenu* menu, CString home, CString user, CWnd* pParent = NULL);  
	
	virtual ~CDSetupDlg();

// Dialog Data
	enum { IDD = IDD_DSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CString	m_home;
	CString	m_user;
	int	m_iselectlist;
public:
	virtual BOOL OnInitDialog();
	CImageList *m_pImageList;
	//CXListCtrl m_list;
	CListCtrl m_list;

	void  Init();
	void	ApplySetup();
	void	DefaultSetup();
	void	save_info();
	void	SetListItem();
	CString Parser(CString &srcstr, CString substr);

	void InitList(CListCtrl &list, const TCHAR**pszTitle);
	void ResizeListColumn(CXListCtrl &list, const int *arrWidth);
	afx_msg void OnBnClickedDbtnPlay();
	CComboBox m_cbFsize;
	CComboBox m_cbMaxMap;
	CComboBox m_cbSoundFunc;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
//	afx_msg void OnItemchangedDlistSound(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedDlistSound(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangeDcbSoundfunc();
	afx_msg void OnDblclkDlistSound(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
