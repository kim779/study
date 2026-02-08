#include "stdafx.h"
#include "BEnvelope.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBEnvelope::CBEnvelope(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
: CBongBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)

{
	class COrgData *pOrgData;
	if (m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
	{
		m_iDataKey = pDataMgr->CreateAppData(pOrgData, m_iDataKey, GetGraphKind(),
			&m_pgraph->cInfo, m_pgraph->wCalOption);
	}
}

CBEnvelope::~CBEnvelope()
{
	if (m_iDataKey > 0)
		m_pDataMgr->DetachData(m_iDataKey);
}

void CBEnvelope::DrawGraph(CDC* pDC)
{
	m_RgnInfo.iCount = 0;

	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return;

	if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_PRC)
	{
		m_dYMax = m_pCoDraw->PMax;
		m_dYMin = m_pCoDraw->PMin;
		if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
			return;
	}
	else
	{
		int	start = m_iDispSPos;
		int	end = m_iDispEPos;
		if (!(m_dwDspOpt & DO_SCREEN))
		{
			start = 0;
			end = m_iTtlDtCnt;
		}

		if (!pAppData->CalculateMinMax(start, end, &m_dYMin, &m_dYMax))
			return;
	}

//	CBongBase::DrawTick(pDC);
	drawChart(pDC);
}

void CBEnvelope::drawChart(CDC* pDC)
{
	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return;

	CString	dispText;
	double	*pValue = new double[m_iTtlDtCnt];

	int btLStyle = 0;
	int btLWidth = 0;
	int lcolor = 0;

	int	nCount = pAppData->GetDataCount();

	for ( int ii = 0 ; ii < nCount ; ii++)
	{
		for ( int jj = m_iDispSPos ; jj < m_iDispEPos ; jj++ )
			pValue[jj] = pAppData->GetVal(ii, jj);

		btLStyle = m_pgraph->cInfo.aline[ii].btLStyle;
		btLWidth = m_pgraph->cInfo.aline[ii].btLWidth;
		lcolor = m_pgraph->cInfo.aline[ii].rgbLColor;

		CBongBase::DrawLine(pDC, m_dYMax, m_dYMin, pValue, btLStyle, btLWidth, lcolor, ii);

		if (ii == 0)
			dispText.Format("Envelope[기간:%d,승수:%d] 상한선 ", (int)m_pgraph->cInfo.awValue[0], (int)m_pgraph->cInfo.awValue[1]);			
		else if (ii == 1)
			dispText.Format("중심선 ");
		else
			dispText.Format("하한선");			
		DrawTickIndex(pDC, dispText, lcolor, true);
		m_RgnInfo.iCount++;
	}

	delete[] pValue;
}

CString CBEnvelope::GetDisplayPosData(CPoint pt, bool bGrid)
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
	char*	title[] = {"Envelope상한", "Envelope중심", "Envelope하한"};

	int	nCount = pAppData->GetDataCount();
	
	char	split = ':';
	if (!bGrid)	split = '\t';
	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		val = pAppData->GetVal(ii, dataPos);

		if (val == GD_UNUSED)
			continue;

		FormatVariableComma(tmpS, val, val);
		tmpS2.Format("%s%c%s", title[ii], split, tmpS);

		if (!unitStr.IsEmpty())
			unitStr += "\t" + tmpS2;
		else
			unitStr = tmpS2;
	}

	return unitStr;
}

CString CBEnvelope::ExcelData(int idx)
{
	CString	dispStr = _T("");
	CString	tmpS = _T("");
	char*	title[] = {"Envelope상한", "Envelope중심", "Envelope하한"};	
	int	nSize = sizeof(title) / sizeof(char*);
	
	if (idx < 0)
	{
		
		for ( int ii = 0 ; ii < nSize ; ii++ )
		{
			if (ii != 0)
				dispStr += _T("\t");
			
			dispStr += title[ii];			
		}		
	}
	else
	{
		class CAppData *pAppData;

		if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
			return dispStr;

		for ( int ii = 0 ; ii < nSize ; ii++ )
		{
			if (ii != 0)
				dispStr += _T("\t");

			double	val = pAppData->GetVal(ii, idx);

			if (val == GD_UNUSED)
				tmpS = _T("0");
			else
				FormatVariableComma(tmpS, val, 0, 0);

			dispStr += tmpS;
		}
	}
	return dispStr;	
}