// Smain.cpp : implementation file
// SDI 모드의 상단 기본툴바

#include "stdafx.h"
#include "Smain.h"
#include "xcom.h"
#include "xcomex.h"
#include "resource.h"
#include "smdefine.h"
#include "nbutton.h"
//#include "SDIBand.h"
#include "hband.h"
#include "SVband.h"
#include "trsearch.h"
#include "tmenu.h"
#include "NPalette.h"

#include "../../h/axisvar.h"
#include "../../axis/axmsg.hxx"

#define GAP	3
#define IMGWH	16
#define SMENUW	44

#define	TOOL_MENUBASE	6000
#define	TOOL_MENUEND	6100

#define CTRL_CHANGE	0x58

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int buttonSize = 22;

/////////////////////////////////////////////////////////////////////////////
// CSmain

CSmain::CSmain(bool (*callback)(int, WPARAM, LPARAM))
{
	m_axiscall	= callback;
	m_height	= HEIGHT_CTRLBAR1-1;

	m_xcom		= NULL;
	m_searchS	= NULL;
	m_smenu		= NULL;
	m_xcomex	= NULL;
	m_band		= NULL;
	m_vband		= NULL;
	m_vsN		= V_SCREEN1;

	m_bmpBand = Axis::GetSkinBitmap("BAND");
	m_bmpHome = Axis::GetBitmap("TEXT_SCREEN");

	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);

	CProfile profile(pkUserTool);
	m_vMode = profile.GetInt(szUserSN, "stoolex");
}

CSmain::~CSmain()
{
#ifdef DF_USESTL_SMAIN
	if (m_vband)
	{
		CProfile profile(pkUserTool);
		m_vMode = m_vband->GetBandMode();
		profile.Write(szUserSN, "stoolex", Format("%d", m_vMode));
	}
#else
	if (m_xcom)		delete m_xcom;
	if (m_searchS)		delete m_searchS;
	if (m_smenu)		delete m_smenu;
	if (m_xcomex)		delete m_xcomex;
	if (m_band)		delete m_band;

	if (m_vband)
	{
		CProfile profile(pkUserTool);
		m_vMode = m_vband->GetBandMode();
		profile.Write(szUserSN, "stoolex", Format("%d", m_vMode));

		delete m_vband;
	}
#endif

	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSmain, CDialogBar)
	//{{AFX_MSG_MAP(CSmain)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmain message handlers
void CSmain::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI	state;

	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = AFX_IDW_STATUS_BAR;

	if (CControlBar::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;
	state.DoUpdate(pTarget, FALSE);
}

CSize CSmain::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect	rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	CRect	client;
	GetClientRect(client);

	CSize	size;
	CRect	fRc;

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

void CSmain::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC	 memDC;
	CRect	 cRc;

	GetClientRect(cRc);
	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap	bitmap;
		bitmap.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		CBitmap* oldbuffBitmap = memDC.SelectObject(&bitmap);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			drawBackground(&memDC);
			drawEtc(&memDC);
			
			dc.BitBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
}

void CSmain::OnDestroy() 
{
	XMSG();
	CDialogBar::OnDestroy();
}

void CSmain::OnSize(UINT nType, int cx, int cy) 
{
	SetPosition();
	CDialogBar::OnSize(nType, cx, cy);
}

void CSmain::OnLButtonDown(UINT nFlags, CPoint point) 
{

	CDialogBar::OnLButtonDown(nFlags, point);
}

LONG CSmain::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case CTRL_SEARCHS:
		{
			CTrSearch	dlg(m_menu, m_axiscall);
			if (dlg.DoModal() == IDOK)
				(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(const char *)dlg.getTR());
		}
		break;
	case CTRL_SDIMENU:
		{
			CRect	sRc;
			m_smenu->GetWindowRect(sRc);
			m_menu->ShowFullMenu(CPoint(sRc.right, sRc.bottom));
		}
		break;
	case CTRL_CHANGE:
		SetPosition();
		Invalidate();
	}
	return 0;
}

void CSmain::drawBackground(CDC* pDC)
{
	CRect	cRc;
	GetClientRect(cRc);

	BITMAP	bm{};
	CDC	memDC;
	CBitmap* oldBmp = NULL;

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
	/*
	if (m_bmpHome && m_bmpHome->m_hObject)
	{
		m_bmpHome->GetBitmap(&bm);
			
		if (!memDC.m_hDC)
			memDC.CreateCompatibleDC(pDC);
		oldBmp = memDC.SelectObject(m_bmpHome);
		pDC->BitBlt(5, 2, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(oldBmp);
	}
	*/
}

void CSmain::drawGripper(CDC* pDC)
{
	CRect	gRc = GetGRect();
	pDC->Draw3dRect(gRc, GRIP_LT, GRIP_RB);
}

void CSmain::drawEtc(CDC* pDC)
{
	CRect	wRc;
	const CRect iRc;

	m_searchS->GetWindowRect(wRc);
	ScreenToClient(&wRc);

	wRc.OffsetRect(GAP*2, 0);
//	drawSeperate(pDC, wRc);
}

void CSmain::drawSeperate(CDC* pDC, CRect rc)
{
	CRect	sRc, cRc;

	GetClientRect(cRc);
	sRc.SetRect(rc.right+1, cRc.top + 3, rc.right+3, cRc.bottom - 2);
	pDC->Draw3dRect(sRc, GRIP_RB, GRIP_LT);
}

CRect CSmain::GetGRect()
{
	CRect pRect;
	GetClientRect(pRect);
	pRect.OffsetRect(-pRect.left,-pRect.top);

	pRect.DeflateRect(4, 3, 4, 2);
	pRect.right = pRect.left + 3;
	return pRect;
}

void CSmain::make_Ctrl()
{
#ifdef DF_USESTL_SMAIN
	CString		imgC, imgO, imgD, text;

	m_xcom = std::make_unique<CXcom>(CTRL_SCREEN);
	const DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	if (!m_xcom->Create(NULL, "sdi screenNo", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_xcom = NULL;
	else	m_xcom->init(m_axiscall);

	m_searchS = std::make_unique<CNButton>(CTRL_SEARCHS);
	m_searchS->set_Color(RGB(247, 251, 196));
	if (!m_searchS->Create(NULL, "search screen", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_searchS = NULL;
	else
	{
		text.LoadString(ST_TEXT_SCREENSEARCH);
		m_searchS->set_ToolTipText(text);
		m_searchS->set_Image(CAxButtonName("TOOL_검색", true));
	}

	m_smenu = std::make_unique<CNButton>(CTRL_SDIMENU);
	m_smenu->set_Color(RGB(247, 251, 196));
	if (!m_smenu->Create(NULL, "SDI메뉴", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_smenu = NULL;
	else
	{
		m_smenu->set_ToolTipText("메뉴");
		m_smenu->set_Image(CAxButtonName("SDI_메뉴"));
	}

	m_xcomex = std::make_unique<CXcomex>(this);
	if (!m_xcomex->Create(NULL, "sdi toolgroup", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_xcomex = NULL;
	else	m_xcomex->init();

	m_band = std::make_unique<CHBand>(m_axiscall);
	if (!m_band->Create(NULL, "SDI TOOLMENU", WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, -1))
		m_band = NULL;
	else	load_BandItem();

	m_vband = std::make_unique<CSVband>(m_axiscall, true);
	if (!m_vband->Create(NULL, "SVBAND", WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, -3))
		m_vband = NULL;
	else
		m_vband->make_Ctrl(m_vMode);

	m_vband->SetBandType(TTYPE1);
#else
	CString		imgC, imgO, imgD, text;

	m_xcom = new CXcom(CTRL_SCREEN);
	const DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	if (!m_xcom->Create(NULL, "sdi screenNo", dwStyle, CRect(0, 0, 0, 0), this, -1))
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
		m_searchS->set_Image(CAxButtonName("TOOL_검색", true));
	}

	m_smenu = new CNButton(CTRL_SDIMENU);
	m_smenu->set_Color(RGB(247, 251, 196));
	if (!m_smenu->Create(NULL, "SDI메뉴", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_smenu;
		m_smenu = NULL;
	}
	else
	{
		m_smenu->set_ToolTipText("메뉴");
		m_smenu->set_Image(CAxButtonName("SDI_메뉴"));
	}

	m_xcomex = new CXcomex(this);
	if (!m_xcomex->Create(NULL, "sdi toolgroup", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_xcomex;
		m_xcomex = NULL;
	}
	else	m_xcomex->init();

	//m_band = new CSDIBand(m_axiscall, true);
	m_band = new CHBand(m_axiscall);
	if (!m_band->Create(NULL, "SDI TOOLMENU", WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_band;
		m_band = NULL;
	}
	else	load_BandItem();

	m_vband = new CSVband(m_axiscall, true);
	if (!m_vband->Create(NULL, "SVBAND", WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | WS_CHILD, CRect(0, 0, 0, 0), this, -3))
	{
		delete m_vband;
		m_vband = NULL;
	}
	else
		m_vband->make_Ctrl(m_vMode);

	m_vband->SetBandType(TTYPE1);
#endif
}

void CSmain::SetPosition()
{
	if (!m_band || !m_band->GetSafeHwnd())
		return;

	CDC*	pDC = GetDC();

	CRect	cRc, xRc, wRc, tRc, mRc;
	const CSize	size = pDC->GetTextExtent(_T("$$$$$$"));
	CFont*	font = pDC->SelectObject(&m_font);
	const int	mode = pDC->SetBkMode(TRANSPARENT);
	
	
	GetClientRect(cRc);
	tRc.SetRect(3, cRc.top + 4, 3, cRc.bottom - 4);
	tRc.OffsetRect(5, 2);

	if (m_xcom)
	{
		xRc.SetRect(tRc.right, tRc.top, tRc.right+GAP+size.cx+COMBTN_WIDTH, tRc.bottom);
		int	gap = 0;
		if (xRc.Height() != XCOM_HEIGHT)
		{
			gap = (xRc.Height() - XCOM_HEIGHT)/2;
			xRc.DeflateRect(0, gap);
		}
		m_xcom->MoveWindow(xRc);
		m_xcom->changepos();

		wRc.SetRect(xRc.right/*+GAP*/, xRc.top, xRc.right /*+ GAP*/ + buttonSize, xRc.top+xRc.Height());
		//wRc.SetRect(xRc.right+GAP, xRc.top, xRc.right + GAP + buttonSize, xRc.top+xRc.Height());
		m_searchS->MoveWindow(wRc);	
		wRc.OffsetRect(GAP + wRc.Width(), 0);
		wRc.OffsetRect(GAP, 0);	
		wRc.right = wRc.left + SMENUW;
		m_smenu->MoveWindow(wRc);
	}

	if (m_xcomex)
	{
		wRc.SetRect(wRc.right, wRc.top, 
				wRc.right+WIDTH_TOOLGROUP, wRc.top+wRc.Height());
		wRc.OffsetRect(GAP*2, 0);
		int	gap = 0;
		if (wRc.Height() != XCOM_HEIGHT)
		{
			gap = (wRc.Height() - XCOM_HEIGHT)/2;
			wRc.DeflateRect(0, gap);
		}
		m_xcomex->MoveWindow(wRc);
		m_xcomex->changepos();
		
		mRc.SetRect(wRc.right+GAP*3, cRc.top, cRc.right - m_vband->GetBandWidth().cx, cRc.bottom);
		m_band->MoveWindow(mRc);
		
		mRc.SetRect(mRc.right + 1, mRc.top, cRc.right, mRc.bottom);
		if (mRc.left < wRc.right + GAP)
			mRc.left = wRc.right + GAP;
		m_vband->MoveWindow(mRc);
	}

	pDC->SelectObject(font);
	pDC->SetBkMode(mode);
	ReleaseDC(pDC);
}

void CSmain::load_BandItem()
{
	if (!m_band)	return;

	m_band->removeALL();

	CString	cpfile, tmpS;

	CProfile profile(pkUserTool);
	cpfile.Format("%s\\%s\\usertool.ini", Axis::home, MTBLDIR);
	CopyFile(cpfile, profile.GetFileName(), TRUE);

	tmpS = profile.GetString(szUserSN, USEREN, "0, 0");
	int	idx = tmpS.Find(',');
	if (idx == -1)	
		idx = 0;
	else
	{
		tmpS = tmpS.Left(idx);
		idx = atoi(tmpS);
	}

	tmpS.Format("%02d", idx);

	CString string(profile.GetSectionLF(tmpS));
	const int len = string.GetLength();
	
	if (len<= 0)
	{
		m_band->ReCalc();
		m_band->SetToolTipText("USER SPACE");
		m_band->Invalidate();
		m_xcomex->set_gName(profile.GetString(szUserSN, tmpS));
		return;
	}

	CString item;
	for (; !string.IsEmpty(); )
	{
		idx = string.Find('\n');
		if (idx == -1)	break;

		item  = string.Left(idx++);
		string = string.Mid(idx);

		idx = item.Find('=');
		if (idx == -1)	continue;

		m_band->ItemADD(item, IsList(item.Left(idx)));
	}

	m_band->ReCalc();
	m_band->SetToolTipText("USER SPACE");
	m_band->Invalidate();

	m_xcomex->set_gName(profile.GetString(szUserSN, tmpS));
}

bool CSmain::IsList(CString key)
{
	if (key.Find('#') != -1)
		return true;
	return false;
}

void CSmain::Change_Skin(CString skinName)
{
	m_band->change_Skin(skinName);
	m_vband->change_Skin(skinName);

	Invalidate();
}

void CSmain::add_History(CString dat)
{
	if (m_xcom)	m_xcom->add_History(dat);
}

void CSmain::change_VirtualScreen(int vsN)
{
	m_vsN = vsN;

	//**m_vband->change_VirtualScreen(m_vsN);

	Invalidate(FALSE);
}

void CSmain::Refresh()
{
	load_BandItem();
}

void CSmain::set_List(const CStringArray& array)
{
	if (m_xcom)	m_xcom->set_List(array);
}

BOOL CSmain::OnCommand(WPARAM wParam, LPARAM lParam) 
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
	else if (comm >= TOOL_MENUBASE && comm < TOOL_MENUEND)
	{
		comm -= TOOL_MENUBASE;
		(*m_axiscall)(AXI_FUNCKEY, MAKEWPARAM(CTRL_REGISTERALL, 0), (int) comm);
	}	
	return CDialogBar::OnCommand(wParam, lParam);
}

void CSmain::change_Group(int index)
{
	CString		msg, title;

	CProfile profile(pkUserTool);
	CString tmpS(profile.GetString(szUserSN, USEREN, "0, 0"));
	const int pos  = tmpS.Find(',');
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

bool CSmain::IsInputFocus()
{
	return m_xcom->IsInputFocus();
}

void CSmain::SetHome()
{
	if (m_xcom)
		m_xcom->set_Home();
}