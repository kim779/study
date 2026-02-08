// GrpWnd.cpp : implementation file
//

#include "stdafx.h"
#include "C_TickEx.h"
#include "GrpWnd.h"
#include "memDC.h"
#include "ToolTip.h"
#include "CrossLine.h"
#include "Grp_Data.h"
#include "Grp_Tick.h"
#include "ConfigDlg.h"
#include "CtrlWnd.h"
#include "MainWnd.h"

#include "../../h/axiscgrp.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrpWnd
#define	TIMER_PID	10000
#define	TIMER_PGAP	250
#define	TIMER_ID	10001
#define	TIMER_GAP	1000

CGrpWnd::CGrpWnd(CWnd* pView, CWnd* pParent, struct _param* pInfo)
{
	m_pApp = (CC_TickExApp *)AfxGetApp();
	m_pApp->Init(pView);

	m_pView = pView;
	m_pParent = pParent;
	m_pToolTip = std::make_unique<CToolTip>(m_pView, this);
	m_pCrossLine = nullptr;
	
//	m_pGrpRgn[0] = m_pGrpRgn[1] = nullptr;
//	m_pGrpData = nullptr;
//	m_pGrpTick[0] = m_pGrpTick[1] = nullptr;

	m_bExpand = m_bMygl = false;
	m_dKind = CDK_TICK;
	m_dIndex = CDI_TICK;
	m_dCount = 100;
	m_dDay = 100;
	m_dUnit = CDU_JONGMOK;
	m_sJgga = 0;
	m_sState = sp_NO;

	m_digit = 0;
	m_fPoint = pInfo->point;
	m_fName.Format("%s", pInfo->fonts);
	m_tRGB = GetPalette(pInfo->tRGB);
	m_pRGB = GetPalette(pInfo->pRGB);
	m_fRGB = GetPalette(BGIndex);

	m_TickView = CDT_LEFT;
	m_ObjRect = pInfo->rect;
	ParseParam((char*)pInfo->options.operator LPCTSTR());

	m_ParamKey = pInfo->key;
	m_sParamName.Format("%s", pInfo->name);

	ZeroMemory(&m_tickinfo, sz_TICKINFO);
	CheckFile();

	m_pFont = nullptr;
	m_ltColor = m_tRGB;
	m_rtColor = m_tRGB;
	m_btColor = m_tRGB;

	m_PosData = _T("");
	m_timerID = 0;
}

CGrpWnd::~CGrpWnd()
{
	if (m_dispDay > 0)
	{
		sprintf(m_tickinfo.display, "%d", m_dispDay);
		CString	filename = m_pApp->m_Path + m_FileName;
		m_pApp->SaveCondition(filename, &m_tickinfo);
	}
}


BEGIN_MESSAGE_MAP(CGrpWnd, CWnd)
	//{{AFX_MSG_MAP(CGrpWnd)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(CM_GRP, OnGrp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGrpWnd message handlers
LONG CGrpWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	const LONG	ret = 0;
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:	// set data from server
		m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Clear, 0), 0);
		return DispatchData(wParam, lParam);
	case DLL_INB:	// get data from out
		return RequestHead();
	case DLL_ALERTx:
		RealTimeData(lParam);
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
	case DLL_SETFONTx:
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
			{
				CString	tmpS = (char *)lParam;
				if (tmpS.IsEmpty())	return FALSE;

				CString tmpS_;
				int pos= 0;
				for (int ii = 0; !tmpS.IsEmpty(); ii++)
				{
					pos = tmpS.Find('\t');
					if (pos != -1)
					{
						tmpS_ = tmpS.Left(pos);
						switch (ii)
						{
						case 0:
							m_dispPos = atoi(tmpS_);
							break;
						case 1:
							m_dispEnd = atoi(tmpS_);
							break;
						}
					}
					tmpS = tmpS.Mid(pos+1);
				}

				RecalculateMinMax();
				Invalidate();
			}
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
		case outConfig:
			return userConfig();
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

void CGrpWnd::OnDestroy() 
{
	if (m_timerID > 0)
		KillTimer(m_timerID);

	if (m_pToolTip)
		m_pToolTip->DestroyWindow();

	RemoveComponent();

	CWnd::OnDestroy();
}

void CGrpWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC	mdc(&dc);
	DrawGraph(&mdc);
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
			if (m_pCrossLine)
				m_pCrossLine->HideCrossLine();
			m_timerID = 0;
		}
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CGrpWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_pParent->SendMessage(WM_RBUTTONDOWN, WPARAM(nFlags), MAKELPARAM(point.x, point.y));
	
	CWnd::OnRButtonDown(nFlags, point);
}

void CGrpWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx == 0 || cy == 0)
		return;

	Resize();
}

int CGrpWnd::DispatchData(WPARAM wParam, LPARAM lParam)
{
	struct _extTHx* info = (struct _extTHx*)lParam;
	const int dlen = info->size;
	char*   pData = info->data;

	RemoveComponent();
	if (dlen < sz_CDJUGA)
	{
		Invalidate();
		return dlen;
	}

	const struct _cdJuga	*pcdJuga = (struct _cdJuga *)pData;
	const int dCnt = CGrp_Data::CharToInt(CString(pcdJuga->nData, sizeof(pcdJuga->nData)));

	m_digit = CGrp_Data::CharToInt(CString(pcdJuga->digit, sizeof(pcdJuga->digit)));
	m_pJgga = CGrp_Data::CharToInt(CString(pcdJuga->pjgga, sizeof(pcdJuga->pjgga)));
	m_uJgga = CGrp_Data::CharToInt(CString(pcdJuga->ujgga, sizeof(pcdJuga->ujgga)));
	m_dJgga = CGrp_Data::CharToInt(CString(pcdJuga->djgga, sizeof(pcdJuga->djgga)));
	m_totalDay = dCnt;
	int mgap = CGrp_Data::CharToInt(CString(pcdJuga->mgap, sizeof(pcdJuga->mgap)));
	if (mgap >= 900)
		mgap = mgap - 900;
	else
		mgap = mgap * 60;

	int	dPos = sz_CDJUGA;

	if (dlen != int(sz_cdTick2*m_totalDay + sz_CDJUGA))
		return dlen;

	m_pGrpData = std::make_unique<CGrp_Data>(this, m_dKind, 0, m_dIndex);
	const int	szFrame = m_pGrpData->AttachGraphData(&pData[dPos], m_totalDay, mgap, mgap);
	dPos += szFrame * m_totalDay;

	if (m_dDay == 0 || m_dDay > m_totalDay)
		m_dispDay = m_totalDay;
	else
		m_dispDay = m_dDay;
	m_dispPos = m_totalDay - m_dispDay;
	m_dispEnd = m_dispPos + m_dispDay;

	m_pCrossLine = std::make_unique<CCrossLine>(m_pView, this, m_pRGB, RGB(180,77,77));
	
	AssignRegion();
	AssignGraph();
	Resize();
	Invalidate();

	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Enable, TRUE), 0);
	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_UpdateScr, 0), 0);

	return dPos;
}

void CGrpWnd::RealTimeData(LPARAM lParam)
{
	if (!m_pGrpData)
		return;

	struct _alertR* alertR;
	alertR = (struct _alertR*)lParam;
	DWORD* data = nullptr;
	bool	bShift = false;

	
	CString code = alertR->code;
	if (!(alertR->stat & alert_SCR))
		return;

	// ... 1번째 일경우...
	//data = (DWORD*)alertR->ptr[0];
	for (int ii = alertR->size - 1; ii >= 0; ii--)
	{
		data = (DWORD*)alertR->ptr[ii];
		if (Alert(code, data))
		{
			if (m_pGrpData && m_bExpand)
			{
				m_totalDay = m_pGrpData->GetDataCount();
				//m_dispDay = m_totalDay;
				m_dispPos += 1;
				m_dispEnd += 1;
			}
			bShift = true;
		}
	}

	const bool bRedraw = m_pGrpTick[0]->IsChangeMinMax();
	const bool bRedraw2 = m_pGrpTick[1]->IsChangeMinMax();

	if (bRedraw || bRedraw2 || bShift)
	{
		Resize();
		Invalidate();
	}
	else
	{
		const int width = int(double(m_ObjRect.Width()) * 5 / m_dispDay);
		const CRect	RC = CRect(m_ObjRect.right - width, m_ObjRect.top,
			m_ObjRect.right, m_ObjRect.bottom);

		InvalidateRect(&RC);
	}
}

bool CGrpWnd::Alert(CString code, DWORD* data)
{
	const double val = pow(10, m_digit);

	switch (m_digit)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		if (data[23]) //R_CURR
		{
			double dVal = atof(CString((char*)data[23])) * val;
			if (dVal < 0)	dVal *= (-1);
			m_jgga.Format("%.f", dVal);
		}

		if (data[32])  //R_CVOL
			m_cvol = CString((char*)data[32]);

		if (data[34])  //R_CTIM
			m_ctim = (char*)data[34];
		break;
	case 0:
	default:
		if (data[23]) //R_CURR
		{
			double dVal = atof(CString((char*)data[23])) * 10.0;
			if (dVal < 0)	dVal *= (-1);
			m_jgga.Format("%.f", dVal);
		}

		if (data[32])  //R_CVOL
			m_cvol = CString((char*)data[32]);

		if (data[34])  //R_CTIM
			m_ctim = (char*)data[34];
		break;
	}
	
	if (m_jgga.IsEmpty() || m_cvol.IsEmpty() || m_ctim.IsEmpty())
		return false;

	if (!m_cvol.IsEmpty() && (code.GetLength() == 8))
		m_cvol.Delete(0);
	else
	{
		m_cvol.Remove('+');
		m_cvol.Remove('-');
		m_cvol.Format("%d", atoi(m_cvol));
	}

	for_each(_mapSig.begin(), _mapSig.end(), [&](const auto& item) {		
		const int nSig = item.second->m_nSig;
		if (nSig > 0)
		{			
			const struct _cgTickEx *gTickEx = m_pGrpData->GetLastData();
			const int    curJgga = gTickEx->epr;
			CString strProc;
			
			if (nSig < curJgga)
			{
				if (item.second->m_nState == sp_DN)	// down->up
				{
					strProc.Format("OnTickUpSignal\t%s", item.first);		
					m_pView->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)strProc);
				}
				item.second->m_nState = sp_UP;
			}
			else if (nSig > curJgga)
			{
				if (item.second->m_nState == sp_UP)	// up->down
				{
					strProc.Format("OnTickDownSignal\t%s", item.first);
					m_pView->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)strProc);
				}
				item.second->m_nState = sp_DN;
			}
		}
	});		
	return m_pGrpData->UpdateRTM();
}

bool CGrpWnd::AssignRegion()
{
	m_pGrpRgn[0] = std::make_unique<_RgnInfo>();

	m_pGrpRgn[0]->tick[ctkLEFT].bDraw = (m_TickView & CDT_LEFT)? true:false;
	m_pGrpRgn[0]->tick[ctkRIGHT].bDraw = (m_TickView & CDT_RIGHT)? true:false;
	m_pGrpRgn[0]->tick[ctkLEFT].lcolor = RGB(192, 192, 192);
	m_pGrpRgn[0]->tick[ctkLEFT].lstyle = PS_SOLID;
	m_pGrpRgn[0]->tick[ctkLEFT].lwidth = 1;
	m_pGrpRgn[0]->tick[ctkRIGHT].lcolor = RGB(192, 192, 192);
	m_pGrpRgn[0]->tick[ctkRIGHT].lstyle = PS_SOLID;
	m_pGrpRgn[0]->tick[ctkRIGHT].lwidth = 1;
	m_pGrpRgn[0]->tick[ctkBOTTOM].lcolor = RGB(192, 192, 192);
	m_pGrpRgn[0]->tick[ctkBOTTOM].lstyle = PS_SOLID;
	m_pGrpRgn[0]->tick[ctkBOTTOM].lwidth = 1;

	m_pGrpRgn[1] = std::make_unique<_RgnInfo>();
	CopyMemory(m_pGrpRgn[1].get(), m_pGrpRgn[0].get(), sizeof(_RgnInfo));

	return true;
}

bool CGrpWnd::AssignGraph()
{
	struct _gInfo	gInfo;
	gInfo.dIndex = m_dIndex;
	gInfo.dKind = m_dKind;
	gInfo.gKind = CGK_LINE;
	gInfo.pRGB = RGB(255, 255, 255);
	gInfo.tRGB = RGB(0, 0, 255);
	m_pGrpTick[0] = std::make_unique<CGrp_Tick>(m_pView, this, 0, (char *)&gInfo);

	gInfo.gKind = CGK_VOL;
	gInfo.pRGB = RGB(255, 255, 255);
	gInfo.tRGB = RGB(0, 128, 0);

	m_pGrpTick[1] = std::make_unique<CGrp_Tick>(m_pView, this, 1, (char *)&gInfo);

	return true;
}

void CGrpWnd::DrawGraph(CDC *pDC)
{
	m_cs.Lock();

	DrawEmpty(pDC, true);
	if (!m_pGrpTick[0])
	{
		DrawEmpty(pDC, false);
		m_cs.Unlock();
		return;
	}

	m_pGrpTick[0]->DrawGraph(pDC);
	if (m_pGrpTick[1])	m_pGrpTick[1]->DrawGraph(pDC);

	DrawEmpty(pDC, false);
	m_pCrossLine->DrawCrossLine(pDC);

	m_cs.Unlock();
}

void CGrpWnd::DrawEmpty(CDC *pDC, bool bInit)
{
	if (bInit)
	{
		CBrush	*pBrush = m_pApp->GetBrush(m_pView, m_fRGB);
		CBrush	*tBrush = m_pApp->GetBrush(m_pView, m_pRGB);
		CBrush	*fBrush = m_pApp->GetBrush(m_pView, m_tRGB);

		pDC->FillRect(&m_ObjRect, pBrush);
		pDC->FrameRect(&m_ObjRect, fBrush);
		pDC->FillRect(&m_GrpRect, tBrush);
	}
	else
	{
		DrawFrameRect(pDC, m_GrpRect, RGB(0,0,0));

		if (m_pGrpRgn[1])
		{
			CPen	*cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(128,128,128));
			CPen	*sPen = pDC->SelectObject(cPen);
		
			pDC->MoveTo(m_pGrpRgn[1]->gpRect.left, m_pGrpRgn[1]->gpRect.top);
			pDC->LineTo(m_pGrpRgn[1]->gpRect.right, m_pGrpRgn[1]->gpRect.top);
			pDC->SelectObject(sPen);
		}
	}
}

void CGrpWnd::DrawFrameRect(CDC *pDC, CRect rc, COLORREF rgb)
{
	CPen	*pPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, rgb);
	CPen	*sPen = pDC->SelectObject(pPen);

	pDC->MoveTo(rc.left, rc.top);
	pDC->LineTo(rc.right, rc.top);
	pDC->LineTo(rc.right, rc.bottom);
	pDC->LineTo(rc.left, rc.bottom);
	pDC->LineTo(rc.left, rc.top);

	pDC->SelectObject(sPen);
}

void CGrpWnd::Resize()
{
	GetClientRect(&m_ObjRect);

	if (!m_pGrpRgn[0])
		return;

	CDC *pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(m_pFont);
	CSize	sz = pDC->GetOutputTextExtent("9,999,999");
	if (m_pGrpTick[0])
	{
		CString	sMax, sMin;
		m_pGrpTick[0]->GetMaxStr(sMax);
		m_pGrpTick[0]->GetMaxStr(sMin);
		CSize	szMax = pDC->GetOutputTextExtent(sMax);
		CSize	szMin = pDC->GetOutputTextExtent(sMin);

		if (m_pGrpTick[1])
		{
			m_pGrpTick[1]->GetMaxStr(sMax);
			m_pGrpTick[1]->GetMaxStr(sMin);
			const CSize szMax2 = pDC->GetOutputTextExtent(sMax);
			const CSize szMin2 = pDC->GetOutputTextExtent(sMin);

			if (szMax.cx < szMax2.cx)	szMax = szMax2;
			if (szMin.cx < szMin2.cx)	szMin = szMin2;
		}

		if (szMax.cx > szMin.cx)
		{
			if (sz.cx > szMax.cx+5)	sz.cx = szMax.cx+5;
		}
		else
		{
			if (sz.cx > szMin.cx+5)	sz.cx = szMin.cx+5;
		}		
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
	
	bottomRC.left = m_GrpRect.left;
	bottomRC.right = m_GrpRect.right;

	int	topHeight = m_GrpRect.Height();
	if (m_pGrpRgn[1])
		topHeight = int(m_GrpRect.Height()*0.7);

	m_pGrpRgn[0]->tkHeight = sz.cy;
	m_pGrpRgn[0]->tick[ctkLEFT].tkRect = CRect(0, m_GrpRect.top, m_GrpRect.left, m_GrpRect.top + topHeight);
	m_pGrpRgn[0]->tick[ctkRIGHT].tkRect = CRect(m_GrpRect.right, m_GrpRect.top, m_ObjRect.right, m_GrpRect.top + topHeight);
	m_pGrpRgn[0]->tick[ctkBOTTOM].tkRect = bottomRC;
	m_pGrpRgn[0]->gpRect.top = m_pGrpRgn[0]->tick[ctkLEFT].tkRect.top;
	m_pGrpRgn[0]->gpRect.bottom = m_pGrpRgn[0]->tick[ctkLEFT].tkRect.bottom;
	m_pGrpRgn[0]->gpRect.left = m_pGrpRgn[0]->tick[ctkLEFT].tkRect.right;
	m_pGrpRgn[0]->gpRect.right = m_pGrpRgn[0]->tick[ctkRIGHT].tkRect.left;
	m_pGrpTick[0]->Resize();

	if (m_pGrpRgn[1])
	{
		m_pGrpRgn[1]->tkHeight = sz.cy;
		m_pGrpRgn[1]->tick[ctkLEFT].tkRect = CRect(0, m_GrpRect.top + topHeight, m_GrpRect.left, m_GrpRect.bottom);	
		m_pGrpRgn[1]->tick[ctkRIGHT].tkRect = CRect(m_GrpRect.right, m_GrpRect.top + topHeight, m_ObjRect.right, m_GrpRect.bottom);
		m_pGrpRgn[1]->tick[ctkBOTTOM].tkRect = bottomRC;
		m_pGrpRgn[1]->gpRect.top = m_pGrpRgn[1]->tick[ctkLEFT].tkRect.top;
		m_pGrpRgn[1]->gpRect.bottom = m_pGrpRgn[1]->tick[ctkLEFT].tkRect.bottom;
		m_pGrpRgn[1]->gpRect.left = m_pGrpRgn[1]->tick[ctkLEFT].tkRect.right;
		m_pGrpRgn[1]->gpRect.right = m_pGrpRgn[1]->tick[ctkRIGHT].tkRect.left;
		m_pGrpTick[1]->Resize();
	}

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	m_pCrossLine->Resize(m_GrpRect);
}

/*****/
// n : filename
// d : display count - 0:all
// r : request count
// t : draw tick 
// e : data expand
// u : data unit - 1:종목, 3:선물, 4:옵션
// i : data index - 1:틱, 2:분
/*****/
void CGrpWnd::ParseParam(char *param)
{
	m_sParam.Format("%s", param);
	CString	sParam = m_sParam;

	CString	sTmp, sToken;
	int		nPos = 0;

	// 저장 파일명
	m_FileName = _T("");
	sToken = "/n";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		const int index = sParam.Find("/", nPos+1);
		sTmp = sParam.Mid(nPos+2, index - nPos - 2);
		m_FileName.Format("%s", sTmp);
	}

	if (m_FileName.IsEmpty())
		m_FileName.Format("tickex.gex");
	else
		m_FileName += _T(".gex");


	// 출력 개수
	sToken = "/d";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + sToken.GetLength());
		m_dDay = atoi(sTmp);
	}

	// request data count
	sToken = "/r";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + sToken.GetLength());
		m_dCount = atoi(sTmp);
	}

	// tick
	sToken = "/t";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + sToken.GetLength());
		m_TickView = atoi(sTmp);
	}

	// data expand
	sToken = "/e";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)	m_bExpand = true;
	else 		m_bExpand = false;

	// if use control, expand is false
	sToken = "/c";	nPos = sParam.Find(sToken, 0);
	//if (nPos >= 0)	m_bExpand = false;

	// data unit
	sToken = "/u";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + 2);
		const int unit = atoi(sTmp);

		switch (unit)
		{
		case CDU_JONGMOK:	m_dUnit = CDU_JONGMOK;	break;
		default:		m_dUnit = 0;		break;
		}
	}

	// data index
	sToken = "/i";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + 2);
		const int index = atoi(sTmp);
		switch (index)
		{
		case CDI_MIN:	m_dIndex = CDI_MIN;	break;
		default:	m_dIndex = CDI_TICK;	break;
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
	for (auto& item: m_pGrpRgn)
		item.reset();
	m_pGrpData.reset();

	for (auto& item : m_pGrpTick)
		item.reset();

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

	if (m_totalDay < 1)
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
	if (!m_pGrpTick[0])
		return _T("");

	CString	dispStr = m_pGrpTick[0]->GetDisplayPosData(point);

	return dispStr;
}

void CGrpWnd::RecalculateMinMax()
{
	m_pGrpTick[0]->CalculateMinMax();
	m_pGrpTick[1]->CalculateMinMax();
}

bool CGrpWnd::ChangeDisplayScale(int scale)
{
	int	gap = m_totalDay/20;
	if (gap < 5)		gap = 5;
	else if (gap > 10)	gap = 10;

	scale *= gap;
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
	if (!m_pGrpTick[0].get() || m_totalDay <= 0)
		return;

	CString	tmpS;
	CString	ExcelS = m_pGrpTick[0]->GetExcelHead(-1);

	tmpS = m_pGrpTick[0]->GetExcelData(-1);
	if (!tmpS.IsEmpty())
		ExcelS += "\t" + tmpS;

	ExcelS += "\n";

	for (int ii = 0; ii < m_totalDay; ii++)
	{
		tmpS = m_pGrpTick[0]->GetExcelHead(ii);
		ExcelS += tmpS;

		tmpS = m_pGrpTick[0]->GetExcelData(ii);
		if (!tmpS.IsEmpty())
			ExcelS += "\t" + tmpS;

		ExcelS += "\n";
	}

	if (ExcelS.IsEmpty())
		return;

	m_pView->SendMessage(WM_USER, MAKEWPARAM(excelDLL, 1), (long)ExcelS.operator LPCTSTR());
}

void CGrpWnd::CheckFile()
{
	CString	filename = m_pApp->m_Path + m_FileName;
	if (m_pApp->ReadCondition(filename, &m_tickinfo))
	{
		m_dCount = atoi(m_tickinfo.request);
		m_dDay = atoi(m_tickinfo.display);
	}
	else
	{
		sprintf(m_tickinfo.request, "%d", m_dCount);
		sprintf(m_tickinfo.display, "%d", m_dDay);
		m_pApp->SaveCondition(filename, &m_tickinfo);
	}
}

BOOL CGrpWnd::userConfig()
{
	CConfigDlg	dlg(&m_tickinfo);

	if (dlg.DoModal() == IDOK)
	{
		m_dCount = atoi(m_tickinfo.request);

		CString	filename = m_pApp->m_Path + m_FileName;
		m_pApp->SaveCondition(filename, &m_tickinfo);

		return TRUE;
	}

	return FALSE;
}
void CGrpWnd::StartOrderSignal(CString sPrice, CString strKey)
{
	CString	sTmp;
	sPrice.Remove(',');

	sTmp.Format("%.f", atof(sPrice) * (m_digit == 0 ? 1 : m_digit));

	const int	sjgga = atoi(sTmp);

//	CSignal* pSig = new CSignal;
	const auto item = _mapSig.emplace(std::make_pair(strKey, std::make_unique<CSignal>()));
	auto& pSig = item.first->second;

	if (nullptr == m_pGrpData)
	{
		pSig->m_nState = sp_NO;
		pSig->m_nSig = sjgga;
		
//		m_mapSig.SetAt(strKey, pSig);
		return;
	}

	const struct _cgTickEx *gTickEx = m_pGrpData->GetLastData();
	if (nullptr == gTickEx)
	{
		pSig->m_nState = sp_NO;
		pSig->m_nSig = sjgga;
		
//		m_mapSig.SetAt(strKey, pSig);
		return;
	}

	if (gTickEx->epr > sjgga)
		pSig->m_nState = sp_UP;
	else if (gTickEx->epr < sjgga)
		pSig->m_nState = sp_DN;
	else
		pSig->m_nState = sp_EQ;

	pSig->m_nSig = sjgga;

//	m_mapSig.SetAt(strKey, pSig);
}

void CGrpWnd::EndOrderSignal(CString strKey)
{
	_mapSig.erase(strKey);
//	m_mapSig.RemoveKey(strKey);
/*
	m_sJgga = 0;
	m_sState = sp_NO;
*/
}

void CGrpWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Focus, TRUE), 0);	
	CWnd::OnLButtonDown(nFlags, point);
}

