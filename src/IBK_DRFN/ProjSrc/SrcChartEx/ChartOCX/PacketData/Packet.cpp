// PacketBase.cpp: implementation of the CPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PacketList.h"
#include "PacketBase.h"

#include "../Include_Chart/DLL_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_Chart/Conversion.h"
#include "PacketImplementation.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPacket::CPacket(CPacketList* pParentList, CString strName,
				 CString strFieldName, CString strType,
				 CString szExpressionType, int nDataLength, 
				 bool bUseReal, int nStartPeriod, 
				 CString strRealFieldName)
{
	m_pPacketImplementation = new CPacketImplementation(this, pParentList, strName,
								strFieldName, strType, szExpressionType, nDataLength,
								bUseReal, nStartPeriod, strRealFieldName);
}

CPacket::~CPacket()
{
	if(m_pPacketImplementation)
		delete m_pPacketImplementation;
}

CString CPacket::GetName()
{
	return m_pPacketImplementation->GetName();
}

//<< 20100324_JS.Kim 솔로몬.FX	시고저종 외의 패킷을 시고저종처럼 사용하기 위한 처리
CString CPacket::GetSubName()
{
	return m_pPacketImplementation->GetSubName();
}

void CPacket::SetSubName(CString strSubName)
{
	m_pPacketImplementation->SetSubName(strSubName);
}
//>>

CString CPacket::GetFieldName()
{
	return m_pPacketImplementation->GetFieldName();
}

void CPacket::SetRealFieldName(CString strRealFieldName, CString strRealKeyName)
{
	m_pPacketImplementation->SetRealFieldName(strRealFieldName, strRealKeyName);
}

int CPacket::GetCount() const
{
	return m_pPacketImplementation->GetCount();
}

int CPacket::GetDataCount()
{
	return m_pPacketImplementation->GetDataCount();
}

int CPacket::GetMaxCount()
{
	return m_pPacketImplementation->GetMaxCount();
}

void CPacket::AppendData(double nData, int nIndex)
{
	m_pPacketImplementation->AppendData(nData, nIndex);
}

void CPacket::AppendData(const CString& strData)
{
	m_pPacketImplementation->AppendData(strData);
}

void CPacket::AppendData(long pData, int nDataLength)
{
	m_pPacketImplementation->AppendData(pData, nDataLength);
}

void CPacket::AppendData(CList<double, double>& lsnewData)
{
	m_pPacketImplementation->AppendData(lsnewData);
}

void CPacket::AppendData(CList<CString, CString>& lsnewData)
{
	m_pPacketImplementation->AppendData(lsnewData);
}

void CPacket::InsertData(CString strData)
{
	m_pPacketImplementation->InsertData(strData);
}

void CPacket::InsertData(long pData, int nDataLength)
{
	m_pPacketImplementation->InsertData(pData, nDataLength);
}

bool CPacket::IsTRData() const
{
	return m_pPacketImplementation->IsTRData();
}

int CPacket::GetLength()
{
	return m_pPacketImplementation->GetLength();
}

void CPacket::ChangeData(CList<double, double>& lsnewData)
{
	m_pPacketImplementation->ChangeData(lsnewData);
}

void CPacket::ChangeData(CList<CString, CString>& lsnewData)
{
	m_pPacketImplementation->ChangeData(lsnewData);
}

//	return value
//	-2 : type error
//	-1 : error
//	 0 : nEnd - nStart = Data Count
//	 1 : success
// (2009/1/17 - Seung-Won, Bae) Add without-Zero option.
int CPacket::GetMinMax(int nStart, int nEnd, double& nMin, double& nMax, BOOL p_bWithoutZero)
{
	return m_pPacketImplementation->GetMinMax(nStart, nEnd, nMin, nMax, p_bWithoutZero);
}


//	return value 
//	-2 : type error
//	-1 : error
//	 none zero : success
int CPacket::SetMinMax(CPacketDataInfo::DATAINSERTTYPE nInsertType, CPoint ptDataRange)
{
	return m_pPacketImplementation->SetMinMax(nInsertType, ptDataRange);
}

int CPacket::ClearData(const bool bClearIncludingTR)
{
	return m_pPacketImplementation->ClearData(bClearIncludingTR);
}

void CPacket::Initialize(const bool bClearIncludingTR)
{
	m_pPacketImplementation->Initialize(bClearIncludingTR);
}

const CList<double, double>* CPacket::GetnumericDataList() const
{
	return m_pPacketImplementation->GetnumericDataList();
}

CList<double, double>* CPacket::GetnumericDataList()
{
	return m_pPacketImplementation->GetnumericDataList();
}

CList<CString, CString>* CPacket::GetstrDataList()
{
	return m_pPacketImplementation->GetstrDataList();
}

//bIsFulllType
// true 이면 packettype과 같은 타입으로 scale, title 리턴.
// false 이면 공식에서 scale 표현형으로 추가한 타입리턴.
CString CPacket::GetType(const bool bIsFullType)
{
	return m_pPacketImplementation->GetType(bIsFullType);
}

//*, %등 제거 해서 넘긴다.
//bIsFullType이 true면 packet과 같은 타입 넘김
//false면 표현타입과 같은 넘김
double CPacket::GetTypeFormatDouble(const bool bIsFullType)
{
	return m_pPacketImplementation->GetTypeFormatDouble(bIsFullType);
}

int CPacket::GetStartPeriod()
{
	return m_pPacketImplementation->GetStartPeriod();
}

void CPacket::SetRealDataGap( const double &p_dDataGap)
{
	m_pPacketImplementation->SetRealDataGap( p_dDataGap);
}

bool CPacket::GetRealDataGap(double& dDataGap) const
{
	return m_pPacketImplementation->GetRealDataGap(dDataGap);
}

void CPacket::SetLength(int nLength)
{
	m_pPacketImplementation->SetLength(nLength);
}

void CPacket::SetType(const CString& strType)
{
	m_pPacketImplementation->SetType(strType);
}

void CPacket::SetType(const CString& strFullType, const CString& strExpressionType)
{
	m_pPacketImplementation->SetType(strFullType, strExpressionType);
}

void CPacket::SetName(CString strName)
{
	m_pPacketImplementation->SetName(strName);
}

void CPacket::SetStartPeriod(int nStartPeriod)
{
	m_pPacketImplementation->SetStartPeriod(nStartPeriod);
}

bool CPacket::IsUseStandardValue( const int p_nType) const
{
	return m_pPacketImplementation->IsUseStandardValue( p_nType);
}

double CPacket::GetStandardValue( const int p_nType)
{
	return m_pPacketImplementation->GetStandardValue( p_nType);
}

void CPacket::SetStandardValue( const char *p_szValue)
{
	m_pPacketImplementation->SetStandardValue( p_szValue);
}

bool CPacket::GetUseReal()
{
	return m_pPacketImplementation->GetUseReal();
}

CString CPacket::GetRealFieldName()
{
	return m_pPacketImplementation->GetRealFieldName();
}

CString CPacket::GetRealKeyName()
{
	return m_pPacketImplementation->GetRealKeyName();
}

void CPacket::UpdateData(const CString& strData)
{
	m_pPacketImplementation->UpdateData(strData);
}

void CPacket::UpdateData(const double nData)
{
	m_pPacketImplementation->UpdateData(nData);
}

void CPacket::UpdateData(const double nData, const int nIndex, const bool bIsCurrentData)
{
	m_pPacketImplementation->UpdateData(nData, nIndex, bIsCurrentData);
}

void CPacket::UpdateData(const CString& strData, const int nIndex, const bool bIsCurrentData)
{
	m_pPacketImplementation->UpdateData(strData, nIndex, bIsCurrentData);
}

//void CPacket::UpdateData(CList<double, double>& lsnewData, CPoint ptUpdateRange)
//{
//	m_pPacketImplementation->UpdateData(lsnewData, ptUpdateRange);
//}

// 03.22.2002  문자형
void CPacket::UpdateData(CList<CString, CString>& lsnewData, CPoint ptUpdateRange)
{
	m_pPacketImplementation->UpdateData(lsnewData, ptUpdateRange);
}

int CPacket::GetDateUnitType()
{
	return m_pPacketImplementation->GetDateUnitType();
}

int CPacket::GetTimeInterval()
{
	return m_pPacketImplementation->GetTimeInterval();
}

bool CPacket::GetData(const int nIndex, double& dData) const
{
	return m_pPacketImplementation->GetData(nIndex, dData);
}

double CPacket::GetData(int nIndex)
{
	return m_pPacketImplementation->GetData(nIndex);
}

CString CPacket::GetStrData(int nIndex)
{
	return m_pPacketImplementation->GetStrData(nIndex);
}
// 2010.01.06 by SYS >> 분차트 YYYY(년) 추가 <<
//int CPacket::GetDataPastandFuture(int nIndex)
double CPacket::GetDataPastandFuture(int nIndex)
{
	return m_pPacketImplementation->GetDataPastandFuture(nIndex);
}

bool CPacket::GetDataPastandFuture(const int nIndex, double& dData)
{
	return m_pPacketImplementation->GetDataPastandFuture(nIndex, dData);
}

int CPacket::GetIndexFromData(double nData)
{
	return m_pPacketImplementation->GetIndexFromData(nData);
}

// 분차트 근접시간 적용 - ojtaso (20080811)
int CPacket::GetIndexFromData(double nData, CString strType)
{
	return m_pPacketImplementation->GetIndexFromData(nData, strType);
}

//sy 2004.06.18.
// return : true -> 범위 안인 경우
//			false -> 범위를 벗어나거나 같은 값이 없는 경우
bool CPacket::GetCurOrPrevIndexFromData( const double dData, int& nCurOrPrevIndex, double& dCurOrPrevIndexData) const
{
	return m_pPacketImplementation->GetCurOrPrevIndexFromData( dData, nCurOrPrevIndex, dCurOrPrevIndexData);
}

void CPacket::SetBoundLimit(double nUpperBound, double nLowerBound)
{
	m_pPacketImplementation->SetBoundLimit(nUpperBound, nLowerBound);
}

void CPacket::SetBaseData(const CString &strType, 
						  const CString &strExpressionType,
						  const int nStartPeriod, 
						  const bool bIsAutoMinMax)
{
	m_pPacketImplementation->SetBaseData(strType, strExpressionType, nStartPeriod, bIsAutoMinMax);
}

bool CPacket::RemoveData(int nStart, int nEnd)
{
	return m_pPacketImplementation->RemoveData(nStart, nEnd);
}

bool CPacket::RemoveBaseData( int p_nCount)
{
	return ( p_nCount == m_pPacketImplementation->RemoveBaseData( p_nCount));
}

CPacket* CPacket::GetDatePacket()
{
	return m_pPacketImplementation->GetDatePacket();
}

bool CPacket::SetMinMax(const double nMin, const double nMax)
{
	return m_pPacketImplementation->SetMinMax(nMin, nMax);
}

bool CPacket::RestoreMinMax()
{
	return m_pPacketImplementation->RestoreMinMax();
}

bool CPacket::SetMin(const double nMin)
{
	return m_pPacketImplementation->SetMin(nMin);
}

bool CPacket::SetMax(const double nMax)
{
	return m_pPacketImplementation->SetMax(nMax);
}

bool CPacket::RestoreMin()
{
	return m_pPacketImplementation->RestoreMin();
}

bool CPacket::RestoreMax()
{
	return m_pPacketImplementation->RestoreMax();
}

CPacketList* CPacket::GetPacketList()
{	
	return m_pPacketImplementation->GetPacketList();
}

//삭제한 packet의 count return
int CPacket::RemoveFictionData(const double dReallastDate, const bool bDate)
{
	return m_pPacketImplementation->RemoveFictionData(dReallastDate, bDate);
}

// 장구분
CPacketType::COMMODITY_BOND_NAME CPacket::GetCommodityBondName() const
{
	return m_pPacketImplementation->GetCommodityBondName();
}

void CPacket::SetCommodityBondName(const CPacketType::COMMODITY_BOND_NAME eCommodityBondName)
{
	m_pPacketImplementation->SetCommodityBondName(eCommodityBondName);
}

void CPacket::SetYesOrNoBaseData(const bool bBaseData)
{
	m_pPacketImplementation->SetYesOrNoBaseData(bBaseData);
}

//sy 2005.11.08.
bool CPacket::IsAcceptFromServer() const
{
	return m_pPacketImplementation->IsAcceptFromServer();
}

void CPacket::SetAcceptFromServer(const bool bAcceptFromServer)
{
	m_pPacketImplementation->SetAcceptFromServer(bAcceptFromServer);
}
//sy end

// data 기본이 "0" 인 packet
bool CPacket::IsZeroBasePacket() const
{
	return m_pPacketImplementation->IsZeroBasePacket();
}

bool CPacket::IsChangeRealData() const
{
	return m_pPacketImplementation->IsChangeRealData();
}

/*
double CPacket::GetMin() const 
{
	return m_pPacketImplementation->GetMin();
}

double CPacket::GetMax() const
{
	return m_pPacketImplementation->GetMax();
}
*/

void CPacket::AddTailTempList(const double& dData)
{
	m_pPacketImplementation->AddTailTempList(dData);
}

void CPacket::RemoveAllTempList()
{
	m_pPacketImplementation->RemoveAllTempList();
}

bool CPacket::SetTempListData(const int nIndex, const double& dData, const bool bIsAutoTail)
{
	return m_pPacketImplementation->SetTempListData(nIndex, dData, bIsAutoTail);
}

bool CPacket::GetTempListData(const int nIndex, double& dData) const
{
	return m_pPacketImplementation->GetTempListData(nIndex, dData);
}

bool CPacket::GetHeadTempListData(double& dData) const
{
	return m_pPacketImplementation->GetHeadTempListData(dData);
}

bool CPacket::GetTailTempListData(double& dData) const
{
	return m_pPacketImplementation->GetTailTempListData(dData);
}

CList<double, double>* CPacket::GetTempList() const
{
	return m_pPacketImplementation->GetTempList();
}


/////CBracket Class////////////////////////////////////////////////////////////////////////////////
CBracket::CBracket( CString szItem, int nIndex, int nDate)
{
	m_szItem	= szItem;
	m_nIndex	= nIndex;
	m_nDate		= nDate;
}

CBracket::~CBracket()
{
}

/////CMPItem Class////////////////////////////////////////////////////////////////////////////////
CMPItem::CMPItem()
{
	m_dwVolume = 0;
}

CMPItem::~CMPItem()
{
	POSITION pos = m_listBracket.GetHeadPosition();

	while(pos) {
		delete m_listBracket.GetNext(pos);
	}
	m_listBracket.RemoveAll();
}

// (2004.01.19, 배승원, 분석) 새로운 CBracket Object를 생성 및 Data를 설정한다.
void CMPItem::AddBracket( CString& strAlpa, int nIndex, int nDate)
{
	m_listBracket.AddTail( new CBracket( strAlpa, nIndex, nDate));
}

/////CMProfile Class/////////////////////////////////////////////////////////////////////////
CMProfile::CMProfile():
	m_nDate(0)
{
}

CMProfile::~CMProfile()
{
	POSITION pos = m_listMPItem.GetHeadPosition();

	while(pos) {
		delete m_listMPItem.GetNext(pos);
	}
	m_listMPItem.RemoveAll();
}

CMPItem* CMProfile::GetMPItem(const int nIndex)
{
	int nCount = m_listMPItem.GetCount();
	if(nIndex >= nCount || nIndex < 0)
		return NULL;

	return m_listMPItem.GetAt(m_listMPItem.FindIndex(nIndex));
}

int CMProfile::AddMPItem(CMPItem *item)
{
	m_listMPItem.AddTail(item);

	return m_listMPItem.GetCount()-1;
}

CMPItem* CMProfile::AddTail()
{
	CMPItem *mp = new CMPItem();

	m_listMPItem.AddTail(mp);

	return mp;
}

CMPItem* CMProfile::AddHead()
{
	CMPItem *mp = new CMPItem();

	m_listMPItem.AddHead(mp);

	return mp;
}

int CMProfile::GetCount() const
{
	return m_listMPItem.GetCount();
}

// (2004.01.19, 배승원) 단위 MP Chart의 가격 값의 크기를 구한다. (구간 대표값의 범위이다.)
//		(CMPItem는 가격에 따라 Sorted되었음을 감안한다.)
BOOL CMProfile::GetPriceMinMaxX100( int &nMinX100, int &nMaxX100)
{
	// 1. 먼저 CMPItem가 하나도 등록되지 않은 경우를 제외한다.
	if( m_listMPItem.IsEmpty()) return FALSE;

	// 2. Min 값을 구한다.
	nMinX100 = m_listMPItem.GetHead()->GetPriceX100();

	// 3. Max 값을 구한다.
	nMaxX100 = m_listMPItem.GetTail()->GetPriceX100();

	return TRUE;
}

// (2004.01.26, 배승원) Max Volume과 Max Bracket Count인 CMPItem이 1개 이상일 경우를 감안하여
//		Max인 Index값을 제공하지 않고, 그 값을 제공토록한다.
int CMProfile::GetMaxBracketCount()
{
	int maxcount = 0;
	POSITION pos = m_listMPItem.GetTailPosition();
	while( pos != NULL)
	{
		CList<CBracket*, CBracket*>* pBracketList = m_listMPItem.GetPrev(pos)->GetBracketList();
		if(pBracketList == NULL || pBracketList->GetCount() < 0) return 0;
		if( pBracketList->GetCount() > maxcount) maxcount = pBracketList->GetCount();
	}
	return maxcount;
}

// (2004.01.26, 배승원) Max Volume과 Max Bracket Count인 CMPItem이 1개 이상일 경우를 감안하여
//		Max인 Index값을 제공하지 않고, 그 값을 제공토록한다.
DWORD CMProfile::GetMaxVolume()
{
	DWORD dwVolume = 0;
	POSITION pos = m_listMPItem.GetTailPosition();
	while( pos)
	{
		CMPItem *pMPItem = m_listMPItem.GetPrev( pos);
		if( pMPItem->GetVolume() > dwVolume) dwVolume = pMPItem->GetVolume();
	}
	return dwVolume;
}

//MProfile의 모든 Item의 Bracket의 수를 리턴한다..
int CMProfile::GetTotalBracketCount()
{
	int maxcount = 0;
	POSITION pos = m_listMPItem.GetHeadPosition();
	while(pos != NULL)
	{
		CList<CBracket*, CBracket*>* pBracketList = m_listMPItem.GetNext(pos)->GetBracketList();
		if(pBracketList == NULL || pBracketList->GetCount() < 0)
			return 0;
		maxcount += pBracketList->GetCount();
	}
	return maxcount;
}

CMPItem* CMProfile::InsertBefore(int nIndex)
{
	int nCount = m_listMPItem.GetCount();
	if(nIndex < 0 || nIndex	>= nCount)
		return NULL;

	CMPItem *item = new CMPItem();
	m_listMPItem.InsertBefore(m_listMPItem.FindIndex(nIndex), item);
	return item;
}

CMPItem* CMProfile::InsertAfter(int nIndex)
{
	int nCount = m_listMPItem.GetCount();
	if(nIndex < 0 || nIndex	>= nCount)
		return NULL;

	CMPItem *item = new CMPItem();
	m_listMPItem.InsertAfter(m_listMPItem.FindIndex(nIndex), item);
	return item;
}
/////CMPPacket Class//////////////////////////////////////////////////////////////////////////////
CMPPacket::CMPPacket(CPacketList* pParentList, CString szName, CString szFieldName,
					 CString szType, CString szExpType, int nLength, bool bUseReal,
					 int nStartPeriod, CString szRealFieldName)
					 : CPacket(pParentList, szName, szFieldName, szType, szExpType,
							nLength, bUseReal, nStartPeriod, szRealFieldName)
{
	// (2004.01.19, 배승원) Drawing 시에 참조할 수 있도록 가격 폭을 관리한다.
	m_nPriceUnitX100 = 0;
}

CMPPacket::~CMPPacket()
{
	RemoveAll();
}

void CMPPacket::RemoveAll()
{
	POSITION pos = m_listMarketProfile.GetHeadPosition();

	while(pos) {
		delete m_listMarketProfile.GetNext(pos);
	}
	m_listMarketProfile.RemoveAll();	
}

CMProfile* CMPPacket::GetMarketProfile(const int nIndex)
{
	int nCount = m_listMarketProfile.GetCount();
	if(nIndex >= nCount && nIndex < 0)
		return NULL;

	return m_listMarketProfile.GetAt(m_listMarketProfile.FindIndex(nIndex));
}

int CMPPacket::AddMarketProfile(CMProfile *mp)
{
	m_listMarketProfile.AddTail(mp);

	return m_listMarketProfile.GetCount()-1;
}

CMProfile* CMPPacket::AddTail()
{
	CMProfile *mp = new CMProfile();

	m_listMarketProfile.AddTail(mp);

	return mp;
}

int CMPPacket::GetCount() const
{
	return m_listMarketProfile.GetCount();
}

int	CMPPacket::ClearData(const bool bClearIncludingTR)
{
	// (2004.01.19, 배승원) Drawing 시에 참조할 수 있도록 가격 폭을 관리한다.
	m_nPriceUnitX100 = 0;

	int nResult = CPacket::ClearData(bClearIncludingTR);
	if(m_listMarketProfile.GetCount() <= 0)
		return nResult;

	RemoveAll();
	return 1;
}

// (2004.01.19, 배승원) Drawing 시에 Y Scale인 가격축에 점유하는 영역을 제공한다.
BOOL CMPPacket::GetPriceScaleX100( int &nMinX100, int &nMaxX100)
{
	// 1. 먼저 Return 값을 초기화한다.
	nMinX100 = INT_MAX;
	nMaxX100 = INT_MIN;

	BOOL bUpdated = FALSE;

	POSITION pos = m_listMarketProfile.GetHeadPosition();
	while(pos != NULL)
	{
		CMProfile *pMProfile = m_listMarketProfile.GetNext(pos);

		int nMPMinX100, nMPMaxX100;
		if( !pMProfile->GetPriceMinMaxX100( nMPMinX100, nMPMaxX100)) continue;

		if( nMPMinX100 < nMinX100) nMinX100 = nMPMinX100;
		if( nMaxX100 < nMPMaxX100) nMaxX100 = nMPMaxX100;

		bUpdated = TRUE;
	}

	if( bUpdated) nMaxX100 += m_nPriceUnitX100;

	return bUpdated;
}

// (2004.01.26, 배승원) MProfile의 GetMaxVolume()을 이용하여 처리한다.
DWORD CMPPacket::GetMaxVolume( void)
{
	DWORD dwMaxVolume = 0, dwVolume;

	POSITION pos = m_listMarketProfile.GetHeadPosition();
	while(pos != NULL)
	{
		CMProfile* pMProfile = m_listMarketProfile.GetNext(pos);
		dwVolume = pMProfile->GetMaxVolume();
		if( dwVolume > dwMaxVolume) dwMaxVolume = dwVolume;
	}
	return dwMaxVolume;
}

int CMPPacket::GetMaxHitting()
{
	if(&m_listMarketProfile == NULL)
		return -1;
	POSITION pos = m_listMarketProfile.GetHeadPosition();

	int MaxHit = 0;
	while(pos != NULL)
	{
		CMProfile* pMProfile = m_listMarketProfile.GetNext(pos);
		CList<CMPItem*, CMPItem*>* pMPItemList = pMProfile->GetMPItemList();
		POSITION itempos = pMPItemList->GetHeadPosition();
		while(itempos != NULL)
		{
			CMPItem* pMPItem = pMPItemList->GetNext(itempos);
			CList<CBracket*, CBracket*>* pBracketList = pMPItem->GetBracketList();
			if(pBracketList->GetCount() > MaxHit)
				MaxHit = (int)pBracketList->GetCount();
		}
	}
	return MaxHit;
}

int CMPPacket::GetTotalMPItemCount()
{
	int count = 0;
	if(&m_listMarketProfile == NULL)
		return -1;
	POSITION pos = m_listMarketProfile.GetHeadPosition();
	while(pos != NULL)
	{
		CMProfile* pMProfile = m_listMarketProfile.GetNext(pos);
		count += pMProfile->GetCount();
	}
	return count;
}

int CMPPacket::GetMaxMPItemCount()
{
	int count = 0;
	if(&m_listMarketProfile == NULL)
		return -1;
	POSITION pos = m_listMarketProfile.GetHeadPosition();
	while(pos != NULL)
	{
		CMProfile* pMProfile = m_listMarketProfile.GetNext(pos);
		if( pMProfile->GetCount() > count)
			count = pMProfile->GetCount();
	}
	return count;
}

// (2004.01.20, 배승원) Drawing시에 X Scale인 Bracket 수를 조회하는 Interface를 제공한다.
int CMPPacket::GetMaxBracketCount( void) const
{
	int nMaxBracketCount = INT_MIN;
	POSITION posMProfile = m_listMarketProfile.GetHeadPosition();
	while( posMProfile)
	{
		int nCount = m_listMarketProfile.GetNext( posMProfile)->GetMaxBracketCount();
		if( nMaxBracketCount < nCount) nMaxBracketCount = nCount;
	}
	
	return nMaxBracketCount;
}

// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
void CPacket::AppendTrData( const char *p_szData)
{
	m_pPacketImplementation->AppendTrData( p_szData);
}
void CPacket::AppendTrData( const double dData)
{
	m_pPacketImplementation->AppendTrData( dData);
}
void CPacket::InsertTrData( const char *p_szData)
{
	m_pPacketImplementation->InsertTrData( p_szData);
}
void CPacket::InsertTrData( const double dData)
{
	m_pPacketImplementation->InsertTrData( dData);
}

// (2007/2/7 - Seung-Won, Bae) for Exception Value
void CPacket::SetExceptionValue( const double &p_dExceptionValue)
{
	m_pPacketImplementation->SetExceptionValue( p_dExceptionValue);
}
double CPacket::GetExceptionValue( void)
{
	return m_pPacketImplementation->GetExceptionValue();
}

// (2007/2/22 - Seung-Won, Bae) for Continue Code
void CPacket::SetContinueCode( const double &p_dContinueCode)
{
	m_pPacketImplementation->SetContinueCode( p_dContinueCode);
}
double CPacket::GetContinueCode( void)
{
	return m_pPacketImplementation->GetContinueCode();
}


// (2007/2/21 - Seung-Won, Bae) Support Not-Auto Scale Type.
void CPacket::SetAutoScaleType( BOOL p_bAutoScaleType)
{
	m_pPacketImplementation->SetAutoScaleType( p_bAutoScaleType);
}
BOOL CPacket::IsAutoScaleType( void)
{
	return m_pPacketImplementation->IsAutoScaleType();
}
void CPacket::SetScaleType( const CString &p_strScaleType)
{
	m_pPacketImplementation->SetScaleType( p_strScaleType);
}
CString CPacket::GetScaleType( void)
{
	return m_pPacketImplementation->GetScaleType();
}
double CPacket::GetScaleTypeFormatDouble( void)
{
	return m_pPacketImplementation->GetScaleTypeFormatDouble();
}

// 20080908 김진순	8진법/32진법 표시 적용
void CPacket::SetBaseNumber(int nBase)
{
	m_pPacketImplementation->SetBaseNumber(nBase);
}

int	CPacket::GetBaseNumber()
{
	return m_pPacketImplementation->GetBaseNumber();
}

void CPacket::SetDecLen(int nDecLen, int nDecLen2)
{
	m_pPacketImplementation->SetDecLen(nDecLen, nDecLen2);
}

int	CPacket::GetDecLen(int &nDecLen, int &nDecLen2)
{
	return m_pPacketImplementation->GetDecLen(nDecLen, nDecLen2);
}

// (2009/1/17 - Seung-Won, Bae) Support Min/Max cache. Do not make Min/Max every time of updating data.
//		Do not support Full Range Min/Max for collision with Pre-Min/Max.
void CPacket::ClearMinMax( void)
{
	m_pPacketImplementation->ClearMinMax();
}

// (2009/5/21 - Seung-Won, Bae) Shift the data of packet to future.
BOOL CPacket::ShiftData( long p_szShiftCount)
{
	return m_pPacketImplementation->ShiftData( p_szShiftCount);
}

// 현재가 등락률 - ojtaso (20080602)
void CPacket::OffsetPacketData(double dData)
{
	m_pPacketImplementation->OffsetPacketData(dData);
}

// 20081007 JS.Kim	사용자시간대 선택
// Packet의 모든 데이타의 시간대를 계산하여 Reset
void CPacket::CalcPacketTimeDiff(int nTimeDiff)
{
	m_pPacketImplementation->CalcPacketTimeDiff(nTimeDiff);
}

//	double	CalcTimeDiff(double dData);

double CPacket::CalcTimeDiff(double dData, int nTimeDiff)
{
	return m_pPacketImplementation->CalcTimeDiff(dData, nTimeDiff);
}

//	double	CalcBackTimeDiff(double dData);		// 원래 시간대로 되돌리는 함수
