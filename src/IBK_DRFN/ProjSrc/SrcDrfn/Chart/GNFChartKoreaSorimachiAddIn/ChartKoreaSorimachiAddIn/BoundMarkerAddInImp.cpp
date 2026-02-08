// BoundMarkerAddInImp.cpp: implementation of the CBoundMarkerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BoundMarkerAddInImp.h"

#include <math.h>													// for HUGE_VAL
#include "../Include_Chart/Dll_Load/IMetaTable.h"					// for _MTEXT()
#include "../Include_Chart/IPropertyMap.h"							// for IPropertyMap
#include "../Include_AddIn_381931/I381931/_IChartOCX.h"				// for IChartOCX
#include "../Include_AddIn_381931/I381931/_IChartManager.h"			// for IChartManager
#include "../Include_AddIn_381931/I381931/_IBlock.h"				// for IBlock
#include "../Include_AddIn_381931/I381931/_IGraphInfoManager.h"		// for IGraphInfoManager
#include "../Include_AddIn_381931/I381931/_IGraphInfo.h"			// for IGraphInfo
#include "../Include_AddIn_381931/I381931/_IPacket.h"				// for IPacket
#include "../Include_AddIn_381931/I381931/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn_381931/I381931/_IDoubleList.h"			// for IDoubleList
#include "../Include_AddIn_381931/I381931/_IString.h"				// for ILPCSTR
#include "Conversion.h"												// for CDataConversion
#include "../../COMMONTR/OAQ69901_io.h"					// for _MTEXT()

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
	m_pmapOrder = NULL;
	m_bDraw = TRUE;
}

CBoundMarkerAddInImp::~CBoundMarkerAddInImp()
{
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CBoundMarkerAddInImp::m_strAddInItemName = "BOUND_MARKER_CKSA";

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
	m_strIndicatorName	= szIndicatorName;
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
	if (m_bDraw == FALSE)
		return;
	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return;

	m_rctBoundMark.SetRectEmpty();

	if( m_pIChartOCX->IsOnSpecialChart()) return;

	int nR, nC, nV, nG;
	if (m_pIChartManager->GetIndexOfGraph(_MTEXT(C2_PRICE_CHART),  nR, nC, nV, nG) == FALSE)
		return;

	IBlock *pIBlock = m_pIChartManager->GetBlock( nR, nC);
	if( !pIBlock) return;
	CRect rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( m_rctGraphRegion, rctNoFrameAndMargin);
	m_rctBoundMark.left = m_rctGraphRegion.right - 1; //-2
	m_rctBoundMark.right = m_rctGraphRegion.right + 2;//3		// FillRect의 Drawing 방식을 감안하여 1 Pixcel 추가

	pIBlock->GetVScaleGroupMinMax( nV, m_dViewMin, m_dViewMax);

	if( m_dViewMin == m_dViewMax && m_dViewMax == -HUGE_VAL)
	{
		pIBlock->Release();
		return;
	}

	m_bLog = pIBlock->IsLogScale();
	m_bReverse = pIBlock->IsVScaleInvert( nV);
	// 2.2.2 상한가 좌표를 구한다.
	m_rctGraphDrawingRegion		= pIBlock->GetGraphDrwingRegion();

	COLORREF clrMedo = RGB(0, 0, 255);
	COLORREF clrMesu = RGB(255, 0, 0);
	CPen MedoPen(PS_SOLID, 1, clrMedo);
	CPen MesuPen(PS_SOLID, 1, clrMesu);

	CString strKey;
	POSITION pos;
	ORDER_INFO * pOrerInfo;
	CString	strCode = m_strItemCode;
	pos = m_pmapOrder->GetStartPosition();
	while (pos)
	{
		m_pmapOrder->GetNextAssoc(pos, strKey, (CObject*&)pOrerInfo);

		if (pOrerInfo->m_strseries == strCode)
		{
			CPen * pOldPen = NULL;
			int nBoundCenter = m_pIChartOCX->ConvertDataToYPosition(pOrerInfo->m_dblPrice,
				m_dViewMax, m_dViewMin, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_bLog, m_bReverse);
		
			if (pOrerInfo->m_strbuysellsect == "0")
			{
				pOldPen = (CPen*) pDC->SelectObject(&MesuPen);
				pDC->SetTextColor(clrMesu);
			}
			else
			{
				pOldPen = (CPen*) pDC->SelectObject(&MedoPen);
				pDC->SetTextColor(clrMedo);
			}
		
			if (m_rctGraphDrawingRegion.top <= nBoundCenter && m_rctGraphDrawingRegion.bottom >= nBoundCenter)
			{
				pDC->MoveTo(m_rctGraphDrawingRegion.left, nBoundCenter);
				pDC->LineTo(m_rctGraphDrawingRegion.right, nBoundCenter);
				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOut(m_rctGraphDrawingRegion.left, nBoundCenter+2, pOrerInfo->m_strposprice);
			}
			pDC->SelectObject(pOldPen);
		}

	}

	MedoPen.DeleteObject();
	MesuPen.DeleteObject();

	pIBlock->Release();
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
