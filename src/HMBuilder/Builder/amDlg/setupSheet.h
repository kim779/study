#pragma once

#include "baseInfoPage.h"
#include "defaultPage.h"
#include "scriptPage.h"

// CSetupSheet

class AFX_EXT_CLASS CSetupSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSetupSheet)

public:
	CSetupSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSetupSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSetupSheet();

	CBaseInfoPage*	m_basePage;
	CDefaultPage*	m_defaultPage;
	CScriptPage*	m_scriptPage;

	void	Save();

protected:
	DECLARE_MESSAGE_MAP()
};


