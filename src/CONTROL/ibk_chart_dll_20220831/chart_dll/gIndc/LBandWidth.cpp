// LBandWidth.cpp: implementation of the CLBandWidth class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LBandWidth.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLBandWidth::CLBandWidth(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	m_iValidDigit = 4;
}

CLBandWidth::~CLBandWidth()
{

}

CString CLBandWidth::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
		ret.Format("Band Width[±â°£:%d,½Â¼ö:%d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1]);
	else if (nIndex == 1)
		ret.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[2]);
	else
		ret = _T("");

	return ret;
}

void CLBandWidth::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	tmpstr.Format("Band Width");
	arTitle.Add(tmpstr);
	tmpstr.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[2]);
	arTitle.Add(tmpstr);
}
