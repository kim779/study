#include "stdafx.h"
#include "LDisparity.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLDisparity::CLDisparity(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_iValidDigit = 2;
}

CLDisparity::~CLDisparity()
{

}

CString CLDisparity::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
		ret.Format("이격도[%d]", (int)m_pgraph->cInfo.awValue[0]);
	else if (nIndex == 1)
		ret.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[1]);
	else
		ret = _T("");

	return ret;
}

void CLDisparity::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	tmpstr.Format("이격도[%d]", (int)m_pgraph->cInfo.awValue[0]);
	arTitle.Add(tmpstr);
	tmpstr.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[1]);
	arTitle.Add(tmpstr);
}

