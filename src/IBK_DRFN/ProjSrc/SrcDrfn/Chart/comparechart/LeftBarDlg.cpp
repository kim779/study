// LeftBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LeftBarDlg.h"
#include "StdDialog.h"
#include "TabDlg.h"
#include "./define/ChartMsgDef.h"
//#include "WndMapView.h"
//#include "FavoriteThingsMng.h"
//#include "../../../DataMgr/DBMgr.h"
//#include "../../../DrMLib/DrMiddleLib.h"

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
	m_pChartMng = NULL;
	m_lJMaster = NULL;
	m_lKMaster = NULL;
	m_bEnable = FALSE;
	/*=============== 2005. 06. 09 sy, nam ============================
	m_pWndMapView = NULL;
	/*=============== 2005. 06. 09 sy, nam ============================*/
//	m_pFavoriteMng = NULL;
	m_bIgnoreShareData = FALSE;

	m_nID = 1;

//	m_pDBMgr = NULL; //2005. 05. 12 - CAny2Code 대체용...
}


void CLeftBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftBarDlg)
//	DDX_Control(pDX, IDC_BTHIDECHART, m_btHideChart);
	DDX_Control(pDX, IDC_TAB, m_tabctrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeftBarDlg, CRscDlg)
	//{{AFX_MSG_MAP(CLeftBarDlg)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_WM_PAINT()
//	ON_BN_CLICKED(IDC_BTHIDECHART, OnBthidechart)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftBarDlg message handlers

BOOL CLeftBarDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here	
	m_strRootPath = ((CStdDialog*)GetParent())->m_strRootPath;
	m_strConfig = ((CStdDialog*)GetParent())->m_strConfig;
	m_szImageDir = ((CStdDialog*)GetParent())->m_szImageDir;

	CString strImageUp;
	CString strImageDown;

	m_tabctrl.Init(GetParent(),m_pChartMng, 0);//05.06.13 DBMgr추가
	m_tabctrl.SetTopLeftCorner(CPoint(0,20));
//	SetTimer(200,1,NULL);
	//JS.Kim_20101005   화면 깜박임 현상 제거
	m_tabctrl.ModifyStyle(0, WS_CLIPSIBLINGS|WS_CLIPCHILDREN);

	m_UsrAddListDlg.m_szImageDir = m_szImageDir;
	m_UsrAddListDlg.Create(CUsrAddListDlg::IDD, this);	

	m_UsrAddListDlg.ShowWindow(SW_SHOW);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CLeftBarDlg::OnSize(UINT nType, int cx, int cy) 
{
	CRscDlg::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CWnd* pParent = GetParent();
	if(pParent==NULL || pParent->GetSafeHwnd()==NULL) return;

	if(((CStdDialog*)GetParent())->m_nChartType==2)
	{
		/*=============== 2005. 06. 09 sy, nam ============================
		if(m_pWndMapView)
		{
			m_pWndMapView->Invalidate();
		}	
		/*=============== 2005. 06. 09 sy, nam ============================*/
	}
	else
	{
		CRect rect, tabRect, listDlgRect;
		GetClientRect(rect);
		if(m_tabctrl.GetSafeHwnd())
		{
			//2005. 06. 10 ==========================================
			//비교차트에서 사용자가 추가한 항목 리스트를 표시하는 Dialog를 
			//보이게하기 위해 크기를 절반으로 고정 시킨다.
			//=======================================================
			tabRect = rect;
//-->@.080617-003 : 탭영역 하얀줄.
			tabRect.top += 2;
//<--
			tabRect.bottom = tabRect.bottom - 200;  
			m_tabctrl.MoveWindow(tabRect);
			m_tabctrl.Invalidate();
		}
		
//		if(m_btHideChart.GetSafeHwnd())
//		{
//			rect.left = rect.right - 20;
//			rect.bottom = rect.top + 20;
//
//			m_btHideChart.MoveWindow(rect);
//		}

		//2005. 06. 10 ==========================================
		//비교차트에서 사용자가 추가한 항목 리스트를 표시하는 Dialog를 
		//표시한다.
		//=======================================================
		if(m_UsrAddListDlg.GetSafeHwnd())
		{
			GetClientRect(listDlgRect);
			listDlgRect.top = listDlgRect.bottom - 200;
			listDlgRect.right = rect.right;
			m_UsrAddListDlg.MoveWindow(listDlgRect);						
			m_UsrAddListDlg.Invalidate();			
		}
	}
}

void CLeftBarDlg::SetConfig(BOOL bLoad)
{	
	if(bLoad)
	{
		m_tabctrl.SetCurSel(0);
	}
	else
	{
	}
}

void CLeftBarDlg::OnDestroy() 
{
	// TODO: Add your message handler code here
	/*=============== 2005. 06. 09 sy, nam ============================
	if(m_pWndMapView)
	{
		m_pWndMapView->DestroyWindow();
		delete m_pWndMapView;
		m_pWndMapView = NULL;
	}
	/*=============== 2005. 06. 09 sy, nam ============================*/
	//@유진삭제 SetConfig(FALSE);
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
	VERIFY(m_pChartMng);
	VERIFY(m_pChartMng->GetSafeHwnd());
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
	case UMSG_SETINITIALCHARTDATA:// 차트의 기본초기값을 보낸다.
		{
			/*=============== 2005. 08. 03 sy, nam ============================
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
			/*=============== 2005. 06. 09 sy, nam ============================
			if(m_pWndMapView)
			{
				stChartInitData* _pstData;
				_pstData = (stChartInitData*)lParam;
				if(!_pstData->m_strCode.IsEmpty()) 
				{
					STSHAREDDATA stData;
					stData.m_szKey = "코드-코코2";
					stData.m_szValue = _pstData->m_strCode;
					m_pWndMapView->SetGShareData(&stData);				
				}
			}
			/*=============== 2005. 06. 09 sy, nam ============================*/
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
			/*=============== 2005. 06. 09 sy, nam ============================
			if(m_pWndMapView)
			{
				m_pWndMapView->SetGShareData((LPVOID)lParam);				
			}
			/*=============== 2005. 06. 09 sy, nam ============================*/
		}
		break;
	case UMSG_SENDJCODECHECKED:
	case UMSG_GETJCODECHECKEDFROMLEFTBAR:
	case UMSG_SENDJCODEUNHIGHLIGHTED:	
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
		}
		break;
	case UMSG_UPDATECODE:
		{
			GetParent()->SendMessage(UMSG_UPDATECODE,wParam,lParam);
		}
		break;
//	case UMSG_GETFAVORITETHINGSMNG:
//		{
//			return (LRESULT)m_pFavoriteMng;
//		}
//		break;
	//===================================================================
	//2005. 06. 14 add by sy,nam
	//===================================================================
	case UMSG_ADDITEM_BTNCLICK:
		m_tabctrl.SendMessage(UMSG_ADDITEM_BTNCLICK, 0, 0);
		break;
	case UMSG_DELITEM_BTNCLICK:
		
		break;
	case UMSG_INITIAL_BTNCLICK:
		//Drds끊기.
		//차트 내용없애기 
		m_UsrAddListDlg.SendMessage(UMSG_INITIAL_BTNCLICK);
		break;
	case UMSG_ADDITEM:
		m_UsrAddListDlg.SendMessage(UMSG_ADDITEM, wParam, lParam);
		// 종목코드 연동처리.
//		{
//			CString szTmp = *((CString*)lParam);	//넘어온 lparam내의 값을 CString 변수에 넣는다.
//
//			int nPos = szTmp.Find('|', 0);
//			if(nPos>0) 
//			{
//				CString szCode;
//				szCode = szTmp.Mid(nPos+1);	szCode.TrimLeft(); szCode.TrimRight();
//				if(szCode.GetLength()>0)
//					GetParent()->GetParent()->SendMessage(WM_USER+107, 0, (LPARAM)(LPCSTR)szCode);
//			}
//		}
		break;
	case UMSG_RE_REQUEST_TRDATA:  //Real 모두 다시 요청한다.
		m_UsrAddListDlg.SendMessage(UMSG_RE_REQUEST_TRDATA, wParam, lParam);
		break;
	case WM_NOTIFY:
		{
			NMHDR* nmhdr = (NMHDR*)lParam;
			int nCode = nmhdr->code;
			
			if(nCode == TCN_SELCHANGE)			// Left button click
			{
				int iNewTab = m_tabctrl.GetCurSel();	
				if(GetParent())
					GetParent()->SendMessage(UMSG_TAB_SELCHANGED, 0, iNewTab);
			}
		}
		break;
	case UMSG_END_REQUEST_CHARTDATA:
		m_UsrAddListDlg.AddELWIndexCode();
		break;
	//===================================================================
	}
	return CRscDlg::WindowProc(message, wParam, lParam);
}

BOOL CLeftBarDlg::IsAdvisingCodeOf(LPCTSTR lpCode)
{
	return (BOOL)m_Codes.Find(lpCode);
}



void CLeftBarDlg::SetRealPriceData(char *pData, int nSize)
{
//	CTabDlg *ptabDlg = (CTabDlg *)m_tabctrl.GetWndFocused();
//	if(ptabDlg)
//	{
//		ptabDlg->SetRealPriceData(pData, nSize);
//	}	
//	ptabDlg = (CTabDlg *)m_tabctrl.GetWndCodeSearch();
//	if(ptabDlg)
//	{
//		ptabDlg->SetRealPriceData(pData, nSize);
//	}
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

void CLeftBarDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
// 	if(nIDEvent==200)
// 	{
// 		KillTimer(200);
// 		SetConfig();		
// 	}

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
	((CStdDialog*)GetParent())->HideChart();
}

void CLeftBarDlg::GetIndicatorNameListIntoComboBox(CComboBox& rComboBox,LPCTSTR p_strGroup)
{
#ifdef _DEBUG	
	VERIFY(m_pChartMng);
	if(NULL == m_pChartMng->GetSafeHwnd()) return;
	//VERIFY(m_pChartMng->GetSafeHwnd());
#endif

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
	if(pMsg->wParam == VK_HOME)
			return FALSE;

	if(::IsWindow(m_hWnd))
	
		return GetParent()->PreTranslateMessage(pMsg);
	else
		return CRscDlg::PreTranslateMessage(pMsg);	
}

void CLeftBarDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iNewTab = m_tabctrl.GetCurSel();	
	GetParent()->SendMessage(UMSG_TAB_SELCHANGED, 0, iNewTab);
	*pResult = 0;
}


void CLeftBarDlg::ChangeSkinColor(COLORREF clrBackground)
{
	m_tabctrl.ChangeSkinColor();
	m_UsrAddListDlg.ChangeSkinColor(clrBackground);
	CRscDlg::ChangeSkinColor(clrBackground);
}

//-->@.080617-003 : 탭영역 하얀줄.
void CLeftBarDlg::OnPaint() 
{
//	CPaintDC dc(this); // device context for painting
//	CRect rcClient;
//	GetClientRect(&rcClient);
//	rcClient.bottom = rcClient.top + 2;
//	dc.FillSolidRect(rcClient, RGB(255,255,255));
	CRscDlg::OnPaint();
}
//<--

CString CLeftBarDlg::GetCurItemName()
{
	return m_UsrAddListDlg.GetCodeName();
}