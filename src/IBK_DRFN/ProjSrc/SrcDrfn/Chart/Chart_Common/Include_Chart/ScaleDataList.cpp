// ScaleDataList.cpp: implementation of the CScaleDataList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ScaleDataList.h"

#include <assert.h>

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
//class CInsertingBlockHorzScaleData

CInsertingBlockHorzScaleData& CInsertingBlockHorzScaleData::operator=(const CInsertingBlockHorzScaleData& data)
{
	if(this == &data)
		return *this;

	m_bOnlyBottomBlockShow = data.IsOnlyBottomBlockShow();
	m_horzScale = data.GetHorizontalScale();
	return *this;
}

CInsertingBlockHorzScaleData::CInsertingBlockHorzScaleData():
	m_bOnlyBottomBlockShow(false)
{
}

CInsertingBlockHorzScaleData::CInsertingBlockHorzScaleData(const bool bOnlyBottomBlockShow, const CHorizontalScale& horzScale):
	m_bOnlyBottomBlockShow(bOnlyBottomBlockShow),
	m_horzScale(horzScale)
{
}

bool CInsertingBlockHorzScaleData::IsOnlyBottomBlockShow() const
{
	return m_bOnlyBottomBlockShow;
}

CHorizontalScale CInsertingBlockHorzScaleData::GetHorizontalScale() const
{
	return m_horzScale;
}

CHorizontalScale& CInsertingBlockHorzScaleData::GetHorizontalScale()
{
	return m_horzScale;
}

void CInsertingBlockHorzScaleData::SetOnlyBottomBlockShow(const bool bOnlyBottomBlockShow)
{
	m_bOnlyBottomBlockShow = bOnlyBottomBlockShow;
}

void CInsertingBlockHorzScaleData::SetHorizontalScale(const CHorizontalScale& horzScale)
{
	m_horzScale = horzScale;
}


///////////////////////////////////////////////////////////////////////////////
//class CMovingGraphVertScaleData

CMovingGraphVertScaleData::CMovingGraphVertScaleData():
	m_nVertScaleGroupIndex(0)
{
}

CMovingGraphVertScaleData::CMovingGraphVertScaleData(const int nVertScaleGroupIndex, const CVerticalScale& verticalScale):
	m_nVertScaleGroupIndex(nVertScaleGroupIndex),
	m_vertScale(verticalScale)
{
}

int CMovingGraphVertScaleData::GetVertScaleGroupIndex() const
{
	return m_nVertScaleGroupIndex;
}

CVerticalScale CMovingGraphVertScaleData::GetVerticalScale() const
{
	return m_vertScale;
}

void CMovingGraphVertScaleData::SetVertScaleGroupIndex(const int nVertScaleGroupIndex)
{
	m_nVertScaleGroupIndex = nVertScaleGroupIndex;
}

void CMovingGraphVertScaleData::SetVerticalScale(const CVerticalScale& verticalScale)
{
	m_vertScale = verticalScale;
}


///////////////////////////////////////////////////////////////////////////////
//class CVerticalScaleGroupData

CVerticalScaleGroupData& CVerticalScaleGroupData::operator=(const CVerticalScaleGroupData& vertScaleGroupData)
{
	if(this == &vertScaleGroupData)
		return *this;

	m_vertScale = vertScaleGroupData.GetVertScale();
	m_strVertScaleTitle = vertScaleGroupData.GetVertScaleTitle();
	return *this;
}

// public =====================================================================
CVerticalScaleGroupData::CVerticalScaleGroupData():
	m_strVertScaleTitle("")
{
}

CVerticalScaleGroupData::CVerticalScaleGroupData(const CVerticalScale& vertScale, const CString& vertScaleTitle):
	m_vertScale(vertScale),
	m_strVertScaleTitle(vertScaleTitle)
{
}

// ----------------------------------------------------------------------------
CVerticalScale& CVerticalScaleGroupData::GetVertScale()
{
	return m_vertScale;
}

CVerticalScale CVerticalScaleGroupData::GetVertScale() const
{
	return m_vertScale;
}

CString CVerticalScaleGroupData::GetVertScaleTitle() const
{
	return m_strVertScaleTitle;
}

void CVerticalScaleGroupData::SetVertScale(const CVerticalScale& vertScale)
{
	m_vertScale = vertScale;
}

void CVerticalScaleGroupData::SetVertScaleTitle(const CString& vertScaleTitle)
{
	m_strVertScaleTitle = vertScaleTitle;
}


///////////////////////////////////////////////////////////////////////////////
//class CVerticalScaleGroupDataList

CVerticalScaleGroupDataList& CVerticalScaleGroupDataList::operator=(const CVerticalScaleGroupDataList& vertScaleGroupData)
{
	if(this == &vertScaleGroupData)
		return *this;

	m_nFirstIndex = vertScaleGroupData.GetFirstIndex();
	for(int nIndex = 0; nIndex < vertScaleGroupData.GetCount(); nIndex++){
		m_vertScaleGroupDataList.AddTail(vertScaleGroupData.GetAt(nIndex));
	}

	return *this;
}

CVerticalScaleGroupDataList::CVerticalScaleGroupDataList():
	m_nFirstIndex(0)
{
}

CVerticalScaleGroupDataList::CVerticalScaleGroupDataList(const CVerticalScaleGroupDataList& vertScaleGroupData)
{
	m_nFirstIndex = vertScaleGroupData.GetFirstIndex();
	for(int nIndex = 0; nIndex < vertScaleGroupData.GetCount(); nIndex++){
		m_vertScaleGroupDataList.AddTail(vertScaleGroupData.GetAt(nIndex));
	}
}


// public =====================================================================
void CVerticalScaleGroupDataList::AddTail(CVerticalScaleGroupData vertScaleGroupData)
{
	m_vertScaleGroupDataList.AddTail(vertScaleGroupData);
}

void CVerticalScaleGroupDataList::RemoveAll()
{
	m_vertScaleGroupDataList.RemoveAll();
}

// ----------------------------------------------------------------------------
void CVerticalScaleGroupDataList::SetFirstIndex(const int nFirstIndex)
{
	m_nFirstIndex = nFirstIndex;
}

// ----------------------------------------------------------------------------
int CVerticalScaleGroupDataList::GetFirstIndex() const
{
	return m_nFirstIndex;
}

int CVerticalScaleGroupDataList::GetCount() const
{
	return m_vertScaleGroupDataList.GetCount();
}

CVerticalScaleGroupData& CVerticalScaleGroupDataList::GetAt(const int nIndex)
{
	POSITION pos = m_vertScaleGroupDataList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_vertScaleGroupDataList.GetAt(pos);
}

CVerticalScaleGroupData CVerticalScaleGroupDataList::GetAt(const int nIndex) const
{
	POSITION pos = m_vertScaleGroupDataList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_vertScaleGroupDataList.GetAt(pos);
}


///////////////////////////////////////////////////////////////////////////////
//class CHorizontalScalePtrData

CHorizontalScalePtrData& CHorizontalScalePtrData::operator=(const CHorizontalScalePtrData& horzScalePtrData)
{
	if(this == &horzScalePtrData)
		return *this;

	m_pHorzScale = horzScalePtrData.GetHorzScale();
	m_strHorzScaleTitle = horzScalePtrData.GetHorzScaleTitle();
	return *this;
}

// public =====================================================================
CHorizontalScalePtrData::CHorizontalScalePtrData():
	m_pHorzScale(NULL),
	m_strHorzScaleTitle("")
{
}

CHorizontalScalePtrData::CHorizontalScalePtrData(CHorizontalScale* pHorzScale, const CString& horzScaleTitle):
	m_pHorzScale(pHorzScale),
	m_strHorzScaleTitle(horzScaleTitle)
{
}

// ----------------------------------------------------------------------------
CHorizontalScale* CHorizontalScalePtrData::GetHorzScale()
{
	return m_pHorzScale;
}

CHorizontalScale* CHorizontalScalePtrData::GetHorzScale() const
{
	return m_pHorzScale;
}

CString CHorizontalScalePtrData::GetHorzScaleTitle() const
{
	return m_strHorzScaleTitle;
}


///////////////////////////////////////////////////////////////////////////////
//class CHorizontalScalePtrDataList

CHorizontalScalePtrDataList& CHorizontalScalePtrDataList::operator=(const CHorizontalScalePtrDataList& horzScalePtrDataList)
{
	if(this == &horzScalePtrDataList)
		return *this;

	m_nFirstIndex = horzScalePtrDataList.GetFirstIndex();
	m_nMarginCount = horzScalePtrDataList.GetMarginCount();
	m_pScaleGridColor = horzScalePtrDataList.GetScaleGridColor();
	m_pScaleTextColor = horzScalePtrDataList.GetScaleTextColor();
	for(int nIndex = 0; nIndex < horzScalePtrDataList.GetCount(); nIndex++){
		m_horzScalePtrDataList.AddTail(horzScalePtrDataList.GetAt(nIndex));
	}

	return *this;
}

CHorizontalScalePtrDataList::CHorizontalScalePtrDataList():
	m_nFirstIndex(0),
	m_nMarginCount(0),
	m_pScaleGridColor(NULL),
	m_pScaleTextColor(NULL)
{
}

CHorizontalScalePtrDataList::CHorizontalScalePtrDataList(const CHorizontalScalePtrDataList& horzScalePtrDataList)
{
	m_nFirstIndex = horzScalePtrDataList.GetFirstIndex();
	m_nMarginCount = horzScalePtrDataList.GetMarginCount();
	m_pScaleGridColor = horzScalePtrDataList.GetScaleGridColor();
	m_pScaleTextColor = horzScalePtrDataList.GetScaleTextColor();
	for(int nIndex = 0; nIndex < horzScalePtrDataList.GetCount(); nIndex++){
		m_horzScalePtrDataList.AddTail(horzScalePtrDataList.GetAt(nIndex));
	}
}


// public =====================================================================
void CHorizontalScalePtrDataList::AddTail(CHorizontalScalePtrData horzScalePtrData)
{
	m_horzScalePtrDataList.AddTail(horzScalePtrData);
}

void CHorizontalScalePtrDataList::RemoveAll()
{
	m_horzScalePtrDataList.RemoveAll();
}

// ----------------------------------------------------------------------------
void CHorizontalScalePtrDataList::SetFirstIndex(const int nFirstIndex)
{
	m_nFirstIndex = nFirstIndex;
}

void CHorizontalScalePtrDataList::SetMarginCount(const int nMarginCount)
{
	m_nMarginCount = nMarginCount;
}

void CHorizontalScalePtrDataList::SetScaleColor(COLORREF* pGridColor, COLORREF* pTextColor)
{
	m_pScaleGridColor = pGridColor;
	m_pScaleTextColor = pTextColor;
}

void CHorizontalScalePtrDataList::SetScaleColor(const COLORREF& gridColor, const COLORREF& textColor)
{
	if(m_pScaleGridColor != NULL)
		*m_pScaleGridColor = gridColor;
	if(m_pScaleTextColor != NULL)
		*m_pScaleTextColor = textColor;
}

void CHorizontalScalePtrDataList::SetHorzScale(const int nIndex, const CHorizontalScale& horzScale)
{
	CHorizontalScale* pHorzScale = GetAt(nIndex).GetHorzScale();
	if(pHorzScale == NULL)
		return;

	pHorzScale->SetHorzScalePosition(horzScale.GetHorzScalePosition());
	pHorzScale->SetGridType(horzScale.GetGridType());
	pHorzScale->SetScaleCompart(horzScale.GetScaleCompart());
}

// ----------------------------------------------------------------------------
int CHorizontalScalePtrDataList::GetFirstIndex() const
{
	return m_nFirstIndex;
}

int CHorizontalScalePtrDataList::GetMarginCount() const
{
	return m_nMarginCount;
}

COLORREF* CHorizontalScalePtrDataList::GetScaleGridColor() const
{
	return m_pScaleGridColor;
}

COLORREF* CHorizontalScalePtrDataList::GetScaleTextColor() const
{
	return m_pScaleTextColor;
}

int CHorizontalScalePtrDataList::GetCount() const
{
	return m_horzScalePtrDataList.GetCount();
}

CHorizontalScalePtrData& CHorizontalScalePtrDataList::GetAt(const int nIndex)
{
	POSITION pos = m_horzScalePtrDataList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_horzScalePtrDataList.GetAt(pos);
}

CHorizontalScalePtrData CHorizontalScalePtrDataList::GetAt(const int nIndex) const
{
	POSITION pos = m_horzScalePtrDataList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_horzScalePtrDataList.GetAt(pos);
}


///////////////////////////////////////////////////////////////////////////////
//class CHorizontalScaleDataList

CHorizontalScaleDataList& CHorizontalScaleDataList::operator=(const CHorizontalScaleDataList& horzScaleDataList)
{
	if(this == &horzScaleDataList)
		return *this;

	m_scaleGridColor = horzScaleDataList.GetScaleGridColor();
	m_scaleTextColor = horzScaleDataList.GetScaleTextColor();
	for(int nIndex = 0; nIndex < horzScaleDataList.GetCount(); nIndex++){
		m_horzScaleList.AddTail(horzScaleDataList.GetAt(nIndex));
	}

	return *this;
}

CHorizontalScaleDataList::CHorizontalScaleDataList():
	m_scaleGridColor(RGB(202, 202, 202)),
	m_scaleTextColor(RGB(0, 0, 0))
{
}

CHorizontalScaleDataList::CHorizontalScaleDataList(const CHorizontalScaleDataList& horzScaleDataList)
{
	m_scaleGridColor = horzScaleDataList.GetScaleGridColor();
	m_scaleTextColor = horzScaleDataList.GetScaleTextColor();
	for(int nIndex = 0; nIndex < horzScaleDataList.GetCount(); nIndex++){
		m_horzScaleList.AddTail(horzScaleDataList.GetAt(nIndex));
	}
}

CHorizontalScaleDataList::CHorizontalScaleDataList(const CHorizontalScalePtrDataList* pHorzScalePtrDataList)
{
	if(pHorzScalePtrDataList == NULL)
		return;

	if(pHorzScalePtrDataList->GetScaleGridColor() != NULL)
		m_scaleGridColor = *pHorzScalePtrDataList->GetScaleGridColor();
	if(pHorzScalePtrDataList->GetScaleTextColor() != NULL)
		m_scaleTextColor = *pHorzScalePtrDataList->GetScaleTextColor();

	for(int nIndex = 0; nIndex < pHorzScalePtrDataList->GetCount(); nIndex++){
		m_horzScaleList.AddTail(*pHorzScalePtrDataList->GetAt(nIndex).GetHorzScale());
	}
}


// public =====================================================================
void CHorizontalScaleDataList::AddTail(CHorizontalScale horzScale)
{
	m_horzScaleList.AddTail(horzScale);
}

void CHorizontalScaleDataList::RemoveAll()
{
	m_horzScaleList.RemoveAll();
}

// ----------------------------------------------------------------------------
COLORREF CHorizontalScaleDataList::GetScaleGridColor() const
{
	return m_scaleGridColor;
}

COLORREF CHorizontalScaleDataList::GetScaleTextColor() const
{
	return m_scaleTextColor;
}

int CHorizontalScaleDataList::GetCount() const
{
	return m_horzScaleList.GetCount();
}

CHorizontalScale& CHorizontalScaleDataList::GetAt(const int nIndex)
{
	POSITION pos = m_horzScaleList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_horzScaleList.GetAt(pos);
}

CHorizontalScale CHorizontalScaleDataList::GetAt(const int nIndex) const
{
	POSITION pos = m_horzScaleList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_horzScaleList.GetAt(pos);
}


///////////////////////////////////////////////////////////////////////////////
//class CVerticalScalePtrData

CVerticalScalePtrData& CVerticalScalePtrData::operator=(const CVerticalScalePtrData& vertScalePtrData)
{
	if(this == &vertScalePtrData)
		return *this;

	m_pVertScale = vertScalePtrData.GetVertScale();
	m_strVertScaleTitle = vertScalePtrData.GetVertScaleTitle();
	return *this;
}

// public =====================================================================
CVerticalScalePtrData::CVerticalScalePtrData():
	m_pVertScale(NULL),
	m_strVertScaleTitle("")
{
}

CVerticalScalePtrData::CVerticalScalePtrData(CVerticalScale* pVertScale, const CString& vertScaleTitle):
	m_pVertScale(pVertScale),
	m_strVertScaleTitle(vertScaleTitle)
{
}

// ----------------------------------------------------------------------------
CVerticalScale* CVerticalScalePtrData::GetVertScale()
{
	return m_pVertScale;
}

CVerticalScale* CVerticalScalePtrData::GetVertScale() const
{
	return m_pVertScale;
}

CString CVerticalScalePtrData::GetVertScaleTitle() const
{
	return m_strVertScaleTitle;
}


///////////////////////////////////////////////////////////////////////////////
//class CVerticalScalePtrDataList

CVerticalScalePtrDataList& CVerticalScalePtrDataList::operator=(const CVerticalScalePtrDataList& vertScalePtrDataList)
{
	if(this == &vertScalePtrDataList)
		return *this;

	m_nFirstIndex = vertScalePtrDataList.GetFirstIndex();
	m_pScaleGridColor = vertScalePtrDataList.GetScaleGridColor();
	m_pScaleTextColor = vertScalePtrDataList.GetScaleTextColor();
	m_bEnableLogType = vertScalePtrDataList.IsEnableLogType();
	m_bEnableInvertType = vertScalePtrDataList.IsEnableInvertType();
	m_pbLogType = vertScalePtrDataList.GetScaleLogType();
	m_pbInvertType = vertScalePtrDataList.GetScaleInvertType();
	for(int nIndex = 0; nIndex < vertScalePtrDataList.GetCount(); nIndex++){
		m_vertScalePtrDataList.AddTail(vertScalePtrDataList.GetAt(nIndex));
	}

	return *this;
}

CVerticalScalePtrDataList::CVerticalScalePtrDataList():
	m_nFirstIndex(0),
	m_pScaleGridColor(NULL),
	m_pScaleTextColor(NULL),
	m_bEnableLogType(true),
	m_bEnableInvertType(true),
	m_pbLogType(NULL),
	m_pbInvertType(NULL)
{
}

CVerticalScalePtrDataList::CVerticalScalePtrDataList(const CVerticalScalePtrDataList& vertScalePtrDataList)
{
	m_nFirstIndex = vertScalePtrDataList.GetFirstIndex();
	m_pScaleGridColor = vertScalePtrDataList.GetScaleGridColor();
	m_pScaleTextColor = vertScalePtrDataList.GetScaleTextColor();
	m_bEnableLogType = vertScalePtrDataList.IsEnableLogType();
	m_bEnableInvertType = vertScalePtrDataList.IsEnableInvertType();
	m_pbLogType = vertScalePtrDataList.GetScaleLogType();
	m_pbInvertType = vertScalePtrDataList.GetScaleInvertType();
	for(int nIndex = 0; nIndex < vertScalePtrDataList.GetCount(); nIndex++){
		m_vertScalePtrDataList.AddTail(vertScalePtrDataList.GetAt(nIndex));
	}
}


// public =====================================================================
void CVerticalScalePtrDataList::AddTail(CVerticalScalePtrData vertScalePtrData)
{
	m_vertScalePtrDataList.AddTail(vertScalePtrData);
}

void CVerticalScalePtrDataList::RemoveAll()
{
	m_vertScalePtrDataList.RemoveAll();
}

// ----------------------------------------------------------------------------
void CVerticalScalePtrDataList::SetFirstIndex(const int nFirstIndex)
{
	m_nFirstIndex = nFirstIndex;
}

void CVerticalScalePtrDataList::SetScaleColor(COLORREF* pGridColor, COLORREF* pTextColor)
{
	m_pScaleGridColor = pGridColor;
	m_pScaleTextColor = pTextColor;
}

void CVerticalScalePtrDataList::SetScaleColor(const COLORREF& gridColor, const COLORREF& textColor)
{
	if(m_pScaleGridColor != NULL)
		*m_pScaleGridColor = gridColor;
	if(m_pScaleTextColor != NULL)
		*m_pScaleTextColor = textColor;
}

void CVerticalScalePtrDataList::SetEnableLogType(const bool bIsEnable)
{
	m_bEnableLogType = bIsEnable;
}

void CVerticalScalePtrDataList::SetEnableInvertType(const bool bIsEnable)
{
	m_bEnableInvertType = bIsEnable;
}

void CVerticalScalePtrDataList::SetScaleType(bool* pIsLog, bool* pIsInvert)
{
	m_pbLogType = pIsLog;
	m_pbInvertType = pIsInvert;
}

void CVerticalScalePtrDataList::SetScaleType(const bool bIsLog, const bool bIsInvert)
{
	if(m_pbLogType != NULL)
		*m_pbLogType = bIsLog;
	if(m_pbInvertType != NULL)
		*m_pbInvertType = bIsInvert;
}

void CVerticalScalePtrDataList::SetVertScale(const int nIndex, const CVerticalScale& vertScale)
{
	CVerticalScale* pVertScale = GetAt(nIndex).GetVertScale();
	if(pVertScale == NULL)
		return;

	pVertScale->SetVertScalePosition(vertScale.GetVertScalePosition());
	pVertScale->SetGridType(vertScale.GetGridType());
	pVertScale->SetVertScaleUnitType(vertScale.GetVertScaleUnitType());
}

// ----------------------------------------------------------------------------
int CVerticalScalePtrDataList::GetFirstIndex() const
{
	return m_nFirstIndex;
}

COLORREF* CVerticalScalePtrDataList::GetScaleGridColor() const
{
	return m_pScaleGridColor;
}

COLORREF* CVerticalScalePtrDataList::GetScaleTextColor() const
{
	return m_pScaleTextColor;
}

bool CVerticalScalePtrDataList::IsEnableLogType() const
{
	return m_bEnableLogType;
}

bool CVerticalScalePtrDataList::IsEnableInvertType() const
{
	return m_bEnableInvertType;
}

bool* CVerticalScalePtrDataList::GetScaleLogType() const
{
	return m_pbLogType;
}

bool* CVerticalScalePtrDataList::GetScaleInvertType() const
{
	return m_pbInvertType;
}

int CVerticalScalePtrDataList::GetCount() const
{
	return m_vertScalePtrDataList.GetCount();
}

CVerticalScalePtrData& CVerticalScalePtrDataList::GetAt(const int nIndex)
{
	POSITION pos = m_vertScalePtrDataList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_vertScalePtrDataList.GetAt(pos);
}

CVerticalScalePtrData CVerticalScalePtrDataList::GetAt(const int nIndex) const
{
	POSITION pos = m_vertScalePtrDataList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_vertScalePtrDataList.GetAt(pos);
}


///////////////////////////////////////////////////////////////////////////////
//class CVerticalScaleDataList

CVerticalScaleDataList& CVerticalScaleDataList::operator=(const CVerticalScaleDataList& vertScaleDataList)
{
	if(this == &vertScaleDataList)
		return *this;

	m_scaleGridColor = vertScaleDataList.GetScaleGridColor();
	m_scaleTextColor = vertScaleDataList.GetScaleTextColor();
	m_bLogType = vertScaleDataList.IsScaleLogType();
	m_bInvertType = vertScaleDataList.IsScaleInvertType();
	for(int nIndex = 0; nIndex < vertScaleDataList.GetCount(); nIndex++){
		m_vertScaleList.AddTail(vertScaleDataList.GetAt(nIndex));
	}

	return *this;
}

CVerticalScaleDataList::CVerticalScaleDataList():
	m_scaleGridColor(RGB(202, 202, 202)),
	m_scaleTextColor(RGB(0, 0, 0)),
	m_bLogType(false),
	m_bInvertType(false)
{
}

CVerticalScaleDataList::CVerticalScaleDataList(const CVerticalScaleDataList& vertScaleDataList)
{
	m_scaleGridColor = vertScaleDataList.GetScaleGridColor();
	m_scaleTextColor = vertScaleDataList.GetScaleTextColor();
	m_bLogType = vertScaleDataList.IsScaleLogType();
	m_bInvertType = vertScaleDataList.IsScaleInvertType();
	for(int nIndex = 0; nIndex < vertScaleDataList.GetCount(); nIndex++){
		m_vertScaleList.AddTail(vertScaleDataList.GetAt(nIndex));
	}
}

CVerticalScaleDataList::CVerticalScaleDataList(const CVerticalScalePtrDataList* pVertScalePtrDataList)
{
	if(pVertScalePtrDataList == NULL)
		return;

	m_scaleGridColor = *pVertScalePtrDataList->GetScaleGridColor();
	m_scaleTextColor = *pVertScalePtrDataList->GetScaleTextColor();
	m_bLogType = *pVertScalePtrDataList->GetScaleLogType();
	m_bInvertType = *pVertScalePtrDataList->GetScaleInvertType();
	for(int nIndex = 0; nIndex < pVertScalePtrDataList->GetCount(); nIndex++){
		m_vertScaleList.AddTail(*pVertScalePtrDataList->GetAt(nIndex).GetVertScale());
	}
}


// public =====================================================================
void CVerticalScaleDataList::AddTail(CVerticalScale vertScale)
{
	m_vertScaleList.AddTail(vertScale);
}

void CVerticalScaleDataList::RemoveAll()
{
	m_vertScaleList.RemoveAll();
}

// ----------------------------------------------------------------------------
COLORREF CVerticalScaleDataList::GetScaleGridColor() const
{
	return m_scaleGridColor;
}

COLORREF CVerticalScaleDataList::GetScaleTextColor() const
{
	return m_scaleTextColor;
}

int CVerticalScaleDataList::GetCount() const
{
	return m_vertScaleList.GetCount();
}

bool CVerticalScaleDataList::IsScaleLogType() const
{
	return m_bLogType;
}

bool CVerticalScaleDataList::IsScaleInvertType() const
{
	return m_bInvertType;
}

CVerticalScale& CVerticalScaleDataList::GetAt(const int nIndex)
{
	POSITION pos = m_vertScaleList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_vertScaleList.GetAt(pos);
}

CVerticalScale CVerticalScaleDataList::GetAt(const int nIndex) const
{
	POSITION pos = m_vertScaleList.FindIndex(nIndex);
	assert(pos != NULL);
	return m_vertScaleList.GetAt(pos);
}
