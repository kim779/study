// HogaWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MapWnd.h"
#include "HogaWnd.h"
#include "SelJumunDlg.h"
#include "DialogConfirm.h"
#include "ConfigDlg.h"
#include "RectWnd.h"
#include "SendOrder.h"
#include "ControlWnd.h"
#include "Tool.h"
#include "UserDefine.h"
#include "../../control/fx_grid/memdc.h"
#include "../../h/AxisVar.h"
#include "../../h/axisfire.h"
#include <math.h>
#include "../../h/MistakeStopOrder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_CTRL_SCROLL	0

#define	SCROLL_WIDTH	16
#define HEAD_HEIGHT	18

#define CELL_HOGA	0
#define CELL_VOL	1
#define CELL_JUMUN	2
#define CELL_RESERVE	3
#define CELL_Y			4	//2015.03.26 KSJ 셀의 어디클릭하던지 호가 얻어오게


#define belowCURR	0
#define aboveCURR	1
#define LEFT_START	0

#define COLOR_ARROW	RGB(150, 150, 150)
#define CLR_OPENPRICE	RGB(200,150,100)	//시가
#define CLR_HIGHPRICE	RGB(255,0,0)//RGB(200,100,50)	//고가
#define CLR_LOWPRICE	RGB(0,0,255)//RGB(50,100,200)	//저가
#define CLR_PIVOTPRICE	RGB(0,0,0)	//기준가
#define CLR_SHPRICE		RGB(255,0,0)		//상한가
#define CLR_HHPRICE		RGB(0,0,255)		//하한가
#define CLR_GAJUNGPRICE	RGB(50,180,240)		//가중평균가

/////////////////////////////////////////////////////////////////////////////
// CHogaWnd

CHogaWnd::CHogaWnd(class CTool* pTool, class CControlWnd* pCtrlWnd, CWnd* parent)
{
	m_pParent = (CMapWnd*) parent;
	m_pTool		= pTool;
	m_pCtrlWnd	= pCtrlWnd;

	m_bDragJumun	= false;
	m_bDragResv	= false;
	m_bTracking	= FALSE;

	m_pfont		= NULL;
	m_pfontB	= NULL;

	m_ppenLine	= NULL;
	m_ppenTable	= NULL;


	m_iFocusMmgb	= -1;
	m_iFocusIndex	= -1;
	m_iPosSHGA	= D10SHGA;
	m_iPosHHGA	= D10HHGA;
	m_iPosCHVL	= D10CHVL;		
	m_iPosBBTN	= D10BBTN;	//2012.10.30 KSJ 
	m_iRows		= D10BBTN + 1;
	m_iIndexTip	= 0;

	m_pTooltip	= NULL;
	m_pTrackingWnd	= NULL;

	m_clrBG		= RGB(255, 255, 255);
	m_clrTH		= RGB(255, 255, 255);
	m_clrGH		= RGB(255, 255, 255);
	m_clrSBG	= RGB(255, 255, 255);
	m_clrBBG	= RGB(255, 255, 255);
	m_clrLGFG	= RGB(0, 0, 0);
	m_clrUp		= RGB(255, 0, 0);
	m_clrDown	= RGB(0, 0, 255);
	m_clrYellowBG = RGB(255, 255, 180);
	m_clrNomalBG = RGB(255, 255, 255);


	m_eDepth	= hgDEPTH10;
	m_eFdgb		= hgFOLD;

	RemoveAllJumun();
	RemoveAllReserve();
	RemoveAllArrow();
	m_sendS = "";
	m_bHogaFix = false;
	m_bClearAll = false;
	m_sVIUp = "";
	m_sVIDown = "";


	// 20221201 호가단위 변경
	CString file;
	file.Format("%s\\tab\\axis.ini", m_pTool->GetHome());
	m_bNewHOGA = (BOOL)GetPrivateProfileInt("K2023SyStem", "Newhgunit", 0, file);

	
}

CHogaWnd::~CHogaWnd()
{
	if (m_pTrackingWnd)
	{
		::DestroyWindow(m_pTrackingWnd->m_hWnd);
	}

	RemoveAllJumun();
	RemoveAllReserve();
	RemoveAllArrow();

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
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(UM_HOGAMESSAGE, OnHogaMessage)
	ON_BN_CLICKED(IDC_BTN_MADO_JUMN, OnAllMadoCancel)
	ON_BN_CLICKED(IDC_BTN_MASU_JUMN, OnAllMasuCancel)
	ON_BN_CLICKED(IDC_BTN_MADO_RSVD, OnAllMadoRsvdCancel)
	ON_BN_CLICKED(IDC_BTN_MASU_RSVD, OnAllMasuRsvdCancel)
	ON_BN_CLICKED(IDC_BTN_HOGA_FIX, OnHogaFix)
	ON_BN_CLICKED(IDC_BTN_MADO_MAKET, OnMarketMado)
	ON_BN_CLICKED(IDC_BTN_MASU_MAKET, OnMarketMasu)
END_MESSAGE_MAP()

LRESULT CHogaWnd::OnHogaMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	case UP_RESERVE:
		{
			if (!m_pTool)	break;
			CString datS = CString((char *)lParam);
			CString codeS = m_pTool->Parser(datS, '\t');
			CString currS = datS;
			CheckReserveJumun(codeS, currS);
		}
		break;
	}
	return 0;
}



/////////////////////////////////////////////////////////////////////////////
// CHogaWnd message handlers

int CHogaWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return Initialize();
	//return 0;
}

int CHogaWnd::Initialize()
{
	m_pfont = m_pTool->RequestFont("굴림체", 9, false, 0);
	m_pfontB = m_pTool->RequestFont("굴림체", 9, false, FW_BOLD);

	m_clrBG = m_pTool->GetIndexColor(COLOR_BK);
	m_clrTH = m_pTool->GetIndexColor(COLOR_TABLEHEAD);
	m_clrGH = m_pTool->GetIndexColor(COLOR_GRIDHEAD);
	m_clrSBG = m_pTool->GetIndexColor(119);
	m_clrBBG = m_pTool->GetIndexColor(120);
	m_clrLGFG = m_pTool->GetIndexColor(COLOR_LINE);
	m_clrUp = m_pTool->GetIndexColor(94);
	m_clrDown = m_pTool->GetIndexColor(95);
	m_clrGridHdrTxt = m_pTool->GetIndexColor(COLOR_GRIDHEADTXT);

	m_ppenLine = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_LINE), 1, PS_SOLID);
	m_ppenTable = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_TABLE), 1, PS_SOLID);

	m_pMadoRsvd = std::make_unique<CfxButton>(m_pfont);
	m_pMadoJumn = std::make_unique<CfxButton>(m_pfont);
	m_pMadoMarket = std::make_unique<CfxButton>(m_pfont);
	m_pMasuRsvd = std::make_unique<CfxButton>(m_pfont);
	m_pMasuJumn = std::make_unique<CfxButton>(m_pfont);
	m_pMasuMarket = std::make_unique<CfxButton>(m_pfont);
	m_pHogaFix  = std::make_unique<CfxButton>(m_pfont);

	m_pMadoRsvd->Create("", WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_BTN_MADO_RSVD);
	m_pMadoJumn->Create("", WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_BTN_MADO_JUMN);
	m_pMadoMarket->Create("", WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_BTN_MADO_MAKET);
	m_pMasuJumn->Create("", WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_BTN_MASU_JUMN);
	m_pMasuMarket->Create("", WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_BTN_MASU_MAKET);
	m_pMasuRsvd->Create("", WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_BTN_MASU_RSVD);
	m_pHogaFix->Create("", WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_BTN_HOGA_FIX);
	
	m_pMadoRsvd->Init(bsGENERAL|bsCOLOR|bsTEXT, "예약취소", "예약취소", RGB(0xBC, 0xE6, 0xC5));
	m_pMadoMarket->Init(bsGENERAL|bsCOLOR|bsTEXT, "시장가", "시장가", RGB(0xBC, 0xA0, 0xBC));
	m_pMadoJumn->Init(bsGENERAL|bsCOLOR|bsTEXT, "매도취소", "매도취소", m_clrSBG);
	m_pHogaFix->Init(bsTOGGLE|bsTEXT, "호가고정", "호가고정", m_clrLGFG);
	m_pMasuJumn->Init(bsGENERAL|bsCOLOR|bsTEXT, "매수취소", "매수취소", m_clrBBG);
	m_pMasuMarket->Init(bsGENERAL|bsCOLOR|bsTEXT, "시장가", "시장가", RGB(0xBC, 0xA0, 0xBC));
	m_pMasuRsvd->Init(bsGENERAL|bsCOLOR|bsTEXT, "예약취소", "예약취소", RGB(0xBC, 0xE6, 0xC5));

	m_HandCursorWnd.insert( m_pMadoRsvd->GetSafeHwnd() );
	m_HandCursorWnd.insert( m_pMadoMarket->GetSafeHwnd() );
	m_HandCursorWnd.insert( m_pMadoJumn->GetSafeHwnd() );
	m_HandCursorWnd.insert( m_pMasuJumn->GetSafeHwnd() );
	m_HandCursorWnd.insert( m_pMasuMarket->GetSafeHwnd() );
	m_HandCursorWnd.insert( m_pMasuRsvd->GetSafeHwnd() );

	CString strPath;
	strPath.Format("%s\\%s\\%s\\%s.ini", m_pTool->GetHome(), USRDIR, m_pTool->GetName(), MAP_NAME);
	CConfigDlg	dlg(strPath);
	dlg.InitData();
	m_config = dlg.m_config;

	if (!m_pTooltip)
	{
		[[gsl::suppress(26409)]]
		m_pTooltip = new CToolTipCtrl();
		m_pTooltip->Create(GetDesktopWindow(), TTS_ALWAYSTIP);
		m_pTooltip->SetMaxTipWidth(1000);
		m_pTooltip->Activate(TRUE);
	}

	m_pTrackingWnd = std::make_unique<CRectWnd>(this);
	m_pTrackingWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0);

	CreateScroll();

	SetHogaRect();

	return 0;
}

void CHogaWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);

	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pbitmapOld = memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(rc, RGB(255, 255, 255));

	DrawHead(&memDC);

	if (m_eFdgb == hgFOLD)
	{
		DrawHogaFold(&memDC);
		DrawArrow(&memDC);
	}
	else
	{
		DrawHogaUnfold(&memDC);
		DrawReserve(&memDC);
		DrawArrow(&memDC);
	}

	dc.BitBlt(0, 0, rc.Width() - SCROLL_WIDTH, rc.Height(), &memDC, 0, 0, SRCCOPY);

	memDC.SelectObject(pbitmapOld);
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CHogaWnd::DrawHead(CDC *pDC)
{
	CRect cRc, rc;
	GetClientRect(&rc);

	CPen* ppenOld = pDC->SelectObject(m_ppenTable);
	const int iOldBkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->SelectStockObject(NULL_BRUSH);

	const	int cx = LEFT_START, cy = 0;
//	COLORREF colortxt = 
	if (m_eFdgb == hgFOLD)
	{
		cRc.SetRect(cx, cy, m_aiCol[1], cy + HEAD_HEIGHT);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매    도", cRc, DT_CENTER, 0, m_clrGridHdrTxt);

		cRc.SetRect(m_aiCol[2] - 1, cRc.top, m_aiCol[4], cRc.bottom);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매    수", cRc, DT_CENTER,  0, m_clrGridHdrTxt);
		
		cRc.SetRect(cx, cRc.bottom - 1, m_aiCol[0], cRc.bottom + HEAD_HEIGHT);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "주    문", cRc, DT_CENTER,  0, m_clrGridHdrTxt);

		cRc.SetRect(m_aiCol[0] - 1, cRc.top, m_aiCol[1], cRc.bottom);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "잔    량", cRc, DT_CENTER,  0, m_clrGridHdrTxt);

		cRc.SetRect(m_aiCol[2] - 1, cRc.top, m_aiCol[3], cRc.bottom);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "잔    량", cRc, DT_CENTER,  0, m_clrGridHdrTxt);

		cRc.SetRect(m_aiCol[3] - 1, cRc.top, m_aiCol[4], cRc.bottom);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "주    문", cRc, DT_CENTER,  0, m_clrGridHdrTxt);
		
		m_rcHeadHoga.SetRect(m_aiCol[1] - 1, cy, m_aiCol[2], cRc.bottom);
		pDC->FillSolidRect(&m_rcHeadHoga, m_clrGH);
		pDC->Rectangle(&m_rcHeadHoga);
		DrawSiseText(pDC, "호    가", m_rcHeadHoga, DT_CENTER,  0, m_clrGridHdrTxt);	
		
		DrawColorBox(pDC, CRect(cx, cy, m_aiCol[4], cRc.bottom));
	}
	else
	{
		cRc.SetRect(cx, cy, m_aiCol[0], cy + HEAD_HEIGHT);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "예  약", cRc, DT_CENTER,  0, m_clrGridHdrTxt);

		cRc.SetRect(m_aiCol[0] - 1, cRc.top, m_aiCol[2], cRc.bottom);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매  도", cRc, DT_CENTER,  0, m_clrGridHdrTxt);

		cRc.SetRect(m_aiCol[3] - 1, cRc.top, m_aiCol[5], cRc.bottom);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "매  수", cRc, DT_CENTER,  0, m_clrGridHdrTxt);

		cRc.SetRect(m_aiCol[5] - 1, cRc.top, m_aiCol[6], cRc.bottom);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "예  약", cRc, DT_CENTER,  0, m_clrGridHdrTxt);

		cRc.SetRect(cx, cRc.bottom - 1, m_aiCol[0], cRc.bottom + HEAD_HEIGHT);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "STOP", cRc, DT_CENTER,  0, m_clrGridHdrTxt);

		cRc.SetRect(m_aiCol[0] - 1, cRc.top, m_aiCol[1], cRc.bottom);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "주  문", cRc, DT_CENTER,  0, m_clrGridHdrTxt);

		cRc.SetRect(m_aiCol[1] - 1, cRc.top, m_aiCol[2], cRc.bottom);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "잔  량", cRc, DT_CENTER,  0, m_clrGridHdrTxt);

		cRc.SetRect(m_aiCol[3] - 1, cRc.top, m_aiCol[4], cRc.bottom);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "잔  량", cRc, DT_CENTER,  0, m_clrGridHdrTxt);

		cRc.SetRect(m_aiCol[4] - 1, cRc.top, m_aiCol[5], cRc.bottom);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "주  문", cRc, DT_CENTER,  0, m_clrGridHdrTxt);
	
		cRc.SetRect(m_aiCol[5] - 1, cRc.top, m_aiCol[6], cRc.bottom);
		pDC->FillSolidRect(&cRc, m_clrGH);
		pDC->Rectangle(&cRc);
		DrawSiseText(pDC, "STOP", cRc, DT_CENTER,  0, m_clrGridHdrTxt);
		
		m_rcHeadHoga.SetRect(m_aiCol[2] - 1, cy, m_aiCol[3], cRc.bottom);
		pDC->FillSolidRect(&m_rcHeadHoga, m_clrGH);
		pDC->Rectangle(&m_rcHeadHoga);
		DrawSiseText(pDC, "호  가", m_rcHeadHoga, DT_CENTER,  0, m_clrGridHdrTxt);

		DrawColorBox(pDC, CRect(cx, cy, m_aiCol[6], cRc.bottom));
	}

	pDC->SelectObject(ppenOld);
	pDC->SetBkMode(iOldBkMode);
}

void CHogaWnd::DrawHogaFold(CDC *pDC)
{
	SCROLLINFO	info{};
	CString		string, str, tmpS, data, hoga;
	int		index{}, nPos{};
	CRect		rcCurr, rcHoga;
	CBrush brush;
	const int nGap = 7;	//2012.11.15 KSJ 시고저 표시위한 간격

	rcCurr.SetRectEmpty();
	
	//CPen *pPen = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_TABLE), 1, PS_SOLID);
	CPen *ppenOld = pDC->SelectObject(m_ppenTable);

	const int bkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->SelectStockObject(NULL_BRUSH);
	
	for (int jj = 1; jj <= m_iIndexTip; jj++)
		m_pTooltip->DelTool(this, jj);

	m_iIndexTip = 0;
	
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	nPos = info.nPos;

	//2012.11.05 KSJ 예약이 아닐때 버튼 사이즈 조절
	const CRect rt = CRect(0, 0, 0, 0);

	m_pMadoRsvd->MoveWindow(&rt, FALSE);
	m_pMasuRsvd->MoveWindow(&rt, FALSE);
	// KSJ

	for (int ii = 0; ii < m_iRows; ii++)
	{
		if (m_arHoga.GetSize() > 0 && ii != m_iPosSHGA && ii != m_iPosSHGA && ii != m_iPosHHGA)
		{
			index = nPos + ii - 1;
			if (index < m_arHoga.GetSize())
				hoga = m_arHoga.GetAt(index);
			else
				hoga = "";
		}
		else
			hoga = "";
	
		if (ii == m_iPosBBTN)
		{
			m_pMadoJumn->MoveWindow(m_rcMDJumun[ii], FALSE);
			m_pMadoMarket->MoveWindow(m_rcDvol[ii], FALSE);
			m_pHogaFix->MoveWindow(m_rcHoga[ii], FALSE);
			m_pMasuMarket->MoveWindow(m_rcSvol[ii], FALSE);
			m_pMasuJumn->MoveWindow(m_rcMSJumun[ii], FALSE);
		}
		else if (ii == m_iPosCHVL)
		{
			pDC->FillSolidRect(&m_rcMDJumun[ii], m_clrSBG);
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = GetHogaToJumun(hoga, 1, m_rcMDJumun[ii], true);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);
			DrawSiseText(pDC, "-" + m_strDvol, m_rcDvol[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE);
			
			rcHoga = CRect(m_rcHoga[ii].left, m_rcHoga[ii].top, m_rcHoga[ii].right - nGap, m_rcHoga[ii].bottom);

			pDC->Rectangle(&m_rcHoga[ii]);
// 			DrawSiseText(pDC, m_strDSCha, m_rcHoga[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_BOLD);
			DrawSiseText(pDC, m_strDSCha, rcHoga, DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_BOLD);
			
			pDC->Rectangle(&m_rcSvol[ii]);
			DrawSiseText(pDC, "+" + m_strSvol, m_rcSvol[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE);
			
			pDC->FillSolidRect(&m_rcMSJumun[ii], m_clrBBG);

			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = GetHogaToJumun(hoga, 2, m_rcMSJumun[ii], true);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
		}
		else if (ii == m_iPosSHGA)
		{
			pDC->FillSolidRect(&m_rcMDJumun[ii], m_clrSBG);
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = GetHogaToJumun(m_strShga, 1, m_rcMDJumun[ii]);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);
			DrawSiseText(pDC, "상한", m_rcDvol[ii], DT_RIGHT);
			
			rcHoga = CRect(m_rcHoga[ii].left, m_rcHoga[ii].top, m_rcHoga[ii].right - nGap, m_rcHoga[ii].bottom);

			pDC->Rectangle(&m_rcHoga[ii]);
			data = "1 " + m_strShga;
// 			DrawSiseText(pDC, data, m_rcHoga[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_SHGA | ST_DRYL | ST_BOLD);
			DrawSiseText(pDC, data, rcHoga, DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_SHGA | ST_DRYL | ST_BOLD);
			
			pDC->Rectangle(&m_rcSvol[ii]);		
			pDC->FillSolidRect(&m_rcMSJumun[ii], m_clrBBG);
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = GetHogaToJumun(m_strShga, 2, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
		}
		else if (ii == m_iPosHHGA)
		{
			pDC->FillSolidRect(&m_rcMDJumun[ii], m_clrSBG);
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = GetHogaToJumun(m_strHhga, 1, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);	
			rcHoga = CRect(m_rcHoga[ii].left, m_rcHoga[ii].top, m_rcHoga[ii].right - nGap, m_rcHoga[ii].bottom);

			pDC->Rectangle(&m_rcHoga[ii]);
			data = "4 " + m_strHhga;
//			DrawSiseText(pDC, data, m_rcHoga[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_SHGA | ST_DRYL | ST_BOLD);
			DrawSiseText(pDC, data, rcHoga, DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_SHGA | ST_DRYL | ST_BOLD);
			
			pDC->Rectangle(&m_rcSvol[ii]);
			DrawSiseText(pDC, "하한", m_rcSvol[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMSJumun[ii], m_clrBBG);
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = GetHogaToJumun(m_strHhga, 2, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
		}
		else
		{
			pDC->FillSolidRect(&m_rcMDJumun[ii], m_clrSBG);
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = GetHogaToJumun(hoga, 1, m_rcMDJumun[ii]);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);

			
			pDC->Rectangle(&m_rcDvol[ii]);
			data = GetHogaData(hoga, 1);
			DrawSiseText(pDC, data, m_rcDvol[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE);

			if(Str2int(m_strKoga) >= Str2int(hoga) && Str2int(m_strJega) <= Str2int(hoga))	//고가에서 저가까지 노란색칠하기
			{
				pDC->FillSolidRect(&m_rcHoga[ii], m_clrYellowBG);

				if(m_config.hogaclr)
				{
					if(Str2int(m_strSiga) == Str2int(hoga))
					{
						brush.CreateSolidBrush(CLR_OPENPRICE);
						pDC->FillRect(&CRect(m_rcHoga[ii].right -10, m_rcHoga[ii].top, m_rcHoga[ii].right-4, m_rcHoga[ii].bottom), &brush);
						brush.DeleteObject();
					}
					else if(Str2int(m_strKoga) == Str2int(hoga))
					{
						brush.CreateSolidBrush(CLR_HIGHPRICE);
						pDC->FillRect(&CRect(m_rcHoga[ii].right -10, m_rcHoga[ii].top, m_rcHoga[ii].right-4, m_rcHoga[ii].bottom), &brush);
						brush.DeleteObject();
					}
					else if(Str2int(m_strJega) == Str2int(hoga))
					{
						brush.CreateSolidBrush(CLR_LOWPRICE);
						pDC->FillRect(&CRect(m_rcHoga[ii].right -10, m_rcHoga[ii].top, m_rcHoga[ii].right -4, m_rcHoga[ii].bottom), &brush);
						brush.DeleteObject();
					}
				}
			}
			else
				pDC->FillSolidRect(&m_rcHoga[ii], m_clrNomalBG);
			
			pDC->Rectangle(&m_rcHoga[ii]);

			rcHoga = CRect(m_rcHoga[ii].left, m_rcHoga[ii].top, m_rcHoga[ii].right - nGap, m_rcHoga[ii].bottom);

// 			DrawSiseText(pDC, hoga, m_rcHoga[ii], DT_RIGHT, ST_COMMA | ST_COLOR | ST_REMOVE | ST_DRYL | ST_BOLD);
			
			CRect rcVI, rc;
			rcVI = rcHoga;
			rcVI.right = rcVI.left + 30;
			rc = m_rcHoga[ii];
			rc.DeflateRect(1,1);
		//	if(Str2int(hoga) == Str2int(m_sVIUp) && m_config.showVI)
			if(m_sVIUp != "0" && !m_sVIUp.IsEmpty() && Str2int(hoga) == Str2int(m_sVIUp) && m_config.showVI)
			{
				pDC->FillSolidRect(&rc, RGB(250,190,216));
				DrawSiseText(pDC, "VI", rcVI, DT_CENTER,  ST_COLOR | ST_REMOVE | ST_DRYL | ST_BOLD, RGB(255,0,0));
			}
		//	else if(Str2int(hoga) == Str2int(m_sVIDown) && m_config.showVI)
			else if(m_sVIDown != "0" && !m_sVIDown.IsEmpty() && Str2int(hoga) == Str2int(m_sVIDown) && m_config.showVI)
			{
				pDC->FillSolidRect(&rc, RGB(166,168,251));
				DrawSiseText(pDC, "VI", rcVI, DT_CENTER,  ST_COLOR | ST_REMOVE | ST_DRYL | ST_BOLD, RGB(0,0,255));
			}
			
			DrawSiseText(pDC, hoga, rcHoga, DT_RIGHT, ST_COMMA | ST_COLOR | ST_REMOVE | ST_DRYL | ST_BOLD);
				
			if (Str2int(m_strCurr) == Str2int(hoga) && Str2int(m_strCurr) != 0)
				rcCurr = m_rcHoga[ii];
			
			pDC->Rectangle(&m_rcSvol[ii]);
			data = GetHogaData(hoga, 2);
			DrawSiseText(pDC, data, m_rcSvol[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE);
			
			pDC->FillSolidRect(&m_rcMSJumun[ii], m_clrBBG);
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = GetHogaToJumun(hoga, 2, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
		}
	}
	
	DrawColorBox(pDC, CRect(m_rcMDJumun[0].left, m_rcMDJumun[0].top, m_rcMSJumun[m_iPosCHVL].right, m_rcMSJumun[m_iPosCHVL].bottom));

	pDC->SelectObject(m_pTool->RequestPen(m_pTool->GetIndexColor(86), 1, PS_SOLID));
	pDC->Rectangle(rcCurr);
	
	pDC->SelectObject(ppenOld);
	pDC->SetBkMode(bkMode);		
}

void CHogaWnd::DrawHogaUnfold(CDC *pDC)
{
	SCROLLINFO	info{};
	CString		string, str, tmpS, data, hoga;
	int		index{}, nPos{};
	CRect		rcCurr, rcHoga;
	CBrush brush;
	const int nGap = 7;	//2012.11.15 KSJ 시고저 표시위한 간격

	rcCurr.SetRectEmpty();
	
	//CPen *pPen = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_TABLE), 1, PS_SOLID);
	CPen *ppenOld = pDC->SelectObject(m_ppenTable);
	const int iOldBkMode = pDC->SetBkMode(TRANSPARENT);
	pDC->SelectStockObject(NULL_BRUSH);
	
	for (int jj = 1; jj <= m_iIndexTip; jj++)
		m_pTooltip->DelTool(this, jj);
	m_iIndexTip = 0;
	
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	nPos = info.nPos;
	
	for (int ii = 0; ii < m_iRows; ii++)
	{
		if (m_arHoga.GetSize() > 0 && ii != m_iPosSHGA && ii != m_iPosSHGA && ii != m_iPosHHGA)
		{
			index = nPos + ii - 1;
			if (index < m_arHoga.GetSize())
				hoga = m_arHoga.GetAt(index);
			else
				hoga = "";
		}
		else
			hoga = "";
		
		if (ii == m_iPosBBTN)
		{
			m_pMadoRsvd->MoveWindow(m_rcMDResv[ii], FALSE);
			m_pMadoJumn->MoveWindow(m_rcMDJumun[ii], FALSE);
			m_pMadoMarket->MoveWindow(m_rcDvol[ii], FALSE);
			m_pHogaFix->MoveWindow(m_rcHoga[ii], FALSE);
			m_pMasuMarket->MoveWindow(m_rcSvol[ii], FALSE);
			m_pMasuJumn->MoveWindow(m_rcMSJumun[ii], FALSE);
			m_pMasuRsvd->MoveWindow(m_rcMSResv[ii], FALSE);
		}
		else if (ii == m_iPosCHVL)
		{
			pDC->FillSolidRect(&m_rcMDResv[ii], m_clrSBG);
			pDC->Rectangle(&m_rcMDResv[ii]);
			data = GetReserveData(hoga, 1, m_rcMDResv[ii], true);
			DrawSiseText(pDC, data, m_rcMDResv[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMDJumun[ii], m_clrSBG);
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = GetHogaToJumun(hoga, 1, m_rcMDJumun[ii], true);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);
			DrawSiseText(pDC, "-" + m_strDvol, m_rcDvol[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE);
			
			rcHoga = CRect(m_rcHoga[ii].left, m_rcHoga[ii].top, m_rcHoga[ii].right - nGap, m_rcHoga[ii].bottom);

			pDC->Rectangle(&m_rcHoga[ii]);
// 			DrawSiseText(pDC, m_strDSCha, m_rcHoga[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_BOLD);
			DrawSiseText(pDC, m_strDSCha, rcHoga, DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_BOLD);
			
			pDC->Rectangle(&m_rcSvol[ii]);
			DrawSiseText(pDC, "+" + m_strSvol, m_rcSvol[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE);
			
			pDC->FillSolidRect(&m_rcMSJumun[ii], m_clrBBG);
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = GetHogaToJumun(hoga, 2, m_rcMSJumun[ii], true);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMSResv[ii], m_clrBBG);
			pDC->Rectangle(&m_rcMSResv[ii]);
			data = GetReserveData(hoga, 2, m_rcMSResv[ii], true);
			DrawSiseText(pDC, data, m_rcMSResv[ii], DT_RIGHT);
		}
		else if (ii == m_iPosSHGA)
		{
			pDC->FillSolidRect(&m_rcMDResv[ii], m_clrSBG);
			pDC->Rectangle(&m_rcMDResv[ii]);
			
			pDC->FillSolidRect(&m_rcMDJumun[ii], m_clrSBG);
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = GetHogaToJumun(m_strShga, 1, m_rcMDJumun[ii]);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);
			DrawSiseText(pDC, "상한", m_rcDvol[ii], DT_RIGHT);
			
			rcHoga = CRect(m_rcHoga[ii].left, m_rcHoga[ii].top, m_rcHoga[ii].right - nGap, m_rcHoga[ii].bottom);

			pDC->Rectangle(&m_rcHoga[ii]);
			data = "1 " + m_strShga;
			//DrawSiseText(pDC, data, m_rcHoga[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_SHGA | ST_BOLD);
			DrawSiseText(pDC, data, rcHoga, DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_SHGA | ST_DRYL | ST_BOLD);
			pDC->Rectangle(&m_rcSvol[ii]);		
			pDC->FillSolidRect(&m_rcMSJumun[ii], m_clrBBG);
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = GetHogaToJumun(m_strShga, 2, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMSResv[ii], m_clrBBG);
			pDC->Rectangle(&m_rcMSResv[ii]);
		}
		else if (ii == m_iPosHHGA)
		{
			pDC->FillSolidRect(&m_rcMDResv[ii], m_clrSBG);
			pDC->Rectangle(&m_rcMDResv[ii]);
			
			pDC->FillSolidRect(&m_rcMDJumun[ii], m_clrSBG);
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = GetHogaToJumun(m_strHhga, 1, m_rcMDJumun[ii]);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);		
			rcHoga = CRect(m_rcHoga[ii].left, m_rcHoga[ii].top, m_rcHoga[ii].right - nGap, m_rcHoga[ii].bottom);

			pDC->Rectangle(&m_rcHoga[ii]);
			data = "4 " + m_strHhga;
			//DrawSiseText(pDC, data, m_rcHoga[ii], DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_SHGA | ST_BOLD);
			DrawSiseText(pDC, data, rcHoga, DT_RIGHT, ST_NORMAL | ST_COMMA | ST_COLOR | ST_REMOVE | ST_SHGA | ST_DRYL | ST_BOLD);
			
			pDC->Rectangle(&m_rcSvol[ii]);
			DrawSiseText(pDC, "하한", m_rcSvol[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMSJumun[ii], m_clrBBG);
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = GetHogaToJumun(m_strHhga, 2, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMSResv[ii], m_clrBBG);
			pDC->Rectangle(&m_rcMSResv[ii]);
		}
		else
		{
			pDC->FillSolidRect(&m_rcMDResv[ii], m_clrSBG);
			pDC->Rectangle(&m_rcMDResv[ii]);
			data = GetReserveData(hoga, 1, m_rcMDResv[ii]);
			DrawSiseText(pDC, data, m_rcMDResv[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMDJumun[ii], m_clrSBG);
			pDC->Rectangle(&m_rcMDJumun[ii]);
			data = GetHogaToJumun(hoga, 1, m_rcMDJumun[ii]);
			DrawSiseText(pDC, data, m_rcMDJumun[ii], DT_RIGHT);
			
			pDC->Rectangle(&m_rcDvol[ii]);
			data = GetHogaData(hoga, 1);
			DrawSiseText(pDC, data, m_rcDvol[ii], DT_RIGHT, ST_COMMA | ST_COLOR | ST_REMOVE);
			
			if(Str2int(m_strKoga) >= Str2int(hoga) && Str2int(m_strJega) <= Str2int(hoga))	//KSJ 고가에서 저가까지 노란색칠하기
			{
				pDC->FillSolidRect(&m_rcHoga[ii], m_clrYellowBG);
			
				if(m_config.hogaclr)
				{
					if(Str2int(m_strSiga) == Str2int(hoga))
					{
						brush.CreateSolidBrush(CLR_OPENPRICE);
						pDC->FillRect(&CRect(m_rcHoga[ii].right -10, m_rcHoga[ii].top, m_rcHoga[ii].right-4, m_rcHoga[ii].bottom), &brush);
						brush.DeleteObject();
					}
					else if(Str2int(m_strKoga) == Str2int(hoga))
					{
						brush.CreateSolidBrush(CLR_HIGHPRICE);
						pDC->FillRect(&CRect(m_rcHoga[ii].right -10, m_rcHoga[ii].top, m_rcHoga[ii].right-4, m_rcHoga[ii].bottom), &brush);
						brush.DeleteObject();
					}
					else if(Str2int(m_strJega) == Str2int(hoga))
					{
						brush.CreateSolidBrush(CLR_LOWPRICE);
						pDC->FillRect(&CRect(m_rcHoga[ii].right -10, m_rcHoga[ii].top, m_rcHoga[ii].right -4, m_rcHoga[ii].bottom), &brush);
						brush.DeleteObject();
					}
				}
			}
			else
				pDC->FillSolidRect(&m_rcHoga[ii], m_clrNomalBG);

			pDC->Rectangle(&m_rcHoga[ii]);

			rcHoga = CRect(m_rcHoga[ii].left, m_rcHoga[ii].top, m_rcHoga[ii].right - nGap, m_rcHoga[ii].bottom);
// 			DrawSiseText(pDC, hoga, m_rcHoga[ii], DT_RIGHT, ST_COMMA | ST_COLOR | ST_REMOVE | ST_DRYL | ST_BOLD);

			CRect rcVI, rc;
			rcVI = rcHoga;
			rcVI.right = rcVI.left + 30;
			rc = m_rcHoga[ii];
			rc.DeflateRect(1,1);
			if(Str2int(hoga) == Str2int(m_sVIUp) && m_config.showVI)
			{
				pDC->FillSolidRect(&rc, RGB(250,190,216));
				DrawSiseText(pDC, "VI", rcVI, DT_CENTER,  ST_COLOR | ST_REMOVE | ST_DRYL | ST_BOLD, RGB(255,0,0));
			}
			else if(Str2int(hoga) == Str2int(m_sVIDown) && m_config.showVI)
			{
				pDC->FillSolidRect(&rc, RGB(166,168,251));
				DrawSiseText(pDC, "VI", rcVI, DT_CENTER,  ST_COLOR | ST_REMOVE | ST_DRYL | ST_BOLD, RGB(0,0,255));
			}

			DrawSiseText(pDC, hoga, rcHoga, DT_RIGHT, ST_COMMA | ST_COLOR | ST_REMOVE | ST_DRYL | ST_BOLD);
			if (Str2int(m_strCurr) == Str2int(hoga) && Str2int(m_strCurr) != 0)
				rcCurr = m_rcHoga[ii];
			
			pDC->Rectangle(&m_rcSvol[ii]);
			data = GetHogaData(hoga, 2);
			DrawSiseText(pDC, data, m_rcSvol[ii], DT_RIGHT, ST_COMMA | ST_COLOR | ST_REMOVE);
			
			pDC->FillSolidRect(&m_rcMSJumun[ii], m_clrBBG);
			pDC->Rectangle(&m_rcMSJumun[ii]);
			data = GetHogaToJumun(hoga, 2, m_rcMSJumun[ii]);
			DrawSiseText(pDC, data, m_rcMSJumun[ii], DT_RIGHT);
			
			pDC->FillSolidRect(&m_rcMSResv[ii], m_clrBBG);
			pDC->Rectangle(&m_rcMSResv[ii]);
			data = GetReserveData(hoga, 2, m_rcMSResv[ii]);
			DrawSiseText(pDC, data, m_rcMSResv[ii], DT_RIGHT);
		}
	}	
	
	DrawColorBox(pDC, CRect(m_rcMDResv[0].left, m_rcMDResv[0].top, m_rcMSResv[m_iPosCHVL].right, m_rcMSResv[m_iPosCHVL].bottom));

	pDC->SelectObject(m_pTool->RequestPen(m_pTool->GetIndexColor(86), 1, PS_SOLID));
	
	pDC->Rectangle(rcCurr);
	
	pDC->SelectObject(ppenOld);
	pDC->SetBkMode(iOldBkMode);
}

void CHogaWnd::DrawReserve(CDC *pDC)
{
	CRgn rgn;
	if(m_eDepth == hgDEPTH10)
		rgn.CreateRectRgn(m_rcMDResv[1].left, m_rcMDResv[1].top, m_rcMSResv[20].right, m_rcMSResv[20].bottom);
	else
		rgn.CreateRectRgn(m_rcMDResv[1].left, m_rcMDResv[1].top, m_rcMSResv[10].right, m_rcMSResv[10].bottom);

	pDC->SelectClipRgn(&rgn);

	CPoint		ptStart, ptEnd;

	
	class CReserve *pResv = NULL;

	for (int ii = 0; ii < m_arReserve.GetSize(); ii++)
	{
		pResv = m_arReserve.GetAt(ii);
		GetPointReserve(pResv, ptStart, ptEnd);
		if(pResv->sijang == 1)	//2012.11.15 KSJ 시장가인지 구분
			DrawArrowBox(pDC, ptStart);
		else
			DrawArrowLine(pDC, ptStart, ptEnd);
	}

	pDC->SelectClipRgn(NULL);
}

void CHogaWnd::drawPgdg(CDC *pDC)
{
	if (!m_pCtrlWnd)	return;

	const int pgdg = m_pCtrlWnd->GetPGDG();
	if (pgdg <= 0 || m_arHoga.GetSize() == 0 || pgdg > atoi(m_strShga) || pgdg < atoi(m_strHhga))	return;
	int idx = -1;

	SCROLLINFO	info;
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	const int nPos = info.nPos;

	bool bEqual = false;
	for (int ii = 0; ii < m_arHoga.GetSize(); ii++)
	{
		const int price = Str2int(m_arHoga.GetAt(ii));
		if (price == 0)	continue;

		if (pgdg == price)
		{
			bEqual = true;
			idx = ii;
			break;
		}
		else if (pgdg > price)
		{
			bEqual = false;
			idx = ii;
			break;
		}
	}

	if (idx == -1)	return;

	const int gap = idx - nPos;
	if (gap < 0)	return;
	const CRect cRc = m_rcHoga[gap];
	CRect rcStart;
	if (bEqual)
		rcStart.SetRect(cRc.left, m_rcMDJumun[1].top + (int)(m_height * gap) - 1, cRc.right, m_rcMDJumun[1].top + (int)(m_height * (gap + 1)));
	else
		rcStart.SetRect(cRc.left, m_rcMDJumun[1].top + (int)(m_height * gap) - 1 - (int)(m_height/2), cRc.right, m_rcMDJumun[1].top + (int)(m_height * (gap + 1)- m_height/2));

	rcStart.DeflateRect(0, 4, 0, 3);
	rcStart.right = rcStart.left + 6;

	if (rcStart.top < m_rcMDJumun[m_iPosSHGA].bottom)
		return;
	if (rcStart.bottom > m_rcMDJumun[m_iPosHHGA].top)
		return;

	CPoint pt[4];
	pt[0].x = rcStart.left;	pt[0].y = rcStart.top;
	pt[1].x = rcStart.left;	pt[1].y = rcStart.bottom;
	pt[2].x = rcStart.right;	pt[2].y = rcStart.top + rcStart.Height()/2;
	pt[3].x = rcStart.left;	pt[3].y = rcStart.top;

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0, 65, 15));
	CPen *pOldPen = (CPen *) pDC->SelectObject(&pen);

	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 190, 50));
	CBrush* pOldBrush = (CBrush *) pDC->SelectObject(&brush);

	pDC->Polygon(pt, 4);

	pDC->SelectObject(pOldBrush);
	brush.DeleteObject();
	pDC->SelectObject(pOldPen);
	pen.DeleteObject();
}

void CHogaWnd::DrawArrow(CDC *pDC)
{
	CRgn rgn;
	rgn.CreateRectRgn(m_rcMDJumun[1].left, m_rcMDJumun[1].top, m_rcMSJumun[20].right, m_rcMSJumun[20].bottom);
	pDC->SelectClipRgn(&rgn);
	
	CPoint	ptStart, ptEnd;
//	class CArrow *pArrow = NULL;
	std::shared_ptr<CArrow> pArrow = nullptr;


	for (int ii = 0; ii < m_arArrow.GetSize(); ii++)
	{
		pArrow = m_arArrow.GetAt(ii);
		GetPointArrow(pArrow.get(), ptStart, ptEnd);
		DrawArrowLine3(pDC, ptStart, ptEnd, pArrow->jflg);
	}
	
	pDC->SelectClipRgn(NULL);
}

void CHogaWnd::GetPointArrow(class CArrow* pArrow, CPoint &ptStart, CPoint &ptEnd)
{
	int hoga{};
	SCROLLINFO	info;
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);

	CString price, gprc, jprc;
	int gap{}, gIndex{}, jIndex{};
	const int index = info.nPos;
	if (index < m_arHoga.GetSize())
		hoga = Str2int(m_arHoga.GetAt(index));

	for (int ii = 0; ii < m_arHoga.GetSize(); ii++)
	{
		price = m_arHoga.GetAt(ii);	price.Remove('+');	price.Remove('-');
		gprc.Format("%d", pArrow->oprc);
		jprc.Format("%d", pArrow->mprc);
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

void CHogaWnd::GetPointReserve(class CReserve* pResv, CPoint &ptStart, CPoint &ptEnd)
{
	int hoga{};
	SCROLLINFO	info;
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);

	int gap{}, gIndex{}, jIndex{};
	const int index = info.nPos;
	if (index < m_arHoga.GetSize())
		hoga = Str2int(m_arHoga.GetAt(index));

	for (int ii = 0; ii < m_arHoga.GetSize(); ii++)
	{
		const int price = Str2int(m_arHoga.GetAt(ii));
		if (pResv->gprc == price)
			gIndex = ii;
		if (pResv->jprc == price)
			jIndex = ii;
	}

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

void CHogaWnd::ReceiveSise(char* pData, int len)
{
	if (len != sz_SISEMOD)
		return;

	CString tmpS;
	struct _sise_mod* pMod = (struct _sise_mod*)pData;

	CString codeS = m_pTool->ToString(pMod->codx, sizeof(pMod->codx));
	CString strCode = GetCODX();

	if(codeS.Compare(strCode))
	{
		RemoveAllJumun();
		RemoveAllReserve();
		RemoveAllArrow();
		m_arHoga.RemoveAll();
		
		SetHogaRect();
	}

	for (int ii = 0; ii < 10; ii++)
	{
/*		m_hoga[ii].mdga = m_pTool->ToString(pMod->hoga[ii].mdga, sizeof(pMod->hoga[ii].mdga));
		m_hoga[ii].msga = m_pTool->ToString(pMod->hoga[ii].msga, sizeof(pMod->hoga[ii].msga));
		m_hoga[ii].dvol = m_pTool->ToString(pMod->hoga[ii].dvol, sizeof(pMod->hoga[ii].dvol));
		m_hoga[ii].svol = m_pTool->ToString(pMod->hoga[ii].svol, sizeof(pMod->hoga[ii].svol));
		m_hoga[ii].pdvl = m_pTool->ToString(pMod->hoga[ii].dcnt, sizeof(pMod->hoga[ii].dcnt));
		m_hoga[ii].psvl = m_pTool->ToString(pMod->hoga[ii].scnt, sizeof(pMod->hoga[ii].scnt));
*/
		m_hoga[ii].mdga = m_pTool->ToString(pMod->hoga[ii].mdga, sizeof(pMod->hoga[ii].mdga));
		m_hoga[ii].msga = m_pTool->ToString(pMod->hoga[ii].msga, sizeof(pMod->hoga[ii].msga));
		m_hoga[ii].dvol = m_pTool->ToString(pMod->hoga[ii].dvol, sizeof(pMod->hoga[ii].dvol));
		m_hoga[ii].svol = m_pTool->ToString(pMod->hoga[ii].svol, sizeof(pMod->hoga[ii].svol));
		m_hoga[ii].pdvl = m_pTool->ToString(pMod->hoga[ii].dcnt, sizeof(pMod->hoga[ii].dcnt));
		m_hoga[ii].psvl = m_pTool->ToString(pMod->hoga[ii].scnt, sizeof(pMod->hoga[ii].scnt));	
	}
	m_strDvol = m_pTool->ToString(pMod->dvol, sizeof(pMod->dvol));
	m_strSvol = m_pTool->ToString(pMod->svol, sizeof(pMod->svol));
	m_strDSCha = m_pTool->ToString(pMod->sdch, sizeof(pMod->sdch));

	tmpS = m_pTool->ToString(pMod->jmgb, sizeof(pMod->jmgb));
	if (tmpS == "+")
		m_eMarket = KOSPI;
	else 
		m_eMarket = KOSDAQ;

	const int type = m_pTool->GetCodeType(codeS);
	if      (type == etfType || type == etnType) m_eMarket = ETF;
	else if (type == elwType) m_eMarket = ELW;
	else if (type == kospiType) m_eMarket = KOSPI;
	else if (type == kosdaqType) m_eMarket = KOSDAQ;

	m_strCurr = m_pTool->ToString(pMod->curr, sizeof(pMod->curr));
	m_strDiff = m_pTool->ToString(pMod->diff, sizeof(pMod->diff));
	if (m_strDiff[0] == '1' || m_strDiff[0] == '2')
		m_strJijg.Format("%d", Str2int(m_strCurr) - Str2int(m_strDiff.Mid(1)));
	else
		m_strJijg.Format("%d", Str2int(m_strCurr) + Str2int(m_strDiff.Mid(1)));

	m_strGijun = m_pTool->ToString(pMod->gjga, sizeof(pMod->gjga));
	m_strSiga = m_pTool->ToString(pMod->siga, sizeof(pMod->siga));
	m_strKoga = m_pTool->ToString(pMod->koga, sizeof(pMod->koga));
	m_strJega = m_pTool->ToString(pMod->jega, sizeof(pMod->jega));

	m_strShga = m_pTool->ToString(pMod->shga, sizeof(pMod->shga));
	if (atoi(m_strShga) == 0 && atoi(m_strCurr) != 0 && GetCODX().GetAt(0) == 'J')
		m_strShga = MakeSangHan(atoi(m_strCurr));
	m_strHhga = m_pTool->ToString(pMod->hhga, sizeof(pMod->hhga));
	if (atoi(m_strHhga) == 0 && atoi(m_strCurr) != 0 && GetCODX().GetAt(0) == 'J')
		m_strHhga = MakeHaHan(atoi(m_strCurr));

	RemoveAllJumun();
	CreateHogaSet();


	//SCROLLINFO	info;
	//m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	//if (info.nPos < 0)
	//{
	//	CString strstsr;
	//}

	Invalidate();
}
//#define DF_VEC
void CHogaWnd::ReceiveMiche(char* pcData, int iLen)
{
	struct _miche_mod *pMod = (struct _miche_mod *)pcData;
	int nrec = atoi(m_pTool->ToString(pMod->nrec, sizeof(pMod->nrec)));
	
	if (nrec * L_MICHE_REC + 15 > strlen(pcData))
	{
		CMapWnd* pWnd = (CMapWnd*)GetParent();
		pWnd->SetGuide("잘못된 미체결 데이터 수신. 재조회 하시기 바랍니다");
		return;
	}
#ifdef 	DF_VEC
	for (int ii = 0; ii < nrec; ii++)
	{
//		std::unique_ptr<COrder> order = std::make_unique<COrder>();
		auto& order = _vOrder.emplace_back(std::make_unique<COrder>());
		class COrder* pOrder = order.get();
//		class COrder* pOrder = new COrder;

		pOrder->code = GetCODX();
		pOrder->juno = m_pTool->ToString(pMod->rec[ii].juno, sizeof(pMod->rec[ii].juno));
		pOrder->mmgb = m_pTool->ToString(pMod->rec[ii].odgb, sizeof(pMod->rec[ii].odgb));
		if (pOrder->mmgb.Find("매도") >= 0)
			pOrder->mmgb = "1";
		else if (pOrder->mmgb.Find("매수") >= 0)
			pOrder->mmgb = "2";
		else
		{
			//_vOrder.erase(_vOrder.end());
			continue;
		}

		pOrder->jprc = m_pTool->ToString(pMod->rec[ii].jprc, sizeof(pMod->rec[ii].jprc));
		pOrder->jqty = m_pTool->ToString(pMod->rec[ii].jqty, sizeof(pMod->rec[ii].jqty));
		pOrder->mcvl = m_pTool->ToString(pMod->rec[ii].wqty, sizeof(pMod->rec[ii].wqty));

		const int juno = Str2int(pOrder->juno);
		m_mapOrder.SetAt(juno, pOrder);
	}
#else 
	for (int ii = 0; ii < nrec; ii++)
	{
		class COrder* pOrder = new COrder;

		pOrder->code = GetCODX();
		pOrder->juno = m_pTool->ToString(pMod->rec[ii].juno, sizeof(pMod->rec[ii].juno));
		pOrder->mmgb = m_pTool->ToString(pMod->rec[ii].odgb, sizeof(pMod->rec[ii].odgb));
		if (pOrder->mmgb.Find("매도") >= 0 || pOrder->mmgb.Find("대출상환") >= 0)
			pOrder->mmgb = "1";
		else if (pOrder->mmgb.Find("매수") >= 0 || pOrder->mmgb.Find("대주상환") >= 0)
			pOrder->mmgb = "2";
		else
		{
			delete pOrder;
			continue;
		}

		pOrder->jprc = m_pTool->ToString(pMod->rec[ii].jprc, sizeof(pMod->rec[ii].jprc));
		pOrder->jqty = m_pTool->ToString(pMod->rec[ii].jqty, sizeof(pMod->rec[ii].jqty));
		pOrder->mcvl = m_pTool->ToString(pMod->rec[ii].wqty, sizeof(pMod->rec[ii].wqty));

		const int juno = Str2int(pOrder->juno);
		m_mapOrder.SetAt(juno, pOrder);
	}
#endif
	Invalidate();
}

void CHogaWnd::ReceiveAlert(char* pcData)
{
	CString strAlert = CString(pcData);
	
	CString total_str = strAlert;
	strAlert = SplitString(total_str,"\n");
	while(!strAlert.IsEmpty())
	{
		CString strCODX = m_pTool->Parser(strAlert, '\t');
		if (strCODX != GetCODX())
			return;

		CMap<int, int, CString, CString> mapAlert;
		m_pTool->Parsing(strAlert, mapAlert);
		
		CString	   dvol;                // 매도호가총수량
		CString    svol;                // 매수호가총수량
			CString    pdvl;                // 매도호가총건수
			CString    psvl;                // 매수호가총건수

		bool bCurrChg = false;
		CString strVal;
		if (mapAlert.Lookup(23, strVal))			// 현재가
		{
			if (strVal != m_strCurr)
				bCurrChg = true;
			m_strCurr = strVal;
		}
		if (mapAlert.Lookup(24, strVal))		// 전일대비
		{
			m_strDiff = strVal;
		}
		if (mapAlert.Lookup(29, strVal))
		{
			m_strSiga = strVal;
		}
		if (mapAlert.Lookup(30, strVal))
		{
			m_strKoga = strVal;
		}
		if (mapAlert.Lookup(31, strVal))
		{
			m_strJega = strVal;
		}
		if (mapAlert.Lookup(51, strVal))
		{
			m_hoga[0].mdga = strVal;
		}
		if (mapAlert.Lookup(52, strVal))
			m_hoga[1].mdga = strVal;
		if (mapAlert.Lookup(53, strVal))
			m_hoga[2].mdga = strVal;
		if (mapAlert.Lookup(54, strVal))
			m_hoga[3].mdga = strVal;
		if (mapAlert.Lookup(55, strVal))
			m_hoga[4].mdga = strVal;
		if (mapAlert.Lookup(56, strVal))
			m_hoga[5].mdga = strVal;
		if (mapAlert.Lookup(57, strVal))
			m_hoga[6].mdga = strVal;
		if (mapAlert.Lookup(58, strVal))
			m_hoga[7].mdga = strVal;
		if (mapAlert.Lookup(59, strVal))
			m_hoga[8].mdga = strVal;
		if (mapAlert.Lookup(60, strVal))
			m_hoga[9].mdga = strVal;
		if (mapAlert.Lookup(71, strVal))
			m_hoga[0].msga = strVal;
		if (mapAlert.Lookup(72, strVal))
			m_hoga[1].msga = strVal;
		if (mapAlert.Lookup(73, strVal))
			m_hoga[2].msga = strVal;
		if (mapAlert.Lookup(74, strVal))
			m_hoga[3].msga = strVal;
		if (mapAlert.Lookup(75, strVal))
			m_hoga[4].msga = strVal;
		if (mapAlert.Lookup(76, strVal))
			m_hoga[5].msga = strVal;
		if (mapAlert.Lookup(77, strVal))
			m_hoga[6].msga = strVal;
		if (mapAlert.Lookup(78, strVal))
			m_hoga[7].msga = strVal;
		if (mapAlert.Lookup(79, strVal))
			m_hoga[8].msga = strVal;
		if (mapAlert.Lookup(80, strVal))
			m_hoga[9].msga = strVal;
		if (mapAlert.Lookup(41, strVal))
			m_hoga[0].dvol = strVal;
		if (mapAlert.Lookup(42, strVal))
			m_hoga[1].dvol = strVal;
		if (mapAlert.Lookup(43, strVal))
			m_hoga[2].dvol = strVal;
		if (mapAlert.Lookup(44, strVal))
			m_hoga[3].dvol = strVal;
		if (mapAlert.Lookup(45, strVal))
			m_hoga[4].dvol = strVal;
		if (mapAlert.Lookup(46, strVal))
			m_hoga[5].dvol = strVal;
		if (mapAlert.Lookup(47, strVal))
			m_hoga[6].dvol = strVal;
		if (mapAlert.Lookup(48, strVal))
			m_hoga[7].dvol = strVal;
		if (mapAlert.Lookup(49, strVal))
			m_hoga[8].dvol = strVal;
		if (mapAlert.Lookup(50, strVal))
			m_hoga[9].dvol = strVal;
		if (mapAlert.Lookup(61, strVal))
			m_hoga[0].svol = strVal;
		if (mapAlert.Lookup(62, strVal))
			m_hoga[1].svol = strVal;
		if (mapAlert.Lookup(63, strVal))
			m_hoga[2].svol = strVal;
		if (mapAlert.Lookup(64, strVal))
			m_hoga[3].svol = strVal;
		if (mapAlert.Lookup(65, strVal))
			m_hoga[4].svol = strVal;
		if (mapAlert.Lookup(66, strVal))
			m_hoga[5].svol = strVal;
		if (mapAlert.Lookup(67, strVal))
			m_hoga[6].svol = strVal;
		if (mapAlert.Lookup(68, strVal))
			m_hoga[7].svol = strVal;
		if (mapAlert.Lookup(69, strVal))
			m_hoga[8].svol = strVal;
		if (mapAlert.Lookup(70, strVal))
			m_hoga[9].svol = strVal;
 		if (mapAlert.Lookup(81, strVal))
			m_hoga[0].pdvl = strVal;
		if (mapAlert.Lookup(82, strVal))
			m_hoga[1].pdvl = strVal;
		if (mapAlert.Lookup(83, strVal))
			m_hoga[2].pdvl = strVal;
		if (mapAlert.Lookup(84, strVal))
			m_hoga[3].pdvl = strVal;
		if (mapAlert.Lookup(85, strVal))
			m_hoga[4].pdvl = strVal;
 		if (mapAlert.Lookup(86, strVal))
			m_hoga[5].pdvl = strVal;
		if (mapAlert.Lookup(87, strVal))
			m_hoga[6].pdvl = strVal;
		if (mapAlert.Lookup(88, strVal))
			m_hoga[7].pdvl = strVal;
		if (mapAlert.Lookup(89, strVal))
			m_hoga[8].pdvl = strVal;
		if (mapAlert.Lookup(90, strVal))
			m_hoga[9].pdvl = strVal;
		if (mapAlert.Lookup(91, strVal))
			m_hoga[0].psvl = strVal;
		if (mapAlert.Lookup(92, strVal))
			m_hoga[1].psvl = strVal;
		if (mapAlert.Lookup(93, strVal))
			m_hoga[2].psvl = strVal;
		if (mapAlert.Lookup(94, strVal))
			m_hoga[3].psvl = strVal;
		if (mapAlert.Lookup(95, strVal))
			m_hoga[4].psvl = strVal;
		if (mapAlert.Lookup(96, strVal))
			m_hoga[5].psvl = strVal;
		if (mapAlert.Lookup(97, strVal))
			m_hoga[6].psvl = strVal;
		if (mapAlert.Lookup(98, strVal))
			m_hoga[7].psvl = strVal;
		if (mapAlert.Lookup(99, strVal))
			m_hoga[8].psvl = strVal;
		if (mapAlert.Lookup(100, strVal))
			m_hoga[9].psvl = strVal;
		if (mapAlert.Lookup(101, strVal))
		{
			m_strDvol = strVal;
		}
		if (mapAlert.Lookup(104, strVal))
		{
			m_strDcnt = strVal;
		}
		if (mapAlert.Lookup(106, strVal))
		{
			m_strSvol = strVal;
			m_strDSCha.Format("%d", Str2int(m_strSvol) - Str2int(m_strDvol));
			if (Str2int(m_strDSCha) > 0)
				m_strDSCha = "+" + m_strDSCha;
		}
		if (mapAlert.Lookup(109, strVal))
		{
			m_strScnt = strVal;
		}
		
		if (bCurrChg && m_arReserve.GetSize() > 0)
		{
			m_sendS.Format("%s\t%s", GetCODX(), m_strCurr);
			PostMessage(UM_HOGAMESSAGE, UP_RESERVE, (LPARAM)(LPCTSTR)m_sendS);

			//CheckReserveJumun(GetCODX(), m_strCurr);
		}

		Invalidate();
		strAlert = SplitString(total_str,"\n");
	}
}

void CHogaWnd::ReceiveAlert(struct _alertR* alertR)
{
	if (alertR->code != GetCODX())
		return;
	
	DWORD* data{};
	
	for (int i = alertR->size - 1; i > -1; --i)
	{
		data = (DWORD*)alertR->ptr[i];

		bool bCurrChg = false;

		CString strVal;

		if (data[23])			// 현재가
		{
			strVal = (char*)data[23];

			if (strVal != m_strCurr)
				bCurrChg = true;
			m_strCurr = strVal;
		}
		if (data[24])		// 전일대비
		{
			strVal = (char*)data[24];
			m_strDiff = strVal;
		}
		if (data[29])
		{
			strVal = (char*)data[29];
			m_strSiga = strVal;
		}
		if (data[30])
		{
			strVal = (char*)data[30];
			m_strKoga = strVal;
		}
		if (data[31])
		{
			strVal = (char*)data[31];
			m_strJega = strVal;
		}
		if (data[51])
		{
			strVal = (char*)data[51];
			m_hoga[0].mdga = strVal;
		}
		if (data[52])
		{
			strVal = (char*)data[52];
			m_hoga[1].mdga = strVal;
		}
		if (data[53])
		{
			strVal = (char*)data[53];
			m_hoga[2].mdga = strVal;
		}
		if (data[54])
		{
			strVal = (char*)data[54];
			m_hoga[3].mdga = strVal;
		}
		if (data[55])
		{
			strVal = (char*)data[55];
			m_hoga[4].mdga = strVal;
		}
		if (data[56])
		{
			strVal = (char*)data[56];
			m_hoga[5].mdga = strVal;
		}
		if (data[57])
		{
			strVal = (char*)data[57];
			m_hoga[6].mdga = strVal;
		}
		if (data[58])
		{
			strVal = (char*)data[58];
			m_hoga[7].mdga = strVal;
		}
		if (data[59])
		{
			strVal = (char*)data[59];
			m_hoga[8].mdga = strVal;
		}
		if (data[60])
		{
			strVal = (char*)data[60];
			m_hoga[9].mdga = strVal;
		}
		if (data[71])
		{
			strVal = (char*)data[71];
			m_hoga[0].msga = strVal;
		}
		if (data[72])
		{
			strVal = (char*)data[72];
			m_hoga[1].msga = strVal;
		}
		if (data[73])
		{
			strVal = (char*)data[73];
			m_hoga[2].msga = strVal;
		}
		if (data[74])
		{
			strVal = (char*)data[74];
			m_hoga[3].msga = strVal;
		}
		if (data[75])
		{
			strVal = (char*)data[75];
			m_hoga[4].msga = strVal;
		}
		if (data[76])
		{
			strVal = (char*)data[76];
			m_hoga[5].msga = strVal;
		}
		if (data[77])
		{
			strVal = (char*)data[77];
			m_hoga[6].msga = strVal;
		}
		if (data[78])
		{
			strVal = (char*)data[78];
			m_hoga[7].msga = strVal;
		}
		if (data[79])
		{
			strVal = (char*)data[79];
			m_hoga[8].msga = strVal;
		}
		if (data[80])
		{
			strVal = (char*)data[80];
			m_hoga[9].msga = strVal;
		}
		if (data[41])
		{
			strVal = (char*)data[41];
			m_hoga[0].dvol = strVal;
		}
		if (data[42])
		{
			strVal = (char*)data[42];
			m_hoga[1].dvol = strVal;
		}
		if (data[43])
		{
			strVal = (char*)data[43];
			m_hoga[2].dvol = strVal;
		}
		if (data[44])
		{
			strVal = (char*)data[44];
			m_hoga[3].dvol = strVal;
		}
		if (data[45])
		{
			strVal = (char*)data[45];
			m_hoga[4].dvol = strVal;
		}
		if (data[46])
		{
			strVal = (char*)data[46];
			m_hoga[5].dvol = strVal;
		}
		if (data[47])
		{
			strVal = (char*)data[47];
			m_hoga[6].dvol = strVal;
		}
		if (data[48])
		{
			strVal = (char*)data[48];
			m_hoga[7].dvol = strVal;
		}
		if (data[49])
		{
			strVal = (char*)data[49];
			m_hoga[8].dvol = strVal;
		}
		if (data[50])
		{
			strVal = (char*)data[50];
			m_hoga[9].dvol = strVal;
		}
		if (data[61])
		{
			strVal = (char*)data[61];
			m_hoga[0].svol = strVal;
		}
		if (data[62])
		{
			strVal = (char*)data[62];
			m_hoga[1].svol = strVal;
		}
		if (data[63])
		{
			strVal = (char*)data[63];
			m_hoga[2].svol = strVal;
		}
		if (data[64])
		{
			strVal = (char*)data[64];
			m_hoga[3].svol = strVal;
		}
		if (data[65])
		{
			strVal = (char*)data[65];
			m_hoga[4].svol = strVal;
		}
		if (data[66])
		{
			strVal = (char*)data[66];
			m_hoga[5].svol = strVal;
		}
		if (data[67])
		{
			strVal = (char*)data[67];
			m_hoga[6].svol = strVal;
		}
		if (data[68])
		{
			strVal = (char*)data[68];
			m_hoga[7].svol = strVal;
		}
		if (data[69])
		{
			strVal = (char*)data[69];
			m_hoga[8].svol = strVal;
		}
		if (data[70])
		{
			strVal = (char*)data[70];
			m_hoga[9].svol = strVal;
		}
		if (data[81])
		{
			strVal = (char*)data[81];
			m_hoga[0].pdvl = strVal;
		}
		if (data[82])
		{
			strVal = (char*)data[82];
			m_hoga[1].pdvl = strVal;
		}
		if (data[83])
		{
			strVal = (char*)data[83];
			m_hoga[2].pdvl = strVal;
		}
		if (data[84])
		{
			strVal = (char*)data[84];
			m_hoga[3].pdvl = strVal;
		}
		if (data[85])
		{
			strVal = (char*)data[85];
			m_hoga[4].pdvl = strVal;
		}
		if (data[86])
		{
			strVal = (char*)data[86];
			m_hoga[5].pdvl = strVal;
		}
		if (data[87])
		{
			strVal = (char*)data[87];
			m_hoga[6].pdvl = strVal;
		}
		if (data[88])
		{
			strVal = (char*)data[88];
			m_hoga[7].pdvl = strVal;
		}
		if (data[89])
		{
			strVal = (char*)data[89];
			m_hoga[8].pdvl = strVal;
		}
		if (data[90])
		{
			strVal = (char*)data[90];
			m_hoga[9].pdvl = strVal;
		}
		if (data[91])
		{
			strVal = (char*)data[91];
			m_hoga[0].psvl = strVal;
		}
		if (data[92])
		{
			strVal = (char*)data[92];
			m_hoga[1].psvl = strVal;
		}
		if (data[93])
		{
			strVal = (char*)data[93];
			m_hoga[2].psvl = strVal;
		}
		if (data[94])
		{
			strVal = (char*)data[94];
			m_hoga[3].psvl = strVal;
		}
		if (data[95])
		{
			strVal = (char*)data[95];
			m_hoga[4].psvl = strVal;
		}
		if (data[96])
		{
			strVal = (char*)data[96];
			m_hoga[5].psvl = strVal;
		}
		if (data[97])
		{
			strVal = (char*)data[97];
			m_hoga[6].psvl = strVal;
		}
		if (data[98])
		{
			strVal = (char*)data[98];
			m_hoga[7].psvl = strVal;
		}
		if (data[99])
		{
			strVal = (char*)data[99];
			m_hoga[8].psvl = strVal;
		}
		if (data[100])
		{
			strVal = (char*)data[100];
			m_hoga[9].psvl = strVal;
		}
		if (data[101])
		{
			strVal = (char*)data[101];
			m_strDvol = strVal;
		}
		if (data[104])
		{
			strVal = (char*)data[104];
			m_strDcnt = strVal;
		}
		if (data[106])
		{
			strVal = (char*)data[106];
			m_strSvol = strVal;
			m_strDSCha.Format("%d", Str2int(m_strSvol) - Str2int(m_strDvol));
			if (Str2int(m_strDSCha) > 0)
				m_strDSCha = "+" + m_strDSCha;
		}
		if (data[109])
		{
			strVal = (char*)data[109];
			m_strScnt = strVal;
		}

		if (data[191])
		{	
			strVal = (char*)data[191];
			
			if(strVal == "19")
			{
			//	if(m_strKrgb.Find("유의") < 0)
					m_strKrgb = "유의";	
			}
			else if(strVal == "0")
				m_strKrgb = "정상";
			else 
				m_strKrgb = "";
		}	

		if(data[241])
		{
			m_sVIUp = (char*)data[241];
		}

		if(data[258])
		{
			m_sVIDown = (char*)data[258];
		}
		
		if (bCurrChg && m_arReserve.GetSize() > 0)
		{
			m_sendS.Format("%s\t%s", GetCODX(), m_strCurr);
			PostMessage(UM_HOGAMESSAGE, UP_RESERVE, (LPARAM)(LPCTSTR)m_sendS);
			//CheckReserveJumun(GetCODX(), m_strCurr);
		}

		//2012.11.06 KSJ  호가고정일때
		if (bCurrChg && m_bHogaFix)
		{
			if (IsValidHoga(Str2int(m_strCurr)))
				SetScroll(m_eDepth);
		}
		//KSJ

		Invalidate();
	}
}

void CHogaWnd::ReceiveNotice(char* pcData)
{
	CString strNotice = CString(pcData);
	int iPos = strNotice.Find('\n');
	if (iPos < 0)
		return;

	CString strHead = strNotice.Left(iPos++);
	strNotice = strNotice.Mid(iPos);

	CMap<int, int, CString, CString> mapNotice;
	strHead.Replace('\r', '\t');
	m_pTool->Parsing(strHead, mapNotice);

	CString	strAcc;
	if (!mapNotice.Lookup(ACCN, strAcc))
		return;;
	if (GetAccount() != strAcc)
		return;

	CString strJuNo;
	if (!mapNotice.Lookup(JUNO, strJuNo))
		return;
	const int iJuNo = atoi(strJuNo);
	if (iJuNo < 1)
		return;

	CString strcrType;
	if (mapNotice.Lookup(911, strcrType))
	{
		if(strcrType == _T("05"))
			return;
	}



	m_pTool->Parsing(strNotice, mapNotice);

	CString	strCODX;
	mapNotice.Lookup(CODX, strCODX);
	if (GetCODX() != strCODX)
		return;

	m_cs.Lock();

	CString strValue;
	if (!mapNotice.Lookup(NTKD, strValue))
		return;
	const int iNoticeKind = atoi(strValue);
	
	int iOneJuMunNo = 0;
	int iOkCount = 0;
	switch (iNoticeKind)
	{
	case 12:	//	case  2:    // 정정확인     
	case 13:	//	case  3:    // 취소확인     
		if (mapNotice.Lookup(OJNO, strValue))		// 원주문번호
			iOneJuMunNo = atoi(strValue);
		
		if (mapNotice.Lookup(OKCT, strValue))		// 확정수량
			iOkCount = atoi(strValue);
		break;
	case 2:		//	case 22:    // 정정접수     
	case 3:		//	case 23:    // 취소접수     
		return;
		
	case 11:	//	case  1:    // 체결
	case 1:		//	case 21:    // 접수         
	case 14:	//	case  4:    // 거부         
		//	case 31:    // 접수오류     
		//	case 32:    // 정정접수오류 
		//	case 33:    // 취소접수오류 
		//	case 34:    // 기타접수오류 
		//	case 24:    // 기타접수     		
	default:
		break;
	}


	class COrder* pOrder;
	if (m_mapOrder.Lookup(iJuNo, pOrder))
	{
		if (mapNotice.Lookup(MCCT, strValue))	// 미체결 수량
			pOrder->mcvl = strValue;
	}
	else
	{
		auto& order = _vOrder.emplace_back(std::make_unique<COrder>());
		pOrder = order.get();

		pOrder->code = GetCODX();		// 종모코드
		pOrder->juno = strJuNo;			// 주문번호

		if (mapNotice.Lookup(MMGB, strValue))	// 매매구분
		{
			if (strValue.Find("매수") >= 0)
				pOrder->mmgb = "2";
			else
				pOrder->mmgb = "1";
		}

		if (mapNotice.Lookup(JPRC, strValue))
//			pOrder->jprc.Format("%d", atoi(strValue) / 100);	// 주문가격
			pOrder->jprc.Format("%d", atoi(strValue));			// 주문가격

		if (mapNotice.Lookup(JQTY, strValue))	// 주문수량
			pOrder->jqty = strValue;

		if (mapNotice.Lookup(MCCT, strValue))	// 미체결 수량
			pOrder->mcvl = strValue;

		m_mapOrder.SetAt(iJuNo, pOrder);

		class COrder* pOrderOld{};
		switch (iNoticeKind)	// 정정확인, 취소확인
		{
		case 12:
		case 13:
			if (m_mapOrder.Lookup(iOneJuMunNo, pOrderOld) && iOkCount > 0)
			{
				pOrderOld->mcvl.Format("%d", atoi(pOrderOld->mcvl) - iOkCount);

				if (Str2int(pOrderOld->mcvl) == 0)
				{
					RemoveJumun(iOneJuMunNo);
				}
			}
			break;
		}
	}

	if (Str2int(pOrder->mcvl) == 0)
	{
		RemoveJumun(iJuNo);
	}

	m_cs.Unlock();

	Invalidate();	
}

void CHogaWnd::CreateHogaSet()
{
	m_arHoga.RemoveAll();

	CString hoga, tmpS;
	int price = Str2int(m_strShga);
	hoga.Format("+%d", Str2int(m_strShga));
	int cnt{};
	
	if (m_eDepth == hgDEPTH10)		cnt = 10;
	else
					cnt = 5;	
	for (int ii = 0; ii < cnt; ii++)
	{
		m_arHoga.Add("  ");
	}

	for (int ii = 0; ; ii++)
	{
		m_arHoga.Add(hoga);

		if (price <= Str2int(m_strHhga))
			break;

		price = GetPriceUnit(price, false);
		if (price > Str2int(m_strJijg))
			hoga.Format("+%d", price);
		else if (price < Str2int (m_strJijg))
			hoga.Format("-%d", price);
		else
			hoga.Format("%d", price);
	}
	
	for (int ii = 0; ii < cnt; ii++)
	{
		m_arHoga.Add("  ");
	}

	SetScroll(m_eDepth);
}

int CHogaWnd::GetPriceUnit(int iPrice, bool bPlus)
{
#ifdef DETAIL_HOGA
	if (m_eMarket == KOSPI)
	{
		// (1000원미만 1원단휘 호가 적용 2010.08.02 by lkm 
		if (bPlus)
		{
			if (iPrice < 1000)			iPrice += 1;
			else if (iPrice < 5000)		iPrice += 5;
			else if (iPrice < 10000)	iPrice += 10;
			else if (iPrice < 50000)	iPrice += 50;
			else if (iPrice < 100000)	iPrice += 100;
			else if (iPrice < 500000)	iPrice += 500;
			else if (iPrice >= 500000)	iPrice += 1000;
		}
		else
		{
			if (iPrice <= 1000)         iPrice -= 1;
			else if (iPrice <= 5000)	iPrice -= 5;
			else if (iPrice <= 10000)	iPrice -= 10;
			else if (iPrice <= 50000)	iPrice -= 50;
			else if (iPrice <= 100000)	iPrice -= 100;
			else if (iPrice <= 500000)	iPrice -= 500;
			else if (iPrice > 500000)	iPrice -= 1000;
		}
	}
	else if (m_eMarket == KOSDAQ)
	{
		if (bPlus)
		{
			if (iPrice < 1000)          iPrice += 1;
			else if (iPrice < 5000)		iPrice += 5;
			else if (iPrice < 10000)	iPrice += 10;
			else if (iPrice < 50000)	iPrice += 50;
			else if (iPrice > 50000)	iPrice += 100;
		}
		else
		{
			if (iPrice <= 1000)         iPrice -= 1;
			else if (iPrice <= 5000)	iPrice -= 5;
			else if (iPrice <= 10000)	iPrice -= 10;
			else if (iPrice <= 50000)	iPrice -= 50;
			else if (iPrice > 50000)	iPrice -= 100;
		}
	}
	else if (m_eMarket == ETF || m_eMarket == ELW)
	{
		if (bPlus)
		{
			iPrice += 5;
		}
		else
		{
			iPrice -= 5;
		}
	}
	else 
	{
		if (bPlus)
		{
			if (iPrice < 1000)          iPrice += 1;
			else if (iPrice < 5000)		iPrice += 5;
			else if (iPrice < 10000)	iPrice += 10;
			else if (iPrice < 50000)	iPrice += 50;
			else if (iPrice > 50000)	iPrice += 100;
		}
		else
		{
			if (iPrice <= 1000)         iPrice -= 1;
			else if (iPrice <= 5000)	iPrice -= 5;
			else if (iPrice <= 10000)	iPrice -= 10;
			else if (iPrice <= 50000)	iPrice -= 50;
			else if (iPrice > 50000)	iPrice -= 100;
		}
	}
#endif
#ifdef NEW_HOGA 

	if (m_bNewHOGA == false) {
		if (m_eMarket == KOSPI)
		{
			// (1000원미만 1원단휘 호가 적용 2010.08.02 by lkm 
			if (bPlus)
			{
				if (iPrice < 1000)			iPrice += 1;
				else if (iPrice < 5000)		iPrice += 5;
				else if (iPrice < 10000)	iPrice += 10;
				else if (iPrice < 50000)	iPrice += 50;
				else if (iPrice < 100000)	iPrice += 100;
				else if (iPrice < 500000)	iPrice += 500;
				else if (iPrice >= 500000)	iPrice += 1000;
			}
			else
			{
				if (iPrice <= 1000)         iPrice -= 1;
				else if (iPrice <= 5000)	iPrice -= 5;
				else if (iPrice <= 10000)	iPrice -= 10;
				else if (iPrice <= 50000)	iPrice -= 50;
				else if (iPrice <= 100000)	iPrice -= 100;
				else if (iPrice <= 500000)	iPrice -= 500;
				else if (iPrice > 500000)	iPrice -= 1000;
			}
		}
		else if (m_eMarket == KOSDAQ)
		{
			if (bPlus)
			{
				if (iPrice < 1000)          iPrice += 1;
				else if (iPrice < 5000)		iPrice += 5;
				else if (iPrice < 10000)	iPrice += 10;
				else if (iPrice < 50000)	iPrice += 50;
				else if (iPrice > 50000)	iPrice += 100;
			}
			else
			{
				if (iPrice <= 1000)         iPrice -= 1;
				else if (iPrice <= 5000)	iPrice -= 5;
				else if (iPrice <= 10000)	iPrice -= 10;
				else if (iPrice <= 50000)	iPrice -= 50;
				else if (iPrice > 50000)	iPrice -= 100;
			}
		}
		else if (m_eMarket == ETF)
		{
			if (bPlus)
			{
				if (iPrice >= 2000)
					iPrice += 5;
				else
					iPrice += 1;
			}
			else
			{
				if (iPrice < 2000)
					iPrice -= 1;
				else
					iPrice -= 5;
			}
		}
		else if (m_eMarket == ELW)
		{
			if (bPlus)
			{
				iPrice += 5;
			}
			else
			{
				iPrice -= 5;
			}
		}
		else
		{
			if (bPlus)
			{
				if (iPrice < 1000)          iPrice += 1;
				else if (iPrice < 5000)		iPrice += 5;
				else if (iPrice < 10000)	iPrice += 10;
				else if (iPrice < 50000)	iPrice += 50;
				else if (iPrice > 50000)	iPrice += 100;
			}
			else
			{
				if (iPrice <= 1000)         iPrice -= 1;
				else if (iPrice <= 5000)	iPrice -= 5;
				else if (iPrice <= 10000)	iPrice -= 10;
				else if (iPrice <= 50000)	iPrice -= 50;
				else if (iPrice > 50000)	iPrice -= 100;
			}
		}
	}
	else {

		if (m_eMarket == KOSPI || m_eMarket == KOSDAQ)
		{
			if (bPlus)
			{
				if (iPrice < 2000)			iPrice += 1;
				else if (iPrice < 5000)		iPrice += 5;
				else if (iPrice < 20000)	iPrice += 10;
				else if (iPrice < 50000)	iPrice += 50;
				else if (iPrice < 200000)	iPrice += 100;
				else if (iPrice < 500000)	iPrice += 500;
				else if (iPrice >= 500000)	iPrice += 1000;
			}
			else
			{
				if (iPrice <= 2000)         iPrice -= 1;
				else if (iPrice <= 5000)	iPrice -= 5;
				else if (iPrice <= 20000)	iPrice -= 10;
				else if (iPrice <= 50000)	iPrice -= 50;
				else if (iPrice <= 200000)	iPrice -= 100;
				else if (iPrice <= 500000)	iPrice -= 500;
				else if (iPrice > 500000)	iPrice -= 1000;
			}
		}
		else if (m_eMarket == ETF)
		{
			if (bPlus)
			{
				if (iPrice >= 2000)
					iPrice += 5;
				else
					iPrice += 1;
			}
			else
			{
				if (iPrice <= 2000)
					iPrice -= 1;
				else
					iPrice -= 5;
			}
		}
		else if (m_eMarket == ELW)
		{
			if (bPlus)
			{
				iPrice += 5;
			}
			else
			{
				iPrice -= 5;
			}
		}
		else
		{
			if (bPlus)
			{
				if (iPrice < 1000)          iPrice += 1;
				else if (iPrice < 5000)		iPrice += 5;
				else if (iPrice < 10000)	iPrice += 10;
				else if (iPrice < 50000)	iPrice += 50;
				else if (iPrice > 50000)	iPrice += 100;
			}
			else
			{
				if (iPrice <= 1000)         iPrice -= 1;
				else if (iPrice <= 5000)	iPrice -= 5;
				else if (iPrice <= 10000)	iPrice -= 10;
				else if (iPrice <= 50000)	iPrice -= 50;
				else if (iPrice > 50000)	iPrice -= 100;
			}
		}
	}
#else
	if (m_eMarket == KOSPI)
	{
		if (bPlus)
		{
			if (iPrice < 5000)		iPrice += 5;
			else if (iPrice < 10000)	iPrice += 10;
			else if (iPrice < 50000)	iPrice += 50;
			else if (iPrice < 100000)	iPrice += 100;
			else if (iPrice < 500000)	iPrice += 500;
			else if (iPrice >= 500000)	iPrice += 1000;
		}
		else
		{
			if (iPrice <= 5000)		iPrice -= 5;
			else if (iPrice <= 10000)	iPrice -= 10;
			else if (iPrice <= 50000)	iPrice -= 50;
			else if (iPrice <= 100000)	iPrice -= 100;
			else if (iPrice <= 500000)	iPrice -= 500;
			else if (iPrice > 500000)	iPrice -= 1000;
		}
	}
	else if (m_eMarket == ETF)
	{
		if (bPlus)
		{
			iPrice += 5;
		}
		else
		{
			iPrice -= 5;
		}
	}
	else 
	{
		if (bPlus)
		{
			if (iPrice < 5000)		iPrice += 5;
			else if (iPrice < 10000)	iPrice += 10;
			else if (iPrice < 50000)	iPrice += 50;
			else if (iPrice > 50000)	iPrice += 100;
		}
		else
		{
			if (iPrice <= 5000)		iPrice -= 5;
			else if (iPrice <= 10000)	iPrice -= 10;
			else if (iPrice <= 50000)	iPrice -= 50;
			else if (iPrice > 50000)	iPrice -= 100;
		}
	}
#endif
	return iPrice;
}

int CHogaWnd::GetTickUnit(int price, int mmgb, int count)
{
	if (count == 0)
		count = m_pCtrlWnd->GetTickCount();

	bool bPlus{};
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
		price = GetPriceUnit(price, bPlus);
	}
	return price;
}

double CHogaWnd::Str2double(CString string, bool bABS /*true*/)
{
	string.Remove(',');
	
	if (bABS)
		return fabs(atof(string));
	else
		return atof(string);
}

int CHogaWnd::Str2int(CString string, bool bABS /*true*/)
{
	string.Remove(',');
	
	if (bABS)
		return abs(atoi(string));
	else
		return atoi(string);
}

//void CHogaWnd::DrawSiseText(CDC* pDC, CString strData, CRect rect, UINT nFormat, UINT nType, int iClrIndex)
void CHogaWnd::DrawSiseText(CDC* pDC, CString strData, CRect rect, UINT nFormat, UINT nType, COLORREF clrText)
{
	if (strData.IsEmpty())
		return;

	nFormat = DT_SINGLELINE | DT_VCENTER | nFormat;

	if (strData.GetAt(0) == 'p')
	{
		strData = strData.Mid(1);
		clrText = m_clrLGFG;
		nType = ST_COMMA;
	}

	CFont* pfontOld{};
	if (nType & ST_BOLD)
		pfontOld = pDC->SelectObject(m_pfontB);
	else
		pfontOld = pDC->SelectObject(m_pfont);

	rect.DeflateRect(5, 0);
	if (nType & ST_TEXT)
	{
		pDC->DrawText(strData, rect, nFormat);
		pDC->SelectObject(pfontOld);
		return;
	}

	CString strGiho;
	if (nType & ST_SHGA)
	{
		if (strData[0] == '1')
		{
			strGiho = "↑ ";
			clrText = m_clrUp;
		}
		else if (strData[0] == '2')
		{
			strGiho = "▲ ";
			clrText = m_clrUp;
		}
		else if (strData[0] == '4')
		{
			strGiho = "↓ ";
			clrText = m_clrDown;
		}
		else if (strData[0] == '5')
		{
			strGiho = "▼ ";	
			clrText = m_clrDown;
		}

		strData = strData.Mid(1);
	}

	int iPos=0;
	if (nType & ST_COLOR)
	{
		iPos = strData.Find('+');
		if (iPos >= 0)
			clrText = m_clrUp;

		iPos = strData.Find('-');
		if (iPos >= 0)
			clrText = m_clrDown;
	}

	CString strPercent;
	if (nType & ST_PERCENT)
		strPercent = " %";

	if (nType & ST_REMOVE)
	{
		strData.Remove('+');
		strData.Remove('-');
	}

	CString strValue;
	if (nType & ST_NORMAL)
	{
		CString strPre = _T("");
		iPos = strData.Find('+');
		if (iPos >= 0)
		{
			strPre = "+";
			strData.Remove('+');
		}

		iPos = strData.Find('-');
		if (iPos >= 0)
		{
			strPre = "-";
			strData.Remove('-');
		}
		strData.Remove(' ');

		iPos = strData.Find('.');
		if (iPos >= 0)
			strValue.Format("%.2f", atof(strData));
		else
			strValue.Format("%d", atoi(strData));

		if (atof(strData) == 0.0)
			strData = "";
		else
			strData = strPre + strValue;
	}

	if (nType & ST_COMMA)
	{
		strData = m_pTool->AddComma(strData);	
	}

	if (nType & ST_DRYL)
	{
		CString dryl;
		dryl = CalculatePercent(strData);
		strData += " " + dryl;
	}

	const COLORREF color = pDC->SetTextColor(clrText);
	pDC->DrawText(strGiho + strData + strPercent, rect, nFormat);

	pDC->SelectObject(pfontOld);
	pDC->SetTextColor(color);
}
/*
CString CHogaWnd::SetComma(CString src)
{
	CString temp, floatStr;
	int	ii, jj = -1;

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
*/

void CHogaWnd::SetHogaRect()
{
	CRect rc;
	GetClientRect(&rc);

	int cx = LEFT_START, cy = 0;

	if (m_eFdgb == hgFOLD)
	{
		m_width = (rc.Width() - SCROLL_WIDTH - cx) / 5;

		for (int ii = 0; ii < 5; ii++)
		{
			if (ii == 4)
				m_aiCol[ii] = rc.Width() - SCROLL_WIDTH - cx;
			else 
				m_aiCol[ii] = (int)(m_width * (ii + 1));
		}

		// 2006.02.01 schbang : bold체일때 등락율 표시시 사이즈조절을 위해
		int iXGabF = 0;
		if (m_config.percent)
		{
			iXGabF	 = 16;
		}
		else
		{
			iXGabF	 = 4;
		}
		m_aiCol[0] -= iXGabF;
		m_aiCol[1] -= iXGabF * 2;
		m_aiCol[2] += iXGabF * 2;
		m_aiCol[3] += iXGabF;

	}
	else
	{
		m_width = (double)(rc.Width() - SCROLL_WIDTH - cx) / MAX_COL;

		for (int ii = 0; ii < MAX_COL; ii++)
		{
			if (ii == MAX_COL - 1)
				m_aiCol[ii] = rc.Width() - SCROLL_WIDTH - cx;
			else 
				m_aiCol[ii] = (int)(m_width * (ii + 1));
		}

		//2013.09.11 KSJ 예약컬럼이 보일때도 등락률 표시해줘야함.
		int iXGabF = 0;
		if (m_config.percent)
		{
			iXGabF	 = 15;
		}
		else
		{
			iXGabF	 = 5;
		}
		m_aiCol[0] -= iXGabF - 2;
		m_aiCol[1] -= iXGabF * 2 - 2;
		m_aiCol[2] -= iXGabF * 3 - 2;
		m_aiCol[3] += iXGabF * 3 + 2;
		m_aiCol[4] += iXGabF * 2 + 2;
		m_aiCol[5] += iXGabF + 1;
	}

	cx = LEFT_START;	cy = HEAD_HEIGHT * 2 - 1;
	m_width = (rc.Width() - SCROLL_WIDTH) / 5;
	m_height = (rc.Height() - cy) / (m_iRows * 1.0);

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

	if (m_eFdgb == hgFOLD)
	{
		for (int ii = 0; ii < m_iRows; ii++)
		{
			if (ii == m_iPosSHGA)
			{
				m_rcMDJumun[ii].SetRect(cx, cy, m_aiCol[0], cy + (int)(m_height * (ii + 1)));
				m_rcDvol[ii].SetRect(m_aiCol[0] - 1, cy, m_aiCol[1], cy + (int)(m_height * (ii + 1)));
				m_rcHoga[ii].SetRect(m_aiCol[1] - 1, cy, m_aiCol[2], cy + (int)(m_height * (ii + 1)));
				m_rcSvol[ii].SetRect(m_aiCol[2] - 1, cy, m_aiCol[3], cy + (int)(m_height * (ii + 1)));
				m_rcMSJumun[ii].SetRect(m_aiCol[3] - 1, cy, m_aiCol[4], cy + (int)(m_height * (ii + 1)));
			}
			else if (ii == m_iPosBBTN)
			{
				m_rcMDJumun[ii].SetRect(cx, cy + (int)(m_height * ii) - 1, m_aiCol[0], rc.bottom);
				m_rcDvol[ii].SetRect(m_aiCol[0] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[1], rc.bottom);
				m_rcHoga[ii].SetRect(m_aiCol[1] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[2], rc.bottom);
				m_rcSvol[ii].SetRect(m_aiCol[2] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[3], rc.bottom);
				m_rcMSJumun[ii].SetRect(m_aiCol[3] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[4], rc.bottom);

				m_pMadoJumn->MoveWindow(m_rcMDJumun[ii], TRUE);
				m_pMadoMarket->MoveWindow(m_rcDvol[ii], TRUE);
				m_pHogaFix->MoveWindow(m_rcHoga[ii], TRUE);
				m_pMasuMarket->MoveWindow(m_rcSvol[ii], TRUE);
				m_pMasuJumn->MoveWindow(m_rcMSJumun[ii], TRUE);
			}
			else
			{
				m_rcMDJumun[ii].SetRect(cx, cy + (int)(m_height * ii) - 1, m_aiCol[0], cy + (int)(m_height * (ii + 1)));
				m_rcDvol[ii].SetRect(m_aiCol[0] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[1], cy + (int)(m_height * (ii + 1)));
				m_rcHoga[ii].SetRect(m_aiCol[1] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[2], cy + (int)(m_height * (ii + 1)));
				m_rcSvol[ii].SetRect(m_aiCol[2] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[3], cy + (int)(m_height * (ii + 1)));
				m_rcMSJumun[ii].SetRect(m_aiCol[3] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[4], cy + (int)(m_height * (ii + 1)));
			}
		}
	}
	else
	{
		for (int ii = 0; ii < m_iRows; ii++)
		{
			if (ii == m_iPosSHGA)
			{
				m_rcMDResv[ii].SetRect(cx, cy, m_aiCol[0], cy + (int)(m_height * (ii + 1)));
				m_rcMDJumun[ii].SetRect(m_aiCol[0] - 1, cy, m_aiCol[1], cy + (int)(m_height * (ii + 1)));
				m_rcDvol[ii].SetRect(m_aiCol[1] - 1, cy, m_aiCol[2], cy + (int)(m_height * (ii + 1)));
				m_rcHoga[ii].SetRect(m_aiCol[2] - 1, cy, m_aiCol[3], cy + (int)(m_height * (ii + 1)));
				m_rcSvol[ii].SetRect(m_aiCol[3] - 1, cy, m_aiCol[4], cy + (int)(m_height * (ii + 1)));
				m_rcMSJumun[ii].SetRect(m_aiCol[4] - 1, cy, m_aiCol[5], cy + (int)(m_height * (ii + 1)));
				m_rcMSResv[ii].SetRect(m_aiCol[5] - 1, cy, m_aiCol[6], cy + (int)(m_height * (ii + 1)));
			}
			else if (ii == m_iPosBBTN)
			{
				m_rcMDResv[ii].SetRect(cx, cy + (int)(m_height * ii) - 1, m_aiCol[0], rc.bottom);
				m_rcMDJumun[ii].SetRect(m_aiCol[0] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[1], rc.bottom);
				m_rcDvol[ii].SetRect(m_aiCol[1] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[2], rc.bottom);
				m_rcHoga[ii].SetRect(m_aiCol[2] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[3], rc.bottom);
				m_rcSvol[ii].SetRect(m_aiCol[3] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[4], rc.bottom);
				m_rcMSJumun[ii].SetRect(m_aiCol[4] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[5], rc.bottom);
				m_rcMSResv[ii].SetRect(m_aiCol[5] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[6], rc.bottom);

				m_pMadoRsvd->MoveWindow(m_rcMDResv[ii], TRUE);
				m_pMadoJumn->MoveWindow(m_rcMDJumun[ii], TRUE);
				m_pMadoMarket->MoveWindow(m_rcDvol[ii], TRUE);
				m_pHogaFix->MoveWindow(m_rcHoga[ii], TRUE);
				m_pMasuMarket->MoveWindow(m_rcSvol[ii], TRUE);
				m_pMasuJumn->MoveWindow(m_rcMSJumun[ii], TRUE);
				m_pMasuRsvd->MoveWindow(m_rcMSResv[ii], TRUE);
			}
			else
			{
				m_rcMDResv[ii].SetRect(cx, cy + (int)(m_height * ii) - 1, m_aiCol[0], cy + (int)(m_height * (ii + 1)));
				m_rcMDJumun[ii].SetRect(m_aiCol[0] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[1], cy + (int)(m_height * (ii + 1)));
				m_rcDvol[ii].SetRect(m_aiCol[1] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[2], cy + (int)(m_height * (ii + 1)));
				m_rcHoga[ii].SetRect(m_aiCol[2] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[3], cy + (int)(m_height * (ii + 1)));
				m_rcSvol[ii].SetRect(m_aiCol[3] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[4], cy + (int)(m_height * (ii + 1)));
				m_rcMSJumun[ii].SetRect(m_aiCol[4] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[5], cy + (int)(m_height * (ii + 1)));
				m_rcMSResv[ii].SetRect(m_aiCol[5] - 1, cy + (int)(m_height * ii) - 1, m_aiCol[6], cy + (int)(m_height * (ii + 1)));
			}
		}
	}
}

void CHogaWnd::CreateScroll()
{
	CRect rc;
	GetClientRect(&rc);
		
	m_vbar.Create(WS_CHILD|WS_VISIBLE|SBS_VERT|SBS_RIGHTALIGN, 
		CRect(rc.right - SCROLL_WIDTH, rc.top, rc.right - 1, rc.bottom), this, ID_CTRL_SCROLL);
}

bool CHogaWnd::IsValidHoga(int price)
{
	if (price < 0) 
		return false;

	int ival;
	if (m_eMarket == KOSPI || m_eMarket == KOSDAQ)
	{
			if (price <= 2000)         ival = 1;
			else if (price <= 5000)	 ival = 5;
			else if (price <= 20000)	ival = 10;
			else if (price <= 50000)	ival = 50;
			else if (price <= 200000)	ival = 100;
			else if (price <= 500000)	ival = 500;
			else if (price > 500000)	ival = 1000;
	}
	else if (m_eMarket == ETF)
	{
			if (price <= 2000)
				ival = 1;
			else
				ival = 5;
	}
	else if (m_eMarket == ELW)
	{
		ival = 5;
	}
	else
	{
		if (price <= 1000)         ival = 1;
		else if (price <= 5000)	ival = 5;
		else if (price <= 10000)	ival = 10;
		else if (price <= 50000)	ival = 50;
		else if (price > 50000)	ival = 100;
	}

	if (price % ival != 0)
		return false;

	return true;
}

void CHogaWnd::SetScroll(int nHoga)
{
	CRect rc;
	GetClientRect(&rc);

	int nPos = GetKey(Str2int(m_strCurr));
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

	Invalidate();
}

int CHogaWnd::GetKey(int value)
{
	int hoga = 0;
	for (int ii = 0; ii < m_arHoga.GetSize(); ii++)
	{
		hoga = Str2int(m_arHoga.GetAt(ii));
		if (hoga == value)
			return ii - (m_eDepth == hgDEPTH10 ? 10 : 5);
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
		SetScroll(m_eDepth);		
	}
	else
	{
// 일반주문
		int juno{}, mmgb{}, index = GetPointToIndex(point, CELL_JUMUN, mmgb);
		if (index >= m_iPosSHGA && index <= m_iPosHHGA)
		{
			SCROLLINFO	info;
			m_vbar.GetScrollInfo(&info, SIF_ALL);
			const int nPos = info.nPos;

			const int hoga = GetIndexToHoga(index);
			
			m_cs.Lock();
			POSITION pos = m_mapOrder.GetStartPosition();
			while( pos != NULL )
			{
				m_mapOrder.GetNextAssoc(pos, juno, pOrder);

				if (hoga == Str2int(pOrder->jprc) && mmgb == Str2int(pOrder->mmgb))
				{
					m_bDragJumun = true;
					m_iHogaStart = hoga;
					m_ptStart = point;
					m_iMmgb = mmgb;

					SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
					SetCapture();

					break;
				}
			}
			m_cs.Unlock();
		}
		else if (index == m_iPosCHVL)
		{
			m_cs.Lock();
			POSITION pos = m_mapOrder.GetStartPosition();
			while( pos != NULL )
			{
				m_mapOrder.GetNextAssoc(pos, juno, pOrder);
				
				if (mmgb == Str2int(pOrder->mmgb))
				{
					m_bDragJumun = true;
					m_iHogaStart = 0;
					m_ptStart = point;
					m_iMmgb = mmgb;
					
					SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
					SetCapture();
					
					break;
				}
			}
			m_cs.Unlock();
		}
// 예약매매 
		index = GetPointToIndex(point, CELL_RESERVE, mmgb);
		class CReserve *pResv = NULL;
		
		if (index > m_iPosSHGA && index < m_iPosHHGA)
		{
			SCROLLINFO	info;
			m_vbar.GetScrollInfo(&info, SIF_ALL);
			const int nPos = info.nPos;

			const int hoga = GetIndexToHoga(index);
			
			for (int ii = 0; ii < m_arReserve.GetSize(); ii++)
			{
				pResv = m_arReserve.GetAt(ii);
				if (hoga == pResv->gprc && pResv->mmgb == mmgb)
				{
					m_bDragResv = true;
					m_iHogaStart = hoga;
					m_ptStart = point;
					m_iMmgb = mmgb;

					SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
					SetCapture();
				}
			}
		}
		else if (index == m_iPosCHVL)
		{
			for (int ii = 0; ii < m_arReserve.GetSize(); ii++)
			{
				pResv = m_arReserve.GetAt(ii);
				if (pResv->mmgb == mmgb)
				{
					m_bDragResv = true;
					m_iHogaStart = 0;
					m_ptStart = point;
					m_iMmgb = mmgb;

					SetCursor(LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_HAND)));
					SetCapture();
				}
			}
		}
	}

	
	CWnd::OnLButtonDown(nFlags, point);
}

void CHogaWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect sRc, rc;
	GetClientRect(&rc);
	sRc = rc;
	ClientToScreen(&sRc);

	CString price, priceFrom, priceTo, junox, junos = "";

	if (m_bDragJumun)
	{
		m_bDragJumun = false;
		ReleaseCapture();
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		if (!CheckJumunAvailable(false))
			return;

		if (rc.PtInRect(point))		// 정정
		{
			int juno{}, mmgb{};
			const int index = GetPointToIndex(point, CELL_JUMUN, mmgb);
			class COrder *pOrder = NULL;

			const int hoga = GetIndexToHoga(index);
			priceFrom.Format("%d", m_iHogaStart);	
			priceFrom = m_pTool->AddComma(priceFrom);
			priceTo.Format("%d", hoga);
			priceTo = m_pTool->AddComma(priceTo);
			if (m_iHogaStart == 0)
				price.Format("%s원", priceTo);
			else
				price.Format("%s원 -> %s원", priceFrom, priceTo);
			
			if (index >= 0 && m_iMmgb == mmgb && hoga != m_iHogaStart)
			{
				if (hoga == 0)
				{
					SetGuide("주문 가능한 가격이 아닙니다.");
					return;
				}

				CSelJumunDlg dlg(this);
				CPoint	pt = point;
				ClientToScreen(&pt);
				dlg.Init(pt);

				int count = 0;
				m_cs.Lock();
				POSITION pos = m_mapOrder.GetStartPosition();
//				CString strOne;
				while( pos != NULL )
				{
					m_mapOrder.GetNextAssoc(pos, juno, pOrder);
					
					if (m_iHogaStart == 0 && mmgb == Str2int(pOrder->mmgb))
					{
						dlg.SetList(pOrder->juno, pOrder->mcvl);
						junos += pOrder->juno + "\t";
						count++;
//						strOne = pOrder->juno;
					}
					else if (m_iHogaStart == Str2int(pOrder->jprc) && mmgb == Str2int(pOrder->mmgb))
					{
						dlg.SetList(pOrder->juno, pOrder->mcvl);
						junos += pOrder->juno + "\t";
						count++;
//						strOne = pOrder->juno;
					}
				}
//				if (count > 1)
//					strOne.Format("%s 외 %d개", strOne, count - 1);
				m_cs.Unlock();

				if (m_config.mod == 0 || count == 1)
				{
					//2014.05.22 KSJ 주문착오방지 적용
					MistakeOrder misOdr;
					
					misOdr.strCode = GetCODX();
					misOdr.lPrice = hoga;
					misOdr.lQuantity = GetJumunCount(junos, mmgb);
					misOdr.nMmgb = mmgb;
					misOdr.strHogb.Format("%c%c", m_pCtrlWnd->GetJumunCondition().GetAt(0), '0');
					misOdr.strCurPrc = m_strCurr;
					misOdr.strMaxPrc = m_strShga;
					misOdr.strMinPrc = m_strHhga;
					misOdr.strListedQuantity = GetListedQuantity();
					misOdr.strStdPrc = m_strGijun;
					misOdr.iMarketType = m_eMarket; //20200904 ETF, ETN 주문확인
					misOdr.strKrgb = m_strKrgb;
					misOdr.strKrgb += "\t";
					misOdr.strKrgb += m_strMkms;
					
					const bool bRet = CheckupMistakeOrder(this, misOdr);
					
					if (!bRet)
						return;
					//KSJ END 

					if (GetConfirmJumun())
					{
						CDialogConfirm cdlg(sRc, m_pTool->GetWizard(), m_pParent);
						cdlg.SetData('F', GetAccount(), GetName(), GetCodeName(), GetJumunCount(junos, mmgb), price, NULL, dlg.GetJunoString());
						if (cdlg.DoModal() != IDOK)
						{
							return;
						}
					}
					while (!junos.IsEmpty())
					{
						junox = m_pTool->Parser(junos, '\t');
						SendJumun('3', hoga, Str2int(junox), 0);
						SetModArrow(3, hoga, Str2int(junox));
					}
				}
				else if (dlg.DoModal() == IDOK)
				{
					if (!dlg.GetJumunCount())
						return;

					junos = dlg.GetJuno();

					//2014.05.22 KSJ 주문착오방지 적용
					MistakeOrder misOdr;
					
					misOdr.strCode = GetCODX();
					misOdr.lPrice = hoga;
					misOdr.lQuantity = GetJumunCount(junos, mmgb);
					misOdr.nMmgb = mmgb;
					misOdr.strHogb.Format("%c%c", m_pCtrlWnd->GetJumunCondition().GetAt(0), '0');
					misOdr.strCurPrc = m_strCurr;
					misOdr.strMaxPrc = m_strShga;
					misOdr.strMinPrc = m_strHhga;
					misOdr.strListedQuantity = GetListedQuantity();
					misOdr.strStdPrc = m_strGijun;
					misOdr.iMarketType = m_eMarket; //20200904 ETF, ETN 주문확인
					misOdr.strKrgb = m_strKrgb;
					misOdr.strKrgb += "\t";
					misOdr.strKrgb += m_strMkms;
					
					const bool bRet = CheckupMistakeOrder(this, misOdr);
					
					if (!bRet)
						return;
					//KSJ END 

					if (GetConfirmJumun())
					{
						CDialogConfirm cdlg(sRc, m_pTool->GetWizard(), m_pParent);
						cdlg.SetData('F', GetAccount(), GetName(), GetCodeName(), GetJumunCount(junos, mmgb), price, NULL, dlg.GetJunoString());
						if (cdlg.DoModal() != IDOK)
						{
							return;
						}
					}

					while (!junos.IsEmpty())
					{
						junox = m_pTool->Parser(junos, '\t');
						SendJumun('3', hoga, Str2int(junox), 0);
						SetModArrow(3, hoga, Str2int(junox));
					}
				}
			}
		}
		else				// 취소
		{
			AllJumnCancel(BASIC_JUMUN);
		}
	}
	else if (m_bDragResv)
	{
		m_bDragResv = false;

		ReleaseCapture();
		SetCursor(LoadCursor(NULL, IDC_ARROW));

		if (rc.PtInRect(point))		// 정정
		{
			int mmgb=0;
			const int index = GetPointToIndex(point, CELL_RESERVE, mmgb);
			class CReserve *pResv = NULL;

			const int hoga = GetIndexToHoga(index);
			if (hoga == 0)	
			{
				SetGuide("감시 가능한 가격이 아닙니다.");
				return;
			}

			if (hoga == Str2int(m_strCurr))
			{
				SetGuide("현재가에서는 예약주문을 할 수 없습니다.");
				return;
			}

			if (((index > m_iPosSHGA && index < m_iPosHHGA) || index == m_iPosCHVL) && m_iMmgb == mmgb && hoga != m_iHogaStart)
			{
				const int count = m_arReserve.GetSize() - 1;
				for (int ii = count; ii >= 0; ii--)
				{
					pResv = m_arReserve.GetAt(ii);
					if (m_iHogaStart == pResv->gprc && pResv->mmgb == mmgb)
					{
						int gb = 0;
						const int jIndex = GetPointToIndex(m_ptStart, CELL_RESERVE, gb);
						const int jprc = GetTickUnit(pResv->jprc, mmgb, mmgb == MS_JUMUN ? jIndex - index : index - jIndex);

						if (jprc > Str2int(m_strShga) || jprc < Str2int(m_strHhga))
						{
							SetGuide("주문 가능한 가격이 아닙니다.");
							return;
						}
						pResv->gprc = hoga;
						pResv->jprc = jprc;

						if (pResv->gprc > Str2int(m_strCurr))
							pResv->gubn = aboveCURR;
						else
							pResv->gubn = belowCURR;
					}
				}
				Invalidate();
			}
		}
		else				// 취소
		{
			AllRsvdCancel(BASIC_JUMUN);
		}
	}
	else 
	{
		//2015.03.26 KSJ 모든셀을 클릭했을때 가격세팅해줌.
		int mmgb = 0;
		const int index = GetPointToIndex(point, CELL_Y, mmgb);
		const int hoga = GetIndexToHoga(index);
		CString jmdg;
		jmdg.Format("%d", hoga);
		jmdg = m_pTool->AddComma(jmdg);
		m_pCtrlWnd->SetJmdg(jmdg);
	}
	m_bDragJumun = false;
	m_bDragResv = false;

	Invalidate();

	if(m_pCtrlWnd->GetOneClickJumun())
	{
		Click_Order(point);
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CHogaWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rc;
	GetClientRect(&rc);

	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	int mmgb{}, index{};
	int hoga{};
	if (GetPointToIndex(point, CELL_JUMUN, mmgb) >= 0)
	{
		index = GetPointToIndex(point, CELL_JUMUN, mmgb);
		hoga = GetIndexToHoga(index);

		if (mmgb == MD_JUMUN)
		{
			m_rcFocus.SetRect(m_rcMDJumun[index].left, m_rcMDJumun[index].top, m_rcHoga[index].right, m_rcHoga[index].bottom);
			m_iFocusMmgb = 1;
			m_iFocusIndex = index;
		}
		else
		{
			m_rcFocus.SetRect(m_rcHoga[index].left, m_rcHoga[index].top, m_rcMSJumun[index].right, m_rcMSJumun[index].bottom);
			m_iFocusMmgb = 2;
			m_iFocusIndex = index;
		}

		if (index != m_iPosCHVL)
			m_pCtrlWnd->SetPrice(mmgb, (int)hoga);
	}
	else if (GetPointToIndex(point, CELL_RESERVE, mmgb) >= 0)
	{
		index = GetPointToIndex(point, CELL_RESERVE, mmgb);
		hoga = GetIndexToHoga(index);

		if (mmgb == MD_JUMUN)
		{
			m_rcFocus.SetRect(m_rcMDResv[index].left, m_rcMDResv[index].top, m_rcHoga[index].right, m_rcHoga[index].bottom);
			m_iFocusMmgb = 1;
			m_iFocusIndex = index;
		}
		else
		{
			m_rcFocus.SetRect(m_rcHoga[index].left, m_rcHoga[index].top, m_rcMSResv[index].right, m_rcMSResv[index].bottom);
			m_iFocusMmgb = 2;
			m_iFocusIndex = index;
		}

		if (index != m_iPosCHVL)
			m_pCtrlWnd->SetPrice(mmgb, (int)hoga);
	}
	else
	{
		m_rcFocus.SetRectEmpty();
		m_iFocusIndex = -1;
	}

	if (m_bDragJumun)
	{
		if (GetPointToIndex(point, CELL_JUMUN, mmgb) >= 0 && mmgb == m_iMmgb)
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
		if (GetPointToIndex(point, CELL_RESERVE, mmgb) >= 0 && mmgb == m_iMmgb)
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

	if (m_iFocusMmgb == 1)
		m_pTrackingWnd->m_color = 1;
	else
		m_pTrackingWnd->m_color = 2;
	m_pTrackingWnd->MoveWindow(m_rcFocus);
	
	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CHogaWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking	= FALSE;

	m_rcFocus.SetRectEmpty();
	m_iFocusIndex = -1;

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
	if(m_pCtrlWnd->GetOneClickJumun())
		return;

	Click_Order(point);

	CWnd::OnLButtonDblClk(nFlags, point);
}

CString CHogaWnd::GetHogaData(CString hoga, int mmgb)
{
	if (hoga.IsEmpty() || hoga.GetLength() <= 0 || Str2int(hoga) == 0)
		return "";
	CString data = "";

	for (int ii = 0; ii < 10; ii++)
	{
		if (mmgb == 1)		// 매도
		{
			if (Str2int(m_hoga[ii].mdga) == Str2int(hoga))
				data = m_hoga[ii].dvol;
		}
		else if (mmgb == 2)	// 매수
		{
			if (Str2int(m_hoga[ii].msga) == Str2int(hoga))
				data = m_hoga[ii].svol;	
		}
	}
	return data;
}

int CHogaWnd::GetIndexToHoga(int index)
{
	if (index < 0 || index >= m_arHoga.GetSize())	return 0;

	SCROLLINFO	info;
	m_vbar.GetScrollInfo(&info, SIF_ALL | SIF_DISABLENOSCROLL);
	const int nPos = info.nPos;	
	
	if (index == m_iPosSHGA)
		return Str2int(m_strShga);
	else if (index == m_iPosHHGA)
		return Str2int(m_strHhga);
	else if (index == m_iPosCHVL)
		return 0;
	else
		return Str2int(m_arHoga.GetAt(nPos + index - 1));
}

CString CHogaWnd::GetHogaToJumun(CString hoga, int mmgb, CRect rc, bool bTotal)
{
	if (Str2int(hoga) == 0 && !bTotal)
		return "";

	CString data, no, vol, tip = "";
	int	juno{}, jvol = 0, jcnt = 0;
	class COrder* pOrder{};
	
	m_cs.Lock();
	POSITION pos = m_mapOrder.GetStartPosition();
	while( pos != NULL )
	{
		m_mapOrder.GetNextAssoc(pos, juno, pOrder);

		if (bTotal)
		{
			if (Str2int(pOrder->mmgb) == 1)
				TRACE("TOTALDATA : %s\n", pOrder->code);

			if (mmgb == Str2int(pOrder->mmgb) && GetCODX() == pOrder->code)
			{
				jvol += Str2int(pOrder->mcvl);
				jcnt++;
				no.Format("%10d", Str2int(pOrder->juno));	//2015.05.28 KSJ 번호 %05d -> %10d
				vol.Format("%9d", Str2int(pOrder->mcvl));
				tip += no + vol + "\n";
			}
		}
		else
		{
			if (Str2int(hoga) == Str2int(pOrder->jprc) && mmgb == Str2int(pOrder->mmgb))
			{
				jvol += Str2int(pOrder->mcvl);
				jcnt++;
				no.Format("%10d", Str2int(pOrder->juno));	//2015.05.28 KSJ 번호 %05d -> %10d
				vol.Format("%9d", Str2int(pOrder->mcvl));
				tip += no + vol + "\n";
			}
		}
	}
	m_cs.Unlock();
	if (!tip.IsEmpty())
	{
		tip = tip.Left(tip.GetLength() - 1);
		tip = " 주문번호   수량\n" + tip;
	}
	
	if (m_config.jmcn == 0)
		data.Format("%d (%d)", jvol, jcnt);
	else
		data.Format("%d", jvol);

	if (jcnt > 0)
	{
		if (m_pTooltip)
			m_pTooltip->AddTool(this, tip, rc, ++m_iIndexTip);

		return data;
	}
	else
		return "";
}

CString CHogaWnd::GetReserveData(CString hoga, int mmgb, CRect rc, bool bTotal)
{
	if (Str2int(hoga) == 0 && !bTotal)
		return "";

	int	jvol = 0, jcnt = 0;
	CString data, tip = "", text = "";
	for (int ii = 0; ii < m_arReserve.GetSize(); ii++)
	{
		class CReserve *pResv = m_arReserve.GetAt(ii);
		if (bTotal)
		{
			if (mmgb == pResv->mmgb && GetCODX() == pResv->code)
			{
				jvol += pResv->jqty;
				jcnt++;
				if(pResv->sijang == 1)	// 시장가 추가
					text.Format("%d. 현재가가 %d 원이면 시장가에 %d 주 주문\n", jcnt, pResv->gprc, pResv->jqty);
				else if (pResv->gubn == aboveCURR)
					text.Format("%d. 현재가가 %d 원보다 같거나 크면 %d 원에 %d 주 주문\n", jcnt, pResv->gprc, pResv->jprc, pResv->jqty);
				else
					text.Format("%d. 현재가가 %d 원보다 같거나 작으면 %d 원에 %d 주 주문\n", jcnt, pResv->gprc, pResv->jprc, pResv->jqty);
				tip += text;
			}
		}
		else
		{
			if (mmgb == pResv->mmgb && pResv->gprc == Str2int(hoga) && GetCODX() == pResv->code)
			{
				jvol += pResv->jqty;
				jcnt++;
				if(pResv->sijang == 1)	//시장가 추가
					text.Format("%d. 현재가가 %d 원이면 시장가에 %d 주 주문\n", jcnt, pResv->gprc, pResv->jqty);
				else if (pResv->gubn == aboveCURR)
					text.Format("%d. 현재가가 %d 원보다 같거나 크면 %d 원에 %d 주 주문\n", jcnt, pResv->gprc, pResv->jprc, pResv->jqty);
				else
					text.Format("%d. 현재가가 %d 원보다 같거나 작으면 %d 원에 %d 주 주문\n", jcnt, pResv->gprc, pResv->jprc, pResv->jqty);
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
		if (m_pTooltip)
			m_pTooltip->AddTool(this, tip, rc, ++m_iIndexTip);

		return data;
	}
	else
		return "";
}

void CHogaWnd::RemoveJumun(int juno)
{
	m_cs.Lock();

	class COrder* pOrder = NULL;
	m_mapOrder.Lookup(juno, pOrder);
		
	if (pOrder)
	{
//		delete pOrder;
		m_mapOrder.RemoveKey(juno);
	}

	for (int ii = 0; ii < m_arArrow.GetSize(); ii++)
	{
		std::shared_ptr<CArrow> pArrow = m_arArrow.GetAt(ii);
//		class CArrow *pArrow = m_arArrow.GetAt(ii);
		if (juno == pArrow->ojno)
		{
			m_arArrow.RemoveAt(ii);
			//delete pArrow;
			pArrow.reset();
			break;
		}
	}

	m_cs.Unlock();
}

void CHogaWnd::RemoveAllJumun()
{
	m_cs.Lock();

	int	juno=0;
	class COrder* pOrder{};
	POSITION pos = m_mapOrder.GetStartPosition();
	while( pos != NULL )
	{
		m_mapOrder.GetNextAssoc(pos, juno, pOrder);
		//delete pOrder;
		m_mapOrder.RemoveKey(juno);
	}
	m_mapOrder.RemoveAll();

	_vOrder.clear();

	m_cs.Unlock();
}

void CHogaWnd::RemoveAllReserve()
{
	for (int ii = 0; ii < m_arReserve.GetSize(); ii++)
	{
		class CReserve *pResv = m_arReserve.GetAt(ii);

		delete pResv;
	}

	m_arReserve.RemoveAll();
}

void CHogaWnd::RemoveAllArrow()
{
	for (int ii = 0; ii < m_arArrow.GetSize(); ii++)
	{
		std::shared_ptr pArrow = m_arArrow.GetAt(ii);
		//class CArrow *pArrow = m_arArrow.GetAt(ii);
		//delete pArrow;
		pArrow.reset();
	}

	m_arArrow.RemoveAll();
}

int CHogaWnd::GetPointToIndex(CPoint point, int cell, int& mmgb)
{
	int index = -1;

	for (int ii = 0; ii < m_iRows; ii++)
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
		case CELL_Y:
			rcMD = m_rcHoga[ii];
			rcMS = m_rcHoga[ii];
			point.x = rcMD.left;
			break;
		}

		//2015.03.26 KSJ 모든셀을 클릭했을때 가격입력
		if ((cell == CELL_Y) && rcMD.PtInRect(point))
		{
			mmgb = 1;
			index = ii;
			break;
		}
		else if (rcMD.PtInRect(point))
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

//CString strTemp;
//strTemp.Format("[KSJ] GetPointToIndex mmgb[%d], index[%d]", mmgb, index);
//OutputDebugString(strTemp);

	return index;
}

BOOL CHogaWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
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
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CHogaWnd::SetHogaDepth(int depth)
{
	if (m_eDepth == depth)
		return;

	CRect	rc;
	GetClientRect(&rc);

	m_eDepth = (depth == 0) ? hgDEPTH10 : hgDEPTH5;

	if (m_eDepth == hgDEPTH10) 
	{
		m_iPosSHGA = D10SHGA;
		m_iPosHHGA = D10HHGA;
		m_iPosCHVL = D10CHVL;
		m_iPosBBTN = D10BBTN;
	}
	else
	{
		m_iPosSHGA = D5SHGA;
		m_iPosHHGA = D5HHGA;
		m_iPosCHVL = D5CHVL;
		m_iPosBBTN = D5BBTN;
	}
	m_iRows = m_iPosBBTN + 1;
	SetHogaRect();
	SetScroll(m_eDepth);
}

bool CHogaWnd::GetConfirmJumun()
{
	if(m_bClearAll)
		return false;
	else
		return m_config.confirm ? true : false;
}

bool CHogaWnd::CheckJumunAvailable(bool bCheckJmcn)
{
	CString acc, pswd, code;
	acc = GetAccount();
	pswd = GetPassword();
	code = GetCODX();
	const int jmcn = m_pCtrlWnd->GetJumunCount();

	if (acc.IsEmpty())
	{
		SetGuide("계좌번호를 확인하십시오.");
		return false;
	}

	if (pswd.IsEmpty())
	{
		SetGuide("비밀번호를 확인하십시오.");
		return false;
	}

	if (code.IsEmpty())
	{
		SetGuide("종목코드를 확인하십시오.");
		return false;
	}

	if (jmcn <= 0 && bCheckJmcn)
	{
		SetGuide("주문수량을 확인하십시오.");
		return false;
	}

	return true;
}

int CHogaWnd::GetJumunCount(CString junos, int mmgb)
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
			
			if (mmgb == Str2int(pOrder->mmgb))
				count += atoi(pOrder->mcvl);
		}
	}
	else
	{
		while (!junos.IsEmpty())
		{
			//orderNo = m_pParent->Parser(junos, "\t");
			orderNo = m_pTool->Parser(junos, '\t');
			if (m_mapOrder.Lookup(atoi(orderNo), pOrder))
				count += atoi(pOrder->mcvl);
		}
	}

	m_cs.Unlock();

	return count;
}

void CHogaWnd::DoModalConfigDlg()
{
	CString strPath;
	strPath.Format("%s\\%s\\%s\\%s.ini", m_pTool->GetHome(), USRDIR, m_pTool->GetName(), MAP_NAME);

	CConfigDlg dlg(strPath);
	dlg.DoModal();

	m_config = dlg.m_config;

	if(m_config.popupremain)
		m_pCtrlWnd->SetAmountRemain(true);
	else
		m_pCtrlWnd->SetAmountRemain(false);

	SetHogaRect();
	Invalidate();
}

CString CHogaWnd::CalculatePercent(CString hoga)
{
	if (!m_config.percent)
		return "";
	else if (m_config.percent == 2)
		return CalculateDiffTick(hoga);

	CString percent, space = "              ";
	int price = -1;
	if (m_config.percentRadio == 0)
	{
		if (m_config.percentKind == 0)		// 기준가
			price = Str2int(m_strGijun);
		else if (m_config.percentKind == 1)	// 전일종가
			price = Str2int(m_strJijg);
		else if (m_config.percentKind == 2)	// 시가
			price = Str2int(m_strSiga);
		else if (m_config.percentKind == 3)	// 고가
			price = Str2int(m_strKoga);
		else if (m_config.percentKind == 4)	// 저가
			price = Str2int(m_strJega);
	}
//	else if (m_config.percentRadio == 1)		// BEP
//	{
//		double BEP = atof(m_config.BEP);
//		int PGDG = m_pCtrlWnd->GetPGDG();
//		price = (int)((PGDG * (BEP * 0.01 + 1)) / (1 - (0.003 + BEP*0.01)));
//		if (PGDG == 0)
//			price = -1;
//	}
	else if (m_config.percentRadio == 2)		// user
		price = atoi(m_config.user);
	
	// 전일종가가 없으면 등락율을 클리어후 리턴
	if (price <= 0)
		return "";

	int iPrice = 0;
	double dPercent = 0;
	
	iPrice = Str2int(hoga);
	
	if (iPrice == 0)
		return "";
		
	// 전일종가 보다 크면
	if (iPrice > price)
	{
		dPercent = (double)(iPrice - price) / (double)price * 100;
		percent.Format("%2.2f%%", dPercent);
		const int pos = percent.Find(".");
		percent = space.Left(3 - pos) + percent;
	}
	// 전일종가 보다 작으면
	else if (iPrice < price)
	{
		dPercent = (double)(price - iPrice) / (double)price * 100;
		percent.Format("%2.2f%%", dPercent);
		const  int pos = percent.Find(".");
		percent = space.Left(3 - pos) + percent;
	}
	// 전일종가와 같으면
	else
		percent = "  0.00%";

	return percent;
}

CString CHogaWnd::CalculateDiffTick(CString hoga)
{
	if (!m_config.percent)
		return "";


	CString percent, space = "              ";
	int price = -1;
	if (m_config.percentRadio == 0)
	{
		if (m_config.percentKind == 0)		// 기준가
			price = Str2int(m_strGijun);
		else if (m_config.percentKind == 1)	// 전일종가
			price = Str2int(m_strJijg);
		else if (m_config.percentKind == 2)	// 시가
			price = Str2int(m_strSiga);
		else if (m_config.percentKind == 3)	// 고가
			price = Str2int(m_strKoga);
		else if (m_config.percentKind == 4)	// 저가
			price = Str2int(m_strJega);
	}
	else if (m_config.percentRadio == 2)		// user
		price = Str2int(m_config.user);
	
	// 전일종가가 없으면 전일대비 클리어후 리턴
	if (price <= 0)
		return "";

	int iDiff = 0;
	const int iHoga = Str2int(hoga);
	
	if (iHoga == 0)
		return "";
		
	// 전일종가 보다 크면
	if (iHoga > price)
	{
		iDiff = (iHoga - price);
		percent.Format("%d", iDiff);
		percent = m_pTool->AddComma(percent);
	}
	// 전일종가 보다 작으면
	else if (iHoga < price)
	{
		iDiff = (price - iHoga);
		percent.Format("%d", iDiff);
		percent = m_pTool->AddComma(percent);
	}
	// 전일종가와 같으면
	else
		percent = "0";

	percent = space.Left(7 - percent.GetLength()) + percent;

	return percent;
}

int CHogaWnd::SetHogaReserve()
{
	m_eFdgb = m_eFdgb == hgFOLD ? hgUNFOLD : hgFOLD;

	SetHogaRect();

	Invalidate();

	return m_eFdgb;
}

void CHogaWnd::DrawArrowLine(CDC *pDC, CPoint StartPoint, CPoint EndPoint, bool bHolder)
{
	CPen *pOldPen = pDC->SelectObject(m_pTool->RequestPen(COLOR_ARROW, 1, PS_SOLID));
	

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
	CBrush BlackBrush(COLOR_ARROW);

	Point[0] = CPoint( EndPoint.x + int( Par * cosy - ( Par / 2.0 * siny ) ),
		EndPoint.y + int( Par * siny + ( Par / 2.0 * cosy ) ) );
	Point[1] = CPoint( EndPoint.x + int( Par * cosy + Par / 2.0 * siny ),
		EndPoint.y - int( Par / 2.0 * cosy - Par * siny ) );
	Point[2] = EndPoint;

	Rgn.CreatePolygonRgn(Point, sizeof(Point) / sizeof(CPoint), ALTERNATE);
	pDC->FillRgn(&Rgn, &BlackBrush);
	
	pDC->MoveTo( StartPoint );
	pDC->LineTo( EndPoint );
	
	if (bHolder)
		pDC->FillRect(&rc, &BlackBrush);

	pDC->SelectObject(pOldPen);
}

void CHogaWnd::DrawArrowLine2(CDC *pDC, CPoint StartPoint, CPoint ViaPoint, CPoint EndPoint, bool bHolder)
{
	if (ViaPoint == EndPoint)
	{
		DrawArrowLine(pDC, StartPoint, EndPoint, bHolder);
		return;
	}
	else
	{
		CPen *pOldPen = pDC->SelectObject(m_pTool->RequestPen(COLOR_ARROW, 1, PS_SOLID));
		pDC->MoveTo(StartPoint);
		pDC->LineTo(ViaPoint);
		pDC->SelectObject(pOldPen);

		DrawArrowLine(pDC, ViaPoint, EndPoint, false);
	}
	CRect rc(StartPoint, StartPoint);
	rc.InflateRect(3, 3);
	
	CBrush BlackBrush(COLOR_ARROW);
	if (bHolder)
		pDC->FillRect(&rc, &BlackBrush);
}

void CHogaWnd::DrawArrowLine3(CDC *pDC, CPoint StartPoint, CPoint EndPoint, int mmgb)
{
	COLORREF color = RGB(200, 30, 30);
	if (mmgb == 3)
		color = RGB(0, 124, 0);

	CPen *pOldPen = pDC->SelectObject(m_pTool->RequestPen(color, 1, PS_SOLID));

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

void CHogaWnd::DrawArrowBox(CDC *pDC, CPoint StartPoint)	//2012.11.15 KSJ 시장가 일때 그리기
{
	const CRect rc(StartPoint.x, StartPoint.y -3, StartPoint.x+10, StartPoint.y+7);
	
	pDC->FillSolidRect(&rc, COLOR_ARROW);
}

void CHogaWnd::CheckReserveJumun(CString code, CString curr)
{
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);

	class CReserve *pResv = NULL;

	CString mmgb, price;
	
	for (int ii = m_arReserve.GetUpperBound(); ii >= 0; ii--)
	{
		if (ii > m_arReserve.GetUpperBound() || ii < 0)
			break;

		pResv = m_arReserve.GetAt(ii);
		if (!pResv)	continue;

		if ((pResv->gubn == aboveCURR && pResv->gprc <= Str2int(curr) && code == pResv->code)
			|| (pResv->gubn == belowCURR && pResv->gprc >= Str2int(curr) && code == pResv->code))
		{
			CString name = pResv->name;
			const int jqty = pResv->jqty;
			const int jprc = pResv->jprc;
			const char mmgb = pResv->mmgb;
			const char sijang = pResv->sijang;

			delete pResv;	pResv = NULL;
			m_arReserve.RemoveAt(ii);

			//2014.05.22 KSJ 주문착오방지 적용
			MistakeOrder misOdr;
			
			misOdr.strCode = GetCODX();
			misOdr.lPrice = jprc;
			misOdr.lQuantity = jqty;
			misOdr.nMmgb = mmgb - '0';
			if(sijang == 1)				misOdr.strHogb = "03";
			else						misOdr.strHogb.Format("%c%c", m_pCtrlWnd->GetJumunCondition().GetAt(0), '0');
			misOdr.strCurPrc = m_strCurr;
			misOdr.strMaxPrc = m_strShga;
			misOdr.strMinPrc = m_strHhga;
			misOdr.strListedQuantity = GetListedQuantity();
			misOdr.strStdPrc = m_strGijun;
			misOdr.iMarketType = m_eMarket; //20200904 ETF, ETN 주문확인
			misOdr.strKrgb = m_strKrgb;
			misOdr.strKrgb += "\t";
			misOdr.strKrgb += m_strMkms;

			const bool bRet = CheckupMistakeOrder(this, misOdr);
			
			if (!bRet)
				return;
			//KSJ END 

			if (GetConfirmJumun())
			{
				CDialogConfirm dlg(rc, m_pTool->GetWizard(), m_pParent);
				price.Format("%d", jprc);

				if(sijang == 1)	//시장가 일때 KSJ 
					dlg.SetData(mmgb == MD_JUMUN ? 'Y' : 'Z', GetAccount(), GetName(), name, jqty, "시장가", 'M', "", GetCondition());
				else
					dlg.SetData(mmgb == MD_JUMUN ? 'S' : 'B', GetAccount(), GetName(), name, jqty, price, 'F', GetCondition());
				
				if (dlg.DoModal() != IDOK)
				{
					continue;
				}
			}

			if(sijang == 1)	//시장가 일때 KSJ 
				SendJumun(mmgb == MD_JUMUN ? '1' : '2', 0, 0, 0, 1);
			else
				SendJumun(mmgb == MD_JUMUN ? '1' : '2', jprc, 0, jqty);
		}
		/*
		else if (pResv->gubn == belowCURR && pResv->gprc >= Str2int(curr) && code == pResv->code)
		{
			if (GetConfirmJumun())
			{
				CDialogConfirm dlg(rc, m_pTool);
				price.Format("%d", pResv->jprc);
				dlg.SetData(pResv->mmgb == MD_JUMUN ? 'S' : 'B', GetAccount(), GetName(), pResv->name, pResv->jqty, price, 'F', "");
				if (dlg.DoModal() != IDOK)
				{
					m_arReserve.RemoveAt(ii);
					delete pResv;
					continue;
				}
			}

			SendJumun(pResv->mmgb == MD_JUMUN ? '1' : '2', pResv->jprc, 0, pResv->jqty);

			delete pResv;
			m_arReserve.RemoveAt(ii);
		}
		*/
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
			if (!m_pTooltip)
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
		
			m_pTooltip->RelayEvent(&msg);
		}
		break;
	default:
		break;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

void CHogaWnd::OnDestroy() 
{
	if (m_pTooltip && m_pTooltip->m_hWnd)
	{
		m_pTooltip->Activate(FALSE);
		m_pTooltip->DelTool(this);
		m_pTooltip->RelayEvent(NULL);

		delete m_pTooltip;
	}
	
	if (m_pMadoRsvd) { m_pMadoRsvd->DestroyWindow();}
	if (m_pMadoMarket) { m_pMadoMarket->DestroyWindow(); }
	if (m_pMadoJumn) { m_pMadoJumn->DestroyWindow(); }
	if (m_pMasuRsvd) { m_pMasuRsvd->DestroyWindow(); }
	if (m_pMasuJumn) { m_pMasuJumn->DestroyWindow(); }
	if (m_pMasuMarket) { m_pMasuMarket->DestroyWindow(); }
	if (m_pHogaFix)  { m_pHogaFix->DestroyWindow(); }

	CWnd::OnDestroy();
}

void CHogaWnd::ClearJumun()
{
	RemoveAllJumun();
	RemoveAllReserve();
	RemoveAllArrow();
}

void CHogaWnd::SetModArrow(int jflg, int price, int juno)
{
	m_cs.Lock();

	class COrder *pOrder = NULL;
	if (!m_mapOrder.Lookup(juno, pOrder))
	{
		m_cs.Unlock();
		return;
	}

//	class CArrow *pArrow = new class CArrow;
	std::shared_ptr<CArrow> pArrow = std::make_shared<CArrow>();
	pArrow->ojno = juno;
	pArrow->jflg = jflg;
	pArrow->mmgb = Str2int(pOrder->mmgb);
	pArrow->oprc = Str2int(pOrder->jprc);
	pArrow->mprc = price;

	m_arArrow.Add(pArrow);

	m_cs.Unlock();
}

CString CHogaWnd::GetCODX()
{
	return CString((char*)GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_GET_CODX, NULL), NULL));
}

CString CHogaWnd::GetAccount()
{
	return CString((char*)GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_GET_ACCOUNT, NULL), NULL));
}

CString CHogaWnd::GetCondition()
{
	return CString((char*)GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_GET_CONDITION, NULL), NULL));
}

CString CHogaWnd::GetPassword()
{
	return CString((char*)GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_GET_PASSWORD, NULL), NULL));
}

CString CHogaWnd::GetName()
{
	return CString((char*)GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_GET_NAME, NULL), NULL));
}

CString CHogaWnd::GetCodeName()
{
	return CString((char*)GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_GET_CODENAME, NULL), NULL));
}

//2014.06.23 KSJ 상장주식수 가져오기
CString CHogaWnd::GetListedQuantity()
{
	return CString((char*)GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_GET_LISTEDQUANTITY, NULL), NULL));
}

void CHogaWnd::SetGuide(CString strMsg)
{
	GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_SET_GUIDE, NULL), (LPARAM)strMsg.operator LPCTSTR());
}

void CHogaWnd::SendJumun(char cMmgb, int iPrice, int iJuno, int iJqty, int bMarketPrice)
{
	struct _jumun Jumun;
	Jumun.cMmgb = cMmgb;
	Jumun.iPrice = iPrice;
	Jumun.iJuno = iJuno;
	Jumun.iJqty = iJqty;
	Jumun.bMarketPrice = bMarketPrice;

	GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_SEND_JUMUN, NULL), (LPARAM)&Jumun);
}

void CHogaWnd::DrawColorBox(CDC* pDC, CRect rc, BOOL l, BOOL t, BOOL r, BOOL b)
{
//	CPen *pHorz = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_LINE), 1, PS_SOLID);
//	CPen *pVert = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_LINE), 1, PS_SOLID);

	//CPen *pOldPen = pDC->SelectObject(pHorz);
	CPen *ppenOld = pDC->SelectObject(m_ppenLine);
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

//	pDC->SelectObject(pVert);
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

	pDC->SelectObject(ppenOld);
}

void CHogaWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptScreen = point;
	ClientToScreen(&ptScreen);
	m_pTool->PopupMenu(MAP_NAME, ptScreen.x, ptScreen.y);
	
	CWnd::OnRButtonUp(nFlags, point);
}


char CHogaWnd::GetMarket()
{
	switch (m_eMarket)
	{
	case KOSPI:
		return '1';
	case KOSDAQ:
		return '2';
	case ELW:
		return '3';
	case ETF:
		return '1';
	}

	return '0';
}

CPoint CHogaWnd::GetRect2HogaPoint(CPoint point)
{
	CPoint pt = point;
	ScreenToClient(&point);
	return pt;
}

CString CHogaWnd::MakeSangHan(int iCurr)
{
	const int iLimit = (int)((double)abs(iCurr) * 1.3);
	int  iValue = GetPriceUnit(abs(iCurr), true);
	while (iValue < iLimit)
	{
		iValue = GetPriceUnit(iValue, true);
	}

	CString strValue;
	strValue.Format("%d", iValue);
	return strValue;
}

CString CHogaWnd::MakeHaHan(int iCurr)
{
	const int iLimit = (int)((double)abs(iCurr) * 0.7);
	int  iValue = GetPriceUnit(abs(iCurr), true);
	while (iValue > iLimit)
	{
		iValue = GetPriceUnit(iValue, false);
	}

	CString strValue;
	strValue.Format("%d", iValue);
	return strValue;
}

void CHogaWnd::Palette()
{
	m_clrBG = m_pTool->GetIndexColor(COLOR_BK);
	m_clrTH = m_pTool->GetIndexColor(COLOR_TABLEHEAD);
	m_clrGH = m_pTool->GetIndexColor(COLOR_GRIDHEAD);
	m_clrSBG = m_pTool->GetIndexColor(119);
	m_clrBBG = m_pTool->GetIndexColor(120);
	m_clrLGFG = m_pTool->GetIndexColor(COLOR_LINE);
	m_clrUp = m_pTool->GetIndexColor(94);
	m_clrDown = m_pTool->GetIndexColor(95);
	m_clrGridHdrTxt = m_pTool->GetIndexColor(COLOR_GRIDHEADTXT);


	m_ppenLine = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_LINE), 1, PS_SOLID);
	m_ppenTable = m_pTool->RequestPen(m_pTool->GetIndexColor(COLOR_TABLE), 1, PS_SOLID);

	Invalidate();
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

void CHogaWnd::ChangeFont(double* lRate, CFont* pfont, CFont* boldFont)
{
	m_pfont = pfont;
	if(boldFont)
		m_pfontB = boldFont;
}

void CHogaWnd::Click_Order(CPoint point)
{
	if (!CheckJumunAvailable())
		return;

	int	hoga{};
	const int	count = m_pCtrlWnd->GetMultiCount();
	CString price, gprc, jprc;

	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);

	for (int ii = 0; ii < m_iRows - 1; ii++)
	{
		hoga = GetIndexToHoga(ii);
		if (hoga == 0)
			continue;

		price.Format("%d", hoga);
		price = m_pTool->AddComma(price);

		if (m_rcMDJumun[ii].PtInRect(point))
		{
			//2014.05.22 KSJ 주문착오방지 적용
			MistakeOrder misOdr;
			
			misOdr.strCode = GetCODX();
			misOdr.lPrice = hoga;
			misOdr.lQuantity = m_pCtrlWnd->GetJumunCount();
			misOdr.nMmgb = MD_JUMUN;
			if(m_pCtrlWnd->GetSijang())		misOdr.strHogb = "03";
			else							misOdr.strHogb.Format("%c%c", m_pCtrlWnd->GetJumunCondition().GetAt(0), '0');
			misOdr.strCurPrc = m_strCurr;
			misOdr.strMaxPrc = m_strShga;
			misOdr.strMinPrc = m_strHhga;
			misOdr.strListedQuantity = GetListedQuantity();
			misOdr.strStdPrc = m_strGijun;
			misOdr.iMarketType = m_eMarket; //20200904 ETF, ETN 주문확인
			
			const bool bRet = CheckupMistakeOrder(this, misOdr);  //매도
			
			if (!bRet)
				return;
			//KSJ END 



			if (GetConfirmJumun())
			{
				CDialogConfirm dlg(rc, m_pTool->GetWizard(), m_pParent);
				dlg.SetData('S', GetAccount(), GetName(), GetCodeName(), m_pCtrlWnd->GetJumunCount(), price, 'F', "", GetCondition());
				if (dlg.DoModal() != IDOK)
					return;
			}

			for (int jj = 0; jj < count; jj++)
			{
				SendJumun('1', hoga);
				hoga = GetPriceUnit(hoga, true);
			}
			break;
		}
		else if (m_rcMSJumun[ii].PtInRect(point))
		{
			//2014.05.22 KSJ 주문착오방지 적용
			MistakeOrder misOdr;
			
			misOdr.strCode = GetCODX();
			misOdr.lPrice = hoga;
			misOdr.lQuantity = m_pCtrlWnd->GetJumunCount();
			misOdr.nMmgb = MS_JUMUN;
			if(m_pCtrlWnd->GetSijang())		misOdr.strHogb = "03";
			else							misOdr.strHogb.Format("%c%c", m_pCtrlWnd->GetJumunCondition().GetAt(0), '0');
			misOdr.strCurPrc = m_strCurr;
			misOdr.strMaxPrc = m_strShga;
			misOdr.strMinPrc = m_strHhga;
			misOdr.strListedQuantity = GetListedQuantity();
			misOdr.strStdPrc = m_strGijun;
			misOdr.iMarketType = m_eMarket; //20200904 ETF, ETN 주문확인
			misOdr.strKrgb = m_strKrgb;
			misOdr.strKrgb += "\t";
			misOdr.strKrgb += m_strMkms;
			
			const bool bRet = CheckupMistakeOrder(this, misOdr);  //매수
			
			if (!bRet)
				return;
			//KSJ END 
		
			if(GetPrgbType() == 2)  //대출종목에서 매수시도하면
			{
				MessageBox("대출매수는 불가합니다." , "IBK투자증권", MB_OK);
				return;
			}

			if (GetConfirmJumun())
			{
				CDialogConfirm dlg(rc, m_pTool->GetWizard(), m_pParent);
				dlg.SetData('B', GetAccount(), GetName(), GetCodeName(), m_pCtrlWnd->GetJumunCount(), price, 'F', "", GetCondition());
				if (dlg.DoModal() != IDOK)
					return;
			}

			for (int jj = 0; jj < count; jj++)
			{
				SendJumun('2', hoga);
				hoga = GetPriceUnit(hoga, false);
			}
			break;
		}
		else if (m_rcMDResv[ii].PtInRect(point))
		{
			if (ii == m_iPosSHGA || ii == m_iPosHHGA || ii == m_iPosCHVL)
				return;

			if (hoga == Str2int(m_strCurr))
			{
				::MessageBox(m_hWnd, "현재가에서는 예약주문을 할 수 없습니다.", MAP_TITLE, MB_OK | MB_ICONSTOP);
				return;
			}

			if (GetTickUnit(hoga, MD_JUMUN) < Str2int(m_strHhga) 
				|| GetTickUnit(hoga, MD_JUMUN) > Str2int(m_strShga))
			{
				SetGuide("주문 가능한 가격이 아닙니다.");
				return;
			}

			class CReserve *pResv = new class CReserve;
			pResv->code = GetCODX();
			pResv->name = GetCodeName();
			pResv->gprc = hoga;
			pResv->jprc = GetTickUnit(hoga, MD_JUMUN);
			pResv->jqty = m_pCtrlWnd->GetJumunCount();
			pResv->mmgb = MD_JUMUN;
			
			pResv->sijang = m_pCtrlWnd->GetSijang() ? 1:0;	//2012.11.15 KSJ 시장가 세팅

			if (pResv->gprc > Str2int(m_strCurr))
				pResv->gubn = aboveCURR;
			else
				pResv->gubn = belowCURR;
			m_arReserve.Add(pResv);

			break;
		}
		else if (m_rcMSResv[ii].PtInRect(point))
		{
			if (ii == m_iPosSHGA || ii == m_iPosHHGA || ii == m_iPosCHVL)
				return;

			if (hoga == Str2int(m_strCurr))
			{
				::MessageBox(m_hWnd, "현재가에서는 예약주문을 할 수 없습니다.", MAP_TITLE, MB_OK | MB_ICONSTOP);
				return;
			}

			if (GetTickUnit(hoga, MS_JUMUN) < Str2int(m_strHhga) 
				|| GetTickUnit(hoga, MS_JUMUN) > Str2int(m_strShga))
			{
				SetGuide("주문 가능한 가격이 아닙니다.");
				return;
			}

			if(GetPrgbType() == 2)  //대출종목에서 매수시도하면
			{
				MessageBox("대출매수는 불가합니다." , "IBK투자증권", MB_OK);
				return;
			}

			class CReserve *pResv = new class CReserve;
			pResv->code = GetCODX();
			pResv->name = GetCodeName();
			pResv->gprc = hoga;
			pResv->jprc = GetTickUnit(hoga, MS_JUMUN);
			pResv->jqty = m_pCtrlWnd->GetJumunCount();
			pResv->mmgb = MS_JUMUN;

			pResv->sijang = m_pCtrlWnd->GetSijang() ? 1:0;	//2012.11.15 KSJ 시장가 세팅

			if (pResv->gprc > Str2int(m_strCurr))
				pResv->gubn = aboveCURR;
			else
				pResv->gubn = belowCURR;			
			m_arReserve.Add(pResv);

			break;
		}
	}
	
	Invalidate();
}

BOOL CHogaWnd::PreTranslateMessage(MSG* pMsg) 
{
	if (m_HandCursorWnd.find(pMsg->hwnd) != m_HandCursorWnd.end())
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CHogaWnd::OnAllMadoCancel()
{
	m_bClearAll = true;
	AllJumnCancel(MD_JUMUN);
	m_bClearAll = false;
}

void CHogaWnd::OnAllMasuCancel()
{
	m_bClearAll = true;
	AllJumnCancel(MS_JUMUN);
	m_bClearAll = false;
}

void CHogaWnd::OnAllMadoRsvdCancel()
{
	m_bClearAll = true;
	AllRsvdCancel(MD_JUMUN);
	m_bClearAll = false;
}

void CHogaWnd::OnAllMasuRsvdCancel()
{
	m_bClearAll = true;
	AllRsvdCancel(MS_JUMUN);
	m_bClearAll = false;
}

void CHogaWnd::OnHogaFix()
{
	m_bHogaFix = !m_bHogaFix;

	if(m_bHogaFix)
		SetScroll(m_eDepth);
}

void CHogaWnd::OnMarketMado()
{
	if (!CheckJumunAvailable())
		return;

	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);

	//2014.05.22 KSJ 주문착오방지 적용
	MistakeOrder misOdr;
	
	misOdr.strCode = GetCODX();
	misOdr.lPrice = atol(m_strCurr);
	misOdr.lQuantity = m_pCtrlWnd->GetJumunCount();
	misOdr.nMmgb = MD_JUMUN;
	misOdr.strHogb = "03";
	misOdr.strCurPrc = m_strCurr;
	misOdr.strMaxPrc = m_strShga;
	misOdr.strMinPrc = m_strHhga;
	misOdr.strListedQuantity = GetListedQuantity();
	misOdr.strStdPrc = m_strGijun;
	misOdr.iMarketType = m_eMarket; //20200904 ETF, ETN 주문확인
	misOdr.strKrgb = m_strKrgb;
	misOdr.strKrgb += "\t";
	misOdr.strKrgb += m_strMkms;
	
	const bool bRet = CheckupMistakeOrder(this, misOdr);  //시장가 매도
	
	if (!bRet)
		return;
	//KSJ END 

	if (GetConfirmJumun())
	{
		CDialogConfirm dlg(rc, m_pTool->GetWizard(), m_pParent);
		dlg.SetData('Y', GetAccount(), GetName(), GetCodeName(), m_pCtrlWnd->GetJumunCount(), "시장가", 'M', "", GetCondition());
		if (dlg.DoModal() != IDOK)
			return;
	}

	SendJumun('1', 0, 0, 0, 1);
}

void CHogaWnd::OnMarketMasu()
{
	if (!CheckJumunAvailable())
		return;

	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);

	//2014.05.22 KSJ 주문착오방지 적용
	MistakeOrder misOdr;
	
	misOdr.strCode = GetCODX();
	misOdr.lPrice = atol(m_strCurr);
	misOdr.lQuantity = m_pCtrlWnd->GetJumunCount();
	misOdr.nMmgb = MS_JUMUN;
	misOdr.strHogb = "03";
	misOdr.strCurPrc = m_strCurr;
	misOdr.strMaxPrc = m_strShga;
	misOdr.strMinPrc = m_strHhga;
	misOdr.strListedQuantity = GetListedQuantity();
	misOdr.strStdPrc = m_strGijun;
	misOdr.iMarketType = m_eMarket; //20200904 ETF, ETN 주문확인
	misOdr.strKrgb = m_strKrgb;
	misOdr.strKrgb += "\t";
	misOdr.strKrgb += m_strMkms;
	
	if(GetPrgbType() == 2)  //대출종목에서 매수시도하면
	{
		MessageBox("대출매수는 불가합니다." , "IBK투자증권", MB_OK);
		return;
	}
	
	const bool bRet = CheckupMistakeOrder(this, misOdr);  //시장가 매수
	
	if (!bRet)
		return;
	//KSJ END 

	if (GetConfirmJumun())
	{
		CDialogConfirm dlg(rc, m_pTool->GetWizard(), m_pParent);
		dlg.SetData('Z', GetAccount(), GetName(), GetCodeName(), m_pCtrlWnd->GetJumunCount(), "시장가", 'M', "", GetCondition());
		if (dlg.DoModal() != IDOK)
			return;
	}

	SendJumun('2', 0, 0, 0, 1);
}

void CHogaWnd::AllJumnCancel(int nMmgb)	//0 지정안함(기존로직), 1 매도, 2, 매수
{
	CRect sRc;
	ClientToScreen(&sRc);

	CPoint cp;
	
	switch (nMmgb)
	{
	case BASIC_JUMUN:
		cp = m_ptStart; break;
	case MD_JUMUN:
		cp = m_rcMDJumun[m_iPosCHVL].CenterPoint(); 
		m_iHogaStart = 0;
		break;
	case MS_JUMUN:
		cp = m_rcMSJumun[m_iPosCHVL].CenterPoint(); 
		m_iHogaStart = 0;
		break;
	default:
		cp = m_ptStart;
	}

	int juno = 0, mmgb = 0, jmcn = 0;
	const int index = GetPointToIndex(cp, CELL_JUMUN, mmgb);
	class COrder *pOrder = NULL;
	
	const int hoga = GetIndexToHoga(index);
	
	m_cs.Lock();
	POSITION pos = m_mapOrder.GetStartPosition();
	CString strOne;
	int iCnt = 0;
	while( pos != NULL )
	{
		m_mapOrder.GetNextAssoc(pos, juno, pOrder);
		
		if (m_iHogaStart == 0 && mmgb == Str2int(pOrder->mmgb))
		{
			jmcn += Str2int(pOrder->mcvl);
			strOne = pOrder->juno;
			iCnt++;
		}
		else if (m_iHogaStart == Str2int(pOrder->jprc) && mmgb == Str2int(pOrder->mmgb))
		{
			jmcn += Str2int(pOrder->mcvl);
			strOne = pOrder->juno;
			iCnt++;
		}
	}
	
	strOne.TrimLeft();
	strOne.TrimRight();
	strOne.Format("%10d", atoi((LPCTSTR)strOne)); 	//2015.05.28 KSJ 번호 %05d -> %10d
	
	CString strTemp;
	
//CString strTmp;
//strTmp.Format("[KSJ] point[%d,%d] index[%d], mmgb[%d], hoga[%d], iCnt[%d]", cp.x, cp.y, index, mmgb, hoga, iCnt);
//OutputDebugString(strTmp);

	if (iCnt > 1)
	{
		strTemp.Format("%s 외 %d개", (LPCTSTR)strOne, iCnt - 1);
		strOne = strTemp;
	}
	
	if (GetConfirmJumun())
	{
		CDialogConfirm dlg(sRc, m_pTool->GetWizard(), m_pParent);
		dlg.SetData('C', GetAccount(), GetName(), GetCodeName(), jmcn, "", NULL, strOne);
		if (dlg.DoModal() != IDOK)
		{
			m_cs.Unlock();
			return;
		}
	}
	
	pos = m_mapOrder.GetStartPosition();
	int nCount = 0;
	while( pos != NULL )
	{
		m_mapOrder.GetNextAssoc(pos, juno, pOrder);
		
		if (m_iHogaStart == 0 && mmgb == Str2int(pOrder->mmgb))
		{
			SendJumun('4', hoga, Str2int(pOrder->juno), 0);
			SetModArrow(4, 0, Str2int(pOrder->juno));
		}
		else if (m_iHogaStart == Str2int(pOrder->jprc) && mmgb == Str2int(pOrder->mmgb))
		{
			SendJumun('4', hoga, Str2int(pOrder->juno), 0);
			SetModArrow(4, 0, Str2int(pOrder->juno));
		}
		
		nCount++;
		
		if (nCount % 30 == 0)
		{
			Sleep(100);
		}
		else
			Sleep(0);
	}	
	Invalidate();
	m_cs.Unlock();
}

void CHogaWnd::AllRsvdCancel(int nMmgb)	//0 지정안함(기존로직), 1 매도, 2, 매수
{
	CPoint cp;

	switch (nMmgb)
	{
		case BASIC_JUMUN:
			cp = m_ptStart; break;
		case MD_JUMUN:
			cp = m_rcMDResv[m_iPosCHVL].CenterPoint();
			m_iHogaStart = 0;
			break;
		case MS_JUMUN:
			cp = m_rcMSResv[m_iPosCHVL].CenterPoint(); 
			m_iHogaStart = 0;
			break;
		default:
			cp = m_ptStart;
	}

	int mmgb = 0;
	const int index = GetPointToIndex(cp, CELL_RESERVE, mmgb);
	class CReserve *pResv = NULL;
	
	const int count = m_arReserve.GetSize() - 1;
	for (int ii = count; ii >= 0; ii--)
	{
		pResv = m_arReserve.GetAt(ii);
		if (m_iHogaStart == 0 && pResv->mmgb == mmgb)
		{
			delete pResv;
			m_arReserve.RemoveAt(ii);
		}
		else if (m_iHogaStart == pResv->gprc && pResv->mmgb == mmgb)
		{
			delete pResv;
			m_arReserve.RemoveAt(ii);
		}
	}
	Invalidate();
}

int CHogaWnd::GetPrgbType()  //그리드에서 선택된 잔고의 타입 //현금 , 신용, 대출 
{
	return GetParent()->SendMessage(UM_MYMESSAGE, MAKEWPARAM(UP_GET_PRGBTYPE, NULL), NULL);
}