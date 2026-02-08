// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB210200.h"
#include "MapWnd.h"
#include "setup.h"
#include "misc.h"
#include "item.hxx"

#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include "../../control/fx_misc/misctype.h"

#include "../H/interMSG.h"

#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define	F_POINT		9
#define	S_TITLE		"마켓레이더"
#define	S_STOP		"멈춤"
#define	S_RUN		"실행"
#define	S_INQ		"조회"
#define	S_SETUP		"설정"
#define	S_NEXT		"다음"
#define	S_FONT		"굴림체"
#define	S_FILE		"u_config.ini"

#define MAP_CONFIG	"IB200100"
#define MK_OPENSCR		13	// 화면 열기
/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd* parent)
{
	m_parent = parent;

	m_panel		= NULL;
	m_panel2	= NULL;
	m_code		= NULL;
	m_searchBN	= NULL;
	m_stopBN	= NULL;
	m_inqBN		= NULL;
	m_setupBN	= NULL;
	m_nextBN	= NULL;
	m_grid		= NULL;

	m_symbol	= NULL;
	m_inst		= NULL;

	m_string.Empty();

	m_stop = false;
	m_rows = 1;

	m_nFontSize = F_POINT;

	m_next = false;
	m_file.Empty();
	m_rts.Empty();
	for (int ii = 0; ii < 4; ii++)
		m_flags[ii] = 0;
	m_state.Empty();
	m_keys.Empty();
	m_sound.Empty();
	m_items.RemoveAll();

	m_chkVol = FALSE;
	m_volS.Empty();
	m_volVal = 0.0;
	m_gubnS.Empty();
	m_font = nullptr;

	m_bCreated = FALSE;
}

CMapWnd::~CMapWnd()
{
	if (m_panel)	m_panel = nullptr;
	if (m_panel2)	m_panel2 = nullptr;
	if (m_code) 	m_code = nullptr;
	if (m_searchBN)	m_searchBN = nullptr;
	if (m_stopBN)	m_stopBN = nullptr;
	if (m_inqBN)	m_inqBN = nullptr;
	if (m_setupBN)	m_setupBN = nullptr;
	if (m_nextBN)	m_nextBN = nullptr;
	if (m_grid)		m_grid = nullptr;

	if (m_symbol)
	{
		if (IsWindow(m_symbol->GetSafeHwnd()))
			m_symbol->SendMessage(WM_CLOSE);
		m_symbol->Detach();
		delete m_symbol;
	}
	if (m_inst)
		AfxFreeLibrary(m_inst);
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
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

	m_file.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, S_FILE);
	m_font = getFont(F_POINT, S_FONT);
	variant(titleCC, S_TITLE);

	m_clrBack  = getColor(clBack);
	m_clrBackIn  = getColor(clPanel);

	loadConfig();
	m_parent->SendMessage(WM_USER, MAKEWPARAM(dropDLL, 0));

	return create(m_parent);
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

//	CRect	rc;
//	GetClientRect(&rc);
//	dc.FillSolidRect(rc, m_clrBack);

//	CRect panelRc;
//	m_panel->GetClientRect(panelRc);
//	rc.SetRect(rc.left + GAP_SIDE, panelRc.bottom + GAP_BLOCK + GAP_TOP,
//			rc.right - GAP_SIDE, rc.bottom - GAP_BOTTOM);
//	dc.FillSolidRect(rc, getColor(clContents));
//	DrawRoundBitmap(&dc, rc, ROUND_CONTENTS);
	
	return;
	//CRect clipRC;
	//dc.GetClipBox(&clipRC);
	//dc.FillSolidRect(clipRC, m_clrBack);
}


void CMapWnd::OnDestroy() 
{
//	DWORD	key=0;
	Citem*	item=nullptr;

	m_bCreated = FALSE;

	//for (POSITION pos = m_items.GetStartPosition(); pos; )
	//{
	//	m_items.GetNextAssoc(pos, key, item);
	//	delete item;
	//}
	m_items.RemoveAll();
	_vData.clear();

	CWnd::OnDestroy();
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
			inquiry(true);
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
				inquiry(true);
			}
		}
		break;

	case IDC_STOP:
		m_stop = !m_stop;
		m_stopBN->SetWindowText(m_stop ? S_RUN : S_STOP);
		if (!m_stop)
		{
			text = "";
			text = m_code->GetEditData();
			if (text.IsEmpty())
				inquiry(false, false);
			else
				inquiry(true, false);
		}
		break;

	case IDC_INQ:
		text = "";
		text = m_code->GetEditData();
		if (text.IsEmpty())
			inquiry(false);
		else
			inquiry(true);
		break;

	case IDC_SETUP:
		setup();
		break;

	case IDC_NEXT:
		next();
		break;
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
		CString	param;

		switch (lpNMH->hdr.code)
		{
		case GVNM_LMOUSEDOWN:
			if (lpNMH->row > 0 && lpNMH->col == iNAME && !m_grid->GetItemText(lpNMH->row, iCODE).IsEmpty())
			{
				CString titleCode = m_code->GetEditData();
				if (!titleCode.IsEmpty())
					break;
				CString	string;
				string.Format("1301\t%s", m_grid->GetItemText(lpNMH->row, iCODE));

				//선택된 표시
				m_grid->SetSelectRow(lpNMH->row);
				m_grid->SetFocus();
				variant(triggerCC, string);			// 20071026
				m_parent->SendMessage(WM_USER, MAKEWPARAM(dragDLL, string.GetLength()), (LPARAM)(char *)string.operator LPCTSTR());
			}
			break;

			if (lpNMH->row > 0 && (lpNMH->col == iCODE || lpNMH->col == iNAME))
			{
				text = m_grid->GetItemText(lpNMH->row, iCODE);
				if (!text.IsEmpty())
				{
					CString	string;
					
					string.Format("1301\t%s", text);
					m_parent->SendMessage(WM_USER, MAKEWPARAM(dragDLL, string.GetLength()),
						(LPARAM)(char *)string.operator LPCTSTR());
				}
			}
			break;

		case GVNM_DBLCLICKED:
// 			if (lpNMH->col == iDESC && !m_grid->GetItemText(lpNMH->row, iCODE).IsEmpty()
// 						&& !m_grid->GetItemText(lpNMH->row, iMAPS).IsEmpty())
// 				openView(m_grid->GetItemText(lpNMH->row, iCODE), m_grid->GetItemText(lpNMH->row, iMAPS));
//			m_parent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, 0x01), (LPARAM)(LPCTSTR)param);

			//창 띄우기
			m_grid->SetFocus();
 			param.Format("%s /p5 /S/d1301\t%s", MAP_CONFIG, m_grid->GetItemText(lpNMH->row, iCODE));
			m_parent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, 0x01), (LPARAM)(LPCTSTR)param);

			break;

		case GVNM_RMOUSEDOWN:
			GetCursorPos(&pt);
			text = m_grid->GetItemText(lpNMH->row, iCODE);
			if (text.IsEmpty())
				m_string.Format("IB210200\n%d\t%d\n", pt.x, pt.y);
			else
				m_string.Format("IB210200\n%d\t%d\n1301\t%s\n", pt.x, pt.y, text);
			m_parent->PostMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (long)m_string.operator LPCTSTR());
			break;

		case GVNM_UPVSCROLL:
			if (m_rows >= nROW)
				inquiry(false);
			break;

		case GVNM_DNVSCROLL:
			if (m_nextBN->IsWindowEnabled())
				next();
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
		m_sync.Lock();
		dispatch(HIBYTE(LOWORD(wParam)), (char *)lParam, HIWORD(wParam));
		m_sync.Unlock();
		break;

	case DLL_ALERT:
		if (HIWORD(wParam) == alert_DEIN || m_stop)
			break;
		m_sync.Lock();
		alert((char *)lParam, wParam);
		m_sync.Unlock();
		break;

	case DLL_SETPAL:
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
		m_symbol->SendMessage(WM_USER, MAKEWPARAM(DLL_SETPAL, 0));
		m_clrBack = getColor(clBack);
		Invalidate();
		break;

	case DLL_DOMINO:
		if (!HIWORD(wParam))
		{
			int	pos=0;
			CString	sym, data;

			data = (char *)lParam;
			for ( ; !data.IsEmpty(); )
			{
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
				if (!sym.CompareNoCase("interest"))
				{
					data.TrimRight();
					if (m_code)
					{
						m_code->SetEditData(data);
						inquiry(true);
					}
					break;
				}
			}
		}
		break;

	case DLL_TRIGGER:
/*		if (!HIWORD(wParam))
		{
			int	pos;
			CString	sym, data;

			data = (char *)lParam;
			for ( ; !data.IsEmpty(); )
			{
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
							inquiry(true, false);
						}
					}
					break;
				}
			}
		}*/
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
					inquiry(true, false);
				}
			}
		}
		break;

	case DLL_SETFONTx:
		m_nFontSize = (int)HIWORD(wParam);
		m_font = getFont(m_nFontSize, "굴림체");
		SetResize(m_nFontSize);
		break;
	}
	

	return 0;
}

BOOL CMapWnd::create(CWnd* parent)
{
	if (!CWnd::Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, MAP_RECT, parent, 100))
		return FALSE;

	CreateControl();
	
	LoadBuffer();

	if(m_nFontSize != F_POINT)
		SetResize(m_nFontSize);

	sendTR(true);
	m_bCreated = TRUE;

	return TRUE;
}

void CMapWnd::sendTR(bool all)
{
	struct	_inR	inR {};
	CString	tmps, code;

	doWait(TRUE);

	if (!m_next)
	{
		m_rows = 1;
		m_grid->Clear();

// 그리드 사이즈를 변경하도록 수정하여 고정값을 제거(2009.6.1)
//		m_grid->SetRowCount(nROWx);
//		for (int ii = 0; ii < nROWx; ii++)
//			m_grid->SetRowHeight(ii, m_grid->GetRowHeight(0));
	}
	else	
		m_rows = (m_rows < m_grid->GetRowCount()) ? m_rows : m_grid->GetRowCount();
//		m_rows = (m_rows < nROWx) ? m_rows : m_grid->GetRowCount();

	FillMemory(&inR, L_inR, ' ');
	code = m_code->GetEditData();
	if (!code.IsEmpty())
		CopyMemory(inR.code, code, min(code.GetLength(), sizeof(inR.code)));
	tmps.Format("%10d", m_flags[0]);
	CopyMemory(inR.data[0], tmps.operator LPCTSTR(), 10);
	tmps.Format("%10d", m_flags[1]);
	CopyMemory(inR.data[1], tmps.operator LPCTSTR(), 10);
	tmps.Format("%10d", m_flags[2]);
	CopyMemory(inR.data[2], tmps.operator LPCTSTR(), 10);
	tmps.Format("%10d", m_flags[3]);
	CopyMemory(inR.data[3], tmps.operator LPCTSTR(), 10);
	if (m_next && !m_keys.IsEmpty())
		CopyMemory(inR.keys, m_keys, min(m_keys.GetLength(), sizeof(inR.keys)));
	if (!m_state.IsEmpty())
		CopyMemory(inR.stat, m_state, min(m_state.GetLength(), sizeof(inR.stat)));

	// 거래량 추가
	if (m_chkVol)
		CopyMemory(inR.gvol, m_volS, min(m_volS.GetLength(), sizeof(inR.gvol)));

	// 거래소
	CopyMemory(inR.jchk, m_gubnS, min(m_gubnS.GetLength(), sizeof(inR.jchk)));
	
	sendTR(watchTR, wKEY, (char *)&inR, L_inR);
	if (all)
	{
		tmps.Format("1301%c%s\t1021\t17413\t", 0x7f, code);
 		sendTR(oopTR, oKEY, (char *)tmps.operator LPCTSTR(), tmps.GetLength(), true);
	}
}

void CMapWnd::sendTR(CString trN, int key, char* datb, int datl, bool oop)
{
	std::unique_ptr<char[]> wb = std::make_unique<char[]>(L_userTH + datl);
	struct	_userTH* userth{};

	FillMemory(wb.get(), L_userTH+datl, ' ');
	userth = (struct _userTH *)wb.get();
	CopyMemory(userth->trc, trN, sizeof(userth->trc));
	userth->key  = key;
	userth->stat = oop ? US_OOP : 0;
	CopyMemory(&wb.get()[L_userTH], datb, datl);

	m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)wb.get());

	wb = nullptr;
}

void CMapWnd::dispatch(int key, char* datb, int datl)
{
	int	value=0;
	CString	text;

	CString stmp;
	stmp.Format("[memo][IB210200]  key =[%s]   len=[%d]\r\n", GetMemoTrKeyType(key), datl);
	OutputDebugString(stmp);

	if (key == 0 && datl > 300)
	{
		int	value;
		CString	text;

		text = CString(datb, datl);
		value = text.Find('\t');
		if (value != -1)
		{
			WPARAM	wParam{};
			CString m_rts;
			if (value == 7)
			{
				m_rts = text.Left(value++);
				text = text.Mid(value);
				wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 0), (text.GetLength() > 6) ? text.GetLength() : 0);
				m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
			}
			else
			{
				m_rts = text.Left(value++);
				//				text   = text.Mid(value);
				wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, 0), (text.GetLength() > 6) ? text.GetLength() : 0);
				m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
			}
		}
		return;
	}

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
		break;
	}
	default:
		break;
	}

	if (key == oKEY)
	{
		text  = CString(datb, datl);
		value = text.Find('\t');
		if (value != -1)
		{
			WPARAM	wParam{};

			m_rts  = text.Left(value++);
			text   = text.Mid(value);
			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, oKEY), (text.GetLength() > 6) ? text.GetLength() : 0);
			m_symbol->SendMessage(WM_USER, wParam, (LPARAM)text.operator LPCTSTR());
		}
		return;
	}

	DWORD	flag=0;
	Citem*	item=nullptr;
	struct	_outR* outR{};
	struct	_recR* recR{};
	const bool next = m_next;
	const int start = 0;

	m_next = false;
	if (datl < L_outR)
	{
		doWait(FALSE);
		m_code->SetFocus();
		return;
	}

	outR = (struct _outR *)datb;
	recR = (struct _recR *)&datb[L_outR];
	m_keys = CString(outR->keys, sizeof(outR->keys));
	value  = atoi(CString(outR->nrec, sizeof(outR->nrec)));

	for (int row=0, ii = 0, start = m_rows; ii < value; ii++, recR++)
	{
		flag = atoi(CString(recR->data, sizeof(recR->data)));
		if (!m_items.Lookup(flag, item))
			continue;

		row  = insertRow(-1);
		text = CString(recR->code, sizeof(recR->code));
		m_grid->SetItemText(row, iCODE, text.Mid(1));
		text = CString(recR->time, sizeof(recR->time));
		text.Insert(2, ':');
		m_grid->SetItemText(row, iTIME, text);
		text = CString(recR->name, sizeof(recR->name));
		text.TrimLeft();
		m_grid->SetItemText(row, iNAME, text);
		m_grid->SetItemText(row, iDESC, CString(recR->desc, sizeof(recR->desc)));
		m_grid->SetItemFgColor(row, iDESC, item->m_clr);
		m_grid->SetItemText(row, iMAPS, item->m_maps);
		if (row >= nROW)
		{
			removeRow(0);
			start--;
		}
	}
	doWait(FALSE);
	text = m_keys;
	text.TrimLeft(); text.TrimRight();
	m_nextBN->EnableWindow(atoi(m_keys) > 0 ? TRUE : FALSE);
	m_grid->SendMessage(WM_VSCROLL, MAKEWPARAM(SB_THUMBTRACK, start), 0);
	if (next)
		m_grid->SetFocus();
	else
		m_code->SetFocus();

	const int nIncreaseHeight = INCREASE_HEIGHT * (m_nFontSize-9);
	CRect cRc;
	m_grid->GetClientRect(cRc);
	const int nRowCount = m_grid->GetRowCount();
	const int nInsertGridNum = cRc.Height() / (hROW + nIncreaseHeight) - nRowCount;
	if(nInsertGridNum < 0)
	{
		if(nRowCount > 5)
		{
			for(int i = nRowCount + nInsertGridNum; i < nRowCount; i++)
			{
				CString strRowData = m_grid->GetItemText(i, 0);
				if(strRowData.IsEmpty())
					m_grid->DeleteRow(m_grid->GetRowCount() - 1);
			}
		}
	}
}

void CMapWnd::alert(CString data, WPARAM wParam)
{
	int	pos;
	CString	text;

	pos = data.Find('\t');
	if (pos == -1)
		return;

	text = data.Left(pos++);
	data = data.Mid(pos);
	if (!text.IsEmpty() && !m_rts.IsEmpty() && !text.CompareNoCase(m_rts))
	{
		m_symbol->SendMessage(WM_USER, wParam, (LPARAM)data.operator LPCTSTR());
		return;
	}

	if (text.CompareNoCase("screen"))
		return;

//TRACE("alert => %.400s\n", data);

	for ( ; !data.IsEmpty(); )
	{
		pos = data.Find('\n');
		if (pos != -1)
		{
			text = data.Left(pos++);
			data = data.Mid(pos);
		}
		else
		{
			text = data;
			data.Empty();
		}
		parse(text);
	}

}

void CMapWnd::parse(CString data)
{
	// 거래량 추가
	int	row=0, pos=0;
	Citem*	item=nullptr;
	bool	cancel = false;
	DWORD	target=0, flag = 0;
	SCROLLINFO	sbi{};
	CString	sym, code, time, name, desc, stat, text, gubn, gvol;

	code.Empty(); time.Empty();
	name.Empty(); desc.Empty();

	for ( ; !data.IsEmpty(); )
	{
		pos = data.Find('\t');
		if (pos != -1)
		{
			sym  = data.Left(pos++);
			data = data.Mid(pos);
		}
		else	break;

		pos = data.Find('\t');
		if (pos != -1)
		{
			text = data.Left(pos++);
			data = data.Mid(pos);
		}
		else
		{
			text = data;
			data.Empty();
		}

		switch (atoi(sym))
		{
		case 21:	// 코드
			code = text;
			break;
		case 801:	// 구분
			flag = (DWORD)atoi(text);
			break;
		case 802:	// 시간
			if (text.GetLength() >= 4)
				time.Format("%s:%s", text.Left(2), text.Mid(2, 2));
			break;
		case 803:	// 종목명
			name = text;
			name.TrimRight();
			break;
		case 804:	// 특이사항
			desc = text;
			break;
		case 805:	// 대상제외
			stat = text.Left(min(8, text.GetLength()));
			break;
		case 806:	// 거래소구분
			gubn = text;
			break;
		case 807:	// 거래량
			gvol = text;
			break;
		}
	}

	text = m_code->GetEditData();
	if (!text.IsEmpty() && code.CompareNoCase("A"+text))
		return;

	if (flag & S_CANL)
	{
		cancel = true;
		flag &= ~S_CANL;
	}
	if (flag & S_NEWS)
		target = m_flags[0];
	else if (flag & S_QUOTE)
		target = m_flags[1];
	else if (flag & S_TREND)
		target = m_flags[2];
	else if (flag & S_CHART)
		target = m_flags[3];
	if ((target & flag) != flag || !m_items.Lookup(flag, item))
		return;

	// 거래소
	if (!checkGubn(gubn))
		return;

	// 제외종목
	for (int ii = 0; ii < m_state.GetLength(); ii++)
	{
		if (ii >= stat.GetLength())
			break;
		if (m_state.GetAt(ii) == '0')
			continue;
		if (stat.GetAt(ii) == '1')
			return;
	}

	// 거래량
	if (m_chkVol)
	{
		if (atof(gvol) < m_volVal)
			return;
		//TRACE("code[%s] vol[%s]\n", code, gvol);
	}

	
	m_grid->GetScrollInfo(SB_VERT, &sbi);
	if (m_rows >= nROW)
	{
		if (sbi.nPos != 0 && sbi.nPos < nROW)
			return;
		removeRow(m_rows-1);
	}

	row = insertRow(1);
	m_grid->SetItemText(row, iCODE, code.Mid(1));
	m_grid->SetItemText(row, iTIME, time);
	m_grid->SetItemText(row, iNAME, name);
	m_grid->SetItemText(row, iDESC, desc);
	m_grid->SetItemFgColor(row, iDESC, item->m_clr);
	m_grid->SetItemText(row, iMAPS, item->m_maps);
	if (cancel)
	{
		LOGFONT* lf;

		lf = m_grid->GetItemFont(row, iNAME);
		lf->lfStrikeOut = true;
		m_grid->SetItemFont(row, iNAME, lf);
	}
	m_grid->Refresh();
}

void CMapWnd::inquiry(bool all, bool trigger)
{
	CString	data;

	data.Empty();
	if (m_code)
	{
		data = m_code->GetEditData();
		data.TrimLeft(); data.TrimRight();
		SaveBuffer(JONGMUK_CODE, data);

		if (!data.IsEmpty() && data.GetLength() == 6 && variant(whichCC, data))
		{
			data.Insert(0, "1301\t");
			variant(codeCC, data);
			if (trigger)
				variant(triggerCC, data);
		}
		else	
			m_code->SetEditData(_T(""));
	}
	sendTR(all);
}

void CMapWnd::setup()
{
	std::unique_ptr<Csetup>	setup = std::make_unique<Csetup>(nullptr, m_root, m_user, m_parent);
	if (setup->DoModal() == IDOK)
	{
		loadConfig();
		sendTR();
	}
	setup = nullptr;
}

void CMapWnd::next()
{
	CString	data;

	if (m_code)
	{
		data = m_code->GetEditData();
		data.TrimLeft(); data.TrimRight();
		if (!data.IsEmpty())
		{
			data.Insert(0, "1301\t");
			variant(codeCC, data);
		}
	}
	m_next = true;
	sendTR();
}

void CMapWnd::loadConfig()
{
	char	wb[2048]{};
	Citem*	item=nullptr;
	CString	keys, text, str, data;

	GetPrivateProfileString(_T("2102"), "news",  "0x01ffffff", wb, sizeof(wb), m_file);
	m_flags[0] = hexs2int(wb);
	GetPrivateProfileString(_T("2102"), "quote", "0x02ffffff", wb, sizeof(wb), m_file);
	m_flags[1] = hexs2int(wb);
	GetPrivateProfileString(_T("2102"), "trend", "0x04ffffff", wb, sizeof(wb), m_file);
	m_flags[2] = hexs2int(wb);
	GetPrivateProfileString(_T("2102"), "chart", "0x08ffffff", wb, sizeof(wb), m_file);
	m_flags[3] = hexs2int(wb);

	// load informations
	int	pos=0;
	DWORD key=0;

	for (POSITION p = m_items.GetStartPosition(); p; )
	{
		m_items.GetNextAssoc(p, key, item);
		delete item;
	}

	m_items.RemoveAll();

	const DWORD	rc = GetPrivateProfileSection(_T("2102"), wb, sizeof(wb), m_file);
	if (rc > 0)
	{
		str = CString(wb, rc);
		for ( ; !str.IsEmpty(); )
		{
			pos = str.Find('\0');
			if (pos != -1)
			{
				text = str.Left(pos++);
				str  = str.Mid(pos);
			}
			else
			{
				text = str;
				str.Empty();
			}

			pos = text.Find('=');
			if (pos != -1)
			{
				keys = text.Left(pos++);
				text = text.Mid(pos);
			}
			else	continue;

			keys.TrimLeft(); keys.TrimRight();
			text.TrimLeft(); text.TrimRight();
			if (keys.IsEmpty() || text.IsEmpty())
				continue;

			pos = text.Find(',');
			if (pos != -1)
			{
				data = text.Left(pos++);
				text = text.Mid(pos);

				data.TrimRight();
				text.TrimLeft();

				item = new Citem;
				item->m_flag = hexs2int(keys);
				item->m_clr  = hexs2int(data);
				item->m_maps = text;
				m_items.SetAt(item->m_flag, item);
			}
		}

	}
	else
	{
		struct	_infoR
		{
			DWORD	flag;
			DWORD	color;
			char*	maps;
		};
		const struct _infoR infoR[] = {
			{ S_NEWS|F_NEWS,	RGB(  0,   0,   0),	"IB210200"	},
			{ S_NEWS|F_GONG,	RGB(  0,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_JUN_KOGA,	RGB(255,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_JUN_GVOL,	RGB(255,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_DAN_SHGA,	RGB(255,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_DAN_HHGA,	RGB(  0,   0, 255),	"IB210200"	},
			{ S_QUOTE|F_MAMUL_5D,	RGB(255,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_MAMUL_60D,	RGB(255,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_SHGA_5D,	RGB(255,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_GVOL_52M,	RGB(255,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_GVOL_5D,	RGB(255,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_3D_UP_15P,	RGB(  0,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_5D_UP_20P,	RGB(  0,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_KOGA_5D,	RGB(255,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_JEGA_5D,	RGB(  0,   0, 255),	"IB210200"	},
			{ S_QUOTE|F_UPDN_5P,	RGB(  0,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_SIGA_5P,	RGB(  0,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_UPXX_3D,	RGB(255,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_DOWN_3D,	RGB(  0,   0, 255),	"IB210200"	},
			{ S_QUOTE|F_MASU_200P,	RGB(255,   0,   0),	"IB210200"	},
			{ S_QUOTE|F_RATE_20P,	RGB(  0,   0,   0),	"IB210200"	},
			{ S_TREND|F_WMASU,	RGB(255,   0,   0),	"IB210200"	},
			{ S_TREND|F_WMADO,	RGB(  0,   0, 255),	"IB210200"	},
			{ S_TREND|F_WMASU_3D,	RGB(255,   0,   0),	"IB210200"	},
			{ S_TREND|F_WMADO_3D,	RGB(  0,   0, 255),	"IB210200"	},
			{ S_TREND|F_TMASU_1,	RGB(255,   0,   0),	"IB210200"	},
			{ S_TREND|F_TMADO_1,	RGB(  0,   0, 255),	"IB210200"	},
			{ S_TREND|F_TMASU_F,	RGB(255,   0,   0),	"IB210200"	},
			{ S_TREND|F_TMADO_F,	RGB(  0,   0, 255),	"IB210200"	},
			{ S_CHART|F_JUGA_GC,	RGB(255,   0,   0),	"IB210200"	},
			{ S_CHART|F_JUGA_DC,	RGB(  0,   0, 255),	"IB210200"	},
			{ S_CHART|F_GVOL_GC,	RGB(255,   0,   0),	"IB210200"	},
			{ S_CHART|F_GVOL_DC,	RGB(  0,   0, 255),	"IB210200"	},
			{ S_CHART|F_3SUN_UP,	RGB(255,   0,   0),	"IB210200"	},
			{ S_CHART|F_3SUN_DN,	RGB(  0,   0, 255),	"IB210200"	},
			{ S_CHART|F_5MAJ_UP,	RGB(255,   0,   0),	"IB210200"	},
			{ S_CHART|F_5MAJ_DN,	RGB(  0,   0, 255),	"IB210200"	},
			{ S_CHART|F_5MAG_UP,	RGB(255,   0,   0),	"IB210200"	},
			{ 0,			0,			""		},
		};

		for (int ii = 0; infoR[ii].flag; ii++)
		{
//			item = new Citem;
			auto& item = _vData.emplace_back(std::make_unique<Citem>());

			item->m_flag = infoR[ii].flag;
			item->m_clr  = infoR[ii].color;
			item->m_maps = infoR[ii].maps;
			m_items.SetAt(item->m_flag, item.get());
		}
	}

	GetPrivateProfileString(_T("2102"), "item", "00000000", wb, sizeof(wb), m_file);
	m_state = wb;
	m_state += CString('0', 8-m_state.GetLength());

	GetPrivateProfileString(_T("2102"), "beep", "", wb, sizeof(wb), m_file);
	m_sound = wb;

	// 거래량
	m_chkVol = GetPrivateProfileInt(_T("2102"), "chkVol", 0, m_file);
	GetPrivateProfileString(_T("2102"), "vol", "0", wb, sizeof(wb), m_file);
	m_volS = wb;
	if (m_chkVol)
		m_volVal = atof(m_volS) * 1000.0;
	else 
		m_chkVol = 0;

	// 거래소
	GetPrivateProfileString(_T("2102"), "gubn", "11100000", wb, sizeof(wb), m_file);
	m_gubnS = wb;
}

int CMapWnd::hexs2int(CString hexs)
{
	struct	_hexmap {
		char	ch;
		int	value;
	} hexmap [] = {
		{ '0', 0 },	{ '1', 1 },	{ '2', 2 },	{ '3', 3 },	{ '4', 4 },
		{ '5', 5 },	{ '6', 6 },	{ '7', 7 },	{ '8', 8 },	{ '9', 9 },
		{ 'a', 10 },	{ 'b', 11 },	{ 'c', 12 },	{ 'd', 13 },	{ 'e', 14 },
		{ 'f', 15 },	{ NULL, NULL }
	};
	bool	first = true;
	int	value=0, result = 0;
	CMap	< TCHAR, TCHAR, int, int& > hmap;

	hexs.MakeLower();
	if (hexs.Find("0x") == 0)
		hexs = hexs.Mid(2);

	for (int ii = 0; hexmap[ii].ch != NULL; ii++)
		hmap.SetAt(hexmap[ii].ch, hexmap[ii].value);

	for (int ii = 0; ii < hexs.GetLength(); ii++)
	{
		if (hmap.Lookup(hexs.GetAt(ii), value))
		{
			if (!first)
				result <<= 4;
			result |= value;
			first = false;
			continue;
		}
		break;
	}

	return result;
}

int CMapWnd::insertRow(int row)
{
	GVITEM	item{};
	LOGFONT	lf{};

//	if (row < 0 && m_rows < m_grid->GetRowCount())
	if (row < 0 && m_rows < m_grid->GetRowCount())
		row = m_rows;
	row = m_grid->InsertRow(_T(""), row);
	if (row < 0)
		return -1;
	m_rows++;
	if (m_rows < m_grid->GetRowCount())
//	if (m_rows <= nROWx)
		removeRow(m_grid->GetRowCount()-1);
	lf.lfHeight         = m_nFontSize * 10;
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
	item.row = row;
	for (int ii = 0; ii < nCOL; ii++)
	{
		item.col    = ii;
		item.mask   = GVMK_FORMAT|GVMK_ATTR|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
		item.format = infos[ii].format;
		item.attr   = infos[ii].attr;
		item.fgcol  = getColor(clText);
		item.bkcol  = getColor(clData);
		CopyMemory(&item.font, &lf, sizeof(LOGFONT));
		m_grid->SetItem(&item);

//		m_grid->SetColumnWidth(ii, m_grid->GetColumnWidth(ii));
	}
	m_grid->SetRowHeight(row, m_grid->GetRowHeight(0));
	return row;
}

void CMapWnd::removeRow(int row)
{
	if (row >= 0 && row < m_grid->GetRowCount())
	{
		m_grid->DeleteRow(row);
		if (m_grid->GetRowCount() < nROWx)
		{
			GVITEM	item{};
			LOGFONT	lf{};
			int	row=0;

			row = m_grid->InsertRow(_T(""), m_grid->GetRowCount());
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
			item.row = row;
			for (int ii = 0; ii < nCOL; ii++)
			{
				item.col    = ii;
				item.mask   = GVMK_FORMAT|GVMK_ATTR|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
				item.format = infos[ii].format;
				item.attr   = infos[ii].attr;
				item.fgcol  = getColor(clText);
				item.bkcol  = getColor(clData);
				CopyMemory(&item.font, &lf, sizeof(LOGFONT));
				m_grid->SetItem(&item);
			}
			m_grid->SetRowHeight(row, m_grid->GetRowHeight(0));
		}
	}
}

void CMapWnd::doWait(BOOL wait)
{
	m_code->EnableWindow(!wait);
	m_searchBN->EnableWindow(!wait);
	m_inqBN->EnableWindow(!wait);
	m_setupBN->EnableWindow(!wait);
	m_nextBN->EnableWindow(!wait);
}

void CMapWnd::sound()
{
	if (m_sound.IsEmpty() || ::GetFileAttributes(m_sound) == 0xffffffff)
		return;

	::sndPlaySound(NULL, SND_ASYNC|SND_NODEFAULT);
	::sndPlaySound(m_sound, SND_ASYNC|SND_NODEFAULT);
}

COLORREF CMapWnd::getColor(int color)
{
	if (color & 0x02000000)
		return color;

	return m_parent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CFont* CMapWnd::getFont(int point, CString name, int bold, bool italic)
{
	struct	_fontR	fontR {};

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

	path.Format("%s\\%s\\%s", m_root, IMAGEDIR, bmps);
	return ((CBitmap *)m_parent->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, 0), (LPARAM)(LPCTSTR)path))->operator HBITMAP();
}

long CMapWnd::variant(int key, CString data)
{
	return m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, key), (LPARAM)(LPCTSTR)data);
}

void CMapWnd::openView(CString code, CString maps)
{
	CString	param;

	param.Format("%s/t/s/p5/d1301\t%s", maps, code);
	m_parent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, 0x01), (LPARAM)(LPCTSTR)param);
}

void CMapWnd::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);

	if(!m_bCreated) 
		return;
	
	if (m_code)
		m_code->SetFocus();
}

bool CMapWnd::checkGubn(CString gubn)
{
	int idx = -1;
	if (gubn.GetLength() < 1 || m_gubnS.GetLength() < 4)
		return false;

	switch (gubn.GetAt(0))
	{
	case 'P':	// kospi
		idx = 0;	break;
	case 'Q':	// kosdaq
		idx = 1;	break;
	case '3':	// 프리보드
		idx = 2;	break;
	case 'E':	// ELW
		idx = 3;	break;
	}

	if (idx < 0 || idx > 3)
		return false;

	if (m_gubnS.GetAt(idx) == '1')
		return true;
	
	return false;
}


void CMapWnd::SetResize(int nFontSize)
{
	const int nSize = nFontSize - 9;	//font size 9를 100%로 지정하여 8~12까지 표현

	const CRect cWindowRc = MAP_RECT;
	const int nWidth = cWindowRc.Width() + (3 * INCREASE_WIDTH * nSize);
	const int nHeight = cWindowRc.Height() + (21 * INCREASE_HEIGHT * nSize);

	SetWindowPos(NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER | SWP_NOMOVE);
	m_parent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(nWidth, nHeight));
}

void CMapWnd::CreateControl()
{
	LOGFONT	lf{};
	GVITEM	item{};

	CRect	screenRect;
	CRect	topRect;
	CRect	tempRect;

	const int	BASIC_CONTROL_HIGHT = 26;
	const int	CODECTL_SZ = 79;
	const int	REACHBTN_SZ = 19;
	const int	BTN_SZ = 32;
	const int	SYMBOL_SZ = 133;
	const int	cx = 6, cy = 3;
	const int	GAP = 5;
	const int	smallGAP = 6;
	const int	tinyGAP = 1;
	const int tinyGAPButton = 1;

	GetClientRect(&screenRect);

//	for(i = 0; i < 4; i++)
//		DeleteObject(BitMap[i]);

//	m_panel의 내부 좌표
	topRect.SetRect(screenRect.left + GAP_SIDE, screenRect.top + GAP_TOP, 
					screenRect.right - GAP_SIDE, screenRect.top + GAP_TOP + PANAL_HEIGHT);
	
	tempRect.SetRect(topRect.left + cx, topRect.top + cy, CODECTL_SZ + cx, topRect.bottom - cy);
	m_code = std::make_unique<CfxCodeCtrl>(this, m_root + "\\tab");
	m_code->Create(this, tempRect, IDC_CODE);
	m_code->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
	m_code->SetUnit(GU_CODE);
	m_code->SetFont(m_font);

	tempRect.SetRect(tempRect.right, tempRect.top, tempRect.right + REACHBTN_SZ, topRect.bottom - cy);
	m_searchBN = std::make_unique <CfxImgButton>();
	m_searchBN->Create(_T(""), tempRect, this, IDC_SEARCH, FALSE, TRUE);
	m_searchBN->SetImgBitmap(getBitmap("검색.bmp"), getBitmap("검색_DN.bmp"), getBitmap("검색_EN.bmp"));
	m_searchBN->SetTextColor(getColor(clBtnText));
	m_searchBN->SetFont(m_font, TRUE);

	tempRect.SetRect(tempRect.right + 3, tempRect.top, 
					tempRect.right + 3 + SYMBOL_SZ, topRect.bottom - cy);
	CString	text;
	CWnd*	(APIENTRY* axCreate)(CWnd*, void*)=nullptr;

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
			//param.rect  = CRect(104,5,264,23);
			param.rect  = tempRect;
			param.fonts = S_FONT;
			param.point = m_nFontSize;
			param.style = 1;
			param.tRGB  = 69;
			param.pRGB  = 90;
			param.options = _T("/a89/b91/c92/d69/i99/s2102");

			m_symbol = (*axCreate)(m_parent, &param);
			m_symbol->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
			if (m_symbol == NULL)
			{
				AfxFreeLibrary(m_inst);
				m_inst = NULL;
			}
		}
	}

	const int space = BUTTON_WIDTH_INQ + BUTTON_WIDTH * 3 + tinyGAPButton * 3 + GAP_CTRL + GAP_SIDE;
	tempRect.SetRect(screenRect.right - space, tempRect.top, screenRect.right - space + BUTTON_WIDTH, topRect.bottom - cy);
	m_stopBN = std::make_unique <CfxImgButton>();
	m_stopBN->Create(S_STOP, tempRect, this, IDC_STOP, FALSE, TRUE);
	m_stopBN->SetImgBitmap(getBitmap("2BTN.bmp"), getBitmap("2BTN_DN.bmp"), getBitmap("2BTN_EN.bmp"));
	m_stopBN->SetTextColor(getColor(clBtnText));
	m_stopBN->SetFont(m_font, TRUE);

	tempRect.OffsetRect(BUTTON_WIDTH + tinyGAPButton, 0);
	tempRect.right = tempRect.left + BUTTON_WIDTH_INQ;
	m_inqBN = std::make_unique <CfxImgButton>();
	m_inqBN->Create(S_INQ, tempRect, this, IDC_INQ, FALSE, TRUE);
	m_inqBN->SetImgBitmap(getBitmap("조회BTN.bmp"), getBitmap("조회BTN_DN.bmp"), getBitmap("조회BTN_EN.bmp"));
	m_inqBN->SetTextColor(getColor(clBtnText));
	m_inqBN->SetFont(m_font, TRUE);

	tempRect.OffsetRect(BUTTON_WIDTH_INQ + tinyGAPButton, 0);
	tempRect.right = tempRect.left + BUTTON_WIDTH;
	m_setupBN = std::make_unique<CfxImgButton>();
	m_setupBN->Create(S_SETUP, tempRect, this, IDC_SETUP, FALSE, TRUE);
	m_setupBN->SetImgBitmap(getBitmap("2BTN.bmp"), getBitmap("2BTN_DN.bmp"), getBitmap("2BTN_EN.bmp"));
	m_setupBN->SetTextColor(getColor(clBtnText));
	m_setupBN->SetFont(m_font, TRUE);

	tempRect.OffsetRect(BUTTON_WIDTH + tinyGAPButton, 0);
	m_nextBN = std::make_unique <CfxImgButton>();
	m_nextBN->Create(S_NEXT, tempRect, this, IDC_NEXT, FALSE, TRUE);
	m_nextBN->SetImgBitmap(getBitmap("2BTN.bmp"), getBitmap("2BTN_DN.bmp"), getBitmap("2BTN_EN.bmp"));
	m_nextBN->SetTextColor(getColor(clBtnText));
	m_nextBN->SetFont(m_font, TRUE);

	m_grid = std::make_unique <CfxGrid>();
	tempRect.SetRect(GAP_PANEL + GAP_SIDE, topRect.bottom + GAP_BLOCK + GAP_PANEL, 
				topRect.right - GAP_PANEL, screenRect.bottom - GAP_BOTTOM - GAP_PANEL);
	m_grid->Create(tempRect, this, IDC_GRID, GVSC_VERT, GVDD_NONE);
	m_grid->SetRowColResize(FALSE, FALSE);
	m_grid->Initial(nROWx, nCOL, 1, 0);

	lf.lfHeight         = m_nFontSize * 10;
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
		item.mask   = GVMK_FORMAT|GVMK_ATTR|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR;
		item.format = infos[ii].format;
		item.attr   = infos[ii].attr;
		item.fgcol  = getColor(clText);
		item.bkcol  = getColor(clData);
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

	for (int ii = 0; ii < nROWx; ii++)
		m_grid->SetRowHeight(ii, hROW);

	m_grid->SetStepColor(1, getColor(clRow1), getColor(clRow2));
	m_grid->SetBKSelColor(getColor(clSel));
	m_grid->SetRowColResize(FALSE, TRUE);
	m_grid->SetGridColor(getColor(clLine));
	m_grid->SetGridLine(GVLN_VERT);
	m_grid->SetGridOutline(FALSE);
	m_grid->SetGridFocusLine(FALSE, TRUE);
	m_grid->SetConditionColor(getColor(clUp), getColor(clDown), getColor(clText));
	m_grid->SetGridDIScroll(true);
	m_grid->Adjust();

	SetBitmap();

	CRect panelRc;
	panelRc.SetRect(screenRect.left, screenRect.top, screenRect.right, screenRect.top + GAP_TOP + PANAL_HEIGHT + GAP_BLOCK);
	m_panel = std::make_unique <CfxStatic>();
	m_panel->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, panelRc, this, IDC_PANEL);
	m_panel->SetBkColor(getColor(clPanel));

	int nLineTick[4];
	nLineTick[0] = GAP_SIDE;	nLineTick[1] = GAP_TOP;
	nLineTick[2] = GAP_SIDE;	nLineTick[3] = GAP_BLOCK;
	m_panel->SetCornerRound(m_hBmpRoundPanel, 5, &nLineTick[0]);
	m_panel->SetBorderColor(getColor(clBack));

	CRect panelRc2;
	panelRc2.SetRect(panelRc.left, panelRc.bottom, panelRc.right, screenRect.bottom);
	m_panel2 = std::make_unique <CfxStatic>();
	m_panel2->Create(_T(""), WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, panelRc2, this, IDC_PANEL2);
	m_panel2->SetBkColor(getColor(clContents));

	nLineTick[0] = GAP_SIDE;	nLineTick[1] = 0;
	nLineTick[2] = GAP_SIDE;	nLineTick[3] = GAP_BOTTOM;
	m_panel2->SetCornerRound(m_hBmpRoundContents, 5, &nLineTick[0]);
	m_panel2->SetBorderColor(getColor(clBack));

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

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if(m_bCreated)
		ReSize();
}

void CMapWnd::ReSize()
{
	const int nSize = m_nFontSize - 9;
	const int nIncreaseWidth = INCREASE_WIDTH * nSize;
	const int nIncreaseHeight = INCREASE_HEIGHT * nSize;

	const int	BASIC_CONTROL_HIGHT = 26;
	const int	CODECTL_SZ = 79;
	const int	REACHBTN_SZ = 19;
	const int	BTN_SZ = 32;
	const int	SYMBOL_SZ = 133;
	CRect	screenRect;
	CRect	tempRect;
	CRect	topRect;
	CRect	panelRc;
	CRect	panelRc2;
	const int	xGap = 6, YGap = 3;
	const int	smallGAP = 6 + YGap;
	const int	tinyGAP = 1;

	GetClientRect(&screenRect);

	panelRc.SetRect(screenRect.left, screenRect.top, screenRect.right, screenRect.top + GAP_TOP + PANAL_HEIGHT + GAP_BLOCK + nIncreaseHeight);
	m_panel->MoveWindow(panelRc);
	
	panelRc2.SetRect(panelRc.left, panelRc.bottom, panelRc.right, screenRect.bottom);
	m_panel2->MoveWindow(panelRc2);

//	m_panel의 내부 좌표
	topRect.SetRect(screenRect.left + GAP_SIDE, screenRect.top + GAP_TOP, screenRect.right - GAP_SIDE, screenRect.top + GAP_TOP + BASIC_CONTROL_HIGHT + nIncreaseHeight);

	tempRect.SetRect(topRect.left + xGap, topRect.top + YGap, CODECTL_SZ + xGap + nSize, topRect.bottom - YGap);
	m_code->MoveWindow(tempRect);
	m_code->SetUnit(GU_CODE);
	m_code->SetFont(m_font);

	tempRect.SetRect(tempRect.right, tempRect.top, tempRect.right + REACHBTN_SZ + nSize, topRect.bottom - YGap);
	m_searchBN->MoveWindow(tempRect);
	m_searchBN->SetFont(m_font, TRUE);

	tempRect.SetRect(tempRect.right + 3, tempRect.top, tempRect.right + 3 + SYMBOL_SZ + nIncreaseWidth, topRect.bottom - YGap);
	m_symbol->MoveWindow(tempRect);
	m_symbol->SendMessage(WM_USER, MAKEWPARAM(DLL_SETFONT, m_nFontSize), (LPARAM)_T("굴림체"));

	const int nButtonWidth = BUTTON_WIDTH + 2 * nSize;
	const int nInqSize = 1;		//기존 버튼 보다 1px 크다
	const int space = nButtonWidth * 4 + nInqSize + tinyGAP * 3 + GAP_SIDE + GAP_CTRL;

	tempRect.SetRect(screenRect.right - space, tempRect.top, 
					screenRect.right - space + nButtonWidth, topRect.bottom - YGap);
	m_stopBN->MoveWindow(tempRect);
	m_stopBN->SetFont(m_font, TRUE);	

	tempRect.OffsetRect(nButtonWidth + tinyGAP, 0);
	tempRect.right += nInqSize;
	m_inqBN->MoveWindow(tempRect);
	m_inqBN->SetFont(m_font, TRUE);

	tempRect.OffsetRect(nButtonWidth + nInqSize + tinyGAP, 0);
	tempRect.right -= nInqSize;
	m_setupBN->MoveWindow(tempRect);
	m_setupBN->SetFont(m_font, TRUE);

	tempRect.OffsetRect(nButtonWidth + tinyGAP, 0);
	m_nextBN->MoveWindow(tempRect);
	m_nextBN->SetFont(m_font, TRUE);

	tempRect.SetRect(GAP_PANEL + GAP_SIDE, topRect.bottom + GAP_BLOCK + GAP_PANEL, 
				screenRect.Width() - GAP_SIDE - GAP_PANEL, screenRect.bottom - GAP_BOTTOM - GAP_PANEL);
    m_grid->MoveWindow(tempRect);

	const int nRowCount = m_grid->GetRowCount();
	const int nInsertGridNum = (tempRect.Height()) / (hROW + nIncreaseHeight) - nRowCount;
	if(nInsertGridNum < 0)
	{
		if(nRowCount > 5)
		{
			for(int i = nRowCount + nInsertGridNum; i < nRowCount; i++)
			{
				CString strRowData = m_grid->GetItemText(i, 0);
				if(strRowData.IsEmpty())
					m_grid->DeleteRow(m_grid->GetRowCount() - 1);
			}
		}
	}
	else
	{
		for(int i = 0; i < nInsertGridNum; i++)
			m_grid->InsertRow("", m_grid->GetRowCount());
	}

	for(int i = 0; i < m_grid->GetRowCount(); i++)
	{
		for(int j = 0; j < m_grid->GetColumnCount(); j++)
		{	
			GVITEM		gvitem;
			LOGFONT* lf = m_grid->GetItemFont(i, j);
			lf->lfHeight = m_nFontSize * 10;

			CopyMemory(&gvitem.font, lf, sizeof(LOGFONT));
			m_grid->SetItemFont(i, j, lf);

			if(infos[j].width > 0)
				m_grid->SetColumnWidth(j, infos[j].width + nIncreaseWidth);
		}
	
		m_grid->SetRowHeight(i, hROW + nIncreaseHeight);
	}
}

void CMapWnd::SaveBuffer(CString sKey, CString sData)
{
	CIB210200App* pApp = (CIB210200App*)AfxGetApp();
	pApp->SaveBuffer(sKey, sData);
}

CString CMapWnd::GetBuffer(CString sKey)
{
	CIB210200App* pApp = (CIB210200App*)AfxGetApp();
	return pApp->GetBuffer(sKey);
}

void CMapWnd::LoadBuffer()
{
	m_strCode = GetBuffer(JONGMUK_CODE);
	m_code->SetEditData(m_strCode);
}