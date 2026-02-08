// ItemInChart.cpp : 구현 파일입니다.
//

#include "stdafx.h"
//#include "CtrlChart.h"
#include "ItemInChart.h"
#include ".\iteminchart.h"


// CItemInChart

CItemInChart::CItemInChart()
: m_strName(_T(""))
, m_strField(_T(""))
, m_strType(_T(""))
, m_bValid(false)
, m_lLength(0)
, m_strFormat(_T(""))
, m_bReal(FALSE)
, m_nFID(-1)
, m_nGID(0)
, m_nIndex(-1)
, m_sExponent(0)
, m_strRecord(_T(""))
{
}

CItemInChart::~CItemInChart()
{
}

CItemInChartEu::CItemInChartEu()
{
	m_nRealID = -1;
}