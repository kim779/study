#include "stdafx.h"
#include "LOBV.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLOBV::CLOBV(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_iValidDigit = 0;
}

CLOBV::~CLOBV()
{

}

CString CLOBV::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
		ret = _T("OBV");
	else if (nIndex == 1)
		ret.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[0]);
	else
		ret = _T("");

	return ret;
}

void CLOBV::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	tmpstr.Format("OBV");
	arTitle.Add(tmpstr);
	tmpstr.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[0]);
	arTitle.Add(tmpstr);
}

