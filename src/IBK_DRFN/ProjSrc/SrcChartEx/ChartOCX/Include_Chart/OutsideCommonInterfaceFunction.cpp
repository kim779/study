// OutsideCommonInterfaceFunction.cpp: implementation of the COutsideCommonInterfaceFunction class.
// 종찬(05/07/28) 이부분은 단순 바이패스하는 구간이오니 주석은 생략함을 공지하는 바입니다.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OutsideCommonInterfaceFunction.h"
#include "OutsideCommonInterface.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// (2006/11/15 - Seung-Won, Bae) for Interface in COutsideCommonInterface
//////////////////////////////////////////////////////////////////////

COutsideCommonInterfaceFunction::COutsideCommonInterfaceFunction()
{
	m_pOutsideCommonInterface = NULL;

	// (2006/11/15 - Seung-Won, Bae) for Interface in CMChartInterface
	m_bMChartInterfaceInited	= FALSE;
	m_pMChartInterface			= NULL;
	m_pOldMChartInterface	= NULL;		// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface

	// (2006/11/15 - Seung-Won, Bae) for New Interface in 06/11/15
	m_bMChartInterfaceInited061115	= FALSE;
	m_pMChartInterface061115		= NULL;
	m_pOldMChartInterface061115		= NULL;	// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface

	// (2009/9/3 - Seung-Won, Bae) for WorldOn MChartInterface
	m_bIsOldInterface = FALSE;
}

COutsideCommonInterfaceFunction::~COutsideCommonInterfaceFunction()
{
}

// public =====================================================================
void COutsideCommonInterfaceFunction::SetOutsideCommonInterface(COutsideCommonInterface* pOutsideCommonInterface, CString p_strDataPath)
{
	m_pOutsideCommonInterface = pOutsideCommonInterface;
	if( !p_strDataPath.IsEmpty())
	{
		char szMChartMode[ 256];
		DWORD dwError = GetPrivateProfileString( "Chart Version", "MChartMode", "", szMChartMode, 255, p_strDataPath + "ChartMetaTable.dat");
		CString strChartMode( szMChartMode);
		m_bIsOldInterface =  ( ( strChartMode == "FX") || ( strChartMode == "HTS"));
	}
}
COutsideCommonInterface *COutsideCommonInterfaceFunction::GetOutsideCommonInterface( void)
{
	return m_pOutsideCommonInterface;
}

// CommonInterface 용 =========================================================
//--------- 리얼처리
void COutsideCommonInterfaceFunction::Outside_SetRealInfo(void* pDrds, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName)
{
	if(m_pOutsideCommonInterface == NULL) return;

	m_pOutsideCommonInterface->SetRealInfo(pDrds, hWnd, id, DataName, FieldName, KeyName);
}

void COutsideCommonInterfaceFunction::Outside_UnSetRealInfo(void* pDrds, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName)
{
	if(m_pOutsideCommonInterface == NULL) return;

	m_pOutsideCommonInterface->UnSetRealInfo(pDrds, hWnd, id, DataName, FieldName, KeyName);
}

//----------- HTS Path와 User ID.
LPCTSTR COutsideCommonInterfaceFunction::Outside_GetMainPath()
{
	if(m_pOutsideCommonInterface == NULL) return _T("");
	return m_pOutsideCommonInterface->GetMainPath();
}

LPCTSTR COutsideCommonInterfaceFunction::Outside_GetUserID()
{
	if(m_pOutsideCommonInterface == NULL) return _T("");
	return m_pOutsideCommonInterface->GetUserID();
}

// TR 요청
void COutsideCommonInterfaceFunction::Outside_RequestTRPacket(const short nID, const LPCTSTR pcszPacketNames, const short nCount, const bool bIsNew)
{
#ifdef _DEBUG
	CString strMsg;
	strMsg.Format( "MarketIndicator TR Requested (for MChart) : %s\r\n", pcszPacketNames);
	TRACE( strMsg);
#endif
	if(m_pOutsideCommonInterface == NULL) return;
	m_pOutsideCommonInterface->RequestTRPacket(nID, pcszPacketNames, nCount, bIsNew);
}

// 시장지표 Load 할지에 대한 유무
bool COutsideCommonInterfaceFunction::Outside_IsLoad_MarketChart() const
{
	if(m_pOutsideCommonInterface == NULL) return false;
	return m_pOutsideCommonInterface->IsLoad_MarketChart();
}

//////////////////////////////////////////////////////////////////////
// (2006/11/15 - Seung-Won, Bae) for Interface in CMChartInterface
//////////////////////////////////////////////////////////////////////

BOOL COutsideCommonInterfaceFunction::InitMChartInterface()
{
	if( m_bMChartInterfaceInited) return ( m_pMChartInterface != NULL);
	m_bMChartInterfaceInited = TRUE;

	if( !m_pOutsideCommonInterface) return FALSE;

	// (2006/11/18 - Seung-Won, Bae) Caution!
	//		The Next Version Interface must support the Previous Version Interface Correctly.
	//		Because, Old Version Supported Interface Checking is processed with Version Nember by '<=' (Greater than or Equal to)
	if( m_pOutsideCommonInterface->IsOutsideInterfaceType() < COutsideCommonInterface::MC_TYPE_ORG) return FALSE;
	m_pMChartInterface = ( CMChartInterface *)m_pOutsideCommonInterface;
	m_pOldMChartInterface = ( COldMChartInterface *)m_pOutsideCommonInterface;
	return TRUE;
}

bool COutsideCommonInterfaceFunction::MChart_IsActiveDataWindow()
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->IsActiveDataWindow();
		else				return m_pMChartInterface->IsActiveDataWindow();
	}
	return false;
}
void COutsideCommonInterfaceFunction::MChart_SetDataWindowData(CList<CString, CString&>* pListData)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	m_pOldMChartInterface->SetDataWindowData(pListData);
		else				m_pMChartInterface->SetDataWindowData(pListData);
	}
}
bool COutsideCommonInterfaceFunction::MChart_SetImaginaryCandleAndGapRevisionToOutside( const bool bImaginaryCandle, const bool bUseGapRevision)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetImaginaryCandleToOutside( bImaginaryCandle);
		else				return m_pMChartInterface->SetImaginaryCandleAndGapRevisionToOutside( bImaginaryCandle, bUseGapRevision);
	}

	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetYScalePositionToOutside(const int nPosition)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetYScalePositionToOutside(nPosition);
		else				return m_pMChartInterface->SetYScalePositionToOutside(nPosition);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetRightMarginToOutside(const int nRightMargin)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetRightMarginToOutside(nRightMargin);
		else				return m_pMChartInterface->SetRightMarginToOutside(nRightMargin);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetShowIndicatorVariableToOutside(const bool bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetShowIndicatorVariableToOutside( bShow);
		else				return m_pMChartInterface->SetShowIndicatorVariableToOutside( bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetShowIndicatorValueToOutside(const bool bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetShowIndicatorValueToOutside(bShow);
		else				return m_pMChartInterface->SetShowIndicatorValueToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetShowHighLowBoundToOutside(const bool bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetShowHighLowBoundToOutside(bShow);
		else				return m_pMChartInterface->SetShowHighLowBoundToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetShowExRightDividendToOutside(const bool bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetShowExRightDividendToOutside(bShow);
		else				return m_pMChartInterface->SetShowExRightDividendToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetShowCurrentPriceToOutside(const int nShowType)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetShowCurrentPriceToOutside(nShowType);
		else				return m_pMChartInterface->SetShowCurrentPriceToOutside(nShowType);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetTrendLineColorToOutside(const COLORREF clrLineColor)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetTrendLineColorToOutside(clrLineColor);
		else				return m_pMChartInterface->SetTrendLineColorToOutside(clrLineColor);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetShowTrendLineValueToOutside(const int nPosition)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetShowTrendLineValueToOutside(nPosition);
		else				return m_pMChartInterface->SetShowTrendLineValueToOutside(nPosition);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetTimeLinkageToOutside(const bool bTimeLinkage)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetTimeLinkageToOutside(bTimeLinkage);
		else				return m_pMChartInterface->SetTimeLinkageToOutside(bTimeLinkage);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetAutoSaveToOutside(const bool bAutoSave)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetAutoSaveToOutside(bAutoSave);
		else				return m_pMChartInterface->SetAutoSaveToOutside(bAutoSave);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetShowTrendLineTextToOutside(const bool bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetShowTrendLineTextToOutside(bShow);
		else				return m_pMChartInterface->SetShowTrendLineTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetShowTimeLineTextToOutside(const bool bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetShowTimeLineTextToOutside(bShow);
		else				return m_pMChartInterface->SetShowTimeLineTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetShowPriceLineLeftTextToOutside(const bool bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetShowPriceLineLeftTextToOutside(bShow);
		else				return m_pMChartInterface->SetShowPriceLineLeftTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetShowPriceLineRightTextToOutside(const bool bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetShowPriceLineRightTextToOutside(bShow);
		else				return m_pMChartInterface->SetShowPriceLineRightTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetShowTrisectionLineTextToOutside(const bool bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetShowTrisectionLineTextToOutside(bShow);
		else				return m_pMChartInterface->SetShowTrisectionLineTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetShowQuadrisectionLineTextToOutside(const bool bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetShowQuadrisectionLineTextToOutside(bShow);
		else				return m_pMChartInterface->SetShowQuadrisectionLineTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetShowControlLineTextToOutside(const bool bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetShowControlLineTextToOutside(bShow);
		else				return m_pMChartInterface->SetShowControlLineTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetImaginaryCandleToOutside(bool& bImaginaryCandle)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetImaginaryCandleToOutside(bImaginaryCandle);
		else				return m_pMChartInterface->GetImaginaryCandleToOutside(bImaginaryCandle);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetYScalePositionToOutside(int& nPosition)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetYScalePositionToOutside(nPosition);
		else				return m_pMChartInterface->GetYScalePositionToOutside(nPosition);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetRightMarginToOutside(int& nRightMargin)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetRightMarginToOutside(nRightMargin);
		else				return m_pMChartInterface->GetRightMarginToOutside(nRightMargin);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetShowIndicatorVariableToOutside(bool& bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetShowIndicatorVariableToOutside(bShow);
		else				return m_pMChartInterface->GetShowIndicatorVariableToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetShowIndicatorValueToOutside(bool& bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetShowIndicatorValueToOutside(bShow);
		else				return m_pMChartInterface->GetShowIndicatorValueToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetShowHighLowBoundToOutside(bool& bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetShowHighLowBoundToOutside(bShow);
		else				return m_pMChartInterface->GetShowHighLowBoundToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetShowExRightDividendToOutside(bool& bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetShowExRightDividendToOutside(bShow);
		else				return m_pMChartInterface->GetShowExRightDividendToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetShowCurrentPriceToOutside(int& nShowType)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetShowCurrentPriceToOutside(nShowType);
		else				return m_pMChartInterface->GetShowCurrentPriceToOutside(nShowType);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetTrendLineColorToOutside(COLORREF& clrLineColor)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetTrendLineColorToOutside(clrLineColor);
		else				return m_pMChartInterface->GetTrendLineColorToOutside(clrLineColor);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetShowTrendLineValueToOutside(int& nPosition)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetShowTrendLineValueToOutside(nPosition);
		else				return m_pMChartInterface->GetShowTrendLineValueToOutside(nPosition);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetTimeLinkageToOutside(bool& bTimeLinkage)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetTimeLinkageToOutside(bTimeLinkage);
		else				return m_pMChartInterface->GetTimeLinkageToOutside(bTimeLinkage);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetAutoSaveToOutside(bool& bAutoSave)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetAutoSaveToOutside(bAutoSave);
		else				return m_pMChartInterface->GetAutoSaveToOutside(bAutoSave);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetShowTrendLineTextToOutside(bool& bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetShowTrendLineTextToOutside(bShow);
		else				return m_pMChartInterface->GetShowTrendLineTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetShowTimeLineTextToOutside(bool& bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetShowTimeLineTextToOutside(bShow);
		else				return m_pMChartInterface->GetShowTimeLineTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetShowPriceLineLeftTextToOutside(bool& bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetShowPriceLineLeftTextToOutside(bShow);
		else				return m_pMChartInterface->GetShowPriceLineLeftTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetShowPriceLineRightTextToOutside(bool& bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetShowPriceLineRightTextToOutside(bShow);
		else				return m_pMChartInterface->GetShowPriceLineRightTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetShowTrisectionLineTextToOutside(bool& bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetShowTrisectionLineTextToOutside(bShow);
		else				return m_pMChartInterface->GetShowTrisectionLineTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetShowQuadrisectionLineTextToOutside(bool& bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetShowQuadrisectionLineTextToOutside(bShow);
		else				return m_pMChartInterface->GetShowQuadrisectionLineTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_GetShowControlLineTextToOutside(bool& bShow)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->GetShowControlLineTextToOutside(bShow);
		else				return m_pMChartInterface->GetShowControlLineTextToOutside(bShow);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_SetPriceLog(const bool bLog)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->SetPriceLog(bLog);
		else				return m_pMChartInterface->SetPriceLog(bLog);
	}
	return false;
}
bool COutsideCommonInterfaceFunction::MChart_OnChartEnvSetupOk( void)
{
	if( InitMChartInterface())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface->OnChartEnvSetupOk();
		else				return m_pMChartInterface->OnChartEnvSetupOk();
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// (2006/11/15 - Seung-Won, Bae) for New Interface in 06/11/15
//////////////////////////////////////////////////////////////////////
BOOL COutsideCommonInterfaceFunction::InitMChartInterface061115()
{
	if( m_bMChartInterfaceInited061115) return ( m_pMChartInterface061115 != NULL);
	m_bMChartInterfaceInited061115 = TRUE;

	if( !m_pOutsideCommonInterface) return FALSE;
	// (2006/11/18 - Seung-Won, Bae) Caution!
	//		The Next Version Interface must support the Previous Version Interface Correctly.
	//		Because, Old Version Supported Interface Checking is processed with Version Nember by '<=' (Greater than or Equal to)
	if( m_pOutsideCommonInterface->IsOutsideInterfaceType() < COutsideCommonInterface::MC_TYPE_061115) return FALSE;
	m_pMChartInterface061115 = ( CMChartInterface061115 *)m_pOutsideCommonInterface;
	m_pOldMChartInterface061115 = ( COldMChartInterface061115 *)m_pOutsideCommonInterface;
	return TRUE;
}
// (2006/11/15 - Seung-Won, Bae) Revised Price
BOOL COutsideCommonInterfaceFunction::GetRevisedPriceFlag( void)
{
	if( InitMChartInterface061115())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface061115->GetRevisedPriceFlag();
		else				return m_pMChartInterface061115->GetRevisedPriceFlag();
	}
	return FALSE;
}
BOOL COutsideCommonInterfaceFunction::SetRevisedPriceFlag( BOOL p_bRevisedFlag)
{
	if( InitMChartInterface061115())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface061115->SetRevisedPriceFlag( p_bRevisedFlag);
		else				return m_pMChartInterface061115->SetRevisedPriceFlag( p_bRevisedFlag);
	}
	return FALSE;
}
// (2006/11/15 - Seung-Won, Bae) Notify the Event of Indicator Added or Removed.
BOOL COutsideCommonInterfaceFunction::OnIndicatorAddOrRemoved( const char *p_szGraphName, const char *p_szIndicatorName, const BOOL p_bAdded)
{
#ifdef _DEBUG
	CString strMsg;
	strMsg.Format( "Graph %s (for MChart) : %s[%s]\r\n", ( p_bAdded ? "Added" : "Deleted"), p_szIndicatorName, p_szGraphName);
	TRACE( strMsg);
#endif
	if( InitMChartInterface061115())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface061115->OnIndicatorAddOrRemoved( p_szIndicatorName, p_bAdded);
		else				return m_pMChartInterface061115->OnIndicatorAddOrRemoved( p_szIndicatorName, p_bAdded);
	}
	return FALSE;
}
// (2006/11/15 - Seung-Won, Bae) User Zoom Changed.
BOOL COutsideCommonInterfaceFunction::OnUserZoomChanged( const int p_nDataCountInView)
{
#ifdef _DEBUG
	CString strMsg;
	strMsg.Format( "User Zoom Changed (for MChart) : %d\r\n", p_nDataCountInView);
	TRACE( strMsg);
#endif
	if( InitMChartInterface061115())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface061115->OnUserZoomChanged( p_nDataCountInView);
		else				return m_pMChartInterface061115->OnUserZoomChanged( p_nDataCountInView);
	}
	return FALSE;
}
// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
BOOL COutsideCommonInterfaceFunction::OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn)
{
	if( InitMChartInterface061115())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface061115->OnToolStateChanged( p_eToolID, p_bTurnOn);
		else				return m_pMChartInterface061115->OnToolStateChanged( p_eToolID, p_bTurnOn);
	}
	return FALSE;
}

// FX차트 주문 연동 - onlyojt (20091204)
BOOL COutsideCommonInterfaceFunction::OnOrderInfomationChanged(const double dPirce, const int Qty, const int nOrderType, LPCTSTR szOrgOrderID, LPCTSTR strCustItem1, LPCTSTR strCustItem2, LPCTSTR strCustItem3)
{
	if( InitMChartInterface061115())
	{
		if( m_bIsOldInterface)	return m_pOldMChartInterface061115->OnOrderInfomationChanged(dPirce, Qty, nOrderType, szOrgOrderID, strCustItem1, strCustItem2, strCustItem3);
		else				return m_pMChartInterface061115->OnOrderInfomationChanged(dPirce, Qty, nOrderType, szOrgOrderID, strCustItem1, strCustItem2, strCustItem3);
	}
	return FALSE;
}
