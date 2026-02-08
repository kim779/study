// IWPacket.cpp: implementation of the CIWPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWPacket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// _IWRAPPER_IUNKNOWN_MANAGER_IMP
//////////////////////////////////////////////////////////////////////
_IWRAPPER_IUNKNOWN_MANAGER_IMP( CIWPacket)

//////////////////////////////////////////////////////////////////////
// [04/10/11] Packet의 정보를 조회하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// [04/10/26] Packet의 Type을 Unit Value(Double)로 조회하는 Interface를 제공한다.
double CIWPacket::GetPacketTypeUnitValue( void)
{
	if( !m_pPacket) return 0;
	return m_pPacket->GetTypeFormatDouble();
}

// [04/11/30] Packet Name을 구하는 Interface를 제공한다.
BOOL CIWPacket::GetPacketName( CString &p_strPacketName)
{
	if( !m_pPacket) return FALSE;
	p_strPacketName = m_pPacket->GetName();
	return TRUE;
}

// [04/10/11] 주어지는 값의 Data Index, 또는 그 Data 값이 있어야할 위치 이전의 Index를 구한다. (Sort 전제)
//			TRUE	-> 범위 안인 경우
//			FALSE	-> 범위를 벗어나거나 같은 값이 없는 경우
BOOL CIWPacket::GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData)
{
	if( m_pPacket) return m_pPacket->GetCurOrPrevIndexFromData( dData, nCurOrPrevIndex, dCurOrPrevIndexData);
	return FALSE;
}

// [04/10/15] Data의 수를 확인하는 Interface를 제공한다.
int CIWPacket::GetDataCount( void)
{
	if( m_pPacket) return m_pPacket->GetCount();
	return 0;
}

// [04/10/11] 해당 Index의 Data를 조회하는 Interface를 제공한다.
double CIWPacket::GetData( int p_nIndex)
{
	if( m_pPacket) return m_pPacket->GetData( p_nIndex);
	return ERRORVALUE;
}
double CIWPacket::GetDataPastandFuture( int p_nIndex)
{
	if( m_pPacket) return m_pPacket->GetDataPastandFuture( p_nIndex);
	return ERRORVALUE;
}
BOOL CIWPacket::GetData(const int nIndex, CString &p_strData)
{
	if( !m_pPacket) return FALSE;
	p_strData = m_pPacket->GetStrData( nIndex);
	return TRUE;
}

// [04/10/15] 해당 Index의 Data를 설정하는 Interface를 제공한다. (Zero Base)
BOOL CIWPacket::SetData( int p_nIndex, double p_dData)
{
	if( !m_pPacket) return FALSE;
	m_pPacket->UpdateData( p_dData, p_nIndex);
	return TRUE;
}

// [04/12/07] Double Data List를 조회하는 Interface를 제공한다.
CList< double, double> *CIWPacket::GetDataList( void)
{
	if( !m_pPacket) return NULL;
	return m_pPacket->GetnumericDataList();
}

// (2005/9/2 - Seung-Won, Bae) Get String Data List
CList< CString, CString> *CIWPacket::GetStrDataList( void)
{
	if( !m_pPacket) return NULL;
	return m_pPacket->GetstrDataList();
}


/*-----------------------------------------------------------------------------------------
 - Function    :  AppendData
 - Created at  :  2005-07-26   19:56
 - Author      :  최종찬
 - Description :  Data를 추가하는 Interface를 제공한다
 -----------------------------------------------------------------------------------------*/
void CIWPacket::AppendData(double dData)
{
	if(!m_pPacket)
		return;

	m_pPacket->AppendData(dData);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  ClearData
 - Created at  :  2005-07-29   10:04
 - Author      :  최종찬
 - Description :  Packet의 데이타를 Clear한다. 
 -----------------------------------------------------------------------------------------*/
void CIWPacket::ClearData(const bool bBaseDataClear /* = false */)
{
	if(!m_pPacket)
		return;
	m_pPacket->ClearData(bBaseDataClear);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  SetType
 - Created at  :  2005-08-29   11:00
 - Author      :  최종찬
 - Description :  패킷타입을 설정한다.
					strType : 속성 page에서 셋팅한 type, -> 수치조회창, data 전체보기에서 사용
					strExperssionType : 일반적인 type -> 화면 영역을 구할때, scale 표현할때 사용
 -----------------------------------------------------------------------------------------*/
void CIWPacket::SetType(const CString& strType, const CString& strExperssionType)
{
	if(!m_pPacket)
		return;
	m_pPacket->SetType(strType, strExperssionType);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  GetType
 - Created at  :  2005-08-29   13:50
 - Author      :  최종찬
 - Description :  패킷타입을 가져온다.
 -----------------------------------------------------------------------------------------*/
CString CIWPacket::GetType(const bool bIsFullType /* = true */)
{
	if(!m_pPacket)
		return _T("");
	return m_pPacket->GetType(bIsFullType);
}

//////////////////////////////////////////////////////////////////////
// (2006/6/15 - Seung-Won, Bae) Get Index and Data
//////////////////////////////////////////////////////////////////////

int CIWPacket::GetIndexFromData( double p_dData)
{
	if( !m_pPacket) return -1;
	return m_pPacket->GetIndexFromData( p_dData);
}

BOOL CIWPacket::GetDataPastandFuture( int p_nIndex, double &p_dData)
{
	if( !m_pPacket) return FALSE;
	return m_pPacket->GetDataPastandFuture( p_nIndex, p_dData);
}
