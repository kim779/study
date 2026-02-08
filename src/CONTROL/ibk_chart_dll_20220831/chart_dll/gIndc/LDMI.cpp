#include "stdafx.h"
#include "LDMI.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLDMI::CLDMI(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_iValidDigit = 2;
}

CLDMI::~CLDMI()
{

}

CString CLDMI::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
		ret = _T("+DI");
	else if (nIndex == 1)
		ret.Format("-DI[%d]", (int)m_pgraph->cInfo.awValue[0]);
	else
		ret = _T("");

	return ret;
}

void CLDMI::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	tmpstr.Format("+DI[%d]", (int)m_pgraph->cInfo.awValue[0]);
	arTitle.Add(tmpstr);
	tmpstr.Format("-DI[%d]", (int)m_pgraph->cInfo.awValue[0]);
	arTitle.Add(tmpstr);
}
