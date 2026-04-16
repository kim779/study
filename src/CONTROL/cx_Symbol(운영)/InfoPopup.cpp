// InfoPopup.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "InfoPopup.h"
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include "../../h/memDC.h"
#include "../../H/interMSG.h"
#include "ControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const int headerH = 25, dataH = 20;
const int titleH = 23, tabW = 60, gap1 = 1;

#define DF_NOMEMO 2
#define DF_EXIST_MEMO 1

#define TM_CHECKMOVE  9898

/////////////////////////////////////////////////////////////////////////////
// CInfoPopup

CInfoPopup::CInfoPopup()
{
	m_bDrag = false;
	m_nSelected = 0;
	m_pEdit = nullptr;
	m_sKey = "";
	m_bShow = FALSE;

	m_ujongsise = false;
}

CInfoPopup::~CInfoPopup()
{
//	saveMemo();
	if (m_pEdit)
	{
		if (IsWindow(m_pEdit->GetSafeHwnd()))
			m_pEdit->SendMessage(WM_CLOSE);
		m_pEdit->Detach();
		m_pEdit.reset();
	}
}

BEGIN_MESSAGE_MAP(CInfoPopup, CWnd)
	//{{AFX_MSG_MAP(CInfoPopup)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInfoPopup message handlers

BOOL CInfoPopup::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

int CInfoPopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pFont = getAxFont(LoadStringTb(IDS_BASEFONTNAME4), 9, 0);

	m_pEdit = std::make_unique<CEditX>();
	m_pEdit->Create(this, 100); m_pEdit->SetFont(m_pFont);
	m_pEdit->SetPopupParent(this);

	m_pParent->GetWindowRect(m_parentRect);
	SetTimer(TM_CHECKMOVE, 10, nullptr);
	return 0;
}

void CInfoPopup::OnSetFocus(CWnd* pOldWnd) 
{	
	CWnd::OnSetFocus(pOldWnd);
	m_bShow = TRUE;
}

void CInfoPopup::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE)
	{
		PostMessage(WM_USER, 0, 0);
	}
}

BOOL CInfoPopup::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CRect rc;
	CPoint pnt;
	GetCursorPos(&pnt);

	ScreenToClient(&pnt);

	GetClientRect(&rc);

	rc.left = rc.right - 20;
	rc.top = rc.bottom - 20;

	if (rc.PtInRect(pnt))
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENWSE));
	else
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CInfoPopup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rc;

	GetClientRect(&rc);

	rc.left = rc.right - 14;
	rc.top = rc.bottom - 14;

	if (rc.PtInRect(point))
	{
		m_firstPnt = point;
		SetCapture();
		GetClientRect(&m_rc);
		m_bDrag = true;
	}
	else
	{
		m_firstPnt = CPoint(-1, -1);
		GetClientRect(&rc);
		rc.bottom = rc.top + 20;
		rc.right = rc.left + 80;
		if (rc.PtInRect(point))
		{
			m_nSelected = 0;
			RedrawWindow();
			m_pEdit->ShowWindow(SW_HIDE);
		}
		
		rc.OffsetRect(80, 0);
		if (rc.PtInRect(point))
		{
			m_nSelected = 1;
			RedrawWindow();
			m_pEdit->ShowWindow(SW_SHOW);
			m_pEdit->SetFocus();
			m_pEdit->SetWindowText(_T(""));
			loadServerMemo(m_sKey);
		}

		GetClientRect(&rc);
		rc.left = rc.right - 28;
		rc.top = rc.top + 4;
		rc.bottom = rc.top + 10;
		rc.right = rc.left + 10;
		if (m_closeRect.PtInRect(point))
			ShowWindow(SW_HIDE);

		if (m_saveBtnRect.PtInRect(point))
		{
			if (m_nSelected == 1)
			{
				if (saveServerMemo(m_sKey))
					ShowWindow(SW_HIDE);
			}
			return;
		}

		if (m_searchBtnRect.PtInRect(point))
		{
			if (m_nSelected == 1)
			{
				loadServerMemo(m_sKey);
			}
			return;
		}
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CInfoPopup::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDrag)
	{
		CSize sz;
		sz.cx = m_rc.Width() - (m_firstPnt.x - point.x);
		sz.cy = m_rc.Height() - (m_firstPnt.y - point.y);

		if (sz.cx < 200)
			sz.cx = 200;
		if (sz.cy < 150)
			sz.cy = 150;

		SetWindowPos(NULL, 0, 0, sz.cx, sz.cy, SWP_NOZORDER|SWP_NOMOVE);
		Invalidate();
	}

	if (!m_bShow)
	{
		if (GetFocus() != this)
		{
			SetFocus();

			m_bShow = TRUE;
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CInfoPopup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDrag)
	{
		ReleaseCapture();
		m_bDrag = false;
	}
	CWnd::OnLButtonUp(nFlags, point);
}


void CInfoPopup::OnPaint() 
{
#ifndef _DEBUG
	CPaintDC dc(this); // device context for painting
	xxx::CMemDC	memdc(&dc);
#else
	CPaintDC memdc(this); // device context for painting
#endif
	CRect	rc;
	CPen	*pOldPen;

	GetClientRect(&rc); rc.DeflateRect(0, 18, 0, 0);

	memdc.FillSolidRect(rc, GetSysColor(COLOR_INACTIVEBORDER));
	drawCaption(&memdc);
	drawData(&memdc);

	pOldPen = (CPen*)memdc.SelectObject(getAxPen(GetSysColor(COLOR_HIGHLIGHTTEXT), 1, PS_SOLID));

	// exit button
	GetClientRect(&rc);
	rc.left = rc.right - 30;
	rc.top = rc.top + 5;
	rc.bottom = rc.top + 15;
	rc.right = rc.right - 4;
	memdc.Rectangle(rc);
	rc.DeflateRect(1, 1);
	m_closeRect = rc;
	memdc.FillSolidRect(m_closeRect, GetSysColor(COLOR_HIGHLIGHT));
	rc.InflateRect(1, 1);

	CFont* pOldFont;
	memdc.SetTextColor(RGB(255, 255, 255));
	pOldFont = (CFont*)memdc.SelectObject(getAxFont(LoadStringTb(IDS_CHANGEFONTNAME4), 8, 0));
	memdc.DrawText(_T("닫기"), -1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	//save button
	rc.OffsetRect(-1 * (rc.Width()) - 3, 0);
	m_saveBtnRect = rc;

	if (m_nSelected)
	{
		memdc.Rectangle(m_saveBtnRect);
		m_saveBtnRect.DeflateRect(1, 1);
		memdc.FillSolidRect(m_saveBtnRect, GetSysColor(COLOR_HIGHLIGHT));
		memdc.DrawText(_T("저장"), -1, m_saveBtnRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		//m_searchBtnRect = m_saveBtnRect;
		//m_searchBtnRect.OffsetRect(-1 * (rc.Width()) - 3, 0);
		//m_searchBtnRect.InflateRect(1, 1);
		//memdc.Rectangle(m_searchBtnRect);
		//m_searchBtnRect.DeflateRect(1, 1);
		//memdc.FillSolidRect(m_searchBtnRect, GetSysColor(COLOR_HIGHLIGHT));
		//memdc.DrawText(_T("조회"), -1, m_searchBtnRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		memdc.SelectObject(pOldFont);
		memdc.SelectObject(pOldPen);
	}

	if (!m_bShow)
	{
		if (GetFocus() != this || GetFocus() != m_pEdit.get())
		{
			SetFocus();
			m_bShow = TRUE;
		}
	}

/*
	// display resizing
	GetClientRect(&rc);

	rc.left = rc.right - 14;
	rc.top = rc.bottom - 14;
	
	for (int ii = 0; ii < 3; rc.OffsetRect(2, 2), ii++)
	{
		memdc.MoveTo(rc.left, rc.bottom);
		memdc.LineTo(rc.right, rc.top);
	}
*/
}

void CInfoPopup::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rc; GetClientRect(&rc);
	rc.DeflateRect(1, headerH+1, 1, 2);
	m_pEdit->MoveWindow(&rc);
}

void CInfoPopup::setJinfo(Cjinfo jinfo, bool ujongsise)
{
	m_jinfo = jinfo;
	m_ujongsise = ujongsise;
}

void CInfoPopup::drawCaption(CDC *pDC)
{
	CFont	*pOldFont = nullptr;
	CRect	clientRc, rect;
	
	GetClientRect(&clientRc); rect = clientRc;

	rect.top += gap1; rect.bottom = rect.top + titleH;
	pDC->FillSolidRect(rect, GetSysColor(COLOR_INFOBK));


	// display IDS_DETAILINFO
	if (m_nSelected == 0)
	{
		pDC->SetTextColor(RGB(38, 55, 100));
		pOldFont = (CFont*)pDC->SelectObject(getAxFont(LoadStringTb(IDS_CHANGEFONTNAME4), 9, 3));
	}
	else
	{
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		pOldFont = (CFont*)pDC->SelectObject(getAxFont(LoadStringTb(IDS_CHANGEFONTNAME4), 9, 0));
	}

	rect.right = rect.left + tabW;
	pDC->DrawText(LoadStringTb(IDS_DETAILINFO), -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);

	// display "|"
	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	pOldFont = (CFont*)pDC->SelectObject(getAxFont(LoadStringTb(IDS_CHANGEFONTNAME4), 9, 0));

	rect.left = rect.right; rect.right = rect.left + gap1;
	pDC->DrawText("|", -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	pDC->SelectObject(pOldFont);


	// display IDS_MEMOSTR
	if (m_nSelected == 1)
	{
		pDC->SetTextColor(RGB(38, 55, 100));
		pOldFont = (CFont*)pDC->SelectObject(getAxFont(LoadStringTb(IDS_CHANGEFONTNAME4), 9, 3));
	}
	else
	{
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		pOldFont = (CFont*)pDC->SelectObject(getAxFont(LoadStringTb(IDS_CHANGEFONTNAME4), 9, 0));
	}

	rect.left = rect.right; rect.right = rect.left + tabW;
	pDC->DrawText(LoadStringTb(IDS_MEMOSTR), -1, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);

	rect.left = rect.right; rect.right = clientRc.right; rect.DeflateRect(0, 2, 2, 2);
	pDC->FillSolidRect(rect, GetSysColor(COLOR_HIGHLIGHT));
}

void CInfoPopup::drawData(CDC *pDC)
{
	if (m_nSelected != 0)
		return;

	CRect	rect, irc, drc;
	CFont	font;
	CString	tmpx, temp, string;
	CStringArray	stringAry;
	const	char*  const	sGubn[]= {"", "코스피", "코스닥", "제3시장", "ETF"};

	GetClientRect(&rect);
	rect.top = rect.top + headerH; rect.DeflateRect(1, 1);
	pDC->FillSolidRect(rect, GetSysColor(COLOR_INFOBK));

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	CFont*pOldFont = (CFont*)pDC->SelectObject(getAxFont(LoadStringTb(IDS_CHANGEFONTNAME4), 9, 0));

	CString codx, jgub, jrat, ujgb, frgb, sjjs, amga, ksmm, jbkm, dyga, lock, nrate, memeunit, sijang;
	CString tmp;
	
	tmp = m_jinfo.codx;
	tmp.Replace("N.", "");
	tmp.Replace("M.", "");
	if (tmp.GetLength() > 1 ) 
		tmp.Delete(0);
	tmp.TrimRight();

	if (m_jinfo.rlnk != ' ' )
		codx.Format("종 목 코 드   : %s (%s등급)", tmp, m_jinfo.rlnk);
	else
		codx.Format("종 목 코 드   : %s", tmp); 

	stringAry.Add(codx);

	//2012.05.04 KSJ 매매단위 계산하는 것 추가함.
	switch (m_jinfo.jgub.GetAt(0))
	{
	case '1': jgub = "시 장 구 분   : 코스피";		break;
	case '2': jgub = "시 장 구 분   : 코스닥";		break;
	case '3': jgub = "시 장 구 분   : K-OTC";		break;
	case '4': jgub = "시 장 구 분   : ETF";		break;
	case '6': jgub = "시 장 구 분   : ELW";	break;
	case 'A': jgub = "시 장 구 분   : 코넥스";			break;
	case 'B': jgub = "시 장 구 분   : ETN";			break;
	case 'E': jgub = "시 장 구 분   : 리츠";		break;
	case 'F': jgub = "시 장 구 분   : 스팩";		break;
	case 'G': jgub = "시 장 구 분   : 신주인수권";		break;
	case 'H': jgub = "시 장 구 분   : 신주인수권증서";		break;
	case 'I': jgub = "시 장 구 분   : 수익증권";		break;
	case 'J': jgub = "시 장 구 분   : 신탁수익증서";		break;
	case 'K': jgub = "시 장 구 분   : 투자계약증권";		break;
	case 'R': jgub = "시 장 구 분   : 상장형수익증권";		break;
	case 'C': jgub = "시 장 구 분   : BDC수익증권";		break;
	case 'D': jgub = "시 장 구 분   : BDC투자회사";		break;
	default: jgub = m_jinfo.jgub;	break;
	}
	//KSJ
	stringAry.Add(jgub);

	if (m_jinfo.bNXT)
	{
		tmp.Format("거 래 소      : %s", "NXT거래가능");
		tmp.TrimRight();
	
	}
	else
	{
		tmp.Format("거 래 소      : %s", "NXT거래불가능");
		tmp.TrimRight();
	}
	stringAry.Add(tmp);

	ujgb.Format("업       종   : %s", m_jinfo.ujgb);
	stringAry.Add(ujgb);
	jrat.Format("현금증거금률  : %d %%", atoi((LPCTSTR)m_jinfo.jrat));
	stringAry.Add(jrat);
	nrate.Format("신용보증금률  : %d %%", atoi((LPCTSTR)m_jinfo.nrat));
	stringAry.Add(nrate);
	frgb.Format("외국인보유비중: %s", (m_jinfo.frgb));
	stringAry.Add(frgb);
	tmp.Format("%d", atoi(m_jinfo.sjjs));
	tmp = addComma(tmp);
	sjjs.Format("상장 주식수   : %s 천주", tmp);
	stringAry.Add(sjjs);
	amga = m_jinfo.amga;
	amga.Remove(_T(','));
	tmp.Format("%d", atoi(amga));
	tmp = addComma(tmp);

	amga.Format("액   면   가  : %s 원", tmp);
	stringAry.Add(amga);
	ksmm.Format("결   산   월  : %d 월", atoi(m_jinfo.ksmm));
	stringAry.Add(ksmm);
	//jbkm = m_jinfo.jbkm;
	//jbkm.Remove(_T(','));
	jbkm.Format("자   본   금  : %s", m_jinfo.jbkm);
	dyga.Format("%d", atoi(m_jinfo.dyga));
	tmp.Format("%d", atoi(m_jinfo.dyga));
	tmp = addComma(tmp);
	dyga.Format("대   용   가  : %s 원", tmp);
	stringAry.Add(dyga);
	lock.Format("락   구   분  : %s", m_jinfo.lock);
	stringAry.Add(lock);
	sijang.Format("시 장 조 치   : %s", m_jinfo.sijang);
	sijang.TrimRight();
			
	//2016.06.22 KSJ 단기, 과열, 정리, 등에 ,가 들어간다.
	if (sijang.Find(",") != -1)
		tmp = sijang.Mid(0, sijang.GetLength()-1);
	else
		tmp = sijang;

	stringAry.Add(tmp);

	//2012.05.04 KSJ 매매단위 추가함. 전일종가를 토대로 계산함
	//memeunit.Format("매 매 단 위   : %s 주", m_jinfo.str1377);
	//stringAry.Add(memeunit);
	//KSJ

	//2013.06.11 KSJ 코넥스 추가
	if (m_jinfo.jgub.GetAt(0) == 'A')
	{
		memeunit.Format("지정자문인    : %s", m_jinfo.str1971);
		stringAry.Add(memeunit);
		//추후에 추가됨.
		//memeunit.Format("코넥스연속매매여부: %s", "연속");
		//stringAry.Add(memeunit);
	}
	//KSJ

	CString strTemp;
	for (int ii = 0; ii < stringAry.GetSize(); ii++)
	{
		string = stringAry.GetAt(ii);
		if (string.IsEmpty())
			continue;

		drc = rect;
		drc.bottom = drc.top + dataH;

		irc = drc; irc.right = irc.left + dataH;
		drawIndicator(pDC, irc);

		drc.left += dataH; drc.top += 2;
		pDC->DrawText(string, drc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		rect.OffsetRect(CPoint(0, dataH));
	}
	pDC->SelectObject(pOldFont);
}

void CInfoPopup::drawIndicator(CDC* dc, CRect rect)
{
	CPoint	pts[3];
	int	gap = 0;

	CPen*	oldPen   = dc->SelectObject(getAxPen(GetSysColor(COLOR_INFOBK), 1, PS_SOLID));
	CBrush* oldBrush = dc->SelectObject(getAxBrush(GetSysColor(COLOR_GRAYTEXT)));

	dc->SetPolyFillMode(ALTERNATE);
	gap = rect.Height()*2/5;

	rect.DeflateRect(gap, 4, gap+1, 4);
	rect.OffsetRect(0, 1);
	rect.bottom = rect.top + (rect.Height()/2)*2-1;
	rect.right = rect.left + rect.Height()/2;

	pts[0].y = rect.top;
	pts[1].y = rect.top + (rect.Height()/2);
	pts[2].y = rect.bottom-1;
	pts[0].x = rect.left;
	pts[1].x = rect.right;
	pts[2].x = rect.left;
	dc->Polygon(pts, 3);

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);
}

BOOL CInfoPopup::saveServerMemo(CString sKey)
{
	if (sKey.IsEmpty())
		return FALSE;

	if (m_pEdit)
	{
		CString str;

		m_pEdit->GetWindowText(str);

		if (str.GetLength() >= 9999)
		{
			CString slog;
			slog.Format("메모 저장 내용 한도[9999] 초과!!![%d]", str.GetLength());
			if (MessageBox(slog, "메모저장", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
			{
				return FALSE;
			}
			else
			{
				return FALSE;
			}
		}

		if (!str.IsEmpty())
		{
			mid_memo* pmid = new mid_memo;
			if (m_mode == MEMO::memo_non)
				pmid->chgubn[0] = 'I';
			else if (m_mode == MEMO::memo_update)
				pmid->chgubn[0] = 'U';

			char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
			if ((long)pData > 1)
				memcpy(pmid->chusid, pData, strlen(pData));

			memcpy(pmid->chcode, (char*)sKey.GetBuffer(0), sKey.GetLength());
			memcpy(pmid->chmemo, (char*)str.GetBuffer(0), str.GetLength());

			m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> gubn=[%c] code=[%.16s]  str=[%s] ",
				__FUNCTION__, __LINE__, "saveServerMemo", pmid->chgubn[0], pmid->chcode, str.Left(50));
			OutputDebugString(m_slog);

			if (m_mode == MEMO::memo_non)
				if (((CControlWnd*)m_pParent)->m_bFromDLL)
					((CControlWnd*)m_pParent)->SendMemoTR_Dll((char*)pmid, sizeof(mid_memo), TRKEY_MEMO_INSERT, sKey);
				else
					((CControlWnd*)m_pParent)->SendMemoTR_Control("pidomemo", (char*)pmid, sizeof(mid_memo), US_KEY, TRKEY_MEMO_INSERT, sKey);
			else if (m_mode == MEMO::memo_update)
				if (((CControlWnd*)m_pParent)->m_bFromDLL)
					((CControlWnd*)m_pParent)->SendMemoTR_Dll((char*)pmid, sizeof(mid_memo), TRKEY_MEMO_UPDATE, sKey);
				else
					((CControlWnd*)m_pParent)->SendMemoTR_Control("pidomemo", (char*)pmid, sizeof(mid_memo), US_KEY, TRKEY_MEMO_UPDATE, sKey);
		}
		else //내용이 없으면 메모를 지운다
		{
			mid_SDEmemo* pmid = new mid_SDEmemo;
			pmid->chgubn[0] = 'D';

			char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
			if ((long)pData > 1)
				memcpy(pmid->chusid, pData, strlen(pData));

			memcpy(pmid->chcode, (char*)sKey.GetBuffer(0), sKey.GetLength());

			m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->gubn=[%c] code=[%s]  ", __FUNCTION__, __LINE__, "saveServerMemo 메모를 지우는 TR",
				pmid->chgubn[0], pmid->chcode);
			OutputDebugString(m_slog);

			if (((CControlWnd*)m_pParent)->m_bFromDLL)
				((CControlWnd*)m_pParent)->SendMemoTR_Dll((char*)pmid, sizeof(mid_SDEmemo), TRKEY_MEMO_DELETE, sKey);
			else
				((CControlWnd*)m_pParent)->SendMemoTR_Control("pidomemo", (char*)pmid, sizeof(mid_SDEmemo), US_KEY, TRKEY_MEMO_DELETE, sKey);

			((CControlWnd*)m_pParent)->m_memostate = DF_NOMEMO;
			((CControlWnd*)m_pParent)->Invalidate();
		}
	}
	return TRUE;
}

void CInfoPopup::saveMemo(CString sKey)
{
	m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->  code=[%s] ",
		__FUNCTION__, __LINE__, "saveMemo start", sKey);
	OutputDebugString(m_slog);

	if (sKey.IsEmpty())
		return;

	if (m_pEdit)
	{
		char	key[12]{}, lBytes[4]{};
		CString dat;
		CString str, rDir, wDir;

		rDir.Format("%s\\%s\\%s\\memo.mmo", m_sRoot, USER, m_sUserID);
		wDir.Format("%s\\%s\\%s\\memo.mm2", m_sRoot, USER, m_sUserID);

		CFileFind	finder;
		if (!finder.FindFile(rDir))  //memo.mmo 파일이 없으면 자체 저장 하지는 말라 서버저장 하라
		{
			//m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->!!!!파일이 없으니 굳이 저장하지 않는다 !!! just server save]", __FUNCTION__, __LINE__,"saveMemo");
			//OutputDebugString(m_slog);
			//		return;
		}

		m_pEdit->GetWindowText(str);
		
		CFile wFile(wDir, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone);

		if (!str.IsEmpty())
		{
			FillMemory(key, sizeof(key), ' ');
			FillMemory(lBytes, sizeof(lBytes), ' ');

			strcpy(key, sKey.GetString());
			itoa(str.GetLength(), lBytes, 10);
			wFile.Write(key, 12);
			wFile.Write(lBytes, 4);
			wFile.Write(str, str.GetLength());
		}

		if (!finder.FindFile(rDir))
		{
			wFile.Close();
			CopyFile(wDir, rDir, false);
			DeleteFile(wDir);
			return;
		}
		
		TRY
		{
			CFile rFile(rDir, CFile::modeRead | CFile::shareDenyNone);
			
			UINT	nBytesRead = 0;
			do
			{
				FillMemory(key, sizeof(key), ' ');
				FillMemory(lBytes, sizeof(lBytes), ' ');

				nBytesRead = rFile.Read(&key, sizeof(key));
				if (nBytesRead == sizeof(key))
				{
					nBytesRead = rFile.Read(&lBytes, sizeof(lBytes));
					if (nBytesRead == sizeof(lBytes))
					{
						int	lSize = atoi(CString(lBytes, 4));
						nBytesRead = rFile.Read(dat.GetBuffer(lSize), lSize);
						
						if ((int)nBytesRead != lSize)
							break;
						
						if (!sKey.Compare(key))  //파일에 적힌 내용의 코드와 메모윈도우의 코드가 같으면 적지 않는다.
						{
							dat.ReleaseBuffer();
							continue;
						}
						wFile.Write(key, 12);
						wFile.Write(lBytes, 4);
						wFile.Write(dat, nBytesRead);
						dat.ReleaseBuffer();   //memo.mm2

						m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> !!!! save memo file !!!  code=[%s] ", __FUNCTION__, __LINE__, "saveMemo", sKey);
						OutputDebugString(m_slog);

					}
					else
						break;
				}
				else
					break;
				
			} while ((int)nBytesRead);

			rFile.Close();
		}
		CATCH(CFileException, e )
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		}
		END_CATCH

		wFile.Close();
		CopyFile(wDir, rDir, false);
		DeleteFile(wDir);

		m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->  code=[%s] ",
			__FUNCTION__, __LINE__, "saveMemo end", sKey);
		OutputDebugString(m_slog);
	}
}

void CInfoPopup::OnDestroy() 
{
	CWnd::OnDestroy();
}

void CInfoPopup::loadServerMemo(CString sKey)
{
	if (sKey.IsEmpty())
		return;

	m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> code=[%s] ", __FUNCTION__, __LINE__, "loadServerMemo", sKey);
	OutputDebugString(m_slog);

	mid_SDEmemo* pmid = new mid_SDEmemo;
	pmid->chgubn[0] = 'S';

	char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
	if ((long)pData > 1)
		memcpy(pmid->chusid, pData, strlen(pData));

	memcpy(pmid->chcode, (char*)sKey.GetBuffer(0), sKey.GetLength());

	if (((CControlWnd*)m_pParent)->m_bFromDLL)
		((CControlWnd*)m_pParent)->SendMemoTR_Dll((char*)pmid, sizeof(mid_SDEmemo), TRKEY_MEMO_SEARCH, sKey);
	else
		((CControlWnd*)m_pParent)->SendMemoTR_Control("pidomemo", (char*)pmid, sizeof(mid_SDEmemo), US_KEY, TRKEY_MEMO_SEARCH, sKey);

	return;
}

void CInfoPopup::removeMemoInMemoFile(CString sKey)
{
	CString strFile;

	strFile.Format("%s\\%s\\SAVEMEMOFILE.TXT", m_sRoot, TABDIR);

	int bSaveMemoFile = GetPrivateProfileInt("MEMO", "SAVEFILE", 0, (LPCSTR)strFile);

	if (!bSaveMemoFile)
		return;

	CString dir, dat;
	char	key[12]{}, lBytes[4]{};

	m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> sKey=[%s] 종목메모를 지운다", __FUNCTION__, __LINE__, "removeMemoInMemoFile", sKey);
	OutputDebugString(m_slog);

	dir.Format("%s\\%s\\%s\\memo.mmo", m_sRoot, USER, m_sUserID);
	CFileFind	finder;
	if (!finder.FindFile(dir))
	{
		m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->", __FUNCTION__, __LINE__, "removeMemoInMemoFile @@@@  no memofile @@@@]");
		OutputDebugString(m_slog);
		return;
	}

	CString stmp;
	CMapStringToString _mapCodeMemo;
	TRY
	{
		CFile file(dir, CFile::modeRead | CFile::shareDenyNone);
		UINT nBytesRead;
		do
		{
			nBytesRead = file.Read(&key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = file.Read(&lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = file.Read(dat.GetBufferSetLength(lSize), lSize);

					if ((int)nBytesRead != lSize)
					{

	m_slog.Format("\r\n[memo[%-40s][%d][%-35s]--> nBytesRead=[%d]  lSize=[%d]", __FUNCTION__, __LINE__,"removeMemoInMemoFile",nBytesRead, lSize);
	OutputDebugString(m_slog);
						dat.ReleaseBuffer();
						break;
					}

					if (sKey != key)
					{
						stmp.Format("%-12s%-4d%s", key, lSize, dat);

	m_slog.Format("\r\n[memo][%-40s][%d][%-35s]--> 파일에 쓸종목=[%s]", __FUNCTION__, __LINE__,"removeMemoInMemoFile", key);
	OutputDebugString(m_slog);

						_mapCodeMemo.SetAt(stmp.Left(12).Trim(), stmp);
					}

					dat.ReleaseBuffer();
				}
				else
					break;
			}
			else
				break;

		} while ((int)nBytesRead);
		file.Close();

		DeleteFile(dir);

		if (_mapCodeMemo.GetSize() <= 0)
		{
			m_slog.Format("\r\n[memo][%-40s][%d][%-35s]", __FUNCTION__, __LINE__,"removeMemoInMemoFile  _mapCodeMemo size = 0 파일 안만들고 return");
			OutputDebugString(m_slog);
			return;
		}

		//CStdioFile sfile;
		//if (!sfile.Open(dir, CFile::modeCreate | CFile::modeWrite)) {
		//	AfxMessageBox(_T("파일을 열 수 없습니다."));
		//	return;
		//}

		CFile wFile(dir, CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone);

		CString strKey, strValue;
		POSITION pos = _mapCodeMemo.GetStartPosition();
		while (pos != NULL) {
			_mapCodeMemo.GetNextAssoc(pos, strKey, strValue);

			FillMemory(key, sizeof(key), ' ');
			FillMemory(lBytes, sizeof(lBytes), ' ');

			dat = strValue.Left(12).Trim();
			strcpy(key, dat.GetString());
			dat = strValue.Right(strValue.GetLength() - 16);
			itoa(dat.GetLength(), lBytes, 10);
			wFile.Write(key, 12);
			wFile.Write(lBytes, 4);
			wFile.Write(dat, dat.GetLength());

	m_slog.Format("\r\n[memo][%-40s][%d][%-35s] -->!!!!!!  [%s]종목은 파일에 있다.", __FUNCTION__, __LINE__, "removeMemoInMemoFile", strValue.Left(12).Trim());
	OutputDebugString(m_slog);
	//sfile.WriteString(strValue);
	}

		// 파일 닫기
		wFile.Close();

	}
		CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH
}

void CInfoPopup::loadMemo(CString sKey)
{
	CString dir, dat;
	char	key[12]{}, lBytes[4]{};

	((CControlWnd*)m_pParent)->m_memostate = DF_NOMEMO;
	((CControlWnd*)m_pParent)->Invalidate();

	dir.Format("%s\\%s\\%s\\memo.mmo", m_sRoot, USER, m_sUserID);
	CFileFind	finder;
	if (!finder.FindFile(dir))
	{
		return;
	}

	TRY
	{
		CFile file(dir, CFile::modeRead | CFile::shareDenyNone);
		UINT nBytesRead;
		do
		{
			nBytesRead = file.Read(&key, sizeof(key));
			if (nBytesRead == sizeof(key))
			{
				nBytesRead = file.Read(&lBytes, sizeof(lBytes));
				if (nBytesRead == sizeof(lBytes))
				{
					int lSize = atoi(CString(lBytes, 4));
					nBytesRead = file.Read(dat.GetBufferSetLength(lSize), lSize);

					if ((int)nBytesRead != lSize)
					{
						dat.ReleaseBuffer();
						break;
					}

					if (!sKey.Compare(key))
					{
m_slog.Format("\r\n[memo][%-40s][%d][%-35s]-->!!!!!!  [%s]종목은 서버는 없지만 파일에 있다.", __FUNCTION__, __LINE__,"loadMemo", sKey.Trim());
OutputDebugString(m_slog);
						m_pEdit->SetWindowText(dat);
						m_pEdit->SetSel(0, -1);
						dat.ReleaseBuffer();
						((CControlWnd*)m_pParent)->m_memostate = DF_EXIST_MEMO;
						((CControlWnd*)m_pParent)->Invalidate();
						break;
					}

					dat.ReleaseBuffer();
				}
				else
					break;
			}
			else
				break;

		} while ((int)nBytesRead);
		file.Close();


	}
		CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH
}

void CInfoPopup::setPathInfo(CString sRoot, CString sID)
{
	m_sUserID = sID;
	m_sRoot = sRoot;
	m_sKey = CString(m_jinfo.codx, 12);
	m_sKey.Trim(); 
}

CString CInfoPopup::SetComma(CString src)
{
	CString temp, floatStr;

	if (src.Find('.') >= 0)
	{
		floatStr = src.Mid(src.Find('.'));
		src = src.Left(src.Find('.'));
	}
	else
		floatStr.Empty();
	
	temp = src;
	src.Empty();
	
	for (int ii = temp.GetLength() - 1, jj = -1; ii >= 0; ii--)
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

CPen* CInfoPopup::getAxPen(COLORREF clr, int width, int style)
{
	struct	_penR	penR {};

	penR.clr = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pWizard->SendMessage(WM_USER, getPEN, (LPARAM)&penR);
}

CBrush* CInfoPopup::getAxBrush(COLORREF clr)
{
	return (CBrush*) m_pWizard->SendMessage(WM_USER, getBRUSH, (LPARAM)clr);
}

CFont* CInfoPopup::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR {};

	fontR.name = (LPSTR)fName.GetString();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;
	switch (style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (LPARAM)&fontR);
}

CString CInfoPopup::getDiffSym(char chDiff)
{
	switch (chDiff)
	{
	case '1':	// 상한
		return "↑";
	case '+':
	case '2':	// 상승
		return "▲";
	case '3':	// 보합
		return "";
	case '4':	// 하락
		return "↓";
	case '-':
	case '5':	// 상한
		return "▼";
	}
	return "";
}

CString CInfoPopup::LoadStringTb(UINT nIdx)
{
	CString str;
	str.LoadString(nIdx);
	return str;
}

CString CInfoPopup::addComma(CString data)
{
	CString	tmps;
	int	pos = 0, length = 0;

	tmps = data;
	length = tmps.GetLength();
	pos = tmps.Find('.') + 1;
	if (pos > 0)
		return data;

	length -= pos;
	if (length < 4)
		return data;

	data.Empty();
	for (int ii = 0; ii < length; ii++)
	{
		if ((length-ii) % 3 == 0 && ii != 0)
			data += ',';
		data += tmps.GetAt(ii);
	}
	return data;
}

void CInfoPopup::OnKillFocus(CWnd* pNewWnd) 
{	
	CWnd::OnKillFocus(pNewWnd);

	/*if (pNewWnd == (CWnd*)m_pEdit.get() || pNewWnd == this)
	{
		return;
	}
	
	if (m_bShow)
	{
		ShowWindow(SW_HIDE);
		m_sKey = CString(m_jinfo.codx, 12);
		m_sKey.Trim();
		if (!m_sKey.IsEmpty()) 
			saveServerMemo(m_sKey);
	}*/
}

void CInfoPopup::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CWnd::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	//m_bShow = bShow;
}


void CInfoPopup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case TM_CHECKMOVE:
		{
			if (IsWindowVisible())
			{
				CRect rec, tmprec;
				m_pParent->GetWindowRect(&rec);
				if (rec != m_parentRect)
				{
					GetWindowRect(tmprec);
					tmprec.OffsetRect(rec.left - m_parentRect.left, rec.top - m_parentRect.top);
					MoveWindow(tmprec, true);
					m_parentRect = rec;
				}
			}
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}
