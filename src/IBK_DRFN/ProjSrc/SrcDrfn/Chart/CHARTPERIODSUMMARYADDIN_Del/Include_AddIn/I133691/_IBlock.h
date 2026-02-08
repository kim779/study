// IBlock.h: interface for the IBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IBLOCK_H__6372B958_80FB_40CE_B14D_DAC8EAA9AEBA__INCLUDED_)
#define AFX_IBLOCK_H__6372B958_80FB_40CE_B14D_DAC8EAA9AEBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_AddInVer.h"			// for AddIn Version Define
#include "BlockBaseEnum.h"		// for CScaleBaseData

interface IPacket;
interface IGraphInfo;
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
	virtual BOOL	GetGraphRegions( RECT &p_rctNoFrame, RECT &p_rctNoFrameAndMargin)				= 0;
	virtual BOOL	GetHorzScaleRegion( RECT &p_rctTopScale, RECT &p_rctBottomScale)				= 0;
	virtual BOOL	GetVertScaleRegion( RECT &p_rctLeftScale, RECT &p_rctRightScale)				= 0;
	virtual RECT	GetGraphDrwingRegion( const IBlock::REGIONPOSITION p_rgnType = IBlock::CENTER)	= 0;

// (2006/6/15 - Seung-Won, Bae) Get Data Info of Scale
public:
	// (2006/6/15 - Seung-Won, Bae) Get Packet
	virtual IPacket *	GetHorzScaleDataPacket( void) = 0;
public:
	virtual	BOOL		GetLogAndReverseScaleFlag( BOOL &p_bLogScale, BOOL &p_bReverseScale) = 0;
	virtual	BOOL		SetLogAndReverseScaleFlag( BOOL p_bLogScale, BOOL p_bReverseScale) = 0;

// (2006/6/25 - Seung-Won, Bae) Graph Handling
public:
	// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
	virtual BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nV, int &p_nG) = 0;
	// (2006/8/8 - Seung-Won, Bae) Update Graph Info
	virtual BOOL	ChangeGraphInfoAndData_NotUsed( int p_nV, int p_nG, IGraphInfo *p_pIGraphInfo, BOOL p_bWithCalculate) = 0;
	// (2006/8/8 - Seung-Won, Bae) Remove Graph
	virtual	BOOL	RemoveGraph( int p_nV, int p_nG) = 0;

// (2006/6/30 - Seung-Won, Bae) ETC Temp Interface for Old Module
public:
	virtual IPacket *	GetPacket( const char *p_szPacketName) = 0;

// (2006/8/16 - Seung-Won, Bae) Vertical Scale Group Handling
public:
	virtual CScaleBaseData::VERTSCALEPOSITION	GetVScaleGroupYScalePosition( const int p_nVScaleGroupIndex) = 0;
	virtual BOOL								GetVScaleGroupMinMax( const int p_nVScaleGroupIndex, double &p_dMin, double &p_dMax) = 0;
	virtual BOOL								GetVScaleGroupHeadPacketAndDataType( const int p_nVScaleGroupIndex, IPacket *&p_pIPacket, double &p_dDataType) = 0;
};

#endif // !defined(AFX_IBLOCK_H__6372B958_80FB_40CE_B14D_DAC8EAA9AEBA__INCLUDED_)
