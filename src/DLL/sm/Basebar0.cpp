// Basebar0.cpp : implementation file
//

#include "stdafx.h"
#include "Basebar0.h"
#include "runx.h"
#include "nbutton.h"
#include "regmap.h"
#include "tmenu.h"
#include "sedit.h"
//#include "RegCode.h"
#include "CategoryPopup.h"
#include "CoolDialogBar.h"


#include "../../h/axisvar.h"
#include "../../axis/axmsg.hxx"
#include "../../h/comcode.h"
#include "ioformat.h"

#define COLOR_BORDER	RGB(123, 123, 123)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBasebar0 dialog


CBasebar0::CBasebar0(CWnd* parent, bool (*callback)(int, WPARAM, LPARAM), CWnd* pParent /*=NULL*/)
	: CDialog(CBasebar0::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBasebar0)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_parent	= parent;
	m_axiscall	= callback;
	m_runX		= NULL;
//	m_groupS = NULL;
	m_codeS		= NULL;
	m_mapA		= NULL;
	m_mapS		= NULL;
	m_screen	= NULL;
	
	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);

	m_arMap.RemoveAll();

	m_bk = RGB(221, 221, 221);
	m_categoryPopup = NULL;
}

CBasebar0::~CBasebar0()
{
#ifndef DF_USESTL_CBASEBAR
	if (m_runX)	delete m_runX;
	//	if (m_groupS)	delete m_groupS;
	if (m_codeS)	delete m_codeS;
	if (m_mapA)	delete m_mapA;
	if (m_mapS)	delete m_mapS;
	if (m_screen)	delete m_screen;
#endif


	m_font.DeleteObject();
	m_arMap.RemoveAll();
}

void CBasebar0::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBasebar0)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBasebar0, CDialog)
	//{{AFX_MSG_MAP(CBasebar0)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasebar0 message handlers

BOOL CBasebar0::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBasebar0::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	const int	limitH = BANDHEIGHT*2 + CONTROLH + BANDGAP*4;
	if (cy > limitH)
		change_Position();
}

void CBasebar0::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TM_CON)
	{
		if (GetKind() == KIND_MANUAL)
			KillTimer(TM_CON);
		else	Next();
	}
	CDialog::OnTimer(nIDEvent);
}

void CBasebar0::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect	client;
	GetClientRect(client);

	const COLORREF color = dc.SetBkColor(RGB(242, 242, 242));
	dc.ExtTextOut(client.left, client.top, ETO_OPAQUE, client, "", 0, NULL);

	draw_Band(&dc);
}

void CBasebar0::draw_Band(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	CString	text;
	pDC->SetBkMode(TRANSPARENT);
	CFont*	font = pDC->SelectObject(&m_font);
	
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->FillSolidRect(m_tRc[0], m_bk);
	text.LoadString(ST_TEXT_SELECTCODE);
	pDC->DrawText(text, m_tRc[0], DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	pDC->Draw3dRect(m_tRc[0], RGB(108, 108, 108), RGB(255, 255, 255));

	pDC->FillSolidRect(m_tRc[1], m_bk);
	text.LoadString(ST_TEXT_SELECTMAP);
	pDC->DrawText(text, m_tRc[1], DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	pDC->Draw3dRect(m_tRc[1], RGB(108, 108, 108), RGB(255, 255, 255));

	pDC->SelectObject(font);

	CRect	bRc;
	m_codeList.GetWindowRect(bRc);
	ScreenToClient(&bRc);

	bRc.InflateRect(1, 1);
	pDC->Draw3dRect(bRc, COLOR_BORDER, COLOR_BORDER);

	m_mapList.GetWindowRect(bRc);
	ScreenToClient(&bRc);

	bRc.InflateRect(1, 1);
	pDC->Draw3dRect(bRc, COLOR_BORDER, COLOR_BORDER);
	
	if (m_runX)
	{
		m_runX->GetWindowRect(bRc);
		ScreenToClient(&bRc);

		bRc.InflateRect(1, 1);
		pDC->Draw3dRect(bRc, COLOR_BORDER, COLOR_BORDER);
	}
}

void CBasebar0::make_Ctrl()
{
	DWORD	style = WS_VISIBLE|WS_CHILD/*|WS_BORDER*/|WS_CLIPSIBLINGS|LVS_REPORT|
		LVS_SHOWSELALWAYS|LVS_ALIGNTOP|LVS_SINGLESEL|LVS_NOSORTHEADER;
	
	CString	text;
	m_codeList.Create(style, CRect(0, 0, 0 ,0), this, 20001);
	m_codeList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	text.LoadString(ST_TEXT_CODE);
	addColumn(m_codeList, text, 0);
	text.LoadString(ST_TEXT_CODENAME);
	addColumn(m_codeList, text, 1);

	m_mapList.Create(style, CRect(0, 0, 0, 0), this, 20002);
	m_mapList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	text.LoadString(ST_TSETUP_TEXT_TR);
	addColumn(m_mapList, text, 0);
	text.LoadString(ST_TEXT_MAPNAME);
	addColumn(m_mapList, text, 1);

#ifdef DF_USESTL_CBASEBAR
	m_runX = std::make_unique< CRunX>(this, m_bk);
#else
	m_runX = new CRunX(this, m_bk);
#endif

	style = WS_CHILD|WS_VISIBLE/*|WS_BORDER*/;
#ifdef DF_USESTL_CBASEBAR
	if (!m_runX->Create(NULL, "runX", style, CRect(0,0,0,0), this, (int) m_runX.get()))
#else
	if (!m_runX->Create(NULL, "runX", style, CRect(0, 0, 0, 0), this, (int)m_runX))
#endif
	{
#ifndef DF_USESTL_CBASEBAR
	 delete m_runX;
#endif
		m_runX = NULL;
	}

	DWORD	dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
/*
	m_groupS = new CNButton(CTRL_GROUP);
	m_groupS->set_Color(RGB(247, 251, 196));
	if (!m_groupS->Create(NULL, "groupS", dwStyle, CRect(0), this, -1))
	{
		delete m_groupS;
		m_groupS = NULL;
	}
	else
	{
		text.LoadString(ST_TEXT_DIVIDE_GROUP);
		m_groupS->set_ToolTipText(text);
		m_groupS->set_Image(CAxButtonName("Dgroup"));
	}
*/
	
#ifdef DF_USESTL_CBASEBAR
	m_codeS = std::make_unique<CNButton>(CTRL_CODE);
#else
	m_codeS = new CNButton(CTRL_CODE);
#endif

	m_codeS->set_Color(RGB(247, 251, 196));
	if (!m_codeS->Create(NULL, "Code Search", dwStyle, CRect(0,0,0,0), this, -1))
	{
#ifndef DF_USESTL_CBASEBAR
		delete m_codeS;
#endif
		m_codeS = NULL;
	}
	else
	{
		text.LoadString(ST_TEXT_CODE_SEARCH);
		m_codeS->set_ToolTipText(text);
		m_codeS->set_Image(CAxButtonName("codecodesearch"));
	}

#ifdef DF_USESTL_CBASEBAR
	m_mapA = std::make_unique<CNButton>(CTRL_MAPADD);
#else
	m_mapA = new CNButton(CTRL_MAPADD);
#endif
	m_mapA->set_Color(RGB(247, 251, 196));
	if (!m_mapA->Create(NULL, "all append", dwStyle, CRect(0,0,0,0), this, -1))
	{
		m_mapA = NULL;
	}
	else
	{
		text.LoadString(ST_TEXT_ALL_ADDSCREEN);
		m_mapA->set_ToolTipText(text);
		m_mapA->set_Image(CAxButtonName("codealladd"));
	}

#ifdef DF_USESTL_CBASEBAR
	m_mapS = std::make_unique<CNButton>(CTRL_MAP);
#else
	m_mapS = new CNButton(CTRL_MAP);
#endif

	m_mapS->set_Color(RGB(247, 251, 196));
	if (!m_mapS->Create(NULL, "Screen Search", dwStyle, CRect(0,0,0,0), this, -1))
	{
		m_mapS = NULL;
	}
	else
	{
		text.LoadString(ST_TEXT_SCREENSEARCH);
		m_mapS->set_ToolTipText(text);
		m_mapS->set_Image(CAxButtonName("codescreensearch"));
	}

#ifdef DF_USESTL_CBASEBAR
	m_screen = std::make_unique<CSEdit>(this);
#else
	m_screen = new CSEdit(this);
#endif

	dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_TABSTOP|ES_NUMBER|ES_LEFT;
#ifdef DF_USESTL_CBASEBAR
	if (!m_screen->CreateEx(WS_EX_STATICEDGE, _T("EDIT"), "", dwStyle, CRect(0,0,0,0), this, (int)m_screen.get()))
#else
	if (!m_screen->CreateEx(WS_EX_STATICEDGE, _T("EDIT"), "", dwStyle, CRect(0, 0, 0, 0), this, (int)m_screen))
#endif
	{
	//	delete m_screen;
		m_screen = NULL;
	}
	else	
		m_screen->SetLimitText(4);

	change_BandInfo();
	enable_Button(0);
}

void CBasebar0::change_Position()
{
	const int	gap = 2;
	int listH{};
	CRect	cRc, lRc;
	GetClientRect(cRc);

	cRc.DeflateRect(BANDGAP, BANDGAP, BANDGAP-1, BANDGAP-1);
	m_tRc[0].CopyRect(&cRc);
	m_tRc[0].bottom = m_tRc[0].top + BANDHEIGHT;

	if (m_runX)
	{
		lRc.CopyRect(&cRc);
		lRc.top = lRc.bottom - CONTROLH;
		m_runX->MoveWindow(lRc);
	}

	listH = (cRc.Height() - CONTROLH - 2*BANDHEIGHT - 2*BANDGAP - gap*2)/2;
	if (m_codeList.GetSafeHwnd())
	{
		lRc.SetRect(m_tRc[0].left, m_tRc[0].bottom + gap,
			m_tRc[0].right, m_tRc[0].bottom + gap + listH);
		m_codeList.MoveWindow(lRc);
		fit(m_codeList);
	}

	m_tRc[1].SetRect(m_tRc[0].left, m_tRc[0].bottom + gap + listH + BANDGAP,
			m_tRc[0].right, m_tRc[0].bottom + gap + listH + BANDGAP + BANDHEIGHT);
	if (m_mapList.GetSafeHwnd())
	{
		lRc.SetRect(m_tRc[1].left, m_tRc[1].bottom + gap,
			m_tRc[1].right, m_tRc[1].bottom + gap + listH);
		m_mapList.MoveWindow(lRc);
		fit(m_mapList);
	}

	
	CSize size = m_codeS->get_ImgSize();
	lRc.SetRect(m_tRc[0].right - gap*2 - size.cx, m_tRc[0].top,
			m_tRc[0].right - gap*2, m_tRc[0].bottom);
	CPoint pt = lRc.CenterPoint();
	lRc.SetRect(pt.x - size.cx/2, pt.y - size.cy/2,
			pt.x + size.cx/2, pt.y + size.cy/2);
	m_codeS->MoveWindow(lRc);

	lRc.OffsetRect(-(lRc.Width()+gap), 0);
//	m_groupS->MoveWindow(lRc);

	size = m_mapS->get_ImgSize();
	lRc.SetRect(m_tRc[1].right - gap*2 - size.cx, m_tRc[1].top,
			m_tRc[1].right - gap*2, m_tRc[1].bottom);
	pt = lRc.CenterPoint();
	lRc.SetRect(pt.x - size.cx/2, pt.y - size.cy/2,
			pt.x + size.cx/2, pt.y + size.cy/2);
	m_mapS->MoveWindow(lRc);

	lRc.OffsetRect(-(lRc.Width()+gap), 0);
	m_mapA->MoveWindow(lRc);
	if (m_screen)
		m_screen->MoveWindow(gap*4, lRc.top+gap, 48, lRc.Height() - gap*2);
	Invalidate(FALSE);
}

void CBasebar0::fit(CListCtrl& list)
{
	if (list.GetSafeHwnd())
	{
		int	width{};
		CRect	cRc;
		list.GetClientRect(cRc);
		if (list.GetItemCount() > 0)
		{
			CClientDC	dc(this);
			CString text = list.GetItemText(0, 0);
			const CSize	size = dc.GetTextExtent(text);
			width = size.cx + 5;
		}
		else
			width = list.GetColumnWidth(0);
		list.SetColumnWidth(0, width);
		list.SetColumnWidth(1, cRc.Width() - width);
	}
}

BOOL CBasebar0::addColumn(CListCtrl& list, LPCTSTR columns, int col, int width)
{
	LV_COLUMN lvc{};
	if (width == 0)
	{
		CClientDC	dc(this);
		CString		str = columns;
		const CSize		size = dc.GetTextExtent(str + "  ");
		
		width = size.cx + 5;
	}

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR)columns;
	lvc.cx	     = width;
	lvc.iSubItem = -1;

	return list.InsertColumn(col, &lvc);
}

BOOL CBasebar0::addItem(CListCtrl& list, CString items, int item, int subi)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subi;
	lvi.pszText  = (LPSTR)(LPCTSTR)items;

	if (!subi)	return list.InsertItem(&lvi);
	return list.SetItem(&lvi);
}

void CBasebar0::change_BandInfo()
{
	int		pos{};
	CString		tmpS, dat, skin;
	CStringArray	ary;

	CProfile profile(pkPalette);
	skin = profile.GetString(GENERALSN, szPalette);
	dat  = profile.GetString(GENERALSN, szPalettes);
	if (dat.IsEmpty())
		return;

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

		if (!skin.IsEmpty() && (skin != dat))		
			continue;

		dat = profile.GetString(dat, "RGB");
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
		if (arRgb.GetSize() == 3)
			m_color = RGB(arRgb.GetAt(0), arRgb.GetAt(1), arRgb.GetAt(2));
		else
			m_color = RGB(159,159,159);
		arRgb.RemoveAll();
		break;
	}

	draw_Band();
}

LONG CBasebar0::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case CTRL_CLOSE:
		Stop();	
		(*m_axiscall)(AXI_CONSEE, 0, CON_HIDE);	
		break;
	case CTRL_CODE:
		SelectCode();
		break;
	case CTRL_MAPADD:
		(*m_axiscall)(AXI_APPENDALLMAP, 0, 0);
		reload_Map();
		break;
	case CTRL_MAP:
		{
			CRegMap	dlg(m_menu);
			if (dlg.DoModal() == IDOK)	reload_Map();
		}
		break;
	case CTRL_PREV:		Prev();		break;
	case CTRL_NEXT:		Next();		break;
	case CTRL_PLAY:		(*m_axiscall)(AXI_ROTATEVIEW, 0, 1);	Play();		break;
	case CTRL_PAUSE:	(*m_axiscall)(AXI_ROTATEVIEW, 0, 0);	Pause();	break;
	case CTRL_CONTINUE:	Continue();	break;
	case CTRL_STOP:		(*m_axiscall)(AXI_ROTATEVIEW, 0, 0);	Stop();		break;
	case CTRL_CATEGORY: OnSelectCode((const char*)lParam);
	case CTRL_REQUESTGROUP:  
	{
		int	sendL = 0;
		char	sendB[16 * 1024] = { 0, }, tempB[32];
		int isz_updn = sz_uinfo;
		struct _updn* updn = (struct _updn*)&sendB[sendL];
		sendL += isz_updn;

		FillMemory((char*)updn, isz_updn, ' ');

		CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
		updn->uinfo.dirt[0] = 'U';
		updn->uinfo.cont[0] = 'g';
		CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
		updn->uinfo.retc[0] = 'U';
		(*m_axiscall)(AXI_SEARCH_GROUPLIST, MAKEWPARAM(0,sendL), (LPARAM)sendB); 
	}
	break;
	case CTRL_REQUESTGROUPCODE:
	{
		int	sendL = 0;
		CString stmp;
		char	sendB[16 * 1024] = { 0, }, tempB[32];
		int isz_updn = sz_uinfo + 2;
		struct _updn* updn = (struct _updn*)&sendB[sendL];
		sendL += isz_updn;

		FillMemory((char*)updn, isz_updn, ' ');

		CopyMemory(updn->uinfo.gubn, "MY", sizeof(updn->uinfo.gubn));
		updn->uinfo.dirt[0] = 'U';
		updn->uinfo.cont[0] = 'j';
		CopyMemory(updn->uinfo.nblc, "00001", sizeof(updn->uinfo.nblc));
		updn->uinfo.retc[0] = 'U';

		stmp.Format("%s", (const char*)lParam);

		memcpy((char*)&sendB[sz_uinfo], stmp, 2);

		(*m_axiscall)(AXI_SEARCH_GROUPCODE, MAKEWPARAM(0, sendL), (LPARAM)sendB);
	}
	break;
	default:break;
	}
	return 0;
}

void CBasebar0::set_MenuInfo(class CTMenu* menu)
{
	m_menu = menu;
	reload_Code(true);
	reload_Map();
}

void CBasebar0::reload_Map()
{
	CString	str, mapN;

	m_arMap.RemoveAll();
	m_mapList.DeleteAllItems();

	CProfile profile(pkScreenReg);

	CString buffer(profile.GetSectionLF(szScreen));
	const DWORD	dw = buffer.GetLength();

	if (dw > 0)
	{
		for (int ii = 0; ii < (int) dw; ii++)
		{
			str += buffer[ii];
			if (buffer[ii] != '\n')	continue;
			const int	pos = str.Find('=');
			if (pos == -1)	continue;
			mapN = str.Mid(pos+1);
			const int	itemN = m_mapList.GetItemCount();
			addItem(m_mapList, m_menu->GetMapping(mapN).Mid(2, 4), itemN, 0);
			addItem(m_mapList, m_menu->GetDesc(mapN), itemN, 1);
			str.Empty();
			m_arMap.Add(mapN);
		}

		fit(m_mapList);
	}
}

void CBasebar0::reload_Code(bool first)
{
	m_codeList.DeleteAllItems();

	CFile		file;
	CString		path, code, name, value;
	CMapStringToString	ary;

#ifdef DF_USESTL_CBASEBAR
	if (first)
	{
		std::unique_ptr<char[]>buffer;
		path.Format("%s\\%s\\%s", Axis::home, TABDIR, SCODE_FN);
		if (file.Open(path, CFile::modeRead))
		{
			const int L_scode = sizeof(struct scode);
			int fileL = gsl::narrow_cast<int>(file.GetLength());
			buffer = std::make_unique<char[]>(fileL + 1);;
			file.Read(buffer.get(), fileL);
			file.Close();

			const struct	scode* scode = (struct scode*)buffer.get();
			for (int ii = 0; fileL >= L_scode; ii++)
			{
				ary.SetAt(CString(scode[ii].scod, L_SCOD), "1");
				fileL -= L_scode;
			}
		}
	}
#else
	if (first)
	{
		char* buffer{};
		path.Format("%s\\%s\\%s", Axis::home, TABDIR, SCODE_FN);
		if (file.Open(path, CFile::modeRead))
		{
			const int L_scode = sizeof(struct scode);
			int fileL = gsl::narrow_cast<int>(file.GetLength());
			buffer = new char[fileL + 1];
			file.Read(buffer, fileL);
			file.Close();

			const struct	scode* scode = (struct scode*)buffer;
			for (int ii = 0; fileL >= L_scode; ii++)
			{
				ary.SetAt(CString(scode[ii].scod, L_SCOD), "1");
				fileL -= L_scode;
			}
			delete[] buffer;
		}
	}
#endif
	
	path.Format("%s\\%s\\%s\\regCode", Axis::home, USRDIR, Axis::user);
	if (!file.Open(path, CFile::modeRead))
	{
		fit(m_codeList);
		return;
	}
	struct _regCode
	{
		char	code[12];
		char	name[40];
	};

	struct _regCode regCode;
	while (file.Read(&regCode, sizeof(regCode)))
	{
		code = CString(regCode.code, sizeof(regCode.code));
		name = CString(regCode.name, sizeof(regCode.name));
		code.TrimRight();	
		name.TrimRight();
	
		if (first && !ary.Lookup(code, value))	//	stock code verification
			continue;

		const int row = m_codeList.GetItemCount();
		addItem(m_codeList, code, row, 0);
		addItem(m_codeList, name, row, 1);
	}
	file.Close();
	fit(m_codeList);
	ary.RemoveAll();
}

CString CBasebar0::GetCode()
{
	CString	code;
	return code;
}

CString CBasebar0::GetMapN()
{
	CString	mapN;
	return mapN;
}

void CBasebar0::enable_Button(int kind)
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

void CBasebar0::Prev()
{
	if (m_mapList.GetItemCount() <= 0)
	{
		CString	title, msg;
		msg.LoadString(ST_MSG_NOMAP);
		title.LoadString(STSM_TEXT_OK);
		::MessageBox(m_hWnd, msg, title, MB_ICONASTERISK);
		return;
	}

	CSize	sz;	CRect	rc;
	int	indexC{}, indexM{}, pos{};
	bool	prevCode = false, prevMap = false;
	SCROLLINFO	info{};

	if (GetOrder() ==ORDER_CODE)
	{
		if (m_codeList.GetItemCount() > 0)
		{
			indexC = m_codeList.GetNextItem(-1, LVNI_SELECTED);
			switch (indexC)
			{
			case -1:
				indexC = 0;
				m_codeList.SetItemState(indexC, LVIS_SELECTED, LVIS_SELECTED);
				m_codeList.SetScrollPos(SB_VERT, 0, TRUE);
				break;
			case 0:	
				prevMap = true;
				indexC = m_codeList.GetItemCount() -1;
				m_codeList.SetItemState(indexC, LVIS_SELECTED, LVIS_SELECTED);
			
				m_codeList.GetScrollInfo(SB_VERT, &info, SIF_ALL);
				pos = indexC - (int)info.nPos;
				m_codeList.GetItemRect(0, rc, LVIR_BOUNDS);
				sz.cy = rc.Height()*pos;
				m_codeList.Scroll(sz);
				break;
			default:
				indexC--;
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

		if (m_codeList.GetItemCount() > 0)
		{
			indexC = m_codeList.GetNextItem(-1, LVNI_SELECTED);
			switch (indexC)
			{
			case -1:
				indexC = 0;
				m_codeList.SetItemState(indexC, LVIS_SELECTED, LVIS_SELECTED);
				m_codeList.SetScrollPos(SB_VERT, 0, TRUE);
				break;
			case 0:	
				if (prevCode)	indexC = m_codeList.GetItemCount() -1;
				m_codeList.SetItemState(indexC, LVIS_SELECTED, LVIS_SELECTED);

				m_codeList.GetScrollInfo(SB_VERT, &info, SIF_ALL);
				pos = indexC - (int)info.nPos;
				m_codeList.GetItemRect(0, rc, LVIR_BOUNDS);
				sz.cy = rc.Height()*pos;
				m_codeList.Scroll(sz);
				break;
			default:
				if (prevCode)	indexC--;// = m_codeList.GetItemCount() -1;
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

void CBasebar0::Next()
{
	if (m_mapList.GetItemCount() <= 0)
	{
		CString	title, msg;
		msg.LoadString(ST_MSG_NOMAP);
		title.LoadString(STSM_TEXT_OK);
		KillTimer(TM_CON);
		::MessageBox(m_hWnd, msg, title, MB_ICONASTERISK);
		return;
	}
	
	CSize	sz;	CRect	rc;
	int	indexC{}, indexM{}, pos{};
	bool	nextCode = false, nextMap = false;
	SCROLLINFO	info{};

	if (GetOrder() ==ORDER_CODE)
	{
		if (m_codeList.GetItemCount() > 0)
		{
			indexC = m_codeList.GetNextItem(-1, LVNI_SELECTED);
			switch (indexC)
			{
			case -1:
				indexC = 0;
				m_codeList.SetItemState(indexC, LVIS_SELECTED, LVIS_SELECTED);
				m_codeList.SetScrollPos(SB_VERT, 0, TRUE);
				break;
			default:
				if (indexC == m_codeList.GetItemCount() - 1)
				{
					nextMap = true;
					indexC = 0;
				}
				else	indexC++;
			
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

		if (m_codeList.GetItemCount() > 0)
		{
			indexC = m_codeList.GetNextItem(-1, LVNI_SELECTED);
			switch (indexC)
			{
			case -1:
				indexC = 0;
				m_codeList.SetItemState(indexC, LVIS_SELECTED, LVIS_SELECTED);
				m_codeList.SetScrollPos(SB_VERT, 0, TRUE);
				break;
			default:
				if (nextCode)
				{
					if (indexC == m_codeList.GetItemCount() -1)
						indexC = 0;
					else	indexC++;
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

void CBasebar0::Play()
{
	if (m_mapList.GetItemCount() <= 0)
	{
		CString	title, msg;
		msg.LoadString(ST_MSG_NOMAP);
		title.LoadString(STSM_TEXT_OK);
		::MessageBox(m_hWnd, msg, title, MB_ICONASTERISK);
		return;
	}
	const int interval = m_runX->GetInterval()*1000;
	SetTimer(TM_CON, interval, NULL);

	enable_Button(CON_PALY);
}

void CBasebar0::Pause()
{
	KillTimer(TM_CON);

	enable_Button(CON_PAUSE);
}

void CBasebar0::Continue()
{
	if (m_mapList.GetItemCount() <= 0)
	{
		CString	title, msg;
		msg.LoadString(ST_MSG_NOMAP);
		title.LoadString(STSM_TEXT_OK);
		::MessageBox(m_hWnd, msg, title, MB_ICONASTERISK);
		return;
	}

	const int interval = m_runX->GetInterval()*1000;
	SetTimer(TM_CON, interval, NULL);
	enable_Button(CON_CONTINUE);
}

void CBasebar0::Stop()
{
	KillTimer(TM_CON);
	enable_Button(CON_STOP);
}

void CBasebar0::ChangeMap(bool code)
{
	// 주식 심볼 : 1301 , 선물 : 30301, 옵션 : 40301 => 선물/옵션 : focod
	CString	string, key;
	if (m_code.IsEmpty())
		string = m_mapN;
	else	string.Format("%s1301\t%s", m_mapN, m_code);
	
	(*m_axiscall)(AXI_CHANGEVIEW, 3, (LPARAM) (const char *)string);
//	if (!code)	return;
//	(*m_axiscall)(AXI_SETALL, 0, (LPARAM) (char *)string.operator LPCTSTR());
}

int CBasebar0::GetOrder()		{ return m_runX->GetOrder(); }
int CBasebar0::GetKind()		{ return m_runX->GetKind(); }
int CBasebar0::GetInterval()	{ return m_runX->GetInterval(); }

BOOL CBasebar0::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NM_LISTVIEW* listView{};
	switch(wParam)
	{
	case 20001:	// code list
		listView = (NM_LISTVIEW*) lParam;
		if (m_mapList.GetItemCount() <= 0)
			break;
		if (listView->hdr.code == NM_CLICK && listView->iItem != -1)	// 20070503-kwon
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

COLORREF CBasebar0::GetBkColor()
{
	int	r{}, g{}, b{};
	CString	value, tmpS;
	CProfile profile(pkPalette);

	value = profile.GetString(szPalette, "RGB038", "223, 239, 255");
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

void CBasebar0::Append_Map(CString dispN)
{
	CString file, key, mapN = m_menu->GetMap(dispN);
	if (mapN.IsEmpty())	return;

	for (int ii = 0; ii < m_arMap.GetSize(); ii++)
	{
		if (!m_arMap.GetAt(0).Compare(mapN))
			return;
	}

	m_arMap.Add(mapN);
	const int	itemN = m_mapList.GetItemCount();
	addItem(m_mapList, dispN, itemN, 0);
	addItem(m_mapList, m_menu->GetDesc(mapN), itemN, 1);
	fit(m_mapList);

	key.Format("%04d", itemN);

	CProfile profile(pkScreenReg);
	profile.Write(szScreen, key, mapN);
}

BOOL CBasebar0::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		// // 20070503-kwon추가
		case VK_UP:
		case VK_DOWN:
			if (GetFocus() == &m_codeList)
			{
				if (m_mapList.GetItemCount() <= 0)
					break;

				int index = m_codeList.GetSelectionMark();
				if (index != -1)
				{
					if (pMsg->wParam == VK_UP)	index--;
					if (pMsg->wParam == VK_DOWN)	index++;

					if (index < 0 || index >= m_codeList.GetItemCount())
						break;

					m_code = m_codeList.GetItemText(index, 0);
					index = m_mapList.GetNextItem(-1, LVNI_SELECTED);
					if (index < 0)	index = 0;
					m_mapN = m_arMap.GetAt(index);
					m_mapList.SetItemState(index, LVIS_SELECTED, LVIS_SELECTED);
					ChangeMap(true);
				}
			}
			break;
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CBasebar0::SelectCode()
{
#ifdef DF_USESTL_CBASEBAR
	if (m_categoryPopup)
		m_categoryPopup.reset();
#else
	m_categoryPopup->DestroyWindow();
	delete m_categoryPopup;
	m_categoryPopup = nullptr;
#endif

	if (!m_categoryPopup)
	{
#ifdef DF_USESTL_CBASEBAR
		m_categoryPopup = std::make_unique<CCategoryPopup>(this);
#else
		m_categoryPopup = new CCategoryPopup(this);
#endif
		m_categoryPopup->setParent(this);
		m_categoryPopup->CreateEx(WS_EX_TOOLWINDOW| WS_EX_TOPMOST, AfxRegisterWndClass(0), NULL,
			WS_POPUP, CRect(0, 0, 0, 0), NULL, NULL, NULL);
		m_categoryPopup->SetFont(&m_font);
	}

	CRect rc;
	m_codeS->GetWindowRect(&rc);
	rc.SetRect(rc.left, rc.bottom, rc.left + PT_CX, rc.bottom + PT_CY);

	m_categoryPopup->Initialize();
	m_categoryPopup->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER | SWP_SHOWWINDOW);
}

void CBasebar0::OnSelectCode(const char* data)
{
	CString s(data), line, code, name;

	int find{};

	m_codeList.DeleteAllItems();

	while (TRUE)
	{
		find = s.Find('\n');
		if (find < 0)
			break;
		
		line = s.Left(find);
		s.Delete(0, find + 1);

		find = line.Find('\t');
		if (find < 0)
			break;

		code = line.Left(find);
		name = line.Mid(find + 1, line.GetLength() - (find + 1));


		const int row = m_codeList.GetItemCount();
		addItem(m_codeList, code, row, 0);
		addItem(m_codeList, name, row, 1);
	}
}

void CBasebar0::OnDestroy() 
{
	CDialog::OnDestroy();
	
#ifndef DF_USESTL_CBASEBAR
if (m_categoryPopup)
{
	m_categoryPopup->DestroyWindow();
	delete m_categoryPopup;
}
#endif

}
