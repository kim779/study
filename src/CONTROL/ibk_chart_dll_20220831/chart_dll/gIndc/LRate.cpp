#include "stdafx.h"
#include "LRate.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLRate::CLRate(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
}

CLRate::~CLRate()
{

}

CString CLRate::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
		ret = _T("회전율");
	else if (nIndex == 1)
		ret.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[0]);
	else
		ret = _T("");

	return ret;
}

void CLRate::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	tmpstr.Format("회전율");
	arTitle.Add(tmpstr);
	tmpstr.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[0]);
	arTitle.Add(tmpstr);
}
