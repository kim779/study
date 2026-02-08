// StdDialog.cpp : implementation file
//
#include "stdafx.h"
#include "StdDialog.h"
//#include "TabDlg.h"
#include "TabStockDlg.h"
#include "TabForeignDlg.h"

void CStdDialog::DoSetPartnerSite(IPartnerManager* pPtManager)
{
	m_pPTManager = pPtManager;
	m_szPTManager.Format("%08X", pPtManager);
	if(pPtManager) pPtManager->SetSite(&m_xPartnerSite);
}

HRESULT CStdDialog::CPartnerSite::RecieveFidData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveTRData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	pThis->ReceiveData(0, szTR, szMsgCode, szMessage, aTRData, dwTRDateLen);

	return S_OK;
}


HRESULT CStdDialog::CPartnerSite::RecieveRealData(long dwKey, LONG dwData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)
		
	pThis->m_ChartMng.SetRealJPriceData((LPVOID)dwData);
	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveKeyData(int nKey, LPCSTR _szData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)
		
	switch(nKey)
	{
		case IPartnerSite::IPS_RK_shareCode:	//0
			//WPARAM 종목코드, LPARAM MarketType	
			break;

		//case 1: //종목Dialog에서 선택한 Code
		case IPartnerSite::IPS_RK_codeDlg: //종목Dialog에서 선택한 Code
			{
				//CString szSymbol;
				//g_pPtUtilMng->GetSymboCode(_szData, szSymbol);
				
				CString szCode(_szData);
				int nPos = szCode.Find('\t');
				if(nPos>0) szCode = szCode.Left(nPos);
				
				//WPARAM wp = (WPARAM)(LPCSTR)szCode;
				//LPARAM lp = pThis->m_nChartType;
				//pThis->SendMessage(UMSG_SENDSHAREDCODE, wp, lp);
				int nCodeType;
				int nMarketType = pThis->GetMarketTypeFromCode(szCode, nCodeType);
//				if(pThis->m_nMarketType == nMarketType)
//					pThis->ChangeCode(szCode);

				CTabDlg *pTabDlg = pThis->m_pLeftBarDlg->m_tabctrl.GetWndCodeSearch();
				if(pTabDlg)
				{
					switch(pThis->m_nScreenType)
					{
					case Screen_Compchart:
						((CTabStockDlg*)pTabDlg)->SetCodeFromOutSide(szCode);
							break;
					case Screen_ForeignChart:
						((CTabForeignDlg*)pTabDlg)->SetCodeFromOutSide(szCode);
						break;
					}
				}
			}
			break;
		case IPartnerSite::IPS_RK_gwansim:
			{
				//관심그룹이 변경되었다는 메시지를 받아서 관심그룹 갱신을 요청함.
				if(pThis->m_pGwanMng)
					pThis->m_pGwanMng->RequestGwansimGroup();
			}		
			break;
	}
	
	return S_OK;
}


HRESULT CStdDialog::CPartnerSite::RecieveRealDataEx(long dwKey, LPCTSTR pszCode, LONG dwData)
{
/*
	METHOD_PROLOGUE(CStdDialog, PartnerSite)
//	pThis->m_ChartMng.ReceiveRealHogaData(dwKey, pszCode, dwData);
*/
	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::ReceiveOnUser(WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}
