// SetTickInfo.cpp : implementation file
//

#include "stdafx.h"
#include "SetTickInfo.h"
#include "cpdefine.h"
#include "../sm/pane.h"

#include <afxtempl.h>
#include "../../h/interSt.h"
#include "../../h/axisvar.h"
#include "interMSG.h"

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

#define COLOR_INLINE	RGB(103,170,197)
#define COLOR_OUTLINE	RGB(255,255,255)


/////////////////////////////////////////////////////////////////////////////
// CSetTickInfo dialog


CSetTickInfo::CSetTickInfo(int id, CWnd* pParent /*=NULL*/)
	: CSetupDialog(CSetTickInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTickInfo)
	m_bar1cnt = 0;
	m_bar2cnt = 0;
	m_view1 = FALSE;
	m_view2 = FALSE;
	m_screenbar = FALSE;
	//}}AFX_DATA_INIT

	m_caption = "사용자티커 설정";
	m_active	= id;

	m_brush.CreateSolidBrush(ACTIVE_COLOR);
	m_pMain = pParent;
}

CSetTickInfo::~CSetTickInfo()
{
	//_tickitem* item{};
	//for (int ii = 0; ii < m_arItemInfo.GetSize(); ii++)
	//{
	//	item = (_tickitem*) m_arItemInfo.GetAt(ii);
	//	delete item;
	//}
	m_arItemInfo.clear();

	m_brush.DeleteObject();
}

void CSetTickInfo::DoDataExchange(CDataExchange* pDX)
{
	CSetupDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTickInfo)
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

BEGIN_MESSAGE_MAP(CSetTickInfo, CSetupDialog)
	//{{AFX_MSG_MAP(CSetTickInfo)
	ON_EN_CHANGE(IDC_PANECOUNT1, OnChangePanecount1)
	ON_EN_CHANGE(IDC_PANECOUNT2, OnChangePanecount2)
	ON_CBN_SELCHANGE(IDC_FONT, OnSelchangeFont)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ON_CHKBOX, OnChangeLIST)
	ON_MESSAGE(WM_MSG_MAINTOMAP, OnMsgFromMain)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTickInfo message handlers

BOOL CSetTickInfo::OnInitDialog() 
{
	CSetupDialog::OnInitDialog();
	m_drawfont.CreatePointFont(90, "굴림체");
	m_bmpCombo = Axis::GetBitmap("CONF_COMBO");
	m_bmpCombo_N = Axis::GetBitmap("CONF_COMBO_N");
	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetTickInfo::Init()
{
	m_indexList.DeleteAllItems();
	m_newsList.DeleteAllItems();

	m_indexList.InsertColumn(0, "index Item");
	m_indexList.SetCheckboxeStyle(RC_CHKBOX_NORMAL);	// Enable checkboxes

	m_newsList.InsertColumn(0, "news Item");
	m_newsList.SetCheckboxeStyle(RC_CHKBOX_NORMAL);		// Enable checkboxes

	CString sIndex;
	int i = 0;
	while (TRUE)
	{
		sIndex = GetIndexString(i);
		if (sIndex.IsEmpty())
			break;
		if (sIndex != "환율" && sIndex != "-")
			m_indexList.InsertItem(i++, sIndex);
		else 
			i++;
	}

	for (i = 0; i < Axis::GetNewsCount(); i++)
	{
		m_newsList.InsertItem(i, Axis::GetNewsName(i));
	}

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
	int	nRes{};
	char	buf[256]{};

	file.Format("%s\\user\\%s\\%s", Axis::home, Axis::user, TICKSETUP);
	m_bar1cnt	= GetPrivateProfileInt(szGeneral, "BAR00", SPLIT_CNT, file);
	m_bar2cnt	= GetPrivateProfileInt(szGeneral, "BAR01", SPLIT_CNT, file);
	m_view1		= GetPrivateProfileInt(szGeneral, "view1", 1, file);
	m_view2		= GetPrivateProfileInt(szGeneral, "view2", 1, file);
	m_screenbar	= GetPrivateProfileInt(szGeneral, "screenbar", 1, file);
	m_view1 = m_view1>0 ? true : false;
	m_view2 = m_view2>0 ? true : false;
	//UpdateData(FALSE);
	
	
	m_fontsize = GetPrivateProfileInt(szGeneral, "fontsize", 9, file);
	switch (m_fontsize)
	{
	default:
	case 9:		nRes = IDC_9F;		break;
	case 10:	nRes = IDC_10F;		break;
	case 11:	nRes = IDC_11F;		break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(1);

	m_bold = GetPrivateProfileInt(szGeneral, "bold", 0, file);
	if (m_bold)	((CButton *) GetDlgItem(IDC_BOLD))->SetCheck(1);
	else		((CButton *) GetDlgItem(IDC_NORMAL))->SetCheck(1);

	GetPrivateProfileString(szGeneral, "fontname", "굴림체", buf, sizeof(buf), file);
	m_fontname = buf;	m_fontname.TrimLeft();		m_fontname.TrimRight();
	((CComboBox *)GetDlgItem(IDC_FONT))->SelectString(-1, m_fontname);

	setFont();

	m_period = GetPrivateProfileInt(szGeneral, "period", 5, file);
	value.Format("%d", m_period);
	((CComboBox *)GetDlgItem(IDC_PERIOD))->SelectString(-1, value);

	loadInter();
	for (int ii = 0; ii < TICKBAR_CNT; ii++)
	{
		for (int jj = 0; jj < SPLIT_CNT; jj++)
		{
			//_tickitem* item = new _tickitem;
			auto item = m_arItemInfo.emplace_back (std::make_shared< _tickitem>());

			section.Format("BAR_INFO_%d%d", ii, jj);
			item->kind = GetPrivateProfileInt(section, "INDEXK", 0, file);
			switch (item->kind)
			{
			case TKIND_INDEX:
				{
					for (int kk = 0, hh = 0; ; kk++)
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
					for (int kk = 0, hh = 0; ; kk++)
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
			case TKIND_INVEST:
				{
					for (int kk = 0, hh = 0; ; kk++)
					{
						keys.Format("%03d", kk);
						hh = GetPrivateProfileInt(section, keys, -1, file);
						if (hh == -1)	break;
						value.Format("%d", hh);
						item->sAry.Add(value);
					}
					keys = _T("AUX");
					item->aux = GetPrivateProfileInt(section, keys, 0, file);

					value.Format("%d", INVEST_KOSPI);
					if (!item->sAry.GetSize())	item->sAry.Add(value);
				}
				break;
			default:
				AfxMessageBox("Tick infomation load Error!");
				continue;
			}
		//	m_arItemInfo.Add((CObject *) item);
		}
	}
}

void CSetTickInfo::saveInfo()
{
	CString		section, keys, value, file, name;
	int		index = 0;

	file.Format("%s\\user\\%s\\%s", Axis::home, Axis::user, TICKSETUP);

	value.Format("%d", m_bar1cnt);
	WritePrivateProfileString(szGeneral, "BAR00", value, file);
	value.Format("%d", m_bar2cnt);
	WritePrivateProfileString(szGeneral, "BAR01", value, file);

	UpdateData();
	value.Format("%d", m_view1);
	WritePrivateProfileString(szGeneral, "view1", value, file);
	value.Format("%d", m_view2);
	WritePrivateProfileString(szGeneral, "view2", value, file);
	value.Format("%d", m_screenbar);
	WritePrivateProfileString(szGeneral, "screenbar", value, file);

	value.Format("%d", m_fontsize);
	WritePrivateProfileString(szGeneral, "fontsize", value, file);

	WritePrivateProfileString(szGeneral, "fontname", m_fontname, file);

	value.Format("%d", m_bold);
	WritePrivateProfileString(szGeneral, "bold", value, file);

	((CComboBox *) GetDlgItem(IDC_PERIOD))->GetWindowText(value);
	WritePrivateProfileString(szGeneral, "period", value, file);

	for (int ii = 0; ii < TICKBAR_CNT; ii++)
	{
		for (int jj = 0; jj < SPLIT_CNT; jj++)
		{
			//_tickitem* item = (_tickitem*) m_arItemInfo.GetAt(index++);
			//auto item = m_arItemInfo.at(index++);
			auto item = m_arItemInfo[index++];
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
			//		writeInterCode(section, item->selectN);  //관심종목 서버조회로 변경
				}
				break;
			case TKIND_CONCLUSION:
				break;
			case TKIND_INVEST:
				{
					if (item->sAry.GetSize() <= 0)	
					{
						value.Format("%d", INVEST_KOSPI);
						item->sAry.Add(value);
					}

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
			default:
				AfxMessageBox("Tick infomation save Error!");
				continue;
			}
		}
	}
}

void CSetTickInfo::SetSplit(int cnt, int idx)
{
	int		width{}, ii{};  //vc2019
	CRect		sRc, bRc;
	CWnd* wnd{};
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
	CWnd* wnd{};
	int		ii{};
	const int bIdx = m_active / 10;
	const int iIdx = m_active%10;

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
				else	
					continue;
			}
			else
			{
				if (wnd->GetExStyle() & ACTIVE)
					wnd->ModifyStyleEx(ACTIVE, INACTIVE, SWP_DRAWFRAME);
				else	
					continue;
			}
			wnd->Invalidate();
		}
	}
	ary.RemoveAll();

	//const _tickitem* item = (_tickitem*) m_arItemInfo.GetAt(SPLIT_CNT*bIdx + iIdx);
	auto item = m_arItemInfo.at(SPLIT_CNT * bIdx + iIdx);
	GetKindAry(ary);
	for (ii = 0; ii < ary.GetSize(); ii++)
	{
		wnd = (CWnd *) ary.GetAt(ii);
		if (ii == item->kind)
			((CButton *) wnd)->SetCheck(1);
		else	
			((CButton *) wnd)->SetCheck(0);
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
				//item = (_tickitem*) m_arItemInfo.GetAt(index++);
				 auto item = m_arItemInfo.at(index++);
				wnd = (CWnd*) ary.GetAt(jj);
				//wnd->SetWindowText(GetPaneString(item));
				wnd->SetWindowText(GetPaneString(item.get()));
			}
		}
	}
	else
	{
		const int	bIdx = m_active/10;
		const int	iIdx = m_active%10;
		const int	idx = SPLIT_CNT*bIdx + iIdx;

		//item = (_tickitem*) m_arItemInfo.GetAt(idx);
		auto item = m_arItemInfo.at(idx);
		GetSplitItemArray(bIdx, ary);
		wnd = (CWnd*) ary.GetAt(iIdx);
		wnd->SetWindowText(GetPaneString(item.get()));
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
	ary.Add(GetWnd(IDC_SETINVEST));
}

void CSetTickInfo::SetItemInfo()
{
	const int	bIdx = m_active/10;
	const int	iIdx = m_active%10;
	const int	idx = SPLIT_CNT*bIdx + iIdx;

	CString		key, value;
	int		indexN{};

//	const _tickitem* item = (_tickitem*) m_arItemInfo.GetAt(idx);
	auto item =m_arItemInfo.at(idx);
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
				{
					//CString tmp; tmp.Format("%d | %d", indexN, m_indexList.GetItemCount());
					//AfxMessageBox(tmp);
					continue;
				}	

				m_indexList.SetItemStates(indexN, RC_ITEM_CHECKED);
			}
			
			if (item->aux == 1)
			{
				((CButton*) GetWnd(IDC_DBARROW))->SetCheck(0);
				((CButton*) GetWnd(IDC_JISUVOL))->SetCheck(1);
				((CButton*) GetWnd(IDC_TRADEMONEY))->SetCheck(0);
			}
			else if (item->aux == 0) 
			{
				((CButton*) GetWnd(IDC_DBARROW))->SetCheck(1);
				((CButton*) GetWnd(IDC_JISUVOL))->SetCheck(0);
				((CButton*) GetWnd(IDC_TRADEMONEY))->SetCheck(0);
			}
			else if (item->aux == 2)
			{
				((CButton*) GetWnd(IDC_DBARROW))->SetCheck(0);
				((CButton*) GetWnd(IDC_JISUVOL))->SetCheck(0);
				((CButton*) GetWnd(IDC_TRADEMONEY))->SetCheck(1);
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
				UINT	nRes{};
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
	case TKIND_INVEST:
		{
			int ii{};  //vc2019
			const int ids[] = { IDC_INVEST_KOSPI, IDC_INVEST_KOSDAQ, IDC_INVEST_FUTURE, IDC_INVEST_CALL, IDC_INVEST_PUT, -1 };  //vc2019 UINT->int
			for (int ii = 0; ids[ii] != -1; ii++)
			{
				((CButton *)GetDlgItem(ids[ii]))->SetCheck(0);
			}

			for (ii = 0; ii < item->sAry.GetSize(); ii++)
			{
				key = item->sAry.GetAt(ii);
				indexN = atoi(key) - INVEST_KOSPI + IDC_INVEST_KOSPI;
				((CButton *) GetWnd(indexN))->SetCheck(1);
			}
			
			if (item->aux)
			{
				((CButton*) GetWnd(IDC_INVEST_PRICE))->SetCheck(0);
				((CButton*) GetWnd(IDC_INVEST_VOL))->SetCheck(1);
			}
			else
			{
				((CButton*) GetWnd(IDC_INVEST_PRICE))->SetCheck(1);
				((CButton*) GetWnd(IDC_INVEST_VOL))->SetCheck(0);
			}
		}
		break;
	default:
		::MessageBox(m_hWnd, "Information file error!", "Error", MB_ICONSTOP);
		break;
	}
}

CString CSetTickInfo::GetPaneString(_tickitem* item)
{
	CString		keys, str, value;
	const int		cnt = 0;

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
	case TKIND_INVEST:
		str = _T("매매현황");
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
	GetWnd(IDC_TRADEMONEY)->EnableWindow(FALSE);
	GetWnd(IDC_INVEST_KOSPI)->EnableWindow(FALSE);
	GetWnd(IDC_INVEST_KOSDAQ)->EnableWindow(FALSE);
	GetWnd(IDC_INVEST_FUTURE)->EnableWindow(FALSE);
	GetWnd(IDC_INVEST_CALL)->EnableWindow(FALSE);
	GetWnd(IDC_INVEST_PUT)->EnableWindow(FALSE);
	GetWnd(IDC_INVEST_PRICE)->EnableWindow(FALSE);
	GetWnd(IDC_INVEST_VOL)->EnableWindow(FALSE);

	m_interCom.EnableWindow(FALSE);

	switch (kind)
	{
	default:
	case TKIND_INDEX:
		bEnableIDX = true;
		m_indexList.EnableWindow(TRUE);
		GetWnd(IDC_DBARROW)->EnableWindow(TRUE);
		GetWnd(IDC_JISUVOL)->EnableWindow(TRUE);
		GetWnd(IDC_TRADEMONEY)->EnableWindow(TRUE);
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
	case TKIND_INVEST:
		GetWnd(IDC_INVEST_KOSPI)->EnableWindow(TRUE);
		GetWnd(IDC_INVEST_KOSDAQ)->EnableWindow(TRUE);
		GetWnd(IDC_INVEST_FUTURE)->EnableWindow(TRUE);
		GetWnd(IDC_INVEST_CALL)->EnableWindow(TRUE);
		GetWnd(IDC_INVEST_PUT)->EnableWindow(TRUE);
		GetWnd(IDC_INVEST_PRICE)->EnableWindow(TRUE);
		GetWnd(IDC_INVEST_VOL)->EnableWindow(TRUE);
		break;
	}

	COLORREF	bkColor{}, textColor{};
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
	
	return CSetupDialog::PreTranslateMessage(pMsg);
}

void CSetTickInfo::ProcessLbutton(CPoint pt)
{
	CWnd* wnd{};
	bool		match = false;
	CRect		iRect;
	CObArray	ary;
	int ii{};  //vc2019
	
	GetSplitItemArray(0, ary);
	for (int ii = 0; ii < (int)m_bar1cnt && !match; ii++)
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

LRESULT CSetTickInfo::OnChangeLIST(WPARAM wParam, LPARAM lParam)  //vc2019
{
	const int	bIdx = m_active/10;
	const int	iIdx = m_active%10;
	const int	idx = SPLIT_CNT*bIdx + iIdx;
	
	//_tickitem*	item = (_tickitem*) m_arItemInfo.GetAt(idx);
	auto item = m_arItemInfo.at(idx);
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
// 	const char* index[] = {
// 		"KRX100",		"KOSPI",		"KOSPI200",		"KOSTAR",		"KOSDAQ",
// 		"KOSDAQ 벤처",	"KP 최근월",	"KP 차근월",		
// 		
// 		"KOSPI 프로그램", "KOSDAQ 프로그램", "KOSPI 투자자", "KOSDAQ 투자자",
// 		"선물투자자",	"콜-투자자",	"풋-투자자",
// 
// 		"NASDAQ",		"NASDAQ100선물", "Mini N100",	"S&P500",		"S&P500 선물",
// 		"Mini SP500",	"DOWJONES",
// 
// 		"NIKKEI(일본)", "WEIGHT(대만)",	"ASSHARE(중국)", "HANGSE(홍콩)", "HSCE(항셍H지수)", "KRX SRI", "KP최근월 BASIS",
// 
// 	    "상해B", "심천B", "BOMBAY(인도)", "대만가권지수","호치민(베트남)", "반도체(필라델피아)",
// 		"INMEX(멕시코)", "BOVESPA(브라질)", "FTSE100(영국)", "Xetra DAX(독일)", "CAC40(프랑스)", "RTSI(러시아)", "상해종합", ""
// 	};
	//티커 지수 변경시 수정해야하는 부분
	const char* const index[] = {
		"KRX100",		"KOSPI",		"KOSPI200",		"KQ150",/*"KOSTAR",*/		"KOSDAQ",
			"KOSDAQ 벤처",	"KP 최근월",	"KP 차근월",		
			
			"KOSPI 프로그램", "KOSDAQ 프로그램", "KOSPI 투자자(금액)", "KOSDAQ 투자자(금액)",
			"선물투자자(수량)",	"콜-투자자(금액)",	"풋-투자자(금액)",
			
			"KRX SRI", "KP최근월 BASIS", ""
	};
	

	if (idx >= 0 && idx < ArraySize(index))
		return CString(index[idx]);
	else
		return CString("");
}

//기존 해외 지수
// const int indexKey[] = {
// 	1, 2, 3, 4, 5, 6, 9, 10, 13, 14, 15, 16, 17, 18, 19, 20,
// 		21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33
// };
//새로운 해외 지수
// const int indexKey[] = {
// 	1, 2, 3, 4, 5, 6, 9, 10, 13, 14, 15, 16, 17, 18, 19, 20,
// 		21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50
// };
//티커 지수 수정시 고쳐야하는 부분
const int indexKey[] = {
	1, 2, 3, 4,/*4,*/ 5, 6, 9, 10, 13, 14, 15, 16, 17, 18, 19, 32, 33
};

BOOL CSetTickInfo::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	const UINT command = LOWORD(wParam);
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
	case IDC_SETINVEST:
		SetStatus(TKIND_INVEST);
		UpdateItemInfo(TKIND_INVEST);
		UpdateInfoText();
		break;
	case IDC_INVEST_KOSPI:		case IDC_INVEST_KOSDAQ:
	case IDC_INVEST_FUTURE:		case IDC_INVEST_CALL:		case IDC_INVEST_PUT:
	case IDC_INVEST_PRICE:		case IDC_INVEST_VOL:
		UpdateItemInfo(TKIND_INVEST);
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
	case IDC_TRADEMONEY:
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
	return CSetupDialog::OnCommand(wParam, lParam);
}

void CSetTickInfo::UpdateItemInfo(int kind)
{
	const int	bIdx = m_active/10;
	const int	iIdx = m_active%10;
	const int	idx = SPLIT_CNT*bIdx + iIdx;

	CObArray	ary;

	//_tickitem* item = (_tickitem*) m_arItemInfo.GetAt(idx);
	auto item = m_arItemInfo.at(idx);
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

			const CButton* button = (CButton*) GetWnd(IDC_DBARROW);
			const CButton* button2 = (CButton*) GetWnd(IDC_JISUVOL);
			const CButton* button3 = (CButton*) GetWnd(IDC_TRADEMONEY);

			if (button->GetCheck() == BST_CHECKED)
				item->aux = 0;
			else if (button2->GetCheck() == BST_CHECKED)	item->aux = 1;
			else if (button3->GetCheck() == BST_CHECKED)    item->aux = 2;
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
		    const CButton* button = (CButton *) GetWnd(IDC_UPS_GRAPH);
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
	case TKIND_INVEST:
		{
			CString	key, value;
			item->sAry.RemoveAll();

			const int ids[] = { IDC_INVEST_KOSPI, IDC_INVEST_KOSDAQ, IDC_INVEST_FUTURE, IDC_INVEST_CALL, IDC_INVEST_PUT, -1 };  //vc2019 UINT->int

			for (int ii = 0; ids[ii] != -1; ii++)
			{
				if (((CButton *)GetDlgItem(ids[ii]))->GetCheck())
				{
					CString indexS;
					indexS.Format("%d", INVEST_KOSPI + ids[ii] - IDC_INVEST_KOSPI);
					item->sAry.Add(indexS);
				}
			}

			const CButton* button = (CButton*) GetWnd(IDC_INVEST_PRICE);
			if (button->GetCheck() == BST_CHECKED)
				item->aux = 0;
			else	item->aux = 1;
		}
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
	/*char	wb[256];
	CString file, keys, ifile;
	CFileFind find;
	
	file.Format("%s\\user\\%s\\portfolio.ini", Axis::home, Axis::user);
	DWORD dw = GetPrivateProfileString("GROUPORDER", "00", "", wb, sizeof(wb), file);

	m_ary.RemoveAll();
	m_interCom.ResetContent();
	for (int ii = 0; ii < 100; ii++)
	{
		keys.Format("%02d", ii);
		dw = GetPrivateProfileString("GROUPNAME", keys, "", wb, sizeof(wb), file);
		if (dw <= 0)	continue;
		ifile.Format("%s\\user\\%s\\PortFolio.i%02d", Axis::home, Axis::user, ii);
		if (find.FindFile(ifile))
		{
			m_ary.Add(keys);
			m_interCom.AddString(wb);
		}
	}*/
}

void CSetTickInfo::writeInterCode(CString section, int groupN)
{
	if (groupN < 0)	return;
	CString		file, ifile;
	CFile		fileH;

	file.Format("%s\\user\\%s\\%s", Axis::home, Axis::user, TICKSETUP);
	ifile.Format("%s\\user\\%s\\portfolio.i%s", Axis::home, Axis::user, m_ary.GetAt(groupN));

  	if (!fileH.Open(ifile, CFile::modeRead|CFile::typeBinary))
	{
		CString	msg;
		msg.Format("관심종목 파일을 찾을수 없습니다.\n[%s]", ifile);
		::MessageBox(m_hWnd, msg, "파일오류", MB_ICONASTERISK);
		return;
	}

	int	idx = 0;
	struct _inters	inter;
	while (fileH.Read(&inter, sizeof(inter)))
	{
		char	jmCode [16];
		sprintf(jmCode, "%s", (LPSTR)(LPCTSTR)CString(inter.code, sizeof(inter.code)));  
	
		CString keys, jCode = jmCode;	jCode.TrimRight();
		if (jCode.IsEmpty())
			continue;

		if (jCode.GetLength() == 6)
			jCode = "A" + jCode;
		else if (jCode.GetLength() != 8)	// stock, future, call, put
			continue;
		keys.Format("%02d", idx++);
		WritePrivateProfileString(section, keys, jCode, file);
      	}
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
	int	nRes{}, ii{};  //vc2019
	char	buf[256]{};

	file.Format("%s\\%s\\%s", Axis::home, MTBLDIR, TICKSETUP);
	m_bar1cnt	= GetPrivateProfileInt(szGeneral, "BAR00", SPLIT_CNT, file);
	m_bar2cnt	= GetPrivateProfileInt(szGeneral, "BAR01", SPLIT_CNT, file);
	m_view1		= GetPrivateProfileInt(szGeneral, "view1", 1, file);
	m_view2		= GetPrivateProfileInt(szGeneral, "view2", 1, file);
	m_screenbar	= GetPrivateProfileInt(szGeneral, "screenbar", 1, file);
	UpdateData(false);

	m_fontsize = GetPrivateProfileInt(szGeneral, "fontsize", 9, file);
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
	m_bold = GetPrivateProfileInt(szGeneral, "bold", 0, file);
	if (m_bold)	((CButton *) GetDlgItem(IDC_BOLD))->SetCheck(1);
	else		((CButton *) GetDlgItem(IDC_NORMAL))->SetCheck(1);

	GetPrivateProfileString(szGeneral, "fontname", "굴림체", buf, sizeof(buf), file);
	m_fontname = buf;	m_fontname.TrimLeft();		m_fontname.TrimRight();
	((CComboBox *)GetDlgItem(IDC_FONT))->SelectString(-1, m_fontname);

	setFont();

	m_period = GetPrivateProfileInt(szGeneral, "period", 5, file);
	value.Format("%d", m_period);
	((CComboBox *)GetDlgItem(IDC_PERIOD))->SelectString(-1, value);

	loadInter();

	_tickitem* item{};
	//for (int ii = 0; ii < m_arItemInfo.GetSize(); ii++)
	//{
	//	item = (_tickitem*) m_arItemInfo.GetAt(ii);
	//	delete item;
	//}
	m_arItemInfo.clear();
	for (ii = 0; ii < TICKBAR_CNT; ii++)
	{
		for (int jj = 0; jj < SPLIT_CNT; jj++)
		{
			//_tickitem* item = new _tickitem;
			auto item = m_arItemInfo.emplace_back(std::make_shared<_tickitem>());

			section.Format("BAR_INFO_%d%d", ii, jj);
			item->kind = GetPrivateProfileInt(section, "INDEXK", 0, file);
			switch (item->kind)
			{
			case TKIND_INDEX:
				{
					for (int kk = 0, hh = 0; ; kk++)
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
					for (int kk = 0, hh = 0; ; kk++)
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
			case TKIND_INVEST:
				{
					for (int kk = 0, hh = 0; ; kk++)
					{
						keys.Format("%03d", kk);
						hh = GetPrivateProfileInt(section, keys, -1, file);
						if (hh == -1)	break;
						value.Format("%d", hh);
						item->sAry.Add(value);
					}
					keys = _T("AUX");
					item->aux = GetPrivateProfileInt(section, keys, 0, file);

					value.Format("%d", INVEST_KOSPI);
					if (!item->sAry.GetSize())	item->sAry.Add(value);
				}
				break;
			default:
				AfxMessageBox("Tick infomation load Error!");
				continue;
			}
			//m_arItemInfo.Add((CObject *) item);
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
	int		pos{};
	char		buffer[128];
	CString		file, tmpS, dat, section, key;
	CStringArray	ary;

	file.Format("%s\\%s\\%s", Axis::home, TABDIR, PALETTE);
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
	NM_LISTVIEW* listView{};

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
	
	return CSetupDialog::OnNotify(wParam, lParam, pResult);
}

void CSetTickInfo::draw_groupBox(CPaintDC *pdc, int ids)
{
	pdc->SelectObject(m_drawfont);
	CRect rc, out, pout;
	GetDlgItem(ids)->GetClientRect(rc);
	GetDlgItem(ids)->GetWindowRect(out);
	GetWindowRect(pout);
	out.InflateRect(pout);
	/*
	CString tmp;
	tmp.Format("[%d]",out.left);
	AfxMessageBox(tmp);
	*/
	rc.top+=6;
	//rc.right -=20;
	CPen pn_in, pn_out;
	pn_in.CreatePen(PS_SOLID, 1, COLOR_INLINE);
	pn_out.CreatePen(PS_SOLID, 1, COLOR_OUTLINE);
	rc.OffsetRect(out.left, out.top);	
	rc.OffsetRect(1,1);

	pdc->SelectObject(pn_out);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);
	rc.OffsetRect(-1,-1);
	pdc->SelectObject(pn_in);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);
	//pdc->SelectObject(pdc->GetCurrentBrush());
	//Axis::dialogBgColor
	//pdc->SelectObject(&Axis::dialogBgBrush);
	pdc->SetBkColor(Axis::dialogBgColor);
	CString data;
	GetDlgItem(ids)->GetWindowText(data);
	pdc->TextOut(rc.left+7, rc.top-5, data);
}

void CSetTickInfo::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	draw_groupBox(&dc, IDC_STATIC_17);
	draw_groupBox(&dc, IDC_STATIC_18);
	draw_groupBox(&dc, IDC_STATIC_19);
	draw_groupBox(&dc, IDC_STATIC_20);
	draw_groupBox(&dc, IDC_STATIC_21);
	// Do not call CSetupDialog::OnPaint() for painting messages
}

HBRUSH CSetTickInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CSetupDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	const int ids = ::GetWindowLong(pWnd->m_hWnd, GWL_ID);
	if ( (ids == IDC_UPDOWNLIST) || (ids == IDC_INTERGROUP)/* || (ids == IDC_CB_THEME)*/ )
	{
		if(((CComboBox *) GetDlgItem(ids))->IsWindowEnabled())
		//if(pWnd->IsWindowEnabled())
		{
			if (m_bmpCombo)
			{
				CDC dc; 
				dc.CreateCompatibleDC(pDC);
				dc.SelectObject(m_bmpCombo);
				CRect rc;
				CPen pn; pn.CreatePen(PS_SOLID, 1, RGB(125,125,125));
				pDC->SelectObject(&pn);
				((CComboBox *) GetDlgItem(ids))->GetClientRect(rc);
				pDC->Rectangle(rc);
				//pDC->BitBlt(rc.right-19,0,rc.right,20,&dc,0,0,19,20,SRCCOPY);
				pDC->StretchBlt(rc.right-20,0,19,rc.bottom,&dc,0,0,19,20,SRCCOPY);
				dc.DeleteDC();
			}
		}else
		{
			if (m_bmpCombo_N)
			{
				CDC dc; 
				dc.CreateCompatibleDC(pDC);
				dc.SelectObject(m_bmpCombo_N);
				CRect rc;
				CPen pn; pn.CreatePen(PS_SOLID, 1, RGB(125,125,125));
				pDC->SelectObject(&pn);
				((CComboBox *) GetDlgItem(ids))->GetClientRect(rc);
				pDC->Rectangle(rc);
				//pDC->BitBlt(rc.right-19,0,rc.right,20,&dc,0,0,SRCCOPY);
				pDC->StretchBlt(rc.right-20,0,19,rc.bottom,&dc,0,0,19,20,SRCCOPY);
				dc.DeleteDC();
			}		
		}
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
void CSetTickInfo::SearchGroupList()
{
	CString stemp;
	CString strHwnd;
	strHwnd.Format("%d", this->m_hWnd);
	::SendMessage(m_pMain->m_hWnd, WM_INTERMSG, MAKEWPARAM(MMSG_SEARCH_INTERGROUP, 0),  (LPARAM)strHwnd.operator LPCSTR());
}

LRESULT CSetTickInfo::OnMsgFromMain(WPARAM wParam, LPARAM lParam)
{
	CString m_slog;
	int datL = HIWORD(wParam);
	int key = LOWORD(wParam);
	switch (LOWORD(wParam))
	{
		case MMSG_SEARCH_INTERGROUP:
		{
			m_ary.RemoveAll();
			m_interCom.ResetContent();

			CString stmp, seq, sgname;
			char* pdata = (char*)lParam;
			int cnt = atoi(CString(pdata, 4));
			if (cnt == 0) return 0;

			int parseL = 4;
			struct _grSearch
			{
				char ngrs[2];
				char grseq[2];
				char gname[30];
			};

			struct _grSearch* stgr;
			char* pSeq, * pgame;
			pSeq = new char[sizeof(stgr->grseq) + 1];
			pgame = new char[sizeof(stgr->gname) + 1];

			for (int ii = 0; ii < cnt; ii++)
			{
				stgr = (struct _grSearch*)&pdata[parseL];
				memset(pSeq, 0x00, sizeof(stgr->grseq) + 1);
				memcpy(pSeq, stgr->grseq, sizeof(stgr->grseq));
				seq.Format("%s", pSeq);

				memset(pgame, 0x00, sizeof(stgr->gname) + 1);
				memcpy(pgame, stgr->gname, sizeof(stgr->gname));
				sgname.Format("%s", pgame);
				sgname.TrimRight();
	
				parseL += sizeof(struct _grSearch);

				m_ary.Add(seq);
				m_interCom.AddString(sgname);
			}
			m_interCom.SetCurSel(0);
			delete[] pSeq; pSeq = nullptr;
			delete[] pgame; pgame = nullptr;
			
			CString section, file;
			file.Format("%s\\user\\%s\\%s", Axis::home, Axis::user, TICKSETUP);
			for (int ii = 0; ii < TICKBAR_CNT; ii++)
			{
				for (int jj = 0; jj < SPLIT_CNT; jj++)
				{
					section.Format("BAR_INFO_%d%d", ii, jj);
					int ikind = GetPrivateProfileInt(section, "INDEXK", 0, file);
					switch (ikind)
					{
					case TKIND_INTER:
						if (m_interCom.GetCount())
						{
							int iselectN = GetPrivateProfileInt(section, "SELECT", 0, file);
							m_interCom.SetCurSel(iselectN);
						}
						break;
					}
				}
			}
		}
		break;
		case MMSG_RET_SEARCH_GROUPCODE:
		{
	//		m_slog.Format("\r\n------------------------ 관심그룹 코드 조회 [%d] \n", datL);
	//		OutputDebugString(m_slog);
	////		m_pdlg->AddTextEdit(m_slog);
	//		char* pdata = (char*)lParam;

	//		CString stmp;
	//		CString strseq = CString(pdata, 2);
	//		CString strgname = CString(&pdata[2], 20);
	//		int cnt = atoi(CString(&pdata[22], 4));
	//		int parseL = 26;

	//		struct _jinfo* jinfo;
	//		char* pgubn, * pCode, * pxprc, * pxnum;
	//		pgubn = new char[sizeof(jinfo->gubn) + 1];
	//		pCode = new char[sizeof(jinfo->code) + 1];
	//		pxprc = new char[sizeof(jinfo->xprc) + 1];
	//		pxnum = new char[sizeof(jinfo->xnum) + 1];

	//		for (int ii = 0; ii < cnt; ii++)
	//		{
	//			jinfo = (struct _jinfo*)&pdata[parseL];

	//			memset(pgubn, 0x00, sizeof(jinfo->gubn) + 1);
	//			memcpy(pgubn, jinfo->gubn, sizeof(jinfo->gubn));  // 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수

	//			memset(pCode, 0x00, sizeof(jinfo->code) + 1);
	//			memcpy(pCode, jinfo->code, sizeof(jinfo->code));  // 종목코드[12]

	//			memset(pxprc, 0x00, sizeof(jinfo->xprc) + 1);
	//			memcpy(pxprc, jinfo->xprc, sizeof(jinfo->xprc));  // 보유단가[10]

	//			memset(pxnum, 0x00, sizeof(jinfo->xnum) + 1);
	//			memcpy(pxnum, jinfo->xnum, sizeof(jinfo->xnum));  	// 보유수량[10]

	//			stmp.Format("%s\t%s\t%s\t%s", pgubn, pCode, pxprc, pxnum);

	//			m_slog.Format("[%d]    [%s]\n", ii, stmp);
	//			OutputDebugString(m_slog);
	//	//		m_pdlg->AddTextEdit(m_slog);
	//			XMSG(m_slog);

	//			parseL += sizeof(struct _jinfo);
	//		}

	//		delete[] pgubn; pgubn = nullptr;
	//		delete[] pCode; pCode = nullptr;
	//		delete[] pxprc; pxprc = nullptr;
	//		delete[] pxnum; pxnum = nullptr;
		}
		break;
	}
	return 0;
}