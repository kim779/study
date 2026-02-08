// FileBuilder.cpp: implementation of the CFileBuilder class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileBuilder.h"

#include <io.h>
#include <assert.h>
#include "../Include_Chart/DLL_Load/GraphTypeData.h"
#include "../Include_Chart/DLL_Load/IndicatorInfo.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()
#include "Conversion.h"
#include "IndicatorConditionBaseData.h"
#include "Graph.h"		// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CFileBuilder

// public =====================================================================
// file 을 open 하고 data를 읽는다.
char *CFileBuilder::FileRead( const char *p_szFilePathAndName, CString &p_strFileData) const
{
	p_strFileData.Empty();
	if( !p_szFilePathAndName) return NULL;
	if( !*p_szFilePathAndName) return NULL;

	FILE *stream = fopen( p_szFilePathAndName, "rb");
	if(stream == NULL) return NULL;
	
	//fseek( stream, 0L, SEEK_SET);
	//long nbytes = _filelength(stream->_file);

	fseek(stream, 0L, SEEK_END);//tour2k.2022.7.27
	long nbytes = ftell(stream);
	fseek(stream, 0L, SEEK_SET);

	char* buffer = 	p_strFileData.GetBufferSetLength( nbytes);
	fread( buffer, sizeof(char), nbytes, stream);
    fclose( stream);
	return buffer;
}

// file 을 생성하고 data를 쓴다. (이미 file이 존재하면 해당 file에 내용을 쓴다.)
bool CFileBuilder::FileCreatWrite( const char *p_szFilePathAndName, CStringList &p_slStringData)
{
	// 해당 file에 data 쓰기.
	FILE *stream = fopen( p_szFilePathAndName, "wb+");
	if(stream == NULL) return false;

	fseek(stream, 0L, SEEK_END);

	POSITION psn = p_slStringData.GetHeadPosition();
	while( psn)
	{
		CString &strData = p_slStringData.GetNext( psn);
		fwrite( strData, sizeof(char), strData.GetLength(), stream);
		//fprintf(stream, "%s", writeData);
	}

    fclose(stream);
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// class CIndicatorBuilder -> indicator data file data 객체로 만들기

// protected ==================================================================
// [Name:Comment] => [, Name\0Comment]
char *CIndicatorBuilder::GetItemName( char *&p_szDataString) const
{
	if( !p_szDataString) return NULL;
	if( !*p_szDataString) return NULL;

	while( *p_szDataString == ' ') p_szDataString++;

	// 주석처리인 경우.
	if( *( p_szDataString + 1) == '\'') return NULL;

	char *szStart = p_szDataString + 1;						// skip '['
	char *szEnd = strchr( p_szDataString, _ITEMNAME_END);
	if( !szEnd) return false;
	*szEnd++ = '\0';

	p_szDataString = szEnd;
	szEnd = strchr( szStart, _ITEMNAME_MIDDLE_END);
	if( szEnd) *szEnd = '\0';
	else szEnd = p_szDataString - 1;

	// RTrim
	szEnd--;
	while( *szEnd == ' ') *szEnd-- = '\0';

	return szStart;
}

///////////////////////////////////////////////////////////////////////////////
// class CGraphTypeOrStyleDataBuilder -> graph type or style

const int CGraphTypeOrStyleDataBuilder::nLen_GRAPH_TYPE_START	= strlen( _GRAPH_TYPE_START);
const int CGraphTypeOrStyleDataBuilder::nLen_GRAPH_TYPE_END		= strlen( _GRAPH_TYPE_END);
const int CGraphTypeOrStyleDataBuilder::nLen_GRAPH_STYLE_START	= strlen( _GRAPH_STYLE_START);
const int CGraphTypeOrStyleDataBuilder::nLen_GRAPH_STYLE_END	= strlen( _GRAPH_STYLE_END);

bool CGraphTypeOrStyleDataBuilder::FileDataBuild_GraphType( char *&p_szDataString, CGraphTypeDataList& graphTypeDataList)
{
	char *szStart = strstr( p_szDataString, _GRAPH_TYPE_START);
	if( !szStart) return false;
	szStart += nLen_GRAPH_TYPE_START;

	char *szEnd = strstr( szStart, _GRAPH_TYPE_END);
	if( !szEnd) return false;
	*szEnd = '\0';
	szEnd += nLen_GRAPH_TYPE_END;

	bool bResult = SetGraphTypeDataList( szStart, graphTypeDataList);
	p_szDataString = szEnd;

	return bResult;
}

bool CGraphTypeOrStyleDataBuilder::FileDataBuild_GraphStyle( char *&p_szDataString, CGraphTypeDataList& graphStyleDataList)
{
	char *szStart = strstr( p_szDataString, _GRAPH_STYLE_START);
	if( !szStart) return false;
	szStart += nLen_GRAPH_STYLE_START;

	char *szEnd = strstr( szStart, _GRAPH_STYLE_END);
	if( !szEnd) return false;
	*szEnd = '\0';
	szEnd += nLen_GRAPH_STYLE_END;

	bool bResult = SetGraphTypeDataList( szStart, graphStyleDataList);
	p_szDataString = szEnd;

	return bResult;
}


// private ====================================================================
bool CGraphTypeOrStyleDataBuilder::SetGraphTypeDataList( char *&p_szDataString, CGraphTypeDataList& graphTypeDataList)
{
	graphTypeDataList.DeleteAll();

	if( !p_szDataString) return false;
	if( !*p_szDataString) return false;

	char *szStart = strchr( p_szDataString, _ITEMNAME_START);
	char *szEnd = NULL;
	while( szStart)
	{
		szEnd = strchr( szStart + 1, _ITEMNAME_START);
		if( szEnd) *( szEnd - 1) = '\0';

		graphTypeDataList.AddTail( MakeGraphTypeData( szStart));

		szStart = szEnd;
	}

	return (graphTypeDataList.GetCount() > 0);
}

// ----------------------------------------------------------------------------
CGraphTypeData* CGraphTypeOrStyleDataBuilder::MakeGraphTypeData( char *&p_szDataString)
{
	char *szName = GetItemName( p_szDataString);
	if( !szName) return NULL;
	if( !*szName) return NULL;

	CGraphTypeData* pGraphTypeData = CGraphTypeData::Make();
	if( pGraphTypeData == NULL) return false;

	// name
	pGraphTypeData->SetName( szName);
	// style 들
	SetGraphStyleData( p_szDataString, *pGraphTypeData->GetStyleDataList());
	return pGraphTypeData;
}

void CGraphTypeOrStyleDataBuilder::SetGraphStyleData( char *&p_szDataString, CStringDataList& styleDataList)
{
	styleDataList.RemoveAll();
	if( !p_szDataString) return;
	if( !*p_szDataString) return;

	int nStartSize = 2;
	// (2006/11/2 - Seung-Won, Bae) Search '\r' instead of "\r\n" for last '\r' with null before GetItemName().
	char *szStart = strchr( p_szDataString, '\r');	// skip [] line.
	char *szEnd = NULL;
	while( szStart)
	{
		szStart += nStartSize;

		szEnd = strchr( szStart, '\r');
		if( szEnd) *szEnd = '\0';

		if( *szStart)
		{
			styleDataList.AddTail( szStart);
			szStart = szEnd;
		}
		else szStart = NULL;
	}
}


///////////////////////////////////////////////////////////////////////////////
// class CDefaultIndicatorListBuilder -> default file

const int CDefaultIndicatorListBuilder::nLen_GENERAL_START	= strlen( _GENERAL_START);
const int CDefaultIndicatorListBuilder::nLen_GENERAL_END	= strlen( _GENERAL_END);
const int CDefaultIndicatorListBuilder::nLen_SPECIAL_START	= strlen( _SPECIAL_START);
const int CDefaultIndicatorListBuilder::nLen_SPECIAL_END	= strlen( _SPECIAL_END);
const int CDefaultIndicatorListBuilder::nLen_NONEINDE_START	= strlen( _NONEINDE_START);
const int CDefaultIndicatorListBuilder::nLen_NONEINDE_END	= strlen( _NONEINDE_END);
// public =====================================================================
bool CDefaultIndicatorListBuilder::FileDataBuild( CBaseIndicatorList* pBaseIndiList, char *&p_szDataString)
{
	if( pBaseIndiList == NULL) return false;
	if( !p_szDataString) return false;
	if( !*p_szDataString) return false;

	// 일반지표
	// ***** General *****
	// [가격차트]
	// ...
	//
	// [가격 이동평균]
	// ...
	//
	// ***** General End *****
	char *szStart = strstr( p_szDataString, _GENERAL_START);
	if( !szStart) return false;
	szStart += nLen_GENERAL_START;

	char *szEnd = strstr( szStart, _GENERAL_END);
	if( !szEnd) return false;
	*szEnd = '\0';
	szEnd += nLen_GENERAL_END;

	bool bResult = IndicatorListBuild( pBaseIndiList->GetGeneralIndiMap(), szStart);
	p_szDataString = szEnd;

	// 특수지표
	// ***** Special *****
	// [P&F]
	// ...
	//
	// [삼선전환도]
	// ...
	//
	// ***** Special End *****
	szStart = strstr( p_szDataString, _SPECIAL_START);
	if( !szStart) return false;
	szStart += nLen_SPECIAL_START;

	szEnd = strstr( szStart, _SPECIAL_END);
	if( !szEnd) return false;
	*szEnd = '\0';
	szEnd += nLen_SPECIAL_END;

	if( IndicatorListBuild( pBaseIndiList->GetSpecialIndiMap(), szStart)) bResult = true;
	p_szDataString = szEnd;

	// none지표
	// ***** None Indicator *****
	// [상대비교차트]
	// ...
	//
	// [System Signal]
	// ...
	//
	// ***** None Indicator End *****
	szStart = strstr( p_szDataString, _NONEINDE_START);
	if( !szStart) return false;
	szStart += nLen_NONEINDE_START;

	szEnd = strstr( szStart, _NONEINDE_END);
	if( !szEnd) return false;
	*szEnd = '\0';
	szEnd += nLen_NONEINDE_END;

	if( CDefaultIndicatorListBuilder::IndicatorListBuild( ( ( CDefaultIndicatorList*)pBaseIndiList)->GetNoneIndiMap(), szStart)) bResult = true;
	p_szDataString = szEnd;

	return bResult;
}

// Indicator List
bool CDefaultIndicatorListBuilder::IndicatorListBuild( CIndicatorInfoMap *p_pIndiInfoMap, char *&p_szDataString)
{
	if( !p_szDataString || !p_pIndiInfoMap) return false;
	if( !*p_szDataString) return false;

	EIndicatorInfoType eIndicatorInfoType = p_pIndiInfoMap->GetIndicatorInfoType();
	char *szStart = strchr( p_szDataString, _ITEMNAME_START);
	char *szEnd = NULL;
	while( szStart)
	{
		szEnd = strchr( szStart + 1, _ITEMNAME_START);
		if( szEnd) *( szEnd - 1) = '\0';

		// Skip commented Info
		if( *( szStart + 1) != '\'')
		{
			CIndicatorInfo* pNewIndiInfo = MakeIndicatorInfo( eIndicatorInfoType, szStart);
			if( pNewIndiInfo) if( !p_pIndiInfoMap->AddIndicatorInfo( pNewIndiInfo)) delete pNewIndiInfo;
		}

		szStart = szEnd;
	}

	return true;
}

CIndicatorInfo* CDefaultIndicatorListBuilder::MakeIndicatorInfo( EIndicatorInfoType p_eIndicatorInfoType, char *&p_szDataString)
{
	if( !p_szDataString) return NULL;
	if( !*p_szDataString) return NULL;

	CIndicatorInfo* pNewIndiInfo = new CIndicatorInfo( p_eIndicatorInfoType);
	if( ReadInfoCfgStrToObject( pNewIndiInfo, p_szDataString)) return pNewIndiInfo;

	delete pNewIndiInfo;
	return NULL;
}

// private ====================================================================



///////////////////////////////////////////////////////////////////////////////
// class CDefaultTypeIndiListBuilder -> indicator list : default file data 형태

CDefaultTypeIndiListBuilder::CDefaultTypeIndiListBuilder( CGraphTypeAndStyleData* pGraphTypeAndStyleData, HWND p_hOcxWnd) : CDefaultIndicatorListBuilder( p_hOcxWnd)
{
	m_pGraphTypeAndStyleData = pGraphTypeAndStyleData;
}


// protected ==================================================================
// file에서 읽은 info data 를 info에 적용하기. (데이터 검증을 통함)
bool CDefaultTypeIndiListBuilder::ReadInfoCfgStrToObject( CIndicatorInfo* pIndicatorInfo, char *&p_szDataString)
{
	if( pIndicatorInfo == NULL) return false;
	if( m_pGraphTypeAndStyleData == NULL) return false;

	CDefaultTypeIndiInfoBuilder indiInfoBuilder( m_pGraphTypeAndStyleData, m_hOcxWnd);
	return indiInfoBuilder.FileDataBuild( pIndicatorInfo, p_szDataString);
}

///////////////////////////////////////////////////////////////////////////////
// class CDefaultUserTypeIndiListBuilder -> user type 이 default type 형태 (user file에만 해당)


// protected ==================================================================
// file에서 읽은 info data 를 info에 적용하기. (데이터 검증을 통함)
bool CDefaultUserTypeIndiListBuilder::ReadInfoCfgStrToObject( CIndicatorInfo* pIndicatorInfo, char *&p_szDataString)
{
	if(pIndicatorInfo == NULL) return false;
	CUserTypeIndiInfoBuilder indiInfoBuilder( m_hOcxWnd);
	return indiInfoBuilder.FileDataBuild( pIndicatorInfo, p_szDataString);
}

// public =====================================================================
// 저장할 file data.
void CDefaultUserTypeIndiListBuilder::GetWritingFileData( const CUserIndicatorList& userIndiList, CStringList &p_slDataString) const
{
	// 일반지표
	POSITION psnTail = p_slDataString.GetTailPosition();
	GetWritingFileData( userIndiList.GetIndicatorListType(), userIndiList.GetGeneralIndiMap(), p_slDataString);
	if( p_slDataString.GetTailPosition() != psnTail)
	{
		p_slDataString.InsertAfter( psnTail, _GENERAL_START);
		p_slDataString.AddTail( _GENERAL_END);
	}

	// 특수지표
	psnTail = p_slDataString.GetTailPosition();
	GetWritingFileData( userIndiList.GetIndicatorListType(), userIndiList.GetSpecialIndiMap(), p_slDataString);
	if( p_slDataString.GetTailPosition() != psnTail)
	{
		p_slDataString.InsertAfter( psnTail, _SPECIAL_START);
		p_slDataString.AddTail( _SPECIAL_END);
	}

	// 
	psnTail = p_slDataString.GetTailPosition();
	GetWritingFileData( userIndiList.GetIndicatorListType(), userIndiList.GetNoneIndiMap(), p_slDataString);
	if( p_slDataString.GetTailPosition() != psnTail)
	{
		p_slDataString.InsertAfter( psnTail, _NONEINDE_START);
		p_slDataString.AddTail( _NONEINDE_END);
	}
}

// private ====================================================================
// 저장할 file data.
void CDefaultUserTypeIndiListBuilder::GetWritingFileData( EIndicatorListType p_eIndicatorListType, const CIndicatorInfoMap *p_pIndiInfoMap, CStringList &p_slDataString) const
{
	if( !p_pIndiInfoMap) return;
	if( p_pIndiInfoMap->GetCount() <= 0) return;

	CUserTypeIndiInfoBuilder indiInfoBuilder( m_hOcxWnd, p_eIndicatorListType);
	
	CString strKey;
	CIndicatorInfo *pIndicatorInfo = NULL;
	POSITION psnIndicatorInfo = p_pIndiInfoMap->GetStartPosition();
	while( psnIndicatorInfo)
	{
		p_pIndiInfoMap->GetNextAssoc( psnIndicatorInfo, strKey, pIndicatorInfo);
		indiInfoBuilder.GetWritingFileData( *pIndicatorInfo, p_slDataString);
	}
}

///////////////////////////////////////////////////////////////////////////////
// class CNotDefaultUserTypeIndiListBuilder -> user type 이 default type 이전 형태 (user file에만 해당)

CNotDefaultUserTypeIndiListBuilder::CNotDefaultUserTypeIndiListBuilder( CDefaultIndicatorList* pDefaultIndicatorList, HWND p_hOcxWnd) : CDefaultIndicatorListBuilder( p_hOcxWnd)
{
	m_pDefaultIndicatorList = pDefaultIndicatorList;
	m_bInited = FALSE;
}

// (2006/5/27 - Seung-Won, Bae) Add 'General/Special' Indicator Info from CFG Info List String to List
// (2006/5/30 - Seung-Won, Bae) Override for Denying the previous Operation
bool CNotDefaultUserTypeIndiListBuilder::FileDataBuild( CBaseIndicatorList* pBaseIndiList, char *&p_szDataString)
{
	if( !m_bInited)
	{
		RegisterPrevOperation();
		DenyPrevOperation();
		m_bInited = TRUE;
	}

	return CDefaultIndicatorListBuilder::FileDataBuild( pBaseIndiList, p_szDataString);
}

// protected ==================================================================
// subGraph 갯수나 name는 변경될 수 없다.
// default 값을 기본으로 하고 변경된 내용만을 수정한다.
CIndicatorInfo* CNotDefaultUserTypeIndiListBuilder::MakeIndicatorInfo( EIndicatorInfoType p_eIndicatorInfoType, char *&p_szDataString)
{
	char *szName = NULL;

	// strIndiInfoData 에서 Indicator Name 을 가지고 와서 default Info를 찾는다.
	CIndicatorInfo* pNewIndicatorInfo = NULL;
	if( m_pDefaultIndicatorList)
	{
		//char *szName = GetItemName( p_szDataString); 이거 풀지 마삼...죽음이야..
		szName = GetItemName( p_szDataString);
		if( !szName) return NULL;								// Old Indicator Info damaged.

		CIndicatorInfo* pDefaultIndiInfo = m_pDefaultIndicatorList->GetIndicatorInfo( szName, CChartInfo::DEVELOPER);
		if( !pDefaultIndiInfo) return NULL;						// Old Indicator will not supported any more.

		pNewIndicatorInfo = CIndicatorInfo::Make( pDefaultIndiInfo);
		if( !pNewIndicatorInfo) return NULL;					// Copy Object Creation failed.

		if( pNewIndicatorInfo->GetIndicatorName().IsEmpty())	// Copy Failed.
		{
			if( !pNewIndicatorInfo) delete pNewIndicatorInfo;
			return NULL;
		}
	}

	// default data 형태에 맞춰서 user 내용을 적용한다.
	if( UpdateInfoCfgStrToObject( pNewIndicatorInfo, p_szDataString, szName)) return pNewIndicatorInfo;

	delete pNewIndicatorInfo;
	return NULL;
}

// file에서 읽은 info data 를 info에 적용하기. (데이터 검증을 통함)
bool CNotDefaultUserTypeIndiListBuilder::UpdateInfoCfgStrToObject(CIndicatorInfo* pIndicatorInfo, char *&p_szDataString, const char *p_szName)
{
	if(pIndicatorInfo == NULL) return false;

	// pIndicatorInfo는 default 를 copy 해서 만든 것이므로
	// pIndicatorInfo에 sourceIndiInfo 중 변경될 내용만으로 수정한다.
	CIndicatorInfo sourceIndiInfo( pIndicatorInfo->GetIndicatorInfoType());
	CUserTypeIndiInfoBuilder indiInfoBuilder( m_hOcxWnd);
	indiInfoBuilder.FileDataBuild( &sourceIndiInfo, p_szDataString, p_szName);
	CString cspName,csSourceName;
	cspName = pIndicatorInfo->GetIndicatorName();
	csSourceName = sourceIndiInfo.GetIndicatorName();
//	if( pIndicatorInfo->GetIndicatorName() != sourceIndiInfo.GetIndicatorName()) return false;
	if( pIndicatorInfo->GetIndicatorName() != sourceIndiInfo.GetIndicatorName()) return false;
	if( !VersionUpOldInfo( pIndicatorInfo, &sourceIndiInfo)) return false;

	if( !pIndicatorInfo->CopyDataWithVerificationFrom( sourceIndiInfo)) return false;
	// (2007/4/9 - Seung-Won, Bae) Update Graph Name
	CGraphDataBase *pGraphDataBase = ( CGraphDataBase *)&pIndicatorInfo->GetGraphData();
	pGraphDataBase->SetGraphName( sourceIndiInfo.GetGraphData().GetGraphName());
	return true;
}


///////////////////////////////////////////////////////////////////////////////
// class CUserVer3Dot21TypeIndiListBuilder

CUserVer3Dot21TypeIndiListBuilder::CUserVer3Dot21TypeIndiListBuilder( CDefaultIndicatorList* pDefaultIndicatorList, HWND p_hOcxWnd):
		CUserVer5Dot00TypeIndiListBuilder( pDefaultIndicatorList, p_hOcxWnd)
{
}

// (2006/6/7 - Seung-Won, Bae) Override for Moving of VolumeForSale Chart, from Special to General
// (2006/12/3 - Seung-Won, Bae) Comment for Multiple Graph of the Indicator Info.
//bool CUserVer3Dot21TypeIndiListBuilder::FileDataBuild( CBaseIndicatorList* pBaseIndiList, char *&p_szDataString)
//{
//	if( !pBaseIndiList) return false;
//	bool bResult = CUserVer5Dot00TypeIndiListBuilder::FileDataBuild( pBaseIndiList, p_szDataString);
//
//	// (2006/6/7 - Seung-Won, Bae) Move Indicator Info of VolueForSale.
//	CIndicatorInfoMap &iilSpacialIndicatorMap = pBaseIndiList->GetSpecialIndiMap();
//	CIndicatorInfo *pIndicatorInfo = pBaseIndiList->GetIndicatorInfo( iilSpacialIndicatorList, _MTEXT( C2_OVERHANGING_SUPPLY));
//	if( pIndicatorInfo)
//	{
//		pBaseIndiList->AddTail( CIndicatorInfo::Make( pIndicatorInfo));
//		pBaseIndiList->Delete( iilSpacialIndicatorList, pIndicatorInfo);
//	}
//
//	return bResult;
//}

// (2006/5/30 - Seung-Won, Bae) Override for Registering and Denying the previous Operation
//		Caution! You should call the base member function in the end of routine.
void CUserVer3Dot21TypeIndiListBuilder::RegisterPrevOperation( void)
{
	// (2006/5/29 - Seung-Won, Bae) Register Operation.
	m_mapVersionUpOperation.SetAt( _V3_21_PRICE_GRAPHDATADRAWING,		_MTEXT( C2_PRICE_CHART));
	m_mapVersionUpOperation.SetAt( _V3_21_PRICE_MINMAXDATADRAWING,		_MTEXT( C2_PRICE_CHART));
	m_mapVersionUpOperation.SetAt( _V3_21_PRICE_MA_CAL_CND,				_MTEXT( C2_PRICE_MA));
	m_mapVersionUpOperation.SetAt( _V3_21_VOLUME_DRAW_CND,				_MTEXT( C2_VOLUME_CHART));
	m_mapVersionUpOperation.SetAt( _V3_21_VOLUME_MA_CAL_CND,			_MTEXT( C2_VOLUME_MA));
	m_mapVersionUpOperation.SetAt( _V3_21_GLANCEBALANCE_CAL_CND,		_MTEXT( C2_OBVIOUSLY_BALANCE_CHART));
	m_mapVersionUpOperation.SetAt( _V3_21_ADX_CAL_CND,					_MTEXT( C2_ADX));
	m_mapVersionUpOperation.SetAt( _V3_21_ATR_CAL_CND,					_MTEXT( C2_ATR));
	m_mapVersionUpOperation.SetAt( _V3_21_BAND_PERCENT_B_CAL_CND,		_MTEXT( C2_BAND_PERCENT_B));
	m_mapVersionUpOperation.SetAt( _V3_21_DPO_CAL_CND,					_MTEXT( C2_DPO));
	m_mapVersionUpOperation.SetAt( _V3_21_DPO_COLOR,					_MTEXT( C2_DPO));
	m_mapVersionUpOperation.SetAt( _V3_21_ENVELOPE_CAL_CND,				_MTEXT( C2_ENVELOPE));
	m_mapVersionUpOperation.SetAt( _V3_21_LRS_CAL_CND,					_MTEXT( C2_LRS));
	m_mapVersionUpOperation.SetAt( _V3_21_MAC_CAL_CND,					_MTEXT( C2_MAC));
	m_mapVersionUpOperation.SetAt( _V3_21_MACD_CAL_CND,					_MTEXT( C2_MACD));
	m_mapVersionUpOperation.SetAt( _V3_21_STANDARD_DEVIATION_CAL_CND,	_MTEXT( C2_STANDARD_DEVIATION));
	m_mapVersionUpOperation.SetAt( _V3_21_ZIG_ZAG_CAL_CND,				_MTEXT( C2_ZIG_ZAG));

	// (2006/5/29 - Seung-Won, Bae) Manage Chart Name to check the Target for Version Up.
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_PRICE_CHART),				E_V3_21_PRICE);
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_PRICE_MA),								E_V3_21_PRICE_MA);
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_VOLUME_CHART),				E_V3_21_VOLUME);
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_VOLUME_MA),								E_V3_21_VOLUME_MA);
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_OBVIOUSLY_BALANCE_CHART),	E_V3_21_GLANCEBALANCE);
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_ADX),						E_V3_21_ADX);
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_ATR),						E_V3_21_ATR);
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_BAND_PERCENT_B),			E_V3_21_BAND_PERCENT_B);
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_DPO),						E_V3_21_DPO);
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_ENVELOPE),			E_V3_21_ENVELOPE);
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_MAC),					E_V3_21_MAC);
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_MACD),						E_V3_21_MACD);
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_STANDARD_DEVIATION),		E_V3_21_STANDARD_DEVIATION);
	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_ZIG_ZAG),			E_V3_21_ZIG_ZAG);

	CUserVer5Dot00TypeIndiListBuilder::RegisterPrevOperation();
}


// (2006/5/27 - Seung-Won, Bae) Make Old Version Info to New Version Type for Next Version Up.
//		Version Up Target	:	The Item that is supported in Copy with verification. (look IndicatorInfo.h)
//									and It can be changed by User in Old Version.
//									and You want it's changing not to work in new version.
//								Or The Item that is not supported in Copy with verification. (look IndicatorInfo.h)
//									and Customer want it's changing to work in new version.
//		Caution!	1. VersionUpOldInfo() of Base Class must be called for Next Version Up.
//					2. Do Verion Up by Item Unit in Each Indicator Info with Indicator Info Name.
//							Because Next Version Up must control the Previous Version Up Operation.
//					3. return FALSE in only purpose not to load Old Version CFG Info.
BOOL CUserVer3Dot21TypeIndiListBuilder::VersionUpOldInfo( CIndicatorInfo* pTargetInfo, CIndicatorInfo* pSourceInfo)
{
	// 0. Check Target
	WORD wIndicatorID;
	CString strDummy;
	CString strIndicatorName = pTargetInfo->GetIndicatorName();
	if( m_mapVersionUpIndicators.Lookup( strIndicatorName, wIndicatorID)) switch( wIndicatorID)
	{
		// 1. Check Price Chart
		case E_V3_21_PRICE:
		{
			// 1.1 Update 'graphdatadrawing'
			if( m_mapVersionUpOperation.Lookup( _V3_21_PRICE_GRAPHDATADRAWING, strDummy))
			{
				CGraphDataDrawingData *pGraphDataDrawingData = pTargetInfo->GetGraphDataDrawingData();
				if( pGraphDataDrawingData) pSourceInfo->SetGraphDataDrawingDataWithCopy( pGraphDataDrawingData);
			}
			// 1.2 Update 'minmaxdatadrawing'
			if( m_mapVersionUpOperation.Lookup( _V3_21_PRICE_MINMAXDATADRAWING, strDummy))
			{
//				CViewMinMaxDrawingData *pViewMinMaxDrawingData = pTargetInfo->GetViewMinMaxDrawingData();
//				if( pViewMinMaxDrawingData) pSourceInfo->SetViewMinMaxDrawingDataWithCopy( pViewMinMaxDrawingData);
			}
			break;
		}

		// 2. Check Price MA
		case E_V3_21_PRICE_MA:
		{
			// 1.5.1 Check Calculate Condition.
			if( m_mapVersionUpOperation.Lookup( _V3_21_PRICE_MA_CAL_CND, strDummy))
				SetMAComCalToSubCal( pSourceInfo);
			break;
		}

		// 3. Check Volume
		case E_V3_21_VOLUME:
			if( m_mapVersionUpOperation.Lookup( _V3_21_VOLUME_DRAW_CND, strDummy))
			{
				CList<double, double>* pDrawCondList = pSourceInfo->GetDrawConditionList();
				if( !pDrawCondList) break;
				pDrawCondList->AddTail( 0.0);
			}
			break;

		// 4. Check Volume MA
		case E_V3_21_VOLUME_MA:
			if( m_mapVersionUpOperation.Lookup( _V3_21_VOLUME_MA_CAL_CND, strDummy))
				SetMAComCalToSubCal( pSourceInfo);
			break;

		// 5. Check Glance-Balance
		case E_V3_21_GLANCEBALANCE:
			if( m_mapVersionUpOperation.Lookup( _V3_21_GLANCEBALANCE_CAL_CND, strDummy))
			{
				CList<double, double>* pCommonCalCondList = pSourceInfo->GetCommonCalConditionList();
				if( !pCommonCalCondList) break;
				double dPrePriod = pCommonCalCondList->RemoveHead();
				double dBasePriod = pCommonCalCondList->RemoveHead();
				pCommonCalCondList->AddHead( dPrePriod);
				pCommonCalCondList->AddHead( dBasePriod);
			}
			break;

		// 6. Check ADX
		case E_V3_21_ADX:
			if( m_mapVersionUpOperation.Lookup( _V3_21_ADX_CAL_CND, strDummy))
			{
				CList<double, double>* pCommonCalCondList = pSourceInfo->GetCommonCalConditionList();
				if( !pCommonCalCondList) break;
				if( pCommonCalCondList->GetCount() < 2) break;
				pCommonCalCondList->RemoveTail();
			}
			break;

		// 7. Check ATR
		case E_V3_21_ATR:
			if( m_mapVersionUpOperation.Lookup( _V3_21_ATR_CAL_CND, strDummy))
				AddSignalCalculateCondition( pTargetInfo, pSourceInfo);
			break;

		// 8. Check ATR
		case E_V3_21_BAND_PERCENT_B:
			if( m_mapVersionUpOperation.Lookup( _V3_21_BAND_PERCENT_B_CAL_CND, strDummy))
				AddSignalCalculateCondition( pTargetInfo, pSourceInfo);
			break;

		// 9. Check DPO
		case E_V3_21_DPO:
			if( m_mapVersionUpOperation.Lookup( _V3_21_DPO_CAL_CND, strDummy))
				AddSignalCalculateCondition( pTargetInfo, pSourceInfo);
			if( m_mapVersionUpOperation.Lookup( _V3_21_DPO_COLOR, strDummy))
			{
				// 9.2.1 Add Signal Graph
				CSubGraphPartData *pSubGraphPartData = pTargetInfo->GetSubGraphPartData( 1);
				if( !pSubGraphPartData) break;
				pSourceInfo->AddSubGraphPartDataWithCopy( pSubGraphPartData);
				// 9.2.2 Set Signal color with DPO Color
				pSourceInfo->SetSubGraphColor( 1, *pSourceInfo->GetSubGraphColor_PointerType( 0));
			}
			break;

		// 10. Check Envelope
		case E_V3_21_ENVELOPE:
			if( m_mapVersionUpOperation.Lookup( _V3_21_ENVELOPE_CAL_CND, strDummy))
			{
				CList<double, double>* pCommonCalCondList = pSourceInfo->GetCommonCalConditionList();
				if( !pCommonCalCondList) break;
				if( pCommonCalCondList->GetCount() < 1) break;
				pCommonCalCondList->AddTail( pCommonCalCondList->GetTail());
			}
			break;

		// 11. Check LRS
		case E_V3_21_LRS:
			if( m_mapVersionUpOperation.Lookup( _V3_21_LRS_CAL_CND, strDummy))
				AddSignalCalculateCondition( pTargetInfo, pSourceInfo);
			break;

		// 12. Check MAC
		case E_V3_21_MAC:
			if( m_mapVersionUpOperation.Lookup( _V3_21_MAC_CAL_CND, strDummy))
			{
				CList<double, double>* pSourceCalCndLst = pSourceInfo->GetCommonCalConditionList();
				CList<double, double>* pTargetCalCndLst = pTargetInfo->GetCommonCalConditionList();
				if( !pSourceCalCndLst || !pTargetCalCndLst) break;
				if( pTargetCalCndLst->GetCount() < 1) break;
				POSITION posCalCnd = pTargetCalCndLst->GetHeadPosition();
				pTargetCalCndLst->GetNext( posCalCnd);
				while( posCalCnd) pSourceCalCndLst->AddTail( pTargetCalCndLst->GetNext( posCalCnd));
			}
			break;

		// 13. Check MACD
		case E_V3_21_MACD:
			if( m_mapVersionUpOperation.Lookup( _V3_21_MACD_CAL_CND, strDummy))
				RemoveOneCalculateCondition( pSourceInfo);
			break;

		// 14. Check Standard Deviation.
		case E_V3_21_STANDARD_DEVIATION:
			if( m_mapVersionUpOperation.Lookup( _V3_21_STANDARD_DEVIATION_CAL_CND, strDummy))
				RemoveOneCalculateCondition( pSourceInfo);
			break;

		// 15. Check ZigZag.
		case E_V3_21_ZIG_ZAG:
			if( m_mapVersionUpOperation.Lookup( _V3_21_ZIG_ZAG_CAL_CND, strDummy))
				RemoveOneCalculateCondition( pSourceInfo);
			break;
	}

	// (2006/5/27 - Seung-Won, Bae) Do Next Version's Preparing
	return CUserVer5Dot00TypeIndiListBuilder::VersionUpOldInfo(pTargetInfo, pSourceInfo);
}

// (2006/5/30 - Seung-Won, Bae) Price and Volume MA's Calculate Condition Processing Routine (to Sub Calculate Condition)
void CUserVer3Dot21TypeIndiListBuilder::SetMAComCalToSubCal( CIndicatorInfo* pSourceInfo)
{
	// 1.5.1 Get Common Calculate Condition.
	CList<double, double>* pCommonCalCondList = pSourceInfo->GetCommonCalConditionList();
	if( !pCommonCalCondList) return;
	if( pCommonCalCondList->GetCount() < 2) return;	// MA Period and Type.

	// 1.5.2 Get MA Calculate Type
	double dMAType = pCommonCalCondList->GetTail();

	// 1.5.3 Get MA Count
	int nMACount = pCommonCalCondList->GetCount() - 1;

	// 1.5.4 Set Sub Calculate Condition.
	POSITION posCommCond = pCommonCalCondList->GetHeadPosition();
	CList<double, double>* pSubCalCondList = NULL;
	for( int i = 0; i < nMACount; i++)
	{
		pSubCalCondList = pSourceInfo->GetCalConditionList( i);
		if( !pSubCalCondList) return;

		while( !pSubCalCondList->IsEmpty()) pSubCalCondList->RemoveTail();
		pSubCalCondList->AddTail( pCommonCalCondList->GetNext( posCommCond));
		pSubCalCondList->AddTail( dMAType);
	}
}

// (2006/5/31 - Seung-Won, Bae) Add signal cal_cnd.
void CUserVer3Dot21TypeIndiListBuilder::AddSignalCalculateCondition( CIndicatorInfo* pTargetInfo, CIndicatorInfo* pSourceInfo)
{
	CList<double, double>* pSourceCalCndLst = pSourceInfo->GetCommonCalConditionList();
	CList<double, double>* pTargetCalCndLst = pTargetInfo->GetCommonCalConditionList();
	if( !pSourceCalCndLst || !pTargetCalCndLst) return;
	if( pTargetCalCndLst->GetCount() < 1) return;
	pSourceCalCndLst->AddTail( pTargetCalCndLst->GetTail());
}

// (2006/5/31 - Seung-Won, Bae) Add remove MA-Type cal_cnd.
void CUserVer3Dot21TypeIndiListBuilder::RemoveOneCalculateCondition( CIndicatorInfo* pSourceInfo)
{
	CList<double, double>* pCommonCalCondList = pSourceInfo->GetCommonCalConditionList();
	if( !pCommonCalCondList) return;
	if( pCommonCalCondList->GetCount() < 1) return;
	pCommonCalCondList->RemoveTail();
}

////////////////////////////////////////////////
// class CUserVer5Dot00TypeIndiListBuilder
CUserVer5Dot00TypeIndiListBuilder::CUserVer5Dot00TypeIndiListBuilder(CDefaultIndicatorList* pDefaultIndicatorList, HWND p_hOcxWnd):
		CUserVer5Dot10TypeIndiListBuilder(pDefaultIndicatorList, p_hOcxWnd)
{
}

void CUserVer5Dot00TypeIndiListBuilder::RemoveOneCalculateCondition( CIndicatorInfo* pSourceInfo)
{
	CList<double, double>* pCommonCalCondList = pSourceInfo->GetCommonCalConditionList();
	if( !pCommonCalCondList) return;
	if( pCommonCalCondList->GetCount() < 1) return;
	pCommonCalCondList->RemoveTail();
}

void CUserVer5Dot00TypeIndiListBuilder::RegisterPrevOperation( void)
{
	m_mapVersionUpOperation.SetAt( _V5_00_PANDF_CAL_CND,				_MTEXT( C2_P_AND_F));
	m_mapVersionUpOperation.SetAt( _V5_00_PANDF_DARW_CND,				_MTEXT( C2_P_AND_F));

	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_P_AND_F),					E_V5_00_PANDF);

	CUserVer5Dot10TypeIndiListBuilder::RegisterPrevOperation();
}

BOOL CUserVer5Dot00TypeIndiListBuilder::VersionUpOldInfo( CIndicatorInfo* pTargetInfo, CIndicatorInfo* pSourceInfo)
{
	// 0. Check Target
	WORD wIndicatorID;
	CString strDummy;
	CString strIndicatorName = pTargetInfo->GetIndicatorName();
	if( m_mapVersionUpIndicators.Lookup( strIndicatorName, wIndicatorID)) switch( wIndicatorID)
	{
		// 1. Check Price Chart
		case E_V5_00_PANDF:
		{
			// 1.1 Update 'graphdatadrawing'
			if( m_mapVersionUpOperation.Lookup( _V5_00_PANDF_CAL_CND, strDummy))
			{
				CList<double, double>* pSourceCalCndLst = pSourceInfo->GetCommonCalConditionList();
				CList<double, double>* pTargetCalCndLst = pTargetInfo->GetCommonCalConditionList();
				if( !pSourceCalCndLst || !pTargetCalCndLst) break;
				if( pTargetCalCndLst->GetCount() < 1) break;
				pSourceCalCndLst->AddTail( pTargetCalCndLst->GetTail());
			}
			// 1.2 Update 'datadrawing'
			if( m_mapVersionUpOperation.Lookup( _V5_00_PANDF_DARW_CND, strDummy))
			{
				CList<double, double>* pDrawCondList = pSourceInfo->GetDrawConditionList();
				CList<double, double>* pTargetDrawCondLis = pTargetInfo->GetDrawConditionList();
				//if( !pSourceCalCndLst || !pTargetCalCndLst) return;
				if( pTargetDrawCondLis->GetCount() < 1) break;
				pDrawCondList->AddTail( pTargetDrawCondLis->GetTail());
			}
			break;
		}
		break;
	}

	// (2006/5/27 - Seung-Won, Bae) Do Next Version's Preparing
	return CUserVer5Dot10TypeIndiListBuilder::VersionUpOldInfo(pTargetInfo, pSourceInfo);
}

// (2006/10/13 - Seung-hwan, Ahn) Version Up for Each version and Call Next Version.
// class CUserVer5Dot10TypeIndiListBuilder
CUserVer5Dot10TypeIndiListBuilder::CUserVer5Dot10TypeIndiListBuilder(CDefaultIndicatorList* pDefaultIndicatorList, HWND p_hOcxWnd):
		CUserVer5Dot20TypeIndiListBuilder(pDefaultIndicatorList, p_hOcxWnd)
{
}

void CUserVer5Dot10TypeIndiListBuilder::RegisterPrevOperation( void)
{
	m_mapVersionUpOperation.SetAt( _V5_10_VOLUME_FOR_SALE_CAL_CND,		_MTEXT( C2_OVERHANGING_SUPPLY));

	m_mapVersionUpIndicators.SetAt( _MTEXT( C2_OVERHANGING_SUPPLY),	E_V5_10_VOLUME_FOR_SALE);


}

BOOL CUserVer5Dot10TypeIndiListBuilder::VersionUpOldInfo( CIndicatorInfo* pTargetInfo, CIndicatorInfo* pSourceInfo)
{
	// 0. Check Target
	WORD wIndicatorID;
	CString strDummy;
	CString strIndicatorName = pTargetInfo->GetIndicatorName();
	if( m_mapVersionUpIndicators.Lookup( strIndicatorName, wIndicatorID)) switch( wIndicatorID)
	{
		// 1. Check Price Chart
		case E_V5_10_VOLUME_FOR_SALE:
		{
			// 1.1 Update 'graphdatadrawing'
			if( m_mapVersionUpOperation.Lookup( _V5_10_VOLUME_FOR_SALE_CAL_CND, strDummy))
			{
				CList<double, double>* pSourceCalCndLst = pSourceInfo->GetCommonCalConditionList();
				CList<double, double>* pTargetCalCndLst = pTargetInfo->GetCommonCalConditionList();
				if( !pSourceCalCndLst || !pTargetCalCndLst) break;
				if( pTargetCalCndLst->GetCount() < 1) break;
				pSourceCalCndLst->AddTail( pTargetCalCndLst->GetTail());
			}
			break;
		}
		break;
	}

	return CUserVer5Dot20TypeIndiListBuilder::VersionUpOldInfo(pTargetInfo, pSourceInfo);
}


////////////////////////////////////////////////
// class CUserVer5Dot20TypeIndiListBuilder
CUserVer5Dot20TypeIndiListBuilder::CUserVer5Dot20TypeIndiListBuilder(CDefaultIndicatorList* pDefaultIndicatorList, HWND p_hOcxWnd):
		CNotDefaultUserTypeIndiListBuilder(pDefaultIndicatorList, p_hOcxWnd)
{
}
void CUserVer5Dot20TypeIndiListBuilder::RegisterPrevOperation( void)
{
	m_mapVersionUpOperation.SetAt(	_V5_20_BUY_SELL_OVERHANGING_SUPPLY_DRAW_CND,	_MTEXT( C2_BUY_SELL_OVERHANGING_SUPPLY));
	m_mapVersionUpIndicators.SetAt(	_MTEXT( C2_BUY_SELL_OVERHANGING_SUPPLY),		E_V5_20_BUY_SELL_OVERHANGING_SUPPLY);

	CNotDefaultUserTypeIndiListBuilder::RegisterPrevOperation();
}

BOOL CUserVer5Dot20TypeIndiListBuilder::VersionUpOldInfo( CIndicatorInfo* pTargetInfo, CIndicatorInfo* pSourceInfo)
{
	// 0. Check Target
	WORD wIndicatorID;
	CString strDummy;
	CString strIndicatorName = pTargetInfo->GetIndicatorName();
	if( m_mapVersionUpIndicators.Lookup( strIndicatorName, wIndicatorID)) switch( wIndicatorID)
	{
		// 1. Check Price Chart
		case E_V5_20_BUY_SELL_OVERHANGING_SUPPLY:
		{
			// 1.1 Update 'graphdatadrawing'
			if( m_mapVersionUpOperation.Lookup( _V5_20_BUY_SELL_OVERHANGING_SUPPLY_DRAW_CND, strDummy))
			{
				CList<double, double>* pSourceDrawCndLst = pSourceInfo->GetDrawConditionList();
				CList<double, double>* pTargetDrawCndLst = pTargetInfo->GetDrawConditionList();
				if( !pSourceDrawCndLst || !pTargetDrawCndLst) break;
				if( pTargetDrawCndLst->GetCount() < 1) break;
				pSourceDrawCndLst->AddTail( pTargetDrawCndLst->GetTail());
			}
			break;
		}
		break;
	}

	// (2006/5/27 - Seung-Won, Bae) Do Next Version's Preparing
	return CNotDefaultUserTypeIndiListBuilder::VersionUpOldInfo(pTargetInfo, pSourceInfo);
}


// (2006/10/13 - Seung-hwan, Ahn) Version Up for Each version and Call Next Version.

///////////////////////////////////////////////////////////////////////////////
// class CDefaultIndicatorInfoBuilder -> indicator info (default 형태)

const int CDefaultIndicatorInfoBuilder::nLen_GROUPNAME				= strlen( _GROUPNAME);
const int CDefaultIndicatorInfoBuilder::nLen_GRAPHNAME				= strlen( _GRAPHNAME);					// (2006/11/29 - Seung-Won, Bae) Support Graph Name for Binding with Indicator Info.
const int CDefaultIndicatorInfoBuilder::nLen_GRAPHRQ				= strlen( _GRAPHRQ);					// 복수종목 저장 : 복수종목 - ojtaso (20070514)
const int CDefaultIndicatorInfoBuilder::nLen_CAL_CND				= strlen( _CAL_CND);
const int CDefaultIndicatorInfoBuilder::nLen_DRAW_CND				= strlen( _DRAW_CND);
const int CDefaultIndicatorInfoBuilder::nLen_BASELINE				= strlen( _BASELINE);
const int CDefaultIndicatorInfoBuilder::nLen_PACKET					= strlen( _PACKET);
const int CDefaultIndicatorInfoBuilder::nLen_SIGNAL					= strlen( _SIGNAL);
const int CDefaultIndicatorInfoBuilder::nLen_GRAPHDATADRAWING		= strlen( _GRAPHDATADRAWING);
const int CDefaultIndicatorInfoBuilder::nLen_MINMAXDATADRAWING		= strlen( _MINMAXDATADRAWING);
const int CDefaultIndicatorInfoBuilder::nLen_SUBGRAPH_START			= strlen( _SUBGRAPH_START);
const int CDefaultIndicatorInfoBuilder::nLen_SUBGRAPH_END			= strlen( _SUBGRAPH_END);
const int CDefaultIndicatorInfoBuilder::nLen_SUB_PACKET				= strlen( _SUB_PACKET);
const int CDefaultIndicatorInfoBuilder::nLen_SUB_CAL_CND			= strlen( _SUB_CAL_CND);
const int CDefaultIndicatorInfoBuilder::nLen_SUBGRAPH_NAME			= strlen( _SUBGRAPH_NAME);
const int CDefaultIndicatorInfoBuilder::nLen_SUBGRAPH_TITLE			= strlen( _SUBGRAPH_TITLE);
const int CDefaultIndicatorInfoBuilder::nLen_SUBGRAPH_HIDE			= strlen( _SUBGRAPH_HIDE);
const int CDefaultIndicatorInfoBuilder::nLen_SUBGRAPH_COLOR			= strlen( _SUBGRAPH_COLOR);
const int CDefaultIndicatorInfoBuilder::nLen_SUBGRAPH_LINETHICKNESS	= strlen( _SUBGRAPH_LINETHICKNESS);
const int CDefaultIndicatorInfoBuilder::nLen_EXPLANATION			= strlen( _EXPLANATION);

bool CDefaultIndicatorInfoBuilder::FileDataBuild( CIndicatorInfo *p_pIndicatorInfo, char *&p_szDataString, const char *p_szName)
{
	if( p_pIndicatorInfo == NULL || !p_szDataString) return false;
	if( !*p_szDataString) return false;

	// group name/graph(subGraph) 관련 내용들.
	CGraphPartData *pGraphPartData = p_pIndicatorInfo->GetGraphPartData();
	if( !pGraphPartData) return false;

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	if( !FileDataBuild_Graph( p_szDataString, p_pIndicatorInfo, *pGraphPartData, p_szName)) return false;

	return FileDataBuild_DoubleList( p_pIndicatorInfo, p_szDataString);
}

// public =====================================================================
// [가격차트]
// groupName=기본지표
// cal_cnd=1
// packet=시가,고가,저가,종가,
// signal=1
// graphdatadrawing=1;1;1;1;0;
// <SubGraph>
// name=가격
// title=가격
// type=봉형;캔들차트
// color=218,46,46;218,46,46;46,46,218;46,46,218;255,0,255;255,0,255;1;0;
// <SubGraphEnd>

// -> graph(subGraph) 관련 부분.
// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
bool CDefaultIndicatorInfoBuilder::FileDataBuild_Graph( char *&p_szDataString, CIndicatorInfo* p_pIndicatorInfo, CGraphPartData& graphPartData, const char *p_szName)
{
	graphPartData.Initialize();

	// groupName 관련 부분
	SetGroupData( p_szDataString, graphPartData);

	// graphData 관련 부분
	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	if( !SetGraphData( p_szDataString, p_pIndicatorInfo, graphPartData.GetGraphData(), p_szName))
	{
		graphPartData.DeleteAllSubGraph();
		return false;
	}

	// subGraph 관련 부분
	return AddTail_SubGraphData( p_szDataString, graphPartData);
}

// groupName 관련 부분.
void CDefaultIndicatorInfoBuilder::SetGroupData( char *&p_szDataString, CGraphPartData& graphPartData)
{
	char *szStart = strstr( p_szDataString, _GROUPNAME);
	if( !szStart) return;
	szStart += nLen_GROUPNAME;

	char *szEnd = strstr( szStart, "\r\n");
	if( szEnd)
	{
		while( *( szEnd - 1) == ' ') szEnd--;
		*szEnd = '\0';
	}
	else
	{
		char *szEnd2 = szStart + strlen( szStart);
		while( *( szEnd2 - 1) == ' ') szEnd2--;
		if( *szEnd2)
		{
			*szEnd2 = '\0';
			szEnd = szEnd2;
		}
	}

	graphPartData.SetGroupName( _MTOM7( szStart));
	if( szEnd) *szEnd = ' ';									// repare for Next Item Search
}

// graphData 관련 부분.
// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
bool CDefaultIndicatorInfoBuilder::SetGraphData( char *&p_szDataString, CIndicatorInfo* p_pIndicatorInfo, CGraphData& graphData, const char *p_szName)
{
	graphData.ClearData();

	// 함수명
	const char *szName = NULL;
	if( p_szName) szName = p_szName;
	else szName = _MTOM2( GetItemName( p_szDataString));
	if( !szName) return false;
	if( !*szName) return false;

	// (2006/11/22 - Seung-Won, Bae) the Indicator Name of CGraphData can be setted in Only CDefaultIndicatorInfoBuilder
	CGraphDataBase *pGraphDataBase = ( CGraphDataBase *)&graphData;
	pGraphDataBase->SetIndicatorName( szName);

	// (2006/11/29 - Seung-Won, Bae) Support Graph Name for Binding with Indicator Info.
	// Graph Name
	char *szStart = strstr( p_szDataString, _GRAPHNAME);
	char *szEnd = NULL;
	if( szStart)
	{
		szStart += nLen_GRAPHNAME;

		szEnd = strstr( szStart, "\r\n");
		if( szEnd)
		{
			while( *( szEnd - 1) == ' ') szEnd--;
			*szEnd = '\0';
		}

		pGraphDataBase->SetGraphName( _MTOM2( szStart));	// (2008/3/3 - Seung-Won, Bae) It can be Indicator Name of Old version.
		if( szEnd) *szEnd = ' ';							// repare for Next Item Search
	}
	if( graphData.GetGraphName().IsEmpty()) pGraphDataBase->SetGraphName( graphData.GetIndicatorName());

	// 복수종목 불러오기 : 복수종목 - ojtaso (20070514)
	szStart = strstr( p_szDataString, _GRAPHRQ);
	szEnd = NULL;
	if( szStart)
	{
		szStart += nLen_GRAPHRQ;

		szEnd = strstr( szStart, "\r\n");
		if( szEnd)
		{
			while( *( szEnd - 1) == ' ') szEnd--;
			*szEnd = '\0';
		}

		// 지표 파일저장때 불필요 지표 저장되는 오류 수정 - ojtaso (20080310)
		if( !szStart) return false;
		if( !*szStart) return false;

		p_pIndicatorInfo->SetLoadedRQ(szStart);
		if( szEnd) *szEnd = ' ';							// repare for Next Item Search
	}
	
	// packet 이름들
	szStart = strstr( p_szDataString, _PACKET);
	szEnd = NULL;
	if( szStart)
	{
		szStart += nLen_PACKET;

		szEnd = strstr( szStart, "\r\n");
		if( szEnd)
		{
			while( *( szEnd - 1) == ' ') szEnd--;
			*szEnd = '\0';
		}

		if( strchr( szStart, '('))	graphData.SetPacketNames( _MTOMS( szStart, "(+)/23,", 0));
		else						graphData.SetPacketNames( _MTOMS( szStart, ",", 0));
		if( szEnd) *szEnd = ' ';							// repare for Next Item Search
	}

	// signal 여부 -> <SIGNAL>이 있는 경우는 무조건 signal 사용함.
	szStart = strstr( p_szDataString, _SIGNAL);
	szEnd = NULL;
	if( szStart)
	{
		szStart += nLen_SIGNAL;

		szEnd = strstr( szStart, "\r\n");
		if( szEnd)
		{
			while( *( szEnd - 1) == ' ') szEnd--;
			*szEnd = '\0';
		}

		graphData.SetSignalData( true, atoi( szStart) == 1);
		if( szEnd) *szEnd = ' ';							// repare for Next Item Search
	}

	// graphData drawing 여부
	szStart = strstr( p_szDataString, _GRAPHDATADRAWING);
	szEnd = NULL;
	if( szStart)
	{
		szStart += nLen_GRAPHDATADRAWING;

		szEnd = strstr( szStart, "\r\n");
		if( szEnd)
		{
			while( *( szEnd - 1) == ' ') szEnd--;
			*szEnd = '\0';
		}

		graphData.SetGraphDataDrawingData( CGraphDataDrawingData( szStart));
		if( szEnd) *szEnd = ' ';							// repare for Next Item Search
	}

	// viewMinMax drawing 여부
/*
	szStart = strstr( p_szDataString, _MINMAXDATADRAWING);
	szEnd = NULL;
	if( szStart)
	{
		szStart += nLen_MINMAXDATADRAWING;

		szEnd = strstr( szStart, "\r\n");
		if( szEnd)
		{
			while( *( szEnd - 1) == ' ') szEnd--;
			*szEnd = '\0';
		}
		
		graphData.SetViewMinMaxDrawingData( CViewMinMaxDrawingData( szStart));
		if( szEnd) *szEnd = ' ';							// repare for Next Item Search
	}
*/

	return true;
}

// subGraph 관련 부분
bool CDefaultIndicatorInfoBuilder::AddTail_SubGraphData( char *&p_szDataString, CGraphPartData& graphPartData)
{
	graphPartData.DeleteAllSubGraph();

	char *szStart = strstr( p_szDataString, _SUBGRAPH_START);
	char *szEnd = NULL;
	while( szStart)
	{
		szStart += nLen_SUBGRAPH_START;

		szEnd = strstr( szStart, _SUBGRAPH_END);
		if( szEnd) *szEnd = '\0';
		graphPartData.AddTail( MakeSubGraphPartData( szStart, graphPartData.GetGraphData().GetIndicatorName()));
		if( szEnd)
		{
			*szEnd = *( const char *)_SUBGRAPH_END;
			szEnd += nLen_SUBGRAPH_END;

			szStart = strstr( szEnd, _SUBGRAPH_START);
		}
		else szStart = NULL;
	}

	return (graphPartData.GetSubGraphPartDataCount() > 0);
}

// private ====================================================================
CSubGraphPartData* CDefaultIndicatorInfoBuilder::MakeSubGraphPartData( char *&p_szDataString, const char *p_szIndicatorName)
{
	if( !p_szDataString) return NULL;
	if( !*p_szDataString) return NULL;

	CSubGraphPartData* pNewSubGraphPartData = new CSubGraphPartData();

	// subGraphData 부분
	if( !SetSubGraphData( p_szDataString, pNewSubGraphPartData->GetSubGraphData(), p_szIndicatorName))
	{
		delete pNewSubGraphPartData;
		return NULL;
	}

	// subGraph calPacketNames 부분
	SetSubGraphPacketNames( p_szDataString, pNewSubGraphPartData);
	// subGraph condition 부분
	SetSubGraphConditionData( p_szDataString, pNewSubGraphPartData->GetConditionList());
	return pNewSubGraphPartData;
}

//-> subGraph 관련
bool CDefaultIndicatorInfoBuilder::SetSubGraphData( char *&p_szDataString, CSubGraphData& subGraphData, const char *p_szIndicatorName)
{
	// graph type가 올바르지 않은 경우에는 만들지 않는다.
	if( !SetSubGraphType( p_szDataString, subGraphData)) return false;

	SetSubGraphNameAndTitle( p_szDataString, subGraphData, p_szIndicatorName);
	SetSubGraphHiding( p_szDataString, subGraphData);
	SetSubGraphColorAndFill( p_szDataString, subGraphData);
	SetSubGraphThickness( p_szDataString, subGraphData);
	return true;
}

// -> subGraph type
bool CDefaultIndicatorInfoBuilder::SetSubGraphType( char *&p_szDataString, CSubGraphData& subGraphData)
{
	// graph type/style/drawStyle
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	int nType(0), nStyle(0), nDrawStyle(0), nPaintStyle(1), nBaseLineStyle(0);
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	if( !GetSubGraphTypeAndStyle( p_szDataString, nType, nStyle, nDrawStyle, nPaintStyle, nBaseLineStyle)) return false;

	subGraphData.SetType(CGraphBaseData::GRAPHTYPE(nType));
	subGraphData.SetStyle(nStyle);
	subGraphData.SetDrawStyle(nDrawStyle);
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	subGraphData.SetPaintStyle(nPaintStyle);
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	subGraphData.SetBaseLineStyle(nBaseLineStyle);
	return true;
}

// -> subGraph Name/Title
void CDefaultIndicatorInfoBuilder::SetSubGraphNameAndTitle( char *&p_szDataString, CSubGraphData& subGraphData, const char *p_szIndicatorName)
{
	char *szStart = strstr( p_szDataString, _SUBGRAPH_NAME);
	if( !szStart) return;
	szStart += nLen_SUBGRAPH_NAME;

	char *szEnd = strstr( szStart, "\r\n");
	if( szEnd)
	{
		while( *( szEnd - 1) == ' ' || *( szEnd - 1) == ';') szEnd--;
		*szEnd = '\0';
	}

	subGraphData.SetName( _MTOM3( szStart));
	if( szEnd) *szEnd = ' ';							// repare for Next Item Search

	char *szStart2 = strstr( p_szDataString, _SUBGRAPH_TITLE);
	if( szStart2)
	{
		szStart2 += nLen_SUBGRAPH_TITLE;

		char *szEnd = strstr( szStart2, "\r\n");
		if( szEnd)
		{
			while( *( szEnd - 1) == ' ' || *( szEnd - 1) == ';') szEnd--;
			*szEnd = '\0';
		}

		if( g_iMetaTable.IsTitleIncludeCondition( p_szIndicatorName))
				subGraphData.SetTitle( szStart2);
		else	subGraphData.SetTitle( _STOM1( szStart2));

		if( szEnd) *szEnd = ' ';							// repare for Next Item Search
	}
	else
	{
		if( szEnd) *szEnd = '\0';							// repare for Next Item Search

		if( g_iMetaTable.IsTitleIncludeCondition( p_szIndicatorName))
				subGraphData.SetTitle( szStart);
		else	subGraphData.SetTitle( _STOM1( szStart));
		
		if( szEnd) *szEnd = ' ';							// repare for Next Item Search
	}
}

// -> subGraph hide
void CDefaultIndicatorInfoBuilder::SetSubGraphHiding( char *&p_szDataString, CSubGraphData& subGraphData)
{
	subGraphData.SetHiding( false);

	char *szStart = strstr( p_szDataString, _SUBGRAPH_HIDE);
	if( !szStart) return;
	szStart += nLen_SUBGRAPH_HIDE;

	char *szEnd = strstr( szStart, "\r\n");
	if( szEnd)
	{
		while( *( szEnd - 1) == ' ') szEnd--;
		*szEnd = '\0';
	}

	subGraphData.SetHiding( ( atoi( szStart) == 1));
	if( szEnd) *szEnd = ' ';							// repare for Next Item Search
}

// -> subGraph color(6개)/fill(2개)
/*@Solomon
void CDefaultIndicatorInfoBuilder::SetSubGraphColorAndFill( char *&p_szDataString, CSubGraphData& subGraphData)
{
	char *szStart = strstr( p_szDataString, _SUBGRAPH_COLOR);
	if( !szStart) return;
	szStart += nLen_SUBGRAPH_COLOR;

	char *szEnd = strstr( szStart, "\r\n");
	if( szEnd)
	{
		while( *( szEnd - 1) == ' ') szEnd--;
		*szEnd = '\0';
	}

	// set Color
	char *szColon = NULL;
	char *szComma = NULL;
	COLORREF clrColor[ 6];
	int nColor = 0;
	int nRGB[ 3];
	int nRGBi = 0;
	while( szStart && nColor < 6) 
	{
		szColon = strchr( szStart, ';');
		if( szColon) *szColon = '\0';

		nRGBi = 0;
		while( szStart)
		{
			szComma = strchr( szStart, ',');
			if( szComma) *szComma = '\0';

			if( nRGBi < 3) nRGB[ nRGBi++] = atoi( szStart);
			
			if( szComma) *szComma = ',';
			szStart = szComma;
			if( szStart) szStart++;
		}
		for( ; nRGBi < 3; nRGBi++) nRGB[ nRGBi] = 0;

		clrColor[ nColor++] = RGB( nRGB[ 0], nRGB[ 1], nRGB[ 2]);

		if( szColon) *szColon = ';';
		szStart = szColon;
		if( szStart)
		{
			szStart++;
			if( !*szStart) szStart = NULL;
		}
	}
	for( ; nColor < 6; nColor++) if( 0 < nColor) clrColor[ nColor] = clrColor[ nColor - 1];

	CGraphColor &graphColor = subGraphData.GetColor();
	graphColor.SetColor( clrColor[ 0], clrColor[ 1], clrColor[ 2], clrColor[ 3], clrColor[ 4], clrColor[ 5]);

	// set Fill
	if( szStart)
	{
		szColon = strchr( szStart, ';');
		if( szColon) *szColon = '\0';
		graphColor.SetUpGraphFill( atoi( szStart) == 1);
		if( szColon) *szColon = ';';
		szStart = szColon;
		if( szStart)
		{
			szStart++;
			if( !*szStart) szStart = NULL;
		}
	}
	if( szStart)
	{
		szColon = strchr( szStart, ';');
		if( szColon) *szColon = '\0';
		graphColor.SetDownGraphFill( atoi( szStart) == 1);
		if( szColon) *szColon = ';';
		szStart = szColon;
		if( szStart)
		{
			szStart++;
			if( !*szStart) szStart = NULL;
		}
	}

	if( szEnd) *szEnd = ' ';							// repare for Next Item Search
}
*/
void CDefaultIndicatorInfoBuilder::SetSubGraphColorAndFill( char *&p_szDataString, CSubGraphData& subGraphData)
{
	char *szStart = strstr( p_szDataString, _SUBGRAPH_COLOR);
	if( !szStart) return;
	szStart += nLen_SUBGRAPH_COLOR;

	char *szEnd = strstr( szStart, "\r\n");
	if( szEnd)
	{
		while( *( szEnd - 1) == ' ') szEnd--;
		*szEnd = '\0';
	}

	// set Color
	char *szColon = NULL;
	char *szComma = NULL;
	COLORREF clrColor[ 6];
	int nColor = 0;
	int nRGB[ 3];
	int nRGBi = 0;
	while( szStart && nColor < 6) 
	{
		szColon = strchr( szStart, ';');
		if( szColon) *szColon = '\0';

		nRGBi = 0;
		while( szStart)
		{
			szComma = strchr( szStart, ',');
			if( szComma) *szComma = '\0';

			if( nRGBi < 3) nRGB[ nRGBi++] = atoi( szStart);
			
			if( szComma) *szComma = ',';
			szStart = szComma;
			if( szStart) szStart++;
		}
		for( ; nRGBi < 3; nRGBi++) nRGB[ nRGBi] = 0;

		clrColor[ nColor++] = RGB( nRGB[ 0], nRGB[ 1], nRGB[ 2]);

		if( szColon) *szColon = ';';
		szStart = szColon;
		if( szStart)
		{
			szStart++;
			if( !*szStart) szStart = NULL;
		}
	}
	for( ; nColor < 6; nColor++) if( 0 < nColor) clrColor[ nColor] = clrColor[ nColor - 1];

	CGraphColor &graphColor = subGraphData.GetColor();
	graphColor.SetColor( clrColor[ 0], clrColor[ 1], clrColor[ 2], clrColor[ 3], clrColor[ 4], clrColor[ 5]);

	// set Fill
	if( szStart)
	{
		szColon = strchr( szStart, ';');
		if( szColon) *szColon = '\0';
		graphColor.SetUpGraphFill( atoi( szStart) == 1);
		if( szColon) *szColon = ';';
		szStart = szColon;
		if( szStart)
		{
			szStart++;
			if( !*szStart) szStart = NULL;
		}
	}
	if( szStart)
	{
		szColon = strchr( szStart, ';');
		if( szColon) *szColon = '\0';
		graphColor.SetDownGraphFill( atoi( szStart) == 1);
		if( szColon) *szColon = ';';
		szStart = szColon;
		if( szStart)
		{
			szStart++;
			if( !*szStart) szStart = NULL;
		}
	}

	// 상승, 양봉, 하락, 음봉 색상 분리 - ojtaso (20080707)
	if(!szStart || !::lstrcmp(szStart, "1,0,0;1,0,0;1,0,0;0,0,0;0;1;"))
	{
		if( szEnd) *szEnd = ' ';							// repare for Next Item Search
		return;
	}

	nColor = 0;
	while( szStart && nColor < 4) 
	{
		szColon = strchr( szStart, ';');
		if( szColon) *szColon = '\0';

		nRGBi = 0;
		while( szStart)
		{
			szComma = strchr( szStart, ',');
			if( szComma) *szComma = '\0';

			if( nRGBi < 3) nRGB[ nRGBi++] = atoi( szStart);
			
			if( szComma) *szComma = ',';
			szStart = szComma;
			if( szStart) szStart++;
		}
		for( ; nRGBi < 3; nRGBi++) nRGB[ nRGBi] = 0;

		clrColor[ nColor++] = RGB( nRGB[ 0], nRGB[ 1], nRGB[ 2]);

		if( szColon) *szColon = ';';
		szStart = szColon;
		if( szStart)
		{
			szStart++;
			if( !*szStart) szStart = NULL;
		}
	}
	for( ; nColor < 4; nColor++) if( 0 < nColor) clrColor[ nColor] = clrColor[ nColor - 1];
	{
		graphColor.SetColor7( clrColor[ 2]);
		graphColor.SetColor8( clrColor[ 3]);
		graphColor.SetColor9( clrColor[ 0]);
		graphColor.SetColor10( clrColor[ 1]);
	}

	// set Fill
	if( szStart)
	{
		szColon = strchr( szStart, ';');
		if( szColon) *szColon = '\0';
		graphColor.SetDownPositiveGraphFill( atoi( szStart) == 1);
		if( szColon) *szColon = ';';
		szStart = szColon;
		if( szStart)
		{
			szStart++;
			if( !*szStart) szStart = NULL;
		}
	}
	if( szStart)
	{
		szColon = strchr( szStart, ';');
		if( szColon) *szColon = '\0';
		graphColor.SetDownNegativeGraphFill( atoi( szStart) == 1);
		if( szColon) *szColon = ';';
		szStart = szColon;
		if( szStart)
		{
			szStart++;
			if( !*szStart) szStart = NULL;
		}
	}

	if( szEnd) *szEnd = ' ';							// repare for Next Item Search
}
// -> subGraph thickness/강조
void CDefaultIndicatorInfoBuilder::SetSubGraphThickness( char *&p_szDataString, CSubGraphData& subGraphData)
{
	char *szStart = strstr( p_szDataString, _SUBGRAPH_LINETHICKNESS);
	if( !szStart) return;
	szStart += nLen_SUBGRAPH_LINETHICKNESS;

	char *szEnd = strstr( szStart, "\r\n");
	if( szEnd)
	{
		while( *( szEnd - 1) == ' ') szEnd--;
		*szEnd = '\0';
	}

	char *szColon = NULL;
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	int nThickness[ 4];
	int nThicknessI = 0;
	while( szStart)
	{
		szColon = strchr( szStart, ';');
		if( szColon) *szColon = '\0';

		// 기준선 두께, 모양 설정 - ojtaso (20071213)
		if( nThicknessI < 3) nThickness[ nThicknessI] = atoi( szStart);
		else if( nThicknessI == 4) nThickness[ 3] = atoi(szStart);
			
		if( szColon) *szColon = ';';
		szStart = szColon;
		if( szStart)
		{
			szStart++;
			if( !*szStart) szStart = NULL;
		}

		// 기준선 두께, 모양 설정 - ojtaso (20071213)
		nThicknessI++;
	}
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	for( ; nThicknessI < 4; nThicknessI++) nThickness[ nThicknessI] = 1;

	subGraphData.GetPenThickness().SetThickness( nThickness[ 0], nThickness[ 1], nThickness[ 2]);
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	subGraphData.GetPenThickness().SetBaseLineThickness(nThickness[ 3]);

	if( szEnd) *szEnd = ' ';							// repare for Next Item Search
}

//-> subGraph의 calPacketName 관련
bool CDefaultIndicatorInfoBuilder::SetSubGraphPacketNames( char *&p_szDataString, CSubGraphPartData* pSubGraphPartData)
{
	if( pSubGraphPartData == NULL) return false;
	
	char *szStart = strstr( p_szDataString, _SUB_PACKET);
	if( !szStart) return false;
	szStart += nLen_SUB_PACKET;

	char *szEnd = strstr( szStart, "\r\n");
	if( szEnd)
	{
		while( *( szEnd - 1) == ' ') szEnd--;
		*szEnd = '\0';
	}

	// packet 이름들
	bool bResult = ( pSubGraphPartData->GetCalPacketNames() != szStart);
	if( bResult)
	{
		// (2008/6/10 - Seung-Won, Bae) Support combined packet.
		if( strchr( szStart, '('))	pSubGraphPartData->SetCalPacketNames( _MTOMS( szStart, "(+)/23,", 0));
		else						pSubGraphPartData->SetCalPacketNames( _MTOMS( szStart, ",", 0));
	}

	if( szEnd) *szEnd = ' ';							// repare for Next Item Search
	return bResult;
}

//-> subGraph의 doubleList 관련
bool CDefaultIndicatorInfoBuilder::SetSubGraphConditionData( char *&p_szDataString, CDoubleList& doubleList)
{
	return AddTail_DoubleList( p_szDataString, _SUB_CAL_CND, nLen_SUB_CAL_CND, doubleList);
}

// double list 관련 부분
bool CDefaultIndicatorInfoBuilder::AddTail_DoubleList( char *&p_szDataString, const char *p_szKey, int p_nLenKey, CDoubleList& doubleList)
{
	doubleList.RemoveAll();
	if( !p_szKey) return false;
	if( !*p_szKey) return false;

	char *szStart = strstr( p_szDataString, p_szKey);
	while( szStart)
	{
		szStart += p_nLenKey;

		char *szEnd = strstr( szStart, "\r\n");
		if( szEnd)
		{
			while( *( szEnd - 1) == ' ') szEnd--;
			*szEnd = '\0';
		}

		AddTail_DoubleData( szStart, doubleList);

		if( szEnd)
		{
			*szEnd = ' ';							// repare for Next Item Search
			szStart = strstr( szEnd, p_szKey);
		}
		else szStart = NULL;
	}

	return (doubleList.GetCount() > 0);
}

// double data 관련 부분
void CDefaultIndicatorInfoBuilder::AddTail_DoubleData( char *&p_szDataString, CDoubleList& doubleList)
{
	if( !p_szDataString) return;
	if( !*p_szDataString) return;

	// 설명 부분 삭제 (괄호(ex:...) 로 되어 있다.)
	char *szEnd = strstr( p_szDataString, _EXPLANATION);
	if( szEnd)
	{
		while( *( szEnd - 1) == ' ') szEnd--;
		*szEnd = '\0';
	}

	if( !*p_szDataString)
	{
		*szEnd = ' ';							// repare for Next Item Search
		return;
	}

	char *szStart = p_szDataString;
	char *szColon = NULL;
	while( szStart)
	{
		szColon = strchr( szStart, ';');
		if( szColon) *szColon = '\0';

		doubleList.AddTail( atof( szStart));

		if( szColon) *szColon = ';';

		szStart = szColon;
		if( szStart)
		{
			szStart++;
			if( !*szStart) szStart = NULL;
		}
	}

	if( szEnd) *szEnd = ' ';							// repare for Next Item Search
}

bool CDefaultIndicatorInfoBuilder::FileDataBuild_DoubleList( CIndicatorInfo *p_pIndicatorInfo, char *&p_szDataString)
{
	if( p_pIndicatorInfo == NULL || !p_szDataString) return false;
	if( !*p_szDataString) return false;

	char *szEnd = strstr( p_szDataString, _SUBGRAPH_START);
	if( szEnd) *szEnd = '\0';

	// 계산을 위해 필요한 조건값 list.
	FileDataBuild_CalCondition( p_szDataString, *p_pIndicatorInfo->GetCommonCalConditionList());
	// 그리기위해 필요한 조건값 list.
	FileDataBuild_DrawCondition( p_szDataString, *p_pIndicatorInfo->GetDrawConditionList());
	// 기준선 list.
	FileDataBuild_BaseLine( p_szDataString, *p_pIndicatorInfo->GetBaseLineList());

	if( szEnd) *szEnd = *( const char *)_SUBGRAPH_START;
	return true;
}

// -> 계산을 위해 필요한 조건값 list.
bool CDefaultIndicatorInfoBuilder::FileDataBuild_CalCondition( char *&p_szDataString, CDoubleList& calConditionList)
{
	return AddTail_DoubleList( p_szDataString, _CAL_CND, nLen_CAL_CND, calConditionList);
}

// -> 그리기 위해 필요한 조건값 list.
bool CDefaultIndicatorInfoBuilder::FileDataBuild_DrawCondition( char *&p_szDataString, CDoubleList& drawConditionList)
{
	return AddTail_DoubleList( p_szDataString, _DRAW_CND, nLen_DRAW_CND, drawConditionList);
}

// -> 기준선 list.
bool CDefaultIndicatorInfoBuilder::FileDataBuild_BaseLine( char *&p_szDataString, CDoubleList& baseLineList)
{
	return AddTail_DoubleList( p_szDataString, _BASELINE, nLen_BASELINE, baseLineList);
}

///////////////////////////////////////////////////////////////////////////////
// class CDefaultTypeIndiInfoBuilder -> indicator info default type

CDefaultTypeIndiInfoBuilder::CDefaultTypeIndiInfoBuilder(CGraphTypeAndStyleData* pGraphTypeAndStyleData, HWND p_hOcxWnd) : CDefaultIndicatorInfoBuilder( p_hOcxWnd)
{
	m_pGraphTypeAndStyleData = pGraphTypeAndStyleData;
}

const int CDefaultTypeIndiInfoBuilder::nLen_SUBGRAPH_TYPE = strlen( _SUBGRAPH_TYPE);

// subGraph type : "봉형;캔들차트;", "선형;일반식;실선;", "데이터표현형;하단식바형;데이터바형;45도 빗살식;"
// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
// 기준선 두께, 모양 설정 - ojtaso (20071213)
bool CDefaultTypeIndiInfoBuilder::GetSubGraphTypeAndStyle( char *&p_szDataString, int& nType, int& nStyle, int& nDrawStyle, int& nPaintStyle, int& nBaseLineStyle) const
{
	if( m_pGraphTypeAndStyleData == NULL) return false;

	char *szStart = strstr( p_szDataString, _SUBGRAPH_TYPE);
	if( !szStart) return false;
	szStart += nLen_SUBGRAPH_TYPE;

	char *szEnd = strstr( szStart, "\r\n");
	if( szEnd)
	{
		while( *( szEnd - 1) == ' ') szEnd--;
		*szEnd = '\0';
	}

	// graph type/style/drawStyle
	char *szType = szStart;
	char *szColon = strchr( szStart, ';');
	if( szColon)
	{
		*szColon = '\0';
		szStart = szColon + 1;
	}
	else szStart = NULL;

	char *szStyle = szStart;
	if( szStart)
	{
		szColon = strchr( szStart, ';');
		if( szColon)
		{
			*szColon = '\0';
			szStart = szColon + 1;
		}
		else szStart = NULL;
	}

	char *szDrawStyleName = szStart;
	if( szStart)
	{
		szColon = strchr( szStart, ';');
		if( szColon)
		{
			*szColon = '\0';
			szStart = szColon + 1;
		}
		else szStart = NULL;
	}

	char *szDrawStyle = szStart;
	if( szStart)
	{
		szColon = strchr( szStart, ';');
		if( szColon)
		{
			*szColon = '\0';
			szStart = szColon + 1;
		}
		else szStart = NULL;
	}

	if( szDrawStyle)	m_pGraphTypeAndStyleData->GetGraphTypeAndStyle( szType, szStyle, 
							szDrawStyleName, szDrawStyle, nType, nStyle, nDrawStyle);
	else				m_pGraphTypeAndStyleData->GetGraphTypeAndStyle( szType, szStyle, 
							szDrawStyleName, nType, nStyle, nDrawStyle);

	if( szStyle)			*( szStyle - 1)			= ';';
	if( szDrawStyleName)	*( szDrawStyleName - 1)	= ';';
	if( szDrawStyle)		*( szDrawStyle - 1)		= ';';
	if( szStart)			*( szStart - 1)			= ';';
	if( szEnd)				*szEnd = ' ';											// repare for Next Item Search
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// class CUserTypeIndiInfoBuilder -> indicator info user type

const int CUserTypeIndiInfoBuilder::nLen_SUBGRAPH_TYPE_INT = strlen( _SUBGRAPH_TYPE_INT);

// protected ==================================================================
// subGraph type : "0;0;0;"
// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
// 기준선 두께, 모양 설정 - ojtaso (20071213)
bool CUserTypeIndiInfoBuilder::GetSubGraphTypeAndStyle( char *&p_szDataString, int& nType, int& nStyle, int& nDrawStyle, int& nPaintStyle, int& nBaseLineStyle) const
{
	if( !p_szDataString) return false;
	if( !*p_szDataString) return false;

	char *szStart = strstr( p_szDataString, _SUBGRAPH_TYPE_INT);
	if( !szStart) return false;
	szStart += nLen_SUBGRAPH_TYPE_INT;

	char *szEnd = strstr( szStart, "\r\n");
	if( szEnd)
	{
		while( *( szEnd - 1) == ' ') szEnd--;
		*szEnd = '\0';
	}

	// graph type/style/drawStyle
	char *szType = szStart;
	char *szColon = strchr( szStart, ';');
	if( szColon)
	{
		*szColon = '\0';
		szStart = szColon + 1;
	}
	else szStart = NULL;
	nType = atoi( szType);

	char *szStyle = szStart;
	if( szStart)
	{
		szColon = strchr( szStart, ';');
		if( szColon)
		{
			*szColon = '\0';
			szStart = szColon + 1;
		}
		else szStart = NULL;
	}
	if( szStyle) nStyle = atoi( szStyle);
	else nStyle = 0;

	char *szDrawStyle = szStart;
	nDrawStyle = 0;
	// "봉형" 일 경우에는 style 이 drawStyle 이다!!
	if(CGraphBaseData::GRAPHTYPE(nType) == CGraphBaseData::Bong_Type) nDrawStyle = nStyle;
	else
	{
		if( szStart)
		{
			szColon = strchr( szStart, ';');
			if( szColon)
			{
				*szColon = '\0';
				szStart = szColon + 1;
			}
			else szStart = NULL;
		}
		if( szDrawStyle) nDrawStyle = atoi( szDrawStyle);
		else nDrawStyle = 0;
	}

	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	char *szPaint = szStart;
	if( szStart && ::strlen(szStart))
	{
		szColon = strchr( szStart, ';');
		if( szColon)
		{
			*szColon = '\0';
			szStart = szColon + 1;
		}
		else szStart = NULL;
	}
	if( szPaint && ::strlen(szPaint)) nPaintStyle = atoi( szPaint);
	else nPaintStyle = 1;

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	char *szBaseLine = szStart;
	if( szStart && ::strlen(szStart))
	{
		szColon = strchr( szStart, ';');
		if( szColon)
		{
			*szColon = '\0';
			szStart = szColon + 1;
		}
		else szStart = NULL;
	}
	if( szBaseLine && ::strlen(szBaseLine)) nBaseLineStyle = atoi( szBaseLine);
	else nBaseLineStyle = 0;

	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	if( szBaseLine)			*( szBaseLine - 1)		= ' ';
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	if( szPaint)			*( szPaint - 1)			= ' ';
	if( szStyle)			*( szStyle - 1)			= ' ';
	if( szDrawStyle)		*( szDrawStyle - 1)		= ' ';
	if( szStart)			*( szStart - 1)			= ' ';
	if( szEnd)				*szEnd					= ' ';		// repare for Next Item Search

	return true;
}

// public =====================================================================
// 저장할 file data.
// [가격차트]
// groupName=기본지표
// cal_cnd=1
// packet=시가,고가,저가,종가,
// signal=1
// graphdatadrawing=1;1;1;1;0;
// <SubGraph>
// name=가격
// title=가격
// type=봉형;캔들차트
// color=218,46,46;218,46,46;46,46,218;46,46,218;255,0,255;255,0,255;1;0;
// <SubGraphEnd>
void CUserTypeIndiInfoBuilder::GetWritingFileData( const CIndicatorInfo& indicatorInfo, CStringList &p_slDataString) const
{
	if( !IsWritingFileData( indicatorInfo.GetGraphPartData())) return;

	GetWritingFileData_IndicatorName( indicatorInfo.GetGraphPartData()->GetGraphData(), p_slDataString);
	
	// -> groupName
	GetWritingFileData_GroupName( indicatorInfo.GetGraphPartData(), p_slDataString);
	
	// -> 계산을 위해 필요한 조건값 list.
	GetWritingFileData_DoubleList( _CAL_CND, indicatorInfo.GetCommonCalConditionList(), p_slDataString);
	
	// -> 그리기 위해 필요한 조건값 list.
	GetWritingFileData_DoubleList( _DRAW_CND, indicatorInfo.GetDrawConditionList(), p_slDataString);
	
	// -> 기준선 list.
	GetWritingFileData_DoubleList( _BASELINE, indicatorInfo.GetBaseLineList(), p_slDataString);
	
	// -> graph 관련 부분.
	// 복수종목 저장 : 복수종목 - ojtaso (20070514)
	// 지표 파일저장때 불필요 지표 저장되는 오류 수정 - ojtaso (20080310)
	CGraph* lpGraph = indicatorInfo.GetGraph();
	// 특수차트 이후 이평이 0되는 버그 수정 - ojtaso (20080415)
	GetWritingFileData_Graph(lpGraph ? lpGraph->GetGraphRQ() : NULL, indicatorInfo.GetGraphPartData()->GetGraphData(), p_slDataString);
	
	// -> subGraph 관련 부분.
	GetWritingFileData_SubGraphPart( indicatorInfo.GetGraphPartData()->GetSubGraphPartDataList(), p_slDataString);

	p_slDataString.AddTail( "\r\n");
}

// private ====================================================================
// file에 저장할 data를 만들지의 여부
bool CUserTypeIndiInfoBuilder::IsWritingFileData(const CGraphPartData* pGraphPartData) const
{
	if( pGraphPartData == NULL || pGraphPartData->GetGraphData().GetIndicatorName().IsEmpty()) return false;
	return (pGraphPartData->GetSubGraphPartDataCount() > 0);
}

// ----------------------------------------------------------------------------
// -> indicator name 부분
void CUserTypeIndiInfoBuilder::GetWritingFileData_IndicatorName( const CGraphData& graphData, CStringList &p_slDataString) const
{
	p_slDataString.AddTail( _ITEMNAME_START);
	p_slDataString.AddTail( graphData.GetIndicatorName());
	p_slDataString.AddTail( _ITEMNAME_END);
	p_slDataString.AddTail( "\r\n");
}

void CUserTypeIndiInfoBuilder::GetWritingFileData_GroupName( const CGraphPartData* pGraphPartData, CStringList &p_slDataString) const
{
	if( pGraphPartData == NULL) return;

	CString strGroupName( pGraphPartData->GetGroupName());
	if( strGroupName.IsEmpty()) return;

	p_slDataString.AddTail( _GROUPNAME);
	p_slDataString.AddTail( strGroupName);
	p_slDataString.AddTail( "\r\n");
}

// -> 계산을 위해 필요한 조건값 list.
// -> 그리기 위해 필요한 조건값 list.
// -> 기준선 list.
void CUserTypeIndiInfoBuilder::GetWritingFileData_DoubleList( const char *p_szItemName, const CDoubleList* pDoubleList, CStringList &p_slDataString) const
{
	if( pDoubleList == NULL || pDoubleList->GetCount() <= 0) return;

	p_slDataString.AddTail( p_szItemName);

	POSITION pos = pDoubleList->GetHeadPosition();
	while(pos != NULL)
	{
		p_slDataString.AddTail( CString());
		CString &strData = p_slDataString.GetTail();
		double dData = pDoubleList->GetNext( pos);
		if( dData == ( int)dData) strData.Format( "%d;", ( int)dData);
		else
		{
			strData.Format( "%f", dData);
			strData.TrimRight( '0');
			p_slDataString.AddTail( ";");
		}
	}

	p_slDataString.AddTail( "\r\n");
}

// -> graph 관련 부분. : packetName, signal여부, graphData drawing여부
// 복수종목 저장 : 복수종목 - ojtaso (20070514)
void CUserTypeIndiInfoBuilder::GetWritingFileData_Graph(LPCTSTR lpszRQ, const CGraphData& graphData, CStringList &p_slDataString) const
{
	// (2006/11/29 - Seung-Won, Bae) Support Graph Name for Binding with Indicator Info.
	if( m_eIndicatorListType == EILT_MAP_DEFAULT || m_eIndicatorListType == EILT_USER)
	{
		p_slDataString.AddTail( _GRAPHNAME);
		p_slDataString.AddTail( graphData.GetGraphName());
		p_slDataString.AddTail( "\r\n");
	}

	// 특수차트 이후 이평이 0되는 버그 수정 - ojtaso (20080415)
	if(lpszRQ)
	{
		p_slDataString.AddTail( _GRAPHRQ);
		p_slDataString.AddTail( lpszRQ);
		p_slDataString.AddTail( "\r\n");
	}

	// packet 이름들
	p_slDataString.AddTail( _PACKET);
	p_slDataString.AddTail( graphData.GetPacketNames());
	p_slDataString.AddTail( "\r\n");

	// signal 사용여부:보임 (사용여부가 true 일때만 저장, "보임"여부만 저장)
	CSignalData &signalData = graphData.GetSignalData();
	if( signalData.IsUse())
	{
		p_slDataString.AddTail( _SIGNAL);

		p_slDataString.AddTail( CString());
		CString &strData = p_slDataString.GetTail();
		strData.Format( "%d", ( int)graphData.GetSignalData().IsShow());

		p_slDataString.AddTail( "\r\n");
	}

	// graphData drawing 여부 (하나라도 true가 있을 경우만 저장)
	CGraphDataDrawingData graphDataDrawingData = graphData.GetGraphDataDrawingData();
	if(!graphDataDrawingData.DoesAllDefaultData())
	{
		p_slDataString.AddTail( _GRAPHDATADRAWING);

		CString strGraphDataDrawingData;
		graphDataDrawingData.GetData( strGraphDataDrawingData);
		p_slDataString.AddTail( strGraphDataDrawingData);

		p_slDataString.AddTail( "\r\n");
	}
}

// -> subGraph 들 관련 부분.
void CUserTypeIndiInfoBuilder::GetWritingFileData_SubGraphPart( const CSubGraphPartDataList* pSubGraphDataList, CStringList &p_slDataString) const
{
	if( pSubGraphDataList == NULL || pSubGraphDataList->size() <= 0) return;

	CSubGraphPartDataList::const_iterator i;
	for(i = pSubGraphDataList->begin(); i != pSubGraphDataList->end(); ++i)
		GetWritingFileData_SubGraphPart( ( CSubGraphPartData *)*i, p_slDataString);
}

// -> subGraphPart
void CUserTypeIndiInfoBuilder::GetWritingFileData_SubGraphPart( const CSubGraphPartData* pSubGraphPartData, CStringList &p_slDataString) const
{
	if( pSubGraphPartData == NULL) return;

	p_slDataString.AddTail( _SUBGRAPH_START);
	// packet Name
	//-> subGraph 에 해당되는 계산을 위한 Packet Name
	CString strPacketNames( pSubGraphPartData->GetCalPacketNames());
	if( !strPacketNames.IsEmpty())
	{
		p_slDataString.AddTail( _SUB_PACKET);
		p_slDataString.AddTail( strPacketNames);
		p_slDataString.AddTail( "\r\n");
	}
	// condition
	GetWritingFileData_DoubleList( _SUB_CAL_CND, &pSubGraphPartData->GetConditionList(), p_slDataString);
	// subGraph
	GetWritingFileData_SubGraph( pSubGraphPartData->GetSubGraphData(), p_slDataString);
	
	p_slDataString.AddTail( _SUBGRAPH_END);
}


// -> subGraph 관련 부분.
void CUserTypeIndiInfoBuilder::GetWritingFileData_SubGraph( const CSubGraphData& subGraphData, CStringList &p_slDataString) const
{
	// subGraph type
	p_slDataString.AddTail( _SUBGRAPH_TYPE_INT);
	p_slDataString.AddTail( CString());
	CString &strData = p_slDataString.GetTail();
	// Singbar Graph 색상 비교기준 추가 - ojtaso (20071121)
	// 기준선 두께, 모양 설정 - ojtaso (20071213)
	strData.Format( "%d;%d;%d;%d;%d;", subGraphData.GetType(), subGraphData.GetStyle(), subGraphData.GetDrawStyle(), subGraphData.GetPaintStyle(), subGraphData.GetBaseLineDrawStyle());
	p_slDataString.AddTail( "\r\n");

	// subGraph Name/Title
	p_slDataString.AddTail( _SUBGRAPH_NAME);
	p_slDataString.AddTail( subGraphData.GetName());
	p_slDataString.AddTail( "\r\n");
	if(subGraphData.GetName() != subGraphData.GetTitle())
	{
		p_slDataString.AddTail( _SUBGRAPH_TITLE);
		p_slDataString.AddTail( subGraphData.GetTitle());
		p_slDataString.AddTail( "\r\n");
	}

	// subGraph hide
	if( subGraphData.GetHiding())
	{
		p_slDataString.AddTail( _SUBGRAPH_HIDE);
		p_slDataString.AddTail( CString());
		CString &strData = p_slDataString.GetTail();
		char *szHide = strData.GetBufferSetLength( 1);
		*szHide = ( subGraphData.GetHiding() ? '1' : '0');
		p_slDataString.AddTail( "\r\n");
	}
	
	// subGraph color/fill
	p_slDataString.AddTail( _SUBGRAPH_COLOR);
	p_slDataString.AddTail( CString());
	CString &strGraphColor = p_slDataString.GetTail();
	subGraphData.GetColor().GetStringData( strGraphColor);
	p_slDataString.AddTail( "\r\n");

	// subGraph thickness/강조
	CPenThickness &penThickness = subGraphData.GetPenThickness();
	if( !( penThickness == CPenThickness()))
	{
		p_slDataString.AddTail( _SUBGRAPH_LINETHICKNESS);
		p_slDataString.AddTail( CString());
		CString &strPenThickness = p_slDataString.GetTail();
		penThickness.GetStringData( strPenThickness);
		p_slDataString.AddTail( "\r\n");
	}
}
