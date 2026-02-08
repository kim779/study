// IndicatorDefaultData.cpp: implementation of the CIndicatorDefaultData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IndicatorDefaultData.h"

#include "../Include_Chart/DLL_Load/GraphTypeData.h"		// for CGraphTypeAndStyleData
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for _MTEXT()

#include "Conversion.h"
#include "GraphData.h"
#include "IndicatorList.h"
#include "IndicatorInfo.h"
#include "KTBChartCtl.h"		// for CKTBChartCtrl

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndicatorDefaultData::CIndicatorDefaultData( const CKTBChartCtrl *p_pChartOCX, const CScaleBaseData::SCALEGRIDTYPE eBaseGridType, 
	const CScaleBaseData::VERTSCALEPOSITION eVertScalePosition) :
		m_eBaseGridType(eBaseGridType), 
		m_eVertScalePosition(eVertScalePosition)
{
	// (2004.10.06, 배승원) 가격차트의 Graph Drawing Data를 지정받아 DefaultType에 따라 관리, 적용토록 한다.
	m_pChartOCX = p_pChartOCX;
}

void CIndicatorDefaultData::SetVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition)
{
	m_eVertScalePosition = ePosition;
}

// ----------------------------------------------------------------------------
// 지표를 객체로 만들기 위한 string.

// BlockColumn;\r\n
// Block;\r\n
// 사용할 Data;위치;grid타입;일자타입;시간타입;text타입;log;invert;2배block;\r\n
// VertScaleGroup;\r\n
// 사용할 Data;위치;grid타입;unit타입;\r\n
// Graph;\r\n
// index;가격차트;시가,고가,저자,종가;signal사용여부:signal보임;
//		최저가최고가등락률;최저가현재가등락률;최고가현재가등락률;전일비;현재가;\r\n
// subGraphtype;subGraphStyle;subGraphDrawStyle;subGraphName;title;숨김;색1;색2;색3;색4;색5;색6;
//		상승채움;하락채움;선굵기1;선굵기2;선굵기3;강조;\r\n
// .... subGraph 갯수만큼..
// GraphEnd;\r\n
// VertScaleGroupEnd;\r\n
// BlockEnd;\r\n
// BlockColumnEnd;\r\n
// -> Block 만들기 : 지표만 해당되는 경우
BOOL CIndicatorDefaultData::GetBlockDataForMaking( CString &p_strBlockData, CIndicatorList* pIndicatorList, const CString& strIndicatorName, 
		const CScaleBaseData::SCALEGRIDTYPE eHorzGridType, const CScaleBaseData::SCALEGRIDTYPE eVertGridType, CString &p_strGraphName)
{
	p_strBlockData.Empty();
	p_strGraphName.Empty();
	if( strIndicatorName.IsEmpty()) return FALSE;

	// vertScaleGroup 부분.
	m_eBaseGridType = eVertGridType;
	CString strVertScaleGroupData;
	GetVertScaleGroupDataForMaking( strVertScaleGroupData, pIndicatorList, strIndicatorName, p_strGraphName);
	if( strVertScaleGroupData.IsEmpty() || p_strGraphName.IsEmpty()) return FALSE;

	// 가로 scale 부분.
	m_eBaseGridType = eHorzGridType;

	CString strHorizontalScaleData = GetHorizontalScaleData( strIndicatorName, TRUE);

	int nBlockDataLen = 0;
	nBlockDataLen += 22;									// "BlockColumn;\r\nBlock;\r\n"
	nBlockDataLen += strHorizontalScaleData.GetLength();
	nBlockDataLen += strVertScaleGroupData.GetLength();
	nBlockDataLen += 28;									// "BlockEnd;\r\nBlockColumnEnd;\r\n"

	char *szBlockData = p_strBlockData.GetBufferSetLength( nBlockDataLen);
	
	strcpy( szBlockData, "BlockColumn;\r\nBlock;\r\n");
	szBlockData += 22;

	strcpy( szBlockData, strHorizontalScaleData);
	szBlockData += strHorizontalScaleData.GetLength();

	strcpy( szBlockData, strVertScaleGroupData);
	szBlockData += strVertScaleGroupData.GetLength();
	
	strcpy( szBlockData, "BlockEnd;\r\nBlockColumnEnd;\r\n");
	szBlockData += 28;

	return TRUE;
}

//sy 2004.11.02.
// -> VertScaleGroup 만들기 : 지표만 해당되는 경우
BOOL CIndicatorDefaultData::GetVertScaleGroupDataForMaking( CString &p_strVertScaleGroupData, CIndicatorList* pIndicatorList, const CString &strIndicatorName, CString &p_strGraphName) const
{
	p_strGraphName.Empty();
	p_strVertScaleGroupData.Empty();
	if( !pIndicatorList) return FALSE;
	CIndicatorInfo *pIndicatorInfo = pIndicatorList->CreateGraphIndicatorInfo( strIndicatorName);
	if( !pIndicatorInfo) return FALSE;
	GetVertScaleGroupDataForMaking( p_strVertScaleGroupData, pIndicatorInfo);
	p_strGraphName = pIndicatorInfo->GetGraphName();
	return TRUE;
}

// -> VertScaleGroup 만들기 : 지표만 해당되는 경우
void CIndicatorDefaultData::GetVertScaleGroupDataForMaking( CString &p_strVertScaleGroupData, CIndicatorInfo* pIndicatorInfo) const
{
	p_strVertScaleGroupData.Empty();
	if( !pIndicatorInfo) return;

	// (2004.10.06, 배승원) 가격차트의 Graph Drawing Data를 지정받아 DefaultType에 따라 관리, 적용토록 한다.
	CString strGraphData;
	pIndicatorInfo->GetGraphDataForMaking( strGraphData);
	if( strGraphData.IsEmpty()) return;

	CString strVertScaleData;
	GetVerticalScaleData( strVertScaleData, pIndicatorInfo->GetIndicatorName());

	int nVertScaleGroupDataLen = 0;
	nVertScaleGroupDataLen += 17;		// "VertScaleGroup;\r\n"
	nVertScaleGroupDataLen += strVertScaleData.GetLength();
	nVertScaleGroupDataLen += strGraphData.GetLength();
	nVertScaleGroupDataLen += 20;		// "VertScaleGroupEnd;\r\n"

	char *szVertScaleGroupData = p_strVertScaleGroupData.GetBufferSetLength( nVertScaleGroupDataLen);
	
	strcpy( szVertScaleGroupData, "VertScaleGroup;\r\n");
	szVertScaleGroupData += 17;
	
	strcpy( szVertScaleGroupData, strVertScaleData);
	szVertScaleGroupData += strVertScaleData.GetLength();
	
	strcpy( szVertScaleGroupData, strGraphData);
	szVertScaleGroupData += strGraphData.GetLength();
	
	strcpy( szVertScaleGroupData, "VertScaleGroupEnd;\r\n");
	szVertScaleGroupData += 20;
}
//sy end

// ----------------------------------------------------------------------------
// 가로 scaleData 만들기
// 사용할 Data;위치;grid타입;일자타입;시간타입;text타입;log;invert;2배block;\r\n
CString CIndicatorDefaultData::GetHorizontalScaleData(const CString& strGraphName, const BOOL bIsEndBlock) const
{
	CString strData;
	// (2004.03.11, 배승원) 모든 자료일자를 '/'로 통일한다.
	strData.Format("%s;%d;%d;/;:;가로;\r\n",
		GetHorizontalPacketName(strGraphName), // 사용할 Data -> 지표 Data
		GetHorizontalPosition(strGraphName, bIsEndBlock),   // 위치
		m_eBaseGridType);	// grid type

	return strData;
}

// 세로 scaleData 만들기
// 사용할 Data;위치;grid타입;unit타입;\r\n
void CIndicatorDefaultData::GetVerticalScaleData( CString &p_strVerticalScaleData, const CString& strGraphName) const
{
	//sy 2004.05.12. -> graph 에 따른 세로 스케일 위치 변경.
	// (2004.09.16, 배승원, 분석) Main Block의 현재 Scale 설정을 따라가도록 한다.
	CScaleBaseData::VERTSCALEPOSITION eVertScalePosition = m_eVertScalePosition;

	// (2006/4/16 - Seung-Won, Bae) Special Chart has scale in Both Side.
	if( g_iMetaTable.IsSpecialGroupGraph( strGraphName)) eVertScalePosition = CScaleBaseData::VERT_BOTH;

	int nIndiDataLen = strlen( _MTEXT( C6_INDICATOR_DATA));

	char szVertScalePosition[ 20];
	itoa( eVertScalePosition, szVertScalePosition, 10);
	int nVertScalePositionLen = strlen( szVertScalePosition);

	char szBaseGridType[ 20];
	itoa( m_eBaseGridType, szBaseGridType, 10);
	int nBaseGridTypeLen = strlen( szBaseGridType);

	int nVerticalScaleDataLen = 0;
	nVerticalScaleDataLen += nIndiDataLen;
	nVerticalScaleDataLen++;						// ;
	nVerticalScaleDataLen += nVertScalePositionLen;	
	nVerticalScaleDataLen++;						// ;
	nVerticalScaleDataLen += nBaseGridTypeLen;	
	nVerticalScaleDataLen++;						// ;
	nVerticalScaleDataLen += 4;						// 0;\r\n

	char *szVirticalScaleData = p_strVerticalScaleData.GetBufferSetLength( nVerticalScaleDataLen);

	strcpy( szVirticalScaleData, _MTEXT( C6_INDICATOR_DATA));
	szVirticalScaleData += nIndiDataLen;
	*szVirticalScaleData++ = ';';

	strcpy( szVirticalScaleData, szVertScalePosition);
	szVirticalScaleData += nVertScalePositionLen;
	*szVirticalScaleData++ = ';';

	strcpy( szVirticalScaleData, szBaseGridType);
	szVirticalScaleData += nBaseGridTypeLen;
	*szVirticalScaleData++ = ';';

	strcpy( szVirticalScaleData, "0;\r\n");
}

// ----------------------------------------------------------------------------
// 가로 scale 사용할 Data.
CString CIndicatorDefaultData::GetHorizontalPacketName(const CString& strGraphName) const
{
	if(strGraphName == _MTEXT( C2_THREE_LINE_BREAK))	// "삼선전환도"
		return _MTEXT( C0_THREE_LINE_BREAK_DATE_TIME);
	else if(strGraphName == _MTEXT( C2_INVERSE_TIME_LINE))		// "역시계곡선"
		return _MTEXT( C0_INVERSETIMELINE_VOLUME);
	//2007.06.01 by LYH P&F X스케일 숨김에서 보임으로 수정 return ""; -> return _MTEXT( C0_PF_DATE_TIME);
	else if(strGraphName == _MTEXT( C2_P_AND_F))	// "P&F"
		return _MTEXT( C0_PF_DATE_TIME);
	else if(strGraphName == _MTEXT( C2_SWING_CHART))	// "SwingChart"
		return _MTEXT( C0_SWING_CHART_DATE_TIME);
	else if(strGraphName == _MTEXT( C2_KAGI_CHART))		// "KagiChart"
		return _MTEXT( C0_KAGI_DATE_TIME);
	else if(strGraphName == _MTEXT( C2_RENKO_CHART))	// "RenKoChart"
		return _MTEXT( C0_RENKO_CHART_DATETIME);
// (2006.11.09 안승환) 분산차트 추가
	else if(strGraphName == _MTEXT( C2_DISPERSION))	// "분산Chart"
		return _MTEXT( C0_VOLUME);
// (2006.11.09 안승환) 분산차트 추가

	return _MTEXT( C0_DATE_TIME);
}

// 가로 scale 의 위치.
CScaleBaseData::HORZSCALEPOSITION CIndicatorDefaultData::GetHorizontalPosition(const CString& strGraphName, const BOOL bIsEndBlock) const
{
	// Block 하나 / 끝 -> 하단(점선 및 text 나옴)
	if(bIsEndBlock)
		return CScaleBaseData::HORZ_BOTTOM;

	// Block 두개 이상 시 처음/중간 -> 숨김(점선만 나옴)
	return CScaleBaseData::HORZ_HIDE;
}

CIndicatorDefaultData::BLOCKMAKINGTYPE CIndicatorDefaultData::GetBlockMakingType(const int nCount, const int nIndex) const
{
	if(nCount <= 0)
		return NONE;
	else if(nCount == 1) // block 1개인 경우
		return ONE_BLOCK;

	if(nIndex == 0) // block 2개 이상 중 처음인 경우
		return START_TWO_BLOCK;
	else if(nIndex == nCount - 1) // block 2개 이상 중 끝인 경우
		return END_TWO_BLOCK;

	return MIDDEL_THREE_BLOCK; // block 3개 이상 중 가운데인 경우
}
