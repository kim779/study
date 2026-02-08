// USetup.cpp : implementation file
//

#include "stdafx.h"
#include "IB100000.h"
#include "USetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUSetup dialog


CUSetup::CUSetup(CString home, CString user, CWnd* pParent /*=NULL*/)
	: CDialog(CUSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUSetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_home = home;
	m_user = user;
	m_selectRes = -1;
	m_selectA = -1;
	m_selectType = TYPE_NONE;
	m_section = _T("STOCK");
	m_bChange = FALSE;

	m_brushC.CreateSolidBrush(RGB(229, 251, 253));
}

CUSetup::~CUSetup()
{
	for (int ii = 0; ii < 4; ii++)
		m_arS[ii].RemoveAll();
	m_arInfo.RemoveAll();
	m_brushC.DeleteObject();
}

void CUSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUSetup)
	DDX_Control(pDX, IDC_USERMAPLIST, m_mlist);
	DDX_Control(pDX, IDC_SLIST, m_slist);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_TYPE7, m_type7);
	DDX_Control(pDX, IDC_TYPE6, m_type6);
	DDX_Control(pDX, IDC_TYPE5, m_type5);
	DDX_Control(pDX, IDC_TYPE4, m_type4);
	DDX_Control(pDX, IDC_TYPE3, m_type3);
	DDX_Control(pDX, IDC_TYPE2, m_type2);
	DDX_Control(pDX, IDC_TYPE1, m_type1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUSetup, CDialog)
	//{{AFX_MSG_MAP(CUSetup)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_TYPE1, OnType1)
	ON_BN_CLICKED(IDC_TYPE2, OnType2)
	ON_BN_CLICKED(IDC_TYPE3, OnType3)
	ON_BN_CLICKED(IDC_TYPE4, OnType4)
	ON_BN_CLICKED(IDC_TYPE5, OnType5)
	ON_BN_CLICKED(IDC_TYPE6, OnType6)
	ON_BN_CLICKED(IDC_TYPE7, OnType7)
	ON_BN_CLICKED(IDC_STOCK, OnStock)
	ON_BN_CLICKED(IDC_FOPTION, OnFoption)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_SLIST, OnDblclkSlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_USERMAPLIST, OnItemchangedUsermaplist)
	ON_NOTIFY(NM_CLICK, IDC_USERMAPLIST, OnClickUsermaplist)
	ON_BN_CLICKED(IDC_BTN_ADDALL, OnBtnAddall)
	ON_BN_CLICKED(IDC_BTN_DELALL, OnBtnDelall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUSetup message handlers

BOOL CUSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	init_Ctrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUSetup::init_Ctrl()
{
	init_Button();
	init_List();

	saveViewPos();
	positionView();
	((CButton *) GetDlgItem(IDC_STOCK))->SetCheck(TRUE);
}

void CUSetup::init_Button()
{
	m_type1.SetBitmaps(IDB_TYPE1_, RGB(255, 0, 255), IDB_TYPE1, RGB(255, 0, 255));
	m_type2.SetBitmaps(IDB_TYPE2_, RGB(255, 0, 255), IDB_TYPE2, RGB(255, 0, 255));
	m_type3.SetBitmaps(IDB_TYPE3_, RGB(255, 0, 255), IDB_TYPE3, RGB(255, 0, 255));
	m_type4.SetBitmaps(IDB_TYPE4_, RGB(255, 0, 255), IDB_TYPE4, RGB(255, 0, 255));
 	m_type5.SetBitmaps(IDB_TYPE5_, RGB(255, 0, 255), IDB_TYPE5, RGB(255, 0, 255));
	m_type6.SetBitmaps(IDB_TYPE6_, RGB(255, 0, 255), IDB_TYPE6, RGB(255, 0, 255));
	m_type7.SetBitmaps(IDB_TYPE7_, RGB(255, 0, 255), IDB_TYPE7, RGB(255, 0, 255));
}

void CUSetup::init_List()
{
	CRect	rc;

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_slist.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_mlist.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_list.GetClientRect(rc);
	addColumn(m_list, "화 면 이 름", 0, rc.Width());

	const int	snameW = 90;
	m_slist.GetClientRect(rc);
	addColumn(m_slist, "화 면 이 름", 0, rc.Width()-snameW);
	addColumn(m_slist, "탭 명", 1, snameW);

	m_mlist.GetClientRect(rc);
	addColumn(m_mlist, "사용자종합", 0, rc.Width());

	fill_List();
	fill_UList();
}

void CUSetup::fill_List(char type /* = 0 */)
{
	int	pos = 0, idx = 0;
	char	buf[1024*4];
	CString	file, str, tmpS;
	
	m_list.DeleteAllItems();
	m_arInfo.RemoveAll();

	file.Format("%s\\tab\\종합list.ini", m_home );
	const DWORD dw = GetPrivateProfileSection(m_section, buf, sizeof(buf), file);

	if (type == 0 && m_selectRes >= 0)
	{
		CString str;
		GetDlgItemText(m_selectRes, str);
		if (!str.IsEmpty())
			type = str[0];
	}

	if (dw > 0)
	{
		for (int ii = 0;; ii++)
		{
			str = &buf[idx];
			if (str.IsEmpty())	
				break;

			idx += str.GetLength()+1;
			pos = str.Find('=');
			if (pos == -1)		
				continue;

			pos++;

			tmpS = str.Mid(pos, str.Find(';', pos) - pos);
			tmpS.TrimLeft(); tmpS.TrimRight();

			if (!type || type == str[str.GetLength() - 1])
			{
				m_arInfo.Add(str);
				addItem(m_list, tmpS, ii);
			}
		}
	}
	fit(m_list);
}

void CUSetup::fill_UList()
{
// 사용자 정의화면
	m_mlist.DeleteAllItems();

	char	section[32]{};
	char	buf[1024 * 4]{};
	CString file, str;

	file.Format("%s\\user\\%s\\종합.ini", m_home, m_user);
	DWORD dw = GetPrivateProfileString("GENERAL", "section", "", section, sizeof(section), file);
	if (dw > 0)
	{
		for (int ii = 0;; ii++)
		{
			str.Format("%02d", ii);
			dw = GetPrivateProfileString(section, str, "", buf, sizeof(buf), file);
			if (dw <= 0)	
				break;
			addItem(m_mlist, buf, ii);
		}
	}

	fit(m_mlist);
}

BOOL CUSetup::addColumn(CListCtrl& list, LPCTSTR columns, int col, int width)
{
	LV_COLUMN lvc;

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR)columns;
	lvc.cx	     = /*list.GetStringWidth(lvc.pszText) + */width;
	lvc.iSubItem = -1;

	return list.InsertColumn(col, &lvc);
}

BOOL CUSetup::addItem(CListCtrl& list, CString items, int item, int subi)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subi;
	lvi.pszText  = (LPTSTR)items.operator LPCTSTR();

	if (!subi)	return list.InsertItem(&lvi);
	return list.SetItem(&lvi);
}

HBRUSH CUSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	const UINT resource = pWnd->GetDlgCtrlID();
	switch (resource)
	{
	case IDC_VIEW1:case IDC_VIEW2:case IDC_VIEW3:case IDC_VIEW4:
		if (resource == (UINT)m_selectRes)
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = m_brushC;
		}
		break;
	default:
		break;
	}

	return hbr;
}

void CUSetup::OnType1() 
{
	positionView(TYPE_1);
}

void CUSetup::OnType2() 
{
	positionView(TYPE_2);
}

void CUSetup::OnType3() 
{
	positionView(TYPE_3);
}

void CUSetup::OnType4() 
{
	positionView(TYPE_4);
}

void CUSetup::OnType5() 
{
	positionView(TYPE_5);
}

void CUSetup::OnType6() 
{
	positionView(TYPE_6);
}

void CUSetup::OnType7() 
{
	positionView(TYPE_7);
}

void CUSetup::OnStock() 
{
	m_section = _T("STOCK");
	fill_List();
}

void CUSetup::OnFoption() 
{
	m_section = _T("FOPTION");
	fill_List();
}

void CUSetup::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	if (m_selectType == TYPE_NONE)
	{
		::MessageBox(m_hWnd, "화면 스타일을 선택하여 주십시오.", "화면스타일 선택", MB_ICONWARNING);
		return;
	}

	if (m_selectRes == -1)
	{
		::MessageBox(m_hWnd, "화면 영역을 선택하여 주십시오.", "화면영역 선택", MB_ICONWARNING);
		return;
	}

	CString	string, tmpS, dat;
	int pos=0, idx = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (idx != -1)
	{
		string = m_arInfo.GetAt(idx);
		idx = m_slist.GetItemCount();
		pos = string.Find('=');
		tmpS = string.Left(8);
		for (int ii = 0; ii < m_slist.GetItemCount(); ii++)
		{
			dat = m_arS[m_selectA].GetAt(ii);
			dat = dat.Left(8);
			if (!dat.Compare(tmpS))	return;
		}
		m_arS[m_selectA].Add(string);
		tmpS = string.Mid(pos+1, string.Find(';', pos) - (pos+1));
		tmpS.TrimLeft();	tmpS.TrimRight();
		addItem(m_slist, tmpS, idx);
		pos = string.Find(';');
		tmpS = string.Mid(pos+1);
		pos = tmpS.Find(';');
		tmpS = tmpS.Mid(0,pos);
		addItem(m_slist, tmpS, idx, 1);
		countView(m_selectA);
	}

	fit2(m_slist);
}

void CUSetup::OnDblclkSlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	const int idx = m_slist.GetNextItem(-1, LVNI_SELECTED);
	if (idx != -1)
	{
		m_slist.DeleteItem(idx);
		CString	dat = m_arS[m_selectA].GetAt(idx);
		m_arS[m_selectA].RemoveAt(idx);
		countView(m_selectA);
		fit2(m_slist);
	}
}

void CUSetup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ChangeView(point);
	CDialog::OnLButtonDown(nFlags, point);
}

void CUSetup::ChangeView(CPoint point)
{
	CRect	vRc;
	bool	noselect = true;

	ClientToScreen(&point);
	GetDlgItem(IDC_VIEW1)->GetWindowRect(vRc);
	if (vRc.PtInRect(point))
	{
		if (m_selectA == 0)	return;
		m_selectRes = IDC_VIEW1;
		m_selectA = 0;
		noselect = false;
	}
	
	GetDlgItem(IDC_VIEW2)->GetWindowRect(vRc);
	if (vRc.PtInRect(point) && noselect)
	{
		if (m_selectA == 1)	return;
		m_selectRes = IDC_VIEW2;
		m_selectA = 1;
		noselect = false;
	}

	GetDlgItem(IDC_VIEW3)->GetWindowRect(vRc);
	if (vRc.PtInRect(point) && noselect)
	{
		if (m_selectA == 2)	return;
		m_selectRes = IDC_VIEW3;
		m_selectA = 2;
		noselect = false;
	}

	GetDlgItem(IDC_VIEW4)->GetWindowRect(vRc);
	if (vRc.PtInRect(point) && noselect)
	{
		if (m_selectA == 3)	return;
		m_selectRes = IDC_VIEW4;
		m_selectA = 3;
		noselect = false;
	}

	char buffer[MAX_PATH];
	GetDlgItemText(m_selectRes, buffer, MAX_PATH);
	if (strlen(buffer) > 0) 
	{
		fill_List(buffer[0]);
	}

	load_Slist();
	UpdateView();
}

void CUSetup::UpdateView()
{
	GetDlgItem(IDC_VIEW1)->Invalidate();
	GetDlgItem(IDC_VIEW2)->Invalidate();
	GetDlgItem(IDC_VIEW3)->Invalidate();
	GetDlgItem(IDC_VIEW4)->Invalidate();
}

void CUSetup::countView(int selectA)
{
	CString	text, tmpS, count;
	UINT	res{};
	switch (selectA)
	{
	case 0:	res = IDC_VIEW1;	break;
	case 1: res = IDC_VIEW2;	break;
	case 2: res = IDC_VIEW3;	break;
	case 3: res = IDC_VIEW4;	break;
	default:	return;
	}
	GetDlgItem(res)->GetWindowText(text);
	tmpS = text.Left(1);
	text.Format("%s(%d)", tmpS, m_arS[selectA].GetSize());
	GetDlgItem(res)->SetWindowText(text);
}

void CUSetup::saveViewPos()
{
	GetDlgItem(IDC_VIEW1)->GetWindowRect(m_vRc[0]);
	GetDlgItem(IDC_VIEW2)->GetWindowRect(m_vRc[1]);
	GetDlgItem(IDC_VIEW3)->GetWindowRect(m_vRc[2]);
	GetDlgItem(IDC_VIEW4)->GetWindowRect(m_vRc[3]);

	for (int ii = 0; ii < 4; ii++)
		ScreenToClient(&m_vRc[ii]);
}

void CUSetup::positionView(int type)
{
	const int	pos = m_vRc[0].Width()/3;

	m_selectA    = -1;
	m_selectRes  = -1;
	m_selectType = type;

	fill_List();

	switch (type)
	{
	case TYPE_NONE:
		GetDlgItem(IDC_VIEW1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VIEW2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VIEW3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VIEW4)->ShowWindow(SW_HIDE);
		break;

	case TYPE_1:
		GetDlgItem(IDC_VIEW1)->SetWindowPos(NULL, m_vRc[0].left, m_vRc[0].top,
			m_vRc[0].Width(), m_vRc[0].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW2)->SetWindowPos(NULL, m_vRc[1].left, m_vRc[1].top,
			m_vRc[1].Width(), m_vRc[1].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW3)->SetWindowPos(NULL, m_vRc[2].left, m_vRc[2].top,
			m_vRc[2].Width(), m_vRc[2].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW4)->SetWindowPos(NULL, m_vRc[3].left, m_vRc[3].top,
			m_vRc[3].Width(), m_vRc[3].Height(), SWP_SHOWWINDOW);

		GetDlgItem(IDC_VIEW1)->SetWindowText("A(0)");
		GetDlgItem(IDC_VIEW2)->SetWindowText("A(0)");
		GetDlgItem(IDC_VIEW3)->SetWindowText("A(0)");
		GetDlgItem(IDC_VIEW4)->SetWindowText("A(0)");
		break;

	case TYPE_2:
		GetDlgItem(IDC_VIEW1)->SetWindowPos(NULL, m_vRc[0].left, m_vRc[0].top,
			m_vRc[0].Width(), m_vRc[0].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW2)->SetWindowPos(NULL, m_vRc[1].left, m_vRc[1].top,
			m_vRc[1].Width(), m_vRc[1].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW3)->SetWindowPos(NULL, m_vRc[2].left, m_vRc[2].top,
			pos*2, m_vRc[2].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW4)->SetWindowPos(NULL, m_vRc[2].left+pos*2, m_vRc[3].top,
			m_vRc[3].right - (m_vRc[2].left + pos*2), m_vRc[3].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW1)->SetWindowText("B(0)");
		GetDlgItem(IDC_VIEW2)->SetWindowText("C(0)");
		GetDlgItem(IDC_VIEW3)->SetWindowText("D(0)");
		GetDlgItem(IDC_VIEW4)->SetWindowText("A(0)");
		break;

	case TYPE_3:
		GetDlgItem(IDC_VIEW1)->SetWindowPos(NULL, m_vRc[0].left, m_vRc[0].top,
			m_vRc[0].Width(), m_vRc[0].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW2)->SetWindowPos(NULL, m_vRc[1].left, m_vRc[1].top,
			m_vRc[1].Width(), m_vRc[1].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW3)->SetWindowPos(NULL, m_vRc[2].left, m_vRc[2].top,
			m_vRc[3].right - (m_vRc[2].left + pos*2), m_vRc[2].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW4)->SetWindowPos(NULL, m_vRc[3].right-pos*2, m_vRc[3].top,
			pos*2, m_vRc[3].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW1)->SetWindowText("B(0)");
		GetDlgItem(IDC_VIEW2)->SetWindowText("C(0)");
		GetDlgItem(IDC_VIEW3)->SetWindowText("A(0)");
		GetDlgItem(IDC_VIEW4)->SetWindowText("D(0)");
		break;

	case TYPE_4:
		GetDlgItem(IDC_VIEW1)->SetWindowPos(NULL, m_vRc[0].left, m_vRc[0].top,
			m_vRc[1].right - (m_vRc[0].left + pos*2), m_vRc[0].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW2)->SetWindowPos(NULL, m_vRc[1].right-pos*2, m_vRc[1].top,
			pos*2, m_vRc[1].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW3)->SetWindowPos(NULL, m_vRc[2].left, m_vRc[2].top,
			m_vRc[2].Width(), m_vRc[2].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW4)->SetWindowPos(NULL, m_vRc[3].left, m_vRc[3].top,
			m_vRc[3].Width(), m_vRc[3].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW1)->SetWindowText("A(0)");
		GetDlgItem(IDC_VIEW2)->SetWindowText("D(0)");
		GetDlgItem(IDC_VIEW3)->SetWindowText("B(0)");
		GetDlgItem(IDC_VIEW4)->SetWindowText("C(0)");
		break;

	case TYPE_5:
		GetDlgItem(IDC_VIEW3)->SetWindowPos(NULL, m_vRc[2].left, m_vRc[2].top,
			pos*2, m_vRc[2].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW4)->SetWindowPos(NULL, m_vRc[2].left+pos*2, m_vRc[3].top,
			m_vRc[3].right - (m_vRc[2].left + pos*2), m_vRc[3].Height(), SWP_SHOWWINDOW);

		GetDlgItem(IDC_VIEW1)->SetWindowPos(NULL, m_vRc[0].left, m_vRc[0].top,
			pos*2, m_vRc[0].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW2)->SetWindowPos(NULL, m_vRc[0].left+pos*2, m_vRc[1].top,
			m_vRc[1].right - (m_vRc[0].left + pos*2), m_vRc[1].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW3)->SetWindowPos(NULL, m_vRc[2].left, m_vRc[2].top,
			m_vRc[2].Width(), m_vRc[2].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW4)->SetWindowPos(NULL, m_vRc[3].left, m_vRc[3].top,
			m_vRc[3].Width(), m_vRc[3].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW1)->SetWindowText("D(0)");
		GetDlgItem(IDC_VIEW2)->SetWindowText("A(0)");
		GetDlgItem(IDC_VIEW3)->SetWindowText("B(0)");
		GetDlgItem(IDC_VIEW4)->SetWindowText("C(0)");
		break;

	case TYPE_6:
		GetDlgItem(IDC_VIEW4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VIEW1)->SetWindowPos(NULL, m_vRc[0].left, m_vRc[0].top,
			m_vRc[0].Width(), m_vRc[0].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW3)->SetWindowPos(NULL, m_vRc[2].left, m_vRc[2].top,
			m_vRc[2].Width(), m_vRc[2].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW2)->SetWindowPos(NULL, m_vRc[1].left, m_vRc[1].top,
			m_vRc[1].Width(), m_vRc[3].bottom - m_vRc[1].top, SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW1)->SetWindowText("A(0)");
		GetDlgItem(IDC_VIEW2)->SetWindowText("D(0)");
		GetDlgItem(IDC_VIEW3)->SetWindowText("A(0)");
		GetDlgItem(IDC_VIEW4)->SetWindowText("A(0)");
		break;

	case TYPE_7:
		GetDlgItem(IDC_VIEW1)->SetWindowPos(NULL, m_vRc[0].left, m_vRc[0].top,
			m_vRc[0].Width(), m_vRc[2].bottom - m_vRc[0].top, SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW2)->SetWindowPos(NULL, m_vRc[1].left, m_vRc[1].top,
			m_vRc[1].Width(), m_vRc[1].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW4)->SetWindowPos(NULL, m_vRc[3].left, m_vRc[3].top,
			m_vRc[3].Width(), m_vRc[3].Height(), SWP_SHOWWINDOW);
		GetDlgItem(IDC_VIEW3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_VIEW1)->SetWindowText("D(0)");
		GetDlgItem(IDC_VIEW2)->SetWindowText("A(0)");
		GetDlgItem(IDC_VIEW3)->SetWindowText("A(0)");
		GetDlgItem(IDC_VIEW4)->SetWindowText("A(0)");
		break;
	}

	for (int ii = 0; ii < 4; ii++)
		m_arS[ii].RemoveAll();
	m_slist.DeleteAllItems();
}

void CUSetup::fit(CListCtrl& list)
{
	if (list.GetSafeHwnd())
	{
		CRect	cRc;
		list.GetClientRect(cRc);
		list.SetColumnWidth(0, cRc.Width());
	}
}

void CUSetup::fit2(CListCtrl& list)
{
	if (list.GetSafeHwnd())
	{
		CRect	cRc;
		list.GetClientRect(cRc);
		const int W = cRc.Width() - list.GetColumnWidth(1);
		list.SetColumnWidth(0, W);
	}
}

void CUSetup::OnSave() 
{
	if (m_selectType == TYPE_NONE)
	{
		::MessageBox(m_hWnd, "화면 스타일을 선택하여 주십시오.", "화면스타일 오류", MB_ICONWARNING);
		return;
	}

	bool	save = false;
	int ii = 0;

	for (int ii = 0; ii < 4; ii++)
	{
		if (m_arS[ii].GetSize())
		{
			save = true;
			break;
		}
	}

	if (!save)
	{
		::MessageBox(m_hWnd, "화면목록을 없습니다.", "화면목록오류", MB_ICONWARNING);
		return;
	}
	CString	dir, file, dat, keys, name;
	dir.Format("%s\\user\\%s", m_home, m_user);
	file.Format("%s\\user\\%s\\종합.ini", m_home, m_user);

	::CreateDirectory(dir, NULL);
	GetDlgItem(IDC_NAME)->GetWindowText(name);
	if (name.IsEmpty())
	{
		::MessageBox(m_hWnd, "저장이름이 없습니다.", "저장오류", MB_ICONWARNING);
		GetDlgItem(IDC_NAME)->SetFocus();
		return;
	}
	save = true;
	for (ii = 0; ii < m_mlist.GetItemCount(); ii++)
	{
		dat = m_mlist.GetItemText(ii, 0);
		if (!name.Compare(dat))
		{
			save = false;
			break;
		}
	}

	int	nitem = m_mlist.GetItemCount();
	if (save)
	{
		char	section[128];
		keys.Format("%02d", nitem);
		addItem(m_mlist, name, nitem);
		
		GetPrivateProfileString("GENERAL", "section", "LIST", section, sizeof(section), file);
		WritePrivateProfileString("GENERAL", "section", section, file);
		WritePrivateProfileString(section, keys, name, file);
	}
	else	nitem = ii;

	dat.Format("%02d", nitem);
	WritePrivateProfileString("GENERAL", "key", dat, file);

	dat.Format("%d", m_selectType);
	WritePrivateProfileString(name, "type", dat, file);
	for (ii = 0; ii < 4; ii++)
	{
		dat.Empty();

		for (int jj = 0; jj < m_arS[ii].GetSize(); jj++)
		{
			if (jj)	
				dat += ',' + m_arS[ii].GetAt(jj).Left(8);
			else	
				dat = m_arS[ii].GetAt(jj).Left(8);
		}

		//**keys.Format("%c", ii + 'A');
		GetDlgItemText(IDC_VIEW1 + ii, keys);
		ASSERT(!keys.IsEmpty());
		WritePrivateProfileString(name, keys.Left(1), dat, file);
	}

	m_mlist.SetItemState(nitem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	m_mlist.SetFocus();
	fit(m_mlist);

	m_bChange = TRUE;
}

void CUSetup::OnDelete() 
{
	char	section[256]{};// , dat[1024 * 2]{};

	std::unique_ptr<char[]> dat = std::make_unique<char[]>(1024 * 2);


	CString	tfile, file, keys, name;
	file.Format("%s\\user\\%s\\종합.ini", m_home, m_user);
	tfile.Format("%s\\user\\%s\\종합", m_home, m_user);

	GetDlgItem(IDC_NAME)->GetWindowText(name);
	int	idx = m_mlist.GetNextItem(-1, LVNI_SELECTED);
	if (idx == -1)
	{
		::MessageBox(m_hWnd, "선택된 목록이 없습니다.", "목록오류", MB_ICONSTOP);
		return;
	}
	m_mlist.DeleteItem(idx);

	GetPrivateProfileString("GENERAL", "section", "LIST", section, sizeof(section), file);
	WritePrivateProfileString("GENERAL", "section", section, tfile);
	WritePrivateProfileString("GENERAL", "key", "00", tfile);

	for (int ii = 0, jj = 0; ; ii++)
	{
		if (ii == idx)	continue;
		//**keys.Format("%02d", ii);
		keys.Format("%c", ii + 'A');
		const DWORD dw = GetPrivateProfileString(section, keys, "", dat.get(), sizeof(dat), file);
		if (dw <= 0)	
			break;
		if (ii != jj)	
			//**keys.Format("%02d", jj);
			keys.Format("%c", jj + 'A');
		WritePrivateProfileString(section, keys, dat.get(), tfile);

		CString	sec = dat.get();
		GetPrivateProfileSection(sec, dat.get(), sizeof(dat.get()), file);
		WritePrivateProfileSection(sec, dat.get(), tfile);

		jj++;
	}

	::CopyFile(tfile, file, FALSE);
	::DeleteFile(tfile);

	if (idx >= m_mlist.GetItemCount())
		idx = m_mlist.GetItemCount() -1;
	if (!m_mlist.GetItemCount())
	{
		GetDlgItem(IDC_NAME)->SetWindowText(_T("사용자 종합화면"));
		file.Format("%s\\tab\\종합config.ini", m_home);
		WritePrivateProfileString("GENERAL", "option", "", file);
	}
	else
	{
		m_mlist.SetItemState(idx, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		keys.Format("%02d", idx);
		WritePrivateProfileString("GENERAL", "key", keys, file);
	}

	m_mlist.SetFocus();
	fit(m_mlist);

	m_bChange = TRUE;
}

void CUSetup::load_Slist()
{
	m_slist.DeleteAllItems();
	if (m_selectA == -1)	return;

	int	idx{}, pos{};
	CString	string, tmpS;

	for (int ii = 0; ii < m_arS[m_selectA].GetSize(); ii++)
	{
		string = m_arS[m_selectA].GetAt(ii);
		idx = m_slist.GetItemCount();
		pos = string.Find('=');

		tmpS = string.Mid(pos+1, string.Find(';', pos) - (pos+1));

		addItem(m_slist, tmpS, idx);

		pos = string.Find(';');
		tmpS = string.Mid(pos+1);

		pos = tmpS.Find(';');
		tmpS = tmpS.Mid(0, pos);
		addItem(m_slist, tmpS, idx, 1);
	}
	countView(m_selectA);
}

void CUSetup::OnItemchangedUsermaplist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if (pNMListView->uNewState)
	{
		CString text = m_mlist.GetItemText(pNMListView->iItem, pNMListView->iSubItem);
		GetDlgItem(IDC_NAME)->SetWindowText(text);
		change_UserConfig();
	}
	*pResult = 0;
}
	
void CUSetup::OnClickUsermaplist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem == -1) return;
	CString text = m_mlist.GetItemText(pNMListView->iItem, pNMListView->iSubItem);
	GetDlgItem(IDC_NAME)->SetWindowText(text);
	*pResult = 0;
}

void CUSetup::change_UserConfig()
{
	CString		file, appS;
	char	dat[512]{};
	int ii = 0;
	for (ii = 0; ii < 4; ii++)
		m_arS[ii].RemoveAll();
	m_slist.DeleteAllItems();
	GetDlgItem(IDC_NAME)->GetWindowText(appS);
	
	file.Format("%s\\user\\%s\\종합.ini", m_home, m_user);
	m_selectType = GetPrivateProfileInt(appS, "type", 2, file);
	if (m_selectType < 2)
		m_selectType = 2;
	else if (m_selectType > 5)
		m_selectType = 5;

	positionView(m_selectType);

	for (ii = 0; ii < 4; ii++)
	{
		int	pos{};
		CString	keys, mapN, str, info;
		
		GetDlgItemText(IDC_VIEW1 + ii, keys);
		ASSERT(!keys.IsEmpty());

		DWORD dw = GetPrivateProfileString(appS, keys.Left(1), "", dat, sizeof(dat), file);
		if (dw > 0)
		{
			str = dat;
			while (!str.IsEmpty())
			{
				pos = str.Find(',');
				if (pos == -1)
				{
					info = mapInfo(str);
					if (!info.IsEmpty())
						m_arS[ii].Add(info);
					break;
				}
				mapN = str.Left(pos);
				info = mapInfo(mapN);
				str = str.Mid(pos+1);

				if (!info.IsEmpty())
					m_arS[ii].Add(info);
			}
			
		}
	}

	m_selectA = -1;
	m_selectRes = -1;
	for (ii = 0; ii < 4; ii++)
		countView(ii);
}

CString CUSetup::mapInfo(CString mapN)
{
	char	buf[256];
	CString	info, file;

	mapN.TrimLeft();	mapN.TrimRight();
	file.Format("%s\\tab\\종합list.ini", m_home);
	DWORD dw = GetPrivateProfileString("STOCK", mapN, "", buf, sizeof(buf), file);
	if (dw > 0)
	{
		info = mapN + '=';
		info += buf;
		return info;
	}

	dw = GetPrivateProfileString("FOPTION", mapN, "", buf, sizeof(buf), file);
	if (dw > 0)
	{
		info = mapN + '=';
		info += buf;
		return info;
	}

	return info;
}

void CUSetup::OnBtnAddall() 
{
	// TODO: Add your control notification handler code here
	if (m_selectType == TYPE_NONE)
	{
		::MessageBox(m_hWnd, "화면 스타일을 선택하여 주십시오.", "화면스타일 선택", MB_ICONWARNING);
		return;
	}
	
	if (m_selectRes == -1)
	{
		::MessageBox(m_hWnd, "화면 영역을 선택하여 주십시오.", "화면영역 선택", MB_ICONWARNING);
		return;
	}

	int pos{}, idx{};
	CString	string, tmpS, dat;

	bool isSame{};

	for(int ii=0 ; ii<m_list.GetItemCount() ; ii++)
	{
		idx = ii;

		string = m_arInfo.GetAt(idx);
		idx = m_slist.GetItemCount();
		pos = string.Find('=');
		tmpS = string.Left(8);

		isSame = false;

		for (int j = 0; j < m_slist.GetItemCount(); j++)
		{
			dat = m_arS[m_selectA].GetAt(j);
			dat = dat.Left(8);

			if (!dat.Compare(tmpS))
			{
				isSame = true;
				break;
			}
		} 
		
		if(isSame == true)
			continue;

		m_arS[m_selectA].Add(string);
		tmpS = string.Mid(pos+1, string.Find(';', pos) - (pos+1));
		tmpS.TrimLeft();	tmpS.TrimRight();

		addItem(m_slist, tmpS, idx);

		pos = string.Find(';');
		tmpS = string.Mid(pos+1);
		pos = tmpS.Find(';');
		tmpS = tmpS.Mid(0, pos);

		addItem(m_slist, tmpS, idx, 1);

		countView(m_selectA);

		
		fit2(m_slist);
	}

}

void CUSetup::OnBtnDelall() 
{
	// TODO: Add your control notification handler code here
	int idx{};
	
	const int cnt = m_slist.GetItemCount();

	for(int ii=0 ; ii<cnt ; ii++)
	{
		idx = 0;
		m_slist.DeleteItem(idx);
		CString	dat = m_arS[m_selectA].GetAt(idx);
		m_arS[m_selectA].RemoveAt(idx);
		countView(m_selectA);
		fit2(m_slist);
	}
}
