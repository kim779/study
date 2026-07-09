// SetTickInfo.cpp : implementation file
//

#include "stdafx.h"
#include "SetTickInfo.h"
#include "smdefine.h"
#include "pane.h"

#include <afxtempl.h>
#include "../../h/interSt.h"
#include "../../h/axisvar.h"

#define INACTIVE	WS_EX_DLGMODALFRAME|WS_EX_WINDOWEDGE
#define ACTIVE		WS_EX_STATICEDGE
#define ACTIVE_COLOR	RGB(255, 255, 255)

#define TICKBAR_CNT	2
#define SPLIT_CNT	6
#define NEWSGAP		1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTickInfo dialog


CSetTickInfo::CSetTickInfo(CString home, CString userN, int id, CWnd* pParent /*=NULL*/)
	: CDialog(CSetTickInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTickInfo)
	m_bar1cnt = 0;
	m_bar2cnt = 0;
	m_view1 = FALSE;
	m_view2 = FALSE;
	m_screenbar = FALSE;
	//}}AFX_DATA_INIT
	m_home		= home;
	m_userName	= userN;
	m_active	= id;

	m_brush.CreateSolidBrush(ACTIVE_COLOR);
}

CSetTickInfo::~CSetTickInfo()
{
	_tickitem* item;
	for (int ii = 0; ii < m_arItemInfo.GetSize(); ii++)
	{
		item = (_tickitem*) m_arItemInfo.GetAt(ii);
		delete item;
	}
	m_arItemInfo.RemoveAll();

	m_brush.DeleteObject();
}

void CSetTickInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTickInfo)
	DDX_Control(pDX, IDC_COLOR, m_cbColor);
	DDX_Control(pDX, IDC_PANESPIN2, m_spin2);
	DDX_Control(pDX, IDC_PANESPIN1, m_spin1);
	DDX_Control(pDX, IDC_INTERGROUP, m_interCom);
	DDX_Control(pDX, IDC_UPDOWNLIST, m_updnCom);
	DDX_Control(pDX, IDC_INDEXLIST, m_indexList);
	DDX_Control(pDX, IDC_NEWSLIST, m_newsList);
	DDX_Text(pDX, IDC_PANECOUNT1, m_bar1cnt);
	DDX_Text(pDX, IDC_PANECOUNT2, m_bar2cnt);
	DDX_Check(pDX, IDC_VIEWTICK1, m_view1);
	DDX_Check(pDX, IDC_VIEWTICK2, m_view2);
	DDX_Check(pDX, IDC_SCREENBAR, m_screenbar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetTickInfo, CDialog)
	//{{AFX_MSG_MAP(CSetTickInfo)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_PANECOUNT1, OnChangePanecount1)
	ON_EN_CHANGE(IDC_PANECOUNT2, OnChangePanecount2)
	ON_CBN_SELCHANGE(IDC_FONT, OnSelchangeFont)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ON_CHKBOX, OnChangeLIST)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTickInfo message handlers

BOOL CSetTickInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();

	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetTickInfo::Init()
{
	m_indexList.InsertColumn(0, "index Item");
	m_indexList.SetCheckboxeStyle(RC_CHKBOX_NORMAL);	// Enable checkboxes

	m_newsList.InsertColumn(0, "news Item");
	m_newsList.SetCheckboxeStyle(RC_CHKBOX_NORMAL);		// Enable checkboxes

	int	idx = 0;
	m_indexList.InsertItem(idx++, "KRX100");		// 1	
	m_indexList.InsertItem(idx++, "KOSPI");			// 2
	m_indexList.InsertItem(idx++, "KOSDAQ");		// 3
	m_indexList.InsertItem(idx++, "KOSPI200");		// 4
	m_indexList.InsertItem(idx++, "KOSPI100");		// 5
	m_indexList.InsertItem(idx++, "배당지수");		// 6
	m_indexList.InsertItem(idx++, "KOSTAR");		// 7
	m_indexList.InsertItem(idx++, "거래소 BASIS");		// 8
	m_indexList.InsertItem(idx++, "거래소 선물");		// 9
	m_indexList.InsertItem(idx++, "스타선물");		// 10
	m_indexList.InsertItem(idx++, "S&P500선물");		// 11
	m_indexList.InsertItem(idx++, "NASDAQ100선물");		// 12
	m_indexList.InsertItem(idx++, "환율");			// 13
								// 14 거래소 등락
								// 15 코스닥 등락
								// 16 news
	m_indexList.InsertItem(idx++, "NIKKEI225");		// 17 
	m_indexList.InsertItem(idx++, "항셍H지수");		// 18 

	idx = 0;
	m_newsList.InsertItem(idx++, "증전공시");		
	m_newsList.InsertItem(idx++, "이데일리");		
	m_newsList.InsertItem(idx++, "머니투데이");		
	m_newsList.InsertItem(idx++, "한국경제");		
	m_newsList.InsertItem(idx++, "연합뉴스");		
	m_newsList.InsertItem(idx++, "인포스탁");		
	m_newsList.InsertItem(idx++, "FnGuide");		
	m_newsList.InsertItem(idx++, "해외뉴스");		

	CRect	cRc;
	m_indexList.GetClientRect(cRc);
	m_indexList.SetColumnWidth(0, cRc.Width());

	m_newsList.GetClientRect(cRc);
	m_newsList.SetColumnWidth(0, cRc.Width());

	loadInfo();

	CComboBox* uplist = (CComboBox*) GetWnd(IDC_UPDOWNLIST);
	uplist->SetCurSel(0);
	CButton* button = (CButton*) GetWnd(IDC_UPS_TEXT);button->SetCheck(1);
	button = (CButton*) GetWnd(IDC_DBARROW);	button->SetCheck(1);

	if (m_bar1cnt < 1 || m_bar1cnt > 6)	m_bar1cnt = 6;
	if (m_bar2cnt < 1 || m_bar2cnt > 6)	m_bar2cnt = 6;
	UpdateData(FALSE);

	m_spin1.SetRange(1, 6);
	m_spin2.SetRange(1, 6);
	m_spin1.SetBuddy(GetWnd(IDC_PANECOUNT1));
	m_spin2.SetBuddy(GetWnd(IDC_PANECOUNT2));

	SetSplit(m_bar1cnt);
	SetSplit(m_bar2cnt, 1);

	SelectTicker();
	UpdateInfoText(true);
}

void CSetTickInfo::loadInfo()
{
	CString	file, section, keys, value;
	int	nRes;
	char	buf[256];

	file.Format("%s\\user\\%s\\%s", m_home, m_userName, TICKSETUP);
	m_bar1cnt	= GetPrivateProfileInt("GENERAL", "BAR00", SPLIT_CNT, file);
	m_bar2cnt	= GetPrivateProfileInt("GENERAL", "BAR01", SPLIT_CNT, file);
	m_view1		= GetPrivateProfileInt("GENERAL", "view1", 1, file);
	m_view2		= GetPrivateProfileInt("GENERAL", "view2", 1, file);
	m_screenbar	= GetPrivateProfileInt("GENERAL", "screenbar", 1, file);
	UpdateData(FALSE);

	m_fontsize = GetPrivateProfileInt("GENERAL", "fontsize", 9, file);
	switch (m_fontsize)
	{
	default:
	case 9:		nRes = IDC_9F;		break;
	case 10:	nRes = IDC_10F;		break;
	case 11:	nRes = IDC_11F;		break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(1);

	m_bold = GetPrivateProfileInt("GENERAL", "bold", 0, file);
	if (m_bold)	((CButton *) GetDlgItem(IDC_BOLD))->SetCheck(1);
	else		((CButton *) GetDlgItem(IDC_NORMAL))->SetCheck(1);

	GetPrivateProfileString("GENERAL", "fontname", "굴림체", buf, sizeof(buf), file);
	m_fontname = buf;	m_fontname.TrimLeft();		m_fontname.TrimRight();
	((CComboBox *)GetDlgItem(IDC_FONT))->SelectString(-1, m_fontname);

	setFont();

	m_period = GetPrivateProfileInt("GENERAL", "period", 5, file);
	value.Format("%d", m_period);
	((CComboBox *)GetDlgItem(IDC_PERIOD))->SelectString(-1, value);

	m_color = GetPrivateProfileInt("GENERAL", "color", GetColor(COLOR_TICK_INSIDE), file);
	m_cbColor.SetColor(m_color);
	
	loadInter();
	for (int ii = 0; ii < TICKBAR_CNT; ii++)
	{
		for (int jj = 0; jj < SPLIT_CNT; jj++)
		{
			_tickitem* item = new _tickitem;

			section.Format("BAR_INFO_%d%d", ii, jj);
			item->kind = GetPrivateProfileInt(section, "INDEXK", 0, file);
			switch (item->kind)
			{
			case TKIND_INDEX:
				{
					for (int kk = 0, hh; ; kk++)
					{
						keys.Format("%03d", kk);
						hh = GetPrivateProfileInt(section, keys, -1, file);
						if (hh == -1)	break;
						value.Format("%d", hh);
						item->sAry.Add(value);
					}
					keys = _T("AUX");
					item->aux = GetPrivateProfileInt(section, keys, 0, file);

					if (!item->sAry.GetSize())	item->sAry.Add("1");
				}
				break;
			case TKIND_UPDOWN:
				{
					keys = _T("SELECT");
					item->selectN = GetPrivateProfileInt(section, keys, 0, file);
					keys = _T("AUX");
					item->aux = GetPrivateProfileInt(section, keys, 1, file);
					if (item->aux < 1 || item->aux > 2)	item->aux = 1;
				}
				break;
			case TKIND_NEWS:
				{
					item->aux = 0;
					item->sAry.RemoveAll();
					item->selectN = 0;
					for (int kk = 0, hh; ; kk++)
					{
						keys.Format("%02d", kk);
						hh = GetPrivateProfileInt(section, keys, -1, file);
						if (hh == -1)	break;
						value.Format("%d", hh);
						item->sAry.Add(value);
					}
					keys = _T("AUX");
					item->aux = GetPrivateProfileInt(section, keys, 0, file);

					if (!item->sAry.GetSize())	item->sAry.Add("1");
				}
				break;
			case TKIND_INTER:
				if (m_interCom.GetCount())
				{
					item->selectN = GetPrivateProfileInt(section, "SELECT", 0, file);
					m_interCom.SetCurSel(0);
				}
				else	item->selectN = -1;
				break;
			case TKIND_CONCLUSION:
				break;
			default:
				AfxMessageBox("Tick infomation load Error!");
				continue;
			}
			m_arItemInfo.Add((CObject *) item);
		}
	}
}

void CSetTickInfo::saveInfo()
{
	CString		section, keys, value, file, name;
	int		index = 0;

	file.Format("%s\\user\\%s\\%s", m_home, m_userName, TICKSETUP);

	value.Format("%d", m_bar1cnt);
	WritePrivateProfileString("GENERAL", "BAR00", value, file);
	value.Format("%d", m_bar2cnt);
	WritePrivateProfileString("GENERAL", "BAR01", value, file);

	UpdateData();
	value.Format("%d", m_view1);
	WritePrivateProfileString("GENERAL", "view1", value, file);
	value.Format("%d", m_view2);
	WritePrivateProfileString("GENERAL", "view2", value, file);
	value.Format("%d", m_screenbar);
	WritePrivateProfileString("GENERAL", "screenbar", value, file);

	value.Format("%d", m_fontsize);
	WritePrivateProfileString("GENERAL", "fontsize", value, file);

	WritePrivateProfileString("GENERAL", "fontname", m_fontname, file);

	value.Format("%d", m_bold);
	WritePrivateProfileString("GENERAL", "bold", value, file);

	((CComboBox *) GetDlgItem(IDC_PERIOD))->GetWindowText(value);
	WritePrivateProfileString("GENERAL", "period", value, file);

	value.Format("%d", m_cbColor.GetColor());
	WritePrivateProfileString("GENERAL", "color", value, file);

	for (int ii = 0; ii < TICKBAR_CNT; ii++)
	{
		for (int jj = 0; jj < SPLIT_CNT; jj++)
		{
			_tickitem* item = (_tickitem*) m_arItemInfo.GetAt(index++);
			section.Format("BAR_INFO_%d%d", ii, jj);

			value.Format("%d", item->kind);
			WritePrivateProfileSection(section, "", file);
			WritePrivateProfileString(section, "INDEXK", value, file);
			switch (item->kind)
			{
			case TKIND_INDEX:
				{
					if (item->sAry.GetSize() <= 0)	item->sAry.Add("1");
					for (int kk = 0; kk < item->sAry.GetSize(); kk++)
					{
						keys.Format("%03d", kk);
						value = item->sAry.GetAt(kk);
						WritePrivateProfileString(section, keys, value, file);
					}
					keys = _T("AUX");
					value.Format("%d", item->aux);
					WritePrivateProfileString(section, keys, value, file);
				}
				break;
			case TKIND_UPDOWN:
				{
					keys = _T("SELECT");
					value.Format("%d", item->selectN);
					WritePrivateProfileString(section, keys, value, file);

					keys = _T("AUX");
					value.Format("%d", item->aux);
					WritePrivateProfileString(section, keys, value, file);
				}
				break;
			case TKIND_NEWS:
				{
					if (item->sAry.GetSize() <= 0)	item->sAry.Add("1");
					for (int kk = 0; kk < item->sAry.GetSize(); kk++)
					{
						keys.Format("%02d", kk);
						value = item->sAry.GetAt(kk);
						WritePrivateProfileString(section, keys, value, file);
					}
					keys = _T("AUX");
					value.Format("%d", item->aux);
					WritePrivateProfileString(section, keys, value, file);
				}
				break;
			case TKIND_INTER:
				{
					keys = _T("SELECT");
					value.Format("%d", item->selectN);
					WritePrivateProfileString(section, keys, value, file);
					writeInterCode(section, item->selectN);
				}
				break;
			case TKIND_CONCLUSION:
				break;
			default:
				AfxMessageBox("Tick infomation save Error!");
				continue;
			}
		}
	}
}

void CSetTickInfo::SetSplit(int cnt, int idx)
{
	int		ii = 0;
	int		width;
	CRect		sRc, bRc;
	CWnd*		wnd;
	CObArray	ary;

	switch (idx)
	{
	case 0:
		wnd = GetWnd(IDC_PANEBORDER1);
		wnd->GetWindowRect(sRc);
		ary.Add(GetWnd(IDC_PANE1_1));
		ary.Add(GetWnd(IDC_PANE1_2));
		ary.Add(GetWnd(IDC_PANE1_3));
		ary.Add(GetWnd(IDC_PANE1_4));
		ary.Add(GetWnd(IDC_PANE1_5));
		ary.Add(GetWnd(IDC_PANE1_6));
		break;
	case 1:
		wnd = GetWnd(IDC_PANEBORDER2);
		wnd->GetWindowRect(sRc);
		ary.Add(GetWnd(IDC_PANE2_1));
		ary.Add(GetWnd(IDC_PANE2_2));
		ary.Add(GetWnd(IDC_PANE2_3));
		ary.Add(GetWnd(IDC_PANE2_4));
		ary.Add(GetWnd(IDC_PANE2_5));
		ary.Add(GetWnd(IDC_PANE2_6));
		break;
	}

	if (cnt > ary.GetSize())
	{
		ary.RemoveAll();
		return;
	}

	width = sRc.Width()/cnt;
	ScreenToClient(&sRc);

	bRc.SetRect(sRc.left, sRc.top, sRc.left + width, sRc.bottom);
	for (ii = 0; ii < cnt; ii++)
	{
		wnd = (CWnd *) ary.GetAt(ii);
		if (ii && (ii == cnt - 1))
		{
			bRc.SetRect(sRc.left + width*ii, sRc.top,
					sRc.right, sRc.bottom);
			wnd->MoveWindow(bRc);
		}
		else
		{
			wnd->MoveWindow(bRc);
			bRc.OffsetRect(width, 0);
		}
	}

	for (ii = 0; ii < ary.GetSize(); ii++)
	{
		wnd = (CWnd *) ary.GetAt(ii);
		if (ii < cnt)
		{
			wnd->ShowWindow(SW_SHOW);
			wnd->Invalidate();
		}
		else	wnd->ShowWindow(SW_HIDE);
	}

	wnd = (CWnd *) ary.GetAt(m_active%10);
	if (!(wnd->GetStyle() & WS_VISIBLE))
	{
		wnd = (CWnd *) ary.GetAt(0);
		m_active = idx*10;
		SelectTicker();
	}
	ary.RemoveAll();
}

void CSetTickInfo::SelectTicker()
{
	CObArray	ary;
	CWnd*		wnd;
	int		ii, bIdx = m_active/10;
	int		iIdx = m_active%10;

	for (ii = 0; ii < TICKBAR_CNT; ii++)
	{
		GetSplitItemArray(ii, ary);
		for (int jj = 0; jj < ary.GetSize(); jj++)
		{
			wnd = (CWnd*) ary.GetAt(jj);
			if (ii == bIdx)
			{
				if (jj == iIdx)
					wnd->ModifyStyleEx(INACTIVE, ACTIVE, SWP_DRAWFRAME);
				else if (wnd->GetExStyle() & ACTIVE)
					wnd->ModifyStyleEx(ACTIVE, INACTIVE, SWP_DRAWFRAME);
				else	continue;
			}
			else
			{
				if (wnd->GetExStyle() & ACTIVE)
					wnd->ModifyStyleEx(ACTIVE, INACTIVE, SWP_DRAWFRAME);
				else	continue;
			}
			wnd->Invalidate();
		}
	}
	ary.RemoveAll();

	_tickitem* item = (_tickitem*) m_arItemInfo.GetAt(SPLIT_CNT*bIdx + iIdx);
	GetKindAry(ary);
	for (ii = 0; ii < ary.GetSize(); ii++)
	{
		wnd = (CWnd *) ary.GetAt(ii);
		if (ii == item->kind)
			((CButton *) wnd)->SetCheck(1);
		else	((CButton *) wnd)->SetCheck(0);
	}
	SetItemInfo();
}

void CSetTickInfo::UpdateInfoText(bool all/* = false*/)
{
	CObArray	ary;
	_tickitem*	item = NULL;
	CWnd*		wnd = NULL;

	if (all)
	{
		int	index = 0;
		for (int ii = 0; ii < TICKBAR_CNT; ii++)
		{
			GetSplitItemArray(ii, ary);
			for (int jj = 0; jj < SPLIT_CNT; jj++)
			{
				item = (_tickitem*) m_arItemInfo.GetAt(index++);
				wnd = (CWnd*) ary.GetAt(jj);
				wnd->SetWindowText(GetPaneString(item));
			}
		}
	}
	else
	{
		int	bIdx = m_active/10;
		int	iIdx = m_active%10;
		int	idx = SPLIT_CNT*bIdx + iIdx;

		item = (_tickitem*) m_arItemInfo.GetAt(idx);
		GetSplitItemArray(bIdx, ary);
		wnd = (CWnd*) ary.GetAt(iIdx);
		wnd->SetWindowText(GetPaneString(item));
	}
	ary.RemoveAll();
}

void CSetTickInfo::GetSplitItemArray(int barIndex, CObArray& ary)
{
	ary.RemoveAll();
	switch (barIndex)
	{
	case 0:
		ary.Add(GetWnd(IDC_PANE1_1));
		ary.Add(GetWnd(IDC_PANE1_2));
		ary.Add(GetWnd(IDC_PANE1_3));
		ary.Add(GetWnd(IDC_PANE1_4));
		ary.Add(GetWnd(IDC_PANE1_5));
		ary.Add(GetWnd(IDC_PANE1_6));
		break;
	case 1:
		ary.Add(GetWnd(IDC_PANE2_1));
		ary.Add(GetWnd(IDC_PANE2_2));
		ary.Add(GetWnd(IDC_PANE2_3));
		ary.Add(GetWnd(IDC_PANE2_4));
		ary.Add(GetWnd(IDC_PANE2_5));
		ary.Add(GetWnd(IDC_PANE2_6));
		break;
	}
}

void CSetTickInfo::GetKindAry(CObArray& ary)
{
	ary.Add(GetWnd(IDC_SETINDEX));
	ary.Add(GetWnd(IDC_SETNEWS));
	ary.Add(GetWnd(IDC_SETUPDOWN));
	ary.Add(GetWnd(IDC_SETINTERCODE));
	ary.Add(GetWnd(IDC_SETCONCLUSION));
}

void CSetTickInfo::SetItemInfo()
{
	int	bIdx = m_active/10;
	int	iIdx = m_active%10;
	int	idx = SPLIT_CNT*bIdx + iIdx;

	CString		key, value;
	int		indexN;

	_tickitem* item = (_tickitem*) m_arItemInfo.GetAt(idx);
	SetStatus(item->kind);

	switch (item->kind)
	{
	case TKIND_INDEX:
		{
			m_indexList.SetAllItemStates(RC_ITEM_CHECKED, RC_ITEM_UNCHECKED);

			for (int ii = 0; ii < item->sAry.GetSize(); ii++)
			{
				key = item->sAry.GetAt(ii);
				indexN = atoi(key);//matchItemN(atoi(key));
				if (indexN >= m_indexList.GetItemCount())
					continue;
				m_indexList.SetItemStates(indexN, RC_ITEM_CHECKED);
			}
			
			if (item->aux)
			{
				((CButton*) GetWnd(IDC_DBARROW))->SetCheck(0);
				((CButton*) GetWnd(IDC_JISUVOL))->SetCheck(1);
			}
			else
			{
				((CButton*) GetWnd(IDC_DBARROW))->SetCheck(1);
				((CButton*) GetWnd(IDC_JISUVOL))->SetCheck(0);
			}
		}
		break;
	case TKIND_NEWS:
		{
			m_newsList.SetAllItemStates(RC_ITEM_CHECKED, RC_ITEM_UNCHECKED);

			for (int ii = 0; ii < item->sAry.GetSize(); ii++)
			{
				key = item->sAry.GetAt(ii);
				indexN = atoi(key) - NEWSGAP;
				if (indexN >= m_newsList.GetItemCount())
					continue;
				m_newsList.SetItemStates(indexN, RC_ITEM_CHECKED);
			}
		}
		break;
	case TKIND_UPDOWN:
		{
			m_updnCom.SetCurSel(item->selectN);
			for (int ii = 1; ii <= 2; ii++)
			{
				UINT	nRes;
				switch (ii)
				{
				case 1:	nRes = IDC_UPS_TEXT;	break;
				case 2:	nRes = IDC_UPS_GRAPH;	break;
				}

				if (ii == item->aux)
					((CButton *) GetWnd(nRes))->SetCheck(1);
				else	((CButton *) GetWnd(nRes))->SetCheck(0);
			}

			if (!item->aux)	((CButton *) GetWnd(IDC_UPS_TEXT))->SetCheck(1);
		}
		break;
	case TKIND_INTER:
		if (m_interCom.GetCount())
			m_interCom.SetCurSel(item->selectN);
		break;
	case TKIND_CONCLUSION:
		break;
	default:
		::MessageBox(m_hWnd, "Information file error!", "Error", MB_ICONSTOP);
		break;
	}
}

CString CSetTickInfo::GetPaneString(_tickitem* item)
{
	CString		keys, str, value;
	int		cnt = 0;

	switch (item->kind)
	{
	case TKIND_INDEX:
		if (item->sAry.GetSize() == 0)
			str = _T("지수");
		else if (item->sAry.GetSize() == 1)
		{
			keys = item->sAry.GetAt(0);
			str = GetIndexString(atoi(keys));
		}
		else	str = _T("복합");
		break;
	case TKIND_NEWS:
		str = _T("뉴스");
		break;
	case TKIND_UPDOWN:
		str = _T("등락");
		break;
	case TKIND_INTER:
		str = _T("관심종목");
		break;
	case TKIND_CONCLUSION:
		str = _T("체결통보");
		break;
	}

	return str;
}

void CSetTickInfo::SetStatus(int kind)
{
	bool	bEnableIDX = false, bEnableNEWS = false;
	m_indexList.EnableWindow(FALSE);
	m_newsList.EnableWindow(FALSE);

	m_updnCom.EnableWindow(FALSE);
	GetWnd(IDC_UPS_TEXT)->EnableWindow(FALSE);
	GetWnd(IDC_UPS_GRAPH)->EnableWindow(FALSE);
	GetWnd(IDC_DBARROW)->EnableWindow(FALSE);
	GetWnd(IDC_JISUVOL)->EnableWindow(FALSE);

	m_interCom.EnableWindow(FALSE);

	switch (kind)
	{
	default:
	case TKIND_INDEX:
		bEnableIDX = true;
		m_indexList.EnableWindow(TRUE);
		GetWnd(IDC_DBARROW)->EnableWindow(TRUE);
		GetWnd(IDC_JISUVOL)->EnableWindow(TRUE);
		break;
	case TKIND_NEWS:
		bEnableNEWS = true;
		m_newsList.EnableWindow(TRUE);
		break;
	case TKIND_UPDOWN:
		m_updnCom.EnableWindow(TRUE);
		GetWnd(IDC_UPS_TEXT)->EnableWindow(TRUE);
		GetWnd(IDC_UPS_GRAPH)->EnableWindow(TRUE);
		break;
	case TKIND_INTER:
		m_interCom.EnableWindow(TRUE);
		break;
	case TKIND_CONCLUSION:
		break;
	}

	COLORREF	bkColor, textColor;
	if (bEnableIDX)
	{
		bkColor = ACTIVE_COLOR;
		textColor = GetSysColor(COLOR_WINDOWTEXT);
	}
	else
	{
		bkColor = GetSysColor(COLOR_BTNFACE);
		textColor = GetSysColor(COLOR_GRAYTEXT);
	}

	m_indexList.SetBkColor(bkColor);
	m_indexList.SetItemBkColor(-1, -1, bkColor);
	m_indexList.SetItemTextColor(-1, -1, textColor);

	if (bEnableNEWS)
	{
		bkColor = ACTIVE_COLOR;
		textColor = GetSysColor(COLOR_WINDOWTEXT);
	}
	else
	{
		bkColor = GetSysColor(COLOR_BTNFACE);
		textColor = GetSysColor(COLOR_GRAYTEXT);
	}

	m_newsList.SetBkColor(bkColor);
	m_newsList.SetItemBkColor(-1, -1, bkColor);
	m_newsList.SetItemTextColor(-1, -1, textColor);
}

BOOL CSetTickInfo::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			pMsg->wParam = VK_TAB;
		break;
	case WM_LBUTTONDOWN:
		ProcessLbutton(CPoint(pMsg->pt));
		break;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CSetTickInfo::ProcessLbutton(CPoint pt)
{
	CWnd*		wnd;
	bool		match = false;
	CRect		iRect;
	CObArray	ary;
	int			ii = 0;
	
	GetSplitItemArray(0, ary);
	for (ii = 0; ii < (int)m_bar1cnt && !match; ii++)
	{
		wnd = (CWnd*) ary.GetAt(ii);
		wnd->GetWindowRect(&iRect);
		if (iRect.PtInRect(pt))
		{
			if (ii == m_active)
				continue;
			match = true;
			m_active = ii;
		}
	}

	GetSplitItemArray(1, ary);
	for (ii = 0; ii < (int)m_bar2cnt && !match; ii++)
	{
		wnd = (CWnd*) ary.GetAt(ii);
		wnd->GetWindowRect(&iRect);
		if (iRect.PtInRect(pt))
		{
			if (ii + 10 == m_active)
				continue;
			match = true;
			m_active = ii + 10;
		}
	}

	ary.RemoveAll();

	if (match)	SelectTicker();
}

HBRUSH CSetTickInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT nRes = pWnd->GetDlgCtrlID();
	switch (nRes)
	{
	case IDC_PANE1_1:case IDC_PANE1_2:case IDC_PANE1_3:case IDC_PANE1_4:case IDC_PANE1_5:case IDC_PANE1_6:
	case IDC_PANE2_1:case IDC_PANE2_2:case IDC_PANE2_3:case IDC_PANE2_4:case IDC_PANE2_5:case IDC_PANE2_6:
		if (pWnd->GetExStyle() & ACTIVE)
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = m_brush;
		}
		break;
	}
	return hbr;
}

LRESULT CSetTickInfo::OnChangeLIST(WPARAM wParam, LPARAM lParam)
{
	int	bIdx = m_active/10;
	int	iIdx = m_active%10;
	int	idx = SPLIT_CNT*bIdx + iIdx;
	
	_tickitem*	item = (_tickitem*) m_arItemInfo.GetAt(idx);
	item->sAry.RemoveAll();
	if (item->kind == TKIND_INDEX)
	{
		for (int ii = 0; ii < m_indexList.GetItemCount(); ii++)
		{
			if (m_indexList.GetItemStates(ii) & RC_ITEM_CHECKED)
			{
				CString indexS;
				indexS.Format("%d", ii);
				item->sAry.Add(indexS);
			}
		}
	}
	else if (item->kind == TKIND_NEWS)
	{
		for (int ii = 0; ii < m_newsList.GetItemCount(); ii++)
		{
			if (m_newsList.GetItemStates(ii) & RC_ITEM_CHECKED)
			{
				CString indexS;
				indexS.Format("%d", ii+NEWSGAP);
				item->sAry.Add(indexS);
			}
		}
	}

	UpdateInfoText();

	return 0;
}

CString CSetTickInfo::GetIndexString(int idx)
{
	CString	string;

	switch (idx)
	{
	case 0:		string = _T("KRX100");		break;
	case 1:		string = _T("KOSPI");		break;
	case 2:		string = _T("KOSDAQ");		break;
	case 3:		string = _T("KOSPI200");	break;
	case 4:		string = _T("KOSPI100");	break;

	case 5:		string = _T("배당지수");	break;
	case 6:		string = _T("KOSTAR");		break;
	case 7:		string = _T("BASIS");		break;
	case 8:		string = _T("선물");		break;
	case 9:		string = _T("스타선물");	break;

	case 10:	string = _T("S&&P500");		break;
	case 11:	string = _T("NASDAQ100");	break;
	case 12:	string = _T("환율");		break;
	case 13:	string = _T("NIKKEI225");	break;
	case 14:	string = _T("항셍H지수");	break;
	}

	return string;
}

BOOL CSetTickInfo::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	UINT command = LOWORD(wParam);
	switch (command)
	{
	case IDC_SETINDEX:
		SetStatus(TKIND_INDEX);
		UpdateItemInfo(TKIND_INDEX);
		UpdateInfoText();
		break;
	case IDC_SETNEWS:
		SetStatus(TKIND_NEWS);
		UpdateItemInfo(TKIND_NEWS);
		UpdateInfoText();
		break;
	case IDC_SETUPDOWN:
		SetStatus(TKIND_UPDOWN);
		UpdateItemInfo(TKIND_UPDOWN);
		UpdateInfoText();
		break;
	case IDC_SETINTERCODE:
		SetStatus(TKIND_INTER);
		UpdateItemInfo(TKIND_INTER);
		UpdateInfoText();
		break;
	case IDC_SETCONCLUSION:
		SetStatus(TKIND_CONCLUSION);
		UpdateItemInfo(TKIND_CONCLUSION);
		UpdateInfoText();
		break;
	case IDOK:
		saveInfo();
		break;
	case IDC_UPDOWNLIST:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			UpdateItemInfo(TKIND_UPDOWN);
			UpdateInfoText();
		}
		break;
	case IDC_UPS_TEXT:
	case IDC_UPS_GRAPH:
		UpdateItemInfo(TKIND_UPDOWN);
		UpdateInfoText();
		break;
	case IDC_INTERGROUP:
		if (HIWORD(wParam) == CBN_SELCHANGE)
		{
			UpdateItemInfo(TKIND_INTER);
			UpdateInfoText();
		}
		break;
	case IDC_DBARROW:
	case IDC_JISUVOL:
		UpdateItemInfo(TKIND_INDEX);
		break;
	case IDC_9F:
		m_fontsize = 9;		setFont();
		break;
	case IDC_10F:
		m_fontsize = 10;	setFont();
		break;
	case IDC_11F:
		m_fontsize = 11;	setFont();
		break;
	case IDC_NORMAL:
		m_bold = 0;		setFont();
		break;
	case IDC_BOLD:
		m_bold = 1;		setFont();
	}	
	return CDialog::OnCommand(wParam, lParam);
}

void CSetTickInfo::UpdateItemInfo(int kind)
{
	int	bIdx = m_active/10;
	int	iIdx = m_active%10;
	int	idx = SPLIT_CNT*bIdx + iIdx;

	CObArray	ary;

	_tickitem* item = (_tickitem*) m_arItemInfo.GetAt(idx);
	item->kind = kind;
	item->selectN = 0;

	switch (kind)
	{
	case TKIND_INDEX:
		{
			CString	key, value;
			item->sAry.RemoveAll();
			for (int ii = 0; ii < m_indexList.GetItemCount(); ii++)
			{
				if (m_indexList.GetItemStates(ii) & RC_ITEM_CHECKED)
				{
					CString indexS;
					indexS.Format("%d", ii);
					item->sAry.Add(indexS);
				}
			}
/*			m_indexList.SetAllItemStates(RC_ITEM_CHECKED, RC_ITEM_UNCHECKED);
			for (int ii = 0; ii < item->sAry.GetSize(); ii++)
			{
				key = item->sAry.GetAt(ii);
				int indexN = atoi(key);
				if (indexN >= m_indexList.GetItemCount())
					continue;
				m_indexList.SetItemStates(indexN, RC_ITEM_CHECKED);
			}
*/
			CButton* button = (CButton*) GetWnd(IDC_DBARROW);
			if (button->GetCheck() == BST_CHECKED)
				item->aux = 0;
			else	item->aux = 1;
		}
		break;
	case TKIND_NEWS:
		{
			item->sAry.RemoveAll();
			for (int ii = 0; ii < m_newsList.GetItemCount(); ii++)
			{
				if (m_newsList.GetItemStates(ii) & RC_ITEM_CHECKED)
				{
					CString indexS;
					indexS.Format("%d", ii+NEWSGAP);
					item->sAry.Add(indexS);
				}
			}
		}
		break;
	case TKIND_UPDOWN:
		{
			CButton* button = (CButton *) GetWnd(IDC_UPS_GRAPH);
			if (button->GetCheck() != BST_CHECKED)
				item->aux = 1;
			else	item->aux = 2;

			item->selectN = m_updnCom.GetCurSel();
		}
		break;
	case TKIND_INTER:
		item->selectN = m_interCom.GetCurSel();
		break;
	case TKIND_CONCLUSION:
		break;
	}
}

void CSetTickInfo::OnChangePanecount1() 
{
	UpdateData(TRUE);
	SetSplit(m_bar1cnt);	
}

void CSetTickInfo::OnChangePanecount2() 
{
	UpdateData(TRUE);
	SetSplit(m_bar2cnt, 1);
}

void CSetTickInfo::loadInter()
{
	char	wb[256];
	CString file, keys, ifile;
	CFileFind find;
	
	file.Format("%s\\user\\%s\\portfolio.ini", m_home, m_userName);
	DWORD dw = GetPrivateProfileString("GROUPORDER", "00", "", wb, sizeof(wb), file);

	m_ary.RemoveAll();
	m_interCom.ResetContent();
	for (int ii = 0; ii < 100; ii++)
	{
		keys.Format("%02d", ii);
		dw = GetPrivateProfileString("GROUPNAME", keys, "", wb, sizeof(wb), file);
		if (dw <= 0)	continue;
		ifile.Format("%s\\user\\%s\\PortFolio.i%02d", m_home, m_userName, ii);
		if (find.FindFile(ifile))
		{
			m_ary.Add(keys);
			m_interCom.AddString(wb);
		}
	}
}

void CSetTickInfo::writeInterCode(CString section, int groupN)
{
	if (groupN < 0)	return;
	CString		file, ifile;
	CFile		fileH;

	file.Format("%s\\user\\%s\\%s", m_home, m_userName, TICKSETUP);
	ifile.Format("%s\\user\\%s\\portfolio.i%s", m_home, m_userName, m_ary.GetAt(groupN));

  	if (!fileH.Open(ifile, CFile::modeRead|CFile::typeBinary))
	{
		CString	msg;
		msg.Format("관심종목 파일을 찾을수 없습니다.\n[%s]", ifile);
		::MessageBox(m_hWnd, msg, "파일오류", MB_ICONASTERISK);
		return;
	}

	int	idx = 0;
//	struct _inters	inter;
//	struct _alarms	alarm;
// 	while (fileH.Read(&inter, sizeof(inter)))
// 	{
// 		char	jmCode [16];
// 		sprintf(jmCode, "%s", CString(inter.code, sizeof(inter.code)));
// 	
// 		for (int ii = 0; ii < inter.a_num; ii++)
// 			fileH.Read(&alarm, sizeof(alarm));
// 
// 		CString keys, jCode = jmCode;	jCode.TrimRight();
// 		if (jCode.IsEmpty())
// 			continue;
// 
// 		if (jCode.GetLength() == 6)
// 			jCode = "A" + jCode;
// 		else if (jCode.GetLength() != 8)	// stock, future, call, put
// 			continue;
// 		keys.Format("%02d", idx++);
// 		WritePrivateProfileString(section, keys, jCode, file);
//       	}
	fileH.Close();
}

BOOL CSetTickInfo::GetViewTick(int index)
{
	switch (index)
	{
	case 0:	return m_view1;
	case 1:	return m_view2;
	case 2: return m_screenbar;
	}
	return TRUE;
}

int CSetTickInfo::matchItemN(int key)
{
#ifdef NDEBUG
	return key;
#endif
	int	nItem = key;
	switch (key)
	{
	case 0:	case 1:	case 2:	case 3:	case 4:	case 5:	case 6:	case 9:	case 10:
	case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19:
		nItem = key;		break;
	// 해외지수
	case 20:	nItem = key;	break;
	case 21:	nItem = 0;	break;
	case 22:	nItem = 0;	break;
	case 23:	nItem = 0;	break;
	case 24:	nItem = 0;	break;
	case 25:	nItem = 0;	break;
	case 26:	nItem = 0;	break;
	case 27:	nItem = 0;	break;
	case 28:	nItem = 0;	break;
	case 29:	nItem = 0;	break;
	case 30:	nItem = 0;	break;
	case 31:	nItem = 0;	break;
	case 32:	nItem = 0;	break;
	case 33:	nItem = 0;	break;
	case 34:	nItem = 0;	break;
	case 35:	nItem = 0;	break;
	}
	return  nItem;
}

void CSetTickInfo::ApplySetup()
{
	saveInfo();
}

void CSetTickInfo::DefaultSetup()
{
	CString	file, section, keys, value;
	int	nRes;
	char	buf[256];
	int ii = 0;

	file.Format("%s\\%s\\%s", m_home, MTBLDIR, TICKSETUP);
	m_bar1cnt	= GetPrivateProfileInt("GENERAL", "BAR00", SPLIT_CNT, file);
	m_bar2cnt	= GetPrivateProfileInt("GENERAL", "BAR01", SPLIT_CNT, file);
	m_view1		= GetPrivateProfileInt("GENERAL", "view1", 1, file);
	m_view2		= GetPrivateProfileInt("GENERAL", "view2", 1, file);
	m_screenbar	= GetPrivateProfileInt("GENERAL", "screenbar", 1, file);
	UpdateData(FALSE);

	m_fontsize = GetPrivateProfileInt("GENERAL", "fontsize", 9, file);
	((CButton *) GetDlgItem(IDC_9F))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_10F))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_11F))->SetCheck(0);
	switch (m_fontsize)
	{
	default:
	case 9:		nRes = IDC_9F;		break;
	case 10:	nRes = IDC_10F;		break;
	case 11:	nRes = IDC_11F;		break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(1);

	((CButton *) GetDlgItem(IDC_BOLD))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_NORMAL))->SetCheck(0);
	m_bold = GetPrivateProfileInt("GENERAL", "bold", 0, file);
	if (m_bold)	((CButton *) GetDlgItem(IDC_BOLD))->SetCheck(1);
	else		((CButton *) GetDlgItem(IDC_NORMAL))->SetCheck(1);

	GetPrivateProfileString("GENERAL", "fontname", "굴림체", buf, sizeof(buf), file);
	m_fontname = buf;	m_fontname.TrimLeft();		m_fontname.TrimRight();
	((CComboBox *)GetDlgItem(IDC_FONT))->SelectString(-1, m_fontname);

	setFont();

	m_period = GetPrivateProfileInt("GENERAL", "period", 5, file);
	value.Format("%d", m_period);
	((CComboBox *)GetDlgItem(IDC_PERIOD))->SelectString(-1, value);

	m_color = GetPrivateProfileInt("GENERAL", "color", GetColor(COLOR_TICK_INSIDE), file);
	m_cbColor.SetColor(m_color);
	
	loadInter();

	_tickitem* item;
	for (ii = 0; ii < m_arItemInfo.GetSize(); ii++)
	{
		item = (_tickitem*) m_arItemInfo.GetAt(ii);
		delete item;
	}
	m_arItemInfo.RemoveAll();
	for (ii = 0; ii < TICKBAR_CNT; ii++)
	{
		for (int jj = 0; jj < SPLIT_CNT; jj++)
		{
			_tickitem* item = new _tickitem;

			section.Format("BAR_INFO_%d%d", ii, jj);
			item->kind = GetPrivateProfileInt(section, "INDEXK", 0, file);
			switch (item->kind)
			{
			case TKIND_INDEX:
				{
					for (int kk = 0, hh; ; kk++)
					{
						keys.Format("%03d", kk);
						hh = GetPrivateProfileInt(section, keys, -1, file);
						if (hh == -1)	break;
						value.Format("%d", hh);
						item->sAry.Add(value);
					}
					keys = _T("AUX");
					item->aux = GetPrivateProfileInt(section, keys, 0, file);

					if (!item->sAry.GetSize())	item->sAry.Add("1");
				}
				break;
			case TKIND_UPDOWN:
				{
					keys = _T("SELECT");
					item->selectN = GetPrivateProfileInt(section, keys, 0, file);
					keys = _T("AUX");
					item->aux = GetPrivateProfileInt(section, keys, 1, file);
					if (item->aux < 1 || item->aux > 2)	item->aux = 1;
				}
				break;
			case TKIND_NEWS:
				{
					item->aux = 0;
					item->sAry.RemoveAll();
					item->selectN = 0;
					for (int kk = 0, hh; ; kk++)
					{
						keys.Format("%02d", kk);
						hh = GetPrivateProfileInt(section, keys, -1, file);
						if (hh == -1)	break;
						value.Format("%d", hh);
						item->sAry.Add(value);
					}
					keys = _T("AUX");
					item->aux = GetPrivateProfileInt(section, keys, 0, file);

					if (!item->sAry.GetSize())	item->sAry.Add("1");
				}
				break;
			case TKIND_INTER:
				if (m_interCom.GetCount())
				{
					item->selectN = GetPrivateProfileInt(section, "SELECT", 0, file);
					m_interCom.SetCurSel(0);
				}
				else	item->selectN = -1;
				break;
			case TKIND_CONCLUSION:
				break;
			default:
				AfxMessageBox("Tick infomation load Error!");
				continue;
			}
			m_arItemInfo.Add((CObject *) item);
		}
	}

	SetSplit(m_bar1cnt);
	SetSplit(m_bar2cnt, 1);

	SelectTicker();
	UpdateInfoText(true);
}

void CSetTickInfo::setFont()
{
	m_font.DeleteObject();

	LOGFONT	lf;
	m_font.CreatePointFont(m_fontsize*10, m_fontname);
	m_font.GetLogFont(&lf);
	m_font.DeleteObject();

	m_font.CreateFont(	lf.lfHeight,			// nHeight
				lf.lfWidth,			// nWidth
				lf.lfEscapement,		// nEscapement
				lf.lfOrientation,		// nOrientation
				m_bold ? FW_BOLD : FW_NORMAL,	// nWeight
				lf.lfItalic,			// bItalic
				lf.lfUnderline,			// bUnderline
				lf.lfStrikeOut,			// cStrikeOut
				lf.lfCharSet,			// nCharSet
				lf.lfOutPrecision,		// nOutPrecision
				lf.lfClipPrecision,		// nClipPrecision
				lf.lfQuality,			// nQuality
				lf.lfPitchAndFamily,		// nPitchAndFamily
				(char *) m_fontname.operator LPCTSTR());        // lpszFacename//*/

	GetDlgItem(IDC_PREVIEW)->SetFont(&m_font);
}

void CSetTickInfo::OnSelchangeFont() 
{
	((CComboBox *) GetDlgItem(IDC_FONT))->GetWindowText(m_fontname);

	setFont();
}

COLORREF CSetTickInfo::GetColor(int index)
{
	int		pos;
	char		buffer[128];
	CString		file, tmpS, dat, section, key;
	CStringArray	ary;

	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	GetPrivateProfileString(GENERALSN, "Palette", "Palette", buffer, sizeof(buffer), file);

	section = buffer;
	key.Format("RGB%03d", index);
	DWORD dw = GetPrivateProfileString(section, key, "", buffer, sizeof(buffer), file);
	if (dw <= 0)
	{
		if (!section.Compare("Palette"))
			return RGB(215, 215, 215);
		section = _T("Palette");
		dw = GetPrivateProfileString(section, key, "", buffer, sizeof(buffer), file);
		if (dw <= 0)	return RGB(215, 215, 215);
	}

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

	if (ary.GetSize() < 3)
		return RGB(215, 215, 215);
	return RGB(atoi(ary.GetAt(0)), atoi(ary.GetAt(1)), atoi(ary.GetAt(2)));
}

BOOL CSetTickInfo::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NM_LISTVIEW* listView;

	switch(wParam)
	{
	case IDC_NEWSLIST:
		listView = (NM_LISTVIEW*) lParam;
		if (listView->hdr.code == NM_CLICK && listView->iItem != -1 && listView->ptAction.x > 15)
		{
			if (m_newsList.GetItemStates(listView->iItem) & RC_ITEM_CHECKED)
				m_newsList.SetItemStates(listView->iItem, RC_ITEM_UNCHECKED);
			else if (m_newsList.GetItemStates(listView->iItem) & RC_ITEM_UNCHECKED)
				m_newsList.SetItemStates(listView->iItem, RC_ITEM_CHECKED);
		}
		break;
	case IDC_INDEXLIST:
		listView = (NM_LISTVIEW*) lParam;
		if (listView->hdr.code == NM_CLICK && listView->iItem != -1 && listView->ptAction.x > 15)
		{
			if (m_indexList.GetItemStates(listView->iItem) & RC_ITEM_CHECKED)
				m_indexList.SetItemStates(listView->iItem, RC_ITEM_UNCHECKED);
			else if (m_indexList.GetItemStates(listView->iItem) & RC_ITEM_UNCHECKED)
				m_indexList.SetItemStates(listView->iItem, RC_ITEM_CHECKED);
		}
		break;
	default:
		break;
	}
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}
