#include "stdafx.h"
#include "LROC.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLROC::CLROC(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	// 대신과 맞추기 위해서 100을 곱하지는 않고 digit만 2를 준다.
	m_iValidDigit = 2;
}

CLROC::~CLROC()
{

}

CString CLROC::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
	{
		if (m_pgraph->wGKind == GK_PROC)
			ret.Format("P-ROC[%d]", (int)m_pgraph->cInfo.awValue[0]);
		else
			ret.Format("V-ROC[%d]", (int)m_pgraph->cInfo.awValue[0]);
	}
	else if (nIndex == 1)
		ret.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[1]);
	else
		ret = _T("");

	return ret;
}

void CLROC::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	if (m_pgraph->wGKind == GK_PROC)
		tmpstr.Format("P-ROC[%d]", (int)m_pgraph->cInfo.awValue[0]);
	else
		tmpstr.Format("V-ROC[%d]", (int)m_pgraph->cInfo.awValue[0]);
	arTitle.Add(tmpstr);
	tmpstr.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[1]);
	arTitle.Add(tmpstr);
}
