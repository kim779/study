// ChartItemData.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CBaseValue
//////////////////////////////////////////////////////////////////////
CBaseValue::CBaseValue()
{
}

CBaseValue::~CBaseValue()
{
}

//////////////////////////////////////////////////////////////////////
// CCharValue
//////////////////////////////////////////////////////////////////////
CCharValue::CCharValue()
{
	m_nSize = 0;
	m_pszValue = NULL;
}

CCharValue::CCharValue(CCharValue *pItem)
{
	memcpy(m_pszValue, pItem->m_pszValue, pItem->m_nSize);
}

CCharValue::~CCharValue()
{
	if (m_pszValue)
	{
		delete [] m_pszValue;
		m_pszValue = NULL;

		m_nSize = 0;
	}
}

void CCharValue::Copy(const CCharValue& item)
{
	memcpy(m_pszValue, item.m_pszValue, item.m_nSize);
}

CCharValue& CCharValue::operator = (const CCharValue& item)
{
	ResetValue(item.m_nSize);
	Copy(item);

	return *this;
}

void CCharValue::ResetValue(int nSize)
{
	m_nSize = nSize;
	if (m_pszValue)
	{
		delete [] m_pszValue;
		m_pszValue = NULL;
	}

	if (m_nSize <= 0)
		return;

	m_pszValue = new char[m_nSize+1];
	memset(m_pszValue, 0x00, m_nSize+1);
}

void CCharValue::SetValue(void *pValue, int nSize)
{
	if (nSize <= 0)
		return;

	ResetValue(nSize);

	memcpy(m_pszValue, pValue, nSize);
}

int CCharValue::GetValue(void **pValue)
{
	*pValue = m_pszValue;
	return m_nSize;
}

void CCharValue::SetstrValue(CString strData)
{
	sprintf(m_pszValue, _T("%*.*s"), m_nSize, m_nSize, (LPCSTR)(LPCTSTR)strData);
}

CString CCharValue::GetstrValue()
{
	if (m_pszValue == NULL)
		return _T("");

	CString strData;
	strData.Format(_T("%s"), m_pszValue);

	return strData;
}

void CCharValue::SetbValue(BOOL bValue)
{
	if (m_nSize <= 0)
		return;

	sprintf(m_pszValue, _T("%d"), (int)bValue);
}

BOOL CCharValue::GetbValue()
{
	if (m_pszValue == NULL)
		return FALSE;

	return (BOOL)atoi(m_pszValue);
}

void CCharValue::SetnValue(int nValue)
{
	if (m_nSize <= 0)
		return;

	sprintf(m_pszValue, _T("%d"), nValue);
}

int CCharValue::GetnValue()
{
	if (m_pszValue == NULL)
		return 0;

	return atoi(m_pszValue);
}

void CCharValue::SetlValue(long lValue)
{
	if (m_nSize <= 0)
		return;

	sprintf(m_pszValue, _T("%*.*ld"), m_nSize, m_nSize, lValue);
}

long CCharValue::GetlValue()
{
	if (m_pszValue == NULL)
		return 0;

	return atol(m_pszValue);
}

void CCharValue::SetdValue(double dValue)
{
	if (m_nSize <= 0)
		return;

	sprintf(m_pszValue, _T("%*.*f"), m_nSize, m_nSize, dValue);
}

double CCharValue::GetdValue()
{
	if (m_pszValue == NULL)
		return 0;

	return atof(m_pszValue);
}

//////////////////////////////////////////////////////////////////////
// CStringValue
//////////////////////////////////////////////////////////////////////
CStringValue::CStringValue()
{
	m_strValue = _T("");
}

CStringValue::CStringValue(CStringValue *pItem)
{
	m_strValue = pItem->m_strValue;
}

CStringValue::~CStringValue()
{
}

void CStringValue::Copy(const CStringValue& item)
{
	m_strValue = item.m_strValue;
}

CStringValue& CStringValue::operator = (const CStringValue& item)
{
	Copy(item);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// CIntValue
//////////////////////////////////////////////////////////////////////
CIntValue::CIntValue()
{
	m_nValue = 0;
}

CIntValue::CIntValue(CIntValue *pItem)
{
	m_nValue = pItem->m_nValue;
}

CIntValue::~CIntValue()
{
}

void CIntValue::Copy(const CIntValue& item)
{
	m_nValue = item.m_nValue;
}

CIntValue& CIntValue::operator = (const CIntValue& item)
{
	Copy(item);

	return *this;
}


//////////////////////////////////////////////////////////////////////
// CBoolValue
//////////////////////////////////////////////////////////////////////
CBoolValue::CBoolValue()
{
	m_bValue = TRUE;
}

CBoolValue::CBoolValue(CBoolValue *pItem)
{
	m_bValue = pItem->m_bValue;
}

CBoolValue::~CBoolValue()
{
}

void CBoolValue::Copy(const CBoolValue& item)
{
	m_bValue = item.m_bValue;
}

CBoolValue& CBoolValue::operator = (const CBoolValue& item)
{
	Copy(item);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// CLongValue
//////////////////////////////////////////////////////////////////////
CLongValue::CLongValue()
{
	m_lValue = 0;
}

CLongValue::CLongValue(CLongValue *pItem)
{
	m_lValue = pItem->m_lValue;
}

CLongValue::~CLongValue()
{
}

void CLongValue::Copy(const CLongValue& item)
{
	m_lValue = item.m_lValue;
}

CLongValue& CLongValue::operator = (const CLongValue& item)
{
	Copy(item);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// CDoubleValue
//////////////////////////////////////////////////////////////////////
CDoubleValue::CDoubleValue()
{
	m_dValue = 0;
}

CDoubleValue::CDoubleValue(CDoubleValue *pItem)
{
	m_dValue = pItem->m_dValue;
}

CDoubleValue::~CDoubleValue()
{
}

void CDoubleValue::Copy(const CDoubleValue& item)
{
	m_dValue = item.m_dValue;
}

CDoubleValue& CDoubleValue::operator = (const CDoubleValue& item)
{
	Copy(item);

	return *this;
}
