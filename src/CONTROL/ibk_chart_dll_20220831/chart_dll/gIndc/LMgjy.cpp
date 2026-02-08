// LMgjy.cpp: implementation of the CLMgjy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LMgjy.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLMgjy::CLMgjy(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
	: CLineBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{

}

CLMgjy::~CLMgjy()
{

}

CString CLMgjy::MakeLegend(int nIndex)
{
	CString	ret;

	ret = _T("미결제약정");

	return ret;
}

void CLMgjy::MakeTitle(CStringArray& arTitle)
{
	arTitle.RemoveAll();

	CString	tmpstr;

	tmpstr.Format("미결제약정");
	arTitle.Add(tmpstr);
}

CString CLMgjy::GetDisplayPosData(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData) || pAppData->GetDataCount() <= 0)
		return "";

	int DrawWidth = m_rectGrp.Width();
	int xPosition = pt.x - m_rectGrp.left;

	double szOneDay = double(DrawWidth) / double(m_iDispDtCnt);
	int dataPos = int(double(xPosition) / szOneDay);
	dataPos += m_iDispSPos;

	if (dataPos < 0 || dataPos >= m_iTtlDtCnt)
		return _T("");

	CString	unitStr = _T(""), tmpS, tmpS2;
	double	val = 0.0;
	CStringArray	arTitle;

	arTitle.RemoveAll();
	MakeTitle(arTitle);	

	int	nCount = pAppData->GetDataCount();
	
	char	split = ':';
	if (!bGrid)	split = '\t';
	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		val = pAppData->GetVal(ii, dataPos);

		if (val == GD_UNUSED)
			continue;

		CIndcBase::FormatVariableComma(tmpS, val, 1, 100);
		tmpS2.Format("%s%c%s", arTitle.GetAt(ii), split, tmpS);

		if (!unitStr.IsEmpty())
			unitStr += "\t" + tmpS2;
		else
			unitStr = tmpS2;
	}

	return unitStr;
}

