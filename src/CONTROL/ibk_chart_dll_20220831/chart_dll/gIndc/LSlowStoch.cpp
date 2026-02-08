#include "stdafx.h"
#include "LSlowStoch.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLSlowStoch::CLSlowStoch(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_iValidDigit = 2;
}

CLSlowStoch::~CLSlowStoch()
{

}

CString CLSlowStoch::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
		ret.Format("S-STOCH[%d, %d, %d] %%K", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1], (int)m_pgraph->cInfo.awValue[2]);
	else if (nIndex == 1)
		ret.Format("%%D");		
	else
		ret = _T("");

	return ret;
}

void CLSlowStoch::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;
	
	tmpstr.Format("%%KS[%d,%d,%d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1], (int)m_pgraph->cInfo.awValue[2]);
	arTitle.Add(tmpstr);
	tmpstr.Format("%%DS[%d,%d,%d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1], (int)m_pgraph->cInfo.awValue[2]);
	arTitle.Add(tmpstr);
}

