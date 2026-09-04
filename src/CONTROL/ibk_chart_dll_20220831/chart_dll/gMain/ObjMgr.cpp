// ObjMgr.cpp: implementation of the CObjMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axisGMain.h"
#include "ObjMgr.h"
#include "ObjEvent.h"
#include "GrpGrid.h"
#include "DataFormat.h"
#include "CrossLine.h"

#include "../gData/DataMgr.h"
#include "../gIndc/GraphMgr.h"
#include "../gTool/ToolMgr.h"

#include "UserInputDlg.h"
#include "DaedeungDlg.h"

#include "PnChart.h"
#include "../AxMPattern/ctt/cttkit.h"
#include "../AxMPattern/AxPattern.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ANAL_NONE		-1

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjMgr::CObjMgr(CWnd* pwndView, CPnChart* pwndPnChart, char* pcEnvInfo, COLORREF clrBack, CFont* pfont)
{
	m_pGMainApp = (CAxisGMainApp *)AfxGetApp();
	m_pwndView = pwndView;
	m_pwndPnChart = pwndPnChart;
	m_crBG = clrBack;
	m_pFont = pfont;

	struct _envInfo* pEnvInfo = (struct _envInfo*)pcEnvInfo;
	char* mapInfo = (char *)pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewChartInfo, getMapInfo), 0);
	struct _interChart *pInter = (struct _interChart *)mapInfo;

	m_pDataFormat = NULL;
	m_pObjEvent = NULL;
	m_pDataMgr = NULL;
	m_pGrpGrid = NULL;
	m_rectGrp.SetRectEmpty();
	m_rectObj.SetRectEmpty();

	m_pDataFormat = new CDataFormat(this);
	m_pDataFormat->SetFgraph(pcEnvInfo, &mapInfo[sz_INTERCHART], true);

	
	CreateGrid(true);

	if (pEnvInfo->display.dwDspOption & DO_NOCROSS)
		m_pCrossLine = new CCrossLine(m_pwndView, this, false);
	else
		m_pCrossLine = new CCrossLine(m_pwndView, this, true);

	m_bDrawAll = true;
	m_iCoDrawCnt = 0;
//	m_bDrawMouse = false;

//	m_pGMainApp = (CAxisGMainApp *)AfxGetApp();
	CClientDC	dc(m_pwndPnChart);
	m_psdcBitmap = new SMemDC(&dc, (pEnvInfo->wEnvOption & EO_MAP));
	m_pObjEvent = new CObjEvent(pwndPnChart, this);
	m_pDataMgr = new CDataMgr(m_pwndPnChart);
	m_bDrawAll = true;

	m_pObjEvent->SetBookMark(pEnvInfo->ulist);

	m_pCoDraw = NULL;

	m_arGraphQue.RemoveAll();
	m_mapRegion.RemoveAll();

	m_rcCrossX.SetRectEmpty();
	m_rcCrossY.SetRectEmpty();
	m_strCrossX = m_strCrossY = _T("");

	m_bTableMode = false;
	m_bOrderMode = false;

	m_patternUP = FALSE;
	m_patternDN = FALSE;
	m_pPattern = NULL;
	m_prevPatternDate.Empty();

	m_bmpMode = FALSE;
	m_bmpPatternBearUp = m_bmpPatternBearDn = m_bmpPatternBullUp = m_bmpPatternBullDn = NULL;
	if (!(pEnvInfo->wEnvOption & EO_MAP))	// 종합차트에서만 사용
	{
		CString bmpPath = (LPCTSTR)m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, getRootPath));

		m_bmWidth = m_bmHeight = 0;
		if (!bmpPath.IsEmpty()) 
		{
			bmpPath += "\\Image\\";

			m_bmpPatternBearUp = (HBITMAP)LoadImage(NULL, bmpPath + "GTP_BearUp.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			m_bmpPatternBearDn = (HBITMAP)LoadImage(NULL, bmpPath + "GTP_BearDn.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			m_bmpPatternBullUp = (HBITMAP)LoadImage(NULL, bmpPath + "GTP_BullUp.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			m_bmpPatternBullDn = (HBITMAP)LoadImage(NULL, bmpPath + "GTP_BullDn.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

			if (m_bmpPatternBearUp && m_bmpPatternBearDn && m_bmpPatternBullUp && m_bmpPatternBullDn) 
			{
				m_bmpMode = TRUE;
				BITMAP bitmap;
				ZeroMemory(&bitmap, sizeof(bitmap));

				CBitmap bmp;
				bmp.Attach(m_bmpPatternBearUp);
				bmp.GetBitmap(&bitmap);
				m_bmWidth = bitmap.bmWidth;
				m_bmHeight = bitmap.bmHeight;
				bmp.Detach();
			}
		}
	}

	m_patternToolTip = new CToolTip(m_pwndView, NULL);
	m_patternToolTip->Create();

	m_analStart = m_analEnd = ANAL_NONE;
	m_mouseDown = FALSE;

	m_analMode = FALSE;

	m_analNotifyMessageHandler = NULL;
	m_analNotifyMessageID = NULL;

	m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, tableChart), m_bTableMode);
}

CObjMgr::~CObjMgr()
{
	RemoveGrpComponent();

	if (m_pObjEvent)	delete m_pObjEvent;
	if (m_pDataMgr)		delete m_pDataMgr;

	if (m_pGrpGrid)		{	delete m_pGrpGrid;		m_pGrpGrid = NULL;	}
	if (m_pCrossLine)	{	delete m_pCrossLine;	m_pCrossLine = NULL;	}
	if (m_pDataFormat)	{	delete m_pDataFormat;	m_pDataFormat = NULL;	}
	if (m_pCoDraw)		{	delete[] m_pCoDraw;	m_pCoDraw = NULL;	}

	if(m_psdcBitmap)
	{
		m_psdcBitmap->FreeDC();
		delete m_psdcBitmap;
	}

	if (m_patternToolTip) 
	{
		DeletePatternToolTip();
	}

	ClearPattern();

	if (m_bmpPatternBearUp)	DeleteObject(m_bmpPatternBearUp);
	if (m_bmpPatternBearDn)	DeleteObject(m_bmpPatternBearDn);
	if (m_bmpPatternBullUp)	DeleteObject(m_bmpPatternBullUp);
	if (m_bmpPatternBullDn)	DeleteObject(m_bmpPatternBullDn);
}

void CObjMgr::RemoveGrpComponent(bool bClearAll)
{
	m_cs.Lock();

	RemoveGraphQue();
	if (bClearAll)
		RemoveGraphData();
	RemoveGraphRegion();

	if (m_pObjEvent)
	{
		m_pObjEvent->Init();
		m_pObjEvent->SetStateCursor();
	}

	if (m_bTableMode)
		m_pwndPnChart->SendMessage(GEV_TABLE, tbItem, NULL);

	if (m_pCoDraw)
	{
		delete[] m_pCoDraw;
		m_pCoDraw = NULL;
	}
	m_iCoDrawCnt = 0;

	m_cs.Unlock();
}

void CObjMgr::ObjectClear()
{
	RemoveGrpComponent();
	ReDrawGraphObject(true);
}

char* CObjMgr::GetGrpHeader(int* piLen)
{
	return m_pDataFormat->GetGrpHeader(piLen); 
}

char* CObjMgr::GetGrpHeader2(int* piLen)
{
	return m_pDataFormat->GetGrpHeader2(piLen); 
}

char* CObjMgr::GetGrpHeaderS(int* piLen)
{
	return m_pDataFormat->GetGrpHeaderS(piLen); 
}

struct _envInfo* CObjMgr::GetEnvInfo()
{
	return m_pDataFormat->GetEnvInfo();
}

char* CObjMgr::GetMapInfo()
{
	int 	rcnt, gcnt;
	return m_pDataFormat->GetSaveGrpInfo(rcnt, gcnt);
}

char* CObjMgr::GetMapInter()
{
	int	rcnt, gcnt;
	char *pGrpInfo = m_pDataFormat->GetGrpInfo(rcnt, gcnt);

	char *pInfo = new char[sz_INTERCHART + SZ_REGION*rcnt + SZ_GRAPH*gcnt];
	struct _interChart	*interChart = (_interChart *)pInfo;
	interChart->rgncnt = rcnt;
	interChart->grpcnt = gcnt;
	CopyMemory(&pInfo[sz_INTERCHART] , pGrpInfo, SZ_REGION*rcnt + SZ_GRAPH*gcnt);

	return pInfo;
}

int CObjMgr::DispatchGraphData(struct _trData* pTD)
{
	RemoveGrpComponent();
	ClearPattern();

	if (pTD->iLen[0] == 0 || !pTD->pcData[0])
	{
		return 0;
	}

	if (m_pGrpGrid)
		m_pGrpGrid->ShowData("", NULL);

	int iGCnt, iRCnt;
	char* pcGrpInfo = m_pDataFormat->GetGrpInfo(iRCnt, iGCnt);
	struct _envInfo* peiNow = (struct _envInfo*)m_pDataFormat->GetEnvInfo();

	AddGraphRegion(iRCnt, pcGrpInfo);

	m_iDtIndex = peiNow->datainfo.btIndex;

	int iValidDataCount = 0;
	if (!AddGraphData(pTD, iValidDataCount))
	{
		//Debug("Invalid Graph DataH");
		RemoveGrpComponent();
		ResizeGraphObject();
		return 0;
	}
	if (!AddGraphQue(iGCnt, &pcGrpInfo[iRCnt * SZ_REGION], false))
	{
		//Debug("Invalid graph Information..");
		RemoveGrpComponent();
		ResizeGraphObject();
		return 0;
	}

	ReviseTick();
	ResizeGraphObject();
	ReDrawGraphObject(true);
/*
	// 2006.07.25 schbang : 당일체크시 실제영역만 보여주기 위해
	if (m_iDtIndex == GI_MINUTE || m_iDtIndex == GI_TICK)
	{
		int iUnit = 0;
		switch (peiNow->datainfo.btUnit)
		{
		case GU_FUTURE:
		case GU_OPTION:
		case GU_COFUT:
		case GU_COOPT:
			iUnit = GU_FUTURE - GU_CODE;
			break;
		case GU_CODE:
		case GU_INDEX:	
			iUnit = m_pDataFormat->GetEnvInfo()->datainfo.btUnit - GU_CODE;
			break;
		case GU_FOREIGN:
			iUnit = 3;
			break;
		default:
			break;
		}
		if (peiNow->datainfo.aaDayInfo[iUnit][m_iDtIndex - 1].cToday == 1)
			ChangeDisplay(iValidDataCount);
	}
*/

	if (m_bTableMode)
	{
		GetTableData();
		m_pwndPnChart->SendMessage(GEV_TABLE, tbItem, long(m_strData.operator LPCTSTR()));
	}

	CIndcBase* pindcMain = GetMainGraph();
	if (pindcMain)
	{
		int iDispDay = 1;
		switch (pindcMain->GetGraphKind())
		{
			case GK_PNF:
			case GK_THREE:
			case GK_SWING:
			case GK_PV:
			case GK_CLOCK:
			case GK_KAGI:
			case GK_RENKO:
//				m_pDataMgr->SetNoDeleteData(pindcMain->GetDataKey(), true);
				break;
			default:
// 2007.01.04 schbang : 막기
/*
				if (peiNow->datainfo.cToday == 1 && (m_iDtIndex == GI_MINUTE || m_iDtIndex == GI_TICK))
				{
					if (iValidDataCount > 0)
						iDispDay = iValidDataCount;
					else
					{
						iDispDay = iValidDataCount = 1;
					}
					
					ChangeDisplay(iValidDataCount);
				}
				else {
*/
					iDispDay = pindcMain->GetDisplayDay();
//				}
				if (iDispDay <= 0)
					iDispDay = m_pDataFormat->GetDispDay();

				m_pDataFormat->SaveDay(iDispDay, true);
				break;
		}

		return iDispDay;		
	}

	return 1;
}

int CObjMgr::DispatchGraphDataS(struct _trData* pTD)
{
	if (!AddGraphDataS(pTD))
		return 0;

	return 1;
}

bool CObjMgr::RealtimeGraphData(char* pcRTData)
{
	CString slog;
	if (m_arGraphQue.GetSize() <= 0)
		return false;

	CString	sRTMData(pcRTData);
	CString	sData;

	int iPos = sRTMData.Find('\t');
	if (iPos < 0)
		return false;

	CString sCode = sRTMData.Left(iPos++);
	sRTMData = sRTMData.Mid(iPos);
	if (m_iDtIndex >= GI_MINUTE)	// 분/틱차트는 모두처리 , 데이타 - 심볼 \t 최근데이타 \n....과거데이타 \n
	{
		CStringArray	strArray;
		strArray.RemoveAll();
		for (;!sRTMData.IsEmpty();)
		{
			iPos = sRTMData.Find("\n");
			if (iPos == -1)
			{
				sData = sRTMData;
				sRTMData = "";
			}
			else
			{
				sData = sRTMData.Left(iPos++);
				sRTMData = sRTMData.Mid(iPos);
			}
			strArray.Add(sData);
		}
		for (int ii = strArray.GetUpperBound(); ii >= 0; ii--)
		{
			if (!RealtimeProc(sCode, strArray.GetAt(ii)))
				return false;
		}
	}
	else	// 일주월은 최근만 처리
	{
		iPos = sRTMData.Find("\n");
		if (iPos == -1)
			sData = sRTMData;
		else
			sData = sRTMData.Left(iPos);

		RealtimeProc(sCode, sData);
	}

	return true;
}

bool CObjMgr::RealtimeProc(CString sCode, CString sData)
{
	bool	bIncrease = false;
	int iResponse = m_pDataMgr->ParseRealtime(sCode, sData, bIncrease);

	int ii;
	CToolBase* pToolBase;
	CIndcBase* pIndcBase;
	switch (iResponse)
	{
	case RTM_UPDATE:
		for (ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
		{
			pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);
			
// 수정 시작
			m_pwndPnChart->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, CTRL_UPDATE_SLIDE), NULL);
			if (bIncrease)
			{
				int iDispCount = pIndcBase->IncreaseCount();
				if (iDispCount)
				{
					m_pDataFormat->SaveDay(iDispCount, true);
					m_pwndPnChart->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, CTRL_SET_EDIT), iDispCount);
				}
			}
// 수정 끝
			pIndcBase->IsChangeMinMaxRTM(false);
		}
		ReviseMinMax();

		m_bDrawAll = true;
		m_pwndPnChart->Invalidate();
		RealTableData(false);
		break;
	case RTM_ALL:
		for (ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
		{
			pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);

// 수정 시작
			m_pwndPnChart->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, CTRL_UPDATE_SLIDE), NULL);
			if (bIncrease)
			{
				int iDispCount = pIndcBase->IncreaseCount();
				if (iDispCount)
				{
					m_pDataFormat->SaveDay(iDispCount, true);
					m_pwndPnChart->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, CTRL_SET_EDIT), iDispCount);
				}
			}
// 수정 끝
			pIndcBase->IsChangeMinMaxRTM(false);
		}
		ReviseMinMax();

		m_bDrawAll = true;
		m_pwndPnChart->Invalidate();
		RealTableData(false);
		break;
	case RTM_SHIFT:
		for (ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
		{
			pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);

// 수정 시작
			m_pwndPnChart->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, CTRL_UPDATE_SLIDE), NULL);
			if (bIncrease)
			{
				int iDispCount = pIndcBase->IncreaseCount();
				if (iDispCount)
				{
					m_pDataFormat->SaveDay(iDispCount, true);
					//m_pwndPnChart->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, CTRL_SET_EDIT), iDispCount);
					m_pwndPnChart->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipSetDisplay), iDispCount);
				}
			}
// 수정 끝
			pIndcBase->IsChangeMinMaxRTM(true, bIncrease);
		}

	
		for (ii = 0; ii <= m_pObjEvent->m_arToolQue.GetUpperBound(); ii++)
		{
			pToolBase = m_pObjEvent->m_arToolQue.GetAt(ii);
			pToolBase->IndexShift();
		}
		ReviseMinMax();

		m_bDrawAll = true;
		m_pwndPnChart->Invalidate();
		RealTableData(true);
		break;
	default:
		return false;
	}

	return true;
}

bool CObjMgr::ResizeGraphObject(bool bForceResize)
{
	if (DrawCntIsZero()) return false;

	CRect rectC;
	m_pwndPnChart->GetClientRect(&rectC);

	CRect rectTemp = rectC;
	m_rectObj = rectTemp;
	m_rectObj.top += 2;
	m_rectGrp = m_rectObj;

	struct _envInfo* penvinfo = (struct _envInfo*)m_pDataFormat->GetEnvInfo();

	if (bForceResize)
	{
		CalculateTk(TICK_LEFT);
		CalculateTk(TICK_RIGHT);
		CalculateTk(TICK_BOTTOM);

		if (penvinfo->display.dwDspOption & DO_DISPLT)
			m_rectGrp.left += m_aszTick[TICK_LEFT].cx;
		else
			m_rectGrp.left += 2;

		if (penvinfo->display.dwDspOption & DO_DISPRT)
			m_rectGrp.right -= m_aszTick[TICK_RIGHT].cx;
		else
			m_rectGrp.right -= 2;

		m_rectGrp.bottom -= int(m_aszTick[TICK_BOTTOM].cy * 1.5);

		m_arectXTick[TICK_LEFT].SetRect(m_rectObj.left, m_rectObj.top, m_rectGrp.left, m_rectGrp.bottom);
		m_arectXTick[TICK_RIGHT].SetRect(m_rectGrp.right, m_rectGrp.top, m_rectObj.right, m_rectGrp.bottom);
		m_arectXTick[TICK_BOTTOM].SetRect(m_rectObj.left, m_rectGrp.bottom, m_rectObj.right, m_rectObj.bottom);
	}

	struct _xRegion* pxRegion;
	int iMapKey;
	for (POSITION pos = m_mapRegion.GetStartPosition(); pos; )
	{
		m_mapRegion.GetNextAssoc(pos, iMapKey, pxRegion);
		pxRegion->xRect = m_rectGrp;
		pxRegion->xRect.top = m_rectGrp.top + int((m_rectGrp.Height() * pxRegion->pRect.top) / 10000.);
		pxRegion->xRect.bottom = m_rectGrp.top + int((m_rectGrp.Height() * pxRegion->pRect.bottom) / 10000.);
	}

	CIndcBase* pIndcBase;
	CRect rectTick[3];

	rectTick[TICK_LEFT] = m_arectXTick[TICK_LEFT];
	rectTick[TICK_RIGHT] = m_arectXTick[TICK_RIGHT];
	rectTick[TICK_BOTTOM] = m_arectXTick[TICK_BOTTOM];

	int iBalanceGap = 0;
	if (GetMainKind() == GK_BALANCE)
	{
		if (m_arGraphQue.GetSize() > 0)
		{
			pIndcBase = m_arGraphQue.GetAt(0);
			iBalanceGap = pIndcBase->GetValue();
		}
	}

	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);
		GetGrpRegion(pIndcBase->GetGraphRegionID(), rectTemp);
		rectTick[TICK_LEFT].top = rectTemp.top;
		rectTick[TICK_LEFT].bottom = rectTemp.bottom;
		rectTick[TICK_RIGHT].top = rectTemp.top;
		rectTick[TICK_RIGHT].bottom = rectTemp.bottom;
		//pIndcBase->ResizeGraph(m_rectObj, m_rectGrp, rectTemp, rectTick, 
		//	penvinfo->display.wLMargin, penvinfo->display.wRMargin, iBalanceGap);
		pIndcBase->ResizeGraph(m_rectObj, m_rectGrp, rectTemp, rectTick, 
			penvinfo->display.wLMargin, penvinfo->display.wRMargin, 
			penvinfo->display.btTopMargin, penvinfo->display.btBottomMargin, iBalanceGap);
	}
	ReviseMinMax();

	CToolBase* pToolBase;
	for (int ii = 0; ii <= m_pObjEvent->m_arToolQue.GetUpperBound(); ii++)
	{
		pToolBase = m_pObjEvent->m_arToolQue.GetAt(ii);
		pIndcBase = (CIndcBase *)pToolBase->GetGrpBase();
		GetGrpRegion(pIndcBase->GetGraphRegionID(), rectTemp);
		
		pToolBase->ResizeTool(rectTemp, m_rectGrp);
	}

	if (m_pCrossLine)
		m_pCrossLine->ResizeCrossLine(m_rectGrp);

	if (IsGridMode() && m_pGrpGrid)
		m_pGrpGrid->MoveGrid(CPoint(m_rectObj.left, m_rectObj.top));

	m_pObjEvent->AddSplitMap();
	if (m_arGraphQue.GetSize())
		m_pObjEvent->AddMarkMap();

	m_psdcBitmap->SetSize(rectC.Width(), rectC.Height());

	return true;
}

void CObjMgr::DrawGraphObject()
{
	CClientDC dc(m_pwndPnChart);
	m_cs.Lock();

	if (m_psdcBitmap->IsValid())
	{
		if (m_bDrawAll)
		{
			DrawChart(m_psdcBitmap);
			m_psdcBitmap->SBitBlt(&dc);
			DrawEtc(&dc);
		}
		else
		{
			m_psdcBitmap->SBitBlt(&dc);
			DrawEtc(&dc);
		}
	}
	else
	{
		DrawChart(&dc);
		DrawEtc(&dc);
	}

	m_bDrawAll = false;
//	m_bDrawMouse = false;

	m_cs.Unlock();
}

void CObjMgr::DrawChart(CDC *pDC)
{
	CRect rectC;
	m_pwndPnChart->GetClientRect(rectC);
	DrawEmpty(pDC, rectC);

	if (DrawCntIsZero())
		return;

	if (m_arGraphQue.GetSize() < 1)
		return;

	for (int ii = 0; ii < m_iCoDrawCnt; ii++)
	{
		m_pCoDraw[ii].tickx = 0;
		m_pCoDraw[ii].ticky = 0;
	}

	
	CIndcBase *pindcMain = GetMainGraph();
	if (pindcMain)
	{
		struct _envInfo	*envInfo = (struct _envInfo*)m_pDataFormat->GetEnvInfo();
		// Box
		pindcMain->DrawTickBox(pDC, envInfo->display.rgbEdge, envInfo->display.rgbBG, envInfo->display.rgbChartEdge, envInfo->display.rgbChartBG);
		// X축의 세로선
		pindcMain->DrawTickVertical(pDC);
//		// Y축의 가로선
//		pindcMain->DrawTick(pDC);
	}

	// Tick 을 먼저 그림
	for (int ii = 0; ii < m_arGraphQue.GetSize(); ii++)
	{
		CIndcBase* pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);

		if (!pIndcBase->IsUnion())
		{
			// Y축의 가로선
			pIndcBase->DrawTick(pDC);
		}
	}

	// 기준선 정보가 있으면 기준선 먼저 그림. 
	if (m_pwndPnChart->HasStanLine()) 
	{
		m_pwndPnChart->DrawStanLine(pDC, GetMainGraph());
	}

	if (m_analMode) 
	{
		DrawAnalArea(pDC);
	}

	m_arGraphQueToDraw.RemoveAll();
	m_arGraphQueToDraw.Copy(m_arGraphQue);

	// 메인범례를 먼저
	for (int ii = 0; ii < m_arGraphQueToDraw.GetSize(); ii++)
	{
		CIndcBase* pIndcBase = (CIndcBase *) m_arGraphQueToDraw.GetAt(ii);
		if (pIndcBase == pindcMain && !pIndcBase->IsUnion())
		{
			pIndcBase->DrawLegend(pDC);
			m_arGraphQueToDraw.RemoveAt(ii);
			ii--;
		}
	}

	// 가격 이평 범례
	for (int ii = 0; ii < m_arGraphQueToDraw.GetSize(); ii++)
	{
		CIndcBase* pIndcBase = (CIndcBase *) m_arGraphQueToDraw.GetAt(ii);
		if (pIndcBase->GetGraphKind() == GK_PMA && !pIndcBase->IsUnion())
		{
			pIndcBase->DrawLegend(pDC);
			m_arGraphQueToDraw.RemoveAt(ii);
			ii--;
		}
	}

	// 거래량 범례
	for (int ii = 0; ii < m_arGraphQueToDraw.GetSize(); ii++)
	{
		CIndcBase* pIndcBase = (CIndcBase *) m_arGraphQueToDraw.GetAt(ii);
		if (pIndcBase->GetGraphKind() == GK_VOL && !pIndcBase->IsUnion())
		{
			pIndcBase->DrawLegend(pDC);
			m_arGraphQueToDraw.RemoveAt(ii);
			ii--;
		}
	}

	// 거래량 이평 범례
	for (int ii = 0; ii < m_arGraphQueToDraw.GetSize(); ii++)
	{
		CIndcBase* pIndcBase = (CIndcBase *) m_arGraphQueToDraw.GetAt(ii);
		if (pIndcBase->GetGraphKind() == GK_VMA && !pIndcBase->IsUnion())
		{
			pIndcBase->DrawLegend(pDC);
			m_arGraphQueToDraw.RemoveAt(ii);
			ii--;
		}
	}

	// 거래대금 범례
	for (int ii = 0; ii < m_arGraphQueToDraw.GetSize(); ii++)
	{
		CIndcBase* pIndcBase = (CIndcBase *) m_arGraphQueToDraw.GetAt(ii);
		if (pIndcBase->GetGraphKind() == GK_AMT && !pIndcBase->IsUnion())
		{
			pIndcBase->DrawLegend(pDC);
			m_arGraphQueToDraw.RemoveAt(ii);
			ii--;
		}
	}

	// 거래대금 이평 범례
	for (int ii = 0; ii < m_arGraphQueToDraw.GetSize(); ii++)
	{
		CIndcBase* pIndcBase = (CIndcBase *) m_arGraphQueToDraw.GetAt(ii);
		if (pIndcBase->GetGraphKind() == GK_AMA && !pIndcBase->IsUnion())
		{
			pIndcBase->DrawLegend(pDC);
			m_arGraphQueToDraw.RemoveAt(ii);
			ii--;
		}
	}

	m_arGraphQueToDraw.RemoveAll();
	m_arGraphQueToDraw.Copy(m_arGraphQue);


	// 매물차트를 먼저
	for (int ii = 0; ii < m_arGraphQueToDraw.GetSize(); ii++)
	{
		CIndcBase* pIndcBase = (CIndcBase *) m_arGraphQueToDraw.GetAt(ii);
		if (pIndcBase->GetGraphKind() == GK_AVOL)
		{
			pIndcBase->DrawGraph(pDC);
			m_arGraphQueToDraw.RemoveAt(ii);
			ii--;
		}
	}

	if (m_patternUP || m_patternDN) 
		DrawPattern(pDC, GetMainGraph());

	// 거래량 쪽을 다음에
	for (int ii = 0; ii < m_arGraphQueToDraw.GetSize(); ii++)
	{
		CIndcBase* pIndcBase = (CIndcBase *) m_arGraphQueToDraw.GetAt(ii);
		switch (pIndcBase->GetGraphKind())
		{
		case GK_VOL:
		case GK_AMT:
			pIndcBase->DrawGraph(pDC);
			m_arGraphQueToDraw.RemoveAt(ii);
			ii--;
			break;
		}
	}


	// 그물차트를 다음에
	for (int ii = 0; ii < m_arGraphQueToDraw.GetSize(); ii++)
	{
		CIndcBase* pIndcBase = (CIndcBase *) m_arGraphQueToDraw.GetAt(ii);
		if (pIndcBase->GetGraphKind() == GK_NET)
		{
			pIndcBase->DrawGraph(pDC);
			m_arGraphQueToDraw.RemoveAt(ii);
			ii--;
		}
	}


	// 드래그된 보조지표 (과매도, 과매수가 가능한 것만)
	for (int ii = 0; ii < m_arGraphQueToDraw.GetSize(); ii++)
	{
		CIndcBase* pIndcBase = (CIndcBase *) m_arGraphQueToDraw.GetAt(ii);
		switch (pIndcBase->GetGraphKind())
		{
		case GK_RSIWEIGHT: // 20090923 : CHANGE : RSI Weight 지표 추가
		case GK_RSI:
		case GK_CCI:
		case GK_GAP:
		case GK_STOCHF:
		case GK_STOCHS:
		case GK_WPR:
		case GK_MFI:
		case GK_VR:
		case GK_NPSY:
		case GK_PSY:
			pIndcBase->DrawGraph(pDC);
			m_arGraphQueToDraw.RemoveAt(ii);
			ii--;
			break;
		}
	}

	// 나머지
	for (int ii = 0; ii < m_arGraphQueToDraw.GetSize(); ii++)
	{
		CIndcBase* pIndcBase = (CIndcBase *) m_arGraphQueToDraw.GetAt(ii);
		pIndcBase->DrawGraph(pDC);
	}

	// 오른쪽에 현재가 표시 (색깔 사각형)
	if (pindcMain)
	{
		pindcMain->DrawCurPrice(pDC);
		int	mkind = pindcMain->GetGraphKind();
		switch (m_iDtIndex)
		{
		case GI_MINUTE:
		case GI_TICK:
			switch (mkind)
			{
			case GK_JPN:
			case GK_BAR:
			case GK_LIN:
			case GK_POLE:
			case GK_BALANCE:
			case GK_AVOL:
			case GK_EQV:
			case GK_CDV:
				pindcMain->DrawPrePrice(pDC);
				pindcMain->DrawExtPrice(pDC, m_bOrderMode);
				break;
			}
			break;
		default:
			switch (mkind)
			{
			case GK_JPN:
			case GK_BAR:
			case GK_LIN:
			case GK_POLE:
			case GK_BALANCE:
			case GK_AVOL:
			case GK_EQV:
			case GK_CDV:
				if (m_bOrderMode)
					pindcMain->DrawPrePrice(pDC);
				pindcMain->DrawExtPrice(pDC, m_bOrderMode);
				break;
			}
			break;
		}
	}

	m_pObjEvent->DrawSplitLine(pDC);
	m_pObjEvent->DrawDeleteMark(pDC);
	m_pObjEvent->DrawSelectArea(pDC);
	m_pObjEvent->DrawTopLine(pDC);	// 추가

	// 분석도구
	CToolBase* pToolBase;
	for (int ii = m_pObjEvent->m_arToolQue.GetUpperBound(); ii >= 0; ii--)
	{
		pToolBase = (CToolBase *) m_pObjEvent->m_arToolQue.GetAt(ii);
		pToolBase->DrawTool(pDC);
	}

}

void CObjMgr::DrawEtc(CDC *pDC)
{
	if (DrawCntIsZero()) return;

	DrawCrossStr(pDC);

	if (!m_pObjEvent)
		return;

	//if (m_pObjEvent->m_ptSSize.x >= 0)
	if (m_pObjEvent->GetStartPoint().x >= 0)
	{
		CPoint	pt;
		GetCursorPos(&pt);
		m_pwndPnChart->ScreenToClient(&pt);
		m_pObjEvent->DrawMouseArea(pt, false, pDC);
	}		

	if (m_pCrossLine)
		m_pCrossLine->DrawCrossLine(pDC);
}

void CObjMgr::DrawEmpty(CDC* pDC, CRect rc)
{
	if (!m_pDataFormat)
		return;

	int sMode = pDC->SetBkMode(TRANSPARENT);
	struct _envInfo	*envInfo = (struct _envInfo*)m_pDataFormat->GetEnvInfo();
	COLORREF	color = envInfo->display.rgbBG;//m_crBG;

	CPen	*cPen = m_pGMainApp->GetPen(m_pwndView, PS_SOLID, 1, color);
	CBrush	*cBrush = m_pGMainApp->GetBrush(m_pwndView, color);
	CPen	*sPen = pDC->SelectObject(cPen);
	CBrush	*sBrush = pDC->SelectObject(cBrush);

	pDC->Rectangle(&rc);

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
	pDC->SetBkMode(sMode);
}

void CObjMgr::DrawCrossStr(CDC *pDC)
{
	if (m_strCrossX.IsEmpty() || m_strCrossY.IsEmpty())
		return;

	CPen	*cPen = m_pGMainApp->GetPen(m_pwndView, PS_SOLID, 1, RGB(0,0,0));
	CPen	*sPen = pDC->SelectObject(cPen);
	CBrush	*cBrush = m_pGMainApp->GetBrush(m_pwndView, RGB(253, 255, 230));
	CBrush	*sBrush = pDC->SelectObject(cBrush);
	pDC->Rectangle(m_rcCrossY);
	pDC->Rectangle(m_rcCrossX);
	pDC->SelectObject(sBrush);
	pDC->SelectObject(sPen);

	int sMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF sColor = pDC->SetTextColor(RGB(0,0,0));
	CFont	*sFont = pDC->SelectObject(m_pFont);

	pDC->DrawText(m_strCrossY, m_rcCrossY, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->DrawText(m_strCrossX, m_rcCrossX, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	pDC->SetBkMode(sMode);
	pDC->SetTextColor(sColor);
	pDC->SelectObject(sFont);
}

void CObjMgr::ReDrawGraphObject(bool bAll, bool bMouse)
{
	m_bDrawAll = bAll;
//	m_bDrawMouse = bMouse;
	DrawGraphObject();
}

bool CObjMgr::AddGraphData(struct _trData* pTD, int& riValidDataCount)
{
	int iGDataLen = pTD->iLen[0] - pTD->iSiseLen[0];
	//if (iGDataLen - SZ_DATAH <= 2)
	if (iGDataLen - SZ_DATAH < 0)
		return false;

	char* pcGData = pTD->pcData[0] + pTD->iSiseLen[0];
	struct _dataH* pDataH = (struct _dataH*)pcGData;

	int iDtKey = pDataH->dkey;
	int iCount = 0;
	int iDummy = 0;

	int iIndex = m_pDataFormat->GetEnvInfo()->datainfo.btIndex;

	bool bNoDeleteData = false;
	int iRequest = m_pDataFormat->GetRequestCount();
/*	// 당일 차트
	if (m_pDataFormat->GetEnvInfo()->datainfo.cToday && (iIndex == GI_MINUTE || iIndex == GI_TICK))
	{
		iCount = CharToInt(pDataH->count, sizeof(pDataH->count));
//iCount = 0;
		//iDummy = 0;
		iDummy = (iCount == 0 ? 1 : 0);
		m_pwndPnChart->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, SET_TOTAL_DAY), iCount);
		bNoDeleteData = true;

	}

	if (iRequest > 0)
	{
		iCount = CharToInt(pDataH->count, sizeof(pDataH->count));
		if (iRequest < iCount)
		{
			iCount = iRequest;
			iDummy = 0;
		}
		else
		{
			iDummy = iRequest - iCount;
		}
	}
	else
*/	{
		iCount = CharToInt(pDataH->count, sizeof(pDataH->count));
		iDummy = CharToInt(pDataH->dummy, sizeof(pDataH->dummy));
		m_pwndPnChart->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, SET_TOTAL_DAY), iCount + iDummy);
	}

	riValidDataCount = iCount;

	m_pDataMgr->CreateOrgData(pcGData);
	int iDtPos = SZ_DATAH;
	iGDataLen -= SZ_DATAH;

	char acTemp[128];
	m_pwndPnChart->SendMessage(GEV_SISE, MAKEWPARAM(siseData, siseDate), (long)acTemp);
	bool	bTodayData = m_pwndPnChart->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, GET_TODAY), 0)? true:false;
	int iParseLen = m_pDataMgr->AttachData(iDtKey, iCount, iDummy, pcGData + iDtPos, iGDataLen, acTemp, bTodayData);
	if (iParseLen < 0)
	{
		TRACE("data kind or data length Error...\n");
		return false;
	}                                                                                                                                                                                                                                                                                       
	else if (pTD->pcData[1])
	{
		pcGData = pTD->pcData[1];
		iGDataLen = pTD->iLen[1] - pTD->iSiseLen[1];

		iDtPos = 0;
		pDataH = (struct _dataH *)pcGData;
//		iDtKey = pDataH->dkey;

		iDtPos += SZ_DATAH;
		iGDataLen -= SZ_DATAH;

//		iCount = CharToInt(pDataH->count, sizeof(pDataH->count));
//		iDummy = CharToInt(pDataH->dummy, sizeof(pDataH->dummy));

		m_pDataMgr->AttachData2(iDtKey, iCount, iDummy, pcGData + iDtPos, iGDataLen, m_pDataFormat->GetOuntID2());
	}

	m_pwndPnChart->SendMessage(GEV_SISE, MAKEWPARAM(siseData, siseCurr), (long)acTemp);
	m_pDataMgr->SetCurr(iDtKey, acTemp);
	if (m_bOrderMode)
	{
		CString	sCode;
		m_pDataMgr->GetRtsCode(sCode);
		char	wb[16];

		strcpy_s(wb, sizeof(wb), sCode.Mid(1));
		CString	name = (char *)m_pwndView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, whichTYPE), (LPARAM)(LPCTSTR)wb);
		m_pDataMgr->SetStockKind(iDtKey, wb[0]);	//	if 1-거래소, 2-코스닥
	}

	char* pcStock = (char*)m_pwndPnChart->SendMessage(GEV_SISE, MAKEWPARAM(siseData, siseStock), 0);
	if (pcStock)
		m_pDataMgr->SetStock(iDtKey, pcStock);

	/**
	if (bNoDeleteData)
		m_pDataMgr->SetNoDeleteData(pDataH->dkey, true);
	**/

	return true;
}

bool CObjMgr::AddGraphDataS(struct _trData* pTD)
{
	CString	symbol, pDay;

	char* pcGData = pTD->pcData[0] + pTD->iSiseLen[0];
	int iGDataLen = pTD->iLen[0] - pTD->iSiseLen[0];

	if (iGDataLen - SZ_DATAH <= 2)
		return false;

	struct _dataH* pDataH = (struct _dataH*)pcGData;

	int iDKey = pDataH->dkey;
	//int request = m_pDataFormat->m_iRequestCnt;
	int iRequest = m_pDataFormat->GetRequestCount();
	int iCount;
	int iDummy;
	if (iRequest > 0)
	{
		iCount = CharToInt(pDataH->count, sizeof(pDataH->count));
		if (iRequest < iCount)
		{
			iCount = iRequest;
			iDummy = 0;
		}
		else
		{
			iDummy = iRequest - iCount;
		}
	}
	else
	{
		iCount = CharToInt(pDataH->count, sizeof(pDataH->count));
		iDummy = CharToInt(pDataH->dummy, sizeof(pDataH->dummy));
	}

	int iDatPos = SZ_DATAH;
	iGDataLen -= SZ_DATAH;
	//int iParseLen = m_pDataMgr->AttachData2(iDKey, iCount, iDummy, pcGData + iDatPos, iGDataLen, m_pDataFormat->m_iOutIDS);
	int iParseLen = m_pDataMgr->AttachData2(iDKey, iCount, iDummy, pcGData + iDatPos, iGDataLen, m_pDataFormat->GetOuntIDS());
	
	if (iParseLen < 0)
	{
		TRACE("single data Error...\n");
		return false;
	}

	if (m_arGraphQue.GetSize())
	{
		CIndcBase* pIndcBase = m_arGraphQue.GetAt(m_arGraphQue.GetUpperBound());
		pIndcBase->CalculateMinMax(true);
		CRect rc = pIndcBase->GetOrgDrawRect();
		rc.left = m_rectObj.left;
		rc.right = m_rectObj.right;
		ReDrawGraphObject(true);

		if (m_bTableMode)
		{
			GetTableData();
			m_pwndPnChart->SendMessage(GEV_TABLE, tbItem, long(m_strData.operator LPCTSTR()));
		}
	}

	return true;
}

void CObjMgr::RemoveGraphData()
{
	if (m_pDataMgr)
		m_pDataMgr->DetachData();
}

CIndcBase* CObjMgr::AddGraphQue(int iGrpCnt, char* pcData, bool bAppend, bool bUnion)
{
	struct _graph* pGraph = (struct _graph*)pcData;
	CIndcBase* pIndcBase = NULL;

	struct _envInfo	*penvinfo = GetEnvInfo();
	int iDispDay = m_pDataFormat->GetDispDay();

	if (!bAppend)
	{
		if(m_pDataFormat->GetMainGraph() == GK_AVOL)
		{
			struct _graph *graph = m_pDataFormat->GetAVolGrp();
			graph->btRgnKey = m_pDataFormat->GetMainGrpRegion();
			pIndcBase = CGraphMgr::CreateGraph(m_pwndView, m_pwndPnChart, m_pDataMgr, (char*)penvinfo, (char *)graph, iDispDay);
			m_arGraphQue.Add(pIndcBase);
		}
		else if (m_pDataFormat->GetMainGraph() == GK_BALANCE)
		{
			struct _graph *graph = m_pDataFormat->GetBalanceGrp();
			graph->btRgnKey = m_pDataFormat->GetMainGrpRegion();
			pIndcBase = CGraphMgr::CreateGraph(m_pwndView, m_pwndPnChart, m_pDataMgr, (char*)penvinfo, (char *)graph, iDispDay);
			m_arGraphQue.Add(pIndcBase);
		}
	}

	for (int ii = 0; ii < (!bAppend ? iGrpCnt : 1); ii++)
	{
		//*** temporary code
		if (pGraph[ii].btGType == 0 || pGraph[ii].btGType > GT_VARS)
			pGraph[ii].btGType = GT_LINE;
		//**
		if (pGraph[ii].btGType <= 0 || pGraph[ii].wGKind <= 0)
		{
			break;
		}

		int iDtKey = pGraph[ii].btDataKey;
 		if (!IsValidGraphHeader(iDtKey, pGraph[ii].btRgnKey))
		{
			TRACE("Graph key or region is invalid[%d][%d]\n", iDtKey, pGraph[ii].btRgnKey);
			continue;
		}

		pIndcBase = CGraphMgr::CreateGraph(m_pwndView, m_pwndPnChart, m_pDataMgr, (char*)penvinfo, (char*)&pGraph[ii], iDispDay);

		
		if (pIndcBase)
		{
			m_arGraphQue.Add(pIndcBase);
		}
	}

	return pIndcBase;
}

void CObjMgr::RemoveGraphQue()
{
	CIndcBase* pIndcBase = NULL;
	for (int ii = m_arGraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pIndcBase = m_arGraphQue.GetAt(ii);
		delete pIndcBase;
		pIndcBase = NULL;
	}
	m_arGraphQue.RemoveAll();

	if (m_pObjEvent)
		m_pObjEvent->m_pindcSelect = NULL;
}

bool CObjMgr::RemoveGraphQue(CIndcBase* pBase)
{
	CIndcBase* pTemp = (CIndcBase *)NULL;
	for (int ii = m_arGraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pTemp = m_arGraphQue.GetAt(ii);
		if (pBase == pTemp)
		{
			delete pTemp;
			pTemp = NULL;
			m_arGraphQue.RemoveAt(ii);
			return true;
		}
	}
	return false;
}

bool CObjMgr::RemoveGraphQue(int rgnKey)
{
	bool response = false;

	CIndcBase* pTemp;
	int	tempRgnKey;
	for (int ii = m_arGraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pTemp = m_arGraphQue.GetAt(ii);
		tempRgnKey = pTemp->GetGraphRegionID();

		if (rgnKey == tempRgnKey)
		{
			delete pTemp;
			pTemp = NULL;
			m_arGraphQue.RemoveAt(ii);
		}
	}

	return response;
}

void CObjMgr::AddGraphRegion(int iRgnCnt, char* pcData)
{
	int iMapKey;
	struct _xRegion* pxRegion;

	struct _region* pRegion;
	pRegion = (struct _region*)pcData;

	int iGrpWidth = m_rectGrp.Width();
	int iGrpHeight = m_rectGrp.Height();
	for (int ii = 0; ii < iRgnCnt; ii++)
	{
		if ((iMapKey = pRegion[ii].btRKey) <= 0)
			break;

		if (m_mapRegion.Lookup(iMapKey, pxRegion))
			continue;

		pxRegion = (struct _xRegion *)new char [sizeof (struct _xRegion)];

		pxRegion->xRect.left = m_rectGrp.left  + static_cast<int>( ( static_cast<double>(iGrpWidth) * static_cast<double>(pRegion[ii].wRLeft) )  / 10000. );
		pxRegion->xRect.right = m_rectGrp.left + static_cast<int>( (static_cast<double>(iGrpWidth)  * static_cast<double>(pRegion[ii].wRRight) ) / 10000. );
		pxRegion->xRect.top = m_rectGrp.top    + static_cast<int>( (static_cast<double>(iGrpHeight) * static_cast<double>(pRegion[ii].wRTop) )   / 10000. );
		pxRegion->xRect.bottom = m_rectGrp.top + static_cast<int>( (static_cast<double>(iGrpHeight) * static_cast<double>(pRegion[ii].wRBottom) )/ 10000. );

		pxRegion->pRect.left = pRegion[ii].wRLeft;
		pxRegion->pRect.right = pRegion[ii].wRRight;
		pxRegion->pRect.top = pRegion[ii].wRTop;
		pxRegion->pRect.bottom = pRegion[ii].wRBottom;

		m_mapRegion.SetAt(iMapKey, pxRegion);
	}
}

void CObjMgr::RemoveGraphRegion()
{
	struct _xRegion* xRegion;
	int	mapKey;
	
	for (POSITION pos = m_mapRegion.GetStartPosition(); pos; )
	{
		m_mapRegion.GetNextAssoc(pos, mapKey, xRegion);
		delete[] xRegion;
		xRegion = NULL;
	}
	m_mapRegion.RemoveAll();
}

bool CObjMgr::RemoveGraphRegion(int iRgnKey)
{
	struct _xRegion* pxRegion;

	if (m_mapRegion.Lookup(iRgnKey, pxRegion))
	{
		delete[] pxRegion;
		pxRegion = NULL;
		m_mapRegion.RemoveKey(iRgnKey);
		return true;
	}
	return false;
}

void CObjMgr::SetDisplayGraph(int start, int end)
{
	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		CIndcBase *pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);
		pIndcBase->SetDisplayPos(start);
		pIndcBase->SetDisplayDay(end - start + 1);
		pIndcBase->SetDisplayEnd(end + 1);
		pIndcBase->CalculateMinMax();
	}
	struct _envInfo	*envinfo = GetEnvInfo();
	if (envinfo->datainfo.wMainGrpKind == GK_BALANCE)
		ResizeGraphObject(true);

	ReviseMinMax();
	ReDrawGraphObject(true);

	if (m_pGrpGrid)
		m_pGrpGrid->ShowData("", NULL);
}

int CObjMgr::GetDisplayDay()
{
	CIndcBase* pIndcBase;
	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);
		if (!pIndcBase)
			return -1;

		return pIndcBase->GetDisplayDay();
	}
	return -1;
}

void CObjMgr::SaveRegion()
{
	struct _xRegion* xRegion;

	int	RgnKey;
	int	TotalT = 10000, TotalB = -1;
	for (POSITION pos = m_mapRegion.GetStartPosition(); pos; )
	{
		m_mapRegion.GetNextAssoc(pos, RgnKey, xRegion);

		if (xRegion->xRect.top < TotalT)
			TotalT = xRegion->xRect.top;

		if (xRegion->xRect.bottom > TotalB)
			TotalB = xRegion->xRect.bottom;
	}

	int	Total = TotalB - TotalT;
	if (Total == 0)
		return;

	int	gcnt, rcnt;
	struct _region*	region;
	char	*pGInfo = m_pDataFormat->GetGrpInfo(rcnt, gcnt);
	for (int ii = 0; ii < rcnt; ii++)
	{
		region = (struct _region *)&pGInfo[SZ_REGION * ii];
		for (POSITION pos = m_mapRegion.GetStartPosition(); pos; )
		{
			m_mapRegion.GetNextAssoc(pos, RgnKey, xRegion);
			if (region->btRKey == UINT(RgnKey))
			{
				long	top = xRegion->xRect.top;
				long bottom = xRegion->xRect.bottom;

				region->wRTop = static_cast<WORD>((top - TotalT) * 10000 / Total);
				region->wRBottom = static_cast<WORD>((bottom - TotalT) * 10000 / Total);
				xRegion->pRect.top = region->wRTop;
				xRegion->pRect.bottom = region->wRBottom;

				break;
			}
		}
	}
}

void CObjMgr::SetReverseMode()
{
	struct _envInfo	*envInfo = (struct _envInfo*)GetEnvInfo();

	if (envInfo->display.dwDspOption & DO_REVERSE)
		envInfo->display.dwDspOption &= ~DO_REVERSE;
	else
		envInfo->display.dwDspOption |= DO_REVERSE;

	CIndcBase* pIndcBase;
	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		pIndcBase = m_arGraphQue.GetAt(ii);
		pIndcBase->SetGrpInfo(NULL, envInfo, NULL);
	}

	ReDrawGraphObject(true);
}

void CObjMgr::SetCrossMode()
{
	struct _envInfo	*envInfo = (struct _envInfo*)GetEnvInfo();

	if (envInfo->display.dwDspOption & DO_NOCROSS)
	{
		envInfo->display.dwDspOption &= ~DO_NOCROSS;
		m_pCrossLine->SetVisable(true);
	}
	else
	{
		envInfo->display.dwDspOption |= DO_NOCROSS;
		HideCrossLine();
		m_pCrossLine->SetVisable(false);
	}
}

void CObjMgr::SetJTraceMode()
{
	struct _envInfo	*envInfo = (struct _envInfo*)GetEnvInfo();

	if (envInfo->display.dwDspOption & DO_MAGNET)
		envInfo->display.dwDspOption &= ~DO_MAGNET;
	else
		envInfo->display.dwDspOption |= DO_MAGNET;
}

void CObjMgr::SetOrderMode(bool bOrderMode)
{ 
	m_bOrderMode = bOrderMode;
	struct _envInfo	*envInfo = (struct _envInfo*)GetEnvInfo();
	if (envInfo->display.dwDspOption & DO_NOCROSS)
	{
		envInfo->display.dwDspOption &= ~DO_NOCROSS;
		m_pCrossLine->SetVisable(true);
	}

	envInfo->display.dwDspOption |= DO_ECURRENT|DO_PCURRENT;

	envInfo->display.dwDspOption |= DO_NOVIEWTIP;
	m_pObjEvent->SetViewTooltip(false);
	//m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, gridChart), FALSE);
}

void CObjMgr::SetTipMode(bool bMsgSend)
{
	struct _envInfo	*envInfo = (struct _envInfo*)GetEnvInfo();

	if (envInfo->display.dwDspOption & DO_NOVIEWTIP)
	{
		envInfo->display.dwDspOption &= ~DO_NOVIEWTIP;
		m_pObjEvent->SetViewTooltip(true);

		if (bMsgSend)
			m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, gridChart), TRUE);
	}
	else
	{
		envInfo->display.dwDspOption |= DO_NOVIEWTIP;
		m_pObjEvent->SetViewTooltip(false);
		if (bMsgSend)
			m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, gridChart), FALSE);
	}
}

void CObjMgr::SetGridMode(bool bMsgSend)
{
	struct _envInfo	*envInfo = (struct _envInfo*)GetEnvInfo();

	if (envInfo->display.dwDspOption & DO_VIEWGRID)
	{
		envInfo->display.dwDspOption &= ~DO_VIEWGRID;
		HideGrid();

		if (bMsgSend)
			m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, gridChart), FALSE);
	}
	else
	{
		envInfo->display.dwDspOption |= DO_VIEWGRID;
		if (bMsgSend)
			m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, gridChart), TRUE);
	}
}

void CObjMgr::SetTableMode(bool bView, bool bMsgSend)
{
	m_bTableMode = bView;

	if (bMsgSend)
		m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, tableChart), m_bTableMode);

	if (m_bTableMode)
	{
		GetTableData();
		m_pwndPnChart->SendMessage(GEV_TABLE, tbItem, long(m_strData.operator LPCTSTR()));
	}
}

void CObjMgr::SetLogMode()
{
	struct _envInfo	*envInfo = (struct _envInfo*)GetEnvInfo();

	if (envInfo->display.dwDspOption & DO_LOG)
		envInfo->display.dwDspOption &= ~DO_LOG;
	else
		envInfo->display.dwDspOption |= DO_LOG;

	CIndcBase* pIndcBase;
	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		pIndcBase = m_arGraphQue.GetAt(ii);
		pIndcBase->SetGrpInfo(NULL, envInfo, NULL);
		// 2007.01.02 schbang : 로그모드 적용 on/off 시 min max 다시 계산하기 위해
		pIndcBase->CalculateMinMax();
	}

	ReviseMinMax();	// 2007.01.03 schbang : 로그모드 적용 on/off 시 min max 다시 계산하기 위해

	ReDrawGraphObject(true);
}

bool CObjMgr::IsCrossMode()
{
	struct _envInfo	*envInfo = (struct _envInfo*)GetEnvInfo();

	return (envInfo->display.dwDspOption & DO_NOCROSS)? false:true;
}

bool CObjMgr::IsJTraceMode()
{
	struct _envInfo	*envInfo = (struct _envInfo*)GetEnvInfo();

	return (envInfo->display.dwDspOption & DO_MAGNET)? true:false;
}

bool CObjMgr::IsGridMode()
{
	struct _envInfo	*envInfo = (struct _envInfo*)GetEnvInfo();

	return (envInfo->display.dwDspOption & DO_VIEWGRID)? true:false;
}

bool CObjMgr::IsReverseMode()
{
	struct _envInfo	*envInfo = (struct _envInfo*)GetEnvInfo();

	return (envInfo->display.dwDspOption & DO_REVERSE)? true:false;
}

bool CObjMgr::IsLogMode()
{
	struct _envInfo	*envInfo = (struct _envInfo*)GetEnvInfo();

	return (envInfo->display.dwDspOption & DO_LOG)? true:false;
}

bool CObjMgr::IsAutoScr()
{	
	return (m_pObjEvent->m_dwObjStatus & GOS_autoScroll)? true:false;
}

//** macho add
/**
bool CObjMgr::IsShowDrawingToolBar()
{
	return (
**/

void CObjMgr::SetUserDefined(CIndcBase *pIndcBase)
{
	CString	gVal;

	pIndcBase->GetUsedValue(gVal);
	if (gVal.IsEmpty())
		return;
	
	CString	name;
	switch (pIndcBase->GetGraphKind())
	{
	case GK_PNF:
		name = "한칸의값 입력";
		break;		
	case GK_KAGI:
		name = "최소반전폭 입력";
		break;
	case GK_RENKO:
		name = "최소변화폭 입력";
		break;
	default:
		return;
	}
	CUserInputDlg	dlg(gVal, name);
	if (dlg.DoModal() == IDCANCEL)
		return;

	gVal = dlg.GetDisplayString();
	pIndcBase->SetUserDefined(gVal);
	pIndcBase->CalculateMinMax(true);
	
	ReDrawGraphObject(true);
}

void CObjMgr::InitGraph()
{
	m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewChartInfo, initGrpInfo), 0);
}

CIndcBase* CObjMgr::GetMainGraph()
{
	CIndcBase* pIndcBase;

	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		pIndcBase = m_arGraphQue.GetAt(ii);
		switch (pIndcBase->GetGraphType())
		{
		case GT_BONG:
			switch (pIndcBase->GetGraphKind())
			{
			case GK_JPN:
			case GK_BAR:
			case GK_LIN:
			case GK_POLE:
			case GK_EQV:
			case GK_CDV:
				return pIndcBase;
			default:
				break;
			}
			break;
		case GT_VARS:
			switch (pIndcBase->GetGraphKind())
			{
			case GK_PNF:
			case GK_THREE:
			case GK_SWING:
			case GK_PV:
			case GK_CLOCK:
			case GK_KAGI:
			case GK_RENKO:
				return pIndcBase;
			}
			break;
		default:
			break;
		}
	}

	return (CIndcBase *) 0;
}

int CObjMgr::GetMainKind()
{
	return m_pDataFormat->GetMainGraph();
}

bool CObjMgr::GetGrpRegion(int key, CRect& rect)
{
	struct	_xRegion* xRegion;
	if (!m_mapRegion.Lookup(key, xRegion))
		return false;

	rect = xRegion->xRect;
	return true;
}

int CObjMgr::GetRegionAdjustBottom(int bottom, int& bottomKey)
{
	struct _xRegion* xRegion;
	int	rKey = 0;
	bottomKey = -1;
	for (POSITION pos = m_mapRegion.GetStartPosition(); pos; )
	{
		m_mapRegion.GetNextAssoc(pos, rKey, xRegion);
		if (bottom == xRegion->xRect.top)
		{
			bottomKey = rKey;
			return xRegion->xRect.bottom;
		}
	}

	return -1;
}

int CObjMgr::GetCountGraphinRgn(int rgnKey)
{
	CIndcBase* pTemp;
	int  count = 0, tempRgnKey;

	for (int ii = m_arGraphQue.GetUpperBound(); ii >= 0; ii--)
	{
		pTemp = m_arGraphQue.GetAt(ii);
		tempRgnKey = pTemp->GetGraphRegionID();
		if (rgnKey == tempRgnKey)
			count++;
	}
	return count;
}

int CObjMgr::GetGraphCount(int iRgnKey, int iGKind)
{
	int iCount = 0;
	CIndcBase* pIndcBase;
	for (int ii = 0; ii < m_arGraphQue.GetSize(); ii++)
	{
		pIndcBase = m_arGraphQue.GetAt(ii);
		if (pIndcBase->GetGraphRegionID() == iRgnKey && pIndcBase->GetGraphKind() != iGKind)
			iCount++;
	}

	return iCount;
}

bool CObjMgr::DeleteGraph(CIndcBase* pindcDelete)
{
	double dRatio, dMRatio, dARatio;
	int iPreTopH = 0;
	int iPreTopR = 0;
	int iCount, iRgnCnt, iGrpCnt;
	UINT* puiSortKey = NULL;
	UINT* puiSortTop = NULL;
	char* pcGrpInfo = m_pDataFormat->GetGrpInfo(iRgnCnt, iGrpCnt);

	int iDeleteKind = pindcDelete->GetGraphKind();
	int iDeleteID = pindcDelete->GetGraphID();
	int iOrgCount = m_mapRegion.GetCount();
	int iRgnKey;
	
	struct _xRegion* pxRegion;
	for (POSITION pos = m_mapRegion.GetStartPosition(); pos; )
	{
		m_mapRegion.GetNextAssoc(pos, iRgnKey, pxRegion);
		if (pxRegion->pRect.top == 0)
		{
			iPreTopH = pxRegion->xRect.top;
			break;
		}
	}

	// 지울거 찾기
	int ii = 0;
	CIndcBase* pIndcBase;
	for (ii = 0; ii < m_arGraphQue.GetSize(); ii++)
	{
		pIndcBase = m_arGraphQue.GetAt(ii);
		if (pIndcBase == pindcDelete)
		{
			iRgnKey = pIndcBase->GetGraphRegionID();
			if (GetGraphCount(iRgnKey, -1) <= 1)
			{
				RemoveGraphRegion(iRgnKey);
			}
		}
	}

	iCount = m_mapRegion.GetCount();
	puiSortKey = new UINT[iCount + 1]{};  //test 20230421
	puiSortTop = new UINT[iCount + 1]{};

	ii = 0; //chart 20230421
	for ( POSITION pos = m_mapRegion.GetStartPosition(); pos; )
	{
		m_mapRegion.GetNextAssoc(pos, iRgnKey, pxRegion);
		puiSortKey[ii] = iRgnKey;
		puiSortTop[ii] = pxRegion->xRect.top;
		ii++;
	}

	if (iOrgCount > iCount)
	{
		// 영역배분
		for (ii = 0; ii < iCount; ii++)
		{
			for (int jj = ii+1; jj < iCount; jj++)
			{
				if (puiSortTop[ii] > puiSortTop[jj])
				{
					puiSortTop[ii] += puiSortTop[jj];
					puiSortKey[ii] += puiSortKey[jj];

					puiSortTop[jj] = puiSortTop[ii] - puiSortTop[jj];
					puiSortKey[jj] = puiSortKey[ii] - puiSortKey[jj];

					puiSortTop[ii] = puiSortTop[ii] - puiSortTop[jj];
					puiSortKey[ii] = puiSortKey[ii] - puiSortKey[jj];
				}
			}
		}

		switch (iCount)
		{
		case 1:	dMRatio = 1.0;	dARatio = 0.1;	break; 
		case 2: dMRatio = 0.7;	dARatio = 0.3;	break;
		case 3: dMRatio = 0.6;	dARatio = 0.2;	break;
		case 4: dMRatio = 0.46;	dARatio = 0.18;	break;
		case 5: dMRatio = 0.40;	dARatio = 0.15;	break;
		case 6: dMRatio = 0.30;	dARatio = 0.14;	break;
		case 7: dMRatio = 0.28;	dARatio = 0.12;	break;
		default:  
			dMRatio = 0.28;	
			dARatio = 0.72 / double(iCount - 1);
			break;
		}

		int iAddHeight;
		struct _region* pRegion;
		CIndcBase* pMainGrp = GetMainGraph();
		UINT uiMainRgn = pMainGrp->GetGraphRegionID();
		for (ii = 0; ii < iCount; ii++)
		{
			m_mapRegion.Lookup(puiSortKey[ii], pxRegion);
			if (puiSortKey[ii] == uiMainRgn)	dRatio = dMRatio;
			else	dRatio = dARatio;

			iAddHeight = int(m_rectGrp.Height() * dRatio);
			pxRegion->xRect.top = iPreTopH;
			pxRegion->xRect.bottom = pxRegion->xRect.top + iAddHeight;
			iPreTopH = pxRegion->xRect.bottom;

			pxRegion->pRect.top = iPreTopR;
			if (ii == iCount - 1)
			{
				pxRegion->xRect.bottom = m_rectGrp.bottom;
				pxRegion->pRect.bottom = 10000;
			}
			else
			{
				pxRegion->pRect.bottom = pxRegion->pRect.top + int(dRatio*10000);
			}
			iPreTopR = pxRegion->pRect.bottom;

			for (int jj = 0; jj < iRgnCnt; jj++)
			{
				pRegion = (struct _region*)&pcGrpInfo[SZ_REGION*jj];
				if (puiSortKey[ii] == pRegion->btRKey)
				{
					pRegion->wRTop = WORD(pxRegion->pRect.top);
					pRegion->wRBottom = WORD(pxRegion->pRect.bottom);
					break;
				}
			}
		}
	}

	if (puiSortKey)  delete[] puiSortKey;  //test 20230421
	if (puiSortTop)  delete[] puiSortTop;
		

	char* pcNewGrpInfo = new char[SZ_REGION * iCount + SZ_GRAPH * iGrpCnt];
	int iNRCnt = 0;
	for (ii = 0; ii < iRgnCnt; ii++)
	{
		struct _region *pRegion = (struct _region *)&pcGrpInfo[SZ_REGION * ii];
		if (m_mapRegion.Lookup(pRegion->btRKey, pxRegion))
		{
			CopyMemory(&pcNewGrpInfo[SZ_REGION * iNRCnt], 
				&pcGrpInfo[SZ_REGION * ii], SZ_REGION);
			iNRCnt++;
		}
	}

	// 해당 indc에 있는 tool 삭제
	if (m_pObjEvent->m_arToolQue.GetSize() > 0)
	{
		int iTDcnt = 0, jj = 0;
		int* piToolIdx = new int[m_pObjEvent->m_arToolQue.GetSize()];
		for (jj = 0; jj < m_pObjEvent->m_arToolQue.GetSize(); jj++)
		{
			CToolBase *pToolBase = m_pObjEvent->m_arToolQue.GetAt(jj);
			if (((CIndcBase*)pToolBase->GetGrpBase()) == pindcDelete)
			{
				delete pToolBase;
				pToolBase = NULL;
				piToolIdx[iTDcnt++] = jj;
			}
		}
		for (jj = iTDcnt - 1; jj >= 0; jj--)
			m_pObjEvent->m_arToolQue.RemoveAt(piToolIdx[jj]);

		delete [] piToolIdx;
	}

	RemoveGraphQue(pindcDelete);
	m_pObjEvent->m_pindcSelect = NULL;

	bool bDeleteAll = true;
	int iNGCnt = 0;
	for (ii = 0; ii < iGrpCnt; ii++)
	{
		struct _graph* graph = (struct _graph*)&pcGrpInfo[SZ_REGION * iRgnCnt + SZ_GRAPH * ii];
		if (graph->wKey == UINT(iDeleteID))
			continue;
		if (graph->wGKind == iDeleteKind)
			bDeleteAll = false;

		CopyMemory(&pcNewGrpInfo[SZ_REGION * iNRCnt + SZ_GRAPH * iNGCnt], graph, SZ_GRAPH);
		iNGCnt++;
	}

	m_pDataFormat->SetFgraph(pcNewGrpInfo, iNRCnt, iNGCnt);
	delete[] pcNewGrpInfo;

	ReviseTick();
	ResizeGraphObject();
	ReDrawGraphObject(true);

	if (bDeleteAll)
	{
		m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, aidDelChart), iDeleteKind);
	}

	if (m_bTableMode)
	{
		GetTableData();
		m_pwndPnChart->SendMessage(GEV_TABLE, tbItem, long(m_strData.operator LPCTSTR()));
	}
	return true;
}

void CObjMgr::MapConfig()
{
	int	rcnt, gcnt;
	char *pGrpInfo = m_pDataFormat->GetGrpInfo(rcnt, gcnt);

	char *pInfo = new char[sz_INTERCHART + SZ_REGION*rcnt + SZ_GRAPH*gcnt];
	struct _interChart	*interChart = (_interChart *)pInfo;
	interChart->rgncnt = rcnt;
	interChart->grpcnt = gcnt;
	CopyMemory(&pInfo[sz_INTERCHART] , pGrpInfo, SZ_REGION*rcnt + SZ_GRAPH*gcnt);

	BOOL ret = (BOOL)m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgMap), (long)pInfo);

	delete[] pInfo;
}

void CObjMgr::ChartConfig(CIndcBase* pIndcBase)
{
	if (!pIndcBase)
		return;

	int ii, iRgnCnt, iGrpCnt;
	char* pcGrpInfo = m_pDataFormat->GetGrpInfo(iRgnCnt, iGrpCnt);
	DWORD dwDraOption = ((struct _graph *)pIndcBase->GetGrpInfo())->dwDraOption;

	struct _graph* pGrp = nullptr;
	switch (pIndcBase->GetGraphKind())
	{
	case GK_BALANCE:
		pGrp = m_pDataFormat->GetBalanceGrp();
		break;
	case GK_AVOL:
		pGrp = m_pDataFormat->GetAVolGrp();
		break;
	case GK_CDVV:
	case GK_EQVV:
		return;
	default:
		for (ii = 0; ii < iGrpCnt; ii++)
		{
			pGrp = (struct _graph*)&pcGrpInfo[SZ_REGION * iRgnCnt + SZ_GRAPH * ii];
			if (UINT(pIndcBase->GetGraphID()) == pGrp->wKey)
				break;
		}

		if (ii >= iGrpCnt)
			return;
		break;
	}
	
	if (!pGrp) return;

	pGrp->dwDraOption = dwDraOption;
	int iIndex = m_pDataFormat->GetEnvInfo()->datainfo.btIndex;
	int iUnit = 0;
	switch (m_pDataFormat->GetEnvInfo()->datainfo.btUnit)
	{
	case GU_FUTURE:
	case GU_OPTION:
	case GU_COFUT:
	case GU_COOPT:
	case GU_CFUTURE: // 20090917 : ADD
	case GU_FFUTURE: // 20090917 : ADD
		iUnit = GU_FUTURE - GU_CODE;
		break;
	case GU_CODE:
	case GU_INDEX:	
		iUnit = m_pDataFormat->GetEnvInfo()->datainfo.btUnit - GU_CODE;
		break;
	case GU_FOREIGN:
		iUnit = 3;
		break;
	default:
		break;
	}

	WORD wUnionGKindOld = m_pDataFormat->GetEnvInfo()->datainfo.aaDayInfo[iUnit][iIndex - GI_DAY].wUnionGKind;
	BOOL bRet = (BOOL)m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, DlgIndc), long(pGrp));

	if (!bRet)
		return;

	pIndcBase->SetGrpInfo(pGrp, NULL, NULL);

	if (wUnionGKindOld != m_pDataFormat->GetEnvInfo()->datainfo.aaDayInfo[iUnit][iIndex - GI_DAY].wUnionGKind)
	{
		UnionGraph(pIndcBase, pGrp->wGKind, m_pDataFormat->GetEnvInfo()->datainfo.aaDayInfo[iUnit][iIndex - GI_DAY].wUnionGKind, wUnionGKindOld);
	}


	ReviseTick();
	ReviseMinMax();
	ResizeGraphObject();
	ReDrawGraphObject(true);

	if (m_bTableMode)
	{
		GetTableData();
		m_pwndPnChart->SendMessage(GEV_TABLE, tbItem, long(m_strData.operator LPCTSTR()));
	}
}

void CObjMgr::ToolConfig(CToolBase *pToolBase)
{
	if (!pToolBase)
		return;

	struct _toolInfo toolinfo;
	pToolBase->GetToolInfo(&toolinfo);
	BOOL bRet = (BOOL)m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgTool), (LPARAM)(&toolinfo));
	if (bRet)
	{
		//pToolBase->SetLineStyle(&line);
		pToolBase->SetToolInfo(&toolinfo, true);
		int	nToolKind = pToolBase->GetToolName();
		if ((int)enumTK::tkRECT <= nToolKind && nToolKind <= (int)enumTK::tkARC)
			m_pDataFormat->GetEnvInfo()->display.toolinfo.dwOption = toolinfo.dwOption;

		ReDrawGraphObject(true);
	}	
}

bool CObjMgr::ChangeMainGraph(int iKind)
{
	m_pDataFormat->ChangeMain(iKind);
	return true;
}

void CObjMgr::ChangeMap(char *info)
{
	int	mkind = m_pDataFormat->GetEnvInfo()->datainfo.wMainGrpKind;
	switch (mkind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_POLE:
	case GK_LIN:
	case GK_AVOL:	
	case GK_BALANCE:
		break;
	default:
		m_pDataFormat->ChangeMain(mkind);
		return;
	}

	struct _interChart *iChart = (struct _interChart *)info;

	int	mrgn = 0;
	struct _graph	*graph;
	for (int ii = 0; ii < int(iChart->grpcnt); ii++)
	{
		graph = (struct _graph*)&info[sz_INTERCHART + SZ_REGION*iChart->rgncnt + SZ_GRAPH*ii];
		switch (graph->wGKind)
		{
		case GK_JPN:	case GK_BAR:	case GK_POLE:	case GK_LIN:
			mrgn = graph->btRgnKey;
			break;
		default:
			break;
		}
		graph->btDataKey = 1;
		graph->wKey = ii+1;
	}

	if (mrgn <= 0)	mrgn = 1;

	int	Mratio, Aratio;
	switch (iChart->rgncnt)
	{
	case 2: Mratio = 7000;	Aratio = 3000;	break;
	case 3: Mratio = 6000;	Aratio = 2000;	break;
	case 4: Mratio = 4600;	Aratio = 1800;	break;
	case 5: Mratio = 4000;	Aratio = 1500;	break;
	case 6: Mratio = 3000;	Aratio = 1400;	break;
	case 7: Mratio = 2800;	Aratio = 1200;	break;
	default:  
		Mratio = 2800;	
		Aratio = int(7200 / double(iChart->rgncnt));
		break;
	}

	int preTopH = 0;
	for (int ii = 0; ii < int(iChart->rgncnt); ii++)
	{
		struct _region *region = (struct _region *)&info[sz_INTERCHART + SZ_REGION*ii];
		region->wRLeft = 0;
		region->wRRight = 100;
		region->wRTop = preTopH;

		if (ii+1 == mrgn)
			region->wRBottom = region->wRTop + Mratio;
		else
			region->wRBottom = region->wRTop + Aratio;

		if (ii+1 == int(iChart->rgncnt))
			region->wRBottom = 10000;

		preTopH = region->wRBottom;
	}

	m_pDataFormat->SetFgraph(&info[sz_INTERCHART], iChart->rgncnt, iChart->grpcnt);
}

void CObjMgr::ChangeMenuMKind(int kind)
{
	m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, mainChart), kind);
}

void CObjMgr::ChangeMenuAKind(int kind)
{
	//*Debug("ChangeMenuAKind =================== %d", kind);
	m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, aidAddChart), kind);
}

void CObjMgr::ChangeIndex(int index, int gap)
{
	m_pDataFormat->ChangeIndex(index, gap);
}

void CObjMgr::ChangeUnit(int unit)
{
	m_pDataFormat->ChangeUnit(unit);
}

void CObjMgr::ChangeTotalDay(int total)
{
	m_pDataFormat->SaveDay(total, false);
}

bool CObjMgr::ChangeAidGraph(bool bAdd, struct _graph *pGrp)
{
	if (m_arGraphQue.GetSize() <= 0)
		return false;

	int iKind = pGrp->wGKind;

	double	dRatio, dMRatio, dARatio;
	int iPreTopH = 0;
	int iPreTopR = 0;
	int iCount, iRgnCnt, iGrpCnt;
	UINT* puiSortKey = NULL;
	UINT* puiSortTop = NULL;
	char* pcGrpInfo = m_pDataFormat->GetGrpInfo(iRgnCnt, iGrpCnt);

	if (bAdd)
	{
		iCount = m_mapRegion.GetCount();
		puiSortKey = new UINT[iCount];
		puiSortTop = new UINT[iCount];

		int ii = 0;
		int iRgnKey;
		struct _xRegion* pxRegion = nullptr;
		for (POSITION pos = m_mapRegion.GetStartPosition(); pos; )
		{
			m_mapRegion.GetNextAssoc(pos, iRgnKey, pxRegion);
			puiSortKey[ii] = iRgnKey;
			puiSortTop[ii] = pxRegion->xRect.top;
			ii++;
		}

		for (ii = 0; ii < iCount; ii++)
		{
			for (int jj = ii+1; jj < iCount; jj++)
			{
				if (puiSortTop[ii] > puiSortTop[jj])
				{
					puiSortTop[ii] += puiSortTop[jj];
					puiSortKey[ii] += puiSortKey[jj];

					puiSortTop[jj] = puiSortTop[ii] - puiSortTop[jj];
					puiSortKey[jj] = puiSortKey[ii] - puiSortKey[jj];

					puiSortTop[ii] = puiSortTop[ii] - puiSortTop[jj];
					puiSortKey[ii] = puiSortKey[ii] - puiSortKey[jj];
				}
			}
		}
		
		CIndcBase* pMainGrp = GetMainGraph();
		UINT uiMainRgn = pMainGrp->GetGraphRegionID();

		int iAddRgn = uiMainRgn; 
		bool bAddRgn = true;
		switch (iKind)
		{
		case GK_PARB:
		case GK_ENVL:
		case GK_BOLB:
		case GK_NET:
		case GK_PMA:
		case GK_DEMARK:
		case GK_PIVOT:
			{
				/** 중복 체크로 인해 막음
				for (int ii = m_arGraphQue.GetUpperBound(); ii >= 0; ii--)
				{
					CIndcBase *pIndcBase = m_arGraphQue.GetAt(ii);
					if ((pIndcBase->GetGraphKind() == iKind) && (UINT(pIndcBase->GetGraphRegionID()) == uiMainRgn))
					{
						if (iKind == GK_NET)
							return false;
						break;
					}
				}
				if (ii < 0)
				{
					bAddRgn = false;
					iAddRgn = uiMainRgn;
				}
				 **/
				if (IsExistGraph(iKind)) 
					return false;

				bAddRgn = false;		// 중복 체크
				iAddRgn = uiMainRgn;
			}
			break;
		case GK_VMA:
			if (IsExistGraph(iKind))	// 중복 체크
				return false;
			[[fallthrough]];
		case GK_VOL:
			{
				CIndcBase *pVolGrp = GetVolGraph(iKind);
				if (pVolGrp)
				{
					bAddRgn = false;
					iAddRgn = pVolGrp->GetGraphRegionID();
				}
			}
			break; 
		case GK_AMT:
		case GK_AMA:
			{
				CIndcBase *pAmtGrp = GetAmtGraph(iKind);
				if (pAmtGrp)
				{
					bAddRgn = false;
					iAddRgn = pAmtGrp->GetGraphRegionID();
				}
			}
		}

		struct _graph graphAdd;

		if (pGrp)
		{
			CopyMemory(&graphAdd, pGrp, SZ_GRAPH);
			graphAdd.btRgnKey = uiMainRgn;
			graphAdd.btDataKey = pMainGrp->GetDataKey();
			graphAdd.wKey = GetUnusedGraphKey();
		}

		if (bAddRgn)
		{
			switch (iCount)
			{
			case 1: dMRatio = 0.7;	dARatio = 0.3;	break;
			case 2: dMRatio = 0.6;	dARatio = 0.2;	break;
			case 3: dMRatio = 0.46;	dARatio = 0.18;	break;
			case 4: dMRatio = 0.40;	dARatio = 0.15;	break;
			case 5: dMRatio = 0.30;	dARatio = 0.14;	break;
			case 6: dMRatio = 0.28;	dARatio = 0.12;	break;
			default:  
				dMRatio = 0.28;	
				dARatio = 0.72 / double(iCount);
				break;
			}

			int iAddHeight;
			for (ii = 0; ii < iCount; ii++)	// 기존거
			{
				m_mapRegion.Lookup(puiSortKey[ii], pxRegion);
				if (puiSortKey[ii] == uiMainRgn)
					dRatio = dMRatio;
				else
					dRatio = dARatio;

				if (iPreTopH == 0)
					iPreTopH = pxRegion->xRect.top;
				if (iPreTopR == 0)
					iPreTopR = pxRegion->pRect.top;

				iAddHeight = int(m_rectGrp.Height() * dRatio);
				pxRegion->xRect.top = iPreTopH;
				pxRegion->xRect.bottom = pxRegion->xRect.top + iAddHeight;
				iPreTopH = pxRegion->xRect.bottom;

				pxRegion->pRect.top = iPreTopR;
				pxRegion->pRect.bottom = pxRegion->pRect.top + int(dRatio * 10000);
				iPreTopR = pxRegion->pRect.bottom;

				for (int jj = 0; jj < iRgnCnt; jj++)
				{
					struct _region *pRegion = (struct _region*)&pcGrpInfo[SZ_REGION * jj];
					if (puiSortKey[ii] == pRegion->btRKey)
					{
						pRegion->wRTop = WORD(pxRegion->pRect.top);
						pRegion->wRBottom = WORD(pxRegion->pRect.bottom);
					}
				}
			}

			// new
			struct _xRegion* appRegion;
			appRegion = (struct _xRegion *)new char[sizeof(struct _xRegion)];

			if (pxRegion)
				CopyMemory(appRegion, pxRegion, sizeof (struct _xRegion));
			appRegion->xRect.top = iPreTopH;
			appRegion->xRect.bottom = m_rectGrp.bottom;
			appRegion->pRect.top = iPreTopR;
			appRegion->pRect.bottom = 10000;

			int iAddRgnKey = GetUnusedRegionKey();
			m_mapRegion.SetAt(iAddRgnKey, appRegion);

			char* pcNewGrpInfo = new char[SZ_REGION * (iRgnCnt + 1) + SZ_GRAPH * (iGrpCnt + 1)];
			struct	_region	region;
			region.btRKey = iAddRgnKey;
			region.wRLeft = WORD(appRegion->pRect.left);
			region.wRTop = WORD(appRegion->pRect.top);
			region.wRRight = WORD(appRegion->pRect.right);
			region.wRBottom = WORD(appRegion->pRect.bottom);
			
			graphAdd.btRgnKey = iAddRgnKey;
			CopyMemory(pcNewGrpInfo, pcGrpInfo, SZ_REGION * iRgnCnt);
			CopyMemory(&pcNewGrpInfo[SZ_REGION * iRgnCnt], &region, SZ_REGION);
			CopyMemory(&pcNewGrpInfo[SZ_REGION * (iRgnCnt+1)], &pcGrpInfo[SZ_REGION * iRgnCnt], SZ_GRAPH * iGrpCnt);
			CopyMemory(&pcNewGrpInfo[SZ_REGION * (iRgnCnt+1) + SZ_GRAPH * iGrpCnt], &graphAdd, SZ_GRAPH);

			m_pDataFormat->SetFgraph(pcNewGrpInfo, iRgnCnt+1, iGrpCnt+1);

			delete[] pcNewGrpInfo;
		}
		else
		{
			graphAdd.btRgnKey = iAddRgn;
			char* pNewGrpInfo = new char[SZ_REGION * iRgnCnt + SZ_GRAPH * (iGrpCnt + 1)];
			CopyMemory(pNewGrpInfo, pcGrpInfo, SZ_REGION * iRgnCnt);
			CopyMemory(&pNewGrpInfo[SZ_REGION * iRgnCnt], &pcGrpInfo[SZ_REGION*iRgnCnt], SZ_GRAPH*iGrpCnt);
			CopyMemory(&pNewGrpInfo[SZ_REGION * iRgnCnt + SZ_GRAPH * iGrpCnt], &graphAdd, SZ_GRAPH);

			m_pDataFormat->SetFgraph(pNewGrpInfo, iRgnCnt, iGrpCnt + 1);

			delete[] pNewGrpInfo;
		}


		CIndcBase* pGrpAppend = AddGraphQue(1, (char *)&graphAdd, true);

		SyncGraphDisplay(pMainGrp, pGrpAppend);

	}
	else	// delete
	{
		int ii;
		int iRgnKey;
		struct	_xRegion* pxRegion;
		for (POSITION pos = m_mapRegion.GetStartPosition(); pos; )
		{
			m_mapRegion.GetNextAssoc(pos, iRgnKey, pxRegion);
			if (pxRegion->pRect.top == 0)
			{
				iPreTopH = pxRegion->xRect.top;
				break;
			}
		}

		int iOrgCount = m_mapRegion.GetCount();
		CIndcBase* pIndcBase;
		for (ii = 0; ii < m_arGraphQue.GetSize(); ii++)
		{
			pIndcBase = m_arGraphQue.GetAt(ii);
			if (pIndcBase->GetGraphKind() == iKind)
			{
				iRgnKey = pIndcBase->GetGraphRegionID();
				if (GetGraphCount(iRgnKey, iKind) <= 0)
				{
					RemoveGraphRegion(iRgnKey);
				}
			}
		}

		iCount = m_mapRegion.GetCount();
		puiSortKey = new UINT[iCount];
		puiSortTop = new UINT[iCount];

		ii = 0; 
		for (POSITION pos = m_mapRegion.GetStartPosition(); pos; )
		{
			m_mapRegion.GetNextAssoc(pos, iRgnKey, pxRegion);
			puiSortKey[ii] = iRgnKey;
			puiSortTop[ii] = pxRegion->xRect.top;
			ii++;
		}

		if (iOrgCount > iCount)
		{
			for (ii = 0; ii < iCount; ii++)
			{
				for (int jj = ii+1; jj < iCount; jj++)
				{
					if (puiSortTop[ii] > puiSortTop[jj])
					{
						puiSortTop[ii] += puiSortTop[jj];
						puiSortKey[ii] += puiSortKey[jj];

						puiSortTop[jj] = puiSortTop[ii] - puiSortTop[jj];
						puiSortKey[jj] = puiSortKey[ii] - puiSortKey[jj];

						puiSortTop[ii] = puiSortTop[ii] - puiSortTop[jj];
						puiSortKey[ii] = puiSortKey[ii] - puiSortKey[jj];
					}
				}
			}

			switch (iCount)
			{
			case 1:	dMRatio = 1.0;	dARatio = 0.1;	break; 
			case 2: dMRatio = 0.7;	dARatio = 0.3;	break;
			case 3: dMRatio = 0.6;	dARatio = 0.2;	break;
			case 4: dMRatio = 0.46;	dARatio = 0.18;	break;
			case 5: dMRatio = 0.40;	dARatio = 0.15;	break;
			case 6: dMRatio = 0.30;	dARatio = 0.14;	break;
			case 7: dMRatio = 0.28;	dARatio = 0.12;	break;
			default:  
				dMRatio = 0.28;	
				dARatio = 0.72 / (static_cast<double>(iCount) - 1 );
				break;
			}

			int iAddHeight;
			struct _region* pRegion;
			CIndcBase* pMainGrp = GetMainGraph();
			UINT uiMainRgn = pMainGrp->GetGraphRegionID();
			for (ii = 0; ii < iCount; ii++)
			{
				m_mapRegion.Lookup(puiSortKey[ii], pxRegion);
				if (puiSortKey[ii] == uiMainRgn)
					dRatio = dMRatio;
				else
					dRatio = dARatio;

				iAddHeight = int(m_rectGrp.Height() * dRatio);
				pxRegion->xRect.top = iPreTopH;
				pxRegion->xRect.bottom = pxRegion->xRect.top + iAddHeight;
				iPreTopH = pxRegion->xRect.bottom;

				pxRegion->pRect.top = iPreTopR;
				if (ii == iCount - 1)
				{
					pxRegion->xRect.bottom = m_rectGrp.bottom;
					pxRegion->pRect.bottom = 10000;
				}
				else
				{
					pxRegion->pRect.bottom = pxRegion->pRect.top + int(dRatio * 10000);
				}
				iPreTopR = pxRegion->pRect.bottom;

				for (int jj = 0; jj < iRgnCnt; jj++)
				{
					pRegion = (struct _region*)&pcGrpInfo[SZ_REGION * jj];
					if (puiSortKey[ii] == pRegion->btRKey)
					{
						pRegion->wRTop = WORD(pxRegion->pRect.top);
						pRegion->wRBottom = WORD(pxRegion->pRect.bottom);
						break;
					}
				}
			}
		}

		char* pcNewGrpInfo = new char[SZ_REGION * iCount + SZ_GRAPH * iGrpCnt];
		int iNRgnCnt = 0;
		for (ii = 0; ii < iRgnCnt; ii++)
		{
			struct _region *pRegion = (struct _region *)&pcGrpInfo[SZ_REGION*ii];
			if (m_mapRegion.Lookup(pRegion->btRKey, pxRegion))
			{
				CopyMemory(&pcNewGrpInfo[SZ_REGION * iNRgnCnt], &pcGrpInfo[SZ_REGION * ii], SZ_REGION);
				iNRgnCnt++;
			}
		}

		// tool 삭제, indc 삭제
		int iTGCnt = 0;
		int* piGrpIdx = new int[m_arGraphQue.GetSize()];
		for (ii = 0; ii < m_arGraphQue.GetSize(); ii++)
		{
			pIndcBase = m_arGraphQue.GetAt(ii);
			if (pIndcBase->GetGraphKind() == iKind)
			{
				if (m_pObjEvent->m_arToolQue.GetSize() > 0)
				{
					int jj = 0, iTDCnt = 0;
					int* piToolIdx = new int[m_pObjEvent->m_arToolQue.GetSize()];
					for (jj = 0; jj < m_pObjEvent->m_arToolQue.GetSize(); jj++)
					{
						CToolBase *pToolBase = m_pObjEvent->m_arToolQue.GetAt(jj);
						if (((CIndcBase *)pToolBase->GetGrpBase()) == pIndcBase)
						{
							delete pToolBase;
							pToolBase = NULL;
							piToolIdx[iTDCnt++] = jj;
						}
					}
					for (jj = iTDCnt - 1; jj >= 0; jj--)
						m_pObjEvent->m_arToolQue.RemoveAt(piToolIdx[jj]);

					delete[] piToolIdx;
				}

				piGrpIdx[iTGCnt++] = ii;
				delete pIndcBase;
				pIndcBase = NULL;
			}
		}
		for (ii = iTGCnt - 1; ii >= 0; ii--)
			m_arGraphQue.RemoveAt(piGrpIdx[ii]);
		delete[] piGrpIdx;

		int iNGCnt = 0;
		for (ii = 0; ii < iGrpCnt; ii++)
		{
			struct _graph* pGraph = (struct _graph *)&pcGrpInfo[SZ_REGION * iRgnCnt + SZ_GRAPH * ii];
			if (pGraph->wGKind == iKind)
				continue;

			CopyMemory(&pcNewGrpInfo[SZ_REGION * iNRgnCnt + SZ_GRAPH * iNGCnt], pGraph, SZ_GRAPH);
			iNGCnt++;
		}

		m_pDataFormat->SetFgraph(pcNewGrpInfo, iNRgnCnt, iNGCnt);
		delete[] pcNewGrpInfo;
	}

	if (puiSortKey)	
		delete[] puiSortKey;
	if (puiSortTop)	
		delete[] puiSortTop;

	ReviseTick();
	ResizeGraphObject();
	ReDrawGraphObject(true);

	if (m_bTableMode)
	{
		GetTableData();
		m_pwndPnChart->SendMessage(GEV_TABLE, tbItem, long(m_strData.operator LPCTSTR()));
	}

	if (bAdd)
	{
		int iSingle = m_pDataFormat->CheckHeaderS(iKind);
		if (iSingle)
			m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, requestTRS), 0);
	}

	return true;
}

void CObjMgr::ChangeEnvInfo()
{
	m_pDataFormat->MakeHeader();
	m_pDataFormat->CheckHeader2();

	struct _envInfo	*envinfo = GetEnvInfo();
	CIndcBase* pIndcBase;
	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);
		pIndcBase->SetGrpInfo(NULL, envinfo, NULL);
	}

	bool	bViewTip = (envinfo->display.dwDspOption & DO_NOVIEWTIP)? false:true;
	m_pObjEvent->SetViewTooltip(bViewTip);
	m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify,tipChart), bViewTip);

	bool	bViewGrid = (envinfo->display.dwDspOption & DO_VIEWGRID)? true:false;
	m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, gridChart), bViewGrid);

	if (!bViewGrid)
		HideGrid();

	m_pDataFormat->ChangeMain(envinfo->datainfo.wMainGrpKind);
}

int CObjMgr::ChangeDisplayScale(int scale)
{
	int dispDay = 0;
	CIndcBase* pIndcBase;

	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);
		dispDay = pIndcBase->SetDisplayScale(scale);
	}

	if (dispDay)
	{
		if (GetMainKind() == GK_BALANCE)	ResizeGraphObject();
		ReviseMinMax();
		ReDrawGraphObject(true);
		m_pDataFormat->SaveDay(dispDay, true);
	}

	return dispDay;
}

bool CObjMgr::ChangeDisplayShift(int shift)
{
	bool bReturn = false;
	CIndcBase* pIndcBase;

	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);
		bReturn = pIndcBase->SetDisplayShift(shift);
	}

	if (bReturn)
	{
		ReviseMinMax();
		ReDrawGraphObject(true);
	}

	return bReturn;
}

bool CObjMgr::ChangeDisplayPos(int dispPos, int dispEnd)
{
	CIndcBase* pIndcBase;
	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);
		pIndcBase->ChangeDisplayPos(dispPos, dispEnd);
	}

	struct _envInfo	*envinfo = GetEnvInfo();
	if (envinfo->datainfo.wMainGrpKind == GK_BALANCE)
		ResizeGraphObject(true);

	ReviseMinMax();
	ReDrawGraphObject(true);

	if (m_pGrpGrid)
		m_pGrpGrid->ShowData("", NULL);

	return true;
}

bool CObjMgr::ChangeDisplay(int iDispDay)
{
	if (DrawCntIsZero())
		return false;

	bool bReturn = true;
	CIndcBase* pIndcBase = this->GetMainGraph();
	if (pIndcBase)
	{
		int iTotal = pIndcBase->GetTotalDataCount();
//		int iCount = m_pwndPnChart->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, GET_TOTAL_DAY), 0);
/*
		//Debug("iTotal = %d, iCount = %d", iTotal, iCount);
		TRACE("iTotal = %d, iCount = %d\n", iTotal, iCount);
		//if (iTotal != iCount)	// tr
		if (iTotal != iCount)	// tr
		{
			if (iCount < iDispDay)
			{
				//Debug("+=====================================================iCount = %d, iDispDay = %d  ==", iCount, iDispDay);
//				TRACE("+=====================================================iCount = %d, iDispDay = %d  ==", iCount, iDispDay);
				iDispDay = iCount;
			}

			m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, dispDayChart), iDispDay);

			m_pDataFormat->SaveDay(iCount, false);
			m_pDataFormat->SaveDay(iDispDay, true);

			m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, requestTR), 0);

			return true;
		}
*/
		//else if (iTotal < iDispDay)
		if (iTotal < iDispDay)
		{
			iDispDay = iTotal;
			bReturn = false;
		}
	}
	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);
		pIndcBase->SetDisplay(iDispDay);
	}

	if (GetMainKind() == GK_BALANCE)
		ResizeGraphObject();

	ReviseMinMax();

	ReDrawGraphObject(true);
	if (m_pGrpGrid)
		m_pGrpGrid->ShowData("", NULL);

	m_pDataFormat->SaveDay(iDispDay, true);

	if (!bReturn)
	{
		m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, dispDayChart), iDispDay);
		return false;
	}

	return true;
}

bool CObjMgr::ChangeTool(int select)
{
	if (GetMainGraph() == (CIndcBase *) 0)
		return false;

	return m_pObjEvent->ToolSelect(select);
}

void CObjMgr::ChangeToolColor(int rgb)
{
	m_pObjEvent->ToolColor(rgb);
}

void CObjMgr::ChangeToolWidth(int width)
{
	m_pObjEvent->ToolWidth(width);
}

int CObjMgr::GetScrollInfo(char *pBuf)
{
	CIndcBase	*pMainGrp = GetMainGraph();
	if (pMainGrp == NULL)
		return 0;

	CString tmpS = _T("");
	tmpS.Format("%05d%05d%05d%05d", 
		pMainGrp->GetTotalDataCount(),
		pMainGrp->GetDisplayPos(),
		pMainGrp->GetDisplayEnd(),
		pMainGrp->GetDisplayDay());

	memcpy(pBuf, tmpS, tmpS.GetLength());

	return tmpS.GetLength();
}

void CObjMgr::SetAutoScroll(bool bSet)
{
	if (bSet)
		m_pObjEvent->m_dwObjStatus |= GOS_autoScroll;
	else
		m_pObjEvent->m_dwObjStatus &= ~GOS_autoScroll;
}

void CObjMgr::ReloadGrp(char *pData, bool bTableReset)
{
	_envInfo *envinfo = (_envInfo *)pData;

	bool	bViewTip = (envinfo->display.dwDspOption & DO_NOVIEWTIP)? false:true;
	m_pObjEvent->SetViewTooltip(bViewTip);

	if (!(envinfo->display.dwDspOption & DO_VIEWGRID))
		HideGrid();

	m_pObjEvent->SetBookMark(envinfo->ulist);
	m_pDataFormat->SetFgraph((char *)envinfo, &pData[SZ_ENVINFO], bTableReset);

	if (bTableReset)
		m_bTableMode = false;
}

void CObjMgr::Excel()
{
	if (m_pwndPnChart)
		m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewUtility, viewExcel), 0);
}

void CObjMgr::SaveImage()
{
	m_pwndPnChart->KillTimer(TIMER_IMG);
	m_pwndPnChart->SetTimer(TIMER_IMG, 100, NULL);
}

void CObjMgr::Print()
{
	if (m_pwndPnChart)
		m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewUtility, viewPrint), 0);
}

bool CObjMgr::GetExcelData(CString &data)
{
	if (m_arGraphQue.GetSize() <= 0)
		return false;

	CIndcBase	*pIndcBase = GetMainGraph();
	int	dIndex = pIndcBase->GetDataIndex();
	int	dUnit = pIndcBase->GetDataUnit();
	int	dKey = pIndcBase->GetDataKey();
	COrgData	*pOrgData;
	if (!m_pDataMgr->GetOrgData(dKey, pOrgData))
		return false;

	switch (dIndex)
	{
	case GI_MINUTE:	case GI_TICK:
		data = _T("시간"); data += "\t";
		break;
	default:
		data = _T("일자"); data += "\t";
		break;
	}

	data += _T("시가"); data += "\t"; data += _T("고가"); data += "\t";
	data += _T("저가"); data += "\t"; data += _T("종가"); data += "\t";
	data += _T("거래량");

	CString	tmpS;
	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		pIndcBase = m_arGraphQue.GetAt(ii);
		tmpS = pIndcBase->ExcelData(-1);
		if (!tmpS.IsEmpty())
			data += "\t" + tmpS;
	}

	data += _T("\n");
	
	int	kk = 0;
	int	size = pOrgData->GetArraySize();
	for (int ii = 0; ii < size; ii++)
	{
		CGrpBasic* gBasic = pOrgData->GetGraphData(ii);

		if (gBasic->m_iClose == GD_UNUSED)
			continue;

		switch (dIndex)
		{
		case GI_MINUTE:
		case GI_TICK:
			if (gBasic->m_index.time.hh == 0)
				continue;
			tmpS.Format("%02d%02d%02d\t", gBasic->m_index.time.hh,
						      gBasic->m_index.time.mm,
						      gBasic->m_index.time.ss);
			break;
		default:
			if (gBasic->m_index.date.yy == 0)
				continue;
			tmpS.Format("%04d%02d%02d\t", gBasic->m_index.date.yy,
						      gBasic->m_index.date.mm,
						      gBasic->m_index.date.dd);
			break;
		}
		data += tmpS;

		switch (dUnit)
		{
		case GU_FUTURE:	case GU_OPTION:
		case GU_CFUTURE: // 20090917 : ADD
			tmpS.Format("%.2f\t%.2f\t%.2f\t%.2f\t%.0f",
							gBasic->m_iOpen/100.,
							gBasic->m_iHigh/100.,
							gBasic->m_iLow/100.,
							gBasic->m_iClose/100.,
							gBasic->m_dVolume);
			break;
		case GU_INDEX:
			tmpS.Format("%.2f\t%.2f\t%.2f\t%.2f\t%.0fK",
							gBasic->m_iOpen/100.,
							gBasic->m_iHigh/100.,
							gBasic->m_iLow/100.,
							gBasic->m_iClose/100.,
							gBasic->m_dVolume);
			break;
		case GU_FFUTURE: // 20090918 : ADD : 상품 선물 데이터 처리
			tmpS.Format("%d\t%d\t%d\t%d\t%.0f",
							gBasic->m_iOpen,
							gBasic->m_iHigh,
							gBasic->m_iLow,
							gBasic->m_iClose,
							gBasic->m_dVolume);
			break;
		case GU_CODE:	case GU_COFUT:	case GU_COOPT:
		default:
			tmpS.Format("%d\t%d\t%d\t%d\t%.0f", gBasic->m_iOpen,
							    gBasic->m_iHigh,
							    gBasic->m_iLow,
							    gBasic->m_iClose,
							    gBasic->m_dVolume);
			break;
		}
		data += tmpS;

		for (kk = 0; kk <= m_arGraphQue.GetUpperBound(); kk++)
		{
			pIndcBase = m_arGraphQue.GetAt(kk);
			tmpS = pIndcBase->ExcelData(ii);
			if (!tmpS.IsEmpty())
				data += "\t" + tmpS;
		}
		
		data += _T("\n");
	}

	return true;
}

char* CObjMgr::GetTableData()
{
	m_strData = _T("");

	CString	headerS = _T("");;
	CString	bodyS = _T("");
	int row = 0, col = 0;

	if (m_arGraphQue.GetSize() <= 0)
		return false;

	CIndcBase	*pIndcBase = GetMainGraph();
	int	dIndex = pIndcBase->GetDataIndex();
	int	dUnit = pIndcBase->GetDataUnit();
	int	dKey = pIndcBase->GetDataKey();
	COrgData	*pOrgData;
	if (!m_pDataMgr->GetOrgData(dKey, pOrgData))
		return false;

	switch (dIndex)
	{
	case GI_MINUTE:	case GI_TICK:
		bodyS = _T("시간"); bodyS += "\t";
		break;
	default:
		bodyS = _T("일자"); bodyS += "\t";
		break;
	}

	bodyS += _T("시가"); bodyS += "\t"; bodyS += _T("고가"); bodyS += "\t";
	bodyS += _T("저가"); bodyS += "\t"; bodyS += _T("종가"); bodyS += "\t";
	bodyS += _T("거래량");

	CString	tmpS;
	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		pIndcBase = m_arGraphQue.GetAt(ii);
		tmpS = pIndcBase->ExcelData(-1);
		if (!tmpS.IsEmpty())
			bodyS += "\t" + tmpS;
	}

	bodyS += "\t";
	tmpS = bodyS;
	for (col = 0; !tmpS.IsEmpty(); col++)
	{
		int index = tmpS.Find("\t");
		if (index < 0)
			break;

		tmpS = tmpS.Mid(index+1);
	}
	
	bodyS += _T("\n");
	
	int	ii, kk = 0;
	int	size = pOrgData->GetArraySize();
	for (ii = size - 1; ii >= 0; ii--)
	{
		CGrpBasic	*gBasic = pOrgData->GetGraphData(ii);

		if (gBasic->m_iClose == GD_UNUSED)
			continue;

		switch (dIndex)
		{
		case GI_MINUTE:	case GI_TICK:
			if (gBasic->m_index.time.hh == 0)
				continue;
			tmpS.Format("%02d:%02d:%02d\t", 
				gBasic->m_index.time.hh, gBasic->m_index.time.mm, gBasic->m_index.time.ss);
			break;
		default:
			if (gBasic->m_index.date.yy == 0)
				continue;
			tmpS.Format("%04d/%02d/%02d\t", 
				gBasic->m_index.date.yy, gBasic->m_index.date.mm, gBasic->m_index.date.dd);
			break;
		}
		bodyS += tmpS;

		switch (dUnit)
		{
		case GU_INDEX:	case GU_FUTURE:	case GU_OPTION:
		case GU_FOREIGN:
		case GU_CFUTURE: // 20090917 : ADD
			tmpS.Format("%.2f\t%.2f\t%.2f\t%.2f\t%.0f",
				gBasic->m_iOpen/100., gBasic->m_iHigh/100., gBasic->m_iLow/100.,
				gBasic->m_iClose/100., gBasic->m_dVolume);
			break;
		case GU_FFUTURE: // 20090917 : ADD : 상품 선물 데이터 처리
			tmpS.Format("%d\t%d\t%d\t%d\t%.0f", 
				gBasic->m_iOpen, gBasic->m_iHigh, gBasic->m_iLow,
				gBasic->m_iClose, gBasic->m_dVolume);
			break;
		case GU_CODE:	case GU_COFUT:	case GU_COOPT:
		default:
			tmpS.Format("%d\t%d\t%d\t%d\t%.0f", 
				gBasic->m_iOpen, gBasic->m_iHigh, gBasic->m_iLow, gBasic->m_iClose, gBasic->m_dVolume);
			break;
		}
		bodyS += tmpS;

		for (kk = 0; kk <= m_arGraphQue.GetUpperBound(); kk++)
		{
			pIndcBase = m_arGraphQue.GetAt(kk);
			tmpS = pIndcBase->ExcelData(ii);
			if (!tmpS.IsEmpty())
				bodyS += "\t" + tmpS;
		}
		
		bodyS += _T("\t\n");
		row++;
	}

	headerS.Format(_T("%05d%05d\n"), row, col);
	m_strData = headerS + bodyS;

	return (char*)(m_strData.GetString());
}

char* CObjMgr::GetPrintString()
{
	m_strData = _T("");

	CIndcBase	*pIndcBase = GetMainGraph();
	if (!pIndcBase)
		return (char*)m_strData.operator LPCTSTR();

	int	dIndex = pIndcBase->GetDataIndex();
	int	dKey = pIndcBase->GetDataKey();

	COrgData	*pOrgData;
	if (!m_pDataMgr->GetOrgData(dKey, pOrgData))
		return (char*)m_strData.operator LPCTSTR();

	CGrpBasic* gBasic = NULL;
	int	size = pOrgData->GetArraySize();
	for (int ii = 0; ii < size; ii++)
	{
		gBasic = pOrgData->GetGraphData(ii);
		if (gBasic->m_iClose != GD_UNUSED)
			break;
	}

	if (!gBasic)
		return (char*)m_strData.operator LPCTSTR();

	CString	startS, endS;
	startS.Format("%04d/%02d/%02d", gBasic->m_index.date.yy, gBasic->m_index.date.mm, gBasic->m_index.date.dd);
	gBasic = pOrgData->GetGraphData(size-1);
	endS.Format("%04d/%02d/%02d", gBasic->m_index.date.yy, gBasic->m_index.date.mm, gBasic->m_index.date.dd);

	int	gap = pOrgData->GetDataGap();

	char	*pName = (char*)m_pwndPnChart->SendMessage(GEV_SISE, MAKEWPARAM(siseData, siseName), 0);
	switch (m_iDtIndex)
	{
	case GI_DAY:
		m_strData.Format(_T("종합차트:일간차트-%s(%s - %s)"), pName, startS.GetString(), endS.GetString());
		break;
	case GI_WEEK:
		m_strData.Format(_T("종합차트:주간차트-%s(%s - %s)"), pName, startS.GetString(), endS.GetString());
		break;
	case GI_MONTH:
		m_strData.Format(_T("종합차트:월간차트-%s(%s - %s)"), pName, startS.GetString(), endS.GetString());
		break;
	case GI_MINUTE:
		if (gap > 900)
			m_strData.Format(_T("종합차트:%d초차트-%s(%s - %s)"), gap - 900, pName, startS.GetString(), endS.GetString());
		else
			m_strData.Format(_T("종합차트:%d분차트-%s(%s - %s)"), gap, pName, startS.GetString(), endS.GetString());
		break;
	case GI_TICK:
		m_strData.Format(_T("종합차트:%d틱차트-%s"), gap, pName);
		break;
	}

	return  (char*)m_strData.operator LPCTSTR();
}

char* CObjMgr::GetSaveString()
{
	m_strData = _T("");

	CIndcBase	*pIndcBase = GetMainGraph();
	if (!pIndcBase)
		return (char*)m_strData.operator LPCTSTR();

	int	unit = pIndcBase->GetDataUnit();
	int	dIndex = pIndcBase->GetDataIndex();
	int	dKey = pIndcBase->GetDataKey();

	COrgData	*pOrgData;
	if (!m_pDataMgr->GetOrgData(dKey, pOrgData))
		return (char*)m_strData.operator LPCTSTR();
	int	gap = pOrgData->GetDataGap();

	char	*pCode = (char*)m_pwndPnChart->SendMessage(GEV_SISE, MAKEWPARAM(siseData, siseCode), 0);
	CString	sName;
	if (unit == GU_CODE && strlen(pCode))
		sName.Format("%s", &pCode[1]);
	else
		sName.Format("%s", &pCode[0]);

	switch (m_iDtIndex)
	{
	case GI_DAY:
		m_strData.Format(_T("%s_일간"), sName.GetString());
		break;
	case GI_WEEK:
		m_strData.Format(_T("%s_주간"), sName.GetString());
		break;
	case GI_MONTH:
		m_strData.Format(_T("%s_월간"), sName.GetString());
		break;
	case GI_MINUTE:
		if (gap > 900)
			m_strData.Format(_T("%s_%d초"), sName.GetString(), gap - 900);
		else
			m_strData.Format(_T("%s_%d분"), sName.GetString(), gap);
		break;
	case GI_TICK:
		m_strData.Format(_T("%s_%d틱"), sName.GetString(), gap);
		break;
	}

	return  (char*)m_strData.operator LPCTSTR();
}

void CObjMgr::RealTableData(bool bShift)
{
	if (!m_bTableMode)
		return;

	CIndcBase	*pIndcBase = GetMainGraph();
	int	dIndex = pIndcBase->GetDataIndex();
	int	dUnit = pIndcBase->GetDataUnit();
	int	dKey = pIndcBase->GetDataKey();

	COrgData	*pOrgData;
	if (!m_pDataMgr->GetOrgData(dKey, pOrgData))
		return;

	int	index = pOrgData->GetArraySize() - 1;
	CGrpBasic* gBasic = pOrgData->GetGraphData(index);

	if (gBasic->m_iClose == GD_UNUSED)
		return;

	CString	tmpS;
	switch (dIndex)
	{
	case GI_MINUTE:
	case GI_TICK:
		if (gBasic->m_index.time.hh == 0)
			return;
		tmpS.Format("%02d:%02d:%02d\t", 
			gBasic->m_index.time.hh, gBasic->m_index.time.mm, gBasic->m_index.time.ss);
		break;
	default:
		if (gBasic->m_index.date.yy == 0)
			return;
		tmpS.Format("%04d/%02d/%02d\t", 
			gBasic->m_index.date.yy, gBasic->m_index.date.mm, gBasic->m_index.date.dd);
		break;
	}
	m_strData = tmpS;

	switch (dUnit)
	{
	case GU_INDEX:
	case GU_FUTURE:
	case GU_OPTION:
	case GU_CFUTURE: // 20090917 : ADD
		tmpS.Format("%.2f\t%.2f\t%.2f\t%.2f\t%.0f",
			gBasic->m_iOpen/100., gBasic->m_iHigh/100.,
			gBasic->m_iLow/100., gBasic->m_iClose/100., gBasic->m_dVolume);
		break;
	case GU_FFUTURE: // 20090917 : ADD : 상품 선물 데이터 처리
		tmpS.Format("%d\t%d\t%d\t%d\t%.0f", 
			gBasic->m_iOpen, gBasic->m_iHigh,
			gBasic->m_iLow, gBasic->m_iClose, gBasic->m_dVolume);
		break;
	case GU_CODE:
	case GU_COFUT:
	case GU_COOPT:
	default:
		tmpS.Format("%d\t%d\t%d\t%d\t%.0f", 
			gBasic->m_iOpen, gBasic->m_iHigh, gBasic->m_iLow, gBasic->m_iClose, gBasic->m_dVolume);
		break;
	}
	m_strData += tmpS;

	for (int kk = 0; kk <= m_arGraphQue.GetUpperBound(); kk++)
	{
		CIndcBase *pIndcBase = m_arGraphQue.GetAt(kk);
		tmpS = pIndcBase->ExcelData(index);
		if (!tmpS.IsEmpty())
			m_strData += "\t" + tmpS;
	}
	
	m_strData += _T("\t\n");
	if (bShift)
		m_pwndPnChart->SendMessage(GEV_TABLE, tbShift, long(m_strData.operator LPCTSTR()));
	else
		m_pwndPnChart->SendMessage(GEV_TABLE, tbUpdate, long(m_strData.operator LPCTSTR()));
}

// private member function
bool CObjMgr::IsValidGraphHeader(int iKey, int iRegion)	// iKey는 왜???
{
	CRect rectRgn;
	struct _xRegion* pxRegion;
	if (!m_mapRegion.Lookup(iRegion, pxRegion))
		return false;

	return true;
}

void CObjMgr::CalculateTk(int iWhichTK)
{
	int iFontPoint = 9;
	int iFontWeight = FW_NORMAL;
	CString	strFontName = "굴림";
	
	float fFontCount = 9;
	struct _envInfo	*penvinfo = GetEnvInfo();

	/**
	if (m_pObjEvent->m_bMapChart)
	{
		if (penvinfo->datainfo.btUnit != GU_INDEX)	
			fFontCount = 8;

		// 2005.11.01 macho
		// 스타지수일 땐 가격 자릿수가 크므로 fFontCount를 9로 설정한다.
		CString sCode = (LPSTR)m_pwndPnChart->SendMessage(GEV_SISE, MAKEWPARAM(siseData, siseCode), 0);
		if (sCode.Left(3) == "103")
			fFontCount = 9;
	}
	else **/if (penvinfo->datainfo.btUnit == GU_FOREIGN)	
		fFontCount = 10;


	if (strlen(penvinfo->display.font.acFName))
		strFontName.Format("%s", penvinfo->display.font.acFName);
	if (penvinfo->display.font.btFPoint > 0 && penvinfo->display.font.btFPoint < 15)
		iFontPoint = penvinfo->display.font.btFPoint;

	int iWhich;
	switch (iWhichTK)
	{
	case TICK_LEFT:
		iWhich = TICK_LEFT;	
		break;
	case TICK_RIGHT:
		iWhich = TICK_RIGHT;
		break;
	default:
		iWhich = TICK_BOTTOM;
		break;
	}

	struct _envInfo* envInfo = GetEnvInfo();

	iFontPoint  = envInfo->display.atick[iWhich].tfont.btFPoint;
	iFontWeight = envInfo->display.atick[iWhich].tfont.btFStyle;
	if (strlen(envInfo->display.atick[iWhich].tfont.acFName))
		strFontName = envInfo->display.atick[iWhich].tfont.acFName;
	if (envInfo->display.atick[iWhich].wTArea > 500)
		fFontCount = float(envInfo->display.atick[iWhich].wTArea/10000.);

	CFont* pfontCur = m_pGMainApp->GetFont(m_pwndView, iFontPoint, strFontName);

	CClientDC clientDC(m_pwndPnChart);
	CFont	*sFont = clientDC.SelectObject(pfontCur);

	CSize size = clientDC.GetOutputTextExtent("0");
	m_aszTick[iWhich].cx = int(size.cx * fFontCount);
	m_aszTick[iWhich].cy = size.cy;

	clientDC.SelectObject(sFont);
}

void CObjMgr::ReviseTick()
{
	if (m_pCoDraw)
	{
		delete[] m_pCoDraw;
		m_pCoDraw = NULL;
	}

	DWORD dwDispTick = GO_TKBOTTOM;
	struct _envInfo* penvinfo = GetEnvInfo();
	if (penvinfo->display.dwDspOption & DO_DISPRT)
		dwDispTick |= GO_TKRIGHT;
	if (penvinfo->display.dwDspOption & DO_DISPLT)
		dwDispTick |= GO_TKLEFT;


	m_iCoDrawCnt = m_mapRegion.GetCount();
	m_pCoDraw = new _CoDraw[m_iCoDrawCnt]{ 0, };

	int iRgnKey = 0;
	int iRgnIndex = 0;
	struct _xRegion* pxRegion;
	CIndcBase* pIndcBase;

	for (POSITION pos = m_mapRegion.GetStartPosition(); pos; )
	{
		int iPMMCnt = 0;
		int iVMMCnt = 0;
		int iAMMCnt = 0;
		int iRgnCnt = 0;
// 2006.10.11 schbang
int iMacdCnt = 0;
		m_mapRegion.GetNextAssoc(pos, iRgnKey, pxRegion);


		struct _graph* pgraphFirst = NULL;
		for (int ii = 0; ii < m_arGraphQue.GetSize(); ii++)
		{
			pIndcBase = m_arGraphQue.GetAt(ii);
			// 같은 rgn 일경우
			if (pIndcBase->GetGraphRegionID() == iRgnKey)
			{
				pIndcBase->SetGrpInfo(NULL, NULL, &m_pCoDraw[iRgnIndex]);
				struct _graph* pGraph = (struct _graph*)pIndcBase->GetGrpInfo();
				pGraph->dwDraOption &= ~(GO_TKLEFT | GO_TKRIGHT | GO_TKBOTTOM);
				// rgn의 맨 처음 rgp 인 경우
				if (iRgnCnt == 0)
				{
					pGraph->dwDraOption |= dwDispTick;
					pgraphFirst = pGraph;
				}
				else
				{
					if ((pGraph->dwDraOption & GO_TICK_LEFT_DRAG) && (pGraph->dwDraOption & GO_TICK_RIGHT_DRAG))
					{
						pGraph->dwDraOption |= GO_TKLEFT | GO_TKRIGHT;
						pgraphFirst->dwDraOption &= ~(GO_TKLEFT | GO_TKRIGHT);
					}
					else if (pGraph->dwDraOption & GO_TICK_LEFT_DRAG)
					{
						pGraph->dwDraOption |= GO_TKLEFT;
						pgraphFirst->dwDraOption &= ~GO_TKLEFT;
						pgraphFirst->dwDraOption |= GO_TICK_SKIP_HLINE;
					}
					else if (pGraph->dwDraOption & GO_TICK_RIGHT_DRAG)
					{
						if (penvinfo->display.dwDspOption & DO_DISPLT)
							pGraph->dwDraOption |= GO_TKRIGHT | GO_TICK_SKIP_HLINE;
						else
							pGraph->dwDraOption |= GO_TKRIGHT;

						pgraphFirst->dwDraOption &= ~GO_TKRIGHT;
					}
				}

				iRgnCnt++;

				switch(pIndcBase->GetGraphKind())
				{
				case GK_JPN:
				case GK_BAR:
				case GK_LIN:
				case GK_POLE:
				case GK_NET:
				case GK_PMA:	
				case GK_PARB:
				case GK_ENVL:
				case GK_BOLB:
				case GK_DEMARK:
				case GK_PIVOT:
				case GK_BALANCE:
				case GK_AVOL:
					iPMMCnt++;
					break;
				case GK_VOL:
				case GK_VMA:
					iVMMCnt++;
					break;
				case GK_AMT:
				case GK_AMA:
					iAMMCnt++;
					break;
// 2006.10.11 schbang
case GK_MACD:
case GK_MACDOS:
	iMacdCnt++;
	break;
				default:
					break;
				}
			}
		}

		if (m_iCoDrawCnt > iRgnIndex)
		{
			m_pCoDraw[iRgnIndex].region = iRgnKey;
			if (iPMMCnt > 1)
				m_pCoDraw[iRgnIndex].coWork |= MC_PRC;
			else
				m_pCoDraw[iRgnIndex].coWork &= ~MC_PRC;

			if (iVMMCnt > 1)
				m_pCoDraw[iRgnIndex].coWork |= MC_VOL;
			else
				m_pCoDraw[iRgnIndex].coWork &= ~MC_VOL;

			if (iAMMCnt > 1)
				m_pCoDraw[iRgnIndex].coWork |= MC_AMT;
			else
				m_pCoDraw[iRgnIndex].coWork &= ~MC_AMT;

			// 2006.10.11 schbang
			if (iMacdCnt > 1)
				m_pCoDraw[iRgnIndex].coWork |= MC_MACD;
			else
				m_pCoDraw[iRgnIndex].coWork &= ~MC_MACD;

		}
		iRgnIndex++;
	}

	ReviseMinMax();
}

void CObjMgr::ReviseMinMax()
{
	for (int ii = 0; ii < m_iCoDrawCnt; ii++)
	{
		m_pCoDraw[ii].PMax = DBL_MIN;
		m_pCoDraw[ii].PMin = DBL_MAX;
		m_pCoDraw[ii].VMax = DBL_MIN;
		m_pCoDraw[ii].VMin = DBL_MAX;
		m_pCoDraw[ii].AMax = DBL_MIN;
		m_pCoDraw[ii].AMin = DBL_MAX;
		m_pCoDraw[ii].tickx = 0;
		m_pCoDraw[ii].ticky = 0;
// 2006.10.11 schbang
m_pCoDraw[ii].dMacdMax = DBL_MIN;
m_pCoDraw[ii].dMacdMin = DBL_MAX;
	}

	CIndcBase* pIndcBase;
	struct _CoDraw* pCoDraw;
	for (int ii = 0; ii <= m_arGraphQue.GetUpperBound(); ii++)
	{
		pIndcBase = (CIndcBase*)m_arGraphQue.GetAt(ii);
		switch (pIndcBase->GetGraphKind())
		{
		case GK_JPN:
		case GK_BAR:
		case GK_LIN:
		case GK_POLE:
		case GK_NET:
		case GK_PMA:
		case GK_PARB:
		case GK_ENVL:
		case GK_BOLB:
		case GK_DEMARK:
		case GK_PIVOT:
		case GK_BALANCE:
		case GK_AVOL:
			if (pCoDraw = GetCoDrawInfo(pIndcBase->GetGraphRegionID(), false, MC_PRC))
			{
				if (pCoDraw->coWork & MC_PRC)
				{
					double	dMin, dMax;
					pIndcBase->GetMinMaxValue(dMin, dMax);

					if (dMin != DBL_MAX && dMax != DBL_MIN)
					{
						if (pCoDraw->PMax < dMax)
							pCoDraw->PMax = dMax;
						if (pCoDraw->PMin > dMin)
							pCoDraw->PMin = dMin;
					}
				}
			}
			break;
		case GK_VMA:
		case GK_VOL:
			if (pCoDraw = GetCoDrawInfo(pIndcBase->GetGraphRegionID(), false, MC_VOL))
			{
				if (pCoDraw->coWork & MC_VOL)
				{

					double	dMin, dMax;
					pIndcBase->GetMinMaxValue(dMin, dMax);
					if (dMin != DBL_MAX && dMax != DBL_MIN)
					{
						if (pCoDraw->VMax < dMax)
							pCoDraw->VMax = dMax;
						if (pCoDraw->VMin > dMin)
							pCoDraw->VMin = dMin;
					}
				}
			}
			break;
		case GK_AMA:
		case GK_AMT:
			if (pCoDraw = GetCoDrawInfo(pIndcBase->GetGraphRegionID(), false, MC_AMT))
			{
				if (pCoDraw->coWork & MC_AMT)
				{

					double	dMin, dMax;
					pIndcBase->GetMinMaxValue(dMin, dMax);
					if (dMin != DBL_MAX && dMax != DBL_MIN)
					{
						if (pCoDraw->AMax < dMax)
							pCoDraw->AMax = dMax;
						if (pCoDraw->AMin > dMin)
							pCoDraw->AMin = dMin;
					}
				}
			}
			break;
// 2006.10.11 schbang
case GK_MACD:
case GK_MACDOS:
	if (pCoDraw = GetCoDrawInfo(pIndcBase->GetGraphRegionID(), false, MC_MACD))
	{
		if (pCoDraw->coWork & MC_MACD)
		{

			double	dMin, dMax;
			pIndcBase->GetMinMaxValue(dMin, dMax);
			if (dMin != DBL_MAX && dMax != DBL_MIN)
			{
				if (pCoDraw->dMacdMax < dMax)
					pCoDraw->dMacdMax = dMax;
				if (pCoDraw->dMacdMin > dMin)
					pCoDraw->dMacdMin = dMin;
			}
		}
	}
	break;

		default:
			break;
		}
	}
}

struct _CoDraw* CObjMgr::GetCoDrawInfo(int iRgnKey, bool bTick, int iKind)
{
	if (m_iCoDrawCnt <= 0 || !m_pCoDraw)
		return NULL;

	if (bTick)
	{
		for (int ii = 0; ii < m_iCoDrawCnt; ii++)
		{
			if (m_pCoDraw[ii].region == iRgnKey)
				return &m_pCoDraw[ii];
		}
	}
	else
	{
		if (iKind & MC_VOL)
		{
			for (int ii = 0; ii < m_iCoDrawCnt; ii++)
			{
				if (m_pCoDraw[ii].region == iRgnKey && (m_pCoDraw[ii].coWork & MC_VOL))
					return &m_pCoDraw[ii];
			}
		}
		else if (iKind & MC_AMT)
		{
			for (int ii = 0; ii < m_iCoDrawCnt; ii++)
			{
				if (m_pCoDraw[ii].region == iRgnKey && (m_pCoDraw[ii].coWork & MC_AMT))
					return &m_pCoDraw[ii];
			}
		}
// 2006.10.11 schbang
else if (iKind & MC_MACD)
{
	for (int ii = 0; ii < m_iCoDrawCnt; ii++)
	{
		if (m_pCoDraw[ii].region == iRgnKey && (m_pCoDraw[ii].coWork & MC_MACD))
			return &m_pCoDraw[ii];
	}
}
		else
		{
			for (int ii = 0; ii < m_iCoDrawCnt; ii++)
			{
				if (m_pCoDraw[ii].region == iRgnKey && (m_pCoDraw[ii].coWork & MC_PRC))
					return &m_pCoDraw[ii];
			}
		}
	}

	return NULL;
}

void CObjMgr::SyncGraphDisplay(CIndcBase* pindcSrc, CIndcBase* pindcDst)
{
	int iValue;
	pindcDst->SetDisplayDay(iValue = pindcSrc->GetDisplayDay());
	pindcDst->SetDisplayPos(iValue = pindcSrc->GetDisplayPos());
	pindcDst->SetDisplayEnd(iValue = pindcSrc->GetDisplayEnd());
}

class CIndcBase* CObjMgr::GetVolGraph(int iGKind)
{
	int ii, jj;
	CIndcBase* pIndcBase, *tIndcBase;

	if (iGKind == GK_VOL)
	{
		for (ii = m_arGraphQue.GetUpperBound(); ii >= 0; ii--)
		{
			pIndcBase = m_arGraphQue.GetAt(ii);
			if (pIndcBase->GetGraphKind() == GK_VMA)
			{
				for (jj = m_arGraphQue.GetUpperBound(); jj >= 0; jj--)
				{
					tIndcBase = m_arGraphQue.GetAt(jj);
					if (tIndcBase->GetGraphKind() == GK_VOL &&
						tIndcBase->GetGraphRegionID() == pIndcBase->GetGraphRegionID())
						break;
				}
				if (jj < 0)
					return pIndcBase;
			}
		}
	}

	if (iGKind == GK_VMA)
	{
		for (ii = m_arGraphQue.GetUpperBound(); ii >= 0; ii--)
		{
			pIndcBase = m_arGraphQue.GetAt(ii);
			if (pIndcBase->GetGraphKind() == GK_VOL)
			{
				for (jj = m_arGraphQue.GetUpperBound(); jj >= 0; jj--)
				{
					tIndcBase = m_arGraphQue.GetAt(jj);
					if (tIndcBase->GetGraphKind() == GK_VMA &&
						tIndcBase->GetGraphRegionID() == pIndcBase->GetGraphRegionID())
						break;
				}
				if (jj < 0)
					return pIndcBase;
			}
		}
	}

	return (CIndcBase *) 0;
}

class CIndcBase* CObjMgr::GetAmtGraph(int wGKind)
{
	int	ii, jj;
	CIndcBase* pIndcBase, *tIndcBase;

	if (wGKind == GK_AMT)
	{
		for (ii = m_arGraphQue.GetUpperBound(); ii >= 0; ii--)
		{
			pIndcBase = m_arGraphQue.GetAt(ii);
			if (pIndcBase->GetGraphKind() == GK_AMA)
			{
				for (jj = m_arGraphQue.GetUpperBound(); jj >= 0; jj--)
				{
					tIndcBase = m_arGraphQue.GetAt(jj);
					if (tIndcBase->GetGraphKind() == GK_AMT &&
						tIndcBase->GetGraphRegionID() == pIndcBase->GetGraphRegionID())
						break;
				}
				if (jj < 0)
					return pIndcBase;
			}
		}
	}
	else if (wGKind == GK_AMA)
	{
		for (ii = m_arGraphQue.GetUpperBound(); ii >= 0; ii--)
		{
			pIndcBase = m_arGraphQue.GetAt(ii);
			if (pIndcBase->GetGraphKind() == GK_AMT)
			{
				for (jj = m_arGraphQue.GetUpperBound(); jj >= 0; jj--)
				{
					tIndcBase = m_arGraphQue.GetAt(jj);
					if (tIndcBase->GetGraphKind() == GK_AMA &&
						tIndcBase->GetGraphRegionID() == pIndcBase->GetGraphRegionID())
						break;
				}
				if (jj < 0)
					return pIndcBase;
			}
		}
	}

	return (CIndcBase *) 0;
}

int CObjMgr::GetUnusedRegionKey()
{
	struct	_xRegion* pxRegion;
	for (int iRgnKey = 1; iRgnKey < 254; iRgnKey++)
	{
		if (!m_mapRegion.Lookup(iRgnKey, pxRegion))
			return iRgnKey;
	}

	return 254;
}

int CObjMgr::GetUnusedGraphKey()
{
	CIndcBase *pIndcBase = nullptr;
	int ii, GraphID, gCount = m_arGraphQue.GetSize();
	for (GraphID = 1; GraphID <= gCount; GraphID++)
	{
		for (ii = m_arGraphQue.GetUpperBound(); ii >= 0; ii--)
		{
			pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);
			if (pIndcBase->GetGraphID() == GraphID)
				break;
		}

		if (ii < 0) break;
	}

	return GraphID;
}

void CObjMgr::GetRTSCode(CString &code)
{
	code = _T("");
	m_pDataMgr->GetRtsCode(code);
}

bool CObjMgr::GetCrossValue(CPoint pt, CIndcBase *pIndcBase, bool bForce, CString sYPosVal)
{
	if (!pIndcBase)
	{
		m_strCrossY = m_strCrossX = _T("");
		m_rcCrossX.SetRectEmpty();
		m_rcCrossY.SetRectEmpty();
		return false;
	}

	CString	strY;
	if (sYPosVal.IsEmpty())
		strY = pIndcBase->GetYposValue(pt, false);
	else
		strY = sYPosVal;
	CString	strX = pIndcBase->GetXposValue(pt);

	if (bForce)
	{
		m_strCrossX = strX;
		m_strCrossY = strY;
	}
	else
	{
		if (!strX.IsEmpty())	m_strCrossX = strX;
		if (!strY.IsEmpty())	m_strCrossY = strY;
		if (m_strCrossY.IsEmpty() || m_strCrossX.IsEmpty())
			return false;
	}

	CDC	*pDC = m_pwndPnChart->GetDC();
	CFont	*sFont= pDC->SelectObject(m_pFont);
	CSize szY = pDC->GetOutputTextExtent(m_strCrossY);
	CSize szX = pDC->GetOutputTextExtent(m_strCrossX);
	pDC->SelectObject(sFont);
	m_pwndPnChart->ReleaseDC(pDC);

	CPoint	curPointY, curPointX;
	curPointX = curPointY = pt;

	curPointX.Offset(-(szX.cx+5)/2, 0);
	curPointX.y = m_rectGrp.bottom;

	struct _envInfo	*envInfo = (struct _envInfo*)m_pDataFormat->GetEnvInfo();
	if (envInfo->display.dwDspOption & DO_DISPRT)
	{
		m_rcCrossY.SetRect(m_rectGrp.right, curPointY.y -(szY.cy+5)/2, m_rectObj.right, curPointY.y + (szY.cy+5)/2);
		m_rcCrossX.SetRect(curPointX.x, curPointX.y, curPointX.x + (szX.cx+5), curPointX.y + (szX.cy+5));
	}
	else
	{
		m_rcCrossY.SetRect(m_rectGrp.right - (szY.cx+5), curPointY.y - (szY.cy+5), m_rectGrp.right, curPointY.y);
		m_rcCrossX.SetRect(curPointX.x, curPointX.y, curPointX.x + (szX.cx+5), curPointX.y + (szX.cy+5));
	}
	
	return true;
}

void CObjMgr::ShowCrossLine(CPoint pt, CString sPosVal)
{
	if (!m_pCrossLine->GetVisable())
	{
		m_strCrossY = m_strCrossX = _T("");
		m_rcCrossX.SetRectEmpty();
		m_rcCrossY.SetRectEmpty();

		ReDrawGraphObject(false, true);

		return;
	}

	bool		bFind = false;
	CIndcBase	*pIndcBase = NULL;
	int		dwDspOption = DO_DISPRT || DO_DISPLT;
	struct _envInfo	*envInfo = GetEnvInfo();
	if (envInfo->display.dwDspOption & dwDspOption)
	{
		for (int ii = 0; ii < m_arGraphQue.GetSize(); ii++)
		{
			pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);
			if (pIndcBase->IsPointInGrpRect(pt) && pIndcBase->IsTickDraw(dwDspOption))
			{
				bFind = true;
				break;
			}
		}
	}
	else
	{
		for (int ii = 0; ii < m_arGraphQue.GetSize(); ii++)
		{
			pIndcBase = (CIndcBase *) m_arGraphQue.GetAt(ii);
			if (pIndcBase->IsPointInGrpRect(pt))
			{
				bFind = true;
				break;
			}
		}
	}

	if (!bFind || !pIndcBase)
	{
		m_strCrossY = m_strCrossX = _T("");
		m_rcCrossX.SetRectEmpty();
		m_rcCrossY.SetRectEmpty();

		ReDrawGraphObject(false, true);
		m_pCrossLine->ShowCrossLine(pt);
		return;
	}

	if (!GetCrossValue(pt, pIndcBase, false, sPosVal))
	{
		m_strCrossY = m_strCrossX = _T("");
		m_rcCrossX.SetRectEmpty();
		m_rcCrossY.SetRectEmpty();

		ReDrawGraphObject(false, true);
		m_pCrossLine->ShowCrossLine(pt);
		return;
	}

	ReDrawGraphObject(false, true);
	m_pCrossLine->ShowCrossLine(pt);
}

void CObjMgr::HideCrossLine()
{
	m_strCrossY = m_strCrossX = _T("");

	ReDrawGraphObject(false, true);
	m_rcCrossX.SetRectEmpty();
	m_rcCrossY.SetRectEmpty();

	m_pCrossLine->HideCrossLine();
}

int CObjMgr::CharToInt(const char* cnvB, int cnvL)
{
	char	rWb[64];
	CopyMemory(rWb, cnvB, cnvL);
	rWb[cnvL] = '\0';
	return atoi(rWb);
}

void CObjMgr::MouseEvent(int event, CPoint pt, int dwDspOption)
{
	if (!m_pObjEvent)
		return;

	if (DrawCntIsZero())
		return;

	switch (event)
	{
	case GWE_MSLBDN:
		if (m_analMode)
			OnLButtonDownAnal(pt);
		else
			m_pObjEvent->LButtonDownEvent(pt);
		break;
	case GWE_MSLBUP:
		if (m_analMode)
			OnLButtonUpAnal(pt);
		else
			m_pObjEvent->LButtonUpEvent(pt);
		break;
	case GWE_MSRBDN:
		m_pObjEvent->RButtonDownEvent(pt);
		break;
	case GWE_MSRBUP:
		m_pObjEvent->RButtonUpEvent(pt);
		break;
	case GWE_MSMOVE:
		if (m_analMode)
			OnMouseMoveAnal(pt);
		else {
			m_pObjEvent->MouseMoveEvent(pt, dwDspOption);
			OnMouseMoveForPattern(pt);
		}
		break;
	case GWE_MSDBLC:
		m_pObjEvent->DoubleClickEvent(pt);
		break;
	case GWE_KEYBOARD:
		m_pObjEvent->KeyboardEvent(dwDspOption);
		break;
	case GWE_MSWHEEL:
		m_pObjEvent->MouseWheelEvent(dwDspOption? true:false);
		break;
	case GWE_FOCUS:
		m_pObjEvent->FocusEvent(dwDspOption? true:false);
		break;
	}
}

void CObjMgr::HideGrid()
{
	m_pwndPnChart->KillTimer(TIMER_HIDE);
	m_pObjEvent->ViewGrid(false);
}

void CObjMgr::CreateGrid(bool bCreate)
{
	if (bCreate)
	{
		if (!m_pGrpGrid)
		{
			m_pGrpGrid = new CGrpGrid();
			m_pGrpGrid->Create(m_pwndView, m_pwndPnChart, this);
		}
	}
	else
	{
		if (m_pGrpGrid)
		{	
			delete m_pGrpGrid;		
			m_pGrpGrid = NULL;	
		}
	}
}

BOOL CObjMgr::GetXY(LPCTSTR time, LPCTSTR price, int &x, int &y)
{
	int hh, nn, ss;
	CString tm(time);
	CString prc(price);
	BOOL high, low;

	high = prc == "high";
	low  = prc == "low";
	double value = atof(price);
	
	hh = atoi(tm.Left(2));
	nn = atoi(tm.Mid(2, 2));
	ss = atoi(tm.Mid(4, 2));

	CIndcBase *pIndcBase = GetMainGraph();
	int	dIndex = pIndcBase->GetDataIndex();
	int	dUnit = pIndcBase->GetDataUnit();
	int	dKey = pIndcBase->GetDataKey();
	COrgData	*pOrgData;
	if (!m_pDataMgr->GetOrgData(dKey, pOrgData))
		return false;
	
	int	size = pOrgData->GetArraySize();
	//int size = pIndcBase->GetDisplayEnd() - pIndcBase->GetDisplayPos();
	int ii, index = -1;

	int ts = hh * 60 * 60 + nn * 60 + ss;
	int tss;
	CGrpBasic* gBasic;

	for (ii = size - 1; ii >= 0; ii--) 
	{
		gBasic = pOrgData->GetGraphData(ii);

		if (gBasic->m_iClose == GD_UNUSED) continue;

		tss = gBasic->m_index.time.hh * 60 * 60 + gBasic->m_index.time.mm * 60 + gBasic->m_index.time.ss;

		if (tss <= ts) 
		{
			//Debug("%d.%d.%d    %d.%d.%d", gBasic->m_index.time.hh, gBasic->m_index.time.mm, gBasic->m_index.time.ss, 
			//	hh, nn, ss);
			index = ii;

			if (high)
				value = gBasic->m_iHigh;
			if (low)
				value = gBasic->m_iLow;
			break;
		}
	}
	/**
	for (ii = 0; ii < size; ii++)
	{
		gBasic = (_gBasic *)pOrgData->GetGraphData(ii);
		
		if (gBasic->m_iClose == GD_UNUSED)	continue;

		tss = gBasic->m_index.time.hh * 60 * 60 + gBasic->m_index.time.mm * 60 + gBasic->m_index.time.ss;

		//Debug("%d.%d.%d    %d.%d.%d", gBasic->m_index.time.hh, gBasic->m_index.time.mm, gBasic->m_index.time.ss, 
		//	hh, nn, ss);
		//if (gBasic->m_index.time.hh == hh && gBasic->m_index.time.mm == nn && gBasic->m_index.time.ss == ss) {
		if (ts <= tss && ((tss-ts) < 5)) {
			Debug("%d.%d.%d    %d.%d.%d", gBasic->m_index.time.hh, gBasic->m_index.time.mm, gBasic->m_index.time.ss, 
				hh, nn, ss);
			index = ii;
			break;
		}
	}
	**/

	double dValue;
	int DrawHeight = pIndcBase->GetDrawRect().Height();
	int DrawWidth  = pIndcBase->GetDrawRect().Width();

	if (index >= 0) 
	{
		x = pIndcBase->GetDrawRect().left;
		dValue = double(DrawWidth * (index - pIndcBase->GetDisplayPos())) / double(pIndcBase->GetDisplayDay());

		x += int(dValue);

		/**
		pointFPR.x = m_DrawRect.left;
		dValue = double(DrawWidth * ii) / double(m_dispDay);
		pointFPR.x += int(dValue);
		
		pointEPR.x = m_DrawRect.left;
		dValue = double(DrawWidth * (ii+1)) / double(m_dispDay);
		pointEPR.x += int(dValue);
		pointEPR.x -= distanceGAP;	// 그래프간 간격
		
		pointHPR.x = int(double(pointFPR.x + pointEPR.x - distanceGAP) / 2.);
		pointLPR.x = pointHPR.x;
		**/

	}
	else
		return FALSE;

	double max, min;

	pIndcBase->GetMinMaxValue(min, max);
	//if (pIndcBase->IsLogChart()) {
	//	max = pIndcBase->GetLogData(max);
	//	min = pIndcBase->GetLogData(min);
	//}

	double ValueHeight = max - min;
	if (ValueHeight == 0) 
		ValueHeight = 1;
	
	y = pIndcBase->GetDrawRect().top;

	dValue = double(DrawHeight * (max - value)) / ValueHeight;

	y += (int)dValue;

	if (!y) 
		y = 200;

	return TRUE;
}

void CObjMgr::UnionGraph(CIndcBase* pIndcBase, WORD wGKind, WORD wUnionGKindNew, WORD wUnionGKindOld)
{
	if (!pIndcBase || !wGKind || wUnionGKindNew == wUnionGKindOld)
		return;

	switch (wGKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_BALANCE:
	case GK_POLE:
	case GK_AVOL:
//	case GK_EQV:
//	case GK_CDV:
		break;
	default:
		return;
	}


	// delete
	for (int ii = m_arGraphQue.GetSize() - 1; ii >= 0 && wUnionGKindOld; ii--)
	{
		CIndcBase *pIndcUnion = m_arGraphQue.GetAt(ii);
		if ((pIndcUnion->GetGraphKind() == wUnionGKindOld)
			&& (pIndcUnion->GetGraphRegionID() == pIndcBase->GetGraphRegionID())
			&& (pIndcUnion != pIndcBase)
			&& (pIndcUnion->IsUnion()))
		{
			DeleteGraph(pIndcUnion);
//			break;
		}
	}

	switch (wUnionGKindNew)
	{
	case GK_PNF:
	case GK_SWING:
	case GK_THREE:
	case GK_PV:
	case GK_KAGI:
	case GK_RENKO:
	case GK_GUKKI:
	case GK_AVOL:	// 매물차트
		break;
	default:
		return;
	}

	// add
	struct _graph graphAdd;
	ZeroMemory(&graphAdd, sizeof(graphAdd));
	graphAdd.wGKind = wUnionGKindNew;

	BOOL bRet = (BOOL)m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewChartInfo, getSGrpInfo), (long)&graphAdd);
	
	graphAdd.btRgnKey = pIndcBase->GetGraphRegionID();
	graphAdd.btDataKey = pIndcBase->GetDataKey();
	graphAdd.wKey = GetUnusedGraphKey();
	//graphAdd.btGType = GT_VARS;
	graphAdd.btGType = (wUnionGKindNew == GK_AVOL ? GT_BONG : GT_VARS);
	graphAdd.wGKind = wUnionGKindNew;
	graphAdd.dwDraOption |= GO_UNION;
	

	int iRgnCnt, iGrpCnt;
	char* pcGrpInfo = m_pDataFormat->GetGrpInfo(iRgnCnt, iGrpCnt);

//	graphAdd.btRgnKey = iAddRgn;
	char* pNewGrpInfo = new char[SZ_REGION * iRgnCnt + SZ_GRAPH * (iGrpCnt + 1)];
	CopyMemory(pNewGrpInfo, pcGrpInfo, SZ_REGION * iRgnCnt);
	CopyMemory(&pNewGrpInfo[SZ_REGION * iRgnCnt], &pcGrpInfo[SZ_REGION * iRgnCnt], SZ_GRAPH * iGrpCnt);
	CopyMemory(&pNewGrpInfo[SZ_REGION * iRgnCnt + SZ_GRAPH * iGrpCnt], &graphAdd, SZ_GRAPH);

	m_pDataFormat->SetFgraph(pNewGrpInfo, iRgnCnt, iGrpCnt + 1);

	delete[] pNewGrpInfo;



	//CIndcBase* pGrpAppend = AddGraphQue(1, (char *)&graphAdd, false, true);
	CIndcBase* pGrpAppend = AddGraphQue(1, (char *)&graphAdd, true, true);


	SyncGraphDisplay(pIndcBase, pGrpAppend);
}

void CObjMgr::YAxisConfig(CIndcBase* pIndcBase)
{
	if (!pIndcBase)
		return;

	int iRgnCnt, iGrpCnt;
	char* pcGrpInfo = m_pDataFormat->GetGrpInfo(iRgnCnt, iGrpCnt);
//	DWORD dwDraOption = ((struct _graph *)pIndcBase->GetGrpInfo())->dwDraOption;

	struct _graph* pGrp = NULL;
	switch (pIndcBase->GetGraphKind())
	{
	case GK_BALANCE:
		pGrp = m_pDataFormat->GetBalanceGrp();
		break;
	case GK_AVOL:
		pGrp = m_pDataFormat->GetAVolGrp();
		break;
	case GK_CDVV:
	case GK_EQVV:
		return;
	default:
		{
			int ii = 0;
			for (ii = 0; ii < iGrpCnt; ii++)
			{
				pGrp = (struct _graph*)&pcGrpInfo[SZ_REGION * iRgnCnt + SZ_GRAPH * ii];
				if (UINT(pIndcBase->GetGraphID()) == pGrp->wKey)
					break;
			}

			if (ii >= iGrpCnt)
				return;
		}
		break;
	}
	
	if (!pGrp)
		return;

	BOOL bRet = (BOOL)m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, DlgYAxisCfg), (LPARAM)pGrp);
	if (!bRet)
		return;

	ChangeEnvInfo();
	_envInfo* penvinfo = GetEnvInfo();
	m_pwndPnChart->SendMessage(GEV_SISE, MAKEWPARAM(siseChgEnv, 0), penvinfo->display.rgbBG);

	pIndcBase->SetGrpInfo(pGrp, NULL, NULL);


	ReviseTick();
//	ReviseMinMax();
	ResizeGraphObject();
	ReDrawGraphObject(true);

}

// 2006.10.12 schbang 수정
bool CObjMgr::YShowConfigByMove(CIndcBase* pindcMove, int iMovRgn)
{
	if (!pindcMove)
		return false;

	int iRet = (int)m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, DlgYShowCfg), NULL);
	if (iRet == -1)	// 취소
		return false;

	CIndcBase* pincTemp;
	struct _graph* pGraph;
	pGraph = (struct _graph*)pindcMove->GetGrpInfo();
	DWORD dwDraOption = NULL;
	if (pGraph->dwDraOption & GO_TICK_LEFT_DRAG)
		dwDraOption |= GO_TICK_LEFT_DRAG;
	if (pGraph->dwDraOption & GO_TICK_RIGHT_DRAG)
		dwDraOption |= GO_TICK_RIGHT_DRAG;

	for (int ii = 0; ii < m_arGraphQue.GetSize(); ii++)
	{
		pincTemp = m_arGraphQue.GetAt(ii);

		// 2006.10.12 schbang 이전의 같은 rgn 일경우 드래그 정보 clear
		if (pincTemp->GetGraphRegionID() == pindcMove->GetGraphRegionID())
		{
			pGraph = (struct _graph*)pincTemp->GetGrpInfo();
			pGraph->dwDraOption &= ~(dwDraOption);

		}
	}

	_envInfo* penvinfo = GetEnvInfo();
	pGraph = (struct _graph*)pindcMove->GetGrpInfo();

	switch (iRet)
	{
	case 0:	// none
		break;
	case 1:	// right
		for (int ii = 0; ii < m_arGraphQue.GetSize(); ii++)
		{
			pincTemp = m_arGraphQue.GetAt(ii);

			// 2006.10.12 schbang 이전의 같은 rgn 일경우 드래그 정보 clear
			if (pincTemp->GetGraphRegionID() == iMovRgn)
			{
				struct _graph* pGrp = (struct _graph*)pincTemp->GetGrpInfo();
				pGrp->dwDraOption &= ~(GO_TICK_RIGHT_DRAG);
			}
		}

		pGraph->dwDraOption |= GO_TICK_RIGHT_DRAG;
		penvinfo->display.dwDspOption |= DO_DISPRT;
		break;
	case 2:	// left
		for (int ii = 0; ii < m_arGraphQue.GetSize(); ii++)
		{
			pincTemp = m_arGraphQue.GetAt(ii);

			// 2006.10.12 schbang 이전의 같은 rgn 일경우 드래그 정보 clear
			if (pincTemp->GetGraphRegionID() == iMovRgn)
			{
				struct _graph* pGrp = (struct _graph*)pincTemp->GetGrpInfo();
				pGrp->dwDraOption &= ~(GO_TICK_LEFT_DRAG);
			}
		}

		pGraph->dwDraOption |= GO_TICK_LEFT_DRAG;
		penvinfo->display.dwDspOption |= DO_DISPLT;
		break;
	case 3:	// both
		for (int ii = 0; ii < m_arGraphQue.GetSize(); ii++)
		{
			pincTemp = m_arGraphQue.GetAt(ii);

			// 2006.10.12 schbang 이전의 같은 rgn 일경우 드래그 정보 clear
			if (pincTemp->GetGraphRegionID() == iMovRgn)
			{
				struct _graph* pGrp = (struct _graph*)pincTemp->GetGrpInfo();
				pGrp->dwDraOption &= ~(GO_TICK_LEFT_DRAG | GO_TICK_RIGHT_DRAG);
			}
		}

		pGraph->dwDraOption |= GO_TICK_LEFT_DRAG | GO_TICK_RIGHT_DRAG;
		penvinfo->display.dwDspOption |= DO_DISPLT | DO_DISPRT;
		break;
	default:	// error
		TRACE("CObjMgr::YShowConfigByMove() Error\n");
		return false;
	}

	return true;
}

bool CObjMgr::YShowConfigByMenu(CIndcBase* pindcConfig)
{
	if (!pindcConfig)
		return false;

	int iRet = (int)m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, DlgYShowCfg), NULL);
	if (iRet == -1)	// 취소
		return false;

	CIndcBase* pincTemp;
	struct _graph* pGraph;
	pGraph = (struct _graph*)pindcConfig->GetGrpInfo();

	_envInfo* penvinfo = GetEnvInfo();
	pGraph = (struct _graph*)pindcConfig->GetGrpInfo();
	int ii;
	switch (iRet)
	{
	case 0:	// none
		pGraph->dwDraOption &= ~(GO_TICK_LEFT_DRAG | GO_TICK_RIGHT_DRAG);
		break;
	case 1:	// right
		for (ii = 0; ii < m_arGraphQue.GetSize(); ii++)
		{
			pincTemp = m_arGraphQue.GetAt(ii);

			// 이전의 같은 rgn 일경우 드래그 정보 clear
			if (pincTemp->GetGraphRegionID() == pindcConfig->GetGraphRegionID())
			{
				struct _graph* pGrp = (struct _graph*)pincTemp->GetGrpInfo();
				pGrp->dwDraOption &= ~(GO_TICK_RIGHT_DRAG);
			}
		}

		pGraph->dwDraOption |= GO_TICK_RIGHT_DRAG;
		pGraph->dwDraOption &= ~GO_TICK_LEFT_DRAG;
		penvinfo->display.dwDspOption |= DO_DISPRT;
		break;
	case 2:	// left
		for (ii = 0; ii < m_arGraphQue.GetSize(); ii++)
		{
			pincTemp = m_arGraphQue.GetAt(ii);

			// 이전의 같은 rgn 일경우 드래그 정보 clear
			if (pincTemp->GetGraphRegionID() == pindcConfig->GetGraphRegionID())
			{
				struct _graph* pGrp = (struct _graph*)pincTemp->GetGrpInfo();
				pGrp->dwDraOption &= ~(GO_TICK_LEFT_DRAG);

			}
		}

		pGraph->dwDraOption |= GO_TICK_LEFT_DRAG;
		pGraph->dwDraOption &= ~GO_TICK_RIGHT_DRAG;
		penvinfo->display.dwDspOption |= DO_DISPLT;
		break;
	case 3:	// both
		for (ii = 0; ii < m_arGraphQue.GetSize(); ii++)
		{
			pincTemp = m_arGraphQue.GetAt(ii);

			// 이전의 같은 rgn 일경우 드래그 정보 clear
			if (pincTemp->GetGraphRegionID() == pindcConfig->GetGraphRegionID())
			{
				struct _graph* pGrp = (struct _graph*)pincTemp->GetGrpInfo();
				pGrp->dwDraOption &= ~(GO_TICK_LEFT_DRAG | GO_TICK_RIGHT_DRAG);

			}
		}

		pGraph->dwDraOption |= GO_TICK_LEFT_DRAG | GO_TICK_RIGHT_DRAG;
		penvinfo->display.dwDspOption |= DO_DISPLT | DO_DISPRT;
		break;
	default:	// error
		TRACE("CObjMgr::YShowConfigByMenu() Error\n");
		return false;
	}

	return true;
}

BOOL CObjMgr::IsExistGraph(int iKind)
{
	CIndcBase* p;
	for (int i = 0; i <= m_arGraphQue.GetUpperBound(); i++) 
	{
		p = m_arGraphQue.GetAt(i);
		if (p->GetGraphKind() == iKind)
			return TRUE;
	}

	return FALSE;
}

BOOL CObjMgr::DrawCntIsZero()
{
	return (m_iCoDrawCnt <= 0)? TRUE:FALSE;
}

void CObjMgr::SetPattern(BOOL patternStatus, bool bUP)
{
	if (bUP)
	{
		if (m_patternUP == patternStatus) 
			return;

			m_patternUP = patternStatus;
	}
	else
	{
		if (m_patternDN == patternStatus) 
			return;

			m_patternDN = patternStatus;
	}

	ClearPattern();
	ReDrawGraphObject(true);
}

void CObjMgr::ClearPattern()
{
	if (m_pPattern) 
	{
		delete m_pPattern;
		m_pPattern = NULL;
	}
}

void CObjMgr::MakePattern(CIndcBase* indc)
{
	ClearPattern();

	COrgData *pOrgData;
	CGrpBasic* gBasic;
	DATA	data;
	ZeroMemory(&data, sizeof(DATA));

	if (!m_pDataMgr->GetOrgData(indc->GetDataKey(), pOrgData)) 
		return;

	m_pPattern = new CAxPattern((LPCTSTR)m_pwndPnChart->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, getRootPath)));
	for (int i = 0; i < pOrgData->GetArraySize(); i++) 
	{
		gBasic = pOrgData->GetGraphData(i);

		data.date.yy = gBasic->m_index.date.yy;
		data.date.mm = gBasic->m_index.date.mm;
		data.date.dd = gBasic->m_index.date.dd;
		data.time.hh = gBasic->m_index.time.hh;
		data.time.mm = gBasic->m_index.time.mm;
		data.time.ss = gBasic->m_index.time.ss;

		data.open    = (float)gBasic->m_iOpen;
		data.high    = (float)gBasic->m_iHigh;
		data.low     = (float)gBasic->m_iLow;
		data.close   = (float)gBasic->m_iClose;
		data.xvolume = (float)gBasic->m_dVolume;
		
		m_pPattern->AddData(data);
	}
}

void CObjMgr::DrawPattern(CDC* dc, CIndcBase* indc)
{
	if (!dc || !indc) 
		return;

	if (m_iDtIndex == GI_TICK)
		return;

	struct _envInfo	*envInfo = (struct _envInfo*)GetEnvInfo();
	switch (envInfo->datainfo.wMainGrpKind) 
	{
	case GK_JPN: case GK_BALANCE: case GK_AVOL:
		break;
	default:
		return;
	}

	if (!m_pPattern) MakePattern(indc);

	m_pPattern->FindPattern(6, m_patternUP, m_patternDN);

	LPDATA data;
	CPoint pt;
	CRect  rc = indc->GetChartRect();
	
	BOOL reverse = (envInfo->display.dwDspOption & DO_REVERSE);

	int high, low;
	int width, height;
	int x;
	const int arrowGap = 10;

	CDC		mdcBearUp, mdcBearDn, mdcBullUp, mdcBullDn;
	HBITMAP		sBmpBearUp = nullptr, sBmpBearDn = nullptr, sBmpBullUp = nullptr, sBmpBullDn = nullptr;
	CFont		font, *sFont = NULL;
	COLORREF	sTextColor;
	int		sMode;

	if (m_bmpMode) 
	{
		width = m_bmWidth;
		height = m_bmHeight;

		mdcBearUp.CreateCompatibleDC(dc);
		mdcBearDn.CreateCompatibleDC(dc);
		mdcBullUp.CreateCompatibleDC(dc);
		mdcBullDn.CreateCompatibleDC(dc);

		sBmpBearUp = (HBITMAP)::SelectObject(mdcBearUp.m_hDC, m_bmpPatternBearUp);
		sBmpBearDn = (HBITMAP)::SelectObject(mdcBearDn.m_hDC, m_bmpPatternBearDn);
		sBmpBullUp = (HBITMAP)::SelectObject(mdcBullUp.m_hDC, m_bmpPatternBullUp);
		sBmpBullDn = (HBITMAP)::SelectObject(mdcBullDn.m_hDC, m_bmpPatternBullDn);
	}
	else 
	{
		font.CreatePointFont(240, "굴림");
		LOGFONT lf; font.GetLogFont(&lf);

		lf.lfWeight |= FW_BOLD;
		font.CreateFontIndirect(&lf);
		
		sFont = dc->SelectObject(&font);
		sTextColor = dc->SetTextColor(RGB(0xFF, 0, 0));
		sMode = dc->SetBkMode(TRANSPARENT);

		CSize sz = dc->GetTextExtent("↑");
		width = sz.cx - 14;  // 문자 빈 공간 만큼 보정.
		height = sz.cy;
	}

	const char* txt[] = { "", "↑", "↓" };

	int	preIdx = -2;	// for skip 
	HDC	hDC = NULL;
	for (int i = 0; i < m_pPattern->GetCount(); i++) 
	{
		data = m_pPattern->GetData(i);
		if (data->pattern) 
		{
			if (preIdx == (i-1))	// 연속이면 뒤에거는 skip
				continue;

			indc->GetValueToPoint(pt, i, reverse ? data->low: data->high);
			x = pt.x;
			high = pt.y;
			indc->GetValueToPoint(pt, i + 1, reverse ? data->high: data->low);
			low = pt.y;
			data->rc.left = (int)((x + pt.x) / 2.0 - (width / 2.0));
			data->rc.right = data->rc.left + width;

			if (high + low / 2 < rc.Height() / 2) 
			{
				data->arrow = ARR_UP;
				data->rc.top = low + arrowGap;
				data->rc.bottom = data->rc.top + height;

				if (data->pattern <= UpsideGap3Method)
					hDC = mdcBullUp.m_hDC;
				else
					hDC = mdcBearUp.m_hDC;
			}
			else 
			{
				data->arrow = ARR_DN;
				data->rc.bottom = high - arrowGap;
				data->rc.top = data->rc.bottom - height;

				if (data->pattern <= UpsideGap3Method)	
					hDC = mdcBullDn.m_hDC;
				else
					hDC = mdcBearDn.m_hDC;
			}

			if (rc.PtInRect(CPoint(data->rc.left, data->rc.top))) 
			{
				data->draw = TRUE;
				if (m_bmpMode) 
				{
					TransparentBlt(dc->m_hDC, data->rc.left, data->rc.top, data->rc.right - data->rc.left, data->rc.bottom - data->rc.top,
						hDC, 0, 0, m_bmWidth, m_bmHeight, RGB(255, 255, 255));
				}
				else 
				{
					dc->DrawText(CString(txt[data->arrow]), CRect(data->rc), DT_CENTER | DT_VCENTER);
				}
			}
			else 
			{
				data->draw = FALSE;
			}
			preIdx = i;
		}
	}

	if (m_bmpMode) 
	{
		if (sBmpBearUp != nullptr) ::SelectObject(mdcBearUp.m_hDC, sBmpBearUp);
		if (sBmpBearDn != nullptr) ::SelectObject(mdcBearDn.m_hDC, sBmpBearDn);
		if (sBmpBullUp != nullptr) ::SelectObject(mdcBullUp.m_hDC, sBmpBullUp);
		if (sBmpBullDn != nullptr) ::SelectObject(mdcBullDn.m_hDC, sBmpBullDn);
	}
	if (sFont) 
	{
		dc->SelectObject(sFont);
		dc->SetTextColor(sTextColor);
		dc->SetBkMode(sMode);
	}
}

void CObjMgr::OnMouseMoveForPattern(CPoint pt)
{
	if ((!m_patternUP && !m_patternDN) || !m_pPattern) 
	{
		PatternReset();
		return;
	}

	CIndcBase* indc = GetMainGraph();
	if (!indc) 
	{
		PatternReset();
		return;
	}

	CRect rcChart = indc->GetChartRect();
	if (!rcChart.PtInRect(pt)) 
	{
		PatternReset();
		return;
	}

	LPDATA data;
	CString datetime, alias, description, imageName;
	alias.Empty();

	for (int i = m_pPattern->GetCount() - 1; i >= 0; i--) 
	{
		data = m_pPattern->GetData(i);
		if (data->draw) 
		{
			if (::PtInRect(&data->rc, pt)) 
			{
				if (m_iDtIndex == GI_MINUTE) 
				{
					datetime.Format("%04d.%02d.%02d %02d:%02d",
						data->date.yy, data->date.mm, data->date.dd,
						data->time.hh, data->time.mm);
				}
				else 
				{
					datetime.Format("%04d.%02d.%02d", data->date.yy, data->date.mm, data->date.dd);
				}
				alias.Format("%s (%s)", m_pPattern->GetPatternName(data->pattern), 
					m_pPattern->GetPatternNameEng(data->pattern));
				description = m_pPattern->GetDescription(data->pattern);
				imageName = m_pPattern->GetPatternImage(data->pattern);
				break;
			}
		}			
	}

	if (alias.IsEmpty()) 
	{
		PatternReset();
	}
	else 
	{
		if (m_prevPatternDate != datetime) 
		{
			CSize sz = m_patternToolTip->SetPattern(datetime, alias, imageName, description);

			CPoint pt;
			::GetCursorPos(&pt);
			int scrWidth = GetSystemMetrics(SM_CXSCREEN);
			if (pt.x + sz.cx > scrWidth) 
				pt.x = scrWidth - sz.cx;
			m_patternToolTip->WindowMove(pt.x, pt.y + 20, sz.cx, sz.cy);
			m_prevPatternDate = datetime;

			m_patternToolTip->SetTimer(1, 200, NULL);
		}
		else 
		{
			
		}
	}
}

void CObjMgr::DeletePatternToolTip()
{
	if (m_patternToolTip) 
	{
		m_patternToolTip->DestroyWindow();
		delete m_patternToolTip;
		m_patternToolTip = NULL;
	}
}

void CObjMgr::PatternReset()
{
	m_patternToolTip->KillTimer(1);
	m_patternToolTip->ShowWindow(SW_HIDE);
	m_prevPatternDate.Empty();
}

void CObjMgr::OnLButtonDownAnal(CPoint pt)
{
	CIndcBase* indc = GetMainGraph();
	if (!indc) return;

	double x, y;
	indc->GetPtToPosValue(pt, x, y);
	m_analStart = m_analEnd = (int)x;
	m_mouseDown = TRUE;
}

//** macho@2007.01.08 <start>
#define ANAL_COUNT 20

void CObjMgr::OnLButtonUpAnal(CPoint pt)
{
	if (m_analStart == ANAL_NONE)
		return;

	CIndcBase* indc = GetMainGraph();
	if (!indc) return;

	double x, y;
	indc->GetPtToPosValue(pt, x, y);
	//int index = indc->GetDisplayPosIndex(pt);

	m_analEnd = (int)x;

	if ((abs(m_analEnd - m_analStart) + 1) > ANAL_COUNT) 
	{
		if (m_analEnd > m_analStart)
			m_analEnd = m_analStart + ANAL_COUNT - 1;
		else
			m_analEnd = m_analStart - ANAL_COUNT + 1;
	}

	m_mouseDown = FALSE;

	if ((abs(m_analEnd - m_analStart) + 1) != ANAL_COUNT) 
		RemoveAnal();
	else 
		CreateAnal();

	ReDrawGraphObject(true);
}

void CObjMgr::OnMouseMoveAnal(CPoint pt)
{
	if (m_analStart == ANAL_NONE || !m_mouseDown)
		return;

	CIndcBase* indc = GetMainGraph();
	if (!indc) return;

	double x, y;
	indc->GetPtToPosValue(pt, x, y);

	m_analEnd = (int)x;

	if ((abs(m_analEnd - m_analStart) + 1) > ANAL_COUNT) 
	{
		if (m_analEnd > m_analStart)
			m_analEnd = m_analStart + ANAL_COUNT - 1;
		else
			m_analEnd = m_analStart - ANAL_COUNT + 1;
	}

	ReDrawGraphObject(true);
}

BOOL CObjMgr::DrawAnalArea(CDC* dc)
{
	int start = m_analStart, end = m_analEnd;

	if (start == end) 
		return FALSE;

	CIndcBase* indc = GetMainGraph();
	if (!indc)
		return FALSE;

	CPoint pt1, pt2;

	indc->GetValueToPoint(pt1, start + .5, 0);
	indc->GetValueToPoint(pt2, end + .5, 0);

	if (pt1.x > pt2.x) 
	{
		int temp = pt1.x;
		pt1.x = pt2.x;
		pt2.x = temp;
	}

	CRect rc = indc->GetChartRect();

	pt1.y = rc.top + 1;
	pt2.y = rc.bottom - 1;

	if (!rc.PtInRect(pt1) || !rc.PtInRect(pt2)) 
	{
		RemoveAnal();
		return FALSE;
	}

	CPen pen;
	CBrush br;
	if (abs(end - start) + 1 == ANAL_COUNT) 
	{
		pen.CreatePen(PS_SOLID, 1, COLORREF(0));
		br.CreateSolidBrush(RGB(225, 240, 255));
	}
	else 
	{
		pen.CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
		br.CreateSolidBrush(RGB(192, 192, 192));
	}
	CPen	*sPen = dc->SelectObject(&pen);
	CBrush	*sBrush = dc->SelectObject(&br);

	dc->Rectangle(pt1.x, pt1.y, pt2.x, pt2.y);
	dc->SelectObject(sBrush);
	dc->SelectObject(sPen);

	return TRUE;
}
//** macho@2007.01.08 <end>

void CObjMgr::RemoveAnal()
{
	m_analStart = m_analEnd = ANAL_NONE;
	m_mouseDown = FALSE;

	if (m_analNotifyMessageHandler && ::IsWindow(m_analNotifyMessageHandler->m_hWnd) && m_analNotifyMessageID)
		m_analNotifyMessageHandler->SendMessage(m_analNotifyMessageID, analAreaDeleted);
}

void CObjMgr::CreateAnal()
{
	if (m_analNotifyMessageHandler && ::IsWindow(m_analNotifyMessageHandler->m_hWnd) && m_analNotifyMessageID) 
	{
		int start, end;
		if (m_analStart == m_analEnd)
			return;

		if (m_analStart < m_analEnd) 
		{
			start = m_analStart;
			end = m_analEnd;
		}
		else 
		{
			start = m_analEnd;
			end = m_analStart;
		}

		CIndcBase* indc = GetMainGraph();
		if (!indc) 
			return;

		COrgData* pOrgData;
		CGrpBasic* gBasic;

		if (!m_pDataMgr->GetOrgData(indc->GetDataKey(), pOrgData)) 
			return;

		int count = end - start + 1;

		LPBYTE pa = new BYTE[sizeof(ANALAREA) + (count-1) * sizeof(ANALPRICE)];
		LPANALAREA aa = (LPANALAREA)pa;

		//** macho@2008.01.08 <start>
		strcpy_s(aa->code, sizeof(aa->code), (char*)m_pwndPnChart->SendMessage(GEV_SISE, MAKEWPARAM(siseData, siseCode), 0));
		aa->count = 0;

		for (int i = 0; i < count; i++) 
		{
			gBasic = pOrgData->GetGraphData(i + start);
			if (gBasic) 
			{
				aa->data[i].date.yy = gBasic->m_index.date.yy;
				aa->data[i].date.mm = gBasic->m_index.date.mm;
				aa->data[i].date.dd = gBasic->m_index.date.dd;
				aa->data[i].time.hh = gBasic->m_index.time.hh;
				aa->data[i].time.mm = gBasic->m_index.time.mm;
				aa->data[i].time.ss = gBasic->m_index.time.ss;
					
				aa->data[i].open = gBasic->m_iOpen;
				aa->data[i].high = gBasic->m_iHigh;
				aa->data[i].low  = gBasic->m_iLow;
				aa->data[i].close = gBasic->m_iClose;
				aa->data[i].volume = gBasic->m_dVolume;
				aa->count++;
			}
		}
		//** macho@2008.01.08 <end>

		m_analNotifyMessageHandler->SendMessage(m_analNotifyMessageID, analAreaSelected, (LPARAM)pa);
		delete [] pa;
	}
}

LRESULT CObjMgr::AnalMessageProc(WORD msgKind, LPARAM lParam)
{
	switch (msgKind) 
	{
	case initAnalMode:
		RemoveAnal();
		ReDrawGraphObject(true);
		break;
	case setAnalMode:
		m_analMode = lParam;
		ReDrawGraphObject(true);
		break;
	case getAnalMode:
		return m_analMode;
	case setMessageHandler:
		m_analNotifyMessageHandler = (CWnd*)lParam;
		break;
	case setMessageID:
		m_analNotifyMessageID = lParam;
		break;
	}
	return TRUE;
}
