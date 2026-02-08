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

IMPLEMENT_DYNAMIC(CTrPack, CObject)
IMPLEMENT_DYNAMIC(CTrQuery, CObject)

#define		INDEX_OF_CHARTBOZOSIZE		4
#define		INDEX_OF_NEXTKEY			6

//////////////////////////////////////////////////////////////////////
// CTrPack
//////////////////////////////////////////////////////////////////////
CTrPack::CTrPack()
{
	m_nVer = 3;

	m_nSortType = 0;
	m_bRealABS = FALSE;

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
	m_nSortType		= item.m_nSortType;
	m_bRealABS		= item.m_bRealABS;
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
		m_nVer = 4;
		ar << m_nVer;
		ar << m_bInput;
		ar << m_strID;
		ar << m_strName;
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
		ar >> m_strName;
		if (m_nVer > 3)
		{
			ar >> m_bRealABS;
			ar >> m_nSortType;
		}

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
	m_strNextKey.Empty();

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
	for (int nBlockIndex=0; nBlockIndex<nCnt; nBlockIndex++)
	{
		pBlock = m_arrBlock.GetAt(nBlockIndex);
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

			if (m_nOneArrSize*nArrCnt > nSize-nIndex)
			{
				CString strDebug;
				strDebug.Format("[Error Data Size] OneArrSize[%d], ArrCnt[%d], nSize[%d], nIndex[%d]", m_nOneArrSize, nArrCnt, nSize, nIndex);
				AfxMessageBox( strDebug );
				break;
			}
		}
		int nTRIndex = 0;
		//<--@IMSI-alzioyes

		CString strTranDate;
		CString strTempDate, strTempTime;
		CString strTimePacket;

		CString strDatePacketType = GetPacketType( _T("보조일자") );
		CString strTimePacketType = GetPacketType( _T("자료일자") );
		CString strBojoMsgPacketType = GetBojoMsgDataFromKey( _T("조회주기") );

		BOOL bConvertDateTime = TRUE;	//날짜와 시간데이터 가공여부 판단	
		if( strDatePacketType.IsEmpty() )
			bConvertDateTime = FALSE;

		for (int j=0; j<nArrCnt; j++)
		{
			strTranDate = "";
			strTempDate = "";
			for (int k=0; k<nYSize; k++)
			{
				if (pBlock->m_bArray)
				{
					pArrItem = (CTrItemArr*)pBlock->m_TrItemArr.GetAt(k);
					if (pArrItem == NULL)
						continue;

					if( bConvertDateTime )
					{
						if( pArrItem->GetItemID().CompareNoCase( _T("date") ) == 0 )
							strTempDate = CString( (pszData+nIndex), pArrItem->GetSize() );
						else if( pArrItem->GetItemID().CompareNoCase( _T("time") ) == 0 )
						{
							strTempTime = CString( (pszData+nIndex), pArrItem->GetSize() );	
							strTranDate = CombineDateTime( strBojoMsgPacketType, strTempDate, strTempTime );
							nIndex += pArrItem->SetDataFromData( pszData, nIndex, strTranDate );
							continue;
						}
					}					

					nIndex += pArrItem->SetDataFromBuffer(pszData, nIndex);
				}
				else
				{
					pItem = pBlock->m_TrItemArr.GetAt(k);
					if (pItem == NULL)
						continue;

					if (j > 0)
						continue;

					//TR의 Output구조를 서로 약속했기 때문에 셋팅한 구조가 아닌 조회된 형태로 변경한다.
					if(nBlockIndex==0 && k==INDEX_OF_CHARTBOZOSIZE)
					{
						int nSize = pItem->GetSize();
						int nBozoLen = 0;
						char* pSizeData = new char[nSize+1];
						ZeroMemory(pSizeData, nSize + 1);
						CopyMemory(pSizeData, pszData+nIndex, nSize);
						nBozoLen = atoi(pSizeData);

						nIndex += pItem->SetDataFromBuffer(pszData, nIndex);

						pItem = pBlock->m_TrItemArr.GetAt(k+1);
						pItem->SetSize(nBozoLen);

						delete[] pSizeData;

						if(nBozoLen==0) k++;	//보조메시지정보가 없으면 skip +1						
					}
					//NextKey정보 얻기
					else if(nBlockIndex==0 && k==INDEX_OF_NEXTKEY)
					{
						int nSize = pItem->GetSize();
						char* pSizeData = new char[nSize+1];
						ZeroMemory(pSizeData, nSize + 1);
						CopyMemory(pSizeData, pszData+nIndex, nSize);
						m_strNextKey = pSizeData;
						//m_strNextKey.TrimRight();
						//pItem->SetNextKey(pSizeData);
						delete[] pSizeData;

						nIndex += pItem->SetDataFromBuffer(pszData, nIndex);
					}
					else
					{
						nIndex += pItem->SetDataFromBuffer(pszData, nIndex);
					}
				}
			}
		}
	}

	delete []pszData;
}

int CTrPack::GetChartBozoInfo(CString &rString)
{
	CTrBlock *pBlock = NULL;
	CTrItem *pItem = NULL;
	int nCnt = m_arrBlock.GetSize();
	if(nCnt != 2) return 0; //Block Count는 2 이어야 함. (일반 데이터블락 + 배열블락)

	pBlock = m_arrBlock.GetAt(0);
	if (pBlock == NULL)
		return 0;

	pItem = pBlock->m_TrItemArr.GetAt(INDEX_OF_CHARTBOZOSIZE+1);
	int nBozeLen = pItem->GetSize();
	rString = pItem->GetstrValue();
	return nBozeLen;
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

int CTrPack::GetDataBuffer(char *pszData, int nBufSize, CStringArray *pArrPackName, CArray<int, int> *pArrPackSize)
{
	int nIndex = 0, nSize = 0, nFind = 0;
	void *pszTemp = NULL;
	CString strTemp;

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
			if (pArrPackName && pArrPackName->GetSize() > 0)
			{
				CTrItem* pTrItem;
				CArrTrItem arrTrItem;
				CArray<int, int> arrPackSize;
				for (int m=0; m<pArrPackName->GetSize(); m++)
				{
					pTrItem = pBkItem->GetItemAt(pArrPackName->GetAt(m));
					if (pTrItem)
					{
						arrTrItem.Add(pTrItem);
						arrPackSize.Add(pArrPackSize->GetAt(m));
					}
				}

				if (arrTrItem.GetSize() <= 0)
				{
					for (int m=0; m<pBkItem->m_TrItemArr.GetSize(); m++)
					{
						pTrItem = pBkItem->m_TrItemArr.GetAt(m);
						arrTrItem.Add(pTrItem);
						arrPackSize.Add(pTrItem->GetSize());
					}
				}

				if (nItemCnt <= 0)
					continue;
				pItem = pBkItem->m_TrItemArr.GetAt(0);

				int nValueIdx = 0;
				int nValCnt = ((CTrItemArr*)pItem)->GetValueCount();
				for (int k=nValCnt-1; k>-1; k--)
				{
					if (m_nSortType)	nValueIdx = nValCnt-k-1;
					else				nValueIdx = k;

//					for (int j=0; j<nItemCnt; j++)
					for (int j=0; j<arrTrItem.GetSize(); j++)
					{
//						pItem = pBkItem->m_TrItemArr.GetAt(j);
						pItem = arrTrItem.GetAt(j);
						if ((pItem == NULL))//KHD :|| (pItem->GetUseBuf() == FALSE))
							continue;

						nSize = ((CTrItemArr*)pItem)->GetValueAt(nValueIdx)->GetValue(&pszTemp);
						// 차트의 패킷 사이즈로 만든다.
						nSize = arrPackSize.GetAt(j);

						strTemp.Format(_T("%s"), pszTemp);
						strTemp.TrimRight();
						if (pItem->GetABS())
						{
							nFind = strTemp.Find(_T("-"), 0);
							if (nFind >= 0)
								strTemp.Delete(nFind);
						}

						if (nIndex+nSize > nBufSize)
							return 0;

						memcpy(pszData+nIndex, (LPTSTR)(LPCTSTR)strTemp, strTemp.GetLength());
						if (strTemp.Compare("20151026") == 0)
							strTemp = "20151026";
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

				strTemp.Format(_T("%s"), pszTemp);
				strTemp.TrimRight();
				if (pItem->GetABS())
				{
					nFind = strTemp.Find(_T("-"), 0);
					if (nFind >= 0)
						strTemp.Delete(nFind);
				}

				if (nIndex+nSize > nBufSize)
					return 0;

				memcpy(pszData+nIndex, (LPTSTR)(LPCTSTR)strTemp, strTemp.GetLength());
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

CString CTrPack::GetPacketType( CString strType )
{
	CString strPacketType = "";
	ST_CHART_PACKET stPack;
	int nSize = m_prop.GetPacketSize();
	for (int nIndex = 0; nIndex < nSize; nIndex++)
	{
		stPack = m_prop.GetPacket( nIndex );
		if( stPack.szPacketName.CompareNoCase( strType ) == 0 )
		{
			strPacketType = stPack.szTextType;
			break;
		}
	}
	return strPacketType;
}

CString CTrPack::GetLastDate()
{
	CString strDate;
	CTrBlock *pBlock = NULL;
	CTrItem *pItem = NULL;
	int nCnt = m_arrBlock.GetSize();
	for (int nBlockIndex=0; nBlockIndex<nCnt; nBlockIndex++)
	{
		pBlock = m_arrBlock.GetAt(nBlockIndex);
		if (pBlock == NULL)
			continue;

		if (!pBlock->IsArray())
			continue;

		int nYSize = pBlock->m_TrItemArr.GetSize();
		for (int k=0; k<nYSize; k++)
		{
			pItem = pBlock->m_TrItemArr.GetAt(k);
			if( pItem->GetItemID().CompareNoCase( _T("date") ) == 0 )
				break;
		}

		if (pItem)
		{
			int nIndex = pItem->GetSize();
			if (nIndex > 0)
			{
				CString strSDate = pItem->GetstrValue(0);
				CString strEDate = pItem->GetstrValue(nIndex-1);
				if (atoi(strSDate) > atoi(strEDate))
					strDate = strSDate;
				else
					strDate = strEDate;
			}
		}
	}

	return strDate;
}

CString CTrPack::GetBojoMsgDataFromKey( CString strKey )
{
	CString strResult = "DDHHMMSS";
	CString strBojoMsg, strTemp;
	GetChartBozoInfo( strBojoMsg );

	if( strBojoMsg.GetLength() == 0 )
		return strResult;

	int nFind = strBojoMsg.Find( strKey );
	if( nFind < 0 )
		return strResult;
		
	strTemp = strBojoMsg.Mid( nFind + strKey.GetLength() + 1 );
	nFind = strTemp.Find( _T(";") );
	if( nFind < 0 )
		return strResult;

	strResult = strTemp.Left( nFind );

	//DrCommon/PartnerMng/_ChartBozoManager.cpp 함수 MakeChtBojo..에 선언된 타입과 일치하게.
	switch ( atoi( strResult) )
	{
	case 0:
		strResult = _T("DDHHMMSS");
		break;
	case 1:
		strResult = _T("MMDDHHMM");
		break;
	case 2:
		strResult = _T("YYYYMMDD");
		break;
	case 3:
		strResult = _T("YYYYMMDD");
		break;
	case 4:
		strResult = _T("YYYYMM");
		break;
	case 6:
		strResult = _T("DDHHMMSS");
		break;
	default:
		strResult = "";
		break;
	}
	return strResult;
}

CString CTrPack::CombineDateTime( CString strPacketType, CString strTempDate, CString strTempTime )
{
	CString strResult = "";
	if( strPacketType.CompareNoCase( _T("DDHHMMSS") ) == 0 )
	{
		strResult.Format("%s%06d", strTempDate.Right( 2 ), atoi( strTempTime ) );
	}
	else if( strPacketType.CompareNoCase( _T("MMDDHHMM") ) == 0 )
	{
		CString strTemp;
		strTemp.Format("%d", atoi( strTempTime ) );
		strResult.Format("%s%04d", strTempDate.Right( 4 ), atoi( strTemp.Left( 4 ) ) );
	}
	else if( strPacketType.CompareNoCase( _T("YYYYMMDD") ) == 0 )
	{
		strResult = strTempDate;
	}
	else if( strPacketType.CompareNoCase( _T("YYYYMMDD") ) == 0 )
	{
		strResult = strTempDate;
	}
	else if( strPacketType.CompareNoCase( _T("YYYYMM") ) == 0 )
	{
		strResult = strTempDate.Left( 6 );
	}
	else if( strPacketType.CompareNoCase( _T("DDHHMMSS") ) == 0 )
	{
		CString strTemp;
		strTemp.Format("%d", atoi( strTempTime ) );
		strResult.Format("%s%06d", strTempDate.Right( 2 ), atoi( strTemp ) );
	}
	return strResult;
}


//////////////////////////////////////////////////////////////////////
// CTrQuery
//////////////////////////////////////////////////////////////////////
CTrQuery::CTrQuery()
{
	m_nVer = 2;
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
		m_nVer = 2;
		ar << m_nVer;
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

