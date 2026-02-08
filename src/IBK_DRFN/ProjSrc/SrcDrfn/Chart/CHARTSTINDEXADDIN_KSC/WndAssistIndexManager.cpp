//-->090902-solomon : alzioyes ST에서의 @수정.
// 솔로몬의 경우 @가 버전처리되는 파일에 사용이 되면 안되기 때문에 
// '@'를 '^'으로 수정함.
//-->090902-solomon : alzioyes ST에서의 @수정.

// WndAssistIndexManager.cpp : implementation file
//

#include "stdafx.h"
#include "Winsock2.h"
#include <mmsystem.h>

#include "./Include_AddIn/I139092/_IGraphInfo.h"		// for IGraphInfo
#include "./Include_AddIn/I139092/_IGraphInfoManager.h"	// for IGraphInfoManager
#include "./Include_AddIn/I139092/_IPacket.h"			// for IPacket
#include "./Include_AddIn/I139092/_IPacketManager.h"	// for IPacketManager
#include "./Include_AddIn/I139092/_IChartManager.h"		// for IChartManager
#include "./Include_AddIn/I139092/_IBlock.h"
#include "./Include_AddIn/I139092/_IDoubleList.h"		// for IDoubleList

#include "../Common_ST/STConfigdef.h"
#include "../Common_ST/DealDataItem.h"
#include "../Include_ST/DumpLogger.h"					// for CDumpLogger
#include "../Include_ST/DebugDefine.h"					// for _PLOT_LOG

#include "WndAssistIndexManager.h"
#include "ChartSTIndexAddIn.h"
#include "IndexItem.h"
#include "SiteCustomAddin.h" //추가 :  신호 호출을 위해
#include "../../inc/ISTMng.h"						// No41.ISTManager
#include "../../inc/IPCTRMng.h"
#include "../../inc/RealUpdate.h"

#include "../../inc/IAuTrCommMng.h"
#include "../../inc/IMainInfoMng.h"//KHD

#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../chart_common/Include_Chart/Conversion.h"
#include "../../inc/ISTManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct{	
	char sMsg[80];	
	char sFiller[48];
} FINDER_HD;

const UINT RMSG_STCONFIGURENAME = ::RegisterWindowMessage("RMSG_STCONFIGURENAME");// when user clicks OK or Cancel At Configure Dlg, send it...
const UINT RMSG_STDATAMANAGER_RESULTDATA = ::RegisterWindowMessage("RMSG_STDATAMANAGER_RESULTDATA");// after calculating, result data is received by this msg.
const UINT RMSG_STCONTROLLER_POINTER = ::RegisterWindowMessage("RMSG_STCONTROLLER_POINTER");// to use stcontroller, this msg is used.

extern CChartSTIndexAddInApp theApp;

//__MULTI_LANG__
LPCTSTR g_strChartType[] =
{
	"_Candle Type_",
	"_Line Type_",
	"_Bar Type_"
};

#define PACKET_TIME		"_DateTime_"
#define PACKET_OPEN		"_Open_"
#define PACKET_HIGH		"_High_"
#define PACKET_LOW		"_Low_"
#define PACKET_VOLUME	"_Volume_"
#define PACKET_CLOSE	"_Close_"

//청산조건//////////////////////////////
#define		TRS_ST_SECTION			"RunSetup"

#define	QACNT_MAX	1

#define	GET_LASTFOLDER_NAME(strDir, p_LastFolder)\
{\
	CString	_strRoot_ = strDir;\
	int _loc_ = _strRoot_.ReverseFind('\\');\
	if( _loc_ > 0)\
	p_LastFolder = _strRoot_.Mid(_loc_+1);\
}

/////////////////////////////////////////////////////////////////////////////
// CWndAssistIndexManager

CWndAssistIndexManager::CWndAssistIndexManager()
{
	m_pIChartManager = NULL;
	m_pIChartOCX = NULL;
	m_pIPacketManager = NULL;
	m_pIGraphInfoManager = NULL;
	m_pIPacketTime = NULL;
	m_pIPacketOpen = NULL;
	m_pIPacketHigh = NULL;
	m_pIPacketLow = NULL;
	m_pIPacketClose = NULL;
	m_pIPacketVolume = NULL;
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	m_pIPacketExt = NULL;
	m_lCntOfCurrentPacketData = 0;
	m_pSTItem = NULL;
	m_pSTItemCfg = NULL;
	m_rectTitle.SetRectEmpty();

	m_lHeightBuy = 0;
	m_lHeightSell = 0;
	m_lHeightExitLong = 0;
	m_lHeightExitShort = 0;
	m_bIsDragging = FALSE;
//	m_pStDataIndex = NULL;
	m_bSetDataFirst = TRUE;

	m_pStrategyItem = NULL;			// WID_060913_A1
	m_bSinHo =FALSE;
	m_pIndexItem = NULL;
	m_stCode ="000000";
	m_pStieCustomAddin =NULL;

	m_pITrComm = NULL;

	m_hSysTabEx = NULL;		//@@@@@06.11.20 HWND
	m_pstOrderData = new STORDER_INDATA;
	::memset(m_pstOrderData, 0, SZSTORDER_INDATA);
	m_bUseExitSound = m_bUseEntrySound = FALSE;

	m_strRQ = _T("DEFAULT");	// 사용되고 있는 RQ :복수종목 - ojtaso (20070307)

	m_pstrategyLoader = NULL;
}

CWndAssistIndexManager::~CWndAssistIndexManager()
{
	DestroyStrategyItem();

	// (2006/1/19 - Seung-Won, Bae) Release Interfaces
	if( m_pIChartManager)
	{
		m_pIChartManager->Release();
		m_pIChartManager = NULL;
	}
	if( m_pIChartOCX)
	{
		m_pIChartOCX->Release();
		m_pIChartOCX = NULL;
	}
	if( m_pIPacketManager)
	{
		m_pIPacketManager->Release();
		m_pIPacketManager = NULL;
	}
	if( m_pIGraphInfoManager)
	{
		m_pIGraphInfoManager->Release();
		m_pIGraphInfoManager = NULL;
	}
	if( m_pIPacketTime)
	{
		m_pIPacketTime->Release();
		m_pIPacketTime = NULL;
	}
	if( m_pIPacketOpen)
	{
		m_pIPacketOpen->Release();
		m_pIPacketOpen = NULL;
	}
	if( m_pIPacketHigh)
	{
		m_pIPacketHigh->Release();
		m_pIPacketHigh = NULL;
	}
	if( m_pIPacketLow)
	{
		m_pIPacketLow->Release();
		m_pIPacketLow = NULL;
	}
	if( m_pIPacketClose)
	{
		m_pIPacketClose->Release();
		m_pIPacketClose = NULL;
	}
	if( m_pIPacketVolume)
	{
		m_pIPacketVolume->Release();
		m_pIPacketVolume = NULL;
	}
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	if( m_pIPacketExt)
	{
		m_pIPacketExt->Release();
		m_pIPacketExt = NULL;
	}

	m_StDataIndex.RemoveAll();

	POSITION pos = m_mapOrderData.GetStartPosition();
	UINT nKey;
	STORDER_INDATA* rValue;
	while(pos)
	{
		rValue = NULL;
		m_mapOrderData.GetNextAssoc(pos, nKey, rValue);
		if(rValue)
			delete rValue;
	}

	m_mapOrderData.RemoveAll();

	pos = m_mapProcOrderData.GetStartPosition();
	PROCEED_ORDER* rValue2;
	while(pos)
	{
		rValue = NULL;
		m_mapProcOrderData.GetNextAssoc(pos, nKey, rValue2);
		if(rValue2)
			delete rValue2;
	}

	m_mapProcOrderData.RemoveAll();
}


BEGIN_MESSAGE_MAP(CWndAssistIndexManager, CWnd)
	//{{AFX_MSG_MAP(CWndAssistIndexManager)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_STDATAMANAGER_RESULTDATA, OnSTDataManagerMessage)
	ON_REGISTERED_MESSAGE(RMSG_STCONTROLLER_POINTER, OnSTController)
	ON_REGISTERED_MESSAGE(RMSG_STCONFIGURENAME, OnUpdateSTSfg)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CWndAssistIndexManager message handlers

BOOL CWndAssistIndexManager::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, IChartManager *p_pIChartManager, IChartOCX *p_pIChartOCX, IPacketManager *p_pIPacketManager, IGraphInfoManager * p_pIGraphInfoManager) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( !p_pIChartManager || !p_pIChartOCX || !p_pIPacketManager) return FALSE;	
	
	// (2006/1/19 - Seung-Won, Bae) Get Reference
	m_pIChartManager = p_pIChartManager;
	m_pIChartManager->AddRef();
	m_pIChartOCX = p_pIChartOCX;
	m_pIChartOCX->AddRef();
	m_pIPacketManager = p_pIPacketManager;
	m_pIPacketManager->AddRef();
	m_pIGraphInfoManager = p_pIGraphInfoManager;
	m_pIGraphInfoManager->AddRef();

	// (2006/1/19 - Seung-Won, Bae) GetHtsPath() It have not returned CString. Only const char.
	//m_strPath = p_pIChartOCX->GetHtsPathString();
	ILPCSTR szHtsPath = p_pIChartOCX->GetHtsPathString();
	m_strPath = (LPCSTR)szHtsPath;

	if(!theApp.IsInitedMapIndex())
	{
		theApp.InitMapIndex(m_strPath);
	}
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, NULL);
}

///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	지표 전략을 생성한다.
//////////////////////////////////////////////////////////////////////////////
CIndexItem * CWndAssistIndexManager::CreateIndexItem(LPCTSTR lpszParamCombine, CList<IPacket*, IPacket*>* pListIPacket,BOOL &rBHasPriceInput)
{
	CString strType;
	CString strScaleType;
	CString strGraphPath = theApp.GetIndexFile(lpszParamCombine,rBHasPriceInput,strType,strScaleType);

	CIndexItem *pIndexItem = NULL;
	BOOL bHasData = m_mapIndexItem.Lookup(lpszParamCombine,pIndexItem);
	if(bHasData)
	{			
		//<<20100312_JS.Kim 솔로몬. 종목별로 패킷타입이 다르므로 매번 셋팅해 준다.
		// 20081021 박승룡 - 종목에 따라 scaletype 변경처리
		if( pIndexItem)
		{
			if( strType.IsEmpty() || strScaleType.IsEmpty())
			{
				strType = m_strCloseScaleType;
				strScaleType = m_strCloseScaleType;
			}
			pIndexItem->m_strType = strType;
			pIndexItem->m_strScaleType = strScaleType;
		}
		//>>

		CString strIndexName;
		rBHasPriceInput = m_MapIndexNoPriceInput.Lookup(lpszParamCombine,strIndexName);
		rBHasPriceInput = !rBHasPriceInput;
		return pIndexItem;
	}

	// 20081021 박승룡 - 종목에 따라 scaletype 변경처리
	if( strType.IsEmpty() || strScaleType.IsEmpty())
	{
		strType = m_strCloseScaleType;
		strScaleType = m_strCloseScaleType;
	}
	if( pIndexItem)
	{
		if( strType.IsEmpty() || strScaleType.IsEmpty())
		{
			strType = m_strCloseScaleType;
			strScaleType = m_strCloseScaleType;
		}
		pIndexItem->m_strType = strType;
		pIndexItem->m_strScaleType = strScaleType;
		return pIndexItem;
	}
	if(strGraphPath.GetLength()<=0) return NULL;
	pIndexItem = new CIndexItem;
	pIndexItem->m_indexType = AssistIndex;
	
	// (2006/4/24 - Seung-Won, Bae) Set Index Name to CIndexItem for Log
	pIndexItem->m_strSTItemName = lpszParamCombine;

	CString strOption;
	strOption.Format("%s=%s",LOAD_MODE,MODE_ACTION);
	long lRetValue = pIndexItem->m_DllSTLoader.Load(strGraphPath,this->GetSafeHwnd(), (long)pIndexItem,NULL,0,strOption);
	if(lRetValue==-1)
	{
		delete pIndexItem;
		pIndexItem = NULL;
		return pIndexItem;
	}


#if _PLOT_LOG
	// (2006/4/24 - Seung-Won, Bae) Log for Output Binding
	CString strBuffer;
	strBuffer.Format( "====================================================[%s] : Bind to Packet\r\n", pIndexItem->m_strSTItemName);
	CDumpLogger::LogStringToFile( NULL, strBuffer, strBuffer.GetLength(), "IndiValue");
#endif

	// (2006/1/19 - Seung-Won, Bae) Get Interface References
	IPacket *pIPacket = NULL;
	POSITION pos = pListIPacket->GetHeadPosition();
	while(pos)
	{
		pIPacket = pListIPacket->GetNext(pos);

#if _PLOT_LOG
		// (2006/4/24 - Seung-Won, Bae) Log for Output Binding
		pIPacket->GetPacketName( strBuffer);
		strBuffer += "\r\n";
		CDumpLogger::LogStringToFile( NULL, strBuffer, strBuffer.GetLength(), "IndiValue");
#endif

		pIndexItem->m_ListOutputIPacket.AddTail( pIPacket);
		pIPacket->AddRef();
	}

	m_mapIndexItem.SetAt(lpszParamCombine,pIndexItem);
	if(!rBHasPriceInput)			 
	{
		m_MapIndexNoPriceInput.SetAt(lpszParamCombine,lpszParamCombine);
	}
	
	pIndexItem->m_strType = strType;
	pIndexItem->m_strScaleType = strScaleType;
	return pIndexItem;
}

///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	지표 전략을 제거한다.
//////////////////////////////////////////////////////////////////////////////
BOOL CWndAssistIndexManager::RemoveIndexItem(LPCTSTR lpszParamCombine)
{
	CIndexItem *pIndexItem = NULL;
	CString szParamCombine = lpszParamCombine;
	CString szFoundParamCombine;
	POSITION pos = m_mapIndexItem.GetStartPosition();
	while(pos)
	{
		m_mapIndexItem.GetNextAssoc(pos, szFoundParamCombine, pIndexItem);
		if(szParamCombine.Compare(szFoundParamCombine) == 0)
		{
			m_mapIndexItem.RemoveKey(szParamCombine);
			delete pIndexItem;
			pIndexItem = NULL;
			return TRUE;
		}
	}

	return FALSE;
}


void CWndAssistIndexManager::OnDestroy() 
{
	// TODO: Add your message handler code here	
	CIndexItem *pIndexItem = NULL;
	CString strDllSTName;
	POSITION pos = m_mapIndexItem.GetStartPosition();
	while(pos != NULL)
	{		
		m_mapIndexItem.GetNextAssoc(pos, strDllSTName, pIndexItem);
		delete pIndexItem;
	}
	m_mapIndexItem.RemoveAll();

	DeleteSTInfo();
	DestroyStrategyItem();	// WID_060913_A1

	if(m_pstOrderData)
	{
		delete m_pstOrderData;
		m_pstOrderData = NULL;
	}

	if(m_pSTItemCfg)
	{
		delete m_pSTItemCfg;
		m_pSTItemCfg = NULL;
	}

	if(m_brushSell.GetSafeHandle())			m_brushSell.DeleteObject();
	if(m_brushBuy.GetSafeHandle())			m_brushBuy.DeleteObject();
	if(m_brushExitLong.GetSafeHandle())		m_brushExitLong.DeleteObject();
	if(m_brushExitShort.GetSafeHandle())	m_brushExitShort.DeleteObject();

	if(m_rgnSell.GetSafeHandle())		m_rgnSell.DeleteObject();
	if(m_rgnBuy.GetSafeHandle())		m_rgnBuy.DeleteObject();
	if(m_rgnExitLong.GetSafeHandle())	m_rgnExitLong.DeleteObject();
	if(m_rgnExitShort.GetSafeHandle())	m_rgnExitShort.DeleteObject();

	if(m_pITrComm)
	{
		IAUTrCommManager* pICommManager = (IAUTrCommManager*)AfxGetPctrInterface(2);
		pICommManager->UnAdviseTrComm(m_pITrComm);
	}
	CWnd::OnDestroy();
}



///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	지표를 그린다.
//////////////////////////////////////////////////////////////////////////////
void CWndAssistIndexManager::AddGraph(LPCTSTR lpszParamCombine, CList<IPacket*, IPacket*>* pListIPacket)
{
	//<<20100323_JS.Kim 솔로몬.FX 맵차트콘트롤에서 매도/매수를 동시에 그리기 위해 종가 데이타가 매수종가/매도종가 2가지가 있다.
	//if(!m_pIPacketTime||(!m_pIPacketOpen&&!m_pIPacketHigh&&!m_pIPacketLow&&!m_pIPacketClose&&!m_pIPacketVolume))
	//{
	//	CheckRawData();
	//}

	// (2006/2/28 - Seung-Won, Bae) Release Graph Info Interface.
	CString szParamCombine = lpszParamCombine;
	int nDivinePos = szParamCombine.Find(";");
	CString szIndicatorName = szParamCombine.Left(nDivinePos);
	CString szGraphName = szParamCombine.Mid(nDivinePos + 1);
	if( szIndicatorName == "_Price MA_" && (szGraphName == "_SellClose_" || szGraphName == "_BuyClose_"))
	{
		CheckRawData();
	}
	else if(!m_pIPacketTime||(!m_pIPacketOpen&&!m_pIPacketHigh&&!m_pIPacketLow&&!m_pIPacketClose&&!m_pIPacketVolume))
	{
		CheckRawData();
	}
	//>>

	BOOL bHasPriceInput = FALSE;
	CIndexItem *pIndexItem = CreateIndexItem(lpszParamCombine,pListIPacket,bHasPriceInput);
	if(pIndexItem)
	{
		BOOL bIsMASet = FALSE;
		CString strAdditional;
		CString strAdditionalItem;
		int nIndexAdditional = 0;
		// CList<double,double>* pListConditionList = NULL;
		IDoubleList* pListConditionList = NULL;
		double dValue = 0;
		//// (2006/2/28 - Seung-Won, Bae) Release Graph Info Interface.
		//CString szParamCombine = lpszParamCombine;
		//int nDivinePos = szParamCombine.Find(";");
		//CString szIndicatorName = szParamCombine.Left(nDivinePos);
		//CString szGraphName = szParamCombine.Mid(nDivinePos + 1);
		IGraphInfo *pIGraphInfo = m_pIGraphInfoManager->GetGraphInfo(IGraphInfoManager::IEILT_USER, szGraphName);

		if( pIGraphInfo)
		{
			//pListConditionList = pIGraphInfo->GetCommonCalculateVarList();
			pListConditionList = pIGraphInfo->GetCommonCalculateConditionList();
			POSITION pos = pListConditionList->GetHeadPosition();
			if(pos)
			{
				while(pos)
				{
					pListConditionList->GetNext(pos, dValue);
					strAdditionalItem.Format("%s%d=%f",INPUTVALUE,nIndexAdditional,dValue);
					strAdditional += strAdditionalItem;
					strAdditional += ',';
					nIndexAdditional++;
				}
			}
			else
			{		// for MA
				BOOL bShow=FALSE;
				bIsMASet = TRUE;
				CString strPacketName;
				//CList<double,double>* pListConditionListIndexed;
				IDoubleList* pListConditionListIndexed = NULL;
				POSITION pos;
				long lIndex = 0;
				//pListConditionListIndexed = pIGraphInfo->GetCalculateVarList(lIndex);
				pListConditionListIndexed = pIGraphInfo->GetCalculateConditionList((const int)lIndex);
				while(pListConditionListIndexed)
				{
					pos = pListConditionListIndexed->GetHeadPosition();
					pIGraphInfo->GetSubGraphShowFlag(lIndex, bShow);
					while(pos)
					{
						pListConditionListIndexed->GetNext(pos, dValue);
						if(!bShow) dValue = 0;
						strAdditionalItem.Format("%s%d=%f",INPUTVALUE,nIndexAdditional,dValue);			
						strAdditional += strAdditionalItem;
						strAdditional += ',';
						nIndexAdditional++;
					}
					if(pListConditionListIndexed) pListConditionListIndexed->Release();	// IDoubleList로 받은것은 Release를 꼭 해준다.

					lIndex++;
					//pListConditionListIndexed = pIGraphInfo->GetCalculateVarList(lIndex);
					pListConditionListIndexed = pIGraphInfo->GetCalculateConditionList((const int)lIndex);
				}
				if(pListConditionListIndexed) pListConditionListIndexed->Release();	// IDoubleList로 받은것은 Release를 꼭 해준다.
				pListConditionListIndexed = NULL;
			}

			if(pListConditionList) pListConditionList->Release();	// IDoubleList로 받은것은 Release를 꼭 해준다.
			pListConditionList = NULL;

			if(bHasPriceInput)
			{
				if(!bIsMASet)
				{
					CString strPacketName;
					int nPos = 0;
					//strPacketName = pIGraphInfo->GetCommonCalculatePacketNames();
					 ILPCSTR istTmp = pIGraphInfo->GetPacketNameString();
					 // strPacketName = (LPCSTR)istTmp;
					 strPacketName = istTmp;

					if(strPacketName.GetLength())// only one name is sent.
					{
						strPacketName.TrimRight(',');
						nPos = strPacketName.ReverseFind(',');
						if(nPos!=-1) strPacketName = strPacketName.Mid(nPos+1);
						strAdditionalItem.Format("%s%d=%s",INPUTVALUE,nIndexAdditional,strPacketName);
						strAdditional += strAdditionalItem;					
						nIndexAdditional++;
					}
				}
				else
				{	// several names are sent.
					long lIndex = 0;
					int nPos = 0;
					CString strPacketName;
					//strPacketName = pIGraphInfo->GetCalculatePacketNames(lIndex);
					ILPCSTR szPacketName = pIGraphInfo->GetSubGraphPacketNameString(lIndex);
					strPacketName = szPacketName;

					// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
					if(strPacketName.IsEmpty())
					{
						szPacketName = pIGraphInfo->GetPacketNameString();
						strPacketName = szPacketName;
						
						if(strPacketName.GetLength())// only one name is sent.
						{
							strPacketName.TrimRight(',');
							nPos = strPacketName.ReverseFind(',');
							if(nPos!=-1) strPacketName = strPacketName.Mid(nPos+1);
							strAdditionalItem.Format("%s%d=%s",INPUTVALUE,nIndexAdditional,strPacketName);
							strAdditional += strAdditionalItem;					
							nIndexAdditional++;
						}
					}
					else
					{
						while(strPacketName.GetLength())
						{
							strPacketName.TrimRight(',');
							nPos = strPacketName.ReverseFind(',');
							if(nPos!=-1) strPacketName = strPacketName.Mid(nPos+1);
							strAdditionalItem.Format("%s%d=%s",INPUTVALUE,nIndexAdditional,strPacketName);
							strAdditional += strAdditionalItem;
							strAdditional += ',';
							nIndexAdditional++;
							lIndex++;
							//strPacketName = pIGraphInfo->GetCalculatePacketNames(lIndex);
							szPacketName = pIGraphInfo->GetSubGraphPacketNameString(lIndex);
							strPacketName = szPacketName;
						}
						strAdditional.TrimRight(',');
					}
				}
			}
			pIGraphInfo->Release();
		}
		pIndexItem->SetInputValue(strAdditional);

		pIndexItem->m_DllSTLoader.Stop();

//#if _PLOT_LOG
//		// (2006/3/30 - Seung-Won, Bae) Log Title for Indicator Dump Data
//		CString strBuffer;
//		strBuffer.Format( "====================================================[%s] : AddGraph\r\n", lpGraphName);
//		CDumpLogger::LogStringToFile( NULL, strBuffer, strBuffer.GetLength(), "IndiValue");
//		strBuffer = m_strBasicalExecutionOption + pIndexItem->GetInputValue() + "\r\n";
//		CDumpLogger::LogStringToFile( NULL, strBuffer, strBuffer.GetLength(), "IndiValue");
//#endif
		pIndexItem->m_DllSTLoader.Execute(m_strBasicalExecutionOption + pIndexItem->GetInputValue());
	}
}


///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	m_DllSTLoader가 Execute하면 RMSG_STDATAMANAGER_RESULTDATA에 의하여 결과값을 받는다.
//////////////////////////////////////////////////////////////////////////////
#include "SiteCustomAddin.h"
#include ".\wndassistindexmanager.h"
LRESULT CWndAssistIndexManager::OnSTDataManagerMessage( WPARAM wParam, LPARAM lParam)
{
	CIndexItem *pIndexItem = (CIndexItem *)wParam;// unique key
	ST_DRAWINFO_EX* pInfo = (ST_DRAWINFO_EX*)lParam;	// result info
	int nPos = 0;
	if(pIndexItem->m_indexType==AssistIndex)// it's for assist index :지표
	{	
		CISDataItem *pISDataItem = NULL;
		CIDataItem *pIDataItem[256];
		IPacket *pPacketData[256];
		long lPacketIndex = 0;
		int nCntOfPlot = pInfo->pPtrArrayPlot->GetSize();
		int nCntOfPacket = pIndexItem->m_ListOutputIPacket.GetCount();

		// (2006/3/3 - Seung-Won, Bae) Support Partial Packets
		//	if( nCntOfPacket<nCntOfPlot) return FALSE;
		if(!nCntOfPacket||!nCntOfPlot) return FALSE;


		POSITION pos = pIndexItem->m_ListOutputIPacket.GetHeadPosition();
		while(pos)
		{
			pPacketData[lPacketIndex] = pIndexItem->m_ListOutputIPacket.GetNext(pos);// receiver Buffer
			pIDataItem[lPacketIndex] = (CIDataItem *)pInfo->pPtrArrayPlot->GetAt(lPacketIndex);// sender buffer

			lPacketIndex++;
			if(lPacketIndex>=nCntOfPlot) break;
		}
		CString strPacketName;
		int nTemp = 0;
		long lTemp= 0;
		int nIndex = 0;
		int nSizeTime = m_SDataItemDate.GetSize();
		
		if(!pInfo->bIsInRealTime)// when initializing, set scaletype and type and fill receiver.
		{
			CString strScaleType;
			if(pIndexItem->m_strScaleType.GetLength())
			{
				if(pIndexItem->m_strScaleType==PACKET_CLOSE)
				{
					strScaleType = m_strCloseScaleType;
				}
				else if(pIndexItem->m_strScaleType==PACKET_OPEN)
				{
					strScaleType = m_strOpenScaleType;
				}
				else if(pIndexItem->m_strScaleType==PACKET_HIGH)
				{
					strScaleType = m_strHighScaleType;
				}
				else if(pIndexItem->m_strScaleType==PACKET_LOW)
				{
					strScaleType = m_strLowScaleType;
				}
				else if(pIndexItem->m_strScaleType==PACKET_VOLUME)
				{
					strScaleType = m_strVolumeScaleType;
				}
				else
				{
					// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
					ILPCSTR lpszSTDataName = m_pIChartOCX->GetUseSTDataName();
					CString strSTDataName = lpszSTDataName;
					if(!strSTDataName.IsEmpty() && pIndexItem->m_strScaleType==strSTDataName)
						strScaleType = m_strExtScaleType;
					else
						strScaleType = pIndexItem->m_strScaleType;
				}
			}

			long lValueTemp = 0;
			long lFirstDataIndex = 0;
			for(nIndex = 0;nIndex<lPacketIndex;nIndex++)
			{
				//strPacketName = pPacketData[nIndex]->GetPacketName();
				ILPCSTR istTmp = pPacketData[nIndex]->GetPacketNameString();
				strPacketName = istTmp;

				pPacketData[nIndex]->ClearData();
				pPacketData[nIndex]->SetType(pIndexItem->m_strType,strScaleType);
				lFirstDataIndex = pIDataItem[nIndex]->GetFirstDataIndex()-1;
				if(lFirstDataIndex>nSizeTime)	lFirstDataIndex = nSizeTime;
				// Y축 환경설정에서 전체데이터 최고저 따라기기일때 최소값이 0나오는 버그 수정 - ojtaso (20071129)
				m_pIChartOCX->SetStartIndexOfPacket(m_strRQ, strPacketName,lFirstDataIndex,FALSE);
				for( nPos=0;nPos<lFirstDataIndex;nPos++)
				{		
					pPacketData[nIndex]->AppendData(0.);
				}
				for(nPos=lFirstDataIndex;nPos<nSizeTime;nPos++)
				{		
					pPacketData[nIndex]->AppendData(pIDataItem[nIndex]->GetAt(nPos));
				}				
				m_pIChartOCX->SetStartIndexOfPacket(m_strRQ, strPacketName,lFirstDataIndex,TRUE);
			}
		}
		else	// update or insert data..
		{
			double dValueInDataItem = 0.;
			long lValueTemp = 0;
			long lCntPacketData = 0;
			long lIndexDataItem = nSizeTime-1;
			long lFirstDataIndex = 0;
			for(nIndex = 0;nIndex<lPacketIndex;nIndex++)
			{
				//strPacketName = pPacketData[nIndex]->GetPacketName();
				ILPCSTR  istTmp = pPacketData[nIndex]->GetPacketNameString();
				strPacketName = istTmp;

				lCntPacketData = pPacketData[nIndex]->GetDataCount();
				lFirstDataIndex = pIDataItem[nIndex]->GetFirstDataIndex()-1;
				if(lFirstDataIndex>nSizeTime)	lFirstDataIndex = nSizeTime;
				dValueInDataItem = pIDataItem[nIndex]->GetAt(lIndexDataItem);
				if(lFirstDataIndex>lIndexDataItem)
				{
					dValueInDataItem = 0;
				}			
				// Y축 환경설정에서 전체데이터 최고저 따라기기일때 최소값이 0나오는 버그 수정 - ojtaso (20071129)
				if(lFirstDataIndex>=nSizeTime)
				{
					m_pIChartOCX->SetStartIndexOfPacket(m_strRQ, strPacketName,lFirstDataIndex,FALSE);					
				}			
				if(lCntPacketData<nSizeTime)
				{
					pPacketData[nIndex]->AppendData(dValueInDataItem);
				}
				else
				{
					pPacketData[nIndex]->SetData(lIndexDataItem,dValueInDataItem);
				}			
				if(lFirstDataIndex>=nSizeTime)
				{
					m_pIChartOCX->SetStartIndexOfPacket(m_strRQ, strPacketName,lFirstDataIndex,TRUE);					
				}			
			}		
		}
	}
	else if(pIndexItem->m_indexType==STIndex)// it's for systemtrading Index..
	{
		CDealDataItem *pDealDataItem = (CDealDataItem *)pInfo->pDealDataItem;
		CPtrArray* pPtrArrayPlot = pInfo->pPtrArrayPlot;
		int nSizeArrayPlot = pPtrArrayPlot->GetSize();
		int nTotalDataCnt	= m_SDataItemDate.GetSize();
		CIDataItem *pDataItem = NULL;
		CIDataItem *pDataItemBullBearArea = NULL;
		///////////// Set Plot pointer
		///////////////////////////////
		CString strDataItemName;
		CString strDataItemName4;
		CString strDataItemName8;

		long lType = 0;
		long lShape = 0;
		long lThick = 0;
		long lChartStyle = 0;
		COLORREF clrIndex = RGB(0,0,0);
		CString strChartType;
		CStringArray arrayPacket;
		BOOL bFirstIndexGraph = TRUE;
		IPacket *pIPacketDataItem = NULL;

		for(int nPos=0;nPos<nSizeArrayPlot;nPos++)// Make graph index... and find BullBear Data
		{
			pDataItem = (CIDataItem *)pPtrArrayPlot->GetAt(nPos);
			strDataItemName = pDataItem->GetDataItemName();
			strDataItemName4 = strDataItemName.Left(4);
			strDataItemName8 = strDataItemName.Left(8);
			if(strDataItemName4=="Plot")
			{
				if(!pInfo->bIsInRealTime)
				{
					lType = atol(pDataItem->GetComment(INDEXTYPE));
					clrIndex = atol(pDataItem->GetComment(INDEXCOLOR));
					lShape = atol(pDataItem->GetComment(INDEXSHAPE));
					lThick = atol(pDataItem->GetComment(INDEXTHICK)) + 1;
					switch(lType)
					{
					case 1://dot style
						strChartType = g_strChartType[2];
						lChartStyle = 2;
						lShape = 2;
						break;
					case 2:// bar style
						strChartType = g_strChartType[2];
						lChartStyle = 2;
						lShape = 0;
						lThick = 1;
						break;
					default://  line style
						strChartType = g_strChartType[1];
						lChartStyle = 0;
						break;
					}

#ifdef _DEBUG
{
	POSITION pos = m_listPacket.Find(strDataItemName);
	ASSERT(!pos);
	pos = m_listGraph.Find(strDataItemName);
	ASSERT(!pos);
}
#endif
					COLORREF rgbLine[] = {RGB(0,0,0), RGB(255,0,0), RGB(0,128,0), RGB(0,0,255), RGB(128,128,0), RGB(0,128,128), RGB(128,0,128) };
					m_pIChartOCX->AddPacket(MakePacketOption(strDataItemName,8,"× 1",FALSE));
					m_listPacket.AddTail(strDataItemName);
					arrayPacket.RemoveAll();
					arrayPacket.Add(strDataItemName);
					m_listGraph.AddTail(strDataItemName);
					m_pIChartOCX->AddGraph(1,0,0
						,MakeGraphOption(strChartType
										,strDataItemName
										,arrayPacket
										,strDataItemName
										,lChartStyle
										,lShape
										, clrIndex
										, clrIndex
										, clrIndex
										, clrIndex
										, clrIndex
										, clrIndex,
										1,1,lThick,lThick,0)
						,bFirstIndexGraph
						,bFirstIndexGraph
						,TRUE);

					bFirstIndexGraph = FALSE;
				}
				pIPacketDataItem	= m_pIPacketManager->GetPacket(m_strRQ, strDataItemName);	// 사용되고 있는 RQ :복수종목 - ojtaso (20070307)
			
				CopyPlotDataToPacket(pIPacketDataItem, pDataItem,strDataItemName, nTotalDataCnt, pInfo->bIsInRealTime);
			}
			else if(strDataItemName8=="Bullbear")
			{
				pDataItemBullBearArea = pDataItem;
			}
		}

		if(pDealDataItem)// set deal data....
		{
			CDealData *pDealData = NULL;
			if(!pInfo->bIsInRealTime)
			{
				m_arrayDealPosition.RemoveAll();
				m_arrayDealName.RemoveAll();
				m_arrayDealDrawPosition.RemoveAll();
				LoadEnvDealPos(pDealDataItem);
				if(m_brushSell.GetSafeHandle())			m_brushSell.DeleteObject();
				if(m_brushBuy.GetSafeHandle())			m_brushBuy.DeleteObject();
				if(m_brushExitLong.GetSafeHandle())		m_brushExitLong.DeleteObject();
				if(m_brushExitShort.GetSafeHandle())	m_brushExitShort.DeleteObject();
				m_brushSell.CreateSolidBrush(m_clrSell);
				m_brushBuy.CreateSolidBrush(m_clrBuy);
				m_brushExitLong.CreateSolidBrush(m_clrExitLong);
				m_brushExitShort.CreateSolidBrush(m_clrExitShort);
				m_arrayDealPosition.SetSize(nTotalDataCnt);
				m_arrayDealName.SetSize(nTotalDataCnt);
				m_arrayDealDrawPosition.SetSize(nTotalDataCnt);

				if(m_rgnSell.GetSafeHandle())		m_rgnSell.DeleteObject();
				if(m_rgnBuy.GetSafeHandle())		m_rgnBuy.DeleteObject();
				if(m_rgnExitLong.GetSafeHandle())	m_rgnExitLong.DeleteObject();
				if(m_rgnExitShort.GetSafeHandle())	m_rgnExitShort.DeleteObject();
				
				m_lHeightBuy = MakeDealShape(10,m_rgnBuy,m_lShapeSignalBuy,1);
				m_lHeightExitShort = MakeDealShape(10,m_rgnExitShort,m_lShapeSignalExitShort,1);
				m_lHeightSell = MakeDealShape(10,m_rgnSell,m_lShapeSignalSell,0);		
				m_lHeightExitLong = MakeDealShape(10,m_rgnExitLong,m_lShapeSignalExitLong,0);		
				
				POSITION pos = pDealDataItem->GetHeadPosition();
				while(pos)
				{
					pDealData = pDealDataItem->GetNext(pos);					
					if(pDealData->m_lDealValue&VALBUY)		// Buy & ExitShort Position
					{
						m_arrayDealDrawPosition.SetAt(pDealData->m_nIndex
									,m_DataItemLow.GetAt(pDealData->m_nIndex));	
					}
					else if(pDealData->m_lDealValue&VALSELL)	// Sell & ExitLong Position
					{
						m_arrayDealDrawPosition.SetAt(pDealData->m_nIndex,
									m_DataItemHigh.GetAt(pDealData->m_nIndex));
					}
					else
					{
						m_arrayDealDrawPosition.SetAt(pDealData->m_nIndex,0);	
					}			
					m_arrayDealPosition.SetAt(pDealData->m_nIndex,pDealData->m_lDealValue);				
					m_arrayDealName.SetAt(pDealData->m_nIndex,pDealData->m_strDealName);
				}
			}
			else
			{
				m_arrayDealPosition.SetSize(nTotalDataCnt);
				m_arrayDealName.SetSize(nTotalDataCnt);
				m_arrayDealDrawPosition.SetSize(nTotalDataCnt);
				int nIndex = nTotalDataCnt - 1;
				if(pDealDataItem->GetCount())
				{
					pDealData = pDealDataItem->GetTail();				
					if(pDealData->m_nIndex>=(nIndex-1))
					{
						if(pDealData->m_lDealValue&VALBUY)		// Set Buy & ExitShort Position
						{
							m_arrayDealDrawPosition.SetAt(pDealData->m_nIndex
											,m_DataItemLow.GetAt(pDealData->m_nIndex));	
						}
						else if(pDealData->m_lDealValue&VALSELL)	// Set Sell & ExitLong Position
						{
							m_arrayDealDrawPosition.SetAt(pDealData->m_nIndex
											,m_DataItemHigh.GetAt(pDealData->m_nIndex));				
						}
						else
						{
							m_arrayDealDrawPosition.SetAt(pDealData->m_nIndex,0);	
						}			
						m_arrayDealPosition.SetAt(pDealData->m_nIndex,pDealData->m_lDealValue);
						m_arrayDealName.SetAt(pDealData->m_nIndex,pDealData->m_strDealName);
					}			
				}
			}
		}
		if(pDataItemBullBearArea)// Set BullBearData
		{
			int nIndex = 0;
			double dValue = 0;
			double dHigh = 0;
			double dLow = 0;

			if(!pInfo->bIsInRealTime)
			{
				m_arrayBullBearPosition.RemoveAll();
				LoadEnvBullBear(pDataItemBullBearArea);
				m_arrayBullBearPosition.SetSize(nTotalDataCnt);
				for(nIndex=0;nIndex<nTotalDataCnt;nIndex++)
				{
					pDataItemBullBearArea->GetAt(nIndex,&dValue);
					m_arrayBullBearPosition.SetAt(nIndex,dValue);
				}
			}
			else
			{
				m_arrayBullBearPosition.SetSize(nTotalDataCnt);		
				nIndex = nTotalDataCnt - 1;			
				pDataItemBullBearArea->GetAt(nIndex,&dValue);
				m_arrayBullBearPosition.SetAt(nIndex,dValue);
			}

		}
	}
	else if(pIndexItem->m_indexType==STCFG)
	{	
		
	}
	else
	{
		// ST 파일백업처리
		STBackup(m_szMSTName);

		// 이미 실행중인 전략이 존재
		if(m_pStrategyItem)
		{
			// 전략 실행중에 발생한 신호정보가 있을 경우
			CDealDataItem* pDealDataItem = (CDealDataItem*)pInfo->pDealDataItem;
			if(pDealDataItem)
			{
				CDealData* pEntryDealData	= NULL;
				CDealData* pExitDealData	= NULL;
				CDealData* pDealData_ForReal = NULL;

				//-------- Draw하기 위해 신호정보 셋팅 (조회) --------
				if(!pInfo->bIsInRealTime)
				{
					//-------- 전략신호정보 초기화 -------- 
					InitStrategySignalInfo(pDealDataItem);
					
					m_arrayDealPosition.Copy(*pInfo->pArDealPosition);
					m_arrayDealName.Copy(*pInfo->pArDealName);
					m_arrayDealDrawPosition.Copy(*pInfo->pArDealDrawPosition);

					// 마지막 주문신호 인덱스 셋팅
					m_nOrderIndex = pDealDataItem->GetLastIndex();
// 2008.04.04 by LYH >>
					m_nOrderType = 0;
// 2008.04.04 by LYH <<
					m_pIChartOCX->InvalidateControl();
// test order
//					SetTimer(1111, 1000, NULL);
				}
				//-------- Draw하기 위해 신호정보 셋팅 (실시간) --------
				else
				{
					int nTotalDataCnt = m_SDataItemDate.GetSize();
					m_arrayDealPosition.SetSize(nTotalDataCnt);
					m_arrayDealName.SetSize(nTotalDataCnt);
					m_arrayDealDrawPosition.SetSize(nTotalDataCnt);

					if(!m_StDataIndex.GetSize() == 0)//분할 정보일때...
					{
						m_arrayDealPosition.Copy(m_arrayDivideDealPosition);
						m_arrayDealDrawPosition.Copy(m_arrayDivideDealDrawPosition);
						m_arrayDealName.Copy(m_arrayDivideDealName);
						int nTotalDivideData = m_arrayDivideDealDrawPosition.GetSize();
						for(int i = nTotalDivideData ; i < nTotalDataCnt; i++)//뒤의 것만 저장한다.
						{
								if(pInfo->pArDealDrawPosition->GetSize() > i && m_arrayDealDrawPosition.GetSize() > i)
								{
									m_arrayDealDrawPosition.SetAt(i, pInfo->pArDealDrawPosition->GetAt(i));
									m_arrayDealPosition.SetAt(i, pInfo->pArDealPosition->GetAt(i));
									m_arrayDealName.SetAt(i, pInfo->pArDealName->GetAt(i));
								}else break;
						}
					}
					else
					{
						m_arrayDealPosition.Copy(*pInfo->pArDealPosition);
						m_arrayDealName.Copy(*pInfo->pArDealName);
						m_arrayDealDrawPosition.Copy(*pInfo->pArDealDrawPosition);
					}
					m_pIChartOCX->InvalidateControl();

					if(pDealDataItem->GetCount())
					{
						pDealData_ForReal = pDealDataItem->GetTail();
						int nLastIndex = nTotalDataCnt - 1;
						if(pDealData_ForReal->m_nIndex == (nLastIndex - m_nTiming_Order) && 
// 2008.04.04 by LYH >>
//						   pDealData_ForReal->m_nIndex > m_nOrderIndex)
						     ((pDealData_ForReal->m_nIndex > m_nOrderIndex) || IsCheckPosition(pDealData_ForReal)))
// 2008.04.04 by LYH <<
						{
// 2008.04.04 by LYH >>
							m_nOrderType = 0;
// 2008.04.04 by LYH <<
							// vntsorl
							// 강제청산
							if(IsForcedLiquidation(pDealData_ForReal->m_lDealValue))
							{
								if(IsBuyForcedLiquidation(pDealData_ForReal->m_lDealValue) && (m_nApplyPosition == 0 || m_nApplyPosition == 2))
								{
									m_pstOrderData->nOrderType = FORCEDEXIT | pDealData_ForReal->m_lDealValue;
									m_pstOrderData->nOrderPriceTypeClr = m_nChungValue;	// 청산가격

									// 주문신호
//									((CSiteCustomAddin*)m_pStieCustomAddin)->InvokeAddIn(20, (LONG)m_pstOrderData);//주문신호를 낸다.
									OrderSign(m_pstOrderData);
									m_nOrderIndex = (nLastIndex - m_nTiming_Order);
								}
								if(m_pstOrderData->nMarket == 1)	// 선물
								{
									if(IsSellForcedLiquidation(pDealData_ForReal->m_lDealValue) && (m_nApplyPosition == 1 || m_nApplyPosition == 2))
									{
										m_pstOrderData->nOrderType = FORCEDEXIT | pDealData_ForReal->m_lDealValue;
										m_pstOrderData->nOrderPriceTypeClr = m_nChungValue;	// 청산가격

										// 주문신호
//										((CSiteCustomAddin*)m_pStieCustomAddin)->InvokeAddIn(20, (LONG)m_pstOrderData);//주문신호를 낸다.
										OrderSign(m_pstOrderData);
										m_nOrderIndex = (nLastIndex - m_nTiming_Order);
									}
								}
							}
//							else
							{
								// 매도
								if(pDealData_ForReal->m_lDealValue & VALENTRYSELL)
								{
									if(m_pstOrderData->nMarket == 1)	// 선물
									{
										// 매도/매도청산 허용 or 모두 허용
										if(m_nApplyPosition == 1 || m_nApplyPosition == 2)
										{
											m_pstOrderData->nOrderType = VALENTRYSELL;
											m_pstOrderData->nOrderPriceTypeEnt = m_nJinValue;		// 진입가격

											// 주문신호
//											((CSiteCustomAddin*)m_pStieCustomAddin)->InvokeAddIn(20, (LONG)m_pstOrderData);
											OrderSign(m_pstOrderData);
											m_nOrderIndex = (nLastIndex - m_nTiming_Order);
// 2008.04.04 by LYH >>
											m_nOrderType |= VALENTRYSELL;
// 2008.04.04 by LYH <<
										}
									}
								}
								// 매도청산
								else if(pDealData_ForReal->m_lDealValue & VALEXITSHORT)
								{
									if(m_pstOrderData->nMarket == 1)	// 선물
									{
										// 매도/매도청산 허용 or 모두 허용
										if(m_nApplyPosition == 1 || m_nApplyPosition == 2)
										{
											m_pstOrderData->nOrderType = VALEXITSHORT;
											m_pstOrderData->nOrderPriceTypeClr = m_nChungValue;	// 청산가격

											// 주문신호
//											((CSiteCustomAddin*)m_pStieCustomAddin)->InvokeAddIn(20, (LONG)m_pstOrderData);
											OrderSign(m_pstOrderData);
											m_nOrderIndex = (nLastIndex - m_nTiming_Order);
// 2008.04.04 by LYH >>
											m_nOrderType |= VALEXITSHORT;
// 2008.04.04 by LYH <<
										}
									}
								}
								// 매수
								if(pDealData_ForReal->m_lDealValue & VALENTRYBUY)
								{
									if(m_pstOrderData->nMarket == 0)		// 주식
									{
										m_pstOrderData->nOrderType = VALENTRYBUY;
										m_pstOrderData->nOrderPriceTypeEnt = m_nJinValue;		// 진입가격

										// 주문신호
//										((CSiteCustomAddin*)m_pStieCustomAddin)->InvokeAddIn(20, (LONG)m_pstOrderData);
										OrderSign(m_pstOrderData);
										m_nOrderIndex = (nLastIndex - m_nTiming_Order);
// 2008.04.04 by LYH >>
										m_nOrderType |= VALENTRYBUY;
// 2008.04.04 by LYH <<
									}
									else if(m_pstOrderData->nMarket == 1)	// 선물
									{
										// 매수/매수청산 허용 or 모두 허용
										if(m_nApplyPosition == 0 || m_nApplyPosition == 2)
										{
											m_pstOrderData->nOrderType = VALENTRYBUY;
											m_pstOrderData->nOrderPriceTypeEnt = m_nJinValue;		// 진입가격

											// 주문신호
//											((CSiteCustomAddin*)m_pStieCustomAddin)->InvokeAddIn(20, (LONG)m_pstOrderData);
											OrderSign(m_pstOrderData);
											m_nOrderIndex = (nLastIndex - m_nTiming_Order);
// 2008.04.04 by LYH >>
											m_nOrderType |= VALENTRYBUY;
// 2008.04.04 by LYH <<
										}
									}
								}
								// 매수청산
								else if(pDealData_ForReal->m_lDealValue & VALEXITLONG)
								{
									if(m_pstOrderData->nMarket == 0)		// 주식
									{
										m_pstOrderData->nOrderType = VALEXITLONG;
										m_pstOrderData->nOrderPriceTypeClr = m_nChungValue;		// 청산가격

										// 주문신호
//										((CSiteCustomAddin*)m_pStieCustomAddin)->InvokeAddIn(20, (LONG)m_pstOrderData);
										OrderSign(m_pstOrderData);
										m_nOrderIndex = (nLastIndex - m_nTiming_Order);
// 2008.04.04 by LYH >>
										m_nOrderType |= VALEXITLONG;
// 2008.04.04 by LYH <<
									}
									else if(m_pstOrderData->nMarket == 1)	// 선물
									{
										// 매수/매수청산 허용 or 모두 허용
										if(m_nApplyPosition == 0 || m_nApplyPosition == 2)
										{
											m_pstOrderData->nOrderType = VALEXITLONG;
											m_pstOrderData->nOrderPriceTypeClr = m_nChungValue;	// 청산가격

											// 주문신호
//											((CSiteCustomAddin*)m_pStieCustomAddin)->InvokeAddIn(20, (LONG)m_pstOrderData);
											OrderSign(m_pstOrderData);
											m_nOrderIndex = (nLastIndex - m_nTiming_Order);
// 2008.04.04 by LYH >>
											m_nOrderType |= VALEXITLONG;
// 2008.04.04 by LYH <<
										}
									}
								}
							}
							m_pIChartOCX->InvalidateControl();
						}
					}
				}
			}
		}
	}

	return 0;
}

void CWndAssistIndexManager::InitStrategySignalInfo(CDealDataItem* pDealDataItem)
{
	int nTotalDataCnt	= m_SDataItemDate.GetSize();

	m_arrayDealPosition.RemoveAll();
	m_arrayDealName.RemoveAll();
	m_arrayDealDrawPosition.RemoveAll();

	m_arrayDealPosition.SetSize(nTotalDataCnt);
	m_arrayDealName.SetSize(nTotalDataCnt);
	m_arrayDealDrawPosition.SetSize(nTotalDataCnt);

	// 신호모양 및 색상을 위한 초기화
	LoadEnvDealPos(pDealDataItem);

	if(m_brushSell.GetSafeHandle())				m_brushSell.DeleteObject();
	if(m_brushBuy.GetSafeHandle())				m_brushBuy.DeleteObject();
	if(m_brushExitLong.GetSafeHandle())		m_brushExitLong.DeleteObject();
	if(m_brushExitShort.GetSafeHandle())		m_brushExitShort.DeleteObject();

	m_brushSell.CreateSolidBrush(m_clrSell);
	m_brushBuy.CreateSolidBrush(m_clrBuy);
	m_brushExitLong.CreateSolidBrush(m_clrExitLong);
	m_brushExitShort.CreateSolidBrush(m_clrExitShort);

	if(m_rgnSell.GetSafeHandle())		m_rgnSell.DeleteObject();
	if(m_rgnBuy.GetSafeHandle())		m_rgnBuy.DeleteObject();
	if(m_rgnExitLong.GetSafeHandle())	m_rgnExitLong.DeleteObject();
	if(m_rgnExitShort.GetSafeHandle())	m_rgnExitShort.DeleteObject();

	m_lHeightBuy = MakeDealShape(10, m_rgnBuy, m_lShapeSignalBuy, 1);
	m_lHeightExitShort = MakeDealShape(10, m_rgnExitShort, m_lShapeSignalExitShort, 1);
	m_lHeightSell = MakeDealShape(10, m_rgnSell, m_lShapeSignalSell, 0);
	m_lHeightExitLong = MakeDealShape(10, m_rgnExitLong, m_lShapeSignalExitLong, 0);
}

// 진입 포지션
BOOL CWndAssistIndexManager::IsEntryPosition(long lDealType)
{
	return (IsBuy(lDealType) || IsSell(lDealType));
}

// 청산 포지션
BOOL CWndAssistIndexManager::IsExitPosition(long lDealType)
{
	return (IsExitLong(lDealType) || IsExitShort(lDealType));
}

// 강제청산 포지션
BOOL CWndAssistIndexManager::IsForcedLiquidation(long lDealType)
{
	if( lDealType & FORCEDEXIT )
		return TRUE;

	return FALSE;
}

BOOL CWndAssistIndexManager::IsBuyForcedLiquidation(long lDealType)
{
	if( lDealType & FL_BUY_LOSSRATE				||
		lDealType & FL_BUY_TARGETBENEFIT		||
		lDealType & FL_BUY_HPRICECPBEAR			||
		lDealType & FL_BUY_TARGETVALUEPRESERVE	||
		lDealType & FL_BUY_BENEFITPRESERVE		||
		lDealType & FL_BUY_MINIPRICE			||
		lDealType & FL_BUY_DAYLIQUIDATION )
		return TRUE;

	return FALSE;
}

BOOL CWndAssistIndexManager::IsSellForcedLiquidation(long lDealType)
{
	if( lDealType & FL_SELL_LOSSRATE			||
		lDealType & FL_SELL_TARGETBENEFIT		||
		lDealType & FL_SELL_HPRICECPBEAR		||
		lDealType & FL_SELL_TARGETVALUEPRESERVE	||
		lDealType & FL_SELL_BENEFITPRESERVE		||
		lDealType & FL_SELL_MINIPRICE			||
		lDealType & FL_SELL_DAYLIQUIDATION )
		return TRUE;

	return FALSE;
}

BOOL CWndAssistIndexManager::IsBuyLossRate(long lDealType)
{
	return lDealType & FL_BUY_LOSSRATE;
}

BOOL CWndAssistIndexManager::IsBuyTargetBenefit(long lDealType)
{
	return lDealType & FL_BUY_TARGETBENEFIT;
}

BOOL CWndAssistIndexManager::IsBuyHPriceCPBear(long lDealType)
{
	return lDealType & FL_BUY_HPRICECPBEAR;
}

BOOL CWndAssistIndexManager::IsBuyTargetValuePreserve(long lDealType)
{
	return lDealType & FL_BUY_TARGETVALUEPRESERVE;
}

BOOL CWndAssistIndexManager::IsBuyBenefitPreserve(long lDealType)
{
	return lDealType & FL_BUY_BENEFITPRESERVE;
}

BOOL CWndAssistIndexManager::IsBuyMiniPrice(long lDealType)
{
	return lDealType & FL_BUY_MINIPRICE;
}

BOOL CWndAssistIndexManager::IsBuyDayLiquidation(long lDealType)
{
	return lDealType & FL_BUY_DAYLIQUIDATION;
}

BOOL CWndAssistIndexManager::IsSellLossRate(long lDealType)
{
	return lDealType & FL_SELL_LOSSRATE;
}

BOOL CWndAssistIndexManager::IsSellTargetBenefit(long lDealType)
{
	return lDealType & FL_SELL_TARGETBENEFIT;
}

BOOL CWndAssistIndexManager::IsSellHPriceCPBear(long lDealType)
{
	return lDealType & FL_SELL_HPRICECPBEAR;
}

BOOL CWndAssistIndexManager::IsSellTargetValuePreserve(long lDealType)
{
	return lDealType & FL_SELL_TARGETVALUEPRESERVE;
}

BOOL CWndAssistIndexManager::IsSellBenefitPreserve(long lDealType)
{
	return lDealType & FL_SELL_BENEFITPRESERVE;
}

BOOL CWndAssistIndexManager::IsSellMiniPrice(long lDealType)
{
	return lDealType & FL_SELL_MINIPRICE;
}	

BOOL CWndAssistIndexManager::IsSellDayLiquidation(long lDealType)
{
	return lDealType & FL_SELL_DAYLIQUIDATION;
}

// 매수 사용여부
BOOL CWndAssistIndexManager::IsBuy(long lDealType)
{
	return lDealType & VALENTRYBUY;
}

// 매도청산 사용여부
BOOL CWndAssistIndexManager::IsExitShort(long lDealType)
{
	return lDealType & VALEXITSHORT;
}

// 매도 사용여부
BOOL CWndAssistIndexManager::IsSell(long lDealType)
{
	return lDealType & VALENTRYSELL;
}

// 매수청산 사용여부
BOOL CWndAssistIndexManager::IsExitLong(long lDealType)
{
	return lDealType & VALEXITLONG;
}

// 매수 / 매도청산 사용여부
BOOL CWndAssistIndexManager::IsBuyOrExitShort(long lDealType)
{
	return IsBuy(lDealType) || IsExitShort(lDealType);
}

// 매도 / 매수청산 사용여부
BOOL CWndAssistIndexManager::IsSellOrExitLong(long lDealType)
{
	return IsSell(lDealType) || IsExitLong(lDealType);
}

BOOL CWndAssistIndexManager::IsLossRate(long lDealType)
{
	return lDealType == VALSONJUL;
}

BOOL CWndAssistIndexManager::IsTargetBenefit(long lDealType)
{
	return lDealType == VALSOOIK;
}

BOOL CWndAssistIndexManager::IsHPriceCPBear(long lDealType)
{
	return lDealType == VALGOGA;
}

//BOOL CWndAssistIndexManager::IsLPriceCPBull(long lDealType)
//{
//	return lDealType == VALJUGA;
//}

BOOL CWndAssistIndexManager::IsTargetValuePreserve(long lDealType)
{
	return lDealType == VALTARGETPRESERV;
}

BOOL CWndAssistIndexManager::IsBenefitPreserve(long lDealType)
{
	return lDealType == VALBENEFITPRESERV;
}

BOOL CWndAssistIndexManager::IsMiniPrice(long lDealType)
{
	return lDealType == VALMIN;
}

BOOL CWndAssistIndexManager::IsDayLiquidation(long lDealType)
{
	return lDealType == VALDANGIL;
}

void CWndAssistIndexManager::DrawSignal_EntryPosition(CDC* pDC, long lDealType, int nXPos, int nYPos, CString strDealName)
{
	CRgn rgn;
	CRgn rgnCanvas;
	CRgn rgnSignal;
	rgn.CreateRectRgn(0, 0, 0, 0);
	rgnCanvas.CreateRectRgnIndirect(m_rctBlockRegion);
	rgnSignal.CreateRectRgn(0, 0, 0, 0);

	CSize szDealName;
	if(!strDealName.IsEmpty())
		szDealName = pDC->GetOutputTextExtent(strDealName);

	// 주식
	if(m_STConfigInfo.m_nCodeType == 0)	// 0:주식, 1:선옵, 2:업종
	{
		rgn.CopyRgn(&m_rgnBuy);
		rgn.OffsetRgn(nXPos, nYPos + 10);
		rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

		// 매수 신호 Draw
		if(m_lShapeSignalBuy == 2)				// Brush로 신호 Shape의 Border만 Draw
			pDC->FrameRgn(&rgnSignal, &m_brushBuy, 1, 1);
		else									// Brush로 신호 Shape를 채움
			pDC->FillRgn(&rgnSignal, &m_brushBuy);						
	}
	// 선물
	else if(m_STConfigInfo.m_nCodeType == 1)
	{
		// 매수/매수청산 허용 or 모두 허용
		if(m_nApplyPosition == 0 || m_nApplyPosition == 2)
		{
			rgn.CopyRgn(&m_rgnBuy);
			rgn.OffsetRgn(nXPos, nYPos + 10);
			rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

			// 매수 신호 Draw
			if(m_lShapeSignalBuy == 2)			// Brush로 신호 Shape의 Border만 Draw
				pDC->FrameRgn(&rgnSignal, &m_brushBuy, 1, 1);
			else								// Brush로 신호 Shape를 채움
				pDC->FillRgn(&rgnSignal, &m_brushBuy);
		}

		// 매도/매도청산 허용 or 모두 허용
		if(m_nApplyPosition == 1 || m_nApplyPosition == 2)
		{
			rgn.CopyRgn(&m_rgnSell);
			rgn.OffsetRgn(nXPos, nYPos - 20);	// 매수청산 신호 위에 표시함
			rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

			// 매도청산 신호 Draw
			if(m_lShapeSignalSell == 2)			// Brush로 신호 Shape의 Border만 Draw
				pDC->FrameRgn(&rgnSignal, &m_brushSell, 1, 1);
			else								// Brush로 신호 Shape를 채움
				pDC->FillRgn(&rgnSignal, &m_brushSell);						
		}
	}

	pDC->SetBkColor(RGB(255,255,255));
	if(m_rctBlockRegion.PtInRect(CPoint(nXPos - szDealName.cx/2, nYPos + m_lHeightSell - szDealName.cy - 2)) && m_bShowName)
		pDC->TextOut(nXPos - szDealName.cx/2, nYPos + m_lHeightSell - szDealName.cy - 2, strDealName);
}

void CWndAssistIndexManager::DrawSignal_ExitPosition(CDC* pDC, long lDealType, int nXPos, int nYPos, CString strDealName)
{
	CRgn rgn;
	CRgn rgnCanvas;
	CRgn rgnSignal;
	rgn.CreateRectRgn(0, 0, 0, 0);
	rgnCanvas.CreateRectRgnIndirect(m_rctBlockRegion);
	rgnSignal.CreateRectRgn(0, 0, 0, 0);

	CSize szDealName;
	if(!strDealName.IsEmpty())
		szDealName = pDC->GetOutputTextExtent(strDealName);

	// 주식
	if(m_STConfigInfo.m_nCodeType == 0)	// 0:주식, 1:선옵, 2:업종
	{
		rgn.CopyRgn(&m_rgnExitLong);
		rgn.OffsetRgn(nXPos, nYPos - 10);
		rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

		// 매수 신호 Draw
		if(m_lShapeSignalExitLong == 2)			// Brush로 신호 Shape의 Border만 Draw
			pDC->FrameRgn(&rgnSignal, &m_brushExitLong, 1, 1);
		else									// Brush로 신호 Shape를 채움
			pDC->FillRgn(&rgnSignal, &m_brushExitLong);						
	}
	// 선물
	else if(m_STConfigInfo.m_nCodeType == 1)
	{
		// 매수/매수청산 허용 or 모두 허용
		if(m_nApplyPosition == 0 || m_nApplyPosition == 2)
		{
			rgn.CopyRgn(&m_rgnExitLong);
			rgn.OffsetRgn(nXPos, nYPos - 10);
			rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

			// 매수 신호 Draw
			if(m_lShapeSignalExitLong == 2)		// Brush로 신호 Shape의 Border만 Draw
				pDC->FrameRgn(&rgnSignal, &m_brushExitLong, 1, 1);
			else								// Brush로 신호 Shape를 채움
				pDC->FillRgn(&rgnSignal, &m_brushExitLong);
		}

		// 매도/매도청산 허용 or 모두 허용
		if(m_nApplyPosition == 1 || m_nApplyPosition == 2)
		{
			rgn.CopyRgn(&m_rgnExitShort);
			rgn.OffsetRgn(nXPos, nYPos + 20);	// 매수청산 신호 위에 표시함
			rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

			// 매도청산 신호 Draw
			if(m_lShapeSignalExitShort == 2)	// Brush로 신호 Shape의 Border만 Draw
				pDC->FrameRgn(&rgnSignal, &m_brushExitShort, 1, 1);
			else								// Brush로 신호 Shape를 채움
				pDC->FillRgn(&rgnSignal, &m_brushExitShort);						
		}
	}

	pDC->SetBkColor(RGB(255,255,255));
	if(m_rctBlockRegion.PtInRect(CPoint(nXPos - szDealName.cx/2, nYPos + m_lHeightSell - szDealName.cy - 2)) && m_bShowName)
		pDC->TextOut(nXPos - szDealName.cx/2, nYPos + m_lHeightSell - szDealName.cy - 2, strDealName);
}

void CWndAssistIndexManager::DrawSignal_Buy(CDC* pDC, int nXPos, int nYPos, CString strDealName)
{
	CRgn rgn;
	CRgn rgnCanvas;
	CRgn rgnSignal;
	rgn.CreateRectRgn(0, 0, 0, 0);
	rgnCanvas.CreateRectRgnIndirect(m_rctBlockRegion);
	rgnSignal.CreateRectRgn(0, 0, 0, 0);

	CSize szDealName;
	if(!strDealName.IsEmpty())
		szDealName = pDC->GetOutputTextExtent(strDealName);

	// 주식
	if(m_STConfigInfo.m_nCodeType == 0)	// 0:주식, 1:선옵, 2:업종
	{
		rgn.CopyRgn(&m_rgnBuy);
		rgn.OffsetRgn(nXPos, nYPos);
		rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

		// 매수 신호 Draw
		if(m_lShapeSignalBuy == 2)			// Brush로 신호 Shape의 Border만 Draw
			pDC->FrameRgn(&rgnSignal, &m_brushBuy, 1, 1);
		else									// Brush로 신호 Shape를 채움
			pDC->FillRgn(&rgnSignal, &m_brushBuy);						
	}
	// 선물
	else
	{
		// 매수/매수청산 허용 or 모두 허용
		if(m_nApplyPosition == 0 || m_nApplyPosition == 2)
		{
			rgn.CopyRgn(&m_rgnBuy);
			rgn.OffsetRgn(nXPos, nYPos);
			rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

			// 매수 신호 Draw
			if(m_lShapeSignalBuy == 2)			// Brush로 신호 Shape의 Border만 Draw
				pDC->FrameRgn(&rgnSignal, &m_brushBuy, 1, 1);
			else								// Brush로 신호 Shape를 채움
				pDC->FillRgn(&rgnSignal, &m_brushBuy);
		}
	}

	pDC->SetBkColor(RGB(255,255,255));
	if(m_rctBlockRegion.PtInRect(CPoint(nXPos - szDealName.cx/2, nYPos + m_lHeightSell - szDealName.cy - 2)) && m_bShowName)
		pDC->TextOut(nXPos - szDealName.cx/2, nYPos + m_lHeightSell - szDealName.cy - 2, strDealName);
}

void CWndAssistIndexManager::DrawSignal_ExitShort(CDC* pDC, int nXPos, int nYPos, CString strDealName)
{
	CRgn rgn;
	CRgn rgnCanvas;
	CRgn rgnSignal;
	rgn.CreateRectRgn(0, 0, 0, 0);
	rgnCanvas.CreateRectRgnIndirect(m_rctBlockRegion);
	rgnSignal.CreateRectRgn(0, 0, 0, 0);

	CSize szDealName;
	if(!strDealName.IsEmpty())
		szDealName = pDC->GetOutputTextExtent(strDealName);

	// 선물
	if(m_STConfigInfo.m_nCodeType == 1)	// 0:주식, 1:선옵, 2:업종
	{
		// 매도/매도청산 허용 or 모두 허용
		if(m_nApplyPosition == 1 || m_nApplyPosition == 2)
		{
			rgn.CopyRgn(&m_rgnExitShort);
			rgn.OffsetRgn(nXPos, nYPos + 10);	// 매수청산 신호 위에 표시함
			//rgn.OffsetRgn(nXPos, nYPos - 5);	// 매수청산 신호 위에 표시함
			rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

			// 매도청산 신호 Draw
			if(m_lShapeSignalExitShort==2)		// Brush로 신호 Shape의 Border만 Draw
				pDC->FrameRgn(&rgnSignal, &m_brushExitShort, 1, 1);
			else								// Brush로 신호 Shape를 채움
				pDC->FillRgn(&rgnSignal, &m_brushExitShort);						
		}
	}

	pDC->SetBkColor(RGB(255,255,255));
	if(m_rctBlockRegion.PtInRect(CPoint(nXPos - szDealName.cx/2, nYPos + m_lHeightSell - szDealName.cy - 2)) && m_bShowName)
		pDC->TextOut(nXPos - szDealName.cx/2, nYPos + m_lHeightSell - szDealName.cy - 2, strDealName);
}

void CWndAssistIndexManager::DrawSignal_Sell(CDC* pDC, int nXPos, int nYPos, CString strDealName)
{
	CRgn rgn;
	CRgn rgnCanvas;
	CRgn rgnSignal;
	rgn.CreateRectRgn(0, 0, 0, 0);
	rgnCanvas.CreateRectRgnIndirect(m_rctBlockRegion);
	rgnSignal.CreateRectRgn(0, 0, 0, 0);

	CSize szDealName;
	if(!strDealName.IsEmpty())
		szDealName = pDC->GetOutputTextExtent(strDealName);

	// 선물
	if(m_STConfigInfo.m_nCodeType == 1)	// 0:주식, 1:선옵, 2:업종
	{				
		// 매도/매도청산 허용 or 모두 허용
		if(m_nApplyPosition == 1 || m_nApplyPosition == 2)
		{
			rgn.CopyRgn(&m_rgnSell);
			rgn.OffsetRgn(nXPos, nYPos - 10);	// 매수 신호 아래에 표시함
			rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);
	
			// 매도 신호 Draw
			if(m_lShapeSignalSell == 2)			// Brush로 신호 Shape의 Border만 Draw
				pDC->FrameRgn(&rgnSignal, &m_brushSell, 1, 1);
			else								// Brush로 신호 Shape를 채움
				pDC->FillRgn(&rgnSignal, &m_brushSell);						
		}
	}
	pDC->SetBkColor(RGB(255,255,255));
	if(m_rctBlockRegion.PtInRect(CPoint(nXPos - szDealName.cx/2, nYPos + m_lHeightBuy + szDealName.cy + 2)) && m_bShowName)
		pDC->TextOut(nXPos - szDealName.cx/2, nYPos + m_lHeightBuy + 2, strDealName);
}

void CWndAssistIndexManager::DrawSignal_ExitLong(CDC* pDC, int nXPos, int nYPos, CString strDealName)
{
	CRgn rgn;
	CRgn rgnCanvas;
	CRgn rgnSignal;
	rgn.CreateRectRgn(0, 0, 0, 0);
	rgnCanvas.CreateRectRgnIndirect(m_rctBlockRegion);
	rgnSignal.CreateRectRgn(0, 0, 0, 0);

	CSize szDealName;
	if(!strDealName.IsEmpty())
		szDealName = pDC->GetOutputTextExtent(strDealName);

	// 주식
	if(m_STConfigInfo.m_nCodeType == 0)	// 0:주식, 1:선옵, 2:업종
	{
		rgn.CopyRgn(&m_rgnExitLong);
		rgn.OffsetRgn(nXPos, nYPos);
		rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

		// 매수청산 신호 Draw
		if(m_lShapeSignalExitLong == 2)			// Brush로 신호 Shape의 Border만 Draw
			pDC->FrameRgn(&rgnSignal, &m_brushExitLong, 1, 1);
		else									// Brush로 신호 Shape를 채움
			pDC->FillRgn(&rgnSignal, &m_brushExitLong);
	}
	// 선물
	else
	{				
		// 매수/매수청산 허용 or 모두 허용
		if(m_nApplyPosition == 0 || m_nApplyPosition == 2)
		{
			rgn.CopyRgn(&m_rgnExitLong);
			rgn.OffsetRgn(nXPos, nYPos);
			rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

			// 매수청산 신호 Draw
			if(m_lShapeSignalExitLong == 2)		// Brush로 신호 Shape의 Border만 Draw
				pDC->FrameRgn(&rgnSignal, &m_brushExitLong, 1, 1);
			else								// Brush로 신호 Shape를 채움
				pDC->FillRgn(&rgnSignal, &m_brushExitLong);
		}
	}
	pDC->SetBkColor(RGB(255,255,255));
	if(m_rctBlockRegion.PtInRect(CPoint(nXPos - szDealName.cx/2, nYPos + m_lHeightBuy + szDealName.cy + 2)) && m_bShowName)
		pDC->TextOut(nXPos - szDealName.cx/2, nYPos + m_lHeightBuy + 2, strDealName);
}

// 강제청산 신호 그리기
void CWndAssistIndexManager::DrawSignal_BuyForcedLiquidation(CDC* pDC, long lDealType, int nXPos, int nYPos)
{
	// 주식
	if(m_STConfigInfo.m_nCodeType == 0)	// 0:주식, 1:선옵, 2:업종
	{
		// 손절매
		if(IsBuyLossRate(lDealType))
		{
			pDC->SetTextColor(m_ColorSon);
			pDC->TextOut(nXPos, nYPos, m_stLogoSon);
		}
		// 목표수익
		else if(IsBuyTargetBenefit(lDealType))
		{
			pDC->SetTextColor(m_ColorSooik);
			pDC->TextOut(nXPos, nYPos, m_stLogoSooik);
		}
		// 최고가대비 하락
		else if(IsBuyHPriceCPBear(lDealType))
		{
			pDC->SetTextColor(m_ColorGoga);
			pDC->TextOut(nXPos, nYPos, m_stLogoGoga);
		}
		// 목표가보존청산
		else if(IsBuyTargetValuePreserve(lDealType))
		{
			pDC->SetTextColor(m_ColorTarget);
			pDC->TextOut(nXPos, nYPos, m_stLogoTarget);
		}
		// 이익보존청산
		else if(IsBuyBenefitPreserve(lDealType))
		{
			pDC->SetTextColor(m_ColorBenefit);
			pDC->TextOut(nXPos, nYPos, m_stLogoBenefit);
		}
		// 최소가격변화
		else if(IsBuyMiniPrice(lDealType))
		{
			pDC->SetTextColor(RGB(0,255,0));
			pDC->TextOut(nXPos, nYPos, "▦");
		}
		// 당일청산
		else if(IsBuyDayLiquidation(lDealType))
		{
			pDC->SetTextColor(RGB(214,29,204));
			pDC->TextOut(nXPos, nYPos, "◐");
		}
	}
	// 선물
	else if(m_STConfigInfo.m_nCodeType == 1)
	{				
		// 매수/매수청산 허용 or 모두 허용
		if(m_nApplyPosition == 0 || m_nApplyPosition == 2)
		{
			// 손절매
			if(IsBuyLossRate(lDealType))
			{
				pDC->SetTextColor(m_ColorSon);
				pDC->TextOut(nXPos, nYPos, m_stLogoSon);
			}
			// 목표수익
			else if(IsBuyTargetBenefit(lDealType))
			{
				pDC->SetTextColor(m_ColorSooik);
				pDC->TextOut(nXPos, nYPos, m_stLogoSooik);
			}
			// 최고가대비 하락
			else if(IsBuyHPriceCPBear(lDealType))
			{
				pDC->SetTextColor(m_ColorGoga);
				pDC->TextOut(nXPos, nYPos, m_stLogoGoga);
			}
			// 목표가보존청산
			else if(IsBuyTargetValuePreserve(lDealType))
			{
				pDC->SetTextColor(m_ColorTarget);
				pDC->TextOut(nXPos, nYPos, m_stLogoTarget);
			}
			// 이익보존청산
			else if(IsBuyBenefitPreserve(lDealType))
			{
				pDC->SetTextColor(m_ColorBenefit);
				pDC->TextOut(nXPos, nYPos, m_stLogoBenefit);
			}
			// 최소가격변화
			else if(IsBuyMiniPrice(lDealType))
			{
				pDC->SetTextColor(RGB(0,255,0));
				pDC->TextOut(nXPos, nYPos, "▦");
			}
			// 당일청산
			else if(IsBuyDayLiquidation(lDealType))
			{
				pDC->SetTextColor(RGB(214,29,204));
				pDC->TextOut(nXPos, nYPos, "◐");
			}
		}
	}
}

void CWndAssistIndexManager::DrawSignal_SellForcedLiquidation(CDC* pDC, long lDealType, int nXPos, int nYPos)
{
	// 선물
	if(m_STConfigInfo.m_nCodeType == 1)	// 0:주식, 1:선옵, 2:업종
	{				
		// 매도/매도청산 허용 or 모두 허용
		if(m_nApplyPosition == 1 || m_nApplyPosition == 2)
		{
			// 손절매
			if(IsSellLossRate(lDealType))
			{
				pDC->SetTextColor(m_ColorSon);
				pDC->TextOut(nXPos, nYPos, m_stLogoSon);
			}
			// 목표수익
			else if(IsSellTargetBenefit(lDealType))
			{
				pDC->SetTextColor(m_ColorSooik);
				pDC->TextOut(nXPos, nYPos, m_stLogoSooik);
			}
			// 최고가대비 하락
			else if(IsSellHPriceCPBear(lDealType))
			{
				pDC->SetTextColor(m_ColorGoga);
				pDC->TextOut(nXPos, nYPos, m_stLogoGoga);
			}
			// 목표가보존청산
			else if(IsSellTargetValuePreserve(lDealType))
			{
				pDC->SetTextColor(m_ColorTarget);
				pDC->TextOut(nXPos, nYPos, m_stLogoTarget);
			}
			// 이익보존청산
			else if(IsSellBenefitPreserve(lDealType))
			{
				pDC->SetTextColor(m_ColorBenefit);
				pDC->TextOut(nXPos, nYPos, m_stLogoBenefit);
			}
			// 최소가격변화
			else if(IsSellMiniPrice(lDealType))
			{
				pDC->SetTextColor(RGB(0,255,0));
				pDC->TextOut(nXPos, nYPos, "▦");
			}
			// 당일청산
			else if(IsSellDayLiquidation(lDealType))
			{
				pDC->SetTextColor(RGB(214,29,204));
				pDC->TextOut(nXPos, nYPos, "◐");
			}
		}
	}
}

void CWndAssistIndexManager::DrawSignal_BuyOrExitShort(CDC* pDC, long lDealType, int nXPos, int nYPos, CString strDealName)
{
	CRgn rgn;
	CRgn rgnCanvas;
	CRgn rgnSignal;
	rgn.CreateRectRgn(0, 0, 0, 0);
	rgnCanvas.CreateRectRgnIndirect(m_rctBlockRegion);
	rgnSignal.CreateRectRgn(0, 0, 0, 0);

	CSize szDealName;
	if(!strDealName.IsEmpty())
		szDealName = pDC->GetOutputTextExtent(strDealName);

	// 주식
	if(m_STConfigInfo.m_nCodeType == 0)	// 0:주식, 1:선옵, 2:업종
	{
		rgn.CopyRgn(&m_rgnBuy);
		rgn.OffsetRgn(nXPos, nYPos);
		rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

		// 매수 신호 Draw
		if(m_lShapeSignalBuy == 2)			// Brush로 신호 Shape의 Border만 Draw
			pDC->FrameRgn(&rgnSignal, &m_brushBuy, 1, 1);
		else									// Brush로 신호 Shape를 채움
			pDC->FillRgn(&rgnSignal, &m_brushBuy);						
	}
	// 선물
	else
	{
		// 매수/매수청산 허용 or 모두 허용
		if(m_nApplyPosition == 0 || m_nApplyPosition == 2)
		{
			rgn.CopyRgn(&m_rgnBuy);
			rgn.OffsetRgn(nXPos, nYPos);
			rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

			// 매수 신호 Draw
			if(m_lShapeSignalBuy == 2)			// Brush로 신호 Shape의 Border만 Draw
				pDC->FrameRgn(&rgnSignal, &m_brushBuy, 1, 1);
			else								// Brush로 신호 Shape를 채움
				pDC->FillRgn(&rgnSignal, &m_brushBuy);
		}

		// 매도/매도청산 허용 or 모두 허용
		if(m_nApplyPosition == 1 || m_nApplyPosition == 2)
		{
			rgn.CopyRgn(&m_rgnExitShort);
			rgn.OffsetRgn(nXPos, nYPos + 10);	// 매수청산 신호 위에 표시함
			rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

			// 매도청산 신호 Draw
			if(m_lShapeSignalExitShort==2)		// Brush로 신호 Shape의 Border만 Draw
				pDC->FrameRgn(&rgnSignal, &m_brushExitShort, 1, 1);
			else								// Brush로 신호 Shape를 채움
				pDC->FillRgn(&rgnSignal, &m_brushExitShort);						
		}
	}

	pDC->SetBkColor(RGB(255,255,255));
	if(m_rctBlockRegion.PtInRect(CPoint(nXPos - szDealName.cx/2, nYPos + m_lHeightSell - szDealName.cy - 2)) && m_bShowName)
		pDC->TextOut(nXPos - szDealName.cx/2, nYPos + m_lHeightSell - szDealName.cy - 2, strDealName);
}

void CWndAssistIndexManager::DrawSignal_SellOrExitLong(CDC* pDC, long lDealType, int nXPos, int nYPos, CString strDealName)
{
	CRgn rgn;
	CRgn rgnCanvas;
	CRgn rgnSignal;
	rgn.CreateRectRgn(0, 0, 0, 0);
	rgnCanvas.CreateRectRgnIndirect(m_rctBlockRegion);
	rgnSignal.CreateRectRgn(0, 0, 0, 0);

	CSize szDealName;
	if(!strDealName.IsEmpty())
		szDealName = pDC->GetOutputTextExtent(strDealName);

	// 주식
	if(m_STConfigInfo.m_nCodeType == 0)	// 0:주식, 1:선옵, 2:업종
	{
		rgn.CopyRgn(&m_rgnExitLong);
		rgn.OffsetRgn(nXPos, nYPos);
		rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

		// 매수청산 신호 Draw
		if(m_lShapeSignalExitLong == 2)			// Brush로 신호 Shape의 Border만 Draw
			pDC->FrameRgn(&rgnSignal, &m_brushExitLong, 1, 1);
		else									// Brush로 신호 Shape를 채움
			pDC->FillRgn(&rgnSignal, &m_brushExitLong);
	}
	// 선물
	else
	{				
		// 매수/매수청산 허용 or 모두 허용
		if(m_nApplyPosition == 0 || m_nApplyPosition == 2)
		{
			rgn.CopyRgn(&m_rgnExitLong);
			rgn.OffsetRgn(nXPos, nYPos);
			rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

			// 매수청산 신호 Draw
			if(m_lShapeSignalExitLong == 2)		// Brush로 신호 Shape의 Border만 Draw
				pDC->FrameRgn(&rgnSignal, &m_brushExitLong, 1, 1);
			else								// Brush로 신호 Shape를 채움
				pDC->FillRgn(&rgnSignal, &m_brushExitLong);
		}

		// 매도/매도청산 허용 or 모두 허용
		if(m_nApplyPosition == 1 || m_nApplyPosition == 2)
		{
			rgn.CopyRgn(&m_rgnSell);
			rgn.OffsetRgn(nXPos, nYPos - 10);	// 매수 신호 아래에 표시함
			rgnSignal.CombineRgn(&rgn, &rgnCanvas, RGN_AND);

			// 매도 신호 Draw
			if(m_lShapeSignalSell == 2)			// Brush로 신호 Shape의 Border만 Draw
				pDC->FrameRgn(&rgnSignal, &m_brushSell, 1, 1);
			else								// Brush로 신호 Shape를 채움
				pDC->FillRgn(&rgnSignal, &m_brushSell);						
		}
	}
	pDC->SetBkColor(RGB(255,255,255));
	if(m_rctBlockRegion.PtInRect(CPoint(nXPos - szDealName.cx/2, nYPos + m_lHeightBuy + szDealName.cy + 2)) && m_bShowName)
		pDC->TextOut(nXPos - szDealName.cx/2, nYPos + m_lHeightBuy + 2, strDealName);
}

// 강제청산 신호 그리기
void CWndAssistIndexManager::DrawSignal_ForcedLiquidation(CDC* pDC, int nXPos, int nYPos, CString strLogoName, COLORREF clrLogo)
{
	pDC->SetTextColor(clrLogo);
	pDC->SetBkColor(m_pIChartOCX->GetStyleValueGraphBGColor());//KHD: 수정..그릴때 Bk컬러 설정..
	pDC->TextOut(nXPos, nYPos, strLogoName);
}

void CWndAssistIndexManager::SetSiteCUstomAddin(LONG Addin)
{
	m_pStieCustomAddin = Addin;
}

///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	기존 시고저종 데이터를 Setting한다.
//////////////////////////////////////////////////////////////////////////////
BOOL CWndAssistIndexManager::SetRawData()
{
	// 시고저종 자료일자를 구한다 ---------------------------------------------->>

	if( m_pIPacketTime)		m_pIPacketTime->Release();
	if( m_pIPacketOpen)		m_pIPacketOpen->Release();
	if( m_pIPacketHigh)		m_pIPacketHigh->Release();
	if( m_pIPacketLow)		m_pIPacketLow->Release();
	if( m_pIPacketClose)	m_pIPacketClose->Release();
	if( m_pIPacketVolume)	m_pIPacketVolume->Release();
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	if( m_pIPacketExt)		m_pIPacketExt->Release();

	m_pIPacketTime = NULL;
	m_pIPacketOpen = NULL;
	m_pIPacketHigh = NULL;
	m_pIPacketLow = NULL;
	m_pIPacketClose = NULL;
	m_pIPacketVolume = NULL;
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	m_pIPacketExt = NULL;

	if(m_pSTItem)// 시스템트레이딩 신호를 데이터가 바뀌면 제거해준다.. (멀티차트에서 다시 세팅해줌)
	{
		DeleteSTInfo();
	}
	//@@@@@06.11.20 HWND[[
	HWND hSysTabEx = m_hSysTabEx;
	CString strSTPath(m_szMSTName);
	DestroyStrategyItem();
	ClearData();

	if(hSysTabEx && IsWindow(hSysTabEx))
	{
		const UINT RMSG_INDEXADDIN_NOTIFY_RAWDATA		= ::RegisterWindowMessage("RMSG_INDEXADDIN_NOTIFY_RAWDATA");
		::PostMessage(hSysTabEx, RMSG_INDEXADDIN_NOTIFY_RAWDATA, 0, 0);
	}
	//	SetHelpMessage();
	//@@@@@06.11.20 HWND]]
	return TRUE;
}


//*****************************************************************************
// 함  수  명  : BOOL CWndAssistIndexManager::CheckRawData()
// 내      용  : 
// 인      자  : 
// 호출  함수  : 
// 작  성  자  : 이준옥     (주)튜브픽처스
// Date        : 2006-12-27 오전 10:39:39
//				 선물틱의 경우 이평을 그리지 않는데도 데이터백업이 이뤄지기 때문에 SetRawData에서는 Clear관련 처리만하고
//				 데이터는 실제필요로 하는 경우에는 얻어올 수 있도록 수정한다.
//				 AddGraph()루틴에서 Called된다.
//				 테스트는 충분히 더 해봐야함.
//*****************************************************************************
BOOL CWndAssistIndexManager::CheckRawData()
{
	// 시고저종 자료일자를 구한다 ---------------------------------------------->>
	ILPCSTR istTmp;

	// 사용되고 있는 RQ :복수종목 - ojtaso (20070307)
	m_pIPacketTime	= m_pIPacketManager->GetPacket(m_strRQ,		PACKET_TIME);
	m_pIPacketOpen	= m_pIPacketManager->GetPacket(m_strRQ,		PACKET_OPEN);
	m_pIPacketHigh	= m_pIPacketManager->GetPacket(m_strRQ,		PACKET_HIGH);
	m_pIPacketLow	= m_pIPacketManager->GetPacket(m_strRQ,		PACKET_LOW);
	m_pIPacketClose	= m_pIPacketManager->GetPacket(m_strRQ,		PACKET_CLOSE);
	m_pIPacketVolume  = m_pIPacketManager->GetPacket(m_strRQ,	PACKET_VOLUME);
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	ILPCSTR lpszSTDataName = m_pIChartOCX->GetUseSTDataName();
	CString strSTDataName = lpszSTDataName;
	if(!strSTDataName.IsEmpty())
		m_pIPacketExt  = m_pIPacketManager->GetPacket(m_strRQ,	strSTDataName);

	if(!m_pIPacketTime
		||(!m_pIPacketOpen&&!m_pIPacketHigh&&!m_pIPacketLow&&!m_pIPacketClose&&!m_pIPacketVolume))	return FALSE;

	if(m_pIPacketTime)
		m_lCntOfCurrentPacketData = m_pIPacketTime->GetDataCount();		//Data 갯수를 가져온다. 

	if(m_pIPacketClose)
	{
		istTmp = m_pIPacketClose->GetType();
		m_strCloseScaleType = istTmp;
	}
	if(m_pIPacketOpen)
	{
		istTmp = m_pIPacketOpen->GetType();
		m_strOpenScaleType = istTmp;
	}
	if(m_pIPacketHigh)
	{
		istTmp = m_pIPacketHigh->GetType();
		m_strHighScaleType = istTmp;
	}
	if(m_pIPacketLow)
	{
		istTmp = m_pIPacketLow->GetType();
		m_strLowScaleType = istTmp;
	}
	if(m_pIPacketVolume)
	{
		istTmp = m_pIPacketVolume->GetType();
		m_strVolumeScaleType = istTmp;
	}
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	if(m_pIPacketExt)
	{
		istTmp = m_pIPacketExt->GetType();
		m_strExtScaleType = istTmp;
	}
	//>> 20081204 박승룡 - 허봉 생긴 후 데이터 추가 요청시 이평값 계산에 대한 버그수정
	// 허봉을 위한 Date Packet이 데이터(시고저종) Packet 보다 1가 더 많은 경우
	// 데이터 Packet을 실제 데이터 Count로 셋팅함
	long lCntOfCurrentCloseData = m_pIPacketClose->GetDataCount();
	if(lCntOfCurrentCloseData < m_lCntOfCurrentPacketData)
		m_lCntOfCurrentPacketData = lCntOfCurrentCloseData;
	//<< 20081204 박승룡 - 허봉 생긴 후 데이터 추가 요청시 이평값 계산에 대한 버그수정

	m_SDataItemDate.Init("DATE",NULL,m_lCntOfCurrentPacketData);
	m_DataItemOpen.Init("OPEN",NumericSeries,0,m_lCntOfCurrentPacketData);
	m_DataItemHigh.Init("HIGH",NumericSeries,0,m_lCntOfCurrentPacketData);
	m_DataItemLow.Init("LOW",NumericSeries,0,m_lCntOfCurrentPacketData);
	m_DataItemClose.Init("CLOSE",NumericSeries,0,m_lCntOfCurrentPacketData);
	m_DataItemVolume.Init("VOLUME",NumericSeries,0,m_lCntOfCurrentPacketData);
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	m_DataItemExt.Init(strSTDataName,NumericSeries,0,m_lCntOfCurrentPacketData);

	CString strDate;
	double	dData;
	if(m_pIPacketOpen&&m_pIPacketHigh&&m_pIPacketLow&&m_pIPacketClose&&m_pIPacketVolume)
	{		
		IDoubleList* plPacketTime	  = m_pIPacketTime->GetDataList();
		IDoubleList* pIPacketOpen	  = m_pIPacketOpen->GetDataList();
		IDoubleList* plPacketHigh	  = m_pIPacketHigh->GetDataList();
		IDoubleList* pIPacketLow	  = m_pIPacketLow->GetDataList();
		IDoubleList* pIPacketClose  = m_pIPacketClose->GetDataList();
		IDoubleList* pIPacketVolume = m_pIPacketVolume->GetDataList();

		POSITION posPacketTime		= plPacketTime->GetHeadPosition();
		POSITION posPacketOpen		= pIPacketOpen->GetHeadPosition();
		POSITION posPacketHigh		= plPacketHigh->GetHeadPosition();
		POSITION posPacketLow		= pIPacketLow->GetHeadPosition();
		POSITION posPacketClose		= pIPacketClose->GetHeadPosition();
		POSITION posPacketVolumn	= pIPacketVolume->GetHeadPosition();

		for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
		{
			if(posPacketTime)
			{
				plPacketTime->GetNext(posPacketTime, dData);
				strDate.Format("%.f", dData);
				m_SDataItemDate.SetAt(nPos,strDate);
			}

			if(posPacketOpen)
			{
				pIPacketOpen->GetNext(posPacketOpen, dData);
				m_DataItemOpen.SetAt(nPos, dData);
			}

			if(posPacketHigh)
			{
				plPacketHigh->GetNext(posPacketHigh, dData);
				m_DataItemHigh.SetAt(nPos, dData);
			}

			if(posPacketLow)
			{
				pIPacketLow->GetNext(posPacketLow, dData);
				m_DataItemLow.SetAt(nPos, dData);
			}

			if(posPacketClose)
			{
				pIPacketClose->GetNext(posPacketClose, dData);
				m_DataItemClose.SetAt(nPos, dData);
			}

			if(posPacketVolumn)
			{
				pIPacketVolume->GetNext(posPacketVolumn, dData);
				m_DataItemVolume.SetAt(nPos, dData);
			}
		}

		// 장전 마지막 시고저 데이터의 0값에 대한 보정
		long lPos = m_lCntOfCurrentPacketData - 1;
		if(lPos >= 0)
		{
			double dLastHighData = m_DataItemHigh.GetAt(lPos);
			double dLastLowData = m_DataItemLow.GetAt(lPos);
			double dLastOpenData = m_DataItemOpen.GetAt(lPos);
			double dLastCloseData = m_DataItemClose.GetAt(lPos);
			if(dLastHighData < dLastCloseData || 
				(dLastHighData == 0. || dLastLowData == 0. || dLastOpenData == 0.))
			{
				m_DataItemHigh.SetAt(lPos, dLastCloseData);
				m_DataItemLow.SetAt(lPos, dLastCloseData);
				m_DataItemOpen.SetAt(lPos, dLastCloseData);
			}
		}

		plPacketTime->Release();
		pIPacketOpen->Release();
		plPacketHigh->Release();
		pIPacketLow->Release();
		pIPacketClose->Release();
		pIPacketVolume->Release();

		// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
		if(m_pIPacketExt)
		{		
			IDoubleList* plPacketExt	  = m_pIPacketExt->GetDataList();

			POSITION posPacketExt		= plPacketExt->GetHeadPosition();

			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				if(posPacketExt)
				{
					plPacketExt->GetNext(posPacketExt, dData);
					m_DataItemExt.SetAt(nPos, dData);
				}
			}

			plPacketExt->Release();
		}
	}
	else
	{
		if(m_pIPacketTime)
		{
			IDoubleList* plPacketTime	  = m_pIPacketTime->GetDataList();
			POSITION posPacketTime = plPacketTime->GetHeadPosition();
			if(posPacketTime)
			{
				// 패킷 데이타가 없는 경우 처리 - ojtaso (20080618)
				for(int nPos = 0; nPos < m_lCntOfCurrentPacketData && posPacketTime; nPos++)
				{
					plPacketTime->GetNext(posPacketTime, dData);
					strDate.Format("%.f", dData);		
					m_SDataItemDate.SetAt(nPos,strDate);
				}
			}
			plPacketTime->Release();
		}

		if(m_pIPacketOpen)
		{
			IDoubleList* pIPacketOpen	  = m_pIPacketOpen->GetDataList();
			POSITION posPacketOpen = pIPacketOpen->GetHeadPosition();

			if(posPacketOpen)
			{
				// 패킷 데이타가 없는 경우 처리 - ojtaso (20080618)
				for(int nPos = 0; nPos < m_lCntOfCurrentPacketData && posPacketOpen; nPos++)
				{
					pIPacketOpen->GetNext(posPacketOpen, dData);
					m_DataItemOpen.SetAt(nPos, dData);
				}
			}
			pIPacketOpen->Release();
		}
		else
		{
			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				m_DataItemOpen.SetAt(nPos,0);
			}
		}
		if(m_pIPacketHigh)
		{
			IDoubleList* pIPacketHigh	  = m_pIPacketHigh->GetDataList();
			POSITION posPacketHigh = pIPacketHigh->GetHeadPosition();

			if(posPacketHigh)
			{
				// 패킷 데이타가 없는 경우 처리 - ojtaso (20080618)
				for(int nPos = 0; nPos < m_lCntOfCurrentPacketData && posPacketHigh; nPos++)
				{
					pIPacketHigh->GetNext(posPacketHigh, dData);
					m_DataItemHigh.SetAt(nPos, dData);
				}
			}
			pIPacketHigh->Release();
		}
		else
		{
			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				m_DataItemHigh.SetAt(nPos,0);
			}
		}
		if(m_pIPacketLow)
		{
			IDoubleList* pIPacketLow = m_pIPacketLow->GetDataList();
			POSITION posPacketLow = pIPacketLow->GetHeadPosition();

			if(posPacketLow)
			{
				// 패킷 데이타가 없는 경우 처리 - ojtaso (20080618)
				for(int nPos = 0; nPos < m_lCntOfCurrentPacketData && posPacketLow; nPos++)
				{
					pIPacketLow->GetNext(posPacketLow, dData);
					m_DataItemLow.SetAt(nPos, dData);
				}
			}
			pIPacketLow->Release();
		}
		else
		{
			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				m_DataItemLow.SetAt(nPos,0);
			}
		}
		if(m_pIPacketClose)
		{
			IDoubleList* pIPacketClose = m_pIPacketClose->GetDataList();
			POSITION posPacketClose = pIPacketClose->GetHeadPosition();
			if(posPacketClose)
			{
				// 패킷 데이타가 없는 경우 처리 - ojtaso (20080618)
				for(int nPos = 0; nPos<m_lCntOfCurrentPacketData && posPacketClose; nPos++)
				{
					pIPacketClose->GetNext(posPacketClose, dData);
					m_DataItemClose.SetAt(nPos, dData);
				}
			}
			pIPacketClose->Release();
		}
		else
		{
			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				m_DataItemClose.SetAt(nPos,0);
			}
		}
		if(m_pIPacketVolume)
		{
			IDoubleList* pIPacketVolume = m_pIPacketVolume->GetDataList();
			POSITION posPacketVolume = pIPacketVolume->GetHeadPosition();

			if(posPacketVolume)
			{
				// 패킷 데이타가 없는 경우 처리 - ojtaso (20080618)
				for(int nPos = 0; nPos < m_lCntOfCurrentPacketData && posPacketVolume; nPos++)
				{
					pIPacketVolume->GetNext(posPacketVolume, dData);
					m_DataItemVolume.SetAt(nPos, dData);
				}
			}
			pIPacketVolume->Release();
		}
		else
		{
			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				m_DataItemVolume.SetAt(nPos,0);
			}
		}

		// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
		if(m_pIPacketExt)
		{
			IDoubleList* pIPacketExt = m_pIPacketExt->GetDataList();
			POSITION posPacketExt = pIPacketExt->GetHeadPosition();

			if(posPacketExt)
			{
				// 패킷 데이타가 없는 경우 처리 - ojtaso (20080618)
				for(int nPos = 0; nPos < m_lCntOfCurrentPacketData && posPacketExt; nPos++)
				{
					pIPacketExt->GetNext(posPacketExt, dData);
					m_DataItemExt.SetAt(nPos, dData);
				}
			}
			pIPacketExt->Release();
		}
		else
		{
			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				m_DataItemExt.SetAt(nPos,0);
			}
		}

		// 장전 마지막 시고저 데이터의 0값에 대한 보정
		long lPos = m_lCntOfCurrentPacketData - 1;
		if(lPos >= 0)
		{
			double dLastHighData = m_DataItemHigh.GetAt(lPos);
			double dLastLowData = m_DataItemLow.GetAt(lPos);
			double dLastOpenData = m_DataItemOpen.GetAt(lPos);
			double dLastCloseData = m_DataItemClose.GetAt(lPos);
			if(dLastHighData < dLastCloseData || 
				(dLastHighData == 0. || dLastLowData == 0. || dLastOpenData == 0.))
			{
				m_DataItemHigh.SetAt(lPos, dLastCloseData);
				m_DataItemLow.SetAt(lPos, dLastCloseData);
				m_DataItemOpen.SetAt(lPos, dLastCloseData);
			}
		}
	}

	if(m_pIPacketTime)
	{
		istTmp = m_pIPacketTime->GetType();
		m_strTimeType = istTmp;;
	}

	SetHelpMessage();

	return TRUE;
}



BOOL CWndAssistIndexManager::SetCopyData()
{
	if(!m_pIPacketTime
		||(!m_pIPacketOpen&&!m_pIPacketHigh&&!m_pIPacketLow&&!m_pIPacketClose&&!m_pIPacketVolume))	return FALSE;

	//KHD : 수정 
	if(m_pIPacketTime)
		m_lCntOfCurrentPacketData = m_pIPacketTime->GetDataCount();//Data 갯수를 가져온다. 
	ILPCSTR istTmp;
	if(m_pIPacketOpen)
	{
		istTmp = m_pIPacketOpen->GetType();
		m_strOpenScaleType = istTmp;
	}

	if(m_pIPacketClose)
	{
		istTmp = m_pIPacketClose->GetType();
		m_strCloseScaleType = istTmp;
	}

	if(m_pIPacketHigh)
	{
		istTmp = m_pIPacketHigh->GetType();
		m_strHighScaleType = istTmp;
	}
	if(m_pIPacketLow)
	{
		istTmp = m_pIPacketLow->GetType();
		m_strLowScaleType = istTmp;
	}
	if(m_pIPacketVolume)
	{
		istTmp = m_pIPacketVolume->GetType();
		m_strVolumeScaleType = istTmp;
	}
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	if(m_pIPacketExt)
	{
		istTmp = m_pIPacketExt->GetType();
		m_strExtScaleType = istTmp;
	}

	m_SDataItemDate.Init("DATE",NULL,m_lCntOfCurrentPacketData);
	m_DataItemOpen.Init("OPEN",NumericSeries,0,m_lCntOfCurrentPacketData);
	m_DataItemHigh.Init("HIGH",NumericSeries,0,m_lCntOfCurrentPacketData);
	m_DataItemLow.Init("LOW",NumericSeries,0,m_lCntOfCurrentPacketData);
	m_DataItemClose.Init("CLOSE",NumericSeries,0,m_lCntOfCurrentPacketData);
	m_DataItemVolume.Init("VOLUME",NumericSeries,0,m_lCntOfCurrentPacketData);
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	ILPCSTR lpszSTDataName = m_pIChartOCX->GetUseSTDataName();
	CString strSTDataName = lpszSTDataName;
	m_DataItemExt.Init(strSTDataName,NumericSeries,0,m_lCntOfCurrentPacketData);

	CString strDate;
	double	dData;
	if(m_pIPacketOpen&&m_pIPacketHigh&&m_pIPacketLow&&m_pIPacketClose&&m_pIPacketVolume)
	{		
		IDoubleList* plPacketTime	  = m_pIPacketTime->GetDataList();
		IDoubleList* pIPacketOpen	  = m_pIPacketOpen->GetDataList();
		IDoubleList* plPacketHigh	  = m_pIPacketHigh->GetDataList();
		IDoubleList* pIPacketLow	  = m_pIPacketLow->GetDataList();
		IDoubleList* pIPacketClose  = m_pIPacketClose->GetDataList();
		IDoubleList* pIPacketVolume = m_pIPacketVolume->GetDataList();

		POSITION posPacketTime		= plPacketTime->GetHeadPosition();
		POSITION posPacketOpen		= pIPacketOpen->GetHeadPosition();
		POSITION posPacketHigh		= plPacketHigh->GetHeadPosition();
		POSITION posPacketLow		= pIPacketLow->GetHeadPosition();
		POSITION posPacketClose		= pIPacketClose->GetHeadPosition();
		POSITION posPacketVolumn	= pIPacketVolume->GetHeadPosition();

		for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
		{
			plPacketTime->GetNext(posPacketTime, dData);
			strDate.Format("%.f", dData);
			m_SDataItemDate.SetAt(nPos,strDate);

			pIPacketOpen->GetNext(posPacketOpen, dData);
			m_DataItemOpen.SetAt(nPos, dData);

			plPacketHigh->GetNext(posPacketHigh, dData);
			m_DataItemHigh.SetAt(nPos, dData);

			pIPacketLow->GetNext(posPacketLow, dData);
			m_DataItemLow.SetAt(nPos, dData);

			pIPacketClose->GetNext(posPacketClose, dData);
			m_DataItemClose.SetAt(nPos, dData);

			pIPacketVolume->GetNext(posPacketVolumn, dData);
			m_DataItemVolume.SetAt(nPos, dData);
		}

		plPacketTime->Release();
		pIPacketOpen->Release();
		plPacketHigh->Release();
		pIPacketLow->Release();
		pIPacketClose->Release();
		pIPacketVolume->Release();
		
		// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
		if(m_pIPacketExt)
		{		
			IDoubleList* plPacketExt	  = m_pIPacketExt->GetDataList();

			POSITION posPacketExt		= plPacketExt->GetHeadPosition();

			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				plPacketExt->GetNext(posPacketExt, dData);
				m_DataItemExt.SetAt(nPos, dData);
			}

			plPacketExt->Release();
		}
	}
	else
	{
		if(m_pIPacketTime)
		{
			IDoubleList* plPacketTime	  = m_pIPacketTime->GetDataList();
			POSITION posPacketTime = plPacketTime->GetHeadPosition();

			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				plPacketTime->GetNext(posPacketTime, dData);
				strDate.Format("%.f", dData);		
				m_SDataItemDate.SetAt(nPos,strDate);
			}
			plPacketTime->Release();
		}

		if(m_pIPacketOpen)
		{
			IDoubleList* pIPacketOpen	  = m_pIPacketOpen->GetDataList();
			POSITION posPacketOpen = pIPacketOpen->GetHeadPosition();

			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				pIPacketOpen->GetNext(posPacketOpen, dData);
				m_DataItemOpen.SetAt(nPos, dData);
			}
			pIPacketOpen->Release();
		}
		else
		{
			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				m_DataItemOpen.SetAt(nPos,0);
			}
		}
		if(m_pIPacketHigh)
		{
			IDoubleList* pIPacketHigh	  = m_pIPacketHigh->GetDataList();
			POSITION posPacketHigh = pIPacketHigh->GetHeadPosition();

			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				pIPacketHigh->GetNext(posPacketHigh, dData);
				m_DataItemHigh.SetAt(nPos, dData);
			}
			pIPacketHigh->Release();
		}
		else
		{
			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				m_DataItemHigh.SetAt(nPos,0);
			}
		}
		if(m_pIPacketLow)
		{
			IDoubleList* pIPacketLow = m_pIPacketLow->GetDataList();
			POSITION posPacketLow = pIPacketLow->GetHeadPosition();

			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				pIPacketLow->GetNext(posPacketLow, dData);
				m_DataItemLow.SetAt(nPos, dData);
			}
			pIPacketLow->Release();
		}
		else
		{
			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				m_DataItemLow.SetAt(nPos,0);
			}
		}
		if(m_pIPacketClose)
		{
			IDoubleList* pIPacketClose = m_pIPacketClose->GetDataList();
			POSITION posPacketClose = pIPacketClose->GetHeadPosition();

			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				pIPacketClose->GetNext(posPacketClose, dData);
				m_DataItemClose.SetAt(nPos, dData);
			}
			pIPacketClose->Release();
		}
		else
		{
			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				m_DataItemClose.SetAt(nPos,0);
			}
		}
		if(m_pIPacketVolume)
		{
			IDoubleList* pIPacketVolume = m_pIPacketVolume->GetDataList();
			POSITION posPacketVolume = pIPacketVolume->GetHeadPosition();

			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				pIPacketVolume->GetNext(posPacketVolume, dData);
				m_DataItemVolume.SetAt(nPos, dData);
			}
			pIPacketVolume->Release();
		}
		else
		{
			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				m_DataItemVolume.SetAt(nPos,0);
			}
		}
		// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
		if(m_pIPacketExt)
		{
			IDoubleList* pIPacketExt = m_pIPacketExt->GetDataList();
			POSITION posPacketExt = pIPacketExt->GetHeadPosition();

			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				pIPacketExt->GetNext(posPacketExt, dData);
				m_DataItemExt.SetAt(nPos, dData);
			}
			pIPacketExt->Release();
		}
		else
		{
			for(int nPos = 0; nPos < m_lCntOfCurrentPacketData; nPos++)
			{
				m_DataItemExt.SetAt(nPos,0);
			}
		}
	}

	if(m_pIPacketTime)
	{
		istTmp = m_pIPacketTime->GetType();
		m_strTimeType = istTmp;;
	}

	//]] 2006.09.30

	
	return TRUE;
}

//KHD : m_strBasicalExecutionOption를 초기화 한다. 
BOOL CWndAssistIndexManager::SetHelpMessage()
{
	//KHD : PREVPRICE2=32500:33000:32000:32700
	if(m_pIPacketManager)
	{
		ILPCSTR str;
		m_pIPacketManager->GetHelpMessageItem(EXECUTE_PREVDATA,str);
		CString stMessage;
		stMessage = (LPCSTR)str;// stTest = "177.90:177.60:177.80:177.50";

		////////////////////////
		// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
		m_strBasicalExecutionOption.Format("%s=%d,%s=%d,%s=%d,%s=%d,%s=%d,%s=%d,%s=%d,%s=%s,%s=%s"
			,EXECUTE_DATEPOINTER
			,(long)&m_SDataItemDate
			,EXECUTE_OPENPOINTER
			,(long)&m_DataItemOpen
			,EXECUTE_HIGHPOINTER
			,(long)&m_DataItemHigh
			,EXECUTE_LOWPOINTER
			,(long)&m_DataItemLow
			,EXECUTE_CLOSEPOINTER
			,(long)&m_DataItemClose
			,EXECUTE_VOLUMEPOINTER
			,(long)&m_DataItemVolume
			,EXECUTE_EXTDATA
			,(long)&m_DataItemExt
			,EXECUTE_TIMETYPE
			,m_strTimeType
			,EXECUTE_PREVDATA//KHD 지표관련 
			,stMessage);
		return TRUE;
	}
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	실시간 데이터를 받을때 기본데이터 와 지표들을 Update및 Insert 해준다.
//////////////////////////////////////////////////////////////////////////////
/*
BOOL CWndAssistIndexManager::UpdateRawData()
{
	if(!m_pIPacketTime
		||(!m_pIPacketOpen&&!m_pIPacketHigh&&!m_pIPacketLow&&!m_pIPacketClose&&!m_pIPacketVolume))	return FALSE;
	int nCntInMap = m_mapIndexItem.GetCount();
	if(!nCntInMap) return FALSE;

	// (2006/2/27 - Seung-Won, Bae) Check Data Count of Time, Close, Volume Packet
	long lNewCntOfPacketTimeData = m_pIPacketTime->GetDataCount();
	if( m_pIPacketClose)	if( m_pIPacketClose->GetDataCount()		!= lNewCntOfPacketTimeData) return FALSE;
	if(m_pIPacketOpen)		if( m_pIPacketOpen->GetDataCount()		!= lNewCntOfPacketTimeData) return FALSE;
	if(m_pIPacketHigh)		if( m_pIPacketHigh->GetDataCount()		!= lNewCntOfPacketTimeData) return FALSE;
	if(m_pIPacketLow)		if( m_pIPacketLow->GetDataCount()		!= lNewCntOfPacketTimeData) return FALSE;
	if(m_pIPacketVolume)	if( m_pIPacketVolume->GetDataCount()	!= lNewCntOfPacketTimeData) return FALSE;

	BOOL bIsUpdateData = TRUE;
	double dValue = 0.;
	double dLastClose = 0.;
	CString strDate;
	long lCntUpdate = 1;
	if(m_lCntOfCurrentPacketData!=lNewCntOfPacketTimeData)
	{
		lCntUpdate = lNewCntOfPacketTimeData - m_lCntOfCurrentPacketData;
		if(lCntUpdate<0)
		{
			gOffIndex ++;
			TRACE("minus happened\n");
			//return UpdateRawDataEx(gOffIndex, lCntUpdate);
			gOffUpdate += ((lCntUpdate) * (-1));
		}

		m_lCntOfCurrentPacketData = lNewCntOfPacketTimeData;
		bIsUpdateData = FALSE;

// alziotest{
		if(lCntUpdate<0)
		{			

			int nReverseCnt = lCntUpdate * (-1);
			//int nReverseCnt = 50;
			// nReverseCnt = 1;
			m_SDataItemDate.RemoveFront(nReverseCnt);
			m_DataItemOpen.m_arrayData.RemoveAt(0, nReverseCnt);
			m_DataItemHigh.m_arrayData.RemoveAt(0, nReverseCnt);
			m_DataItemLow.m_arrayData.RemoveAt(0, nReverseCnt);
			m_DataItemClose.m_arrayData.RemoveAt(0, nReverseCnt);
			m_DataItemVolume.m_arrayData.RemoveAt(0, nReverseCnt);
// alziotest}


// / *
				m_SDataItemDate.SetSize(m_lCntOfCurrentPacketData);
				m_DataItemOpen.SetSize(m_lCntOfCurrentPacketData);
				m_DataItemHigh.SetSize(m_lCntOfCurrentPacketData);
				m_DataItemLow.SetSize(m_lCntOfCurrentPacketData);
				m_DataItemClose.SetSize(m_lCntOfCurrentPacketData);
				m_DataItemVolume.SetSize(m_lCntOfCurrentPacketData);
	
				for(int i=0; i<m_lCntOfCurrentPacketData; i++)
				{
					strDate.Format("%.f",m_pIPacketTime->GetData(i));		
					m_SDataItemDate.SetAt(i,strDate);
	
					if(m_pIPacketClose)
					{			
						m_DataItemClose.SetAt(i,m_pIPacketClose->GetData(i));
					}
					if(m_pIPacketOpen)
					{
						m_DataItemOpen.SetAt(i,m_pIPacketOpen->GetData(i));
					}
					if(m_pIPacketHigh)
					{
						m_DataItemHigh.SetAt(i,m_pIPacketHigh->GetData(i));
					}
					if(m_pIPacketLow)
					{
						m_DataItemLow.SetAt(i,m_pIPacketLow->GetData(i));
					}
					if(m_pIPacketVolume)
					{
						m_DataItemVolume.SetAt(i,m_pIPacketVolume->GetData(i));
					}
				}
// * /
			}

		}

	

	POSITION pos = NULL;
	CIndexItem *pIndexItem = NULL;
	CString strDllSTName;
	for(long lDataItemPos=(m_lCntOfCurrentPacketData-lCntUpdate);lDataItemPos<m_lCntOfCurrentPacketData;lDataItemPos++)
	{
		long rDataItemPos = lDataItemPos + (gOffIndex * 50);

		if(!bIsUpdateData)
		{
			m_SDataItemDate.SetSize(lDataItemPos+1);
			m_DataItemOpen.SetSize(lDataItemPos+1);
			m_DataItemHigh.SetSize(lDataItemPos+1);
			m_DataItemLow.SetSize(lDataItemPos+1);
			m_DataItemClose.SetSize(lDataItemPos+1);
			m_DataItemVolume.SetSize(lDataItemPos+1);
		}
		strDate.Format("%.f",m_pIPacketTime->GetData(lDataItemPos));		
		m_SDataItemDate.SetAt(lDataItemPos,strDate);
		if(m_pIPacketClose)
		{			
			m_DataItemClose.SetAt(lDataItemPos,m_pIPacketClose->GetData(lDataItemPos));
		}
		if(m_pIPacketOpen)
		{
			m_DataItemOpen.SetAt(lDataItemPos,m_pIPacketOpen->GetData(lDataItemPos));
		}
		if(m_pIPacketHigh)
		{
			m_DataItemHigh.SetAt(lDataItemPos,m_pIPacketHigh->GetData(lDataItemPos));
		}
		if(m_pIPacketLow)
		{
			m_DataItemLow.SetAt(lDataItemPos,m_pIPacketLow->GetData(lDataItemPos));
		}
		if(m_pIPacketVolume)
		{
			m_DataItemVolume.SetAt(lDataItemPos,m_pIPacketVolume->GetData(lDataItemPos));
		}

		// 보조지표
		pos = m_mapIndexItem.GetStartPosition();
		while(pos != NULL)
		{		
			m_mapIndexItem.GetNextAssoc(pos, strDllSTName, pIndexItem);
			pIndexItem->m_DllSTLoader.Update(lDataItemPos,bIsUpdateData);
		}

		// 시스템 트레이딩 신호
		if(m_pSTItem)
		{
			m_pSTItem->m_indexType = STIndex;
			m_pSTItem->m_DllSTLoader.Update(lDataItemPos,bIsUpdateData);
		}
	}
	return TRUE;
}
*/

//int gOffIndex=0;
//int gOffUpdate = 0;
BOOL CWndAssistIndexManager::UpdateRawData()
{
	if(!m_pIPacketTime
		||(!m_pIPacketOpen&&!m_pIPacketHigh&&!m_pIPacketLow&&!m_pIPacketClose&&!m_pIPacketVolume))	return FALSE;
	int nCntInMap = m_mapIndexItem.GetCount();

	if(!nCntInMap) return FALSE;

	// (2006/2/27 - Seung-Won, Bae) Check Data Count of Time, Close, Volume Packet
	long lNewCntOfPacketTimeData = m_pIPacketTime->GetDataCount();
	if(lNewCntOfPacketTimeData ==0) return FALSE;//KHD : 11.30 수정 
	if( m_pIPacketClose)	if( m_pIPacketClose->GetDataCount()		!= lNewCntOfPacketTimeData) return FALSE;
	if(m_pIPacketOpen)		if( m_pIPacketOpen->GetDataCount()		!= lNewCntOfPacketTimeData) return FALSE;
	if(m_pIPacketHigh)		if( m_pIPacketHigh->GetDataCount()		!= lNewCntOfPacketTimeData) return FALSE;
	if(m_pIPacketLow)		if( m_pIPacketLow->GetDataCount()		!= lNewCntOfPacketTimeData) return FALSE;
	if(m_pIPacketVolume)	if( m_pIPacketVolume->GetDataCount()	!= lNewCntOfPacketTimeData) return FALSE;
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	if(m_pIPacketExt)		if( m_pIPacketExt->GetDataCount()		!= lNewCntOfPacketTimeData) return FALSE;

	BOOL bIsUpdateData = TRUE;
	double dValue = 0.;
	double dLastClose = 0.;
	CString strDate;
	long lCntUpdate = 1;
	if(m_lCntOfCurrentPacketData!=lNewCntOfPacketTimeData)
	{
		lCntUpdate = lNewCntOfPacketTimeData - m_lCntOfCurrentPacketData;
		m_lCntOfCurrentPacketData = lNewCntOfPacketTimeData;
		bIsUpdateData = FALSE;

// alziotest
		if(lCntUpdate<0)
		{
			double dData;

			// 차트ocx에서 50개가 삭제되었으므로 똑같이 50개를 지운다.
			int nReverseCnt = ( lCntUpdate * (-1) ) + 1;
			m_SDataItemDate.RemoveFront(nReverseCnt);
			m_DataItemOpen.m_arrayData.RemoveAt(0, nReverseCnt);
			m_DataItemHigh.m_arrayData.RemoveAt(0, nReverseCnt);
			m_DataItemLow.m_arrayData.RemoveAt(0, nReverseCnt);
			m_DataItemClose.m_arrayData.RemoveAt(0, nReverseCnt);
			m_DataItemVolume.m_arrayData.RemoveAt(0, nReverseCnt);
			// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
			m_DataItemExt.m_arrayData.RemoveAt(0, nReverseCnt);

			// 리얼로 들어온 한 개를 위해 추가한다.
			m_SDataItemDate.SetSize(m_lCntOfCurrentPacketData);
			m_DataItemOpen.SetSize(m_lCntOfCurrentPacketData);
			m_DataItemHigh.SetSize(m_lCntOfCurrentPacketData);
			m_DataItemLow.SetSize(m_lCntOfCurrentPacketData);
			m_DataItemClose.SetSize(m_lCntOfCurrentPacketData);
			m_DataItemVolume.SetSize(m_lCntOfCurrentPacketData);
			// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
			m_DataItemExt.SetSize(m_lCntOfCurrentPacketData);

			long lDataItemPos = m_lCntOfCurrentPacketData-1;

			m_pIPacketTime->GetData(lDataItemPos, dData);
			strDate.Format("%.f", dData);
			m_SDataItemDate.SetAt(lDataItemPos,strDate);

			if(m_pIPacketClose)
			{
				m_pIPacketClose->GetData(lDataItemPos, dData);
				m_DataItemClose.SetAt(lDataItemPos, dData);
			}

			double t_dvalue = 0.00;
			if(m_pIPacketOpen)
			{
				m_pIPacketOpen->GetData(lDataItemPos, t_dvalue);
				m_DataItemOpen.SetAt(lDataItemPos, t_dvalue);
			}
			if(m_pIPacketHigh)
			{
				m_pIPacketHigh->GetData(lDataItemPos, dData);
				m_DataItemHigh.SetAt(lDataItemPos, dData);
			}
			if(m_pIPacketLow)
			{
				m_pIPacketLow->GetData(lDataItemPos, dData);
				m_DataItemLow.SetAt(lDataItemPos, dData);
			}
			if(m_pIPacketVolume)
			{
				m_pIPacketVolume->GetData(lDataItemPos, dData);
				m_DataItemVolume.SetAt(lDataItemPos, dData);
			}
			// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
			if(m_pIPacketExt)
			{
				m_pIPacketExt->GetData(lDataItemPos, dData);
				m_DataItemExt.SetAt(lDataItemPos, dData);
			}
			//
			CIndexItem *pIndexItem = NULL;
			// 보조지표
			POSITION pos = m_mapIndexItem.GetStartPosition();
			CString strDllSTName;
			while(pos != NULL)
			{		
				m_mapIndexItem.GetNextAssoc(pos, strDllSTName, pIndexItem);
				pIndexItem->m_DllSTLoader.Update(lDataItemPos,bIsUpdateData);
			}

			// 시스템 트레이딩 신호
			if(m_pSTItem)
			{
				m_pSTItem->m_indexType = STIndex;
				m_pSTItem->m_DllSTLoader.Update(lDataItemPos,bIsUpdateData);
			}

			// 테스트로 넣음. 아직 검증 안된 루틴.
			// @060922전략Real관련수정

				if(m_pStrategyItem)
					m_pStrategyItem->Update(lDataItemPos, bIsUpdateData);
		}
		else
		{	
			POSITION pos = NULL;
			CIndexItem *pIndexItem = NULL;
			CString strDllSTName;
			double dData;
			for(long lDataItemPos=(m_lCntOfCurrentPacketData-lCntUpdate);lDataItemPos<m_lCntOfCurrentPacketData;lDataItemPos++)
			{
				//long rDataItemPos = lDataItemPos + (gOffIndex * 50);
				long rDataItemPos = lDataItemPos;

				if(!bIsUpdateData)
				{
					m_SDataItemDate.SetSize(lDataItemPos+1);
					m_DataItemOpen.SetSize(lDataItemPos+1);
					m_DataItemHigh.SetSize(lDataItemPos+1);
					m_DataItemLow.SetSize(lDataItemPos+1);
					m_DataItemClose.SetSize(lDataItemPos+1);
					m_DataItemVolume.SetSize(lDataItemPos+1);
					// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
					m_DataItemExt.SetSize(lDataItemPos+1);
				}
				m_pIPacketTime->GetData(lDataItemPos, dData);
				strDate.Format("%.f",dData);		
				m_SDataItemDate.SetAt(lDataItemPos,strDate);
				if(m_pIPacketClose)
				{			
					m_pIPacketClose->GetData(lDataItemPos, dData);
					m_DataItemClose.SetAt(lDataItemPos, dData);
				}
				if(m_pIPacketOpen)
				{
					m_pIPacketOpen->GetData(lDataItemPos, dData);
					m_DataItemOpen.SetAt(lDataItemPos, dData);
				}
				if(m_pIPacketHigh)
				{
					m_pIPacketHigh->GetData(lDataItemPos, dData);
					m_DataItemHigh.SetAt(lDataItemPos, dData);
				}
				if(m_pIPacketLow)
				{
					m_pIPacketLow->GetData(lDataItemPos, dData);
					m_DataItemLow.SetAt(lDataItemPos, dData);
				}
				if(m_pIPacketVolume)
				{
					m_pIPacketVolume->GetData(lDataItemPos, dData);
					m_DataItemVolume.SetAt(lDataItemPos, dData);
				}
				// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
				if(m_pIPacketExt)
				{
					m_pIPacketExt->GetData(lDataItemPos, dData);
					m_DataItemExt.SetAt(lDataItemPos, dData);
				}

				// 보조지표
				pos = m_mapIndexItem.GetStartPosition();
				while(pos != NULL)
				{		
					m_mapIndexItem.GetNextAssoc(pos, strDllSTName, pIndexItem);
					pIndexItem->m_DllSTLoader.Update(lDataItemPos,bIsUpdateData);
				}

				// 시스템 트레이딩 신호
				if(m_pSTItem)
				{
					m_pSTItem->m_indexType = STIndex;
					m_pSTItem->m_DllSTLoader.Update(lDataItemPos,bIsUpdateData);
				}

				// 테스트로 넣음. 아직 검증 안된 루틴.
				// @060922전략Real관련수정
				if(m_pStrategyItem)
					m_pStrategyItem->Update(lDataItemPos, bIsUpdateData);
			}
		}
	}

	// 일봉같은 경우 데이터 갯수는 같지만 Real을 반영해야 하는 경우가 있다.
	// 이런 경우 처리 루틴.
	else
	{
		lCntUpdate = 1;
		POSITION pos = NULL;
		CIndexItem *pIndexItem = NULL;
		CString strDllSTName;
		double dData;
		for(long lDataItemPos=(m_lCntOfCurrentPacketData-lCntUpdate);lDataItemPos<m_lCntOfCurrentPacketData;lDataItemPos++)
		{
			long rDataItemPos = lDataItemPos;
			if(!bIsUpdateData)
			{
				m_SDataItemDate.SetSize(lDataItemPos+1);
				m_DataItemOpen.SetSize(lDataItemPos+1);
				m_DataItemHigh.SetSize(lDataItemPos+1);
				m_DataItemLow.SetSize(lDataItemPos+1);
				m_DataItemClose.SetSize(lDataItemPos+1);
				m_DataItemVolume.SetSize(lDataItemPos+1);
				// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
				m_DataItemExt.SetSize(lDataItemPos+1);
			}
			m_pIPacketTime->GetData(lDataItemPos, dData);
			strDate.Format("%.f",dData);		
			m_SDataItemDate.SetAt(lDataItemPos,strDate);
			if(m_pIPacketClose)
			{			
				m_pIPacketClose->GetData(lDataItemPos, dData);
				m_DataItemClose.SetAt(lDataItemPos, dData);
			}
			if(m_pIPacketOpen)
			{
				m_pIPacketOpen->GetData(lDataItemPos, dData);
				m_DataItemOpen.SetAt(lDataItemPos, dData);
			}
			if(m_pIPacketHigh)
			{
				m_pIPacketHigh->GetData(lDataItemPos, dData);
				m_DataItemHigh.SetAt(lDataItemPos, dData);
			}
			if(m_pIPacketLow)
			{
				m_pIPacketLow->GetData(lDataItemPos, dData);
				m_DataItemLow.SetAt(lDataItemPos, dData);
			}
			if(m_pIPacketVolume)
			{
				m_pIPacketVolume->GetData(lDataItemPos, dData);
				m_DataItemVolume.SetAt(lDataItemPos, dData);
			}
			// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
			if(m_pIPacketExt)
			{
				m_pIPacketExt->GetData(lDataItemPos, dData);
				m_DataItemExt.SetAt(lDataItemPos, dData);
			}

			// 보조지표
			pos = m_mapIndexItem.GetStartPosition();
			while(pos != NULL)
			{		
				m_mapIndexItem.GetNextAssoc(pos, strDllSTName, pIndexItem);
				pIndexItem->m_DllSTLoader.Update(lDataItemPos,bIsUpdateData);
			}

			// 시스템 트레이딩 신호
			if(m_pSTItem)
			{
				m_pSTItem->m_indexType = STIndex;
				m_pSTItem->m_DllSTLoader.Update(lDataItemPos,bIsUpdateData);
			}

			// 테스트로 넣음. 아직 검증 안된 루틴.
			// @060922전략Real관련수정
			if(m_pStrategyItem)
				m_pStrategyItem->Update(lDataItemPos, bIsUpdateData);
		}
	}
	return TRUE;
}

/*
BOOL CWndAssistIndexManager::UpdateRawDataEx(int nOffIndex, long lCntUpdate)
{
	POSITION pos = NULL;
	CIndexItem *pIndexItem = NULL;
	CString strDllSTName;
	CString strDate;
	lCntUpdate = lCntUpdate * (-1);
//	lCntUpdate = (lCntUpdate)
	lCntUpdate = 1;

	for(long l=0; l<lCntUpdate; l++)
//	for(long lDataItemPos=(m_lCntOfCurrentPacketData-lCntUpdate);lDataItemPos<m_lCntOfCurrentPacketData;lDataItemPos++)
	{
		//long lDataItemPos = m_lCntOfCurrentPacketData-lCntUpdate;
		long lDataItemPos = m_lCntOfCurrentPacketData-(nOffIndex*lCntUpdate);
		long rDataItemPos = lDataItemPos + (gOffIndex * lCntUpdate);

		BOOL bIsUpdateData = FALSE;
		if(!bIsUpdateData)
		{
			m_SDataItemDate.SetSize(rDataItemPos+1);
			m_DataItemOpen.SetSize(rDataItemPos+1);
			m_DataItemHigh.SetSize(rDataItemPos+1);
			m_DataItemLow.SetSize(rDataItemPos+1);
			m_DataItemClose.SetSize(rDataItemPos+1);
			m_DataItemVolume.SetSize(rDataItemPos+1);
		}
		strDate.Format("%.f",m_pIPacketTime->GetData(lDataItemPos));		
		m_SDataItemDate.SetAt(rDataItemPos,strDate);
		if(m_pIPacketClose)
		{			
			m_DataItemClose.SetAt(rDataItemPos,m_pIPacketClose->GetData(lDataItemPos));
		}
		if(m_pIPacketOpen)
		{
			m_DataItemOpen.SetAt(rDataItemPos,m_pIPacketOpen->GetData(lDataItemPos));
		}
		if(m_pIPacketHigh)
		{
			m_DataItemHigh.SetAt(rDataItemPos,m_pIPacketHigh->GetData(lDataItemPos));
		}
		if(m_pIPacketLow)
		{
			m_DataItemLow.SetAt(rDataItemPos,m_pIPacketLow->GetData(lDataItemPos));
		}
		if(m_pIPacketVolume)
		{
			m_DataItemVolume.SetAt(rDataItemPos,m_pIPacketVolume->GetData(lDataItemPos));
		}

		// 보조지표
		pos = m_mapIndexItem.GetStartPosition();
		while(pos != NULL)
		{		
			m_mapIndexItem.GetNextAssoc(pos, strDllSTName, pIndexItem);
			pIndexItem->m_DllSTLoader.Update(rDataItemPos,bIsUpdateData);
		}

		// 시스템 트레이딩 신호
		if(m_pSTItem)
		{
			m_pSTItem->m_indexType = STIndex;
			m_pSTItem->m_DllSTLoader.Update(rDataItemPos,bIsUpdateData);
		}

		lDataItemPos++;
	}
	return TRUE;
}
*/


///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	시스템 트레이딩 신호를 그려준다.
//	lpST		단위전략명
//	lpSTPath	단위전략위치 
//////////////////////////////////////////////////////////////////////////////
BOOL CWndAssistIndexManager::AddST(LPCTSTR lpST, LPCTSTR lpSTPath)
{
	// 실행중인 전략이 있으면 닫는다.
	if(m_pStrategyItem)
	{
		DestroyStrategyItem();
	}

	CString strLastSTName;
	if(m_pSTItem)
	{
		strLastSTName = m_pSTItem->m_strSTItemName;
		DeleteSTInfo();
	}
	if(strLastSTName==lpST) return FALSE;

	if(!m_pIPacketTime||(!m_pIPacketOpen&&!m_pIPacketHigh&&!m_pIPacketLow&&!m_pIPacketClose&&!m_pIPacketVolume))
	{
		CheckRawData();
	}

	CString strOption;
	strOption.Format("%s=%s",LOAD_MODE,MODE_ACTION);
	m_pSTItem = new CIndexItem;
	m_pSTItem->m_strSTItemName = lpST;
	m_pSTItem->m_strPath = lpSTPath;
	m_pSTItem->m_indexType = STIndex;
	long lRetValue = m_pSTItem->m_DllSTLoader.Load( lpSTPath, this->GetSafeHwnd(), (long)m_pSTItem,NULL,0,strOption);

	if(lRetValue==-1)
	{
		delete m_pSTItem;
		m_pSTItem = NULL;
		return FALSE;
	}
//	m_pSTItem->m_DllSTLoader.Stop();

	m_pSTItem->m_DllSTLoader.Execute(m_strBasicalExecutionOption);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	매매신호 환경정보를 가져온다. :여기서 체크합니다. KHD 
//////////////////////////////////////////////////////////////////////////////
void CWndAssistIndexManager::LoadEnvDealPos(CDealDataItem *pDataItem)
{
	m_lShapeSignalBuy = atol(pDataItem->GetComment(SHAPEBUY));
	m_lShapeSignalSell = atol(pDataItem->GetComment(SHAPESELL));
	m_lShapeSignalExitLong = atol(pDataItem->GetComment(SHAPEEXITLONG));
	m_lShapeSignalExitShort = atol(pDataItem->GetComment(SHAPEEXITSHORT));
	m_clrBuy = atol(pDataItem->GetComment(COLORBUY));
	m_clrSell = atol(pDataItem->GetComment(COLORSELL));
	m_clrExitLong = atol(pDataItem->GetComment(COLOREXITLONG));
	m_clrExitShort = atol(pDataItem->GetComment(COLOREXITSHORT));
	m_bShowName = atol(pDataItem->GetComment(SHOWSIGNALNAME));
	m_bShowQty = atol(pDataItem->GetComment(SHOWSIGNALQTY));
}

///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	강세약세신호 환경정보를 가져온다.
//////////////////////////////////////////////////////////////////////////////
void CWndAssistIndexManager::LoadEnvBullBear(CIDataItem *pDataItem)
{
	m_strBullName = pDataItem->GetComment("PLOTBULL1NAME");
	m_strBearName = pDataItem->GetComment("PLOTBEAR1NAME");
	m_clrBkBull = atol(pDataItem->GetComment(COLORBULL));
	m_clrBkBear = atol(pDataItem->GetComment(COLORBEAR));
}
//데이터를 Clear한다.
void CWndAssistIndexManager::ClearData()
{
	m_arrayDealDrawPosition.RemoveAll();
	m_arrayDealPosition.RemoveAll();
	m_arrayBullBearPosition.RemoveAll();

	//@@@@@06.11.20 HWND[[
	HWND hSysTabEx = m_hSysTabEx;
	DeleteSTInfo();
	DestroyStrategyItem();
	if(hSysTabEx && IsWindow(hSysTabEx))
	{
		const UINT RMSG_INDEXADDIN_NOTIFY_RAWDATA		= ::RegisterWindowMessage("RMSG_INDEXADDIN_NOTIFY_RAWDATA");
		//1로 보낸다. 왜냐하면 초기화 되기때문에 해당 Dlg의 리스트나 분할도 초기화 해야 하기 때문이다.
		::PostMessage(hSysTabEx, RMSG_INDEXADDIN_NOTIFY_RAWDATA, 0, 1);
	}
//	IMainInfoManager* pMainMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
//	if(pMainMng)
//		pMainMng->GetMainData("SETPW",NULL);
	m_szPW = "";
}

void CWndAssistIndexManager::Draw(CDC *p_pDC)
{
	// Drawing이 가능한 상황인지 확인한다.
	
	if( !m_pIChartManager || !m_pIChartOCX || !m_pIPacketManager) return;
	// (2004.09.15, 배승원) 특수차트인 경우에는 보이지 않도록 한다. (역시계곡선/삼선전환도/P&F)
	if( m_pIChartOCX->IsOnSpecialChart()) return;	
	long lCntOfDealPos = m_arrayDealPosition.GetSize();
	long lCntOfBullBearPos = m_arrayBullBearPosition.GetSize();	
	if(!m_pSTItem && m_StDataIndex.GetSize() == 0 && lCntOfDealPos <= 0 && lCntOfBullBearPos <= 0) 
		return;//KHD : 수정 : 날짜데이터 루틴 추가
	
	if(!lCntOfDealPos&&!lCntOfBullBearPos) return;
	try
	{
		// 차트의 기본정보 --------------------------------------------------------------------->>
		// 현 Data View의 Start/End Index를 구한다.
		// XScale : 관련 Interface로 사용 - vntsorl(20070411)
		//if( !m_pIChartOCX->GetViewStartEndIndex( m_nStartIndex, m_nEndIndex)) throw this;
		if( !m_pIChartManager->GetStartEndDrawIdx_Cur(m_strRQ, m_nStartIndex, m_nEndIndex)) throw this;

		CString szIndicatorName = "_PriceChart_";
		// 가격차트가 포함된 Block의 Index를 구한다.
		int nR, nC; int nV, nG;
		if( !m_pIChartManager->GetIndexOfGraph(m_strRQ, szIndicatorName, nR, nC, nV, nG)) throw this;
		
		// Block의 Log/뒤집기 설정을 확인하여 정확하게 Data를 변환한다. (가격<->Y Pos)
		BOOL bLog, bReverse;
		IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
		if( pIBlock)
		{
			bLog = pIBlock->IsLogScale();
			bReverse = pIBlock->IsVScaleInvert( nV);
//			pIBlock->GetLogAndReverseScaleFlag( bLog, bReverse);
			//pIBlock->Release();
		}


		// View의 Min/Max 값을 구한다.
		double dViewMin, dViewMax;
		if( m_pIChartManager->GetIndexOfGraph(m_strRQ, szIndicatorName, nR, nC, nV, nG))
		{
			pIBlock->GetVScaleGroupMinMax( nV, dViewMin, dViewMax);
		}
		CRect rct;
		if( pIBlock)
		{
			CRect rctTemp;
			pIBlock->GetGraphRegions( m_rctBlockRegion, rctTemp );
		}

		if( pIBlock)
		{
			m_rctGraphRegion = pIBlock->GetGraphDrwingRegion();
			//pIBlock->Release();
		}
		if(pIBlock) pIBlock->Release();
		int		nDataOnPage	 = m_nEndIndex - m_nStartIndex + 1;						// 현 Data View의 Data 갯수
		m_dCandleWidth = double( m_rctGraphRegion.Width()) / double( nDataOnPage);	// 봉의 폭을 구한다


		CPen *pOldPen = NULL;
		pOldPen = p_pDC->SelectObject(&m_penNormal);
		int		nX, nY=0;
		int		nLeftX, nRightX;
		double	dTempValue = 0;
		double	dTempDrawValue = 0;
		CString strDealName;
		CSize	szDealName;

		CRect	rectBkArea = m_rctBlockRegion;
		double	dArrawWidth = 10;
		

		CRgn rgn;
		CRgn rgnNew;
		CRgn rgnRectBlockRegion;
		rgn.CreateRectRgn(0, 0, 0, 0);
		rgnNew.CreateRectRgn(0, 0, 0, 0);
		rgnRectBlockRegion.CreateRectRgnIndirect(m_rctBlockRegion);

		//// Draw Each Range By m_pStDataIndex's Info...KHD
		/////////////////////////////////////////////////////
		
		if(!m_StDataIndex.GetSize() == 0)//날짜데이터가 들어와 있다면 수행!!:KHD
		{			
			CRect rectPeriod = m_rctGraphRegion;
			CRect rectSelected;
			ST_DATAINDEX stDataIndex;
			CString strTitle,strTitle2; //KHD : 2개로 분할
			CSize szTitle;
			CString szSTName;
			if(m_bIsDragging)
			{
				int nSizeStDataIndex = m_StDataIndex.GetSize();
				int nSizeArrayRectPeriod = m_arrayRectPeriod.GetSize();
				long lStartIndex = 0;
				long lEndIndex = 0;
				long lRange = 0;
				for(int nPos=0;nPos<nSizeArrayRectPeriod;nPos++)
				{
					stDataIndex = m_StDataIndex.GetAt(nPos+1);
					szSTName = stDataIndex.STName;
					rectPeriod = m_arrayRectPeriod.GetAt(nPos);
					if(!rectPeriod.IsRectEmpty())
					{
						rectSelected = rectPeriod;
						rectSelected.InflateRect(1,1,1,1);
						p_pDC->FrameRect(rectSelected,&m_brushPeriodFrame);
						if(m_nSelectedRectIndex==nPos)
						{
							p_pDC->FillSolidRect(rectPeriod,m_clrPeriodSelected);

							GetDataOffset(lStartIndex,lEndIndex,rectPeriod.left,rectPeriod.right
								,m_nStartIndex,m_dCandleWidth,m_rctGraphRegion);
							if(m_bIsContinuedByLeftOutside)
							{
								lStartIndex = stDataIndex.nStart;
							}
							if(m_bIsContinuedByRightOutside)
							{
								lEndIndex = stDataIndex.nEnd;
							}
							lRange = lEndIndex - lStartIndex;
							strTitle.Format("%ld(%ld봉):",nPos+1,lRange);
							szTitle = p_pDC->GetOutputTextExtent(strTitle);
							if(szTitle.cx <= rectPeriod.Width())
							{
								p_pDC->SetTextColor(RGB(255,255,255));
								p_pDC->TextOut(rectPeriod.left+(rectPeriod.right-rectPeriod.left)/2-szTitle.cx/2
									,rectPeriod.top,strTitle);
							}
						}
						else
						{
							p_pDC->FillSolidRect(rectPeriod,m_clrPeriod);
							lRange = stDataIndex.nEnd - stDataIndex.nStart;
							strTitle.Format("%ld(%ld봉)",nPos+1,lRange);
							szTitle = p_pDC->GetOutputTextExtent(strTitle);
							if(szTitle.cx <= rectPeriod.Width())
							{
								p_pDC->SetTextColor(RGB(0,0,0));
								p_pDC->TextOut(rectPeriod.left+(rectPeriod.right-rectPeriod.left)/2-szTitle.cx/2
									,rectPeriod.top,strTitle);
							}
						}
					}
				}
			}//Draging
			else
			{
				BOOL bIsContinuedByLeftOutside = FALSE;
				BOOL bIsContinuedByRightOutside = FALSE;
				m_arrayRectPeriod.RemoveAll();
				int nSize = m_StDataIndex.GetSize();
				m_arrayRectPeriod.SetSize(nSize-1);
				for(int nPos=1;nPos<nSize;nPos++)
				{
					if(nPos>1)	rectPeriod.top = rectPeriod.top + 28;
					rectPeriod.bottom = rectPeriod.top + 26;//원래 13이였으나 데이터 정보 추가로 늘림(KHD)
					rectPeriod.left = m_rctGraphRegion.left;
					rectPeriod.right = m_rctGraphRegion.right;
					stDataIndex = m_StDataIndex.GetAt(nPos);
					szSTName  = stDataIndex.STName;
					bIsContinuedByLeftOutside = FALSE;
					bIsContinuedByRightOutside = FALSE;
					
					if(stDataIndex.nStart>=m_nStartIndex&&stDataIndex.nStart<=m_nEndIndex)
					{
						// XScale : 관련 Interface로 사용 - vntsorl(20070413)
						//GetXPositoin(stDataIndex.nStart - m_nStartIndex
						//	, m_dCandleWidth, m_rctGraphRegion,nLeftX,nRightX);
						m_pIChartManager->GetXPosition(m_strRQ, stDataIndex.nStart, nC, nLeftX, nRightX);
						rectPeriod.left = nLeftX;
					}
					else if(stDataIndex.nStart>m_nEndIndex)
					{
						stDataIndex.nStart = -1;	
					}
					else
					{
						bIsContinuedByLeftOutside = TRUE;
					}

					if(stDataIndex.nEnd>=m_nStartIndex&&stDataIndex.nEnd<=m_nEndIndex)
					{
						// XScale : 관련 Interface로 사용 - vntsorl(20070413)
						//GetXPositoin(stDataIndex.nEnd - m_nStartIndex
						//	, m_dCandleWidth, m_rctGraphRegion,nLeftX,nRightX);
						m_pIChartManager->GetXPosition(m_strRQ, stDataIndex.nEnd, nC, nLeftX, nRightX);
						rectPeriod.right = nRightX;
					}
					else if(stDataIndex.nEnd<m_nStartIndex)
					{
						stDataIndex.nEnd = -1;
					}	
					else
					{
						bIsContinuedByRightOutside = TRUE;
					}

					if(stDataIndex.nStart == -1 ||stDataIndex.nEnd == -1 )
					{
						rectPeriod.SetRectEmpty();
						m_arrayRectPeriod.SetAt(nPos-1,rectPeriod);
						continue;
					}				
					//rectPeriod.SetRect(0,0,100,100);
					m_clrBkBull =RGB(255,0,0);
					m_clrBkBear = RGB(177,214,250);
					m_clrPeriod = RGB(177,214,250);
					m_clrPeriodSelected = RGB(255,0,0);
					//KHD : 색상적용
					p_pDC->FillSolidRect(rectPeriod,m_clrBkBull);
					rectSelected = rectPeriod;
					rectSelected.InflateRect(1,1,1,1);
					p_pDC->FrameRect(rectSelected,&m_brushPeriodFrame);

					//if(m_nSelectedRectIndex==(nPos-1))//When Selected
					//{
					//	p_pDC->FillSolidRect(rectPeriod,m_clrPeriodSelected);
					//	m_bIsContinuedByLeftOutside = bIsContinuedByLeftOutside;
					//	m_bIsContinuedByRightOutside = bIsContinuedByRightOutside;
					//	strTitle.Format("구간%ld(%ld봉) %s",nPos,stDataIndex.nEnd-stDataIndex.nStart,stDataIndex.strInfo1);//수정
					//	szTitle = p_pDC->GetOutputTextExtent(strTitle);
					//	if(szTitle.cx <= rectPeriod.Width())
					//	{
					//		p_pDC->SetTextColor(RGB(255,255,255));
					//		p_pDC->TextOut(rectPeriod.left+(rectPeriod.right-rectPeriod.left)/2-szTitle.cx/2
					//			,rectPeriod.top,strTitle);
					//	}

					//	szTitle = p_pDC->GetOutputTextExtent(stDataIndex.strInfo2);
					//	if(szTitle.cx <= rectPeriod.Width())
					//	{
					//		p_pDC->SetTextColor(RGB(255,255,255));
					//		p_pDC->TextOut(rectPeriod.left+(rectPeriod.right-rectPeriod.left)/2-szTitle.cx/2
					//			,rectPeriod.top+14,stDataIndex.strInfo2);
					//	}
					//}
					//else
					{
						//KHD : 수정 : 글씨 그대로 보이도록.. 수정 : 2006.09.4
						p_pDC->FillSolidRect(rectPeriod,m_clrPeriod);
						strTitle.Format("%ld(%ld봉)-%s",nPos,(stDataIndex.nEnd-stDataIndex.nStart)+1,stDataIndex.strInfo1);
						int x = rectPeriod.left+17;
						{
							p_pDC->SetTextColor(RGB(0,0,0));
							CRect rt(x,rectPeriod.top+1,rectPeriod.right,rectPeriod.bottom);
							p_pDC->DrawText(strTitle,rt,DT_LEFT);

						}
						{
							p_pDC->SetTextColor(RGB(0,0,0));
							CRect rt(x,rectPeriod.top+14,rectPeriod.right,rectPeriod.bottom);
							p_pDC->DrawText(stDataIndex.strInfo2,rt,DT_LEFT);
						}
						m_szSTName = stDataIndex.STName;
						//END :KHD
					}
					m_arrayRectPeriod.SetAt(nPos-1,rectPeriod);
				}
			}
			//전략명 집어넣기..

			//m_pstOrderData->szSTName = szSTName;
			//int nFind = m_pstOrderData->szSTName.Find("@");
			//if(nFind>0) m_pstOrderData->szSTName = m_pstOrderData->szSTName.Left(nFind);

		}//KHD END;
		///// Fill Bull Area & Bear Area
		/////////////////////////////////////////////////////
		if(lCntOfBullBearPos)
		{
			CRect rectBullBear = rectBkArea;
			int nSizeArrayBullBearPosition = m_arrayBullBearPosition.GetSize();
			int nEndIndexEdited = 0;
			if(m_nEndIndex>=nSizeArrayBullBearPosition)
			{
				nEndIndexEdited = nSizeArrayBullBearPosition - 1;
			}
			else
			{
				nEndIndexEdited = m_nEndIndex;
			}
			for(int nPos = m_nStartIndex;nPos<=nEndIndexEdited;nPos++)
			{
				// XScale : 관련 Interface로 사용 - vntsorl(20070413)
				//nX = GetXPositoin(nPos - m_nStartIndex , m_dCandleWidth, m_rctGraphRegion,nLeftX,nRightX);
				nX = m_pIChartManager->GetXPosition(m_strRQ, nPos, nC, nLeftX, nRightX);
				dTempValue = m_arrayBullBearPosition.GetAt(nPos);
				if(dTempValue)
				{
					rectBullBear.left = long( nX - m_dCandleWidth/2 -1);
					rectBullBear.right = long( nX + m_dCandleWidth/2 + 1);
					rectBullBear.OffsetRect(3,0);
					if(dTempValue==1)
					{
						p_pDC->FillSolidRect(rectBullBear,m_clrBkBull);
					}
					else if(dTempValue==-1)
					{
						p_pDC->FillSolidRect(rectBullBear,m_clrBkBear);
					}
				}
			}
		}
		//// Draw Buy, Sell, ExitLong and ExitShort : 화살표 그리는 루틴이다. : 이쪽에서 그린다.:KHD
		double dDealType;
		int nXPos;
		int nYPos;
		if(lCntOfDealPos)
		{
			int nSizeArrayDealPosition = m_arrayDealPosition.GetSize();
			int nEndIndexEdited = 0;
			if(m_nEndIndex >= nSizeArrayDealPosition)
				nEndIndexEdited = nSizeArrayDealPosition - 1;
			else
				nEndIndexEdited = m_nEndIndex;

			for(int nIndx = m_nStartIndex; nIndx <= nEndIndexEdited; nIndx++)
			{
				dDealType = m_arrayDealPosition.GetAt(nIndx);
				if(!dDealType && !m_arrayDealDrawPosition.GetAt(nIndx)) 
					continue;
				strDealName = m_arrayDealName.GetAt(nIndx);

				// XScale : 관련 Interface로 사용 - vntsorl(20070412)
				// 신호를 표시할 X, Y좌표를 구한다
				//nXPos = GetXPositoin(nIndx - m_nStartIndex , m_dCandleWidth, m_rctGraphRegion, nLeftX, nRightX);
//				nXPos = m_pIChartManager->GetXPosition(m_strRQ, nIndx, nC, nLeftX, nRightX);
//				nYPos = m_pIChartOCX->ConvertDataToYPosition(m_arrayDealDrawPosition.GetAt(nIndx), dViewMax, dViewMin, 
//					m_rctGraphRegion.top, m_rctGraphRegion.bottom, bLog, bReverse);

				//////////////////////////////////////////////////////////////////////////
				nXPos = m_pIChartManager->GetXPosition(m_strRQ, nIndx, nC, nLeftX, nRightX);
				if(IsBuyOrExitShort((long)dDealType))
				{
					nYPos = m_pIChartOCX->ConvertDataToYPosition(m_DataItemLow.GetAt(nIndx), dViewMax, dViewMin, 
																m_rctGraphRegion.top, m_rctGraphRegion.bottom, bLog, bReverse);
				}
				else if(IsSellOrExitLong((long)dDealType) || IsForcedLiquidation((long)dDealType))
				{
					nYPos = m_pIChartOCX->ConvertDataToYPosition(m_DataItemHigh.GetAt(nIndx), dViewMax, dViewMin, 
																m_rctGraphRegion.top, m_rctGraphRegion.bottom, bLog, bReverse);
				}

				// 강제청산 포지션
				if(IsForcedLiquidation((long)dDealType))
				{
					if(IsBuyForcedLiquidation((long)dDealType))
					{
						DrawSignal_BuyForcedLiquidation(p_pDC, (long)dDealType, nXPos - 8, nYPos - 25);
					}

					if(IsSellForcedLiquidation((long)dDealType))
					{
						DrawSignal_SellForcedLiquidation(p_pDC, (long)dDealType, nXPos - 8, nYPos - 25);
					}
				}

				// 매수
				if(IsBuy((long)dDealType))
					DrawSignal_Buy(p_pDC, nXPos, nYPos + 10, strDealName);
				// 매도청산
				if(IsExitShort((long)dDealType))
					DrawSignal_ExitShort(p_pDC, nXPos, nYPos + 10, strDealName);
				// 매도
				if(IsSell((long)dDealType))
					DrawSignal_Sell(p_pDC, nXPos, nYPos - 10, strDealName);
				// 매수청산
				if(IsExitLong((long)dDealType))
					DrawSignal_ExitLong(p_pDC, nXPos, nYPos - 10, strDealName);
				//////////////////////////////////////////////////////////////////////////

//				// 매수 / 매도청산
//				if(IsBuyOrExitShort((long)dDealType))
//					DrawSignal_BuyOrExitShort(p_pDC, (long)dDealType, nXPos, nYPos+10, strDealName);
//				// 매도 / 매수청산
//				else if(IsSellOrExitLong((long)dDealType))
//					DrawSignal_SellOrExitLong(p_pDC, (long)dDealType, nXPos, nYPos-10, strDealName);
//				// 손절매
//				else if(IsLossRate((long)dDealType))
//					DrawSignal_ForcedLiquidation(p_pDC, nXPos - 8, nYPos - 25, m_stLogoSon, m_ColorSon);
//				// 목표수익
//				else if(IsTargetBenefit((long)dDealType))
//					DrawSignal_ForcedLiquidation(p_pDC, nXPos - 8, nYPos - 25, m_stLogoSooik, m_ColorSooik);
//				// 최고가대비 하락
//				else if(IsHPriceCPBear((long)dDealType))
//					DrawSignal_ForcedLiquidation(p_pDC, nXPos - 8, nYPos - 25, m_stLogoGoga, m_ColorGoga);
//				//// 당일 저가대비 상승
//				//else if(IsLPriceCPBull((long)dDealType))
//				//	DrawSignal_ForcedLiquidation(p_pDC, nXPos - 8, nYPos - 25, m_stLogoJuga, m_ColorJuga);
//				// 목표가보존청산
//				else if(IsTargetValuePreserve((long)dDealType))
//					DrawSignal_ForcedLiquidation(p_pDC, nXPos - 8, nYPos - 25, m_stLogoTarget, m_ColorTarget);
//				// 이익보존청산
//				else if(IsBenefitPreserve((long)dDealType))
//					DrawSignal_ForcedLiquidation(p_pDC, nXPos - 8, nYPos - 25, m_stLogoBenefit, m_ColorBenefit);
//				// 최소가격변화
//				else if(IsMiniPrice((long)dDealType))
//					DrawSignal_ForcedLiquidation(p_pDC, nXPos - 8, nYPos - 25, "▦", RGB(0,255,0));
//				// 당일청산
//				else if(IsDayLiquidation((long)dDealType))
//					DrawSignal_ForcedLiquidation(p_pDC, nXPos - 8, nYPos - 25, "◐", RGB(214,29,204));
			}
		}

		/////////////////////////////////////////////////////
		p_pDC->SelectObject(pOldPen);

		//if(m_pstOrderData || m_pSTItem)//전략명나타내기..
		//{
			//p_pDC->SetTextColor(RGB(0,0,0));
			//CString strTitle;

			//if(m_pSTItem) 
			//{
			//	if(m_pSTItem->m_strSTItemName.Find("P_")==0)
			//		strTitle.Format( "전략:%s",m_pSTItem->m_strSTItemName.Mid(2));
			//	else
			//		strTitle.Format( "전략:%s",m_pSTItem->m_strSTItemName);
			//}
			//else	strTitle.Format( "전략:%s",m_pstOrderData->szSTName);
			p_pDC->SetTextColor(RGB(0,0,0));
			CString strTitle;
			strTitle.Format("전략:%s",m_szSTName);
			CSize szTitle = p_pDC->GetOutputTextExtent(strTitle);
			m_rectTitle.SetRect(m_rctBlockRegion.left+1,m_rctBlockRegion.top + 18
				, m_rctBlockRegion.left + 4 + szTitle.cx, m_rctBlockRegion.top + 18 + szTitle.cy);
			m_rectTitle.InflateRect(2,2,2,2);
			p_pDC->SetBkMode(TRANSPARENT);
			p_pDC->Rectangle(m_rectTitle);
			p_pDC->TextOut(m_rctBlockRegion.left + 4, m_rctBlockRegion.top + 20, strTitle );
		//}
	}
	catch(...)
	{
		return;
	}	

}
//KHD : 2006.08.29
BOOL CWndAssistIndexManager::GetDataOffset(long& rLLeftIndex, long& rLRightIndex, long lLeftXPosition, long lRightXPosition
										 , long lStartIndex, double dCandleWidth, CRect rctGraphRegion)
{
	rLLeftIndex = (long)(((double)(lLeftXPosition - 1 - rctGraphRegion.left)-0.5)
		/dCandleWidth +  (double)lStartIndex + 0.5);
	rLRightIndex = (long)(((double)(lRightXPosition + 1 - rctGraphRegion.left)-0.5)
		/dCandleWidth + (double)lStartIndex - 0.5 );	
	return TRUE;
}
void CWndAssistIndexManager::SetpChartModule(IChartManager *pIChartManager, IChartOCX *pIChartOCX, IPacketManager *pIPacketManager)
{
	
	m_pIChartManager = pIChartManager;
	m_pIChartOCX = pIChartOCX;
	m_pIPacketManager = pIPacketManager;	
}
void CWndAssistIndexManager::SetPeriod(long p_lData)
{
	CArray<ST_DATAINDEX,ST_DATAINDEX>* pArray = (CArray<ST_DATAINDEX,ST_DATAINDEX>*)p_lData; //Index: 0 전체 , 1~10: 분할자
	m_StDataIndex.Copy(*pArray);
}
//분할 적용 계산 처리 : 2006.11.08
void CWndAssistIndexManager::SetOnlyDealSignal(long p_lData)
{
	if( !m_pIChartManager || !m_pIChartOCX || !m_pIPacketManager) return;
	CIDataItem *pDataItem = NULL;

	//IPacket *pIPacketTime	= m_pIPacketManager->GetPacket(m_strRQ, "자료일자");	// 사용되고 있는 RQ :복수종목 - ojtaso (20070307)
	IPacket *pIPacketTime	= m_pIPacketManager->GetPacket(m_strRQ, PACKET_TIME);
	ST_DRAWINFO_EX* pInfo = (ST_DRAWINFO_EX*)p_lData;
	CDealDataItem *pDealDataItem = (CDealDataItem *)pInfo->pDealDataItem;
	ASSERT(pDealDataItem);

	int nTotalDataCnt = 0;
	if(pIPacketTime)
	{
		nTotalDataCnt = pIPacketTime->GetDataCount();
		pIPacketTime->Release();
		pIPacketTime = NULL;

		if(nTotalDataCnt == 0)
		{
			AfxMessageBox("자료일자 데이터가 없습니다.");
			return;
		}
	}
	else
		return;
	

	if(m_bSetDataFirst)
	{
		if(pDealDataItem)
		{
			//-------- Draw하기 위해 신호정보 셋팅 (조회) --------
			ReadConfig_ForcedLiquidation(pInfo->STPath);//강제청산 적용키 위해 호출 
			if(!pInfo->bIsInRealTime)
			{
				//-------- 전략신호정보 초기화 -------- //
				InitStrategySignalInfo(pDealDataItem);
				m_arrayDealPosition.Copy(*pInfo->pArDealPosition);
				m_arrayDealName.Copy(*pInfo->pArDealName);
				m_arrayDealDrawPosition.Copy(*pInfo->pArDealDrawPosition);

				if(!m_StDataIndex.GetSize() == 0)
				{  
					ST_DATAINDEX stDataIndex;
					int nSize = m_StDataIndex.GetSize();
					BOOL bIsDivRate = FALSE;//중첩인지 아닌지..
					if(nSize>2)//중첩 상태인지 아닌지 점검 :KHD : 11.10
					{
						ST_DATAINDEX stFirst   = m_StDataIndex.GetAt(1);
						ST_DATAINDEX stSecond = m_StDataIndex.GetAt(2);
						if(stFirst.nEnd > stSecond.nStart)// 2분할만 비교
							bIsDivRate = TRUE; 
					}
					//현재 신호 표시를 가지고 있는놈이다..
					CDealData *pDealData = NULL;
					CDealData *pFirstDealData = NULL;
					CDealData *pSecondDealData = NULL;
					POSITION pos = pDealDataItem->GetHeadPosition();
					int nPos = 2;
					stDataIndex = m_StDataIndex.GetAt(1);
					while(pos)
					{
						pFirstDealData = pDealDataItem->GetNext(pos);
						if(pFirstDealData->m_lDealValue & VALSELL)//청산 쪽이라면 한 번더..
							continue;
						if(pos==NULL) break;
						pSecondDealData = pDealDataItem->GetNext(pos);
						
						if(stDataIndex.nEnd < pSecondDealData->m_nIndex && nPos<nSize)//분할에서 청산이 넘었다면..
						{  //해당 청산을 인덱스를 지우고 마지막날 인덱스로 청산 신호를 넣는다.
							if(pFirstDealData->m_nIndex < stDataIndex.nEnd)
							{
								if(!bIsDivRate)//중첩이라면..:KHD : 11.10
								{
									//pSecondDealData->m_lDealValue = NULL; 제거..
									m_arrayDealDrawPosition.SetAt(pSecondDealData->m_nIndex,0);
								}
								m_arrayDealDrawPosition.SetAt(stDataIndex.nEnd,	m_DataItemHigh.GetAt(stDataIndex.nEnd));
								m_arrayDealPosition.SetAt(stDataIndex.nEnd,VALSELL);
							}
							stDataIndex = m_StDataIndex.GetAt(nPos);
							++nPos;
						}
					}

					//KHD : 중첩시 오류 발생 막기!!!!
					m_arrayDivideDealDrawPosition.Copy(m_arrayDealDrawPosition);
					m_arrayDivideDealPosition.Copy(m_arrayDealPosition);
					m_arrayDivideDealName.Copy(m_arrayDealName);
					//pInfo->pArDealPosition = &m_arrayDealPosition;
					//pInfo->pArDealName    = &m_arrayDealName;
					//pInfo->pArDealDrawPosition = &m_arrayDealDrawPosition ; 
	
				}
			}
		}
	}
}
//KHD END
///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	매매신호표시형태를 구성한다. : 여기서 화살표 및 구분자를 둬서 세팅을 하네요...
//////////////////////////////////////////////////////////////////////////////
long CWndAssistIndexManager::MakeDealShape(double dArrawWidth,CRgn &rShape,long lShape, long lSellBuy)
{
	//KHD : 화살표 Design 수정 
	long	lHeight = 0;
	CPoint point[8];
	double dArrawWidth_divide2 = dArrawWidth/2 +1;
	double dArrawWidth_divide6 = dArrawWidth/6 +1;
	switch(lShape*10+lSellBuy)
	{
	case 0:	// Arrow Sell :
		if(dArrawWidth<6)
		{
			dArrawWidth = 6;
		}
		point[0].x = 0;
		point[0].y = 0 - 5;
		point[1].x = long( 0 - dArrawWidth_divide2);
		point[1].y = long( 0 - 5 - (dArrawWidth)*2/3);
		point[2].x = long( 0 - dArrawWidth_divide6-1);
		point[2].y = point[1].y;
		point[3].x = point[2].x;
		point[3].y = long( 0 - 5 - (dArrawWidth)*1.5);
		point[4].x = long( 0 + dArrawWidth_divide6+1);
		point[4].y = point[3].y;
		point[5].x = point[4].x;
		point[5].y = long( 0 - 5 - (dArrawWidth)*2/3);
		point[6].x = long( 0 + dArrawWidth_divide2);
		point[6].y = point[5].y;
		point[7] = point[0];
		rShape.CreatePolygonRgn(point, 8, WINDING);
		lHeight = long( 0 - 5 - (dArrawWidth)*2);
		break;
	case 1:	// Arrow Buy
		if(dArrawWidth<6)
		{
			dArrawWidth = 6;
		}
		point[0].x = 0;
		point[0].y = 0 + 5;
		point[1].x = long( 0 - dArrawWidth_divide2);
		point[1].y = long( 0 + 5 + (dArrawWidth)*2/3);
		point[2].x = long( 0 - dArrawWidth_divide6-1);
		point[2].y = point[1].y;
		point[3].x = point[2].x;
		point[3].y = long( 0 + 5 + (dArrawWidth)*1.5);
		point[4].x = long( 0 + dArrawWidth_divide6+1);
		point[4].y = point[3].y;
		point[5].x = point[4].x;
		point[5].y = long( 0 + 5 + (dArrawWidth)*2/3);
		point[6].x = long( 0 + dArrawWidth_divide2);
		point[6].y = point[5].y;
		point[7] = point[0];
		rShape.CreatePolygonRgn(point, 8, WINDING);
		lHeight = long( 0 + 5 + (dArrawWidth)*2);
		break;
	case 10:	// triangle Sell
		if(dArrawWidth<6)
		{
			dArrawWidth = 6;
		}
		point[0].x = 0;
		point[0].y = 0 - 5;
		point[1].x = long( 0 - dArrawWidth_divide2);
		point[1].y = long( 0 - 5 - (dArrawWidth)*2/3);
		point[2].x = long( 0 + dArrawWidth_divide2);
		point[2].y = point[1].y;
		point[3] = point[0];
		rShape.CreatePolygonRgn(point, 4, WINDING);
		lHeight = long( 0 - 5 - (dArrawWidth)*2/3);
		break;
	case 11:	// triangle buy
		if(dArrawWidth<6)
		{
			dArrawWidth = 6;
		}
		point[0].x = 0;
		point[0].y = 0 + 5;
		point[1].x = long( 0 - dArrawWidth_divide2);
		point[1].y = long( 0 + 5 + (dArrawWidth)*2/3);
		point[2].x = long( 0 + dArrawWidth_divide2);
		point[2].y = point[1].y;
		point[3] = point[0];
		rShape.CreatePolygonRgn(point, 4, WINDING);
		lHeight = long( 0 + 5 + (dArrawWidth)*2/3);
		break;
	case 20://속이 안찬 삼각형..
		if(dArrawWidth<6)
		{
			dArrawWidth = 6;
		}
		point[0].x = 0;
		point[0].y = 0 - 5;
		point[1].x = long( 0 - dArrawWidth_divide2);
		point[1].y = long( 0 - 5 - (dArrawWidth)*2/3);
		point[2].x = long( 0 + dArrawWidth_divide2);
		point[2].y = point[1].y;
		point[3] = point[0];
		rShape.CreatePolygonRgn(point, 4, WINDING);
		lHeight = long( 0 - 5 - (dArrawWidth)*2/3);

		break;
	case 21: //속이 안찬 삼각형
		if(dArrawWidth<6)
		{
			dArrawWidth = 6;
		}
		point[0].x = 0;
		point[0].y = 0 + 5;
		point[1].x = long( 0 - dArrawWidth_divide2);
		point[1].y = long( 0 + 5 + (dArrawWidth)*2/3);
		point[2].x = long( 0 + dArrawWidth_divide2);
		point[2].y = point[1].y;
		point[3] = point[0];
		rShape.CreatePolygonRgn(point, 4, WINDING);
		lHeight = long( 0 + 5 + (dArrawWidth)*2/3);

		break;
	case 40:	// Circle Sell : 하얀 삼각형
		if(dArrawWidth*0.7<1)
		{
			dArrawWidth = 1/0.7;
		}
		point[0].x = long( 0 - dArrawWidth*0.7);
		point[0].y = long( 0 - dArrawWidth*2*0.7 - 5);
		point[1].x = long( 0 + dArrawWidth*0.7);
		point[1].y = 0 - 5;
		rShape.CreateEllipticRgn(point[0].x,point[0].y,point[1].x,point[1].y);
		lHeight = long( 0 - dArrawWidth*2*0.7 - 5);
		break;
	case 41:	// Circle Buy
		if(dArrawWidth*0.7<1)
		{
			dArrawWidth = 1/0.7;
		}
		point[0].x = long( 0 - dArrawWidth*0.7);
		point[0].y = long( 0 + 5);
		point[1].x = long( 0 + dArrawWidth*0.7);
		point[1].y = long( 0 + dArrawWidth*2*0.7 + 5);
		rShape.CreateEllipticRgn(point[0].x,point[0].y,point[1].x,point[1].y);
		lHeight = long( 0 + dArrawWidth*2*0.7 + 5);
		break;
	case 30:	// rectangle Sell
		if(dArrawWidth*0.7<1)
		{
			dArrawWidth = 1/0.7;
		}
		rShape.CreateRectRgn( long( 0 - dArrawWidth/2), -5 - ( long)dArrawWidth, long( dArrawWidth/2), -5);
		lHeight = long( 0 - 5 - long(dArrawWidth));
		break;
	case 31:// rectangle Buy
		if(dArrawWidth*0.7<1)
		{
			dArrawWidth = 1/0.7;
		}
		rShape.CreateRectRgn( long( 0 - dArrawWidth/2), + 5 , long( dArrawWidth/2), 5 + ( long)dArrawWidth);
		lHeight = long( 0 + 5 + long(dArrawWidth));
		break;	
	}
	return lHeight;
}

int CWndAssistIndexManager::GetXPositoin(double dDataOffset, double dCandleWidth, CRect rctGraphRegion,int &nLeftBorder,int &nRightBorder)
{
	int	nDataOffset, nXPos, nAdd;
	double dDecimalFrac;

	nDataOffset = int(dDataOffset);
	dDecimalFrac = dDataOffset - nDataOffset;

	// 봉의 좌경계를 구한다. (실 봉의 영역보다 1 Pixcel(여백) 이후이다.)
	nLeftBorder = rctGraphRegion.left + int( dCandleWidth * double( nDataOffset) + 0.5) + 1;
	
	// 봉의 우경계를 구한다. (다음 실 봉의 좌경계보다 1 Pixcel(여백) 이전이다.)
	nRightBorder = rctGraphRegion.left + int( dCandleWidth * double( nDataOffset + 1) + 0.5) - 1;

	// 소수점에 의해 추가될 Pixel
	nAdd = int(dDecimalFrac * dCandleWidth);

	// 봉의 중심을 잡는다. (버림으로 계산한다.)
	nXPos = ( nLeftBorder + nRightBorder) / 2 + nAdd;

	return nXPos;
}


CString CWndAssistIndexManager::MakePacketOption(LPCTSTR lpPacketName,long lPacketSize,LPCTSTR lpPacketType, BOOL bUseInReal)
{
	char szTemp[64];
	CString strPacket;
	strPacket = "1;";
	strPacket += lpPacketName; // 사용할 Packet명
	strPacket += ";;";
	sprintf(szTemp,"%d",lPacketSize);
	strPacket += szTemp;
	strPacket += ";";
	strPacket += lpPacketType;
	strPacket += ";";
	if(bUseInReal)
	{
		strPacket += "_Yes_;\r\n";
	}
	else
	{
		strPacket += "_No_;\r\n";
	}
	return strPacket;
}



CString CWndAssistIndexManager::MakeGraphOption(LPCTSTR lpChartType,LPCTSTR lpChartName, CStringArray& rArrayPacketList, LPCTSTR lpChartTitle, long lChartStyle /*=0*/, long lLineStyle/*=0*/,
									COLORREF clrGrp1 /*= RGB(218,46,46)*/, COLORREF clrGrp2 /*= RGB(218,46,46)*/,
									COLORREF clrGrp3 /*= RGB(46,46,218)*/, COLORREF clrGrp4 /*= RGB(46,46,218)*/,
									COLORREF clrGrp5 /*= RGB(218,46,46)*/, COLORREF clrGrp6 /*= RGB(218,46,46)*/,
									BOOL bUpFilled /*= 0*/, BOOL bDownFilled /*= 1*/,
									long lUpLineWidth /*= 1*/, long lDownLineWidth /*= 1*/, BOOL bHideGrp /*= 0*/)
{
	CString strChartType = lpChartType;
	CString tempStr;
	CString strGraphOption("Graph;\r\n");
	strGraphOption += lpChartType;
	strGraphOption += ";";
	strGraphOption +=lpChartName;
	strGraphOption += ";";
	LONG lSize = rArrayPacketList.GetSize();
	for(long lIndex=0;lIndex<lSize;lIndex++)
	{
		if(lIndex)	strGraphOption += ",";
		strGraphOption += rArrayPacketList.GetAt(lIndex);		
	}
	if(strChartType==g_strChartType[0])
	{
		strGraphOption += ";0;0;0;0;1;0";
	}
	strGraphOption += ";\r\n";

	// "그래프타입;그래프스타일;그리는스타일;이름;title;숨김;색1;색2;색3;색4;색5;색6;
	//  상승채움;하락채움;선굵기1;선굵기2;선굵기3;강조;\r\n"
	
	if(strChartType==g_strChartType[0])
	{
		strGraphOption += "0;";
	}
	else if(strChartType==g_strChartType[1])
	{
		strGraphOption += "1;";
	}
	else if(strChartType==g_strChartType[2])
	{
		strGraphOption += "2;";
	}
	else
	{
		ASSERT(0); // Error !! 
	}
	tempStr.Format("%ld;",lChartStyle);
	strGraphOption += tempStr;
	tempStr.Format("%ld;",lLineStyle);	
	strGraphOption += tempStr; 
	strGraphOption += lpChartName;
	strGraphOption += ";";
	strGraphOption += lpChartTitle;
	tempStr.Format(";0;%ld,%ld,%ld;%ld,%ld,%ld;%ld,%ld,%ld;%ld,%ld,%ld;%ld,%ld,%ld;%ld,%ld,%ld;%ld;%ld;%ld;%ld;%ld;\r\n",
		GetRValue(clrGrp1),GetGValue(clrGrp1), GetBValue(clrGrp1),
		GetRValue(clrGrp2),GetGValue(clrGrp2), GetBValue(clrGrp2),
		GetRValue(clrGrp3),GetGValue(clrGrp3), GetBValue(clrGrp3),
		GetRValue(clrGrp4),GetGValue(clrGrp4), GetBValue(clrGrp4),
		GetRValue(clrGrp5),GetGValue(clrGrp5), GetBValue(clrGrp5),
		GetRValue(clrGrp6),GetGValue(clrGrp6), GetBValue(clrGrp6),
		bUpFilled,bDownFilled,
		lUpLineWidth,lDownLineWidth,bHideGrp);
	strGraphOption += tempStr;
	
	strGraphOption += "GraphEnd;\r\n";	
	return strGraphOption;
}


BOOL CWndAssistIndexManager::CopyPlotDataToPacket(IPacket *pIPacketDataItem, CIDataItem *pDataItem, LPCTSTR lpPacketName, long lSize, BOOL bIsInRealTime)
{
	double dValue = 0.;
	long lFirstDataIndex = 0;
	int nIndex = 0;
	if(!bIsInRealTime)
	{
		pIPacketDataItem->ClearData(true);
		int nCnt = pIPacketDataItem->GetDataCount();
		lFirstDataIndex = pDataItem->GetFirstDataIndex()-1;
		if(lFirstDataIndex>lSize)	lFirstDataIndex = lSize;
		for( nIndex = 0;nIndex<lFirstDataIndex;nIndex++)
		{
			pIPacketDataItem->AppendData(0.);
		}
		// Y축 환경설정에서 전체데이터 최고저 따라기기일때 최소값이 0나오는 버그 수정 - ojtaso (20071129)
		m_pIChartOCX->SetStartIndexOfPacket(m_strRQ, lpPacketName,pDataItem->GetFirstDataIndex(),FALSE);
		for(nIndex = lFirstDataIndex;nIndex<lSize;nIndex++)
		{
			pDataItem->GetAt(nIndex,&dValue);					
			pIPacketDataItem->AppendData(dValue);
		}
		m_pIChartOCX->SetStartIndexOfPacket(m_strRQ, lpPacketName,pDataItem->GetFirstDataIndex(),TRUE);
	}
	else
	{		
		pDataItem->GetAt(lSize-1,&dValue);
		int nCnt = pIPacketDataItem->GetDataCount();
		if(lSize==nCnt)
		{				
			pIPacketDataItem->SetData(nCnt-1,dValue);
		}
		else
		{
			pIPacketDataItem->AppendData(dValue);					
		}	
	}
	return TRUE;
}

void CWndAssistIndexManager::DeleteSTInfo()
{
	CString strGraphName, strPacketName;
	POSITION pos = m_listGraph.GetHeadPosition();
	while(pos)
	{
		 strGraphName = m_listGraph.GetNext(pos);
		 m_pIChartOCX->RemoveGraph(strGraphName);
	}
	m_listGraph.RemoveAll();

	pos = m_listPacket.GetHeadPosition();
	while(pos)
	{
		 strPacketName = m_listPacket.GetNext(pos);
		 m_pIChartOCX->RemovePacket(strPacketName,false);
	}
	m_listPacket.RemoveAll();

	m_arrayDealDrawPosition.RemoveAll();
	m_arrayDealName.RemoveAll();
	m_arrayDealVol.RemoveAll();
	m_arrayDealPosition.RemoveAll();
	m_arrayBullBearPosition.RemoveAll();	

	if(m_pSTItem)
	{
		m_pSTItem->m_DllSTLoader.Stop();
		delete m_pSTItem;
		m_pSTItem = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	시스템 트레이딩의 환경설정창을 띄운다.
//////////////////////////////////////////////////////////////////////////////
BOOL CWndAssistIndexManager::OnLButtonDblClk( UINT nFlags, CPoint &point)
{
	// 정의된 기능없음 - ojtaso 061009
	return FALSE;

	if(m_rectTitle.PtInRect(point))
	{
		if(m_pSTItemCfg)
		{
			if(m_pSTItemCfg->m_strSTItemName!=m_pSTItem->m_strSTItemName)
			{
				delete m_pSTItemCfg;
				m_pSTItemCfg = new CIndexItem;
				m_pSTItemCfg->m_strSTItemName = m_pSTItem->m_strSTItemName;
				m_pSTItemCfg->m_strPath = m_pSTItem->m_strPath;
				m_pSTItemCfg->m_indexType = STCFG;
			}			
		}
		else
		{
			m_pSTItemCfg = new CIndexItem;
			m_pSTItemCfg->m_strSTItemName = m_pSTItem->m_strSTItemName;
			m_pSTItemCfg->m_strPath = m_pSTItem->m_strPath;
			m_pSTItemCfg->m_indexType = STCFG;
		}
		CString strOption;
		strOption.Format("%s=%s",LOAD_MODE,MODE_TEST);
		long lRetValue = m_pSTItemCfg->m_DllSTLoader.Load(m_pSTItemCfg->m_strPath,this->GetSafeHwnd(), (long)m_pSTItemCfg,NULL,0,strOption);
		if(lRetValue==-1)
		{
			delete m_pSTItemCfg;
			m_pSTItemCfg = NULL;
			return FALSE;
		}
		m_pSTItemCfg->m_DllSTLoader.Stop();

#if _PLOT_LOG
		// (2006/3/30 - Seung-Won, Bae) Log Title for Indicator Dump Data
		CString strBuffer;
		strBuffer.Format( "====================================================[%s] : OnLButtonDblClk \r\n", m_pSTItemCfg->m_strSTItemName);
		CDumpLogger::LogStringToFile( NULL, strBuffer, strBuffer.GetLength(), "IndiValue");
#endif

		m_pSTItemCfg->m_DllSTLoader.Execute(NULL);// RMSG_STCONTROLLER_POINTER를 받아서 설정창을 실행한다.
		return TRUE;
	}
	return FALSE;
}

LRESULT CWndAssistIndexManager::OnSTController( WPARAM wParam, LPARAM lParam) 
{
	CISTControllerD *pISTControllerD = (CISTControllerD *)lParam;	
	pISTControllerD->SetConfig();
	return S_OK;
}

///////////////////////////////////////////////////////////////////////////////
//	author	:	Geun-moon Oh
//	E-Mail	:	gm0604@dooriic.co.kr
//	date	:	2005.08.18
//	Object	:	시스템트레이딩환경설정창에서 수정된 부분을 반영한다. 
//////////////////////////////////////////////////////////////////////////////
LRESULT CWndAssistIndexManager::OnUpdateSTSfg( WPARAM wParam, LPARAM lParam)
{
	if(lParam==1)// Set Ok...
	{
		LPCTSTR  lpSfgName = (LPCTSTR)wParam;
		CString strLastSTName = m_pSTItem->m_strSTItemName;
		CString strLastSTPath = m_pSTItem->m_strPath;
		DeleteSTInfo();
		AddST(strLastSTName,strLastSTPath);		
		m_pIChartOCX->InvalidateControl();
	}	
	return S_OK;
}

void CWndAssistIndexManager::SetChartDivideInfo(int nDivideState, int nUnitPos)
{
	// 분할 상태 : m_nDivideState
	// HORZ_NOTYPE,		// 없음
	// HORZ_DAILY,		// 일간
	// HORZ_WEEKLEY,	// 주간
	// HORZ_MONTHLY,	// 월간
	// HORZ_TIME_ONE,	// 시간-한줄
	// HORZ_TICK,		// 틱
	// HORZ_TIME_TWO,	// 시간-두줄
	// HORZ_TEXT,		// text
	// HORZ_LEFTRIGHT,	// 좌우식
	// HORZ_PRICEZONE,	// 가격대
	// HORZ_DATA,		// data(거래량 포함)
	// HORZ_MARKETPROFILE, // MarketProfile
	// HORZ_TIME_SECOND	// 초

	// 분할 하위 시간구분 : m_nUnitPos
	m_nDivideState = nDivideState;
	m_nUnitPos = nUnitPos;
}

// [WID_060913_A1]
//	Return값의미 :	0인경우는 실행중인 전략의 삭제일때임,
//					마이너스값은 에러, 플러스는 성공
int CWndAssistIndexManager::LoadST(LPCSTR szSTFile, long pChartInterface, LPCSTR szCode, int nCount, LPCSTR szEndDate, CStringArray &stArrayPath,CString szSTName)
{
	// 실행중인 지표, 강세약세가 있으면 닫는다.
	if(m_pSTItem)
	{
		DeleteSTInfo();
	}
	if(m_pStrategyItem)
	{
		m_CriticalSection.Lock();
		CString szOldSTFile = m_szMSTName;
		DestroyStrategyItem();
		ClearData();
		m_CriticalSection.Unlock();

		if(szOldSTFile.CompareNoCase(szSTFile)==0){		
			return 0;		// 현재 실행중이면 전략실행을 멈춘다.
		}
	}

	if(!m_pIPacketTime||(!m_pIPacketOpen&&!m_pIPacketHigh&&!m_pIPacketLow&&!m_pIPacketClose&&!m_pIPacketVolume))
	{
		CheckRawData();
	}

	if(!m_pStrategyItem)
	{
		m_pstrategyLoader = new CStrategyLoader;
		//m_pstOrderData = new STORDER_INDATA;
		m_pStrategyItem = m_pstrategyLoader->GetStrategyItem();// 메모리 생성 
		if(m_pStrategyItem == NULL) return -1;
		m_pIndexItem = new CIndexItem;
		m_pIndexItem->m_indexType = MSTIndex;
		int nRetValue = m_pStrategyItem->Create(this, (long)m_pIndexItem, stArrayPath, pChartInterface);
		if(nRetValue==-1) 
		{
			m_pStrategyItem->DestoryWindow();
			delete m_pStrategyItem;
			m_pStrategyItem = NULL;
			return 0;
		}
	}

	// 차트 시간구분 정보 셋팅
	m_pStrategyItem->SetChartDivideInfo(m_nDivideState, m_nUnitPos);

	//LoadForRun종목명(String), 데이터갯수(int),날짜(String)
	BOOL bRetValue = m_pStrategyItem->LoadST(szSTFile, szCode, nCount, szEndDate);
	m_stCode = szCode;
	if(bRetValue)
	{
		m_szMSTName=szSTFile;
		m_szSTName.Format("%s",szSTName);// = szSTName;//KHD : 2006.11.16 
		ReadConfiguration(szSTFile);
		m_pStrategyItem->Run();
		m_bSinHo = TRUE;
		ISTManager* pMng = (ISTManager*)AfxGetPctrInterface(UUID_ISTManager);
		if(pMng) pMng->AddSTRunINfo(m_szMSTName, TRUE);
	}
//	m_pdlgInfo->ApplyST();
	return 1;
}

void CWndAssistIndexManager::ReadConfiguration(CString szPath)
{
	m_STConfigInfo.LoadInfo(szPath);
	
	//-------- 신호 설정내용 -------- 
	// 신호설정은 LoadEnvDealPos 함수를 통해서 전략실행 결과를 통해 얻음
	//-------- 강제청산 설정내용 --------
	ReadConfig_ForcedLiquidation(szPath);
	//-------- 운용 설정내용 --------
	ReadConfig_Setup(szPath);
	//-------- 주문 설정내용 --------
	ReadConfig_Order(szPath);
}

void CWndAssistIndexManager::ReadConfig_ForcedLiquidation(CString szPath)
{
	CString szKey, szTmp;
	COLORREF clrSignal = RGB(0,0,0);
	int nSel;
	szTmp.GetBufferSetLength(MAX_PATH);

	// 강제청산시점 (0 : 조건 만족시 / 1 : 봉완성시)
	m_nTiming_ForcedLiquidation = ::GetPrivateProfileInt(STCONFIG_CLEAR, CLEARPOINT, 0, szPath);

	// 손절매(손실률)
	// Shape
	nSel = GetPrivateProfileInt(STCONFIG_CLEAR, LOSSRATESHAPE, 0, szPath);
	m_stLogoSon = GetOptionLogo(nSel);

	// Color
	GetPrivateProfileString(STCONFIG_CLEAR, LOSSRATECOLOR, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	clrSignal = atol(szTmp);
	if(clrSignal == 0)
		clrSignal = SIGNALBUYDEFAULT;
	m_ColorSon = clrSignal;

	// 목표수익	
	// Shape
	nSel = GetPrivateProfileInt(STCONFIG_CLEAR, TARGETBENEFITSHAPE, 0, szPath);
	if(nSel < 0 ) nSel = 0;
	m_stLogoSooik = GetOptionLogo(nSel);

	// Color
	GetPrivateProfileString(STCONFIG_CLEAR, TARGETBENEFITCOLOR, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	clrSignal = atol(szTmp);
	if(clrSignal == 0)
		clrSignal = SIGNALBUYDEFAULT;
	m_ColorSooik = clrSignal;

	// 최고가대비 하락
	// Shape
	nSel = GetPrivateProfileInt(STCONFIG_CLEAR, HIGHPRICESHAPE, 0, szPath);
	m_stLogoGoga = GetOptionLogo(nSel);

	// Color
	GetPrivateProfileString(STCONFIG_CLEAR, HIGHPRICECOLOR, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	clrSignal = atol(szTmp);
	if(clrSignal == 0)
		clrSignal = SIGNALBUYDEFAULT;
	m_ColorGoga = clrSignal;

	//// 당일 저가대비 상승
	//// Shape
	//szKey.Format("%s", "S4");
	//nSel = GetPrivateProfileInt(STCONFIG_CLEAR, szKey, 0, szPath);
	//m_stLogoJuga = GetOptionLogo(nSel);

	//// Color
	//szKey.Format("CLR_%s", "4");
	//GetPrivateProfileString(STCONFIG_CLEAR, szKey, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	//clrSignal = atol(szTmp);
	////if(clrSignal == 0)
	////	clrSignal = SIGNALBUYDEFAULT;
	//m_ColorJuga = clrSignal;

	// 목표가보존청산
	// Shape
	nSel = GetPrivateProfileInt(STCONFIG_CLEAR, TARGETVALUEPRESERVSHAPE, 0, szPath);
	m_stLogoTarget = GetOptionLogo(nSel);

	// Color
	GetPrivateProfileString(STCONFIG_CLEAR, TARGETVALUEPRESERVCOLOR, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	clrSignal = atol(szTmp);
	if(clrSignal == 0)
		clrSignal = SIGNALBUYDEFAULT;
	m_ColorTarget = clrSignal;

	// 이익보존청산
	// Shape
	nSel = GetPrivateProfileInt(STCONFIG_CLEAR, BENEFITPRESERVSHAPE, 0, szPath);
	m_stLogoBenefit = GetOptionLogo(nSel);

	// Color
	GetPrivateProfileString(STCONFIG_CLEAR, BENEFITPRESERVCOLOR, "0", (LPSTR)(LPCSTR)szTmp, MAX_PATH, szPath);
	clrSignal = atol(szTmp);
	if(clrSignal == 0)
		clrSignal = SIGNALBUYDEFAULT;
	m_ColorBenefit = clrSignal;

	szTmp.ReleaseBuffer();
}

void CWndAssistIndexManager::ReadConfig_Setup(CString szPath)
{
	// 허용포지션 (0 : 매수/매수청산, 1 : 매도/매도청산, 2 : 모두 허용)
	m_nApplyPosition = GetPrivateProfileInt(STCONFIG_SETUP, ALLOWPOSITION, 0, szPath);
}

void CWndAssistIndexManager::ReadConfig_Order(CString szPath)
{
	CString szKey;
	char szTmp[MAX_PATH];
	int nSel;

	// 적용계좌
	if(m_stCode.GetLength()==6)	
		m_pstOrderData->nMarket = 0;				// 주식
	else 
		m_pstOrderData->nMarket = 1;				// 선물

	if(m_szMSTName.Find("\\User\\") < 0)
		m_pstOrderData->nSTType = 0;				// 제공전략
	else
		m_pstOrderData->nSTType = 1;				// 사용자전략

	strcpy(m_pstOrderData->aCode, m_stCode);		// 종목코드
	
	theApp.GetCodeName(m_pstOrderData->nMarket, m_stCode, m_pstOrderData->aCodeName, sizeof(m_pstOrderData->aCodeName));

	::strncpy(m_pstOrderData->szSTName, m_szSTName, m_szSTName.GetLength());

	//int ch = '@';
	int ch = gSTGubunKey;	//090902-solomon @->^

	char* psrc = m_pstOrderData->szSTName;
	char* pdest = ::strchr(m_pstOrderData->szSTName, ch);
	int nFind = pdest - psrc + 1;
	if(nFind>0) 
		::strncpy(m_pstOrderData->szSTName, m_pstOrderData->szSTName, nFind);


	szKey.Format("%s", "Acct");						// 계좌번호 - 차후 계좌번호 얻어오는 루틴 필요
	GetPrivateProfileString(STCONFIG_ORDER, szKey, "", szTmp, MAX_PATH, szPath);
	strcpy(m_pstOrderData->aAcctNo, szTmp);

	// 계좌명
	szKey.Format("%s", "AcctName");
	GetPrivateProfileString(STCONFIG_ORDER, szKey, "", szTmp, MAX_PATH, szPath);
	strcpy(m_pstOrderData->aAcctName, szTmp);

	// 주문수량
	szKey.Format("%s", "JAmount");
	GetPrivateProfileString(STCONFIG_ORDER, szKey, "", szTmp, MAX_PATH, szPath);
	strcpy(m_pstOrderData->aOrderAmt, szTmp);

	// 진입가격
	szKey.Format("%s", "BuyPrice");
	nSel = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szPath);
	m_nJinValue = nSel;

	szKey.Format("%s", "MKPRC_ENT");
	m_bJinMarket = (BOOL)::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szPath);
	m_pstOrderData->nOrderPriceTypeEnt = m_bJinMarket;

	// 청산가격
	szKey.Format("%s", "SellPrice");
	nSel = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szPath);
	m_nChungValue = nSel;
	m_pstOrderData->nOrderPriceTypeClr = m_nChungValue;

	szKey.Format("%s", "MKPRC_CLR");
	m_bChungMarket = (BOOL)::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szPath);

	// 주문방식
	szKey.Format("%s", "JMethod");
	m_pstOrderData->nAutoOrder = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szPath);
	
	// 주문시점 (0 : 조건 만족시 / 1 : 봉완성시)
	szKey.Format("%s", "JTime");
	nSel = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szPath);
	m_nTiming_Order = nSel;

	// 신호발생음(진입신호) 사용여부
	szKey.Format("%s", "Sound_Buy_Use");
	m_bUseEntrySound = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szPath);

	// 신호발생음(진입신호) 소리
	szKey.Format("%s", "Sound_Buy_File");
	GetPrivateProfileString(STCONFIG_ORDER, szKey, "", szTmp, MAX_PATH, szPath);
	//sprintf(m_szEntrySound,(LPSTR)(LPCSTR)szTmp);
	m_szEntrySound = szTmp;

	// 신호발생음(청산신호) 사용여부
	szKey.Format("%s", "Sound_Sell_Use");
	m_bUseExitSound = ::GetPrivateProfileInt(STCONFIG_ORDER, szKey, 0, szPath);

	// 신호발생음(청산신호) 소리
	szKey.Format("%s", "Sound_Sell_File");
	GetPrivateProfileString(STCONFIG_ORDER, szKey, "", szTmp, MAX_PATH, szPath);
	m_szExitSound = szTmp;

//	//비밀번호 세팅..
//	IMainInfoManager* pMainMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
//	if(m_pstOrderData->nAutoOrder!=0)
//	{
//		m_szPW			  = pMainMng->GetMainData("PW",NULL);
//		pMainMng->GetMainData("SETPW",NULL);// 비밀번호를 전역에서 NULL 시킨다. 중복되지 않도록...
//	}else
//	{
//		pMainMng->GetMainData("SETPW",NULL);
//		m_szPW="";
//	}
}

//청산시에 필요한 정보를 세팅하는 기호
CString CWndAssistIndexManager::GetOptionLogo(int nSel)
{
	//"★", "☆", "♣", "♧", "◈", "▣", "■", "□"
	CString stLogo;
	switch(nSel) {
	case 0:
		stLogo ="★";
		break;
	case 1:
		stLogo ="☆";
		break;
	case 2:
		stLogo ="♣";
		break;
	case 3:
		stLogo ="♧";
		break;
	case 4:
		stLogo ="◈";
		break;
	case 5:
		stLogo ="▣";
		break;
	case 6:
		stLogo ="■";
		break;
	case 7:
		stLogo ="□";
		break;
	}
	return stLogo;
}
void CWndAssistIndexManager::DestroyStrategyItem()
{
	if(m_pIndexItem)
	{
		delete m_pIndexItem;
		m_pIndexItem = NULL;
	}

	if(m_pStrategyItem)
	{
		ISTManager* pMng = (ISTManager*)AfxGetPctrInterface(UUID_ISTManager);
		if(pMng) pMng->AddSTRunINfo(m_szMSTName, FALSE);

		m_pStrategyItem->Stop();
		if(m_pStrategyItem->GetSafeHwnd()) 
			m_pStrategyItem->DestoryWindow();
		delete m_pStrategyItem;
		m_pStrategyItem = NULL;
		m_szMSTName = _T("");
		m_hSysTabEx = NULL;		//@@@@@06.11.20 HWND
		if(m_pstrategyLoader)//KHD: 포인터로 바꿈..
		{
			delete m_pstrategyLoader;
			m_pstrategyLoader = NULL;
		}
	}

}

// 사용자가 전략적용을 할 경우 파일을 사용자폴더에 저장하고 않고 처리하기 때문에 추후에 다시 백업처리를 한다.
void CWndAssistIndexManager::STBackup(LPCSTR szMSTName)
{
	char szTmp[MAX_PATH]={0,};
	if(::GetPrivateProfileString(STRATEGYLIST, "IMSIST", "", szTmp, MAX_PATH, szMSTName)>0)
	{
		::WritePrivateProfileString(STRATEGYLIST, "IMSIST", "", szTmp);
        ::CopyFile(szTmp, szMSTName, FALSE);
		::DeleteFile(szTmp);
	}
}

void CWndAssistIndexManager::QueryPrice(STORDER_INDATA* lpOrderData, UINT nOrderIndex)
{
//@Solomon 091012
// 		WORD wFID[] = {15001, 14531, 14532, 14533, 14501, 14502, 14503, 16117, 16118};
// 
// 		CString strTR;
// 		strTR.Format(_T("%05d"), nOrderIndex);
// 
// 		m_pITrComm->SetTR(strTR);
// 		m_pITrComm->AddMainKey(DI_CODE, lpOrderData->aCode, C_DT_STRING, ::lstrlen(lpOrderData->aCode));
// 
// 		m_pITrComm->AddDataKey(wFID[lpOrderData->nOrderPriceTypeEnt]);
// 		m_pITrComm->AddDataKey(wFID[lpOrderData->nOrderPriceTypeClr]);
// 		m_pITrComm->Send2Server("", 0, FALSE);
}

BOOL IsAvailableOrderData(STORDER_INDATA* lpOrderData)
{
	if(!::lstrlen(lpOrderData->aAcctNo))
		return FALSE;

	if(!::lstrlen(lpOrderData->aCode))
		return FALSE;

	if(atoi(lpOrderData->aOrderAmt) <= 0)
		return FALSE;

	return TRUE;
}

extern long KSLink_GetEnvInfo(int nType);
#define		UM_SM_SIGNALORDER_MSG		(WM_USER+551)
const UINT RMSG_INDEXADDIN_NOTIFY_BUYSELL = ::RegisterWindowMessage("RMSG_INDEXADDIN_NOTIFY_BUYSELL");
const UINT RMSG_INDEXADDIN_NOTIFY_SIGNAL = ::RegisterWindowMessage("RMSG_INDEXADDIN_NOTIFY_SIGNAL");
void CWndAssistIndexManager::OrderSign(STORDER_INDATA* lpOrderData)
{
	if(!IsAvailableOrderData(lpOrderData))
		return;

	// 0:주식, 1:선옵 일때만 주문 처리를 한다.
	if(m_STConfigInfo.m_nCodeType==0 ||
		m_STConfigInfo.m_nCodeType==1)
	{
		UINT uMsg;
		// m_nJMethod==0 이면 가상적용
		if(m_STConfigInfo.m_InfoJumun.m_nJMethod==0)
			uMsg = RMSG_INDEXADDIN_NOTIFY_SIGNAL;
		else
			uMsg = RMSG_INDEXADDIN_NOTIFY_BUYSELL;

		m_STConfigInfo.m_nOrderType = lpOrderData->nOrderType;

		::SendMessage(m_hSysTabEx, uMsg, (WPARAM)m_pstOrderData, (LPARAM)&m_STConfigInfo);
	}
	
//@유진처리
//	HWND m_hSignalWnd;
//	m_hSignalWnd = (HWND)KSLink_GetEnvInfo(2);
//	if(m_hSignalWnd == NULL)
//		return;
//	
//	//주문창 뜨기 전이나.. 자동 주문시에 소리가 나게 설정 : by KHD
//	if(lpOrderData->nOrderType & VALENTRYBUY)//매수진입
//	{
//		if(m_bUseEntrySound && !m_szEntrySound.IsEmpty())
//			sndPlaySound(m_szEntrySound, SND_ASYNC);
//	}
//	else if(lpOrderData->nOrderType & VALEXITLONG )//매수청산 
//	{
//		if(m_bUseExitSound && !m_szExitSound.IsEmpty())
//			sndPlaySound(m_szExitSound, SND_ASYNC);
//	}
//
//	// 주문처리
//	if(lpOrderData->nAutoOrder == 1)//반자동
//	{
//		SIGNALORDERSINFO signalOrdersInfo;
//		memset(&signalOrdersInfo, 0, sizeof(SIGNALORDERSINFO));
//		signalOrdersInfo.hDLLWnd = (long)GetSafeHwnd();																// 호출한 컨트롤 핸들
//		memcpy(signalOrdersInfo.szIndexNo, "16", sizeof(signalOrdersInfo.szIndexNo)-1);								// DLL Index번호 : 16(시스템트레이딩)
//		memcpy(signalOrdersInfo.szWindName, "차트", sizeof(signalOrdersInfo.szWindName)-1);							// 신호이름 : "차트"
//		memcpy(signalOrdersInfo.szSignalgb, "차", sizeof(signalOrdersInfo.szSignalgb)-1);							// 신호구분 : "차"
//		memcpy(signalOrdersInfo.szPopgb, "2", sizeof(signalOrdersInfo.szPopgb)-1);									// 팝업Type : (2)주문창
//		memcpy(signalOrdersInfo.szActionGb, "1", sizeof(signalOrdersInfo.szActionGb)-1);							// 버튼 처리 구분값 : (1)주문
//		memcpy(signalOrdersInfo.szAcctno, lpOrderData->aAcctNo, sizeof(signalOrdersInfo.szAcctno)-1);				// 계좌번호
//		memcpy(signalOrdersInfo.szCode, lpOrderData->aCode, sizeof(signalOrdersInfo.szCode)-1);						// 종목코드
//		memcpy(signalOrdersInfo.szLoangb, "0", sizeof(signalOrdersInfo.szLoangb)-1);								// 신용구분 : (0)현금
//		memcpy(signalOrdersInfo.szJanGogb, "0", sizeof(signalOrdersInfo.szJanGogb)-1);								// 잔고구분 : (0)전체
//
//		if(lpOrderData->nOrderType & VALENTRYSELL  || lpOrderData->nOrderType & VALENTRYBUY)
//		{
//			memcpy(signalOrdersInfo.szMemegb, "2", sizeof(signalOrdersInfo.szMemegb)-1);							// 매매구분 : (2)매수
//			memcpy(signalOrdersInfo.szOrdgb, lpOrderData->nOrderPriceTypeEnt ? "1" : "0", sizeof(signalOrdersInfo.szOrdgb)-1);		// 주문구분(보통가, 시장가,...)
//		}
//		else if(lpOrderData->nOrderType & VALEXITLONG  || lpOrderData->nOrderType & VALEXITSHORT)
//		{
//			memcpy(signalOrdersInfo.szMemegb, "1", sizeof(signalOrdersInfo.szMemegb)-1);							// 매매구분 : (1)매도
//			memcpy(signalOrdersInfo.szOrdgb, lpOrderData->nOrderPriceTypeClr ? "1" : "0", sizeof(signalOrdersInfo.szOrdgb)-1);		// 주문구분(보통가, 시장가,...)
//		}
//
//		memcpy(signalOrdersInfo.szOrdqtygb, "0", sizeof(signalOrdersInfo.szOrdqtygb)-1);							// 주문수량구분(0:수량, 1:금액, 2:매도가능수량%)
//		memcpy(signalOrdersInfo.szOrdqty, lpOrderData->aOrderAmt, sizeof(signalOrdersInfo.szOrdqty)-1);				// 주문수량
//		memcpy(signalOrdersInfo.szOrdprcgb, "0", sizeof(signalOrdersInfo.szOrdprcgb)-1);							// 주문단가구분(0:지정가...(인덱스))
//		memcpy(signalOrdersInfo.szOrdprc, "0", sizeof(signalOrdersInfo.szOrdprc)-1);								// 주문단가
//		memcpy(signalOrdersInfo.szbContinue, "1", sizeof(signalOrdersInfo.szbContinue)-1);							// 감시 진행여부
//		memcpy(signalOrdersInfo.szMsg, "시스템 주문이 발생하였습니다.", sizeof(signalOrdersInfo.szMsg)-1);			// 감시MSG
//
//		::SendMessage(m_hSignalWnd, UM_SM_SIGNALORDER_MSG, (WPARAM)0L, (long)&signalOrdersInfo);
//	}
//@유진처리
//@유진삭제	
//	if(!m_pITrComm)
//	{
//		IAUTrCommManager* pICommManager = (IAUTrCommManager*)AfxGetPctrInterface(2);
//		m_pITrComm = pICommManager->AdviseTrComm();
//		m_pITrComm->ChangeCallback(&m_xMyITrCommSite);
//
//	}
//
//
//	STORDER_INDATA* pNewOrderData = new STORDER_INDATA;
//	::memcpy(pNewOrderData, lpOrderData, sizeof(STORDER_INDATA));
//
//	UINT nOrderIndex = 0;
//	STORDER_INDATA* pOrderData;
//	while(TRUE)
//	{
//		if(m_mapOrderData.Lookup(nOrderIndex, pOrderData))
//			nOrderIndex++;
//		else
//			break;
//	}
//
//	m_mapOrderData.SetAt(nOrderIndex, pNewOrderData);
//
//	//이쪽 부분에 대해 물어봐야 겠다. 각 시장가들에 대해서 어떤 로직으로 실행 되게 되었는지.... KHD 즉 어떤 경우에 이걸타고 저걸타고...
//	// 주식 매수인경우 시장가로 주문가능 수량 조회 불가 -> 바로 주문(주문가격 조회도 없음)
//	//if((lpOrderData->nMarket == 0 && lpOrderData->nOrderType & VALENTRYBUY) && lpOrderData->nOrderGubun & MARKETENTRY)//진입시장가 체크시
//	//{
//	//	AfxMessageBox("하하하하");
//	//	SendOrderStock(lpOrderData, 0);
//	//}
//	// 시장가인 경우 // 이쪽 부분이 이해가 안간다. ? : KHD 
//	 
//	if(((lpOrderData->nOrderType & VALENTRYSELL  || lpOrderData->nOrderType & VALENTRYBUY) && lpOrderData->nOrderGubun & MARKETENTRY  )||
//	      ((lpOrderData->nOrderType & VALEXITLONG    || lpOrderData->nOrderType & VALEXITSHORT) &&  lpOrderData->nOrderGubun & MARKETCLEAR))
//	{
//		PROCEED_ORDER* pProceedOrderData = new PROCEED_ORDER;
//
//		pProceedOrderData->pThis = this;
//		pProceedOrderData->nOrderIndex = nOrderIndex;
//		pProceedOrderData->nOrderQty = 0;
//		pProceedOrderData->nClearQty = 0;
//		pProceedOrderData->fOrderPriceEnt = 0.;
//		pProceedOrderData->fOrderPriceClr = 0.;
//
//		MakeQueryOrderQty(pOrderData, pProceedOrderData);
//	}
//	else	// 어느 한쪽만이라도 지정가인 경우
//		QueryPrice(lpOrderData, nOrderIndex);
//@유진삭제	
}

void CWndAssistIndexManager::MakeOrderData_Qty_Future(CStringArray& arrOrder, STORDER_INDATA* pOrderData, PROCEED_ORDER* pProceedOrderData)
{
	CString strData;

	arrOrder.Add("1");  			// RECORD : 비반복 입력이므로 “1”
	strData = pOrderData->aAcctNo;
	arrOrder.Add(strData);          // “계좌번호”부터 순서대로 입력된다.

	arrOrder.Add(m_szPW);
	arrOrder.Add("1");				// 일반 : 1
	arrOrder.Add("0");
	arrOrder.Add("0");

	arrOrder.Add(pOrderData->aCode);

	if(pOrderData->nOrderType & VALENTRYSELL || pOrderData->nOrderType & VALEXITLONG)
		arrOrder.Add("1");
	else if(pOrderData->nOrderType & VALENTRYBUY || pOrderData->nOrderType & VALEXITSHORT)
		arrOrder.Add("2");

	// 주문가
	strData.Format(_T("%10.2f"), pProceedOrderData->fOrderPriceEnt);
	arrOrder.Add(strData);

	if(pOrderData->nOrderGubun & MARKETENTRY)
		arrOrder.Add("03");
	else
		arrOrder.Add("00");
}

void CWndAssistIndexManager::MakeOrderData_Qty_Stock(CStringArray& arrOrder, STORDER_INDATA* pOrderData, PROCEED_ORDER* pProceedOrderData)
{
	CString strData;
	arrOrder.Add("1");  			// RECORD : 비반복 입력이므로 “1”

	if(pOrderData->nOrderType & VALEXITLONG)			// 매매구분
		arrOrder.Add("1");
	else if(pOrderData->nOrderType & VALENTRYBUY)
		arrOrder.Add("2");

//	arrOrder.Add("001");

	strData = pOrderData->aAcctNo;
	arrOrder.Add(strData);          // “계좌번호”부터 순서대로 입력된다.

	arrOrder.Add(m_szPW);

	strData.Format(_T("A%s"), pOrderData->aCode);
	arrOrder.Add(strData);

	// 주문가
	strData.Format(_T("%10.2f"), pProceedOrderData->fOrderPriceEnt);
	arrOrder.Add(strData);
	//청산가는?? 언제 세팅??
	arrOrder.Add("00");		// (임시:00) 추후 인터페이스 제공될거라 함
	arrOrder.Add("00");		// (고정:00)
}

void CWndAssistIndexManager::MakeQueryOrderQty(STORDER_INDATA* lpOrderData, PROCEED_ORDER* pProceedOrderData)
{
	CStringArray arrOrder;
	int nQAIndex = -1;

	if(lpOrderData->nMarket == 1)	// 선물
	{
		MakeOrderData_Qty_Future(arrOrder, lpOrderData, pProceedOrderData);
//		nQAIndex = m_pITrComm->Send2ServerServiceMgr(_T("SONBQ101"), &arrOrder);
	}
	else							// 주식
	{
		MakeOrderData_Qty_Stock(arrOrder, lpOrderData, pProceedOrderData);
//		nQAIndex = m_pITrComm->Send2ServerServiceMgr(_T("SONAQ200"), &arrOrder);
	}

	if(nQAIndex >= 0)
		m_mapProcOrderData.SetAt((UINT)nQAIndex, pProceedOrderData);
}

void CWndAssistIndexManager::MakeQueryPrice(STORDER_INDATA* lpOrderData)
{
	STORDER_INDATA* pNewOrderData = new STORDER_INDATA;
	::memcpy(pNewOrderData, lpOrderData, sizeof(STORDER_INDATA));

	UINT nOrderIndex = 0;
	STORDER_INDATA* pOrderData;
	while(TRUE)
	{
		if(m_mapOrderData.Lookup(nOrderIndex, pOrderData))
			nOrderIndex++;
		else
			break;
	}

	m_mapOrderData.SetAt(nOrderIndex, pNewOrderData);

	QueryPrice(lpOrderData, nOrderIndex);
}

int CWndAssistIndexManager::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 오정택 테스트
	//SetTimer(1212, 3000, NULL);

	return 0;
}

UINT THREAD_QUERYORDERQTY(LPVOID lpVoid)
{
	PROCEED_ORDER* pProceedOrderData = (PROCEED_ORDER*)lpVoid;
	CWndAssistIndexManager* pWnd = (CWndAssistIndexManager*)pProceedOrderData->pThis;

	STORDER_INDATA* pOrderData;

	if(pWnd->m_mapOrderData.Lookup(pProceedOrderData->nOrderIndex, pOrderData))
		pWnd->MakeQueryOrderQty(pOrderData, pProceedOrderData);

	return 0;
}

UINT THREAD_PROCEEDORDERSIGN(LPVOID lpVoid)
{
	PROCEED_ORDER* pProceedOrderData = (PROCEED_ORDER*)lpVoid;
	CWndAssistIndexManager* pWnd = (CWndAssistIndexManager*)pProceedOrderData->pThis;

	STORDER_INDATA* pOrderData;

	if(pWnd->m_mapOrderData.Lookup(pProceedOrderData->nOrderIndex, pOrderData))
		pWnd->SendOrder(pOrderData, pProceedOrderData);
	else
		return 0;
	pWnd->m_mapOrderData.RemoveKey(pProceedOrderData->nOrderIndex);

	//delete pOrderData;
	pOrderData = NULL;
	delete pProceedOrderData;
	pProceedOrderData = NULL;
	return 0;
}
void CWndAssistIndexManager::MakeOrderData_Stock(CStringArray& arrOrder, STORDER_INDATA* pOrderData, float fOrderPrice)
{
	CString strData;
	arrOrder.Add("1");  			// RECORD : 비반복 입력이므로 “1”

	strData = pOrderData->aAcctNo;
	arrOrder.Add(strData);          // “계좌번호”부터 순서대로 입력된다.

	arrOrder.Add(m_szPW);

	strData.Format(_T("A%s"), pOrderData->aCode);
	arrOrder.Add(strData);

	strData = pOrderData->aOrderAmt;
	arrOrder.Add(strData);

	strData.Format(_T("%d"), (int)fOrderPrice);
	arrOrder.Add(strData);

	if(pOrderData->nOrderType & VALEXITLONG)
		arrOrder.Add("1");
	else if(pOrderData->nOrderType & VALENTRYBUY)
		arrOrder.Add("2");

	if(pOrderData->nOrderGubun & MARKETENTRY)
		arrOrder.Add("03");
	else
		arrOrder.Add("00");

/*
	arrOrder.Add("00101108509"); 
	arrOrder.Add("0122");
	arrOrder.Add("A000660");
	arrOrder.Add("10");
	arrOrder.Add("34500");
	arrOrder.Add("2");
	arrOrder.Add("00");
*/

	arrOrder.Add("00");
	arrOrder.Add("0");
	arrOrder.Add("0");
	arrOrder.Add("00");
	arrOrder.Add("000");
	arrOrder.Add("00000000");
	arrOrder.Add("000");
	arrOrder.Add("0");
	arrOrder.Add("000000");
	arrOrder.Add(" ");
	arrOrder.Add("+999999");					// 주문회차, 클라이언트 고유 키
	arrOrder.Add("+000000000");
	arrOrder.Add("+000000000");
	arrOrder.Add("+000000000");
	arrOrder.Add("+000000000");
	arrOrder.Add(" ");
	arrOrder.Add("0");
	arrOrder.Add("0");			// 총 26개 ( RECORD 1 + ITEM 25 )
}

void CWndAssistIndexManager::MakeOrderData_FutureOption(CStringArray& arrOrder, STORDER_INDATA* pOrderData, float fOrderPrice)
{
	CString strData;
	arrOrder.Add("1");  			// RECORD : 비반복 입력이므로 “1”
	arrOrder.Add("40");

	strData = pOrderData->aAcctNo;
	arrOrder.Add(strData);          // “계좌번호”부터 순서대로 입력된다.

	arrOrder.Add(m_szPW);

	arrOrder.Add(pOrderData->aCode);
	if(pOrderData->nOrderType & VALENTRYSELL || pOrderData->nOrderType & VALEXITLONG )//
		arrOrder.Add("1");
	else if(pOrderData->nOrderType & VALENTRYBUY || pOrderData->nOrderType & VALEXITSHORT)//
		arrOrder.Add("2");

	arrOrder.Add("00"); 

	if(pOrderData->nOrderGubun & MARKETENTRY)
		arrOrder.Add("03");
	else
		arrOrder.Add("00");

	arrOrder.Add("03"); 

	strData.Format(_T("%10.2f"), fOrderPrice);
	arrOrder.Add(strData);

	strData = pOrderData->aOrderAmt;
	arrOrder.Add(strData);

	arrOrder.Add("00");
	arrOrder.Add("000000");
	arrOrder.Add(" ");
	arrOrder.Add("+999999");					// 주문회차, 클라이언트 고유 키
	arrOrder.Add("+000000000");
	arrOrder.Add("+000000000");
	arrOrder.Add("+000000000");
	arrOrder.Add("+000000000");
	arrOrder.Add(" ");
	arrOrder.Add(" ");
	arrOrder.Add("0");
	arrOrder.Add("0");
}

BOOL CWndAssistIndexManager::SendOrder(STORDER_INDATA* pOrderData, PROCEED_ORDER* pProceedOrderData) 
{
	BOOL bReturn = FALSE;

	if(pOrderData->nMarket == 1)		// 선물
	{
		int nOrderAmount = atoi(pOrderData->aOrderAmt);
			if(pOrderData->nOrderType & VALENTRYBUY || pOrderData->nOrderType & VALEXITSHORT )		// 매수 #define VALBUY (VALEXITSHORT|VALENTRYBUY)
		{
			if(pProceedOrderData->nClearQty && m_nApplyPosition !=0)//청산가능수량(기존 매도 진입수량있을때)
			{
				::sprintf(pOrderData->aOrderAmt, _T("%d"), pProceedOrderData->nClearQty);

				//pOrderData->nOrderType &= ~VALENTRYBUY;			
				bReturn = SendOrderFuture(pOrderData, pProceedOrderData->fOrderPriceClr);//VALEXITSHORT 매도청산으로 주문을...
				//pOrderData->nOrderType |= VALENTRYBUY;
			}
			if(pProceedOrderData->nOrderQty && (m_nApplyPosition == 0 || m_nApplyPosition ==2))//주문 가능수량(신규진입주문일때)
			{
				::sprintf(pOrderData->aOrderAmt, _T("%d"), nOrderAmount > pProceedOrderData->nOrderQty ? pProceedOrderData->nOrderQty : nOrderAmount);

				//pOrderData->nOrderType &= ~VALEXITSHORT;			// 매도진입
				bReturn = SendOrderFuture(pOrderData, pProceedOrderData->fOrderPriceEnt);//VALENTRYBUY매수진입으로 주문
				//pOrderData->nOrderType |= VALEXITSHORT;
			}
		}
		else if(pOrderData->nOrderType & VALEXITLONG || pOrderData->nOrderType & VALENTRYSELL )	// 매도
		{
			if(pProceedOrderData->nClearQty && (m_nApplyPosition == 0 || m_nApplyPosition ==2))//신규 청산 가능수량(기존 매수진입수량있을때)
			{
				::sprintf(pOrderData->aOrderAmt, _T("%d"), pProceedOrderData->nClearQty);
				
				//pOrderData->nOrderType &= ~VALENTRYSELL;			// 매수청산
				bReturn = SendOrderFuture(pOrderData, pProceedOrderData->fOrderPriceClr);//매수청산으로
				//pOrderData->nOrderType |= VALENTRYSELL;
			}

			if(pProceedOrderData->nOrderQty && m_nApplyPosition != 0)//신규 주문 가능 수량 
			{
				::sprintf(pOrderData->aOrderAmt, _T("%d"), nOrderAmount > pProceedOrderData->nOrderQty ? pProceedOrderData->nOrderQty : nOrderAmount);

				//pOrderData->nOrderType &= ~VALEXITLONG;		
				bReturn = SendOrderFuture(pOrderData, pProceedOrderData->fOrderPriceEnt);//매도진입으로 ..
				//pOrderData->nOrderType |= VALEXITLONG;
			}
		}
	}
	else if(pOrderData->nMarket == 0)	// 주식
	{
		if(pOrderData->nOrderType & VALEXITLONG && !pProceedOrderData->nClearQty ||
			pOrderData->nOrderType & VALENTRYBUY && !pProceedOrderData->nOrderQty)//매수일때 주문가능 수량이 없으면 리턴..
			return bReturn;
		
		float fOrderPrice = pProceedOrderData->fOrderPriceEnt;//진입가
		if(pOrderData->nOrderType & VALEXITLONG)//매수청산 
		{
			CString strMessage;
			fOrderPrice = pProceedOrderData->fOrderPriceClr;//청산가
			int nOrderAmount = atoi(pOrderData->aOrderAmt);
			if(nOrderAmount > pProceedOrderData->nClearQty)// KHD 
			{
				strMessage.Format("고객님이 주문가능한 수량은 %d 주 입니다. ",pProceedOrderData->nClearQty);
				AfxMessageBox(strMessage);
			}
			::sprintf(pOrderData->aOrderAmt, _T("%d"), nOrderAmount > pProceedOrderData->nClearQty ? pProceedOrderData->nClearQty : nOrderAmount);
		}
		bReturn = SendOrderStock(pOrderData, fOrderPrice);
	}

	return bReturn;
}

BOOL CWndAssistIndexManager::SendOrderStock(STORDER_INDATA* pOrderData, float fOrderPrice)
{
	int nRet = IDCANCEL;
	BOOL bReturn = FALSE;

	if(pOrderData->nAutoOrder == 1)//반자동
	{
		//주문창 뜨기 전이나.. 자동 주문시에 소리가 나게 설정 : by KHD
		if(pOrderData->nOrderType & VALENTRYBUY)//매수진입
		{
			if(m_bUseEntrySound && !m_szEntrySound.IsEmpty())
				sndPlaySound(m_szEntrySound, SND_ASYNC);
		}
		else if(pOrderData->nOrderType & VALEXITLONG )//매수청산 
		{
			if(m_bUseExitSound && !m_szExitSound.IsEmpty())
				sndPlaySound(m_szExitSound, SND_ASYNC);
		}

		CStrategyOrderLoader orderLoader;
		nRet = orderLoader.STOrder_Sign((long)pOrderData, fOrderPrice);
	}
	//else if(pOrderData->nAutoOrder == 2)//자동
	//	nRet = IDOK;

	if(nRet == IDOK)
	{
		CStringArray arrOrder;
		MakeOrderData_Stock(arrOrder, pOrderData, fOrderPrice);
//		bReturn = m_pITrComm->Send2ServerServiceMgr(_T("SONAT000"), &arrOrder);
	}

	return bReturn;
}

BOOL CWndAssistIndexManager::SendOrderFuture(STORDER_INDATA* pOrderData, float fOrderPrice)
{
	int nRet = IDCANCEL;
	BOOL bReturn = FALSE;

	if(pOrderData->nAutoOrder == 1)//반자동
	{
		//KHD : 주문 소리나게...
		if(pOrderData->nOrderType & VALENTRYBUY || pOrderData->nOrderType & VALEXITSHORT )
		{
			if(m_bUseEntrySound && !m_szEntrySound.IsEmpty())
				sndPlaySound(m_szEntrySound, SND_ASYNC);
		}
		else if(pOrderData->nOrderType & VALEXITLONG || pOrderData->nOrderType & VALENTRYSELL )
		{
			if(m_bUseExitSound && !m_szExitSound.IsEmpty())
				sndPlaySound(m_szExitSound, SND_ASYNC);
		}

		CStrategyOrderLoader orderLoader;
		nRet = orderLoader.STOrder_Sign((long)pOrderData, fOrderPrice);
	}
	//else if(pOrderData->nAutoOrder == 2)
	//	nRet = IDOK;

	if(nRet == IDOK)
	{
		CStringArray arrOrder;
		MakeOrderData_FutureOption(arrOrder, pOrderData, fOrderPrice);
//		bReturn = m_pITrComm->Send2ServerServiceMgr(_T("SONBT001"), &arrOrder);
	}

	return bReturn;
}


HRESULT CWndAssistIndexManager::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	if(atoi(szMsgCode) > 1000)//Error!! 처리 
	{
		AfxMessageBox(szMessage);
		return 0L;
	}

	if(dwTRDateLen)
	{
		if(!::lstrcmp(szTR, _T("SONAT000")))		// 주식주문 결과
		{
			//AfxMessageBox("주식주문결과");
//			CStringArray* pArrData = (CStringArray*)aTRData;
		}
		else if(!::lstrcmp(szTR, _T("SONBT001")))	// 선물주문 결과
		{
			//AfxMessageBox("선물주문결과");
//			CStringArray* pArrData = (CStringArray*)aTRData;
		}
		else if(!::lstrcmp(szTR, _T("SONBQ101")))// 선물주문가능수량
		{
			//AfxMessageBox("선물주문가능수량");
			CStringArray* pArrData = (CStringArray*)aTRData;
/*
			int nCount = pArrData->GetCount();
			CString strData;
			for(int i = 0; i < nCount; i++)
				strData = pArrData->GetAt(i);
*/
			
//			UINT nQAIndex = m_pITrComm->GetRealQAIndex(dwKey);
			UINT nQAIndex = 0;

			PROCEED_ORDER* pProcOrderData;
			if(!m_mapProcOrderData.Lookup(nQAIndex, pProcOrderData))
				return 0L;

			m_mapProcOrderData.RemoveKey(nQAIndex);

			pProcOrderData->nOrderQty = atoi(pArrData->GetAt(15));//신규진입가능 수량
			pProcOrderData->nClearQty = atoi(pArrData->GetAt(16));//청산 가능 수량

			AfxBeginThread(THREAD_PROCEEDORDERSIGN, pProcOrderData);
		
		}
		else if(!::lstrcmp(szTR, _T("SONAQ200")))//주식주문가능수량
		{
			//AfxMessageBox("주식주문수량을 받았다.");
			CStringArray* pArrData = (CStringArray*)aTRData;
//			UINT nQAIndex = m_pITrComm->GetRealQAIndex(dwKey);
			UINT nQAIndex = 0;

			PROCEED_ORDER* pProcOrderData;
			if(!m_mapProcOrderData.Lookup(nQAIndex, pProcOrderData))
				return 0L;

			m_mapProcOrderData.RemoveKey(nQAIndex);

			if(pArrData->GetAt(1) == _T("2"))			// 매수
				pProcOrderData->nOrderQty = atoi(pArrData->GetAt(32));
			else /*if(pArrData->GetAt(1) == _T("1"))*/	// 매도
				pProcOrderData->nClearQty = atoi(pArrData->GetAt(32));

			AfxBeginThread(THREAD_PROCEEDORDERSIGN, pProcOrderData);

		}
//@Solomon 091012
// 			else		// 호가요청 결과를 받는다.
// 			{
// 				UINT nKey = atoi(szTR);
// 				STORDER_INDATA* pOrderData = NULL;
// 				if(m_mapOrderData.Lookup(nKey, pOrderData))
// 				{
// 					PROCEED_ORDER* pProceedOrderData = new PROCEED_ORDER;
// 					pProceedOrderData->pThis = this;
// 					pProceedOrderData->nOrderIndex = nKey;
// 
// 					int iKeyCount = 2;
// 					int iKeySize  = 6;
// 
// 					WORD wDataType = C_DT_ULONG;
// 					int nDotPos = 0;
// 					if(pOrderData->nMarket == 1)
// 					{
// 						wDataType = C_DT_SHORT;
// 						nDotPos = 2;
// 						iKeySize  = 8;
// 					}
// 
// 					int nIdx = 11 + 2 * iKeyCount + iKeySize;
// 
// 					CString strData = m_pITrComm->ApplyByteOrder(wDataType, (char*)aTRData + nIdx, 10, &nIdx, nDotPos);
// 					pProceedOrderData->fOrderPriceEnt = (float)atof(strData);//매수금액
// 
// 					strData = m_pITrComm->ApplyByteOrder(wDataType, (char*)aTRData + nIdx, 10, &nIdx, nDotPos);
// 					pProceedOrderData->fOrderPriceClr = (float)atof(strData);//청산금액
// 
// 	//				if(pOrderData->nMarket == 1)
// 						AfxBeginThread(THREAD_QUERYORDERQTY, pProceedOrderData);
// 	//				else
// 	//				{
// 	//					SendOrder(pOrderData, pProceedOrderData);
// 	//
// 	//					m_mapOrderData.RemoveKey(nKey);
// 	//
// 	//					delete pOrderData;
// 	//					delete pProceedOrderData;
// 	//				}
// 				}
// 			}
	}

	return 0L;
}

void CWndAssistIndexManager::OnTimer(UINT nIDEvent)
{
// test order
//	if(nIDEvent == 1111)
//	{
//		KillTimer(1111);
//
//		m_pstOrderData->nOrderType = VALENTRYBUY | VALEXITSHORT;
//		m_pstOrderData->nOrderPriceTypeEnt = m_nJinValue;		// 진입가격
//
//		// 주문신호
//		OrderSign(m_pstOrderData);
//	}
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// 오정택 테스트
//	if(nIDEvent == 1212)
//	{
//		KillTimer(1212);
////		::memset(m_pstOrderData, 0x00, sizeof(STORDER_INDATA));
//		m_pstOrderData->nMarket = 0;
//		m_pstOrderData->nOrderType = VALBUY;
//		::strcpy(m_pstOrderData->aAcctNo, _T("00101108509"));		// 주식 테스트
////		::strcpy(m_pstOrderData->aAcctNo, _T("00151000226"));		// 선물 테스트
//		::strcpy(m_pstOrderData->aAcctName, _T("병행테스트1차"));
////		::strcpy(m_pstOrderData->aCode, _T("101AC000"));
//		::strcpy(m_pstOrderData->aCode, _T("000660"));
//		theApp.GetCodeName(m_pstOrderData->nMarket, m_pstOrderData->aCode, m_pstOrderData->aCodeName, sizeof(m_pstOrderData->aCodeName));
//		m_pstOrderData->nOrderGubun = 0;
//		::strcpy(m_pstOrderData->aOrderAmt, _T("10"));
//		m_pstOrderData->nOrderPriceTypeEnt = 0;
//		m_pstOrderData->nOrderPriceTypeClr = 3;
//		m_pstOrderData->nSTType = 0;
//	//	m_pstOrderData->szSTName = _T("");
//		m_pstOrderData->nAutoOrder = 0;
//
//		OrderSign(m_pstOrderData);
//	}

	CWnd::OnTimer(nIDEvent);
}

BOOL CWndAssistIndexManager::STItem_IsRun(LPCSTR p_szRunSTFile, LPCSTR p_szSTName)
{
	if(m_pSTItem)
	{
		if(m_pSTItem->m_strPath.CompareNoCase(p_szRunSTFile)==0) return TRUE;
		return FALSE;
	}
	return FALSE;	
}

// 2008.04.04 by LYH >>
BOOL CWndAssistIndexManager::IsCheckPosition(CDealData* pDealData)
{
	if(m_nTiming_Order == 1)
		return FALSE;

	BOOL retValue = FALSE;
	if(pDealData->m_nIndex == m_nOrderIndex)
	{
		if(IsBuy(m_nOrderType) || IsExitLong(m_nOrderType))
		{
			if(!IsSell(m_nOrderType) && !IsExitShort(m_nOrderType))
			{
				if(IsSell(pDealData->m_lDealValue) || IsExitShort(pDealData->m_lDealValue))
				{
					retValue = TRUE;
					m_nOrderType = 0;
				}
			}
		}
		if(IsSell(m_nOrderType) || IsExitShort(m_nOrderType))
		{
			if(!IsBuy(m_nOrderType) && !IsExitLong(m_nOrderType))
			{
				if(IsBuy(pDealData->m_lDealValue) || IsExitLong(pDealData->m_lDealValue))
				{
					retValue = TRUE;
					m_nOrderType = 0;
				}
			}
		}
	}

	return retValue;
}
// 2008.04.04 by LYH <<
