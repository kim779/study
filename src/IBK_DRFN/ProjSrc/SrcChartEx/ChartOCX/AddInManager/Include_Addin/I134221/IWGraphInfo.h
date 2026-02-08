// IWGraphInfo.h: interface for the CIWGraphInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWGRAPHINFO_H__D3FCC53E_FF7F_4CF0_830D_0402C03D7E7E__INCLUDED_)
#define AFX_IWGRAPHINFO_H__D3FCC53E_FF7F_4CF0_830D_0402C03D7E7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../_IWUnknown.h"
#include "_IGraphInfo.h"
#include "IndicatorInfo.h"	// for delete

class CIWGraphInfo : public CIWUnknown, public IGraphInfo 
{
// (2004.10.07, 배승원) Interface Wrapper에서 IUnknown의 Interface를 손쉽게 지원할 수 있도록 Macro를 정의한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER_REFERENCE( m_pIndicatorInfo)
	_IWRAPPER_IUNKNOWN_MANAGER_MGR( IGraphInfo, CIWGraphInfo)
	_IWRAPPER_IUNKNOWN_MANAGER_CREATE( IGraphInfo, CIWGraphInfo, CIndicatorInfo, m_pIndicatorInfo)
protected:
	static long	m_nObjectCount;
protected:
	CIWGraphInfo()
	{
		m_bAutoDelete		= FALSE;
		m_nSubGraphIndex	= -1;
		m_pSubGraphData		= NULL;
		_IWRAPPER_IUNKNOWN_CREATE_TRACE( CIWGraphInfo)
	}
	~CIWGraphInfo()
	{
		_IWRAPPER_IUNKNOWN_DESTROY_TRACE( CIWGraphInfo)
	}

// (2006/8/11 - Seung-Won, Bae) Sub Graph Data Pointer Cache.
protected:
	int					m_nSubGraphIndex;
	CSubGraphData *		m_pSubGraphData;
protected:
	CSubGraphData *		GetSubGraphData( const int p_nSubGraphIndex);
	
protected:
	// (2007/3/27 - Seung-Won, Bae) Get Indicator Info Type
	virtual IString *	GetIndicatorGroupNameString( void);
	virtual IString *	GetIndicatorNameString( void);

	// 종찬(05/07/14) 지표 설정값을 제공한다.
	virtual IDoubleList *	GetCommonCalculateConditionList( void);
	virtual IDoubleList *	GetDrawConditionList( void);
	virtual IDoubleList *	GetCalculateConditionList( const char *pcszSubGraphName);
	virtual IDoubleList *	GetCalculateConditionList( const int nSubGraphIndex);

	// 종찬(05/07/15) 계산에 쓰이는 패킷명을 제공한다.
	virtual IString *	GetPacketNameString( void);
	virtual BOOL		SetPacketName( const char *p_szPacketName);

	// (2006/8/14 - Seung-Won, Bae) Support BaseLine Data List
	virtual IDoubleList *	GetBaseLineList( void);

// (2006/7/7 - Seung-Won, Bae) SubGraph Info
protected:
	// (2006/7/7 - Seung-Won, Bae) Get Sub Graph Count.
	virtual int			GetSubGraphCount( void);
	// 서브그래프의 인덱스를 리턴한다.
	virtual int			GetSubGraphIndex(const char* pcszSubGraphName);	
	// 종찬(05/07/29) 서브그래프의 Show/Hide 여부를 리턴한다.
	virtual BOOL		GetSubGraphShowFlag( const int p_nSubGraphIndex, BOOL &p_bShow);
	virtual BOOL		SetSubGraphShowFlag( const int p_nSubGraphIndex, BOOL p_bShow);
	// (2006/8/11 - Seung-Won, Bae) Graph Name
	virtual IString *	GetSubGraphNameString( const int p_nSubGraphIndex);
	// (2006/8/11 - Seung-Won, Bae) Packet Name
	virtual IString *	GetSubGraphPacketNameString( const int p_nSubGraphIndex);
	virtual BOOL		SetSubGraphPacketName( const int p_nSubGraphIndex, const char *p_szPacketName);
	// (2006/8/11 - Seung-Won, Bae) Graph Title
	virtual IString *	GetSubGraphTitleString( const int p_nSubGraphIndex);
	virtual BOOL		SetSubGraphTitle( const int p_nSubGraphIndex, const char *p_szTitle);
	// (2006/8/10 - Seung-Won, Bae) Graph Type
	virtual int			GetSubGraphType( const int p_nSubGraphIndex);
	// (2006/8/10 - Seung-Won, Bae) Graph Style
	virtual int			GetSubGraphStyle( const int p_nSubGraphIndex);
	virtual BOOL		SetSubGraphStyle( const int p_nSubGraphIndex, const int p_nGraphStyle);
	// (2006/8/10 - Seung-Won, Bae) Draw Style
	virtual int			GetSubGraphDrawStyle( const int p_nSubGraphIndex);
	virtual BOOL		SetSubGraphDrawStyle( const int p_nSubGraphIndex, const int p_nGraphDrawStyle);
	// (2006/8/10 - Seung-Won, Bae) Color (ZBI : with Zero Base Index)
	virtual COLORREF	GetSubGraphColorWithZBI( const int p_nSubGraphIndex, const int p_nColorIndex);
	virtual BOOL		SetSubGraphColorWithZBI( const int p_nSubGraphIndex, const int p_nColorIndex, const COLORREF p_clrColor);
	virtual BOOL		SetSubGraphColor( const int p_nSubGraphIndex, const COLORREF p_clrColor1, const COLORREF p_clrColor2, const COLORREF p_clrColor3, const COLORREF p_clrColor4, const COLORREF p_clrColor5, const COLORREF p_clrColor6);
	virtual BOOL		GetSubGraphColorAndFillFlag( const int p_nSubGraphIndex, COLORREF &p_clrColor1, COLORREF &p_clrColor2, COLORREF &p_clrColor3, COLORREF &p_clrColor4, COLORREF &p_clrColor5, COLORREF &p_clrColor6, BOOL &p_bFill1, BOOL &p_bFill2);
	virtual BOOL		SetSubGraphColorAndFillFlag( const int p_nSubGraphIndex, const COLORREF p_clrColor1, const COLORREF p_clrColor2, const COLORREF p_clrColor3, const COLORREF p_clrColor4, const COLORREF p_clrColor5, const COLORREF p_clrColor6, const BOOL p_bFill1, const BOOL p_bFill2);
	virtual BOOL		GetSubGraphFillFlag( const int p_nSubGraphIndex, const int p_nFillIndex);
	virtual BOOL		SetSubGraphFillFlag( const int p_nSubGraphIndex, const int p_nFillIndex, const BOOL p_bFill);
	// (2006/8/11 - Seung-Won, Bae) Pen (ZBI : with Zero Base Index)
	virtual int			GetSubGraphPenThicknessWithZBI( const int p_nSubGraphIndex, const int p_nPenIndex);
	virtual BOOL		SetSubGraphPenThickness( const int p_nSubGraphIndex, const int p_nThickness1, const int p_nThickness2, const int p_nThickness3);

// (2006/7/6 - Seung-Won, Bae) Retrieve Graph Info by string
protected:
	virtual IString *	GetGraphDataStringForMaking( void);
	virtual IString *	GetAllSubGraphDataStringForMaking( void);
	virtual IString *	GetGraphNameString( void);

// (2006/7/6 - Seung-Won, Bae) Retrieve the Graph Data
protected:
	virtual void	GetGraphDataDrawingData( BOOL &p_bLowHigh, BOOL &p_bLowCur, BOOL &p_bHighCur, BOOL &p_bPrevCur, int &p_nCurData, BOOL &p_bShowOHLC);
	virtual void	SetGraphDataDrawingData( BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, int p_nCurData, BOOL p_bShowOHLC);

// (2006/7/6 - Seung-Won, Bae) Retrieve Signal Info
protected:
	virtual BOOL	GetSignalShowFlag( BOOL &p_bValidFlag, BOOL &p_bShowSignal);
	virtual BOOL	SetSignalShowFlag( BOOL p_bValidFlag, BOOL p_bShowSignal);

// (2006/8/8 - Seung-Won, Bae) Graph Info Handling
protected:
	virtual IGraphInfo *	CreateCopyObject( void);
	virtual BOOL			GetCopyDataFrom( IGraphInfo *p_pIGrpahInfo);
	virtual BOOL			ChangeAllSubGraphTitleFromCalConditionData( void);
};

#endif // !defined(AFX_IWGRAPHINFO_H__D3FCC53E_FF7F_4CF0_830D_0402C03D7E7E__INCLUDED_)
