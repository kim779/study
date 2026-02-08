#include "stdafx.h"
#include "axisGraph.h"
#include "grpObject.h"
#include "dataFormat.h"
#include "graphMgr.h"

#include "GrpGrid.h"
#include "RgnSplit.h"
#include "CrossLine.h"

#include "Tendency.h"
#include "FiboLine.h"
#include "FiboFan.h"
#include "FiboArc.h"
#include "FiboRet.h"
#include "GannLine.h"
#include "GannFan.h"
#include "AdCross.h"
#include "AccelFan.h"
#include "AccelArc.h"
#include "UsrInput.h"
#include "GtypeBong.h"
#include "GtypeLine.h"
#include "GtypeUser.h"
#include "GtypeVars.h"
#include "TickLine.h"
#include "volume.h"
#include "envboll.h"
#include "parabol.h"
#include "Balance.h"
#include "amt.h"
#include "adline.h"
#include "Cci.h"
#include "Disparity.h"
#include "dmi.h"
#include "LFI.h"
#include "Macd.h"
#include "mao.h"
#include "Nco.h"
#include "NewPsy.h"
#include "OBV.h"
#include "PV.h"
#include "RClock.h"
#include "rrate.h"
#include "RSI.h"
#include "Sigma.h"
#include "Psy.h"
#include "Sona.h"
#include "Sona2.h"
#include "Stochastic.h"
#include "Trix.h"
#include "VR.h"
#include "WilliamPR.h"
#include "Pivot.h"
#include "Demark.h"
#include "Roc.h"
#include "Momentum.h"
#include "MarketProfile.h"
#include "radar.h"
#include "PnF.h"
#include "SamSun.h"
#include "Swing.h"
#include "Net.h"

#include "UserLine.h"
#include "UserLine2.h"
#include "UserStep.h"
#include "UserBar.h"
#include "UserVBar.h"
#include "UserVBar2.h"
#include "FullPie.h"
#include "SplitPie.h"
#include "ZBaseXBar.h"
#include "ZBaseXVBar.h"
#include "weeksVratio.h"

#include "grpData.h"

#include "toolBase.h"

#include "HorzLine.h"
#include "VertLine.h"
#include "ArrowTool.h"
#include "DiagramRect.h"

#include "ndistribution.h"

#include <math.h>
#include <fstream.h>
#include "../grc/libgrc.h"
#include "../../h/common.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum	_mthDRAW { mthNONE = 0, mthLOCAL = 1, mthGLOBAL = 2, mthSHIFT = 3 };
CRect CGrpObject::m_saveRIGHT;
CRect CGrpObject::m_saveLEFT;

const int chartBASE = 3000;
const int chartCOMMON = 3100;
const int chartFUTURE = 3200;

const int toolMENUID  = 4000;
const int modifyMENUID= 4100;

DWORD	DUPCOLOR[MAXDUP*2] = { _CHART2, _CHART3, _CHART4, _CHART5, _CHART6, _CHART7, _CHART8, _CHART9 };


CGrpObject::CGrpObject(CGraphMgr* mgr, char* info)
{
	m_pGrpApp = (CAxisGraphApp *)AfxGetApp();

	ASSERT(mgr);
	m_pMgr = mgr;
	m_pGraph = (CWnd *)m_pMgr->m_pGraph;

	struct	_graphi*	gInfo;
	gInfo = (struct _graphi *) info;
	struct _fgraph*	pfgraph = (struct _fgraph *)gInfo->file;

	m_ObjName = CString((char *)gInfo->objects.operator LPCTSTR(), 
		gInfo->objects.GetLength());

	m_mapRECT = gInfo->rect;
	m_mapRECT.OffsetRect(-1 * (gInfo ->rect.left), -1 * (gInfo->rect.top));
	if (m_pMgr->m_Status & GST_CTRL)
		m_mapRECT.bottom = m_mapRECT.bottom - heightCtrl;

	if (m_pMgr->m_Status & GST_TOTAL)
	{
		m_mapRECT.bottom = m_mapRECT.bottom - heightInput;
		m_mapRECT.bottom = m_mapRECT.bottom - heightSise;
	}

	m_ObjRect = m_mapRECT;
	m_GrpRect = m_mapRECT;
	m_ObjRegion.SetRectEmpty();
	m_GrpRegion.SetRectEmpty();

	m_dayDisp = dispDAY;
	if (gInfo->days > 0)
		m_dayDisp = gInfo->days;

	m_fName = CString((char *)gInfo->fName.operator LPCTSTR(), 
		gInfo->fName.GetLength());

	m_fWidth = 0;
	m_fHeight = float(gInfo->fPoint);
	m_fPoint = float(gInfo->fPoint);
	m_fStyle = gInfo->fStyle;

	m_transparent = false;
	m_ObjUnit  = 0;
	m_ObjIndex = 0;

	for (int ii = 0; ii < magicMAX; ii++)
	{
		m_magicVAL[ii][chkMAGIC] = 0;		// validity
		m_magicVAL[ii][maxMAGIC] = 0;		// max
		m_magicVAL[ii][minMAGIC] = 0;		// min
	}

	for (ii = 0; ii < nTICK; ii++)
	{
		m_xTick[ii].xRect.SetRectEmpty();
		m_xTick[ii].sRect.SetRectEmpty();
		m_xTick[ii].xScale = 0;
		m_xTick[ii].xColor = 0;
	}
	
	m_bToolChange = false;
	m_bJTrace = false;
	m_bNMLine = true;
	m_bNoSplit = false;
	m_bRefresh = false;
	m_bNoMargin = false;
	m_bEditable = false;
	m_bDataVisible = true;
	m_bCrossVisable = true;
	m_bUsableGrid = false;
	m_bUpdateREAL = false;
	m_bReverseMode = false;
	m_bEveryUpdate = false;
	m_bForeign = FOR_NO;
	m_bECNData = false;
	m_bAutoTran = false;
	m_bSolidAidLine = true;
	m_bGradient = true;
	m_bUpFill = true;
	m_bDnFill = true;
	m_UpColor = GetPaletteRGB(_UPCOLOR);
	m_DnColor = GetPaletteRGB(_DNCOLOR);
	m_hRatio = m_vRatio = 1.0;
	m_pCrossLine = NULL;
	m_bAppendMode = true;
	m_mouseOWNER = pointNONE;
	m_tkSELECT  = -1;
	m_tkSAVE = -1;
	m_tkCHOICED = tkNONE;
	m_ObjStatus = 0;
	m_DevOption = 0;
	m_inputDisp = 0;
	m_BongDataKey = -1;

	m_DataFormat = new CDataFormat(m_ObjName, this);
	if (!m_DataFormat->SetFgraph(m_pGrpApp->GetuserID(), pfgraph->name))
		m_DataFormat->SetFgraph(gInfo->file);
	else
		pfgraph = m_DataFormat->GetGrpFileInfo();

	m_tRGB = GetPaletteRGB(pfgraph->fgcolor);
	m_pRGB = GetPaletteRGB(pfgraph->bgcolor);
	m_aRGB = GetPaletteRGB(pfgraph->adcolor);
      
	m_ObjEnv = 0x0000;
	m_ObjLine = 0x0000;
	m_pSelLineGrp = NULL;
	m_pGrid = new CGrpGrid();
	m_pGrid->Create((CWnd *)m_pMgr->m_pGraph, this);

	m_ToolQue.RemoveAll();
	m_ToolQueEx.RemoveAll();
	m_SSizePt = CPoint(-1, -1);

	m_bLineSelect = false;
	m_rGap = 0;
	m_lGap = 0;
	m_bTickChart = false;
	m_bViewCurr = false;
	if (m_pMgr->m_bTotal)
	{
		m_lMargin = initLMargin;
		m_rMargin = initRMargin;
	}
	else
	{
		m_lMargin = initLMargin;
		m_rMargin = initRMargin*2;
	}

	ZeroMemory(m_pVal, sizeof(m_pVal));
	m_bStack = false;
	m_bDrawTool = false;
	m_bFirst = true;
	m_mgjySave = _T("");

	m_bMouseCaptureTCreate = FALSE;
}

CGrpObject::~CGrpObject()
{
	for (int ii = 0; ii < maxAVLINE; ii++)
	{
		if (m_pVal[ii])
		{
			delete[] m_pVal[ii];
			m_pVal[ii] = NULL;
		}
	}
	
	if (m_DataFormat)
	{
		delete m_DataFormat;
		m_DataFormat = NULL;
	}
	if (m_pGrid)
	{
		m_pGrid->DestroyWindow();
		delete m_pGrid;
		m_pGrid = NULL;
	}
	RemoveGrpComponent();
}

void CGrpObject::RemoveGrpComponent(bool bClearAll)
{
	m_cs.Lock();

	RemoveToolQue();
	RemoveGraphQue();
	if (bClearAll)
		RemoveGraphData();
	RemoveGraphRegion();
	removeDisplayStack();
	removeDisplayStack();
	if (ResetCursorModify())
	{
		HCURSOR hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		::SetCursor(hCursor);
	}
	if (m_pCrossLine != NULL)
	{
		delete m_pCrossLine;
		m_pCrossLine = NULL;
	}
	m_tkSELECT  = -1;
	m_tkSAVE    = -1;
	m_tkCHOICED = tkNONE;
	m_ObjStatus = 0;
	m_cs.Unlock();
}

char* CGrpObject::GetGrpHeader(int& hLen)
{
	return m_DataFormat->GetNewGrpHeader(hLen); 
}

void CGrpObject::ObjectClear()
{
	RemoveGrpComponent();
	ReDrawGraphObject();
}

bool CGrpObject::DispatchGraphData(char* gData, int& gDataL)	// struct _dgraphH
{
	m_bUpdateREAL = false;
	m_bNoSplit = false;		
	m_bNoMargin = false;
	m_bEditable = false;
	m_bUsableGrid = false;
	m_bEveryUpdate = false;
	m_bViewCurr = false;
	m_mgjySave = _T("");

	m_SSizePt = CPoint(-1, -1);
	if (gData == NULL || gData == "")
	{
		RemoveGrpComponent();
		ReDrawGraphObject();
		return false;
	}

	int	UpLen;
	
	char	*pUpGraph = m_DataFormat->GetGrpHeader(&UpLen);
	if (m_ObjEnv & ENV_DUP)
	{
		struct _fgraph *tfgraph = m_DataFormat->GetGrpFileInfo();
		struct _graph tgraph;
		bool	bDupEnable = true;
		for (int kk = 0; kk < maxGRAPH; kk++)
		{
			tgraph = tfgraph->graph[kk];
			switch (tgraph.type)
			{
			case GT_LINE:
				if (tgraph.kind == GK_PV || tgraph.kind == GK_CLOCK)
					bDupEnable = false;
				break;
			case GT_VARS:
			case GT_USER:
				bDupEnable = false;
				break;
			default:
				break;
			}
		}
		m_DevOption |= DF_NRTM;
		if (bDupEnable)
			return DispatchDup(gData, gDataL);
	}
	if (m_pMgr->m_grxType == DEF_SALE)
	{
		m_ObjEnv &= ~(ENV_BONG | ENV_USA | ENV_LOG);
		m_ObjEnv |= ENV_LINE;
	}
	else if (m_pMgr->m_grxType == DEF_EQUIVOL)
	{
		int index = m_pMgr->m_pView->SendMessage(GEV_INPUT, 
				MAKEWPARAM(inputIndex, 1), 0);
		if (index == GI_TICK)
		{
			m_ObjEnv &= ~(ENV_BONG | ENV_USA | ENV_LOG);
			m_ObjEnv |= ENV_LINE;
		}
		else
		{
			m_ObjEnv &= ~(ENV_LINE | ENV_USA | ENV_LOG);
			m_ObjEnv |= ENV_BONG;
		}
		
	}

	RemoveGrpComponent();

	struct _ugraphH *pUgraphH = (struct _ugraphH *)pUpGraph;
	struct _dgraphH *pDgraphH = (struct _dgraphH *)gData;
	struct _fgraph *pFgraph = m_DataFormat->GetGrpFileInfo();

	m_ObjUnit  = pDgraphH->unit;
	m_ObjIndex = pDgraphH->index;

	if (pFgraph->date > 10000)
		pFgraph->date = 300;
	m_inputDisp = pFgraph->date;
	
	m_ObjRegion.SetRectEmpty();
	m_GrpRegion.SetRectEmpty();
	m_GrpRect = m_ObjRect = m_mapRECT;
	
	m_tRGB = GetPaletteRGB(pFgraph->fgcolor);
	m_pRGB = GetPaletteRGB(pFgraph->bgcolor);
	m_aRGB = GetPaletteRGB(pFgraph->adcolor);


	if (m_pMgr->m_bTotal)
	{
		if (m_bFirst)
			m_bFirst = false;
		else
			SaveMouseOption();
	}

	
	if (pFgraph->update & GF_RTM)
		m_bUpdateREAL = true;
	if (pFgraph->update & GF_NSP)
		m_bNoSplit = true;		
	if (pFgraph->update & GF_NMG)
		m_bNoMargin = true;
	if (pFgraph->update & GF_EDT)
		m_bEditable = true;
	if (pFgraph->update & GF_GRID)
		m_bUsableGrid = true;
	if (pFgraph->update & GF_TICK)	
		m_bEveryUpdate = true;
	if (pFgraph->update & GF_HMS && pFgraph->gap == 0)
		m_bEveryUpdate = true;

	switch (m_pMgr->m_grxType)
	{
	case DEF_CLOCK:
	case DEF_PV:
	case DEF_SALE:
		m_bUpdateREAL = false;
		break;
	}

	if (m_inputDisp > 0)
		m_dayDisp = m_inputDisp;
	if (pFgraph->update & GF_COM)
	{
		m_dayDisp = compressDAY;
	}

	AddGraphTick((char *)&pFgraph->xtick, xBOTTOM);
	AddGraphTick((char *)&pFgraph->ltick, xLEFT);
	AddGraphTick((char *)&pFgraph->rtick, xRIGHT);
	AddGraphRegion((char *)pFgraph->region);

	// 개발자 동적 설정 적용
	m_DevOption = pDgraphH->update1;
	if (m_DevOption & DF_COM)
		m_dayDisp = compressDAY;
	if (pDgraphH->update1 & DF_FPOS)
		m_bForeign = FOR_POS;
	else if (pDgraphH->update1 & DF_FTRD)
		m_bForeign = FOR_TRD;
	if (pDgraphH->update2 & DF_ECNDATA)
		m_bECNData = true;
	else
		m_bECNData = false;
	if (pDgraphH->update2 & DF_AUTOTRAN)
		m_bAutoTran = true;
	else
		m_bAutoTran = false;
	if (pDgraphH->tkgap > 0 && pDgraphH->tkgap%10 == 0)
		m_tkGap = pDgraphH->tkgap;
	else
		m_tkGap = 10;

	if (!AddGraphData(pDgraphH->index, pDgraphH->ndat, &gData[sz_DGRAPHH], gDataL))
	{
		TRACE("Invalid Graph DataH...Cnt[%d]\n", pDgraphH->ndat);
		SetGraphError(AGENODATA);
		RemoveGraphRegion();	// after check 
		ResizeGraphObject(m_hRatio, m_vRatio, true);
		return true;
	}

	if (m_bTickChart)
		m_bEditable = false;

	if (AddGraphQue((char *)pFgraph->graph, false, true) == (CGrpBase* ) 0)
	{
		TRACE("Invalid graph Information..\n");
		SetGraphError(AGENOGRAPH);
		RemoveGraphData();	// after check 
		RemoveGraphRegion();	// after check 
		ResizeGraphObject(m_hRatio, m_vRatio, true);
		return true;
	}

	RecalculateMagic();
	if (m_bUsableGrid)
	{
		CGrpBase* pGrpBase = ExistBongTypeGraph();
		if (pGrpBase == (CGrpBase *) 0)
			pGrpBase = m_GraphQue.GetAt(0);

		m_pCrossLine = new CCrossLine(this, pGrpBase, tkCROSSLINE);
		m_ObjStatus |= GOS_CROSSLINE;
	}

	if (pFgraph->operation & GO_OVRLAP)
		m_bAppendMode = false;
	if (pFgraph->operation & GO_REVGRP)
		m_bReverseMode = true;
	if (pFgraph->operation & GO_NOGVIEW)
	{
		if (m_pCrossLine != NULL)
			m_pCrossLine->SetDataVisable(m_bDataVisible);
		m_bDataVisable = false;
	}

	if (pFgraph->operation & GO_CHIDE)
	{
		if (m_pCrossLine != NULL)
			m_pCrossLine->SetVisable();
		m_bCrossVisable = false;
	}
	if (pFgraph->operation & GO_JTRACE)
		m_bJTrace = true;

	m_bNMLine = true;
	if (pFgraph->operation & GO_NM_LINE)	
		m_bNMLine = true;
	else if (pFgraph->operation & GO_ST_LINE)
		m_bNMLine = false;
	
	SetBongOption(pFgraph->upcolor, pFgraph->dncolor, pFgraph->operation);

	SetGrpEnv();

	if (pDgraphH->index == GI_HMS)
		GetDataGap();

	if (m_pMgr->m_bTotal)
		setDupGraph();

	struct _addGrp addGrp;
	m_pMgr->AddFileInfo(2000, (char *)&addGrp, sz_ADDGRP, (char *)pFgraph, sz_FGRAPH);
	if (addGrp.vcurr == 1)
		m_bViewCurr = true;
	else
		m_bViewCurr = false;
	m_lMargin = addGrp.lmargin;
	m_rMargin = addGrp.rmargin;

	if (m_pMgr->m_bTotal)
	{
		char* buf = (char *)m_pMgr->m_pView->SendMessage(GEV_SISE, 
			MAKEWPARAM(siseSetData, 1), 0);
		CString currS = CString(buf);
		CGrpData	*pGrpData = NULL;
		CGrpBase	*pGrpBase = NULL;
		for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
		{
			pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
			if (!pGrpBase)
				continue;
			int dKey = pGrpBase->GetDataKey();
			if (!GetGrpData(dKey, pGrpData))
				continue;
			if (pGrpData->m_curr != atof(currS))
				pGrpData->m_curr = atof(currS);
		}			
	}
	ResizeGraphObject(m_hRatio, m_vRatio, true);
	return true;
}

void CGrpObject::RecalculateMagic()
{
	CGrpBase* pTemp;
	int	dKey, MagicMin, MagicMax;
	for (int idxMagic = 0; idxMagic < magicMAX; idxMagic++)
	{
		if (!m_magicVAL[idxMagic][chkMAGIC])
			continue;

		MagicMin = magicBAS + idxMagic * 10;
		MagicMax = MagicMin + 9;

		int	nMax = 0;
		bool	bUserLine = true;
		for (int jj = m_GraphQue.GetUpperBound(); jj >= 0; jj--)
		{
			pTemp = m_GraphQue.GetAt(jj);
			dKey = pTemp->GetDataKey();

			if (dKey >= MagicMin && dKey <= MagicMax)
			{
				if (nMax < pTemp->m_MaxMultiple)
					nMax = pTemp->m_MaxMultiple;
				switch (pTemp->GetGraphKind())
				{
				case GK_LINE_VAL:
				case GK_LINE_VIDX:
					break;
				default:
					bUserLine = false;
					break;
				}
			}
		}

		int	gap = int (m_magicVAL[idxMagic][maxMAGIC] - m_magicVAL[idxMagic][minMAGIC]) / 2;
		if (nMax > 1)
		{
			m_magicVAL[idxMagic][maxMAGIC] += gap * (nMax - 1);
			m_magicVAL[idxMagic][minMAGIC] -= gap * (nMax - 1);
		}
	}
}

void CGrpObject::RecalculateRTMMagic(CGrpBase *pGrpBase)
{
	CGrpData*	pGrpData;
	if (!pGrpBase->GetDataInstance(pGrpData))
		return;

	int	mapKey = pGrpBase->GetDataKey();
	if (mapKey < magicBAS)
		return;

	int	idxMagic = (mapKey % 100) / 10;
	if (idxMagic < 0 || idxMagic > magicMAX)
	{
		TRACE("magickey error...[%d]\n", mapKey);
		return;
	}

	if (!pGrpData && pGrpData->GetArraySize() <= 0)
		return;

	int	MagicMin = magicBAS + idxMagic * 10;
	int	MagicMax = MagicMin + 9;
	int	nMax = 0;
	CGrpBase*	pTemp;
	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pTemp = m_GraphQue.GetAt(ii);
		int	dKey = pTemp->GetDataKey();

		if (dKey >= MagicMin && dKey <= MagicMax)
		{
			if (nMax < pTemp->m_MaxMultiple)
				nMax = pTemp->m_MaxMultiple;
		}
	}
	
	int	Multi = nMax - 1;
	double	max = m_magicVAL[idxMagic][maxMAGIC];
	double	min = m_magicVAL[idxMagic][minMAGIC];
	double	orgMAX = max;
	double	orgMIN = min;

	if (nMax > 1)
	{
		orgMAX = ( (max + min) + (max - min) / (Multi + 1) ) / 2;
		orgMIN = ( (max + min) - (max - min) / (Multi + 1) ) / 2;
	}

	bool	bChange = false;
	struct _gVidx*	gVidx;
	gVidx = (struct _gVidx *) pGrpData->m_DataQue.GetAt(pGrpData->GetArraySize() - 1);
	if (gVidx->val && gVidx->val > orgMAX)
	{
		bChange = true;
		orgMAX = gVidx->val;
	}
	if (gVidx->val && gVidx->val < orgMIN)
	{
		bChange = true;
		orgMIN = gVidx->val;
	}

	if (!bChange)
		return;

	int	gap = int (orgMAX - orgMIN) / 2;
	m_magicVAL[idxMagic][maxMAGIC] = orgMAX;
	m_magicVAL[idxMagic][minMAGIC] = orgMIN;
	if (nMax > 1)
	{
		m_magicVAL[idxMagic][maxMAGIC] += gap * (nMax - 1);
		m_magicVAL[idxMagic][minMAGIC] -= gap * (nMax - 1);
	}
}

void CGrpObject::SetGraphError(int no)
{
	
}

void CGrpObject::AddGraphRegion(char* data)
{
	int	mapKey;
	struct	_xRegion* xRegion;

	struct	_region* pRegion;
	pRegion = (struct _region *) data;

	int	gWidth  = m_GrpRect.Width();
	int	gHeight = m_GrpRect.Height();
	for (int ii = 0; ii < maxGRAPH; ii++)
	{
		if ((mapKey = pRegion[ii].key) == GV_UNUSED)
			break;
		if (m_MapRegion.Lookup(mapKey, xRegion))
			continue;

		xRegion = (struct _xRegion *)new char [sizeof (struct _xRegion)];

		xRegion->xRect.left = m_GrpRect.left + ((gWidth * pRegion[ii].left) / 100);
		xRegion->xRect.right = m_GrpRect.left + ((gWidth * pRegion[ii].right) / 100);
		xRegion->xRect.top = m_GrpRect.top + ((gHeight * pRegion[ii].top) / 100);
		xRegion->xRect.bottom = m_GrpRect.top + ((gHeight * pRegion[ii].bottom) / 100);

		xRegion->orgRgnKey = unusedKEY;
		xRegion->sRect = xRegion->xRect;
		m_MapRegion.SetAt(mapKey, xRegion);
	}
}

void CGrpObject::RemoveGraphRegion()
{
	struct _xRegion* xRegion;
	int	mapKey;
	
	for (POSITION pos = m_MapRegion.GetStartPosition(); pos; )
	{
		m_MapRegion.GetNextAssoc(pos, mapKey, xRegion);
		delete[] xRegion;
		xRegion = NULL;
	}
	m_MapRegion.RemoveAll();
}

bool CGrpObject::RemoveGraphRegion(int RgnKey)
{
	struct _xRegion* xRegion;

	if (m_MapRegion.Lookup(RgnKey, xRegion))
	{
		delete[] xRegion;
		xRegion = NULL;
		m_MapRegion.RemoveKey(RgnKey);
		return true;
	}
	return false;
}

bool CGrpObject::AddGraphData(int index, int ndat, char* gData, int& gDataL) // data header + body
{
	m_nTotalCnt = 0;
	m_bTickChart = false;
	int	mapKey;
	char	dWb[64];
	bool	bCreate = false;
	CGrpData* pGrpData;

	int	nDate, dummy;
	int	datPos = 0, structL = 0;
	struct	_dataH*	pData;
	int ii = 0;

	for (ii = 0; ii < maxAVLINE; ii++)
	{
		if (m_pVal[ii] != NULL)
		{
			delete[] m_pVal[ii];
			m_pVal[ii] = NULL;
		}
	}

	gDataL = sz_DGRAPHH;
	for (ii = 0; ii < ndat; ii++)
	{
		pData = (struct _dataH *) &gData[datPos];

		if (pData->dkind == DK_AMT && m_BongDataKey != -1)	
			mapKey = m_BongDataKey;
		else
			mapKey = pData->key;

		bCreate = false;
		if (!m_MapData.Lookup(mapKey, pGrpData))
		{
			pGrpData = new CGrpData(mapKey, index);
			bCreate = true;
		}
		ASSERT(pGrpData);

		CopyAndSpaceFilter(dWb, pData->name, sizeof(pData->name));
		pGrpData->SetName(dWb);

		CopyAndSpaceFilter(dWb, pData->date, sizeof(pData->date));
		nDate = atoi(dWb);
		CopyAndSpaceFilter(dWb, pData->dummy, sizeof(pData->dummy));
		dummy = atoi(dWb);
		
		if (dummy > 0)
			dummy= 0;
		if (dummy < 0)
		{
			nDate += dummy;
			dummy = 0;
		}


		datPos += sizeof(struct _dataH);
		structL = pGrpData->AttachGraphData((char *)pData, &gData[datPos], nDate, dummy);
		if (structL < 0)
		{
			TRACE("data kind or data length Error...\n");
			if (bCreate)
			{
				delete pGrpData;
				pGrpData = NULL;
				m_MapData.RemoveKey(mapKey);
			}
			break;
		}
		else
		{
			if (pData->dkind == DK_VAL2)	
			{
				struct _gVal2 *gVal2;
				int size = pGrpData->m_DataQue.GetSize();
				
				if (m_pVal[m_nTotalCnt] == NULL)
				{
					m_pVal[m_nTotalCnt] = new double[size];
					for (int kk = 0; kk < size; kk++)
						m_pVal[m_nTotalCnt][kk] = 0;
				}
				for (int kk = 0; kk < size; kk++)
				{
					gVal2 = (struct _gVal2 *)pGrpData->m_DataQue.GetAt(kk);
					m_pVal[m_nTotalCnt][kk] = gVal2->val1;
				}
				m_nTotalCnt++;
			}
		}
		if (pData->dkind == DK_BONG || pData->dkind == DK_TICK)	// save bong type data key 
			m_BongDataKey = pData->key;
		if (pData->dkind == DK_TICK)
			m_bTickChart = true; 

		datPos += (structL * nDate);
		if (bCreate)
			m_MapData.SetAt(mapKey, pGrpData);
		if (mapKey >= magicBAS)
		{
			if (pGrpData->GetDataPattern() == dpVAL)
				SetMagicValue(mapKey, pGrpData, dpVAL);
			else if(pGrpData->GetDataPattern() == dpVIDX)
				SetMagicValue(mapKey, pGrpData, dpVIDX);
		}
		gDataL += sz_DATAH + (structL * nDate);
	}

	if (ii <= 0)
		return false;

	if (m_bTickChart)
	{
		struct _gTick* gTick = 
			(struct _gTick *)pGrpData->m_DataQue.GetAt(pGrpData->m_DataQue.GetUpperBound());
		CString tmpS;
		tmpS.Format("%d", gTick->epr);
		m_mgjySave = tmpS;
	}
	return true;
}

void CGrpObject::RemoveGraphData()
{
	CGrpData* pGrpData;
	int	mapKey;
	
	for (POSITION pos = m_MapData.GetStartPosition(); pos; )
	{
		m_MapData.GetNextAssoc(pos, mapKey, pGrpData);
		delete pGrpData;
		pGrpData = NULL;
	}
	m_MapData.RemoveAll();

	for (int ii = 0; ii < magicMAX; ii++)
	{
		m_magicVAL[ii][chkMAGIC] = 0;		// validity
		m_magicVAL[ii][maxMAGIC] = 0;		// max
		m_magicVAL[ii][minMAGIC] = 0;		// min
	}
}

CGrpBase* CGrpObject::AddGraphQue(char* data, bool bAppend, bool bScrollRgn)
{
	bool	bCalculatedMAV = false;
	struct _graph*	pGraph;

	pGraph = (struct _graph *) data;
	CGrpBase* pGrpBase = NULL;
	for (int ii = 0; ii < (!bAppend ? maxGRAPH : 1); ii++)
	{
		if (pGraph[ii].type == GV_UNUSED
			|| pGraph[ii].kind == GV_UNUSED)
		{
			break;
		}

		int	dKey = pGraph[ii].key;
		if (pGraph[ii].kind == GK_AMT || pGraph[ii].kind == GK_LG_AMA)
			GetBongDataKey(dKey);

 		if (!IsValidGraphHeader(dKey, pGraph[ii].region))
		{
			TRACE("Graph key or region is invalid[%d][%d]\n", 
				dKey, pGraph[ii].region);
			continue;
		}

		switch (pGraph[ii].type)
		{
		case GT_BONG:
			switch (pGraph[ii].kind)
			{
			case GK_JPN: case GK_USA:
			case GK_LIN: case GK_LOG:
			case GK_LG_LIN:	case GK_LG_LOG:	case GK_LG_PMA:	case GK_LG_LMA:
			case GK_EQUI:
				{
					struct _fgraph	*pFgraph = m_DataFormat->GetGrpFileInfo();
					
					struct _addGrp addGrp;
					if (m_pMgr->m_bTotal)
						m_pMgr->AddFileInfo(2000, (char *)&addGrp, sz_ADDGRP);
					else
						m_pMgr->AddFileInfo(2000, (char *)&addGrp, sz_ADDGRP, (char *)pFgraph, sz_FGRAPH);
					pGrpBase = new CGtypeBong(this, (char *)&pGraph[ii], addGrp);
				}
				break;
			case GK_NET:
			case GK_AD_NET:
			case GK_LG_NET:
				pGrpBase = new CNet(this, (char *)&pGraph[ii]);
				break;
			case GK_ENVL:		case GK_BOLB:
			case GK_LG_ENVL:	case GK_LG_BOLB:
			case GK_AD_ENVL:	case GK_AD_BOLB:
				pGrpBase = new CEnvBoll(this, (char *)&pGraph[ii]);
				break;
			case GK_PARB:	case GK_LG_PARB:
			case GK_AD_PARB:
				pGrpBase = new CParabol(this, (char *)&pGraph[ii]);
				break;
			case GK_BALANCE:
				pGrpBase = new CBalance(this, (char *)&pGraph[ii]);
				break;
			case GK_VOL:	case GK_LG_VMA:
			case GK_EVOL:
				{
					struct _fgraph	*pFgraph = m_DataFormat->GetGrpFileInfo();
					struct _addGrp addGrp;
					if (m_pMgr->m_bTotal)
						m_pMgr->AddFileInfo(2000, (char *)&addGrp, sz_ADDGRP);
					else
						m_pMgr->AddFileInfo(2000, (char *)&addGrp, sz_ADDGRP, (char *)pFgraph, sz_FGRAPH);
					pGrpBase = new CVolume(this, (char *)&pGraph[ii], addGrp);
				}
				break;
			case GK_AMT:	case GK_LG_AMA:
				pGrpBase = new CAmt(this, (char *)&pGraph[ii]);
				break;
			case GK_TICK:
			case GK_STEP:
			case GK_MGJY:
				pGrpBase = new CTickLine(this, (char *)&pGraph[ii]);
				break;
			default:
				break;
			}
			break;
		case GT_LINE:
			switch (pGraph[ii].kind)
			{
			case GK_ADLINE:
				pGrpBase = new CADLine(this, (char *)&pGraph[ii]);
				break;
			case GK_ADX:
			case GK_DMI:
				pGrpBase = new CDMI(this, (char *)&pGraph[ii]);
				break;
			case GK_OBV:
				pGrpBase = new COBV(this, (char *)&pGraph[ii]);
				break;
			case GK_CCI:
				pGrpBase = new CCci(this, (char *)&pGraph[ii]);
				break;
			case GK_CLOCK:
				pGrpBase = new CRClock(this, (char *)&pGraph[ii]);
				break;
			case GK_GAP:
				pGrpBase = new CDisparity(this, (char *)&pGraph[ii]);
				break;
			case GK_LFI:
				pGrpBase = new CLFI(this, (char *)&pGraph[ii]);
				break;
			case GK_MACD:
			case GK_MACDOS:
				pGrpBase = new CMacd(this, (char *)&pGraph[ii]);
				break;
			case GK_NCO:
				pGrpBase = new CNco(this, (char *)&pGraph[ii]);
				break;
			case GK_NPSY:
				pGrpBase = new CNewPsy(this, (char *)&pGraph[ii]);
				break;
			case GK_PMAO:
			case GK_VMAO:
				pGrpBase = new CMao(this, (char *)&pGraph[ii]);
				break;
			case GK_PSY:
				pGrpBase = new CPsy(this, (char *)&pGraph[ii]);
				break;
			case GK_PV:
				pGrpBase = new CPV(this, (char *)&pGraph[ii]);
				break;
			case GK_RRATE:
				pGrpBase = new CRRate(this, (char *)&pGraph[ii]);
				break;
			case GK_RSI:
				pGrpBase = new CRSI(this, (char *)&pGraph[ii]);
				break;
			case GK_SIGMA:
				pGrpBase = new CSigma(this, (char *)&pGraph[ii]);
				break;
			case GK_SONA:
				pGrpBase = new CSona(this, (char *)&pGraph[ii]);
				break;
			case GK_SONA2:
			case GK_SONA2OS:
				pGrpBase = new CSona2(this, (char *)&pGraph[ii]);
				break;
			case GK_STOCH:	// slow
			case GK_STOCH1:	// 동원
			case GK_STOCH2: // fast
				pGrpBase = new CStochastic(this, (char *)&pGraph[ii]);
				break;
			case GK_VR:
				pGrpBase = new CVR(this, (char *)&pGraph[ii]);
				break;
			case GK_WPR:
				pGrpBase = new CWilliamPR(this, (char *)&pGraph[ii]);
				break;
			case GK_PIVOT:
				pGrpBase = new CPivot(this, (char *)&pGraph[ii]);
				break;
			case GK_DEMARK:
				pGrpBase = new CDemark(this, (char *)&pGraph[ii]);
				break;
			case GK_MOMENTUM:
				pGrpBase = new CMomentum(this, (char *)&pGraph[ii]);
				break;
			case GK_ROC:
				pGrpBase = new CROC(this, (char *)&pGraph[ii]);
				break;
			case GK_TRIX:
				pGrpBase = new CTrix(this, (char *)&pGraph[ii]);
				break;
			default:
				break;
			}
			break;
		case GT_VARS:
			switch (pGraph[ii].kind)
			{
			case GK_MKPRO:
				pGrpBase = new CMarketProfile(this, (char *)&pGraph[ii]);
				break;
			case GK_PNF:
				pGrpBase = new CPnF(this, (char *)&pGraph[ii]);
				break;
			case GK_RADAR_VAL:
			case GK_RADAR_NMVAL:
				pGrpBase = new CRadar(this, (char *)&pGraph[ii]);
				break;
			case GK_RVOL:
				pGrpBase = new CWeeksVRatio(this, (char *)&pGraph[ii]);
				break;
			case GK_SWING:
				pGrpBase = new CSwing(this, (char *)&pGraph[ii]);
				break;
			case GK_TURN:
				pGrpBase = new CSamSun(this, (char *)&pGraph[ii]);
				break;
			default:
				break;
			}
			break;
		case GT_USER:
			switch (pGraph[ii].kind)
			{
			case GK_LINE_VAL:
			case GK_LINE_VIDX:
				pGrpBase = new CUserLine(this, (char *)&pGraph[ii]);
				break;
			case GK_2DLINE:
				pGrpBase = new CUserLine2(this, (char *)&pGraph[ii]);
				break;
			case GK_STEP_VAL:
			case GK_STEP_VIDX:
			case GK_STEP_RATIO:
				pGrpBase = new CUserStep(this, (char *)&pGraph[ii]);
				break;
			case GK_BARV1_VAL:
			case GK_BARV1_NMVAL:
			case GK_BARV2_BTK:
			case GK_BARV2_NBTK:
				pGrpBase = new CUserBar(this, (char *)&pGraph[ii]);
				break;
			case GK_VBAR:
			case GK_VBAR_:
			case GK_VBAR2:
			case GK_VBAR_VIDX:
			case GK_VBAR_REV:
			case GK_VBAR_TK:
				pGrpBase = new CUserVBar(this, (char *)&pGraph[ii]);
				break;
			case GK_VBAR_STACK:
				pGrpBase = new CUserVBar2(this, (char *)&pGraph[ii]);
				pGrpBase->SetValue(ii);
				break;
			case GK_XBAR_NMVAL:
				pGrpBase = new CZBaseXBar(this, (char *)&pGraph[ii]);
				break;
			case GK_XVBAR_VAL:
			case GK_XVBAR_VIDX:
			case GK_XVBAR_NMVAL:
				pGrpBase = new CZBaseXVBar(this, (char *)&pGraph[ii]);
				break;
			case GK_FULLPIE:
				pGrpBase = new CFullPie(this, (char *)&pGraph[ii]);
				break;
			case GK_SPLITPIE:
				pGrpBase = new CSplitPie(this, (char *)&pGraph[ii]);
				break;
			case GK_NDTB:
				pGrpBase = new CNDistribution(this, (char *)&pGraph[ii]);
				break;
			//case GK_MKPRO:
			//	pGrpBase = new CMarketProfile(this, (char *)&pGraph[ii]);
			//	break;
			default:
				break;
			}
			break;
		default:
			TRACE("Graph type is invalid[%d]\n", pGraph[ii].type);
			continue;
		}

		if (pGrpBase != (CGrpBase *) 0)
		{
			if (bAppend)
				pGrpBase->m_bAdd = true;
			m_GraphQue.Add(pGrpBase);	
		}
	}

	return pGrpBase;
}

void CGrpObject::RemoveGraphQue()
{
	CGrpBase* pGrpBase = NULL;
	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pGrpBase = m_GraphQue.GetAt(ii);
		delete pGrpBase;
		pGrpBase = NULL;
	}
	m_GraphQue.RemoveAll();
	m_pSelLineGrp = NULL;
}

bool CGrpObject::RemoveGraphQue(CGrpBase* pBase)
{
	CGrpBase* pTemp = (CGrpBase *)NULL;
	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pTemp = m_GraphQue.GetAt(ii);
		if (pBase == pTemp && pTemp->IsAdditionalType() >= 0)
		{
			delete pTemp;
			pTemp = NULL;
			m_GraphQue.RemoveAt(ii);
			return true;
		}
	}
	return false;
}

bool CGrpObject::RemoveGraphQue(int rgnKey)
{
	bool response = false;

	CGrpBase* pTemp;
	int	tempRgnKey;
	int	Addition;
	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pTemp = m_GraphQue.GetAt(ii);
		tempRgnKey = pTemp->GetGraphRegionID();
		Addition = pTemp->IsAdditionalType();

		// 원래 그려진 그래프가 존재
		if (rgnKey == tempRgnKey && Addition < 0)
		{			
			response = true;
			continue;
		}
		if (rgnKey == tempRgnKey && Addition >= 0)
		{
			delete pTemp;
			pTemp = NULL;
			m_GraphQue.RemoveAt(ii);
		}
	}

	return response;
}

int CGrpObject::GetCountAddGraph(int rgnKey)
{
	int Addition;
	CGrpBase* pTemp;
	int  count = 0, tempRgnKey;

	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pTemp = m_GraphQue.GetAt(ii);
		tempRgnKey = pTemp->GetGraphRegionID();
		Addition = pTemp->IsAdditionalType();

		if (rgnKey == tempRgnKey && Addition >= 0)
			count++;
	}

	return count;
}

int CGrpObject::GetCountRgnGraph(int rgnKey, int& idx)
{
	CGrpBase* pTemp;
	int  count = 0, tempRgnKey;
	idx = 0;
	CUIntArray aVal;
	aVal.RemoveAll();
	for (int ii = 0; ii < m_GraphQue.GetSize(); ii++)
	{
		pTemp = m_GraphQue.GetAt(ii);
		tempRgnKey = pTemp->GetGraphRegionID();
		if (rgnKey == tempRgnKey)
		{
			aVal.Add((UINT)pTemp->GetDupIndex());
			count++;
		}
	}

	if (count > 0)
	{
		int value = 0;
		for (ii = 0; ii <= MAXDUP; ii++)
		{
			for (int jj = 0; jj < aVal.GetSize(); jj++)
			{
				value = (int)aVal.GetAt(jj);
				if (ii == value)
					break;
			}
			if (jj == aVal.GetSize())
			{
				idx = ii;
				break;
			}
			else 
				continue;
		}
	}
	
	aVal.RemoveAll();
	return count;
}


int CGrpObject::GetCountRgnGraph(int rgnKey)
{
	CGrpBase* pTemp;
	int  count = 0, tempRgnKey;

	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pTemp = m_GraphQue.GetAt(ii);
		tempRgnKey = pTemp->GetGraphRegionID();
		if (rgnKey == tempRgnKey)
			count++;
	}
	return count;
}

CToolBase* CGrpObject::AddToolQue(int toolKind, CGrpBase *pGrpBase)
{
	CToolBase* pToolBase = (CToolBase *) 0;

	switch (toolKind)
	{
	case tkTENDENCY:
		pToolBase = new CTendency(this, pGrpBase, tkTENDENCY);
		break;
	case tkUSRINPUT:
		{
			m_ObjStatus &= ~GOS_DURATION;
			m_ObjStatus &= ~GOS_MODIFY;
			DurationCursorModify(true);
			CString	InputStr;

			if (axGrcInputDlg(InputStr) == IDCANCEL)
			{
				return (CToolBase *) 0;
			}
			
			pToolBase = new CUsrInput(this, pGrpBase, tkUSRINPUT);
			pToolBase->SetDisplayString(InputStr);
		}
		break;
/////////////////////////////////////////////////////////////////////////////
//	case tkFIBOLINE:
//		pToolBase = new CFiboLine(this, pGrpBase, tkFIBOLINE);
//		break;
//	case tkFIBOFAN:
//		pToolBase = new CFiboFan(this, pGrpBase, tkFIBOFAN);
//		break;
//	case tkFIBOARC:
//		pToolBase = new CFiboArc(this, pGrpBase, tkFIBOARC);
//		break;
//	case tkFIBORET:
//		pToolBase = new CFiboRet(this, pGrpBase, tkFIBORET);
//		break;


	case tkFIBOLINE:
		pToolBase = new CArrowTool(this, pGrpBase, tkFIBOLINE);
		break;
	case tkFIBOFAN:
		pToolBase = new CArrowTool(this, pGrpBase, tkFIBOFAN);
		break;
	case tkFIBOARC:
		pToolBase = new CArrowTool(this, pGrpBase, tkFIBOARC);
		break;
	case tkFIBORET:
		pToolBase = new CArrowTool(this, pGrpBase, tkFIBORET);
		break;
///////////////////////////////////////////////////////////////////////////////////
/*
	case tkGANNLINE:
		pToolBase = new CGannLine(this, pGrpBase, tkGANNLINE);
		break;
	case tkGANNFAN:
		pToolBase = new CGannFan(this, pGrpBase, tkGANNFAN);
		break;

	case tkACCELFAN:
		pToolBase = new CAccelFan(this, pGrpBase, tkACCELFAN);
		break;
	case tkACCELARC:
		pToolBase = new CAccelArc(this, pGrpBase, tkACCELARC);
		break;
*/
/////////////////////////////////////////////////////////////////////////////
	case tkGANNLINE:
		pToolBase = new CDiagram(this, pGrpBase, tkGANNLINE);
		break;
	case tkGANNFAN:
		pToolBase = new CDiagram(this, pGrpBase, tkGANNFAN);
		break;

	case tkACCELFAN:
		pToolBase = new CDiagram(this, pGrpBase, tkACCELFAN);
		break;
	case tkACCELARC:
		pToolBase = new CDiagram(this, pGrpBase, tkACCELARC);
		break;
/*
	case tkACCELFAN:
		pToolBase = new CHorzLine(this, pGrpBase, tkACCELFAN);
		break;
	case tkACCELARC:
		pToolBase = new CHorzLine(this, pGrpBase, tkACCELARC);
		break;
*/
/////////////////////////////////////////////////////////////////////////////
	case tkCROSS:
		pToolBase = new CAdCross(this, pGrpBase, tkCROSS);
		break;
	case tkTIRONE:
	case tkQUADRANT:
	
	default:
		return (CToolBase *) 0;
	}

	if (pToolBase != (CToolBase *) 0)
		m_ToolQue.Add(pToolBase);

	return pToolBase;
}

void CGrpObject::RemoveToolQue()
{
	CToolBase* pToolBase = (CToolBase *)NULL;
	for (int ii = m_ToolQue.GetUpperBound(); ii >= 0; ii--)
	{
		pToolBase = m_ToolQue.GetAt(ii);
		delete pToolBase;
		pToolBase = NULL;
	}

	m_ToolQue.RemoveAll();
}

void CGrpObject::RemoveLastToolQue()
{
	int idx = m_ToolQue.GetUpperBound();
	if (idx < 0)
		return;
	CToolBase* pToolBase = m_ToolQue.GetAt(idx);
	if (pToolBase == NULL)
		return;

	delete pToolBase;
	pToolBase = NULL;
	m_ToolQue.RemoveAt(idx);
	
	m_tkSAVE = m_ToolQue.GetUpperBound();
	
}

int CGrpObject::GetDataCount(int key)
{
	CGrpData* pGrpData;
	
	if (m_MapData.Lookup(key, pGrpData))
		return pGrpData->GetArraySize();
	return -1;
}

bool CGrpObject::GetGrpTick(int id, CRect& rect, int& scale, int& color)
{
	if (id < 0 || id > 2)
		return false;

	rect   = m_xTick[id].xRect;
	scale  = m_xTick[id].xScale;
	color  = m_xTick[id].xColor;

	return true;
}

bool CGrpObject::GetGrpData(int key, CGrpData* &pGrpData)
{
	if (!m_MapData.Lookup(key, pGrpData))
		return false;
	return true;
}

bool CGrpObject::GetBongDataKey(int& key) 
{ 
	if (m_BongDataKey != -1)
	{
		key = m_BongDataKey;
		return true;
	}
	else
		return false;
}

bool CGrpObject::GetGrpCommonData(CGrpData* &pGrpData)
{
	int	mapKey;
	for (POSITION pos = m_MapData.GetStartPosition(); pos; )
	{
		m_MapData.GetNextAssoc(pos, mapKey, pGrpData);
		if (pGrpData->GetDataPattern() == dpCOMMON)
			return true;
	}
	pGrpData = (CGrpData *) 0;
	return false;
}

bool CGrpObject::GetGrpVidxData(CGrpData* &pGrpData)
{
	int	mapKey;
	for (POSITION pos = m_MapData.GetStartPosition(); pos; )
	{
		m_MapData.GetNextAssoc(pos, mapKey, pGrpData);
		if (pGrpData->GetDataPattern() == dpVIDX)
			return true;
	}
	pGrpData = (CGrpData *) 0;
	return false;
}

bool CGrpObject::GetGrpRegion(int key, CRect& rect)
{
	struct	_xRegion* xRegion;
	if (!m_MapRegion.Lookup(key, xRegion))
		return false;

	rect = xRegion->xRect;
	return true;
}

bool CGrpObject::GetGrpRegion(int key, struct _xRegion*& xRegion)
{
	if (!m_MapRegion.Lookup(key, xRegion))
		return false;

	return true;
}

void CGrpObject::MoveObject(CRect newRC)
{
	struct	_svRGN{
		int	key;
		int	left;
		int	top;
		int	right;
		int	bottom;
		int	orgkey;
	}svRGN[20];

	struct _xRegion* xRegion;
	int	RgnKey;
	int	minTop = 10000, maxBottom = -1;
	int	minLeft = 10000, maxRight = -1;
	for (POSITION pos = m_MapRegion.GetStartPosition(); pos;)
	{
		m_MapRegion.GetNextAssoc(pos, RgnKey, xRegion);

		if (xRegion->sRect.top < minTop)
			minTop = xRegion->sRect.top;
		if (xRegion->sRect.bottom > maxBottom)
			maxBottom = xRegion->sRect.bottom;

		if (xRegion->sRect.left < minLeft)
			minLeft = xRegion->sRect.left;

		if (xRegion->sRect.right > maxRight)
			maxRight = xRegion->sRect.right;
	}

	int	TotalH = maxBottom - minTop;
	int	TotalW = maxRight - minLeft;
	if (TotalH == 0 || TotalW == 0)
		return;

	int idx = 0;
	ZeroMemory(svRGN, sizeof(struct _svRGN) * 20);
	for (pos = m_MapRegion.GetStartPosition(); pos; idx++)
	{
		m_MapRegion.GetNextAssoc(pos, RgnKey, xRegion);
		svRGN[idx].key = RgnKey;
		svRGN[idx].left = (xRegion->sRect.left - minLeft)*100 / TotalW;
		svRGN[idx].right = (xRegion->sRect.right - minLeft)*100 / TotalW;
		svRGN[idx].top = (xRegion->sRect.top - minTop)*100 / TotalH;
		svRGN[idx].bottom = (xRegion->sRect.bottom - minTop)*100 / TotalH;
		svRGN[idx].orgkey = xRegion->orgRgnKey;
	}

	CRect	newOrgRC;	// resized graph object region
	newOrgRC = newRC;

	m_fWidth = m_fWidth * float(newOrgRC.Width()) / float(m_mapRECT.Width());
	m_fHeight = m_fHeight * float(newOrgRC.Height()) / float(m_mapRECT.Height());

	m_mapRECT = newOrgRC;
	m_ObjRect = newOrgRC;
	m_GrpRect = newOrgRC;
	m_ObjRegion.SetRectEmpty();
	m_GrpRegion.SetRectEmpty();

	struct _fgraph *pFgraph = m_DataFormat->GetGrpFileInfo();
	AddGraphTick((char *)&pFgraph->xtick, xBOTTOM);
	AddGraphTick((char *)&pFgraph->ltick, xLEFT);
	AddGraphTick((char *)&pFgraph->rtick, xRIGHT);

	int	gWidth  = m_GrpRect.Width();
	int	gHeight = m_GrpRect.Height();
	for (idx = 0, pos = m_MapRegion.GetStartPosition(); pos; idx++)
	{
		m_MapRegion.GetNextAssoc(pos, RgnKey, xRegion);

		xRegion->xRect.left = m_GrpRect.left + ((gWidth * svRGN[idx].left) / 100);
		xRegion->xRect.right = m_GrpRect.left + ((gWidth * svRGN[idx].right) / 100);
		xRegion->xRect.top = m_GrpRect.top + ((gHeight * svRGN[idx].top) / 100);
		xRegion->xRect.bottom = m_GrpRect.top + ((gHeight * svRGN[idx].bottom) / 100);

		xRegion->orgRgnKey = svRGN[idx].orgkey;
		xRegion->sRect = xRegion->xRect;
	}

	ResizeGraphObject(m_hRatio, m_vRatio, true);
}

void CGrpObject::ResizeGraphObject(float hRatio, float vRatio, bool bUpdate, bool bForce, bool bSplit)
{
	if (m_bAutoTran)	m_cs.Lock();

	m_hRatio = hRatio;
	m_vRatio = vRatio;
	
	CRect calcRect = CalculateRect(m_ObjRect, hRatio, vRatio);
	if (!bForce && m_ObjRegion.EqualRect(&calcRect))
	{
		if (m_bAutoTran)	m_cs.Unlock();
		return;
	}
	m_ObjRegion = calcRect;

	calcRect = CalculateRect(m_GrpRect, hRatio, vRatio);
	m_GrpRegion = calcRect;
	int ii = 0;

	m_RealGrpRgn.SetRect(3000, 3000, 0, 0);
	CGrpBase* pGrpBase;
	CRect	DrawRgn;

	for (ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		pGrpBase->ResizeGraph(hRatio, vRatio, true);

		DrawRgn = pGrpBase->GetDrawRect();
		if (IsObjEditable())
		{
			DrawRgn.right += m_rGap;
			DrawRgn.left -= m_lGap;
		}
		if (!DrawRgn.IsRectEmpty())
		{
			if (m_RealGrpRgn.left > DrawRgn.left)
				m_RealGrpRgn.left = DrawRgn.left;
			if (m_RealGrpRgn.right < DrawRgn.right)
				m_RealGrpRgn.right = DrawRgn.right;
			if (m_RealGrpRgn.top > DrawRgn.top)
				m_RealGrpRgn.top = DrawRgn.top;
			if (m_RealGrpRgn.bottom < DrawRgn.bottom)
				m_RealGrpRgn.bottom = DrawRgn.bottom;
		}
	}

	m_GrpRegion = m_RealGrpRgn;

	int	scale, color;
	GetGrpTick(0, DrawRgn, scale, color);
	if (!DrawRgn.IsRectEmpty())
	{
		if (m_RealGrpRgn.bottom < int(DrawRgn.bottom * vRatio))
			m_RealGrpRgn.bottom = int(DrawRgn.bottom * vRatio);
	}

	GetGrpTick(1, DrawRgn, scale, color);
	if (!DrawRgn.IsRectEmpty())
	{
		if (m_RealGrpRgn.left > int(DrawRgn.left * hRatio))
			m_RealGrpRgn.left = int(DrawRgn.left * hRatio);
	}

	GetGrpTick(2, DrawRgn, scale, color);
	if (!DrawRgn.IsRectEmpty())
	{
		if (m_RealGrpRgn.right < int(DrawRgn.right * hRatio))
			m_RealGrpRgn.right = int(DrawRgn.right * hRatio);
	}
	m_RealGrpRgn.right += m_RealGrpRgn.left;
	m_RealGrpRgn.bottom += m_RealGrpRgn.top;

	CToolBase* pToolBase;
	for (ii = 0; ii <= m_ToolQue.GetUpperBound(); ii++)
	{
		pToolBase = (CToolBase *) m_ToolQue.GetAt(ii);
		pToolBase->ResizeTool(hRatio, vRatio);
	}
	if (m_pCrossLine)
		m_pCrossLine->ResizeTool(hRatio, vRatio);

	if (bUpdate)
	{
		ReDrawGraphObject();
	}

	if (m_bUsableGrid && !m_GrpRegion.IsRectEmpty())
	{
		m_pGrid->MoveGrid(CPoint(m_ObjRegion.left, m_ObjRegion.top));
	}

	if (m_bAutoTran)	m_cs.Unlock();

}

void CGrpObject::DrawGraphObject(CDC* pDC, CRect rc)
{
	m_ObjRegion = rc;
	m_RealGrpRgn = m_ObjRegion;
	m_cs.Lock();

	CGrpBase* pGrpBase;
	if (m_GraphQue.GetSize() <= 0)
	{
		DrawEmpty(pDC, m_ObjRegion);
		m_cs.Unlock();
		return;
	}

	bool	bUseORGDC = false;

	if (!pDC->IsPrinting())
	{
		CDC memDC;
		CRect bitmapRC = m_RealGrpRgn;
		CBitmap bitmap, *oldBitmap;

		if (memDC.CreateCompatibleDC(pDC))
		{
			if (bitmap.CreateCompatibleBitmap(pDC, bitmapRC.Width(), bitmapRC.Height()))
			{
				oldBitmap = (CBitmap *) memDC.SelectObject(&bitmap);

				for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
				{
					pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
					if (!ii)
					{
						pGrpBase->DrawTickBox(&memDC, m_tRGB, m_pRGB);
					}
					pGrpBase->DrawGraph(&memDC);
				}

				for (ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
				{
					pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
					pGrpBase->DrawSplit(&memDC);
				}

				CToolBase* pToolBase;
				for (ii = 0; ii <= m_ToolQue.GetUpperBound(); ii++)
				{
					pToolBase = (CToolBase *) m_ToolQue.GetAt(ii);
					pToolBase->DrawTool(&memDC);
				}
				pDC->BitBlt(bitmapRC.left, bitmapRC.top, 
					bitmapRC.Width() - bitmapRC.left, 
					bitmapRC.Height() - bitmapRC.top, 
					&memDC, bitmapRC.left, bitmapRC.top, SRCCOPY);
				memDC.SelectObject(oldBitmap);
				bitmap.DeleteObject();
			}
			else
				bUseORGDC = true;
		}
		else
			bUseORGDC = true;
	}
	else
		bUseORGDC = true;
	
	if (bUseORGDC)
	{
		for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
		{
			pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
			if (!ii)
				pGrpBase->DrawTickBox(pDC, m_tRGB, m_pRGB);

			pGrpBase->DrawGraph(pDC);
		}

		for (ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
		{
			pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
			pGrpBase->DrawSplit(pDC);
		}

		CToolBase* pToolBase;
		for (ii = 0; ii <= m_ToolQue.GetUpperBound(); ii++)
		{
			pToolBase = (CToolBase *) m_ToolQue.GetAt(ii);
			pToolBase->DrawTool(pDC);
		}
	}

	m_saveRIGHT.SetRectEmpty();
	m_saveLEFT.SetRectEmpty();
	m_cs.Unlock();
}

void CGrpObject::DrawEmpty(CDC* pDC, CRect rc)
{
	int sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF	color = m_pRGB;

	CPen	cPen(PS_SOLID, 1, color);
	CBrush	cBrush(color);
	CPen*	sPen = pDC->SelectObject(&cPen);
	CBrush* sBrush = pDC->SelectObject(&cBrush);

	pDC->Rectangle(&rc);

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
	cPen.DeleteObject();
	cBrush.DeleteObject();
	pDC->SetBkMode(sMode);
}

void CGrpObject::ReDrawGraphObject()
{
	ASSERT(m_pGraph);

	BOOL	bErase = (m_transparent) ? TRUE : FALSE;
	CRect	drawAREA = m_ObjRegion;
	m_pGraph->InvalidateRect(drawAREA);
}

void CGrpObject::ReDrawRegion(CRect& rect)
{
	ASSERT(m_pGraph);

	BOOL	bErase = (m_transparent) ? TRUE : FALSE;
	CRect	drawAREA = rect;
	m_pGraph->InvalidateRect(&drawAREA);
}

void CGrpObject::ReDrawRTMRegion(bool bForceErase)
{
	ASSERT(m_pGraph);

	BOOL	bErase = (m_transparent || bForceErase) ? TRUE : FALSE;
	CRect	drawAREA = m_ObjRegion;
	m_pGraph->InvalidateRect(&drawAREA);

	if (m_bNoSplit)
	{
		m_pGraph->InvalidateRect(&drawAREA);
		return;
	}

	CGrpBase	*pGrpBase;
	CRgnSplit	*pRgnSplit;
	CRect	GrpRect;

	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		pRgnSplit = pGrpBase->GetRgnSplit();

		GrpRect = pGrpBase->GetDrawRect();
		GrpRect.left = drawAREA.left;
		GrpRect.right = drawAREA.right;
		if (pRgnSplit && !pRgnSplit->m_DrawRgn.IsRectEmpty())
			GrpRect.bottom = pRgnSplit->m_DrawRgn.top;

		m_pGraph->InvalidateRect(&GrpRect);

	}

	if (pGrpBase)
	{
		pGrpBase->GetxTickRect(GrpRect);
		m_pGraph->InvalidateRect(&GrpRect);
	}

}

COLORREF CGrpObject::GetPaletteRGB(int index)
{
	if (index & rgbCOLOR)	// PALETTERGB
		return COLORREF(index);

	if (index < 0 || index > 255)
		index = _BLACK;

	return m_pMgr->GetPaletteRGB(index);
}


bool CGrpObject::AppendGraphChart(CGrpBase* pBase, int OrgRgnKey, int aType, int aKind)
{
	struct	_xRegion* xRegion, newRegion;
	int CurRgnKey = pBase->GetGraphRegionID();
	m_MapRegion.Lookup(CurRgnKey, xRegion);

	int xUNIT = xRegion->xRect.Height() / 2;
	int sUNIT = xRegion->sRect.Height() / 2;

	if (OrgRgnKey < 0)
		OrgRgnKey = CurRgnKey;

	CopyMemory(&newRegion, xRegion, sizeof (struct _xRegion));
	xRegion->xRect.bottom -= xUNIT;
	xRegion->sRect.bottom -= sUNIT;

	newRegion.xRect.top = xRegion->xRect.bottom;
	newRegion.sRect.top = xRegion->sRect.bottom;
	newRegion.orgRgnKey = OrgRgnKey;
	
	struct	_xRegion* appRegion;
	appRegion = (struct _xRegion *)new char [sizeof (struct _xRegion)];

	int	aRegionKey = GetUnusedRegionKey();
	CopyMemory(appRegion, &newRegion, sizeof (struct _xRegion));
	m_MapRegion.SetAt(aRegionKey, appRegion);

	CGrpBase* pGrpLast;
	for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
	{
		pGrpLast = (CGrpBase *) m_GraphQue.GetAt(ii);
		if (pGrpLast->GetGraphRegionID() == CurRgnKey)
			break;
	}


	struct _graph	graph;
	memset(&graph, 0x00, sz_GRAPH);
	graph.type   = aType;
	graph.kind   = aKind;
	graph.key    = m_BongDataKey;

	graph.region = aRegionKey;
	graph.tick   = pGrpLast->GetGraphTickInfo();
	
	COLORREF	fg;
	pBase->GetBasicColor(&fg, NULL, NULL, false);
	graph.fgcolor = fg;
	graph.ApplyFormula = 0;
	for (ii = 0; ii < maxAVLINE; ii++)
	{
		graph.value[ii] = 0;
		graph.icolor[ii] = 0;
	}

	CGrpBase* pGrpAppend = AddGraphQue((char *) &graph, true);
	SyncGraphDisplay(pBase, pGrpAppend);
	pGrpAppend->SetAdditionalType();

	ResizeGraphObject(m_hRatio, m_vRatio, true, true);
	return true;
}


void CGrpObject::dupChartList(int rgnKey, char* buf)
{
	CGrpBase	*pTemp;
	int		Addition;
	int		tempRgnKey;
	struct	_dupSet dupSet;
	memset(&dupSet, 0x00, sz_DUPSET);

	int cnt = GetCountAddGraph(rgnKey);
	if (cnt > 0)
	{
		for (int ii = 0, jj = 0; ii < m_GraphQue.GetSize(); ii++)
		{
			pTemp = m_GraphQue.GetAt(ii);
			tempRgnKey = pTemp->GetGraphRegionID();
			Addition = pTemp->IsAdditionalType();

			if (rgnKey == tempRgnKey && Addition >= 0)
			{
				dupSet.dupInfo[jj].key = rgnKey;
				dupSet.dupInfo[jj].kind = pTemp->GetGraphKind();
				dupSet.dupInfo[jj].type = pTemp->GetGraphType();
				dupSet.dupInfo[jj].ichkcolor = pTemp->m_ichkcolor;
				for (int kk = 0; kk < maxAVLINE; kk++)
				{
					dupSet.dupInfo[jj].value[kk] = pTemp->m_value[kk];
					dupSet.dupInfo[jj].icolor[kk] = pTemp->m_icolor[kk];
				}
				jj++;
			}
		}
	}
	else if (cnt == 0)
	{
		dupSet.dupInfo[0].key = rgnKey;
	}
	CopyMemory(buf, &dupSet, sz_DUPSET);
}


void CGrpObject::deleteDupChart(int delRgnKey, bool bAllClear)
{
	CGrpBase	*pTemp;
	int		Addition;
	int		tempRgnKey;
	
	if (GetCountAddGraph(delRgnKey) > 0)
	{
		for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--) // delete graph
		{
			pTemp = m_GraphQue.GetAt(ii);
			tempRgnKey = pTemp->GetGraphRegionID();
			Addition = pTemp->IsAdditionalType();

			if (delRgnKey == tempRgnKey && Addition >= 0)
			{
				RemoveGraphQue(pTemp);
			} 
		}
	}
	m_pSelLineGrp = NULL;
	if (bAllClear && m_pMgr->m_bTotal)
	{
		struct _dupGraph dupGraph;
		memset(&dupGraph, 0x00, sz_DUPGRAPH);
		m_pMgr->m_pView->SendMessage(GEV_VIEW, 
			MAKEWPARAM(viewDup, 1), (long)&dupGraph);
	}
}

bool CGrpObject::DeleteGraphChart(CGrpBase* pBase)
{
	struct	_xRegion *xDelete, *xRegion;

	if (pBase->IsAdditionalType() < 0)
		return false;

	int delRgnKey = pBase->GetGraphRegionID();
	if (!m_MapRegion.Lookup(delRgnKey, xDelete))
	{
		TRACE("Delete Region not found !!\n");
		return false;
	}

	POSITION	pos;
	CGrpBase	*pTemp;
	int		Addition;
	int		tempRgnKey;
	
	if (GetCountAddGraph(delRgnKey) > 0)
	{
		for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--) // delete graph
		{
			pTemp = m_GraphQue.GetAt(ii);
			tempRgnKey = pTemp->GetGraphRegionID();
			Addition = pTemp->IsAdditionalType();

			if (delRgnKey == tempRgnKey && Addition >= 0)
			{
				if (m_pMgr->m_bTotal)
				{
					struct _dupGraph dupGraph;
					struct _dupInfo dupData;
					m_pMgr->m_pView->SendMessage(GEV_VIEW, 
						MAKEWPARAM(viewDup, 2), (long)&dupGraph);
					for (int ii = 0; ii < MAXDUP; ii++)
					{
						memset(&dupData, 0x00, sz_DUPINFO);
						dupGraph.dupInfo[tempRgnKey-1][ii] = dupData;
					}
					m_pMgr->m_pView->SendMessage(GEV_VIEW, 
						MAKEWPARAM(viewDup, 1), (long)&dupGraph);
				}
				m_pSelLineGrp = NULL;
				RemoveGraphQue(pTemp);
			}
		}
	}
	int	OrgRgnKey = xDelete->orgRgnKey;
	if (OrgRgnKey == unusedKEY)
	{
		ResizeGraphObject(m_hRatio, m_vRatio, true, true);
		return true;
	}

	int	rgnKey;
	for (pos = m_MapRegion.GetStartPosition(); pos; )
	{
		m_MapRegion.GetNextAssoc(pos, rgnKey, xRegion);
		if (xRegion->orgRgnKey == OrgRgnKey 
			&& xRegion->sRect.top < xDelete->sRect.top)
		{
			xRegion->sRect.OffsetRect(0, xDelete->sRect.Height());
			xRegion->xRect.OffsetRect(0, xDelete->xRect.Height());
		}
	}

	if (!m_MapRegion.Lookup(OrgRgnKey, xRegion))
	{
		TRACE("Original Region not found !!\n");
		return false;
	}

	xRegion->sRect.bottom += xDelete->sRect.Height();
	xRegion->xRect.bottom += xDelete->xRect.Height();

	RemoveGraphRegion(delRgnKey);	// if that region is added, delete region 
	ResizeGraphObject(m_hRatio, m_vRatio, true, true);

	return true;
}

void CGrpObject::ModifyColor(CGrpBase *pGrpBase)
{
	COLORREF	ifg, ibg, iad, itk, tfg;
	COLORREF	fg, bg, ad, tk;

	if (GetCountRgnGraph(pGrpBase->GetGraphRegionID()) > 1)
	{
		char	tmp[64];
		sprintf(tmp, "중복된 그래프가 있습니다.");
		::MessageBox(NULL, tmp, "확인", MB_ICONWARNING|MB_OK);
		return;
	}

	struct _fgraph *pFgraph = m_DataFormat->GetGrpFileInfo();
	pGrpBase->GetBasicColor(&ifg, NULL, NULL, false);
	tfg = ifg;
	ibg = pFgraph->bgcolor;
	iad = pFgraph->adcolor;
	itk = pFgraph->fgcolor;

	if (axGrcColorModDlg(ifg, ibg, iad, itk) == IDOK)
	{
		fg = GetPaletteRGB(ifg);
		bg = GetPaletteRGB(ibg);
		ad = GetPaletteRGB(iad);
		tk = GetPaletteRGB(itk);
		m_pRGB = bg;

		if (pFgraph->bgcolor != ibg || tfg != ifg || 
			pFgraph->adcolor != iad || pFgraph->fgcolor != itk)
		{
			CGrpBase* tGrpBase;

			m_aRGB = ad;
			for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
			{
				tGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
				tGrpBase->SetBasicColor(-1, bg, ad, -1, true);
			}

			struct _fgraph fgraph;

			CopyMemory(&fgraph, pFgraph, sz_FGRAPH);
			fgraph.bgcolor = ibg;
			fgraph.adcolor = iad;
			fgraph.fgcolor = itk;
			m_tRGB = GetPaletteRGB(itk);

			if (tfg != ifg)
			{
				pGrpBase->SetBasicColor(ifg, -1, -1, false);
				_graph	*pgraph;
				for (int ii = 0; ii < maxGRAPH; ii++)
				{
					pgraph = &fgraph.graph[ii];
					if (pgraph->region == pGrpBase->GetGraphRegionID())
					{
						pgraph->fgcolor = ifg;
						break;
					}
				}
			}

			ReDrawGraphObject();

			char	dBuf[sz_FGRAPH + 32];
			CopyMemory(dBuf, &fgraph, sz_FGRAPH);

			m_pMgr->FileInfo(1000, dBuf, sz_FGRAPH, m_ObjName);
			m_DataFormat->SetFgraph((char *)&fgraph);
		}
	}
}

int CGrpObject::GetGraphKind()
{
	struct _fgraph	*pFgraph = m_DataFormat->GetGrpFileInfo();
	return pFgraph->graph[0].kind;
}

void CGrpObject::ChartChange(int kind)
{
	struct _fgraph	*pFgraph = m_DataFormat->GetGrpFileInfo();
	struct _fgraph	fgraph;
	CopyMemory(&fgraph, pFgraph, sz_FGRAPH);
	fgraph.graph[0].kind = kind;
	changeObject(fgraph);
}

void CGrpObject::GraphConfig()
{
	CGrpBase* pGrpBase = (CGrpBase *) m_GraphQue.GetAt(0);
	if (pGrpBase == NULL)
		return;

	struct _fgraph	fgraph;
	struct _fgraph	orgFile;
	struct _addGrp	addGrp;
	
	struct _fgraph	*pFgraph = m_DataFormat->GetGrpFileInfo();
	m_pMgr->FileInfo(2000, (char *)&orgFile, sz_FGRAPH, m_ObjName, true);
	CopyMemory(&fgraph, pFgraph, sz_FGRAPH);

	switch (fgraph.graph[0].kind)
	{
	case GK_JPN:
	case GK_LIN:
	case GK_EQUI:
		m_pMgr->AddFileInfo(2000, (char *)&addGrp, sz_ADDGRP, (char *)&fgraph, sz_FGRAPH);
		if (!axGrcConfig(&fgraph, &orgFile, &addGrp))
			return;
		m_pMgr->AddFileInfo(1000, (char *)&addGrp, sz_ADDGRP, (char *)&fgraph, sz_FGRAPH);
		break;
	case GK_TURN:
	case GK_CLOCK:
	case GK_PV:
		return;
	default:
		if (!axGrcConfig(&fgraph, &orgFile))
			return;
		break;
	}
	
	CGrpBase* pBase = (CGrpBase *)NULL;
	for (int kk = 0; kk < m_GraphQue.GetSize(); kk++)
	{
		pBase = m_GraphQue.GetAt(kk);
		if (pBase == NULL)
			continue;
		deleteDupChart(pBase->GetGraphRegionID(), true);
	}
	changeObject(fgraph);
}

void CGrpObject::changeObject(struct _fgraph fgraph, bool bChange)
{
	RemoveGrpComponent(false);

	struct _fgraph tmpFgraph;
	memcpy(&tmpFgraph, &fgraph, sz_FGRAPH);
	struct _addGrp addGrp;
	m_pMgr->AddFileInfo(2000, (char *)&addGrp, sz_ADDGRP, (char *)&tmpFgraph, sz_FGRAPH);
	if (addGrp.vcurr == 1)
		m_bViewCurr = true;
	else
		m_bViewCurr = false;

	m_lMargin = addGrp.lmargin;
	m_rMargin = addGrp.rmargin;

	m_tRGB = GetPaletteRGB(fgraph.fgcolor);
	m_pRGB = GetPaletteRGB(fgraph.bgcolor);
	m_aRGB = GetPaletteRGB(fgraph.adcolor);
	m_GrpRect = m_ObjRect = m_mapRECT;
		
	AddGraphTick((char *)&fgraph.xtick, xBOTTOM);
	AddGraphTick((char *)&fgraph.ltick, xLEFT);
	AddGraphTick((char *)&fgraph.rtick, xRIGHT);
	AddGraphRegion((char *)fgraph.region);

	AddGraphQue((char *)fgraph.graph, false, true);
	
	if (m_bUsableGrid)
	{
		CGrpBase* pGrpBase = ExistBongTypeGraph();
		if (pGrpBase == (CGrpBase *) 0)
			pGrpBase = m_GraphQue.GetAt(0);

		m_pCrossLine = new CCrossLine(this, pGrpBase, tkCROSSLINE);
		m_ObjStatus |= GOS_CROSSLINE;
	}

	SetBongOption(fgraph.upcolor, fgraph.dncolor, fgraph.operation);
	if (bChange)
	{
		if (m_pMgr->m_bTotal)
		{
			int type = m_pMgr->m_pView->SendMessage(GEV_CTRL, MAKEWPARAM(ctrlGetType, 0), 0);
			SetObjType(type);

			int index = m_pMgr->m_pView->SendMessage(GEV_INPUT, 
					MAKEWPARAM(inputIndex, 1), 0);
			if (index == GI_TICK)
				SetObjType(ENV_NONE);
		}
		ResizeGraphObject(m_hRatio, m_vRatio, true, true);
	}
	else
		ResizeGraphObject(m_hRatio, m_vRatio, false, true);

	char	dBuf[sz_FGRAPH + 32];
	CopyMemory(dBuf, &fgraph, sz_FGRAPH);
	m_pMgr->FileInfo(1000, dBuf, sz_FGRAPH, m_ObjName);
	m_DataFormat->SetFgraph((char *)&fgraph);
}

void CGrpObject::GraphAppend()
{
	CGrpBase* pGrpBase = (CGrpBase *) m_GraphQue.GetAt(0);
	if (pGrpBase == NULL)
		return;

	if (m_pMgr->m_grxType == DEF_SALE || m_pMgr->m_grxType == DEF_EQUIVOL)
		return;

	struct _fgraph	*pFgraph = m_DataFormat->GetGrpFileInfo();
	struct _fgraph	fgraph;
	struct _addGrp	addGrp;
	CopyMemory(&fgraph, pFgraph, sz_FGRAPH);
	m_pMgr->AddFileInfo(2000, (char *)&addGrp, sz_ADDGRP, (char *)&fgraph, sz_FGRAPH);
	switch (pGrpBase->GetGraphKind())
	{
	case GK_BALANCE:
	case GK_TURN:
	case GK_CLOCK:
	case GK_PV:
	case GK_PNF:
	case GK_EVOL:
	case GK_EQUI:
		return;
	
	default:
		m_bAppendMode = true;
		if (!axAppendChart((char *)&fgraph, (char *)&addGrp))
			return;
		m_pMgr->AddFileInfo(1000, (char *)&addGrp, sz_ADDGRP, (char *)&fgraph, sz_FGRAPH);
	
		{
			CGrpBase* pBase = (CGrpBase *)NULL;
			for (int kk = 0; kk < m_GraphQue.GetUpperBound(); kk++)
			{
				pBase = m_GraphQue.GetAt(kk);
				if (pBase == NULL)
					continue;
				deleteDupChart(pBase->GetGraphRegionID(), true);
			}
		}
	}
	
	changeObject(fgraph);
}




void CGrpObject::ChartConfig(CGrpBase *pGrpBase)
{
	struct _fgraph	*pFgraph = m_DataFormat->GetGrpFileInfo();
	struct _fgraph	fgraph;
	struct _addGrp	addGrp;

	struct _fgraph	orgFile;
	m_pMgr->FileInfo(2000, (char *)&orgFile, sz_FGRAPH, m_ObjName, true);
	CopyMemory(&fgraph, pFgraph, sz_FGRAPH);
	m_pMgr->AddFileInfo(2000, (char *)&addGrp, sz_ADDGRP, (char *)&fgraph, sz_FGRAPH);
	switch (pGrpBase->GetGraphKind())
	{
	case GK_BALANCE:
	case GK_TURN:
	case GK_CLOCK:
	case GK_PV:
	case GK_PNF:
		if (!axGrcConfig(&fgraph, &orgFile))
			return;
		changeObject(fgraph);
		break;
	default:
		{
			int dnum = fgraph.count;
			int disp = fgraph.date;

			if (!axGrcClientConfig(m_ObjName, (char *)&fgraph, m_pMgr->m_grxType, (char *)&addGrp, &orgFile))
				return;
			m_pMgr->AddFileInfo(1000, (char *)&addGrp, sz_ADDGRP, (char *)&fgraph, sz_FGRAPH);
			
			CGrpBase* pBase = (CGrpBase *)NULL;
			for (int kk = 0; kk < m_GraphQue.GetSize(); kk++)
			{
				pBase = m_GraphQue.GetAt(kk);
				if (pBase == NULL)
					continue;
				deleteDupChart(pBase->GetGraphRegionID(), true);
			}
			
			if (m_pMgr->m_bTotal)
			{
				changeObject(fgraph, false);
				CString tmpS;
				tmpS.Format("%d\t%d", fgraph.date, fgraph.count);
				m_pMgr->m_pView->SendMessage(GEV_INPUT, MAKEWPARAM(inputDispDnum, 2), (LPARAM)(LPCTSTR)tmpS);	
			}
			else
				changeObject(fgraph);

		}
		break;
	}
	
}

void CGrpObject::ChangeFGraph(char* buf, bool bClear)
{
	m_DataFormat->SetFgraph(buf);
	if (bClear)
	{
		RemoveGrpComponent(false);
		ResizeGraphObject(m_hRatio, m_vRatio, true, true);
	}
}

bool CGrpObject::ChangeDisplay(int dispDay)
{
	bool bReturn;
	CGrpBase* pGrpBase;
	for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		bReturn = pGrpBase->SetDisplay(dispDay);
		if (bReturn)
		{
			CRect rect = m_ObjRegion;
			ReDrawRegion(rect);
		}
	}

	CToolBase	*pToolBase;
	for (int jj = m_ToolQue.GetUpperBound(); jj >= 0; jj--)
	{
		pToolBase = m_ToolQue.GetAt(jj);
		pToolBase->InflateHolder();
		if (pToolBase->m_showHolder)
			pToolBase->DrawHolder(false);
		pToolBase->m_showHolder = false;
	}

	return bReturn;
}

bool CGrpObject::ChangeDisplayScale(int& scale)
{
	bool bReturn;
	CGrpBase* pGrpBase;

	for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		bReturn = pGrpBase->SetDisplayScale(scale);
		if (bReturn)
		{
			CRect rect = m_ObjRegion;
			ReDrawRegion(rect);
		}
	}
	return bReturn;
}

bool CGrpObject::ChangeDisplayShift(int& shift)
{
	bool bReturn;
	CGrpBase* pGrpBase;

	for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		bReturn = pGrpBase->SetDisplayShift(shift);
		if (bReturn)
		{
			CRect rect = m_ObjRegion;
			ReDrawRegion(rect);
		}
	}
	return bReturn;
}

bool CGrpObject::ChangeDisplayPos(int dispPos, int dispEnd)
{
	CGrpBase* pGrpBase;
	for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		pGrpBase->ChangeDisplayPos(dispPos, dispEnd);
	}
/*
	CToolBase	*pToolBase;

	for (int jj = m_ToolQue.GetUpperBound(); jj >= 0; jj--)
	{
		pToolBase = m_ToolQue.GetAt(jj);
		pToolBase->InflateHolder();
		if (pToolBase->m_showHolder)
			pToolBase->DrawHolder(false);
		pToolBase->m_showHolder = false;
	}
*/
	CRect rect = m_ObjRegion;
	ReDrawRegion(rect);
	return true;
}


void CGrpObject::ToolSelect(int select)
{
	if (select == toolVIEW)
	{
		m_pMgr->m_pView->SendMessage(GEV_VIEW,
			MAKEWPARAM(viewInput, 5), 0);
		return;
	}
	
	if (GetMainBaseGraph() == (CGrpBase *) 0)
		return;

	switch (select)
	{
	case toolSETUP:
		{
			CGrpBase* pGrpBase = ExistBongTypeGraph();
			if (pGrpBase == (CGrpBase *) 0)
				pGrpBase = m_GraphQue.GetAt(0);
			if (pGrpBase)
				ChartConfig(pGrpBase);
			return;
		}
		break;
	case toolNUM:
		{
			SetDataVisible();		
			return;
		}
		break;
	case toolDELETE:
		{
			RemoveToolQue();
			m_tkSAVE = -1;
			ReDrawGraphObject();
			return;
		}
		break;
	case toolHISTORY:
		{
			RemoveLastToolQue();
			ReDrawGraphObject();
			return;
		}
		break;
	}	

	m_tkCHOICED = tkNONE;
	switch (select)
	{
	case toolTEND:
		m_tkCHOICED = tkTENDENCY;
		if (m_ObjStatus & GOS_TENDENCY)
			m_ObjStatus &= ~GOS_TENDENCY;
		else
			m_ObjStatus |= GOS_TENDENCY;
		break;
	case toolINPUT:
		m_tkCHOICED = tkUSRINPUT;
		if (m_ObjStatus & GOS_USRINPUT)
			m_ObjStatus &= ~GOS_USRINPUT;
		else
			m_ObjStatus |= GOS_USRINPUT;
		break;
	case toolFIBOLINE:
		m_tkCHOICED = tkFIBOLINE;
		break;
	case toolFIBOFAN:
		m_tkCHOICED = tkFIBOFAN;
		break;
	case toolFIBOARC:
		m_tkCHOICED = tkFIBOARC;
		break;
	case toolFIBORET:
		m_tkCHOICED = tkFIBORET;
		break;
	case toolGANNLINE:
		m_tkCHOICED = tkGANNLINE;
		break;
	case toolGANNFAN:
		m_tkCHOICED = tkGANNFAN;
		break;
	case toolACCELFAN:
		m_tkCHOICED = tkACCELFAN;
		break;
	case toolACCELARC:
		m_tkCHOICED = tkACCELARC;
		break;
	case toolCROSS:
		m_tkCHOICED = tkCROSS;
		break;
	
	default:
		m_ObjStatus &= ~GOS_MODIFY;
		break;
	}

	m_ObjStatus |= GOS_TCREATE;
	switch (m_tkCHOICED)
	{
	case tkTENDENCY:
		if (m_ObjStatus & GOS_TENDENCY)
		{
			m_ObjStatus &= ~GOS_DURATION;
			m_ObjStatus |= GOS_TENDENCY;
		}
		else
		{
			m_tkCHOICED = tkNONE;
			m_ObjStatus &= ~GOS_MODIFY;
			m_ObjStatus &= ~GOS_DURATION;
		}
		break;
	case tkUSRINPUT:
		if (m_ObjStatus & GOS_USRINPUT)
		{
			m_ObjStatus &= ~GOS_DURATION;
			m_ObjStatus |= GOS_USRINPUT;
		}
		else
		{
			m_tkCHOICED = tkNONE;
			m_ObjStatus &= ~GOS_MODIFY;
			m_ObjStatus &= ~GOS_DURATION;
		}
		break;
	default:
		m_ObjStatus &= ~GOS_DURATION;
		break;
	}

	if (!DurationCursorModify())
		OnetimeCursorModify();
}
CWnd* CGrpObject::GetGraphWnd()
{
	return m_pGraph;
}

bool CGrpObject::IsPointInRect(CPoint pt)
{
	if (m_ObjRegion.PtInRect(pt))
		return true;
	return false;
}

void CGrpObject::LButtonDownAction(CPoint pt)
{
	CRgnSplit	*pRgnSplit;
	if ((pRgnSplit = IsSplitRgn(pt)))
	{
		m_ObjStatus |= GOS_SPLITMOVE;
		CRect	Rc = pRgnSplit->GetSplitMovableRgn();

		if (m_ObjStatus & GOS_SPLITOVER)
			m_ObjStatus &= ~GOS_SPLITOVER;
		SplitCursor(&Rc);
		pRgnSplit->BeginDragRgn(pt);
		m_DragRgnSplit = pRgnSplit;
		return;
	}

	
	pointWHO pointOWNER = SearchPointInRegion(pt);
	m_mouseOWNER = pointOWNER;

	CToolBase* pToolBase = (CToolBase *)0;
	CGrpBase *pGrpBase;
	CPoint	AdjustPt;

	bool	bInputchg = false;
	switch (pointOWNER)
	{
	case pointGRAPH:
		if (!SearchPointInTool(pt))
			m_ObjStatus &= ~GOS_MODIFY;

		if (m_ObjStatus & GOS_AUTOSCROLL)
			return;

		pGrpBase = SearchPointInGraph(pt);
		if (!pGrpBase)
			return;
		switch (m_ObjStatus & GOS_MODIFY)
		{
		case GOS_TCREATE:
			pToolBase = AddToolQue(m_tkCHOICED, pGrpBase);
			if (pToolBase == (CToolBase *) 0)
				return;
			if (pToolBase->m_tkNAME == tkCROSS)
				getCrossValue(pToolBase, pt);
			pToolBase->CreateToolLBDN(pt);
			if (m_tkCHOICED == tkUSRINPUT)
				bInputchg = true;
			else
				m_ObjStatus |= GOS_TCREATE;

			m_linePoint = pt;
			break;
		case GOS_TCHANGE:
			if (m_tkSELECT < 0)
				break;
			if (m_ToolQue.GetSize() > 0)
				pToolBase = m_ToolQue.GetAt(m_tkSELECT);
			if (!pToolBase)
				return;
			if (!pToolBase->IsVisibleHolder())
			{
				m_ObjStatus &= ~GOS_TCHANGE;
				return;
			}
			
			m_bToolChange = true;
			pToolBase->ChangeToolLBDN(pt);
			if (pToolBase->GetToolName() == tkUSRINPUT)
				bInputchg = true;
			break;
		case GOS_TMOVE:
			if (m_tkSELECT < 0)
				break;
			if (m_ToolQue.GetSize() > 0)
				pToolBase = m_ToolQue.GetAt(m_tkSELECT);
			if (pToolBase)
			{
				m_bToolChange = true;
				if (pToolBase->m_tkNAME == tkCROSS)
					getCrossValue(pToolBase, pt);
				pToolBase->MoveToolLBDN(pt);
			}
			break;
		default:
			pGrpBase = IsPtOverLine(pt, true);
			if (pGrpBase)
			{
				if (pGrpBase->GetDrawRect().Height() > 10)
				{
					if (m_pSelLineGrp 
						&& m_pSelLineGrp != pGrpBase)
					{
						m_pSelLineGrp->ResetSelLine();
						ReDrawRegion(m_pSelLineGrp->GetDrawRect());
					}

					m_pSelLineGrp = pGrpBase;
					ReDrawRegion(m_pSelLineGrp->GetDrawRect());
					break;
				}
			}

			if (m_pCrossLine == NULL)
				break;
			m_tkSAVE = -1;

			if (m_bUsableGrid && IsDataVisible())
			{
				if (m_pMgr->m_bTotal)
				{
					BOOL ret = (BOOL)m_pMgr->m_pView->SendMessage(GEV_TOOL, 
						MAKEWPARAM(toolStatus, 0), 0);
					if (ret == TRUE)
						m_bDataVisible = false;
					else 
						m_bDataVisible = true;
					SetDataVisible();		
				}
				if (m_pMgr->m_bTotal)
				{
					switch (m_pMgr->m_grxType)
					{
					case DEF_TURN:
					case DEF_PNF:
					case DEF_CLOCK:
					case DEF_PV:
					case DEF_MKPRO:
					case DEF_SALE:
						break;
					default:
						AdjustPt = SetDisplayPosData(pt, GOS_MSLBTNDN);
						break;
					}
				}
				else
					AdjustPt = SetDisplayPosData(pt, GOS_MSLBTNDN);
			}
			else
			{
				if (m_pMgr->m_bTotal)
				{
					BOOL ret = (BOOL)m_pMgr->m_pView->SendMessage(GEV_TOOL, 
						MAKEWPARAM(toolStatus, 0), 0);
					if (ret == TRUE)
						m_bDataVisible = false;
					else 
						m_bDataVisible = true;
					SetDataVisible();		
				}
				if (m_bTickChart && !m_bEditable)
				{
					mouseResizeArea(pt);
					toolHideHolder();
					return;
				}
				if (m_bEditable)
				{
					if (m_pMgr->m_bTotal)
					{
						switch (m_pMgr->m_grxType)
						{
						case DEF_BALANCE:
						case DEF_TURN:
						case DEF_PNF:
						case DEF_CLOCK:
						case DEF_PV:
						case DEF_MKPRO:
						case DEF_SALE:
							break;
						default:
							mouseResizeArea(pt);
							toolHideHolder();
							return;
						}
					}
					else
					{
						mouseResizeArea(pt);
						toolHideHolder();
						return;
					}
				}	
			}
			
			break;
		}
		break;
	
	default:
		break;
	}
	for (int ii = m_ToolQue.GetUpperBound(); ii >= 0; ii--)
	{
		if (ii == m_tkSELECT)
			continue;
		pToolBase = m_ToolQue.GetAt(ii);
		pToolBase->HideHolder();
	}
	if (!bInputchg)
		m_ObjStatus |= GOS_MSLBTNDN;
}

void CGrpObject::LButtonUpAction(CPoint pt)
{
	if (m_SSizePt.x >= 0)
	{
		mouseResizeArea(pt, true);
		m_SSizePt = CPoint(-1, -1);
		return;
	}
	m_pGrid->HideGrid();
	if (m_bRefresh)
	{
		ReDrawRTMRegion();
		m_bRefresh = false;
	}

	if (m_bLineSelect)
	{
		m_bLineSelect = false;
		SplitCursor(NULL, false, true);
		m_ObjStatus &= ~GOS_MSLBTNDN;

		copyGraphChart(pt);

		CGrpBase* pGrpBase = IsPtOverLine(pt, false);
		if (pGrpBase)
		{
			if (pGrpBase->GetDrawRect().Height() > 10)
			{
				m_ObjLine |= GOL_LINEOVER;
				SplitCursor(NULL, true);
			}
		}
		return;
	}
	m_bLineSelect = false;

	if (m_ObjStatus & GOS_SPLITMOVE)
	{
		m_ObjStatus &= ~GOS_SPLITMOVE;
		SplitCursor();
		if (m_ObjStatus & GOS_MODIFY)
		{
			if (!DurationCursorModify())
				OnetimeCursorModify();
		}
		m_DragRgnSplit->DropRgn(pt);
		m_DragRgnSplit = NULL;
		return;
	}

	pointWHO pointOWNER = SearchPointInRegion(pt);
	if (m_mouseOWNER != pointOWNER)
		pointOWNER = m_mouseOWNER;

	m_mouseOWNER = pointNONE;

	CToolBase* pToolBase = (CToolBase *)0;
	int	idx;
	switch (pointOWNER)
	{
	case pointGRAPH:
		if (m_ObjStatus & GOS_AUTOSCROLL)
			return;
		switch (m_ObjStatus & GOS_MODIFY)
		{
		case GOS_TCREATE:
			idx = m_ToolQue.GetUpperBound();
			if (idx < 0)
				break;
			pToolBase = m_ToolQue.GetAt(idx);
			if (pToolBase == (CToolBase *) 0) 
				break;

			if (pToolBase->m_tkNAME == tkTENDENCY && m_linePoint == pt)
			{
				if (pToolBase)
				{
					delete pToolBase;
					pToolBase = NULL;
					m_ToolQue.RemoveAt(idx);
				}
				m_tkSELECT = -1;
				ResetCursorModify();
				if (!(m_tkSELECT == tkTENDENCY) && !(m_ObjStatus & GOS_TENDENCY))
					m_ObjStatus &= ~GOS_TCREATE;
				m_ObjStatus = GOS_CROSSLINE;
				return;
			}
			pToolBase->CreateToolLBUP(pt);
			if (!(m_tkSELECT == tkTENDENCY) && !(m_ObjStatus & GOS_TENDENCY))
				m_ObjStatus &= ~GOS_TCREATE;
			m_tkSAVE = m_tkSELECT = m_ToolQue.GetUpperBound();
			break;
		case GOS_TCHANGE:
			if (m_tkSELECT < 0)
				break;
			pToolBase = m_ToolQue.GetAt(m_tkSELECT);
			pToolBase->ChangeToolLBUP(pt);
			m_ObjStatus &= ~GOS_TCHANGE;
//			ReDrawGraphObject();
			m_bToolChange = false;
			break;
		case GOS_TMOVE:
			if (m_tkSELECT < 0)
				break;
			if (m_ToolQue.GetSize() > 0)
				pToolBase = m_ToolQue.GetAt(m_tkSELECT);
			if (pToolBase)
				pToolBase->MoveToolLBUP(pt);
			m_ObjStatus &= ~GOS_TMOVE;
//			ReDrawGraphObject();
			m_bToolChange = false;
			break;
		default:
			if (m_pCrossLine == NULL)
				break;

			if (m_bJTrace)
			{
				CPoint  AdjustPt;
				AdjustPt = GetAdjustPoint(pt);
				if (m_ObjStatus & GOS_CROSSLINE)
				{
					m_pCrossLine->LButtonUpAction(AdjustPt);
				}
			}
			else
			{
				if (m_ObjStatus & GOS_CROSSLINE)
					m_pCrossLine->LButtonUpAction(pt);
			}
			break;
		}
		break;
	
	default:
		break;
	}

	if (!(m_tkSELECT == tkTENDENCY) && !(m_ObjStatus & GOS_TENDENCY))
		m_tkSELECT = -1;
	m_ObjStatus &= ~GOS_MSLBTNDN;

	if ((pointOWNER == pointGRAPH) && (m_ObjStatus & GOS_CAPTURE))
	{
		if (m_ObjStatus & GOS_DURATION)
			DurationCursorModify();
		else
			OnetimeCursorModify(true);
	}
}

void CGrpObject::MouseMoveAction(CPoint pt)
{
	if (m_SSizePt.x >= 0)
	{
		mouseResizeArea(pt);
		return;
	}

	if (m_bLineSelect)
	{
		if (m_ObjStatus & GOS_MSLBTNDN)
		{
			CRect	rc = m_GrpRegion;
			SplitCursor(&rc, false, true); 
			m_ObjStatus &=  ~GOS_MSLBTNDN;
		}
		return;
	}

	if (m_ObjStatus & GOS_SPLITMOVE)
	{
		if (m_DragRgnSplit)
			m_DragRgnSplit->DragRgn(pt);
		return;
	}

	if (!(m_ObjStatus & GOS_MSLBTNDN))
	{
		if (IsSplitRgn(pt))
		{
			if (m_ObjStatus & GOS_SPLITOVER)
				return;

			m_ObjStatus |= GOS_SPLITOVER;
			SplitCursor();
			return;
		}
		else if (m_ObjStatus & GOS_SPLITOVER)
		{
			m_ObjStatus &= ~GOS_SPLITOVER;
			SplitCursor();

			if (m_ObjStatus & GOS_MODIFY)
			{
				if (!DurationCursorModify())
					OnetimeCursorModify();
			}
		}

		if (!(m_ObjStatus & GOS_MODIFY))
		{
			CGrpBase* pGrpBase = IsPtOverLine(pt, false);
			if (pGrpBase)
			{
				if (pGrpBase->GetDrawRect().Height() > 10)
				{
					if (m_ObjLine & GOL_LINEOVER)
						return;

					m_ObjLine |= GOL_LINEOVER;
					SplitCursor(NULL, true);
					return;
				}
			}
			else if (m_ObjLine & GOL_LINEOVER)
				SplitCursor();
		}

		if (m_ObjStatus & GOS_MSRBTNDN)
			return;

		return;
	}

	if (m_ObjStatus & GOS_AUTOSCROLL)
		return;

	CToolBase* pToolBase = (CToolBase *)0;
	pointWHO pointOWNER = SearchPointInRegion(pt);
	int idx;
	CPoint	AdjustPt;

	switch (pointOWNER)
	{
	case pointGRAPH:
		switch (m_ObjStatus & GOS_MODIFY)
		{
		case GOS_TCREATE:
			idx = m_ToolQue.GetUpperBound();
			if (idx < 0)
				break;
			pToolBase = m_ToolQue.GetAt(idx);
			if (pToolBase == (CToolBase *) 0)
				break;
			if (pToolBase->m_tkNAME == tkCROSS)
				getCrossValue(pToolBase, pt);
			pToolBase->CreateToolLBMV(pt);
			break;
		case GOS_TCHANGE:
			if (m_tkSELECT < 0)
				break;
			pToolBase = m_ToolQue.GetAt(m_tkSELECT);
			pToolBase->ChangeToolLBMV(pt);
			break;
		case GOS_TMOVE:
			if (m_tkSELECT < 0)
				break;
			if (m_ToolQue.GetSize() > 0)
				pToolBase = m_ToolQue.GetAt(m_tkSELECT);
			if (pToolBase)
			{
				if (pToolBase->m_tkNAME == tkCROSS)
					getCrossValue(pToolBase, pt);
				pToolBase->MoveToolLBMV(pt);
			}
			break;
		default:
			if (m_pCrossLine == NULL)
				break;
			if (IsDataVisible())
			{
				if (m_pMgr->m_bTotal)
				{
					switch (m_pMgr->m_grxType)
					{
					case DEF_TURN:
					case DEF_PNF:
					case DEF_CLOCK:
					case DEF_PV:
					case DEF_MKPRO:
					case DEF_SALE:
						break;
					default:
						AdjustPt = SetDisplayPosData(pt, GOS_MSMOVE);
						break;
					}

				}
				else
					AdjustPt = SetDisplayPosData(pt, GOS_MSMOVE);
			}
			else if (m_pCrossLine && m_ObjStatus & GOS_CROSSLINE)
			{
				if (m_bJTrace)
					AdjustPt = GetAdjustPoint(pt);
				else
				{
					if (m_pMgr->m_bTotal)
					{
						switch (m_pMgr->m_grxType)
						{
						case DEF_TURN:
						case DEF_PNF:
						case DEF_CLOCK:
						case DEF_PV:
						case DEF_MKPRO:
						case DEF_SALE:
						case DEF_BALANCE:
							break;
						default:
							AdjustPt = pt;
							break;
						}
					}
					else
						AdjustPt = pt;

				}
				m_pCrossLine->LButtonMoveAction(AdjustPt);
			}
			m_lastPt = AdjustPt;
			break;
		}
		break;
	default:
		if (m_pCrossLine && m_ObjStatus & GOS_CROSSLINE)
		{
			m_pGrid->HideGrid();
			m_pCrossLine->LButtonUpAction(pt);
		}
		break;
	}
}

void CGrpObject::RButtonDownAction(CPoint pt)
{
	if (m_ObjStatus & GOS_AUTOSCROLL)
		return;
	pointWHO pointOWNER = SearchPointInRegion(pt);
	if (m_mouseOWNER != pointNONE && m_mouseOWNER != pointOWNER)
		pointOWNER = m_mouseOWNER;

	m_mouseOWNER = pointOWNER;
	switch (pointOWNER)
	{
	case pointGRAPH:
		if (ResetCursorModify())
			return;
		break;
	default:
		if (ResetCursorModify())
			return;
		return;
	}
	m_ObjStatus |= GOS_MSRBTNDN;
}

void CGrpObject::RButtonUpAction(CPoint pt)
{
	if (m_ObjStatus & GOS_AUTOSCROLL)
		return;
	
	int RgnKey;
	int aType, aKind, menuID;
	CGrpBase* pGrpBase;
	CToolBase *pToolBase, *pToolBaseX;

	pointWHO pointOWNER = SearchPointInRegion(pt);
	if (m_mouseOWNER != pointNONE && m_mouseOWNER != pointOWNER)
		pointOWNER = m_mouseOWNER;

	switch (pointOWNER)
	{
	case pointGRAPH:
		CheckGraphTypeAndKind(pt, RgnKey);
		
		if (m_ObjStatus & GOS_TCREATE)
		{
			m_ObjStatus = GOS_CROSSLINE;
			return;
		}

		pGrpBase = SearchPointInGraph(pt);
		if (pGrpBase == (CGrpBase *) 0)
			return;

		if (m_tkSAVE >= 0 && m_ToolQue.GetSize() > 0 
			&& (pToolBase = m_ToolQue.GetAt(m_tkSAVE)) != NULL
			&& pToolBase->IsVisibleHolder())
		{
			if (pToolBase->IsVisibleHolder())
			{
				menuID = DisplayModifyToolMenu();
				switch (menuID)
				{
				case 0:
					return;
				case modifyMENUID:		// 복사
					pToolBaseX = new CTendency((CTendency *)pToolBase);
					m_ToolQue.Add(pToolBaseX);
					pToolBaseX->ChangeMemberValue();
					break;
				case modifyMENUID+1:		// 삭제
					delete pToolBase;
					pToolBase = NULL;

					m_ToolQue.RemoveAt(m_tkSAVE);
					m_tkSAVE = -1;
					ReDrawGraphObject();
					break;
				case modifyMENUID+2:		// 전체 삭제
					RemoveToolQue();
					m_tkSAVE = -1;
					ReDrawGraphObject();
					break;
				}
			}
			break;
		}

		if (m_pMgr->m_pCtrl == NULL && !m_bEditable) // not edit
			return;

		menuID = DisplayAddChartMenu(pGrpBase, aType, aKind, pt);
		switch (menuID)
		{
		case 0:					// 선택 에러
			return;
		case chartBASE:				// 추가 모드
			SetAppendMode(true);
			return;
		case chartBASE+1:			// 겹침 모드
			SetAppendMode(false);
			return;
		case chartBASE+2:			// 대칭모드
			SetReverseMode();
			return;
		case chartBASE+3:			// 입체형
			SetGradient();
			return;
		case chartBASE+5:			// 차트설정
			ChartConfig(pGrpBase);
			return;
		
		case chartBASE+6:			// 중복차트
			{
				m_bAppendMode = false;
				graphDup(pGrpBase, pt);
			}
			return;
		case chartBASE+7:			// 챠트 삭제
			DeleteGraphChart(pGrpBase);
			return;
		case chartBASE+8:			// 색상변경
			ModifyColor(pGrpBase);
			return;
		case chartBASE+15:			// 수치조회
			SetDataVisible();
			return;
		case chartBASE+16:			// line type
			SetLineType();
			return;
		case chartBASE+17:			// 십자선 숨기기
			SetCrossView();
			return;
		case chartBASE+18:			// 십자선 종가따라가기
			SetJTrace();
			return;
		case chartBASE+20:			// PNF
			SetUserDefined(pGrpBase);
			return;
		default:
			break;
		}
		break;
	default:
		break;
	}

	m_mouseOWNER = pointNONE;
	m_ObjStatus &= ~GOS_MSRBTNDN;
}

void CGrpObject::SetUserDefined(CGrpBase *pGrpBase)
{
	int	pValue[5]; 
	int nValue = pGrpBase->GetUsedValue(pValue);
	if (!nValue)
		return;
	if (axUserDefined(pValue, nValue))
	{
		pGrpBase->SetUserDefined(pValue, nValue);
		ReDrawGraphObject();
	}
}

BOOL CGrpObject::WheelAction(CPoint pt, bool bLeft)
{
	return FALSE;
}

void CGrpObject::OnKey(int key)
{
}

CPoint CGrpObject::SetDisplayPosData(CPoint pt, int mEvent)
{
	CPoint  AdjustPt(-1,-1);
	if (m_pCrossLine == NULL)
		return pt;

	CGrpBase* pGrpBase;
	CString dispString = _T(""), unitString = _T(""), valueString = _T("");
	CString	posValue;
	if (m_bJTrace)
	{
		for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
		{
			pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
			if (ii == 0 && !ExistBongTypeGraph())
				unitString = pGrpBase->GetDisplayPosData(pt, true);
			else
				unitString = pGrpBase->GetDisplayPosData(pt);
			if (!unitString.IsEmpty())
			{
				dispString += unitString;
				dispString += " ";
			}

			if (pGrpBase->GetGraphType() == GT_BONG && ii == 0)
			{
				if (m_ObjStatus & GOS_CROSSLINE)
				{
					AdjustPt = pGrpBase->GetAdjustPoint(pt);
					if (AdjustPt.y > 0)
						AdjustPt.x = pt.x;
				}
			}

			if (pGrpBase->GetGraphKind() == GK_BALANCE)
				return AdjustPt;
		}

		if (AdjustPt.x == -1)
			AdjustPt = pt;
	}
	else
	{
		for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
		{
			pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
			if (ii == 0 && !ExistBongTypeGraph())
				unitString = pGrpBase->GetDisplayPosData(pt, true);
			else
				unitString = pGrpBase->GetDisplayPosData(pt);
			if (!unitString.IsEmpty())
			{
				dispString += unitString;
				dispString += " ";
			}

			if (pGrpBase->GetGraphKind() == GK_BALANCE)
				return AdjustPt;

			if (pGrpBase->GetGraphType() == GT_BONG && ii == 0 && !(m_ObjEnv & ENV_LOG))
				posValue = pGrpBase->GetYposValue(pt);
		}
		AdjustPt = pt;
	}

	if (mEvent != 0 && !m_GrpRegion.IsRectEmpty())
	{
		CString	sData = dispString + posValue;
		if (!m_pGrid->IsWindowVisible() || sData.IsEmpty())
			m_pGrid->SetData(_T(""));

		m_pGrid->ShowData(sData, mEvent, &AdjustPt, (m_ObjStatus & GOS_CROSSLINE)? true:false);
	}

	return AdjustPt;
}

CPoint CGrpObject::GetAdjustPoint(CPoint pt)
{
	CPoint	AdjustPt(-1,-1);

	return AdjustPt;
	CGrpBase* pGrpBase;
	for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		if (pGrpBase->GetGraphType() == GT_BONG && ii == 0)
		{
			if (m_ObjStatus & GOS_CROSSLINE)
			{
				AdjustPt = pGrpBase->GetAdjustPoint(pt);
				if (AdjustPt.y > 0)
				{
					AdjustPt.x = pt.x;
					break;
				}
			}
		}
	}
	if (AdjustPt.y == -1)
		AdjustPt = pt;

	return AdjustPt;
}

//	<GI_HMS>
// mthLOCAL		: update        [epr, trn          ] at last index
// mthGLOBAL	: update        [epr, trn, hpr/lpr ] at last index
// mthSHIFT		: insert(update)[epr, trn, hpr, lpr] at index of zero data
//				: insert(add)   [epr, trn, hpr, lpr] at last index, remove first index
//	<GI_MON, GI_WEEK, GI_DAY>
// mthLOCAL		: update		[epr, trn          ] at last index
// mthGLOBAL	: update		[epr, trn, hpr/lpr ] at last index

struct	_rtmEx {
	int	count;
	struct _info {
		CGrpBase	*pGrpBase;
		CString	Symbol;
		CString	val;
	}arg[maxGRAPH];
};

void CGrpObject::RealTimeExMessage(CString rtm)
{
	if (!m_bUpdateREAL || m_DevOption & DF_NRTM)
		return;
	
	bool bChangeCurr = false;
	struct _rtmEx rtmEx;
	rtmEx.count = 0;
	if (m_bECNData)
	{
		if (!ParseRealtimeECN(rtm, &rtmEx))
			return;
	}
	else if (!ParseRealtimeEx(rtm, &rtmEx))
		return;
	
	CString currRTM = m_currRTM;
	double	dVal;
	switch(m_ObjUnit)
	{
	case GU_INDEX:
	case GU_KOSPI:
	case GU_FUTURE:
	case GU_KQFUT:
	case GU_OPTION:
	case GU_KQOPT:
	default:
		break;
	}
	int	dKey;
	CGrpData	*pGrpData = NULL;
	CGrpBase	*pGrpBase = NULL;

	_mthDRAW	response;
	_mthDRAW	mthDRAW = mthNONE;
	bool	doCommon = false;
	for (int ii = 0; ii < rtmEx.count; ii++)
	{
		pGrpBase = rtmEx.arg[ii].pGrpBase;
		dKey = pGrpBase->GetDataKey();
		if (!GetGrpData(dKey, pGrpData))
			return;
		
		if (pGrpData->m_curr != atof(currRTM))
		{
			pGrpData->m_curr = atof(currRTM);
			bChangeCurr = true;
		}

		if (m_bTickChart)
		{
			if (pGrpBase->GetGraphKind() == GK_MGJY)
			{
				if (m_mgjySave == m_mgjyRTM)
					continue;
				m_mgjySave = m_mgjyRTM;
				currRTM = m_mgjyRTM;
			}
			response = _mthDRAW(pGrpData->UpdateTickRTM(m_timeRTM, currRTM, m_gvolRTM, 
				pGrpBase->GetGrpUnit(), m_bEveryUpdate));
			if (mthDRAW < response)
				mthDRAW = response;
		}
		else
		{
			switch (pGrpData->GetDataKind())
			{
			case DK_BONG:
			case DK_VOL:
			case DK_AMT:
				if (doCommon)
					continue;
				else
					doCommon = true;

				response = _mthDRAW(pGrpData->UpdateCommonRTM(m_timeRTM, currRTM, m_gvolRTM, 
					m_gamtRTM, m_bEveryUpdate, pGrpBase->GetGrpUnit(), 
					(m_DevOption & DF_INSRTM)? true:false));
				if (mthDRAW < response)
					mthDRAW = response;
				break;
			case DK_VIDX:
				if (rtmEx.arg[ii].pGrpBase->m_digit > 0)
				{
					dVal = atof(rtmEx.arg[ii].val);
					rtmEx.arg[ii].val.Format("%d", 
						int(dVal * pow (10.0, rtmEx.arg[ii].pGrpBase->m_digit)));
				}

				response = _mthDRAW(pGrpData->UpdateVidxRTM(m_timeRTM, rtmEx.arg[ii].val, 
					m_bEveryUpdate, (m_DevOption & DF_INSRTM)? true:false));
				if (mthDRAW < response)
					mthDRAW = response;
				break;
			default:
				return;
			}
		}
	}

	if (m_pGrid->IsWindowVisible() || m_bToolChange)
	{
		for (ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
		{
			pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
			pGrpBase->IsChangeMinMaxRTM();
		}
		m_bRefresh = true;
		return;
	}

	if (pGrpBase == NULL)
		return;

	int dispDay  = pGrpBase->GetDisplayDay();
	int totalDay = pGrpBase->GetTotalDataCount();
	int dispEnd  = pGrpBase->GetDisplayEnd();

	if (totalDay < dispEnd)
		return;

	CRect	rtmRC  = pGrpBase->GetDrawRect();
	rtmRC.top = m_GrpRegion.top;
	rtmRC.bottom = m_GrpRegion.bottom;

	double szOneDay = double(rtmRC.Width()) / double(dispDay);
	if (int(szOneDay) < 1)	szOneDay = 1;

	switch (mthDRAW)
	{
	case mthLOCAL:	// 1	-> min or max is not changed, but check aid chart
	case mthGLOBAL:	// 2	-> if min or max is changed, draw all region
		for (ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
		{
			pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
			if (pGrpBase->IsChangeMinMaxRTM())
			{
				if (!m_bDrawTool)
					ReDrawRTMRegion();
				return;
			}
		}
		if (!m_bDrawTool)
			ReDrawRTMRegion();

		if (bChangeCurr && !m_bDrawTool)
			ReDrawRTMRegion();

		break;
	case mthSHIFT:	// 3	->redraw all
		for (ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
		{
			pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
			pGrpBase->IsChangeMinMaxRTM();
		}
		if (!m_bDrawTool)
		{
			ReDrawRTMRegion();

			CToolBase	*pToolBase;
			for (int jj = m_ToolQue.GetUpperBound(); jj >= 0; jj--)
			{
				pToolBase = m_ToolQue.GetAt(jj);
				pToolBase->IndexShift();
			}
		}
		break;
	default:
		return;
	}

	return;
}


bool CGrpObject::ParseRealtimeEx(CString rtm, struct _rtmEx *rtmEx)
{
	CGrpBase	*pGrpBase;
	CString	symbol;
	bool	bVol = false;
	bool	bCur = false;
	bool	bMgjy = false;
	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		if (!pGrpBase)
			return false;

		switch (pGrpBase->GetGraphType())
		{
		case GT_BONG:
			switch (pGrpBase->GetGraphKind())
			{
			case GK_JPN:
			case GK_LOG:
			case GK_USA:
			case GK_LIN:
			case GK_ENVL:
			case GK_PARB:
			case GK_BOLB:
			case GK_BALANCE:
			case GK_NET:
			case GK_AD_ENVL:
			case GK_AD_BOLB:
			case GK_AD_PARB:
			case GK_AD_NET:
			case GK_TICK:
			case GK_STEP:
			case GK_EQUI:
				if (!IsAlreadyGet(R_CURR, rtmEx))
				{
					rtmEx->arg[rtmEx->count].Symbol = R_CURR;
					rtmEx->arg[rtmEx->count++].pGrpBase = pGrpBase;
				}

				bCur = true;
				break;
			case GK_MGJY:
				if (!IsAlreadyGet(R_MGJY, rtmEx))
				{
					rtmEx->arg[rtmEx->count].Symbol = R_MGJY;
					rtmEx->arg[rtmEx->count++].pGrpBase = pGrpBase;
				}
				bMgjy = true;
				break;
			case GK_VOL:
			case GK_EVOL:
				if (m_ObjIndex == GI_HMS)
					symbol = R_CVOL;	// 체결량
				else
					symbol = R_GVOL;	// 누적거래량
				if (!IsAlreadyGet(symbol, rtmEx))
				{
					rtmEx->arg[rtmEx->count].Symbol = symbol;
					rtmEx->arg[rtmEx->count++].pGrpBase = pGrpBase;
				}

				bVol = true;
				break;
			case GK_LG_PMA:
			case GK_LG_LMA:
			case GK_LG_VMA:
			case GK_LG_AMA:
			case GK_LG_LIN:
			case GK_LG_LOG:
			case GK_LG_ENVL:
			case GK_LG_PARB:
			case GK_LG_NET:
			case GK_LG_BOLB:
				continue;
			
			
			default:
				return false;
			}
			break;
		case GT_LINE:
			switch (pGrpBase->GetGraphKind())
			{
			case GK_ADLINE:
			case GK_OBV:
			case GK_VR:
			case GK_PV:
			case GK_VMAO:
			case GK_CLOCK:
				if (m_ObjIndex == GI_HMS)
					symbol = R_CVOL;
				else
					symbol = R_GVOL;
				if (!IsAlreadyGet(symbol, rtmEx))
				{
					rtmEx->arg[rtmEx->count].Symbol = symbol;
					rtmEx->arg[rtmEx->count++].pGrpBase = pGrpBase;
				}

				bVol = true;
			default:
				if (!IsAlreadyGet(R_CURR, rtmEx))
				{
					rtmEx->arg[rtmEx->count].Symbol = R_CURR;
					rtmEx->arg[rtmEx->count++].pGrpBase = pGrpBase;
				}

				bCur = true;
				break;
			case GK_RRATE:
				return false;
			}
			break;
		case GT_USER:
			switch (pGrpBase->GetGraphKind())
			{
			case GK_LINE_VIDX:	// vidx
			case GK_STEP_VIDX:	// vidx
			case GK_XVBAR_VIDX:	// vidx
			case GK_VBAR_VIDX:	// vidx
			case GK_VBAR_REV:	// vidx
				if (pGrpBase->m_valueAV[0] <= 0)
					return false;

				if (pGrpBase->m_valueAV[0] > 0)
				{
					symbol.Format("%03d", pGrpBase->m_valueAV[0]);
					if (!IsAlreadyGet(symbol, rtmEx))
					{
						rtmEx->arg[rtmEx->count].Symbol = symbol;
						rtmEx->arg[rtmEx->count++].pGrpBase = pGrpBase;
					}
				}
				else
				{
					return false;
				}
				break;
			default:
				return false;
			}
			break;
		case GT_VARS:
			switch (pGrpBase->GetGraphKind())
			{
			case GK_PNF:
			case GK_TURN:
				if (!IsAlreadyGet(R_CURR, rtmEx))
				{
					rtmEx->arg[rtmEx->count].Symbol = R_CURR;
					rtmEx->arg[rtmEx->count++].pGrpBase = pGrpBase;
				}

				bCur = true;
				break;
			}
			break;
		default:
			return false;
		}
	}

	// search (symbol,val)
	CMapStringToString	rtmStore;
	CString	val;
	int	index;
	for (; !rtm.IsEmpty(); )
	{
		index = rtm.Find('\t');
		if (index < 0)
			break;
		symbol = rtm.Left(index++);
		rtm = rtm.Mid(index);

		index = rtm.Find('\t');
		if (index < 0)
			index = rtm.GetLength();
		val = rtm.Left(index++);

		rtmStore.SetAt(symbol, val);

		if (rtm.GetLength() <= index)
			break;
		rtm = rtm.Mid(index);
	}

	POSITION	pos;
	CString	CTime = _T("");
	CString	HTime = _T("");
	m_mgjyRTM = m_currRTM = m_gvolRTM = _T("");
	CString mgjyS = _T("");

	switch (m_ObjUnit)
	{
	case GU_INDEX:	case GU_KOSPI:	case GU_FUTURE:
	case GU_KQFUT:	case GU_OPTION:	case GU_KQOPT:
		for (pos = rtmStore.GetStartPosition(); pos;)
		{
			rtmStore.GetNextAssoc(pos, symbol, val);
			if (R_CTIM == symbol)
				CTime = val;
			else if (R_HTIM == symbol)
				HTime = val;
			else if (R_CURR == symbol)
			{
				double dVal = atof(val) * 100.0;
				if (dVal < 0)
					dVal *= (-1);
				m_currRTM.Format("%.f", dVal);
			}
			else if (R_CVOL == symbol && m_ObjIndex == GI_HMS)
				m_gvolRTM.Format("%d", abs(atoi(val)));
			else if (R_GVOL == symbol && m_ObjIndex != GI_HMS)
				m_gvolRTM = val;
			else if (R_MGJY == symbol)
				m_mgjyRTM.Format("%.f", fabs(atof(val)));
		}
		break;
	default:
		for (pos = rtmStore.GetStartPosition(); pos;)
		{
			rtmStore.GetNextAssoc(pos, symbol, val);
			if (R_CTIM == symbol)
				CTime = val;
			else if (R_HTIM == symbol)
				HTime = val;
			else if (R_CURR == symbol)
				m_currRTM.Format("%d", abs(atoi(val)));
			else if (R_CVOL == symbol && m_ObjIndex == GI_HMS)
				m_gvolRTM.Format("%d", abs(atoi(val)));
			else if (R_GVOL == symbol && m_ObjIndex != GI_HMS)
				m_gvolRTM = val;
			else if (R_MGJY == symbol)
				m_mgjyRTM.Format("%.f", fabs(atof(val)));
		}
		break;
	}

	if (CTime.IsEmpty() && HTime.IsEmpty())
		return false;

	if (bVol && m_gvolRTM.IsEmpty())
		return false;
	if (bCur && m_currRTM.IsEmpty())
		return false;
	if (bMgjy && m_mgjyRTM.IsEmpty())
		return false;

	int	num;
	bool	bHtime = false;	//호가 시간

	for (ii = 0; ii < rtmEx->count; ii++)
	{
		num = atoi(rtmEx->arg[ii].Symbol);
		if ( num >= 41 && num <= 110)
			bHtime = true;

		for (pos = rtmStore.GetStartPosition(); pos;)
		{
			rtmStore.GetNextAssoc(pos, symbol, val);
			if (rtmEx->arg[ii].Symbol == symbol)
			{
				rtmEx->arg[ii].val = val;
				pos = (struct __POSITION *)1;
				break;
			}
		}
		if (pos == NULL)
			return false;
	}

	if (bHtime)
		m_timeRTM = HTime;
	else
		m_timeRTM = CTime;

	rtmStore.RemoveAll();
	return ((m_timeRTM.GetLength() >= 6)? true:false);
}

bool CGrpObject::IsAlreadyGet(CString symbol, struct _rtmEx *rtmEx)
{
	for (int ii = 0; ii < rtmEx->count; ii++)
	{
		if (rtmEx->arg[ii].Symbol == symbol)
			return true;
	}

	return false;
}

CSize CGrpObject::GetSizeAdjust()
{
	return CSize(0, 0);
}

// private member
bool CGrpObject::IsValidGraphHeader(int key, int region)
{
	CGrpData* pGrpData;
	if (!m_MapData.Lookup(key, pGrpData))
		return false;

	CRect	rgnRect;
	struct _xRegion* xRegion;
	if (!m_MapRegion.Lookup(region, xRegion))
		return false;

	return true;
}

void CGrpObject::CopyAndSpaceFilter(char* des, char* src, char srcL, bool bFilter)
{
	CopyMemory(des, src, srcL);
	des[srcL] = '\0';
	if (!bFilter)
		return;

	for (int ii = srcL -1; ii >= 0; ii--)
	{
		if (des[ii] > ' ')
			break;
		des[ii] = '\0';
	}
	return;
}

CRect CGrpObject::CalculateRect(CRect rect, float hRatio, float vRatio)
{
	CRect	calcRect;

	calcRect.left  = (int) ((float)rect.left  * hRatio);
	calcRect.right = (int) ((float)rect.right * hRatio);
	calcRect.top   = (int) ((float)rect.top   * vRatio);
	calcRect.bottom= (int) ((float)rect.bottom* vRatio);

	return calcRect;
}

void CGrpObject::AddGraphTick(char* data, int bPos)
{
	struct _tick*	pTick;

	pTick = (struct _tick *)data;
	if (pTick->region <= 0)
		return;

	m_xTick[bPos].xRect = m_ObjRect;
	switch(bPos)
	{
	case xBOTTOM:
		m_xTick[bPos].xRect.top
		= m_ObjRect.bottom - ((m_ObjRect.Height() * pTick->region) / 100);
		m_GrpRect.bottom -= m_xTick[bPos].xRect.Height();
		break;
	case xLEFT:
		m_xTick[bPos].xRect.right
		= m_ObjRect.left + ((m_ObjRect.Width() * pTick->region) / 100);
		m_GrpRect.left += m_xTick[bPos].xRect.Width();
		break;
	case xRIGHT:
		m_xTick[bPos].xRect.left
		= m_ObjRect.right - ((m_ObjRect.Width() * pTick->region) / 100);
		m_GrpRect.right -= m_xTick[bPos].xRect.Width();
		break;
	default:
		return;
	}
	m_xTick[bPos].sRect  = m_xTick[bPos].xRect;
	m_xTick[bPos].xScale = pTick->scale;
	m_xTick[bPos].xColor = pTick->color;

}

void CGrpObject::SetMagicValue(int mapKey, CGrpData* pGrpData, int gkind)
{
	int idxMagic;
	
	idxMagic = (mapKey % 100) / 10;
	if (idxMagic < 0 || idxMagic > magicMAX)
	{
		TRACE("magickey error...[%d]\n", mapKey);
		return;
	}

	struct _gVal*	gVal;
	struct _gVidx*	gVidx;
	if (!m_magicVAL[idxMagic][chkMAGIC])
	{
		gVal = (struct _gVal *) pGrpData->m_DataQue.GetAt(0);
		m_magicVAL[idxMagic][maxMAGIC] = -DBL_MAX;
		m_magicVAL[idxMagic][minMAGIC] = DBL_MAX;
	}

	bool	bFindMaxMin = true;
	if (pGrpData->m_maxBP >= pGrpData->m_minBP)
	{
		if (pGrpData->m_maxBP && pGrpData->m_maxBP > m_magicVAL[idxMagic][maxMAGIC])
			m_magicVAL[idxMagic][maxMAGIC] = pGrpData->m_maxBP;
		if (pGrpData->m_minBP && pGrpData->m_minBP < m_magicVAL[idxMagic][minMAGIC])
			m_magicVAL[idxMagic][minMAGIC] = pGrpData->m_minBP;
		bFindMaxMin = false;
	}

	if (gkind == dpVAL)
	{
		for (int ii = 0; bFindMaxMin && ii < pGrpData->GetArraySize(); ii++)
		{
			gVal = (struct _gVal *) pGrpData->m_DataQue.GetAt(ii);
			if (gVal->val && gVal->val > m_magicVAL[idxMagic][maxMAGIC])
				m_magicVAL[idxMagic][maxMAGIC] = gVal->val;
			if (gVal->val && gVal->val < m_magicVAL[idxMagic][minMAGIC])
				m_magicVAL[idxMagic][minMAGIC] = gVal->val;
		}
	}
	else if (gkind == dpVIDX)
	{
		for (int ii = 0; bFindMaxMin && ii < pGrpData->GetArraySize(); ii++)
		{
			gVidx = (struct _gVidx *) pGrpData->m_DataQue.GetAt(ii);
			if (gVidx->val && gVidx->val > m_magicVAL[idxMagic][maxMAGIC])
				m_magicVAL[idxMagic][maxMAGIC] = gVidx->val;
			if (gVidx->val && gVidx->val < m_magicVAL[idxMagic][minMAGIC])
				m_magicVAL[idxMagic][minMAGIC] = gVidx->val;
		}
	}
	m_magicVAL[idxMagic][chkMAGIC] = 1;
	if (m_magicVAL[idxMagic][minMAGIC] == DBL_MAX || 
		m_magicVAL[idxMagic][maxMAGIC] == -DBL_MAX)
		m_magicVAL[idxMagic][maxMAGIC] = m_magicVAL[idxMagic][minMAGIC] = 0;
}

pointWHO CGrpObject::SearchPointInRegion(CPoint pt)
{
	if (m_GrpRegion.PtInRect(pt))
	 	return pointGRAPH;
	else if (m_ObjRegion.PtInRect(pt))
		if (m_bMouseCaptureTCreate)
			return pointGRAPH;
		else
			return pointTICK;
	else if (m_bMouseCaptureTCreate)
		return pointGRAPH;
	return pointNONE;
}

CGrpBase* CGrpObject::SearchPointInGraph(CPoint pt)
{
	CGrpBase* pGrpBase;
	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		if (pGrpBase->IsPointInRect(pt))
			return pGrpBase;
	}
	return (CGrpBase *) 0;
}

CGrpBase* CGrpObject::IsPtOverLine(CPoint pt, bool bSet)
{
	if (m_DevOption & DF_NOHAND)
		return (CGrpBase *)0;

	bool	bSelect;
	CGrpBase *pGrpBase;
	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		if (pGrpBase->IsPointInLines(pt, bSelect, bSet))
		{
			if (pGrpBase->m_bAdd && bSet && bSelect)
				m_bLineSelect = true;
			return pGrpBase;
		}
	}

	return (CGrpBase *)0;
}

int CGrpObject::DisplayAddChartMenu(CGrpBase *pGrpBase, int& aType, int& aKind, CPoint pt)
{
	struct _menuString {
		UINT	uFlag;
		int	nType;
		int	nKind;
		char	item[32];
	} ;
	
	struct	_menuString aMenuBase[] = {
		{MFT_STRING,	0,	0,		"추가모드"	},	// +0
		{MFT_STRING,	0,	0,		"겹침모드"	},	// +1
		{MFT_STRING,	0,	0,		"대칭모드"	},	// +2
		{MFT_STRING,	0,	0,		"입체형"	},	// +3
		{MFT_SEPARATOR,	0,	0,		""		},	// +4
		{MFT_STRING,	0,	0,		"차트설정"	},	// +5
		{MFT_STRING,	0,	0,		"중복차트"	},	// +6
		{MFT_STRING,	0,	0,		"중복챠트삭제"	},	// +7
		{MFT_STRING,	0,	0,		"색상변경"	},	// +8
		{MFT_SEPARATOR,	0,	0,		""		},	// +9
		{MFT_STRING,	0,	0,		"차트설정 전송"	},	// +10
		{MFT_STRING,	0,	0,		"차트설정 받기"	},	// +11
		{MFT_STRING,	0,	0,		"차트설정 전체전송"	},	// +12
		{MFT_STRING,	0,	0,		"차트설정 전체받기"	},	// +13
		{MFT_SEPARATOR,	0,	0,		""		},	// +14
		{MFT_STRING,	0,	0,		"수치조회"},		// +15
		{MFT_STRING,	0,	0,		"라인차트"	},	// +16
		{MFT_STRING,	0,	0,		"십자선 숨기기"},	// +17
		{MFT_STRING,	0,	0,		"십자선 종가따라가기"},	// +18
		{MFT_SEPARATOR,	0,	0,		""		},	// +19
		{MFT_STRING,	0,	0,		"P&&F chart 설정"},	// +20
		
		
	};

	if (m_pMgr->m_pCtrl == NULL && !m_bEditable) // not edit
		return 0;

	CPoint	curPoint;
	GetCursorPos(&curPoint);

	int menuID = 0;
	int baseID = chartBASE;
	CMenu baseMENU;
	baseMENU.CreatePopupMenu();
	switch (pGrpBase->GetGraphKind())
	{
	case GK_PNF:
		baseMENU.AppendMenu(aMenuBase[20].uFlag, chartBASE + 20,	// 차트설정
				aMenuBase[20].item);
		menuID = baseMENU.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD,
			curPoint.x, curPoint.y, m_pGraph, NULL);
		return menuID;
		break;
	case GK_BALANCE:
	case GK_TURN:
	case GK_CLOCK:
	case GK_PV:
		return -1;
	}

	for (int ii = 0; ii < sizeof(aMenuBase) / sizeof(aMenuBase[0]); ii++)
	{
		baseID = chartBASE + ii;
		switch (baseID)
		{
		case chartBASE:			//추가모드
			continue;
			if (m_bAppendMode)
				aMenuBase[ii].uFlag |= MF_CHECKED;
			break;
		case chartBASE+1:		// 겹침모드
			continue;
			if (!m_bAppendMode)
				aMenuBase[ii].uFlag |= MF_CHECKED;
			break;
		case chartBASE+2:		// 대칭모드
			if (m_bReverseMode)
				aMenuBase[ii].uFlag |= MF_CHECKED;
			break;
		case chartBASE+3:		// Gradient
			//if (m_pMgr->m_bTotal)
			continue;
			
			if (m_bGradient)
				sprintf(aMenuBase[ii].item, "평면형 차트");
			else
				sprintf(aMenuBase[ii].item, "입체형 차트");
			break;
		case chartBASE+5:		// 차트설정
			if (m_pMgr->m_bTotal)
				continue;
			else
				break;
			break;
			
		case chartBASE+6:		// 중복차트
		case chartBASE+7:		// 중복차트삭제
			if (m_bTickChart)
				continue;
			if (m_pMgr->m_grxType == DEF_EQUIVOL)
				continue;
			switch (pGrpBase->GetGraphKind())
			{
			case GK_EVOL:
			case GK_EQUI:
				aMenuBase[ii].uFlag |= MF_DISABLED;
				aMenuBase[ii].uFlag |= MF_GRAYED;
				//continue; 
				break;
			}
			break;
		case chartBASE+8:		// 색상변경
			continue;
		case chartBASE+9:
			if (m_pMgr->m_grxType == DEF_EQUIVOL)
				continue;
			break;
		case chartBASE+10:
		case chartBASE+11:
		case chartBASE+12:
		case chartBASE+13:
		case chartBASE+14:
			continue;
		case chartBASE+15:		// 수치조회
			if (m_pMgr->m_bTotal)
				continue;

			if (IsDataVisible())
				aMenuBase[ii].uFlag |= MF_CHECKED;
			break;
		case chartBASE+16:		// line type
			if (!m_pMgr->m_bTotal)
				continue;
			if (m_ObjEnv & ENV_LINE)
			{
				if (m_bNMLine)
					aMenuBase[ii].uFlag |= MF_CHECKED;
			}
			else
				continue;
			break;
		case chartBASE+17:		// 십자선숨기기
			if (!IsCrossVisible())
				aMenuBase[ii].uFlag |= MF_CHECKED;
			break;
		case chartBASE+18:		// 종가따라가기
			if (m_bJTrace)
				aMenuBase[ii].uFlag |= MF_CHECKED;
			break;
		case chartBASE+19:
			continue;
			break;
		case chartBASE+20:
			continue;
		
		default:
			break;
		}
		baseMENU.AppendMenu(aMenuBase[ii].uFlag, baseID,
						aMenuBase[ii].item);
	}
	baseID = ii;

	menuID = baseMENU.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD,
		curPoint.x, curPoint.y, m_pGraph, NULL);

	if (menuID <= 0)
	{
		aType = aKind = 0;
		return menuID;
	}
	return menuID;
}

int CGrpObject::DisplayModifyToolMenu()
{
	struct _menuString {
		UINT	uFlag;
		char	item[32];
	} menuString [] = { 
		{MFT_STRING,		"도구복사"},
		{MFT_STRING,		"도구삭제"},
		{MFT_STRING,		"전체삭제"},
	};

	CPoint	curPoint;
	GetCursorPos(&curPoint);

	CToolBase* pToolBase;
	HMENU hMenu = CreatePopupMenu();
	for (int ii = 0; ii < sizeof(menuString) / sizeof(menuString[0]); ii++)
	{
		if (ii == 0)
		{
			pToolBase = m_ToolQue.GetAt(m_tkSAVE);
			if (pToolBase->GetToolName() != tkTENDENCY)
				continue;
		}

		AppendMenu(hMenu, menuString[ii].uFlag, modifyMENUID + ii,
					menuString[ii].item);
	}

	return TrackPopupMenu(hMenu,
		TPM_RIGHTBUTTON|TPM_RETURNCMD,
		curPoint.x, curPoint.y, 0, m_pGraph->m_hWnd, NULL);
}

bool CGrpObject::CheckGraphTypeAndKind(CPoint pt, int &OrgRgnKey)
{
	CGrpBase* pGrpBase = SearchPointInGraph(pt);
	if (pGrpBase == (CGrpBase *) 0)
		return false;

	struct	_xRegion* xRegion;
	int RgnKey = pGrpBase->GetGraphRegionID();
	if (!m_MapRegion.Lookup(RgnKey, xRegion))
	{
		TRACE("AppendGraphChar :: regionkey invalid....\n");
		return false;
	}

	OrgRgnKey = -1;
	if (xRegion->orgRgnKey != unusedKEY)
		OrgRgnKey = xRegion->orgRgnKey;	// original region key

	CGrpBase* pBase;
	int nGraphCount = m_GraphQue.GetSize();
	for (int ii = 0; ii < nGraphCount; ii++)
	{
		pBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		if (pBase->GetGraphRegionID() == RgnKey)
		{
			if (m_BongDataKey < 0)
				return false;

			switch (pBase->GetGraphType())
			{
			case GT_BONG:
				break;
			case GT_LINE:
				switch(pBase->GetGraphKind())
				{
				case GK_CLOCK:
				case GK_PV:
					return false;
				default:
					break;
				}
				break;
			case GT_USER:
			case GT_VARS:
				return false;
			default:
				break;
			}
			break;
		}
	}

	return true;
}

void CGrpObject::SyncGraphDisplay(CGrpBase* graphS, CGrpBase* graphD)
{
	int	value;
	graphD->SetDisplayDay(value = graphS->GetDisplayDay());
	graphD->SetDisplayPos(value = graphS->GetDisplayPos());
	graphD->SetDisplayEnd(value = graphS->GetDisplayEnd());
	graphD->SetDisplaySkip(value = graphS->GetDisplaySkip());
}

bool CGrpObject::SearchPointInTool(CPoint pt)
{
	m_tkSELECT = -1;

	if (m_ObjStatus & GOS_TCREATE)
		return true;
	if (m_ObjStatus & GOS_DURATION)
		return true;

	CToolBase* pToolBase;
	for (int ii = m_ToolQue.GetUpperBound(); ii >= 0; ii--)
	{
		pToolBase = m_ToolQue.GetAt(ii);
		if (pToolBase->PtInToolRegion(pt))	// on line
		{
			m_tkSELECT = ii;
			m_tkSAVE = ii;
			m_ObjStatus |= GOS_TMOVE;
			return true;
		}
		if (pToolBase->PtInHolderRect(pt))	// on holder
		{
			m_tkSELECT = ii;
			m_tkSAVE = ii;
			m_ObjStatus |= GOS_TCHANGE;
			return true;
		}
		
	}
	return false;


}

CGrpBase* CGrpObject::GetMainBaseGraph()
{
	CGrpBase* pGrpBase;

	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pGrpBase = m_GraphQue.GetAt(ii);
		switch (pGrpBase->GetGraphType())
		{
		case GT_BONG:
			if (pGrpBase->GetGraphKind() != GK_VOL
				&& pGrpBase->GetGraphKind() != GK_AMT)
				break;
			continue;
		case GT_VARS:
			if (pGrpBase->GetGraphKind() == GK_PNF)
				break;
			continue;
		default:
			continue;
		}
		return pGrpBase;
	}

	return (CGrpBase *) 0;
}

void CGrpObject::SetGrpEnv(bool bReset)
{
	CGrpBase* pGrpBase;

	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pGrpBase = m_GraphQue.GetAt(ii);
		switch (pGrpBase->GetGraphType())
		{
		case GT_BONG:
			if (pGrpBase->GetGraphKind() != GK_VOL
				&& pGrpBase->GetGraphKind() != GK_AMT)
				pGrpBase->SetStatus(m_ObjEnv, bReset);
			break;
		case GT_LINE:
			if (pGrpBase->GetGraphKind() == GK_PIVOT || 
				pGrpBase->GetGraphKind() == GK_DEMARK)
				pGrpBase->SetStatus(m_ObjEnv, bReset);
			break;
		default:
			continue;
		}
	}
}


bool CGrpObject::SetObjType(int type)
{
	if (m_pMgr->m_grxType == DEF_SALE)
	{
		m_ObjEnv &= ~(ENV_BONG | ENV_USA | ENV_LOG);
		m_ObjEnv |= ENV_LINE;
	}
	else if (m_pMgr->m_grxType == DEF_EQUIVOL)
	{
		int index = m_pMgr->m_pView->SendMessage(GEV_INPUT, 
				MAKEWPARAM(inputIndex, 1), 0);
		if (index == GI_TICK)
		{
			m_ObjEnv &= ~(ENV_BONG | ENV_USA | ENV_LOG);
			m_ObjEnv |= ENV_LINE;
		}
		else
		{
			m_ObjEnv &= ~(ENV_LINE | ENV_USA | ENV_LOG);
			m_ObjEnv |= ENV_BONG;
		}
	}
	else
	{
		switch (type)
		{
		case ENV_BONG:
			m_ObjEnv &= ~(ENV_USA | ENV_LINE | ENV_LOG);
			m_ObjEnv |= ENV_BONG;
			break;
		case ENV_USA:
			m_ObjEnv &= ~(ENV_BONG | ENV_LINE | ENV_LOG);
			m_ObjEnv |= ENV_USA;
			break;
		case ENV_LINE:
			m_ObjEnv &= ~(ENV_BONG | ENV_USA | ENV_LOG);
			m_ObjEnv |= ENV_LINE;
			break;
		case ENV_LOG:
			m_ObjEnv &= ~(ENV_BONG | ENV_USA | ENV_LINE);
			m_ObjEnv |= ENV_LOG;
			break;
		case ENV_NONE:
			m_ObjEnv &= ~(ENV_BONG | ENV_USA | ENV_LOG);
			m_ObjEnv |= ENV_LINE;
			break;
		}
	}

	SetGrpEnv(true);
	ReDrawGraphObject();
	return true;
}


void CGrpObject::SetObjMod(bool bSet)
{
	if (!IsObjEditable())
		return;

	if (bSet)
		m_ObjEnv |= ENV_MOD;
	else
		m_ObjEnv &= ~ENV_MOD;
}

bool CGrpObject::SetObjBong(bool bBong)
{
	if (!IsObjEditable())
		return false;

	if (bBong)
	{
		if (m_ObjEnv & ENV_BONG)
		{
			return true;
		}
		else
		{
			if (ExistDupBong())
				return false;

			m_ObjEnv &= ~ENV_LINE;
			m_ObjEnv |= ENV_BONG;
		}
	}
	else
	{
		if (m_ObjEnv & ENV_LINE)
		{
			return true;
		}
		else
		{
			m_ObjEnv &= ~ENV_BONG;
			m_ObjEnv |= ENV_LINE;
		}
	}

	SetGrpEnv(true);
	ReDrawGraphObject();

	return true;
}

void CGrpObject::SetObjLog(bool bSet)
{
	if (!IsObjEditable())
		return;

	if (bSet)
		m_ObjEnv |= ENV_LOG;
	else
		m_ObjEnv &= ~ENV_LOG;

	SetGrpEnv();
	ReDrawGraphObject();
}

void CGrpObject::SetObjDup()
{
	if (!IsObjEditable())
		return;

	if (m_ObjEnv & ENV_BONG)
		return;

	if (GetCountDupBong() >= MAXDUP)
	{
		char tmp[64];

		sprintf(tmp, "중복 차트는 최대%d개 까지입니다.", MAXDUP);
		::MessageBox(NULL, tmp, "확인", MB_ICONWARNING|MB_OK);
		return;
	}

	CGrpBase* pGrpBase;
	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pGrpBase = m_GraphQue.GetAt(ii);

		if (pGrpBase->GetGraphType() != GT_BONG)
			continue;

		switch (pGrpBase->GetGraphKind())
		{
			case GK_JPN:
			case GK_LOG:
			case GK_USA:
				if (m_ObjEnv & ENV_LINE)
				{
					m_ObjEnv |= ENV_DUP;
					return;
				}
				break;
			case GK_ENVL:
			case GK_PARB:
			case GK_BOLB:
			case GK_BALANCE:
			case GK_TICK:
			case GK_STEP:
			case GK_MGJY:
				break;
			case GK_LIN:
				m_ObjEnv |= ENV_DUP;
				return;
			default:
				break;
		}
	}
}

void CGrpObject::SetNewData(char *NewData)
{
	if (m_DataFormat)
		m_DataFormat->SetFgraph(NewData); 
}

bool CGrpObject::IsObjEditable()
{
	return m_DataFormat->IsEditable();
}

bool CGrpObject::ExistDupBong(int rgn)
{
	CGrpBase* pGrpBase;

	if (rgn)
	{
		for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
		{
			pGrpBase = m_GraphQue.GetAt(ii);

			if (pGrpBase->GetGraphRegionID() != rgn)	continue;
			if (pGrpBase->GetGraphType() != GT_BONG)	continue;
			if (pGrpBase->IsAdditionalType() >= 0)	return true;
		}
	}
	else
	{
		for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
		{
			pGrpBase = m_GraphQue.GetAt(ii);

			if (pGrpBase->GetGraphType() != GT_BONG)	continue;
			if (pGrpBase->IsAdditionalType() >= 0)	return true;
		}
	}

	return false;
}

int CGrpObject::GetCountDupBong(int rgn)
{
	int	count = 0;
	CGrpBase* pGrpBase;

	if (rgn)	//get all basic graph count
	{
		for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
		{
			pGrpBase = m_GraphQue.GetAt(ii);

			if (pGrpBase->GetGraphRegionID() != rgn)	continue;
			if (pGrpBase->GetGraphType() != GT_BONG)	continue;

			switch (pGrpBase->GetGraphKind())
			{
			case GK_LG_PMA:	case GK_LG_LMA:	case GK_LG_VMA:	
			case GK_LG_AMA:	case GK_LG_LIN:	case GK_LG_LOG:	
			case GK_LG_ENVL: case GK_LG_PARB: case GK_LG_BOLB:
			case GK_LG_NET:
				break;
			default:
				count++;
				break;
			}
		}
	}
	else
	{
		for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
		{
			pGrpBase = m_GraphQue.GetAt(ii);

			if (pGrpBase->GetGraphType() != GT_BONG)	continue;
			if (pGrpBase->IsAdditionalType() >= 0)
			{
				switch (pGrpBase->GetGraphKind())
				{
				case GK_LG_PMA:	case GK_LG_LMA:	case GK_LG_VMA:	
				case GK_LG_AMA:	case GK_LG_LIN:	case GK_LG_LOG:	
				case GK_LG_ENVL: case GK_LG_PARB: case GK_LG_BOLB:
				case GK_LG_NET:
					break;
				default:
					count++;
					break;
				}
			}
		}
	}

	return count;
}

CGrpBase* CGrpObject::ExistBongTypeGraph()
{
	CGrpBase* pGrpBase;

	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pGrpBase = m_GraphQue.GetAt(ii);
		if (pGrpBase->GetGraphType() == GT_BONG && ii == 0)
		{
			return pGrpBase;
		}
	}
	return (CGrpBase *) 0;
}

CRgnSplit* CGrpObject::IsSplitRgn(CPoint pt)
{
	CGrpBase *pGrpBase;
	CRgnSplit* pRgnSplit = NULL;
	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pGrpBase = m_GraphQue.GetAt(ii);
		if (pGrpBase->ExistSplit() )
		{
			pRgnSplit = pGrpBase->GetRgnSplit();
			if (pRgnSplit->PtInRgn(pt))
			{
				return pRgnSplit;
			}
		}
	}

	return (CRgnSplit *) 0;
}

bool CGrpObject::DurationCursorModify(bool bArrow)
{
	HCURSOR hCursor = NULL;

	if (bArrow)
	{
		::ClipCursor(NULL);
		::ReleaseCapture();
		m_ObjStatus &= ~GOS_CAPTURE;
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		::SetCursor(hCursor);
		return true;
	}

	switch (m_tkCHOICED)
	{
	case tkTENDENCY:
		hCursor = AfxGetApp()->LoadCursor(IDC_TENDLINE);
		break;
	case tkUSRINPUT:
		hCursor = AfxGetApp()->LoadCursor(IDC_TEXT);
		break;
	default:
		return false;
	}

	if (m_ObjStatus & GOS_CAPTURE)
	{
		::ClipCursor(NULL);
		::ReleaseCapture();
		m_ObjStatus &= ~GOS_CAPTURE;
		if (hCursor == NULL)
		{
			hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
			::SetCursor(hCursor);
			return true;
		}
	}

	CRect clipRC = m_GrpRegion;
	clipRC.right--;
	m_pGraph->ClientToScreen(&clipRC);
//	::ClipCursor(&clipRC);

	::SetCapture(m_pGraph->m_hWnd);
	m_ObjStatus |= GOS_CAPTURE;
	::SetCursor(hCursor);
	return true;
}


bool CGrpObject::OnetimeCursorModify(bool bArrow)
{
	HCURSOR	hCursor = NULL;

	if (bArrow)
	{
		::ClipCursor(NULL);
		::ReleaseCapture();
		m_ObjStatus &= ~GOS_CAPTURE;
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		::SetCursor(hCursor);
		return true;
	}

	switch (m_tkCHOICED)
	{
	case tkFIBOLINE:
		hCursor = AfxGetApp()->LoadCursor(IDC_FIBOLINE);
		break;
	case tkFIBOFAN:
		hCursor = AfxGetApp()->LoadCursor(IDC_FIBOFAN);
		break;
	case tkFIBOARC:
		hCursor = AfxGetApp()->LoadCursor(IDC_FIBOARC);
		break;
	case tkFIBORET:
		hCursor = AfxGetApp()->LoadCursor(IDC_FIBORET);
		break;
	case tkGANNLINE:
		hCursor = AfxGetApp()->LoadCursor(IDC_GANNLINE);
		break;
	case tkGANNFAN:
		hCursor = AfxGetApp()->LoadCursor(IDC_GANNFAN);
		break;
	case tkACCELFAN:
		hCursor = AfxGetApp()->LoadCursor(IDC_ACCELFAN);
		break;
	case tkACCELARC:
		hCursor = AfxGetApp()->LoadCursor(IDC_ACCELARC);
		break;
	case tkCROSS:
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		break;
	default:
		return false;
	}

	if (m_ObjStatus & GOS_CAPTURE)
	{
		::ClipCursor(NULL);
		::ReleaseCapture();
		m_ObjStatus &= ~GOS_CAPTURE;

		if (hCursor == NULL)
		{
			hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
			::SetCursor(hCursor);
			return true;
		}
	}

	CRect clipRC = m_GrpRegion;
	m_pGraph->ClientToScreen(&clipRC);
	::ClipCursor(&clipRC);

	::SetCapture(m_pGraph->m_hWnd);
	m_ObjStatus |= GOS_CAPTURE;
	::SetCursor(hCursor);
	return true;
}

void CGrpObject::SplitCursor(CRect *pRc, bool bLineOver, bool bLineDrag)
{
	HCURSOR	hCursor = NULL;

	::ClipCursor(NULL);
	::ReleaseCapture();

	if (bLineDrag)
	{
		if (m_bLineSelect)
		{
			if (pRc && !pRc->IsRectEmpty())	
			{
				m_pGraph->ClientToScreen(pRc);
				::ClipCursor(pRc);
			}

			::SetCapture(m_pGraph->m_hWnd);

			hCursor = AfxGetApp()->LoadCursor(IDC_DRAGLINE);
			::SetCursor(hCursor);
		}
		else
		{
			hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
			::SetCursor(hCursor);
		}

		return;
	}


	if (bLineOver)
	{
		::SetCapture(m_pGraph->m_hWnd);
		hCursor = AfxGetApp()->LoadCursor(IDC_HAND);
		::SetCursor(hCursor);
		return;
	}

	m_ObjLine &= ~GOL_LINEOVER;
	if (m_ObjStatus & GOS_SPLITOVER || m_ObjStatus & GOS_SPLITMOVE)	// first split rgn
	{
		if (pRc && !pRc->IsRectEmpty())	
		{
			m_pGraph->ClientToScreen(pRc);
			::ClipCursor(pRc);
		}

		::SetCapture(m_pGraph->m_hWnd);
		hCursor = AfxGetApp()->LoadCursor(IDC_SPLIT);
		::SetCursor(hCursor);
		return;
	}

	hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	::SetCursor(hCursor);
}

bool CGrpObject::ResetCursorModify()
{
	if (m_tkCHOICED != tkNONE && (m_ObjStatus & GOS_TCREATE))
	{
		if (m_ObjStatus & GOS_CAPTURE)
		{
			::ClipCursor(NULL);
			::ReleaseCapture();
		}

		int idx = m_ToolQue.GetUpperBound();
		if (idx >= 0)
		{
			CToolBase* pToolBase = m_ToolQue.GetAt(idx);
			pToolBase->HideHolder();
		}
		return true;
	}

	return false;
}

int CGrpObject::ReadUserData(CString& data)
{
	int	mapKey;
	CGrpData* pGrpData;

	int	unit = 0, index = 0;
	for (POSITION pos = m_MapData.GetStartPosition(); pos; )
	{
		m_MapData.GetNextAssoc(pos, mapKey, pGrpData);
		if (pGrpData->GetDataPattern() == dpCOMMON)
			return pGrpData->ReadUserData(data);
	}

	return -1;
}

void CGrpObject::ReAssignRgn(CGrpBase *pDnBase, CGrpBase *pUpBase, int vShift)
{
	struct _xRegion*	pDnRgn;
	struct _xRegion*	pUpRgn;
	int	RgnKey;

	if (!GetGrpRegion(pUpBase->GetGraphRegionID(), pUpRgn))
	{
		TRACE("Assign Region not found !!\n");
		return;
	}

	if (pDnBase)
	{
		if (!GetGrpRegion(pDnBase->GetGraphRegionID(), pDnRgn))
		{
			TRACE("Assign Region not found !!\n");
			return;
		}
	}
	else
	{
		RgnKey = pDnBase->GetGraphRegionID();
		struct _xRegion* xRegion;
		for (POSITION pos = m_MapRegion.GetStartPosition(); pos; )
		{
			m_MapRegion.GetNextAssoc(pos, RgnKey, xRegion);
			if (xRegion->sRect.top == pUpRgn->sRect.bottom)
				pDnRgn = xRegion;
		}
	}

	struct _xRegion SumRgn;

	SumRgn.sRect = pUpRgn->sRect;
	SumRgn.sRect.bottom = pDnRgn->sRect.bottom;
	pUpRgn->sRect = pDnRgn->sRect = SumRgn.sRect;
	SumRgn.xRect = pUpRgn->xRect;
	SumRgn.xRect.bottom = pDnRgn->xRect.bottom;
	pUpRgn->xRect = pDnRgn->xRect = SumRgn.xRect;

	CRect	UpRect = pUpBase->GetDrawRect();
	CRect	DnRect = pDnBase->GetDrawRect();
	CRgnSplit*	pUpSplit = pUpBase->GetRgnSplit();
	CRgnSplit*	pDnSplit = pDnBase->GetRgnSplit();

	if (pUpSplit)
		UpRect.bottom = pUpSplit->GetRgn().bottom;
	if (pDnSplit)
		DnRect.bottom = pDnSplit->GetRgn().bottom;

	CRect	SumRect = UpRect;
	SumRect.bottom = DnRect.bottom;
	UpRect.bottom += vShift;

	DnRect.top += vShift;
	float	ratio = float(UpRect.Height()) / float(SumRect.Height());

	pUpRgn->sRect.bottom =  pUpRgn->sRect.top + int(float(SumRgn.sRect.Height()) * ratio);
	pDnRgn->sRect.top =  pUpRgn->sRect.bottom;
	pUpRgn->xRect.bottom =  pUpRgn->xRect.top + int(float(SumRgn.xRect.Height()) * ratio);
	pDnRgn->xRect.top =  pUpRgn->xRect.bottom;

	int minimumH = int((float)(splitHEIGHT + 1)/ m_vRatio);
	if (pDnRgn->sRect.Height() < minimumH)
	{
		pDnRgn->sRect.top = pDnRgn->sRect.bottom - minimumH;
		pUpRgn->sRect.bottom = pDnRgn->sRect.top;
		pDnRgn->xRect.top = pDnRgn->xRect.bottom - minimumH;
		pUpRgn->xRect.bottom = pDnRgn->xRect.top;
	}
	
	if (GetTopAdjustGrp(pUpRgn) == (CGrpBase *)0 )
	{
		if (pUpRgn->sRect.Height() < minimumH * 2)
		{
			pUpRgn->sRect.bottom = pUpRgn->sRect.top + minimumH *2;
			pDnRgn->sRect.top = pUpRgn->sRect.bottom;
			pUpRgn->xRect.bottom = pUpRgn->xRect.top + minimumH *2;
			pDnRgn->xRect.top = pUpRgn->xRect.bottom;
		}
	}
	if (pUpRgn->sRect.Height() < minimumH)
	{
			pUpRgn->sRect.bottom = pUpRgn->sRect.top + minimumH;
			pDnRgn->sRect.top = pUpRgn->sRect.bottom;
			pUpRgn->xRect.bottom = pUpRgn->xRect.top + minimumH;
			pDnRgn->xRect.top = pUpRgn->xRect.bottom;
	}

	SaveRegion();
	ResizeGraphObject(m_hRatio, m_vRatio, true, true, true);
}

void CGrpObject::SaveRegion()
{
	struct _xRegion* xRegion;

	int	RgnKey;
	int	TotalT = 10000, TotalB = -1;
	for (POSITION pos = m_MapRegion.GetStartPosition(); pos; )
	{
		m_MapRegion.GetNextAssoc(pos, RgnKey, xRegion);

		if (xRegion->sRect.top < TotalT)
			TotalT = xRegion->sRect.top;

		if (xRegion->sRect.bottom > TotalB)
			TotalB = xRegion->sRect.bottom;
	}

	int	Total = TotalB - TotalT;
	if (Total == 0)
		return;

	struct _fgraph *pFgraph = m_DataFormat->GetGrpFileInfo();
	struct _fgraph fgraph;

	CopyMemory(&fgraph, pFgraph, sz_FGRAPH);

	struct _region*	region;
	for (int ii = 0; ii < maxGRAPH; ii++)
	{
		region = &fgraph.region[ii];
		for (pos = m_MapRegion.GetStartPosition(); pos; )
		{
			m_MapRegion.GetNextAssoc(pos, RgnKey, xRegion);
			if (region->key == RgnKey)
			{
				long	top = xRegion->sRect.top;
				long bottom = xRegion->sRect.bottom;

				int	sRgnKey;
				struct _xRegion* sRegion;
				for (POSITION sPos = m_MapRegion.GetStartPosition(); sPos; )
				{
					m_MapRegion.GetNextAssoc(sPos, sRgnKey, sRegion);
					if (sRegion->orgRgnKey == RgnKey)
					{
						if (bottom < sRegion->sRect.bottom)
							bottom = sRegion->sRect.bottom;
					}
				}

				region->top = (top - TotalT) * 100 / Total;
				region->bottom = (bottom - TotalT) * 100 / Total;
				break;
			}
		}
	}

	char	dBuf[sz_FGRAPH + 32];
	CopyMemory(dBuf, &fgraph, sz_FGRAPH);

	m_pMgr->FileInfo(1000, dBuf, sz_FGRAPH, m_ObjName);
	m_DataFormat->SetFgraph((char *)&fgraph);
}

CGrpBase* CGrpObject::GetBottomAdjustGrp(struct _xRegion* pPivotRegion)
{
	struct _xRegion* xRegion;
	int	tRgnKey, RgnKey;

	for (POSITION pos = m_MapRegion.GetStartPosition(); pos; )
	{
		m_MapRegion.GetNextAssoc(pos, tRgnKey, xRegion);
		if (pPivotRegion->sRect.bottom == xRegion->sRect.top)
		{
			RgnKey = tRgnKey;
			break;
		}
	}

	CGrpBase* pTemp;
	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pTemp = m_GraphQue.GetAt(ii);
		if (pTemp->GetGraphRegionID() == RgnKey)
			return pTemp;
	}

	return (CGrpBase *) 0;
}

CGrpBase* CGrpObject::GetTopAdjustGrp(struct _xRegion* pPivotRegion)
{
	struct _xRegion* xRegion;
	int	tRgnKey, RgnKey;

	for (POSITION pos = m_MapRegion.GetStartPosition(); pos; )
	{
		m_MapRegion.GetNextAssoc(pos, tRgnKey, xRegion);
		if (xRegion->sRect.bottom == pPivotRegion->sRect.top)
		{
			RgnKey = tRgnKey;
			break;
		}
	}

	CGrpBase* pTemp;
	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pTemp = m_GraphQue.GetAt(ii);
		if (pTemp->GetGraphRegionID() == RgnKey)
			return pTemp;
	}

	return (CGrpBase *) 0;
}

bool CGrpObject::IsDataVisible()
{
	return m_bDataVisible;
}

bool CGrpObject::IsCrossVisible()
{
	return m_bCrossVisable;
}

DWORD CGrpObject::GetOperation()
{
	DWORD	operation = 0;

	if (!m_bAppendMode)
		operation |= GO_OVRLAP;
	if (m_bReverseMode)
		operation |= GO_REVGRP;
	if (!m_bGradient)
		operation |= GO_NOGRAD;
	if (!m_bUpFill)
		operation |= GO_NOUPFILL;
	if (!m_bDnFill)
		operation |= GO_NODNFILL;
	if (!m_bDataVisible)
		operation |= GO_NOGVIEW;
	if (!m_bCrossVisable)
		operation |= GO_CHIDE;
	if (m_bJTrace)
		operation |= GO_JTRACE;
	if (m_bSolidAidLine)
		operation |= GO_SOLIDAIDLINE;

	if (m_bNMLine)
	{
		operation &= ~GO_ST_LINE;
		operation |= GO_NM_LINE;
	}
	else
	{
		operation &= ~GO_NM_LINE;
		operation |= GO_ST_LINE;
	}

	return operation;
}

void CGrpObject::SaveMouseOption()
{
	struct _fgraph *pFgraph = m_DataFormat->GetGrpFileInfo();
	struct _fgraph fgraph;

	CopyMemory(&fgraph, pFgraph, sz_FGRAPH);
	fgraph.operation = GetOperation();

	char	dBuf[sz_FGRAPH + 32];
	CopyMemory(dBuf, &fgraph, sz_FGRAPH);

	m_pMgr->FileInfo(1000, dBuf, sz_FGRAPH, m_ObjName);
	m_DataFormat->SetFgraph((char *)&fgraph);
}

void CGrpObject::SetAppendMode(bool bAppend)
{
	m_bAppendMode = bAppend;
	SaveMouseOption();
}

void CGrpObject::SetReverseMode()
{
	m_bReverseMode = !m_bReverseMode;
	SaveMouseOption();
	ReDrawGraphObject();
}

void CGrpObject::SetLineType()
{
	m_bNMLine = !m_bNMLine;
	SaveMouseOption();
	ReDrawGraphObject();
}

void CGrpObject::SetJTrace()
{
	m_bJTrace = !m_bJTrace;
	SaveMouseOption();
}

void CGrpObject::SetDataVisible()
{
	m_bDataVisible = !m_bDataVisible;
	if (m_pCrossLine != NULL)
		m_pCrossLine->SetDataVisable(m_bDataVisible);
	SaveMouseOption();
}

void CGrpObject::SetCrossView()
{
	m_bCrossVisable = !m_bCrossVisable;
	if (m_pCrossLine != NULL)
		m_pCrossLine->SetVisable();
	SaveMouseOption();
}

void CGrpObject::SetGradient()
{
	m_bGradient = !m_bGradient;
	ReDrawGraphObject();
	SaveMouseOption();
}

void CGrpObject::GetDataGap()
{	
	if (!m_bUpdateREAL || (m_bUpdateREAL && m_bEveryUpdate))
		return;

	struct	_gCommon	*CurCommon, *PreCommon;
	struct	_gTick	*CurTick, *PreTick;
	struct	_gVidx	*CurVidx, *PreVidx;

	int	RTMGap = INT_MAX, Gap;
	int	mapKey;
	int	Size;
	int	ii;
	CGrpData	*pGrpData;
	for (POSITION pos = m_MapData.GetStartPosition(); pos; )
	{
		m_MapData.GetNextAssoc(pos, mapKey, pGrpData);
		if (pGrpData->GetDataIndex() == GI_HMS)
		{
			switch (pGrpData->GetDataKind())
			{
			case DK_BONG:	case DK_VOL:	case DK_AMT:
				Size = pGrpData->GetArraySize();
				for (ii = 1; ii < Size; ii++)
				{
					PreCommon = (_gCommon *)pGrpData->GetGraphData(ii - 1);
					CurCommon = (_gCommon *)pGrpData->GetGraphData(ii);
					if (PreCommon->epr == GD_UNUSED || CurCommon->epr == GD_UNUSED)
						continue;

					Gap = (CurCommon->index.time.hh - PreCommon->index.time.hh) * 3600;
					Gap += (CurCommon->index.time.mm - PreCommon->index.time.mm) * 60;
					Gap += (CurCommon->index.time.ss - PreCommon->index.time.ss);

					if (Gap>0 && Gap < RTMGap)
						RTMGap = Gap;
				}
				break;
			case DK_VIDX:
				Size = pGrpData->GetArraySize();
				for (ii = 1; ii < Size; ii++)
				{
					PreVidx = (_gVidx *)pGrpData->GetGraphData(ii - 1);
					CurVidx = (_gVidx *)pGrpData->GetGraphData(ii);

					Gap = (CurVidx->index.time.hh - PreVidx->index.time.hh) * 3600;
					Gap += (CurVidx->index.time.mm - PreVidx->index.time.mm) * 60;
					Gap += (CurVidx->index.time.ss - PreVidx->index.time.ss);

					if (Gap>0 && Gap < RTMGap)
						RTMGap = Gap;
				}
				break;
			case DK_TICK:
				// 수정 0624 ===>
				Size = pGrpData->GetArraySize();
				for (ii = 1; ii < Size; ii++)
				{
					PreTick = (_gTick *)pGrpData->GetGraphData(ii - 1);
					CurTick = (_gTick *)pGrpData->GetGraphData(ii);
					if (PreTick->epr == GD_UNUSED || CurTick->epr == GD_UNUSED)
						continue;

					Gap = (CurTick->time.hh - PreTick->time.hh) * 3600;
					Gap += (CurTick->time.mm - PreTick->time.mm) * 60;
					Gap += (CurTick->time.ss - PreTick->time.ss);

					if (Gap>0 && Gap < RTMGap)
						RTMGap = Gap;
				}
				// 수정 0624 <===
				break;
			default:
				break;
			}
		}
	}

	if (RTMGap != INT_MAX)
	{
		for (POSITION pos = m_MapData.GetStartPosition(); pos; )
		{
			m_MapData.GetNextAssoc(pos, mapKey, pGrpData);
			pGrpData->SetRTMGap(RTMGap);
		}
	}
}

int CGrpObject::GetUnusedRegionKey()
{
	struct	_xRegion* xRegion;
	for (int rgnKey = 1; rgnKey < 254; rgnKey++)
	{
		if (!m_MapRegion.Lookup(rgnKey, xRegion))
			return rgnKey;
	}

	return 254;
}

void CGrpObject::SetBongOption(DWORD UpColor, DWORD DnColor, DWORD option)
{
	if (option & GO_NOGRAD)
		m_bGradient = false;
	else
		m_bGradient = true;
	if (option & GO_NOUPFILL)
		m_bUpFill = false;
	else
		m_bUpFill = true;
	if (option & GO_NODNFILL)
		m_bDnFill = false;
	else
		m_bDnFill = true;
	if (option & GO_SOLIDAIDLINE)
		m_bSolidAidLine = true;
	else
		m_bSolidAidLine = false;

	if (UpColor & rgbCOLOR)
		m_UpColor = GetPaletteRGB(UpColor);
	if (DnColor & rgbCOLOR)
		m_DnColor = GetPaletteRGB(DnColor);
}

COLORREF CGrpObject::GetUserInputColor()
{
	return GetPaletteRGB(m_xTick[xBOTTOM].xColor);
}

bool CGrpObject::ParseRealtimeECN(CString rtm, struct _rtmEx *rtmEx)
{
	CGrpBase	*pGrpBase;
	CString	symbol;
	bool	bVol = false;
	bool	bCur = false;
	bool	bMgjy = false;
	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		if (!pGrpBase)
			return false;

		switch (pGrpBase->GetGraphType())
		{
		case GT_BONG:
			switch (pGrpBase->GetGraphKind())
			{
			
			case GK_JPN:
			case GK_LOG:
			case GK_USA:
			case GK_LIN:
			case GK_ENVL:
			case GK_PARB:
			case GK_BOLB:
			case GK_BALANCE:
			case GK_NET:
			case GK_TICK:
			case GK_STEP:
				if (!IsAlreadyGet(ER_CURR, rtmEx))
				{
					rtmEx->arg[rtmEx->count].Symbol = ER_CURR;
					rtmEx->arg[rtmEx->count++].pGrpBase = pGrpBase;
				}

				bCur = true;
				break;
			case GK_VOL:
			case GK_EVOL:
				if (m_ObjIndex == GI_HMS)
					symbol = ER_CVOL;	// 체결량
				else
					symbol = ER_GVOL;	// 누적거래량
				if (!IsAlreadyGet(symbol, rtmEx))
				{
					rtmEx->arg[rtmEx->count].Symbol = symbol;
					rtmEx->arg[rtmEx->count++].pGrpBase = pGrpBase;
				}

				bVol = true;
				break;
			
			default:
				return false;
			}
			break;
		case GT_LINE:
			switch (pGrpBase->GetGraphKind())
			{
			case GK_ADLINE:
			case GK_OBV:
			case GK_VR:
			case GK_PV:
			case GK_VMAO:
			case GK_CLOCK:
				if (m_ObjIndex == GI_HMS)
					symbol = ER_CVOL;
				else
					symbol = ER_GVOL;
				if (!IsAlreadyGet(symbol, rtmEx))
				{
					rtmEx->arg[rtmEx->count].Symbol = symbol;
					rtmEx->arg[rtmEx->count++].pGrpBase = pGrpBase;
				}

				bVol = true;
			default:
				if (!IsAlreadyGet(ER_CURR, rtmEx))
				{
					rtmEx->arg[rtmEx->count].Symbol = ER_CURR;
					rtmEx->arg[rtmEx->count++].pGrpBase = pGrpBase;
				}

				bCur = true;
				break;
			case GK_RRATE:
				return false;
			}
			break;
		case GT_USER:
			switch (pGrpBase->GetGraphKind())
			{
			case GK_LINE_VIDX:	// vidx
			case GK_STEP_VIDX:	// vidx
			case GK_XVBAR_VIDX:	// vidx
			case GK_VBAR_VIDX:	// vidx
			case GK_VBAR_REV:	// vidx
				if (pGrpBase->m_valueAV[0] <= 0)
					return false;

				if (pGrpBase->m_valueAV[0] > 0)
				{
					symbol.Format("%03d", pGrpBase->m_valueAV[0]);
					if (!IsAlreadyGet(symbol, rtmEx))
					{
						rtmEx->arg[rtmEx->count].Symbol = symbol;
						rtmEx->arg[rtmEx->count++].pGrpBase = pGrpBase;
					}
				}
				else
				{
					return false;
				}
				break;
			default:
				return false;
			}
			break;
		default:
			return false;
		}
	}

	// search (symbol,val)
	CMapStringToString	rtmStore;
	CString	val;
	int	index;
	for (; !rtm.IsEmpty(); )
	{
		index = rtm.Find('\t');
		if (index < 0)
			break;
		symbol = rtm.Left(index++);
		rtm = rtm.Mid(index);

		index = rtm.Find('\t');
		if (index < 0)
			index = rtm.GetLength();
		val = rtm.Left(index++);

		rtmStore.SetAt(symbol, val);

		if (rtm.GetLength() <= index)
			break;
		rtm = rtm.Mid(index);
	}

	POSITION	pos;
	CString	CTime = _T("");
	CString	HTime = _T("");
	m_mgjyRTM = m_currRTM = m_gvolRTM = _T("");
	switch (m_ObjUnit)
	{
	case GU_INDEX:	case GU_KOSPI:	case GU_FUTURE:
	case GU_KQFUT:	case GU_OPTION:	case GU_KQOPT:
		for (pos = rtmStore.GetStartPosition(); pos;)
		{
			rtmStore.GetNextAssoc(pos, symbol, val);
			if (ER_CTIM == symbol)
				CTime = val;
			else if (ER_HTIM == symbol)
				HTime = val;
			else if (ER_CURR == symbol)
				m_currRTM.Format("%.2f", fabs(atof(val)));
			else if (ER_CVOL == symbol && m_ObjIndex == GI_HMS)
				m_gvolRTM = val;
			else if (ER_GVOL == symbol && m_ObjIndex != GI_HMS)
				m_gvolRTM = val;
		}
		break;
	default:
		for (pos = rtmStore.GetStartPosition(); pos;)
		{
			rtmStore.GetNextAssoc(pos, symbol, val);
			if (ER_CTIM == symbol)
				CTime = val;
			else if (ER_HTIM == symbol)
				HTime = val;
			else if (ER_CURR == symbol)
				m_currRTM.Format("%d", abs(atoi(val)));
			else if (ER_CVOL == symbol && m_ObjIndex == GI_HMS)
				m_gvolRTM = val;
			else if (ER_GVOL == symbol && m_ObjIndex != GI_HMS)
				m_gvolRTM = val;
		}
		break;
	}

	if (CTime.IsEmpty() && HTime.IsEmpty())
		return false;

	if (bVol && m_gvolRTM.IsEmpty())
		return false;
	if (bCur && m_currRTM.IsEmpty())
		return false;

	int	num;
	bool	bHtime = false;	//호가 시간

	for (ii = 0; ii < rtmEx->count; ii++)
	{
		num = atoi(rtmEx->arg[ii].Symbol);
		if ( num >= 741 && num <= 810)
			bHtime = true;

		for (pos = rtmStore.GetStartPosition(); pos;)
		{
			rtmStore.GetNextAssoc(pos, symbol, val);
			if (rtmEx->arg[ii].Symbol == symbol)
			{
				rtmEx->arg[ii].val = val;
				pos = (struct __POSITION *)1;
				break;
			}
		}
		if (pos == NULL)
			return false;
	}

	if (bHtime)
		m_timeRTM = HTime;
	else
		m_timeRTM = CTime;

	return ((m_timeRTM.GetLength() >= 6)? true:false);
}

CGrpBase* CGrpObject::GetGrpBase()
{
	CGrpBase* pGrpBase = ExistBongTypeGraph();
	if (pGrpBase == (CGrpBase *) 0 && m_GraphQue.GetSize() > 0)
		pGrpBase = m_GraphQue.GetAt(0);
	return pGrpBase;
}

void CGrpObject::mouseResizeArea(CPoint pt, bool bEnd)
{
	if (pt.x < 0)
		pt.x = 0;

	pt.x = getPosition(-1, pt);
	if (m_SSizePt.x < 0) // first LBUTTONDOWN
	{
		::SetCapture(m_pGraph->m_hWnd);
		m_SSizePt = m_ESizePt = pt;
		drawMouseArea(pt, false);
		return;
	}

	if (!bEnd)
	{
		drawMouseArea(pt, true);	// delete pre area, draw current area
		return;
	}

	drawMouseArea(pt, false);
	::ReleaseCapture();

	int start = getPosition(m_SSizePt.x);
	int end = getPosition(m_ESizePt.x);
	if (start > end)
	{
		start = end + start;
		end = start - end;
		start = start - end;
	}

	if (end - start + 1 < 10)
	{
		if (!popDisplayStack(start, end))
			return;
		else
			setDisplayGraph(start, end);
	}
	else
	{
		saveCurrentPos();
		setDisplayGraph(start, end);
	}
	if (m_pMgr->m_bTotal)
	{
		m_pMgr->m_pView->SendMessage(GEV_INPUT, 
			MAKEWPARAM(inputDisp, 0), GetDisplayDay());
	}
	m_pMgr->SendMessage(GEV_CTRL, 
		MAKEWPARAM(ctrlScrollStatus, 2), 0);
}


void CGrpObject::drawMouseArea(CPoint pt, bool bMid)
{
	CDC	*pDC = m_pGraph->GetDC();

	CPen	cPen(PS_SOLID, 1, RGB(255, 255, 255));
	CBrush	cBrush(RGB(30, 30, 30));
	int	sROP = pDC->SetROP2(R2_XORPEN);
	int	sMode = pDC->SetBkMode(TRANSPARENT);
	CPen	*sPen = pDC->SelectObject(&cPen);
	CBrush	*sBrush = pDC->SelectObject(&cBrush);

	CRect	RC, RC2;
	RC = m_GrpRegion;
	RC.left = m_SSizePt.x;
	RC.right = m_ESizePt.x;
	RC.NormalizeRect();
	RC.IntersectRect(&RC, &m_GrpRegion);
	if (bMid)
	{
		pDC->MoveTo(m_ESizePt.x, m_GrpRegion.top);
		pDC->LineTo(m_ESizePt.x, m_GrpRegion.bottom);

		m_ESizePt.x = pt.x;
		RC2 = m_GrpRegion;
		RC2.left = m_SSizePt.x;
		RC2.right = m_ESizePt.x;
		RC2.NormalizeRect();
		RC2.IntersectRect(&RC2, &m_GrpRegion);
	}
	else
	{
		RC2.SetRectEmpty();
	}

	pDC->MoveTo(m_ESizePt.x, m_GrpRegion.top);
	pDC->LineTo(m_ESizePt.x, m_GrpRegion.bottom);

	CSize	sz = CSize(RC.Width(), RC.Height());
	CSize	sz2 = CSize(RC2.Width(), RC2.Height());
	pDC->DrawDragRect(&RC, sz, &RC2, sz2, &cBrush, &cBrush);

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
	pDC->SetBkMode(sMode);
	pDC->SetROP2(sROP);
	m_pGraph->ReleaseDC(pDC);
}

int CGrpObject::getPosition(int Pos, CPoint pt, bool bBalance)
{
	CGrpBase *pGrpBase = GetMainBaseGraph();
	CRect	rtmRC  = pGrpBase->GetDrawRect();

	int dispDay  = pGrpBase->GetDisplayDay();
	int dispPos = pGrpBase->GetDisplayPos();
	int totalDay = pGrpBase->GetTotalDataCount();
	double szOneDay = double(rtmRC.Width()) / double(dispDay);

	int	index;
	if (pt.x >= 0)
	{
		if (pt.x < rtmRC.left)
		{
			pt.x = rtmRC.left + int(szOneDay/2.0);
		}
		else if (pt.x > rtmRC.right)
		{
			pt.x = rtmRC.right - int(szOneDay/2.0);
		}
		else
		{
			index = int(double(pt.x - rtmRC.left) / szOneDay);
			pt.x = rtmRC.left + int (double(index) * szOneDay + szOneDay/2.0);
		}

		return pt.x;
	}

	index = int((Pos - rtmRC.left) / szOneDay);
	if (dispPos + index >= totalDay)
		index = totalDay - dispPos - 1;

	return dispPos + index;
}

int CGrpObject::GetDisplayDay()
{
	CGrpBase* pGrpBase;
	for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		if (!pGrpBase)
			return -1;
		return pGrpBase->GetDisplayDay();
	}
	return -1;
}

int CGrpObject::GetTotalDataCount()
{
	CGrpBase* pGrpBase;
	for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		if (!pGrpBase)
			return -1;
		return pGrpBase->GetTotalDataCount();
	}
	return -1;
}

void CGrpObject::setDisplayGraph(int start, int end)
{
	for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
	{
		CGrpBase *pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		pGrpBase->SetDisplayPos(start);
		pGrpBase->SetDisplayDay(end - start + 1);
		pGrpBase->SetDisplayEnd(end + 1);
		pGrpBase->SetDisplaySkip(0);
	}
	ReDrawGraphObject();
}

void CGrpObject::saveCurrentPos(int start, int end)
{
	if (start < 0)
	{
		CGrpBase *pGrpBase = GetMainBaseGraph();
		start = pGrpBase->GetDisplayPos();
		end  = pGrpBase->GetDisplayEnd();
	}
	pushDisplayStack(start, end);
}


bool CGrpObject::popDisplayStack(int &start, int &end)
{
	if (m_DisplayQue.GetSize() <= 0)
		return false;
	struct _xDisplay *display = m_DisplayQue.GetAt(m_DisplayQue.GetUpperBound());
	if (!display)
		return false;

	start = display->start;
	end = display->end;
	delete display;
	display = NULL;
	m_DisplayQue.RemoveAt(m_DisplayQue.GetUpperBound());

	return true;
}


void CGrpObject::pushDisplayStack(int start, int end)
{
	struct _xDisplay *display = (struct _xDisplay *)new char [sizeof(struct _xDisplay)];

	CGrpBase *pGrpBase = GetMainBaseGraph();
	if (pGrpBase->GetTotalDataCount() <= end)
		end = pGrpBase->GetTotalDataCount() - 1;

	if (start < 0)
		start = 0;

	display->start = start;
	display->end = end;

	int ret = m_DisplayQue.Add(display);
}

void CGrpObject::removeDisplayStack()
{
	struct _xDisplay* xDisplay = NULL;
	for (int ii = m_DisplayQue.GetUpperBound(); ii >= 0; ii--)
	{
		xDisplay = m_DisplayQue.GetAt(ii);
		delete xDisplay;
		xDisplay = NULL;
	}
	
	m_DisplayQue.RemoveAll();
}

void CGrpObject::toolHideHolder()
{
	CToolBase	*pToolBase;
	for (int ii = m_ToolQue.GetUpperBound(); ii >= 0; ii--)
	{
		if (ii == m_tkSELECT)
			continue;

		pToolBase = m_ToolQue.GetAt(ii);
		pToolBase->HideHolder();
	}
}

bool CGrpObject::IsAddable()
{
	int count = 0;
	for (int ii = 0; ii <= m_GraphQue.GetUpperBound(); ii++)
	{
		CGrpBase *pGrpBase = m_GraphQue.GetAt(ii);
		if (pGrpBase->m_ValidMAIndex == MA_BASIC)
			count++;
	}
	if (count >= maxGRAPH)
	{
		char tmp[64];
		sprintf(tmp, "최대 차트개수는 %d개 까지입니다.", maxGRAPH);
		::MessageBox(NULL, tmp, "확인", MB_OK);
		return false;
	}
	return true;
}

void CGrpObject::ViewTip(BOOL bShow, CString tipS)
{
	if (m_pMgr->m_bTotal)
		m_pMgr->m_pView->SendMessage(GEV_VIEW, MAKEWPARAM(viewShowTick, bShow), (long)(LPCTSTR)tipS);
	else
		m_pMgr->SendMessage(GEV_CTRL, MAKEWPARAM(ctrlShowTick, bShow), (long)(LPCTSTR)tipS);
}

void CGrpObject::getCrossValue(CToolBase* pToolBase, CPoint pt)
{
	CString tmpS = _T("");
	CGrpBase* pGrpBase = (CGrpBase *) NULL;
	bool bFind = false;
	for (int ii = 0; ii < m_GraphQue.GetSize(); ii++)
	{
		pGrpBase = (CGrpBase *) m_GraphQue.GetAt(ii);
		if (pGrpBase->IsPointInRect(pt))
		{
			bFind = true;
			break;
		}
	}
	if (!bFind || !pGrpBase || !pToolBase)
		return;
	
	if (pGrpBase->GetGraphType() != GT_BONG || (m_ObjEnv & ENV_LOG))
		return;

	tmpS = pGrpBase->GetYposValue(pt);
	int pos = 0;
	pos = tmpS.Find(':');
	if (pos != -1)
		tmpS = tmpS.Mid(pos+1);
	pToolBase->SetDisplayString(tmpS);
}

bool CGrpObject::DispatchDup(char* gData, int& gDataL)
{
	int	UpLen;
	char	*pUpGraph = m_DataFormat->GetGrpHeader(&UpLen);
	struct _ugraphH *pUgraphH = (struct _ugraphH *)pUpGraph;
	struct _dgraphH *pDgraphH = (struct _dgraphH *)gData;
	struct _fgraph *pFgraph = m_DataFormat->GetGrpFileInfo();

	CGrpBase*	pGrpBase;
	int	MaxKey = 0;
	for (int ii = m_GraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pGrpBase = m_GraphQue.GetAt(ii);

		if (pGrpBase->GetDataKey() > MaxKey)
			MaxKey = pGrpBase->GetDataKey();
	}

	struct _dataH	*pDataH = (struct _dataH *)&gData[sz_DGRAPHH];
	pDataH->key = MaxKey + 1;

	if (!AddGraphData(pDgraphH->index, 1, &gData[sz_DGRAPHH], gDataL))
	{
		TRACE("Invalid Graph DataH...\n");
		SetGraphError(AGENODATA);
		return false;
	}

	struct _graph	graph;
	CopyMemory(&graph, pFgraph->graph, sz_GRAPH);

	graph.tick &= ~TK_BOTTOM;
	if (graph.tick & TK_LEFT && graph.tick & TK_RIGHT)
		graph.tick &= ~TK_LEFT;

	graph.key = MaxKey + 1;
	graph.value[0] = 0;

	pGrpBase = AddGraphQue((char *)&graph, true, true);
	if (pGrpBase == (CGrpBase* ) 0)
	{
		TRACE("Invalid graph Information..\n");
		SetGraphError(AGENOGRAPH);
		return false;
	}

	int	DupCount = GetCountDupBong();
	COLORREF color = GetPaletteRGB(DUPCOLOR[pGrpBase->GetDupIndex()]);
	pGrpBase->SetBasicColor(color, -1, -1, false);
	pGrpBase->SetAdditionalType(DupCount-1);
	SetGrpEnv();

	m_ObjEnv &= ~ENV_DUP;
	ReDrawGraphObject();

	return true;
}


void CGrpObject::copyGraphChart(CPoint pt)
{
	m_bAppendMode = false;
	CGrpBase *pRgnGrp = SearchPointInGraph(pt);
	if (!pRgnGrp || !m_pSelLineGrp)
		return;

	if (pRgnGrp->GetGraphRegionID() == m_pSelLineGrp->GetGraphRegionID())
		return;

	struct _dupGraph dupGraph;
	if (m_pMgr->m_bTotal)
		m_pMgr->m_pView->SendMessage(GEV_VIEW, 
			MAKEWPARAM(viewDup, 2), (long)&dupGraph);

	struct _graph	graph;
	ZeroMemory(&graph, sz_GRAPH);

	int	gType = m_pSelLineGrp->GetGraphType();
	int	gKind = m_pSelLineGrp->GetGraphKind();
	int	dKey = m_pSelLineGrp->GetDataKey();
	int	rKey = pRgnGrp->GetGraphRegionID();

	int idx = 0;
	int	DupCount = GetCountRgnGraph(rKey, idx);
	if (DupCount > MAXDUP)
	{
		CString tmpS;
		tmpS.Format("중복 차트는 최대%d개 까지입니다.", MAXDUP);
		::MessageBox(NULL, tmpS, "확인", MB_ICONWARNING|MB_OK);
		return;
	}

	int	index = -1;
	int	val;
	switch (gType)
	{
	case GT_BONG:
		if (!m_pSelLineGrp->GetSelectLineInfo(index, val))
		{
			TRACE("selected line don't exist in CopyGraphChart()\n");
			return;
		}

		if (m_pSelLineGrp->GetGraphKind() == GK_EQUI
			|| m_pSelLineGrp->GetGraphKind() == GK_EVOL)
			return;
		gKind = GK_LG_LIN;
		switch (m_pSelLineGrp->GetGraphKind())
		{
		case GK_JPN:	case GK_USA:	case GK_LIN:
			if (index >= 0)
				gKind = GK_LG_PMA;
			break;
		case GK_NET:
		case GK_AD_NET:
			if (index >= 0)
				gKind = GK_LG_NET;
			break;

		case GK_LOG:
			if (index < 0)
				gKind = GK_LG_LOG;
			else
				gKind = GK_LG_LMA;
			break;
		case GK_ENVL:
		case GK_AD_ENVL:
			if (index >= 0)
				gKind = GK_LG_ENVL;
			break;
		case GK_PARB:
		case GK_AD_PARB:
		//	if (index >= 0)
				gKind = GK_LG_PARB;
			break;
		case GK_BOLB:
		case GK_AD_BOLB:
			if (index >= 0)
				gKind = GK_LG_BOLB;
			break;
		case GK_BALANCE:	// only single chart
			break;
		case GK_VOL:
		case GK_EVOL:
			if (index < 0)
			{
				TRACE("invalid line index[%d]\n", index);
				return;
			}

			gKind = GK_LG_VMA;
			break;
		case GK_AMT:
			if (index < 0)
			{
				TRACE("invalid line index[%d]\n", index);
				return;
			}

			gKind = GK_LG_AMA;
			break;
		case GK_LG_PMA:		case GK_LG_LMA:		case GK_LG_VMA:
		case GK_LG_AMA:		case GK_LG_LIN:		case GK_LG_LOG:
		case GK_LG_ENVL:	case GK_LG_PARB:	case GK_LG_BOLB:
		case GK_LG_NET:
			gKind = m_pSelLineGrp->GetGraphKind();
			break;
		case GK_TICK:
		case GK_STEP:
		case GK_MGJY:
			break;
		default:
			break;
		}
		break;
	case GT_LINE:
		break;
	default:
		return;
	}

	int ii = 0;
	if (m_pMgr->m_bTotal)
	{
		int sKey = m_pSelLineGrp->GetGraphRegionID();
		
		if (sKey-1 >= 0)
		{
			for (ii = MAXDUP-1; ii >= 0; ii--)
			{
				if (checkCmpGraph(dupGraph.dupInfo[sKey-1][ii], m_pSelLineGrp))
				{
					dupGraph.dupInfo[sKey-1][ii].key = 0;
					dupGraph.dupInfo[sKey-1][ii].kind = 0;
					dupGraph.dupInfo[sKey-1][ii].type = 0;
					dupGraph.dupInfo[sKey-1][ii].ichkcolor = 0;
					for (int kk = 0; kk < maxAVLINE; kk++)
					{
						dupGraph.dupInfo[sKey-1][ii].value[kk] = 0;
						dupGraph.dupInfo[sKey-1][ii].icolor[kk] = 0;
					}
					break;
				}
			}
		}

		for (ii = 0; ii < MAXDUP; ii++)
		{
			if (dupGraph.dupInfo[rKey-1][ii].key == 0)
			{
				switch (gKind)
				{
				case GK_LG_PMA:
				case GK_LG_LMA:
				case GK_LG_VMA:
					break;
				default:
					if (rKey-1 >= 0)
					{
						dupGraph.dupInfo[rKey-1][ii].key = dKey;
						dupGraph.dupInfo[rKey-1][ii].kind = gKind;
						dupGraph.dupInfo[rKey-1][ii].type = gType;
						dupGraph.dupInfo[rKey-1][ii].ichkcolor = 
							m_pSelLineGrp->m_ichkcolor;
						for (int kk = 0; kk < maxAVLINE; kk++)
						{
							dupGraph.dupInfo[rKey-1][ii].value[kk] = 
								m_pSelLineGrp->m_value[kk];
							dupGraph.dupInfo[rKey-1][ii].icolor[kk] = 
								m_pSelLineGrp->m_icolor[kk];
						}
					}
					break;
				}
				break;
			}	
		}

		m_pMgr->m_pView->SendMessage(GEV_VIEW, 
			MAKEWPARAM(viewDup, 1), (long)&dupGraph);
	}

	graph.type   = gType;
	graph.kind   = gKind;
	graph.key    = dKey;
	graph.region = rKey;
	graph.tick   = m_pSelLineGrp->GetGraphTickInfo();
	graph.tick   = graph.tick & ~(TK_HGRID | TK_VGRID | TK_LEFT | TK_RIGHT) | TK_BOTTOM;

	COLORREF	fg;;
	m_pSelLineGrp->GetBasicColor(&fg, NULL, NULL, false);
	graph.fgcolor = fg;
	graph.ApplyFormula = 0;
	for (ii = 0; ii < maxAVLINE; ii++)
	{
		graph.value[ii] = m_pSelLineGrp->m_value[ii];
		graph.icolor[ii] = m_pSelLineGrp->m_icolor[ii];
	}
	graph.ichkcolor = m_pSelLineGrp->m_ichkcolor;

	CGrpBase	*pGrpBase = AddGraphQue((char *) &graph, true);
	if (!pGrpBase)
	{
		TRACE("addGraphQue Error in CopyGraph\n");
		return;
	}
	
	// 수정 0624 ===>
	//DupCount++;
	pGrpBase->SetBasicColor(GetPaletteRGB(DUPCOLOR[idx]), -1, -1, false);
	pGrpBase->SetAdditionalType(idx);
	// 수정 0624 <===
	SyncGraphDisplay(m_pSelLineGrp, pGrpBase);
	
	if (gType == GT_BONG)
	{
		switch (gKind)
		{
		case GK_LG_PMA:	case GK_LG_LMA:	case GK_LG_VMA:	case GK_LG_AMA:  //case GK_LG_LIN:	//case GK_LG_BOLB: 
			pGrpBase->m_ValidMAIndex = index;
			if (pGrpBase->SetSelectLine(0))
			{
				m_pSelLineGrp->ResetSelLine();
				if (m_pSelLineGrp->IsAdditionalType() >= 0)
					RemoveGraphQue(m_pSelLineGrp);

				m_pSelLineGrp = pGrpBase;
			}
			break;
		case GK_LG_LOG:	case GK_LG_ENVL: case GK_LG_PARB:  case GK_LG_BOLB: case GK_LG_LIN:	
		case GK_LG_NET:
			pGrpBase->m_ValidMAIndex = MA_INVALID;
			if (pGrpBase->SetSelectLine(0))
			{
				m_pSelLineGrp->ResetSelLine();
				if (m_pSelLineGrp->IsAdditionalType() >= 0)
					RemoveGraphQue(m_pSelLineGrp);
				m_pSelLineGrp = pGrpBase;
			}
			break;
		default:
			break;
		}
	}
	else
	{
		pGrpBase->m_ValidMAIndex = MA_INVALID;
		if (pGrpBase->SetSelectLine(index))
		{
			m_pSelLineGrp->ResetSelLine();
			if (m_pSelLineGrp->IsAdditionalType() >= 0)
				RemoveGraphQue(m_pSelLineGrp);
			m_pSelLineGrp = pGrpBase;
		}
	}
	//DupCount++;
	ResizeGraphObject(m_hRatio, m_vRatio, true, true);
	return;
}

bool CGrpObject::checkCmpGraph(struct _dupInfo orgInfo, CGrpBase* cmpBase)
{
	if (orgInfo.kind == cmpBase->GetGraphKind()
		&& orgInfo.type == cmpBase->GetGraphType()
		&& orgInfo.ichkcolor == cmpBase->m_ichkcolor)
	{
		for (int ii = 0; ii < maxAVLINE; ii++)
		{
			if (orgInfo.icolor[ii] != cmpBase->m_icolor[ii])
				return false;
			if (orgInfo.value[ii] != cmpBase->m_value[ii])
				return false;
		}
		return true;
	}
	return false;
}


void CGrpObject::setDupGraph()
{
	struct _dupGraph dupGraph;
	
	m_pMgr->m_pView->SendMessage(GEV_VIEW, 
		MAKEWPARAM(viewDup, 2), (long)&dupGraph);

	CGrpBase* pBase = (CGrpBase *)NULL;
	
	struct _graph	graph;
	int	dupCount = 0;
	for (int ii = 0; ii < maxGRAPH; ii++)
	{
		pBase = (CGrpBase *)NULL;
		for (int kk = m_GraphQue.GetUpperBound(); kk >= 0; kk--)
		{
			pBase = m_GraphQue.GetAt(kk);
			if (pBase->GetGraphRegionID()-1 == ii)
				break;
		}
		if (pBase == NULL)
			continue;

		m_bAppendMode = false;
		
		for (int jj = 0; jj < MAXDUP; jj++)
		{
			if (dupGraph.dupInfo[ii][jj].kind == 0)
				continue;
			
			memset(&graph, 0x00, sz_GRAPH);
			graph.type   = dupGraph.dupInfo[ii][jj].type;
			graph.kind   = dupGraph.dupInfo[ii][jj].kind;
			graph.key    = pBase->GetDataKey();
			graph.region = pBase->GetGraphRegionID();
			graph.tick   = pBase->GetGraphTickInfo();
			graph.tick &= ~(TK_HGRID | TK_VGRID | TK_LEFT | TK_RIGHT | TK_BOTTOM);	

			COLORREF	fg;
			pBase->GetBasicColor(&fg, NULL, NULL, false);
			graph.fgcolor = fg;
			graph.ichkcolor = dupGraph.dupInfo[ii][jj].ichkcolor;
			graph.ApplyFormula = 0;
			for (int kk = 0; kk < maxAVLINE; kk++)
			{
				graph.value[kk] = dupGraph.dupInfo[ii][jj].value[kk];
				graph.icolor[kk] = dupGraph.dupInfo[ii][jj].icolor[kk];
			}
			

			int	region = pBase->GetGraphRegionID();
			dupCount = 0;
			for (kk = m_GraphQue.GetUpperBound(); kk >= 0; kk--)
			{
				CGrpBase *pGrpBase = m_GraphQue.GetAt(kk);
				if (pGrpBase->IsAdditionalType() >= 0 
					&& region == pGrpBase->GetGraphRegionID())
				{
					dupCount++;
				}
			}
			
			CGrpBase* pGrpAppend = AddGraphQue((char *) &graph, true);
			if (pGrpAppend)
			{
				pGrpAppend->SetAdditionalType(dupCount+1);
				COLORREF fgColor = GetPaletteRGB(DUPCOLOR[pGrpAppend->GetDupIndex()]);//GetPaletteRGB(graph.fgcolor);
				pGrpAppend->SetBasicColor(fgColor, -1, -1, false);
				SyncGraphDisplay(pBase, pGrpAppend);
			}
		}
	}
}

void CGrpObject::graphDup(CGrpBase* pBase, CPoint pt)
{
	struct _dupSet dupSet;
	memset(&dupSet, 0x00, sz_DUPSET);
	dupChartList(pBase->GetGraphRegionID(), (char *)&dupSet);
	
	struct _addGrp	addGrp;
	memset(&addGrp, 0x00, sz_ADDGRP);
	if (!axAppendChart((char *)&dupSet, (char *)&addGrp, false))
		return;

	deleteDupChart(pBase->GetGraphRegionID());
	pBase = SearchPointInGraph(pt);
	if (pBase == (CGrpBase *) 0)
		return;

	struct _dupGraph dupGraph;
	struct _dupInfo  dupData;
	if (m_pMgr->m_bTotal)
		m_pMgr->m_pView->SendMessage(GEV_VIEW, 
			MAKEWPARAM(viewDup, 2), (long)&dupGraph);

	if (dupSet.dupInfo[0].kind == 0 && dupSet.dupInfo[0].type == 0)
	{
		if (m_pMgr->m_bTotal)
		{
			for (int ii = 0; ii < MAXDUP; ii++)
			{
				memset(&dupData, 0x00, sz_DUPINFO);
				dupGraph.dupInfo[pBase->GetGraphRegionID()-1][ii] = dupData;
			}
			m_pMgr->m_pView->SendMessage(GEV_VIEW, 
				MAKEWPARAM(viewDup, 1), (long)&dupGraph);
		}
	
		ResizeGraphObject(m_hRatio, m_vRatio, true, true);
		return;
	}

	CGrpBase* pGrpBong;
	struct _graph	graph;
	int	dupCount = 0;
	for (int ii = 0; ii < MAXDUP; ii++)
	{
		if (dupSet.dupInfo[ii].kind == 0 
			&& dupSet.dupInfo[ii].type == 0)
			continue;

		memset(&graph, 0x00, sz_GRAPH);
		graph.type   = dupSet.dupInfo[ii].type;
		graph.kind   = dupSet.dupInfo[ii].kind;
		graph.key    = pBase->GetDataKey();
		if ((pGrpBong = ExistBongTypeGraph()) != (CGrpBase *) 0)
			graph.key = pGrpBong->GetDataKey();

		graph.region = dupSet.dupInfo[ii].key;//pBase->GetGraphRegionID();
		graph.tick   = pBase->GetGraphTickInfo();
		graph.tick &= ~(TK_HGRID | TK_VGRID | TK_LEFT | TK_RIGHT | TK_BOTTOM);

		COLORREF	fg;
		pBase->GetBasicColor(&fg, NULL, NULL, false);
		graph.fgcolor = fg;
		graph.ichkcolor = dupSet.dupInfo[ii].ichkcolor;
		graph.ApplyFormula = 0;
		for (int jj = 0; jj < maxAVLINE; jj++)
		{
			graph.value[jj] = dupSet.dupInfo[ii].value[jj];
			graph.icolor[jj] = dupSet.dupInfo[ii].icolor[jj];
		}

		dupCount = 0;
		int	region = pBase->GetGraphRegionID();
		for (jj = m_GraphQue.GetUpperBound(); jj >= 0; jj--)
		{
			CGrpBase *pGrpBase = m_GraphQue.GetAt(jj);
			if (pGrpBase->IsAdditionalType() >= 0 && region == pGrpBase->GetGraphRegionID())
				dupCount++;
		}
		CGrpBase* pGrpAppend = AddGraphQue((char *) &graph, true);
		if (pGrpAppend)
		{
			pGrpAppend->SetAdditionalType(dupCount + 1);
			COLORREF fgColor = GetPaletteRGB(DUPCOLOR[pGrpAppend->GetDupIndex()]);
			pGrpAppend->SetBasicColor(fgColor, -1, -1, false);
			SyncGraphDisplay(pBase, pGrpAppend);
		}
	}

	
	if (m_pMgr->m_bTotal)
	{
		for (ii = 0; ii < MAXDUP; ii++)
		{
			memset(&dupData, 0x00, sz_DUPINFO);
			dupGraph.dupInfo[pBase->GetGraphRegionID()-1][ii] = dupData;
		}
		CopyMemory(&dupGraph.dupInfo[pBase->GetGraphRegionID()-1], &dupSet, sz_DUPSET);
		m_pMgr->m_pView->SendMessage(GEV_VIEW, 
				MAKEWPARAM(viewDup, 1), (long)&dupGraph);
	}
	ResizeGraphObject(m_hRatio, m_vRatio, true, true);
}


void CGrpObject::GetBongMinMax(int regionID, double& dMin, double& dMax)
{
	CGrpBase* pGrpBase;
	double cMin, cMax;
	
	cMin = INT_MAX;
	cMax = INT_MIN;
	for (int ii = 0; ii < m_GraphQue.GetSize(); ii++)
	{
		pGrpBase = m_GraphQue.GetAt(ii);

		if (pGrpBase->GetGraphRegionID() != regionID)
			continue;
		if (pGrpBase->GetGraphType() != GT_BONG)
			continue;
		if (pGrpBase->GetDupIndex() != 0)
			continue;
		switch (pGrpBase->GetGraphKind())
		{
		case GK_VOL:
		case GK_EVOL:
			continue;
		}
		pGrpBase->GetMinMaxValue(cMin, cMax);
		if (cMin == INT_MAX || cMax == INT_MIN)
			continue;
		dMin = cMin;
		dMax = cMax;
		return;
	}
}
