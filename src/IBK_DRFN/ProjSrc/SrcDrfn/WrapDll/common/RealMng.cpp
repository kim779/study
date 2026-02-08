// RealMng.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RealMng.h"

#include "../../../SrcSite/include/axisGrid.h"
#include "WinixConverter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CRealMng, CObject)

//////////////////////////////////////////////////////////////////////
// CRealMng
//////////////////////////////////////////////////////////////////////
CRealMng::CRealMng()
{
	m_pComm = NULL;
}

CRealMng::~CRealMng()
{
}

void CRealMng::Copy(const CRealMng& item)
{
	m_arrCode.Copy(item.m_arrCode);
}

CRealMng& CRealMng::operator = (const CRealMng& item)
{
	Copy(item);

	return *this;
}

void CRealMng::RemoveAll()
{
	m_arrCode.RemoveAll();
}

int CRealMng::AdviceRealKey(CString strCode)
{
	CString strData;
	int nRealKey = -1, nEmpty = -1;
	int nSize = m_arrCode.GetSize();
	for (int i=0; i<nSize; i++)
	{
		strData = m_arrCode.GetAt(i);
		if (strData.Compare(strCode) == 0)
			return i + COMM_REALKEY_START;

		if (nEmpty < 0)
		{
			if (strData.IsEmpty())
				nEmpty = i;
		}
	}

	if (nEmpty < 0)
	{
		nEmpty = nSize;
		m_arrCode.Add(strCode);
	}
	else
		m_arrCode.SetAt(nEmpty, strCode);

	return nEmpty + COMM_REALKEY_START;
}

CString CRealMng::UnAdviceRealKey(int nKey)
{
	nKey -= COMM_REALKEY_START;

	int nSize = m_arrCode.GetSize();
	if (nKey >= nSize)
		return _T("");

	if (nKey < 0)
		return _T("");

	CString strCode = m_arrCode.GetAt(nKey);
	m_arrCode.SetAt(nKey, _T(""));

	return strCode;
}

int CRealMng::UnAdviceRealKey(CString strCode)
{
	CString strData;
	int nSize = m_arrCode.GetSize();
	for (int i=0; i<nSize; i++)
	{
		strData = m_arrCode.GetAt(i);
		if (strData.Compare(strCode) == 0)
		{
			m_arrCode.SetAt(i, _T(""));
			return i + COMM_REALKEY_START;
		}
	}

	return -1;
}

CString CRealMng::GetRealCode(int nKey)
{
	nKey -= COMM_REALKEY_START;
	int nSize = m_arrCode.GetSize();
	if (nKey >= nSize)
		return _T("");

	return m_arrCode.GetAt(nKey);
}

int CRealMng::GetRealKey(CString strCode)
{
	CString strData;
	int nSize = m_arrCode.GetSize();
	for (int i=0; i<nSize; i++)
	{
		strData = m_arrCode.GetAt(i);
		if (strData.Compare(strCode) == 0)
			return i + COMM_REALKEY_START;
	}

	return -1;
}

int CRealMng::SendRealData(int nKey, CString strCode, CTrQuery &qry, CString strRealSym)
{
	CString strCodeFID, strLayout, strPriceFID;

	char cType = CWinixConverter::GetMarketTypeFromCode(strCode);
	if (cType == CMasterTypeInfo::STOCK_CHART)
		strCodeFID = CFidQuery::GetCodeFID(MARKET_STOCK);

	if (nKey < 0)
		return -1;

	strPriceFID.Format(_T("%d023"), atoi(qry.m_strRealSymbol)/1000);

	strLayout += strCodeFID;
	strLayout += sDS;
	strLayout += strCode;
	strLayout += sFS;
	if (strRealSym.IsEmpty())	strLayout += qry.m_strRealSymbol;
	else						strLayout += strRealSym;
	strLayout += sFS;
	strLayout += strPriceFID;
	strLayout += sFS;

	return m_pComm->SendTR(_T("pooppoop"), strLayout, nKey, US_OOP|US_KEY);
}

int CRealMng::SendRealData(int nKey, CFidQuery &qry, CString strRealSym)
{
	CString strCodeFID, strLayout, strPriceFID;

	CString strCode = qry.m_InPack.GetCodeData(strCodeFID);

	if (strCode.IsEmpty())
		return -1;

	if (nKey < 0)
		return -1;

	strPriceFID.Format(_T("%d023"), atoi(qry.m_strRealSymbol)/1000);

	strLayout += strCodeFID;
	strLayout += sDS;
	strLayout += strCode;
	strLayout += sFS;
	if (strRealSym.IsEmpty())	strLayout += qry.m_strRealSymbol;
	else						strLayout += strRealSym;
	strLayout += sFS;
	strLayout += strPriceFID;
	strLayout += sFS;

	return m_pComm->SendTR(qry.m_strOOPCode, strLayout, nKey, US_OOP|US_KEY);
}

