// HogaWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB300200.h"
#include "MapWnd.h"
#include "HogaWnd.h"
#include "SelJumunDlg.h"
#include "DialogConfirm.h"
#include "ConfigDlg.h"
#include "RectWnd.h"
#include <math.h>
#include <process.h>
#include "../../h/MistakeStopOrder.h"

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


#define ID_CTRL_SCROLL	0

#define	SCROLL_WIDTH	16
#define HEAD_HEIGHT	18

#define CELL_HOGA	0
#define CELL_VOL	1
#define CELL_JUMUN	2
#define CELL_RESERVE	3

#define belowCURR	0
#define aboveCURR	1
#define LEFT_START	0
#define COLOR_ARROW	RGB(150, 150, 150)

#define	TMID_PAINT	1000
#define	TMGAP_PAINT	2

#define	TMID_RESERVE	1001
#define TMID_RESERVE_MAX 2001
#define	TMGAP_RESERVE	0

#define	IDC_MDSTOPBTN		0x11
#define	IDC_MDJMBTN		0x12
#define	IDC_MSJMBTN		0x13
#define	IDC_MSSTOPBTN		0x14
/////////////////////////////////////////////////////////////////////////////
// CHogaWnd

/* 예약주문 체크로직을 Event->Callback으로 전환하기 위해 추가함 */
CCriticalSection rsvd_cs;
map<HWND, CHogaWnd*> v_Windows;
//vector<CHogaWnd*> v_Windows;
list< pair< CString, CString > > v_rsvdData;
void CALLBACK CheckReserve(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	rsvd_cs.Lock();

	/* 현재가 변경 히스토리를 과거치부터 추출하여 예약주문여부를 검사한다 */
	/* v_rsvdData.size()>1 인경우 -> 타이머 호출후 현재 함수가 호출되기 전에 새로운 현재가가 전달되는 경우 */
	while(!v_rsvdData.empty()) {
		const pair<CString, CString> &data = v_rsvdData.front();
		map<HWND, CHogaWnd*>::iterator pos = v_Windows.find(hwnd);
		if (pos!=v_Windows.end()) (*pos).second->checkReserveJumun( data.first, data.second );
		v_rsvdData.pop_back();
	}
	::KillTimer(hwnd, idEvent);

	rsvd_cs.Unlock();
}
/* ------------------------------------------------------------ */

CHogaWnd::CHogaWnd(CWnd *pParent, CWnd *pView)
{
	SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
	bAlertClose = false;

	m_pParent = (CMapWnd*)pParent;
	m_pView = pView;
	
	m_pFont = GetAxFont("굴림체", 9, false, 0);
	m_pBFont = GetAxFont("굴림체", 9, false, FW_BOLD);

	m_depth = hgDEPTH10;
	m_fdgb = hgFOLD;

	m_bDragJumun = false;
	m_bDragResv = false;
	m_bTracking = FALSE;
	m_focusMmgb = -1;
	m_focusIndex = -1;
	
	m_posSHGA = D10SHGA;
	m_posHHGA = D10HHGA;
	m_posCHVL = D10CHVL;		
	m_rows = D10CHVL + 1;
	
	m_pTrackingWnd = NULL;
	m_tooltip = NULL;
	m_indexTip = 0;

	m_market = FUTURE;

	m_code = "";

	removeAllJumun();
	removeAllReserve();
	removeAllArrow();

	m_bReal = false;
	m_rsvCode = "";
	m_rsvCurr = "";
//	m_rtsReserve.RemoveAll();

	m_mdStopBtn = NULL;
	m_mdJmBtn = NULL;
	m_msJmBtn = NULL;
	m_msStopBtn = NULL;

	m_rcWnd.SetRectEmpty();
	m_bHold = false;
}

CHogaWnd::~CHogaWnd()
{
	bAlertClose = true;
	
	rsvd_cs.Lock();
	const map<HWND, CHogaWnd*>::iterator pos = v_Windows.find(this->m_hWnd);
	if (pos!=v_Windows.end()) v_Windows.erase(pos);
	rsvd_cs.Unlock();
//	m_rtsReserve.RemoveAll();

	if (m_pTrackingWnd)	m_pTrackingWnd = nullptr;
	if (m_mdStopBtn)	m_mdStopBtn = nullptr;
	if (m_mdJmBtn)		m_mdJmBtn = nullptr;;
	if (m_msJmBtn)		m_msJmBtn = nullptr;;
	if (m_msStopBtn)	m_msStopBtn = nullptr;;
	removeAllJumun();
	removeAllReserve();
	removeAllArrow();

	m_arHoga.RemoveAll();
}


BEGIN_MESSAGE_MAP(CHogaWnd, CWnd)
	//{{AFX_MSG_MAP(CHogaWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_MDSTOPBTN, OnButtonMdStop)
	ON_BN_CLICKED(IDC_MDJMBTN, OnButtonMdJm)
	ON_BN_CLICKED(IDC_MSJMBTN, OnButtonMsJm)
	ON_BN_CLICKED(IDC_MSSTOPBTN, OnButtonMsStop)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


void CHogaWnd::OnButtonMdStop()
{
	onBtn(1, true);
}
void CHogaWnd::OnButtonMdJm()
{
	onBtn(1, false);
}

void CHogaWnd::OnButtonMsJm()
{
	onBtn(2, false);
}

void CHogaWnd::OnButtonMsStop()
{
	onBtn(2, true);
}

/////////////////////////////////////////////////////////////////////////////
// CHogaWnd message handlers

void CHogaWnd::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pbitmapOld = memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(rc, GetIndexColor(COLOR_BODYBACK) );
	
	drawHead(&memDC);

	if (m_fdgb == hgFOLD)
	{
		drawHogaFold(&memDC);
		drawArrow(&memDC);
	}
	else
	{
		drawHogaUnfold(&memDC);
		drawReserve(&memDC);
		drawArrow(&memDC);
	}

	dc.BitBlt(0, 0, rc.Width() - SCROLL_WIDTH, rc.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pbitmapOld);
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CHogaWnd::DrawScreen() 
{
	CClientDC dc(this);
	CDC *pDC = &dc;

	CRect rc;
	GetClientRect(&rc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pbitmapOld = memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(rc, GetIndexColor(COLOR_BODYBACK));
	
	drawHead(&memDC);
	if (m_fdgb == hgFOLD)
	{
		drawHogaFold(&memDC);
		drawArrow(&memDC);
	}
	else
	{
		drawHogaUnfold(&memDC);
		drawReserve(&memDC);
		drawArrow(&memDC);
	}

/*
	CRect rect(0,0,400, 20);
	CString queue;
	static size_t p[20];
	size_t pc = m_alertmsg.size()+1;
	queue.Format("Queue : %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", pc, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15], p[16], p[17], p[18], p[19]);
	for(int n=19; n>0; --n) p[n] = p[n-1];
	p[0] = pc;
	memDC.DrawText(queue, &rect, DT_SINGLELINE|DT_CENTER);
*/	

	pDC->BitBlt(0, 0, rc.Width() - SCROLL_WIDTH, rc.Height()-20, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pbitmapOld);
	bitmap.DeleteObject();
	memDC.DeleteDC();
}


void CHogaWnd::drawHead(CDC *pDC)
{
	CRect cRc, rc;
	GetClientRect(&rc);

	CPen *pPen = GetAxPen(GetIndexColor(COLOR_TABLE), 1, PS_SOLID);
	CPen *pOldPen = pDC->SelectObject(pPen);
	const int bkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->SelectStockObject(NULL_BRUSH);

	const	int cx = LEFT_START, cy = 0;
	if (m_fdgb == hgFOLD)
	{
	
		cRc.SetRect(cx, cy, m_col[0], cy + HEAD_HEIGHT);
		pDC->FillSolidRect(&cRc, GetIndexColor(COLOR_GRIDHEAD));	pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매도주문", cRc, DT_CENTER, 0, COLOR_GRIDHEADTXT);

		cRc.SetRect(m_col[0] - 1, cRc.top, m_col[1], cRc.bottom);
		pDC->FillSolidRect(&cRc, GetIndexColor(COLOR_GRIDHEAD));	pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매도잔량", cRc, DT_CENTER, 0, COLOR_GRIDHEADTXT);

		cRc.SetRect(m_col[2] - 1, cRc.top, m_col[3], cRc.bottom);
		pDC->FillSolidRect(&cRc, GetIndexColor(COLOR_GRIDHEAD));	pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매수잔량", cRc, DT_CENTER, 0, COLOR_GRIDHEADTXT);

		cRc.SetRect(m_col[3] - 1, cRc.top, m_col[4], cRc.bottom);
		pDC->FillSolidRect(&cRc, GetIndexColor(COLOR_GRIDHEAD));	pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매수주문", cRc, DT_CENTER, 0, COLOR_GRIDHEADTXT);
		
		m_rcHeadHoga.SetRect(m_col[1] - 1, cy, m_col[2], cRc.bottom);
		pDC->FillSolidRect(&m_rcHeadHoga, GetIndexColor(COLOR_GRIDHEAD));	pDC->Rectangle(&m_rcHeadHoga);
		DrawSiseText(pDC, "호    가", m_rcHeadHoga, DT_CENTER, 0, COLOR_GRIDHEADTXT);	
		
		DrawColorBox(pDC, CRect(cx, cy, m_col[4], cRc.bottom));
	}
	else
	{
		cRc.SetRect(cx, cy, m_col[0], cy + HEAD_HEIGHT);
		pDC->FillSolidRect(&cRc, GetIndexColor(COLOR_GRIDHEAD));	pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매도예약", cRc, DT_CENTER, 0, COLOR_GRIDHEADTXT);

		cRc.SetRect(m_col[0] - 1, cRc.top, m_col[1], cRc.bottom);
		pDC->FillSolidRect(&cRc, GetIndexColor(COLOR_GRIDHEAD));	pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매도주문", cRc, DT_CENTER, 0, COLOR_GRIDHEADTXT);

		cRc.SetRect(m_col[1] - 1, cRc.top, m_col[2], cRc.bottom);
		pDC->FillSolidRect(&cRc, GetIndexColor(COLOR_GRIDHEAD));	pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매도잔량", cRc, DT_CENTER, 0, COLOR_GRIDHEADTXT);

		cRc.SetRect(m_col[3] - 1, cRc.top, m_col[4], cRc.bottom);
		pDC->FillSolidRect(&cRc, GetIndexColor(COLOR_GRIDHEAD));	pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매수잔량", cRc, DT_CENTER, 0, COLOR_GRIDHEADTXT);

		cRc.SetRect(m_col[4] - 1, cRc.top, m_col[5], cRc.bottom);
		pDC->FillSolidRect(&cRc, GetIndexColor(COLOR_GRIDHEAD));	pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매수주문", cRc, DT_CENTER, 0, COLOR_GRIDHEADTXT);
	
		cRc.SetRect(m_col[5] - 1, cRc.top, m_col[6], cRc.bottom);
		pDC->FillSolidRect(&cRc, GetIndexColor(COLOR_GRIDHEAD));	pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매수예약", cRc, DT_CENTER, 0, COLOR_GRIDHEADTXT);
		
		m_rcHeadHoga.SetRect(m_col[2] - 1, cy, m_col[3], cRc.bottom);
		pDC->FillSolidRect(&m_rcHeadHoga, GetIndexColor(COLOR_GRIDHEAD));	pDC->Rectangle(&m_rcHeadHoga);
		DrawSiseText(pDC, "호  가", m_rcHeadHoga, DT_CENTER, 0, COLOR_GRIDHEADTXT);

		DrawColorBox(pDC, CRect(cx, cy, m_col[6], cRc.bottom));
	}

	pDC->SelectObject(pOldPen);
	pDC->SetBkMode(bkMode);
}

void CHogaWnd::drawHogaFold(CDC *pDC)
{
	SCROLLINFO	info{};
	CString		string, str, tmpS, data, hoga;
	int		index{}, nPos{};
	CRect		rcCurr;
	rcCurr.SetRectEmpty();
	
	CPen *pPen = GetAxPen(GetIndexColor(COLOR_TABLE), 1, PS_SOLID);
	CPen *pOldPen = pDC->SelectObject(pPen);
	const int bkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->SelectStockObject(NULL_BRUSH);
	
	for (int jj = 1; jj <= m_indexTip; jj++)
		m_tooltip->DelTool(this, jj);
	m_indexTip = 0;
	
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	nPos = info.nPos;
	const COLORREF cMDB = GetIndexColor(COLOR_MDB);
	const COLORREF cMSB = GetIndexColor(COLOR_MSB);

	for (int ii = 0; ii < m_rows; ii++)
	{
		if (m_arHoga.GetSize() > 0 && ii != m_posSHGA && ii != m_posSHGA && ii != m_posHHGA)
		{
			index = nPos + ii - 1;
			if (index < m_arHoga.GetSize() && index > -1)
				hoga = m_arHoga.GetAt(index);
			else
				hoga = "";
		}
		else
			hoga = "";
		
		if (ii == m_posCHVL)
		{
			pDC->FillSolidRect(&m_rcMDJumun[ii], cMDB);
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = getHogaToJumun(hoga, 1, m_rcMDJumun[ii], true);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);
			DrawSiseText(pDC, "-" + m_dvol, m_rcDvol[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE);
			
			pDC->Rectangle(&m_rcHoga[ii]);
			DrawSiseText(pDC, m_dscha, m_rcHoga[ii], DT_RIGHT, ST_NORMAL | ST_COLOR | ST_REMOVE | ST_COMMA);
			
			pDC->Rectangle(&m_rcSvol[ii]);
			DrawSiseText(pDC, "+" + m_svol, m_rcSvol[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE);
			
			pDC->FillSolidRect(&m_rcMSJumun[ii], cMSB);
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = getHogaToJumun(hoga, 2, m_rcMSJumun[ii], true);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
		}
		else if (ii == m_posSHGA)
		{
			pDC->FillSolidRect(&m_rcMDJumun[ii], cMDB);
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = getHogaToJumun(m_shga, 1, m_rcMDJumun[ii]);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);
			DrawSiseText(pDC, "상한", m_rcDvol[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcHoga[ii]);
			data = "1 " + m_shga;
			DrawSiseText(pDC, data, m_rcHoga[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_SHGA | ST_DRYL | ST_BOLD);
			
			pDC->Rectangle(&m_rcSvol[ii]);
			pDC->FillSolidRect(&m_rcMSJumun[ii], cMSB);
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = getHogaToJumun(m_shga, 2, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
		}
		else if (ii == m_posHHGA)
		{
			pDC->FillSolidRect(&m_rcMDJumun[ii], cMDB);
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = getHogaToJumun(m_hhga, 1, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);
			pDC->Rectangle(&m_rcHoga[ii]);
			data = "4 " + m_hhga;
			DrawSiseText(pDC, data, m_rcHoga[ii], DT_RIGHT, ST_NORMAL | ST_COLOR | ST_REMOVE | ST_SHGA | ST_DRYL | ST_BOLD);
			
			pDC->Rectangle(&m_rcSvol[ii]);
			DrawSiseText(pDC, "하한", m_rcSvol[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMSJumun[ii], cMSB);
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = getHogaToJumun(m_hhga, 2, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
		}
		else
		{
			pDC->FillSolidRect(&m_rcMDJumun[ii], cMDB);
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = getHogaToJumun(hoga, 1, m_rcMDJumun[ii]);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);
			data = getHogaData(hoga, 1);
			DrawSiseText(pDC, data, m_rcDvol[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE);
			
			double dHoga = atof(hoga);
			dHoga = (dHoga>0) ? dHoga : -dHoga;
			if (dHoga!=0.0F)
			{
				double dKoga = atof(m_koga);
				double dJega = atof(m_jega);
				dJega = dJega>0 ? dJega : -dJega;
				dKoga = dKoga>0 ? dKoga : -dKoga;
				//TRACE("dKoga[%.2f] dJega[%.2f] dHoga[%.2f]\n", dKoga, dJega, dHoga);
				if ( ((dJega<=dHoga)&&(dHoga<=dKoga)) )
					pDC->FillSolidRect(&m_rcHoga[ii], RGB(255,255,180));
			}

			pDC->Rectangle(&m_rcHoga[ii]);
			DrawSiseText(pDC, hoga, m_rcHoga[ii], DT_RIGHT, ST_COLOR | ST_REMOVE | ST_DRYL | ST_BOLD);
			if (str2double(m_curr) == str2double(hoga) && str2double(m_curr) != 0.0)
				rcCurr = m_rcHoga[ii];
			
			pDC->Rectangle(&m_rcSvol[ii]);
			data = getHogaData(hoga, 2);
			DrawSiseText(pDC, data, m_rcSvol[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE);
			
			pDC->FillSolidRect(&m_rcMSJumun[ii], cMSB);
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = getHogaToJumun(hoga, 2, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
		}
	}
	
	// m_posBTN
	pDC->FillSolidRect(&m_rcMDJumun[m_rows], cMDB);
	pDC->Rectangle(&m_rcMDJumun[m_rows]);
	pDC->Rectangle(&m_rcDvol[m_rows]);
	pDC->Rectangle(&m_rcHoga[m_rows]);
	pDC->Rectangle(&m_rcSvol[m_rows]);
	pDC->FillSolidRect(&m_rcMSJumun[m_rows], cMSB);
	pDC->Rectangle(&m_rcMSJumun[m_rows]);
	
	DrawColorBox(pDC, CRect(m_rcMDJumun[0].left, m_rcMDJumun[0].top, m_rcMSJumun[m_posCHVL+1].right, m_rcMSJumun[m_posCHVL+1].bottom));
	
	pDC->SelectObject(GetAxPen(RGB(0,128,0), 2, PS_SOLID));
	pDC->Rectangle(rcCurr);
	
	pDC->SelectObject(pOldPen);
	pDC->SetBkMode(bkMode);		
}

void CHogaWnd::drawHogaUnfold(CDC *pDC)
{
	SCROLLINFO	info{};
	CString		string, str, tmpS, data, hoga;
	int		index{}, nPos{};
	CRect		rcCurr;
	rcCurr.SetRectEmpty();
	
	CPen *pPen = GetAxPen(GetIndexColor(COLOR_TABLE), 1, PS_SOLID);
	CPen *pOldPen = pDC->SelectObject(pPen);
	const int bkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->SelectStockObject(NULL_BRUSH);

	for (int jj = 1; jj <= m_indexTip; jj++)
		m_tooltip->DelTool(this, jj);
	m_indexTip = 0;
	
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	nPos = info.nPos;

	for (int ii = 0; ii < m_rows; ii++)
	{
		if (m_arHoga.GetSize() > 0 && ii != m_posSHGA && ii != m_posSHGA && ii != m_posHHGA)
		{
			index = nPos + ii - 1;
			if (index < m_arHoga.GetSize() && index > -1)
				hoga = m_arHoga.GetAt(index);
			else
				hoga = "";
		}
		else
			hoga = "";
		if (ii == m_posCHVL)
		{
			pDC->FillSolidRect(&m_rcMDResv[ii], GetIndexColor(COLOR_MDB));
			pDC->Rectangle(&m_rcMDResv[ii]);
			data = getReserveData(hoga, 1, m_rcMDResv[ii], true);
			DrawSiseText(pDC, data, m_rcMDResv[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMDJumun[ii], GetIndexColor(COLOR_MDB));
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = getHogaToJumun(hoga, 1, m_rcMDJumun[ii], true);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);
			DrawSiseText(pDC, "-" + m_dvol, m_rcDvol[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE);
			
			pDC->Rectangle(&m_rcHoga[ii]);
			DrawSiseText(pDC, m_dscha, m_rcHoga[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE);
			
			pDC->Rectangle(&m_rcSvol[ii]);
			DrawSiseText(pDC, "+" + m_svol, m_rcSvol[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE);
			
			pDC->FillSolidRect(&m_rcMSJumun[ii], GetIndexColor(COLOR_MSB));
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = getHogaToJumun(hoga, 2, m_rcMSJumun[ii], true);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMSResv[ii], GetIndexColor(COLOR_MSB));
			pDC->Rectangle(&m_rcMSResv[ii]);
			data = getReserveData(hoga, 2, m_rcMSResv[ii], true);
			DrawSiseText(pDC, data, m_rcMSResv[ii], DT_RIGHT);
			
		}
		else if (ii == m_posSHGA)
		{
			pDC->FillSolidRect(&m_rcMDResv[ii], GetIndexColor(COLOR_MDB));
			pDC->Rectangle(&m_rcMDResv[ii]);
			
			pDC->FillSolidRect(&m_rcMDJumun[ii], GetIndexColor(COLOR_MDB));
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = getHogaToJumun(m_shga, 1, m_rcMDJumun[ii]);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);
			DrawSiseText(pDC, "상한", m_rcDvol[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcHoga[ii]);
			data = "1 " + m_shga;
			DrawSiseText(pDC, data, m_rcHoga[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_SHGA | ST_BOLD);
			
			pDC->Rectangle(&m_rcSvol[ii]);
			pDC->FillSolidRect(&m_rcMSJumun[ii], GetIndexColor(COLOR_MSB));
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = getHogaToJumun(m_shga, 2, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMSResv[ii], GetIndexColor(COLOR_MSB));
			pDC->Rectangle(&m_rcMSResv[ii]);
			
		}
		else if (ii == m_posHHGA)
		{
			pDC->FillSolidRect(&m_rcMDResv[ii], GetIndexColor(COLOR_MDB));
			pDC->Rectangle(&m_rcMDResv[ii]);
			
			pDC->FillSolidRect(&m_rcMDJumun[ii], GetIndexColor(COLOR_MDB));
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = getHogaToJumun(m_hhga, 1, m_rcMDJumun[ii]);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);
			pDC->Rectangle(&m_rcHoga[ii]);
			data = "4 " + m_hhga;
			DrawSiseText(pDC, data, m_rcHoga[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_SHGA | ST_BOLD);
			
			pDC->Rectangle(&m_rcSvol[ii]);
			DrawSiseText(pDC, "하한", m_rcSvol[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMSJumun[ii], GetIndexColor(COLOR_MSB));
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = getHogaToJumun(m_hhga, 2, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMSResv[ii], GetIndexColor(COLOR_MSB));
			pDC->Rectangle(&m_rcMSResv[ii]);
	
		}
		else
		{
			pDC->FillSolidRect(&m_rcMDResv[ii], GetIndexColor(COLOR_MDB));
			pDC->Rectangle(&m_rcMDResv[ii]);
			data = getReserveData(hoga, 1, m_rcMDResv[ii]);
			DrawSiseText(pDC, data, m_rcMDResv[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMDJumun[ii], GetIndexColor(COLOR_MDB));
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = getHogaToJumun(hoga, 1, m_rcMDJumun[ii]);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);
			data = getHogaData(hoga, 1);
			DrawSiseText(pDC, data, m_rcDvol[ii], DT_RIGHT, ST_COMMA | ST_COLOR | ST_REMOVE);
			
			double dHoga = atof(hoga);
			dHoga = (dHoga>0) ? dHoga : -dHoga;
			if (dHoga!=0.0F)
			{
				double dKoga = atof(m_koga);
				double dJega = atof(m_jega);
				dJega = dJega>0 ? dJega : -dJega;
				dKoga = dKoga>0 ? dKoga : -dKoga;
				TRACE("dKoga[%.2f] dJega[%.2f] dHoga[%.2f]\n", dKoga, dJega, dHoga);
				if ( ((dJega<=dHoga)&&(dHoga<=dKoga)) )
					pDC->FillSolidRect(&m_rcHoga[ii], RGB(255,255,180));
			}
			pDC->Rectangle(&m_rcHoga[ii]);

			DrawSiseText(pDC, hoga, m_rcHoga[ii], DT_RIGHT, ST_COMMA | ST_COLOR | ST_REMOVE | ST_BOLD);
			if (str2double(m_curr) == str2double(hoga) && str2double(m_curr) != 0.0)
				rcCurr = m_rcHoga[ii];
			
			pDC->Rectangle(&m_rcSvol[ii]);
			data = getHogaData(hoga, 2);
			DrawSiseText(pDC, data, m_rcSvol[ii], DT_RIGHT, ST_COMMA | ST_COLOR | ST_REMOVE);
			
			pDC->FillSolidRect(&m_rcMSJumun[ii], GetIndexColor(COLOR_MSB));
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = getHogaToJumun(hoga, 2, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMSResv[ii], GetIndexColor(COLOR_MSB));
			pDC->Rectangle(&m_rcMSResv[ii]);
			data = getReserveData(hoga, 2, m_rcMSResv[ii]);
			DrawSiseText(pDC, data, m_rcMSResv[ii], DT_RIGHT);
			
		}
	}	

	pDC->FillSolidRect(&m_rcMDResv[m_rows], GetIndexColor(COLOR_MDB));
	pDC->Rectangle(&m_rcMDResv[m_rows]);
	pDC->FillSolidRect(&m_rcMDJumun[m_rows], GetIndexColor(COLOR_MDB));
	pDC->Rectangle(&m_rcMDJumun[m_rows]);
	pDC->Rectangle(&m_rcDvol[m_rows]);
	pDC->Rectangle(&m_rcHoga[m_rows]);
	pDC->Rectangle(&m_rcSvol[m_rows]);
	pDC->FillSolidRect(&m_rcMSJumun[m_rows], GetIndexColor(COLOR_MSB));
	pDC->Rectangle(&m_rcMSJumun[m_rows]);
	pDC->FillSolidRect(&m_rcMSResv[m_rows], GetIndexColor(COLOR_MSB));
	pDC->Rectangle(&m_rcMSResv[m_rows]);

	DrawColorBox(pDC, CRect(m_rcMDResv[0].left, m_rcMDResv[0].top, m_rcMSResv[m_posCHVL+1].right, m_rcMSResv[m_posCHVL+1].bottom));

	pDC->SelectObject(GetAxPen(RGB(0,128,0), 2, PS_SOLID));
	pDC->Rectangle(rcCurr);

	pDC->SelectObject(pOldPen);
	pDC->SetBkMode(bkMode);

}

void CHogaWnd::drawReserve(CDC *pDC)
{
	CRgn rgn;
	rgn.CreateRectRgn(m_rcMDResv[1].left, m_rcMDResv[1].top, m_rcMSResv[20].right, m_rcMSResv[20].bottom);
	pDC->SelectClipRgn(&rgn);

	CPoint		ptStart, ptEnd;
	class CReserve *pResv = NULL;

	for (int ii = 0; ii < m_arReserve.GetSize(); ii++)
	{
		pResv = m_arReserve.GetAt(ii);
		getPointReserve(pResv, ptStart, ptEnd);

		if(ptStart.y == ptEnd.y)
		{
			DrawStar(pDC,ptStart,4);
		}
		else
		{
			drawArrowLine(pDC, ptStart, ptEnd);
		}
	}
	
	pDC->SelectClipRgn(NULL);
}

void CHogaWnd::drawArrow(CDC *pDC)
{
	CRgn rgn;
	rgn.CreateRectRgn(m_rcMDJumun[1].left, m_rcMDJumun[1].top, m_rcMSJumun[20].right, m_rcMSJumun[20].bottom);
	pDC->SelectClipRgn(&rgn);
	
	CPoint	ptStart, ptEnd;
	class CArrow *pArrow = NULL;

	for (int ii = 0; ii < m_arArrow.GetSize(); ii++)
	{
		pArrow = m_arArrow.GetAt(ii);
		getPointArrow(pArrow, ptStart, ptEnd);
// 		CString s;
// 		s.Format("PTSTART : [%d]	PTEND : [%d]\n",ptStart.y,ptEnd.y);
// 		OutputDebugString(s);
		if(ptStart.y == ptEnd.y)
		{
			DrawStar(pDC,ptStart,pArrow->jflg);
		}
		else
		{
			drawArrowLine3(pDC, ptStart, ptEnd, pArrow->jflg);
		}
	}
	
	pDC->SelectClipRgn(NULL);
}

void CHogaWnd::getPointArrow(class CArrow* pArrow, CPoint &ptStart, CPoint &ptEnd)
{
	double hoga{};
	SCROLLINFO	info{};
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);

	CString price, gprc, jprc;
	int gap{}, gIndex{}, jIndex{};
	const int index = info.nPos;
	if (index < m_arHoga.GetSize() && index > -1)
		hoga = str2double(m_arHoga.GetAt(index));

	for (int ii = 0; ii < m_arHoga.GetSize(); ii++)
	{
		price = m_arHoga.GetAt(ii);	price.Remove('+');	price.Remove('-');
		gprc.Format("%.2f", pArrow->oprc);
		jprc.Format("%.2f", pArrow->mprc);
		if (gprc == price)
			gIndex = ii;
		if (jprc == price)
			jIndex = ii;
	}

	CRect rcStart, rcEnd, cRc;
	if (pArrow->jflg == 4)
	{
		jIndex = gIndex;

		if (pArrow->mmgb == MD_JUMUN)
		{
			gap = gIndex - index;
			cRc = m_rcMDJumun[1];
			rcStart.SetRect(cRc.left, cRc.top + (int)(m_height * gap) - 1, cRc.right, cRc.top + (int)(m_height * (gap + 1)));
			ptStart = rcStart.CenterPoint();	ptStart.x = cRc.right - 10;
			ptEnd = ptStart;			ptEnd.x = cRc.left + 10;
		}
		else
		{
			gap = gIndex - index;
			cRc = m_rcMSJumun[1];
			rcStart.SetRect(cRc.left, cRc.top + (int)(m_height * gap) - 1, cRc.right, cRc.top + (int)(m_height * (gap + 1)));
			ptStart = rcStart.CenterPoint();	ptStart.x = cRc.left + 10;
			ptEnd = ptStart;			ptEnd.x = cRc.right - 10;
		}
	}
	else
	{
		if (pArrow->mmgb == MD_JUMUN)
		{
			gap = gIndex - index;
			cRc = m_rcMDJumun[1];
			rcStart.SetRect(cRc.left, cRc.top + (int)(m_height * gap) - 1, cRc.right, cRc.top + (int)(m_height * (gap + 1)));
			ptStart = rcStart.CenterPoint();	ptStart.x = cRc.left + 10;	
			
			gap = jIndex - index;
			cRc = m_rcMDJumun[1];
			rcEnd.SetRect(cRc.left, cRc.top + (int)(m_height * gap) - 1, cRc.right, cRc.top + (int)(m_height * (gap + 1)));
			ptEnd = rcEnd.CenterPoint();		ptEnd.x = cRc.left + 10;
		}
		else
		{
			gap = gIndex - index;
			cRc = m_rcMSJumun[1];
			rcStart.SetRect(cRc.left, cRc.top + (int)(m_height * gap) - 1, cRc.right, cRc.top + (int)(m_height * (gap + 1)));
			ptStart = rcStart.CenterPoint();	ptStart.x = cRc.left + 10;
			
			gap = jIndex - index;
			cRc = m_rcMSJumun[1];
			rcEnd.SetRect(cRc.left, cRc.top + (int)(m_height * gap) - 1, cRc.right, cRc.top + (int)(m_height * (gap + 1)));
			ptEnd = rcEnd.CenterPoint();		ptEnd.x = cRc.left + 10;
		}
	}
}

void CHogaWnd::getPointReserve(class CReserve* pResv, CPoint &ptStart, CPoint &ptEnd)
{
	double hoga{};
	SCROLLINFO	info{};
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);

	CString price, gprc, jprc;
	int gap{}, gIndex{}, jIndex{};
	const int index = info.nPos;
	if (index < m_arHoga.GetSize())
		hoga = str2double(m_arHoga.GetAt(index));
	gIndex = pResv->index-1;
	
	if (pResv->mmgb == MS_JUMUN) jIndex = pResv->index - pResv->tick -1;
	else jIndex = pResv->index + pResv->tick -1;

	CRect rcStart, rcEnd, cRc;
	
	if (pResv->mmgb == MD_JUMUN)
	{
		gap = gIndex - index;
		cRc = m_rcMDResv[1];
		rcStart.SetRect(cRc.left, cRc.top + (int)(m_height * gap) - 1, cRc.right, cRc.top + (int)(m_height * (gap + 1)));
		ptStart = rcStart.CenterPoint();	ptStart.x = cRc.left + 5;	

		gap = jIndex - index;
		cRc = m_rcMDJumun[1];
		rcEnd.SetRect(cRc.left, cRc.top + (int)(m_height * gap) - 1, cRc.right, cRc.top + (int)(m_height * (gap + 1)));
		ptEnd = rcEnd.CenterPoint();		ptEnd.x = cRc.left + 10;
	}
	else
	{
		gap = gIndex - index;
		cRc = m_rcMSResv[1];
		rcStart.SetRect(cRc.left, cRc.top + (int)(m_height * gap) - 1, cRc.right, cRc.top + (int)(m_height * (gap + 1)));
		ptStart = rcStart.CenterPoint();	ptStart.x = cRc.left + 5;

		gap = jIndex - index;
		cRc = m_rcMSJumun[1];
		rcEnd.SetRect(cRc.left, cRc.top + (int)(m_height * gap) - 1, cRc.right, cRc.top + (int)(m_height * (gap + 1)));
		ptEnd = rcEnd.CenterPoint();		ptEnd.x = cRc.left + 5;
	}
}

void CHogaWnd::DispatchData(char* pData, int len)
{
	if (len != sz_SMOD)	return;
	if (m_code != m_pParent->GetCode())
	{
		removeAllJumun();
		removeAllReserve();
		removeAllArrow();
		
		m_arHoga.RemoveAll();
	}
	m_code = m_pParent->GetCode();
	
	setHogaRect();

	CString tmpS;
	if (!m_code.IsEmpty() && m_code.GetAt(0) == '1')
		m_market = FUTURE;
	else
		m_market = OPTION;

	struct _smod* pMod = (struct _smod *)pData;

	for (int ii = 0; ii < 5; ii++)
	{
		m_hoga[ii].mdga = CString(pMod->hoga[ii].mdga, sizeof(pMod->hoga[ii].mdga));
		m_hoga[ii].msga = CString(pMod->hoga[ii].msga, sizeof(pMod->hoga[ii].msga));
		m_hoga[ii].dvol = CString(pMod->hoga[ii].dvol, sizeof(pMod->hoga[ii].dvol));
		m_hoga[ii].svol = CString(pMod->hoga[ii].svol, sizeof(pMod->hoga[ii].svol));
		m_hoga[ii].dcnt = CString(pMod->hoga[ii].dcnt, sizeof(pMod->hoga[ii].dcnt));
		m_hoga[ii].scnt = CString(pMod->hoga[ii].scnt, sizeof(pMod->hoga[ii].scnt));
	}

	m_dvol = CString(pMod->dvol, sizeof(pMod->dvol));
	m_svol = CString(pMod->svol, sizeof(pMod->svol));
	m_dcnt = CString(pMod->dcnt, sizeof(pMod->scnt));
	m_scnt = CString(pMod->scnt, sizeof(pMod->scnt));
	m_dscha = CString(pMod->dscha, sizeof(pMod->dscha));

	m_curr = CString(pMod->curr, sizeof(pMod->curr));
	m_diff = CString(pMod->diff, sizeof(pMod->diff));	m_diff.TrimLeft();	m_diff.TrimRight();
	if (!m_diff.IsEmpty() && m_diff[0] == '+')
		m_jijg.Format("%.2f", str2double(m_curr) - str2double(m_diff.Mid(1)));
	else if (!m_diff.IsEmpty())
		m_jijg.Format("%.2f", str2double(m_curr) + str2double(m_diff.Mid(1)));

	m_siga = CString(pMod->siga, sizeof(pMod->siga));
	m_koga = CString(pMod->koga, sizeof(pMod->koga));
	m_jega = CString(pMod->jega, sizeof(pMod->jega));
	m_koga.TrimLeft();
	m_koga.TrimRight();
	m_jega.TrimLeft();
	m_jega.TrimRight();
	TRACE("DATA : KOGA[%s], JEGA[%s]\n", (LPCSTR)m_koga, (LPCSTR)m_jega);

	m_shga = CString(pMod->shga, sizeof(pMod->shga));
	m_hhga = CString(pMod->hhga, sizeof(pMod->hhga));
	
	removeAllJumun();

	createHogaSet();

	invalidateRC();	
}

void CHogaWnd::DispatchMicheg(char *pData, int len, bool bErase)
{
	if (bErase)
	{
		removeAllJumun();
		removeAllArrow();
	}
	
	struct _cmod *pMod = (struct _cmod *)pData;
	int nrec = atoi(CString(pMod->nrec, sizeof(pMod->nrec)));

	for (int ii = 0; ii < nrec; ii++)
	{
		// 미체결내역을 전부 받으므로, 필터링 로직 추가. 2009.08.19
		CString code(pMod->rec[ii].cod2, sizeof(pMod->rec[ii].cod2));
		if (code!=m_code) continue;

		gsl::owner<class COrder*> pOrder = new class COrder;
		pOrder->code = m_code;
		pOrder->juno = CString(pMod->rec[ii].juno, sizeof(pMod->rec[ii].juno));
		pOrder->mmgb = CString(pMod->rec[ii].odgb, sizeof(pMod->rec[ii].odgb));
		if (pOrder->mmgb.Find("매도") >= 0)
			pOrder->mmgb = "1";
		else
			pOrder->mmgb = "2";
		pOrder->jprc = CString(pMod->rec[ii].jprc, sizeof(pMod->rec[ii].jprc));		pOrder->jprc.TrimLeft();	pOrder->jprc.TrimRight();
		pOrder->jqty = CString(pMod->rec[ii].jqty, sizeof(pMod->rec[ii].jqty));
		pOrder->mcvl = CString(pMod->rec[ii].wqty, sizeof(pMod->rec[ii].wqty));

		const int juno = str2int(pOrder->juno);
		m_mapOrder.SetAt(juno, pOrder);		
	}

	m_bReal = true;
	Invalidate();
}


void CHogaWnd::DispatchAlertX(struct _alertR* alertR)
{
	if (alertR->size <= 0) return;

	CString tmpS, val, code = alertR->code;

	if (m_pParent->GetCode() != code) return;

	CString	dvol; // 매도호가총수량
	CString svol; // 매수호가총수량
	CString pdvl; // 매도호가총건수
	CString psvl; // 매수호가총건수

	bool bCurrChg = false;
	class CHoga hoga[10];

	DWORD* data = (DWORD *)alertR->ptr[0];

	if (data[023])			// 현재가
	{
		val = (char*)data[23];

		if (m_curr != val)
		{
			bCurrChg = true;
			m_curr = val;
		}
	}
	if (data[24])		// 전일대비
		m_diff = (char *)data[24];
	if (data[29])
		m_siga = (char*)data[29];
	if (data[30])
		m_koga = (char *)data[30];
	if (data[31])
		m_jega = (char *)data[31];
	if (data[51])
		m_hoga[0].mdga = (char *)data[51];
	if (data[52])
		m_hoga[1].mdga = (char *)data[52];
	if (data[53])
		m_hoga[2].mdga = (char *)data[53];
	if (data[54])
		m_hoga[3].mdga = (char *)data[54];
	if (data[55])
		m_hoga[4].mdga = (char *)data[55];
	if (data[71])
		m_hoga[0].msga = (char *)data[71];
	if (data[72])
		m_hoga[1].msga = (char *)data[72];
	if (data[73])
		m_hoga[2].msga = (char *)data[73];
	if (data[74])
		m_hoga[3].msga = (char *)data[74];
	if (data[75])
		m_hoga[4].msga = (char *)data[75];
	if (data[41])
		m_hoga[0].dvol = (char *)data[41];
	if (data[42])
		m_hoga[1].dvol = (char *)data[42];
	if (data[43])
		m_hoga[2].dvol = (char *)data[43];
	if (data[44])
		m_hoga[3].dvol = (char *)data[44];
	if (data[45])
		m_hoga[4].dvol = (char *)data[45];
	if (data[61])
		m_hoga[0].svol = (char *)data[61];
	if (data[62])
		m_hoga[1].svol = (char *)data[62];
	if (data[63])
		m_hoga[2].svol = (char *)data[63];
	if (data[64])
		m_hoga[3].svol = (char *)data[64];
	if (data[65])
		m_hoga[4].svol = (char *)data[65];
	if (data[211])
		m_hoga[0].dcnt = (char *)data[111];
	if (data[212])
		m_hoga[1].dcnt = (char *)data[212];
	if (data[213])
		m_hoga[2].dcnt = (char *)data[213];
	if (data[214])
		m_hoga[3].dcnt = (char *)data[214];
	if (data[215])
		m_hoga[4].dcnt = (char *)data[215];
	if (data[221])
		m_hoga[0].scnt = (char *)data[221];
	if (data[222])
		m_hoga[1].scnt = (char *)data[222];
	if (data[223])
		m_hoga[2].scnt = (char *)data[223];
	if (data[224])
		m_hoga[3].scnt = (char *)data[224];
	if (data[225])
		m_hoga[4].scnt = (char *)data[225];
	if (data[101])
		m_dvol = (char *)data[101];
	if (data[103])
		m_dcnt = (char *)data[103];
	if (data[106])
		m_svol = (char *)data[106];
	if (data[108])
		m_scnt = (char *)data[108];

	m_dscha.Format("%.f", str2double(m_svol) - str2double(m_dvol));
	if (str2double(m_dscha) > 0)	m_dscha = "+" + m_dscha;

	if (bCurrChg)
	{
		//TRACE("ALERT : KOGA[%s], JEGA[%s]\n", (LPCSTR)m_koga, (LPCSTR)m_jega);
		if (m_bHold)
			setScroll(m_depth);
		m_rsvCode = m_pParent->GetCode();
		m_rsvCurr = m_curr;

		if (getConfirmJumun())
		{
			rsvd_cs.Lock();
			v_rsvdData.push_back(pair<CString, CString>(m_rsvCode, m_rsvCurr));
			::SetTimer(m_hWnd, TMID_RESERVE, TMGAP_RESERVE, CheckReserve);
			rsvd_cs.Unlock();
		}
		else
		{
			checkReserveJumun(m_rsvCode, m_rsvCurr);
		}

		if (!bCurrChg) m_bReal = true;
	}

	Invalidate();
}


void CHogaWnd::DispatchNotice(CString dat)
{
	CMapStringToString	ary;
	NoticeParse(dat, ary);

	CString		str, value, gubn;
	CString		acc, code;
	int		juno = 0;
	gsl::owner<class COrder*> pOrder{};

	ary.Lookup("907", code);	code.TrimLeft();	code.TrimRight();
	ary.Lookup("901", acc);		acc.TrimRight();

	if (m_pParent->GetCode() != code)
		return;

	if (m_pParent->GetAccount() != acc)
		return;

	//m_cs.Lock();
	ary.Lookup("996", gubn);
	switch (atoi(gubn))
	{
	case  2:    /* 정정확인     */
		if (ary.Lookup("905", value))
			juno = str2int(value);
		else
		{
			//m_cs.Unlock();
			return;
		}
		break;
	case  3:    /* 취소확인     */
		if (ary.Lookup("905", value))	// 905:원주문
		{
			juno = str2int(value);
			
			if (!m_mapOrder.Lookup(juno, pOrder))		// 원주문이 없으면 IOC/FOK
			{
				if (ary.Lookup("904", value))
					juno = str2int(value);
				else
				{
					//m_cs.Unlock();
					return;
				}
			}
		}
		else
		{
			//m_cs.Unlock();
			return;
		}
		break;
	case  1:    /* 체결*/
	case 21:    /* 접수         */
	case 22:    /* 정정접수     */
	case 24:    /* 기타접수     */
	case  4:    /* 거부         */
	case 31:    /* 접수오류     */
	case 32:    /* 정정접수오류 */
	case 33:    /* 취소접수오류 */
	case 34:    /* 기타접수오류 */
	default:
		if (ary.Lookup("904", value))
			juno = str2int(value);
		else
		{
			//m_cs.Unlock();
			return;
		}
		break;
	case 23:    /* 취소접수     */
		{
			//m_cs.Unlock();
			return;
		}
		break;
	}
#if 0
CString s988, s909, s921, s931, s905;
ary.Lookup("988", s988);
ary.Lookup("909", s909);
ary.Lookup("921", s921);
ary.Lookup("931", s931);
ary.Lookup("905", s905);
TRACE("ojno = %s, gubn = %s, 909 = %s, 921 = %s, 931 = %s\n", s905, s988, s909, s921, s931);
#endif
	if (m_mapOrder.Lookup(juno, pOrder))
	{
		if (ary.Lookup("909", value))
			pOrder->jqty = value;
		
		switch (atoi(gubn))
		{
		case 2:
		case 3:
			if (ary.Lookup("931", value))
				pOrder->mcvl.Format("%d", atoi(pOrder->mcvl) - atoi(value));
			break;
		default:
			if (ary.Lookup("921", value))
				pOrder->mcvl = value;
			break;
		}
	}
	else
	{
		pOrder = new class COrder;

		pOrder->code = m_pParent->GetCode();	// 종목코드
		
		if (ary.Lookup("904", value))		// 주문번호
			pOrder->juno = value;

		if (ary.Lookup("912", value))		// 매매구분
		{
			if (value.Find("매수") >= 0)
				pOrder->mmgb = "2";
			else
				pOrder->mmgb = "1";
		}

		if (ary.Lookup("910", value))
			pOrder->jprc.Format("%.2f", atoi(value) / 100.0);		// 주문가격

		if (ary.Lookup("909", value))		// 주문수량
			pOrder->jqty = value;

		if (ary.Lookup("921", value))		// 미체결 수량
			pOrder->mcvl = value;

		m_mapOrder.SetAt(juno, pOrder);
	}

	if (str2int(pOrder->mcvl) == 0)
		removeJumun(juno);

	//m_cs.Unlock();

	invalidateRC();
	//m_bReal = true;
}

void CHogaWnd::NoticeParse(CString dat, CMapStringToString& ary)
{
	int	pos{}, pos2{};
	CString	str, symbol, value;

	pos = dat.Find('\n');
	if (pos != -1)
	{
		str = dat.Left(pos++);
		dat = dat.Mid(pos);
	}

	while (!str.IsEmpty())
	{
		pos = str.Find('\t');
		if (pos == -1)	break;

		symbol = str.Left(pos++);
		str    = str.Mid(pos);

		pos = str.Find('\t');
		if (pos == -1)
		{
			value = str;
			str.Empty();
		}
		else
		{
			pos2 = pos;
			value = str.Left(pos2++);
			pos = value.Find('\r');
			if (pos != -1)
			{
				value = value.Left(pos++);
				str   = str.Mid(pos);
			}
			else
				str = str.Mid(pos2);
		}

		value.TrimRight();
		ary.SetAt(symbol, value);
	}

	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;

		symbol = dat.Left(pos++);
		dat    = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat   = dat.Mid(pos);
		}

		value.TrimRight();
		ary.SetAt(symbol, value);
	}
}

void CHogaWnd::DispatchJango(CString jango)
{

}

void CHogaWnd::createHogaSet()
{
	m_arHoga.RemoveAll();

	CString hoga, tmpS;
	double price = str2double(m_shga);
	hoga.Format("+%.2f", str2double(m_shga));
	int cnt=0;
	
	if (m_depth == hgDEPTH10)		cnt = 10;
	else					cnt = 5;	

	for (int ii = 0; ii < cnt; ii++)
	{
		m_arHoga.Add("  ");
	}

	for (int ii = 0; ; ii++)
	{
		m_arHoga.Add(hoga);

		if (price <= str2double(m_hhga))
			break;

		price = getPriceUnit(price, m_market == FUTURE ? true : false, false);
		CString sPrice;
		sPrice.Format("%.2f", price);
		if (str2double(sPrice) > str2double(m_jijg))
			hoga.Format("+%.2f", price);
		else if (str2double(sPrice) < str2double (m_jijg))
			hoga.Format("-%.2f", price);
		else
			hoga.Format("%.2f", price);
	}
	
	for (int ii = 0; ii < cnt; ii++)
	{
		m_arHoga.Add("  ");
	}

	setScroll(m_depth);
}

double CHogaWnd::getPriceUnit(double price, bool bFuture, bool bPlus)
{
	CString tmpS;

	//2015.07.21 KSJ mini Kospi200 일때
	if(m_code.Mid(1, 2) == "05")
	{
		if (bPlus)
		{
			if (bFuture)	// KOSPI 주식
			{
				price += 0.02;
			}
			else		// KOSDAQ 주식
			{
				//2016.01.05 KSJ 미니옵션 호가단위 변경
				//3P미만 0.01 3P이상 0.02, 10P이상 0.05

				if (price >= 10.0)
					price += (double)0.05;
				else if (price >= 3.0)
					price += (double)0.02;
				else
					price += (double)0.01;
			}
			tmpS.Format("%.2f", price);
			return atof(tmpS);
		}
		else
		{
			if (bFuture)	// KOSPI 주식
			{
				price -= 0.02;
			}
			else		// KOSDAQ 주식
			{
				//2016.01.05 KSJ 미니옵션 호가단위 변경
				//3P미만 0.01 3P이상 0.02, 10P이상 0.05
				
				if (price > 10.0)
					price -= (double)0.05;
				else if (price > 3.0)
					price -= (double)0.02;
				else
					price -= (double)0.01;
			}
			tmpS.Format("%.2f", price);
			return atof(tmpS);
		}
		
		return price - 0.02;
	}
	else	//기존
	{
		if (bPlus)
		{
			if (bFuture)	// KOSPI 주식
			{
				price += 0.05;
			}
			else		// KOSDAQ 주식
			{
				if (price >= 10.0)
					price += (double)0.05;
				else
					price += (double)0.01;
			}
			tmpS.Format("%.2f", price);
			return atof(tmpS);
		}
		else
		{
			if (bFuture)	// KOSPI 주식
			{
				price -= 0.05;
			}
			else		// KOSDAQ 주식
			{
				if (price > 10.0)
					price -= 0.05;
				else
					price -= 0.01;
			}
			tmpS.Format("%.2f", price);
			return atof(tmpS);
		}
		
		return price - 0.05;
	}
}

double CHogaWnd::getTickUnit(double price, bool bFuture, int mmgb, int count)
{
	if (count == 0)
		count = m_pCtrlWnd->GetTickCount();

	bool bPlus=false;
	if (mmgb == MS_JUMUN)
	{
		if (count > 0)		bPlus = true;
		else			bPlus = false;
	}
	else
	{
		if (count > 0)		bPlus = false;
		else			bPlus = true;
	}

	for (int ii = 0; ii < abs(count); ii++)
	{
		price = getPriceUnit(price, m_market == FUTURE ? true : false, bPlus);
	}
	return price;
}

double CHogaWnd::str2double(CString string, bool bABS /*true*/)
{
	if (string.IsEmpty())
		return 0.0;

	string.Remove(',');
	
	if (bABS)
		return fabs(atof(string));
	else
		return atof(string);
}

int CHogaWnd::str2int(CString string, bool bABS /*true*/)
{
	if (string.IsEmpty())
		return 0;

	string.Remove(',');
	
	if (bABS)
		return abs(atoi(string));
	else
		return atoi(string);
}

void CHogaWnd::DrawSiseText(CDC* pDC, CString data, CRect rc, UINT nFormat, UINT nType, int clrIndex)
{
	rc.DeflateRect(5, 0);
	rc.OffsetRect(0, 1);
	nFormat = DT_SINGLELINE | DT_VCENTER | nFormat;
	COLORREF color{}, clrText = GetIndexColor(clrIndex);
	int	pos = -1;
	CString hoga, value, giho = "", percent = "";
	hoga = data;

	if (data.IsEmpty())	return;

	if (data.GetAt(0) == 'p')
	{
		data = data.Mid(1);
		clrText = GetIndexColor(COLOR_TEXT);
		nType = ST_COMMA;
	}

	CFont* pOldFont{};
	if (nType & ST_BOLD)
		pOldFont = pDC->SelectObject(m_pBFont);
	else
		pOldFont = pDC->SelectObject(m_pFont);

	if (nType & ST_TEXT)
	{
		pDC->DrawText(data, rc, nFormat);
		return;
	}

	if (nType & ST_SHGA)
	{
		if (data[0] == '1')
		{
			giho = "↑ ";
			clrText = GetIndexColor(COLOR_PLUSTXT);
		}
		else if (data[0] == '2')
		{
			giho = "▲ ";
			clrText = GetIndexColor(COLOR_PLUSTXT);
		}
		else if (data[0] == '4')
		{
			giho = "↓ ";
			clrText = GetIndexColor(COLOR_MINUSTXT);
		}
		else if (data[0] == '5')
		{
			giho = "▼ ";	
			clrText = GetIndexColor(COLOR_MINUSTXT);
		}

		data = data.Mid(1);
	}

	if (nType & ST_COLOR)
	{
		pos = data.Find('+');
		if (pos >= 0)
			clrText = GetIndexColor(COLOR_PLUSTXT);

		pos = data.Find('-');
		if (pos >= 0)
			clrText = GetIndexColor(COLOR_MINUSTXT);
	}

	if (nType & ST_PERCENT)
		percent = " %";

	if (nType & ST_REMOVE)
	{
		data.Remove('+');
		data.Remove('-');
	}

	if (nType & ST_NORMAL)
	{
		CString pre = _T("");
		pos = data.Find('+');
		if (pos >= 0)
		{
			pre = "+";
			data.Remove('+');
		}

		pos = data.Find('-');
		if (pos >= 0)
		{
			pre = "-";
			data.Remove('-');
		}
		data.Remove(' ');

		pos = data.Find('.');
		if (pos >= 0)
			value.Format("%.2f", atof(data));
		else
			value.Format("%d", atoi(data));

		if (atof(data) == 0.0)
			data = "";
		else
			data = pre + value;
	}

	if (nType & ST_COMMA)
	{
		data = SetComma(data);	
	}

	if (nType & ST_DRYL)
	{
		CString dryl;
		dryl = calculatePercent(data);
		data += " " + dryl;
	}

	color = pDC->SetTextColor(clrText);
	pDC->DrawText(giho + data + percent, rc, nFormat);

	pDC->SelectObject(pOldFont);
	pDC->SetTextColor(color);
}

CPen* CHogaWnd::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pView->SendMessage(WM_USER, getPEN, (long)&penR);
}

CBrush* CHogaWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pView->SendMessage(WM_USER, getBRUSH, (long)clr);
}

CFont* CHogaWnd::GetAxFont(CString fName, int point, bool bItalic, int nBold)
{
	struct _fontR fontR;
	fontR.name   = (LPSTR)fName.operator LPCTSTR();
	fontR.point  = point;
	fontR.italic = bItalic;
	fontR.bold   = nBold;
	return (CFont*)m_pView->SendMessage(WM_USER, getFONT, (long)&fontR);
}

COLORREF CHogaWnd::GetIndexColor(int index)
{
	return m_pView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CString CHogaWnd::SetComma(CString src)
{
	CString temp, floatStr;
	int	ii{}, jj = -1;

	src.TrimLeft();
	if (src.Find('.') >= 0)
	{
		floatStr = src.Mid(src.Find('.'));
		src = src.Left(src.Find('.'));
	}
	else
		floatStr.Empty();
	
	temp = src;
	src.Empty();
	
	for (ii = temp.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (temp.GetAt(0) != '-' && temp.GetAt(0) != '+')
				src = ',' + src;
			else if (ii != 0)
				src = ',' + src;
			
			jj = 0;
		}
		src = temp.GetAt(ii) + src;
	}
	src += floatStr;
	
	return src;
}

void CHogaWnd::setHogaRect()
{
	CRect rc;
	GetClientRect(&rc);
	rc.bottom -= HEAD_HEIGHT;

	m_rcWnd = rc;

	int cx = LEFT_START, cy = 0;

	if (m_fdgb == hgFOLD)
	{
		m_width = (rc.Width() - SCROLL_WIDTH - cx) / 11;
		for (int ii = 0; ii < 5; ii++)
		{
			if (ii == 4)
				m_col[ii] = rc.Width() - SCROLL_WIDTH - cx;
			else if (ii == 3)
				m_col[ii] = (int)(m_width * 9);
			else if (ii == 2)
				m_col[ii] = (int)(m_width * 7);
			else if (ii == 1)
				m_col[ii] = (int)(m_width * 4);
			else if (ii == 0)
				m_col[ii] = (int)(m_width * 2);
		}
	}
	else
	{
		m_width = ((rc.Width() - SCROLL_WIDTH - cx) / MAX_COL);

		for (int ii = 0; ii < MAX_COL; ii++)
		{
			if (ii == MAX_COL - 1)
				m_col[ii] = rc.Width() - SCROLL_WIDTH - cx;
			else 
				m_col[ii] = (int)(m_width * (ii + 1));
		}
	}

//	cx = 2;	cy = HEAD_HEIGHT * 2 - 1;
	cx = LEFT_START;	cy = HEAD_HEIGHT - 1;
	m_width = (rc.Width() - SCROLL_WIDTH) / 5;
	m_height = (rc.Height() - cy) / (m_rows * 1.0);

	for (int ii = 0; ii < MAX_ROW; ii++)
	{
		m_rcHoga[ii].SetRectEmpty();
		m_rcDvol[ii].SetRectEmpty();
		m_rcSvol[ii].SetRectEmpty();
		m_rcMDJumun[ii].SetRectEmpty();
		m_rcMSJumun[ii].SetRectEmpty();
		m_rcMDResv[ii].SetRectEmpty();
		m_rcMSResv[ii].SetRectEmpty();
	}

	if (m_fdgb == hgFOLD)
	{
		for (int ii = 0; ii < m_rows; ii++)
		{
			if (ii == m_posSHGA)
			{
				m_rcMDJumun[ii].SetRect(cx, cy, m_col[0], cy + (int)(m_height * (ii + 1)));
				m_rcDvol[ii].SetRect(m_col[0] - 1, cy, m_col[1], cy + (int)(m_height * (ii + 1)));
				m_rcHoga[ii].SetRect(m_col[1] - 1, cy, m_col[2], cy + (int)(m_height * (ii + 1)));
				m_rcSvol[ii].SetRect(m_col[2] - 1, cy, m_col[3], cy + (int)(m_height * (ii + 1)));
				m_rcMSJumun[ii].SetRect(m_col[3] - 1, cy, m_col[4], cy + (int)(m_height * (ii + 1)));
			}
			else if (ii == m_posCHVL)
			{
				m_rcMDJumun[ii].SetRect(cx, cy + (int)(m_height * ii) - 1, m_col[0], rc.bottom);
				m_rcDvol[ii].SetRect(m_col[0] - 1, cy + (int)(m_height * ii) - 1, m_col[1], rc.bottom);
				m_rcHoga[ii].SetRect(m_col[1] - 1, cy + (int)(m_height * ii) - 1, m_col[2], rc.bottom);
				m_rcSvol[ii].SetRect(m_col[2] - 1, cy + (int)(m_height * ii) - 1, m_col[3], rc.bottom);
				m_rcMSJumun[ii].SetRect(m_col[3] - 1, cy + (int)(m_height * ii) - 1, m_col[4], rc.bottom);
			}
			else
			{
				m_rcMDJumun[ii].SetRect(cx, cy + (int)(m_height * ii) - 1, m_col[0], cy + (int)(m_height * (ii + 1)));
				m_rcDvol[ii].SetRect(m_col[0] - 1, cy + (int)(m_height * ii) - 1, m_col[1], cy + (int)(m_height * (ii + 1)));
				m_rcHoga[ii].SetRect(m_col[1] - 1, cy + (int)(m_height * ii) - 1, m_col[2], cy + (int)(m_height * (ii + 1)));
				m_rcSvol[ii].SetRect(m_col[2] - 1, cy + (int)(m_height * ii) - 1, m_col[3], cy + (int)(m_height * (ii + 1)));
				m_rcMSJumun[ii].SetRect(m_col[3] - 1, cy + (int)(m_height * ii) - 1, m_col[4], cy + (int)(m_height * (ii + 1)));
			}
		}
		
		m_rcMDJumun[m_rows].SetRect(cx, rc.bottom, m_col[0], rc.bottom+(int)m_height);
		m_rcDvol[m_rows].SetRect(m_col[0] - 1, rc.bottom, m_col[1], rc.bottom+(int)m_height);
		m_rcHoga[m_rows].SetRect(m_col[1] - 1, rc.bottom, m_col[2], rc.bottom+(int)m_height);
		m_rcSvol[m_rows].SetRect(m_col[2] - 1, rc.bottom, m_col[3], rc.bottom+(int)m_height);
		m_rcMSJumun[m_rows].SetRect(m_col[3] - 1, rc.bottom, m_col[4], rc.bottom+(int)m_height);
			
	}
	else
	{
		int ii = 0;
		for (ii = 0; ii < m_rows; ii++)
		{
			if (ii == m_posSHGA)
			{
				m_rcMDResv[ii].SetRect(cx, cy, m_col[0], cy + (int)(m_height * (ii + 1)));
				m_rcMDJumun[ii].SetRect(m_col[0] - 1, cy, m_col[1], cy + (int)(m_height * (ii + 1)));
				m_rcDvol[ii].SetRect(m_col[1] - 1, cy, m_col[2], cy + (int)(m_height * (ii + 1)));
				m_rcHoga[ii].SetRect(m_col[2] - 1, cy, m_col[3], cy + (int)(m_height * (ii + 1)));
				m_rcSvol[ii].SetRect(m_col[3] - 1, cy, m_col[4], cy + (int)(m_height * (ii + 1)));
				m_rcMSJumun[ii].SetRect(m_col[4] - 1, cy, m_col[5], cy + (int)(m_height * (ii + 1)));
				m_rcMSResv[ii].SetRect(m_col[5] - 1, cy, m_col[6], cy + (int)(m_height * (ii + 1)));
			}
			else if (ii == m_posCHVL)
			{
				m_rcMDResv[ii].SetRect(cx, cy + (int)(m_height * ii) - 1, m_col[0], rc.bottom);
				m_rcMDJumun[ii].SetRect(m_col[0] - 1, cy + (int)(m_height * ii) - 1, m_col[1], rc.bottom);
				m_rcDvol[ii].SetRect(m_col[1] - 1, cy + (int)(m_height * ii) - 1, m_col[2], rc.bottom);
				m_rcHoga[ii].SetRect(m_col[2] - 1, cy + (int)(m_height * ii) - 1, m_col[3], rc.bottom);
				m_rcSvol[ii].SetRect(m_col[3] - 1, cy + (int)(m_height * ii) - 1, m_col[4], rc.bottom);
				m_rcMSJumun[ii].SetRect(m_col[4] - 1, cy + (int)(m_height * ii) - 1, m_col[5], rc.bottom);
				m_rcMSResv[ii].SetRect(m_col[5] - 1, cy + (int)(m_height * ii) - 1, m_col[6], rc.bottom);
			}
			else
			{
				m_rcMDResv[ii].SetRect(cx, cy + (int)(m_height * ii) - 1, m_col[0], cy + (int)(m_height * (ii + 1)));
				m_rcMDJumun[ii].SetRect(m_col[0] - 1, cy + (int)(m_height * ii) - 1, m_col[1], cy + (int)(m_height * (ii + 1)));
				m_rcDvol[ii].SetRect(m_col[1] - 1, cy + (int)(m_height * ii) - 1, m_col[2], cy + (int)(m_height * (ii + 1)));
				m_rcHoga[ii].SetRect(m_col[2] - 1, cy + (int)(m_height * ii) - 1, m_col[3], cy + (int)(m_height * (ii + 1)));
				m_rcSvol[ii].SetRect(m_col[3] - 1, cy + (int)(m_height * ii) - 1, m_col[4], cy + (int)(m_height * (ii + 1)));
				m_rcMSJumun[ii].SetRect(m_col[4] - 1, cy + (int)(m_height * ii) - 1, m_col[5], cy + (int)(m_height * (ii + 1)));
				m_rcMSResv[ii].SetRect(m_col[5] - 1, cy + (int)(m_height * ii) - 1, m_col[6], cy + (int)(m_height * (ii + 1)));
			}
		}
		m_rcMDResv[ii].SetRect(cx, rc.bottom, m_col[0], rc.bottom+(int)m_height);
		m_rcMDJumun[ii].SetRect(m_col[0] - 1, rc.bottom, m_col[1], rc.bottom+(int)m_height);
		m_rcDvol[ii].SetRect(m_col[1] - 1, rc.bottom, m_col[2], rc.bottom+(int)m_height);
		m_rcHoga[ii].SetRect(m_col[2] - 1, rc.bottom, m_col[3], rc.bottom+(int)m_height);
		m_rcSvol[ii].SetRect(m_col[3] - 1, rc.bottom, m_col[4], rc.bottom+(int)m_height);
		m_rcMSJumun[ii].SetRect(m_col[4] - 1, rc.bottom, m_col[5], rc.bottom+(int)m_height);
		m_rcMSResv[ii].SetRect(m_col[5] - 1, rc.bottom, m_col[6], rc.bottom+(int)m_height);
			
	}

	CRect btnRC;
	if (m_mdStopBtn)
	{
		btnRC = m_rcMDResv[m_rows];
		btnRC.DeflateRect(1, 1, 1, 1);
		m_mdStopBtn->MoveWindow(btnRC);
	}
	if (m_mdJmBtn)
	{
		btnRC = m_rcMDJumun[m_rows];
		btnRC.DeflateRect(1, 1, 1, 1);
		m_mdJmBtn->MoveWindow(btnRC);
	}
	if (m_msJmBtn)
	{
		btnRC = m_rcMSJumun[m_rows];
		btnRC.DeflateRect(1, 1, 1, 1);
		m_msJmBtn->MoveWindow(btnRC);
	}
	if (m_msStopBtn)
	{
		btnRC = m_rcMSResv[m_rows];
		btnRC.DeflateRect(1, 1, 1, 1);
		m_msStopBtn->MoveWindow(btnRC);
	}
}

int CHogaWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	/* 예약주문 체크로직을 Event->Callback으로 전환하기 위해 추가함 */
	rsvd_cs.Lock();
	v_Windows[this->m_hWnd] = this;
	rsvd_cs.Unlock();
	/* ------------------------------------------------------------ */

	m_mdStopBtn = std::make_unique<CExButton>();
	m_mdStopBtn->Create("예약취소", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_MDSTOPBTN);
	m_mdStopBtn->Init(bsTEXT|bsCOLOR|bsPUSH, "예약취소", "", RGB(197, 230, 188), RGB(0, 0, 0), RGB(197, 230, 188));

	m_mdJmBtn = std::make_unique<CExButton>();
	m_mdJmBtn->Create("매도취소", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_MDJMBTN);
	m_mdJmBtn->Init(bsTEXT|bsCOLOR|bsPUSH, "매도취소", "", RGB(132, 193, 255), RGB(0, 0, 0), RGB(132, 193, 255));
	

	m_msJmBtn = std::make_unique<CExButton>();
	m_msJmBtn->Create("매수취소", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_MSJMBTN);
	m_msJmBtn->Init(bsTEXT|bsCOLOR|bsPUSH, "매수취소", "", RGB(255, 172, 214), RGB(0, 0, 0), RGB(255, 172, 214));

	m_msStopBtn = std::make_unique <CExButton>();
	m_msStopBtn->Create("예약취소", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_MSSTOPBTN);
	m_msStopBtn->Init(bsTEXT|bsCOLOR|bsPUSH, "예약취소", "", RGB(197, 230, 188), RGB(0, 0, 0), RGB(197, 230, 188));
	
	return 0;
}

void CHogaWnd::Init(CControlWnd *pCtrlWnd)
{
	m_pCtrlWnd = pCtrlWnd;

	m_home = m_pParent->Variant(homeCC, "");
	m_user = m_pParent->Variant(nameCC, "");

	m_file.Format("%s\\%s\\%s\\IB300200.ini", m_home, USRDIR, m_user);

	CConfigDlg	dlg(m_file);
	dlg.InitData();
	m_config = dlg.m_config;

	if (!m_tooltip)
	{
		m_tooltip = std::make_unique <CToolTipCtrl>();
		m_tooltip->Create(GetDesktopWindow(), TTS_ALWAYSTIP);
		m_tooltip->SetMaxTipWidth(1000);
		m_tooltip->Activate(TRUE);
	}

	m_pTrackingWnd = std::make_unique <CRectWnd>(this);
	m_pTrackingWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0);

	createScroll();

	setHogaRect();
	//SetTimer(TMID_PAINT, TMGAP_PAINT, NULL);
}

void CHogaWnd::createScroll()
{
	CRect rc;
	GetClientRect(&rc);
		
	m_vbar.Create(WS_CHILD|WS_VISIBLE|SBS_VERT|SBS_RIGHTALIGN, 
		CRect(rc.right - SCROLL_WIDTH, rc.top, rc.right - 1, rc.bottom), this, ID_CTRL_SCROLL);
}

void CHogaWnd::setScroll(int nHoga)
{
	CRect rc;
	GetClientRect(&rc);

	//m_cs.Lock();
	int nPos = getKey(str2double(m_curr));
	//m_cs.Unlock();

	if (nPos < 0)	nPos = 0;
	
	SCROLLINFO	info{};
	if (nHoga == hgDEPTH10)
	{
		info.cbSize	= sizeof(SCROLLINFO);
		info.fMask	= SIF_ALL | SIF_DISABLENOSCROLL; 
		info.nMin	= 0;
		info.nMax	= m_arHoga.GetSize() - 1;
		info.nPage	= 20;
		info.nPos	= nPos;
		info.nTrackPos	= 0;
	}
	else if (nHoga == hgDEPTH5)
	{
		info.cbSize	= sizeof(SCROLLINFO);
		info.fMask	= SIF_ALL | SIF_DISABLENOSCROLL; 
		info.nMin	= 0;
		info.nMax	= m_arHoga.GetSize() - 1;
		info.nPage	= 10;
		info.nPos	= nPos;
		info.nTrackPos	= 0;
	}
	
	m_vbar.SetScrollInfo(&info);
	m_vbar.MoveWindow(rc.right - SCROLL_WIDTH, rc.top, SCROLL_WIDTH, rc.Height());

	invalidateRC();
}

int CHogaWnd::getKey(double value)
{
	double	hoga = 0.;
	for (int ii = 0; ii < m_arHoga.GetSize(); ii++)
	{
		hoga = str2double(m_arHoga.GetAt(ii));
		if (hoga == value)
			return ii - (m_depth == hgDEPTH10 ? 10 : 5);
	}

	return -1;
}

void CHogaWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();

	class COrder *pOrder = NULL;
	class CReserve *pResv = NULL;

	if (m_rcHeadHoga.PtInRect(point))
	{
		setScroll(m_depth);	
		CWnd::OnLButtonDown(nFlags, point);
		return;
	}
	
// 일반주문
	m_cs.Lock();

	int juno{}, mmgb{}, index = getPointToIndex(point, CELL_JUMUN, mmgb);
	if (index >= m_posSHGA && index <= m_posHHGA)
	{
		SCROLLINFO	info;
		m_vbar.GetScrollInfo(&info, SIF_ALL);
		const int nPos = info.nPos;

		const double hoga = getIndexToHoga(index);
		
	//	m_cs.Lock();
		POSITION pos = m_mapOrder.GetStartPosition();
		while( pos != NULL )
		{
			m_mapOrder.GetNextAssoc(pos, juno, pOrder);

			if (hoga == str2double(pOrder->jprc) && mmgb == str2int(pOrder->mmgb))
			{
				m_bDragJumun = true;
				m_hogaStart = hoga;
				m_ptStart = point;
				m_mmgb = mmgb;

				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
				SetCapture();

				break;
			}
		}
	//	m_cs.Unlock();
	}
	else if (index == m_posCHVL)
	{
	//	m_cs.Lock();
		POSITION pos = m_mapOrder.GetStartPosition();
		while( pos != NULL )
		{
			m_mapOrder.GetNextAssoc(pos, juno, pOrder);
			
			if (mmgb == str2int(pOrder->mmgb))
			{
				m_bDragJumun = true;
				m_hogaStart = 0;
				m_ptStart = point;
				m_mmgb = mmgb;
				
				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
				SetCapture();
				
				break;
			}
		}
	//	m_cs.Unlock();
	}
// 예약매매 
	index = getPointToIndex(point, CELL_RESERVE, mmgb);

	//m_cs.Lock();
	if (index > m_posSHGA && index < m_posHHGA)
	{
		SCROLLINFO	info;
		m_vbar.GetScrollInfo(&info, SIF_ALL);
		const int nPos = info.nPos;

		const double hoga = getIndexToHoga(index);
		
		for (int ii = 0; ii < m_arReserve.GetSize(); ii++)
		{
			pResv = m_arReserve.GetAt(ii);
			if (hoga == pResv->gprc && pResv->mmgb == mmgb)
			{
				m_bDragResv = true;
				m_hogaStart = hoga;
				m_ptStart = point;
				m_mmgb = mmgb;

				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
				SetCapture();
			}
		}
	}
	else if (index == m_posCHVL)
	{
		for (int ii = 0; ii < m_arReserve.GetSize(); ii++)
		{
			pResv = m_arReserve.GetAt(ii);
			if (pResv->mmgb == mmgb)
			{
				m_bDragResv = true;
				m_hogaStart = 0;
				m_ptStart = point;
				m_mmgb = mmgb;

				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
				SetCapture();
			}
		}
	}
	//m_cs.Unlock();
	
	if (!m_bDragResv && !m_bDragJumun)
	{
		index = getPointToIndex(point, CELL_JUMUN, mmgb);
		if (index >= m_posSHGA && index <= m_posHHGA)
		{
			m_hogaStart = getIndexToHoga(index);
			m_ptStart = point;
			m_mmgb = mmgb;
		}

		index = getPointToIndex(point, CELL_RESERVE, mmgb);
		if (index >= m_posSHGA && index <= m_posHHGA)
		{
			m_hogaStart = getIndexToHoga(index);
			m_ptStart = point;
			m_mmgb = mmgb;
		}
	}
	m_cs.Unlock();
	CWnd::OnLButtonDown(nFlags, point);
}

void CHogaWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect sRc, rc;
	GetClientRect(&rc);
	sRc = rc;
	ClientToScreen(&sRc);

	CString price, priceFrom, priceTo, strOne, junox, junos = "";
	m_bOneClick = m_pCtrlWnd->GetOneClick();
	m_bClickMod = m_pCtrlWnd->GetClickMod();

	if (m_bClickMod)
	{
		ClickMod(point);
	}
	else if (m_bOneClick)
	{
		OneClickJumun(point);
	}
	
	if (m_bDragJumun)
	{
		m_bDragJumun = false;

		ReleaseCapture();
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		if (!checkJumunAvailable(3))
			return;

		if (rc.PtInRect(point))		// 정정
		{
			int juno{}, mmgb{};
			const int index = getPointToIndex(point, CELL_JUMUN, mmgb);
			class COrder *pOrder = NULL;

			const double hoga = getIndexToHoga(index);
			priceFrom.Format("%.2f", m_hogaStart);	
			priceFrom = SetComma(priceFrom);
			priceTo.Format("%.2f", hoga);
			priceTo = SetComma(priceTo);
			if (m_hogaStart == 0)
				price.Format("%s", priceTo);
			else
				price.Format("%s -> %s", priceFrom, priceTo);
			
			if (index >= 0 && m_mmgb == mmgb && hoga != m_hogaStart)
			{
				if (hoga == 0.0)	
				{
					m_pParent->SetGuide("주문 가능한 가격이 아닙니다.");
					return;
				}

				CSelJumunDlg dlg(this);
				CPoint	pt = point;
				ClientToScreen(&pt);
				dlg.Init(pt);

				int count = 0;
				m_cs.Lock();
				POSITION pos = m_mapOrder.GetStartPosition();
				while( pos != NULL )
				{
					m_mapOrder.GetNextAssoc(pos, juno, pOrder);
					
					if (m_hogaStart == 0 && mmgb == str2int(pOrder->mmgb))
					{
						dlg.SetList(pOrder->juno, pOrder->mcvl);
						junos += pOrder->juno + "\t";
						count++;
						strOne = pOrder->juno;
					}
					else if (m_hogaStart == str2double(pOrder->jprc) && mmgb == str2int(pOrder->mmgb))
					{
						dlg.SetList(pOrder->juno, pOrder->mcvl);
						junos += pOrder->juno + "\t";
						count++;
						strOne = pOrder->juno;
					}
				}

				strOne.TrimLeft();
				strOne.TrimRight();
				strOne.Format("%05d", atoi((LPCTSTR)strOne));

				/*
				if (count > 1)
					strOne.Format("%s 외 %d개", strOne, count - 1);
				*/
				if (count > 1) {
					CString tmp = strOne;
					tmp.Format("%s 외 %d개", (LPCTSTR)tmp, count - 1);
					strOne = tmp;
				}
				m_cs.Unlock();

				if (m_config.mod == 0 || count == 1)
				{
					//2014.06.23 KSJ 주문착오방지 수정
					MistakeOrder misOdr;
					CString strJmgb;
					
					misOdr.strCode = m_pParent->GetCode();
					misOdr.lPrice = hoga;
					misOdr.lQuantity = getJumunCount(junos, mmgb);
					misOdr.nMmgb = mmgb;
					strJmgb.Format("%c%c", m_pCtrlWnd->GetJmgb().GetAt(0), '0');
					misOdr.strHogb = strJmgb;
					misOdr.strCurPrc = m_curr;
					misOdr.strStdPrc = m_pivot;
					
					const bool bRet = CheckupMistakeOrder(this, misOdr);
					
					if (!bRet)
						return;
					//END KSJ

					if (getConfirmJumun())
					{
						CDialogConfirm cdlg(sRc, m_pView, m_pParent);
 
						cdlg.SetData('F', m_pParent->GetAccount(), m_pParent->GetName(), 
							m_pParent->GetCodeName(), getJumunCount(junos, mmgb), price, NULL, dlg.GetJunoString());
						if (cdlg.DoModal() != IDOK)
							return;
					}
					while (!junos.IsEmpty())
					{
						junox = m_pParent->Parser(junos, "\t");
						m_pParent->SendJumun("3", hoga, str2int(junox), 0);
						SetModArrow(3, hoga, str2int(junox));
					}
				}
				else if (dlg.DoModal() == IDOK)
				{
					if (!dlg.GetJumunCount())	return;
					junos = dlg.GetJuno();

					//2014.06.23 KSJ 주문착오방지 수정
					MistakeOrder misOdr;
					CString strJmgb;
					
					misOdr.strCode = m_pParent->GetCode();
					misOdr.lPrice = hoga;
					misOdr.lQuantity = getJumunCount(junos, mmgb);
					misOdr.nMmgb = mmgb;
					strJmgb.Format("%c%c", m_pCtrlWnd->GetJmgb().GetAt(0), '0');
					misOdr.strHogb = strJmgb;
					misOdr.strCurPrc = m_curr;
					misOdr.strStdPrc = m_pivot;
					
					const bool bRet = CheckupMistakeOrder(this, misOdr);
					
					if (!bRet)
						return;
					//END KSJ

					if (getConfirmJumun())
					{
						CDialogConfirm cdlg(sRc, m_pView, m_pParent);
						cdlg.SetData('F', m_pParent->GetAccount(), m_pParent->GetName(), 
							m_pParent->GetCodeName(), getJumunCount(junos, mmgb), price, NULL, strOne);
						if (cdlg.DoModal() != IDOK)
							return;
					}

					while (!junos.IsEmpty()) 
					{
						junox = m_pParent->Parser(junos, "\t");
						m_pParent->SendJumun("3", hoga, str2int(junox), 0);
						SetModArrow(3, hoga, str2int(junox));
					}
				}
			}

		}
		else				// 취소
		{
			int juno = 0, mmgb = 0, jmcn = 0, count = 0;
			const int index = getPointToIndex(m_ptStart, CELL_JUMUN, mmgb);
			class COrder *pOrder = NULL;

			const double hoga = getIndexToHoga(index);

			m_cs.Lock();
			POSITION pos = m_mapOrder.GetStartPosition();
			while( pos != NULL )
			{
				m_mapOrder.GetNextAssoc(pos, juno, pOrder);
				
				if (m_hogaStart == 0 && mmgb == str2int(pOrder->mmgb))
				{
					jmcn += str2int(pOrder->mcvl);
					count++;
					strOne = pOrder->juno;
				}
				else if (m_hogaStart == str2double(pOrder->jprc) && mmgb == str2int(pOrder->mmgb))
				{
					jmcn += str2int(pOrder->mcvl);
					count++;
					strOne = pOrder->juno;
				}
			}

			strOne.TrimLeft();
			strOne.TrimRight();
			strOne.Format("%05d", atoi((LPCTSTR)strOne));

			if (count > 1) {
				CString tmp = strOne;
				tmp.Format("%s 외 %d개", (LPCTSTR)tmp, count - 1);
				strOne = tmp;
				//strOne.Format("%s 외 %d개", (LPCTSTR)strOne, count - 1);
			}

			if (getConfirmJumun())
			{
				CDialogConfirm dlg(sRc, m_pView, m_pParent);
				dlg.SetData('C', m_pParent->GetAccount(), m_pParent->GetName(), 
					m_pParent->GetCodeName(), jmcn, "", NULL, strOne);
				if (dlg.DoModal() != IDOK)
				{
					m_cs.Unlock();
					return;
				}
			}

			pos = m_mapOrder.GetStartPosition();
			while( pos != NULL )
			{
				m_mapOrder.GetNextAssoc(pos, juno, pOrder);

// 	@@Zeta  조건 수정, 취소수량은 왜 0으로 보냈었지?
				
				if (mmgb == str2int(pOrder->mmgb) && 
					(m_hogaStart == 0 || m_hogaStart == str2double(pOrder->jprc)))
				{
					m_pParent->SendJumun("4", hoga, str2int(pOrder->juno), 0);
					SetModArrow(4, 0, str2int(pOrder->juno));
				}

/*	
				if (m_hogaStart == 0 && mmgb == str2int(pOrder->mmgb))
				{
					m_pParent->SendJumun("4", hoga, str2int(pOrder->juno), 0);
					SetModArrow(4, 0, str2int(pOrder->juno));
				}
				else if (m_hogaStart == str2double(pOrder->jprc) && mmgb == str2int(pOrder->mmgb))
				{
					m_pParent->SendJumun("4", hoga, str2int(pOrder->juno), 0);
					SetModArrow(4, 0, str2int(pOrder->juno));
				}
*/
			}
			m_cs.Unlock();
		}
	}
	else if (m_bDragResv)
	{
		m_bDragResv = false;

		ReleaseCapture();
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		if (rc.PtInRect(point))		// 정정
		{
			int mmgb{};
			const int index = getPointToIndex(point, CELL_RESERVE, mmgb);
			class CReserve *pResv = NULL;

			const double hoga = getIndexToHoga(index);
			if (hoga == 0.0)
			{
				m_pParent->SetGuide("감시 가능한 가격이 아닙니다.");
				return;
			}

			if (hoga == str2double(m_curr))
			{
				m_pParent->SetGuide("현재가에서는 예약주문을 할 수 없습니다.");
				return;
			}

			if (((index > m_posSHGA && index < m_posHHGA) || index == m_posCHVL) && m_mmgb == mmgb && hoga != m_hogaStart)
			{
				const int count = m_arReserve.GetSize() - 1;
				for (int ii = count; ii >= 0; ii--)
				{
					pResv = m_arReserve.GetAt(ii);
					if (m_hogaStart == pResv->gprc && pResv->mmgb == mmgb)
					{
						int gb{};
						const int jIndex = getPointToIndex(m_ptStart, CELL_RESERVE, gb);
						const double jprc = getTickUnit(pResv->jprc, m_market == FUTURE ? true : false, mmgb, 
							mmgb == MS_JUMUN ? jIndex - index : index - jIndex);
				
						if (jprc > str2double(m_shga) || jprc < str2double(m_hhga))
						{
							m_pParent->SetGuide("주문 가능한 가격이 아닙니다.");
							return;
						}
						pResv->gprc = hoga;
						pResv->jprc = jprc;

						if (pResv->gprc > str2double(m_curr))
							pResv->gubn = aboveCURR;
						else
							pResv->gubn = belowCURR;
						SCROLLINFO	info; int nPos;
						m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
						nPos = info.nPos;	
						pResv->index = nPos+index;
					}
				}
				invalidateRC();
			}
		}
		else				// 취소
		{
			int mmgb{};
			const int index = getPointToIndex(m_ptStart, CELL_RESERVE, mmgb);
			class CReserve *pResv = NULL;
			
			const int count = m_arReserve.GetSize() - 1;
			for (int ii = count; ii >= 0; ii--)
			{
				pResv = m_arReserve.GetAt(ii);
				if (m_hogaStart == 0 && pResv->mmgb == mmgb)
				{
					delete pResv;	pResv = NULL;
					m_arReserve.RemoveAt(ii);
				}
				else if (m_hogaStart == pResv->gprc && pResv->mmgb == mmgb)
				{
					delete pResv;	pResv = NULL;
					m_arReserve.RemoveAt(ii);
				}
			}
			invalidateRC();
		}
	}
	else 
	{
		int mmgb{};
		const int index = getPointToIndex(point, CELL_HOGA, mmgb);
		if (index >= 0)
		{
			const double hoga = getIndexToHoga(index);
			if (hoga > 0.0)
			{
				CString jmdg;
				jmdg.Format("%.2f", hoga);
				m_pCtrlWnd->SetJmdg(jmdg);
			}
		}
	}
	m_bDragJumun = false;
	m_bDragResv = false;

	invalidateRC();

	CWnd::OnLButtonUp(nFlags, point);
}

void CHogaWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	//return;
	if(!m_pCtrlWnd->GetTrackMode()) return;
	CRect rc;
	GetClientRect(&rc);

	if (!m_bTracking)
	{
		//TRACE("OnMouseMove\n");
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
		return;
	}

	int mmgb{}, index{};
	double hoga{};
	if (getPointToIndex(point, CELL_JUMUN, mmgb) >= 0)
	{
		index = getPointToIndex(point, CELL_JUMUN, mmgb);
		hoga = getIndexToHoga(index);
		
		if (mmgb == MD_JUMUN)
		{
			m_rcFocus.SetRect(m_rcMDJumun[index].left, m_rcMDJumun[index].top, m_rcHoga[index].right, m_rcHoga[index].bottom);
			m_focusMmgb = 1;
			m_focusIndex = index;
		}
		else
		{
			m_rcFocus.SetRect(m_rcHoga[index].left, m_rcHoga[index].top, m_rcMSJumun[index].right, m_rcMSJumun[index].bottom);
			m_focusMmgb = 2;
			m_focusIndex = index;
		}

		if (index != m_posCHVL)
			m_pCtrlWnd->SetPrice(mmgb, (int)hoga);
	}
	else if (getPointToIndex(point, CELL_RESERVE, mmgb) >= 0)
	{
		index = getPointToIndex(point, CELL_RESERVE, mmgb);
		hoga = getIndexToHoga(index);
		
		if (mmgb == MD_JUMUN)
		{
			m_rcFocus.SetRect(m_rcMDResv[index].left, m_rcMDResv[index].top, m_rcHoga[index].right, m_rcHoga[index].bottom);
			m_focusMmgb = 1;
			m_focusIndex = index;
		}
		else
		{
			m_rcFocus.SetRect(m_rcHoga[index].left, m_rcHoga[index].top, m_rcMSResv[index].right, m_rcMSResv[index].bottom);
			m_focusMmgb = 2;
			m_focusIndex = index;
		}

		if (index != m_posCHVL)
			m_pCtrlWnd->SetPrice(mmgb, (int)hoga);
	}
	else
	{
		m_rcFocus.SetRectEmpty();
		m_focusIndex = -1;
	}

	if (m_bDragJumun)
	{
		if (getPointToIndex(point, CELL_JUMUN, mmgb) >= 0 && mmgb == m_mmgb)
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
		}
		else
		{
			if (rc.PtInRect(point))
				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_NO)));
			else
				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
		}
	}
	else if (m_bDragJumun)
	{
		if (getPointToIndex(point, CELL_RESERVE, mmgb) >= 0 && mmgb == m_mmgb)
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
		}
		else
		{
			if (rc.PtInRect(point))
				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_NO)));
			else
				SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
		}
	}

	if (m_focusMmgb == 1)
		m_pTrackingWnd->m_color = 1;
	else
		m_pTrackingWnd->m_color = 2;
	//if ((trackpoint.x == index) && (trackpoint.y = mmgb)) return;
	//trackpoint.x = index; trackpoint.y = mmgb;
	m_pTrackingWnd->MoveWindow(m_rcFocus);

	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CHogaWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking	= FALSE;

	m_rcFocus.SetRectEmpty();
	m_focusIndex = -1;

	m_pTrackingWnd->MoveWindow(m_rcFocus);

	return 0;
}

BOOL CHogaWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	m_vbar.OnMouseWheel(nFlags, zDelta, pt);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CHogaWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (!m_bHold)
	{
		for (int ii = 0; ii < m_rows - 1; ii++)
		{	
			if (m_rcHoga[ii].PtInRect(point))
			{
				setScroll(m_depth);	
				break;
			}else if (m_rcDvol[ii].PtInRect(point))
			{
				setScroll(m_depth);	
				break;
			}else if (m_rcSvol[ii].PtInRect(point))
			{
				setScroll(m_depth);
				break;
			}
		}	
	}
	if (!checkJumunAvailable())		
		return;

	if (m_bOneClick)
		return;

	double	hoga{};
	const int	count = m_pCtrlWnd->GetMultiCount();
	CString price, gprc, jprc;

	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);

	for (int ii = 0; ii < m_rows - 1; ii++)
	{
		hoga = getIndexToHoga(ii);
		if (hoga == 0.0)
			continue;

		price.Format("%.2f", hoga);

		if (m_rcMDJumun[ii].PtInRect(point))
		{
			//2014.06.23 KSJ 주문착오방지 수정
			MistakeOrder misOdr;
			CString strJmgb;
			
			misOdr.strCode = m_pParent->GetCode();
			misOdr.lPrice = hoga;
			misOdr.lQuantity = m_pCtrlWnd->GetJumunCount();
			misOdr.nMmgb = MD_JUMUN;
			strJmgb.Format("%c%c", m_pCtrlWnd->GetJmgb().GetAt(0), '0');
			misOdr.strHogb = strJmgb;
			misOdr.strCurPrc = m_curr;
			misOdr.strStdPrc = m_pivot;
			
			const bool bRet = CheckupMistakeOrder(this, misOdr);
			
			if (!bRet)
				return;
			//END KSJ

			if (getConfirmJumun())
			{
				CDialogConfirm dlg(rc, m_pView, m_pParent);;
				dlg.SetData('S', m_pParent->GetAccount(), m_pParent->GetName(), 
					m_pParent->GetCodeName(), m_pCtrlWnd->GetJumunCount(), price, 'F', "", m_pCtrlWnd->GetJmgb());
				if (dlg.DoModal() != IDOK)
					return;
			}

			for (int jj = 0; jj < count; jj++)
			{
				((CMapWnd*)m_pParent)->SendJumun("1", hoga);
				hoga = getPriceUnit(hoga, m_market == FUTURE ? true : false, true);
			}
			break;
		}
		else if (m_rcMSJumun[ii].PtInRect(point))
		{
			//2014.06.23 KSJ 주문착오방지 수정
			MistakeOrder misOdr;
			CString strJmgb;
			
			misOdr.strCode = m_pParent->GetCode();
			misOdr.lPrice = hoga;
			misOdr.lQuantity = m_pCtrlWnd->GetJumunCount();
			misOdr.nMmgb = MS_JUMUN;
			strJmgb.Format("%c%c", m_pCtrlWnd->GetJmgb().GetAt(0), '0');
			misOdr.strHogb = strJmgb;
			misOdr.strCurPrc = m_curr;
			misOdr.strStdPrc = m_pivot;
			
			const bool bRet = CheckupMistakeOrder(this, misOdr);
			
			if (!bRet)
				return;
			//END KSJ

			if (getConfirmJumun())
			{
				CDialogConfirm dlg(rc, m_pView, m_pParent);
				dlg.SetData('B', m_pParent->GetAccount(), m_pParent->GetName(), 
					m_pParent->GetCodeName(), m_pCtrlWnd->GetJumunCount(), price, 'F', "", m_pCtrlWnd->GetJmgb());
				if (dlg.DoModal() != IDOK)
					return;
			}

			for (int jj = 0; jj < count; jj++)
			{
				((CMapWnd*)m_pParent)->SendJumun("2", hoga);
				hoga = getPriceUnit(hoga, m_market == FUTURE ? true : false, false);
			}
			break;
		}
		else if (m_rcMDResv[ii].PtInRect(point))
		{
			if (ii == m_posSHGA || ii == m_posHHGA || ii == m_posCHVL)
				return;

			if (hoga == str2double(m_curr))
			{
				::MessageBox(m_hWnd, "현재가에서는 예약주문을 할 수 없습니다.", "선물옵션 클릭주문", MB_ICONWARNING);
				return;
			}

			if (getTickUnit(hoga, m_market == FUTURE ? true : false, MD_JUMUN) < str2double(m_hhga) 
				|| getTickUnit(hoga, m_market == FUTURE ? true : false, MD_JUMUN) > str2double(m_shga))
			{
				m_pParent->SetGuide("주문 가능한 가격이 아닙니다.");
				return;
			}

			class CReserve *pResv = new class CReserve;
			pResv->code = m_pParent->GetCode();
			pResv->name = m_pParent->GetCodeName();
			pResv->gprc = hoga;
			pResv->jprc = getTickUnit(hoga, m_market == FUTURE ? true : false, MD_JUMUN);
			pResv->jqty = m_pCtrlWnd->GetJumunCount();
			pResv->mmgb = MD_JUMUN;
			if(m_pCtrlWnd->GetMarket())
			{
				pResv->tick = 0;
			}
			else
			{
				pResv->tick = m_pCtrlWnd->GetTickCount();
			}
			pResv->market = m_pCtrlWnd->GetMarket();
			if (pResv->gprc > str2double(m_curr))
				pResv->gubn = aboveCURR;
			else
				pResv->gubn = belowCURR;
			SCROLLINFO	info; int nPos;
			m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
			nPos = info.nPos;	
			pResv->index = nPos+ii;
			m_arReserve.Add(pResv);

			break;
		}
		else if (m_rcMSResv[ii].PtInRect(point))
		{
			if (ii == m_posSHGA || ii == m_posHHGA || ii == m_posCHVL)
				return;

			if (hoga == str2double(m_curr))
			{
				::MessageBox(m_hWnd, "현재가에서는 예약주문을 할 수 없습니다.", "선물옵션 클릭주문", MB_ICONWARNING);
				return;
			}

			if (getTickUnit(hoga, m_market == FUTURE ? true : false, MS_JUMUN) < str2double(m_hhga) 
				|| getTickUnit(hoga, m_market == FUTURE ? true : false, MS_JUMUN) > str2double(m_shga))
			{
				m_pParent->SetGuide("주문 가능한 가격이 아닙니다.");
				return;
			}

			class CReserve *pResv = new class CReserve;
			pResv->code = m_pParent->GetCode();
			pResv->name = m_pParent->GetCodeName();
			pResv->gprc = hoga;
			pResv->jprc = getTickUnit(hoga, m_market == FUTURE ? true : false, MS_JUMUN);
			pResv->jqty = m_pCtrlWnd->GetJumunCount();
			pResv->mmgb = MS_JUMUN;
			if(m_pCtrlWnd->GetMarket())
			{
				pResv->tick = 0;
			}
			else
			{
				pResv->tick = m_pCtrlWnd->GetTickCount();
			}
			pResv->market = m_pCtrlWnd->GetMarket();
			if (pResv->gprc > str2double(m_curr))
				pResv->gubn = aboveCURR;
			else
				pResv->gubn = belowCURR;
			SCROLLINFO	info; int nPos;
			m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
			nPos = info.nPos;	
			pResv->index = nPos+ii;
			m_arReserve.Add(pResv);

			break;
		}
	}
	
	invalidateRC();

	CWnd::OnLButtonDblClk(nFlags, point);
}

CString CHogaWnd::getHogaData(CString hoga, int mmgb)
{
	if (hoga.IsEmpty() || hoga.GetLength() <= 0 || str2double(hoga) == 0.0 || m_code.IsEmpty())
		return "";
	CString data = "", jango = m_pCtrlWnd->GetJango(), maip = m_pCtrlWnd->GetMaip();
	const double money = m_code.GetLength() && m_code.GetAt(0) == '1' ? 500000 : 100000;
	for (int ii = 0; ii < 5; ii++)
	{
		if (mmgb == 1)		// 매도
		{
			if (str2double(m_hoga[ii].mdga) == str2double(hoga))
				data = m_hoga[ii].dvol;
		}
		else if (mmgb == 2)	// 매수
		{
			if (str2double(m_hoga[ii].msga) == str2double(hoga))
				data = m_hoga[ii].svol;	
		}
	}

	return data;
}

double CHogaWnd::getIndexToHoga(int index)
{
	if (index < 0 || index >= m_arHoga.GetSize())	return 0;

	SCROLLINFO	info;
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	const int nPos = info.nPos;
	
	if (index == m_posSHGA)
		return str2double(m_shga);
	else if (index == m_posHHGA)
		return str2double(m_hhga);
	else if (index == m_posCHVL)
		return 0;
	else
	{
		const int pos = nPos + index - 1;
		if (pos < m_arHoga.GetSize() || pos > -1)
			return str2double(m_arHoga.GetAt(pos));
	}
	return 0;
}

CString CHogaWnd::getHogaToJumun(CString hoga, int mmgb, CRect rc, bool bTotal)
{
	if (str2double(hoga) == 0.0 && !bTotal)
		return "";

	CString data, no, vol, tip = "";
	int	juno=0, jvol = 0, jcnt = 0;
	class COrder* pOrder{};
	
	m_cs.Lock();
	POSITION pos = m_mapOrder.GetStartPosition();
	while( pos != NULL )
	{
		m_mapOrder.GetNextAssoc(pos, juno, pOrder);

		if (bTotal)
		{
			if (mmgb == str2int(pOrder->mmgb))
			{
				jvol += str2int(pOrder->mcvl);
				jcnt++;
				no.Format("   %05d", str2int(pOrder->juno));
				vol.Format("%9d", str2int(pOrder->mcvl));
				tip += no + vol + "\n";
			}
		}
		else
		{
			if (str2double(hoga) == str2double(pOrder->jprc) && mmgb == str2int(pOrder->mmgb))
			{
				jvol += str2int(pOrder->mcvl);
				jcnt++;
				no.Format("   %05d", str2int(pOrder->juno));
				vol.Format("%9d", str2int(pOrder->mcvl));
				tip += no + vol + "\n";
			}
		}
	}
	m_cs.Unlock();
	if (!tip.IsEmpty())
	{
		tip = tip.Left(tip.GetLength() - 1);
		tip = "주문번호   수량\n" + tip;
	}
	
	if (m_config.jmcn == 0)
		data.Format("%d(%d)", jvol, jcnt);
	else
		data.Format("%d", jvol);

	if (jcnt > 0)
	{
		if (m_tooltip)
			m_tooltip->AddTool(this, tip, rc, ++m_indexTip);

		return data;
	}
	else
		return "";
}

CString CHogaWnd::getReserveData(CString hoga, int mmgb, CRect rc, bool bTotal)
{
	if (str2double(hoga) == 0.0 && !bTotal)
		return "";

	int	jvol = 0, jcnt = 0;
	CString data, tip = "", text = "";
	for (int ii = 0; ii < m_arReserve.GetSize(); ii++)
	{
		class CReserve *pResv = m_arReserve.GetAt(ii);
		if (bTotal)
		{
			if (mmgb == pResv->mmgb && m_pParent->GetCode() == pResv->code)
			{
				jvol += pResv->jqty;
				jcnt++;
				if (pResv->gubn == aboveCURR)
					text.Format("%d. 현재가가 %.2f 보다 같거나 크면 %.2f 에 %d 계약 주문\n", jcnt, pResv->gprc, pResv->jprc, pResv->jqty);
				else
					text.Format("%d. 현재가가 %.2f 보다 같거나 작으면 %.2f 에 %d 계약 주문\n", jcnt, pResv->gprc, pResv->jprc, pResv->jqty);
				tip += text;
			}
		}
		else
		{
			if (mmgb == pResv->mmgb && pResv->gprc == str2double(hoga) && m_pParent->GetCode() == pResv->code)
			{
				jvol += pResv->jqty;
				jcnt++;
				if (pResv->gubn == aboveCURR)
					text.Format("%d. 현재가가 %.2f 보다 같거나 크면 %.2f 에 %d 계약 주문\n", jcnt, pResv->gprc, pResv->jprc, pResv->jqty);
				else
					text.Format("%d. 현재가가 %.2f 보다 같거나 작으면 %.2f 에 %d 계약 주문\n", jcnt, pResv->gprc, pResv->jprc, pResv->jqty);
				tip += text;
			}
		}
	}
	if (!tip.IsEmpty())
		tip = tip.Left(tip.GetLength() - 1);

	if (m_config.jmcn == 0)
		data.Format("%d (%d)", jvol, jcnt);
	else
		data.Format("%d", jvol);

	if (jcnt > 0)
	{
		if (m_tooltip)
			m_tooltip->AddTool(this, tip, rc, ++m_indexTip);

		return data;
	}
	else
		return "";
}

void CHogaWnd::removeJumun(int juno)
{
	m_cs.Lock();

	class COrder* pOrder = NULL;
	m_mapOrder.Lookup(juno, pOrder);
		
	if (pOrder)
	{
		delete pOrder;	pOrder = NULL;
		m_mapOrder.RemoveKey(juno);
	}

	for (int ii = 0; ii < m_arArrow.GetSize(); ii++)
	{
		class CArrow *pArrow = m_arArrow.GetAt(ii);
		if (juno == pArrow->ojno)
		{
			m_arArrow.RemoveAt(ii);
			[[gsl::suppress(26409)]]
			delete pArrow;	
			pArrow = NULL;
			break;
		}
	}
	
	m_cs.Unlock();
}

void CHogaWnd::removeAllJumun()
{
	m_cs.Lock();

	int	juno=0;
	class COrder* pOrder{};
	POSITION pos = m_mapOrder.GetStartPosition();
	while( pos != NULL )
	{
		m_mapOrder.GetNextAssoc(pos, juno, pOrder);

		delete pOrder;	pOrder = NULL;
		m_mapOrder.RemoveKey(juno);
	}
	m_mapOrder.RemoveAll();

	m_cs.Unlock();
}

void CHogaWnd::removeAllReserve()
{
	for (int ii = 0; ii < m_arReserve.GetSize(); ii++)
	{
		class CReserve *pResv = m_arReserve.GetAt(ii);
		delete pResv;	pResv = NULL;
	}

	m_arReserve.RemoveAll();
}

void CHogaWnd::removeAllArrow()
{
	for (int ii = 0; ii < m_arArrow.GetSize(); ii++)
	{
		class CArrow *pArrow = m_arArrow.GetAt(ii);
		[[gsl::suppress(26409)]]
		delete pArrow;	
		pArrow = NULL;
	}

	m_arArrow.RemoveAll();
}

int CHogaWnd::getPointToIndex(CPoint point, int cell, int& mmgb)
{
	int index = -1;

	for (int ii = 0; ii < m_rows; ii++)
	{
		CRect rcMD, rcMS;
		switch (cell)
		{
		case CELL_HOGA:
			rcMD = m_rcHoga[ii];
			rcMS = m_rcHoga[ii];
			break;
		case CELL_JUMUN:
			rcMD = m_rcMDJumun[ii];
			rcMS = m_rcMSJumun[ii];
			break;
		case CELL_VOL:
			rcMD = m_rcDvol[ii];
			rcMS = m_rcSvol[ii];
			break;
		case CELL_RESERVE:
			rcMD = m_rcMDResv[ii];
			rcMS = m_rcMSResv[ii];
			break;
		}

		if (rcMD.PtInRect(point))
		{
			mmgb = 1;
			index = ii;
			break;
		}
		else if (rcMS.PtInRect(point))
		{
			mmgb = 2;
			index = ii;
			break;
		}
	}

	return index;
}

BOOL CHogaWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
#if 1
	if (m_bDragJumun)
	{
		SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
		return TRUE;
	}
	else
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return TRUE;
	}
#else
	if (m_iFromOrderIndex > -1 && m_enumOrdering == NO_ORDERING && !m_bPopup)
	{
		if (m_iFocusedRectIndex >= SELL_ORDER1 && m_iFocusedRectIndex <= SELL_ORDER_MARKET && m_cSBKind == 'S')
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
			return TRUE;
		}
		else if (m_iFocusedRectIndex >= BUY_ORDER1 && m_iFocusedRectIndex <= BUY_ORDER_MARKET && m_cSBKind == 'B')
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
			return TRUE;
		}
		else
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_NO)));
			return TRUE;
		}
	}
	else if (m_enumOrdering == ORDERING)
		SetCursor(LoadCursor(NULL, IDC_WAIT));
#endif
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CHogaWnd::SetHogaDepth(int depth)
{
	if (m_depth == depth)
		return;

	CRect	rc;
	GetClientRect(&rc);

	m_depth = (depth == 0) ? hgDEPTH10 : hgDEPTH5;

	if (m_depth == hgDEPTH10) 
	{
		m_posSHGA = D10SHGA;
		m_posHHGA = D10HHGA;
		m_posCHVL = D10CHVL;	
		m_posBTN = D10BTN;	
	}
	else
	{
		m_posSHGA = D5SHGA;
		m_posHHGA = D5HHGA;
		m_posCHVL = D5CHVL;
		m_posBTN = D5BTN;	
	}
	m_rows = m_posCHVL + 1;
	setHogaRect();
	setScroll(m_depth);
}

bool CHogaWnd::getConfirmJumun()
{
	return m_config.confirm ? true : false;
}

bool CHogaWnd::checkJumunAvailable(int mmgb)
{
	CString acc, pswd, code;
	acc = m_pParent->GetAccount();
	pswd = m_pParent->GetPswd();
	code = m_pParent->GetCode();
	const int jmcn = m_pCtrlWnd->GetJumunCount();

	if (acc.IsEmpty())
	{
		m_pParent->SetGuide("계좌번호를 확인하십시오.");
		return false;
	}

	if (pswd.IsEmpty())
	{
		m_pParent->SetGuide("비밀번호를 확인하십시오.");
		return false;
	}

	if (code.IsEmpty())
	{
		m_pParent->SetGuide("종목코드를 확인하십시오.");
		return false;
	}

	if (jmcn <= 0 && (mmgb == 1 || mmgb == 2))
	{
		m_pParent->SetGuide("주문수량을 확인하십시오.");
		return false;
	}

	return true;
}

int CHogaWnd::getMichegCount()
{
	int	juno=0, count = 0;
	class COrder *pOrder = NULL;	

	m_cs.Lock();
	POSITION pos = m_mapOrder.GetStartPosition();
	while( pos != NULL )
	{
		m_mapOrder.GetNextAssoc(pos, juno, pOrder);
		++count;
	}
	m_cs.Unlock();	
	return count;
}

int CHogaWnd::getJumunCount(CString junos, int mmgb)
{
	m_cs.Lock();

	int	juno=0, count = 0;
	CString orderNo;
	class COrder *pOrder = NULL;	

	if (junos.IsEmpty())
	{
		POSITION pos = m_mapOrder.GetStartPosition();
		while( pos != NULL )
		{
			m_mapOrder.GetNextAssoc(pos, juno, pOrder);
			
			if (mmgb == str2int(pOrder->mmgb))
				count += atoi(pOrder->mcvl);
		}
	}
	else
	{
		while (!junos.IsEmpty())
		{
			orderNo = m_pParent->Parser(junos, "\t");
			if (m_mapOrder.Lookup(atoi(orderNo), pOrder))
				count += atoi(pOrder->mcvl);
		}
	}
	m_cs.Unlock();

	return count;
}

void CHogaWnd::DoModalConfigDlg()
{
	CConfigDlg dlg(m_file);
	dlg.DoModal();

	m_config = dlg.m_config;

	invalidateRC();
}

CString CHogaWnd::calculatePercent(CString hoga)
{
	if (!m_config.percent)
		return "";
	else if (m_config.percent == 2)
		return calculateDiffTick(hoga);

	CString percent, space = "              ";
	double price = -1;
	if (m_config.percentRadio == 0)
	{
		if (m_config.percentKind == 0)		// 기준가
			price = str2double(m_pivot);
		else if (m_config.percentKind == 1)	// 전일종가
			price = str2double(m_jijg);
		else if (m_config.percentKind == 2)	// 시가
			price = str2double(m_siga);
		else if (m_config.percentKind == 3)	// 고가
			price = str2double(m_koga);
		else if (m_config.percentKind == 4)	// 저가
			price = str2double(m_jega);
	}
	else if (m_config.percentRadio == 2)		// user
		price = str2double(m_config.user);
	
	// 전일종가가 없으면 등락율을 클리어후 리턴
	if (price <= 0)
		return "";

	double dPrice = 0;
	double dPercent = 0;
	
	dPrice = str2double(hoga);
	
	if (dPrice == 0)
		return "";
		
	// 전일종가 보다 크면
	if (dPrice > price)
	{
		dPercent = (double)(dPrice - price) / (double)price * 100;
		percent.Format("%2.2f%%", dPercent);
		const int pos = percent.Find(".");
		percent = space.Left(3 - pos) + percent;
	}
	// 전일종가 보다 작으면
	else if (dPrice < price)
	{
		dPercent = (double)(price - dPrice) / (double)price * 100;
		percent.Format("%2.2f%%", dPercent);
		const int pos = percent.Find(".");
		percent = space.Left(3 - pos) + percent;
	}
	// 전일종가와 같으면
	else
		percent = "  0.00%";

	return percent;
}

CString CHogaWnd::calculateDiffTick(CString hoga)
{
	if (!m_config.percent)
		return "";

	CString percent, space = "              ";
	double price = -1;
	if (m_config.percentRadio == 0)
	{
		if (m_config.percentKind == 0)		// 기준가
			price = str2double(m_pivot);
		else if (m_config.percentKind == 1)	// 전일종가
			price = str2double(m_jijg);
		else if (m_config.percentKind == 2)	// 시가
			price = str2double(m_siga);
		else if (m_config.percentKind == 3)	// 고가
			price = str2double(m_koga);
		else if (m_config.percentKind == 4)	// 저가
			price = str2double(m_jega);
	}
	else if (m_config.percentRadio == 2)		// user
		price = str2double(m_config.user);
	
	// 전일종가가 없으면 등락율을 클리어후 리턴
	if (price <= 0)
		return "";

	double dPrice = 0;
	double dPercent = 0;
	
	dPrice = str2double(hoga);
	
	if (dPrice == 0)
		return "";
		
	// 전일종가 보다 크면
	if (dPrice > price)
	{
		dPercent = (double)(dPrice - price);
		percent.Format("%2.2f", dPercent);
		const int pos = percent.Find(".");
		percent = space.Left(3 - pos) + percent;
	}
	// 전일종가 보다 작으면
	else if (dPrice < price)
	{
		dPercent = (double)(price - dPrice);
		percent.Format("%2.2f", dPercent);
		const int pos = percent.Find(".");
		percent = space.Left(3 - pos) + percent;
	}
	// 전일종가와 같으면
	else
		percent = "  0.00";

	return percent;
}

int CHogaWnd::SetHogaReserve()
{
	m_fdgb = m_fdgb == hgFOLD ? hgUNFOLD : hgFOLD;

	setHogaRect();

	invalidateRC();

	return m_fdgb;
}

void CHogaWnd::drawArrowLine(CDC *pDC, CPoint StartPoint, CPoint EndPoint, bool bHolder)
{
	CPen *pOldPen = pDC->SelectObject(GetAxPen(COLOR_ARROW, 1, PS_SOLID));

	double slopy{}, cosy{}, siny{};
	const double Par = 10.0;
	
	slopy = atan2( ( StartPoint.y - EndPoint.y ),
		( StartPoint.x - EndPoint.x ) );
	cosy = cos( slopy );
	siny = sin( slopy ); 
	
	pDC->MoveTo( StartPoint );
	pDC->LineTo( EndPoint );

	CRect rc(StartPoint, StartPoint);
	rc.InflateRect(3, 3);
	
	CRgn Rgn;
	CPoint Point[3];
	CBrush BlackBrush(COLOR_ARROW);

	Point[0] = CPoint( EndPoint.x + int( Par * cosy - ( Par / 2.0 * siny ) ),
		EndPoint.y + int( Par * siny + ( Par / 2.0 * cosy ) ) );
	Point[1] = CPoint( EndPoint.x + int( Par * cosy + Par / 2.0 * siny ),
		EndPoint.y - int( Par / 2.0 * cosy - Par * siny ) );
	Point[2] = EndPoint;

	Rgn.CreatePolygonRgn(Point, sizeof(Point) / sizeof(CPoint), ALTERNATE);
	pDC->FillRgn(&Rgn, &BlackBrush);

	if (bHolder)
		pDC->FillRect(&rc, &BlackBrush);

	pDC->SelectObject(pOldPen);
}

void CHogaWnd::drawArrowLine3(CDC *pDC, CPoint StartPoint, CPoint EndPoint, int mmgb)
{
	COLORREF color{};
	if (mmgb == 3)
		color = RGB(0, 124, 0);
	else
		color = RGB(200, 30, 30);

	CPen *pOldPen = pDC->SelectObject(GetAxPen(color, 1, PS_SOLID));

	double slopy{}, cosy{}, siny{};
	const double Par = 10.0;
	
	slopy = atan2( ( StartPoint.y - EndPoint.y ),
		( StartPoint.x - EndPoint.x ) );
	cosy = cos( slopy );
	siny = sin( slopy );
	
	CRect rc(StartPoint, StartPoint);
	rc.InflateRect(3, 3);
	
	CRgn Rgn;
	CPoint Point[3];
	CBrush BlackBrush(color);

	Point[0] = CPoint( EndPoint.x + int( Par * cosy - ( Par / 2.0 * siny ) ),
		EndPoint.y + int( Par * siny + ( Par / 2.0 * cosy ) ) );
	Point[1] = CPoint( EndPoint.x + int( Par * cosy + Par / 2.0 * siny ),
		EndPoint.y - int( Par / 2.0 * cosy - Par * siny ) );
	Point[2] = EndPoint;

	Rgn.CreatePolygonRgn(Point, sizeof(Point) / sizeof(CPoint), ALTERNATE);
	pDC->FillRgn(&Rgn, &BlackBrush);

	pDC->MoveTo( StartPoint );
	pDC->LineTo( EndPoint );

	pDC->SelectObject(pOldPen);
}

void CHogaWnd::DrawStar(CDC *pDC, CPoint StartPoint, CPoint EndPoint, int mmgb)
{
	COLORREF color{};
	if (mmgb == 3)
		color = RGB(0, 124, 0);
	else
		color = RGB(200, 30, 30);
	
	CPen *pOldPen = pDC->SelectObject(GetAxPen(color, 1, PS_SOLID));
	
	// 다각형의모양을가지는점들의집합을준비합니다.
	
	// 여기는별만그리지만삼각형도똑같습니다.
	
	// 그리는모양은마음대로바꾸세요.
	
	CPoint points[11]{};
	CBrush BlackBrush(color);

	CRect rc;
	rc.SetRect(0,0,10,10);

	if(StartPoint.x > EndPoint.x)
	{
		rc.OffsetRect(StartPoint.x,StartPoint.y-4);
	}
	else
	{
		rc.OffsetRect(StartPoint.x+6,StartPoint.y-4);
	}

	pDC->FillSolidRect(rc,COLOR_ARROW);
	// 72' 씩회전된꼭지점으로이루어진점생성
	
	// 여기서알아서모양을만드세요.
	
// 	double Pi = 3.14159265359;
// 	
// 	for ( int i = 0; i < 10; i += 2 )	
// 	{	
// 		// 별의바깥쪽꼭지점(큰반지름으로만듬)
// 		
// 		points[i].x = (long)(8*cos((double)(i*72*Pi)/360.0));	
// 		points[i].y = (long)(8*sin((double)(i*72*Pi)/360.0));	
// 		
// 		// 별의안쪽꼭지점(작은반지름으로만듬)
// 		
// 		points[i+1].x = (long)(4*cos((double)((i+1)*72*Pi)/360.0));	
// 		points[i+1].y = (long)(4*sin((double)((i+1)*72*Pi)/360.0));      	
// 	}
// 	
// 	// 끝점은첫점과같게맞춤니다.
// 	
// 	points[10] = points[0];
// 	
// 	CRgn rgnStar;
// 	
// 	rgnStar.CreatePolygonRgn(points, 11, WINDING);
// 	
// 	// 클라이언트영역을가져옵니다.
// 	
// 	CRect r;
// 	
// 	GetClientRect(&r);
// 	
// 	// 그림그릴브러시를초기화합니다.
// 	
// 	//brush.CreateSolidBrush(RGB(0,0,0));
// 	
// 	// 영역을초기화합니다.
// 	
// 	CRgn rgn;
// 	
// 	rgn.CreateRectRgn(0, 0, 0, 0);
// 	rgn.CopyRgn(&rgnStar);
// 	rgn.OffsetRgn(StartPoint.x,StartPoint.y);
// 	
// 	// 별을그립니다.
// 	
// 	pDC->FillRgn(&rgn, &BlackBrush);
	
	pDC->SelectStockObject(NULL_PEN);
	
	//if (bMustRelease) ReleaseDC(pDC);
}

void CHogaWnd::drawArrowLine2(CDC *pDC, CPoint StartPoint, CPoint ViaPoint, CPoint EndPoint, bool bHolder)
{
	if (ViaPoint == EndPoint)
	{
		drawArrowLine(pDC, StartPoint, EndPoint, bHolder);
		return;
	}
	else
	{
		CPen *pOldPen = pDC->SelectObject(GetAxPen(COLOR_ARROW, 1, PS_SOLID));
		pDC->MoveTo(StartPoint);
		pDC->LineTo(ViaPoint);
		pDC->SelectObject(pOldPen);

		drawArrowLine(pDC, ViaPoint, EndPoint, false);
	}
	CRect rc(StartPoint, StartPoint);
	rc.InflateRect(3, 3);
	
	CBrush BlackBrush(COLOR_ARROW);
	if (bHolder)
		pDC->FillRect(&rc, &BlackBrush);
}

void CHogaWnd::checkReserveJumun(CString code, CString curr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);

	class CReserve *pResv = NULL;

	CString price, name, mmgbS;
	double	jprc{};
	int	jqty{};
	int	mmgb{};
	bool market=false;

	for (int ii = m_arReserve.GetUpperBound(); ii >= 0; ii--)
	{
		if (ii > m_arReserve.GetUpperBound() || ii < 0) break;

		pResv = m_arReserve.GetAt(ii);
		if (!pResv) continue;
		if ((pResv->gubn == aboveCURR && pResv->gprc <= str2double(curr) && code == pResv->code)
			|| (pResv->gubn == belowCURR && pResv->gprc >= str2double(curr) && code == pResv->code))

		{
			jqty = pResv->jqty;
			jprc = pResv->jprc;
			mmgb = pResv->mmgb;
			name = pResv->name;
			market = pResv->market;
			
			delete pResv;	pResv = NULL;
			m_arReserve.RemoveAt(ii);

			//2014.06.23 KSJ 주문착오방지 수정
			MistakeOrder misOdr;
			CString strJmgb;
		
			misOdr.strCode = m_rsvCode;
			misOdr.lPrice = jprc;
			misOdr.lQuantity = jqty;
			misOdr.nMmgb = mmgb;
			strJmgb.Format("%c%c", m_pCtrlWnd->GetJmgb().GetAt(0), '0');
			misOdr.strHogb.Format("%s", market ? "03":strJmgb);
			misOdr.strCurPrc = m_curr;
			misOdr.strStdPrc = m_pivot;
			
			const bool bRet = CheckupMistakeOrder(this, misOdr);
			
			if (!bRet)
				return;
			//END KSJ

			if (getConfirmJumun())
			{
				CDialogConfirm dlg(rc, m_pView, m_pParent);
				price.Format("%.2f", jprc);
				dlg.SetData(mmgb == MD_JUMUN ? 'S' : 'B', m_pParent->GetAccount(), m_pParent->GetName(), 
					name, jqty, price, 'F', "", m_pCtrlWnd->GetJmgb(),market);
				
				/* 
				   timer-callback 펑션에 의해 호출될 경우 시스템DLL에서 호출하기때문에
				   hInstance가 변경되어 Dialog resource를 읽어오지 못한다. GetLastError()==1812
				   하여 다음 매크로를 추가하며, 추후, 다른방식으로 변경되어 해당 모듈 내에서 
				   DoModal이 호출될 경우 
						AFX_MANAGE_STATE(AfxGetStaticModuleState());
				   매크로를 주석처리할 것 by lkm 09.05.14
				*/
				const int retc = dlg.DoModal();
				if (retc != IDOK) continue;	
			}

			mmgbS.Format("%d", mmgb);
			((CMapWnd*)m_pParent)->SendJumun(mmgbS, jprc, 0, jqty,market);
		}
	}
}

LRESULT CHogaWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		{
			if (!m_tooltip)
				break;

			MSG	msg{};
			POINT	point;
			
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			msg.hwnd = m_hWnd;
			msg.message = message;
			msg.wParam = wParam;
			msg.lParam = lParam;
			msg.pt = point;
		
			m_tooltip->RelayEvent(&msg);
		}
		break;
	default:
		break;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

void CHogaWnd::OnDestroy() 
{
	KillTimer(TMID_RESERVE);
	m_bReal = false;
	KillTimer(TMID_PAINT);

	if (m_tooltip && m_tooltip->m_hWnd)
	{
		m_tooltip->Activate(FALSE);
		m_tooltip->DelTool(this);
		//m_tooltip->RelayEvent(nullptr);

		m_tooltip = nullptr;
	}
	
	CWnd::OnDestroy();
}

void CHogaWnd::ClearJumun()
{
	removeAllJumun();
	removeAllReserve();
	removeAllArrow();
}

void CHogaWnd::SetModArrow(int jflg, double price, int juno)
{
	m_cs.Lock();

	class COrder *pOrder = NULL;
	if (!m_mapOrder.Lookup(juno, pOrder))
	{
		m_cs.Unlock();
		return;
	}

	gsl::owner<class CArrow *> pArrow = new class CArrow;
	pArrow->ojno = juno;
	pArrow->jflg = jflg;
	pArrow->mmgb = str2int(pOrder->mmgb);
	pArrow->oprc = str2double(pOrder->jprc);
	pArrow->mprc = price;

	m_arArrow.Add(pArrow);

	m_cs.Unlock();
}

void CHogaWnd::OneClickJumun(CPoint point)
{
	CRect sRc, rc;
	GetClientRect(&rc);
	sRc = rc;
	ClientToScreen(&sRc);

	CString price, priceFrom, priceTo, junox, junos = "";

	int mmgb, index = getPointToIndex(point, CELL_JUMUN, mmgb);
	double hoga = getIndexToHoga(index);
	
	if ((index >= m_posSHGA && index <= m_posHHGA) && m_mmgb == mmgb && m_hogaStart == hoga)
	{
		if (!checkJumunAvailable())
			return;
		
		const int	count = m_pCtrlWnd->GetMultiCount();
		CString price, gprc, jprc;
		
		CRect rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		
		for (int ii = 0; ii < m_rows - 1; ii++)
		{
			hoga = getIndexToHoga(ii);
			if (hoga == 0.0)
				continue;
			
			price.Format("%.2f", hoga);
			
			if (m_rcMDJumun[ii].PtInRect(point))
			{
				m_bDragJumun = false;
				ReleaseCapture();
				SetCursor(LoadCursor(NULL, IDC_ARROW));

				//2014.06.23 KSJ 주문착오방지 수정
				MistakeOrder misOdr;
				CString strJmgb;
				
				misOdr.strCode = m_pParent->GetCode();
				misOdr.lPrice = hoga;
				misOdr.lQuantity = m_pCtrlWnd->GetJumunCount();
				misOdr.nMmgb = MD_JUMUN;
				strJmgb.Format("%c%c", m_pCtrlWnd->GetJmgb().GetAt(0), '0');
				misOdr.strHogb = strJmgb;
				misOdr.strCurPrc = m_curr;
				misOdr.strStdPrc = m_pivot;
				
				const bool bRet = CheckupMistakeOrder(this, misOdr);
				
				if (!bRet)
					return;
				//END KSJ


				if (getConfirmJumun())
				{
					CDialogConfirm dlg(rc, m_pView, m_pParent);;
					dlg.SetData('S', m_pParent->GetAccount(), m_pParent->GetName(), 
						m_pParent->GetCodeName(), m_pCtrlWnd->GetJumunCount(), price, 'F', "", m_pCtrlWnd->GetJmgb());
					if (dlg.DoModal() != IDOK)
						return;
				}
				
				for (int jj = 0; jj < count; jj++)
				{
					((CMapWnd*)m_pParent)->SendJumun("1", hoga);
					hoga = getPriceUnit(hoga, m_market == FUTURE ? true : false, true);
				}
				break;
			}
			else if (m_rcMSJumun[ii].PtInRect(point))
			{

				m_bDragJumun = false;
				ReleaseCapture();
				SetCursor(LoadCursor(NULL, IDC_ARROW));

				//2014.06.23 KSJ 주문착오방지 수정
				MistakeOrder misOdr;
				CString strJmgb;
				
				misOdr.strCode = m_pParent->GetCode();
				misOdr.lPrice = hoga;
				misOdr.lQuantity = m_pCtrlWnd->GetJumunCount();
				misOdr.nMmgb = MS_JUMUN;
				strJmgb.Format("%c%c", m_pCtrlWnd->GetJmgb().GetAt(0), '0');
				misOdr.strHogb = strJmgb;
				misOdr.strCurPrc = m_curr;
				misOdr.strStdPrc = m_pivot;
				
				const bool bRet = CheckupMistakeOrder(this, misOdr);
				
				if (!bRet)
					return;
				//END KSJ

				if (getConfirmJumun())
				{
					CDialogConfirm dlg(rc, m_pView, m_pParent);
					dlg.SetData('B', m_pParent->GetAccount(), m_pParent->GetName(), 
						m_pParent->GetCodeName(), m_pCtrlWnd->GetJumunCount(), price, 'F', "", m_pCtrlWnd->GetJmgb());
					if (dlg.DoModal() != IDOK)
						return;
				}
				
				for (int jj = 0; jj < count; jj++)
				{
					((CMapWnd*)m_pParent)->SendJumun("2", hoga);
					hoga = getPriceUnit(hoga, m_market == FUTURE ? true : false, false);
				}
				break;
			}
		}
	}
// Reserve Order
	index = getPointToIndex(point, CELL_RESERVE, mmgb);
	hoga = getIndexToHoga(index);
	
	if ((index >= m_posSHGA && index <= m_posHHGA) && m_mmgb == mmgb && m_hogaStart == hoga)
	{
		if (!checkJumunAvailable())
			return;
		
		const int	count = m_pCtrlWnd->GetMultiCount();
		CString price, gprc, jprc;
		
		CRect rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);
		
		for (int ii = 0; ii < m_rows - 1; ii++)
		{
			hoga = getIndexToHoga(ii);
			if (hoga == 0.0)
				continue;
			
			price.Format("%.2f", hoga);
			
			if (m_rcMDResv[ii].PtInRect(point))
			{
				if (ii == m_posSHGA || ii == m_posHHGA || ii == m_posCHVL)
					return;
				
				if (hoga == str2double(m_curr))
				{
					::MessageBox(m_hWnd, "현재가에서는 예약주문을 할 수 없습니다.", "선물옵션 클릭주문", MB_ICONWARNING);
					return;
				}
				
				if (getTickUnit(hoga, m_market == FUTURE ? true : false, MD_JUMUN) < str2double(m_hhga) 
					|| getTickUnit(hoga, m_market == FUTURE ? true : false, MD_JUMUN) > str2double(m_shga))
				{
					m_pParent->SetGuide("주문 가능한 가격이 아닙니다.");
					return;
				}
				
				class CReserve *pResv = new class CReserve;
				pResv->code = m_pParent->GetCode();
				pResv->name = m_pParent->GetCodeName();
				pResv->gprc = hoga;
				pResv->jprc = getTickUnit(hoga, m_market == FUTURE ? true : false, MD_JUMUN);
				pResv->jqty = m_pCtrlWnd->GetJumunCount();
				pResv->mmgb = MD_JUMUN;

				if(m_pCtrlWnd->GetMarket())
				{
					pResv->tick = 0;
				}
				else
				{
					pResv->tick = m_pCtrlWnd->GetTickCount();
				}
				pResv->market = m_pCtrlWnd->GetMarket();
				if (pResv->gprc > str2double(m_curr))
					pResv->gubn = aboveCURR;
				else
					pResv->gubn = belowCURR;
				SCROLLINFO	info; int nPos;
				m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
				nPos = info.nPos;	
				pResv->index = nPos+ii;
				m_arReserve.Add(pResv);
				
				break;
			}
			else if (m_rcMSResv[ii].PtInRect(point))
			{
				if (ii == m_posSHGA || ii == m_posHHGA || ii == m_posCHVL)
					return;
				
				if (hoga == str2double(m_curr))
				{
					::MessageBox(m_hWnd, "현재가에서는 예약주문을 할 수 없습니다.", "선물옵션 클릭주문", MB_ICONWARNING);
					return;
				}
				
				if (getTickUnit(hoga, m_market == FUTURE ? true : false, MS_JUMUN) < str2double(m_hhga) 
					|| getTickUnit(hoga, m_market == FUTURE ? true : false, MS_JUMUN) > str2double(m_shga))
				{
					m_pParent->SetGuide("주문 가능한 가격이 아닙니다.");
					return;
				}
				
				class CReserve *pResv = new class CReserve;
				pResv->code = m_pParent->GetCode();
				pResv->name = m_pParent->GetCodeName();
				pResv->gprc = hoga;
				pResv->jprc = getTickUnit(hoga, m_market == FUTURE ? true : false, MS_JUMUN);
				pResv->jqty = m_pCtrlWnd->GetJumunCount();
				pResv->mmgb = MS_JUMUN;
				if(m_pCtrlWnd->GetMarket())
				{
					pResv->tick = 0;
				}
				else
				{
					pResv->tick = m_pCtrlWnd->GetTickCount();
				}
				pResv->market = m_pCtrlWnd->GetMarket();
				if (pResv->gprc > str2double(m_curr))
					pResv->gubn = aboveCURR;
				else
					pResv->gubn = belowCURR;
				SCROLLINFO	info; int nPos;
				m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
				nPos = info.nPos;	
				pResv->index = nPos+ii;
				m_arReserve.Add(pResv);
				
				break;
			}
		}
	}
}

void CHogaWnd::ClickMod(CPoint point)
{
	CRect sRc, rc;
	GetClientRect(&rc);
	sRc = rc;
	ClientToScreen(&sRc);

	CString price, priceFrom, priceTo, strOne, junox, junos = "";

	int mmgb=0, index = getPointToIndex(point, CELL_JUMUN, mmgb);
	double hoga = getIndexToHoga(index);
	
	if ((index >= m_posSHGA && index <= m_posHHGA) && m_mmgb == mmgb && m_hogaStart == hoga)
	{
		int juno{}, mmgb;
		const int index = getPointToIndex(point, CELL_JUMUN, mmgb);
		class COrder *pOrder = NULL;
		
		priceTo.Format("%.2f", hoga);
		priceTo = SetComma(priceTo);
		price.Format("%s", priceTo);
		
		if (hoga == 0.0)	
		{
			m_pParent->SetGuide("주문 가능한 가격이 아닙니다.");
			return;
		}
		
		CSelJumunDlg dlg(this);
		CPoint	pt = point;
		ClientToScreen(&pt);
		dlg.Init(pt);
		
		int count = 0;
		m_cs.Lock();
		POSITION pos = m_mapOrder.GetStartPosition();
		while( pos != NULL )
		{
			m_mapOrder.GetNextAssoc(pos, juno, pOrder);
			
			if (mmgb == str2int(pOrder->mmgb))
			{
				dlg.SetList(pOrder->juno, pOrder->mcvl);
				junos += pOrder->juno + "\t";
				count++;
				strOne = pOrder->juno;
			}
			if (str2double(pOrder->jprc) == hoga && mmgb == str2int(pOrder->mmgb))
			{
				OneClickJumun(point);
				m_cs.Unlock();
				return;
			}
		}
		if (count)
			strOne.Format("%s 외 %d개", strOne, count - 1);
		m_cs.Unlock();
		
		if (!count)
		{
			OneClickJumun(point);
			return;
		}

		m_bDragJumun = false;
		ReleaseCapture();
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		if (m_config.mod == 0 || count == 1)
		{
			//2014.06.23 KSJ 주문착오방지 수정
			MistakeOrder misOdr;
			CString strJmgb;
			
			misOdr.strCode = m_pParent->GetCode();
			misOdr.lPrice = hoga;
			misOdr.lQuantity = m_pCtrlWnd->GetJumunCount();
			misOdr.nMmgb = mmgb;
			strJmgb.Format("%c%c", m_pCtrlWnd->GetJmgb().GetAt(0), '0');
			misOdr.strHogb = strJmgb;
			misOdr.strCurPrc = m_curr;
			misOdr.strStdPrc = m_pivot;
			
			const bool bRet = CheckupMistakeOrder(this, misOdr);
			
			if (!bRet)
				return;
			//END KSJ

			if (getConfirmJumun())
			{
				CDialogConfirm cdlg(sRc, m_pView, m_pParent);
				cdlg.SetData('F', m_pParent->GetAccount(), m_pParent->GetName(), 
					m_pParent->GetCodeName(), getJumunCount(junos, mmgb), price, NULL, dlg.GetJunoString());
				if (cdlg.DoModal() != IDOK)
					return;
			}
			while (!junos.IsEmpty())
			{
				junox = m_pParent->Parser(junos, "\t");
				m_pParent->SendJumun("3", hoga, str2int(junox), 0);
				SetModArrow(3, hoga, str2int(junox));
			}
		}
		else if (dlg.DoModal() == IDOK)
		{
			if (!dlg.GetJumunCount())	return;
			junos = dlg.GetJuno();
			
			//2014.06.23 KSJ 주문착오방지 수정
			MistakeOrder misOdr;
			CString strJmgb;
			
			misOdr.strCode = m_pParent->GetCode();
			misOdr.lPrice = hoga;
			misOdr.lQuantity = m_pCtrlWnd->GetJumunCount();
			misOdr.nMmgb = mmgb;
			strJmgb.Format("%c%c", m_pCtrlWnd->GetJmgb().GetAt(0), '0');
			misOdr.strHogb = strJmgb;
			misOdr.strCurPrc = m_curr;
			misOdr.strStdPrc = m_pivot;
			
			const bool bRet = CheckupMistakeOrder(this, misOdr);
			
			if (!bRet)
				return;
			//END KSJ

			if (getConfirmJumun())
			{
				CDialogConfirm cdlg(sRc, m_pView, m_pParent);
				cdlg.SetData('F', m_pParent->GetAccount(), m_pParent->GetName(), 
					m_pParent->GetCodeName(), getJumunCount(junos, mmgb), price, NULL, dlg.GetJunoString());
				if (cdlg.DoModal() != IDOK)
					return;
			}
			
			while (!junos.IsEmpty()) 
			{
				junox = m_pParent->Parser(junos, "\t");
				m_pParent->SendJumun("3", hoga, str2int(junox), 0);
				SetModArrow(3, hoga, str2int(junox));
			}
		}        
	}
// modify reserve order
	index = getPointToIndex(point, CELL_RESERVE, mmgb);
	hoga = getIndexToHoga(index);
	
	if ((index >= m_posSHGA && index <= m_posHHGA) && m_mmgb == mmgb && m_hogaStart == hoga)
	{
		if (rc.PtInRect(point))		// 정정
		{
			int mmgb{};
			const int index = getPointToIndex(point, CELL_RESERVE, mmgb);
			class CReserve *pResv = NULL;

			const double hoga = getIndexToHoga(index);
			if (hoga == 0.0)
			{
				m_pParent->SetGuide("감시 가능한 가격이 아닙니다.");
				return;
			}

			if (hoga == str2double(m_curr))
			{
				m_pParent->SetGuide("현재가에서는 예약주문을 할 수 없습니다.");
				return;
			}

			int cnt = 0;
			const int count = m_arReserve.GetSize() - 1;
			for (int ii = count; ii >= 0; ii--)
			{
				pResv = m_arReserve.GetAt(ii);
				
				if (pResv->mmgb == mmgb && pResv->gprc == hoga)
				{
					OneClickJumun(point);
					return;
				}
				else if (pResv->mmgb == mmgb)
					cnt++;
			}
			if (!cnt)
			{
				OneClickJumun(point);
				return;
			}
			
			for (int ii = count; ii >= 0; ii--)
			{
				pResv = m_arReserve.GetAt(ii);
				if (m_hogaStart != pResv->gprc && pResv->mmgb == mmgb)
				{
//					int jIndex = getHogaToIndex(pResv->jprc);
//					double jprc = getTickUnit(pResv->jprc, m_market == FUTURE ? true : false, mmgb, 
//						mmgb == MS_JUMUN ? jIndex - index : index - jIndex);
					const double jprc = getTickUnit(hoga, m_market == FUTURE ? true : false, mmgb, pResv->tick);

					if (jprc > str2double(m_shga) || jprc < str2double(m_hhga))
					{
						m_pParent->SetGuide("주문 가능한 가격이 아닙니다.");
						continue;
					}
					pResv->gprc = hoga;
					pResv->jprc = jprc;
					
					if (pResv->gprc > str2double(m_curr))
						pResv->gubn = aboveCURR;
					else
						pResv->gubn = belowCURR;
				}
			}
			invalidateRC();
		}
	}
}

int CHogaWnd::getHogaToIndex(double hoga)
{
	return 0;
}

void CHogaWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	ClientToScreen(&pt);
	if (m_code.IsEmpty())
		m_mouseDLL.Format("IB300200\n%d\t%d\n", pt.x, pt.y);
	else
		m_mouseDLL.Format("IB300200\n%d\t%d\ned_focod\t%s\n", pt.x, pt.y, m_code);
	m_pView->PostMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (long)m_mouseDLL.operator LPCTSTR());
	
	CWnd::OnRButtonUp(nFlags, point);
}

void CHogaWnd::DrawColorBox(CDC* pDC, CRect rc, BOOL l, BOOL t, BOOL r, BOOL b)
{
	CPen *pHorz = GetAxPen(GetIndexColor(COLOR_LINE), 1, PS_SOLID);
	CPen *pVert = GetAxPen(GetIndexColor(COLOR_LINE), 1, PS_SOLID);

	CPen *pOldPen = pDC->SelectObject(pHorz);
	if (t)
	{
		pDC->MoveTo(rc.left, rc.top);	
		pDC->LineTo(rc.right - 1, rc.top);
	}
	if (b)
	{
		pDC->MoveTo(rc.left, rc.bottom - 1);
		pDC->LineTo(rc.right - 1, rc.bottom - 1);
	}

	pDC->SelectObject(pVert);
	if (l)
	{
		pDC->MoveTo(rc.left, rc.top);	
		pDC->LineTo(rc.left, rc.bottom - 1);
	}
	if (r)
	{
		pDC->MoveTo(rc.right - 1, rc.top);
		pDC->LineTo(rc.right - 1, rc.bottom);
	}

	pDC->SelectObject(pOldPen);
}

CPoint CHogaWnd::GetRect2HogaPoint(CPoint point)
{
	CPoint pt = point;
	ScreenToClient(&point);
	return pt;
}


void CHogaWnd::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TMID_PAINT:
		if (m_bReal)
		{
			invalidateRC();
			m_bReal = false;
		}
		break;
	case TMID_RESERVE:
		KillTimer(TMID_RESERVE);
		//checkReserveJumun(m_rsvCode, m_rsvCurr);
		break;
	}	
	CWnd::OnTimer(nIDEvent);
}


void CHogaWnd::invalidateRC()
{
	InvalidateRect(m_rcWnd);
}

void CHogaWnd::onBtn(int mmgb, bool bReserve)
{
	// 예약취소
	if (bReserve)
	{
		class CReserve *pResv = NULL;
		const int count = m_arReserve.GetSize() - 1;
		for (int ii = count; ii >= 0; ii--)
		{
			pResv = m_arReserve.GetAt(ii);
			if (pResv->mmgb == mmgb)
			{
				delete pResv;	pResv = NULL;
				m_arReserve.RemoveAt(ii);
			}
		}
		invalidateRC();
		return;
	}

	// 주문취소
	CRect sRc, rc;
	GetClientRect(&rc);
	sRc = rc;
	ClientToScreen(&sRc);

	int juno = 0, jmcn = 0, count = 0;
	class COrder *pOrder = NULL;
	CString	strOne;

	m_cs.Lock();
	POSITION pos = m_mapOrder.GetStartPosition();
	while( pos != NULL )
	{
		m_mapOrder.GetNextAssoc(pos, juno, pOrder);
		
		if (mmgb == str2int(pOrder->mmgb))
		{
			jmcn += str2int(pOrder->mcvl);
			count++;
			strOne = pOrder->juno;
		}
	}
	if (count == 0) {
		m_cs.Unlock();
		return;
	}
	if (count > 1)
		strOne.Format("%s 외 %d개", strOne, count - 1);

	if (getConfirmJumun())
	{
		CDialogConfirm dlg(sRc, m_pView, m_pParent);
		dlg.SetData('C', m_pParent->GetAccount(), m_pParent->GetName(), 
			m_pParent->GetCodeName(), jmcn, "", NULL, strOne);
		if (dlg.DoModal() != IDOK)
		{
			m_cs.Unlock();
			return;
		}
	}

	pos = m_mapOrder.GetStartPosition();
	while( pos != NULL )
	{
		m_mapOrder.GetNextAssoc(pos, juno, pOrder);
		
		if (mmgb == str2int(pOrder->mmgb))
		{
			m_pParent->SendJumun("4", 0, str2int(pOrder->juno), 0);
			SetModArrow(4, 0, str2int(pOrder->juno));
		}
	}
	m_cs.Unlock();
	invalidateRC();
}

void CHogaWnd::SetHold(bool bHold)
{
	setScroll(m_depth);
	m_bHold = bHold;
}

CString CHogaWnd::SplitString(CString &strData, CString strToken)
{
	CString sResult = strData;
	if(strToken == "") return sResult;
	const int iFind = strData.Find(strToken); 
	if(iFind > -1 ) 
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
	}
	
	
	return sResult;
}

void CHogaWnd::ChangeFont(CFont* pFont, CFont* pBFont)
{
	m_pFont = pFont;
	m_pBFont = pBFont;
	DrawScreen() ;
	m_mdJmBtn->SetFont(m_pFont, true);
	m_msJmBtn->SetFont(m_pFont, true);
	m_msStopBtn->SetFont(m_pFont, true);
}
