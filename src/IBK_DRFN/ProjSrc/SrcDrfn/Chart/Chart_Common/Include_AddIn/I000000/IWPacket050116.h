// IWPacket050116.h: interface for the CIWPacket050116 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWPACKET050116_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_)
#define AFX_IWPACKET050116_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의!	050116 Version의 AddIn에서 이용되는 OCX측 Interface의 Wrapper로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		허용되지 않는 편집은 050116 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			배승원
//			ChartPatternAddIn			GZone			정서정
//			ChartGuideLine				GZone			남상윤
//////////////////////////////////////////////////////////////////////

// (2004.10.11, 배승원) Packet Data Object의 Interface Wrapper Class를 정의한다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IWUnknown.h"
#include "IPacket050116.h"

class CPacket;
class CIWPacket050116 : public CIWUnknown, public IPacket050116
{
public:
	CIWPacket050116( CPacket *p_pPacket);
	virtual ~CIWPacket050116();

// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.11, 배승원) Packet Data Object의 Pointer를 관리한다.
protected:
	CPacket *	m_pPacket;

// [04/10/11] Packet의 정보를 조회하는 Interface를 제공한다.
public:
	// [04/10/11] Packet의 Type을 String으로 조회하는 Interface를 제공한다.
	virtual BOOL		GetPacketTypeString( CString &p_strPacketType);
	// [04/10/26] Packet의 Type을 Unit Value(Double)로 조회하는 Interface를 제공한다.
	virtual double		GetPacketTypeUnitValue( void);
	// [04/11/30] Packet Name을 구하는 Interface를 제공한다.
	virtual BOOL		GetPacketName( CString &p_strPacketName);

// [04/10/11] 주어지는 값의 Data Index, 또는 그 Data 값이 있어야할 위치 이전의 Index를 구한다. (Sort 전제)
//			TRUE	-> 범위 안인 경우
//			FALSE	-> 범위를 벗어나거나 같은 값이 없는 경우
public:
	virtual BOOL		GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData);

// (2004.10.15, 배승원) Data를 조회 및 설정하는 Interface를 제공한다.
public:
	// [04/10/15] Data의 수를 확인하는 Interface를 제공한다.
	virtual int							GetDataCount( void);
	// [04/10/11] 해당 Index의 Data를 조회하는 Interface를 제공한다. (Zero Base)
	virtual double						GetData( int p_nIndex);
	// [04/10/15] 해당 Index의 Data를 설정하는 Interface를 제공한다. (Zero Base)
	virtual BOOL						SetData( int p_nIndex, double p_dData);
	// [04/12/07] Double Data List를 조회하는 Interface를 제공한다.
	virtual CList< double, double>	*	GetDataList( void);
};

#endif // !defined(AFX_IWPACKET050116_H__6CDAEE54_DD2F_4200_9DB8_23D3C60875B6__INCLUDED_)
