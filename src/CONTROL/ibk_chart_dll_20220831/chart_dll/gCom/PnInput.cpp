// PnInput.cpp : implementation file
//

#include "stdafx.h"
#include "AxisGCom.h"
#include "PnInput.h"
#include "PnWndCombo.h"
#include "PnWndEdit.h"
#include "PnWndCalendar.h"
#include "CodeCtrl.h"
#include "PnTooltip.h"
#include "memDC.h"
#include "CheckBox.h"

#include "../MxTrace.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPnInput
struct _info	infoInput[] = {
	{ BTN_CFG,	0, 1, 2, 0,	"지표바", "좌측메뉴 보이기/감추기" },
	{ CMB_UNIT,	0, 0, 0, 0,	"", "" },
	{ BTN_UNIT,	0, 1, 2, 0, "주식", "주식"},
	{ CMB_KKIND,	0, 0, 0, 0,	"", "" },
	{ CMB_CODE,	0, 0, 0, 0,	"", "" },
	{ BTN_CODE,	0, 1, 2, 0,	"", "종목 선택창" },
//	{ BTN_K200,	0, 1, 2, 0,	K200_NAME, K200_TIP },
	{ BTN_FUTURE,	0, 1, 2, 0,	"연결선물", "최근월물 연결선물" },
	{ BTN_FO1,	0, 1, 2, 0,	"", "" },
	{ BTN_FO2,	0, 1, 2, 0,	"", "" },
	{ BTN_FO3,	0, 1, 2, 0,	"", "" },
	{ BTN_FO4,	0, 1, 2, 0,	"", "" },
	{ BTN_FO5,	0, 1, 2, 0,	"", "" },
	{ BTN_DDAY,	0, 1, 2, 0,	"", "일간차트" },
	{ BTN_DWEEK,	0, 1, 2, 0,	"", "주간차트" },
	{ BTN_DMON,	0, 1, 2, 0,	"", "월간차트" },
	{ BTN_DMIN,	0, 1, 2, 0,	"", "분 차트" },
	{ BTN_DTICK,	0, 1, 2, 0,	"", "틱 차트" },
	{ BTN_TIME1,	0, 1, 2, 0,	"", "" },
	{ BTN_TIME2,	0, 1, 2, 0,	"", "" },
	{ BTN_TIME3,	0, 1, 2, 0,	"", "" },
	{ BTN_TIME4,	0, 1, 2, 0,	"", "" },
	{ BTN_TIME5,	0, 1, 2, 0,	"", "" },
	{ CMB_TIME,	0, 0, 0, 0,	"", "" },
	{ CALENDAR,	0, 0, 0, 0,	"", "" },
	{ STC_GIGAN,	0, 0, 0, 0,	"", "" },
	{ EDT_GIGAN,	0, 0, 0, 0,	"", "" },
	{ STC_DISP,	0, 0, 0, 0,	"", "" },
	{ EDT_DISP,	0, 0, 0, 0,	"", "" },
	{ CHK_TODAY,	0, 0, 0, 0,	"", "" },
	{ BTN_MENU,	0, 1, 2, 0,	"환경", "환경설정" },
	{ BTN_SAVE,	3, 4, 5, 0,	"저장", "차트틀저장" },
	{ BTN_TOOL,	6, 7, 8, 0,	"보기", "보기설정" }
};

#define	GAP1	1
#define	GAP2	2
#define	GAP3	5
#define	GAP4	15
#define	GAP5	30

#define	rotateTimerID	1111
#define	triggerTimerID	1112

#define MAX_DATA_COUNT	9999

CPnInput::CPnInput(CWnd *pwndView, CWnd* pwndGView, CFont* pFont, int ctrlID, char* info)
: CPnBaseWnd(pwndView, pwndGView, pFont, ctrlID, info)
{
	m_pToolTip = NULL;
	m_pUnitCB = NULL;
	m_pKKindCB = NULL;
	m_pCodeCB = NULL;
	m_pGapCB = NULL;
	m_peditGigan = NULL;
	m_peditDisp = NULL;
	m_pCalendar = NULL;

	m_pCodeImage = NULL;
	m_pIUnitImage = NULL;
	m_pNTimeImage = NULL;
	m_pNBtnImage = NULL;

	m_bInitResize = true;
	m_bStar = false;
	m_bMouseIN = FALSE;
	m_bLButton = false;
	m_iMoveID = 0;
	m_BtnOrgStatus = 0;

	struct _envInfo	*envinfo = (struct _envInfo*)info;

	m_iOrgUnit = m_iDtUnit = envinfo->datainfo.btUnit;
	m_iGKind = envinfo->datainfo.wMainGrpKind;

	SetGapValue(envinfo->datainfo.acIGap);

	ReadEnv(&envinfo->datainfo);

	m_rcSelGap.SetRectEmpty();

	m_iTimerID = 0;
	m_strValidCode = _T("");
	m_strTrigger = _T("");
	m_iTotalToday	= 0;

	m_bSeriesChart = FALSE;
	m_pfontButton = ((CAxisGComApp*)AfxGetApp())->GetFont(pwndView, 9, "돋움");
	m_lock = FALSE;
	m_iDataCount = 0;
//	m_savedDisp.Empty();
}

CPnInput::~CPnInput()
{
}


BEGIN_MESSAGE_MAP(CPnInput, CPnBaseWnd)
	//{{AFX_MSG_MAP(CPnInput)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(GEV_INPUT, OnInputEvent)
	ON_BN_CLICKED(CHK_TODAY, OnClickedToday)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPnInput message handlers


LRESULT CPnInput::OnInputEvent(WPARAM wParam, LPARAM lParam)
{
	CString	strTemp;

	switch (LOWORD(wParam))
	{
	case INPUT_GET_POS_COMMENT:
		m_comment = GetPosComment(CPoint(LOWORD(lParam), HIWORD(lParam)));
		return (LRESULT)(LPCTSTR)m_comment;
	case INPUT_CHANGE:
		switch (HIWORD(wParam))
		{
		case ipUnit:
			ChangeUnit(lParam);
			OnChangeUnit();
			break;
		case ipKKind:
			ChangeKKind(lParam);
			break;
		case IP_CODE:	// lParam:char*
			SetIndexKind(m_pCodeCB->GetEditData());
			if (!SendRequest())
				m_pCodeCB->SetEditData("");
			break;
		case ipCodeArrow:
			break;
		case ipGap:	// time button change
			ChangeGap(lParam);
			SendRequest(FALSE);
			break;
		case ipDate:
			SendRequest(FALSE);
			break;
		case IP_GIGAN:
			if (CheckTR())
			{
				CString	strTemp;
				m_peditGigan->GetWindowText(strTemp);
				m_iDataCount = atoi(strTemp);

				if (m_iDataCount > MAX_DATA_COUNT && m_iDtUnit == GU_CODE)
				{
					m_iDataCount = MAX_DATA_COUNT;
					strTemp.Format("%d", m_iDataCount);
					m_peditGigan->SetWindowText(strTemp);
				}
				if (m_iDataCount > 0)
				{
					m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartTotal), m_iDataCount);
					SendRequest(FALSE);
				}
			}
			break;
		case ipDisplay:
			if (m_peditDisp)
			{
				if (m_bSeriesChart && lParam > 0) 
				{ 
					CString str;

					str.Format("%d", lParam);

					m_peditDisp->SetWindowText(str);
					m_peditDisp->GetWindowText(strTemp);
					m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, dispDayInput), atoi(strTemp));

					m_peditGigan->GetWindowText(str);
					if (atoi(str) != m_iDataCount) 
					{
						m_iDataCount = atoi(str);
						m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartTotal), m_iDataCount);
						SendRequest(FALSE);
					}
				}
			}
			break;
		case ipRotate:
			break;
		case ipTab:
			ProcTab(lParam);
			break;
		case INPUT_BUTTON:
			SendRequest(FALSE);
			break;
		case ipCEdit:
			ProcTab(-1);
			break;
		case ipUnitCode:
			ChangeUnitAndCode((LPCTSTR)lParam);
			break;
		case ipSetDisplay:
			if (m_peditDisp) 
			{
				if (m_bSeriesChart && lParam > 0) 
				{
					CString str;
					str.Format("%d", lParam);
					m_peditDisp->SetWindowText(str);
				}
			}
			break;
		}
		break;
	case INPUT_DATA:
		switch (HIWORD(wParam))
		{
		case GET_TR_INFO:
			return GetTRInfo((char *)lParam);
		case getTRInfo2:
			return GetTRInfo2((char *)lParam);
		case GET_TOTAL_DAY:
			m_peditGigan->GetWindowText(strTemp);
// 2007.01.03 schbang : 당일일때도 조회개수 참조
/*
			if (m_pchbToday->GetCheck())
				return m_iTotalToday;
			else
				m_peditGigan->GetWindowText(strTemp);
*/
			return atoi(strTemp);
		case SET_TOTAL_DAY:
			if (lParam <= 0) return 0;
			/**
			if (m_pchbToday->GetCheck())
				m_iTotalToday = (int)lParam;
			else
			**/
			{
				strTemp.Format("%d", lParam);
				m_iDataCount = lParam;
				m_peditGigan->SetWindowText(strTemp);;
			}
			
			break;
		case getDispDay:
			if (m_peditDisp)
			{
				m_peditDisp->GetWindowText(strTemp);
				return atoi(strTemp);
			}
			return 0;
		case getCalendar:
			{
				CTime time = m_pCalendar->GetDateTime();
				wsprintf((char*)lParam, _T("%04d%02d%02d"), time.GetYear(), time.GetMonth(), time.GetDay());
			}
			break;
		case getCurGapList:
			{
				char* pVal = (char*)lParam;
				for (int ii = 0; ii < 5; ii++)
					pVal[ii] = m_aiGap[ii];
			}
			break;
		case setCodeInfo:
			if (m_iDtUnit == GU_FOREIGN || m_iDtUnit == GU_INDEX) {
				m_pCodeCB->AddHistory(m_iDtUnit == GU_FOREIGN ? STRG: ITRG, m_pCodeCB->GetEditData(), GetSiseName());
			}

			if ((m_iDtIndex == GI_MINUTE || m_iDtIndex == GI_TICK))
				ChangeIndex(m_iDtIndex, true, false);	// caret 때문에..
			break;
		case domminoCode:
			GetDominoCode((char*)lParam, strTemp, false);
			if (strTemp.IsEmpty())
				return TRUE;

			if (m_pCodeCB)
			{
				m_pCodeCB->SetEditData(strTemp);
				m_pCodeCB->SetFocus();
			}

			SendRequest(FALSE);
			break;
		case triggerCode:
			GetDominoCode((char*)lParam, strTemp, true);
			if (strTemp.IsEmpty())
				return FALSE;

			if (m_pCodeCB)
				m_pCodeCB->SetEditData(strTemp);

			SendRequest(FALSE);
			return TRUE;
		case isDollarData:
			return FALSE;
		case GET_VALID_CODE:
			return long(m_strValidCode.operator LPCTSTR());
		case ONLY_SET_CODE:
			m_pCodeCB->SetEditData((char*)lParam);
			break;
		case GET_TODAY:
			return (LRESULT)m_pchbToday->GetCheck();
		case GET_SERIES_KIND:
			return m_iDtIndex;
		}

		break;
	case inputEnv:
		switch (HIWORD(wParam))
		{
		case inputMain:
			ChangeMain(lParam);
			break;
		case inputDisplay:
			if (m_peditDisp) 
			{
				if (m_bSeriesChart && lParam > 0) 
				{
					CString str;
					str.Format("%d", lParam);
					m_peditDisp->SetWindowText(str);
				}
			}
			break;
		case inputReload:
			ReloadEnv((char *)lParam);
			break;
		}
		break;
	case inputViewFocus:
		if (m_pCodeCB)
			m_pCodeCB->SetFocus();
		break;
	case inputChgPalette:
		m_clrBack = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 64);
		m_clrInBack = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 66);
		Invalidate();
		m_pchbToday->SetColorBG(m_clrInBack);

		break;
	default:
		break;
	}

	return TRUE;
}

int CPnInput::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	m_clrBack = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 64);
	m_clrFrame = PALETTERGB(193, 213, 234); 
	m_clrInBack = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 66);

	m_userPath = m_pApp->GetUserPath(m_pwndView);

	CString	path = m_pApp->GetImagePath(m_pwndView, NTIMEIMAGE);
	m_pNTimeImage = m_pApp->GetImage(imgNTime, path);
	path = m_pApp->GetImagePath(m_pwndView, NBTNIMAGE);
	m_pNBtnImage = m_pApp->GetImage(imgNBtn, path);
	path = m_pApp->GetImagePath(m_pwndView, CODEIMAGE);
	m_pCodeImage = m_pApp->GetImage(imgCode, path);
	path = m_pApp->GetImagePath(m_pwndView, IOPTIMAGE);
	m_pIOptImage = m_pApp->GetImage(imgIOpt, path);
	path = m_pApp->GetImagePath(m_pwndView, ICFGIMAGE);
	m_pCfgImage = m_pApp->GetImage(imgICfg, path);

	//** macho add
	path = m_pApp->GetImagePath(m_pwndView, IUNITIMAGE);
	m_pIUnitImage = m_pApp->GetImage(imgIUnit, path);

	CDC	*pDC = GetDC();
	CFont	*sFont = pDC->SelectObject(m_pFont);
	m_szHangul = pDC->GetOutputTextExtent("한");
	m_szNumeric = pDC->GetOutputTextExtent("9");
	m_szAlphabet = pDC->GetOutputTextExtent("K");
	pDC->SelectObject(sFont);
	ReleaseDC(pDC);

	CreateWndCtrl();
	MakeInputSymbol();

	ChangeMain(m_iGKind);
	return 0;
}

void CPnInput::CreateWndCtrl()
{
	int	cmbOption = WS_VISIBLE|WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST;

	// 종합 차트 상단 영역의 "주식", "선옵" 등을 설정하는 콤보박스 초기화
	m_pUnitCB = new CPnWndCombo(this, m_pwndGView, CMB_UNIT);
	m_pUnitCB->Create(cmbOption, CRect(0, 0, 10, 100), this, 1);
	m_pUnitCB->SetFont(m_pFont);

	int	index = m_iDtUnit;
	if (index == GU_OPTION || index == GU_COFUT || index == GU_COOPT)
		index = GU_FUTURE;

	// 20090918 : ADD
	if (index == GU_CFUTURE || index == GU_FFUTURE)
		index = GU_CFUTURE;

	m_pUnitCB->SelectCombo(index);

	m_pUnitCB->ShowWindow(SW_HIDE);

	m_pKKindCB = new CPnWndCombo(this, m_pwndGView, CMB_KKIND);
	m_pKKindCB->Create(cmbOption, CRect(0, 0, 10, 100), this, 2);
	m_pKKindCB->SetFont(m_pFont);
	
	CString tmps;
	COLORREF bg, fg;
	tmps.Format("%s/%s/%s", ((CAxisGComApp*)AfxGetApp())->GetHome(m_pwndView), TABDIR, PALETTE);
	bg = GetPrivateProfileInt(GENERALSN, "ComboBg", -1, tmps);
	fg = GetPrivateProfileInt(GENERALSN, "ComboFg", -1, tmps);
	bg = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), bg);
	fg = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), fg);

	m_pCodeCB = new CCodeCtrl(this, m_pwndGView, m_pFont, CMB_CODE, m_userPath);
	m_pCodeCB->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 10, 200), this, 4);
	m_pCodeCB->SetComboPalette(bg, fg);

	cmbOption = WS_VISIBLE|WS_CHILD|WS_VSCROLL|CBS_DROPDOWN;
	m_pGapCB = new CPnWndCombo(this, m_pwndGView, CMB_TIME);
	m_pGapCB->Create(cmbOption, CRect(0, 0, 10, 10), this, 5);
	m_pGapCB->SetComboPalette(bg, fg);
	m_pGapCB->SetFont(m_pFont);
	m_pGapCB->AddString("1");
	m_pGapCB->AddString("2");
	m_pGapCB->AddString("5");
	m_pGapCB->AddString("10");
	m_pGapCB->AddString("15");
	m_pGapCB->AddString("30");

	m_peditGigan = new CPnWndEdit(this, m_pwndGView, EDT_GIGAN);
	m_peditGigan->CreateEx(WS_EX_CLIENTEDGE, _T("Edit"), "", 
		WS_VISIBLE|WS_CHILD|ES_NUMBER|WS_CLIPSIBLINGS, CRect(0,0,10,10), this, 7);
	m_peditGigan->SetFont(m_pFont);
	m_peditGigan->SetLimitText(4);

	//** macho add
	m_peditDisp = new CPnWndEdit(this, m_pwndGView, EDT_DISP);
	m_peditDisp->CreateEx(WS_EX_CLIENTEDGE, _T("Edit"), "", 
		WS_VISIBLE|WS_CHILD|ES_NUMBER|WS_CLIPSIBLINGS, CRect(0,0,10,10), this, 8);
	m_peditDisp->SetFont(m_pFont);
	m_peditDisp->SetLimitText(4);
	//**m_peditDisp->ShowWindow(SW_HIDE);
	//**

	m_pCalendar = new CPnWndCalendar();
	m_pCalendar->Create(WS_CHILD | WS_VISIBLE|WS_CLIPSIBLINGS, CRect(0,0,10,10), this, CALENDAR);
 	m_pCalendar->SetMonthCalFont(*m_pFont);
	m_pCalendar->SetFormat(_T("yyyy/MM/dd"));
	m_pCalendar->SetMonthCalColour(MCSC_MONTHBK, RGB(250, 252, 248));
	CTime	time = CTime::GetCurrentTime();
	m_pCalendar->SetDateTime(time);


	m_pchbToday = new CCheckBox;
	m_pchbToday->Create("당일", NULL, CRect(0, 0, 10, 10), this, CHK_TODAY, m_pFont);
	m_pchbToday->SetColorBG(m_clrInBack);

	if (!m_pToolTip)
	{
		m_pToolTip = new CPnToolTip(m_pwndView, this, GEV_INPUT);
		m_pToolTip->Create(m_pFont);
	}
}

void CPnInput::OnPaint() 
{
	CPaintDC dc(this);

//	CMemDC	mdc(&dc);
	Draw(&dc);//&mdc);
}

void CPnInput::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx == 0 && cy == 0)
		return;

	if (m_bInitResize)
	{
		m_bInitResize = false;
		SetInfo(CRect(0, 0, cx, cy));
		ArrangeCtrl(CRect(0, 0, cx, cy));

		ChangeIndex(m_iDtIndex);
		ChangeUnit(m_iDtUnit);
		ReadDisplay();

		if (m_iDtUnit == GU_FOREIGN && (m_iDtIndex == GI_MINUTE || m_iDtIndex == GI_TICK))
			ChangeIndex(GI_DAY, true, true, true);
		else if ((m_iDtUnit == GU_FUTURE || m_iDtUnit == GU_OPTION || m_iDtUnit == GU_COFUT || m_iDtUnit == GU_COOPT)
			&& (m_iDtIndex == GI_WEEK || m_iDtIndex == GI_MONTH))
			ChangeIndex(GI_DAY, true, true, true);

		return;
	}

	ArrangeCtrl(CRect(0, 0, cx, cy), false);	
}

void CPnInput::OnDestroy() 
{
	if (m_pUnitCB)		WND_DELETE(m_pUnitCB);
	if (m_pKKindCB)		WND_DELETE(m_pKKindCB);
	if (m_pCodeCB)		WND_DELETE(m_pCodeCB);
	if (m_pGapCB)		WND_DELETE(m_pGapCB);
	if (m_peditGigan)	WND_DELETE(m_peditGigan);
	if (m_peditDisp)	WND_DELETE(m_peditDisp);
	if (m_pCalendar)	WND_DELETE(m_pCalendar);
	if (m_pToolTip)		WND_DELETE(m_pToolTip);
	if (m_pchbToday)	WND_DELETE(m_pchbToday);

	if (m_iTimerID)	
	{
		KillTimer(m_iTimerID);
		m_iTimerID = 0;
	}

	CPnBaseWnd::OnDestroy();
}

void CPnInput::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPnBaseWnd::OnLButtonDown(nFlags, point);

	SetCapture();

	struct _comp* comp = SearchCom(point);
	if (!comp)
	{
		return;
	}
	m_bLButton = true;

	m_iSelID = comp->cID;
	m_BtnOrgStatus = comp->status;
	InvalidateCom(comp, btnDown);
}

void CPnInput::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();

	CPnBaseWnd::OnLButtonUp(nFlags, point);
	LButtonUp(point);
}

void CPnInput::LButtonUp(CPoint point)
{
	ReleaseCapture();

	if (!m_bLButton)
		return;

	m_iMoveID = -1;
	m_bLButton = false;

	struct _comp* pcompOld = SearchCom(m_iSelID);
	struct _comp* pcompNew = SearchCom(point);

	if (!pcompNew || pcompOld != pcompNew)
	{
		if (pcompOld)
		{
			if (m_BtnOrgStatus != btnDown)
				InvalidateCom(pcompOld, btnUp);
		}
		m_iSelID = -1;
		return;
	}

	switch (m_iSelID)
	{
	case BTN_DDAY:
	case BTN_DWEEK:
	case BTN_DMON:
	case BTN_DMIN:
	case BTN_DTICK:
		Invalidate();
		break;
	case BTN_TIME1:	
	case BTN_TIME2:	
	case BTN_TIME3:
	case BTN_TIME4:
	case BTN_TIME5:
		break;
	default:
		if (!pcompNew || m_iSelID != pcompNew->cID || !pcompNew->bshow)
		{
			if (pcompOld)
				InvalidateCom(pcompOld, btnUp);

			m_iSelID = -1;
			return;
		}
	}
	
	if (pcompOld)
	{
		switch (m_iSelID)
		{
		case BTN_UNIT:
			InvalidateCom(pcompOld, btnUp);
			TrackingUnitMenu(point);
			return;
		default:
			InvalidateCom(pcompOld, btnUp);
			break;
		}
	}

	int iIndex = -1;
	int iGap = -1;
	switch (m_iSelID)
	{// process etc button
	case BTN_DDAY:
		iIndex = GI_DAY;
		break;
	case BTN_DWEEK:
		iIndex = GI_WEEK;
		break;
	case BTN_DMON:
		iIndex = GI_MONTH;
		break;
	case BTN_DMIN:
		iIndex = GI_MINUTE;
		break;
	case BTN_DTICK:
		iIndex = GI_TICK;
		break;

	case BTN_TIME1:
		iGap = m_aiGap[0];
		break;
	case BTN_TIME2:
		iGap = m_aiGap[1];
		break;
	case BTN_TIME3:
		iGap = m_aiGap[2];
		break;
	case BTN_TIME4:	
		iGap = m_aiGap[3];
		break;
	case BTN_TIME5:	
		iGap = m_aiGap[4];
		break;

	case BTN_CFG:
		m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewShowWindow, showCfg), 1);
		break;
	case BTN_CODE:
		// 종목 코드 다이얼로그 호출 요청
		OpenCodeDialog();
		break;
	case BTN_K200:
		if (m_bStar)
			m_pCodeCB->SetEditData(STAR_INDEX);
		else
			m_pCodeCB->SetEditData(K200_INDEX);
		m_pCodeCB->SetFocus();
		SendRequest();
		break;
	case BTN_FUTURE: // 연결선물
		if (m_bStar) 
			m_pCodeCB->SetEditData(STAR_FUTURE);
		else
			m_pCodeCB->SetEditData(K200_FUTURE);
		m_pCodeCB->SetFocus();
		SendRequest();
		break;
	case BTN_FO1:
	case BTN_FO2:
	case BTN_FO3:
	case BTN_FO4:
	case BTN_FO5:
		ChangeSKind(m_iSelID - BTN_FO1);
		break;
	case BTN_MENU:
		m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgEnv), 0);
		break;
	case BTN_SAVE:
		m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgSave), 0);
		break;
	case BTN_TOOL:
		m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgToolEnv), 0);
		break;
	}
	
	if (iIndex > 0)
	{
		ChangeIndex(iIndex);
		SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, INPUT_BUTTON), m_iSelID);
	}
	else if (iGap > 0)
	{
		switch(m_iDtIndex)
		{
		case GI_DAY:
		case GI_WEEK:
		case GI_MONTH:
			ChangeIndex(GI_MINUTE, false);
			m_pGapCB->ShowWindow(SW_SHOW);
//			m_pCalendar->ShowWindow(SW_HIDE);
			break;
		default:
			break;
		}

		ChangeGap(iGap);
		SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, INPUT_BUTTON), m_iSelID);
	}
}

void CPnInput::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CPnBaseWnd::OnRButtonUp(nFlags, point);

	struct _comp* comp = SearchCom(point);
	if (comp)
	{
		int	index = -1;
		switch (comp->cID)
		{
		case BTN_TIME1:	index = 0;	break;
		case BTN_TIME2:	index = 1;	break;
		case BTN_TIME3:	index = 2;	break;
		case BTN_TIME4:	index = 3;	break;
		case BTN_TIME5:	index = 4;	break;
		}

		if (index >= 0)
		{
			UINT	pVal[15] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15, 20, 30, 60, 90};
			CMenu cMenu;
			cMenu.CreatePopupMenu();

			CString	tmpS;
			for (int ii = 0; ii < 15; ii++)
			{
				tmpS.Format("%d", pVal[ii]);
				cMenu.AppendMenu(MFT_STRING, pVal[ii], tmpS);
			}

			CPoint	pt;
			GetCursorPos(&pt);
			int ret = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, pt.x, pt.y, this, NULL);

			if (ret >= 1 && ret <= 90)
			{
				m_aiGap[index] = ret;
				if (comp->status != btnDown)
				{
					InvalidateCom(comp, btnUp);
				}
				else
				{
					InvalidateCom(comp, comp->status);
					ChangeGap(ret);
					SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, INPUT_BUTTON), comp->cID);
				}
			}
		}
	}
}

//#include <fstream.h>
void CPnInput::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPnBaseWnd::OnMouseMove(nFlags, point);

	if (!m_bMouseIN)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE;
		m_bMouseIN = _TrackMouseEvent(&tme);
	}

//ofstream dFile ("C:\\dataG.trc", ios::app);
//dFile	<<"<"<<m_bMouseIN<<""<<endl;

	if (m_bLButton)
		return;

	CPoint	Orgpoint = point;
	struct _comp* mcomp = SearchCom(Orgpoint);
	if (!mcomp)
	{
		mcomp = SearchCom(m_iMoveID);
		if (mcomp)
			InvalidateCom(mcomp, btnUp);
		m_iMoveID = -1;

		return;
	}

	switch (mcomp->cID)
	{
	case BTN_UNIT: //** macho add
	case BTN_CFG:		case BTN_CODE:	/**case BTN_GWAN:	case BTN_CPRE:	case BTN_CNEXT case BTN_RSELECT:	**/
	case BTN_MENU:	case BTN_SAVE:	case BTN_TOOL:
		break;
	//**case BTN_RSTART:
	case BTN_K200: case BTN_FUTURE:
	case BTN_FO1: case BTN_FO2: case BTN_FO3: case BTN_FO4: case BTN_FO5:
	case BTN_DDAY:	case BTN_DWEEK:	case BTN_DMON:	case BTN_DMIN:	case BTN_DTICK:
	case BTN_TIME1:	case BTN_TIME2:	case BTN_TIME3:	case BTN_TIME4: case BTN_TIME5:
		if (mcomp->status == btnDown)
		{
			mcomp = SearchCom(m_iMoveID);
			if (mcomp)
				InvalidateCom(mcomp, btnUp);
			m_iMoveID = -1;

			return;
		}
		break;
	default:
		mcomp = SearchCom(m_iMoveID);
		if (mcomp)
			InvalidateCom(mcomp, btnUp);
		m_iMoveID = -1;

		return;
	}

	if (m_iMoveID == mcomp->cID)
	{
		return;
	}

	struct _comp* ocomp = SearchCom(m_iMoveID);
	if (ocomp)
		InvalidateCom(ocomp, btnUp);

	m_iMoveID = mcomp->cID;
	InvalidateCom(mcomp, btnMove);
}

void CPnInput::mouseLeaveAction(UINT nFlags, CPoint point)
{
	if (m_bLButton)
	{
		m_iSelID = -1;
		LButtonUp(point);
		
	}
	else
	{
		struct _comp* mcomp = SearchCom(m_iMoveID);
		if (mcomp)
			InvalidateCom(mcomp, btnUp);
	}
	m_iMoveID = -1;
	m_bMouseIN = FALSE;

	if (m_pToolTip)
	{
		m_pToolTip->m_dispStr = _T("");
		m_pToolTip->hideToolTip();
	}
}

LRESULT CPnInput::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_MOUSELEAVE)
	{
		mouseLeaveAction(0,CPoint(0,0));
		return TRUE;
	}

	if (m_pToolTip)
		m_pToolTip->RelayEvent(message);	
	
	return CPnBaseWnd::WindowProc(message, wParam, lParam);
}

void CPnInput::Draw(CDC *pDC)
{
	DrawBackground(pDC);

	CFont		*sFont = pDC->SelectObject(m_pFont);
	COLORREF	sColor = pDC->SetTextColor(RGB(0,0,0));
	COLORREF	sbkColor = pDC->SetBkColor(m_clrInBack);

	for (int ii = 0; ii < m_arComs.GetSize(); ii++)
		DrawButton2(pDC, m_arComs.GetAt(ii));
	pDC->DrawText("기간", strlen("기간"), &m_rcGigan, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->DrawText("조회", strlen("조회"), &m_rcDisp, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	pDC->SetBkColor(sbkColor);
	pDC->SetTextColor(sColor);
	pDC->SelectObject(sFont);
}

void CPnInput::ArrangeCtrl(CRect rectC, bool bChgIndex)
{
	bool bSHide, bFHide;

	if (m_iDtUnit == GU_FOREIGN)
		bSHide = true;
	else
		bSHide = false;

	if (m_iDtUnit == GU_FUTURE || m_iDtUnit == GU_OPTION || m_iDtUnit == GU_COFUT || m_iDtUnit == GU_COOPT)
		bFHide = true;
	else	
		bFHide = false;

	int lastComp = CHK_TODAY;
	if (m_iDtUnit == GU_FOREIGN)
		lastComp = EDT_DISP;

	struct _comp *pcomp;
	rectC.DeflateRect(1, 1);

	int iLeft = rectC.right - (szIOptImgX * 3 + 2);

	int iGap;
	int iPos = 0;
	for (int ii = BTN_CFG; ii <= BTN_TOOL; ii++)
	{
		if (ii >= m_arComs.GetSize())
			break;

		iGap = 0;
		pcomp = m_arComs.GetAt(ii);
		pcomp->bshow = true;

		switch (pcomp->cID)
		{
		case CMB_UNIT:
			pcomp->bshow = false;
			break;
		case CMB_KKIND:
			pcomp->bshow = false;//**(m_iDtUnit == GU_INDEX);
			m_pKKindCB->ShowWindow(pcomp->bshow ? SW_SHOW : SW_HIDE);
			break;
		case BTN_K200:
			iGap = 4;
		case BTN_FUTURE:
			pcomp->bshow = bFHide;
			break;
		case BTN_FO1:
			iGap = 4;
		case BTN_FO2:
		case BTN_FO3:
		case BTN_FO4:
		case BTN_FO5:
			pcomp->bshow = bSHide;
			break;
		case CMB_TIME:
		case BTN_TIME1:
			iGap = 1;
		case BTN_DMIN:
		case BTN_DTICK:
		case BTN_TIME2:
		case BTN_TIME3:
		case BTN_TIME4:
		case BTN_TIME5:
			pcomp->bshow = !bSHide;
			break;
		case BTN_UNIT:
		case CMB_CODE:
		case BTN_DDAY:
		case STC_DISP:
		case EDT_DISP:
		case STC_GIGAN:
		case EDT_GIGAN:
			iGap = 4;
			break;
		case CALENDAR:
			iGap = 4;
			break;
		case CHK_TODAY:
			iGap = 4;
			pcomp->bshow = (m_iDtIndex == GI_MINUTE || m_iDtIndex == GI_TICK);
			break;
		case BTN_MENU:
		case BTN_SAVE:
		case BTN_TOOL:
			pcomp->cRC.left = iLeft-5;
			pcomp->cRC.right = iLeft + szIOptImgX-5;
			iLeft += szIOptImgX + 1;
			
			continue;
		default:
			break;
		}


		pcomp->cRC.OffsetRect(iPos - pcomp->cRC.left + iGap+5, 0);
		
		//if (pcomp->bshow && pcomp->cID != CALENDAR)
		if (pcomp->bshow)
			iPos += pcomp->cRC.Width() + iGap;

		switch (pcomp->cID)
		{
		case CMB_UNIT:
			m_pUnitCB->MoveWindow(pcomp->cRC);
			break;
		case CMB_KKIND:
			m_pKKindCB->MoveWindow(pcomp->cRC);
			break;
		case CMB_CODE:
			m_pCodeCB->MoveWindow(pcomp->cRC);
			break;
		case CMB_TIME:
			m_pGapCB->MoveWindow(pcomp->cRC);
			break;
		case BTN_DDAY:
			m_rcIndex.left = pcomp->cRC.left;
			break;
		case BTN_DTICK:
			m_rcIndex.right = pcomp->cRC.right;
			break;
		case CALENDAR:
			m_pCalendar->MoveWindow(pcomp->cRC);
//			m_pCalendar->ShowWindow(pcomp->bshow ? SW_SHOW : SW_HIDE);
			break;
		case CHK_TODAY:
			m_pchbToday->MoveWindow(pcomp->cRC);
			m_pchbToday->ShowWindow(pcomp->bshow ? SW_SHOW : SW_HIDE);
			break;
		case EDT_GIGAN:
			m_peditGigan->MoveWindow(pcomp->cRC);
			break;
		//** macho add
		case EDT_DISP:
			m_peditDisp->MoveWindow(pcomp->cRC);
			break;
		case STC_DISP:
			m_rcDisp = pcomp->cRC;
			break;
		case STC_GIGAN:
			m_rcGigan = pcomp->cRC;
		default:
			break;
		}

		if (pcomp->cID == lastComp) 
		{
			if (pcomp->cRC.right + 4 > iLeft)
				iLeft = pcomp->cRC.right + 4;
		}
	}

	if (m_pGapCB && !m_pGapCB->IsFocusWnd())
		ChangeIndex(m_iDtIndex, true, false, bChgIndex);	// caret 때문에..
}

void CPnInput::DrawButton(CDC* pDC, struct _comp *comp)
{
	CRect cRC = comp->cRC;
	CPoint imgPt = CPoint(cRC.left, cRC.top);

	CImageList* pImage = NULL;
	CString	tmpS = _T("");
	enum enmDrawType {bDraw, bTimeImgDraw, bBtnImgDraw} enmDarw = bDraw;

	switch (comp->cID)
	{
	case BTN_DDAY:	tmpS.Format("일");	enmDarw = bTimeImgDraw;	break;
	case BTN_DWEEK:	tmpS.Format("주");	enmDarw = bTimeImgDraw;	break;
	case BTN_DMON:	tmpS.Format("월");	enmDarw = bTimeImgDraw;	break;
	case BTN_DMIN:	tmpS.Format("분");	enmDarw = bTimeImgDraw;	break;
	case BTN_DTICK:	tmpS.Format("틱");	enmDarw = bTimeImgDraw;	break;

	case BTN_TIME1:	tmpS.Format("%d", m_aiGap[0]);	enmDarw = bTimeImgDraw;	break;
	case BTN_TIME2:	tmpS.Format("%d", m_aiGap[1]);	enmDarw = bTimeImgDraw;	break;
	case BTN_TIME3:	tmpS.Format("%d", m_aiGap[2]);	enmDarw = bTimeImgDraw;	break;
	case BTN_TIME4:	tmpS.Format("%d", m_aiGap[3]);	enmDarw = bTimeImgDraw;	break;
	case BTN_TIME5:	tmpS.Format("%d", m_aiGap[4]);	enmDarw = bTimeImgDraw;	break;
	case BTN_K200:	tmpS.Format("%s", comp->str);	enmDarw = bBtnImgDraw;	break;
	case BTN_FUTURE: tmpS.Format("연결선물");	enmDarw = bBtnImgDraw;	break;
	case BTN_FO1:	tmpS.Format("다우산업");	enmDarw = bBtnImgDraw;	break;
	case BTN_FO2:	tmpS.Format("나 스 닥");	enmDarw = bBtnImgDraw;	break;
	case BTN_FO3:	tmpS.Format("S&&P 500");	enmDarw = bBtnImgDraw;	break;
	case BTN_FO4:	tmpS.Format("반 도 체");	enmDarw = bBtnImgDraw;	break;
	case BTN_FO5:	tmpS.Format("나스닥선물");	enmDarw = bBtnImgDraw;	break;

	default:
		break;
	}

	if (enmDarw == bTimeImgDraw)
	{
		CPnBaseWnd::DrawButton(pDC, dtButton, comp, m_pNTimeImage);
	}
	else if (enmDarw == bBtnImgDraw)
	{
		CPnBaseWnd::DrawButton(pDC, dtButton, comp, m_pNBtnImage);
	}
	else
	{
		if (!pImage)
		{
			switch (comp->status)
			{
			case btnDown:
				cRC.DeflateRect(1, 1, 1, 1);
				pDC->DrawFrameControl(&cRC, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
				break;
			case btnMove:
				cRC.DeflateRect(1, 1, 1, 1);
				pDC->DrawEdge(&cRC, BDR_RAISEDINNER, BF_RECT);
				break;
			case btnUp:
			default:
				break;
			}
		}
		else
		{
			pImage->Draw(pDC, 0, imgPt, ILD_TRANSPARENT);
		}
	}

	cRC.DeflateRect(1, 1, 1, 1);
	int mode = pDC->SetBkMode(TRANSPARENT);
	CFont	*sFont = pDC->SelectObject(m_pfontButton);
	if (!tmpS.IsEmpty())
		pDC->DrawText(tmpS, cRC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetBkMode(mode);
	pDC->SelectObject(sFont);
}

void CPnInput::DrawButton2(CDC* pDC, struct _comp* comp)
{
	if (!comp->bshow)
		return;

	switch (comp->cID)
	{
	case BTN_CFG:
		CPnBaseWnd::DrawButton(pDC, dtButton, comp, m_pCfgImage);
		break;
	/** macho **/
	case BTN_UNIT:
		CPnBaseWnd::DrawButton(pDC, dtButton, comp, m_pIUnitImage);
		DrawUnitButton(pDC, comp->cRC);
		break;
	case BTN_DDAY:	case BTN_DWEEK:	case BTN_DMON:	case BTN_DMIN:	case BTN_DTICK:
	case BTN_TIME1:	case BTN_TIME2:	case BTN_TIME3:	case BTN_TIME4: case BTN_TIME5:
		DrawButton(pDC, comp);	break;
	case BTN_CODE:	
		CPnBaseWnd::DrawButton(pDC, dtButton, comp, m_pCodeImage);	break;
	case BTN_K200: case BTN_FUTURE:
	case BTN_FO1: case BTN_FO2: case BTN_FO3: case BTN_FO4: case BTN_FO5:
		DrawButton(pDC, comp);	break;
		break;
	case BTN_MENU:	case BTN_SAVE:	case BTN_TOOL:
		CPnBaseWnd::DrawButton(pDC, dtButton, comp, m_pIOptImage);	break;
	}
}

void CPnInput::SetInfo(CRect wRC)
{
	int	size = sizeof(infoInput) / sz_INFO;

	wRC.DeflateRect(1, 1+4);
	int	left = wRC.left+300;
	int	top = wRC.top;
	int	bottom = wRC.bottom;
	int	Height = wRC.Height() - 2;
	int	uleft;

	CRect rcUnit; //** macho

	m_rcIndex.SetRect(left, top, left, bottom);
	for (int ii = 0; ii < size; ii++)
	{
		_comp* pcomp = new _comp;
		ZeroMemory(pcomp, sz_COMP);

		pcomp->cID = infoInput[ii].kind;
		pcomp->status = btnUp;
		pcomp->bshow = true;
		pcomp->imggen = infoInput[ii].imggen;
		pcomp->imgmov = infoInput[ii].imgmov;
		pcomp->imgsel = infoInput[ii].imgsel;
		pcomp->imgdis = infoInput[ii].imgdis;
		CopyMemory(pcomp->str, infoInput[ii].disp, sizeof(infoInput[ii].disp));
		CopyMemory(pcomp->comment, infoInput[ii].com, sizeof(infoInput[ii].com));

		switch(pcomp->cID)
		{
		case BTN_CFG:
			pcomp->cRC.SetRect(left, top, left+szICfgImgX, bottom);
			left += szICfgImgX + 1;
			break;
		case CMB_UNIT:
			pcomp->cRC.SetRect(left, top, left+m_szHangul.cx*2+6+Height, bottom-1);
			rcUnit = pcomp->cRC;
			left += m_szHangul.cx*2+6+Height + 1;
			pcomp->cRC.OffsetRect(0, -30);
			break;
		case BTN_UNIT:
			pcomp->cRC = rcUnit;
			break;
		case CMB_KKIND:
			pcomp->cRC.SetRect(left, top, left+m_szAlphabet.cx*5+m_szNumeric.cx*4 +4+Height, bottom-1);
			uleft = left;
			uleft += m_szAlphabet.cx*5+m_szNumeric.cx*4 +4+Height + 1;
			break;
		case CMB_CODE:
			pcomp->cRC.SetRect(left, top, left+m_szHangul.cx*5+4+Height, bottom-1);
			left += m_szHangul.cx*5+4+Height + 1;
			break;
		case BTN_CODE:
			pcomp->cRC.SetRect(left, top+1, left+szCodeImgX, bottom-1);
			left += szCodeImgX + 1;
			m_ptCodeBtn = pcomp->cRC.BottomRight();
			break;
		case BTN_K200: 
		case BTN_FUTURE:
			pcomp->cRC.SetRect(left, top+1, left+szIBtnImgX, bottom-1);
			left += szIOptImgX + 1;
			break;
		case BTN_FO1: case BTN_FO2: case BTN_FO3: case BTN_FO4: case BTN_FO5:
			pcomp->cRC.SetRect(left, top+1, left+szIBtnImgX, bottom-1);
			left += szIBtnImgX + 1;
			break;
		case BTN_DDAY:
			m_rcIndex.left = left;
			pcomp->cRC.SetRect(left, top+1, left+szDateImgX, bottom-1);
			left += szDateImgX;
			m_rcDAY = pcomp->cRC;
			break;
		case BTN_DMON:
			pcomp->cRC.SetRect(left, top+1, left+szDateImgX, bottom-1);
			left += szDateImgX;
			m_rcMON = pcomp->cRC;
			break;
		case BTN_DWEEK:
		case BTN_DMIN:
			pcomp->cRC.SetRect(left, top+1, left+szDateImgX, bottom-1);
			left += szDateImgX;
			break;
		case BTN_DTICK:
			pcomp->cRC.SetRect(left, top+1, left+szDateImgX, bottom-1);
			left += szDateImgX + 1;
			m_rcIndex.right = left - 1;
			break;

		case CMB_TIME:
			pcomp->cRC.SetRect(left, top, left+m_szNumeric.cx*3+8+Height, bottom+200);
			left += m_szNumeric.cx*3+8+Height + 1;
			break;

		case BTN_TIME1:
		case BTN_TIME2:
		case BTN_TIME3:
		case BTN_TIME4:
			pcomp->cRC.SetRect(left, top+1, left+szTimeImgX, bottom-1);
			left += szTimeImgX;
			break;
		case BTN_TIME5:
			pcomp->cRC.SetRect(left, top+1, left+szTimeImgX, bottom-1);
			left += szTimeImgX + 3;
			break;

		case CALENDAR:
			pcomp->cRC.SetRect(left, top, left+m_szNumeric.cx*11+Height+4, bottom - 1);
			left += m_szNumeric.cx * 11 + Height + 4 + 3;
			break;
		case STC_GIGAN:
			pcomp->cRC.SetRect(left, top, left+m_szHangul.cx*2, bottom);
			left += m_szHangul.cx*2 + 3;
			m_rcGigan = pcomp->cRC;
			break;
		case EDT_GIGAN:
			pcomp->cRC.SetRect(left, top+1, left+m_szNumeric.cx*5, bottom-1);
			left += m_szNumeric.cx*5 + 3;
			break;
		case STC_DISP:
			pcomp->cRC.SetRect(left, top, left+m_szHangul.cx*2, bottom);
			left += m_szHangul.cx*2 + 3;
			m_rcDisp = pcomp->cRC;
			break;
		case EDT_DISP:
			pcomp->cRC.SetRect(left, top+1, left+m_szNumeric.cx*5, bottom-1);
			left += m_szNumeric.cx*5 + 3;
			break;
		case CHK_TODAY:
			pcomp->cRC.SetRect(left, top + 3, left + m_szHangul.cx * 4, bottom - 1);
			left += m_szHangul.cx * 4 + 3;
			break;
		case BTN_MENU:
		case BTN_SAVE:
		case BTN_TOOL:
			pcomp->cRC.SetRect(left, top+1, left+szIOptImgX, bottom-1);
			left += szIOptImgX + 1;
			break;
		default:
			continue;
		}
		m_arComs.Add(pcomp);
	}
}

// 시간,이름,현재가,대비,등락율,거래량,매도호가,매수호가,시가,고가,저가,
// 코드,최근영업일자,상한가,하한가,전일종가,주식수
void CPnInput::MakeInputSymbol()
{
	switch (m_iDtUnit)
	{
	case GU_CODE:
		m_strOutSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OJ_TIME, OJ_NAME, OJ_JGGA, OJ_DIFF, OJ_RATE, OJ_GVOL, OJ_MDHG, OJ_MSHG, 
			OJ_SIGA, OJ_KOGA, OJ_JEGA,
			OJ_CODE, OJ_CDAY, OJ_SAHG, OJ_HAHG, OJ_JJGA, OJ_STCK, OJ_START, OJ_GUBN);
		break;
	case GU_INDEX:
		m_strOutSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OI_TIME, OI_NAME, OI_JGGA, OI_DIFF, OI_RATE, OI_GVOL, OI_MDHG, OI_MSHG, 
			OI_SIGA, OI_KOGA, OI_JEGA,
			OI_CODE, OI_CDAY, OI_SAHG, OI_HAHG, OI_JJGA, OI_STCK, OI_START);
		break;
	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
	case GU_FFUTURE:	// 20090916 : ADD : 상품 선물
	case GU_CFUTURE:	// 20090916 : ADD : 채권, 금리 선물
		switch(WhichFOCode(m_pCodeCB->GetEditData())) 
		{
		case foOption:
		m_strOutSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OO_TIME, OO_NAME, OO_JGGA, OO_DIFF, OO_RATE, OO_GVOL, OO_MDHG, OO_MSHG, 
			OO_SIGA, OO_KOGA, OO_JEGA, 
			OO_CODE, OO_CDAY, OO_SAHG, OO_HAHG, OO_JJGA, OO_STCK, OO_START);
		break;
		case foCoFut:
		m_strOutSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OCF_TIME, OCF_NAME, OCF_JGGA, OCF_DIFF, OCF_RATE, OCF_GVOL, OCF_MDHG, OCF_MSHG, 
			OCF_SIGA, OCF_KOGA, OCF_JEGA,
			OCF_CODE, OCF_CDAY, OCF_SAHG, OCF_HAHG, OCF_JJGA, OCF_STCK, OCF_START);
		break;
		case foCoOpt:
		m_strOutSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OCO_TIME, OCO_NAME, OCO_JGGA, OCO_DIFF, OCO_RATE, OCO_GVOL, OCO_MDHG, OCO_MSHG, 
			OCO_SIGA, OCO_KOGA, OCO_JEGA,
			OCO_CODE, OCO_CDAY, OCO_SAHG, OCO_HAHG, OCO_JJGA, OCO_STCK, OCO_START);
		break;
		case foCmFut:	// 20090916 : ADD : 채권, 금리 선물 추가. 트랜잭션 심볼 정의
		case foFmFut:	// 20090916 : ADD : 상품 선물 추가. 트랜잭션 심볼 정의
		m_strOutSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			PDF_TIME, PDF_NAME, PDF_JGGA, PDF_DIFF, PDF_RATE, PDF_GVOL, PDF_MDHG, PDF_MSHG,
			PDF_SIGA, PDF_KOGA, PDF_JEGA,
			PDF_CODE, PDF_CDAY, PDF_SAHG, PDF_HAHG, PDF_JJGA, PDF_STCK, PDF_START);
		break;
		case foFuture:
		default:
		m_strOutSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OF_TIME, OF_NAME, OF_JGGA, OF_DIFF, OF_RATE, OF_GVOL, OF_MDHG, OF_MSHG, 
			OF_SIGA, OF_KOGA, OF_JEGA,
			OF_CODE, OF_CDAY, OF_SAHG, OF_HAHG, OF_JJGA, OF_STCK, OF_START);
		break;
		}
		break;
	case GU_FOREIGN:
		m_strOutSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OS_TIME, OS_NAME, OS_JGGA, OS_DIFF, OS_RATE, OS_GVOL, OS_MDHG, OS_MSHG,
			OS_SIGA, OS_KOGA, OS_JEGA, 
			OS_CODE, OS_CDAY, OS_SAHG, OS_HAHG, OS_JJGA, OS_STCK, OS_START);
		break;
	}
}

void CPnInput::ChangeUnit(int unit)
{
	// 종목 분류 선택시 이전 선택된 분류와 현재 선택된 분류를 고려하여 처리
	CString	code = m_pCodeCB->GetEditData();
	CString	indexCode;
	switch (m_iDtUnit)
	{
	case GU_CODE:
		switch (unit)
		{
		case GU_INDEX:
			SetIndexCode(code);
			break;
		case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:	case GU_FOREIGN:
		case GU_FFUTURE:	// 20090916 : ADD : 상품 선물
		case GU_CFUTURE:	// 20090916 : ADD : 채권, 금리 선물
			code = GetHistory(unit);
			break;
		case GU_CODE:
			m_pCodeCB->SetUnit(m_iDtUnit);
			return;
		}
		break;
	case GU_INDEX:
		switch (unit)
		{
		case GU_CODE:	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:	case GU_FOREIGN:
		case GU_FFUTURE: // 20090916 : ADD
		case GU_CFUTURE: // 20090916 : ADD
			code = GetHistory(unit);
			break;
		case GU_INDEX:
			SetIndexCode(code);
			break;
		}
		break;
	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
	case GU_FFUTURE: // 20090916 : ADD
	case GU_CFUTURE: // 20090916 : ADD
		switch (unit)
		{
		case GU_CODE:	case GU_FOREIGN:
			code = GetHistory(unit);
			break;
		case GU_INDEX:
			SetIndexCode(code);
			break;
		case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
		case GU_FFUTURE: // 20090916 : ADD
		case GU_CFUTURE: // 20090916 : ADD
			if (unit == m_iDtUnit)
			{
				m_pCodeCB->SetUnit(m_iDtUnit);
				return;
			}

			m_iDtUnit = unit;
			code = GetHistory(unit);		// 20090916 : ADD
			m_pCodeCB->SetEditData(code);	// 20090916 : ADD
			m_pCodeCB->SetUnit(m_iDtUnit);
			MakeInputSymbol();
			m_strValidCode.Format("%s", code);
			m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, unitInput), m_iDtUnit);

			CRect	wrc;
			GetClientRect(&wrc);
			ArrangeCtrl(wrc);
			Invalidate(true);

			return;
		}
		break;
	case GU_FOREIGN:
		switch (unit)
		{
		case GU_INDEX:
			SetIndexCode(code);
			break;
		case GU_CODE:	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
		case GU_FFUTURE:	// 20090916 : ADD : 상품 선물
		case GU_CFUTURE:	// 20090916 : ADD : 채권, 금리 선물
			code = GetHistory(unit);
			break;
		case GU_FOREIGN:
			m_pCodeCB->SetUnit(unit);
			return;
		}
		break;
	}

	m_iDtUnit = unit;
	m_pCodeCB->SetUnit(m_iDtUnit);

	struct _comp* comp = SearchCom(BTN_UNIT);
	if (comp)
	{
		CString	comment = "";
		switch (m_iDtUnit)
		{
		case GU_CODE:		comment = "주식";	break;
		case GU_INDEX:		comment = "업종";	break;
		case GU_FUTURE:		comment = "선물옵션";	break;
		case GU_FOREIGN:	comment = "해외";	break;
		//case GU_FFUTURE:	comment = "상품선물";	break;	// 20090916 : ADD : 분류 버픈 툴팁 표시
		}

		if (!comment.IsEmpty())
			sprintf_s(comp->comment, sizeof(comp->comment), _T("%s"), comment.GetString());
	}

	int	kkind;
	m_pKKindCB->GetData(kkind);
	m_pCodeCB->SetKkind(kkind);

	MakeInputSymbol();
	m_strValidCode.Format("%s", code);
	m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, unitInput), m_iDtUnit);

	CRect	wrc;
	GetClientRect(&wrc);
	ArrangeCtrl(wrc);
	Invalidate(true);

	m_pCodeCB->SetEditData(_T(""));
	if (!code.IsEmpty())
		m_pCodeCB->SetEditData(code);
	else
		m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, clearInput), 0);

	m_pCodeCB->SetFocus();

	ReadDisplay();
}

void CPnInput::ChangeIndex(int iIndex, bool bChgGap, bool bRedraw, bool bMessage)
{
	int iID = -1;

	struct _comp* pcomp;
	if (bRedraw)
	{
		switch(iIndex)
		{
		case GI_DAY:
			iID = BTN_DDAY;	
			break;
		case GI_WEEK:
			iID = BTN_DWEEK;
			break;
		case GI_MONTH:
			iID = BTN_DMON;
			break;
		case GI_MINUTE:
			iID = BTN_DMIN;
			break;
		case GI_TICK:
			iID = BTN_DTICK;
			break;
		default:
			iID = -1;
			break;
		}
		
		for (int ii = BTN_DDAY; ii <= BTN_DTICK; ii++)
		{
			pcomp = CPnBaseWnd::SearchCom(ii);
			if (ii == iID)
				pcomp->status = btnDown;
			else
				pcomp->status = btnUp;
		}
		InvalidateRect(&m_rcIndex);


		m_iDtIndex = iIndex;
		ReadDisplay();

		//** macho
		UINT gaps[] = { 1, 2, 3, 5, 10, 15, 20, 30, 60, 90, 120 };

		m_pGapCB->ResetContent();
		if (m_iDtIndex == GI_MINUTE)
			m_pGapCB->AddString("0.5");

		CString s;
		for (int i = 0; i < (sizeof(gaps) / sizeof(gaps[0])); i++) {
			s.Format("%d", gaps[i]);
			m_pGapCB->AddString(s);
		}
	}

	// 보이기 안보이기 설정
	if (bChgGap)
	{
		switch(iIndex)
		{
		case GI_DAY:
		case GI_WEEK:
		case GI_MONTH:
			ChangeGap();
			m_pGapCB->ShowWindow(SW_HIDE);
//			m_pCalendar->ShowWindow(SW_SHOW);
			m_pchbToday->ShowWindow(SW_HIDE);
			// 2007.01.03 schbang : 항상 입력가능하게
//			m_peditGigan->EnableWindow(true);
			break;
		case GI_MINUTE:
			ChangeGap(m_iMGap, !bRedraw);
			m_pGapCB->ShowWindow(SW_SHOW);
//			m_pCalendar->ShowWindow(SW_HIDE);
			m_pchbToday->ShowWindow(SW_SHOW);
			// 2007.01.03 schbang : 항상 입력가능하게
//			if (m_pchbToday->GetCheck()) {
//				m_peditGigan->EnableWindow(false);
//			}
//			else {
//				m_peditGigan->EnableWindow(true);
//			}
			break;
		case GI_TICK:
			ChangeGap(m_iTGap, !bRedraw);
			m_pGapCB->ShowWindow(SW_SHOW);
//			m_pCalendar->ShowWindow(SW_HIDE);
			m_pchbToday->ShowWindow(SW_SHOW);
			// 2007.01.03 schbang : 항상 입력가능하게
//			if (m_pchbToday->GetCheck()) {
//				m_peditGigan->EnableWindow(false);
//			}
//			else {
//				m_peditGigan->EnableWindow(true);
//			}
			break;
		default:
			break;
		}
	}
	else
	{
		if (bMessage)
			NotifyIndexChange();
	}
}

void CPnInput::ChangeGap(int gap, bool bOnlyChange)
{
	if (!bOnlyChange)
	{
		CRect	btnRC(0,0,0,0);
		if (gap <= 0)
		{
			struct _comp	*comp;
			for (int ii = BTN_TIME1; ii <= BTN_TIME5; ii++)
			{
				comp = CPnBaseWnd::SearchCom(ii);
				if (comp->status == btnDown)
				{
					btnRC = comp->cRC;
					comp->status = btnUp;
				}
			}
			if (!btnRC.IsRectEmpty())
				InvalidateRect(&btnRC);

			m_rcSelGap.SetRectEmpty();
			NotifyIndexChange();

			return;
		}

		int	id = -1;
		if (gap == m_aiGap[0])		id = BTN_TIME1;
		else if (gap == m_aiGap[1])	id = BTN_TIME2;
		else if (gap == m_aiGap[2])	id = BTN_TIME3;
		else if (gap == m_aiGap[3])	id = BTN_TIME4;
		else if (gap == m_aiGap[4])	id = BTN_TIME5;

		struct _comp	*comp;
		for (int ii = BTN_TIME1; ii <= BTN_TIME5; ii++)
		{
			comp = CPnBaseWnd::SearchCom(ii);
			if (ii == id)
			{
				comp->status = btnDown;
				btnRC = comp->cRC;
			}
			else
			{
				comp->status = btnUp;
			}
		}
		InvalidateRect(&m_rcSelGap);
		InvalidateRect(&btnRC);
		m_rcSelGap = btnRC;
	}

	CString	sVal;
	if (gap == 930)
		sVal.Format("0.5");
	else
		sVal.Format("%d", gap);
	m_pGapCB->SetWindowText(sVal);

	if (!bOnlyChange)
	{
		if (m_iDtIndex == GI_MINUTE)	m_iMGap = gap;
		else if (m_iDtIndex == GI_TICK)	m_iTGap = gap;

		NotifyIndexChange();
	}
}

void CPnInput::ChangeKKind(int kkind)
{
	CString	code = _T("");
	switch (kkind)
	{
	case upKospi:
	case upKos200:
	case upKos100:
	case upKosdaq:
	case upKq50:
	case upKoStar:
	case upKrx100:	// Added by schbang 2006.02.20
	case upSector:	// Added by schbang 2006.02.15
	case upFreeboard:
		code = GetIndexCode(kkind);
		break;
	}

	if (code.IsEmpty())	return;

	m_pCodeCB->SetKkind(kkind);
	m_pCodeCB->SetEditData(code);
	m_pCodeCB->SetFocus();

	SendRequest();
}

void CPnInput::ChangeSKind(int skind)
{
	CString	code = _T("");
	switch (skind)
	{
	case fnKind1:	code = fnCode1;	break;
	case fnKind2:	code = fnCode2;	break;
	case fnKind3:	code = fnCode3;	break;
	case fnKind4:	code = fnCode4;	break;
	case fnKind5:	code = fnCode5;	break;
	}

	if (code.IsEmpty())	return;

	m_pCodeCB->SetEditData(code);
	m_pCodeCB->SetFocus();

	SendRequest();
}

void CPnInput::NotifyIndexChange()
{
	int	gap = 0;
	switch(m_iDtIndex)
	{
	case GI_MINUTE:	gap = m_iMGap;	break;
	case GI_TICK:	gap = m_iTGap;	break;
	default:	break;
	}

	m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, indexInput), MAKELPARAM(m_iDtIndex, gap));
}

bool CPnInput::CheckTR()
{
	CString strCode = m_pCodeCB->GetEditData();
	if (strCode.IsEmpty())
		return false;

	MakeInputSymbol();
	switch (m_iDtUnit)
	{
	case GU_FUTURE:
	case GU_OPTION:
	case GU_FFUTURE: // 20090916 : ADD
	case GU_CFUTURE: // 20090916 : ADD
		{
			if (strCode.GetLength() != 8) return false;

			if (strCode != K200_INDEX && strCode != K200_FUTURE && strCode != STAR_INDEX && strCode != STAR_FUTURE)
			{
				char	code[16];
				sprintf_s(code, _T("%s"), strCode.GetString());

				// 20090916 : CHANGE : 상품 선물일 경우 종목명을 가져와 확인한다.
				CString	name; 
				if ((m_iDtUnit == GU_CFUTURE) || (m_iDtUnit == GU_FFUTURE))
				{
					name = (char*)m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgcommodityName), long(code));
				}
				else
				{
					name = (char*)m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgGubnFOSO), long(code));
				}
				if (name.IsEmpty())
					return false;

				int	foCode = WhichFOCode(strCode);
				switch (foCode)
				{
				case foFuture:	
				case foOption:
				case foCmFut:	// 20090916 : ADD
				case foFmFut:	// 20090918 : ADD : 현물 선물 코드 처리
					if (m_bStar)
					{
						m_bStar = false;
						_comp	*comp = SearchCom(BTN_K200);
						if (comp)
						{
							sprintf_s(comp->str, K200_NAME);
							sprintf_s(comp->comment, K200_TIP);
							InvalidateCom(comp, comp->status);
						}
					}
					break;
				case foStar:
					if (!m_bStar)
					{
						m_bStar = true;
						_comp	*comp = SearchCom(BTN_K200);
						if (comp)
						{
							sprintf_s(comp->str, STAR_NAME);
							sprintf_s(comp->comment, STAR_TIP);
							InvalidateCom(comp, comp->status);
						}
					}
					break;
				}
			}
		}

		return TRUE;
	case GU_CODE:
		{
			CString	name = (char*)m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgGubnFOSO), long(strCode.operator LPCTSTR()));
			if (name.IsEmpty())
				return false;
		}
		break;
	case GU_INDEX:
		if (strCode.GetLength() != sz_INDEX)
			return false;
		break;
	case GU_FOREIGN:
		break;
	}

	return true;
}

void CPnInput::ProcTab(int id)
{
	switch (id)
	{
	case CMB_UNIT:
		if (m_pCodeCB->IsWindowEnabled())
			m_pCodeCB->SetFocus();
		break;
	case CMB_TIME:
		m_pCalendar->SetFocus();
		break;

	case CMB_CODE:
		if (m_pGapCB->IsWindowVisible())
			m_pGapCB->SetFocus();
		else
			m_pCalendar->SetFocus();
		break;
	case CALENDAR:
		m_peditGigan->SetFocus();
		break;
	case EDT_GIGAN:
		m_peditDisp->SetFocus();
		break;
	case EDT_DISP:
		m_pCodeCB->SetFocus();
		break;
	case -1:
		if (m_pCalendar && m_pCalendar->IsWindowEnabled())
			m_pCalendar->SetFocus();
		break;
	}
}


void CPnInput::OpenCodeDialog()
{
	int kind;

	switch (m_iDtUnit)
	{
	case GU_CODE:	kind = dlgJCode;	break;
	case GU_INDEX:	kind = dlgUCode;	break;
//		kind = dlgJUCode;	break;
	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
		kind = dlgFOCode;	break;
	// 20090907 : ADD : Commodities Future/Option Code dialog call
	case GU_CFUTURE:	// 채권, 금리 상품 선물 
	case GU_FFUTURE:	// 현물 상품 선물
		kind = dlgCommoditiesFutureCode;
		break;
	default:	kind = 0;
	}

	CPoint	pt = m_ptCodeBtn;
	ClientToScreen(&pt);

	CString	sCode = (char*)m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, kind), MAKELPARAM(pt.x, pt.y));

	if (sCode.IsEmpty())
		return;

	CString	code = "";
	int index = sCode.Find("\t");
	if (index > 0)
		code = sCode.Left(index);
	else
		code = sCode;

	if (m_iDtUnit == GU_CODE || m_iDtUnit == GU_INDEX)
	{
		if (code.GetLength() == 6)
			ChangeUnitAndCode("1;" + code);
		else
			ChangeUnitAndCode("2;" + code);
	}
	else
	{
		m_pCodeCB->SetEditData(code);
		m_pCodeCB->SetFocus();

		SendRequest();
	}
}

int CPnInput::GetTRInfo(char* pcHeader)
{
	CString	strTemp = m_pCodeCB->GetEditData();

	if (strTemp.IsEmpty()) return 0;

	if (m_iDtUnit == GU_INDEX)
	{
		CString	strVal;
		int	iVal;
		m_pKKindCB->GetData(iVal);
		//if (iVal >= 0)
		//	strVal.Format("%02d", iVal);
		//else
			strVal.Format("00");
		strTemp = strVal + strTemp;
	}

	CString	strCode = _T("");
	switch (m_iDtUnit)
	{
	case GU_CODE:	
		strCode = IJ_CODE;
		break;
	case GU_INDEX:	
		strCode = II_CODE;
		break;
	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
	case GU_FFUTURE:	// 20090916 : ADD : 상품 선물
	case GU_CFUTURE:	// 20090916 : ADD : 채권, 금리 선물
		switch(WhichFOCode(strTemp))
		{
		case foOption:
			strCode = IO_CODE;	break;
		case foCoFut:
			strCode = ICF_CODE;	break;
		case foCoOpt:
			strCode = ICO_CODE;	break;
		case foCmFut: // 20090916 : ADD : 채권,  금리 선물 처리
		case foFmFut: // 20090918 : ADD : 현물 선물 처리
			strCode = IPF_CODE; break;
		case foFuture:
		default:
			strCode = IF_CODE;	break;
		}
		break;
	case GU_FOREIGN: 
		strCode = IS_CODE;
		break;
	default:	
		return 0;
	}

	m_strValidCode.Format("%s", strTemp);
	wsprintf(pcHeader, _T("%s%c%s\t%s"), LPCTSTR(strCode) , 0x7F, LPCTSTR(strTemp), LPCTSTR(m_strOutSymbol));

	return strlen(pcHeader);
}

int CPnInput::GetTRInfo2(char *pHeader)
{
	CString	tmpS;
	
	if (m_strValidCode.IsEmpty())
	{
		tmpS = m_pCodeCB->GetEditData();
		if (m_iDtUnit == GU_INDEX)
		{
			CString	tVal;
			int	val;
			m_pKKindCB->GetData(val);
			//if (val >= 0)
			//	tVal.Format("%02d", val);
			//else
				tVal.Format("00");
			tmpS = tVal + tmpS;
		}

		m_strValidCode.Format("%s", tmpS);
	}

	CString	codeS = _T("");
	switch (m_iDtUnit)
	{
	case GU_CODE:	
		codeS = IJ_CODE;
		break;
	case GU_INDEX:	
		codeS = II_CODE;
		break;
	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
		switch(WhichFOCode(m_strValidCode))
		{
		case foOption:
			codeS = IO_CODE;	break;
		case foCoFut:	
			codeS = ICF_CODE;	break;
		case foCoOpt:
			codeS = ICO_CODE;	break;
		case foCmFut: // 20090916 : ADD
		case foFmFut: // 20090918 : ADD : 현물 선물 처리
			codeS = IPF_CODE; break;
		case foFuture:
		default:
			codeS = IF_CODE;	break;
		}
		break;
	case GU_FOREIGN: 
		codeS = IS_CODE;
		break;
	default:	
		return 0;
	}

	wsprintf(pHeader, _T("%s%c%s\t"), LPCTSTR(codeS), 0x7F, LPCTSTR(m_strValidCode));

	return strlen(pHeader);
}

void CPnInput::ReadEnv(struct _dataInfo *datainfo)
{
	switch (datainfo->btUnit)
	{
	case GU_INDEX:
	case GU_FUTURE:
	case GU_OPTION:
	case GU_COFUT:
	case GU_COOPT:
	case GU_FOREIGN:
	case GU_FFUTURE:	// 20090916 : ADD : 상품 선물
	case GU_CFUTURE:	// 20090916 : ADD : 채권, 금리 선물
		m_iDtUnit = datainfo->btUnit;
		break;
	default:
		m_iDtUnit = GU_CODE;
		break;
	}

	switch (datainfo->btIndex)
	{
	case GI_WEEK:
	case GI_MONTH:
	case GI_MINUTE:
	case GI_TICK:
		m_iDtIndex = datainfo->btIndex;
		break;
	default:
		m_iDtIndex = GI_DAY;
		break;
	}
	
	m_iMGap = datainfo->wMGap;
	m_iTGap = datainfo->wTGap;
}

void CPnInput::ReadDisplay()
{
	struct _envInfo* penvinfo = (_envInfo*)m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);

	int iUnit = 0;
	switch (m_iDtUnit)
	{
	case GU_FUTURE:
	case GU_OPTION:
	case GU_COFUT:
	case GU_COOPT:
	case GU_FFUTURE: // 20090916 : ADD
	case GU_CFUTURE: // 20090916 : ADD
		iUnit = GU_FUTURE - GU_CODE;
		break;
	case GU_CODE:
	case GU_INDEX:	
		iUnit = m_iDtUnit - GU_CODE;
		break;
	case GU_FOREIGN:
		iUnit = 3;
		break;
	default:
		break;
	}

	int iTotCnt = penvinfo->datainfo.aaDayInfo[iUnit][m_iDtIndex - GI_DAY].dwTotCnt;
//	int iDisCnt = penvinfo->datainfo.aaDayInfo[iUnit][m_iDtIndex - GI_DAY].dwDisCnt;

	switch (m_iDtIndex)
	{
	case GI_MINUTE:
	case GI_TICK:
		//if (penvinfo->datainfo.aaDayInfo[iUnit][m_iDtIndex - GI_DAY].cToday)
		if (penvinfo->datainfo.cToday)
			m_pchbToday->SetCheck(true);
		else
			m_pchbToday->SetCheck(false);
		break;
	default:
		m_pchbToday->SetCheck(false);
		break;
	}
	

	CString	tmpS;
	tmpS.Format("%d", iTotCnt);
	m_peditGigan->SetWindowText(tmpS);

	//** need set disp
}

void CPnInput::GetDominoCode(char *dCode, CString &domino, bool bTrigger)
{
	domino = _T("");
	CString	sData = dCode;
	if (sData.IsEmpty())
		return;

	int	index;
	CString	name, symbol, val;
	CMapStringToString	rtmStore;
	rtmStore.RemoveAll();
	for (; !sData.IsEmpty(); )
	{
		index = sData.Find('\t');
		if (index < 0)
			break;

		symbol = sData.Left(index++);
		sData = sData.Mid(index);

		index = sData.Find('\n');
		if (index < 0)	index = sData.GetLength();
		val = sData.Left(index++);

		rtmStore.SetAt(symbol, val);

		if (sData.GetLength() <= index)	break;

		sData = sData.Mid(index);
	}

	if (rtmStore.GetCount() > 1)
	{
		switch(m_iDtUnit)
		{
		case GU_CODE:	name = JTRG;	break;
		case GU_INDEX:	name = ITRG;	break;
		case GU_FUTURE:	name = FTRG;	break;
		case GU_OPTION:	name = OTRG;	break;
		case GU_COFUT:	name = CFTRG;	break;
		case GU_COOPT:	name = COTRG;	break;
		case GU_FOREIGN: name = STRG;	break;
		case GU_FFUTURE:					   // 20090916 : ADD
		case GU_CFUTURE: name = CDFTRG; break; // 20090916 : ADD : 상품 선물 도미노 데이터 처리
		default:
			return;
		}

		for (POSITION pos = rtmStore.GetStartPosition(); pos;)
		{
			rtmStore.GetNextAssoc(pos, symbol, val);
			if (name == symbol)
			{
				if (m_iDtUnit == GU_FOREIGN)
					domino = val.Mid(1);
				else
					domino = val;
				break;
			}
		}

		if (m_iDtUnit == GU_INDEX)
		{
			if (domino.IsEmpty())
			{
				domino = GetIndexCode(1);
			}
			else
			{
				SetIndexTrigger(domino);
			}
		}
		else if (m_iDtUnit == GU_FUTURE || m_iDtUnit == GU_OPTION || m_iDtUnit == GU_COFUT || m_iDtUnit == GU_COOPT)
		{
			CString	symbol = FOTRG;
			CString history = (char *)m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getHistory), long(symbol.operator LPCTSTR()));
			if (!history.IsEmpty())
			{
				int index = history.Find("\t");
				if (index > 0)
				{
					CString	Str = history.Left(index);
					index = Str.Find(" ");
					if (index > 0)
					{
						domino = Str.Left(index);
					}
				}
			}
		}

		domino.TrimRight(" ");
		if (domino.IsEmpty())
		{
			rtmStore.RemoveAll();
			if (!bTrigger)
				domino = GetDefaultCode();

			return;
		}
		else if (m_iDtUnit == GU_FUTURE || m_iDtUnit == GU_OPTION || m_iDtUnit == GU_COFUT || m_iDtUnit == GU_COOPT)
		{
			CString	code;
			code.Format("%s", domino);
			CString	name = (char*)m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgGubnFOSO), long(code.operator LPCTSTR()));
			if (!name.IsEmpty())
			{
				int	focode = WhichFOCode(code.operator LPCTSTR());
				switch (focode)
				{
				case foOption:
					if (m_iDtUnit != GU_OPTION)	
						ChangeUnit(GU_OPTION);	
					break;
				case foCoFut:
					if (m_iDtUnit != GU_COFUT)	
						ChangeUnit(GU_COFUT);	
					break;
				case foCoOpt:
					if (m_iDtUnit != GU_COOPT)	
						ChangeUnit(GU_COOPT);	
					break;
				case foFuture:
				default:
					if (m_iDtUnit != GU_FUTURE)	
						ChangeUnit(GU_FUTURE);	
					break;
				}
			}
		}
		else if (m_iDtUnit == GU_CODE)
		{
			if (m_strValidCode == domino)
			{
				domino = _T("");
			}
			else if (bTrigger)
			{
				m_strTrigger.Format("%s\t%s", JTRG, domino);
				SetTimer(triggerTimerID, 10, NULL);
			}
		}
	}
	else if (rtmStore.GetCount() == 1)
	{
		POSITION pos = rtmStore.GetStartPosition();
		rtmStore.GetNextAssoc(pos, symbol, val);
/*		// 20080729
		if (m_strValidCode == val)
			return;
*/
		if (symbol == JTRG)
		{
			if (m_iDtUnit == GU_CODE)
			{
				domino = val;
				domino.TrimRight(" ");
			}

			else if (m_iOrgUnit == GU_CODE && !bTrigger)
			{
				m_pUnitCB->SelectCombo(GU_CODE);
				ChangeUnit(GU_CODE);
				domino = val;
				domino.TrimRight(" ");
			}

			if (!domino.IsEmpty() && bTrigger)	// trigger
			{
				m_strTrigger.Format("%s\t%s", JTRG, domino);
				SetTimer(triggerTimerID, 10, NULL);
			}
		}
		else if (symbol == ITRG)
		{
			if (m_iDtUnit == GU_INDEX)
			{
				domino = val;
				SetIndexTrigger(domino);
			}
		}
		else if (symbol == FTRG)	// 선물
		{
			if (m_iDtUnit == GU_OPTION || m_iDtUnit == GU_COFUT || m_iDtUnit == GU_COOPT)
			{
				domino = val;
				domino.TrimRight(" ");

				m_iDtUnit = GU_FUTURE;
				m_pCodeCB->SetUnit(m_iDtUnit);
				MakeInputSymbol();

				m_strValidCode.Format("%s", domino);
				m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, unitInput), m_iDtUnit);
			}
			else if (m_iDtUnit == GU_FUTURE)
			{
				domino = val;
				domino.TrimRight(" ");
			}

			else if (m_iOrgUnit == GU_FUTURE && !bTrigger)
			{
				m_pUnitCB->SelectCombo(GU_FUTURE);
				ChangeUnit(GU_FUTURE);
				domino = val;
				domino.TrimRight(" ");
			}

			if (!domino.IsEmpty() && bTrigger)	// trigger
			{
				m_strTrigger.Format("%s\t%s", FOTRG, domino);
				SetTimer(triggerTimerID, 10, NULL);
			}
		}
		else if (symbol == OTRG)	// 옵션
		{
			if (m_iDtUnit == GU_FUTURE || m_iDtUnit == GU_COFUT || m_iDtUnit == GU_COOPT)
			{
				domino = val;
				domino.TrimRight(" ");

				m_iDtUnit = GU_OPTION;
				m_pCodeCB->SetUnit(m_iDtUnit);
				MakeInputSymbol();

				m_strValidCode.Format("%s", domino);
				m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, unitInput), m_iDtUnit);
			}
			else if (m_iDtUnit == GU_OPTION)
			{
				domino = val;
				domino.TrimRight(" ");
			}
			else if (m_iOrgUnit == GU_OPTION && !bTrigger)
			{
				m_pUnitCB->SelectCombo(GU_FUTURE);
				ChangeUnit(GU_OPTION);
				domino = val;
				domino.TrimRight(" ");
			}
			if (!domino.IsEmpty() && bTrigger)	// trigger
			{
				m_strTrigger.Format("%s\t%s", FOTRG, domino);
				SetTimer(triggerTimerID, 10, NULL);
			}
		}
		else if (symbol == CFTRG)	// 주식선물
		{
			if (m_iDtUnit == GU_FUTURE || m_iDtUnit == GU_OPTION || m_iDtUnit == GU_COOPT)
			{
				domino = val;
				domino.TrimRight(" ");

				m_iDtUnit = GU_COFUT;
				m_pCodeCB->SetUnit(m_iDtUnit);
				MakeInputSymbol();

				m_strValidCode.Format("%s", domino);
				m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, unitInput), m_iDtUnit);
			}
			else if (m_iDtUnit == GU_COFUT)
			{
				domino = val;
				domino.TrimRight(" ");
			}
			else if (m_iOrgUnit == GU_FUTURE && !bTrigger)
			{
				m_pUnitCB->SelectCombo(GU_FUTURE);
				ChangeUnit(GU_COFUT);
				domino = val;
				domino.TrimRight(" ");
			}
			if (!domino.IsEmpty() && bTrigger)	// trigger
			{
				m_strTrigger.Format("%s\t%s", FOTRG, domino);
				SetTimer(triggerTimerID, 10, NULL);
			}
		}
		else if (symbol == COTRG)	// 주식옵션
		{
			if (m_iDtUnit == GU_FUTURE || m_iDtUnit == GU_OPTION || m_iDtUnit == GU_COFUT)
			{
				domino = val;
				domino.TrimRight(" ");

				m_iDtUnit = GU_COOPT;
				m_pCodeCB->SetUnit(m_iDtUnit);
				MakeInputSymbol();

				m_strValidCode.Format("%s", domino);
				m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, unitInput), m_iDtUnit);
			}
			else if (m_iDtUnit == GU_COOPT)
			{
				domino = val;
				domino.TrimRight(" ");
			}

			else if (m_iOrgUnit == GU_FUTURE && !bTrigger)
			{
				m_pUnitCB->SelectCombo(GU_FUTURE);
				ChangeUnit(GU_COOPT);
				domino = val;
				domino.TrimRight(" ");
			}

			if (!domino.IsEmpty() && bTrigger)	// trigger
			{
				m_strTrigger.Format("%s\t%s", FOTRG, domino);
				SetTimer(triggerTimerID, 10, NULL);
			}
		}
		else if (symbol == CDFTRG) // 상품 선물 // 20090916 : ADD
		{
			if ((m_iDtUnit == GU_CFUTURE) || (m_iDtUnit == GU_FFUTURE))
			{
				domino = val;
				domino.TrimRight(" ");
			}

		}
		else if (symbol == STRG)	// 해외
		{
			if (m_iDtUnit == GU_FOREIGN)
			{
				domino = val.Mid(1);
				domino.TrimRight(" ");
			}
		}

		if (domino.IsEmpty() && (!bTrigger || !strlen(m_strValidCode)))
		{
			domino = GetDefaultCode();
		}
	}

	rtmStore.RemoveAll();
}

CString CPnInput::GetDefaultCode()
{
	CString	strCode = "";
	switch (m_iDtUnit)
	{
	case GU_CODE:
		strCode = GetHistory(m_iDtUnit);
		break;
	case GU_FOREIGN:
		strCode = GetHistory(m_iDtUnit);
		break;
	case GU_INDEX:
		strCode = GetHistory(m_iDtUnit);
		break;
	default:
		{
			strCode = GetHistory(m_iDtUnit);

			CString	code;
			code.Format("%s", strCode);
			CString	name = (char*)m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgGubnFOSO), long(code.operator LPCTSTR()));
			if (!name.IsEmpty())
			{
				int	focode = WhichFOCode(code.operator LPCTSTR());
				switch (focode)
				{
				case foOption:
					if (m_iDtUnit != GU_OPTION)	
						ChangeUnit(GU_OPTION);	
					break;
				case foCoFut:
					if (m_iDtUnit != GU_COFUT)	
						ChangeUnit(GU_COFUT);	
					break;
				case foCoOpt:
					if (m_iDtUnit != GU_COOPT)	
						ChangeUnit(GU_COOPT);	
					break;
				case foFuture:
				default:
					if (m_iDtUnit != GU_FUTURE)	
						ChangeUnit(GU_FUTURE);	
					break;
				}
			}
		}
		break;
	}

	return strCode;
}

void CPnInput::SetTriggerCode()
{
	bool bFO = false;
	CString	trgSymbol, htySymbol;
	CString code = m_pCodeCB->GetEditData();
	switch(m_iDtUnit)
	{
	case GU_CODE:
		trgSymbol = JTRG;
		htySymbol = JTRG;
		break;
	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
	case GU_FFUTURE: // 20090916 : ADD
	case GU_CFUTURE: // 20090916 : ADD
		switch(WhichFOCode(code))
		{
		case foOption:
			trgSymbol = OTRG;
			htySymbol = OTRG;
			bFO = true;
			break;
		case foCoFut:
			trgSymbol = CFTRG;
			htySymbol = CFTRG;
			bFO = true;
			break;
		case foCoOpt:
			trgSymbol = COTRG;
			htySymbol = COTRG;
			bFO = true;
			break;
		case foCmFut:		// 20090916 : ADD
		case foFmFut:		// 20090918 : ADD : 현물 선물 처리
			trgSymbol = CDFTRG;
			htySymbol = CDFTRG;
			bFO = false;	// 주식 선물 옵션 히스토리에 저장하지 않는다.
			break;
		case foFuture:
		default:
			trgSymbol = FTRG;
			htySymbol = FTRG;
			bFO = true;
			break;		
		}
		break;
	case GU_FOREIGN:
		trgSymbol = STRG;
		htySymbol = STRG;
		break;
	case GU_INDEX:
		trgSymbol = ITRG;
		htySymbol = ITRG;
		break;
	default:
		return;
	}

	if (code == K200_INDEX || code == K200_FUTURE || code == STAR_INDEX || code == STAR_FUTURE)
		return;

	CString	trgStr, htyStr;
	if (m_iDtUnit == GU_FOREIGN)
	{
		trgStr.Format("%s\ts%s", trgSymbol, code);
		htyStr.Format("%s\ts%s", htySymbol, code);
	}
/*	else if (m_iDtUnit == GU_INDEX)
	{
		int	kkind;
		m_pKKindCB->GetData(kkind);

		trgStr.Format("%s\t%02d%s", trgSymbol, kkind, code);
		htyStr.Format("%s\t%02d%s", htySymbol, kkind, code);
	}*/
	else
	{
		trgStr.Format("%s\t%s", trgSymbol, code);
		htyStr.Format("%s\t%s", htySymbol, code);
	}

	m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, setTrigger), long(trgStr.operator LPCTSTR()));
	m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, setHistory), long(htyStr.operator LPCTSTR()));

	if (bFO)	
	{
		htyStr.Format("%s\t%s", FOTRG, code);
		m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, setHistory), long(htyStr.operator LPCTSTR()));
	}
}

CString CPnInput::GetHistory(int unit)
{
	CString	history = _T("");
	CString	symbol = _T("");
	switch (unit)
	{
	case GU_CODE:	
		symbol = JTRG;	break;
	case GU_INDEX:
		symbol = ITRG;	break;
	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
		symbol = FOTRG;	break;
	case GU_FOREIGN:	
		symbol = STRG;	break;
	case GU_FFUTURE:	// 20090916 : ADD
	case GU_CFUTURE:	// 20090915 : ADD
		symbol = CDFTRG;
		break;
	}

	if (unit == GU_FOREIGN) 
	{
		history = m_pCodeCB->GetHistory(symbol);
		if (history.IsEmpty()) 
		{
			history = "DJI@DJI ";
		}
	}
	else {
		history = (char *)m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getHistory), long(symbol.operator LPCTSTR()));
	}


	if (history.IsEmpty())
		return history;

	int	index = history.Find("\t");
	if (index <= 0)	return history;

	CString	Str = history.Left(index);

	index = Str.Find(" ");
	if (index > 0)
	{
		CString	code = Str.Left(index);
		return code;
	}

	return _T("");
}

void CPnInput::ChangeMain(int kind)
{
	if (!m_peditDisp)
		return;

	switch (kind)
	{
	case GK_THREE:
	case GK_PNF:
	case GK_SWING:
	case GK_CLOCK:
	case GK_PV:
	case GK_RENKO:
	case GK_KAGI:
		m_bSeriesChart = FALSE;
		m_peditDisp->EnableWindow(false);
		break;
	default:
		m_bSeriesChart = TRUE;
		m_peditDisp->EnableWindow(true);
		break;
	}
}

void CPnInput::ReloadEnv(char* pcData)
{
	_envInfo *penvinfo = (_envInfo*)pcData;
	ChangeMain(penvinfo->datainfo.wMainGrpKind);

	m_iMGap = penvinfo->datainfo.wMGap;
	m_iTGap = penvinfo->datainfo.wTGap;
	SetGapValue(penvinfo->datainfo.acIGap);

	int iUnit = penvinfo->datainfo.btUnit;
	int iIndex = penvinfo->datainfo.btIndex;
	if (iUnit == GU_OPTION || iUnit == GU_COFUT || iUnit == GU_COOPT)
		iUnit = GU_FUTURE;

	m_pUnitCB->SelectCombo(iUnit);

	ChangeUnit(iUnit);
	ChangeIndex(iIndex, true);

	if (m_iDtUnit == GU_FOREIGN && (m_iDtIndex == GI_MINUTE || m_iDtIndex == GI_TICK))
		ChangeIndex(GI_DAY, true, true, true);
	else if ((m_iDtUnit == GU_FUTURE || m_iDtUnit == GU_OPTION || m_iDtUnit == GU_COFUT || m_iDtUnit == GU_COOPT) 
		&& (m_iDtIndex == GI_WEEK || m_iDtIndex == GI_MONTH))
	{
		ChangeIndex(GI_DAY, true, true, true);
	}

	Invalidate();
	CheckTR();
}

void CPnInput::SetGapValue(char *pGap)
{
	char	reserve[5] = { GAP1, GAP2, GAP3, GAP4, GAP5 };
	for (int ii = 0; ii < 5; ii++)
	{
		if (pGap[ii] <= 0 || pGap[ii] > 90)
			m_aiGap[ii] = reserve[ii];
		else
			m_aiGap[ii] = pGap[ii];
	}
}

void CPnInput::OnClickedToday()
{
	struct _envInfo* penvinfo = (_envInfo*)m_pwndGView->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);
	int iUnit = 0;
	switch (m_iDtUnit)
	{
	case GU_FUTURE:
	case GU_OPTION:
	case GU_COFUT:
	case GU_COOPT:
	case GU_CFUTURE: // 20090918 : ADD :
	case GU_FFUTURE: // 20090918 : ADD :
		iUnit = GU_FUTURE - GU_CODE;
		break;
	case GU_CODE:
	case GU_INDEX:	
		iUnit = m_iDtUnit - GU_CODE;
		break;
	case GU_FOREIGN:
		iUnit = 3;
		break;
	default:
		break;
	}

	switch (m_iDtIndex)
	{
	case GI_MINUTE:
	case GI_TICK:
		penvinfo->datainfo.cToday = m_pchbToday->GetCheck();
		break;
	}

	// 2007.01.03 schbang : 항상 입력가능하게
//	if (m_pchbToday->GetCheck())
//		m_peditDisp->GetWindowText(m_savedDisp);
//	else {
//		m_peditDisp->SetWindowText(m_savedDisp);
//		m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, dispDayInput), atoi(m_savedDisp));
//	}
	SendRequest(FALSE);
}

// 20090904 : CHANCE
//** macho add
//const char *units[] = { "주식", "업종", "선옵", "해외" };
const char *units[] = {"주식", "업종", "선옵", "해외", "상품"};

CString CPnInput::GetCurrentUnit()
{
	int index = m_pUnitCB->GetCurSel();

	if (index >= 0 && index < (sizeof(units) / sizeof(units[0])))
		return CString(units[index]);
	else {
		return CString("");
	}
}

void CPnInput::DrawUnitButton(CDC* dc, CRect rc)
{
	dc->SetBkMode(TRANSPARENT);
	dc->DrawText(GetCurrentUnit(), -1, rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
}

void CPnInput::TrackingUnitMenu(CPoint pt)
{
	CMenu menu;

	int index = m_pUnitCB->GetCurSel();

	menu.CreatePopupMenu();

	for (int i = 0; i < (sizeof(units) / sizeof(units[0])); i++) {
		menu.AppendMenu(MF_STRING | (i == index ? MF_CHECKED: 0), i + 1, units[i]);
	}

	CWnd* wnd = NULL;//GetDlgItem(BTN_UNIT);
	if (wnd) {
		CRect rc;
		wnd->GetWindowRect(rc);
		pt.x = rc.left;// - rc.Width();
		pt.y = rc.bottom ;
	}
	else {
		ClientToScreen(&pt);
	}
	

	int choice = -1 + menu.TrackPopupMenu(TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, this, NULL);

	if (choice >= 0 && choice < (sizeof(units) / sizeof(units[0]))) 
	{
		if (choice != m_pUnitCB->GetCurSel()) 
		{
			m_pUnitCB->SetCurSel(choice);
			int unit = FindUnit(choice);
			ChangeUnit(unit);
			OnChangeUnit();
		}
	}
}

//** CUnit array referenced from PnWndCombo.cpp
// 20090915 : CHANGE
//extern struct _combo CUnit[4];
extern struct _combo CUnit[5];

int CPnInput::FindUnit(int index)
{
	return CUnit[index].val;
}

void CPnInput::OnChangeUnit()
{
	if (m_iDtUnit == GU_FOREIGN && (m_iDtIndex == GI_MINUTE || m_iDtIndex == GI_TICK))
		ChangeIndex(GI_DAY, true, true, true);
	else if ((m_iDtUnit == GU_FUTURE || m_iDtUnit == GU_OPTION || m_iDtUnit == GU_COFUT || m_iDtUnit == GU_COOPT)
		&& (m_iDtIndex == GI_WEEK || m_iDtIndex == GI_MONTH))
		ChangeIndex(GI_DAY, true, true, true);

	if (!m_lock) 
	{
		SendRequest();
	}
}

CString CPnInput::GetIndexCode(int indexCode)
{
	CString format, code;
	format.Format("%%0%dd", sz_INDEX);
	code.Format(format, indexCode);
	return code;			
}

void CPnInput::ChangeUnitAndCode(LPCTSTR param)
{
	CString str(param);

	int find = str.Find(';');
	if (find < 0) return;

	int choice, unit = atoi(str.Left(find));
	CString code = str.Right(str.GetLength() - (find + 1));

	code.TrimRight();
	if (unit != 1 && unit != 2)
	{
		if (code.IsEmpty())
			return;

		char	pCode[128];
		wsprintf(pCode, _T("%s"), code.GetString());
		CString	name = (char*)m_pwndView->SendMessage(WM_USER, nameDLL, long(pCode));
		if (name.IsEmpty())
			return;
	}

	if (code.GetLength() == 3 && atoi(code) > 0) 
		choice = 1;
	else if (code.GetLength() == 6 && atoi(code) > 0) 
		choice = 0;
	else if (code.GetLength() == 8 && atoi(code.Left(3)) > 0) 
		choice = 2;
	else 
		choice = 3;

	unit = FindUnit(choice);
	if (choice >= 0 && choice < (sizeof(units) / sizeof(units[0]))) 
	{
		if (choice != m_pUnitCB->GetCurSel()) 
		{
			m_pUnitCB->SetCurSel(choice);
			int unit = FindUnit(choice);
			ChangeUnit(unit);
			m_lock = TRUE;
			OnChangeUnit();
			m_lock = FALSE;
		}
	}

	m_pCodeCB->SetEditData(code);
	m_pCodeCB->SetFocus();

	SendRequest();
}

BOOL CPnInput::SendRequest(BOOL bTrigger)
{
	if (!CheckTR()) return FALSE;

	BOOL result = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, requestTR));
	
	// debug : 20090907
	//CString strDebug;
	//strDebug.Format("CPnInput::SendRequest() : m_pwndGView->SendMessage = %d", result);
	//AfxMessageBox(strDebug);

	if (/**result && **/bTrigger)
		SetTriggerCode();

	return result;
}

int CPnInput::WhichFOCode(LPCTSTR code)
{
	CString cd(code);
	if (cd.GetLength() != 8) return foNone;

	if (cd != K200_INDEX && cd != K200_FUTURE && cd != STAR_INDEX && cd != STAR_FUTURE)
	{
		if ((cd[0] == '2' || cd[0] == '3' || cd[0] == 'B' || cd[0] == 'C'))   //파생상품 코드개편
		{
			return foOption;
		}
		else if (cd[0] == '1' || cd[0] == 'A')	// 1xxxxxxx //파생상품 코드개편
		{
			if ((cd[1] >= '1') && (cd[1] < '6'))	// 11xxxxxx ~ 15xxxxxx : 주식 선물 옵션
				return foCoFut;
			else if ((cd[1] >= '6') && (cd[1] < '8'))
				return foCmFut;						// 16xxxxxx ~ 17xxxxxx : 채권, 금리 선물 // 20090916 : ADD
			else if (cd[1] >= '8')
				return foFmFut;						// 18xxxxxx : 상품선물(돈육, 금 선물)
			else if (cd[2] == '3')
				return foStar;

			return foFuture;
		}
		else	// "4xxxxxxx"	spread
		{
			if (cd[1] < '6')
				return foFuture;					// 46xxxxxx ~ 48xxxxxx : 상품 선물
			else
				return foCmFut;				
		}
	}

	return foFuture;
}

CString CPnInput::GetSiseName()
{
	CString name = (LPCTSTR)
		m_pwndGView->SendMessage(GEV_SISE, MAKEWPARAM(siseData, siseName));

	return name;
}

void CPnInput::SetIndexCode(CString& code)
{
	code = GetIndexCode(1);

	DWORD kind = atoi(code);

	SetIndexKind(code);
}


void CPnInput::SetIndexTrigger(CString& domino)
{
	domino = domino.Right(sz_INDEX);

	int	kkind = atoi(domino);

	m_pCodeCB->SetKkind(kkind);
	SetIndexKind(domino);
}

void CPnInput::SetIndexKind(LPCTSTR code)
{
	DWORD kind = atoi(code);
	for (int i = 0; i < m_pKKindCB->GetCount(); i++) 
	{
		if (m_pKKindCB->GetItemData(i) == kind) 
		{
			m_pKKindCB->SetCurSel(i);
			return;
		}
	}
	m_pKKindCB->SetCurSel(CB_ERR);
}
