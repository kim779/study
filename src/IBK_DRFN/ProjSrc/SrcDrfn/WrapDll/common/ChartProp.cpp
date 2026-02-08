// ChartProp.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartProp.h"

#ifdef __MULTI_LANG__
#include "../../Chart/Include_Chart/Dll_Load/IMetaTable.h"	//@Solomon-MultiLang:091117
#endif


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CChartProp, CObject)

//////////////////////////////////////////////////////////////////////
// CChartProp
//////////////////////////////////////////////////////////////////////
CChartProp::CChartProp()
{
	DefaultSet();
}

CChartProp::~CChartProp()
{
}

void CChartProp::DefaultSet()
{
	m_bReal			= FALSE;

	m_bABS			= FALSE;
	m_nRealKey		= -1;
	m_nRowIdx		= -1;
	m_strPacketName	= _T("");
	m_strKey		= _T("");
	m_strRQ			= _T("");
	m_strTitle		= _T("");
	m_strTRCode		= _T("");
	m_strCode		= _T("");
}

void CChartProp::Copy(const CChartProp& item)
{
	m_bReal			= item.m_bReal;

	m_bABS			= item.m_bABS;
	m_nRealKey		= item.m_nRealKey;
	m_nRowIdx		= item.m_nRowIdx;
	m_strPacketName	= item.m_strPacketName;
	m_strKey		= item.m_strKey;
	m_strRQ			= item.m_strRQ;
	m_strTitle		= item.m_strTitle;
	m_strTRCode		= item.m_strTRCode;
	m_strCode		= item.m_strCode;

	memcpy(&m_stGridOut, &item.m_stGridOut, sizeof(GRID_O));

	m_arrPacket.Copy(item.m_arrPacket);
}

CChartProp& CChartProp::operator = (const CChartProp& item)
{
	Copy(item);

	return *this;
}

void CChartProp::RemoveAll()
{
	DefaultSet();
	m_arrPacket.RemoveAll();
}

//	1;자료일자;302(043);8;YYYYMMDD;무;
void CChartProp::AddPacket(CString strPacket, int nVer)
{
	ST_CHART_PACKET stPacket;
	CString strLeft, strData;
	int nFind=0, nFind2=0, nIndex=0;
	nFind = strPacket.Find(_T(";"));

	while (nFind >= 0)
	{
		strLeft = strPacket.Left(nFind);
		strPacket = strPacket.Right(strPacket.GetLength()-nFind-1);
		if ((nIndex == 0) && (atoi(strLeft) != 1))
			break;

		if (nIndex == 1)
		{
#ifdef __MULTI_LANG__
			if (nVer < 7)
				stPacket.szPacketName = strLeft;
			else
				stPacket.szPacketName = g_iMetaTable.GetLanguageTextDirectly(IMetaTable::PACKET_NAME, IMetaTable::KOREAN, strLeft);
#else
			stPacket.szPacketName = strLeft;
#endif
		}
		else if (nIndex == 2)
		{
			nFind2 = strLeft.Find(_T("("));
			if (nFind2 >= 0)
			{
				strData = strLeft.Left(nFind2);
				stPacket.szFID = strData;

				strLeft = strLeft.Right(strLeft.GetLength()-nFind2-1);
				nFind2 = strLeft.Find(_T(")"));
				if (nFind2 >= 0)
					strLeft = strLeft.Left(nFind2);

				stPacket.szRealFID = strLeft;
			}
			else
			{
				stPacket.szFID = strLeft;
				stPacket.szRealFID = strLeft;
			}
		}
		else if (nIndex == 3)	stPacket.nPacketSize = atoi(strLeft);
		else if (nIndex == 4)	stPacket.szTextType = strLeft;
		else if (nIndex == 5)
		{
#ifdef __MULTI_LANG__
			strLeft = g_iMetaTable.GetLanguageTextDirectly(IMetaTable::SCALE_TYPE_AND_ETC, IMetaTable::KOREAN, strLeft);
#endif
			if (strLeft.Compare(_T("유"))==0)
			{
				stPacket.bReal = TRUE;
				m_bReal = TRUE;
			}
			else								stPacket.bReal = FALSE;
		}

		nIndex++;
		nFind = strPacket.Find(_T(";"));
	}

	if (nIndex > 0)
		m_arrPacket.Add(stPacket);
}

/*---------------------------------------
	1;자료일자;302(043);8;YYYYMMDD;무;
---------------------------------------*/
CString CChartProp::GetPacketToString(int nIndex)
{
	int nSize = m_arrPacket.GetSize();
	if (nSize <= nIndex)
		return _T("");

	ST_CHART_PACKET stPacket = m_arrPacket.GetAt(nIndex);

	CString strSize, strPacket = _T("1;");
	strSize.Format(_T("%d"), stPacket.nPacketSize);

	strPacket += stPacket.szPacketName + ";";
	strPacket += stPacket.szFID + "(" + stPacket.szRealFID + ");";
	strPacket += strSize + ";";
	strPacket += stPacket.szTextType + ";";
	if (stPacket.bReal)	strPacket += "유;";
	else				strPacket += "무;";

	return strPacket;
}

BOOL CChartProp::IsReal()
{
	ST_CHART_PACKET stPacket;
	int nSize = m_arrPacket.GetSize();
	for (int i=0; i<nSize; i++)
	{
		stPacket = m_arrPacket.GetAt(i);
		if (stPacket.bReal)
			return TRUE;
	}

	return FALSE;
}

void CChartProp::ResetPacketName()
{
	m_strPacketName = _T("");
	int nSize = m_arrPacket.GetSize();
	ST_CHART_PACKET stPacket;
	CString szInUseName;
	for (int i=0; i<nSize; i++)
	{
		stPacket = m_arrPacket.GetAt(i);
#ifdef __MULTI_LANG__
		szInUseName = g_iMetaTable.GetMetaTextDirectly(IMetaTable::PACKET_NAME, IMetaTable::KOREAN, stPacket.szPacketName);
#else
		szInUseName = stPacket.szPacketName;
#endif

		m_strPacketName += szInUseName + _T(";");
	}
}

void CChartProp::SetPacket(int nIndex, ST_CHART_PACKET &stPack)
{
	m_arrPacket.SetAt(nIndex, stPack);
}

