// IWBlock.cpp: implementation of the CIWBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWBlock.h"

#include "../../Include_Chart/Dll_Load/PacketBase.h"		// for CPacket
#include "IWPacket.h"										// for CIWPacket
#include "IWGraphInfo.h"									// for CIWGraphInfo
#include "IWString.h"										// for CIWString

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWBlock)

//////////////////////////////////////////////////////////////////////
// (2006/6/15 - Seung-Won, Bae) Get Regions
//////////////////////////////////////////////////////////////////////

BOOL CIWBlock::GetGraphRegions( RECT &p_rctNoFrame, RECT &p_rctNoFrameAndMargin)
{
	if( !m_pBlock) return FALSE;
	CRect rctNoFrame, rctNoFrameAndMargin;
	BOOL bResult = m_pBlock->GetGraphRegions( rctNoFrame, rctNoFrameAndMargin);
	p_rctNoFrame			= rctNoFrame;
	p_rctNoFrameAndMargin	= rctNoFrameAndMargin;
	return bResult;
}
BOOL CIWBlock::GetHorzScaleRegion( RECT &p_rctTopScale, RECT &p_rctBottomScale)
{
	if( !m_pBlock) return FALSE;
	CRect rctTopScale, rctBottomScale;
	BOOL bResult = m_pBlock->GetHorzScaleRegion( rctTopScale, rctBottomScale);
	p_rctTopScale		= rctTopScale;
	p_rctBottomScale	= rctBottomScale;
	return bResult;
}
BOOL CIWBlock::GetVertScaleRegion( RECT &p_rctLeftScale, RECT &p_rctRightScale)
{
	if( !m_pBlock) return FALSE;
	CRect rctLeftScale, rctRightScale;
	BOOL bResult = m_pBlock->GetVertScaleRegion( rctLeftScale, rctRightScale);
	p_rctLeftScale	= rctLeftScale;
	p_rctRightScale	= rctRightScale;
	return bResult;
}
RECT CIWBlock::GetGraphDrwingRegion( const IBlock::REGIONPOSITION p_rgnType)
{
	if( !m_pBlock) return RECT();
	return m_pBlock->GetGraphDrwingRegion( ( CBlock::REGIONPOSITION)p_rgnType);
}

//////////////////////////////////////////////////////////////////////
// (2006/6/15 - Seung-Won, Bae) Get Data Info of Scale
//////////////////////////////////////////////////////////////////////

// (2006/6/15 - Seung-Won, Bae) Get Packet
IPacket *CIWBlock::GetHorzScaleDataPacket( void)
{
	if( !m_pBlock) return NULL;
	CPacket *pPacket = m_pBlock->GetHorzScaleDataPacket();
	if( !pPacket) return NULL;
	return CIWPacket::CreateObject( pPacket);
}

BOOL CIWBlock::IsLogScale( void)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->IsLogScale();
}

BOOL CIWBlock::SetLogScale( BOOL p_bReverseScale)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->SetLogScale( p_bReverseScale);
}

//////////////////////////////////////////////////////////////////////
// (2006/6/25 - Seung-Won, Bae) Get Data Info of Graph
//////////////////////////////////////////////////////////////////////

// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
BOOL CIWBlock::GetIndexOfGraph( const char *p_szGraphName, int &p_nV, int &p_nG)
{
	p_nV = p_nG = -1;
	if( !m_pBlock) return FALSE;
	return m_pBlock->GetIndexOfGraph( p_szGraphName, p_nV, p_nG);
}

// (2006/8/8 - Seung-Won, Bae) Remove Graph
BOOL CIWBlock::RemoveGraph( int p_nV, int p_nG)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->DeleteGraph( p_nV, p_nG);
}

// (2006/11/28 - Seung-Won, Bae) Update Graph with Graph Infos. (The Changing of IGraphInfo did not make the Graph Updated.)
BOOL CIWBlock::UpdateGraphWithGraphInfo( int p_nV, int p_nG, BOOL p_bWithCalculate)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->UpdateGraphWithGraphInfo( p_nV, p_nG, p_bWithCalculate);
}

// (2007/1/20 - Seung-Won, Bae) Search Indicator
IString *CIWBlock::FindFirstIndicator( const char *p_szIndicatorName, int &p_nV, int &p_nG, const char *p_szRQ)
{
	if( !m_pBlock) return NULL;
	CString strGraphName;
	if( !m_pBlock->FindFirstIndicator( p_szIndicatorName, p_nV, p_nG, strGraphName, p_szRQ)) return NULL;
	return CIWString::CreateObject( strGraphName);
}
IString *CIWBlock::FindNextIndicator( const char *p_szIndicatorName, int &p_nV, int &p_nG, const char *p_szRQ)
{
	if( !m_pBlock) return NULL;
	CString strGraphName;
	if( !m_pBlock->FindNextIndicator( p_szIndicatorName, p_nV, p_nG, strGraphName, p_szRQ)) return NULL;
	return CIWString::CreateObject( strGraphName);
}

//////////////////////////////////////////////////////////////////////
// (2006/8/16 - Seung-Won, Bae) Vertical Scale Group Handling
//////////////////////////////////////////////////////////////////////

int CIWBlock::GetFirstVisibleVScaleIndex( void)
{
	if( !m_pBlock) return -1;
	return m_pBlock->GetFirstVisibleVScaleIndex();
}

CScaleBaseData::VERTSCALEPOSITION CIWBlock::GetVScaleGroupYScalePosition( const int p_nVScaleGroupIndex)
{
	if( !m_pBlock) return CScaleBaseData::VERT_NONE;
	return m_pBlock->GetVScalePosition( p_nVScaleGroupIndex);
}

BOOL CIWBlock::GetVScaleGroupMinMax( const int p_nVScaleGroupIndex, double &p_dMin, double &p_dMax)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->GetYScaleMinMax( p_nVScaleGroupIndex, p_dMin, p_dMax);
}

BOOL CIWBlock::GetVScaleGroupHeadPacketAndDataType( const int p_nVScaleGroupIndex, IPacket *&p_pIPacket, double &p_dDataType)
{
	if( !m_pBlock) return FALSE;
	CPacket *pPacket = NULL;
	BOOL bResult = m_pBlock->GetHeadPacketAndDataType( p_nVScaleGroupIndex, pPacket, p_dDataType);
	if( !pPacket) p_pIPacket = NULL;
	else p_pIPacket = CIWPacket::CreateObject( pPacket);

	return bResult;
}

BOOL CIWBlock::GetVScaleMinMaxSetting( const int p_nVScaleGroupIndex, BOOL &p_bIsLock, CScaleBaseData::VERTMINMAXTYPE &p_eMinMaxType, double &p_dMin, double &p_dMax)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->GetVScaleMinMaxSetting( p_nVScaleGroupIndex, p_bIsLock, p_eMinMaxType, p_dMin, p_dMax);
}

BOOL CIWBlock::SetVScaleMinMaxSetting( const int p_nVScaleGroupIndex, BOOL p_bIsLock, CScaleBaseData::VERTMINMAXTYPE p_eMinMaxType, double p_dMin, double p_dMax)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->SetVScaleMinMaxSetting( p_nVScaleGroupIndex, p_bIsLock, p_eMinMaxType, p_dMin, p_dMax);
}

BOOL CIWBlock::IsVScaleInvert( const int p_nV)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->IsVScaleInvert( p_nV);
}

BOOL CIWBlock::SetVScaleInvert( const int p_nV, BOOL p_bInvertScale)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->SetVScaleInvert( p_nV, p_bInvertScale);
}

int CIWBlock::GetActiveVScaleIndex( CScaleBaseData::VERTSCALEPOSITION p_eVScalePosition)
{
	if( !m_pBlock) return -1;
	return m_pBlock->GetActiveVScaleIndex( p_eVScalePosition);
}

int CIWBlock::GetVScaleCount( void)
{
	if( !m_pBlock) return 0;
	return m_pBlock->GetVScaleCount();
}

// XScale Text 툴팁표시 - ojtaso (20080429)
int CIWBlock::GetScaleTextType( void)
{
	if( !m_pBlock) return 0;
	return (int)m_pBlock->GetScaleTextType();
}

// XScale 높이 지정 - ojtaso (20080429)
int CIWBlock::GetHorizontalScaleHeight( void) const
{
	if( !m_pBlock) return -1;
	return m_pBlock->GetHorizontalScaleHeight();
}

//////////////////////////////////////////////////////////////////////
// (2006/9/20 - Seung-Won, Bae) Horizontal Scale Group Info
//////////////////////////////////////////////////////////////////////

IString *CIWBlock::GetDateCompart( void)
{
	if( !m_pBlock) return NULL;
	return CIWString::CreateObject( m_pBlock->GetDateCompart());
}

IString *CIWBlock::GetTimeCompart( void)
{
	if( !m_pBlock) return NULL;
	return CIWString::CreateObject( m_pBlock->GetTimeCompart());
}

// (2008/10/20 - Seung-Won, Bae) Retreive RQ
IString *CIWBlock::GetMainRQ( void)
{
	if( !m_pBlock) return NULL;

	IString *pIString = CIWString::CreateObject( m_pBlock->GetMasterValueRQ());
	return pIString;
}

// (2009/10/15 - Seung-Won, Bae) Retrieve Coordinate Infos.
IString *CIWBlock::GetCoordinateInfos( IPacket *&p_pIPacketX, IPacket *&p_pIPacketY, double &p_dMinY, double &p_dMaxY, double &p_dDataType, BOOL &p_bLog, BOOL &p_bInvert)
{
	if( !m_pBlock) return NULL;
	CPacket *pPacketX = NULL, *pPacketY = NULL;

	CString strRQ = m_pBlock->GetCoordinateInfos( pPacketX, pPacketY, p_dMinY, p_dMaxY, p_dDataType, p_bLog, p_bInvert);

	IString *pIString = NULL;
	if( pPacketX) p_pIPacketX = CIWPacket::CreateObject( pPacketX);
	if( pPacketY) p_pIPacketY = CIWPacket::CreateObject( pPacketY);
	if( !strRQ.IsEmpty()) pIString = CIWString::CreateObject( strRQ);
	return pIString;
}
