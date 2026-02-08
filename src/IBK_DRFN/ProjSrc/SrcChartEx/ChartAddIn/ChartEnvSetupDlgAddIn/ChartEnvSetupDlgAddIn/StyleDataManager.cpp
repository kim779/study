// StyleDataManager.cpp: implementation of the CStyleDataManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "StyleDataManager.h"

#include "../Include_Chart/DLL_Load/IMetaTable.h"		// for _MTEXT()
#include "EnvironmentData.h"							// for CStyleData
#include "StyleFileBuilder.h"							// for CStyleFileBuilder

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SAMPLE_BONG_COUNT	42

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStyleDataManager::CStyleDataManager()
{

}

CStyleDataManager::~CStyleDataManager()
{
}

//////////////////////////////////////////////////////////////////////
// Satic Handler
//////////////////////////////////////////////////////////////////////
CList<CStyleData*, CStyleData*> CStyleDataManager::m_lstStyleDefault;
CList<CStyleData*, CStyleData*> CStyleDataManager::m_lstStyleUser;

void CStyleDataManager::RemoveAll( void)
{
	while( !m_lstStyleDefault.IsEmpty())	delete m_lstStyleDefault.RemoveHead();
	while( !m_lstStyleUser.IsEmpty())		delete m_lstStyleUser.RemoveHead();
}

void CStyleDataManager::SetStyleDataToOCX( CStyleData &p_styleData, IChartOCX *p_pIChartOCX, BOOL p_bResetSaveCFG, BOOL p_bSkipAnalysisColor, BOOL p_bSkipVerticalScaleGrid, BOOL p_bSkipHorizontalScaleGrid)
{
	if( !p_pIChartOCX) return;

	// Graph Region Color
	p_pIChartOCX->SetStyleValueGraphBGColor(		p_styleData.GetGraphBGColor());
	p_pIChartOCX->SetStyleValueGraphBorderColor(	p_styleData.GetGraphBorderColor());
	p_pIChartOCX->SetStyleValueGraphGridColor(		p_styleData.GetGraphGridColor(),	TRUE);
	p_pIChartOCX->SetStyleValueGridHeight(			p_styleData.GetGridHeight());
	p_pIChartOCX->SetStyleValueGridWidth(			p_styleData.GetGridWidth(),			TRUE);
	p_pIChartOCX->SetStyleValueGridHLineColor(		p_styleData.GetScaleGridColor());
	p_pIChartOCX->SetStyleValueGridVLineColor(		p_styleData.GetScaleGridColor());
	p_pIChartOCX->SetStyleValueScaleTextColor(		p_styleData.GetScaleTextColor(),	TRUE);

	// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
	if( !p_bSkipVerticalScaleGrid)		p_pIChartOCX->SetAllVertScaleGridType(		p_styleData.GetDivideHeight(),		TRUE);
	if( !p_bSkipHorizontalScaleGrid)	p_pIChartOCX->SetAllHorzScaleGridType(		p_styleData.GetDivideWidth(),		TRUE);

	//scale 관련 색 : Update in OnDraw
	p_pIChartOCX->SetStyleValueScaleBGColor(		p_styleData.GetScaleBGColor());
	p_pIChartOCX->SetStyleValueScaleBorderColor(	p_styleData.GetScaleBorderColor());

	//Analysis tool	: Update in EAI_ALL_APPLY_STYLE
	// (2007/5/2 - Seung-Won, Bae) Analysis Color Notify Skip Flag
	if( !p_bSkipAnalysisColor) p_pIChartOCX->SetStyleValueAnalysisToolColor(	p_styleData.GetAnalyzeColor());


	////KHD : 시간구분
	p_pIChartOCX->SetStyleValueHScaleGridColor_TimeGubun(p_styleData.GetHScaleGridColor_Day(),p_styleData.GetHScaleGridColor_Week(),p_styleData.GetHScaleGridColor_Mon(),p_styleData.GetHScaleGridColor_Year(),		TRUE);//컬러 세팅
	////굵기 
	p_pIChartOCX->SetStyleValueScaleLineThickness_TimeGubun(CScaleBaseData::MINTICK_DAY_GUBUN,	p_styleData.GetScaleLineThickness_Day());
	p_pIChartOCX->SetStyleValueScaleLineThickness_TimeGubun(CScaleBaseData::DAYCHART_WEEK_GUBUN,	p_styleData.GetScaleLineThickness_Week());
	p_pIChartOCX->SetStyleValueScaleLineThickness_TimeGubun(CScaleBaseData::DAYCHART_MON_GUBUN,	p_styleData.GetScaleLineThickness_Mon());
	p_pIChartOCX->SetStyleValueScaleLineThickness_TimeGubun(CScaleBaseData::DAYCHART_YEAR_GUBUN,	p_styleData.GetScaleLineThickness_Year());
	
	// 수치조회창.	: Update in EAI_ALL_APPLY_STYLE
	p_pIChartOCX->SetStyleValueNIDLGColor(			p_styleData.GetNumericalColor());

	// (2009/9/22 - Seung-Won, Bae) Use SetStyleDataToIndex();
	ILPCSTR szRQ = p_pIChartOCX->GetCurrentRQ();
	IChartManager *pIChartManager = p_pIChartOCX->GetIChartManager();
	if( !pIChartManager) return;
	int nR = -1, nC = -1, nV = -1, nG = -1;
	ILPCSTR szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, szRQ);
	if( !szGraphName.IsEmpty()) SetStyleDataToIndex( p_styleData, p_pIChartOCX, szGraphName);
	szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_MA), nR, nC, nV, nG, szRQ);
	if( !szGraphName.IsEmpty()) SetStyleDataToIndex( p_styleData, p_pIChartOCX, szGraphName);
	szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_VOLUME_CHART), nR, nC, nV, nG, szRQ);
	if( !szGraphName.IsEmpty()) SetStyleDataToIndex( p_styleData, p_pIChartOCX, szGraphName);
	szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_VOLUME_MA), nR, nC, nV, nG, szRQ);
	if( !szGraphName.IsEmpty()) SetStyleDataToIndex( p_styleData, p_pIChartOCX, szGraphName);
	pIChartManager->Release();

	if( p_bResetSaveCFG) p_pIChartOCX->SaveChartCFG_ResetWithCurrentStyle();
 
	// (2006/1/26 - Seung-Won, Bae) Apply Style to AddIn Tools
	IAddInToolMgr *pIAddInToolMgr = p_pIChartOCX->GetIAddInToolMgr();
	if( pIAddInToolMgr)
	{
		pIAddInToolMgr->OnAddInToolCommand( EAI_ALL_APPLY_STYLE);
		pIAddInToolMgr->Release();
	}

	p_pIChartOCX->InvalidateControl();
}

void CStyleDataManager::SetStyleDataToIndex( CStyleData &p_styleData, IChartOCX *p_pIChartOCX, const char *p_szGraphName)
{
	IGraphInfoManager *pIGraphInfoManager = p_pIChartOCX->GetIGraphInfoManager();
	if( !pIGraphInfoManager) return;

	IChartManager *pIChartManager = p_pIChartOCX->GetIChartManager();
	if( !pIChartManager)
	{
		pIGraphInfoManager->Release();
		return;
	}

	IGraphInfo *pIGraphInfo = pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, p_szGraphName);
	if( pIGraphInfo)
	{
		ILPCSTR szIndicatorName = pIGraphInfo->GetIndicatorNameString();
		int nMACount = 0, i = 0;
		switch( g_iMetaTable.GetIndicatorID( szIndicatorName))
		{
			// Price Chart
			case IMetaTable::EDDC_PRICECHART:	pIGraphInfo->SetSubGraphColorAndFillFlag( 0, p_styleData.GetPriceUpColor(), p_styleData.GetPriceUpColor(),
													p_styleData.GetPriceDownColor(), p_styleData.GetPriceDownColor(), p_styleData.GetPriceEqualColor(),
													p_styleData.GetPriceEqualColor(), p_styleData.GetPriceUpFill(), p_styleData.GetPriceDownFill());
												break;
			// PriceMA Chart
// vntsorl:[0003431] [7111]차트화면에서의 조회속도가 구차트에 비해 늦어 개선 요구
// 			case IMetaTable::EDDC_ST_PRICEMACHART:	nMACount = p_styleData.GetPriceMACount();
			case IMetaTable::EDDC_OCX_PRICEMACHART:	nMACount = p_styleData.GetPriceMACount();
													for( i = 0; i < nMACount; i++) pIGraphInfo->SetSubGraphColor( i, 1, p_styleData.GetPriceMAColor( i));
													break;
			// Volume Chart
			case IMetaTable::EDDC_VOLUMECHART:	pIGraphInfo->SetSubGraphColorAndFillFlag( 0, p_styleData.GetTradeUpColor(), p_styleData.GetTradeUpColor(),
													p_styleData.GetTradeDownColor(), p_styleData.GetTradeDownColor(), p_styleData.GetTradeEqualColor(),
													p_styleData.GetTradeEqualColor(), p_styleData.GetTradeUpFill(), p_styleData.GetTradeDownFill());
												break;
		// VolumeMA Chart
// 			case IMetaTable::EDDC_ST_VOLUMEMACHART:	nMACount = p_styleData.GetTradeMACount();
			case IMetaTable::EDDC_OCX_VOLUMEMACHART:	nMACount = p_styleData.GetTradeMACount();
													for( int i = 0; i < nMACount; i++) pIGraphInfo->SetSubGraphColor( i, 1, p_styleData.GetTradeMAColor( i));
													break;
		}

		pIGraphInfo->Release();
		pIChartManager->ChangeGraphDataFromGraphInfo( p_szGraphName, FALSE);
	}
	pIGraphInfoManager->Release();
	pIChartManager->Release();
}

//////////////////////////////////////////////////////////////////////
// Operation
//////////////////////////////////////////////////////////////////////

// (2006/8/20 - Seung-Won, Bae) Initizling need the User Data.
//		If None - User Data, Read default data for Initial Data of User Data.
// (2008/4/7 - Seung-Won, Bae) Change Order for pure User File.
int CStyleDataManager::Initialize( IChartOCX *p_pIChartOCX, BOOL p_bDefaultData)
{
	// 1. Read Default
	// Init Default (for User)
	int nStyleCount = m_lstStyleDefault.GetCount();
	if( p_bDefaultData && 0 < nStyleCount) return nStyleCount;
	if( nStyleCount <= 0)
	{
		ILPCSTR szPath = p_pIChartOCX->GetDefaultPathString();
		CStyleFileBuilder().ReadStyleFile_Default( szPath, nStyleCount, m_lstStyleDefault);
	}
	if( p_bDefaultData) return nStyleCount;

	// 2. Read User
	// 2.0 Check already read?
	nStyleCount = m_lstStyleUser.GetCount();
	if( 0 < nStyleCount) return nStyleCount;
	// 2.2 Read and Add User Style
	ILPCSTR szPath = p_pIChartOCX->GetUserFilePathString();
	CStyleFileBuilder().ReadStyleFile_User( szPath, m_lstStyleUser);
	return m_lstStyleUser.GetCount();
}

int CStyleDataManager::GetDataCount( BOOL p_bDefaultData)
{
	if( p_bDefaultData) return m_lstStyleDefault.GetCount();
	return m_lstStyleUser.GetCount();
}

int CStyleDataManager::WriteStyleFile( IChartOCX *p_pIChartOCX)
{
	ILPCSTR szPath = p_pIChartOCX->GetUserFilePathString();
	return CStyleFileBuilder().WriteUserStyleFile( szPath, m_lstStyleDefault.GetCount(), m_lstStyleUser);
}

BOOL CStyleDataManager::RemoveStyleData( const char *p_szStyleName, BOOL p_bDefaultData)
{
	if( p_bDefaultData) return FALSE;

	POSITION pos = m_lstStyleUser.GetHeadPosition();
	while(pos != NULL)
	{
		POSITION curPos = pos;
		CStyleData* pStyleData = m_lstStyleUser.GetNext(pos);
	
		if(pStyleData->GetStyleName() == p_szStyleName)
		{
			m_lstStyleUser.RemoveAt(curPos);
			delete pStyleData;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CStyleDataManager::CopyStyleDataListTo( CList< CStyleData *, CStyleData * > &p_lstStyleDataList, BOOL p_bDefaultData)
{
	CList<CStyleData*, CStyleData*> *pStyleDataList = NULL;
	if( p_bDefaultData) pStyleDataList = &m_lstStyleDefault;
	else pStyleDataList = &m_lstStyleUser;
	
	CStyleData *pStyleData = NULL;
	POSITION psnStyle = pStyleDataList->GetHeadPosition();
	while( psnStyle)
	{
		pStyleData = pStyleDataList->GetNext( psnStyle);
		p_lstStyleDataList.AddTail( new CStyleData( *pStyleData));
	}
	return TRUE;
}

BOOL CStyleDataManager::CopyStyleDataListFrom( CList< CStyleData *, CStyleData * > &p_lstStyleDataList, BOOL p_bDefaultData)
{
	CList<CStyleData*, CStyleData*> *pStyleDataList = NULL;
	if( p_bDefaultData) pStyleDataList = &m_lstStyleDefault;
	else pStyleDataList = &m_lstStyleUser;
	
	while( !pStyleDataList->IsEmpty()) delete pStyleDataList->RemoveHead();

	CStyleData *pStyleData = NULL;
	POSITION psnStyle = p_lstStyleDataList.GetHeadPosition();
	while( psnStyle)
	{
		pStyleData = p_lstStyleDataList.GetNext( psnStyle);
		pStyleDataList->AddTail( new CStyleData( *pStyleData));
	}
	return TRUE;
}

void CStyleDataManager::AddStyleData( CStyleData *p_pStyleData, BOOL p_bDefaultData)
{
	if( p_bDefaultData) return;
	m_lstStyleUser.AddTail( p_pStyleData);
}

CStyleData *CStyleDataManager::GetStyleData( int p_nIndex, BOOL p_bDefaultData)
{
	if( p_nIndex < 0) return NULL;

	CList<CStyleData*, CStyleData*> *pStyleDataList = NULL;
	if( p_bDefaultData)
	{
		if( m_lstStyleDefault.GetCount() <= p_nIndex) return NULL;
		else pStyleDataList = &m_lstStyleDefault;
	}
	else
	{
		if(	m_lstStyleUser.GetCount() <= p_nIndex) return NULL;
		else pStyleDataList = &m_lstStyleUser;
	}
	
	POSITION psnStyle = pStyleDataList->FindIndex( p_nIndex);
	return pStyleDataList->GetAt( psnStyle);
}

CStyleData *CStyleDataManager::GetStyleData( const char *p_szStyleName, BOOL p_bDefaultData)
{
	CList<CStyleData*, CStyleData*> *pStyleDataList = NULL;
	if( p_bDefaultData) pStyleDataList = &m_lstStyleDefault;
	else pStyleDataList = &m_lstStyleUser;
	
	CStyleData *pStyleData = NULL;
	POSITION psnStyle = pStyleDataList->GetHeadPosition();
	while( psnStyle)
	{
		pStyleData = pStyleDataList->GetNext( psnStyle);
		if( pStyleData->GetStyleName() == p_szStyleName) return pStyleData;
	}
	return NULL;
}

BOOL CStyleDataManager::RemoveStyleDataIn( const int p_nIndex, BOOL p_bDefaultData)
{
	if( p_bDefaultData) return FALSE;

	POSITION pos = m_lstStyleUser.FindIndex( p_nIndex);
	if(pos == NULL) return TRUE;

	CStyleData* pStyleData = m_lstStyleUser.GetAt( pos);
	m_lstStyleUser.RemoveAt( pos);
	delete pStyleData;

	return TRUE;
}

CList<CStyleData*, CStyleData*> *CStyleDataManager::GetStyleDataList( BOOL p_bDefaultData)
{
	if( p_bDefaultData) return &m_lstStyleDefault;
	return &m_lstStyleUser;
}

void CStyleDataManager::CreateDummyUserStyle( IChartOCX *p_pIChartOCX)
{
	CList<CStyleData*, CStyleData*> lstStyleUserNew;
	lstStyleUserNew.AddTail( &m_lstStyleUser);
	m_lstStyleUser.RemoveAll();

	POSITION psnDefault = m_lstStyleDefault.GetHeadPosition();
	while( psnDefault)
	{
		CStyleData *pDefaultStyleData = m_lstStyleDefault.GetNext( psnDefault);
		if( !pDefaultStyleData) continue;

		CStyleData *pUserStyleData = NULL;
		BOOL bFound = FALSE;
		POSITION psnUserPrev = NULL, psnUser = lstStyleUserNew.GetHeadPosition();
		while( psnUser && !bFound)
		{
			psnUserPrev = psnUser;
			pUserStyleData = lstStyleUserNew.GetNext( psnUser);
			if( !pUserStyleData) continue;
			bFound = ( pDefaultStyleData->GetStyleName() == pUserStyleData->GetStyleName());
		}

		if(	bFound)
		{
			m_lstStyleUser.AddTail( pUserStyleData);
			lstStyleUserNew.RemoveAt( psnUserPrev);
		}
		else m_lstStyleUser.AddTail( new CStyleData( *pDefaultStyleData));
	}

	m_lstStyleUser.AddTail( &lstStyleUserNew);

	CString strStyle;
	strStyle.LoadString(IDS_CURRENT_STYLE);
	RemoveStyleData( strStyle);
	CStyleData *pStyleData = new CStyleData;
	GetStyleDataFromOCX( *pStyleData, p_pIChartOCX);
	AddStyleData( pStyleData);
}

void CStyleDataManager::RemoveDummyUserStyle( void)
{
	//m_styleDataManager.RemoveStyleData( "현화면스타일");
	CString strStyle;
	strStyle.LoadString(IDS_CURRENT_STYLE);
	RemoveStyleData( strStyle);

	CString strStyleName;
	POSITION posUser = m_lstStyleUser.GetHeadPosition();
	while( posUser != NULL)
	{
		POSITION curPosUser = posUser;
		CStyleData* pUserStyleData = m_lstStyleUser.GetNext( posUser);
		strStyleName = pUserStyleData->GetStyleName();

		POSITION posDefault = m_lstStyleDefault.GetHeadPosition();
		while( !strStyleName.IsEmpty() && posDefault)
		{
			CStyleData* pDefaultStyleData = m_lstStyleDefault.GetNext( posDefault);
			if( pDefaultStyleData->IsSameWith( *pUserStyleData))
			{
				delete pUserStyleData;
				m_lstStyleUser.RemoveAt( curPosUser);
				break;
			}
		}
	}
}

// (2006/8/20 - Seung-Won, Bae) Get Style Data form OCX
void CStyleDataManager::GetStyleDataFromOCX( CStyleData &p_styleData, IChartOCX *p_pIChartOCX, BOOL p_bWithIndex)
{
// 2008.01.25 by LYH >>
	//p_styleData.SetStyleName("현화면스타일");
	CString strStyle;
	strStyle.LoadString(IDS_CURRENT_STYLE);
	p_styleData.SetStyleName(strStyle);
// 2008.01.25 by LYH <<

	// Graph Region Color
	p_styleData.SetGraphBGColor(		p_pIChartOCX->GetStyleValueGraphBGColor());
	p_styleData.SetGraphBorderColor(	p_pIChartOCX->GetStyleValueGraphBorderColor());
	p_styleData.SetGraphGridColor(		p_pIChartOCX->GetStyleValueGraphGridColor());
	p_styleData.SetGridHeight(			p_pIChartOCX->GetStyleValueGridHeight());
	p_styleData.SetGridWidth(			p_pIChartOCX->GetStyleValueGridWidth());
	p_styleData.SetScaleGridColor(		p_pIChartOCX->GetStyleValueGridHLineColor());
	p_styleData.SetScaleGridColor(		p_pIChartOCX->GetStyleValueGridVLineColor());
	p_styleData.SetScaleTextColor(		p_pIChartOCX->GetStyleValueScaleTextColor());
	//scale type
	p_styleData.SetDivideWidth(			p_pIChartOCX->GetFirstBlockHGridLineType());
	p_styleData.SetDivideHeight(		p_pIChartOCX->GetFirstBlockVGridLineType());

	//scale 관련 색 : Update in OnDraw
	p_styleData.SetScaleBGColor(		p_pIChartOCX->GetStyleValueScaleBGColor());
	p_styleData.SetScaleBorderColor(	p_pIChartOCX->GetStyleValueScaleBorderColor());

	//KHD : 시간구분
	//타입 
	p_styleData.SetHScaleGridType_TimeGubun(		p_pIChartOCX->GetFirstBlockHScaleGridType_TimeGubun(CScaleBaseData::MINTICK_DAY_GUBUN), CScaleBaseData::MINTICK_DAY_GUBUN);
	p_styleData.SetHScaleGridType_TimeGubun(		p_pIChartOCX->GetFirstBlockHScaleGridType_TimeGubun(CScaleBaseData::DAYCHART_WEEK_GUBUN), CScaleBaseData::DAYCHART_WEEK_GUBUN);
	p_styleData.SetHScaleGridType_TimeGubun(		p_pIChartOCX->GetFirstBlockHScaleGridType_TimeGubun(CScaleBaseData::DAYCHART_MON_GUBUN), CScaleBaseData::DAYCHART_MON_GUBUN);
	p_styleData.SetHScaleGridType_TimeGubun(		p_pIChartOCX->GetFirstBlockHScaleGridType_TimeGubun(CScaleBaseData::DAYCHART_YEAR_GUBUN), CScaleBaseData::DAYCHART_YEAR_GUBUN);

	//컬러
	p_styleData.SetHScaleGridColor_TimeGubun(		p_pIChartOCX->GetStyleValueHScaleGridColor_TimeGubun(CScaleBaseData::MINTICK_DAY_GUBUN), CScaleBaseData::MINTICK_DAY_GUBUN);
	p_styleData.SetHScaleGridColor_TimeGubun(		p_pIChartOCX->GetStyleValueHScaleGridColor_TimeGubun(CScaleBaseData::DAYCHART_WEEK_GUBUN), CScaleBaseData::DAYCHART_WEEK_GUBUN);
	p_styleData.SetHScaleGridColor_TimeGubun(		p_pIChartOCX->GetStyleValueHScaleGridColor_TimeGubun(CScaleBaseData::DAYCHART_MON_GUBUN), CScaleBaseData::DAYCHART_MON_GUBUN);
	p_styleData.SetHScaleGridColor_TimeGubun(		p_pIChartOCX->GetStyleValueHScaleGridColor_TimeGubun(CScaleBaseData::DAYCHART_YEAR_GUBUN), CScaleBaseData::DAYCHART_YEAR_GUBUN);

	//굵기
	p_styleData.SetScaleLineThickness_TimeGubun(	CScaleBaseData::MINTICK_DAY_GUBUN, p_pIChartOCX->GetStyleValueScaleLineThickness_TimeGubun(CScaleBaseData::MINTICK_DAY_GUBUN));
	p_styleData.SetScaleLineThickness_TimeGubun(	CScaleBaseData::DAYCHART_WEEK_GUBUN, p_pIChartOCX->GetStyleValueScaleLineThickness_TimeGubun(CScaleBaseData::DAYCHART_WEEK_GUBUN));
	p_styleData.SetScaleLineThickness_TimeGubun(	CScaleBaseData::DAYCHART_MON_GUBUN, p_pIChartOCX->GetStyleValueScaleLineThickness_TimeGubun(CScaleBaseData::DAYCHART_MON_GUBUN));
	p_styleData.SetScaleLineThickness_TimeGubun(	CScaleBaseData::DAYCHART_YEAR_GUBUN, p_pIChartOCX->GetStyleValueScaleLineThickness_TimeGubun(CScaleBaseData::DAYCHART_YEAR_GUBUN));

	//Analysis tool	: Update in EAI_ALL_APPLY_STYLE
	p_styleData.SetAnalyzeColor(		p_pIChartOCX->GetStyleValueAnalysisToolColor());
	// 수치조회창.	: Update in EAI_ALL_APPLY_STYLE
	p_styleData.SetNumericalColor(		p_pIChartOCX->GetStyleValueNIDLGColor());

	if( !p_bWithIndex) return;

	IGraphInfoManager *pIGraphInfoManager = p_pIChartOCX->GetIGraphInfoManager();
	if( !pIGraphInfoManager) return;
	IChartManager *pIChartManager = p_pIChartOCX->GetIChartManager();
	if( !pIChartManager)
	{
		pIGraphInfoManager->Release();
		return;
	}

	// (2009/9/22 - Seung-Won, Bae) for Selected Main Item.
	ILPCSTR szRQ = p_pIChartOCX->GetCurrentRQ();
	int nR = -1, nC = -1, nV = -1, nG = -1;
	IGraphInfo *pIGraphInfo = NULL;
	// Price Chart
	ILPCSTR szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_CHART), nR, nC, nV, nG, szRQ);
	if( szGraphName.IsEmpty())
			pIGraphInfo = pIGraphInfoManager->GetIndicatorInfo( IGraphInfoManager::IEILT_SITE_DEFAULT, _MTEXT( C2_PRICE_CHART), IGraphInfoManager::IEIIGT_WITH_DEFAULT_GET);
	else	pIGraphInfo = pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szGraphName);
	if( pIGraphInfo)
	{
		COLORREF clrPriceUp1 = 0, clrPriceUp2 = 0, clrPriceDown1 = 0, clrPriceDown2 = 0, clrPriceEqual1 = 0, clrPriceEqual2 = 0;
		BOOL bPriceUpFill = FALSE, bPriceDownFill = FALSE;
		pIGraphInfo->GetSubGraphColorAndFillFlag( 0, clrPriceUp1, clrPriceUp2, clrPriceDown1, clrPriceDown2, clrPriceEqual1, clrPriceEqual2, bPriceUpFill, bPriceDownFill);
		p_styleData.SetPriceUpColor(		clrPriceUp1);
		p_styleData.SetPriceDownColor(		clrPriceDown1);
		p_styleData.SetPriceEqualColor(	clrPriceEqual1);
		p_styleData.SetPriceUpFill(		bPriceUpFill ? true : false);
		p_styleData.SetPriceDownFill(		bPriceDownFill ? true : false);
		pIGraphInfo->Release();
	}
	// PriceMA Chart
	szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_PRICE_MA), nR, nC, nV, nG, szRQ);
	if( szGraphName.IsEmpty())
			pIGraphInfo = pIGraphInfoManager->GetIndicatorInfo( IGraphInfoManager::IEILT_SITE_DEFAULT, _MTEXT( C2_PRICE_MA), IGraphInfoManager::IEIIGT_WITH_DEFAULT_GET);
	else	pIGraphInfo = pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szGraphName);
	if( pIGraphInfo)
	{
		int nMACount = pIGraphInfo->GetSubGraphCount();
		for( int i = 0; i < nMACount; i++) p_styleData.SetPriceMAColor( i, pIGraphInfo->GetSubGraphColor( i, 1));

		CList<CString, CString> *pTitleList = p_styleData.GetMATitleListData().GetPriceMATitleList_Pointer();
		ILPCSTR szTitle = NULL;
		for(int  i = 0; i < nMACount; i++) 
		{
			szTitle = pIGraphInfo->GetSubGraphTitleString( i);
			pTitleList->AddTail( ( const char *)szTitle);
		}

		pIGraphInfo->Release();
	}
	// Volume Chart
	szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_VOLUME_CHART), nR, nC, nV, nG, szRQ);
	if( szGraphName.IsEmpty())
			pIGraphInfo = pIGraphInfoManager->GetIndicatorInfo( IGraphInfoManager::IEILT_SITE_DEFAULT, _MTEXT( C2_VOLUME_CHART), IGraphInfoManager::IEIIGT_WITH_DEFAULT_GET);
	else	pIGraphInfo = pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, szGraphName);
	if( pIGraphInfo)
	{
		COLORREF clrTradeUp1 = 0, clrTradeUp2 = 0, clrTradeDown1 = 0, clrTradeDown2 = 0, clrTradeEqual1 = 0, clrTradeEqual2 = 0;
		BOOL bTradeUpFill = FALSE, bTradeDownFill = FALSE;
		pIGraphInfo->GetSubGraphColorAndFillFlag( 0, clrTradeUp1, clrTradeUp2, clrTradeDown1, clrTradeDown2, clrTradeEqual1, clrTradeEqual2, bTradeUpFill, bTradeDownFill);
		p_styleData.SetTradeUpColor(		clrTradeUp1);	
		p_styleData.SetTradeDownColor(		clrTradeDown1);
		p_styleData.SetTradeEqualColor(		clrTradeEqual1);
		p_styleData.SetTradeUpFill(			bTradeUpFill ? true : false);
		p_styleData.SetTradeDownFill(		bTradeDownFill ? true : false);
		pIGraphInfo->Release();
	}
	// VolumeMA Chart
	szGraphName = pIChartManager->FindFirstIndicator( _MTEXT( C2_VOLUME_MA), nR, nC, nV, nG, szRQ);
	if( szGraphName.IsEmpty())
			pIGraphInfo = pIGraphInfoManager->GetIndicatorInfo( IGraphInfoManager::IEILT_SITE_DEFAULT, _MTEXT( C2_VOLUME_MA), IGraphInfoManager::IEIIGT_WITH_DEFAULT_GET);
	else	pIGraphInfo = pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, _MTEXT( C2_VOLUME_MA ));
	if( pIGraphInfo)
	{
		int nMACount = pIGraphInfo->GetSubGraphCount();
		for( int i = 0; i < nMACount; i++) p_styleData.SetTradeMAColor( i, pIGraphInfo->GetSubGraphColor( i, 1));

		CList<CString, CString> *pTitleList = p_styleData.GetMATitleListData().GetTradeMATitleList_Pointer();
		ILPCSTR szTitle = NULL;
		for(int i = 0; i < nMACount; i++) 
		{
			szTitle = pIGraphInfo->GetSubGraphTitleString( i);
			pTitleList->AddTail( ( const char *)szTitle);
		}

		pIGraphInfo->Release();
	}
	pIChartManager->Release();
	pIGraphInfoManager->Release();
}

void CStyleDataManager::DrawSampleImage( CDC *pDC, const CRect p_rctArea, CStyleData *p_pStyleData, CChartInfo::CHART_MODE p_eChartMode, CFont *p_pTitleFont, HWND p_hOcxWnd, int p_nDrawCount)
{
	// 차트크기
	CRect rectScale = p_rctArea; 
	CRect rectPrice( p_rctArea.left, p_rctArea.top, p_rctArea.right - 37, p_rctArea.top + int( double( p_rctArea.Height() - 20) / 257 * 170));
	CRect rectTrade( p_rctArea.left, rectPrice.bottom, rectPrice.right, p_rctArea.bottom - 20);

	// 가격 { 시가, 종가, 고가, 저가 }
	int	price[SAMPLE_BONG_COUNT][4] =
	{
		{ 93, 102, 91, 98 }, 
		{ 103, 109, 90, 106 }, 
		{ 102, 109, 97, 101 }, 
		{ 100, 104, 85, 92 }, 
		{ 100, 109, 95, 99 }, 
		{ 92, 96, 86, 91 }, 
		{ 67, 99, 64, 70 }, 
		{ 63, 70, 58, 67 }, 
		{ 69, 75, 60, 64 }, 
		{ 84, 85, 67, 75 }, 
		{ 91, 93, 83, 89 }, 
		{ 94, 97, 89, 94 }, 
		{ 106, 111, 88, 109 }, 
		{ 117, 118, 104, 112 }, 
		{ 115, 122, 113, 117 }, 
		{ 120, 120, 102, 116 }, 
		{ 119, 121, 110, 116 }, 
		{ 132, 134, 120, 128 }, 
		{ 134, 138, 129, 136 }, 
		{ 134, 140, 130, 132 }, 
		{ 127, 132, 116, 125 },
		{ 117, 118, 104, 112 }, 
		{ 106, 111, 88, 109 }, 
		{ 93, 102, 91, 98 }, 
		{ 103, 109, 90, 106 }, 
		{ 102, 109, 97, 101 }, 
		{ 100, 109, 95, 99 }, 
		{ 67, 99, 64, 70 }, 
		{ 63, 70, 58, 67 }, 
		{ 69, 75, 60, 64 }, 
		{ 84, 85, 67, 75 }, 
		{ 91, 93, 83, 89 }, 
		{ 94, 97, 89, 94 }, 
		{ 106, 111, 88, 109 }, 
		{ 117, 118, 104, 112 }, 
		{ 115, 122, 113, 117 }, 
		{ 120, 120, 102, 116 }, 
		{ 119, 121, 110, 116 }, 
		{ 132, 134, 120, 128 }, 
		{ 134, 138, 129, 136 }, 
		{ 134, 140, 130, 132 }, 
		{ 127, 132, 116, 125 }
	};

	// 가격 이동평균
	int pAverage[10][SAMPLE_BONG_COUNT] =
	{
		{ 71,  82, 91, 97, 99, 98, 91, 84, 78, 73, 73, 78, 86, 96, 104, 109, 114, 118, 123, 126, 128, 71,  82, 91, 97, 99, 98, 91, 84, 78, 73, 73, 78, 86, 96, 104, 109, 114, 118, 123, 126, 128 },
		{ 68, 73, 76, 77, 80, 85, 86, 88, 88, 86, 85, 84, 85, 87, 89, 91, 96, 102, 109, 115, 119, 68, 73, 76, 77, 80, 85, 86, 88, 88, 86, 85, 84, 85, 87, 89, 91, 96, 102, 109, 115, 119 },
		{ 71, 74, 76, 77, 79, 79, 78, 78, 77, 77, 77, 79, 80, 82, 84, 88, 91, 95, 98, 101, 102, 71, 74, 76, 77, 79, 79, 78, 78, 77, 77, 77, 79, 80, 82, 84, 88, 91, 95, 98, 101, 102 },
		{ 56, 60, 63, 66, 69, 71, 73, 74, 74, 75, 76, 78, 79, 80, 82, 83, 84, 86, 88, 89, 91, 56, 60, 63, 66, 69, 71, 73, 74, 74, 75, 76, 78, 79, 80, 82, 83, 84, 86, 88, 89, 91 },
		{ 32, 35, 38, 41, 44, 48, 51, 54, 57, 60, 63, 66, 69, 71, 74, 76, 78, 81, 83, 85, 87, 32, 35, 38, 41, 44, 48, 51, 54, 57, 60, 63, 66, 69, 71, 74, 76, 78, 81, 83, 85, 87 },
		{ 23, 25, 27, 29, 31, 34, 36, 38, 39, 41, 43, 45, 47, 50, 53, 56, 60, 64, 67, 71, 74, 23, 25, 27, 29, 31, 34, 36, 38, 39, 41, 43, 45, 47, 50, 53, 56, 60, 64, 67, 71, 74 },
		{ 23, 24, 25, 26, 27, 29, 30, 30, 31, 32, 33, 35, 37, 39, 41, 43, 46, 48, 51, 53, 56, 23, 24, 25, 26, 27, 29, 30, 30, 31, 32, 33, 35, 37, 39, 41, 43, 46, 48, 51, 53, 56 },
		{ 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 32, 34, 35, 36, 38, 39, 40, 42, 44, 45, 21, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 32, 34, 35, 36, 38, 39, 40, 42, 44, 45 },
		{ 16, 18, 19, 21, 22, 23, 24, 25, 26, 28, 29, 30, 31, 33, 34, 35, 37, 38, 40, 41, 42, 16, 18, 19, 21, 22, 23, 24, 25, 26, 28, 29, 30, 31, 33, 34, 35, 37, 38, 40, 41, 42 },
		{ 10, 12, 14, 15, 17, 19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 31, 32, 34, 36, 37, 39, 10, 12, 14, 15, 17, 19, 20, 21, 22, 23, 24, 25, 27, 28, 29, 31, 32, 34, 36, 37, 39 }
	};


	// 거래량
	int	trade[SAMPLE_BONG_COUNT] = { 32, 25, 21, 19, 18, 17, 23, 19, 18, 22, 22, 24, 19, 22, 19, 18, 18, 17, 18, 17, 18,
					  32, 25, 21, 19, 18, 17, 23, 19, 18, 22, 22, 24, 19, 22, 19, 18, 18, 17, 18, 17, 18 };

	// 거래 이동평균
	int tAverage[6][SAMPLE_BONG_COUNT] =
	{
		{ 56, 48, 26, 22, 19, 18, 19, 20, 20, 20, 21, 23, 22, 22, 20, 20, 18, 18, 18, 18, 18, 56, 48, 26, 22, 19, 18, 19, 20, 20, 20, 21, 23, 22, 22, 20, 20, 18, 18, 18, 18, 18 },
		{ 36, 38, 38, 39, 34, 22, 21, 20, 19, 20, 20, 21, 21, 21, 21, 21, 20, 19, 19, 18, 18, 36, 38, 38, 39, 34, 22, 21, 20, 19, 20, 20, 21, 21, 21, 21, 21, 20, 19, 19, 18, 18 },
		{ 30, 30, 31, 31, 32, 32, 32, 29, 21, 20, 20, 20, 20, 21, 21, 20, 20, 20, 20, 19, 19, 30, 30, 31, 31, 32, 32, 32, 29, 21, 20, 20, 20, 20, 21, 21, 20, 20, 20, 20, 19, 19 },
		{ 27, 28, 28, 28, 28, 28, 28, 29, 29, 29, 27, 22, 21, 20, 20, 20, 20, 20, 20, 20, 20, 27, 28, 28, 28, 28, 28, 28, 29, 29, 29, 27, 22, 21, 20, 20, 20, 20, 20, 20, 20, 20 },
		{ 25, 26, 26, 26, 26, 26, 26, 26, 26, 27, 27, 28, 28, 26, 21, 20, 20, 20, 20, 20, 20, 25, 26, 26, 26, 26, 26, 26, 26, 26, 27, 27, 28, 28, 26, 21, 20, 20, 20, 20, 20, 20 },
		{ 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 25, 21, 20, 20, 19, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 25, 21, 20, 20, 19 }
	};

	// (2009/9/21 - Seung-Won, Bae) for FX
	double dFactor = double( p_rctArea.Height() - 20) / 257;
	if( CChartInfo::FX == p_eChartMode)
	{
		rectTrade.top = rectTrade.bottom;
		rectPrice.bottom = rectTrade.bottom;
		dFactor = double( p_rctArea.Height() - 20) / 170;
	}

	for( int i = 0; i < SAMPLE_BONG_COUNT; i++)
	{
		for( int j = 0; j < 10; j++)
		{
			if( j < 1) trade[ i] = int( dFactor * trade[ i]);
			if( j < 4) price[ i][ j] = int( dFactor * price[ i][ j]);
			if( j < 6) tAverage[ j][ i] = int( dFactor * tAverage[ j][ i]);
			pAverage[ j][ i] = int( dFactor * pAverage[ j][ i]);
		}
	}

	DrawScale( pDC, rectScale, rectPrice, rectTrade, p_pStyleData, p_pTitleFont, p_eChartMode, dFactor);
	DrawGraph( pDC,  rectPrice, rectTrade, p_pStyleData, p_eChartMode);
	DrawGraphGrid( pDC,  rectPrice, rectTrade, p_pStyleData);
	DrawScaleGrid( pDC,  rectPrice, rectTrade, p_pStyleData, p_eChartMode, dFactor);
	DrawTitle( pDC,  rectPrice, rectTrade, p_pStyleData, p_pTitleFont, p_hOcxWnd, p_eChartMode);

	//sy 2005.04.19.
	DrawBong( pDC,  rectPrice, price,
			p_pStyleData->GetPriceUpColor(), p_pStyleData->GetPriceDownColor(), p_pStyleData->GetPriceEqualColor(), 
			p_pStyleData->GetPriceUpFill(), p_pStyleData->GetPriceDownFill(), p_nDrawCount);
	// (2009/9/21 - Seung-Won, Bae) for FX
	if( CChartInfo::FX != p_eChartMode)
		DrawBar( pDC,  rectTrade, trade,
				p_pStyleData->GetTradeUpColor(), p_pStyleData->GetTradeDownColor(), p_pStyleData->GetTradeEqualColor(),
				p_pStyleData->GetTradeUpFill(), p_pStyleData->GetTradeDownFill(), p_nDrawCount);

	int nMACount = p_pStyleData->GetPriceMACount();
	if( 10 < nMACount) nMACount = 10;
	for(int i = 0; i < nMACount; i++) DrawLine( pDC, rectPrice, pAverage[ i], p_pStyleData->GetPriceMAColor( i), p_nDrawCount);

	// (2009/9/21 - Seung-Won, Bae) for FX
	if( CChartInfo::FX != p_eChartMode)
	{
		nMACount = p_pStyleData->GetTradeMACount();
		if( 6 < nMACount) nMACount = 6;
		for(int i = 0; i < nMACount; i++) DrawLine( pDC, rectTrade, tAverage[ i], p_pStyleData->GetTradeMAColor( i), p_nDrawCount);
	}
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 22
// Return void : 
// Param  CDC *pDC : 
// Param  CRect rect1 : 
// Param  CRect rect2 : 
// Param  CRect rect3 : 
// Param  COLORREF bgColor : 
// Param  COLORREF borderColor : 
// Param  COLORREF textColor : 
// Comments		: 
//-----------------------------------------------------------------------------
void CStyleDataManager::DrawScale( CDC *pDC, CRect rect1, CRect rect2, CRect rect3,
									  CStyleData *p_pStyleData, CFont *p_pTitleFont, CChartInfo::CHART_MODE p_eChartMode, double p_dFactor)
{
	COLORREF bgColor		= p_pStyleData->GetScaleBGColor();
	COLORREF borderColor	= p_pStyleData->GetScaleBorderColor();
	COLORREF textColor		= p_pStyleData->GetScaleTextColor();

	CPen pen( PS_SOLID, 1, borderColor );
	CBrush brush( bgColor );
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	
	DrawRect(pDC, rect1);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pen.DeleteObject();
	brush.DeleteObject();

	// 1. 사용할 폰트를 만든다.
	CFont *pOldFont = pDC->GetCurrentFont();
	if( p_pTitleFont != NULL )
		pDC->SelectObject( p_pTitleFont);

	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF ordColor = pDC->GetTextColor();
	pDC->SetTextColor( textColor );

	// 가로 스케일
	pDC->TextOut( rect1.left + 2, rect1.bottom - 14, "10" );
	pDC->TextOut( rect1.left + 73, rect1.bottom - 14, "11" );

	// 세로 스케일
	int	correct = 5;
	// (2009/9/21 - Seung-Won, Bae) for FX
	pDC->TextOut( rect2.right + 3, rect2.bottom - int( p_dFactor * 140) - correct, "3,800" );
	pDC->TextOut( rect2.right + 3, rect2.bottom - int( p_dFactor * 120) - correct, "3,700" );
	pDC->TextOut( rect2.right + 3, rect2.bottom - int( p_dFactor * 100) - correct, "3,600" );
	pDC->TextOut( rect2.right + 3, rect2.bottom - int( p_dFactor * 80) - correct, "3,500" );
	pDC->TextOut( rect2.right + 3, rect2.bottom - int( p_dFactor * 60) - correct, "3,400" );
	pDC->TextOut( rect2.right + 3, rect2.bottom - int( p_dFactor * 40) - correct, "3,300" );
	pDC->TextOut( rect2.right + 3, rect2.bottom - int( p_dFactor * 20) - correct, "3,200" );

	// (2009/9/21 - Seung-Won, Bae) for FX
	if( CChartInfo::FX != p_eChartMode)
	{
		pDC->TextOut( rect3.right + 3, rect3.bottom - int( p_dFactor * 60) - correct, "1,500" );
		pDC->TextOut( rect3.right + 3, rect3.bottom - int( p_dFactor * 40) - correct, "1,000" );
		pDC->TextOut( rect3.right + 3, rect3.bottom - int( p_dFactor * 20) - correct, "  500" );
		pDC->TextOut( rect3.right + 3, rect3.bottom - int( p_dFactor * 5) - correct, "X100" );
	}

	pDC->SetTextColor( ordColor );
	pDC->SetBkMode( nOldMode );
	pDC->SelectObject( pOldFont );
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 22
// Return void : 
// Param  CDC *pDC : 
// Param  CRect rect1 : 
// Param  CRect rect2 : 
// Param  COLORREF bgColor : 
// Param  COLORREF borderColor : 
// Comments		: 
//-----------------------------------------------------------------------------
void CStyleDataManager::DrawGraph( CDC *pDC, CRect rect1, CRect rect2, CStyleData *p_pStyleData, CChartInfo::CHART_MODE p_eChartMode)
{
	COLORREF bgColor		= p_pStyleData->GetGraphBGColor();
	COLORREF borderColor	= p_pStyleData->GetGraphBorderColor();

	CPen pen(PS_SOLID, 1, borderColor);
	CBrush brush(bgColor);
	CPen* pOldPen = pDC->SelectObject(&pen);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	// 1. 2개의 영역이 서로 겹치게 사각형을 그린다.
	rect1.bottom += 1; 
	DrawRect( pDC, rect1 );
	// (2009/9/21 - Seung-Won, Bae) for FX
	if( CChartInfo::FX != p_eChartMode)
		DrawRect( pDC, rect2 );

	pDC->SelectObject( pOldPen );
	pDC->SelectObject( pOldBrush );
	pen.DeleteObject();
	brush.DeleteObject();
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 22
// Return void : 
// Param  CDC *pDC : 
// Param  CRect rect1 : 
// Param  CRect rect2 : 
// Param  COLORREF color : 
// Comments		: 
//-----------------------------------------------------------------------------
void CStyleDataManager::DrawGraphGrid( CDC *pDC, CRect rect1, CRect rect2, CStyleData *p_pStyleData)
{
	COLORREF color = p_pStyleData->GetGraphGridColor();

	CPen pen( PS_SOLID, 1, color );
	CPen *pOldPen = pDC->SelectObject( &pen );
	int left = 0;
	// 1. 가로 grid
	if( p_pStyleData->GetGridWidth() > 0 )
	{
		for( left = rect1.left + p_pStyleData->GetGridWidth() ; left < rect1.right ; left += p_pStyleData->GetGridWidth() )
		{
			pDC->MoveTo( left, rect1.top + 1 );
			pDC->LineTo( left, rect1.bottom + 1 );
		}
		left = rect2.left + p_pStyleData->GetGridWidth();

		for( ; left < rect2.right ; left += p_pStyleData->GetGridWidth() )
		{
			pDC->MoveTo( left, rect2.top + 1 );
			pDC->LineTo( left, rect2.bottom );
		}
	}

	// 2. 세로 grid
	if( p_pStyleData->GetGridHeight() > 0 )
	{
		int top = rect1.top + p_pStyleData->GetGridHeight();
		for( ; top < rect1.bottom ; top += p_pStyleData->GetGridHeight() )
		{
			pDC->MoveTo( rect1.left +1, top );
			pDC->LineTo( rect1.right - 1, top );
		}
		top = rect2.top + p_pStyleData->GetGridHeight();
		
		for( ; top < rect2.bottom ; top += p_pStyleData->GetGridHeight() )
		{
			pDC->MoveTo( rect2.left + 1, top );
			pDC->LineTo( rect2.right - 1, top );
		}
	}

	pDC->SelectObject( pOldPen );
	pen.DeleteObject();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 22
// Return void : 
// Param  CDC *pDC : 
// Param  CRect rect1 : 
// Param  CRect rect2 : 
// Param  COLORREF color : 
// Comments		: 
//-----------------------------------------------------------------------------
void CStyleDataManager::DrawScaleGrid( CDC *pDC, CRect rect1, CRect rect2, CStyleData *p_pStyleData, CChartInfo::CHART_MODE p_eChartMode, double p_dFactor)
{
	COLORREF color = p_pStyleData->GetScaleGridColor();

	CPen pen( PS_SOLID, 1, color );
	CPen* pOldPen = pDC->SelectObject( &pen );

	int	i, j, len, gap;
	int	left, bottom;
	
	if( ( p_pStyleData->GetDivideHeight() >=0 ) && ( p_pStyleData->GetDivideHeight() < 3 ) )
	{
		if( p_pStyleData->GetDivideHeight() == 0 )
		{
			pDC->MoveTo( rect1.left + 73, rect1.bottom - 1 );
			pDC->LineTo( rect1.left + 73, rect1.top + 1 );
			pDC->MoveTo( rect2.left + 73, rect2.bottom - 1 );
			pDC->LineTo( rect2.left + 73, rect2.top + 1 );
		}
		
		else if( p_pStyleData->GetDivideHeight() == 1 )
		{
			len = 3;
			gap = 3;
			
			for( bottom = rect1.bottom - 1 ; bottom > rect1.top + 1 ; bottom -= (len + gap) )
			{
				pDC->MoveTo( rect1.left + 73, bottom );
				if( ( bottom - len ) < ( rect1.top + 1 ) )
					pDC->LineTo( rect1.left + 73, rect1.top + 1 );
				
				else
					pDC->LineTo( rect1.left + 73, bottom - len );
			}

			for( bottom = rect2.bottom - 1 ; bottom > rect2.top + 1 ; bottom -= ( len + gap ))
			{
				pDC->MoveTo( rect2.left + 73, bottom );
				if( ( bottom - len ) < ( rect2.top + 1 ) )
					pDC->LineTo( rect2.left + 73, rect2.top + 1 );
				
				else
					pDC->LineTo( rect2.left + 73, bottom - len );
			}
		}
		
		else if( p_pStyleData->GetDivideHeight() == 2 )
		{
			len = 1;
			gap = 3;
			
			for( bottom = rect1.bottom - 1 ; bottom > rect1.top + 1 ; bottom -= ( len + gap ) )
			{
				pDC->MoveTo( rect1.left + 73, bottom );
				
				if( ( bottom - len ) < ( rect1.top + 1 ) )
					pDC->LineTo( rect1.left + 73, rect1.top + 1 );
				
				else
					pDC->LineTo( rect1.left + 73, bottom - len );
			}
			
			for( bottom = rect2.bottom - 1 ; bottom > rect2.top + 1 ; bottom -= ( len + gap ) )
			{
				pDC->MoveTo( rect2.left + 73, bottom );
				
				if( ( bottom - len ) < ( rect2.top + 1 ) )
					pDC->LineTo( rect2.left + 73, rect2.top + 1 );
				
				else
					pDC->LineTo( rect2.left + 73, bottom - len );
			}
		}
	}

	// 스케일 세로 구분선
	if( ( p_pStyleData->GetDivideWidth() >=0 ) && ( p_pStyleData->GetDivideWidth() < 3 ) )
	{
		if( p_pStyleData->GetDivideWidth() == 0 )
		{
			for( i = int( p_dFactor * 20); i < int( p_dFactor * 155); i += int( p_dFactor * 20))
			{
				pDC->MoveTo( rect1.left + 1, rect1.bottom - i );
				pDC->LineTo( rect1.right - 1, rect1.bottom - i );
			}

			// (2009/9/21 - Seung-Won, Bae) for FX
			if( CChartInfo::FX != p_eChartMode)
				for( j = int( p_dFactor * 20) ; j < int( p_dFactor * 75) ; j += int( p_dFactor * 20))
			{
				pDC->MoveTo( rect2.left + 1, rect2.bottom - j );
				pDC->LineTo( rect2.right - 1, rect2.bottom - j );
			}
		}

		else if( p_pStyleData->GetDivideWidth() == 1 )
		{
			len = 3;
			gap = 3;
			
			for( i = int( p_dFactor * 20); i < int( p_dFactor * 155); i += int( p_dFactor * 20))
			{
				for( left = rect1.left + 1 ; left < rect1.right - 1 ; left += ( len + gap ) )
				{
					pDC->MoveTo( left, rect1.bottom - i );
					
					if( ( left + len ) > ( rect1.right - 1 ) )
						pDC->LineTo( rect1.right - 1, rect1.bottom -i );
					
					else
						pDC->LineTo( left + len, rect1.bottom - i );
				}
			}

			// (2009/9/21 - Seung-Won, Bae) for FX
			if( CChartInfo::FX != p_eChartMode)
				for( j = int( p_dFactor * 20) ; j < int( p_dFactor * 75) ; j += int( p_dFactor * 20))
			{
				for( left = rect2.left + 1 ; left < rect2.right - 1 ; left += ( len + gap ) )
				{
					pDC->MoveTo( left, rect2.bottom - j );
				
					if( ( left + len ) > ( rect2.right - 1 ) )
						pDC->LineTo( rect2.right - 1, rect2.bottom - j );
				
					else
						pDC->LineTo( left + len, rect2.bottom - j );
				}
			}
		}
		
		else if( p_pStyleData->GetDivideWidth() == 2 )
		{
			len = 1;
			gap = 3;
			
			for( i = int( p_dFactor * 20); i < int( p_dFactor * 155); i += int( p_dFactor * 20))
			{
				for( left = rect1.left + 1 ; left < rect1.right - 1 ; left += ( len + gap ) )
				{
					pDC->MoveTo( left, rect1.bottom - i );
					
					if( ( left + len ) > ( rect1.right - 1 ) )
						pDC->LineTo( rect1.right - 1, rect1.bottom - i );
				
					else
						pDC->LineTo( left + len, rect1.bottom - i );
				}
			}

			// (2009/9/21 - Seung-Won, Bae) for FX
			if( CChartInfo::FX != p_eChartMode)
				for( j = int( p_dFactor * 20) ; j < int( p_dFactor * 75) ; j += int( p_dFactor * 20))
			{
				for( left = rect2.left + 1 ; left < rect2.right - 1 ; left += ( len + gap ) )
				{
					pDC->MoveTo( left, rect2.bottom - j );
					
					if( ( left + len ) > ( rect2.right - 1 ) )
						pDC->LineTo( rect2.right - 1, rect2.bottom - j );
					
					else
						pDC->LineTo( left + len, rect2.bottom - j );
				}
			}
		}
	}

	pDC->SelectObject( pOldPen );
	pen.DeleteObject();
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Param  CPaintDC *pDC : 
// Param  CRect rect1 : 
// Param  CRect rect2 : 
// Param  COLORREF bgColor : 
// Param  COLORREF borderColor : 
// Comments		: 
//-----------------------------------------------------------------------------
void CStyleDataManager::DrawTitle( CDC *pDC, CRect rect1, CRect rect2, CStyleData *p_pStyleData, CFont *p_pTitleFont, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode)
{
	COLORREF bgColor		= p_pStyleData->GetGraphBGColor();
	COLORREF borderColor	= p_pStyleData->GetGraphBorderColor();

	CFont* pOldFont = pDC->GetCurrentFont();
	if( p_pTitleFont != NULL)
		pDC->SelectObject(p_pTitleFont);
	//sy end
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF ordColor = pDC->GetTextColor();

	_MHWND( p_hOcxWnd);
	pDC->SetTextColor( p_pStyleData->GetPriceUpColor());
	pDC->TextOut( rect1.left + 4, rect1.top + 4, _LTEXT3(C3_PRICE));
	// (2009/9/21 - Seung-Won, Bae) for FX
	if( CChartInfo::FX != p_eChartMode)
	{
		pDC->SetTextColor(p_pStyleData->GetTradeUpColor());
		pDC->TextOut(rect2.left + 4, rect2.top + 4, _LTEXT3(C3_VOLUME));
	}

	pDC->SetTextColor(ordColor);
	pDC->SetBkMode(nOldMode);
	pDC->SelectObject(pOldFont);
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 22
// Return void : 
// Param  CDC *pDC : 
// Param  CRect rect : 
// Param  int : 
// Comments		: 
//-----------------------------------------------------------------------------
void CStyleDataManager::DrawBong( CDC *pDC, CRect rect, int (*price)[4], COLORREF upColor, COLORREF downColor, COLORREF equalColor, BOOL bUpFill, BOOL bDownFill, int p_nDrawCount)
{
	CPen upPen( PS_SOLID, 1, upColor ), downPen( PS_SOLID, 1, downColor ), equalPen( PS_SOLID, 1, equalColor );
	CPen *pOldPen = ( CPen * )pDC->SelectStockObject( NULL_PEN );
	CBrush upBrush( upColor ), downBrush( downColor );
	CBrush *pOldBrush = ( CBrush * )pDC->SelectStockObject( NULL_BRUSH );

	int	nWidth = 5, nGap = 1;
	int nOrgPos_y = rect.bottom;
	int nPrevEnd_x = rect.left + 1 + nGap; // 1: graph 영역의 테두리 굵기.
	for( int i = SAMPLE_BONG_COUNT - p_nDrawCount; i < SAMPLE_BONG_COUNT ; i++ )
	{
		int start	= price[i][0];
		int high	= price[i][1];
		int low		= price[i][2];
		int end		= price[i][3];

		CRect bongRect( nPrevEnd_x, ( nOrgPos_y - start ), ( nPrevEnd_x + nWidth ), ( nOrgPos_y - end ) );
		bongRect.NormalizeRect();

		if( start < end )
		{
			pDC->SelectObject( &upPen );
			
			if( bUpFill )
				pDC->SelectObject( &upBrush );
			
			else
				pDC->SelectStockObject( NULL_BRUSH );
		}
		
		else if( start == end )
		{
			pDC->SelectObject( &equalPen );
		} 

		else
		{
			pDC->SelectObject( &downPen );
			
			if( bDownFill )
				pDC->SelectObject( &downBrush );	
			
			else
				pDC->SelectStockObject( NULL_BRUSH );
		}

		DrawRect( pDC, bongRect );

		if( high > end )
		{
			CPoint highPoint( bongRect.CenterPoint().x, nOrgPos_y - high );
			if( highPoint.y < bongRect.top )
			{
				pDC->MoveTo(highPoint.x, bongRect.top);
				pDC->LineTo(highPoint);
			}
		}

		if( low < start )
		{
			CPoint lowPoint( bongRect.CenterPoint().x, nOrgPos_y - low );
			if(lowPoint.y > bongRect.bottom){
				pDC->MoveTo(lowPoint.x, bongRect.bottom);
				pDC->LineTo(lowPoint);
			}
		}

		nPrevEnd_x = bongRect.right + nGap;
	}
	
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	upPen.DeleteObject();
	downPen.DeleteObject();
	upBrush.DeleteObject();
	downBrush.DeleteObject();
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 22
// Return void : 
// Param  CDC *pDC : 
// Param  CRect rect : 
// Param  int *trade : 
// Param  COLORREF upColor : 
// Param  COLORREF downColor : 
// Param  COLORREF equalColor : 
// Param  BOOL bUpFill : 
// Param  BOOL bDownFill : 
// Comments		: 
//-----------------------------------------------------------------------------
void CStyleDataManager::DrawBar( CDC *pDC, CRect rect, int *trade, COLORREF upColor,
									COLORREF downColor, COLORREF equalColor, BOOL bUpFill, BOOL bDownFill, int p_nDrawCount)
{
	CPen upPen( PS_SOLID, 1, upColor ), downPen( PS_SOLID, 1, downColor ), equalPen( PS_SOLID, 1, equalColor );
	CPen *pOldPen = ( CPen * )pDC->SelectStockObject( NULL_PEN );
	CBrush upBrush( upColor ), downBrush( downColor );
	CBrush *pOldBrush = ( CBrush * )pDC->SelectStockObject( NULL_BRUSH );

	int	nWidth = 5, nGap = 1;
	int nOrgPos_y = rect.bottom;
	int nPrevEnd_x = rect.left + 1 + nGap; // 1: graph 영역의 테두리 굵기.
	
	for( int i = SAMPLE_BONG_COUNT - p_nDrawCount; i < SAMPLE_BONG_COUNT ; i++ )
	{
		CRect barRect( nPrevEnd_x, nOrgPos_y - 1, nPrevEnd_x + nWidth, nOrgPos_y - trade[ i ] );
		barRect.NormalizeRect();
		
		if( i == 0 || trade[ i ] > trade[ i - 1 ] )
		{
			pDC->SelectObject( &upPen );
			if( bUpFill )	pDC->SelectObject( &upBrush);
			else			pDC->SelectStockObject( NULL_BRUSH);
		}
		else
		{
			pDC->SelectObject( &downPen );
			if( bDownFill)	pDC->SelectObject( &downBrush);
			else			pDC->SelectStockObject( NULL_BRUSH);
		}
		DrawRect( pDC, barRect );

		nPrevEnd_x = barRect.right + nGap;
	}
	
	pDC->SelectObject( pOldPen );
	pDC->SelectObject( pOldBrush );
	upPen.DeleteObject();
	downPen.DeleteObject();
	upBrush.DeleteObject();
	downBrush.DeleteObject();
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Param  CDC *pDC : 
// Param  CRect rect : 
// Param  int *ave : 
// Param  COLORREF color : 
// Comments		: 
//-----------------------------------------------------------------------------
void CStyleDataManager::DrawLine( CDC *pDC, CRect rect, int *ave, COLORREF color, int p_nDrawCount)
{
	CPen pen(PS_SOLID, 1, color);
	CPen* pOldPen = (CPen*) pDC->SelectObject(&pen);

	int	nWidth = 5, nGap = 1;
	int nOrgPos_y = rect.bottom;
	CPoint prevPt(0, 0);
	for(int i = SAMPLE_BONG_COUNT - p_nDrawCount; i < SAMPLE_BONG_COUNT; i++)
	{
		if(i == ( SAMPLE_BONG_COUNT - p_nDrawCount))
		{
			prevPt.x = rect.left + 1 + nGap + nWidth/2;
			prevPt.y = nOrgPos_y - ave[i];
			continue;
		}

		CPoint curPt((prevPt.x + nGap + nWidth), (nOrgPos_y - ave[i]));
		if(prevPt.y < nOrgPos_y && curPt.y < nOrgPos_y)
		{
			pDC->MoveTo(prevPt);
			pDC->LineTo(curPt);
		}

		prevPt = curPt;
	}

	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 8 / 2
// Return void : 
// Param  CDC* pDC : 
// Param  CRect rect : 
// Comments		: 
//-----------------------------------------------------------------------------
void CStyleDataManager::DrawRect(CDC* pDC, CRect rect)
{
	rect.bottom += 1;
	pDC->Rectangle(rect);
}
