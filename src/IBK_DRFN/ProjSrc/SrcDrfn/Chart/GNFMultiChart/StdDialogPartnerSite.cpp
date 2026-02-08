// StdDialog.cpp : implementation file
//
#include "stdafx.h"
#include "MultiChart.h"
#include "StdDialog.h"
#include "TabCodeSearch.h"
#include "../../inc/RealUpdate.h"

#include "../../../SrcSite/include/axisfire.h"
// 차트 주문 연동 - onlyojt (20091204)
#include "../CommonTR/SM_CheJanStruct.h"

void CStdDialog::DoSetPartnerSite(IPartnerManager* pPtManager)
{
	m_pPTManager = pPtManager;
	m_szPTManager.Format("%08X", pPtManager);
	if(pPtManager) pPtManager->SetSite(&m_xPartnerSite);

	if (m_hWinixWnd == NULL)
	{
		IGateManager* pGateMng;
		AfxGetIGateManager(pGateMng);
		if (pGateMng == NULL)
			return;

		IPartnerSvrManager* pMng = (IPartnerSvrManager*)pGateMng->GetInterface(UUID_IPartnerSvrManager);
		if (pMng)
			pMng->GetParentHandle(0, m_hWinixWnd);
	}
}

int CStdDialog::GetMarketTypeFromCode(CString strItemCode, int& nCodeType)
{
	int nRetType = CMasterTypeInfo::NONE_CHART;
	nCodeType = -1;
	if (strItemCode.IsEmpty())
		return -1;

	CString szChkCode;
	szChkCode = strItemCode.Right(2);
	if(szChkCode.Compare(".1")==0)
	{
		nCodeType = gfutureType;
		nRetType = CMasterTypeInfo::STOCK_CHART;
	}
	else
	{
		if (m_hWinixWnd)
		{
			nCodeType = (int)::SendMessage(m_hWinixWnd, WM_USER, MAKEWPARAM(codeTYPE, NULL), (LPARAM)(LPCSTR)strItemCode);
			if (nCodeType == gfutureType)
				nRetType = CMasterTypeInfo::STOCK_CHART;
		}
	}

	return nRetType;
}

HRESULT CStdDialog::CPartnerSite::RecieveFidData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveTRData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	CString strTR(szTR);
//	if (dwKey == TRKEY_WORKDATE)		strTR = TRNO_WORKDATE;
//	else if (dwKey == TRKEY_STOCK_HOGA)	strTR = QUERY_STOCK_HOGA;
	
	pThis->ReceiveData(dwKey, strTR, szMsgCode, szMessage, aTRData, dwTRDateLen);

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveRealData(long dwKey, LONG dwData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	pThis->m_ChartMng.SetRealJPriceData((LPVOID)dwData, STOCK_SISE_PID);
	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveKeyData(int nKey, LPCSTR _szData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	switch(nKey)
	{
		case IPartnerSite::IPS_RK_shareCode:
			//100531-alzioyes: 공유일때는 CHANGE_ITEM 모드만 처리한다.
			if(pThis->m_pInputBarDlg->GetCurrentMode() != CHANGE_ITEM)
				return S_OK;

			//조회중이면 return;
			if (pThis->m_pInputBarDlg->IsWindowEnabled()==FALSE)
				return S_OK;
			else			//WPARAM 종목코드, LPARAM MarketType	
			{
				CString szData(_szData), szCode,szSymbol;
				int nPos = szData.Find('\t');
				if(nPos<0) return FALSE;

				szSymbol = szData.Left(nPos);
				szCode = szData.Mid(nPos+1);
				szCode.TrimRight();

				int nCodeType;
				pThis->GetMarketTypeFromCode(szCode, nCodeType);
				if (nCodeType == gfutureType)
				{
//					pThis->DoNotifyChangeCode(FALSE);
					pThis->ChangeCode(szCode);
//					pThis->DoNotifyChangeCode(TRUE);
				}
			}
			break;
		case IPartnerSite::IPS_RK_codeDlg: //종목Dialog에서 선택한 Code
			if (pThis->m_pInputBarDlg->IsWindowEnabled())
			{
				CString szCode(_szData);
				int nPos = szCode.Find('\t');
				if (nPos > 0)
					szCode = szCode.Left(nPos);

				int nCodeType;
				pThis->GetMarketTypeFromCode(szCode, nCodeType);
				if (nCodeType == gfutureType)
				{
					pThis->ChangeCode(szCode);
					if (g_pPtUtilMng)
						g_pPtUtilMng->AddToHistory(pThis->m_hPlatform, szCode);
				}
			}
			break;

		case IPartnerSite::IPS_RK_codeHidDlg:
			break;
		case IPartnerSite::IPS_RK_gwansim:
			break;
		case IPartnerSite::IPS_RK_SaveKeyEnd:	//사용자저장 및 마지막 저장 사용시처리.
			break;
	}

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveRealDataEx(long dwKey, LPCTSTR pszCode, LONG dwData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::ReceiveOnUser(WPARAM wParam, LPARAM lParam)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	return S_OK;
}

//<<20100412_JS.Kim FX. 체결데이타 실시간 수신
HRESULT CStdDialog::CPartnerSite::ReceiveNoticeData(LPCTSTR pszCode, LONG dwData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	pThis->ReceiveNoticeData(pszCode, dwData);

	return S_OK;
}

void CStdDialog::ReceiveNoticeData(LPCTSTR pszCode, LONG dwData)
{
	FX_CHE_STRUCT* pOrderInfomation = (FX_CHE_STRUCT*)dwData;
	if(!pOrderInfomation)
		return;
			
	CString strCodeInputCode = m_pInputBarDlg->m_pCodeInput->GetCode();

	if(strCodeInputCode.IsEmpty())
		return;

	CString strAccount = "", strAccName = "", strAccPW = "";
	if( m_pExtInputBarDlg )	m_pExtInputBarDlg->GetAccountInfo(strAccount, strAccName, strAccPW);
			
	CString strItemCode (pOrderInfomation->SHCODE,	sizeof(pOrderInfomation->SHCODE));
	CString strAccNo (pOrderInfomation->szAccNo,	sizeof(pOrderInfomation->szAccNo));

	strItemCode.TrimRight ();
	strCodeInputCode.TrimRight ();
	strAccount.TrimRight ();
	strAccNo.TrimRight ();
			
	if (strCodeInputCode == strItemCode && strAccount == strAccNo)
	{
		CString strKeyType(pOrderInfomation->szKeyType, sizeof(pOrderInfomation->szKeyType));

		CChartItem* pChartItem = m_ChartMng.GetpChartItemSelected();
		if(pChartItem)
		{
			if(strKeyType == "CH01" || strKeyType == "CLSL")		// 체결
			{
				pChartItem->InvokeAddIn("ChartOutsideCustomAddIn.dll:ORDER_LINE", 3, (long)pOrderInfomation);
			}
		}
	}
}
//>>