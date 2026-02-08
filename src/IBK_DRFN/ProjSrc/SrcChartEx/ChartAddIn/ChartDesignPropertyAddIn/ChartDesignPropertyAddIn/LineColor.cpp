// LineColor.cpp: implementation of the CLineColor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "LineColor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineColor::CLineColor()
{
}

CLineColor::~CLineColor()
{

}

void CLineColor::SetLineColor(const int nLineIndex, const CString &strColorValue)
{
	m_pColorStringList.AddTail(strColorValue);
	m_pOriginalColorStringList.AddTail(strColorValue);
}

CString CLineColor::MakeFromSixLineColorStringToOneString(const int nGraphCount)
{
	CString strValue = "";
	if(m_pColorStringList.GetCount() < nGraphCount) {
		POSITION Originalpos = m_pOriginalColorStringList.GetHeadPosition();
		while(Originalpos) {
			CString strColor = m_pOriginalColorStringList.GetNext(Originalpos);
			m_pColorStringList.AddTail(strColor);
		}
	}

	if(m_pColorStringList.GetCount() <= 0) return "";
	int i = 0;
	POSITION pos = m_pColorStringList.GetHeadPosition();
	while(pos) {
		CString temp = m_pColorStringList.GetNext(pos);
		if(i < nGraphCount) {
			m_pColorStringList.RemoveHead();
			if(temp.Right(1) != ";")
				temp += ";";
			strValue += temp;
		}
		i++;
	}
	return strValue;
}


//////////////////////////////////////////////////////////////////////
// class CGraphSetting2Data

CGraphSetting2Data::CGraphSetting2Data(const bool bHighNow, const bool bLowHigh, 
		const bool bLowNow, const bool bPrevNow, const int nCurDataType):
	m_bHighNow(bHighNow), 
	m_bLowHigh(bLowHigh), 
	m_bLowNow(bLowNow), 
	m_bPrevNow(bPrevNow), 
	m_nCurDataType(nCurDataType)
{
}

bool CGraphSetting2Data::IsHighNow() const
{
	return m_bHighNow;
}

bool CGraphSetting2Data::IsLowHigh() const
{
	return m_bLowHigh;
}

bool CGraphSetting2Data::IsLowNow() const
{
	return m_bLowNow;
}

bool CGraphSetting2Data::IsPrevNow() const
{
	return m_bPrevNow;
}

int	CGraphSetting2Data::GetCurDataType() const
{
	return m_nCurDataType;
}

// -------------------------------------------------------------------
void CGraphSetting2Data::SetData(const bool bHighNow, const bool bLowHigh, const bool bLowNow, const bool bPrevNow, const int nCurDataType)
{
	m_bHighNow = bHighNow;
	m_bLowHigh = bLowHigh;
	m_bLowNow = bLowNow;
	m_bPrevNow = bPrevNow;
	m_nCurDataType = nCurDataType;
}

void CGraphSetting2Data::SetHighNow(const bool bHighNow)
{
	m_bHighNow = bHighNow;
}

void CGraphSetting2Data::SetLowHigh(const bool bLowHigh)
{
	m_bLowHigh = bLowHigh;
}

void CGraphSetting2Data::SetLowNow(const bool bLowNow)
{
	m_bLowNow = bLowNow;
}

void CGraphSetting2Data::SetPrevNow(const bool bPrevNow)
{
	m_bPrevNow = bPrevNow;
}

void CGraphSetting2Data::SetCurDataType(const int nCurDataType)
{
	m_nCurDataType = nCurDataType;
}
