// LSonaM.cpp: implementation of the CLSonaM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LSonaM.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLSonaM::CLSonaM(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	if (m_iUnit != GU_CODE && m_iUnit != GU_COFUT && m_iUnit != GU_COOPT)
		m_iValidDigit = 2;
}

CLSonaM::~CLSonaM()
{

}

CString CLSonaM::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
		ret.Format("SONA Momentum[%d,%d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1]);
	else if (nIndex == 1)
		ret.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[2]);
	else
		ret = _T("");

	return ret;
}

void CLSonaM::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	tmpstr.Format("SONAM[%d,%d]", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1]);
	arTitle.Add(tmpstr);
	tmpstr.Format("Signal[%d]", (int)m_pgraph->cInfo.awValue[2]);
	arTitle.Add(tmpstr);
}
