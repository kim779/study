// OptionsSheet.cpp : implementation file
//

#include "stdafx.h"
#include "OptionsSheet.h"

#include "../mainvar.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionsSheet

IMPLEMENT_DYNAMIC(COptionsSheet, CPropertySheet)

COptionsSheet::COptionsSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

COptionsSheet::COptionsSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_pBaseInfoPage = new CBaseInfoPage();
	m_pDefaultPage  = new CDefaultPage();
	m_pScriptPage   = new CScriptPage();

	AddPage(m_pBaseInfoPage);
	AddPage(m_pDefaultPage);
	AddPage(m_pScriptPage);

	m_psh.dwFlags &= ~(PSH_HASHELP);
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
}

COptionsSheet::~COptionsSheet()
{
	delete m_pBaseInfoPage;
	delete m_pDefaultPage;
	delete m_pScriptPage;
}


BEGIN_MESSAGE_MAP(COptionsSheet, CPropertySheet)
	//{{AFX_MSG_MAP(COptionsSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionsSheet message handlers

void COptionsSheet::SaveInfos()
{
	CString currentPath = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCURRENTPATH),
		sTmp;

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETGAP, m_pBaseInfoPage->m_nGap);	// set gap
	
	WritePrivateProfileString(ENVIRONMENT, "INITMAP", m_pBaseInfoPage->m_mapPath, currentPath + "\\workshop.ini");

	WritePrivateProfileString(ENVIRONMENT, "FONTNAME", m_pDefaultPage->m_fontname, currentPath + "\\workshop.ini");
	WritePrivateProfileString(ENVIRONMENT, "FONTPNT", m_pDefaultPage->m_fontpnt, currentPath + "\\workshop.ini");

	sTmp.Format("%ld", MAKEWPARAM(m_pDefaultPage->m_upRGB, m_pDefaultPage->m_dnRGB));
	WritePrivateProfileString(ENVIRONMENT, "CORGB", sTmp, currentPath + "\\workshop.ini");

	sTmp.Format("%d", m_pDefaultPage->m_clrTabPos);
	WritePrivateProfileString(ENVIRONMENT, "CLRTABPOS", sTmp, currentPath + "\\workshop.ini");

	sTmp.Format("%d", m_pBaseInfoPage->m_nDisplayMode);
	WritePrivateProfileString(ENVIRONMENT, "DISPLAYMODE", sTmp, currentPath + "\\workshop.ini");

	sTmp.Format("%d", m_pScriptPage->m_clBkGnd);
	WritePrivateProfileString("SCRIPT", "CLRBKGND", sTmp, currentPath + "\\workshop.ini");
	sTmp.Format("%d", m_pScriptPage->m_clText);
	WritePrivateProfileString("SCRIPT", "CLRNORMAL", sTmp, currentPath + "\\workshop.ini");
	sTmp.Format("%d", m_pScriptPage->m_clSymbol);
	WritePrivateProfileString("SCRIPT", "CLRSYMBOL", sTmp, currentPath + "\\workshop.ini");
	sTmp.Format("%d", m_pScriptPage->m_clComment);
	WritePrivateProfileString("SCRIPT", "CLRCOMMENT", sTmp, currentPath + "\\workshop.ini");
	sTmp.Format("%d", m_pScriptPage->m_clConstant);
	WritePrivateProfileString("SCRIPT", "CLRCONSTANT", sTmp, currentPath + "\\workshop.ini");
	sTmp.Format("%d", m_pScriptPage->m_clKeyword);
	WritePrivateProfileString("SCRIPT", "CLRKEYWORD", sTmp, currentPath + "\\workshop.ini");
	sTmp.Format("%d", m_pScriptPage->m_clString);
	WritePrivateProfileString("SCRIPT", "CLRSTRING", sTmp, currentPath + "\\workshop.ini");
}
