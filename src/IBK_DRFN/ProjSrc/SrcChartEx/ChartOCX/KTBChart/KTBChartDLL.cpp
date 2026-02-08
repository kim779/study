// KTBChartDLL.cpp: implementation of the CKTBChartDLL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/DLL_Load/AddInManager.h"			// for CAddInManager
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../AddInManager/Include_AddIn/AddInCommand.h"		// for EAI_CUSTOM_CLEAR_TIMEMARK
#include "OutsideCommonInterface.h"							// for RMSG_CHARTMSG_EVENTMESSAGE
#include "KTBChartCtl.h"									// for CKTBChartCtrl
#include "IndicatorGroupData.h"								// for CIndicatorGroupDataList
#include "IndicatorList.h"									// for CIndicatorList
#include "Color.h"											// for CGraphRegionColor
#include "Conversion.h"										// for CDataConversion
#include "IndicatorDefaultData.h"							// for CIndicatorDefaultData
#include "IndicatorInfo.h"									// for CIndicatorInfo
#include "ChartCfg.h"										// for CChartCfg
#include "MainBlock.h"										// for CMainBlock
#include "PacketList.h"										// for CPacketList
#include "ToolBarManager.h"									// for CToolBarManager
#include "XScaleManager.h"									// for CXScaleManager
#include "PacketListManager.h"								// 여러 PacketList를 관리 : 복수종목 - ojtaso (20070109)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// (2004.05.06, 배승원) DLL Container의 Window Handle과 Chart의 내부 ID를 관리하여 Event Notify를 전달하도록 한다.
// (2004.10.08, 배승원) SendEventMsgToDLL()의 이용 편이를 위하여 Filtering하지 않는다.
const UINT RMSG_CHARTMSG_EVENTMESSAGE	= ::RegisterWindowMessage( _CHARTMSG_EVENTMESSAGE);
LRESULT CKTBChartCtrl::SendEventMsgToDLL( UINT message, WPARAM wParam, LPARAM lParam)
{
	MSG msg;
	msg.hwnd	= GetSafeHwnd();
	msg.message	= message;
	msg.wParam	= wParam;
	msg.lParam	= lParam;
	msg.time	= 0;
	msg.pt.x	= 0;
	msg.pt.y	= 0;
	return ::SendMessage( m_hContainerDllWnd, RMSG_CHARTMSG_EVENTMESSAGE, m_nID, ( LPARAM)&msg);
}

// (2004.05.06, 배승원) DLL Container의 Window Handle과 Chart의 내부 ID를 관리하여 Event Notify를 전달하도록 한다.
void CKTBChartCtrl::SetContainerDllInfo(long p_lDllWnd, short p_nID) 
{
	// TODO: Add your dispatch handler code here
	m_hContainerDllWnd	= ( HWND)p_lDllWnd;
	m_nID				= p_nID;
}

// (2004.05.04, 배승원) 차트에서 제공하는 지표의 이름들을 구하여
//		외부 Module에게 알려주는 Interface를 제공한다.
//		1. Parameter
//			LPCTSTR p_strGroup	:	조회하고자 하는 지표의 Group명을 지정한다.
//									NULL인 경우 전체 지표를 Group 명과 함께 조회할 수 있다.
//		2. Return
//			BSTR				:	조회한 지표명 List String이다.
//									- Group을 지정하여 요청한 경우의 Format
//										"지표1,지표2,...,지표k"
//									- Group을 지정하지 않고 요청한 경우의 Format
//										"구룹명1:지표11,지표12,...,지표1k\n구룹명2:지표21,지표22,...,지표2k\n...\n구룹명l:지표l1,지표l2,...,지표lk\n"
BSTR CKTBChartCtrl::GetIndicatorNameList(LPCTSTR p_strGroup2) 
{
	// TODO: Add your dispatch handler code here
	CString p_strGroup = _STOM7( p_strGroup2);
	CString strResult;

	// 0. 먼저 Indicator List DLL이 Loading되었는지 확인한다.
	if( !m_pIndicatorList) return strResult.AllocSysString();

	// 1. Indicator Group List를 구한다.
	CIndicatorGroupDataList *pIndicatorGroupList = m_pIndicatorList->GetAllIndicatorNameWithGroupName();
	if( !pIndicatorGroupList) return strResult.AllocSysString();

	// 2. 각 Indicator Group에 대해서, Indicator Name을 구한다.
	POSITION posIndicatorGroup = pIndicatorGroupList->GetHeadPosition();
	while( posIndicatorGroup)
	{
		// 2.1 먼저 Indicator Group Name을 구한다.
		CIndicatorGroupData *pIndicatorGroup = pIndicatorGroupList->GetNext( posIndicatorGroup);
		CString strIndicatorGroupName = pIndicatorGroup->GetGroupName();

		// 2.2 요청한 Indicator Group인지를 확인하여 처리한다.
		if( !p_strGroup || !*p_strGroup || strIndicatorGroupName == p_strGroup)
		{
			// 2.2.1 요청한 Indicator Group이 없는 경우 (전체 요청의 경우)
			//		Indicator Group Name도 등록시킨다.
			if( !p_strGroup || !*p_strGroup) strResult += strIndicatorGroupName + ":";

			CList< CString, CString> *pGraphNameList = pIndicatorGroup->GetIndicatorNameList();
			POSITION posGraphName = pGraphNameList->GetHeadPosition();
			while( posGraphName)
			{
				strResult += pGraphNameList->GetNext( posGraphName);
				if( posGraphName) strResult += ",";
			}

			// 2.2.1 요청한 Indicator Group이 없는 경우 (전체 요청의 경우)
			//		다음 Indcator Group을 위하여 줄바꿈 구분자를 등록한다.
			if( !p_strGroup || !*p_strGroup) strResult += "\r\n";
		}
	}
 
	return strResult.AllocSysString();
}

//sy 2006.03.02
// (2004.05.04, 배승원) Chart DLL 화면에서 Graph를 추가시킬 수 있는 Interface를 제공한다.
void CKTBChartCtrl::AddGraph(short nRow, short nColumn, short nVertScaleGroup, LPCTSTR szGraphOption2, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale) 
{
 	if(m_pMainBlock == NULL)
		return;
	CString szGraphOption = _STOM2( szGraphOption2);

	CString strGraphDataForMaking;
	if(!GetGraphDataForMaking(szGraphOption, strGraphDataForMaking))
		return;

	// JSR 20060122 Add : Get The OCX Block State.
	bool bIsBlockEmpty = IsBlockEmpty();
	
  	// RQ별로 그래프 삽입 : 복수종목 - ojtaso (20061224)
  	// CPacketRQ*로 변경 : 복수종목 - ojtaso (20070109)
  	LPVOID pPacketRQ = m_pPacketListManager->GetPacketRQ(m_strRQ);
	if(!pPacketRQ)
		return;

  	if(!m_pMainBlock->InsertGraphInBlock(pPacketRQ, CBlockIndex(nRow, nColumn), nVertScaleGroup, 
		strGraphDataForMaking, bCreateBlock == TRUE, bCreateVertScaleGroup == TRUE, bUseAutoScale == TRUE))
  		return;

	// JSR 20060122 Add : Make a Block Double Size When The OCX is Empty State.
	if(bIsBlockEmpty)
		m_pMainBlock->SetDoubleSizeBlockNumber(CBlockIndex(m_nRowDoubleSizeBlock, m_nColumnDoubleSizeBlock));

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	if( m_strRQ.Right( 5) == "_PAIR" && szGraphOption == _MTEXT( C2_PRICE_CHART))
	{
		CList<CString, CString> strIndicatorNameList, strGraphNameList;
		CString strMainRQ = m_strRQ.Left( m_strRQ.GetLength() - 5);
		m_pMainBlock->GetAllIndicatorAndGraphNameWithRQ( strIndicatorNameList, strGraphNameList, strMainRQ);
		POSITION psnIndicator = strIndicatorNameList.GetHeadPosition();
		POSITION psnGraph = strGraphNameList.GetHeadPosition();
		CString strIndicatorName, strGraphName, strPairGraphName;
		while( psnIndicator && psnGraph)
		{
			strIndicatorName = strIndicatorNameList.GetNext( psnIndicator);
			strGraphName = strIndicatorNameList.GetNext( psnGraph);
			if( strIndicatorName != _MTEXT( C2_PRICE_CHART))
				AddChart_GenneralChart( strIndicatorName, strPairGraphName, FALSE);
			else
			{
				int nR = -1, nC = -1, nV = -1, nG = -1;
				m_pMainBlock->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, strPairGraphName, m_strRQ);
			}
			CIndicatorInfo *pMainIndiInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strGraphName, strIndicatorName, EIIGT_WITHOUT_DEFAULT);
			CIndicatorInfo *pPairIndiInfo = m_pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strPairGraphName, strIndicatorName, EIIGT_WITHOUT_DEFAULT);
			if( !pMainIndiInfo || !pPairIndiInfo) continue;
			pPairIndiInfo->GetCopyDataFrom( pMainIndiInfo, FALSE);
			if( strIndicatorName == _MTEXT( C2_PRICE_CHART))
			{
				CGraphColor graphColor = pPairIndiInfo->GetSubGraphColor( 0);
				graphColor.SetColor1( graphColor.GetColor4());
				pPairIndiInfo->SetSubGraphColor( 0, graphColor);
			}
		}

		m_pMainBlock->ChangeAllGraphDataFromIndicatorInfo( TRUE);
	}

	InvalidateControl();
}

bool CKTBChartCtrl::GetGraphDataForMaking( const CString& strGraphOption, CString& strGraphDataForMaking) const
{
	if(strGraphOption.IsEmpty())
		return false;

	// (2004.09.08, 배승원)지표명만 들어온 것인지 확인 -> "Graph;\r\n"의 유무로 확인
	if( !strncmp( strGraphOption, "Graph;\r\n", 8))
	{
		// "Graph;\r\n;;시가,고가,저가,종가;\r\n1;0;0;Graph1;title;0;238,0,0;\r\nGraphEnd;\r\n";
		strGraphDataForMaking = strGraphOption;
		return true;
	}

	// strGraphOption 지표명만 들어오는 경우 -> 기본 Indicator Info에 따라 등록
	if(m_pIndicatorList == NULL) return false;

	CIndicatorInfo* pIndicatorInfo = m_pIndicatorList->CreateGraphIndicatorInfo( strGraphOption);
	if( !pIndicatorInfo) return FALSE;
	pIndicatorInfo->GetGraphDataForMaking( strGraphDataForMaking);

	return (!strGraphDataForMaking.IsEmpty());
}

// JSR 20060122 Add Start : Get The OCX Block State.
bool CKTBChartCtrl::IsBlockEmpty() const
{
	if(m_pMainBlock == NULL)
		return true;

	return (m_pMainBlock->GetRowCount() <= 0 || m_pMainBlock->GetColumnCount() <= 0);
}
// JSR 20060122 Add End

// (2004.05.04, 배승원) Chart DLL 화면에서 Graph를 추가시킬 수 있는 Interface를 제공한다.
void CKTBChartCtrl::AddPacket(LPCTSTR szPacketOption) 
{
	// TODO: Add your dispatch handler code here
	if(m_pPacketList == NULL || szPacketOption == "")
		return;

	m_pPacketList->AddPacket(szPacketOption);
}

// (2004.05.04, 배승원) Chart DLL 화면에서 Graph Data를 삭제시킬 수 있는 Interface를 제공한다.
BOOL CKTBChartCtrl::RemovePacket(LPCTSTR szPacketName2) 
{
	// TODO: Add your dispatch handler code here
	if(m_pPacketList == NULL)
		return FALSE;

	CString szPacketName = _STOM0( szPacketName2);
	if(!m_pPacketList->RemovePacket(szPacketName))
		return FALSE;

	if( m_pMainBlock == NULL)
		return FALSE;

	if(m_pMainBlock->Initialize_Packet(szPacketName, FALSE))
		InvalidateControl() ;

	return TRUE;
}

#include "DumpLogger.h"									// for CDumpLogger
// (2004.05.12, 배승원) Chart DLL 화면에서 TR Data를 등록시킬 수 있는 Interface를 제공한다.
// (2004.05.19, 배승원) Packet Data를 Clear후 설정할지, 이어 붙일지를 선택할 수 있는 Parameter를 추가한다.
// (2004.06.25, 배승원) Packet이 Adding될 경우의 Scroll 이동 오류를 제한한다.
//		View Index가 300~400 인 경우, 400 Data가 Adding되면, 700~800으로 View Index를 변경하여야 한다.
BOOL CKTBChartCtrl::SetPacketData(LPCTSTR szPacketName2, long szData, long lLength, BOOL bReset) 
{
	if( m_bEnableLogChartLoading) GetLocalTime( &m_stLogStart);

	// 0. 먼저 Pointer를 확인한다.
	if( !szPacketName2 || !m_pPacketList) return FALSE;
	if( !*szPacketName2) return FALSE;

	// (2009/5/29 - Seung-Won, Bae) Support No TR Test Environment.
	CString strEnvValue;
	GetTestEnvValue( "TR Limit", strEnvValue);
	if( !strEnvValue.IsEmpty())
	{
		long lCount = atol( strEnvValue);
		if( 0 <= lCount)
		{
			CString strCount;
			strCount.Format( "%05d", lCount);
			strncpy( ( ( char *)szData), strCount, 5);
		}
	}

	CString szPacketName = _STOMS( szPacketName2, ";", 0);

	// (2004.07.26, 배승원) 가격차트가 새로 지정되는 경우, 가격차트의 자동 Style 기능을 설정한다.
	// 0.5 먼저 이전 Style을 기억한다.
	int nPrevTimeInterval;
	CScaleBaseData::HORZSCALEDRAWERTYPE ePrevUnitType;
	BOOL bPriceChart = ( BOOL)strstr( szPacketName, CString( _MTEXT( C0_OPEN)) + ";" + _MTEXT( C0_HIGH) + ";" + _MTEXT( C0_LOW) + ";" + _MTEXT( C0_CLOSE) + ";") && bReset;
	if( bPriceChart)
	{
		nPrevTimeInterval = m_pPacketList->GetTimeInterval();
		ePrevUnitType = m_pPacketList->GetDateUnitType();
	}

	// 2.1 먼저 Packet의 전체 Data수와 Scroll Index위치를 확인한다.
	// 2.1.1 Data수를 구한다.
	int nDataCount = 0;
	// 2.1.2 Scroll Index를 구한다.
	int nStartIndex = 0, nEndIndex = 0;
	if(!bReset)
	{
		nDataCount = m_pPacketList->GetMaxCount();
		GetDataStartEnd( nStartIndex, nEndIndex);
	}

	//sy 2005.11.09. packet data 넣기
	SetPacketDataInPacketList(szPacketName, szData, lLength, bReset);

	// (2004.08.12, 배승원) 분석도구도 모두 Clear한다.
	// (2009/8/20 - Seung-Won, Bae) To load previous setting of same time period.
	//		Delete with checking current state.
	//		See CAnalysisAddInImp::OnAddInToolCommand()
	if( bPriceChart) RunToolFunction( CToolOptionInfo::T_DELETE_ALL, 820);

	//sy 2005.08.30.
	OnPacketTRData(szPacketName, bReset);

	InvalidateControl();

	if( m_bEnableLogChartLoading)
	{
		SYSTEMTIME stEnd;
		GetLocalTime( &stEnd);
		CString strLog;
		strLog.Format( "Start Time : %02d:%02d:%02d.%03d\r\nEnd Time : %02d:%02d:%02d.%03d\r\nData Count : %d\r\n",
			m_stLogStart.wHour, m_stLogStart.wMinute, m_stLogStart.wSecond, m_stLogStart.wMilliseconds,
			stEnd.wHour, stEnd.wMinute, stEnd.wSecond, stEnd.wMilliseconds,
			m_pPacketList->GetMaxCount());
		CDumpLogger::LogStringToFile( "CKTBChartCtrl::SetPacketData()", strLog, strLog.GetLength(), "ChartSpeedLog");
	}

	return TRUE;
}

//sy 2005.11.09.
BOOL CKTBChartCtrl::SetPacketDataInAdditionPacket(LPCTSTR szPacketName2, long szData, long lLength, BOOL bReset) 
{
	CString szPacketName = _STOM0( szPacketName2);
	if(!SetPacketDataInPacketList(szPacketName, szData, lLength, bReset))
		return FALSE;

	if(m_pMainBlock == NULL || szPacketName == "")
		return TRUE;

	CList<CString, CString> packetNameList;
	CString strPacketNames = szPacketName;
	if(!strPacketNames.IsEmpty() && strPacketNames.Right(1) != ';')
		strPacketNames += ';';
	while(!strPacketNames.IsEmpty())
	{
		CString strPacketName = CDataConversion::GetStringData(strPacketNames, ";");
		if(!strPacketNames.IsEmpty())
			packetNameList.AddTail(strPacketName);
	}

	m_pMainBlock->CalculateAllGraphs(packetNameList, CCalculateBaseData::TRDATA_CAL, true);
	InvalidateControl();

	return TRUE;
}

bool CKTBChartCtrl::SetPacketDataInPacketList(LPCTSTR szPacketName, long szData, long lLength, BOOL bReset) 
{
	// 0. 먼저 Pointer를 확인한다.
	if( !szPacketName || !m_pPacketList || !*szPacketName)
		return false;

	// (2004.06.25, 배승원) Packet이 Adding될 경우의 Scroll 이동 오류를 제한한다.
	//		View Index가 300~400 인 경우, 400 Data가 Adding되면, 700~800으로 View Index를 변경하여야 한다.
	// 1. 따라서, Adding이 아닌 Reset인 경우에는 SetScrollData를 이용하고,
	if( bReset)
	{
		m_pPacketList->ClearData( szPacketName, ";");

		m_pPacketList->SetPacketData( szPacketName, szData, lLength);
	}
	// 2. Adding의 경우에는 이전의 Scroll 위치를 기억하였다가, 새로운 위치로 Scrolling을 처리한다.
	else
	{
		// 2.2 Packet Data를 추가한다.
		m_pPacketList->SetPacketData( szPacketName, szData, lLength);
	}

	return true;
}
//sy end

// (2004.05.17, 배승원) DLL에서 Block BackColor와 OCX BackColor(Scale Area)를 일괄 지정할 수 있도록,
//		Interface를 제공한다.
void CKTBChartCtrl::SetBackgroundColors(OLE_COLOR p_rgbBlockArea, OLE_COLOR p_rgbScaleArea) 
{
	// TODO: Add your dispatch handler code here
	if( !m_pMainBlock) return;

	m_clrBlock		= p_rgbBlockArea;
	m_clrBackground	= p_rgbScaleArea;

	CClientDC dc(this);	
	CGraphRegionColor BlockColor(m_clrBlock, m_clrLattice, m_clrBlockBorder);
	m_pMainBlock->SetBlockEnvironment(&dc, GetMainBlockRegion(), (CBlockBaseData::BLOCKTYPE)m_nBlockType, 
		(CBlockBaseData::BLOCKCOMPART)m_nBlockCompart, CBlockIndex(m_nBlockRowCount, m_nBlockColumnCount), 
		m_nBlockBorderThickness, m_nBlockGap, BlockColor) ;
	InvalidateControl() ;
}

// (2004.05.18, 배승원) 전체 지표설정창을 출력시키는 Interface를 제공한다.
void CKTBChartCtrl::ShowIndicatorConfigDlg() 
{
	// TODO: Add your dispatch handler code here
	RunToolFunction( CToolOptionInfo::T_INDICATOR_CFG, 1);
}

// (2004.05.19, 배승원) 보유하고 있는 Data Packet의 시작일자 정보를 조회하는 Property를 제공한다. (Read Only)
BSTR CKTBChartCtrl::GetStartDateOfData() 
{
	CString strResult;
	// TODO: Add your property handler here

	if( m_pPacketList)
		strResult = CDataConversion::IntToString( m_pPacketList->GetStartDate(),
			m_pPacketList->GetPacketType( _MTEXT( C0_DATE_TIME)).GetLength());

	return strResult.AllocSysString();
}

//sy 2006.03.02
// JSR 20060119 Modify Begin
// (2004.05.20, 배승원) DLL Container에서 지정한 Packet 초기화 상태까지만 RollBack 시키는 기능을 추가한다.
//		0로 초기화를 시도하는 경우, DLL Container에서 지정한 Packet 초기화 상태까지만 RollBack시킨다.
//		즉 DLL Container가 있는 경우,
//			-1,		OCX 초기화		~ 상태 Index 0 (Reset OCX State, No Graph)
//			0,		지표 초기화		~ 상태 Index 1 (Reset Chart State, 기본 Graph - 가격, 이평, 거래량)
//			1 & 2,	단계 RollBack	~ 상태 Index는 최소 1 (RollBack State)
/*CChartCfg *CKTBChartCtrl::GetChartCfgWithPacketReset( int nOption)
{
	int nRollBackCount = 0;
	CChartCfg * pChart = NULL;
	switch( nOption)
	{
		case -1:	pChart = m_SaveChartcfg.GetChartCfg( 0);
					// (2004.07.07, 배승원) DLL에서 Reset OCX를 처리하는 경우로,
					//		이후 Reset Chart에서 복원되는 오류가 없도록, 기본 설정까지 지운다.
					m_SaveChartcfg.RemoveFromIndexToEnd( 1);
					break;
		case 0:		pChart = m_SaveChartcfg.GetChartCfg( 1);
					// (2004.09.10, 배승원) DLL Container가 있는 경우의 전체 초기화는 모든 지표의 속성을 Default 속성의 것으로 재수정한다.
					//		초기화하기 이전에 변경된 Default 속성(User Default)으로 설정되도록 한다.
					if( pChart && m_hContainerDllWnd)
					{
						// 1. Chart List String을 Graph별로 분할한다.
						//		또한, 분할 String을 구하면서 바로 Graph의 Defalt String을 구한다.
						//		그리고, 다시 조립을 하기 위한 전체 String의 길이도 바로 구한다.
						// 1.1 먼저 앞에서 부터 분할 위치를 확인한다. (실분할은 뒤부터 끝을 지워오면서 처리한다.)
						// 1.1.0 분할을 위하여 Buffer에 Data를 옮긴다.
						// 1.1.0.1 Chart List String을 구한다.
						CString strChartList = pChart->GetChartList();
						// 1.1.0.2 Buffer에 Chart List String을 옮긴다.
						CString strChartListBuffer;
						char *szChartList = strChartListBuffer.GetBufferSetLength( strChartList.GetLength());
						strcpy( szChartList, strChartList);
						// 1.2 앞부터 분할 위치를 Array로 확보한다.
						static const CString			StrGraphStartText	= "Graph;\r\n";
						static const CString			StrGraphEndText		= "GraphEnd;\r\n";
						CTypedPtrList<CPtrList, char *>	szaChartList;
						while( szChartList)
						{
							// 1.2.1 Graph 앞부분을 분할 위치로 등록한다.
							szaChartList.AddTail( szChartList);
							// 1.2.2 Graph 부분을 찾는다. ("Graph;\r\n" 부터 "GraphEnd;\r\n"까지 이다.)
							szChartList = strstr( szChartList, StrGraphStartText);
							// 1.2.3 Graph 부분이 없다면 분할 위치 확인을 끝낸다.
							if( !szChartList) break;
							// 1.2.4 Graph 부분을 다음 분할 위치로 등록한다.
							szaChartList.AddTail( szChartList);
							// 1.2.5 Graph의 끝부분을 검색하여, 다음 Graph를 등록시킨다.
							szChartList = strstr( szChartList, StrGraphEndText);
							while( szChartList)
							{
								// 1.2.5.1 Graph 다음 부분을 구한다.
								szChartList += StrGraphEndText.GetLength();
								// 1.2.5.2 또 Graph가 아니라면, 연속 Graph 등록을 취소한다. (같은 Vertical Scale Group의 등록 끝)
								if( strncmp( szChartList, StrGraphStartText, StrGraphStartText.GetLength())) break;
								// 1.2.5.3 다음 Graph를 분할 위치로 등록시킨다.
								szaChartList.AddTail( szChartList);
								// 1.2.5.4 Graph의 다음 부분을 검색하여, 다음 Graph를 등록시킨다.
								szChartList = strstr( szChartList, StrGraphEndText);
							}
						}
						// 1.3 Graph의 부분이 없는 경우, 분할을 진행하지 않는다.
						if( szaChartList.GetCount() == 1) break;
						// 1.4 분할 위치의 끝에서 부터 분할을 진행한다.
						//		이때 기존의 Graph 설정 String을 Default 속성의 Graph String으로 변경하여 등록한다.
						//		그리고, 다시 조립을 하기 위한 전체 String의 길이도 바로 구한다.
						int				nFullLength = 0;
						char *			szGraphNameStart = NULL, *szGraphNameEnd = NULL;
						CString			strDefaultGraph;
						POSITION		posChartList = szaChartList.GetTailPosition();
						CStringList	straChartList;
						while( posChartList)
						{
							// 1.4.1 분할 String을 등록한다.
							//		(이때 기존의 Graph 설정 String을 Default 속성의 Graph String으로 변경하여 등록한다.)
							// 1.4.1.1 분할 String을 구한다.
							szChartList = szaChartList.GetPrev( posChartList);
							// 1.4.1.2 Graph String인지 확인하여 Default Graph String을 등록한다.
							if( !strncmp( szChartList, StrGraphStartText, StrGraphStartText.GetLength()))
							{
								// 1.4.1.2.1 지표명을 찾는다. ( "Graph;\r\n" 이후 첫 ';' 다음에 다음 ';'가지가 지표명이다.)
								szGraphNameStart = strchr( szChartList + StrGraphStartText.GetLength(), ';');
								if( szGraphNameStart)
								{
									szGraphNameStart++;
									// 1.4.1.2.2 지표명의 끝을 찾아 자른다.
									szGraphNameEnd = strchr( szGraphNameStart, ';');
									if( szGraphNameEnd)
									{
										*szGraphNameEnd = '\0';
										// 1.4.1.2.3 Default Graph String을 구하여 등록한다.
										// 1.4.1.2.3.1 지표명에 해당하는 Indicator Info를 구한다.
										CIndicatorInfo *pIndicatorInfo = m_pIndicatorList->GetIndicatorInfo( EILT_USER, szGraphNameStart, 2);
										if( pIndicatorInfo)
										{
											// 1.4.1.2.3.2 기본 Indicator 정보에 의해 Graph Option Data를 구성한다.
											// (2004.10.06, 배승원) 가격차트의 Graph Drawing Data를 지정받아 DefaultType에 따라 관리, 적용토록 한다.
											strDefaultGraph = GetGraphDataForMaking( pIndicatorInfo);
											straChartList.AddHead( strDefaultGraph);
											// 1.4.1.2.4 전체 String의 길이를 감안한다.
											nFullLength += strDefaultGraph.GetLength();
											// 1.4.1.2.5 Default Graph가 등록 완료되면, 이전 Graph String을 Clear하여 이미 등록되었음을 설정한다.
											*szChartList = '\0';
										}
									}
								}
							}
							// 1.4.1.2 Graph String이 등록된 것이 아니라면 그냥 등록한다.
							if( *szChartList)
							{
								straChartList.AddHead( szChartList);
								// 1.4.1.2.1 전체 String의 길이를 감안한다.
								nFullLength += strlen( szChartList);
								// 1.4.1.3 다음 String의 분할 위치를 등록시 분할 부분만 등록되도록 이번 분할 부분의 String을 끊는다.
								//		Default Graph String이 등록된 경우에는 이미 Clear되어 있다.
								*szChartList = '\0';
							}
						}
						// 2. 분할 String을 다시 합친다.
						// 2.1 전체 길이의 Buffer를 확보한다.
						szChartList = strChartListBuffer.GetBufferSetLength( nFullLength);
						// 2.2 분할 String을 합친다.
						posChartList = straChartList.GetHeadPosition();
						while( posChartList)
						{
							// 2.2.1 분할 String을 구한다.
							strcpy( szChartList, straChartList.GetNext( posChartList));
							szChartList += strlen( szChartList);
						}
						// 3. 구해진 새로운 Chart List String을 다시 Chart CFG에 등록시킨다.
						pChart->SetChartList( strChartListBuffer);
					}
					break;
		// ?. 왜 RollBack Count가 이렇지?
		case 1: nRollBackCount = 2;
		case 2: nRollBackCount = 1;
		{
			int nChartIndex = m_SaveChartcfg.GetCfgCount() - nRollBackCount;
			if( nChartIndex < 1) nChartIndex = 1;
			pChart = m_SaveChartcfg.GetChartCfg( nChartIndex);
			break;
		}
	}
	return pChart;
}

// (2004.05.20, 배승원) DLL Container에서 지정한 Packet 초기화 상태까지만 RollBack 시키는 기능을 추가한다.
//			Packet 마저도 초기화시키는 Interface를 제공한다.
//		다른 Routine에서는 DLL Container가 없는 경우 ResetSaveChartCfg( 0)로 Chart 초기화가 이뤄지며,
//			이 Routine을 통해서면 OCX 초기화가 이뤄진다.
void CKTBChartCtrl::ResetOCX() 
{
	// TODO: Add your dispatch handler code here

	// (2004.08.31, 배승원) OCX를 초기화하는 경우, 전 Packet Data도 Clear시킨다.
	if( m_pPacketList) m_pPacketList->ClearDataAll( FALSE);

	// (2004.07.16, 배승원) Reset시에는 분석도구 등도 Clear되도록 OnReturnOrg2()를 호출한다. (MessageBox 제외)
	OnReturnOrg2( FALSE);

	ResetSaveChartCfg( -1);
}*/

//sy 2006.03.02
// JSR 20060119 Modify Begin
//chartCfg 내용을 특정위치까지 삭제.
//->  0이하 : 모두 삭제하고 현생태를 default 로 바꿈 -> 총 1개
//    1		: 가장첫번째 값(SetAllProperty2)을 default로 두고 현상태를 cfg로 저장 -> 총 2개
//    n		: 중간부분을 삭제.
void CKTBChartCtrl::ResetChartCfgWith( short p_nIndex) 
{
 	if(p_nIndex <= 0)
		m_SaveChartcfg.RemoveAll();
	else
		m_SaveChartcfg.RemoveFromIndexToEnd(p_nIndex);

	SaveChartCfg();

	//sy 2006.02.20. -> map default
	if(p_nIndex <= 0) SetMapDefaultInCurrent( false);
}
// JSR 20060119 Modify End.

// JSR 20060119 ADD Start.
/*///////////////////////////////////////////////////////////////////////////////
// Made By		 : Yoo Je Sun 
// MadeDate		 : 2006/01/17
// Function Name : EmptyChart
// Parameter Info: p_szChartList : Graph Build String Info (CL)
				   bIsDeleteAllPacket : true -> TRdata, 지표data 모두 삭제
										false -> 지표data만 삭제
				   bIsDefault : true -> 빈상태을 디폴트값으로 변경
								false -> 디폴트값으로 변경 안함 
///////////////////////////////////////////////////////////////////////////////*/
BOOL CKTBChartCtrl::EmptyChart(BOOL bIsDeleteAllPacket, BOOL bIsDefault)
{
	CString strChartList =	"BlockColumn;\r\n"
							"Block;\r\n";
	strChartList += _MTEXT( C0_DATE_TIME);
	strChartList +=					";4;2;/;:;0;0;0;\r\n"
							"VertScaleGroup;\r\n";
	strChartList += _MTEXT( C6_INDICATOR_DATA);
	strChartList +=					";1;2;0;\r\n"
							"VertScaleGroupEnd;\r\n"
							"BlockEnd;\r\n"
							"BlockColumnEnd;";
	return EmptyChart2( strChartList, bIsDeleteAllPacket, bIsDefault);
}
BOOL CKTBChartCtrl::EmptyChart2(LPCTSTR p_szChartList, BOOL p_bIsDeleteAllPacket, BOOL p_bIsDefault) 
{
	// TODO: Add your dispatch handler code here

	if(m_pPacketList == NULL || m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return false;

	RunToolFunction(CToolOptionInfo::T_DELETE_ALL, 1);

	// OCX의 멤버값 변경/적용
	SetEmptyOcxData( p_szChartList);

	// 모든 Packet Data를 Clear
	m_pPacketList->ClearDataAll( p_bIsDeleteAllPacket);
	m_pIndicatorList->RemoveAllIndicatorInfo( EILT_USER);
	m_pMainBlock->Initialize_IndicatorInfo( FALSE);

	// 스크롤 적용 : data 갯수가 틀려졌기때문
	ResetScroll();

	// 현재상태를 default로 저장
	if( p_bIsDefault) ResetChartCfgWith(0);

	return true;
}
// Emtpy 속성
void CKTBChartCtrl::SetEmptyOcxData( const char *p_szChartList)
{
	// (2006/10/27 - Seung-Won, Bae) Init with One Empty Block.
	m_nBlockRowCount  = 1;
	m_nBlockColumnCount = 1;
	m_strChartList = p_szChartList;
	m_strBlockRegion = "";

	//적용
	SetMainBlock_MainBlock(true);
}
// JSR 20060119 ADD End.
//sy end

BOOL CKTBChartCtrl::HasGraph(LPCTSTR p_szGraphName2) 
{
	// TODO: Add your dispatch handler code here

	CString p_szGraphName = _STOM2( p_szGraphName2);
	return HasIndicator( p_szGraphName);
}

//sy 2004.12.07. 특수&일반차트 저장과 관련된 부분.
//지표추가하기 -> 특수와 일반을 구분하여 저장&삭제한다.
BOOL CKTBChartCtrl::AddChartBlock(LPCTSTR strChartName2) 
{
	CString strChartName = _STOM2( strChartName2);	

	CString strName = strChartName;
	strName.TrimLeft();
	strName.TrimRight();
	if(strName.IsEmpty() || m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return FALSE;

	//특수차트인 경우
	if( g_iMetaTable.IsSpecialGroupGraph(strName))
		return AddOrDeleteChart_Special(strName);

	//일반차트인 경우
	return AddOrDeleteChart_Genneral(strName);
}

//sy 2005.03.03.
bool CKTBChartCtrl::AddOrDeleteChart(const CString& strChartName)
{
	CString strName = strChartName;
	strName.TrimLeft();
	strName.TrimRight();
	if(strName.IsEmpty() || m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return FALSE;

	//특수차트인 경우
	if( g_iMetaTable.IsSpecialGroupGraph(strName))
		return AddOrDeleteChart_Special(strName);

	//일반차트인 경우
	return AddOrDeleteChart_Genneral(strName);
}
//sy end

//sy 2004.12.06. 특수차트와 일반차트의 저장관련 부분.
//-> 특수차트 추가&삭제
bool CKTBChartCtrl::AddOrDeleteChart_Special( const CString &strIndicatorName)
{
	if(m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return false;

	if( !g_iMetaTable.IsSpecialGroupGraph( strIndicatorName))
		return false;

	// 0. Do not remove current Special Chart in Multiple Same-Name Indicator)
	if( m_pMainBlock->IsExistenceIndicator( strIndicatorName)) return false;

	//현재화면이 특수차트 & add 할 차트랑 이름이 같은 경우
	//-> 특수차트가 아닌 마지막 차트로 복원.
	if(m_pMainBlock->IsExistenceIndicator( strIndicatorName))
	{
		RollbackLastNoOneChart();
	}
	else
	{
		CString strGraphName;
		ChangeOneChart( strIndicatorName, strGraphName);
	}

	return true;
}

//-> 일반차트 추가&삭제
// 모든 RQ의 그래프의 차트 변경 - ojtaso (20080516)
bool CKTBChartCtrl::AddOrDeleteChart_Genneral(const CString& strChartName, const BOOL bSelect/* = TRUE*/)
{
	if(m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return false;

	//지표가 존재하는 경우 지표삭제
	// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
	if( !m_bMultipleSameNameIndicator)
	{
		if( DeleteChart_Genneral(strChartName)) 
		{
			// 일목균형표 삭제 : 복수종목 - ojtaso (20070122)
//			if(m_pxScaleManager && strChartName.CompareNoCase( _MTEXT( C2_OBVIOUSLY_BALANCE_CHART)))
//				m_pxScaleManager->DelOEBTRQ(m_strRQ);

			return true;
		}
	}

	//특수차트가 아닌 화면으로 돌아간다.
	//->특수차트(원차트)가 아닌 마지막의 상태로 복원
	RollbackLastNoOneChart();

	// (2006/6/12 - Seung-Won, Bae) Delete General Graph with Special Chart Back.
	// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
	if( !m_bMultipleSameNameIndicator) if( DeleteChart_Genneral( strChartName)) return true;

	//가격차트의 style 변경
	CString strGraphName;
	// 모든 RQ의 그래프의 차트 변경 - ojtaso (20080516)
	if( ChangeChart_PriceChartStyle( strChartName, strGraphName, bSelect)) return true;

   	//일반차트 추가
	BOOL bReturn = AddChart_GenneralChart(strChartName, strGraphName);

	return bReturn;
}

//sy 2006.03.31.
//-> 일반차트 Style 변경 : user형태
BOOL CKTBChartCtrl::AddChart_GenneralChart( const CString &strIndicatorName, CString &p_strGraphName, BOOL p_bWithPair)
{
	if(m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return false;

	// (2007/1/10 - Seung-Won, Bae) Delete Previous IndicatorInfo for avoiding the same-Name Indicator Info Situation.
	// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
	if( !m_bMultipleSameNameIndicator)
	{
		if( m_pMainBlock->IsExistenceIndicator( strIndicatorName)) return false;
		m_pIndicatorList->RemoveAllIndicatorInfo( EILT_USER, strIndicatorName);
	}

	// (2008/9/14 - Seung-Won, Bae) for Pair Tick
	CString strRQ = m_strRQ;
	for( int i = 0; i < 2; i++)
	{
		LPVOID pPacketRQ = m_pPacketListManager->GetPacketRQ( strRQ);
		if( !pPacketRQ) continue;
		
		// (2007/1/22 - Seung-Won, Bae) Check the Indicator that can not be added with mulitple.
		if( g_iMetaTable.CanNotMultipleAddedOnMultipleSameNameIndicator( strIndicatorName) || g_iMetaTable.IsMarketOrServerIndex( strIndicatorName))
			if( m_pMainBlock->IsExistenceIndicator(pPacketRQ, strIndicatorName)) return false;

		// (2004.10.06, 배승원) 가격차트의 Graph Drawing Data를 지정받아 DefaultType에 따라 관리, 적용토록 한다.
		CIndicatorDefaultData indiDefaultData(this);
		GetIndicatorDefaultData( indiDefaultData);
		CString szNewGraph;
		indiDefaultData.GetVertScaleGroupDataForMaking( szNewGraph, m_pIndicatorList, strIndicatorName, p_strGraphName);

		if(!m_pMainBlock->InsertGraph(pPacketRQ, szNewGraph, CGraphBaseData::INSERT, true))
			return false;

		if( !p_bWithPair) break;
		if( strRQ.Right( 5) == "_PAIR")	strRQ = strRQ.Left( strRQ.GetLength() - 5);
		else							strRQ = strRQ + "_PAIR";
	}
	
	InvalidateControl();
	// 추가된 사항을 저장
	SaveChartCfg();
	return true;
}

//-> 일반차트 삭제
bool CKTBChartCtrl::DeleteChart_Genneral( const CString &strIndicatorName)
{
	if(m_pMainBlock == NULL)
		return false;

	//2.2 지표가 존재하는 경우 지표삭제
	if( !m_pMainBlock->IsExistenceIndicator( strIndicatorName))
		return false;

	m_pMainBlock->DeleteAllIndicator( strIndicatorName);

	InvalidateControl();
	return true;
}

//-> 가격차트 Style 변경
// 모든 RQ의 그래프의 차트 변경 - ojtaso (20080516)
BOOL CKTBChartCtrl::ChangeChart_PriceChartStyle( const CString &p_strIndicatorName, CString &p_strGraphName, const BOOL bSelect/* = TRUE*/)
{
	if(m_pIndicatorList == NULL || m_pMainBlock == NULL) return FALSE;

	// (2008/9/12 - Seung-Won, Bae) for Price Chart Style.
	CString strChartName = _STOM5( p_strIndicatorName);	

	if( !g_iMetaTable.IsPriceChartType( strChartName)) return FALSE;

	CString strGraphName;
	int nR = -1, nC = -1, nV = -1, nG = -1;
	// 모든 RQ의 그래프의 차트 변경 - ojtaso (20080516)
	CList < CString, CString> slRQs;
	if( bSelect) slRQs.AddTail(m_strRQ);
	else
	{
		m_pPacketListManager->GetAllRQ( slRQs);
		slRQs.AddTail(_T("DEFAULT"));
	}
	
	BOOL bAllResult = TRUE;
	POSITION psn = slRQs.GetHeadPosition();
	while( psn)
	{
		CString strRQ = slRQs.GetNext( psn);

		// RQ로 검색 : 복수종목 - ojtaso (20070621)
		if( !m_pMainBlock->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, strGraphName, strRQ))
			continue;

		BOOL bResult = FALSE;
		CString strPackets = CString( _MTEXT( C0_OPEN)) + "," + _MTEXT( C0_HIGH) + "," + _MTEXT( C0_LOW) + "," + _MTEXT( C0_CLOSE);
		// 1.1.2 가격차트의 유형을 지정한다.
		if( !stricmp( strChartName, _MTEXT( C5_CANDLE_CHART)))						bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_CANDLE_CHART),					strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_BAR_CHART)) || !stricmp( strChartName, _MTEXT( C5_BAR_CHART_OPEN_HIGH_LOW_CLOSE)))
																					bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_BAR_CHART_OPEN_HIGH_LOW_CLOSE),	strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_LINE_CHART)))						bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_LINE_CHART),						strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_ANTENNA_CHART)))						bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_ANTENNA_CHART),					strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_BAR_CHART_HIGH_LOW_CLOSE)))			bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_BAR_CHART_HIGH_LOW_CLOSE),		strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_FLOW_CHART)))						bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_FLOW_CHART),						strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_CANDLE_VOLUME_CHART)))				bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_CANDLE_VOLUME_CHART),				strPackets);
		if( !stricmp( strChartName, _MTEXT( C5_EQUI_VOLUME_CHART)))					bResult = ChangeChartType( strGraphName, _MTEXT( C5_CANDLE_TYPE), _MTEXT( C5_EQUI_VOLUME_CHART),				strPackets);

		bAllResult = bAllResult && bResult;
	}
	
	return bAllResult;
}

// (2004.05.25, 배승원) 내부 ToolBar를 보이고, 숨기는 Interface를 제공한다.
//		별도의 Flag을 두어, ToolBar를 OCX의 영역보다 아래쪽에 위치되도록 한다.
//		(주의! ToolBar가 없는 경우에 ToolBar를 숨기는 설정을 하지 않도록 한다.)
void CKTBChartCtrl::ShowInnerToolBar(BOOL p_bShow) 
{
	// TODO: Add your dispatch handler code here

	if( m_bShowInnerToolBar == p_bShow) return;
	m_bShowInnerToolBar = p_bShow;
	OnWindowPosChanged( NULL);
	// (2004.06.04, 배승원) Block의 배치도 이에 따라 조정되도록 OnSize()도 호출해 준다.
	OnSize( 0, 0, 0);
}

// (2004.06.02, 배승원) Scale의 위치를 조정하는 Interface를 제공한다.
//		Block Index나 Vertical Scale Group Index중 하나라도 음수인 경우 전체 일괄 지정으로 간주한다.
BOOL CKTBChartCtrl::ChangeHorzScalePosition(short nScalePosition, short nRowIndex, short nColumnIndex) 
{
	// TODO: Add your dispatch handler code here
	if(m_pMainBlock == NULL)
		return FALSE;

	BOOL bResult = FALSE;
	if( nColumnIndex < 0 || nRowIndex < 0)
	{
		m_pMainBlock->SetAllHorzScalePosition((CScaleBaseData::HORZSCALEPOSITION) nScalePosition);
		bResult = TRUE;
	}
	else
		bResult = m_pMainBlock->SetHorzScalePosition(CBlockIndex(nRowIndex, nColumnIndex), (CScaleBaseData::HORZSCALEPOSITION) nScalePosition);

	if(bResult) Invalidate();

	return TRUE;
}
BOOL CKTBChartCtrl::ChangeVertScalePosition(short nScalePosition, short nRowIndex, short nColumnIndex, short nVSGroupIndex) 
{
	// TODO: Add your dispatch handler code here
	if(m_pMainBlock == NULL)
		return FALSE;

	BOOL bResult = FALSE;
	if( nColumnIndex < 0 || nRowIndex < 0 || nVSGroupIndex < 0)
	{
		m_pMainBlock->SetAllVertScalePosition((CScaleBaseData::VERTSCALEPOSITION) nScalePosition);
		bResult = TRUE;
	}
	else
		bResult = m_pMainBlock->SetVertScalePosition(CBlockIndex(nRowIndex, nColumnIndex), nVSGroupIndex, (CScaleBaseData::VERTSCALEPOSITION) nScalePosition);

	if(bResult) Invalidate();

	return TRUE;
}

// (2004.06.02, 배승원) 현재 Chart Block의 행/열 수를 구하는 Interface를 제공한다.
//		*. 주의 : 기존 Property는 지표추가시에 자동으로 증가한 Block 행을 인식하지 못하는 경우가 있다.
short CKTBChartCtrl::GetRowBlockCount() 
{
	// TODO: Add your dispatch handler code here
	if( !m_pMainBlock) return 0;
	return m_pMainBlock->GetRowCount();
}

short CKTBChartCtrl::GetColumnBlockCount() 
{
	// TODO: Add your dispatch handler code here
	if( !m_pMainBlock) return 0;
	return m_pMainBlock->GetColumnCount();
}

// (2004.06.04, 배승원) Scroll이 변경될 때마다 DLL Container에게 끝일시를 전달한다.
// (2004.06.25, 배승원) p_nIndex값을 확인한다.
// (2005/2/17 - Seung-Won, Bae) Add Message Parameter for UWM_DRAGSTART/UWM_DRAGEND
void CKTBChartCtrl::SendTimeOfIndexToDLL( UINT p_uMessage, int p_nIndex)
{
	// 1. 자료일자를 통하여, 해당 Index의 시간을 구한다.
	if( !m_pPacketList || p_nIndex < 0) return;
	CPacket* pPacket = m_pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pPacket) return;
	if( pPacket->GetCount() <= p_nIndex) return;
	long lDate = pPacket->GetData( p_nIndex);
	// 2. 구해진 일자를 DLL Container에게 전달한다.
	SendEventMsgToDLL( p_uMessage, 0, lDate);
}

// (2004.06.08, 배승원) 지정되는 Packet의 시작일자를 지정할 수 있도록 한다.
BOOL CKTBChartCtrl::SetStartPeriodOfPacket(LPCTSTR p_szPacketName2, long p_lStartPeriod, BOOL p_bAddSpace) 
{
	// 1. 먼저 Packet List를 확인한다.
	if( !m_pPacketList) return FALSE;

	CString p_szPacketName = _STOM0( p_szPacketName2);
	// 2. 지정한 Packet을 검색한다.
	CPacket *pPacket = m_pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return FALSE;
	
	// 3. 검색된 Packet의 시작일자에 해당하는 Data Index를 구한다.
	CPacket *pTimePacket = m_pPacketList->GetBaseData( _MTEXT( C0_DATE_TIME));
	if( !pTimePacket) return FALSE;
	int nStartIndex = pTimePacket->GetIndexFromData( p_lStartPeriod);

	// 4. p_bAddSpace에 따라 시작 Index 만큼 Packet Data를 Shift한다.
	// (2004.06.18, 배승원) 추후에 한번더 공백을 추가하면서, StartPeriod를 지정할 수 있도록
	//		Packet에 추가하는 공백의 수를 StartIndex가 아닌, 자료일자와의 Data수 차이로 한다.
	if( p_bAddSpace)
	{
		CList<double, double>* pDblList = pPacket->GetnumericDataList();
		if( !pDblList) return FALSE;
		int nAddingCount = pTimePacket->GetCount() - pPacket->GetCount();
		for( int i = 0; i < nAddingCount; i++) pDblList->AddHead( 0.0);
	}

	// TEST: 종찬(05/08/27) 시작 Index이전값을 모두 0.0으로 세팅한다.
//	CList<double, double>* pTmpList = pPacket->GetnumericDataList();
//	POSITION pos;
//	for(int i = 0; i < nStartIndex; i++)
//	{
//		pos = pTmpList->FindIndex(i);
//		if(!pos)
//			continue;
//		pTmpList->SetAt(pos, 0.0);
//	}

	// 4. Packet의 시작 Index를 지정한다.
	pPacket->SetStartPeriod( nStartIndex);

	return TRUE;
}
long CKTBChartCtrl::GetStartPeriodOfPacket(LPCTSTR p_szPacketName2) 
{
	// TODO: Add your dispatch handler code here
	
	// 1. 먼저 Packet List를 확인한다.
	if( !m_pPacketList) return -1;

	CString p_szPacketName = _STOM0( p_szPacketName2);
	// 2. 지정한 Packet을 검색한다.
	CPacket *pPacket = m_pPacketList->GetBaseData( p_szPacketName);
	if( !pPacket) return -1;

	// 3. 검색된 Packet의 시작일자를 설정한다.
	return m_pPacketList->GetDataFromIndex( _MTEXT( C0_DATE_TIME), pPacket->GetStartPeriod());
}

//sy 2005.04.26.
LRESULT CKTBChartCtrl::RmsgFormStateChange(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case 500:
		ApplySkin();
		break;
	}
	
	return 0L;	
}
//sy end

// (2004.07.26, 배승원) TimeMark를 Clear시키는 Method를 제공한다.
void CKTBChartCtrl::ClearTimeMark() 
{
	// TODO: Add your dispatch handler code here

	// (2004.10.11, 배승원) TimeMark 기능을 AddIn DLL로 옮긴다.
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_CUSTOM_CLEAR_TIMEMARK);
}

// (2004.07.27, 배승원) 환경설정 Dialog를 보이는 Method를 제공한다.
void CKTBChartCtrl::ShowEnvironmentDlg() 
{
	// TODO: Add your dispatch handler code here
	RunToolFunction( CToolOptionInfo::T_ENVIRONMENT_CFG, 1);
}

// (2004.08.24, 배승원) 시간 Line을 설정하는 Interface를 제공한다.
//		Mark Time String의 길이에 따라 다음 3가지로 인식한다.
//			HHMMSS			:  6자리
//			YYYYMMDD		:  8자리
//			YYYYMMDDHHmmSS	: 14자리
void CKTBChartCtrl::SetTimeMark(LPCTSTR p_szMarkTime, BOOL p_bWithScroll) 
{
	// TODO: Add your dispatch handler code here

	// (2004.10.11, 배승원) TimeMark 기능을 AddIn DLL로 옮긴다.
	if( m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_CUSTOM_SET_TIMEMARK, p_szMarkTime, p_bWithScroll);
}

// (2004.09.16, 배승원) DLL Container에서 Y Scale의 위치 정보를 관리하면서 재설정할 수 있도록 조회 Interface를 제공한다.
//		-1이 Return되는 경우, 해당 Scale이 존재하지 않는 경우이다.
short CKTBChartCtrl::GetVertScalePosition(short p_nRowIndex, short p_nColumnIndex, short p_nVSGroupIndex) 
{
	// TODO: Add your dispatch handler code here

	if(m_pMainBlock == NULL) return -1;
	if( p_nRowIndex < 0 || p_nColumnIndex < 0 || p_nVSGroupIndex < 0) return -1;

	return m_pMainBlock->GetVertScalePosition( CBlockIndex( p_nRowIndex, p_nColumnIndex), p_nVSGroupIndex);
}

// 차트DLL에서 복수종목을 사용할지 : 복수종목 - ojtaso (20070904)
void CKTBChartCtrl::SetMultiItemMode(BOOL bMultiItem) 
{
	if(m_bUseMultiItemMode == bMultiItem)
		return;

	m_bUseMultiItemMode = bMultiItem;
	
	if(m_pMainBlock)
		m_pMainBlock->SetMultiItemMode(bMultiItem);
}

// 모든 RQ의 그래프의 차트 변경 - ojtaso (20080516)
BOOL CKTBChartCtrl::AddChartAllBlock(LPCTSTR strChartName) 
{
	CString strName = strChartName;
	strName.TrimLeft();
	strName.TrimRight();
	if(strName.IsEmpty() || m_pIndicatorList == NULL || m_pMainBlock == NULL)
		return FALSE;

	//특수차트인 경우
	if( g_iMetaTable.IsSpecialGroupGraph(strName))
		return AddOrDeleteChart_Special(strName);

	//일반차트인 경우
	return AddOrDeleteChart_Genneral(strName, FALSE);
}
