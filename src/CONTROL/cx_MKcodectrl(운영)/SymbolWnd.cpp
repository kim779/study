// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SymbolWnd.h"
#include "controlwnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int indicatorW = 11;
const int bitmapW = 18;



/////////////////////////////////////////////////////////////////////////////
// CSymbolWnd

CSymbolWnd::CSymbolWnd(CWnd* pWnd) : CAxWnd(pWnd)
{
	m_capture = false;
	m_bCheck = false;
	m_pPopup = nullptr;
	m_bTracking = FALSE;
	m_bHover = FALSE;
	m_bMouseOver = false;
	m_sData = "";

	m_sCode = _T("");
	m_ujongsise = false;
	m_memoexist = false;
	m_bitmap = nullptr;

	m_sName.Empty();
	m_sRate.Empty();

	m_pBitmap = nullptr;
	m_pTipCtrl = nullptr;
	m_hjinfo = nullptr;
	m_ejinfo = nullptr;
}

CSymbolWnd::~CSymbolWnd()
{
	if (m_pPopup)
	{
		if (IsWindow(m_pPopup->GetSafeHwnd()))
			m_pPopup->SendMessage(WM_CLOSE);
		m_pPopup->Detach();
		m_pPopup.reset();
	}

	if (m_hjinfo)
		m_hjinfo.reset();
	if (m_ejinfo)
		m_ejinfo.reset();
}

void CSymbolWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CSymbolWnd, CAxWnd)
	//{{AFX_MSG_MAP(CSymbolWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolWnd message handlers

long CSymbolWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		return (long)m_rtnStr.GetString();

	case DLL_OUBx:
		{
			struct	_extTHx* exth = (struct _extTHx*)lParam;
			CString szData = CString(exth->data);
			CString Code = Parser(szData, "\t");

			if (m_sCode.CompareNoCase(Code) != 0)
				return 0;

			if (m_hjinfo)
				m_hjinfo.reset();
			if (m_ejinfo)
				m_ejinfo.reset();

			if (exth->size <= L_jHeader)
				return 0;

			struct _jHeader* jHeader = (struct _jHeader*)exth->data;
			struct _ejinfo* ejinfo = nullptr;
			struct _hjinfo* hjinfo = nullptr;

			if (jHeader->jgub[0] == '5' && exth->size >= L_ejinfo) // elw
			{
				ejinfo = (struct _ejinfo*)exth->data;
				m_ejinfo = std::make_unique<Cejinfo>();

				m_ejinfo->codx = CString(ejinfo->codx, sizeof(ejinfo->codx));
				m_ejinfo->hnam = CString(ejinfo->hnam, sizeof(ejinfo->hnam));
				m_ejinfo->jgub = CString(ejinfo->jgub, sizeof(ejinfo->jgub));
				m_ejinfo->gcjs = CString(ejinfo->gcjs, sizeof(ejinfo->gcjs));
				m_ejinfo->krye = CString(ejinfo->krye, sizeof(ejinfo->krye));
				m_ejinfo->khbe = CString(ejinfo->khbe, sizeof(ejinfo->khbe));
				m_ejinfo->gjbe = CString(ejinfo->gjbe, sizeof(ejinfo->gjbe));
				m_ejinfo->hbge = CString(ejinfo->hbge, sizeof(ejinfo->hbge));
				m_ejinfo->sjjs = CString(ejinfo->sjjs, sizeof(ejinfo->sjjs));
				m_ejinfo->sgca = CString(ejinfo->sgca, sizeof(ejinfo->sgca));
				m_ejinfo->hsga = CString(ejinfo->hsga, sizeof(ejinfo->hsga));
				m_ejinfo->jhre = CString(ejinfo->jhre, sizeof(ejinfo->jhre));
				m_ejinfo->sjil = CString(ejinfo->sjil, sizeof(ejinfo->sjil));
				m_ejinfo->edte = CString(ejinfo->edte, sizeof(ejinfo->edte));
				m_ejinfo->mgil = CString(ejinfo->mgil, sizeof(ejinfo->mgil));
				m_ejinfo->jgil = CString(ejinfo->jgil, sizeof(ejinfo->jgil));
				m_ejinfo->jjis = CString(ejinfo->jjis, sizeof(ejinfo->jjis));
				m_ejinfo->lpsa = CString(ejinfo->lpsa, sizeof(ejinfo->lpsa));
				m_ejinfo->lpjg = CString(ejinfo->lpjg, sizeof(ejinfo->lpjg));
				m_ejinfo->lpsu = CString(ejinfo->lpsu, sizeof(ejinfo->lpsu));
				m_ejinfo->lpbr = CString(ejinfo->lpbr, sizeof(ejinfo->lpbr));

				m_sCode = m_ejinfo->codx; m_sCode.Trim();
				m_memoexist = checkMemo(m_sCode);
				
				CString	imagePath;
				imagePath.Format("%s/%s/", Variant(homeCC, ""), IMAGEDIR);
				m_bitmap = getBitmap(imagePath + _T("ELW.bmp"));
				m_pBitmap = getBitmap(imagePath + _T("100.bmp"));
				m_sRate.Format("100");
				loadGubn(jHeader);
				m_ejinfo->hnam.TrimRight();
				m_sName = m_ejinfo->hnam;
				m_sData = m_sName;
				SetDataTooltip();
				Invalidate();

			}
			else if (exth->size >= L_hjinfo)
			{
				int	jrat = 0, nrat = 0;
				hjinfo = (struct _hjinfo*)exth->data;
				m_hjinfo = std::make_unique<Chjinfo>();

				m_hjinfo->codx = CString(hjinfo->codx, sizeof(hjinfo->codx));
				m_hjinfo->hnam = CString(hjinfo->hnam, sizeof(hjinfo->hnam));
				m_hjinfo->jgub = CString(hjinfo->jgub, sizeof(hjinfo->jgub));
				m_hjinfo->size = CString(hjinfo->size, sizeof(hjinfo->size));
				m_hjinfo->k2gb = CString(hjinfo->k2gb, sizeof(hjinfo->k2gb));
				m_hjinfo->usun = CString(hjinfo->usun, sizeof(hjinfo->usun));
				m_hjinfo->ksmm = CString(hjinfo->ksmm, sizeof(hjinfo->ksmm));
				m_hjinfo->amga = CString(hjinfo->amga, sizeof(hjinfo->amga));
				m_hjinfo->jbkm = CString(hjinfo->jbkm, sizeof(hjinfo->jbkm));
				m_hjinfo->siga = CString(hjinfo->siga, sizeof(hjinfo->siga));
				m_hjinfo->sjjs = CString(hjinfo->sjjs, sizeof(hjinfo->sjjs));
				m_hjinfo->jrat = CString(hjinfo->jrat, sizeof(hjinfo->jrat));
				m_hjinfo->nrat = CString(hjinfo->nrat, sizeof(hjinfo->nrat));	// 20070718

				// extended information
				m_hjinfo->krgb = CString(hjinfo->krgb, sizeof(hjinfo->krgb));
				m_hjinfo->jggb = CString(hjinfo->jggb, sizeof(hjinfo->jggb));
				m_hjinfo->lock = CString(hjinfo->lock, sizeof(hjinfo->lock));
				m_hjinfo->dyga = CString(hjinfo->dyga, sizeof(hjinfo->dyga));
				m_hjinfo->ujgb = CString(hjinfo->ujgb, sizeof(hjinfo->ujgb));
				m_hjinfo->ujcd = CString(hjinfo->ujcd, sizeof(hjinfo->ujcd));
				m_hjinfo->jisu = CString(hjinfo->jisu, sizeof(hjinfo->jisu));
				m_hjinfo->diff = CString(hjinfo->diff, sizeof(hjinfo->diff));
				m_hjinfo->rate = CString(hjinfo->rate, sizeof(hjinfo->rate));
				m_hjinfo->gvol = CString(hjinfo->gvol, sizeof(hjinfo->gvol));
				m_hjinfo->unio = CString(hjinfo->unio, sizeof(hjinfo->unio));

				m_sCode = m_hjinfo->codx; m_sCode.Trim();
				m_memoexist = checkMemo(m_sCode);
				m_hjinfo->hnam.TrimRight();
				m_sName = m_hjinfo->hnam;
				m_sData = m_sName;
				loadBitmap(jHeader->jgub, m_hjinfo->krgb, m_hjinfo->jrat);
				loadGubn(jHeader);
				m_hjinfo->jisu.Replace("+", "");
				m_hjinfo->jisu.Replace("-", "");
				jrat = atoi(m_hjinfo->jrat);
				nrat = atoi(m_hjinfo->nrat);
				if (jrat > 0)
				{
					m_sData.Format("%s", m_hjinfo->hnam);
					m_sRate.Format("%d", jrat);
				}

				SetDataTooltip();
				Invalidate();
			}
			else
			{
				m_sData.Empty();
				m_bitmap = NULL;
				Invalidate();
			}

			//	((CControlWnd*)GetParent())->SetEditFocus();
		}
		break;

	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only.
		{
			CString szData = CString((char*)lParam);
			CString Code = Parser(szData, "\t");

			if (m_sCode.CompareNoCase(Code) != 0)
				return 0;

			if (m_hjinfo)
				m_hjinfo.reset();
			if (m_ejinfo)
				m_ejinfo.reset();

			const	int	len = HIWORD(wParam);
			if (len <= L_jHeader)
				return 0;

			struct _jHeader* jHeader = (struct _jHeader*)szData.operator LPCTSTR();
			struct _ejinfo* ejinfo = nullptr;
			struct _hjinfo* hjinfo = nullptr;

			if (jHeader->jgub[0] == '5' && len >= L_ejinfo) // elw
			{
				ejinfo = (struct _ejinfo*)szData.operator LPCTSTR();
				m_ejinfo = std::make_unique<Cejinfo>();

				m_ejinfo->codx = CString(ejinfo->codx, sizeof(ejinfo->codx));
				m_ejinfo->hnam = CString(ejinfo->hnam, sizeof(ejinfo->hnam));
				m_ejinfo->jgub = CString(ejinfo->jgub, sizeof(ejinfo->jgub));
				m_ejinfo->gcjs = CString(ejinfo->gcjs, sizeof(ejinfo->gcjs));
				m_ejinfo->krye = CString(ejinfo->krye, sizeof(ejinfo->krye));
				m_ejinfo->khbe = CString(ejinfo->khbe, sizeof(ejinfo->khbe));
				m_ejinfo->gjbe = CString(ejinfo->gjbe, sizeof(ejinfo->gjbe));
				m_ejinfo->hbge = CString(ejinfo->hbge, sizeof(ejinfo->hbge));
				m_ejinfo->sjjs = CString(ejinfo->sjjs, sizeof(ejinfo->sjjs));
				m_ejinfo->sgca = CString(ejinfo->sgca, sizeof(ejinfo->sgca));
				m_ejinfo->hsga = CString(ejinfo->hsga, sizeof(ejinfo->hsga));
				m_ejinfo->jhre = CString(ejinfo->jhre, sizeof(ejinfo->jhre));
				m_ejinfo->sjil = CString(ejinfo->sjil, sizeof(ejinfo->sjil));
				m_ejinfo->edte = CString(ejinfo->edte, sizeof(ejinfo->edte));
				m_ejinfo->mgil = CString(ejinfo->mgil, sizeof(ejinfo->mgil));
				m_ejinfo->jgil = CString(ejinfo->jgil, sizeof(ejinfo->jgil));
				m_ejinfo->jjis = CString(ejinfo->jjis, sizeof(ejinfo->jjis));
				m_ejinfo->lpsa = CString(ejinfo->lpsa, sizeof(ejinfo->lpsa));
				m_ejinfo->lpjg = CString(ejinfo->lpjg, sizeof(ejinfo->lpjg));
				m_ejinfo->lpsu = CString(ejinfo->lpsu, sizeof(ejinfo->lpsu));
				m_ejinfo->lpbr = CString(ejinfo->lpbr, sizeof(ejinfo->lpbr));

				m_sCode = m_ejinfo->codx; m_sCode.Trim();
				m_memoexist = checkMemo(m_sCode);
				CString	imagePath; imagePath.Format("%s/%s/", Variant(homeCC, ""), IMAGEDIR);
				m_bitmap = getBitmap(imagePath + _T("ELW.bmp"));
				m_pBitmap = getBitmap(imagePath + _T("100.bmp"));
				m_sRate.Format("100");
				loadGubn(jHeader);
				m_ejinfo->hnam.TrimRight();
				m_sName = m_ejinfo->hnam;
				m_sData = m_sName;
				SetDataTooltip();
				Invalidate();
				
			}
			else if (HIWORD(wParam) >= L_hjinfo)
			{
				int	jrat = 0, nrat = 0;
				hjinfo = (struct _hjinfo*)szData.operator LPCTSTR();
				m_hjinfo = std::make_unique<Chjinfo>();

				m_hjinfo->codx = CString(hjinfo->codx, sizeof(hjinfo->codx));
				m_hjinfo->hnam = CString(hjinfo->hnam, sizeof(hjinfo->hnam));
				m_hjinfo->jgub = CString(hjinfo->jgub, sizeof(hjinfo->jgub));
				m_hjinfo->size = CString(hjinfo->size, sizeof(hjinfo->size));
				m_hjinfo->k2gb = CString(hjinfo->k2gb, sizeof(hjinfo->k2gb));
				m_hjinfo->usun = CString(hjinfo->usun, sizeof(hjinfo->usun));
				m_hjinfo->ksmm = CString(hjinfo->ksmm, sizeof(hjinfo->ksmm));
				m_hjinfo->amga = CString(hjinfo->amga, sizeof(hjinfo->amga));
				m_hjinfo->jbkm = CString(hjinfo->jbkm, sizeof(hjinfo->jbkm));
				m_hjinfo->siga = CString(hjinfo->siga, sizeof(hjinfo->siga));
				m_hjinfo->sjjs = CString(hjinfo->sjjs, sizeof(hjinfo->sjjs));
				m_hjinfo->jrat = CString(hjinfo->jrat, sizeof(hjinfo->jrat));
				m_hjinfo->nrat = CString(hjinfo->nrat, sizeof(hjinfo->nrat));	// 20070718
				

				// extended infomation
				m_hjinfo->krgb = CString(hjinfo->krgb, sizeof(hjinfo->krgb));
				m_hjinfo->jggb = CString(hjinfo->jggb, sizeof(hjinfo->jggb));
				m_hjinfo->lock = CString(hjinfo->lock, sizeof(hjinfo->lock));
				m_hjinfo->dyga = CString(hjinfo->dyga, sizeof(hjinfo->dyga));
				m_hjinfo->ujgb = CString(hjinfo->ujgb, sizeof(hjinfo->ujgb));
				m_hjinfo->ujcd = CString(hjinfo->ujcd, sizeof(hjinfo->ujcd));
				m_hjinfo->jisu = CString(hjinfo->jisu, sizeof(hjinfo->jisu));
				m_hjinfo->diff = CString(hjinfo->diff, sizeof(hjinfo->diff));
				m_hjinfo->rate = CString(hjinfo->rate, sizeof(hjinfo->rate));
				m_hjinfo->gvol = CString(hjinfo->gvol, sizeof(hjinfo->gvol));
				m_hjinfo->unio = CString(hjinfo->unio, sizeof(hjinfo->unio));
	
				m_sCode = m_hjinfo->codx; m_sCode.Trim();
				m_memoexist = checkMemo(m_sCode);
				m_hjinfo->hnam.TrimRight();
				m_sName = m_hjinfo->hnam;
				m_sData = m_sName;
				loadBitmap(jHeader->jgub, m_hjinfo->krgb, m_hjinfo->jrat);
				loadGubn(jHeader);
				m_hjinfo->jisu.Replace("+", ""); m_hjinfo->jisu.Replace("-", "");
				jrat = atoi(m_hjinfo->jrat);
				nrat = atoi(m_hjinfo->nrat);
				if (jrat > 0)
				{
					m_sData.Format("%s", m_hjinfo->hnam);
					m_sRate.Format("%d", jrat);
				}
				
				SetDataTooltip();
				Invalidate();
			}
			else
			{
				m_sData.Empty(); m_bitmap = NULL; Invalidate();
			}

		//	((CControlWnd*)GetParent())->SetEditFocus();
		}
		break;
	case DLL_ALERTx:
		WriteAlertx((struct _alertR *)lParam);
		break;
	case DLL_ALERT:
	//	WriteAlert((char*)lParam);
		break;
	case DLL_TRIGGER:
		break;
	case DLL_DOMINO:
		break;
	case DLL_NOTICE:
		break;
	case DLL_SETPAL:
		Invalidate();
		break;
	case DLL_SETFONT:
		{
		}
		break;
	}
	return 0;
}

void CSymbolWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CString txt;
	CPen	Pen;
	CRect	rc, rect;
	COLORREF bxColor = 0, bgColor = 0, fgColor = 0;
	const	CControlWnd* pWnd = (CControlWnd*)GetParent();

	GetClientRect(&rc);
	// set font
	CFont	*pFont = GetAxFont(pWnd->m_Param.fonts, pWnd->m_Param.point, pWnd->m_Param.style);
	CFont	*pOldFont = (CFont*)dc.SelectObject(pFont);

	if (!m_bHover)
	{
		// boxcolor
		bxColor = m_boxColor;
		bgColor = m_bgColor;
		fgColor = GetIndexColor(69);
	}
	else
	{
		bxColor = m_boxColorHover;
		bgColor = m_bgColorHover;
		fgColor = GetIndexColor(69);
	}

	Pen.CreatePen(PS_SOLID, 0, bxColor);
	CPen	*pOldPen = (CPen*)dc.SelectObject(&Pen);
	dc.Rectangle(rc);
	dc.SelectObject(pOldPen);
	Pen.DeleteObject();

	rc.DeflateRect(1, 1);

	if (GetState() & 0x0004)
		dc.FillSolidRect(rc, GetSysColor(COLOR_INACTIVEBORDER));
	else
		dc.FillSolidRect(rc, bgColor);

	rc.InflateRect(1, 1);
	drawTriangle(&dc, rc, RGB(0, 0, 0));

	rect = rc; rect.left = rect.right - indicatorW;
//	drawIndicator1(&dc, rect, RGB(128, 128, 128));	// 종목정보

	if (m_memoexist)
		drawIndicator2(&dc, rect, RGB( 68, 225, 115));	// 메모여부
	rc.right -= indicatorW;

	rc.DeflateRect(0, 1);
	// bitmap draw
	rect = rc; rect.right = rect.left + bitmapW;
	rect.DeflateRect(CSize(2, 2));
	drawBitmap(&dc, rect, m_bitmap);


	//===========================================	증거금	==================================================
	rect.OffsetRect(bitmapW - 1, 0);
	rect.InflateRect(CSize(2, 0));

	if (!m_sRate.IsEmpty())
	{
		if (atoi(m_sRate) == 100)
			drawBitmap(&dc, rect, m_pBitmap);
		else 
		{
			dc.FillSolidRect(&rect, RGB(165, 165, 165));
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(RGB(255, 255, 255));
			CFont font, *poldfont;
			font.CreatePointFont(80, "굴림체");
			poldfont = dc.SelectObject(&font);
			dc.DrawText(m_sRate, rect, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
			dc.SelectObject(poldfont);
			font.DeleteObject();
		}
	}

	rc.left += 2*bitmapW + 1;
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(fgColor);
	dc.DrawText(m_sData, rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		
	dc.SelectObject(pOldFont);
}

void CSymbolWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!m_capture)
	{
		SetCapture();
		SetFocus();
		m_capture = true;
		m_bMouseOver = true;
		Invalidate();
	}
	
	CAxWnd::OnLButtonDown(nFlags, point);
}

void CSymbolWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_capture)
	{
		CRect rc;
		m_capture = false;
		ReleaseCapture();
		GetClientRect(&rc);
		Invalidate();

		if (!m_sData.IsEmpty() && rc.PtInRect(point))
		{
			GetWindowRect(&rc);

			if (m_pPopup)
				m_pPopup.reset();

			int	cx = 320, cy = m_ujongsise ? 190 : 150;
			if (m_ejinfo)
				cx += 50, cy += 40;

			CString sClassName  = AfxRegisterWndClass(0);
			m_pPopup = std::make_unique<CInfoPopup>(m_pWizard);
			m_pPopup->setPathInfo(Variant(homeCC, ""), Variant(nameCC, ""));
			if (!m_pPopup->CreateEx(WS_EX_TOOLWINDOW|WS_EX_TOPMOST/*|WS_EX_DLGMODALFRAME*/, sClassName, NULL,
				WS_POPUP/*|WS_BORDER*/, CRect(rc.left, rc.bottom, rc.left + cx, rc.bottom + cy),
				NULL, NULL, NULL))
			{
				m_pPopup.reset();
				return;
			}

			m_pPopup->setJinfo(m_hjinfo.get(), m_ejinfo.get(), m_ujongsise);
			m_pPopup->ShowWindow(SW_SHOW);
		}

		m_memoexist = checkMemo(m_sCode); Invalidate();
	}
	
	CAxWnd::OnLButtonUp(nFlags, point);
}

void CSymbolWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	CRect rc; GetClientRect(&rc);
	if (rc.PtInRect(point))
	{
		if (!m_bMouseOver)
			Invalidate();
		m_bMouseOver = true;
	}
	else
	{
		if (m_bMouseOver)
			Invalidate();
		m_bMouseOver = false;
	}

	CAxWnd::OnMouseMove(nFlags, point);
}

BOOL CSymbolWnd::GetState()
{
	if (m_capture && m_bMouseOver)
		return 0x0004;
	return 0;
}

void CSymbolWnd::SetCheck(bool bCheck)
{
	m_bCheck = bCheck; Invalidate();
}

bool CSymbolWnd::GetCheck()
{
	return m_bCheck;
}

void CSymbolWnd::OnDestroy() 
{
	if (m_pTipCtrl)
		m_pTipCtrl.reset();
	CWnd::OnDestroy();	
}

LOGFONT CSymbolWnd::setFont(CString fName, int fsize, int style)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = fsize * 10; 
	
	strcpy(lf.lfFaceName, fName);  
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	switch (style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		lf.lfItalic = TRUE;
		break;
	case 3: // bold
		lf.lfWeight = FW_BOLD;
		break;
	case 4: // both
		lf.lfItalic = TRUE;
		lf.lfWeight = FW_BOLD;
		break;
	}

	return lf;
}

void CSymbolWnd::drawTriangle(CDC *pDC, CRect rc, COLORREF clr)
{
	if (rc.Width() < 3)
		return;

	const	CSize	sz(10, 8);

	pDC->FillSolidRect(rc.right - sz.cx, rc.bottom - sz.cy, 5, 1, clr);
	pDC->FillSolidRect(rc.right - sz.cx + 1, rc.bottom - sz.cy + 1, 3, 1, clr);
	pDC->FillSolidRect(rc.right - sz.cx + 2, rc.bottom - sz.cy + 2, 1, 1, clr);
}

// updateX_20060110
void CSymbolWnd::drawIndicator1(CDC* dc, CRect rect, COLORREF clr)
{
	CPoint	pts[3];
	int	gap = 0, height = 0;

	CPen*   oldPen   = dc->SelectObject(GetAxPen(clr, 1, PS_SOLID));
	CBrush* oldBrush = dc->SelectObject(GetAxBrush(clr));

	dc->SetPolyFillMode(ALTERNATE);

	rect.DeflateRect(1, 1, 2, 2);
	gap    = rect.Height() / 6;
	height = (rect.Height() - gap) / 2;

	rect.top  = rect.bottom - height;
	rect.left = rect.right - height;

	pts[0].x = rect.left;
	pts[0].y = rect.bottom;
	pts[1].x = rect.right;
	pts[1].y = rect.top;
	pts[2].x = rect.right;
	pts[2].y = rect.bottom;
	dc->Polygon(pts, 3);

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);
}

// updateX_20060110, 메모표시여부
void CSymbolWnd::drawIndicator2(CDC* dc, CRect rect, COLORREF clr)
{
	CPoint	pts[3];
	int	gap = 0, height = 0;

	CPen*   oldPen   = dc->SelectObject(GetAxPen(clr, 1, PS_SOLID));
	CBrush* oldBrush = dc->SelectObject(GetAxBrush(clr));

	dc->SetPolyFillMode(ALTERNATE);

	rect.DeflateRect(1, 1, 2, 2);
	gap    = rect.Height() / 5;
	height = (rect.Height() - gap) / 2;

	rect.bottom = rect.top + height;
	rect.left   = rect.right - height;

	pts[0].x = rect.left;
	pts[0].y = rect.top;
	pts[1].x = rect.right;
	pts[1].y = rect.bottom;
	pts[2].x = rect.right;
	pts[2].y = rect.top;
	dc->Polygon(pts, 3);

	dc->SelectObject(oldBrush);
	dc->SelectObject(oldPen);
}

void CSymbolWnd::drawBitmap(CDC *pDC, CRect rect, CBitmap *pBitmap)
{
//	rect.DeflateRect(CSize(2, 2));

	if (pBitmap)
	{
		CDC	memDC;
		CBitmap	*pSBitmap = nullptr;

		BITMAP stBitmapInfo;
		pBitmap->GetBitmap(&stBitmapInfo);

		memDC.CreateCompatibleDC(pDC);	
		if (memDC.m_hDC != NULL) 
			pSBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
		pDC->StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(),
				&memDC, 0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, SRCCOPY);
		if (pSBitmap)
			memDC.SelectObject(pSBitmap);
		memDC.DeleteDC();
	}
}

void CSymbolWnd::WriteAlert(CString str)
{
	if (m_hjinfo)
	{
		CString sym, dat;
		bool bChanged = false;
		while (!str.IsEmpty())
		{
			sym = Parser(str, "\t");
			dat = Parser(str, "\t");

			switch (atoi(sym))
			{
			case 23:	// 현재가
				m_hjinfo->jisu = dat;
				m_hjinfo->jisu.Replace("+", "");
				m_hjinfo->jisu.Replace("-", "");
				bChanged = true;
				break;
			case 24:	// 전일대비
				m_hjinfo->diff = dat;
				bChanged = true;
				break;
			case 33:	// 등락율
				m_hjinfo->rate = dat;
				bChanged = true;
				break;
			case 27:	// 거래량
				m_hjinfo->gvol = dat;
				bChanged = true;
				break;
			}
		}
		if (bChanged && m_pPopup)
		{
			m_pPopup->setJinfo(m_hjinfo.get(), m_ejinfo.get(), m_ujongsise);
			m_pPopup->Invalidate();
		}
	}
}

void CSymbolWnd::WriteAlertx(struct _alertR* alertR)
{
	if (m_hjinfo)
	{
		if (m_sCode.CompareNoCase(alertR->code) != 0)
			return;

		bool	bChanged = false;
		DWORD* data = (DWORD*)alertR->ptr[0];

		if (data[23])	// 현재가
		{
			m_hjinfo->jisu = (char*)data[23];
			m_hjinfo->jisu.Replace("+", "");
			m_hjinfo->jisu.Replace("-", "");
			bChanged = true;
		}

		if (data[24])	// 전일대비
		{
			m_hjinfo->diff = (char*)data[24];
			bChanged = true;
		}

		if (data[33])	// 등락율
		{
			m_hjinfo->rate = (char*)data[33];
			bChanged = true;
		}
				
		if (data[27])	// 거래량
		{
			m_hjinfo->gvol = (char*)data[27];
			bChanged = true;
		}

		if (bChanged && m_pPopup)
		{
			m_pPopup->setJinfo(m_hjinfo.get(), m_ejinfo.get(), m_ujongsise);
			m_pPopup->Invalidate();
		}
	}
}

void CSymbolWnd::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	m_bHover = TRUE;
	Invalidate();
}

LRESULT CSymbolWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover = FALSE;
	Invalidate();
	return 0;
}

CBitmap* CSymbolWnd::getBitmap(CString path)
{
	return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (LPARAM)path.GetString());
}

// updateX_20060110
bool CSymbolWnd::checkMemo(CString sKey)
{
	if (sKey.IsEmpty())
		return false;

	CString dir, dat;
	char	key[12]{}, lBytes[4]{};

	dir.Format("%s\\%s\\%s\\memo.mmo", Variant(homeCC, ""), USRDIR, Variant(nameCC, ""));
	CFileFind	finder;
	if (!finder.FindFile(dir))
		return false;

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

					if (!sKey.Compare(key) && lSize > 0)
					{
						dat.ReleaseBuffer();
						return true;
					}

					dat.ReleaseBuffer();
				}
				else
					break;
			}
			else
				break;
				
		}while ((int)nBytesRead);
		file.Close();
	}
	CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	return false;
}


void CSymbolWnd::loadBitmap(CString jgub, CString kind, CString jrat)
{
	CString	imagePath;
	
	imagePath.Format("%s/%s/", Variant(homeCC, ""), IMAGEDIR);
	m_pBitmap = getBitmap(imagePath + _T("100.bmp"));
	kind.Trim();

	if (!kind.CollateNoCase("거래정지"))
		m_bitmap = getBitmap(imagePath + _T("거래정지.bmp"));	
	else if (!kind.CompareNoCase("관리종목"))
		m_bitmap = getBitmap(imagePath + _T("관리종목.bmp"));
	else if (!kind.CompareNoCase("이상급등")) 
		m_bitmap = getBitmap(imagePath + _T("이상급등.bmp"));
	else if (!kind.CompareNoCase("투자주의")) 
		m_bitmap = getBitmap(imagePath + _T("투자주의.bmp"));		
	else if (!kind.CompareNoCase("투자위험")) 
		m_bitmap = getBitmap(imagePath + _T("투자위험.bmp"));
	else if	(!kind.CompareNoCase("투자경고"))
		m_bitmap = getBitmap(imagePath + _T("투자경고.bmp"));
	else if (!kind.CompareNoCase("투자유의"))
		m_bitmap = getBitmap(imagePath + _T("투자유의.bmp"));
	else if (!kind.CompareNoCase("불성실공시"))
		m_bitmap = getBitmap(imagePath + _T("불성실공시.bmp"));
	else if (!kind.CompareNoCase("정상") && jgub.GetLength() >= 1)
	{
		switch (jgub.GetAt(0))
		{
		case '1':	
		//================ 거래소	========================================
			m_bitmap = getBitmap(imagePath + _T("거래소.bmp"));
			break;
		case '2':	
		//================ 코스닥	========================================
			m_bitmap = getBitmap(imagePath + _T("코스닥.bmp"));
			break;
		case '3':	
		//================ 프리보드	========================================
			m_bitmap = getBitmap(imagePath + _T("BTN_JANG.bmp"));
			break;
		case '4':	
		//================ ETF	================================================
			m_bitmap = getBitmap(imagePath + _T("거래소.bmp"));
			break;
		case '5':
		//================ ELW	================================================
			m_bitmap = getBitmap(imagePath + _T("ELW.bmp"));
			break;
		}
	}
	else
	{
		m_bitmap = NULL;
	}
}

int CSymbolWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CAxWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
//	m_boxColor	= GetIndexColor(89);
//	m_boxColorHover = GetIndexColor(91);
//	m_pRgbHover	= GetIndexColor(92);
//	m_tRgbHover	= GetIndexColor(69);

	return 0;
}

const char*	gSYMBOL	= _T("$90303");
const char	gnCHAR	= 0x7f;
const char	tabCHAR	= 0x09;
const char	lfCHAR	= 0x0A;

void CSymbolWnd::Send(CString szCode)
{
	CString trData;

	szCode.TrimLeft();
	trData.Format("1301%c%s%c1301%c17413%c", gnCHAR, szCode, tabCHAR, tabCHAR, tabCHAR);
	SendTRC("POOPPOOP", trData, US_OOP);
}

void CSymbolWnd::loadGubn(struct _jHeader* pinfo)
{
	switch (pinfo->jgub[0])
	{
	case '1':
		{
		//=============	거래소	============================================
		m_szMarket = _T("KOSPI");

		//	=======	코스피 200 추가	     ========================================================================
		if (m_hjinfo)
		{
			CString str = CString(m_hjinfo->k2gb);
			if (str.Left(4).CompareNoCase("K200") == 0 && !(str.Left(12).CompareNoCase("K200(미채용)") == 0))
				m_szMarket = _T("K200");
			//	=====================================================================================================
			}
		}
		break;
	case '2':
	//============= 코스닥	============================================
		m_szMarket = _T("KOSDAQ");
		break;
	case '3':
	//============= 프리보드 ===========================================
		m_szMarket = _T("K-OTC");
		break;
	case '4': 
	//============= ETF	===========================================
		m_szMarket = _T("ETF");
		break;
	case '5':
	//============= ELW	============================================
		m_szMarket = _T("ELW");
		break;
	default:
		m_szMarket = _T("");
		break;
	}

	m_szUpjong = m_szDanger = "";

	if (pinfo->jgub[0] != '5' && m_hjinfo)
	{
		m_szUpjong = m_hjinfo->ujgb;
		m_szUpjong.TrimRight();
		m_szUpjong.Replace(" ", "");

		// 시장조치...
		m_szDanger = m_hjinfo->krgb;

		m_szDanger.TrimRight();
		if (m_szDanger.CompareNoCase("정상") == 0)
		{
			CString szTemp = CString(m_hjinfo->lock, 12);
			szTemp.TrimRight();
			if (szTemp.CompareNoCase("권리락") == 0)
				m_szDanger = szTemp;
			else if (szTemp.CompareNoCase("배당락") == 0)
				m_szDanger = szTemp;
			else if (szTemp.CompareNoCase("권배락") == 0)
				m_szDanger = szTemp;

		}

		m_szDanger.TrimRight();
	}

	CRect rc;
	GetParent()->GetClientRect(rc);
	rc.left = 120;
	GetParent()->InvalidateRect(rc);
}

void CSymbolWnd::CreateTooltip(CString str)
{	
	if (m_pTipCtrl)
		m_pTipCtrl.reset();

	CRect	rc;

	GetClientRect(&rc);
	m_pTipCtrl = std::make_unique<CToolTipCtrl>();
	m_pTipCtrl->Create(this);
	m_pTipCtrl->SetDelayTime(0);
	m_pTipCtrl->SetMaxTipWidth(320);
	m_pTipCtrl->AddTool(this, str, &rc, GetDlgCtrlID());
	m_pTipCtrl->Activate(TRUE);
}

LRESULT CSymbolWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = message;
	msg.wParam = wParam;
	msg.lParam = lParam;
	
	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
	{
		if (m_pTipCtrl)
			m_pTipCtrl->RelayEvent(&msg);
	}
	return CAxWnd::WindowProc(message, wParam, lParam);
}

void CSymbolWnd::Clear()
{
	m_bitmap = NULL;
	m_sRate.Empty();
	m_sData.Empty();

	Invalidate();
}

void CSymbolWnd::SetDataTooltip()
{
	// line 1
	CString string, temp, tmpx;
	CString sInfo;
	int	ival = 0;
	const	char	sGubn[][20] = {"", "거래소", "코스닥", "제3시장", "ETF", "ELW"};
	CStringArray stringAry;
	const	CjHeader* jHeader = (m_hjinfo) ? (CjHeader*)m_hjinfo.get() : (CjHeader*)m_ejinfo.get();

	if (jHeader == NULL)
		return;

	ival = atoi(jHeader->jgub);
	temp.Format("%s", (ival >= 0 && ival <= 4 ? sGubn[ival] : "" )); // 거래소 구분

	if (ival == 5)	// elw
	{
		if (m_ejinfo == NULL)
			return;
		if (!temp.IsEmpty())
		{
			string = temp;
		}
		// Line 1
		temp.Format("기초자산 %s", m_ejinfo->gcjs); temp.Trim();// 기초자산
		string += temp + " | ";
		temp.Format("%s", m_ejinfo->krye); temp.TrimRight();	// 권리유형
		string += temp + " | ";
		temp.Format("%s", m_ejinfo->khbe); temp.TrimRight();	// 권리행사방법
		string += temp;
		stringAry.Add(string);

		// Line 2
		temp.Format("%s", m_ejinfo->gjbe); temp.TrimRight();	// 결제방법
		string = temp + " | ";
		temp.Format("%s", m_ejinfo->hbge); temp.TrimRight();	// 한글발행기관명
		string += temp;
		stringAry.Add(string);

		// Line 3
		temp.Format("%s", m_ejinfo->sjjs); temp.Trim();		// 발행주식수 (천)
		string = "발행주식수 " + temp + " 천주 | ";
		temp.Format("%s", m_ejinfo->sgca); temp.Trim();		// 발행가 (시가총액:억)
		string += "시가총액 " + temp + "억";
		stringAry.Add(string);

		// Line 4
		temp.Format("%s", m_ejinfo->hsga); temp.TrimRight();	// 행사가격
		string = "행사가 " + temp + "  | ";
		temp.Format("%s", m_ejinfo->jhre); temp.TrimRight();	// 전환비율
		string += "전환비율 " + temp + " | ";
		temp.Format("%s", m_ejinfo->jjis); temp.Trim();		// 잔존일수
		string += "잔존일수 " + temp + " 일 ";
		
		stringAry.Add(string);

		// Line 5
		temp.Format("%s", m_ejinfo->mgil); temp.TrimRight();	// 상장일
		string = "만기일 " + temp + "  | ";
		temp.Format("%s", m_ejinfo->edte); temp.TrimRight();	// 최종거래일
		string += "최종거래일 " + temp;
		stringAry.Add(string);

		// Line 6
		temp.Format("%s", m_ejinfo->sjil); temp.TrimRight();	// 상장일
		string = "상장일 " + temp + "  | ";
		temp.Format("%s", m_ejinfo->jgil); temp.TrimRight();	// 최종거래일
		string += "지급일 " + temp;
		stringAry.Add(string);
		
		// Line 7
		temp.Format("%s", m_ejinfo->lpsa); temp.TrimRight();	// LP회원사
		string = "LP회원사 " + temp + "  | ";
		temp.Format("%s", m_ejinfo->lpjg); temp.TrimRight();	// LP주문가능여부
		string += "LP주문가능여부 " + temp;
		stringAry.Add(string);
		
		// Line 8
		temp.Format("%s", m_ejinfo->lpsu); temp.TrimRight();	// LP회원사
		string = "LP보유수량 " + temp + "  | ";
		temp.Format("%s", m_ejinfo->lpbr); temp.TrimRight();	// LP주문가능여부
		string += "LP보유비율 " + temp;
		stringAry.Add(string);
		
		sInfo = m_ejinfo->hnam + "\n";
	}
	else
	{
		if (m_hjinfo == NULL)
			return;
		if (!temp.IsEmpty())
		{
			string += temp + " | ";
			temp.Format("%s", m_hjinfo->size); temp.TrimRight();		// 기업규모
			string += temp + " | ";
			temp.Format("%s", m_hjinfo->k2gb); temp.TrimRight();		// 소구분
			string += temp;
			stringAry.Add(string);
		}

		// line 2
		string = _T("");

		temp.Format("%s", m_hjinfo->usun); temp.TrimRight();			// 보통주,우선주...
		string += temp;

		stringAry.Add(string);


		// line 3
		string = _T("");

		ival = atoi(m_hjinfo->ksmm);
		temp.Format("%s %02d%s", "결산월", ival, "월");				// 결산월
		string += temp + " | ";

		//tmpx.Format("%.f", atof(m_hjinfo->amga)); tmpx = SetComma(tmpx);
		//temp.Format("%s %s%s", "액면가", tmpx, "원"); temp.TrimRight();
		//string += temp;
		m_hjinfo->amga.TrimRight();
		string += "액면가 " + m_hjinfo->amga;
		
		stringAry.Add(string);

		// line 4
		string = _T("");

		//tmpx.Format("%.f", atof(m_hjinfo->jbkm)); tmpx = SetComma(tmpx);
		//temp.Format("%s %s%s", "자본금", tmpx, "억");
		//string += temp;
		m_hjinfo->jbkm.TrimRight();
		string += "자본금 " + m_hjinfo->jbkm;
		
		stringAry.Add(string);

		// line 5
		string = _T("");

		tmpx.Format("%.f", atof(m_hjinfo->siga)); tmpx = Comma(tmpx);
		temp.Format("%s %s%s", "전일시가총액", tmpx, "억");
		string += temp;
		
		stringAry.Add(string);

		// line 6
		string = _T("");

		tmpx.Format("%.f", atof(m_hjinfo->sjjs)); tmpx = Comma(tmpx);
		temp.Format("%s %s%s", "상장주식수", tmpx, "천주");
		string += temp;

		stringAry.Add(string);

		
		

		sInfo = m_hjinfo->hnam + "\n";
	}

	for (int ii = 0; ii < stringAry.GetSize(); ii++)
		sInfo += stringAry.GetAt(ii) + "\n";

	CreateTooltip(sInfo);
}
