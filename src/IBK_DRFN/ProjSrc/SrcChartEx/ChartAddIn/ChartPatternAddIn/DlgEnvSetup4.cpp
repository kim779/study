// DlgEnvSetup4.cpp : implementation file
//

#include "stdafx.h"
#include "DlgEnvSetup4.h"

#include "PatternDefine.h"
#include "Include/ColorTable.h"
#include "CandlePattern.h"
#include <windowsx.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetup4 dialog


CDlgEnvSetup4::CDlgEnvSetup4(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEnvSetup4::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgEnvSetup4)
	//}}AFX_DATA_INIT

	m_pPatternEnvData = NULL;


	m_brBack.CreateSolidBrush( COLOR_BG_ANLYS_DLG);		// Dialog BG Color
	m_brWhite.CreateSolidBrush(COLOR_WHITE);
}


void CDlgEnvSetup4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEnvSetup4)
	DDX_Control(pDX, IDC_CHK_DISPLAYNAME, m_chkDisplayName);
	DDX_Control(pDX, IDC_TREE_ITEMLIST, m_treeItem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEnvSetup4, CDialog)
	//{{AFX_MSG_MAP(CDlgEnvSetup4)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ALLCHECK, OnButtonAllcheck)
	ON_BN_CLICKED(IDC_BUTTON_ALLUNCHECK, OnButtonAlluncheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEnvSetup4 message handlers


BOOL CDlgEnvSetup4::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetPatternEnvDataToControl();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgEnvSetup4::SetPatternEnvData(CPatternEnvData *pPatternEnvData)
{
	if ( pPatternEnvData)
	{
		m_pPatternEnvData = pPatternEnvData;
	}
}

void CDlgEnvSetup4::SetPatternEnvDataToControl()
{
	if ( m_pPatternEnvData)
	{
		// 추세패턴
		m_chkDisplayName.SetCheck(m_pPatternEnvData->m_bDrawPatternName);

		// Check Tree Init
		m_treeItem.ModifyStyle(0, WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER | TVS_DISABLEDRAGDROP | 
			TVS_HASBUTTONS | TVS_NOTOOLTIPS | 
			TVS_CHECKBOXES);
		//	m_treeItem.ModifyStyle(TVS_CHECKBOXES,0);
		//	m_treeItem.ModifyStyle(0,TVS_CHECKBOXES);

		HTREEITEM hFirst=0;

		CCandlePattern candlePattern;
		CString strName;
		HTREEITEM hItem = NULL;
		int nItem=0;

		//for(int i = CPTN_HAMMER; i<=CPTN_BULLISH_DRAGONFLY; i++)
		for(int i = CPTN_HAMMER; i<=CPTN_THRUSTINGLINE; i++)
		{
			strName = candlePattern.GetCandlePatternName(i);
			hItem = m_treeItem.InsertItem(strName);	

			if(!hFirst) hFirst = hItem; 
			m_treeItem.SetItemData(hItem,nItem);		
			if(m_pPatternEnvData->IsDisplayCandle(i))
				m_treeItem.SetCheck(hItem,TRUE);		
			else 
				m_treeItem.SetCheck(hItem,FALSE);
			nItem++;
		}
		//m_treeItem.SelectItem(hFirst);
		UpdateData(FALSE);
	}
}

void CDlgEnvSetup4::OnOK() 
{
	// TODO: Add extra validation here
	if ( m_pPatternEnvData)	
	{
		UpdateData(TRUE);
		m_pPatternEnvData->m_bDrawPatternName = m_chkDisplayName.GetCheck();
		
		m_pPatternEnvData->m_arrNotDisplayCandleItems.RemoveAll();
		HTREEITEM hNextItem = m_treeItem.GetRootItem();
		//HTREEITEM hNextItem;
		int nIdx=0;
		while (hNextItem != NULL)
		{
			
			BOOL bCheck = m_treeItem.GetCheck(hNextItem);
			if(!bCheck)
			{
				m_pPatternEnvData->m_arrNotDisplayCandleItems.Add(nIdx + CPTN_HAMMER);
			}		
			hNextItem = m_treeItem.GetNextItem(hNextItem, TVGN_NEXT);
			//hChildItem = hNextItem;
			nIdx++;
		}
	}

	//CDialog::OnOK();
}


HBRUSH CDlgEnvSetup4::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
//	pDC->SetBkMode( TRANSPARENT);

	if ( nCtlColor == CTLCOLOR_EDIT)
	{
		pDC->SetBkColor( COLOR_WHITE);

		hbr = m_brWhite;
	}
	else
	{
		pDC->SetBkColor( COLOR_BG_ANLYS_DLG);
		hbr = m_brBack;
	}
		
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgEnvSetup4::OnDestroy() 
{
	// Delete Brush
	if ( m_brBack.GetSafeHandle())	m_brBack.DeleteObject();
	if ( m_brWhite.GetSafeHandle())	m_brWhite.DeleteObject();


	CDialog::OnDestroy();

}

void CDlgEnvSetup4::OnButtonAllcheck() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hNextItem = m_treeItem.GetRootItem();
	int nIdx=0;
	while (hNextItem != NULL)
	{
			
		m_treeItem.SetCheck(hNextItem, TRUE);
		hNextItem = m_treeItem.GetNextItem(hNextItem, TVGN_NEXT);
		nIdx++;
	}

}

void CDlgEnvSetup4::OnButtonAlluncheck() 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hNextItem = m_treeItem.GetRootItem();
	int nIdx=0;
	while (hNextItem != NULL)
	{
			
		m_treeItem.SetCheck(hNextItem, FALSE);
		hNextItem = m_treeItem.GetNextItem(hNextItem, TVGN_NEXT);
		nIdx++;
	}
}

LRESULT CDlgEnvSetup4::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if(message == WM_NOTIFY)
//	{
//	   LPNMHDR lpnmh = (LPNMHDR) lParam;
//	   TVHITTESTINFO ht = {0};
//    
//	   if((lpnmh->code  == NM_CLICK) && (lpnmh->idFrom == IDC_TREE_ITEMLIST))
//	   {
//		  DWORD dwpos = GetMessagePos();
//
//		  // include <windowsx.h> and <windows.h> header files
//		  ht.pt.x = GET_X_LPARAM(dwpos);
//		  ht.pt.y = GET_Y_LPARAM(dwpos);
//		  CWnd::MapWindowPoints(&m_treeItem, &ht.pt, 1);
//
//		  TreeView_HitTest(lpnmh->hwndFrom, &ht);
//         
//		  if(TVHT_ONITEMSTATEICON & ht.flags)
//		  {
//			  m_treeItem.SelectItem(ht.hItem);
//			  AfxMessageBox("TEST");
//			  return FALSE;
//			
//			 //PostMessage(hWnd, UM_CHECKSTATECHANGE, 0, (LPARAM)ht.hItem);
//		  }
//	   }
//	}
	return CDialog::WindowProc(message, wParam, lParam);
}
