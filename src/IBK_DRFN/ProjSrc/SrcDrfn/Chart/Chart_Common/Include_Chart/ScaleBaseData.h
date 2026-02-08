// ScaleBaseData.h: interface for the CScaleBaseData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALEBASEDATA_H__F252545D_49F3_4EC9_B326_F6CF6CE4EA89__INCLUDED_)
#define AFX_SCALEBASEDATA_H__F252545D_49F3_4EC9_B326_F6CF6CE4EA89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"

// 구분 type 저장 class : 일짜, 시간, text 
class CScaleCompart
{
public:
	CScaleCompart& operator=(const CScaleCompart& data);

public:
	CScaleCompart();
	CScaleCompart(const CString& dateCompart, const CString& timeCompart, const CScaleBaseData::TEXTCOMPARTTYPE eTextType);

	CString GetDateCompart() const;
	CString GetTimeCompart() const;
	CScaleBaseData::TEXTCOMPARTTYPE GetScaleTextType() const;

	void SetCompart(const CString& strData);
	void SetDateCompart(const CString& dateCompart);
	void SetTimeCompart(const CString& timeCompart);
	void SetScaleTextType(const CScaleBaseData::TEXTCOMPARTTYPE eTextType);

private:
	CString m_strDateCompart;
	CString m_strTimeCompart;
	CScaleBaseData::TEXTCOMPARTTYPE m_eTextType;
};


// scale
class CScale
{
public:
	CScale& operator=(const CScale& data);

public:
	CScale();

	// scale로 사용할 data에 setting
	void SetData(const CString& scaleData);
	virtual void SetScaleData(const CString& scaleData);
	void SetGridType(const CScaleBaseData::SCALEGRIDTYPE eGridType);

	CString GetScaleData() const;
	CScaleBaseData::SCALEGRIDTYPE GetGridType() const;

protected:
	void SetGridType(const CString& strGridType);

private:
	CString m_strScaleData; // scale로 사용할 data. (packet 중 scale로 사용하는 것 - "자료일자", "자료시간", "시가"...)	
	CScaleBaseData::SCALEGRIDTYPE m_eScaleGrid;
};


// 가로 Scale
class CHorizontalScale : public CScale
{
public:
	CHorizontalScale& operator=(const CHorizontalScale& data);

public:
	CHorizontalScale();

	// 가로 scale에 대한 모든 data를 setting
	virtual void SetScaleData(const CString& scaleData);
	void SetHorzScalePosition(const CString& strPosition);
	void SetHorzScalePosition(const CScaleBaseData::HORZSCALEPOSITION ePosition);
	void SetScaleCompart(const CScaleCompart& scaleCompart);

	CScaleBaseData::HORZSCALEPOSITION GetHorzScalePosition() const;
	CScaleCompart GetScaleCompart() const;
	CScaleCompart& GetScaleCompart();

	// scale의 모든 data
	CString GetScaleAllData() const;

private:
	CScaleBaseData::HORZSCALEPOSITION m_ePosition; // Scale position (상단, 하단, 양쪽, Hide)
	CScaleCompart m_scaleCompart; // 날짜, 시간, text scale 구분타입
};


// 세로 Scale
class CVerticalScale : public CScale  
{
public:
	CVerticalScale& operator=(const CVerticalScale& data);

public:
	CVerticalScale();

	// 값 setting
	virtual void SetScaleData(const CString& scaleData);
	void SetVertScalePosition(const CString& strPosition);
	void SetVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition);
	void SetVertScaleUnitType(const CString& strUnitType);
	void SetVertScaleUnitType(const CScaleBaseData::VERTSCALEUNITTYPE eUnitType);

	CScaleBaseData::VERTSCALEPOSITION GetVertScalePosition() const;
	CScaleBaseData::VERTSCALEUNITTYPE GetVertScaleUnitType() const;

	// scale의 모든 data
	CString GetScaleAllData() const;
	CString GetScaleAllData(const CString& strScaleData) const;

private:
	CScaleBaseData::VERTSCALEPOSITION m_ePosition;	// 세로 scale 위치
	CScaleBaseData::VERTSCALEUNITTYPE m_eUnitType;	// 세로 scale 단위 표현법
};


// ============================================================================
// 가로 scale의 높이 저장 class
class CHorzScaleRegionHeight  
{
public:
	CHorzScaleRegionHeight();
	CHorzScaleRegionHeight(const int topHeight, const int bottomHeight);

	void SetTopHorzScaleRegionHeight(const int topHeight);
	void SetBottomHorzScaleRegionHeight(const int bottomHeight);

	int GetTopHorzScaleRegionHeight() const;
	int GetBottomHorzScaleRegionHeight() const;

private:
	int m_nTopHeight;
	int m_nBottomHeight;
};

// 세로 scale의 폭 저장 class
class CVertScaleRegionWidth  
{
public:
	CVertScaleRegionWidth();
	CVertScaleRegionWidth(const int leftWidth, const int rightWidth);

	void SetLeftVertScaleRegionWidth(const int leftWidth);
	void SetRightVertScaleRegionWidth(const int rightWidth);

	int GetLeftVertScaleRegionWidth() const;
	int GetRightVertScaleRegionWidth() const;

private:
	int m_nLeftWidth;
	int m_nRightWidth;
};

// 가로 scale 영역을 저장 class
class CHorzScaleRegion
{
public:
	CHorzScaleRegion();

	void SetTopScaleRegion(const CRect& region);
	void SetBottomScaleRegion(const CRect& region);

	CRect GetTopScaleRegion() const;
	CRect GetBottomScaleRegion() const;

	bool PointInRegion(const CPoint& point) const;

private:
	CRect m_topScaleRegion;
	CRect m_bottomScaleRegion;
};

// 세로 scale 영역을 저장 class
class CVertScaleRegion
{
public:
	CVertScaleRegion();

	void SetLeftScaleRegion(const CRect& region);
	void SetRightScaleRegion(const CRect& region);
	void OffsetRegion(const int nTop, const int nBottom);

	CRect GetLeftScaleRegion() const;
	CRect GetRightScaleRegion() const;
	CRect GetLeftScaleTextRegion() const;
	CRect GetLeftScaleGridRegion() const;
	CRect GetRightScaleTextRegion() const;
	CRect GetRightScaleGridRegion() const;
	
	bool PointInRegion(const CPoint& point) const;

private:
	CRect m_leftScaleRegion;
	CRect m_rightScaleRegion;
};

#endif // !defined(AFX_SCALEBASEDATA_H__F252545D_49F3_4EC9_B326_F6CF6CE4EA89__INCLUDED_)
