// Block.h: interface for the CBlock class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCK_H__4CF85EB7_9538_4C3E_8FEB_5BD2D6CDBBD9__INCLUDED_)
#define AFX_BLOCK_H__4CF85EB7_9538_4C3E_8FEB_5BD2D6CDBBD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseEnum.h"		// for CScaleBaseData

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/6/15
// Comments		: Interface Class of CBlockImp
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

class CPacket;
class CIndicatorInfo;
class CBlock : public CObject  
{
public:
	enum REGIONPOSITION
	{
		CENTER = 0,
		TOP,
		BOTTOM,
	};

// (2006/6/15 - Seung-Won, Bae) Get Regions
public:
	virtual BOOL	GetGraphRegions( CRect &p_rctNoFrame, CRect &p_rctNoFrameAndMargin)	= 0;
	virtual BOOL	GetHorzScaleRegion( CRect &p_rctTopScale, CRect &p_rctBottomScale)	= 0;
	virtual BOOL	GetVertScaleRegion( CRect &p_rctLeftScale, CRect &p_rctRightScale)	= 0;
	virtual CRect	GetGraphDrwingRegion( const CBlock::REGIONPOSITION p_rgnType)		= 0;

// (2006/6/15 - Seung-Won, Bae) Get Data Info of Scale
public:
	virtual BOOL									GetYScaleMinMax( int p_nVScaleIndex, double &p_dMin, double &p_dMax) = 0;
	virtual BOOL									GetHeadPacketAndDataType( int p_nVertScaleGroupIndex, CPacket *&p_pPacket, double &p_dDataType)	= 0;
	virtual	CScaleBaseData::VERTSCALEPOSITION		GetVScalePosition( const int p_nVScaleGroupIndex) = 0;
	// (2006/6/15 - Seung-Won, Bae) Get Packet
	virtual CPacket *								GetHorzScaleDataPacket( void) = 0;
	// (2006/10/24 - Seung-Won, Bae) Retrieve the Scale Index of First Visible VScale.
	virtual int										GetFirstVisibleVScaleIndex( void) = 0;
	// (2006/11/12 - Seung-Won, Bae) Get VScale Min/Max Type
	virtual CScaleBaseData::VERTMINMAXTYPE			GetVScaleMinMaxType( int p_nVScaleIndex) = 0;
	virtual BOOL									GetVScaleMinMaxSetting( const int p_nVScaleGroupIndex, BOOL &p_bIsLock, CScaleBaseData::VERTMINMAXTYPE &p_eMinMaxType, double &p_dMin, double &p_dMax) = 0;
	virtual BOOL									SetVScaleMinMaxSetting( const int p_nVScaleGroupIndex, BOOL p_bIsLock, CScaleBaseData::VERTMINMAXTYPE p_eMinMaxType, double p_dMin, double p_dMax) = 0;
public:
	virtual	BOOL		IsLogScale( void) = 0;
	virtual	BOOL		SetLogScale( BOOL p_bLogScale) = 0;
	virtual BOOL		IsVScaleInvert( const int p_nV) = 0;
	virtual BOOL		SetVScaleInvert( const int p_nV, BOOL p_bInvertScale) = 0;
	virtual	BOOL		HasInvertScale( void) = 0;
	virtual	BOOL		SetAllInvertScale( BOOL p_bInvertScale) = 0;
	virtual int			GetActiveVScaleIndex( CScaleBaseData::VERTSCALEPOSITION p_eVScalePosition) = 0;
	virtual int			GetVScaleCount( void) = 0;
	// (2009/10/15 - Seung-Won, Bae) Retrieve Coordinate Infos.
	virtual CString		GetCoordinateInfos( CPacket *&p_pPacketX, CPacket *&p_pPacketY, double &p_dMinY, double &p_dMaxY, double &p_dDataType, BOOL &p_bLog, BOOL &p_bInvert) = 0;

// (2006/6/25 - Seung-Won, Bae) Graph Handling
public:
	// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
	virtual BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nV, int &p_nG) = 0;
	// (2006/8/8 - Seung-Won, Bae) Remove Graph
	virtual	BOOL	DeleteGraph( int p_nV, int p_nG) = 0;
	// (2006/11/28 - Seung-Won, Bae) Update Graph with Graph Infos. (The Changing of IGraphInfo did not make the Graph Updated.)
	virtual BOOL	UpdateGraphWithGraphInfo( int p_nV, int p_nG, BOOL p_bWithCalculate) = 0;

// (2007/1/20 - Seung-Won, Bae) Search Indicator
public:
	virtual BOOL	FindFirstIndicator( const char *p_szIndicatorName, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ)	= 0;
	virtual BOOL	FindNextIndicator( const char *p_szIndicatorName, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ)	= 0;

// (2006/9/20 - Seung-Won, Bae) Horizontal Scale Group Info
public:
	virtual CString		GetDateCompart( void) = 0;
	virtual CString 	GetTimeCompart( void) = 0;

	// XScale Text 툴팁표시 - ojtaso (20080429)
	virtual CScaleBaseData::TEXTCOMPARTTYPE		GetScaleTextType( void) = 0;
	// XScale 높이 지정 - ojtaso (20080429)
	virtual int GetHorizontalScaleHeight() const = 0;

// (2008/10/22 - Seung-Won, Bae) Retreive RQ
public:
	virtual const char *	GetMasterValueRQ( void) = 0;
};

#endif // !defined(AFX_BLOCK_H__4CF85EB7_9538_4C3E_8FEB_5BD2D6CDBBD9__INCLUDED_)
