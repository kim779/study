// IndicatorGroupButton.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "IndicatorGroupButton.h"

#include "GroupButtonCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIndicatorGroupButton

CIndicatorGroupButton::CIndicatorGroupButton()
{
	m_pGroupButtonCtrl = NULL;
	m_nIndex = 0;
}

CIndicatorGroupButton::CIndicatorGroupButton(CGroupButtonCtrl* pGroupButtonCtrl, int nIndex)
{
	m_pGroupButtonCtrl = pGroupButtonCtrl;
	m_nIndex = nIndex;
}

CIndicatorGroupButton::~CIndicatorGroupButton()
{
}


BEGIN_MESSAGE_MAP(CIndicatorGroupButton, CButton)
	//{{AFX_MSG_MAP(CIndicatorGroupButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIndicatorGroupButton message handlers

void CIndicatorGroupButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_pGroupButtonCtrl == NULL)
		return;

	m_pGroupButtonCtrl->OnGroupButtonDown(m_nIndex);	
//	CButton::OnLButtonDown(nFlags, point);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  OnLButtonDblClk
 - Created at  :  2004-03-30   17:36
 - Author      :  최종찬
 - Description :  Double Click을 막는다.
 -----------------------------------------------------------------------------------------*/
void CIndicatorGroupButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
//	CButton::OnLButtonDblClk(nFlags, point);
}
