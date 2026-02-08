// LeftBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LeftBarDlg.h"
#include "StdDialog.h"
#include "TabDlg.h"
#include "RegFavoriteDlg.h"
#include "./define/ChartMsgDef.h"
//#include "WndMapView.h"
#include "FavoriteThingsMng.h"
//#include "../../../DataMgr/DBMgr.h" //05.09.27
//#include "../../../DrMLib/DrMiddleLib.h"//05.09.27

#define	WIDTH_SIDEBAR		8

const UINT RMSG_ADDACCOUNTCODE		=   ::RegisterWindowMessage("RMSG_ADDACCOUNTCODE");
const UINT RMSG_CHECKAVGPRICE		=   ::RegisterWindowMessage("RMSG_CHECKAVGPRICE");
const UINT RMSG_ADDACCOUNTCONC		=   ::RegisterWindowMessage("RMSG_ADDACCOUNTCONC");
const UINT RMSG_RESETCONCDATA		=   ::RegisterWindowMessage("RMSG_RESETCONCDATA");

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftBarDlg dialog
//extern IMainDllObj	* g_pMainDll;
//extern CWnd *g_pViewInMain;

CLeftBarDlg::CLeftBarDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CLeftBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLeftBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pStdDialog = NULL;
	m_pChartMng = NULL;
	m_lJMaster = NULL;
	m_lKMaster = NULL;
	m_bEnable = FALSE;
	m_pFavoriteMng = NULL;
	m_bIgnoreShareData = FALSE;

	m_nID = 1;
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
	ON_REGISTERED_MESSAGE( RMSG_CHECKAVGPRICE, OnCheckAvgPrice )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftBarDlg message handlers

BOOL CLeftBarDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();	

	m_strRootPath = m_pStdDialog->m_strRootPath;
	m_szImageDir = m_pStdDialog->m_szImageDir;

	m_clrBk = (COLORREF)m_pStdDialog->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
	m_brBkColor.CreateSolidBrush(m_clrBk);	//RGB(222,222,222));
	CString strImageUp;
	CString strImageDown;

	if( m_tabctrl.GetSafeHwnd() && ::IsWindow(m_tabctrl.GetSafeHwnd()) )
	{
		m_tabctrl.Init(GetParent(),m_pChartMng, m_nMarketType);
		m_tabctrl.SetTopLeftCorner(CPoint(2,20));

		if( m_tabctrl.GetItemCount() > 0 )
			m_tabctrl.SetCurSel(0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CLeftBarDlg::OnSize(UINT nType, int cx, int cy) 
{
	ModifyStyle(0, WS_CLIPCHILDREN);	// turn on CLIPCHILDREN
	CRscDlg::OnSize(nType, cx, cy);

	if(m_pStdDialog->m_nChartType==2)
	{
	}
	else
	{
		CRect rect;
		GetClientRect(rect);
		rect.top = 1;

		// 사이드바 사용시 사이즈 조정
		if(m_pStdDialog->GetLeftBarType() == 0)
			rect.right -= 2;

		if(m_tabctrl.GetSafeHwnd())
		{
			m_tabctrl.MoveWindow(rect);
		}
	}
	ModifyStyle(WS_CLIPCHILDREN, 0);	// turn off CLIPCHILDREN
}

void CLeftBarDlg::SetConfig(BOOL bLoad /*= TRUE*/)
{
	if(m_pStdDialog->IsSystemChart())
	{
		if(bLoad && m_tabctrl.GetSafeHwnd() && m_tabctrl.GetItemCount() > 0 )
			m_tabctrl.SetCurSel(0);

		return;
	}

	CString strTemp;
	char szTemp[64];	

	CString strCfgFile = m_pStdDialog->m_strConfig;	
	if(bLoad && m_tabctrl.GetSafeHwnd() && m_tabctrl.GetItemCount() > 1 )
	{
		::GetPrivateProfileString("LeftBar","TabPos","0",szTemp,sizeof(szTemp), strCfgFile);
		m_tabctrl.SetCurSel(1);
	}
	else
	{
		strTemp.Format("%d",m_tabctrl.GetCurSel());
		::WritePrivateProfileString("LeftBar","TabPos",strTemp, strCfgFile);
	}
}

void CLeftBarDlg::OnDestroy() 
{
	SetConfig(FALSE);
	m_brBkColor.DeleteObject();
	CRscDlg::OnDestroy();	
}

HRESULT CLeftBarDlg::ReceiveData(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID aTRData, long dwTRDateLen)
{
	if( m_tabctrl.GetSafeHwnd() )
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
	switch(message)
	{
	case UMSG_SHOWREGFAVORITEDLG:
		{
			CRegFavoriteDlg dlg;
			dlg.m_pPtTopLeftOfDlg = (CPoint*)wParam;
			dlg.SetFavoriteThingsMng(m_pFavoriteMng, lParam);
			dlg.DoModal();
		}
		break;
	case UMSG_SETINITIALCHARTDATA:// 차트의 기본초기값을 보낸다.
		{
			if( m_tabctrl.GetSafeHwnd() )
			{
				CWnd *pWnd = m_tabctrl.GetWndFocused();
				if(pWnd && pWnd->GetSafeHwnd())
				{
					pWnd->SendMessage(message,wParam,lParam);
				}
				pWnd = (CTabDlg *)m_tabctrl.GetWndCodeSearch();
				if(pWnd && pWnd->GetSafeHwnd())
				{
					pWnd->SendMessage(message,wParam,lParam);
				}		
			}
		}
		break;
	case UMSG_SETCHARTHOGA:
		{
			if( m_tabctrl.GetSafeHwnd() )
			{
				CWnd *pWnd = m_tabctrl.GetWndFocused();
				if(pWnd && pWnd->GetSafeHwnd())
				{
					pWnd->SendMessage(message,wParam,lParam);
				}	
				pWnd = (CTabDlg *)m_tabctrl.GetWndCodeSearch();
				if(pWnd && pWnd->GetSafeHwnd())
				{
					pWnd->SendMessage(message,wParam,lParam);
				}			
			}
		}
		break;
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
			if( m_tabctrl.GetSafeHwnd() )
			{
				CWnd *pWnd = m_tabctrl.GetWndFocused();
				if(pWnd && pWnd->GetSafeHwnd())
				{
					pWnd->SendMessage(message,wParam,lParam);
				}	
			}
		}
		break;
	case UMSG_RUN_AUTOTIMER:
		{
			if( m_tabctrl.GetSafeHwnd() )
			{
				CTabDlg *pTabCodeSearch = m_tabctrl.GetWndCodeSearch();
				if(pTabCodeSearch && pTabCodeSearch->GetSafeHwnd())
				{
					pTabCodeSearch->SendMessage(message, wParam, lParam);
				}
			}
		}
		break;
	case UMSG_IS_PLAYSTATE:
		{
			if( m_tabctrl.GetSafeHwnd() )
			{
				CTabDlg *pTabCodeSearch = m_tabctrl.GetWndCodeSearch();
				if(pTabCodeSearch && pTabCodeSearch->GetSafeHwnd())
				{
					pTabCodeSearch->SendMessage(message, wParam, lParam);
				}
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
	if( m_tabctrl.GetSafeHwnd() )
	{
		CTabDlg *ptabDlg = (CTabDlg *)m_tabctrl.GetWndFocused();
		if(ptabDlg && ptabDlg->GetSafeHwnd())
		{
			ptabDlg->SetRealPriceData(pData);
		}	
		ptabDlg = (CTabDlg *)m_tabctrl.GetWndCodeSearch();
		if(ptabDlg && ptabDlg->GetSafeHwnd())
		{
			ptabDlg->SetRealPriceData(pData);
		}
	}
}

void CLeftBarDlg::SetPacketFromOutSide(CString strPacketFromOutSide)
{	
	m_strPacketFromOutSide.Empty();
	m_strPacketFromOutSide = strPacketFromOutSide;
	if(m_pChartMng->GetSafeHwnd()) // 2005. 04. 22 by Nam
		m_pChartMng->SendMessage(UMSG_RESETOCX);
	if( m_tabctrl.GetSafeHwnd() )
	{
		CTabDlg *pTabDlg = m_tabctrl.GetWndCodeSearch();
		if( pTabDlg && pTabDlg->GetSafeHwnd() )
			pTabDlg->ReceiveDataFromOutSide();
		m_tabctrl.ShowTab(TABNO_CODESEARCH);
	}
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

			//<<20100412_JS.Kim 솔로몬 요청사항. 무조건 지표탭을 보여준다
			if( m_tabctrl.GetSafeHwnd()  && m_tabctrl.GetItemCount() > 1 )
				m_tabctrl.SetCurSel(1);
			//>>

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
			rcSideBar.bottom = rcChart.bottom+3;
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

				//<<20100412_JS.Kim 솔로몬 요청사항. 무조건 지표탭을 보여준다
				if( m_tabctrl.GetSafeHwnd()  && m_tabctrl.GetItemCount() > 1 )
					m_tabctrl.SetCurSel(1);
				//>>

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
	VERIFY(m_pChartMng);

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
}

void CLeftBarDlg::ChangeSkinColor()
{
	if(m_pStdDialog)
	{
		m_clrBk = (COLORREF)m_pStdDialog->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
		m_brBkColor.DeleteObject();
		m_brBkColor.CreateSolidBrush(m_clrBk);

		if( m_tabctrl.GetSafeHwnd() )
			m_tabctrl.ChangeSkinColor((CWnd*)m_pStdDialog);
	}
	Invalidate();
}

void CLeftBarDlg::AddAccountCodeItem(LPVOID aTRData, long dwTRDateLen)
{
	if( m_tabctrl.GetSafeHwnd() )
	{
		CTabDlg *pTabCodeSearch = m_tabctrl.GetWndCodeSearch();
		if( pTabCodeSearch && pTabCodeSearch->GetSafeHwnd() )
			pTabCodeSearch->SendMessage(RMSG_ADDACCOUNTCODE, (WPARAM)dwTRDateLen, (LPARAM)aTRData);
	}
}

HRESULT CLeftBarDlg::OnCheckAvgPrice(WPARAM wParam, LPARAM lParam)
{
	if( m_tabctrl.GetSafeHwnd() )
	{
		CTabDlg *pTabCodeSearch = m_tabctrl.GetWndCodeSearch();
		if( pTabCodeSearch && pTabCodeSearch->GetSafeHwnd() )
			return pTabCodeSearch->SendMessage(RMSG_CHECKAVGPRICE, wParam, lParam);
	}
	return 0L;
}

void CLeftBarDlg::MakeAccountConcData(LPVOID aTRData, long dwTRDateLen)
{
	if( m_tabctrl.GetSafeHwnd() )
	{
		CTabDlg *pTabCodeSearch = m_tabctrl.GetWndCodeSearch();
		if( pTabCodeSearch && pTabCodeSearch->GetSafeHwnd() )
			pTabCodeSearch->SendMessage(RMSG_ADDACCOUNTCONC, (WPARAM)dwTRDateLen, (LPARAM)aTRData);
	}
}

void CLeftBarDlg::ResetConcData()
{
	if( m_tabctrl.GetSafeHwnd() )
	{
		CTabDlg *pTabCodeSearch = m_tabctrl.GetWndCodeSearch();
		if( pTabCodeSearch && pTabCodeSearch->GetSafeHwnd() )
			pTabCodeSearch->SendMessage(RMSG_RESETCONCDATA);
	}
}
