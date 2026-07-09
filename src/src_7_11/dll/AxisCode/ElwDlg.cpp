#include "stdafx.h"
#include "axiscode.h"
#include "ElwDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ASCENDING	0
#define	DESCENDING	1

// 검색combo선택
#define	SR_FRONT	0
#define	SR_WORD		1

#define	HANGULMSK	0x80
#define	ChosungS	0xA4A1	// ㄱ
#define	ChosungE	0xA4BE	// ㅎ


CListCtrl* g_plistCtrl;

int CALLBACK CompareElwFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
int CALLBACK ElwListViewCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

CElwDlg::CElwDlg(CPoint pt, CWnd* pParent /*=NULL*/)
	: CDialog(CElwDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElwDlg)
	//}}AFX_DATA_INIT
	m_point = pt;
	m_gListSort = FALSE;
	m_arSelCode.RemoveAll();

	m_bAddAll = true;
}

void CElwDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElwDlg)
	DDX_Control(pDX, IDC_BUTTON_9, m_ctlBtn9);
	DDX_Control(pDX, IDC_BUTTON_8, m_ctlBtn8);
	DDX_Control(pDX, IDC_BUTTON_7, m_ctlBtn7);
	DDX_Control(pDX, IDC_BUTTON_6, m_ctlBtn6);
	DDX_Control(pDX, IDC_BUTTON_5, m_ctlBtn5);
	DDX_Control(pDX, IDC_BUTTON_4, m_ctlBtn4);
	DDX_Control(pDX, IDC_BUTTON_3, m_ctlBtn3);
	DDX_Control(pDX, IDC_BUTTON_22, m_ctlBtn22);
	DDX_Control(pDX, IDC_BUTTON_21, m_ctlBtn21);
	DDX_Control(pDX, IDC_BUTTON_20, m_ctlBtn20);
	DDX_Control(pDX, IDC_BUTTON_2, m_ctlBtn2);
	DDX_Control(pDX, IDC_BUTTON_19, m_ctlBtn19);
	DDX_Control(pDX, IDC_BUTTON_18, m_ctlBtn18);
	DDX_Control(pDX, IDC_BUTTON_17, m_ctlBtn17);
	DDX_Control(pDX, IDC_BUTTON_16, m_ctlBtn16);
	DDX_Control(pDX, IDC_BUTTON_15, m_ctlBtn15);
	DDX_Control(pDX, IDC_BUTTON_14, m_ctlBtn14);
	DDX_Control(pDX, IDC_BUTTON_13, m_ctlBtn13);
	DDX_Control(pDX, IDC_BUTTON_12, m_ctlBtn12);
	DDX_Control(pDX, IDC_BUTTON_11, m_ctlBtn11);
	DDX_Control(pDX, IDC_BUTTON_10, m_ctlBtn10);
	DDX_Control(pDX, IDC_BUTTON_1, m_ctlBtn1);
	DDX_Control(pDX, IDC_COMBO_SEARCH, m_ctlComboSearch);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_edtSearch);
	DDX_Control(pDX, IDC_RADIO_ALL, m_rdoAll);
	DDX_Control(pDX, IDC_LIST_COM, m_listPB);
	DDX_Control(pDX, IDC_LIST_BASE, m_listBase);
	DDX_Control(pDX, IDC_LIST_ELW, m_listCtrl);
	DDX_Control(pDX, IDC_COM_MONTH, m_comMonth);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CElwDlg, CDialog)
	//{{AFX_MSG_MAP(CElwDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
	ON_BN_CLICKED(IDC_RADIO_CALL, OnRadioCall)
	ON_BN_CLICKED(IDC_RADIO_PUT, OnRadioPut)
	ON_LBN_SELCHANGE(IDC_LIST_BASE, OnSelchangeListBase)
	ON_LBN_SELCHANGE(IDC_LIST_COM, OnSelchangeListCom)
	ON_CBN_SELCHANGE(IDC_COM_MONTH, OnSelchangeComMonth)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ELW, OnDblclkListElw)
	ON_CBN_SELENDOK(IDC_COMBO_SEARCH, OnSelendokComboSearch)
//	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BUTTON_1, IDC_BUTTON_22, OnClickBtnSort)
	ON_MESSAGE(WM_EDITX, OnEditXMsg)
END_MESSAGE_MAP()

BOOL CElwDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgPos(m_point.x, m_point.y);

//	loadELWCode();
	
	initBtn();
	initListCtrl();
	fillListBox();
	fillListCtrl();
	FillMonth();

	m_listPB.SetCurSel(0);
	m_listBase.SetCurSel(0);
	m_comMonth.SetCurSel(0);
	m_rdoAll.SetCheck(true);
	m_cpType = cpAll;

	m_ctlComboSearch.SetCurSel(0);
	
	return TRUE;
}

void CElwDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	clearListCtrl();
	
	m_arSelCode.RemoveAll();
	EndDialog(1);	
}

void CElwDlg::OnRadioAll() 
{
	fillListCtrl();
}

void CElwDlg::OnRadioCall() 
{
	fillListCtrl();
}

void CElwDlg::OnRadioPut() 
{
	fillListCtrl();
}

void CElwDlg::OnSelchangeListBase() 
{
	fillListCtrl();
}

void CElwDlg::OnSelchangeListCom() 
{
	fillListCtrl();
}

void CElwDlg::OnSelchangeComMonth() 
{
	fillListCtrl();	
}

void CElwDlg::OnDblclkListElw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_szCode = m_szName = "";

	int item = m_listCtrl.GetSelectionMark();
	
	m_szCode = m_listCtrl.GetItemText(item, 0);
	m_szName = m_listCtrl.GetItemText(item, 1);
	
	OnOK();	
	*pResult = 0;
}

void CElwDlg::SetDlgPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	int	scWidth, scHeight;
	scWidth	= GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CRect	rc;
	GetClientRect(&rc);

	int	gabX = scWidth - (x + rc.Width());
	int	gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(NULL, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}


BOOL CElwDlg::loadELWCode()
{
/*	
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int	codeN;
	struct  elwcode  ELWCode;
	struct _elwcode* savecode;
	PBCODE pbcode;
	CString path; 
	int	ii, jj, kk;
	struct	_sortStr *pMonthSortArr = NULL, *pBaseSortArr = NULL, *pPBSortArr = NULL;
	
	if (m_arrayELWCode.GetSize() > 0)	
		return FALSE;

	codeN = pApp->m_arrayELWCode.GetSize();
	pMonthSortArr = new struct _sortStr[codeN];
	pBaseSortArr = new struct _sortStr[codeN];
	pPBSortArr = new struct _sortStr[codeN];

	bool bFound;
	int nMonth = 0, nBase = 0, pbNum = 0;
	CString sMonth;
	CString tmpS;
	CString jName;
	for (ii = 0; ii < codeN; ii++)
	{
		ELWCode = pApp->m_arrayELWCode.GetAt(ii);
		savecode = new struct _elwcode;
		jName = CString(ELWCode.codx, ELWCodeLen);
		savecode->code = jName.Mid(1);
		savecode->name = CString(ELWCode.hnam, ELWHNameLen);
		savecode->name.TrimRight();
		savecode->ktype = CString(ELWCode.krye, sizeof(ELWCode.krye));
		savecode->pcode = CString(ELWCode.bgce, sizeof(ELWCode.bgce));
		savecode->pname = CString(ELWCode.hbge, sizeof(ELWCode.hbge));
		savecode->mjmt  = CString(ELWCode.mjmt, sizeof(ELWCode.mjmt));
	
		for (jj = 0; jj < 5; jj ++)
		{
			savecode->bcode[jj] = CString((char*)ELWCode.gcjs[jj], 12);
			savecode->bcode[jj].TrimRight();
		}
		
		// 발행기관
		bFound = false;
		for (kk = 0; kk < pbNum; kk++)
		{
			if (savecode->pcode.CompareNoCase(pPBSortArr[kk].code) == 0)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			pPBSortArr[pbNum].code = savecode->pcode;
			pPBSortArr[pbNum].name = savecode->pname;
			pbNum++;
		}

		// 기초자산
		for (jj = 0; jj < 5; jj ++)
		{
			CString bcode;
			bcode.Format("%s", savecode->bcode[jj]); bcode.TrimRight();
			if (bcode == "") break;
			bFound = false;
			for (kk = 0; kk < nBase; kk++)
			{
				if (bcode.CompareNoCase(pBaseSortArr[kk].code) == 0)
				{
					bFound = true;
					break;
				}
			}

			if (!bFound)
			{
				if (findJCode(bcode, tmpS))
					pBaseSortArr[nBase].name = tmpS;
				else if (bcode.CompareNoCase("KOSPI200") == 0)
					continue;

					//pBaseSortArr[nBase].name = bcode;
				else
					continue;
				pBaseSortArr[nBase].code = bcode;
				nBase++;
			}
		}

		// 만기월
		bFound = false;
		sMonth.Format("%s.%s", savecode->mjmt.Left(4), savecode->mjmt.Mid(4, 2));
		for (jj = 0; jj < nMonth; jj++)
		{
			if (sMonth.CompareNoCase(pMonthSortArr[jj].code) ==0)
			{
				bFound = true;
				break;
			}
		}
		if (!bFound)
		{
			pMonthSortArr[nMonth].code = sMonth;
			nMonth++;
		}

		// ELW코드저장
		m_arrayELWCode.Add(savecode);
	}

	// combo에 만기년월 combo
	m_comMonth.AddString("전체");
	qsort((void*)pMonthSortArr, nMonth, sizeof(struct _sortStr), _sortCode);
	for (ii = 0; ii < nMonth; ii++)
	{
		m_comMonth.AddString(pMonthSortArr[ii].code);
	}

	// 기초자산(한글명순)
	BASECODE basecode;
	qsort((void*)pBaseSortArr, nBase, sizeof(struct _sortStr), _sortName);
	for (ii = 0; ii < nBase; ii++)
	{
		basecode.code = pBaseSortArr[ii].code;
		basecode.name = pBaseSortArr[ii].name;
		m_arrayBASECode.Add(basecode);
	}

	// KOSPI200 은 뒤로
	basecode.code = "KOSPI200";
	basecode.name = "KOSPI200";
	m_arrayBASECode.Add(basecode);

	// 발행기관(코드순)
	qsort((void*)pPBSortArr, pbNum, sizeof(struct _sortStr), _sortCode);
	for (ii = 0; ii < pbNum; ii++)
	{
		pbcode.code = pPBSortArr[ii].code;
		pbcode.name = pPBSortArr[ii].name;
		m_arrayPBCode.Add(pbcode);
	}

	delete[] pMonthSortArr;	pMonthSortArr = NULL;
	delete[] pBaseSortArr;	pBaseSortArr = NULL;
	delete[] pPBSortArr;	pPBSortArr = NULL;
*/
	return TRUE;
}

void CElwDlg::FillMonth()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	// combo에 만기년월 combo
	m_comMonth.AddString("전체");
	for (int ii = 0; ii < pApp->m_arrayElwMonth.GetSize(); ii++)
	{
		m_comMonth.AddString(pApp->m_arrayElwMonth.GetAt(ii));
	}

	
}
void CElwDlg::clearListCtrl()
{
	m_listCtrl.RemoveAllCustomItem();
/*	for (int ii = 0; ii < m_listCtrl.GetItemCount(); ii++)
	{
		_listItem *pItem1 = (_listItem *)m_listCtrl.GetItemData(ii);
		if (pItem1) 
			delete pItem1;
	}

	m_listCtrl.DeleteAllItems();*/
}

void CElwDlg::initListCtrl()
{
//	DWORD dwStyle = m_listCtrl.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE) | LVS_EX_FULLROWSELECT;
//	m_listCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
	
	m_listCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listCtrl.SetBkColor(COR_WHITE);
	m_listCtrl.SetTextBkColor(COR_WHITE);
	m_listCtrl.SetTextColor(RGB(0,0,0));
	m_listCtrl.SetCustomDrawList(FALSE);
	//m_listCtrl.SetBkColor(RGB(237,237,237));
	//m_listCtrl.SetTextBkColor(RGB(237,237,237));
	//m_listCtrl.SetTextColor(RGB(0,0,0));

	m_listCtrl.InsertColumn(1, "종목코드", LVCFMT_LEFT, 60);
	m_listCtrl.InsertColumn(2, "종목명", LVCFMT_LEFT, 139);
}

void CElwDlg::fillListBox()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	// 발행사
	PBCODE pbcode;

	m_listPB.ResetContent();
	m_listPB.AddString(" 전체");
	INT_PTR ii = 0;

	for (ii = 0; ii < pApp->m_arrayPBCode.GetSize(); ii++)
	{
		pbcode = pApp->m_arrayPBCode.GetAt(ii);
		m_listPB.AddString(" " + pbcode.name);
	};

	// 기초자산
	BASECODE basecode;

	m_listBase.ResetContent();
	m_listBase.AddString(" 전체");
	m_listBase.AddString(" 바스켓");
	for (ii = 0; ii < pApp->m_arrayBASECode.GetSize(); ii++)
	{
		basecode = pApp->m_arrayBASECode.GetAt(ii);
		m_listBase.AddString(" " + basecode.name);
	};
}

void CElwDlg::fillListCtrl()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	struct _elwcode* ncode;
	CString tcode, bcode, pcode;
	BASECODE basecode;
	PBCODE pbcode;
	CString sMonth("");
	int	cnt = 0;

	if (((CButton *)GetDlgItem(IDC_RADIO_ALL))->GetCheck())
		m_cpType = cpAll;
	else if (((CButton *)GetDlgItem(IDC_RADIO_CALL))->GetCheck())
		m_cpType = cpCall;
	else if (((CButton *)GetDlgItem(IDC_RADIO_PUT))->GetCheck())
		m_cpType = cpPut;
	else
		m_cpType = cpAll;
	
	INT_PTR ct = pApp->m_arrayDlgELWCode.GetSize();
	if (ct <= 0)	
		return;

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
		pbcode = pApp->m_arrayPBCode.GetAt(nIdx-1);
		pcode = pbcode.code;
	}

	m_listCtrl.SetRedraw(FALSE);
	clearListCtrl();
	m_arSelCode.RemoveAll();

	int idx = 0;
	for (int ii = 0; ii < ct ; ii++)
	{
		ncode = pApp->m_arrayDlgELWCode.GetAt(ii);

		nIdx = m_listBase.GetCurSel();
		if (nIdx >= 2)
		{
			basecode = pApp->m_arrayBASECode.GetAt(nIdx-2);
			bcode = basecode.code;

			bool bBaseCode = false;
			for (int jj = 0; jj < 5; jj++)
			{
				if (ncode->bcode[jj].CompareNoCase(bcode) == 0) 
					bBaseCode = true;
			}
			if (bBaseCode == false) 
				continue;
		}
		else if (nIdx == 1)
		{
			if (ncode->name.Find("바스켓") != -1)
			{
				addItem(idx, ncode->code, ncode->name);
				m_arSelCode.Add(ncode);
				continue;
			}
			else 
				continue;
		}
		

		if (!pcode.IsEmpty())
		{
			if (ncode->pcode != pcode) 
				continue;
		}

		switch (m_cpType)
		{
		case cpCall:
			if (ncode->ktype != "01") 
				continue;
			break;
		case cpPut:
			if (ncode->ktype != "02") 
				continue;
			break;
		}

		if (!sMonth.IsEmpty() && (sMonth.CompareNoCase(ncode->mjmt.Left(6)) != 0))
			continue;

		addItem(idx, ncode->code, ncode->name);
		m_arSelCode.Add(ncode);
		idx++;
	}

	m_listCtrl.SetItemCount(idx);
	m_gListSort = FALSE;
	g_plistCtrl = &m_listCtrl;
	m_bAddAll = true;

	cnt = m_listCtrl.GetItemCount();
	if (cnt == 0)
		return;

//	m_listCtrl.SortItems(CompareElwFunc, MAKELPARAM(0, DESCENDING));

	if (cnt <= 20)
	{
		m_listCtrl.SetColumnWidth(1, 155);
	}
	else
	{
		m_listCtrl.SetColumnWidth(1, 139);
	}
	
	m_edtSearch.SetWindowText("");

	m_listCtrl.Selected(0);
	m_listCtrl.SetRedraw(TRUE);
}

void CElwDlg::addItem(int row, CString szCode, CString szName)
{
	LISTITEM* pitem;		
	pitem = new LISTITEM;
	strcpy((char*)pitem->Code, szCode);
	strcpy((char*)pitem->Name, szName);
	m_listCtrl.AddCustomItem((DWORD)pitem);
}

bool CElwDlg::findJCode(CString code, CString& name)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	hjcode*	jCode;
	for (int ii = 0; ii < pApp->m_arrayHJCode.GetSize(); ii++)
	{
		jCode = pApp->m_arrayHJCode.GetAt(ii);
		if (jCode->code == code)
		{
			name = CString(jCode->hnam, HNameLen);
			name.TrimRight();
			return true;
		}
	}
	return false;
}


int CALLBACK CompareElwFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CString    strItem1 = g_plistCtrl->GetItemText((int)lParam1, LOWORD(lParamSort));
	CString    strItem2 = g_plistCtrl->GetItemText((int)lParam2, LOWORD(lParamSort));

	if (HIWORD(lParamSort) == ASCENDING)
		return strcmp(strItem2, strItem1);
	return strcmp(strItem1, strItem2);
}

BOOL CElwDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == IDC_LIST_ELW)
 	{
 		switch (((LPNMHDR)lParam)->code)
 		{
 		case LVN_COLUMNCLICK:
 			#define pnm ((LPNMLISTVIEW)lParam)
			switch ((LPARAM)pnm->iSubItem)
			{
 			case 0:
 				m_gListSort = !m_gListSort;
 				break;
 			case 1:
 				m_gListSort = !m_gListSort;
				break;
 			}
			m_listCtrl.SortCustomItem((LPARAM)pnm->iSubItem, m_gListSort);
			ListView_SortItems((HWND)pnm->hdr.hwndFrom, ElwListViewCompareFunc, (LPARAM)pnm->iSubItem);
 			
 			#undef pnm

			break;
 		}
 	}	
	return CDialog::OnNotify(wParam, lParam, pResult);
}

int CALLBACK ElwListViewCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{
	_listItem *pItem1 = (_listItem *) lParam1; 
	_listItem *pItem2 = (_listItem *) lParam2; 

	int	iCmp=0;

	switch ((int)lParamSort)
	{
	case 0:
		iCmp = lstrcmpi((char*)pItem1->Code, (char*)pItem2->Code); 
		break;
	case 1:
		{
			CString s1((char*)pItem1->Name), s2((char*)pItem2->Name);
			s1.TrimLeft(); s2.TrimLeft();
			iCmp = lstrcmpi(s1, s2);
		}
		break;
	}

	if (m_gListSort)
		iCmp *= -1;

	return iCmp;
}


/*
HBRUSH CElwDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here	
	DeleteObject(m_hBrush);

	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		m_hBrush = CreateSolidBrush(USER_BACKCOLOR);
		
		return m_hBrush;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
*/

void CElwDlg::initBtn()
{
	short	shBtnColor = 245;
	int nheight	= 9;
	CString sztext	= "굴림";

	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	CString strImg("");
	strImg.Format("%s/%s/", pApp->m_RootPath, "image");

	UINT button[] = {
		IDC_BUTTON_1, IDC_BUTTON_2, IDC_BUTTON_3, IDC_BUTTON_4, IDC_BUTTON_5, 
		IDC_BUTTON_6, IDC_BUTTON_7, IDC_BUTTON_8, IDC_BUTTON_9, IDC_BUTTON_10, 
		IDC_BUTTON_11, IDC_BUTTON_12, IDC_BUTTON_13, IDC_BUTTON_14, IDC_BUTTON_15, 
		IDC_BUTTON_16, IDC_BUTTON_17, IDC_BUTTON_18, IDC_BUTTON_19, IDC_BUTTON_20, 
		IDC_BUTTON_21, IDC_BUTTON_22
	};

	for (int ii = 0 ; ii < sizeof(button)/sizeof(UINT) ; ii++)
	{
		//((CButtonST*)GetDlgItem(button[ii]))->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		//((CButtonST*)GetDlgItem(button[ii]))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		//((CButtonST*)GetDlgItem(button[ii]))->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
		//((CButtonST*)GetDlgItem(button[ii]))->SetWnd(this);
		((CfxButton*)GetDlgItem(button[ii]))->SetBtnCursor(IDC_CURSOR_HAND);
		((CfxButton*)GetDlgItem(button[ii]))->LoadImg(strImg);
	}
}

#include "jongmukdlg.h"
void CElwDlg::OnClickBtnSort(UINT nID)
{
	CString	sName, sCode, sTitle;
	CWnd	*pBtn = GetDlgItem(nID);
	pBtn->GetWindowText(sTitle);

	if (sTitle == _T("영문"))
	{
		struct JCode* jcode;
		int	position = 0;

		m_listCtrl.SetRedraw(FALSE);
		clearListCtrl();
		for (int ii = 0; ii < m_arSelCode.GetSize(); ii++)
		{
			jcode = (struct JCode*)m_arSelCode.GetAt(ii);

			sName = CString(jcode->Name, jcode->Name.GetLength());
			//if (jcode->ssgb != jmELW)	sName = sName.Mid(1);
			if (0x41 >  sName.GetAt(1) || sName.GetAt(1) > 0x7A)
				continue;

			sCode = jcode->Code;
			sName.TrimRight();
			addItem(position, sCode, sName);
			position += 1;
		}
 
		if (m_listCtrl.GetItemCount() > 0)
		{
			m_gListsort = FALSE;
			m_listCtrl.SortCustomItem(1, m_gListSort);
//			ListView_SortItems(m_listCtrl.GetSafeHwnd(), ElwListViewCompareFunc, 1);

			m_listCtrl.Selected(0);
			m_szCode = m_listCtrl.GetItemText(0, 0);
			m_szName = m_listCtrl.GetItemText(0, 1);
			m_szName.TrimLeft();
		}

		m_listCtrl.SetRedraw(TRUE);
		m_edtSearch.SetWindowText("");
	}
	else if (sTitle == _T("지우기"))
	{
		m_edtSearch.SetWindowText("");
		if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			searchAction(0);
		else
		{
			m_bAddAll = false;
			searchWord();
		}
	}
	else if (sTitle == _T("←"))
	{
		m_edtSearch.GetWindowText(sName);
		if (sName.IsEmpty())
		{
			if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			{
				searchAction(0);
			}
			else
			{
				m_bAddAll = false;
				searchWord();
			}
		}
		else if (sName.GetLength() == 1)	// 문자
		{
			m_edtSearch.SetWindowText("");
			if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
				searchAction(0);
			else
			{
				m_bAddAll = false;
				searchWord();
			}
		}
		else
		{
			char	cChar = sName.GetAt(sName.GetLength()-1);
			if (cChar & HANGULMSK)	sName = sName.Left(sName.GetLength() - 2);
			else			sName = sName.Left(sName.GetLength() - 1);

			m_edtSearch.SetWindowText(sName);
			if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			{
				if (sName.IsEmpty())
					searchAction(0);
				else
					searchAction(1);
			}
			else
			{
				searchWord();
			}
		}
	}
	else
	{
		m_edtSearch.GetWindowText(sName);
		m_edtSearch.SetWindowText(sName + sTitle);

		if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			searchAction(1);
		else
			searchWord();
	}

	m_edtSearch.SetFocus();
	m_edtSearch.SetSel(0, -1);
	m_edtSearch.SetSel(-1, -1);
}

LRESULT CElwDlg::OnEditXMsg(WPARAM wParam, LPARAM lParam)
{
	UpdateData();
	CString	string;

	switch ((int) wParam)
	{
	case wpBLANK:
		if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			searchAction(0);
		else
			searchWord();
		break;
	case wpENTER:
		break;
	case wpSEARCH1:
		if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			searchAction(0);	// number search by kwon
		else
			searchWord();
		break;
	case wpSEARCH2:
		if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
			searchAction(1);	// char search by kwon
		else
			searchWord();
		break;
	case wpUP:
		if (m_listCtrl.GetItemCount() > 0)
		{
			POSITION	pos = m_listCtrl.GetFirstSelectedItemPosition();
			if (pos == NULL)
				break;
			int nItem = m_listCtrl.GetNextSelectedItem(pos);
			nItem--;
			if (nItem < 0)	nItem = 0;
			m_listCtrl.Selected(nItem);
			m_szCode = m_listCtrl.GetItemText(nItem, 0);
			m_szName = m_listCtrl.GetItemText(nItem, 1);
			m_szName.TrimLeft();

			m_edtSearch.SetFocus();
			m_edtSearch.SetSel(0, -1);
			m_edtSearch.SetSel(-1, -1);
		}
		break;
	case wpDOWN:
		if (m_listCtrl.GetItemCount() > 0)
		{
			POSITION	pos = m_listCtrl.GetFirstSelectedItemPosition();
			if (pos == NULL)
				break;
			int nItem = m_listCtrl.GetNextSelectedItem(pos);
			nItem++;
			if (nItem >= m_listCtrl.GetItemCount())	nItem = m_listCtrl.GetItemCount() - 1;
			m_listCtrl.Selected(nItem);
			m_szCode = m_listCtrl.GetItemText(nItem, 0);
			m_szName = m_listCtrl.GetItemText(nItem, 1);
			m_szName.TrimLeft();

			m_edtSearch.SetFocus();
			m_edtSearch.SetSel(0, -1);
			m_edtSearch.SetSel(-1, -1);
		}
		break;
	}

	return 0;
}

void CElwDlg::SearchString(CString szText)
{
	int len = szText.GetLength();

	m_gListsort = FALSE;
	ListView_SortItems(m_listCtrl.GetSafeHwnd(), ElwListViewCompareFunc, 1);

	for (int ii = 0; ii < m_listCtrl.GetItemCount(); ii++)
	{
		CString str = m_listCtrl.GetItemText(ii, 1);
		
		str = str.Left(len);

		if (szText.CompareNoCase(str) == 0)
		{
			m_listCtrl.Selected(ii);
			
			m_szCode = m_listCtrl.GetItemText(ii, 0);
			m_szName = m_listCtrl.GetItemText(ii, 1);
			return;
		}
	}
}

void CElwDlg::OnSelendokComboSearch() 
{
	//m_bAddAll = false;
	if (m_ctlComboSearch.GetCurSel() == SR_FRONT)
		searchAction(1);
	else
		searchWord();
}

void CElwDlg::searchAction(int column)
{
	CString	string; m_edtSearch.GetWindowText(string);

	string.MakeUpper();
	if (column == 0)	// number search
	{
		if (!m_bAddAll)
		{
			CString		code, name;
			struct JCode*	jcode;

			m_listCtrl.SetRedraw(FALSE);
			clearListCtrl();
			for (int ii = 0; ii < m_arSelCode.GetSize(); ii++)
			{
				jcode = (struct JCode*)m_arSelCode.GetAt(ii);

				name = CString(jcode->Name, jcode->Name.GetLength());
				//if (jcode->ssgb != jmELW)	name = name.Mid(1);

				code = jcode->Code;
				name.TrimRight();
				addItem(ii, code, name);
			}
			m_bAddAll = true;
			m_listCtrl.SetRedraw(TRUE);
		}

		if (m_listCtrl.GetItemCount() > 0)
		{
			int	idx = -1;
			if (string.GetLength() > 0)
			{
				m_gListsort = FALSE;
				ListView_SortItems(m_listCtrl.GetSafeHwnd(), ElwListViewCompareFunc, column);

				idx = findMatchIndex(string, column);
				if (idx >= 0) 
				{
					m_listCtrl.Selected(idx);
					m_szCode = m_listCtrl.GetItemText(idx, 0);
					m_szName = m_listCtrl.GetItemText(idx, 1);
					m_szName.TrimLeft();
				}
			}
			else
			{
				m_listCtrl.Selected(0);
				m_szCode = m_listCtrl.GetItemText(0, 0);
				m_szName = m_listCtrl.GetItemText(0, 1);
				m_szName.TrimLeft();
			}
		}
	}
	else			// name search
	{
		m_bAddAll = SearchData(string, m_bAddAll);
		if (m_listCtrl.GetItemCount() > 0)
		{
			m_gListsort = FALSE;
			ListView_SortItems(m_listCtrl.GetSafeHwnd(), ElwListViewCompareFunc, column);
			m_listCtrl.Selected(0);
			m_szCode = m_listCtrl.GetItemText(0, 0);
			m_szName = m_listCtrl.GetItemText(0, 1);
			m_szName.TrimLeft();
		}
	}
}

void CElwDlg::searchWord()
{
	CString		word;
	CString		code, name;
	struct JCode*	jcode;

	int idx = 0;
	m_edtSearch.GetWindowText(word);
	word.TrimRight();
	word.MakeUpper();

	if (word.IsEmpty())
	{
		if (m_listCtrl.GetItemCount() != m_arSelCode.GetSize())
		{
			CString		code, name;
			struct JCode*	jcode;

			m_listCtrl.SetRedraw(FALSE);
			clearListCtrl();
			for (int ii = 0; ii < m_arSelCode.GetSize(); ii++)
			{
				jcode = (struct JCode*)m_arSelCode.GetAt(ii);

				name = CString(jcode->Name, jcode->Name.GetLength());
				//if (jcode->ssgb != jmELW)	name = name.Mid(1);

				code = jcode->Code;
				name.TrimRight();
				addItem(ii, code, name);
			}
			m_bAddAll = true;

			if (m_listCtrl.GetItemCount() > 0)
			{
				m_gListsort = FALSE;
				ListView_SortItems(m_listCtrl.GetSafeHwnd(), ElwListViewCompareFunc, 1);
				m_listCtrl.Selected(0);
				m_szCode = m_listCtrl.GetItemText(0, 0);
				m_szName = m_listCtrl.GetItemText(0, 1);
				m_szName.TrimLeft();
			}
			m_listCtrl.SetRedraw(TRUE);
		}
	}
	else
	{
		m_listCtrl.SetRedraw(FALSE);
		clearListCtrl();
		for (int ii = 0; ii < m_arSelCode.GetSize(); ii++)
		{
			jcode = (struct JCode*)m_arSelCode.GetAt(ii);

			name = CString(jcode->Name, jcode->Name.GetLength());
			//if (jcode->ssgb != jmELW)	name = name.Mid(1);

			if (name.Find(word, 0) != -1)
			{
				code = jcode->Code;
				name.TrimRight();
				addItem(idx, code, name);
				idx++;
			}
		}
		m_listCtrl.SetRedraw(TRUE);
	}
}

int CElwDlg::findMatchIndex(CString text, int column)
{
	CString	string;
	int	textL = text.GetLength();
	for (int ii = 0; ii < m_listCtrl.GetItemCount(); ii++)
	{
		string = m_listCtrl.GetItemText(ii, column);
		if (column == 1) string = string.Mid(1);

		if (!strncmp(text, string, textL))
			return ii;
	}
	return -1;
}

bool CElwDlg::SearchData(CString sName, bool bAddAll)
{
	CString	code, name;
	struct JCode*	jcode;

	if (sName.IsEmpty())
	{	// add all
		if (!bAddAll)
		{
			m_listCtrl.SetRedraw(FALSE);
			clearListCtrl();
			for (int ii = 0; ii < m_arSelCode.GetSize(); ii++)
			{
				jcode = (struct JCode*)m_arSelCode.GetAt(ii);
				name = CString(jcode->Name, jcode->Name.GetLength());
				//if (jcode->ssgb != jmELW)	name = name.Mid(1);

				code = jcode->Code;
				name.TrimRight();
				addItem(ii, code, name);
			}
			m_listCtrl.SetRedraw(TRUE);
		}
		return true;
	}

	m_listCtrl.SetRedraw(FALSE);
	clearListCtrl();

	int	idx = 0;
	char	cName;
	int	sLen = sName.GetLength();
	if (sLen == 1)
	{	// 일반검색적용(길이만큼 맞는것만 적용)
		int	sLen = sName.GetLength();
		for (int ii = 0; ii < m_arSelCode.GetSize(); ii++)
		{
			jcode = (struct JCode*)m_arSelCode.GetAt(ii);
			name = CString(jcode->Name, jcode->Name.GetLength());
			//if (jcode->ssgb != jmELW)	name = name.Mid(1);
			if (strncmp(sName, name, sLen) != 0)
				continue;

			code = jcode->Code;
			name.TrimRight();
			addItem(idx++, code, name);
		}
	}
	else
	{	// 한글 검색
		WORD	wHangul, wHangul2, wStart, wEnd;
		CPtrArray	arSearch;
		arSearch.RemoveAll();
		int ii = 0;

		cName = sName.GetAt(ii);
		if (cName & HANGULMSK)	// 한글일땐 2Byte
		{
			wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
			if (IsChosung(wHangul, wStart, wEnd))
			{	// 초성 비교
				for (int jj = 0; jj < m_arSelCode.GetSize(); jj++)
				{
					jcode = (struct JCode*)m_arSelCode.GetAt(jj);
					name = CString(jcode->Name, jcode->Name.GetLength());
					//if (jcode->ssgb != jmELW)		name = name.Mid(1);

					if (name.GetLength() < 2)	continue;
					wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
					if (wStart <= wHangul2 && wHangul2 <= wEnd)	arSearch.Add(jcode);
				}
			}
			else
			{	// 한글 비교

				for (int jj = 0; jj < m_arSelCode.GetSize(); jj++)
				{
					jcode = (struct JCode*)m_arSelCode.GetAt(jj);
					name = CString(jcode->Name, jcode->Name.GetLength());
					//if (jcode->ssgb != jmELW)		name = name.Mid(1);

					if (name.GetLength() < 2)	continue;
					wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
					if (wHangul == wHangul2)	arSearch.Add(jcode);
				}
			}
			ii += 2;
		}
		else			// 그외는 1Byte
		{
			for (int jj = 0; jj < m_arSelCode.GetSize(); jj++)
			{
				jcode = (struct JCode*)m_arSelCode.GetAt(jj);
				name = CString(jcode->Name, jcode->Name.GetLength());
				//if (jcode->ssgb != jmELW)		name = name.Mid(1);

				if (name.GetLength() < 1)	continue;
				if (cName == name.GetAt(ii))	arSearch.Add(jcode);
			}
			ii += 1;
		}

		for (; ii < sLen && arSearch.GetSize();)
		{
			cName = sName.GetAt(ii);
			if (cName & HANGULMSK)	// 한글일땐 2Byte
			{
				wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
				if (IsChosung(wHangul, wStart, wEnd))
				{	// 초성 비교
					for (int jj = (int)arSearch.GetUpperBound(); jj >= 0; jj--)
					{
						jcode = (struct JCode*)arSearch.GetAt(jj);
						name = CString(jcode->Name, jcode->Name.GetLength());
						//if (jcode->ssgb != jmELW)	name = name.Mid(1);

						if (name.GetLength() < ii+2)
						{
							arSearch.RemoveAt(jj);
							continue;
						}
						wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
						if (wStart > wHangul2 || wHangul2 > wEnd)	arSearch.RemoveAt(jj);
					}
				}
				else
				{	// 한글 비교
					for (int jj = (int)arSearch.GetUpperBound(); jj >= 0; jj--)
					{
						jcode = (struct JCode*)arSearch.GetAt(jj);
						name = CString(jcode->Name, jcode->Name.GetLength());
						//if (jcode->ssgb != jmELW)	name = name.Mid(1);

						if (name.GetLength() < ii+2)
						{
							arSearch.RemoveAt(jj);
							continue;
						}
						wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
						if (wHangul != wHangul2)
							arSearch.RemoveAt(jj);
					}
				}
				ii += 2;
			}
			else			// 그외는 1Byte
			{
				for (int jj = (int)arSearch.GetUpperBound(); jj >= 0; jj--)
				{
					jcode = (struct JCode*)arSearch.GetAt(jj);
					name = CString(jcode->Name, jcode->Name.GetLength());
					//if (jcode->ssgb != jmELW)	name = name.Mid(1);

					if (name.GetLength() < ii+1)
					{
						arSearch.RemoveAt(jj);
						continue;
					}
					if (cName != name.GetAt(ii))	arSearch.RemoveAt(jj);
				}
				ii += 1;
			}
		}

		for (ii = 0; ii < arSearch.GetSize(); ii++)
		{
			jcode = (struct JCode*)arSearch.GetAt(ii);
			name = CString(jcode->Name, jcode->Name.GetLength());
			//if (jcode->ssgb != jmELW)	name = name.Mid(1);

			code = jcode->Code;
			name.TrimRight();
			addItem(idx++, code, name);
		}
	}
	m_listCtrl.SetRedraw(TRUE);

	return false;
}

bool CElwDlg::IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd)
{	// 초성만 가진 한글인지~~
	if (ChosungS > wHangul || wHangul > ChosungE)
		return false;

	WORD	pChosung[19] = 
	{
		0xA4A1, 0xA4A2, 0xA4A4, 0xA4A7, 0xA4A8, 0xA4A9, 0xA4B1,	// ㄱ,ㄲ,ㄴ,ㄷ,ㄸ,ㄹ,ㅁ
		0xA4B2, 0xA4B3, 0xA4B5, 0xA4B6, 0xA4B7, 0xA4B8, 0xA4B9,	// ㅂ,ㅃ,ㅅ,ㅆ,ㅇ,ㅈ,ㅉ
		0xA4BA, 0xA4BB, 0xA4BC, 0xA4BD, 0xA4BE			// ㅊ,ㅋ,ㅌ,ㅍ,ㅎ
	};

	WORD	pHangulStart[19] = 
	{
		0xB0A1, 0xB1EE, 0xB3AA, 0xB4D9, 0xB5FB, 0xB6F3, 0xB8B6,	// 가,까,나,다,따,라,마
		0xB9D9, 0xBAFC, 0xBBE7, 0xBDCE, 0xBEC6, 0xC0DA, 0xC2A5,	// 바,빠,사,싸,아,자,짜
		0xC2F7, 0xC4AB, 0xC5B8, 0xC6C4, 0xC7CF			// 차,카,타,파,하
	};
	WORD	pHangulEnd[19] = 
	{
			0xB1ED, 0xB3A9, 0xB4D8, 0xB5FA, 0xB6F2, 0xB8B5,		
		0xB9D8, 0xBAFB, 0xBBE6, 0xBDCD, 0xBEC5, 0xC0D9, 0xC2A4,	
		0xC2F6, 0xC4AA, 0xC5B7, 0xC6C3, 0xC7CE, 0xC8FE			
	};

	INT_PTR ii = 0;
	for (ii = 0; ii < 19; ii++)
	{
		if (wHangul == pChosung[ii])
		{
			wStart = pHangulStart[ii];
			wEnd = pHangulEnd[ii];
			return true;
		}			
	}

	wStart = pHangulStart[ii] - 1;
	wEnd = pHangulEnd[ii] + 1;

	return true;
}

WORD CElwDlg::MakeHangul(char cChar1, char cChar2)
{	// 2Byte문자로부터 한글을 맹글어주라~~
	WORD	wHangul = (cChar1<<8) | (0x00FF & cChar2);
	return wHangul;
}
