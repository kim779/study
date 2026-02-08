// STController.cpp: implementation of the CSTController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "STController.h"
#include <comdef.h>

#include "TempMemMng.h"
#include "../Common_ST/DataItem.h"
#include "LogicItem.h"
#include "ApQurier.h"
#include "MemManager.h"


#include "../Common_ST/STControllerDef.h"
#include "../Common_ST/STConfigDef.h"
#include "../Include_ST/CommentData.h"

#ifdef __MULTI_LANG__
	#include "../Include_Chart/Dll_Load/IMetaTable.h"	//@Solomon-MultiLang:091117
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
const UINT RMSG_STDATAMANAGER_RESULTDATA = ::RegisterWindowMessage("RMSG_STDATAMANAGER_RESULTDATA");


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CString Dtos(double dValue);
extern CString Parser(LPCTSTR lpInputData, int *pNShifter );

CSTController::CSTController(CSTControllerApp *pTheApp)
{
	m_pTheApp = pTheApp;
	m_pRootMemManager = NULL;
	m_pCurrentPosMemManager = NULL;
	m_lCurrentPos = 0;
	m_lCntOfAllApData = 0;
	m_hReceiver = NULL;
	m_lKey = NULL;
	m_lExpiredDate = 0;
	m_lSourceType = -1; 
	m_nSTPos = -1;
	m_bUseUserID = FALSE;
	m_bUsePassword = FALSE;
	m_bUseExpiredDate = FALSE;
	m_pStringArrayInputs = NULL;
	m_bIsUpdateData = FALSE;
	m_lCurrentBar = 0;
	m_bIsExternaInputData = FALSE;


	::ZeroMemory(m_pDataItemPlot,sizeof(m_pDataItemPlot));
	::ZeroMemory(m_pLogicItemInput,sizeof(m_pLogicItemInput));	
	m_pDataItemBullBearArea = NULL;
	m_hParent = NULL;
	m_pAPQurier = NULL;
	m_pAPDataItem = NULL;
	m_bOperate = FALSE;
	m_bProcess = FALSE;
	m_pChartItem = NULL;
}

CSTController::~CSTController()
{
	//Destory();
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	AP조회기를 생성한다.
///////////////////////////////////////////////////////////////////////////////

// 20060705 함수인자 제거 : , HWND hSocket, CDBMgr* pDBMgr
//BOOL CSTController::CreateApQurier(CWnd *pWnd, HWND hSocket, CDBMgr* pDBMgr)
BOOL CSTController::CreateApQurier(CWnd *pWnd)
{
	m_hParent = pWnd->GetSafeHwnd();

	// 20060705 함수인자 제거 : ,hSocket,(long)pDBMgr
	//m_pAPQurier = m_pTheApp->GetAPQurier(pWnd,hSocket,(long)pDBMgr);
	m_pAPQurier = m_pTheApp->GetAPQurier(pWnd);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	전략을 실행한다. 리턴값은 0,1,2,3인데.. 왜 long을 썻냐고..ㅡ.ㅡ;
///////////////////////////////////////////////////////////////////////////////
short CSTController::Execute(LPCTSTR lpInputData) 
{
	
	m_bProcess = TRUE;	// this controller is Operating now..
	if(!m_bOperate)
	{
		m_bOperate = TRUE;
		InitMemManager();
		// Having Option data, Use it instead of normal input data.
		CString strInputData = lpInputData;

		CString strContents;
		CString strTitleWanted;
		CCommentData Commentdata;
		int nCnt = 0;
		Commentdata.Init();
		Commentdata.SetCommentByString(lpInputData);
		strTitleWanted.Format("%s%d",INPUTVALUE,nCnt);

		while(1)
		{
			strContents = Commentdata.GetComment(1,strTitleWanted);
			if(strContents.IsEmpty()) break;
			if(nCnt==0)
			{
				m_StringArrayInputsFromExternal.RemoveAll();
			}
			m_StringArrayInputsFromExternal.Add(strContents);
			nCnt++;
			strTitleWanted.Format("%s%d",INPUTVALUE,nCnt);
			m_bIsExternaInputData = TRUE;// External Inputdata is used.
		}

		
		BOOL bRetValue = m_pAPQurier->RequestDataToServer(&Commentdata,this,&m_pAPDataItem);
		// Basical Items are registered in Logic Maker.
		RegisterItemIntoLogicMaker();
		if(!bRetValue)	//  bRetValue means whether it needs to Request AP or not..
		{
			bRetValue = OnReceiveQuoteData();
			if(!bRetValue) return 0;
			else return 2;
		}	
		return 1;
	}
	return 3;
	
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	전략의 입력값 분석을 위한 Item를 등록한다.
///////////////////////////////////////////////////////////////////////////////
BOOL CSTController::RegisterItemIntoLogicMaker()
{
	if(m_pChartItem!=NULL) return TRUE;
//	if(m_pChartItem!=NULL)
//	{
		//차트 아이템....을 직접 추가 한다.
//		ISTAnalysisInput* pMng ;
//		ISTInterface* pSTInterface = (ISTInterface*)m_pChartItem;
//		LONG dwDataTime, dwDataOpen, dwDataHigh, dwDataLow, dwDataClose,dwDataVolume;
//		if(pSTInterface)
//		{
	/*		pMng = (ISTAnalysisInput*)pSTInterface->GetInterface(UUSTID_ISTAnalysisInput);
			int nCnt = pMng->GetChartData(dwDataTime, dwDataOpen, dwDataHigh, dwDataLow, dwDataClose,dwDataVolume);
			TRACE("nCnt=%d, Time=%ld, Open=%ld, High=%ld, Low=%ld, Close=%ld, Volume=%ld\n",
				nCnt, dwDataTime, dwDataOpen, dwDataHigh, dwDataLow, dwDataClose, dwDataVolume);

			CSDataItem* pSdataItemDate;
			CSTDataItem* pDataItemOpen;
			CSTDataItem* pDataItemHigh;
			CSTDataItem* pDataItemLow;
			CSTDataItem* pDataItemClose;
			CSTDataItem* pDataItemVolume;

			pSdataItemDate = (CSDataItem*)dwDataTime;
			pDataItemOpen = (CSTDataItem*)dwDataOpen;
			pDataItemHigh = (CSTDataItem*)dwDataHigh;
			pDataItemLow= (CSTDataItem*)dwDataLow;
			pDataItemClose = (CSTDataItem*)dwDataClose;
			pDataItemVolume = (CSTDataItem*)dwDataVolume;

			m_logicMaker.AddDataItemToUse(pDataItemOpen->GetDataItemName(),pDataItemOpen);
			m_logicMaker.AddDataItemToUse("시가",pDataItemOpen);
			m_logicMaker.AddDataItemToUse("O",pDataItemOpen);

			m_logicMaker.AddDataItemToUse(pDataItemHigh->GetDataItemName(),pDataItemHigh);
			m_logicMaker.AddDataItemToUse("고가",pDataItemHigh);
			m_logicMaker.AddDataItemToUse("H",pDataItemHigh);

			m_logicMaker.AddDataItemToUse(pDataItemLow->GetDataItemName(),pDataItemLow);
			m_logicMaker.AddDataItemToUse("저가",pDataItemLow);
			m_logicMaker.AddDataItemToUse("L",pDataItemLow);

			m_logicMaker.AddDataItemToUse(pDataItemClose->GetDataItemName(),pDataItemClose);	
			m_logicMaker.AddDataItemToUse("종가",pDataItemClose);
			m_logicMaker.AddDataItemToUse("C",pDataItemClose);

			m_logicMaker.AddDataItemToUse(pDataItemVolume->GetDataItemName(),pDataItemVolume);	
			m_logicMaker.AddDataItemToUse("기본거래량",pDataItemVolume);
			m_logicMaker.AddDataItemToUse("V",pDataItemVolume);*/
			/*m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetOpenData()->GetDataItemName(),m_pAPDataItem->GetOpenData());
			m_logicMaker.AddDataItemToUse("시가",m_pAPDataItem->GetOpenData());
			m_logicMaker.AddDataItemToUse("O",m_pAPDataItem->GetOpenData());
			m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetHighData()->GetDataItemName(),m_pAPDataItem->GetHighData());
			m_logicMaker.AddDataItemToUse("고가",m_pAPDataItem->GetHighData());
			m_logicMaker.AddDataItemToUse("H",m_pAPDataItem->GetHighData());
			m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetLowData()->GetDataItemName(),m_pAPDataItem->GetLowData());
			m_logicMaker.AddDataItemToUse("저가",m_pAPDataItem->GetLowData());
			m_logicMaker.AddDataItemToUse("L",m_pAPDataItem->GetLowData());
			m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetCloseData()->GetDataItemName(),m_pAPDataItem->GetCloseData());	
			m_logicMaker.AddDataItemToUse("종가",m_pAPDataItem->GetCloseData());
			m_logicMaker.AddDataItemToUse("C",m_pAPDataItem->GetCloseData());
			m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetVolumeData()->GetDataItemName(),m_pAPDataItem->GetVolumeData());	
			m_logicMaker.AddDataItemToUse("기본거래량",m_pAPDataItem->GetVolumeData());
			m_logicMaker.AddDataItemToUse("V",m_pAPDataItem->GetVolumeData());*/
			//m_ptrArrayHistoryDataItem.RemoveAll();
			//m_ptrArrayHistoryDataItem.Add(m_pAPDataItem->GetOpenData());
			//m_ptrArrayHistoryDataItem.Add(m_pAPDataItem->GetHighData());
			//m_ptrArrayHistoryDataItem.Add(m_pAPDataItem->GetLowData());
			//m_ptrArrayHistoryDataItem.Add(m_pAPDataItem->GetCloseData());
			//m_ptrArrayHistoryDataItem.Add(m_pAPDataItem->GetVolumeData());
			//m_lCntOfAllApData = m_pAPDataItem->GetCloseData()->GetSize();
			////if(m_lCntOfAllApData<=0) return FALSE;
			//m_ptrArrayHistoryDataItem.RemoveAll();
			//m_ptrArrayHistoryDataItem.Add(pDataItemOpen);
			//m_ptrArrayHistoryDataItem.Add(pDataItemHigh);
			//m_ptrArrayHistoryDataItem.Add(pDataItemLow);
			//m_ptrArrayHistoryDataItem.Add(pDataItemClose);
			//m_ptrArrayHistoryDataItem.Add(pDataItemVolume);
			//m_bIsUpdateData = FALSE;
			//::SendMessage(m_hParent,UMSG_ENDRECEIVINGFROMSERVER,m_pAPDataItem->GetCntData(),0);
	//		if(GetError())	return FALSE;

	//	}
	//}
//	else
	//{

#ifdef __MULTI_LANG__
//@Solomon-MultiLang:091117 --> 한글명을 수정함.
	m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetOpenData()->GetDataItemName(),m_pAPDataItem->GetOpenData());
	m_logicMaker.AddDataItemToUse(_MTEXT( C0_OPEN),m_pAPDataItem->GetOpenData());
	m_logicMaker.AddDataItemToUse("O",m_pAPDataItem->GetOpenData());
	m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetHighData()->GetDataItemName(),m_pAPDataItem->GetHighData());
	m_logicMaker.AddDataItemToUse(_MTEXT( C0_HIGH),m_pAPDataItem->GetHighData());
	m_logicMaker.AddDataItemToUse("H",m_pAPDataItem->GetHighData());
	m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetLowData()->GetDataItemName(),m_pAPDataItem->GetLowData());
	m_logicMaker.AddDataItemToUse(_MTEXT( C0_LOW),m_pAPDataItem->GetLowData());
	m_logicMaker.AddDataItemToUse("L",m_pAPDataItem->GetLowData());
	m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetCloseData()->GetDataItemName(),m_pAPDataItem->GetCloseData());	
	m_logicMaker.AddDataItemToUse(_MTEXT( C0_CLOSE),m_pAPDataItem->GetCloseData());
	m_logicMaker.AddDataItemToUse("C",m_pAPDataItem->GetCloseData());
	m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetVolumeData()->GetDataItemName(),m_pAPDataItem->GetVolumeData());	
	m_logicMaker.AddDataItemToUse(_MTEXT( C0_VOLUME),m_pAPDataItem->GetVolumeData());
	m_logicMaker.AddDataItemToUse("V",m_pAPDataItem->GetVolumeData());
//@Solomon-MultiLang:091117 <--
#else
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetOpenData()->GetDataItemName(),m_pAPDataItem->GetOpenData());
		m_logicMaker.AddDataItemToUse("시가",m_pAPDataItem->GetOpenData());
		m_logicMaker.AddDataItemToUse("O",m_pAPDataItem->GetOpenData());
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetHighData()->GetDataItemName(),m_pAPDataItem->GetHighData());
		m_logicMaker.AddDataItemToUse("고가",m_pAPDataItem->GetHighData());
		m_logicMaker.AddDataItemToUse("H",m_pAPDataItem->GetHighData());
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetLowData()->GetDataItemName(),m_pAPDataItem->GetLowData());
		m_logicMaker.AddDataItemToUse("저가",m_pAPDataItem->GetLowData());
		m_logicMaker.AddDataItemToUse("L",m_pAPDataItem->GetLowData());
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetCloseData()->GetDataItemName(),m_pAPDataItem->GetCloseData());	
		m_logicMaker.AddDataItemToUse("종가",m_pAPDataItem->GetCloseData());
		m_logicMaker.AddDataItemToUse("C",m_pAPDataItem->GetCloseData());
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetVolumeData()->GetDataItemName(),m_pAPDataItem->GetVolumeData());	
		m_logicMaker.AddDataItemToUse("기본거래량",m_pAPDataItem->GetVolumeData());
		m_logicMaker.AddDataItemToUse("V",m_pAPDataItem->GetVolumeData());
#endif
		
		// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
		//<<20100427_JS.Kim 비정상종료 수정
		//		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetExtData()->GetDataItemName(), m_pAPDataItem->GetExtData());
		if( m_pAPDataItem && m_pAPDataItem->GetExtData() )
				m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetExtData()->GetDataItemName(), m_pAPDataItem->GetExtData());

		//>>

	//}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	전략을 중지한다.
///////////////////////////////////////////////////////////////////////////////
long CSTController::Stop()
{
	m_bOperate = FALSE;
	m_bProcess = FALSE;
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	전략을 잠깐 멈춘다.
///////////////////////////////////////////////////////////////////////////////
long CSTController::Pause()
{
	m_bProcess = FALSE;// this controller is Operating but SystemTrading Logic is stopped.
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	전략계산 데이터를 Update한다.
///////////////////////////////////////////////////////////////////////////////
long CSTController::Update(long lPos, BOOL bIsUpdateData)
{
	// (2006/2/22 - Seung-Won, Bae) Do Initial Update (TR) Process for Output Binding.
	if( m_lCntOfAllApData <= 0)
	{
		OnReceiveQuoteData();
		return 1;
	}

	if(!bIsUpdateData)	SetSizeDataItem(lPos+1);
	BOOL bRetValue = OnReceiveQuoteDataInRealTime(lPos, bIsUpdateData);
	if(!bRetValue)	return FALSE;
	return 1;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	전략을 종료한다.
///////////////////////////////////////////////////////////////////////////////
BOOL CSTController::Destory()
{
	if(m_pAPQurier) m_pAPQurier->DeleteDataItem(m_pAPDataItem,this);
	m_pAPDataItem = NULL;
	m_pTheApp->DeleteAPQurier(m_pAPQurier);
	m_pAPQurier = NULL;
	ClearMemManager();		
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	전략계산 결과를 받을 Handle을 세팅한다.
///////////////////////////////////////////////////////////////////////////////
void CSTController::SetReceiver(HWND hReceiver, long lKey) 
{
	// TODO: Add your dispatch handler code here
	m_hReceiver = (HWND)hReceiver;
	m_lKey = lKey;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	내부 메모리를 초기화한다.
///////////////////////////////////////////////////////////////////////////////
void CSTController::InitMemManager()
{
	ClearMemManager();
	m_pRootMemManager = new CMemManager;
	m_pRootMemManager->SetSubFuncPath(m_strSubFuncPath);
	m_pCurrentPosMemManager = m_pRootMemManager;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	내부메모리를 삭제한다.
///////////////////////////////////////////////////////////////////////////////
void CSTController::ClearMemManager()
{
// 	{
// 		CString szMsg;
// 		szMsg.Format("%s[%d]", __FILE__, __LINE__);
// 		OutputDebugString(szMsg);
// 	}

	if(m_pRootMemManager)
	{
		delete m_pRootMemManager;
		m_pRootMemManager = NULL;
		m_pCurrentPosMemManager = NULL;
		::ZeroMemory(m_pDataItemPlot,sizeof(m_pDataItemPlot));
		m_dealDataItem.Clear();
		m_pDataItemBullBearArea = NULL;
		for(int nPos=0;nPos<MAXCNTOFINPUTHOLDER;nPos++)
		{
			if(m_pLogicItemInput[nPos])
			{
				delete m_pLogicItemInput[nPos];
				m_pLogicItemInput[nPos] = NULL;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	전략의 시간기준 Index를 세팅한다.
///////////////////////////////////////////////////////////////////////////////
BOOL CSTController::SetCurrentPos(long lPos) 
{
	// TODO: Add your dispatch handler code here
	if(m_pRootMemManager!=m_pCurrentPosMemManager)
	{
		m_pCurrentPosMemManager = m_pRootMemManager;
		TRACE("[%s] %s\n", __FILE__, "CSTController::SetCurrentPos(long lPos)");

//		AfxMessageBox("SetCurrentPos Error!!");
//		return FALSE;
	}
	if(lPos&&m_pRootMemManager->IsFirst())
	{
		m_pRootMemManager->SetCheckNotFirst();
	}
	m_pRootMemManager->Next();
	m_lCurrentPos = lPos;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	Sub함수로 들어간다.
///////////////////////////////////////////////////////////////////////////////
void CSTController::SubIn(long lIndex) 
{
	m_pCurrentPosMemManager = m_pCurrentPosMemManager->SubIn(m_lCurrentPos, FALSE);	
	if(m_pCurrentPosMemManager->IsFirst())
	{
		m_pCurrentPosMemManager->InitFuncData(m_lCntOfAllApData);
		m_pCurrentPosMemManager->SetOffset(lIndex);
	}
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	Sub함수에서 나온다.
///////////////////////////////////////////////////////////////////////////////
void CSTController::SubOut() 
{
	m_pCurrentPosMemManager = m_pCurrentPosMemManager->SubOut(m_lCurrentPos);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	If 함수로 들어간다.
///////////////////////////////////////////////////////////////////////////////
void CSTController::IfIn() 
{
	m_pCurrentPosMemManager = m_pCurrentPosMemManager->SubIn(m_lCurrentPos, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	If 함수에서 나온다.
///////////////////////////////////////////////////////////////////////////////
void CSTController::IfOut() 
{
	m_pCurrentPosMemManager = m_pCurrentPosMemManager->SubOut(m_lCurrentPos);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	Bracket 으로 들어간다.
///////////////////////////////////////////////////////////////////////////////
void CSTController::BracketIn() 
{
	m_pCurrentPosMemManager = m_pCurrentPosMemManager->SubIn(m_lCurrentPos, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	Bracket 에서 나온다.
///////////////////////////////////////////////////////////////////////////////
void CSTController::BracketOut() 
{
	m_pCurrentPosMemManager = m_pCurrentPosMemManager->SubOut(m_lCurrentPos);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	For구문으로 들어간다.
///////////////////////////////////////////////////////////////////////////////
void CSTController::ForIn() 
{
	m_pCurrentPosMemManager = m_pCurrentPosMemManager->SubIn(m_lCurrentPos, TRUE);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	For구문에서 나온다.
///////////////////////////////////////////////////////////////////////////////
void CSTController::ForOut() 
{
	m_pCurrentPosMemManager = m_pCurrentPosMemManager->SubOut(m_lCurrentPos);
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	시간기준 Index를 받는다.
///////////////////////////////////////////////////////////////////////////////
long CSTController::BarIndex()
{
	return m_lCurrentPos;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	저장 정보를 가져온다.
///////////////////////////////////////////////////////////////////////////////
DWORD CSTController::GetPrivateProfileStringInST(LPCTSTR lpAppName,LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize)
{
	DWORD dwSizeRead = 0;
	if(m_strSTName.GetLength()&&m_nSTPos>=0)
	{
		CString strAppNameSTName;
		strAppNameSTName.Format("%s%d",lpAppName,m_nSTPos);
		dwSizeRead = ::GetPrivateProfileString(strAppNameSTName,lpKeyName,lpDefault,lpReturnedString,nSize,m_strSTName);
		if(dwSizeRead)
		{
			return dwSizeRead;
		}
	}
	dwSizeRead = ::GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpReturnedString,nSize,m_strSfgFile);
	if(!dwSizeRead)
	{
		lpReturnedString = (LPTSTR)lpDefault;
	}
	return dwSizeRead;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	Input정보를 Setting한다.
//	lIndex		인덱스
//	lpName		input name
//	nType		input type
//	dInitValue	초기값
///////////////////////////////////////////////////////////////////////////////
BOOL CSTController::SetInputs(long lIndex, LPCTSTR lpName, int nType, double dInitValue) 
{
	// TODO: Add your dispatch handler code here	
	if(m_pCurrentPosMemManager->IsFirst())
	{
		if(m_pRootMemManager==m_pCurrentPosMemManager)
		{	
			if(m_bIsExternaInputData)
			{
				int nSize = m_StringArrayInputsFromExternal.GetSize();
				if(lIndex>=nSize) 
				{
					m_strError.Format("%d번째 입력변수가 존재하지 않습니다.",lIndex+1);
					ASSERT( FALSE);
				}
				CString strInputExternal = m_StringArrayInputsFromExternal.GetAt(lIndex);
				m_pLogicItemInput[lIndex] = m_logicMaker.EvaluatesByExp(strInputExternal);
				LPCTSTR lpError = m_logicMaker.GetErrorMessage();
				if(lpError)	
				{
					m_strError = lpError;
					ASSERT( FALSE);
				}
				m_pStringArrayInputs->SetAt(lIndex,strInputExternal);
			}
			else
			{
				DWORD dwSizeRead = 0;
				char szBuffer[256];
				CString strKeyName;
				strKeyName.Format("%s%d",INPUTVALUE,lIndex);
				dwSizeRead = GetPrivateProfileStringInST(VARIABLES,strKeyName,"",szBuffer,sizeof(szBuffer));
				if(dwSizeRead)
				{					
					m_pLogicItemInput[lIndex] = m_logicMaker.EvaluatesByExp(szBuffer);
					LPCTSTR lpError = m_logicMaker.GetErrorMessage();
					if(lpError)	
					{
						m_strError = lpError;
						ASSERT(FALSE);
					}
					m_pStringArrayInputs->SetAt(lIndex,szBuffer);
				}
				else
				{	
					int ddd = m_pStringArrayInputs->GetSize();
					m_pLogicItemInput[lIndex] = m_logicMaker.EvaluatesByExp(m_pStringArrayInputs->GetAt(lIndex));
					LPCTSTR lpError = m_logicMaker.GetErrorMessage();
					if(lpError)	
					{
						m_strError = lpError;
						ASSERT(FALSE);
					}
				}
			}
		}
		m_pCurrentPosMemManager->SetInputs(m_lCurrentPos, lIndex, lpName, nType, dInitValue,m_lCntOfAllApData);
		m_pCurrentPosMemManager->SetInputHolder(lIndex,lpName);
	}

	if(m_pRootMemManager==m_pCurrentPosMemManager)
	{
		long lPos = m_lCurrentPos -  m_pCurrentPosMemManager->GetOffset();
		if(lPos>=0)
		{
			m_pLogicItemInput[lIndex]->SetIndex(lPos);
			dInitValue = m_pLogicItemInput[lIndex]->GetValue();				
		}
		else
		{
			dInitValue = 0;
		}
	}
	m_pCurrentPosMemManager->SetInputHolderData(lIndex,m_lCurrentPos,dInitValue);		

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	오근문(Geunmoon Oh)
//	E-Mail	:	gm0604@dooriic.co.kr
//	일자	:	2005.11.21
//	목적	:	Variables정보를 Setting한다.
//	lIndex		인덱스
//	lpName		input name
//	nType		input type
//	dInitValue	초기값
///////////////////////////////////////////////////////////////////////////////
BOOL CSTController::SetVariables(long lHolderIndex, LPCTSTR lpName, int nType, double dInitValue) 
{
	// TODO: Add your dispatch handler code here	
	if(m_pCurrentPosMemManager->IsFirst())
	{
		m_pCurrentPosMemManager->SetVariables(m_lCurrentPos,lpName,nType,dInitValue,m_lCntOfAllApData);
		return m_pCurrentPosMemManager->SetVarHolder(lHolderIndex,lpName);
	}
	else
	{
		double dValueLast = m_pCurrentPosMemManager->GetVarHolderData(lHolderIndex,m_lCurrentPos-1);
		if(dValueLast==HUGE_VAL||dValueLast==-HUGE_VAL)	dValueLast = dInitValue;
		m_pCurrentPosMemManager->SetVarHolderData(lHolderIndex,m_lCurrentPos,dValueLast);
	}
	return TRUE;
}


BOOL CSTController::SetSubInputsFrom(long lIndex, double dValue) 
{
	return m_pCurrentPosMemManager->SetInputs(m_lCurrentPos,lIndex,"[SubInputs]",0,dValue,m_lCntOfAllApData);
}

BOOL CSTController::IfCondition(BOOL bIfConditon) 
{
	return m_pCurrentPosMemManager->Condition(bIfConditon);
}

BOOL CSTController::PrepareGraph()
{
	m_ptrArraySDataItem.RemoveAll();
	m_ptrArraySDataItem.Add(m_pAPDataItem->GetDateData());

	DWORD dwSizeRead = 0;
	char szBuffer[256];
	CString strComment;
	CString strKeyName;
	long lType = 0;
	long lShape = 0;
	long lThick = 0;
	COLORREF clrIndex = RGB(0,0,0);

	// set configuration about plots
	m_ptrArrayPlot.RemoveAll();	

	for(int nPos=0;nPos<MAXCNTOFPLOTHOLDER;nPos++)
	{
		if(m_pDataItemPlot[nPos])///KHD : Plot를 통해 초기화된다.
		{
			m_ptrArrayPlot.Add(m_pDataItemPlot[nPos]);

			strKeyName.Format("%s%d",INDEXTYPE,nPos);
			dwSizeRead = GetPrivateProfileStringInST(INDEX,strKeyName,"",szBuffer,sizeof(szBuffer));
			if(!dwSizeRead)
			{
				lType = INDEXTYPEDEFAULT;	
			}
			else
			{
				lType = atol(szBuffer);
			}	
			strComment.Format("%ld",lType);
			m_pDataItemPlot[nPos]->SetComment(INDEXTYPE,strComment);

			strComment = m_pDataItemPlot[nPos]->GetComment(INDEXCOLOR);
			if(strComment=="")
			{
				strKeyName.Format("%s%d",INDEXCOLOR,nPos);
				dwSizeRead = GetPrivateProfileStringInST(INDEX,strKeyName,"",szBuffer,sizeof(szBuffer));
				if(!dwSizeRead)
				{
					clrIndex = INDEXCOLORDEFAULT;	
				}
				else
				{
					clrIndex = atol(szBuffer);
				}		
				strComment.Format("%ld",clrIndex);
				m_pDataItemPlot[nPos]->SetComment(INDEXCOLOR,strComment);
			}

			strKeyName.Format("%s%d",INDEXSHAPE,nPos);
			dwSizeRead = GetPrivateProfileStringInST(INDEX,strKeyName,"",szBuffer,sizeof(szBuffer));
			if(!dwSizeRead)
			{
				lShape = INDEXSHAPEDEFAULT;	
			}
			else
			{
				lShape = atol(szBuffer);
			}		
			strComment.Format("%ld",lShape);
			m_pDataItemPlot[nPos]->SetComment(INDEXSHAPE,strComment);

			
			strKeyName.Format("%s%d",INDEXTHICK,nPos);
			dwSizeRead = GetPrivateProfileStringInST(INDEX,strKeyName,"",szBuffer,sizeof(szBuffer));
			if(!dwSizeRead)
			{
				lThick = INDEXTHICKDEFAULT;	
			}
			else
			{
				lThick = atol(szBuffer);
			}		
			strComment.Format("%ld",lThick);
			m_pDataItemPlot[nPos]->SetComment(INDEXTHICK,strComment);

			
		}
	}
	// set configuration about buy, sell, exitlong, and exitshort.
	if(m_dealDataItem.GetCount())
	{
		//////////////////////////////////////////////////////////////////////////////////////////////
		long lShapeSignal;
		COLORREF clrSignal;

		dwSizeRead = GetPrivateProfileStringInST(SIGNAL,SHAPEBUY,"",szBuffer,sizeof(szBuffer));
		if(!dwSizeRead)
		{
			lShapeSignal = 0;
		}
		else
		{
			lShapeSignal = atol(szBuffer);
		}
		strComment.Format("%ld",lShapeSignal);
		m_dealDataItem.SetComment(SHAPEBUY,strComment);


		dwSizeRead = GetPrivateProfileStringInST(SIGNAL,COLORBUY,"",szBuffer,sizeof(szBuffer));
		if(!dwSizeRead)
		{
			clrSignal = SIGNALBUYDEFAULT;
		}
		else
		{
			clrSignal = atol(szBuffer);
		}
		strComment.Format("%ld",clrSignal);
		m_dealDataItem.SetComment(COLORBUY,strComment);

		dwSizeRead = GetPrivateProfileStringInST(SIGNAL,SHAPESELL,"",szBuffer,sizeof(szBuffer));
		if(!dwSizeRead)
		{
			lShapeSignal = 0;
		}
		else
		{
			lShapeSignal = atol(szBuffer);
		}
		strComment.Format("%ld",lShapeSignal);
		m_dealDataItem.SetComment(SHAPESELL, strComment);

		dwSizeRead = GetPrivateProfileStringInST(SIGNAL,COLORSELL,"",szBuffer,sizeof(szBuffer));
		if(!dwSizeRead)
		{
			clrSignal = SIGNALSELLDEFAULT;
		}
		else
		{
			clrSignal = atol(szBuffer);
		}
		strComment.Format("%ld",clrSignal);
		m_dealDataItem.SetComment(COLORSELL,strComment);



		dwSizeRead = GetPrivateProfileStringInST(SIGNAL,SHAPEEXITSHORT,"",szBuffer,sizeof(szBuffer));
		if(!dwSizeRead)
		{
			lShapeSignal = 0;
		}
		else
		{
			lShapeSignal = atol(szBuffer);
		}
		strComment.Format("%ld",lShapeSignal);
		m_dealDataItem.SetComment(SHAPEEXITSHORT, strComment);


		dwSizeRead = GetPrivateProfileStringInST(SIGNAL,COLOREXITSHORT,"",szBuffer,sizeof(szBuffer));
		if(!dwSizeRead)
		{
			clrSignal = SIGNALEXITSHORTDEFAULT;
		}
		else
		{
			clrSignal = atol(szBuffer);
		}
		strComment.Format("%ld",clrSignal);
		m_dealDataItem.SetComment(COLOREXITSHORT, strComment);


		dwSizeRead = GetPrivateProfileStringInST(SIGNAL,SHAPEEXITLONG,"",szBuffer,sizeof(szBuffer));
		if(!dwSizeRead)
		{
			lShapeSignal = 0;
		}
		else
		{
			lShapeSignal = atol(szBuffer);
		}
		strComment.Format("%ld",lShapeSignal);
		m_dealDataItem.SetComment(SHAPEEXITLONG, strComment);


		dwSizeRead = GetPrivateProfileStringInST(SIGNAL,COLOREXITLONG,"",szBuffer,sizeof(szBuffer));
		if(!dwSizeRead)
		{
			clrSignal = SIGNALEXITLONGDEFAULT;
		}
		else
		{
			clrSignal = atol(szBuffer);
		}
		strComment.Format("%ld",clrSignal);
		m_dealDataItem.SetComment(COLOREXITLONG,strComment);


		BOOL bShow = FALSE;
		dwSizeRead = GetPrivateProfileStringInST(SIGNAL,SHOWSIGNALNAME,"0",szBuffer,sizeof(szBuffer));
		bShow = atol(szBuffer);
		strComment.Format("%d",bShow);
		m_dealDataItem.SetComment(SHOWSIGNALNAME, strComment);

		dwSizeRead = GetPrivateProfileStringInST(SIGNAL,SHOWSIGNALQTY,"0",szBuffer,sizeof(szBuffer));
		bShow = atol(szBuffer);
		strComment.Format("%d,",bShow);
		m_dealDataItem.SetComment(SHOWSIGNALQTY,strComment);
		m_dealDataItem.EnableContinuty(TRUE,10000);
		//////////////////////////////////////////////////////////////////////////////////////////////
	}	
	// set configuration about bull, and bear area.
	if(m_pDataItemBullBearArea)
	{
		m_ptrArrayPlot.Add(m_pDataItemBullBearArea);
		//////////////////////////////////////////////////////////////////////////////////////////////
		DWORD dwSizeRead = 0;
		char szBuffer[256];
		CString strComment;
		COLORREF clrSignal;

		dwSizeRead = GetPrivateProfileStringInST(INDEX,COLORBULL,"",szBuffer,sizeof(szBuffer));
		if(!dwSizeRead)
		{
			clrSignal = INDEXBULLDEFAULT;
		}
		else
		{
			clrSignal = atol(szBuffer);
		}
		strComment.Format("%ld",clrSignal);
		m_pDataItemBullBearArea->SetComment(COLORBULL,strComment);
		dwSizeRead = GetPrivateProfileStringInST(INDEX,COLORBEAR,"",szBuffer,sizeof(szBuffer));
		if(!dwSizeRead)
		{
			clrSignal = INDEXBEARDEFAULT;
		}
		else
		{
			clrSignal = atol(szBuffer);
		}
		strComment.Format("%ld,",clrSignal);
		m_pDataItemBullBearArea->SetComment(COLORBEAR,strComment);
		//////////////////////////////////////////////////////////////////////////////////////////////
	}

	return TRUE;
}

BOOL CSTController::DrawGraph(BOOL bIsInRealTime) 
{
	
	// TODO: Add your dispatch handler code here
	if(m_hReceiver && IsWindow(m_hReceiver)) // Having Reveiver, send draw info.
	{		
		m_drawInfo.bIsInRealTime = bIsInRealTime;
		m_drawInfo.pPtrArraySDataItem = &m_ptrArraySDataItem;
		m_drawInfo.pPtrArrayHistoryDataItem = &m_ptrArrayHistoryDataItem;
		m_drawInfo.pPtrArrayPlot = &m_ptrArrayPlot;
		m_drawInfo.pDealDataItem = (void*)&m_dealDataItem;
		
		::SendMessage(m_hReceiver,RMSG_STDATAMANAGER_RESULTDATA,m_lKey,(LPARAM)&m_drawInfo);
	}			
	return TRUE;
}

CIDataItem* CSTController::GetRawData(long lDataIndex)
{
	switch(lDataIndex)
	{
	case RAWOPENDATA:
		return m_pAPDataItem->GetOpenData();
		break;
	case RAWHIGHDATA:
		return m_pAPDataItem->GetHighData();
		break;
	case RAWLOWDATA:
		return m_pAPDataItem->GetLowData();
		break;
	case RAWCLOSEDATA:
		return m_pAPDataItem->GetCloseData();
		break;
	case RAWVOLUMEDATA:
		return m_pAPDataItem->GetVolumeData();
		break;
	}
	return NULL;
}

CISDataItem* CSTController::GetRawSData(long lDataIndex)
{
	switch(lDataIndex)
	{
	case RAWDATEDATA:
		return m_pAPDataItem->GetDateData();
		break;
	}
	return NULL;
}

CString CSTController::GetRawSData(long lDataIndex, double dReverseIndex)
{
	long lIndex = m_lCurrentPos - (long)dReverseIndex - m_pCurrentPosMemManager->GetOffset();
	if(lIndex<0)
	{
		return "";
	}
	switch(lDataIndex)
	{
	case RAWDATEDATA:
		return m_pAPDataItem->GetDate(lIndex);		
		break;
	case RAWDATETYPE:
		return m_pAPDataItem->GetTimeFormat();
		break;
	}
	return "";
}

double CSTController::GetRawData(long lDataIndex, double dReverseIndex)
{
	long lIndex = m_lCurrentPos - (long)dReverseIndex - m_pCurrentPosMemManager->GetOffset();
	long lIndexForDay = m_lCurrentPos  - m_pCurrentPosMemManager->GetOffset();
	if(lIndex<0)
	{
		return 0.0;
	}
	switch(lDataIndex)
	{
	case RAWOPENDATA:
		return m_pAPDataItem->GetOpenData(lIndex);		
		break;
	case RAWHIGHDATA:
		return m_pAPDataItem->GetHighData(lIndex);
		break;
	case RAWLOWDATA:
		return m_pAPDataItem->GetLowData(lIndex);
		break;
	case RAWCLOSEDATA:
		return m_pAPDataItem->GetCloseData(lIndex);
		break;
	case RAWVOLUMEDATA:
		return m_pAPDataItem->GetVolumeData(lIndex);
		break;
	case RAWAMOUNTDATA:
		return m_pAPDataItem->GetAmountData(lIndex);
		break;
	case RAWINDEX:
		return m_lCurrentPos;
		break;
	case RAWCURRENTBAR:
		{
			m_lCurrentBar = (m_lCurrentPos - m_pCurrentPosMemManager->GetFuncHolderPointer()->GetFirstDataIndex() - 1);
			return m_lCurrentBar;
		}		
		break;
	case RAWBARINTERVAL:
		{
			return m_pAPDataItem->GetVarInterval();
		}
		break;
	case RAWCODECATEGORY:
		{
			return 1;
//KOSPI 장내→1
//KOSDAQ→2
//제3시장→3
//선물→4
//지수→5
//옵션→6
//종목옵션→7
		}
		break;
	case RAWCURRENTDATE:
		break;
	case RAWCURRENTTIME:
		break;
	case RAWDATACOMPRESS:
		{
			// 틱'0'/분'1'/일'2'/주'3'/월'4'
			char chTimeType = m_pAPDataItem->GetTimeType();
			if(chTimeType=='0') return 0;
			else if(chTimeType=='1') return 2;
			else if(chTimeType=='2') return 3;
			else if(chTimeType=='3') return 4;
			else if(chTimeType=='4') return 5;
			//0 → 틱봉
			//1 → 초봉
			//2 → 분봉
			//3 → 일봉
			//4 → 주봉
			//5 → 월봉
		}		
		break;
	case RAWPREVOPENDATA:
		return m_pAPDataItem->GetPrevOpenData();
		break;
	case RAWPREVHIGHDATA:
		return m_pAPDataItem->GetPrevHighData();
		break;
	case RAWPREVLOWDATA:
		return m_pAPDataItem->GetPrevLowData();
		break;
	case RAWPREVCLOSEDATA:
		return m_pAPDataItem->GetPrevCloseData();
		break;
	}

	return 0.0;
}

double CSTController::GetVarHolderData(long lHolderIndex, double dIndex)
{
	double dValue = 0;
	long lIndex = m_lCurrentPos - (long)dIndex;
	if(lIndex<0)
	{
		return 0.0;
	}
	BOOL bRetValue = m_pCurrentPosMemManager->GetVarHolderData(lHolderIndex,lIndex, &dValue);
	if(bRetValue)
	{
		return dValue;
	}
	return 0.0;
}

double CSTController::GetInputHolderData(long lHolderIndex, double dIndex)
{
	double dValue = 0;
	long lIndex = m_lCurrentPos - (long)dIndex;
	if(lIndex<0)
	{
		return HUGE_VAL;
	}
	BOOL bRetValue = m_pCurrentPosMemManager->GetInputHolderData(lHolderIndex,lIndex, &dValue);
	if(bRetValue)
	{
		return dValue;
	}
	return 0.0;
}

double CSTController::GetFuncData(double dIndex)
{
	double dValue = 0;
	long lIndex = m_lCurrentPos - (long)dIndex;
	if(lIndex<0)
	{
		return 0.0;
	}
	BOOL bRetValue = m_pCurrentPosMemManager->GetFuncData(lIndex, &dValue);
	if(bRetValue)
	{
		return dValue;
	}
	return 0.0;
}

void CSTController::SetVarHolderData(long lHolderIndex,double dIndex,  double dValue)
{
	m_pCurrentPosMemManager->SetVarHolderData(lHolderIndex,m_lCurrentPos, dValue);
}

void CSTController::SetInputHolderData(long lHolderIndex, double dIndex, double dValue)
{
	m_pCurrentPosMemManager->SetInputHolderData(lHolderIndex,m_lCurrentPos, dValue);
}

void CSTController::SetFuncData(double dIndex, double dValue)
{
	m_pCurrentPosMemManager->SetFuncData(m_lCurrentPos, dValue);
}

//KHD : 여기서 데이터를 세팅하자궁....
BOOL CSTController::OnReceiveQuoteData()
{
	if(m_pChartItem)
	{
#ifdef __MULTI_LANG__
//@Solomon-MultiLang:091117 --> 한글명을 수정함.
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetOpenData()->GetDataItemName(),m_pAPDataItem->GetOpenData());
		m_logicMaker.AddDataItemToUse(_MTEXT( C0_OPEN),m_pAPDataItem->GetOpenData());
		m_logicMaker.AddDataItemToUse("O",m_pAPDataItem->GetOpenData());
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetHighData()->GetDataItemName(),m_pAPDataItem->GetHighData());
		m_logicMaker.AddDataItemToUse(_MTEXT( C0_HIGH),m_pAPDataItem->GetHighData());
		m_logicMaker.AddDataItemToUse("H",m_pAPDataItem->GetHighData());
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetLowData()->GetDataItemName(),m_pAPDataItem->GetLowData());
		m_logicMaker.AddDataItemToUse(_MTEXT( C0_LOW),m_pAPDataItem->GetLowData());
		m_logicMaker.AddDataItemToUse("L",m_pAPDataItem->GetLowData());
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetCloseData()->GetDataItemName(),m_pAPDataItem->GetCloseData());	
		m_logicMaker.AddDataItemToUse(_MTEXT( C0_CLOSE),m_pAPDataItem->GetCloseData());
		m_logicMaker.AddDataItemToUse("C",m_pAPDataItem->GetCloseData());
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetVolumeData()->GetDataItemName(),m_pAPDataItem->GetVolumeData());	
		m_logicMaker.AddDataItemToUse(_MTEXT( C0_VOLUME),m_pAPDataItem->GetVolumeData());
		m_logicMaker.AddDataItemToUse("V",m_pAPDataItem->GetVolumeData());
//@Solomon-MultiLang:091117 <--
#else
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetOpenData()->GetDataItemName(),m_pAPDataItem->GetOpenData());
		m_logicMaker.AddDataItemToUse("시가",m_pAPDataItem->GetOpenData());
		m_logicMaker.AddDataItemToUse("O",m_pAPDataItem->GetOpenData());
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetHighData()->GetDataItemName(),m_pAPDataItem->GetHighData());
		m_logicMaker.AddDataItemToUse("고가",m_pAPDataItem->GetHighData());
		m_logicMaker.AddDataItemToUse("H",m_pAPDataItem->GetHighData());
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetLowData()->GetDataItemName(),m_pAPDataItem->GetLowData());
		m_logicMaker.AddDataItemToUse("저가",m_pAPDataItem->GetLowData());
		m_logicMaker.AddDataItemToUse("L",m_pAPDataItem->GetLowData());
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetCloseData()->GetDataItemName(),m_pAPDataItem->GetCloseData());	
		m_logicMaker.AddDataItemToUse("종가",m_pAPDataItem->GetCloseData());
		m_logicMaker.AddDataItemToUse("C",m_pAPDataItem->GetCloseData());
		m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetVolumeData()->GetDataItemName(),m_pAPDataItem->GetVolumeData());	
		m_logicMaker.AddDataItemToUse("기본거래량",m_pAPDataItem->GetVolumeData());
		m_logicMaker.AddDataItemToUse("V",m_pAPDataItem->GetVolumeData());
#endif

		// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
		//<<20100427_JS.Kim 비정상종료 수정
		//m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetExtData()->GetDataItemName(), m_pAPDataItem->GetExtData());
		if( m_pAPDataItem && m_pAPDataItem->GetExtData() )
			m_logicMaker.AddDataItemToUse(m_pAPDataItem->GetExtData()->GetDataItemName(), m_pAPDataItem->GetExtData());
		//>>
	}

	

	m_lCntOfAllApData = m_pAPDataItem->GetCntData();
	if(m_lCntOfAllApData<=0) return FALSE;
	m_ptrArrayHistoryDataItem.RemoveAll();
	m_ptrArrayHistoryDataItem.Add(m_pAPDataItem->GetOpenData());
	m_ptrArrayHistoryDataItem.Add(m_pAPDataItem->GetHighData());
	m_ptrArrayHistoryDataItem.Add(m_pAPDataItem->GetLowData());
	m_ptrArrayHistoryDataItem.Add(m_pAPDataItem->GetCloseData());
	m_ptrArrayHistoryDataItem.Add(m_pAPDataItem->GetVolumeData());
	m_bIsUpdateData = FALSE;

	::SendMessage(m_hParent,UMSG_ENDRECEIVINGFROMSERVER,m_pAPDataItem->GetCntData(),0);
	if(GetError())	return FALSE;
	return TRUE;
}

BOOL CSTController::OnReceiveQuoteDataInRealTime(long lPos, BOOL bIsUpdateData)
{
	if(m_bOperate)
	{
		m_bIsUpdateData = bIsUpdateData;
		::SendMessage(m_hParent,UMSG_ENDREALTIMEDATAFROMSERVER,lPos,0);
		if(GetError())	return FALSE;
	}
	return TRUE;
}


void CSTController::SetSizeDataItem(long lSize)
{
	ASSERT(m_pRootMemManager);
	m_lCntOfAllApData = lSize;
	m_pRootMemManager->SetSizeDataItem(lSize);
}

double CSTController::SubFunc(long lSTActor, long lIndexFunc, long lIndex, BOOL bTemp0, BOOL bTemp1,BOOL bTemp2 , BOOL bTemp3 ,BOOL bTemp4 , BOOL bTemp5 ,BOOL bTemp6 , BOOL bTemp7 ,BOOL bTemp8 , BOOL bTemp9 )
{
	return m_pCurrentPosMemManager->SubFunc(lSTActor, lIndexFunc, lIndex, bTemp0, bTemp1,bTemp2, bTemp3, bTemp4, bTemp5, bTemp6, bTemp7, bTemp8, bTemp9);
}

void CSTController::SetPath(LPCTSTR lpPath)
{
	m_strPath = lpPath;
	m_strSubFuncPath = lpPath;
	int nLength = m_strSubFuncPath.GetLength();
	int nPos = m_strSubFuncPath.ReverseFind('\\');
	m_strSubFuncPath.Delete(nPos,nLength-nPos);
	m_strSfgFile = lpPath;
	nLength = m_strSfgFile.GetLength();
	nPos = m_strSfgFile.ReverseFind('.');
	m_strSfgFile.Delete(nPos,m_strSfgFile.GetLength()-nPos);
	m_strSfgFile += ".SFG";
}

BOOL CSTController::DeclareSubFunc(long lHolderIndex, LPCTSTR lpFuncName)
{
	if(m_pCurrentPosMemManager->IsFirst())
	{
		BOOL bRetValue = m_pCurrentPosMemManager->IncludeSubFunc(lpFuncName);
		if(!bRetValue) 
		{
			m_strError = m_pCurrentPosMemManager->GetErrorMessage();
			return FALSE;
		}
		return m_pCurrentPosMemManager->SetSubFuncHolder(lHolderIndex,lpFuncName);
	}
	return TRUE;
}

BOOL CSTController::IsFirstInThisMem()
{
	return m_pCurrentPosMemManager->IsFirst();
}

void CSTController::SetValueInBuffer(long lIndex,double dValue)
{
	m_pCurrentPosMemManager->SetValueInBuffer(lIndex,dValue);
}

double CSTController::GetValueInBuffer(long lIndex)
{
	return m_pCurrentPosMemManager->GetValueInBuffer(lIndex);
}

BOOL CSTController::SetPlotDataHolder(long lHolderIndex, LPCTSTR lpName)
{
	m_pDataItemPlot[lHolderIndex] = (CSTDataItem *)m_pCurrentPosMemManager->GetVarPointer(lpName);
	return TRUE;
}

double CSTController::GetPlotData(long lHolderIndex, double dIndex)
{
	double dValue = 0;
	long lIndex = m_lCurrentPos - (long)dIndex;
	if(lIndex<0)
	{
		return 0.0;
	}
	BOOL bRetValue = m_pDataItemPlot[lHolderIndex]->GetAt((long)dIndex,&dValue);
	if(bRetValue)
	{
		return dValue;
	}
	return 0.0;
}

void CSTController::SetPlotData(long lHolderIndex,double dIndex, double dValue)
{
	m_pDataItemPlot[lHolderIndex]->SetAt(m_lCurrentPos,dValue);
}

double CSTController::InnerFunc(long lType, long lIndexFunc,long lIndex, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10) 
{
	CInnerFunction *pInnerFunction = m_pCurrentPosMemManager->GetInnerFuncFromHolder(lIndexFunc);
	
	// (2006/3/3 - Seung-Won, Bae) Check NULL
	if( !pInnerFunction) return HUGE_VAL;

	m_pCurrentPosMemManager = m_pCurrentPosMemManager->SubIn(m_lCurrentPos, FALSE);
	m_ValueInInnerFunc =  pInnerFunction->Func(this, lType, varArg1,varArg2,varArg3,varArg4,varArg5,varArg6,varArg7,varArg8,varArg9,varArg10);
	m_pCurrentPosMemManager = m_pCurrentPosMemManager->SubOut(m_lCurrentPos);
	return m_ValueInInnerFunc;
}

BOOL CSTController::DeclareInnerFunc(long lType, long lHolderIndex, LPCTSTR lpFuncName)
{
	if(m_pCurrentPosMemManager->IsFirst())
	{
		CInnerFunction *pInnerFunction = NULL;
		BOOL bExists = m_pTheApp->Lookup(lType, lpFuncName,pInnerFunction);
		if(bExists)
		{
			CString strInput = pInnerFunction->GetInputs(lType);
			DWORD dwSizeRead = 0;
			CString strAppName;
			CString strKeyName;			
			int nPos = strInput.Find('[');
			int nPos2 = -1;
			if(nPos!=-1)
			{
				nPos2 = strInput.Find(']');
				strInput.Delete(nPos2+1,strInput.GetLength()-(nPos2+1));
				
				strInput.Delete(0,nPos+1); // remove until '['
				nPos = strInput.Find(',');
				strAppName = strInput.Left(nPos);
				strInput.Delete(0,nPos+1); //remove until ','
				nPos = strInput.Find(',');
				if(nPos==-1)
				{
					nPos = strInput.Find(']');
				}
				strKeyName = strInput.Left(nPos);
				if(m_strDeclaredInnerFuncInfo.GetLength())
				{
					m_strDeclaredInnerFuncInfo += ',';
				}				
				m_strDeclaredInnerFuncInfo += strKeyName;
				
			}
			return m_pCurrentPosMemManager->SetInnerFuncHolder(lHolderIndex,pInnerFunction);
		}
		m_strError.Format("%s 이름의 내부함수가 존재하지 않습니다.",lpFuncName);
		return FALSE;	
	}
	return TRUE;
}

void CSTController::SetDealPosData(double dValue, long lType, LPCTSTR lpDealName, double dContracts, double dPrice, LPCTSTR lpExitEntryName)
{
	// dIndex is useless parameter..
	if(m_bProcess)
	{	
		if(lpExitEntryName)
		{
			CStringList strListExitEntryName;
			strListExitEntryName.AddTail(lpExitEntryName);
			m_dealDataItem.SetData(m_lCurrentPos,(long)dValue,lType, lpDealName,dPrice,dContracts,&strListExitEntryName);
		}
		else
		{
			m_dealDataItem.SetData(m_lCurrentPos,(long)dValue,lType, lpDealName,dPrice,dContracts,NULL);
		}		
	}	
}

// Bull Bear
BOOL CSTController::SetBullBearDataHolder(LPCTSTR lpName)
{
	m_pDataItemBullBearArea = (CSTDataItem *)m_pCurrentPosMemManager->GetVarPointer(lpName);
	return TRUE;
}

double CSTController::GetBullBearData(double dIndex)
{
	double dValue = 0;
	long lIndex = m_lCurrentPos - (long)dIndex;
	if(lIndex<0)
	{
		return 0.0;
	}
	BOOL bRetValue = m_pDataItemBullBearArea->GetAt((long)dIndex,&dValue);
	if(bRetValue)
	{
		return dValue;
	}
	return 0.0;

}

void CSTController::SetBullBearData(double dIndex, double dValue)	
{
	m_pDataItemBullBearArea->SetAt(m_lCurrentPos,dValue);
}

void CSTController::AddCommentAtDealPosData(LPCTSTR lpTitle, LPCTSTR lpComment)
{
	m_dealDataItem.SetComment(lpTitle,lpComment);
}

void CSTController::AddCommentAtBullBearData(LPCTSTR lpTitle, LPCTSTR lpComment)
{
	m_pDataItemBullBearArea->SetComment(lpTitle,lpComment);
}

void CSTController::AddCommentAtPlotData(long lHolderIndex,LPCTSTR lpTitle, LPCTSTR lpComment)
{
	m_pDataItemPlot[lHolderIndex]->SetComment(lpTitle,lpComment);
}

BOOL CSTController::SetArray(long lHolderIndex, LPCTSTR lpName, int nType, double dInitValue, long lArrayCnt)
{
	if(m_pCurrentPosMemManager->IsFirst())
	{
		m_pCurrentPosMemManager->SetArray(m_lCurrentPos,lpName,nType,dInitValue,lArrayCnt);
		return m_pCurrentPosMemManager->SetArrayHolder(lHolderIndex,lpName);
	}
	return TRUE;
}

double CSTController::GetArrayHolderData(long lHolderIndex, double dIndex)
{
	double dValue = 0;
	long lIndex = (long)dIndex;
	if(lIndex<0)
	{
		return 0.0;
	}
	BOOL bRetValue = m_pCurrentPosMemManager->GetArrayHolderData(lHolderIndex,lIndex, &dValue);
	if(bRetValue)
	{
		return dValue;
	}
	return 0.0;
}

void CSTController::SetArrayHolderData(long lHolderIndex,double dIndex, double dValue)
{
	m_pCurrentPosMemManager->SetArrayHolderData(lHolderIndex,dIndex,dValue);
}

CIDataItem* CSTController::GetVarHolderDataItem(long lHolderIndex)
{
	return m_pCurrentPosMemManager->GetVarHolderPointer(lHolderIndex);
}

CIDataItem* CSTController::GetArrayHolderDataItem(long lHolderIndex)
{
	return m_pCurrentPosMemManager->GetArrayHolderPointer(lHolderIndex);
}

CIDataItem* CSTController::GetInputHolderDataItem(long lHolderIndex)
{
	return (CSTDataItem*)m_pRootMemManager->GetInputHolder(lHolderIndex);
}

CIDataItem* CSTController::GetPlotHolderDataItem(long lHolderIndex)
{
	return m_pDataItemPlot[lHolderIndex];
}

CString CSTController::GetCommentAtPlotData(long lHolderIndex,LPCTSTR lpTitle)
{
	return m_pDataItemPlot[lHolderIndex]->GetComment(lpTitle);
}

CString CSTController::GetCommentAtDealPosData(LPCTSTR lpTitle)
{
	return m_dealDataItem.GetComment(lpTitle);
}

CString CSTController::GetCommentAtBullBearData(LPCTSTR lpTitle)
{
	if(m_pDataItemBullBearArea)
	{
		return m_pDataItemBullBearArea->GetComment(lpTitle);
	}
	return "";
}

CString CSTController::GetUserID()
{
	return m_strUserID;
}

long CSTController::GetExpiredDate()
{
	return m_lExpiredDate;
}

void CSTController::SetUserID(LPCTSTR lpUserID)
{
	m_strUserID = lpUserID;	
}

void CSTController::SetExpiredDate(long lExpriedDate)
{
	m_lExpiredDate = lExpriedDate;
}

CString CSTController::GetPassword()
{
	return m_strPassword;
}

void CSTController::SetPassword(LPCTSTR lpPassword)
{
	m_strPassword = lpPassword;
}

long CSTController::GetSourceType()
{
	return m_lSourceType;
}

void CSTController::SetSourceType(long lSourceType)
{
	m_lSourceType = lSourceType;
}

void CSTController::SetUseUserID(BOOL bUse)
{
	m_bUseUserID = bUse;
}

void CSTController::SetUsePassword(BOOL bUse)
{
	m_bUsePassword = bUse;
}

void CSTController::SetUseExpiredDate(BOOL bUse)
{
	m_bUseExpiredDate = bUse;
}

BOOL CSTController::GetUseUserID()
{
	return m_bUseUserID;
}

BOOL CSTController::GetUsePassword()
{
	return m_bUsePassword;
}

BOOL CSTController::GetUseExpiredDate()
{
	return m_bUseExpiredDate;
}

void CSTController::SetSTName(LPCTSTR lpSTName, int nSTPos)
{
	m_strSTName = lpSTName;
	m_nSTPos = nSTPos;
}


CITempMemMng *CSTController::GetITempMemMng()
{
	return m_pCurrentPosMemManager->GetITempMemMng();
}

void CSTController::SetAdditonalOption(LPCTSTR lpAdditionalOption)
{
}

double CSTController::GetInputData(LPCTSTR lpName, double dIndex)
{
	double dValue = 0;
	long lIndex = m_lCurrentPos - (long)dIndex;
	if(lIndex<0)
	{
		return 0.0;
	}
	BOOL bRetValue = m_pCurrentPosMemManager->GetInputData(lpName,lIndex,&dValue);
	if(bRetValue)
	{
		return dValue;
	}
	return 0.0;
}

double CSTController::GetInputDataInRoot(LPCTSTR lpName, double dIndex)
{
	double dValue = 0;
	long lIndex = m_lCurrentPos - (long)dIndex;
	if(lIndex<0)
	{
		return 0.0;
	}
	BOOL bRetValue = m_pRootMemManager->GetInputData(lpName,lIndex,&dValue);
	if(bRetValue)
	{
		return dValue;
	}
	return 0.0;
}

CString CSTController::GetInputs(long lIndex)
{
	if(m_pStringArrayInputs)
	{
		if(lIndex<m_pStringArrayInputs->GetSize())
		{
			return m_pStringArrayInputs->GetAt(lIndex);
		}		
	}
	return "";
}

void CSTController::SetpStringArrayInputs(CStringArray *pStringArrayInputs)
{
	m_pStringArrayInputs = pStringArrayInputs;
}

long CSTController::Command(LPCTSTR lpCommand, LPCTSTR lpOption)
{
	CString strCommand = lpCommand;
	return 1;
}

LPCTSTR CSTController::GetError()
{
	if(m_strError.GetLength())
	{
		return (LPCTSTR)m_strError;
	}
	return NULL;
}

//KHD : ChartItem 추가 : 2006.08.04
void CSTController::SetChartItem(long pChartItem)
{
	m_pChartItem = pChartItem;
}

void CSTController::ResetLastSignal_ForReal(long lCurrentPos)
{
	m_dealDataItem.SetData(lCurrentPos, 0);
}
