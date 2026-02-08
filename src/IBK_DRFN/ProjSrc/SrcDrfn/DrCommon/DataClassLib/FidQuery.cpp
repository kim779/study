// ChartItemData.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FidQuery.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CFidPack, CObject)
IMPLEMENT_DYNAMIC(CFidQuery, CObject)

//////////////////////////////////////////////////////////////////////
// CFidPack
//////////////////////////////////////////////////////////////////////
CFidPack::CFidPack()
{
	m_nVer = 2;

	m_nSortType = 0;
	m_bRealABS = FALSE;

	//-->@IMSI-alzioyes
	m_nTRDataCnt=0;	
	m_nOneArrSize = 0;
	//m_pTRData = NULL;
	//<--@IMSI-alzioyes
}

CFidPack::~CFidPack()
{
	RemoveAll();
}

void CFidPack::Copy(const CFidPack& item)
{
	RemoveAll();

	CFidBlock	*pItem = NULL, *pNewItem = NULL;
	int nSize = item.m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pItem = item.m_arrBlock.GetAt(i);
		if (pItem)
		{
			pNewItem = new CFidBlock;
			pNewItem->Copy((CFidBlock&)*pItem);
			m_arrBlock.Add(pNewItem);

			CFidItem *pItem = NULL;
			int nYSize = pNewItem->m_arrItem.GetSize();
			for (int j=0; j<nYSize; j++)
			{
				pItem = pNewItem->m_arrItem.GetAt(j);
				if (pItem)
				{
					m_arrItem.Add(pItem);
					m_mapItem.SetAt(pItem->GetItemID(), pItem);
					m_mapChartNameItem.SetAt(pItem->GetChartID(), pItem);
				}
			}
		}
	}

	m_bInput		= item.m_bInput;
	m_strID			= item.m_strID;
	m_bRealABS		= item.m_bRealABS;
	m_nSortType		= item.m_nSortType;
}

CFidPack& CFidPack::operator = (const CFidPack& item)
{
	Copy(item);

	return *this;
}

void CFidPack::Serialize(CArchive& ar)
{
	int i, nSize = 0;
	CFidBlock	*pBlock = NULL;
	if (ar.IsStoring())
	{
		m_nVer = 4;
		ar << m_nVer;
		ar << m_bInput;
		ar << m_strID;
		ar << m_bRealABS;
		ar << m_nSortType;

		nSize = m_arrBlock.GetSize();
		ar << nSize;

		for (i=0; i<nSize; i++)
		{
			pBlock = m_arrBlock.GetAt(i);
			if (pBlock)
				pBlock->Serialize(ar, m_nVer);
		}
	}
	else
	{

		ar >> m_nVer;
		ar >> m_bInput;
		ar >> m_strID;
		if (m_nVer > 3)
		{
			ar >> m_bRealABS;
			ar >> m_nSortType;
		}

		ar >> nSize;

		for (i=0; i<nSize; i++)
		{
			pBlock = new CFidBlock;
			pBlock->Serialize(ar, m_nVer);
			m_arrBlock.Add(pBlock);

			CFidItem *pItem = NULL;
			int nYSize = pBlock->m_arrItem.GetSize();
			for (int j=0; j<nYSize; j++)
			{
				pItem = pBlock->m_arrItem.GetAt(j);
				if (pItem)
				{
					m_arrItem.Add(pItem);
					m_mapItem.SetAt(pItem->GetItemID(), pItem);
					m_mapChartNameItem.SetAt(pItem->GetChartID(), pItem);
				}
			}
		}
	}
}

void CFidPack::RemoveAll()
{
	CFidBlock	*pItem = NULL;
	int nSize = m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pItem = m_arrBlock.GetAt(i);
		if (pItem)
		{
			delete pItem;
			pItem = NULL;
		}
	}

	m_arrBlock.RemoveAll();
	m_arrItem.RemoveAll();
	m_mapItem.RemoveAll();
	m_mapChartNameItem.RemoveAll();
}

void CFidPack::RemoveAllData()
{
	CFidItem *pItem = NULL;
	int nSize = m_arrItem.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pItem = m_arrItem.GetAt(i);
		if (pItem == NULL)
			continue;

		pItem->RemoveAllData();
	}
}

BOOL CFidPack::ResetKeydata()
{
	m_arrItem.RemoveAll();
	m_mapItem.RemoveAll();
	m_mapChartNameItem.RemoveAll();

	CFidBlock *pBlock = NULL;
	int nSize = m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pBlock = m_arrBlock.GetAt(i);

		CFidItem *pItem = NULL;
		int nYSize = pBlock->m_arrItem.GetSize();
		for (int j=0; j<nYSize; j++)
		{
			pItem = pBlock->m_arrItem.GetAt(j);
			if (pItem)
			{
				m_arrItem.Add(pItem);
				m_mapItem.SetAt(pItem->GetItemID(), pItem);
				m_mapChartNameItem.SetAt(pItem->GetChartID(), pItem);
			}
		}
	}

	return TRUE;
}

void CFidPack::ReceiveData(const char* szData, int nSize)
{
	RemoveAllData();
}

CFidItem* CFidPack::GetItemAt(int nIndex)
{
	if (m_arrItem.GetSize() <= nIndex)
		return NULL;

	return m_arrItem.GetAt(nIndex);
}

CString CFidPack::GetDataAt(CString szKey)
{
	CFidItem* pItem = NULL;
	m_mapItem.Lookup(szKey, pItem);
	if (pItem)
		return pItem->GetValue();

	return _T("");
}

CString CFidPack::GetDataAt(int nIndex)
{
	if (m_arrItem.GetSize() <= nIndex)
		return _T("");

	CFidItem* pItem = m_arrItem.GetAt(nIndex);
	if (pItem)
		return pItem->GetValue();

	return _T("");
}

CString CFidPack::GetCodeData(CString &strFID, int nIndex)
{
	CString strID;
	int nID;
	CFidItem* pItem = NULL;
	int nSize = m_arrItem.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pItem = m_arrItem.GetAt(i);
		if (pItem)
		{
			strID = pItem->GetItemID();
			nID = atoi(strID)%1000;
			if (nID != 301)
				continue;

			strFID = pItem->GetItemID();
			if (pItem->IsKindOf(RUNTIME_CLASS(CFidItemArr)))
				return ((CFidItemArr*)pItem)->GetValue(nIndex);
			else
				return pItem->GetValue();
		}
	}

	return _T("");
}

BOOL CFidPack::GetArrDataAt(CStringArray* pArr, CString szKey)
{
	if (pArr == NULL)
		return FALSE;

	CFidItem* pItem = NULL;
	m_mapItem.Lookup(szKey, pItem);
	if (pItem && pItem->IsKindOf(RUNTIME_CLASS(CFidItemArr)))
	{
		int nSize = ((CFidItemArr*)pItem)->GetValueCount();
		for (int i=0; i<nSize; i++)
			pArr->Add(pItem->GetValue(i));

		return TRUE;
	}

	return FALSE;
}

BOOL CFidPack::GetArrDataAt(CStringArray* pArr, int nIndex)
{
	if (pArr == NULL)
		return FALSE;

	if (m_arrItem.GetSize() <= nIndex)
		return FALSE;

	CFidItem* pItem = NULL;
	pItem = m_arrItem.GetAt(nIndex);
	if (pItem && pItem->IsKindOf(RUNTIME_CLASS(CFidItemArr)))
	{
		int nSize = ((CFidItemArr*)pItem)->GetValueCount();
		for (int i=0; i<nSize; i++)
			pArr->Add(pItem->GetValue(i));

		return TRUE;
	}

	return FALSE;
}

int CFidPack::GetDataBuffer(char *pszData, int nBufSize)
{
	int nIndex = 0, nSize = 0;
	void *pszTemp = NULL;
	CString strData;

	CFidBlock *pBkItem = NULL;
	CFidItem *pItem = NULL;
	int nCount = m_arrBlock.GetSize();
	for (int i=0; i<nCount; i++)
	{
		pBkItem = m_arrBlock.GetAt(i);
		if (pBkItem == NULL)
			continue;

		int nItemCnt = pBkItem->m_arrItem.GetSize();
		if (nItemCnt <= 0)
			continue;

		if (pBkItem->IsKindOf(RUNTIME_CLASS(CFidItemArr)))
		{
			pItem = pBkItem->m_arrItem.GetAt(0);

			int nValCnt = ((CFidItemArr*)pItem)->GetValueCount();
			if(m_nSortType==0)
			{
				for (int k=0; k<nValCnt; k++)
				{
					for (int j=0; j<nItemCnt; j++)
					{
						pItem = pBkItem->m_arrItem.GetAt(j);
						if ((pItem == NULL) || (pItem->GetUseBuf() == FALSE))
							continue;
						
						strData = ((CFidItemArr*)pItem)->GetValue(k);
						nSize = strData.GetLength();
						if (nIndex+nSize > nBufSize)
							return 0;
						
						memcpy(pszData+nIndex, strData, nSize);
						nIndex += nSize;
					}
				}
			}
			else
			{
				for (int k=nValCnt-1; k>=0; k--)
				{
					for (int j=0; j<nItemCnt; j++)
					{
						pItem = pBkItem->m_arrItem.GetAt(j);
						if ((pItem == NULL) || (pItem->GetUseBuf() == FALSE))
							continue;

						strData = ((CFidItemArr*)pItem)->GetValue(k);
						nSize = strData.GetLength();
						if (nIndex+nSize > nBufSize)
							return 0;

						memcpy(pszData+nIndex, strData, nSize);
						nIndex += nSize;
					}
				}
			}
		}
		else
		{
			for (int j=0; j<nItemCnt; j++)
			{
				pItem = pBkItem->m_arrItem.GetAt(j);
				if ((pItem == NULL) || (pItem->GetUseBuf() == FALSE))
					continue;

				strData = pItem->GetValue();
				nSize = strData.GetLength();
				if (nIndex+nSize > nBufSize)
					return 0;

				memcpy(pszData+nIndex, strData, nSize);
				nIndex += nSize;
			}
		}
	}

	return 1;
}

void CFidPack::SetUseBufAtItem(CString strItemID, BOOL bUse)
{
	CFidItem* pItem = GetItemAt(strItemID);

	if (pItem)
		pItem->SetUseBuf(bUse);
}

BOOL CFidPack::GetUseBufAtItem(CString strItemID)
{
	CFidItem* pItem = GetItemAt(strItemID);

	if (pItem)
		return pItem->GetUseBuf();

	return FALSE;
}

CFidItem* CFidPack::GetItemAt(CString strItemID)
{
	CFidItem* pItem = NULL;
	m_mapItem.Lookup(strItemID, pItem);

	return pItem;
}

CFidItem* CFidPack::GetItemChartAt(CString strItemID)
{
	CFidItem* pItem = NULL;
	m_mapChartNameItem.Lookup(strItemID, pItem);

	return pItem;
}

BOOL CFidPack::AddBlock(CFidBlock* pBlock)
{
	CFidBlock *pItem = NULL;
	int nSize = m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pItem = m_arrBlock.GetAt(i);
		if (pItem)
		{
			if (pItem->m_strBlockName.Compare(pBlock->m_strBlockName) == 0)
				return FALSE;
		}
	}

	m_arrBlock.Add(pBlock);

	return TRUE;
}

CFidBlock* CFidPack::GetBlockAt(CString strName)
{
	CFidBlock *pItem = NULL;
	int nSize = m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pItem = m_arrBlock.GetAt(i);
		if (pItem)
		{
			if (strName.Compare(pItem->m_strBlockName) == 0)
				return pItem;
		}
	}

	return NULL;
}

CFidBlock* CFidPack::GetBlockAt(int nIndex)
{
	if (nIndex >= m_arrBlock.GetSize())
		return NULL;

	return m_arrBlock.GetAt(nIndex);
}

int CFidPack::GetBlockSize()
{
	return m_arrBlock.GetSize();
}

int CFidPack::AddItem(CString strBlockName, CFidItem* pItem)
{
	CString strData;
	strData = pItem->GetItemID();
	if (atoi(strData) <= 0)
		return 3;
/*
	CFidItem* pTemp = GetItemAt(strData);
	if (pTemp)
	{
		strData.Format(_T("%s 와 동일한 자료명이 있습니다."), pItem->GetItemID());
		AfxMessageBox(strData);

		return 1;
	}
*/
	CFidBlock* pBlock = GetBlockAt(strBlockName);
	if (!pBlock)
	{
		strData.Format(_T("%s 블럭을 찾을수가 없습니다."), pBlock->m_strBlockName);
		AfxMessageBox(strData);

		return 2;
	}

	CFidItem* pTemp = pBlock->GetItemAt(strData);
	if (pTemp)
	{
		strData.Format(_T("%s 와 동일한 자료명이 있습니다."), pItem->GetItemID());
		AfxMessageBox(strData);

		return 1;
	}

	pBlock->AddItem(pItem);
	m_arrItem.Add(pItem);
	m_mapItem.SetAt(pItem->GetItemID(), pItem);
	m_mapChartNameItem.SetAt(pItem->GetChartID(), pItem);

	return 0;
}

int CFidPack::GetItemSizeFromBlock(CString strBlockName)
{
	CFidBlock* pBlock = GetBlockAt(strBlockName);
	if (pBlock)
		return pBlock->GetItemSize();

	return 0;
}

CString CFidPack::GetGridSymbol()
{
	CFidBlock* pBlock = NULL;
	int nSize = m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pBlock = m_arrBlock.GetAt(i);
		if (pBlock->m_bArray)
			return pBlock->m_strGridSym;
	}

	return _T("");
}

void CFidPack::SetGridSymbol(CString strSymbol)
{
	CFidBlock* pBlock = NULL;
	int nSize = m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pBlock = m_arrBlock.GetAt(i);
		if (pBlock->m_bArray)
		{
			pBlock->m_strGridSym = strSymbol;
			if (pBlock->m_strDiffGridSym.IsEmpty())
				pBlock->m_strDiffGridSym = strSymbol;
		}
	}
}

CString CFidPack::GetDiffGridSymbol()
{
	CFidBlock* pBlock = NULL;
	int nSize = m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pBlock = m_arrBlock.GetAt(i);
		if (pBlock->m_bArray)
			return pBlock->m_strDiffGridSym;
	}

	return _T("");
}

void CFidPack::SetDiffGridSymbol(CString strDiffSymbol)
{
	CFidBlock* pBlock = NULL;
	int nSize = m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pBlock = m_arrBlock.GetAt(i);
		if (pBlock->m_bArray)
			pBlock->m_strDiffGridSym = strDiffSymbol;
	}
}

//////////////////////////////////////////////////////////////////////
// CFidQuery
//////////////////////////////////////////////////////////////////////
CFidQuery::CFidQuery()
{
	m_nVer = 2;
	m_nMarket = MARKET_STOCK;
	m_nRealKey = -1;
	m_strRealSymbol = _T("");
}

CFidQuery::~CFidQuery()
{
	RemoveAll();
}

void CFidQuery::RemoveAll()
{
	m_InPack.RemoveAll();
	m_OutPack.RemoveAll();
}

void CFidQuery::Copy(const CFidQuery& item)
{
	RemoveAll();

	m_nVer			= item.m_nVer;
	m_strID			= item.m_strID;
	m_strOOPCode	= item.m_strOOPCode;
	m_strRealSymbol	= item.m_strRealSymbol;
	m_nRealKey		= item.m_nRealKey;
	m_nMarket		= item.m_nMarket;

	m_InPack		= item.m_InPack;
	m_OutPack		= item.m_OutPack;
}

CFidQuery& CFidQuery::operator = (const CFidQuery& item)
{
	Copy(item);

	return *this;
}

BOOL CFidQuery::WriteQueryFile(CString strPath)
{
	CFile file;
	if (!file.Open(strPath, CFile::typeBinary|CFile::modeCreate|CFile::modeWrite))
	{
		CString strMsg;
		strMsg = _T("failed create file \"") + strPath + _T("\"");
		AfxMessageBox(strMsg);

		return FALSE;
	}

	CArchive ar(&file, CArchive::store);

	Serialize(ar);

	ar.Close();
	file.Close();

	return TRUE;
}

BOOL CFidQuery::ReadQueryFile(CString strPath)
{
	CFile file;
	if (!file.Open(strPath, CFile::typeBinary|CFile::modeRead))
	{
		CString strMsg;
		strMsg = _T("not find file \"") + strPath + _T("\"");
		AfxMessageBox(strMsg);

		return FALSE;
	}

	CArchive ar(&file, CArchive::load);

	Serialize(ar);

	ar.Close();
	file.Close();

	return TRUE;
}

void CFidQuery::Serialize(CArchive& ar)
{
	CString strData;
	if (ar.IsStoring())
	{
		ar << 2;
		ar << m_strID;
		ar << m_strOOPCode;
		ar << m_strRealSymbol;
	}
	else
	{
		ar >> m_nVer;
		ar >> m_strID;
		ar >> m_strOOPCode;

		if (m_nVer > 1)
		{
			ar >> strData;
			SetRealSymbol(strData);
		}
	}

	m_InPack.Serialize(ar);
	m_OutPack.Serialize(ar);

	if (m_strRealSymbol.IsEmpty())
	{
		if (m_InPack.GetItemAt(CFidQuery::GetCodeFID(MARKET_STOCK)) != NULL)
			m_nMarket = MARKET_STOCK;
		else if (m_InPack.GetItemAt(CFidQuery::GetCodeFID(MARKET_FUTOPT)) != NULL)
			m_nMarket = MARKET_FUTOPT;
		else if (m_InPack.GetItemAt(CFidQuery::GetCodeFID(MARKET_UPJONG)) != NULL)
			m_nMarket = MARKET_UPJONG;
	}
}

void CFidQuery::SetRealSymbol(CString strSymbol)
{
	m_strRealSymbol = strSymbol;

	int nRealSym = atoi(strSymbol);
	int nLeft3 = nRealSym/1000;

	if (nLeft3 < 20)		m_nMarket = MARKET_STOCK;	//	주식
	else if (nLeft3 >= 30)	m_nMarket = MARKET_FUTOPT;	//	선/옵
	else					m_nMarket = MARKET_UPJONG;	//	업종
}

CString CFidQuery::GetCodeFID(int nMarket)
{
	if (nMarket == MARKET_STOCK)		return _T("1301");
	else if (nMarket == MARKET_UPJONG)	return _T("20301");
	else if (nMarket == MARKET_FUTOPT)	return _T("30301");

	return _T("");
}

CString CFidQuery::GetTickFID(int nType)
{
	return _T("308");
/*
	switch (nType)
	{
	case 310:
	case 410:
		return _T("308");		//	틱
	case 511:
		return _T("308");		//	초
	case 510:
		return _T("308");		//	분
	case 610:
	case 710:
	case 810:
		return _T("NONE");		//	일/주/월
	default:
		break;
	}

	return _T("NONE");
*/
}

