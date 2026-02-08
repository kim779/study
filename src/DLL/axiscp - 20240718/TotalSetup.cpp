// TotalSetup.cpp : implementation file
//

#include "stdafx.h"
#include "TotalSetup.h"
#include "../sm/tmenu.h"

#include "esetup.h"
#include "mngsetup.h"
#include "ordersetup.h"
#include "tsetup.h"
#include "settickinfo.h"
#include "hotkey.h"
#include "ImageSetup.h"
#include "ToolFunc.h"
#include "EqualizerSetup.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define IDC_BTN_CLOSE 6781
#define COLOR_FRAME_CONF		RGB(0,80,141)

/////////////////////////////////////////////////////////////////////////////
// CTotalSetup dialog


CTotalSetup::CTotalSetup(class CTMenu* menu, int index, int tick, CWnd* pParent /*=NULL*/)
	: CDialog(CTotalSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTotalSetup)
	//}}AFX_DATA_INIT
	m_menu = menu;
	m_index = index;
	m_tick = tick;
	m_pMain = pParent;
}


void CTotalSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTotalSetup)
	DDX_Control(pDX, IDC_TREEMENU, m_tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTotalSetup, CDialog)
	//{{AFX_MSG_MAP(CTotalSetup)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEMENU, OnSelchangedTreemenu)
	ON_BN_CLICKED(ID_DEFAULT, OnDefault)
	ON_BN_CLICKED(ID_APPLY, OnApply)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTotalSetup message handlers

BOOL CTotalSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//CString stemp;
	//CString strHwnd;
	//strHwnd.Format("%d", this->m_hWnd);
	//::SendMessage(m_pMain->m_hWnd, WM_INTERMSG, MAKEWPARAM(MAKEWORD(MMSG_SET_INTEREMAP, -1), this->m_hWnd), (LPARAM)(LPCTSTR)strHwnd);


	//m_font.CreatePointFont(90, "굴림체");
	m_font.CreateFont(	12,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_BOLD,	           // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			DEFAULT_CHARSET,           // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("굴림체"));     
	//m_bmplogo = Axis::GetBitmap("화면창_LOGO");
	CRect trc;
	GetClientRect(trc);
	
	//m_bitmapBtn = new CBmpButton();
	m_bitmapBtn = std::make_unique<CBmpButton>();
	
	if(!m_bitmapBtn->Create("", BS_BITMAP, CRect(trc.right-19,3, trc.right-3,16), this, IDC_BTN_CLOSE))
	{
		//delete m_bitmapBtn;
		m_bitmapBtn.release();
		m_bitmapBtn = NULL;
	}
		
	m_bitmapBtn->SetImgBitmap(Axis::GetSkinBitmap("창닫기_T"), 2);
	
	
	//ModifyStyle(WS_CAPTION, 0, SWP_DRAWFRAME);
	
	//m_changer = new CControlChanger(this);
	m_changer = std::make_unique< CControlChanger>(this);
	//m_bmpcap = Axis::GetBitmap("BLUE_SFRAME_MIDDLE");
	m_tree.DeleteAllItems();
	m_tree.SetBkColor( Axis::GetColor(39));

	CBitmap bmp;
	m_imageList.Create(20, 20, ILC_COLORDDB | ILC_MASK, 9, 1);
	
	ASSERT(m_imageList.m_hImageList);
	
	bmp.LoadBitmap(IDB_SETUP_IMGLIST);
	m_imageList.Add(&bmp, RGB(255, 0, 255));
	m_tree.SetImageList(&m_imageList, TVSIL_NORMAL);
	bmp.DeleteObject();

	CRect	rc;
	GetDlgItem(IDC_STATIC_DLG)->GetWindowRect(rc);
	ScreenToClient(rc);

	/*m_dlgs.Add(new CESetup);
	m_dlgs.Add(new CMngSetup);
	m_dlgs.Add(new COrderSetup);
	m_dlgs.Add(new CImageSetup);
	m_dlgs.Add(new CSetTickInfo(m_tick));
	m_dlgs.Add(new CHotkey(m_menu));
	m_dlgs.Add(new CTSetup(m_menu));
	m_dlgs.Add(new CToolFunc);
	m_dlgs.Add(new CEqualizerSetup);*/
	m_dlgs.emplace_back(std::make_shared<CESetup>());
	m_dlgs.emplace_back(std::make_shared< CMngSetup>());
	m_dlgs.emplace_back(std::make_shared< COrderSetup>());
	m_dlgs.emplace_back(std::make_shared< CImageSetup>());
	m_dlgs.emplace_back(std::make_shared< CSetTickInfo>(m_tick, m_pMain));
	m_dlgs.emplace_back(std::make_shared< CHotkey>(m_menu));
	m_dlgs.emplace_back(std::make_shared< CTSetup>(m_menu));
	m_dlgs.emplace_back(std::make_shared< CToolFunc>());
	m_dlgs.emplace_back(std::make_shared< CEqualizerSetup>());

	m_clrFrame = Axis::GetColor( 64 );

	HTREEITEM hItem{}, hSave = NULL;

	int i= 0;
	CSetupDialog* wnd{};
	for (auto item : m_dlgs)
	{
		wnd = getanyFind(item);
		hItem = m_tree.InsertItem(wnd->m_caption, i, i, 0);
		m_tree.SetItemData(hItem, i);
		wnd->Create(wnd->m_resourceID, this);
		wnd->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);

		if (m_index == wnd->m_resourceID)
		{
			hSave = hItem;
			m_index = i;
		}

		m_applys.SetAt(wnd->m_resourceID, false);
		i++;
	}

	//for (int i = 0; i < m_dlgs.GetSize(); i++)
	//{
	//	m_dlgs[i]->Create(m_dlgs[i]->m_resourceID, this);
	//	m_dlgs[i]->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);

	//	hItem = m_tree.InsertItem(m_dlgs[i]->m_caption, i, i, 0);
	//	m_tree.SetItemData(hItem, i);
	//	if (m_index == (int)m_dlgs[i]->m_resourceID)
	//	{
	//		hSave = hItem;
	//		m_index = i;
	//	}

	//	m_applys.SetAt(m_dlgs[i]->m_resourceID, false);
	//}

	m_tree.SetItemHeight(30);
	if (hSave)
		m_tree.SelectItem(hSave);

	return TRUE;
}

CSetupDialog* CTotalSetup::getanyFind(std::any& item)
{
	CSetupDialog* wnd{};

	if (item.type() == typeid(std::shared_ptr< CESetup>))
		wnd = std::any_cast<std::shared_ptr< CESetup>>(item).get();
	else if (item.type() == typeid(std::shared_ptr<CMngSetup>))
		wnd = std::any_cast<std::shared_ptr< CMngSetup>>(item).get();
	else if (item.type() == typeid(std::shared_ptr<COrderSetup>))
		wnd = std::any_cast<std::shared_ptr< COrderSetup>>(item).get();
	else if (item.type() == typeid(std::shared_ptr<CImageSetup>))
		wnd = std::any_cast<std::shared_ptr<CImageSetup>>(item).get();
	else if (item.type() == typeid(std::shared_ptr<CSetTickInfo>))
		wnd = std::any_cast<std::shared_ptr<CSetTickInfo>>(item).get();
	else if (item.type() == typeid(std::shared_ptr<CHotkey>))
		wnd = std::any_cast<std::shared_ptr<CHotkey>>(item).get();
	else if (item.type() == typeid(std::shared_ptr<CTSetup>))
		wnd = std::any_cast<std::shared_ptr<CTSetup>>(item).get();
	else if (item.type() == typeid(std::shared_ptr<CToolFunc>))
		wnd = std::any_cast<std::shared_ptr<CToolFunc>>(item).get();
	else if (item.type() == typeid(std::shared_ptr<CEqualizerSetup>))
		wnd = std::any_cast<std::shared_ptr< CEqualizerSetup>>(item).get();

	return wnd;
}

void CTotalSetup::OnSelchangedTreemenu(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	m_index = m_tree.GetItemData(pNMTreeView->itemNew.hItem);

	auto item = m_dlgs.at(m_index);
	if (item.type() == typeid(std::shared_ptr< CSetTickInfo>))
	{
		CSetupDialog* wnd = std::any_cast<std::shared_ptr< CSetTickInfo>>(item).get();
		wnd->SearchGroupList();
	}


	//for (int i = 0; i < m_dlgs.GetSize(); i++)
	//	m_dlgs[i]->ShowWindow(SW_HIDE);
	
	for(int i = 0; i < (int)m_dlgs.size(); i++)
		getanyFind(m_dlgs.at(i))->ShowWindow(SW_HIDE);

	//if (m_index >= 0 && m_index < m_dlgs.GetSize())
	//	m_dlgs[m_index]->ShowWindow(SW_SHOW);
	
	if (m_index >= 0 && m_index < (int)m_dlgs.size())
		getanyFind(m_dlgs.at(m_index))->ShowWindow(SW_SHOW);

	*pResult = 0;
}

void CTotalSetup::OnDefault() 
{
	//if (m_index >= 0 && m_index < m_dlgs.GetSize())
	//	m_dlgs[m_index]->DefaultSetup();

	if (m_index >= 0 && m_index < (int)m_dlgs.size())
		getanyFind(m_dlgs.at(m_index)) ->DefaultSetup();
}

void CTotalSetup::OnOK() 
{
	OnApply();

	DestroyAllDialogs();

	CDialog::OnOK();
}

void CTotalSetup::OnCancel() 
{
	DestroyAllDialogs();

	CDialog::OnCancel();

}

void CTotalSetup::DestroyAllDialogs()
{
	/*for (int i = 0; i < m_dlgs.GetSize(); i++)
	{
		m_dlgs[i]->EndDialog(IDCANCEL);
		m_dlgs[i]->DestroyWindow();
		delete m_dlgs[i];
	}

	m_dlgs.RemoveAll();*/
	m_dlgs.clear();
}

void CTotalSetup::OnApply() 
{
	if (m_index >= 0 && m_index < (int)m_dlgs.size())
	{
		auto wnd = getanyFind(m_dlgs[m_index]);
		m_applys.SetAt(wnd->m_resourceID, true);

		if (m_index >= 0 && m_index < (int)m_dlgs.size())
		{
			wnd->ApplySetup();

			switch (wnd->m_resourceID)
			{
			case IDD_SETTICKINFO:
			{
				CSetTickInfo* dlg = (CSetTickInfo*)wnd;
				m_bTickView[0] = dlg->GetViewTick(0);
				m_bTickView[1] = dlg->GetViewTick(1);
				m_bScreenBar = dlg->GetViewTick(2);
			}
			break;
			case IDD_IMAGESETUP:
				Invalidate();
				break;
			}

			AfxGetMainWnd()->SendMessage(WM_USER, 3, wnd->m_resourceID);
		}
	}

	/*if (m_index >= 0 && m_index < m_dlgs.GetSize())
	{
		m_applys.SetAt(m_dlgs[m_index]->m_resourceID, true);

		if (m_index >= 0 && m_index < m_dlgs.GetSize())
		{
			m_dlgs[m_index]->ApplySetup();

			switch (m_dlgs[m_index]->m_resourceID)
			{
				case IDD_SETTICKINFO:
					{
						CSetTickInfo* dlg = (CSetTickInfo*)m_dlgs[m_index];
						m_bTickView[0] = dlg->GetViewTick(0);
						m_bTickView[1] = dlg->GetViewTick(1);
						m_bScreenBar = dlg->GetViewTick(2);
					}
					break;
				case IDD_IMAGESETUP:
					Invalidate();
					break;
			}

			AfxGetMainWnd()->SendMessage(WM_USER, 3, m_dlgs[m_index]->m_resourceID);
		}
	}*/
}

bool CTotalSetup::IsApplied(UINT id)
{
	bool applied;

	if (m_applys.Lookup(id, applied))
		return applied;
	else
		return false;
}

BOOL CTotalSetup::OnEraseBkgnd(CDC* pDC) 
{
	if (Axis::dialogBgColor > 0)
	{
		CRect rc;
		GetClientRect(&rc);
		
		CBrush* oldBrush = pDC->SelectObject(&Axis::dialogBgBrush);

		pDC->PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(oldBrush);

		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

HBRUSH CTotalSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	return hbr;
}

void CTotalSetup::OnDestroy() 
{
	CDialog::OnDestroy();
/*
	if (m_bmplogo)
	{
		delete m_bmplogo;
		m_bmplogo = NULL;
	}
*/
	//if (m_bitmapBtn)
	//{
		//delete m_bitmapBtn;
		//m_bitmapBtn = NULL;
	//}
	
	//delete m_changer;	
}

void CTotalSetup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	DrawFrame(&dc);
	// Do not call CDialog::OnPaint() for painting messages
}

void CTotalSetup::DrawFrame(CDC *pDC)
{
	//if (m_bBackGround) return;
	if (GetStyle() & WS_CAPTION)
	{
		SetWindowRgn(0, FALSE);
	}

	CRect winRc;
	GetClientRect(winRc);

	CBrush br;
	br.CreateSolidBrush(m_clrFrame);
	//pDC->SelectObject(&br);
	pDC->FillRect(CRect(winRc.left, winRc.top, winRc.left+3, winRc.bottom), &br);
	pDC->FillRect(CRect(winRc.left, winRc.bottom-4, winRc.right-1, winRc.bottom), &br);
	pDC->FillRect(CRect(winRc.right-4, winRc.top, winRc.right-1, winRc.bottom-1), &br);


	pDC->MoveTo(winRc.left, winRc.top);  pDC->LineTo(winRc.right-1, winRc.top); pDC->LineTo(winRc.right-1, winRc.bottom-1);
	pDC->LineTo(winRc.left, winRc.bottom-1);  pDC->LineTo(winRc.left, winRc.top);


	winRc.bottom = 20;
	CBitmap* m_bmpcap = Axis::GetSkinBitmap("CAP_ACT");
	if (m_bmpcap)
	{
		CDC dc_cap;
		dc_cap.CreateCompatibleDC(pDC);
		dc_cap.SelectObject(m_bmpcap);
		pDC->StretchBlt(0,0,winRc.Width(), winRc.Height(),&dc_cap, 0, 0, 1, 20, SRCCOPY);
		dc_cap.DeleteDC();

	}
	
	pDC->MoveTo(winRc.left, winRc.top);  pDC->LineTo(winRc.right-1, winRc.top); pDC->LineTo(winRc.right-1, winRc.bottom-1);
	pDC->LineTo(winRc.left, winRc.bottom-1);  pDC->LineTo(winRc.left, winRc.top);
	
	pDC->SelectObject(m_font);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,255));
	pDC->TextOut(25, 4, "종합환경설정");
	//pDC->Rectangle(winRc);

	CRect rcTree;
	m_tree.GetWindowRect( rcTree );
	ScreenToClient( &rcTree );

	CPen pen1, pen2, pen3, pen4;

	pen1.CreatePen( PS_SOLID, 1, Axis::GetColor(38) );
	pen2.CreatePen( PS_SOLID, 1, Axis::GetColor(46) );

	pen3.CreatePen( PS_SOLID, 1, Axis::GetColor(61) );
	pen4.CreatePen( PS_SOLID, 1, Axis::GetColor(76) );

	pDC->SelectObject( pen1 );
	pDC->MoveTo( rcTree.left-2, rcTree.top-2 );
	pDC->LineTo( rcTree.left-2, rcTree.bottom+2 );
	pDC->MoveTo( rcTree.left-2, rcTree.top-2 );
	pDC->LineTo( rcTree.right+2, rcTree.top-2 );

	pDC->SelectObject( pen2 );
	pDC->MoveTo( rcTree.left-1, rcTree.top-1 );
	pDC->LineTo( rcTree.left-1, rcTree.bottom+1 );
	pDC->MoveTo( rcTree.left-1, rcTree.top-1 );
	pDC->LineTo( rcTree.right+1, rcTree.top-1 );

	pDC->SelectObject( pen3 );
	pDC->MoveTo( rcTree.right, rcTree.top );
	pDC->LineTo( rcTree.right, rcTree.bottom );
	pDC->MoveTo( rcTree.right, rcTree.bottom );
	pDC->LineTo( rcTree.left, rcTree.bottom );

	pDC->SelectObject( pen4 );
	pDC->MoveTo( rcTree.right+1, rcTree.top-1 );
	pDC->LineTo( rcTree.right+1, rcTree.bottom+1 );
	pDC->MoveTo( rcTree.right+1, rcTree.bottom+1 );
	pDC->LineTo( rcTree.left-1, rcTree.bottom+1 );

}

LRESULT CTotalSetup::OnNcHitTest(CPoint point)   //vc2019 UINT->LRESULT
{
	const UINT result = CDialog::OnNcHitTest(point);

	return result;
}

void CTotalSetup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CTotalSetup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ReleaseCapture();
	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CTotalSetup::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	const int	index = LOWORD(wParam);
	if (index == IDC_BTN_CLOSE)
	{
		OnCancel();
	}
	
	return CDialog::OnCommand(wParam, lParam);
}
