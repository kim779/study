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
#include "../COMMONTR/EU_s0001.h"	// 주식 호가
#include "../COMMONTR/EU_f0201.h"	// 선물옵션 호가
#include "../COMMONTR/EU_s3301.h"	// 주식선물 호가
#include "../COMMONTR/EU_f3201.h"	// 주식옵션 호가
#include "../Include_ST/STOrderDef.h"

const UINT RMSG_DESTROY_CONFIRMWND = ::RegisterWindowMessage("RMSG_DESTROY_CONFIRMWND");

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CConfirmWnd

CConfirmWnd::CConfirmWnd()
{
	m_pITrCommManager = NULL;
	m_pITrComm = NULL;
	m_pSTDlgConfirmSendJumun = NULL;
}

CConfirmWnd::~CConfirmWnd()
{
}


BEGIN_MESSAGE_MAP(CConfirmWnd, CWnd)
	//{{AFX_MSG_MAP(CConfirmWnd)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CConfirmWnd::OpenWindow()
{
	CString		szClassName;	
	
	szClassName =	AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 
		0, 0, 0);
	BOOL bSuccess = CreateEx(0L,														//dwExStyle, 
					(LPCSTR)szClassName,
					"",
					WS_VISIBLE,	// | WS_CHILD, 
					0, 0, 0, 0, 
					HWND_DESKTOP, 
					NULL,
					NULL);
	if(bSuccess)
	{
		ShowWindow(SW_HIDE);

		// 매매구분, 00시장가, 03지정가.
		if(m_InData.m_nOrderPriceType==hogaIdx_market)
		{
			if(m_InData.m_nMarket==0)	memcpy(m_InData.m_jutype, "00", 2);
			else	m_InData.m_jutype[0] = '1';
		}
		else
		{
			if(m_InData.m_nMarket==0) memcpy(m_InData.m_jutype, "03", 2);
			else	m_InData.m_jutype[0] = '2';
		}

		InitTrComm();

		RequestHogaData();
	}

	return bSuccess;
}

BOOL CConfirmWnd::InitTrComm()
{
	//<<20100308_JS.Kim 솔로몬
	//m_pITrCommManager = (IAUTrCommManager*)AfxGetPctrInterface(UUID_IAUTrCommManager, 2);
	m_pITrCommManager = (IAUTrCommManager*)AfxGetPctrInterface(UUID_IAUTrCommManager);
	//>>
	m_pITrComm = (ITrComm2*)m_pITrCommManager->AdviseTrComm();
	m_pITrComm->ChangeCallback(&m_xMyITrCommSite);

	return TRUE;
}

void CConfirmWnd::RequestHogaData()
{
	if(m_InData.m_nMarket == 0)		// 주식
	{
		CString strFidInfo("0                                                                               ");
		CString strTrInfo;
		strTrInfo.Format("%s20001", m_InData.m_szCode);
		CString strRecordInfo("1               000060220221001002003004005006008122126130134138124128132136140162166170174178164168172176180202204207209061062222070075010092;");
		CString strOutput;
		strOutput = strFidInfo + strTrInfo + strRecordInfo;

		stSend2Server st;
		st.m_nType = 0;
		st.m_pWnd = (CWnd*)this;
		st.m_pBuffer = (void*)(LPCTSTR)strOutput;
		st.m_lLength= strOutput.GetLength();
		st.m_strTRCode = QUERY_STOCK_HOGA;

		m_pITrComm->SetBaseData("SCREENCODE", "450100");
		m_pITrComm->SetTR(QUERY_STOCK_HOGA);
		m_pITrComm->Send2Server(st.m_pBuffer, st.m_lLength, FALSE);
	}
	else if(m_InData.m_nMarket == 1)	// 선물옵션
	{
		CString strFidInfo("0                                                                               ");
		CString strTrInfo;
		strTrInfo.Format("%s20001", m_InData.m_szCode);
		CString strRecordInfo("1               000014018019052077083089078084090;");
		CString strOutput;
		strOutput = strFidInfo + strTrInfo + strRecordInfo;

		stSend2Server st;
		st.m_nType = 0;
		st.m_pWnd = (CWnd*)this;
		st.m_pBuffer = (void*)(LPCTSTR)strOutput;
		st.m_lLength= strOutput.GetLength();
		st.m_strTRCode = QUERY_FUTOPT_HOGA;	

		m_pITrComm->SetBaseData("SCREENCODE", "450100");
		m_pITrComm->SetTR(QUERY_FUTOPT_HOGA);
		m_pITrComm->Send2Server(st.m_pBuffer, st.m_lLength, FALSE);
	}

}
void CConfirmWnd::OnDestroy()
{
	if(m_pITrCommManager && m_pITrComm)
		m_pITrCommManager->UnAdviseTrComm(m_pITrComm);
	m_pITrComm = NULL;

	CWnd::OnDestroy();
}

HRESULT CConfirmWnd::ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData)
{
	return S_OK;
}

HRESULT CConfirmWnd::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID aTRData, long dwTRDateLen)
{
	CWnd *pWndRequested = NULL;
	CString strTRCode = szTR;
	strTRCode.Remove(' ');

	//<<20100308_JS.Kim 솔로몬
	//IDataUtilManager* pUtilMng = (IDataUtilManager*)AfxGetPctrInterface(UUID_IDataUtilManager, 2);
	IDataUtilManager* pUtilMng = (IDataUtilManager*)AfxGetPctrInterface(UUID_IDataUtilManager);
	//>>
	if(pUtilMng == NULL)
		return 0l;

	if( (strTRCode == QUERY_STOCK_HOGA ) && aTRData != NULL)
	{
		ConvertStockHoga(aTRData, dwTRDateLen);
		CEU_s0001 eu_s0001;
		eu_s0001.Convert((EU_s0001_OutRec1*)(LPCSTR)m_strOut);

		// 현재가 저장
		m_strCurPrice = pUtilMng->GetComma(eu_s0001.m_szprice);
		
		// 상한가
		m_strUpLimitPrice = pUtilMng->GetComma(eu_s0001.m_szuplmtprice);

		// 하한가
		m_strDownLimitPrice = pUtilMng->GetComma(eu_s0001.m_szdnlmtprice);
		
		// 매도3호가
		m_strSell3Hoga = pUtilMng->GetComma(eu_s0001.m_szofferho3);

		// 매도2호가
		m_strSell2Hoga = pUtilMng->GetComma(eu_s0001.m_szofferho2);

		// 매도1호가
		m_strSell1Hoga = pUtilMng->GetComma(eu_s0001.m_szofferho1);

		// 매수1호가
		m_strBuy1Hoga = pUtilMng->GetComma(eu_s0001.m_szbidho1);

		// 매수2호가
		m_strBuy2Hoga = pUtilMng->GetComma(eu_s0001.m_szbidho2);

		// 매수3호가
		m_strBuy3Hoga = pUtilMng->GetComma(eu_s0001.m_szbidho3);

		SendJumun_Stock();
	}
	else if( (strTRCode == QUERY_FUTOPT_HOGA ) && aTRData != NULL)
	{
		ConvertFutOptHoga(aTRData, dwTRDateLen);
		CEU_f0201 eu_f0201;
		eu_f0201.Convert((EU_f0201_OutRec1*)(LPCSTR)m_strOut);

		// 현재가 저장
		m_strCurPrice.Format("%.2f", atof(eu_f0201.m_szprice)/100.);
		
		// 상한가
		m_strUpLimitPrice.Format("%.2f", atof(eu_f0201.m_szuplmtprice)/100.);

		// 하한가
		m_strDownLimitPrice.Format("%.2f", atof(eu_f0201.m_szdnlmtprice)/100.);
		
		// 매도3호가
		m_strSell3Hoga.Format("%.2f", atof(eu_f0201.m_szofferho3)/100.);

		// 매도2호가
		m_strSell2Hoga.Format("%.2f", atof(eu_f0201.m_szofferho2)/100.);

		// 매도1호가
		m_strSell1Hoga.Format("%.2f", atof(eu_f0201.m_szofferho1)/100.);

		// 매수1호가
		m_strBuy1Hoga.Format("%.2f", atof(eu_f0201.m_szbidho1)/100.);

		// 매수2호가
		m_strBuy2Hoga.Format("%.2f", atof(eu_f0201.m_szbidho2)/100.);

		// 매수3호가
		m_strBuy3Hoga.Format("%.2f", atof(eu_f0201.m_szbidho3)/100.);
		
		SendJumun_FO();
	}
	else if( (strTRCode == QUERY_STOCKFUT_HOGA ) && aTRData != NULL)
	{
		ConvertStockFutHoga(aTRData, dwTRDateLen);
		CEU_s3301 eu_s3301;
		eu_s3301.Convert((EU_s3301_OutRec1*)(LPCSTR)m_strOut);

		// 현재가 저장
		m_strCurPrice = pUtilMng->GetComma(eu_s3301.m_szprice);
		
		// 상한가
		m_strUpLimitPrice = pUtilMng->GetComma(eu_s3301.m_szuplmtprice);

		// 하한가
		m_strDownLimitPrice = pUtilMng->GetComma(eu_s3301.m_szdnlmtprice);
		
		// 매도3호가
		m_strSell3Hoga = pUtilMng->GetComma(eu_s3301.m_szofferho3);

		// 매도2호가
		m_strSell2Hoga = pUtilMng->GetComma(eu_s3301.m_szofferho2);

		// 매도1호가
		m_strSell1Hoga = pUtilMng->GetComma(eu_s3301.m_szofferho1);

		// 매수1호가
		m_strBuy1Hoga = pUtilMng->GetComma(eu_s3301.m_szbidho1);

		// 매수2호가
		m_strBuy2Hoga = pUtilMng->GetComma(eu_s3301.m_szbidho2);

		// 매수3호가
		m_strBuy3Hoga = pUtilMng->GetComma(eu_s3301.m_szbidho3);
		
		SendJumun_FO();
	}
	else if( (strTRCode == QUERY_STOCKOPT_HOGA ) && aTRData != NULL)
	{
		ConvertStockOptHoga(aTRData, dwTRDateLen);
		CEU_f3201 eu_f3201;
		eu_f3201.Convert((EU_f3201_OutRec1*)(LPCSTR)m_strOut);

		// 현재가 저장
		m_strCurPrice = pUtilMng->GetComma(eu_f3201.m_szprice);
		
		// 상한가
		m_strUpLimitPrice = pUtilMng->GetComma(eu_f3201.m_szhprice);

		// 하한가
		m_strDownLimitPrice = pUtilMng->GetComma(eu_f3201.m_szlprice);
		
		// 매도3호가
		m_strSell3Hoga = pUtilMng->GetComma(eu_f3201.m_szofferho3);

		// 매도2호가
		m_strSell2Hoga = pUtilMng->GetComma(eu_f3201.m_szofferho2);

		// 매도1호가
		m_strSell1Hoga = pUtilMng->GetComma(eu_f3201.m_szofferho1);

		// 매수1호가
		m_strBuy1Hoga = pUtilMng->GetComma(eu_f3201.m_szbidho1);

		// 매수2호가
		m_strBuy2Hoga = pUtilMng->GetComma(eu_f3201.m_szbidho2);

		// 매수3호가
		m_strBuy3Hoga = pUtilMng->GetComma(eu_f3201.m_szbidho3);
		
		SendJumun_FO();
	}

	return 0L;
}

extern long KSLink_GetEnvInfo(int nType);
#define		UM_SM_SIGNALORDER_MSG		(WM_USER+551)
int CConfirmWnd::SendJumun_Stock()
{
//<<20100308_JS.Kim 솔로몬
	/*
	HWND hSignalWnd;
	hSignalWnd = (HWND)KSLink_GetEnvInfo(2);
	if(hSignalWnd == NULL)
		return 0;
	
// 강제청산 or 매수청산 or 매도청산
//	if( (m_InData.m_nOrderType & FORCEDEXIT) || (m_InData.m_nOrderType & VALEXITLONG) || (m_InData.m_nOrderType & VALEXITSHORT) )
//	{
//		if(m_InData.m_bSound_Sell_Use) sndPlaySound (m_InData.m_szSound_Sell_File, SND_ASYNC);
//	}
//	매수 or 매도
//	else if((m_InData.m_nOrderType & VALENTRYBUY) || (m_InData.m_nOrderType & VALENTRYSELL) )
//	{
//		if(m_InData.m_bSound_Buy_Use) sndPlaySound (m_InData.m_szSound_Buy_File, SND_ASYNC);
//	}
// 강제청산 or 매수청산 or 매도
	if( (m_InData.m_nOrderType & FORCEDEXIT) || (m_InData.m_nOrderType & VALEXITLONG) || (m_InData.m_nOrderType & VALENTRYSELL) )
	{
		if(m_InData.m_bSound_Sell_Use) sndPlaySound (m_InData.m_szSound_Sell_File, SND_ASYNC);
	}
// 매수 or 매도청산
	else if((m_InData.m_nOrderType & VALENTRYBUY) || (m_InData.m_nOrderType & VALEXITSHORT) )
	{
		if(m_InData.m_bSound_Buy_Use) sndPlaySound (m_InData.m_szSound_Buy_File, SND_ASYNC);
	}

	// 주문처리
	if(m_InData.m_nAutoOrder == 1) // 반자동
	{
		SIGNALORDERSINFO signalOrdersInfo;
		memset(&signalOrdersInfo, 0, sizeof(SIGNALORDERSINFO));
		signalOrdersInfo.hDLLWnd = (long)GetSafeHwnd();																// 호출한 컨트롤 핸들
		memcpy(signalOrdersInfo.szIndexNo, "16", sizeof(signalOrdersInfo.szIndexNo)-1);								// DLL Index번호 : 16(시스템트레이딩)
		memcpy(signalOrdersInfo.szWindName, "차트", sizeof(signalOrdersInfo.szWindName)-1);							// 신호이름 : "차트"
		memcpy(signalOrdersInfo.szSignalgb, "차", sizeof(signalOrdersInfo.szSignalgb)-1);							// 신호구분 : "차"
		memcpy(signalOrdersInfo.szPopgb, "2", sizeof(signalOrdersInfo.szPopgb)-1);									// 팝업Type : (2)주문창
		memcpy(signalOrdersInfo.szActionGb, "1", sizeof(signalOrdersInfo.szActionGb)-1);							// 버튼 처리 구분값 : (1)주문
		memcpy(signalOrdersInfo.szAcctno, (LPSTR)(LPCSTR)m_InData.m_szGeja, sizeof(signalOrdersInfo.szAcctno)-1);				// 계좌번호
		memcpy(signalOrdersInfo.szCode, (LPSTR)(LPCSTR)m_InData.m_szCode, sizeof(signalOrdersInfo.szCode)-1);						// 종목코드
		memcpy(signalOrdersInfo.szLoangb, "0", sizeof(signalOrdersInfo.szLoangb)-1);								// 신용구분 : (0)현금
		memcpy(signalOrdersInfo.szJanGogb, "0", sizeof(signalOrdersInfo.szJanGogb)-1);								// 잔고구분 : (0)전체

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

		CString strMsg("시스템트레이딩");
//		if(m_InData.m_nOrderType & VALENTRYSELL  || m_InData.m_nOrderType & VALENTRYBUY)
		if(m_InData.m_nOrderType & VALEXITSHORT  || m_InData.m_nOrderType & VALENTRYBUY)
		{
			if(m_InData.m_nOrderType & VALENTRYBUY)
			{
				strMsg += " 매수진입";
			}
			if(m_InData.m_nOrderType & VALEXITSHORT)
			{
				if(strMsg != "시스템트레이딩")
					strMsg += " /";

				strMsg += " 매도청산";
			}
			
			memcpy(signalOrdersInfo.szMemegb, "2", sizeof(signalOrdersInfo.szMemegb)-1);							// 매매구분 : (2)매수
			if(m_InData.m_nOrderMarketEntry == 1)		// 시장가
			{
				memcpy(signalOrdersInfo.szOrdgb, "1", sizeof(signalOrdersInfo.szOrdgb)-1);							// 주문구분(보통가, 시장가,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// 주문단가구분(0:지정가...(인덱스))
				memcpy(signalOrdersInfo.szOrdprc, "0", sizeof(signalOrdersInfo.szOrdprc)-1);						// 주문단가
			}
			else
			{
				memcpy(signalOrdersInfo.szOrdgb, "0", sizeof(signalOrdersInfo.szOrdgb)-1);							// 주문구분(보통가, 시장가,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// 주문단가구분(0:지정가...(인덱스))
				memcpy(signalOrdersInfo.szOrdprc, (LPSTR)(LPCSTR)szPrice, sizeof(signalOrdersInfo.szOrdprc)-1);						// 주문단가
			}
		}
//		else if(m_InData.m_nOrderType & VALEXITLONG  || m_InData.m_nOrderType & VALEXITSHORT)
		else if(m_InData.m_nOrderType & VALEXITLONG  || m_InData.m_nOrderType & VALENTRYSELL || m_InData.m_nOrderType & FORCEDEXIT)
		{
			if(m_InData.m_nOrderType & FORCEDEXIT)
			{
				strMsg += " 강제청산";
			}
			if(m_InData.m_nOrderType & VALENTRYSELL)
			{
				if(strMsg != "시스템트레이딩")
					strMsg += " /";

				strMsg += " 매도진입";
			}
			if(m_InData.m_nOrderType & VALEXITLONG)
			{
				if(strMsg != "시스템트레이딩")
					strMsg += " /";

				strMsg += " 매수청산";
			}

			memcpy(signalOrdersInfo.szMemegb, "1", sizeof(signalOrdersInfo.szMemegb)-1);							// 매매구분 : (1)매도
			if(m_InData.m_nOrderMarketEntry == 1)		// 시장가
			{
				memcpy(signalOrdersInfo.szOrdgb, "1", sizeof(signalOrdersInfo.szOrdgb)-1);							// 주문구분(보통가, 시장가,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// 주문단가구분(0:지정가...(인덱스))
				memcpy(signalOrdersInfo.szOrdprc, "0", sizeof(signalOrdersInfo.szOrdprc)-1);						// 주문단가
			}
			else
			{
				memcpy(signalOrdersInfo.szOrdgb, "0", sizeof(signalOrdersInfo.szOrdgb)-1);							// 주문구분(보통가, 시장가,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// 주문단가구분(0:지정가...(인덱스))
				memcpy(signalOrdersInfo.szOrdprc, (LPSTR)(LPCSTR)szPrice, sizeof(signalOrdersInfo.szOrdprc)-1);						// 주문단가
			}
		}

		memcpy(signalOrdersInfo.szOrdqtygb, "0", sizeof(signalOrdersInfo.szOrdqtygb)-1);							// 주문수량구분(0:수량, 1:금액, 2:매도가능수량%)
		CString strQty;
		strQty.Format("%d", m_InData.m_nQty);
		memcpy(signalOrdersInfo.szOrdqty, (LPSTR)(LPCSTR)strQty, sizeof(signalOrdersInfo.szOrdqty)-1);				// 주문수량
		memcpy(signalOrdersInfo.szbContinue, "1", sizeof(signalOrdersInfo.szbContinue)-1);							// 감시 진행여부
		memcpy(signalOrdersInfo.szMsg, strMsg, sizeof(signalOrdersInfo.szMsg)-1);									// 감시MSG

		::SendMessage(hSignalWnd, UM_SM_SIGNALORDER_MSG, (WPARAM)0L, (long)&signalOrdersInfo);
	}

	if(m_pSTDlgConfirmSendJumun) 
	{
		CWnd* pParentWnd = m_pSTDlgConfirmSendJumun->GetParent();
		pParentWnd->PostMessage(RMSG_DESTROY_CONFIRMWND, (WPARAM)this);
	}
	*/
//>>
	return 1;
}

//------------------------------------------------------------------------------
//		선물주문
//------------------------------------------------------------------------------
int CConfirmWnd::SendJumun_FO()
{
//<<20100308_JS.Kim 솔로몬
	/*
	HWND hSignalWnd;
	hSignalWnd = (HWND)KSLink_GetEnvInfo(2);
	if(hSignalWnd == NULL)
		return 0;
	
// 강제청산 or 매수청산 or 매도청산
//	if( (m_InData.m_nOrderType & FORCEDEXIT) || (m_InData.m_nOrderType & VALEXITLONG) || (m_InData.m_nOrderType & VALEXITSHORT) )
//	{
//		if(m_InData.m_bSound_Sell_Use) sndPlaySound (m_InData.m_szSound_Sell_File, SND_ASYNC);
//	}
//	매수 or 매도
//	else if((m_InData.m_nOrderType & VALENTRYBUY) || (m_InData.m_nOrderType & VALENTRYSELL) )
//	{
//		if(m_InData.m_bSound_Buy_Use) sndPlaySound (m_InData.m_szSound_Buy_File, SND_ASYNC);
//	}
// 강제청산 or 매수청산 or 매도
	if( (m_InData.m_nOrderType & FORCEDEXIT) || (m_InData.m_nOrderType & VALEXITLONG) || (m_InData.m_nOrderType & VALENTRYSELL) )
	{
		if(m_InData.m_bSound_Sell_Use) sndPlaySound (m_InData.m_szSound_Sell_File, SND_ASYNC);
	}
// 매수 or 매도청산
	else if((m_InData.m_nOrderType & VALENTRYBUY) || (m_InData.m_nOrderType & VALEXITSHORT) )
	{
		if(m_InData.m_bSound_Buy_Use) sndPlaySound (m_InData.m_szSound_Buy_File, SND_ASYNC);
	}

	// 주문처리
	if(m_InData.m_nAutoOrder == 1) // 반자동
	{
		SIGNALORDERSINFO signalOrdersInfo;
		memset(&signalOrdersInfo, 0, sizeof(SIGNALORDERSINFO));
		signalOrdersInfo.hDLLWnd = (long)GetSafeHwnd();																// 호출한 컨트롤 핸들
		memcpy(signalOrdersInfo.szIndexNo, "16", sizeof(signalOrdersInfo.szIndexNo)-1);								// DLL Index번호 : 16(시스템트레이딩)
		memcpy(signalOrdersInfo.szWindName, "차트", sizeof(signalOrdersInfo.szWindName)-1);							// 신호이름 : "차트"
		memcpy(signalOrdersInfo.szSignalgb, "차", sizeof(signalOrdersInfo.szSignalgb)-1);							// 신호구분 : "차"
		memcpy(signalOrdersInfo.szPopgb, "2", sizeof(signalOrdersInfo.szPopgb)-1);									// 팝업Type : (2)주문창
		memcpy(signalOrdersInfo.szActionGb, "1", sizeof(signalOrdersInfo.szActionGb)-1);							// 버튼 처리 구분값 : (1)주문
		memcpy(signalOrdersInfo.szAcctno, (LPSTR)(LPCSTR)m_InData.m_szGeja, sizeof(signalOrdersInfo.szAcctno)-1);				// 계좌번호
		memcpy(signalOrdersInfo.szCode, (LPSTR)(LPCSTR)m_InData.m_szCode, sizeof(signalOrdersInfo.szCode)-1);						// 종목코드
		memcpy(signalOrdersInfo.szLoangb, "0", sizeof(signalOrdersInfo.szLoangb)-1);								// 신용구분 : (0)현금
		memcpy(signalOrdersInfo.szJanGogb, "0", sizeof(signalOrdersInfo.szJanGogb)-1);								// 잔고구분 : (0)전체

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

		CString strMsg("시스템트레이딩");
//		if(m_InData.m_nOrderType & VALENTRYSELL  || m_InData.m_nOrderType & VALENTRYBUY)
		if(m_InData.m_nOrderType & VALEXITSHORT  || m_InData.m_nOrderType & VALENTRYBUY)
		{
			if(m_InData.m_nOrderType & VALENTRYBUY)
			{
				strMsg += " 매수진입";
			}
			if(m_InData.m_nOrderType & VALEXITSHORT)
			{
				if(strMsg != "시스템트레이딩")
					strMsg += " /";

				strMsg += " 매도청산";
			}

			memcpy(signalOrdersInfo.szMemegb, "2", sizeof(signalOrdersInfo.szMemegb)-1);							// 매매구분 : (2)매수
			if(m_InData.m_nOrderMarketEntry == 1)		// 시장가
			{
				memcpy(signalOrdersInfo.szOrdgb, "1", sizeof(signalOrdersInfo.szOrdgb)-1);							// 주문구분(보통가, 시장가,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// 주문단가구분(0:지정가...(인덱스))
				memcpy(signalOrdersInfo.szOrdprc, "0", sizeof(signalOrdersInfo.szOrdprc)-1);						// 주문단가
			}
			else
			{
				memcpy(signalOrdersInfo.szOrdgb, "0", sizeof(signalOrdersInfo.szOrdgb)-1);							// 주문구분(보통가, 시장가,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// 주문단가구분(0:지정가...(인덱스))
				memcpy(signalOrdersInfo.szOrdprc, (LPSTR)(LPCSTR)szPrice, sizeof(signalOrdersInfo.szOrdprc)-1);						// 주문단가
			}
		}
//		else if(m_InData.m_nOrderType & VALEXITLONG  || m_InData.m_nOrderType & VALEXITSHORT)
		else if(m_InData.m_nOrderType & VALEXITLONG  || m_InData.m_nOrderType & VALENTRYSELL || m_InData.m_nOrderType & FORCEDEXIT)
		{
			if(m_InData.m_nOrderType & FORCEDEXIT)
			{
				strMsg += " 강제청산";
			}
			if(m_InData.m_nOrderType & VALENTRYSELL)
			{
				if(strMsg != "시스템트레이딩")
					strMsg += " /";

				strMsg += " 매도진입";
			}
			if(m_InData.m_nOrderType & VALEXITLONG)
			{
				if(strMsg != "시스템트레이딩")
					strMsg += " /";

				strMsg += " 매수청산";
			}

			memcpy(signalOrdersInfo.szMemegb, "1", sizeof(signalOrdersInfo.szMemegb)-1);							// 매매구분 : (1)매도
			if(m_InData.m_nOrderMarketEntry == 1)		// 시장가
			{
				memcpy(signalOrdersInfo.szOrdgb, "1", sizeof(signalOrdersInfo.szOrdgb)-1);							// 주문구분(보통가, 시장가,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// 주문단가구분(0:지정가...(인덱스))
				memcpy(signalOrdersInfo.szOrdprc, "0", sizeof(signalOrdersInfo.szOrdprc)-1);						// 주문단가
			}
			else
			{
				memcpy(signalOrdersInfo.szOrdgb, "0", sizeof(signalOrdersInfo.szOrdgb)-1);							// 주문구분(보통가, 시장가,...)
				memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);					// 주문단가구분(0:지정가...(인덱스))
				memcpy(signalOrdersInfo.szOrdprc, (LPSTR)(LPCSTR)szPrice, sizeof(signalOrdersInfo.szOrdprc)-1);						// 주문단가
			}
		}

		memcpy(signalOrdersInfo.szOrdqtygb, "0", sizeof(signalOrdersInfo.szOrdqtygb)-1);							// 주문수량구분(0:수량, 1:금액, 2:매도가능수량%)
		CString strQty;
		strQty.Format("%d", m_InData.m_nQty);
		memcpy(signalOrdersInfo.szOrdqty, (LPSTR)(LPCSTR)strQty, sizeof(signalOrdersInfo.szOrdqty)-1);				// 주문수량
		memcpy(signalOrdersInfo.szbContinue, "1", sizeof(signalOrdersInfo.szbContinue)-1);							// 감시 진행여부
		memcpy(signalOrdersInfo.szMsg, strMsg, sizeof(signalOrdersInfo.szMsg)-1);									// 감시MSG

		::SendMessage(hSignalWnd, UM_SM_SIGNALORDER_MSG, (WPARAM)0L, (long)&signalOrdersInfo);
	}

	if(m_pSTDlgConfirmSendJumun) 
	{
		CWnd* pParentWnd = m_pSTDlgConfirmSendJumun->GetParent();
		pParentWnd->PostMessage(RMSG_DESTROY_CONFIRMWND, (WPARAM)this);
	}
	*/
//>>
	return 1;
}

void CConfirmWnd::ConvertStockHoga(LPVOID aTRData, long dwTRDateLen)
{
	STDR_FID* pStdrFid_Rec1 = (STDR_FID*)aTRData;
	m_strOut = "";

	CString strOut;
	// 메인키
	strOut += pStdrFid_Rec1->vvRpValueList[1][0];
	// 기준가
	strOut += pStdrFid_Rec1->vvRpValueList[1][1];
	// 예상체결가격
	strOut += pStdrFid_Rec1->vvRpValueList[1][2];
	// 예상체결수량
	strOut += pStdrFid_Rec1->vvRpValueList[1][3];
	// 종목명
	strOut += pStdrFid_Rec1->vvRpValueList[1][4];
	// 현재가
	strOut += pStdrFid_Rec1->vvRpValueList[1][5];
	// 등락부호
	strOut += pStdrFid_Rec1->vvRpValueList[1][6];
	// 등락폭
	strOut += pStdrFid_Rec1->vvRpValueList[1][7];
	// 대비율
	strOut += pStdrFid_Rec1->vvRpValueList[1][8];
	// 누적거래량
	strOut += pStdrFid_Rec1->vvRpValueList[1][9];
	// 누적거래대금
	strOut += pStdrFid_Rec1->vvRpValueList[1][10];
	// 매도호가1
	strOut += pStdrFid_Rec1->vvRpValueList[1][11];
	// 매도호가2
	strOut += pStdrFid_Rec1->vvRpValueList[1][12];
	// 매도호가3
	strOut += pStdrFid_Rec1->vvRpValueList[1][13];
	// 매도호가4
	strOut += pStdrFid_Rec1->vvRpValueList[1][14];
	// 매도호가5
	strOut += pStdrFid_Rec1->vvRpValueList[1][15];
	// 매도잔량1
	strOut += pStdrFid_Rec1->vvRpValueList[1][16];
	// 매도잔량2
	strOut += pStdrFid_Rec1->vvRpValueList[1][17];
	// 매도잔량3
	strOut += pStdrFid_Rec1->vvRpValueList[1][18];
	// 매도잔량4
	strOut += pStdrFid_Rec1->vvRpValueList[1][19];
	// 매도잔량5
	strOut += pStdrFid_Rec1->vvRpValueList[1][20];
	// 매수호가1
	strOut += pStdrFid_Rec1->vvRpValueList[1][21];
	// 매수호가2
	strOut += pStdrFid_Rec1->vvRpValueList[1][22];
	// 매수호가3
	strOut += pStdrFid_Rec1->vvRpValueList[1][23];
	// 매수호가4
	strOut += pStdrFid_Rec1->vvRpValueList[1][24];
	// 매수호가5
	strOut += pStdrFid_Rec1->vvRpValueList[1][25];
	// 매수잔량1
	strOut += pStdrFid_Rec1->vvRpValueList[1][26];
	// 매수잔량2
	strOut += pStdrFid_Rec1->vvRpValueList[1][27];
	// 매수잔량3
	strOut += pStdrFid_Rec1->vvRpValueList[1][28];
	// 매수잔량4
	strOut += pStdrFid_Rec1->vvRpValueList[1][29];
	// 매수잔량5
	strOut += pStdrFid_Rec1->vvRpValueList[1][30];
	// 총매도잔량
	strOut += pStdrFid_Rec1->vvRpValueList[1][31];
	// 총매수잔량
	strOut += pStdrFid_Rec1->vvRpValueList[1][32];
	// 시간외매도잔량
	strOut += pStdrFid_Rec1->vvRpValueList[1][33];
	// 시간외 매수잔량
	strOut += pStdrFid_Rec1->vvRpValueList[1][34];
	// 상한가
	strOut += pStdrFid_Rec1->vvRpValueList[1][35];
	// 하한가
	strOut += pStdrFid_Rec1->vvRpValueList[1][36];
	// 예상등락율
	strOut += pStdrFid_Rec1->vvRpValueList[1][37];
	// 고가
	strOut += pStdrFid_Rec1->vvRpValueList[1][38];
	// 저가
	strOut += pStdrFid_Rec1->vvRpValueList[1][39];
	// 전일거래량
	strOut += pStdrFid_Rec1->vvRpValueList[1][40];
	// 상장kospi발행kosdaq 주식수
	strOut += pStdrFid_Rec1->vvRpValueList[1][41];
	//strData = "          ";
	m_strOut = strOut;
}

void CConfirmWnd::ConvertFutOptHoga(LPVOID aTRData, long dwTRDateLen)
{
	STDR_FID* pStdrFid_Rec1 = (STDR_FID*)aTRData;
	m_strOut = "";

	CString strOut;
	// 단축코드
	strOut += pStdrFid_Rec1->vvRpValueList[1][0];
	// 기준가격
	strOut += pStdrFid_Rec1->vvRpValueList[1][1];
	// 상한가
	strOut += pStdrFid_Rec1->vvRpValueList[1][2];
	// 하한가
	strOut += pStdrFid_Rec1->vvRpValueList[1][3];
	// 현재가 
	strOut += pStdrFid_Rec1->vvRpValueList[1][4];
	// 매도호가1
	strOut += pStdrFid_Rec1->vvRpValueList[1][5];
	// 매도호가2
	strOut += pStdrFid_Rec1->vvRpValueList[1][6];
	// 매도호가3
	strOut += pStdrFid_Rec1->vvRpValueList[1][7];
	// 매수호가1
	strOut += pStdrFid_Rec1->vvRpValueList[1][8];
	// 매수호가2
	strOut += pStdrFid_Rec1->vvRpValueList[1][9];
	// 매수호가3
	strOut += pStdrFid_Rec1->vvRpValueList[1][10];
	m_strOut = strOut;
}

void CConfirmWnd::ConvertStockFutHoga(LPVOID aTRData, long dwTRDateLen)
{
	STDR_FID* pStdrFid_Rec1 = (STDR_FID*)aTRData;
	m_strOut = "";

	CString strOut;
	// 단축코드
	strOut += pStdrFid_Rec1->vvRpValueList[1][0];
	// 기준가격
	strOut += pStdrFid_Rec1->vvRpValueList[1][1];
	// 상한가
	strOut += pStdrFid_Rec1->vvRpValueList[1][2];
	// 하한가
	strOut += pStdrFid_Rec1->vvRpValueList[1][3];
	// 현재가 
	strOut += pStdrFid_Rec1->vvRpValueList[1][4];
	// 매도호가1
	strOut += pStdrFid_Rec1->vvRpValueList[1][5];
	// 매도호가2
	strOut += pStdrFid_Rec1->vvRpValueList[1][6];
	// 매도호가3
	strOut += pStdrFid_Rec1->vvRpValueList[1][7];
	// 매수호가1
	strOut += pStdrFid_Rec1->vvRpValueList[1][8];
	// 매수호가2
	strOut += pStdrFid_Rec1->vvRpValueList[1][9];
	// 매수호가3
	strOut += pStdrFid_Rec1->vvRpValueList[1][10];
	m_strOut = strOut;
}

void CConfirmWnd::ConvertStockOptHoga(LPVOID aTRData, long dwTRDateLen)
{
	STDR_FID* pStdrFid_Rec1 = (STDR_FID*)aTRData;
	m_strOut = "";

	CString strOut;
	// 단축코드
	strOut += pStdrFid_Rec1->vvRpValueList[1][0];
	// 기준가격
	strOut += pStdrFid_Rec1->vvRpValueList[1][1];
	// 상한가
	strOut += pStdrFid_Rec1->vvRpValueList[1][2];
	// 하한가
	strOut += pStdrFid_Rec1->vvRpValueList[1][3];
	// 현재가 
	strOut += pStdrFid_Rec1->vvRpValueList[1][4];
	// 매도호가1
	strOut += pStdrFid_Rec1->vvRpValueList[1][5];
	// 매도호가2
	strOut += pStdrFid_Rec1->vvRpValueList[1][6];
	// 매도호가3
	strOut += pStdrFid_Rec1->vvRpValueList[1][7];
	// 매수호가1
	strOut += pStdrFid_Rec1->vvRpValueList[1][8];
	// 매수호가2
	strOut += pStdrFid_Rec1->vvRpValueList[1][9];
	// 매수호가3
	strOut += pStdrFid_Rec1->vvRpValueList[1][10];
	m_strOut = strOut;
}
