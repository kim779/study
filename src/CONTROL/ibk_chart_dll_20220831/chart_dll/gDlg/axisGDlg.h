#if !defined(AFX_AXISGDLG_H__E94C608C_14C6_40A1_A39A_297D1BE16369__INCLUDED_)
#define AFX_AXISGDLG_H__E94C608C_14C6_40A1_A39A_297D1BE16369__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

class CAxisGDlgApp : public CWinApp
{
public:
	CAxisGDlgApp();
	virtual ~CAxisGDlgApp();

public:
	CArray<CString, CString>	m_arGroup;

private:
	struct _envInfo		m_envDefault;

	CIHMaps<CMapWordToPtr, int, class CHART_INFO*>	m_mapChartInfo;	
	CIHMaps<CMapWordToPtr, int, struct _graph*>	m_mapDefault;	
	CMap<int, int, _indiInfo*, _indiInfo*>		m_mapScreenInfo;
	

public:
	bool	IsMain(int nKind);
	bool	IsExist(int nKind);
	bool	IsPrice(int nKind);
	int	GetGroup(int nKind);
	int	GetKind(CArray<int, int>& arKind, bool bMain = false);
	void	InitChart();
	void	ReleaseChart();

	CString	GetTitle(int nKind);
	CString	GetGroupName(int nKind);
	CFont*	GetFont(CWnd *pView, int point, CString name, int bold, bool italic);

	class CHART_INFO*	GetChartObj(int nKind);
	struct _graph*		GetDefault(int nKind);
	struct _indiInfo*	GetIndiObj(int nKind);
	
	struct _envInfo*	GetEnvDefault() { return &m_envDefault; }

public:
	//{{AFX_VIRTUAL(CAxisGDlgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAxisGDlgApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_AXISGDLG_H__E94C608C_14C6_40A1_A39A_297D1BE16369__INCLUDED_)
