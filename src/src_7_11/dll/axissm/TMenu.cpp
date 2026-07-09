// TMenu.cpp : implementation file
//

#include "stdafx.h"
#include "TMenu.h"
#include "iMenu.h"
#include "Xcom.h"
#include "../../axis/axmsg.hxx"

#define	BKCOLOR		RGB(76, 81, 90)//GetSysColor(COLOR_MENU)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTMenu

CTMenu::CTMenu(CWnd* parent, CString home, int height)
{
	m_parent = parent;
	m_home = home;
	m_height = height + GetSystemMetrics(SM_CYBORDER)*2;
	m_iMenu = NULL;
	m_pXcom = NULL;
	m_pButton = nullptr;
	m_pCombo  = nullptr;
}

CTMenu::~CTMenu()
{
	SAFE_DELETE(m_pXcom);
	SAFE_DELETE(m_pButton);
	SAFE_DELETE(m_pCombo);
	if (m_iMenu)	delete m_iMenu;
}


BEGIN_MESSAGE_MAP(CTMenu, CDialogBar)
	//{{AFX_MSG_MAP(CTMenu)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(10, OnClickUserSpace)
	ON_CBN_SELCHANGE(20, OnComboChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTMenu message handlers
CSize CTMenu::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect	rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	CRect	client;
	GetClientRect(client);

	CSize	size;
	CRect	fRc;
//	size.cx = 32767;
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

void CTMenu::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI	state;

	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = AFX_IDW_STATUS_BAR;

	if (CControlBar::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;
	state.DoUpdate(pTarget, FALSE);
}

void CTMenu::Init(int cmdBase)
{
	DWORD dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	/*
	m_pXcom = new CXcom(CTRL_SCREEN);
	if (!m_pXcom->Create(NULL, "screenNo", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_pXcom;
		m_pXcom = NULL;
	}
	else	m_pXcom->init(m_axiscall, m_home);

	m_pXcom->ShowWindow(SW_HIDE);
	*/

	m_iMenu = new CIMenu(m_parent, m_home, cmdBase);
	if (!m_iMenu->Create(NULL, "", WS_CHILD|WS_VISIBLE, CRect(0), this, (UINT) m_iMenu))
	{
		delete m_iMenu;
		m_iMenu = NULL;
	}

	
	//m_pButton = new CButton();
	//if ( !m_pButton->Create("사용자화면", WS_VISIBLE|WS_CHILD, CRect(0), this, 10) )
	//{
	//	delete m_pButton;
	//	m_pButton = nullptr;
	//}

	//m_pCombo = new CComboBox();
	//if ( !m_pCombo->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST, CRect(0), this, 20) )
	//{
	//	delete m_pCombo;
	//	m_pCombo = nullptr;
	//}

	//m_pCombo->AddString("NONE");
	//CString sFile, sKey;
	//int nn;
	//sFile.Format("%s\\tab\\combo.ini", m_home);	
	//
	//GetPrivateProfileString("ROOT", "combo", "", sKey.GetBuffer(256), 256, sFile);
	//sKey.ReleaseBuffer();
	//nn = atoi(sKey) + 1;

	//for (int ii = 1; ii < nn ; ii++)
	//{
	//	sKey.Format("user%02d", ii);
	//	m_pCombo->AddString(sKey);
	//}

	//m_pCombo->SetCurSel(0);
}

BOOL CTMenu::InitMenu(UINT nResource, int cmdBase)
{
	if (m_iMenu)	return FALSE;

	Init(cmdBase);
//	m_iMenu->LoadFramemenu(nResource);
	m_iMenu->LoadMenufromfile();
	m_iMenu->LoadFramemenu(nResource);
	fitMenu();
	return TRUE;
}

BOOL CTMenu::ReloadMenu(UINT nResource, int cmdBase)
{
	if (!m_iMenu)	return FALSE;
	delete	m_iMenu;
	m_iMenu = NULL;

	InitMenu(nResource, cmdBase);
	fitMenu();
	return TRUE;
}

BOOL CTMenu::PreTranslateMsg(MSG* pMsg)
{
	if (m_iMenu && m_iMenu->PreTranslateMsg(pMsg))
		return TRUE;
	return FALSE;
}

int  CTMenu::GetMCount()			{ return m_iMenu->GetMCount(); }
BOOL CTMenu::ExistMenu(CString mapN)	{ return m_iMenu->ExistMenu(mapN); }
BOOL CTMenu::IsWeb(CString mapN)		{ return m_iMenu->IsWeb(mapN); }
BOOL CTMenu::IsMappinged(CString mapN)	{ return m_iMenu->IsMappinged(mapN); }
BOOL CTMenu::GetSelectList(CString input, CStringArray& array)
					{ return m_iMenu->GetSelectList(input, array); }
CString	CTMenu::GetURL(CString mapN)	{ return m_iMenu->GetURL(mapN); }
CString	CTMenu::GetSName(CString mapN)	{ return m_iMenu->GetSName(mapN); }
CString	CTMenu::GetDesc(CString mapN)	{ return m_iMenu->GetDesc(mapN); }
CString	CTMenu::GetInfo(CString mapN)	{ return m_iMenu->GetInfo(mapN); }
CString	CTMenu::GetMap(int index)	{ return m_iMenu->GetMap(index); }
CString	CTMenu::GetMap(CString dispN)	{ return m_iMenu->GetMap(dispN); }
CString	CTMenu::GetMapping(CString mapN)	
{ 
	return m_iMenu->GetMapping(mapN); 
}
CString	CTMenu::GetDispN(CString mapN)	{ return m_iMenu->GetDispN(mapN); }

CMenu* CTMenu::GetPopupMenu(int index)	{ return m_iMenu->GetPopupMenu(index); }

void CTMenu::fitMenu()
{
	if (!m_iMenu)	return;

	CRect	cRc;
	GetClientRect(cRc);
	// 메뉴 공간 여백
	cRc.left += 100;
	m_iMenu->MoveWindow(cRc);
}

void CTMenu::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);

	if (m_iMenu)
	{
		CRect	cRc;

		GetClientRect(cRc);

		// 메뉴 공간 여백
		//cRc.left += 110;
		cRc.OffsetRect(5, 3);  
		m_iMenu->MoveWindow(cRc);
	}

	CRect xRc;
	if (m_pXcom)
	{
		GetClientRect(xRc);

		xRc.right = 90;
		xRc.OffsetRect(10, 0);
		//xRc.SetRect(tRc.right + 20, tRc.top - 6, tRc.right+GAP+size.cx+COMBTN_WIDTH+40, tRc.bottom -6);
		int	gap = 0;
		if (xRc.Height() != XCOM_HEIGHT)
		{
			gap = (xRc.Height() - XCOM_HEIGHT)/2;
			xRc.DeflateRect(0, gap);
		}

		m_pXcom->MoveWindow(xRc);
		m_pXcom->changepos();
	}

	CRect rc;
	GetClientRect(rc);

	if (m_pCombo)
	{
		rc.top += 3;
		rc.left  = xRc.right + 10;
		rc.right = rc.left + 100;
		rc.bottom += 300;
	
		m_pCombo->MoveWindow(rc);
	}

	if (m_pButton)
	{
		rc.CopyRect(xRc);
		rc.OffsetRect(210, 0);
		m_pButton->MoveWindow(rc);
	}
}

void CTMenu::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	cRc;
	GetClientRect(cRc);

	dc.SetBkColor(BKCOLOR);
	dc.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, &cRc, "", 0, NULL);


	//***************************************
	// 맨 위에 라인 그리기
	//***************************************
	CPen pen, pen1, pen2;
	CPen *oldpen;


	pen1.CreatePen(PS_SOLID, 2, RGB(34,38,44));
	pen2.CreatePen(PS_SOLID, 1, RGB(255,176,0));
	
	oldpen = dc.SelectObject(&pen1);

	dc.SelectObject(&pen1);
	dc.MoveTo(cRc.TopLeft()); 
	dc.LineTo(cRc.right, cRc.top);

	//dc.MoveTo(cRc.TopLeft()); 
	//dc.LineTo(cRc.right, cRc.top);

	//cRc.DeflateRect(0, 1);
	cRc.OffsetRect(0,3);
	dc.SelectObject(&pen2);
	dc.MoveTo(cRc.TopLeft()); 
	dc.LineTo(cRc.right, cRc.top);

	dc.SelectObject(oldpen);

	pen1.DeleteObject();
	pen2.DeleteObject();
	//**************************************
}

BOOL CTMenu::IsvalidMap(CString mapN)
{
	if (!m_iMenu)	return FALSE;
	return m_iMenu->ExistMenu(mapN);
}

void CTMenu::MeasureMenuItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if (m_iMenu)
		m_iMenu->MeasureMenuItem(nIDCtl,lpMeasureItemStruct); 
}

void CTMenu::ShowFullMenu(CPoint pt, bool bTray)
{
	if (m_iMenu)
		m_iMenu->ShowFullMenu(pt, bTray);
}

void CTMenu::SetHome()
{
	if(m_pXcom)
		m_pXcom->set_Home();
}

void CTMenu::set_Maps(CString mapname)
{
	CString mapN = GetMapping(mapname);
	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axTMenu, 0), (LPARAM)mapN.operator LPCSTR());
	//m_pXcom->set_Maps(mapN);
}

void CTMenu::add_History(CString dat)
{
	if (m_pXcom)
		m_pXcom->add_History(dat);
}

void CTMenu::set_Owner()
{
	if(m_pXcom)
		m_pXcom->set_Owner();
}

#define	WM_AXIS		(WM_USER+500)
void CTMenu::OnClickUserSpace()
{
	if (m_pCombo == nullptr)
		return;

	CString sUser;
	int nn = m_pCombo->GetCount();
	int index = m_pCombo->GetCurSel();

	m_pCombo->GetLBText(index, sUser);

	CString sFile, sKey;
	sFile.Format("%s\\tab\\combo.ini", m_home);
	if (sUser.CompareNoCase("NONE") == 0)	
	{
		sUser.Format("user%02d", nn);
		m_pCombo->AddString(sUser);
		sKey.Format("%d", nn);
		WritePrivateProfileString("ROOT", "combo", sKey, sFile);
	}
	else					
	{
		if ( MessageBox("저장되어 있습니다. 덮어 씌우시겠습니까?", "메세지",  MB_YESNO) != IDYES)
			return;
	}

	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(25, 0), (LPARAM)sUser.operator LPCSTR());

}

void CTMenu::OnComboChange()
{
	if (m_pCombo == nullptr)
		return;
	
	CString sUser;
	int index = m_pCombo->GetCurSel();
	m_pCombo->GetLBText(index, sUser);

	if (sUser.IsEmpty() || sUser.CompareNoCase("NONE") == 0)
		return;

	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(43, 0), (LPARAM)sUser.operator LPCSTR());
}

void CTMenu::set_List(CStringArray& array)
{
	if (m_pXcom)	
		m_pXcom->set_List(array);
}