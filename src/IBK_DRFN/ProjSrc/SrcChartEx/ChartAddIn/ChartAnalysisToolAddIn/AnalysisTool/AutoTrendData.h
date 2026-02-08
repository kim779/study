// AutoTrendData.h: interface for the CAutoTrendData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOTRENDDATA_H__F96C8DB1_CCA1_4662_9488_EA888E677740__INCLUDED_)
#define AFX_AUTOTRENDDATA_H__F96C8DB1_CCA1_4662_9488_EA888E677740__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../Include_Analysis/CoordinateCalculatorData.h"

class CCoordinate;

// 자동추세의 point
class CAutoTrendPoint
{
public:
	CAutoTrendPoint& operator=(const CAutoTrendPoint& data);

public:
	CAutoTrendPoint();
	CAutoTrendPoint(const int nIndex, const double& dPrice);

	void SetIndex(const int nIndex);
	void SetPrice(const double& dPrice);

	int GetIndex() const;
	double GetPrice() const;

private:
	int m_nIndex;
	double m_dPrice;
};


// 자동추세를 구하기 위해 필요한 data.
class CAutoTrendDrawingCalculatorData
{
public:
	CAutoTrendDrawingCalculatorData& operator=(const CAutoTrendDrawingCalculatorData& data);

public:
	CAutoTrendDrawingCalculatorData( IChartManager *p_pIChartManager);

	void Initialize( IChartManager *p_pIChartManager);

	void SetNoFrameGraphRegion(const CRect& region);
	void SetPacketList(IPacketManager* pPacketList, const CString& strPacketNames);
	void SetCoordinateCalculatorData(const CCoordinateCalculatorData& calData);

	CRect GetNoFrameGraphRegion() const;
	const CList<IPacket*, IPacket*>& GetPacketList() const;
	CList<IPacket*, IPacket*>& GetPacketList();
	IPacket* GetPacket(const int nIndex) const;
	CCoordinateCalculatorData GetCoordinateCalculatorData() const;
	CCoordinateCalculatorData& GetCoordinateCalculatorData();

	bool DoesCalculator(const CPoint& point) const;

	double ApplyLog(const double& dData) const;
	double ApplyExp(const double& dData) const;

private:
	CRect m_noFrameGraphRegion;
	CList<IPacket*, IPacket*> m_packetList;
	CCoordinateCalculatorData m_calData;
};


class CAutoTrendDrawingCalculator
{
public:
	bool GetDrawingPoints(const CAutoTrendDrawingCalculatorData& calData, const int nCurrentDataIndex, 
			CPoint& high_leftPt, CPoint& high_rightPt, CPoint& low_leftPt, CPoint& low_rightPt) const;
	CString GetText(IPacket* pPacket, const int nStartIndex, const int nEndIndex) const;

private:
	bool DoesRelativePoints(IPacket* pIndexPacket, IPacket* pDataPacket) const;
	bool DoesExistenceText(IPacket* pPacket, const int nStartIndex, const int nEndIndex) const;

	bool GetDrawingPoints(const CAutoTrendDrawingCalculatorData& calData, 
		const int nCurrentDataIndex, const int nPacketIndex, CPoint& leftPt, CPoint& rightPt) const;
	bool GetRelativePoints(IPacket* pIndexPacket, IPacket* pDataPacket, const int nCurrentDataIndex, CAutoTrendPoint& atLeftPt, CAutoTrendPoint& atRightPt) const;
	bool GetAbsolutePoints(const CCoordinateCalculatorData& calData, const CAutoTrendPoint& atLeftPt, const CAutoTrendPoint& atRightPt, CPoint& leftPt, CPoint& rightPt) const;

	int GetComparisonData(IPacket* pDataPacket) const;
};

#endif // !defined(AFX_AUTOTRENDDATA_H__F96C8DB1_CCA1_4662_9488_EA888E677740__INCLUDED_)
