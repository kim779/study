// ChartMng.cpp: implementation of the CChartItemData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartMng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CChartMng, CObject)

//////////////////////////////////////////////////////////////////////
// CChartMng
//////////////////////////////////////////////////////////////////////
CChartMng::CChartMng()
{
}

CChartMng::~CChartMng()
{
}

void CChartMng::Copy(const CChartMng& item)
{
	m_arrChartProp.Copy(item.m_arrChartProp);
}

CChartMng& CChartMng::operator = (const CChartMng& item)
{
	Copy(item);

	return *this;
}

void CChartMng::RemoveAll()
{
	m_arrChartProp.RemoveAll();
}

int CChartMng::GetChartProp(CString strKey, CChartProp &info)
{
	CChartProp prop;
	int nSize = m_arrChartProp.GetSize();
	for (int i=0; i<nSize; i++)
	{
		prop = m_arrChartProp.GetAt(i);
		if (prop.CampareKey(strKey) == 0)
		{
			info = prop;
			return i;
		}
	}

	return -1;
}

void CChartMng::GetChartPropArr(CString strCode, CChartPropArr &infoArr)
{
	CChartProp prop;
	int nSize = m_arrChartProp.GetSize();
	for (int i=0; i<nSize; i++)
	{
		prop = m_arrChartProp.GetAt(i);
		if (prop.CampareCode(strCode) == 0)
			infoArr.Add(prop);
	}
}

void CChartMng::SetChartProp(CString strKey, CChartProp &info)
{
	CChartProp prop;
	int nIndex = GetChartProp(strKey, prop);
	if (nIndex < 0)
	{
		m_arrChartProp.Add(info);
		return;
	}

	m_arrChartProp.SetAt(nIndex, info);
}

int CChartMng::GetChartRQ(CString strRQ, CChartProp &info)
{
	int nSize = m_arrChartProp.GetSize();
	for (int i=0; i<nSize; i++)
	{
		info = m_arrChartProp.GetAt(i);
		if (info.CampareRQ(strRQ) == 0)
			return i;
	}

	return -1;
}

void CChartMng::RemoveRQ(CString strRQ)
{
	CChartProp info;
	int nIndex = GetChartRQ(strRQ, info);
	m_arrChartProp.RemoveAt(nIndex);
}

int CChartMng::GetChartPropAt(int nIndex, CChartProp& info)
{
	if (m_arrChartProp.GetSize() > nIndex)
	{
		info = m_arrChartProp.GetAt(nIndex);
		return nIndex;
	}

	return -1;
}

bool CChartMng::GetRQChartAt(CString &szRQ, int nIndex)
{
	if(m_arrChartProp.GetSize()==0)
		return false;

	if (nIndex<-1 || m_arrChartProp.GetSize() <= nIndex)
		return false;

	if(nIndex==-1) nIndex = m_arrChartProp.GetSize()-1;
	CChartProp info = m_arrChartProp.GetAt(nIndex);
	szRQ = info.GetRQ();

	return true;
}
