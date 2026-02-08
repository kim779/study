// DataFormat.cpp: implementation of the CDataFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axisGMain.h"
#include "DataFormat.h"
#include "ObjMgr.h"

#include "PnChart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_DATA_COUNT_S	"009999"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataFormat::CDataFormat(class CObjMgr *pObjMgr)
{
	m_pObjMgr = pObjMgr;
	m_pEnvInfo = NULL;
	m_pcGrpHeader = NULL;

	m_pEnvInfo = new _envInfo;
	ZeroMemory(m_pEnvInfo, SZ_ENVINFO);
	m_pcGrp = NULL;
	m_iRequestCnt = 0;
	m_iVolumeOption = 0;

	ZeroMemory(m_acPVGrp, sizeof(m_acPVGrp));
	ZeroMemory(m_acRCGrp, sizeof(m_acRCGrp));
	ZeroMemory(m_acTNGrp, sizeof(m_acTNGrp));
	ZeroMemory(m_acPFGrp, sizeof(m_acPFGrp));
	ZeroMemory(m_acVolGrp, sizeof(m_acVolGrp));
	ZeroMemory(m_acKGGrp, sizeof(m_acKGGrp));
	ZeroMemory(m_acRKGrp, sizeof(m_acRKGrp));
	ZeroMemory(&m_grpAVol, sizeof(m_grpAVol));
	ZeroMemory(&m_grpBalance, sizeof(m_grpBalance));
	ZeroMemory(&m_grpKagi, sizeof(m_grpKagi));

	ReadEnvPath();
}

CDataFormat::~CDataFormat()
{
	if (m_pEnvInfo)		{	delete m_pEnvInfo;	m_pEnvInfo = NULL;	}
	if (m_pcGrpHeader)	{	delete[] m_pcGrpHeader;	m_pcGrpHeader = NULL;	}
	if (m_pcGrp)		{	delete[] m_pcGrp;		m_pcGrp = NULL;		}
}

void CDataFormat::InitData()
{
	struct _region	*pRgn;
	struct _graph	*pGrp;

	COLORREF	color = PALETTERGB(0, 0, 0);

	// 기타차트 초기화
	pRgn = (struct _region *)m_acPVGrp;
	pRgn->wRLeft = 0;
	pRgn->wRTop = 0;
	pRgn->wRRight = 10000;
	pRgn->wRBottom = 10000;
	pRgn->btRKey = 1;
	pGrp = (struct _graph *)&m_acPVGrp[SZ_REGION];
	pGrp->btGType = GT_BONG;
	pGrp->wGKind = GK_PV;
	pGrp->wKey = 1;
	pGrp->btDataKey = 1;
	pGrp->btRgnKey = 1;
	pGrp->dwDraOption = GO_TKLEFT | GO_TKRIGHT | GO_TKBOTTOM;
	CopyMemory(m_acRCGrp, m_acPVGrp, SZ_REGION + SZ_GRAPH);
	CopyMemory(m_acTNGrp, m_acPVGrp, SZ_REGION + SZ_GRAPH);
	CopyMemory(m_acPFGrp, m_acPVGrp, SZ_REGION + SZ_GRAPH);
	CopyMemory(m_acKGGrp, m_acPVGrp, SZ_REGION + SZ_GRAPH);
	CopyMemory(m_acRKGrp, m_acPVGrp, SZ_REGION + SZ_GRAPH);

////////////////////////////////////////////////////////////
	m_grpBalance.btGType = GT_BONG;
	m_grpBalance.wGKind = GK_BALANCE;
	m_grpBalance.wKey = 0;
	m_grpBalance.btDataKey = 1;
	m_grpBalance.btRgnKey = 1;
	m_grpBalance.rgbLabel = color;

	m_grpAVol.btGType = GT_BONG;
	m_grpAVol.wGKind = GK_AVOL;
	m_grpAVol.wKey = 0;
	m_grpAVol.btDataKey = 1;
	m_grpAVol.btRgnKey = 1;
	m_grpAVol.rgbLabel = color;

	//m_grpKagi.btGType = GT_BONG;
	m_grpKagi.btGType = GT_VARS;
	m_grpKagi.wGKind = GK_KAGI;
	m_grpKagi.wKey = 0;
	m_grpKagi.btDataKey = 1;
	m_grpKagi.btRgnKey = 1;
	m_grpKagi.rgbLabel = color;

	
////////////////////////////////////////////////////////////

	pGrp = (struct _graph *)&m_acRCGrp[SZ_REGION];
	pGrp->btGType = GT_LINE;	pGrp->wGKind = GK_CLOCK;
	pGrp = (struct _graph *)&m_acTNGrp[SZ_REGION];
	pGrp->btGType = GT_VARS;	pGrp->wGKind = GK_THREE;
	pGrp = (struct _graph *)&m_acPFGrp[SZ_REGION];
	pGrp->btGType = GT_VARS;	pGrp->wGKind = GK_PNF;
	pGrp = (struct _graph *)&m_acKGGrp[SZ_REGION];
	pGrp->btGType = GT_VARS;	pGrp->wGKind = GK_KAGI;
	pGrp = (struct _graph *)&m_acRKGrp[SZ_REGION];
	pGrp->btGType = GT_VARS;	pGrp->wGKind = GK_RENKO;

	// 볼륨차트 초기화
	pRgn = (struct _region *)m_acVolGrp;
	pRgn->wRLeft = 0;
	pRgn->wRTop = 0;
	pRgn->wRRight = 10000;
	pRgn->wRBottom = 7000;
	pRgn->btRKey = 1;
	pRgn = (struct _region *)&m_acVolGrp[SZ_REGION];
	pRgn->wRLeft = 0;
	pRgn->wRTop = 7000;
	pRgn->wRRight = 10000;
	pRgn->wRBottom = 10000;
	pRgn->btRKey = 2;
	pGrp = (struct _graph *)&m_acVolGrp[SZ_REGION*2];
	pGrp->btGType = GT_BONG;
	pGrp->wGKind = GK_EQV;
	pGrp->wKey = 1;
	pGrp->btDataKey = 1;
	pGrp->btRgnKey = 1;
	pGrp->dwDraOption = GO_TKLEFT | GO_TKRIGHT | GO_TKBOTTOM;
	pGrp = (struct _graph *)&m_acVolGrp[SZ_REGION*2 + SZ_GRAPH];
	pGrp->btGType = GT_BONG;
	pGrp->wGKind = GK_EQVV;
	pGrp->wKey = 2;
	pGrp->btDataKey = 1;
	pGrp->btRgnKey = 2;
	pGrp->dwDraOption = GO_TKLEFT | GO_TKRIGHT | GO_TKBOTTOM;

	SetSingleGrp(m_pEnvInfo->datainfo.wMainGrpKind);
}

void CDataFormat::VolumeInit()
{
	struct _graph* graph;
	struct _graph* mgraph = NULL;

	for (int ii = 0; ii < int(m_dwGrpCnt); ii++)
	{
		graph = (struct _graph*)&m_pcGrp[SZ_REGION * m_dwRgnCnt + SZ_GRAPH * ii];
		switch (graph->wGKind)
		{
			case GK_JPN:
			case GK_BAR:
			case GK_LIN:
			case GK_POLE:
				mgraph = graph;
				break;
		}
	}

	if (!mgraph)
		return;

	graph = (struct _graph*)&m_acVolGrp[SZ_REGION*2];
	graph->dwDraOption = mgraph->dwDraOption;
	graph->rgbLabel = mgraph->rgbLabel;
	CopyMemory(&graph->cInfo, &mgraph->cInfo, SZ_CHARTINFO);

	struct _graph	volgrp;
	volgrp.wGKind = GK_VOL;
	BOOL ret = (BOOL)m_pObjMgr->m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewChartInfo, getSGrpInfo), (long)&volgrp);
	if (ret)
	{
		graph = (struct _graph*)&m_acVolGrp[SZ_REGION*2 + SZ_GRAPH];
		graph->dwDraOption = volgrp.dwDraOption;
		graph->rgbLabel = volgrp.rgbLabel;
		CopyMemory(&graph->cInfo, &volgrp.cInfo, SZ_CHARTINFO);
	}
}

char* CDataFormat::GetGrpHeader(int *piLen)
{
	if (m_pcGrpHeader)
		delete[] m_pcGrpHeader;

	CString	strTemp;
	struct _dataH dataH;
	FillMemory(&dataH, SZ_DATAH, 0x20);
		
	m_iGrpHLen = 0;
	m_iRequestCnt = m_pObjMgr->m_pwndPnChart->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, GET_TOTAL_DAY), 0);

	strTemp.Format("%06d", m_iRequestCnt);
	CopyMemory(dataH.count, strTemp, strTemp.GetLength());
	dataH.dkey = 1;

	m_pObjMgr->m_pwndPnChart->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, getCalendar), (long)&dataH.pday);
	dataH.dindex = m_pEnvInfo->datainfo.btIndex;
	dataH.dunit = m_pEnvInfo->datainfo.btUnit;

	if (dataH.dindex == GI_MINUTE)
	{
		strTemp.Format("%0*d", sizeof(dataH.lgap), m_pEnvInfo->datainfo.wMGap);
		if (m_pObjMgr->m_pwndPnChart->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, GET_TODAY), 0))
		{
			dataH.dkind = DK_TODAY;
			dataH.option1 |= GUI_TODAY;
// 2007.01.03 schbang : 당일차트시 개수참조 
//			if (m_pEnvInfo->datainfo.btUnit == GU_CODE)
//				CopyMemory(dataH.count, MAX_DATA_COUNT_S, 6);
		}

	}
	else if (dataH.dindex == GI_TICK)
	{
		strTemp.Format("%0*d", sizeof(dataH.lgap), m_pEnvInfo->datainfo.wTGap);
		if (m_pObjMgr->m_pwndPnChart->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, GET_TODAY), 0))
		{
			dataH.dkind = DK_TODAY;
			dataH.option1 |= GUI_TODAY;
// 2007.01.03 schbang : 당일차트시 개수참조 
//			if (m_pEnvInfo->datainfo.btUnit == GU_CODE)
//				CopyMemory(dataH.count, MAX_DATA_COUNT_S, 6);
		}
	}
	else
		strTemp.Format("%0*d", sizeof(dataH.lgap), 0);

	CopyMemory(dataH.lgap, strTemp, strTemp.GetLength() );

	if (m_pEnvInfo->display.dwDspOption & DO_MOD)
		dataH.option1 |= GUI_MOD;
	m_iVolumeOption = ReadVolumeOption();
	dataH.option2 |= m_iVolumeOption;

//	BOOL bToday = (BOOL)m_pObjMgr->m_pwndPnChart->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, isTodayData), 0);
//	if (bToday)	dataH.option1 |= GUI_TODAY;

	CString requestCode = (LPCTSTR)m_pObjMgr->m_pwndPnChart->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, GET_VALID_CODE));
	if (!requestCode.IsEmpty())
		MakeHeader(requestCode);
	dataH.dunit = m_pEnvInfo->datainfo.btUnit;	// notice

	m_iGrpHLen = m_strDataSymbol.GetLength() + m_strOutSymbol.GetLength() + SZ_DATAH;
	m_pcGrpHeader = new char[m_iGrpHLen];

	CopyMemory(m_pcGrpHeader, m_strDataSymbol, m_strDataSymbol.GetLength());
	CopyMemory(&m_pcGrpHeader[m_strDataSymbol.GetLength()], (char *)&dataH, SZ_DATAH);
	CopyMemory(&m_pcGrpHeader[m_strDataSymbol.GetLength() + SZ_DATAH], m_strOutSymbol, m_strOutSymbol.GetLength());

	*piLen = m_iGrpHLen;

	return m_pcGrpHeader;
}

char* CDataFormat::GetGrpHeader2(int *len)
{
	if (m_pcGrpHeader)
		delete[] m_pcGrpHeader;

	CheckHeader2();
	if (m_iOutID2 == 0)
	{
		m_pcGrpHeader = NULL;
		*len = 0;
		return NULL;
	}

	MakeHeader2();

	CString	temp;
	struct _dataH	Datah;
	FillMemory(&Datah, SZ_DATAH, 0x20);
		
	m_iGrpHLen = 0;
	m_iRequestCnt = m_pObjMgr->m_pwndPnChart->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA,GET_TOTAL_DAY), 0);
	temp.Format("%06d", m_iRequestCnt);
	CopyMemory(Datah.count, temp, temp.GetLength());
	Datah.dkey = 1;

	m_pObjMgr->m_pwndPnChart->SendMessage(GEV_INPUT, 
		MAKEWPARAM(INPUT_DATA, getCalendar), (long)&Datah.pday);
	Datah.dindex = m_pEnvInfo->datainfo.btIndex;
	Datah.dunit = m_pEnvInfo->datainfo.btUnit;

	if (Datah.dindex == GI_MINUTE)
		temp.Format("%0*d", sizeof(Datah.lgap), m_pEnvInfo->datainfo.wMGap);
	else if (Datah.dindex == GI_TICK)
		temp.Format("%0*d", sizeof(Datah.lgap), m_pEnvInfo->datainfo.wTGap);
	else
		temp.Format("%0*d", sizeof(Datah.lgap), 0);
	CopyMemory(Datah.lgap, temp, temp.GetLength() );

	Datah.option2 |= m_iVolumeOption;

	m_iGrpHLen = m_strDataSymbol.GetLength() + m_strOutSymbol2.GetLength() + SZ_DATAH;
	m_pcGrpHeader = new char[m_iGrpHLen];

	CopyMemory(m_pcGrpHeader, m_strDataSymbol, m_strDataSymbol.GetLength());
	CopyMemory(&m_pcGrpHeader[m_strDataSymbol.GetLength()], (char *)&Datah, SZ_DATAH);
	CopyMemory(&m_pcGrpHeader[m_strDataSymbol.GetLength() + SZ_DATAH], m_strOutSymbol2, m_strOutSymbol2.GetLength());

	*len = m_iGrpHLen;

	return m_pcGrpHeader;
}

char* CDataFormat::GetGrpHeaderS(int *len)
{
	if (m_pcGrpHeader)
		delete[] m_pcGrpHeader;

	if (m_iOutIDS == 0)
	{
		*len = 0;
		return NULL;
	}

	MakeHeaderS();

	CString	temp;
	struct _dataH	Datah;
	FillMemory(&Datah, SZ_DATAH, 0x20);
		
	m_iGrpHLen = 0;
	m_iRequestCnt = m_pObjMgr->m_pwndPnChart->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA,GET_TOTAL_DAY), 0);
	temp.Format("%06d", m_iRequestCnt);
	CopyMemory(Datah.count, temp, temp.GetLength());
	Datah.dkey = 1;

	m_pObjMgr->m_pwndPnChart->SendMessage(GEV_INPUT, 
		MAKEWPARAM(INPUT_DATA, getCalendar), (long)&Datah.pday);
	Datah.dindex = m_pEnvInfo->datainfo.btIndex;
	Datah.dunit = m_pEnvInfo->datainfo.btUnit;

	if (Datah.dindex == GI_MINUTE)
		temp.Format("%0*d", sizeof(Datah.lgap), m_pEnvInfo->datainfo.wMGap);
	else if (Datah.dindex == GI_TICK)
		temp.Format("%0*d", sizeof(Datah.lgap), m_pEnvInfo->datainfo.wTGap);
	else
		temp.Format("%0*d", sizeof(Datah.lgap), 0);
	CopyMemory(Datah.lgap, temp, temp.GetLength() );

	Datah.option2 |= m_iVolumeOption;

	m_iGrpHLen = m_strDataSymbol.GetLength() + m_strOutSymbolS.GetLength() + SZ_DATAH;
	m_pcGrpHeader = new char[m_iGrpHLen];

	CopyMemory(m_pcGrpHeader, m_strDataSymbol, m_strDataSymbol.GetLength());
	CopyMemory(&m_pcGrpHeader[m_strDataSymbol.GetLength()], (char *)&Datah, SZ_DATAH);
	CopyMemory(&m_pcGrpHeader[m_strDataSymbol.GetLength() + SZ_DATAH], m_strOutSymbolS, m_strOutSymbolS.GetLength());

	*len = m_iGrpHLen;

	return m_pcGrpHeader;
}

struct _envInfo* CDataFormat::GetEnvInfo()
{ 
	return m_pEnvInfo; 
}

char* CDataFormat::GetGrpInfo(int& iRgnCnt, int& iGrpCnt)
{
	switch (m_pEnvInfo->datainfo.wMainGrpKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_POLE:

	case GK_BALANCE:
	case GK_AVOL:
//case GK_KAGI:
		iRgnCnt = m_dwRgnCnt;
		iGrpCnt = m_dwGrpCnt;
		return m_pcGrp; 

	case GK_EQV:
	case GK_CDV:
		iRgnCnt = 2; 
		iGrpCnt = 2;
		return	m_acVolGrp;
	case GK_THREE:
		iRgnCnt = 1;
		iGrpCnt = 1;
		return m_acTNGrp;
	case GK_PNF:
		iRgnCnt = 1; 
		iGrpCnt = 1; 
		return m_acPFGrp;
	case GK_CLOCK:
		iRgnCnt = 1; 
		iGrpCnt = 1; 
		return m_acRCGrp;
	case GK_PV:
		iRgnCnt = 1; 
		iGrpCnt = 1; 
		return m_acPVGrp;
	case GK_KAGI:
		iRgnCnt = 1; 
		iGrpCnt = 1; 
		return m_acKGGrp;
	case GK_RENKO:
		iRgnCnt = 1; 
		iGrpCnt = 1; 
		return m_acRKGrp;
	default:
		break;
	}

	return NULL;
}

char* CDataFormat::GetSaveGrpInfo(int &rcnt, int &gcnt)
{
	rcnt = m_dwRgnCnt; 
	gcnt = m_dwGrpCnt; 
	return m_pcGrp; 
}

void CDataFormat::SetFgraph(char *pEnvInfo, char *pGrpInfo, bool bInit)
{
	if (!pEnvInfo)
		return;

	if (m_pcGrp)
		delete[] m_pcGrp;

	CopyMemory(m_pEnvInfo, pEnvInfo, SZ_ENVINFO);
	m_dwRgnCnt = m_pEnvInfo->grpinfo[m_pEnvInfo->datainfo.btIndex - GI_DAY].wRgnCnt;
	m_dwGrpCnt = m_pEnvInfo->grpinfo[m_pEnvInfo->datainfo.btIndex - GI_DAY].wGrpCnt;

	m_pcGrp = new char[m_dwRgnCnt * SZ_REGION + m_dwGrpCnt * SZ_GRAPH + 1];
	CopyMemory(m_pcGrp, pGrpInfo, m_dwRgnCnt * SZ_REGION + m_dwGrpCnt * SZ_GRAPH);

	MakeHeader();
	
	if (bInit)
	{
		//m_pEnvInfo->display.dwDspOption &= ~DO_VIEWGRID;
		InitData();
	}
	else
	{
		SetSingleGrp(m_pEnvInfo->datainfo.wMainGrpKind);
	}

	VolumeInit();
}

void CDataFormat::SetFgraph(char *pGrpInfo, int rcnt, int gcnt)
{
	if (!pGrpInfo)
		return;

	if (m_pcGrp)
		delete[] m_pcGrp;

	m_dwRgnCnt = m_pEnvInfo->grpinfo[m_pEnvInfo->datainfo.btIndex - GI_DAY].wRgnCnt = rcnt;
	m_dwGrpCnt = m_pEnvInfo->grpinfo[m_pEnvInfo->datainfo.btIndex - GI_DAY].wGrpCnt = gcnt;
	
	m_pcGrp = new char[m_dwRgnCnt * SZ_REGION + m_dwGrpCnt * SZ_GRAPH + 1];
	CopyMemory(m_pcGrp, pGrpInfo, m_dwRgnCnt * SZ_REGION + m_dwGrpCnt * SZ_GRAPH);

	VolumeInit();
}

void CDataFormat::ChangeMain(int iKind)
{
	m_pEnvInfo->datainfo.wMainGrpKind = iKind;

	switch (m_pEnvInfo->datainfo.wMainGrpKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_POLE:

	case GK_EQV:
	case GK_CDV:

	case GK_BALANCE:
	case GK_AVOL:

	case GK_CLOCK:
	case GK_PV:
	case GK_THREE:
	case GK_PNF:
//	case GK_SWING:
	case GK_KAGI:
	case GK_RENKO:
		SetSingleGrp(m_pEnvInfo->datainfo.wMainGrpKind);
		break;
	default:
		break;
	}
}
 
void CDataFormat::ChangeIndex(int index, int gap)
{
	m_pEnvInfo->datainfo.btIndex = index;
	if (index == GI_MINUTE)
		m_pEnvInfo->datainfo.wMGap = gap;
	else if (index == GI_TICK)
		m_pEnvInfo->datainfo.wTGap = gap;

	MakeHeader();
}

void CDataFormat::ChangeUnit(int unit)
{
	m_pEnvInfo->datainfo.btUnit = unit;

	MakeHeader();
}

void CDataFormat::SaveDay(int iDay, bool bDisplay)
{
	int iUnit = m_pEnvInfo->datainfo.btUnit;
	int iIndex = m_pEnvInfo->datainfo.btIndex;

	int iUnitIndex = 0;
	switch (iUnit)
	{
	case GU_FUTURE:
	case GU_OPTION:
	case GU_CFUTURE: // 20090917 : ADD
	case GU_FFUTURE: // 20090917 : ADD
	case GU_COFUT:
	case GU_COOPT:
		iUnitIndex = GU_FUTURE-GU_CODE;
		break;
	case GU_CODE:
	case GU_INDEX:	
		iUnitIndex = iUnit - GU_CODE;
		break;
	case GU_FOREIGN:
		iUnitIndex = 3;
		break;
	default:
		break;
	}

	if (bDisplay)	
		m_pEnvInfo->datainfo.aaDayInfo[iUnitIndex][iIndex - GI_DAY].dwDisCnt = iDay;	// display
	else		
		m_pEnvInfo->datainfo.aaDayInfo[iUnitIndex][iIndex - GI_DAY].dwTotCnt = iDay;	// total
}

int CDataFormat::GetDispDay()
{
	int	unit = m_pEnvInfo->datainfo.btUnit;
	int	index = m_pEnvInfo->datainfo.btIndex;

	int	uIndex = 0;
	int	dispDay;
	switch (unit)
	{
	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
	case GU_FFUTURE: // 20090917 : ADD
	case GU_CFUTURE: // 20090917 : ADD
		uIndex = GU_FUTURE-GU_CODE;
		break;
	case GU_CODE:	case GU_INDEX:	
		uIndex = unit - GU_CODE;
		break;
	case GU_FOREIGN:
		uIndex = 3;
		break;
	default:
		break;
	}
	dispDay = m_pEnvInfo->datainfo.aaDayInfo[uIndex][index-GI_DAY].dwDisCnt;

	return dispDay;
}

void CDataFormat::SetSingleGrp(int iKind)
{
	struct _graph graph;
	graph.wGKind = iKind;
	BOOL bRet = (BOOL)m_pObjMgr->m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewChartInfo, getSGrpInfo), (long)&graph);

	struct _graph* pGrp = nullptr;

	switch (iKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_POLE:
		if (bRet)
		{
			for(int ii = 0; ii < int(m_dwGrpCnt); ii++)
			{
				pGrp = (struct _graph *)&m_pcGrp[SZ_REGION * m_dwRgnCnt + SZ_GRAPH * ii];
				switch (pGrp->wGKind)
				{
				case GK_JPN:
				case GK_BAR:
				case GK_LIN:
				case GK_POLE:
					{
						WORD wKey = pGrp->wKey;
						BYTE btDataKey = pGrp->btDataKey;
						BYTE btRgnKey = pGrp->btRgnKey;

						CopyMemory(pGrp, &graph, SZ_GRAPH);
						pGrp->wKey = (BYTE)wKey;
						pGrp->btDataKey = btDataKey;
						pGrp->btRgnKey = (BYTE)btRgnKey;
					}
					break;
				default:
					break;
				}
			}
		}
		return;
	case GK_EQV:
	case GK_CDV:
		if (bRet)
		{
			for (int ii = 0; ii < 2; ii++)
			{
				pGrp = (struct _graph *)&m_acVolGrp[SZ_REGION*2 + SZ_GRAPH*ii];
				if (pGrp->wGKind == GK_CDV || pGrp->wGKind == GK_EQV)
				{
					DWORD	wKey = pGrp->wKey;
					unsigned char	btDataKey = pGrp->btDataKey;
					DWORD	btRgnKey = pGrp->btRgnKey;

					CopyMemory(pGrp, &graph, SZ_GRAPH);
					pGrp->wKey = (BYTE)wKey;
					pGrp->btDataKey = btDataKey;
					pGrp->btRgnKey = (BYTE)btRgnKey;

					break;
				}
			}
		}
		return;
	case GK_PV:
		pGrp = (struct _graph *)&m_acPVGrp[SZ_REGION];
		break;
	case GK_CLOCK:
		pGrp = (struct _graph *)&m_acRCGrp[SZ_REGION];
		break;
	case GK_THREE:
		pGrp = (struct _graph *)&m_acTNGrp[SZ_REGION];
		break;
	case GK_PNF:
		pGrp = (struct _graph *)&m_acPFGrp[SZ_REGION];
		break;
	case GK_KAGI:
		pGrp = (struct _graph *)&m_acKGGrp[SZ_REGION];
		break;
	case GK_RENKO:
		pGrp = (struct _graph *)&m_acRKGrp[SZ_REGION];
		break;
	case GK_BALANCE:	
		if (bRet)
		{
			m_grpBalance.wCalOption = graph.wCalOption;
			m_grpBalance.dwDraOption = graph.dwDraOption;
			m_grpBalance.rgbLabel = graph.rgbLabel;
			CopyMemory(&m_grpBalance.cInfo, &graph.cInfo, SZ_CHARTINFO);
			CopyMemory(&m_grpBalance.tInfo, &graph.tInfo, SZ_TICKINFO);

			// 항상 봉차트를 기본으로
			graph.wGKind = GK_JPN;
			BOOL nret = (BOOL)m_pObjMgr->m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewChartInfo, getSGrpInfo), (long)&graph);
			if (!nret)	break;

			for(int ii = 0; ii < int(m_dwGrpCnt); ii++)
			{
				pGrp = (struct _graph *)&m_pcGrp[SZ_REGION*m_dwRgnCnt + SZ_GRAPH*ii];
				switch (pGrp->wGKind)
				{
				case GK_JPN:	case GK_BAR:	case GK_LIN:	case GK_POLE:
				case GK_EQV:	case GK_CDV:
					{
						DWORD	wKey = pGrp->wKey;
						unsigned char	btDataKey = pGrp->btDataKey;
						DWORD	btRgnKey = pGrp->btRgnKey;

						CopyMemory(pGrp, &graph, SZ_GRAPH);
						pGrp->wKey = (BYTE)wKey;
						pGrp->btDataKey = btDataKey;
						pGrp->btRgnKey = (BYTE)btRgnKey;
					}
					break;
				default:
					break;
				}
			}
		}
		return;
	case GK_AVOL:
		if (bRet)
		{
			m_grpAVol.wCalOption = graph.wCalOption;
			m_grpAVol.dwDraOption = graph.dwDraOption;
			m_grpAVol.rgbLabel = graph.rgbLabel;
			CopyMemory(&m_grpAVol.cInfo, &graph.cInfo, SZ_CHARTINFO);
			CopyMemory(&m_grpAVol.tInfo, &graph.tInfo, SZ_TICKINFO);

			// 항상 봉차트를 기본으로
			graph.wGKind = GK_JPN;
			BOOL nret = (BOOL)m_pObjMgr->m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewChartInfo, getSGrpInfo), (long)&graph);
			if (!nret)	break;

			for(int ii = 0; ii < int(m_dwGrpCnt); ii++)
			{
				pGrp = (struct _graph *)&m_pcGrp[SZ_REGION*m_dwRgnCnt + SZ_GRAPH*ii];
				switch (pGrp->wGKind)
				{
				case GK_JPN:	case GK_BAR:	case GK_LIN:	case GK_POLE:
				case GK_EQV:	case GK_CDV:
					{
						DWORD	wKey = pGrp->wKey;
						unsigned char	btDataKey = pGrp->btDataKey;
						DWORD	btRgnKey = pGrp->btRgnKey;

						CopyMemory(pGrp, &graph, SZ_GRAPH);
						pGrp->wKey = (BYTE)wKey;
						pGrp->btDataKey = btDataKey;
						pGrp->btRgnKey = (BYTE)btRgnKey;
					}
					break;
				default:
					break;
				}
			}
		}
		return;
	}
	
	if (bRet && !pGrp)
	{
		WORD wKey = pGrp->wKey;
		BYTE btDataKey = pGrp->btDataKey;
		BYTE btRgnKey = pGrp->btRgnKey;

		CopyMemory(pGrp, &graph, SZ_GRAPH);
		pGrp->wKey = (BYTE)wKey;
		pGrp->btDataKey = btDataKey;
		pGrp->btRgnKey = (BYTE)btRgnKey;
	}
}

int CDataFormat::GetMainGrpRegion()
{
	struct _graph	*graph;
	for (int ii = 0; ii < int(m_dwGrpCnt); ii++)
	{
		graph = (struct _graph*)&m_pcGrp[SZ_REGION*m_dwRgnCnt + SZ_GRAPH*ii];
		switch (graph->wGKind)
		{
		case GK_JPN:	case GK_LIN:	case GK_BAR:	case GK_POLE:
			return graph->btRgnKey;
		default:
			break;
		}
	}

	return -1;
}


void CDataFormat::MakeHeader_code()
{
	switch(m_pEnvInfo->datainfo.btIndex)
	{
	case GI_DAY:
	case GI_WEEK:
	case GI_MONTH:
		m_strDataSymbol.Format("?%s%c", DJD_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DJD_DAY, DJD_HMS, DJD_SIGA, DJD_KOGA, DJD_JEGA, DJD_JGGA, 
			DJD_GVOL, DJD_GAMT, DJD_LOCK, DJD_RATE);
		break;
//		case GI_WEEK:
//		case GI_MONTH:
//			m_strDataSymbol.Format("?%s%c", DJD_SYM, 0x7F);
//			m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
//				DJD_DAY, DJD_HMS, DJD_SIGA, DJD_KOGA, DJD_JEGA, DJD_JGGA, 
//				DJD_GVOL, DJD_GAMT);
//			break;
	case GI_MINUTE:
		m_strDataSymbol.Format("?%s%c", DJM_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DJM_DAY, DJM_HMS, DJM_SIGA, DJM_KOGA, DJM_JEGA, DJM_JGGA, 
			DJM_CVOL, DJM_GAMT);
		break;
	case GI_TICK:
		m_strDataSymbol.Format("?%s%c", DJT_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DJT_DAY, DJT_HMS, DJT_SIGA, DJT_KOGA, DJT_JEGA, DJT_JGGA,
			DJT_CVOL, DJT_GAMT);
		break;
	}
}

void CDataFormat::MakeHeader_index()
{
	switch(m_pEnvInfo->datainfo.btIndex)
	{
//	case GI_DAY:
	case GI_WEEK:
	case GI_MONTH:
		m_strDataSymbol.Format("?%s%c", DID_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DID_DAY, DID_HMS, DID_SIGA, DID_KOGA, DID_JEGA,
			DID_JGGA, DID_GVOL, DID_GAMT);
		break;
	case GI_DAY:	// 2007.01.01 ADR 차트때문에
		m_strDataSymbol.Format("?%s%c", DID_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DID_DAY, DID_HMS, DID_SIGA, DID_KOGA, DID_JEGA, DID_JGGA, 
			DID_GVOL, DID_GAMT, DID_UCNT, DID_DCNT);
		break;
	case GI_MINUTE:
		m_strDataSymbol.Format("?%s%c", DIM_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DIM_DAY, DIM_HMS, DIM_SIGA, DIM_KOGA, DIM_JEGA, DIM_JGGA, 
			DIM_CVOL, DIM_GAMT);
		break;
	case GI_TICK:
		m_strDataSymbol.Format("?%s%c", DIT_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DIT_DAY, DIT_HMS, DIT_SIGA, DIT_KOGA, DIT_JEGA, DIT_JGGA, 
			DIT_CVOL, DIT_GAMT);
		break;
	}
}

void CDataFormat::MakeHeader_future()
{
	switch(m_pEnvInfo->datainfo.btIndex)
	{
	case GI_DAY:
	case GI_WEEK:
	case GI_MONTH:
		m_strDataSymbol.Format("?%s%c", DFD_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DFD_DAY, DFD_HMS, DFD_SIGA, DFD_KOGA, DFD_JEGA, DFD_JGGA, 
			DFD_GVOL, DFD_GAMT, DFD_MGJY);
		break;
	case GI_MINUTE:
		m_strDataSymbol.Format("?%s%c", DFM_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DFM_DAY, DFM_HMS, DFM_SIGA, DFM_KOGA, DFM_JEGA, DFM_JGGA, 
			DFM_CVOL, DFM_GAMT, DFM_MGJY);
		break;
	case GI_TICK:
		m_strDataSymbol.Format("?%s%c", DFT_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DFT_DAY, DFT_HMS, DFT_SIGA, DFT_KOGA, DFT_JEGA, DFT_JGGA, 
			DFT_CVOL, DFT_GAMT, DFT_MGJY);
		break;
	}
}

void CDataFormat::MakeHeader_option()
{
	switch(m_pEnvInfo->datainfo.btIndex)
	{
	case GI_DAY:
	case GI_WEEK:
	case GI_MONTH:
		m_strDataSymbol.Format("?%s%c", DOD_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DOD_DAY, DOD_HMS, DOD_SIGA, DOD_KOGA, DOD_JEGA, DOD_JGGA, 
			DOD_GVOL, DOD_GAMT, DOD_MGJY);
		break;
	case GI_MINUTE:
		m_strDataSymbol.Format("?%s%c", DOM_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DOM_DAY, DOM_HMS, DOM_SIGA, DOM_KOGA, DOM_JEGA, DOM_JGGA, 
			DOM_CVOL, DOM_GAMT, DOM_MGJY);
		break;
	case GI_TICK:
		m_strDataSymbol.Format("?%s%c", DOT_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DOT_DAY, DOT_HMS, DOT_SIGA, DOT_KOGA, DOT_JEGA, DOT_JGGA, 
			DOT_CVOL, DOT_GAMT, DOT_MGJY);
		break;
	}
}

void CDataFormat::MakeHeader_cofuture()
{
	switch(m_pEnvInfo->datainfo.btIndex)
	{
	case GI_DAY:
	case GI_WEEK:
	case GI_MONTH:
		m_strDataSymbol.Format("?%s%c", DCFD_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DCFD_DAY, DCFD_HMS, DCFD_SIGA, DCFD_KOGA, DCFD_JEGA, DCFD_JGGA, 
			DCFD_GVOL, DCFD_GAMT, DCFD_MGJY);
		break;
	case GI_MINUTE:
		m_strDataSymbol.Format("?%s%c", DCFM_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DCFM_DAY, DCFM_HMS, DCFM_SIGA, DCFM_KOGA, DCFM_JEGA, DCFM_JGGA, 
			DCFM_CVOL, DCFM_GAMT, DCFM_MGJY);
		break;
	case GI_TICK:
		m_strDataSymbol.Format("?%s%c", DCFT_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DCFT_DAY, DCFT_HMS, DCFT_SIGA, DCFT_KOGA, DCFT_JEGA, DCFT_JGGA, 
			DCFT_CVOL, DCFT_GAMT, DCFT_MGJY);
		break;
	}
}

void CDataFormat::MakeHeader_cooption()
{
	switch(m_pEnvInfo->datainfo.btIndex)
	{
	case GI_DAY:
	case GI_WEEK:
	case GI_MONTH:
		m_strDataSymbol.Format("?%s%c", DCOD_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DCOD_DAY, DCOD_HMS, DCOD_SIGA, DCOD_KOGA, DCOD_JEGA, DCOD_JGGA, 
			DCOD_GVOL, DCOD_GAMT, DCOD_MGJY);
		break;
	case GI_MINUTE:
		m_strDataSymbol.Format("?%s%c", DCOM_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DCOM_DAY, DCOM_HMS, DCOM_SIGA, DCOM_KOGA, DCOM_JEGA, DCOM_JGGA, 
			DCOM_CVOL, DCOM_GAMT, DCOM_MGJY);
		break;
	case GI_TICK:
		m_strDataSymbol.Format("?%s%c", DCOT_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DCOT_DAY, DCOT_HMS, DCOT_SIGA, DCOT_KOGA, DCOT_JEGA, DCOT_JGGA, 
			DCOT_CVOL, DCOT_GAMT, DCOT_MGJY);
		break;
	}
}

void CDataFormat::MakeHeader_foreign()
{
	switch(m_pEnvInfo->datainfo.btIndex)
	{
	case GI_WEEK:	case GI_MONTH:
	case GI_DAY:
		m_strDataSymbol.Format("?%s%c", DSD_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DSD_DAY, DSD_SIGA, DSD_KOGA, DSD_JEGA, DSD_JGGA, DSD_GVOL);
		break;
	}
}

// 20090915 : ADD : 상품 선물 추가
void CDataFormat::MakeHeader_commoditiesfuture()
{
	switch(m_pEnvInfo->datainfo.btIndex)
	{
	case GI_DAY:
	case GI_WEEK:
	case GI_MONTH:
		m_strDataSymbol.Format("?%s%c", DGFD_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DGFD_DAY, DGFD_HMS, DGFD_SIGA, DGFD_KOGA, DGFD_JEGA, DGFD_JGGA, 
			DGFD_GVOL, DGFD_GAMT, DGFD_MGJY);
		break;
	case GI_MINUTE:
		m_strDataSymbol.Format("?%s%c", DGFM_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DGFM_DAY, DGFM_HMS, DGFM_SIGA, DGFM_KOGA, DGFM_JEGA, DGFM_JGGA, 
			DGFM_CVOL, DGFM_GAMT, DGFM_MGJY);
		break;
	case GI_TICK:
		m_strDataSymbol.Format("?%s%c", DGFT_SYM, 0x7F);
		m_strOutSymbol.Format("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\t",
			DGFT_DAY, DGFT_HMS, DGFT_SIGA, DGFT_KOGA, DGFT_JEGA, DGFT_JGGA, 
			DGFT_CVOL, DGFT_GAMT, DGFT_MGJY);
		break;
	}
}

void CDataFormat::MakeHeader(LPCTSTR lpszCode)
{
	CString code;

	if (lpszCode) 
		code = lpszCode;

	switch (m_pEnvInfo->datainfo.btUnit)
	{
	case GU_CODE:
		MakeHeader_code();
		break;
	case GU_INDEX:
		MakeHeader_index();
		break;
	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
	case GU_FFUTURE:	// 20090916: ADD
	case GU_CFUTURE:	// 20090916: ADD
		if (!lpszCode || code.GetLength() != 8)
		{
			break;
		}
		else
		{
			int	unit = WhichFOCode(code);
			switch (unit)
			{
			case foOption:
				m_pEnvInfo->datainfo.btUnit = GU_OPTION;
				MakeHeader_option();	break;
			case foCoFut:
				m_pEnvInfo->datainfo.btUnit = GU_COFUT;
				MakeHeader_cofuture();	break;
			case foCoOpt:
				m_pEnvInfo->datainfo.btUnit = GU_COOPT;
				MakeHeader_cooption();	break;
			case foCmFut: // 20090916 : ADD
				m_pEnvInfo->datainfo.btUnit = GU_CFUTURE;
				MakeHeader_commoditiesfuture(); break;
			case foFmFut: // 20090917 : ADD
				m_pEnvInfo->datainfo.btUnit = GU_FFUTURE;
				MakeHeader_commoditiesfuture(); break;
			case foFuture:
			default:
				m_pEnvInfo->datainfo.btUnit = GU_FUTURE;
				MakeHeader_future();	break;
			}
			m_pObjMgr->m_pwndPnChart->SendMessage(GEV_SISE, siseUnit, m_pEnvInfo->datainfo.btUnit);
		}
		break;
	case GU_FOREIGN:
		MakeHeader_foreign();
		break;
	}
}

void AddSymbol(CString& s, LPCTSTR lpszSymbol)
{
	CString symbol(lpszSymbol);
	//symbol = symbol.Right(3);
	//symbol.Insert("10", 3);
	symbol.Format("%s\n", lpszSymbol);
	s += symbol;
}


#define IF_SYMBOL(symbolConstant, requestSymbol) if (m_iOutID2 & symbolConstant) AddSymbol(m_strOutSymbol2, requestSymbol);

void CDataFormat::MakeHeader2()
{
	m_strOutSymbol2.Empty();
	switch (m_pEnvInfo->datainfo.btUnit)
	{
	case GU_CODE:
		switch(m_pEnvInfo->datainfo.btIndex)
		{
		case GI_DAY:
			IF_SYMBOL(FRGNHAVE, INV_FRGNHAVE);
			IF_SYMBOL(FRGNRATE, INV_FRGNRATE);
			IF_SYMBOL(PSNLPBUY, INV_PSNLPBUY);
			IF_SYMBOL(FRGNPBUY, INV_FRGNPBUY);
			IF_SYMBOL(ORGNPBUY, INV_ORGNPBUY);
			IF_SYMBOL(PSNLTBUY, INV_PSNLTBUY);
			IF_SYMBOL(FRGNTBUY, INV_FRGNTBUY);
			IF_SYMBOL(ORGNTBUY, INV_ORGNTBUY);
			m_strOutSymbol2 += "\t";
		}
		break;
	case GU_INDEX:
		switch (m_pEnvInfo->datainfo.btIndex)
		{
		case GI_DAY:
			IF_SYMBOL(PSNLPBUY, IINV_PSNLPBUY);
			IF_SYMBOL(FRGNPBUY, IINV_FRGNPBUY);
			IF_SYMBOL(ORGNPBUY, IINV_ORGNPBUY);
			IF_SYMBOL(PSNLTBUY, IINV_PSNLTBUY);
			IF_SYMBOL(FRGNTBUY, IINV_FRGNTBUY);
			IF_SYMBOL(ORGNTBUY, IINV_ORGNTBUY);
			m_strOutSymbol2 += "\t";
			break;
		}
		break;
	}
}

void CDataFormat::MakeHeaderS()
{
	m_strOutSymbolS.Empty();
	switch (m_pEnvInfo->datainfo.btUnit)
	{
	case GU_CODE:
		switch(m_pEnvInfo->datainfo.btIndex)
		{
		case GI_DAY:
			switch (m_iOutIDS)
			{
			case FRGNHAVE:	AddSymbol(m_strOutSymbolS, INV_FRGNHAVE); break;
			case FRGNRATE:	AddSymbol(m_strOutSymbolS, INV_FRGNRATE); break;
			case PSNLPBUY:	AddSymbol(m_strOutSymbolS, INV_PSNLPBUY); break;
			case FRGNPBUY:	AddSymbol(m_strOutSymbolS, INV_FRGNPBUY); break;
			case ORGNPBUY:	AddSymbol(m_strOutSymbolS, INV_ORGNPBUY); break;
			case PSNLTBUY:	AddSymbol(m_strOutSymbolS, INV_PSNLTBUY); break;
			case FRGNTBUY:	AddSymbol(m_strOutSymbolS, INV_FRGNTBUY); break;
			case ORGNTBUY:	AddSymbol(m_strOutSymbolS, INV_ORGNTBUY); break;
			}
			m_strOutSymbolS += "\t";
			break;
		}
		break;
	case GU_INDEX:
		switch (m_pEnvInfo->datainfo.btIndex)
		{
		case GI_DAY:
			switch (m_iOutIDS)
			{
			case PSNLPBUY:	AddSymbol(m_strOutSymbolS, IINV_PSNLPBUY); break;
			case FRGNPBUY:	AddSymbol(m_strOutSymbolS, IINV_FRGNPBUY); break;
			case ORGNPBUY:	AddSymbol(m_strOutSymbolS, IINV_ORGNPBUY); break;
			case PSNLTBUY:	AddSymbol(m_strOutSymbolS, IINV_PSNLTBUY); break;
			case FRGNTBUY:	AddSymbol(m_strOutSymbolS, IINV_FRGNTBUY); break;
			case ORGNTBUY:	AddSymbol(m_strOutSymbolS, IINV_ORGNTBUY); break;
			}
			m_strOutSymbolS += "\t";
			break;
		}
		break;
	}
}

void CDataFormat::CheckHeader2()
{
	m_iOutID2 = 0;
	switch (m_pEnvInfo->datainfo.wMainGrpKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_POLE:
	case GK_BALANCE:
	case GK_AVOL:
		break;
	default:
		return;
	}

	int	ii;
	struct _graph	*graph;
	switch (m_pEnvInfo->datainfo.btUnit)
	{
	case GU_CODE:
		switch(m_pEnvInfo->datainfo.btIndex)
		{
		case GI_DAY:
			for (ii = 0; ii < int(m_dwGrpCnt); ii++)
			{
				graph = (_graph *)&m_pcGrp[m_dwRgnCnt * SZ_REGION + SZ_GRAPH * ii];
				switch (graph->wGKind)
				{
				case GK_FRGNHAVE: m_iOutID2 |= FRGNHAVE; break;
				case GK_FRGNRATE: m_iOutID2 |= FRGNRATE; break;
				case GK_PSNLPBUY: m_iOutID2 |= PSNLPBUY; break;
				case GK_FRGNPBUY: m_iOutID2 |= FRGNPBUY; break;
				case GK_ORGNPBUY: m_iOutID2 |= ORGNPBUY; break;
				case GK_PSNLTBUY: m_iOutID2 |= PSNLTBUY; break;
				case GK_FRGNTBUY: m_iOutID2 |= FRGNTBUY; break;
				case GK_ORGNTBUY: m_iOutID2 |= ORGNTBUY; break;
				}
			}
			break;
		case GI_WEEK:
		case GI_MONTH:
			break;
		case GI_MINUTE:
		case GI_TICK:
			break;
		}
		break;
	case GU_INDEX:
		switch(m_pEnvInfo->datainfo.btIndex)
		{
		case GI_DAY:
			for (ii = 0; ii < int(m_dwGrpCnt); ii++)
			{
				graph = (_graph *)&m_pcGrp[m_dwRgnCnt * SZ_REGION + SZ_GRAPH * ii];
				switch (graph->wGKind)
				{
				case GK_PSNLPBUY: m_iOutID2 |= PSNLPBUY; break;
				case GK_FRGNPBUY: m_iOutID2 |= FRGNPBUY; break;
				case GK_ORGNPBUY: m_iOutID2 |= ORGNPBUY; break;
				case GK_PSNLTBUY: m_iOutID2 |= PSNLTBUY; break;
				case GK_FRGNTBUY: m_iOutID2 |= FRGNTBUY; break;
				case GK_ORGNTBUY: m_iOutID2 |= ORGNTBUY; break;
				}
			}
			break;
		}
		break;
	case GU_FUTURE:
	case GU_OPTION:	
	case GU_CFUTURE: // 20090917 : ADD
	case GU_FFUTURE: // 20090917 : ADD
	case GU_FOREIGN:
	case GU_COFUT:
	case GU_COOPT:
		break;
	}
}

int CDataFormat::CheckHeaderS(int wGKind)
{
	m_iOutIDS = 0;
	switch (m_pEnvInfo->datainfo.wMainGrpKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_POLE:
	case GK_BALANCE:
	case GK_AVOL:
		break;
	default:
		return 0;
	}

	switch (m_pEnvInfo->datainfo.btUnit)
	{
	case GU_CODE:
	case GU_INDEX:
		switch(m_pEnvInfo->datainfo.btIndex)
		{
		case GI_DAY:
			m_iOutIDS = isExtraTR(wGKind);
			break;
		case GI_WEEK:
		case GI_MONTH:
			break;
		case GI_MINUTE:
		case GI_TICK:
			break;
		}
		break;
	case GU_FUTURE:
	case GU_OPTION:
	case GU_CFUTURE: // 20090917 : ADD
	case GU_FFUTURE: // 20090917 : ADD
	case GU_FOREIGN:
	case GU_COFUT:
	case GU_COOPT:
		break;
	}

	return m_iOutIDS;
}


#define IS_VALIDTR(chartKind, symbol) case chartKind: if (m_iOutID2 & symbol) return 0;\
									m_iOutID2 |= symbol; return symbol;

int CDataFormat::isExtraTR(int wGKind)
{
	switch (wGKind)
	{
		IS_VALIDTR(GK_FRGNHAVE, FRGNHAVE);
		IS_VALIDTR(GK_FRGNRATE, FRGNRATE);
		IS_VALIDTR(GK_PSNLPBUY, PSNLPBUY);
		IS_VALIDTR(GK_FRGNPBUY, FRGNPBUY);
		IS_VALIDTR(GK_ORGNPBUY, ORGNPBUY);
		IS_VALIDTR(GK_PSNLTBUY, PSNLTBUY);
		IS_VALIDTR(GK_FRGNTBUY, FRGNTBUY);
		IS_VALIDTR(GK_ORGNTBUY, ORGNTBUY);
	}

	return 0;
}

#include "../../h/axisfire.h"
#include "../../h/axisvar.h"

void CDataFormat::ReadEnvPath()
{
	CString	root = (char *)(m_pObjMgr->m_pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0));
	CString name = (char *)(m_pObjMgr->m_pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), 0));
	m_strEnvPath.Format(_T("%s\\%s\\%s\\ensetup.ini"), root.GetString(), USER, name.GetString());
}

int CDataFormat::ReadVolumeOption()
{
	char	buf[16];
	buf[0] = 0x00;
	int res = GetPrivateProfileString("GRAPH", "volume_skip", "1", buf, sizeof(buf), m_strEnvPath);
	if (res > 0 && atoi(buf))
		return GUI_ZEROSKIP;

	return 0;
}

int CDataFormat::WhichFOCode(LPCTSTR code)
{
	CString cd(code);
	if (cd.GetLength() != 8) return foNone;

	CString strGubn{};
	strGubn = cd.Mid(1, 2);
	if (cd != K200_INDEX && cd != K200_FUTURE && cd != STAR_INDEX && cd != STAR_FUTURE)
	{
		if ((cd[0] == '2' || cd[0] == '3' || cd[0] == 'B' || cd[0] == 'C'))  //파생상품 코드개편
		{
			return foOption;
		}
		else if (cd[0] == '1' || cd[0] == 'A')	// 1xxxxxxx //파생상품 코드개편
		{
			if ((cd[1] >= '1') && (cd[1] < '6'))	// 11xxxxxx ~ 15xxxxxx : 주식 선물 옵션
				return foCoFut;
			else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
				|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
				|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z"))
			{
				return foCoFut;
			}
			else if ((cd[1] >= '6') && (cd[1] < '8'))
				return foCmFut;						// 16xxxxxx ~ 18xxxxxx : 상품 선물 // 20090916 : ADD
			else if (cd[1] >= '8')
				return foFmFut;						// 18xxxxxx : 돈육, 금 선물 
			else if (cd[2] == '3')
				return foStar;
			
			return foFuture;
		}
		else	// "4xxxxxxx"	spread
		{
			if (cd[1] < '6')
				return foFuture;
			else
				return foCmFut;						// 46xxxxxx ~ 48xxxxxx : 상품 선물 스프레드 // 20090916 : ADD
		}
	}

	return foFuture;
}
