// XTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "XTabCtrl.h"
#include "TabDlg.h"
#include "./define/ChartMsgDef.h"
#include "StdDialog.h"
#include "DlgSysEdu.h"
#include "../../inc/ICompanyMng.h"			// 시스템트레이딩 교육여부 체크루틴. IsSystemChart()에서 사용
#include "../../inc/IMainInfoMng.h"
#include "../../inc/IKSLibMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT RMSG_OPENSCREEN = ::RegisterWindowMessage(_T("RMSG_OPENSCREEN"));

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

	BOOL bExtraChart = pParent->IsChartScreen("OldFuture");

	if(pParent->IsSystemChart())
	{
		for(int i=0;i<STOCK_TABTOTALCNT + 1;i++)
		{
			m_tabDlgCreator.GetCaptionOfTabDlg(i,szCaption);
			item.pszText = szCaption;
			InsertItem(i, &item, EXTAB_02);
		}
	}
	else if(bExtraChart)
	{
		for(int i=1;i<STOCK_TABTOTALCNT;i++)
		{
			m_tabDlgCreator.GetCaptionOfTabDlg(i,szCaption);
			item.pszText = szCaption;
			InsertItem(i, &item, EXTAB_02);
		}
	}
	// 상품선물옵션차트 - ojtaso (20071001)
//@유진변경
//	else if(m_nMarketType == STOCK_CHART || m_nMarketType == FUTOPT_CHART || m_nMarketType == UPJONG_CHART || m_nMarketType == COMMODITY_CHART)
	else if(   m_nMarketType == CMasterTypeInfo::STOCK_CHART 
			|| m_nMarketType == CMasterTypeInfo::FUTOPT_CHART 
			|| m_nMarketType == CMasterTypeInfo::UPJONG_CHART 
			|| m_nMarketType == CMasterTypeInfo::FOREIGN_CHART
			|| m_nMarketType == CMasterTypeInfo::FUOPT_JIPYO_CHART
			|| m_nMarketType == CMasterTypeInfo::COMMODITY_CHART
			|| m_nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART
			)
//@유진변경
	{
		for(int i=0;i< STOCK_TABTOTALCNT ;i++)
		{
			m_tabDlgCreator.GetCaptionOfTabDlg(i,szCaption);
			item.pszText = szCaption;
			InsertItem(i, &item, EXTAB_02);
		}

//		if(m_pTabDlgCodeSearch == NULL)
//		{
//			m_pTabDlgCodeSearch = m_tabDlgCreator.CreateTabDlg(TABNO_CODESEARCH);
//		}

	}
//	else if(m_nMarketType == FUTOPT_CHART)
//	{
//		//chart
//		m_tabDlgCreator.GetCaptionOfTabDlg(0,szCaption);
//		item.pszText = szCaption;
//		InsertItem(0, &item);
//
//		//보조지표
//		m_tabDlgCreator.GetCaptionOfTabDlg(1,szCaption);
//		item.pszText = szCaption;
//		InsertItem(1, &item);
//
//		//즐겨찾기
//		m_tabDlgCreator.GetCaptionOfTabDlg(4,szCaption);
//		item.pszText = szCaption;
//		InsertItem(2, &item);
//
////#ifdef _DEBUG
//		//시스템트레이딩
//		m_tabDlgCreator.GetCaptionOfTabDlg(TABNO_SYSTEMTRADING,szCaption);
//		item.pszText = szCaption;
//		InsertItem(3, &item);
////#endif
//		//시스템
//		//m_tabDlgCreator.GetCaptionOfTabDlg(7,szCaption);
//		//item.pszText = szCaption;
//		//InsertItem(3, &item);
//
//	}
//	else if(m_nMarketType == UPJONG_CHART)
//	{
//		//chart
//		m_tabDlgCreator.GetCaptionOfTabDlg(0,szCaption);
//		item.pszText = szCaption;
//		InsertItem(0, &item);
//
//		//보조지표
//		m_tabDlgCreator.GetCaptionOfTabDlg(1,szCaption);
//		item.pszText = szCaption;
//		InsertItem(1, &item);
//
//		//즐겨찾기
//		m_tabDlgCreator.GetCaptionOfTabDlg(4,szCaption);
//		item.pszText = szCaption;
//		InsertItem(2, &item);
//
////#ifdef _DEBUG		
//		//시스템트레이딩
//		m_tabDlgCreator.GetCaptionOfTabDlg(TABNO_SYSTEMTRADING,szCaption);
//		item.pszText = szCaption;
//		InsertItem(3, &item);
////#endif
//	}
/*
	else if (m_nMarketType == FOREIGN_CHART)
	{
		//chart
		m_tabDlgCreator.GetCaptionOfTabDlg(TABNO_ASSISTINDEX,szCaption);
		item.pszText = szCaption;
		InsertItem(0, &item);
	}
*/
}

void CXTabCtrl::DeleteCurrentDlg()
{
	if(m_pTabDlgFocused)
	{
		if( m_pTabDlgFocused->GetSafeHwnd() )
			m_pTabDlgFocused->DestroyWindow();		
		delete m_pTabDlgFocused;
		m_pTabDlgFocused = NULL;	
	}
}

void CXTabCtrl::ShowTab(int nIndex)
{
	{
		CStdDialog* pParent = (CStdDialog*)GetParent()->GetParent();
		
		BOOL bExtraChart = pParent->IsChartScreen("OldFuture");
		if(bExtraChart)
			nIndex = TABNO_ASSISTINDEX;
	}

	if(nIndex==TABNO_SYSTEMTRADING)
	{
//		CStdDialog* pParent = (CStdDialog*)GetParent()->GetParent();
//		if(!pParent->IsSystemTradingUser())
//		{
//			CString strMessage = _T("시스템트레이딩 사용신청을 하지 않았습니다. 신청하시겠습니까?");
//			int nID = MessageBox(strMessage, _T("시스템트레이딩"), MB_YESNO);
//			if(nID == IDYES)
//				pParent->OpenScreen(_T("6702"));
//		
//			SetCurSel(0);
//			return;
//		}
	}

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

			STTabColor tabColor;
			CStdDialog* pStdDialog = (CStdDialog*)GetParent()->GetParent();
			if((COLORREF)pStdDialog->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)1, (LPARAM)&tabColor))
			{
				if(m_pTabDlgCodeSearch)		m_pTabDlgCodeSearch->ChangeSkinColor(tabColor.clrTabBkgrnd);
			}
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
		if(m_pTabDlgCodeSearch && m_pTabDlgCodeSearch->GetSafeHwnd())
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
		if( m_pTabIndices && m_pTabIndices->GetSafeHwnd() )
		{
			m_pTabIndices->SetWindowPos(NULL, x, y , rect.right - x, rect.bottom - y,
				SWP_FRAMECHANGED| SWP_NOZORDER);
			m_pTabIndices->ShowWindow(SW_SHOW);
		}
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
		if(m_nMarketType == CMasterTypeInfo::UPJONG_CHART&&nIndex==TABNO_CODESEARCH)
			m_pTabDlgFocused = m_tabDlgCreator.CreateTabDlg(TABNO_CODESEARCHUP);
		else if(m_nMarketType == CMasterTypeInfo::FUTOPT_CHART&&nIndex==TABNO_CODESEARCH)
			m_pTabDlgFocused = m_tabDlgCreator.CreateTabDlg(TABNO_CODESEARCHFO);
		//101 2006.9.14 {{
		else if(m_nMarketType == CMasterTypeInfo::FOREIGN_CHART&&nIndex==TABNO_CODESEARCH)
			m_pTabDlgFocused = m_tabDlgCreator.CreateTabDlg(TABNO_CODESEARCHFR);
//@유진삭제
//		else if(m_nMarketType == ELW_CHART&&nIndex==TABNO_CODESEARCH)
//			m_pTabDlgFocused = m_tabDlgCreator.CreateTabDlg(TABNO_CODESEARCHELW);
//		else if(m_nMarketType == COMMODITY_CHART&&nIndex==TABNO_CODESEARCH)// 상품선물옵션차트 - ojtaso (20071001) - 2차개발에서 적용
//			m_pTabDlgFocused = m_tabDlgCreator.CreateTabDlg(TABNO_CODESEARCHCMM);
//@유진삭제
		//101 2006.9.14 }}
		else
			m_pTabDlgFocused = m_tabDlgCreator.CreateTabDlg(nIndex);

		if(m_pTabDlgFocused && m_pTabDlgFocused->GetSafeHwnd())
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
	if(m_pTabDlgFocused && m_pTabDlgFocused->GetSafeHwnd())
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
		if( m_pTabDlgCodeSearch->GetSafeHwnd() )
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
//
//	COLORREF clrTabActive = RGB(193,180,130);
//	COLORREF clrTabInactive = RGB(189,189,189);
//@신영삭제
//	IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
//	if(pKSLibMng)
//	{
//		clrTabActive = pKSLibMng->Lib_KSLink_GetColor(135);		// 탭 색상 Active
//		clrTabInactive = pKSLibMng->Lib_KSLink_GetColor(98);	// 탭 색상 Inactive
//	}

		SetSTTabColor((long)&tabColor);
	}

//	SetTabColor(clrBkgrnd, clrTabActive, clrTabInactive);

	Invalidate();
}

void CXTabCtrl::SetWndSystemTabEx()
{
	if(m_pTabDlgSystemTabEx == NULL)
	{
		m_pTabDlgSystemTabEx = m_tabDlgCreator.CreateTabDlg(TABNO_SYSTEMTRADING);
	}
}

