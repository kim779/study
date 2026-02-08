// IBlock.h: interface for the IBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IBLOCK_H__6372B958_80FB_40CE_B14D_DAC8EAA9AEBA__INCLUDED_)
#define AFX_IBLOCK_H__6372B958_80FB_40CE_B14D_DAC8EAA9AEBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_AddInVer.h"		// for AddIn Version Define
#include "_IPacket.h"		// for IPacket

interface IBlock : public IUnknown  
{
protected:
	virtual ~IBlock()	{}

public:
	enum REGIONPOSITION
	{
		CENTER = 0,
		TOP,
		BOTTOM,
	};

// (2006/6/15 - Seung-Won, Bae) Get Regions
public:
	virtual BOOL	GetGraphRegions( CRect &p_rctNoFrame, CRect &p_rctNoFrameAndMargin)				= 0;
	virtual BOOL	GetHorzScaleRegion( CRect &p_rctTopScale, CRect &p_rctBottomScale)				= 0;
	virtual BOOL	GetVertScaleRegion( CRect &p_rctLeftScale, CRect &p_rctRightScale)				= 0;
	virtual CRect	GetGraphDrwingRegion( const IBlock::REGIONPOSITION p_rgnType = IBlock::CENTER)	= 0;

// (2006/6/15 - Seung-Won, Bae) Get Data Info of Scale
public:
	virtual BOOL		GetYScaleMinMax( int p_nVScaleIndex, double &p_dMin, double &p_dMax) = 0;
	virtual	BOOL		GetLogAndReverseScaleFlag( BOOL &p_bLogScale, BOOL &p_bReverseScale) = 0;
	virtual BOOL		GetHeadPacketAndDataType( int p_nVertScaleGroupIndex, IPacket *&p_pIPacket, double &p_dDataType) = 0;
	// (2006/6/15 - Seung-Won, Bae) Get Packet
	virtual IPacket *	GetHorzScaleDataPacket( void) = 0;

// (2006/6/25 - Seung-Won, Bae) Get Data Info of Graph
public:
	// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
	virtual BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nV, int &p_nG) = 0;
};

#endif // !defined(AFX_IBLOCK_H__6372B958_80FB_40CE_B14D_DAC8EAA9AEBA__INCLUDED_)
