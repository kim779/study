// ChartItemData.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TrQuery.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	CURRENT_VER		2

IMPLEMENT_DYNAMIC(CTrPack, CObject)
IMPLEMENT_DYNAMIC(CTrQuery, CObject)

//////////////////////////////////////////////////////////////////////
// CTrPack
//////////////////////////////////////////////////////////////////////
CTrPack::CTrPack()
{
	m_nVer = CURRENT_VER;

	m_nSortType = 0;

	//-->@IMSI-alzioyes
	m_nTRDataCnt=0;	
	m_nOneArrSize = 0;
	//m_pTRData = NULL;
	//<--@IMSI-alzioyes
}

CTrPack::~CTrPack()
{
	RemoveAll();
}

void CTrPack::Copy(const CTrPack& item)
{
	RemoveAll();

	CTrBlock	*pItem = NULL, *pNewItem = NULL;
	int nSize = item.m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pItem = item.m_arrBlock.GetAt(i);
		if (pItem)
		{
			pNewItem = new CTrBlock;
			pNewItem->Copy((CTrBlock&)*pItem);
			m_arrBlock.Add(pNewItem);

			CTrItem *pItem = NULL;
			int nYSize = pNewItem->m_TrItemArr.GetSize();
			for (int j=0; j<nYSize; j++)
			{
				pItem = pNewItem->m_TrItemArr.GetAt(j);
				if (pItem)
				{
					m_TrItemArr.Add(pItem);
					m_mapItem.SetAt(pItem->GetItemID(), pItem);
				}
			}
		}
	}

	m_bInput		= item.m_bInput;
	m_strID			= item.m_strID;
	m_strName		= item.m_strName;
}

CTrPack& CTrPack::operator = (const CTrPack& item)
{
	Copy(item);

	return *this;
}

BOOL CTrPack::WriteQueryFile(CString strPath)
{
//	CString strFile;
//	if (m_bInput)	strFile.Format(_T("%s\\%s_in.qry"), strPath, m_strID);
//	else			strFile.Format(_T("%s\\%s_out.qry"), strPath, m_strID);

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

BOOL CTrPack::ReadQueryFile(CString strPath)
{
//	CString strFile;
//	if (m_bInput)	strFile.Format(_T("%s\\%s_in.qry"), strPath, m_strID);
//	else			strFile.Format(_T("%s\\%s_out.qry"), strPath, m_strID);

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

void CTrPack::Serialize(CArchive& ar)
{
	int i, nSize = 0;
	CTrBlock	*pBlock = NULL;
	if (ar.IsStoring())
	{
		ar << CURRENT_VER;
		ar << m_bInput;
		ar << m_strID;
		ar << m_strName;

		nSize = m_arrBlock.GetSize();
		ar << nSize;

		for (i=0; i<nSize; i++)
		{
			pBlock = m_arrBlock.GetAt(i);
			if (pBlock)
				pBlock->Serialize(ar, CURRENT_VER);
		}
	}
	else
	{

		ar >> m_nVer;
		ar >> m_bInput;
		ar >> m_strID;
		ar >> m_strName;
		ar >> nSize;

		for (i=0; i<nSize; i++)
		{
			pBlock = new CTrBlock;
			pBlock->Serialize(ar, m_nVer);
			m_arrBlock.Add(pBlock);

			CTrItem *pItem = NULL;
			int nYSize = pBlock->m_TrItemArr.GetSize();
			for (int j=0; j<nYSize; j++)
			{
				pItem = pBlock->m_TrItemArr.GetAt(j);
				if (pItem)
				{
					m_TrItemArr.Add(pItem);
					m_mapItem.SetAt(pItem->GetItemID(), pItem);
				}
			}
		}
	}
}

void CTrPack::RemoveAll()
{
	CTrBlock	*pItem = NULL;
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
	m_TrItemArr.RemoveAll();
	m_mapItem.RemoveAll();
}

void CTrPack::RemoveAllData()
{
	CTrItem *pItem = NULL;
	int nSize = m_TrItemArr.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pItem = m_TrItemArr.GetAt(i);
		if (pItem == NULL)
			continue;

		pItem->RemoveAllData();
	}
}

BOOL CTrPack::ResetKeydata()
{
	m_TrItemArr.RemoveAll();
	m_mapItem.RemoveAll();

	CTrBlock *pBlock = NULL;
	int nSize = m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pBlock = m_arrBlock.GetAt(i);

		CTrItem *pItem = NULL;
		int nYSize = pBlock->m_TrItemArr.GetSize();
		for (int j=0; j<nYSize; j++)
		{
			pItem = pBlock->m_TrItemArr.GetAt(j);
			if (pItem)
			{
				m_TrItemArr.Add(pItem);
				m_mapItem.SetAt(pItem->GetItemID(), pItem);
			}
		}
	}

	return TRUE;
}

void CTrPack::ReceiveData(const char* szData, int nSize)
{
	RemoveAllData();

	char *pszData = new char[nSize+1];
	memset(pszData, 0x00, sizeof(nSize+1));
	memcpy(pszData, szData, nSize);

	int nIndex = 0;

	CTrBlock *pBlock = NULL;
	CTrItem *pItem = NULL;
	CTrItemArr *pArrItem = NULL;
	int nArrCnt, nCnt, nYSize;
	nCnt = m_arrBlock.GetSize();
	for (int i=0; i<nCnt; i++)
	{
		pBlock = m_arrBlock.GetAt(i);
		if (pBlock == NULL)
			continue;

		if (pBlock->IsArray())
		{
			nArrCnt = GetArrCnt(pBlock->m_strCntItemID); 

			//@IMSI-alzioyes
			m_nTRDataCnt=nArrCnt;
		}
		else
			nArrCnt = 1;

		nYSize = pBlock->m_TrItemArr.GetSize();

		//-->@IMSI-alzioyes
		if (pBlock->m_bArray)
		{
			//if(m_pTRData) delete [] m_pTRData;

			int nOneArrSize=0;
			for (int k=0; k<nYSize; k++)
			{
				pItem = pBlock->m_TrItemArr.GetAt(k);
				nOneArrSize += pItem->GetSize();
			}

			m_nOneArrSize = nOneArrSize;
			//m_pTRData = new BYTE[(nOneArrSize*nArrCnt)+1];
		}
		int nTRIndex = 0;
		//<--@IMSI-alzioyes

		for (int j=0; j<nArrCnt; j++)
		{
			for (int k=0; k<nYSize; k++)
			{
				if (pBlock->m_bArray)
				{
					pArrItem = (CTrItemArr*)pBlock->m_TrItemArr.GetAt(k);
					if (pArrItem == NULL)
						continue;

					nIndex += pArrItem->SetDataFromBuffer(pszData, nIndex);
				}
				else
				{
					pItem = pBlock->m_TrItemArr.GetAt(k);
					if (pItem == NULL)
						continue;

					if (j > 0)
						continue;

					nIndex += pItem->SetDataFromBuffer(pszData, nIndex);
				}
			}
		}
	}

	delete []pszData;
}

DWORD CTrPack::LoadDataFile(CString strFile)
{
	RemoveAllData();

	ResetKeydata();

	CFile file;
	if (!file.Open(strFile, CFile::typeBinary|CFile::modeRead))
	{
		CString strMsg;
		strMsg = _T("not find file \"") + strFile + _T("\"");
		AfxMessageBox(strMsg);

		return -1;
	}

	CTrBlock *pBlock = NULL;
	CTrItem *pItem = NULL;
	int nArrCnt, nSize, nYSize;
	nSize = m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pBlock = m_arrBlock.GetAt(i);
		if (pBlock == NULL)
			continue;

		if (pBlock->IsArray())
			nArrCnt = GetArrCnt(pBlock->m_strCntItemID);
		else
			nArrCnt = 1;

		nYSize = pBlock->m_TrItemArr.GetSize();

		for (int j=0; j<nArrCnt; j++)
		{
			for (int k=0; k<nYSize; k++)
			{
				pItem = pBlock->m_TrItemArr.GetAt(k);
				if (pItem == NULL)
					continue;

				if (pBlock->m_bArray)
					((CTrItemArr*)pItem)->SetDataFromFile(&file);
				else
				{
					if (j > 0)
						continue;

					pItem->SetDataFromFile(&file);
				}
			}
		}
	}

	int nLenth = file.GetLength();

	file.Close();

	return nLenth;
}

int CTrPack::GetArrCnt(CString strCntItemID)
{
	CTrBlock *pBlock = NULL;
	CTrItem *pItem = NULL;

	int nYSize, nSize = m_arrBlock.GetSize();
	for (int i=0; i<nSize; i++)
	{
		pBlock = m_arrBlock.GetAt(i);
		if (pBlock == NULL)
			continue;

		nYSize = pBlock->m_TrItemArr.GetSize();

		for (int k=0; k<nYSize; k++)
		{
			pItem = pBlock->m_TrItemArr.GetAt(k);
			if (pItem == NULL)
				continue;

			if (strCntItemID.Compare(pItem->GetItemID()) == 0)
			{
				if (pItem->IsValue())
					return pItem->GetnValue();
				else
					return 0;
			}
		}
	}

	return 0;
}

CTrItem* CTrPack::GetItemAt(int nIndex)
{
	if (m_TrItemArr.GetSize() <= nIndex)
		return NULL;

	return m_TrItemArr.GetAt(nIndex);
}

CString CTrPack::GetDataAt(CString szKey)
{
	CTrItem* pItem = NULL;
	m_mapItem.Lookup(szKey, pItem);
	if (pItem)
		return pItem->GetstrValue();

	return _T("");
}

CString CTrPack::GetDataAt(int nIndex)
{
	if (m_TrItemArr.GetSize() <= nIndex)
		return _T("");

	CTrItem* pItem = m_TrItemArr.GetAt(nIndex);
	if (pItem)
		return pItem->GetstrValue();

	return _T("");
}

BOOL CTrPack::GetArrDataAt(CStringArray* pArr, CString szKey)
{
	if (pArr == NULL)
		return FALSE;

	CTrItem* pItem = NULL;
	m_mapItem.Lookup(szKey, pItem);
	if (pItem && pItem->IsArray())
	{
		int nSize = ((CTrItemArr*)pItem)->GetValueCount();
		for (int i=0; i<nSize; i++)
			pArr->Add(pItem->GetstrValue(i));

		return TRUE;
	}

	return FALSE;
}

BOOL CTrPack::GetArrDataAt(CStringArray* pArr, int nIndex)
{
	if (pArr == NULL)
		return FALSE;

	if (m_TrItemArr.GetSize() <= nIndex)
		return FALSE;

	CTrItem* pItem = NULL;
	pItem = m_TrItemArr.GetAt(nIndex);
	if (pItem && pItem->IsArray())
	{
		int nSize = ((CTrItemArr*)pItem)->GetValueCount();
		for (int i=0; i<nSize; i++)
			pArr->Add(pItem->GetstrValue(i));

		return TRUE;
	}

	return FALSE;
}

int CTrPack::GetDataBuffer(char *pszData, int nBufSize)
{
	int nIndex = 0, nSize = 0;
	void *pszTemp = NULL;

	CTrBlock *pBkItem = NULL;
	CTrItem *pItem = NULL;
	int nCount = m_arrBlock.GetSize();
	for (int i=0; i<nCount; i++)
	{
		pBkItem = m_arrBlock.GetAt(i);
		if (pBkItem == NULL)
			continue;

		int nItemCnt = pBkItem->m_TrItemArr.GetSize();
		if (nItemCnt <= 0)
			continue;

		if (pBkItem->IsArray())
		{
			pItem = pBkItem->m_TrItemArr.GetAt(0);

			int nValCnt = ((CTrItemArr*)pItem)->GetValueCount();
			if(m_nSortType==0)
			{
				for (int k=0; k<nValCnt; k++)
				{
					for (int j=0; j<nItemCnt; j++)
					{
						pItem = pBkItem->m_TrItemArr.GetAt(j);
						if ((pItem == NULL) || (pItem->GetUseBuf() == FALSE))
							continue;
						
						nSize = ((CTrItemArr*)pItem)->GetValueAt(k)->GetValue(&pszTemp);
						if (nIndex+nSize > nBufSize)
							return 0;
						
						memcpy(pszData+nIndex, pszTemp, nSize);
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
						pItem = pBkItem->m_TrItemArr.GetAt(j);
						if ((pItem == NULL) || (pItem->GetUseBuf() == FALSE))
							continue;

						nSize = ((CTrItemArr*)pItem)->GetValueAt(k)->GetValue(&pszTemp);
						if (nIndex+nSize > nBufSize)
							return 0;

						memcpy(pszData+nIndex, pszTemp, nSize);
						nIndex += nSize;
					}
				}
			}
		}
		else
		{
			for (int j=0; j<nItemCnt; j++)
			{
				pItem = pBkItem->m_TrItemArr.GetAt(j);
				if ((pItem == NULL) || (pItem->GetUseBuf() == FALSE))
					continue;

				nSize = pItem->GetValue(&pszTemp);
				if (nIndex+nSize > nBufSize)
					return 0;

				memcpy(pszData+nIndex, pszTemp, nSize);
				nIndex += nSize;
			}
		}
	}

	return 1;
}

void CTrPack::SetUseBufAtItem(CString strItemID, BOOL bUse)
{
	CTrItem* pItem = GetItemAt(strItemID);

	if (pItem)
		pItem->SetUseBuf(bUse);
}

BOOL CTrPack::GetUseBufAtItem(CString strItemID)
{
	CTrItem* pItem = GetItemAt(strItemID);

	if (pItem)
		return pItem->GetUseBuf();

	return FALSE;
}

CTrItem* CTrPack::GetItemAt(CString strItemID)
{
	CTrItem* pItem = NULL;
	m_mapItem.Lookup(strItemID, pItem);

	return pItem;
}

BOOL CTrPack::AddBlock(CTrBlock* pBlock)
{
	CTrBlock *pItem = NULL;
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

CTrBlock* CTrPack::GetBlockAt(CString strName)
{
	CTrBlock *pItem = NULL;
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

CTrBlock* CTrPack::GetBlockAt(int nIndex)
{
	if (nIndex >= m_arrBlock.GetSize())
		return NULL;

	return m_arrBlock.GetAt(nIndex);
}

int CTrPack::GetBlockSize()
{
	return m_arrBlock.GetSize();
}

int CTrPack::AddItem(CString strBlockName, CTrItem* pItem)
{
	CString strData;
	strData = pItem->GetItemID();
	if (strData.GetLength() <= 0)
		return 3;

	CTrItem* pTemp = GetItemAt(pItem->GetItemID());
	if (pTemp)
	{
		strData.Format(_T("%s 와 동일한 자료명이 있습니다."), pItem->GetItemID());
		AfxMessageBox(strData);

		return 1;
	}

	CTrBlock* pBlock = GetBlockAt(strBlockName);
	if (!pBlock)
	{
		strData.Format(_T("%s 블럭을 찾을수가 없습니다."), pBlock->m_strBlockName);
		AfxMessageBox(strData);

		return 2;
	}

	pBlock->AddItem(pItem);
	m_TrItemArr.Add(pItem);
	m_mapItem.SetAt(pItem->GetItemID(), pItem);

	return 0;
}

int CTrPack::GetItemSizeFromBlock(CString strBlockName)
{
	CTrBlock* pBlock = GetBlockAt(strBlockName);
	if (pBlock)
		return pBlock->GetItemSize();

	return 0;
}


//////////////////////////////////////////////////////////////////////
// CTrQuery
//////////////////////////////////////////////////////////////////////
CTrQuery::CTrQuery()
{
	m_nVer = CURRENT_VER;
	m_InPack.m_bInput = TRUE;
	m_OutPack.m_bInput = FALSE;
}

CTrQuery::~CTrQuery()
{
	RemoveAll();
}

void CTrQuery::RemoveAll()
{
	m_InPack.RemoveAll();
	m_OutPack.RemoveAll();
}

void CTrQuery::Copy(const CTrQuery& item)
{
	RemoveAll();

	m_nVer		= item.m_nVer;
	m_strID		= item.m_strID;
	m_strTRCode	= item.m_strTRCode;
	m_strRealSymbol	= item.m_strRealSymbol;

	m_InPack	= item.m_InPack;
	m_OutPack	= item.m_OutPack;
}

CTrQuery& CTrQuery::operator = (const CTrQuery& item)
{
	Copy(item);

	return *this;
}

BOOL CTrQuery::WriteQueryFile(CString strPath)
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

BOOL CTrQuery::ReadQueryFile(CString strPath)
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

void CTrQuery::Serialize(CArchive& ar)
{
	CString strData;
	if (ar.IsStoring())
	{
		ar << 2;
		ar << m_strID;
		ar << m_strTRCode;
		ar << m_strRealSymbol;
	}
	else
	{
		ar >> m_nVer;
		ar >> m_strID;
		ar >> m_strTRCode;
		if (m_nVer > 1)
			ar >> m_strRealSymbol;
	}

	m_InPack.Serialize(ar);
	m_InPack.m_bInput = TRUE;
	m_OutPack.Serialize(ar);
	m_OutPack.m_bInput = FALSE;
}

