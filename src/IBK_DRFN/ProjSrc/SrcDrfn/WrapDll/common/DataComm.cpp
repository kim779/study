// DataComm.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataComm.h"

#include "../../../SrcSite/include/axisGrid.h"
#include "../../inc/IMainInfoMng.h"	
#include "DrChartCtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CDataComm, CObject)

//////////////////////////////////////////////////////////////////////
// CDataComm
//////////////////////////////////////////////////////////////////////
CDataComm::CDataComm()
{
	m_pCommWnd = NULL;
}

CDataComm::~CDataComm()
{
}

void CDataComm::Copy(const CDataComm& item)
{
}

CDataComm& CDataComm::operator = (const CDataComm& item)
{
	Copy(item);

	return *this;
}

void CDataComm::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}

void CDataComm::RemoveAll()
{
}

int CDataComm::SendTR(CString strTR, CString strData, int nKey, int nStat)
{
	if (m_pCommWnd == NULL)
		return -1;

 	int nDataLen = strData.GetLength();
	int bRet;
	if(strTR.CompareNoCase("GOOPHOOP") != 0)
	{	
		int nSize = nDataLen;
		LPBYTE pSendBuffer = new BYTE[nSize+10];

		int nOff=0;
		memcpy(pSendBuffer, (LPCSTR)strData, nDataLen);
		nOff += nDataLen;
		pSendBuffer[nOff]=NULL;

		bRet = m_pCommWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nDataLen), (LPARAM)(const char*)pSendBuffer);
		delete []pSendBuffer;
		return bRet;
	}
	else
	{
		CString strTRData = "";
		
		struct _userTH user;
		strncpy(&user.trc[0], strTR, 8);
		user.key  = nKey;
		user.stat = nStat;
		int  nHead = sizeof(_userTH);
		long nSendLen = nHead+nDataLen;
		LPBYTE pSendBuffer = new BYTE[nSendLen+1];
		memcpy(pSendBuffer, (char*)&user, nHead);
		memcpy(pSendBuffer+nHead, strData, nDataLen);
		pSendBuffer[nSendLen]=NULL;

		bRet = m_pCommWnd->GetParent()->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nDataLen), (LPARAM)(const char*)pSendBuffer);
		delete []pSendBuffer;
		return bRet;

	}
	
	return bRet;
}

int CDataComm::SendTRData(CChartProp &prop, CTrQuery *pTrQry, CString strData)
{
	CTrQuery qry;
	qry = *pTrQry;

	int nKey = GetTrKey(TRUE);
	if (nKey >= 0)
		m_mapQuery.SetAt(nKey, prop);

//	CString strLayout;
//	CStringArray arr;
//	ParsingInputData(strData, arr);
//	int nSize = arr.GetSize();
//	for (int i=1; i<nSize; i++)
//		strLayout += arr.GetAt(i);
//	strLayout += sFS;

	return nKey;//SendTR(qry.m_strTRCode, strLayout, nKey, US_KEY);
}

int CDataComm::SendFIDData(CChartProp &prop, CFidQuery &fidQry, CFidBlock *pBlock, CString strData, BOOL bNext)
{
	int nKey = GetTrKey(FALSE);
	if (nKey >= 0)
		m_mapQuery.SetAt(nKey, prop);

	CString strLayout;
	strLayout = GetFidInboundDataTest(fidQry, pBlock, bNext);

	return SendTR(fidQry.m_strOOPCode, strLayout, nKey, US_OOP|US_KEY);
}

int CDataComm::GetTrKey(BOOL bTR)
{
	CChartProp prop;
	int nKey, nStart;
	if (bTR)	nKey = nStart = COMM_TRKEY_START;
	else		nKey = nStart = COMM_FIDKEY_START;

	while (1)
	{
		if (m_mapQuery.Lookup(nKey, prop) == 0)
			break;

		nKey++;

		if (nKey > nStart + COMM_QUERYSEND_COUNT)
			return -1;
	}

	return nKey;
}

BOOL CDataComm::GetRcvTr(int nKey, CChartProp &prop)
{
	BOOL bRet = FALSE;
	CChartProp propItem;
	if (m_mapQuery.Lookup(nKey, propItem))
	{
		prop = propItem;
		bRet = TRUE;
	}

	m_mapQuery.RemoveKey(nKey);

	return bRet;
}

CString CDataComm::GetFidInboundData(CFidQuery &fidQry, CFidBlock *pDefBlock, BOOL bNext)
{
//	Make Inbound FID Layout..
	CString strLayout, strCode;
	int nIndex = 0;

	CString strData, strID, strRealID;
	CFidItem *pItem = NULL;
	int nCount = fidQry.m_InPack.GetItemCount();
	for (int i=0; i<nCount; i++)
	{
		pItem = fidQry.m_InPack.GetItemAt(i);
		if (pItem == NULL)
			continue;

		strID = pItem->GetItemID();

		strLayout += strID;
		strLayout += sDS;

		strData = pItem->GetValue();
		strLayout += strData;
		strLayout += sCELL;
	}

	CString strGridSymbol;
	CFidBlock *pBlock = NULL;
	nCount = fidQry.m_OutPack.GetBlockSize();
	for (int i=-1; i<nCount; i++)
	{
		if (i<0)
			pBlock = pDefBlock;
		else
			pBlock = fidQry.m_OutPack.GetBlockAt(i);

		if (pBlock == NULL)
			continue;

		if (pBlock->IsArray())
		{
			strLayout += _T('?'); //$KHD

			strGridSymbol = pBlock->m_strGridSym;// 입력값 
			strLayout += strGridSymbol;
			strLayout += sDS;

			GRID_I grid_i;
			memset(&grid_i, 0x20, sizeof(grid_i));

			char szTemp[5];
			grid_i.gdir[0] = _T('1');	//	top.

			//	일반(Next조회).
			if (bNext)	grid_i.ikey[0] = _T('2');	//	Page Down.
			else		grid_i.ikey[0] = _T('0');

			sprintf(szTemp, _T("%04d"), pBlock->m_nReqRow);//nCount
			memcpy(grid_i.nrow, szTemp, sizeof(grid_i.nrow));
			sprintf(szTemp, _T("%04d"), 0);
			memcpy(grid_i.page, szTemp, sizeof(grid_i.page));
			grid_i.sdir[0] = _T('1');
			grid_i.vflg[0] = _T('1');
			memcpy(grid_i.vrow, _T("111"), sizeof(grid_i.vrow));

			if (bNext)
			{
				strCode = fidQry.m_InPack.GetDataAt(CFidQuery::GetCodeFID(fidQry.m_nMarket));
				CString strKey;
				strKey = strCode + "\t" + fidQry.m_strID;
				m_pParent->SendMessage(WMU_GETNEXTKEY, (WPARAM)(LPCTSTR)strKey, (LPARAM)&grid_i);
			}

			char szGrid_i[128];
			memset(szGrid_i, 0x00, sizeof(szGrid_i));
			memcpy(szGrid_i, (char*)&grid_i, sizeof(grid_i));

			strLayout += (LPCTSTR)(LPTSTR)szGrid_i;
		}

		int nSize = pBlock->GetItemSize();
		for (int k=0; k<nSize; k++)
		{
			pItem = pBlock->GetItemAt(k);
			if (pItem == NULL)
				continue;

			strID = pItem->GetItemID();

			strLayout += strID;
			if (pBlock->IsArray())
				strLayout += sCOL;
			else if (i<0)
				strLayout += sSUB;
			else
				strLayout += sCELL;
		}

		if (pBlock->IsArray())
			strLayout += sCELL;
	}


	//strLayout = "&..#...2014GOOPHOOP0		02951301005870.1034.1022.1023.1024.1033.1027.1025.1026.1029.1030.1031.1021.1310.1311.1312.1313.1316.1318.?5500000200.20120822..0000!!5302.5034.5029.5030.5031.5023.5027.5028.5311.5310.. ";
//	strLayout = "Start!!!!";
	return strLayout;
}

#include "../../../SrcSite/include/axisgenv.h"//:\Project\IBKS\ProjSrc\SrcSite\include\axisgenv.h
CString CDataComm::GetFidInboundDataTest(CFidQuery &fidQry, CFidBlock *pDefBlock, BOOL bNext)
{
//	Make Inbound FID Layout..
	CString strLayout, strCode;
	int nIndex = 0;

	CString strData, strID, strRealID;
	CFidItem *pItem = NULL;
	int nCount = fidQry.m_InPack.GetItemCount();
	for (int i=0; i<nCount; i++)
	{
		pItem = fidQry.m_InPack.GetItemAt(i);
		if (pItem == NULL)
			continue;

		strID = pItem->GetItemID();

		strLayout += strID;
		strLayout += sDS;

		strData = pItem->GetValue();
		strLayout += strData;
		strLayout += sCELL;
	}

	CString strGridSymbol;
	CFidBlock *pBlock = NULL;
	nCount = fidQry.m_OutPack.GetBlockSize();
	for (int i=-1; i<nCount; i++)
	{
		if (i<0)
			pBlock = pDefBlock;
		else
			pBlock = fidQry.m_OutPack.GetBlockAt(i);

		if (pBlock == NULL)
			continue;

		if (pBlock->IsArray())
		{
			strLayout += _T('?'); //$KHD

			strGridSymbol = pBlock->m_strGridSym;// IBK : 입력값 
			strLayout += strGridSymbol;
			strLayout += sDS;

			_dataH	Datah; // IBK : 차트 인풋 
			FillMemory(&Datah, sz_DATAH, 0x20);
			
			Datah.dindex = GI_DAY; //IBK : 입력값 
			Datah.dunit = GI_DAY;   //IBK : 입력값 

		//	char szTemp[5];

			//	일반(Next조회).
//			if (bNext)	grid_i.ikey[0] = _T('2');	//	Page Down.
//			else		grid_i.ikey[0] = _T('0');
			CString szTemp;
			szTemp.Format(_T("%06d"), pBlock->m_nReqRow);//nCount
			CopyMemory(Datah.count, szTemp, szTemp.GetLength());
			Datah.dkey = 0 + 0x30;
			
			CString strDate;
			strDate = "20120911";
			CopyMemory(Datah.pday, strDate, strDate.GetLength());
			szTemp.Format("%0*d", sizeof(Datah.lgap), 1);// N분봉 
			CopyMemory(Datah.lgap, szTemp, szTemp.GetLength() );
//			if (bNext)
//			{
//				strCode = fidQry.m_InPack.GetDataAt(CFidQuery::GetCodeFID(fidQry.m_nMarket));
//				CString strKey;
//				strKey = strCode + "\t" + fidQry.m_strID;
//				m_pParent->SendMessage(WMU_GETNEXTKEY, (WPARAM)(LPCTSTR)strKey, (LPARAM)&grid_i);
//			}

//			char szGrid_i[128];
//			memset(szGrid_i, 0x00, sizeof(szGrid_i));
//			memcpy(szGrid_i, (char*)&grid_i, sizeof(grid_i));
			
			char szData_i[sz_DATAH];
			memset(szData_i, 0x20, sz_DATAH);//sizeof(Datah));
			CopyMemory(szData_i, (char*)&Datah, sz_DATAH);//sizeof(Datah));
			szData_i[sz_DATAH-1] = 0x00; // KHD : 맨끝에 0x00문자를 집어 넣는다.
			strLayout += szData_i;

			//strLayout += (LPCTSTR)(LPTSTR)strData;
		}
//////////////////////////////// 차트 반복데이터 /////////////////////////////////
		int nSize = pBlock->GetItemSize();
		for (int k=0; k<nSize; k++)
		{
			pItem = pBlock->GetItemAt(k);
			if (pItem == NULL)
				continue;

			strID = pItem->GetItemID();

			strLayout += strID;
			if (pBlock->IsArray())
				strLayout += sCOL;
			else if (i<0)
				strLayout += sCELL;//KHD : IBK
			else
				strLayout += sCELL;//KHD : IBK
		}

		if (pBlock->IsArray())
			strLayout += sCELL;
	}


	//strLayout = "&..#...2014GOOPHOOP0		02951301005870.1034.1022.1023.1024.1033.1027.1025.1026.1029.1030.1031.1021.1310.1311.1312.1313.1316.1318.?5500000200.20120822..0000!!5302.5034.5029.5030.5031.5023.5027.5028.5311.5310.. ";
//	strLayout = "Start!!!!";
	return strLayout;
}
void CDataComm::ParsingInputData(CString strData, CStringArray &arr)
{
	CString strRight, strLeft, strValue = strData;
	int nFind = strValue.Find(_T("\t"));
	while (nFind >= 0)
	{
		strLeft = strValue.Left(nFind);
		strRight = strValue.Right(strValue.GetLength()-nFind-1);

		arr.Add(strLeft);

		strValue = strRight;
		nFind = strValue.Find(_T("\t"));
	}

	strValue.TrimLeft();	strValue.TrimRight();
	if (strValue.GetLength() > 0)
		arr.Add(strValue);
}

