// WinixConverter.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinixConverter.h"

#include "../../ForSite/FS_Env.h"
//#include "../../DrCommon/PcTrMng/PcTrMng.h"
#include "DrChartCtl.h"

#include <math.h>

#ifdef __MULTI_LANG__
	#include "../../Chart/Include_Chart/Dll_Load/IMetaTable.h"	//@Solomon-MultiLang:091117
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMasterDataManagerLast* CWinixConverter::m_pMaster = NULL;

CMapStringToString	CWinixConverter::m_mapCodeType;

//////////////////////////////////////////////////////////////////////
// CWinixConverter
//////////////////////////////////////////////////////////////////////
CWinixConverter::CWinixConverter()
{
	//	Serialize Version Number.
	m_nVer = 0;
	m_nTRType = 0;
	m_nCnt = 200;
	m_nSortType = 0;
	m_strGridSymbol = _T("");

	m_pszLayout = NULL;

	m_strMajorFID = _T("101");
	m_bDirectSend = FALSE;

	m_nRealAbs = 1;

	CString strRoot;
	GetDir_DrfnRoot(strRoot);
	strRoot += "\\Icss\\ChartDefaultFID.qry";
	m_defFidQry.ReadQueryFile(strRoot);

	m_pBozoMng = NULL;
//	m_pMaster = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);

	m_pParent = NULL;
	m_bRunMode = FALSE;
	m_nChartType = 0;
	m_nFXChartType = 0;		//20100310_JS.Kim

	m_nFidTypeDate = MAPCHART_DATETYPE_BASE;
	m_nFidTypeTime = MAPCHART_TIMETYPE_BASE;
	m_szFidTypeDate = STR_MAPCHART_DATETYPE_BASE;
	m_szFidTypeTime = STR_MAPCHART_TIMETYPE_BASE;
}

CWinixConverter::~CWinixConverter()
{
	RemoveAll();

	m_defFidQry.RemoveAll();
}

void CWinixConverter::Copy( CWinixConverter& item)
{
	m_nVer			= item.m_nVer;
	m_nTRType		= item.m_nTRType;
	m_nCnt			= item.m_nCnt;
	m_nSortType		= item.m_nSortType;

	m_strMapFile	= item.m_strMapFile;
	m_strGridSymbol	= item.m_strGridSymbol;

	m_nRealAbs		= item.m_nRealAbs;
	m_bDirectSend	= item.m_bDirectSend;
	m_nChartType	= item.m_nChartType;
	m_nFXChartType	= item.m_nFXChartType;		//20100310_JS.Kim

	if(m_nChartType==3)
	{
		m_nFidTypeDate = MAPCHART_DATETYPE_CME;
		m_nFidTypeTime = MAPCHART_TIMETYPE_CME;

		m_szFidTypeDate = STR_MAPCHART_DATETYPE_CME;
		m_szFidTypeTime = STR_MAPCHART_TIMETYPE_CME;
	}

	CString strKey, strData;
	const char *szKey=NULL, *szData=NULL;
	POSITION pos = item.m_propMap.GetStartPosition();
	while (pos)
	{
		item.m_propMap.GetNextAssoc(pos, szKey, szData);
		strKey.Format(_T("%s"), szKey);
		strData.Format(_T("%s"), szData);
		m_propMap.SetAt(strKey, strData);
	}

	CTrQuery trQuery;
	int i, nSize = item.m_arrTrQuery.GetSize();
	for (i=0; i<nSize; i++)
		m_arrTrQuery.Add(item.m_arrTrQuery.GetAt(i));

	nSize = item.m_arrFidQuery.GetSize();
	for (i=0; i<nSize; i++)
		m_arrFidQuery.Add(item.m_arrFidQuery.GetAt(i));
}

CWinixConverter& CWinixConverter::operator = ( CWinixConverter& item)
{
	Copy(item);

	return *this;
}

void CWinixConverter::Serialize(CArchive& ar)
{
	CTrQuery trQry;
	CFidQuery fidQry;
	int nCount = 0, i = 0;
	const char *szKey=NULL, *szData=NULL;
	CString strKey, strData;
	if (ar.IsStoring())
	{	// storing code
		m_nVer = 8;	//090915여기에서 버전을 셋팅해야 함. 그래야 반영됨.

		ar << m_nVer;
		ar << m_nTRType;
		ar << m_nCnt;
		ar << m_nSortType;
		ar << m_strGridSymbol;
		ar << m_nRealAbs;
		ar << m_bDirectSend;
		ar << m_nChartType;

		if (m_nTRType)
		{
			nCount = m_arrTrQuery.GetSize();
			ar << nCount;
			for (i=0; i<nCount; i++)
			{
				trQry = m_arrTrQuery.GetAt(i);
				trQry.Serialize(ar);
			}
		}
		else
		{
			nCount = m_arrFidQuery.GetSize();
			ar << nCount;
			for (i=0; i<nCount; i++)
			{
				fidQry = m_arrFidQuery.GetAt(i);
				fidQry.Serialize(ar);
			}
		}

		nCount = m_propMap.GetCount();
		ar << nCount;

		POSITION pos = m_propMap.GetStartPosition();
		while (pos)
		{
			m_propMap.GetNextAssoc(pos, szKey, szData);
			strKey.Format(_T("%s"), szKey);
			strData.Format(_T("%s"), szData);
			ar << strKey;
			ar << strData;
		}
	}
	else
	{	// loading code
		ar >> m_nVer;
		ar >> m_nTRType;
		ar >> m_nCnt;
		if (m_nVer >= 3)			ar >> m_nSortType;
		if (m_nVer >= 4)
		{
			ar >> strData;
			SetGridSymbol(strData);
		}
		if (m_nVer >= 5)			ar >> m_nRealAbs;
		if (m_nVer >= 6)			ar >> m_bDirectSend;
		if (m_nVer >= 8)			ar >> m_nChartType;

		if(m_nChartType==3)
		{
			m_nFidTypeDate = MAPCHART_DATETYPE_CME;
			m_nFidTypeTime = MAPCHART_TIMETYPE_CME;
			
			m_szFidTypeDate = STR_MAPCHART_DATETYPE_CME;
			m_szFidTypeTime = STR_MAPCHART_TIMETYPE_CME;
		}

		if (m_nVer <= 5)
		{
			m_bDirectSend = FALSE;
			trQry.m_strID = _T("Only Output TR");
			trQry.m_OutPack.Serialize(ar);
			m_arrTrQuery.Add(trQry);
		}
		else
		{
			ar >> nCount;
			if (m_nTRType)
			{
				for (i=0; i<nCount; i++)
				{
					trQry.RemoveAll();
					trQry.Serialize(ar);
					m_arrTrQuery.Add(trQry);
				}
			}
			else
			{
				for (i=0; i<nCount; i++)
				{
					fidQry.RemoveAll();
					fidQry.Serialize(ar);
					if (m_bDirectSend)
						m_arrFidQuery.Add(fidQry);
				}
				if (!m_bDirectSend)
					fidQry.RemoveAll();
			}
		}

		ar >> nCount;
		for (i=0; i<nCount; i++)
		{
			ar >> strKey;
			ar >> strData;
			m_propMap.SetAt(strKey, strData);
		}

		MakeInboundFIDLayout();

//		if (m_nVer <= 5)
//		{
		if (!m_nTRType && !m_bDirectSend)
		{
			CString strName, strFID, strMajor;
			CFidItem *pItem = NULL;
			int nSize = m_prop.GetPacketSize();
			CFidBlock *pBlock = new CFidBlock;
			pBlock->m_strBlockName = _T("Single Block");
			pBlock->m_bArray = TRUE;
			pBlock->m_nReqRow = m_nCnt;
			fidQry.m_OutPack.AddBlock(pBlock);
			fidQry.m_OutPack.SetGridSymbol(m_strGridSymbol);	// 그리드 심볼을 넣어야 한다.
			strMajor.Format(_T("%03d"), atoi(pBlock->m_strGridSym) / 1000);
			fidQry.m_strID = _T("FID Query");

			ST_CHART_PACKET stPack;
			for (i=0; i<nSize; i++)
			{
				CFidItem *pItem = new CFidItem;
				stPack = m_prop.GetPacket(i);

				strFID = strMajor + stPack.szFID;
				pItem->SetItemID(strFID);
				pItem->SetSize(stPack.nPacketSize);
				pItem->SetChartID(stPack.szPacketName);
				pItem->SetRealID(stPack.szRealFID);

				fidQry.m_OutPack.AddItem(_T("Single Block"), pItem);
			}
			m_prop.SetABS(fidQry.m_OutPack.GetRealABS());

			m_arrFidQuery.Add(fidQry);
		}
//		}

		if (m_nTRType == 1)
		{
			// Set Default Use Buffer to FALSE.
			int nSize = m_outTrQry.GetItemCount();
			for (i=0; i<nSize; i++)
				m_outTrQry.GetItemAt(i)->SetUseBuf(FALSE);

			CString strID;
			//	Set Use Buffer to TRUE.
			ST_CHART_PACKET stPack;
			nSize = m_prop.GetPacketSize();
			for (i=0; i<nSize; i++)
			{
				stPack = m_prop.GetPacket(i);
				if (atoi(stPack.szRealFID) == m_nFidTypeDate)		m_strDateName = stPack.szPacketName;
				else if (atoi(stPack.szRealFID) == m_nFidTypeTime)	m_strTimeName = stPack.szPacketName;

				m_outTrQry.SetUseBufAtItem(stPack.szFID, TRUE);
			}
		}

		InitDateTimeFID();
	}
}

void CWinixConverter::InitDateTimeFID()
{
	return;//KHD: IBK에서 필요한가? 
	if (!IsRunMode())
		return;

	if (m_nTRType == 1)
		return;

	BOOL bDate = FALSE, bTime = FALSE;
	int nID, nBCnt, nICnt, i, j, k;
	int nSize = m_arrFidQuery.GetSize();
	CFidQuery fidQry;
	CString strID;
	CFidBlock* pBlock = NULL;
	CFidItemArr* pItem = NULL;
	for (i=0; i<nSize; i++)
	{
		fidQry = m_arrFidQuery.GetAt(i);

		nBCnt = fidQry.m_OutPack.GetBlockSize();
		for (j=0; j<nBCnt; j++)
		{
			pBlock = fidQry.m_OutPack.GetBlockAt(j);
			if (!pBlock->IsArray())
				continue;

			bDate = FALSE;
			bTime = FALSE;

			nICnt = pBlock->GetItemSize();
			for (k=0; k<nICnt; k++)
			{
				pItem = (CFidItemArr*)pBlock->GetItemAt(k);
				nID = atoi(pItem->GetItemID());
				nID = nID%1000;

				if (nID == m_nFidTypeDate)		bDate = TRUE;
				else if (nID == m_nFidTypeTime)	bTime = TRUE;
			}

			nID = atoi(pBlock->m_strGridSym) / 1000;
			if (!bDate)
			{
				pItem = new CFidItemArr;
				strID.Format(_T("%06d"), nID*1000 + m_nFidTypeDate);
				pItem->SetItemID(strID);
				pBlock->AddItem(pItem);
			}
			if (!bTime)
			{
				pItem = new CFidItemArr;
				strID.Format(_T("%06d"), nID*1000 + m_nFidTypeTime);
				pItem->SetItemID(strID);
				pBlock->AddItem(pItem);
			}
		}

		m_arrFidQuery.SetAt(i, fidQry);
	}
}

void CWinixConverter::RemoveAll()
{
	m_propMap.RemoveAll();
	m_arrTrQuery.RemoveAll();
	m_arrFidQuery.RemoveAll();
}

void CWinixConverter::GetLineData(CString strData, CStringArray *pArr)
{
	CStringArray arrData;
	CString strFind;
	int nFind = strData.Find(_T("\r\n"));
	while (nFind>=0)
	{
		strFind = strData.Left(nFind);
		pArr->Add(strFind);
		strData = strData.Right(strData.GetLength()-nFind-2);
		nFind = strData.Find(_T("\r\n"));
	}

	if (strData.GetLength() > 2)
		pArr->Add(strData);
}

void CWinixConverter::GetDataPropArr(CString strValue, int nSel, CStringArray &arr)
{
	CString strLeft, strData;
	CStringArray arrLine;
	GetLineData(strValue, &arrLine);

/*---------------------------------------
	0;보조일자;;;YYYYMMDD;무;
	1;자료일자;302(043);8;YYYYMMDD;무;
	1;시가;029;;× 1;무;
	1;고가;030;;× 1;무;
	1;저가;031;;× 1;무;
	1;종가;023;10;× 1;무;
	1;기본거래량;027;10;× 1;무;
---------------------------------------*/
	int nFind=0, nFind2=0, nIndex = 0;
	int nSize = arrLine.GetSize();
	for (int i=0; i<nSize; i++)
	{
		nIndex = 0;
		strData = arrLine.GetAt(i);
		nFind = strData.Find(_T(";"));
		while (nFind >= 0)
		{
//			1;자료일자;302(043);8;YYYYMMDD;무;			//
			strLeft = strData.Left(nFind);
			strData = strData.Right(strData.GetLength()-nFind-1);
			if ((nIndex == 0) && (atoi(strLeft) != 1))
				break;

			if (nIndex == nSel)
				arr.Add(strLeft);

			nIndex++;
			nFind = strData.Find(_T(";"));
		}
	}
}

char* CWinixConverter::MakeInboundFIDLayout()
{
	CString strData, strValue;

	const char* szValue=NULL;
	m_propMap.Lookup(_T("PO"), szValue);
	strValue.Format(_T("%s"), szValue);

	CStringArray arrLine;
	GetLineData(strValue, &arrLine);

	m_prop.m_bReal = FALSE;

	m_prop.RemoveAllPacket();
	int nSize = arrLine.GetSize();
	for (int i=0; i<nSize; i++)
	{
		strValue = arrLine.GetAt(i);
		m_prop.AddPacket(strValue, m_nVer);
	}

	m_prop.ResetPacketName();

	return _T("");
}

/*
char* CWinixConverter::MakeInboundFIDLayout()
{
	CString strFID, strLeft, strReal, strData, strValue;

	const char* szValue=NULL;
	m_propMap.Lookup(_T("PO"), szValue);
	strValue.Format(_T("%s"), szValue);

	CStringArray arrLine;
	GetLineData(strValue, &arrLine);

	m_arrFID.RemoveAll();
	m_arrPacketSize.RemoveAll();
	m_arrReal.RemoveAll();
	m_arrIsReal.RemoveAll();

	m_arrName.RemoveAll();
	m_bIsReal = FALSE;

//---------------------------------------
//	0;보조일자;;;YYYYMMDD;무;
//	1;자료일자;302(043);8;YYYYMMDD;무;
//	1;시가;029;;× 1;무;
//	1;고가;030;;× 1;무;
//	1;저가;031;;× 1;무;
//	1;종가;023;10;× 1;무;
//	1;기본거래량;027;10;× 1;무;
//-------------------------------------
	int nFind=0, nFind2=0, nIndex = 0;
	int nSize = arrLine.GetSize();
	for (int i=0; i<nSize; i++)
	{
		nIndex = 0;
		strValue = arrLine.GetAt(i);
		nFind = strValue.Find(_T(";"));
		while (nFind >= 0)
		{
//			1;자료일자;302(043);8;YYYYMMDD;무;			//
			strLeft = strValue.Left(nFind);
			strValue = strValue.Right(strValue.GetLength()-nFind-1);
			if ((nIndex == 0) && (atoi(strLeft) != 1))
				break;

			if (nIndex == 1)
				m_arrName.Add(strLeft);
			else if (nIndex == 2)
			{
				int nFind2 = strLeft.Find(_T("("));
				if (nFind2 >= 0)
				{
					strData = strLeft.Left(nFind2);
					m_arrFID.Add(strData);

					strLeft = strLeft.Right(strLeft.GetLength()-nFind2-1);
					nFind2 = strLeft.Find(_T(")"));
					if (nFind2 >= 0)
						strLeft = strLeft.Left(nFind2);

					m_arrReal.Add(strLeft);
				}
				else
				{
					m_arrReal.Add(strLeft);
					m_arrFID.Add(strLeft);
				}
			}
			else if (nIndex == 3)
				m_arrPacketSize.Add(atoi(strLeft));
			else if (nIndex == 5)
			{
				if (strLeft.Compare(_T("유"))==0)
				{
					m_arrIsReal.Add(1);
					m_bIsReal = TRUE;
				}
				else
					m_arrIsReal.Add(0);
			}

			nIndex++;
			nFind = strValue.Find(_T(";"));
		}
	}

	return _T("");
}
*/

void CWinixConverter::SetPropMap(IPropertyMap* pPropMap)
{
	m_propMap.RemoveAll();

	CString strKey, strData;
	const char *szKey=NULL, *szData=NULL;
	POSITION pos = pPropMap->GetStartPosition();
	while (pos)
	{
		pPropMap->GetNextAssoc(pos, szKey, szData);
		strKey.Format(_T("%s"), szKey);
		strData.Format(_T("%s"), szData);
		m_propMap.SetAt(strKey, strData);
	}
}

BOOL CWinixConverter::ConvReceiveData(char* pszChartData, CChartProp *pProp)
{
	if (m_nTRType == 1)
	{
		int nRowSize = GetRowPacketSize();
		if (m_nSortType)
			m_outTrQry.SetSortType(m_nSortType);

		CStringArray arrPacketName;
		CArray<int, int> arrPacketSize;
		ST_CHART_PACKET stPack;
		int nCnt = pProp->GetPacketSize();
		for (int k=0; k<nCnt; k++)
		{
			stPack = pProp->GetPacket(k);
			arrPacketName.Add(stPack.szFID);
			arrPacketSize.Add(stPack.nPacketSize);
		}

		m_outTrQry.GetDataBuffer(pszChartData, m_nRowCount*nRowSize, &arrPacketName, &arrPacketSize);
	}
	else
	{
		BOOL bReverse = m_nSortType;

		int nVSize=0;
		int nMinor = atoi(m_strMinorFID);
		int nCnt=0, nBuffSize=0, i, j, k;
		int nDateIdx=0, nTimeIdx=0, nValueIdx=0;
		CString strID, strValue, strMajor;
		CFidBlock* pBlock = NULL;
		CFidItem* pItem = NULL;
		int nCount = m_curFidQry.m_OutPack.GetBlockSize();
		for (i=0; i<nCount; i++)
		{
			pBlock = m_curFidQry.m_OutPack.GetBlockAt(i);
			bReverse = m_curFidQry.m_OutPack.GetSortType();
			if (!pBlock)			continue;
			if (!pBlock->IsArray())	continue;

			if (pProp == NULL)
				pProp = &m_prop;

			//	속도 향상을 위한 ---------------------->>
			CArrFidItem arrItem;
			nDateIdx=0; nTimeIdx=0;
			ST_CHART_PACKET stPack;
			nCnt = pProp->GetPacketSize();
			for (k=0; k<nCnt; k++)
			{
				stPack = pProp->GetPacket(k);
				pItem = pBlock->GetItemNameAt(stPack.szPacketName);

				arrItem.Add(pItem);
			}
			strMajor.Format(_T("%03d"), atoi(pBlock->m_strGridSym)/1000);
			strID = strMajor + m_szFidTypeDate;
			CFidItem* pDateItem = pBlock->GetItemAt(strID);
			strID = strMajor + m_szFidTypeTime;
			CFidItem* pTimeItem = pBlock->GetItemAt(strID);
			//	---------------------------------------<<

			for (j=0; j<m_nRowCount; j++)
			{
				if (m_nSortType || bReverse)	nValueIdx = m_nRowCount-j-1;
				else							nValueIdx = j;

				nCnt = arrItem.GetSize();
				for (k=0; k<nCnt; k++)
				{
					stPack = pProp->GetPacket(k);
					pItem = arrItem.GetAt(k);

					if ((stPack.nPacketSize == 8) && pDateItem && pTimeItem && (pItem==pDateItem || pItem==pTimeItem))
					{
						strValue = pDateItem->GetValue(nValueIdx) + pTimeItem->GetValue(nValueIdx);
						switch (nMinor)
						{
						case 310:
						case 411:
						case 511:
						case 410:
							strValue = strValue.Mid(6, stPack.nPacketSize);
							break;
						case 510:
							strValue = strValue.Mid(4, stPack.nPacketSize);
							break;
						case 610:
						case 710:
						case 810:
							strValue = strValue.Left(stPack.nPacketSize);
							break;
						default:
							strValue = strValue.Left(stPack.nPacketSize);
							break;
						}
					}
					else
					{
						if (pItem)
							strValue = pItem->GetValue(nValueIdx);
						else
							strValue = "";
					}

					nVSize = strValue.GetLength();

					if (nVSize > stPack.nPacketSize)
						nVSize = stPack.nPacketSize;

					memcpy(pszChartData+nBuffSize, (LPTSTR)(LPCTSTR)strValue, nVSize);
					nBuffSize += stPack.nPacketSize;
				}
			}
		}
	}

	return TRUE;
}

void CWinixConverter::ParsingDataToArr(const char* pszData, int nSize)
{
	m_arrFIDOut.RemoveAll();

	if (m_nTRType == 0)	ParsingFID(pszData, nSize);
	else				ParsingTR(pszData, nSize);
}

void CWinixConverter::ParsingTR(const char* pszData, int nSize)
{
	m_outTrQry.SetChartProp( m_prop );
	m_outTrQry.ReceiveData(pszData, nSize);

	if (m_prop.GetPacketSize() > 0)
	{
		m_strNextKey = m_outTrQry.GetNextKey();

		//<--@IMSI-alzioyes
		m_nRowCount = m_outTrQry.m_nTRDataCnt;
	}
}

void CWinixConverter::ParsingFID(const char* pszData, int nSize)
{
	m_strLastDate[0].Empty();
	m_strLastDate[1].Empty();

	CString strData;
	char szData[1024];
	memset(szData, 0x00,  sizeof(szData));

	char *pszBuf = new char[nSize+1];
	memcpy(pszBuf, pszData, nSize);
	pszBuf[nSize] = 0x00;

	char *pszBuffer = pszBuf;

	BOOL bGrid = FALSE;
	m_nRowCount = 0;
	int nIndex = 0, nColIdx=0;
	while (pszBuffer < pszBuf+nSize)
	{
		switch (pszBuffer[0])
		{
		case sFS:
		case sSUB:
			strData = _T("");
			if (nIndex > 0)
				strData.Format(_T("%.*s"), nIndex, szData);

			m_arrFIDOut.Add(strData);

			bGrid = FALSE;
			memset(szData, 0x00, sizeof(szData));
			nIndex = 0;

			break;
		case sROW:
			m_nRowCount++;
		case sCELL:
			if (!bGrid)
			{
				memcpy(&m_OutGrid, szData, sizeof(m_OutGrid));
				char szTemp[1024];
				memset(szTemp, 0x00, sizeof(szTemp));
				memcpy(szTemp, szData+sizeof(m_OutGrid), 1024-sizeof(m_OutGrid));
				memset(szData, 0x00, sizeof(szData));
				memcpy(szData, szTemp, sizeof(szTemp));

				nIndex -= sizeof(m_OutGrid);
				bGrid = TRUE;
			}

			strData = _T("00000000");
			if (nIndex>0)
				strData.Format(_T("%.*s"), nIndex, szData);

//			if (strData.Compare(_T("장마감")) == 0)
//				strData = _T("8888");

			m_arrFIDOut.Add(strData);

			memset(szData, 0x00, sizeof(szData));
			nIndex = 0;

			break;
		default :
			szData[nIndex++] = pszBuffer[0];
			break;
		}

		pszBuffer++;
	}
	delete []pszBuf;

//---------------------------------------------------------
//	Chart Default Data Convert.	---------------------------
	int nOutSize = m_arrFIDOut.GetSize();

	CFidItem *pItem = NULL;
	CFidBlock *pBlock = GetDefaultFIDBlock(atoi(m_strMajorFID));

	if (pBlock && (nOutSize > 0))
		m_strCurCode = m_arrFIDOut.GetAt(0);

	m_defFidQry.m_OutPack.RemoveAllData();

	char cType = ' ';
	CString strGB, strRes;
	int nPcnt;
	double dRate;
	int nFind = 0;
	nIndex = 0;
	if (pBlock)
	{
		int nCount = pBlock->GetItemSize();
		for (int i=0; i<nCount; i++)
		{
			if (nIndex >= nOutSize)
				break;

			strData = m_arrFIDOut.GetAt(nIndex++);
			pItem = pBlock->GetItemAt(i);
			if (pItem->GetABS())
			{
				nFind = strData.Find(_T("-"), 0);
				if (nFind >= 0)
					strData.Delete(nFind);
			}
			pItem->SetValue(strData);
		}

		if (GetChartType() == 1)
		{
			pItem = pBlock->GetItemNameAt(_T("가격표시구분"));
			if (pItem)
			{
				strGB = pItem->GetValue();
				cType = GetDecimalRate(strGB, nPcnt, dRate, strRes);
				CWinixConverter::SetCodeType(m_strCurCode, strGB);
			}
		}
		else if(GetChartType() == 2)
		{
			pItem = pBlock->GetItemNameAt(_T("소숫점자리"));
			if (pItem)
			{
				strGB = pItem->GetValue();
				int nType = atoi(strGB);
				if( nType == 0 )
					m_strPacketType = "";
				else
					m_strPacketType.Format("x %.*f", strGB.GetLength()-1, (double)1/(double)nType); 
			}
		}
	}


	int nCount = m_curFidQry.m_OutPack.GetBlockSize();
	for (int i=0; i<nCount; i++)
	{
		pBlock = m_curFidQry.m_OutPack.GetBlockAt(i);
		if (!pBlock)
			continue;

		if (nIndex >= nOutSize)			break;

		if (!pBlock->IsArray())
		{
			int nCnt = pBlock->GetItemSize();
			for (int k=0; k<nCnt; k++)
			{
				if (nIndex >= nOutSize)		break;

				strData = m_arrFIDOut.GetAt(nIndex++);

				pItem = pBlock->GetItemAt(k);
				if (pItem->GetABS())
				{
					nFind = strData.Find(_T("-"), 0);
					if (nFind >= 0)
						strData.Delete(nFind);
				}
				pItem->SetValue(strData);
			}
		}
		else
		{
			if (!m_strCurCode.IsEmpty() && m_pParent)
			{
				CString strKey = m_strCurCode + "\t" + m_curFidQry.m_strID;
				m_pParent->SendMessage(WMU_SETNEXTKEY, (WPARAM)(LPCTSTR)strKey, (LPARAM)&m_OutGrid);
			}

			for (int j=0; j<m_nRowCount; j++)
			{
				int nCnt = pBlock->GetItemSize();
				for (int k=0; k<nCnt; k++)
				{
					if (nIndex >= nOutSize)		break;

					strData = m_arrFIDOut.GetAt(nIndex++);

					pItem = pBlock->GetItemAt(k);
					if (pItem->GetABS())
					{
						nFind = strData.Find(_T("-"), 0);
						if (nFind >= 0)
							strData.Delete(nFind);
					}

					//>> 마지막 날자정보 셋팅.
					if(m_nSortType==0)
					{
						if(j==m_nRowCount-1) 
						{
							if(pItem->GetRealID().Compare(m_szFidTypeDate)==0)
							{
								m_strLastDate[0] = strData;
							}

							CString strID = pItem->GetItemID().Right(3);
							if(strID.Compare(m_szFidTypeTime)==0)
							{
								m_strLastDate[1] = strData;
							}
						}
					}
					else
					{
						if(m_strLastDate[0].IsEmpty())
						{
							if(pItem->GetRealID().Compare(m_szFidTypeDate)==0)
							{
								m_strLastDate[0] = strData;
							}
						}

						if(m_strLastDate[1].IsEmpty())
						{
							CString strID = pItem->GetItemID().Right(3);
							if(strID.Compare(m_szFidTypeTime)==0)
							{
								m_strLastDate[1] = strData;
							}						
						}
					}
					//<<

					if (GetChartType() == 1)
					{
						CString strID = pItem->GetItemID().Right(3);
						if (strID.Compare(m_szFidTypeDate) && strID.Compare(m_szFidTypeTime) &&
							strID.Compare(_T("027")) && strID.Compare(_T("028")) &&
							strID.Compare(_T("032")) && strID.Compare(_T("033")) &&
							strID.Compare(_T("035")))
						{
							if (!TbondPrcLToF(cType, nPcnt, dRate, strData))
								strData.Format(_T("%lf"), DataSeparation(strData, nPcnt));
						}
					}
					((CFidItemArr*)pItem)->SetValue(strData, j);
				}
			}
		}
	}
//---------------------------------------------------------
}

//vntsorl_20110420:[A00000410] 맵차트에서 첫리얼에 새로운 봉이 생기는 오류수정
int CWinixConverter::GetBozoMsg(CString &strBozo, int nPacketType)
{
	if (m_pBozoMng == NULL)
	{
		m_pBozoMng = (IChartBozoMsgManager*)AfxGetPctrInterface(UUID_IChartBozoMsgManager);
		if (m_pBozoMng == NULL)
		{
			AfxMessageBox(_T("Not found Interface \"IChartBozoMsgMng\""));
			return -1;
		}
	}

	int nLeft, nRight;
	nLeft = atoi(m_strMajorFID);
	nRight = atoi(m_strMinorFID);

	CFidBlock *pBlock = GetDefaultFIDBlock(atoi(m_strMajorFID));
	if (pBlock == NULL)
		return -1;

	CString strPrefix = "";
	if (GetChartType() == 2)
	{
		if( GetFXChartType() == 0 )			strPrefix = _T("매도");
		else if( GetFXChartType() == 1 )	strPrefix = _T("매수");
	}
	//>>
	CChartBozoInput bozoInput;
	CFidItem *pItem = pBlock->GetItemNameAt(_T("현재가"), strPrefix);
	if (pItem)	bozoInput.m_sPrice = pItem->GetValue();
	pItem = pBlock->GetItemNameAt(_T("거래량"));
	if (pItem)	bozoInput.m_sVolume = pItem->GetValue();
	pItem = pBlock->GetItemNameAt(_T("상한가"));
	if (pItem)	bozoInput.m_sUpLmtPrice = pItem->GetValue();
	pItem = pBlock->GetItemNameAt(_T("하한가"));
	if (pItem)	bozoInput.m_sDnLmtPrice = pItem->GetValue();
	pItem = pBlock->GetItemNameAt(_T("시가"), strPrefix);
	if (pItem)	bozoInput.m_sOpen = pItem->GetValue();
	pItem = pBlock->GetItemNameAt(_T("고가"), strPrefix);
	if (pItem)	bozoInput.m_sHigh = pItem->GetValue();
	pItem = pBlock->GetItemNameAt(_T("저가"), strPrefix);
	if (pItem)	bozoInput.m_sLow = pItem->GetValue();
	pItem = pBlock->GetItemNameAt(_T("전일종가"), strPrefix);
	//test
	//-->
	//Org
	if (pItem)	bozoInput.m_sPreprice = pItem->GetValue();

	//test Routine.
	//if (pItem)	bozoInput.m_sPreprice = ""; pItem->GetValue();
	//<--
	pItem = pBlock->GetItemNameAt(_T("전일거래량"));
	if (pItem)	bozoInput.m_sPreVolume = pItem->GetValue();
	pItem = pBlock->GetItemNameAt(_T("예상장시작시간"));
	if (pItem)
		bozoInput.m_sStartTime = pItem->GetValue();
	pItem = pBlock->GetItemNameAt(_T("거래시작시간"));
	if (pItem)
	{
		CString szTmp = pItem->GetValue();
		if (szTmp.Compare("0000") != 0)
			bozoInput.m_sStartTime.Empty();
	}

	bozoInput.m_sDataType = _T("0");		//	고정. 차후 필요시 수정.
	bozoInput.m_sMarket = _T("1");			//	고정. 차후 필요시 수정.

	pItem = pBlock->GetItemNameAt(_T("종목코드"));
	if (pItem)
		bozoInput.m_sShcode = m_strCurCode = pItem->GetValue();

	//	Next 여부.
	char szTemp[5];
	memset(szTemp, 0x00, sizeof(szTemp));
	memcpy(szTemp, m_OutGrid.page, sizeof(m_OutGrid.page));
	if (atoi(szTemp) > 1)
		bozoInput.m_sNxtFlg = _T("1");
	else
		bozoInput.m_sNxtFlg = _T("0");

	CString strTickFID, strTemp;
	int nTerm = GetUnitValue(nRight);
	if (nTerm > 1 && nTerm < 5)
		bozoInput.m_sTick = _T("1");
	else
	{
		strTemp = CFidQuery::GetTickFID(nRight);
		strTickFID.Format(_T("%03d%s"), nLeft, strTemp);
		bozoInput.m_sTick = m_curFidQry.m_InPack.GetDataAt(strTickFID);
	}

	if (bozoInput.m_sTick.IsEmpty())
		bozoInput.m_sTick = _T("1");

	bozoInput.m_sTerm.Format(_T("%d"), nTerm);

//	if (m_nTRType)	nLeft = -1;

	char szKey[4];
	memset(szKey, 0x00, sizeof(szKey));
	sprintf(szKey, _T("%03d"), nLeft);

	int nRet = 0;
	int nChartType = GetChartType();
	if ( nChartType == 1)
	{
		CString strTimeIdx, strRate, strSDate, strEDate, strName;
		CString strMType, strMDay, strDecimalGB, strBozoTail, strRes;
		int nPcnt;
		double dRate;
		pItem = pBlock->GetItemNameAt(_T("가격표시구분"));
		if (pItem)	strDecimalGB = pItem->GetValue();
		if( strDecimalGB.IsEmpty() )		return nRet;

		pItem = pBlock->GetItemNameAt(_T("장시작시간(KST)"));
		if (pItem)	strSDate = pItem->GetValue();
		pItem = pBlock->GetItemNameAt(_T("장종료시간(KST)"));
		if (pItem)	strEDate = pItem->GetValue();
		pItem = pBlock->GetItemNameAt(_T("장구분"));
		if (pItem)	strMType = pItem->GetValue();
		pItem = pBlock->GetItemNameAt(_T("장시작일"));
		if (pItem)	strMDay = pItem->GetValue();
		pItem = pBlock->GetItemNameAt(_T("종목명"));
		if (pItem)	strName = pItem->GetValue();

		char cType = CWinixConverter::GetDecimalRate(strDecimalGB, nPcnt, dRate, strRes);
		CWinixConverter::SetCodeType(m_strCurCode, strDecimalGB);

		strTimeIdx.Format("%s", CWinixConverter::GetTimeIndexInfo(22, 1));	//@Solomon	서울시간대로 무조건 들어 온다.
		if (cType >= 'A' && cType <= 'K')
		{
			if (cType <= 'C')
				dRate = dRate * 100;
		}

		if(nPcnt>0) strRate.Format(_T("%.*f"), nPcnt, dRate);
		else		strRate.Format(_T("%f"), dRate);

		strSDate = strSDate + "|" + strEDate;
		strName = bozoInput.m_sShcode + ":" + strName;

		List_CChartBozoItem arr;
		CChartBozoItem *pItem;
		pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("TIMEDIFF"), strTimeIdx);	arr.AddTail(pItem);
		pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("MARKETTIME"), strSDate);	arr.AddTail(pItem);
//		pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("DATATIME"), strLastDate);	arr.AddTail(pItem);
		pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("CODEANDNAME"), strName);	arr.AddTail(pItem);
		pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("MARKETTYPE"), strMType);	arr.AddTail(pItem);
		pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("MARKETDAY"), strMDay);		arr.AddTail(pItem);
		pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("HBPRICE"), strRate);		arr.AddTail(pItem);

		if (cType >= 'A' && cType <= 'K')
		{
			CString strTDC;
			strTDC.Format(_T("%f:_Open_,|%f:_High_,|%f:_Low_,|%f:_Close_,"), 1.0, 1.0, 1.0, 1.0);
			pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("TDC"), strTDC);				arr.AddTail(pItem);
			pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("RDC"), strTDC);				arr.AddTail(pItem);
		}

//vntsorl_20110420:[A00000410] 맵차트에서 첫리얼에 새로운 봉이 생기는 오류수정
		bozoInput.nPacketType = nPacketType;
		CString strLastDataTime( GetLastDate(0) + GetLastDate(1).Left(6));
		bozoInput.m_sLastDataTime = strLastDataTime;
		nRet = m_pBozoMng->GetChartBozoMsg(szKey, &bozoInput, strBozo, nPcnt);
		nRet = m_pBozoMng->ChangeChartBozoMsg(strBozo, &arr);

		POSITION pos = arr.GetHeadPosition();
		while (pos)
		{
			pItem = arr.GetNext(pos);
			if (pItem)
			{
				delete pItem;
				pItem = NULL;
			}
		}
		arr.RemoveAll();
	}
	//<<20100310_JS.Kim FX.  매도/매수 패킷을 일반 시/고/저/종 패킷명을 Redefine 하는 보조메세지를 셋팅한다.
	else if (nChartType == 2)
	{
		CString strDefPacket;
		if( GetFXChartType() == 0 )		//매도
		{
			strDefPacket = _T(":_BuyClose_|:_BuyOpen_|:_BuyHigh_|:_BuyLow_|_Close_:_SellClose_|_Open_:_SellOpen_|_High_:_SellHigh_|_Low_:_SellLow_|");
		}
		else
		{
			strDefPacket = _T(":_SellClose_|:_SellOpen_|:_SellHigh_|:_SellLow_|_Close_:_BuyClose_|_Open_:_BuyOpen_|_High_:_BuyHigh_|_Low_:_BuyLow_|");
		}

		List_CChartBozoItem arr;
		CChartBozoItem *pItem;
		pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("REDEFPACKET"), strDefPacket);	arr.AddTail(pItem);

//vntsorl_20110420:[A00000410] 맵차트에서 첫리얼에 새로운 봉이 생기는 오류수정
		bozoInput.nPacketType = nPacketType;
		nRet = m_pBozoMng->GetChartBozoMsg(szKey, &bozoInput, strBozo, 1);
		nRet = m_pBozoMng->ChangeChartBozoMsg(strBozo, &arr);

		POSITION pos = arr.GetHeadPosition();
		while (pos)
		{
			pItem = arr.GetNext(pos);
			if (pItem)
			{
				delete pItem;
				pItem = NULL;
			}
		}
		arr.RemoveAll();
	}
	else if(nChartType == 3)
	{
		CString strTimeIdx, strSDate, strEDate, strName;
		CString strMType, strMDay, strDecimalGB, strBozoTail, strRes;
		CString strPrefix = "";
		int nPcnt;
		double dRate;

 		pItem = pBlock->GetItemNameAt(_T("장시작"));
 		if (pItem)
		{
			strSDate = pItem->GetValue();
			strSDate += _T("00");
		}
 		pItem = pBlock->GetItemNameAt(_T("장종료"));
 		if (pItem)
		{
			strEDate = pItem->GetValue();
			strEDate += _T("00");
		}

// 		pItem = pBlock->GetItemNameAt(_T("장구분"));
// 		if (pItem)	strMType = pItem->GetValue();
// 		pItem = pBlock->GetItemNameAt(_T("장시작일"));
// 		if (pItem)	strMDay = pItem->GetValue();
		pItem = pBlock->GetItemNameAt(_T("종목명"));
		if (pItem)
		{
			strName = pItem->GetValue();
			strName.TrimRight();
		}

		pItem = pBlock->GetItemNameAt(_T("전일종가"), strPrefix);
		if (pItem)
		{
			bozoInput.m_sPreprice = pItem->GetValue();
		}

		char cType = CWinixConverter::GetDecimalRate(strDecimalGB, nPcnt, dRate, strRes);
		CWinixConverter::SetCodeType(m_strCurCode, strDecimalGB);

		strTimeIdx.Format("%s", CWinixConverter::GetTimeIndexInfo(22, 1));
		if (cType >= 'A' && cType <= 'K')
		{
			if (cType <= 'C')
				dRate = dRate * 100;
		}
		
		strName = bozoInput.m_sShcode + ":" + strName;
		
		CString szMarketTime = "180000|050000";
		CString szBaseTime = "180000";
		if(strSDate.GetLength()>0)
		{
			szMarketTime.Format("%s|%s", strSDate, strEDate);
			szBaseTime = strSDate;

			bozoInput.m_sStartTime = szBaseTime;
			bozoInput.m_szMarketTime = szMarketTime;
			bozoInput.m_szBaseTime = szBaseTime;
		}

		List_CChartBozoItem arr;
		CChartBozoItem *pItem;
		bozoInput.nPacketType = nPacketType;
		CString strLastDataTime( GetLastDate(0) + GetLastDate(1).Left(6));
		bozoInput.m_sLastDataTime = strLastDataTime;
		nRet = m_pBozoMng->GetChartBozoMsg(szKey, &bozoInput, strBozo, nPcnt);
		nRet = m_pBozoMng->ChangeChartBozoMsg(strBozo, &arr);

		POSITION pos = arr.GetHeadPosition();
		while (pos)
		{
			pItem = arr.GetNext(pos);
			if (pItem)
			{
				delete pItem;
				pItem = NULL;
			}
		}
		arr.RemoveAll();
	}
	//>>
	else
	{
		bozoInput.nPacketType = nPacketType;
		nRet = m_pBozoMng->GetChartBozoMsg(szKey, &bozoInput, strBozo, 1);
	}

	return nRet;
}

int CWinixConverter::GetBozoMsgFromTr(CString &strBozo)
{
	int nRet = 0;
	if (m_pBozoMng == NULL)
	{
		m_pBozoMng = (IChartBozoMsgManager*)AfxGetPctrInterface(UUID_IChartBozoMsgManager);
		if (m_pBozoMng == NULL)
		{
			AfxMessageBox(_T("Not found Interface \"IChartBozoMsgMng\""));
			return -1;
		}
	}

	CString strBozeInfo;
	int nSize = m_outTrQry.GetChartBozoInfo(strBozeInfo);
	strBozeInfo += ";";

	OutputDebugString("[DRCS] Chart BozeMessage Info Start ..........\n");
	CString szDebug;

	CMapStringToString strMapString;	
	int nPos=0, nPosIndex=0, nPosSub;
	CString strValue, strValue1, strValue2;
	while(nPos>=0)
	{
		nPos = strBozeInfo.Find(";", nPosIndex);
		if(nPos==-1) break;

		strValue = strBozeInfo.Mid(nPosIndex, nPos-nPosIndex);
		nPosIndex = nPos + 1;
		//strBozeInfo = strBozeInfo.Mid(nPos+1);

		nPosSub = strValue.Find("=");
		if(nPosSub >=0)
		{
			strValue1 = strValue.Left(nPosSub);
			strValue2 = strValue.Mid(nPosSub+1);
			strValue2.TrimRight();
			//strValue2.Remove(';');
			
			strMapString.SetAt(strValue1,strValue2);
			szDebug.Format("[DRCS] (%s)=(%s)\n", strValue1, strValue2);
			OutputDebugString(szDebug);
		}
	}	
	OutputDebugString("[DRCS] Chart BozeMessage Info End ..........\n");

	CChartBozoInput bozoInput;
	CString strMarketName, strRealState, strMarketEndTime, strMarketStartTime, strMarketTime, strBaseTime, strNTick, strTDC, strRDC, strNxt;

	strMapString.Lookup(_T("TDC")			,strTDC);
	strMapString.Lookup(_T("RDC")			,strRDC);
	strMapString.Lookup(_T("조회주기")		,bozoInput.m_sTerm);
	strMapString.Lookup(_T("마켓구분")		,strMarketName);	
	strMapString.Lookup(_T("종목코드")		,bozoInput.m_sShcode);
	strMapString.Lookup(_T("현재가")		,bozoInput.m_sPrice);
	strMapString.Lookup(_T("누적거래량")	,bozoInput.m_sVolume);
	strMapString.Lookup(_T("상한가")		,bozoInput.m_sUpLmtPrice);
	strMapString.Lookup(_T("하한가")		,bozoInput.m_sDnLmtPrice);
	strMapString.Lookup(_T("시가")			,bozoInput.m_sOpen);
	strMapString.Lookup(_T("고가")			,bozoInput.m_sHigh);
	strMapString.Lookup(_T("저가")			,bozoInput.m_sLow);
	strMapString.Lookup(_T("전일종가")		,bozoInput.m_sPreprice);
	strMapString.Lookup(_T("전일거래량")	,bozoInput.m_sPreVolume);
	strMapString.Lookup(_T("거래시작시간")	,strMarketStartTime);
	strMapString.Lookup(_T("거래종료시간")	,strMarketEndTime);
	strMapString.Lookup(_T("장구분")		,strRealState);
	strMapString.Lookup(_T("NTick")			,strNTick);
	strMapString.Lookup(_T("연속조회")		,strNxt);
	
	bozoInput.m_sDataType = _T("0");		//	고정. 차후 필요시 수정.
	m_strCurCode = bozoInput.m_sShcode;
	
	int nTerm = atoi(bozoInput.m_sTerm);
	if (nTerm > 1 && nTerm < 5)
		bozoInput.m_sTick = _T("1");
	
	if (bozoInput.m_sTick.IsEmpty())
		bozoInput.m_sTick = strNTick;
	
	bozoInput.m_sTerm.Format(_T("%d"), nTerm);

	char szKey[4];
	memset(szKey, 0x00, sizeof(szKey));
	
	bozoInput.m_sMarket = GetMarkeyTypeFromName(strMarketName);
	int nMarket = atoi(bozoInput.m_sMarket);
	
	int nBojoFID = 101;
	int nPoint = 1;

	if(nMarket == 1)//주식
		nBojoFID = 101;
	else if(nMarket == 2)//업종
		nBojoFID = 20;
	else if(nMarket == 3 || nMarket == 4 || nMarket == 6)//선물, 옵션, 상품선물
	{
		// 20150909(이상신) : 주봉 마지막봉 주시작일 셋팅
		if (bozoInput.m_sTerm == "3")
		{
			CString strLastDataTime = m_outTrQry.GetLastDate();
			bozoInput.m_sLastDataTime = strLastDataTime;
		}

		nPoint = 0;
		nBojoFID = 30;
	}
	else if(nMarket == 5)//주식선물
		nBojoFID = 134;
	
	sprintf(szKey, _T("%03d"), nBojoFID);

	BOOL bSetReal = FALSE;

	//장시작전 처리.
	if(nMarket == 1 && (strRealState.Compare("10") == 0 || strRealState.Compare("11") == 0))   //주식
		bSetReal = TRUE;
	else if(nMarket == 2 && (strRealState.Compare("00") == 0 || strRealState.Compare("50") == 0)) //업종
		bSetReal = TRUE;
	else if((nMarket > 2 && nMarket < 7) && strRealState.Compare("00") == 0) //선옵,주식선물,상품선물
		bSetReal = TRUE;

	CString strResetTime = "";
	if(bSetReal)
	{
		strResetTime = strMarketStartTime;
	}
	strBaseTime = strMarketStartTime;
	strMarketTime.Format("%6.6s|%6.6s", strMarketStartTime, strMarketEndTime);

	bozoInput.m_sStartTime      = strResetTime;
	bozoInput.m_szMarketTime	= strMarketTime;	//장운영정보.
	bozoInput.m_szBaseTime		= strBaseTime;		//분봉일 때 봉의 시작기준 시간.

	bozoInput.nPacketType = 1;//nPacketType;

	// 'A' : '1', 'E' : '0'
	if( strNxt.GetAt(0) == 'A')		bozoInput.m_sNxtFlg = '1';
	else bozoInput.m_sNxtFlg = '0';

	nRet = m_pBozoMng->GetChartBozoMsg(szKey, &bozoInput, strBozo, nPoint);

	nRet = SetTDCRDC( strBozo );
	return nRet;
}

int CWinixConverter::GetUnitValue(int nMinor)
{
	int nLeft = atoi(m_strMajorFID);
	CString strTemp, strTickFID;
	//	0:틱/1:분/2:일/3:주/4:월/6:초
	switch (nMinor)
	{
	case 410:	return 0;
	case 510:	return 1;
	case 610:
	case 710:
	case 810:
	case 910:
		strTemp = CFidQuery::GetTickFID(nMinor);
		if (strTemp.IsEmpty())	return 2;

		strTickFID.Format(_T("%03d%s"), nLeft, strTemp);
		nLeft = atoi(m_curFidQry.m_InPack.GetDataAt(strTickFID));

		if (nLeft == 1)			return 2;
		else if (nLeft == 2)	return 3;
		else if (nLeft == 3)	return 4;
		else					return 2;

		break;

	case 511:	return 6;
	default:	break;
	}

	return 0;
}

long CWinixConverter::GetRowPacketSize()
{
	int nRowSize = 0;
	ST_CHART_PACKET stPack;
	int nSizeCnt = m_prop.GetPacketSize();
	for (int i=0; i<nSizeCnt; i++)
	{
		stPack = m_prop.GetPacket(i);
		nRowSize += stPack.nPacketSize;
	}

	return nRowSize;
}

CString CWinixConverter::GetPacketNameList()
{
	return m_prop.GetPackName();
}

void CWinixConverter::SetGridSymbol(CString strSymbol)
{
	m_strGridSymbol = strSymbol;

	m_strMajorFID.Format(_T("%03d"), atoi(strSymbol)/1000);
	m_strMinorFID.Format(_T("%03d"), atoi(strSymbol)%1000);
}

void CWinixConverter::SetGridSymbol2(CString strSymbol, CString strFIDCode)
{
	CFidQuery qry;
	int nIndex = GetFidQuery(strFIDCode, qry);
	if (nIndex >= 0)
	{
		qry.m_OutPack.SetGridSymbol(strSymbol);
		m_arrFidQuery.SetAt(nIndex, qry);
	}

	m_strGridSymbol = strSymbol;
	m_strMajorFID.Format(_T("%03d"), atoi(strSymbol)/1000);
	m_strMinorFID.Format(_T("%03d"), atoi(strSymbol)%1000);
}

void CWinixConverter::SetDiffGridSymbol(CString strFIDCode, CString strSymbol)
{
	CFidQuery qry;
	int nIndex = GetFidQuery(strFIDCode, qry);
	if (nIndex >= 0)
	{
		qry.m_OutPack.SetDiffGridSymbol(strSymbol);
		m_arrFidQuery.SetAt(nIndex, qry);
	}

	m_strGridSymbol = strSymbol;
	m_strMajorFID.Format(_T("%03d"), atoi(strSymbol)/1000);
	m_strMinorFID.Format(_T("%03d"), atoi(strSymbol)%1000);
}

void CWinixConverter::SetRowMaxCount(CString strFIDCode, long lMaxCount)
{
	CFidQuery qry;
	int nIndex = GetFidQuery(strFIDCode, qry);
	if (nIndex >= 0)
	{
		CFidBlock *pBlock = NULL;
		int nSize = qry.m_OutPack.GetBlockSize();
		for (int i=0; i<nSize; i++)
		{
			pBlock = qry.m_OutPack.GetBlockAt(i);
			if (pBlock->IsArray())
			{
				pBlock->m_nReqRow = (int)lMaxCount;
				break;
			}
		}
		m_arrFidQuery.SetAt(nIndex, qry);
	}
}

int CWinixConverter::GetTrQuery(CString strQryID, CTrQuery &qry)
{
	int nSize = m_arrTrQuery.GetSize();
	CTrQuery trQry;
	for (int i=0; i<nSize; i++)
	{
		trQry = m_arrTrQuery.GetAt(i);
		if (strQryID.Compare(trQry.m_strID) == 0)
		{
			qry = trQry;
			return i;
		}
	}

	return -1;
}

int CWinixConverter::GetFidQuery(CString strQryID, CFidQuery &qry)
{
	int nSize = m_arrFidQuery.GetSize();
	CFidQuery fidQry;
	for (int i=0; i<nSize; i++)
	{
		fidQry = m_arrFidQuery.GetAt(i);
		if (strQryID.Compare(fidQry.m_strID) == 0)
		{
			qry = fidQry;
			return i;
		}
	}

	return -1;
}

void CWinixConverter::SelectQuery(CString strQryID)
{
	int i, nSize;
	if (m_nTRType)
	{
		if (m_bDirectSend)
		{
			nSize = m_arrTrQuery.GetSize();
			CTrQuery trQry;
			for (i=0; i<nSize; i++)
			{
				trQry = m_arrTrQuery.GetAt(i);
				if (strQryID.IsEmpty() || strQryID.Compare(trQry.m_strID) == 0)
				{
					m_inTrQry = trQry.m_InPack;
					m_outTrQry = trQry.m_OutPack;
					break;
				}
			}
		}
		else
		{
			if (m_arrTrQuery.GetSize() <= 0)
				return;

			m_outTrQry = m_arrTrQuery.GetAt(0).m_OutPack;
		}

		// Set Default Use Buffer to FALSE.
		nSize = m_outTrQry.GetItemCount();
		for (i=0; i<nSize; i++)
			m_outTrQry.GetItemAt(i)->SetUseBuf(FALSE);

		CString strID;
		//	Set Use Buffer to TRUE.
		ST_CHART_PACKET stPack;
		nSize = m_prop.GetPacketSize();
		for (i=0; i<nSize; i++)
		{
			stPack = m_prop.GetPacket(i);
			if (atoi(stPack.szRealFID) == m_nFidTypeDate)		m_strDateName = stPack.szPacketName;
			else if (atoi(stPack.szRealFID) == m_nFidTypeTime)	m_strTimeName = stPack.szPacketName;

			m_outTrQry.SetUseBufAtItem(stPack.szFID, TRUE);
		}
	}
	else
	{
		if (m_bDirectSend)
		{
			if (strQryID.Compare(m_curFidQry.m_strID))
			{
				nSize = m_arrFidQuery.GetSize();
				CFidQuery fidQry;
				for (i=0; i<nSize; i++)
				{
					fidQry = m_arrFidQuery.GetAt(i);
					if (strQryID.IsEmpty() || strQryID.Compare(fidQry.m_strID) == 0)
					{
						m_curFidQry = fidQry;
						break;
					}
				}
			}
		}
		else
		{
			if (m_arrFidQuery.GetSize() <= 0)
				return;

			m_curFidQry = m_arrFidQuery.GetAt(0);
		}

		ResetFID();
	}
}

void CWinixConverter::ResetFID()
{
	CFidItem* pItem = NULL;
	CString strFID, strReal;
	ST_CHART_PACKET stPack;
	int nSize = m_prop.GetPacketSize();
	for (int i=0; i<nSize; i++)
	{
		stPack = m_prop.GetPacket(i);
		pItem = m_curFidQry.m_OutPack.GetItemChartAt(stPack.szPacketName);
		if (pItem)
		{
			strFID.Format(_T("%03d"), atoi(pItem->GetItemID()) % 1000);

			if (strFID.Compare(m_szFidTypeDate) == 0)			m_strDateName = stPack.szPacketName;
			else if (strFID.Compare(m_szFidTypeTime) == 0)	m_strTimeName = stPack.szPacketName;

			stPack.szFID = strFID;

			strReal = pItem->GetRealID();
			if (strReal.IsEmpty())
				strReal = strFID;

			stPack.szRealFID = strReal;
			m_prop.SetPacket(i, stPack);
		}
	}

	SetGridSymbol(m_curFidQry.m_OutPack.GetDiffGridSymbol());
//	SetGridSymbol(m_curFidQry.m_OutPack.GetGridSymbol());

//	GetFIDLayout();
}

char CWinixConverter::GetMarketTypeFromCode(CString strCode)
{
	if (CWinixConverter::m_pMaster == NULL)
		CWinixConverter::m_pMaster = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);

	return CWinixConverter::m_pMaster->GetMarketTypeFromCode(strCode);
}

BOOL CWinixConverter::GetSiteUpKode(LPCSTR szCode, CString& rCode)
{
	if (CWinixConverter::m_pMaster == NULL)
		CWinixConverter::m_pMaster = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);

	return CWinixConverter::m_pMaster->GetSiteUpKode(szCode, rCode);
}

CFidBlock* CWinixConverter::GetDefaultFIDBlock(int nLeftSym)
{
	if (nLeftSym <= 0)
		return NULL;

	if (nLeftSym < 0)
	{
		if (m_strCurCode.IsEmpty())
			nLeftSym = atoi(m_strMajorFID);
		else
		{
			char nType = GetMarketTypeFromCode(m_strCurCode);
			if (nType == CMasterTypeInfo::STOCK_CHART)
				return m_defFidQry.m_OutPack.GetBlockAt(_T("주식"));
			else if (nType == CMasterTypeInfo::UPJONG_CHART)
				return m_defFidQry.m_OutPack.GetBlockAt(_T("업종"));
			else if (nType == CMasterTypeInfo::FUTOPT_CHART)
				return m_defFidQry.m_OutPack.GetBlockAt(_T("선물"));
		}
	}

	switch (nLeftSym)
	{
	case 101:	return m_defFidQry.m_OutPack.GetBlockAt(_T("주식"));
	case 102:	return m_defFidQry.m_OutPack.GetBlockAt(_T("업종"));
	case 103:	return m_defFidQry.m_OutPack.GetBlockAt(_T("선물"));
	case 109:	return m_defFidQry.m_OutPack.GetBlockAt(_T("선물지표"));
	case 110:	return m_defFidQry.m_OutPack.GetBlockAt(_T("CME"));		//[alzioyes] CME추가.
	case 111:	return m_defFidQry.m_OutPack.GetBlockAt(_T("해외선물"));
	//<<20100310_JS.Kim FX 추가 aaaaaaaaaaaaaaaaaaaaaaaaaaaaa
	case 112:	return m_defFidQry.m_OutPack.GetBlockAt(_T("FX"));
	//>>
	default:
		if (nLeftSym < 20)
			return m_defFidQry.m_OutPack.GetBlockAt(_T("주식"));
		else if (nLeftSym < 30)
			return m_defFidQry.m_OutPack.GetBlockAt(_T("업종"));
		else if (nLeftSym < 50)
			return m_defFidQry.m_OutPack.GetBlockAt(_T("선물"));

		break;
	}

	return NULL;
}


//*******************************************************************/
/*! Function Name : TbondPrcLToF
/*! Function      : 미국채 표시변환 TB Price long -> float decimal
/*! Param         : CString strDecimalGB, CString strData
/*! Return        : CString
/*! Comment       : 
//******************************************************************/
BOOL CWinixConverter::TbondPrcLToF(char cType, int nPcnt, double dRate, CString &strData)
{
	//Test
	//-->
	//Org
	if (cType <= '9')	return FALSE;

	//Test Routine
	//if (cType <= '9')	return TRUE;
	//<--

	CString strInData = strData;
	if (strData.GetLength() <= 0)
	{
		strData = _T("");
		return TRUE;
	}

//	소수부 정수갯수, 소수부 소수갯수	예시) 9999'99.9
//	int nFcnt, nHcnt;
	CString strFvalue, strHvalue;

	int nFind = strInData.Find(_T("`"));
	if (nFind < 0)
		nFind = strInData.Find(_T("'"));

	strFvalue = strInData.Left(nFind);
	strHvalue = strInData.Mid(nFind+1);
/*
	if (cType == 'K')
	{
		//if (strHvalue.Right(1).CompareNoCase(_T("5")))
		if (strHvalue.Right(1) == '2' || strHvalue.Right(1) == '7')
			strHvalue += _T("5");
	}
*/
	double dRetData = (atof(strFvalue) + atof(strHvalue) * dRate);// * pow(10, nPcnt);
	strData.Format(_T("%f"), dRetData);

	return TRUE;
}

char CWinixConverter::GetDecimalRate(CString strDecimalGB, int &nPcnt, double &dRate, CString &strResult)
{
	if (strDecimalGB.GetLength() <= 0)
		return -1;

	int nFcnt=0, nHcnt=0;
	nPcnt = 0;
	dRate = 1.0;

	char cRet = '0';
	int nType = atoi(strDecimalGB);

	if (nType >= 1 && nType <= 10)
	{
		nPcnt = nType-1;
		dRate = 1.0/pow(10, nPcnt);
		cRet += nPcnt;
		strResult.Format("× %0.*f", nPcnt, 1 / pow(10, nPcnt));
	}
	else
	{
		cRet = strDecimalGB.GetAt(0);
		switch (cRet)
		{
		case 'A':	nPcnt = 1;	dRate = 1.0/2.0;	nFcnt = 1;	nHcnt = 0;	break;	//	9999'9
		case 'B':	nPcnt = 2;	dRate = 1.0/4.0;	nFcnt = 1;	nHcnt = 0;	break;	//	9999'9
		case 'C':	nPcnt = 3;	dRate = 1.0/8.0;	nFcnt = 1;	nHcnt = 0;	break;	//	9999'9
		case 'D':	nPcnt = 4;	dRate = 1.0/16.0;	nFcnt = 2;	nHcnt = 0;	break;	//	9999'99
		case 'E':	nPcnt = 5;	dRate = 1.0/32.0;	nFcnt = 2;	nHcnt = 0;	break;	//	9999'99
		case 'F':	nPcnt = 6;	dRate = 1.0/64.0;	nFcnt = 2;	nHcnt = 0;	break;	//	9999'99
		//case 'I':	nPcnt = 6;	dRate = 1.0/64.0;	nFcnt = 2;	nHcnt = 1;	break;	//	9999'99.9
		//case 'J':	nPcnt = 7;
		//case 'K':	nPcnt = 7;	dRate = 1.0/128.0;	nFcnt = 2;	nHcnt = 1;	break;	//	9999'99.9
//		case 'G':			break;
		case 'I':	nPcnt = 6;	dRate = 1.0/32.0;	nFcnt = 2;	nHcnt = 1;	break;	//	9999'99.9
		case 'J':	nPcnt = 7;	dRate = 1.0/64.0;	nFcnt = 2;	nHcnt = 1;	break;	//	9999'99.9
		case 'K':	nPcnt = 7;	dRate = 1.0/32.0;	nFcnt = 2;	nHcnt = 1;	break;	//	9999'99.9
		default:	return ' ';
		}

		strResult.Format("× 1/%d\'%d.%d", (int)(dRate/(dRate*dRate)), nFcnt+nHcnt, nHcnt);
	}

	return cRet;
}

double CWinixConverter::DataSeparation(CString strData, int nMask)
{
	strData.TrimLeft();
	strData.TrimRight();

	if (strData.IsEmpty())
		return 99999999.;

	if (nMask <= 0)
		//nMask = 1;
		return atof(strData);

	double dMask = pow(10, nMask);

	return (atof(strData) * dMask);
}

CString CWinixConverter::GetTimeIndexInfo(int nIndex, int nType)
{
	CString strFileName, strTemp, strKey, strData, strTitle, strGmt, strSummerTime, strCityName;

	char szItem[MAX_PATH];
	int nCount;

	GetDir_DrfnRoot(strFileName);
	strFileName += "\\Icss\\timesetup.ini";

	CString strSection = "TIME";
	nCount = GetPrivateProfileInt(strSection, "COUNT", 0, strFileName);

	if (nCount < nIndex)
		return "";

	strKey.Format("%d", nIndex);
	memset(szItem, 0, MAX_PATH);
	GetPrivateProfileString(strSection, strKey, "", szItem, MAX_PATH, strFileName);

	strTemp = szItem;
	int nCnt = strTemp.Find(",");
	strTitle= strTemp.Left(nCnt);	
	strTemp = strTemp.Mid(nCnt+1);	
	nCnt	= strTemp.Find(",");
	strGmt	= strTemp.Left(nCnt);

	if (strTemp.GetLength() < nCnt+1)
	{
		if (nIndex == -1 && (nType != 1 || nType != 2))
			return "원래시간대";

		return "";
	}
	
	strSummerTime = strTemp.Mid(nCnt+1);

	// 도시명만 취득한다.
	nCnt = strTitle.Find('(');	
	strCityName = strTitle.Left(nCnt);	

	switch (nType)
	{
	case 1 :		//	시간차
		if (strSummerTime == "1")
			strData.Format("%d", atoi(strGmt)+1);
		else
			strData.Format("%d", atoi(strGmt));

		break;

	case 2 :		//	섬머타임 여부
		strData = strData;
		break;

	default :		//	타이틀
		if (nIndex == -1 && (nType != 1 || nType != 2))
			return "원래시간대";
		else
			strData = strTitle;

		break;
	}

	return strData;
}

CString CWinixConverter::GetCodeType(CString strCode)
{
	CString strType = "";
	m_mapCodeType.Lookup(strCode, strType);
	
	return strType;
}

//save to file with Text Format.
BOOL CWinixConverter::SaveOnlyChartProperties(LPCSTR szSaveFile)
{
	CString strKey, strData, strTemp;
	const char *szKey=NULL, *szData=NULL;

	CFile file;
	if (!file.Open(szSaveFile, CFile::typeBinary|CFile::modeCreate|CFile::modeWrite))
	{
		CString strMsg;
		strMsg.Format("failed create file (%s)", szSaveFile);
		AfxMessageBox(strMsg);
		
		return FALSE;
	}
	
	CArchive ar(&file, CArchive::store);

	int nCount = m_propMap.GetCount();
	ar << nCount;
	
	POSITION pos = m_propMap.GetStartPosition();
	while (pos)
	{
		m_propMap.GetNextAssoc(pos, szKey, szData);
		strKey.Format(_T("%s"), szKey);
		strData.Format(_T("%s"), szData);
		ar << strKey;
		ar << strData;
	}

	ar.Close();
	file.Close();
	return TRUE;
}

CString CWinixConverter::GetMarkeyTypeFromName(CString strMarketName)
{
	CString strType = _T("1");
	if(strMarketName.CompareNoCase(_T("업종")) == 0)
		strType = _T("2");
	else if(strMarketName.CompareNoCase(_T("선물")) == 0)
		strType = _T("3");
	else if(strMarketName.CompareNoCase(_T("옵션")) == 0)
		strType = _T("4");
	else if(strMarketName.CompareNoCase(_T("주식선물")) == 0)
		strType = _T("5");
	else if(strMarketName.CompareNoCase(_T("상품선물")) == 0)
		strType = _T("6");

	return strType;
}

int CWinixConverter::SetTDCRDC(CString &strBojoMsg)
{
	CString strResult = "";
	CString strTemp, strPacketType;
	CString strTDC = _T("TDC=");
	CString strRDC = _T("RDC=");
	
	double dPacketType = 1;
	ST_CHART_PACKET stPack;
	int nConverDataCnt = 0;
	int nSize = m_prop.GetPacketSize();
	for ( int nIndex = 0; nIndex < nSize; nIndex++ )
	{	
		stPack = m_prop.GetPacket( nIndex );
		strPacketType = stPack.szTextType;		
		strPacketType.Replace( "x", "" );
		strPacketType.Replace( "X", "" );
		strPacketType.TrimLeft();
		strPacketType.TrimRight();

		dPacketType = atof( strPacketType );
		if( dPacketType == 1.0 || dPacketType == 0.0 )
			continue;

		strTemp.Format( "%s:%s,|", strPacketType, stPack.szPacketName );
		strTDC += strTemp;

		int nPoint = strPacketType.GetLength() - 1;
		strTemp.Format( "%s:%d:%s,|", strPacketType, nPoint, stPack.szPacketName );
		strRDC += strTemp;

		nConverDataCnt++;
	}

	if( nConverDataCnt > 0 )
	{
		int nLength = strTDC.GetLength();
		strTemp = strTDC.Left( nLength - 1 );
		strTDC = strTemp;

		nLength = strRDC.GetLength();
		strTemp = strRDC.Left( nLength - 1 );
		strRDC = strTemp;

		strTDC += _T("@");
		strRDC += _T("@");
		CString strCB = "PREVPRICE=_MB0A1B0DD_:257.00|_Volume_:       36236,_VolumeAmount_:@";
		CString strOHL= "OHL=:256.45:256.65:255.35@";
		
		SetBojoMsgData( _T("TDC"), strTDC, strBojoMsg );
		SetBojoMsgData( _T("RDC"), strRDC, strBojoMsg );
// 		SetBojoMsgData( _T("PREVPRICE"), strCB, strBojoMsg );
// 		SetBojoMsgData( _T("OHL"), strOHL, strBojoMsg );
	}
	int nRet = strBojoMsg.GetLength();

	return nRet;
}


void CWinixConverter::SetBojoMsgData(CString strKey, CString strKeyData, CString &strBojoMsg)
{
	if( strBojoMsg.GetLength() <= 0 )
		return;

	int nFS = strBojoMsg.Find( strKey );
	CString strFront = strBojoMsg.Left( nFS );
	CString strTemp  = strBojoMsg.Mid( nFS );

	int nFE = strTemp.Find( "@" );
	CString strBack = strTemp.Mid( nFE + 1 );

	strBojoMsg.Format( "%s%s%s", strFront, strBack, strKeyData );


	CString strMsgLength;
	strMsgLength.Format("%04d", strBojoMsg.GetLength() - 4 );

	strTemp = strMsgLength + strBojoMsg.Mid(4);
	strBojoMsg = strTemp;
}

CString CWinixConverter::ReflectPoint( CString strSrc )
{
	CString strResult = "";
	return strResult;
}