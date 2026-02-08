// SiteCustomAddin.cpp: implementation of the CSiteCustomAddin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartPatternAddin.h"
#include "SiteCustomAddin.h"

// 2008.02.20 by LYH >> 7.0 -> 6.0
//#include "../Chart_common/BlockBaseEnum.h"	// for CToolOptionInfo
#include "BlockBaseEnum.h"	// for CToolOptionInfo
//#include "../../inc/LoadDrds.h"						// for DrdsData
// 2008.02.20 by LYH <<

//#include "DlgPatternEnvSetup.h"
#include "DlgSetup.h"

// 2008.02.20 by LYH >>
#include "./Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
// 2008.02.20 by LYH <<

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern HINSTANCE g_hInstance;
CSiteCustomAddin::CSiteCustomAddin( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase, const BOOL p_bOverExclusive)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase, p_bOverExclusive)
{
	m_bRecvRealData_S31 = FALSE;

	//m_objPatternMaker.SetIChartOCX( m_pIChartManager, m_pIChartOCX,  m_pIPacketManager);
	m_lpszRQ = _T("DEFAULT");	// 현재 RQ : 복수종목 - ojtaso (20070305)

	ILPCSTR strRQ = m_pIChartOCX->GetSelectedRQ();
	m_lpszRQ = strRQ;
	//p_pIChartOCX->GetSelectedRQ();
	// RQ별 관리 : 복수종목 - ojtaso (20070305)
	CPatternMaker* pPatternMaker = new CPatternMaker;
//	pPatternMaker->Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
	pPatternMaker->SetIChartOCX( m_pIChartManager, m_pIChartOCX,  m_pIPacketManager);
	pPatternMaker->SetRQ(m_lpszRQ);

	m_mapPatternMaker.insert(MAP_PATTERNMAKER::value_type(m_lpszRQ, pPatternMaker));

		
	//m_bBlock			= FALSE;
	//m_dCurPrice		= ZERO;
	//m_dOpen			= ZERO;
	//m_dHigh			= ZERO;
 	//m_dLow			= ZERO;

	m_hOcxWnd = NULL;
	if( p_pIChartOCX) m_hOcxWnd = p_pIChartOCX->GetOcxHwnd();
}

CSiteCustomAddin::~CSiteCustomAddin()
{
	//m_objPatternMaker.DestroyAnalysisDialog();
	// RQ별 관리 : 복수종목 - ojtaso (20070305)
	if(!m_mapPatternMaker.empty())
	{
		MAP_PATTERNMAKER::iterator it = m_mapPatternMaker.begin();
		while(it != m_mapPatternMaker.end())
		{
			(*it).second->DestroyAnalysisDialog();
			delete (*it++).second;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CSiteCustomAddin)
	ONADDINEVENT( OnDrdsData)
	ONADDINEVENT( OnDrdsPacketEnd)
	ONADDINEVENT( OnDrawAfterGraph)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnResetChart)
	// 복수종목 지원을 위해 추가 : 복수종목 - ojtaso (20070125)
//	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType_char_char_char_CObject)
//	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long_char_char_char)
	// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
	ONADDINEVENT( OnUpdatePacketDataMultiItem)
	ONADDINEVENT( OnPacketDataMultiItem)
	// 복수종목 추가/삭제 : 복수종목 - ojtaso (20070302)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// (2004.10.15, 배승원) Real 관련 Event를 전달한다.
//////////////////////////////////////////////////////////////////////

// [04/10/14] Real을 수신받아 RELPSA를 처리한다.
//		속도 관계로 DRDS의 Data를 그대로 Pass한다.
void CSiteCustomAddin::OnDrdsData( WPARAM p_wParam, LPARAM p_lParam)
{
	LPCSTR	pData = (LPSTR)p_lParam; //(LPCSTR)lParam;
	int		len	  = (int)p_wParam;
	CString strTemp;

// 2008.02.20 by LYH >> Not Used
//	if(len > 3)   
//	{
//		DrdsData* pmyDData = (DrdsData*)pData;
//		
//// DEBUG------------------------------------------------------------------>>
////		CString	strDbg;
////		strDbg.Format("%s", pmyDData->Name);
////		OutputDebugString("\n[HSpeedJumun-DrdsData] DataName = " + strDbg);
//// <<------------------------------------------------------------------------
//
//		if(memcmp(pmyDData->Name, "S31", 3) == 0)			// 현재가 패킷
//		{
//			m_bRecvRealData_S31 = TRUE;
//		}
//	}
// 2008.02.20 by LYH <<
}

// [04/10/15] Real Packet의 수신이 끝났음을 AddIn DLL에게 알려 처리토록 한다.
void CSiteCustomAddin::OnDrdsPacketEnd( WPARAM p_wParam, LPARAM p_lParam)
{
	// a. OnDrdsData()에서 실시간으로 현재가 패킷을 받은 이후에
	// b. OnDrdsPacketEnd()에서 Real Packet의 수신이 끝났음을 알리는 신호가 들어오면 패턴 재계산
	//if ( m_bRecvRealData_S31 )
	//{
	//	m_bRecvRealData_S31 = FALSE;


	//	m_objPatternMaker.DeleteAllData();					// 패턴관련 데이터 모두 삭제
	//	m_objPatternMaker.InitSelectedPatternData(TRUE);	// Init Selected Pattern Data

	//	// Get All WavePattern Data
	//	m_objPatternMaker.GetAllData_AboutPattern();
	//		
	//	// 라인을 그리는 경우
	//	if ( !m_objPatternMaker.m_PatternEnvData.m_bNotDrawAll)
	//	{
	//		// 실시간에서 그려줄 선택된 패턴이 있는지 체크
	//		m_objPatternMaker.CheckSelectedPatternData_ForReal();
	//	}
	//}
}

// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
void CSiteCustomAddin::OnUpdatePacketDataMultiItem(LPCTSTR lpszRQ, BOOL p_bTrUpdate)
{
	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	GetPatternMaker(pPatternMaker, lpszRQ);

	if(!pPatternMaker)
		return;

	if(p_bTrUpdate==0&&pPatternMaker)
	{
		if ( m_bRecvRealData_S31 )
		{
			m_bRecvRealData_S31 = FALSE;


			pPatternMaker->DeleteAllData();					// 패턴관련 데이터 모두 삭제
			pPatternMaker->InitSelectedPatternData(TRUE);	// Init Selected Pattern Data

			// Get All WavePattern Data
			pPatternMaker->GetAllData_AboutPattern();
				
			// 라인을 그리는 경우
			if ( !pPatternMaker->m_PatternEnvData.m_bNotDrawAll)
			{
				// 실시간에서 그려줄 선택된 패턴이 있는지 체크
				pPatternMaker->CheckSelectedPatternData_ForReal();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) 기본 Event를 수신한다. (필요 없는 경우 정의하지 않는다. Base에서 끊어준다.)
//////////////////////////////////////////////////////////////////////

// [04/12/25] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
// 추세 라인이 봉그래프에 가려지기때문에 DrawAfter 사용
void CSiteCustomAddin::OnDrawAfterGraph( CDC *p_pDC)
{
	//AFX_MANAGE_STATE( AfxGetStaticModuleState());

	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	MAP_PATTERNMAKER::iterator it = m_mapPatternMaker.begin();
	while(it != m_mapPatternMaker.end())
	{
		pPatternMaker = (*it).second;

		pPatternMaker->OnDraw( p_pDC);
		it++;
	}

	//m_objPatternMaker.OnDraw( p_pDC);
}

// 복수종목별 Real반영 : 복수종목 - ojtaso (20070131)
void CSiteCustomAddin::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
// 2008.01.20 by LYH >>	
	//if( !( BOOL)strstr( p_szPacketNames, "종가;") /*|| !p_bWithReset*/) return;
	CString strClose;
	strClose.Format("%s;", _MTEXT(C0_CLOSE));
	if( !( BOOL)strstr( p_szPacketNames, strClose) /*|| !p_bWithReset*/) return;
// 2008.01.20 by LYH <<	
	// (2006/2/22 - Seung-Won, Bae) Init I/O Packet.
	// (2006/2/24 - Seung-Won, Bae) Do not SetRawData() in CalculateIndicator.
	//	if( m_assistIndexManager.GetSafeHwnd()) m_assistIndexManager.SetRawData(lpszRQ);

	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	GetPatternMaker(pPatternMaker, lpszRQ);

	if(!pPatternMaker)
		return;
	

	pPatternMaker->DeleteAllData();					// 패턴관련 데이터 모두 삭제
	pPatternMaker->InitSelectedPatternData();		// Init Selected Pattern Data

	// Get All WavePattern Data
	pPatternMaker->GetAllData_AboutPattern();
	// Get All CandlePattern Data
	pPatternMaker->GetAllData_AboutCandlePattern();
}


//////////////////////////////////////////////////////////////////////
// (2004.10.29, 배승원) AddIn DLL에게 그밖의 Windows Event를 전달한다.
//////////////////////////////////////////////////////////////////////

// JSJ_ADD_50119__차트에서 InvokeAddIn함수 호출함
BOOL CSiteCustomAddin::InvokeAddIn( int p_nCommandType, long p_lData)
{
	// (2004.10.19, 배승원) Resource의 접근이 잘되도록 Module State를 적용한다.
	//AFX_MANAGE_STATE( AfxGetStaticModuleState());

	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CPatternMaker* pPatternMaker;

	// Create Or Destroy Analysis Dialog (p_lData  0:Destroy, 1:자동추세 2:추세패턴 3:패턴분석 4:패턴설정)
	if ( p_lData > 0)
	{
		//2007.02.26 by LYH 전체초기화시만 파일 삭제
		if(p_lData == 200 )	//전체초기화
		{
			// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
			CPatternMaker* pPatternMaker;
			GetPatternMaker(pPatternMaker);

			pPatternMaker->DestroyAnalysisDialog();
			pPatternMaker->m_bDrawAutoTrendLine = FALSE;
			pPatternMaker->m_bDrawAutoPattern = FALSE;
			pPatternMaker->m_bDrawCandlePattern = FALSE;
			return TRUE;
		}
		//}}
		//if(p_lData == 2)
		//	m_objPatternMaker.CreateDlgAnalysis( m_pIChartOCX, p_lData, TRUE);
		//else
		//m_objPatternMaker.CreateDlgAnalysis( m_pIChartOCX, p_lData, FALSE);
		GetPatternMaker(pPatternMaker, m_lpszRQ);
		if(!pPatternMaker)
		{
			pPatternMaker = new CPatternMaker;
			pPatternMaker->SetIChartOCX( m_pIChartManager, m_pIChartOCX,  m_pIPacketManager);
			pPatternMaker->SetRQ(m_lpszRQ);
			//pPatternMaker->Create(NULL,"Assist", WS_CHILD,CRect(0,0,0,0), (CWnd*)m_pChartCWnd, 0x001, m_pIChartManager, m_pIChartOCX, m_pIPacketManager, m_pIGraphInfoManager);
			m_mapPatternMaker.insert(MAP_PATTERNMAKER::value_type(m_lpszRQ, pPatternMaker));
		}
		pPatternMaker->CreateDlgAnalysis( m_pIChartOCX, p_lData, FALSE);

		pPatternMaker->DeleteAllData();					// 패턴관련 데이터 모두 삭제
		pPatternMaker->InitSelectedPatternData();		// Init Selected Pattern Data

		// Get All WavePattern Data
		pPatternMaker->GetAllData_AboutPattern();
		// Get All CandlePattern Data
		pPatternMaker->GetAllData_AboutCandlePattern();

		//m_objPatternMaker.DeleteAllData();					// 패턴관련 데이터 모두 삭제
		//m_objPatternMaker.InitSelectedPatternData();		// Init Selected Pattern Data

		//// Get All WavePattern Data
		//m_objPatternMaker.GetAllData_AboutPattern();
		//// Get All CandlePattern Data
		//m_objPatternMaker.GetAllData_AboutCandlePattern();

		if(p_lData == 8)
		{
			ML_SET_LANGUAGE_RES();

			CDlgSetup dlgSetup;
			//dlgSetup.SetPatternEnvData(&(m_objPatternMaker.m_PatternEnvData));
			dlgSetup.SetPatternEnvData(&(pPatternMaker->m_PatternEnvData));

			if( dlgSetup.DoModal() == IDOK )
			{
				//m_objPatternMaker.DeleteAllData();				// 패턴관련 데이터 모두 삭제
				//m_objPatternMaker.InitSelectedPatternData();		// Init Selected Pattern Data

				//// Get All WavePattern Data
				//m_objPatternMaker.GetAllData_AboutPattern();
				//// Get All CandlePattern Data
				//m_objPatternMaker.GetAllData_AboutCandlePattern();
				pPatternMaker->DeleteAllData();				// 패턴관련 데이터 모두 삭제
				pPatternMaker->InitSelectedPatternData();		// Init Selected Pattern Data

				// Get All WavePattern Data
				pPatternMaker->GetAllData_AboutPattern();
				// Get All CandlePattern Data
				pPatternMaker->GetAllData_AboutCandlePattern();
			}	
		}
	}
	else if ( p_lData == 0)
	{
		//m_objPatternMaker.DestroyAnalysisDialog();
		GetPatternMaker(pPatternMaker, m_lpszRQ);
		if(pPatternMaker)
			pPatternMaker->DestroyAnalysisDialog();
		return FALSE;
	}


	m_pIChartOCX->InvalidateControl();
	

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////
BOOL CSiteCustomAddin::OnLButtonDown(CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();

	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	GetPatternMaker(pPatternMaker);

	CPoint pt(point);

	if(pPatternMaker)
		return pPatternMaker->OnLButtonDown(nFlags,point);
	else
		return FALSE;
	//return m_objPatternMaker.OnLButtonDown( nFlags, point);
}

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////
BOOL CSiteCustomAddin::OnMouseMove(CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	ML_SET_LANGUAGE_RES();

	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	GetPatternMaker(pPatternMaker);

	CPoint pt(point);
	if(pPatternMaker)
		return pPatternMaker->OnMouseMove( nFlags, point, pDC);
	else
		return FALSE;
	//return m_objPatternMaker.OnMouseMove( nFlags, point, pDC);
}

BOOL CSiteCustomAddin::OnLButtonUp(CDC *pDC, UINT nFlags, CPoint &point, const DWORD p_dwMouseActionID)
{
	//AFX_MANAGE_STATE( AfxGetStaticModuleState());
	// 해당 RQ의 AssistIndexManager : 복수종목 - ojtaso (20070305)
	CPatternMaker* pPatternMaker;
	GetPatternMaker(pPatternMaker);

	CPoint pt(point);
	if(pPatternMaker)
		return pPatternMaker->OnLButtonUp( nFlags, point);
	else 
		return FALSE;
	//return m_objPatternMaker.OnLButtonUp( nFlags, point);
}

//{{ 차트 초기화 처리 by LYH 2006.12.22
void CSiteCustomAddin::OnResetChart()
{
	//m_objPatternMaker.DestroyAnalysisDialog();
	//m_objPatternMaker.m_bDrawAutoTrendLine = FALSE;
	//m_objPatternMaker.m_bDrawAutoPattern = FALSE;
	//m_objPatternMaker.m_bDrawCandlePattern = FALSE;
}
//}}

// 해당 RQ의 PatternMaker : 복수종목 - ojtaso (20070305)
void CSiteCustomAddin::GetPatternMaker(CPatternMaker*& pPatternMaker, LPCTSTR lpszRQ/* = NULL*/)
{
	BOOL bRetunNull = TRUE;

	if(!lpszRQ)
	{
		lpszRQ = m_lpszRQ;
		bRetunNull = FALSE;
	}

	MAP_PATTERNMAKER::iterator it = m_mapPatternMaker.find(lpszRQ);
	if(it == m_mapPatternMaker.end())
	{
		// RQ를 지정했을경우 기본 RQ의 값을 리턴하지 않는다
		if(bRetunNull)
		{
			pPatternMaker = NULL;
			return;
		}
		else
		{
			it = m_mapPatternMaker.find(_T("DEFAULT"));
			if(it == m_mapPatternMaker.end())
			{
				pPatternMaker = NULL;
				return;
			}
		}
	}

	pPatternMaker = (*it).second;
}

// 복수종목 추가/삭제 : 복수종목 - ojtaso (20070302)
BOOL CSiteCustomAddin::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam)
{
	if(p_eCommandID == EAI_PACKETLISTMANAGER_NEW_RQ)
	{
		CPatternMaker* pPatternMaker;
		GetPatternMaker(pPatternMaker, p_szParam);
		if(pPatternMaker != NULL)
			return FALSE;

		pPatternMaker = new CPatternMaker;
		pPatternMaker->SetIChartOCX( m_pIChartManager, m_pIChartOCX,  m_pIPacketManager);
		pPatternMaker->SetRQ(p_szParam);

		m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50031,FALSE);
		m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50032,FALSE);
		m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50033,FALSE);

		m_mapPatternMaker.insert(MAP_PATTERNMAKER::value_type(p_szParam, pPatternMaker));
		m_lpszRQ = p_szParam;
	}
	else if(p_eCommandID == EAI_PACKETLISTMANAGER_CHANGE_RQ)
	{
		m_lpszRQ = p_szParam;

		CPatternMaker* pPatternMaker;
		GetPatternMaker(pPatternMaker, m_lpszRQ);
		if(pPatternMaker != NULL)
		{
			m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50031,pPatternMaker->m_bDrawAutoTrendLine);
			m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50032,pPatternMaker->m_bDrawAutoPattern );
			m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50033,pPatternMaker->m_bDrawCandlePattern);
		}
		else
		{
			m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50031,FALSE);
			m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50032,FALSE);
			m_pIChartOCX->OnToolStateChanged((CToolOptionInfo::TOOLOPTION)50033,FALSE);
		}
	}
	else if(p_eCommandID == EAI_PACKETLISTMANAGER_DELETE_RQ)
	{
		CPatternMaker* pPatternMaker;
		GetPatternMaker(pPatternMaker, p_szParam);

		if(pPatternMaker)
		{
			if(pPatternMaker)
				pPatternMaker->DestroyAnalysisDialog();
			delete pPatternMaker;
		}

		m_mapPatternMaker.erase(p_szParam);
	}

	return FALSE;
}
