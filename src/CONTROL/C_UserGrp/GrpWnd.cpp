// GrpWnd.cpp : implementation file
//

#include "stdafx.h"
#include "C_UserGrp.h"
#include "GrpWnd.h"
#include "Grp_Data.h"
#include "Grp_Base.h"
#include "Grp_Line.h"
#include "Grp_Bar.h"
#include "Grp_VBar.h"
#include "ToolTip.h"
#include "CrossLine.h"
#include "../../H/memDc.h"

#include <math.h>
#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrpWnd
#define	TIMER_STRID	10000
#define	TIMER_STRGAP	250
#define	TIMER_CROSSID	10001
#define	TIMER_CROSSGAP	1000

#define	TIMER_PREID	10002
#define	TIMER_PREGAP	100
#define	TIMER_RTSID	10003
#define	TIMER_RTSGAP	2000


#define	CLR_CROSS	RGB(180,77,77)

CGrpWnd::CGrpWnd(CWnd *pView, CWnd *pParent, struct _param *pInfo)
{
	m_pView = pView;
	m_pParent = pParent;
	m_pToolTip = std::make_unique<CToolTip>(m_pView, this);
	m_pCrossLine = std::make_unique<CCrossLine>(m_pView, this);
	m_bStringIndex = false;
	m_bPreview = false;
	m_bRevDraw = false;
	m_bTickVertic = false;
	m_bItemRts = false;
	m_b3NChart = false;
	
	m_RgnCnt = m_GrpCnt = 0;
	m_pDataInfo = nullptr;
	for (int ii = 0; ii < MAX_REGION; ii++)
		m_pRgnInfo[ii] = nullptr;
	for (int ii = 0; ii < MAX_GRAPH; ii++)
		m_pGrpInfo[ii] = nullptr;

	m_MinMaxCnt = 0;

	m_ReqCnt = 100;
	m_ShowCnt = 100;
	m_totalDay = 0;

	m_fPoint = pInfo->point;
	m_fName.Format("%s", pInfo->fonts);
	
	m_tOrgRGB = pInfo->tRGB;
	m_pOrgRGB = pInfo->pRGB;
	m_tRGB = GetPalette(m_tOrgRGB);
	m_pRGB = GetPalette(m_pOrgRGB);
	
	m_dIndex = CDI_DAY;
	ParseParam((char*)pInfo->options.operator LPCTSTR());	
	m_TkTextColor = m_tRGB;
	m_TkLineColor = GetTickColor(m_TkTextColor);

	m_pApp = (CC_UserGrpApp*)AfxGetApp();

	SetError("");
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
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(CM_GRP, OnGrp)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGrpWnd message handlers

LONG CGrpWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		return DispatchOubData(wParam, lParam);
		break;
	case DLL_OUBx:		// set data from server
		return DispatchData(wParam, lParam);
		break;
	case DLL_INB:		// get data from out
		return RequestHead();
		break;
	case DLL_ALERTx:
		RealTimeData((struct _alertR*)lParam);
		break;
	case DLL_SETPAL:	// Palette Change
		ChangePalette();
		break;
	case DLL_SETFONT:
	case DLL_SETFONTx:	//Font Change
		{
			m_fPoint = HIWORD(wParam);
			CString fName = (char *)lParam;

			if (!fName.IsEmpty())
				m_fName.Format("%s", fName);

			m_pFont = m_pApp->GetFont(m_pView, m_fPoint, m_fName);
			m_pBoldFont = m_pApp->GetFont(m_pView, m_fPoint, m_fName, FW_BOLD);
			Resize();
			Invalidate();
		}				
		break;
	}

	return 0;
}

LONG CGrpWnd::OnGrp(WPARAM wParam, LPARAM lParam)
{
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
		case ChgParam:
			ParseParam((char*)lParam);
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

	return 0;
}

int CGrpWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pToolTip->Create();
	m_pToolTip->ShowWindow(SW_HIDE);
	m_pFont = m_pApp->GetFont(m_pView, m_fPoint, m_fName);
	m_pBoldFont = m_pApp->GetFont(m_pView, m_fPoint, m_fName, FW_BOLD);

	if (m_bPreview)
		SetTimer(TIMER_PREID, TIMER_PREGAP, nullptr);

	return 0;
}

void CGrpWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	xxx::CMemDC	mdc(&dc);
	DrawGraph(&mdc);
}

void CGrpWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx == 0 || cy == 0)
		return;

	Resize();
	Invalidate();
}

void CGrpWnd::OnTimer(UINT nIDEvent) 
{
	if (!GetSafeHwnd())
		return;

	switch (nIDEvent)
	{
	case TIMER_STRID:
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
		break;
	case TIMER_CROSSID:
		KillTimer(nIDEvent);
		if (!IsPtInGrp())
		{
			m_pToolTip->ShowWindow(SW_HIDE);
			m_pCrossLine->HideCrossLine();
			m_timerID = 0;
		}
		break;
	case TIMER_PREID:
		{
			KillTimer(nIDEvent);

			_TestInfo	ti;
			ti.gcount = m_arGrpLine.GetSize();
			ti.bVerticStr = m_bTickVertic;
			ti.bItemRts = m_bItemRts;
			for (int ii =0; ii < ti.gcount; ii++)
				ti.gKind[ii] = atoi(CString(m_arGrpLine.GetAt(ii).kind, sizeof(m_arGrpLine.GetAt(ii).kind)));

			m_pParent->SendMessage(CM_PREVIEW, long(&ti), RequestHead());
		}
		break;
	case TIMER_RTSID:
		{
			static	int seq = 0;
			char	pRts[64]{};
			int	val = rand()*10 + rand();

			if (seq%5 == 0)
				val *= -1;

			if (m_bItemRts)
			{
				sprintf(pRts, "%s\t%03d\t-%d", RS_CODE, RS_BASE + rand()%m_totalDay, val);
				seq++;
				SendMessage(WM_USER, MAKEWPARAM(DLL_ALERT, strlen(pRts)), (LPARAM)pRts);
			}
			else
			{
				sprintf(pRts, "%s\t%s\t%s\t%s\t%d\t", RS_CODE, RS_CTIM, "000000", RS_SYM, val);
				seq++;
				SendMessage(WM_USER, MAKEWPARAM(DLL_ALERT, strlen(pRts)), (LPARAM)pRts);
			}
		}
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CGrpWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_timerID > 0)
		KillTimer(m_timerID);

	if (m_pToolTip)
	{
		m_pToolTip->DestroyWindow();
	}

	RemoveComponent();	
}

void CGrpWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CString	str = GetTipStr(point);
	if (m_PosData != str)
	{
		m_pToolTip->ShowWindow(SW_HIDE);
		m_PosData.Format("%s", str);
		KillTimer(TIMER_STRID);
		SetTimer(TIMER_STRID, TIMER_STRGAP, nullptr);
	}

	MouseMove(false, point, true);

	CWnd::OnMouseMove(nFlags, point);
}

int CGrpWnd::DispatchOubData(WPARAM wParam, LPARAM lParam)
{
	int	dlen = HIWORD(wParam);
	DWORD	dAdd = HIBYTE(LOWORD(wParam));
	char* pData = (char*)lParam;
	dAdd = (dAdd << 16) & 0x00FF0000;
	dlen |= dAdd;
	int	nGap = 0;

	RemoveComponent();
	if (dlen < sz_DNHEAD)
	{
		SetError("length < sz_DnHead");
		Invalidate();
		return dlen;
	}

	const	struct	_DnHead* pDnHead = (struct _DnHead*)pData;
	const	int	nData = CGrp_Data::CharToInt(pDnHead->nData, sizeof(pDnHead->nData));	// data count
	const	int	iSize = CGrp_Data::CharToInt(pDnHead->irSize, sizeof(pDnHead->irSize));	// index size
	const	int	dSize = CGrp_Data::CharToInt(pDnHead->drSize, sizeof(pDnHead->drSize));	// data size
	int	nHead = CGrp_Data::CharToInt(pDnHead->nHead, sizeof(pDnHead->nHead));	// graph count

	if (nHead <= 0)	//|| nData <= 0)
	{
		SetError("Graph Count is 0");

		Invalidate(FALSE);
		return sz_DNHEAD;
	}

	const int nFullsize = sz_DNHEAD + (nData * iSize) + nHead * (sz_DATAINFO + nData * dSize);
	if (dlen != nFullsize)
	{
		CString	str;
		str.Format("size error[%d/%d]", dlen, nFullsize);
		SetError(str);

		Invalidate(FALSE);
		return dlen;
	}

	if (!AssignRegion(m_CfgRgnCnt))
	{
		SetError("AssignRegion Error");

		Invalidate(FALSE);
		return 0;
	}

	m_sTickUnit[0].Format("%s", CString(pDnHead->xUnit[1], sizeof(pDnHead->xUnit[1])));	m_sTickUnit[0].Trim();
	m_sTickUnit[1].Format("%s", CString(pDnHead->xUnit[2], sizeof(pDnHead->xUnit[2])));	m_sTickUnit[1].Trim();
	m_sTickUnit[2].Format("%s", CString(pDnHead->xUnit[0], sizeof(pDnHead->xUnit[0])));	m_sTickUnit[2].Trim();
	m_bStringIndex = (pDnHead->xtype == ' ') ? false : true;
	m_bStringIndex = (m_bTickVertic) ? true : m_bStringIndex;

	m_sTipName = CString(pDnHead->tipName, sizeof(pDnHead->tipName));	m_sTipName.TrimRight();
	if (!m_sTipName.IsEmpty())
	{
		int	index = m_sTipName.Find('\t');
		if (index >= 0)
		{
			CString	tipname = m_sTipName.Left(index++);
			CString sName = m_sTipName.Mid(index);
			m_sTipName = tipname;
			m_sExtraName = sName;
		}
	}

	m_totalDay = nData;
	int	dPos = sz_DNHEAD;
	int	dGap = CGrp_Data::CharToInt(pDnHead->mgap, sizeof(pDnHead->mgap));

	if (m_dIndex < CDI_DAY)
	{
		if (dGap >= 900)	// 초틱 : 9xx(0초는 무조건 추가하는 틱차트)
		{
			//m_dIndex = CDI_TICK;
			dGap = dGap - 900;
		}
		else			// 분틱 : xx
		{
			//m_dIndex = CDI_MIN;
			dGap = dGap * 60;
		}
	}

	if (m_arGrpLine.GetSize() < nHead)
		nHead = m_arGrpLine.GetSize();

	m_pDataInfo = std::make_unique<CGrp_Data>(this, m_dIndex, dGap);
	// 20080911
	const	int	len = m_pDataInfo->AttachGraphData(&pData[dPos], nHead, nData, iSize, dSize);
	dPos += len;
	//-----------------------------------

	if (m_ShowCnt <= 0 || m_ShowCnt > m_totalDay)
		m_dispDay = m_totalDay;
	else
		m_dispDay = m_ShowCnt;

	m_dispPos = m_totalDay - m_dispDay;
	m_dispEnd = m_dispPos + m_dispDay;

	m_pCrossLine = std::make_unique<CCrossLine>(m_pView, this, m_pRGB, CLR_CROSS);

	AssignGraph(nHead);
	ReviseTick();
	Resize();
	Invalidate();

	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Enable, TRUE), 0);
	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_UpdateScr, 0), 0);

	if (m_bPreview && m_bItemRts)
		SetTimer(TIMER_RTSID, TIMER_RTSGAP, nullptr);

	return 0;
}

int CGrpWnd::DispatchData(WPARAM wParam, LPARAM lParam)
{
	struct _extTHx* info = (struct _extTHx*)lParam;
	const int dlen = info->size;
	char	*pData = info->data;

	RemoveComponent();
	if (dlen < sz_DNHEAD)
	{
		SetError("length < sz_DnHead");
		Invalidate();
		return dlen;
	}

	const	struct	_DnHead	*pDnHead = (struct _DnHead *)pData;
	//const	int	nData = CGrp_Data::CharToInt(pDnHead->nData, sizeof(pDnHead->nData));	// data count
	int	nData = CGrp_Data::CharToInt(pDnHead->nData, sizeof(pDnHead->nData));	// data count
	const	int	iSize = CGrp_Data::CharToInt(pDnHead->irSize, sizeof(pDnHead->irSize));	// index size
	const	int	dSize = CGrp_Data::CharToInt(pDnHead->drSize, sizeof(pDnHead->drSize));	// data size
	int	nHead = CGrp_Data::CharToInt(pDnHead->nHead, sizeof(pDnHead->nHead));	// graph count

	if (nHead <= 0)	//|| nData <= 0)
	{
		SetError("Graph Count is 0");

		Invalidate(FALSE);
		return sz_DNHEAD;
	}

	const int nFullsize = sz_DNHEAD + (nData * iSize) + nHead * (sz_DATAINFO + nData * dSize);

	CString slog;
	slog.Format("[usergrp] nFullsize=[%d]  dlen=[%d] nData=[%d] iSize=[%d] dSize=[%d] nHead=[%d] ", nFullsize, dlen, nData, iSize, dSize, nHead);
	OutputDebugString(slog);

	slog.Format("[usergrp] sz_DNHEAD=[%d]  sz_DATAINFO=[%d]  ", sz_DNHEAD, sz_DATAINFO);
	OutputDebugString(slog);

	int iData = (dlen - sz_DNHEAD - (nHead * sz_DATAINFO)) / (iSize + nHead * dSize);
	slog.Format("[usergrp] iData=[%d]    ", iData);
	OutputDebugString(slog);


	if (dlen != nFullsize)
	{
		CString	str;
		str.Format("size error[%d/%d]", dlen, nFullsize);
		SetError(str);

		Invalidate(FALSE);
		return dlen;
	}

	if (!AssignRegion(m_CfgRgnCnt))
	{
		SetError("AssignRegion Error");

		Invalidate(FALSE);
		return 0;
	}

	m_sTickUnit[0].Format("%s", CString(pDnHead->xUnit[1], sizeof(pDnHead->xUnit[1])));	m_sTickUnit[0].Trim();
	m_sTickUnit[1].Format("%s", CString(pDnHead->xUnit[2], sizeof(pDnHead->xUnit[2])));	m_sTickUnit[1].Trim();
	m_sTickUnit[2].Format("%s", CString(pDnHead->xUnit[0], sizeof(pDnHead->xUnit[0])));	m_sTickUnit[2].Trim();
	m_bStringIndex = (pDnHead->xtype == ' ')? false:true;
	m_bStringIndex = (m_bTickVertic)? true:m_bStringIndex;

	m_sTipName = CString(pDnHead->tipName, sizeof(pDnHead->tipName));	m_sTipName.TrimRight();
	if (!m_sTipName.IsEmpty())
	{
		int	index = m_sTipName.Find('\t');
		if (index >= 0)
		{
			CString	tipname = m_sTipName.Left(index++);
			CString sName = m_sTipName.Mid(index);
			m_sTipName = tipname;
			m_sExtraName = sName;
		}
	}

	m_totalDay = nData;
	int	dPos = sz_DNHEAD;
	int	dGap = CGrp_Data::CharToInt(pDnHead->mgap, sizeof(pDnHead->mgap));

	if (m_dIndex < CDI_DAY)
	{
		if (dGap >= 900)	// 초틱 : 9xx(0초는 무조건 추가하는 틱차트)
		{
			//m_dIndex = CDI_TICK;
			dGap = dGap - 900;
		}
		else			// 분틱 : xx
		{
			//m_dIndex = CDI_MIN;
			dGap = dGap * 60;			
		}
	}

	if (m_arGrpLine.GetSize() < nHead)
		nHead = m_arGrpLine.GetSize();

	m_pDataInfo = std::make_unique<CGrp_Data>(this, m_dIndex, dGap);
// 20080911
	const	int	len = m_pDataInfo->AttachGraphData(&pData[dPos], nHead, nData, iSize, dSize);
	dPos += len;
//-----------------------------------

	if (m_ShowCnt <= 0 || m_ShowCnt > m_totalDay)
		m_dispDay = m_totalDay;
	else
		m_dispDay = m_ShowCnt;

	m_dispPos = m_totalDay - m_dispDay;
	m_dispEnd = m_dispPos + m_dispDay;

	m_pCrossLine = std::make_unique<CCrossLine>(m_pView, this, m_pRGB, CLR_CROSS);

	AssignGraph(nHead);
	ReviseTick();
	Resize();
	Invalidate();

	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Enable, TRUE), 0);
	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_UpdateScr, 0), 0);

	if (m_bPreview && m_bItemRts)
		SetTimer(TIMER_RTSID, TIMER_RTSGAP, nullptr);

	return 0;
}


void CGrpWnd::RealTimeData(struct _alertR* alert)
{
	if (!m_pDataInfo || !m_pDataInfo->m_bRealTime || m_GrpCnt <= 0)
		return;

	bool	bChange = false;
	CString sCode = alert->code;
	if (sCode != m_pDataInfo->m_pInfo[0]->RtsCode)  //test rts
	//if (m_sRTSCode != sCode)
		return;
	
	for (int ii = alert->size - 1; ii >= 0; ii--)
	{
		if (m_bItemRts)
		{
			if (RealTimeItems(sCode, (DWORD*)alert->ptr[ii]))
				bChange = true;

		}
		else
		{
			if (RealTimeBasic(sCode, (DWORD*)alert->ptr[ii]))
				bChange = true;
		}
	}

	if (bChange)
	{
		if (m_pDataInfo->GetDataCount() == 1)
			Resize();
		else
			RecalculateMinMax();
		Invalidate();
	}
}

bool CGrpWnd::RealTimeBasic(CString sCode, DWORD* data)
{
	CString	sVal, sTime = "";
	if (data[34])		// RS_CTIM
	{
		sTime = (char*)data[34];
		if (sTime.IsEmpty())
			return false;
	}
	else
		return false;

	if (sTime == "000000")
		sTime = "120000";

	bool	bChange = false;
	bool	bInsert = true;
	int	iResult = 0;
	double	dVal = 0.;

	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		if (sCode == m_pDataInfo->m_pInfo[ii]->RtsCode)  //test rts
		//if(m_sRTSCode == sCode)
		{
			CString sKey = m_pDataInfo->m_pInfo[ii]->RtsSymbol;
			if (data[atoi(sKey)])
			{	// 각각의 그래프는 코드와 심볼 두개를 가지고 있다
				sVal = (char*)data[atoi(sKey)];
				if (sVal.IsEmpty())
					return false;

				bChange = true;
				switch(m_pDataInfo->m_pInfo[ii]->RtsSign)
				{
				case RM_REMOVE:
					sVal = sVal.Mid(1);
					break;
				case RM_MINUS:
					if (sVal.GetAt(0) == '-')
						sVal = sVal.Mid(1);
					break;
				default:
					break;
				}

				dVal = atof(sVal) * m_pDataInfo->m_pInfo[ii]->power;
				sVal.Format("%.f", dVal);
				iResult = m_pDataInfo->RealTimeData(ii, sTime, sVal, bInsert);
				if (iResult == RTS_INSERT)
					bInsert = false;
			}
		}
	}
	return bChange;
}

bool CGrpWnd::RealTimeItems(CString sCode, DWORD* data)
{
	if (m_pDataInfo->m_arItemRts.GetSize() < 1)
		return false;

	if (sCode != m_pDataInfo->m_pInfo[0]->RtsCode)  //test rts
	//if (sCode != m_sRTSCode)
		return false;
	
	CString	sVal;
	bool	bChange = false;
	for (int ii = 0; ii < m_totalDay; ii++)
	{		
		CString skey = m_pDataInfo->m_arItemRts.GetAt(ii);		
		if (data[atoi(skey)])
		{
			bChange = true;
			sVal = (char*)data[atoi(skey)];
			if (sVal.IsEmpty())
				return false;

			switch(m_pDataInfo->m_pInfo[0]->RtsSign)
			{
			case RM_REMOVE:
				sVal = sVal.Mid(1);
				break;
			case RM_MINUS:
				if (sVal.GetAt(0) == '-')
					sVal = sVal.Mid(1);
				break;
			default:
				break;
			}

			sVal.Format("%.f", atof(sVal) * m_pDataInfo->m_pInfo[0]->power);
			m_pDataInfo->RealTimeItemData(ii, sVal);
		}
	}
	return bChange;
}

bool CGrpWnd::AssignRegion(int nRgn)
{	
	const int lColor = GetTickColor(m_tRGB);

	m_RgnCnt = nRgn;
	for (int ii = 0; ii < m_RgnCnt; ii++)
	{
		m_pRgnInfo[ii] = std::make_unique<_RgnInfo>();
		m_pRgnInfo[ii]->tick[ctkLEFT].bDraw = (m_TickView & CDT_LEFT)? true:false;
		m_pRgnInfo[ii]->tick[ctkRIGHT].bDraw = (m_TickView & CDT_RIGHT)? true:false;
		m_pRgnInfo[ii]->tick[ctkLEFT].lcolor = lColor;
		m_pRgnInfo[ii]->tick[ctkLEFT].lstyle = PS_DOT;
		m_pRgnInfo[ii]->tick[ctkLEFT].lwidth = 1;
		m_pRgnInfo[ii]->tick[ctkRIGHT].lcolor = lColor;
		m_pRgnInfo[ii]->tick[ctkRIGHT].lstyle = PS_DOT;
		m_pRgnInfo[ii]->tick[ctkRIGHT].lwidth = 1;
		m_pRgnInfo[ii]->tick[ctkBOTTOM].lcolor = lColor;
		m_pRgnInfo[ii]->tick[ctkBOTTOM].lstyle = PS_DOT;
		m_pRgnInfo[ii]->tick[ctkBOTTOM].lwidth = 1;
	}
	return true;
}

bool CGrpWnd::AssignGraph(int nGrp)
{
	struct _gNInfo	gInfo{};
	struct _GrpLine	GrpLine{};

	if (m_arGrpLine.GetSize() < nGrp)
		return false;

	CString	msg = "", str;
	for (int ii = 0, dKey = 0; ii < nGrp; ii++, dKey++)
	{
		GrpLine = m_arGrpLine.GetAt(ii);
		gInfo.gKind = atoi(CString(GrpLine.kind, sizeof(GrpLine.kind)));
		gInfo.rKey = GrpLine.region - 1;
		gInfo.dKey = dKey;
		gInfo.dIndex = m_dIndex;
		gInfo.Tick = 0;
		gInfo.lwidth = GrpLine.width;
		gInfo.lstyle = GrpLine.style;

		if (GrpLine.tick & ICF_LEFT && m_TickView & CDT_LEFT)		gInfo.Tick |= CDT_LEFT;
		if (GrpLine.tick & ICF_RIGHT && m_TickView & CDT_RIGHT)		gInfo.Tick |= CDT_RIGHT;

		gInfo.mIndex = (GrpLine.option1 & IGO_MINMAX)? 0:-1;
		gInfo.lIndex = (GrpLine.option1 & IGO_NAMER)? 1:0;
		gInfo.pRGB = m_pRGB;
		gInfo.tRGB = atoi(CString(GrpLine.color, sizeof(GrpLine.color)));
		switch (gInfo.gKind)
		{
		case IGK_LINE:
		case IGK_DOT:
		case IGK_YBASELINE:
			m_pGrpInfo[m_GrpCnt] = std::make_unique<CGrp_Line>(m_pView, this, (char *)&gInfo);
			break;
		case IGK_2D1NBAR:
			m_pGrpInfo[m_GrpCnt] = std::make_unique<CGrp_Bar>(m_pView, this, (char *)&gInfo);
			break;
		case IGK_2D3NBAR:
			m_pGrpInfo[m_GrpCnt] = std::make_unique<CGrp_Bar>(m_pView, this, (char *)&gInfo);
			dKey += 2;
			break;
		default:
			m_pGrpInfo[m_GrpCnt] = std::make_unique<CGrp_VBar>(m_pView, this, (char *)&gInfo);
			break;
		}

		m_GrpCnt++;
	}

	return true;
}

void CGrpWnd::DrawGraph(CDC *pDC)
{
	DrawEmpty(pDC, true);
	if (m_GrpCnt == 0)
	{
		DrawEmpty(pDC, false);
		return;
	}

	for_each(m_pMinMaxChk.begin(), m_pMinMaxChk.end(), [](const auto& item) {
		item->Max[0] = INT_MIN;
		item->Max[1] = INT_MIN;
		item->Min[0] = INT_MAX;
		item->Min[1] = INT_MAX;
		item->tickx[0] = 0;
		item->tickx[1] = 0;
		item->ticky[0] = 0;
		item->ticky[1] = 0;
	});

	int	mIndex = 0;
	double	dMin = 0, dMax = 0;
	struct _NMinMaxChk *pMinMaxChk = nullptr;
	class CGrp_Base	*pBase = nullptr;
	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		pBase = m_pGrpInfo[ii].get();
		mIndex = pBase->GetMinMaxIndex();
		pMinMaxChk = GetMinMaxChk(pBase->GetRgnKey());
		if (pMinMaxChk && mIndex >= 0 && pMinMaxChk->coMinMax[mIndex])
		{
			if (pBase->CalculateMinMax())
			{
				pBase->GetMinMaxValue(dMin, dMax);
				if (dMin != INT_MAX && dMax != INT_MIN)
				{
					if (pMinMaxChk->Max[mIndex] < dMax)
						pMinMaxChk->Max[mIndex] = dMax;
					if (pMinMaxChk->Min[mIndex] > dMin)
						pMinMaxChk->Min[mIndex] = dMin;
				}
			}
		}
	}

	if (m_TickView & CDT_BOTTOM)
		m_pGrpInfo[0]->DrawTickBottom(pDC);

	if (m_bRevDraw)
	{
		for (int ii = m_GrpCnt - 1; ii >= 0; ii--)
		{
			m_pGrpInfo[ii]->DrawGraph(pDC);
		}
	}
	else
	{
		for (int ii = 0; ii < m_GrpCnt; ii++)
		{
			m_pGrpInfo[ii]->DrawGraph(pDC);
		}
	}

	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		if(m_pGrpInfo[ii]->m_bHide == FALSE)
			m_pGrpInfo[ii]->DrawName(pDC);
	}

	DrawEmpty(pDC, false);
	m_pCrossLine->DrawCrossLine(pDC);

}

void CGrpWnd::SetShowLine(int nLine,bool bflag)
{
	if(m_GrpCnt > nLine)
	{
		if(nLine > -1)
		{
			if(bflag == false)
				m_pGrpInfo[nLine]->m_bHide = TRUE;
			else
				m_pGrpInfo[nLine]->m_bHide = FALSE;
		}
	}
}

void CGrpWnd::DrawEmpty(CDC *pDC, bool bInit)
{
	if (bInit)
	{
		CBrush	*pBrush = m_pApp->GetBrush(m_pView, m_pRGB);
		pDC->FillRect(&m_ObjRect, pBrush);

		DrawFrameRect(pDC, m_GrpRect, m_tRGB);
	}
	else
	{
		DrawFrameRect(pDC, m_GrpRect, m_tRGB);

		if (!m_sError.IsEmpty())
		{
			/*const	int	sColor = pDC->SetTextColor(RGB(128, 0, 0));
			pDC->DrawText(m_sError, &m_GrpRect, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			pDC->SetTextColor(sColor);*/
			OutputDebugString(+"\r\n " + m_sError);
		}
		else if (m_RgnCnt > 1)
		{
			CPen	*cPen;
			cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(128,128,128));
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

void CGrpWnd::ClearGrp()
{
	RemoveComponent();
	Invalidate();
}

void CGrpWnd::Resize()
{
	GetClientRect(&m_ObjRect);
	m_GrpRect = m_ObjRect;

	CDC *pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(m_pFont);
	CSize	sz2, sz = pDC->GetOutputTextExtent("999");

	if (m_RgnCnt == 0 || m_GrpCnt == 0)
	{
		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);

		m_GrpRect.top += 3;
		if (m_TickView & CDT_BOTTOM)
		{
			m_GrpRect.bottom -= sz.cy + 5;
		}
		else
		{
			m_GrpRect.bottom -= 5;
		}

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

		return;
	}

	int	ii;
	for (ii = 0; ii < m_GrpCnt; ii++)	// 각 그래프의 최대최소를 비교하여 조정
	{
		if (m_pGrpInfo[ii])
		{
			RecalculateMinMax();

			double dMax, dMin;
			m_pGrpInfo[ii]->GetMinMaxValue(dMin, dMax);
			if (dMax == INT_MIN || dMin == INT_MAX)	// 20080911
				continue;

			CString tmpS;
			m_pGrpInfo[ii]->FormatVariableComma(tmpS, dMax, m_pDataInfo->m_pInfo[ii]->digit);
			tmpS += "  ";
			sz2 = pDC->GetOutputTextExtent(tmpS);
			if (sz2.cx > sz.cx)
				sz = sz2;

			m_pGrpInfo[ii]->FormatVariableComma(tmpS, dMin, m_pDataInfo->m_pInfo[ii]->digit);
			tmpS += "   ";
			sz2 = pDC->GetOutputTextExtent(tmpS);
			if (sz2.cx > sz.cx)
				sz = sz2;
		}
	}

	double	pHeight[MAX_REGION]{};
	const	int	iRate = (int)(100.0/(double)m_RgnCnt);
	const	int	iMod = 100 - iRate * m_RgnCnt;

	sz2 = sz;
	if (m_CfgRgnInfo.GetLength() == m_CfgRgnCnt * 2 && m_CfgRgnCnt == m_RgnCnt)
	{
		for (ii = 0; ii < m_RgnCnt; ii++)
		{
			pHeight[ii] = atoi(m_CfgRgnInfo.Mid(ii*2, 2)) * 0.01;
		}
	}
	else
	{
		for (ii = 0; ii < m_RgnCnt; ii++)
		{
			if (ii == m_RgnCnt - 1)
				pHeight[ii] = (iRate + iMod) * 0.01;
			else
				pHeight[ii] = iRate * 0.01;
		}
	}

	CRect	bottomRC = m_ObjRect;

	if (m_TickView & CDT_BOTTOM)
	{
		if (m_bTickVertic && m_pDataInfo && m_pDataInfo->GetDataCount() > 0)
			sz.cy = CetBottomVerticHeight(pDC);

		bottomRC.top = bottomRC.bottom - sz.cy - 5;
	}
	else
	{
		bottomRC.top = bottomRC.bottom - 5;
	}
		
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

	for (ii = 0; ii < m_RgnCnt; ii++)
			m_pRgnInfo[ii]->tkHeight = sz2.cy;

	bottomRC.left = m_GrpRect.left;
	bottomRC.right = m_GrpRect.right;

	int	top = m_GrpRect.top;
	for (ii = 0; ii < m_RgnCnt; ii++)
	{
		m_pRgnInfo[ii]->tick[ctkLEFT].tkRect = 
			CRect(0, top, m_GrpRect.left, top + (int)(m_GrpRect.Height()*pHeight[ii]));

		m_pRgnInfo[ii]->tick[ctkRIGHT].tkRect = 
			CRect(m_GrpRect.right, top, m_ObjRect.right, top + (int)(m_GrpRect.Height()*pHeight[ii]));

		m_pRgnInfo[ii]->tick[ctkBOTTOM].tkRect = bottomRC;
		m_pRgnInfo[ii]->gpRect.top = m_pRgnInfo[ii]->tick[ctkLEFT].tkRect.top;
		m_pRgnInfo[ii]->gpRect.bottom = m_pRgnInfo[ii]->tick[ctkLEFT].tkRect.bottom;
		m_pRgnInfo[ii]->gpRect.left = m_pRgnInfo[ii]->tick[ctkLEFT].tkRect.right;
		m_pRgnInfo[ii]->gpRect.right = m_pRgnInfo[ii]->tick[ctkRIGHT].tkRect.left;

		top = top + (int)(m_GrpRect.Height()*pHeight[ii]);
	}

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	for (ii = 0; ii < m_GrpCnt; ii++)
		m_pGrpInfo[ii]->Resize();
	m_pCrossLine->Resize(m_GrpRect);
}

void CGrpWnd::ParseParam(char *param)
{
	CString	sParam;
	sParam.Format("%s", param);
	if (sParam.GetLength() < sz_GRPINFO + sz_GRPLINE + 2)
		return;

	int	index = sParam.Find("/i", 0);
	if (index < 0)
		return;

	CString	sInfo = sParam.Mid(index + 2);
	const	char	*pBuf = (char *)sInfo.operator LPCTSTR();
	const	_GrpInfo *pGrpInfo = (_GrpInfo *)&pBuf[0];
	m_ShowCnt = atoi(CString(pGrpInfo->show, sizeof(pGrpInfo->show)));
	m_ReqCnt = atoi(CString(pGrpInfo->req, sizeof(pGrpInfo->req)));
	m_dIndex = atoi(CString(pGrpInfo->index, sizeof(pGrpInfo->index)));

	m_TickView = 0;
	if ((pGrpInfo->tick & ICF_LEFT) == ICF_LEFT)		m_TickView |= CDT_LEFT;
	if ((pGrpInfo->tick & ICF_RIGHT) == ICF_RIGHT)		m_TickView |= CDT_RIGHT;
	if ((pGrpInfo->tick & ICF_BOTTOM) == ICF_BOTTOM)	m_TickView |= CDT_BOTTOM;

	m_bCtrl = m_bHDraw = m_bVDraw = m_bMinMax = m_bPreview = m_bRevDraw = m_bTickVertic = false;
	m_bItemRts = m_b3NChart = false;

	if (pGrpInfo->ctrl > ICF_CHAR)				m_bCtrl = true;
	if ((pGrpInfo->option1 & ICF_HDRAW) == ICF_HDRAW)	m_bHDraw = true;
	if ((pGrpInfo->option1 & ICF_VDRAW) == ICF_VDRAW)	m_bVDraw = true;
	if ((pGrpInfo->option1 & ICF_MINMAX) == ICF_MINMAX)	m_bMinMax = true;

	if ((pGrpInfo->option2 & ICF_PREVIEW) == ICF_PREVIEW)	m_bPreview = true;
	if ((pGrpInfo->option2 & ICF_REVDRAW) == ICF_REVDRAW)	m_bRevDraw = true;
	if ((pGrpInfo->option2 & ICF_TICKVERTIC) == ICF_TICKVERTIC)	m_bTickVertic = true;

	if ((pGrpInfo->option3 & ICF_ITEMRTS) == ICF_ITEMRTS)	m_bItemRts = true;

	int	gCnt = atoi(CString(pGrpInfo->gcnt, sizeof(pGrpInfo->gcnt)));

	if (sInfo.GetLength() < (int)(sz_GRPINFO + sz_GRPLINE * gCnt))
	{
		TRACE("invalid grapg Config[%d-%d]", sInfo.GetLength(), sz_GRPINFO + sz_GRPLINE * gCnt);
		return;
	}

	struct	_GrpLine GrpLine {};

	for (int ii = 0; ii < gCnt; ii++)
	{
		const	struct _GrpLine	*pGrpLine = (_GrpLine*)&pBuf[sz_GRPINFO + sz_GRPLINE*ii];
		CopyMemory(GrpLine.kind, pGrpLine->kind, sizeof(pGrpLine->kind));
		GrpLine.region = pGrpLine->region - ICF_CHAR;	// -ICF_CHAR
		CopyMemory(GrpLine.color, pGrpLine->color, sizeof(pGrpLine->color));
		GrpLine.tick  = pGrpLine->tick - ICF_CHAR;	// -ICF_CHAR
		GrpLine.width = pGrpLine->width - ICF_CHAR;	// -ICF_CHAR
		GrpLine.style = pGrpLine->style - ICF_CHAR;	// -ICF_CHAR
		GrpLine.option1 = pGrpLine->option1 - ICF_CHAR;	// -ICF_CHAR
		m_arGrpLine.Add(GrpLine);
	}

	if (gCnt == 1)
	{
		index = atoi(CString(m_arGrpLine.GetAt(0).kind, sizeof(m_arGrpLine.GetAt(0).kind)));
		if (index == IGK_2D3NBAR)	
			m_b3NChart = true;
	}

	m_CfgRgnCnt = atoi(CString(pGrpInfo->rcnt, sizeof(pGrpInfo->rcnt)));
	m_CfgRgnInfo = CString(pGrpInfo->rinfo, sizeof(pGrpInfo->rinfo));
	
	int	iTotal = 0;
	for (int ii = 0 ; ii < m_CfgRgnCnt; ii++)
	{
		iTotal += atoi(m_CfgRgnInfo.Mid(ii*2, 2));
	}
	if (iTotal != 100)
		m_CfgRgnInfo = "";
	else
		m_CfgRgnInfo = m_CfgRgnInfo.Left(m_CfgRgnCnt*2);

	// notice!!
	if (m_CfgRgnCnt > 1)
		m_bItemRts = false;
}

int CGrpWnd::RequestHead()
{
/* _UpHead{
	char		dunit[2];	// 종목,선물,옵션..
	char		dindex[2];	// 일,주,월,분,틱
	char		dcount[6];	// data request count
	char		reserve[22];	// reserved};	*/
	CString	sTmp;

	sprintf(m_pHeader, "%0*d", sz_UPHEADER, 0);
	sTmp.Format("%02d%06d", m_dIndex, m_ReqCnt);
	CopyMemory(&m_pHeader[2], sTmp, sTmp.GetLength());

	return (long)m_pHeader;
}

void CGrpWnd::RemoveComponent()
{
	for (int ii = 0; ii < m_RgnCnt; ii++)
		m_pRgnInfo[ii].reset();


	for(int ii = 0; ii < m_GrpCnt; ii++)
		m_pGrpInfo[ii].reset();

	m_pDataInfo.reset();

	m_RgnCnt = 0;
	m_GrpCnt = 0;
	m_sTickUnit[0] = m_sTickUnit[1] = m_sTickUnit[2] = "";

	m_pMinMaxChk.clear();
	m_MinMaxCnt = 0;
	SetError("");
	m_sTipName = "";
	m_sExtraName = "";

	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Enable, FALSE), 0);

}

int CGrpWnd::GetPalette(int index)
{
	return m_pView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

int CGrpWnd::GetTickColor(int color)
{
	return RGB(180,192,203);

	const	int	rColor = GetRValue(color);
	const	int	gColor = GetGValue(color);
	const	int	bColor = GetBValue(color);
	const	int	rRate  = rColor / 2;
	const	int	gRate  = gColor / 2;
	const	int	bRate  = bColor / 2;

	int	r = (rColor + rRate > 255)? 255:rColor + rRate;
	int	g = (gColor + gRate > 255)? 255:gColor + gRate;
	int	b = (bColor + bRate > 255)? 255:bColor + bRate;
	
	if (r == rColor)	r = (r + 150 > 255)? 255: r + 150;
	if (g == gColor)	g = (g + 150 > 255)? 255: g + 150;
	if (b == bColor)	b = (b + 150 > 255)? 255: b + 150;
	
	return RGB(r, g, b);
}

BOOL CGrpWnd::IsPtInGrp()
{
	POINT	pt;
	CRect	rect;

	::GetCursorPos(&pt);
	GetWindowRect(rect);

	const	CWnd	*pWnd = CWnd::WindowFromPoint(pt);
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
		const	int	gap = 0 - m_dispPos;
		m_dispPos += gap;
		m_dispEnd += gap;
	}

	return true;
}

void CGrpWnd::RecalculateMinMax()
{
	for (int ii = 0; ii < m_GrpCnt; ii++)
		m_pGrpInfo[ii]->CalculateMinMax();
}

void CGrpWnd::ReviseTick()
{
	m_pMinMaxChk.clear();
	m_MinMaxCnt = m_RgnCnt;
	for (int ii = 0; ii < m_RgnCnt; ii++)
	{
		auto& item = m_pMinMaxChk.emplace_back(std::make_unique<_NMinMaxChk>());	
		item->region = ii;

		for (int jj = 0, gcnt = 0; jj < 2; jj++)
		{
			gcnt = 0;
			for (int kk = 0; kk < m_GrpCnt; kk++)
			{			
				if (m_pGrpInfo[kk]->GetRgnKey() == ii && m_pGrpInfo[kk]->GetMinMaxIndex() == jj)
					gcnt++;
			}
			if (gcnt > 1)	item->coMinMax[jj] = 1;
		}
	}
}

int CGrpWnd::CetBottomVerticHeight(CDC *pDC)
{
	int	Height = 0, DrawHeight = 0;
	CString	sTmp;
	const	int	dCount = m_pDataInfo->GetDataCount();
	for(int ii = 0; ii < dCount; ii++)
	{
		const auto gVal = m_pDataInfo->GetGraphData(ii);
		sTmp.Format("%s", gVal->index.name.str);
		Height = CGrp_Base::DrawVerticalString(pDC, sTmp, CRect(0, 0, 0, 0), false);
		if (DrawHeight < Height)
			DrawHeight = Height;
	}

	return DrawHeight;
}

_NMinMaxChk* CGrpWnd::GetMinMaxChk(int rKey)
{
	if (rKey < 0 || rKey >= m_RgnCnt)
		return nullptr;
	
	return m_pMinMaxChk.at(rKey).get();
}

int CGrpWnd::GetGrpKindFromConfig(int index)
{
	if (index < 0 || m_arGrpLine.GetSize() <= index)
		return 0;

	const	struct _GrpLine	GrpLine = m_arGrpLine.GetAt(index);
	return atoi(CString(GrpLine.kind, sizeof(GrpLine.kind)));
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

		m_timerID = TIMER_CROSSID;
		SetTimer(m_timerID, TIMER_CROSSGAP, nullptr);
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

	m_timerID = TIMER_CROSSID;
	SetTimer(m_timerID, TIMER_CROSSGAP, nullptr);
}

CString CGrpWnd::GetTipStr(CPoint point)
{
	if (m_GrpCnt < 1)
		return _T("");
/*
	int	rgnIdx = -1;
	for (int ii = 0; ii < m_RgnCnt; ii++)
	{
		if (m_pRgnInfo[ii]->gpRect.PtInRect(point))
		{
			rgnIdx = ii;
			break;
		}
	}
*/
	CString stmp;
	CString dispStr = m_pGrpInfo[0]->GetDisplayPosHead(point);
	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		//if (m_pGrpInfo[ii]->GetRgnKey() == rgnIdx)
		{
			stmp = m_pGrpInfo[ii]->GetDisplayPosData(point);
			if (!stmp.IsEmpty())
			{
				if (!dispStr.IsEmpty())
					dispStr = dispStr + "\n" + stmp;
				else
					dispStr = stmp;
			}	
		}
	}

	return dispStr;
}

bool CGrpWnd::ChangeDisplayShift(int shift)
{
	if (shift > 0)		// right scroll
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

void CGrpWnd::ChangePalette()
{
	m_tRGB = GetPalette(m_tOrgRGB);
	m_pRGB = GetPalette(m_pOrgRGB);

	m_TkTextColor = m_tRGB;
	m_TkLineColor = GetTickColor(m_TkTextColor);
	for (int ii = 0; ii < m_RgnCnt; ii++)
	{
		m_pRgnInfo[ii]->tick[ctkLEFT].lcolor = m_TkLineColor;
		m_pRgnInfo[ii]->tick[ctkRIGHT].lcolor = m_TkLineColor;
		m_pRgnInfo[ii]->tick[ctkBOTTOM].lcolor = m_TkLineColor;
	}

	for (int ii = 0; ii < m_GrpCnt; ii++)
		m_pGrpInfo[ii]->SetBgColor(m_pRGB);

	if (m_pCrossLine)
		m_pCrossLine->SetColor(m_pRGB, CLR_CROSS);

	Invalidate();
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

void CGrpWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_pParent->SendMessage(WM_RBUTTONDOWN, WPARAM(nFlags), MAKELPARAM(point.x, point.y));	
	CWnd::OnRButtonDown(nFlags, point);
}


void CGrpWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Focus, TRUE), 0);
	
	CWnd::OnLButtonDown(nFlags, point);
}
