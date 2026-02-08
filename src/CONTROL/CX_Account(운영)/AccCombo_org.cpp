// AccCombo.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Account.h"
#include "AccCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccCombo

CAccCombo::CAccCombo()
	: m_bGroup(FALSE)
	, m_bGroupList(TRUE)
	, m_nGroupKey(-1)
{
}

CAccCombo::~CAccCombo()
{
}


BEGIN_MESSAGE_MAP(CAccCombo, CComboBox)
	//{{AFX_MSG_MAP(CAccCombo)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccCombo message handlers
