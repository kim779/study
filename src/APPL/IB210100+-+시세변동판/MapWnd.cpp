// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB210100.h"
#include "MapWnd.h"
#include "misc.h"
#include "setup.h"

#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include "../../control/fx_misc/misctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	F_POINT		9
#define	S_TITLE		"시세변동판"
#define	S_SETUP		"대상조건설정"
#define	S_STOP		"중지"
#define	S_FONT		"굴림체"
#define	S_FILE		"u_config.ini"

#define	flagRTS		"w  "
#define	keyRTS		"quote"

#define	TM_ATTACH	1233
#define MAP_CONFIG	"IB200100"
#define MK_OPENSCR		13	// 화면 열기
/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd* parent)
{
	m_parent = parent;

	m_panel   = NULL;
	m_panel2  = NULL;

	m_setupBN = NULL;
	m_stopBN  = NULL;
	m_grid    = NULL;

	m_on     = false;
	m_stop   = false;

	m_bFont		= FALSE;
	m_nFontSize = 9;
	m_bCreated	= FALSE;
	
	m_row    = 0;
	m_rows   = 20;
	m_width  = 0;

	m_config = std::make_unique<Cconfig>();

	for (int i = 0; i < CNT_TREE; i++)
	{
		m_config->m_tree[i] = "99";
	}
}

CMapWnd::~CMapWnd()
{
	if (m_panel)	m_panel = nullptr;
	if (m_panel2)	m_panel2 = nullptr;
	if (m_setupBN)	m_setupBN = nullptr;
	if (m_stopBN)	m_stopBN = nullptr;
	if (m_grid)		m_grid = nullptr;

	m_config = nullptr;
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

BOOL CMapWnd::Create()
{
	m_root = (char *)variant(homeCC);
	m_user = (char *)variant(nameCC);
	m_strPal = GetPalette();

	m_font = getFont(F_POINT, S_FONT);
	variant(titleCC, S_TITLE);
	m_clrBack  = getColor(clBack);
	m_clrData  = getColor(clData);
	m_clrFocus = getColor(clFocus);
	loadConfig();

	return create(m_parent);
}

BOOL CMapWnd::OnEraseBkgnd(CDC* pDC) 
{
	return CWnd::OnEraseBkgnd(pDC);
}

void CMapWnd::OnDestroy()
{
	sendTR(true);
	CWnd::OnDestroy();
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (m_grid && IsWindow(m_grid->GetSafeHwnd()))
	{
		float	rows=0;

		CRect cRc;
		m_panel->GetWindowRect(cRc);

		cy -= (cRc.Height() + 3);		// top + bottom margin
		rows = (float)cy;
		// (float)hROW;
		m_rows = (int)rows;
		if (rows - (float)m_rows > 0.5)
			m_rows++;
	}

	const int nSize = m_nFontSize - 9;	//font size 9를 100%로 지정하여 8~12까지 표현

	CRect rcMap;
	GetClientRect(rcMap);

	if(m_bCreated)
	{
		CRect rcTop;
		rcTop.SetRect(GAP_SIDE, GAP_TOP, rcMap.right - GAP_SIDE, GAP_TOP + PANAL_HEIGHT + nSize);

		CRect rcPanel;
		rcPanel.SetRect(rcMap.left, rcMap.top, rcMap.right, PANAL_HEIGHT + GAP_TOP + GAP_BLOCK  + nSize);
		m_panel->MoveWindow(rcPanel, TRUE);

		CRect rcPanel2;
		rcPanel2.SetRect(rcMap.left, rcPanel.bottom, rcMap.right, rcMap.bottom);
		m_panel2->MoveWindow(rcPanel2, TRUE);

		CRect btRc;
		btRc.SetRect(rcTop.right - STOPBT_WIDTH - 6, rcTop.top + 3, rcTop.right - 6, rcTop.top + BUTTON_HEIGHT + 3);
		btRc.left	-= INCREASE_WIDTH * nSize;
		btRc.bottom	+= nSize;
		m_stopBN->MoveWindow(btRc);
		m_stopBN->SetFont(m_font);

		btRc = CRect(btRc.left - SETUPBT_WIDTH - 1, btRc.top, btRc.left - 1, btRc.bottom);
		btRc.left	-= INCREASE_WIDTH * nSize;
		m_setupBN->MoveWindow(btRc);
		m_setupBN->SetFont(m_font);

		CRect gridRc;
		gridRc = CRect(rcTop.left + GAP_PANEL, rcTop.bottom + GAP_BLOCK + GAP_PANEL, rcTop.right - GAP_PANEL, rcMap.bottom - GAP_BOTTOM - GAP_PANEL);
		m_grid->MoveWindow(gridRc);

		if(m_bFont && (m_grid != NULL))
		{
			LOGFONT* lf{};
			GVITEM	gvitem;
			for (int i = 0; i < m_grid->GetRowCount(); i++)
			{
				for (int j = 0; j < m_grid->GetColumnCount(); j++)
				{
					lf = m_grid->GetItemFont(i, j);
					lf->lfHeight = m_nFontSize * 10;

					CopyMemory(&gvitem.font, lf, sizeof(LOGFONT));
					m_grid->SetItemFont(i, j, lf);

					if (j != 1)
						m_grid->SetColumnWidth(j, infos[j].width + INCREASE_WIDTH * nSize);
				}

				m_grid->SetRowHeight(i, hROW + INCREASE_HEIGHT * nSize);
			}
			m_bFont = FALSE;
		}
	}
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	if (nIDEvent == TM_ATTACH)
	{
//		CRect	rect;
//
//		GetClientRect(&rect);
//		m_parent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(0, rect.Height()));
		
		sendTR();
		setColumns();
	}

	CWnd::OnTimer(nIDEvent);
}

BOOL CMapWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD(wParam) == BN_CLICKED)
	{
		switch (LOWORD(wParam))
		{
		case IDC_SETUP:
			setup();
			break;

		case IDC_STOP:
			m_stop = !m_stop;
			sendTR(m_stop);
			break;
		}
	}

	return CWnd::OnCommand(wParam, lParam);
}

BOOL CMapWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NM_GVITEM* lpNMH = (NM_GVITEM *)lParam;
	if (lpNMH->hdr.hwndFrom == m_grid->GetSafeHwnd())
	{
		CPoint	pt;
		CString	text;
		CString param;

		switch (lpNMH->hdr.code)
		{
		case GVNM_LMOUSEDOWN:
			if (lpNMH->row > 0 && (lpNMH->col == iCODE || lpNMH->col == iNAME))
			{
				text = m_grid->GetItemText(lpNMH->row, iCODE);
				if (!text.IsEmpty())
				{
					CString	string;
					
					
					string.Format("1301\t%s", text);
				
					//선택된 표시
					m_grid->SetSelectRow(lpNMH->row);
					m_parent->SendMessage(WM_USER, MAKEWPARAM(dragDLL, string.GetLength()),
								(LPARAM)(char *)string.operator LPCTSTR());
				}
			}
			break;

		case GVNM_RMOUSEDOWN:
			GetCursorPos(&pt);
			text = m_grid->GetItemText(lpNMH->row, iCODE);
			if (text.IsEmpty())
				m_string.Format("IB210100\n%d\t%d\n", pt.x, pt.y);
			else
				m_string.Format("IB210100\n%d\t%d\n1301\t%s\n", pt.x, pt.y, text);
			m_parent->PostMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (long)m_string.operator LPCTSTR());
			break;

		case GVNM_DBLCLICKED:
			if (!m_grid->GetItemText(lpNMH->row, iCODE).IsEmpty())
			{
				CString string;
				text.Format("1301\t%s", m_grid->GetItemText(lpNMH->row, iCODE));
				variant(triggerCC, text);

				//창 띄우기
				m_grid->SetFocus();
				param.Format("%s /p5 /S/d1301\t%s", MAP_CONFIG, m_grid->GetItemText(lpNMH->row, iCODE));
				m_parent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, 0x01), (LPARAM)(LPCTSTR)param);
			}
			break;
		}
	}

	return CWnd::OnNotify(wParam, lParam, pResult);
}


long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		OutputDebugString("RCV DLL_OUB\n");
		m_sync.Lock();
		OutputDebugString("LOCK DLL_OUB\n");
		dispatch(HIBYTE(LOWORD(wParam)), (char *)lParam, HIWORD(wParam));
		m_sync.Unlock();
		break;

	case DLL_NOTICE:
		{
		CString s;
		s.Format("[%s]\n",(char*)lParam);
		OutputDebugString(s);
		m_sync.Lock();
		OutputDebugString("LOCK NOTICE\n");
		alert((char *)lParam);
		m_sync.Unlock();
		}
		break;

	case DLL_SETPAL:
		m_clrBack = getColor(clBack);
		m_strPal = GetPalette();
		SetBitmap();

		if (m_panel)
		{
			int nLineTick[4];
			nLineTick[0] = GAP_SIDE;	nLineTick[1] = GAP_TOP;
			nLineTick[2] = GAP_SIDE;	nLineTick[3] = GAP_BLOCK;
			m_panel->SetCornerRound(m_hBmpRoundPanel, 5, &nLineTick[0]);
			m_panel->SetBkColor(getColor(clPanel), false);
			m_panel->SetBk2Color(getColor(clPanel));
			m_panel->SetBorderColor(getColor(clBack));

			nLineTick[0] = GAP_SIDE;	nLineTick[1] = 0;
			nLineTick[2] = GAP_SIDE;	nLineTick[3] = GAP_BOTTOM;
			m_panel2->SetCornerRound(m_hBmpRoundContents, 5, &nLineTick[0]);
			m_panel2->SetBkColor(getColor(clContents), false);
			m_panel2->SetBorderColor(getColor(clBack));
		}

		if (m_grid)
		{
			for (int ii = 0; ii < nCOL; ii++)
			{
				m_grid->SetItemBkColor(0, ii, getColor(clHead), true);
				m_grid->SetItemFgColor(0, ii, getColor(clHText), true);
			}
			m_grid->SetGridColor(getColor(clLine));
		}
		Invalidate();
		break;

	case DLL_SETFONTx:
		m_nFontSize = (int)HIWORD(wParam);
		m_font = getFont(m_nFontSize, S_FONT);
		m_bFont = TRUE;
		SetResize();
		setColumns();
		break;
	}
	return 0;
}

BOOL CMapWnd::create(CWnd* parent)
{
	if (!CWnd::Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, WINDOW_RECT, parent, 100))
		return FALSE;

	LOGFONT	lf{}, blf{};
	GVITEM	item;
	CRect rcMap;
	GetClientRect(rcMap);

//메뉴의 컨트롤 영역
	CRect rcTop;
	rcTop.SetRect(GAP_SIDE, GAP_TOP, rcMap.right - GAP_SIDE, GAP_TOP + PANAL_HEIGHT);

	CRect rcBt;
	rcBt.SetRect(rcTop.right - STOPBT_WIDTH - 6, rcTop.top + 3, rcTop.right - 6, rcTop.top + BUTTON_HEIGHT + 3);
	m_stopBN = std::make_unique<CfxImgButton>();
	m_stopBN->Create(S_STOP, rcBt, this, IDC_STOP, FALSE, TRUE);
	m_stopBN->SetImgBitmap(getBitmap("2BTN.bmp"), getBitmap("2BTN_DN.bmp"), getBitmap("2BTN_EN.bmp"));
	m_stopBN->SetTextColor(getColor(clBtnText));
	m_stopBN->SetFont(m_font, TRUE);

	rcBt = CRect(rcBt.left - SETUPBT_WIDTH - 1, rcBt.top, rcBt.left - 1, rcBt.bottom);
	m_setupBN = std::make_unique<CfxImgButton>();
	m_setupBN->Create(S_SETUP, rcBt, this, IDC_SETUP, FALSE, TRUE);
	m_setupBN->SetImgBitmap(getBitmap("6BTN.bmp"), getBitmap("6BTN_DN.bmp"), getBitmap("6BTN_EN.bmp"));
	m_setupBN->SetTextColor(getColor(clBtnText));
	m_setupBN->SetFont(m_font, TRUE);

	CRect rcGrid;
	rcGrid.SetRect(rcTop.left + GAP_PANEL, rcTop.bottom + GAP_BLOCK + GAP_PANEL, rcTop.right - GAP_PANEL, rcMap.bottom - GAP_BOTTOM - GAP_PANEL);
	m_grid = std::make_unique<CfxGrid>();
	m_grid->Create(rcGrid, this, IDC_GRID, GVSC_NONE, GVDD_NONE, WS_CHILD | WS_VISIBLE | WS_VSCROLL);
	m_grid->SetRowColResize(FALSE, FALSE);
	m_grid->Initial(19, nCOL, 1, 0);
	m_grid->SetUpColor(getColor(clUp));
	m_grid->SetDnColor(getColor(clDown));
	m_grid->SetBKSelColor(getColor(clSel));

	lf.lfHeight         = F_POINT * 10;
	lf.lfWidth          = 0;
	lf.lfEscapement     = 0;
	lf.lfOrientation    = 0;
	lf.lfWeight         = FW_NORMAL;
	lf.lfItalic         = false;
	lf.lfUnderline      = false;
	lf.lfStrikeOut      = false;
	lf.lfCharSet        = DEFAULT_CHARSET;
	lf.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	lf.lfQuality        = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
	strcpy(lf.lfFaceName, S_FONT);

	for (int ii = 0; ii < nCOL; ii++)
	{
		item.col    = ii;
		item.mask   = GVMK_FORMAT|GVMK_ATTR|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
		item.format = infos[ii].format;
		item.attr   = infos[ii].attr;
		item.fgcol  = getColor(clText);
		item.bkcol  = m_clrData;
		CopyMemory(&item.font, &lf, sizeof(LOGFONT));
		m_grid->SetColumnItems(ii, &item);
		m_grid->SetColumnWidth(ii, infos[ii].width);

		item.row    = 0;
		item.mask   = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_ATTR;
		item.format = GVFM_HEADER;
		item.attr   = GVAT_ONLYROW;
		if (infos[ii].attr & GVAT_HIDDEN)
			item.attr |= GVAT_HIDDEN;
		item.text   = infos[ii].heads;
		item.fgcol  = getColor(clHText);
		item.bkcol  = getColor(clHead);
		m_grid->SetItem(&item);

		m_width += infos[ii].width;
	}

	CopyMemory(&blf, &lf, sizeof(LOGFONT));
	blf.lfWeight = FW_SEMIBOLD;
	for (int ii = 0; ii < nROW; ii++)
	{
		m_grid->SetRowHeight(ii, hROW);
		if (ii > 0)
			m_grid->SetItemFont(ii, iNAME, &blf);	// 종목명 : bold font
	}

	m_grid->SetStepColor(1, getColor(clRow1), getColor(clRow2));
	m_grid->SetGridColor(getColor(getColor(clLine)));
	m_grid->SetGridLine(GVLN_VERT);
	m_grid->SetGridOutline(FALSE);
	m_grid->SetGridFocusLine(FALSE, TRUE);
	m_grid->SetGridDIScroll(true);
	m_grid->Adjust();
	
	SetBitmap();
	
	CRect rcPanel, rcPanel2;
	rcPanel.SetRect(rcMap.left, rcMap.top, rcMap.right, PANAL_HEIGHT + GAP_TOP + GAP_BLOCK);
	m_panel = std::make_unique<CfxStatic>();
	m_panel->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, rcPanel, this, IDC_PANEL);
	m_panel->Init(ssGRADIENT, 0, _T(""), _T(""), getColor(clPanel), getColor(clPanel), getColor(clBtnText), 0);

	int nLineTick[4];
	nLineTick[0] = GAP_SIDE;	nLineTick[1] = GAP_TOP;
	nLineTick[2] = GAP_SIDE;	nLineTick[3] = GAP_BLOCK;

	m_panel->SetCornerRound(m_hBmpRoundPanel, 5, &nLineTick[0]);
	m_panel->SetBorderColor(getColor(clBack));

	rcPanel2.SetRect(rcMap.left, rcPanel.bottom, rcMap.right, rcMap.bottom);
	m_panel2 = std::make_unique<CfxStatic>();
	m_panel2->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, rcPanel2, this, IDC_PANEL2);
	m_panel2->SetBkColor(getColor(clContents));

	nLineTick[0] = GAP_SIDE;	nLineTick[1] = 0;
	nLineTick[2] = GAP_SIDE;	nLineTick[3] = GAP_BOTTOM;

	m_panel2->SetCornerRound(m_hBmpRoundContents, 5, &nLineTick[0]);
	m_panel2->SetBorderColor(getColor(clBack));

	SetTimer(TM_ATTACH, 50, NULL);

	m_bCreated = TRUE;
	return TRUE;
}

void CMapWnd::Setinsertrow()
{
	LOGFONT	lf, blf;
	GVITEM	item;

	m_grid->InsertRow(" ", 1);
	m_grid->DeleteRow(m_rows);

	lf.lfHeight = F_POINT * 10;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy(lf.lfFaceName, S_FONT);

	for (int ii = 0; ii < nCOL; ii++)
	{
		item.col = ii;
		item.mask = GVMK_FORMAT | GVMK_ATTR | GVMK_FONT | GVMK_FGCOLOR | GVMK_BKCOLOR;
		item.format = infos[ii].format;
		item.attr = infos[ii].attr;
		item.fgcol = getColor(clText);
		item.bkcol = m_clrData;
		CopyMemory(&item.font, &lf, sizeof(LOGFONT));
		m_grid->SetColumnItems(ii, &item);
		m_grid->SetColumnWidth(ii, infos[ii].width);

		item.row = 0;
		item.mask = GVMK_TEXT | GVMK_FORMAT | GVMK_FONT | GVMK_FGCOLOR | GVMK_BKCOLOR | GVMK_ATTR;
		item.format = GVFM_HEADER;
		item.attr = GVAT_ONLYROW;
		if (infos[ii].attr & GVAT_HIDDEN)
			item.attr |= GVAT_HIDDEN;
		item.text = infos[ii].heads;
		item.fgcol = getColor(clHText);
		item.bkcol = getColor(clHead);
		m_grid->SetItem(&item);

		m_width += infos[ii].width;
	}

	CopyMemory(&blf, &lf, sizeof(LOGFONT));
	blf.lfWeight = FW_SEMIBOLD;
	for (int ii = 0; ii < m_rows; ii++)
	{
		m_grid->SetRowHeight(ii, hROW);
		if (ii > 0)
			m_grid->SetItemFont(ii, iNAME, &blf);	// 종목명 : bold font
	}
}

void CMapWnd::SetBitmap()
{
	CStringArray strImageArr;
	CStringArray strImageArr2;
	CString sImageDir;
	//panel
	strImageArr.Add("axpanel" + m_strPal + "1_lt.bmp");
	strImageArr.Add("axpanel" + m_strPal + "1_rt.bmp");
	strImageArr.Add("axpanel" + m_strPal + "1_lb.bmp");
	strImageArr.Add("axpanel" + m_strPal + "1_rb.bmp");

	//contents
	strImageArr2.Add("axpanel" + m_strPal + "0_lt.bmp");
	strImageArr2.Add("axpanel" + m_strPal + "0_rt.bmp");
	strImageArr2.Add("axpanel" + m_strPal + "0_lb.bmp");
	strImageArr2.Add("axpanel" + m_strPal + "0_rb.bmp");

	for(int i = 0; i < 4; i++)
	{	
		m_hBmpRoundContents[i] = getBitmap(strImageArr.GetAt(i));
		m_hBmpRoundPanel[i] = getBitmap(strImageArr2.GetAt(i));
	}
}

CString CMapWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_root, "tab", "palette.ini");

	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;

	return strPal;
}

void CMapWnd::sendTR(bool clear)
{

	std::unique_ptr<char[]> wb = std::make_unique<char[]>(L_userTH + L_inR);
	struct	_userTH* userth{}; 
	struct	_inR* inR{};
	CString	tmps;

	FillMemory(wb.get(), sizeof(wb.get()), ' ');
	userth = (struct _userTH *)wb.get();
	CopyMemory(userth->trc, trN, sizeof(userth->trc));
	userth->key  = 0;
	userth->stat = 0;

	inR = (struct _inR *)&wb[L_userTH];
	if (clear)
		inR->func[0] = 'C';
	else
	{
		m_on = false;
		inR->func[0] = 'S';
		tmps.Format("%03d", m_config->m_vol);
		CopyMemory(inR->cvol, tmps.operator LPCTSTR(), sizeof(inR->cvol));
		tmps.Format("%03d", m_config->m_amt);
		CopyMemory(inR->camt, tmps.operator LPCTSTR(), sizeof(inR->camt));
		inR->ador[0] = m_config->m_and     ? 'a' : 'o';
		inR->prcf[0] = m_config->m_price   ? '1' : '0';
		tmps.Format("%07d", m_config->m_sprc);
		CopyMemory(inR->sprc, tmps.operator LPCTSTR(), sizeof(inR->sprc));
		tmps.Format("%07d", m_config->m_eprc);
		CopyMemory(inR->eprc, tmps.operator LPCTSTR(), sizeof(inR->eprc));
		inR->updn[0] = m_config->m_ulimit  ? '1' : '0';
		inR->updn[1] = m_config->m_up      ? '1' : '0';
		inR->updn[2] = m_config->m_flat    ? '1' : '0';
		inR->updn[3] = m_config->m_dlimit  ? '1' : '0';
		inR->updn[4] = m_config->m_down    ? '1' : '0';
		tmps.Format("%02d", m_config->m_tree);
	//	CopyMemory(inR->tree, tmps.operator LPCTSTR(), sizeof(inR->tree));
		for (int ii = 0; ii < CNT_TREE; ii++)
		{
			CopyMemory(inR->tree[ii], m_config->m_tree[ii], 2);
		}
	}

	m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, L_inR), (LPARAM)(LPCTSTR)wb.get());

	wb = nullptr;
}

void CMapWnd::dispatch(int key, char* datB, int datL)
{
	//CString s;
	//s.Format("2101 OUB : [%s]\n",CString(datB,datL));
	//OutputDebugString(s);

	struct	_outR* outR{};

	if (datL < L_outR)
		return;

	outR = (struct _outR *)datB;
	m_on = (outR->retc[0] == '0') ? true : false;
}

void CMapWnd::alert(CString data)
{
	CString s;
	s.Format("NOTICE DATA : [%s]\n",data);
	OutputDebugString(s);

	if (!m_on || m_stop)
		return;

	int	pos{};
	CString	text, str;
	struct	_rtsR* rtsR=nullptr;

	while (true)
	{
		if (data.GetLength() < L_rtsR)
			break;

		rtsR = (struct _rtsR *)(char *)data.operator LPCTSTR();
		str  = CString(rtsR->flag, sizeof(rtsR->flag));
		if (str.CompareNoCase(flagRTS))
			return;

		pos  = atoi(CString(rtsR->datl, sizeof(rtsR->datl)));
		text = data.Mid(L_rtsR, pos);
		if ((int)(L_rtsR + pos) < data.GetLength())
			data = data.Mid(L_rtsR + pos);
		else
			data.Empty();

		pos = text.Find('\t');
		if (pos == -1)
			continue;

		str = text.Left(pos++);
		if (str.CompareNoCase(keyRTS))
			continue;

		if (m_row >= 1)
		{
			for (int ii = 0; ii < nCOL; ii++)
				m_grid->SetItemBkColor(m_row, ii, m_clrData, true);
		}

		Setinsertrow();
		//m_row ++;
		m_row = 1;
		if (m_row >= m_rows)
			m_row = 1;
		text = text.Mid(pos);
		for (int ii = 0; ii < nCOL; ii++)
		{
			pos = text.Find('\t');
			if (pos == -1)
			{
				str = text;
				text.Empty();
			}
			else
			{
				str  = text.Left(pos++);
				text = text.Mid(pos);
			}

			if (ii == 7 && !m_config->m_color)	// 순간체결량 && 순간체결색상표시
			{
				str.Remove('+');
				str.Remove('-');
			}
			m_grid->SetItemText(m_row, ii, str);
			m_grid->SetItemBkColor(m_row, ii, m_clrFocus, true);
		}
	}
}

void CMapWnd::setup()
{
	std::unique_ptr<Csetup>	setup = std::make_unique<Csetup>(nullptr, m_config.get());

	if (setup->DoModal() == IDOK)
	{
		setup->GetConfig(m_config.get());
		sendTR();
		saveConfig();
		setColumns();
	}
	
	setup = nullptr;
}

void CMapWnd::setColumns()
{
	CRect	rcMap, rc;
	DWORD	attr=0;
	int	width = 0;

	const int nSize = m_nFontSize - 9;	//font size 9를 100%로 지정하여 8~12까지 표현
	const int nIncreaseWidth = INCREASE_WIDTH * nSize;

	for (int ii = 0; ii < nCOL; ii++)
	{
		attr = GVAT_ONLYROW;
		switch (ii)
		{
		case iCODE:	// 코드
			if (!(m_config->m_field & F_CODE))
			{
				attr |= GVAT_HIDDEN;
				m_grid->SetItemAttr(0, ii, attr);
				continue;
			}
			m_grid->SetItemAttr(0, ii, attr);
			m_grid->SetColumnWidth(ii, infos[ii].width + nIncreaseWidth);
			break;
		case iRATE:	// 등락률
			if (!(m_config->m_field & F_RATE))
			{
				attr |= GVAT_HIDDEN;
				m_grid->SetItemAttr(0, ii, attr);
				continue;
			}
			m_grid->SetItemAttr(0, ii, attr);
			m_grid->SetColumnWidth(ii, infos[ii].width + nIncreaseWidth);
			break;
		case iQTY:	// 거래량
			if (!(m_config->m_field & F_VOL))
			{
				attr |= GVAT_HIDDEN;
				m_grid->SetItemAttr(0, ii, attr);
				continue;
			}
			m_grid->SetItemAttr(0, ii, attr);
			m_grid->SetColumnWidth(ii, infos[ii].width + nIncreaseWidth);
			break;
		case iCAMT:	// 순간체결금액
			if (!(m_config->m_field & F_AMT))
			{
				attr |= GVAT_HIDDEN;
				m_grid->SetItemAttr(0, ii, attr);
				continue;
			}
			m_grid->SetItemAttr(0, ii, attr);
			m_grid->SetColumnWidth(ii, infos[ii].width + nIncreaseWidth);
		default:
			break;
		}

		if(m_grid->GetColumnWidth(ii) != 0)
			width += infos[ii].width + nIncreaseWidth;
	}

	m_width = width + 2 * (GAP_SIDE + GAP_PANEL) + 18;

	GetClientRect(rcMap);

	CRect rcPanel;
	rcPanel.SetRect(rcMap.left, rcMap.top, rcMap.right, PANAL_HEIGHT + GAP_TOP + GAP_BLOCK  + nSize);
	m_panel->MoveWindow(rcPanel, TRUE);

	CRect rcPanel2;
	rcPanel2.SetRect(rcMap.left, rcPanel.bottom, rcMap.right, rcMap.bottom);
	m_panel2->MoveWindow(rcPanel2, TRUE);

	CRect topRc;
	topRc.SetRect(GAP_SIDE, GAP_TOP, m_width - GAP_SIDE, GAP_TOP + PANAL_HEIGHT + nSize);
//	m_panel->MoveWindow(topRc);
	
	CRect gridRc;
	gridRc.SetRect(topRc.left + GAP_PANEL, topRc.bottom + GAP_BLOCK + GAP_PANEL, topRc.right - GAP_PANEL, rcMap.bottom - GAP_BOTTOM - GAP_PANEL);
	m_grid->MoveWindow(gridRc);
	
	width = (m_width) - rcMap.Width();

	m_setupBN->GetWindowRect(rc);
	ScreenToClient(rc);
	rc.OffsetRect(width, 0);
	m_setupBN->MoveWindow(rc);

	m_stopBN->GetWindowRect(rc);
	ScreenToClient(rc);
	rc.OffsetRect(width, 0);
	m_stopBN->MoveWindow(rc);
	
	m_parent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(m_width, rcMap.Height()));
}

void CMapWnd::loadConfig()
{
	char	wb[32];
	CString	path, tmps;

	path.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, S_FILE);

	// 체결량 범위   (vol * 1,000)
	GetPrivateProfileString(_T("2101"), "vol", "3", wb, sizeof(wb), path);
	m_config->m_vol = max(1, atoi(wb));

	// 체결금액 범위 (amt * 10,000,000)
	GetPrivateProfileString(_T("2101"), "amt", "3", wb, sizeof(wb), path);
	m_config->m_amt = max(1, atoi(wb));

	// and / or
	GetPrivateProfileString(_T("2101"), "and", "0", wb, sizeof(wb), path);
	m_config->m_and = atoi(wb) ? true : false;

	// 가격범위 설정
	GetPrivateProfileString(_T("2101"), "pricef", "0", wb, sizeof(wb), path);
	m_config->m_price = atoi(wb) ? true : false;

	// 가격범위 시작값
	GetPrivateProfileString(_T("2101"), "sprice", "0", wb, sizeof(wb), path);
	m_config->m_sprc = atoi(wb);

	// 가격범위 끝값
	GetPrivateProfileString(_T("2101"), "eprice", "0", wb, sizeof(wb), path);
	m_config->m_eprc = atoi(wb);

	// 상한
	GetPrivateProfileString(_T("2101"), "updn", "11111", wb, sizeof(wb), path);
	tmps = wb;
	int ii = 0;
	for (ii = 0; ii < tmps.GetLength(); ii++)
	{
		switch (ii)
		{
		case 0:	m_config->m_ulimit = (tmps.GetAt(ii) == '1') ? true : false;
		case 1:	m_config->m_up     = (tmps.GetAt(ii) == '1') ? true : false;
		case 2:	m_config->m_flat   = (tmps.GetAt(ii) == '1') ? true : false;
		case 3:	m_config->m_dlimit = (tmps.GetAt(ii) == '1') ? true : false;
		case 4:	m_config->m_down   = (tmps.GetAt(ii) == '1') ? true : false;
		}
	}
	for ( ; ii < 5; ii++)
	{
		switch (ii)
		{
		case 0:	m_config->m_ulimit = true;
		case 1:	m_config->m_up     = true;
		case 2:	m_config->m_flat   = true;
		case 3:	m_config->m_dlimit = true;
		case 4:	m_config->m_down   = true;
		}
	}

	// 색상표시
	GetPrivateProfileString(_T("2101"), "color", "1", wb, sizeof(wb), path);
	m_config->m_color = atoi(wb) ? true : false;

	// 필드표시
	GetPrivateProfileString(_T("2101"), "field", "15", wb, sizeof(wb), path);
	m_config->m_field = atoi(wb);

	// 대상업종 지정(시장구분+업종코드)
	GetPrivateProfileString(_T("2101"), "ucode", "100000", wb, sizeof(wb), path);
//	m_config->m_tree = atoi(wb);
	//숫자로 변환하여 0~10 미만이면 그냥 저장
	//10 이상이면, 문자를 2개씩 쪼개서 숫자로 변환해서 저장
	//추후 체크박스가 늘어나면 변경 필요
	int tmp = atoi(wb);
	CString strTmp, parseWord;
	ii = 0;

	if (tmp < CNT_CHECK)
	{
		strTmp.Format("%02d", tmp);
		m_config->m_tree[0] = strTmp;
	}
	else
	{
		strTmp = (CString)wb;

		while (strTmp != "")
		{
			parseWord = strTmp.Left(2);
			strTmp = strTmp.Mid(2);

			if (atoi(parseWord) < 10)
			{
				m_config->m_tree[ii] = parseWord;
				ii++;
			}
		}
	}
}

void CMapWnd::saveConfig()
{
	CString	path, tmps;

	path.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, S_FILE);

	// 체결량 범위   (vol * 1,000)
	tmps.Format("%d", m_config->m_vol);
	WritePrivateProfileString(_T("2101"), "vol", tmps, path);

	// 체결금액 범위 (amt * 10,000,000)
	tmps.Format("%d", m_config->m_amt);
	WritePrivateProfileString(_T("2101"), "amt", tmps, path);

	// 가격범위 설정
	WritePrivateProfileString(_T("2101"), "pricef", m_config->m_price ? _T("1") : _T("0"), path);

	// 가격범위 시작값
	tmps.Format("%d", m_config->m_sprc);
	WritePrivateProfileString(_T("2101"), "sprice", tmps, path);

	// 가격범위 끝값
	tmps.Format("%d", m_config->m_eprc);
	WritePrivateProfileString(_T("2101"), "eprice", tmps, path);
	
	// and / or
	WritePrivateProfileString(_T("2101"), "and", m_config->m_and ? _T("1") : _T("0"), path);

	// 상한 / 상승 / 보합 / 하한 / 하락
	tmps.Empty();
	tmps += m_config->m_ulimit ? _T("1") : _T("0");
	tmps += m_config->m_up     ? _T("1") : _T("0");
	tmps += m_config->m_flat   ? _T("1") : _T("0");
	tmps += m_config->m_dlimit ? _T("1") : _T("0");
	tmps += m_config->m_down   ? _T("1") : _T("0");
	WritePrivateProfileString(_T("2101"), "updn", tmps, path);

	// 색상표시
	WritePrivateProfileString(_T("2101"), "color", m_config->m_color ? _T("1") : _T("0"), path);

	// 필드표시
	tmps.Format("%d", m_config->m_field);
	WritePrivateProfileString(_T("2101"), "field", tmps, path);

	// 대상업종 지정(시장구분+업종코드)
	//tmps.Format("%d", m_config->m_tree);
	CString tempStr(_T(""));
	for (int ii = 0; ii < CNT_TREE; ii++)
	{
		if (atoi(m_config->m_tree[ii]) < CNT_CHECK)
		{
			tempStr += m_config->m_tree[ii];
		}
	}

	WritePrivateProfileString(_T("2101"), "ucode", tempStr, path);
}

COLORREF CMapWnd::getColor(int color)
{
	if (color & 0x02000000)
		return color;

	return m_parent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CFont* CMapWnd::getFont(int point, CString name, int bold, bool italic)
{
	struct	_fontR	fontR;

	if (name.IsEmpty())
		name = S_FONT;
	fontR.name   = (char *)name.operator LPCTSTR();
	fontR.point  = point;
	fontR.italic = italic;
	fontR.bold   = bold;

	return (CFont *)m_parent->SendMessage(WM_USER, MAKEWPARAM(getFONT, 0), (LPARAM)&fontR);
}

HBITMAP CMapWnd::getBitmap(CString bmps)
{
	CString	path;

	path.Format("%s\\%s\\%s", m_root, IMAGEDIR, bmps);
	return ((CBitmap *)m_parent->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, 0), (LPARAM)(LPCTSTR)path))->operator HBITMAP();
}

long CMapWnd::variant(int key, CString data)
{
	return m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, key), (LPARAM)(LPCTSTR)data);
}


void CMapWnd::SetResize()
{
	const int nSize = m_nFontSize - 9;	//font size 9를 100%로 지정하여 8~12까지 표현

	CRect gridRc;
	GetClientRect(gridRc);	

	int nGridCol = 0;
	for(int i = 0; i < m_grid->GetColumnCount(); i++)
	{
		if(m_grid->GetColumnWidth(i) != 0)
			nGridCol++;
	}
	const int nIncreaseWidth = INCREASE_WIDTH * nSize * nGridCol;
	const int nIncreaseHeight = INCREASE_HEIGHT * nSize * 20 + nSize;

	CRect mapRc = WINDOW_RECT;
	mapRc.right += nIncreaseWidth;
	mapRc.bottom += nIncreaseHeight;
	m_parent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(mapRc.right, mapRc.bottom));
}


void CMapWnd::SaveBuffer(CString sKey, CString sData)
{
	CIB210100App* pApp = (CIB210100App*)AfxGetApp();
	pApp->SaveBuffer(sKey, sData);
}

CString CMapWnd::GetBuffer(CString sKey)
{
	CIB210100App* pApp = (CIB210100App*)AfxGetApp();
	return pApp->GetBuffer(sKey);
}




