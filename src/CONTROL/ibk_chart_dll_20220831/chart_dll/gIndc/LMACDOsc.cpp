#include "stdafx.h"
#include "axisgindc.h"
#include "LMACDOsc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CLMACDOsc::CLMACDOsc(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
}

CLMACDOsc::~CLMACDOsc()
{

}

CString CLMACDOsc::MakeLegend(int nIndex)
{
	CString	ret;

	if (nIndex == 0)
		ret.Format("MACD Osc[%d, %d, %d]", (int)m_graph->cInfo.value[0], (int)m_graph->cInfo.value[1], (int)m_graph->cInfo.value[2]);
	else
		ret = _T("");

	return ret;
}

void CLMACDOsc::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	tmpstr.Format("MACD Osc[%d, %d, %d]", (int)m_graph->cInfo.value[0], (int)m_graph->cInfo.value[1], (int)m_graph->cInfo.value[2]);
	arTitle.Add(tmpstr);	
}
