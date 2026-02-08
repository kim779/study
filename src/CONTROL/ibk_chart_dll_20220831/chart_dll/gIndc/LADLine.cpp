#include "stdafx.h"
#include "LADLine.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLADLine::CLADLine(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
}

CLADLine::~CLADLine()
{

}

CString CLADLine::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
		ret = _T("A/D Line");
	else if (nIndex == 1)
		ret.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[0]);
	else
		ret = _T("");

	return ret;
}

void CLADLine::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	tmpstr.Format("A/D Line");
	arTitle.Add(tmpstr);
	tmpstr.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[0]);
	arTitle.Add(tmpstr);
}

