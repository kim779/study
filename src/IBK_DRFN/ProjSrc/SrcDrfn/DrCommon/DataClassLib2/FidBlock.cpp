// FidBlock.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FidBlock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CFidItem, CObject)
IMPLEMENT_DYNAMIC(CFidItemArr, CObject)
IMPLEMENT_DYNAMIC(CFidBlock, CObject)

//////////////////////////////////////////////////////////////////////
// CFidItem
//////////////////////////////////////////////////////////////////////
CFidItem::CFidItem()
{
	m_strValue = _T("");
	m_nSize = 0;
	m_strItemID = _T("");
	m_strRealID = _T("");
	m_strChartID = _T("");

	m_bUseBuf = TRUE;
}

CFidItem::CFidItem(CFidItem *pItem)
{
	Copy((CFidItem&)*pItem);
}

CFidItem::~CFidItem()
{
	RemoveAllData();
}

void CFidItem::RemoveAllData()
{
	m_strValue = _T("");
}

void CFidItem::Copy(const CFidItem& item)
{
	m_strValue		= item.m_strValue;
	m_nSize			= item.m_nSize;
	m_bUseBuf		= item.m_bUseBuf;
	m_strItemID		= item.m_strItemID;
	m_strRealID		= item.m_strRealID;
	m_strChartID	= item.m_strChartID;
}

CFidItem& CFidItem::operator = (const CFidItem& item)
{
	Copy(item);

	return *this;
}

void CFidItem::Serialize(CArchive &ar, int nVer)
{
	if (ar.IsStoring())
	{
		ar << m_nSize;
		ar << m_strItemID;
		ar << m_strRealID;
		ar << m_strChartID;
	}
	else
	{
		ar >> m_nSize;
		ar >> m_strItemID;
		ar >> m_strRealID;
		ar >> m_strChartID;
	}
}

void CFidItem::SetValue(CString strData, int nIndex)
{
	m_strValue = strData;
}

CString CFidItem::GetValue(int nIndex)
{
	return m_strValue;
}

//////////////////////////////////////////////////////////////////////
// CFidItemArr
//////////////////////////////////////////////////////////////////////
CFidItemArr::CFidItemArr()
{
}

CFidItemArr::~CFidItemArr()
{
	RemoveAllData();
}

void CFidItemArr::Copy(const CFidItemArr& item)
{
	RemoveAllData();

	CFidItem::Copy(item);

	m_arrValue.Copy(item.m_arrValue);
}

void CFidItemArr::RemoveAllData()
{
	CFidItem::RemoveAllData();
	m_arrValue.RemoveAll();
}

CFidItemArr& CFidItemArr::operator = (const CFidItemArr& item)
{
	Copy(item);

	return *this;
}

void CFidItemArr::SetValue(CString strData, int nIndex)
{
	if ((nIndex < 0) || (m_arrValue.GetSize() <= nIndex))
		m_arrValue.Add(strData);
	else
	{
		m_arrValue.RemoveAt(nIndex);
		m_arrValue.InsertAt(nIndex, strData);
	}
}

CString CFidItemArr::GetValue(int nIndex)
{
	if (nIndex < 0)
		nIndex = 0;

	if (m_arrValue.GetSize() <= nIndex)
		return _T("");

	return m_arrValue.GetAt(nIndex);
}

//////////////////////////////////////////////////////////////////////
// CFidBlock
//////////////////////////////////////////////////////////////////////
CFidBlock::CFidBlock()
{
	m_nReqRow = 200;
}

CFidBlock::~CFidBlock()
{
	RemoveAll();
}

void CFidBlock::Copy(const CFidBlock& item)
{
	RemoveAll();

	m_bArray		= item.m_bArray;
	m_strBlockName	= item.m_strBlockName;
	m_strCntItemID	= item.m_strCntItemID;
	m_nReqRow		= item.m_nReqRow;

	int nSize = item.m_arrItem.GetSize();

	CFidItem *pItem = NULL, *pNewItem = NULL;
	for (int i=0; i<nSize; i++)
	{
		pItem = item.m_arrItem.GetAt(i);
		if (pItem)
		{
			if (m_bArray)	pNewItem = new CFidItemArr;
			else			pNewItem = new CFidItem;

			pNewItem->Copy(*pItem);
			m_arrItem.Add(pNewItem);
		}
	}
}

void CFidBlock::RemoveAll()
{
	int nSize = m_arrItem.GetSize();

	CFidItem *pItem = NULL;
	for (int i=0; i<nSize; i++)
	{
		pItem = m_arrItem.GetAt(i);
		if (!pItem)
			continue;

		if (IsArray())
		{
			delete (CFidItemArr*)pItem;
			pItem = NULL;
		}
		else
		{
			delete pItem;
			pItem = NULL;
		}
	}
}

CFidItem* CFidBlock::GetItemNameAt(CString strID)
{
	int nSize = m_arrItem.GetSize();

	CFidItem *pItem = NULL;
	for (int i=0; i<nSize; i++)
	{
		pItem = m_arrItem.GetAt(i);
		if (pItem)
		{
			if (strID.Compare(pItem->GetChartID()) == 0)
				return pItem;
		}
	}

	return NULL;
}

CFidItem* CFidBlock::GetItemAt(CString strID)
{
	int nSize = m_arrItem.GetSize();

	CFidItem *pItem = NULL;
	for (int i=0; i<nSize; i++)
	{
		pItem = m_arrItem.GetAt(i);
		if (pItem)
		{
			if (strID.Compare(pItem->GetItemID()) == 0)
				return pItem;
		}
	}

	return NULL;
}

CFidBlock& CFidBlock::operator = (const CFidBlock& item)
{
	Copy(item);

	return *this;
}

void CFidBlock::Serialize(CArchive &ar, int nVer)
{
	CFidItem *pItem = NULL;
	int nSize = 0;
	if (ar.IsStoring())
	{
		nSize = m_arrItem.GetSize();

		ar << m_strBlockName;
		ar << m_strCntItemID;
		ar << m_bArray;
		ar << nSize;
		ar << m_nReqRow;

		for (int i=0; i<nSize; i++)
		{
			pItem = m_arrItem.GetAt(i);
			pItem->Serialize(ar, nVer);
		}
	}
	else
	{
		ar >> m_strBlockName;
		ar >> m_strCntItemID;
		ar >> m_bArray;
		ar >> nSize;

		if (nVer > 1)
			ar >> m_nReqRow;

		for (int i=0; i<nSize; i++)
		{
			if (m_bArray)	pItem = new CFidItemArr;
			else			pItem = new CFidItem;

			pItem->Serialize(ar, nVer);

			m_arrItem.Add(pItem);
		}
	}
}

