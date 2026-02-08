// YScaleSetUpAddInImp.cpp: implementation of the CYScaleSetUpAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "YScaleSetUpAddInImp.h"

#include <MATH.H>

#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../../Include_Chart/Conversion.h"					// for CDataConversion
#include "../../Include_Chart/IPropertyMap.h"				// for IPropertyMap
#include "../Include_AddIn/AddInCommand.h"					// for EAI_CUSTOM_ENABLE_BOUNDMARK
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IDoubleList.h"			// for IDoubleList
#include "../Include_AddIn/I000000/_IString.h"				// for ILPCSTR
#include "../Include_AddIn/I000000/_IBlock.h"				// for IBlock


#include "../Include_AddIn/_resource.h"						// for ID_SCI_SAVE_SCREEN
#include "DlgYScaleSetUp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CYScaleSetUpAddInImp::CYScaleSetUpAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_eYScaleSetupApplyState = YSCALE_APPLY_STATE::APPLY_CANCEL;

	m_strStockCode = "000000";
	m_strPrevStockCode = "";
	
	m_strFormat = "%.f";

	// 설정값 / 비율 ( 0 : 설정값, 1 : 비율 )
	m_nYScaleMaxMinSetupType = 0;

	// 설정 - 최대 / 최소값
	m_dYScaleMaxValue = 0;
	m_dYScaleMinValue = 0;

	// 비율 - 최대 / 최소값
	m_dYScaleMaxPercentValue = 30;
	m_dYScaleMinPercentValue = -30;
}


CYScaleSetUpAddInImp::~CYScaleSetUpAddInImp()
{
}

// ( 2006 / 11 / 16 - Sang-Woo, Cho ) - Multiple Item in DLL
CString CYScaleSetUpAddInImp::m_strAddInItemName = "PRICE_YSCALE_SETUP";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CYScaleSetUpAddInImp)
	ONADDINEVENT( OnToolCommand)	
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_char)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
	ONADDINEVENT( OnPacketDataMultiItem)
END_ADDIN_EVENT_MAP()

// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CYScaleSetUpAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	if( !m_pIDefaultPacketManager || strcmp( lpszRQ, "DEFAULT")) return;

	// 1. 가격차트가 포함된 Block의 Index를 구한다.
	// 특정 Graph가 등록된 Block의 Index를 구하는 Interface를 OCX에서 제공한다.
	ILPCSTR szHelpMsg = NULL;
	// 20081016 이주표 >>
	BOOL bSuccess = m_pIDefaultPacketManager->GetHelpMessageItem( "BOUNDMARK", szHelpMsg );
	if(bSuccess)
	{
		// 2. 보조메세지로 AddIn에 가격 정보를 설정한다.
		ResetYScaleSetUp( szHelpMsg );
	}
	else
	{		
		bSuccess = m_pIDefaultPacketManager->GetHelpMessageItem( "PREVPRICE", szHelpMsg );
		if(bSuccess)
		{ 
			CString strYScale;
			CString strData, strPacket, strPacketName, strValue;
			strData.Format("%s", szHelpMsg);
			int nPos = 0, nIdx = 0;
			while(nPos != -1)
			{
				nPos = strData.Find("|");
				if( nPos == -1)
				{
					strPacket = strData;
					strData.Empty();
				}
				else
				{
					strPacket = strData.Mid(0, nPos);
					strData = strData.Mid(nPos + 1);
				}
				nPos = strPacket.Find(":");
				strPacketName = strPacket.Mid(0, nPos);
				strValue = strPacket.Mid(nPos + 1);
				strValue.TrimLeft();strValue.TrimRight();
				if(strPacketName.Compare(_MTOL0(_MTEXT(C0_CLOSE))) == 0)
				{
					m_strGraphName = strPacketName;
					m_dYesterdayClosePrice = atof(strValue);
					break;
				}
			}
		}
	}
	// 20081016 이주표 <<
	
	// 2. 보조메세지로 AddIn에 가격 정보를 설정한다.
//	ResetYScaleSetUp( szHelpMsg );

	// YScale을 설정한 적이 없거나 취소한 상태이면 아무런 작업도 하지 않는다.
	if( m_eYScaleSetupApplyState == YSCALE_APPLY_STATE::APPLY_CANCEL )
	{
		m_strPrevStockCode = m_strStockCode;
		return;
	}

	int nR, nC, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, m_nVertScaleGroupIndex, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return;

	// Use Block Interface
	IBlock *pIBlock = NULL;
	pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock) return;

	// 3. 이전 조회 종목이 없으면 현재 종목 정보를 입력한다.
	if( m_strPrevStockCode == "" )		m_strPrevStockCode = m_strStockCode;

	if( m_eYScaleSetupApplyState == YSCALE_APPLY_STATE::APPLY_PERCENT )
	{
		m_dYScaleMaxValue =	m_dYesterdayClosePrice * ( 1 + m_dYScaleMaxPercentValue / 100 );
		m_dYScaleMinValue =	m_dYesterdayClosePrice * ( 1 + m_dYScaleMinPercentValue / 100 );
	}
	
	// 4. TR을 수신 했을때 종목 Code가 바뀌면
	if( m_strStockCode != m_strPrevStockCode )
	{
		// 값지정 상태이면 Lock을 풀고 VERT_MINMAX_OF_VIEW 상태로 돌아간다.
		if( m_eYScaleSetupApplyState == YSCALE_APPLY_STATE::APPLY_VALUE )
		{
			if( pIBlock )
				pIBlock->SetVScaleMinMaxSetting( m_nVertScaleGroupIndex, FALSE,
												 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_VIEW,
												 m_dYScaleMinValue, m_dYScaleMaxValue);
			
			m_eYScaleSetupApplyState = YSCALE_APPLY_STATE::APPLY_CANCEL;
		}

		// 비율지정 상태이면 Lock을 걸고 VERT_MINMAX_OF_USER 상태로 유지한다.
		else if( m_eYScaleSetupApplyState == YSCALE_APPLY_STATE::APPLY_PERCENT )
		{
			if( pIBlock )
				pIBlock->SetVScaleMinMaxSetting( m_nVertScaleGroupIndex, TRUE,
												 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_USER,
												 m_dYScaleMinValue, m_dYScaleMaxValue);
		}
		 
		m_strPrevStockCode = m_strStockCode;
	}

	// 동일 종목일 경우
	else
	{
		// YScale을 설정한 적이 없거나 취소한 상태이면 아무런 작업도 하지 않는다.
		if( m_eYScaleSetupApplyState == YSCALE_APPLY_STATE::APPLY_CANCEL )
			return;

		// Lock을 걸고 VERT_MINMAX_OF_USER 상태로 유지한다.
		if( pIBlock )
			pIBlock->SetVScaleMinMaxSetting( m_nVertScaleGroupIndex, TRUE,
											 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_USER,
											 m_dYScaleMinValue, m_dYScaleMaxValue);
	}

	if( pIBlock )	pIBlock->Release();
	pIBlock = NULL;

	// OCX를 다시 그려준다.
	m_pIChartOCX->InvalidateControl();
}

// (2006/11/24 - Seung-Won, Bae) On Tool Command
void CYScaleSetUpAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( p_nToolType != CToolOptionInfo::T_PRICE_YSCALE_SETUP) return;
	InvokeAddInStr( "YSCALE_SETUP_LOAD", "");
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 16
// Return BOOL	: 
// Param  LPCTSTR p_szCommandName : 
// Param  LPCTSTR p_szData : 
// Comments		: Message를 받으면 설정 Dialog를 연다.
//-----------------------------------------------------------------------------
BOOL CYScaleSetUpAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	// 1. "LOAD_YSCALE_SETUP" Message가 들어오면 설정 dialog를 연다.
	if( stricmp( p_szCommandName, "YSCALE_SETUP_LOAD")) return FALSE;

	// 1.1 Block 정보를 얻어온다.
	int nR, nC, nG;
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, m_nVertScaleGroupIndex, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return FALSE;

	// 1.2 Use Block Interface
	IBlock *pIBlock = NULL;
	pIBlock = m_pIChartManager->GetBlock( nR, nC );
	if( !pIBlock) return FALSE;

	// 1.3 AddIn에 가격 정보를 설정할 수 없으면 Return 한다.
	if( !SetYScaleInfo( pIBlock ) )
	{
		if( pIBlock )	pIBlock->Release();
		pIBlock = NULL;
		return FALSE;
	}

	// 1.4 설정 dialog를 생성하고 정보를 Setting한다.
	UINT uDlgID = IDD_YSCALE_SETUP_HTS;
	if( CChartInfo::WORLD_ON == m_eChartMode) uDlgID = IDD_YSCALE_SETUP_WORLDON;
	CDlgYScaleSetUp dlgYScaleSetUp( this, m_pIChartOCX->GetOcxHwnd(), m_eChartMode, uDlgID);
	
	// 1.5 설정 / 비율의 선택값 전달
	dlgYScaleSetUp.SetYScaleMaxMinSetupType( m_nYScaleMaxMinSetupType );		

	// 1.6 호가 정밀도 전달
	dlgYScaleSetUp.SetValueFormat( m_strFormat );
	
	// 1.7 전일종가, 상한가, 하한가 정보를 전달
	dlgYScaleSetUp.SetStockPriceInfo( m_dYesterdayClosePrice, m_dUpperBound, m_dLowerBound );
	
	// 1.8 최대값 / 최소값을 전달
	dlgYScaleSetUp.SetYScaleMaxMin( m_dYScaleMaxValue, m_dYScaleMinValue);
	
	// 1.9 최대 비율값 / 최소 비율값을 전달
	dlgYScaleSetUp.SetYScaleMaxMinPercent( m_dYScaleMaxPercentValue, m_dYScaleMinPercentValue);

	// (2006/11/25 - Seung-Won, Bae) Notify ToolState.
	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_PRICE_YSCALE_SETUP, TRUE);

	// (2007/5/2 - Seung-Won, Bae) Check Market Type for Unit.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE));
	if( pIPacket)
	{
		dlgYScaleSetUp.m_eMarketType = pIPacket->GetCommodityBondName();
		pIPacket->Release();
	}
	// 20081016 이주표 >>	
	ILPCSTR szType = pIPacket->GetType();
	CString strPacketName(szType);
	dlgYScaleSetUp.SetPacketType(strPacketName);
	// 20081016 이주표 <<

	// 1.10 설정 dialog를 Load한다.
	ML_SET_LANGUAGE_RES();
	if( dlgYScaleSetUp.DoModal() == IDOK )
	{
		// 사용자가 비율을 설정하고 적용을 했을 경우 비율값으로 최대 최소값을 얻어온다.
		if( m_nYScaleMaxMinSetupType == 1 )
		{
			m_eYScaleSetupApplyState = YSCALE_APPLY_STATE::APPLY_PERCENT;

			m_dYScaleMaxValue =	m_dYesterdayClosePrice * ( 1 + m_dYScaleMaxPercentValue / 100 );
			m_dYScaleMinValue =	m_dYesterdayClosePrice * ( 1 + m_dYScaleMinPercentValue / 100 );
		}
		
		else
			m_eYScaleSetupApplyState = YSCALE_APPLY_STATE::APPLY_VALUE;


		// Block에 설정값을 전달한다.
		pIBlock->SetVScaleMinMaxSetting( m_nVertScaleGroupIndex, TRUE,
										 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_USER,
										 m_dYScaleMinValue, m_dYScaleMaxValue);
	}

	// 시용자가 취소를 선택하면 
	else
	{
		m_eYScaleSetupApplyState = YSCALE_APPLY_STATE::APPLY_CANCEL;

		// Block에 설정값을 전달한다.
		pIBlock->SetVScaleMinMaxSetting( m_nVertScaleGroupIndex, FALSE,
										 CScaleBaseData::VERTMINMAXTYPE::VERT_MINMAX_OF_VIEW,
										 m_dYScaleMinValue, m_dYScaleMaxValue);
	}

	// (2006/11/25 - Seung-Won, Bae) Notify ToolState.
	m_pIChartOCX->OnToolStateChanged( CToolOptionInfo::T_PRICE_YSCALE_SETUP, FALSE);

	pIBlock->Release();
	pIBlock = NULL;
				
	m_pIChartOCX->InvalidateControl();
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 14
// Return void	: 
// Param  IPropertyMap *p_pIAddInEnvMap : 
// Comments		: File에서 m_strAddInItemName에 해당되는 내용을 읽어온다.
//				: ex) "1:000660:%%.%2f:50000:10000:15:-15" 
//				: 1				=> CANCEL : 해지, APPLY_VALUE : 값적용, APPLY_PERCENT : 비율적용
//				: 000660		=> 종목코드
//				: %%.%2f		=> 호가 format
//				: 50000:10000	=> 최대 / 최저값
//				: 15:-15		=> 비율 최대 / 최소값.
//-----------------------------------------------------------------------------
void CYScaleSetUpAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap )
{
	// 1. p_pIAddInEnvMap에서 "YSCALE_SETUP"으로 저장된 값을 읽어온다.
	const char *szEnvValue = NULL;
	if( !p_pIAddInEnvMap->Lookup( m_strAddInItemName, szEnvValue) )
		return;


	// 2. 읽어온 값의 길이가 0이면 return한다.
	CString strEnvValue = szEnvValue;
	if( strEnvValue.GetLength() <= 0 )
		return;


	// 3. 읽어온 값에서 구분 설정값을 분리한다.
	CString strYScaleMaxMinSetupType = strEnvValue.Mid( 0, 1 );
	if( strYScaleMaxMinSetupType == "" )	return;

	m_eYScaleSetupApplyState = (YSCALE_APPLY_STATE)atoi(strYScaleMaxMinSetupType);
	
	if( m_eYScaleSetupApplyState == YSCALE_APPLY_STATE::APPLY_PERCENT )
		m_nYScaleMaxMinSetupType = 1;

	else
		m_nYScaleMaxMinSetupType = 0;

	strEnvValue = strEnvValue.Mid(2);


	// 4. 종목코드를 얻어온다.
	int nIdx = strEnvValue.Find(":");
	if( nIdx < 0 )	return;
	m_strStockCode = strEnvValue.Mid(0, nIdx);
	
	strEnvValue = strEnvValue.Mid(nIdx+1);

	// 5. 가격 foramt
	nIdx = strEnvValue.Find(":", 1);
	if( nIdx < 0 )	return;
	m_strFormat = strEnvValue.Mid(0, nIdx);


	strEnvValue = strEnvValue.Mid(nIdx+1);

	// 6. 사용자가 설정한 최대값을 가져온다.
	nIdx = strEnvValue.Find(":", 1);
	if( nIdx < 0 )	return;
	CString strYScaleMaxValue = strEnvValue.Mid(0, nIdx);
	m_dYScaleMaxValue = atof(strYScaleMaxValue);

	strEnvValue = strEnvValue.Mid(nIdx+1);
	

	// 7. 사용자가 설정한 최소값을 가져온다.
	nIdx = strEnvValue.Find(":", 1);
	if( nIdx < 0 )	return;
	CString strYScaleMinValue = strEnvValue.Mid(0, nIdx);
	m_dYScaleMinValue = atof(strYScaleMinValue);

	strEnvValue = strEnvValue.Mid(nIdx+1);


	// 8. 비율 설정에서 사용자가 설정한 최대값을 가져온다.
	nIdx = strEnvValue.Find(":", 1);
	if( nIdx < 0 )	return;
	CString strYScaleMaxPercentValue = strEnvValue.Mid(0, nIdx);
	m_dYScaleMaxPercentValue = atof(strYScaleMaxPercentValue);

	strEnvValue = strEnvValue.Mid(nIdx+1);


	// 9. 비율 설정에서 사용자가 설정한 최소값을 가져온다.
	if( strEnvValue.GetLength() <= 0 )	return;
	CString strYScaleMinPercentValue = strEnvValue;
	m_dYScaleMinPercentValue = atof(strYScaleMinPercentValue);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 14
// Return void	: 
// Param  IPropertyMap *p_pIAddInEnvMap : 
// Comments		: File에서 m_strAddInItemName에 해당되는 내용을 저장한다.
//				: ex) "1:15:-15" 
//				: 1 -> 설정값 사용 : 0, 비율 사용 : 1
//				: 15: -15 -> 비율 최대 / 최소값.
//-----------------------------------------------------------------------------
void CYScaleSetUpAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap )
{
	// 1. p_pIAddInEnvMap에서 "YSCALE_SETUP"으로 저장된 값을 읽어온다.
	const char *szEnvValue = NULL;
	if( p_pIAddInEnvMap->Lookup( m_strAddInItemName, szEnvValue) ) return;

	
	// 2. 설정/비율 선택 정보, 최대 / 최소 비율 정보만 저장한다.
	CString strEnvValue;
	strEnvValue.Format( "%d:%s:%s:%f:%f:%f:%f",
						m_eYScaleSetupApplyState,		\
						m_strStockCode,					\
						m_strFormat,					\
						m_dYScaleMaxValue,				\
						m_dYScaleMinValue,				\
						m_dYScaleMaxPercentValue,		\
						m_dYScaleMinPercentValue );
	
	p_pIAddInEnvMap->SetAt( m_strAddInItemName, strEnvValue);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 15
// Return void	: 
// Param  const char *p_szHelpMsg : 
// Comments		: 
//-----------------------------------------------------------------------------
void CYScaleSetUpAddInImp::ResetYScaleSetUp( const char *p_szHelpMsg)
{
	// 1. 기존 Bound 값을 초기화한다. (설정에 실패하면, Clear되도록)
	m_strGraphName.Empty();

	// 2. Message를 확인한다.
	if( !p_szHelpMsg) return;
	if( !*p_szHelpMsg) return;

	// 3. Message를 Buffer로 옮긴다.
	CString strYScale;
	char *szYScale = strYScale.GetBufferSetLength( strlen( p_szHelpMsg ) );
	strcpy( szYScale, p_szHelpMsg);

	// 4. 먼저 Graph Name을 구한다.
	char *	szGraphName		= szYScale;
	char *	szLowerBound	= NULL;
	char *	szUpperBound	= NULL;
	szYScale = strchr( szYScale, ':');
	if( !szYScale) return;
	*szYScale = '\0';

	// 5. 상한가 Message를 구한다.
	szYScale++;
	szUpperBound = szYScale;
	szYScale = strchr( szYScale, ',');
	if( !szYScale) return;
	*szYScale = '\0';

	// 6. 하한가 Message를 구한다.
	szYScale++;
	szLowerBound = szYScale;
	
	// (2004.10.25, 배승원) 확장된 정보가 있어도 오류가 나지 않도록 수정한다. (확장 정보를 별도로 관리한다.)
	szYScale = strchr( szYScale, ':');
	if( szYScale)
	{
		*szYScale = '\0';
		szYScale++;
		m_strExtMsg = szYScale;
		// (2004.10.28, 배승원) 뒷 Space를 미리 Clear해 둔다.
		m_strExtMsg.TrimRight();
	}

	if( !*szGraphName || !*szUpperBound || !*szLowerBound) return;
	// 7. 구해진 Graph Name, 상/하한가를 구하여 설설정한다.
	m_strGraphName	= szGraphName;
	m_dUpperBound	= atof( szUpperBound);
	m_dLowerBound	= atof( szLowerBound);

	// 8. 구해진 상/하한가가 뒤집힌 경우를 바로잡는다.
	if( m_dUpperBound < m_dLowerBound)
	{
		double dTemp	= m_dUpperBound;
		m_dUpperBound	= m_dLowerBound;
		m_dLowerBound	= dTemp;
	}

	// 9. 상/하한바의 기준점이 되는 전일종가 설정한다.
	SetYesterdayClosePrice(m_strExtMsg);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 15
// Return void	: 
// Param  const CString& strData : 
// Comments		: 
//-----------------------------------------------------------------------------
void CYScaleSetUpAddInImp::SetYesterdayClosePrice(const CString& strData)
{
	CString strTemp = strData;
	m_strStockCode = CDataConversion::GetStringData(strTemp, ":");	//종목코드
	CDataConversion::GetStringData(strTemp, ":");//??
	CString strPrice = CDataConversion::GetStringData(strTemp, ":");	//전일종가
	m_dYesterdayClosePrice = atof(strPrice);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 16
// Return BOOL	: 
// Comments		: 가격정보를 얻어온다.
//-----------------------------------------------------------------------------
BOOL CYScaleSetUpAddInImp::SetYScaleInfo( IBlock *pIBlock )
{
	if( m_strGraphName.IsEmpty() )
		return FALSE;

	if( !pIBlock) return FALSE;


	// 3. Block에서 VScale의 최대, 최소값을 얻어온다.
	pIBlock->GetVScaleGroupMinMax( m_nVertScaleGroupIndex, m_dYScaleMinValue, m_dYScaleMaxValue );
	
	
	// 4. 호가 정밀도를 얻기 위해 Packet정보를 얻어온다.
	IPacket *pIPacket = NULL;
	double dDataType = 0.0L;
	BOOL bResult = pIBlock->GetVScaleGroupHeadPacketAndDataType( m_nVertScaleGroupIndex, pIPacket, dDataType );
	if( !bResult)
	{
		if( pIPacket) pIPacket->Release();
		return FALSE;
	}


	// 5. Packet 정보가 있으면 호가 정밀도를 얻고 가격 정보의 자리수를 조정한다.
	if( pIPacket )
	{
		// 호가의 정밀도(Precision)을 관리하여 호가 String의 생성에 참고하도록 한다.
		int nDecimal = 0;

		if( dDataType != 0)
		{
			m_dYesterdayClosePrice = int( m_dYesterdayClosePrice / dDataType + 0.5) * dDataType;
			m_dUpperBound = int( m_dUpperBound / dDataType + 0.5) * dDataType;
			m_dLowerBound = int( m_dLowerBound / dDataType + 0.5) * dDataType;
			
			// double을 Int로 변환하는 과정에서 정확한 Int로 변환되지 않는 문제를 해결하기 위해
			// 100배 유효자리수를 얻도록 조정한다.
			nDecimal = ( int)( ( log10( dDataType) * 100) / 100);	
		}
	
		if( 0 <= nDecimal)
			m_strFormat = "%.0f";
		else
			m_strFormat.Format( "%%.%df", -nDecimal);
		
#ifdef _DEBUG
	TRACE( "%f	%f	%f\r\n", m_dYesterdayClosePrice, m_dUpperBound, m_dLowerBound);
#endif
		pIPacket->Release();
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 16
// Return void	: 
// Param  int nYScaleSetUpType : 설정 / 비율의 선택 정보
// Param  double dMax : 최대값
// Param  double dMin : 최소값
// Param  double dMaxPercent : 최대 비율값 
// Param  double dMinPercent : 최소 비율값
// Comments		: DlgYScale에서 설정된 값을 저장하기 위해 만든 함수.
//-----------------------------------------------------------------------------
void CYScaleSetUpAddInImp::SetYScaleAllValue( int nYScaleSetUpType, double dMax, double dMin, double dMaxPercent, double dMinPercent )
{
	// 1. 설정 / 비율의 선택 정보
	m_nYScaleMaxMinSetupType = nYScaleSetUpType;
	
	// 2. 최대값 / 최소값
	m_dYScaleMaxValue = dMax;
	m_dYScaleMinValue = dMin;

	// 3. 최대 / 최소 Percent 값
	m_dYScaleMaxPercentValue = dMaxPercent;
	m_dYScaleMinPercentValue = dMinPercent;
}