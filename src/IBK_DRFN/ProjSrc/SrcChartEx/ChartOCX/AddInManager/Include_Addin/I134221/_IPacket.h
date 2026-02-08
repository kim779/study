// IPacket.h: interface for the IPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPACKET_H__33499AC4_ED71_4247_AB51_30A908195E29__INCLUDED_)
#define AFX_IPACKET_H__33499AC4_ED71_4247_AB51_30A908195E29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseEnum.h"		// for CPacketType::COMMODITY_BOND_NAME
#include "_AddInVer.h"			// for AddIn Version Define
#include "_IString.h"			// for ILPCSTR

interface IDoubleList;
interface IStringList;
interface IPacket : public IUnknown
{
protected:
	virtual ~IPacket()	{}

// [04/10/11] Packet의 정보를 조회 및 설정하는 Interface를 제공한다.
public:
	// [04/10/26] Packet의 Type을 Unit Value(Double)로 조회하는 Interface를 제공한다.
	virtual double		GetPacketTypeUnitValue( void) = 0;
	// [04/11/30] Packet Name을 구하는 Interface를 제공한다.
	virtual IString *	GetPacketNameString( void) = 0;
	// 종찬(05/08/29) 타입을 설정하는 Interface를 제공한다.
	virtual void		SetType( const char *p_szType, const char *p_szExpressionType) = 0;
	// 종찬(05/08/29) 타입값을 가져오는 Interface를 제공한다.
	virtual IString *	GetType(const bool bIsFullType = true) = 0;
	// (2007/2/3 - Seung-Won, Bae) Retrieve the Data Index of Real Data Start.
	virtual int			GetIndexOfRealDataStart( void) = 0;
	// (2007/5/2 - Seung-Won, Bae) Check Market Type.
	virtual CPacketType::COMMODITY_BOND_NAME	GetCommodityBondName( void)	= 0;
	// 분차트 근접시간 적용 - ojtaso (20080811)
	virtual int			GetDateUnitType( void) = 0;

// (2004.10.15, 배승원) Data를 조회 및 설정하는 Interface를 제공한다.
public:
	// [04/10/15] Data의 수를 확인하는 Interface를 제공한다.
	// [04/10/11] 주어지는 값의 Data Index, 또는 그 Data 값이 있어야할 위치 이전의 Index를 구한다. (Sort 전제)
	//			TRUE	-> 범위 안인 경우
	//			FALSE	-> 범위를 벗어나거나 같은 값이 없는 경우
	virtual int		GetDataCount( void) = 0;
	virtual int		GetIndexFromData( double p_dData) = 0;
	// 분차트 근접시간 적용 - ojtaso (20080811)
	virtual int		GetIndexFromData( double p_dData, const char *p_szType) = 0;
	virtual BOOL	GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData) = 0;
	// [04/10/11] 해당 Index의 Data를 조회하는 Interface를 제공한다. (Zero Base)
	virtual BOOL	GetData( const int nIndex, double &p_dData) = 0;
	virtual BOOL	GetData( const int nIndex, ILPCSTR &p_pszData) = 0;
	virtual BOOL	GetDataPastandFuture( int p_nIndex, double &p_dData) = 0;
	// [04/10/15] 해당 Index의 Data를 설정하거나 추가하는 Interface를 제공한다. (Zero Base)
	virtual BOOL	SetData( int p_nIndex, double p_dData) = 0;
	virtual void	AppendData(double dData) = 0;
	// 종찬(05/07/28) Data를 클리어한다. true일경우는 기본데이타도 Clear.(true는 ST.에서는 사용할 필요없을듯)
	virtual void	ClearData(const bool bBaseDataClear = false) = 0;

// [04/12/07] Data List를 조회하는 Interface를 제공한다.
public:
	virtual IDoubleList *	GetDataList( void) = 0;
	virtual IStringList *	GetStrDataList( void) = 0;
	// 현재가 등락률 - ojtaso (20080602)
	virtual void OffsetPacketData(double dData) = 0;
};

#endif // !defined(AFX_IPACKET_H__33499AC4_ED71_4247_AB51_30A908195E29__INCLUDED_)
