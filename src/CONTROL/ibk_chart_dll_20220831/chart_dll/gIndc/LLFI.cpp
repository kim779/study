#include "stdafx.h"
#include "LLFI.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLLFI::CLLFI(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	if (GetDataUnit() != GU_CODE && GetDataUnit() != GU_COFUT && GetDataUnit() != GU_COOPT)
		m_iValidDigit = 2;
}

CLLFI::~CLLFI()
{

}

CString CLLFI::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
		ret = _T("LFI");
	else if (nIndex == 1)
		ret.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[0]);
	else
		ret = _T("");

	return ret;
}

void CLLFI::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	tmpstr.Format("LFI");
	arTitle.Add(tmpstr);
	tmpstr.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[0]);
	arTitle.Add(tmpstr);
}
