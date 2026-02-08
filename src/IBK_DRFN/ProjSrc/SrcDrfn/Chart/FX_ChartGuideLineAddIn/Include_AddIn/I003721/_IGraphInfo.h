// IGraphInfo.h: interface for the IGraphInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGRAPHINFO_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_)
#define AFX_IGRAPHINFO_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>
#include "_AddInVer.h"			// for AddIn Version Define
#include "_IString.h"					// for ILPCSTR

interface IDoubleList;
interface IGraphInfo : public IUnknown
{
protected:
	virtual ~IGraphInfo() {}
		
public:
	// 종찬(05/07/14) 지표 계산에 필요한 조건값을 제공한다.
	virtual IDoubleList *	GetCommonCalculateConditionList( void) = 0;
	virtual IDoubleList *	GetDrawConditionList( void) = 0;
	virtual IDoubleList *	GetCalculateConditionList( const char *pcszSubGraphName) = 0;
	virtual IDoubleList *	GetCalculateConditionList( const int nSubGraphIndex) = 0;

	// 종찬(05/07/15) 계산에 쓰이는 패킷명을 제공한다.
	virtual IString *	GetPacketNameString( void) = 0;
	virtual BOOL		SetPacketName( const char *p_szPacketName) = 0;

	// (2006/8/14 - Seung-Won, Bae) Support BaseLine Data List
	virtual IDoubleList *	GetBaseLineList( void) = 0;

// (2006/7/7 - Seung-Won, Bae) SubGraph Info
public:
	// (2006/7/7 - Seung-Won, Bae) Get Sub Graph Count.
	virtual int			GetSubGraphCount( void) = 0;
	// 서브그래프의 인덱스를 리턴한다.
	virtual int			GetSubGraphIndex(const char* pcszSubGraphName) = 0;	
	// 종찬(05/07/29) 서브그래프의 Show/Hide 여부를 리턴한다.
	virtual BOOL		GetSubGraphShowFlag( const int p_nSubGraphIndex, BOOL &p_bShow) = 0;
	virtual BOOL		SetSubGraphShowFlag( const int p_nSubGraphIndex, BOOL p_bShow) = 0;
	// (2006/8/11 - Seung-Won, Bae) Graph Name
	virtual IString *	GetSubGraphNameString( const int p_nSubGraphIndex) = 0;
	// (2006/8/11 - Seung-Won, Bae) Packet Name
	virtual IString *	GetSubGraphPacketNameString( const int p_nSubGraphIndex) = 0;
	virtual BOOL		SetSubGraphPacketName( const int p_nSubGraphIndex, const char *p_szPacketName) = 0;
	// (2006/8/11 - Seung-Won, Bae) Graph Title
	virtual IString *	GetSubGraphTitleString( const int p_nSubGraphIndex) = 0;
	virtual BOOL		SetSubGraphTitle( const int p_nSubGraphIndex, const char *p_szTitle) = 0;
	// (2006/8/10 - Seung-Won, Bae) Graph Type
	virtual int			GetSubGraphType( const int p_nSubGraphIndex) = 0;
	// (2006/8/10 - Seung-Won, Bae) Graph Style
	virtual int			GetSubGraphStyle( const int p_nSubGraphIndex) = 0;
	virtual BOOL		SetSubGraphStyle( const int p_nSubGraphIndex, const int p_nGraphStyle) = 0;
	// (2006/8/10 - Seung-Won, Bae) Draw Style
	virtual int			GetSubGraphDrawStyle( const int p_nSubGraphIndex) = 0;
	virtual BOOL		SetSubGraphDrawStyle( const int p_nSubGraphIndex, const int p_nGraphDrawStyle) = 0;
	// (2006/8/10 - Seung-Won, Bae) Color
	virtual COLORREF	GetSubGraphColor( const int p_nSubGraphIndex, const int p_nColorIndex) = 0;
	virtual BOOL		SetSubGraphColor( const int p_nSubGraphIndex, const int p_nColorIndex, const COLORREF p_clrColor) = 0;
	virtual BOOL		SetSubGraphColor( const int p_nSubGraphIndex, const COLORREF p_clrColor1, const COLORREF p_clrColor2, const COLORREF p_clrColor3, const COLORREF p_clrColor4, const COLORREF p_clrColor5, const COLORREF p_clrColor6) = 0;
	virtual BOOL		GetSubGraphColorAndFillFlag( const int p_nSubGraphIndex, COLORREF &p_clrColor1, COLORREF &p_clrColor2, COLORREF &p_clrColor3, COLORREF &p_clrColor4, COLORREF &p_clrColor5, COLORREF &p_clrColor6, BOOL &p_bFill1, BOOL &p_bFill2) = 0;
	virtual BOOL		SetSubGraphColorAndFillFlag( const int p_nSubGraphIndex, const COLORREF p_clrColor1, const COLORREF p_clrColor2, const COLORREF p_clrColor3, const COLORREF p_clrColor4, const COLORREF p_clrColor5, const COLORREF p_clrColor6, const BOOL p_bFill1, const BOOL p_bFill2) = 0;
	virtual BOOL		GetSubGraphFillFlag( const int p_nSubGraphIndex, const int p_nFillIndex) = 0;
	virtual BOOL		SetSubGraphFillFlag( const int p_nSubGraphIndex, const int p_nFillIndex, const BOOL p_bFill) = 0;
	// (2006/8/11 - Seung-Won, Bae) Pen
	virtual int			GetSubGraphPenThickness( const int p_nSubGraphIndex, const int p_nPenIndex) = 0;
	virtual BOOL		SetSubGraphPenThickness( const int p_nSubGraphIndex, const int p_nThickness1, const int p_nThickness2, const int p_nThickness3) = 0;

// (2006/7/6 - Seung-Won, Bae) Retrieve Graph Info by string
public:
	virtual IString *	GetGraphDataStringForMaking( void)			= 0;
	virtual IString *	GetAllSubGraphDataStringForMaking( void)	= 0;
	virtual IString *	GetGraphNameString( void)					= 0;

// (2006/7/6 - Seung-Won, Bae) Retrieve the Graph Data
public:
	virtual void	GetGraphDataDrawingData( BOOL &p_bLowHigh, BOOL &p_bLowCur, BOOL &p_bHighCur, BOOL &p_bPrevCur, int &p_nCurData, BOOL &p_bShowOHLC) = 0;
	virtual void	SetGraphDataDrawingData( BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, int p_nCurData, BOOL p_bShowOHLC) = 0;

// (2006/7/6 - Seung-Won, Bae) Retrieve Signal Info
public:
	virtual BOOL	GetSignalShowFlag( BOOL &p_bValidFlag, BOOL &p_bShowSignal)	= 0;
	virtual BOOL	SetSignalShowFlag( BOOL p_bValidFlag, BOOL p_bShowSignal)	= 0;

// (2006/8/8 - Seung-Won, Bae) Graph Info Handling
public:
	virtual IGraphInfo *	CreateCopyObject( void) = 0;
	virtual BOOL			GetCopyDataFrom( IGraphInfo *p_pIGrpahInfo) = 0;
	virtual BOOL			ChangeAllSubGraphTitleFromCalConditionData( void) = 0;
};

#endif // !defined(AFX_IGRAPHINFO_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_)
