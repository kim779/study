// KTBChartReal.cpp: implementation of the Real Processing Routine
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/DLL_Load/AddInManager.h"			// for CAddInManager
#include "../Include_Chart/DLL_Load/IMetaTable.h"			// for _STOM0()
#include "../Include_Chart/DLL_Load/XScaleManager.h"		// for CXScaleManager
#include "../Include_Chart/IRealReceiver.h"					// for IRealReceiver
#include "../Include_Chart/IOldFXRealReceiver.h"			// for IOldFXRealReceiver
#include "../Include_Chart/IOldHTSRealReceiver.h"			// for IOldHTSRealReceiver
#include "../Include_Chart/MessageDefine.h"					// for UDM_RECALC_DRI
#include "../AddInManager/Include_AddIn/AddInCommand.h"		// for EAI_LOG_REAL_SETTING_CHANGED
#include "KTBChartCtl.h"									// for CKTBChartCtrl
#include "Conversion.h"										// for CDataConversion
#include "MainBlock.h"										// for CMainBlock
#include "ToolBarManager.h"									// for IToolBarManager
#include "LocalShareInfoList.h"								// for CLocalShareInfo
#include "PacketList.h"										// for CPacketList
#include "PacketListManager.h"								// 여러 PacketList를 관리 : 복수종목 - ojtaso (20061224)
#include "OutsideCommonInterface.h"								// 복수종목 선택/삭제 : 복수종목 - ojtaso (20070209)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// (2004.11.17, 배승원, 분석) Real 관련 Property 처리 Routine
//////////////////////////////////////////////////////////////////////

// (2004.11.17, 배승원, 분석) m_bUseRealData (Real 이용여부) 변경 처리 Routine이다.
//	(주의! 그러나 실제로 FALSE로 설정된 Map에서는 Script로 TRUE로 변경하여 이용할 수 없다.
//		DRDS DLL이 Loading되지 않기 때문이다.)
void CKTBChartCtrl::OnbUseRealDataChanged() 
{
	BoundPropertyChanged(dispidBUseRealData);

	SetModifiedFlag();
}


//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2005/3/2
// Modifier		: 
// Comments		: Change Real Setting String with Adising
//-----------------------------------------------------------------------------
BOOL CKTBChartCtrl::ResetRealListString(LPCTSTR p_szRealList, BOOL p_bAdviseNow) 
{
	// TODO: Add your dispatch handler code here

	_ML_TEST_NOT_USED();
	// 1. Reset Real Setting String
	m_strRealList = p_szRealList;

	// 2. Update Real Setting Info
	OnstrRealListChanged();

	// 3. Advise Now
	if( p_bAdviseNow && m_pIRealReceiver)
	{
		m_pIRealReceiver->SetRealAdvise( FALSE);
		m_pIRealReceiver->SetRealAdvise( TRUE);
	}

	return TRUE;
}

// (2004.11.17, 배승원, 분석) m_strRealList (Real 설정)을 변경 처리하는 Routine이다.
//  (주의! 실제로 이 변경후 Real이 처리되기 위해서는 m_AdviseList의 자료구조를 재구축 하여야 하는데
//		이는 공유가 변경되어 설정되어야 반영된다.)
void CKTBChartCtrl::OnstrRealListChanged() 
{
	BoundPropertyChanged( dispidStrRealList);

	_ML_TEST_NOT_USED();
	SetModifiedFlag();

	// (2005/3/2 - Seung-Won, Bae) Update Real Setting Info
	// (2006/2/3 - Seung-Won, Bae) Show Real Setting Error Message
	if( m_pIRealReceiver) if( !m_pIRealReceiver->ResetRealSettingInfo( m_strRealList))
	{
		AfxMessageBox( "Error in real list string setting!\r\nPlease, check string again!\r\n\r\n" + m_strRealList);
		return;
	}

	// (2005/9/29 - Seung-Won, Bae) Log the New Real Setting
	if( m_bInLogging && m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_LOG_REAL_SETTING_CHANGED, m_strRealList);
}

// (2004.11.26, 배승원) Real의 분차트시 허수봉개념의 전체 시간을 지원할 것인지를 Flag으로 관리 지원한다.
//		특히, 별도의 Global을 속성을 두어 HTS 기본 설정을 관리하고,
//		별도의 Property를 제공하여 Chart별 속성을 변경토록 한다.
void CKTBChartCtrl::OnbNormalizedMinuteTimeRule() 
{
	BoundPropertyChanged( dispidBNormalizedMinuteTimeRule);

	SetModifiedFlag();

	// (2009/11/1 - Seung-Won, Bae) for Nomalized Minute Time Rule.
	if( CChartInfo::FX == m_nChartMode) m_bNormalizedMinuteTimeRule = TRUE;

// (2009/11/23 - Seung-Won, Bae) for DEV
//#ifdef _DEBUG
//	char szChartMode[ 256];
//	DWORD dwError = GetPrivateProfileString( "Chart Version", "MChartMode", "", szChartMode, 255, GetBaseDeskPath() + DATA_FILE_FOLDER + "ChartMetaTable.dat");
//	CString strChartMode( szChartMode);
//	if( strChartMode == "HTS") m_bNormalizedMinuteTimeRule = TRUE;
//#endif

	// (2005/11/7 - Seung-Won, Bae) Log for Normalized Minute Time Rule
	if( m_bInLogging && m_pAddInManager) m_pAddInManager->OnAddInToolCommand( EAI_LOG_SET_IMAGENARY_CANDLE, "CKTBChartCtrl::OnbNormalizedMinuteTimeRule", m_bNormalizedMinuteTimeRule);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.17, 배승원, 분석) Real Data Process Routine & Method
//////////////////////////////////////////////////////////////////////

// (2004.12.30, 배승원, 분석) Real의 1개 Field Data를 수신하는 Routine이다.
//		Notify Routine of 1 Real Field
//			int			wParam	~ Size of DrdsData, Fixed Value
//			DrdsData *	lParam	~ Data Structure
LONG CKTBChartCtrl::OnDrdsData( UINT wParam, LPARAM lParam)
{
	if( !m_pIRealReceiver) return 0;
	return m_pIRealReceiver->SetDrdsData( wParam, lParam);
}

// (2004.12.30, 배승원, 분석) Real의 1개 Packet 수신이 끝난 경우의 처리이다.
//		Notify Routine of 1 Real Packet Process End
//			const char *wParam	~ Packet Name
//			const char *lParam	~ Key Code
LONG CKTBChartCtrl::OnDrdsPacketEnd(UINT wParam, LPARAM lParam)
{
	if( !m_pIRealReceiver) return 0;
	return m_pIRealReceiver->SetDrdsPacketEnd( wParam, lParam);
}

// (2004.01.06, 배승원) BigBang의 MarketProfile Chart를 위하여 Interface를 Porting한다.
//	참고	:	nOption == 1	- update
//				nOption == 2	- append
//sy 2004.07.16. -> packet 이 삭제된 만큼 scroll 변경
void CKTBChartCtrl::SetRealData(LPCTSTR strPacketName2, double dData, short nOption) 
{
	if(!m_pPacketList)
		return;

	CString strPacketName = _STOM0( strPacketName2);
	// ADD: 종찬(04/08/03) PacketData에서 PacketData제거루틴추가에 의한 그 구분 Flag를 설정.
	int nRemovePacketDataCount = 0;
	int nResult = m_pPacketList->SetRealData( strPacketName, dData, nRemovePacketDataCount, nOption);
	if(nResult <= 0)
		return;
	
	// return : 0(처리 안됨)  1(처리됨 : packet삭제는 안됨)  2(처리됨 : packet삭제 됨)
	//sy 2003.3.7 - 속도 개선 - 스크립트로 실시간 data들어옴
	if(m_pMainBlock != NULL)
		m_pMainBlock->CalculateAllGraphs(CCalculateBaseData::REALDATA_CAL);

	// (2004.12.07, 배승원) Real의 반영에 따른 지표 계산을 싯점을 AddIn Module에게 알린다.
	if( m_pAddInManager) m_pAddInManager->OnManualRealData( strPacketName, dData, nOption);

	if( nRemovePacketDataCount)
	{
		// (2006/6/5 - Seung-Won, Bae) Notify to ST AddIn for Packet Data Cutting.
		if( m_pAddInManager) m_pAddInManager->OnPacketTruncated( nRemovePacketDataCount);

		// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
		if( m_pxScaleManager) m_pxScaleManager->SetFlagForCalcDRIandScrollByReal( TRUE);
	}

	// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
	if( m_pxScaleManager) m_pxScaleManager->SetFlagForCalcDRIandScrollByReal();

	InvalidateControl();
	// packet 이 삭제된 만큼 scroll 변경
	//if(nResult == 1)
	//	SetScrollData_RemovePacketPart();

	return;
}

// 복수종목을 위한 PacketList별 RealData : 복수종목 - ojtaso (20061224)
void CKTBChartCtrl::SetRealDataCode(LPCTSTR strCode, LPCTSTR strPacketName2, double dData, short nOption) 
{
	if(!m_pPacketListManager)
		return;

	CString strPacketName = _STOM0( strPacketName2);
	// ADD: 종찬(04/08/03) PacketData에서 PacketData제거루틴추가에 의한 그 구분 Flag를 설정.
	int nRemovePacketDataCount = 0;
	m_pPacketListManager->SetRealData(strCode, strPacketName, dData, nOption);
	
	// return : 0(처리 안됨)  1(처리됨 : packet삭제는 안됨)  2(처리됨 : packet삭제 됨)
	//sy 2003.3.7 - 속도 개선 - 스크립트로 실시간 data들어옴
	if(m_pMainBlock != NULL)
		m_pMainBlock->CalculateAllGraphs(CCalculateBaseData::REALDATA_CAL);

	// (2004.12.07, 배승원) Real의 반영에 따른 지표 계산을 싯점을 AddIn Module에게 알린다.
	if( m_pAddInManager) m_pAddInManager->OnManualRealData( strPacketName, dData, nOption);

	if( nRemovePacketDataCount)
	{
		// (2006/6/5 - Seung-Won, Bae) Notify to ST AddIn for Packet Data Cutting.
		if( m_pAddInManager) m_pAddInManager->OnPacketTruncated( nRemovePacketDataCount);

		// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
		if( m_pxScaleManager) m_pxScaleManager->SetFlagForCalcDRIandScrollByReal( TRUE);
	}

	// (2009/5/10 - Seung-Won, Bae) Scroll with added and trucated by Real
	if( m_pxScaleManager) m_pxScaleManager->SetFlagForCalcDRIandScrollByReal();

	InvalidateControl();
	// packet 이 삭제된 만큼 scroll 변경
	//if(nResult == 1)
	//	SetScrollData_RemovePacketPart();

	return;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.17, 배승원, 분석) Real Setting Methods
//////////////////////////////////////////////////////////////////////

// (2004.11.17, 배승원, 분석) 1개 Real을 바로 설정 및 요청하는 Method이다.
// (2004.05.18, 배승원) DLL Container로부터 Real Item을 설정받는 Interface를 제공한다.
BOOL CKTBChartCtrl::AddRealItem(LPCTSTR szRealInfo) 
{
	// TODO: Add your dispatch handler code here
	return AddRealItemWithSeparator( szRealInfo, ".");
}
// (2006/4/28 - Seung-Won, Bae) for Compare Chart in Future & Option of SK
//		Packet Name can have '.'. So, We need new separator.
BOOL CKTBChartCtrl::AddRealItemWithSeparator(LPCTSTR szRealInfo, LPCTSTR szSeparatorChar) 
{					
	// TODO: Add your dispatch handler code here
	if( !IsRunTimeMode() || !m_pPacketList || !szRealInfo || !szSeparatorChar)	return FALSE;
	if( !*szRealInfo || !*szSeparatorChar)										return FALSE;
	if( !m_pIRealReceiver)														return FALSE;
	// 1. Real Setting Info에 추가한다.
	CString strShareName, strSharedData;
	// (2006/2/3 - Seung-Won, Bae) on Error, return FALSE
	if( !m_pIRealReceiver->AddRealSettingInfoRQ(m_strRQ, szRealInfo, &strShareName, &strSharedData, *szSeparatorChar)) return FALSE;

	// 2. Local Shared Info에 추가한다.
	if( m_plLocalShareInfoList) m_plLocalShareInfoList->AddLocalShareInfo( strShareName, 0, strSharedData);

	return TRUE;
}

// (2004.11.17, 배승원, 분석) 1개 Real을 바로 취소 및 해제하는 Method이다.
// (2004.05.24, 배승원) 설정한 Real Item을 제거하는 Interface를 제공한다.
// (2004.11.18, 배승원, 주의) 또한 m_AdviseList의 등록 제거 방식에 있어서 Data Packet을 Key로 이용하지 않는 관계로,
//		같은 Real Data (하나의 Field를) 복수개의 Data Packet에 설정하지 못한다.
void CKTBChartCtrl::RemoveRealItem(LPCTSTR szRealInfo) 
{
	// TODO: Add your dispatch handler code here
	RemoveRealItemWithSeparator( szRealInfo, ".");
}
// (2006/4/28 - Seung-Won, Bae) for Compare Chart in Future & Option of SK
//		Packet Name can have '.'. So, We need new separator.
void CKTBChartCtrl::RemoveRealItemWithSeparator(LPCTSTR szRealInfo, LPCTSTR szSeparatorChar) 
{					
	// TODO: Add your dispatch handler code here
	if( !IsRunTimeMode() || !m_pPacketList)	return;
	if( !szRealInfo || !*szRealInfo)		return;
	if( !m_pIRealReceiver)					return;

	// 1. Real Setting Info에 추가한다.
	CString strShareName;
	m_pIRealReceiver->RemoveRealSettingInfoRQ(m_strRQ, szRealInfo, &strShareName, *szSeparatorChar);

	// 2. Local Shared Info에서 제거한다.
	if( !strShareName.IsEmpty() && m_plLocalShareInfoList) m_plLocalShareInfoList->RemoveLocalShareInfo( strShareName);
}

//////////////////////////////////////////////////////////////////////
// (2004.11.17, 배승원, 분석) Real On/Off Method & Routine
//////////////////////////////////////////////////////////////////////

// (2004.11.17, 배승원, 분석) Real을 일시 중지시키고, 다시 요청하는 Method Interface이다.
BOOL CKTBChartCtrl::SetDrdsAdvise(BOOL bOnAdvise) 
{
	if( !m_pIRealReceiver) return FALSE;

	// RQ별 DRDS관리 : 복수종목 - ojtaso (20070111)
	m_pIRealReceiver->SetRealAdviseRQ(m_strRQ, bOnAdvise);

	return TRUE;
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetRealSettingInfo
 - Created at  :  2005-05-24   13:56
 - Author      :  최종찬
 - Return Value:  BSTR - 리얼설정 데이타
 - Description :  차트에 세팅된 리얼설정 데이타를 가져온다.
 -----------------------------------------------------------------------------------------*/
BSTR CKTBChartCtrl::GetRealSettingInfo() 
{
	CString strResult = _T("");
	if( m_pIRealReceiver) m_pIRealReceiver->GetRealAdvisedInfo( strResult);
	return strResult.AllocSysString();
}

// RQ별 Real등록/해제 : 복수종목 - ojtaso (20070119)
void CKTBChartCtrl::SetDrdsAdviseRQ(LPCTSTR lpszRQ, BOOL bOnAdvise) 
{
	if(m_pIRealReceiver) 
	{
		if(!bOnAdvise)
			m_pIRealReceiver->RemoveRQPacket(lpszRQ);
		
		m_pIRealReceiver->SetRealAdviseRQ(lpszRQ, bOnAdvise);
	}
}

// (2009/12/11 - Seung-Won, Bae) Append lost NMTR data.
void CKTBChartCtrl::SetFirstNMTRClock( void)
{
	if( !m_pPacketListManager || !m_pIRealReceiver) return;

	_int64 nClock = 0;

	IOldFXRealReceiver	*pIOldFXRealReceiver = ( IOldFXRealReceiver *)m_pIRealReceiver;
	IOldHTSRealReceiver	*pIOldHTSRealReceiver = ( IOldHTSRealReceiver *)m_pIRealReceiver;
	// (2009/9/5 - Seung-Won, Bae) Support Old MChart Mode
	char szMChartMode[ 256];
	DWORD dwError = GetPrivateProfileString( "Chart Version", "MChartMode", "", szMChartMode, 255, GetDefaultPath() + "ChartMetaTable.dat");
	CString strChartMode( szMChartMode);
	if(			strChartMode == "FX")		nClock = pIOldFXRealReceiver->GetNMTRClock();
	else if(	strChartMode == "WORLDON")	nClock = m_pIRealReceiver->GetNMTRClock();
	else if(	strChartMode == "HTS")		nClock = pIOldHTSRealReceiver->GetNMTRClock();
	//<<20100310_JS.Kim	솔로몬.. HTS/해외선물/FX가 같은 ChartMetaTable.dat을 쓰므로 여기에 셋팅할 수 없어 셋팅 안되어 있는 경우는 일반적으로 처리
	else									nClock = m_pIRealReceiver->GetNMTRClock();
	//>>
	
	nClock += 5 - nClock % 5;
	m_pPacketListManager->OnNMTRClock( nClock, m_pIRealReceiver);
}

