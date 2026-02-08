#include "stdafx.h"
#include "C_MultiLineEx.h"
#include "GrpWnd.h"
#include "memDC.h"
#include "Grp_Data.h"
#include "Grp_Base.h"
#include "Grp_Line1.h"
#include "ToolTip.h"
#include "CrossLine.h"

#include <math.h>
#include <float.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TIMER_PID	10000
#define	TIMER_PGAP	250
#define	TIMER_ID	10001
#define	TIMER_GAP	1000

CGrpWnd::CGrpWnd(CWnd *pView, CWnd *pParent, struct _param *pInfo)
{
	m_pView = pView;
	m_pParent = pParent;
	m_pToolTip = new CToolTip(m_pView, this);
	m_pCrossLine = new CCrossLine(pView, this);
	
	m_RgnCnt = m_GrpCnt = 0;
	for (int ii = 0; ii < maxCnt; ii++)
	{
		m_pRgnInfo[ii] = nullptr;		
		m_pGrpInfo[ii] = nullptr;
	}

	m_pDataInfo = nullptr;
	m_pMinMaxChk = nullptr;
	m_MinMaxCnt = 0;

	m_dKind = 0;
	m_dIndex = CDI_DAY;
	m_dCount = 100;
	m_dDay = 100;
	m_dUnit = CDU_JONGMOK;
	m_totalDay = 0;

	m_fPoint = pInfo->point;
	m_fName.Format("%s", pInfo->fonts);
	m_tRGB = GetPalette(pInfo->tRGB);
	m_pRGB = GetPalette(pInfo->pRGB);
	m_fRGB = GetPalette(BGIndex);

	m_TickView = CDT_LEFT;
	m_ObjRect = pInfo->rect;	
	m_ltColor = m_tRGB;
	m_rtColor = m_tRGB;
	m_btColor = m_tRGB;

	m_PosData = _T("");
	m_timerID = 0;
		
	m_rgnRate = RATE_1_1;
	ParseParam((char*)pInfo->options.operator LPCTSTR());	
	m_pApp = (CC_MultiLineExApp*)AfxGetApp();

	m_dMin = INT_MAX;
	m_dMax = INT_MIN;	
	m_GrpRect.SetRectEmpty();
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
	CWnd::OnDestroy();
	
	if (m_timerID > 0)
		KillTimer(m_timerID);

	if (m_pToolTip)
	{
		m_pToolTip->DestroyWindow();
		delete m_pToolTip;
		m_pToolTip = nullptr;
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
	bool	bRepeat = false;
	int	nGap = 0;

	struct _extTHx* exTH = (struct _extTHx*)lParam;
	const int dlen = exTH->size;
	char* pData = exTH->data;



	RemoveComponent();

	if (dlen <= sz_CDMULTICODE)
	{
		Invalidate();
		return dlen;
	}

	const struct _cdMulticode*	pcdMulti = (struct _cdMulticode*)pData;
	const int	dCnt = CGrp_Data::CharToInt(pcdMulti->nData, sizeof(pcdMulti->nData));

	m_lcnt = CGrp_Data::CharToInt(pcdMulti->lcnt, sizeof(pcdMulti->lcnt));

	if (dCnt <= 0 || m_lcnt <= 0)
	{
		Invalidate(FALSE);
		return sz_CDMULTICODE;
	}

	if (dCnt >= m_totalDay && m_totalDay != 0)
	{
		bRepeat = true;
		nGap = dCnt - m_totalDay;
	}

	memset(m_UnitInfo, 0x00, sizeof(m_UnitInfo));
	memcpy(m_szDate, pcdMulti->pday, sizeof(pcdMulti->pday));

	memcpy(m_region, pcdMulti->rgn, sizeof(pcdMulti->rgn));
	m_totalDay = dCnt;

	int	dPos = sz_CDMULTICODE;
	int	mgap = CGrp_Data::CharToInt(pcdMulti->mgap, sizeof(pcdMulti->mgap));

	
	CString	tmpstr = _T(""), output = _T("");
	int	ii = 0;


	CMapStringToString	mapRegion;

	mapRegion.RemoveAll();
	int	rcnt[5]{};
	int	nIndex = 0;

	memset(rcnt, 0x00, sizeof(rcnt));

	for ( ii = 0 ; ii < m_lcnt ; ii++ )
	{
		m_UnitInfo[ii].digit = CGrp_Data::CharToInt(pcdMulti->digit[ii], sizeof(pcdMulti->digit[ii]));
		m_UnitInfo[ii].index = ii;
		memcpy(m_UnitInfo[ii].name, pcdMulti->name[ii], sizeof(pcdMulti->name[ii]));
		memcpy(m_UnitInfo[ii].code, pcdMulti->rcod[ii], sizeof(pcdMulti->rcod[ii]));
		memcpy(m_UnitInfo[ii].rsym, pcdMulti->rsym[ii], sizeof(pcdMulti->rsym[ii]));
		m_UnitInfo[ii].jgga = CharToDouble(pcdMulti->jgga[ii], sizeof(pcdMulti->jgga[ii]));
		
		tmpstr.Format("%c", pcdMulti->rgn[ii][0]);

		nIndex = atoi(tmpstr);
		nIndex--;
		m_UnitInfo[ii].rIndex = rcnt[nIndex];
		rcnt[nIndex]++;

		if (mapRegion.Lookup(tmpstr, output))
			continue;
		else
			mapRegion.SetAt(tmpstr, output);
	}

	m_RgnCnt = mapRegion.GetCount();

	for ( ii = 0 ; ii < m_RgnCnt ; ii++ )
	{		
		m_pRgnInfo[ii] = new _RgnInfo;
	}

	struct	_multi_lineD*	mlineD = nullptr;
	const int nFullsize = (sz_CDMULTICODE + (sizeof(mlineD->index) + m_lcnt * sizeof(mlineD->data[0])) * dCnt);
	
	if (dlen != nFullsize)
	{
		Invalidate(FALSE);
		return dlen;
	}

	for ( ii = 0 ; ii < m_RgnCnt ; ii++ )
	{
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

	
	m_pDataInfo = new CGrp_Data(this, m_dKind, m_dUnit, m_dIndex);
	const int	szFrame = m_pDataInfo->AttachGraphData(&pData[dPos], m_totalDay, mgap);

	dPos += szFrame * m_totalDay;

	if (!bRepeat)
	{
		if (m_dDay == 0 || m_dDay > m_totalDay)
			m_dispDay = m_totalDay;
		else
			m_dispDay = m_dDay;

		m_dispPos = m_totalDay - m_dispDay;
		m_dispEnd = m_dispPos + m_dispDay;
	}
	else
	{
		m_dispDay += nGap;
		m_dispEnd += nGap;
	}

	m_pCrossLine = new CCrossLine(m_pView, this, m_pRGB, RGB(180,77,77));

	AssignGraph();
	ReviseTick();
	Resize();
	Invalidate();

	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Enable, TRUE), 0);
	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_UpdateScr, 0), 0);
	return 0;
}

void CGrpWnd::RealTimeData(char* sRTM)
{
	const bool	bShift = m_pDataInfo->UpdateRTM(sRTM);
	bool	bRedraw = false;	

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

bool CGrpWnd::AssignGraph()
{
	int	rKey = 0;
	struct _gInfo	gInfo;
	CString	tmpstr;
	
	for ( int ii = 0 ; ii < m_lcnt ; ii++ )
	{		
		gInfo.dIndex = m_dIndex;
		gInfo.dKey = 0;
		gInfo.dKind = m_dKind;
		gInfo.gKind = 0;
		tmpstr.Format("%c", m_region[ii][0]);
		rKey = atoi(tmpstr) - 1;
		gInfo.rKey = rKey;		
		gInfo.pRGB = RGB(255, 255, 255);
		gInfo.tRGB = RGB(0, 0, 255);

		m_pGrpInfo[m_GrpCnt] = new CGrp_Line1(m_pView, this, (char *)&gInfo, (char*)&m_UnitInfo[ii]);
		m_GrpCnt++;
	}
	
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
/*
	for (int ii = 0; ii < m_MinMaxCnt; ii++)
	{
		m_pMinMaxChk[ii].Max = INT_MIN;
		m_pMinMaxChk[ii].Min = INT_MAX;
		m_pMinMaxChk[ii].tickx = 0;
		m_pMinMaxChk[ii].ticky = 0;
	}

	double	dMin, dMax;
	struct _MinMaxChk	*pMinMaxChk;
	class CGrp_Base	*pBase;

	for (ii = 0; ii < m_GrpCnt; ii++)
	{
		pBase = m_pGrpInfo[ii];
		pMinMaxChk = GetMinMaxChk(pBase->GetRgnKey());
		if (pMinMaxChk && pMinMaxChk->coMinMax)
		{
			if (pBase->CalculateMinMax())
			{
				pBase->GetMinMaxValue(dMin, dMax);
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
*/
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
			CPen	*cPen;
			cPen = m_pApp->GetPen(m_pView, PS_SOLID, 1, RGB(128,128,128));
			CPen	*sPen = pDC->SelectObject(cPen);
			
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
	CRect	rcc;
	GetClientRect(&rcc);
	
	m_ObjRect = rcc;

	CDC *pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(m_pFont);
	const CSize	sz = pDC->GetOutputTextExtent("9,999,999");
	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
	
	double	*pHeight = nullptr;
	if (m_RgnCnt > 0)
	{
		pHeight = new double[m_RgnCnt];

		if (m_RgnCnt == 1)
			pHeight[0] = 1.0;
		else
		{
			switch (m_rgnRate)
			{
			case RATE_1_1:
				pHeight[0] = 0.5;
				pHeight[1] = 0.5;
				break;
			case RATE_2_1:
				pHeight[0] = 2.0 / 3.0;
				pHeight[1] = 1.0 - pHeight[0];
				break;
			case RATE_3_2:
				pHeight[0] = 3.0 / 5.0;
				pHeight[1] = 1.0 - pHeight[0];
				break;
			}
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

	if (m_RgnCnt == 0 || m_GrpCnt == 0)
		return;

	for (int ii = 0; ii < m_RgnCnt; ii++)
			m_pRgnInfo[ii]->tkHeight = sz.cy;

	bottomRC.left = m_GrpRect.left;
	bottomRC.right = m_GrpRect.right;

	int	top = m_GrpRect.top;
	for (int ii = 0; ii < m_RgnCnt; ii++)
	{
		m_pRgnInfo[ii]->tick[ctkLEFT].tkRect = 
			CRect(0, top, m_GrpRect.left, top + int(m_GrpRect.Height()*pHeight[ii]));

		m_pRgnInfo[ii]->tick[ctkRIGHT].tkRect = 
			CRect(m_GrpRect.right, top, 
			m_ObjRect.right, top + int(m_GrpRect.Height()*pHeight[ii]));

		m_pRgnInfo[ii]->tick[ctkBOTTOM].tkRect = bottomRC;
		m_pRgnInfo[ii]->gpRect.top = m_pRgnInfo[ii]->tick[ctkLEFT].tkRect.top;
		m_pRgnInfo[ii]->gpRect.bottom = m_pRgnInfo[ii]->tick[ctkLEFT].tkRect.bottom;
		m_pRgnInfo[ii]->gpRect.left = m_pRgnInfo[ii]->tick[ctkLEFT].tkRect.right;
		m_pRgnInfo[ii]->gpRect.right = m_pRgnInfo[ii]->tick[ctkRIGHT].tkRect.left;

		top = top + int(m_GrpRect.Height()*pHeight[ii]);
	}

	delete pHeight;

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
void CGrpWnd::ParseParam(char *param)
{
	m_sParam.Format("%s", param);
	CString	sParam = m_sParam;
	CString	sTmp, sToken;
	int		nPos = 0;

	// 단위
	sToken = "/n";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + sToken.GetLength());
		nPos = sTmp.Find("/");

		m_sDegree = _T("");

		if (!sTmp.IsEmpty())
		{
			if (nPos >= 0)
				sTmp = sTmp.Left(nPos);			
			
			CString tmpstr;
			sToken = ",";
			sTmp += sToken;
			sTmp.TrimLeft();
			sTmp.TrimRight();
			tmpstr = sTmp;
			
			while (1)
			{
				nPos = tmpstr.Find(sToken + sToken, 0);

				if (nPos < 0)
					break;
				else
					tmpstr.Replace(sToken + sToken, sToken);
			}

			while (!tmpstr.IsEmpty())
			{					
				nPos = tmpstr.Find(sToken, 0);
				sTmp = tmpstr.Left(nPos);
				sTmp.TrimLeft();
				sTmp.TrimRight();
				
				if (!sTmp.IsEmpty())
				{
					CString	sss;
					sss.Format("%s", sTmp);
					sTmp = sss;
				}
				m_sDegree = sTmp;
				tmpstr = tmpstr.Mid(nPos + sToken.GetLength());
			}
		}
	}
	
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
	
	// 영역비율
	sToken = "/p";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
	{
		sTmp = sParam.Mid(nPos + 2);
		const int index = atoi(sTmp);
		switch (index)
		{
		case RATE_1_1:	m_rgnRate = RATE_1_1;	break;
		case RATE_2_1:	m_rgnRate = RATE_2_1;	break;
		case RATE_3_2:	m_rgnRate = RATE_3_2;	break;
		default:	m_rgnRate = RATE_1_1;	break;
		}
	}
	
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
		default:		m_dUnit = CDU_JONGMOK;	break;
		}
	}

	// 좌우 틱을 그릴때 개별로 그릴때 옵션(디폴트는 보정해서 좌우동형이다)
	sToken = "/e";	nPos = sParam.Find(sToken, 0);
	if (nPos >= 0)
		m_bEachTick = true;
	else
		m_bEachTick = false;
}

int CGrpWnd::RequestHead()
{
	m_sHeader.Format("%02d%02d%02d%06d%06d0000000000", m_dUnit, m_dIndex, m_dKind, m_dCount, 0);
	return long(m_sHeader.operator LPCTSTR());
}

void CGrpWnd::RemoveComponent()
{
	m_cs.Lock();

	if (m_RgnCnt)
	{
		for (int ii = 0; ii < m_RgnCnt; ii++)
		{
			if (m_pRgnInfo[ii])
			{
				delete m_pRgnInfo[ii];
				m_pRgnInfo[ii] = nullptr;
			}
		}
	}

	if (m_pDataInfo)
	{
		delete m_pDataInfo;
		m_pDataInfo = nullptr;
	}
	
	if (m_GrpCnt)
	{
		for (int ii = 0; ii < m_GrpCnt; ii++)
		{
			if (m_pGrpInfo[ii])
			{
				delete m_pGrpInfo[ii];
				m_pGrpInfo[ii] = nullptr;
			}
		}
	}

	m_RgnCnt = 0;
	m_GrpCnt = 0;
	m_DataCnt = 0;

	if (m_pMinMaxChk)
	{
		delete m_pMinMaxChk;
		m_pMinMaxChk = nullptr;
	}

	m_MinMaxCnt = 0;

	if (m_pCrossLine)
	{
		delete m_pCrossLine;
		m_pCrossLine = nullptr;
	}

	m_pParent->SendMessage(CM_CTRL, MAKEWPARAM(CTRL_Enable, FALSE), 0);
	m_cs.Unlock();
}

int CGrpWnd::GetPalette(int index)
{
	return m_pView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

double CGrpWnd::CharToDouble(const char* cnvB, int cnvL)
{
	char	rWb[64];
	CopyMemory(rWb, cnvB, cnvL);
	rWb[cnvL] = '\0';

	return atof(rWb);
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

void CGrpWnd::RecalculateMinMax()
{
	for (int ii = 0; ii < m_GrpCnt; ii++)
		m_pGrpInfo[ii]->CalculateMinMax();
}

void CGrpWnd::ReviseTick()
{
	if (m_pMinMaxChk)
	{
		delete[] m_pMinMaxChk;
		m_pMinMaxChk = nullptr;
	}

	m_MinMaxCnt = m_RgnCnt;
	m_pMinMaxChk = new _MinMaxChk[m_MinMaxCnt];
	ZeroMemory(m_pMinMaxChk, sizeof(_MinMaxChk) * m_MinMaxCnt);

	for (int ii = 0; ii < m_RgnCnt; ii++)
	{
		int	grpcnt = 0;
		for (int jj = 0; jj < m_GrpCnt; jj++)
		{			
			if (m_pGrpInfo[jj]->GetRgnKey() == ii)
				grpcnt++;
		}

		m_pMinMaxChk[ii].region = ii;
		if (grpcnt > 1)	m_pMinMaxChk[ii].coMinMax = 1;
	}
}

_MinMaxChk* CGrpWnd::GetMinMaxChk(int rKey)
{
	if (rKey < 0 || rKey >= m_RgnCnt)
		return nullptr;
	
	return &m_pMinMaxChk[rKey];
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
		tmpS = m_pGrpInfo[ii]->GetExcelData(-1, 0);
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
			tmpS = m_pGrpInfo[jj]->GetExcelData(ii, jj);
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