// XTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "XTabCtrl.h"
#include "TabDlg.h"
#include "./define/ChartMsgDef.h"
#include "StdDialog.h"
#include "../../inc/ICompanyMng.h"			// 시스템트레이딩 교육여부 체크루틴. IsSystemChart()에서 사용
#include "../../inc/IMainInfoMng.h"
#include "../../inc/IKSLibMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CXTabCtrl

CXTabCtrl::CXTabCtrl()
{
	m_iSelectedTab = m_nPrevIndex = 0;
	m_ptTabs.x = 0;
	m_ptTabs.y = 0;

	m_bDown = FALSE;

	m_nType = 0;
	m_nRowCnt = 1;
	m_pTabDlgFocused = NULL;
	m_pTabDlgCodeSearch = NULL;
	m_pMultiChartWnd = NULL;
	m_pTabDlgSystemTabEx = NULL;	//@@@@@06.11.20
	m_pTabIndices = NULL;
}

CXTabCtrl::~CXTabCtrl()
{
	
}


BEGIN_MESSAGE_MAP(CXTabCtrl, CExTabCtrl)
	//{{AFX_MSG_MAP(CXTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelchanging)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTabCtrl message handlers

void CXTabCtrl::SetTopLeftCorner(CPoint pt)
{
	m_ptTabs.x = pt.x;
	m_ptTabs.y = pt.y;
}

void CXTabCtrl::Init(CWnd* pMultiChartWnd,CWnd *pChartMng, int nMarketType)
{	
	m_tabDlgCreator.Init(pMultiChartWnd, (CLeftBarDlg*)GetParent(),this,pChartMng);
	char szCaption[64];
	//int nCnt = m_tabDlgCreator.GetCntOfTabDlg();
	TCITEM item;
	item.mask = TCIF_TEXT|TCIF_PARAM|TCIF_IMAGE;
	item.lParam = NULL;
	item.pszText = _T("");
	item.iImage = 0;


	m_nMarketType = nMarketType;

	// 시스템트레이딩 교육이수 확인
	int nSysEdu = -1;
	ICompanyManager_SS* pMng = (ICompanyManager_SS*)AfxGetPctrInterface(UUID_ICompanyManager);
	if(pMng)
	{
		LPCSTR szMainKey = "SYSEDUE";
		nSysEdu = pMng->GetSysEdu(szMainKey, (long)this, (long)NULL, NULL);
	}

	CStdDialog* pParent = (CStdDialog*)GetParent()->GetParent();
	if(pParent->IsSystemChart())
	{
		for(int i=0;i<STOCK_TABTOTALCNT + 1;i++)
		{
			m_tabDlgCreator.GetCaptionOfTabDlg(i,szCaption);
			item.pszText = szCaption;
			InsertItem(i, &item, EXTAB_02);
		}
	}
	// 상품선물옵션차트 - ojtaso (20071001)
	else if(m_nMarketType == CMasterTypeInfo::STOCK_CHART || m_nMarketType == CMasterTypeInfo::FUTOPT_CHART || m_nMarketType == CMasterTypeInfo::UPJONG_CHART)
	{
		for(int i=0;i<STOCK_TABTOTALCNT;i++)
		{
			m_tabDlgCreator.GetCaptionOfTabDlg(i,szCaption);
			item.pszText = szCaption;
			InsertItem(i, &item, EXTAB_02);
		}

	}
	else if (m_nMarketType == CMasterTypeInfo::FOREIGN_CHART)
	{
		//chart
		m_tabDlgCreator.GetCaptionOfTabDlg(TABNO_ASSISTINDEX,szCaption);
		item.pszText = szCaption;
		InsertItem(0, &item);
	}
}

void CXTabCtrl::DeleteCurrentDlg()
{
	if(m_pTabDlgFocused)
	{
		m_pTabDlgFocused->DestroyWindow();		
		delete m_pTabDlgFocused;
		m_pTabDlgFocused = NULL;	
	}
}

void CXTabCtrl::ShowTab(int nIndex)
{
#ifdef _DEBUG
	if(nIndex==TABNO_SYSTEMTRADING)
	{
		IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
		if(pMng)
		{
			int nSysEdu=-1;
			gGetEugeneSysEdu(nSysEdu);
			if(nSysEdu<=0)
			{
				//CStdDialog* pParent = (CStdDialog*)GetParent()->GetParent();
				AfxMessageBox("시스템트레이딩교실에서 교육을 이수하셔야 사용이 가능합니다.");
				SetCurSel(m_iSelectedTab);
				return;
			}
		}
	}
#endif

	DeleteCurrentDlg();
	//SetCurSel(nIndex);

	if(/*m_nMarketType == STOCK_CHART&&*/nIndex==TABNO_CODESEARCH)// gm0604 modified 2005.11.13
	{
		if(m_pTabDlgCodeSearch && m_pTabDlgCodeSearch->IsWindowVisible())
			return;
		
		if(m_pTabDlgSystemTabEx && m_pTabDlgSystemTabEx->GetSafeHwnd()) m_pTabDlgSystemTabEx->ShowWindow(SW_HIDE);
		if(m_pTabIndices && m_pTabIndices->GetSafeHwnd()) m_pTabIndices->ShowWindow(SW_HIDE);
		if( (m_pTabDlgCodeSearch == NULL) ||
			(m_pTabDlgCodeSearch->GetSafeHwnd() == NULL)
		)
		{	
			m_pTabDlgCodeSearch = m_tabDlgCreator.CreateTabDlg(TABNO_CODESEARCH);
		}


		CRect rect;
		GetClientRect(rect);
		int x = m_ptTabs.x;
		int y = m_ptTabs.y+(m_nRowCnt-1)*GetTabHeight();
		//2005. 05. 17 -------------------------------------------------------
		//전체 Row수가 2개 이상이 되면 화면이 보이는 위치를 조절한다.
		//--------------------------------------------------------------------
		if(m_nRowCnt == 3)
			y += 8;
		else if(m_nRowCnt > 3)
			y += 10;
		//--------------------------------------------------------------------
		if(m_pTabDlgCodeSearch)
		{
			m_pTabDlgCodeSearch->SetWindowPos(NULL, x, y , rect.right - x, rect.bottom - y,
								SWP_FRAMECHANGED| SWP_NOZORDER);
			m_pTabDlgCodeSearch->ShowWindow(SW_SHOW);
		}
	}//@@@@@06.11.20 [[
	else if(nIndex==TABNO_SYSTEMTRADING)
	{
		if(m_pTabDlgCodeSearch && m_pTabDlgCodeSearch->GetSafeHwnd()) m_pTabDlgCodeSearch->ShowWindow(SW_HIDE);
		if(m_pTabIndices && m_pTabIndices->GetSafeHwnd()) m_pTabIndices->ShowWindow(SW_HIDE);

		if( (m_pTabDlgSystemTabEx == NULL) ||
			(m_pTabDlgSystemTabEx->GetSafeHwnd() == NULL) )
		{	
			m_pTabDlgSystemTabEx = m_tabDlgCreator.CreateTabDlg(TABNO_SYSTEMTRADING);

			STTabColor tabColor;
			CStdDialog* pStdDialog = (CStdDialog*)GetParent()->GetParent();
			if((COLORREF)pStdDialog->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)1, (LPARAM)&tabColor))
			{
				if(m_pTabDlgSystemTabEx)		m_pTabDlgSystemTabEx->ChangeSkinColor(tabColor.clrTabBkgrnd);
			}
		}
		CRect rect;
		GetClientRect(rect);
		int x = m_ptTabs.x - 2;
		int y = m_ptTabs.y+(m_nRowCnt-1)*GetTabHeight();
		//2005. 05. 17 -------------------------------------------------------
		//전체 Row수가 2개 이상이 되면 화면이 보이는 위치를 조절한다.
		//--------------------------------------------------------------------
		if(m_nRowCnt == 3)
			y += 8;
		else if(m_nRowCnt > 3)
			y += 10;
		//--------------------------------------------------------------------
		m_pTabDlgSystemTabEx->SetWindowPos(NULL, x, y , rect.right - x, rect.bottom - y,
			SWP_FRAMECHANGED| SWP_NOZORDER);
		m_pTabDlgSystemTabEx->ShowWindow(SW_SHOW);
	}
	//@@@@@06.11.20 ]]
	//@@@@@06.12.07 [[
	else if(nIndex==TABNO_ASSISTINDEX)
	{
		if(m_pTabDlgCodeSearch && m_pTabDlgCodeSearch->GetSafeHwnd()) m_pTabDlgCodeSearch->ShowWindow(SW_HIDE);
		if(m_pTabDlgSystemTabEx && m_pTabDlgSystemTabEx->GetSafeHwnd()) m_pTabDlgSystemTabEx->ShowWindow(SW_HIDE);

		if( (m_pTabIndices == NULL) ||
			(m_pTabIndices->GetSafeHwnd() == NULL) )
		{	
			m_pTabIndices = m_tabDlgCreator.CreateTabDlg(TABNO_ASSISTINDEX);

			STTabColor tabColor;
			CStdDialog* pStdDialog = (CStdDialog*)GetParent()->GetParent();
			if((COLORREF)pStdDialog->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)1, (LPARAM)&tabColor))
			{
				if(m_pTabIndices)		m_pTabIndices->ChangeSkinColor(tabColor.clrTabBkgrnd);
			}
		}
		CRect rect;
		GetClientRect(rect);
		int x = m_ptTabs.x - 2;
		int y = m_ptTabs.y+(m_nRowCnt-1)*GetTabHeight();
		//2005. 05. 17 -------------------------------------------------------
		//전체 Row수가 2개 이상이 되면 화면이 보이는 위치를 조절한다.
		//--------------------------------------------------------------------
		if(m_nRowCnt == 3)
			y += 8;
		else if(m_nRowCnt > 3)
			y += 10;
		//--------------------------------------------------------------------
		m_pTabIndices->SetWindowPos(NULL, x, y , rect.right - x, rect.bottom - y,
			SWP_FRAMECHANGED| SWP_NOZORDER);
		m_pTabIndices->ShowWindow(SW_SHOW);
	}
	//@@@@@06.12.07 ]]
	else
	{
		
		if(		m_pTabDlgCodeSearch != NULL &&
				m_pTabDlgCodeSearch->GetSafeHwnd()
		)
		{
			m_pTabDlgCodeSearch->ShowWindow(SW_HIDE);
		}
		//@@@@@06.11.20 [[
		if(		m_pTabDlgSystemTabEx != NULL &&
			m_pTabDlgSystemTabEx->GetSafeHwnd()
			)
		{
			m_pTabDlgSystemTabEx->ShowWindow(SW_HIDE);
		}
		//@@@@@06.11.20 ]]

		//@@@@@06.12.07 [[
		if(		m_pTabIndices != NULL &&
			m_pTabIndices->GetSafeHwnd()
			)
		{
			m_pTabIndices->ShowWindow(SW_HIDE);
		}
		//@@@@@06.12.07 ]]

		nIndex = ChangeTabIndexByMarketType(nIndex);
		if ((m_nMarketType==CMasterTypeInfo::UPJONG_CHART) && (nIndex==TABNO_CODESEARCH))
			m_pTabDlgFocused = m_tabDlgCreator.CreateTabDlg(TABNO_CODESEARCHUP);
		else if ((m_nMarketType==CMasterTypeInfo::FUTOPT_CHART) && (nIndex==TABNO_CODESEARCH))
			m_pTabDlgFocused = m_tabDlgCreator.CreateTabDlg(TABNO_CODESEARCHFO);
		else if ((m_nMarketType==CMasterTypeInfo::FOREIGN_CHART) && (nIndex==TABNO_CODESEARCH))
			m_pTabDlgFocused = m_tabDlgCreator.CreateTabDlg(TABNO_CODESEARCHFR);
		else
			m_pTabDlgFocused = m_tabDlgCreator.CreateTabDlg(nIndex);

		if(m_pTabDlgFocused)
		{
			CRect rect;
			GetClientRect(rect);
			int x = m_ptTabs.x;
			int y = m_ptTabs.y+(m_nRowCnt-1)*GetTabHeight();
			//2005. 05. 17 -------------------------------------------------------
			//전체 Row수가 2개 이상이 되면 화면이 보이는 위치를 조절한다.
			//--------------------------------------------------------------------
			if(m_nRowCnt == 3)
				y += 8;
			else if(m_nRowCnt > 3)
				y += 10;
			//--------------------------------------------------------------------
			m_pTabDlgFocused->SetWindowPos(NULL, x, y , rect.right - x, rect.bottom - y,
								SWP_FRAMECHANGED| SWP_NOZORDER);
			m_pTabDlgFocused->ShowWindow(SW_SHOW);
			
			CStdDialog* pParent = (CStdDialog*)GetParent()->GetParent();
			pParent->SetPermanentHandle(m_pTabDlgFocused);
		}
	}

}

void CXTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CExTabCtrl::OnSelchange(pNMHDR, pResult);
	int iNewTab = GetCurSel();	
	if(iNewTab<0) iNewTab=0;
	ShowTab(iNewTab);
	*pResult = 0;
}

void CXTabCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	// TODO: Add your control notification handler code here
	CExTabCtrl::OnSelchange(pNMHDR, pResult);
	m_nPrevIndex = m_iSelectedTab;
	m_iSelectedTab = GetCurSel();
	*pResult = 0;
}

void CXTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CExTabCtrl::OnSize(nType, cx, cy);
	
	m_nRowCnt = GetRowCount();

	int x = m_ptTabs.x;

	int y = m_ptTabs.y+(m_nRowCnt-1)*GetTabHeight();
	//2005. 05. 17 -------------------------------------------------------
	//전체 Row수가 2개 이상이 되면 화면이 보이는 위치를 조절한다.
	//--------------------------------------------------------------------
	if(m_nRowCnt == 3)
		y += 8;
	else if(m_nRowCnt > 3)
		y += 10;
	//--------------------------------------------------------------------
		
	cx = cx - x;
	cy = cy - y;
	if(m_pTabDlgFocused->GetSafeHwnd())
	{
		m_pTabDlgFocused->SetWindowPos(NULL,x, y,cx,cy,SWP_FRAMECHANGED | SWP_NOZORDER);
	}		
	if(m_pTabDlgCodeSearch)
	{
		if(m_pTabDlgCodeSearch->GetSafeHwnd())
		{
			m_pTabDlgCodeSearch->SetWindowPos(NULL,x, y,cx,cy,SWP_FRAMECHANGED | SWP_NOZORDER);
		}		
	}

	//@@@@@06.11.20[[
	if(m_pTabDlgSystemTabEx)
	{
		if(m_pTabDlgSystemTabEx->GetSafeHwnd())
		{
			m_pTabDlgSystemTabEx->SetWindowPos(NULL,x, y,cx,cy,SWP_FRAMECHANGED | SWP_NOZORDER);
		}		
	}
	//@@@@@06.11.20]]

	//@@@@@06.12.07[[
	if(m_pTabIndices)
	{
		if(m_pTabIndices->GetSafeHwnd())
		{
			m_pTabIndices->SetWindowPos(NULL,x, y,cx,cy,SWP_FRAMECHANGED | SWP_NOZORDER);
		}		
	}
	//@@@@@06.12.07]]
}

void CXTabCtrl::OnDestroy() 
{
	DeleteCurrentDlg();
	if(m_pTabDlgCodeSearch)
	{
		m_pTabDlgCodeSearch->DestroyWindow();
		delete m_pTabDlgCodeSearch;
		m_pTabDlgCodeSearch = NULL;
	}

	//@@@@@06.11.20[[
	if(m_pTabDlgSystemTabEx)
	{
		m_pTabDlgSystemTabEx->DestroyWindow();
		delete m_pTabDlgSystemTabEx;
		m_pTabDlgSystemTabEx = NULL;
	}
	//@@@@@06.11.20]]

	//@@@@@06.12.07[[
	if(m_pTabIndices)
	{
		m_pTabIndices->DestroyWindow();
		delete m_pTabIndices;
		m_pTabIndices = NULL;
	}
	//@@@@@06.12.07]]
	CExTabCtrl::OnDestroy();
}


// 각 탭의 고유번호는 이미 정해져 있어서 
// 차트 구분에 따라 보여주고 안보여주는 탭이 있어서
// 탭 고유번호로 내려오는 Index에 다른 화면이 
// 뜨는 현상이 발생하여 
// 바꾸어 해당 Tab에 맞는 Index를 리턴한다.
int CXTabCtrl::ChangeTabIndexByMarketType(int nIndex)
{
//	if(m_nMarketType == FUTOPT_CHART)
//	{
//		if(nIndex == 2)		return TABNO_MYFAVORITE;
//		else if(nIndex == 3) return TABNO_SYSTEMTRADING;// 2005. 11. 14 gm0604		
//	}
//	else if(m_nMarketType == UPJONG_CHART)
//	{
//		if(nIndex == 2)		return TABNO_MYFAVORITE;		
//		else if(nIndex == 3) return TABNO_SYSTEMTRADING;// 2005. 11. 14 gm0604				
//	}
//	else 
//	if (m_nMarketType == FOREIGN_CHART)
//	{
//		if(nIndex == 2)		return TABNO_MYFAVORITE;
//		else if(nIndex == 3) return TABNO_SYSTEMTRADING;// 2005. 11. 14 gm0604				
//	}
//	if (m_nMarketType == FOREIGN_CHART)
//	{
//		if(nIndex == 0)		return 1;
//		else if(nIndex == 1) return 2;// 2005. 11. 14 gm0604				
//	}
	
	return nIndex;
}

void CXTabCtrl::ChangeSkinColor(CWnd* pStdDialog)
{
	if(pStdDialog == NULL)
		return;

//	COLORREF clrBkgrnd = (COLORREF)pStdDialog->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
	STTabColor tabColor;
	if((COLORREF)pStdDialog->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)1, (LPARAM)&tabColor))
	{
		if(m_pTabDlgCodeSearch)		m_pTabDlgCodeSearch->ChangeSkinColor(tabColor.clrTabBkgrnd);
		if(m_pTabIndices)			m_pTabIndices->ChangeSkinColor(tabColor.clrTabBkgrnd);
		if(m_pTabDlgSystemTabEx)	m_pTabDlgSystemTabEx->ChangeSkinColor(tabColor.clrTabBkgrnd);

		SetSTTabColor((long)&tabColor);
	}
/*
	COLORREF clrTabActive = RGB(151,28,30);
	COLORREF clrTabInactive = RGB(209,209,209);
	SetTabColor(clrBkgrnd, clrTabActive, clrTabInactive);
*/
	Invalidate();
}

void CXTabCtrl::SetWndSystemTabEx()
{
	if(m_pTabDlgSystemTabEx == NULL)
	{
		m_pTabDlgSystemTabEx = m_tabDlgCreator.CreateTabDlg(TABNO_SYSTEMTRADING);
	}
}

