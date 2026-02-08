// LRSIWeight.cpp: implementation of the CLRSIWeight class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LRSIWeight.h"
#include "../gData/DataMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLRSIWeight::CLRSIWeight(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_iValidDigit = 2;
}

CLRSIWeight::~CLRSIWeight()
{

}

CString CLRSIWeight::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
		ret.Format("RSI(가중치)[%d]", (int)m_pgraph->cInfo.awValue[0]);
	else if (nIndex == 1)
		ret.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[1]);
	else
		ret = _T("");

	return ret;
}

void CLRSIWeight::MakeTitle(CStringArray &arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	tmpstr.Format("RSI(가중치)[%d]", (int)m_pgraph->cInfo.awValue[0]);
	arTitle.Add(tmpstr);
	tmpstr.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[1]);
	arTitle.Add(tmpstr);
}
