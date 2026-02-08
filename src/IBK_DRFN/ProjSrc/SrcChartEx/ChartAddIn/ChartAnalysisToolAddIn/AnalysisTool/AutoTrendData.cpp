// AutoTrendData.cpp: implementation of the CAutoTrendData class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoTrendData.h"

#include "../Include_Chart/Conversion.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_Addin_133101/I133101/_IStringList.h"	// for IStringList
#include "../Include_Addin_133101/I133101/_IDoubleList.h"	// for IDoubleList

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CAutoTrendPoint

CAutoTrendPoint& CAutoTrendPoint::operator=(const CAutoTrendPoint& data)
{
	if(this == &data)
		return *this;

	m_nIndex = data.GetIndex();
	m_dPrice = data.GetPrice();
	return *this;
}

CAutoTrendPoint::CAutoTrendPoint():
	m_nIndex(-1),
	m_dPrice(0.0)
{
}

CAutoTrendPoint::CAutoTrendPoint(const int nIndex, const double& dPrice):
	m_nIndex(nIndex),
	m_dPrice(dPrice)
{
}


// public =====================================================================
void CAutoTrendPoint::SetIndex(const int nIndex)
{
	m_nIndex = nIndex;
}

void CAutoTrendPoint::SetPrice(const double& dPrice)
{
	m_dPrice = dPrice;
}

int CAutoTrendPoint::GetIndex() const
{
	return m_nIndex;
}

double CAutoTrendPoint::GetPrice() const
{
	return m_dPrice;
}


///////////////////////////////////////////////////////////////////////////////
// class CAutoTrendDrawingCalculatorData

CAutoTrendDrawingCalculatorData& CAutoTrendDrawingCalculatorData::operator=(const CAutoTrendDrawingCalculatorData& data)
{
	if(this == &data)
		return *this;

	m_noFrameGraphRegion = data.GetNoFrameGraphRegion();
	const CList<IPacket*, IPacket*>& packetList = data.GetPacketList();
	POSITION pos = packetList.GetHeadPosition();
	while(pos != NULL)
	{
		IPacket *pIPacket = packetList.GetNext( pos);
		if( pIPacket) pIPacket->AddRef();
		m_packetList.AddTail( pIPacket);
	}

	m_calData = data.GetCoordinateCalculatorData();
	return *this;
}

CAutoTrendDrawingCalculatorData::CAutoTrendDrawingCalculatorData( IChartManager *p_pIChartManager):
	m_noFrameGraphRegion(0, 0, 0, 0),
	m_calData( p_pIChartManager)
{
}

// public =====================================================================
void CAutoTrendDrawingCalculatorData::Initialize( IChartManager *p_pIChartManager)
{
	m_noFrameGraphRegion = CRect(0, 0, 0, 0);
	m_calData = CCoordinateCalculatorData( p_pIChartManager);
	IPacket *pIPacket = NULL;
	while( !m_packetList.IsEmpty())
	{
		pIPacket = m_packetList.RemoveHead();
		if( pIPacket) pIPacket->Release();
	}
}

// ----------------------------------------------------------------------------
void CAutoTrendDrawingCalculatorData::SetNoFrameGraphRegion(const CRect& region)
{
	m_noFrameGraphRegion = region;
}

void CAutoTrendDrawingCalculatorData::SetPacketList(IPacketManager* pPacketList, const CString& strPacketNames)
{
	IPacket *pIPacket = NULL;
	while( !m_packetList.IsEmpty())
	{
		pIPacket = m_packetList.RemoveHead();
		if( pIPacket) pIPacket->Release();
	}

	if(pPacketList == NULL || strPacketNames.IsEmpty())
		return;

	CString strRealPacketNames = strPacketNames;
	if(strRealPacketNames.Right(1) != ',')
		strRealPacketNames += ',';

	int nCount = CDataConversion::GetStringDataCount(strRealPacketNames, ",");
	for(int nIndex = 0; nIndex < nCount; nIndex++)
	{
		CString packetName = CDataConversion::GetMidToTrimDataIndex(strRealPacketNames, ",", nIndex);
		IPacket* pPacket = pPacketList->GetPacket(packetName);
		if(pPacket == NULL)
		{
			while( !m_packetList.IsEmpty())
			{
				pIPacket = m_packetList.RemoveHead();
				if( pIPacket) pIPacket->Release();
			}
			return;
		}
		m_packetList.AddTail(pPacket);
	}
}

void CAutoTrendDrawingCalculatorData::SetCoordinateCalculatorData(const CCoordinateCalculatorData& calData)
{
	m_calData = calData;
}

// ----------------------------------------------------------------------------
CRect CAutoTrendDrawingCalculatorData::GetNoFrameGraphRegion() const
{
	return m_noFrameGraphRegion;
}

const CList<IPacket*, IPacket*>& CAutoTrendDrawingCalculatorData::GetPacketList() const
{
	return m_packetList;
}

CList<IPacket*, IPacket*>& CAutoTrendDrawingCalculatorData::GetPacketList()
{
	return m_packetList;
}

IPacket* CAutoTrendDrawingCalculatorData::GetPacket(const int nIndex) const
{
	POSITION pos = m_packetList.FindIndex(nIndex);
	if(pos == NULL) return NULL;

	IPacket *pIPacket = m_packetList.GetAt( pos);
	if( pIPacket) pIPacket->AddRef();
	return pIPacket;
}
	
CCoordinateCalculatorData CAutoTrendDrawingCalculatorData::GetCoordinateCalculatorData() const
{
	return m_calData;
}

CCoordinateCalculatorData& CAutoTrendDrawingCalculatorData::GetCoordinateCalculatorData()
{
	return m_calData;
}

// ----------------------------------------------------------------------------
bool CAutoTrendDrawingCalculatorData::DoesCalculator(const CPoint& point) const
{
	if(m_packetList.GetCount() <= 0)
		return false;

	return m_calData.DoesCalculator(point);
}

// ----------------------------------------------------------------------------
double CAutoTrendDrawingCalculatorData::ApplyLog(const double& dData) const
{
	return m_calData.ApplyLog(dData);
}

double CAutoTrendDrawingCalculatorData::ApplyExp(const double& dData) const
{
	return m_calData.ApplyExp(dData);
}


///////////////////////////////////////////////////////////////////////////////
// class CAutoTrendDrawingCalculator

// public =====================================================================
bool CAutoTrendDrawingCalculator::GetDrawingPoints(const CAutoTrendDrawingCalculatorData& calData, const int nCurrentDataIndex, 
		CPoint& high_leftPt, CPoint& high_rightPt, CPoint& low_leftPt, CPoint& low_rightPt) const
{
	if(!GetDrawingPoints(calData, nCurrentDataIndex, 1, high_leftPt, high_rightPt))
		return false;

	if(!GetDrawingPoints(calData, nCurrentDataIndex, 2, low_leftPt, low_rightPt))
		return false;

	return true;
}

CString CAutoTrendDrawingCalculator::GetText(IPacket* pPacket, const int nStartIndex, const int nEndIndex) const
{
	if(!DoesExistenceText(pPacket, nStartIndex, nEndIndex))
		return "";

	IStringList *piStrDataList = pPacket->GetStrDataList();
	if( piStrDataList == NULL) return "";
	if( piStrDataList->GetCount() <= 0) return "";

	POSITION pos = piStrDataList->FindIndex(nEndIndex);
	int nIndex = nEndIndex;
	while(pos != NULL && nIndex >= nStartIndex)
	{
		ILPCSTR szData = NULL;
		piStrDataList->GetPrev( pos, szData);
		if( !szData.IsEmpty()) return szData;
		nIndex--;
	}
	piStrDataList->Release();

	return "";
}


// private ====================================================================
// 상대좌표를 구할지의 여부
bool CAutoTrendDrawingCalculator::DoesRelativePoints(IPacket* pIndexPacket, IPacket* pDataPacket) const
{
	if(pIndexPacket == NULL || pDataPacket == NULL)
		return false;
	
	if(pIndexPacket->GetDataList() == NULL || pDataPacket->GetDataList() == NULL)
	{
//		ILPCSTR szPacketName = pIndexPacket->GetPacketNameString();
//		szPacketName = pDataPacket->GetPacketNameString();
		return false;
	}

	if( pIndexPacket->GetDataCount() <= 0)
		return false;
	return (pIndexPacket->GetDataCount() == pDataPacket->GetDataCount());
}

bool CAutoTrendDrawingCalculator::DoesExistenceText(IPacket* pPacket, const int nStartIndex, const int nEndIndex) const
{
	if(nStartIndex > nEndIndex)
		return false;

	return (pPacket != NULL && pPacket->GetDataCount() > 0);
}

// ----------------------------------------------------------------------------
bool CAutoTrendDrawingCalculator::GetDrawingPoints(const CAutoTrendDrawingCalculatorData& calData, 
		const int nCurrentDataIndex, const int nPacketIndex, CPoint& leftPt, CPoint& rightPt) const
{
	CAutoTrendPoint atLeftPt(0, 0.0), atRightPt(0, 0.0);
	IPacket *pIPacket0 = calData.GetPacket( 0);
	IPacket *pIPacket = calData.GetPacket( nPacketIndex);
	BOOL bResult = GetRelativePoints( pIPacket0, pIPacket, nCurrentDataIndex, atLeftPt, atRightPt);
	if( pIPacket0) pIPacket0->Release();
	if( pIPacket) pIPacket->Release();
	if( !bResult) return false;

	return GetAbsolutePoints(calData.GetCoordinateCalculatorData(), atLeftPt, atRightPt, leftPt, rightPt);
}

// 상대좌표를 구하기
bool CAutoTrendDrawingCalculator::GetRelativePoints(IPacket* pIndexPacket, IPacket* pDataPacket, const int nCurrentDataIndex, CAutoTrendPoint& atLeftPt, CAutoTrendPoint& atRightPt) const
{
	if(!DoesRelativePoints(pIndexPacket, pDataPacket))
		return false;

	IDoubleList *piIndexList = pIndexPacket->GetDataList();
	IDoubleList *piDataList = pDataPacket->GetDataList();
	if( !piIndexList || piDataList) return false;

	int nComparisonData = GetComparisonData(pDataPacket);
	if(nComparisonData <= 0)
		return false;

	POSITION indexPos = piIndexList->FindIndex( nCurrentDataIndex);
	POSITION dataPos = piDataList->FindIndex( nCurrentDataIndex);
	bool bRightPt = true;
	for(int nDataIndex = nCurrentDataIndex; nDataIndex >= 0; nDataIndex--)
	{
		if(indexPos == NULL || dataPos == NULL)
			break;

		double dData = 0.0;
		piIndexList->GetPrev( indexPos, dData);

		double dPrice = 0.0;
		piDataList->GetPrev( dataPos, dPrice);

		if((int)dData != nComparisonData)
			continue;

		if(!bRightPt)
		{
			atLeftPt = CAutoTrendPoint(nDataIndex, dPrice);
			piIndexList->Release();
			piDataList->Release();
			return true;
		}
		atRightPt = CAutoTrendPoint(nDataIndex, dPrice);
		bRightPt = false;
	}
	piIndexList->Release();
	piDataList->Release();
	return false;
}

// 절대좌표를 구하기
bool CAutoTrendDrawingCalculator::GetAbsolutePoints(const CCoordinateCalculatorData& calData, const CAutoTrendPoint& atLeftPt, const CAutoTrendPoint& atRightPt, CPoint& leftPt, CPoint& rightPt) const
{
	int nDataCount = 0;
	IPacket *pIPacket = calData.GetPacket_X();
	if( pIPacket)
	{
		nDataCount = pIPacket->GetDataCount();
		pIPacket->Release();
		pIPacket = NULL;
	}
	if( nDataCount <= 0) return false;

	if(atLeftPt.GetIndex() < 0 || atRightPt.GetIndex() < 0)
		return false;

	leftPt = CDataConversion::RelativeToAbsolute(calData.GetRegion(), 
			calData.GetDisplayDataStart(), calData.GetDisplayDataEnd(), 
			calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType(), 
			atLeftPt.GetIndex(), calData.ApplyLog(atLeftPt.GetPrice()));

	rightPt = CDataConversion::RelativeToAbsolute(calData.GetRegion(), 
			calData.GetDisplayDataStart(), calData.GetDisplayDataEnd(), 
			calData.GetDisplayDataMin_ScaleType(), calData.GetDisplayDataMax_ScaleType(), 
			atRightPt.GetIndex(), calData.ApplyLog(atRightPt.GetPrice()));

	return true;
}

// ----------------------------------------------------------------------------
// data 형태를 비교할 값을 가져온다
// 0 : 무시   1 : 저가 포인트    2 : 고가 포인트
int CAutoTrendDrawingCalculator::GetComparisonData(IPacket* pDataPacket) const
{
	if(pDataPacket == NULL)
		return -1;

	ILPCSTR szPacketName = pDataPacket->GetPacketNameString();
	CString strPacketName( szPacketName);
	if( strPacketName == _MTEXT( C0_LOW)) // "저가"
		return 1;
	else if(strPacketName == _MTEXT( C0_HIGH)) // "고가"
		return 2;

	return -1;
}
