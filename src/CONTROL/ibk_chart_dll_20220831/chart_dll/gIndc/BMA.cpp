#include "stdafx.h"
#include "BMA.h"
#include "../gData/DataMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBMA::CBMA(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay)
: CBongBase(pView, pParent, pDataMgr, pInfo, graph, dispDay)
{
	int kind = GetGraphKind();
	if (kind == GK_NET)
	{
		if (m_pgraph->cInfo.awValue[0] > 0)
		{
			m_dayMAV = int(m_pgraph->cInfo.awValue[0]);
			if (m_dayMAV <= 0)
				m_dayMAV = 5;
		}
		if (m_pgraph->cInfo.awValue[1] > 0)
		{
			m_gap = int(m_pgraph->cInfo.awValue[1]);
			if (m_gap < 0 || m_gap > 20)
				m_gap = 20;
		}
		if (m_pgraph->cInfo.awValue[2] > 0)
		{
			m_netCnt = int(m_pgraph->cInfo.awValue[2]);
			if (m_netCnt <= 0 || m_netCnt > MAX_NET)
				m_netCnt = MAX_NET;
		}
	}
	else if (kind == GK_VMA || kind == GK_AMA)
		m_iValidDigit = 0;

	class COrgData *pOrgData;
	if (m_pDataMgr->GetOrgData(m_iDataKey, pOrgData))
	{
		m_iDataKey = pDataMgr->CreateAppData(pOrgData, m_iDataKey, GetGraphKind(), &m_pgraph->cInfo, m_pgraph->wCalOption);
	}
}

CBMA::~CBMA()
{
	if (m_iDataKey > 0)
		m_pDataMgr->DetachData(m_iDataKey);
}

void CBMA::DrawGraph(CDC* pDC)
{
	m_RgnInfo.iCount = 0;

	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return;

	if (m_iGrpKind == GK_VMA)
	{
		if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_VOL)
		{
			m_dYMax = m_pCoDraw->VMax;
			m_dYMin = m_pCoDraw->VMin;
		}
	}
	else if (m_iGrpKind == GK_AMA)
	{
		if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_AMT)
		{
			m_dYMax = m_pCoDraw->AMax;
			m_dYMin = m_pCoDraw->AMin;
		}
	}
	else
	{
		if (CIndcBase::m_pCoDraw && CIndcBase::m_pCoDraw->coWork == MC_PRC)
		{
			m_dYMax = m_pCoDraw->PMax;
			m_dYMin = m_pCoDraw->PMin;
		}
	}

	if (m_dYMax == DBL_MIN || m_dYMin == DBL_MAX)
		return;

//	CBongBase::DrawTick(pDC);

//	int gKind = GetGraphKind();
	switch (m_iGrpKind)
	{
	case GK_PMA:
	case GK_VMA:
	case GK_AMA:
		DrawMA(pDC);
		break;
	case GK_NET:
		DrawNET(pDC);
		break;
	}
	
}

void CBMA::DrawNET(CDC *pDC)
{
	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return;

	CString	dispText;
	double	*pValue = new double[m_iTtlDtCnt + 1];

	int btLStyle = m_pgraph->cInfo.aline[0].btLStyle;
	int btLWidth = m_pgraph->cInfo.aline[0].btLWidth;
	int lcolor = m_pgraph->cInfo.aline[0].rgbLColor;
	for (int day = m_dayMAV, ii = 0; ii < m_netCnt; day += m_gap, ii++)
	{
		if ( m_iTtlDtCnt < m_dayMAV || m_dayMAV <= 0)
			continue;

		for (int jj = m_iDispSPos; jj < m_iDispEPos; jj++)
			pValue[jj] = pAppData->GetVal(ii, jj);

		CBongBase::DrawLine(pDC, m_dYMax, m_dYMin, pValue, btLStyle, btLWidth, lcolor, ii);

		if (ii == 0)
		{
			dispText.Format("그물차트 시작이평 %d, 증가값 %d, 이평개수 %d", m_dayMAV, m_gap, m_netCnt);
			DrawTickIndex(pDC, dispText, lcolor, true);
		}
		m_RgnInfo.iCount++;
	}

	delete[] pValue;
}

void CBMA::DrawMA(CDC *pDC)
{	
	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return;

	CString	dispText;
	DWORD	chkbit = 0x00000001;
	double	*pValue = new double[m_iTtlDtCnt + 1];
	for (int ii = 0; ii < MAX_LINE; ii++, chkbit <<= 1)
	{
		if (!(m_pgraph->cInfo.wCOption & chkbit))
			continue;
		
		if ( m_iTtlDtCnt < int(m_pgraph->cInfo.awValue[ii]) 
			|| int(m_pgraph->cInfo.awValue[ii]) <= 0)
			continue;

		for (int jj = m_iDispSPos; jj < m_iDispEPos; jj++)
			pValue[jj] = pAppData->GetVal(ii, jj);

		int btLStyle = m_pgraph->cInfo.aline[ii].btLStyle;
		int btLWidth = m_pgraph->cInfo.aline[ii].btLWidth;
		int lcolor = m_pgraph->cInfo.aline[ii].rgbLColor;

		CBongBase::DrawLine(pDC, m_dYMax, m_dYMin, pValue, btLStyle, btLWidth, lcolor, ii);

//		switch (gKind)
//		{
//		case GK_PMA:
//			dispText.Format("MA[%d]", int(m_pgraph->cInfo.awValue[ii]));
//			break;
//		case GK_VMA:
//			dispText.Format("MA[%d]", int(m_pgraph->cInfo.awValue[ii]));
//			break;
//		case GK_AMA:
//			dispText.Format("MA[%d]", int(m_pgraph->cInfo.awValue[ii]));
//			break;
//		}
//		
//		DrawTickIndex(pDC, dispText, lcolor, true);
		m_RgnInfo.iCount++;
	}

	delete[] pValue;
}

CString CBMA::GetDisplayPosData(CPoint pt, bool bGrid)
{
	if (pt.x <= m_rectGrp.left || pt.x >= m_rectGrp.right)
		return _T("");

	class CAppData *pAppData;
	if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
		return "";

	if (GetGraphKind() == GK_NET)
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
	DWORD	chkbit = 0x00000001;
	
	char	split = ':';
	if (!bGrid)	split = '\t';
	for (int ii = 0; ii < MAX_LINE; ii++, chkbit <<= 1)
	{
		if (!(m_pgraph->cInfo.wCOption & chkbit))
			continue;
		
		if ( m_iTtlDtCnt < int(m_pgraph->cInfo.awValue[ii]) 
			|| int(m_pgraph->cInfo.awValue[ii]) <= 0)
			continue;

		val = pAppData->GetVal(ii, dataPos);

		if (val == GD_UNUSED)
			continue;

		if (m_iUnit != GU_INDEX && m_iGrpKind == GK_VMA)
		{
			if (val >= dispMUNIT)
			{
				FormatVariableComma(tmpS, val, dispMUNIT + 1, 14);
				tmpS += "백만";
			}
			else if (val >= dispUNIT)
			{
				FormatVariableComma(tmpS, val, dispUNIT + 1, 14);
				tmpS += "천";
			}
			else
				FormatVariableComma(tmpS, val, 100, 14);
				
		}
		else
		{
			FormatVariableComma(tmpS, val, 100, 14);
		}

		if (m_iUnit == GU_INDEX)
		{
			switch (m_iGrpKind)
			{
			case GK_VMA:
				tmpS2.Format("거래이평[%d]%c%s천", int(m_pgraph->cInfo.awValue[ii]), split, tmpS);
				break;
			case GK_AMA:
				tmpS2.Format("대금이평[%d]%c%sM", int(m_pgraph->cInfo.awValue[ii]), split, tmpS);
				break;
			case GK_PMA:
				tmpS2.Format("가격이평[%d]%c%s", int(m_pgraph->cInfo.awValue[ii]), split, tmpS);
				break;
			}
		}
		else
		{
			switch (m_iGrpKind)
			{
			case GK_VMA:
				tmpS2.Format("거래이평[%d]%c%s", int(m_pgraph->cInfo.awValue[ii]), split, tmpS);
				break;
			case GK_AMA:
				//tmpS2.Format("대금이평[%d]%c%sM", int(m_pgraph->cInfo.awValue[ii]), split, tmpS);
				tmpS2.Format("대금이평[%d]%c%s백만", int(m_pgraph->cInfo.awValue[ii]), split, tmpS);
				break;
			case GK_PMA:
				tmpS2.Format("가격이평[%d]%c%s", int(m_pgraph->cInfo.awValue[ii]), split, tmpS);
				break;
			}
		}

		if (!unitStr.IsEmpty())
			unitStr += "\t" + tmpS2;
		else
			unitStr = tmpS2;
	}

	return unitStr;
}

CString CBMA::ExcelData(int idx)
{
	CString	dispStr = _T("");
	CString	name = _T("");
	switch (m_iGrpKind)
	{
	case GK_PMA:	name = _T("가격이평");	break;
	case GK_VMA:	name = _T("거래이평");	break;
	case GK_AMA:	name = _T("대금이평");	break;
	default:	return dispStr;
	}

	CString	tmpS;
	DWORD	chkbit = 0x00000001;
	if (idx < 0)
	{
		for (int ii = 0; ii < MAX_LINE; ii++, chkbit <<= 1)
		{
			if (!(m_pgraph->cInfo.wCOption & chkbit))		continue;
			if ( m_iTtlDtCnt < int(m_pgraph->cInfo.awValue[ii]))	continue;

			if (!dispStr.IsEmpty())	dispStr += _T("\t");
			tmpS.Format("%s[%d]", name, int(m_pgraph->cInfo.awValue[ii]));
			dispStr += tmpS;
		}
	}
	else
	{
		class CAppData *pAppData;
		if (!m_pDataMgr->GetData(m_iDataKey, pAppData))
			return dispStr;

		for (int ii = 0; ii < MAX_LINE; ii++, chkbit <<= 1)
		{
			if (!(m_pgraph->cInfo.wCOption & chkbit))		continue;
			if ( m_iTtlDtCnt < int(m_pgraph->cInfo.awValue[ii]))	continue;

			if (!dispStr.IsEmpty())	dispStr += _T("\t");

			double	val = pAppData->GetVal(ii, idx);
			if (val == GD_UNUSED)
				tmpS = _T("0");
			else
			{
				FormatVariableComma(tmpS, val, 0, 0);
				if (m_iUnit == GU_INDEX)
				{
					switch (m_iGrpKind)
					{
					//case GK_VMA:	tmpS += "K";	break;
					case GK_VMA:	tmpS += "천";	break;
					//case GK_AMA:	tmpS += "M";	break;
					case GK_AMA:	tmpS += "백만";	break;
					case GK_PMA:	break;
					}
				}
				else
				{
					switch (m_iGrpKind)
					{
					case GK_VMA:	break;
					case GK_AMA:	tmpS += "M";	break;
					case GK_PMA:	break;
					}
				}
			}

			dispStr += tmpS;
		}
	}

	return dispStr;
}

void CBMA::SetGrpInfo(struct _graph *pGrp, struct _envInfo *pInfo, struct _CoDraw *pCoDraw)
{
	bool bChangeVar = false;
	if (pGrp)
	{
		CopyMemory(m_pgraph, pGrp, SZ_GRAPH);
		m_crText = m_pgraph->rgbLabel;

		m_iUTickNum = 0;
		DWORD	chkbit = 0x00000001;
		for (int ii = 0; ii < MAX_LINE; ii++, chkbit <<= 1)
		{
			if (m_pgraph->tInfo.wCOption & chkbit)
				m_iUTickNum++;
		}

		bChangeVar = true;
	}

	bool bChangeOption = false;
	if (pInfo)
	{
		m_wEnvOpt = pInfo->wEnvOption;
		m_dwDspOpt = pInfo->display.dwDspOption;
		m_crBG = pInfo->display.rgbChartBG;
		m_crFG = pInfo->display.rgbChartEdge;
		CopyMemory(&m_lineH, &pInfo->display.hline, SZ_LINE);
		CopyMemory(&m_lineV, &pInfo->display.vline, SZ_LINE);	
		CopyMemory(&m_lineDate, &pInfo->display.dline, SZ_LINE);	
		CopyMemory(m_atick, pInfo->display.atick, sizeof(pInfo->display.atick));

		CalculateTickInfo();
		bChangeOption = true;
	}

	if (bChangeVar || bChangeOption)
	{
		class CAppData *pAppData;
		if (m_pDataMgr->GetData(m_iDataKey, pAppData))
		{
			pAppData->SetVariable(&m_pgraph->cInfo, m_pgraph->wCalOption);
			CalculateMinMax(true);
		}

		if (GetGraphKind() == GK_NET)
		{
			if (m_pgraph->cInfo.awValue[0] > 0)
			{
				m_dayMAV = int(m_pgraph->cInfo.awValue[0]);
				if (m_dayMAV <= 0)
					m_dayMAV = 5;
			}
			if (m_pgraph->cInfo.awValue[1] > 0)
			{
				m_gap = int(m_pgraph->cInfo.awValue[1]);
				if (m_gap < 0 || m_gap > 20)
					m_gap = 20;
			}
			if (m_pgraph->cInfo.awValue[2] > 0)
			{
				m_netCnt = int(m_pgraph->cInfo.awValue[2]);
				if (m_netCnt <= 0 || m_netCnt > MAX_NET)
					m_netCnt = MAX_NET;
			}
		}
	}

	if (pCoDraw)
		m_pCoDraw = pCoDraw;
}

void CBMA::DrawLegend(CDC* pDC)
{
	m_RgnInfo.iCount= 0;

	CString	dispText;
	DWORD	chkbit = 0x00000001;
	for (int ii = 0; ii < MAX_LINE; ii++, chkbit <<= 1)
	{
		if (!(m_pgraph->cInfo.wCOption & chkbit))
			continue;
		
		if ( m_iTtlDtCnt < int(m_pgraph->cInfo.awValue[ii]) 
			|| int(m_pgraph->cInfo.awValue[ii]) <= 0)
			continue;

		switch (m_iGrpKind)
		{
		case GK_PMA:
		case GK_VMA:
		case GK_AMA:
			dispText.Format("MA[%d]", int(m_pgraph->cInfo.awValue[ii]));
			DrawTickIndex(pDC, dispText, m_pgraph->cInfo.aline[ii].rgbLColor, true);
			break;
//		case GK_NET:
//			if (ii == 0)
//			{
//				dispText.Format("그물차트 시작이평 %d, 증가값 %d, 이평개수 %d", m_dayMAV, m_gap, m_netCnt);
//				DrawTickIndex(pDC, dispText, m_pgraph->cInfo.aline[0].rgbLColor, true);
//			}
//			break;
		}
		m_RgnInfo.iCount++;

	}
}