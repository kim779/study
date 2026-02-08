// StdDialog.cpp : implementation file
//
#include "stdafx.h"
#include "MultiChart.h"
#include "StdDialog.h"
#include "TabCodeSearch.h"

void CStdDialog::DoSetPartnerSite(IPartnerManager* pPtManager)
{
	m_pPTManager = pPtManager;
	m_szPTManager.Format("%08X", pPtManager);
	if(pPtManager) pPtManager->SetSite(&m_xPartnerSite);
}

void CStdDialog::_DoCheckMetrixScreen()
{
	//if(m_nScreenID<0367 && m_nScreenID>0371) return;
	if(m_nScreenID<367 && m_nScreenID>371) return;

	CString szCode;
	if( m_ChartMng.GetpChartItemSelected() != NULL)
		szCode = m_ChartMng.GetpChartItemSelected()->GetSelectedItemCode();

	if(!g_pPtUtilMng) return;

	m_nMatrix;
	CString szDomino;
	int nMajor = (int)g_pPtUtilMng->DoWinixVariant(m_hPlatform, majorCC, "");
	szDomino.Format("OnSaveHistory\t%d\t%d\t%s", nMajor, m_nMatrix, szCode);

	g_pPtUtilMng->DoWinixVariant(m_hPlatform, triggerCC, szDomino);

}

HRESULT CStdDialog::CPartnerSite::RecieveFidData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveTRData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
// 	{
// 		CString szDebug;
// 		szDebug.Format("[DRCS][LOAD](%8d) CStdDialog::CPartnerSite::RecieveTRData: Entry\n", __LINE__);
// 		OutputDebugString(szDebug);
// 	}

	METHOD_PROLOGUE(CStdDialog, PartnerSite)

	CString strTR(szTR);
	if (dwKey == TRKEY_WORKDATE)		strTR = TRNO_WORKDATE;
	else if (dwKey == TRKEY_STOCK_HOGA)	strTR = QUERY_STOCK_HOGA;
	else if (dwKey == TRKEY_MSTCODE)	strTR = TRNO_MSTCODE;

	pThis->ReceiveData(dwKey, strTR, szMsgCode, szMessage, aTRData, dwTRDateLen);

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
		case IPartnerSite::IPS_RK_shareCode:
			//매트릭스화면은 공유(도미노)처리를 하지않는다.
			if(pThis->m_bMetrixScreen) return S_OK;

			//100531-alzioyes: 공유일때는 CHANGE_ITEM 모드만 처리한다.
			if(pThis->m_pInputBarDlg->GetCurrentMode() != CHANGE_ITEM)
				return S_OK;

			//조회중이면 return;
			if(pThis->m_pInputBarDlg->IsWindowEnabled()==FALSE)
				return S_OK;

			//WPARAM 종목코드, LPARAM MarketType	
			{
				CString szData(_szData), szCode,szSymbol;
				int nPos = szData.Find('\t');
				if(nPos<0) return FALSE;
				
				szSymbol = szData.Left(nPos);
				szCode = szData.Mid(nPos+1);
				szCode.TrimRight();

				if(g_pPtUtilMng)
					if(g_pPtUtilMng->IsCodeSymbol(szSymbol)==FALSE)
						return S_OK;

				//WPARAM wp = (WPARAM)(LPCSTR)szCode;
				//LPARAM lp = pThis->m_nChartType;
				//pThis->SendMessage(UMSG_SENDSHAREDCODE, wp, lp);
				int nCodeType;
				int nMarketType = pThis->GetMarketTypeFromCode(szCode, nCodeType);
				// 0026192: [7114] 선옵종합차트 화면 - 화면 연동 시 차트화면 멈춤 및 안내 멘트 팝업되는 현상
				if (nCodeType == 0xff)
					return S_OK;

				if (pThis->m_nChartType>SINGLE_CHART && 
					(nMarketType==CMasterTypeInfo::STOCKFUTOPT_CHART || nMarketType==CMasterTypeInfo::FUOPT_JIPYO_CHART))
					return S_OK;

//{ //@alzioyes-A001
//	CString szDebug;
//	szDebug.Format("[%s]%08X]symbol=%s, code=%s\n", pThis->GetScreenNo(), this, szSymbol, szCode);
//	OutputDebugString(szDebug);
//}
				//@Solomon	미니종합차트에서 선/옵,업종이 공유되는 현상을 막음.
//				if (pThis->m_nChartType == MINI_SINGLE2_CHART && nCodeType != CMasterTypeInfo::STOCK_CHART)
				//0006516: [7117] 미니차트 - 코스닥 종목의 종목공유가 안되는 현상 수정.
				if (pThis->m_nChartType == MINI_SINGLE2_CHART && nMarketType != CMasterTypeInfo::STOCK_CHART)
					return S_OK;

				if( pThis->IsOpenLoaded() && pThis->m_nMarketType == nMarketType )
				{
					//@Solomon	과거종목일 경우 종목 공유 없음.	-->
					if (pThis->m_pInputBarDlg && pThis->m_pInputBarDlg->m_pFOHisBtn)
					{
						nCodeType = pThis->m_pInputBarDlg->m_pFOHisBtn->GetCodeType(szCode);
						switch (nCodeType)
						{
							case futureType:	// 선물
//KHD							case spreadType:	// 스프레드
							case callType:		// 콜옵션
							case putType:		// 풋옵션
							case sfutureType:	// 주식선물
							case koptionType:	// 주식옵션
								break;
							default:
								return S_OK;
						}
						//@Solomon	과거종목일 경우 종목 공유 없음.	<--
						pThis->DoNotifyChangeCode(FALSE);
						pThis->ChangeCode(szCode);
						pThis->DoNotifyChangeCode(TRUE);
					}
					else
					{
						pThis->ChangeCode(szCode);

						//@alzioyes-A001 
						pThis->DoShareCode(szCode);						
						if(g_pPtUtilMng)
							g_pPtUtilMng->AddToHistory(pThis->m_hPlatform, szCode);
					}
				}
			}
			break;
		case IPartnerSite::IPS_RK_codeDlg: //종목Dialog에서 선택한 Code
			//조회중이면 return;
			if(pThis->m_pInputBarDlg->IsWindowEnabled()==FALSE)
				return S_OK;

			{
				//CString szSymbol;
				//g_pPtUtilMng->GetSymboCode(_szData, szSymbol);

				CString szCode(_szData);
				int nPos = szCode.Find('\t');
				if(nPos>0) szCode = szCode.Mid(nPos);

				szCode.TrimLeft();szCode.TrimRight();
				//빈종목넘기면 처리안한다.
				if(szCode.GetLength()==0) return S_OK;

				//WPARAM wp = (WPARAM)(LPCSTR)szCode;
				//LPARAM lp = pThis->m_nChartType;
				//pThis->SendMessage(UMSG_SENDSHAREDCODE, wp, lp);
				int nCodeType;
				int nMarketType = pThis->GetMarketTypeFromCode(szCode, nCodeType);
				// 0026192: [7114] 선옵종합차트 화면 - 화면 연동 시 차트화면 멈춤 및 안내 멘트 팝업되는 현상
				if (nCodeType == 0xff)
					return S_OK;

				if (pThis->m_nChartType == SINGLE_CHART &&
					pThis->m_nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART &&
					nMarketType == CMasterTypeInfo::FUTOPT_CHART)
				{
					pThis->ChangeStockMode(CMasterTypeInfo::FUTOPT_CHART);
				}

				if(pThis->m_nMarketType == nMarketType)
				{
					//@Solomon	과거종목일 경우 종목 공유 없음.	-->
					if (pThis->m_pInputBarDlg && pThis->m_pInputBarDlg->m_pFOHisBtn)
					{
						nCodeType = pThis->m_pInputBarDlg->m_pFOHisBtn->GetCodeType(szCode);
						switch (nCodeType)
						{
						case futureType:	// 선물
//KHD						case spreadType:	// 스프레드
						case callType:		// 콜옵션
						case putType:		// 풋옵션
						case sfutureType:	// 주식선물
						case koptionType:	// 주식옵션
							return S_OK;
						}
					}
					//@Solomon	과거종목일 경우 종목 공유 없음.	<--
					if(pThis->m_pInputBarDlg->GetCurrentCode() != szCode)
						pThis->ChangeCode(szCode);
					if(g_pPtUtilMng) g_pPtUtilMng->AddToHistory(pThis->m_hPlatform, szCode);
				}
			}
			break;
		case IPartnerSite::IPS_RK_codeHidDlg:
			//조회중이면 return;
			if(pThis->m_pInputBarDlg->IsWindowEnabled()==FALSE)
				return S_OK;

			{
				CString szCode(_szData);

//				int nMarketType = CMasterTypeInfo::FUTOPT_CHART;
				int nCodeType;
				int nMarketType = pThis->GetMarketTypeFromCode(szCode, nCodeType);
				// 0026192: [7114] 선옵종합차트 화면 - 화면 연동 시 차트화면 멈춤 및 안내 멘트 팝업되는 현상
				if (nCodeType == 0xff)
					return S_OK;
					
				if(pThis->m_nMarketType == nMarketType)
//				if (nMarketType == CMasterTypeInfo::FUTOPT_CHART || nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
				{
					pThis->ChangeCode(szCode);
					if(g_pPtUtilMng) g_pPtUtilMng->AddToHistory(pThis->m_hPlatform, szCode);
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

		case IPartnerSite::IPS_RK_SaveKeyEnd:
			{
				CString szSaveKey1(_szData);
				CString szOldSaveKey;
				if(szSaveKey1.Find("USERSCREEN")==0)				
				{
					szOldSaveKey = pThis->GetSaveKey();
					pThis->SetSaveKey(_szData);

					pThis->SaveShowInfo();
					pThis->m_ChartMng.SaveChart(pThis->m_strConfig);
					if(pThis->m_pInputBarDlg)
 						pThis->m_pInputBarDlg->SaveValue();
					if(pThis->m_pExtInputBarDlg)
						pThis->m_pExtInputBarDlg->SaveExtInputBar();

					pThis->SetSaveKey(szOldSaveKey);
				}
				else
				{
					pThis->SetSaveKey(_szData);
				}
			}
			break;
	}

	return S_OK;
}

HRESULT CStdDialog::CPartnerSite::RecieveRealDataEx(long dwKey, LPCTSTR pszCode, LONG dwData)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)
		
	pThis->m_ChartMng.ReceiveRealHogaData(dwKey, pszCode, dwData);

	return S_OK;
}

#define		DLL_Check_AdviseCode		0x80
//--> 0027629: [0000] 서버로 부터 조회되는 차트모듈 관한 확인 및 수정요청
#define TRKEY_MEMO_POPUPOOP		118
#define TRKEY_MEMO_CHECK		124
//<--
HRESULT CStdDialog::CPartnerSite::ReceiveOnUser(WPARAM wParam, LPARAM lParam)
{
	METHOD_PROLOGUE(CStdDialog, PartnerSite)
		
	BYTE bt = LOBYTE(LOWORD(wParam));
	switch (bt)
	{
	case DLL_DOMINO:		// 화면 열리면서 데이터 공유 받을 때
		{
			pThis->m_mapDominoCode.RemoveAll();
			
			CString szData((char*)lParam), strTemp, strTemp2, strKey, strValue;
			CMapStringToString strMap;
			strTemp = szData;
			strTemp += _T("\n");

			int nFind = strTemp.Find(_T("\n"));
			while (nFind>=0)
			{
				strTemp2 = strTemp.Left(nFind);
				strTemp = strTemp.Mid(nFind+1);
				nFind = strTemp2.Find(_T("\t"));
				if (nFind >= 0)
				{
					strKey = strTemp2.Left(nFind);
					strValue = strTemp2.Mid(nFind+1);
					strMap.SetAt(strKey, strValue);
					
					if (strKey[0]>='1' && strKey[0]<='9')
					{
						if (strKey.Find(_T("301")) > 0)
						{
							pThis->m_mapDominoCode.SetAt(strKey, strValue);
							RecieveKeyData(0, strTemp2);
						}
					}
					else if(strKey.CompareNoCase("gicode") == 0)
					{
						pThis->m_mapDominoCode.SetAt(strKey, strValue);
						RecieveKeyData(0, strTemp2);
					}
					else if (strKey.CompareNoCase(_T("OnLoadHistory")) == 0)
					{
						pThis->m_bMetrixScreen = TRUE;
						//pThis->m_nMatrix = atoi(strValue);
						nFind = strValue.Find(_T("\t"));
						CString sLHIndex, sLHValue;

						if(nFind<0) sLHIndex = strValue;
						else sLHIndex = strValue.Left(nFind);
						pThis->m_nMatrix = atoi(sLHIndex);

						sLHValue = strValue.Mid(nFind+1);
						sLHValue.TrimLeft(); sLHValue.TrimRight();
						if(sLHValue.GetLength()>0)
						{
							CString szInitKey;
							szInitKey.Format("OnLoadHistory_Init_%d", pThis->m_nMatrix);
							pThis->m_mapDominoCode.SetAt(szInitKey, sLHValue);
						}
					}
				}

				nFind = strTemp.Find(_T("\n"));
			}
		}		
		break;
		//Code Cehck.
		//추후 Code관리가 필요할 경우 여기서 개선한다.
		case DLL_Check_AdviseCode:
			break;
		case DLL_OUB: // 0027629: [0000] 서버로 부터 조회되는 차트모듈 관한 확인 및 수정요청
		{
			int key = HIBYTE(LOWORD(wParam));
			if ((key >= TRKEY_MEMO_POPUPOOP && key <= TRKEY_MEMO_CHECK))
			{
				OutputDebugString("[StdDialogPartnerSite]-[DLL_OUB]");
				if (pThis->m_pInputBarDlg && pThis->m_pInputBarDlg->m_pwndSymbol)
				{
					pThis->m_pInputBarDlg->m_pwndSymbol->SendMessage(WM_USER, wParam, lParam);
				}
			}
			break;
		}
	}

	return S_OK;
}

LRESULT CStdDialog::OnRmsgDRUser(WPARAM wp, LPARAM lp)
{
	BYTE bt = LOBYTE(LOWORD(wp));
	BYTE ht = HIBYTE(LOWORD(wp));
	
	switch(bt)
	{
	case drbt_REGWINDOW:
		{
			CString szTmp = (LPCSTR)lp;
			int nPos = szTmp.Find('\t');
			if(nPos<0) return 0L;
			
			CString szName = szTmp.Left(nPos);
			szTmp = szTmp.Mid(nPos+1);
			
			if(szName.CompareNoCase("TabCodeSearch")==0)
				sscanf((LPCSTR)szTmp, "%08X", &m_hTabCodeSearch);
		}
		return 0L;

	case drbt_GETINPUTBARMARKET:
		return m_pInputBarDlg->GetComboMarket();
	default:
		break;
	}
	
	return 0L;
}

