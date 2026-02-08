// PnInput.cpp : implementation file
//

#include "stdafx.h"
#include "C_Total.h"
#include "PnInput.h"
#include "PnWndCombo.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPnInput
struct _info	infoInput[] = {
	{ STC_JNAME,	0, 0, 0, 0,	"", "" },
	{ BTN_DDAY,	0, 1, 2, 0,	"", "" },
	{ BTN_DWEEK,	0, 1, 2, 0,	"", "" },
	{ BTN_DMON,	0, 1, 2, 0,	"", "" },
	{ BTN_DMIN,	0, 1, 2, 0,	"", "" },
	{ BTN_DTICK,	0, 1, 2, 0,	"", "" },

	{ BTN_TIME1,	0, 1, 2, 0,	"", "" },
	{ BTN_TIME2,	0, 1, 2, 0,	"", "" },
	{ BTN_TIME3,	0, 1, 2, 0,	"", "" },
	{ BTN_TIME4,	0, 1, 2, 0,	"", "" },
	{ CMB_TIME,	0, 0, 0, 0,	"", "" },
	
	{ STC_GIGAN,	0, 0, 0, 0,	"", "" },
	{ EDT_GIGAN,	0, 0, 0, 0,	"", "" },
	{ BTN_MENU,	0, 1, 2, 0,	"환경설정", "환경설정" },
};

#define	GAP1	1
#define	GAP2	5
#define	GAP3	15
#define	GAP4	30

CPnInput::CPnInput(CWnd *pView, CWnd* pParent, CFont* pFont, int ctrlID, char* info)
: CPnBaseWnd(pView, pParent, pFont, ctrlID, info)
{
	m_pNTimeImage = NULL;
	m_pIOptImage = NULL;

	m_pGapCB = NULL;
	m_pGiganET = NULL;
	m_pToolTip = NULL;

	m_pMainWnd = (CMainWnd *)pParent;
	m_pApp = (CC_TotalApp *)AfxGetApp();
	m_pBtnFont = m_pApp->GetFont(pView, 9, "돋움");

	m_LastRight = 0;
	m_bInitResize = true;
	m_bMouseIN = FALSE;
	m_bLButton = false;
	m_moveID = 0;
	m_selID = -1;

	struct _envInfo	*envinfo = (struct _envInfo*)info;
	SetGapValue(envinfo->datainfo.acIGap);

	if (envinfo->wEnvOption & EO_JNAME)
		m_bNameDisplay = true;
	else
		m_bNameDisplay = false;

	if (envinfo->wEnvOption & EO_SINPUT)
		m_bMiniInput = true;
	else
		m_bMiniInput = false;

	m_rcSelGap.SetRectEmpty();
}

CPnInput::~CPnInput()
{
}


BEGIN_MESSAGE_MAP(CPnInput, CPnBaseWnd)
	//{{AFX_MSG_MAP(CPnInput)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(GEV_INPUT, OnInputEvent)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPnInput message handlers
LRESULT CPnInput::OnInputEvent(WPARAM wParam, LPARAM lParam)
{
	CString	tmpS;
	switch (LOWORD(wParam))
	{
	case INPUT_GET_POS_COMMENT:
		m_comment = GetPosComment(CPoint(LOWORD(lParam), HIWORD(lParam)));
		return (LRESULT)(LPCTSTR)m_comment;
	case INPUT_CHANGE:
		switch (HIWORD(wParam))
		{
		case ipUnit:
			MakeInputSymbol();
			if (m_pMainWnd->m_iDtUnit == GU_FOREIGN && 
				(m_pMainWnd->m_iDtIndex == GI_MINUTE || m_pMainWnd->m_iDtIndex == GI_TICK))
				ChangeIndex(GI_DAY, true, true, true);
			else if ((m_pMainWnd->m_iDtUnit == GU_FUTURE || m_pMainWnd->m_iDtUnit == GU_OPTION || m_pMainWnd->m_iDtUnit == GU_COFUT || 
				m_pMainWnd->m_iDtUnit == GU_COOPT || m_pMainWnd->m_iDtUnit == GU_CFUTURE || m_pMainWnd->m_iDtUnit == GU_FFUTURE) && 
				(m_pMainWnd->m_iDtIndex == GI_WEEK || m_pMainWnd->m_iDtIndex == GI_MONTH))
				ChangeIndex(GI_DAY, true, true, true);

			break;
		case ipGap:	// time button change
			ChangeGap(m_pMainWnd->m_iDtIndex, lParam);
			if (!m_pMainWnd->m_strCode.IsEmpty())
				m_pParent->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, requestTR), 0);
			break;
		case IP_GIGAN:
			if (!m_pMainWnd->m_strCode.IsEmpty())
			{
				m_pGiganET->GetWindowText(tmpS);
				if (atoi(tmpS) > 0)
				{
					m_pParent->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartTotal), atoi(tmpS));
					m_pParent->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, requestTR), 0);
				}
			}
			break;
		case INPUT_BUTTON:
			if (!m_pMainWnd->m_strCode.IsEmpty())
				m_pParent->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, requestTR), 0);
			break;
		case ipOutSide:
			ChangeFromOutside(LOWORD(lParam), HIWORD(lParam));
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
			{
				m_pGiganET->GetWindowText(tmpS);
				return atoi(tmpS);
			}
			break;
		case getDispDay:
			{
				struct _envInfo	*envinfo = (_envInfo*)m_pParent->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);

				int	uIndex = 0;
				switch (m_pMainWnd->m_iDtUnit)
				{
				case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:	case GU_CFUTURE:	case GU_FFUTURE:
					uIndex = GU_FUTURE - GU_CODE;
					break;
				case GU_CODE:	case GU_INDEX:	
					uIndex = m_pMainWnd->m_iDtUnit - GU_CODE;
					break;
				case GU_FOREIGN:
					uIndex = 3;
					break;
				default:
					break;
				}
				return envinfo->datainfo.aaDayInfo[uIndex][m_pMainWnd->m_iDtIndex-GI_DAY].dwDisCnt;
			}
			break;
		case getCalendar:
			sprintf((char*)lParam, "%s", m_pMainWnd->m_strDay);
			break;
		case getCurGapList:
			{
				char	*pVal = (char*)lParam;
				for (int ii = 0; ii < 4; ii++)	pVal[ii] = m_gap[ii];
			}
			break;
		case setCodeInfo:	break;
		case domminoCode:	break;
		case triggerCode:	break;
		case isDollarData:	return FALSE;
		case isTodayData:	return FALSE;
		case GET_VALID_CODE:	return (long)m_pMainWnd->m_strCode.operator LPCTSTR();
		}
		break;
	case inputEnv:		break;
	case inputViewFocus:	break;
	case inputChgPalette:
		m_clrBack = m_pParent->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 66);
		Invalidate();
		break;
	default:		break;
	}

	return FALSE;
}

LRESULT CPnInput::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	//if (message == WM_MOUSELEAVE)
	//{
	//	OnMouseLeave(0,CPoint(0,0));
	//	return TRUE;
	//}

	if (m_pToolTip)
		m_pToolTip->RelayEvent(message);	
	
	return CPnBaseWnd::WindowProc(message, wParam, lParam);
}

int CPnInput::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPnBaseWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ModifyStyleEx(0, WS_EX_CONTROLPARENT);

	m_clrBack = m_pParent->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 66);
	m_clrFrame = PALETTERGB(193, 213, 234); 

	CString tmps;
	COLORREF bg, fg;
	tmps.Format("%s/%s/%s", ((CC_TotalApp *)AfxGetApp())->GetRoot(), TABDIR, PALETTE);
	bg = GetPrivateProfileInt(GENERALSN, "ComboBg", -1, tmps);
	fg = GetPrivateProfileInt(GENERALSN, "ComboFg", -1, tmps);
	bg = m_pParent->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), bg);
	fg = m_pParent->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), fg);

	int	cmbOption = WS_VISIBLE|WS_CHILD|WS_VSCROLL|CBS_DROPDOWN;
	m_pGapCB = new CPnWndCombo(this, m_pParent, CMB_TIME);
	m_pGapCB->Create(cmbOption, CRect(0, 0, 10, 10), this, 5);
	m_pGapCB->SetComboPalette(bg, fg);
	m_pGapCB->SetFont(m_pFont);
	m_pGapCB->AddString("1");	m_pGapCB->AddString("2");
	m_pGapCB->AddString("5");	m_pGapCB->AddString("10");
	m_pGapCB->AddString("15");	m_pGapCB->AddString("30");
	m_pGapCB->AddString("60");	m_pGapCB->AddString("90");
	m_pGapCB->AddString("120");

	m_pGiganET = new CPnWndEdit(this, m_pParent, EDT_GIGAN);
	m_pGiganET->CreateEx(WS_EX_CLIENTEDGE, _T("Edit"), "", 
		WS_VISIBLE|WS_CHILD|ES_NUMBER, CRect(0,0,10,10), this, 7);
	m_pGiganET->SetFont(m_pFont);
	//m_pGiganET->SetLimitText(4);
	//modi 기간입력은 3자리까지
	m_pGiganET->SetLimitText(3);

	m_pToolTip = new CPnToolTip(m_pView, this, GEV_INPUT);
	m_pToolTip->Create(m_pFont);
	
	CString	path = m_pApp->GetImagePath(m_pParent, NTIMEIMAGE);
	m_pNTimeImage = m_pApp->GetImage(imgNTime, path);
	path = m_pApp->GetImagePath(m_pParent, IOPTIMAGE);
	m_pIOptImage = m_pApp->GetImage(imgIOpt, path);

	CDC	*pDC = GetDC();
	CFont	*sFont = pDC->SelectObject(m_pFont);
	m_szHangul = pDC->GetOutputTextExtent("한");
	m_szNumeric = pDC->GetOutputTextExtent("9");
	pDC->SelectObject(sFont);
	ReleaseDC(pDC);

	MakeInputSymbol();

	return 0;
}

void CPnInput::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	OnDraw(&dc);
}

void CPnInput::OnDraw(CDC *pDC)
{
	DrawBackground(pDC);

	CFont		*sFont = pDC->SelectObject(m_pFont);
	COLORREF	sColor = pDC->SetTextColor(RGB(0,0,0));
	COLORREF	sbkColor = pDC->SetBkColor(m_clrBack);

	for (int ii = 0; ii < m_arComs.GetSize(); ii++)
		drawButton(pDC, m_arComs.GetAt(ii));
	if (m_bNameDisplay)
		pDC->DrawText(m_pMainWnd->m_strJName, m_pMainWnd->m_strJName.GetLength(), &m_rcJNAME, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	if (!m_bMiniInput)
		pDC->DrawText("기간", strlen("기간"), &m_rcGigan, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	pDC->SetBkColor(sbkColor);
	pDC->SetTextColor(sColor);
	pDC->SelectObject(sFont);
}

void CPnInput::DrawButton(CDC* pDC, struct _comp *comp)
{
	CRect cRC = comp->cRC;
	CPoint imgPt = CPoint(cRC.left, cRC.top);

	CString	tmpS = _T("");

	switch (comp->cID)
	{
	case BTN_DDAY:	tmpS.Format("일");	break;
	case BTN_DWEEK:	tmpS.Format("주");	break;
	case BTN_DMON:	tmpS.Format("월");	break;
	case BTN_DMIN:	tmpS.Format("분");	break;
	case BTN_DTICK:	tmpS.Format("틱");	break;
	case BTN_TIME1:	tmpS.Format("%d", m_gap[0]);	break;
	case BTN_TIME2:	tmpS.Format("%d", m_gap[1]);	break;
	case BTN_TIME3:	tmpS.Format("%d", m_gap[2]);	break;
	case BTN_TIME4:	tmpS.Format("%d", m_gap[3]);	break;
	default:
		break;
	}

	CPnBaseWnd::DrawButton(pDC, dtButton, comp, m_pNTimeImage);

	cRC.DeflateRect(1, 1, 1, 1);
	int mode = pDC->SetBkMode(TRANSPARENT);
	CFont	*sFont = pDC->SelectObject(m_pBtnFont);
	if (!tmpS.IsEmpty())
		pDC->DrawText(tmpS, cRC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	pDC->SetBkMode(mode);
	pDC->SelectObject(sFont);
}

void CPnInput::drawButton(CDC* pDC, struct _comp* comp)
{
	if (!comp->bshow)
		return;

	switch (comp->cID)
	{
	case BTN_DDAY:	case BTN_DWEEK:	case BTN_DMON:	case BTN_DMIN:	case BTN_DTICK:
	case BTN_TIME1:	case BTN_TIME2:	case BTN_TIME3:	case BTN_TIME4:
		DrawButton(pDC, comp);	break;
	case BTN_MENU:
		if (!m_pMainWnd->m_bHideMenuBtn)
			CPnBaseWnd::DrawButton(pDC, dtButton, comp, m_pIOptImage);	break;
	}
}

void CPnInput::OnDestroy() 
{
	if (m_pGapCB)		WND_DELETE(m_pGapCB);
	if (m_pGiganET)		WND_DELETE(m_pGiganET);
	if (m_pToolTip)		WND_DELETE(m_pToolTip);

	CPnBaseWnd::OnDestroy();
}

void CPnInput::OnSize(UINT nType, int cx, int cy) 
{
	CPnBaseWnd::OnSize(nType, cx, cy);
	
	if (cx == 0 && cy == 0)
		return;

	if (m_bInitResize)
	{
		m_bInitResize = false;
		SetInfo(CRect(0, 0, cx, cy));
		arrangeCtrl(CRect(0, 0, cx, cy));

		ChangeIndex(m_pMainWnd->m_iDtIndex);
		ReadDisplay(m_pMainWnd->m_iDtIndex);

		if (m_pMainWnd->m_iDtUnit == GU_FOREIGN && (m_pMainWnd->m_iDtIndex == GI_MINUTE || m_pMainWnd->m_iDtIndex == GI_TICK))
			ChangeIndex(GI_DAY, true, true, true);
		else if ((m_pMainWnd->m_iDtUnit == GU_FUTURE || m_pMainWnd->m_iDtUnit == GU_OPTION || 
			m_pMainWnd->m_iDtUnit == GU_COFUT || m_pMainWnd->m_iDtUnit == GU_COOPT ||
			m_pMainWnd->m_iDtUnit == GU_CFUTURE || m_pMainWnd->m_iDtUnit == GU_FFUTURE) && 
			(m_pMainWnd->m_iDtIndex == GI_WEEK || m_pMainWnd->m_iDtIndex == GI_MONTH))
			ChangeIndex(GI_DAY, true, true, true);
	}

	arrangeCtrl(CRect(0, 0, cx, cy), false);	
}

void CPnInput::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPnBaseWnd::OnLButtonDown(nFlags, point);

	m_pParent->SendMessage(GEV_VIEW, MAKEWPARAM(viewMulti, activeView), 1);

	struct _comp* comp = SearchCom(point);
	if (!comp)
	{
		return;
	}
	m_bLButton = true;
	m_selID = comp->cID;

	InvalidateCom(comp, btnDown);
}

void CPnInput::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CPnBaseWnd::OnLButtonUp(nFlags, point);
	lbuttonup(point);
}

void CPnInput::lbuttonup(CPoint point)
{
	if (!m_bLButton)
		return;

	m_moveID = -1;
	m_bLButton = false;

	struct _comp* pOldCom = SearchCom(m_selID);
	struct _comp* comp = SearchCom(point);

	switch (m_selID)
	{
	case BTN_DDAY:	case BTN_DWEEK:	case BTN_DMON:	case BTN_DMIN:	case BTN_DTICK:
	case BTN_TIME1:	case BTN_TIME2:	case BTN_TIME3:	case BTN_TIME4:
		break;
	default:
		if (!comp || m_selID != comp->cID || !comp->bshow)
		{
			if (pOldCom)
				InvalidateCom(pOldCom, btnUp);

			m_selID = -1;
			return;
		}
	}
	
	if (pOldCom)
		InvalidateCom(pOldCom, btnUp);

	int	index = -1;
	int	gap = -1;
	switch (m_selID)
	{// process etc button
	case BTN_DDAY:	index = GI_DAY;		break;
	case BTN_DWEEK:	index = GI_WEEK;	break;
	case BTN_DMON:	index = GI_MONTH;	break;
	case BTN_DMIN:	index = GI_MINUTE;	break;
	case BTN_DTICK:	index = GI_TICK;	break;

	case BTN_TIME1:	gap = m_gap[0];		break;
	case BTN_TIME2:	gap = m_gap[1];		break;
	case BTN_TIME3:	gap = m_gap[2];		break;
	case BTN_TIME4:	gap = m_gap[3];		break;

	case BTN_MENU:
		if (!m_pMainWnd->m_bHideMenuBtn)
			m_pParent->SendMessage(GEV_VIEW, MAKEWPARAM(viewDialog, dlgEnv), 0);
		break;
	default:	break;
	}
	
	if (index > 0)
	{
		ChangeIndex(index);
		SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, INPUT_BUTTON), m_selID);
	}
	else if (gap > 0)
	{
		switch(m_pMainWnd->m_iDtIndex)
		{
		case GI_DAY:	case GI_WEEK:	case GI_MONTH:
			ChangeIndex(GI_MINUTE, false);	m_pGapCB->ShowWindow(SW_SHOW);	break;
		default:
			break;
		}

		ChangeGap(m_pMainWnd->m_iDtIndex, gap);
		SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, INPUT_BUTTON), m_selID);
	}
}

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

	if (m_bLButton)
		return;

	CPoint	Orgpoint = point;
	struct _comp* mcomp = SearchCom(Orgpoint);
	if (!mcomp)
	{
		mcomp = SearchCom(m_moveID);
		if (mcomp)
			InvalidateCom(mcomp, btnUp);
		m_moveID = -1;

		return;
	}

	switch (mcomp->cID)
	{
	case BTN_MENU:
		break;
	case BTN_DDAY:	case BTN_DWEEK:	case BTN_DMON:	case BTN_DMIN:	case BTN_DTICK:
	case BTN_TIME1:	case BTN_TIME2:	case BTN_TIME3:	case BTN_TIME4:
		if (mcomp->status == btnDown)
		{
			mcomp = SearchCom(m_moveID);
			if (mcomp)
				InvalidateCom(mcomp, btnUp);
			m_moveID = -1;

			return;
		}
		break;
	default:
		mcomp = SearchCom(m_moveID);
		if (mcomp)
			InvalidateCom(mcomp, btnUp);
		m_moveID = -1;

		return;
	}

	if (m_moveID == mcomp->cID)
	{
		return;
	}

	struct _comp* ocomp = SearchCom(m_moveID);
	if (ocomp)
		InvalidateCom(ocomp, btnUp);

	m_moveID = mcomp->cID;
	InvalidateCom(mcomp, btnMove);
}

LRESULT CPnInput::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	GetCursorPos(&point);
	if (m_bLButton)
	{
		lbuttonup(point);
	}
	else
	{
		struct _comp* mcomp = SearchCom(m_moveID);
		if (mcomp)
			InvalidateCom(mcomp, btnUp);
	}
	m_moveID = -1;
	m_bMouseIN = FALSE;

	if (m_pToolTip)
	{
		m_pToolTip->m_dispStr = _T("");
		m_pToolTip->hideToolTip();
	}
	return 0;
}

void CPnInput::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CPnBaseWnd::OnRButtonUp(nFlags, point);
	m_pParent->SendMessage(GEV_VIEW, MAKEWPARAM(viewMulti, activeView), 1);

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
				m_gap[index] = ret;
				if (comp->status != btnDown)
				{
					InvalidateCom(comp, btnUp);
				}
				else
				{
					InvalidateCom(comp, comp->status);
					ChangeGap(m_pMainWnd->m_iDtIndex, ret);
					SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, INPUT_BUTTON), comp->cID);
				}
			}
		}
	}
}

void CPnInput::SetInfo(CRect wRC)
{
	int	size = sizeof(infoInput) / sz_INFO;

	wRC.DeflateRect(1, 1);
	int	left = wRC.left;
	int	top = wRC.top;
	int	bottom = wRC.bottom;
	int	Height = wRC.Height() - 2;

	CRect rcUnit; //** macho

	m_rcIndex.SetRect(left, top, left, bottom);
	for (int ii = 0; ii < size; ii++)
	{
		_comp* comp = new _comp;
		ZeroMemory(comp, sz_COMP);

		comp->cID = infoInput[ii].kind;
		comp->status = btnUp;
		comp->bshow = true;
		comp->imggen = infoInput[ii].imggen;
		comp->imgmov = infoInput[ii].imgmov;
		comp->imgsel = infoInput[ii].imgsel;
		comp->imgdis = infoInput[ii].imgdis;
		CopyMemory(comp->str, infoInput[ii].disp, sizeof(infoInput[ii].disp));
		CopyMemory(comp->comment, infoInput[ii].com, sizeof(infoInput[ii].com));

		switch(comp->cID)
		{
		case STC_JNAME:
			if (m_bNameDisplay)
			{
				comp->cRC.SetRect(left, top, left+m_szHangul.cx*6, bottom);
				left += m_szHangul.cx*6 + 3;
				m_rcJNAME = comp->cRC;
				break;
			}

			delete comp;
			continue;
		case BTN_DDAY:
			m_rcIndex.left = left;
			comp->cRC.SetRect(left, top+1, left+szDateImgX, bottom-1);
			left += szDateImgX;
			m_rcDAY = comp->cRC;
			break;
		case BTN_DMON:
			comp->cRC.SetRect(left, top+1, left+szDateImgX, bottom-1);
			left += szDateImgX;
			m_rcMON = comp->cRC;
			break;
		case BTN_DWEEK:
		case BTN_DMIN:
			comp->cRC.SetRect(left, top+1, left+szDateImgX, bottom-1);
			left += szDateImgX;
			break;
		case BTN_DTICK:
			comp->cRC.SetRect(left, top+1, left+szDateImgX, bottom-1);
			left += szDateImgX + 1;
			m_rcIndex.right = left - 1;
			break;

		case CMB_TIME:
			comp->cRC.SetRect(left, top, left+m_szNumeric.cx*3+8+Height, bottom+100);
			left += m_szNumeric.cx*3+8+Height + 1;
			break;

		case BTN_TIME1:
		case BTN_TIME2:
		case BTN_TIME3:
			comp->cRC.SetRect(left, top+1, left+szTimeImgX, bottom-1);
			left += szTimeImgX;
			break;
		case BTN_TIME4:
			if (m_bMiniInput)
			{
				delete comp;
				continue;
			}
			comp->cRC.SetRect(left, top+1, left+szTimeImgX, bottom-1);
			left += szTimeImgX + 1;
			break;
		case STC_GIGAN:
			if (m_bMiniInput)
			{
				delete comp;
				continue;
			}
			comp->cRC.SetRect(left, top, left+m_szHangul.cx*2, bottom);
			left += m_szHangul.cx*2 + 3;
			m_rcGigan = comp->cRC;
			break;
		case EDT_GIGAN:
			if (m_bMiniInput)	left += 5;

			comp->cRC.SetRect(left, top+1, left+m_szNumeric.cx*5, bottom-1);
			left += m_szNumeric.cx*5 + 3;
			m_LastRight = left;
			break;
		case BTN_MENU:
			if (m_bMiniInput)
			{
				delete comp;
				continue;
			}
			comp->cRC.SetRect(left, top+1, left+szIOptImgX, bottom-1);
			left += szIOptImgX + 1;
			break;
		default:
			delete comp;
			continue;
		}
		m_arComs.Add(comp);
	}
}

void CPnInput::arrangeCtrl(CRect rectC, bool bChgIndex)
{
	bool	bJHide, bSHide, bFHide;
	if (m_pMainWnd->m_iDtUnit == GU_CODE)	bJHide = false;
	else			bJHide = true;
	if (m_pMainWnd->m_iDtUnit == GU_FOREIGN)	bSHide = true;
	else			bSHide = false;
	if (m_pMainWnd->m_iDtUnit == GU_FUTURE || m_pMainWnd->m_iDtUnit == GU_OPTION || m_pMainWnd->m_iDtUnit == GU_COFUT || 
		m_pMainWnd->m_iDtUnit == GU_COOPT || m_pMainWnd->m_iDtUnit == GU_CFUTURE || m_pMainWnd->m_iDtUnit == GU_FFUTURE)
		bFHide = true;
	else	
		bFHide = false;

	struct _comp	*pcomp;
	rectC.DeflateRect(1, 1);
	int iLeft = rectC.right - (szIOptImgX * 1 + 2);

	int iGap;
	int iPos = 0;
	for (int ii = BTN_CFG; ii <= BTN_SAVE; ii++)
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
		case CMB_TIME:
		case BTN_TIME1:
			iGap = 1;
		case BTN_DMIN:
		case BTN_DTICK:
		//case BTN_DOLLAR:
		case BTN_TIME2:
		case BTN_TIME3:
		case BTN_TIME4:
		//case BTN_TIME5:
			pcomp->bshow = !bSHide;
			break;
		case BTN_DDAY:
		case STC_GIGAN:
		case EDT_GIGAN:
			iGap = 4;
			break;
		case BTN_MENU:
			pcomp->cRC.left = iLeft;
			pcomp->cRC.right = iLeft + szIOptImgX;
			iLeft += szIOptImgX + 1;
			continue;
		default:
			break;
		}

		pcomp->cRC.OffsetRect(iPos - pcomp->cRC.left + iGap, 0);
		if (pcomp->bshow && pcomp->cID != CALENDAR)
			iPos += pcomp->cRC.Width() + iGap;

		switch (pcomp->cID)
		{
		case CMB_TIME:
			m_pGapCB->MoveWindow(pcomp->cRC);
			break;
		case BTN_DDAY:
			m_rcIndex.left = pcomp->cRC.left;
			break;
		case BTN_DTICK:
			m_rcIndex.right = pcomp->cRC.right;
			break;
		case EDT_GIGAN:
			m_pGiganET->MoveWindow(pcomp->cRC);
			break;
		case STC_GIGAN:
			m_rcGigan = pcomp->cRC;
		default:
			break;
		}
	}
	if (m_pGapCB && !m_pGapCB->IsFocusWnd())
		ChangeIndex(m_pMainWnd->m_iDtIndex, true, false, bChgIndex);	// caret 때문에..
}

int CPnInput::GetTRInfo(char *pHeader)
{
	CString	codeS = _T("");
	switch (m_pMainWnd->m_iDtUnit)
	{
	case GU_CODE:	
		codeS = IJ_CODE;	break;
	case GU_INDEX:	
		codeS = II_CODE;	break;
	case GU_FUTURE:	
		codeS = IF_CODE;	break;
	case GU_OPTION:	
		codeS = IO_CODE;	break;
	case GU_FOREIGN: 
		codeS = IS_CODE;	break;
	case GU_COFUT:
		codeS = ICF_CODE;	break;
	case GU_COOPT:	
		codeS = ICO_CODE;	break; 
	case GU_CFUTURE:
	case GU_FFUTURE:
		codeS = IPF_CODE; break;
	default:	
		return 0;
	}
	sprintf(pHeader, "%s%c%s\t%s", codeS, 0x7F, m_pMainWnd->m_strCode, m_outSymbol);

	return strlen(pHeader);
}

int CPnInput::GetTRInfo2(char *pHeader)
{
	CString	codeS = _T("");
	switch (m_pMainWnd->m_iDtUnit)
	{
	case GU_CODE:	
		codeS = IJ_CODE;	break;
	case GU_INDEX:	
		codeS = II_CODE;	break;
	case GU_FUTURE:	
		codeS = IF_CODE;	break;
	case GU_OPTION:	
		codeS = IO_CODE;	break;
	case GU_FOREIGN: 
		codeS = IS_CODE;	break;
	case GU_COFUT:
		codeS = ICF_CODE;	break;
	case GU_COOPT:	
		codeS = ICO_CODE;	break; 
	case GU_CFUTURE:
	case GU_FFUTURE:
		codeS = IPF_CODE; break;
	default:	
		return 0;
	}
	sprintf(pHeader, "%s%c%s\t", codeS, 0x7F, m_pMainWnd->m_strCode);

	return strlen(pHeader);
}

void CPnInput::MakeInputSymbol()
{
	switch (m_pMainWnd->m_iDtUnit)
	{
	case GU_CODE:
		m_outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OJ_TIME, OJ_NAME, OJ_JGGA, OJ_DIFF, OJ_RATE, OJ_GVOL, OJ_MDHG, OJ_MSHG, 
			OJ_SIGA, OJ_KOGA, OJ_JEGA,
			OJ_CODE, OJ_CDAY, OJ_SAHG, OJ_HAHG, OJ_JJGA, OJ_STCK, OJ_START);
		break;
	case GU_INDEX:
		m_outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OI_TIME, OI_NAME, OI_JGGA, OI_DIFF, OI_RATE, OI_GVOL, OI_MDHG, OI_MSHG, 
			OI_SIGA, OI_KOGA, OI_JEGA,
			OI_CODE, OI_CDAY, OI_SAHG, OI_HAHG, OI_JJGA, OI_STCK, OI_START);
		break;
	case GU_FUTURE:
		m_outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OF_TIME, OF_NAME, OF_JGGA, OF_DIFF, OF_RATE, OF_GVOL, OF_MDHG, OF_MSHG, 
			OF_SIGA, OF_KOGA, OF_JEGA, 
			OF_CODE, OF_CDAY, OF_SAHG, OF_HAHG, OF_JJGA, OF_STCK, OF_START);
		break;
	case GU_OPTION:
		m_outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OO_TIME, OO_NAME, OO_JGGA, OO_DIFF, OO_RATE, OO_GVOL, OO_MDHG, OO_MSHG, 
			OO_SIGA, OO_KOGA, OO_JEGA, 
			OO_CODE, OO_CDAY, OO_SAHG, OO_HAHG, OO_JJGA, OO_STCK, OO_START);
		break;
	case GU_FOREIGN:
		m_outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OS_TIME, OS_NAME, OS_JGGA, OS_DIFF, OS_RATE, OS_GVOL, OS_MDHG, OS_MSHG,
			OS_SIGA, OS_KOGA, OS_JEGA, 
			OS_CODE, OS_CDAY, OS_SAHG, OS_HAHG, OS_JJGA, OS_STCK, OS_START);
		break;
	case GU_COFUT:
		m_outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OCF_TIME, OCF_NAME, OCF_JGGA, OCF_DIFF, OCF_RATE, OCF_GVOL, OCF_MDHG, OCF_MSHG, 
			OCF_SIGA, OCF_KOGA, OCF_JEGA, 
			OCF_CODE, OCF_CDAY, OCF_SAHG, OCF_HAHG, OCF_JJGA, OCF_STCK, OCF_START);
		break;
	case GU_COOPT:
		m_outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			OCO_TIME, OCO_NAME, OCO_JGGA, OCO_DIFF, OCO_RATE, OCO_GVOL, OCO_MDHG, OCO_MSHG, 
			OCO_SIGA, OCO_KOGA, OCO_JEGA, 
			OCO_CODE, OCO_CDAY, OCO_SAHG, OCO_HAHG, OCO_JJGA, OCO_STCK, OCO_START);
		break;
	case GU_CFUTURE:
	case GU_FFUTURE:
		m_outSymbol.Format("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t",
			PDF_TIME, PDF_NAME, PDF_JGGA, PDF_DIFF, PDF_RATE, PDF_GVOL, PDF_MDHG, PDF_MSHG, 
			PDF_SIGA, PDF_KOGA, PDF_JEGA, 
			PDF_CODE, PDF_CDAY, PDF_SAHG, PDF_HAHG, PDF_JJGA, PDF_STCK, PDF_START);
		break;
	}
}

void CPnInput::ReadDisplay(int index)
{
	struct _envInfo	*envinfo = (_envInfo*)m_pParent->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);

	int	totCnt;
	int	uIndex = 0;
	switch (m_pMainWnd->m_iDtUnit)
	{
	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:	case GU_CFUTURE:	case GU_FFUTURE:
		uIndex = GU_FUTURE-GU_CODE;
		break;
	case GU_CODE:	case GU_INDEX:	
		uIndex = m_pMainWnd->m_iDtUnit - GU_CODE;
		break;
	case GU_FOREIGN:
		uIndex = 3;
		break;
	default:
		break;
	}

	totCnt = envinfo->datainfo.aaDayInfo[uIndex][index-GI_DAY].dwTotCnt;

	CString	tmpS;
	tmpS.Format("%d", totCnt);
	m_pGiganET->SetWindowText(tmpS);
}

void CPnInput::ChangeIndex(int index, bool bChgGap, bool bRedraw, bool bMessage)
{
	int	id = -1;

	struct _comp	*comp;
	if (bRedraw)
	{
		switch(index)
		{
		case GI_DAY:	id = BTN_DDAY;	break;
		case GI_WEEK:	id = BTN_DWEEK;	break;
		case GI_MONTH:	id = BTN_DMON;	break;
		case GI_MINUTE:	id = BTN_DMIN;	break;
		case GI_TICK:	id = BTN_DTICK;	break;
		default:	id = -1;	break;
		}
		
		for (int ii = BTN_DDAY; ii <= BTN_DTICK; ii++)
		{
			comp = CPnBaseWnd::SearchCom(ii);
			if (ii == id)
				comp->status = btnDown;
			else
				comp->status = btnUp;
		}
		InvalidateRect(&m_rcIndex);

		ReadDisplay(index);
		if (index == GI_MINUTE)
		{
			m_pGapCB->ResetContent();
			m_pGapCB->AddString("0.5");
			m_pGapCB->AddString("1");	m_pGapCB->AddString("2");
			m_pGapCB->AddString("3");	m_pGapCB->AddString("5");
			m_pGapCB->AddString("10");	m_pGapCB->AddString("15");
			m_pGapCB->AddString("20");	m_pGapCB->AddString("30");
			m_pGapCB->AddString("60");	m_pGapCB->AddString("90");
			m_pGapCB->AddString("120");
		}
		else if (index == GI_TICK)
		{
			m_pGapCB->ResetContent();
			m_pGapCB->AddString("1");	m_pGapCB->AddString("2");
			m_pGapCB->AddString("3");	m_pGapCB->AddString("5");
			m_pGapCB->AddString("10");	m_pGapCB->AddString("15");
			m_pGapCB->AddString("20");	m_pGapCB->AddString("30");
			m_pGapCB->AddString("60");	m_pGapCB->AddString("90");
			m_pGapCB->AddString("120");
		}
	}

	if (bChgGap)
	{
		switch(index)
		{
		case GI_DAY:	case GI_WEEK:	case GI_MONTH:	
				ChangeGap(index, 0);				m_pGapCB->ShowWindow(SW_HIDE);	break;
		case GI_MINUTE:	ChangeGap(index, m_pMainWnd->m_iMGap, !bRedraw);	m_pGapCB->ShowWindow(SW_SHOW);	break;
		case GI_TICK:	ChangeGap(index, m_pMainWnd->m_iTGap, !bRedraw);	m_pGapCB->ShowWindow(SW_SHOW);	break;
		default:	break;
		}
	}
	else
	{
		if (bMessage)	NotifyIndexChange(index);
	}
}

void CPnInput::ChangeGap(int index, int gap, bool bOnlyChange)
{
	if (!bOnlyChange)
	{
		int	end = (m_bMiniInput)?	BTN_TIME3:BTN_TIME4;
		CRect	btnRC(0,0,0,0);
		if (gap <= 0)
		{
			struct _comp	*comp;
			for (int ii = BTN_TIME1; ii <= end; ii++)
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
			NotifyIndexChange(index);

			return;
		}

		int	id = -1;
		if (gap == m_gap[0])		id = BTN_TIME1;
		else if (gap == m_gap[1])	id = BTN_TIME2;
		else if (gap == m_gap[2])	id = BTN_TIME3;
		else if (!m_bMiniInput && gap == m_gap[3])	id = BTN_TIME4;

		struct _comp	*comp;
		for (int ii = BTN_TIME1; ii <= end; ii++)
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
		if (index == GI_MINUTE)		m_pMainWnd->m_iMGap = gap;
		else if (index == GI_TICK)	m_pMainWnd->m_iTGap = gap;

		NotifyIndexChange(index);
	}
}

void CPnInput::NotifyIndexChange(int index)
{
	int	gap = 0;
	switch(index)
	{
	case GI_MINUTE:	gap = m_pMainWnd->m_iMGap;	break;
	case GI_TICK:	gap = m_pMainWnd->m_iTGap;	break;
	default:	break;
	}

	m_pParent->SendMessage(GEV_VIEW, MAKEWPARAM(viewNotify, indexInput), MAKELPARAM(index, gap));
}

void CPnInput::SetGapValue(char *pGap)
{
	char	reserve[4] = { GAP1, GAP2, GAP3, GAP4};
	for (int ii = 0; ii < 4; ii++)
	{
		if (pGap[ii] <= 0 || pGap[ii] > 90)
			m_gap[ii] = reserve[ii];
		else
			m_gap[ii] = pGap[ii];
	}
}

void CPnInput::ChangeFromOutside(int index, int gap)
{
	int	nSelect = -1;
	switch (index)
	{
	case GI_DAY:	m_selID = BTN_DDAY;	break;
	case GI_WEEK:	m_selID = BTN_DWEEK;	break;
	case GI_MONTH:	m_selID = BTN_DMON;	break;
	case GI_MINUTE:	m_selID = BTN_DMIN;	break;
	case GI_TICK:	m_selID = BTN_DTICK;	break;
	default:	// 기간 데이터
		{
			CString	strVal;
			strVal.Format("%d", gap);
			m_pGiganET->SetWindowText(strVal);		
		}
		return;
	}

	struct _comp* pOldCom = SearchCom(m_selID);
	struct _comp* comp = SearchCom(nSelect);

	if (comp != pOldCom)
	{
		InvalidateCom(pOldCom, btnUp);
		InvalidateCom(comp, btnDown);
	}

	ChangeIndex(index, true, true, false);
}