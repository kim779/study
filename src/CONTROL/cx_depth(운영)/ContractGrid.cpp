// ContractGrid.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Depth.h"
#include "ContractGrid.h"
#include "ContractScroll.h"
#include "ContractWnd.h"
#include "mmsystem.h"

#include "../../H/memDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContractGrid

CContractGrid::CContractGrid(CWnd *pWizard, CWnd *pParent)
{
	m_pWizard = pWizard;
	m_pParent = pParent;
	m_pApp = (CCX_DepthApp*)AfxGetApp();

	m_pScroll = nullptr;

	m_rcData.SetRectEmpty();
	
	m_SCRpos = m_SCRday = m_SCRend = m_SCRtotal = 0;
	m_iDispCnt = 10;
	m_iHalfLine = 0;
	m_pFont = nullptr;

	m_showTitle = FALSE;
	m_iHalfInfo = 0;

	m_pColor[clr_UpIndex] = m_pColor[clr_NormalIndex] = m_pColor[clr_DnIndex] = RGB(0,0,0);
}

CContractGrid::~CContractGrid()
{
	for (int n=0; n<3; ++n)
		eq_bmp[n].DeleteObject();
}


BEGIN_MESSAGE_MAP(CContractGrid, CWnd)
	//{{AFX_MSG_MAP(CContractGrid)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SCRL, OnVScroll)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CContractGrid message handlers
long CContractGrid::OnVScroll(WPARAM wParam, LPARAM lParam)
{
	int	pos = 0;

	switch (LOWORD(wParam))
	{
	case SCRL_VCHANGE:
		switch ((int)HIWORD(wParam))
		{
		case SB_LINEUP:
			if (m_SCRpos < 1)
				break;

			m_SCRpos--;
			m_SCRend--;

			UpdateScroll(m_SCRpos, m_SCRend, m_SCRday, m_SCRtotal);
			Invalidate();

			break;
		case SB_LINEDOWN:
			if (m_SCRend >= m_SCRtotal)
				break;

			m_SCRpos++;
			m_SCRend++;

			UpdateScroll(m_SCRpos, m_SCRend, m_SCRday, m_SCRtotal);
			Invalidate();

			break;
		case SB_PAGEUP:
			if (m_SCRpos < 1)
				break;

			m_SCRpos -= m_iDispCnt - 1;
			m_SCRend -= m_iDispCnt - 1;

			if (m_SCRpos < 0)
			{
				m_SCRend -= m_SCRpos;
				m_SCRpos = 0;
			}

			UpdateScroll(m_SCRpos, m_SCRend, m_SCRday, m_SCRtotal);
			Invalidate();

			break;
		case SB_PAGEDOWN:
			if (m_SCRend >= m_SCRtotal)
				break;

			m_SCRpos += m_iDispCnt - 1;
			m_SCRend += m_iDispCnt - 1;

			if (m_SCRend > m_SCRtotal)
			{
				m_SCRpos -= m_SCRend - m_SCRtotal;
				m_SCRend = m_SCRtotal;
			}

			UpdateScroll(m_SCRpos, m_SCRend, m_SCRday, m_SCRtotal);
			Invalidate();

			break;
		case SB_THUMBTRACK:
			pos = (int)lParam;
			if (pos >= 0)
			{
				int	scrPos = (int)((pos) * (m_SCRtotal) / 100); 
				int	scrEnd = m_SCRday + scrPos;
				if (scrEnd > m_SCRtotal)
				{
					const	int	gap = scrEnd - m_SCRtotal;
					scrPos -= gap;
					scrEnd -= gap;
				}
				m_SCRpos = scrPos;
				m_SCRend = scrEnd;

				UpdateScroll(m_SCRpos, m_SCRend, m_SCRday, m_SCRtotal);
				Invalidate();
			}
			break;
		}
		break;
	}
	
	return 0;
}

int CContractGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_pScroll == nullptr)	// invalidate()에서 깜빡임이 나타나므로  Parent를 상위 윈도우를 준다.
	{
		m_pScroll = std::make_unique<CContractScroll>();
		m_pScroll->Create(WS_CHILD|WS_VISIBLE|SBS_VERT|SBS_BOTTOMALIGN | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), GetParent(), 0);
		m_pScroll->SetOwner(this);
		m_pScroll->EnableWindow(FALSE);
	}

	LoadEqualizer();
	
	return 0;
}

void CContractGrid::OnDestroy() 
{
	while (!m_match.IsEmpty())
		m_match.RemoveHead().reset();

	if (m_pScroll)
		m_pScroll.reset();

	CWnd::OnDestroy();
}

void CContractGrid::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	xxx::CMemDC	mdc(&dc);
	DrawData(&mdc);
}

void CContractGrid::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);	
	Resize();
}

void CContractGrid::Resize()
{
	CRect	rc;
	GetClientRect(&rc);

	m_rcData = rc;
	if (m_iHalfInfo == 0)
		m_iHalfLine = (int)(m_rcData.Width() * (50 / 100.0));
	else
		m_iHalfLine = m_iHalfInfo;

	MakeEqualizerBmp();
	
	Invalidate();
}

void CContractGrid::SetFontx(CFont *pFont)
{
	m_pFont = pFont;
	Resize();
}

void CContractGrid::InsertData(const char* price, const char* contract, int cp, int cc)
{
	while (((CContractWnd*)m_pParent)->GetMax() < m_match.GetCount())
		m_match.RemoveTail().reset();

	m_match.AddHead(std::make_shared<CMatch>(price, contract, cp, cc));

	AutoUpdate();
}

void CContractGrid::AddData(const char* price, const char* contract, int cp, int cc)
{
	m_match.AddTail(std::make_shared<CMatch>(price, contract, cp, cc));
}

void CContractGrid::AutoScroll()
{
	m_SCRtotal = GetCount();

	if (m_SCRtotal <= 0)
	{
		m_SCRpos = m_SCRday = m_SCRend = 0;
	}

	if (m_SCRday < m_iDispCnt)
	{
		m_SCRend = m_SCRday = m_SCRtotal;
	}

	UpdateScroll(m_SCRpos, m_SCRend, m_SCRday, m_SCRtotal);
}

void CContractGrid::UpdateScroll(int pos, int end, int day, int total)
{
	SCROLLINFO sInfo;
	sInfo.cbSize = sizeof(SCROLLINFO);
	sInfo.fMask = SIF_ALL|SIF_DISABLENOSCROLL;     
	sInfo.nMin = 0;
	sInfo.nMax = 100;

	if ((total) == 0)
	{
		sInfo.nPos = 100;
		sInfo.nPage = 110;
	}
	else
	{
		sInfo.nPos = (end == total)? 100:(pos*100) / (total);
		sInfo.nPage = (day*100) / (total);
	}

	if ((int)sInfo.nPage == sInfo.nMax)
		sInfo.nPage = sInfo.nMax+1;

	if (m_pScroll)
		m_pScroll->Initialize(sInfo);
	m_pScroll->Invalidate();
}

void CContractGrid::Clear()
{
	while (!m_match.IsEmpty())
		m_match.RemoveHead().reset();

	AutoScroll();
	Invalidate();
}

void CContractGrid::PaletteGrid(COLORREF upColor, COLORREF downColor, COLORREF normalColor, COLORREF lineColor, COLORREF gridColor1st, COLORREF gridColor2st)
{
	m_colorUp = upColor;
	m_colorDown = downColor;
	m_colorNormal = normalColor;
	m_colorLine = lineColor;
	m_colorGrid1st = gridColor1st;
	m_colorGrid2st = gridColor2st;

	m_pColor[clr_UpIndex] = m_colorUp;
	m_pColor[clr_NormalIndex] = m_colorNormal;
	m_pColor[clr_DnIndex] = m_colorDown;
}

void CContractGrid::DrawData(CDC *pDC)
{
	CRect	rect, cRc, rcFill, rcTemp;

	const	COLORREF clr = RGB(227, 227, 227);
	rect = m_rcData;

	if (m_pFont == nullptr)
		return;

	CFont	*pOldFont = pDC->SelectObject(m_pFont);
	CPen	*pPen = m_pApp->GetPen(m_pWizard, m_colorLine, 1);
	CPen	*pPenWhite = m_pApp->GetPen(m_pWizard, RGB(255, 255, 255), 1);
	CPen	*pOldPen = pDC->SelectObject(pPen);
	
	pDC->FillSolidRect(&rect, RGB(255, 255, 255));

	CString sPrice, sContract;
	const	double	height = double(rect.Height()) / double(m_iDispCnt + (m_showTitle ? 1 : 0));
	const	COLORREF	sColor = pDC->SetTextColor(RGB(0,0,0));
	int	row = 0;
	POSITION pos{};
	std::shared_ptr<CMatch>	match = nullptr;

	for (int ii = 0; ii < m_SCRday; ii++)
	{
		row = ii + (m_showTitle ? 1 : 0);
		rcFill.SetRect(0, (int)(row * height), rect.Width() - 1, (int)((row + 1) * height));
		
		if (ii % 2 == 0)
			pDC->FillSolidRect(rcFill, m_colorGrid1st);
		else
			pDC->FillSolidRect(rcFill, m_colorGrid2st);

		pos = m_match.FindIndex(ii + m_SCRpos);
		if (pos)
		{
			match = m_match.GetAt(pos);
			if (match)
			{
				sPrice = match->price;
				sContract = match->contract;
			}
		}
		else 
		{
			sPrice = sContract = "";
		}

		cRc.SetRect(0, (int)(row * height)+2, m_iHalfLine - 1, (int)((row + 1) * height));
		if (match)
			pDC->SetTextColor(m_pColor[match->cp]);
		pDC->DrawText(sPrice, cRc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

		cRc.SetRect(cRc.right, cRc.top, rect.Width() - 1, cRc.bottom);
		if (match)
			pDC->SetTextColor(m_pColor[match->cc]);
		pDC->DrawText(sContract, cRc, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
	}

	if (m_showTitle)
	{
		pDC->SelectObject(pPenWhite);
		pDC->MoveTo(rect.right, rect.top);
		pDC->LineTo(rect.left, rect.top);
		pDC->LineTo(rect.left, rect.bottom-1);
		pDC->LineTo(rect.right, rect.bottom-1);

		rcTemp.SetRect(rect.left, rect.top, rect.right, rect.top + (int)height);
		pDC->FillSolidRect(rcTemp, clr);
		pDC->SelectObject(pPen);
		
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->DrawText("체결가", CRect(rcTemp.left, rcTemp.top, rcTemp.left + m_iHalfLine, rcTemp.bottom), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		pDC->DrawText("체결량", CRect(rcTemp.left + m_iHalfLine, rcTemp.top, rcTemp.right, rcTemp.bottom), DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
		pDC->MoveTo(rcTemp.left, rcTemp.bottom);
		pDC->LineTo(rcTemp.right, rcTemp.bottom);
	}

	pDC->SelectObject(pPen);
	pDC->MoveTo(m_iHalfLine, rect.top);
	pDC->LineTo(m_iHalfLine, rect.bottom);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.right, rect.top);
	pDC->LineTo(rect.left, rect.top);

	if (!m_match.IsEmpty())
		DrawEqualizer(pDC, CRect(m_iHalfLine+1, 1, m_iHalfLine+max(rect.Width()/12, 3), rect.Height()));

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
}

BOOL CContractGrid::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;//CWnd::OnEraseBkgnd(pDC);
}

void CContractGrid::AutoUpdate()
{
	Invalidate();
	AutoScroll();
}

void CContractGrid::ChangeDisplay(int display)
{
	m_iDispCnt = display;
	AutoScroll();
	Invalidate();
}

void CContractGrid::LoadEqualizer()
{
	const	int	def_vols[] = {80,60,40,20,0};
	const	int	def_levs[] = {500, 1000, 3000, 5000, 10000};
	CString path, key;
	const	char*	eq_section = "EQUALIZER";
	char	fname[MAX_PATH]{};
	
	const	char*	buff = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), (LPARAM)0);
	const	char*	name = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), (LPARAM)0);
	path.Format("%s\\user\\%s\\%s.ini", buff, name, name);
	
	m_eq_cnt = 5;
	m_eq_type = max(1, GetPrivateProfileInt(eq_section, "TYPE", 1, path));
	for (int ii = 0; ii < 5; ii++)
	{
		key.Format("VOL%02d", ii+1);
		m_eq_vol[ii] = GetPrivateProfileInt(eq_section, key, def_vols[ii], path);
		
		key.Format("LEV%02d", ii+1);
		m_eq_lev[ii] = GetPrivateProfileInt(eq_section, key, def_levs[ii], path);
		
		key.Format("DCOL%02d", ii+1);
		m_eq_mdcol[ii] = GetPrivateProfileInt(eq_section, key, RGB(0xE0-(ii*48), 0xE0-(ii*48), 0xFF), path);
		
		key.Format("SCOL%02d", ii+1);
		m_eq_mscol[ii] = GetPrivateProfileInt(eq_section, key, RGB(0xFF, 0xE0-(ii*48), 0xE0-(ii*48)), path);
		
		m_eq_nmcol[ii] = RGB(0xE0,0xE0,0xE0);
	}
	for (int ii = 4; ii >= 0; ii--)
	{
		if (ii == 0)
			m_eq_vol[ii] = (100 - m_eq_vol[ii]);
		else
			m_eq_vol[ii] = (m_eq_vol[ii-1]-m_eq_vol[ii]);
	}
	
	m_eq_alarm[0] = GetPrivateProfileInt(eq_section, "MADO_ALARM", 0, path);
	m_eq_alarm[1] = GetPrivateProfileInt(eq_section, "MASU_ALARM", 0, path);
	m_eq_alarm_lev[0] = GetPrivateProfileInt(eq_section, "MADO_ALARM_LEV", 0, path);
	m_eq_alarm_lev[1] = GetPrivateProfileInt(eq_section, "MASU_ALARM_LEV", 0, path);
	GetPrivateProfileString(eq_section, "MADO_ALARM_WAV", "", fname, sizeof(fname), path);
	m_eq_alarm_wav[0] = fname;
	GetPrivateProfileString(eq_section, "MASU_ALARM_WAV", "", fname, sizeof(fname), path);
	m_eq_alarm_wav[1] = fname;
	
	MakeEqualizerBmp();
}

void CContractGrid::MakeEqualizerBmp()
{
	int	yy = 0, sy = 0;
	double	rate = 0;
	const	CRect	rect(0, 0, max(m_rcData.Width() / 12, 3), m_rcData.Height());
	const	int	ww = rect.Width();
	const	int	hh = rect.Height();
	CDC*	pDC = GetDC();
	CPen	Pen, * pPen = nullptr;

	Pen.CreatePen(PS_SOLID, 1, RGB(0xBF, 0xBF, 0xBF));
	for (int ii = 0; ii < 3; ii++)
	{
		CDC	memDC;
		CBitmap* pBmp = nullptr;

		eq_bmp[ii].DeleteObject();
		eq_bmp[ii].CreateCompatibleBitmap(pDC, ww, hh);
		memDC.CreateCompatibleDC(pDC);

		pBmp = memDC.SelectObject(&eq_bmp[ii]);
		pPen = memDC.SelectObject(&Pen);

		memDC.FillSolidRect(0, 0, ww, hh, RGB(0xFF,0xFF,0xFF));
		memDC.MoveTo(ww-2, 0);
		memDC.LineTo(ww-2, hh);

		yy = hh;
		rate = hh/100.0f;
		for (int jj = 0; jj < m_eq_cnt; jj++)
		{
			sy = (int)(rate * m_eq_vol[jj]);
			switch (ii)
			{
			case 0: memDC.FillSolidRect(0, yy-sy, ww-2, sy, m_eq_mdcol[jj] ); break;
			case 1: memDC.FillSolidRect(0, yy-sy, ww-2, sy, m_eq_mscol[jj] ); break;
			default:break;
			}
			yy -= sy;
		}
		for (int jj = hh-6; jj > 0; jj -= 5) 
		{
			memDC.MoveTo(0, jj);
			memDC.LineTo(ww, jj);
		}
		memDC.SelectObject(pPen);
		memDC.SelectObject(pBmp);
		memDC.DeleteDC();
	}
	Pen.DeleteObject();
	ReleaseDC(pDC);
}

void CContractGrid::DrawEqualizer(CDC *pDC, const CRect& rc)
{
	static CString prev_prc = "";

	if (m_match.IsEmpty())
		return;

	double	val = 0;
	CString prc, amt;
	std::shared_ptr<CMatch> pMatch = m_match.GetHead();

	prc = pMatch->price;
	amt = pMatch->contract;
	if (prc==prev_prc) return;
	amt.Remove(',');
	prc.Remove(',');
	if (m_eq_type==1)
		val = atof(amt);
	else if (m_eq_type==2)
		val = atof(prc)*atof(amt);
	else	return;

	CDC	memDC;
	CBitmap *pBmp = nullptr;
	int	hh = 0;

	memDC.CreateCompatibleDC(pDC);
	for (int ii = 0; ii < m_eq_cnt; ii++)
	{
		if (val >= m_eq_lev[ii])
		{
			hh += rc.Height() * (int)(m_eq_vol[ii]/100.f);
		}
		else if (ii == 0)
		{
			if (val <= m_eq_lev[ii] && val > 0)
			{
				hh += (int)(m_eq_vol[ii] * (val/m_eq_lev[ii]));
			}
		}
		else if (ii > 0)
		{
			if (val <= m_eq_lev[ii] && val > m_eq_lev[ii-1])
			{
				hh += (int)(m_eq_vol[ii] * (val-m_eq_lev[ii-1])/(m_eq_lev[ii]-m_eq_lev[ii-1]));
			}
		}
	}

	// level meter 그리기
	if (pMatch->cc == clr_UpIndex)
		pBmp = memDC.SelectObject(&eq_bmp[1]);
	else
		pBmp = memDC.SelectObject(&eq_bmp[0]);
	pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBmp);

	// Background 그리기
	pBmp = memDC.SelectObject(&eq_bmp[2]);
	pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height()-hh, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pBmp);

	memDC.DeleteDC();

	if (pMatch->cc == clr_DnIndex && m_eq_alarm[0] != 0 && !m_eq_alarm_wav[0].IsEmpty())
	{
		if (m_eq_lev[m_eq_alarm_lev[0]] <= val)
			PlaySound((LPCSTR)m_eq_alarm_wav[0], NULL, SND_ASYNC);
	}
	else if (pMatch->cc == clr_UpIndex && m_eq_alarm[0] != 0 && !m_eq_alarm_wav[1].IsEmpty())
	{
		if (m_eq_lev[m_eq_alarm_lev[1]]<=val)
			PlaySound((LPCSTR)m_eq_alarm_wav[1], NULL, SND_ASYNC);
	}
}

void CContractGrid::OnRButtonDown(UINT nFlags, CPoint point) 
{	
	CWnd::OnRButtonDown(nFlags, point);
}
