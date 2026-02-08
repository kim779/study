#include "stdafx.h"
#include "C_Avol.h"
#include "GrpWnd.h"
#include "memDC.h"
#include "Grp_Data.h"
#include "Grp_Base.h"
#include "Grp_Juga.h"
#include "ToolTip.h"
#include "CrossLine.h"
#include "MainWnd.h"

#include "../../h/axiscgrp.h"
#include <math.h>
#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// ECN realtime Symbol
#define	ER_HNAM	"722"	// 종목명
#define	ER_CURR	"723"	// 현재가
#define	ER_DIFF	"724"	// 전일대비
#define	ER_MDGA	"725"	// 매도호가
#define	ER_MSGA	"726"	// 매수호가
#define	ER_GVOL	"727"	// 거래량
#define	ER_GAMT	"728"	// 거래대금(억)
#define	ER_SIGA	"729"	// 시가
#define	ER_KOGA	"730"	// 고가
#define	ER_JEGA	"731"	// 저가
#define	ER_CVOL	"732"	// 체결거래량(체결수량)
#define	ER_UDYL	"733"	// 등락율
#define	ER_CTIM	"734"	// 체결시간
#define	ER_CCOL	"735"	// 체결구분
#define	ER_HTIM	"740"	// 호가시간
/////////////////////////////////////////////////////////////////////////////
// CGrpWnd
#define	TIMER_PID	10000
#define	TIMER_PGAP	250
#define	TIMER_ID	10001
#define	TIMER_GAP	1000

CGrpWnd::CGrpWnd(CWnd *pView, CWnd *pParent, struct _param *pInfo)
{

	m_pView = pView;
	m_pParent = pParent;
	m_pToolTip = std::make_unique<CToolTip>(m_pView, this);
	m_pCrossLine = nullptr;	
	m_RgnCnt = m_DataCnt = m_GrpCnt = 0;

	for (int ii = 0; ii < maxCnt; ii++)
	{
		m_pRgnInfo[ii] = nullptr;
		m_pDataInfo[ii] = nullptr;
		m_pGrpInfo[ii] = nullptr;
	}

	m_MinMaxCnt = 0;
	m_nPoll = 10;

	m_bOutLine = false;
	m_jChart = 0;
	m_dKind = CDK_BONG;
	m_dIndex = CDI_DAY;
	m_dCount = 100;
	m_dDay = 100;
	m_dUnit = CDU_JONGMOK;

	m_digit = 0;
	m_fPoint = pInfo->point;
	m_fName.Format("%s", pInfo->fonts);
	m_tRGB = GetPalette(pInfo->tRGB);
	m_pRGB = GetPalette(pInfo->pRGB);
	m_fRGB = GetPalette(BGIndex);

	m_TickView = CDT_LEFT;
	m_ObjRect = pInfo->rect;
	m_poleType = PT_2D;
	m_bCtrl = ((CMainWnd*)m_pParent)->IsUseCtrl();
	ParseParam((char*)pInfo->options.operator LPCTSTR());	
	m_ltColor = m_tRGB;
	m_rtColor = m_tRGB;
	m_btColor = m_tRGB;

	m_PosData = _T("");
	m_timerID = 0;

	m_pApp = (CC_AVolApp*)AfxGetApp();
}

CGrpWnd::~CGrpWnd()
{
}


BEGIN_MESSAGE_MAP(CGrpWnd, CWnd)
	//{{AFX_MSG_MAP(CGrpWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(CM_GRP, OnGrp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGrpWnd message handlers

LONG CGrpWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	const LONG ret = 0;
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:	// set data from server
		return DispatchData(wParam, lParam);
		break;
	case DLL_INB:	// get data from out
		return RequestHead();
		break;
	case DLL_ALERT:	// realtime data
		RealTimeData((char *)lParam);
		break;
	case DLL_SETPAL:
		m_fRGB = GetPalette(BGIndex);
		Invalidate();
		break;
	case DLL_SETFONT:
		{
			m_fPoint = HIWORD(wParam);
			CString fName = (char *)lParam;

			if (!fName.IsEmpty())
				m_fName.Format("%s", fName);

			m_pFont = m_pApp->GetFont(m_pView, m_fPoint, m_fName);
			Resize();
			Invalidate();
		}
		break;
	}

	return ret;
}

LONG CGrpWnd::OnGrp(WPARAM wParam, LPARAM lParam)
{
	const LONG	ret = 0;

	switch (LOWORD(wParam))
	{
	case GRP_Control:
		switch (HIWORD(wParam))
		{
		case GetScrData:
			{
				CString	tmpS;
				tmpS.Format("%d\t%d\t%d\t%d", m_totalDay, m_dispPos, m_dispEnd, m_dispDay);
				memcpy((char *)lParam, tmpS, tmpS.GetLength());
				return tmpS.GetLength();
			}
			break;
		case ChgDispPos:	// change pos, LOWORD(lParam) : dispPos, HIWORD(lParam) : dispEnd
			m_dispPos = LOWORD(lParam);
			m_dispEnd = HIWORD(lParam);
			RecalculateMinMax();
			Invalidate();
			break;
		case ChgDispScale:	// change scale, lParam:scale
			if (ChangeDisplayScale(lParam))
			{
				RecalculateMinMax();
				Invalidate();
			}
			break;
		case ChgDisplay:	// change display, lParam:display cnt
			if (ChangeDisplay(lParam))
			{
				RecalculateMinMax();
				Invalidate();
				return TRUE;
			}
			break;
		case autoScroll:
			break;
		case ChgShift:
			return ChangeDisplayShift(lParam);
		case ExcelData:
			ExcelProc();
			break;
		}
		break;
	}

	return ret;
}

int CGrpWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pToolTip->Create();
	m_pToolTip->ShowWindow(SW_HIDE);
	m_pFont = m_pApp->GetFont(m_pView, m_fPoint, m_fName);
	return 0;
}

void CGrpWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC	mdc(&dc);
	DrawGraph(&mdc);
}

void CGrpWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx == 0 || cy == 0)
		return;

	Resize();
}

void CGrpWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TIMER_PID)
	{
		KillTimer(nIDEvent);
		if (IsPtInGrp())
		{
			CPoint	pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			CString str = GetTipStr(pt);

			if (m_PosData == str)
				MouseMove(true, pt);
		}
	}

	if (nIDEvent == TIMER_ID)
	{	
		if (!IsPtInGrp())
		{
			KillTimer(nIDEvent);
			m_pToolTip->ShowWindow(SW_HIDE);
			m_pCrossLine->HideCrossLine();
			m_timerID = 0;
		}
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CGrpWnd::OnDestroy() 
{
	if (m_timerID > 0)
		KillTimer(m_timerID);

	if (m_pToolTip)
		m_pToolTip->DestroyWindow();

	RemoveComponent();
	CWnd::OnDestroy();
}

void CGrpWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CString	str = GetTipStr(point);
	if (m_PosData != str)
	{
		m_pToolTip->ShowWindow(SW_HIDE);
		m_PosData.Format("%s", str);
		KillTimer(TIMER_PID);
		SetTimer(TIMER_PID, TIMER_PGAP, nullptr);
	}

	MouseMove(false, point, true);

	CWnd::OnMouseMove(nFlags, point);
}

int CGrpWnd::DispatchData(WPARAM wParam, LPARAM lParam)
{
//	int	dlen = HIWORD(wParam);
//	DWORD	dAdd = HIBYTE(LOWORD(wParam));
//	char	*pData = (char *)lParam;
//	dAdd = (dAdd << 16) & 0x00FF0000;
//	dlen |= dAdd;

	struct _extTHx* exTH = (struct _extTHx*)lParam;
	const int dlen = exTH->size;
	char* pData = exTH->data;


	RemoveComponent();

	if (dlen <= sz_CDJUGA)
	{
		Invalidate();
		return dlen;

	}

	const struct _cdJuga	*pcdJuga = (struct _cdJuga *)pData;
	const int	dCnt = CGrp_Data::CharToInt(pcdJuga->nData, sizeof(pcdJuga->nData));
	if (dCnt <= 0)
	{
		Invalidate(FALSE);
		return sz_CDJUGA;
	}

	const int	nFullsize = (sz_CDJUGA + sizeof(struct _cdBong) * dCnt);

	if (dlen != nFullsize)
	{
		Invalidate(FALSE);
		return dlen;
	}

	if (!AssignRegion())
		return 0;

	memcpy(m_szDate, pcdJuga->pday, sizeof(pcdJuga->pday));
	m_digit = CGrp_Data::CharToInt(pcdJuga->digit, sizeof(pcdJuga->digit));
	m_totalDay = dCnt;
	int mgap = CGrp_Data::CharToInt(pcdJuga->mgap, sizeof(pcdJuga->mgap));
	
	if (!mgap)
		m_dIndex = CDI_DAY;
	else
	{
		if (mgap == 900)
			m_dIndex = CDI_TICK;
		else
		{
			m_dIndex = CDI_MIN;

			if (mgap > 900)
				mgap = mgap - 900;
			else				
				mgap = mgap * 60;			
		}
	}
	
	int	dPos = sz_CDJUGA;
	m_pDataInfo[m_DataCnt] = std::make_unique<CGrp_Data>(this, m_dKind, 0, m_dIndex);
	const int	szFrame = m_pDataInfo[m_DataCnt]->AttachGraphData(&pData[dPos], m_totalDay, mgap);
	dPos += szFrame * m_totalDay;
	m_DataCnt++;

	if (m_dDay == 0 || m_dDay > m_totalDay)
		m_dispDay = m_totalDay;
	else
		m_dispDay = m_dDay;
	m_dispPos = m_totalDay - m_dispDay;
	m_dispEnd = m_dispPos + m_dispDay;

	m_pCrossLine = std::make_unique<CCrossLine>(m_pView, this, m_pRGB, RGB(180,77,77));

	AssignGraph();
	ReviseTick();
	Resize();
	Invalidate();
	m_pApp = (CC_AVolApp *)AfxGetApp();
	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Enable, TRUE), 0);
	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_UpdateScr, 0), 0);

	return 0;
}

void CGrpWnd::RealTimeData(CString sRTM)
{
	if (m_dUnit == CDU_ECN)
	{
		RealTimeECN(sRTM);
		return;
	}

	CString	rtm = sRTM;
	CMapStringToString	rtmStore;
	CString	val, symbol;
	int	index{};
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

	m_siga = m_koga = m_jega = m_jgga = _T("");
	m_gvol = m_ctim = _T("");

	CString	sVal;
	switch (m_digit)
	{
	case 2:
		if (rtmStore.Lookup(R_CURR, sVal))
		{
			double dVal = atof(sVal) * 100.0;
			if (dVal < 0)	dVal *= (-1);
			m_jgga.Format("%.f", dVal);
		}
		
		if (rtmStore.Lookup(R_CTIM, sVal))
		{
			m_ctim = sVal;
		}

		if (m_dIndex == CDI_DAY)
		{
			if (rtmStore.Lookup(R_SIGA, sVal))
			{
				double dVal = atof(sVal) * 100.0;
				if (dVal < 0)	dVal *= (-1);
				m_siga.Format("%.f", dVal);
			}

			if (rtmStore.Lookup(R_KOGA, sVal))
			{
				double dVal = atof(sVal) * 100.0;
				if (dVal < 0)	dVal *= (-1);
				m_koga.Format("%.f", dVal);
			}

			if (rtmStore.Lookup(R_JEGA, sVal))
			{
				double dVal = atof(sVal) * 100.0;
				if (dVal < 0)	dVal *= (-1);
				m_jega.Format("%.f", dVal);
			}

			if (rtmStore.Lookup(R_GVOL, sVal))
			{
				m_gvol = sVal;
			}
		}
		else
		{
			if (rtmStore.Lookup(R_CVOL, sVal))
				m_gvol.Format("%.f", fabs(atof(sVal)));
		}
		break;
	default:
		if (rtmStore.Lookup(R_CURR, sVal))
		{
			double dVal = atof(sVal);
			if (dVal < 0)	dVal *= (-1);
			m_jgga.Format("%.f", dVal);
		}

		if (rtmStore.Lookup(R_CTIM, sVal))
		{
			m_ctim = sVal;
		}

		if (m_dIndex == CDI_DAY)
		{
			if (rtmStore.Lookup(R_SIGA, sVal))
			{
				double dVal = atof(sVal);
				if (dVal < 0)	dVal *= (-1);
				m_siga.Format("%.f", dVal);
			}

			if (rtmStore.Lookup(R_KOGA, sVal))
			{
				double dVal = atof(sVal);
				if (dVal < 0)	dVal *= (-1);
				m_koga.Format("%.f", dVal);
			}

			if (rtmStore.Lookup(R_JEGA, sVal))
			{
				double dVal = atof(sVal);
				if (dVal < 0)	dVal *= (-1);
				m_jega.Format("%.f", dVal);
			}

			if (rtmStore.Lookup(R_GVOL, sVal))
			{
				m_gvol = sVal;
			}
		}
		else
		{
			if (rtmStore.Lookup(R_CVOL, sVal))
				m_gvol.Format("%.f", fabs(atof(sVal)));
		}
		break;
	}

	rtmStore.RemoveAll();

	if (m_jgga.IsEmpty() || m_ctim.IsEmpty())
		return;

	bool	bRedraw = false;
	bool	bShift = false;
	for (int ii = 0; ii < m_DataCnt; ii++)
	{
		const bool bRet = m_pDataInfo[ii]->UpdateRTM();
		if (bRet)	bShift = true;
	}

	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		const bool bRet = m_pGrpInfo[ii]->IsChangeMinMax(bShift);
		if (bRet)	bRedraw = true;
	}

	if (bRedraw || bShift)
	{
		Invalidate();
	}
	else
	{
		const int width = int(double(m_ObjRect.Width())*5 / m_dispDay);
		const CRect	RC = CRect(m_ObjRect.right - width, m_ObjRect.top,
			m_ObjRect.right, m_ObjRect.bottom);

		InvalidateRect(&RC);
	}
}

void CGrpWnd::RealTimeECN(CString sRTM)
{
	CString	rtm = sRTM;
	CMapStringToString	rtmStore;
	CString	val, symbol;
	int	index{};
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

	m_siga = m_koga = m_jega = m_jgga = _T("");
	m_gvol = m_ctim = _T("");

	CString	sVal;
	switch (m_digit)
	{
	case 2:
		if (rtmStore.Lookup(ER_CURR, sVal))
		{
			double dVal = atof(sVal) * 100.0;
			if (dVal < 0)	dVal *= (-1);
			m_jgga.Format("%.f", dVal);
		}
		if (rtmStore.Lookup(ER_CTIM, sVal))
		{
			m_ctim = sVal;
		}

		if (m_dIndex == CDI_DAY)
		{
			if (rtmStore.Lookup(ER_SIGA, sVal))
			{
				double dVal = atof(sVal) * 100.0;
				if (dVal < 0)	dVal *= (-1);
				m_siga.Format("%.f", dVal);
			}
			if (rtmStore.Lookup(ER_KOGA, sVal))
			{
				double dVal = atof(sVal) * 100.0;
				if (dVal < 0)	dVal *= (-1);
				m_koga.Format("%.f", dVal);
			}
			if (rtmStore.Lookup(ER_JEGA, sVal))
			{
				double dVal = atof(sVal) * 100.0;
				if (dVal < 0)	dVal *= (-1);
				m_jega.Format("%.f", dVal);
			}
			if (rtmStore.Lookup(ER_GVOL, sVal))
			{
				m_gvol = sVal;
			}
		}
		else
		{
			if (rtmStore.Lookup(ER_CVOL, sVal))
				m_gvol.Format("%.f", fabs(atof(sVal)));
		}
		break;
	default:
		if (rtmStore.Lookup(ER_CURR, sVal))
		{
			double dVal = atof(sVal);
			if (dVal < 0)	dVal *= (-1);
			m_jgga.Format("%.f", dVal);
		}
		if (rtmStore.Lookup(ER_CTIM, sVal))
		{
			m_ctim = sVal;
		}

		if (m_dIndex == CDI_DAY)
		{
			if (rtmStore.Lookup(ER_SIGA, sVal))
			{
				double dVal = atof(sVal);
				if (dVal < 0)	dVal *= (-1);
				m_siga.Format("%.f", dVal);
			}
			if (rtmStore.Lookup(ER_KOGA, sVal))
			{
				double dVal = atof(sVal);
				if (dVal < 0)	dVal *= (-1);
				m_koga.Format("%.f", dVal);
			}
			if (rtmStore.Lookup(ER_JEGA, sVal))
			{
				double dVal = atof(sVal);
				if (dVal < 0)	dVal *= (-1);
				m_jega.Format("%.f", dVal);
			}
			if (rtmStore.Lookup(ER_GVOL, sVal))
			{
				m_gvol = sVal;
			}
		}
		else
		{
			if (rtmStore.Lookup(ER_CVOL, sVal))
				m_gvol.Format("%.f", fabs(atof(sVal)));
		}
		break;
	}

	rtmStore.RemoveAll();

	if (m_jgga.IsEmpty() || m_ctim.IsEmpty())
		return;

	bool	bRedraw = false;
	bool	bShift = false;
	for (int ii = 0; ii < m_DataCnt; ii++)
	{
		const bool bRet = m_pDataInfo[ii]->UpdateRTM();
		if (bRet)	bShift = true;
	}

	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		const bool bRet = m_pGrpInfo[ii]->IsChangeMinMax(bShift);
		if (bRet)	bRedraw = true;
	}

	if (bRedraw || bShift)
	{
		Invalidate();
	}
	else
	{
		const int width = int(double(m_ObjRect.Width())*5 / m_dispDay);
		const CRect	RC = CRect(m_ObjRect.right - width, m_ObjRect.top,
			m_ObjRect.right, m_ObjRect.bottom);

		InvalidateRect(&RC);
	}
}

bool CGrpWnd::AssignRegion()
{
	m_RgnCnt = 1;	
	for (int ii = 0; ii < m_RgnCnt; ii++)
	{		
		m_pRgnInfo[ii] = std::make_unique<_RgnInfo>();
		m_pRgnInfo[ii]->tick[ctkLEFT].bDraw = (m_TickView & CDT_LEFT)? true:false;
		m_pRgnInfo[ii]->tick[ctkRIGHT].bDraw = (m_TickView & CDT_RIGHT)? true:false;
		m_pRgnInfo[ii]->tick[ctkLEFT].lcolor = RGB(192, 192, 192);
		m_pRgnInfo[ii]->tick[ctkLEFT].lstyle = PS_SOLID;
		m_pRgnInfo[ii]->tick[ctkLEFT].lwidth = 1;
		m_pRgnInfo[ii]->tick[ctkRIGHT].lcolor = RGB(192, 192, 192);
		m_pRgnInfo[ii]->tick[ctkRIGHT].lstyle = PS_SOLID;
		m_pRgnInfo[ii]->tick[ctkRIGHT].lwidth = 1;
		m_pRgnInfo[ii]->tick[ctkBOTTOM].lcolor = RGB(192, 192, 192);
		m_pRgnInfo[ii]->tick[ctkBOTTOM].lstyle = PS_SOLID;
		m_pRgnInfo[ii]->tick[ctkBOTTOM].lwidth = 1;
	}

	return true;
}

bool CGrpWnd::AssignGraph()
{
	const int	rKey = 0;
	struct _gInfo	gInfo;
	
	m_GrpCnt = 0;
	gInfo.dIndex = m_dIndex;
	gInfo.dKey = 0;
	gInfo.dKind = m_dKind;
	gInfo.gKind = m_jChart;
	gInfo.rKey = rKey;
	gInfo.pRGB = RGB(255, 255, 255);
	gInfo.tRGB = RGB(0, 0, 255);

	m_pGrpInfo[m_GrpCnt] = std::make_unique<CGrp_Juga>(m_pView, this, (char *)&gInfo);
	m_GrpCnt = 1;
	return true;
}

void CGrpWnd::DrawGraph(CDC *pDC)
{
	m_cs.Lock();

	DrawEmpty(pDC, true);
	if (m_GrpCnt == 0)
	{
		DrawEmpty(pDC, false);
		m_cs.Unlock();
		return;
	}

	for (int ii = 0; ii < m_MinMaxCnt; ii++)
	{
		m_pMinMaxChk[ii]->Max = INT_MIN;
		m_pMinMaxChk[ii]->Min = INT_MAX;
		m_pMinMaxChk[ii]->tickx = 0;
		m_pMinMaxChk[ii]->ticky = 0;
	}

	double	dMin{}, dMax{};
	struct _MinMaxChk* pMinMaxChk{};
	class CGrp_Base* pBase{};

	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		pBase = m_pGrpInfo[ii].get();
		pMinMaxChk = GetMinMaxChk(pBase->GetRgnKey());
		if (pMinMaxChk && pMinMaxChk->coMinMax)
		{
			if (pBase->CalculateMinMax())
			{
				pBase->GetMinMaxValue(dMin, dMax);

			//	if (pBase->m_pGinfo->gKind == CGK_PMA || 
			//		pBase->m_pGinfo->gKind == CGK_VMA)
				{
					if (dMin != INT_MAX && dMax != INT_MIN)
					{
						if (pMinMaxChk->Max < dMax)
							pMinMaxChk->Max = dMax;
						if (pMinMaxChk->Min > dMin)
							pMinMaxChk->Min = dMin;
					}
				}
			}
		}
	}

	for (int ii = 0; ii < m_GrpCnt; ii++)
		m_pGrpInfo[ii]->DrawGraph(pDC);

	DrawEmpty(pDC, false);
	m_pCrossLine->DrawCrossLine(pDC);
	m_cs.Unlock();
}

void CGrpWnd::DrawEmpty(CDC *pDC, bool bInit)
{
	if (bInit)
	{
		CBrush* pBrush{};
		CBrush* tBrush{};
		CBrush* fBrush{};

		pBrush = m_pApp->GetBrush(m_pView, m_fRGB);
		tBrush = m_pApp->GetBrush(m_pView, m_pRGB);
		fBrush = m_pApp->GetBrush(m_pView, m_tRGB);

		pDC->FillRect(&m_ObjRect, pBrush);
		pDC->FrameRect(&m_ObjRect, fBrush);
		pDC->FillRect(&m_GrpRect, tBrush);
	}
	else
	{
		CBrush*	fBrush;

		fBrush = m_pApp->GetBrush(m_pView, RGB(0,0,0));
		pDC->FrameRect(&m_GrpRect, fBrush);	

		if (m_RgnCnt > 1)
		{
			CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(128,128,128));
			CPen *sPen = pDC->SelectObject(cPen);
			for (int ii = 1; ii < m_RgnCnt; ii++)
			{
				pDC->MoveTo(m_pRgnInfo[ii]->gpRect.left, m_pRgnInfo[ii]->gpRect.top);
				pDC->LineTo(m_pRgnInfo[ii]->gpRect.right, m_pRgnInfo[ii]->gpRect.top);
			}
			pDC->SelectObject(sPen);			
		}
	}
}

void CGrpWnd::Resize()
{
	GetClientRect(&m_ObjRect);

	if (m_RgnCnt == 0 || m_GrpCnt == 0)
		return;

	CDC *pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(m_pFont);
	const CSize	sz = pDC->GetOutputTextExtent("9,999,999");
	
	std::array<double, maxCnt> arrHeight;
	arrHeight.fill(0);

	switch (m_RgnCnt)
	{
	case 1:
		arrHeight[0] = 1.0;
		break;
	case 2:
		arrHeight[0] = 0.7;
		arrHeight[1] = 0.3;
		break;
	case 3:
		arrHeight[0] = 0.4;
		arrHeight[1] = 0.3;
		arrHeight[2] = 0.3;
		break;
	}

	CRect	bottomRC = m_ObjRect;
	bottomRC.top = bottomRC.bottom - sz.cy - 5;
	m_GrpRect = m_ObjRect;
	m_GrpRect.top += 3;
	m_GrpRect.bottom = bottomRC.top;
	if ((m_TickView & CDT_LEFT) && (m_TickView & CDT_RIGHT))
	{
		m_GrpRect.left = m_ObjRect.left + sz.cx;
		m_GrpRect.right = m_ObjRect.right - sz.cx;
	}
	else if (m_TickView & CDT_LEFT)
	{
		m_GrpRect.left = m_ObjRect.left + sz.cx;
		m_GrpRect.right = m_ObjRect.right - 5;
	}
	else if (m_TickView & CDT_RIGHT)
	{
		m_GrpRect.left = m_ObjRect.left + 5;
		m_GrpRect.right = m_ObjRect.right - sz.cx;
	}
	else
	{
		m_GrpRect.left = m_ObjRect.left + 5;
		m_GrpRect.right = m_ObjRect.right - 5;
	}

	for (int ii = 0; ii < m_RgnCnt; ii++)
			m_pRgnInfo[ii]->tkHeight = sz.cy;

	bottomRC.left = m_GrpRect.left;
	bottomRC.right = m_GrpRect.right;

	int	top = m_GrpRect.top;
	
	for (int ii = 0; ii < m_RgnCnt; ii++)
	{
		m_pRgnInfo[ii]->tick[ctkLEFT].tkRect = 
			CRect(0, top, m_GrpRect.left, top + int(m_GrpRect.Height()* arrHeight[ii]));

		m_pRgnInfo[ii]->tick[ctkRIGHT].tkRect = 
			CRect(m_GrpRect.right, top, 
			m_ObjRect.right, top + int(m_GrpRect.Height()* arrHeight[ii]));

		m_pRgnInfo[ii]->tick[ctkBOTTOM].tkRect = bottomRC;
		m_pRgnInfo[ii]->gpRect.top = m_pRgnInfo[ii]->tick[ctkLEFT].tkRect.top;
		m_pRgnInfo[ii]->gpRect.bottom = m_pRgnInfo[ii]->tick[ctkLEFT].tkRect.bottom ;
		m_pRgnInfo[ii]->gpRect.left = m_pRgnInfo[ii]->tick[ctkLEFT].tkRect.right;
		m_pRgnInfo[ii]->gpRect.right = m_pRgnInfo[ii]->tick[ctkRIGHT].tkRect.left;

		top = top + int(m_GrpRect.Height()* arrHeight[ii]);
	}

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	for (int ii = 0; ii < m_GrpCnt; ii++)
		m_pGrpInfo[ii]->Resize();
	m_pCrossLine->Resize(m_GrpRect);
}

/*****/
// c : use control
// r : request count
// d : display count - 0:all
// t : draw tick 
// j : juga chart
// v : volume chart
// o : outline
// i : data index - 1:틱, 2:분, 3:일, 4:주, 5:월
// u : data unit - 1:종목, 2:업종, 3:선물, 4:업종, 5:ECN
// m : ma
// g : vm
/*****/
/*
[C_Mgjv]
컨트롤사용|3|$bool|False|c
그래프모양|3|2-수직업다운바;3-균등수직업다운바;|균등수직업다운바|s
바타입|3|2-2D;3-3D;|2D|p
틱표시|3|0-표시안함;1-왼쪽;2-오른쪽;3-모두표시;|왼쪽|t
거래량표시|3|$bool|False|v
미결제표시|3|$bool|False|x
인덱스구분|3|1-틱;2-분;3-일;4-주;5-월;|일|i
종목구분|3|1-주식;2-업종;3-선물;4-옵션;5-ECN;6-주식옵션;7-KOFEX;|주식|u
주가차트|3|0-사용안함;1-라인차트;2-봉차트;3-바차트;|봉차트|j
주가이평1|1||5|m1
주가이평2|1||20|m2
주가이평3|1||60|m3
주가이평4|1||120|m4
거래량표시|3|$bool|False|v
거래이평1|1||5|g1
거래이평2|1||20|g2
거래이평3|1||60|g3
거래이평4|1||120|g4

*/
void CGrpWnd::ParseParam(char *param)
{
	m_sParam.Format("%s", param);
	CString	sParam = m_sParam;

	CString	sTmp, sToken;
	int		nPos = 0;

	// pole type
	sToken = "/p";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + 2);
		const int index = atoi(sTmp);
		switch (index)
		{
		case PT_2D:	m_poleType = PT_2D;	break;
		case PT_3D:	m_poleType = PT_3D;	break;
		default:	m_poleType = PT_2D;	break;
		}
	}

	// 출력 개수
	sToken = "/d";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + sToken.GetLength());
		m_dDay = atoi(sTmp);
	}

	// 막대개수
	sToken = "/x";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + sToken.GetLength());
		m_nPoll = atoi(sTmp);

		if (m_nPoll <= 0 || m_nPoll > 50)
			m_nPoll = 10;
	}

	// request data count
	sToken = "/r";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + sToken.GetLength());
		m_dCount = atoi(sTmp);
	}

	// 봉차트
	sToken = "/j";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + sToken.GetLength());
		const int type = atoi(sTmp);
		switch (type)
		{
		case CGK_LINE:	m_jChart = CGK_LINE;	break;
		case CGK_BONG:	m_jChart = CGK_BONG;	break;
		case CGK_BAR:	m_jChart = CGK_BAR;	break;
		}
	}

	if (m_jChart > 0)
		m_TickView = CDT_LEFT | CDT_RIGHT;
	else
		m_TickView = CDT_LEFT;

	// data unit
	sToken = "/u";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + 2);
		const int unit = atoi(sTmp);

		switch (unit)
		{
		case CDU_JONGMOK:	m_dUnit = CDU_JONGMOK;	break;
		case CDU_UPJONG:	m_dUnit = CDU_UPJONG;	break;
		case CDU_FUTURE:	m_dUnit = CDU_FUTURE;	break;
		case CDU_OPTION:	m_dUnit = CDU_OPTION;	break;
		case CDU_ECN:		m_dUnit = CDU_ECN;	break;
		default:		m_dUnit = 0;		break;
		}
	}
}

int CGrpWnd::RequestHead()
{
	m_sHeader.Format("%02d%02d%02d%06d0000000000000000", m_dUnit, m_dIndex, m_dKind, m_dCount);
	return long(m_sHeader.operator LPCTSTR());
}

void CGrpWnd::RemoveComponent()
{
	for_each(m_pRgnInfo.begin(), m_pRgnInfo.end(), [](auto& item) {
		item.reset();
	});

	for_each(m_pDataInfo.begin(), m_pDataInfo.end(), [](auto& item) {
		item.reset();
	});

	for_each(m_pGrpInfo.begin(), m_pGrpInfo.end(), [](auto& item){
		item.reset();
	});


	m_RgnCnt = 0;
	m_GrpCnt = 0;
	m_DataCnt = 0;


	for_each(m_pMinMaxChk.begin(), m_pMinMaxChk.end(), [](auto& item) {
		item.reset();
	});
	m_MinMaxCnt = 0;
	
	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Enable, FALSE), 0);

}

int CGrpWnd::GetPalette(int index)
{
	return m_pView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

BOOL CGrpWnd::IsPtInGrp()
{
	POINT	pt;
	CRect	rect;

	::GetCursorPos(&pt);
	GetWindowRect(rect);

	const CWnd* pWnd = CWnd::WindowFromPoint(pt);
	if (pWnd != this)
		return FALSE;

	return (rect.PtInRect(pt));
}

bool CGrpWnd::ChangeDisplayScale(int scale)
{
	scale *=10;
	if (scale > 0)	// dispDay 증가, scale 축소
	{
		if (m_dispDay >= m_totalDay)
			return false;

		if (m_dispPos == 0)
		{
			scale = (m_totalDay - m_dispDay >= scale)?
						scale:m_totalDay - m_dispDay;
			m_dispDay += scale;
			m_dispEnd += scale;

			return true;
		}
		else if (m_dispPos - abs(scale) < 0)
		{
			scale = m_dispPos;
		}
	}
	else if (scale < 0)	// dispDay 감소, scale 확대
	{
		if (m_dispDay <= 20)
			return false;
		if ((m_dispDay - abs(scale)) <= 20)
			scale = 20 - m_dispDay;

		if (scale == 0)
			return false;
	}
	else
		return false;

	m_dispDay += scale;
	m_dispPos -= scale;

	return true;
}

bool CGrpWnd::ChangeDisplay(int dispDay)
{
	m_dispDay = dispDay;
	m_dispPos = m_dispEnd - dispDay;

	if (m_dispPos < 0)
	{
		const int gap = 0 - m_dispPos;
		m_dispPos += gap;
		m_dispEnd += gap;
	}

	return true;
}

bool CGrpWnd::ChangeDisplayShift(int shift)
{
	if (shift > 0)	// right scroll
	{
		if (m_dispEnd >= m_totalDay)
			return false;
		if ((m_dispEnd + shift) > m_totalDay)
			shift = m_totalDay - m_dispEnd;
	}
	else if (shift < 0)	// left scroll
	{
		if (m_dispPos <= 0)
			return false;
		if ((m_dispPos - abs(shift)) <  0)
			shift = m_dispPos * (-1);
	}

	if (shift == 0)
		return false;

	m_dispEnd += shift;
	m_dispPos += shift;

	RecalculateMinMax();
	Invalidate();

	return true;
}

void CGrpWnd::ExcelProc()
{
	if (m_GrpCnt <= 0 || !m_pGrpInfo[0] || m_totalDay <= 0)
		return;

	CString	tmpS;
	CString	ExcelS = m_pGrpInfo[0]->GetExcelHead(-1);
	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		tmpS = m_pGrpInfo[ii]->GetExcelData(-1);
		if (!tmpS.IsEmpty())
			ExcelS += "\t" + tmpS;
	}
	ExcelS += "\n";

	for (int ii = 0; ii < m_totalDay; ii++)
	{
		tmpS = m_pGrpInfo[0]->GetExcelHead(ii);
		ExcelS += tmpS;
		for (int jj = 0; jj < m_GrpCnt; jj++)
		{
			tmpS = m_pGrpInfo[jj]->GetExcelData(ii);
			if (!tmpS.IsEmpty())
				ExcelS += "\t" + tmpS;
		}
		ExcelS += "\n";
	}

	if (ExcelS.IsEmpty())
		return;

	m_pView->SendMessage(WM_USER, MAKEWPARAM(excelDLL, 1), (long)ExcelS.operator LPCTSTR());
}

void CGrpWnd::RecalculateMinMax()
{
	for (int ii = 0; ii < m_GrpCnt; ii++)
		m_pGrpInfo[ii]->CalculateMinMax();
}

void CGrpWnd::ReviseTick()
{
	//if (m_pMinMaxChk)
	//{
	//	delete[] m_pMinMaxChk;
	//	m_pMinMaxChk = nullptr;
	//}

	
	for (auto& item : m_pMinMaxChk)
		item.reset();
	m_MinMaxCnt = m_RgnCnt;

	for (int ii = 0; ii < m_MinMaxCnt; ii++) 
		m_pMinMaxChk.at(ii) = std::make_unique<_MinMaxChk>();

	for (int ii = 0; ii < m_RgnCnt; ii++)
	{
		int	grpcnt = 0;
		for (int jj = 0; jj < m_GrpCnt; jj++)
		{			
			if (m_pGrpInfo[jj]->GetRgnKey() == ii)
				grpcnt++;
		}

		m_pMinMaxChk[ii]->region = ii;
		if (grpcnt > 1)	m_pMinMaxChk[ii]->coMinMax = 1;		
	}
}

_MinMaxChk* CGrpWnd::GetMinMaxChk(int rKey)
{
	if (rKey < 0 || rKey >= m_RgnCnt)
		return nullptr;
	
	return m_pMinMaxChk[rKey].get();
}

void CGrpWnd::SetProperty(CString str)
{
}

void CGrpWnd::Clear()
{
	RemoveComponent();
	Invalidate();
}

void CGrpWnd::TipMouseMove(CPoint point)
{
	ScreenToClient(&point);
	MouseMove(true, point);
}

void CGrpWnd::MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly)
{
	if (!m_pToolTip || !m_pCrossLine)
		return;

	if (!m_GrpRect.PtInRect(point))
	{
		m_pToolTip->ShowWindow(SW_HIDE);
		m_pCrossLine->HideCrossLine(point);
		return;
	}

	if (m_GrpCnt < 1)
	{
		m_pCrossLine->HideCrossLine(point);
		m_pToolTip->ShowWindow(SW_HIDE);
		return;
	}

	m_pCrossLine->ShowCrossLine(point);

	if (bCrossOnly)
	{
		if (m_timerID > 0)
			KillTimer(m_timerID);

		m_timerID = TIMER_ID;
		SetTimer(m_timerID, TIMER_GAP, nullptr);
		return;
	}

	if (m_PosData.IsEmpty())
	{
		m_pToolTip->ShowWindow(SW_HIDE);
		return;
	}

	if (!bIsTipEvent)
	{
		if (m_pToolTip->IsWindowVisible())
			return;
	}

	CPoint	pt = point;
	ClientToScreen(&pt);
	
	pt.x += 10;
	pt.y += 10;

	CRect	rect;
	CSize	size;	

	rect = m_pToolTip->SetData(m_PosData);
	
	size.cx = rect.Width();
	size.cy = rect.Height();

	rect.left = pt.x;
	rect.right = pt.x + size.cx;
	rect.top = pt.y;
	rect.bottom = rect.top + size.cy;

	CRect rcWnd;
	GetWindowRect(rcWnd);	

	m_pToolTip->SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);

	if (m_pToolTip->IsWindowVisible())
		m_pToolTip->Invalidate(FALSE);
	else
		m_pToolTip->ShowWindow(SW_SHOWNOACTIVATE);

	if (m_timerID > 0)
		KillTimer(m_timerID);

	m_timerID = TIMER_ID;
	SetTimer(m_timerID, TIMER_GAP, nullptr);
}

CString CGrpWnd::GetTipStr(CPoint point)
{
	if (m_GrpCnt < 1)
		return _T("");

	int	rgnIdx = -1;
	int ii = 0;
	for (ii = 0; ii < m_RgnCnt; ii++)
	{
		if (m_pRgnInfo[ii]->gpRect.PtInRect(point))
		{
			rgnIdx = ii;
			break;
		}
	}

	CString stmp;
	CString dispStr = m_pGrpInfo[0]->GetDisplayPosHead(point);
	for (; ii < m_GrpCnt; ii++)
	{
		if (m_pGrpInfo[ii]->GetRgnKey() == rgnIdx)
		{
			stmp = m_pGrpInfo[ii]->GetDisplayPosData(point);
			if (!stmp.IsEmpty())
				dispStr = dispStr + "\n" + m_pGrpInfo[ii]->GetDisplayPosData(point);
		}
	}

	return dispStr;
}

void CGrpWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Focus, TRUE), 0);	
	CWnd::OnLButtonDown(nFlags, point);
}