// DlgChartInvertSetting.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "DlgChartInvertSetting.h"

#include "ChartLink.h"							// for CChartLink

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgChartInvertSetting dialog


CDlgChartInvertSetting::CDlgChartInvertSetting( CChartLink *pBlockItem, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChartInvertSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChartInvertSetting)
	//}}AFX_DATA_INIT

	m_pBlockItem = pBlockItem;
}


void CDlgChartInvertSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChartInvertSetting)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChartInvertSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgChartInvertSetting)
	ON_BN_CLICKED(IDC_BTN_RESET, OnBtnReset)
	ON_BN_CLICKED(IDC_BTN_REVERSE, OnBtnReverse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChartInvertSetting message handlers

void CDlgChartInvertSetting::OnBtnReset() 
{
	// TODO: Add your control notification handler code here
	if( !m_pBlockItem) return;
	if( !m_pBlockItem->m_Child) return;

	CChartLink *pVScaleItem = m_pBlockItem->m_Child;
	pVScaleItem->m_bIsInvert = FALSE;
	while( pVScaleItem->m_Next)
	{
		pVScaleItem = pVScaleItem->m_Next;
		pVScaleItem->m_bIsInvert = FALSE;
	}
}

void CDlgChartInvertSetting::OnBtnReverse() 
{
	// TODO: Add your control notification handler code here
	
	if( !m_pBlockItem) return;
	if( !m_pBlockItem->m_Child) return;
	CChartLink *pVScaleItem = m_pBlockItem->m_Child;
	pVScaleItem->m_bIsInvert = TRUE;
	while( pVScaleItem->m_Next)
	{
		pVScaleItem = pVScaleItem->m_Next;
		pVScaleItem->m_bIsInvert = TRUE;
	}
}
