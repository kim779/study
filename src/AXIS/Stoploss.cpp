// Stoploss.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "Stoploss.h"
#include "../dll/sm/Dbar2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStoploss dialog


CStoploss::CStoploss(class CDbar2 *pBar2 /*= NULL*/, CWnd* pParent /*=NULL*/)
	: CDialog(CStoploss::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStoploss)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pBar2 = pBar2;
}


void CStoploss::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStoploss)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStoploss, CDialog)
	//{{AFX_MSG_MAP(CStoploss)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STOPMSG, OnStopMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStoploss message handlers

BOOL CStoploss::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CStoploss::OnStopMsg(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case SK_STOP_RUNNING:
		m_pBar2->SetRunningStop(HIWORD(wParam), lParam);	break;
	}
	return 0;
}
