// ChartItemData.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CTrItem, CObject)
IMPLEMENT_DYNAMIC(CTrItemArr, CObject)
IMPLEMENT_DYNAMIC(CTrBlock, CObject)

//////////////////////////////////////////////////////////////////////
// CTrItem
//////////////////////////////////////////////////////////////////////
CTrItem::CTrItem()
{
	m_pValue = NULL;
	m_bByteOrd = FALSE;
	m_nSize = 0;
	m_uDataType = VALUE_TYPE_CHAR;
	m_strItemID = _T("");
	m_strRealID = _T("");
	m_strChartID = _T("");
	m_bABS = FALSE;

	m_bUseBuf = TRUE;
}

CTrItem::CTrItem(CTrItem *pItem)
{
	Copy((CTrItem&)*pItem);
}

CTrItem::~CTrItem()
{
	RemoveAllData();
}

void CTrItem::RemoveAllData()
{
	if (m_pValue)
	{
		delete m_pValue;
		m_pValue = NULL;
	}
}

void CTrItem::Copy(const CTrItem& item)
{
	if (m_pValue)
	{
		delete m_pValue;
		m_pValue = NULL;
	}

	if (item.m_pValue)
	{
		m_pValue = CreateValue(item.m_uDataType);
		CopyValue(item.m_uDataType, m_pValue, item.m_pValue);
	}

	m_bByteOrd		= item.m_bByteOrd;
	m_nSize			= item.m_nSize;
	m_uDataType		= item.m_uDataType;
	m_strItemID		= item.m_strItemID;
	m_strRealID		= item.m_strRealID;
	m_strChartID	= item.m_strChartID;
	m_bABS			= item.m_bABS;
}

CTrItem& CTrItem::operator = (const CTrItem& item)
{
	Copy(item);

	return *this;
}

CBaseValue* CTrItem::CreateValue(BYTE ucType)
{
	CBaseValue* pValue = NULL;
	switch (ucType)
	{
	case VALUE_TYPE_INT:
		pValue = new CIntValue;
		break;
	case VALUE_TYPE_BOOL:
		pValue = new CBoolValue;
		break;
	case VALUE_TYPE_LONG:
		pValue = new CLongValue;
		break;
	case VALUE_TYPE_DOUBLE:
		pValue = new CDoubleValue;
		break;
	case VALUE_TYPE_STRING:
		pValue = new CStringValue;
		break;
	case VALUE_TYPE_CHAR:
		pValue = new CCharValue;
		break;
	}

	return pValue;
}

void CTrItem::CopyValue(BYTE ucType, CBaseValue *pToItem, CBaseValue *pFromItem)
{
	if (ucType == VALUE_TYPE_INT)
		((CIntValue*)pToItem)->Copy((CIntValue&)*pFromItem);
	else if (ucType == VALUE_TYPE_BOOL)
		((CBoolValue*)pToItem)->Copy((CBoolValue&)*pFromItem);
	else if (ucType == VALUE_TYPE_LONG)
		((CLongValue*)pToItem)->Copy((CLongValue&)*pFromItem);
	else if (ucType == VALUE_TYPE_DOUBLE)
		((CDoubleValue*)pToItem)->Copy((CDoubleValue&)*pFromItem);
	else if (ucType == VALUE_TYPE_STRING)
		((CStringValue*)pToItem)->Copy((CStringValue&)*pFromItem);
	else if (ucType == VALUE_TYPE_CHAR)
		((CCharValue*)pToItem)->Copy((CCharValue&)*pFromItem);
}

int CTrItem::SetDataFromBuffer(char* pszData, int nIndex)
{
	if (m_pValue == NULL)
		m_pValue = CreateValue(m_uDataType);

	m_pValue->SetValue((void*)(pszData+nIndex), (int)m_nSize);

	return (int)(m_nSize);
}

void CTrItem::SetDataFromFile(CFile *pFile)
{
	int uSize = 0;
	char szBuf[1024];
	memset(szBuf, 0x00, sizeof(szBuf));
	if (m_pValue == NULL)
		m_pValue = CreateValue(m_uDataType);

	uSize = pFile->Read(szBuf, m_nSize);
	m_pValue->SetValue((void*)szBuf, (int)uSize);
}

void CTrItem::Serialize(CArchive &ar, int nVer)
{
	if (ar.IsStoring())
	{
		ar << m_bByteOrd;
		ar << m_nSize;
		ar << m_strItemID;
		ar << m_uDataType;
		ar << m_strRealID;
		ar << m_strChartID;
		ar << m_bABS;
	}
	else
	{
		ar >> m_bByteOrd;
		ar >> m_nSize;
		ar >> m_strItemID;
		ar >> m_uDataType;
		if (nVer >= 2)
		{
			ar >> m_strRealID;
			ar >> m_strChartID;
		}

		if (nVer >= 3)
			ar >> m_bABS;
	}
}

void CTrItem::SetDataType(BYTE uDataType)
{
	m_uDataType = uDataType;

	if (m_uDataType == VALUE_TYPE_INT)			m_nSize = sizeof(int);
	else if (m_uDataType == VALUE_TYPE_BOOL)	m_nSize = sizeof(BOOL);
	else if (m_uDataType == VALUE_TYPE_LONG)	m_nSize = sizeof(long);
	else if (m_uDataType == VALUE_TYPE_DOUBLE)	m_nSize = sizeof(double);
}

void CTrItem::SetSize(int nSize)
{
	if ((m_uDataType == VALUE_TYPE_STRING) || (m_uDataType == VALUE_TYPE_CHAR))
		m_nSize = nSize;
}

void CTrItem::SetValue(void *pValue, int nIndex)
{
	if (pValue == NULL)	return;

	if (m_pValue == NULL)
		m_pValue = CreateValue(m_uDataType);

	m_pValue->SetValue(pValue, m_nSize);
}

int CTrItem::GetValue(void **pValue, int nIndex)
{
	return m_pValue->GetValue(pValue);
}

void CTrItem::SetstrValue(CString strData, int nIndex)
{
	if (m_pValue == NULL)
		m_pValue = CreateValue(m_uDataType);

	m_pValue->SetstrValue(strData);
}

CString CTrItem::GetstrValue(int nIndex)
{
	if (m_pValue == NULL)	return _T("");

	return m_pValue->GetstrValue();
}

void CTrItem::SetbValue(BOOL bValue, int nIndex)
{
	if (m_pValue == NULL)
		m_pValue = CreateValue(m_uDataType);

	m_pValue->SetbValue(bValue);
}

BOOL CTrItem::GetbValue(int nIndex)
{
	if (m_pValue == NULL)	return FALSE;

	return m_pValue->GetbValue();
}

void CTrItem::SetnValue(int nValue, int nIndex)
{
	if (m_pValue == NULL)
		m_pValue = CreateValue(m_uDataType);

	m_pValue->SetnValue(nValue);
}

int CTrItem::GetnValue(int nIndex)
{
	if (m_pValue == NULL)	return 0;

	return m_pValue->GetnValue();
}

void CTrItem::SetlValue(long lValue, int nIndex)
{
	if (m_pValue == NULL)
		m_pValue = CreateValue(m_uDataType);

	m_pValue->SetlValue(lValue);
}

long CTrItem::GetlValue(int nIndex)
{
	if (m_pValue == NULL)	return 0;

	return m_pValue->GetlValue();
}

void CTrItem::SetdValue(double dValue, int nIndex)
{
	if (m_pValue == NULL)
		m_pValue = CreateValue(m_uDataType);

	m_pValue->SetdValue(dValue);
}

double CTrItem::GetdValue(int nIndex)
{
	if (m_pValue == NULL)	return 0;

	return m_pValue->GetdValue();
}

//////////////////////////////////////////////////////////////////////
// CTrItemArr
//////////////////////////////////////////////////////////////////////
CTrItemArr::CTrItemArr()
{
	m_pValue = NULL;
}

CTrItemArr::~CTrItemArr()
{
	if (m_pValue)
	{
		delete m_pValue;
		m_pValue = NULL;
	}

	RemoveAllData();
}

void CTrItemArr::Copy(const CTrItemArr& item)
{
	CTrItem::Copy(item);

	RemoveAllData();

	CBaseValue *pValue = NULL, *pNewItem = NULL;
	int nSize = item.m_arrValue.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pValue = item.m_arrValue.GetAt(i);
		if (pValue == NULL)
			continue;

		pNewItem = CreateValue(item.m_uDataType);
		CopyValue(item.m_uDataType, pNewItem, pValue);

		m_arrValue.Add(pNewItem);
	}
}

void CTrItemArr::RemoveAllData()
{
	CTrItem::RemoveAllData();

	CBaseValue* pDel = NULL;
	int nSize = m_arrValue.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pDel = m_arrValue.GetAt(i);
		if (pDel)
		{
			delete pDel;
			pDel = NULL;
		}
	}

	m_arrValue.RemoveAll();
}

CTrItemArr& CTrItemArr::operator = (const CTrItemArr& item)
{
	Copy(item);

	return *this;
}

int CTrItemArr::SetDataFromBuffer(char* pszData, int nIndex)
{
	CBaseValue *pValue = NULL;
	pValue = CreateValue(m_uDataType);

	pValue->SetValue((void*)(pszData+nIndex), (int)m_nSize);

	if (pValue)
		m_arrValue.Add(pValue);

	//090916-alzioyes.
	//return (int)(nIndex+m_nSize);
	return (int)(m_nSize);
}

int CTrItemArr::SetDataFromData(char* pszData, int nIndex, CString strData)
{
	CBaseValue *pValue = NULL;
	pValue = CreateValue(m_uDataType);
	
	pValue->SetValue((void*)(LPCTSTR)strData, strData.GetLength());
	
	if (pValue)
		m_arrValue.Add(pValue);	

	return (int)(m_nSize);
}

void CTrItemArr::SetDataFromFile(CFile *pFile)
{
	CBaseValue *pValue = NULL;
	int uSize = 0;
	char szBuf[1024];
	memset(szBuf, 0x00, sizeof(szBuf));
	pValue = CreateValue(m_uDataType);

	if (m_uDataType == VALUE_TYPE_INT)
		uSize = pFile->Read(szBuf, sizeof(int));
	else if (m_uDataType == VALUE_TYPE_BOOL)
		uSize = pFile->Read(szBuf, sizeof(BOOL));
	else if (m_uDataType == VALUE_TYPE_LONG)
		uSize = pFile->Read(szBuf, sizeof(long));
	else if (m_uDataType == VALUE_TYPE_DOUBLE)
		uSize = pFile->Read(szBuf, sizeof(double));
//	else if (m_uDataType == VALUE_TYPE_STRING)
//		uSize = pFile->Read(szBuf, sizeof(long));
	else if (m_uDataType == VALUE_TYPE_CHAR)
		uSize = pFile->Read(szBuf, m_nSize);

	pValue->SetValue((void*)szBuf, (int)uSize);

	if (pValue)
		m_arrValue.Add(pValue);
}

void CTrItemArr::SetValue(void *pValue, int nIndex)
{
	if (pValue == NULL)	return;

	CBaseValue *pItemVal = NULL;
	if ((nIndex < 0) || (m_arrValue.GetSize() <= nIndex))
	{
		pItemVal = CreateValue(m_uDataType);
		pItemVal->SetValue(pValue, m_nSize);
		m_arrValue.Add(pItemVal);
	}
	else
	{
		pItemVal = m_arrValue.GetAt(nIndex);
		pItemVal->SetValue(pValue, m_nSize);
	}
}

int CTrItemArr::GetValue(void **pValue, int nIndex)
{
	if (nIndex < 0)		nIndex = 0;

	if (m_arrValue.GetSize() <= nIndex)
	{
		pValue = NULL;
		return 0;
	}

	CBaseValue* pVal = m_arrValue.GetAt(nIndex);
	if (!pVal)
	{
		pValue = NULL;
		return 0;
	}

	return pVal->GetValue(pValue);
}

void CTrItemArr::SetstrValue(CString strData, int nIndex)
{
	CBaseValue *pItemVal = NULL;
	if ((nIndex < 0) || (m_arrValue.GetSize() <= nIndex))
	{
		pItemVal = CreateValue(m_uDataType);
		pItemVal->SetstrValue(strData);
		m_arrValue.Add(pItemVal);
	}
	else
	{
		pItemVal = m_arrValue.GetAt(nIndex);
		pItemVal->SetstrValue(strData);
	}
}

CString CTrItemArr::GetstrValue(int nIndex)
{
	if (nIndex < 0)		nIndex = 0;

	if (m_arrValue.GetSize() <= nIndex)
		return _T("");

	CBaseValue* pVal = m_arrValue.GetAt(nIndex);
	if (!pVal)	return _T("");

	return pVal->GetstrValue();
}

void CTrItemArr::SetbValue(BOOL bValue, int nIndex)
{
	CBaseValue *pItemVal = NULL;
	if ((nIndex < 0) || (m_arrValue.GetSize() <= nIndex))
	{
		pItemVal = CreateValue(m_uDataType);
		pItemVal->SetbValue(bValue);
		m_arrValue.Add(pItemVal);
	}
	else
	{
		pItemVal = m_arrValue.GetAt(nIndex);
		pItemVal->SetbValue(bValue);
	}
}

BOOL CTrItemArr::GetbValue(int nIndex)
{
	if (nIndex < 0)		nIndex = 0;

	if (m_arrValue.GetSize() <= nIndex)
		return FALSE;

	CBaseValue* pVal = m_arrValue.GetAt(nIndex);
	if (!pVal)	return FALSE;

	return pVal->GetbValue();
}

void CTrItemArr::SetnValue(int nValue, int nIndex)
{
	CBaseValue *pItemVal = NULL;
	if ((nIndex < 0) || (m_arrValue.GetSize() <= nIndex))
	{
		pItemVal = CreateValue(m_uDataType);
		pItemVal->SetnValue(nValue);
		m_arrValue.Add(pItemVal);
	}
	else
	{
		pItemVal = m_arrValue.GetAt(nIndex);
		pItemVal->SetnValue(nValue);
	}
}

int CTrItemArr::GetnValue(int nIndex)
{
	if (nIndex < 0)		nIndex = 0;

	if (m_arrValue.GetSize() <= nIndex)
		return FALSE;

	CBaseValue* pVal = m_arrValue.GetAt(nIndex);
	if (!pVal)	return 0;

	return pVal->GetnValue();
}

void CTrItemArr::SetlValue(long lValue, int nIndex)
{
	CBaseValue *pItemVal = NULL;
	if ((nIndex < 0) || (m_arrValue.GetSize() <= nIndex))
	{
		pItemVal = CreateValue(m_uDataType);
		pItemVal->SetlValue(lValue);
		m_arrValue.Add(pItemVal);
	}
	else
	{
		pItemVal = m_arrValue.GetAt(nIndex);
		pItemVal->SetlValue(lValue);
	}
}

long CTrItemArr::GetlValue(int nIndex)
{
	if (nIndex < 0)		nIndex = 0;

	if (m_arrValue.GetSize() <= nIndex)
		return FALSE;

	CBaseValue* pVal = m_arrValue.GetAt(nIndex);
	if (!pVal)	return 0;

	return pVal->GetlValue();
}

void CTrItemArr::SetdValue(double dValue, int nIndex)
{
	CBaseValue *pItemVal = NULL;
	if ((nIndex < 0) || (m_arrValue.GetSize() <= nIndex))
	{
		pItemVal = CreateValue(m_uDataType);
		pItemVal->SetdValue(dValue);
		m_arrValue.Add(pItemVal);
	}
	else
	{
		pItemVal = m_arrValue.GetAt(nIndex);
		pItemVal->SetdValue(dValue);
	}
}

double CTrItemArr::GetdValue(int nIndex)
{
	if (nIndex < 0)		nIndex = 0;

	if (m_arrValue.GetSize() <= nIndex)
		return FALSE;

	CBaseValue* pVal = m_arrValue.GetAt(nIndex);
	if (!pVal)	return 0;

	return pVal->GetdValue();
}

//////////////////////////////////////////////////////////////////////
// CTrBlock
//////////////////////////////////////////////////////////////////////
CTrBlock::CTrBlock()
{
}

CTrBlock::~CTrBlock()
{
	RemoveAll();
}

void CTrBlock::Copy(const CTrBlock& item)
{
	RemoveAll();

	m_bArray		= item.m_bArray;
	m_strBlockName	= item.m_strBlockName;
	m_strCntItemID	= item.m_strCntItemID;

	int nSize = item.m_TrItemArr.GetSize();

	CTrItem *pItem = NULL, *pNewItem = NULL;
	for (int i=0; i<nSize; i++)
	{
		pItem = item.m_TrItemArr.GetAt(i);
		if (pItem)
		{
			if (m_bArray)	pNewItem = new CTrItemArr;
			else			pNewItem = new CTrItem;

			pNewItem->Copy(*pItem);
			m_TrItemArr.Add(pNewItem);
		}
	}
}

void CTrBlock::RemoveAll()
{
	int nSize = m_TrItemArr.GetSize();

	CTrItem *pItem = NULL;
	for (int i=0; i<nSize; i++)
	{
		pItem = m_TrItemArr.GetAt(i);
		if (pItem)
		{
			if (IsArray())
			{
				delete (CTrItemArr*)pItem;
				pItem = NULL;
			}
			else
			{
				delete pItem;
				pItem = NULL;
			}
		}
	}
}

CTrItem* CTrBlock::GetItemAt(CString strID)
{
	int nSize = m_TrItemArr.GetSize();

	CTrItem *pItem = NULL;
	for (int i=0; i<nSize; i++)
	{
		pItem = m_TrItemArr.GetAt(i);
		if (pItem)
		{
			if (strID.Compare(pItem->GetItemID()) == 0)
				return pItem;
		}
	}

	return NULL;
}

CTrBlock& CTrBlock::operator = (const CTrBlock& item)
{
	Copy(item);

	return *this;
}

void CTrBlock::Serialize(CArchive &ar, int nVer)
{
	CTrItem *pItem = NULL;
	int nSize = 0;
	if (ar.IsStoring())
	{
		nSize = m_TrItemArr.GetSize();

		ar << m_strBlockName;
		ar << m_strCntItemID;
		ar << m_bArray;
		ar << nSize;

		for (int i=0; i<nSize; i++)
		{
			pItem = m_TrItemArr.GetAt(i);
			pItem->Serialize(ar, nVer);
		}
	}
	else
	{
		ar >> m_strBlockName;
		ar >> m_strCntItemID;
		ar >> m_bArray;
		ar >> nSize;

		for (int i=0; i<nSize; i++)
		{
			if (m_bArray)	pItem = new CTrItemArr;
			else			pItem = new CTrItem;

			pItem->Serialize(ar, nVer);
			pItem->SetIsArray(m_bArray);

			m_TrItemArr.Add(pItem);
		}
	}
}

