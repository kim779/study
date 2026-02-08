// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB230100.h"
#include "MapWnd.h"
#include "misc.h"

#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include "../../control/fx_misc/misctype.h"

#include "../H/interMSG.h"

#include "ContainerWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	MAP_WIDTH	502
#define	MAP_HEIGHT	340

#define	F_POINT		9
#define	S_TITLE		"장중종목 거래원별 매매동향"
#define	S_STOP		"자동"
#define	S_INQ		"조회"
#define	S_FONT		"굴림체"

#define	TM_ATTACH	1233

#define GAP_TOP			3
#define GAP_SIDE		1
#define GAP_BOTTOM		1
#define GAP_BLOCK		5
#define GAP_PANEL		4

#define clContents 181
#define clGuide	26
#define	clBack	64
#define clBox	65
#define	clText	69
/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd* parent)
{
	m_parent = parent;

	m_panel    = NULL;
	m_left     = NULL;
	m_top	   = NULL;
	m_right    = NULL;
	m_bottom   = NULL;
	m_center   = NULL;

	m_code     = NULL;
	m_searchBN = NULL;
	m_stopBN   = NULL;
	m_inqBN    = NULL;

	m_tbl1     = NULL;
	m_tbl2     = NULL;
	m_label    = NULL;
	m_schg     = NULL;
	m_svol     = NULL;
	m_bvol     = NULL;
	m_bchg     = NULL;

	m_grid     = NULL;
	m_symbol   = NULL;
	m_inst     = NULL;

	m_keys.Empty();
	m_keyx.Empty();

	m_fsvol.Empty();
	m_fbvol.Empty();
	m_stop = false;

	m_rows    = 1;
	m_sort    = -1;
	m_descend = true;

	m_bFirst = true;
	m_strCode.Empty();
}

CMapWnd::~CMapWnd()
{
	if (m_panel)	m_panel = nullptr;
	if (m_left)		m_left = nullptr;
	if (m_top)		m_top = nullptr;
	if (m_right)	m_right = nullptr;
	if (m_bottom)	m_bottom = nullptr;
	if (m_center)	m_center = nullptr;
	if (m_code)		m_code = nullptr;
	if (m_searchBN)	m_searchBN = nullptr;
	if (m_stopBN)	m_stopBN = nullptr;
	if (m_inqBN)	m_inqBN = nullptr;
	if (m_tbl1)		m_tbl1 = nullptr;
	if (m_tbl2)		m_tbl2 = nullptr;
	if (m_label)	m_label = nullptr;
	if (m_schg)		m_schg = nullptr;
	if (m_svol)		m_svol = nullptr;
	if (m_bvol)		m_bvol = nullptr;
	if (m_bchg)		m_bchg = nullptr;
	if (m_grid)		m_grid = nullptr;

	if(m_contentspanel) m_contentspanel = nullptr;

	if (m_symbol)
	{
		if (IsWindow(m_symbol->GetSafeHwnd()))
			m_symbol->SendMessage(WM_CLOSE);
		m_symbol->Detach();
		delete m_symbol;
	}

	if (m_inst)
		AfxFreeLibrary(m_inst);

	if (m_pMarketpicker)
	{
		m_pMarketpicker->DestroyWindow();
		delete m_pMarketpicker;
	}
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 99, OnMsgMKDLL)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

BOOL CMapWnd::Create()
{
	m_root  = (char *)variant(homeCC);
	m_font  = getFont(F_POINT, S_FONT);
	m_fontb  = getFont(F_POINT, S_FONT, FW_BOLD);
	m_sfont = getFont(F_POINT, S_FONT, FW_SEMIBOLD);
	m_strPal = GetPalette();

	variant(titleCC, S_TITLE);
	m_clrBackIn = getColor(clBackIn);
	m_clrContentsBackIn = getColor(clcontentsBack);
	m_clrBack = getColor(clBack);
	m_parent->SendMessage(WM_USER, MAKEWPARAM(dropDLL, 0));

	m_rts.RemoveAll();
	for (int ii = 0; ii < L_syms; ii++)
	{
		if (strlen(syms[ii].rts))
			m_rts.SetAt(syms[ii].rts, (void *)MAKELONG(syms[ii].flag, MAKEWORD(syms[ii].row, syms[ii].col)));
	}

	return create(m_parent);
}

BOOL CMapWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	CString	text;

	switch (LOWORD(wParam))
	{
	case IDC_CODE:
		switch (HIWORD(wParam))
		{
		case BN_CLICKED:
			text = (char *)variant(historyCC, _T("1301"));
			m_code->SetHistory(text);
			break;
		case CBN_SELENDOK:
// 			OutputDebugString("[KSJ]CBN_SELENDOK");
			inquiry();
			break;
		}
		break;

	case IDC_SEARCH:
		if (HIWORD(wParam) == BN_CLICKED)
		{
			CRect	rect;
			int	pos;

			m_searchBN->GetWindowRect(&rect);
			text = (char *)m_parent->SendMessage(WM_USER, MAKEWPARAM(codeDLL, MAKEWORD(kospiCODE, 0)), MAKELPARAM(rect.left, rect.bottom));		
			pos  = text.Find('\t');
			if (pos != -1)
				text = text.Left(pos);
			if (!text.IsEmpty())
			{
				m_code->SetEditData(text);
				inquiry();
			}
		}
		break;

	case IDC_STOP:
		m_stop = !m_stop;
		m_stopBN->SetFont(m_stop ? m_font : m_sfont);
		break;

	case IDC_INQ:
		{
// 			CString strTemp;
// 			strTemp.Format("[KSJ] wParam[%d]", HIWORD(wParam));
// 			OutputDebugString(strTemp);

			inquiry(m_bFirst);
		}
		break;
	}

	return CWnd::OnCommand(wParam, lParam);
}

BOOL CMapWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	CPoint	pt;
	CString	text, sCode;
	NM_GVITEM* lpNMH = (NM_GVITEM *)lParam;

	text = "1301\t" + m_code->GetEditData();
	switch (lpNMH->hdr.code)
	{
	case GVNM_LMOUSEDOWN:
		if (lpNMH->hdr.hwndFrom == m_tbl1->GetSafeHwnd())
		{
			if (lpNMH->col == iSNAM)
			{
				if (lpNMH->row == 0)
					openView("IB230300", text);
				else
				{
					text += "\n";
					text += "ed_trcd\t";
					sCode.Format("%03s", m_tbl1->GetItemText(lpNMH->row, iSCOD));
					text += sCode;
					openView("IB230200", text);

// 					OutputDebugString("[KSJ] [" + text + "]");
				}
			}
		}
		else if (lpNMH->hdr.hwndFrom == m_tbl2->GetSafeHwnd())
		{
			if (lpNMH->col == iBNAM)
			{
				if (lpNMH->row == 0)
					openView("IB230300", text);
				else
				{
					text += "\n";
					text += "ed_trcd\t";
					sCode.Format("%03s", m_tbl2->GetItemText(lpNMH->row, iBCOD));
					text += sCode;
					openView("IB230200", text);

// 					OutputDebugString("[KSJ] [" + text + "]");
				}
			}
		}
		else if (lpNMH->hdr.hwndFrom == m_grid->GetSafeHwnd())
		{
			DWORD	attr=0;

			switch (lpNMH->col)
			{
			case iCODE:
			case iNAME:
				text += "\n";
				text += "ed_trcd\t";
				sCode.Format("%03s", m_grid->GetItemText(lpNMH->row, iCODE));
				text += sCode;
				openView("IB230200", text);

// 				OutputDebugString("[KSJ] [" + text + "]");
				break;
			case iBBVL:
			case iRATE:
				if (lpNMH->row == 0)
				{
					if (m_sort != lpNMH->col)
					{
						m_sort = lpNMH->col;
						m_descend = true;
					}
					else	m_descend = !m_descend;
					attr = m_grid->GetItemAttr(0, m_sort);
					m_grid->SetItemAttr(0, m_sort, attr|GVAT_SORTVAL);
					m_grid->Sort(m_sort, !m_descend);
					m_grid->Invalidate(FALSE);
					m_grid->SetItemAttr(0, m_sort, attr);
				}
				break;
			}
		}
		break;

	case GVNM_RMOUSEDOWN:
		GetCursorPos(&pt);
		if (lpNMH->row > 0)
		{
			text += "\n";
			text += "ED_TRAD\t";
			if (lpNMH->hdr.hwndFrom == m_tbl1->GetSafeHwnd())
				text += m_tbl1->GetItemText(lpNMH->row, iSCOD);
			else if (lpNMH->hdr.hwndFrom == m_tbl2->GetSafeHwnd())
				text += m_tbl2->GetItemText(lpNMH->row, iBCOD);
			else if (lpNMH->hdr.hwndFrom == m_grid->GetSafeHwnd())
				text += m_grid->GetItemText(lpNMH->row, iCODE);
		}
		m_string.Format("IB230100\n%d\t%d\n%s\n", pt.x, pt.y, text);
		m_parent->PostMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (long)m_string.operator LPCTSTR());
		break;
	}
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

// 	OutputDebugString("++++++++++++SIZE++++++++++++");
	if (m_grid && IsWindow(m_grid->GetSafeHwnd()))
	{
		CRect rc;
//		cy -= (157 + 3);		// top + bottom margin
		if(m_svol)
		{	
			m_bchg->GetWindowRect(&rc);
			ScreenToClient(&rc);
			const int itop = rc.bottom + 3;
			m_grid->MoveWindow(4, itop, rc.right-4, cy-itop - 7);
		}

		if(m_contentspanel)
		{
			m_contentspanel->GetWindowRect(&rc);
			ScreenToClient(&rc);

			m_contentspanel->MoveWindow(rc.left, rc.top, rc.right, cy - rc.top +3);
		}
	}
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	if (nIDEvent == TM_ATTACH)
	{
// 	 	KillTimer(nIDEvent);
		CRect	rect;

		GetClientRect(&rect);
		m_parent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(rect.Width(), rect.Height()));
	}
// 	else if (nIDEvent == 1)		//자동조회 테스트용
// 	{
// 		if(!m_stop)
// 			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_INQ, 0), BN_CLICKED);
// 	}
	
	CWnd::OnTimer(nIDEvent);
}

long CMapWnd::OnMsgMKDLL(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case 100: //내부 마켓컨트롤 클릭시 메시지
	{
		CString stmp;
		stmp.Format("%s", (char*)lParam);
		int ifnd = stmp.ReverseFind('\t');
		m_sMarket.Format("%s", stmp.Right(stmp.GetLength() - ifnd - 1));
		m_sMarket.TrimRight();
		sendTR();
	}
	break;
	}
	return 0;
}

long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		m_sync.Lock();
		dispatch(HIBYTE(LOWORD(wParam)), (char *)lParam, HIWORD(wParam));
		m_sync.Unlock();

		break;

// 	case DLL_ALERT:
// 		if (HIWORD(wParam) == alert_DEIN && m_stop)
// 			break;
// 
// 		m_sync.Lock();
// 		alert((char *)lParam, wParam);
// 		m_sync.Unlock();
// 
// 		break;

	case DLL_ALERTx:
		if (HIWORD(wParam) == alert_DEIN && m_stop)
			break;
		
		m_sync.Lock();
		alertx(wParam, lParam);
		m_sync.Unlock();
		
		//2016.02.16 KSJ 종목명컨트롤에 실시간 데이터 전달
		m_symbol->SendMessage(WM_USER, wParam, lParam);
		break;


	case DLL_SETPAL:
		m_clrBackIn = getColor(clBackIn);
		m_clrBack = getColor(clBack);
		m_clrContentsBackIn = getColor(clcontentsBack);
		m_strPal = GetPalette();
		SetBitmap();
		if (m_panel)
		{
			int nLineTick[4];
			nLineTick[0] = GAP_SIDE;	nLineTick[1] = GAP_TOP;
			nLineTick[2] = GAP_SIDE;	nLineTick[3] = GAP_BLOCK;
			m_panel->SetCornerRound(m_hBmpRoundPanel, 5, &nLineTick[0]);
			//m_panel->SetBkColor(getColor(clPanel), false);
			m_panel->SetBkColor(getColor(m_clrBackIn));
			//m_panel->SetBk2Color(getColor(clPanel));
			m_panel->SetBorderColor(getColor(clBack));
			/*
			nLineTick[0] = GAP_SIDE;	nLineTick[1] = 0;
			nLineTick[2] = GAP_SIDE;	nLineTick[3] = GAP_BOTTOM;
			*/
			m_contentspanel->SetCornerRound(m_hBmpRoundContents, 5, &nLineTick[0]);
			m_contentspanel->SetBkColor(getColor(clContents));
			m_contentspanel->SetBorderColor(getColor(clBack));
		}
		if (m_left)
			m_left->SetBkColor(m_clrBack);
		if (m_top)
			m_top->SetBkColor(m_clrBack);
		if (m_right)
			m_right->SetBkColor(m_clrBack);
		if (m_bottom)
			m_bottom->SetBkColor(m_clrBack);
		if (m_center)
			m_center->SetBkColor(m_clrBack);
		if (m_grid)
		{
			for (int ii = 0; ii < nCOL; ii++)
			{
				m_grid->SetItemBkColor(0, ii, getColor(clHead), true);
				m_grid->SetItemFgColor(0, ii, getColor(clHText), true);
			}
			m_grid->SetGridColor(getColor(clLine));
		}
		m_symbol->SendMessage(WM_USER, MAKEWPARAM(DLL_SETPAL, 0));
		
		Invalidate();
		if (m_code) m_code->RedrawWindow();
		break;

	case DLL_DOMINO:
	case DLL_TRIGGER:
		if (!HIWORD(wParam))
		{
			int	pos{};
			CString	sym, text, data;

			text = (char *)lParam;
			for ( ; !text.IsEmpty(); )
			{
				pos = text.Find('\n');
				if (pos != -1)
				{
					data = text.Left(pos++);
					text = text.Mid(pos);
				}
				else
				{
					data = text;
					text.Empty();
				}
				pos = data.Find('\t');
				if (pos != -1)
				{
					sym  = data.Left(pos++);
					data = data.Mid(pos);
				}
				else
				{
					sym = data;
					data.Empty();
				}
				if (!sym.CompareNoCase("1301"))
				{
					data.TrimRight();
					if (m_code)
					{
						sym = m_code->GetEditData();
						if (sym.IsEmpty() || sym.CompareNoCase(data))
						{
							m_code->SetEditData(data);
							inquiry(false);
						}
					}
					break;
				}
			}
			CString strBuffer = (char*)lParam;
			CString str = Parser(strBuffer, '\n');
			str.TrimRight();
			if (!sym.CompareNoCase("edMarketTrigger"))
			{
				CString strBuffer = (char*)lParam;
				pos = strBuffer.Find('\t');
				if (pos != -1)
				{
					str = strBuffer.Mid(pos+1);
				}
				_variant_t var((LPCTSTR)str);
				m_pMarketpicker->SetCtrlProperty("sMarket", var);
			}

			}
		break;

	case DLL_DROP:
		if (m_code && HIBYTE(HIWORD(wParam)) != LOBYTE(HIWORD(wParam)))
		{
			int	pos;
			CString	text;

			text = (char *)lParam;
			pos  = text.Find('\n');
			if (pos != -1 && !text.Left(pos).CompareNoCase("1301"))
			{
				CString	code;

				text = text.Mid(++pos);
				code = m_code->GetEditData();
				if (code.IsEmpty() || code.CompareNoCase(text))
				{
					m_code->SetEditData(text);
					inquiry(false);
				}
			}
		}
		break;
	}

	return 0;
}

BOOL CMapWnd::create(CWnd* parent)
{
	if (!CWnd::Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(0, 0, MAP_WIDTH, MAP_HEIGHT), parent, 100))
		return FALSE;

	LOGFONT	lf{};
	GVITEM	item{};

	m_hRoundBitmap[0] = getBitmap(_T("axpanel1_lt.bmp"));
	m_hRoundBitmap[1] = getBitmap(_T("axpanel1_rt.bmp"));
	m_hRoundBitmap[2] = getBitmap(_T("axpanel1_lb.bmp"));
	m_hRoundBitmap[3] = getBitmap(_T("axpanel1_rb.bmp"));
	SetBitmap();
	int nLineTick[4];
	nLineTick[0] = GAP_SIDE;
	nLineTick[1] = GAP_TOP;
	nLineTick[2] = GAP_SIDE;
	nLineTick[3] = GAP_BLOCK;

	int itop = 0;
	m_panel = std::make_unique<CfxStatic>();
	m_panel->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, itop, MAP_WIDTH, 34), this, IDC_STATIC);
	m_panel->SetBkColor(m_clrBackIn);
	m_panel->SetBorderColor(getColor(clBack));
	//m_panel->SetCornerRound(m_hRoundBitmap);
	m_panel->SetCornerRound(m_hBmpRoundPanel, 5, &nLineTick[0]);
	/*
	nLineTick[0] = GAP_SIDE;
	nLineTick[1] = 0;
	nLineTick[2] = GAP_SIDE;
	nLineTick[3] = GAP_BOTTOM;
	*/

	m_contentspanel = std::make_unique<CfxStatic>();
	m_contentspanel->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 31, MAP_WIDTH, 
		MAP_HEIGHT-32), this, IDC_STATIC);
	
	m_contentspanel->SetCornerRound(m_hBmpRoundContents, 5, &nLineTick[0]);
	m_contentspanel->SetBkColor(getColor(clContents));
	m_contentspanel->SetBorderColor(getColor(clBack));

	const int icontentstop = 34 + 4;
	const int icontentsright = MAP_WIDTH - 4;

/*	m_left = std::make_unique<CfxStatic>();
	m_left->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 25, 3, MAP_HEIGHT), this, IDC_STATIC);
	m_left->SetBkColor(m_clrBack);

	m_top = std::make_unique<CfxStatic>();
	m_top->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0,25,MAP_WIDTH,3), this, IDC_STATIC);
	m_top->SetBkColor(m_clrBack);

	m_right = std::make_unique<CfxStatic>();
	m_right->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(MAP_WIDTH-3,25,MAP_WIDTH,MAP_HEIGHT), this, IDC_STATIC);
	m_right->SetBkColor(m_clrBack);

	m_bottom = std::make_unique<CfxStatic>();
	m_bottom->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0,MAP_HEIGHT-3,MAP_WIDTH,MAP_HEIGHT), this, IDC_STATIC);
	m_bottom->SetBkColor(m_clrBack);

	m_center = std::make_unique<CfxStatic>();
	m_center->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0,154,MAP_WIDTH,157), this, IDC_STATIC);
	m_center->SetBkColor(m_clrBack);
*/
	CRect tmpRC;
	itop+=3;

	//시장구분자 
	tmpRC = CRect(0,0,20, 20);
	m_pMarketpicker = (CContainerWnd*) new CContainerWnd;
	m_pMarketpicker->SetParent(m_parent);
	m_pMarketpicker->Create(NULL, NULL, WS_CHILDWINDOW | WS_VISIBLE, tmpRC, this, 0);
	const int major = m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC));
	CString cMajor;
	cMajor.Format("/tIB230100/k2/v%d", major);
	m_pMarketpicker->CreateControl(m_root, "CX_MarketPicker", "cxMarket", tmpRC, cMajor);


	m_pMarketpicker->GetWindowRect(tmpRC);
	ScreenToClient(tmpRC);
	tmpRC.OffsetRect(5, 5);
	m_pMarketpicker->MoveWindow(tmpRC);

	m_sMarket = m_pMarketpicker->GetCtrlProperty("sMarket");
	m_sMarket.TrimRight();
	/// ///////


	m_code = std::make_unique<CfxCodeCtrl>(this, m_root + "\\" + TABDIR + "\\");

	tmpRC = CRect(6, 3 + itop, 68, 23 + itop);
	tmpRC.OffsetRect(20, 0);

	//m_code->Create(this, CRect(6,3+itop,68,23+itop), IDC_CODE);
	m_code->Create(this, tmpRC, IDC_CODE);
	m_code->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	m_code->SetUnit(GU_CODE);
	m_code->SetFont(m_font);

	m_searchBN = std::make_unique<CfxImgButton>();

	tmpRC = CRect(70, 3 + itop, 90, 23 + itop);
	tmpRC.OffsetRect(20, 0);

	//m_searchBN->Create(_T(""), CRect(70,3+itop,90,23+itop), this, IDC_SEARCH);
	m_searchBN->Create(_T(""), tmpRC, this, IDC_SEARCH);
	m_searchBN->SetImgBitmap(getBitmap("S검색.bmp"), getBitmap("S검색_DN.bmp"), getBitmap("S검색_EN.bmp"));
	m_searchBN->SetTextColor(getColor(clText));
	m_searchBN->SetFont(m_font, TRUE);

	CString	text;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*) = nullptr;

	text.Format("%s/%s/cx_symbol.dll", m_root, DEVDIR);
	m_inst = AfxLoadLibrary(text);
	if (m_inst != NULL)
	{
		axCreate = (CWnd* (APIENTRY*)(CWnd*, void*))GetProcAddress(m_inst, _T("axCreate"));
		if (axCreate != NULL)
		{
			struct	_param	param;

			param.key   = 0;
			param.name  = _T("17413");
			tmpRC = CRect(92, 3 + itop, 224, 23 + itop);
			tmpRC.OffsetRect(20, 0);
			param.rect = tmpRC;
			//param.rect  = CRect(92,3+itop,224,23+itop);
			param.fonts = S_FONT;
			param.point = 9;
			param.style = 1;
			param.tRGB  = 69;
			param.pRGB  = 90;
			param.options = _T("/a89/b91/c92/d69/i99/s2301");

			m_symbol = (*axCreate)(parent, &param);
			m_symbol->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			if (m_symbol == NULL)
			{
				AfxFreeLibrary(m_inst);
				m_inst = NULL;
			}
		}
	}
//CRect(MAP_WIDTH-5-64-1,3,MAP_WIDTH-5-32-1,23)
	m_stopBN = std::make_unique<CfxImgButton>();
	int ileft = MAP_WIDTH-6-73-1;
	m_stopBN->Create(S_STOP, CRect(ileft, 3+itop, ileft+36,23+itop), this, IDC_STOP, TRUE);
	m_stopBN->SetImgBitmap(getBitmap("2BTN.bmp"), getBitmap("2BTN_DN.bmp"), getBitmap("2BTN_EN.bmp"));
	m_stopBN->SetCheck(!m_stop);
	m_stopBN->SetTextColor(getColor(clText));
	m_stopBN->SetFont(m_sfont, TRUE);
	ileft += 36;

	m_inqBN = std::make_unique<CfxImgButton>();
	ileft += 1;
	m_inqBN->Create(S_INQ, CRect(ileft,3+itop,ileft+37,23+itop), this, IDC_INQ);
	m_inqBN->SetImgBitmap(getBitmap("조회BTN.bmp"), getBitmap("조회BTN_DN.bmp"), getBitmap("조회BTN_EN.bmp"));
	m_inqBN->SetTextColor(getColor(clText));
	m_inqBN->SetFont(m_sfont, TRUE);

	const int iw = (icontentsright-5) / 2;
	m_tbl1 = std::make_unique<CfxGrid>();
	m_tbl1->Create(CRect(4,icontentstop,iw,146), this, IDC_TBL1, GVSC_NONE, GVDD_NONE);
	m_tbl1->SetRowColResize(FALSE, FALSE);
	m_tbl1->Initial(nROWx, nCOLx, 1, 0);
	m_tbl1->SetConditionColor(getColor(clDown), getColor(clUp), getColor(clSText));

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
	for (int ii = 0; ii < nCOLx; ii++)
	{
		item.col    = ii;
		item.mask   = GVMK_FORMAT|GVMK_ATTR|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
		item.format = tbls1[ii].format;
		item.attr   = tbls1[ii].attr;
		item.fgcol  = getColor(tbls1[ii].tclr);
		item.bkcol  = getColor(tbls1[ii].dclr);
		CopyMemory(&item.font, &lf, sizeof(LOGFONT));
		m_tbl1->SetColumnItems(ii, &item);
		m_tbl1->SetColumnWidth(ii, tbls1[ii].width);

		item.row    = 0;
		item.mask   = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_ATTR;
		item.attr   = GVAT_ONLYROW;
		if (tbls1[ii].attr & GVAT_HIDDEN)
			item.attr |= GVAT_HIDDEN;
		item.text   = tbls1[ii].heads;
		item.format = GVFM_HEADER;
		item.fgcol  = getColor(clHeadText);//clHText);
		item.bkcol  = getColor(tbls1[ii].hclr);
		m_tbl1->SetItem(&item);
	}

	for (int ii = 0; ii < nROWx; ii++)
		m_tbl1->SetRowHeight(ii, hROW);
	m_tbl1->SetGridColor(getColor(clLine));
	m_tbl1->SetGridFocusLine(FALSE, FALSE);
	m_tbl1->SetGridDIScroll(true);
	m_tbl1->SetGridLine(GVLN_VERT);
	m_tbl1->Adjust();

	m_tbl2 = std::make_unique<CfxGrid>();
	m_tbl2->Create(CRect(iw-1,icontentstop,icontentsright,146), this, IDC_TBL2, GVSC_NONE, GVDD_NONE);
	m_tbl2->SetRowColResize(FALSE, FALSE);
	m_tbl2->Initial(nROWx, nCOLx, 1, 0);
	m_tbl2->SetConditionColor(getColor(clUp), getColor(clDown), getColor(clText));

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
	for (int ii = 0; ii < nCOLx; ii++)
	{
		item.col    = ii;
		item.mask   = GVMK_FORMAT|GVMK_ATTR|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
		item.format = tbls2[ii].format;
		item.attr   = tbls2[ii].attr;
		item.fgcol  = getColor(tbls2[ii].tclr);
		item.bkcol  = getColor(tbls2[ii].dclr);
		CopyMemory(&item.font, &lf, sizeof(LOGFONT));
		m_tbl2->SetColumnItems(ii, &item);
		m_tbl2->SetColumnWidth(ii, tbls2[ii].width);

		item.row    = 0;
		item.mask   = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_ATTR;
		item.attr   = GVAT_ONLYROW;
		if (tbls2[ii].attr & GVAT_HIDDEN)
			item.attr |= GVAT_HIDDEN;
		item.text   = tbls2[ii].heads;
		item.format = GVFM_HEADER;
		item.fgcol  = getColor(clHeadText);
		item.bkcol  = getColor(tbls2[ii].hclr);
		m_tbl2->SetItem(&item);
	}
	for (int ii = 0; ii < nROWx; ii++)
		m_tbl2->SetRowHeight(ii, hROW);
	m_tbl2->SetGridColor(getColor(clLine));
	m_tbl2->SetGridFocusLine(FALSE, FALSE);
	m_tbl2->SetGridDIScroll(true);
	m_tbl2->SetGridLine(GVLN_VERT);
	m_tbl2->Adjust();

	itop = 145;

	m_schg = std::make_unique<CfxStatic>();//1
	m_schg->Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(4,itop,76,itop+20), this, IDC_SCHG);
	m_schg->Init(ssTEXT|ssCOLOR|ssBORDER, attrNUMERIC|attrCOMMA, _T(""), _T(""), getColor(78), 0, getColor(clDown), alRIGHT);
	m_schg->SetBorderColor(getColor(clLine), true);
	m_schg->SetFont(m_font);

	m_svol = std::make_unique<CfxStatic>();
	m_svol->Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(75,itop,161,itop+20), this, IDC_SVOL);
	m_svol->Init(ssTEXT|ssCOLOR|ssBORDER, attrNUMERIC|attrCOMMA, _T(""), _T(""), getColor(78), 0, getColor(clDown), alRIGHT);
	m_svol->SetBorderColor(getColor(clLine), true);
	m_svol->SetFont(m_font);

	m_label = std::make_unique<CfxStatic>();
	m_label->Create(_T("외국계합"), WS_CHILD|WS_VISIBLE, CRect(160,itop,342,itop+20), this, IDC_STATIC);
	m_label->SetStyle(ssTEXT|ssCOLOR|ssBORDER, false);
	m_label->SetFont(m_font);
	setStaticColor(m_label.get(), getColor(78), getColor(69), getColor(clLine));
//m_label->ShowWindow(SW_HIDE);

	m_bvol = std::make_unique<CfxStatic>();
	m_bvol->Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(341, itop,427,itop+20), this, IDC_BVOL);
	m_bvol->Init(ssTEXT|ssCOLOR|ssBORDER, attrNUMERIC|attrCOMMA, _T(""), _T(""), getColor(78), 0, getColor(clUp), alRIGHT);
	m_bvol->SetBorderColor(getColor(clLine), true);
	m_bvol->SetFont(m_font);

	m_bchg = std::make_unique<CfxStatic>();
	m_bchg->Create(_T(""), WS_CHILD|WS_VISIBLE, CRect(426,itop,icontentsright,itop+20), this, IDC_BCHG);
	m_bchg->Init(ssTEXT|ssCOLOR|ssBORDER, attrNUMERIC|attrCOMMA, _T(""), _T(""), getColor(78), 0, getColor(clUp), alRIGHT);
	m_bchg->SetBorderColor(getColor(clLine), true);
	m_bchg->SetFont(m_font);

	itop+=26;
	m_grid = std::make_unique<CfxGrid>();
	m_grid->Create(CRect(4,itop,MAP_WIDTH,MAP_HEIGHT-3), this, IDC_GRID, GVSC_NONE, GVDD_NONE);
	m_grid->SetRowColResize(FALSE, FALSE);
	m_grid->Initial(nROW, nCOL, 1, 0);
	m_grid->SetUpColor(getColor(clUp));
	m_grid->SetDnColor(getColor(clDown));

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
		item.fgcol  = getColor(infos[ii].tclr);
		item.bkcol  = getColor(infos[ii].dclr);
		CopyMemory(&item.font, &lf, sizeof(LOGFONT));
		m_grid->SetColumnItems(ii, &item);
		m_grid->SetColumnWidth(ii, infos[ii].width);

		item.row    = 0;
		item.mask   = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_ATTR;
		item.attr   = GVAT_ONLYROW;
		if (infos[ii].attr & GVAT_HIDDEN)
			item.attr |= GVAT_HIDDEN;
		item.text   = infos[ii].heads;
		item.format = GVFM_HEADER;
		item.fgcol  = getColor(clHText);
		item.bkcol  = getColor(clHead);
		m_grid->SetItem(&item);
	}

	for (int ii = 0; ii < nROW; ii++)
		m_grid->SetRowHeight(ii, hROW);

	m_grid->SetGridColor(getColor(clLine));
	m_grid->SetGridFocusLine(FALSE, FALSE);
	m_grid->SetGridDIScroll(true);
	m_grid->SetGridScroll(nROW);
	m_grid->SetGridLine(GVLN_VERT);
	m_grid->Adjust();

	SetTimer(TM_ATTACH, 50, NULL);
// 	SetTimer(1, 2000, NULL);

	return TRUE;
}

void CMapWnd::sendTR()
{
	struct	_inR	inR {};
	CString	text, code, stmp;

	doWait(TRUE);

	//2011.22.23 KSJ
	if (m_bFirst)
	{
		m_tbl1->Clear();
		m_tbl2->Clear();
		m_grid->Clear();
	}

	m_rows = 1;

	code = m_code->GetEditData();
	text.Format("1301%c%s\t", 0x7f, code);

	//거래소별 종목코드 분류를 위한 코드추가. lwj
	_variant_t var((LPCTSTR)code);
	m_pMarketpicker->SetCtrlProperty("sCode", var);

	if (m_sMarket == "KRX")
	{
		stmp.Format("1777%c%s\t", 0x7f, "1");
		text += stmp;
	}
	else if (m_sMarket == "NXT")
	{
		stmp.Format("1777%c%s\t", 0x7f, "2");
		text += stmp;
		code = "N." + code;
	}
	else
	{
		stmp.Format("1777%c%s\t", 0x7f, "3");
		text += stmp;
		code = "M." + code;
	}

	for (int ii = 0; ii < L_syms; ii++)
	{
		if (!strlen(syms[ii].sym))
			continue;
		text += syms[ii].sym;
		text += '\t';
	}
// 	CString s;
// 	s.Format("SENDTR [%s]\n",text);
// 	OutputDebugString(s);
	sendTR(oopTR, oKEY, (char *)text.operator LPCTSTR(), text.GetLength(), true);

	FillMemory(&inR, L_inR, ' ');
	if (!code.IsEmpty())
		CopyMemory(inR.code, code, min(code.GetLength(), sizeof(inR.code)));
	sendTR(gridTR, gKEY, (char *)&inR, L_inR);
}

void CMapWnd::sendTR(CString trN, int key, char* datb, int datl, bool oop)
{
	std::unique_ptr<char[]>	wb = std::make_unique<char[]>(L_userTH + datl);
	struct	_userTH* userth{};
	FillMemory(wb.get(), L_userTH+datl, ' ');
	userth = (struct _userTH *)wb.get();
	CopyMemory(userth->trc, trN, sizeof(userth->trc));
	userth->key  = key;
	//2011.11.23 KSJ
//	userth->stat = oop ? US_OOP : 0;
	userth->stat = oop ? US_OOP|US_PASS : US_PASS;
	CopyMemory(&wb.get()[L_userTH], datb, datl);

	m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)wb.get());
	wb = nullptr;
}

//처음 조회시나 코드명이 바꼈을때.
void CMapWnd::FirstSearch(int key, char* datb, int datl)
{
	int	value=0;
	CString	text;
	CString strTemp, strChag, strSmsu;

	CString stmp;
	stmp.Format("[memo][IB230100]  key =[%s]   len=[%d]\r\n", GetMemoTrKeyType(key), datl);
	OutputDebugString(stmp);

	switch (key)
	{
	case TRKEY_MEMO_POPUPOOP:
	case TRKEY_MEMO_INSERT:
	case TRKEY_MEMO_SEARCH:
	case TRKEY_MEMO_UPDATE:
	case TRKEY_MEMO_DELETE:
	case TRKEY_MEMO_CHECK:
	{
		WPARAM	wParam{};
		wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, key), HIWORD(wParam));
		text = CString(datb, datl);
		m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
		//break;	그리드데이터에 들어가는 오류 발생하여 break -> return으로 처리.
		return;
	}
	default:
		break;
	}

	if (key == oKEY)
	{
		text  = CString(datb, datl);
// 		strTemp.Format("2301 : [%s]\n",text);
// 		OutputDebugString(strTemp);

		value = text.Find('\t');

		if (value != -1)
		{
			WPARAM	wParam=0;
			CStringArray	arr;

			value = parse(text, arr);
		
			if(value == 1)
			{
//				text = arr.GetAt(1);
				const int length = text.GetLength();
				wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, oKEY), (text.GetLength() > 6) ? text.GetLength() : 0);
				m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
			}
			else
			{
				for (int ii = 0; ii < value; ii++)
				{
					switch (syms[ii].flag)
					{
					case -1:
						m_keys = arr.GetAt(ii);

// 						strTemp.Format("[Dispatch][%s][%s]", text, m_keys);
// 						OutputDebugString(strTemp);

						break;
					case -2:
						
						//2016.02.18 KSJ 종목명 클릭했을때(value=3)는 0번째 처음조회시(value=45)에는 1번째 접근 
						if(value > 10)	text = arr.GetAt(1);
						else			text = arr.GetAt(0);

						wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, oKEY), (text.GetLength() > 6) ? text.GetLength() : 0);
						m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
						break;
					case 0xff:
						m_schg->SetText(_T(""));
						break;
					case 0xfe:
						m_svol->SetText(arr.GetAt(ii));
						m_fsvol = arr.GetAt(ii);
						break;
					case 0xfd:
						m_bvol->SetText(arr.GetAt(ii));
						m_fbvol = arr.GetAt(ii);
						break;
					case 0xfc:
						m_bchg->SetText(_T(""));
						break;
					case 0:
						m_tbl1->SetItemText(syms[ii].row, syms[ii].col, arr.GetAt(ii));
						if (syms[ii].col == iSVOL)
						{
							m_tbl1->SetItemText(syms[ii].row, iSVOLp, arr.GetAt(ii));
// 							strTemp.Format("SVOL [%d] [%s]\n",syms[ii].row,arr.GetAt(ii));
// 							OutputDebugString(strTemp);
						}
						break;
					case 1:
						m_tbl2->SetItemText(syms[ii].row, syms[ii].col, arr.GetAt(ii));
						if (syms[ii].col == iBVOL)
						{
							m_tbl2->SetItemText(syms[ii].row, iBVOLp, arr.GetAt(ii));
// 							strTemp.Format("BVOL [%d] [%s]\n",syms[ii].row,arr.GetAt(ii));
// 							OutputDebugString(strTemp);
						}
						break;
					}
				}

			}
		}
		m_label->Invalidate();
		return;
	}

	struct	_outR* outR{};
	struct	_recR* recR{};

	if (datl < L_outR)
	{
		doWait(FALSE);
// 		m_code->SetFocus();
		return;
	}

	outR = (struct _outR *)datb;
	recR = (struct _recR *)&datb[L_outR];
	m_keyx = CString(outR->rts, sizeof(outR->rts));
	value  = atoi(CString(outR->nrec, sizeof(outR->nrec)));

	//2011.11.23 KSJ
	CRect rt;
	m_grid->GetClientRect(&rt);
	m_grid->Invalidate(FALSE);
	m_grid->InvalidateRect(rt, FALSE);

	for (int ii = 1; ii <= value; ii++, recR++)
	{
		text = CString(recR->code, sizeof(recR->code));
		text.TrimRight();

// 		OutputDebugString("[KSJ] text[" + text + "]");

		m_grid->SetItemText(ii, iCODE,  text);
		text = CString(recR->name, sizeof(recR->name));
		if (text.Find('+') == 0)
			text = text.Mid(1);
		if (recR->name[0] == '+')
		{
			if (atoi(CString(recR->smsu, sizeof(recR->smsu))) >= 0)
				text = "+" + text;
			else	text = "-" + text;
		}

		m_grid->SetItemText(ii, iNAME,  text);
		m_grid->SetItemText(ii, iSVOLx, CString(recR->dvol, sizeof(recR->dvol)));
		m_grid->SetItemText(ii, iBVOLx, CString(recR->svol, sizeof(recR->svol)));
		
		
		//2011.11.18 KSJ
		//m_grid->SetItemText(ii, iBBVL,  CString(recR->smsu, sizeof(recR->smsu)));
		strSmsu = CString(recR->smsu, sizeof(recR->smsu));
		
		strSmsu.Replace(" ", "");
		
		//-가 아니면 +이다.
		if(strSmsu.GetAt(0) != '-' && strSmsu.GetAt(0) != '+')
		{
			strSmsu = "+" + strSmsu;
		}
		
		m_grid->SetItemText(ii, iBBVL,  strSmsu);
		m_grid->SetItemText(ii, iBBVLp, CString(recR->smsu, sizeof(recR->smsu)));
		
		//2011.11.17 KSJ
		//그리드쪽은 실시간데이터가없다. 모두 조회성 데이터이다.
		//m_grid->SetItemText(ii, iCHAG,  _T("0"));	//CString(recR->chag, sizeof(recR->chag)));
		strChag = CString(recR->chag, sizeof(recR->chag));

		strChag.Replace(" ", "");

		//-가 아니면 +이다.
		if(strChag.GetAt(0) != '-' && strChag.GetAt(0) != '+')
		{
			strChag = "+" + strChag;
		}

		m_grid->SetItemText(ii, iCHAG,  strChag);

		text = CString(recR->time, sizeof(recR->time));
		text = text.Left(4);
		text.Insert(2, ':');
		m_grid->SetItemText(ii, iTIME, text);
		m_grid->SetItemText(ii, iRATE, setPercent(CString(recR->rate, sizeof(recR->rate))));
	}

	if (value > 0)
	{
		DWORD	attr=0;

		m_sort = (m_sort > 0) ? m_sort : iBBVL;
		attr = m_grid->GetItemAttr(0, m_sort);
		m_grid->SetItemAttr(0, m_sort, attr|GVAT_SORTVAL);
		m_grid->Sort(m_sort, !m_descend);
		m_grid->Invalidate(FALSE);
		m_grid->SetItemAttr(0, m_sort, attr);

		m_rows = value+1;
	}
	
	//2011.11.23 KSJ
	m_grid->Invalidate(TRUE);
	m_grid->InvalidateRect(rt, TRUE);

	doWait(FALSE);
//  	m_code->SetFocus();
}

//alert로 들어갔을때.
void CMapWnd::AlertSearch(int key, char* datb, int datl)
{
	int	value=0;
	CString	text;
	CString strTemp, strChag, strSmsu;

	struct	_outR* outR{};
	struct	_recR* recR{};

	// AlertSearch 처리 시 firstsearch 부분과 동일하게 처리하도록 수정. 25.09.11 lwj---
	switch (key)
	{
	case TRKEY_MEMO_POPUPOOP:
	case TRKEY_MEMO_INSERT:
	case TRKEY_MEMO_SEARCH:
	case TRKEY_MEMO_UPDATE:
	case TRKEY_MEMO_DELETE:
	case TRKEY_MEMO_CHECK:
	{
		WPARAM	wParam{};
		wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, key), HIWORD(wParam));
		text = CString(datb, datl);
		m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
		return;
	}
	default:
		break;
	}

	if (key == oKEY)
	{
		text = CString(datb, datl);
		// 		strTemp.Format("2301 : [%s]\n",text);
		// 		OutputDebugString(strTemp);

		value = text.Find('\t');

		if (value != -1)
		{
			WPARAM	wParam = 0;
			CStringArray	arr;

			value = parse(text, arr);

			if (value == 1)
			{
				//				text = arr.GetAt(1);
				const int length = text.GetLength();
				wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, oKEY), (text.GetLength() > 6) ? text.GetLength() : 0);
				m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
			}
			else
			{
				for (int ii = 0; ii < value; ii++)
				{
					switch (syms[ii].flag)
					{
					case -1:
						m_keys = arr.GetAt(ii);

						// 						strTemp.Format("[Dispatch][%s][%s]", text, m_keys);
						// 						OutputDebugString(strTemp);

						break;
					case -2:

						//2016.02.18 KSJ 종목명 클릭했을때(value=3)는 0번째 처음조회시(value=45)에는 1번째 접근 
						if (value > 10)	text = arr.GetAt(1);
						else			text = arr.GetAt(0);

						wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, oKEY), (text.GetLength() > 6) ? text.GetLength() : 0);
						m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
						break;
					case 0xff:
						m_schg->SetText(_T(""));
						break;
					case 0xfe:
						m_svol->SetText(arr.GetAt(ii));
						m_fsvol = arr.GetAt(ii);
						break;
					case 0xfd:
						m_bvol->SetText(arr.GetAt(ii));
						m_fbvol = arr.GetAt(ii);
						break;
					case 0xfc:
						m_bchg->SetText(_T(""));
						break;
					case 0:
						m_tbl1->SetItemText(syms[ii].row, syms[ii].col, arr.GetAt(ii));
						if (syms[ii].col == iSVOL)
						{
							m_tbl1->SetItemText(syms[ii].row, iSVOLp, arr.GetAt(ii));
							// 							strTemp.Format("SVOL [%d] [%s]\n",syms[ii].row,arr.GetAt(ii));
							// 							OutputDebugString(strTemp);
						}
						break;
					case 1:
						m_tbl2->SetItemText(syms[ii].row, syms[ii].col, arr.GetAt(ii));
						if (syms[ii].col == iBVOL)
						{
							m_tbl2->SetItemText(syms[ii].row, iBVOLp, arr.GetAt(ii));
							// 							strTemp.Format("BVOL [%d] [%s]\n",syms[ii].row,arr.GetAt(ii));
							// 							OutputDebugString(strTemp);
						}
						break;
					}
				}

			}
		}
		m_label->Invalidate();
		return;
	}
	//----------------------------------------------

	if (datl < L_outR)
	{
		doWait(FALSE);
// 		m_code->SetFocus();
		return;
	}

	outR = (struct _outR *)datb;
	recR = (struct _recR *)&datb[L_outR];
	m_keyx = CString(outR->rts, sizeof(outR->rts));
	value  = atoi(CString(outR->nrec, sizeof(outR->nrec)));

	//2011.11.23 KSJ
	CRect rt;
	m_grid->GetClientRect(&rt);
	m_grid->Invalidate(FALSE);
	m_grid->InvalidateRect(rt, FALSE);

	for (int ii = 1; ii <= value; ii++, recR++)
	{
		text = CString(recR->code, sizeof(recR->code));
		text.TrimRight();
		m_grid->SetItemText(ii, iCODE,  text);
		text = CString(recR->name, sizeof(recR->name));
		if (text.Find('+') == 0)
			text = text.Mid(1);
		if (recR->name[0] == '+')
		{
			if (atoi(CString(recR->smsu, sizeof(recR->smsu))) >= 0)
				text = "+" + text;
			else	text = "-" + text;
		}

		m_grid->SetItemText(ii, iNAME,  text);
		m_grid->SetItemText(ii, iSVOLx, CString(recR->dvol, sizeof(recR->dvol)));
		m_grid->SetItemText(ii, iBVOLx, CString(recR->svol, sizeof(recR->svol)));
		
		
		//2011.11.18 KSJ
		//m_grid->SetItemText(ii, iBBVL,  CString(recR->smsu, sizeof(recR->smsu)));
		strSmsu = CString(recR->smsu, sizeof(recR->smsu));
		
		strSmsu.Replace(" ", "");
		
		//-가 아니면 +이다.
		if(strSmsu.GetAt(0) != '-' && strSmsu.GetAt(0) != '+')
		{
			strSmsu = "+" + strSmsu;
		}
		
		m_grid->SetItemText(ii, iBBVL,  strSmsu);
		m_grid->SetItemText(ii, iBBVLp, CString(recR->smsu, sizeof(recR->smsu)));
		
		//2011.11.17 KSJ
		//그리드쪽은 실시간데이터가없다. 모두 조회성 데이터이다.
		//m_grid->SetItemText(ii, iCHAG,  _T("0"));	//CString(recR->chag, sizeof(recR->chag)));
		strChag = CString(recR->chag, sizeof(recR->chag));

		strChag.Replace(" ", "");

		//-가 아니면 +이다.
		if(strChag.GetAt(0) != '-' && strChag.GetAt(0) != '+')
		{
			strChag = "+" + strChag;
		}

		m_grid->SetItemText(ii, iCHAG,  strChag);

		text = CString(recR->time, sizeof(recR->time));
		text = text.Left(4);
		text.Insert(2, ':');
		m_grid->SetItemText(ii, iTIME, text);
		m_grid->SetItemText(ii, iRATE, setPercent(CString(recR->rate, sizeof(recR->rate))));
	}

	if (value > 0)
	{
		DWORD	attr=0;

		m_sort = (m_sort > 0) ? m_sort : iBBVL;
		attr = m_grid->GetItemAttr(0, m_sort);
		m_grid->SetItemAttr(0, m_sort, attr|GVAT_SORTVAL);
		m_grid->Sort(m_sort, !m_descend);
		m_grid->Invalidate(FALSE);
		m_grid->SetItemAttr(0, m_sort, attr);

		m_rows = value+1;
	}
	
	//2011.11.23 KSJ
	m_grid->Invalidate(TRUE);
	m_grid->InvalidateRect(rt, TRUE);

	doWait(FALSE);
}

void CMapWnd::dispatch(int key, char* datb, int datl)
{
// 	OutputDebugString("[KSJ] dispatch");
	//2011.11.24 KSJ
	//코드가 바뀌면 그리드 내용을 지워준다.
	if(m_strCode.Compare(m_code->GetEditData()))
	{
		m_bFirst = true;
		m_tbl1->Clear();
		m_tbl2->Clear();
		m_grid->Clear();
	}

	if(m_bFirst)
	{
		FirstSearch(key, datb, datl);
		m_strCode = m_code->GetEditData();
	}
	else
	{
		AlertSearch(key, datb, datl);
	}
}

void CMapWnd::alert(CString data, WPARAM wParam)
{
	CString strTemp;

	int	pos;
	CString	text, strSourceText;

	//2012.01.31 KSJ 
	strSourceText = data;
	// KSJ

	pos = data.Find('\t');
	if (pos == -1 || m_stop)
		return;

	text = data.Left(pos++);
	data = data.Mid(pos);

// 	strTemp.Format("Alert[text.CompareNoCase][%d] : text = %s, m_keys = %s, Data = %s", !text.CompareNoCase(m_keys), text, m_keys, data);
// 	OutputDebugString(strTemp);
	if (!text.IsEmpty() && !m_keys.IsEmpty() && !text.CompareNoCase(m_keys))
	{
		int	value=0;
		CString	syms, string;

		string = data;
		for ( ; !string.IsEmpty(); )
		{
			pos = string.Find('\t');
			if (pos != -1)
			{
				syms   = string.Left(pos++);
				string = string.Mid(pos);
			}
			else	break;
			pos = string.Find('\t');
			if (pos != -1)
			{
				text   = string.Left(pos++);
				string = string.Mid(pos);
			}
			else
			{
				text = string;
				string.Empty();
			}

			if (m_rts.Lookup(syms, (void *&)value))
			{
				switch (LOWORD(value))
				{
				case 0:
					m_tbl1->SetItemText(LOBYTE(HIWORD(value)), HIBYTE(HIWORD(value)), text);
					if (HIBYTE(HIWORD(value)) == iSVOL)
					{
						syms = m_tbl1->GetItemText(LOBYTE(HIWORD(value)), iSVOLp);
						syms.Format("%.f", atof(text) - atof(syms));
						m_tbl1->SetItemText(LOBYTE(HIWORD(value)), iSVOLp, text);
						m_tbl1->SetItemText(LOBYTE(HIWORD(value)), iSCHG,  !atof(syms) ? _T("") : syms);

// 						strTemp.Format("Alert[매도] : value = %d, text = %s, 변동량 = %s", value, text, syms);
// 						OutputDebugString(strTemp);
					}
					break;
				case 1:
					m_tbl2->SetItemText(LOBYTE(HIWORD(value)), HIBYTE(HIWORD(value)), text);
					if (HIBYTE(HIWORD(value)) == iBVOL)
					{
						syms = m_tbl2->GetItemText(LOBYTE(HIWORD(value)), iBVOLp);
						syms.Format("%.f", atof(text) - atof(syms));
						m_tbl2->SetItemText(LOBYTE(HIWORD(value)), iBVOLp, text);
						m_tbl2->SetItemText(LOBYTE(HIWORD(value)), iBCHG,  !atof(syms) ? _T("") : syms);

// 						strTemp.Format("Alert[매수] : value = %d, text = %s, 변동량 = %s", value, text, syms);
// 						OutputDebugString(strTemp);
					}
					break;
				case 0xff:
				//	m_schg->SetText(text);
					break;
				case 0xfe:
					m_svol->SetText(text);
					syms.Format("%.f", atof(text) - atof(m_fsvol));
					m_fsvol = text;
					m_schg->SetText(!atof(syms) ? _T("") : syms);
					break;
				case 0xfd:
					m_bvol->SetText(text);
					syms.Format("%.f", atof(text) - atof(m_fbvol));
					m_fbvol = text;
					m_bchg->SetText(!atof(syms) ? _T("") : syms);
					break;
				case 0xfc:
				//	m_bchg->SetText(text);
				default:
					break;
				}
			}
		}
		m_symbol->SendMessage(WM_USER, wParam, (LPARAM)data.operator LPCTSTR());
//		return;
	}

	//2011.11.18 KSJ
	text = strSourceText;

// 	strTemp.Format("Alert[text.CompareNoCase][%d] : text = %s, m_keys = %s, Data = %s", !text.CompareNoCase(m_keys), text, m_keys, data);
// 	OutputDebugString(strTemp);
	
	//20112.01.31 KSJ
	// 179 : 외국계 순매수, 168 : 매수직전대비, 124 : 매도증권사4, 125 : 매도증권사5 등이 들어가 있을때만 조회한다.
	if (!text.IsEmpty() && !m_keys.IsEmpty() && text.Find(m_keys) == 0 && text.Find("179") > 0 && text.Find("168") > 0 && text.Find("124") > 0 && text.Find("125") > 0)
	{
// 		strTemp.Format("AlertIN[text.CompareNoCase][%d] : text = %s, m_keys = %s, Data = %s", !text.CompareNoCase(m_keys), text, m_keys, data);
// 		OutputDebugString(strTemp);

		//2011.11.18 KSJ
		//자동버튼이 눌러졌을 때는 다시 조회한다.

//KSJ 수정 시작
		if(!m_stop)
		{
// 			OutputDebugString("[Alert]");
			SendMessage(WM_COMMAND, MAKEWPARAM(IDC_INQ, 0), BN_CLICKED);
			return;
		}

// 		strTemp.Format("Alert[%d] : text = %s, m_keys = %s", text.Find(m_keys), text, m_keys);
// 		OutputDebugString(strTemp);
// 
// 		int	index;
//  	DWORD	attr;
// 		CString	syms;
// 		CMapStringToString maps;
// 		bool	sort = false;
// 
// 		index = -1;
// 		if (parse(data, maps) <= 0)
// 			return;
// 
// 		if (maps.Lookup("192", text) && text.CompareNoCase(m_keys.Mid(1)))
// 			return;
// 
// 		if (maps.Lookup("190", text))
// 		{
// 			for (int ii = 1; ii < m_rows; ii++)
// 			{
// 				syms = m_grid->GetItemText(ii, 0);
// 				if (syms.IsEmpty())
// 					return;
// 				if (atoi(syms) == atoi(text))
// 				{
// 					index = ii;
// 					break;
// 				}
// 			}
// 			if (index < 0)
// 			{
// 				index = m_rows;
// 				m_rows++;
// 				m_grid->SetItemText(index, iCODE, text);
// 			}
// 		}
// 		if (maps.Lookup("191", text))
// 		{
// 			if (text.Find('+') == 0)
// 				text = text.Mid(1);
// 			m_grid->SetItemText(index, iNAME, text);
// 		}
// 		if (maps.Lookup("194", text))
// 		{
// 			text = text.Left(4);
// 			text.Insert(2, ':');
// 			m_grid->SetItemText(index, iTIME, text);
// 		}
// 		if (maps.Lookup("198", text))
// 			m_grid->SetItemText(index, iBVOLx, text);
// 		if (maps.Lookup("199", text))
// 			m_grid->SetItemText(index, iSVOLx, text);
// 		if (maps.Lookup("200", text) && text.CompareNoCase(m_grid->GetItemText(index, iBBVL)))
// 		{
// 			m_grid->SetItemText(index, iBBVL, text);
// 			syms = m_grid->GetItemText(index, iBBVLp);
// 			syms.Format("%.f", atof(text) - atof(syms));
// 			m_grid->SetItemText(index, iBBVLp, text);
// 			m_grid->SetItemText(index, iCHAG,  syms);
// 			if (m_sort == iBBVL)
// 				sort = true;
// 
// 			strTemp.Format("Alert[200] : text = %s, 변동량 = %s", text, syms);
// 			OutputDebugString(strTemp);
// 
// 		}
// 	//	if (maps.Lookup("201", text))
// 	//		m_grid->SetItemText(index, iCHAG, text);
// 		if (maps.Lookup("202", text))
// 		{
// 			text = setPercent(text);
// 			if (text.CompareNoCase(m_grid->GetItemText(index, iRATE)))
// 			{
// 				m_grid->SetItemText(index, iRATE, text);
// 				if (m_sort == iRATE)
// 					sort = true;
// 			}
// 		}
// 
// 		DWORD	attr;
//  		bool	sort = false;
// 		if (sort)
// 		{
// 			m_sort = (m_sort > 0) ? m_sort : iBBVL;
// 			attr = m_grid->GetItemAttr(0, m_sort);
// 			m_grid->SetItemAttr(0, m_sort, attr|GVAT_SORTVAL);
// 			m_grid->Sort(m_sort, !m_descend);
// 			m_grid->Invalidate(FALSE);
// 			m_grid->SetItemAttr(0, m_sort, attr);
// 		}

//KSJ 수정 완료

	}
}

// 2012.02.15 KSJ alertX 추가
//2016.07.01 KSJ 실시간 처리부분 수정
void CMapWnd::alertx(WPARAM wParam, LPARAM lParam)
{
// 	OutputDebugString("[KSJ] alertX");
	CString strTemp;

//	int	pos;
	CString	text, strSourceText, strCode, strGubn;

	struct _alertR* alertR;
	alertR = (struct _alertR*)lParam;

	DWORD* data;

	data = (DWORD*) alertR->ptr[0];

	strCode = alertR->code.GetAt(0) == 'A' ? alertR->code.Right(alertR->code.GetLength() -1) : alertR->code;
	
// 	strTemp.Format("[KSJ] m_code[%s], Size[%d], Code[%s], Data[%s], strCode[%s]", m_code->GetEditData(), alertR->size, alertR->code, data[0], strCode);
// 	OutputDebugString(strTemp);

	if(m_code->GetEditData().Compare(strCode))	return;

	for (int ii = alertR->size - 1; ii > -1; --ii)
	{
		int	value=0;
		CString	strSyms, string, strRts;

		data = (DWORD*) alertR->ptr[ii];

		strGubn = (char*)data[0];

		if(strGubn.Compare("F"))	return;

		for (int jj = 0; jj < L_syms; jj++)
		{
			strRts.Format("%s", syms[jj].rts);
			CString strData = (char*)data[atoi(strRts)];

			if (strRts.GetLength() != 0 && data[atoi(strRts)] && m_rts.Lookup(strRts, (void *&)value))
			{
				text = (char*)data[atoi(strRts)];

				switch (LOWORD(value))
				{
				case 0:
					m_tbl1->SetItemText(LOBYTE(HIWORD(value)), HIBYTE(HIWORD(value)), text);
					if (HIBYTE(HIWORD(value)) == iSVOL)
					{
						strSyms = m_tbl1->GetItemText(LOBYTE(HIWORD(value)), iSVOLp);
						strSyms.Format("%.f", atof(text) - atof(strSyms));
						m_tbl1->SetItemText(LOBYTE(HIWORD(value)), iSVOLp, text);
						m_tbl1->SetItemText(LOBYTE(HIWORD(value)), iSCHG,  !atof(strSyms) ? _T("") : strSyms);
						
// 						strTemp.Format("[KSJ] Alert[매도] : strRts = %s value = %d, text = %s, 변동량 = %s", strRts, value, text, strSyms);
// 						OutputDebugString(strTemp);
					}
					break;
				case 1:
					m_tbl2->SetItemText(LOBYTE(HIWORD(value)), HIBYTE(HIWORD(value)), text);
					if (HIBYTE(HIWORD(value)) == iBVOL)
					{
						strSyms = m_tbl2->GetItemText(LOBYTE(HIWORD(value)), iBVOLp);
						strSyms.Format("%.f", atof(text) - atof(strSyms));
						m_tbl2->SetItemText(LOBYTE(HIWORD(value)), iBVOLp, text);
						m_tbl2->SetItemText(LOBYTE(HIWORD(value)), iBCHG,  !atof(strSyms) ? _T("") : strSyms);
						
// 						strTemp.Format("[KSJ] Alert[매수] : strRts = %s value = %d, text = %s, 변동량 = %s", strRts, value, text, strSyms);
// 						OutputDebugString(strTemp);
					}
					break;
				case 0xff:
					//	m_schg->SetText(text);
					break;
				case 0xfe:
					m_svol->SetText(text);
					strSyms.Format("%.f", atof(text) - atof(m_fsvol));
					m_fsvol = text;
					m_schg->SetText(!atof(strSyms) ? _T("") : strSyms);
					break;
				case 0xfd:
					m_bvol->SetText(text);
					strSyms.Format("%.f", atof(text) - atof(m_fbvol));
					m_fbvol = text;
					m_bchg->SetText(!atof(strSyms) ? _T("") : strSyms);
					break;
				case 0xfc:
					//	m_bchg->SetText(text);
				default:
					break;
				}
			}
		}

		//20112.01.31 KSJ
		// 179 : 외국계 순매수, 168 : 매수직전대비, 124 : 매도증권사4, 125 : 매도증권사5 등이 들어가 있을때만 조회한다.
		if (!m_keys.IsEmpty() && data[179] && data[168] && data[124] && data[125])
		{
			//2011.11.18 KSJ
			//자동버튼이 눌러졌을 때는 다시 조회한다.
			
			//KSJ 수정 시작
			if(!m_stop)
			{
				SendMessage(WM_COMMAND, MAKEWPARAM(IDC_INQ, 1212), BN_CLICKED);
			}
		}
	}
}
// KSJ 2012.02.16 alertX 추가

void CMapWnd::inquiry(bool trigger)
{
	CString	data, strTemp;

	data.Empty();
	if (m_code)
	{
		data = m_code->GetEditData();
		data.TrimLeft(); data.TrimRight();
		if (!data.IsEmpty() && data.GetLength() == 6 && variant(whichCC, data))
		{
			data.Insert(0, "1301\t");
			variant(codeCC, data);
			//2011.11.24 KSJ
			//trigger 보내는것 때문에 위치이동하다가 제자리로 돌아옴
// 			strTemp.Format("trigger = %s, bFirst = %s", trigger ? "TRUE" : "FASLE", m_bFirst ? "TRUE" : "FASLE");
// 			OutputDebugString(strTemp);
			if (trigger)
			{
				m_bFirst = false;
// 				OutputDebugString("+++++++++++triggerCC+++++++++++");
				variant(triggerCC, data);
			}

// 			OutputDebugString("[KSJ]inquiry data[" + data + "]");

		}
		else	m_code->SetEditData(_T(""));
	}
	sendTR();
}

int CMapWnd::parse(CString text, CStringArray& arr)
{
	int	pos{};

	arr.RemoveAll();
	for ( ; !text.IsEmpty(); )
	{
		pos = text.Find('\t');
		if (pos == -1)
		{
			arr.Add(text);
			text.Empty();
		}
		else
		{
			arr.Add(text.Left(pos++));
			text = text.Mid(pos);
		}
	}
	return arr.GetSize();
}

int CMapWnd::parse(CString text, CMapStringToString& maps)
{
	int	pos{};
	CString	str;

	maps.RemoveAll();
	for ( ; !text.IsEmpty(); )
	{
		pos = text.Find('\t');
		if (pos != -1)
		{
			str  = text.Left(pos++);
			text = text.Mid(pos);
		}
		else	break;
		pos = text.Find('\t');
		if (pos != -1)
		{
			maps.SetAt(str, text.Left(pos++));
			text = text.Mid(pos);
		}
		else
		{
			maps.SetAt(str, text);
			text.Empty();
		}
	}

	return maps.GetCount();
}

CString CMapWnd::setPercent(CString data)
{
	CString	text;

	data.TrimLeft();
	data.TrimRight();
	text.Format("%s%%", data);
	return text;
}

void CMapWnd::doWait(BOOL wait)
{
	m_code->EnableWindow(!wait);
	m_searchBN->EnableWindow(!wait);
	m_inqBN->EnableWindow(!wait);
	m_stopBN->EnableWindow(!wait);
}

void CMapWnd::setStaticColor(CfxStatic* pWnd, COLORREF bgclr, COLORREF fgclr, COLORREF bdclr)
{
	pWnd->SetBkColor(bgclr, false);
	pWnd->SetFgColor(fgclr, false);
	pWnd->SetBorderColor(bdclr, false);
	pWnd->Invalidate();
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
		name = _T("굴림체");
	fontR.name   = (char *)name.operator LPCTSTR();
	fontR.point  = point;
	fontR.italic = italic;
	fontR.bold   = bold;

	return (CFont *)m_parent->SendMessage(WM_USER, MAKEWPARAM(getFONT, 0), (LPARAM)&fontR);
}

HBITMAP CMapWnd::getBitmap(CString bmps)
{
	CString	path;

	path.Format("%s\\%s\\%s", (char *)variant(homeCC), IMAGEDIR, bmps);
	return ((CBitmap *)m_parent->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, 0), (LPARAM)(LPCTSTR)path))->operator HBITMAP();
}

long CMapWnd::variant(int key, CString data)
{
	return m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, key), (LPARAM)(LPCTSTR)data);
}

void CMapWnd::openView(CString maps, CString data)
{
	CString	param;

	param.Format("%s/t/s/p5/d%s", maps, data);
	m_parent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, 0x01), (LPARAM)(LPCTSTR)param);
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);

	dc.FillSolidRect(&rcClient, getColor(clBack));
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

CString CMapWnd::Parser(CString& srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}
