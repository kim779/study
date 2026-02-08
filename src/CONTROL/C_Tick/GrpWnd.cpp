// GrpWnd.cpp : implementation file
//

#include "stdafx.h"
#include "C_Tick.h"
#include "GrpWnd.h"
#include "memDC.h"
#include "ToolTip.h"
#include "CrossLine.h"
#include "Grp_Data.h"
#include "Grp_Tick.h"

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

#define	ER_CURR	"023"	// 현재가
#define	ER_CTIM	"734"	// 체결시간
#define	ER_CVOL	"732"	// 체결거래량(체결수량)
#define	ER_DIFF	"724"	// 전일대비
#define	ER_UDYL	"733"	// 등락율

CGrpWnd::CGrpWnd(CWnd* pView, struct _param* pInfo)
{
	m_pApp = (CC_TickApp *)AfxGetApp();
	m_pApp->SetView(pView);
	m_pView = pView;
	m_pToolTip = std::make_unique<CToolTip>(m_pView, this);
	m_pCrossLine = nullptr;
	m_pGrpTick = nullptr;

	m_bExpand = m_bOutLine = false;
	m_dKind = CDK_TICK;
	m_dIndex = CDI_TICK;
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
	ParseParam((char*)pInfo->options.operator LPCTSTR());	

	m_pFont = nullptr;
	m_ltColor = m_tRGB;
	m_rtColor = m_tRGB;
	m_btColor = m_tRGB;

	m_title   = _T("");
	m_PosData = _T("");
	m_timerID = 0;

	EnableAutomation();
}

CGrpWnd::~CGrpWnd()
{
}

void CGrpWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CGrpWnd, CWnd)
	//{{AFX_MSG_MAP(CGrpWnd)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CGrpWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CGrpWnd)
	DISP_FUNCTION(CGrpWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CGrpWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CGrpWnd, "SetTitle", _SetTitle, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IGrpWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {C34A1D1B-1E59-4117-88CA-123F4556BDBE}
static const IID IID_IGrpWnd =
{ 0xc34a1d1b, 0x1e59, 0x4117, { 0x88, 0xca, 0x12, 0x3f, 0x45, 0x56, 0xbd, 0xbe } };

BEGIN_INTERFACE_MAP(CGrpWnd, CWnd)
	INTERFACE_PART(CGrpWnd, IID_IGrpWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrpWnd message handlers
LONG CGrpWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	const LONG	ret = 0;
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:	// set data from server
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
			if (m_pCrossLine)
				m_pCrossLine->HideCrossLine();
			m_timerID = 0;
		}
	}
	
	CWnd::OnTimer(nIDEvent);
}

int CGrpWnd::DispatchData(WPARAM wParam, LPARAM lParam)
{
	_extTHx* info = (_extTHx*)lParam;
	const int dlen = info->size;
	char	*pData = info->data;

	RemoveComponent();
	if (dlen < sz_CDJUGA)
	{
		Invalidate();
		return dlen;
	}

	const struct _cdJuga	*pcdJuga = (struct _cdJuga *)pData;
	const int	dCnt = CGrp_Data::CharToInt(pcdJuga->nData, sizeof(pcdJuga->nData));

	m_digit = CGrp_Data::CharToInt(pcdJuga->digit, sizeof(pcdJuga->digit));
	m_pJgga = CGrp_Data::CharToInt(pcdJuga->pjgga, sizeof(pcdJuga->pjgga));
	m_uJgga = CGrp_Data::CharToInt(pcdJuga->ujgga, sizeof(pcdJuga->ujgga));
	m_dJgga = CGrp_Data::CharToInt(pcdJuga->djgga, sizeof(pcdJuga->djgga));
	m_totalDay = dCnt;
	int mgap = CGrp_Data::CharToInt(pcdJuga->mgap, sizeof(pcdJuga->mgap));
	if (mgap >= 900)
		mgap = mgap - 900;
	else
		mgap = mgap * 60;

	int	dPos = sz_CDJUGA;
	m_pGrpData = std::make_unique<CGrp_Data>(this, m_dKind, 0, m_dIndex);
	const int	szFrame = m_pGrpData->AttachGraphData(&pData[dPos], m_totalDay, 60);
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

	return dPos;
}

void CGrpWnd::RealTimeData(LPARAM lParam)
{
	if (!m_pGrpData)
		return;
	struct _alertR* alertR;
	//CString code = alertR->code;
	//code.TrimRight();

	alertR = (struct _alertR*)lParam;
	DWORD* data = nullptr;
	bool	bShift = false;

	// ... 1번째 일경우...
	//data = (DWORD*)alertR->ptr[0];
	for (int ii = alertR->size - 1; ii >= 0; ii--)
	{
		data = (DWORD*)alertR->ptr[ii];
		if (Alert(data))
			bShift = true;
	}
		
	if (m_pGrpData && m_bExpand)
	{
		m_totalDay = m_pGrpData->GetDataCount();
		m_dispDay = m_totalDay;
		m_dispPos = 0;
		m_dispEnd = m_totalDay;
	}

	const bool bRedraw = m_pGrpTick->IsChangeMinMax();

	if (bRedraw || bShift)
	{
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

bool CGrpWnd::Alert(DWORD* data)
{
	m_jgga = m_cvol = m_diff = m_udyl = m_ctim = _T("");

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

		if (data[24]) // R_DIFF
		{
			double dVal = atof(CString((char*)data[24])) * val;
			if (dVal < 0)	dVal *= (-1);
			m_diff.Format("%.f", dVal);
		}

		if (data[32])  //R_CVOL
			m_cvol.Format("%.f", fabs(atof((char*)data[32])));

		if (data[33]) //R_UDYL
			m_udyl = (char*)data[33];


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

		if (data[24]) // R_DIFF
			m_diff = (char*)data[24];

		if (data[32])  //R_CVOL
			m_cvol.Format("%.f", fabs(atof((char*)data[32])));

		if (data[33]) //R_UDYL
			m_udyl = (char*)data[33];

		if (data[34])  //R_CTIM
			m_ctim = (char*)data[34];

		if (!m_diff.IsEmpty())
			m_diff = m_diff.Mid(1);
		break;
	}

	if (m_jgga.IsEmpty() || m_ctim.IsEmpty())
		return false;

	return m_pGrpData->UpdateRTM();
}

bool CGrpWnd::AssignRegion()
{
	m_pGrpRgn = std::make_unique<_RgnInfo>();
	m_pGrpRgn->tick[ctkLEFT].bDraw = (m_TickView & CDT_LEFT)? true:false;
	m_pGrpRgn->tick[ctkRIGHT].bDraw = (m_TickView & CDT_RIGHT)? true:false;
	m_pGrpRgn->tick[ctkLEFT].lcolor = RGB(192, 192, 192);
	m_pGrpRgn->tick[ctkLEFT].lstyle = PS_SOLID;
	m_pGrpRgn->tick[ctkLEFT].lwidth = 1;
	m_pGrpRgn->tick[ctkRIGHT].lcolor = RGB(192, 192, 192);
	m_pGrpRgn->tick[ctkRIGHT].lstyle = PS_SOLID;
	m_pGrpRgn->tick[ctkRIGHT].lwidth = 1;
	m_pGrpRgn->tick[ctkBOTTOM].lcolor = RGB(192, 192, 192);
	m_pGrpRgn->tick[ctkBOTTOM].lstyle = PS_SOLID;
	m_pGrpRgn->tick[ctkBOTTOM].lwidth = 1;
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

	m_pGrpTick = std::make_unique<CGrp_Tick>(m_pView, this, (char *)&gInfo);

	return true;
}

void CGrpWnd::DrawGraph(CDC *pDC)
{
	DrawEmpty(pDC, true);
	if (!m_pGrpTick)
	{
		DrawTitle(pDC);
		DrawEmpty(pDC, false);
		return;
	}

	m_pGrpTick->DrawGraph(pDC);
	DrawEmpty(pDC, false);
	m_pCrossLine->DrawCrossLine(pDC);
	DrawTitle(pDC);
}

void CGrpWnd::DrawTitle(CDC *pDC)
{
	if (!m_title.IsEmpty())
	{
		CSize szText;
		CRect rcText;
		CFont *ofont;
		ofont = pDC->SelectObject(m_pFont);

		rcText = m_GrpRect;
		rcText.InflateRect(-4, -4, -4, -4);
		szText = pDC->GetTextExtent(m_title);
		rcText.right  = rcText.left + szText.cx;
		rcText.bottom = rcText.top  + szText.cy;
		pDC->DrawText(m_title, rcText, DT_LEFT | DT_TOP | DT_SINGLELINE | DT_NOPREFIX);

		pDC->SelectObject(ofont);
	}
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
		CBrush	*fBrush = m_pApp->GetBrush(m_pView, RGB(0,0,0));
		pDC->FrameRect(&m_GrpRect, fBrush);
	}
}

void CGrpWnd::Resize()
{
	GetClientRect(&m_ObjRect);

	if (!m_pGrpRgn)
		return;

	CDC *pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(m_pFont);
	CSize	sz = pDC->GetOutputTextExtent("99,999,999");
	if (m_pGrpTick)
	{
		CString	sMax, sMin;
		m_pGrpTick->GetMaxStr(sMax);
		m_pGrpTick->GetMaxStr(sMin);
		const CSize	szMax = pDC->GetOutputTextExtent(sMax);
		const CSize	szMin = pDC->GetOutputTextExtent(sMin);

		if (szMax.cx > szMin.cx)
		{
			if (sz.cx > szMax.cx + 10)	sz.cx = szMax.cx + 10;
		}
		else
		{
			if (sz.cx > szMin.cx + 10)	sz.cx = szMin.cx + 10;
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

	m_pGrpRgn->tkHeight = sz.cy;
	m_pGrpRgn->tick[ctkLEFT].tkRect = 
		CRect(0, m_GrpRect.top, m_GrpRect.left, int(m_GrpRect.Height()));

	m_pGrpRgn->tick[ctkRIGHT].tkRect = 
		CRect(m_GrpRect.right, m_GrpRect.top, 
		m_ObjRect.right, int(m_GrpRect.Height()));

	m_pGrpRgn->tick[ctkBOTTOM].tkRect = bottomRC;
	m_pGrpRgn->gpRect.top = m_pGrpRgn->tick[ctkLEFT].tkRect.top;
	m_pGrpRgn->gpRect.bottom = m_pGrpRgn->tick[ctkLEFT].tkRect.bottom;
	m_pGrpRgn->gpRect.left = m_pGrpRgn->tick[ctkLEFT].tkRect.right;
	m_pGrpRgn->gpRect.right = m_pGrpRgn->tick[ctkRIGHT].tkRect.left;

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	m_pGrpTick->Resize();
	m_pCrossLine->Resize(m_GrpRect);
}

/*****/
// d : display count - 0:all
// r : request count
// t : draw tick 
// o : outline
// e : data expand
// u : data unit - 1:종목, 2:업종, 3:선물, 4:업종, 5:ECN
// i : data index - 1:틱, 2:분
/*****/
void CGrpWnd::ParseParam(char *param)
{
	m_sParam.Format("%s", param);
	CString	sParam = m_sParam;

	CString	sTmp, sToken;
	int		nPos = 0;

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

	// out line
	sToken = "/o";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)	m_bOutLine = true;
	else		m_bOutLine = false;

	// data expand
	sToken = "/e";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)	m_bExpand = true;
	else 		m_bExpand = false;

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
	m_pGrpRgn.reset();
	m_pGrpData.reset();
	m_pGrpTick.reset();
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


void CGrpWnd::SetProperties(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here

}

BSTR CGrpWnd::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
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
	if (!m_pGrpTick)
		return _T("");

	CString	dispStr = m_pGrpTick->GetDisplayPosData(point);

	return dispStr;
}

void CGrpWnd::_SetTitle(LPCTSTR title) 
{
	// TODO: Add your dispatch handler code here
	m_title.Format("%s", title);
}
