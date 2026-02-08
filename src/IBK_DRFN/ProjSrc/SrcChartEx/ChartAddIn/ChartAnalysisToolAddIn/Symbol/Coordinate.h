// Coordinate.h: interface for the CCoordinate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COORDINATE_H__738A8587_2CBE_4735_9219_34C5FB5D2705__INCLUDED_)
#define AFX_COORDINATE_H__738A8587_2CBE_4735_9219_34C5FB5D2705__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// 객체에 저장된 좌표 -- 상대좌표
class CCoordinate
{
public:
	CCoordinate();
	CCoordinate(const double& x, const double& y);
	// 20081007 JS.Kim	사용자시간대 선택
	CCoordinate(const double& x, const double& y, const int nTimeDiff);
	CCoordinate(const CPoint& point);

	double GetX() const;
	double GetY() const;
	CPoint GetPixel() const;
	// 20081007 JS.Kim	사용자시간대 선택. 
	int GetTimeDiff() const;

	void SetX(const double& x);
	void SetY(const double& y);
	// 20081007 JS.Kim	사용자시간대 선택. 
	void SetTimeDiff(int nTimeDiff);

private:
	double m_nX;
	double m_nY;
	// 20081007 JS.Kim	사용자시간대 선택. 
	int		m_nTimeDiff;		//원래 데이타의 시간대로 저장하기 위하여 현재 사용자가 선택한 시간대와의 차를 갖고 있는다.
};

#endif // !defined(AFX_COORDINATE_H__738A8587_2CBE_4735_9219_34C5FB5D2705__INCLUDED_)
