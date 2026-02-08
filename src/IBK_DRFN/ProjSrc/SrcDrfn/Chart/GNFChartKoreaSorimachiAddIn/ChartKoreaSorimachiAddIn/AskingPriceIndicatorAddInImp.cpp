// AskingPriceIndicatorAddInImp.cpp: implementation of the CAskingPriceIndicatorAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartKoreaSorimachiAddIn.h"
#include "AskingPriceIndicatorAddInImp.h"

#include <math.h>												// for log()
#include "../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "../Include_AddIn_381931/I381931/_AddInCommand.h"		// for EAI_CUSTOM_ENABLE_BOUNDMARK
#include "../Include_AddIn_381931/I381931/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn_381931/I381931/_IPacket.h"			// for IPacket
#include "../Include_AddIn_381931/I381931/_IPacketManager.h"	// for IPacketManager
#include "../Include_AddIn_381931/I381931/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn_381931/I381931/_IString.h"			// for ILPCSTR

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAskingPriceIndicatorAddInImp::CAskingPriceIndicatorAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CBoundMarkerAddInImp( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2004.10.25, 배승원) 호가연동을 위한 Member Variable을 정의한다.
	m_nIndicatorPosY			= 0;				// Current Indicator Drawing Position			(IndicatorPosition)
	m_bOnSetCaprture			= FALSE;			// Indicator Drawing Situation Flag			(SetCaptureFlag)
	m_bOnFireAskingPriceGShare	= FALSE;			// Asking Price GShare Firing Flag			(AskingPriceGShareFireFlag)
	m_dIndicatedPrice			= 0.0;				// Indicated Price							(IndicatedPrice)

	// (2004.10.25, 배승원) SetCapture()후, 현재 설정하는 Cursor를 관리하면서,
	//		ReleaseCapture()할 때까지 수시로 OCX에 Cursor를 설정한다.
	//		(다른 분석도구들이 수시로 설정을 해제하여 추가적인 설정이 잘 되지 않는다. 구조가 좀...)
	m_hAPCursor = NULL;

	// (2004.10.26, 배승원) 호가의 정밀도(Precision)을 관리하여 호가 String의 생성에 참고하도록 한다.
	m_dPacketUnitValue = 0;

	// (2004.11.01, 배승원) 장구분과 호가산출 DLL을 관리하여, 호가 연동에 이용한다.
	m_nMarketType	= -1;
	m_nRatioByYCP	= 0;			// (2004.11.02, 배승원) 대비율(등락률)인지 가격인지 Flag을 받아 관리한다.
	m_dYClosePrice	= 0;
	m_hAskingPriceFixDLL = LoadLibrary( "PriceUnit.dll");
	if( m_hAskingPriceFixDLL) m_fnFnGetPriceUnit = ( FnGetPriceUnit)::GetProcAddress( m_hAskingPriceFixDLL, "__GetPriceUnit");
	else m_fnFnGetPriceUnit = NULL;

}

CAskingPriceIndicatorAddInImp::~CAskingPriceIndicatorAddInImp()
{
	// (2004.11.01, 배승원) 장구분과 호가산출 DLL을 관리하여, 호가 연동에 이용한다.
	if( m_hAskingPriceFixDLL)
	{
		FreeLibrary( m_hAskingPriceFixDLL);
		m_hAskingPriceFixDLL = NULL;
	}
}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CAskingPriceIndicatorAddInImp::m_strAddInItemName = "ASKED_PRICE_CKSA";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CAskingPriceIndicatorAddInImp)
	ONADDINEVENT( OnResetChart)
	ONADDINEVENT( OnDrawBeforeGraph)
	ONADDINEVENT( DrawCursor)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_long)
	ONADDINEVENT( OnAddInToolCommand_long_EAICommandType)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
	ONADDINEVENT( OnLoadEnvironment)
	ONADDINEVENT( OnSaveEnvironment)
	ONADDINEVENT( OnPacketDataMultiItem)
END_ADDIN_EVENT_MAP()


//////////////////////////////////////////////////////////////////////
// (2004.10.25, 배승원) 호가연동을 위한 Library Routine을 제공한다.
//////////////////////////////////////////////////////////////////////

// (2004.10.25, 배승원) 호가연동 처리 Mark를 Clear시키는 Routine을 제공한다.
void CAskingPriceIndicatorAddInImp::ClearAskingPriceGShareFireFlag( void)
{
	// 1. 호가연동 처리 Flag을 해제한다.
	m_bOnFireAskingPriceGShare = FALSE;

	// 2. 호가연동 영역 Cursor를 보여준다.
	// (2004.10.25, 배승원) SetCapture()후, 현재 설정하는 Cursor를 관리하면서,
	//		ReleaseCapture()할 때까지 수시로 OCX에 Cursor를 설정한다.
	//		(다른 분석도구들이 수시로 설정을 해제하여 추가적인 설정이 잘 되지 않는다. 구조가 좀...)
	AfxSetResourceHandle( ML_DEFAULT_RES);
	m_hAPCursor = ::LoadCursor( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDC_AP_INDICATOR));
	ASSERT( m_hAPCursor);
	m_pIChartOCX->SetCursorHandle( m_hAPCursor);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.25, 배승원) Bound값을 설정하는 Interface를 제공한다.
//		확장된 종목코드 정보를 받아 관리하여 종목공유시에 이용한다.
//////////////////////////////////////////////////////////////////////
void CAskingPriceIndicatorAddInImp::ResetBound( const char *p_szBound)
{
	// 0. 먼저 Base의 Bound Info를 먼저 설정한다.
	CBoundMarkerAddInImp::ResetBound( p_szBound);

	// 0.5 오류의 경우를 대비하여 미리 Item Code를 Clear시킨다.
//	m_strItemCode.Empty();

	// 1. Bound Info가 정상적으로 설정된 경우에, 종목 정보를 추출한다.
	if( m_strIndicatorName.IsEmpty()) return;

	// (2004.11.01, 배승원) 종목코드, 장구분과 전일종가를 추출을 연동에 이용한다.
	// 2.1 Buffer를 할당받는다.
	CString strMsg;
	char *szBuffer = strMsg.GetBuffer( m_strExtMsg.GetLength());
	strcpy( szBuffer, m_strExtMsg);
	// 2.2 종목코드를 구한다.
	char *szItemCode = szBuffer;
	szBuffer = strchr( szBuffer, ':');
	if( szBuffer) *szBuffer++ = '\0';
//	m_strItemCode = szItemCode;
	// (2004.11.01, 배승원) 종목코드는 RTime을 시킨다. Server에서는 기본 7자리로 관리한다고 한다.
//	m_strItemCode.TrimRight();
	// 2.3 장구분를 구한다.
	m_nMarketType = -1;
	if( szBuffer)
	{
		char *szMarketType = szBuffer;
		szBuffer = strchr( szBuffer, ':');
		if( szBuffer) *szBuffer++ = '\0';
		m_nMarketType = atol( szMarketType);
	}
	// 2.4 전일종가를 구한다.
	m_dYClosePrice = 0;
	if( szBuffer)
	{
		char *szYClosePrice = szBuffer;
		szBuffer = strchr( szBuffer, ':');
		if( szBuffer) *szBuffer++ = '\0';
		m_dYClosePrice = atof( szYClosePrice);
	}
	// (2004.11.02, 배승원) 대비율(등락률)인지 가격인지 Flag을 받아 관리한다.
	m_nRatioByYCP = 0;
	if( szBuffer)
	{
		char *szRatioByYCP = szBuffer;
		szBuffer = strchr( szBuffer, ':');
		if( szBuffer) *szBuffer++ = '\0';
		m_nRatioByYCP = atol( szRatioByYCP);
	}

	// 3. 상/하한 표시의 해당 Graph가 이용하는 Packet을 하나 찾아,
	//		해당 정밀도를 관리하여, TEXT 출력에 이용한다.
	// 3.1 먼저 첫 Packet 명을 구한다.
	ILPCSTR szPacketName = m_pIChartManager->GetPacketNameStringOfGraph( m_strIndicatorName);
	CString strPacketName( szPacketName);
	int nNextPacketIndex = strPacketName.Find( ',');
	if( 0 < nNextPacketIndex) strPacketName = strPacketName.Left( nNextPacketIndex);

	// 3.2 첫 Packet명의 Packet Object를 구한다.
	IPacket *pIPacket = NULL;
	if( !strPacketName.IsEmpty())
	{
		// 3.3 Packet의 Packet Type Unit Value를 구한다.
		pIPacket = m_pIDefaultPacketManager->GetPacket( strPacketName);
	}

	// 3.3 Packet의 Packet Unit Value (Precision)를 구한다.
	m_dPacketUnitValue = 0;
	if( pIPacket)
	{
		m_dPacketUnitValue = pIPacket->GetPacketTypeUnitValue();
		pIPacket->Release();
	}
}

//////////////////////////////////////////////////////////////////////
// (2004.10.11, 배승원) 기본적인 Interface를 Default로 제공한다. (Derived Class에서 재정의하지 않아도 오류가 나지 않도록)
//////////////////////////////////////////////////////////////////////

// [04/10/11] AddIn DLL들에게 ResetChart 상황을 알려 처리되도록 한다.
void CAskingPriceIndicatorAddInImp::OnResetChart( void)
{
	// (2005/9/29 - Seung-Won, Bae) Reset BoundMark Flag
	OnAddInToolCommand( EAI_CUSTOM_ENABLE_BOUNDMARK, TRUE);
}

// [04/10/11] Draw Event를 AddIn DLL에 알려 처리될 수 있도록 한다.
void CAskingPriceIndicatorAddInImp::OnDrawBeforeGraph( HDC p_hDC)
{
	// 0. 먼저 Base의 Drawing를 처리한다.
	CBoundMarkerAddInImp::OnDrawBeforeGraph( p_hDC);

	// 1. BoundMark의 영역을 확장하여, Indicator Area를 구한다.
	m_rctAskingPriceMark = m_rctBoundMark;
	m_rctAskingPriceMark.InflateRect( 2, 0);
}

// RQ별 Packet : 복수종목 - ojtaso (20070111)
void CAskingPriceIndicatorAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	// 0. 먼저 Base에 보조 Message를 처리토록 한다.
	CBoundMarkerAddInImp::OnPacketDataMultiItem(lpszRQ, p_szPacketNames, p_bWithReset);
}

// (2006/11/12 - Seung-Won, Bae) Support AddIn ENV Value Map
void CAskingPriceIndicatorAddInImp::OnLoadEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	CBoundMarkerAddInImp::OnLoadEnvironment( p_pIAddInEnvMap);
}
void CAskingPriceIndicatorAddInImp::OnSaveEnvironment( IPropertyMap *p_pIAddInEnvMap)
{
	CBoundMarkerAddInImp::OnSaveEnvironment( p_pIAddInEnvMap);
}

//////////////////////////////////////////////////////////////////////
// (2004.10.29, 배승원) AddIn DLL에게 그밖의 Windows Event를 전달한다.
//////////////////////////////////////////////////////////////////////

// (2007/1/7 - Seung-Won, Bae) Support AddIn Cursor Operation.
RECT CAskingPriceIndicatorAddInImp::DrawCursor( HDC p_hDC)
{
	CRect rct;
	rct.SetRectEmpty();

	if( 0 == m_nIndicatorPosY) return rct;

	CDC *pDC = CDC::FromHandle( p_hDC);
	if( !pDC) return rct;

	// 2. Price Value를 산출한다. (View의 Data Min/Max와 Graph Region을 이용한다.)
	// (2004.10.27, 배승원) Block의 Log/뒤집기 설정을 확인하여 정확하게 Data를 변환한다. (가격<->Y Pos)
	m_dIndicatedPrice = m_pIChartOCX->ConvertYPositionToData( m_nIndicatorPosY, m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, m_dViewMax, m_dViewMin, m_bLog, m_bReverse);

	// 3. Asking Price Text의 출력 영역을 산출한다.
	// (2004.10.26, 배승원) 호가의 정밀도(Precision)을 관리하여 호가 String의 생성에 참고하도록 한다.
	int nDecimal = 0;
	// (2004.10.26, 배승원, 주의!) double을 Int로 변환하는 과정에서 정확한 Int로 변환되지 않는 문제를 해결하기 위해 100배 유효자리수를 얻도록 조정한다.
	if( m_dPacketUnitValue != 0) nDecimal = ( int)( ( log10( m_dPacketUnitValue) * 100) / 100);	
	CString strFormat;
	if( 0 <= nDecimal) strFormat = "%.0f";
	else strFormat.Format( "%%.%df", -nDecimal);
	// 3.1 Asking Price Text를 구한다.
	// (2004.10.26, 배승원) 조회중인 호가 String을 관리하여, 선택시 바로 공유가 발생하도록 한다.
	m_strAskingPrice.Format( strFormat, m_dIndicatedPrice);
	// (2004.10.26, 배승원) Comma를 추가한다.
	// 3.1.1 ','가 필요한 경우 있지를 확인한다.
	int nFixedDecimalLen = m_strAskingPrice.Find( '.');
	if( nFixedDecimalLen < 0) nFixedDecimalLen = m_strAskingPrice.GetLength();
	if( 3 < nFixedDecimalLen)
	{
		// 3.1.2 ','를 추가할 Buffer를 확보한다. (간단히 기본 2배로 잡는다.)
		const char *szBuffer1 = m_strAskingPrice;
		CString strBuffer;
		char *szBuffer2 = strBuffer.GetBuffer( m_strAskingPrice.GetLength() * 2);
		// 3.1.3 Data를 뒤 '.'부터 확인하면서, ','를 등록한다.
		int i, j;
		for( i = 0, j = nFixedDecimalLen - 1; j >= 0; i++)
		{
			szBuffer2[ i] = szBuffer1[ j--];
			if( i % 4 == 2 && j >= 0 && szBuffer1[ j] != '-') szBuffer2[ ++i] = ',';
		}
		szBuffer2[ i] = '\0';
		// 3.1.4 다시 Data를 Revere시키면서 설정한다.
		int nIntLen = i;
		CString strBuffer3;
		char *szFormatData = strBuffer3.GetBuffer( m_strAskingPrice.GetLength() * 2);
		szBuffer2 += nIntLen - 1;
		for( i = 0; i < nIntLen; i++) *( szFormatData++) = *( szBuffer2--);
		// 3.1.5 소숫점 이하를 다시 추가한다.
		do *( szFormatData++) = szBuffer1[ nFixedDecimalLen];	while( szBuffer1[ nFixedDecimalLen++] != '\0');
		strBuffer3.ReleaseBuffer();
		// 3.1.6 Buffer를 마무리한다.
		strBuffer.ReleaseBuffer();
		// 3.1.7 Format된 Data를 설정한다.
		m_strAskingPrice = strBuffer3;
	}
	// 3.2 Price TEXT의 영역크기를 구한다.
	CSize szPriceText = pDC->GetTextExtent( m_strAskingPrice);
	
	// 4. Asking Price Text의 출력 영역을 Backup한다.
	// 4.0 먼저 TEXT의 출력 Position을 확정한다.
	CPoint ptPrice( m_rctBoundMark.right, m_nIndicatorPosY - szPriceText.cy / 2);
	// (2004.10.27, 배승원) 우측 Scale이 없는 경우, (즉 BoundMark의 크기 폭이 얇으면, Text의 위치를 좌측으로 조정한다.
	CRect rctChartOcx;
	m_pChartCWnd->GetClientRect( &rctChartOcx);
	if( !rctChartOcx.PtInRect( ptPrice + CSize( szPriceText.cx, 0)))
		ptPrice.x = m_rctBoundMark.left - szPriceText.cx - 10;

	// (2004.10.25, 배승원) TEXT의 영역을 Left 기준으로 고정하고, 크기를 상하좌우를 늘려서 여백이 되도록 한다.
	rct = CRect( ptPrice.x, ptPrice.y - 2, ptPrice.x + szPriceText.cx + 5, ptPrice.y + szPriceText.cy + 1);
	// (2006/11/5 - Seung-Won, Bae) Use MainOSB

	// 5. 현 위치에 호가를 표시한다.
	// 5.1 NOT Line을 출력한다.
	int nPreROP = pDC->SetROP2( R2_NOT);
	pDC->MoveTo( m_rctGraphRegion.left,		m_nIndicatorPosY);
	pDC->LineTo( m_rctGraphRegion.right,	m_nIndicatorPosY);
	pDC->SetROP2( nPreROP);
	// 5.2 호가 TEXT를 출력한다.
	CPen *	pOldPen		= ( CPen *)pDC->SelectStockObject( BLACK_PEN);
	CBrush *pOldBrush	= ( CBrush *)pDC->SelectStockObject( WHITE_BRUSH);
	pDC->Rectangle( &rct);
	pDC->SelectObject( pOldBrush);
	pDC->SelectObject( pOldPen);
	pDC->TextOut( rct.left + 3, rct.top + 2, m_strAskingPrice);	// Border를 감안하여 위치를 조정한다.

	rct.UnionRect( rct, CRect( m_rctGraphRegion.left, m_nIndicatorPosY, m_rctGraphRegion.right,	m_nIndicatorPosY + 1));
	return rct;
}


//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CAskingPriceIndicatorAddInImp::OnAddInToolCommand( const EAICommandType p_eCommandID, const long p_lParam)
{
	if( EAI_CUSTOM_ENABLE_BOUNDMARK != p_eCommandID) return FALSE;

	// (2004.10.22, 배승원) 상/하한가 표시기능에 호가연동 기능을 보강한다.
	EnableBoundMark( p_lParam);	

	// (2004.10.06, 배승원) 수정시 바로 화면에 반영되도록 Redraw시킨다.
	// (2004.12.13, 배승원) NULL Pointer를 확인한다.
	m_pIChartOCX->InvalidateControl();

	return TRUE;
}

BOOL CAskingPriceIndicatorAddInImp::OnAddInToolCommand( long &p_lResult, const EAICommandType p_eCommandID)
{
	switch( p_eCommandID)
	{
		case EAI_CUSTOM_GET_BOUNDMARK_FLAG:	p_lResult = GetBoundMarkFlag();
											break;
		case EAI_CUSTOM_GET_EXISTENCE_BOUNDMARK:	p_lResult = IsExistenceBoundMark();
													break;
		default:	return FALSE;
	}
	return TRUE;
}

// (2004.10.25, 배승원) Mouse의 위치시에, Indicator Cursor를 출력하도록 한다.
BOOL CAskingPriceIndicatorAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	// 1. 먼저 호가연동이 설정된 경우를 처리한다. (호가연동 Mark를 Clear한다.)
	if( m_bOnFireAskingPriceGShare) ClearAskingPriceGShareFireFlag();

	// 2. 호가연동 영역에 Mouse가 위치한 경우를 처리한다.
	if( m_rctAskingPriceMark.PtInRect( point))
	{
		// 2.1 처음 진입하는 경우를 처리한다.
		//	(처음 진입하면서, 현재 Mouse Button이 눌리지 않은 상태이며, 특별한 Cursor Mode가 아닌 경우)
		//		특히, 종목 정보를 받지 않은 경우는 지원하지 않는다.)
		if( !m_bOnSetCaprture && !nFlags && !m_strItemCode.IsEmpty())
		{
			// 2. Mouse Capturing Flag을 TRUE로 설정한다.
			m_bOnSetCaprture = TRUE;

			// 3. 호가연동 Cursor를 등록한다.
			// (2004.10.25, 배승원) SetCapture()후, 현재 설정하는 Cursor를 관리하면서,
			//		ReleaseCapture()할 때까지 수시로 OCX에 Cursor를 설정한다.
			//		(다른 분석도구들이 수시로 설정을 해제하여 추가적인 설정이 잘 되지 않는다. 구조가 좀...)
			AfxSetResourceHandle( ML_DEFAULT_RES);
			m_hAPCursor = AfxGetApp()->LoadCursor( IDC_AP_INDICATOR);
			ASSERT( m_hAPCursor);
		}

		// 2.2 진입후 이동하는 경우를 처리한다.
		if( m_bOnSetCaprture) m_nIndicatorPosY = point.y;	// Set Indicator Drawing Position			(IndicatorPosition)
	}

	// 3. 호가연동 영역 밖에 Mouse가 위치한 경우를 처리한다. (이미 진입한 경우, 벗어나는 동작을 처리한다.)
	else
	{
		// (2004.10.29, 배승원) Capturing의 Release는 Window Deactivate시에도 나타나므로,
		//		그 뒷처리를 위하여 Capturing의 Release의 처리는 OnCaptureChanged()에서 처리토록 한다.
		//		특히 ReleaseCapture()시에 먼저 ClearPreviousIndicatorDraw()될 수 있도록 한다.
		// (2004.11.02, 배승원) Mouse Capturing이 중첩적으로 처리되도록, 중첩 Count와 별도의 Capturing Interface를 이용한다.
		if( m_bOnSetCaprture)
		{
			// (2004.10.25, 배승원) 호가연동 Indicator의 출력이 마무리되어, 출력 Flag을 Reset한다.
			// (2004.10.29, 배승원) Capturing의 Release는 Window Deactivate시에도 나타나므로,
			//		그 뒷처리를 위하여 Capturing의 Release의 처리는 OnCaptureChanged()에서 처리토록 한다.
			// 1. Mouse Capturing Flag을 FALSE로 설정한다.
			m_bOnSetCaprture = FALSE;

			// 3. 호가연동 Cursor를 해제한다.
			// (2004.10.25, 배승원) SetCapture()후, 현재 설정하는 Cursor를 관리하면서,
			//		ReleaseCapture()할 때까지 수시로 OCX에 Cursor를 설정한다.
			//		(다른 분석도구들이 수시로 설정을 해제하여 추가적인 설정이 잘 되지 않는다. 구조가 좀...)
			m_hAPCursor = NULL;
			if( m_pIChartOCX) m_pIChartOCX->SetCursorHandle( m_hAPCursor);

			// 6. Clear Y Position for DrawCursor();
			m_nIndicatorPosY = 0;
		}
	}

	// (2004.10.25, 배승원) SetCapture()후, 현재 설정하는 Cursor를 관리하면서,
	//		ReleaseCapture()할 때까지 수시로 OCX에 Cursor를 설정한다.
	//		(다른 분석도구들이 수시로 설정을 해제하여 추가적인 설정이 잘 되지 않는다. 구조가 좀...)
	if( m_hAPCursor) m_pIChartOCX->SetCursorHandle( m_hAPCursor);

	// (2006/1/22 - Seung-Won, Bae) return FALSE to pass the Event to a next AddIn Routine
	return FALSE;
}

// (2005/8/18 - Seung-Won, Bae) Define AddIn Event ID
typedef enum {	m_eAskingPriceValue = 0};

// (2004.10.25, 배승원) 호가 연동 선택시, 공유를 발생시킨다.
BOOL CAskingPriceIndicatorAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	// 1. 먼저 호가연동 구간에 위치한 상황인지 확인한다.
	if( !( m_rctAskingPriceMark.PtInRect( point) && m_bOnSetCaprture)) return FALSE;

	// 2. 호가연동 처리 Cursor를 보여준다.
	// (2004.10.25, 배승원) SetCapture()후, 현재 설정하는 Cursor를 관리하면서,
	//		ReleaseCapture()할 때까지 수시로 OCX에 Cursor를 설정한다.
	//		(다른 분석도구들이 수시로 설정을 해제하여 추가적인 설정이 잘 되지 않는다. 구조가 좀...)
	AfxSetResourceHandle( ML_DEFAULT_RES);
	m_hAPCursor = AfxGetApp()->LoadCursor( IDC_AP_INDICATOR);
	ASSERT( m_hAPCursor);
	m_pIChartOCX->SetCursorHandle( m_hAPCursor);

	// 3. 호가연동 처리 Flag을 설정한다.
	m_bOnFireAskingPriceGShare = TRUE;

	// 4. 호가 연동 공유를 발생시킨다.
	CString strShareData = m_strItemCode + "?" + m_strAskingPrice;
	m_pIChartOCX->FireGSharedData( _MTEXT( C6_ASKED_PRICE_CHART), strShareData);

	// (2005/8/18 - Seung-Won, Bae) Modify to Fire Shared Data, even if without Asking Price Unit.
	double dFixedAskingPrice = _FnGetPriceUnit_ERROR;
	// (2004.11.01, 배승원) 장구분을 관리하여, 기본 호가 연동에 이용한다.
	if( m_nMarketType != -1 && m_fnFnGetPriceUnit && m_dYClosePrice && m_nRatioByYCP)
	{
		// FnGetPriceUnit( 장구분,  가격,  전일종가,  가격/대비율구분[nFlag 1:가격, 2:대비율] )
		//		Error Value -99999999
		dFixedAskingPrice = m_fnFnGetPriceUnit( m_nMarketType, m_dIndicatedPrice, m_dYClosePrice, m_nRatioByYCP);
	}
	if( dFixedAskingPrice == _FnGetPriceUnit_ERROR) dFixedAskingPrice = m_dIndicatedPrice;

	CString strFixedAskingPrice;
	// (2004.10.26, 배승원) 호가의 정밀도(Precision)을 관리하여 호가 String의 생성에 참고하도록 한다.
	int nDecimal = 0;
	// (2004.10.26, 배승원, 주의!) double을 Int로 변환하는 과정에서 정확한 Int로 변환되지 않는 문제를 해결하기 위해 100배 유효자리수를 얻도록 조정한다.
	nDecimal = ( int)( ( log10( dFixedAskingPrice) * 100) / 100);	
	CString strFormat;
	if( 0 <= nDecimal) strFormat = "%.0f";
	else strFormat.Format( "%%.%df", -nDecimal);
	strFixedAskingPrice.Format( strFormat, dFixedAskingPrice);

	m_pIChartOCX->FireGSharedData( _MTEXT( C6_CODE_ASKED_PRICE), m_strItemCode + "?" + strFixedAskingPrice);
	// (2005/8/18 - Seung-Won, Bae) Fire Event for Asking Price Data
	m_pIChartOCX->FireAddInEvent( AfxGetApp()->m_pszAppName, m_eAskingPriceValue, strFixedAskingPrice, 0, 0);

	return TRUE;	// 호가연동 처리시 다른 Mouse Action이 이어지지 않도록 한다.
}

// (2004.10.25, 배승원) 호가 연동 선택시의 Marking을 해제한다.
BOOL CAskingPriceIndicatorAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	if( m_bOnFireAskingPriceGShare) ClearAskingPriceGShareFireFlag();

	return FALSE;
}

BOOL CAskingPriceIndicatorAddInImp::InvokeAddInStr( LPCTSTR p_szCommandName, LPCTSTR p_szData)
{
	if (strcmp(p_szCommandName, "1") == 0)
	{
		m_pmapOrder = (CMapStringToOb *)atoi(p_szData);
	}
	else if (strcmp(p_szCommandName, "2") == 0)
	{
		m_strItemCode = p_szData;
	}
	else if (strcmp(p_szCommandName, "3") == 0)
	{
		m_bDraw = atoi(p_szData);
	}
	return TRUE;
}
