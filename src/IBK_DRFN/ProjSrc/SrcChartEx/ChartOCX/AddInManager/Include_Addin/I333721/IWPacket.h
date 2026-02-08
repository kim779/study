// IWPacket.h: interface for the CIWPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWPACKET_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_)
#define AFX_IWPACKET_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/DLL_Load/PacketBase.h"		// for CPacket
#include "../_IWUnknown.h"
#include "_IPacket.h"
#include "_IChartOCX.h"										// for IChartOCX

// (2004.10.11, 배승원) Packet Data Object의 Interface Wrapper Class를 정의한다.
class CIWPacket : public CIWUnknown, public IPacket
{
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IPacket, CIWPacket, CPacket, m_pPacket)

// (2006/7/27 - Seung-Won, Bae) 
public:
	CPacket *	GetPacketPtr( void)	{	return m_pPacket;	}

// [04/10/11] Packet의 정보를 조회하는 Interface를 제공한다.
public:
	// [04/10/26] Packet의 Type을 Unit Value(Double)로 조회하는 Interface를 제공한다.
	virtual double		GetPacketTypeUnitValue( void);
	// [04/11/30] Packet Name을 구하는 Interface를 제공한다.
	virtual IString *	GetPacketNameString( void);
	// 종찬(05/08/29) 타입을 설정하는 Interface를 제공한다.
	virtual void		SetType( const char *p_szType, const char *p_szExpressionType);
	// 종찬(05/08/29) 타입값을 가져오는 Interface를 제공한다.
	virtual IString *	GetType(const bool bIsFullType = true);

// (2004.10.15, 배승원) Data를 조회 및 설정하는 Interface를 제공한다.
public:
	// [04/10/15] Data의 수를 확인하는 Interface를 제공한다.
	// [04/10/11] 주어지는 값의 Data Index, 또는 그 Data 값이 있어야할 위치 이전의 Index를 구한다. (Sort 전제)
	//			TRUE	-> 범위 안인 경우
	//			FALSE	-> 범위를 벗어나거나 같은 값이 없는 경우
	virtual int		GetDataCount( void);
	virtual int		GetIndexFromData( double p_dData);
	virtual BOOL	GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData);
	// [04/10/11] 해당 Index의 Data를 조회하는 Interface를 제공한다. (Zero Base)
	virtual BOOL	GetData( const int nIndex, double &p_dData);
	virtual BOOL	GetData( const int nIndex, ILPCSTR &p_pszData);
	virtual BOOL	GetDataPastandFuture( int p_nIndex, double &p_dData);
	// [04/10/15] 해당 Index의 Data를 설정하거나 추가하는 Interface를 제공한다. (Zero Base)
	virtual BOOL	SetData( int p_nIndex, double p_dData);
	virtual void	AppendData(double dData);
	// 종찬(05/07/28) Data를 클리어한다. true일경우는 기본데이타도 Clear.(true는 ST.에서는 사용할 필요없을듯)
	virtual void	ClearData(const bool bBaseDataClear);

// [04/12/07] Data List를 조회하는 Interface를 제공한다.
public:
	virtual IDoubleList *	GetDataList( void);
	virtual IStringList *	GetStrDataList( void);
};

#endif // !defined(AFX_IWPACKET_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_)
