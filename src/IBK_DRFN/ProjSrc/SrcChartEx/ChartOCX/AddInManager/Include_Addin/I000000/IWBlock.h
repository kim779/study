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
	virtual	BOOL		IsLogScale( void);
	virtual	BOOL		SetLogScale( BOOL p_bReverseScale);

// (2006/6/25 - Seung-Won, Bae) Graph Handling
protected:
	// (2006/6/25 - Seung-Won, Bae) Search Index of Graph
	virtual BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nV, int &p_nG);
	// (2006/8/8 - Seung-Won, Bae) Remove Graph
	virtual	BOOL	RemoveGraph( int p_nV, int p_nG);
	// (2006/11/28 - Seung-Won, Bae) Update Graph with Graph Infos. (The Changing of IGraphInfo did not make the Graph Updated.)
	virtual BOOL	UpdateGraphWithGraphInfo( int p_nV, int p_nG, BOOL p_bWithCalculate);
	// (2007/1/20 - Seung-Won, Bae) Search Indicator
	virtual IString *		FindFirstIndicator( const char *p_szIndicatorName, int &p_nV, int &p_nG, const char *p_szRQ);
	virtual IString *		FindNextIndicator( const char *p_szIndicatorName, int &p_nV, int &p_nG, const char *p_szRQ);

// (2006/8/16 - Seung-Won, Bae) Vertical Scale Group Handling
protected:
	virtual int									GetFirstVisibleVScaleIndex( void);
	virtual CScaleBaseData::VERTSCALEPOSITION	GetVScaleGroupYScalePosition( const int p_nVScaleGroupIndex);
	virtual BOOL								GetVScaleGroupMinMax( const int p_nVScaleGroupIndex, double &p_dMin, double &p_dMax);
	virtual BOOL								GetVScaleGroupHeadPacketAndDataType( const int p_nVScaleGroupIndex, IPacket *&p_pIPacket, double &p_dDataType);
	virtual BOOL								GetVScaleMinMaxSetting( const int p_nVScaleGroupIndex, BOOL &p_bIsLock, CScaleBaseData::VERTMINMAXTYPE &p_eMinMaxType, double &p_dMin, double &p_dMax);
	virtual BOOL								SetVScaleMinMaxSetting( const int p_nVScaleGroupIndex, BOOL p_bIsLock, CScaleBaseData::VERTMINMAXTYPE p_eMinMaxType, double p_dMin, double p_dMax);
	virtual	BOOL								IsVScaleInvert( const int p_nV);
	virtual	BOOL								SetVScaleInvert( const int p_nV, BOOL p_bInvertScale);
	virtual int									GetActiveVScaleIndex( CScaleBaseData::VERTSCALEPOSITION p_eVScalePosition);
	virtual int									GetVScaleCount( void);
	virtual int									GetScaleTextType( void);
	virtual int									GetHorizontalScaleHeight( void) const;		// XScale 높이 지정 - ojtaso (20080429)

// (2006/9/20 - Seung-Won, Bae) Horizontal Scale Group Info
public:
	virtual IString *	GetDateCompart( void);
	virtual IString *	GetTimeCompart( void);

// (2008/10/20 - Seung-Won, Bae) Retrieve RQ
protected:
	virtual IString *	GetMainRQ( void);

// (2009/10/15 - Seung-Won, Bae) Retrieve Coordinate Infos.
protected:
	virtual IString *	GetCoordinateInfos( IPacket *&p_pIPacketX, IPacket *&p_pIPacketY, double &p_dMinY, double &p_dMaxY, double &p_dDataType, BOOL &p_bLog, BOOL &p_bInvert);
};

#endif // !defined(AFX_IWBLOCK_H__A94523D1_8619_412A_ADCC_2BAFCAEA3869__INCLUDED_)
