// TotalSetup.cpp : implementation file
//

#include "stdafx.h"
#include "TotalSetup.h"
#include "afxdialogex.h"

#include "TMenu.h"
#include "ESetup.h"
#include "TSetup.h"
#include "DSetupDlg.h"
#include "AlarmDlg.h"

// CTotalSetup dialog

//IMPLEMENT_DYNAMIC(CTotalSetup, CDialog)

const TCHAR *Header_Total[] = {	_T("1"),			// 0
								_T("2"),			// 1
								_T("3"),		// 2							
								NULL};	

const int colWidth_Total[] = {	1,					// 0
								1,					// 1							
								0};					// 3


CTotalSetup::CTotalSetup(class CTMenu* menu, CString home, CString user, int index, int tick, bool bpcenable, int nWhoi, CWnd* pParent /*=NULL*/)
	: CDialog(CTotalSetup::IDD, pParent)
{
	m_menu		= menu;
	m_home            = home;
	m_user	        = user;
	m_index		= index;
	m_tick		= tick;
	m_bpcenable	= bpcenable;
	m_nWhoi		= nWhoi;

	loadGlobal();
}

CTotalSetup::CTotalSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CTotalSetup::IDD, pParent)
{

}

CTotalSetup::~CTotalSetup()
{
}

void CTotalSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREEMENU, m_tree);
	DDX_Control(pDX, IDC_XLIST, m_list);
}


BEGIN_MESSAGE_MAP(CTotalSetup, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_APPLY, OnApply)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREEMENU, &CTotalSetup::OnSelchangedTreemenu)
	ON_BN_CLICKED(ID_DEFAULT, &CTotalSetup::OnBnClickedDefault)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_XLIST, &CTotalSetup::OnItemchangedXlist)
END_MESSAGE_MAP()


// CTotalSetup message handlers


BOOL CTotalSetup::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	InitList(m_list, Header_Total);

	m_tree.DeleteAllItems();
	CBitmap bmp;
	m_imageList.Create(30,30, ILC_COLORDDB | ILC_MASK, 8, 1);

	bmp.LoadBitmap(IDB_SETUP_IMGLIST);

	CDC* pDC = GetDC();
	CDC memDC;
	BITMAP bm;

	if(memDC.CreateCompatibleDC(pDC))
	{
		bmp.GetBitmap(&bm);
		CBitmap* pbmp = (CBitmap*)memDC.SelectObject(&bmp);
		pDC->StretchBlt(0, 0, bm.bmWidthBytes, bm.bmHeight, &memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
	}

	m_imageList.Add(&bmp, RGB(255, 0, 255));
	m_tree.SetImageList(&m_imageList, TVSIL_NORMAL);
	bmp.DeleteObject();

	CRect rc;
	GetDlgItem(IDC_STATIC_DLG)->GetWindowRect(rc);
	ScreenToClient(rc);

	//m_dlgs.Add(new CESetup(m_home, m_menu));
	//m_dlgs.Add(new CTSetup(m_menu, m_home, m_user));
	m_dlgs.Add(new CDSetupDlg(m_menu, m_home, m_user));
	m_dlgs.Add(new CAlarmDlg(m_menu, m_home, m_user));


	HTREEITEM hItem, hSave = NULL;

	for (int i = 0; i < m_dlgs.GetSize(); i++)
	{
		hItem = m_tree.InsertItem(m_dlgs[i]->m_caption, i, i, 0);
		m_tree.SetItemData(hItem, i);
		m_bSetupCreate[i] = FALSE;

		m_list.InsertItem(i, _T(""));
		m_list.SetItemImage(i, 1, 0, TRUE);
		m_list.SetItemText(i, 2, m_dlgs[i]->m_caption, RGB(0,0,0), RGB(255,255,255));
		int nnn = (int)m_dlgs[i]->m_resourceID;
		if (m_index == (int)m_dlgs[i]->m_resourceID)
		{
			hSave = hItem;
			m_index = i;

			m_dlgs[i]->Create(m_dlgs[i]->m_resourceID, this);
			m_dlgs[i]->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
			m_bSetupCreate[i] = TRUE;
			m_dlgs[i]->ShowWindow(SW_SHOW);
		}
		m_applys.SetAt(m_dlgs[i]->m_resourceID, false);
	}

	int iret = m_tree.GetCount();
	hItem = m_tree.InsertItem(_T("회원정보수정"), m_tree.GetCount(), m_tree.GetCount(), 0);
	m_tree.SetItemData(hItem, iret);

	m_tree.SetItemHeight(30);
	if (hSave)
		m_tree.SelectItem(hSave);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CTotalSetup::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnEraseBkgnd(pDC);
}


HBRUSH CTotalSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


void CTotalSetup::OnSelchangedTreemenu(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	//*pResult = 0;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	m_index = m_tree.GetItemData(pNMTreeView->itemNew.hItem);

	if(m_index == m_tree.GetCount() - 1)
	{
		AfxGetMainWnd()->SendMessage(WM_USER, 3, 5030);
		OnCancel();
	}


	for (int i = 0; i < m_dlgs.GetSize(); i++)
	{
		if (m_bSetupCreate[i])
			m_dlgs[i]->ShowWindow(SW_HIDE);
	}

	if (m_index >= 0 && m_index < m_dlgs.GetSize())
	{
		if (m_bSetupCreate[m_index])
			m_dlgs[m_index]->ShowWindow(SW_SHOW);
		else
		{
			CRect	rc;
			GetDlgItem(IDC_STATIC_DLG)->GetWindowRect(rc);
			ScreenToClient(rc);
			m_dlgs[m_index]->Create(m_dlgs[m_index]->m_resourceID, this);
			m_dlgs[m_index]->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
			m_bSetupCreate[m_index] = TRUE;
		}
	}

	*pResult = 0;
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

void CTotalSetup::OnApply() 
{
	if(m_index >= 0 && m_index < m_dlgs.GetSize())
	{
		m_applys.SetAt(m_dlgs[m_index]->m_resourceID, true);

		if(m_index >= 0 && m_index < m_dlgs.GetSize())
		{
			m_dlgs[m_index]->ApplySetup();
			/*
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
			*/
		}
		AfxGetMainWnd()->SendMessage(WM_USER, 3, m_dlgs[m_index]->m_resourceID);
	}
}

void CTotalSetup::DestroyAllDialogs()
{
	for (int i = 0; i < m_dlgs.GetSize(); i++)
	{
		if (m_bSetupCreate[i])
		{
			m_dlgs[i]->EndDialog(IDCANCEL);
			m_dlgs[i]->DestroyWindow();
		}
		delete m_dlgs[i];
	}

	m_dlgs.RemoveAll();
}


void CTotalSetup::OnBnClickedDefault()
{
	// TODO: Add your control notification handler code here
}


void CTotalSetup::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}


void CTotalSetup::InitList(CXListCtrl &list, const TCHAR**pszTitle)
{
	// Image List
	ListView_SetOutlineColor(list.m_hWnd, RGB(255,0,0));

	DWORD styles = LVS_EX_GRIDLINES | LVS_EX_BORDERSELECT;
	ListView_SetExtendedListViewStyleEx(list.m_hWnd, styles, styles);  
	//list.SetOutlineColor(RGB(255,0,0));


	list.m_bShowHscroll = FALSE;      //리스트의 하단 스크롤을 보이지 않는다
	//list.SetOutlineColor(RGB(255,0,0));

	m_pImageList = new CImageList();		
	if(!m_pImageList->Create( 16, 16, ILC_COLOR32, 0, 1))
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
	else 
	{
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_LOGO);
		
		m_pImageList->Add(&bitmap, RGB(255, 0, 255));
		m_pImageList->Add(&bitmap, RGB(255, 0, 255));
		m_pImageList->Add(&bitmap, RGB(255, 0, 255));
		m_pImageList->Add(&bitmap, RGB(255, 0, 255));
		
	}

	m_list.SetImageList(m_pImageList, LVSIL_NORMAL);	

	LV_COLUMN lvcolumn;
	memset(&lvcolumn, 0, sizeof(lvcolumn));
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT /*| LVCF_WIDTH | LVCF_IMAGE*/;
	for( int i=0; ; i++ )
	{
		if( pszTitle[i] == NULL )
			break;

		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = (TCHAR *)pszTitle[i];
		lvcolumn.iSubItem = i;
		list.InsertColumn(i, &lvcolumn);
	}	

	lvcolumn.pszText = (TCHAR *)pszTitle[0];
	lvcolumn.iSubItem = 0;
	list.SetColumn(0, &lvcolumn);
	list.SetExtendedStyle(LVS_EX_FULLROWSELECT/*| LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_SUBITEMIMAGES*/);
	list.EnableToolTips(TRUE);

	// 리스트 아이템 높이 설정
	list.SetRowHeight(20);
	// 리스트 폰트 설정
	list.SetTextFont( _T("굴림체"), 9, FALSE);
	
	// 언더라인 컬러 설정
	list.SetColorUnderLine( RGB(240,240,240) );

	// 헤더 높이 설정
	list.m_HeaderCtrl.SetHeight(1);
	// 헤더 폰트 설정
	list.m_HeaderCtrl.SetTextFont( Global.GetFont(IDX_FONT_SMALL) );

	ResizeListColumn(m_list, colWidth_Total);		
}


void CTotalSetup::ResizeListColumn(CXListCtrl &list, const int *arrWidth)
{
	SCROLLINFO si;
	CRect rc;
	list.GetWindowRect(&rc);
	int nMargin=0;

	// 스크롤 유무에 따른 마진 추가
	list.GetScrollInfo(SB_VERT, &si);	
	if( si.nPage && si.nPage <= (UINT)si.nMax ) 
	{
		nMargin = ::GetSystemMetrics(SM_CXVSCROLL);
	}

	// 보더 유무에 따른 마진 추가
	DWORD dwExStyle = list.GetExStyle();
	if( dwExStyle&WS_EX_STATICEDGE || 
		dwExStyle&WS_EX_CLIENTEDGE )
	{
		nMargin += ::GetSystemMetrics(SM_CXEDGE)*2;
	}

	int nWidth = 0;
	int nLastWidth = rc.Width()-nMargin;
	for( int i=0; ; i++ )
	{
		// 마지막 컬럼일 경우
		if( arrWidth[i] == 0 )
		{
			list.SetColumnWidth(i, nLastWidth);
			break;
		}

		nWidth = (rc.Width()*arrWidth[i])/100;
		list.SetColumnWidth(i, nWidth);
		nLastWidth -= nWidth;
	}
	list.RedrawWindow();
}

void CTotalSetup::OnItemchangedXlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	if (!(pNMLV->uChanged == LVIF_STATE && pNMLV->uNewState == (LVIS_SELECTED | LVIS_FOCUSED)))
		return;

	m_index = pNMLV->iItem; 

	for (int i = 0; i < m_dlgs.GetSize(); i++)
	{
		if (m_bSetupCreate[i])
			m_dlgs[i]->ShowWindow(SW_HIDE);
	}

	if (m_index >= 0 && m_index < m_dlgs.GetSize())
	{
		if (m_bSetupCreate[m_index])
			m_dlgs[m_index]->ShowWindow(SW_SHOW);
		else
		{
			CRect	rc;
			GetDlgItem(IDC_STATIC_DLG)->GetWindowRect(rc);
			ScreenToClient(rc);
			m_dlgs[m_index]->Create(m_dlgs[m_index]->m_resourceID, this);
			m_dlgs[m_index]->SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
			m_bSetupCreate[m_index] = TRUE;
		}
	}

	*pResult = 0;
}
