// FONetworkModule.cpp: implementation of the CFONetworkModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "networkdefine.h"
#include "FONetworkModule.h"
#include "BaseNetworkModule.h"
#include "networkmodule.h"
#include "exportfunction.h"
#include "../../h/axisfire.h"
#include "../common/global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#define	__SISE_TEST__

CFONetworkModule::CFONetworkModule(CWnd* pWndMainFrame)
{
	m_pParentWnd = NULL;
	m_pBaseWnd   = NULL;
	m_pNetwork	 = NULL;
	m_bRTData    = TRUE;
	m_pMonthOpt  = NULL;

	//////////////////////////////////////////////////////////////////////////
	// 김성곤 추가 2003.02.06
	m_RankBit	= NULL;								// 전송 데이터 구분을 위한 데이터
	//////////////////////////////////////////////////////////////////////////

	m_pMarginInfo = NULL;
	m_pWndMainFrame = pWndMainFrame;

	m_bCommUseBaseModule = FALSE;
}

CFONetworkModule::CFONetworkModule(CWnd * pWnd, CWnd* pWndMainFrame)
{
	m_pParentWnd = pWnd;
	m_pBaseWnd   = NULL;
	m_pNetwork   = NULL;
	m_bRTData    = TRUE;
	m_pMonthOpt  = NULL;
	m_pWndMainFrame = pWndMainFrame;

	HWND hWnd = FindWnd("HADT_BASE_NETWORK_MODULE", "HADT_FO_NETWORK_WND");
	if(hWnd == NULL)
	{
//		m_pBaseWnd = new CBaseNetworkModule;
//		m_pBaseWnd->AddFONetwork(this);
//		m_pBaseWnd->CreateBaseNetworkWnd(10000, AfxGetMainWnd());
	}
	else
	{
		//m_pBaseWnd = CWnd::FromHandle(hWnd);
		//20080316 이문수 메세지 통일>>
		//m_pBaseWnd = (CBaseNetworkModule  *)::SendMessage(hWnd, WM_GET_BASENETWORKMODULE, 0, 0);
		m_pBaseWnd = (CBaseNetworkModule  *)::SendMessage(hWnd, WMM_GET_THIS_WND_PTR, 0, 0);
		//20080316 이문수 <<
		//m_pBaseWnd->SendMessage(WM_ADD_FONETWORKMODULE, NULL, (LPARAM)this);

		if(pWndMainFrame)
			m_bCommUseBaseModule = FALSE;
		else
			m_bCommUseBaseModule = TRUE;

		m_pBaseWnd->AddFONetwork(this);
	}

	SetUnderlyingAssetsArray();

	m_pNetwork = new CNetworkModule(pWnd, this, m_pWndMainFrame);

	//////////////////////////////////////////////////////////////////////////
	// 김성곤 추가 2003.02.06
	m_RankBit	= NULL;								// 전송 데이터 구분을 위한 데이터
	//////////////////////////////////////////////////////////////////////////

	m_pMarginInfo = NULL;
}

void CFONetworkModule::SetUnderlyingAssetsArray()
{
	if(!m_pBaseWnd)	return;

	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++) delete m_UnderlyingAssetsInfoArray.GetAt(i);
	m_UnderlyingAssetsInfoArray.RemoveAll();

	nSize = m_pBaseWnd->m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyingInfo = m_pBaseWnd->m_UnderlyingAssetsInfoArray.GetAt(i);
		CUnderlyingAssetsInfo * pNewUnderlyingInfo = new CUnderlyingAssetsInfo;
		*pNewUnderlyingInfo = *pUnderlyingInfo;
	
		m_UnderlyingAssetsInfoArray.Add(pNewUnderlyingInfo);
	}

	nSize = m_FutureMonthInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++) delete m_FutureMonthInfoArray.GetAt(i);
	m_FutureMonthInfoArray.RemoveAll();

	nSize = m_pBaseWnd->m_FutureMonthInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CItemDataInfo * pFuture = m_pBaseWnd->m_FutureMonthInfoArray.GetAt(i);
		CItemDataInfo * pNewFuture = new CItemDataInfo;
		*pNewFuture = *pFuture;
		m_FutureMonthInfoArray.Add(pNewFuture);
	}
}

CFONetworkModule::~CFONetworkModule()
{
	if(m_pBaseWnd && 
		::IsWindow(m_pBaseWnd->GetSafeHwnd()))
		m_pBaseWnd->DelFONetwork(this);	

	if(m_pNetwork)
	{
		delete m_pNetwork;
		m_pNetwork = NULL;
	}

	int i, nSize = m_ReceiveDataArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
		delete m_ReceiveDataArray.GetAt(i);
	m_ReceiveDataArray.RemoveAll();

	nSize = m_RTDataArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
		delete m_RTDataArray.GetAt(i);
	m_RTDataArray.RemoveAll();
	
	nSize = m_TheoryForMarginArray.GetSize();
	for(i = 0 ; i < nSize ;i++)
		delete m_TheoryForMarginArray.GetAt(i);
	m_TheoryForMarginArray.RemoveAll();

	nSize = m_RqCodeArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
		delete m_RqCodeArray.GetAt(i);
	m_RqCodeArray.RemoveAll();

	if(m_pMonthOpt)
		delete m_pMonthOpt;

	nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++) delete m_UnderlyingAssetsInfoArray.GetAt(i);
	
	nSize = m_FutureMonthInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++) delete m_FutureMonthInfoArray.GetAt(i);

	if(m_pMarginInfo)	delete m_pMarginInfo;

	//20080304 이문수 >>
	for(i = 0 ; i < m_IVItemArray.GetSize(); i++) delete m_IVItemArray.GetAt(i);	
	//20080304 이문수 <<

	//20080308 이문수 >>
	while(!m_PriceProgressInputList.IsEmpty()) delete m_PriceProgressInputList.RemoveHead();
	//20080308 이문수 <<

}

//20080304 이문수 >>
int CFONetworkModule::RequestIVDate(CDataArray & DataArray)
{
	int i;
	for(i = 0 ; i < m_IVItemArray.GetSize(); i++) delete m_IVItemArray.GetAt(i);	
	m_IVItemArray.RemoveAll();	

	if(!m_pNetwork) return 0;

	int nResult = m_pNetwork->RequestIVDate(DataArray);

	m_nRequestDataType = IV_DATE_INFO;

	return TRUE;
}
//20080304 이문수 <<

//int CFONetworkModule::RequestOptionPastIV(CString strEndDate)
//{
// 	if(!m_pNetwork) return FALSE;
// 	m_nRequestDataType = OPTION_PAST_IV_20;
// 	return m_pNetwork->RequestOptionPastIV(strEndDate);
//}

int CFONetworkModule::RequestMonthData(CString strUnderlyingAssetsCode, CString strYM)
{	
	if(!m_pNetwork) return FALSE;

	m_nRequestDataType = MONTH_OPT_INFO;

	return m_pNetwork->RequestMonthData(strUnderlyingAssetsCode, strYM);
}

int CFONetworkModule::RequestMarginInfo(CString strAccount, CString strPassword)
{
	if(!m_pNetwork) return FALSE;

//	m_nRequestDataType = MARGIN_INFO;

	m_pNetwork->RequestMarginInfo(strAccount, strPassword);

	return 1;
}

/* 선물 계좌를 입력하면 자동으로 선물, 주식계좌잔고를 연속적으로 불러오는 함수 */
int CFONetworkModule::RequestData(CString strAccount, CString strPassword)
{
	m_AccountInfo.DeleteArray();

	if(!m_pNetwork) return 0;

	int nResult = m_pNetwork->RequestData(strAccount, strPassword);

	m_nRequestDataType = ACCOUNT_INFO;

	return nResult;
}

/* 입력된 계좌에 대해서만 잔고를 불러오는 함수 */
int CFONetworkModule::RequestAccountData(CString strAccount, CString strPassword)
{
	m_AccountInfo.DeleteArray();

	if(!m_pNetwork) return 0;

	int nResult = m_pNetwork->RequestAccountData(strAccount, strPassword);

	m_nRequestDataType = ACCOUNT_INFO;

	return nResult;
}

int CFONetworkModule::RequestDeltaSum(void* pInput)
{
	if(!m_pNetwork) return 0;

	int nResult = m_pNetwork->RequestDeltaSum(pInput);
	
	return nResult;
}

int CFONetworkModule::RequestData(CDataSet * pDataSet, BOOL bRTData)
{
	int i, nSize = m_ReceiveDataArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
		delete m_ReceiveDataArray.GetAt(i);
	m_ReceiveDataArray.RemoveAll();

	nSize = m_RqCodeArray.GetSize();	
	for(i = 0 ; i < nSize ; i++)
		delete m_RqCodeArray.GetAt(i);
	m_RqCodeArray.RemoveAll();

	if(!m_pNetwork) return 0;

	CDataSet * pNewSet = new CDataSet;
	*pNewSet = *pDataSet;

	m_RqCodeArray.Add(pNewSet);

	// 통신 요청
	int nResult = m_pNetwork->RequestData(pNewSet->m_strCode);
	
	m_bRTData = bRTData;

	m_nRequestDataType = SISE_INFO;

	return nResult;
}

int CFONetworkModule::RequestTheoryForMargin(CDataArray & DataArray)
{
	int i, nSize = m_TheoryForMarginArray.GetSize();
	for(i = 0 ; i < nSize ;i++)
		delete m_TheoryForMarginArray.GetAt(i);
	m_TheoryForMarginArray.RemoveAll();

	if(!m_pNetwork) return 0;

	int nResult = m_pNetwork->RequestTheoryForMargin(DataArray, m_pBaseWnd->m_BaseDataSet.m_strTodayDate);

	m_nRequestDataType = THEORY_FOR_MARGIN;

	return nResult;
}

int CFONetworkModule::RequestAverageIV(CString strUnderlyingAssetsCode)
{
	if(!m_pNetwork)	return 0;

	int nResult;

	if(strUnderlyingAssetsCode.GetLength() == 2)
	{//short code로 들어온 경우.
		nResult = m_pNetwork->RequestAverageIV(strUnderlyingAssetsCode);
		m_nRequestDataType = AVERAGE_IV;
		return nResult;
	}
	else if(strUnderlyingAssetsCode.GetLength() >= 5)
	{//full code로 들어온 경우. -> short code로 변환...
		int i,nSize = m_pBaseWnd->m_UnderlyingAssetsInfoArray.GetSize();
		for(i = 0 ; i< nSize ;i ++)
		{
			CUnderlyingAssetsInfo * pUnderlyingInfo = m_pBaseWnd->m_UnderlyingAssetsInfoArray.GetAt(i);
			if(pUnderlyingInfo->m_strUnderlyingAssetsCode == strUnderlyingAssetsCode)
			{
				nResult = m_pNetwork->RequestAverageIV(pUnderlyingInfo->m_strShortCode);
				m_nRequestDataType = AVERAGE_IV;
				return nResult;
			}
		}
		return 0;
	}
	else
		return 0;	
}

int CFONetworkModule::RequestData(CDataArray & DataArray, BOOL bRTData)
{
	int i, nSize = m_ReceiveDataArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
		delete m_ReceiveDataArray.GetAt(i);
	m_ReceiveDataArray.RemoveAll();

	nSize = m_RqCodeArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
		delete m_RqCodeArray.GetAt(i);
	m_RqCodeArray.RemoveAll();

	if(!m_pNetwork) return 0;

	nSize = DataArray.GetSize();
	if(nSize <= 0) return 0;
	
	int nResult;

	for(i =  0 ; i < nSize ; i++)
	{
		CDataSet * pDataSet = DataArray.GetAt(i);		
		CDataSet * pNewSet  = new CDataSet;
		*pNewSet = *pDataSet;
		m_RqCodeArray.Add(pNewSet);

		// 시세 요청
		//nResult = m_pNetwork->RequestData(pDataSet->m_strCode);
	}

	CDataSet * pSet = m_RqCodeArray.GetAt(0);	
	nResult = m_pNetwork->RequestData(pSet->m_strCode);	

	m_bRTData = bRTData;

	m_nRequestDataType = SISE_INFO;

	return nResult;
}

int CFONetworkModule::ReceiveData(WPARAM wParam, LPARAM lParam)
{
	if(!m_pNetwork) return 0;

	LONG ret = 0;
	int nKind = (int)(HIBYTE(LOWORD(wParam)));
	int len = HIWORD(wParam);

	switch(nKind)
	{
	case TR_REMAIN: // 계좌 잔고 조회
		{
			int nResult;
			if((nResult = m_pNetwork->ReceiveData(wParam, lParam, &m_AccountInfo)) == SUCCESS)
			{
				m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)ACCOUNT_INFO, (LPARAM)&m_AccountInfo);
				return 1;
			}
			else if(nResult == CONTINUE)
				return 0;
			else
				m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)ACCOUNT_INFO, (LPARAM)&m_AccountInfo);
		}	
		break;
	case TR_KP200:	// KOSPI200 조회
		break;

	case TR_MARGININFO:
		{
			if(m_pMarginInfo)	delete m_pMarginInfo;
			m_pMarginInfo = new CMarginInfo;

			if(m_pNetwork->ReceiveMarginInfo(wParam, lParam, *m_pMarginInfo) == SUCCESS)
			{
				m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)MARGIN_INFO, (LPARAM)m_pMarginInfo);
				return 1;
			}
			else
				m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)MARGIN_INFO, 0);
		}
		break;

	case TR_THEORY4MARGIN:
		if(m_pNetwork->ReceiveData(wParam, lParam, m_TheoryForMarginArray) == SUCCESS)
		{
			m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)THEORY_FOR_MARGIN, (LPARAM)&m_TheoryForMarginArray);
			return 1;
		}
		else
			m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)THEORY_FOR_MARGIN, 0);

		break;

	//20080301 이문수 >>
	case TR_TOTALIV:
		if(m_pNetwork->ReceiveData(wParam, lParam, m_TotalIVArray) == SUCCESS)
		{
			m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)TOTAL_IV_INFO, (LPARAM)&m_TotalIVArray);
			for(int i = 0 ; i < m_TotalIVArray.GetSize(); i++) delete m_TotalIVArray.GetAt(i);
			m_TotalIVArray.RemoveAll();
			return 1;
		}
		else
			m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)TOTAL_IV_INFO, 0);
		break;	
	//20080301 이문수 <<
	case TR_HIS_IV_TOT_IV_DATE:
		if(m_pNetwork->ReceiveData(wParam, lParam, m_IVItemArray) == SUCCESS)
		{
			if(m_pNetwork->m_RqDataSetArray.GetSize() <= 0)
			{			
				m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)IV_DATE_INFO, (LPARAM)&m_IVItemArray);
				for(int i = 0 ; i < m_IVItemArray.GetSize(); i++) delete m_IVItemArray.GetAt(i);
				m_IVItemArray.RemoveAll();
			}
			return 1;
		}
		else
			m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)IV_DATE_INFO, 0);
		break;
		
	//20080306 이문수 >>
	case TR_PRICEPROGRESS:
		{
			CPriceProgressData PriceProgressData;
				
			m_iPriceProgressRequestCount--;

			if(m_PriceProgressInputList.GetCount() > 0)
				PriceProgressData.m_iReceiveMode = 1;
			else
				PriceProgressData.m_iReceiveMode = 2;

			int nResult;

			CString strStartDate, strStartTime;
			if((nResult = m_pNetwork->ReceiveData(wParam, lParam, PriceProgressData.m_arrPriceProgressInfo, m_RankBit, strStartDate, strStartTime)) == SUCCESS)
			{				
				m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)PRICE_PROGRESS_INFO, (LPARAM)&PriceProgressData);
				
				POSITION pos, pos1;
				CStringArray strCodeArray;
				int i, nDataCount = 0, nDataBit = 0;				
				for( i = 0 , pos = m_PriceProgressInputList.GetHeadPosition(); (pos1 = pos) ; i++)
				{
					CPriceProgressInput * pInput = m_PriceProgressInputList.GetNext(pos);
					strCodeArray.Add(pInput->m_strCode);

					if( i == 0 )
					{
						nDataBit	= pInput->m_nDataBit;
						nDataCount	= pInput->m_nDataCount;
					}

					delete pInput;
					m_PriceProgressInputList.RemoveAt(pos1);

					if(strCodeArray.GetSize() >= J85OI310_Grid_IN_MAX_CNT)					
						break;					
				}

				if(strCodeArray.GetSize() > 0)
					m_pNetwork->RequestPriceProgressData(strCodeArray, strStartDate, strStartTime, nDataCount, nDataBit, m_RankBit);
				
				return 1;
			}
			else 
				m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)PRICE_PROGRESS_INFO	, 0);
		}
		break;
	//20080306 이문수 <<

	default : // 시세 조회용 => 리얼 자동 요청 됨
		if(nKind >= 50) // m_KeyManager에서 조회용으로 50~255까지 키를 관리하고 있다.
		{
			CDataSet * pDataSet = new CDataSet;

			if(m_pNetwork->ReceiveData(wParam, lParam, pDataSet))
			{
				// 속도를 위해서 리얼종목이 배열에 남아 있으면 먼저 조회
				if(m_RqCodeArray.GetSize() > 1)
				{
					CDataSet * pSet = m_RqCodeArray.GetAt(1);					
					m_pNetwork->RequestData(pSet->m_strCode);
				}

				m_ReceiveDataArray.Add(pDataSet);

				if(m_RqCodeArray.GetSize() > 0)
				{

					CDataSet * pRqSet = m_RqCodeArray.GetAt(0);
					pDataSet->m_nIDArray.Append(pRqSet->m_nIDArray);
					pDataSet->m_strCount		= pRqSet->m_strCount;
					pDataSet->m_strTradeType	= pRqSet->m_strTradeType;
					delete pRqSet;
					m_RqCodeArray.RemoveAt(0);


					if(m_bRTData)	// 같은 종목 등록되어 있을 때 중복 방지		
						FindDataSet(pDataSet);					
					
					if(m_RqCodeArray.GetSize() > 0)
					{
						return 0; //값이 연속됨...
					}
					else
					{				
						m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)SISE_INFO, (LPARAM)&m_ReceiveDataArray);
					
						int i, nSize = m_ReceiveDataArray.GetSize();
						for(i = 0 ; i < nSize ; i++)
							delete m_ReceiveDataArray.GetAt(i);
						m_ReceiveDataArray.RemoveAll();
					}
					return 1;
				}
				else
				{
					m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)SISE_INFO, (LPARAM)&m_ReceiveDataArray);

					int i, nSize = m_ReceiveDataArray.GetSize();
					for(i = 0 ; i < nSize ; i++)
						delete m_ReceiveDataArray.GetAt(i);
					m_ReceiveDataArray.RemoveAll();

					return 1; //성공적으로 모든 데이터를 받았음.
				}
			}
			delete pDataSet;
			//받은 값에 이상이 있음.
			m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)SISE_INFO, (LPARAM)0);
		}
		break;
	}

	return -1;//
}



BOOL CFONetworkModule::DelRTData(CString strCode)
{
	if(!m_pNetwork) return FALSE;

	int i,nSize = m_RTDataArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CDataSet * pDataSet = m_RTDataArray.GetAt(i);
		if(pDataSet->m_strCode == strCode)
		{
			delete pDataSet;
			m_RTDataArray.RemoveAt(i);

			if(m_RTDataArray.GetSize() > 0)
				m_pNetwork->AddRTData(m_RTDataArray);
			else
				m_pNetwork->DelAllData();

			return TRUE;
		}
	}
	return FALSE;
}

BOOL CFONetworkModule::DelRTData(CStringArray strCodeArray)
{
	if(!m_pNetwork) return FALSE;

	int i, j, nSize1, nSize;
	nSize1 = strCodeArray.GetSize();
	for(j = nSize1 - 1 ; j >= 0 ; j--)
	{
		CString strCode = strCodeArray.GetAt(j);
		nSize = m_RTDataArray.GetSize();		
		for(i = 0 ; i < nSize ; i++)
		{
			CDataSet * pDataSet = m_RTDataArray.GetAt(i);
			if(pDataSet->m_strCode == strCode)
			{
				delete pDataSet;
				m_RTDataArray.RemoveAt(i);
				break;
			}
		}
	}
	
	if(m_RTDataArray.GetSize() > 0)
		m_pNetwork->AddRTData(m_RTDataArray);
	else
		m_pNetwork->DelAllData();
	
	return TRUE;
}

BOOL CFONetworkModule::DelRTData(CDataArray & DataArray)
{
	if(!m_pNetwork) return FALSE;

	int iIndex, nCount = DataArray.GetSize();
	for(iIndex = 0 ; iIndex < nCount ; iIndex++)
	{
		CDataSet * pSet = DataArray.GetAt(iIndex);

		int i,nSize = m_RTDataArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CDataSet * pDataSet = m_RTDataArray.GetAt(i);
			if(pDataSet->m_strCode == pSet->m_strCode)
			{				
				if(pSet->m_nIDArray.GetSize() > 0)
				{
					int j,k;
					for(j = 0 ; j < pSet->m_nIDArray.GetSize(); j++)
					{
						int nID = pSet->m_nIDArray.GetAt(j);
						for(k = 0 ; k < pDataSet->m_nIDArray.GetSize(); k++)
						{
							if(nID == (int)pDataSet->m_nIDArray.GetAt(k))
							{
								pDataSet->m_nIDArray.RemoveAt(k);
								break;
							}
						}
					}
					if(pDataSet->m_nIDArray.GetSize() <= 0)
					{
						delete pDataSet;
						m_RTDataArray.RemoveAt(i);
					}
				}
				else
				{			
					delete pDataSet;
					m_RTDataArray.RemoveAt(i);
				}			
				break;
			}
		}
	}

	if(m_RTDataArray.GetSize() > 0)	
		m_pNetwork->AddRTData(m_RTDataArray);
	else
		m_pNetwork->DelAllData();

	return TRUE;
}

BOOL CFONetworkModule::AddRTData(CDataSet * pSet)
{
	if(!m_pNetwork) return FALSE;

	//중복된 종목코드가 있는지 검사한다.
	if(FindDataSet(pSet) != 1) return FALSE;	

	return TRUE;
}

BOOL CFONetworkModule::AddRTData(CDataArray & DataArray)
{
	if(!m_pNetwork) return FALSE;

	//등록된 종목코드가 있는지 검사한다.
	int nResult = -1;
	int j, nSize1;
	nSize1 = DataArray.GetSize();
	for(j = nSize1 - 1 ; j >= 0 ; j--)
	{
		CDataSet * pSet = DataArray.GetAt(j);
		if(FindDataSet(pSet) == 1)		
			nResult = 1;		
	}

	if(nResult == -1) return FALSE;
	
	//같은 종목이 들어있지 않았다면 등록한다.
//	m_pNetwork->AddRTData(m_RTDataArray);
	
	return TRUE;
}

BOOL CFONetworkModule::DelAllRTData()
{
	if(!m_pNetwork) return FALSE;
	
	int i,nSize = m_RTDataArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
		delete m_RTDataArray.GetAt(i);
	m_RTDataArray.RemoveAll();

	m_pNetwork->DelAllData();

	return TRUE;
}

void CFONetworkModule::SendBackKey(CString strCode)
{
	if(m_pNetwork)
		m_pNetwork->m_pKeyManager->SendBackKey(strCode);
}

void CFONetworkModule::ResetKey()
{
	if(m_pNetwork)
		m_pNetwork->m_pKeyManager->ResetKey();
}

BOOL CFONetworkModule::ReceiveRTData(WPARAM wParam, LPARAM lParam)
{
	if(!m_pNetwork)	return FALSE;

	LPTSTR pData = (LPTSTR)lParam;
		
	CString strData(pData);
	CString strKey = Parser(strData, "\t"), strPacket;
	CStringArray arrRealPacket;
	do{
		strPacket = Parser(strData, "\n");
		strPacket = strKey + "\t" + strPacket;
		arrRealPacket.Add(strPacket);
	}while(!strData.IsEmpty());


	int nSize = arrRealPacket.GetSize();

 	for(int nIndex = nSize-1; nIndex >= 0; nIndex--) // 역시간순임 마지막게 오래된것
 	{
		strPacket = arrRealPacket.GetAt(nIndex);

		if(m_ReceiveDataArray.GetSize() > 0)
		{
			CDataArray	ReceiveArray;
			m_pNetwork->ReceiveRTData(wParam, (LPARAM)(LPCTSTR)strPacket, m_ReceiveDataArray, ReceiveArray);

			if(ReceiveArray.GetSize() > 0)
 			{
				for(int i = 0 ; i < ReceiveArray.GetSize(); i++)
				{
					CDataSet * pDataSet = ReceiveArray.GetAt(i);

					for(int k = 0 ; k < m_ReceiveDataArray.GetSize(); k++)
					{
						CDataSet * pRevDataSet = m_ReceiveDataArray.GetAt(k);
						if(pRevDataSet->m_strCode == pDataSet->m_strCode)
						{
							pRevDataSet->m_strCurPrice = pDataSet->m_strCurPrice;
							for(int nHoga = 0 ; nHoga < HOGA_CNT ; nHoga++)
							{
								pRevDataSet->m_BidArray[nHoga] = pDataSet->m_BidArray[nHoga];
								pRevDataSet->m_OfferArray[nHoga] = pDataSet->m_OfferArray[nHoga];
							}			
						}
					}
				}		
			}

		}
		else
		{
			CDataArray	ReceiveArray;
			m_pNetwork->ReceiveRTData(wParam, (LPARAM)(LPCTSTR)strPacket, m_RTDataArray, ReceiveArray);

			if(ReceiveArray.GetSize() > 0)
 			{
				for(int i = 0 ; i < ReceiveArray.GetSize(); i++)
				{
					CDataSet * pDataSet = ReceiveArray.GetAt(i);

					if(m_pParentWnd && IsWindow(m_pParentWnd->GetSafeHwnd()))
					{
						TRACE("FONETWORK RT\n");
						m_pParentWnd->SendMessage(WM_RECEIVE_RT_DATA, (WPARAM)FODATA_TYPE, (LPARAM)pDataSet);
					}
				}		
			}
		}
	}

	return TRUE;
}

//. m_RTDataArray 배열에서 같은 종목이 있는지 찾는다(있으면 0리턴)
//. 없으면 배열에 추가하고  1리턴
int CFONetworkModule::FindDataSet(CDataSet * pDataSet)
{
	if(pDataSet->m_strCode.IsEmpty()) return -1;


	int i = 0, nSize = m_RTDataArray.GetSize();

	for(i = 0 ; i < nSize ; i++)
	{
		CDataSet * pSet = m_RTDataArray.GetAt(i);
		if(pSet->m_strCode == pDataSet->m_strCode)
		{
			for(int j = 0 ; j < pDataSet->m_nIDArray.GetSize(); j++)
			{
				int nID = pDataSet->m_nIDArray.GetAt(j);
				int kSize = pSet->m_nIDArray.GetSize();
				for(int k = 0 ; k < kSize; k++)
				{
					if(nID == (int)pSet->m_nIDArray.GetAt(k)) break;
				}
				if(k == kSize) pSet->m_nIDArray.Add(nID);
			}
			
			//이미 같은 종목이 등록되어 있는 경우....
			return 0;
		}
	}

	//새로운 종목을 등록할 경우.
	CDataSet * pNewSet = new CDataSet;
	*pNewSet = *pDataSet;
	m_RTDataArray.Add(pNewSet);

	return 1;
}

//////////////////////////////////////////////////////////////////////////
// 김성곤 추가 2003.02.06
int CFONetworkModule::RequestPriceProgressData(CStringArray &sarrCode, LPCTSTR pszDate, LPCTSTR pszTime,
											  int iDataCount, char DataBit)
{
	int _j, iCount = sarrCode.GetSize();

	m_iPriceProgressRequestCount = 0;

	m_RankBit++;

	if(m_RankBit == NULL)
		m_RankBit++;
			
	m_nRequestDataType = PRICE_PROGRESS_INFO;

	{
		while(!m_PriceProgressInputList.IsEmpty()) delete m_PriceProgressInputList.RemoveHead();
		m_PriceProgressInputList.RemoveAll();

		// 최대로 보낼수 있는 개수를 넘을때는 최대 개수로 잘라 보내야 한다.
		if(iCount > J85OI310_Grid_IN_MAX_CNT)
		{
			int iPos = 0;
			
			CStringArray sarrTempCode;

			for(_j=0; _j<J85OI310_Grid_IN_MAX_CNT; _j++)
				sarrTempCode.Add(sarrCode.GetAt(_j));			
				
			m_pNetwork->RequestPriceProgressData(sarrTempCode, pszDate, pszTime, iDataCount, DataBit, m_RankBit);
					
			for( _j= 0 ; _j< iCount; _j++ )
				{
				CPriceProgressInput * pInputSet = new CPriceProgressInput;
				pInputSet->m_strCode	= sarrCode.GetAt(_j);
				pInputSet->m_nDataCount	=iDataCount;
				pInputSet->m_nDataBit	= DataBit;
				pInputSet->m_strDate	= pszDate;
				pInputSet->m_strTime	= pszTime;
				m_PriceProgressInputList.AddTail(pInputSet);				
			}
		}
		else
		{
			m_pNetwork->RequestPriceProgressData(sarrCode, pszDate, pszTime, iDataCount, DataBit, m_RankBit);
			m_iPriceProgressRequestCount++;
		}
	}

	return 1;
}

CString CFONetworkModule::Parser(CString &strSrc, CString strSub)
{
	if (strSrc.Find(strSub) == -1)
	{
		CString temp = strSrc;
		strSrc.Empty();
		return temp;
	}
	else
	{
		CString  temp = strSrc.Left(strSrc.Find(strSub));
		strSrc = strSrc.Mid(strSrc.Find(strSub) + strSub.GetLength());
		return temp;
	}
	return "";
}

//20080228 이문수 >>
int CFONetworkModule::RequestTotIV(CString strBaseCode, char chJanFlag, int nTotWorkDateCnt, 
								   CString strStartTime, CString strEndTime)
{
	if(!m_pNetwork) return FALSE;
	
	m_nRequestDataType = TOTAL_IV_INFO;
	m_pNetwork->RequestTotIV(strBaseCode, chJanFlag, nTotWorkDateCnt, strStartTime, strEndTime);
	return 1;
}
//20080228 이문수 <<

CString CFONetworkModule::GetLastErrorMsg()
{
	return m_pNetwork->GetLastErrorMsg();
}

CString CFONetworkModule::GetLastMsg()
{
	return m_pNetwork->GetLastMsg();
}
