// Smain.cpp : implementation file
//

#include "stdafx.h"
#include "Smain.h"
#include "xcom.h"
#include "xcomex.h"
#include "resource.h"
#include "smdefine.h"
#include "nbutton.h"
#include "SDIBand.h"
#include "trsearch.h"
#include "tmenu.h"

#include "../../h/axisvar.h"
#include "../../axis/axmsg.hxx"

#define GAP	3
#define IMGWH	16
#define SMENUW	45

#define	TOOL_MENUBASE	6000
#define	TOOL_MENUEND	6100

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmain

CSmain::CSmain(bool (*callback)(int, WPARAM, LPARAM), CString home, CString userN)
{
	m_axiscall	= callback;
	m_home		= home;
	m_userN		= userN;
	m_height	= HEIGHT_CTRLBAR1;

	m_xcom		= NULL;
	m_searchS	= NULL;
	m_smenu		= NULL;
	m_xcomex	= NULL;
	m_band		= NULL;
	m_vsN		= V_SCREEN1;

	m_bkIMG.LoadBitmap(IDB_INFOBACK);

	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);
}

CSmain::~CSmain()
{
	if (m_xcom)		delete m_xcom;
	if (m_searchS)		delete m_searchS;
	if (m_smenu)		delete m_smenu;
	if (m_xcomex)		delete m_xcomex;
	if (m_band)		delete m_band;

	m_font.DeleteObject();
	m_bkIMG.DeleteObject();
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

LRESULT CSmain::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case CTRL_SEARCHS:
		{
			CTrSearch	dlg(m_menu, m_home, m_axiscall);
			if (dlg.DoModal() == IDOK)
				(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *)dlg.getTR().operator LPCTSTR());
		}
		break;
	case CTRL_SDIMENU:
		{
			CRect	sRc;
			m_smenu->GetWindowRect(sRc);
			m_menu->ShowFullMenu(CPoint(sRc.right, sRc.bottom));
		}
		break;
	}
	return 0;
}

void CSmain::drawBackground(CDC* pDC)
{
#if 1
	CRect	cRc;
	GetClientRect(cRc);
	pDC->FillSolidRect(cRc, COLOR_FRAME);

	CPen	pen1(PS_SOLID, 1, RGB(128, 128, 128));
	CPen	pen2(PS_SOLID, 1, RGB(255, 255, 255));

	CPen	*pOldPen = pDC->SelectObject(&pen1);
	pDC->MoveTo(0, 0);
	pDC->LineTo(cRc.right, 0);
	
	pDC->SelectObject(&pen2);
	pDC->MoveTo(0, 1);
	pDC->LineTo(cRc.right, 1);

	pDC->SelectObject(pOldPen);
#else
	CDC	 memDC;
	CRect	 cRc;

	GetClientRect(cRc);
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* oldbuffBitmap = memDC.SelectObject(&m_bkIMG);
		if (oldbuffBitmap != (CBitmap *) 0)
		{
			BITMAP	 bm;
			m_bkIMG.GetBitmap(&bm);
			pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), 
					&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
			memDC.SelectObject(oldbuffBitmap);
		}
		memDC.DeleteDC();
	}
#endif
}

void CSmain::drawGripper(CDC* pDC)
{
	CRect	gRc = GetGRect();
	pDC->Draw3dRect(gRc, GRIP_LT, GRIP_RB);
}

void CSmain::drawEtc(CDC* pDC)
{
	CRect	wRc, iRc;

	m_searchS->GetWindowRect(wRc);
	ScreenToClient(&wRc);

	wRc.OffsetRect(GAP*2, 0);
	drawSeperate(pDC, wRc);
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
	CString		imgC, imgO, imgD, text;

	m_xcom = new CXcom(CTRL_SCREEN);
	DWORD dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	if (!m_xcom->Create(NULL, "sdi screenNo", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_xcom;
		m_xcom = NULL;
	}
	else	m_xcom->init(m_axiscall, m_home);
	
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
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "mb_screensearch");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "mb_screensearch_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "mb_screensearch_dn");
		m_searchS->set_Image(imgC, imgO, imgD);
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
		imgC.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "smenu");
		imgO.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "smenu_en");
		imgD.Format("%s\\%s\\%s.bmp", m_home, IMAGEDIR, "smenu_dn");
		m_smenu->set_Image(imgC, imgO, imgD);
	}
	
	m_xcomex = new CXcomex(m_home, this);
	if (!m_xcomex->Create(NULL, "sdi toolgroup", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_xcomex;
		m_xcomex = NULL;
	}
	else	m_xcomex->init();

	m_band = new CSDIBand(m_axiscall, m_home, true);
	if (!m_band->Create(NULL, "SDI TOOLMENU", WS_VISIBLE|WS_CHILD, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_band;
		m_band = NULL;
	}
	else	load_BandItem();
}

void CSmain::SetPosition()
{
	if (!m_band || !m_band->GetSafeHwnd())
		return;

	CDC*	pDC = GetDC();

	CRect	cRc, xRc, wRc, tRc, iRc;
	CSize	size = pDC->GetTextExtent(_T("$$$$$$"));
	CFont*	font = pDC->SelectObject(&m_font);
	int	mode = pDC->SetBkMode(TRANSPARENT);
	
	
	GetClientRect(cRc);
	tRc.SetRect(3, cRc.top + 5, 3, cRc.bottom - 3);

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

		wRc.SetRect(xRc.right+GAP, xRc.top, xRc.right+GAP+xRc.Height(), xRc.top+xRc.Height());
		m_searchS->MoveWindow(wRc);	wRc.OffsetRect(GAP*4 + wRc.Width(), 0);
		wRc.OffsetRect(GAP, 0);	wRc.right = wRc.left + SMENUW;
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
		
		wRc.SetRect(wRc.right + GAP*2, cRc.top, cRc.right, cRc.bottom);
		wRc.bottom += 3;
		m_band->MoveWindow(wRc);
	}

	pDC->SelectObject(font);
	pDC->SetBkMode(mode);
	ReleaseDC(pDC);
}

void CSmain::load_BandItem()
{
	if (!m_band)	return;

	m_band->removeALL();

	char	ssb[1024*4];
	CString	file, cpfile, tmpS;
	CString user = AfxGetApp()->GetProfileString(WORKSTATION, SIGNONID);
	int pos  = user.Find('|');
	if (pos < 0)	
		user = _T("Anonymous");
	else		
		user = user.Mid(pos+1);

	if (user.IsEmpty())	user = _T("Anonymous");

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, user);
	cpfile.Format("%s\\%s\\usertool.ini", m_home, MTBLDIR);
	CopyFile(cpfile, file, TRUE);

	DWORD ssL = GetPrivateProfileString(USERSN, USEREN, "0, 0", ssb, sizeof(ssb), file);
	tmpS = CString(ssb, ssL);
	int	idx = tmpS.Find(',');
	if (idx == -1)	idx = 0;
	else
	{
		tmpS = tmpS.Left(idx);
		idx = atoi(tmpS);
	}

	tmpS.Format("%02d", idx);
	ssL = GetPrivateProfileSection(tmpS, ssb, sizeof(ssb), file);
	if (ssL <= 0)
	{
		m_band->ReCalc();
		m_band->SetToolTipText("USER SPACE");
		m_band->Invalidate();
		ssL = GetPrivateProfileString(USERSN, tmpS, "", ssb, sizeof(ssb), file);
		if (ssL <= 0)	m_xcomex->set_gName(_T(""));
		else		m_xcomex->set_gName(ssb);
		return;
	}

	CString item, string = CString(ssb, ssL);
	for (; !string.IsEmpty(); )
	{
		idx = string.Find('\0');
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

	ssL = GetPrivateProfileString(USERSN, tmpS, "", ssb, sizeof(ssb), file);
	if (ssL <= 0)	m_xcomex->set_gName(_T(""));
	else		m_xcomex->set_gName(ssb);

	
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
}

void CSmain::add_History(CString dat)
{
	if (m_xcom)	m_xcom->add_History(dat);
}

void CSmain::change_VirtualScreen(int vsN)
{
	m_vsN = vsN;
}

void CSmain::SetHome()
{
	m_xcom->set_Home();
}

void CSmain::Refresh()
{
	load_BandItem();
}

void CSmain::set_List(CStringArray& array)
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
	char		buf[128];
	CString		tmpS = _T("");
	CString		file, msg, title;

	CString user = AfxGetApp()->GetProfileString(WORKSTATION, SIGNONID);
	int tot, pos  = user.Find('|');
	if (pos < 0)	
		user = _T("Anonymous");
	else		
		user = user.Mid(pos+1);

	if (user.IsEmpty())	
		user = _T("Anonymous");

	file.Format("%s\\%s\\%s\\usertool.ini", m_home, USRDIR, user);
	DWORD dw = GetPrivateProfileString(USERSN, USEREN, "0, 0", buf, sizeof(buf), file);
	tmpS = CString(buf, dw);
	pos  = tmpS.Find(',');
	if (pos == -1)
	{
		title.LoadString(STSM_TEXT_OK);
		msg.LoadString(ST_MSG_ERRORINFOFILE);
		::MessageBox(m_hWnd, msg, title, MB_ICONSTOP);
		return;
	}

	tot = atoi(tmpS.Mid(pos+1));
	tmpS.Format("%d, %d", index, tot);
	WritePrivateProfileString(USERSN, USEREN, tmpS, file);
	load_BandItem();
}

bool CSmain::IsInputFocus()
{
	return m_xcom->IsInputFocus();
}
