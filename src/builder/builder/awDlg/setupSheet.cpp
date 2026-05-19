// setupSheet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "setupSheet.h"

#include "../h/mainvar.h"
#include "../../h/axisvar.h"

// CSetupSheet

IMPLEMENT_DYNAMIC(CSetupSheet, CPropertySheet)

CSetupSheet::CSetupSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_basePage    = new CBaseInfoPage();
	m_defaultPage = new CDefaultPage();
	m_scriptPage  = new CScriptPage();

	AddPage(m_basePage);
	AddPage(m_defaultPage);
	AddPage(m_scriptPage);

	m_psh.dwFlags &= ~(PSH_HASHELP);
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
}

CSetupSheet::CSetupSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_basePage    = new CBaseInfoPage();
	m_defaultPage = new CDefaultPage();
	m_scriptPage  = new CScriptPage();

	AddPage(m_basePage);
	AddPage(m_defaultPage);
	AddPage(m_scriptPage);

	m_psh.dwFlags &= ~(PSH_HASHELP);
	m_psh.dwFlags |= PSH_NOAPPLYNOW;
}

CSetupSheet::~CSetupSheet()
{
	delete m_basePage;
	delete m_defaultPage;
	delete m_scriptPage;
}


BEGIN_MESSAGE_MAP(CSetupSheet, CPropertySheet)
END_MESSAGE_MAP()


// CSetupSheet 메시지 처리기입니다.

void CSetupSheet::Save()
{
	CString	path, tmps;
	
	path = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETCURRENTPATH),
	path += _T("\\workshop.ini");

	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETGAP, m_basePage->m_gap);	// set gap
	WritePrivateProfileString(ENVIRONMENT, _T("INITMAP"), m_basePage->m_maps, path);

	WritePrivateProfileString(ENVIRONMENT, _T("FONTNAME"), m_defaultPage->m_fontName, path);
	WritePrivateProfileString(ENVIRONMENT, _T("FONTPNT"), m_defaultPage->m_fontPoint, path);

	tmps.Format(_T("%ld"), MAKEWPARAM(m_defaultPage->m_upRGB, m_defaultPage->m_dnRGB));
	WritePrivateProfileString(ENVIRONMENT, _T("CORGB"), tmps, path);

	tmps.Format(_T("%d"), m_defaultPage->m_clrTabPos);
	WritePrivateProfileString(ENVIRONMENT, _T("CLRTABPOS"), tmps, path);

	tmps.Format(_T("%d"), m_basePage->m_displayMode);
	WritePrivateProfileString(ENVIRONMENT, _T("DISPLAYMODE"), tmps, path);

	tmps.Format(_T("%d"), m_scriptPage->m_clBkGnd);
	WritePrivateProfileString(_T("SCRIPT"), _T("CLRBKGND"), tmps, path);
	tmps.Format(_T("%d"), m_scriptPage->m_clText);
	WritePrivateProfileString(_T("SCRIPT"), _T("CLRNORMAL"), tmps, path);
	tmps.Format(_T("%d"), m_scriptPage->m_clSymbol);
	WritePrivateProfileString(_T("SCRIPT"), _T("CLRSYMBOL"), tmps, path);
	tmps.Format(_T("%d"), m_scriptPage->m_clComment);
	WritePrivateProfileString(_T("SCRIPT"), _T("CLRCOMMENT"), tmps, path);
	tmps.Format(_T("%d"), m_scriptPage->m_clConstant);
	WritePrivateProfileString(_T("SCRIPT"), _T("CLRCONSTANT"), tmps, path);
	tmps.Format(_T("%d"), m_scriptPage->m_clKeyword);
	WritePrivateProfileString(_T("SCRIPT"), _T("CLRKEYWORD"), tmps, path);
	tmps.Format(_T("%d"), m_scriptPage->m_clString);
	WritePrivateProfileString(_T("SCRIPT"), _T("CLRSTRING"), tmps, path);
}
