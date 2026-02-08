// CoordinateCalculatorData.h: interface for the CCoordinateCalculatorData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COORDINATECALCULATORDATA_H__40E43F4C_1FCC_4DF5_9713_EB7152A22CB1__INCLUDED_)
#define AFX_COORDINATECALCULATORDATA_H__40E43F4C_1FCC_4DF5_9713_EB7152A22CB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPacket;

class CVertScaleType
{
public:
	CVertScaleType& operator=(const CVertScaleType& vertScaleType);

public:
	CVertScaleType();
	CVertScaleType(const CVertScaleType& vertScaleType);
	CVertScaleType(const bool bIsLog, const bool bIsInvert);

	void SetScaleType(const bool bIsLog, const bool bIsInvert);
	void SetScaleLogType(const bool bIsLog);
	void SetScaleInverType(const bool bIsInvert);

	bool IsLog() const;
	bool IsInvert() const;

	double ApplyLog(const double& dData) const;
	double ApplyExp(const double& dData) const;

private:
	bool m_bIsLog;		// log타입
	bool m_bIsInvert;	// 뒤집기 타입
};


class CDisplayDataVertMinMax
{
public:
	CDisplayDataVertMinMax& operator=(const CDisplayDataVertMinMax& data);

public:
	CDisplayDataVertMinMax();
	CDisplayDataVertMinMax(const double& dMin, const double& dMax, const CVertScaleType& vertScaleType);

	void SetMinMax(const double& dMin, const double& dMax);
	void NormalizeMinMax();

	double GetMin() const;
	double GetMax() const;
	CVertScaleType GetVertScaleType() const;
	CVertScaleType& GetVertScaleType();
	double GetMin_ScaleType() const;
	double GetMax_ScaleType() const;

private:
	double m_dMin;		// block안의 보여질 data의 최소값
	double m_dMax;		// block안의 보여질 data의 최대값
	CVertScaleType m_vertScaleType;	// 세로 scale type (log, invert)
};


// 좌표를 구하기 위해 필요한 data.
class CCoordinateCalculatorData
{
public:
	CCoordinateCalculatorData& operator=(const CCoordinateCalculatorData& data);

public:
	CCoordinateCalculatorData();

	void SetRegion(const CRect& region);
	void SetDisplayDataStartEnd(const int start, const int end);
	void SetVertDataType(const double& dDataType);
	void SetPacket_X(CPacket* pDataPacket);
	void SetPacket_Y(CPacket* pClosePacket);

	CRect GetRegion() const;
	int GetDisplayDataStart() const;
	int GetDisplayDataEnd() const;
	int GetDisplayDataCount() const;
	double GetDisplayDataMin_ScaleType() const;
	double GetDisplayDataMax_ScaleType() const;
	CDisplayDataVertMinMax GetDisplayDataVertMinMax() const;
	CDisplayDataVertMinMax& GetDisplayDataVertMinMax();
	CVertScaleType GetVertScaleType() const;
	double GetVertDataType() const;
	CPacket* GetPacket_X() const;
	CPacket* GetPacket_Y() const;

	bool DoesCalculator(const CPoint& point) const;
	
	double ApplyLog(const double& dData) const;
	double ApplyExp(const double& dData) const;

private:
	CRect m_region;				// 그리는 영역
	int m_nDisplayDataStart;	// block안의 보여질 처음 data
	int m_nDisplayDataEnd;		// block안의 보여질 마지막 data
	CDisplayDataVertMinMax m_displayDataVertMinMax;
	double m_dVertDataType;		// 세로 scale의 dataType
	CPacket* m_pPacket_x;		// 가로 scale <ex:자료일자> packet
	CPacket* m_pPacket_y;		// 세로 scale <ex:종가> packet
};

#endif // !defined(AFX_COORDINATECALCULATORDATA_H__40E43F4C_1FCC_4DF5_9713_EB7152A22CB1__INCLUDED_)
