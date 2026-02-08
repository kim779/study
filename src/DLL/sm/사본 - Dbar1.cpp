// Dbar1.cpp : implementation file
// MDI 상태에서의 상단 기본 툴바
//메인툴바 로고때문에 72픽셀 띄움
#include "stdafx.h"
#include "Dbar1.h"
#include "xcom.h"
#include "xcomex.h"
#include "nbutton.h"
#include "hBand.h"
#include "svBand.h"
#include "SDIBand.h"
#include "trsearch.h"
#include "tmenu.h"
#include "smdefine.h"
#include "NPalette.h"

#include "../../h/axisvar.h"
#include "../../axis/axmsg.hxx"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RBANDW		108
#define	GAP		3
#define	LOGOWIDTH	134

#define	TOOL_MENUBASE	6000
#define	TOOL_MENUEND	6100
#define ORDERSCREEN1	6101
#define ORDERSCREEN2	6102

#define CTRL_CHANGE	0x58

#define TOOL_HIDE_WIDTH 0
#define TOPMARGIN		7
#define ID_SETACCOUNT                   32865

const int buttonSize = 22;

#include "Winuser.h"
/////////////////////////////////////////////////////////////////////////////
// CDbar1

CDbar1::CDbar1(bool (*callback)(int, WPARAM, LPARAM))
{
	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);

	m_axiscall	= callback;
	m_height	= HEIGHT_CTRLBAR1;

	m_hide		= NULL;
	m_xcom		= NULL;
	m_searchS	= NULL;

	for (int ii = 0; ii < MAX_FUNC; ii++)
	{
		m_func[ii] = NULL;
	}

	m_xcomex	= NULL;
	m_register	= NULL;
	m_band		= NULL;
	m_sband		= NULL;
	m_vband		= NULL;
	m_dept		= "";

	m_itemW = 0;

	CProfile profile(pkUserTool);
	m_type = profile.GetInt(szUserSN, TOOLTYPE, TTYPE2);
	m_type = TTYPE2;

	m_vsN = V_SCREEN1;
	m_bFunc = profile.GetInt(szUserSN, "tool", 1);
	m_vMode = profile.GetInt(szUserSN, "toolex", 0);

	m_arFunc.RemoveAll();

	CString value(profile.GetString(szToolFunc, "Func", "0, 1, 2, 3, 5, 6"));

	for (ii = 0; !value.IsEmpty(); ii++)
	{
		CString func = Parser(value, ",");	
		func.TrimLeft();	func.TrimRight();
		if (func.IsEmpty())	continue;

		m_arFunc.Add(atoi(func));
	}
	m_nFunc	= m_arFunc.GetSize();

	m_FuncType = profile.GetInt(szToolFunc, "Type", 1);

	//m_bmpBand.LoadBitmap(IDB_BAR1BACK);
	m_bmpBand = Axis::GetSkinBitmap("BAND");

	m_bmpHome = Axis::GetBitmap("TEXT_SCREEN");

	

	m_registerX = m_funcX = m_xcomX = 0;
}

CDbar1::~CDbar1()
{
	if (m_hide)		delete m_hide;
	if (m_xcom)		delete m_xcom;
	if (m_searchS)	delete m_searchS;
	if (m_tacnt)	delete m_tacnt;

	for (int ii = 0; ii < MAX_FUNC; ii++)
		delete m_func[ii];

	if (m_xcomex)	delete m_xcomex;
	if (m_register)	delete m_register;
	if (m_band)		delete m_band;
	if (m_sband)	delete m_sband;
	if (m_vband)	
	{
		m_vMode = m_vband->GetBandMode();

		CProfile profile(pkUserTool);
		profile.Write(szUserSN, "toolex", Format("%d", m_vMode));
		profile.Write(szUserSN, "tool", Format("%d", m_bFunc));

		delete m_vband;
	}

	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CDbar1, CDialogBar)
	//{{AFX_MSG_MAP(CDbar1)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbar1 message handlers

void CDbar1::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	draw_BackGround(&dc);
}

LONG CDbar1::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case CTRL_SEARCHS:
		{
			CTrSearch	dlg(m_menu, m_axiscall);
			dlg.setPos(GetPos(1));
			if (dlg.DoModal() == IDOK)
				(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(const char *)dlg.getTR());
		}
		break;
	case CTRL_REGISTER:
		(*m_axiscall)(AXI_FUNCKEY, wParam, lParam);
		break;
	case CTRL_HIDE:
		(*m_axiscall)(AXI_HIDETICKER, 0, 4);	break;
		break;
	case CTRL_FUNC:
		{
			m_bFunc = !m_bFunc;
			
			SetPosition();
		}
		break;
	case CTRL_FUNC+1:	case CTRL_FUNC+2:	case CTRL_FUNC+3:	case CTRL_FUNC+4:
	case CTRL_FUNC+5:	case CTRL_FUNC+6:	case CTRL_FUNC+7:	case CTRL_FUNC+8:
	case CTRL_FUNC+9:	case CTRL_FUNC+10:	case CTRL_FUNC+11:	case CTRL_FUNC+12:
		(*m_axiscall)(AXI_FUNCKEY, wParam, lParam);
		break;
	case CTRL_CHANGE:
		SetPosition();
		Invalidate();
		break;
	case CTRL_TACNT:
		AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_SETACCOUNT, 0);
		break;
	default:
		break;
	}
	return 0;
}

CSize CDbar1::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect	rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	CRect	client;
	GetClientRect(client);

	/**
	if (m_type == TTYPE1)		m_height = HEIGHT_CTRLBAR1;
	else if (m_type == TTYPE2)	m_height = HEIGHT_CTRLBAR2;
	**/
	m_height = HEIGHT_CTRLBAR1;

	CSize	size;
	CRect	fRc;
	size.cx = 32767;
	size.cy = m_height;
	size.cy += (rect.Height());

	if (IsFloating())
	{
		GetDockingFrame()->GetParent()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}
	else
	{
		GetDockingFrame()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}

	return size;
}

void CDbar1::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI	state;

	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = AFX_IDW_STATUS_BAR;

	if (CControlBar::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;
	state.DoUpdate(pTarget, FALSE);
}

void CDbar1::init()
{
}

const char* strImg[] = { "HIDE", "설정", "인쇄", /**"홈페이지",**/ "잠금", "비번", "저장화면", "계산기", 
	/**"오디오",**/ 	"해외지수", "저장", "엑셀", };

void CDbar1::setDept(CString dept)
{
	m_dept = dept;
	SetPosition();
}

void CDbar1::make_Ctrl()
{
	CString		imgC, imgO, imgD, text, theme = CNPalette::GetTheme();
	DWORD dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	/**
	m_hide = new CNButton(CTRL_HIDE);
	if (!m_hide->Create(NULL, "숨기기", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_hide;
		m_hide = NULL;
	}
	else
	{
		m_hide->set_ToolTipText("숨기기");
		m_hide->set_Image(CAxButtonName(Format("mb_%s_barhide", theme)));
	}
	**/

	m_xcom = new CXcom(CTRL_SCREEN);
	if (!m_xcom->Create(NULL, "screenNo", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_xcom;
		m_xcom = NULL;
	}
	else	m_xcom->init(m_axiscall);
		
	m_searchS = new CNButton(CTRL_SEARCHS);
	m_searchS->set_Color(RGB(247, 251, 196));
	if (!m_searchS->Create(NULL, "search screen", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_searchS;
		m_searchS = NULL;
	}
	else
	{
		text.LoadString(ST_TEXT_SCREENSEARCH);
		m_searchS->set_ToolTipText(text);

		m_searchS->set_Image(CAxButtonName("TOOL_검색",true));
	}


	m_tacnt = new CNButton(CTRL_TACNT);
	m_tacnt->set_Color(RGB(247, 251, 196));
	if (!m_tacnt->Create(NULL, "Total Account", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_tacnt;
		m_tacnt = NULL;
	}
	else
	{
		text.LoadString(ST_TEXT_SCREENSEARCH);
		m_tacnt->set_ToolTipText("계좌연동");

		m_tacnt->set_Image(CAxButtonName("TOOL_계좌",false));
	}
	CString tooltip[] = { "도구 숨기기", "환경설정", "인쇄", "화면잠금", "계좌 비밀번호 저장", 
		"사용자 WorkSpace", "증권계산기", "해외지수 실시간티커", "이미지 저장", "엑셀시세연동", };

	for (int ii = 0; ii < MAX_FUNC; ii++)
	{
		m_func[ii] = new CNButton(CTRL_FUNC + ii);
		m_func[ii]->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));
		if (!m_func[ii]->Create(NULL, "main func", dwStyle, CRect(0, 0, 0, 0), this, -1))
		{
			delete m_func[ii];
			m_func[ii] = NULL;
		}
		else
		{
			m_func[ii]->set_ToolTipText(tooltip[ii]);
			m_func[ii]->set_Image(CAxButtonName(Format("TOOL_%s", strImg[ii])));
		}
		//**m_func[ii]->set_MaskingColor(Axis::maskColor);
		//**if (ii)	m_func[ii]->set_Embossing(theme);
	}

	m_xcomex = new CXcomex(this);
	if (!m_xcomex->Create(NULL, "toolgroup", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_xcomex;
		m_xcomex = NULL;
	}
	else	m_xcomex->init();

	m_register = new CNButton(CTRL_REGISTER);
	m_register->set_Color(RGB(247, 251, 196));
	if (!m_register->Create(NULL, "function key", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_register;
		m_register = NULL;
	}
	else
	{
		text.LoadString(ST_TEXT_REGTOOL_CUR);
		m_register->set_ToolTipText(text);
		m_register->set_Image(CAxButtonName("TOOL_화면추가"));
	}

	m_band = new CHBand(m_axiscall);
	if (!m_band->Create(NULL, "tool menu", WS_CHILD, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_band;
		m_band = NULL;
	}

	m_sband = new CSDIBand(m_axiscall, false);
	if (!m_sband->Create(NULL, "tool menu", WS_CHILD, CRect(0, 0, 0, 0), this, -2))
	{
		delete m_sband;
		m_sband = NULL;
	}

	m_banner.Create("banner", WS_CHILD, CRect(0, 0, 0, 0), this, -1);

	m_vband = new CSVband(m_axiscall);
	if (!m_vband->Create(NULL, "SVBAND", WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|WS_CHILD, CRect(0), this, -3))
	{
		delete m_vband;
		m_vband = NULL;
	}
	else
		m_vband->make_Ctrl(m_vMode);

	SetBandType();
	load_BandItem();
}

void CDbar1::Change_Skin(CString skinName)
{
	m_band->change_Skin(skinName);
	m_sband->change_Skin(skinName);
	m_vband->change_Skin(skinName);

	CString		imgC, imgO, imgD, text, theme = CNPalette::GetTheme();

	if (m_hide)
	{
		m_hide->set_Image(CAxButtonName(Format((m_type == TTYPE1 ? "%s_barhides": "%s_barhide"), //"mb_%s_barhides": "mb_%s_barhide"), 
			theme)));

	}

	if (m_searchS)
	{
		m_searchS->set_Image(CAxButtonName("TOOL_검색",true));
	}

	for (int ii = 0; ii < MAX_FUNC; ii++)
	{
		if (m_func[ii])
		{
			m_func[ii]->set_Bk(CNPalette::GetIndexColor(CLR_TOOL_BK));
			m_func[ii]->set_Image(CAxButtonName(Format("TOOL_%s", strImg[ii])));
			/**
			if (ii)	
				m_func[ii]->set_Embossing(theme);
			**/
		}		
	}

	if (m_register)
	{
		m_register->set_Image(CAxButtonName("TOOL_화면추가"));
	}

	m_xcom->Invalidate();
	m_xcomex->Invalidate();
	m_searchS->Invalidate();

	Invalidate();
}

void CDbar1::draw_BackGround(CDC* pDC)
{
	CRect	cRc;
	GetClientRect(cRc);

	BITMAP	bm;
	CDC	memDC;
	CBitmap* oldBmp = NULL;

	if (m_bmpBand->m_hObject)
	{
		m_bmpBand->GetBitmap(&bm);

		
		if (memDC.CreateCompatibleDC(pDC))
		{
			oldBmp = memDC.SelectObject(m_bmpBand);
			if (oldBmp != (CBitmap *) 0)
			{
				pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), bm.bmHeight, 
						&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
				memDC.SelectObject(oldBmp);
			}
		}
		
	}
	else
	{
		pDC->FillSolidRect(cRc, CNPalette::GetIndexColor(CLR_TOOL_BK));

		CPen	pen(PS_SOLID, 1, RGB(255, 255, 255));

		CPen	*pOldPen = pDC->SelectObject(&pen);
		pDC->MoveTo(0, 0);
		pDC->LineTo(cRc.right, 0);
		
		pDC->SelectObject(pOldPen);
	}
	/*
	Axis::DrawBandTerm(pDC, 5);
	if (m_funcX > 0)
		Axis::DrawBandTerm(pDC, m_funcX - buttonSize + 2);
	if (m_registerX > 0)
		Axis::DrawBandTerm(pDC, m_registerX + 3);
	*/
	/*
	if (m_bmpHome && m_bmpHome->m_hObject)
	{
		m_bmpHome->GetBitmap(&bm);
		
		if (!memDC.m_hDC)
			memDC.CreateCompatibleDC(pDC);
		oldBmp = memDC.SelectObject(m_bmpHome);
		pDC->BitBlt(m_xcomX, 2, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(oldBmp);
	}
	*/
	//draw_IconBack(pDC);
}

void CDbar1::draw_Button(bool push)
{

}

CRect CDbar1::get_GripperRect()
{
	CRect pRect;
	GetClientRect(pRect);
	pRect.OffsetRect(-pRect.left,-pRect.top);

	if (m_type == TTYPE1)
	{
		pRect.top = pRect.top + 1;
		pRect.right = pRect.left + 11;
	}
	else if (m_type == TTYPE2)
	{
		pRect.top = pRect.top + 1;
		pRect.right = pRect.left + 11;
	}

	return pRect;
}

void CDbar1::set_Background(COLORREF color)
{
	if (m_vband)
		m_vband->set_Background(color);
}

void CDbar1::set_Barheight(int height)
{
	m_height = height;
	(*m_axiscall)(AXI_RECALCLAYOUT, 0, 0);
}

void CDbar1::set_Maps(CString mapname)
{
	CString	mapN = m_menu->GetMapping(mapname);
	m_xcom->set_Maps(mapN);
}

void CDbar1::load_BandItem()
{
	if (!m_band || !m_sband)	return;

	m_band->removeALL();
	m_sband->removeALL();

	CProfile profile(pkUserTool);
	CString tmpS, cpFile(Format("%s\\%s\\usertool.ini", Axis::home, MTBLDIR));

	CopyFile(cpFile, profile.GetFileName(), TRUE);

	m_type = profile.GetInt(szUserSN, TOOLTYPE, TTYPE2);
	tmpS = profile.GetString(szUserSN, USEREN, "0, 0");

	int	idx = tmpS.Find(',');
	if (idx == -1)	idx = 0;
	else
	{
		tmpS = tmpS.Left(idx);
		idx = atoi(tmpS);
	}

	tmpS.Format("%02d", idx);

	CString buffer = profile.GetSectionLF(tmpS);
	int len = buffer.GetLength();

	if (len <= 0)
	{
		m_band->ReCalc();
		m_band->SetToolTipText("USER SPACE");
		m_band->Invalidate();

		m_sband->ReCalc();
		m_sband->SetToolTipText("USER SPACE");
		m_sband->Invalidate();

		buffer = profile.GetString(szUserSN, tmpS);
		m_xcomex->set_gName(buffer);

		return;
	}

	CString item, string(buffer);

	for (; !string.IsEmpty(); )
	{
		idx = string.Find('\n');
		if (idx == -1)	break;

		item  = string.Left(idx++);
		string = string.Mid(idx);

		idx = item.Find('=');
		if (idx == -1)	continue;
		
		m_band->ItemADD(item, IsList(item.Left(idx)));
		m_sband->ItemADD(item, IsList(item.Left(idx)));
	}

	m_band->ReCalc();
	m_band->SetToolTipText("USER SPACE");
	m_band->Invalidate();

	m_sband->ReCalc();
	m_sband->SetToolTipText("USER SPACE");
	m_sband->Invalidate();

	m_xcomex->set_gName(profile.GetString(szUserSN, tmpS));
}

void CDbar1::OnSize(UINT nType, int cx, int cy) 
{
	SetPosition();
	CDialogBar::OnSize(nType, cx, cy);
	if (GetStyle() & WS_BORDER)
		ModifyStyle(WS_BORDER, 0);
}

void CDbar1::set_Code(CString code)
{
	if (m_xcom)	m_xcom->set_Code(code);
}

void CDbar1::set_List(CStringArray& array)
{
	if (m_xcom)	m_xcom->set_List(array);
}

void CDbar1::add_History(CString dat)
{
	if (m_xcom)	m_xcom->add_History(dat);
}

BOOL CDbar1::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int	comm = LOWORD(wParam);
	if (comm >= COMM_BASE && comm < COMM_END)
	{
		comm -= COMM_BASE;
		switch (comm)
		{
		case 0:	(*m_axiscall)(AXI_EDITUSERTOOL, 0, 0);	break;
		default:change_Group(comm-1);			break;
		}

		return TRUE;
	}
	/*else if (comm >= 0 && comm < 100 && m_ar.GetSize() > comm)
	{
		CString	str = m_ar.GetAt(comm);
		(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *) str.Left(8).operator LPCTSTR());
		return TRUE;
	}*/
	else if (comm >= TOOL_MENUBASE && comm < TOOL_MENUEND)
	{
		comm -= TOOL_MENUBASE;
		(*m_axiscall)(AXI_FUNCKEY, MAKEWPARAM(CTRL_REGISTERALL, 0), (int) comm);
	}
	
	return CDialogBar::OnCommand(wParam, lParam);
}

void CDbar1::change_Group(int index)
{
	CProfile profile(pkUserTool);
	CString tmpS(profile.GetString(szUserSN, USEREN, "0, 0"));
	int pos  = tmpS.Find(',');
	CString	msg, title;
	if (pos == -1)
	{
		title.LoadString(STSM_TEXT_OK);
		msg.LoadString(ST_MSG_ERRORINFOFILE);
		::MessageBox(m_hWnd, msg, title, MB_ICONSTOP);
		return;
	}

	int tot = atoi(tmpS.Mid(pos+1));
	tmpS.Format("%d, %d", index, tot);
	profile.Write(szUserSN, USEREN, tmpS);

	load_BandItem();
}

CString CDbar1::screenInfo()
{
	CString		info;
	CPoint		point;
	
	CTrSearch	dlg(m_menu);
	GetCursorPos(&point);
	dlg.setPos(point);

	if (dlg.DoModal() == IDOK)
	{
		info = dlg.getTR();
	}
	return info;
}

CPoint CDbar1::GetPos(int index)
{
	CPoint	pos(0, 0);
	CRect	wRc;
	switch (index)
	{
	case 1:	// tr search pos
		m_searchS->GetWindowRect(wRc);
		pos = CPoint(wRc.left, wRc.bottom + 5);
		break;
	case CTRL_FUNC + 6:
		m_func[6]->GetWindowRect(wRc);
		pos = CPoint(wRc.left, wRc.bottom + 1);
		break;
	default:
		break;
	}
	return pos;
}

void CDbar1::OnLButtonDown(UINT nFlags, CPoint point) 
{

	CDialogBar::OnLButtonDown(nFlags, point);
}

void CDbar1::Refresh()
{
	CProfile profile(pkUserTool);
	m_type = profile.GetInt(szUserSN, TOOLTYPE, TTYPE2);

	SetBandType();
	load_BandItem();
}

void CDbar1::RefreshFunc()
{
	CProfile profile(pkUserTool);

	m_arFunc.RemoveAll();

	CString value(profile.GetString(szToolFunc, "Func", "0, 1, 2, 3, 5, 6"));

	for (int ii = 0; !value.IsEmpty(); ii++)
	{
		CString func = Parser(value, ",");	
		Trim(func);
		if (func.IsEmpty())	continue;

		m_arFunc.Add(atoi(func));
	}
	m_nFunc	= m_arFunc.GetSize();
	m_FuncType = profile.GetInt(szToolFunc, "Type", 1);

	SetPosition();
}

bool CDbar1::IsList(CString key)
{
	if (key.Find('#') != -1)
		return true;
	return false;
}

//mapN=툴팁;단축명
void CDbar1::AddItem(CString dat)
{
	if (!m_band || !m_sband)	return;
	m_band->ItemADD(dat);
	m_sband->ItemADD(dat);
}

void CDbar1::ShowMenu()
{
	CRect	wRc;

	m_register->GetWindowRect(wRc);
	CPoint	point(wRc.left, wRc.bottom);

	CMenu		menu;
	//ClientToScreen(&xpoint);
	menu.CreatePopupMenu();

	CString	text;
	text.LoadString(ST_TEXT_REGTOOL_ALLMAP);
	menu.AppendMenu(MF_STRING, TOOL_MENUBASE, text);
	text.LoadString(ST_TEXT_SAVE_WROKSPACE);
	menu.AppendMenu(MF_STRING, TOOL_MENUBASE+1, text);

	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, this);
	menu.DestroyMenu();
}

void CDbar1::set_Owner()
{
	m_xcom->set_Owner();
}

#define FUNC_GAP	9
#define FUNC_BUTTON_GAP		3

void CDbar1::SetPosition()
{
	if (!m_band || !m_band->GetSafeHwnd())
		return;

	if (!m_sband || !m_band->GetSafeHwnd())
		return;

	CDC*	pDC = GetDC();

	CRect	cRc, xRc, wRc, tRc, mRc;
	CSize	size = pDC->GetTextExtent(_T("$$$$$$"));
	CFont*	font = pDC->SelectObject(&m_font);
	int	mode = pDC->SetBkMode(TRANSPARENT);
	
	GetClientRect(cRc);
	

	tRc = get_GripperRect();
	tRc.OffsetRect(72,0);
	if (m_hide)
		m_hide->MoveWindow(tRc);

	if (m_type == TTYPE1)
		m_type = TTYPE2;
	if (m_type == TTYPE1)
	{
		
		tRc.SetRect(tRc.right , cRc.top, tRc.right + 3, cRc.bottom);
		tRc.DeflateRect(0, 6, 0, 2);
		
		if (m_xcom)
		{
			xRc.SetRect(tRc.right+3, tRc.top, tRc.right+GAP+size.cx+COMBTN_WIDTH, tRc.bottom);
			int	gap = 0;
			if (xRc.Height() != XCOM_HEIGHT)
			{
				gap = (xRc.Height() - XCOM_HEIGHT)/2;
				xRc.DeflateRect(0, gap);
			}
			
			m_xcom->MoveWindow(xRc);
			m_xcom->changepos();
			m_xcomX = xRc.left;
			
			wRc.SetRect(xRc.right-1/*+GAP*/, xRc.top, xRc.right/*+GAP*/+buttonSize-1, xRc.top + xRc.Height());
			//wRc.OffsetRect(-20,0);
			m_searchS->MoveWindow(wRc);		
		}
	
		if (m_func[0])
		{
			wRc.OffsetRect(FUNC_GAP, 0);
			//wRc.right = wRc.left + TOOL_HIDE_WIDTH;			

			//m_func[0]->MoveWindow(wRc);
		}

		if (m_xcomex)
		{
			wRc.SetRect(wRc.right+GAP, wRc.top, wRc.right+GAP+WIDTH_TOOLGROUP, wRc.top+wRc.Height());
			wRc.OffsetRect(GAP*2, 0);
			int	gap = 0;
			if (wRc.Height() != XCOM_HEIGHT)
			{
				gap = (wRc.Height() - XCOM_HEIGHT)/2;
				wRc.DeflateRect(0, gap);
			}
			m_xcomex->MoveWindow(wRc);
			m_xcomex->changepos();
			
			wRc.SetRect(wRc.right+GAP, wRc.top, wRc.right+GAP+23, wRc.top+wRc.Height());
			m_register->MoveWindow(wRc);	
			m_registerX = wRc.right;
			
			mRc.SetRect(wRc.right+GAP*3, cRc.top, cRc.right /*- m_vband->GetBandWidth().cx*/, cRc.bottom);
			
			mRc.right -= (m_nFunc*(buttonSize+FUNC_BUTTON_GAP)+8);
			m_band->MoveWindow(mRc);
			m_sband->MoveWindow(mRc);

			//mRc.SetRect(mRc.right, mRc.top, cRc.right, mRc.bottom);
			//mRc.SetRect(mRc.right, mRc.top, cRc.right, mRc.bottom);
			/**if (mRc.left < wRc.right + GAP)
				mRc.left = wRc.right + GAP;
			**/
			CRect vRc; vRc.SetRect(mRc.right, mRc.top, cRc.right, mRc.bottom);

			m_vband->MoveWindow(vRc);
		}
		if (m_func[1])
		{
			for (int ii = 1; ii < MAX_FUNC; ii++)
				m_func[ii]->ShowWindow(SW_HIDE);
			wRc.left = mRc.right;
			if (m_bFunc)
			{
				wRc.OffsetRect(5, 0);
				wRc.right = wRc.left + buttonSize;
				
				for (int ii = 0; ii < m_nFunc; ii++)
				{	
					int index = m_arFunc.GetAt(ii) + 1;
					
					m_func[index]->MoveWindow(wRc);
					m_func[index]->ShowWindow(SW_SHOW);
					wRc.OffsetRect(buttonSize + FUNC_BUTTON_GAP, 0);

					m_funcX = wRc.right;
				}
				wRc.OffsetRect(-buttonSize, 0);
			}
		}
	}
	else if (m_type == TTYPE2)
	{
		
		int cy = TOPMARGIN;

		tRc.SetRect(tRc.right + 3, cRc.top, tRc.right + 3, cRc.bottom);
		tRc.DeflateRect(0, TOPMARGIN);
		
		if (m_xcom)
		{
			xRc.SetRect(tRc.right, tRc.top, tRc.right+GAP+size.cx+COMBTN_WIDTH, tRc.bottom);
			int	gap = 0;
			if (xRc.Height() != XCOM_HEIGHT)
			{
				gap = (xRc.Height() - XCOM_HEIGHT)/2;
				xRc.DeflateRect(0, gap);
			}
			xRc.top = cy;
			xRc.bottom = cy + XCOM_HEIGHT;
			m_xcom->MoveWindow(xRc);
			m_xcom->changepos();

			m_xcomX = xRc.left;
			
			wRc.SetRect(xRc.right-1/*+GAP*/, xRc.top, xRc.right-1/*+GAP*/+buttonSize, xRc.top+xRc.Height());
			//wRc.OffsetRect(-10,0);
			m_searchS->MoveWindow(wRc);		

			CString	Path;
			Path.Format("%s\\%s\\ACCNTDEPT.INI", Axis::home, "tab");
			
			char readB[1024];
			int readL;
			
			readL = GetPrivateProfileString("ACCNTDEPT","DEPT","813",readB,sizeof(readB),Path);
			
			CString tDept(readB,readL);
			tDept.TrimLeft();tDept.TrimRight();

			if (m_dept=="813" || m_dept==tDept)
			{
				wRc.SetRect(wRc.right-1+GAP, xRc.top, wRc.right-1+GAP+buttonSize, xRc.top+xRc.Height());
				m_tacnt->MoveWindow(wRc);
			}
		}
		
		if (m_func[0])
		{
			wRc.OffsetRect(3/*FUNC_GAP*/, 0);
			//wRc.right = wRc.left + TOOL_HIDE_WIDTH;			

			//m_func[0]->MoveWindow(wRc);
		}
		
		
		if (m_xcomex)
		{
			wRc.SetRect(wRc.right/*+GAP*/, wRc.top, 
				wRc.right/*+GAP*/+WIDTH_TOOLGROUP, wRc.top+wRc.Height());
			//wRc.OffsetRect(GAP, 0);
			int	gap = 0;
			
			if (wRc.Height() != XCOM_HEIGHT)
			{
				gap = (wRc.Height() - XCOM_HEIGHT)/2;
				wRc.DeflateRect(0, gap);
			}
			
			m_xcomex->MoveWindow(wRc);
			m_xcomex->changepos();
			
			wRc.SetRect(wRc.right+GAP, wRc.top, wRc.right+GAP+23, wRc.top+wRc.Height());
			m_register->MoveWindow(wRc);	

			m_registerX = wRc.right;
			
			mRc.SetRect(wRc.right+GAP*3, cRc.top, cRc.right /*- m_vband->GetBandWidth().cx*/, cRc.bottom);
			mRc.right -= (m_nFunc*(buttonSize+FUNC_BUTTON_GAP)+8);
			m_band->MoveWindow(mRc);

			m_sband->MoveWindow(mRc);

			//mRc.SetRect(mRc.right, mRc.top, cRc.right, mRc.bottom);
			//mRc.SetRect(mRc.right, mRc.top, cRc.right, mRc.bottom);
			/**if (mRc.left < wRc.right + GAP)
				mRc.left = wRc.right + GAP;
			**/
			CRect vRc; vRc.SetRect(mRc.right, mRc.top, cRc.right, mRc.bottom-1);

			m_vband->MoveWindow(vRc);
			
		}
		
		
		if (m_func[1])
		{
			for (int ii = 1; ii < MAX_FUNC; ii++)
				m_func[ii]->ShowWindow(SW_HIDE);
			wRc.left = mRc.right;
			//draw_IconBack(pDC, CRect(wRc.left, mRc.top, cRc.right, mRc.bottom));
			//tool_icon = CRect(wRc.left, mRc.top, cRc.right, mRc.bottom);
			if (m_bFunc)
			{
				if (m_FuncType == 1)
				{
					wRc.OffsetRect(5, 1);
					wRc.right = wRc.left + buttonSize;
					
					for (int ii = 0; ii < m_nFunc; ii++)
					{	
						int index = m_arFunc.GetAt(ii) + 1;
						
						m_func[index]->MoveWindow(wRc);
						m_func[index]->ShowWindow(SW_SHOW);
						wRc.OffsetRect(buttonSize + FUNC_BUTTON_GAP, 0);
						m_funcX = wRc.right;
					}
					wRc.OffsetRect(-buttonSize, 0);
				}
				else
				{
					mRc.OffsetRect(18, 0);
					mRc.right = mRc.left + buttonSize;

					mRc.SetRect(wRc.left, cRc.top + 2, wRc.right, cRc.top + 2 + buttonSize);
					int quotient = (int)(m_nFunc / 2.0 + 0.5);
					
					for (int ii = 0; ii < m_nFunc; ii++)
					{	
						int index = m_arFunc.GetAt(ii) + 1;
						
						m_func[index]->MoveWindow(mRc);
						m_func[index]->ShowWindow(SW_SHOW);

						if (ii + 1 == quotient)
						{
							tRc = mRc;
							mRc.OffsetRect(-buttonSize * ii, buttonSize);
							wRc.SetRect(tRc.left, wRc.top, tRc.right, wRc.bottom);
						}
						else
							mRc.OffsetRect(buttonSize + FUNC_BUTTON_GAP, 0);
					}
				}
			}
		}
		
		
	}

	pDC->SelectObject(font);
	pDC->SetBkMode(mode);

	draw_BackGround(pDC);
	ReleaseDC(pDC);
}

bool CDbar1::IsInputFocus()
{
	return m_xcom->IsInputFocus();
}

void CDbar1::SetBandType()
{
	if (m_type == TTYPE1)
		m_type = TTYPE2;
	if (m_type == TTYPE1)
	{
		m_band->ShowWindow(SW_HIDE);
		m_sband->ShowWindow(SW_SHOW);
	}
	else if (m_type == TTYPE2)
	{
		m_band->ShowWindow(SW_SHOW);
		m_sband->ShowWindow(SW_HIDE);
	}	

	m_vband->SetBandType(m_type);
	(*m_axiscall)(AXI_RECALCLAYOUT, 0, 0);

	Change_Skin(CNPalette::GetTheme());

	Invalidate();
}

void CDbar1::SetBandType(int type)
{
	m_type = type;

	CProfile profile(pkUserTool);
	profile.Write(szUserSN, TOOLTYPE, Format("%d", m_type));

	SetBandType();
}

CString CDbar1::Parser(CString &srcstr, CString substr)
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

void CDbar1::SetHome()
{
	if (m_xcom)
		m_xcom->set_Home();
}

int CDbar1::GetHeight()
{
	return m_height;
}



