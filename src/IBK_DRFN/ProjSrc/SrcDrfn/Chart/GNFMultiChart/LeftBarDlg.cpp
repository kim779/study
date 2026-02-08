// LeftBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LeftBarDlg.h"
#include "StdDialog.h"
#include "TabDlg.h"
//#include "RegFavoriteDlg.h"
#include "./define/ChartMsgDef.h"
//#include "WndMapView.h"
#include "FavoriteThingsMng.h"
//#include "../../../DataMgr/DBMgr.h" //05.09.27
//#include "../../../DrMLib/DrMiddleLib.h"//05.09.27

#define	WIDTH_SIDEBAR		8


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftBarDlg dialog
//extern IMainDllObj	* g_pMainDll;
//extern CWnd *g_pViewInMain;

void GetViewHandleForWndMapView(HWND &rHParentView, BOOL &rBUseSetTR)
{
	HWND hTemp11 = rHParentView; // view handle when 9001,9000.....
	HWND hTemp12 = ::GetParent(hTemp11);
	HWND hTemp13 = ::GetParent(hTemp12);
	HWND hTemp14 = ::GetParent(hTemp13);
	HWND hTemp15 = ::GetParent(hTemp14);	// view handle when 9020
	HWND hTemp16 = ::GetParent(hTemp15);	// view handle when 9020
	if(hTemp16)
	{
		CString strTitle;
		CWnd *pWnd = CWnd::FromHandle(hTemp16);
		pWnd->GetWindowText(strTitle);
		if(strTitle.Find("9020")!=-1)
		{
			rHParentView =  hTemp15;
			rBUseSetTR = TRUE;
		}		
	}
	rHParentView = hTemp11;
	rBUseSetTR = FALSE;
}


CLeftBarDlg::CLeftBarDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CLeftBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLeftBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
//	m_pctrlCode = NULL;
	m_pStdDialog = NULL;
	m_pChartMng = NULL;
	m_lJMaster = NULL;
	m_lKMaster = NULL;
	m_bEnable = FALSE;
//2005. 06. 08 by sy, nam===============================================================
//	m_pWndMapView = NULL;
	m_pFavoriteMng = NULL;
	m_bIgnoreShareData = FALSE;

	m_nID = 1;

//	m_pDBMgr = NULL; //2005. 05. 12 - CAny2Code 대체용...
	//m_pEzFinderLib = NULL;
}


void CLeftBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftBarDlg)
	//DDX_Control(pDX, IDC_BTHIDECHART, m_btHideChart);
	DDX_Control(pDX, IDC_TAB, m_tabctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeftBarDlg, CRscDlg)
	//{{AFX_MSG_MAP(CLeftBarDlg)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTHIDECHART, OnBthidechart)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftBarDlg message handlers

BOOL CLeftBarDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();	

	m_strRootPath = m_pStdDialog->m_strRootPath;
	m_strConfig = m_pStdDialog->m_strConfig;
	m_szImageDir = m_pStdDialog->m_szImageDir;

	//m_brBkColor.CreateSolidBrush(((CStdDialog*)GetParent())->GetSkinColorManager()->GetColor(20));
	m_clrBk = (COLORREF)GetParent()->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
	m_brBkColor.CreateSolidBrush(m_clrBk);	//RGB(222,222,222));
	CString strImageUp;
	CString strImageDown;

//	if(((CStdDialog*)GetParent())->m_nChartType == STOCK_CHART)
//	{
		m_tabctrl.Init(GetParent(),m_pChartMng, m_nMarketType);
		//if(((CStdDialog*)GetParent())->IsSystemChart())
		//	m_tabctrl.SetMinTabWidth(25);
//		m_tabctrl.SetMinTabWidth(5);
//		m_tabctrl.SetItemSize(CSize(10,20));	
//		m_tabctrl.SetSelectedColor(RGB(0, 0, 0), RGB(113,138,158) , RGB(114,139,159));
//		m_tabctrl.SetNormalColor(RGB(0, 0, 0));
//		m_tabctrl.SetTabBkColor(CRLEFTBARBACK);
		m_tabctrl.SetTopLeftCorner(CPoint(0,20));

		//SetTimer(200, 10, NULL);
//	}
//	else 
//	{
//		m_tabctrl.ShowWindow(SW_HIDE);		
//	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CLeftBarDlg::OnSize(UINT nType, int cx, int cy) 
{
	ModifyStyle(0, WS_CLIPCHILDREN);	// turn on CLIPCHILDREN
	CRscDlg::OnSize(nType, cx, cy);
//	UpdateWindow();
	// TODO: Add your message handler code here
	if(m_pStdDialog->m_nChartType==2)
	{
//2005. 06. 08 by sy, nam
//		if(m_pWndMapView)
//		{
//			m_pWndMapView->Invalidate();
//		}		
	}
	else
	{
		CRect rect;
		GetClientRect(rect);
		rect.top = 2;
		if(m_tabctrl.GetSafeHwnd())
		{
			m_tabctrl.MoveWindow(rect);
//			m_tabctrl.Invalidate();
		}
		
//		if(m_btHideChart.GetSafeHwnd())
//		{
//			rect.left = rect.right - 20;
//			rect.bottom = rect.top + 20;
//
//			m_btHideChart.MoveWindow(rect);
//		}
	}
	ModifyStyle(WS_CLIPCHILDREN, 0);	// turn off CLIPCHILDREN
}

void CLeftBarDlg::SetConfig(BOOL bLoad /*= TRUE*/)
{	
	CString strTemp;
	char szTemp[64];	

	if(bLoad)
	{
		::GetPrivateProfileString("LeftBar","TabPos","0",szTemp,sizeof(szTemp),m_strConfig);
		m_tabctrl.SetCurSel(atoi(szTemp));
		//m_tabctrl.ShowTab(atoi(szTemp));
	}
	else
	{
		strTemp.Format("%d",m_tabctrl.GetCurSel());
		::WritePrivateProfileString("LeftBar","TabPos",strTemp,m_strConfig);
	}
}

void CLeftBarDlg::OnDestroy() 
{
	// TODO: Add your message handler code here
//2005. 06. 08 by sy, nam===============================================================
//	if(m_pWndMapView)
//	{
//		m_pWndMapView->DestroyWindow();
//		delete m_pWndMapView;
//		m_pWndMapView = NULL;
//	}
	SetConfig(FALSE);

	//-----------------------------------------------------------------------------
	// Author		: Dae-Sung, Byun	Date :2005/7/21
	// Reason		: 조건검색 라이브러리 삭제
	//-----------------------------------------------------------------------------
	// start
/*	m_pEzFinderLib->Lib_Close();
	if (m_pEzFinderLib != NULL)
	{
		delete m_pEzFinderLib;
	}
	// end
*/
	m_brBkColor.DeleteObject();
	CRscDlg::OnDestroy();	
}

HRESULT CLeftBarDlg::ReceiveData(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID aTRData, long dwTRDateLen)
{
	CTabDlg *ptabDlg = (CTabDlg *)m_tabctrl.GetWndFocused();
	if(ptabDlg)
	{
		ptabDlg->ReceiveData( hTrComm,  szTR,  szMsgCode,  szMessage,  aTRData,  dwTRDateLen);
	}	
	ptabDlg = (CTabDlg *)m_tabctrl.GetWndCodeSearch();
	if(ptabDlg)
	{
		ptabDlg->ReceiveData( hTrComm,  szTR,  szMsgCode,  szMessage,  aTRData,  dwTRDateLen);
	}
	return NULL;
}

void CLeftBarDlg::GetIndicatorNameListIntoListCtrl(CXListCtrl& rxlistCtrl,LPCTSTR p_strGroup)
{
	//VERIFY(m_pChartMng);
	//VERIFY(m_pChartMng->GetSafeHwnd());

	if(m_pChartMng)
	{
		if(m_pChartMng->GetSafeHwnd())
		{
			m_pChartMng->GetIndicatorNameListIntoListCtrl(rxlistCtrl, p_strGroup);
		}
	}
	
}

void CLeftBarDlg::GetIndicatorNameListIntoTreeCtrl(CTreeCtrl& rxtreeCtrl)
{
	//Han Hwa ======================================
	if(
		(m_pChartMng == NULL) ||
		(m_pChartMng->GetSafeHwnd() == NULL) 
		)
		return;
	//Han Hwa ======================================

	//VERIFY(m_pChartMng);
	//VERIFY(m_pChartMng->GetSafeHwnd());
	if(m_pChartMng)
	{
		if(m_pChartMng->GetSafeHwnd())
		{
			m_pChartMng->GetIndicatorNameListIntoTreeCtrl(rxtreeCtrl);
		}		
	}
	
}

LRESULT CLeftBarDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == 78)
		TRACE("aaa");
	switch(message)
	{
//	case UMSG_SHOWREGFAVORITEDLG:
//		{
//			CRegFavoriteDlg dlg;
//			dlg.m_pPtTopLeftOfDlg = (CPoint*)wParam;
//			dlg.SetFavoriteThingsMng(m_pFavoriteMng, lParam);
//			dlg.DoModal();
//		}
//		break;
	case UMSG_SETINITIALCHARTDATA:// 차트의 기본초기값을 보낸다.
		{
			CWnd *pWnd = m_tabctrl.GetWndFocused();
			if(pWnd)
			{
				pWnd->SendMessage(message,wParam,lParam);
			}
			pWnd = (CTabDlg *)m_tabctrl.GetWndCodeSearch();
			if(pWnd)
			{
				pWnd->SendMessage(message,wParam,lParam);
			}			
//2005. 06. 08 by sy, nam===============================================================
//			if(m_pWndMapView)
//			{
//				stChartInitData* _pstData;
//				_pstData = (stChartInitData*)lParam;
//				if(!_pstData->m_strCode.IsEmpty()) 
//				{
//
////					STSHAREDDATA stData;
////					stData.m_szKey = "코드-코코2";
////					stData.m_szValue = _pstData->m_strCode;
////					m_pWndMapView->SetGShareData(&stData);				
//				}
//			}
//2005. 06. 08 by sy, nam===============================================================
		}
		break;
	case UMSG_SETCHARTHOGA:
		{
			CWnd *pWnd = m_tabctrl.GetWndFocused();
			if(pWnd)
			{
				pWnd->SendMessage(message,wParam,lParam);
			}	
			pWnd = (CTabDlg *)m_tabctrl.GetWndCodeSearch();
			if(pWnd)
			{
				pWnd->SendMessage(message,wParam,lParam);
			}			
//2005. 06. 08 by sy, nam===============================================================
//			if(m_pWndMapView)
//			{
//				m_pWndMapView->SetGShareData((LPVOID)lParam);				
//			}
		}
		break;
//@유진삭제	
//	case UMSG_SENDJCODECHECKED:
//	case UMSG_GETJCODECHECKEDFROMLEFTBAR:
//	case UMSG_SENDJCODEUNHIGHLIGHTED:	
//		{
//			CWnd *pWnd = m_tabctrl.GetWndFocused();
//			if(pWnd)
//			{
//				pWnd->SendMessage(message,wParam,lParam);
//			}	
//			pWnd = (CTabDlg *)m_tabctrl.GetWndCodeSearch();
//			if(pWnd)
//			{
//				pWnd->SendMessage(message,wParam,lParam);
//			}						
//		}
//		break;
//@유진삭제
	case UMSG_UPDATECODE:
		{
			GetParent()->SendMessage(UMSG_UPDATECODE,wParam,lParam);
		}
		break;
	case UMSG_GETFAVORITETHINGSMNG:
		{
			return (LRESULT)m_pFavoriteMng;
		}
		break;
	case UMSG_STOP_JIPYOCYCLE:
		{
			CWnd *pWnd = m_tabctrl.GetWndFocused();
			if(pWnd)
			{
				pWnd->SendMessage(message,wParam,lParam);
			}	
		}
		break;
	}
	return CRscDlg::WindowProc(message, wParam, lParam);
}

BOOL CLeftBarDlg::IsAdvisingCodeOf(LPCTSTR lpCode)
{
	return (BOOL)m_Codes.Find(lpCode);
}

void CLeftBarDlg::OnCodeAdvise(CString strCode)
{
	POSITION pos;
	// 기존 코드가 리스트에 없으면 추가
	if( ( pos = m_Codes.Find(strCode) ) == NULL )
	{
		m_Codes.AddTail(strCode);
		m_pStdDialog->OnCodeAdvise(strCode, FALSE);		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)	
	}	
}

void CLeftBarDlg::OnCodeUnAdvise(CString strCode)
{
	POSITION pos;
	// 기존 코드가 리스트에 있으면 삭제
	if( ( pos = m_Codes.Find(strCode) ) != NULL )
	{
		m_Codes.RemoveAt(pos);
		m_pStdDialog->OnCodeUnAdvise(strCode, FALSE);		// 복수종목 리얼등록 : 복수종목 - ojtaso (20070208)
	}	
}

void CLeftBarDlg::SetRealPriceData(void *pData)
{
	CTabDlg *ptabDlg = (CTabDlg *)m_tabctrl.GetWndFocused();
	if(ptabDlg)
	{
		ptabDlg->SetRealPriceData(pData);
	}	
	ptabDlg = (CTabDlg *)m_tabctrl.GetWndCodeSearch();
	if(ptabDlg)
	{
		ptabDlg->SetRealPriceData(pData);
	}
}


//DEL void CLeftBarDlg::SetpCtrlCode(CAny2Code* pctrlCode)
//DEL {
//DEL 	m_pctrlCode = pctrlCode;
//DEL 	m_lJMaster = m_pctrlCode->GetCodePointer("JMASTER");
//DEL 	m_lKMaster = m_pctrlCode->GetCodePointer("KMASTER");	
//DEL }

long CLeftBarDlg::GetCodeMasterArray(long lGubun, CStringArray& arCode, CStringArray& arName)
{
	/*=====================================================================
		-> 종목 코드로 종목 명을 찾는다.
	  	-> CODE_UPJONGSTOCK  : 장내/코스닥종목, All 업종   에서 종목을 찾는다.
				     : CODE_ALLUPJONG|CODE_LKJONG	

		-> CODE_ALLUPJONG    : 장내/KOSPI200/KOSDAQ 업종
		-> CODE_LKJONG	     : 장내/코스닥 겸용 
		-> CODE_JONGMOK	     : 상장 종목 ( 거래소 )
		-> CODE_KJONGMOK     : 코스닥 종목 
	========================================================================*/
//	if(m_pDBMgr)
//	{	
//		CStringArray arCode, arName;
//		m_pDBMgr->GetMasterData(lGubun, arCode, arName);
//		return 1;
//	}

	return 0;

/*	CString strMaster = lpMaster;
	if(strMaster=="JMASTER")
	{
		return m_lJMaster;
	}
	else if(strMaster=="KMASTER")
	{
		return m_lKMaster;		
	}
	else if(m_pctrlCode)
	{
		return m_pctrlCode->GetCodePointer(lpMaster);
	}
*/
	
}

//DEL long CLeftBarDlg::GetCodePointer(LPCTSTR lpMaster)
//DEL {
//DEL /*	CString strMaster = lpMaster;
//DEL 	if(strMaster=="JMASTER")
//DEL 	{
//DEL 		return m_lJMaster;
//DEL 	}
//DEL 	else if(strMaster=="KMASTER")
//DEL 	{
//DEL 		return m_lKMaster;		
//DEL 	}
//DEL 	else if(m_pctrlCode)
//DEL 	{
//DEL 		return m_pctrlCode->GetCodePointer(lpMaster);
//DEL 	}
//DEL */	
//DEL 	return 0;
//DEL }


int CLeftBarDlg::EzFinderLibGetFindDefine_TR(char *pFullFilePath, int nSize, int &nTrIndex, LPSTR &lpData)
{
//## Han Hwa	return m_pEzFinderLib->GetFindDefine_TR(pFullFilePath,  nSize, nTrIndex, lpData);
	return 0;
}


int CLeftBarDlg::EzFinderLibGetConditionList(int nType, BOOL bGetAlert, LPSTR &lpData, LPSTR &lpPath)
{
//## Han Hwa	return m_pEzFinderLib->GetConditionList(nType, bGetAlert, lpData, lpPath);
	return 0;
}

void CLeftBarDlg::SetPacketFromOutSide(CString strPacketFromOutSide)
{	
	m_strPacketFromOutSide.Empty();
	m_strPacketFromOutSide = strPacketFromOutSide;
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_RESETOCX);
	CTabDlg *pTabDlg = m_tabctrl.GetWndCodeSearch();
	pTabDlg->ReceiveDataFromOutSide();
	m_tabctrl.ShowTab(TABNO_CODESEARCH);
}

CString CLeftBarDlg::GetPacketFromOutSide()
{		
	return m_strPacketFromOutSide;
}

void CLeftBarDlg::StartSideBar()
{
	m_nSideBarIncrease = 0;
	SetTimer(300, 10, NULL);

	if(m_pStdDialog->m_btnSideBar.IsWindowVisible())
	{
		m_pStdDialog->m_btnSideBar.ShowWindow(SW_HIDE);
		m_pStdDialog->m_btnSideBar.Invalidate();
	}
}

void CLeftBarDlg::EndSideBar()
{
	SetTimer(400, 10, NULL);
}

void CLeftBarDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==200)
	{
		KillTimer(200); //m_ChartMng가 생성된후 Call하기위해 
		SetConfig();		
	}
	else if(nIDEvent == 300)
	{
		CRect rcChart;
		m_pStdDialog->GetWindowRect(rcChart);
		ScreenToClient(rcChart);

		CRect rcSideBar;
		rcSideBar.top = rcChart.top-3;
		rcSideBar.bottom = rcChart.bottom+3;
		rcSideBar.left = 0-3;//WIDTH_SIDEBAR;
		rcSideBar.right = rcSideBar.left + m_nSideBarIncrease;

		if(m_nSideBarIncrease < m_pStdDialog->GetWidthOfLeftBar())
		{
			int nInterval = m_pStdDialog->GetWidthOfLeftBar() - m_nSideBarIncrease;
			if(nInterval > 0 && nInterval < 30)
			{
				m_nSideBarIncrease += nInterval;
			}
			else
			{
				m_nSideBarIncrease += 30;
			}

			rcSideBar.right = rcSideBar.left + m_nSideBarIncrease;
			SetWindowPos(NULL, rcSideBar.left,rcSideBar.top,rcSideBar.Width(),rcSideBar.Height(), SWP_SHOWWINDOW);
			//MoveWindow(rcSideBar);
			//ShowWindow(SW_SHOW);
			SetTimer(300, 40, NULL);
		}
		else
		{
//			ModifyStyle(WS_CHILD, WS_POPUP|WS_THICKFRAME);
			KillTimer(nIDEvent);
			SetTimer(400, 40, NULL);
		}
	}
	else if(nIDEvent == 400)
	{
		CRect rcChart;
		m_pStdDialog->GetWindowRect(rcChart);
		ScreenToClient(rcChart);

		CPoint ptCurr;
		::GetCursorPos(&ptCurr);
		CRect rcLeftBar;
		GetWindowRect(rcLeftBar);
		if(!rcLeftBar.PtInRect(ptCurr))
		{
			CRect rcSideBar;
			rcSideBar.top = rcChart.top-3;
			rcSideBar.bottom = rcChart.bottom-3;
			rcSideBar.left = 0-3;//WIDTH_SIDEBAR;
			rcSideBar.right = rcSideBar.left + m_nSideBarIncrease;

			if(m_nSideBarIncrease > 0)
			{
				int nInterval = m_pStdDialog->GetWidthOfLeftBar() - m_nSideBarIncrease;
				if(nInterval > 0 && nInterval < 30)
				{
					m_nSideBarIncrease -= nInterval;
				}
				else
				{
					m_nSideBarIncrease -= 30;
				}

				rcSideBar.right = rcSideBar.left + m_nSideBarIncrease;
				SetWindowPos(NULL, rcSideBar.left,rcSideBar.top,rcSideBar.Width(),rcSideBar.Height(), SWP_SHOWWINDOW);
//				MoveWindow(rcSideBar);
//				ShowWindow(SW_SHOW);
				EndSideBar();
			}
			else
			{
//				ModifyStyle(WS_POPUP|WS_THICKFRAME, WS_CHILD);
				m_nSideBarIncrease = 0;
				m_pStdDialog->m_btnSideBar.EndSideBar();

				if(!m_pStdDialog->m_btnSideBar.IsWindowVisible())
				{
					m_pStdDialog->m_btnSideBar.ShowWindow(SW_SHOW);
					m_pStdDialog->m_btnSideBar.Invalidate();
				}
//				m_tabctrl.Invalidate();

				Invalidate();
				KillTimer(nIDEvent);
			}
		}
	}
	CRscDlg::OnTimer(nIDEvent);
}

BOOL CLeftBarDlg::EnableWindow(BOOL bEnable)
{
	m_bEnable = bEnable;
	return CRscDlg::EnableWindow(bEnable);
}

BOOL CLeftBarDlg::GetEnableWindow()
{
	return m_bEnable;
}

void CLeftBarDlg::OnBthidechart() 
{
	// TODO: Add your control notification handler code here
	m_pStdDialog->HideChart();
}

void CLeftBarDlg::GetIndicatorNameListIntoComboBox(CComboBox& rComboBox,LPCTSTR p_strGroup)
{
#ifdef _DEBUG	
	VERIFY(m_pChartMng);
#endif

	if(NULL == m_pChartMng->GetSafeHwnd()) return;
	//VERIFY(m_pChartMng->GetSafeHwnd());


	if(m_pChartMng)
	{
		if(m_pChartMng->GetSafeHwnd())
		{
			m_pChartMng->GetIndicatorNameListIntoComboBox(rComboBox, p_strGroup);
		}		
	}	
}
BOOL CLeftBarDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(pMsg->message)
	{
		case WM_KEYDOWN:
			switch(pMsg->wParam) 
			{
			case VK_RETURN :
			case VK_ESCAPE :
				{
					return FALSE;			
				}
				break;
			}		
			break;
	}
//	return CRscDlg::PreTranslateMessage(pMsg);

//	if(pMsg->wParam == VK_HOME)
//		return FALSE;

	if(::IsWindow(m_hWnd))
	{
		CWnd* pParent = GetParent();
		if(pParent)
			return GetParent()->PreTranslateMessage(pMsg);
		else
			return CRscDlg::PreTranslateMessage(pMsg);
	}
	else
		return CRscDlg::PreTranslateMessage(pMsg);
}

CString CLeftBarDlg::GetUserPath()
{
	return m_pStdDialog->m_strUserDir;
}

HBRUSH CLeftBarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	return m_brBkColor;
}

BOOL CLeftBarDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CLeftBarDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(rcClient, m_clrBk);
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();

	rcClient.bottom = rcClient.top + 2;
	dc.FillSolidRect(rcClient, RGB(255,255,255));
}

void CLeftBarDlg::ChangeSkinColor()
{
	if(m_pStdDialog)
	{
		m_clrBk = (COLORREF)m_pStdDialog->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
		m_brBkColor.DeleteObject();
		m_brBkColor.CreateSolidBrush(m_clrBk);

		m_tabctrl.ChangeSkinColor((CWnd*)m_pStdDialog);
	}
	Invalidate();
}