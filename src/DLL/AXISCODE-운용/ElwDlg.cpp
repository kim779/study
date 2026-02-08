// ElwDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "ElwDlg.h"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/jmcode.h"
#include "JongmukDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CListCtrl* g_plistCtrl;




/////////////////////////////////////////////////////////////////////////////
// CElwDlg dialog

CElwDlg::CElwDlg(CPoint pt, CWnd* pParent /*=nullptr*/)
	: CDialog(CElwDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElwDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_point = pt;
	m_arrayELWCode.RemoveAll();
	m_arrayPBCode.RemoveAll();
	m_arrayBASECode.RemoveAll();

	m_ElwType = etAll;
	m_CPType = cpAll;
}


void CElwDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElwDlg)
	DDX_Control(pDX, IDC_RADIO_TYPE1, m_rdoType);
	DDX_Control(pDX, IDC_RADIO_ALL, m_rdoAll);
	DDX_Control(pDX, IDC_LIST_ELW, m_listCtrl);
	DDX_Control(pDX, IDC_LIST_COM, m_listPB);
	DDX_Control(pDX, IDC_LIST_BASE, m_listBase);
	DDX_Control(pDX, IDC_COM_MONTH, m_comMonth);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElwDlg, CDialog)
	//{{AFX_MSG_MAP(CElwDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioShare)
	ON_LBN_SELCHANGE(IDC_LIST_BASE, OnSelchangeListBase)
	ON_LBN_SELCHANGE(IDC_LIST_COM, OnSelchangeListCom)
	ON_CBN_SELCHANGE(IDC_COM_MONTH, OnSelchangeComMonth)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ELW, OnDblclkListElw)
	ON_BN_CLICKED(IDC_RADIO_TYPE1, OnElwTypeChange)
	ON_BN_CLICKED(IDC_RADIO_CALL, OnRadioShare)
	ON_BN_CLICKED(IDC_RADIO_PUT, OnRadioShare)
	ON_BN_CLICKED(IDC_RADIO_TYPE2, OnElwTypeChange)
	ON_BN_CLICKED(IDC_RADIO_TYPE3, OnElwTypeChange)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_ELW, OnGetdispinfoListElw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElwDlg message handlers

BOOL CElwDlg::loadELWCode()
{
	int codeN{};

	CFile	file;
	struct  elwcode  ELWCode{};
	struct _elwcode	savecode{};
	PBCODE pbcode;
	CString path; 
	
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\elwcod.dat";

	if (m_arrayELWCode.GetSize() > 0)				return FALSE;
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))	return FALSE;

	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct elwcode));
	std::map<CString, int>			 mMonthSortArr;
	std::vector<std::pair<CString, CString>> vBaseSortArr;

	CJongmukDlg dlg(codeNAME, 0);

	int idx{};
	CString sMonth;
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&ELWCode, sizeof(struct elwcode));

		savecode.code = CString(ELWCode.codx, ELWCodeLen);
		savecode.name.Format("%s", CString(ELWCode.hnam, ELWHNameLen).Mid(1));
		savecode.name.TrimRight();
		savecode.ktype = CString(ELWCode.krye, 2);
		savecode.htype = CString(ELWCode.khbe, 2);
		savecode.pcode = CString(ELWCode.bgce, 12);
		savecode.pname = CString(ELWCode.hbge, 20);
		savecode.mjmt  = CString(ELWCode.mjmt, 8);
		savecode.bkoba = (ELWCode.koba[0]==2) ? TRUE : FALSE;
		for (int jj = 0; jj < 5; jj ++)
			savecode.lpnum[jj] = CString((char*)ELWCode.lphn[jj], 3);
		for (int jj = 0; jj < 5; jj ++)
		{
			savecode.bcode[jj] = CString((char*)ELWCode.gcjs[jj], 12);
			savecode.bcode[jj].TrimRight();
		}
		
		idx = FindPBCode(savecode.pcode);
		if (idx < 0)
		{
			pbcode.code = savecode.pcode;
			pbcode.name = savecode.pname;
			m_arrayPBCode.Add(pbcode);
		}

		for (int jj = 0; jj < 5; jj ++)
		{
			CString bcode;
			bcode.Format("%s", savecode.bcode[jj]); bcode.TrimRight();
			if (bcode == "") break;

			auto ft = std::find_if(vBaseSortArr.begin(), vBaseSortArr.end(), [&bcode](auto const item){
				if (bcode.CompareNoCase(item.first) == 0)
					return true;
				return false;
 			});

			if (ft == vBaseSortArr.end())
			{
				CString sName;
				if (dlg.FindCode(codeNAME, bcode.Mid(1), -1))
					sName = dlg.GetName();
				else if (bcode.CompareNoCase("KOSPI200") == 0)
					continue;
				else if (bcode.CompareNoCase("BASKET") == 0)
					break;
				else
					continue;
				
				vBaseSortArr.emplace_back(std::make_pair(bcode, sName));
			}


		}

		sMonth.Format("%s.%s", savecode.mjmt.Left(4), savecode.mjmt.Mid(4, 2));
		mMonthSortArr.emplace(std::make_pair(std::move(sMonth), std::move(1)));
		m_arrayELWCode.Add(savecode);
	}

	m_comMonth.AddString("전체");
	for_each(mMonthSortArr.begin(), mMonthSortArr.end(), [&](const auto item){
		m_comMonth.AddString(item.first);
	});

	BASECODE basecode;
	std::sort(vBaseSortArr.begin(), vBaseSortArr.end(), [](const auto item, const auto item2){
		return (item.second.Compare(item2.second) < 0);
	});

	for_each(vBaseSortArr.begin(), vBaseSortArr.end(), [&](const auto item){
		basecode.code = item.first;
 		basecode.name = item.second;
 		m_arrayBASECode.Add(basecode);
	});

	file.Close();
	return TRUE;
}

void CElwDlg::SetDlgPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	int	scWidth{}, scHeight{};
	scWidth	= GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CAxisCodeApp* pApp = (CAxisCodeApp *)AfxGetApp();
	const int virScrX = pApp->GetVirtualScreenX();
	if (virScrX > 0)
		scWidth = virScrX;

	CRect	rc;
	GetClientRect(&rc);

	const int	gabX = scWidth - (x + rc.Width());
	const int	gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(nullptr, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}



void CElwDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	EndDialog(1);	
}

BOOL CElwDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgPos(m_point.x, m_point.y);

	loadELWCode();

	const DWORD dwStyle = m_listCtrl.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE) | LVS_EX_FULLROWSELECT;
	m_listCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
	
	m_listCtrl.SetBkColor(RGB(237,237,237));
	m_listCtrl.SetTextBkColor(RGB(237,237,237));
	m_listCtrl.SetTextColor(RGB(0,0,0));

	m_listCtrl.InsertColumn(1, "코드", LVCFMT_LEFT, 50);
	m_listCtrl.InsertColumn(2, "종목명", LVCFMT_LEFT, 190);
	m_listCtrl.InsertColumn(3, "최종일", LVCFMT_LEFT, 70);
	//m_listCtrl.InsertColumn(4, "구분", LVCFMT_LEFT, 60);

	CountBase();
	CountData();
	m_listPB.SetCurSel(0);
	m_listBase.SetCurSel(0);
	m_comMonth.SetCurSel(0);
	m_rdoAll.SetCheck(true);
	m_rdoType.SetCheck(true);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CElwDlg::CountBase()
{
	m_listPB.ResetContent();

	m_listPB.AddString(" 전체");
	PBCODE pbcode;
	for (int ii = 0; ii < m_arrayPBCode.GetSize(); ii++)
	{
		pbcode = m_arrayPBCode.GetAt(ii);
		m_listPB.AddString(" " + pbcode.name);
	};

	m_listBase.ResetContent();
	m_listBase.AddString(" 전체");
	m_listBase.AddString(" KOSPI200");
	m_listBase.AddString(" KOSDAQ150");	//2016.12.26 KOSDAQ150 추가
	m_listBase.AddString(" BASKET");
	BASECODE basecode;
	for (int ii = 0; ii < m_arrayBASECode.GetSize(); ii++)
	{
		basecode = m_arrayBASECode.GetAt(ii);
		m_listBase.AddString(basecode.name);
	};

	return TRUE;
}

BOOL CElwDlg::CountData()
{
	struct _elwcode ncode;
	const int ct = m_arrayELWCode.GetSize();
	CString tcode, bcode, pcode;
	BASECODE basecode;
	PBCODE pbcode;
	ITEMS item;
	
	if (ct <= 0)	return FALSE;

	CString sMonth("");
	if (m_comMonth.GetCurSel() > 0) // 0:전체
	{
		m_comMonth.GetLBText(m_comMonth.GetCurSel(), sMonth);
		sMonth.Replace(".", "");
	}
	
	int nIdx = m_listPB.GetCurSel();
	if (nIdx <= 0)
	{
		pcode = "";
	}
	else
	{
		pbcode = m_arrayPBCode[nIdx -1];
		pcode = pbcode.code;
	}

	m_listCtrl.DeleteAllItems();
	_vData.clear();
	UpdateData(TRUE);

	for (int ii = 0; ii < ct ; ii++)
	{
		ncode = m_arrayELWCode[ii];

		nIdx = m_listBase.GetCurSel();
		if (nIdx == 1)
		{
			bcode = "KOSPI200";
			bool bBaseCode = false;
			for (int jj = 0; jj < 5; jj++)
			{
				if (ncode.bcode[jj].CompareNoCase(bcode) == 0) 
					bBaseCode = true;
			}
			if (bBaseCode == false) continue;
		}
		else if (nIdx == 2)		//2016.12.26 KOSDAQ150 추가
		{
			bcode = "KOSDAQ150";
			bool bBaseCode = false;
			for (int jj = 0; jj < 5; jj++)
			{
				if (ncode.bcode[jj].CompareNoCase(bcode) == 0) 
					bBaseCode = true;
			}
			if (bBaseCode == false) continue;
		}
		else if (nIdx == 3)
		{
			bcode = "BASKET";
			bool bBaseCode = false;
			for (int jj = 0; jj < 5; jj++)
			{
				if (ncode.bcode[jj].CompareNoCase(bcode) == 0) 
					bBaseCode = true;
			}
			if (bBaseCode == false) continue;
		}
		else if (nIdx > 3)
		{
			basecode = m_arrayBASECode[nIdx - 4];	//2016.12.26 KSJ 4부터 들어옴
			bcode = basecode.code;
			if (nIdx == 1) bcode.TrimLeft();

			bool bBaseCode = false;
			for (int jj = 0; jj < 5; jj++)
			{
				if (ncode.bcode[jj].CompareNoCase("BASKET") == 0)
					break;
				if (ncode.bcode[jj].CompareNoCase(bcode) == 0) 
					bBaseCode = true;
			}
			if (bBaseCode == false) continue;
		}

		if (!pcode.IsEmpty())
		{
			if (ncode.pcode != pcode) continue;
		}

		switch (m_CPType)
		{
		case cpCall:
			if (ncode.ktype != "01") continue;
			break;
		case cpPut:
			if (ncode.ktype != "02") continue;
			break;
		}

		switch (m_ElwType)
		{
		case etNormal:
			if (ncode.bkoba) continue;
			break;
		case etKoba:
			if (!ncode.bkoba) continue;
			break;
		}

		if (!sMonth.IsEmpty() && (sMonth.CompareNoCase(ncode.mjmt.Left(6)) != 0))
			continue;

		//AddItem(idx, ncode.code, ncode.name, ncode.mjmt);
		//item.code = ncode.code;
		//item.name = ncode.name;
		//item.date = ncode.mjmt;
		_vData.emplace_back(std::move(std::make_tuple(ncode.code, ncode.name, ncode.mjmt)));
		//m_arList.SetAtGrow(idx,item);
		//idx++;
	}

	//std::sort(_vData.)


	m_listCtrl.SetItemCountEx(_vData.size());
	m_listCtrl.Invalidate();
	
	listsort(0);

	g_plistCtrl = &m_listCtrl;	
	return TRUE;
}

int CElwDlg::FindPBCode(CString code)
{
	const bool bRegistered = false;
	PBCODE pbcode;
	for (int ii = 0; ii < m_arrayPBCode.GetSize(); ii++)
	{
		pbcode = m_arrayPBCode.GetAt(ii);
		if (pbcode.code == code)
		{
			return ii;
		}
	}
	return -1;
}

void CElwDlg::OnRadioShare() 
{
	// TODO: Add your control notification handler code here
	if (((CButton *)GetDlgItem(IDC_RADIO_ALL))->GetCheck() == TRUE)
	{
		m_CPType = cpAll;
	}
	else if (((CButton *)GetDlgItem(IDC_RADIO_CALL))->GetCheck() == TRUE)
	{
		m_CPType = cpCall;
	}
	else // if (((CButton *)GetDlgItem(IDC_RADIO_PUT))->GetCheck() == TRUE)
	{
		m_CPType = cpPut;
	}
	CountData();
}

void CElwDlg::OnSelchangeListBase() 
{
	CountData();
}

void CElwDlg::OnSelchangeListCom() 
{
	CountData();
}

void CElwDlg::OnSelchangeComMonth() 
{
	CountData();
}

void CElwDlg::OnDblclkListElw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const int item = m_listCtrl.GetSelectionMark();
	
	m_szCode = m_listCtrl.GetItemText(item, 0);
	m_szName = m_listCtrl.GetItemText(item, 1);
	
	OnOK();
	
	*pResult = 0;
}

BOOL CElwDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == IDC_LIST_ELW)// <-- 여기서 그리드 컨트롤을 구분..
 	{
 		switch(((LPNMHDR) lParam)->code)
 		{
 		case LVN_COLUMNCLICK:
 			#define pnm ((LPNMLISTVIEW)lParam)
			listsort((LPARAM)pnm->iSubItem);
 			#undef pnm
			break;
 		}
 	}			
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CElwDlg::listsort(int column)
{
	m_gListsort = !m_gListsort;
	std::sort(_vData.begin(), _vData.end(), [column](const auto item, const auto item1) {	
		switch (column)
		{
		case 1:
			if (m_gListsort)
				return (std::get<1>(item).Compare(std::get<1>(item1)) > 0);
			else 
				return (std::get<1>(item).Compare(std::get<1>(item1)) < 0);
			break;
		case 2:
			if (m_gListsort)
				return (std::get<2>(item).Compare(std::get<2>(item1)) > 0);
			else
				return (std::get<2>(item).Compare(std::get<2>(item1)) < 0);
			break;
		case 0:
		default:
			if (m_gListsort)
				return (std::get<0>(item).Compare(std::get<0>(item1)) > 0);
			else
				return (std::get<0>(item).Compare(std::get<0>(item1)) < 0);
			break;
		}
		return false;
	});
	m_listCtrl.Invalidate();
}

void CElwDlg::OnElwTypeChange() 
{
	if      (((CButton*)GetDlgItem(IDC_RADIO_TYPE1))->GetCheck() == TRUE ) m_ElwType = etAll;
	else if (((CButton*)GetDlgItem(IDC_RADIO_TYPE2))->GetCheck() == TRUE ) m_ElwType = etNormal;
	else if (((CButton*)GetDlgItem(IDC_RADIO_TYPE3))->GetCheck() == TRUE ) m_ElwType = etKoba;
	CountData();
}

void CElwDlg::OnGetdispinfoListElw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;
	
	auto [code, name, date] = _vData.at(pItem->iItem);
	if (pItem->mask & LVIF_TEXT) //valid text buffer?
	{
		switch(pItem->iSubItem)
		{
			case 0:
				lstrcpy(pItem->pszText, code);
				break;
			case 1:
				lstrcpy(pItem->pszText, name);
				break;
			case 2:
				lstrcpy(pItem->pszText, date);
				break;
		}
	}
		
	*pResult = 0;
}
