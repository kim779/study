// Dbar0.cpp : implementation file
//

#include "stdafx.h"
#include "Dbar0.h"
#include "runx.h"
#include "nbutton.h"
#include "regmap.h"
#include "Tmenu.h"
#include "CoolDialogBar.h"
#include "../../h/axisvar.h"
#include "../../axis/axmsg.hxx"

#include <afxtempl.h>

#define	CTRL_CLOSE	1000
#define	CTRL_GROUP	1001
#define	CTRL_CODE	1002
#define	CTRL_MAP	1003
#define	CTRL_MAPADD	1004
#define CTRL_CHECK	1005

#define	TM_CON		99

struct _codeReg
{
	char	code[12];
	char	name[40];
};

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDbar0

CDbar0::CDbar0(bool (*callback)(int, WPARAM, LPARAM), CString home, CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CDbar0)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_axiscall	= callback;
	m_home		= home;
	m_parent	= pParent;
	m_runX		= NULL;
	m_closeX	= NULL;
	m_groupS	= NULL;
	m_codeS		= NULL;
	m_mapA		= NULL;
	m_mapS		= NULL;
	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);

	m_arMap.RemoveAll();

	m_bk = RGB(221, 221, 221);
}

CDbar0::~CDbar0()
{
	if (m_runX)	delete m_runX;
	if (m_closeX)	delete m_closeX;
	if (m_groupS)	delete m_groupS;
	if (m_codeS)	delete m_codeS;
	if (m_mapA)	delete m_mapA;
	if (m_mapS)	delete m_mapS;
	if (m_check)	delete m_check;

	m_font.DeleteObject();
	m_arMap.RemoveAll();
}


BEGIN_MESSAGE_MAP(CDbar0, CDialog)
	//{{AFX_MSG_MAP(CDbar0)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
	ON_MESSAGE(WM_ON_CHKBOX, OnChangeLIST)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbar0 message handlers

CSize CDbar0::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	return CSize(VARWIDTH, 2000);
}

void CDbar0::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI	state;

	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = AFX_IDW_STATUS_BAR;

	if (CDialog::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;
	state.DoUpdate(pTarget, FALSE);
}

void CDbar0::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	client;
	GetClientRect(client);

	COLORREF color = dc.SetBkColor(::GetSysColor(COLOR_BTNFACE));
	dc.ExtTextOut(client.left, client.top, ETO_OPAQUE, client, "", 0, NULL);
	dc.SetBkColor(color);

	draw_Band(&dc);
	draw_Img(&dc);
}

void CDbar0::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	int	limitH = BANDHEIGHT*6 + CONTROLH + BANDGAP*5;
	if (cy > limitH)	change_Position();
}

void CDbar0::OnTimer(UINT_PTR nIDEvent) 
{
	if (nIDEvent == TM_CON)
	{
		if (GetKind() == KIND_MANUAL)
			KillTimer(TM_CON);
		else if (m_cbar->IsWindowVisible())
			Next();
		else
			Stop();
	}
	CDialog::OnTimer(nIDEvent);
}

void CDbar0::draw_Band(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	int bkMode = pDC->SetBkMode(TRANSPARENT);
	CFont*	font = pDC->SelectObject(&m_font);
	
	pDC->FillSolidRect(m_tRc[0], m_color);
	COLORREF color = pDC->SetTextColor(RGB(255, 255, 255));
	pDC->DrawText("네 비 게 이 터", m_tRc[0], DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	pDC->Draw3dRect(m_tRc[0], RGB(255, 255, 255), RGB(108, 108, 108));

	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->FillSolidRect(m_tRc[1], m_bk);
	pDC->DrawText("대 상 종 목", m_tRc[1], DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	pDC->Draw3dRect(m_tRc[1], RGB(108, 108, 108), RGB(255, 255, 255));

	pDC->FillSolidRect(m_tRc[2], m_bk);
	pDC->DrawText("대 상 화 면", m_tRc[2], DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	pDC->Draw3dRect(m_tRc[2], RGB(108, 108, 108), RGB(255, 255, 255));

	pDC->SetTextColor(color);
	pDC->SelectObject(font);
	pDC->SetBkMode(bkMode);
}

void CDbar0::make_Ctrl()
{
	DWORD	style = WS_VISIBLE|WS_CHILD|WS_BORDER|WS_CLIPSIBLINGS|LVS_REPORT|
		LVS_SHOWSELALWAYS|LVS_ALIGNTOP|LVS_SINGLESEL|LVS_NOSORTHEADER;
	
//	m_codeList.Create(style, CRect(0), this, 20001);
	m_codeList.Create(this, 20001, CRect(0), style | LVS_NOCOLUMNHEADER);
//	m_codeList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_codeList.SetCheckboxeStyle(RC_CHKBOX_NORMAL);	// Enable checkboxes

	m_codeList.InsertColumn(0, "코드");
	m_codeList.InsertColumn(1, "종목명");

//	addColumn(m_codeList, "코드", 0);
//	addColumn(m_codeList, "종목명", 1);

	m_mapList.Create(style, CRect(0), this, 20002);
	m_mapList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	addColumn(m_mapList, "TR", 0);
	addColumn(m_mapList, "화면명", 1);

	m_runX = new CRunX(this, m_home, m_bk);
	style = WS_CHILD|WS_VISIBLE|WS_BORDER;
	if (!m_runX->Create(NULL, "리모콘", style, CRect(0), this, (int) m_runX))
	{
		delete m_runX;
		m_runX = NULL;
	}

	CString	imgC, imgO, imgD;
	DWORD	dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	m_closeX = new CNButton(CTRL_CLOSE);
	m_closeX->set_Color(RGB(247, 251, 196));
	if (!m_closeX->Create(NULL, "네비게이터 닫기", dwStyle, CRect(0), this, -1))
	{
		delete m_closeX;
		m_closeX = NULL;
	}
	else
	{
		m_closeX->set_ToolTipText("네비게이터 닫기");
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "XBTN");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "XBTN_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "XBTN_dn");
		m_closeX->set_Image(imgC, imgO, imgD);
	}

	m_groupS = new CNButton(CTRL_GROUP);
	m_groupS->set_Color(RGB(247, 251, 196));
	if (!m_groupS->Create(NULL, "분류별", dwStyle, CRect(0), this, -1))
	{
		delete m_groupS;
		m_groupS = NULL;
	}
	else
	{
		m_groupS->set_ToolTipText("분류별");
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "그룹목록");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "그룹목록_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "그룹목록_dn");
		m_groupS->set_Image(imgC, imgO, imgD);
	}

	m_codeS = new CNButton(CTRL_CODE);
	m_codeS->set_Color(RGB(247, 251, 196));
	if (!m_codeS->Create(NULL, "종목검색", dwStyle, CRect(0), this, -1))
	{
		delete m_codeS;
		m_codeS = NULL;
	}
	else
	{
		m_codeS->set_ToolTipText("종목검색");
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "검색");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "검색_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "검색_dn");
		m_codeS->set_Image(imgC, imgO, imgD);
	}

	m_mapA = new CNButton(CTRL_MAPADD);
	m_mapA->set_Color(RGB(247, 251, 196));
	if (!m_mapA->Create(NULL, "all append", dwStyle, CRect(0), this, -1))
	{
		delete m_mapA;
		m_mapA = NULL;
	}
	else
	{
		m_mapA->set_ToolTipText("실행화면 전체추가");
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "tooledit");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "tooledit_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "tooledit_dn");
		m_mapA->set_Image(imgC, imgO, imgD);
	}

	m_mapS = new CNButton(CTRL_MAP);
	m_mapS->set_Color(RGB(247, 251, 196));
	if (!m_mapS->Create(NULL, "화면검색", dwStyle, CRect(0), this, -1))
	{
		delete m_mapS;
		m_mapS = NULL;
	}
	else
	{
		m_mapS->set_ToolTipText("화면검색");
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "검색");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "검색_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "검색_dn");
		m_mapS->set_Image(imgC, imgO, imgD);
	}

	m_check = new CNButton(CTRL_CHECK);
	m_check->set_Color(RGB(247, 251, 196));
	if (!m_check->Create(NULL, "전체선택", dwStyle, CRect(0), this, -1))
	{
		delete m_check;
		m_check = NULL;
	}
	else
	{
		m_check->set_ToolTipText("전체선택");
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check_dn");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check_dn");
		m_check->set_Image(imgC, imgO, imgD);
	}
	m_bCheck = true;

	change_BandInfo();
	enable_Button(0);
}

void CDbar0::change_Position()
{
	int	gap = 2, listH;
	CRect	cRc, lRc;
	GetClientRect(cRc);

	cRc.DeflateRect(BANDGAP, BANDGAP, BANDGAP-1, BANDGAP-1);
	m_tRc[0].SetRectEmpty();

	m_tRc[1].CopyRect(&cRc);
	m_tRc[1].bottom = m_tRc[1].top + BANDHEIGHT;

	if (m_runX)
	{
		lRc.CopyRect(&cRc);
		lRc.top = lRc.bottom - CONTROLH;
		m_runX->MoveWindow(lRc);
	}

	m_hRc.SetRect(m_tRc[1].left, m_tRc[1].bottom + gap, m_tRc[1].right, m_tRc[1].bottom + gap + 16);
	if (m_check)
	{
		lRc.SetRect(m_hRc.left + 5, m_hRc.top + 2, m_hRc.left + 5 + 13, m_hRc.top + 2 + 13);
		m_check->MoveWindow(lRc);
	}

	listH = (cRc.Height() - CONTROLH - 3*BANDHEIGHT - 3*BANDGAP - gap*2)/2;
	if (m_codeList.GetSafeHwnd())
	{
		lRc.SetRect(m_tRc[1].left, m_hRc.bottom, m_tRc[1].right, m_hRc.bottom + listH);
		m_codeList.MoveWindow(lRc);
		fit(m_codeList);
	}

	m_tRc[2].SetRect(m_tRc[1].left, m_tRc[1].bottom + gap + listH + BANDGAP + m_hRc.Height(),
			m_tRc[1].right, m_tRc[1].bottom + gap + listH + BANDGAP + m_hRc.Height() + BANDHEIGHT);
	if (m_mapList.GetSafeHwnd())
	{
		lRc.SetRect(m_tRc[2].left, m_tRc[2].bottom + gap,
			m_tRc[2].right, m_tRc[2].bottom + gap + listH + m_hRc.Height());
		m_mapList.MoveWindow(lRc);
		fit(m_mapList);
	}

	CSize	size = m_closeX->get_ImgSize();
	lRc.SetRect(m_tRc[0].right - gap*2 - size.cx, m_tRc[0].top,
			m_tRc[0].right - gap*2, m_tRc[0].bottom);
	CPoint	pt = lRc.CenterPoint();
	lRc.SetRect(pt.x - size.cx/2, pt.y - size.cy/2,
			pt.x + size.cx/2, pt.y + size.cy/2);
//	m_closeX->MoveWindow(lRc);

	size = m_codeS->get_ImgSize();
	lRc.SetRect(m_tRc[1].right - gap*2 - size.cx, m_tRc[1].top,
			m_tRc[1].right - gap*2, m_tRc[1].bottom);
	pt = lRc.CenterPoint();
	lRc.SetRect(pt.x - size.cx/2, pt.y - size.cy/2,
			pt.x + size.cx/2, pt.y + size.cy/2);
	m_codeS->MoveWindow(lRc);

	lRc.OffsetRect(-(lRc.Width()+gap), 0);
	m_groupS->MoveWindow(lRc);

	size = m_mapS->get_ImgSize();
	lRc.SetRect(m_tRc[2].right - gap*2 - size.cx, m_tRc[2].top,
			m_tRc[2].right - gap*2, m_tRc[2].bottom);
	pt = lRc.CenterPoint();
	lRc.SetRect(pt.x - size.cx/2, pt.y - size.cy/2,
			pt.x + size.cx/2, pt.y + size.cy/2);
	m_mapS->MoveWindow(lRc);

	lRc.OffsetRect(-(lRc.Width()+gap), 0);
	m_mapA->MoveWindow(lRc);

	Invalidate(FALSE);
}

void CDbar0::fit(CListCtrl& list)
{
	if (list.GetSafeHwnd())
	{
		int	width;
		CRect	cRc;
		list.GetClientRect(cRc);
		if (list.GetItemCount() > 0)
		{
			CClientDC	dc(this);
			CString text = list.GetItemText(0, 0);
			CSize	size = dc.GetTextExtent(text);
			width = size.cx + 5;
		}
		else
			width = list.GetColumnWidth(0);
		if (&list == &m_codeList)
			width += 10;
		list.SetColumnWidth(0, width);
		list.SetColumnWidth(1, cRc.Width() - width);
	}
}

BOOL CDbar0::addColumn(CListCtrl& list, LPCTSTR columns, int col, int width)
{
	LV_COLUMN lvc;
	if (width == 0)
	{
		CClientDC	dc(this);
		CString		str = columns;
		CSize		size = dc.GetTextExtent(str);
		
		width = size.cx + 5;
	}

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR)columns;
	lvc.cx	     = width;
	lvc.iSubItem = -1;

	return list.InsertColumn(col, &lvc);
}

BOOL CDbar0::addItem(CListCtrl& list, CString items, int item, int subi)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subi;
	lvi.pszText  = (LPTSTR)items.operator LPCTSTR();

	if (!subi)	return list.InsertItem(&lvi);
	return list.SetItem(&lvi);
}

void CDbar0::change_BandInfo()
{
	int		pos;
	char		buffer[128];
	CString		file, tmpS, dat, skin;
	CStringArray	ary;

	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	DWORD dw = GetPrivateProfileString(GENERALSN, "Palette", "", buffer, sizeof(buffer), file);
	if (dw <= 0)	skin = _T("");
	else		skin = buffer;
	dw = GetPrivateProfileString(GENERALSN, "Palettes", "", buffer, sizeof(buffer), file);
	if (dw <= 0)	return;

	dat = buffer;
	for (; !dat.IsEmpty();)
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			ary.Add(dat);
			break;
		}
		tmpS = dat.Left(pos++);
		dat = dat.Mid(pos);
		ary.Add(tmpS);
	}

	CArray	< int, int >	arRgb;
	for (int ii = 0; ii < ary.GetSize() && ii < MAX; ii++)
	{
		dat = ary.GetAt(ii);
		dat.TrimLeft();	dat.TrimRight();

		if (!skin.IsEmpty() && (skin != dat))		continue;
		GetPrivateProfileString(dat, "RGB", "", buffer, sizeof(buffer), file);
		dat = buffer;
		for (; !dat.IsEmpty();)
		{
			pos = dat.Find(',');
			if (pos == -1)
			{
				arRgb.Add(atoi(dat));
				break;
			}
			tmpS = dat.Left(pos++);
			dat = dat.Mid(pos);
			arRgb.Add(atoi(tmpS));
		}
		m_color = RGB(arRgb.GetAt(0), arRgb.GetAt(1), arRgb.GetAt(2));
		arRgb.RemoveAll();
		break;
	}

	draw_Band();
}

LRESULT CDbar0::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case CTRL_CLOSE:Stop();	(*m_axiscall)(AXI_CONSEE, 0, CON_HIDE);	break;
	case CTRL_CODE:		(*m_axiscall)(AXI_XTR, REGCODE, 0);	break;
	case CTRL_MAPADD:
		(*m_axiscall)(AXI_APPENDALLMAP, 0, 0);
		reload_Map();
		break;
	case CTRL_MAP:
		{
			CRegMap	dlg(m_menu, m_home, m_user);
			if (dlg.DoModal() == IDOK)	reload_Map();
		}
		break;
	case CTRL_PREV:		Prev();		break;
	case CTRL_NEXT:		Next();		break;
	case CTRL_PLAY:		Play();		break;
	case CTRL_PAUSE:	Pause();	break;
	case CTRL_CONTINUE:	Continue();	break;
	case CTRL_STOP:		Stop();		break;
	case CTRL_CHECK:
		{
			m_bCheck = !m_bCheck;

			CString	imgC, imgO, imgD;
			if (m_bCheck)
			{
				imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check_dn");
				imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check_dn");
				imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check_dn");
				m_check->set_Image(imgC, imgO, imgD);
			}
			else
			{
				imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check");
				imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check_en");
				imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check");
				m_check->set_Image(imgC, imgO, imgD);
			}
			m_check->Invalidate();
			setCodeListCheck();
		}
		break;
	default:break;
	}
	return 0;
}

void CDbar0::set_MenuInfo(class CTMenu* menu, CString user)
{
	m_menu = menu;
	m_user = user;

	reload_Code();
	reload_Map();
}

void CDbar0::reload_Map()
{
	CString	file, str, mapN;
	char	buf[1024*16];

	m_arMap.RemoveAll();
	m_mapList.DeleteAllItems();

	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, SCREENREG);
	DWORD	dw = GetPrivateProfileSection("SCREEN", buf, sizeof(buf), file);
	if (dw > 0)
	{
		for (int ii = 0; ii < (int) dw; ii++)
		{
			str += buf[ii];
			if (buf[ii] != '\0')	continue;
	 		int	pos = str.Find('=');
			if (pos == -1)	continue;
			mapN = str.Mid(pos+1);
			int	itemN = m_mapList.GetItemCount();
			addItem(m_mapList, m_menu->GetMapping(mapN).Mid(2, 4), itemN, 0);
			addItem(m_mapList, m_menu->GetDesc(mapN), itemN, 1);
			str.Empty();
			m_arMap.Add(mapN);
		}

		fit(m_mapList);
	}
}

void CDbar0::reload_Code()
{
	m_codeList.DeleteAllItems();

	CFile		file;
	CString		path, code, name;
	path.Format("%s\\%s\\%s\\codereg", m_home, USRDIR, m_user);
	if (!file.Open(path, CFile::modeRead))	return;

	struct _codeReg codeReg;
	while (file.Read(&codeReg, sizeof(codeReg)))
	{
		code = CString(codeReg.code, sizeof(codeReg.code));
		name = CString(codeReg.name, sizeof(codeReg.name));
		code.TrimRight();	name.TrimRight();

		int row = m_codeList.GetItemCount();
		m_codeList.InsertItem(row, code);
		m_codeList.SetItemText(row, 1, name);
		m_codeList.SetItemStates(row, RC_ITEM_CHECKED);
//		addItem(m_codeList, code, row, 0);
//		addItem(m_codeList, name, row, 1);
	}
	file.Close();

	fit(m_codeList);
}

CString CDbar0::GetCode()
{
	CString	code;
	return code;
}

CString CDbar0::GetMapN()
{
	CString	mapN;
	return mapN;
}

void CDbar0::enable_Button(int kind)
{
	switch (kind)
	{
	case CON_PREV:	case CON_NEXT:		break;
	case CON_PALY:
		m_runX->EnableButton(CTRL_PLAY, FALSE);
		m_runX->EnableButton(CTRL_PAUSE, TRUE);
		m_runX->EnableButton(CTRL_CONTINUE, FALSE);
		m_runX->EnableButton(CTRL_STOP, TRUE);
		break;
	case CON_PAUSE:
		m_runX->EnableButton(CTRL_PLAY, TRUE);
		m_runX->EnableButton(CTRL_PAUSE, FALSE);
		m_runX->EnableButton(CTRL_CONTINUE, TRUE);
		m_runX->EnableButton(CTRL_STOP, TRUE);
		break;
	case CON_CONTINUE:
		m_runX->EnableButton(CTRL_PLAY, FALSE);
		m_runX->EnableButton(CTRL_PAUSE, TRUE);
		m_runX->EnableButton(CTRL_CONTINUE, FALSE);
		m_runX->EnableButton(CTRL_STOP, TRUE);
		break;
	case CON_STOP:
	default:
		m_runX->EnableButton(CTRL_PLAY, TRUE);
		m_runX->EnableButton(CTRL_PAUSE, FALSE);
		m_runX->EnableButton(CTRL_CONTINUE, FALSE);
		m_runX->EnableButton(CTRL_STOP, FALSE);
		break;
	}
	m_runX->Invalidate(FALSE);
}

void CDbar0::Prev()
{
	if (m_mapList.GetItemCount() <= 0)
	{
		::MessageBox(m_hWnd, "대상화면이 없습니다.", "오류", MB_ICONASTERISK);
		return;
	}

	CSize	sz;	CRect	rc;
	int	indexC, indexM, pos;
	bool	prevCode = false, prevMap = false;
	SCROLLINFO	info;

	if (GetOrder() == ORDER_CODE)
	{
		if (m_codeList.GetItemCount() > 0 && getCheckCodeCount())
		{
			indexC = m_codeList.GetLastItem(RC_ITEM_CHECKED, m_codeList.GetNextItem(-1, LVNI_SELECTED));
			switch (indexC)
			{
			case -1:
				if (m_codeList.GetNextItem(-1, LVNI_SELECTED) != - 1)
					prevMap = true;
				indexC = m_codeList.GetLastItem(RC_ITEM_CHECKED, m_codeList.GetItemCount());
			default:
				m_codeList.SetItemState(indexC, LVIS_SELECTED, LVIS_SELECTED);
				
				m_codeList.GetScrollInfo(SB_VERT, &info, SIF_ALL);
				pos = indexC - (int)info.nPos;
				m_codeList.GetItemRect(0, rc, LVIR_BOUNDS);
				sz.cy = rc.Height()*pos;
				m_codeList.Scroll(sz);
				break;
			}
			m_code = m_codeList.GetItemText(indexC, 0);
		}
		else
		{
			prevMap = true;
			m_code = _T("");
		}

		indexM = m_mapList.GetNextItem(-1, LVNI_SELECTED);
		switch (indexM)
		{
		case -1:
			indexM = 0;
			m_mapList.SetItemState(indexM, LVIS_SELECTED, LVIS_SELECTED);
			m_mapList.SetScrollPos(SB_VERT, 0, TRUE);
			break;
		case 0:
			if (prevMap)	indexM = m_mapList.GetItemCount() -1;
			m_mapList.SetItemState(indexM, LVIS_SELECTED, LVIS_SELECTED);

			m_mapList.GetScrollInfo(SB_VERT, &info, SIF_ALL);
			pos = indexM - (int)info.nPos;
			m_mapList.GetItemRect(0, rc, LVIR_BOUNDS);
			sz.cy = rc.Height()*pos;
			m_mapList.Scroll(sz);
			break;
		default:
			if (prevMap)	indexM--;
			m_mapList.SetItemState(indexM, LVIS_SELECTED, LVIS_SELECTED);
			
			m_mapList.GetScrollInfo(SB_VERT, &info, SIF_ALL);
			pos = indexM - (int)info.nPos;
			m_mapList.GetItemRect(0, rc, LVIR_BOUNDS);
			sz.cy = rc.Height()*pos;
			m_mapList.Scroll(sz);
		}
		m_mapN = m_arMap.GetAt(indexM);
	}
	else
	{
		indexM = m_mapList.GetNextItem(-1, LVNI_SELECTED);
		switch (indexM)
		{
		case -1:
			indexM = 0;
			m_mapList.SetItemState(indexM, LVIS_SELECTED, LVIS_SELECTED);
			m_mapList.SetScrollPos(SB_VERT, 0, TRUE);
			break;
		case 0:	
			prevCode = true;
			indexM = m_mapList.GetItemCount() -1;
			m_mapList.SetItemState(indexM, LVIS_SELECTED, LVIS_SELECTED);

			m_mapList.GetScrollInfo(SB_VERT, &info, SIF_ALL);
			pos = indexM - (int)info.nPos;
			m_mapList.GetItemRect(0, rc, LVIR_BOUNDS);
			sz.cy = rc.Height()*pos;
			m_mapList.Scroll(sz);
			break;
		default:
			indexM--;
			m_mapList.SetItemState(indexM, LVIS_SELECTED, LVIS_SELECTED);

			m_mapList.GetScrollInfo(SB_VERT, &info, SIF_ALL);
			pos = indexM - (int)info.nPos;
			m_mapList.GetItemRect(0, rc, LVIR_BOUNDS);
			sz.cy = rc.Height()*pos;
			m_mapList.Scroll(sz);
			break;
		}
		m_mapN = m_arMap.GetAt(indexM);

		if (m_codeList.GetItemCount() > 0 && getCheckCodeCount())
		{
			indexC = m_codeList.GetLastItem(RC_ITEM_CHECKED, m_codeList.GetNextItem(-1, LVNI_SELECTED));
			switch (indexC)
			{
			case -1:
				indexC = m_codeList.GetLastItem(RC_ITEM_CHECKED, m_codeList.GetItemCount());
			default:
				if (!prevCode)
				{
					indexC = m_codeList.GetNextItem(-1, LVNI_SELECTED);
					if (indexC == -1)
						indexC = m_codeList.GetLastItem(RC_ITEM_CHECKED, m_codeList.GetItemCount());
				}
				m_codeList.SetItemState(indexC, LVIS_SELECTED, LVIS_SELECTED);

				m_codeList.GetScrollInfo(SB_VERT, &info, SIF_ALL);
				pos = indexC - (int)info.nPos;
				m_codeList.GetItemRect(0, rc, LVIR_BOUNDS);
				sz.cy = rc.Height()*pos;
				m_codeList.Scroll(sz);
				break;
			}
			m_code = m_codeList.GetItemText(indexC, 0);
		}
		else	m_code = _T("");
	}

	ChangeMap((GetOrder() == ORDER_CODE) ? true : false);
}

void CDbar0::Next()
{
	if (m_mapList.GetItemCount() <= 0)
	{
		::MessageBox(m_hWnd, "대상화면이 없습니다.", "오류", MB_ICONASTERISK);
		return;
	}
	
	CSize	sz;	CRect	rc;
	int	indexC, indexM, pos;
	bool	nextCode = false, nextMap = false;
	SCROLLINFO	info;

	if (GetOrder() ==ORDER_CODE)
	{
		if (m_codeList.GetItemCount() > 0 && getCheckCodeCount())
		{
			indexC = m_codeList.GetFirstItem(RC_ITEM_CHECKED, m_codeList.GetNextItem(-1, LVNI_SELECTED));
			switch (indexC)
			{
			case -1:
				if (m_codeList.GetNextItem(-1, LVNI_SELECTED) != - 1)
					nextMap = true;
				indexC = m_codeList.GetFirstItem(RC_ITEM_CHECKED, -1);
			default:
				m_codeList.SetItemState(indexC, LVIS_SELECTED, LVIS_SELECTED);
			
				m_codeList.GetScrollInfo(SB_VERT, &info, SIF_ALL);
				pos = indexC - (int)info.nPos;
				m_codeList.GetItemRect(0, rc, LVIR_BOUNDS);
				sz.cy = rc.Height()*pos;
				m_codeList.Scroll(sz);
				break;
			}
			m_code = m_codeList.GetItemText(indexC, 0);
		}
		else
		{
			nextMap = true;
			m_code = _T("");
		}

		indexM = m_mapList.GetNextItem(-1, LVNI_SELECTED);
		switch (indexM)
		{
		case -1:
			indexM = 0;
			m_mapList.SetItemState(indexM, LVIS_SELECTED, LVIS_SELECTED);
			m_mapList.SetScrollPos(SB_VERT, 0, TRUE);
			break;
		default:
			if (nextMap)
			{
				if (indexM == m_mapList.GetItemCount() - 1)
					indexM = 0;
				else	indexM++;
			}
			m_mapList.SetItemState(indexM, LVIS_SELECTED, LVIS_SELECTED);
			
			m_mapList.GetScrollInfo(SB_VERT, &info, SIF_ALL);
			pos = indexM - (int)info.nPos;
			m_mapList.GetItemRect(0, rc, LVIR_BOUNDS);
			sz.cy = rc.Height()*pos;
			m_mapList.Scroll(sz);
			break;
		}
		m_mapN = m_arMap.GetAt(indexM);
	}
	else
	{
		indexM = m_mapList.GetNextItem(-1, LVNI_SELECTED);
		switch (indexM)
		{
		case -1:
			indexM = 0;
			m_mapList.SetItemState(indexM, LVIS_SELECTED, LVIS_SELECTED);
			m_mapList.SetScrollPos(SB_VERT, 0, TRUE);
			break;
		default:
			if (indexM == m_mapList.GetItemCount() -1)
			{
				nextCode = true;
				indexM = 0;
			}
			else	indexM++;
			m_mapList.SetItemState(indexM, LVIS_SELECTED, LVIS_SELECTED);

			m_mapList.GetScrollInfo(SB_VERT, &info, SIF_ALL);
			pos = indexM - (int)info.nPos;
			m_mapList.GetItemRect(0, rc, LVIR_BOUNDS);
			sz.cy = rc.Height()*pos;
			m_mapList.Scroll(sz);
			break;
		}
		m_mapN = m_arMap.GetAt(indexM);

		if (m_codeList.GetItemCount() > 0 && getCheckCodeCount())
		{
			indexC = m_codeList.GetFirstItem(RC_ITEM_CHECKED, m_codeList.GetNextItem(-1, LVNI_SELECTED));
			switch (indexC)
			{
			case -1:
				indexC = m_codeList.GetFirstItem(RC_ITEM_CHECKED, -1);
			default:
				if (!nextCode)
				{
					indexC = m_codeList.GetNextItem(-1, LVNI_SELECTED);
					if (indexC == -1)
						indexC = m_codeList.GetFirstItem(RC_ITEM_CHECKED, -1);
				}
				m_codeList.SetItemState(indexC, LVIS_SELECTED, LVIS_SELECTED);
				
				m_codeList.GetScrollInfo(SB_VERT, &info, SIF_ALL);
				pos = indexC - (int)info.nPos;
				m_codeList.GetItemRect(0, rc, LVIR_BOUNDS);
				sz.cy = rc.Height()*pos;
				m_codeList.Scroll(sz);
				break;
			}
			m_code = m_codeList.GetItemText(indexC, 0);
		}
		else	m_code = _T("");
	}

	ChangeMap((GetOrder() == ORDER_CODE) ? true : false);
}

void CDbar0::Play()
{
	if (m_mapList.GetItemCount() <= 0)
	{
		::MessageBox(m_hWnd, "대상화면이 없습니다.", "확인", MB_ICONASTERISK);
		return;
	}
	int interval = m_runX->GetInterval()*1000;
	SetTimer(TM_CON, interval, NULL);

	enable_Button(CON_PALY);
}

void CDbar0::Pause()
{
	KillTimer(TM_CON);

	enable_Button(CON_PAUSE);
}

void CDbar0::Continue()
{
	if (m_mapList.GetItemCount() <= 0)
	{
		::MessageBox(m_hWnd, "대상화면이 없습니다.", "확인", MB_ICONASTERISK);
		return;
	}

	int interval = m_runX->GetInterval()*1000;
	SetTimer(TM_CON, interval, NULL);
	enable_Button(CON_CONTINUE);
}

void CDbar0::Stop()
{
	KillTimer(TM_CON);
	enable_Button(CON_STOP);
}

void CDbar0::ChangeMap(bool code)
{
	// 주식 심볼 : 1301 , 선물 : 30301, 옵션 : 40301
	CString	string, key;
	if (m_code.IsEmpty())
		string = m_mapN;
	else
	{
		int len = m_code.GetLength();
		if (len == 6)
			string.Format("%s1301\t%s", m_mapN, m_code);
		else
		{
			key = m_code.Left(2);
			if (key == "10" || key == "40")	// 선물, 스프레드
				string.Format("%s30301\t%s", m_mapN, m_code);
			else
				string.Format("%s40301\t%s", m_mapN, m_code);
		}
	}
	(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM) (char *)string.operator LPCTSTR());
	if (!code)	return;
	(*m_axiscall)(AXI_SETALL, 0, (LPARAM) (char *)string.operator LPCTSTR());
}

int CDbar0::GetOrder()		{ return m_runX->GetOrder(); }
int CDbar0::GetKind()		{ return m_runX->GetKind(); }
int CDbar0::GetInterval()	{ return m_runX->GetInterval(); }

BOOL CDbar0::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NM_LISTVIEW* listView;
	switch(wParam)
	{
	case 20001:	// code list
		listView = (NM_LISTVIEW*) lParam;
		if (m_mapList.GetItemCount() <= 0)
			break;
		if (listView->hdr.code == NM_DBLCLK && listView->iItem != -1)
		{
			m_code = m_codeList.GetItemText(listView->iItem, 0);
			int index = m_mapList.GetNextItem(-1, LVNI_SELECTED);
			if (index < 0)	index = 0;
			m_mapN = m_arMap.GetAt(index);
			m_mapList.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
			ChangeMap(true);
		}
		break;
	case 20002:	// tr list
		listView = (NM_LISTVIEW*) lParam;
		if (listView->hdr.code == NM_DBLCLK && listView->iItem != -1)
		{
			m_mapN = m_arMap.GetAt(listView->iItem);
			if (m_codeList.GetItemCount() < 0)
				m_code = _T("");
			else
			{
				int index = m_codeList.GetNextItem(-1, LVNI_SELECTED);
				if (index < 0)	index = 0;
				m_code = m_codeList.GetItemText(index, 0);
				m_codeList.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
			}
			ChangeMap();
		}
		break;
	default:
		break;
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

COLORREF CDbar0::GetBkColor()
{
	int	r, g, b;
	char	buf[32];
	CString	file, value, tmpS;
	
	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	GetPrivateProfileString("Palette", "RGB038", "223, 239, 255", buf, sizeof(buf), file);

	value = buf;
	int pos = value.Find(',');
	if (pos == -1)	r = 223;
	else
	{
		tmpS = value.Left(pos++);
		value = value.Mid(pos);
		r = atoi(tmpS);
	}

	pos = value.Find(',');
	if (pos == -1)	g = 239;
	else
	{
		tmpS = value.Left(pos++);
		value = value.Mid(pos);
		g = atoi(tmpS);
	}

	value.TrimLeft();
	value.TrimRight();
	if (value.IsEmpty())	b = 255;
	else	b = atoi(value);
	return RGB(r, g, b);
}

void CDbar0::draw_Img(CDC* pDC)
{
	CString		file;
	HBITMAP		hBitmap;
	
	if (((CCoolDialogBar *) m_cbar)->GetFloat())
		file.Format("%s\\image\\codelistheader2.bmp", m_home);
	else
		file.Format("%s\\image\\codelistheader.bmp", m_home);

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); 
	
	if (hBitmap == NULL)	return;
	BITMAP  bm;	CDC     buffDC ;
	(CBitmap::FromHandle(hBitmap))->GetBitmap(&bm);
	
	buffDC.CreateCompatibleDC(pDC) ;
	CBitmap* oldbuffBitmap = buffDC.SelectObject(CBitmap::FromHandle(hBitmap));
	
	pDC->BitBlt(m_hRc.left, m_hRc.top, bm.bmWidth, bm.bmHeight, &buffDC, 0, 0, SRCCOPY);
	buffDC.SelectObject( oldbuffBitmap );
	DeleteObject(hBitmap);
}

void CDbar0::setCodeListCheck()
{
	for (int ii = 0; ii < m_codeList.GetItemCount(); ii++)
	{
		m_codeList.SetItemStates(ii, m_bCheck ? RC_ITEM_CHECKED : RC_ITEM_UNCHECKED);
	}	
}

LRESULT CDbar0::OnChangeLIST(WPARAM wParam, LPARAM lParam)
{
	bool bCheck = true;
	for (int ii = 0; ii < m_codeList.GetItemCount(); ii++)
	{
		if (m_codeList.GetItemStates(ii) & RC_ITEM_UNCHECKED)
		{
			bCheck = false;
			break;
		}
	}

	m_bCheck = bCheck;
	CString	imgC, imgO, imgD;
	if (m_bCheck)
	{
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check_dn");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check_dn");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check_dn");
		m_check->set_Image(imgC, imgO, imgD);
	}
	else
	{
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "check");
		m_check->set_Image(imgC, imgO, imgD);
	}
	m_check->Invalidate();

	return 0;
}

int CDbar0::getCheckCodeCount()
{
	int count = 0;
	for (int ii = 0; ii < m_codeList.GetItemCount(); ii++)
	{
		if (m_codeList.GetItemStates(ii) & RC_ITEM_CHECKED)
			count++;
	}

	return count;
}

BOOL CDbar0::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			pMsg->wParam = VK_TAB;
		break;
	}	
	
	return CDialog::PreTranslateMessage(pMsg);
}
