// BoundMarkerAddInImp.cpp: implementation of the CBoundMarkerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BoundMarkerAddInImp.h"

#include <math.h>											// for HUGE_VAL
#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../../Include_Chart/Conversion.h"					// for CDataConversion
#include "../../Include_Chart/IPropertyMap.h"				// for IPropertyMap
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IBlock.h"				// for IBlock
#include "../Include_AddIn/I000000/_IGraphInfoManager.h"	// for IGraphInfoManager
#include "../Include_AddIn/I000000/_IGraphInfo.h"			// for IGraphInfo
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IDoubleList.h"			// for IDoubleList
#include "../Include_AddIn/I000000/_IString.h"				// for ILPCSTR

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoundMarkerAddInImp::CBoundMarkerAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2004.09.30, 배승원) BoundMark를 이용할지 여부를 설정받아 관리/적용한다.
	m_bEnableMark = TRUE;

	// (2004.10.27, 배승원) View의 환경을 관리하여, CAskingPriceIndicator에서 이용할 수 있도록 한다.
	m_bLog			= FALSE;	// 대상 Graph가 등록된 Block의 Log 차트 설정 여부이다.
	m_bReverse		= FALSE;	// 대상 Graph가 등록된 Block의 뒤집힘 설정 여부이다.
	m_dViewMin		= 0.0;		// View내에 대상 Graph된 View의 Minimun Scale 값이다.
	m_dViewMax		= 0.0;		// View내에 대상 Graph된 View의 Maximum Scale 값이다.

	//sy 2005.11.18.
	m_rctBoundMark = CRect(0, 0, 0, 0);
	m_rctGraphRegion = CRect(0, 0, 0, 0);
	m_rctGraphDrawingRegion = CRect(0, 0, 0, 0);
	m_dUpperBound = 0.0;
	m_dLowerBound = 0.0;
	m_dYesterdayClosePrice = 0.0;
	m_bExistenceBoundMark = false;
	//sy end
}

CBoundMarkerAddInImp::~CBoundMarkerAddInImp()
{
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CBoundMarkerAddInImp::m_strAddInItemName = "BOUND_MARKER";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CBoundMarkerAddInImp)
	ONADDINEVENT( OnDrawBeforeGraph)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
	ONADDINEVENT( OnPacketDataMultiItem)
END_ADDIN_EVENT_MAP()

// (2004.09.14, 배승원) Bound값을 설정하는 Interface를 제공한다.
void CBoundMarkerAddInImp::ResetBound( const char *p_szBound)
{
	// 1. 기존 Bound 값을 초기화한다. (설정에 실패하면, Clear되도록)
	m_strIndicatorName.Empty();
	//sy 2005.11.21.
	m_bExistenceBoundMark = false;

	// 2. Message를 확인한다.
	if( !p_szBound) return;
	if( !*p_szBound) return;

	// 3. Message를 Buffer로 옮긴다.
	CString strBoundMark;
	char *szBoundMark = strBoundMark.GetBufferSetLength( strlen( p_szBound));
	strcpy( szBoundMark, p_szBound);

	// 4. 먼저 Graph Name을 구한다.
	char *	szIndicatorName	= szBoundMark;
	char *	szLowerBound	= NULL;
	char *	szUpperBound	= NULL;
	szBoundMark = strchr( szBoundMark, ':');
	if( !szBoundMark) return;
	*szBoundMark = '\0';

	// 5. 상한가 Message를 구한다.
	szBoundMark++;
	szUpperBound = szBoundMark;
	szBoundMark = strchr( szBoundMark, ',');
	if( !szBoundMark) return;
	*szBoundMark = '\0';

	// 6. 하한가 Message를 구한다.
	szBoundMark++;
	szLowerBound = szBoundMark;
	
	// (2004.10.25, 배승원) 확장된 정보가 있어도 오류가 나지 않도록 수정한다. (확장 정보를 별도로 관리한다.)
	szBoundMark = strchr( szBoundMark, ':');
	if( szBoundMark)
	{
		*szBoundMark = '\0';
		szBoundMark++;
		m_strExtMsg = szBoundMark;
		// (2004.10.28, 배승원) 뒷 Space를 미리 Clear해 둔다.
		m_strExtMsg.TrimRight();
	}

	if( !*szIndicatorName || !*szUpperBound || !*szLowerBound) return;
	// 7. 구해진 Graph Name, 상/하한가를 구하여 설설정한다.
	//m_strIndicatorName	= szIndicatorName;
	m_strIndicatorName	= _STOM2(szIndicatorName);

	m_dUpperBound	= atof( szUpperBound);
	m_dLowerBound	= atof( szLowerBound);

	// 8. 구해진 상/하한가가 뒤집힌 경우를 바로잡는다.
	if( m_dUpperBound < m_dLowerBound)
	{
		double dTemp	= m_dUpperBound;
		m_dUpperBound	= m_dLowerBound;
		m_dLowerBound	= dTemp;
	}

	//sy 2005.11.21.
	// 9. 상/하한바가 사용 여부가 setting 되었음을 설정한다.
	m_bExistenceBoundMark = true;

	//sy 2005.11.18. 전일종가
	// 10. 상/하한바의 기준점이 되는 전일종가 설정한다.
	SetYesterdayClosePrice(m_strExtMsg);
}

// (2004.09.30, 배승원) BoundMark를 이용할지 여부를 설정받아 관리/적용한다.
void CBoundMarkerAddInImp::EnableBoundMark( BOOL p_bEnable)
{
	m_bEnableMark = p_bEnable;
}

//sy 2005.11.18.
void CBoundMarkerAddInImp::SetYesterdayClosePrice(const CString& strData)
{
	CString strTemp = strData;
	CDataConversion::GetStringData(strTemp, ":");//종목코드
	CDataConversion::GetStringData(strTemp, ":");//??
	CString strPrice = CDataConversion::GetStringData(strTemp, ":");//전일종가

	m_dYesterdayClosePrice = atof(strPrice);
}

double CBoundMarkerAddInImp::GetYesterdayClosePrice() const
{
	return m_dYesterdayClosePrice;
}

double CBoundMarkerAddInImp::GetSteadyPrice() const
{
	if(m_dYesterdayClosePrice != 0)
		return m_dYesterdayClosePrice;

	return ((m_dUpperBound + m_dLowerBound) / 2.0);
}
//sy end


//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
//////////////////////////////////////////////////////////////////////

// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
// (2004.09.14, 배승원) 상한가/하한가 Mark를 출력하는 Routine을 제공한다.
// (2004.10.25, 배승원) BoundMark의 출력 영역을 받아 관리한다. (이를 계승받는 Asking Price Indicator에서 이용한다.)
void CBoundMarkerAddInImp::OnDrawBeforeGraph( HDC p_hDC)
{
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;

	// (2004.10.25, 배승원) Drawing Skip의 경우를 대비하여 미리 Empty Rectangle로 초기화한다.
	m_rctBoundMark.SetRectEmpty();

	// (2004.09.30, 배승원) BoundMark를 이용할지 여부를 설정받아 관리/적용한다.
	if( !m_bEnableMark) return;

	// (2004.09.15, 배승원) 특수차트인 경우에는 보이지 않도록 한다.
	if( m_pIChartOCX->IsOnSpecialChart()) return;

	// 0. Bound가 설정된 경우만 그린다.
	if( m_strIndicatorName.IsEmpty()) return;

	// 1. 가격차트가 포함된 Block의 Index를 구한다.
	// (2004.10.27, 배승원) 특정 Graph가 등록된 Block의 Index를 구하는 Interface를 OCX에서 제공한다.
	int nR, nC, nV, nG;
	// (2007/2/1 - Seung-Won, Bae) Receive ReturnValue with ILPCSTR for Memroy Free.
	ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( m_strIndicatorName, nR, nC, nV, nG, "DEFAULT");
	if( szGraphName.IsEmpty()) return;
	// (2004.09.22, 배승원) Bound Mark를 Block영역의 우측끝에 밀착시켜 표시한다.
	// 2. Bound Mark의 기본 위치를 확인한다.
	// 2.1 (2004.09.23, 배승원) Bound Mark는 Block의 우측 Border를 중심으로 좌우 2 Pixel씩 나타나도록 한다.
	//		이때 바탕의 Border Line등이 표시되도록 Color 연산을 처리한다.
	// (2006/6/16 - Seung-Won, Bae) Use Block Interface
	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
	if( !pIBlock) return;
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( m_rctGraphRegion, rctNoFrameAndMargin);
	m_rctBoundMark.left = m_rctGraphRegion.right - 1; //-2
	m_rctBoundMark.right = m_rctGraphRegion.right + 2;//3		// FillRect의 Drawing 방식을 감안하여 1 Pixel 추가
	// 2.2 Bound Mark의 크기를 구한다.
	// 2.2.0 View의 Min/Max 값을 구한다.
	pIBlock->GetVScaleGroupMinMax( nV, m_dViewMin, m_dViewMax);
	// (2007/1/15 - Seung-Won, Bae) Check No Data in Block
	if( m_dViewMin == m_dViewMax && m_dViewMax == -HUGE_VAL) return;
	// (2004.10.27, 배승원) Block의 Log/뒤집기 설정을 확인하여 정확하게 Data를 변환한다. (가격<->Y Pos)
	// 2.2.1 가격차트의 Block에 Log/뒤집기 설정값을 확인한다.
	m_bLog = pIBlock->IsLogScale();
	m_bReverse = pIBlock->IsVScaleInvert( nV);
	// 2.2.2 상한가 좌표를 구한다.
	m_rctGraphDrawingRegion		= pIBlock->GetGraphDrwingRegion();
	m_rctBoundMark.top		= m_pIChartOCX->ConvertDataToYPosition( m_dUpperBound, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
	// 2.2.3 하한가 좌표를 구한다.
	m_rctBoundMark.bottom	= m_pIChartOCX->ConvertDataToYPosition( m_dLowerBound, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
	pIBlock->Release();
	pIBlock = NULL;

	// (2004.10.27, 배승원) Block의 Log/뒤집기 설정을 확인하여 정확하게 Data를 변환한다. (가격<->Y Pos)
	//		'뒤집기' 상태이면 Bound Mark를 뒤지어 놓는다. (아래서 색상도 바꿔 출력하면 바로 보인다.)
	if( m_bReverse)
	{
		int nTemp				= m_rctBoundMark.top;
		m_rctBoundMark.top		= m_rctBoundMark.bottom;
		m_rctBoundMark.bottom	= nTemp;
	}

	// 3. Bound Mark를 출력한다.
	// 3.0 먼저 Box가 그려질 상황인지 확인한다.
	// (2004.10.01, 배승원) Drawing 여부을 GraphRegion이 아니라 BlockRegion으로 확인한다.
	//		(GraphRegion과 BlockReion은 상하에 약간의 여백이 있는데, 그 여백을 두고 그리지 않는 상황이 발생하지 않도록한다.)
	if( m_rctGraphRegion.bottom <= m_rctBoundMark.top || m_rctBoundMark.bottom <= m_rctGraphDrawingRegion.top)
	{
		m_rctBoundMark.SetRectEmpty();
		return;
	}
	// 3.1 Bound Mark의 상한/하한을 보정한다. (Block을 넘치지 않도록)
	//	특히, 보정시 그 조정값을 Graph Region이 아닌 Block Region으로 하여, 잘림을 확인할 수 있도록 한다.
	if( m_rctBoundMark.top < m_rctGraphRegion.top)			m_rctBoundMark.top = m_rctGraphRegion.top;
	if( m_rctGraphRegion.bottom < m_rctBoundMark.bottom)	m_rctBoundMark.bottom = m_rctGraphRegion.bottom;

	// 3.2 Bound Mark의 중심을 구하여 상승과 하락 Mark의 기준으로 잡는다.
	// (2004.10.27, 배승원) Block의 Log/뒤집기 설정을 확인하여 정확하게 Data를 변환한다. (가격<->Y Pos)
	int nBoundCenter = m_pIChartOCX->ConvertDataToYPosition(GetSteadyPrice(), //sy 2005.11.18.
		m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
	// (2004.09.23, 배승원) 상승/하락/보합의 색상을 구하여 Bound Mark를 표시한다.
	// 3.2.5.1 기본 색상을 설정한다.
	COLORREF clrUp		= RGB( 255,	0, 0);
	COLORREF clrDown	= RGB( 0,	0, 255);
	COLORREF clrSteady	= RGB( 0,	0, 0);
	// 3.2.5.2 가격차트 설정값을 조회한다.
	IGraphInfoManager *pIGraphInfoManager = m_pIChartOCX->GetIGraphInfoManager();
	if( pIGraphInfoManager)
	{
		// (2006/9/20 - Seung-Won, Bae) Use IGraphInfo
		IGraphInfo *pIGraphInfo = pIGraphInfoManager->GetGraphInfo( IGraphInfoManager::IEILT_USER, m_strIndicatorName);
		if( pIGraphInfo)
		{
			clrUp		= pIGraphInfo->GetSubGraphColorWithZBI( 0, 0);
			clrDown		= pIGraphInfo->GetSubGraphColorWithZBI( 0, 2);
			clrSteady	= pIGraphInfo->GetSubGraphColorWithZBI( 0, 4);
			
			pIGraphInfo->Release();
		}
		pIGraphInfoManager->Release();
	}

	// (2004.10.27, 배승원) Block의 Log/뒤집기 설정을 확인하여 정확하게 Data를 변환한다. (가격<->Y Pos)
	//		상승구간과 하락구간이 뒤집힌 경우, 색상도 뒤집어 설정한다. (영역은 위에서 이미 뒤집어 놓았다.)
	if( m_bReverse)
	{
		COLORREF clrTemp	= clrDown;
		clrDown				= clrUp;
		clrUp				= clrTemp;
	}

	// 3.3 하락 Mark를 출력한다.
	// (2004.10.01, 배승원) Drawing 여부을 GraphRegion이 아니라 BlockRegion으로 확인한다.
	//		(GraphRegion과 BlockReion은 상하에 약간의 여백이 있는데, 그 여백을 두고 그리지 않는 상황이 발생하지 않도록한다.)
	if( nBoundCenter < m_rctGraphRegion.bottom)
	{
		// 3.3.1 하락 Mark의 크기를 구한다.
		CRect rctDownMark( m_rctBoundMark);
		if( rctDownMark.top < nBoundCenter) rctDownMark.top = nBoundCenter;
		// 3.3.2 하락 Mark를 출력한다.
		//	(FillRect는 Right, Bottom의 Pixel에 Drawing하지 않는 것을 감안하여 Bottom을 1 Pixel 증가시킨다.)
		CBrush brDownMark( clrDown);
		rctDownMark.bottom++;
		pDC->FillRect( &rctDownMark, &brDownMark);
	}

	// 3.4 상승 Mark를 출력한다.
	// (2004.10.01, 배승원) Drawing 여부을 GraphRegion이 아니라 BlockRegion으로 확인한다.
	//		(GraphRegion과 BlockReion은 상하에 약간의 여백이 있는데, 그 여백을 두고 그리지 않는 상황이 발생하지 않도록한다.)
	if( m_rctGraphRegion.top < nBoundCenter)
	{
		// 3.4.1 상승 Mark의 크기를 구한다.
		CRect rctUpMark( m_rctBoundMark);
		if( nBoundCenter < rctUpMark.bottom) rctUpMark.bottom = nBoundCenter;
		// 3.3.2 상승 Mark를 출력한다.
		//	(FillRect는 Right, Bottom의 Pixel에 Drawing하지 않는 것을 감안하여 Bottom을 1 Pixel 증가시킨다.)
		CBrush brUpMark( clrUp);
		rctUpMark.bottom++;
		pDC->FillRect( &rctUpMark, &brUpMark);
	}

	// 3.5 보합 Line을 출력한다.
	// (2004.10.01, 배승원) Drawing 여부을 GraphRegion이 아니라 BlockRegion으로 확인한다.
	//		(GraphRegion과 BlockReion은 상하에 약간의 여백이 있는데, 그 여백을 두고 그리지 않는 상황이 발생하지 않도록한다.)
	if( nBoundCenter < m_rctGraphRegion.bottom && m_rctGraphRegion.top < nBoundCenter)
	{
		CPen pnSteady( PS_SOLID, 1, clrSteady);
		CPen *ppnOld = pDC->SelectObject( &pnSteady);
		pDC->MoveTo( m_rctBoundMark.left,	nBoundCenter);
		pDC->LineTo( m_rctBoundMark.right,	nBoundCenter);
		pDC->SelectObject( ppnOld);
	}

	// 4. 현재가를 구하여 Mark한다.
	// 4.1 가격 Data List를 구한다.
	IPacket *pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_CLOSE));
	if( pIPacket)
	{
		IDoubleList *pIDoubleList = pIPacket->GetDataList();
		pIPacket->Release();
		if( pIDoubleList)
		{
			// (2004.12.17, 배승원) GetTail()시에 오류가 나지 않도록 사전에 확인한다.
			if( pIDoubleList && !pIDoubleList->IsEmpty())
			{
				// 4.2 종가의 Marking Position를 구한다.
				double dClose;
				pIDoubleList->GetTail( dClose);
				int nCloseX = m_rctBoundMark.right;
				int nCloseY = m_pIChartOCX->ConvertDataToYPosition( dClose, m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
				// 4.3 종가 표시를 할 수 있는 상황인지 확인한다.
				if( m_rctBoundMark.top <= nCloseY && nCloseY <= m_rctBoundMark.bottom)
				{
					CPoint ptClose[ 3];
					ptClose[ 0].x = nCloseX;
					ptClose[ 0].y = nCloseY;
					ptClose[ 1].x = nCloseX	+ 5;
					ptClose[ 1].y = nCloseY	+ 5;
					ptClose[ 2].x = nCloseX	+ 5;
					ptClose[ 2].y = nCloseY	- 5;

					// (2004.12.14, 배승원) 현재가 표시 색상을 표시 위치 (상승/하락)에 따라 달리한다.(기본:보합)
					COLORREF clrEndMark = clrSteady;
					//sy 2005.11.18.
					//double dSteady = ( m_dUpperBound + m_dLowerBound) / 2;
					double dSteady = GetSteadyPrice();
					//sy end
					if( dSteady < dClose)		clrEndMark = clrUp;
					else if( dClose < dSteady)	clrEndMark = clrDown;

					CPen	pnEndMark( PS_SOLID, 1, clrEndMark);
					CBrush	brEndMark( clrEndMark);

					CPen *ppnOld	= pDC->SelectObject( &pnEndMark);
					CBrush *pbrOld	= pDC->SelectObject( &brEndMark);

					pDC->Polygon( ptClose, 3);

					pDC->SelectObject( ppnOld);
					pDC->SelectObject( pbrOld);
				}
			}
			pIDoubleList->Release();
		}
	}
}

// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CBoundMarkerAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// 0. NULL Pointer를 확인한다.
	if( !m_pIDefaultPacketManager || strcmp( lpszRQ, "DEFAULT")) return;

	// (2004.10.13, 배승원) 우선 상하한가 보조 Message를 수신할 시점인지 확인한다.
	// (2004.10.22, 배승원) 상한/하한가 표시 경우와 가격 끝일봉 초기화 기능은
	//			모두 가격 차트가 있는 종목의 초기화 TR의 수신시점에 해당한다.
	//		이 기능들은 동양 지존차트의 종목/업종의 가격/비율형 봉차트를 지원하면서,
	//			상/하한가를 표시하거나 전일 종가 표시를 Reset 시키는 기능을 처리하는 것이다.
	//		특히 이 기능들은 차트내 1개의 Graph에만 적용할 수 있는 기능으로
	//			(기능의 Data 구조가 AddIn 당 1개로 국한된 기술적인 문제이다.)
	//			종가 Packet을 이용하는 가격차트에만 제공되는 기능으로 국한한다.
	//		*. 보다 자세한 내용은 각 Class Header 참고
	if( !( BOOL)strstr( p_szPacketNames, CString( _MTEXT( C0_CLOSE)) + ";") || !p_bWithReset) return;

	// (2004.09.14, 배승원) 상하한가 표시를 위한 값을 확인한다.
	// (2004.10.04, 배승원) 상하한가 표시는 가격차트의 값이 변경되는 경우에만 반영한다.
	// (2004.10.14, 배승원) 상하한가를 구하지 못했어도 설정한다. (Clear 효과)
	ILPCSTR szHelpMsg = NULL;
	m_pIDefaultPacketManager->GetHelpMessageItem( "BOUNDMARK", szHelpMsg);
	// (2004.10.22, 배승원) 상/하한가 표시기능에 호가연동 기능을 보강한다.
	ResetBound( szHelpMsg);
}

// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
void CBoundMarkerAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	const char *szEnvValue = NULL;
	if( !p_pIAddInEnvMap->Lookup( m_strAddInItemName, szEnvValue)) return;
	m_bEnableMark = ( atoi( szEnvValue) == 1);
}
void CBoundMarkerAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	const char *szEnvValue = NULL;
	if( p_pIAddInEnvMap->Lookup( m_strAddInItemName, szEnvValue)) return;

	CString strEnvValue;
	strEnvValue.Format( "%d", m_bEnableMark ? 1 : 0);
	p_pIAddInEnvMap->SetAt( m_strAddInItemName, strEnvValue);
}
