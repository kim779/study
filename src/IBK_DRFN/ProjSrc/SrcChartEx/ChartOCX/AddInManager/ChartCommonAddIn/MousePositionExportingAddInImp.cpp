// MousePositionExportingAddInImp.cpp: implementation of the CMousePositionExportingAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chartcommonaddin.h"
#include "MousePositionExportingAddInImp.h"

#include <math.h>	
#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IBlock.h"				// for IBlock
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMousePositionExportingAddInImp::CMousePositionExportingAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	m_pIBlock = NULL;

	// (2007/1/17 - Seung-Won, Bae) Support Data Index with Default.
	m_bXDataExport = FALSE;

	// (2007/1/19 - Seung-Won, Bae) Support Not-Change Data Filter
	m_bSameDataFilter = TRUE;
}


CMousePositionExportingAddInImp::~CMousePositionExportingAddInImp()
{
	if( m_pIBlock )
	{
		m_pIBlock->Release();
		m_pIBlock = NULL;
	}
}


// ( 2006 / 11 / 08 - Sang-Woo, Cho ) - Multiple Item in DLL
CString CMousePositionExportingAddInImp::m_strAddInItemName = "MOUSE_POSITION";


//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CMousePositionExportingAddInImp )
	ONADDINEVENT( OnMouseMove)
END_ADDIN_EVENT_MAP()


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 09
// Modifier		: 
// Comments		: Mouse가 움직일때 FireAddInEvent()로 Map으로 현재 Mouse Point의
//				: X값과 Y값을 Map으로 보내준다.
//-----------------------------------------------------------------------------
BOOL CMousePositionExportingAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	// (2006/3/2 - Seung-Won, Bae) Manage State for Resource Loading of Cursor 
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	// 
	if( !GetScaleData( point ) )
		return FALSE;
	
	CString strXScaleAndYScaleData = m_strXScaleFormatedData + "|" + m_strYScaleFormatedData;
	
	// Fire Event for Asking Price Data
	if( !m_bSameDataFilter || m_dData != m_dYScaleOriginalData || m_lData != (long)m_dXScaleOriginalData)
		m_pIChartOCX->FireAddInEvent( m_strAddInItemName, 0, strXScaleAndYScaleData, m_dYScaleOriginalData, (long)m_dXScaleOriginalData);
	else
	{
		m_dData = m_dYScaleOriginalData;
		m_lData = (long)m_dXScaleOriginalData;
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 09
// Return BOOL	: 
// Param  CPoint ptPoint : 
// Comments		: 현재 Mouse의 point로 X값과 Y값을 얻어온다.
//-----------------------------------------------------------------------------
BOOL CMousePositionExportingAddInImp::GetScaleData( CPoint ptPoint )
{
	// 1. Block값을 얻어온다.
	if( !GetIBlock( ptPoint ) )
		return FALSE;


	// 2. Graph의 영역을 얻어온다.
	if( !GetGraphBlockRegion( ptPoint ) )
		return FALSE;
	

	if( !GetXScaleDataInPoint(ptPoint) )
		return FALSE;
	
	if( !GetYScaleDataInPoint(ptPoint) )
		return FALSE;

	IPacket *pIPacket = NULL;
	double dDataType = 0.0L;
	BOOL bResult = m_pIBlock->GetVScaleGroupHeadPacketAndDataType( m_nVScaleGroupIndex, pIPacket, dDataType);
	if( !bResult)
	{
		if( pIPacket) pIPacket->Release();
		return FALSE;
	}

	if( pIPacket)
	{
		// 호가의 정밀도(Precision)을 관리하여 호가 String의 생성에 참고하도록 한다.
		int nDecimal = 0;

		if( dDataType != 0)
		{
			m_dYScaleOriginalData = int( m_dYScaleOriginalData / dDataType + 0.5) * dDataType;
			// double을 Int로 변환하는 과정에서 정확한 Int로 변환되지 않는 문제를 해결하기 위해 100배 유효자리수를 얻도록 조정한다.
			nDecimal = ( int)( ( log10( dDataType) * 100) / 100);	
		}
	
		CString strFormat;
		if( 0 <= nDecimal)
			strFormat = "%.0f";
		else
			strFormat.Format( "%%.%df", -nDecimal);
				

		m_strYScaleFormatedData.Format( strFormat, m_dYScaleOriginalData);
		
#ifdef _DEBUG
	TRACE( "%s : %f\r\n", m_strYScaleFormatedData, m_dYScaleOriginalData);
#endif

		pIPacket->Release();
	}

	if( m_pIBlock) m_pIBlock->Release();
	m_pIBlock = NULL;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 09
// Return BOOL	: 
// Param  CPoint ptPoint : 
// Comments		: Mouse Position 값으로 IBlock을 얻어온다.
//-----------------------------------------------------------------------------
BOOL CMousePositionExportingAddInImp::GetIBlock( CPoint ptPoint )
{
	// 1. Mouse Point 값으로 Row, Column을 얻어온다.
	int nRow, nColumn;
	if( !m_pIChartManager->GetBlockIndex( ptPoint, nRow, nColumn))
		return FALSE;

	if( m_pIBlock )
	{
		m_pIBlock->Release();
		m_pIBlock= NULL;
	}

	// 2. Row, Column값으로 Block을 얻어온다.
	m_pIBlock = m_pIChartManager->GetBlock( nRow, nColumn );
	if( !m_pIBlock )
		return FALSE;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 09
// Return BOOL	: 
// Param  CPoint ptPoint : 
// Comments		: GraphDrawRegion, BlockRegion을 구하고 현재 Cursor의 영역이
//				: BlockRegion 안에 있는지 점검한다.
//-----------------------------------------------------------------------------
BOOL CMousePositionExportingAddInImp::GetGraphBlockRegion( CPoint ptPoint )
{
	// 1. IBlock 정보가 없으면 Return한다.
	if( m_pIBlock == NULL )
		return FALSE;

	// 2. Block의 Region을 구한다.
	CRect rctNoFrameAndMargin;
	m_pIBlock->GetGraphRegions( m_rctBlockRegion, rctNoFrameAndMargin);

	// 3. Block에서의 Graph Resion을 구한다.
	m_rctGraphDrawingRegion = m_pIBlock->GetGraphDrwingRegion();

	// 4. Block Region을 조정한다.
	m_rctBlockRegion.left = m_rctGraphDrawingRegion.left;
	m_rctBlockRegion.right = m_rctGraphDrawingRegion.right;
	
	// 5. 현재 Mouse Position이 BlockRegion밖에 있을 경우에는 return한다.
	if( !m_rctBlockRegion.PtInRect(ptPoint) ) 
		return FALSE;

	return TRUE;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 11 / 09
// Return BOOL	: 
// Param  CPoint ptPoint : 
// Comments		: Mouse Position으로 Index를 구하고 Packet값을 얻어온다.
//-----------------------------------------------------------------------------
BOOL CMousePositionExportingAddInImp::GetXScaleDataInPoint( CPoint ptPoint )
{
	// 1. 현재 화면에 보이는 Data의 Start / End Index를 구한다.
	int nStartIndex, nEndIndex;
	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex ) )
		return FALSE;


	// 2. Mouse Point의 Data Index를 구한다.
	int nDataOnPage = nEndIndex - nStartIndex + 1;
	double dBarWidth = double( m_rctGraphDrawingRegion.Width() ) / double( nDataOnPage );
	int nDataIndex = nStartIndex + int( double( ptPoint.x - m_rctBlockRegion.left ) / dBarWidth );


	// 3. XScale의 Data Index에서 Packet Type을 얻는다.
	IPacket *pIPacket = m_pIBlock->GetHorzScaleDataPacket();
	if( pIPacket == NULL )
		return FALSE;


	// 2. Data를 구한다.
	ILPCSTR szXScaleData;
	ILPCSTR szType = pIPacket->GetType();
	m_strPacketType = szType;
	
	// (2007/1/17 - Seung-Won, Bae) Support Data Index with Default.
	if( !m_bXDataExport) m_dXScaleOriginalData = nDataIndex;
	else if( !strcmp( m_strPacketType, _MTEXT( C6_CHARACTER)) )
	{
		m_bPacketTypeIsString = TRUE;
		pIPacket->GetData(nDataIndex, szXScaleData);

		m_strXScaleFormatedData = szXScaleData;
		m_dXScaleOriginalData = nDataIndex;
	}

	else
	{
		m_bPacketTypeIsString = FALSE;
		if( !pIPacket->GetData(nDataIndex, m_dXScaleOriginalData) )
			pIPacket->GetDataPastandFuture( nDataIndex, m_dXScaleOriginalData );

		// 6. Span Data에 해당되는 문자열을 구한다.
		GetStringfromDataIdx( nDataIndex );
	}

	pIPacket->Release();
	pIPacket = NULL;

	return TRUE;
}


		
BOOL CMousePositionExportingAddInImp::GetYScaleDataInPoint( CPoint ptPoint )
{ 
	double	dViewMin;					// View내에 대상 Graph된 View의 Minimun Scale 값이다.
	double	dViewMax;					// View내에 대상 Graph된 View의 Maximum Scale 값이다.

	m_nVScaleGroupIndex = m_pIBlock->GetFirstVisibleVScaleIndex();
	m_pIBlock->GetVScaleGroupMinMax( m_nVScaleGroupIndex, dViewMin, dViewMax);

	// Block의 Log/뒤집기 설정을 확인하여 정확하게 Data를 변환한다. (가격<->Y Pos)
	// 가격차트의 Block에 Log/뒤집기 설정값을 확인한다.
	BOOL bLog = m_pIBlock->IsLogScale();
	BOOL bReverse = m_pIBlock->IsVScaleInvert( m_nVScaleGroupIndex);

	m_dYScaleOriginalData = m_pIChartOCX->ConvertYPositionToData( ptPoint.y,
		m_rctGraphDrawingRegion.top, m_rctGraphDrawingRegion.bottom, dViewMax, dViewMin, bLog, bReverse);

	return TRUE;
}


// [05/01/19] AddIn Object가 비정형 Interface를 제공할 수 있도록 제공한다.
BOOL CMousePositionExportingAddInImp::InvokeAddIn( int p_nCommandType, long p_lData)
{
	m_bXDataExport = p_lData;
	return TRUE;
}

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 09 / 15
// Return BOOL	: 
// Param  int nDataIdx : 
// Param  CString &p_strSpan : 
// Comments		: 해당 Data Index의 시간 정보를 구한다.
//-----------------------------------------------------------------------------
BOOL CMousePositionExportingAddInImp::GetStringfromDataIdx( int nDataIdx )
{
	if( !m_bPacketTypeIsString )
	{
		ILPCSTR szDateCompart = m_pIBlock->GetDateCompart();
		m_strDateCompartment = szDateCompart;

		ILPCSTR szTimeCompart = m_pIBlock->GetTimeCompart();
		m_strTimeCompartment = szTimeCompart;
	}

	long lXScaleOriginalData = (long)m_dXScaleOriginalData;
	CString szXScaleOriginalData;
	szXScaleOriginalData.Format("%d", lXScaleOriginalData);

	// 3. Packet Type을 구한다.
	int nPacketTypeLength = m_strPacketType.GetLength();

	// 4. PacketType의 길이와 Data의 길이가 다르면 '0'으로 채워준다.
	for( int i = nPacketTypeLength - szXScaleOriginalData.GetLength() ; i ; i-- )
		szXScaleOriginalData = "0" + szXScaleOriginalData;

	char strChar = m_strPacketType.GetAt(0);
	m_strXScaleFormatedData = szXScaleOriginalData.GetAt(0);

	// 5. XScale에 사용되는 구분자 의 Type을 구분한다.
	CString strCompartment = m_strDateCompartment;
	BOOL bUseOneCharDateCompartment = ( m_strDateCompartment.GetLength() > 1 ) ? FALSE : TRUE;
	BOOL bUseOneCharTimeCompartment = ( m_strTimeCompartment.GetLength() > 1 ) ? FALSE : TRUE;

	
	// "월"과 "분"의 PacketType이 같이 'M'을 사용하기 때문에 구분해준다.
	BOOL bIsMonth = FALSE;				
	CString strPacketChar = "";
	
	// 6. PacketType의 길이만큼 문자열을 parsing한다.
	ML_SET_LANGUAGE_RES();
	for( int nIdx = 1 ; nPacketTypeLength > nIdx ; nIdx++ )
	{
		strPacketChar = m_strPacketType.GetAt(nIdx);

		// 6.1 PacketType내에서 Data의 Type이 바뀌면 해당되는 구분자를 문자열에 추가해준다.
		if( strChar != strPacketChar )
		{
			// 6.1.1 Date의 구분자가 "년 월 일"을 사용할 경우 
			if( !bUseOneCharDateCompartment )
			{
				if( strChar == 'Y' && strPacketChar == 'M' )
				{
					strCompartment.LoadString( IDS_YEAR);
					bIsMonth = TRUE;
				}
				
				else if( strChar == 'M' && strPacketChar == 'D' )		strCompartment.LoadString( IDS_MONTH);
				else if( strChar == 'D' && strPacketChar == 'H' )		strCompartment.LoadString( IDS_DAY);
			}

			// 6.1.2 Time의 구분자가 "시 분 초"를 사용할 경우 
			if( !bUseOneCharTimeCompartment )
			{
				if( strChar == 'H' && strPacketChar == 'M' )
				{
					strCompartment.LoadString( IDS_HOUR);
					bIsMonth = FALSE;
				}
				
				else if( strChar == 'M' && strPacketChar == 'S' )		strCompartment.LoadString( IDS_MINUTE);
				else if( strChar == 'S' && strPacketChar == 'N' )		strCompartment.LoadString( IDS_SECOND);
			}


			// 6.1.3 현재 구분자가 일반 문자일경우 날짜에서 시간으로 변경될 때 구분 문자를 변경해준다.
			if( strChar == 'H' && strPacketChar == 'M' && bUseOneCharTimeCompartment )
				strCompartment = m_strTimeCompartment;

			m_strXScaleFormatedData = m_strXScaleFormatedData + strCompartment + szXScaleOriginalData.GetAt(nIdx);

			strChar = m_strPacketType.GetAt(nIdx);
		}

		// 6.2 PacketType이 변경되지 않으면 문자를 
		else
		{
			// 6.2.1 PacketType의 변경이 없으면 Span문자를 더해준다.
			m_strXScaleFormatedData = m_strXScaleFormatedData + szXScaleOriginalData.GetAt(nIdx);
			
			// 6.2.2 szMarkTime을 모두 parsing하고 나서 마지막으로 구분자를 붙여야 될 경우를 확인하고 붙여준다.
			CString strComp;
			if( nPacketTypeLength == ( nIdx + 1 ) )
			{
				if( strChar == 'Y' && !bUseOneCharDateCompartment )
				{
					strComp.LoadString( IDS_YEAR);
					m_strXScaleFormatedData = m_strXScaleFormatedData + strComp;
				}
				else if( strChar == 'M' && !bUseOneCharDateCompartment && bIsMonth )
				{
					strComp.LoadString( IDS_MONTH);
					m_strXScaleFormatedData = m_strXScaleFormatedData + strComp;
				}
				else if( strChar == 'D' && !bUseOneCharDateCompartment )
				{
					strComp.LoadString( IDS_DAY);
					m_strXScaleFormatedData = m_strXScaleFormatedData + strComp;
				}
				else if( strChar == 'H' && !bUseOneCharTimeCompartment )
				{
					strComp.LoadString( IDS_HOUR);
					m_strXScaleFormatedData = m_strXScaleFormatedData + strComp;
				}
				else if( strChar == 'M' && !bUseOneCharTimeCompartment && !bIsMonth)
				{
					strComp.LoadString( IDS_MINUTE);
					m_strXScaleFormatedData = m_strXScaleFormatedData + strComp;
				}
				else if( strChar == 'S' && !bUseOneCharTimeCompartment )
				{
					strComp.LoadString( IDS_SECOND);
					m_strXScaleFormatedData = m_strXScaleFormatedData + strComp;
				}
			}
		}
	}

	return TRUE;
}
