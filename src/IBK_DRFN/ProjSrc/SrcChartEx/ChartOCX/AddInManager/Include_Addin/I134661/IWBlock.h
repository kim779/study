// IWBlock.h: interface for the CIWBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWBLOCK_H__A94523D1_8619_412A_ADCC_2BAFCAEA3869__INCLUDED_)
#define AFX_IWBLOCK_H__A94523D1_8619_412A_ADCC_2BAFCAEA3869__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/Block.h"		// for CBlock
#include "../_IWUnknown.h"
#include "_IBlock.h"
#include "_IChartOCX.h"							// for IChartOCX

// (2006/6/13 - Seung-Won, Bae) Interface Wrapper Class for Block Object
class CIWBlock : public CIWUnknown, public IBlock  
{
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IBlock, CIWBlock, CBlock, m_pBlock)

// (2006/6/15 - Seung-Won, Bae) Get Regions
public:
	virtual BOOL	GetGraphRegions( CRect &p_rctNoFrame, CRect &p_rctNoFrameAndMargin);
	virtual BOOL	GetHorzScaleRegion( CRect &p_rctTopScale, CRect &p_rctBottomScale);
	virtual BOOL	GetVertScaleRegion( CRect &p_rctLeftScale, CRect &p_rctRightScale);
	virtual CRect	GetGraphDrwingRegion( const IBlock::REGIONPOSITION p_rgnType);

// (2006/6/15 - Seung-Won, Bae) Get Data Info of Scale
public:
	virtual BOOL		GetYScaleMinMax( int p_nVScaleIndex, double &p_dMin, double &p_dMax);
	virtual	BOOL		GetLogAndReverseScaleFlag( BOOL &p_bLogScale, BOOL &p_bReverseScale);
	virtual BOOL		GetHeadPacketAndDataType( int p_nVertScaleGroupIndex, IPacket *&p_pIPacket, double &p_dDataType);
	// (2006/6/15 - Seung-Won, Bae) Get Packet
	virtual IPacket *	GetHorzScaleDataPacket( void);

// (2006/6/25 - Seung-Won, Bae) Get Data Info of Graph
public:
	// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
	virtual BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nV, int &p_nG);
};

#endif // !defined(AFX_IWBLOCK_H__A94523D1_8619_412A_ADCC_2BAFCAEA3869__INCLUDED_)
