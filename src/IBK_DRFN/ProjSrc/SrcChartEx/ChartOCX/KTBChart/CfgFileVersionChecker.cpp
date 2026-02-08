// CfgFileVersionChecker.cpp: implementation of the CCfgFileVersionChecker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CfgFileVersionChecker.h"

#include <assert.h>

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "../Include_Chart/IChartCtrl.h"			// for IChartCtrl
#include "MessageDefine.h"
#include "Color.h"
#include "MainBlock.h"
#include "IndicatorList.h"
#include "IndicatorInfo.h"
#include "KTBChartCtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CCfgFileVersionChecker

//user File version checker
bool CCfgFileVersionChecker::UserFileVersionChecker(CKTBChartCtrl* pChartCtrl, CMainBlock* pMainBlock, bool& bIsCalculate)
{
	double dDefaultVer(0.0), dUserVer(0.0);
	if(!GetUserVersion(pMainBlock, dDefaultVer, dUserVer))
		return false;

	if(dDefaultVer == dUserVer)
		return false;

	if(dUserVer > 3.1 && dUserVer <= 3.21)
	{
		// (2006/5/31 - Seung-Won, Bae) Manage static checker for Meta Init
		static CCfgFileVersion3Dot21 cfgChecker3d21;
		if( cfgChecker3d21.UserFileVersionChecker(pChartCtrl, pMainBlock, bIsCalculate))
			return true;
	}
	if(dUserVer > 4.9 && dUserVer <= 5.1)
	{
		static CCfgFileVersion5Dot00 cfgChecker5d00;
		if( cfgChecker5d00.UserFileVersionChecker(pChartCtrl, pMainBlock, bIsCalculate))
			return true;
	}

	return true;
}


// private ====================================================================
// user file의 version 가져오기
bool CCfgFileVersionChecker::GetUserVersion(CMainBlock* pMainBlock, double& dDefaultVer, double& dUserVer) const
{
	if(pMainBlock == NULL)
		return false;

	CIndicatorList* pIndicatorList = pMainBlock->GetIndicatorList();
	if(pIndicatorList == NULL)
		return false;

	dDefaultVer	= pIndicatorList->GetCfgFileVersion( EILT_SITE_DEFAULT);
	dUserVer	= pIndicatorList->GetCfgFileVersion( EILT_USER);
	return true;
}

//////////////////////////////////////////////////////////////////////
// class CCfgFileVersionBase

CCfgFileVersionBase::CCfgFileVersionBase( void)
{
	m_bInited = FALSE;
}

// 현재 저장된 user ver3.21 인 경우
BOOL CCfgFileVersionBase::UserFileVersionChecker(CKTBChartCtrl* pChartCtrl, CMainBlock* pMainBlock, bool &p_bSubGraphChanged)
{
	if( !m_bInited)
	{
		RegisterPrevOperation();
		DenyPrevOperation();
		m_bInited = TRUE;
	}

	// 화면에 있는 지표명을 추출한다.
	CIndicatorList* pIndicatorList = pMainBlock->GetIndicatorList();
	CList<CString, CString> strIndicatorNameList;
	if(!pMainBlock->GetAllIndicatorName(strIndicatorNameList)) return FALSE;

	CString			strSubGraphNamesIndicator, strSubGraphNamesGraph;
	BOOL			bUpdateToGraph	= FALSE;
	POSITION		pos				= strIndicatorNameList.GetHeadPosition();
	IChartCtrl *	pIChartCtrl		= pChartCtrl->GetIChartCtrl();
	while(pos != NULL)
	{
		CString strGraphName = strIndicatorNameList.GetNext(pos);

		// 0. Check Not Supported Graph
		CString strIndicatorName;
		CIndicatorInfo *pSourceInfo = pIndicatorList->GetGraphIndicatorInfo( EILT_USER, strGraphName, NULL, EIIGT_WITHOUT_DEFAULT);
		if( pSourceInfo) strIndicatorName = pSourceInfo->GetIndicatorName();
		if( !strIndicatorName.IsEmpty())
		{
			CIndicatorInfo *pDefaultInfo = pIndicatorList->GetDefaultIndicatorInfo( EILT_SITE_DEFAULT, strIndicatorName, EIIGT_WITHOUT_DEFAULT);
			if( !pDefaultInfo)
			{
				pMainBlock->DeleteGraph( strGraphName);
				CString strNewIndicatorName;
				if( GetNewGraphForDeletedOld( strIndicatorName, strNewIndicatorName)) if( pIChartCtrl)
				{
					CString strGraphName;
					pIChartCtrl->AddChartBlock( strNewIndicatorName, strGraphName);
				}
				continue;
			}
		}

		bUpdateToGraph = VersionUpOldInfo( strGraphName, pSourceInfo, pChartCtrl, pMainBlock, p_bSubGraphChanged);

		// 2. Update to Graph ( Sub Graph Count & Name & Order and User Packet)
		if( !bUpdateToGraph)
		{
			pSourceInfo->GetSubGraphNames( strSubGraphNamesIndicator);
			strSubGraphNamesGraph = pMainBlock->GetSubGraphNames( strGraphName, false);
		}
		if( strSubGraphNamesIndicator != strSubGraphNamesGraph || bUpdateToGraph)
		{
			pMainBlock->ChangeGraphDataFromIndicatorInfo( strGraphName, false);
			p_bSubGraphChanged = true;
		}
	}

	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// class CCfgFileVersion3Dot21 : user file version (3.21)

CCfgFileVersion3Dot21::CCfgFileVersion3Dot21( void)
{
}

// (2006/5/30 - Seung-Won, Bae) Override for Registering and Denying the previous Operation
//		Caution! You should call the base member function in the end of routine.
void CCfgFileVersion3Dot21::RegisterPrevOperation( void)
{
	// 0. Target Graph
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_PRICE_MA),							E_V3_21_PRICE_MA);				// Sub Graph Count & User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_VOLUME_MA),							E_V3_21_VOLUME_MA);				// Sub Graph Count
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_RAINBOW_CHART),		E_V3_21_RAINBOW);				// User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_AB_RATIO),				E_V3_21_ABRATIO);				// Signal
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_BAND_PERCENT_B),		E_V3_21_BAND_PERCENT_B);		// User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_BAND_WIDTH),			E_V3_21_BAND_WIDTH);			// User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_BOLLINGER_BANDS),		E_V3_21_BOLLINGER_BANDS);		// User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_CCI),								E_V3_21_CCI);					// User Packet and Signal
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_DPO),								E_V3_21_DPO);					// User Packet and Signal
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_ENVELOPE),							E_V3_21_ENVELOPE);				// User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_LRS),								E_V3_21_LRS);					// User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_MACD),								E_V3_21_MACD);					// User Packet and Signal
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_MACD_OSCILLATOR),					E_V3_21_MACD_OSCILLATOR);		// User Packet and Signal
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_MFI),								E_V3_21_MFI);					// Default Packet and Signal
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_MOMENTUM),							E_V3_21_MOMENTUM);				// User Packet and Signal
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_NCO),								E_V3_21_NCO);					// User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_NVI),								E_V3_21_NVI);					// Default Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_OBV),								E_V3_21_OBV);					// Default Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_RSI),								E_V3_21_RSI);					// User Packet and Signal
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_STANDARD_DEVIATION),					E_V3_21_STANDARD_DEVIATION);	// User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_STANDARD_ERROR),						E_V3_21_STANDARD_ERROR);		// User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_TRIX),								E_V3_21_TRIX);					// User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_VHF),								E_V3_21_VHF);					// User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_ZIG_ZAG),							E_V3_21_ZIG_ZAG);				// User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_OVERHANGING_SUPPLY),					E_V3_21_VOLUME_FOR_SALE);		// User Packet
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_INVESTMENT_PSYCHOLOGICAL_LINE),		E_V3_21_OLD_PSYCHCHART);		// New Graph
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_DISPARITY),							E_V3_21_OLD_DISPARITYCHART);	// New Graph
	m_mapVersionUpIndicators.SetAt( "A/D Line",										E_V3_21_OLD_ADLINE);			// New Graph
	m_mapVersionUpIndicators.SetAt( "CO",											E_V3_21_OLD_CO);				// New Graph
	m_mapVersionUpIndicators.SetAt( "MI",											E_V3_21_OLD_MI);				// New Graph
	m_mapVersionUpIndicators.SetAt( "OSCP",											E_V3_21_OLD_PRICEOSCILLATOR);	// New Graph
	m_mapVersionUpIndicators.SetAt( "RoC",											E_V3_21_OLD_ROC);				// New Graph
	m_mapVersionUpIndicators.SetAt( "SIGMA",										E_V3_21_OLD_SIGMA);				// New Graph
	m_mapVersionUpIndicators.SetAt( "SONAR",										E_V3_21_OLD_SONAR);				// New Graph
	m_mapVersionUpIndicators.SetAt( "Stochastics Fast",								E_V3_21_OLD_STOCHASTICS_FAST);	// New Graph
	m_mapVersionUpIndicators.SetAt( "Stochastics Fast2",							E_V3_21_OLD_STOCHASTICS_FAST);	// New Graph
	m_mapVersionUpIndicators.SetAt( "Stochastics Fast3",							E_V3_21_OLD_STOCHASTICS_FAST);	// New Graph
	m_mapVersionUpIndicators.SetAt( "Stochastics Slow",								E_V3_21_OLD_STOCHASTICS_SLOW);	// New Graph
	m_mapVersionUpIndicators.SetAt( "Stochastics Slow2",							E_V3_21_OLD_STOCHASTICS_SLOW);	// New Graph
	m_mapVersionUpIndicators.SetAt( "Stochastics Slow3",							E_V3_21_OLD_STOCHASTICS_SLOW);	// New Graph
	m_mapVersionUpIndicators.SetAt( "William's %R",									E_V3_21_OLD_WILLIAM);			// New Graph

	// 1. Operations
	m_mapVersionUpOperation.SetAt( _V3_21_PRICE_MA_PACKET,				_MTEXT( C2_PRICE_MA));
	m_mapVersionUpOperation.SetAt( _V3_21_RAINBOW_PACKET,				_MTEXT( C2_RAINBOW_CHART));
	m_mapVersionUpOperation.SetAt( _V3_21_BAND_PERCENT_B_PACKET,		_MTEXT( C2_BAND_PERCENT_B));
	m_mapVersionUpOperation.SetAt( _V3_21_BAND_WIDTH_PACKET,			_MTEXT( C2_BAND_WIDTH));
	m_mapVersionUpOperation.SetAt( _V3_21_BOLLINGER_BANDS_PACKET,		_MTEXT( C2_BOLLINGER_BANDS));
	m_mapVersionUpOperation.SetAt( _V3_21_CCI_PACKET,					_MTEXT( C2_CCI));
	m_mapVersionUpOperation.SetAt( _V3_21_DPO_PACKET,					_MTEXT( C2_DPO));
	m_mapVersionUpOperation.SetAt( _V3_21_ENVELOPE_PACKET,				_MTEXT( C2_ENVELOPE));
	m_mapVersionUpOperation.SetAt( _V3_21_LRS_PACKET,					_MTEXT( C2_LRS));
	m_mapVersionUpOperation.SetAt( _V3_21_MACD_PACKET,					_MTEXT( C2_MACD));
	m_mapVersionUpOperation.SetAt( _V3_21_MACD_OSCILLATOR_PACKET,		_MTEXT( C2_MACD_OSCILLATOR));
	m_mapVersionUpOperation.SetAt( _V3_21_MOMENTUM_PACKET,				_MTEXT( C2_MOMENTUM));
	m_mapVersionUpOperation.SetAt( _V3_21_NCO_PACKET,					_MTEXT( C2_NCO));
	m_mapVersionUpOperation.SetAt( _V3_21_RSI_PACKET,					_MTEXT( C2_RSI));
	m_mapVersionUpOperation.SetAt( _V3_21_STANDARD_DEVIATION_PACKET,	_MTEXT( C2_STANDARD_DEVIATION));
	m_mapVersionUpOperation.SetAt( _V3_21_STANDARD_ERROR_PACKET,		_MTEXT( C2_STANDARD_ERROR));
	m_mapVersionUpOperation.SetAt( _V3_21_TRIX_PACKET,					_MTEXT( C2_TRIX));
	m_mapVersionUpOperation.SetAt( _V3_21_VHF_PACKET,					_MTEXT( C2_VHF));
	m_mapVersionUpOperation.SetAt( _V3_21_ZIG_ZAG_PACKET,				_MTEXT( C2_ZIG_ZAG));
	m_mapVersionUpOperation.SetAt( _V3_21_VOLUME_FOR_SALE_PACKET,		_MTEXT( C2_OVERHANGING_SUPPLY));

	CCfgFileVersion5Dot00::RegisterPrevOperation();
}

BOOL CCfgFileVersion3Dot21::GetNewGraphForDeletedOld( const char *p_szOldGraphName, CString &p_strNewGraphName, BOOL p_bPrevResult)
{
	BOOL	bResult			= TRUE;
	WORD	wIndicatorID;
	if( m_mapVersionUpIndicators.Lookup( p_szOldGraphName, wIndicatorID)) switch( wIndicatorID)
	{
		case E_V3_21_OLD_PSYCHCHART:		p_strNewGraphName = _MTEXT( C2_PSYCHOLOGICAL_LINE);
											break;
		case E_V3_21_OLD_DISPARITYCHART:	p_strNewGraphName = _MTEXT( C2_DISPARITY_SMA);
											break;
		case E_V3_21_OLD_ADLINE:			p_strNewGraphName = _MTEXT( C2_AD_LINE);
											break;
		case E_V3_21_OLD_CO:				p_strNewGraphName = _MTEXT( C2_CHAIKINS_OSCILLATOR);
											break;
		case E_V3_21_OLD_MI:				p_strNewGraphName = _MTEXT( C2_MASS_INDEX);
											break;
		case E_V3_21_OLD_PRICEOSCILLATOR:	p_strNewGraphName = _MTEXT( C2_PRICE_OSCILLATOR);
											break;
		case E_V3_21_OLD_ROC:				p_strNewGraphName = _MTEXT( C2_ROC);
											break;
		case E_V3_21_OLD_SIGMA:				p_strNewGraphName = _MTEXT( C2_SIGMA);
											break;
		case E_V3_21_OLD_SONAR:				p_strNewGraphName = _MTEXT( C2_SONAR);
											break;
		case E_V3_21_OLD_STOCHASTICS_FAST:	p_strNewGraphName = _MTEXT( C2_STOCHASTIC_FAST) + CString( p_szOldGraphName[ 16]);
											break;
		case E_V3_21_OLD_STOCHASTICS_SLOW:	p_strNewGraphName = _MTEXT( C2_STOCHASTIC_SLOW) + CString( p_szOldGraphName[ 16]);
											break;
		case E_V3_21_OLD_WILLIAM:			p_strNewGraphName = _MTEXT( C2_PERCENT_R);
											break;
		default:	bResult = FALSE;
					break;
	}

	return CCfgFileVersion5Dot00::GetNewGraphForDeletedOld( p_szOldGraphName, p_strNewGraphName, bResult || p_bPrevResult);
}

void CCfgFileVersion3Dot21::SetIndicatorPacketNameWithGraphPacketName( CMainBlock* pMainBlock, const char *p_szGraphName, CIndicatorInfo *pSourceInfo)
{
	CString strPacketName = pMainBlock->GetGraphPacketName( p_szGraphName);
	if( CString( _MTEXT( C0_HIGH)) + "," + _MTEXT( C0_LOW) + "," + _MTEXT( C0_CLOSE) + "," == strPacketName)
		strPacketName = CString( "(") + _MTEXT( C0_HIGH) + "+" + _MTEXT( C0_LOW) + "+" + _MTEXT( C0_CLOSE) + ")/3,";
	pSourceInfo->SetCommonPacketNames( strPacketName);
}

// (2006/6/2 - Seung-Won, Bae) Version Up for Each version and Call Next Version.
BOOL CCfgFileVersion3Dot21::VersionUpOldInfo( const char *szGraphName, CIndicatorInfo *pSourceInfo, CKTBChartCtrl* pChartCtrl, CMainBlock* pMainBlock, bool &p_bSubGraphChanged)
{
	// 1. Update User Select Packet Info. and Signal
	WORD	wIndicatorID;
	BOOL	bUpdateToGraph = FALSE;
	if( m_mapVersionUpIndicators.Lookup( szGraphName, wIndicatorID))
	{
		CString		strDummy;
		bUpdateToGraph = TRUE;
		switch( wIndicatorID)
		{
			// 1. Price MA, packet
			case E_V3_21_PRICE_MA:
				// 1.5.2 Check Packet ~ Set Sub Packet Name
				if( m_mapVersionUpOperation.Lookup( _V3_21_PRICE_MA_PACKET, strDummy))
				{
					CString strPacketName = pMainBlock->GetGraphPacketName( strDummy);
					if( CString( _MTEXT( C0_HIGH)) + "," + _MTEXT( C0_LOW) + "," + _MTEXT( C0_CLOSE) + "," == strPacketName)
						strPacketName = CString( "(") + _MTEXT( C0_HIGH) + "+" + _MTEXT( C0_LOW) + "+" + _MTEXT( C0_CLOSE) + ")/3,";
					for( int i = 0; i < 6; i++) pSourceInfo->SetPacketNames( i, strPacketName);
				}
				break;

			// 2. Rainbow, packet
			case E_V3_21_RAINBOW:
				if( m_mapVersionUpOperation.Lookup( _V3_21_RAINBOW_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 3. Band %B, packet
			case E_V3_21_BAND_PERCENT_B:
				if( m_mapVersionUpOperation.Lookup( _V3_21_BAND_PERCENT_B_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 4. Band Width, packet
			case E_V3_21_BAND_WIDTH:
				if( m_mapVersionUpOperation.Lookup( _V3_21_BAND_WIDTH_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 5. Bollinger Bands, packet
			case E_V3_21_BOLLINGER_BANDS:
				if( m_mapVersionUpOperation.Lookup( _V3_21_BOLLINGER_BANDS_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 6. CCI, packet & signal
			case E_V3_21_CCI:
				if( m_mapVersionUpOperation.Lookup( _V3_21_CCI_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 7. DPO, packet & signal
			case E_V3_21_DPO:
				if( m_mapVersionUpOperation.Lookup( _V3_21_DPO_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 8. Envelope, packet
			case E_V3_21_ENVELOPE:
				if( m_mapVersionUpOperation.Lookup( _V3_21_ENVELOPE_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 9. LRS, packet
			case E_V3_21_LRS:
				if( m_mapVersionUpOperation.Lookup( _V3_21_LRS_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 10. MACD, packet
			case E_V3_21_MACD:
				if( m_mapVersionUpOperation.Lookup( _V3_21_MACD_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 11. MACD Oscillator, packet
			case E_V3_21_MACD_OSCILLATOR:
				if( m_mapVersionUpOperation.Lookup( _V3_21_MACD_OSCILLATOR_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 13. Momentum, packet ans signal
			case E_V3_21_MOMENTUM:
				if( m_mapVersionUpOperation.Lookup( _V3_21_MOMENTUM_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 14. NCO, packet
			case E_V3_21_NCO:
				if( m_mapVersionUpOperation.Lookup( _V3_21_NCO_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 15. RSI, packet and signal
			case E_V3_21_RSI:
				if( m_mapVersionUpOperation.Lookup( _V3_21_RSI_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 16. Standard Deviation, packet
			case E_V3_21_STANDARD_DEVIATION:
				if( m_mapVersionUpOperation.Lookup( _V3_21_STANDARD_DEVIATION_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 17. Standard Error, packet
			case E_V3_21_STANDARD_ERROR:
				if( m_mapVersionUpOperation.Lookup( _V3_21_STANDARD_ERROR_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 18. TRIX, packet and signal
			case E_V3_21_TRIX:
				if( m_mapVersionUpOperation.Lookup( _V3_21_TRIX_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 19. VHF, packet and signal
			case E_V3_21_VHF:
				if( m_mapVersionUpOperation.Lookup( _V3_21_VHF_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// 20. Zig Zag packet
			case E_V3_21_ZIG_ZAG:
				if( m_mapVersionUpOperation.Lookup( _V3_21_ZIG_ZAG_PACKET, strDummy))
					SetIndicatorPacketNameWithGraphPacketName( pMainBlock, strDummy, pSourceInfo);
				break;

			// *. Etc, signal
			case E_V3_21_ABRATIO:
			case E_V3_21_MFI:		// Default packet and signal
			case E_V3_21_NVI:		// Default packet
			case E_V3_21_OBV:		// Default packet
				break;

			// *. Most of All in Packet and Signal Update needs to update the Graph with Indicator Info.
			//		Because the Packet and Signal Info is already incorrect in Graph.
			default:
				bUpdateToGraph = FALSE;
				break;
		}
	}

	BOOL bUpdateToGraphNext = CCfgFileVersion5Dot00::VersionUpOldInfo( szGraphName, pSourceInfo, pChartCtrl, pMainBlock, p_bSubGraphChanged);
	return bUpdateToGraph || bUpdateToGraphNext;
}

//////////////////////////////////////////////////////////////////////
// class CCfgFileVersion5Dot00 -> user file version (5.00) && default file version (4.3 or 4.4)
CCfgFileVersion5Dot00::CCfgFileVersion5Dot00( void)
{
}

// (2006/5/30 - Seung-Won, Bae) Override for Registering and Denying the previous Operation
//		Caution! You should call the base member function in the end of routine.
void CCfgFileVersion5Dot00::RegisterPrevOperation( void)
{
	// 0. Target Graph
	m_mapVersionUpOperation.SetAt( _V5_00_THREELINEBREAK_FILL,			_MTEXT( C2_THREE_LINE_BREAK));
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_THREE_LINE_BREAK),			E_V5_00_THREELINEBREAK);

}												
//sy 2006.04.17.

BOOL CCfgFileVersion5Dot00::GetNewGraphForDeletedOld( const char *p_szOldGraphName, CString &p_strNewGraphName, BOOL p_bPrevResult)
{
	BOOL	bResult			= TRUE;
	return CCfgFileVersionBase::GetNewGraphForDeletedOld( p_szOldGraphName, p_strNewGraphName, bResult || p_bPrevResult);
}

// (2006/6/2 - Seung-Won, Bae) Version Up for Each version and Call Next Version.
BOOL CCfgFileVersion5Dot00::VersionUpOldInfo( const char *szGraphName, CIndicatorInfo *pSourceInfo, CKTBChartCtrl* pChartCtrl, CMainBlock* pMainBlock, bool &p_bSubGraphChanged)
{
	// 1. Update User Select Packet Info. and Signal
	WORD	wIndicatorID;
	BOOL	bUpdateToGraph = FALSE;
	if( m_mapVersionUpIndicators.Lookup( szGraphName, wIndicatorID))
	{
		CString		strDummy;
		bUpdateToGraph = TRUE;
		switch( wIndicatorID)
		{
			// 1. THREELINEBREAK
			case E_V5_00_THREELINEBREAK:
				break;
			default:
				bUpdateToGraph = FALSE;
				break;
		}
	}

	BOOL bUpdateToGraphNext = CCfgFileVersionBase::VersionUpOldInfo( szGraphName, pSourceInfo, pChartCtrl, pMainBlock, p_bSubGraphChanged);
	return bUpdateToGraph || bUpdateToGraphNext;
}
