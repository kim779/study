#include "stdafx.h"
#include "LMAO.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLMAO::CLMAO(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_iValidDigit = 2;
}

CLMAO::~CLMAO()
{

}

CString CLMAO::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
	{
		if (m_pgraph->wGKind == GK_PMAO)
			ret.Format("PO[%d, %d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1]);
		else if (m_pgraph->wGKind == GK_VMAO)
			ret.Format("VO[%d, %d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1]);
	}
	else if (nIndex == 1)
		ret.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[2]);
	else
		ret = _T("");

	return ret;
}

void CLMAO::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	if (m_pgraph->wGKind == GK_PMAO)
		tmpstr.Format("PO[%d, %d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1]);
	else if (m_pgraph->wGKind == GK_VMAO)
		tmpstr.Format("VO[%d, %d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1]);
	arTitle.Add(tmpstr);
	tmpstr.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[2]);
	arTitle.Add(tmpstr);
}

