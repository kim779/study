// IWBlock.cpp: implementation of the CIWBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWBlock.h"

#include "../../Include_Chart/Dll_Load/PacketBase.h"		// for CPacket
#include "IWPacket.h"										// for CIWPacket
#include "IWGraphInfo.h"									// for CIWGraphInfo

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

BOOL CIWBlock::GetLogAndReverseScaleFlag( BOOL &p_bLogScale, BOOL &p_bReverseScale)
{
	if( !m_pBlock) return FALSE;
	p_bLogScale = m_pBlock->IsLogScale();
	p_bReverseScale = m_pBlock->HasInvertScale();
	return TRUE;
}

BOOL CIWBlock::SetLogAndReverseScaleFlag( BOOL p_bLogScale, BOOL p_bReverseScale)
{
	if( !m_pBlock) return FALSE;
	m_pBlock->SetLogScale( p_bLogScale);
	m_pBlock->SetAllInvertScale( p_bReverseScale);
	return TRUE;
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

BOOL CIWBlock::ChangeGraphInfoAndData_NotUsed( int p_nV, int p_nG, IGraphInfo *p_pIGraphInfo, BOOL p_bWithCalculate)
{
	// (2006/11/27 - Seung-Won, Bae) Not Supported.
	return FALSE;
/*
	if( !m_pBlock || !p_pIGraphInfo) return FALSE;
	CIWGraphInfo *pIWGraphInfo = ( CIWGraphInfo *)p_pIGraphInfo;
	return m_pBlock->ChangeIndicatorInfoAndGraphData( p_nV, p_nG, pIWGraphInfo->GetObjectPtr(), p_bWithCalculate);
*/
}

// (2006/8/8 - Seung-Won, Bae) Remove Graph
BOOL CIWBlock::RemoveGraph( int p_nV, int p_nG)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->DeleteGraph( p_nV, p_nG);
}

//////////////////////////////////////////////////////////////////////
// (2009/10/1 - Seung-Won, Bae) It is not used any more.
//////////////////////////////////////////////////////////////////////
IPacket *CIWBlock::GetPacket_NotUsed( const char *p_szPacketName)
{
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// (2006/8/16 - Seung-Won, Bae) Vertical Scale Group Handling
//////////////////////////////////////////////////////////////////////

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
