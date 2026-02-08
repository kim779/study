// ScaleBaseData.cpp: implementation of the CScaleBaseData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScaleBaseData.h"

#include "Conversion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// class CScaleCompart  -> scale 구분 type 저장

CScaleCompart& CScaleCompart::operator=(const CScaleCompart& data)
{
	if(this == &data)
		return *this;

	m_strDateCompart = data.GetDateCompart();
	m_strTimeCompart = data.GetTimeCompart();
	m_eTextType = data.GetScaleTextType();
	return *this;
}

CScaleCompart::CScaleCompart():
	m_strDateCompart("."),
	m_strTimeCompart(":"),
	m_eTextType(CScaleBaseData::HORZ_TEXT_COMPART)
{
}

CScaleCompart::CScaleCompart(const CString& dateCompart, const CString& timeCompart, const CScaleBaseData::TEXTCOMPARTTYPE eTextType):
	m_strDateCompart(dateCompart),
	m_strTimeCompart(timeCompart),
	m_eTextType(eTextType)
{
}

CString CScaleCompart::GetDateCompart() const
{
	return m_strDateCompart;
}

CString CScaleCompart::GetTimeCompart() const
{
	return m_strTimeCompart;
}

CScaleBaseData::TEXTCOMPARTTYPE CScaleCompart::GetScaleTextType() const
{
	return m_eTextType;
}

void CScaleCompart::SetCompart(const CString& strData)
{
	CString strTemp = strData;
	if(strTemp.IsEmpty())
		return;

	m_strDateCompart = CDataConversion::GetStringData(strTemp, ";");
	m_strTimeCompart = CDataConversion::GetStringData(strTemp, ";");
	if(!strTemp.IsEmpty()){
		m_eTextType = (CScaleBaseData::TEXTCOMPARTTYPE) atoi(CDataConversion::GetStringData(strTemp, ";"));
	}
}

void CScaleCompart::SetDateCompart(const CString& dateCompart)
{
	m_strDateCompart = dateCompart;
}

void CScaleCompart::SetTimeCompart(const CString& timeCompart)
{
	m_strTimeCompart = timeCompart;
}

void CScaleCompart::SetScaleTextType(const CScaleBaseData::TEXTCOMPARTTYPE eTextType)
{
	m_eTextType = eTextType;
}


//////////////////////////////////////////////////////////////////////
// class CScale

CScale& CScale::operator=(const CScale& data)
{
	if(this == &data)
		return *this;

	m_strScaleData = data.GetScaleData();
	m_eScaleGrid = data.GetGridType();
	return *this;
}

CScale::CScale():
	m_eScaleGrid(CScaleBaseData::SLNONE)
{
}

void CScale::SetData(const CString& scaleData)
{
	m_strScaleData = scaleData;
}

void CScale::SetScaleData(const CString& scaleData)
{
	m_strScaleData = scaleData;
}

void CScale::SetGridType(const CScaleBaseData::SCALEGRIDTYPE eGridType)
{
	m_eScaleGrid = eGridType;
}

CString CScale::GetScaleData() const
{
	return m_strScaleData;
}

CScaleBaseData::SCALEGRIDTYPE CScale::GetGridType() const
{
	return m_eScaleGrid;
}

// protected ===============================================================================
void CScale::SetGridType(const CString& strGridType)
{
	if(strGridType.IsEmpty())
		return;

	m_eScaleGrid = (CScaleBaseData::SCALEGRIDTYPE) atoi(strGridType);
}


////////////////////////////////////////////////////////////////////////////////////////////
// class CHorizontalScale

CHorizontalScale& CHorizontalScale::operator=(const CHorizontalScale& data)
{
	if(this == &data)
		return *this;

	CScale::operator = ((CScale) data);
	m_scaleCompart = data.GetScaleCompart();
	m_ePosition = data.GetHorzScalePosition();
	return *this;
}

CHorizontalScale::CHorizontalScale():
	CScale(),
	m_ePosition(CScaleBaseData::HORZ_NONE)
{
}

// -----------------------------------------------------------------------------------------
void CHorizontalScale::SetScaleData(const CString& scaleData)
{
	// 사용할 Data;위치;grid타입;일자타입;시간타입;text타입;
	CString temp = scaleData;
	CScale::SetScaleData(CDataConversion::GetStringData(temp, ";"));
	SetHorzScalePosition(CDataConversion::GetStringData(temp, ";"));
	CScale::SetGridType(CDataConversion::GetStringData(temp, ";"));
	m_scaleCompart.SetCompart(temp);
}

void CHorizontalScale::SetHorzScalePosition(const CString& strPosition)
{
	if(strPosition.IsEmpty())
		return;

	m_ePosition = (CScaleBaseData::HORZSCALEPOSITION) atoi(strPosition);
}

void CHorizontalScale::SetHorzScalePosition(const CScaleBaseData::HORZSCALEPOSITION ePosition)
{
	m_ePosition = ePosition;
}

void CHorizontalScale::SetScaleCompart(const CScaleCompart& scaleCompart)
{
	m_scaleCompart = scaleCompart;
}

// -----------------------------------------------------------------------------------------
CScaleCompart CHorizontalScale::GetScaleCompart() const
{
	return m_scaleCompart;
}

CScaleCompart& CHorizontalScale::GetScaleCompart()
{
	return m_scaleCompart;
}

CScaleBaseData::HORZSCALEPOSITION CHorizontalScale::GetHorzScalePosition() const
{
	return m_ePosition;
}

// scale의 모든 data
CString CHorizontalScale::GetScaleAllData() const
{
	// 사용할 Data;위치;grid타입;일자타입;시간타입;text타입;log;차트뒤집기;

	CString scaleAllData;
	scaleAllData += CScale::GetScaleData() + ';';
	scaleAllData += CDataConversion::IntToString((int)m_ePosition) + ';';
	scaleAllData += CDataConversion::IntToString((int)CScale::GetGridType()) + ';';
	scaleAllData += m_scaleCompart.GetDateCompart() + ';';
	scaleAllData += m_scaleCompart.GetTimeCompart() + ';';
	scaleAllData += CDataConversion::IntToString((int)m_scaleCompart.GetScaleTextType()) + ';';
	return scaleAllData;
}


////////////////////////////////////////////////////////////////////////////////////////////
// class CVerticalScale

CVerticalScale& CVerticalScale::operator=(const CVerticalScale& data)
{
	if(this == &data)
		return *this;

	CScale::operator = ((CScale) data);
	m_ePosition = data.GetVertScalePosition();
	m_eUnitType = data.GetVertScaleUnitType();
	return *this;
}

CVerticalScale::CVerticalScale():
	CScale(),
	m_ePosition(CScaleBaseData::VERT_NONE),
	m_eUnitType(CScaleBaseData::VERT_NUM)
{
}

// -----------------------------------------------------------------------------------------
void CVerticalScale::SetScaleData(const CString& scaleData)
{
	// 사용할 Data;위치;grid타입;unit타입;
	CString temp = scaleData;
	CScale::SetScaleData(CDataConversion::GetStringData(temp, ";"));
	if(temp.IsEmpty())
		return;
	SetVertScalePosition(CDataConversion::GetStringData(temp, ";"));
	CScale::SetGridType(CDataConversion::GetStringData(temp, ";"));
	SetVertScaleUnitType(CDataConversion::GetStringData(temp, ";"));
}

// 값 setting
void CVerticalScale::SetVertScalePosition(const CString& strPosition)
{
	if(strPosition.IsEmpty())
		return;
	m_ePosition = (CScaleBaseData::VERTSCALEPOSITION) atoi(strPosition);
}

void CVerticalScale::SetVertScalePosition(const CScaleBaseData::VERTSCALEPOSITION ePosition)
{
	m_ePosition = ePosition;
}

void CVerticalScale::SetVertScaleUnitType(const CString& strUnitType)
{
	if(strUnitType.IsEmpty())
		return;
	m_eUnitType = (CScaleBaseData::VERTSCALEUNITTYPE) atoi(strUnitType);
}

void CVerticalScale::SetVertScaleUnitType(const CScaleBaseData::VERTSCALEUNITTYPE eUnitType)
{
	m_eUnitType = eUnitType;
}

// -----------------------------------------------------------------------------------------
CScaleBaseData::VERTSCALEPOSITION CVerticalScale::GetVertScalePosition() const
{
	return m_ePosition;
}

CScaleBaseData::VERTSCALEUNITTYPE CVerticalScale::GetVertScaleUnitType() const
{
	return m_eUnitType;
}

// scale의 모든 data
CString CVerticalScale::GetScaleAllData() const
{
	// 사용할 Data;위치;grid타입;현재가 그리기 여부;
	CString scaleAllData;
	scaleAllData += CScale::GetScaleData() + ';';
	scaleAllData += CDataConversion::IntToString((int)m_ePosition) + ';';
	scaleAllData += CDataConversion::IntToString((int)CScale::GetGridType()) + ';';
	scaleAllData += CDataConversion::IntToString((int)m_eUnitType) + ';';
	return scaleAllData + "\r\n";
}

CString CVerticalScale::GetScaleAllData(const CString& strScaleData) const
{
	CString scaleAllData;
	scaleAllData += strScaleData + ';';
	scaleAllData += CDataConversion::IntToString((int)m_ePosition) + ';';
	scaleAllData += CDataConversion::IntToString((int)CScale::GetGridType()) + ';';
	scaleAllData += CDataConversion::IntToString((int)m_eUnitType) + ';';
	return scaleAllData + "\r\n";
}


////////////////////////////////////////////////////////////////////////////////////////////
// class CHorzScaleRegionHeight  -> 가로 scale 높이 저장

CHorzScaleRegionHeight::CHorzScaleRegionHeight():
	m_nTopHeight(0),
	m_nBottomHeight(0)
{
}

CHorzScaleRegionHeight::CHorzScaleRegionHeight(const int topHeight, const int bottomHeight):
	m_nTopHeight(topHeight),
	m_nBottomHeight(bottomHeight)
{
}

// public =================================================================================
void CHorzScaleRegionHeight::SetTopHorzScaleRegionHeight(const int topHeight)
{
	m_nTopHeight = topHeight;
}

void CHorzScaleRegionHeight::SetBottomHorzScaleRegionHeight(const int bottomHeight)
{
	m_nBottomHeight = bottomHeight;
}

int CHorzScaleRegionHeight::GetTopHorzScaleRegionHeight() const
{
	return m_nTopHeight;
}

int CHorzScaleRegionHeight::GetBottomHorzScaleRegionHeight() const
{
	return m_nBottomHeight;
}


///////////////////////////////////////////////////////////////////////////////////////////
// class CVertScaleRegionWidth  -> 세로 scale 폭 저장

CVertScaleRegionWidth::CVertScaleRegionWidth():
	m_nLeftWidth(0),
	m_nRightWidth(0)
{
}

CVertScaleRegionWidth::CVertScaleRegionWidth(const int leftWidth, const int rightWidth):
	m_nLeftWidth(leftWidth),
	m_nRightWidth(rightWidth)
{
}

// public =================================================================================
void CVertScaleRegionWidth::SetLeftVertScaleRegionWidth(const int leftWidth)
{
	m_nLeftWidth = leftWidth;
}

void CVertScaleRegionWidth::SetRightVertScaleRegionWidth(const int rightWidth)
{
	m_nRightWidth = rightWidth;
}

int CVertScaleRegionWidth::GetLeftVertScaleRegionWidth() const
{
	return m_nLeftWidth;
}

int CVertScaleRegionWidth::GetRightVertScaleRegionWidth() const
{
	return m_nRightWidth;
}


///////////////////////////////////////////////////////////////////////////////////////////
// class CHorzScaleRegion  -> 가로 scale 영역 저장

CHorzScaleRegion::CHorzScaleRegion():
	m_topScaleRegion(0, 0, 0, 0),
	m_bottomScaleRegion(0, 0, 0, 0)
{
}

// public =================================================================================
void CHorzScaleRegion::SetTopScaleRegion(const CRect& region)
{
	m_topScaleRegion = region;
}

void CHorzScaleRegion::SetBottomScaleRegion(const CRect& region)
{
	m_bottomScaleRegion = region;
}

CRect CHorzScaleRegion::GetTopScaleRegion() const
{
	return m_topScaleRegion;
}

CRect CHorzScaleRegion::GetBottomScaleRegion() const
{
	return m_bottomScaleRegion;
}

bool CHorzScaleRegion::PointInRegion(const CPoint& point) const
{
	return (m_topScaleRegion.PtInRect(point) || m_bottomScaleRegion.PtInRect(point));
}


///////////////////////////////////////////////////////////////////////////////////////////
// class CVertScaleRegion  -> 세로 scale 영역 저장

CVertScaleRegion::CVertScaleRegion():
	m_leftScaleRegion(0, 0, 0, 0),
	m_rightScaleRegion(0, 0, 0, 0)
{
}

// public =================================================================================
void CVertScaleRegion::SetLeftScaleRegion(const CRect& region)
{
	m_leftScaleRegion = region;
}

void CVertScaleRegion::SetRightScaleRegion(const CRect& region)
{
	m_rightScaleRegion = region;
}

void CVertScaleRegion::OffsetRegion(const int nTop, const int nBottom)
{
	m_leftScaleRegion.top += nTop;
	m_leftScaleRegion.bottom += nBottom;
	m_rightScaleRegion.top += nTop;
	m_rightScaleRegion.bottom += nBottom;
}

// ----------------------------------------------------------------------------------------
CRect CVertScaleRegion::GetLeftScaleRegion() const
{
	return m_leftScaleRegion;
}

CRect CVertScaleRegion::GetRightScaleRegion() const
{
	return m_rightScaleRegion;
}

CRect CVertScaleRegion::GetLeftScaleTextRegion() const
{
	if(m_leftScaleRegion.Width() <= 0)
		return CRect(m_leftScaleRegion.left, m_leftScaleRegion.top,
			m_leftScaleRegion.right, m_leftScaleRegion.bottom);

	return CRect(m_leftScaleRegion.left +MARGIN, m_leftScaleRegion.top,
		m_leftScaleRegion.right -MARGIN*2, m_leftScaleRegion.bottom);
}

CRect CVertScaleRegion::GetLeftScaleGridRegion() const
{
	if(m_leftScaleRegion.Width() <= 0)
		return CRect(m_leftScaleRegion.right, m_leftScaleRegion.top,
			m_leftScaleRegion.right, m_leftScaleRegion.bottom);

	return CRect(m_leftScaleRegion.right -MARGIN, m_leftScaleRegion.top,
		m_leftScaleRegion.right, m_leftScaleRegion.bottom);
}

CRect CVertScaleRegion::GetRightScaleTextRegion() const
{
	if(m_rightScaleRegion.Width() <= 0)
		return CRect(m_rightScaleRegion.left, m_rightScaleRegion.top,
			m_rightScaleRegion.right, m_rightScaleRegion.bottom);

	return CRect(m_rightScaleRegion.left +MARGIN*2, m_rightScaleRegion.top,
		m_rightScaleRegion.right -MARGIN, m_rightScaleRegion.bottom);
}

CRect CVertScaleRegion::GetRightScaleGridRegion() const
{
	if(m_rightScaleRegion.Width() <= 0)
		return CRect(m_rightScaleRegion.left, m_rightScaleRegion.top,
			m_rightScaleRegion.left, m_rightScaleRegion.bottom);

	return CRect(m_rightScaleRegion.left, m_rightScaleRegion.top,
		m_rightScaleRegion.left +MARGIN, m_rightScaleRegion.bottom);
}

bool CVertScaleRegion::PointInRegion(const CPoint& point) const
{
	return (m_rightScaleRegion.PtInRect(point) || m_leftScaleRegion.PtInRect(point));
}
