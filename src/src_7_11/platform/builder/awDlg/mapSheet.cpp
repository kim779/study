// mapSheet.cpp : implementation file
//

#include "stdafx.h"
#include "mapSheet.h"

#include "mapprop.h"
#include "mapoption.h"
#include "mapscript.h"

#include "../mapform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapSheet

IMPLEMENT_DYNAMIC(CMapSheet, CPropertySheet)

CMapSheet::CMapSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CMapSheet::CMapSheet(LPCTSTR pszCaption,  struct _mapH* mapH, CString dir,
				CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME)

//	m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_psh.dwFlags &= ~PSH_HASHELP;
//	m_psh.dwFlags |= PSH_USEHICON;
//	m_psh.hIcon = m_hIcon;

	m_prop   = new CMapProp(pParentWnd, mapH);
	m_option = new CMapOption(mapH);
	m_script = new CMapScript(mapH, dir);

	m_prop->m_psp.dwFlags   &= ~PSP_HASHELP;
	//m_option->m_psp.dwFlags &= ~PSP_HASHELP;
	m_script->m_psp.dwFlags &= ~PSP_HASHELP;

	AddPage(m_prop);
	//AddPage(m_option);
	AddPage(m_script);
}

CMapSheet::~CMapSheet()
{
	if (m_prop)	delete m_prop;
	//if (m_option)	delete m_option;
	if (m_script)	delete m_script;
}


BEGIN_MESSAGE_MAP(CMapSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CMapSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapSheet message handlers

BOOL CMapSheet::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	return CPropertySheet::OnCommand(wParam, lParam);
}
