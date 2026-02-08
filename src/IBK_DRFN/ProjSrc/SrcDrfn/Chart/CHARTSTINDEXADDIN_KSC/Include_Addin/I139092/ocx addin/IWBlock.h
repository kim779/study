// IWBlock.h: interface for the CIWBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWBLOCK_H__A94523D1_8619_412A_ADCC_2BAFCAEA3869__INCLUDED_)
#define AFX_IWBLOCK_H__A94523D1_8619_412A_ADCC_2BAFCAEA3869__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../_IWUnknown.h"
#include "_IBlock.h"
#include "../../Include_Chart/Block.h"						// for CBlock

// (2006/6/13 - Seung-Won, Bae) Interface Wrapper Class for Block Object
class CIWBlock : public CIWUnknown, public IBlock  
{
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IBlock, CIWBlock, CBlock, m_pBlock)

// (2006/6/15 - Seung-Won, Bae) Get Regions
protected:
	virtual BOOL	GetGraphRegions( RECT &p_rctNoFrame, RECT &p_rctNoFrameAndMargin);
	virtual BOOL	GetHorzScaleRegion( RECT &p_rctTopScale, RECT &p_rctBottomScale);
	virtual BOOL	GetVertScaleRegion( RECT &p_rctLeftScale, RECT &p_rctRightScale);
	virtual RECT	GetGraphDrwingRegion( const IBlock::REGIONPOSITION p_rgnType);

// (2006/6/15 - Seung-Won, Bae) Get Data Info of Scale
protected:
	// (2006/6/15 - Seung-Won, Bae) Get Packet
	virtual IPacket *	GetHorzScaleDataPacket( void);
protected:
	virtual	BOOL		GetLogAndReverseScaleFlag( BOOL &p_bLogScale, BOOL &p_bReverseScale);
	virtual	BOOL		SetLogAndReverseScaleFlag( BOOL p_bLogScale, BOOL p_bReverseScale);

// (2006/6/25 - Seung-Won, Bae) Graph Handling
protected:
	// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
	virtual BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nV, int &p_nG);
	// (2006/8/8 - Seung-Won, Bae) Update Graph Info
	virtual BOOL	ChangeGraphInfoAndData_NotUsed( int p_nV, int p_nG, IGraphInfo *p_pIGraphInfo, BOOL p_bWithCalculate);
	// (2006/8/8 - Seung-Won, Bae) Remove Graph
	virtual	BOOL	RemoveGraph( int p_nV, int p_nG);

// (2006/6/30 - Seung-Won, Bae) ETC Temp Interface for Old Module
protected:
	virtual IPacket *	GetPacket( const char *p_szPacketName);

// (2006/8/16 - Seung-Won, Bae) Vertical Scale Group Handling
protected:
	virtual CScaleBaseData::VERTSCALEPOSITION	GetVScaleGroupYScalePosition( const int p_nVScaleGroupIndex);
	virtual BOOL								GetVScaleGroupMinMax( const int p_nVScaleGroupIndex, double &p_dMin, double &p_dMax);
	virtual BOOL								GetVScaleGroupHeadPacketAndDataType( const int p_nVScaleGroupIndex, IPacket *&p_pIPacket, double &p_dDataType);
};

#endif // !defined(AFX_IWBLOCK_H__A94523D1_8619_412A_ADCC_2BAFCAEA3869__INCLUDED_)
