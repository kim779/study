/******************************************************************
FileName              : STDlgConfirm.cpp
Company               : DRFN
Author                : Lee Jun Ok : alzioyes@gmail.com
Purpose               : 
Date Of Creation      : 2006-12-06 오후 9:09:29
Modification History  : 
Date                  :
********************************************************************/

#include "stdafx.h"
#include "STDlgConfirm.h"
#include <mmsystem.h>

#include "../../inc/IDataUtilMng.h"
#include "../../inc/IMasterDataMng.h"
//#include "../chart_common/Grid/Tr_drds/D49890.h"
#include "../COMMONTR/SS_OrderDef.h"
//#include "../COMMONTR/SY_s0001.h"	// 주식 호가
//#include "../COMMONTR/SY_f0201.h"	// 선물옵션 호가
//#include "../COMMONTR/SY_s3301.h"	// 주식선물 호가
//#include "../COMMONTR/SY_f3201.h"	// 주식옵션 호가
/*
#include "../COMMONTR/tr_ohhts_4000_001r.h"
#include "../COMMONTR/tr_ohhts_6302_001r.h"
#include "../COMMONTR/tr_ohhts_6302_002r.h"
*/
#include "../Include_ST/STOrderDef.h"

#include "../CommonTr/KB_f0201.h"
#include "../CommonTr/KB_01230.h"

const UINT RMSG_DESTROY_CONFIRMWND = ::RegisterWindowMessage("RMSG_DESTROY_CONFIRMWND");
const UINT RMSG_RECEIVE_HOGADATA = ::RegisterWindowMessage(_T("RMSG_RECEIVE_HOGADATA"));
const UINT RMSG_ORDER_STOCK = ::RegisterWindowMessage(_T("RMSG_ORDER_STOCK"));
const UINT RMSG_ORDER_FUTOPT = ::RegisterWindowMessage(_T("RMSG_ORDER_FUTOPT"));

const UINT RMSG_RECEIVE_DIRECT	= ::RegisterWindowMessage("RMSG_RECEIVE_DIRECT");

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CConfirmWnd

CConfirmWnd::CConfirmWnd()
{
	m_pSTDlgConfirmSendJumun = NULL;
}

CConfirmWnd::~CConfirmWnd()
{
}


BEGIN_MESSAGE_MAP(CConfirmWnd, CWnd)
	//{{AFX_MSG_MAP(CConfirmWnd)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_RECEIVE_DIRECT, OnRmsgReceiveDirect)
	
END_MESSAGE_MAP()


BOOL CConfirmWnd::OpenWindow()
{
	CString		szClassName;	
	
	szClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 0, 0, 0);
	BOOL bSuccess = CreateEx(0L, (LPCSTR)szClassName, "", WS_VISIBLE, 0, 0, 0, 0, HWND_DESKTOP, NULL, NULL);
	if (bSuccess)
	{
		ShowWindow(SW_HIDE);

		// 매매구분, 00시장가, 03지정가.
		if (m_InData.m_nOrderPriceType == hogaIdx_market)
		{
			if (m_InData.m_nMarket==0)	memcpy(m_InData.m_jutype, "00", 2);
			else	m_InData.m_jutype[0] = '1';
		}
		else
		{
			if (m_InData.m_nMarket==0)	memcpy(m_InData.m_jutype, "03", 2);
			else	m_InData.m_jutype[0] = '2';
		}
	}

	return bSuccess;
}

void CConfirmWnd::OnDestroy()
{
	CWnd::OnDestroy();
}

#define		UM_SM_SIGNALORDER_MSG		(WM_USER+551)
int CConfirmWnd::SendJumun(int nType)
{
	// 강제청산 or 매수청산 or 매도
	if ((m_InData.m_nOrderType & FORCEDEXIT) ||
		(m_InData.m_nOrderType & VALEXITLONG) ||
		(m_InData.m_nOrderType & VALENTRYSELL))
	{
		if (m_InData.m_bSound_Sell_Use)
			sndPlaySound(m_InData.m_szSound_Sell_File, SND_ASYNC);
	}
	// 매수 or 매도청산
	else if ((m_InData.m_nOrderType & VALENTRYBUY) ||
			(m_InData.m_nOrderType & VALEXITSHORT))
	{
		if (m_InData.m_bSound_Buy_Use)
			sndPlaySound(m_InData.m_szSound_Buy_File, SND_ASYNC);
	}

	// 주문처리
	if(m_InData.m_nAutoOrder == 1) // 반자동
	{
		STORDERINFO signalOrdersInfo;
		memset(&signalOrdersInfo, 0, sizeof(STORDERINFO));
		memcpy(signalOrdersInfo.szPopgb, "1", sizeof(signalOrdersInfo.szPopgb)-1);									// 팝업Type : (2)주문창
		memcpy(signalOrdersInfo.szCode, (LPSTR)(LPCSTR)m_InData.m_szCode, sizeof(signalOrdersInfo.szCode)-1);						// 종목코드
		memcpy(signalOrdersInfo.szAcctno, (LPSTR)(LPCSTR)m_InData.m_szGeja, sizeof(signalOrdersInfo.szAcctno)-1);				// 계좌번호

		CString	szPrice;
		if(m_InData.m_nOrderPriceType == 0)		// 현재가
			szPrice = m_strCurPrice;
		else if(m_InData.m_nOrderPriceType == 1)		// 매수1호가
			szPrice = m_strBuy1Hoga;
		else if(m_InData.m_nOrderPriceType == 2)		// 매수2호가
			szPrice = m_strBuy2Hoga;
		else if(m_InData.m_nOrderPriceType == 3)		// 매수3호가
			szPrice = m_strBuy3Hoga;
		else if(m_InData.m_nOrderPriceType == 4)		// 매도1호가
			szPrice = m_strSell1Hoga;
		else if(m_InData.m_nOrderPriceType == 5)		// 매도2호가
			szPrice = m_strSell2Hoga;
		else if(m_InData.m_nOrderPriceType == 6)		// 매도3호가
			szPrice = m_strSell3Hoga;
		else if(m_InData.m_nOrderPriceType == 7)		// 상한가
			szPrice = m_strUpLimitPrice;
		else if(m_InData.m_nOrderPriceType == 8)		// 하한가
			szPrice = m_strDownLimitPrice;

		szPrice.TrimRight();	szPrice.TrimLeft();
		szPrice.Remove('+');	szPrice.Remove('-');

		CString strMsg("시스템트레이딩");
		if(m_InData.m_nOrderType & VALEXITSHORT  || m_InData.m_nOrderType & VALENTRYBUY)
		{
			memcpy(signalOrdersInfo.szMemegb, "1", sizeof(signalOrdersInfo.szMemegb)-1);							// 매매구분 : (1)매수
			if(m_InData.m_nOrderMarketEntry == 1)		// 시장가
			{
				memcpy(signalOrdersInfo.szOrdgb, "03", sizeof(signalOrdersInfo.szOrdgb)-1);							// 주문구분(보통가, 시장가,...)
				memcpy(signalOrdersInfo.szOrdprc, "0", sizeof(signalOrdersInfo.szOrdprc)-1);						// 주문단가
			}
			else
			{
				memcpy(signalOrdersInfo.szOrdgb, "00", sizeof(signalOrdersInfo.szOrdgb)-1);							// 주문구분(보통가, 시장가,...)
				memcpy(signalOrdersInfo.szOrdprc, (LPSTR)(LPCSTR)szPrice, sizeof(signalOrdersInfo.szOrdprc)-1);		// 주문단가
			}
		}
		else if(m_InData.m_nOrderType & VALEXITLONG  || m_InData.m_nOrderType & VALENTRYSELL || m_InData.m_nOrderType & FORCEDEXIT)
		{
			memcpy(signalOrdersInfo.szMemegb, "0", sizeof(signalOrdersInfo.szMemegb)-1);							// 매매구분 : (0)매도
			if(m_InData.m_nOrderMarketClear == 1)		// 시장가
			{
				memcpy(signalOrdersInfo.szOrdgb, "03", sizeof(signalOrdersInfo.szOrdgb)-1);							// 주문구분(보통가, 시장가,...)
				memcpy(signalOrdersInfo.szOrdprc, "0", sizeof(signalOrdersInfo.szOrdprc)-1);						// 주문단가
			}
			else
			{
				memcpy(signalOrdersInfo.szOrdgb, "00", sizeof(signalOrdersInfo.szOrdgb)-1);							// 주문구분(보통가, 시장가,...)
				memcpy(signalOrdersInfo.szOrdprc, (LPSTR)(LPCSTR)szPrice, sizeof(signalOrdersInfo.szOrdprc)-1);		// 주문단가
			}
		}

		CString strQty;
		strQty.Format("%d", m_InData.m_nQty);
		memcpy(signalOrdersInfo.szOrdqty, (LPSTR)(LPCSTR)strQty, sizeof(signalOrdersInfo.szOrdqty)-1);				// 주문수량

		if (m_pSTDlgConfirmSendJumun)
		{
			CString strScrArg, strDomino;
			strDomino.Format(_T("%s||%s|%s||%s"),	signalOrdersInfo.szCode,
													signalOrdersInfo.szOrdprc,
													signalOrdersInfo.szOrdqty, signalOrdersInfo.szOrdgb);	//주문구분 추가 

			if (nType == 1)		// 주식 호가 : 매수 
			{
				
				if(signalOrdersInfo.szMemegb[0] == '1')
					strScrArg.Format(_T("IB101200 /p5 /t/d catch_domino_data\t%s\n") , strDomino);
				else 
					strScrArg.Format(_T("IB101100 /p5 /t/d catch_domino_data\t%s\n") , strDomino);
			}
			else if (nType == 2)	// 선/옵 호가
			{
				if(signalOrdersInfo.szMemegb[0] == '1')
					strScrArg.Format(_T("IB301200 /p5 /t/d catch_domino_data\t%s\n") , strDomino);
				else 
					strScrArg.Format(_T("IB301100 /p5 /t/d catch_domino_data\t%s\n") , strDomino);
			}
			// lParam = MAPs(8) + OPTIONs + domino DATAs ("/d SYM \t DATA ...")
			// OPTIONs : "/c /t /S /s /K /k /p /d" (change, trigger, search, key, pos, domino)
			IPartnerSvrManager* pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
			if (pPtSvrMng)
			{
				IPartnerUtilManager* pPtUtilMng = (IPartnerUtilManager*)pPtSvrMng->GetInerInterface(LUID_IPartnerUtilManager);
				if (pPtUtilMng)
					pPtUtilMng->OpenScreen(GetParent()->GetSafeHwnd(), strScrArg);
			}

//			HWND hScreenView = m_pSTDlgConfirmSendJumun->GetHwndScreenView();
//			::SendMessage(hScreenView, RMSG_ORDER_STOCK, (WPARAM)0L, (long)&signalOrdersInfo);
//			::SendMessage(hScreenView, RMSG_ORDER_FUTOPT, (WPARAM)0L, (long)&signalOrdersInfo);
		}
/*
		stSTORDERMSGINFO stOrderMsg;

		CTime tmCur(CTime::GetCurrentTime());
		CString strData;
		strData = tmCur.Format("%H%M%S");

		::memset(&stOrderMsg, 0x20, sizeof(stSTORDERMSGINFO));
		::memcpy(stOrderMsg.szTime, strData, strData.GetLength());
		::memcpy(stOrderMsg.szItemCode, signalOrdersInfo.szCode, ::lstrlen(signalOrdersInfo.szCode));
		::memcpy(stOrderMsg.szItemName, m_InData.m_szCodeName, m_InData.m_szCodeName.GetLength());

		strData = m_InData.m_szSTPath.Mid(m_InData.m_szSTPath.ReverseFind('\\') + 1);
		int nPos = strData.Find(gSTGubunKey);
		if(nPos > 0)
			strData = strData.Mid(0, nPos);
		else
		{
			nPos = strData.ReverseFind('.');
			strData = strData.Mid(0, nPos);
		}
		::memcpy(stOrderMsg.szSTName, strData, strData.GetLength());

		if(m_InData.m_nOrderType & VALEXITSHORT  || m_InData.m_nOrderType & VALENTRYBUY)
			::memcpy(stOrderMsg.szOrderType, "1", sizeof(stOrderMsg.szOrderType));
		else if(m_InData.m_nOrderType & VALEXITLONG  || m_InData.m_nOrderType & VALENTRYSELL || m_InData.m_nOrderType & FORCEDEXIT)
			::memcpy(stOrderMsg.szOrderType, "0", sizeof(stOrderMsg.szOrderType));

		::memcpy(stOrderMsg.szAccount, signalOrdersInfo.szAcctno, ::lstrlen(signalOrdersInfo.szAcctno));
		if(m_InData.m_nOrderMarketEntry == 1)		// 시장가
			::memcpy(stOrderMsg.szOrdgb, "01", sizeof(stOrderMsg.szOrdgb));
		else
			::memcpy(stOrderMsg.szOrdgb, "00", sizeof(stOrderMsg.szOrdgb));

		::memcpy(stOrderMsg.szOrdqty, signalOrdersInfo.szOrdqty, ::lstrlen(signalOrdersInfo.szOrdqty));
		::memcpy(stOrderMsg.szOrdprc, signalOrdersInfo.szOrdprc, ::lstrlen(signalOrdersInfo.szOrdprc));

		AfxGetMainWnd()->SendMessage(WMU_SIGNAL_CHART, 0, (LPARAM)&stOrderMsg);
*/
	}

	if (m_pSTDlgConfirmSendJumun) 
	{
		CWnd* pParentWnd = m_pSTDlgConfirmSendJumun->GetParent();
		pParentWnd->PostMessage(RMSG_DESTROY_CONFIRMWND, (WPARAM)this);
	}

	return 1;
}

LRESULT CConfirmWnd::OnRmsgReceiveDirect(WPARAM wParam, LPARAM lParam)
{
	char szData[32] = {NULL, };
	FID_RECEIVEDATA *pData = (FID_RECEIVEDATA *)wParam;

	if (pData->szTrCode.CompareNoCase(QUERY_STOCK_HOGA) == 0)
	{
		k01230Out1* lpOut_Ex = (k01230Out1*)pData->pData;
		
		::memcpy(&szData, lpOut_Ex->price, sizeof(lpOut_Ex->price));
		// 현재가 저장
		m_strCurPrice = szData;
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->uplmtprice, sizeof(lpOut_Ex->uplmtprice));
		// 상한가
		m_strUpLimitPrice = szData;
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->dnlmtprice, sizeof(lpOut_Ex->dnlmtprice));
		// 하한가
		m_strDownLimitPrice = szData;
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->offerho3, sizeof(lpOut_Ex->offerho3));
		// 매도3호가
		m_strSell3Hoga = szData;
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->offerho2, sizeof(lpOut_Ex->offerho2));
		// 매도2호가
		m_strSell2Hoga = szData;
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->offerho1, sizeof(lpOut_Ex->offerho1));
		// 매도1호가
		m_strSell1Hoga = szData;
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->bidho1, sizeof(lpOut_Ex->bidho1));
		// 매수1호가
		m_strBuy1Hoga = szData;
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->bidho2, sizeof(lpOut_Ex->bidho2));
		// 매수2호가
		m_strBuy2Hoga = szData;
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->bidho3, sizeof(lpOut_Ex->bidho3));
		// 매수3호가
		m_strBuy3Hoga = szData;
		
		SendJumun(1);
	}
	else if (pData->szTrCode.CompareNoCase(QUERY_FUTOPT_HOGA) == 0)
	{
		KB_f0201_OutRec1 *lpOut_Ex = (KB_f0201_OutRec1*)pData->pData;
		
		::memcpy(&szData, lpOut_Ex->price, sizeof(lpOut_Ex->price));
		// 현재가 저장
		m_strCurPrice.Format("%.2f", atof(szData)/100.);
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->uplmtprice, sizeof(lpOut_Ex->uplmtprice));
		// 상한가
		m_strUpLimitPrice.Format("%.2f", atof(szData)/100.);
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->dnlmtprice, sizeof(lpOut_Ex->dnlmtprice));
		// 하한가
		m_strDownLimitPrice.Format("%.2f", atof(szData)/100.);
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->offerho3, sizeof(lpOut_Ex->offerho3));
		// 매도3호가
		m_strSell3Hoga.Format("%.2f", atof(szData)/100.);
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->offerho2, sizeof(lpOut_Ex->offerho2));
		// 매도2호가
		m_strSell2Hoga.Format("%.2f", atof(szData)/100.);
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->offerho1, sizeof(lpOut_Ex->offerho1));
		// 매도1호가
		m_strSell1Hoga.Format("%.2f", atof(szData)/100.);
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->bidho1, sizeof(lpOut_Ex->bidho1));
		// 매수1호가
		m_strBuy1Hoga.Format("%.2f", atof(szData)/100.);
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->bidho2, sizeof(lpOut_Ex->bidho2));
		// 매수2호가
		m_strBuy2Hoga.Format("%.2f", atof(szData)/100.);
		
		::memset(&szData, 0x00, sizeof(szData));
		::memcpy(&szData, lpOut_Ex->bidho3, sizeof(lpOut_Ex->bidho3));
		// 매수3호가
		m_strBuy3Hoga.Format("%.2f", atof(szData)/100.);

		SendJumun(2);
	}

	return 0L;
}
