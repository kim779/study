// IWBlock.cpp: implementation of the CIWBlock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWBlock.h"

#include "../../Include_Chart/Dll_Load/PacketBase.h"		// for CPacket
#include "IWPacket.h"										// for CIWPacket

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWBlock)

//////////////////////////////////////////////////////////////////////
// (2006/6/15 - Seung-Won, Bae) Get Regions
//////////////////////////////////////////////////////////////////////

BOOL CIWBlock::GetGraphRegions( CRect &p_rctNoFrame, CRect &p_rctNoFrameAndMargin)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->GetGraphRegions( p_rctNoFrame, p_rctNoFrameAndMargin);
}
BOOL CIWBlock::GetHorzScaleRegion( CRect &p_rctTopScale, CRect &p_rctBottomScale)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->GetHorzScaleRegion( p_rctTopScale, p_rctBottomScale);
}
BOOL CIWBlock::GetVertScaleRegion( CRect &p_rctLeftScale, CRect &p_rctRightScale)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->GetVertScaleRegion( p_rctLeftScale, p_rctRightScale);
}
CRect CIWBlock::GetGraphDrwingRegion( const IBlock::REGIONPOSITION p_rgnType)
{
	if( !m_pBlock) return CRect();
	return m_pBlock->GetGraphDrwingRegion( ( CBlock::REGIONPOSITION)p_rgnType);
}

//////////////////////////////////////////////////////////////////////
// (2006/6/15 - Seung-Won, Bae) Get Data Info of Scale
//////////////////////////////////////////////////////////////////////

BOOL CIWBlock::GetYScaleMinMax( int p_nVScaleIndex, double &p_dMin, double &p_dMax)
{
	if( !m_pBlock) return FALSE;
	return m_pBlock->GetYScaleMinMax( p_nVScaleIndex, p_dMin, p_dMax);
}

BOOL CIWBlock::GetLogAndReverseScaleFlag( BOOL &p_bLogScale, BOOL &p_bReverseScale)
{
	if( !m_pBlock) return FALSE;
	p_bLogScale = m_pBlock->IsLogScale();
	p_bReverseScale = m_pBlock->HasInvertScale();
	return TRUE;
}

BOOL CIWBlock::GetHeadPacketAndDataType( int p_nVertScaleGroupIndex, IPacket *&p_pIPacket, double &p_dDataType)
{
	if( !m_pBlock) return FALSE;
	CPacket *pPacket = NULL;
	BOOL bResult = m_pBlock->GetHeadPacketAndDataType( p_nVertScaleGroupIndex, pPacket, p_dDataType);
	if( !pPacket) p_pIPacket = NULL;
	else p_pIPacket = CIWPacket::CreateObject( pPacket);

	return bResult;
}

// (2006/6/15 - Seung-Won, Bae) Get Packet
IPacket *CIWBlock::GetHorzScaleDataPacket( void)
{
	if( !m_pBlock) return NULL;
	CPacket *pPacket = m_pBlock->GetHorzScaleDataPacket();
	if( !pPacket) return NULL;
	return CIWPacket::CreateObject( pPacket);
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
