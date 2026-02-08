#if !defined(AFX_TOTALCFG_H__9BC03867_0E71_418C_B141_08B6BE2AA350__INCLUDED_)
#define AFX_TOTALCFG_H__9BC03867_0E71_418C_B141_08B6BE2AA350__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CTotalCfg : public CDialog
{
public:
	CTotalCfg(CWnd*	pMain, COLORREF tkCOLOR, char* envinfo, char* ginfo, char* scrinfo, CWnd* pParent = NULL);
	char*			m_pOutput;
	char*	GetData() { return m_pOutput; }
	//{{AFX_DATA(CTotalCfg)
	enum { IDD = IDD_TOTAL_CONFIG };
	CTabCtrl	m_tab;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTotalCfg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CTotalCfg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnOk();
	afx_msg void OnBtDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void setSel(int nIndex);
	CRect	m_rcFrame;
	CDialog*	m_pChild;
	int		m_nCurSel;

	CWnd*			m_pMain;
	char*			m_pInput[3];
	struct	_envInfo	m_tmpEnv;
	char*			m_pgraphinfo;
	int			m_nSize;
	COLORREF		m_tkCOLOR;

};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_TOTALCFG_H__9BC03867_0E71_418C_B141_08B6BE2AA350__INCLUDED_)
