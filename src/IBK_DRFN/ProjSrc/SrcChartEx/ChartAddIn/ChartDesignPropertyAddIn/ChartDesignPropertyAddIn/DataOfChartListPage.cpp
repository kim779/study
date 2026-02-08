// DataOfChartListPage.cpp: implementation of the CDataOfChartListPage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "DataOfChartListPage.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTOL6

#include "MyTreeCtrl.h"
#include "Conversion.h"
#include "ToolListData.h"								// for BLOCK

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataOfChartListPage::CDataOfChartListPage(const int nRow, const int nColumn, CMyTreeCtrl* pTreeChart, HWND p_hOcxWnd)
{
	m_pTreeChart = pTreeChart;
	m_nRow = nRow;
	m_nColumn = nColumn;

	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = p_hOcxWnd;
}

CDataOfChartListPage::CDataOfChartListPage( HWND p_hOcxWnd)
{
	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = p_hOcxWnd;
}

CDataOfChartListPage::~CDataOfChartListPage()
{

}

void CDataOfChartListPage::SetBlockItem(const int nBlock, CString strData)
{
	//sy 2004.3.12. - 디폴트로 저장된 block의 내용을 설정된 값으로 변경한다.
	if(strData.IsEmpty())
		return;

	CString szScaleData = CDataConversion::GetStringData(strData, ";");
	int nHorizonpos = atoi(CDataConversion::GetStringData(strData, ";"));
	int nScaleCompart = atoi(CDataConversion::GetStringData(strData, ";"));
	CString szDateScalePosition = CDataConversion::GetStringData(strData, ";");
	CString szTimeScalePosition = CDataConversion::GetStringData(strData, ";");
	int nHorizonStyle = atoi(CDataConversion::GetStringData(strData, ";"));
	int nLog = atoi(CDataConversion::GetStringData(strData, ";"));
	int nOverTurn = atoi(CDataConversion::GetStringData(strData, ";"));

	// (2006/10/25 - Seung-Won, Bae) Do not used Double Size Block Flag.
	//		And Do not make Double Size Block Data to use the RDB(Row Double Size Index).
	CString strScaleData = _MTOL6_N( szScaleData);
	if( strScaleData.IsEmpty()) strScaleData = _MTOL0( szScaleData);
	m_pTreeChart->ChangeBlockData(nBlock, strScaleData, nHorizonpos, nScaleCompart, szDateScalePosition, szTimeScalePosition, nHorizonStyle, nLog, nOverTurn);
}

//sy 2006.04.17.
void CDataOfChartListPage::GetSignalData(const CString& strData, bool& bSignalUse, bool& bSignalShow)
{
	bSignalUse = bSignalShow = false;

	CString strRealData = strData;
	if(strRealData.Right(1) != ';')
		strRealData += ";";

	// "signal사용여부:signal사용;"
	// signal사용여부
	bSignalUse = CDataConversion::atob(CDataConversion::GetStringData(strRealData, ":"));
	// signal사용
	bSignalShow = CDataConversion::atob(CDataConversion::GetStringData(strRealData, ';'));
}
//sy end

CString CDataOfChartListPage::GetBlockandGraphAllData(CChartLink *pRoot)
{
	CChartLink* pBlock = pRoot->m_Child;
	
	CString strBlockandGraphAllData = "";
	if(pBlock == NULL) return "";

	//BlockColumn을 구별하기 위한 변수.
	bool bFirst = false;
	int nOldColumn = 0;

	while(pBlock) { //Block
		if(pBlock->m_iType == BLOCK) {
			if(nOldColumn != pBlock->m_nColumnIndex)
				bFirst = true;
			if(bFirst == true) {
				if(pBlock->m_nColumnIndex != 1) 
					strBlockandGraphAllData += "BlockColumnEnd;\r\n";
				strBlockandGraphAllData += "BlockColumn;\r\n";
				nOldColumn = pBlock->m_nColumnIndex;
				bFirst = false;
			}
			strBlockandGraphAllData += GetBlockAllData(pBlock);
		}
		CChartLink* pVertiScale = pBlock->m_Child; //verti Scale
		if(pVertiScale == NULL) //VertiScale, graph가 없을때..
			strBlockandGraphAllData += "BlockEnd;\r\n";

		while(pVertiScale) { //Vertical가 있을때..
			strBlockandGraphAllData += GetVerticalAllData(pVertiScale); //Vertical Data 얻기.
			CChartLink* pGraph = pVertiScale->m_Child; //graph
			if(pGraph == NULL) //graph가 없을때..
				strBlockandGraphAllData += "VertScaleGroupEnd;\r\n";

			while(pGraph) { //Graph
				if(pGraph->m_iType == GRAPH) 
					strBlockandGraphAllData += GetGraphAllData(pGraph);

				pGraph = pGraph->m_Next; //다음 graph
				if(pGraph == NULL) //다음 grpah가 없을때..
					strBlockandGraphAllData += "VertScaleGroupEnd;\r\n";
			}

			pVertiScale = pVertiScale->m_Next; //next Vertical Scale
			if(pVertiScale == NULL) //다음 Vertical Scale 없을때..
				strBlockandGraphAllData += "BlockEnd;\r\n";
		}
		pBlock = pBlock->m_Next; //next Block
	}
	strBlockandGraphAllData += "BlockColumnEnd;\r\n";
	return strBlockandGraphAllData;
}

CString CDataOfChartListPage::GetBlockAllData(CChartLink *pItem)
{
	CString strValue = "";
	strValue += "Block;\r\n";

	CString strScaleStyle = _LTOM6_N( pItem->m_strScaleStyle);
	if( strScaleStyle.IsEmpty()) strScaleStyle = _LTOM0( pItem->m_strScaleStyle);

	strValue += strScaleStyle + ";" ; //가로스케일데이타
	strValue += CDataConversion::IntToString(pItem->m_nHorizonpos) + ";" ; //가로스케일위치
	strValue += CDataConversion::IntToString(pItem->m_nScaleCompartType) + ";" ; //구분선타입.
	strValue += pItem->m_strDateScaleCompart + ";"; //일자스케일구분
	strValue += pItem->m_strTimeScaleCompart + ";"; //시간스케일구분타입
	strValue += CDataConversion::IntToString(pItem->m_nHorizonStyle) + ";"; //가로, 세로중 하나
	strValue += CDataConversion::IntToString(pItem->m_nLog) + ";"; //log
	strValue += "0;";	// (2008/2/26 - Seung-Won, Bae) Block Reverse. Not used any more.

	// (2006/10/25 - Seung-Won, Bae) Do not make Double Size Block Data to use the RDB(Row Double Size Index).
	//		And Do not used Double Size Block Flag.

	strValue += "\r\n";
	return strValue;
}

CString CDataOfChartListPage::GetGraphAllData(CChartLink *pItem)
{
	CString strValue = "";
	strValue += "Graph;\r\n";
	// (2006/10/26 - Seung-Won, Bae) Skip Grpah Type
	strValue += ";";															//	pItem->m_strName + ";"; //그래프타입
	strValue += pItem->m_strIndicatorName + ";";								//지표명
	strValue += "G:" + pItem->m_strGraphName + ";";								// (2006/11/29 - Seung-Won, Bae) Support Graph Name for Binding with Indicator Info.
	strValue += pItem->m_strCheckedPacketItems + ";";							//그래프 사용할 Packet 이름
	//sy 2006.04.17.
	strValue += CDataConversion::IntToString(pItem->m_bSignalUse) + ":";		//signal 유무
	strValue += CDataConversion::IntToString(pItem->m_bSignalShow) + ";";		//signal 유무
	//sy end
	strValue += CDataConversion::IntToString(pItem->m_bLowHighUpDown) + ";";	//화면오른쪽 위에 수치데이타 표현
	strValue += CDataConversion::IntToString(pItem->m_bLowNowUpDown) + ";";
	strValue += CDataConversion::IntToString(pItem->m_bHighNowUpDown) + ";";
	strValue += CDataConversion::IntToString(pItem->m_bPreNowContrast) + ";";
	strValue += CDataConversion::IntToString(pItem->m_nCurData) + ";";
	strValue += "\r\n";	
	strValue += pItem->m_strSubGraphDatas; 
	strValue += "GraphEnd;\r\n";
	return strValue;
}
/*
CString CDataOfChartListPage::GetVertScaleGroup(CDataOfChartListPage::SCALEJOIN eScaleJoin, const bool bScaleJoin)
{
	CString strValue = "";
	if(eScaleJoin == CDataOfChartListPage::O && bScaleJoin == false) { //Check, NoCheck
		strValue += "VertScaleGroupEnd;\r\n";
		strValue += "VertScaleGroup;\r\n";					
	}
	else if(eScaleJoin == CDataOfChartListPage::X && bScaleJoin == false) { //NoCheck, NoCheck
		strValue += "VertScaleGroupEnd;\r\n";
		strValue += "VertScaleGroup;\r\n";					
	}
	else if(eScaleJoin == CDataOfChartListPage::NONE && bScaleJoin == false) // -, NoCheck
		strValue += "VertScaleGroup;\r\n";					
	else if(eScaleJoin == CDataOfChartListPage::NONE && bScaleJoin == true) // -, Check
		strValue += "VertScaleGroup;\r\n";					
	return strValue;
}
*/
CString CDataOfChartListPage::GetVerticalAllData(CChartLink *pItem)
{
	CString strValue = "";
	strValue += "VertScaleGroup;\r\n";

	strValue +=	_LTOMS2( pItem->m_strScaleStyle, ',', 6, 0)						+ ";";  //세로스케일데이타
	strValue +=	CDataConversion::IntToString(pItem->m_nScalePosition)			+ ";"; //세로스케일 위치
	strValue +=	CDataConversion::IntToString(pItem->m_nScaleCompartType)		+ ";"; //Grid Type
	strValue +=	CDataConversion::IntToString(pItem->m_nScaleExpressionType)		+ ";"; //scale 표현 타입

	// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
	strValue +=	CDataConversion::Bool2Str(			pItem->m_bMinMaxLock)		+ ";"; // Lock
	strValue +=	CDataConversion::IntToString(		( int)pItem->m_eMinMaxType)	+ ";"; // Type
	strValue +=	CDataConversion::DoubleToString(	pItem->m_dMin)				+ ";"; // Min
	strValue +=	CDataConversion::DoubleToString(	pItem->m_dMax)				+ ";"; // Max

	// (2007/7/19 - Seung-Won, Bae) Support Up/Down Space
	strValue +=	CDataConversion::IntToString(		pItem->m_nUpSpace)			+ ";"; // UpSpace
	strValue +=	CDataConversion::IntToString(		pItem->m_nDownSpace)		+ ";"; // DownSpace

	//2007.01.22 Scale사용자 설정,호가단위별
	strValue +=	CDataConversion::DoubleToString(	pItem->m_dHogaNumber)		+ ";";
	strValue +=	CDataConversion::DoubleToString(	pItem->m_dUserNumber)		+ ";";
	strValue +=	CDataConversion::IntToString(		pItem->m_nUserHogaType)		+ ";";

	// 지표 뒤집기 - ojtaso (20071023)
	strValue += CDataConversion::Bool2Str(			pItem->m_bIsInvert)			+ ";";

	strValue += "\r\n";
	return strValue;
}

CChartLink* CDataOfChartListPage::SetVerticalScaleItem(const int nVerticalScale, CString strData)
{
	CString data = strData;

	CDataConversion::GetStringData(data, "VertScaleGroup;\r\n");
	CString strScaleData = CDataConversion::GetStringData(data, ";");
	int nScalePos = atoi(CDataConversion::GetStringData(data, ";"));
	int nScaleCompart = atoi(CDataConversion::GetStringData(data, ";"));
	int nScaleExpessionType = atoi(CDataConversion::GetStringData(data, ";"));

	// (2006/11/11 - Seung-Won, Bae) Min/Max Setting
	BOOL	bMinMaxLock	= atoi( CDataConversion::GetStringData( data, ";"));	// Lock
	int		nMinMaxType	= atoi( CDataConversion::GetStringData( data, ";"));	// Type
	double	dMin		= atof( CDataConversion::GetStringData( data, ";"));	// Min
	double	dMax		= atof( CDataConversion::GetStringData( data, ";"));	// Max

	// (2007/7/19 - Seung-Won, Bae) Support Up/Down Space
	int nUpSpace	= atoi( CDataConversion::GetStringData( data, ";"));	// UpSpace
	int nDownSpace	= atoi( CDataConversion::GetStringData( data, ";"));	// DownSpace

	//2007.01.22 Scale사용자 설정,호가단위별
	double dHogaNumber	= atof( CDataConversion::GetStringData( data, ";"));
	double dUserNumber	= atof( CDataConversion::GetStringData( data, ";"));
	int nUserHogaType	= atoi( CDataConversion::GetStringData( data, ";"));

	// 지표 뒤집기 - ojtaso (20071023)
	BOOL	bIsInvert	= atoi( CDataConversion::GetStringData( data, ";"));	// Invert

	// 지표 뒤집기 - ojtaso (20071023)
	return m_pTreeChart->AddVerticalScaleItem( strScaleData, nScalePos, nScaleCompart, nScaleExpessionType, bMinMaxLock, nMinMaxType, dMin, dMax, nUpSpace, nDownSpace, dHogaNumber, dUserNumber, nUserHogaType, bIsInvert);
}

CChartLink* CDataOfChartListPage::SetVerticalScaleItem_Temp(const int nVerticalScale, const CString strData)
{
	CString data = strData;
	CDataConversion::GetStringData(data, "Graph;\r\n");
	CDataConversion::GetStringData(data, ";");
	CDataConversion::GetStringData(data, ";");
	CDataConversion::GetStringData(data, ";");
	CDataConversion::GetStringData(data, ";");
	CString strScaleData = CDataConversion::GetStringData(data, ";");

	// 지표 뒤집기 - ojtaso (20071023)
	return m_pTreeChart->AddVerticalScaleItem( strScaleData, 2, 0, 0, FALSE, 0, 0.0, 0.0, 0, 0, 0.0, 0.0, 0, 0);
}


void CDataOfChartListPage::SetBlockItem_Temp(const int nBlock, CString strData)
{
	//sy 2004.3.12. - 디폴트로 저장된 block의 내용을 설정된 값으로 변경한다.
	if(strData.IsEmpty())
		return;

	CDataConversion::GetStringData(strData, "Block;\r\n");
	CString szScaleData = CDataConversion::GetStringData(strData, ";");
	int nHorizonpos = atoi(CDataConversion::GetStringData(strData, ";"));
	CString szDateScalePosition = CDataConversion::GetStringData(strData, ";");
	CString szTimeScalePosition = CDataConversion::GetStringData(strData, ";");
	int nHorizonStyle = atoi(CDataConversion::GetStringData(strData, ";"));
	int nLog = atoi(CDataConversion::GetStringData(strData, ";"));
	int nOverTurn = atoi(CDataConversion::GetStringData(strData, ";"));
	m_pTreeChart->ChangeBlockData(nBlock, szScaleData, nHorizonpos, 0, szDateScalePosition, szTimeScalePosition, nHorizonStyle, nLog, nOverTurn);
}

CString CDataOfChartListPage::GetGraphTypeWhenNoTypeInfo(const int nGraphType)
{
	switch(nGraphType)
	{
	case CGraphBaseData::Bong_Type:
		return _MTEXT( C5_CANDLE_TYPE);
	default:
	case CGraphBaseData::Line_Type:
		return _MTEXT( C5_LINE_TYPE);
	case CGraphBaseData::Bar_Type:
		return _MTEXT( C5_BAR_TYPE);
	case CGraphBaseData::Pie_Type:
		return _MTEXT( C5_PIE_TYPE);
	case CGraphBaseData::Special_Type:
		return _MTEXT( C5_SPECIAL_TYPE);
	case CGraphBaseData::Fund_Type:
		return _MTEXT( C5_FUND_TYPE);
	case CGraphBaseData::DataView_Type:
		return _MTEXT( C5_DATA_EXPRESSION_TYPE);
	}
}

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
void CDataOfChartListPage::SetData(const CString &strChartListPageData, IGraphInfoManager *p_pIGraphInfoManager)
{
	CString _szData = strChartListPageData;

	// (2006/11/15 - Seung-Won, Bae) Logic Adjusting
	if( _szData.Find("VertScaleGroup;\r\n") > 0)
	{ 
		CDataConversion::GetStringData(_szData, "BlockColumn;\r\n");
		int nBlockCount = CDataConversion::GetStringDataCount(_szData, "Block;\r\n");
		for(int i = 0 ; i < nBlockCount ; i++)
		{
			CDataConversion::GetStringData( _szData, "Block;\r\n");
			CString strBlockData = CDataConversion::GetStringData(_szData, "BlockEnd;\r\n");
			SetBlockItem(i, CDataConversion::GetStringData(strBlockData, "\r\n"));
			int nScaleGroupCount = CDataConversion::GetStringDataCount(strBlockData, "VertScaleGroup;\r\n");
			for(int nScale = 0 ; nScale< nScaleGroupCount ; nScale++)
			{
				CDataConversion::GetStringData(strBlockData, "VertScaleGroup;\r\n");
				CString ScaleGroup = CDataConversion::GetStringData(strBlockData, "VertScaleGroupEnd;\r\n");
				CChartLink* pVertiScale = SetVerticalScaleItem(nScale, CDataConversion::GetStringData(ScaleGroup, "\r\n"));
				int nGraphCount = CDataConversion::GetStringDataCount(ScaleGroup, "Graph;\r\n");
				for(int j = 0; j < nGraphCount ; j++)
				{
					CDataConversion::GetStringData( ScaleGroup, "Graph;\r\n");
					CString strGraph = CDataConversion::GetStringData( ScaleGroup, "GraphEnd;\r\n");
					if(i < (m_nColumn * m_nRow)) SetItem( pVertiScale, strGraph, p_pIGraphInfoManager);
				}
			}
		}
	}
	else { //임시..
		CDataConversion::GetStringData(_szData, "BlockColumn;\r\n");
		int nBlockCount = CDataConversion::GetStringDataCount(_szData, "Block;\r\n");
		for(int i = 0 ; i < nBlockCount ; i++) {
			CString strBlockData = CDataConversion::GetStringData(_szData, "BlockEnd;\r\n");
			int nGraphCount = CDataConversion::GetStringDataCount(strBlockData, "Graph;\r\n");
			SetBlockItem_Temp(i, CDataConversion::GetStringData(strBlockData, "Graph;\r\n"));
			CChartLink* pVertiScale = SetVerticalScaleItem_Temp(i, strBlockData);
			for(int j = 0; j < nGraphCount ; j++) {
				if(i < (m_nColumn * m_nRow)) 
					SetItem(pVertiScale, CDataConversion::GetStringData(strBlockData, "GraphEnd;\r\n"), p_pIGraphInfoManager);
			}
		}
	}
	return;
}

// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
//sy 2004.3.12. -> <ChartList Setting> 부분의 GraphType인 "봉형", "바형"... type를 
//				왼쪽의 Tree 형태에 그대로 적용하기 위함.
//				기존에는 Graph의 첫번째 SubGraph의 Type으로 무조건 적용하였음.
//				(이걸로 인해 "대기매물"은 "특수형"임에도 불구하고 "바형"으로 나옴.)
void CDataOfChartListPage::SetItem(CChartLink* pVertiScale, const CString& data, IGraphInfoManager *p_pIGraphInfoManager)
{
	CString subGraphData = data;
	CString graphData = CDataConversion::GetStringData(subGraphData, "\r\n");

	//나중에 지표의 인덱스가 들어올 자리
	//지금은 Graph 형태를 저장하고 있다!
	// (2006/10/26 - Seung-Won, Bae) Skip GraphType (? Function Index ?)
	CDataConversion::GetStringData(graphData, ";");
	CString strIndicatorName = CDataConversion::GetStringData(graphData, ";");

	// (2006/11/29 - Seung-Won, Bae) Support Graph Name for Binding with Indicator Info.
	CString strGraphName( strIndicatorName);
	if( graphData.Left( 2) == "G:") strGraphName = CDataConversion::GetStringData( graphData, ";").Mid( 2);

	CString strPacket = CDataConversion::GetStringData(graphData, ";");
	bool bLowHighUpDown = false;
	bool bLowNowUpDown = false;
	bool bHighNowUpDown = false;
	bool bPreNowContrast = false;
	int nCurData = 0;
	//sy 2006.04.17.
	bool bSignalUse = false, bSignalShow = false;
	GetSignalData(CDataConversion::GetStringData(graphData, ";"), bSignalUse, bSignalShow);

	bLowHighUpDown = CDataConversion::atob(CDataConversion::GetStringData(graphData, ";"));
	bLowNowUpDown = CDataConversion::atob(CDataConversion::GetStringData(graphData, ";"));
	bHighNowUpDown = CDataConversion::atob(CDataConversion::GetStringData(graphData, ";"));
	bPreNowContrast = CDataConversion::atob(CDataConversion::GetStringData(graphData, ";"));
	nCurData = atoi(CDataConversion::GetStringData(graphData, ";"));

	//sy end

	CString strType;
	// <data> 에 "봉형", "바형".. 같은 타입이 저장 되어 있을 경우는 그 타입으로 적용한다.
	strType = GetGraphType(subGraphData, p_pIGraphInfoManager);

	m_pTreeChart->AddItem(pVertiScale, _MTOL5( strType), strIndicatorName, strGraphName, strPacket,
							bLowHighUpDown, bLowNowUpDown, bHighNowUpDown, bPreNowContrast, nCurData, 
							bSignalUse, bSignalShow, subGraphData);
}

// (2006/10/25 - Seung-Won, Bae) Set Item Title with First SubGraph Type.
CString CDataOfChartListPage::GetGraphType(const CString& strGraphData, IGraphInfoManager *p_pIGraphInfoManager)
{
	if( strGraphData.IsEmpty()) return _MTEXT( C5_LINE_TYPE);

	const char *szGraphData = strGraphData;
	if( !szGraphData) return _MTEXT( C5_LINE_TYPE);

	char szType[ 2] = { ' ', '\0'};
	szType[ 0] = szGraphData[ 0];
	int nGraphType = atoi( szType);

	// (2006/7/5 - Seung-Won, Bae) Use IGraphInfoManager
	CString strTypeData, strStyleData;
	if( p_pIGraphInfoManager->GetGraphTypeAndStyle( nGraphType, 0, strTypeData, strStyleData))
		return strTypeData;

	return GetGraphTypeWhenNoTypeInfo( nGraphType);
}
