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
interface IString;
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
	virtual	BOOL		IsLogScale( void) = 0;
	virtual	BOOL		SetLogScale( BOOL p_bReverseScale) = 0;

// (2006/6/25 - Seung-Won, Bae) Graph Handling
public:
	// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
	virtual BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nV, int &p_nG) = 0;
	// (2006/8/8 - Seung-Won, Bae) Remove Graph
	virtual	BOOL	RemoveGraph( int p_nV, int p_nG) = 0;
	// (2006/11/28 - Seung-Won, Bae) Update Graph with Graph Infos. (The Changing of IGraphInfo did not make the Graph Updated.)
	virtual BOOL	UpdateGraphWithGraphInfo( int p_nV, int p_nG, BOOL p_bWithCalculate) = 0;
	// (2007/1/20 - Seung-Won, Bae) Search Indicator
	virtual IString *		FindFirstIndicator( const char *p_szIndicatorName, int &p_nV, int &p_nG, const char *p_szRQ) = 0;
	virtual IString *		FindNextIndicator( const char *p_szIndicatorName, int &p_nV, int &p_nG, const char *p_szRQ) = 0;

// (2006/8/16 - Seung-Won, Bae) Vertical Scale Group Handling
public:
	virtual int									GetFirstVisibleVScaleIndex( void) = 0;
	virtual CScaleBaseData::VERTSCALEPOSITION	GetVScaleGroupYScalePosition( const int p_nVScaleGroupIndex) = 0;
	virtual BOOL								GetVScaleGroupMinMax( const int p_nVScaleGroupIndex, double &p_dMin, double &p_dMax) = 0;
	virtual BOOL								GetVScaleGroupHeadPacketAndDataType( const int p_nVScaleGroupIndex, IPacket *&p_pIPacket, double &p_dDataType) = 0;
	virtual BOOL								GetVScaleMinMaxSetting( const int p_nVScaleGroupIndex, BOOL &p_bIsLock, CScaleBaseData::VERTMINMAXTYPE &p_eMinMaxType, double &p_dMin, double &p_dMax) = 0;
	virtual BOOL								SetVScaleMinMaxSetting( const int p_nVScaleGroupIndex, BOOL p_bIsLock, CScaleBaseData::VERTMINMAXTYPE p_eMinMaxType, double p_dMin, double p_dMax) = 0;
	virtual	BOOL								IsVScaleInvert( const int p_nV) = 0;
	virtual	BOOL								SetVScaleInvert( const int p_nV, BOOL p_bInvertScale) = 0;
	virtual int									GetActiveVScaleIndex( CScaleBaseData::VERTSCALEPOSITION p_eVScalePosition) = 0;
	virtual int									GetVScaleCount( void) = 0;

// (2006/9/20 - Seung-Won, Bae) Horizontal Scale Group Info
public:
	virtual IString *	GetDateCompart( void) = 0;
	virtual IString *	GetTimeCompart( void) = 0;

// (2008/10/20 - Seung-Won, Bae) Retreive RQ
public:
	virtual IString *	GetMainRQ( void) = 0;
};

#endif // !defined(AFX_IBLOCK_H__6372B958_80FB_40CE_B14D_DAC8EAA9AEBA__INCLUDED_)
