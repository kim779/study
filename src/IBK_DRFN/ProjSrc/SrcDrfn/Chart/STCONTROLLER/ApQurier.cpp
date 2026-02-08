// ApQurier.cpp : implementation file
//

#include "stdafx.h"
#include "ApQurier.h"
//#include "../include/DRDSDEF.H"
#include "APDataItem.h"
#include "STController.h"
//#include "../../COMMON_BASE/drds/DRDSDEF.h"
//#include "../../../include/CommonDef.h"				//KJI 20030305 Add
#include "../Common_ST/STConfigdef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define QUOTESYMBOLSIZE	6
#define FUOPTSYMBOLSIZE	8

/////////////////////////////////////////////////////////////////////////////
// CApQurier

CApQurier::CApQurier()
{
// 20060705 소켓제거
//	m_hSocket = NULL;

//	m_pIDrMLib_CDBMgr = NULL;
//	m_pDrds = NULL;
}

CApQurier::~CApQurier()
{
}


BEGIN_MESSAGE_MAP(CApQurier, CWnd)
	//{{AFX_MSG_MAP(CApQurier)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WMU_REALDATA, OnRealData)
//	ON_MESSAGE( WM_DRDS_DATA, OnDrdsData )
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CApQurier message handlers
HRESULT CApQurier::ReceiveData(HWND hTrComm, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID aTRData, DWORD dwTRDateLen)
{
	//if(!dwTRDateLen)
	//{
	//	CString strMessage;
	//	strMessage += '[';
	//	strMessage += szTR;
	//	strMessage += '[';
	//	strMessage += szMessage;
	//	MessageBox(szMessage);
	//	return 0L;
	//}
	//ST_RECEIVEFROMSERVER stR;
	//stR.m_strTRCode = szTR;
	//stR.m_pBuffer = aTRData;
	//stR.m_dwLength = dwTRDateLen;


	//char    sBuffer [12];           /* out buffer */
	//::memcpy(sBuffer,aTRData,12);
	//CAPDataItem* pQuoteDataIemReceived = (CAPDataItem*)atol(sBuffer);
	//
	//CAPDataItem* pQuoteDataIem = NULL;
	//POSITION posLast = NULL;
	//POSITION pos = m_listRequested.GetHeadPosition();
	//while(pos)
	//{	
	//	posLast = pos;
	//	pQuoteDataIem = (CAPDataItem*)m_listRequested.GetNext(pos);
	//	if(pQuoteDataIem==pQuoteDataIemReceived)
	//	{
	//		pQuoteDataIem->ReceiveAPData(&stR);
	//		m_listRequested.RemoveAt(posLast);
	//		break;
	//	}
	//}
	//
	//m_bRequestingNow = FALSE;
	return S_OK;
};

HRESULT CApQurier::RecieveError(int nError, LPCSTR szMessage)
{
	MessageBox(szMessage);
	return S_OK;
}


//사용 안함..
BOOL CApQurier::Send2Server(LPCTSTR lpTrCode,LPCTSTR lpData,int nSize)
{
//	m_TRComm.SetTR(lpTrCode);
	m_bRequestingNow = TRUE;
	//TestKHD();
//	return m_TRComm.Send2Server(lpData, nSize, FALSE,"N0080000");	
	return FALSE;
}

//일단 데이터를 받을 컨트롤을 찾아야쥐~~ KHD 
void CApQurier::TestKHD(CAPDataItem* pQuoteDataIemReceived)
{
	CAPDataItem* pQuoteDataIem = NULL;
	POSITION posLast = NULL;
	POSITION pos = m_listRequested.GetHeadPosition();
	while(pos)
	{	
		posLast = pos;
		pQuoteDataIem = (CAPDataItem*)m_listRequested.GetNext(pos);
		if(pQuoteDataIem==pQuoteDataIemReceived)
		{
			pQuoteDataIem->ReceiveAPData();
			m_listRequested.RemoveAt(posLast);
			break;
		}
	}

	m_bRequestingNow = FALSE;
}

LRESULT CApQurier::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case UMSG_ST_SEND2SERVER:
		{
			//여기서 로드해야 하나?
			ST_SEND2SERVER *pSt = (ST_SEND2SERVER *)wParam;
			m_listRequested.AddTail(pSt->m_pRequester);
			TestKHD((CAPDataItem*)pSt->m_pRequester);
			//Send2Server(pSt->m_strTRCode,(char*)pSt->m_pBuffer,pSt->m_lLength);
		}
		break;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

void CApQurier::InitDrds()
{
//	m_pDrds = NULL;
//	m_nDrdsInfoCnt = 2;
	
//	m_DrdsInfo[0].DataName = "S31";
//	m_DrdsInfo[0].FieldName = "*";
//	m_DrdsInfo[0].KeyName = "";
//	m_DrdsInfo[0].nID = 100;
//	m_DrdsInfo[0].bAdvise = FALSE;
//
//	m_DrdsInfo[1].DataName = "JS0";
//	m_DrdsInfo[1].FieldName = "*";
//	m_DrdsInfo[1].KeyName = "";
//	m_DrdsInfo[1].nID = 101;
//	m_DrdsInfo[1].bAdvise = FALSE;

//	m_DrdsDLL.SetLibName(NULL);	
//	if (m_DrdsDLL.LoadLib()) m_pDrds = m_DrdsDLL.DrdsInit(m_hWnd);	

	CTime t = CTime::GetCurrentTime();
	m_strDateOfRealTimeData = t.Format("%Y%m%d");	
}

BOOL CApQurier::MakeDrdsConnectAdviseFromCode(int nI)
{	
//	char* DataName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].DataName;
//	char* FieldName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].FieldName;
//	char* KeyName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].KeyName;
//
//	UINT	nID = m_DrdsInfo[nI].nID;
//
//	if(!m_pDrds) {
//		TRACE("CStdDialog::MakeDrdsConnect1() :  DRDS연결을 확인바랍니다.\n");
//		m_pDrds = NULL;
//		return FALSE;
//	}
//	m_DrdsDLL.DrdsConnect(m_pDrds, m_hWnd, DataName);
//	m_DrdsDLL.DrdsAdvise(m_pDrds, m_hWnd, nID, DataName,  FieldName, KeyName);
//	m_DrdsInfo[nI].bAdvise = TRUE;
	return TRUE;
}

BOOL CApQurier::MakeDrdsUnConnectUnAdviseFromCode(int nI)
{
//	if(m_pDrds) {
//		if(!m_DrdsInfo[nI].bAdvise)
//			return TRUE;
//
//		char* DataName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].DataName;
//		char* FieldName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].FieldName;
//		char* KeyName = (LPSTR)(LPCTSTR)m_DrdsInfo[nI].KeyName;
//		UINT	nID = m_DrdsInfo[nI].nID;
//		m_DrdsDLL.DrdsUnadvise( m_pDrds, m_hWnd, nID,
//			DataName, 
//			FieldName, 
//			KeyName);
//		m_DrdsDLL.DrdsClose(m_pDrds, m_hWnd, DataName);		
//
//		m_DrdsInfo[nI].bAdvise = FALSE;
//	}
	return TRUE;
}

/*
void CApQurier::OnCodeUnAdvise(CAPDataItem *pAPDataItem)
{
	CString strCode = pAPDataItem->GetCode();
	if(!strCode.GetLength()) return;
	int nRef = 0;
	m_mapRefCntCodeAdvice.Lookup(strCode,nRef);
	nRef--;
	if(nRef>0)
	{
		m_mapRefCntCodeAdvice.SetAt(strCode,nRef);
		return;
	}

	CString strRealKey;
	ASSERT(m_pIDrMLib_CDBMgr);
	if(strCode.GetLength()==QUOTESYMBOLSIZE)
	{
		strRealKey.Format("%d,%6s", PID_JONGSISE, strCode);
	}
	else if(strCode.GetLength() == FUOPTSYMBOLSIZE ) 
	{
		// 선물 / 옵션 / 코스닥 옵션 / 주식옵션 구분 필요.==================================
		DWORD dw = m_pIDrMLib_CDBMgr->GetCodeType(strCode);
		switch(dw)
		{ 
			case CODE_QFUTURE	:
			case CODE_FSPREAD	:
			case CODE_QFUTURES	:
			case CODE_FUTURE	: //선물 				
				strRealKey.Format("%d,%8s"	, PID_FUTURESISE, strCode);
				break;
			case CODE_OPTION	: //옵션	
				strRealKey.Format("%d,%8s"	, PID_OPTIONSISE, strCode);
				break;
			case CODE_QOPTION	:						
			case CODE_QOPTIONS	: //코스닥 옵션 
				strRealKey.Format("%d,%10s"	, PID_QOPTIONSISE, strCode);
				break;
			case CODE_JOPTION	: //주식 옵션
				strRealKey.Format("%d,%8s"	, PID_JOPTIONSISE, strCode);
				break;
			default:
#ifdef _DEBUG
				AfxMessageBox("선물 옵션 Real 등록 실패");
#endif
				break;
		}				
		//=========================================================
	}
	m_pIDrMLib_CDBMgr->ResetRealUpdate(strRealKey, m_hWnd);
}
*/

/*
void CApQurier::OnCodeAdvise(CAPDataItem *pAPDataItem)
{
	CString strCode = pAPDataItem->GetCode();
	if(!strCode.GetLength()) return;
	CString strRealKey;
	ASSERT(m_pIDrMLib_CDBMgr);
	if(strCode.GetLength()==QUOTESYMBOLSIZE)
	{
		strRealKey.Format("%d,%6s", PID_JONGSISE, strCode);
	}
	else if(strCode.GetLength() == FUOPTSYMBOLSIZE ) 
	{
		// 선물 / 옵션 / 코스닥 옵션 / 주식옵션 구분 필요.==================================
		DWORD dw = m_pIDrMLib_CDBMgr->GetCodeType(strCode);
		switch(dw)
		{ 
			case CODE_QFUTURE	:
			case CODE_FSPREAD	:
			case CODE_QFUTURES	:
			case CODE_FUTURE	: //선물 				
				strRealKey.Format("%d,%8s"	, PID_FUTURESISE, strCode);
				break;
			case CODE_OPTION	: //옵션	
				strRealKey.Format("%d,%8s"	, PID_OPTIONSISE, strCode);
				break;
			case CODE_QOPTION	:						
			case CODE_QOPTIONS	: //코스닥 옵션 
				strRealKey.Format("%d,%10s"	, PID_QOPTIONSISE, strCode);
				break;
			case CODE_JOPTION	: //주식 옵션
				strRealKey.Format("%d,%8s"	, PID_JOPTIONSISE, strCode);
				break;
			default:
#ifdef _DEBUG
				AfxMessageBox("선물 옵션 Real 등록 실패");
#endif
				break;
		}				
		//=========================================================
	}
	m_pIDrMLib_CDBMgr->AddRealUpdate(strRealKey, m_hWnd);
	int nRef = 0;
	m_mapRefCntCodeAdvice.Lookup(strCode,nRef);
	nRef++;
	m_mapRefCntCodeAdvice.SetAt(strCode,nRef);
}
*/

// 20060705 함수인자 제거 : HWND hSocket, long pDBMgr
//BOOL CApQurier::Init(HWND hSocket, long pDBMgr)
BOOL CApQurier::Init()
{
//	if(!hSocket||!pDBMgr) return FALSE;
//	m_hSocket = hSocket;
//	m_TRComm.Create(this);
//	m_TRComm.ChangeCallback(&m_xMyCallback);
//	m_TRComm.SetSocket(m_hSocket);	
//	m_pIDrMLib_CDBMgr = (CDrMLib_CDBMgr*)pDBMgr;
	InitDrds();	
	return TRUE;
}

/*
LRESULT CApQurier::OnRealData(WPARAM wp, LPARAM lp)
{
	CDrMLib_CRealData RealData((void*)lp);

	if( RealData.wRecordID == PID_JONGSISE
		||RealData.wRecordID == PID_FUTURESISE
		||RealData.wRecordID == PID_OPTIONSISE
		||RealData.wRecordID == PID_QOPTIONSISE
		||RealData.wRecordID == PID_JOPTIONSISE	)
	{
		CAPDataItem* pAPDataIem = NULL;
		POSITION pos = m_listAPDataItem.GetHeadPosition();
		while(pos)
		{
			pAPDataIem = (CAPDataItem*)m_listAPDataItem.GetNext(pos);
			pAPDataIem->OnRealData(&RealData);
		}
	}		 
	return 0;
}
*/

//LONG CApQurier::OnDrdsData(UINT wParam, LPARAM lParam)
//{
//	LPCSTR pData = (LPSTR)lParam; //(LPCSTR)lParam;
//	int len = (int)wParam;
//	if(len > 3)   
//	{
//		DrdsData* pmyDData = (DrdsData*)pData;		
//		if(memcmp(pmyDData->Name, "S31", 3) == 0)			// 종목현재가
//		{
//			if( SZ_PriceData > pmyDData->Len )
//			{
//				AfxMessageBox("현물현재가 이상 Data Recv");
//				return 0L;			
//			}
//
//			CAPDataItem* pQuoteDataIem = NULL;
//			POSITION pos = m_listAPDataItem.GetHeadPosition();
//			while(pos)
//			{
//				pQuoteDataIem = (CAPDataItem*)m_listAPDataItem.GetNext(pos);
//				pQuoteDataIem->OnDrdsData(pmyDData->Len,(LPARAM)&pmyDData->Data[0]);
//			}
//
//			return 1L;
//		}
//		else if(memcmp(pmyDData->Name, "JS0", 3) == 0)			// 업종현재가
//		{
//			if( SZ_JS0_OUT > pmyDData->Len )
//			{
//				AfxMessageBox("업종현재가 이상 Data Recv");
//				return 0L;			
//			}
//			JS0_OUT* Op = (JS0_OUT*)pmyDData->Data;
//			char szbuffer[12];
//			MEMCPYWITHNULL(szbuffer,Op->bdate);
//			m_strDateOfRealTimeData = szbuffer;
//			return 1L;
//		}
////		else if(memcmp(pmyDData->Name, "SC2", 3) == 0)			// 선물현재가
////		{
////			if( SZ_SC2_OUT > pmyDData->Len )
////			{
////				AfxMessageBox("선물현재가 이상 Data Recv");
////				return 0L;			
////			}
//////			if(pmyDData->Key)
//////			m_pLeftBarDlg->SetRealPriceData(pmyDData->Data, pmyDData->Len);
////			m_ChartMng.SetRealFPriceData(pmyDData->Data, pmyDData->Len);
////			return 1L;
////		}
//	}
//
//	return 1L;
//}



void CApQurier::OnDestroy() 
{		
	CWnd::OnDestroy();	
	CString strName;
	CAPDataItem *pQuoteDataItem = NULL;
	POSITION pos = m_mapAPDataItemByInput.GetStartPosition();
	while(pos != NULL)
	{		
		m_mapAPDataItemByInput.GetNextAssoc(pos, strName, pQuoteDataItem);
		delete pQuoteDataItem;
	}
	m_mapAPDataItemByInput.RemoveAll();		
}

BOOL CApQurier::RequestDataToServer(CCommentData* pBasicalInputData, CSTController *pSTController,CAPDataItem **ppAPDataItem)
{
	CString strTemp("[Temp]");
	CString strBasicalInputData;

	if(pBasicalInputData)
	{
		// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
		strBasicalInputData = pBasicalInputData->GetComment(11
													,EXECUTE_SYMBOL
													,EXECUTE_ENDDATE
													,EXECUTE_ALLCOUNT
													,EXECUTE_TYPE
													,EXECUTE_TMNUMBER
													,EXECUTE_DATEPOINTER
													,EXECUTE_OPENPOINTER
													,EXECUTE_HIGHPOINTER
													,EXECUTE_LOWPOINTER
													,EXECUTE_CLOSEPOINTER
													,EXECUTE_VOLUMEPOINTER
													,EXECUTE_EXTDATA);
		if(strBasicalInputData.IsEmpty())
		{
			strBasicalInputData = strTemp;
		}
	}
	else
	{
		strBasicalInputData = strTemp;
	}
	
	// find another CAPDataItem which refer the same lpBasicalQuoteData
	BOOL bRetValue = m_mapAPDataItemByInput.Lookup(strBasicalInputData,*ppAPDataItem);

//시간은 세팅을 해야한다. 날짜 조회시 스크립트의 날짜 타입과 맞지 않는 오류 수정 !! KHD : 2006.12.12
	if(bRetValue)
	{
		CString TimeFormat = pBasicalInputData->GetComment(EXECUTE_TIMETYPE);
		(*ppAPDataItem)->SetTimeFormat(TimeFormat);
		if(TimeFormat=="HHMMSS")//전일 데이터가 필요할 시는 틱일경우 이고, 오늘 날짜데이터일경우 이므로 예외처리..:KHD 
			(*ppAPDataItem)->SetHelpMessageData(pBasicalInputData->GetComment(EXECUTE_PREVDATA));//DataSetting
		m_mapInputBySTC.SetAt(pSTController,strBasicalInputData);	
		(*ppAPDataItem)->AddReceiver(pSTController);//Controll	
		return FALSE;
	}
	CString strBasicanlInputDataInMapInputBySTC;
	// if pSTController already refers something, don't let pSTController refer it.
	bRetValue = m_mapInputBySTC.Lookup(pSTController,strBasicanlInputDataInMapInputBySTC);
	if(bRetValue)
	{
		m_mapAPDataItemByInput.Lookup(strBasicanlInputDataInMapInputBySTC,*ppAPDataItem);
		DeleteDataItem(*ppAPDataItem,pSTController);			
		*ppAPDataItem = NULL;
	}
	*ppAPDataItem = new CAPDataItem;
	//여기서 인터페이스로 가져와야 하나? : KHD 
	//여기서 차트인터페이스를 CAPDataItem에 넘겨야 합니다.
	(*ppAPDataItem)->SetChartItem(pSTController->GetChartItem());
	//
	bRetValue = TRUE;
	(*ppAPDataItem)->AddReceiver(pSTController);
	if(strTemp!=strBasicalInputData)// to avoid At loading cfg info.
	{
		bRetValue = (*ppAPDataItem)->RequestDataToServer(pBasicalInputData,pSTController,this);
	}		
	
	m_mapAPDataItemByInput.SetAt(strBasicalInputData,*ppAPDataItem);	
	m_mapInputBySTC.SetAt(pSTController,strBasicalInputData);

	if(bRetValue)
	{
		// That bRetValue is TRUE means pSTController needs AP quriey. 
		// To update AP Info, add into m_listAPDataItem.
		m_listAPDataItem.AddTail(*ppAPDataItem);
	}
	return bRetValue;
}

//  this doesn't let pSTController refer pQuoteDataItem. 
//	and if pQuoteDataItem is refered by anythig, delete pQuoteDataItem.
BOOL CApQurier::DeleteDataItem(CAPDataItem *pQuoteDataItem,CSTController *pSTController)
{
	CString strBasicalInputData;
	// find pSTController's strInputData;
	BOOL bRetValue = m_mapInputBySTC.Lookup(pSTController,strBasicalInputData);
	if(bRetValue) 
	{
		// remove pSTController's Pointer At pQuoteDataItem
		long lCntRemained = pQuoteDataItem->RemoveReceiver(pSTController);

		// if pQuoteDataItem did refered by anything, delete pQuoteDataItem
		if(!lCntRemained && lCntRemained>0)
		{
			// Remove pQuoteDataItem from m_listAPDataItem
			POSITION pos = m_listAPDataItem.Find(pQuoteDataItem);
			if(pos)
			{
				m_listAPDataItem.RemoveAt(pos);				
			}
			// because pQuoteDataItem is deleted, pQuoteDataItem's kes is removed.
			m_mapAPDataItemByInput.RemoveKey(strBasicalInputData);
			delete pQuoteDataItem;
			pQuoteDataItem = NULL;
		}
		// Remove pSTController From m_mapInputBySTC and m_mapAPDataItemByInput.
		m_mapInputBySTC.RemoveKey(pSTController);			
		

		return TRUE;
	}	
	return FALSE;
}