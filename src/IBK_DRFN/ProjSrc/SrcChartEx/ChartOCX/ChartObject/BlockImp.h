// BlockImp.h: interface for the CBlockImp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKIMP_H__FF8C9947_0D6F_43FB_86AA_F41B38C9A26C__INCLUDED_)
#define AFX_BLOCKIMP_H__FF8C9947_0D6F_43FB_86AA_F41B38C9A26C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/Block.h"				// for CBlock

#include "ChartObjectBaseData.h"
#include "VertScaleGroupList.h"					// for CVertScaleGroupList

class CMainBlockImp;
class CMainBlockBaseData;
class CBlocksColumn;
class CSelectedSubGraphDeque;
class CDrawingRegionCalData;
class CBlockImp : public CBlock
{
protected:
	CBlockImp( CMainBlockBaseData* pMainBlockBaseData, CBlocksColumn* pBlocksColumn);

protected:
	CMainBlockBaseData *	m_pMainBlockBaseData;
	CBlocksColumn *			m_pBlocksColumn;
	CBlockData				m_blockData;			// block에 대한 data
	CVertScaleGroupList		m_vertScaleGroupList;	// 세로 scale가 같은 것끼리 group

// Create
public:
	static CBlockImp *	Make( CMainBlockBaseData* pMainBlockBaseData, CBlocksColumn* pBlocksColumn);
	// 그래프 생성후 계산 여부 : xScaleManager - ojtaso (20070701)
	void				BuildGraphsAndScales( char *p_szBlockData, BOOL& bCalc);
	// RQ로 검색 : 복수종목 - ojtaso (20070118)
	void				BuildGraphsAndScales(LPVOID pPacketRQ, char *p_szBlockData);
	void				GetBlockAllData( CStringList &p_slBlockData) const;

// Delete
// (2007/2/16 - Seung-Won, Bae) Protect the redestructing on Destructor.
protected:
	virtual ~CBlockImp()	{	m_bOnDestructor = TRUE;	}
	BOOL	m_bOnDestructor;
public:
	// Block
	static void		Delete( CBlockImp *pBlock);
	void			DeleteFromParent( void);
	// Graph
	virtual	BOOL	DeleteGraph( int p_nV, int p_nG);

// Draw
public:
	void	DrawBlockBackground( CDC* pDC, const CRect& blockRegion, const int interBlockHalfGap);
	void	DrawHorzAndVertScale( CDC* pDC, const CRect& blockRegion);
	void	CalculateDrawingRegion( CDC* pDC, const CRect& blockRegion, const CVertScaleRegionWidth& vertScaleRegionWidth);
	void	DrawAllGraphs( CDC* pDC, const CRect& blockRegion);

// Query & Update
public:
	// Basic Info
	CMainBlockImp *	GetMainBlock( void) const;
	CBlocksColumn *				GetBlocksColumn( void);
	CBlockData &				GetBlockData( void);
	const CBlockData &			GetBlockData( void) const;
	CVertScaleGroupList &		GetVertScaleGroupList();
	const CVertScaleGroupList &	GetVertScaleGroupList() const;
	// Region
	virtual BOOL	GetGraphRegions( CRect &p_rctNoFrame, CRect &p_rctNoFrameAndMargin);
	virtual BOOL	GetHorzScaleRegion( CRect &p_rctTopScale, CRect &p_rctBottomScale);
	virtual BOOL	GetVertScaleRegion( CRect &p_rctLeftScale, CRect &p_rctRightScale);
	virtual CRect	GetGraphDrwingRegion( const CBlock::REGIONPOSITION p_rgnType);
	// Hirizontal Scale
	virtual CString		GetDateCompart( void);
	virtual CString 	GetTimeCompart( void);
	CDisplayDataMinMax	GetViewHorzMinMax( void);
	// XScale Text 툴팁표시 - ojtaso (20080429)
	CScaleBaseData::TEXTCOMPARTTYPE		GetScaleTextType( void);
	// XScale 높이 지정 - ojtaso (20080429)
	virtual int GetHorizontalScaleHeight() const;
	// Vertical Scale
	void											SetAllScaleGridType(const CScaleBaseData::SCALEGRIDTYPE gridType);
	virtual int										GetFirstVisibleVScaleIndex( void);
	virtual	CScaleBaseData::VERTSCALEPOSITION		GetVScalePosition( const int p_nVScaleGroupIndex);
	virtual BOOL									SetVScaleMinMaxSetting( const int p_nVScaleGroupIndex, BOOL p_bIsLock, CScaleBaseData::VERTMINMAXTYPE p_eMinMaxType, double p_dMin, double p_dMax);
	virtual BOOL									GetVScaleMinMaxSetting( const int p_nVScaleGroupIndex, BOOL &p_bIsLock, CScaleBaseData::VERTMINMAXTYPE &p_eMinMaxType, double &p_dMin, double &p_dMax);
	virtual CScaleBaseData::VERTMINMAXTYPE			GetVScaleMinMaxType( int p_nVScaleIndex);
	virtual BOOL									GetYScaleMinMax( int p_nVScaleIndex, double &p_dMin, double &p_dMax);
	virtual	BOOL									IsLogScale( void);
	virtual	BOOL									SetLogScale( BOOL p_bLogScale);
	virtual BOOL									IsVScaleInvert( const int p_nV);
	virtual BOOL									SetVScaleInvert( const int p_nV, BOOL p_bInvertScale);
	virtual	BOOL									HasInvertScale( void);
	virtual	BOOL									SetAllInvertScale( BOOL p_bInvertScale);
	virtual int										GetActiveVScaleIndex( CScaleBaseData::VERTSCALEPOSITION p_eVScalePosition);
	virtual int										GetVScaleCount( void)	{	return m_vertScaleGroupList.GetCount();	}
	// Graph
	virtual BOOL	GetIndexOfGraph( const char *p_szGraphName, int &p_nV, int &p_nG);
	virtual BOOL	UpdateGraphWithGraphInfo( int p_nV, int p_nG, BOOL p_bWithCalculate);
	virtual BOOL	FindFirstIndicator( const char *p_szIndicatorName, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ);
	virtual BOOL	FindNextIndicator( const char *p_szIndicatorName, int &p_nV, int &p_nG, CString &p_strGraphName, const char *p_szRQ);
	// Packet
	virtual BOOL		GetHeadPacketAndDataType( int p_nVertScaleGroupIndex, CPacket *&p_pPacket, double &p_dDataType);
	CString				GetHorzScaleDataRQ( void);		// (2009/10/2 - Seung-Won, Bae) Get Horizontal Scale RQ for X Scale of each block with different time scale.
	virtual CPacket *	GetHorzScaleDataPacket( void);	// (2009/10/1 - Seung-Won, Bae) Get Horizontal Scale Packet for X Scale of each block with different time scale.

// (2008/10/22 - Seung-Won, Bae) Retreive RQ
protected:
	virtual const char *	GetMasterValueRQ( void);

// (2009/10/15 - Seung-Won, Bae) Retrieve Coordinate Infos.
protected:
	virtual CString		GetCoordinateInfos( CPacket *&p_pPacketX, CPacket *&p_pPacketY, double &p_dMinY, double &p_dMaxY, double &p_dDataType, BOOL &p_bLog, BOOL &p_bInvert);
};

#endif // !defined(AFX_BLOCKIMP_H__FF8C9947_0D6F_43FB_86AA_F41B38C9A26C__INCLUDED_)
