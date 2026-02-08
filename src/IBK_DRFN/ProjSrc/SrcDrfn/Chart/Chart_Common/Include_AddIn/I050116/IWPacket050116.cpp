// IWPacket050116.cpp: implementation of the CIWPacket050116 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. 주의!	050116 Version의 AddIn에서 이용되는 OCX측 Interface로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		Interface 순서 변경 조차 허용되지 않습니다.
//		허용되지 않는 편집은 050116 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			배승원
//			ChartPatternAddIn			GZone			정서정
//			ChartGuideLine				GZone			남상윤
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWPacket050116.h"

#include "Packetbase.h"			// for CPacket

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIWPacket050116::CIWPacket050116( CPacket *p_pPacket)
{
	// (2004.10.11, 배승원) Packet Data Object의 Pointer를 관리한다.
	m_pPacket = p_pPacket;
	if( !m_pPacket)
	{
		AfxMessageBox( "CIWPacket050116 Null Wrapping Error!");
		return;
	}
}

CIWPacket050116::~CIWPacket050116()
{

}

//////////////////////////////////////////////////////////////////////
// [04/10/11] Packet의 정보를 조회하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04/10/11] Packet의 Type을 String으로 조회하는 Interface를 제공한다.
BOOL CIWPacket050116::GetPacketTypeString( CString &p_strPacketType)
{
	p_strPacketType.Empty();
	if( !m_pPacket) return FALSE;
	p_strPacketType = m_pPacket->GetType();
	return TRUE;
}

// [04/10/26] Packet의 Type을 Unit Value(Double)로 조회하는 Interface를 제공한다.
double CIWPacket050116::GetPacketTypeUnitValue( void)
{
	if( !m_pPacket) return 0;
	return m_pPacket->GetTypeFormatDouble();
}

// [04/11/30] Packet Name을 구하는 Interface를 제공한다.
BOOL CIWPacket050116::GetPacketName( CString &p_strPacketName)
{
	if( !m_pPacket) return FALSE;
	p_strPacketName = m_pPacket->GetName();
	return TRUE;
}

// [04/10/11] 주어지는 값의 Data Index, 또는 그 Data 값이 있어야할 위치 이전의 Index를 구한다. (Sort 전제)
//			TRUE	-> 범위 안인 경우
//			FALSE	-> 범위를 벗어나거나 같은 값이 없는 경우
BOOL CIWPacket050116::GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData)
{
	if( m_pPacket) return m_pPacket->GetCurOrPrevIndexFromData( dData, nCurOrPrevIndex, dCurOrPrevIndexData);
	return FALSE;
}

// [04/10/15] Data의 수를 확인하는 Interface를 제공한다.
int CIWPacket050116::GetDataCount( void)
{
	if( m_pPacket) return m_pPacket->GetCount();
	return 0;
}

// [04/10/11] 해당 Index의 Data를 조회하는 Interface를 제공한다.
double CIWPacket050116::GetData( int p_nIndex)
{
	if( m_pPacket) return m_pPacket->GetData( p_nIndex);
	return ERRORVALUE;
}

// [04/10/15] 해당 Index의 Data를 설정하는 Interface를 제공한다. (Zero Base)
BOOL CIWPacket050116::SetData( int p_nIndex, double p_dData)
{
	if( !m_pPacket) return FALSE;
	m_pPacket->UpdateData( p_dData, p_nIndex);
	return TRUE;
}

// [04/12/07] Double Data List를 조회하는 Interface를 제공한다.
CList< double, double> *CIWPacket050116::GetDataList( void)
{
	if( !m_pPacket) return NULL;
	return m_pPacket->GetnumericDataList();
}
