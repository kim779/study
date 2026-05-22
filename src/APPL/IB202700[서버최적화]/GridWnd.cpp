// GridWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB202700.h"
#include "GridWnd.h"
#include "intgrid.h"
#include "../../h/grid.h"
#include "GroupWnd.h"
#include "MainWnd.h"
#include "UsefulSetupDlg.h"
#include "MoveSetupDlg.h"     // ADD PSH 20070918
#include "AutoSaveSetupDlg.h" // ADD PSH 20070918
#include "AlertSoundDlg.h"    // ADD PSH 20070918
#include "PopupDlg.h"
#include "CodeDuplicate.h"
#include "InterDomino.h"
#include "mmsystem.h"
#include <math.h>
#include "resource.h"
#include "axMenu.h"

#define GRIDMAXNUM 31
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef CWnd *(*_DLL_SetLibOpen)(CWnd *pwndMain, HINSTANCE hInstance, CWnd *pWndModuleWnd);
typedef void (*_DLL_SetLibClose)(CWnd *pWndTarget);
typedef int (*_DLL_SetSignal)(BOOL bAdvise, CWnd *pwndMain, CString strType, CString sPBID, CString strKey);

_DLL_SetLibOpen SignalLib_Open;
_DLL_SetLibClose SignalLib_Close;
_DLL_SetSignal SignalLib_SetSignal;

static const std::unordered_map<int, int>& FwdMap()
{
	static const std::unordered_map<int, int> m = {
		{ 23, 623 },
		{ 24, 624 },
		{ 27, 627 },
		{ 28, 628 },
		{ 29, 629 },
		{ 30, 630 },
		{ 31, 631 },
		{ 32, 632 },
		{ 33, 633 },
		{ 36, 634 },
		{ 41, 641 },
		{ 51, 651 },
		{ 61, 661 },
		{ 71, 671 },
		{ 101, 601 },
		{ 104, 604 },
		{ 106, 606 },
		{ 109, 609 },
		{ 146, 646 },
		{ 181, 681 },
		{ 111, 611 },
		{ 112, 612 },
		{ 115, 615 },
		{ 116, 616 },

		// ...
	};
	return m;
}

static const std::unordered_map<int, int>& RevMap()
{
	static const std::unordered_map<int, int> m = [] {
		std::unordered_map<int, int> r;
		r.reserve(FwdMap().size());
		for (const auto& kv : FwdMap())
			r.emplace(kv.second, kv.first);
		return r;
	}();
	return m;
}

// 23 -> 623 (없으면 원본 그대로)
static int ToMapped(int idx)
{
	const auto& m = FwdMap();
	auto it = m.find(idx);

	CString slog;
	

	if (it != m.end())
		return  it->second;
	else
		return idx;

}

// 623 -> 23 (없으면 원본 그대로)
static int ToOriginal(int idx)
{
	const auto& m = RevMap();
	auto it = m.find(idx);
	return (it != m.end()) ? it->second : idx;
}

/////////////////////////////////////////////////////////////////////////////
// CGridWnd

CGridWnd::CGridWnd(CWnd *pMainWnd, int nIndex) : CBaseWnd(pMainWnd)
{
	_pApp = dynamic_cast<CIB202700App*>(AfxGetApp());
	SetIndex(nIndex);
	_classname = "CGridWnd";
	m_grid = nullptr;
	m_kind = 0;
	m_szTitle = _T("");
	m_updateROW = -1;
	m_endsort = false;
	m_posField = false;
	m_ccField = false;
	m_drag = m_drop = -1;
	m_nTurn = 0;
	m_seldrop = -1;
	m_bongField = -1;
	m_sonikField = -1;
	m_suikField = -1;

	m_bTicker = FALSE;
	m_bfirstStart = NULL;

	m_bMargin = FALSE;
	m_bSorting = FALSE;
	m_mapMargin.RemoveAll();
	m_clrMarkerBKG = RGB(240, 240, 240);
	m_clrMarkerTXT = RGB(142, 142, 142);

	m_bShadow = TRUE;
	m_bAllApply = TRUE;
	m_bMoveCfg = FALSE;
	m_nMoveSave = 0;
	m_pMsgWnd = nullptr;
	m_bInfo = FALSE;
	m_bNewsNChart = TRUE; // 2011.12.02 KSJ
	m_bPlusMinus = false; // 2011.12.05 KSJ

	m_bAddCnd = FALSE;
	m_bChgCnd = FALSE;
	m_bAutoSaveAllApply = FALSE;
	m_nRowHeight = 0;
	m_bWaveApply = FALSE;

	m_clrKospi = RGB(0, 0, 0);
	m_clrKosdaq = RGB(128, 64, 64);
	m_bMKClrApply = FALSE;

	m_bExpect = FALSE;
	m_bAutoExpect = FALSE;
	m_bPopupDlg = FALSE;

	m_bRTS = FALSE;
	m_bDispCode = FALSE;

	m_nCodeWidth = 80;

	m_nNewsNChartWidth = 20; // 2011.12.02 KSJ

	m_pRCDlg = nullptr;
	m_bMonitor = FALSE;
	m_bMonSym = FALSE;
	m_bEditWork = FALSE;

	m_bIsFirst = FALSE;

	m_dBase = 0;
	m_ClickRow = 0;



	_mapiYDayVol.clear();	
	for (int i = 0; i < IARRAYSIZE; i++)
	{
		m_irowCode[i] = 0;
	}

	m_yDayVolField = -1; //전일거래대비율필드
	m_EqualizerField = -1;

	m_recomDataField = -1;
	m_recomBasicCostField = -1;
	m_recomSuikrateField = -1;
	m_recomBMjisuField = -1;
	m_recomBMSuikrateField = -1;
	m_recomgoalCostField = -1;
	m_recomJisuGubnField = -1;
	m_upjongCode = -1;
	m_beforeVolField = -1;
	m_newCurrField = -1;
	m_epbDataField = -1; // 2016.04.15 KSJ epb 추천일자

	m_nGridWidth = 0; //그리드 넓이
	// END ADD
	for (int cc = 0; cc < 200; cc++)
	{
		chg_count[cc] = 0;
	}

	m_dKosdaq = 0;
	m_dKospi = 0;

	m_iStartPoint = 0;
	m_iEndPoint = 0;
	m_iSendcount = -1;
	m_staticUpdate = -1;

	m_bContinue = FALSE;
	m_bSecond = FALSE;

	m_pSearchMap.RemoveAll();

	m_strBeginTime = "084000";
	m_strEndTime   = "085959";


//	m_strBeginTime = "142000";
//	m_strBeginTimeEnd = "145959";
	m_nCount = 0;
	m_nBookMarkRow = 0;

	m_arEpbDate.RemoveAll(); // 2016.04.15 KSJ
	m_epbDataField = -1;	 // 2016.04.15 KSJ

	m_bLastSend = TRUE;

	CString path;
	path.Format("%s/%s/%s", m_root, DEVDIR, "lib_signalmng.dll");
	m_hSignalMng = ::LoadLibrary(path);

	if (m_hSignalMng)
	{
		SignalLib_SetSignal = (_DLL_SetSignal)GetProcAddress(m_hSignalMng, "_DLL_SetSignal");
	}

	m_bSkipRTS = false;

	m_arrSymbol.Add("3051");
	m_arrSymbol.Add("3071");
	m_arrSymbol.Add("3041");
	m_arrSymbol.Add("3061");
	m_arrSymbol.Add("3101");
	m_arrSymbol.Add("3106");
	m_arrSymbol.Add("3104");
	m_arrSymbol.Add("3109");
}

CGridWnd::~CGridWnd()
{
	ClearInterest();
	if (m_hSignalMng)
	{
		FreeLibrary(m_hSignalMng);
	}
	m_arrSymbol.RemoveAll();
}

BEGIN_MESSAGE_MAP(CGridWnd, CBaseWnd)
//{{AFX_MSG_MAP(CGridWnd)
ON_WM_LBUTTONDOWN()
ON_WM_TIMER()
ON_WM_RBUTTONUP() // ADD PSH 20070912
ON_WM_SIZING()
//}}AFX_MSG_MAP
ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridWnd message handlers

void CGridWnd::setMemo()
{
	if (m_grid)
		m_grid->setMemoType(_pApp->getMemoType());
}

void  CGridWnd::initgridalert()
{
	_typeAuto = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_AUTOEXPECT);	
}

void CGridWnd::HoldDraw() {
	if (m_grid)
	{	
		bool bVisible = true;
		if (m_pGroupWnd) 
			bVisible = ((CGroupWnd*)m_pGroupWnd)->getVirtualDraw();
		
		m_grid->setVirtualDraw(bVisible);
		m_grid->setDelay(_pApp->getDelay());
		m_grid->setReal(true);
		//m_grid->beginDrawHolding();
		m_grid->setBlinkType(_pApp->getBlinkType());
		m_grid->setBlinkColor(_pApp->getBlinkColor());
		m_grid->setMemoType(_pApp->getMemoType());
	}
}

void CGridWnd::ReleaseDraw() {
	if (m_grid)
	{
		m_grid->setReal(false);
		//m_grid->endDrawHolding();
	}
}

void CGridWnd::UpdateAll()
{
	if ( m_grid )
		m_grid->UpdateWindow();
}


void CGridWnd::OperInit()
{
	m_pView = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	m_pGroupWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	m_pToolWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	m_pTreeWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));


	// 트리 펼치기 버튼
	CString fileIMG;
	fileIMG.Format("%s\\%s\\", m_root, IMAGEDIR);

	LOGFONT lf;
	m_pFont->GetLogFont(&lf);
	struct _fontR fontR;
	fontR.bold = FW_BOLD;
	fontR.italic = FALSE;
	fontR.point = ((CGroupWnd *)m_pGroupWnd)->GetFontSize();
	fontR.name = lf.lfFaceName;
	m_pFontB = getAxFont(&fontR);
	lf.lfHeight = fontR.point * 10;

	m_rowC = 2;
	// END MDOIFY
	m_colC = 8;
	m_clrFOCUS[0] = RGB(255, 255, 128);
	m_clrFOCUS[1] = RGB(255, 255, 255);
	m_clrTEXT[0] = RGB(100, 100, 100);
	m_clrTEXT[1] = RGB(100, 100, 100);
	m_size.cy = ((CGroupWnd *)m_pGroupWnd)->GetRowHeight();
	loadcfg();

	m_grid = std::make_unique<CintGrid>(m_pMainWnd, this, &lf);
	m_grid->Create(CRect(0, 0, 0, 0), this, IDC_GRID, GVSC_BOTH, GVDD_FULL);
	m_grid->m_iTime = m_iTime;

	// ADD PSH 20070911
	m_grid->SetBmpIcon(fileIMG + "MARKER.bmp", 0);
	m_grid->SetBmpIcon(fileIMG + "관.bmp", 1);
	m_grid->SetBmpIcon(fileIMG + "투자유의.bmp", 2);
	m_grid->SetBmpIcon(fileIMG + "주.bmp", 3);
	m_grid->SetBmpIcon(fileIMG + "경.bmp", 4);
	m_grid->SetBmpIcon(fileIMG + "위2.bmp", 5);
	m_grid->SetBmpIcon(fileIMG + "추천종목.BMP", 6);

	m_grid->SetBmpIcon(fileIMG + "환.BMP", 7);
	m_grid->SetBmpIcon(fileIMG + "이상급등.BMP", 8);
	m_grid->SetBmpIcon(fileIMG + "불.BMP", 9);
	m_grid->SetBmpIcon(fileIMG + "정2.BMP", 10); //정지
	m_grid->SetBmpIcon(fileIMG + "정1.BMP", 11); //정리
	m_grid->SetBmpIcon(fileIMG + "배.BMP", 12);  //배분
	m_grid->SetBmpIcon(fileIMG + "임.BMP", 13);  //임의
	m_grid->SetBmpIcon(fileIMG + "과.BMP", 14);  // 2012.10.31 KSJ 단기과열 추가
	m_grid->SetBmpIcon(fileIMG + "위1.BMP", 15); // 2013.03.18 KSJ 위험예정
	m_grid->SetBmpIcon(fileIMG + "권.BMP", 16);  // 2013.03.18 KSJ 권리
	m_grid->SetBmpIcon(fileIMG + "배1.BMP", 17); // 2013.03.18 KSJ 배당
	m_grid->SetBmpIcon(fileIMG + "액.BMP", 18);  // 2013.03.18 KSJ 액면
	m_grid->SetBmpIcon(fileIMG + "병.BMP", 19);  // 2013.03.18 KSJ 병합
	m_grid->SetBmpIcon(fileIMG + "합.BMP", 20);  // 2013.03.18 KSJ 합병
	m_grid->SetBmpIcon(fileIMG + "감1.BMP", 21); // 2013.03.18 KSJ 감자
	m_grid->SetBmpIcon(fileIMG + "감.BMP", 22);  // 2013.03.18 KSJ 감리
	m_grid->SetBmpIcon(fileIMG + "유.BMP", 23);  // 2016.06.14 KSJ 유동
	m_grid->SetBmpIcon(fileIMG + "급.BMP", 24);  // 2017.03.08 KDK 급등

	m_bMoveCfg = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVECFG);
	m_nMoveSave = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVESAVE);

	InitPopupMenu();

	m_btReload.Create("\n그룹 다시 불러오기", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_RELOAD);
	m_btDomino.Create("\n복수현재가2 종목연동", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_DOMINO);
	m_btSAVE.Create("\n관심종목저장", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_SAVE);
	m_btCLOSE.Create("\n그룹초기화", CRect(0, 0, SIZE_BUTTONS, SIZE_BUTTONS), this, IDC_BT_ERASE);

	HBITMAP hBITMAP, hBITMAP_DN, hBITMAP_HV;


	hBITMAP    = getAxBitmap(fileIMG + "btn_Refresh.bmp")->operator HBITMAP();
	hBITMAP_DN = getAxBitmap(fileIMG + "btn_Refresh_DN.bmp")->operator HBITMAP();
	hBITMAP_HV = getAxBitmap(fileIMG + "btn_Refresh_EN.bmp")->operator HBITMAP();

	m_btReload.SetFont(m_pFont, false);
	m_btReload.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	hBITMAP    = getAxBitmap(fileIMG + "GTI_MATRIX2.bmp")->operator HBITMAP();
	hBITMAP_DN = getAxBitmap(fileIMG + "GTI_MATRIX2_dn.bmp")->operator HBITMAP();
	hBITMAP_HV = getAxBitmap(fileIMG + "GTI_MATRIX2_en.bmp")->operator HBITMAP();

	m_btDomino.SetFont(m_pFont, false);
	m_btDomino.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	hBITMAP    = getAxBitmap(fileIMG + "SAVEINT.bmp")->operator HBITMAP();
	hBITMAP_DN = getAxBitmap(fileIMG + "SAVEINT_dn.bmp")->operator HBITMAP();
	hBITMAP_HV = getAxBitmap(fileIMG + "SAVEINT_en.bmp")->operator HBITMAP();

	m_btSAVE.SetFont(m_pFont, false);

	m_btSAVE.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	hBITMAP    = getAxBitmap(fileIMG + "닫기1.bmp")->operator HBITMAP();
	hBITMAP_DN = getAxBitmap(fileIMG + "닫기1_dn.bmp")->operator HBITMAP();
	hBITMAP_HV = getAxBitmap(fileIMG + "닫기1_en.bmp")->operator HBITMAP();

	m_btCLOSE.SetFont(m_pFont, false);
	m_btCLOSE.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);

	RemoveAll();

	// 2016.04.11 KSJ 처음열었을때 트리메뉴보고 필드셋업
	if (CAST_TREEID(m_kind)->kind == xEPBCODE)
		FieldSetup(false, 4, true);
	else
		FieldSetup(false);

	if (m_hSignalMng)
	{
		CString strRealType;
		strRealType.Format("%d", (int)TYPE_SIGNAL_ALERT);
		SignalLib_SetSignal(true, this, strRealType, "", "8");
	}

	m_btReload.Invalidate(TRUE);
	m_btDomino.Invalidate(TRUE);
	m_btSAVE.Invalidate(TRUE);
	m_btCLOSE.Invalidate(TRUE);
}

CString CGridWnd::CalMaketTime(CString strTime, bool bEnd)
{
	CString strHour, strMinute, strData;

	int startIndex = strTime.Find(_T("시"));
	strHour = strTime.Left(startIndex);

	strMinute = strTime.Mid(startIndex);
	strMinute.Replace("시", "");
	strMinute.Replace("분", "");

	if (bEnd)  //마감 1초 빼줘야 한다.
	{
		if (atoi(strMinute) == 0) {
			strData.Format("%02d%02d59", atoi(strHour) - 1, 59);
		}
		else {
			strData.Format("%02d%02d59", atoi(strHour), atoi(strMinute) - 1);
		}
	}
	else  //시작시간
	{
		strData.Format("%02d%02d00", atoi(strHour), atoi(strMinute));
	}

	return strData;
}

// 2013.04.30 bResize SendGrid에서 호출될때는 리사이징 하지 않는다.
void CGridWnd::FieldSetup(bool bDlgSetup, int iEqualizer, bool bResize)
{
	const int nIndex = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_GETAFIELD);
	m_bongField = -1;
	m_sonikField = -1;
	m_suikField = -1;
	m_yDayVolField = -1;
	m_recomDataField = -1;
	m_recomBasicCostField = -1;
	m_recomSuikrateField = -1;
	m_recomBMjisuField = -1;
	m_recomBMSuikrateField = -1;
	m_recomgoalCostField = -1;
	m_recomJisuGubnField = -1;
	m_upjongCode = -1;
	m_EqualizerField = -1;
	m_beforeVolField = -1;
	m_newCurrField = -1;
	m_epbDataField = -1; // 2016.04.15 KSJ ebp 추천일자

	m_ccField = false;
	m_posField = false;


	m_grid->Clear();


	loadField(nIndex, bDlgSetup, iEqualizer);

	initRSymbol();
	initialGrid(bResize);

	SetGridinfo(TRUE);

	// ADD PSH 20070918
	MarkerSetup();
	//	m_grid->MakeEqualizerBmp();
	// END ADD
}

void CGridWnd::OperDestory()
{
	savecfg();
	m_btReload.DestroyWindow();
	m_btDomino.DestroyWindow();
	m_btSAVE.DestroyWindow();
	m_btCLOSE.DestroyWindow();
	ClearInterest();

	//	m_grid->DestroyWindow();

	KillTimer(1000);
}

void CGridWnd::DrawTitle(CDC *pDC)
{
	const int nCurSel = m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_GET));
	const BOOL bFocus = (nCurSel == m_nIndex) ? TRUE : FALSE;
	const int nmode = pDC->SetBkMode(TRANSPARENT);
	
	pDC->FillSolidRect(m_rcTitle,  (bFocus) ? m_clrFOCUS[0] : m_clrFOCUS[1]);
	pDC->Draw3dRect(m_rcTitle, RGB(200, 200, 200), RGB(200, 200, 200));
	
	const COLORREF clr = pDC->SetTextColor((bFocus) ? m_clrTEXT[0] : m_clrTEXT[1]);
	CFont *oldfont = nullptr;
	if (bFocus)
		oldfont = pDC->SelectObject(m_pFontB);

	if (IsInterest())
	{
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	CRect rect = m_rcTitle;
	rect.OffsetRect(0, 1);
	pDC->DrawText(m_szTitle, rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
	pDC->SetTextColor(clr);
	if (oldfont)
		pDC->SelectObject(oldfont);
	pDC->SetBkMode(nmode);
}

void CGridWnd::OperDraw(CDC *pDC)
{
	DrawTitle(pDC);
}

void CGridWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SendMessage(WM_MANAGE, MK_CLICKGROUP);
	CBaseWnd::OnLButtonDown(nFlags, point);
}

// ADD PSH 20070912
void CGridWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_rcTitle.PtInRect(point))
	{
		ShowPopupMenu();
	}

	CBaseWnd::OnRButtonUp(nFlags, point);
}
// END ADD

void CGridWnd::OperResize(int cx, int cy)
{
	CRect rect, rcButton;
	rect = m_rcTitle = m_rect;
	m_rcTitle.bottom = m_rcTitle.top + 24; //@@Zeta m_size.cy + 1;
	rcButton = m_rcTitle;

	rcButton.right -= 1;
	rcButton.top += 2;
	rcButton.left = rcButton.right - SIZE_BUTTONS - 1;
	rcButton.bottom = rcButton.top + SIZE_BUTTONS;
	m_btCLOSE.MoveWindow(rcButton, TRUE);
	rcButton.OffsetRect(-SIZE_BUTTONS - 2, 0);
	m_btSAVE.MoveWindow(rcButton, TRUE);
	rcButton.OffsetRect(-SIZE_BUTTONS - 2, 0);
	m_btDomino.MoveWindow(rcButton, TRUE);
	rcButton.OffsetRect(-SIZE_BUTTONS - 2, 0);
	m_btReload.MoveWindow(rcButton, TRUE);

	rect.top = m_rcTitle.bottom;

	m_grid->MoveWindow(rect, TRUE);
	m_grid->Adjust();

	if (IsInit())
	{
		m_grid->ShowWindow(SW_SHOW);
		m_btReload.ShowWindow(true);
		m_btDomino.ShowWindow(true);
		m_btSAVE.ShowWindow(true);
		m_btCLOSE.ShowWindow(true);
		m_btReload.Invalidate(TRUE);
		m_btDomino.Invalidate(TRUE);
		m_btSAVE.Invalidate(TRUE);
		m_btCLOSE.Invalidate(TRUE);
	}

	InvalidateRect(m_rcTitle);
	InvalidateRect(rect);

	m_btReload.ShowWindow(SW_SHOW);
	m_btDomino.ShowWindow(SW_SHOW);
	m_btSAVE.ShowWindow(SW_SHOW);
	m_btCLOSE.ShowWindow(SW_SHOW);

	m_btReload.Invalidate(TRUE);
	m_btDomino.Invalidate(TRUE);
	m_btSAVE.Invalidate(TRUE);
	m_btCLOSE.Invalidate(TRUE);
}

void CGridWnd::InsertRowNCode(int row)
{
	if (row <= 0)
		return;

	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	int rowcount = 0;

	if (nOver == MO_VISIBLE)
	{
		pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
		rowcount = ((CGroupWnd *)pWnd)->sumEachGroupCount();
	}
	else
	{
		rowcount = GetRowValidcount();
	}

	if (rowcount >= MAX_LINE)
	{
		Variant(guideCC, IH::idTOstring(IDS_GUIDE4));

		return;
	}

	insertInterest(row - 1);
	insertRow(row);
	SetLineColor();
	m_grid->getmemoCheck();
	m_grid->memoRefresh();
	m_grid->SetFocusCellEdit(row, colNAME, true);

	if (m_bAddCnd)
		saveInterest();
}

LONG CGridWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG ret = 0;
	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int retArrange = pWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);

	// DWORD* data;
	CString strTemp;

	switch (LOWORD(wParam))
	{
	case MK_SISECATCH:
		OperAlarm(HIWORD(wParam), (char *)lParam);
		break;
	case MK_MEMO:
		m_grid->getmemoCheck();
		Invalidate(FALSE);
		break;
	case MK_DELETEMEMO:
		break;
	case MK_NEWS:
		queryNewsCode();
		break;
	case MK_GETDATAKIND:
		ret = GetKind();
		break;
	case MK_CHART:
		ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_CHART);
		break;
	case MK_TURNCODE:
	{
		const int nsec = HIWORD(wParam);
		if (nsec == 0)
		{
			KillTimer(1000);
			m_nTurn = 0;
		}
		else
		{
			if (CAST_TREEID(m_kind)->kind == xISSUE)
				break;

			m_nTurn = 0;
			SetTimer(1000, nsec * 1000, nullptr);
		}
	}
	break;
	case MK_EXCEL:
		m_grid->RunExcel();
		break;
	case MK_HIDECODE:
		m_grid->HideCode();
		break;
	case MK_SELGROUP:
		InvalidateRect(m_rcTitle, FALSE);
		break;
	case MK_CLICKGROUP:
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)m_nIndex);
		break;
	case MK_TREEDATA:
		ParseData((class CGridData *)lParam);
		break;
	case MK_UPJONGDATA:
		ParseUpjongData((class CGridData *)lParam);
		break;
	case MK_REMAINDATA:
		ParseRemainData((class CGridData *)lParam);
		break;
	case MK_SENDTR:
		sendTransaction();
		break;
	case MK_RECVDATA:
		RecvOper(HIWORD(wParam), (CRecvData *)lParam);
		break;
	case MK_RTSDATA:
		// 		RecvRTS((char*)lParam);
		break;
		// 2012.01.19 KSJ Alertx 추가
	case MK_RTSDATAx:
		//RecvRTSx(lParam);
		break;
		// 2012.01.19 KSJ Alertx 추가 끝
	case MK_ENDDRAG:
		m_drag = m_drop = -1;
		m_grid->FreeDragDrop();
		CintGrid::m_dropdata.Reset();
		break;
	case MK_INSERTROW:
		ret = insertRow(HIWORD(wParam));
		break;
	case MK_INSERTCODE:
		if (retArrange == 0)
		{

			if (CAST_TREEID(m_kind)->kind == xINTEREST)
				m_bEditWork = TRUE;

			InsertRowNCode(HIWORD(wParam));
		}

		break;
	case MK_DELETEROW:
		DeleteRow(HIWORD(wParam));
		insertInterest(MAX_LINE - 1);
		SetLineColor();

		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = TRUE;

		break;
	case MK_TRIGGEROW:
		triggerCode(HIWORD(wParam));
		break;
	case MK_SAVE:
	{
		if (GVNM_ENDDRAG == lParam)
		{
			if (!m_bSorting)
			{
				saveInterest();
			}
			else
			{
				if (m_bMoveCfg && (1 == m_nMoveSave))
				{
					RemoveAllBaseInters();
					m_grid->SortBase();
					saveInterest();
				}
				else if (!m_bMoveCfg)
				{
					m_bMoveCfg = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVECFG);
					m_nMoveSave = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVESAVE);

					// 091013 수정
					CMoveSetupDlg dlg;
					dlg.m_nOption = m_nMoveSave;
					dlg.m_bAlert = m_bMoveCfg;

					// dlg 항상 열기 옵션 체크
					if (dlg.m_bAlert)
					{
						if (1 == dlg.m_nOption)
						{
							RemoveAllBaseInters();
							m_grid->SortBase();
							saveInterest();
							m_bEditWork = FALSE;
						}
						else
						{
							if (CAST_TREEID(m_kind)->kind == xINTEREST)
								m_bEditWork = TRUE;
						}
					}
					else
					{
						if (dlg.DoModal())
						{
							if (1 == dlg.m_nOption)
							{
								RemoveAllBaseInters();
								m_grid->SortBase();
								saveInterest();
								m_bEditWork = FALSE;
							}
							else
							{
								if (CAST_TREEID(m_kind)->kind == xINTEREST)
									m_bEditWork = TRUE;
							}
						}
					}

					m_pToolWnd->SendMessage(WM_MANAGE, MK_SETMOVECFG, dlg.m_bAlert);
					m_pToolWnd->SendMessage(WM_MANAGE, MK_SETMOVESAVE, dlg.m_nOption);
				}
			}
		}
		else
		{
			saveInterest();
		}
	}
	// END MODIFY
	break;
	case MK_HAWKEYE:
	{
		const LONG val = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
		hawkeyeAction((val) ? true : false);
		//			Invalidate(FALSE);
	}
	break;
	// ADD PSH 20070911
	case MK_HSCROLL:
	{
	}
	break;
	case MK_SORTBASE:
	{
		if (!m_bNews)
			BaseSorting();
	}
	break;
	// END ADD
	case MK_INSERTNEWS:
		InsertNewsx(_newscode, _newsTitle);
		break;
	case MK_NEWSX:
		parsingNewsx();
		break;
	default:
		ret = CBaseWnd::OnManage(wParam, lParam);
		break;
	}
	return ret;
}

void CGridWnd::triggerCode(int row)
{
	m_ClickRow = row;
	CString code = m_grid->GetItemText(row, colCODE);
	CString string;

	code.TrimLeft();
	code.TrimRight();

	if (code.IsEmpty())
		return;

	if (((CGroupWnd *)m_pGroupWnd)->GetSelAction() == 0)
		return;

	switch (GetCodeType(code))
	{
	case kospiCODE:
		string.Format("%s\t%s", HCOD, code);
		break;
	case futureCODE:
		string.Format("%s\t%s", "ed_focod", code); // 2013.01.03 KSJ 2200선물종합화면에도 트리거보냄
		Variant(triggerCC, string);

		string.Format("%s\t%s", FCOD, code);
		break;
	case optionCODE:
		string.Format("%s\t%s", "ed_focod", code); // 2013.01.03 KSJ 2200선물종합화면에도 트리거보냄
		Variant(triggerCC, string);

		string.Format("%s\t%s", OCOD, code);
		break;
	case foptionCODE:
		string.Format("%s\t%s", "ed_focod", code); // 2013.01.03 KSJ 2200선물종합화면에도 트리거보냄
		Variant(triggerCC, string);

		string.Format("%s\t%s", PCOD, code);
		break;
	case indexCODE:
		string.Format("%s\t%s", UCOD, code);
		break;
	case sinjuCODE:
		string.Format("%s\t%s", SINJUCOD, code);
		break;
	case elwCODE:
		string.Format("%s\t%s", HCOD, code);
		Variant(triggerCC, string);
		//					m_grid->SetFocus();
		string.Format("%s\t%s", ELWCOD, code);
		break;
	case thirdCODE:
		string.Format("%s\t%s", THIRDCOD, code);
		break;
	case 0:							  // 2012.10.11 KSJ 주식, 금리, 통화선물 추가
		if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물   //파생상품 코드개편
		{
			int nGubn = atoi(code.Mid(1, 2));
			CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ 주식선물 신규종목 연동

			if (nGubn > 10 && nGubn < 60) //주식선물
			{
				string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
				Variant(triggerCC, string);

				string.Format("%s\t%s", SFOD, code);
				break;
			}
			else if (nGubn > 60 && nGubn <= 70) //금리선물
			{
				string.Format("%s\t%s", SFOD, code);
				break;
			}
			else if (nGubn > 70 && nGubn < 80) //통화선물
			{
				string.Format("%s\t%s", SFOD, code);
				break;
			}
			else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
				|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
				|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z")) // 2024.10.31 주식선물
			{
				string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
				Variant(triggerCC, string);

				string.Format("%s\t%s", SFOD, code);
				break;
			}
		}
	}
	Variant(triggerCC, string);
	Variant(codeCC, string);

	m_grid->SetFocus();
}

void CGridWnd::RecvOper(int kind, CRecvData *rdata)
{
	struct _trkey *trkey = (struct _trkey *)&kind;
	const int group = (trkey->kind == TRKEY_OVER) ? trkey->group + 8 : trkey->group;
	if (group != (UINT)m_nIndex)
		return;

	switch (trkey->kind)
	{
	case TRKEY_GRIDUPJONG:
	case TRKEY_GRIDNEW:
	case TRKEY_OVER:
		parsingOubs(rdata->data, rdata->size);
		// Map 초기화 KSI
		ClearSearchMap();
		ReSetSearchMap(); // 2011.12.29 KSJ
		break;
	case TRKEY_GRIDNEWS:
		parsingNewsCode(rdata);
		break;
	case TRKEY_GRIDROW:
		parsingOubsOne(rdata->data, rdata->size, m_updateROW);
		// Map 초기화 KSI
		ClearSearchMap();
		ReSetSearchMap(); // 2011.12.29 KSJ
		break;
	case TRKEY_GRIDSAVE:
		uploadOK(); // 2016.07.11 KSJ 그룹저장(G)한 다음에 최종상태저장(V) 보낸다.
		break;
	case TRKEY_RECOMMAND:
		parseReCommandData(rdata->data, rdata->size);
		break;
	case TRKEY_LASTSET: // 2016.07.11 KSJ 'V' 날림.
		break;
	}
}

void CGridWnd::AddData_Overoper(int nIndex, CString sztmp)
{
	BOOL bInt = FALSE;
	CString code, amount, price, bookmark;

	// ADD PSH 20070913
	CString strName;

	if (0 == nIndex)
	{
		const UINT attr = m_grid->GetItemAttr(nIndex + 1, colNAME) & ~GVAT_MARKER;
		m_grid->SetItemAttr(nIndex + 1, colNAME, attr);
	}
	else
	{
		insertRow(nIndex + 1);
	}
	// END ADD

	code = IH::Parser(sztmp, PTAB);
	code.Trim();

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
	{
		bInt     = TRUE;
		amount   = IH::Parser(sztmp, PTAB);
		price    = IH::Parser(sztmp, PTAB);
		strName  = IH::Parser(sztmp, PTAB);
		bookmark = IH::Parser(sztmp, PTAB);
	}

	const int count = _vInters.size();
	if (!bInt && code.IsEmpty() && (count > nIndex))
		return;

	auto& pinters = _vInters.at(nIndex);
	pinters.get()->gubn = '0';
	pinters.get()->code = code;

	if (bInt)
	{
		pinters.get()->xnum = amount;
		pinters.get()->xprc = price;
		pinters.get()->bookmark = bookmark[0];
	}

	// ADD PSH 20070914
	if (!code.IsEmpty() && code.GetAt(0) == 'm')
	{
		//pinters->gubn = 'm';    //test mod 'm'
		pinters.get()->gubn = ROW_COMMENT;
		pinters.get()->name = strName;

		const UINT attr = m_grid->GetItemAttr(nIndex + 1, colNAME) | GVAT_MARKER;
		m_grid->SetItemAttr(nIndex + 1, colNAME, attr);
		m_grid->SetItemText(nIndex + 1, colNAME, strName);

		return;
	}
	// END ADD
}

void CGridWnd::AddData(int nIndex, CString sztmp)
{
	BOOL bInt = FALSE;
	vector<CString> vitem;
	const enum { eamount, eprice, ename, ebookmark, efutureGubn, ecreditPrc, emaeipPrc, edate, ebookrgb, ememoYn };

	// ADD PSH 20070913
	CString strName;

	if (0 == nIndex)
	{
		const UINT attr = m_grid->GetItemAttr(nIndex, colNAME & ~GVAT_MARKER);
		m_grid->SetItemAttr(nIndex, colNAME, attr);
	}
	else
	{
		insertRow(nIndex);
	}

	CString code = IH::Parser(sztmp, PTAB);
	code.Trim();

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
	{
		bInt = TRUE;
		AxStd::SplitSub(vitem, sztmp, PTAB);
	}

	const int count = _vInters.size();
	if (!bInt && code.IsEmpty() && (count > nIndex))
		return;

	if (count <= nIndex)
		return;
	
	auto& pinters = _vInters.at(nIndex);
	pinters.get()->gubn = '0';
	pinters.get()->code = code;

	if (bInt && vitem.size() > 8 && CAST_TREEID(m_kind)->kind == xINTEREST)
	{
		pinters.get()->xnum       = vitem[eamount];
		pinters.get()->xprc       = vitem[eprice];
		pinters.get()->name       = vitem[ename];
		pinters.get()->bookmark   = vitem[ebookmark][0];
		pinters.get()->futureGubn = vitem[efutureGubn][0];
		pinters.get()->creditPrc  = atof(vitem[ecreditPrc]);
		pinters.get()->maeipPrc   = atof(vitem[emaeipPrc]);
		pinters.get()->bookrgb    = vitem[ebookrgb];
		pinters.get()->bMemo      = vitem[ememoYn];

		pinters.get()->bookmark = vitem[ebookrgb].CompareNoCase("00") ? '1':'0';
		if (vitem[ebookrgb].IsEmpty())
			pinters.get()->bookmark = '0';

		if (vitem[ebookmark] == "1")
			pinters.get()->gubn = ROW_BOOKMARK;
	}

	// ADD PSH 20070914
	if (!code.IsEmpty() && code.GetAt(0) == 'm')
	{
		//pinters->gubn = 'm';    //test mod 'm'
		pinters.get()->gubn = ROW_COMMENT;
		pinters.get()->name = strName;
		const UINT attr = m_grid->GetItemAttr(nIndex + 1, colNAME);
		m_grid->SetItemAttr(nIndex + 1, colNAME, attr | GVAT_MARKER);
		m_grid->SetItemText(nIndex + 1, colNAME, strName);

		return;
	}
	else
	{
		m_grid->SetItemAttr(nIndex + 1, colNAME, m_grid->GetItemAttr(nIndex + 1, colNAME) & ~GVAT_MARKER);
	}
	// END ADD
}

void CGridWnd::RemoveAll()
{
	ClearInterest();

	for (int ii = 0; ii < MAX_LINE; ii++)
	{
		auto& pinters = _vInters.emplace_back(std::make_shared<struct _intersx>());
		pinters.get()->gubn = '0';
	}

	// ADD PSH 20070917
	int nRow = m_grid->GetRowCount() - 1;
	for (; nRow > 1; nRow--)
	{
		m_grid->DeleteRow(nRow);
		m_grid->memoDelete(nRow);
	}

	for (int i = 0; i < m_colC; i++)
	{
		m_grid->SetItemText(1, i, "");
		m_grid->SetItemData(1, i, 0);
	}


	m_rowC = 2; // BUFFET 2->1
	// END ADD

	// 2012.03.22 KSJ 같은그룹을 다른 화면에서 띄울때 첫번째 줄에서 실시간 데이터를 받았음.
	ClearSearchMap(); // 2011.12.29 KSJ
	ReSetSearchMap(); // 2011.12.29 KSJ
}

void CGridWnd::ParseRemainData(class CGridData *sdata)
{
	RemoveAll();

	SetKind(sdata->GetKind());
	InvalidateRect(m_rcTitle, FALSE);
	ClearInterest();

	CString sztmp = _T("");
	CString code, amount, price, name, strBookmark;
	char bookmark[1];
	bookmark[0] = '0';

	const int ncnt = sdata->GetCount();
	BOOL bInt = FALSE;

	if (CAST_TREEID(m_kind)->kind == xREMAIN)
		bInt = TRUE;

	int ii = 0;
	for (ii = 0; ii < ncnt; ii++)
	{
		sztmp = sdata->m_arDatas.GetAt(ii);
		code = IH::Parser(sztmp, PTAB);
		code.Trim();


		if (!bInt && code.IsEmpty())
			continue;

		auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
	//	pinters = new _intersx();
	//	ZeroMemory(pinters, sz_inters);

		pinters.get()->gubn = '0';
		pinters.get()->code = code;

		if (pinters.get()->code[0] == 'm')
		{
			//pinters->gubn = 'm';  //test mod
			pinters.get()->gubn = ROW_COMMENT;
		}

		if (bInt || !sztmp.IsEmpty())
		{
			amount = IH::Parser(sztmp, PTAB);
			price = IH::Parser(sztmp, PTAB);
			name = IH::Parser(sztmp, PTAB);
			strBookmark = IH::Parser(sztmp, PTAB);

			if (strBookmark.Compare("1")) // 2015.04.03 KSJ 1이아니면 0으로 해준다.
			{
				strBookmark = "0";
			}

			// END ADD
			pinters.get()->xnum = amount;
			pinters.get()->xprc = price;
			pinters.get()->name = name;
			//북마크 추가
			pinters.get()->bookmark = strBookmark[0];
			// END ADD
		}

		if (ii != 0)
		{
			insertRow(ii, FALSE);
		}
	}

	for (; ii < MAX_LINE; ii++)
	{
		auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
		pinters.get()->gubn = '0';
		//AddInters(pinters);
	}

	// ADD PSH 20070918
	MarkerSetup();
	// END ADD
}

void CGridWnd::parseReCommandData(char *datB, int datL)
{
//	struct mod
//	{
//		char nrec[4];	     /*	갯수			*/
//		char acod[1024][12]; /*  종목코드 리스트 */
//	};
//
//#define sz_mod sizeof(struct mod)
//
//	struct mod *mod = nullptr;
//	mod = (struct mod *)new char[sz_mod]{};
//	ZeroMemory(mod, sz_mod);
//
//	class CGridData sdata;
//
//	mod = (struct mod *)datB;
//
//	CString strCnt = CString(mod->nrec, 4);
//	const int nCnt = atoi(strCnt);
//
//	CString code, name;
//
//	for (int i = 0; i < nCnt; i++)
//	{
//		code = CString(mod->acod[i], 12);
//		code.TrimLeft();
//		code.TrimRight();
//
//		if (!code.IsEmpty())
//		{
//			sdata.m_arDatas.Add(code);
//		}
//	}
//
//	ParseData(&sdata);
//	sendTransaction();
}

void CGridWnd::ParseUpjongData(class CGridData *sdata)
{
	RemoveAll();
	ClearInterest();

	CString sztmp = _T("");
	CString code;

	const int ncnt = sdata->GetCount();
	const BOOL bInt = FALSE;
	int ii = 0;
	for (ii = 0; ii < ncnt; ii++)
	{
		sztmp = sdata->m_arDatas.GetAt(ii);
		code = IH::Parser(sztmp, PTAB).Trim();

		if (code.IsEmpty())
			continue;

		auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
		pinters.get()->gubn = '0';
		pinters.get()->code = code;
		if (ii != 0)
			insertRow(ii, FALSE);
	}

	for (; ii < MAX_LINE; ii++)
	{
		auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
		pinters.get()->gubn = '0';
	}
	sendTransaction(200);
}

void CGridWnd::ParseData(class CGridData *sdata)
{
	RemoveAll();
	SetKind(sdata->GetKind());
	InvalidateRect(m_rcTitle, FALSE);
	ClearInterest();
	m_arEpbDate.RemoveAll(); // 2016.04.15 KSJ 여기서만 리셋하면 된다.


	CString sztmp = _T("");
	CString code, amount, price, name, strBookmark, futureGubn, creditPrc, maeipPrc, date = _T(""), bookrgb, MemoYN;
	char bookmark[1];
	bookmark[0] = '0';

	const int ncnt = sdata->GetCount();
	BOOL bInt = FALSE;

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
		bInt = TRUE;

	int ii = 0;
	for (ii = 0; ii < ncnt; ii++)
	{
		sztmp = sdata->m_arDatas.GetAt(ii);
		code = IH::Parser(sztmp, PTAB);

		code.TrimLeft();
		code.TrimRight();

		if (!bInt && code.IsEmpty())
			continue;

		auto& item = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
		const auto& pinters = item.get();

		pinters->gubn = '0';
		pinters->code = code;
		if (pinters->code[0] == 'm')
		{
			//pinters->gubn = 'm'; //test mod 'm'  파일을 읽어서 런타임 vector에 할당
			pinters->gubn = ROW_COMMENT;
		}

		if (bInt || !sztmp.IsEmpty())
		{
			amount = IH::Parser(sztmp, PTAB);
			price = IH::Parser(sztmp, PTAB);
			name = IH::Parser(sztmp, PTAB);
			strBookmark = IH::Parser(sztmp, PTAB);
			futureGubn = IH::Parser(sztmp, PTAB);
			creditPrc = IH::Parser(sztmp, PTAB);
			maeipPrc = IH::Parser(sztmp, PTAB);
			date = IH::Parser(sztmp, PTAB); // 2016.04.15 KSJ 추천일자
			bookrgb = IH::Parser(sztmp, PTAB);
			MemoYN = IH::Parser(sztmp, PTAB); //test memo

			// END ADD
			pinters->xnum = amount;
			pinters->xprc = price;
			pinters->name = name;
			pinters->futureGubn = futureGubn[0];
			pinters->creditPrc = atof(creditPrc);
			pinters->maeipPrc = atof(maeipPrc);
			pinters->bookrgb = bookrgb;
			pinters->bMemo = MemoYN;
			
			
			strBookmark = bookrgb.CompareNoCase("00") ? "1":"0";
			if (bookrgb.IsEmpty())
				strBookmark = "0";

 			pinters->bookmark = strBookmark[0];	
			if (strBookmark == "1")
				pinters->gubn = ROW_BOOKMARK;
		}

		m_arEpbDate.Add(date); // 2016.04.15 KSJ ebp 추천일자

		if (ii != 0)
			insertRow(ii, FALSE);
	}

	for (; ii < MAX_LINE; ii++)
	{
		auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
		pinters.get()->gubn = '0';
	}

	_arrBaseInters.clear();
	rebuildInterest(); // 2013.05.21 KSJ 여기서 m_inter와 baseinter를 같게 만들어준다.
	// ADD PSH 20070918

	MarkerSetup();

	// END ADD
}

int CGridWnd::loadcfg_data()
{
	const int result = GetPrivateProfileInt(m_section, KEY_DATA, 0, m_fileCFG);

	return result;
}

void CGridWnd::savecfg_data(CString keydata)
{
	WritePrivateProfileString(m_section, KEY_DATA, keydata.operator LPCTSTR(), m_fileCFG);
}

void CGridWnd::loadcfg()
{
	const int confirm_start = GetPrivateProfileInt(m_section, KEY_DATA, 9999, m_fileCFG);
	if (confirm_start == 9999)
	{
		m_bfirstStart = TRUE;
	}
	else
	{
		m_bfirstStart = FALSE;
	}

	// data kind
	m_kind = GetPrivateProfileInt(m_section, KEY_DATA, 0, m_fileCFG);
	if (CAST_TREEID(m_kind)->kind == xREMAIN)
		m_kind = 0;

	char buf[1024];

	memset(buf, 0x00, sizeof(buf));
	GetPrivateProfileString(m_section, KEY_FIELD, "", buf, sizeof(buf), m_fileCFG);
	m_szFIELD.Format("%s", buf);

	memset(buf, 0x00, sizeof(buf));
	GetPrivateProfileString(m_section, KEY_WIDTH, "", buf, sizeof(buf), m_fileCFG);
	m_szWIDTH.Format("%s", buf);

	// 2013.07.12 KSJ ,0,0,0,0이 들어가있으면 지우고 저장한다.
	// 7852~7855까지 추가한걸 잘못 반영했다가 생긴 어쩔수 없는 코딩
	if (m_szWIDTH.Find(",0,0,0,0"))
	{
		m_szWIDTH.Replace(",0,0,0,0", "");
		WritePrivateProfileString(m_section, KEY_WIDTH, m_szWIDTH, m_fileCFG);
	}
	// KSJ

	// ADD PSH 20070912
	memset(buf, 0x00, sizeof(buf));
	GetPrivateProfileString(SEC_MAIN, KEY_MARGIN, "", buf, sizeof(buf), m_fileCFG);

	if (m_bfirstStart == TRUE)
	{
		m_bMargin = 0;
	}
	else
	{
		m_bMargin = atoi(buf);
	}

	DWORD dwRes;

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "BKGCLR", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_clrMarkerBKG = RGB(240, 240, 240);
	}
	else
	{
		m_clrMarkerBKG = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TXTCLR", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_clrMarkerTXT = RGB(142, 142, 142);
	}
	else
	{
		m_clrMarkerTXT = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TXTSHD", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_bShadow = TRUE;
	}
	else
	{
		m_bShadow = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "ALLAPPL", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_bAllApply = TRUE;
	}
	else
	{
		m_bAllApply = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(SEC_MAIN, KEY_SYMINFO, "", buf, sizeof(buf), m_fileCFG);

	if (m_bfirstStart == TRUE)
	{
		m_bInfo = 1;
	}
	else
	{
		if (0 == dwRes)
		{
			m_bInfo = FALSE;
		}
		else
		{
			m_bInfo = atoi(buf);
			dwRes = 0;
		}
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKAPPL", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_bApply = FALSE;
	}
	else
	{
		m_tkConfig.m_bApply = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKVOL", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_vol = 0;
	}
	else
	{
		m_tkConfig.m_vol = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKAMT", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_amt = 0;
	}
	else
	{
		m_tkConfig.m_amt = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKAND", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_and = FALSE;
	}
	else
	{
		m_tkConfig.m_and = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKPRC", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_price = FALSE;
	}
	else
	{
		m_tkConfig.m_price = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKSPRC", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_sprc = 0;
	}
	else
	{
		m_tkConfig.m_sprc = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKEPRC", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_eprc = 0;
	}
	else
	{
		m_tkConfig.m_eprc = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKULIM", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_ulimit = TRUE;
	}
	else
	{
		m_tkConfig.m_ulimit = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKUP", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_up = TRUE;
	}
	else
	{
		m_tkConfig.m_up = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKFLAT", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_flat = TRUE;
	}
	else
	{
		m_tkConfig.m_flat = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKDLIM", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_dlimit = TRUE;
	}
	else
	{
		m_tkConfig.m_dlimit = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "TKDOWN", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_tkConfig.m_down = TRUE;
	}
	else
	{
		m_tkConfig.m_down = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "ALLAUTO", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_bAutoSaveAllApply = FALSE;
	}
	else
	{
		m_bAutoSaveAllApply = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "ADDCND", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_bAddCnd = FALSE;
	}
	else
	{
		m_bAddCnd = (BOOL)atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "CHGCND", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_bChgCnd = FALSE;
	}
	else
	{
		m_bChgCnd = (BOOL)atoi(buf);
		dwRes = 0;
	}

	// 2011.12.02 KSJ
	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "NEWSNCHARTWIDTH", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_nNewsNChartWidth = 20;
	}
	else
	{
		m_nNewsNChartWidth = atoi(buf);
		dwRes = 0;
	}
	// KSJ

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "CODEWIDTH", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_nCodeWidth = 80;
	}
	else
	{
		m_nCodeWidth = atoi(buf);
		dwRes = 0;
	}

	memset(buf, 0x00, sizeof(buf));
	dwRes = GetPrivateProfileString(m_section, "GRIDWIDTH", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		m_nGridWidth = 0;
	}
	else
	{
		m_nGridWidth = atoi(buf);
		dwRes = 0;
	}

	//처음 한번만 세팅해주자
	dwRes = GetPrivateProfileString(m_section, "ISFIRSTGRIDSETINFO", "", buf, sizeof(buf), m_fileCFG);

	if (0 == dwRes)
	{
		SetGridinfo();
	}

	// END ADD
}

void CGridWnd::savecfg()
{
	CString slog;
	// data kind
	CString str;

	if (CAST_TREEID(m_kind)->kind == xREMAIN)
		m_kind = 0;

	str.Format("%d", m_kind);
	WritePrivateProfileString(m_section, KEY_DATA, str.operator LPCTSTR(), m_fileCFG);

	int nWidth = 0;
	const int ncnt = m_gridHdrX.GetSize();
	struct _gridHdr grdHdr{};
	CString sztmp, szWIDTH = _T(""), szFIELD = _T("");

	for (int ii = 0; ii < ncnt; ii++)
	{
		grdHdr = m_gridHdrX.GetAt(ii);
		nWidth = m_grid->GetColumnWidth(ii);
		CString strSymbol = (CString)grdHdr.symbol;

		//추천종목필드는 저장하지 않는다
		if (strSymbol.Find("191") > -1)
			continue;

		if (strSymbol.Find("2022") > -1)
			continue;

		if (ii >= colNAME)
		{
			sztmp.Format("%d,", nWidth);
			szWIDTH += sztmp;
		}
		else if (ii == colCODE && 0 < nWidth)
		{
			m_nCodeWidth = nWidth;
		}

		// 2011.12.02 KSJ
		else if (ii == colSIG && 0 < nWidth)
		{
			m_nNewsNChartWidth = nWidth;
		}
		// KSJ

		if (ii > colCURR)
		{
			sztmp.Format("%s,", grdHdr.symbol);
			if (sztmp.Compare("2111,") == 0)
				sztmp.Format("2023,");
			else if (sztmp.Compare("2115,") == 0)
				sztmp.Format("2024,");
			else if (sztmp.Compare("2116,") == 0)
				sztmp.Format("2033,");
			else if (sztmp.Compare("2112,") == 0)
				sztmp.Format("2027,");

			if (grdHdr.needs == 1)
			{
				if (sztmp.Compare("2029,") == 0)
				{
					sztmp = "#1##,";
				}
				else if (sztmp.Compare("2030,") == 0)
				{
					sztmp = "#2##,";
				}
				else if (sztmp.Compare("2031,") == 0)
				{
					sztmp = "#3##,";
				}
			}
			else if (grdHdr.needs == 3)
			{
				if (sztmp.Compare("2029,") == 0)
				{
					sztmp = "#4##,";
				}
				else if (sztmp.Compare("2030,") == 0)
				{
					sztmp = "#5##,";
				}
				else if (sztmp.Compare("2031,") == 0)
				{
					sztmp = "#6##,";
				}
			}

			szFIELD += sztmp;
		}
	}

	WritePrivateProfileString(m_section, KEY_FIELD, szFIELD.operator LPCTSTR(), m_fileCFG);
	WritePrivateProfileString(m_section, KEY_WIDTH, szWIDTH.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_clrMarkerBKG);
	WritePrivateProfileString(m_section, "BKGCLR", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_clrMarkerTXT);
	WritePrivateProfileString(m_section, "TXTCLR", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bShadow);
	WritePrivateProfileString(m_section, "TXTSHD", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bAllApply);
	WritePrivateProfileString(m_section, "ALLAPPL", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_tkConfig.m_bApply);
	WritePrivateProfileString(m_section, "TKAPPL", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_vol); // 체결량 범위   (vol * 1,000)
	WritePrivateProfileString(m_section, "TKVOL", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_amt); // 체결금액 범위 (amt * 10,000,000)
	WritePrivateProfileString(m_section, "TKAMT", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_and); // and / or
	WritePrivateProfileString(m_section, "TKAND", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_price); // 가격설정 flag
	WritePrivateProfileString(m_section, "TKPRC", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_sprc); // 가격설정 시작값
	WritePrivateProfileString(m_section, "TKSPRC", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_eprc); // 가격설정 끝값
	WritePrivateProfileString(m_section, "TKEPRC", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_ulimit); // 상한
	WritePrivateProfileString(m_section, "TKULIM", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_up); // 상승
	WritePrivateProfileString(m_section, "TKUP", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_flat); // 보합
	WritePrivateProfileString(m_section, "TKFLAT", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_dlimit); // 하한
	WritePrivateProfileString(m_section, "TKDLIM", sztmp.operator LPCTSTR(), m_fileCFG);
	sztmp.Format("%d", m_tkConfig.m_down); // 하락
	WritePrivateProfileString(m_section, "TKDOWN", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bAutoSaveAllApply);
	WritePrivateProfileString(m_section, "ALLAUTO", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bAddCnd);
	WritePrivateProfileString(m_section, "ADDCND", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_bChgCnd);
	WritePrivateProfileString(m_section, "CHGCND", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("%d", m_nCodeWidth);
	WritePrivateProfileString(m_section, "CODEWIDTH", sztmp.operator LPCTSTR(), m_fileCFG);
	// END ADD

	// 2011.12.02 KSJ
	sztmp.Format("%d", m_nNewsNChartWidth);
	WritePrivateProfileString(m_section, "NEWSNCHARTWIDTH", sztmp.operator LPCTSTR(), m_fileCFG);
	// KSJ

	//가로크기 저장
	CRect rc;
	GetWindowRect(rc);
	sztmp.Format("%d", rc.Width());
	WritePrivateProfileString(m_section, "GRIDWIDTH", sztmp.operator LPCTSTR(), m_fileCFG);

	sztmp.Format("1"); // 하락
	WritePrivateProfileString(m_section, "ISFIRSTGRIDSETINFO", sztmp.operator LPCTSTR(), m_fileCFG);
}

BOOL CGridWnd::IsInterest()
{
	if (m_kind == 99999)
		return TRUE;

	struct _treeID *treeID = (struct _treeID *)&m_kind;
	if (treeID->kind == xINTEREST)
		return TRUE;
	return FALSE;
}

void CGridWnd::sendTransactionTR(int update, int nStart, int nEnd)
{
	int sendL = 0;
	char tempB[64]{};
	const int bufSize = sizeof(struct _gridHi) + 50 + m_gridHdrX.GetSize() * 5 + _vInters.size() * 12 + 12;
	std::string sendB;
	sendB.resize(bufSize);
	FillMemory(sendB.data(), bufSize, ' ');

	SetInitRecommandInfo();


	_typeAuto = m_pToolWnd->SendMessage(WM_MANAGE, MK_AUTOEXPECT);
	sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, _typeAuto == 2 ? 0 : 1, P_TAB);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	//중요..  여기서 NXT시장 구분값을 날려줌...
	const int type = m_pToolWnd->SendMessage(WM_MANAGE, MK_MARKET);
	sprintf(tempB, "%s%c%d%c", gNXT, P_DELI, type, P_TAB);	// 시장구분자   KRX :1  NXT :2  TOTAL : 3      NXT / KRX : 4 
        CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c%d%c", "90999", P_DELI, 1, P_TAB);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c", gSYMBOL, P_DELI);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	struct _gridHi *gridHi;
	gridHi = (struct _gridHi *)&sendB[sendL];
	sendL += sizeof(struct _gridHi);

	CopyMemory(gridHi->visible, "99", sizeof(gridHi->visible));
	sprintf(tempB, "%04d", _vInters.size());
	CopyMemory(gridHi->rows, tempB, sizeof(gridHi->rows));

	gridHi->type = '0';
	gridHi->dir = '1';
	gridHi->sort = '0';

	for (int ii = nStart; ii < nEnd; ii++)
	{
		strcpy(tempB, (LPTSTR)(LPCTSTR)m_pGridArray.GetAt(ii));
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sprintf(tempB, "2321%c", P_NEW); //전일거래량
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "2950%c", P_NEW); // 2012.06.19 KSJ 배분정보
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "2951%c", P_NEW); // 2012.06.19 KSJ 임의종료
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	sprintf(tempB, "%s%c", CCOD, P_DELI); // 종목코드
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	CString tempStr;
	for_each(_vInters.begin(), _vInters.end(), [&](auto& pinters) {
		sprintf(tempB, "%s%c", pinters.get()->code.IsEmpty() == TRUE ? " " : pinters.get()->code, P_DELI);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	});

	sendB[sendL] = P_NEW;
	sendL += 1;
	sendB[sendL] = P_TAB;
	sendL += 1;
	sendB[sendL] = 0x00;

	CSendData sdata;
	char key{};
	_trkey *trkey = (struct _trkey *)&key;	
	
	if (m_staticUpdate < 0)
	{
		trkey->kind = TRKEY_GRIDNEW;
	}
	else if (m_staticUpdate >= 0 && m_staticUpdate < 200)
	{
		trkey->kind = TRKEY_GRIDROW;
	}
	else
	{
		trkey->kind = TRKEY_GRIDUPJONG;
	}

	if (m_nIndex > 7)
		trkey->kind = TRKEY_OVER;

	// group 개수가  8 이상 넘어가지 못함
	// 8 이상은 TRKEY_OVER로 처리...
	trkey->group = m_nIndex % 8;

	m_updateROW = m_staticUpdate;

	sdata.SetData("pooppoop", key, sendB.data(), sendL, "");

	m_bSending = true;
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
	m_endsort = false;

	sendB.clear();
	m_grid->initFilter();

}

void CGridWnd::sendTransaction(int update)
{
	m_pGridArray.RemoveAll();
	m_staticUpdate = update;

	char tempB[64]{};

	//////////////////////////////////////
	struct _gridHdr gridhdr{};

	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	{
		gridhdr = m_gridHdrX.GetAt(ii);

		sprintf(tempB, "%s%c", gridhdr.symbol, P_NEW);
		m_pGridArray.Add(tempB);
	}
	//////////////////////////////////////
	// 	CWnd* m_pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TREE));
	// 	m_pWnd->SendMessage(WM_MANAGE, MK_GETTREEITEM);

	if (m_pGridArray.GetSize() > GRIDMAXNUM)
	{
		const int nRem = m_pGridArray.GetSize() - GRIDMAXNUM;

		if (!m_bContinue && !m_bSecond)
		{
			m_bContinue = TRUE;

			sendTransactionTR(m_staticUpdate, 0, GRIDMAXNUM);
		}
	}
	else
	{
		m_bContinue = FALSE;
		sendTransactionTR(m_staticUpdate, 0, m_pGridArray.GetSize());
	}
}

void CGridWnd::settingFieldBeforeSend(int &sendL, char *sendB)
{
	char tempB[64]{};
	const int getSize = m_gridHdrX.GetSize();

	if (m_iEndPoint == 0)
	{
		if (getSize > GRIDMAXNUM)
		{
			m_iSendcount = 0; // 연속해서 조회를 해야 한다는 신호를 정해준다
			m_iEndPoint = GRIDMAXNUM;
		}
		else
		{
			m_iEndPoint = m_gridHdrX.GetSize();
		}
	}

	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	{
		const struct _gridHdr xgridHdr = m_gridHdrX.GetAt(ii);
		sprintf(tempB, "%s%c", xgridHdr.symbol, P_NEW);
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sprintf(tempB, "2321%c", P_NEW); //전일거래량
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	m_iStartPoint = m_iEndPoint;
	m_iEndPoint = m_gridHdrX.GetSize();
}

BOOL CGridWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT *pResult)
{
	NM_GVITEM *nmgv = (NM_GVITEM *)lParam;
	if (wParam != IDC_GRID)
		return CBaseWnd::OnNotify(wParam, lParam, pResult);

	switch (nmgv->hdr.code)
	{
	case GVNM_REFRESH:
	{
		m_bEditWork = FALSE;

		// 2016.04.14 KSJ basesorting, 일때 다시한번 조회한다. epb 때문에 추가함.
		if (CAST_TREEID(m_kind)->kind == xISSUE)
		{
			class CGridData sdata;
			sdata.SetKind(m_kind);
			m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, 100), (LPARAM)&sdata); //파일다시 읽어서 sdata 세팅
			// m_pMainWnd->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)&sdata);
			//sdata로 그리드 다시 초기화
			m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_ISSUE, 0), (LPARAM)-1);
		}
		else if (CAST_TREEID(m_kind)->kind != xINTEREST)
		{
			class CGridData sdata;
			sdata.SetKind(m_kind);

			m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_TREEDATA, 100), (LPARAM)&sdata); //파일다시 읽어서 sdata 세팅
			m_pMainWnd->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)&sdata);		  // sdata로 그리드 다시 초기화
		}
		else
		{
			m_pToolWnd->SendMessage(WM_MANAGE, MK_REFRESH);
		}

		ClearSearchMap(); // 2011.12.29 KSJ
		ReSetSearchMap(); // 2011.12.29 KSJ
	}
	break;
	case GVNM_CHGCOLSIZE:
	{
		const int ncnt = m_gridHdrX.GetSize();
		CString sztmp = _T("");
		int nWidth{};
		m_szWIDTH = _T("");
		struct _gridHdr grdHdr
		{
		};
		for (int ii = 0; ii < ncnt; ii++)
		{
			grdHdr = m_gridHdrX.GetAt(ii);
			nWidth = m_grid->GetColumnWidth(ii);
			CString strSymbol = (CString)grdHdr.symbol;

			//추천종목필드는 저장하지 않는다
			if (strSymbol.Find("191") > -1)
				continue;

			if (strSymbol.Find("2022") > -1)
				continue;

			if (ii >= colNAME)
			{
				sztmp.Format("%d,", nWidth);
				m_szWIDTH += sztmp;

				if (ii == colCODE && nWidth != 0)
					m_nCodeWidth = nWidth;

				// 2011.12.02 KSJ
				if (ii == colSIG && nWidth != 0)
					m_nNewsNChartWidth = nWidth;
				// KSJ
			}
		}
		savecfg();
	}
	break;
	case GVNM_CHANGECODE:
	{
		m_grid->SetFocus();
		CString code, oldname, name;
		code = m_grid->GetItemText(nmgv->row, colCODE);

		if (code.GetLength() == 6 || code.GetLength() == 8 || code.GetLength() == 9)
		{
			name = GetCodeName(code);
		}

		//struct _intersx *pinters{};
		const int count = _vInters.size();
		const int xrow = nmgv->row - 1;

		CString strTemp;

		//종목 중복 등록 허용
		if (((CGroupWnd *)GetParent())->GetOverLapAction() == FALSE)
		{
			if (code.GetLength() && IsDuplicateCode(code, nmgv->row))
			{
				CCodeDuplicate dlg;

				// dlg 항상 열기 옵션 체크
				if (dlg.DoModal())
				{
					if (dlg.m_bAlert == FALSE)
					{
						m_grid->SetItemText(nmgv->row, colCODE, "");

						if (m_bAddCnd)
						{
							saveInterest();
						}
						break;
					}
					else
					{
						//파일에 저장
						((CGroupWnd *)GetParent())->saveOverLap(TRUE);
					}
				}
				else
				{
					if (count > xrow)
					{
						auto& pInter = _vInters.at(nmgv->row - 1);
						m_grid->SetItemText(nmgv->row, colCODE, pInter.get()->code);
						DeleteRow(nmgv->row);
						insertInterest(MAX_LINE - 1);
						if (m_bAddCnd)
						{
							saveInterest();
						}
						break;
					}
				}
			}
		}

		if (count > xrow)
		{
			auto& pinters = _vInters.at(xrow);

			// ADD PSH 20070913
		//	const int nRowCnt = m_arrBaseInters.GetSize();

			if (m_bSorting)
			{
				if (!pinters.get()->code.IsEmpty())
				{
					// 2011.12.19 KSJ 소팅하고나서 종목을 바꾸면 여기서 에러남.
					// m_arrBaseInters 사이즈가 0임..
					//CopyInter(m_arrBaseInters.GetAt(xrow), pinters);
					_arrBaseInters.at(xrow) = pinters;
				}
				else
				{
					auto& pBaseInter = _arrBaseInters.emplace_back(std::make_shared<_intersx>());

					pBaseInter.get()->gubn = '0';
					pBaseInter.get()->code = code;
					pBaseInter.get()->name = name;
				}
			}
			// END ADD

			pinters.get()->gubn = '0';
			pinters.get()->code = code;
			pinters.get()->name = name;
			m_grid->SetItemText(xrow + 1, colNAME, name);
			//_vInters.at(xrow) =  pinters;
		}
		else
		{
			for (int ii = count; ii < xrow; ii++)
			{
				auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
				pinters.get()->empty();
			}

			auto& pinters = _vInters.emplace_back(std::move(std::make_shared<_intersx>()));
			pinters.get()->gubn = '0';
			pinters.get()->code = code;
			pinters.get()->name = name;
			m_grid->SetItemText(xrow + 1, colNAME, name);
			//AddInters(pinters);
		}

		if (code.GetLength() <= 0)
		{
			for (int ii = 0; ii < m_colC; ii++)
				m_grid->SetItemText(nmgv->row, ii, "");
			m_grid->setmemoCheck(nmgv->row);
			break;
		}

		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = TRUE;

		if (m_bChgCnd)
		{
			saveInterest();
		}

		sendTransaction(xrow);
	}

		ClearSearchMap(); // 2011.12.29 KSJ
		ReSetSearchMap(); // 2011.12.29 KSJ

		break;
	case GVNM_ENDEDIT:
	case GVNM_ENDRETURN:
	{
		m_grid->SetFocus();

		const int nRow = nmgv->row, nCol = nmgv->col;

		CString symbol, curr;
		symbol = m_grid->GetItemSymbol(nRow, nCol);
		curr = m_grid->GetItemText(nRow, colCURR);
		const auto& pInters = GetData(nRow - 1);
		CString value = m_grid->GetItemText(nRow, nCol);

		CString zeroValue = "0";

		// 2011.12.20 KSJ
		if (symbol.IsEmpty())
			break;
		// KSJ

		if (symbol.CompareNoCase("###1") == 0) // 보유수량
		{
			value = m_grid->GetItemText(nRow, nCol);
			pInters->xnum = value;

			if (m_bChgCnd)
			{
				saveInterest();
			}
		}
		else if (symbol.CompareNoCase("###2") == 0) // 보유단가
		{

			pInters->xprc = value;
			if (m_bChgCnd)
			{
				saveInterest();
			}
		}
		else
		// 2011.12.20 KSJ 중괄호 되어 있지 않아서 추가함.
		{
			Invalidate(false);
			break;
		}
		// KSJ

		if (m_sonikField > colCURR)
		{
			value = CalcuPyungaSonik(pInters.get(), curr);
			m_grid->SetItemText(nRow, m_sonikField, value);
		}

		if (m_suikField > colCURR)
		{
			value = CalcuSuik(pInters.get(), curr);
			m_grid->SetItemText(nRow, m_suikField, value);
		}

		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = TRUE;
	}

		ClearSearchMap(); // 2011.12.29 KSJ
		ReSetSearchMap(); // 2011.12.29 KSJ

		break;

	case GVNM_COLUMNMOVE:
	{
		if (nmgv->row >= m_gridHdrX.GetSize() || nmgv->col >= m_gridHdrX.GetSize())
			break;

		m_btReload.Invalidate(TRUE);
		m_btDomino.Invalidate(TRUE);
		m_btSAVE.Invalidate(TRUE);
		m_btCLOSE.Invalidate(TRUE);
		_gridHdr xgridHdr;

		xgridHdr = m_gridHdrX.GetAt(nmgv->col);
		m_gridHdrX.InsertAt(nmgv->row, xgridHdr);

		if (nmgv->col > nmgv->row)
			nmgv->col += 1;

		m_gridHdrX.RemoveAt(nmgv->col);
		ReIndexSpecial();

		const int ncnt = m_gridHdrX.GetSize();
		CString sztmp, szFIELD = _T("");
		int nWidth{};
		m_szWIDTH = _T("");
		m_szFIELD = _T("");
		for (int ii = 0; ii < ncnt; ii++)
		{
			xgridHdr = m_gridHdrX.GetAt(ii);
			nWidth = m_grid->GetColumnWidth(ii);
			if (ii >= colNAME)
			{
				sztmp.Format("%d,", nWidth);
				m_szWIDTH += sztmp;
			}
			if (ii > colCURR)
			{
				sztmp.Format("%s,", xgridHdr.symbol);
				if (sztmp.Compare("2111,") == 0)
					sztmp.Format("2023,");
				else if (sztmp.Compare("2115,") == 0)
					sztmp.Format("2024,");
				else if (sztmp.Compare("2116,") == 0)
					sztmp.Format("2033,");
				else if (sztmp.Compare("2112,") == 0)
					sztmp.Format("2027,");

				m_szFIELD += sztmp;
			}
		}

		SaveAsInterField();
	}

	break;

	case GVNM_BEGINDRAG:
	{
		m_drag = nmgv->row;
		m_drop = -1;
		m_strCode = m_grid->GetItemText(m_drag, colCODE);

		m_grid->m_bOutPos = false;

		if (CAST_TREEID(m_kind)->kind == xISSUE)
			CintGrid::m_dropdata.SetCode(m_grid->GetItemText(m_drag, colCODE));
	}

	break;
	case GVNM_OUTDRAG:
	{
		if (m_grid->m_bOutPos)
		{
			if (CAST_TREEID(m_kind)->kind == xINTEREST)
			{
				DeleteRow(m_drag);
				m_bEditWork = TRUE;
			}
			m_grid->m_bOutPos = false;
		}
	}

	break;
	case GVNM_ENDEND:
	{
		//드래그 카운트용 호출
		((CGroupWnd *)m_pGroupWnd)->AddDragInCount();

		const int xdrag = m_drag, xdrop = nmgv->row;
		m_drop = nmgv->row;
		int nIdx = 0;
		CString code, name;

		m_grid->m_bOutPos = false;

		code = CintGrid::m_dropdata.GetCode();
		name = CintGrid::m_dropdata.GetCode();

		CString strTemp;

		//종목 중복 등록 허용
		if (((CGroupWnd *)GetParent())->GetOverLapAction() == FALSE)
		{
			if (!code.IsEmpty() && IsDuplicateCode(code, m_drop, true))
			{
				CCodeDuplicate dlg;

				// dlg 항상 열기 옵션 체크
				if (dlg.DoModal())
				{
					if (dlg.m_bAlert == FALSE)
					{
						m_grid->Invalidate(FALSE);
						break;
					}
					else
					{
						//파일에 저장
						((CGroupWnd *)GetParent())->saveOverLap(TRUE);
					}
				}
				else
				{
					m_grid->Invalidate(FALSE);
					break;
				}
			}
		}

		if (m_drop < 0)
			m_drop = m_grid->GetRowCount();
		else if (m_drop == 0)
			m_drop = 1;

		if (m_drop - 2 < 0)
			nIdx = 0;
		else
			nIdx = m_drop - 1;

		const int nCount = _vInters.size();
		if (nCount > nIdx)
		{
			auto& pinters = _vInters.at(nIdx);
			if (strlen(pinters.get()->code) != 0)
			{
				CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
				const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
				int rowcount = 0;

				if (nOver == MO_VISIBLE)
				{
					pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
					rowcount = ((CGroupWnd*)pWnd)->sumEachGroupCount();
				}
				else
				{
					rowcount = GetRowValidcount();
				}

				if (rowcount >= MAX_LINE)
				{

					Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
					m_grid->Invalidate(FALSE);
					break;
				}
				else
				{
					DeleteRow(MAX_LINE);
				}
				// END DEL

				insertInterest(m_drop - 1);
				insertRow(m_drop);
				m_drop -= 1;
				pinters = _vInters.at(m_drop);
			}

			//struct _intersx InterData;
			//memset(&InterData, 0x20, sizeof(struct _intersx));

			pinters.get()->gubn = '0';
			pinters.get()->code = code;
			pinters.get()->name = name;

			//memcpy(pinters, &InterData, sizeof(struct _intersx));

			if (CAST_TREEID(m_kind)->kind == xINTEREST)
				m_bEditWork = TRUE;

			m_drag = m_drop = -1;
			m_seldrop = xdrop;
			sendTransaction();
			m_pGroupWnd->SendMessage(WM_MANAGE, MK_ENDDRAG);

			CString strINI;
			strINI.Format("%s\\User\\%s\\userconf.ini", m_root, m_user);
			const int dragcnt = GetPrivateProfileInt("IB202700", "DRAGCNT", 0, strINI);

			CString cnt;
			cnt.Format("%d", ((CGroupWnd*)m_pGroupWnd)->GetDragInCount() + dragcnt);
			WritePrivateProfileString("IB202700", "DRAGCNT", cnt, strINI);
		}
	}

	break;
	case GVNM_ENDDRAG:
	{
		if (CAST_TREEID(m_kind)->kind == xISSUE)
		{
			m_grid->Invalidate(FALSE);
			break;
		}

		m_grid->m_bOutPos = false;

		CDropData *pDropData = &CintGrid::m_dropdata; // drop information
		CintGrid *pGrid = pDropData->GetGrid();	      // drag grid
		if (pGrid == nullptr)
			break;

		const int nDGrow = pDropData->GetRow(); // drag row

		const int ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);

		if (ret != 0) //등록순이 아닐경우 드래그 허용 안함
		{
			m_grid->SetSelectRow(nDGrow);
			return 0;
		}

		CGridWnd *pDropGrid = (CGridWnd *)pDropData->GetGrid()->GetParent(); // drag gridwnd
		CGroupWnd *pDropGroup = (CGroupWnd *)pDropGrid->GetParent();	     // drag groupwnd
		const int xdrag = m_drag, xdrop = nmgv->row;
		int nIdx = 0; // BUFFET
		CString xCODE;
		CString code, name;
		BOOL bNewsDrop = FALSE;
		const UINT kind = (UINT)pDropGrid->SendMessage(WM_MANAGE, MK_GETDATAKIND);
		if (CAST_TREEID(kind)->kind == xISSUE)
			bNewsDrop = TRUE;
		if (bNewsDrop)
			xCODE = CintGrid::m_dropdata.GetCode();
		m_drop = nmgv->row;


		const int count = _vInters.size();

		if (count >= m_drag && count >= m_drop)
		{
			if (pDropData->GetGrid()->m_hWnd == m_grid->m_hWnd)		// 동일한 화면에서 drop
			{
				if (m_drop < 0)
					m_drop = m_grid->GetRowCount() - 1;
				else if (m_drop == 0)
					m_drop = 1;

				if (m_drag == m_drop)
					break;

				auto pDrag = _vInters.at(m_drag - 1);
				_vInters.erase(_vInters.begin() + m_drag - 1);
				_vInters.insert(_vInters.begin() + m_drop - 1, pDrag);
			}
			else if (pDropData->GetGrid()->m_hWnd != m_grid->m_hWnd) //(m_drag < 0 && m_drop > 0)	// 다른 화면에서 drop
			{
				code = (bNewsDrop) ? xCODE : pGrid->GetItemText(nDGrow, colCODE);
				name = pGrid->GetItemText(nDGrow, colNAME);

				if (m_drop < 0)
					m_drop = m_grid->GetRowCount();
				else if (m_drop == 0)
					m_drop = 1;


				if (pDropData->GetGrid()->m_hWnd != m_grid->m_hWnd)
				{
					//종목 중복 등록 허용
					if (((CGroupWnd*)GetParent())->GetOverLapAction() == FALSE)
					{
						if (!code.IsEmpty() && IsDuplicateCode(code, m_drop, true))
						{
							CCodeDuplicate dlg;
							// dlg 항상 열기 옵션 체크
							if (dlg.DoModal())
							{
								if (dlg.m_bAlert == FALSE)
								{
									m_grid->Invalidate(FALSE);
									break;
								}
								else
								{
									//파일에 저장
									((CGroupWnd*)GetParent())->saveOverLap(TRUE);
								}
							}
							else
							{
								m_grid->Invalidate(FALSE);
								break;
							}
						}
					}
				}

				if (m_drop - 2 < 0)
					nIdx = 0;
				else
					nIdx = m_drop - 2;

				const auto& pinters = _vInters.at(nIdx);
				if (!pinters.get()->code.IsEmpty())
				{
					CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
					const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
					int rowcount = 0;

					if (nOver == MO_VISIBLE)
					{
						pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
						rowcount = ((CGroupWnd*)pWnd)->sumEachGroupCount();
					}
					else
					{
						rowcount = GetRowValidcount();
					}

					if (rowcount >= MAX_LINE)
					{
						Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
						m_grid->Invalidate(FALSE);
						break;
					}
					else
					{
						DeleteRow(MAX_LINE);
					}
					// END DEL

					insertInterest(m_drop - 1);
					insertRow(m_drop);
					m_drop -= 1;
				}
				//그리드에 한줄도 없을 경우 임의로 한줄 추가 생성
				else
				{
					if (nIdx == 0 && m_rowC < 2)
					{
						insertRow(m_drop);
					}
				}

				auto& pDrop = _vInters.at(m_drop);
				// 2013.01.22 KSJ 정렬후에는 그리드의 데이터를 가져올 때 반드시 현재 m_inters의 현재의치를 찾아야 한다.
				const auto& pInterDrag = pDropGrid->GetData(pDropGrid->GetInter(code));
				pDrop = pInterDrag;

				// ADD PSH 20070918
				UINT uAttr{};
				if ('m' == pinters.get()->gubn || pinters.get()->gubn == ROW_COMMENT)  //test mod 'm'
				{
					uAttr = m_grid->GetItemAttr(m_drop + 1, colNAME) | GVAT_MARKER;
					m_grid->SetItemAttr(m_drop + 1, colNAME, uAttr);
					m_grid->SetItemText(m_drop + 1, colNAME, pinters.get()->name);
				}
				else
				{
					uAttr = m_grid->GetItemAttr(m_drop + 1, colNAME) & ~GVAT_MARKER;
					m_grid->SetItemAttr(m_drop + 1, colNAME, uAttr);
					m_grid->SetItemText(m_drop + 1, colNAME, pinters->name);
				}
				// END ADD

				const LPARAM data = pGrid->GetItemData(nDGrow, colSIG);
				m_grid->SetItemData(xdrop, colSIG, data);

				// index가 하나 차이가 나므로
				if (pDropGroup->m_hWnd == m_pGroupWnd->m_hWnd)
				{
					if (!bNewsDrop)
						pDropGrid->SendMessage(WM_MANAGE, MAKEWPARAM(MK_DELETEROW, nDGrow));
				}
			}

			if (!m_bSorting)
			{
				if (CAST_TREEID(m_kind)->kind == xINTEREST)
				{
					m_bEditWork = TRUE;
				}
			}
			else
			{
				if (m_bMoveCfg && (1 == m_nMoveSave))
				{
					RemoveAllBaseInters();
					m_grid->SortBase();
					saveInterest();
				}
				else if (!m_bMoveCfg)
				{
					m_bMoveCfg = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVECFG);
					m_nMoveSave = m_pToolWnd->SendMessage(WM_MANAGE, MK_GETMOVESAVE);

					// 091013 수정
					CMoveSetupDlg dlg;
					dlg.m_nOption = m_nMoveSave;
					dlg.m_bAlert = m_bMoveCfg;

					// dlg 항상 열기 옵션 체크
					if (dlg.m_bAlert)
					{
						if (1 == dlg.m_nOption)
						{
							RemoveAllBaseInters();
							m_grid->SortBase();
							saveInterest();
							m_bEditWork = FALSE;
						}
						else
						{
							if (CAST_TREEID(m_kind)->kind == xINTEREST)
								m_bEditWork = TRUE;
						}
					}
					else
					{
						if (dlg.DoModal())
						{
							if (1 == dlg.m_nOption)
							{
								RemoveAllBaseInters();
								m_grid->SortBase();
								saveInterest();
								m_bEditWork = FALSE;
							}
							else
							{
								if (CAST_TREEID(m_kind)->kind == xINTEREST)
									m_bEditWork = TRUE;
							}
						}
					}

					m_pToolWnd->SendMessage(WM_MANAGE, MK_SETMOVECFG, dlg.m_bAlert);
					m_pToolWnd->SendMessage(WM_MANAGE, MK_SETMOVESAVE, dlg.m_nOption);
				}
			}

			m_drag = m_drop = -1;
			m_seldrop = xdrop;
			sendTransaction();
			m_pGroupWnd->SendMessage(WM_MANAGE, MK_ENDDRAG);
		}
	}
	break;
	case GVNM_SELCHANGED:
	{
		CString code, string;
		if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col >= colNAME)
		{
			code = m_grid->GetItemText(nmgv->row, colCODE);
			code.TrimLeft();
			code.TrimRight();

			if (code.IsEmpty())
				break;

			if (((CGroupWnd *)m_pGroupWnd)->GetSelAction() == 0)
				break;

			switch (GetCodeType(code))
			{
			case kospiCODE:
				string.Format("%s\t%s", HCOD, code);
				break;
			case futureCODE:
				string.Format("%s\t%s", "ed_focod", code); // 2013.01.03 KSJ 2200선물종합화면에도 트리거보냄
				Variant(triggerCC, string);

				string.Format("%s\t%s", FCOD, code);
				break;
			case optionCODE:
				string.Format("%s\t%s", "ed_focod", code); // 2013.01.03 KSJ 2200선물종합화면에도 트리거보냄
				Variant(triggerCC, string);

				string.Format("%s\t%s", OCOD, code);
				break;
			case foptionCODE:
				string.Format("%s\t%s", "ed_focod", code); // 2013.01.03 KSJ 2200선물종합화면에도 트리거보냄
				Variant(triggerCC, string);

				string.Format("%s\t%s", PCOD, code);
				break;
			case indexCODE:
				string.Format("%s\t%s", UCOD, code);
				break;
			case sinjuCODE:
				string.Format("%s\t%s", SINJUCOD, code);
				break;
			case elwCODE:
				string.Format("%s\t%s", HCOD, code);
				Variant(triggerCC, string);
				string.Format("%s\t%s", ELWCOD, code);
				m_grid->SetFocus();
				break;
			case thirdCODE:
				string.Format("%s\t%s", THIRDCOD, code);
				break;
			case 0:							  // 2012.10.11 KSJ 주식, 금리, 통화선물 추가
				if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물  //파생상품 코드개편
				{
					int nGubn = atoi(code.Mid(1, 2));
					CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ 주식선물 신규종목 연동

					if (nGubn > 10 && nGubn < 60) //주식선물
					{
						string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
						Variant(triggerCC, string);

						string.Format("%s\t%s", SFOD, code);
						break;
					}
					else if (nGubn > 60 && nGubn <= 70) //금리선물
					{
						string.Format("%s\t%s", SFOD, code);
						break;
					}
					else if (nGubn > 70 && nGubn < 80) //통화선물
					{
						string.Format("%s\t%s", SFOD, code);
						break;
					}
					else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
						|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
						|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z")) // 2024.1031 주식선물
					{
						string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
						Variant(triggerCC, string);
						string.Format("%s\t%s", SFOD, code);
						break;
					}
				}
			}

			m_grid->SetFocus();

			break;
		}
		break;
		// Map 초기화 KSI
		ClearSearchMap();
		ReSetSearchMap(); // 2011.12.29 KSJ
	}

	case GVNM_TRIGGER:
	{		
		CString code = m_grid->GetItemText(nmgv->row, colCODE);
		CString string;
		// if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col >= colNAME)
		{
			// code = m_grid->GetItemText(nmgv->row, colCODE);
			code.TrimLeft();
			code.TrimRight();

			if (code.IsEmpty())
				break;

			if (((CGroupWnd *)m_pGroupWnd)->GetSelAction() == 0)
				break;

			switch (GetCodeType(code))
			{
			case kospiCODE:
				string.Format("%s\t%s", HCOD, code);
				break;
			case futureCODE:
				string.Format("%s\t%s", FCOD, code);
				break;
			case optionCODE:
				string.Format("%s\t%s", OCOD, code);
				break;
			case foptionCODE:
				string.Format("%s\t%s", PCOD, code);
				break;
			case indexCODE:
				string.Format("%s\t%s", UCOD, code);
				break;
			case sinjuCODE:
				string.Format("%s\t%s", SINJUCOD, code);
				break;
			case elwCODE:
				string.Format("%s\t%s", HCOD, code);
				Variant(triggerCC, string);
				//					m_grid->SetFocus();
				string.Format("%s\t%s", ELWCOD, code);
				break;
			case thirdCODE:
				string.Format("%s\t%s", THIRDCOD, code);
				break;
			case 0:							  // 2012.10.11 KSJ 주식, 금리, 통화선물 추가
				if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물  //파생상품 코드개편
				{
					int nGubn = atoi(code.Mid(1, 2));
					CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ 주식선물 신규종목 연동

					if (nGubn > 10 && nGubn < 60) //주식선물
					{
						string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
						Variant(triggerCC, string);

						string.Format("%s\t%s", SFOD, code);
						break;
					}
					else if (nGubn > 60 && nGubn <= 70) //금리선물
					{
						string.Format("%s\t%s", SFOD, code);
						break;
					}
					else if (nGubn > 70 && nGubn < 80) //통화선물
					{
						string.Format("%s\t%s", SFOD, code);
						break;
					}
					else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
						|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
						|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z")) // 2024.1031 주식선물
					{
						string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
						Variant(triggerCC, string);
						string.Format("%s\t%s", SFOD, code);
						break;
					}
				}
			}
			Variant(triggerCC, string);
			Variant(codeCC, string);

			if (nmgv != nullptr)
			{
				if (nmgv->row != m_ClickRow)
				{
					m_grid->SetFocus();
					m_ClickRow = nmgv->row;
				}
			}
			break;
		}
	}
	break;
	case GVNM_LMOUSEDOWN:
	{
		CString code, string;
		CString strINI;
		strINI.Format("%s\\User\\%s\\userconf.ini", m_root, m_user);

		if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col == colSIG)
		{
			// 2015.01.15 KSJ 빈값이 들어가므로 trim 해준다.
			CString sRecommand;
			sRecommand = GetRecommandInfo(nmgv->row);
			sRecommand.TrimRight();
			sRecommand.TrimLeft();

			if (sRecommand != "")
			{
				code = m_grid->GetItemText(nmgv->row, colCODE);
				code.TrimLeft();
				code.TrimRight();
				if (code.IsEmpty())
					break;

				WritePrivateProfileString("IB202600", "POPUP_FLAG", "1", strINI);
				WritePrivateProfileString("IB202600", "POPUP_CODE", code, strINI);
				string.Format("IB202600 /S/t0/d%s\t%s", HCOD, code);
				openView(typeVIEW, string);
				CString codepopup;
				codepopup.Format("%s\t%s", HCOD, code);
			}
		}
	}
	break;
	case GVNM_DBLCLICKED:
	{
		CString code, string;
		if (nmgv->row >= m_grid->GetFixedRowCount() && nmgv->col >= colNAME)
		{
			code = m_grid->GetItemText(nmgv->row, colCODE);
			code.TrimLeft();
			code.TrimRight();
			if (code.IsEmpty())
				break;

			switch (((CGroupWnd *)m_pGroupWnd)->GetDblAction())
			{
			case dblCURR:
				switch (GetCodeType(code))
				{
				case kospiCODE:
				case thirdCODE: // 2013.08.07 프리보드일때도 2001번을 띄워준다.
					string.Format("%s /S/t0/d%s\t%s", kcurrMAP, HCOD, code);
					openView(typeVIEW, string);
					break;
				case futureCODE:
					string.Format("%s /S/t0/d%s\t%s", fcurrMAP, FCOD, code);
					openView(typeVIEW, string);
					break;
				case optionCODE:
					string.Format("%s /S/t0/d%s\t%s", ocurrMAP, OCOD, code);
					openView(typeVIEW, string);
					break;
				case foptionCODE:
					string.Format("%s /S/t0/d%s\t%s", focurrMAP, PCOD, code);
					openView(typeVIEW, string);
					break;
				case indexCODE:
					string.Format("%s /S/t0/d%s\t%s", icurrMAP, UCOD, code);
					openView(typeVIEW, string);
					break;
				case elwCODE:
					string.Format("%s /S/t0/d%s\t%s", ecurrMAP, ELWCOD, code);
					openView(typeVIEW, string);
					break;
				case 0:							  // 2012.10.11 KSJ 주식, 금리, 통화선물 추가
					if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물//파생상품 코드개편
					{
						int nGubn = atoi(code.Mid(1, 2));
						CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ 주식선물 신규종목 연동

						if (nGubn > 10 && nGubn < 60) //주식선물
						{
							string.Format("%s /S/t0/d%s\t%s", sfcurrMAP, EDFCOD, code);
							openView(typeVIEW, string);
							break;
						}
						else if (nGubn > 60 && nGubn <= 70) //금리선물
						{
							string.Format("%s /S/t0/d%s\t%s", cccurrMAP, SFOD, code);
							openView(typeVIEW, string);
							break;
						}
						else if (nGubn > 70 && nGubn < 80) //통화선물
						{
							string.Format("%s /S/t0/d%s\t%s", cccurrMAP, SFOD, code);
							openView(typeVIEW, string);
							break;
						}
						else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
							|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
							|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z")) // 2024.10.31 주식선물
						{
							string.Format("%s /S/t0/d%s\t%s", sfcurrMAP, EDFCOD, code);
							openView(typeVIEW, string);
							break;
						}
					}
				}
				break;
			case dblCHART:
				string.Format("%s /S/t0/d%s\t%s", chartMAP, CCOD, code);
				openView(typeVIEW, string);
				break;
			case dblUSER:
			{
				CString mapname = ((CGroupWnd *)m_pGroupWnd)->GetLinkmap();
				if (mapname.IsEmpty())
					break;

				switch (GetCodeType(code))
				{
				case kospiCODE:
					string.Format("%s /S/t0/d%s\t%s", mapname, HCOD, code);
					openView(typeVIEW, string);
					break;
				case futureCODE:
					string.Format("%s /S/t0/d%s\t%s", mapname, FCOD, code);
					openView(typeVIEW, string);
					break;
				case optionCODE:
					string.Format("%s /S/t0/d%s\t%s", mapname, OCOD, code);
					openView(typeVIEW, string);
					break;
				case foptionCODE:
					string.Format("%s /S/t0/d%s\t%s", mapname, PCOD, code);
					openView(typeVIEW, string);
					break;
				case indexCODE:
					string.Format("%s /S/t0/d%s\t%s", mapname, UCOD, code);
					openView(typeVIEW, string);
					break;
				case 0:							  // 2012.10.11 KSJ 주식, 금리, 통화선물 추가
					if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물//파생상품 코드개편
					{
						int nGubn = atoi(code.Mid(1, 2));
						const CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ 주식선물 신규종목 연동

						if (nGubn > 10 && nGubn < 60) //주식선물
						{
							string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
							openView(typeVIEW, string);
							break;
						}
						else if (nGubn > 60 && nGubn <= 70) //금리선물
						{
							string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
							openView(typeVIEW, string);
							break;
						}
						else if (nGubn > 70 && nGubn < 80) //통화선물
						{
							string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
							openView(typeVIEW, string);
							break;
						}
						else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
							|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
							|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z")) // 2014.10.21 KSJ 주식선물 신규종목 연동
						{
							string.Format("%s /S/t0/d%s\t%s", mapname, SFOD, code);
							openView(typeVIEW, string);
							break;
						}
					}
				}
				break;
			}
			}
		}
	}
	break;
	case GVNM_ENDSORT:
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
		{
			// 				m_bEditWork = true;		//2012.02.13 KSJ 변경한것 없어도 소트하고나면 저장하라고해서 주석처리함.
		}

		m_endsort = true;
		m_grid->getmemoCheck();
		rebuildInterest();

		ClearSearchMap();
		ReSetSearchMap(); // 2011.12.29 KSJ

		COLORREF bkColor{};
		for (int ii = 0; ii < m_grid->GetRowCount(); ii++)
		{
			CString strTemp = m_grid->GetItemText(ii, colNAME);
			if (!ii)
				continue;

			bkColor = ((CGroupWnd *)m_pGroupWnd)->GetBkColor1();
			if (ii > 1 && ((ii - 1) / ((CGroupWnd *)m_pGroupWnd)->GetLine()) % 2)
				bkColor = ((CGroupWnd *)m_pGroupWnd)->GetBkColor2();
			for (int jj = 0; jj < m_colC; jj++)
				m_grid->SetItemBkColor(ii, jj, bkColor);
		}

		const LONG ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
		hawkeyeAction((ret) ? true : false);
		// DEL PSH 20070913
		// saveInterest();
		// END DEL

		// ADD PSH 20070913
		m_bSorting = TRUE;
		// END ADD

		// Map 초기화 KSI
		ClearSearchMap();
		ReSetSearchMap(); // 2011.12.29 KSJ
	}
	break;

	case GVNM_RMOUSEDOWN:
		// MODIFY PSH 20070912
		{
			if (0 < nmgv->row)
			{
				RbuttonAction(nmgv->row);
			}
			else
			{
				ShowPopupMenu();
			}
		}
		// END MODIFY
		break;
	// ADD PSH 20070914
	case GVNM_CHANGEMARKER:
	{
		m_grid->SetFocus();
		CString strMarker, strMarkerCode;
		strMarker = m_grid->GetItemText(nmgv->row, colNAME);

		const int nCount = _vInters.size();
		const int nMarkerRow = nmgv->row - 1;
		strMarkerCode.Format("m%05d", nMarkerRow);

		if (!m_bSorting && nCount > nMarkerRow)
		{
			auto& pInter = _vInters.at(nMarkerRow);
			pInter.get()->code = strMarkerCode;
			pInter.get()->name = strMarker;
			saveInterest();
		}
	}
	break;
		// END ADD
	}

	return CBaseWnd::OnNotify(wParam, lParam, pResult);
}
bool CGridWnd::IsDuplicateCode(CString code, int nrow, bool drag)
{
	CString string;
	const int nMax = m_grid->GetRowCount();

	for (int ii = 1; ii < nMax /*m_rowC*/; ii++)
	{
		if (!drag)
		{
			if (ii == nrow)
				continue;
		}

		string = m_grid->GetItemText(ii, colCODE);
		if (code == string)
			return true;
	}

	return false;
}

int CGridWnd::GetCodeType(CString code)
{
	const int codeL = code.GetLength();
	if (codeL <= 0)
		return 0;

	const int result = m_pView->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)code);

	switch (result)
	{
	case kospiType:	 // kospi code
	case kosdaqType: // kosdaq code
	case etfType:	 // etf code
	case etnType:	 // 2014.10.28 KSJ ETN 추가
		return kospiCODE;
		;
	case elwType: // ELW
		return elwCODE;
	case futureType:
		return futureCODE; // 선물
	case sfutureType:
		return sfCODE;
	case callType: // call option
	case putType:  // put  option
		return optionCODE;
	case indexType: // 업종
		return indexCODE;
	case thirdType: // 제3시장
		return thirdCODE;
	case sinjuType: // 신주인수권
		return sinjuCODE;
	case mufundType: // 뮤츄얼펀드
	case reitsType:	 // 리츠
	case hyfundType: // 하이일드펀드
		return 200;
	}

	return 0;
}

int CGridWnd::openView(int type, CString data)
{
	return m_pView->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)(LPCTSTR)data);
}

void CGridWnd::hawkeyeAction(bool toggle)
{
	CString code;

	if (toggle) // hawkeye color
	{
		for (int ii = 1; ii < m_rowC; ii++)
		{
			code = m_grid->GetItemText(ii, colCODE);
			code.TrimLeft();
			code.TrimRight();
			if (!code.IsEmpty())
				hawkEyeColor(ii);
			else
				SetLineColor(ii);
		}
	}
	else
		SetLineColor();

	m_grid->m_nAlmRow = -1; // BUFFET
	m_grid->memoRefresh();
}

void CGridWnd::RbuttonAction(int row)
{
	if (row == 0)
		return;

	const int userBASE = 100;
	const int subBASE =  200;
	const int menuBASE = 300;
	

	m_grid->SetSelectRow(row);

	CPoint point;
	GetCursorPos(&point);

	int index = -1, offs{};
	char *ptr{}, wb[1024]{};
	;
	CStringArray items;
	items.RemoveAll();
	CString string, path, domino, mapN = _T("IB202700");

	CString code;
	int itype{};  //test menupop

	if (m_bPopupDlg)
	{
		CPopupDlg dlg;
		dlg.m_nX = point.x;
		dlg.m_nY = point.y;

		path.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, "IB20");

		DWORD rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		if (rc <= 0)
		{
			path.Format("%s/%s/%s", m_root, MTBLDIR, "IB20");
			rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		}

		ptr = wb;
		for (; rc > 0; ptr++, rc--)
		{
			string = ptr;
			ptr += string.GetLength();
			rc -= string.GetLength();
			if (string.GetLength() <= L_MAPN)
			{
				if (string.Find('-') == 0)
					continue;
			}
			mapN = string.Left(L_MAPN);
			string = string.Mid(L_MAPN);
			offs = string.Find('=');
			if (offs != -1)
				string = string.Mid(offs + 1);

			index = items.Add(mapN);
			dlg.AddMenu(menuBASE + index, string);
		}

		code = m_grid->GetItemText(row, colCODE);
		code.TrimRight();

		Variant(triggerCC, code);
		m_grid->SetFocus();

		const int dataidx = CAST_TREEID(m_kind)->kind;

		if (dataidx != xISSUE)
		{
			dlg.AddMenu(userBASE + 0, "종목 추가");
			dlg.AddMenu(userBASE + 1, "종목 삭제");
			dlg.AddMenu(userBASE + 2, "종목 변경");

			if (!code.IsEmpty())
			{
				dlg.AddMenu(userBASE + 3, "메모 입력");
				dlg.AddMenu(userBASE + 11, "메모 삭제");
				dlg.AddMenu(userBASE + 12, "북마크 지정");
				dlg.AddMenu(userBASE + 13, "북마크 삭제");
			}

			dlg.AddMenu(userBASE + 9, "책갈피 추가");
			dlg.AddMenu(userBASE + 10, "책갈피 삭제");
		}

		//		dlg.AddMenu(userBASE+14, "복수현재가2 종목연동");

		if (dataidx == xINTEREST)
			dlg.AddMenu(userBASE + 6, "관심그룹저장");
		else
			dlg.AddMenu(userBASE + 7, "관심그룹추가");

		dlg.AddMenu(userBASE + 8, "그룹초기화");

		dlg.AddMenu(userBASE + 4, "연결화면 편집...");

		dlg.AddMenu(userBASE + 5, "엑셀로 보내기");

		if (-1 == dlg.DoModal())
			return;

		index = dlg.m_nResult;
	}
	else
	{
		code = m_grid->GetItemText(row, colCODE);  //test menupop
		code.TrimRight();

		itype = GetCodeType(code);

		auto popM = std::make_unique<CMenu>();
		popM->CreatePopupMenu();

		path.Format("%s/%s/%s", m_root, TABDIR, "intermenu.txt");
		switch (itype)
		{
		case thirdCODE:  //110  K-OTC
		case kospiCODE: //100  코스피 
			mapN = "IB202200_100";
			break;
		case futureCODE:  //101 지수선물
			mapN = "IB202200_101";
			break;
		case optionCODE: //102 지수옵션
			mapN = "IB202200_102";
			break;
		case indexCODE:   //104 업종지수
			mapN = "IB202200_104";
			break;
		case elwCODE:   //120  ELW
			mapN = "IB202200_120";
			break;
		case 0:
			if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물//파생상품 코드개편
			{
				int nGubn = atoi(code.Mid(1, 2));
				CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ 주식선물 신규종목 연동

				if (nGubn > 60 && nGubn <= 70) //금리선물
				{
					mapN = "IB202200_CF";
					break;
				}
				else if (nGubn > 70 && nGubn < 80) //통화선물
				{
					mapN = "IB202200_CF";
					break;
				}
				else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
					|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
					|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z")) // 2024.1031 주식선물
				{
					mapN = "IB202200_SF";
					break;
				}
			}
		}

		DWORD rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		if (rc <= 0)
		{
			mapN = "IB202700";
			path.Format("%s/%s/%s", m_root, MTBLDIR, "IB20");
			rc = GetPrivateProfileSection(mapN, wb, sizeof(wb), path);
		}

		ptr = wb;
		for (; rc > 0; ptr++, rc--)
		{
			string = ptr;
			ptr += string.GetLength();
			rc -= string.GetLength();
			if (string.GetLength() <= L_MAPN)
			{
				if (string.Find('-') == 0)
					popM->AppendMenu(MF_SEPARATOR, 0);
				continue;
			}
			mapN = string.Left(L_MAPN);
			string = string.Mid(L_MAPN);
			offs = string.Find('=');
			if (offs != -1)
				string = string.Mid(offs + 1);

			index = items.Add(mapN);
			popM->AppendMenu(MF_STRING | MF_ENABLED, menuBASE + index, string);
		}

		if (index >= 0)
		{
			index++;
			popM->AppendMenu(MF_SEPARATOR, 0);
		}

		auto subMenu = std::make_unique<CAxMenu>();
		subMenu->CreatePopupMenu();
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 1, "1");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 2, "2");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 3, "3");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 4, "4");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 5, "5");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 6, "6");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 7, "7");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 8, "8");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 9, "9");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 10, "10");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 11, "11");
		subMenu->AppendMenu(MF_STRING|MF_OWNERDRAW, subBASE + 12, "12");
		const int dataidx = CAST_TREEID(m_kind)->kind;

		// pyt
		CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		const int ret = pWnd->SendMessage(WM_MANAGE, MK_GETARRANGE);
		if (dataidx != xISSUE && ret == 0)
		//		if (dataidx != xISSUE)
		{
			popM->AppendMenu(MF_STRING, userBASE + 0, "종목 추가<INSERT>");
			popM->AppendMenu(MF_STRING, userBASE + 1, "종목 삭제<DELETE>");
			popM->AppendMenu(MF_STRING, userBASE + 2, "종목 변경");

			if (!code.IsEmpty())
			{
				popM->AppendMenu(MF_STRING, userBASE + 3, "메모 입력");
				popM->AppendMenu(MF_STRING, userBASE + 11, "메모 삭제");
				
				popM->InsertMenu(userBASE + 12, MF_POPUP | MF_BYPOSITION, (UINT_PTR)subMenu->GetSafeHmenu(), _T("북마크 지정"));
				popM->AppendMenu(MF_STRING, userBASE + 13, "북마크 삭제");
			}
			// MODIFY PSH 20070914
			popM->AppendMenu(MF_STRING, userBASE + 15, "빈줄추가");
			popM->AppendMenu(MF_STRING, userBASE + 9, "책갈피 추가");
			popM->AppendMenu(MF_STRING, userBASE + 10, "책갈피 삭제");
			// END MODIFY
		}

		// 2016.08.12 KSJ 이슈종목이어도 복수현재가2 가 보여야한다.
		popM->AppendMenu(MF_STRING, userBASE + 14, "복수현재가2 종목연동");
		popM->AppendMenu(MF_SEPARATOR, 0);

		if (dataidx == xINTEREST)
			popM->AppendMenu(MF_STRING, userBASE + 6, "관심그룹저장");
		else
			popM->AppendMenu(MF_STRING, userBASE + 7, "관심그룹추가");
		popM->AppendMenu(MF_STRING, userBASE + 8, "그룹초기화");
		popM->AppendMenu(MF_SEPARATOR, 0);
		popM->AppendMenu(MF_STRING, userBASE + 4, "연결화면 편집...");
		popM->AppendMenu(MF_SEPARATOR, 0);
		popM->AppendMenu(MF_STRING, userBASE + 5, "엑셀로 보내기");

		index = popM->TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, point.x, point.y, this);
		popM->DestroyMenu();
		subMenu->DestroyMenu();
	}

	domino = _T("");
	if (!code.IsEmpty())
	{
		switch (itype)
		{
		case thirdCODE:
		case kospiCODE:
			domino.Format("%s\t%s", HCOD, code);
			break;
		case futureCODE:
			domino.Format("%s\t%s", FCOD, code);
			break;
		case optionCODE:
			domino.Format("%s\t%s", OCOD, code);
			break;
		case foptionCODE:
			domino.Format("%s\t%s", PCOD, code);
			break;
		case indexCODE:
			domino.Format("%s\t%s", UCOD, code);
			break;
		case elwCODE:
			domino.Format("%s\t%s", HCOD, code);
			break;
		case 0:							  // 2012.10.11 KSJ 주식, 금리, 통화선물 추가
			if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물//파생상품 코드개편
			{
				int nGubn = atoi(code.Mid(1, 2));
				CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ 주식선물 신규종목 연동

				if (nGubn > 10 && nGubn < 60) //주식선물
				{
					domino.Format("%s\t%s", SFOD, code);
					break;
				}
				else if (nGubn > 60 && nGubn <= 70) //금리선물
				{
					domino.Format("%s\t%s", SFOD, code);
					break;
				}
				else if (nGubn > 70 && nGubn < 80) //통화선물
				{
					domino.Format("%s\t%s", SFOD, code);
					break;
				}
				else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
					|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
					|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z")) // 2024.1031 주식선물
				{
					domino.Format("%s\t%s", SFOD, code);
					break;
				}
			}
		}
	}

	if (index >= menuBASE)
	{
		index -= menuBASE;
		if (index < items.GetSize())
		{
			string = items.GetAt(index);
			string += "/s/t0/d";
			string += domino;
			openView(typeVIEW, string);
		}
		items.RemoveAll();
		return;
	}
	items.RemoveAll();

	switch (index)
	{
	case userBASE + 0: // 추가
	{
		int gno = -1;
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			gno = CAST_TREEID(m_kind)->value;

		if (gno == -1)
		{
			MessageBox("그룹이 선택되지 않아 관심종목 등록이 불가합니다.\n원하는 그룹을 선택하거나 그룹이 없을 경우 설정화면에서 새그룹을 등록하시기 바랍니다.", "관심종목", MB_OK);
			return;
		}

		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = TRUE;

		CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
		int rowcount = 0;

		if (nOver == MO_VISIBLE)
		{
			pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
			rowcount = ((CGroupWnd *)pWnd)->sumEachGroupCount();
		}
		else
		{
			rowcount = GetRowValidcount();
		}

		if (rowcount >= MAX_LINE)
		{

			Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
			return;
		}

		insertInterest(row - 1);
		insertRow(row);
		SetLineColor();
		m_grid->getmemoCheck();
		//			m_grid->memoCheck(row+1);
		m_grid->memoRefresh();
		m_grid->SetFocusCellEdit(row, colNAME, true);

		if (m_bAddCnd)
		{
			saveInterest();
		}
	}
	break;
	case userBASE + 1: // 삭제
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = TRUE;

		if (gsl::narrow_cast<int>(_vInters.size()) >= row - 1)
		{
			if (!(m_grid->GetItemAttr(row, colNAME) & GVAT_MARKER))
			{
				DeleteRow(row);
				insertInterest(MAX_LINE - 1);
				// DEL PSH 20070914
				// insertRow(m_grid->GetRowCount());
				// END DEL
				//					saveInterest();
			}
			// END MODIFY
		}
	}
	break;
	case userBASE + 2:
		m_grid->SetFocusCellEdit(row, colNAME, true);
		break;
	case userBASE + 3:
		m_grid->memoWindowEdit(row);
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_MEMO);
		break;
	case userBASE + 4:
		mapN = _T("IB202200");
		m_pView->SendMessage(WM_USER, MAKEWPARAM(linkDLL, 0), (LPARAM)(LPCTSTR)mapN);
		break;
	case userBASE + 5:
		if (m_grid)
			m_grid->RunExcel();
		break;
	case userBASE + 6: // 관심그룹저장
		saveInterestX();
		break;
	case userBASE + 7: // 관심그룹추가
			   //		saveInterestX();
		break;
	case userBASE + 8: // 그룹화면초기화
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			Reset(false);
		else
			Reset(true);
		break;
	case userBASE + 15: //빈줄 추가
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = TRUE;

		CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
		int rowcount = 0;

		if (nOver == MO_VISIBLE)
		{
			pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
			rowcount = ((CGroupWnd *)pWnd)->sumEachGroupCount();
		}
		else
		{
			rowcount = GetRowValidcount();
		}

		if (rowcount >= MAX_LINE)
		{

			Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
			return;
		}

		insertInterest(row - 1);
		insertRow(row);
		SetLineColor();
		m_grid->getmemoCheck();
		//			m_grid->memoCheck(row+1);
		m_grid->memoRefresh();

#ifdef DF_SORT
		auto& pInter = _vInters.at(row - 1);

		if (pInter)
			pInter.get()->gubn = ROW_COMMENT;

#endif

		if (m_bAddCnd)
		{
			saveInterest();
		}
	}
	break;
	case userBASE + 9: // 책갈피 추가
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = TRUE;

		CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
		int rowcount = 0;

		if (nOver == MO_VISIBLE)
		{
			pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
			rowcount = ((CGroupWnd *)pWnd)->sumEachGroupCount();
		}
		else
		{
			rowcount = GetRowValidcount();
		}

		if (rowcount >= MAX_LINE)
		{

			Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
			break;
		}

		insertInterest(row - 1);
		insertRow(row);
		SetLineColor();
		m_grid->getmemoCheck();
		m_grid->memoRefresh();

		const UINT attr = m_grid->GetItemAttr(row, colNAME) | GVAT_MARKER;
		m_grid->SetItemAttr(row, colNAME, attr);


		const int nCount = _vInters.size();

		if (nCount > (row - 1))
		{
			auto& pInter = _vInters.at(row - 1);
			CString strMarker;
			strMarker.Format("m%05d", row);

			if (pInter)
			{
				//pInter->gubn = 'm';  //test mod 'm'
				pInter.get()->gubn = ROW_COMMENT;
				pInter.get()->code = strMarker;
			}

			m_grid->SetItemText(row, colCODE, strMarker);

			m_grid->SetFocusCellEdit(row, colNAME, true);

			if (m_bAddCnd)
			{
				saveInterest();
			}
		}
	}
	break;
	case userBASE + 10: // 책갈피삭제
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			m_bEditWork = TRUE;

		if (gsl::narrow_cast<int>(_vInters.size()) >= row - 1)
		{
			if (m_grid->GetItemAttr(row, colNAME) & GVAT_MARKER)
			{
				DeleteRow(row);
				insertInterest(MAX_LINE - 1);
				saveServer(_vInters, -1);
			}
		}
	}
	break;
	case userBASE + 11: //메모 삭제 091013 수정
	{
		if (MessageBox("메모를 삭제하시겠습니까?\n", "메모 삭제 확인창", MB_YESNO) == IDYES)
		{		
			m_grid->delectMemo(row);
		}
	}
	break;
	case userBASE + 12: //북마크 지정 091102
	{
	
	}
	break;

	case userBASE + 13: //북마크 삭제
	{
		m_bEditWork = TRUE;
		deleteBookmark(row);
	}
	break;
	case userBASE + 14: //복수현재가2에 종목 연동하기
		sendtoMutiHoga();
		break;
		// END ADD
		// END ADD
	case subBASE + 1:
	case subBASE + 2:
	case subBASE + 3:
	case subBASE + 4:
	case subBASE + 5:
	case subBASE + 6:
	case subBASE + 7:
	case subBASE + 8:
	case subBASE + 9:
	case subBASE + 10:
	case subBASE + 11:
	case subBASE + 12:
		if (index > subBASE)
		{
			m_bEditWork = TRUE;
			setBookmark(row, AxStd::FORMAT(_T("%02d"), index - subBASE));
		}
	break;

	default:
		break;
	
	}
	

}

void CGridWnd::InsertEmpty(int row)
{
	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	int rowcount = 0;

	if (nOver == MO_VISIBLE)
	{
		pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
		rowcount = ((CGroupWnd *)pWnd)->sumEachGroupCount();
	}
	else
	{
		rowcount = GetRowValidcount();
	}

	if (rowcount >= MAX_LINE)
	{

		Variant(guideCC, IH::idTOstring(IDS_GUIDE4));
		return;
	}

	DeleteRow(MAX_LINE);
	insertRow(row);
}

void CGridWnd::hawkEyeColor(int row)
{
	double curr{}, open{}, high{}, low{};
	double pcurr{}, popen{}, phigh{}, plow{}, dval{};

	curr = IH::TOfabs(m_grid->GetItemText(row, colCURR));
	open = IH::TOfabs(m_grid->GetItemText(row, colOPEN));
	high = IH::TOfabs(m_grid->GetItemText(row, colHIGH));
	low = IH::TOfabs(m_grid->GetItemText(row, colLOW));

	pcurr = IH::TOfabs(m_grid->GetItemText(row, colPCURR));
	popen = IH::TOfabs(m_grid->GetItemText(row, colPOPEN));
	phigh = IH::TOfabs(m_grid->GetItemText(row, colPHIGH));
	plow = IH::TOfabs(m_grid->GetItemText(row, colPLOW));

	if (curr <= 0)
		curr = pcurr;
	if (curr <= 0)
		return;

	// colNAME foreground
	if (open >= phigh) // 상승갭
	{
		m_grid->SetItemFgColor(row, colNAME, colorKKRED);
	}
	else if (open <= plow) // 하락갭
	{
		m_grid->SetItemFgColor(row, colNAME, colorKKBLUE);
	}
	else
	{
		m_grid->SetItemFgColor(row, colNAME, getAxColor(69));
	}

	// colNAME background
	if (curr >= high)
	{
		m_grid->SetItemBkColor(row, colNAME, colorLTRED);
	}
	else if (curr <= low)
	{
		m_grid->SetItemBkColor(row, colNAME, colorLTBLUE);
	}
	else
	{
		m_grid->SetItemBkColor(row, colNAME, getAxColor(68));
	}

	dval = (phigh - plow) / 3.;

	// colCURR background
	if (curr >= phigh)
	{
		m_grid->SetItemBkColor(row, colCURR, colorDKRED);
	}
	else if (curr >= (plow + dval * 2))
	{
		m_grid->SetItemBkColor(row, colCURR, colorLTRED);
	}
	else if (curr >= (plow + dval))
	{
		m_grid->SetItemBkColor(row, colCURR, getAxColor(68));
	}
	else if (curr >= plow)
	{
		m_grid->SetItemBkColor(row, colCURR, colorLTBLUE);
	}
	else if (curr < plow)
	{
		m_grid->SetItemBkColor(row, colCURR, colorDKBLUE);
	}
	else // default
	{
		//		TRACE("check  error....\n");
	}
}

void CGridWnd::DeleteRow(int row)
{
	// 2011.11.09 KSJ
	// GetRowCount가 2이면 실제 Row가 하나 남은 것이므로 지우면 안됨.
	//지우면 Insert가 되지 않음.
	// row가 0이면 (티커)임..

	CString strCode;
	if (m_grid->GetRowCount() <= 2 || row <= 0)
		return;

	// 2012.12.18 KSJ 정렬을 한상태에서 삭제하면 해당하는 row와 m_inters의 값이 맞지 않다.
	// m_inters의 값은 baseSorting 기준으로 되어 있기 때문임.
	//그래서 코드를 가지고 위치를 찾아야 한다.
	strCode = m_grid->GetItemText(row, colCODE);

	m_grid->DeleteRow(row);
	m_grid->memoDelete(row);
	m_rowC--;

	// 2013.01.28 KSJ 코드를 찾아서 지우면 빈줄일때는 지워지지 않는다.
	//그래서 코드가 빈값일때는 row -1을 지운다.
	strCode.TrimLeft();
	strCode.TrimRight();
	if (!strCode.IsEmpty())
		RemoveInters(GetInter(strCode));
	else
		RemoveInters(row - 1);
	// 2013.01.28 KSJ End
	// 2012.12.18 KSJ End

	ReIndexing();

	ClearSearchMap(); // 2011.12.29 KSJ
	ReSetSearchMap(); // 2011.12.29 KSJ
}

void CGridWnd::insertInterest(int idx)
{
	const auto& it = _vInters.emplace(_vInters.begin() + idx, std::move(std::make_shared<struct _intersx>()));
	it->get()->gubn     = '0';
	it->get()->bookmark = '0'; // 2014.05.16 KSJ 기본값
}

void CGridWnd::initialGrid(bool size)
{
	if (m_nIndex == 0)
	{
		CintGrid::m_dropdata.Reset();
	}

	m_grid->Initial(m_rowC, m_colC, 1, 0); // colCURR + 1);//BUFFET SCROLL BAR


	// ADD PSH 20070911
	m_grid->SetMarginRatio(m_bMargin);
	m_grid->SetBkMarkerColor(m_clrMarkerBKG);
	m_grid->SetMarkerColor(m_clrMarkerTXT);
	m_grid->SetMarkerShadow(m_bShadow);
	m_grid->SetInfo(m_bInfo);
	////////////////////////////////////////////////////
	const char use = ((CGroupWnd *)m_pGroupWnd)->GetBkUse();
	if (use == '1' || use == '2')
	{
		int nLine = ((CGroupWnd *)m_pGroupWnd)->GetLine();
		const COLORREF bkColor = ((CGroupWnd *)m_pGroupWnd)->GetBkColor1();
		const COLORREF bkCol2 = ((CGroupWnd *)m_pGroupWnd)->GetBkColor2();

		if (nLine <= 0)
			nLine = 1;
		m_grid->SetStepColor(nLine, bkColor, bkCol2); // BUFF
	}
	else


	m_grid->SetStepColor(1, getAxColor(68), getAxColor(77)); // BUFF
	m_nRowHeight = m_grid->GetRowHeight(0);
	// END ADD

	LOGFONT lf{}, lfB{};
	const int fontsize = ((CGroupWnd *)m_pGroupWnd)->GetFontSize();
	CString fontname = ((CGroupWnd *)m_pGroupWnd)->GetFontName();
	// MODIFY PSH 20070917
	// BOOL	bBold = ((CGroupWnd*)m_pGroupWnd)->GetCurrBold();
	const BOOL bCurrBold = ((CGroupWnd *)m_pGroupWnd)->GetCurrBold();
	const BOOL bNameBold = ((CGroupWnd *)m_pGroupWnd)->GetNameBold();
	const BOOL bAllBold = ((CGroupWnd *)m_pGroupWnd)->GetAllBold();
	// END MODIFY

	m_pFont  = getAxFont(fontname, fontsize);
	m_pFontB = getAxFont(fontname, fontsize, true);


	GVITEM gvitem;
	_gridHdr xgridHdr{};

	//////////////////////////////////////////////////////////////////////////
	// 2013.03.27 KSJ 글꼴크기에 따라 폭넓이 조정
	CString IniFile;
	int nResize = 0;
	int readL = 0;
	char readB[1024]{};

	IniFile.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_FIELD);

	nResize = GetPrivateProfileInt("INTERFIELD", "RESIZE", 0, IniFile);
	readL = GetPrivateProfileString("INTERFIELD", "FONTPOINT", "", readB, sizeof(readB), IniFile);

	// 2013.04.30 KSJ nResize가 0이아니고 false일때 리사이즈 하지 않는다.
	//그래야 여러그룹 및 여러창을 띄웠을때 전부 조절된다.
	if (nResize != 0 && size == false)
		return;
	//////////////////////////////////////////////////////////////////////////
	// column header setting
	for (int ii = 0; ii < m_colC; ii++)
	{
		xgridHdr = m_gridHdrX.GetAt(ii);

		gvitem.row    = 0;
		gvitem.col    = ii;
		gvitem.state  = 0;
		gvitem.mask   = GVMK_TEXT | GVMK_FORMAT | GVMK_FONT | GVMK_ATTR | GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_MISC;
		gvitem.format = GVFM_HEADER;
		gvitem.fgcol  = getAxColor(76);
		gvitem.bkcol  = getAxColor(74);

		//CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));	
		gvitem.font = m_pFont;

		if (xgridHdr.stid > 0)
			gvitem.text = IH::idTOstring(xgridHdr.stid);
		gvitem.attr = xgridHdr.attr;
		m_grid->SetItem(&gvitem);

		if (ii == colCODE)
		{
			m_grid->SetColumnWidth(ii, m_bDispCode ? m_nCodeWidth : 0);
		}
		else if (ii == colSIG)
		{
			// 2011.12.02 KSJ

			m_grid->SetColumnWidth(ii, m_bNewsNChart ? m_nNewsNChartWidth : 0);

			if (m_bNewsNChart)
				m_grid->SetColumnWidth(ii, ((CGroupWnd *)GetParent())->GetRowHeight());
			// KSJ
		}
		else
		{
			if (size)
				m_grid->SetColumnWidth(ii, xgridHdr.width);
		}
	}

	for (int ii = 0; ii < m_colC; ii++)
	{
		xgridHdr = m_gridHdrX.GetAt(ii);

		gvitem.state = 0;
		gvitem.mask = GVMK_FORMAT | GVMK_FONT | GVMK_ATTR | GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_SYMBOL;

		gvitem.format = xgridHdr.format;
		gvitem.symbol = xgridHdr.symbol;
		gvitem.attr = xgridHdr.attrx;

		gvitem.fgcol = getAxColor(69);
		// MODIFY PSH 20070912
		// gvitem.bkcol = GetAxColor(68);
		gvitem.bkcol = ((CGroupWnd *)m_pGroupWnd)->GetRTMColor();
		// END MODIFY

	//	CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		gvitem.font = m_pFont;

		// MODIFY PSH 20070911
		if ((ii == colCODE) || (ii == colNAME) || (ii == colCURR) || bAllBold)
		// END MODIFY
		{					   // 20070706 kwon
			if (gvitem.symbol.Compare("2024")) // 2012.11.29 KSJ 대비일때는 GVAT_CONDITIONx하면 색지정이 안됨
				gvitem.attr |= GVAT_CONDITIONx;

			// MODIFY PSH 20070917
			// if (bBold)
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold) || bAllBold)
			// END MODIFY
			{
				//CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
				gvitem.font = m_pFontB;
			}
		}

		gvitem.bkcol = getAxColor(68);

		if (ii == colNAME)
		{
			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}

		m_grid->SetColumnItems(ii, &gvitem);

		if (ii == colCODE)
		{
			m_grid->SetColumnWidth(ii, m_bDispCode ? m_nCodeWidth : 0);

			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colCODE;
		}
		else if (ii == colSIG)
		{
			// 2011.12.02 KSJ
			m_grid->SetColumnWidth(ii, m_bNewsNChart ? m_nNewsNChartWidth : 0);

			gvitem.mask |= GVMK_PARAM;
			gvitem.param = colSIG;

			if (m_bNewsNChart)
			{
				m_grid->SetColumnWidth(ii, ((CGroupWnd *)GetParent())->GetRowHeight());
			}
			// KSJ
		}
		else
		{
			if (size)
			{
				// 				CString strTemp;

				if (nResize < 0) //폭 넓이 늘이기
				{
					m_grid->SetColumnWidth(ii, gsl::narrow_cast<int>(xgridHdr.width * (1.0 + abs(nResize) / 20.0) + 0.5));
				}
				else if (nResize > 0) //폭 넓이 줄이기
				{
					m_grid->SetColumnWidth(ii, gsl::narrow_cast<int>(xgridHdr.width / (1.0 + abs(nResize) / 20.0) + 0.5));
				}
				else
				{
					m_grid->SetColumnWidth(ii, xgridHdr.width);
				}
			}
		}
	}

	// 2011.12.12 KSJ
	SetDispCode(m_bDispCode);
	SetNewsNChart(m_bNewsNChart);
	SetLineColor();
	// ADD PSH 20070918
	m_grid->SetBkColor(RGB(255, 255, 255)); // BUFFET FOR SAME BACKCOLOR
	m_grid->SetFixBkColor(getAxColor(74));
	// END ADD
	m_grid->SetBKSelColor(getAxColor(78));
	m_grid->SetRowColResize(FALSE, TRUE);
	m_grid->SetKeepingColumn(colCURR + 1); // keep column count
	m_grid->SetGridFocusLine(FALSE, TRUE);
	m_grid->SetGridColor(getAxColor(65));
	m_grid->SetConditionColor(getAxColor(CLR_UP), getAxColor(CLR_DN), getAxColor(CLR_TEXT));
	m_grid->SetTitleTips(TRUE);
	m_grid->Adjust();

}

// ADD PSH 20070912
void CGridWnd::InitPopupMenu()
{
	m_menuSubHeader.CreatePopupMenu();
	m_menuSubHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND1, "항목추가시 자동저장");
	m_menuSubHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND2, "항목변경시 자동저장");

	m_menuHeader.CreatePopupMenu();
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_INTERSAVE, "관심종목 등록하기");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_INTERTAKE, "관심종목 덮어쓰기");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_BASESORT, "등록순으로 정렬");

	m_menuHeader.AppendMenu(MF_SEPARATOR);
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_MULTICHART, "선택종목 멀티차트 보기");
	m_menuHeader.AppendMenu(MF_SEPARATOR);
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_NEWSNCHART, "뉴스/차트 보기"); // 2011.12.02 KSJ
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_CODE, "종목코드 보기");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_INFO, "종목특이사항 보기");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_MARGIN, "주식증거금 보기");
//	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_TICKER, "관심티커 보기");
	m_menuHeader.AppendMenu(MF_SEPARATOR);
//	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_TICKER_SETUP, "관심티커 설정");
	m_menuHeader.AppendMenu(MF_STRING, IDC_MENU_SAVE_CND1, "편의기능 설정");

	m_menuHeader.CheckMenuItem(IDC_MENU_NEWSNCHART, m_bNewsNChart ? MF_CHECKED : MF_UNCHECKED); // 2011.12.02 KSJ
	m_menuHeader.CheckMenuItem(IDC_MENU_CODE, m_bDispCode ? MF_CHECKED : MF_UNCHECKED);
	m_menuHeader.CheckMenuItem(IDC_MENU_INFO, m_bInfo ? MF_CHECKED : MF_UNCHECKED);
	m_menuHeader.CheckMenuItem(IDC_MENU_MARGIN, m_bMargin ? MF_CHECKED : MF_UNCHECKED);
	// m_menuHeader.SetSelectDisableMode(TRUE);
}

void CGridWnd::SetInfo(BOOL bInfo /* = TRUE */)
{
	m_bInfo = bInfo;

	m_menuHeader.CheckMenuItem(IDC_MENU_INFO, m_bInfo ? MF_CHECKED : MF_UNCHECKED);

	m_grid->SetInfo(m_bInfo);

	CRect rcWnd;
	GetWindowRect(rcWnd);
	ScreenToClient(rcWnd);

	InvalidateRect(rcWnd);
}
// KSJ

void CGridWnd::ShowPopupMenu(int nX /* = -1 */, int nY /* = -1 */)
{
	if ((nX == -1) || (nY == -1))
	{
		CPoint point;
		GetCursorPos(&point);

		nX = point.x;
		nY = point.y;
	}

	int nResult{};

	if (m_bPopupDlg)
	{
		CPopupDlg dlg;
		dlg.m_nX = nX;
		dlg.m_nY = nY;

		const int nResultID[] = {
		    IDC_MENU_SAVE_CND1,
		    IDC_MENU_SAVE_CND2,
		    IDC_MENU_INTERSAVE,
		    IDC_MENU_INTERTAKE,
		    IDC_MENU_BASESORT,
		    IDC_MENU_MULTICHART,
		    IDC_MENU_INFO,
		    IDC_MENU_MARGIN,
		    IDC_MENU_TICKER,
		    IDC_MENU_TICKER_SETUP,
		    IDC_MENU_SAVE_CND1,
		};

		constexpr int nBase = 100;

		dlg.AddMenu(nBase + 2, "관심종목 등록하기");

		if (!m_szTitle.IsEmpty())
			dlg.AddMenu(nBase + 3, "관심종목 덮어쓰기");

		dlg.AddMenu(nBase + 4, "등록순으로 정렬");
		//		dlg.AddMenu(nBase + 12, "업종순으로 정렬");
		//		dlg.AddMenu(nBase + 13, "종목명순으로 정렬");
		//		dlg.AddMenu(nBase + 14, "종목코드으로 정렬");

		dlg.AddMenu(nBase + 5, "선택종목 멀티차트 보기");
		//		dlg.AddMenu(nBase + 6, "체결리스트 보기");

		dlg.AddMenu(nBase + 7, "종목특이사항 보기", m_bInfo);
		dlg.AddMenu(nBase + 8, "주식증거금 보기", m_bMargin);

		/*
				dlg.AddMenu(nBase + 9, "관심티커 보기", m_bTicker);
		*/

//		dlg.AddMenu(nBase + 10, "관심티커 설정");
		dlg.AddMenu(nBase + 11, "편의기능 설정");

		if (-1 == dlg.DoModal())
			return;

		nResult = nResultID[dlg.m_nResult - nBase];
	}
	else
	{
		m_menuHeader.EnableMenuItem(IDC_MENU_INTERTAKE, m_szTitle.IsEmpty() ? MF_DISABLED : MF_ENABLED);
		m_menuHeader.EnableMenuItem(IDC_MENU_MARKER, m_bSorting ? MF_DISABLED : MF_ENABLED);

		nResult = m_menuHeader.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, nX, nY, this);
	}

	switch (nResult)
	{
	case IDC_MENU_INTERSAVE:
	{
		saveInterestX();
	}
	break;
	case IDC_MENU_INTERTAKE:
	{
		saveInterest();
	}
	break;
	case IDC_MENU_BASESORT:
	{
		BaseSorting();
	}
	break;
	case IDC_MENU_UPJONGSORT:
	{
	}
	break;
	case IDC_MENU_NAMESORT:
	{
	}
	break;
	case IDC_MENU_CODESORT:
	{
	}
	break;
	case IDC_MENU_MULTICHART:
	{
		MultiChart();
	}
	break;
		// 2011.12.02 KSJ 뉴스와 차트 보이기
	case IDC_MENU_NEWSNCHART:
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_NEWSNCHART, !m_bNewsNChart);
		m_menuHeader.CheckMenuItem(IDC_MENU_NEWSNCHART, m_bNewsNChart ? MF_CHECKED : MF_UNCHECKED);
	}
	break;
		// KSJ

	case IDC_MENU_CODE:
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_MARKETCODE, !m_bDispCode);
		m_menuHeader.CheckMenuItem(IDC_MENU_CODE, m_bDispCode ? MF_CHECKED : MF_UNCHECKED);
	}
	break;
	case IDC_MENU_INFO:
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_INFO, !m_bInfo);
		m_menuHeader.CheckMenuItem(IDC_MENU_INFO, m_bInfo ? MF_CHECKED : MF_UNCHECKED);
		m_grid->SetInfo(m_bInfo);
	}
	break;
	case IDC_MENU_MARGIN:
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_MARGIN, !m_bMargin);
		m_menuHeader.CheckMenuItem(IDC_MENU_MARGIN, m_bMargin ? MF_CHECKED : MF_UNCHECKED);
		m_grid->SetMarginRatio(m_bMargin);
		m_grid->Invalidate(false);
	}
	break;
	case IDC_MENU_TICKER:
	{
	}
	break;
	case IDC_MENU_TICKER_SETUP:
	{
		Csetup dlg(nullptr, &m_tkConfig);
		dlg.DoModal();
	}
	break;
	case IDC_MENU_SAVE_CND1:
	{
		CUsefulSetupDlg dlg;

		dlg.m_bClrApply = m_bMKClrApply;
		dlg.m_clrKospi = m_clrKospi;
		dlg.m_clrKosdaq = m_clrKosdaq;

		dlg.m_clrMarkerBKG = m_clrMarkerBKG;
		dlg.m_clrMarkerTXT = m_clrMarkerTXT;
		dlg.m_bShadow = m_bShadow;
		dlg.m_bMKAllApply = m_bAllApply;

		dlg.m_strFile = m_strWaveFile;
		dlg.m_bWaveApply = m_bWaveApply;

		dlg.m_bAllApply = m_bAutoSaveAllApply;
		dlg.m_bAddCnd = m_bAddCnd;
		dlg.m_bChgCnd = m_bChgCnd;

		if (dlg.DoModal())
		{
			if (-1 != dlg.m_strFile.Find("psh0419"))
			{
				CString strTemp;
				if (nullptr == m_pRCDlg)
				{
					m_pRCDlg = std::make_unique<CRealCheckDlg>();
					m_pRCDlg->Create(IDD_REALTIME_CHECK);
					m_pRCDlg->ShowWindow(SW_SHOW);

					m_pRCDlg->m_stcTitle.SetWindowText(m_szTitle);
					m_pRCDlg->m_pGridWnd = this;
				}
				else
				{
					m_pRCDlg->ShowWindow(SW_SHOW);
				}

				const int nCnt = m_pRCDlg->m_listGrid.GetCount();

				for (int i = nCnt - 1; i >= 0; i--)
				{
					m_pRCDlg->m_listGrid.DeleteString(i);
				}

				m_pRCDlg->m_edSymbol.SetWindowText("");
				m_pRCDlg->m_edData.SetWindowText("");

				for (int i = 1; i < m_rowC; i++)
				{
					strTemp.Format("%03d : %s - %s", i, m_grid->GetItemText(i, colRCOD), m_grid->GetItemText(i, colCODE));
					m_pRCDlg->m_listGrid.AddString(strTemp);
				}

				break;
			}

			if (dlg.m_bClrApply != m_bMKClrApply ||
			    dlg.m_clrKospi != m_clrKospi ||
			    dlg.m_clrKosdaq != m_clrKosdaq)
			{
				m_pGroupWnd->SendMessage(WM_MANAGE, MK_MARKETCOLOR, (LPARAM)&dlg);
			}

			if (dlg.m_clrMarkerBKG != m_clrMarkerBKG ||
			    dlg.m_clrMarkerTXT != m_clrMarkerTXT ||
			    dlg.m_bShadow != m_bShadow ||
			    dlg.m_bMKAllApply != m_bAllApply)
			{
				_marker mkProp;
				mkProp.clrMarkerBKG = dlg.m_clrMarkerBKG;
				mkProp.clrMarkerTXT = dlg.m_clrMarkerTXT;
				mkProp.bShadow = dlg.m_bShadow;
				mkProp.bAllApply = dlg.m_bMKAllApply;

				if (dlg.m_bMKAllApply)
				{
					m_pGroupWnd->SendMessage(WM_MANAGE, MK_MKPROP, (LPARAM)&mkProp);
				}
				else
				{
					SetMarkerProp(&mkProp);
				}
			}

			if (dlg.m_strFile != m_strWaveFile ||
			    dlg.m_bWaveApply != m_bWaveApply)
			{
				m_pGroupWnd->SendMessage(WM_MANAGE, MK_ALERTWAVE, (LPARAM)&dlg);
			}

			if (dlg.m_bAllApply != m_bAutoSaveAllApply ||
			    dlg.m_bAddCnd != m_bAddCnd ||
			    dlg.m_bChgCnd != m_bChgCnd)
			{
				m_pGroupWnd->SendMessage(WM_MANAGE, MK_AUTOSAVE, (LPARAM)&dlg);
			}
		}
	}
	break;
	}
}

bool CGridWnd::GetCodeMap(CMap<CString, LPCTSTR, int, int> &arCodeMap)
{
	CString code;
	if (m_rowC <= 1)
		return false;

	for (int ii = 1; ii < m_rowC; ii++)
	{
		code = m_grid->GetItemText(ii, colCODE);
		if (code != "")
		{
			arCodeMap.SetAt(code, ii);
		}
	}

	return true;
}

void CGridWnd::BaseSorting()
{
	CMap<CString, LPCTSTR, int, int> mapBaseSort, mapCurCode;
	CString strIndex, strCode;

	const int nCnt = _arrBaseInters.size();
	if (1 > nCnt)
		return;
	if (!GetCodeMap(mapCurCode))
		return;

XMSG("정상작동하니 ?");
	int ii = 0;
	for_each(_arrBaseInters.begin(), _arrBaseInters.end(), [&](auto& item) {
		if (item->code.IsEmpty())
			mapBaseSort.SetAt(strCode, ii++);
	});

	//_intersx *pInter = nullptr;
	//for (int i = 0; i < nCnt; i++)
	//{
	//	if (pInter = m_arrBaseInters.GetAt(i))
	//	{
	//		strCode = pInter->code;
	//	}

	//	if (!strCode.IsEmpty())
	//	{
	//		mapBaseSort.SetAt(strCode, i);
	//	}
	//}

	m_grid->SortBase(mapBaseSort, mapCurCode);
	m_bSorting = FALSE;
}

void CGridWnd::SetLineColor()
{
	for (int ii = 0; ii < m_grid->GetRowCount(); ii++)
	{
		SetLineColor(ii);
	}
}

void CGridWnd::SetLineColor(int nRow, CintGrid *pGrid)
{
	if (pGrid == nullptr)
		pGrid = m_grid.get();

	COLORREF bkColor{};
	pGrid->SetRowHeight(nRow, ((CGroupWnd *)m_pGroupWnd)->GetRowHeight());

	if (!nRow)
		return;

	bkColor = ((CGroupWnd *)m_pGroupWnd)->GetBkColor1();

	if (nRow > 1 && ((nRow - 1) / ((CGroupWnd *)m_pGroupWnd)->GetLine()) % 2)
		bkColor = ((CGroupWnd *)m_pGroupWnd)->GetBkColor2();

	for (int jj = 0; jj < m_colC; jj++)
	{
		if (jj == colNAME)
			pGrid->SetItemFgColor(nRow, colNAME, getAxColor(69));

		pGrid->SetItemBkColor(nRow, jj, bkColor);
	}
}
// END MODIFY

//북마크 색 지정하기
void CGridWnd::setBookmark(int row, CString indexrgb)
{
	const int rowinter = row -1;
	const int count = _vInters.size();
	if (rowinter > count)
		return;

	auto&pInter = _vInters.at(rowinter);
	pInter.get()->bookmark = '1';  
	pInter.get()->bookrgb = indexrgb;
	pInter.get()->gubn = ROW_BOOKMARK;
	
	m_nBookMarkRow = rowinter;
//	saveInterest(false, -1, true);

	m_grid->Adjust();
}

void CGridWnd::deleteBookmark(int row)
{
	const int rowinter = row - 1;
	const int count = _vInters.size();

	if (rowinter > count)
		return;

	//북마크 지정
	auto& pInter = _vInters.at(row - 1);
	pInter.get()->bookrgb = "00";
	pInter.get()->bookmark = '0';  
	m_nBookMarkRow = row - 1;
	//saveInterest(false, -1, true);

	//sendTransaction(row - 1);
	m_grid->Adjust();
}

COLORREF CGridWnd::GetBookMarkColor()
{
	return ((CGroupWnd *)m_pGroupWnd)->GetBookMarkColor();
}

COLORREF CGridWnd::GetBookMarkPenColor()
{
	return ((CGroupWnd *)m_pGroupWnd)->GetBookMarkPenColor();
}

int CGridWnd::insertRow2(int afterRow, int beforeRow, BOOL bInsertInter)
{
	if (afterRow > m_grid->GetRowCount())
		afterRow = m_grid->GetRowCount();

	m_rowC++;


	const int ret = m_grid->InsertRow("", afterRow);
	const int count = _vInters.size();

	if (count <= beforeRow)
		return ret;

	auto& pInter = _vInters.at(beforeRow);

	//	if (bInsertInter)
	//		insertInterest(row - 1);

	LOGFONT lf{}, lfB{};
	const int fontsize = ((CGroupWnd *)m_pGroupWnd)->GetFontSize();
	CString fontname = ((CGroupWnd *)m_pGroupWnd)->GetFontName();

	const BOOL bCurrBold = ((CGroupWnd *)m_pGroupWnd)->GetCurrBold();
	const BOOL bNameBold = ((CGroupWnd *)m_pGroupWnd)->GetNameBold();
	const BOOL bAllBold = ((CGroupWnd *)m_pGroupWnd)->GetAllBold();

	m_pFont = getAxFont(fontname, fontsize);
	m_pFontB = getAxFont(fontname, fontsize, true);

	//m_pFont->GetLogFont(&lf);
	//lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	//m_pFontB->GetLogFont(&lfB);
	//lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	GVITEM gvitem;
	_gridHdr xgridHdr{};
	for (int ii = 0; ii < m_colC; ii++)
	{
		xgridHdr = m_gridHdrX.GetAt(ii);

		gvitem.row = afterRow;
		gvitem.col = ii;
		gvitem.state = 0;
		gvitem.mask = GVMK_FORMAT | GVMK_FONT | GVMK_ATTR | GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_SYMBOL;

		gvitem.format = xgridHdr.format;
		gvitem.symbol = xgridHdr.symbol;
		gvitem.attr = xgridHdr.attrx;
		gvitem.fgcol = getAxColor(69);

		//북마크되어있는지 확인
		if (pInter.get()->bookmark == '1')
		{
			gvitem.bkcol = ((CGroupWnd *)m_pGroupWnd)->GetBookMarkColor();
		}
		else
		{
			gvitem.bkcol = getAxColor(68);
		}

		//CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		gvitem.font = m_pFont;

		if ((ii == colCODE) || (ii == colNAME) || (ii == colCURR) || bAllBold)
		{					   // 20070706 kwon
			if (gvitem.symbol.Compare("2024")) // 2012.11.29 KSJ 대비일때는 GVAT_CONDITIONx하면 색지정이 안됨
				gvitem.attr |= GVAT_CONDITIONx;

			// MODIFY PSH 20070917
			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold) || bAllBold)
			{
				//CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
				gvitem.font = m_pFontB;
			}
		}
		if (ii == colNAME)
		{
			gvitem.param = colCODE;
		}

		if (ii == colCODE)
		{
			gvitem.mask |= GVMK_PARAM;
			CString string = m_grid->GetItemText(afterRow, colCODE);
			int result = 2;

			if (string.GetLength() == 6)
			{
				_Wizard->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)string);

				if (result == 1)
				{
					result = 1;
				}
				else if (result == 2)
				{
					result = 0;
				}
				else
				{
					result = 2;
				}
			}

			gvitem.param = (LPARAM)result;
		}

		m_grid->SetItem(&gvitem);
	}

	m_grid->SetGridFocusLine(FALSE, TRUE);

	ClearSearchMap(); // 2011.12.29 KSJ
	ReSetSearchMap(); // 2011.12.29 KSJ

	return ret;
}

int CGridWnd::insertRow(int row, BOOL bInsertInter /* = TRUE */)
{
	// 2011.11.09 KSJ
	// row가 0이면 (티커)임..
	if (row <= 0)
		return 0;

	if (row > m_grid->GetRowCount())
		row = m_grid->GetRowCount();

	m_rowC++;

	LOGFONT lf{}, lfB{};
	const int fontsize = ((CGroupWnd *)m_pGroupWnd)->GetFontSize();
	CString fontname = ((CGroupWnd *)m_pGroupWnd)->GetFontName();
	const BOOL bCurrBold = ((CGroupWnd *)m_pGroupWnd)->GetCurrBold();
	const BOOL bNameBold = ((CGroupWnd *)m_pGroupWnd)->GetNameBold();
	const BOOL bAllBold = ((CGroupWnd *)m_pGroupWnd)->GetAllBold();

	m_pFont = getAxFont(fontname, fontsize);
	m_pFontB = getAxFont(fontname, fontsize, true);


	//m_pFont->GetLogFont(&lf);
	//lf.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);
	//m_pFontB->GetLogFont(&lfB);
	//lfB.lfHeight = fontsize * 10 + (fontsize < 9 ? 5 : 0);

	if (row >= gsl::narrow_cast<int>(_vInters.size()))
	{
		return 0;
	}


	const int ret = m_grid->InsertRow("", row);
	auto& pInter = _vInters.at(row);

	GVITEM gvitem{};
	const COLORREF bookcolor = ((CGroupWnd*)m_pGroupWnd)->GetBookMarkColor();
	const COLORREF fontcolor = getAxColor(69);
	const COLORREF color = getAxColor(68);
	for (int ii = 0; ii < m_colC; ii++)
	{
		const _gridHdr xgridHdr = m_gridHdrX.GetAt(ii);
		gvitem.row = row;
		gvitem.col = ii;
		gvitem.state = 0;
		gvitem.mask = GVMK_FORMAT | GVMK_FONT | GVMK_ATTR | GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_SYMBOL;
		gvitem.format = xgridHdr.format;
		gvitem.symbol = xgridHdr.symbol;
		gvitem.attr   = xgridHdr.attrx & ~GVAT_MARKER;
		gvitem.fgcol  = fontcolor;

		//북마크되어있는지 확인
		if (pInter.get()->bookmark == '1')
		{
			gvitem.bkcol = bookcolor;
		}
		else
		{
			gvitem.bkcol = color;
		}

		//CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));
		gvitem.font = m_pFont;

		if ((ii == colCODE) || (ii == colCURR) || (ii == colNAME) || bAllBold)
		{					   // 20070706 kwon
			if (gvitem.symbol.Compare("2024")) // 2012.11.29 KSJ 대비일때는 GVAT_CONDITIONx하면 색지정이 안됨
				gvitem.attr |= GVAT_CONDITIONx;

			if ((ii == colCODE) || (ii == colCURR && bCurrBold) || (ii == colNAME && bNameBold) || bAllBold)
			{
				//CopyMemory(&gvitem.font, &lfB, sizeof(LOGFONT));
				gvitem.font = m_pFontB;
			}
		}
		if (ii == colNAME)
		{
			gvitem.param = colCODE;
		}
		m_grid->SetItem(&gvitem);
	}

	m_grid->SetGridFocusLine(FALSE, TRUE);
	if (bInsertInter)
	{
		ClearSearchMap(); // 2011.12.29 KSJ
		ReSetSearchMap(); // 2011.12.29 KSJ
	}
	return ret;
}

void CGridWnd::parsingField(CString symbol, bool bDlgSetup /* = false*/)
{
	CString string;
	const int gridHdrC = sizeof(gridHdr) / sizeof(_gridHdr);
	m_gridHdrX.RemoveAll();

	if (CheckField(symbol)) //호가 받아야 하는 필드 설정을 하였을 경우 false , 필드가 없을경우 TRUE
		m_bSkipRTS = false;
	else
		m_bSkipRTS = true;
	m_pMainWnd->SendMessage(WM_MANAGE, MK_ISSKIP, m_bSkipRTS);

	for (int ii = 0; ii < gridHdrC; ii++)
	{
		if (gridHdr[ii].needs == 2)
		{
			m_gridHdrX.Add(gridHdr[ii]);
		}
	}

	if (symbol.IsEmpty())
	{
		m_szFIELD = _T(""), m_szWIDTH = _T("");
		for (int ii = 0; ii < gridHdrC; ii++)
		{
			if (gridHdr[ii].needs == 1) // default
				m_gridHdrX.Add(gridHdr[ii]);
		}
		return;
	}

	if (!symbol.IsEmpty())
		IH::Parser(symbol, deliMITER); // field name 제거

	m_posField = false;
	m_ccField = false;
	int nIndex = 0;
	CString sztmp = _T(""), sztmp2;
	CMapStringToString mapSymbol;
	CWordArray arWidth;
	CStringArray arSymbol;

	int ncnt = m_gridHdrX.GetSize();
	int wcnt = 0;
	struct _gridHdr gHdr
	{
	};
	bool bField = true;

	sztmp.Format("%s", symbol);
	m_szFIELD = sztmp;

	while (!sztmp.IsEmpty())
	{
		string = IH::Parser(sztmp, deliMITER);
		arSymbol.Add(string);
	}

	sztmp.Format("%s", m_szWIDTH);
	while (sztmp.GetLength())
	{
		string = IH::Parser(sztmp, ",");

		if (string.IsEmpty())
			continue;

		arWidth.Add(atoi(string));
	}

	wcnt = arWidth.GetSize();
	sztmp.Format("%s", m_szFIELD);

	while (sztmp.GetLength())
	{
		string = IH::Parser(sztmp, ",");
		if (string.IsEmpty())
			continue;

		mapSymbol.SetAt(string, "");
	}

	wcnt = mapSymbol.GetCount();

	if (!bDlgSetup && wcnt == arSymbol.GetSize())
	{
		bField = true;

		CString strSym;
		for (int ii = 0; ii < wcnt; ii++)
		{
			strSym = arSymbol.GetAt(ii);
			if (!mapSymbol.Lookup(strSym, sztmp2))
			{
				if ((strSym == "#1##") || (strSym == "#4##") ||
				    (strSym == "#2##") || (strSym == "#5##") ||
				    (strSym == "#3##") || (strSym == "#6##"))
				{
					continue;
				}

				bField = false;
				break;
			}
		}

		if (bField)
		{
			symbol.Format("%s", m_szFIELD);
			symbol.Replace(",", ";");
		}
	}

	while (!symbol.IsEmpty())
	{
		string = IH::Parser(symbol, deliMITER);
		if (string.IsEmpty())
			continue;

		for (int ii = 0; ii < gridHdrC; ii++)
		{

			if (gridHdr[ii].needs == 2)
				continue;

			if (string.Compare(gridHdr[ii].symbol))
				continue;

			nIndex = m_gridHdrX.Add(gridHdr[ii]);
			sztmp.Format("%d", gridHdr[ii].stid);

			if ((gridHdr[ii].needs == 1) || (gridHdr[ii].needs == 3))
			{
				if ((string.Compare("#1##") == 0) || (string.Compare("#4##") == 0))
				{
					strcpy(m_gridHdrX[nIndex].symbol, "2029");
				}
				else if ((string.Compare("#2##") == 0) || (string.Compare("#5##") == 0))
				{
					strcpy(m_gridHdrX[nIndex].symbol, "2030");
				}
				else if ((string.Compare("#3##") == 0) || (string.Compare("#6##") == 0))
				{
					strcpy(m_gridHdrX[nIndex].symbol, "2031");
				}
			}

			if (gridHdr[ii].needs == 9)
			{
				m_posField = true;

				if (string.Compare("###3") == 0)
					m_sonikField = nIndex;
				if (string.Compare("###4") == 0)
					m_suikField = nIndex;
			}

			if (gridHdr[ii].needs == 8)
				m_ccField = true;

			if (gridHdr[ii].needs == 7)
				m_bongField = nIndex;

			if (gridHdr[ii].needs == 10)
				m_yDayVolField = nIndex;

			if (gridHdr[ii].needs == 11)
				m_EqualizerField = nIndex;

			if (gridHdr[ii].needs == 12)
				m_recomDataField = nIndex;

			if (gridHdr[ii].needs == 13)
				m_recomBasicCostField = nIndex;

			if (gridHdr[ii].needs == 14)
				m_recomSuikrateField = nIndex;

			if (gridHdr[ii].needs == 18)
				m_recomJisuGubnField = nIndex;

			if (gridHdr[ii].needs == 19)
				m_upjongCode = nIndex;

			if (gridHdr[ii].needs == 21)
				m_newCurrField = nIndex;

			if (gridHdr[ii].needs == 22)
				m_beforeVolField = nIndex;

			// 2016.04.15 KSJ epb 추천일자
			if (gridHdr[ii].needs == 25)
				m_epbDataField = nIndex;

			break;
		}
	}

	CString strSymbol;
	if (bField)
	{
		ncnt = m_gridHdrX.GetSize();
		wcnt = arWidth.GetSize();

		if (((ncnt - colNAME - 3) == wcnt))
		{
			for (int ii = 0; ii < wcnt; ii++)
			{
				gHdr = m_gridHdrX.GetAt(ii + colNAME);
				gHdr.width = arWidth.GetAt(ii);

				m_gridHdrX.SetAt(ii + colNAME, gHdr);
			}
		}
	}
}

bool CGridWnd::loadField(int fno /* = -1*/, bool bDlgSetup /* = false*/, int iEqualizer)
{
	CString tempFile;
	tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);

	CFileFind finder;
	if (!finder.FindFile(tempFile))
	{
		parsingField("", bDlgSetup);
		m_colC = m_gridHdrX.GetSize();
		return true;
	}

	if (fno < 0)
		return false;

	int readL = 0;
	char readB[1024];
	CString string, fieldno;

	fieldno.Format("%02d", fno);

	readL = GetPrivateProfileString("FIELDS", fieldno, "", readB, sizeof(readB), tempFile);

	string = CString(readB, readL);
	if (iEqualizer == 0) //일반
	{
		string += "2022;1915;";
	}
	else if (iEqualizer == 3) //추천종목
	{
		string = "필드 00;2024;2033;2027;7777;1910;1911;2022;1915;";
	}
	else if (iEqualizer == 4) // 2016.04.08 KSJ Epb종목
	{
		string = "필드 00;2024;2033;2027;7777;1999;2022;1915;"; // 2016.04.15 KSJ 1차 epb 종목 추천일자만 표시한다.
	}

	string += "1918;";
	parsingField(string, bDlgSetup);
	m_colC = m_gridHdrX.GetSize();

	WritePrivateProfileString("FIELDS", "ACTIVE", fieldno, tempFile);
	return true;
}

void CGridWnd::SaveAsInterField()
{
	if (m_szFIELD.IsEmpty())
		return;

	CString tempFile;
	tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_FIELD);

	CFileFind finder;
	if (!finder.FindFile(tempFile))
		return;

	const int nIndex = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_GETAFIELD);
	if (nIndex < 0)
		return;

	int readL = 0;
	char readB[1024]{};
	CString sField, sNewField, sFieldName, sFieldNo;

	sFieldNo.Format("%02d", nIndex);

	readL = GetPrivateProfileString("FIELDS", sFieldNo, "", readB, sizeof(readB), tempFile);
	sField = CString(readB, readL);
	const int index = sField.Find(deliMITER);
	if (index == -1)
		return;

	sFieldName = sField.Left(index);
	sField.Format("%s", m_szFIELD);
	sField.Replace(",", deliMITER);
	sNewField.Format("%s%s%s", sFieldName, deliMITER, sField);

	WritePrivateProfileString("FIELDS", sFieldNo, sNewField, tempFile);
}

void CGridWnd::parsingOubsUpjong(char *datB, int datL)
{
	const int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL)
		return;

	CString string, stringx, entry;

	string = CString(&datB[gridHoL], datL - gridHoL);

	CString strCode;
	CString tempData, tempStr;

	if (!string.IsEmpty())
	{
		stringx = IH::Parser(string, PNEW);
		tempData = IH::Parser(stringx, PTAB);
		tempData = IH::Parser(stringx, PTAB);
		tempData = IH::Parser(stringx, PTAB);
		tempData = IH::Parser(stringx, PTAB);
		m_dKospi = atof(tempData);
		tempStr.Format("%.2f", m_dKospi);
		m_dKospi = atof(tempStr);

		stringx = IH::Parser(string, PNEW);
		tempData = IH::Parser(stringx, PTAB);
		tempData = IH::Parser(stringx, PTAB);
		tempData = IH::Parser(stringx, PTAB);
		tempData = IH::Parser(stringx, PTAB);
		m_dKosdaq = atof(tempData);
		tempStr.Format("%.2f", m_dKosdaq);
		m_dKosdaq = atof(tempStr);
	}
}

void CGridWnd::parsingOubs(char *datB, int datL)
{
	// KSJ 장운영시간 저장
	CString strTime = ((CMainWnd *)m_pMainWnd)->GetMarketTime();
	CString strEndTime = ((CMainWnd *)m_pMainWnd)->GetMarketEndTime();
	CString strTemp;
	CString strCurr;

	if (!strTime.IsEmpty())
	{
		const char ch = 0x7e;
		strTime.Trim();
		strTime.Replace("+", "");
		strTime.Replace("-", "");
		strTime.Replace(" ", "");

		m_strBeginTime    = CalMaketTime(strTime.Mid(0, strTime.Find(ch)), false);
		m_strBeginTimeEnd = CalMaketTime(strTime.Mid(strTime.Find(ch) + 1, strTime.GetLength()), true);

		strEndTime.Trim();
		strEndTime.Replace("+", "");
		strEndTime.Replace("-", "");
		strEndTime.Replace(" ", "");
		m_strEndTime = CalMaketTime(strEndTime.Mid(0, strTime.Find(ch)), false);
		m_strEndTimeEnd = CalMaketTime(strEndTime.Mid(strEndTime.Find(ch) + 1, strEndTime.GetLength()), true);
		// 장운영시간 저장

		strTemp.Format("%s|%s|%s|%s", m_strBeginTime, m_strBeginTimeEnd, m_strEndTime, m_strEndTimeEnd);

		m_pMainWnd->SendMessage(WM_MANAGE, MK_MARKETTIME, (LPARAM)(char*)strTemp.operator LPCTSTR());
	}

	const int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL)
		return;

	if (!m_bContinue || (m_bContinue && !m_bSecond))
	{
		if (m_seldrop < 0)
		{
			m_grid->Clear();
			m_grid->MessageToGrid(WM_VSCROLL, SB_TOP);
		}
	}

	CString str950, str951; // 2012.06.19 KSJ 배분, 임의
	CString string, stringx, entry, strField;
	CString bongdata = _T("");
	const BOOL bBold = ((CGroupWnd *)m_pGroupWnd)->GetCurrBold();

	string = CString(&datB[gridHoL], datL - gridHoL);
	m_arExist.RemoveAll();
	const int kind = CAST_TREEID(m_kind)->kind;
	LPARAM lParam = 0;

	int nStart{}, nEnd{};

	nStart = 0;
	nEnd = m_gridHdrX.GetSize();

	if (m_bContinue)
	{
		if (m_bSecond)
		{
			// 			m_bSecond = FALSE;
			// 			m_bContinue = FALSE;

			nStart = GRIDMAXNUM;
			nEnd = m_gridHdrX.GetSize();
		}
		else
		{
			m_mapCodeABR.RemoveAll();
			nStart = 0;
			nEnd = GRIDMAXNUM;
		}
	}

	if (kind == xISSUE)
	{
		lParam = MAKELPARAM(0, 1);
	}

	CString strCode, strMargin, strTodayVolumn, strData;
	CString tempData;
	//, recommand1, recommand2, recommand3, recommand4, recommand5;

	m_grid->beginDrawHolding();
	if (!string.IsEmpty())
	{
		for (size_t ii = 0; ii < _vInters.size(); ii++)
		{
			tempData = "";
		//	recommand1 = "";
		//	recommand2 = "";
		//	recommand3 = "";
		//	recommand4 = "";
		//	recommand5 = "";

			strTodayVolumn = "";
			stringx = IH::Parser(string, PNEW);
			strData = stringx;

			if (!stringx.IsEmpty())
			{
				CString tempOnebyte = stringx.Left(1);
				tempOnebyte.TrimLeft();

				const int iHeadersymbol = 0; //그리드 헤더심볼 값
// CString sss = stringx;
// sss.Replace("\t", "/");
// AxStd::_Msg("receive column [%s]", sss);


				for (int jj = nStart; jj < nEnd; jj++)
				{
					entry = IH::Parser(stringx, PTAB);
					strField = entry;

					strField.TrimRight();
					strField.TrimLeft();

					if (strField == "0" || strField == "-0" || strField == "0.00" || strField == "+0" || strField == "+0.00" || strField == "-0.00" || strField == ".00") // 2012.07.17 옵션일때의 경우 추가
					{
						entry = " ";
					}

					if (jj == colCODE && !entry.IsEmpty())
					{
						CString sztmp1, sztmp2;
						sztmp1 = m_grid->GetItemText(ii + 1, 0);

						sztmp2.Format("%d", ii);
						m_arExist.SetData(sztmp1, sztmp2);
						if (m_seldrop < 0)
							m_grid->SetItemData(ii + 1, colSIG, lParam); // KSJ colSIG

						strCode = entry;

						// 2012.04.03 KSJ 처음 현재가와 데이터들을 저장.
						//m_mapCurValue.SetAt(strCode, strData);
					}

					const _gridHdr xgridHdr = m_gridHdrX.GetAt(jj);
					auto& pinters = _vInters.at(ii);

					if (jj == colNAME)
					{
						UINT attr{};

						if (pinters.get()->gubn == 'm' || pinters.get()->gubn == ROW_COMMENT)  //test mod 'm'
						{
							entry.Format("%s", pinters.get()->name);

							//책갈피 형태로 변경
							attr = m_grid->GetItemAttr(ii + 1, colNAME) | GVAT_MARKER;
							m_grid->SetItemAttr(ii + 1, colNAME, attr);
							m_grid->SetItemText(ii + 1, colCODE, pinters.get()->code);
						}
						else
						{
							attr = m_grid->GetItemAttr(ii + 1, colNAME) & ~GVAT_MARKER;
							m_grid->SetItemAttr(ii + 1, colNAME, attr);
						}

						strMargin = m_grid->GetItemText(ii + 1, colMAR);

						strMargin += "|" + m_grid->GetItemText(ii + 1, colMAR1);

						m_mapMargin.SetAt(strMargin, strMargin);
						m_grid->SetItemData(ii + 1, colNAME, (long)(LPCTSTR)strMargin);
					}
					else if (jj == colMAR)
					{
						entry.TrimLeft();
					}
					else if (!entry.IsEmpty() && jj == colRATE && (xgridHdr.attr & GVAT_HIDDEN))
					{
						int nType = 0;
						switch (entry[0])
						{
						case '1':
							nType = 1;
							break;
						case '+':
						case '2':
							nType = 2;
							break;
						case '4':
							nType = 4;
							break;
						case '-':
						case '5':
							nType = 5;
							break;
						default:
							nType = 3;
						}

						m_grid->SetItemData(ii + 1, jj, nType);
					}
					else if (jj == colCODE)
					{
						if (entry.GetLength() == 6)
						{
							switch (m_pView->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)entry))
							{
							case kospiType:
								m_grid->SetItemData(ii + 1, jj, 1);
								break;
							case kosdaqType:
								m_grid->SetItemData(ii + 1, jj, 0);
								break;
							default:
								m_grid->SetItemData(ii + 1, jj, 2);
							}
						}
						else
							m_grid->SetItemData(ii + 1, jj, 2);
					}

					if (!(xgridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
					{
						if ((strcmp("2029", xgridHdr.symbol) == 0) ||
						    (strcmp("2030", xgridHdr.symbol) == 0) ||
						    (strcmp("2031", xgridHdr.symbol) == 0))
						{
							if ((1 == xgridHdr.needs) || (3 == xgridHdr.needs))
							{
								CString strVal = entry;
								CString strDiff = strVal;

								if (strVal[0] == '+' || strVal[0] == '-')
								{
									strDiff = strVal.Mid(1);
								}

								double dDiffOpen = 0.0;
								double dPClose{}, dVal{};
								dVal = atof(strDiff);					 // 2017.01.25 KSJ GetBuffer(0) 삭제
								dPClose = atof((m_grid->GetItemText(ii + 1, colPCURR))); // 2017.01.25 KSJ GetBuffer(0) 삭제

								if (dVal != 0 && dPClose != 0)
								{
									dDiffOpen = (dVal - dPClose) / dPClose * 100;

									if (1 == xgridHdr.needs)
									{
										if (0 > dDiffOpen)
										{
											entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
										}
										else
										{
											entry.Format("%s( %0.2f%c)", strVal, dDiffOpen, P_PER);
										}
									}
									else if (3 == xgridHdr.needs)
									{
										if (0 > dDiffOpen)
										{
											entry.Format("%0.2f%c", dDiffOpen, P_PER);
										}
										else if (0 == dDiffOpen)
										{
											entry.Format(" %0.2f%c", dDiffOpen, P_PER);
										}
										else
										{
											entry.Format(" +%0.2f%c", dDiffOpen, P_PER);
										}
									}
								}
							}
						}
					}
					// END ADD

					// 2013.08.23 KSJ 스프레드종목이면 보합+, 하락+, 상승- 등 색과 기호를 표시해줘야한다.
					if (jj == colCURR)
					{
						if (strCode.GetLength() == 8 && (strCode.GetAt(0) == '4' || strCode.GetAt(0) == 'D')) //파생상품 코드개편
						{
							if (entry.GetAt(0) == ' ')	 //처음이 빈칸이면 보합에 -, + 표시가 있는 것이다.
								entry.Replace(" ", "0"); //그때는 '　' ㄱ 한자 1번 을 넣어준다. 스페이스 아님..
						}
					}
					// 2013.08.23 KSJ END
					// khs

					CString strsym, sEntry, sLog, sCode;
					strsym = (CString)xgridHdr.symbol;
					if (strsym == "5401" || strsym == "5402" || strsym == "5403" || strsym == "5404" || strsym == "5405")
					{
						strCurr = m_grid->GetItemText(ii + 1, colCURR);
						strCurr.Replace("+", "");
						strCurr.Replace("-", "");

						CMapStringToString *m_symmap{};
						sCode.Format("%s", strCode);
						sCode.TrimRight();
						sCode.TrimLeft();

						if (!sCode.IsEmpty() && sCode.GetLength() > 0)
						{
							entry.TrimLeft();
							entry.TrimRight();
							if (entry.IsEmpty() || entry.GetLength() == 0)
								entry = "0";

							if (m_mapCodeABR.Lookup(sCode, (CObject *&)m_symmap))
							{
								m_symmap->SetAt(strsym, entry);
								m_mapCodeABR.SetAt(sCode, (CObject *&)m_symmap);

								sLog.Format("receive1 ABRtest strsym=%s sCode=%s  entry=%s size=%d \n", strsym, sCode, entry, m_symmap->GetCount());
							}
							else
							{

								m_symmap = new CMapStringToString();
								m_symmap->SetAt(strsym, entry);
								m_symmap->SetAt("23", strCurr);
								m_mapCodeABR.SetAt(sCode, (CObject *&)m_symmap);
								sLog.Format("receive2 ABRtest sCode=%s  entry=%s size=%d \n", sCode, entry, m_symmap->GetCount());
							}

							CString strdata;
							const double dData = abs(atoi(strCurr)) - atof(entry);
							if (entry == "0")
								entry == "0";
							else
								entry.Format("%.2lf", dData / atof(entry) * 100); //변화량 / 조회값 * 100

							if (atof(entry) > 0)
								entry = "+" + entry;
							else if (atof(entry) == 0)
								entry = "";
						}
					}

					// khs
					m_grid->SetItemText(ii + 1, jj, entry);

					//전일종가를 저장(2320심볼)
					CString tempYvol;

					if (jj == colPCURR)
					{
						tempYvol = entry;
						tempYvol.Remove('+');
						tempYvol.Remove('-');

					}

					if (jj == colVOL)
					{
						tempYvol = entry;
						tempYvol.Remove('+');
						tempYvol.Remove('-');
						strTodayVolumn = tempYvol;
					}

					if (m_yDayVolField >= 0)
					{
						if (m_yDayVolField == jj)
						{
							entry.Remove('+'), entry.Remove('-');
							m_grid->SetItemText(ii + 1, jj, entry);
						}
					}

					if (m_bongField >= 0)
					{
						switch (jj)
						{
						case colHIGH:
							bongdata.Format("%s%c", entry, P_TAB);
							break;
						case colLOW:
							bongdata += entry;
							break;
						}

						if (m_bongField == jj)
						{
							bongdata.Remove('+') /*, bongdata.Remove('-')*/;
							m_grid->SetItemText(ii + 1, jj, bongdata);
						}
					}

					if (jj == colCURR)
					{
						//const int iHeadersymbol = atoi(xgridHdr.symbol);

						CString tempStr;

						if (strCode == "001")
						{
							m_dKospi = atof(entry);
							tempStr.Format("%.2f", m_dKospi);
							m_dKospi = atof(tempStr);
						}
						else if (strCode == "201")
						{
							m_dKosdaq = atof(entry);
							tempStr.Format("%.2f", m_dKosdaq);
							m_dKosdaq = atof(tempStr);
						}
					}

				}

				tempData = IH::Parser(stringx, PTAB);
				tempData.TrimRight();
				tempData.TrimLeft();

				//전일거래량을 저장(2321심볼)////////////////////////////////////////////////
				_mapiYDayVol.emplace( std::make_pair(strCode.GetString(), tempData) );
				/////////////////////////////////////////////////////////////////////////
				// 2012.06.20 KSJ 배분, 임의 추가
				str950 = IH::Parser(stringx, PTAB);
				str950.TrimRight();
				str950.TrimLeft();

				str951 = IH::Parser(stringx, PTAB);
				str951.TrimRight();
				str951.TrimLeft();
				// KSJ
				double dval1{}, dval2{}, dval3{}, creditPrc{};
				int iDval = 0;

				CString str;
				dval1 = dval2 = dval3 = creditPrc = 0.0;
				CString futurnGubn, mCode;
				int sizeCode{};

				//추천일자
				if (m_recomDataField > 0)
				{
					CString rDate, fDate;

					rDate = m_grid->GetItemText(ii + 1, m_recomDataField);
					rDate.TrimLeft();
					rDate.TrimRight();

					rDate.Remove('/');

					if (!rDate.IsEmpty())
					{
						fDate.Format("%s/%s/%s", rDate.Left(4), rDate.Mid(4, 2), rDate.Mid(6, 2));
						//						fDate.Format("%04s %02s% 02s", rDate.Left(4), rDate.Mid(4, 2), rDate.Mid(6,2));
						m_grid->SetItemText(ii + 1, m_recomDataField, fDate);
					}
				}

				if (m_epbDataField > 0)
				{
					CString rDate, fDate;

					// 2016.04.15 KSJ epb 추천일자 추가
					if (m_arEpbDate.GetSize() > 0 && m_arEpbDate.GetSize() > (int)ii)
					{
						rDate = m_arEpbDate.GetAt(ii);
						fDate.Format("%s/%s/%s", rDate.Left(4), rDate.Mid(4, 2), rDate.Mid(6, 2));

						m_grid->SetItemText(ii + 1, m_epbDataField, fDate);
					}
				}

				for (int jj = nStart; jj < nEnd; jj++)
				{
					const _gridHdr xgridHdr = m_gridHdrX.GetAt(jj);
					auto& pinters = _vInters.at(ii);

					mCode = pinters.get()->code; 
					mCode.TrimRight();
					mCode.TrimLeft();
					sizeCode = strlen(mCode);

					if (!m_posField)
						continue; //매입단가 등 특정필드가 있을경우 아래로 넘어감

					if (xgridHdr.needs != 9)
						continue;

					switch (xgridHdr.symbol[3])
					{
					case '1': // 보유수량
						entry = pinters.get()->xnum;
						break;
					case '2': // 매입단가
						dval1 = IH::TOfabs(pinters.get()->xprc);

						if (sizeCode == 6 || sizeCode == 9)
						{
							iDval = gsl::narrow_cast<int>(Round(dval1));
							entry.Format("%d", iDval);
						}
						else if (sizeCode != 0) // 2012.03.22 KSJ 코드길이가 0이면 세팅안해줌.
						{
							entry.Format("%.2f", dval1);
						}

						break;
					case '3': // 평가손익
						dval1 = IH::TOfabs(pinters.get()->xnum);
						dval2 = IH::TOfabs(pinters.get()->xprc);
						dval3 = IH::TOfabs(m_grid->GetItemText(ii + 1, colCURR));

						if (dval1 <= 0 || dval2 <= 0)
							continue;

						if (sizeCode == 6 || sizeCode == 9)
						{
							// 2016.04.12 KSJ 현재가보다 항상 뒤에 있음.
							entry = CalcuPyungaSonik(pinters.get(), m_grid->GetItemText(ii + 1, colCURR));
						}
						else
						{
							futurnGubn = CString(pinters.get()->futureGubn, sizeof(pinters.get()->futureGubn));
							entry = CalFutureEvalPrice(pinters.get(), strCode, futurnGubn, dval3, dval2, dval1);
							//							entry = setPyungaSonik(pinters);
						}
						break;
					case '4': // 수익율
						dval1 = IH::TOfabs(pinters->xnum);
						dval2 = IH::TOfabs(pinters->xprc);
						dval3 = IH::TOfabs(m_grid->GetItemText(ii + 1, colCURR));

						if (dval1 <= 0 || dval2 <= 0)
							continue;

						if (sizeCode == 6 || sizeCode == 9)
						{
							// 2016.04.12 KSJ 현재가보다 항상 뒤에 있음.
							entry = CalcuSuik(pinters.get(), m_grid->GetItemText(ii + 1, colCURR));
						}
						else
						{
							futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
							entry = CalFutureEvalRate(pinters.get(), strCode, futurnGubn, dval3, dval2, dval1);
							//	entry = setsuikRate(pinters);
						}

						break;
					default:
						continue;
					}
					m_grid->SetItemText(ii + 1, jj, entry);
				}

				//	strCode = "";
			}

			entry = m_grid->GetItemText(ii + 1, colINFO); // 종목특이사항
			CString strName = m_grid->GetItemText(ii + 1, colNAME);

			//현재는 종목앞에 기호에 따라 이미지를 보여주고 있다
			//이를 시장정보종목에만 이미지 보여주도록 변경
			if (!strName.IsEmpty())
			{
				// 2012.06.20 KSJ 배분, 임의 추가
				//배분= 11 매수상한,12 매수하한,13 매도상한,14 매도하한
				//임의= 12:시초동시 임의종료 지정, 14:마감동시 임의종료 지정 , 16:시간외단일가 임의종료 지정
				bool bCheck = true; // 2012.11.02 KSJ 임의배분 > 단기과열종목 > 시장경보 순으로

				if (!str950.IsEmpty() /*&& str950.GetAt(0) != '0'*/)
				{
					if (str950.GetAt(0) == '1')
					{
						m_grid->SetItemData(ii + 1, colINFO, 12);
						bCheck = false;
					}
					else
					{
						m_grid->SetItemData(ii + 1, colINFO, 0);
						bCheck = true;
					}
				}
				else if (!str951.IsEmpty() /*&& str951.GetAt(0) != '0'*/)
				{
					if (str951 == "12" || str951 == "14" || str951 == "16")
					{
						m_grid->SetItemData(ii + 1, colINFO, 13);
						bCheck = false;
					}
					else
					{
						m_grid->SetItemData(ii + 1, colINFO, 0);
						bCheck = true;
					}
				}
				// KSJ
				if (bCheck)
				{
					SetColInfo(strName, ii + 1, entry);
				}
			}
		}
	}

	m_grid->endDrawHolding();

	if (m_ccField)
		calcInClient();
	const LONG ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
	hawkeyeAction((ret) ? true : false);

	m_grid->getmemoCheck();
	if (m_seldrop >= 0)
		m_grid->SetSelectRow(m_seldrop), m_seldrop = -1;

	if (nullptr != m_pMsgWnd)
	{
		delete m_pMsgWnd;
		m_pMsgWnd = nullptr;
	}

	if (m_bContinue)
	{
		if (!m_bSecond)
		{
			m_bSecond = TRUE;
			sendTransactionTR(-1, GRIDMAXNUM, m_pGridArray.GetSize());
		}
		else
		{
			m_bContinue = FALSE;
			m_bSecond = FALSE;
			m_bSending = false;
		}
	}
	else
		m_bSending = false;
}

void CGridWnd::getBase(CString m_code)
{
	m_code.TrimLeft();
	m_code.TrimRight();

	if (m_code.IsEmpty())
		return;
	const int nkind = getJKind(m_code);

	m_dBase = 100000;

	switch (nkind)
	{
	case JK_JCALLOPT:
	case JK_JPUTOPT:
		m_dBase = 1;
		break;
	case JK_SFUTURE:
	case JK_SFSPREAD:
		m_dBase = 10;
		break;
	case JK_FUTURE:
	case JK_SPREAD:
		m_dBase = 500000;
		break;
	case JK_STAR:
		m_dBase = 10000;
		break;
	case JK_KTBFUTURE:
		m_dBase = 1000000;
		break;
	case JK_USDFUTURE:
		m_dBase = 10000;
		break;
	case JK_GOLDFUTURE:
		m_dBase = 1000;
		break;
	}
}

CString CGridWnd::getFormatData(CString src)
{
	CString sRtn = src;
	if (!sRtn.IsEmpty() && sRtn.FindOneOf("+-") == 0)
		sRtn = sRtn.Mid(1);
	sRtn.TrimLeft();
	return sRtn;
}

int CGridWnd::getJKind(CString m_code)
{
	if (m_code.GetLength() != 8)
		return -1;
	const TCHAR ch1 = m_code.GetAt(0);
	const TCHAR ch2 = m_code.GetAt(1);
	const TCHAR ch3 = m_code.GetAt(2);

	switch (ch1)
	{
	case '1': // future
	case 'A': //파생상품 코드개편
	{
		if (ch2 == '0')
		{
			if (ch3 == '1')
				return JK_FUTURE;
			else
				return JK_STAR;
		}
		else if (ch2 == '6')
			return JK_KTBFUTURE;
		else if (ch2 == '7')
			return JK_USDFUTURE;
		else if (ch2 == '8')
			return JK_GOLDFUTURE;
		else
			return JK_SFUTURE;
	}
	case '4': // future spread
	case 'D': //파생상품 코드개편
		if (ch2 == '0')
			return JK_SPREAD;
		else
			return JK_SFSPREAD;
	case '2': // call option
	case 'B': //파생상품 코드개편
		if (m_code.GetAt(1) == '0')
			return JK_CALLOPT;
		else
			return JK_JCALLOPT;
		break;
	case '3': // put option
	case 'C': //파생상품 코드개편
		if (m_code.GetAt(1) == '0')
			return JK_PUTOPT;
		else
			return JK_JPUTOPT;
		break;
	}
	return -1;
}

double CGridWnd::Round(double data) //(반올림)
{
	return (int)(data + 0.5);
}

CString CGridWnd::CalJusikEvalPrice(struct _intersx *pinters, double m_curr, double m_maip, double m_remian)
{
	double result{};
	CString entry, m_rprice;

	const double rPrice = m_curr * m_remian;
	const double mPrice = pinters->maeipPrc;

	result = rPrice - mPrice;

	if (result > 0)
	{
		entry.Format("%+.0f", result);
	}
	else if (result < 0)
	{
		entry.Format("-%.0f", result);
	}

	return entry;
}

CString CGridWnd::CalJusikEvalRate(struct _intersx *pinters, double m_curr, double m_maip, double m_remian)
{
	CString entry;
	double result{};
	const double creditPrc = pinters->creditPrc;
	const double resultVal = 0;

	const double rPrice = m_curr * m_remian;
	const double mPrice = pinters->maeipPrc;

	result = rPrice - mPrice;

	entry.Format("%+.2f", (result / (mPrice - creditPrc)) * 100);

	return entry;
}

CString CGridWnd::setPyungaSonik(struct _intersx *pinters)
{
	const double pyunggaPrc = pinters->creditPrc;

	CString result;

	if (pyunggaPrc > 0)
		result.Format("+%.0f", pyunggaPrc);
	else if (pyunggaPrc < 0)
		result.Format("-%.0f", pyunggaPrc);

	return result;
}

CString CGridWnd::setsuikRate(struct _intersx *pinters)
{
	const double sonikRate = pinters->maeipPrc;

	CString result;

	result.Format("%.2f", sonikRate);

	if (sonikRate < 0)
		result = "" + result;
	else if (sonikRate > 0)
		result = "+" + result;

	return result;
}

void CGridWnd::parsingOubsOne(char *datB, int datL, int update)
{
	if (m_staticUpdate < 0)
		return;
	const int gridHoL = sizeof(struct _gridHo);
	if (datL < gridHoL)
		return;

	CString str950, str951;

	CString string, stringx, entry, strData, strField;
	CString bongdata = _T("");
	string = CString(&datB[gridHoL], datL - gridHoL);
	const BOOL bBold = ((CGroupWnd *)m_pGroupWnd)->GetCurrBold();

	CString strCode, strMargin;
	CString tempData, recommand1, recommand2, recommand3, recommand4, recommand5;

	int nStart{}, nEnd{};

	nStart = 0;
	nEnd = m_gridHdrX.GetSize();
	if (m_bContinue)
	{
		if (m_bSecond)
		{
			m_bSecond = FALSE;
			m_bContinue = FALSE;

			nStart = GRIDMAXNUM;
			nEnd = m_gridHdrX.GetSize();
		}
		else
		{
			nStart = 0;
			nEnd = GRIDMAXNUM;
		}
	}

	for (size_t ii = 0; ii < _vInters.size(); ii++)
	{
		if (string.IsEmpty())
			break;

		tempData = "";
		recommand1 = "";
		recommand2 = "";
		recommand3 = "";
		recommand4 = "";
		recommand5 = "";

		stringx = IH::Parser(string, PNEW);
		strData = stringx;
		if ((int)ii < m_staticUpdate)
			continue;

		// 2013.07.04 KSJ 필드가 GRIDMAXNUM(27)개를 넘으면 2번 나눠서 조회한다.
		//그렇기때문에 첫번재 데이터에서 27번째까지만 뽑아서 뒷데이터랑 붙이면
		//완전한데이터가 된다.
		/////////////////////////////////////////////////////////
	
		// 2013.07.04 END

		for (int jj = nStart; jj < nEnd; jj++)
		{
			if (stringx.IsEmpty())
				break;

			entry = IH::Parser(stringx, PTAB);

			strField = entry;

			strField.TrimLeft();
			strField.TrimRight();

			if (strField == "0" || strField == "-0" || strField == "0.00" || strField == "+0" || strField == " 0" || strField == ".00" || strField == "+0.00" || strField == "-0.00") // 2012.07.17 KSJ 옵션일때의 경우 추가
			{
				entry = " ";
			}

			// ADD PSH 20070912
			const _gridHdr xgridHdr = m_gridHdrX.GetAt(jj);
			auto& pinters = _vInters.at(ii);

			if (jj == colCODE)
			{
				strCode = entry;

				// 2012.08.31 추가한 데이터도 맵에 저장함
				//m_mapCurValue.SetAt(strCode, strData);

				if (entry.GetLength() == 6)
				{
					// 2012.06.01 KSJ 색상설정 부분
					switch (m_pView->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)entry))
					{
					case kospiType:
						m_grid->SetItemData(ii + 1, jj, 1);
						break;
					case kosdaqType:
						m_grid->SetItemData(ii + 1, jj, 0);
						break;
					default:
						m_grid->SetItemData(ii + 1, jj, 2); // 2012.06.01 KSJ 둘다아닐때 구분하기위해
						break;
					}
				}
				else
				{
					m_grid->SetItemData(ii + 1, jj, 2);
				}
				// KSJ
			}
			else if (jj == colNAME)
			{
				if (pinters.get()->gubn == 'm' || pinters.get()->gubn == ROW_COMMENT)  //test mod 'm'
				{
					entry.Format("%s", pinters.get()->name);
				}

				strMargin = m_grid->GetItemText(ii + 1, colMAR);
				m_mapMargin.SetAt(strCode, strMargin);
				m_grid->SetItemData(ii + 1, colNAME, atoi(strMargin)); // 2017.01.25 KSJ GetBuffer(0) 삭제
			}
			else if (!entry.IsEmpty() && jj == colRATE && (xgridHdr.attr & GVAT_HIDDEN))
			{
				int nType = 0;
				switch (entry[0])
				{
				case '1':
					nType = 1;
					break;
				case '+':
				case '2':
					nType = 2;
					break;
				case '4':
					nType = 4;
					break;
				case '-':
				case '5':
					nType = 5;
					break;
				default:
					nType = 3;
				}
				m_grid->SetItemData(ii + 1, jj, nType);
			}

			CString strsym, sEntry, sLog, sCode, strCurr;
			strsym = (CString)xgridHdr.symbol;
			const int xrow = ii + 1;
			if (strsym == "5401" || strsym == "5402" || strsym == "5403" || strsym == "5404" || strsym == "5405")
			{
				strCode = m_grid->GetItemText(xrow, colCODE);
				strCurr = m_grid->GetItemText(xrow, colCURR);
				strCurr.Replace("+", "");
				strCurr.Replace("-", "");

				CMapStringToString *m_symmap{};
				sCode.Format("%s", strCode);
				sCode.TrimRight();
				sCode.TrimLeft();

				if (!sCode.IsEmpty() && sCode.GetLength() > 0)
				{
					entry.TrimLeft();
					entry.TrimRight();
					if (entry.IsEmpty() || entry.GetLength() == 0)
						entry = "0";

					if (m_mapCodeABR.Lookup(sCode, (CObject *&)m_symmap))
					{
						m_symmap->SetAt(strsym, entry);
						m_mapCodeABR.SetAt(sCode, (CObject *&)m_symmap);
					}
					else
					{

						m_symmap = new CMapStringToString();
						m_symmap->SetAt(strsym, entry);
						m_symmap->SetAt("23", strCurr);
						m_mapCodeABR.SetAt(sCode, (CObject *&)m_symmap);
					}

					CString strdata;
					const double dData = abs(atoi(strCurr)) - atof(entry);
					if (entry == "0")
						entry == "0";
					else
						entry.Format("%.2lf", dData / atof(entry) * 100); //변화량 / 조회값 * 100

					if (atof(entry) > 0)
						entry = "+" + entry;
					else if (atof(entry) == 0)
						entry = "";
				}
			}

			if (!(xgridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
			{
				if ((strcmp("2029", xgridHdr.symbol) == 0) ||
				    (strcmp("2030", xgridHdr.symbol) == 0) ||
				    (strcmp("2031", xgridHdr.symbol) == 0))
				{
					if ((1 == xgridHdr.needs) || (3 == xgridHdr.needs))
					{
						CString strVal = entry;
						CString strDiff = strVal.Mid(1);
						double dDiffOpen{}, dPClose{}, dVal{};
						dVal = atof(strDiff);					 // 2017.01.25 KSJ GetBuffer(0) 삭제
						dPClose = atof((m_grid->GetItemText(ii + 1, colPCURR))); // 2017.01.25 KSJ GetBuffer(0) 삭제

						dDiffOpen = (dVal - dPClose) / dPClose * 100;

						if (1 == xgridHdr.needs)
						{
							if (0 > dDiffOpen)
							{
								entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
							}
							else
							{
								entry.Format("%s( %0.2f%c)", strVal, dDiffOpen, P_PER);
							}
						}
						else if (3 == xgridHdr.needs)
						{
							if (0 > dDiffOpen)
							{
								entry.Format("%0.2f%c", dDiffOpen, P_PER);
							}
							else if (0 == dDiffOpen)
							{
								entry.Format(" %0.2f%c", dDiffOpen, P_PER);
							}
							else
							{
								entry.Format(" +%0.2f%c", dDiffOpen, P_PER);
							}
						}
					}
				}
			}
			// END ADD

			m_grid->SetItemText(ii + 1, jj, entry);

			if (m_bongField >= 0)
			{
				switch (jj)
				{
				case colHIGH:
					bongdata.Format("%s%c", entry, P_TAB);
					break;
				case colLOW:
					bongdata += entry;
					break;
				default:
					break;
				}

				if (m_bongField == jj)
				{
					bongdata.Remove('+'), bongdata.Remove('-');
					m_grid->SetItemText(ii + 1, jj, bongdata);
				}
			}

			if (m_recomJisuGubnField >= 0)
			{
				if (m_recomJisuGubnField == jj)
				{
					entry.TrimRight();
					entry.TrimLeft();
				}
			}

			if (m_beforeVolField >= 0)
			{
				if (m_beforeVolField == jj)
				{
					entry.TrimRight();
					entry.TrimLeft();
				}
			}

			if (m_newCurrField >= 0)
			{
				if (m_newCurrField == jj)
				{
					entry.TrimRight();
					entry.TrimLeft();
					entry.Remove('+');
					entry.Remove('-');
					//전일거래량을 저장(2321심볼)
					_mapiYDayVol.emplace(std::make_pair(strCode.GetString(), entry));
				}
			}

			// if (m_recomJisuGubnField >= 0)
			// {
			// 	if (m_recomJisuGubnField == jj)
			// 	{
			// 		if (((CGroupWnd *)m_pGroupWnd)->getRecommandCursel(m_nIndex) < 0)
			// 		{
			// 			// entry.TrimRight();
			// 			// entry.TrimLeft();
			// 		}
			// 	}
			// }

			//보유수량필드가 있는 확인
			if (!m_posField)
				continue;

			if (xgridHdr.needs != 9)
				continue;

			CString mCode;
			int sizeCode;
			mCode = pinters->code;
			mCode.TrimRight();
			mCode.TrimLeft();
			sizeCode = strlen(mCode);

			switch (xgridHdr.symbol[3])
			{
			case '1': // 보유수량
				entry = pinters->xnum;
				break;
			case '2':		   // 매입단가
				if (sizeCode != 0) // 2212.03.22 KSJ 코드가없을때는 아무것도 넣지않는다.
					entry = pinters->xprc;
				else
					entry = "";
				break;
			case '3': // 평가손익
				entry = CalcuPyungaSonik(pinters.get(), m_grid->GetItemText(ii + 1, colCURR));
				break;
			case '4': // 수익율
				entry = CalcuSuik(pinters.get(), m_grid->GetItemText(ii + 1, colCURR));
				break;
			default:
				continue;
			}
			m_grid->SetItemText(ii + 1, jj, entry);
		}

		//전일거래량을 저장(2321심볼)
		tempData = IH::Parser(stringx, PTAB);
		tempData.TrimRight();
		tempData.TrimLeft();

		// 전일 거래량 저장...
		_mapiYDayVol.emplace( std::make_pair(strCode.GetString(), tempData) );


		// 2012.06.20 KSJ 배분, 임의 추가
		str950 = IH::Parser(stringx, PTAB);
		str950.TrimRight();
		str950.TrimLeft();

		str951 = IH::Parser(stringx, PTAB);
		str951.TrimRight();
		str951.TrimLeft();
		// KSJ

		entry = m_grid->GetItemText(ii + 1, colINFO); // 종목특이사항
		{
			CString strName = m_grid->GetItemText(ii + 1, colNAME);

			if (!strName.IsEmpty())
			{
				// 2012.06.20 KSJ 배분, 임의 추가
				//배분= 11 매수상한,12 매수하한,13 매도상한,14 매도하한
				//임의= 12:시초동시 임의종료 지정, 14:마감동시 임의종료 지정 , 16:시간외단일가 임의종료 지정
				bool bCheck = true; // 2012.11.02 KSJ 임의배분 > 단기과열종목 > 시장경보 순으로

				if (!str950.IsEmpty() /*&& str950.GetAt(0) != '0'*/)
				{
					if (str950.GetAt(0) == '1')
					{
						m_grid->SetItemData(ii + 1, colINFO, 12);
						bCheck = false;
					}
					else
					{
						m_grid->SetItemData(ii + 1, colINFO, 0);
						bCheck = true;
					}
				}
				else if (!str951.IsEmpty() /*&& str951.GetAt(0) != '0'*/)
				{
					if (str951 == "12" || str951 == "14" || str951 == "16")
					{
						m_grid->SetItemData(ii + 1, colINFO, 13);
						bCheck = false;
					}
					else
					{
						m_grid->SetItemData(ii + 1, colINFO, 0);
						bCheck = true;
					}
				}
				// KSJ
				if (bCheck)
				{
					SetColInfo(strName, ii + 1, entry);
				}
			}
		}
		// END ADD

		if (m_ccField)
			calcInClient(ii + 1);
		const LONG ret = m_pToolWnd->SendMessage(WM_MANAGE, MK_HAWKEYE);
		if (ret)
			hawkEyeColor(ii + 1);
		if (m_grid->setmemoCheck(ii + 1))
			m_grid->memoRefresh();
		break;
	}

	// ADD PSH 20070927

	stringx = IH::Parser(string, PTAB);

	if (!stringx.IsEmpty())
	{
		m_mapMargin.SetAt(strCode, stringx);
	}

	m_grid->setmemoCheck(m_updateROW + 1);
	if (m_seldrop >= 0)
		m_grid->SetSelectRow(m_seldrop), m_seldrop = -1;

	if (m_bContinue)
	{
		m_bSecond = TRUE;
		sendTransactionTR(-1, GRIDMAXNUM, m_pGridArray.GetSize());
	}
	else
	{
		m_bSending = false;
		m_updateROW = -1;
		ReIndexing();
		Invalidate(false);
	}
}


// 2012.01.19 KSJ Alertx 추가
void CGridWnd::InsertNewsx(CString code, CString szTitle)
{
	class CIndexMap *idx = nullptr;
	CString strCode, strTemp;
	strCode = code;

	if (_vInters.at(0)->code.CompareNoCase(code) == 0)
		return;

	if (!code.IsEmpty())
	{
		m_szTitle.Format(" %s", (char *)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, m_kind));
		m_szTitle += ("- " + szTitle);
		InvalidateRect(m_rcTitle, FALSE);

		if (code.GetAt(0) == '5')
			code = "Q" + code;
		else
			code = "A" + code; // 2012.09.11 A를 붙여준다.

		int nIndex = 0;
		if (m_pSearchMap.GetCount() > 0)
		{
			if (!m_pSearchMap.Lookup(code, (CObject *&)idx))
			{
				DeleteRow(MAX_LINE);
			}
			else
			{
				nIndex = idx->index[0];
				if (nIndex < 100)
				{
					DeleteRow(nIndex); // 2016.07.04 KSJ 같은종목의 뉴스이면 해당라인을 삭제
				}
			}
			// 2012.10.09 KSJ 구해온 인덱스가 실제인덱스임. 기존의 nIndex +1 을 nIndex 로 수정함.
		}
		insertRow(1);
		const auto &it = _vInters.emplace(_vInters.begin(), std::make_shared<_intersx>());
		m_grid->SetItemData(1, colSIG, MAKEWPARAM(0, 1));
		m_grid->SetItemText(1, colRCOD, code);
		m_grid->SetItemText(1, colCODE, strCode);
		it->get()->code = strCode;

		_vInters.resize(MAX_LINE);
		sendTransaction(0); // 2016.06.03 KSJ 할때 첫줄만 poop 태워야 한다.
		m_grid->ClearAllRTM();
	}
}
// 2012.01.19 KSJ Alertx 추가 끝

void CGridWnd::ReplaceSymbol(CMapStringToString &fms, CString szold, CString sznew)
{
	CString entry = _T("");
	if (fms.Lookup(szold, entry))
	{
		fms.RemoveKey(szold);
		fms.SetAt(sznew, entry);
	}
}

CString SplitString(CString &strData, CString strToken)
{
	CString sResult = strData;
	if (strToken == "")
		return sResult;
	const int iFind = strData.Find(strToken);
	if (iFind > -1)
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength());
	}
	return sResult;
}

//void CGridWnd::SettingGridHeaderName(int index)
//{
//	const struct _symbol
//	{
//		UINT stid1;
//		char *symb1;
//		UINT stid2;
//		char *symb2;
//	} chksym[] = {
//	    {IDS_GH_CURR, "2023", IDS_GH_ANTIPRC, "2111"},
//	    {IDS_GH_DIFF, "2024", IDS_GH_DIFF, "2115"},
//	    {IDS_GH_RATE, "2033", IDS_GH_RATE, "2116"},
//	    {IDS_GH_VOL, "2027", IDS_GH_ANTIVOL, "2112"},
//	};
//
//	const int chksymC = sizeof(chksym) / sizeof(_symbol);
//
//	if (index == 1)
//	{
//		_gridHdr xgridHdr{};
//		for (int ii = 0; ii < chksymC; ii++)
//		{
//			for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
//			{
//				xgridHdr = m_gridHdrX.GetAt(jj);
//				if (atoi(xgridHdr.symbol) != atoi(chksym[ii].symb1))
//					continue;
//
//				xgridHdr.stid = chksym[ii].stid2;
//				m_gridHdrX.SetAt(jj, xgridHdr);
//			}
//		}
//		for (int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
//		{
//			xgridHdr = m_gridHdrX.GetAt(jj);
//		}
//	}
//	else
//	{
//		_gridHdr xgridHdr{};
//		for (int ii = 0; ii < chksymC; ii++)
//		{
//			for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
//			{
//				xgridHdr = m_gridHdrX.GetAt(jj);
//				if (atoi(xgridHdr.symbol) != atoi(chksym[ii].symb2))
//					continue;
//
//				xgridHdr.stid = chksym[ii].stid1;
//				m_gridHdrX.SetAt(jj, xgridHdr);
//				break;
//			}
//		}
//	}
//}


CString CGridWnd::getABRdata(const char *pcurr, CString strCode, CString strsym)
{
	int idata = 0;
	if (pcurr == nullptr)
		idata = 0;
	else
		idata = gsl::narrow_cast<int>(atof((char *)pcurr));

	CString slog;
	CString entry;
	entry = "";
	CMapStringToString *m_symmap;
	if (m_mapCodeABR.Lookup(strCode, (CObject *&)m_symmap))
	{
		CString strdata, strCurr;
		if (idata == 0)
		{
			strCurr = "0";
			m_symmap->Lookup("23", strCurr);
			idata = gsl::narrow_cast<int>(atof(strCurr));
		}

		if (m_symmap->Lookup(strsym, strdata))
		{
			if (strdata.IsEmpty() || strdata == "0")
				entry = "0";
			else
			{
				const double dData = abs(idata) - atof(strdata);
				entry.Format("%.2lf", dData / atof(strdata) * 100); //변화량 / 조회값 * 100
			}

			if (atof(entry) > 0)
				entry = "+" + entry;
			else if (atof(entry) == 0)
				entry = "";
		}
	}

	return entry;
}
// 2012.01.19 KSJ Alertx 추가 끝

void CGridWnd::SetColInfo(CString strName, int nRow, CString strEntry) // 2012.11.02 KSJ 종목특이사항 세팅
{
	if (strName.GetAt(0) == 'X')
	{
		m_grid->SetItemData(nRow, colINFO, 10);
	}
	else if ("정리" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 11);
	}
	else if ("단기" == strEntry) // 2012.10.31 KSJ 단기과열 추가
	{
		m_bGetHogaRTS = TRUE;
		m_grid->SetItemData(nRow, colINFO, 14);
	}
	else if ("투자주의" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 3);
	}
	else if ("투자경고" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 4);
	}
	else if ("위험예정" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 15);
	}
	else if ("투자위험" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 5);
	}
	else if ("급등" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 24);
	}
	else if ("투자주의환기" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 7);
	}
	else if (strName.GetAt(0) == '&')
	{
		m_grid->SetItemData(nRow, colINFO, 1);
	}
	else if ("감리" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 22);
	}
	else if (strName.GetAt(0) == '%')
	{
		m_grid->SetItemData(nRow, colINFO, 8);
	}
	else if (strName.GetAt(0) == '@')
	{
		m_grid->SetItemData(nRow, colINFO, 9);
	}
	else if ("권리" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 16);
	}
	else if ("배당" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 17);
	}
	else if ("액면" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 18);
	}
	else if ("합병" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 20);
	}
	else if ("감자" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 21);
	}
	else if ("초저유동성" == strEntry)
	{
		m_grid->SetItemData(nRow, colINFO, 23);
	}
	else // 2011.12.06 KSJ 일치하는게 없으면 0으로 세팅해준다.
	{
		m_grid->SetItemData(nRow, colINFO, 0);
	}
}

void CGridWnd::alertBMsuik()
{
	for (int ii = 1; ii < m_rowC; ii++)
	{
		//추천 BM수익률 계산
		//수익률 = 100.0 * (현재지수 - BM지수) / BM지수
		if (m_recomBMSuikrateField >= 0)
		{
			CString strBMjisu, suik;
			double dBMjisu = 0;
			double result = 0;

			strBMjisu = m_grid->GetItemText(ii, m_recomBMjisuField);
			dBMjisu = atof(strBMjisu);

			CString upjong;

			if (ii > 0)
			{
				upjong = GetRecommandInfo(ii);
			}

			m_dKospi = fabs(((CGroupWnd *)GetParent())->GetGridWndUpjn()->m_dKospi);
			m_dKosdaq = fabs(((CGroupWnd *)GetParent())->GetGridWndUpjn()->m_dKosdaq);

			if (!upjong.IsEmpty() && m_dKospi > 0 && m_dKosdaq > 0)
			{

				if (atoi(upjong) == 1)
				{
					result = ((m_dKospi - dBMjisu) / dBMjisu) * 100;
				}
				else if (atoi(upjong) == 2)
				{
					result = ((m_dKosdaq - dBMjisu) / dBMjisu) * 100;
				}

				if (result > 0)
				{
					suik.Format("+%.2f", result);
				}
				else
				{
					suik.Format("%.2f", result);
				}

				m_grid->SetItemText(ii, m_recomBMSuikrateField, suik);
			}
		}
	}
}

void CGridWnd::calcInClient()
{
	CString code;
	for (int ii = 1; ii < m_rowC; ii++)
	{
		code = m_grid->GetItemText(ii, colCODE);
		code.TrimLeft();
		code.TrimRight();
		if (code.IsEmpty())
			continue;

		calcInClient(ii);
	}
}

void CGridWnd::calcInClient(int row)
{
	CString string, stringx;
	double curr{}, open{}, high{}, low{};

	curr = IH::TOfabs(m_grid->GetItemText(row, colCURR));
	open = IH::TOfabs(m_grid->GetItemText(row, colOPEN));
	high = IH::TOfabs(m_grid->GetItemText(row, colHIGH));
	low = IH::TOfabs(m_grid->GetItemText(row, colLOW));

	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	{
		const _gridHdr xgridHdr = m_gridHdrX.GetAt(ii);

		if (xgridHdr.needs != 8)
			continue;

		string = _T("");

		switch (xgridHdr.symbol[2])
		{
		case '1': // 시가대비(원)
			if (curr <= 0. || open <= 0.)
				break;
			string.Format("%+.0f", curr - open);
			break;
		case '2': // 시가대비(%)
			if (curr <= 0. || open <= 0.)
				break;
			string.Format("%+.2f%%", (curr - open) / curr * 100);
			break;
		case '3': // 고가대비(원)
			if (curr <= 0. || high <= 0.)
				break;
			string.Format("%+.0f", curr - high);
			break;
		case '4': // 고가대비(%)
			if (curr <= 0. || high <= 0.)
				break;
			string.Format("%+.2f%%", (curr - high) / curr * 100);
			break;
		case '5': // 저가대비(원)
			if (curr <= 0. || low <= 0.)
				break;
			string.Format("%+.0f", curr - low);
			break;
		case '6': // 저가대비(%)
			if (curr <= 0. || low <= 0.)
				break;
			string.Format("%+.2f%%", (curr - low) / curr * 100);
			break;
		}
		stringx = m_grid->GetItemText(row, ii);
		if (atof(stringx) != atof(string))
		{
			m_grid->SetItemText(row, ii, string);
		}
	}
}

#include "mmsystem.h"
#pragma comment(lib, "winmm")

int CGridWnd::CheckRealTimeCode(CString code)
{
	CString slog;
	class CIndexMap *idx = nullptr;
	constexpr int count = 0;
	ZeroMemory(m_irowCode, sizeof(int) * 110);
	// 속도개선 Search 방법 변경
	if (m_pSearchMap.GetCount() > 0)
	{
		if (!m_pSearchMap.Lookup(code, (CObject *&)idx))
			return count;

		for (int ii = 0; ii < idx->idxCnt; ii++)
			m_irowCode[ii] = idx->index[ii];

		slog.Format("[IB202200][symbol] [%s] code=[%s] ii=[%d]", __FUNCTION__, code, idx->idxCnt);
		Output_DebugString(slog);

		return idx->idxCnt;
	}
	return count;
}

// 2011.12.29 KSJ
void CGridWnd::ReSetSearchMap()
{
	CString slog;
	class CIndexMap *idx = nullptr;
	const int realtimeCol = 0;
	CString string, strTemp;
	_mapSymbol.clear();
	m_grid->initRowFilter();

	int count = 0, ii = 0;
	for (count = 0, ii = 1; ii < m_grid->GetRowCount(); ii++)
	{
		string = m_grid->GetItemText(ii, realtimeCol);
		m_irowCode[count++] = ii;

		string.Trim();
		if (string.IsEmpty())
			continue;
		_mapSymbol.emplace(std::make_pair(string, 1));

		// map에 중복 데이터 체크
		if (!m_pSearchMap.Lookup(string, (CObject *&)idx))
			idx = new CIndexMap();

		if (idx->idxCnt >= maxIDX)
			continue;

		idx->index[idx->idxCnt++] = ii;
		m_pSearchMap.SetAt(string, (CObject *)idx);

		slog.Format("[IB202200][symbol] [%s] string=[%s] ii=[%d]",__FUNCTION__, string, ii);
		Output_DebugString(slog);
	}
}

void CGridWnd::ClearSearchMap()
{
	POSITION pos{};
	class CIndexMap *idx = nullptr;
	CString code = "";
	_mapSymbol.clear();
	for (pos = m_pSearchMap.GetStartPosition(); pos != nullptr;)
	{
		m_pSearchMap.GetNextAssoc(pos, code, (CObject *&)idx);
		delete idx;
		idx = nullptr;
	}
	m_pSearchMap.RemoveAll();
}

void CGridWnd::ClearInterest()
{
	_mapSymbol.clear();
	_vInters.clear();
}


// ADD PSH 20070913
void CGridWnd::CopyInter(_intersx *pCpy, _intersx *pSrc)
{
	*pCpy = *pSrc;
}

void CGridWnd::SetMarkerProp(_marker *pProp)
{
	if (nullptr != pProp)
	{
		BOOL bChanged = FALSE;

		if ((m_clrMarkerBKG != pProp->clrMarkerBKG) ||
		    (m_clrMarkerTXT != pProp->clrMarkerTXT) ||
		    (m_bShadow != pProp->bShadow))
		{
			bChanged = TRUE;
		}

		if (bChanged)
		{
			m_clrMarkerBKG = pProp->clrMarkerBKG;
			m_clrMarkerTXT = pProp->clrMarkerTXT;
			m_bShadow = pProp->bShadow;

			m_grid->SetBkMarkerColor(m_clrMarkerBKG);
			m_grid->SetMarkerColor(m_clrMarkerTXT);
			m_grid->SetMarkerShadow(m_bShadow);

			m_grid->Invalidate(false);
		}
		m_bAllApply = pProp->bAllApply;
	}
}

void CGridWnd::MarkerSetup()
{
	int nRow = 1;
	for_each(_vInters.begin(), _vInters.end(), [&](auto& pInter) {
		UINT attr = m_grid->GetItemAttr(nRow++, colNAME);
		if (pInter.get()->gubn == 'm' || pInter.get()->gubn == ROW_COMMENT)  //test mod 'm'
			attr |= GVAT_MARKER;
		else
			attr &= ~GVAT_MARKER;

		m_grid->SetItemAttr(nRow, colNAME, attr);
	});
}

void CGridWnd::RemoveAllBaseInters()
{
	_arrBaseInters.clear();
}
// END ADD

void CGridWnd::AddInters(std::shared_ptr<struct _intersx> pinters)
{
	if (pinters == nullptr)
		return;

	return _vInters.push_back(pinters);
}

void CGridWnd::RemoveInters(int nIndex)
{
	if (nIndex == -1)
		return; // 2013.08.07 KSJ -1이면 종료
	const int count = _vInters.size();
	if (count > nIndex)
	{
		auto& pinters = _vInters.at(nIndex);
		if (pinters.get()->code.GetLength())
		{
			CString code = pinters.get()->code;
			// ADD PSH 20070913	2013.06.03 KSJ m_arrBaseInters는 따로 삭제할 필요가 없다.
			if (!m_bSorting)
			{

				auto et = std::remove_if(_arrBaseInters.begin(), _arrBaseInters.end(), [&code](auto& item) {
					if (item && code.CompareNoCase(item.get()->code) == 0)
						return true;
					return false;
				});

				if (et != _arrBaseInters.end())
					_arrBaseInters.erase(et);
			}
			m_mapMargin.RemoveKey(pinters.get()->code);
		}

		_vInters.erase(_vInters.begin() + nIndex);
	}
	//insertInterest(MAX_LINE - 1);
}

void CGridWnd::SetKind(UINT kind)
{
	m_kind = kind;
	m_szTitle.Format(" %s", (char *)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, kind));

	InvalidateRect(m_rcTitle, FALSE);
	m_grid->SetSortColumn(-1);
}

CString CGridWnd::FindTitle(UINT kind)
{
	CString title = _T("");
	if (m_pTreeWnd)
		title.Format(" %s", (char *)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, kind));

	return title;
}

void CGridWnd::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1000)
	{
		const int ncnt = m_arExist.GetCount();
		if (ncnt < 0)
		{
			KillTimer(1000);
			return;
		}
		int nIndex = atoi(m_arExist.GetValue(m_nTurn));
		CString code = m_grid->GetItemText(nIndex + 1, colCODE);
		CString string;

		switch (GetCodeType(code))
		{
		case kospiCODE:
			string.Format("%s\t%s", HCOD, code);
			break;
		case futureCODE:
			string.Format("%s\t%s", FCOD, code);
			break;
		case optionCODE:
			string.Format("%s\t%s", OCOD, code);
			break;
		case foptionCODE:
			string.Format("%s\t%s", PCOD, code);
			break;
		case indexCODE:
			string.Format("%s\t%s", UCOD, code);
			break;
		case sinjuCODE:
			string.Format("%s\t%s", SINJUCOD, code);
			break;
		case elwCODE:
			string.Format("%s\t%s", ELWCOD, code);
			break;
		case 0:							  // 2012.10.11 KSJ 주식, 금리, 통화선물 추가
			if (code.GetAt(0) == '1' || code.GetAt(0) == '4' || code.GetAt(0) == 'A' || code.GetAt(0) == 'D') //주식, 금리, 통화선물//파생상품 코드개편
			{
				int nGubn = atoi(code.Mid(1, 2));
				CString strGubn = code.Mid(1, 2); // 2014.10.21 KSJ 주식선물 신규종목 연동

				if (nGubn > 10 && nGubn < 60) //주식선물
				{
					string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
					Variant(triggerCC, string);

					string.Format("%s\t%s", SFOD, code);
					break;
				}
				else if (nGubn > 60 && nGubn <= 70) //금리선물
				{
					string.Format("%s\t%s", SFOD, code);
					break;
				}
				else if (nGubn > 70 && nGubn < 80) //통화선물
				{
					string.Format("%s\t%s", SFOD, code);
					break;
				}
				else if ((strGubn >= "B0" && strGubn <= "ZZ") || (strGubn >= "0A" && strGubn <= "0Z") || (strGubn >= "1A" && strGubn <= "1Z") || (strGubn >= "2A" && strGubn <= "2Z")
					|| (strGubn >= "3A" && strGubn <= "3Z") || (strGubn >= "4A" && strGubn <= "4Z") || (strGubn >= "5A" && strGubn <= "5Z") || (strGubn >= "6A" && strGubn <= "6Z")
					|| (strGubn >= "7A" && strGubn <= "7Z") || (strGubn >= "8A" && strGubn <= "8Z") || (strGubn >= "9A" && strGubn <= "9Z")) // 2024.1031 주식선물
				{
					string.Format("%s\t%s", "ed_fcod", code); // 2013.01.02 KSJ 2300선물종합화면에도 트리거보냄
					Variant(triggerCC, string);
					string.Format("%s\t%s", SFOD, code);
					break;
				}
			}
		}

		Variant(triggerCC, string);
		m_grid->SetFocus();
		m_grid->SetSelectRow(nIndex + 1);
		m_nTurn++;
		m_nTurn = m_nTurn % ncnt;
	}
	CBaseWnd::OnTimer(nIDEvent);
}

void CGridWnd::rebuildInterest()
{
	std::copy(_vInters.begin(), _vInters.end(), std::back_inserter(_arrBaseInters));
}

void CGridWnd::Reset(bool bAll)
{
	KillTimer(1000);
	RemoveAll();
	const int nActive = (int)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ACTIVE));
	if (nActive == m_nIndex)
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET));
	m_grid->Clear();

	if (bAll)
	{
		m_szTitle = _T("");
		SetKind(0);
	}
}

//화면 번호 tab 개수 tab 종목 번호 | 종목 번호 | 종목 번호
void CGridWnd::sendtoMutiHoga()
{
	int nScnt = 0;
	CString code(_T("")), string(_T("")), data(_T("")), string_code(_T(""));
	const char chTab = P_TAB;
	const char chLf = P_NEW;

	int i = 0;
	for (i = _vInters.size() - 1; i >= 0; i--)
	{
		auto& pInters = _vInters.at(i);
		if (!pInters.get()->code.IsEmpty())
			break;
	}

	if (i < 0)
		nScnt = 0;
	else
		nScnt = i;

	int rowCount = 0;


	for (i = 0; i <= nScnt; i++)
	{
		auto& pInters = _vInters.at(i);

		if (pInters->code[0] != 'm')
		{
			if (!pInters.get()->code.IsEmpty())
			{
				string_code += pInters.get()->code;
				string_code += "|";
				rowCount++;
			}
		}
	}

	CInterDomino dlgInter(m_pMainWnd, rowCount, string_code);
	dlgInter.DoModal();
}


void CGridWnd::saveServer(const std::vector<std::shared_ptr<struct _intersx>>& vInters, int gno)
{
	CString stmp;

	if (gno < 0)
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			gno = CAST_TREEID(m_kind)->value;
		else
			return;
	}

	const int nScnt = GetRowValidcount(vInters);

	std::string buffer;
	buffer.resize(sz_updn + sz_jinfo * nScnt + 1, ' ');

	struct _updn* updn = (struct _updn*)buffer.data();
	CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
	updn->uinfo.dirt[0] = 'U';
	//updn->uinfo.cont[0] = 'G';
	updn->uinfo.cont[0] = 'N';

	CopyMemory(updn->uinfo.nblc, _T("00001"), sizeof(updn->uinfo.nblc));
	updn->uinfo.retc[0] = 'O';

	CString sNum = AxStd::FORMAT("%02d", gno);
	CString strGrouptName = GetGroupName(gno);

	CopyMemory(updn->ginfo.gnox, sNum, sizeof(updn->ginfo.gnox));
	CopyMemory(updn->ginfo.gnam, (LPCTSTR)strGrouptName, min(sizeof(updn->ginfo.gnam), strGrouptName.GetLength()));

	sNum.Format("%04d", nScnt);
	CopyMemory(updn->ginfo.jrec, sNum, sizeof(updn->ginfo.jrec));

	const gsl::span<struct _jinfo> spanInter((struct _jinfo*)(buffer.data() + sz_updn), nScnt);

	int ii = 0;
	for_each(spanInter.begin(), spanInter.end(), [&](auto& jinfo){
		const auto& pInters = vInters.at(ii++);
		const auto& item    = pInters.get();

		if (!item->code.IsEmpty())
		{
			//FillMemory(&jinfo, sz_jinfo, ' ');
			jinfo.gubn[0] = item->gubn > 0 ? item->gubn : '0';
			
			if (item->gubn == ROW_COMMENT)   //test mod 책갈피 내용 업로드
			{
				stmp.Format("m%05d", ii - 1);
				AxStd::xxCopy(jinfo.code, stmp);
				AxStd::xxCopy(jinfo.xprc, item->name);
			}
			else
			{
				AxStd::xxCopy(jinfo.gubn, &item->gubn);
				AxStd::xxCopy(jinfo.code, item->code);
				AxStd::xxCopy(jinfo.xprc, item->xprc);
				AxStd::xxCopy(jinfo.xnum, item->xnum);
				if (item->bookmark == '1')
					jinfo.gubn[0] = ROW_BOOKMARK;
				
				AxStd::xxCopy(jinfo.bookrgb, item->bookrgb); // 2015.04.03 KSJ 1이아니면 0으로 해준다.
			}		
		}
	});

	char key{};
	_trkey *trkey = (struct _trkey *)&key;
	trkey->kind = TRKEY_GRIDSAVE;
	trkey->group = m_nIndex;

	CSendData sData;
	sData.SetData(trUPDOWN, key, buffer.data(), buffer.size(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
	m_pMainWnd->SendMessage(WM_MANAGE, MK_PROCDLL);

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
		m_bEditWork = FALSE;

	CString string = "OnPortfolio\tok";
	_Wizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);
}

void CGridWnd::saveInterest(bool btmp, int gno, bool bBookMark) // 2013.05.14 KSJ 북마크지정할때는 분할모드 무시하도록.
{
	if (gno < 0)
	{
		if (CAST_TREEID(m_kind)->kind == xINTEREST)
			gno = CAST_TREEID(m_kind)->value;
		else
			return;
	}

	CWnd *pToolWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pToolWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	////분할로 보기모드일 경우
	if (nOver == MO_VISIBLE && !bBookMark)
	{
		//분할로 보여지고 있는 그룹 저장하기
		const auto [no, arr] = ((CGroupWnd*)m_pGroupWnd)->WriteFileSumtoEachGroup(m_kind);
		//서버에 올리기
		saveServer(arr, no);
		return;
	}
	saveServer(_vInters, gno);
}

int CGridWnd::GetRowValidcount(const std::vector<std::shared_ptr<struct _intersx>> &inter)
{
	const int maxRow = inter.size() - 1;
	int i = 0;
	for (i = maxRow; i >= 0; i--)
	{
		auto& pInters = inter.at(i);
		if (!pInters->code.IsEmpty())
			break;
	}
	return i + 1;
}

int CGridWnd::GetRowValidcount()
{
	const int maxRow = _vInters.size() - 1;
	int i = 0;
	for (i = maxRow; i >= 0; i--)
	{
		auto& pInters = _vInters.at(i);
		if (!pInters.get()->code.IsEmpty())
			break;
	}
	return i + 1;
}



CString CGridWnd::GetcolName(int index)
{
	return m_grid->GetItemText(index + 1, colNAME);
}

CString CGridWnd::GetGroupName(int gno)
{
	auto& map = ((CGroupWnd*)m_pGroupWnd)->_GroupName;
	auto it = map.find(gno);

	if (it != map.end())
		return it->second;

	return AxStd::FORMAT("%s%02d", "관심종목", gno);
}

// 관심종목저장 버튼 선택시
void CGridWnd::saveInterestX()
{
	CString code, bookmark, string, stringx;
	//struct _intersx *pinters{};

	CWnd *pTreeWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	CWnd *pGroupWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	const int nOver = (int)pTreeWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	// 2012.02.09 KSJ 여기서 왜 m_inters가 변하는지 모르겠다..
	//예상으로는 BaseSort에서 뭔가를 할것 같은데..
	// rebuildInterest에 안쓸것 같은거 주석처리하니까 제대로됨..
	const int scnt = GetRowValidcount();

	if (scnt == 0)
	{
		if (!m_bPlusMinus) // 2011.12.05 KSJ  그룹 추가 삭제시에는 메세지 안뜨도록
			Variant(guideCC, "선택된 그룹에 종목이 없습니다.");
		return;
	}

	if (CAST_TREEID(m_kind)->kind == xINTEREST)
	{
		//나눠보기 모드인지 판단하는 로직 추가
		const bool isEdited = ((CGroupWnd *)m_pGroupWnd)->isModified(GetKind());

		if (m_bEditWork)
		{
			// 'b' - 'G' - 'V'
			((CMainWnd*)m_pMainWnd)->setAlertx(FALSE);
			saveInterest(true);
			((CMainWnd*)m_pMainWnd)->setAlertx(TRUE);
			m_bEditWork = FALSE;
		}
		else
		{
			//나눠보기 모드인지 판단하는 로직 추가
			if (isEdited == TRUE)
			{
				((CMainWnd*)m_pMainWnd)->setAlertx(FALSE);
				saveInterest(true);
				((CMainWnd*)m_pMainWnd)->setAlertx(TRUE);
			}
			else
			{
				if (!m_bPlusMinus) // 2011.12.05 KSJ  그룹 추가 삭제시에는 메세지 안뜨도록
				{
					string = "해당 관심그룹의 변경된 내용이 없습니다";
					m_pMainWnd->SendMessage(WM_MANAGE, MK_SHOWTOOLTIP, (LPARAM)string.operator LPCTSTR());
				}
			}
		}
	}
	else
	{
		//그룹명 검색해서 내관심종목에 vol 추가하기
		stringx = makeGroupName();
		for (int ii = 0; ii < scnt; ii++)
		{
			auto& pinters = _vInters.at(ii);
			code = CString(pinters.get()->code, sizeof(pinters.get()->code)).Trim();
			if (pinters.get()->bookmark != '1')
				pinters.get()->bookmark = '0';

			string.Format("%s%c%c%c", code, P_DELI, pinters->bookmark, P_DELI);
			stringx += string;
		}

		CString tem;
		tem.Format("[%s]", stringx);

		string.Format("%s /p5 /S/d%s%c%s%c", MAP_CONFIG, "appendParentGROUP", P_TAB, stringx, P_NEW);
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_OPENSCR, typePOPUP), (LPARAM)string.operator LPCTSTR());
	}
}

CString CGridWnd::makeGroupName()
{
	bool isTrue = false;
	CString name = _T("내관심종목");

	auto& map = ((CGroupWnd*)m_pGroupWnd)->_GroupName;
	int makeGNo = 0;
	for_each(map.begin(), map.end(), [&](const auto& item){
		if (!item.second.IsEmpty())
		{
			CString gName = item.second;
			CString gnameRTrim;
			CString gnameLTrim = gName.Left(10);
			if (item.second.Left(10).CompareNoCase("내관심종목") == 0)
			{
				isTrue = true;
				if (gName.GetLength() > 11)
				{
					gnameRTrim = gName.Mid(10, 2);
				}
				else
				{
					gnameRTrim = "0";
				}

				if (!gnameLTrim.IsEmpty())
				{
					int idx = atoi(gnameRTrim);
					if (idx > makeGNo)
					{
						makeGNo = idx;
					}
				}
			}
		}
	});

	if (isTrue == true)
		makeGNo++;
	else
		makeGNo = 1;

	CString stringx;
	stringx.Format("%s%02d%c", name, makeGNo, P_DELI);
	return stringx;
}

LRESULT CGridWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_ERASEBKGND)
		return 1;
	return CBaseWnd::WindowProc(message, wParam, lParam);
}

// int counts = 0;
// CString datas[6] = {"A000660\t023\t+3333\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t111\t+2222\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t111\t-2222\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t023\t+4444\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t023\t+3334\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t111\t+2223\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t111\t-2223\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t023\t+4445\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t023\t+3335\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t111\t+2224\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n",
// 					"A000660\t111\t-2224\t047\t15870\t058\t+9560\t048\t9570\t059\t+9570\t104\t+9280\t049\t20250\t106\t498300\t090\t0\t091\t-1610\t080\t023\t+4446\t109\t-1260\t092\t+130\t070\t53820\t081\t+4070\t093\t-640\t071\t+9480\t082\t+3760\t060\t+9580\t094\t+500\t061\t28010\t072\t+9470\t083\t0\t050\t41100\t095\t0\t062\t24930\t073\t+9460\t084\t+650\t051\t+9490\t040\t102524\t096\t0\t063\t32380\t074\t+9450\t085\t+1000\t041\t5450\t052\t+9500\t097\t0\t064\t49110\t075\t+9440\t086\t0\t042\t29840\t053\t+9510\t098\t0\t065\t20650\t076\t+9430\t087\t-300\t043\t20160\t054\t+9520\t099\t-7120\t066\t27290\t077\t+9420\t088\t+100\t044\t24620\t055\t+9530\t100\t+7480\t067\t48200\t078\t+9410\t089\t0\t045\t12360\t056\t+9540\t101\t191360\t068\t72740\t079\t+9400\t046\t12140\t057\t+9550\n"};
BOOL CGridWnd::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_BT_RELOAD:
	{
		SendMessage(WM_MANAGE, MK_CLICKGROUP);
		m_pToolWnd->PostMessage(WM_MANAGE, MK_REFRESH);
	}
	break;
	case IDC_BT_DOMINO:
		sendtoMutiHoga();
		break;
	case IDC_BT_SAVE:
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)m_nIndex);
		saveInterestX();
		break;
	case IDC_BT_ERASE:
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_CLOSEGRID, m_nIndex));
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_INITGRID);

		if (((CGroupWnd *)m_pGroupWnd)->GetGroupCount() <= 1)
			m_pToolWnd->SendMessage(WM_MANAGE, MK_DISABLE_MINUS); // 2015.08.04 KSJ 그리드의 'X' 버튼을 눌렀을때
	}
	break;
	}
	return CBaseWnd::OnCommand(wParam, lParam);
}

void CGridWnd::Assign(CGridWnd *pGrid)
{
	const int nActive = (int)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ACTIVE));
	if (nActive == m_nIndex)
		m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET));
	m_grid->Clear();

	m_szTitle = _T("");
	SetKind(0);

	SetKind(pGrid->GetKind());
	m_gridHdrX.RemoveAll();

	//그리드의 row 재설정
	RemoveAll();

	pGrid->GetHearder(m_gridHdrX);
	auto& arr = pGrid->getArray(); // m_inters의 row개수를 반환
	std::copy(arr.begin(), arr.end(), std::back_inserter(_vInters));
	const int count = GetRowValidcount();

	for (int ii = 1; ii < count; ii++)
		insertRow(ii, FALSE);

	MarkerSetup();
}

void CGridWnd::GetHearder(CArray<_gridHdr, _gridHdr> &arHeader)
{
	arHeader.Copy(m_gridHdrX);
}


// 2013.01.22 KSJ 입력코드의 m_inters에서의 위치 찾기
int CGridWnd::GetInter(CString code)
{
	auto ft = std::find_if(_vInters.begin(), _vInters.end(), [code](auto& pInter) {
		if (strcmp(pInter.get()->code, (char*)code.operator LPCTSTR()) == 0)
		{
			return true;
		}
		// 2013.08.12 KSJ 신주인수권 종목 같은 경우는 조회나 실시간으로 종목코드를 받아올때 J, K, 같은 코드가 안붙어 오지만
		//실제종목파일에는 붙어있기 때문에 같지 않은경우는 문자열검색을 한번 더 검색해준다.
		else if (strstr(pInter.get()->code, (char*)code.operator LPCTSTR()) != nullptr)
		{
			return true;
		}
		return false;
	});

	if (ft != _vInters.end())
	{
		return ft - _vInters.begin();
	}
	return -1;
}

void CGridWnd::toggleAction(bool toggle)
{
	if (!GetKind())
		return;

	m_bExpect = toggle;

	const struct _symbol
	{
		UINT stid1;
		char *symb1;
		UINT stid2;
		char *symb2;
	} chksym[] = {
	    {IDS_GH_CURR, "2023", IDS_GH_ANTIPRC, "2111"},
	    {IDS_GH_DIFF, "2024", IDS_GH_DIFF, "2115"},
	    {IDS_GH_RATE, "2033", IDS_GH_RATE, "2116"},
	    {IDS_GH_VOL, "2027", IDS_GH_ANTIVOL, "2112"},
	};
	const int chksymC = sizeof(chksym) / sizeof(_symbol);

	m_grid->m_bExpect = toggle;

	// 2011.12.27 KSJ 수정본 업종일때는 심볼바꾸지 안도록 수정하기 위해

	CString strCode;

	if (toggle) // 각 Row마다 코스피, 코스닥이면 헤더를 변경하고 심볼을 변경한다.
	{
		_gridHdr xgridHdr{};

		for (int nRow = 1; nRow < m_grid->GetRowCount(); nRow++)
		{
			strCode.Empty();
			strCode = m_grid->GetItemText(nRow, colCODE);

			if (strCode.GetLength() == 6 || strCode.GetLength() == 8) //코스피, 코스닥
			{
				for (int ii = 0; ii < chksymC; ii++)
				{
					for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
					{
						xgridHdr = m_gridHdrX.GetAt(jj);

						if (atoi(m_grid->GetItemSymbol(nRow, jj)) != atoi(chksym[ii].symb1))
							continue;

						// 2013.07.08 SetItemSymbol로 하면 심볼이 바뀌지 않아서 메모리카피로 변경함.
						// m_grid->SetItemSymbol(nRow, jj, chksym[ii].symb2);

						CopyMemory(xgridHdr.symbol, chksym[ii].symb2, 4);

						xgridHdr.stid = chksym[ii].stid2;

						m_gridHdrX.SetAt(jj, xgridHdr);
						break;
					}
				}
			}
		}
	}
	else // false일때는 무조건 현재가로 바꿔준다.
	{
		_gridHdr xgridHdr{};

		for (int nRow = 1; nRow < m_grid->GetRowCount(); nRow++)
		{
			strCode.Empty();
			strCode = m_grid->GetItemText(nRow, colCODE);

			if (strCode.GetLength() == 6 || strCode.GetLength() == 8) //코스피, 코스닥
			{
				for (int ii = 0; ii < chksymC; ii++)
				{
					for (int jj = colCURR; jj < m_gridHdrX.GetSize(); jj++)
					{
						xgridHdr = m_gridHdrX.GetAt(jj);

						// 2013.07.08 chksym[ii].symb2와 비교해야함.
						if (atoi(m_grid->GetItemSymbol(nRow, jj)) != atoi(chksym[ii].symb2))
							continue;

						// 2013.07.08 SetItemSymbol로 하면 심볼이 바뀌지 않아서 메모리카피로 변경함.
						// m_grid->SetItemSymbol(nRow, jj, chksym[ii].symb1);

						CopyMemory(xgridHdr.symbol, chksym[ii].symb1, 4);
						xgridHdr.stid = chksym[ii].stid1;

						m_gridHdrX.SetAt(jj, xgridHdr);
						break;
					}
				}
			}
		}
	}

	initialGrid(false);
	sendTransaction();
}

void CGridWnd::toggleAutoAction(bool toggle)
{
	if (!GetKind())
		return;

	m_bAutoExpect = toggle;
	sendTransaction();
}

void CGridWnd::queryNewsCode()
{
	struct mid
	{
		char func[1]; /* '1':최근100종목      */
		char date[8]; /* 날짜                 */
	} mid;

	memset(&mid, ' ', sizeof(mid));
	mid.func[0] = '1';
	CString senddata = CString((char *)&mid, sizeof(mid));
	CSendData sdata;
	char key{};
	_trkey *trkey = (struct _trkey *)&key;
	trkey->kind = TRKEY_NEWS;
	trkey->group = m_nIndex;
	sdata.SetData("PIBO2022", key, (char *)senddata.operator LPCTSTR(), senddata.GetLength(), ""); // 2012.07.17 KSJ pibo --> pibf 2013.08.05 원복
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
}

void CGridWnd::parsingNewsCode(CRecvData *data)
{
	struct grid
	{
		char cod2[12];
	};

	struct mod
	{
		char nrec[4];
		struct grid grid[1];
	} * mod{};

	char *buf = data->data;
	mod = (struct mod *)buf;
	CString nrec, code;
	int ncnt = 0;
	const int nsize = sizeof(struct grid);
	nrec.Format("%.*s", sizeof(mod->nrec), mod->nrec);
	ncnt = atoi(nrec);

	SetKind(MAKE_TREEID(xISSUE));

	for (int ii = 0; ii < ncnt; ii++)
	{
		code.Format("%.*s", sizeof(mod->grid[ii].cod2), mod->grid[ii].cod2);
		code.TrimRight();
		if (!code.IsEmpty())
		{
			if (code.GetAt(0) == 'A' || code.GetAt(0) == 'Q') // 2016.07.04 KSJ 이슈종목일때 ETN뉴스도 표시
				code = code.Mid(1);
			_vInters.at(ii).get()->code = code;
		}
	}
	sendTransaction();
}

// 2012.01.19 KSJ Alertx 추가
void CGridWnd::parsingNewsx()
{
	class CIndexMap *idx = nullptr;
	CString code = _newscode;

	const int type = m_pToolWnd->SendMessage(WM_MANAGE, MK_MARKET);
	// 시장구분자   KRX :1  NXT :2  TOTAL : 3      NXT / KRX : 4 
	if (!code.IsEmpty())
	{
		if (code.GetAt(0) == '5')
			code = "Q" + code;
		else {
			switch(type)
			{
			case 2:
				code = "N.A" + code; 
				break;
			case 3:
				code = "M.A" + code; 
				break;
			case 1:
			default:
				code = "A" + code; 
				break;
			}
		}
			
		int nIndex = 0;
		if (m_pSearchMap.GetCount() > 0)
		{
			if (!m_pSearchMap.Lookup(code, (CObject *&)idx))
				return;
			// 2012.10.09 KSJ 구해온 인덱스가 실제인덱스임. 기존의 nIndex +1 을 nIndex 로 수정함.
			for (int ii = 0; ii < idx->idxCnt; ii++)
			{
				nIndex = idx->index[ii];
				const LPARAM lParam = m_grid->GetItemData(nIndex, colSIG);
				const WORD low = LOWORD(lParam);
				constexpr WORD high = 1;
				m_grid->SetItemData(nIndex, colSIG, MAKELPARAM(low, high)); // KSJ colSIG

			}
			
			if (m_bWaveApply)
				PlayWave();
		}
	}
}

// 2012.01.19 KSJ Alertx 추가 끝
CString CGridWnd::GetCodeName(CString code)
{
	code.TrimLeft();
	code.TrimRight();

	if (code.IsEmpty())
		return _T("");

	// 1. 캐시에 있으면 바로 반환
	std::map<CString, CString>::iterator it = m_mapCodeName.find(code);
	if (it != m_mapCodeName.end())
		return it->second;

	// 2. 없으면 기존 방식으로 조회
	CString ret;
	CString sztmp;
	sztmp.Format(_T("%s"), code);

	ret.Format(_T("%s"),
		(char*)m_pView->SendMessage(
			WM_USER,
			MAKEWPARAM(nameDLL, 0),
			(LPARAM)sztmp.operator LPCTSTR()
		)
	);

	ret.TrimLeft();
	ret.TrimRight();

	if (ret.GetLength() > 32)
		ret = ret.Mid(0, 32);

	// 3. 캐시에 저장
	m_mapCodeName[code] = ret;

	return ret;
}

void CGridWnd::OperAlarm(int kind, char *lParam)
{
	LPARAM data = 0;
	WORD hval = 0;
	const int ncnt = m_arExist.GetCount();
	int nindex = -1;

	if (kind == MO_INIT)
	{
		for (int ii = 0; ii < ncnt; ii++)
		{
			nindex = atoi(m_arExist.GetValue(ii));
			data = m_grid->GetItemData(nindex + 1, colSIG);
			hval = HIWORD(data);
			data = MAKELPARAM(0, hval);
			m_grid->SetItemData(nindex + 1, colSIG, data); // KSJ colSIG
		}
	}
	else
	{
		CString strA, strB;
		strA.Format("%s", lParam);
		if (m_arExist.GetCount())
		{
			strB = m_arExist.GetData(strA);
			strB.TrimRight();
			if (strB.IsEmpty())
				return;
			nindex = atoi(strB);
			data = m_grid->GetItemData(nindex + 1, colSIG);
			hval = HIWORD(data);
			data = MAKELPARAM(1, hval);
			m_grid->SetItemData(nindex + 1, colSIG, data); // KSJ colSIG

			if (nindex >= 0)
			{
				m_grid->m_nAlmRow = nindex + 1;
				m_grid->Invalidate(false);
			}
		}
	}
}

void CGridWnd::ReIndexing()
{
	m_arExist.RemoveAll();
	CString sztmp, szindex;
	for (int ii = 0; ii < MAX_LINE; ii++)
	{
		sztmp = m_grid->GetItemText(ii + 1, 0);
		if (!sztmp.IsEmpty())
		{
			szindex.Format("%d", ii);
			m_arExist.SetData(sztmp, szindex);
		}
	}
}

int CGridWnd::GetIndex(CString symbol)
{
	const int ncnt = m_gridHdrX.GetSize();
	struct _gridHdr xgridHdr{};
	int ret = -1;

	for (int ii = colCURR + 1; ii < ncnt; ii++)
	{
		xgridHdr = m_gridHdrX.GetAt(ii);

		if (symbol.CompareNoCase(xgridHdr.symbol) == 0)
		{
			ret = ii;
			break;
		}
	}

	return ret;
}

CString CGridWnd::CalcuPyungaSonik(struct _intersx *pinters, CString curr)
{
	CString ret = _T("");
	const double dval1 = IH::TOfabs(pinters->xnum);
	const double dval2 = IH::TOfabs(pinters->xprc);
	const double dval3 = IH::TOfabs(curr);
	const double maeipPrc = pinters->maeipPrc;
	const double creditPrc = pinters->creditPrc;
	double result = 0;
	double rPrice = 0;

	if (dval1 <= 0 || dval2 <= 0)
		return ret;

	//	평가손익 = 평가금액(신용금액 차감) - 매입금액
	rPrice = (dval3 * dval1) - creditPrc;

	if (maeipPrc != 0)
	{
		result = rPrice - maeipPrc;
	}
	else
	{
		result = rPrice - (dval2 * dval1);
	}

	if (result > 0)
	{
		ret.Format("%+.0f", result);
	}
	else if (result < 0)
	{
		ret.Format("-%.0f", result);
	}
	return ret;
}

CString CGridWnd::CalcuSuik(struct _intersx *pinters, CString curr)
{
	CString ret = _T("");
	const double dval1 = IH::TOfabs(pinters->xnum);
	const double dval2 = IH::TOfabs(pinters->xprc);
	const double dval3 = IH::TOfabs(curr);
	const double creditPrc = pinters->creditPrc;
	const double maeipPrc = pinters->maeipPrc;
	double result = 0;
	const double resultVal = 0;
	double rPrice = 0;

	if (dval1 <= 0 || dval2 <= 0)
		return ret;

	//	평가손익 = 평가금액(신용금액 차감) - 매입금액
	rPrice = (dval3 * dval1) - creditPrc;

	if (maeipPrc != 0)
	{
		result = rPrice - maeipPrc;
	}
	else
	{
		result = rPrice - (dval2 * dval1);
	}

	//	ret.Format("%+.0f", result);
	//	resultVal = atof(ret);

	//  수익률 = 평가손익 * 100 / 매입가
	if (maeipPrc != 0)
	{
		ret.Format("%+.2f", ((result * 100.0) / maeipPrc));
	}
	else
	{
		ret.Format("%+.2f", ((result * 100.0) / (dval2 * dval1)));
	}
	return ret;
}

CString CGridWnd::CalFutureEvalPrice(struct _intersx *pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian)
{
	//	if (!m_dBase)
	getBase(m_code);

	const double maeipPrc = pinters->maeipPrc; //원장에서 받은 매입금액

	double dSonik = 0, dGubn = 1;
	bool bMd = false;
	CString m_pprice, m_pgsonik;
	double m_pmaeip = 0;

	if (m_curr == 0)
	{
		return "";
	}

	const int kind = getJKind(m_code);
	if (m_gubn == "1")
		bMd = true;
	if (bMd)
		dGubn = -1;

	m_pprice.Format("%.f", m_remian * m_curr * m_dBase);

	if (maeipPrc != 0)
	{
		m_pmaeip = maeipPrc;
	}
	else
	{
		m_pmaeip = m_remian * m_maip * m_dBase;
	}

	dSonik = atof(getFormatData(m_pprice)) - m_pmaeip;
	dSonik *= dGubn;

	if (dSonik > 0)
		m_pgsonik.Format("+%.0f", dSonik);
	else if (dSonik < 0)
		m_pgsonik.Format("-%.0f", dSonik);

	return m_pgsonik;
}

CString CGridWnd::CalFutureEvalRate(struct _intersx *pinters, CString m_code, CString m_gubn, double m_curr, double m_maip, double m_remian)
{
	getBase(m_code);

	const double maeipPrc = pinters->maeipPrc; //원장에서 받은 매입금액

	double dSonik = 0, dGubn = 1;
	bool bMd = false;
	CString m_pprice, m_pgsonik, m_suik;
	double m_pmaeip = 0;

	if (m_curr == 0)
	{
		return "";
	}

	const int kind = getJKind(m_code);
	if (m_gubn == "1")
		bMd = true;
	if (bMd)
		dGubn = -1;

	m_pprice.Format("%.f", m_remian * m_curr * m_dBase);

	if (maeipPrc != 0)
	{
		m_pmaeip = maeipPrc;
	}
	else
	{
		m_pmaeip = m_remian * m_maip * m_dBase;
	}

	dSonik = atof(getFormatData(m_pprice)) - m_pmaeip;
	dSonik *= dGubn;

	// 수익률
	dSonik *= 100;
	dSonik /= m_pmaeip; //(atof(m_rprice) * m_dBase * atof(m_remian));
	m_suik.Format("%.2f", dSonik);
	if (dSonik < 0)
		m_suik = "" + m_suik;
	else if (dSonik > 0)
		m_suik = "+" + m_suik;

	return m_suik;
}

void CGridWnd::SetGridBkColor(COLORREF color)
{
	color = RGB(255, 255, 255); ////BUFFET FOR FIXED BACKCOLOR

	if (m_grid != nullptr && m_grid->GetSafeHwnd())
	{
		m_grid->SetBkColor(color);

		// ADD PSH 20070911
		m_grid->SetGridColor(getAxColor(65));
		m_grid->Invalidate(false);
		m_btReload.Invalidate(TRUE);
		m_btDomino.Invalidate(TRUE);
		m_btSAVE.Invalidate(TRUE);
		m_btCLOSE.Invalidate(TRUE);
		// END ADD

		initialGrid(false); // BUFFET
	}
}

void CGridWnd::ReIndexSpecial()
{
	const int ncnt = m_gridHdrX.GetSize();
	UINT needs = 0;
	CString string;
	for (int ii = 0; ii < ncnt; ii++)
	{
		needs = m_gridHdrX.GetAt(ii).needs;
		if (needs == 2)
			continue;

		string.Format("%s", m_gridHdrX.GetAt(ii).symbol);

		if (needs == 9)
		{
			if (string.Compare("###3") == 0)
				m_sonikField = ii;
			if (string.Compare("###4") == 0)
				m_suikField = ii;
		}

		if (needs == 7)
			m_bongField = ii;

		//전일거래대비율 컬럼 index 변경
		if (needs == 10)
			m_yDayVolField = ii;

		if (needs == 11)
			m_EqualizerField = ii;

		if (needs == 12)
			m_recomDataField = ii;

		if (needs == 13)
			m_recomBasicCostField = ii;

		if (needs == 14)
			m_recomSuikrateField = ii;

		/* 2011.12.16 KSJ
		if (needs == 15)
			m_recomBMjisuField = ii;

		if (needs == 16)
			m_recomBMSuikrateField = ii;

		if (needs == 17)
			m_recomgoalCostField = ii;
		*/

		if (needs == 18)
			m_recomJisuGubnField = ii;

		if (needs == 19)
			m_upjongCode = ii;

		if (needs == 22)
			m_beforeVolField = ii;

		if (needs == 21)
			m_newCurrField = ii;

		// 2016.04.15 KSJ epb 추천일자
		if (needs == 25)
			m_epbDataField = ii;
	}
}

void CGridWnd::PlayWave(CString strFile /* = "" */)
{
	if (strFile.IsEmpty())
	{
		strFile = m_strWaveFile;
	}

	CFileFind ff;

	if (ff.FindFile(strFile))
		sndPlaySound(strFile, SND_ASYNC);
}

void CGridWnd::SetMarketColor(BOOL bApply, COLORREF clrKospi, COLORREF clrKosdaq)
{
	m_bMKClrApply = bApply;
	m_clrKospi = clrKospi;
	m_clrKosdaq = clrKosdaq;

	m_grid->m_bMKClrApply = m_bMKClrApply;
	m_grid->m_clrKospi = m_clrKospi;
	m_grid->m_clrKosdaq = m_clrKosdaq;
}

// 2011.12.02	KSJ
void CGridWnd::SetNewsNChart(BOOL bNewsNChart)
{
	int nWidth = 0;
	m_bNewsNChart = bNewsNChart ? true : false;

	if (m_bNewsNChart)
		nWidth = m_nNewsNChartWidth;

	m_menuHeader.CheckMenuItem(IDC_MENU_NEWSNCHART, m_bNewsNChart ? MF_CHECKED : MF_UNCHECKED);

	m_grid->SetColumnWidth(colSIG, nWidth);
	m_grid->ResetScrollBars();
	m_grid->Invalidate(false);

	m_btReload.Invalidate(TRUE);
	m_btDomino.Invalidate(TRUE);
	m_btSAVE.Invalidate(TRUE);
	m_btCLOSE.Invalidate(TRUE);
}
// KSJ

// 2011.12.05	KSJ
void CGridWnd::SetMargin(BOOL bMargin)
{
	m_bMargin = bMargin;

	m_menuHeader.CheckMenuItem(IDC_MENU_MARGIN, m_bMargin ? MF_CHECKED : MF_UNCHECKED);

	m_grid->SetMarginRatio(m_bMargin);
	m_grid->Invalidate(false);
}
// KSJ

void CGridWnd::SetDispCode(BOOL bDispCode)
{
	int nWidth = 0;
	m_bDispCode = bDispCode;

	if (m_bDispCode)
		nWidth = m_nCodeWidth;

	m_menuHeader.CheckMenuItem(IDC_MENU_CODE, m_bDispCode ? MF_CHECKED : MF_UNCHECKED);

	m_grid->SetColumnWidth(colCODE, nWidth);
	m_grid->ResetScrollBars();
	m_grid->Invalidate(false);

	m_btReload.Invalidate(TRUE);
	m_btDomino.Invalidate(TRUE);
	m_btSAVE.Invalidate(TRUE);
	m_btCLOSE.Invalidate(TRUE);
}

void CGridWnd::MultiChart()
{
	CString strData(_T(""));
	strData.Format("%s /S/t0/d%s\t", mchartMAP, MCSYMBOL);

	const int nCount = m_grid->GetRowCount();

	for (int i = m_grid->GetFixedRowCount(); i < nCount; i++)
	{
		if (!m_grid->IsSelectedRow(i))
			continue;

		strData += m_grid->GetItemText(i, colCODE);
	}

	openView(typeVIEW, strData);
}

BOOL CGridWnd::IsValidCode(CString strCode)
{
	BOOL bFind = FALSE;
	if (m_grid)
	{
		for (int i = 1; i < m_grid->GetRowCount(); i++)
		{
			CString strGridCode(m_grid->GetItemText(i, 0));

			if (strGridCode.GetLength() == 7)
				strGridCode.Delete(0);

			if (strGridCode == strCode)
			{
				bFind = TRUE;
				break;
			}
		}
	}

	return bFind;
}

void CGridWnd::OnSizing(UINT fwSide, LPRECT pRect)
{
	CBaseWnd::OnSizing(fwSide, pRect);

	// TODO: Add your message handler code here

	OperResize(pRect->right - pRect->left, pRect->bottom - pRect->top);
}

int CGridWnd::GetScrollCtrl()
{
	//	return m_grid->GetGridScroll();
	return m_grid->GetHscroll();
}

int CGridWnd::GetGridWidth()
{
	const int childWidth = m_grid->GetGridWidth();

	return childWidth;
}

int CGridWnd::GetWidth()
{
	int width;
	CRect rc;
	GetWindowRect(rc);

	width = rc.Width();

	if (width <= 0)
	{
		width = m_nGridWidth;
	}

	return width;
}

CString CGridWnd::GetRecommandInfo(int row)
{
	return m_grid->GetItemText(row, m_recomJisuGubnField);
}

void CGridWnd::SetInitRecommandInfo()
{
	_mapiYDayVol.clear();
}

int CGridWnd::ArrangeField(int type, CString save[100][2])
{
	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	int maxRow = 0;

	if (nOver == MO_SELECT)
	{
		maxRow = 100;
	}
	else
	{
		maxRow = GetRowValidcount();
	}

	maxRow = m_rowC - 1;
	for (int i = 0; i < 100; i++)
	{
		save[i][0] = "";
		save[i][1] = "";
	}

	//일단 그리드의 종목과 정렬할 값들을 담아온다
	for (int i = 0; i < maxRow; i++)
	{
		save[i][0] = m_grid->GetItemText(i + 1, colCODE);

		if (type == 0) //등록순
		{
		}
		else if (type == 1) //업종순
		{
			save[i][1] = m_grid->GetItemText(i + 1, m_upjongCode);
		}
		else if (type == 2) //종목명순
		{
			save[i][1] = m_grid->GetItemText(i + 1, colNAME);
		}
		else //종목코드순
		{
			save[i][1] = m_grid->GetItemText(i + 1, colCODE);
		}
	}

	return maxRow;
}

void CGridWnd::SendWhenVisibleMode(CString data[100][2])
{
	m_pGridArray.RemoveAll();

	char tempB[64]{};

	//////////////////////////////////////
	struct _gridHdr xgridHdr{};
	for (int ii = 0; ii < m_gridHdrX.GetSize(); ii++)
	{
		xgridHdr = m_gridHdrX.GetAt(ii);

		sprintf(tempB, "%s%c", xgridHdr.symbol, P_NEW);
		m_pGridArray.Add(tempB);
	}

	if (m_pGridArray.GetSize() > GRIDMAXNUM)
	{
		const int nRem = m_pGridArray.GetSize() - GRIDMAXNUM;

		m_bContinue = TRUE;

		SendWhenVisibleModeTR(&data[0], 0, GRIDMAXNUM);
	}
	else
	{
		m_bContinue = FALSE;

		SendWhenVisibleModeTR(&data[0], 0, m_pGridArray.GetSize());
	}
}

void CGridWnd::SendWhenVisibleModeTR(CString data[100][2], int nStart, int nEnd)
{
	int sendL = 0;
	char tempB[64]{};
	const int bufSize = sizeof(struct _gridHi) + 50 + m_gridHdrX.GetSize() * 5 + _vInters.size() * 12 + 12;
	std::string sendB;
	sendB.resize(bufSize);
	FillMemory(sendB.data(), bufSize, ' ');

	const BOOL bExpect = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_EXPECT);
	m_bAutoExpect = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_AUTOEXPECT);

	// if (bExpect)
	// {
	// 	sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 1, P_TAB);
	// }
	// else
	// {
	// 	if (m_bAutoExpect)
	// 	{
	// 		sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 1, P_TAB);
	// 	}
	// 	else
	// 	{
	// 		sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 0, P_TAB);
	// 	}
	// }

	const bool flag = (BOOL)m_pToolWnd->SendMessage(WM_MANAGE, MK_AUTOEXPECT);	
	if (flag)
		_typeAuto = 0;
	else 
	{
		const bool bflag = m_pToolWnd->SendMessage(WM_MANAGE, MK_EXPECT);
		_typeAuto = bflag ? 1 : 2;
	}
	
	switch (_typeAuto)
	{
	case 2:
		sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 0, P_TAB);
		break;		
	default:
		sprintf(tempB, "%s%c%d%c", gEXPECT, P_DELI, 1, P_TAB);
		break;
	}

	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	// 2011.12.08 KSJ 2013.08.28 KSJ 이제 일반시세도 필터링되서 내려오기 때문에 따로 받을 필요 없음.
	// 	sprintf(tempB, "%s%c%d%c", gFILTERING, P_DELI, 1, P_TAB);
	// 	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	// 	sendL += strlen(tempB);
	//

	sprintf(tempB, "%s%c", gSYMBOL, P_DELI);
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	struct _gridHi *gridHi{};

	gridHi = (struct _gridHi *)&sendB[sendL];
	sendL += sizeof(struct _gridHi);

	CopyMemory(gridHi->visible, "99", sizeof(gridHi->visible));
	sprintf(tempB, "%04d", _vInters.size());
	CopyMemory(gridHi->rows, tempB, sizeof(gridHi->rows));

	gridHi->type = '0';
	gridHi->dir = '1';
	gridHi->sort = '0';

	// struct	_gridHdr gridHdr;
	for (int ii = nStart; ii < nEnd; ii++)
	{
		//		gridHdr = m_gridHdrX.GetAt(ii);

		// 		sprintf(tempB, "%s%c", gridHdr.symbol, P_NEW);
		// 		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		// 		sendL += strlen(tempB);

		strcpy(tempB, (LPTSTR)(LPCTSTR)m_pGridArray.GetAt(ii));
		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		sendL += strlen(tempB);
	}

	sprintf(tempB, "2321%c", P_NEW); //전일거래량
	CopyMemory(&sendB[sendL], tempB, strlen(tempB));
	sendL += strlen(tempB);

	if (((CGroupWnd *)m_pGroupWnd)->getRecommandCursel(m_nIndex) >= 0)
	{
		// 		sprintf(tempB, "1915%c", P_NEW);					//지수 구분
		// 		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		// 		sendL += strlen(tempB);
	}
	else
	{
		// 		sprintf(tempB, "1910%c", P_NEW);					//추천일자
		// 		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		// 		sendL += strlen(tempB);
		//
		// 		sprintf(tempB, "1915%c", P_NEW);					//지수 구분
		// 		CopyMemory(&sendB[sendL], tempB, strlen(tempB));
		// 		sendL += strlen(tempB);
	}

	int nInterCnt = 0;

	CWnd *pWnd = (CWnd *)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);

	if (nOver == MO_VISIBLE)
	{
		nInterCnt = GetRowValidcount();
	}
	else
	{
		nInterCnt = 100;
	}

	CString code;
	int ncnt = 0;
	CString tempStr;
	int pos = -1;

	for (int ii = 0; ii < nInterCnt; ii++)
	{
		code = data[ii][0];

		if (strlen(code))
		{
			tempStr = code;

			pos = tempStr.Find(" ");
			if (pos != -1)
			{
				//				strcpy(temp, tempStr.Mid(0, pos));
				code = tempStr.Mid(0, pos);
			}

			ncnt++;
		}

		if (strlen(code) > 0)
		{
			sprintf(tempB, "%s%c", code.GetString(), P_DELI);
			CopyMemory(&sendB[sendL], tempB, strlen(tempB));
			sendL += strlen(tempB);
		}
	}

	sendB[sendL] = P_NEW;
	sendL += 1;
	sendB[sendL] = P_TAB;
	sendL += 1;
	sendB[sendL] = 0x00;
	CSendData sdata;

	char key{};
	_trkey *trkey = (struct _trkey *)&key;

	trkey->group = m_nIndex;
	trkey->kind = TRKEY_GRIDNEW;

	m_updateROW = -1;

	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
	m_endsort = false;

}

void CGridWnd::SetEditWork(bool tp)
{
	m_bEditWork = tp == true ? TRUE : FALSE;
}

void CGridWnd::SetGridinfo(bool bWidthSave)
{
	const int ncnt = m_gridHdrX.GetSize();
	CString sztmp = _T("");
	int nWidth{};
	m_szWIDTH = _T("");
	struct _gridHdr grdHdr{};
	for (int ii = 0; ii < ncnt; ii++)
	{
		grdHdr = m_gridHdrX.GetAt(ii);
		nWidth = m_grid->GetColumnWidth(ii);
		CString strSymbol = (CString)grdHdr.symbol;

		//추천종목필드는 저장하지 않는다
		if (strSymbol.Find("191") > -1)
			continue;

		if (strSymbol.Find("2022") > -1)
			continue;

		if (ii >= colNAME)
		//		if (ii >= colCODE)
		{
			sztmp.Format("%d,", nWidth);
			m_szWIDTH += sztmp;

			if (ii == colCODE && nWidth != 0)
				m_nCodeWidth = nWidth;

			if (ii == colSIG && nWidth != 0)
				m_nNewsNChartWidth = nWidth;
		}
	}

	// 2013.04.16 KSJ 처음에 width만 저장한다.
	// 	if(bWidthSave)
	// 	{
	// 		WritePrivateProfileString(m_section, KEY_WIDTH, m_szWIDTH.operator LPCTSTR(), m_fileCFG);
	//
	// 	}
	// 	else
	savecfg();
}

void CGridWnd::uploadOK()
{
	// 2017.02.21 마지막 'G'에만 'V' 보내기
	if (!m_bLastSend)
	{
		m_bLastSend = TRUE;
		return;
	}

	int sendL = 0;
	std::string sendB;
	sendB.resize(1024 * 16);
	char tempB[32]{};
	//	char	strUinfo[500];

	struct _uinfo *uinfo = (struct _uinfo *)&sendB[sendL];
	sendL += sz_uinfo;

	FillMemory((char *)uinfo, sz_uinfo, ' ');

	CopyMemory(uinfo->gubn, "MY", sizeof(uinfo->gubn));
	uinfo->dirt[0] = 'U';
	uinfo->cont[0] = 'V';

	sprintf(tempB, "00000");
	CopyMemory(uinfo->nblc, tempB, sizeof(uinfo->nblc));

	uinfo->retc[0] = 'O';
	sendB[sendL] = 0x00;

	//	SendTR
	CSendData sdata;
	char key{};
	_trkey *trkey = (struct _trkey *)&key;

	trkey->group = m_nIndex;
	trkey->kind = TRKEY_LASTSET;

	sdata.SetData(trUPDOWN, key, sendB.data(), sendB.size(), "");

	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
}

bool CGridWnd::CheckField(CString symbol)
{
	CString arr;
	for (int ii = 0; ii < m_arrSymbol.GetSize(); ii++)
	{
		arr = m_arrSymbol.GetAt(ii);
		if (symbol.Find(arr) >= 0)
			return true;
	}
	return false;
}

void CGridWnd::initRSymbol()
{
	_mRsymbol.clear();
	_mRsymbol.emplace(std::make_pair(0, 0));
	_mRsymbol.emplace(std::make_pair(950, 950));
	_mRsymbol.emplace(std::make_pair(951, 951));
	_mRsymbol.emplace(std::make_pair(32, 32));
	_mRsymbol.emplace(std::make_pair(31, 31));
	_mRsymbol.emplace(std::make_pair(30, 30));
	_mRsymbol.emplace(std::make_pair(29, 29));
	_mRsymbol.emplace(std::make_pair(27, 27));
	_mRsymbol.emplace(std::make_pair(23, 23));
	_mRsymbol.emplace(std::make_pair(24, 24));
	_mRsymbol.emplace(std::make_pair(111, 111));
	_mRsymbol.emplace(std::make_pair(112, 112));
	_mRsymbol.emplace(std::make_pair(115, 115));
	_mRsymbol.emplace(std::make_pair(116, 116));
	_mRsymbol.emplace(std::make_pair(33, 33));
	_mRsymbol.emplace(std::make_pair(34, 34));
	_mRsymbol.emplace(std::make_pair(40, 40));
	_mRsymbol.emplace(std::make_pair(734, 734));
	_mRsymbol.emplace(std::make_pair(740, 740));
	_mRsymbol.emplace(std::make_pair(15, 15));
	_mRsymbol.emplace(std::make_pair(301, 301));

	const int countX = m_gridHdrX.GetSize();
	for (int jj = col7852; jj < countX; jj++)
	{
		const _gridHdr xgridHdr = m_gridHdrX.GetAt(jj);
		CString sSymbol = CString(xgridHdr.symbol, strlen(xgridHdr.symbol));

		//외인소진율은 실시간이 아니다.
		if (sSymbol.CompareNoCase("2204") == 0 || sSymbol.IsEmpty())
			continue;

		sSymbol = sSymbol.Right(3);
		const int symbol = atoi(sSymbol);

		if (symbol > 0 && jj > 21)
			_mRsymbol.emplace(std::make_pair(symbol, symbol));
	}
}

void CGridWnd::getSeverMemoCheck(CArray<bool, bool>& parr)
{
	int ii = 0;
	if (CAST_TREEID(m_kind)->kind == xISSUE)
		return;
	for_each(_vInters.begin(), _vInters.end(), [&](auto& pinters) {
		if (pinters.get()->bMemo == "Y")
			parr.SetAt(ii, true);
		ii++;
	});
}

void CGridWnd::setSeverMemoCheck(int xrow, bool bflag)
{
	if (CAST_TREEID(m_kind)->kind == xISSUE)
		return;

	auto& pinters = _vInters.at(xrow);
	pinters.get()->bMemo = bflag ? "Y" : "N";
}


// 2012.01.19 KSJ Alertx 추가
void CGridWnd::parsingAlertx(LPARAM lParam)
{
	if (m_bSending)
		return;

	int xrow{};
	CString code, name, symbol, entry, datB, strValue, dataExceptCode, strCode, strData, sTime;
	CString strGubn; // 2012.02.09 KSJ 호가 체결 저장
	CString strTemp;
	const bool bCheType = false; // 2012.06.20 KSJ 체결일때는 true로 바꿔준다.
	CString str950, str951;
	CString strTmp;
	CString slog;
	const int beginTime    = _ttoi(m_strBeginTime);
	const int beginTimeEnd = _ttoi(m_strBeginTimeEnd);
	const int endTimeEnd = _ttoi(m_strEndTimeEnd);
	const int endTime =    _ttoi(m_strEndTime);

	bool bKrx = true;
	struct _alertR *alertR = (struct _alertR *)lParam;
	code = alertR->code;
	if (code.GetLength() == 7)
	{
		strCode = code.Mid(1);
	}
	else if (code.GetAt(0) == 'X')
	{
		strCode = code.Mid(1);
	}
	else
		strCode = code;

	// 예상가 통합시작 구분...
	if (code[0] == 'M' || code[0] == 'N')
		bKrx = false;

	const DWORD *data = (const DWORD *)alertR->ptr[0];
	const bool bNeedMap = (data[0] && strcmp(reinterpret_cast<LPCSTR>(data[0]), "w") == 0);
	const auto getDataPtr = [&](int idx) -> LPCSTR {
		//return data[idx] ? reinterpret_cast<LPCSTR>(data[idx]) : nullptr;
		const int real = bNeedMap ? ToMapped(idx) : idx;// 23 -> 623 (매핑 없는 값은 그대로)
		return data[real] ? reinterpret_cast<LPCSTR>(data[real]) : nullptr;
	};
	const auto getDataString = [&](int idx) -> CString {
		if (const LPCSTR ptr = getDataPtr(idx))   
			return CString(ptr);
		return CString();
	};
	const auto isZeroLike = [](const CString& value, bool includeThirty = false) -> bool {
		CString trimmed(value);
		trimmed.Trim();
		if (trimmed.IsEmpty())
			return true;
		static const char* zeroTokens[] = {"0", "-0", "+0", "0.00", "-0.00", "+0.00", ".00"};
		for (const char* token : zeroTokens)
		{
			if (trimmed == token)
				return true;
		}
		if (includeThirty && trimmed == "30")
			return true;
		return false;
	};
	
	if (const LPCSTR ptr = getDataPtr(0))
		strGubn = ptr;
	
	// KSJ
	// 2013.08.26 KSJ	지수일때 예상가 표시
	/*
		08:30~09:00 'X' 예상가표시
		09:01~14:50 'J' 현재가표시
		14:50~15:00 'X' 예상가표시
		15:00~15:01 'J' 현재가표시
	*/

	//if (strGubn == "X" && code.GetLength() == 5) //지수일때
	//{
	//	code.Delete(0);	     //첫 글자 'X'를 삭제한다.
	//	code.Insert(0, 'K'); //첫 글자를 'K'로 바꿔준다.
	//	strCode = code;
	//}

	// 2013.08.26 KSJ END
	if (code.CompareNoCase("S0000") == 0)
	{
		_newsTitle = (char*)data[15];
		_newscode  = (char*)data[301];
		_newscode.Trim();
		PostMessage(WM_MANAGE, (CAST_TREEID(m_kind)->kind == xISSUE) ? MK_INSERTNEWS : MK_NEWSX);
		return; // 2012.09.11 KSJ 뉴스일때는 아래쪽을 탈필요 없다.
	}

	int count = 0;
	
	BOOL bZisu = FALSE;	   // 2013.08.27 KSJ 지수일때는 구분이 'X'이고 예상가가 23에 온다.
	if (strGubn == "X" && code.GetLength() == 5) //지수일때
	{
		code.Delete(0);	     //첫 글자 'X'를 삭제한다.
		code.Insert(0, 'K'); //첫 글자를 'K'로 바꿔준다.
		strCode = code;
		count = CheckRealTimeCode(code); // 중복개수처리...
		bZisu = TRUE;
	}
	else
		count = CheckRealTimeCode(code); // 중복개수처리...

	if (count == 0)
		return;

	for (int rowPosition = 0; rowPosition < count; rowPosition++)
	{
		xrow = m_irowCode[rowPosition]; // 중복종목처리...
		
		entry = _T("");
		CString oldEXP = m_grid->GetItemText(xrow, colEXPECT);
		CString newEXP = _T("");

		BOOL bTransSymbol = FALSE; // 2012.02.09 KSJ 심볼변경되는 상황 일때 TRUE
		BOOL bDaebi = FALSE;	   // 2012.03.20 KSJ 예상대비가 0일때 체크함.
		

		CString strTime, expect, real, excep;
		const CString codeExceptA(strCode);
		const int codeLength = strCode.GetLength();

		//변경이 있을때마다 배열에 저장해 둔 현재가 데이타 업데이트
		CString saveData;

		// ── 예상가/현재가 원본값 추출 ────────────────────────
		// bNeedMap(w 시장): getDataString 통해 치환된 인덱스(611, 623) 접근
		// 기존 시장:        data[] 직접 접근으로 원본 111, 23 사용
		CString rawExpectValue;
		CString rawCurrValue;

		if (bNeedMap)
		{
			rawExpectValue = getDataString(111);  // ToMapped(111) = 611
			rawCurrValue = getDataString(23);   // ToMapped(23)  = 623
		}
		else if (bZisu)
		{
			rawExpectValue = CString(reinterpret_cast<LPCSTR>(data[23]));
			rawCurrValue = CString(reinterpret_cast<LPCSTR>(data[23]));
		}
		else
		{
			rawExpectValue = data[111] ? CString(reinterpret_cast<LPCSTR>(data[111])) : CString();
			rawCurrValue = data[23] ? CString(reinterpret_cast<LPCSTR>(data[23])) : CString();
		}

		rawExpectValue.Trim();
		rawCurrValue.Trim();

		const BOOL bHasExpect = !isZeroLike(rawExpectValue);
		const BOOL bHasCurr = !isZeroLike(rawCurrValue);

		CString expectValue = rawExpectValue;
		CString currValue = rawCurrValue;

		// 예 이미지 표시/삭제는 오직 611 기준
		m_grid->SetItemText(xrow, colEXPECT, bHasExpect ? "1" : "0");

		// 현재가 표시값
		// 611 예상가가 있으면 현재가 칸에는 예상가를 보여준다.
		// 611 예상가가 없으면 623 현재가를 보여준다.
		CString displayCurrValue = bHasExpect ? rawExpectValue : rawCurrValue;

		LPCSTR expectPtr = bHasExpect ? rawExpectValue.GetString() : nullptr;
		LPCSTR currPtr = bHasCurr ? rawCurrValue.GetString() : nullptr;

		saveData = displayCurrValue;

		const LPCSTR dealTimePtr = getDataPtr(34);
		const LPCSTR serverTimePtr = getDataPtr(40);
		const int dealTime = dealTimePtr ? _ttoi(dealTimePtr) : 0;
		const int serverTime = serverTimePtr ? _ttoi(serverTimePtr) : 0;

		// 평가손익/수익률 계산용 저장값
		if (!isZeroLike(expectValue))
		{
			saveData = expectValue;
		}
		else if (!isZeroLike(currValue))
		{
			saveData = currValue;
		}
		//선물체결 K, 선물예상체결 4, 옵션 예상체결 O, 옵션예상호가 P, 옵션시세  O, 옵션호가 P, 상품선물체결 f, 상품선물호가 g, 업종지수 J, 업종예상지수 Y, 상품선물체결 f, 상품선물호가 g
	//	if(isZeroLike(getDataPtr(24))) //전일대비
		{
			slog.Format("[IB202200][symbol] [%s][%s][%d] bZisu=[%d] rawExpectValue=[%s] expectValue=[%s] 예상가=[%s] 현재가=[%s] saveData=[%s] _typeAuto=[%d] 대비=[%s] 등락률=[%s] 거래량=[%s]",
				strCode, strGubn, count,
				bZisu,
				rawExpectValue,
				expectValue,
				expectPtr ? expectPtr : "",
				currPtr ? currPtr : "",
				saveData,
				_typeAuto,
				getDataPtr(24),
				getDataPtr(33),
				getDataPtr(27)
			);
			Output_DebugString(slog);
		}
		

		entry.Empty();

		CString str90 = getDataString(90); // 10차우선호가 매도
		str90.Trim();
		BOOL bLast = FALSE; // 2015.01.15 필터링때문에 오는 마지막 데이터 무시해야함.ㄷ
		//선물체결 K, 선물예상체결 4, 옵션 예상체결 O, 옵션예상호가 P, 옵션시세  O, 옵션호가 P, 상품선물체결 f, 상품선물호가 g, 업종지수 J, 업종예상지수 Y, 상품선물체결 f, 상품선물호가 g
		// 2015.01.15 선물호가 4 추가됨 2015.02.03 KSJ 9시 장시작할 때도 필터링된 데이터가 나온다.
		if ((strGubn == "L" || strGubn == "4" || strGubn == "P" || strGubn == "g") && ((str90 == "99") || (str90 == "40")))
		{    //L 선물호가 잔량(90심볼존재), 4 선물예상가(90심볼존재), P 옵션 호가 잔량(90심볼존재), g상품선물 호가잔량 (90심볼존재)
			bLast = TRUE;
		}
		
		//m_grid->setExpectdelay(true);

		const int nEndOPMarket = m_grid->GetItemData(xrow, colEXPECT);    // 2013.09.17 KSJ 해당종목이 장종료 되었으면
		//if ((strGubn == "n" || strGubn.Compare("X") == 0 || expectPtr || nEndOPMarket == 1) && !bLast) // 예상가 적용	2013.08.22 지수예상가는111심볼이 없고 구분값이X로 온다.
		if(expectPtr)
		{
			if (expectPtr)
			{
				entry = expectPtr;
			}
			else if (nEndOPMarket == 1) // 2013.09.17 KSJ 해당종목이 장종료 되었을때
				entry = " ";	    //예상가가 0이 올때랑 같은 상황임.
			else
			{
				entry = currPtr ? CString(currPtr) : CString(); // 2013.08.22 지수예상가는111심볼이 없고 구분값이X로 온다.
			}

			//현재 예상버튼이 안눌러져 있을때, 강제로 심볼을 바꾸어서 그리드에 데이터를 보여주고 있다
			//(현재가 심볼로 되어있기 때문에 )
			//그럼 자동 체크 눌러있을때만, 보여주면 되고
			//자동 체크 해지시엔, 예상가 안보여주면 된다
					
			if (_typeAuto == 0) //자동
			{
				bTransSymbol = TRUE; // 2012.02.09 KSJ 현재가 심볼이 예상가 심볼로 바껴야함.
			}
			else if(_typeAuto == 1)  //예상
			{
				bTransSymbol = TRUE; // 2012.02.09 KSJ 현재가 심볼이 예상가 심볼로 바껴야함.
			}
			else
			{
				if (!entry.IsEmpty())
				{
					m_grid->SetItemText(xrow, colEXPECT, "0");
				}
			}
		}
		else if (currPtr)
		{
			entry = currPtr;
			if (_typeAuto == 0)  //자동
			{
					m_grid->SetItemText(xrow, colEXPECT, "0"); //여기
			}
			else if (_typeAuto == 1)  //예상
			{
				if (!entry.IsEmpty())
				{
					bTransSymbol = TRUE; // 2012.02.09 KSJ 현재가 심볼이 예상가 심볼로 바껴야함.

					if (!isZeroLike(entry))
					{
						m_grid->SetItemText(xrow, colEXPECT, "1");
					}
					else
					{
						m_grid->SetItemText(xrow, colEXPECT, "0"); // 2012.08.23 KSJ 예상가 취소 추가함
						entry = m_grid->GetItemText(xrow, colCURR);
						return;
					}
				}
			}
		}

		newEXP = m_grid->GetItemText(xrow, colEXPECT);
		const BOOL bForceDraw = (newEXP == oldEXP) ? FALSE : TRUE;
		const BOOL bExpect = static_cast<BOOL>(_ttoi(newEXP)); // 위에서 설정한 0 이나 1 값 return
		const int countX = m_gridHdrX.GetSize();
		_gridHdr xgridHdr{};

		slog.Format("[IB202200][symbol] [%s] newEXP=[%s] bForceDraw=[%d]  bExpect=[%d]", strCode, newEXP, bForceDraw, bExpect);
		//Output_DebugString(slog);

		// 2012.11.08 KSJ 주식과 선물옵션의 심볼값이 똑같은 것이 있다. 외인소진률 같은것.
		const bool bKospi = (codeLength == 6);

		int nSymbol = 0;
		CString strsym;
		for (int ii = 2; ii < countX; ii++)
		{
			xgridHdr = m_gridHdrX.GetAt(ii);
			symbol = CString(xgridHdr.symbol, strlen(xgridHdr.symbol));
			strsym = symbol;

			//외인소진율은 실시간이 아니다.
			if (symbol == "2204" || symbol.IsEmpty() || symbol.FindOneOf("#") != -1)
				continue;

			if (symbol.GetLength() >= 3)
				symbol = symbol.Right(3);

			nSymbol = atoi(symbol);

			//if (nSymbol == 23)
			//	CString str;

			if (!bTransSymbol) // 2013.07.08 예상체크되어 있을때 밑에 타도록
			{
				// bTransSymbol 심볼변환(예상가변환)을 안하는 경우 -> 현재가 보여줘야 할때
				// 그리드 컬럼이 예상이라도 현재가로
				switch (nSymbol)
				{
				//예상가
				case 111:
					entry = currValue;
					break; //현재가
				//예상거래량
				case 112:
					entry = getDataString(27);
					break; //거래량
				//예상전일대비
				case 115:
					entry = getDataString(24);
					break; //전일대비
				//예상등락률
				case 116:
					entry = getDataString(33);
					break; //등락률
				//외인소진률
				case 204:
					entry = bKospi ? entry : " ";
					break;
				// 2014.03.18 KSJ 심볼값에 #이 들어가면 0이됨
				case 0:
					entry = " ";
					break;

				default:
					if (const LPCSTR ptr = getDataPtr(nSymbol))
						entry = ptr;
					else
						entry.Empty();
					break;
				}
			}
			else if (bZisu)
			{
				//지수인경우 심볼을 알아봐야 한다
				if (const LPCSTR ptr = getDataPtr(nSymbol))
				{
					entry = ptr;
				}
				else
				{
					continue;
				}
			}
			else if (bTransSymbol)
			{
				// bTransSymbol 심볼변환(예상가변환)을 하는 경우 -> 예상가 보여줘야 할때
			 // 그리드 컬럼이 현재라도 예상가로
				switch (nSymbol)
				{
				//예상가
				case 23:
					entry = expectValue;
					break; //예상현재가
				//거래량
				case 27:
					entry = getDataString(112);
					break; //예상거래량
				//전일대비
				case 24:
					entry = getDataString(115);
					break; //예상전일대비
				//등락률
				case 33:
					entry = getDataString(116);
					break; //예상등락률
				//외인소진률
				case 204:
					entry = bKospi ? entry : " ";
					break;
				// 2014.03.18 KSJ 심볼값에 #이 들어가면 0이됨
				case 0:
					entry = " ";
					break;

				default:
					if (const LPCSTR ptr = getDataPtr(nSymbol))
						entry = ptr;
					else
						entry.Empty();
					break;
				}
			}
			else
				continue;
 
			const CString prevValue = m_grid->GetItemText(xrow, ii);
			if (!bForceDraw && entry.CompareNoCase(prevValue) == 0)
				continue;
			if (!bForceDraw && IH::TOf(entry) == IH::TOf(prevValue))
				continue;

			
			if (!(xgridHdr.attr & GVAT_HIDDEN) && !entry.IsEmpty())
			{
				if ((strcmp("2029", xgridHdr.symbol) == 0) ||
				    (strcmp("2030", xgridHdr.symbol) == 0) ||
				    (strcmp("2031", xgridHdr.symbol) == 0))
				{
					if ((1 == xgridHdr.needs) || (3 == xgridHdr.needs))
					{
						CString strVal = entry;
						CString strDiff = strVal;

						if (strVal[0] == '+' || strVal[0] == '-')
						{
							strDiff = strVal.Mid(1);
						}

						double dDiffOpen = 0.0;
						double dPClose{}, dVal{};
						dVal = atof(strDiff);
						dPClose = atof((m_grid->GetItemText(xrow, colPCURR)));

						if (dVal != 0 && dPClose != 0)
						{
							dDiffOpen = (dVal - dPClose) / dPClose * 100;

							if (1 == xgridHdr.needs)
							{
								if (0 > dDiffOpen)
								{
									entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
								}
								else
								{
									entry.Format("%s(%0.2f%c)", strVal, dDiffOpen, P_PER);
								}
							}
							else if (3 == xgridHdr.needs)
							{
								if (0 > dDiffOpen)
								{
									entry.Format("%0.2f%c", dDiffOpen, P_PER);
								}
								else if (0 == dDiffOpen)
								{
									entry.Format(" %0.2f%c", dDiffOpen, P_PER);
								}
								else
								{
									entry.Format(" +%0.2f%c", dDiffOpen, P_PER);
								}
							}
						}
					}
				}
			}

			if ((ii == colCURR) && bExpect && !m_bExpect) // 예상버튼이 안눌려져있고, 동시호가때
			{
				entry.Trim();

				if (isZeroLike(entry))
				{
					entry = m_grid->GetItemText(xrow, colCURR);
					return;
				}
			}

			CString tempStr;

			if (ii == colCURR)
			{
				if (code.Find("K0001") > -1)
				{
					m_dKospi = atof(entry);
					tempStr.Format("%.2f", m_dKospi);
					m_dKospi = atof(tempStr);
				}
				else if (code.Find("KQ001") > -1)
				{
					m_dKosdaq = atof(entry);
					tempStr.Format("%.2f", m_dKosdaq);
					m_dKosdaq = atof(tempStr);
				}
			}

			// 2012.03.20 KSJ 현재가 보합될때 대비, 등락률 지워지지 않는 현상 수정
			if ((strcmp("2115", xgridHdr.symbol) == 0) || (strcmp("2024", xgridHdr.symbol) == 0) || (strcmp("2116", xgridHdr.symbol) == 0) || (strcmp("2033", xgridHdr.symbol) == 0))
			{
				entry.Trim();
				if (isZeroLike(entry, true)) // 2012.07.17 KSJ 옵션일때의 경우 추가
				{
					bDaebi = TRUE;
				}
			}
			else
			{
				// 2012.03.22 KSJ 보유수량, 매입단가, 평가손익 등 필드값에 #이 들어가는 것은 atoi에서 0이 나온다.
				//그래서 entry를 data[0]에서 뽑아오므로 최초 구분값 d,f,b, 이런값들이 들어가 그리드에 뿌려진다.
				entry.Trim();
				if (isZeroLike(entry) || atof(entry) == 0 || atof(symbol) == 0) // KSJ 2012.07.17 atoi --> atof로 수정
				{
					if (entry.GetLength() > 0 && atof(entry.Mid(1)) == 0)
						entry = " "; // 2013.09.24 KSJ 스프레드종목은 ++ +- 등 기호가 두자리로 들어오는데 이때는 atof로 값을 구하면 0으로 나온다. 그래서 한자리 자르고 구해봄
				}
			}

			CString tmp;

			if (ii == colCURR)
			{
				if (entry.IsEmpty())
				{
					entry = m_grid->GetItemText(xrow, colCURR);
				}
			}

			CString strPreValue = prevValue;
			strPreValue.Trim();
			

			// khs

			if (strsym == "5401" || strsym == "5402" || strsym == "5403" || strsym == "5404" || strsym == "5405")
			{
				if (currPtr)
				{
					entry = getABRdata(currPtr, strCode, strsym);
				}
				else
				{
					CString strcurr = m_grid->GetItemText(xrow, colCURR);
					strcurr.Replace("+", "");
					strcurr.Replace("-", "");
					entry = getABRdata(strcurr.GetString(), strCode, strsym);
				}
			}

			if (ii != colCURR && (strPreValue != entry || strPreValue.IsEmpty()))
			{
				entry.Trim();
				if (symbol.CompareNoCase("146") == 0 || symbol.CompareNoCase("181") == 0)
				{
					if (getDataPtr(146))
						m_grid->SetItemText(xrow, ii, entry);
				}
 				else if (symbol.CompareNoCase("051") == 0)
 				{
					entry = getDataString(25);
 					if (!entry.IsEmpty())
 						m_grid->SetItemText(xrow, ii, entry);
 				}
 				else if (symbol.CompareNoCase("071") == 0)
 				{
					entry = getDataString(26);
 					if (!entry.IsEmpty())
 						m_grid->SetItemText(xrow, ii, entry);
 				}
				else if (!entry.IsEmpty()) // 2012.03.20 KSJ 현재가 보합될때 대비, 등락률 지워지지 않는 현상 수정
				{
					if (bDaebi)
						entry = "";

					bDaebi = FALSE;

					// 2012.02.27 KSJ 그려지는 데이터에 이상있을때는 여기를 체크해야함
					m_grid->SetItemText(xrow, ii, entry);
				}
			}
			else if (ii == colCURR)
			{
				// 2013.08.23 KSJ 스프레드종목이면 보합+, 하락+, 상승- 등 색과 기호를 표시해줘야한다.
				if (codeLength == 8 && (strCode.GetAt(0) == '4' || strCode.GetAt(0) == 'D')) //파생상품 코드개편
				{
					if (entry.GetAt(0) == ' ')	 //처음이 빈칸이면 보합에 -, + 표시가 있는 것이다.
						entry.Replace(" ", "0"); //그때는 '　' ㄱ 한자 1번 을 넣어준다. 스페이스 아님..
				}
				// 2013.08.23 KSJ END
				//entry.Format("%d", _ttoi(entry) + GetTickCount() % 1000); //test
				m_grid->SetItemText(xrow, ii, entry);
			}

			if (const LPCSTR ptr27 = getDataPtr(27); ptr27  && m_yDayVolField >= 0)
			{
				//전일거래대비율 실시간 계산
				CString str2027, str2403;
				double int2027 = 0;
				double int2403 = 0;
				double int2321 = 0;


				str2027 = ptr27;
				int2027 = atoi(str2027);
				const auto& it = _mapiYDayVol.find(strCode.GetString());
				if (it != _mapiYDayVol.end())
				{
					AxStd::_Msg("전일거래대비율 찾음 : %s => %s", strCode.GetString(), it->second);
					int2321 = atoi(it->second);
				}

				if (int2321 != 0)
				{
					int2403 = (int2027 / int2321) * 100;
				}
				else
				{
					int2403 = 0;
				}

				if (!bExpect && m_yDayVolField >= 0)
				{
					entry.Format("%.2f", int2403);

					m_grid->SetItemText(xrow, m_yDayVolField, entry);
				}
			}

			if (ii == colCURR)
			{
				if (!bExpect && m_bongField >= 0)
				{
					CString bongdata = _T(""), open = _T(""), high = _T(""), low = _T("");

					if (const LPCSTR ptr29 = getDataPtr(29))
					{
						open = ptr29;
						m_grid->SetItemText(xrow, colOPEN, open);
					}

					if (const LPCSTR ptr30 = getDataPtr(30))
					{
						high = ptr30;
						high.Remove('+'), high.Remove('-');

						if (const LPCSTR ptr31 = getDataPtr(31))
						{
							low = ptr31;
							low.Remove('+'), low.Remove('-');
							bongdata.Format("%s%c%s", high, P_TAB, low);

							m_grid->SetItemText(xrow, m_bongField, bongdata);
						}
					}
				}
				else if (bExpect && m_bongField >= 0)
				{
					m_grid->SetItemText(xrow, m_bongField, _T(""));
				}
			}

			if (m_ccField)
				calcInClient(xrow);

			if (/*(bRealData && bTicker) ||*/ m_posField)
			{
				for (int jj = 0; jj < m_gridHdrX.GetSize(); jj++)
				{
					//if (m_posField)
					{
						double dval1{}, dval2{}, dval3;
						CString str;
						dval1 = dval2 = dval3 = 0.0;
						CString futurnGubn, mCode, strCurr;
						int sizeCode{};

						const _gridHdr xxgridHdr = m_gridHdrX.GetAt(jj);
						if (xxgridHdr.needs != 9)
							continue;

						auto& pinters = _vInters.at(xrow - 1);

						mCode = pinters.get()->code;
						mCode.TrimRight();
						mCode.TrimLeft();
						sizeCode = strlen(mCode);

						if (bExpect == FALSE) // 장중
						{
							switch (xxgridHdr.symbol[3])
							{
							case '3': // 평가손익
								dval1 = IH::TOfabs(pinters.get()->xnum);
								dval2 = IH::TOfabs(pinters.get()->xprc);
								dval3 = IH::TOfabs(m_grid->GetItemText(xrow, colCURR));

								if (dval1 <= 0 || dval2 <= 0)
									continue;

								if (sizeCode == 6 || sizeCode == 9)
								{
									entry = CalcuPyungaSonik(pinters.get(), m_grid->GetItemText(xrow, colCURR));
								}
								else
								{
									futurnGubn = CString(pinters.get()->futureGubn, sizeof(pinters.get()->futureGubn));
									entry = CalFutureEvalPrice(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
								}

								break;

							case '4': // 수익율
								dval1 = IH::TOfabs(pinters.get()->xnum);
								dval2 = IH::TOfabs(pinters.get()->xprc);
								dval3 = IH::TOfabs(m_grid->GetItemText(xrow, colCURR));

								if (dval1 <= 0 || dval2 <= 0)
									continue;

								if (sizeCode == 6 || sizeCode == 9)
								{
									entry = CalcuSuik(pinters.get(), m_grid->GetItemText(xrow, colCURR));
								}
								else
								{
									futurnGubn = CString(pinters->futureGubn, sizeof(pinters->futureGubn));
									entry = CalFutureEvalRate(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
								}

								break;

							default:
								continue;
							}

							m_grid->SetItemText(xrow, jj, entry);
						}
						else //동시호가 시간
						{
							if (_typeAuto == 0)
							{
//								int ii = 0;
								switch (xxgridHdr.symbol[3])
								{
								case '3': // 평가손익
									if (saveData.IsEmpty())
										continue;
									dval1 = IH::TOfabs(pinters.get()->xnum);
									dval2 = IH::TOfabs(pinters.get()->xprc);
									if (dval1 <= 0 || dval2 <= 0)
										continue;
									
									if (sizeCode == 6 || sizeCode == 9)
									{
										entry = CalcuPyungaSonik(pinters.get(), saveData);
									}
									else
									{
										futurnGubn = CString(pinters.get()->futureGubn, sizeof(pinters.get()->futureGubn));
										entry = CalFutureEvalPrice(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
									}

									break;

								case '4': // 수익율
									if (saveData.IsEmpty())
										continue;
									dval1 = IH::TOfabs(pinters.get()->xnum);
									dval2 = IH::TOfabs(pinters.get()->xprc);
									if (dval1 <= 0 || dval2 <= 0)
										continue;									

									if (sizeCode == 6 || sizeCode == 9)
									{
										entry = CalcuSuik(pinters.get(), saveData);
									}
									else
									{
										futurnGubn = CString(pinters.get()->futureGubn, sizeof(pinters.get()->futureGubn));
										entry = CalFutureEvalRate(pinters.get(), code, futurnGubn, dval3, dval2, dval1);
									}

									break;
								default:
									continue;
								}

								m_grid->SetItemText(xrow, jj, entry);
								entry.Empty(); // 2012.02.20 KSJ 초기화
							}
						}
					}
				}
			}


		} // for 문 끝

	
		// 2012.06.20 KSJ 배분, 임의 추가
		if (data[950])
		{
			str950 = (char *)data[950];
			if (!str950.IsEmpty())
			{
				if (str950.GetAt(0) == '1')
				{
					m_grid->SetItemData(xrow, colINFO, 12);
				}
				else
				{
					entry = m_grid->GetItemText(xrow, colINFO); // 종목특이사항
					CString strName = m_grid->GetItemText(xrow, colNAME);
					SetColInfo(strName, xrow, entry);
				}
			}
		}

		if (data[951])
		{
			str951 = (char *)data[951];
			if (!str951.IsEmpty())
			{
				if (str951 == "12" || str951 == "14" || str951 == "16")
				{
					m_grid->SetItemData(xrow, colINFO, 13);
				}
				else
				{
					entry = m_grid->GetItemText(xrow, colINFO); // 종목특이사항
					CString strName = m_grid->GetItemText(xrow, colNAME);
					SetColInfo(strName, xrow, entry);
				}
			}
		}
		// KSJ
	}	 
}







