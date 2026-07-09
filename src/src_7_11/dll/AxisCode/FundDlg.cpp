// FundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "FundDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


bool	m_bsort;
/////////////////////////////////////////////////////////////////////////////
// CFundDlg dialog


CFundDlg::CFundDlg(CPoint pt, CWnd* pParent /*=NULL*/)
	: CDialog(CFundDlg::IDD, pParent)
{
	m_Point = pt;

	m_bsort = true;

	m_sRoot = ((CAxisCodeApp*)AfxGetApp())->m_RootPath;

	m_rtnData.code = _T("");
	m_rtnData.name = _T("");
	
	//{{AFX_DATA_INIT(CFundDlg)
	//}}AFX_DATA_INIT
}

void CFundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFundDlg)
	DDX_Control(pDX, IDC_BTN_FSORT16, m_btn_sort16);
	DDX_Control(pDX, IDC_BTN_FSORT15, m_btn_sort15);
	DDX_Control(pDX, IDC_BTN_FSORT14, m_btn_sort14);
	DDX_Control(pDX, IDC_BTN_FSORT13, m_btn_sort13);
	DDX_Control(pDX, IDC_BTN_FSORT12, m_btn_sort12);
	DDX_Control(pDX, IDC_BTN_FSORT11, m_btn_sort11);
	DDX_Control(pDX, IDC_BTN_FSORT10, m_btn_sort10);
	DDX_Control(pDX, IDC_BTN_FSORT09, m_btn_sort09);
	DDX_Control(pDX, IDC_BTN_FSORT08, m_btn_sort08);
	DDX_Control(pDX, IDC_BTN_FSORT07, m_btn_sort07);
	DDX_Control(pDX, IDC_BTN_FSORT06, m_btn_sort06);
	DDX_Control(pDX, IDC_BTN_FSORT05, m_btn_sort05);
	DDX_Control(pDX, IDC_BTN_FSORT04, m_btn_sort04);
	DDX_Control(pDX, IDC_BTN_FSORT03, m_btn_sort03);
	DDX_Control(pDX, IDC_BTN_FSORT02, m_btn_sort02);
	DDX_Control(pDX, IDC_BTN_FSORT01, m_btn_sort01);
	DDX_Control(pDX, IDC_BTN_BMX, m_btn_bmx);
	DDX_Control(pDX, IDC_BTN_AFUND, m_btn_all);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_edit_search);
	DDX_Control(pDX, IDC_BTN_STK, m_btn_stk);
	DDX_Control(pDX, IDC_BTN_SMX, m_btn_smx);
	DDX_Control(pDX, IDC_BTN_SFUND, m_btn_sfund);
	DDX_Control(pDX, IDC_BTN_MMF, m_btn_mmf);
	DDX_Control(pDX, IDC_BTN_BOND, m_btn_bond);
	DDX_Control(pDX, IDC_LIST_CODE, m_ListCtlCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFundDlg, CDialog)
	//{{AFX_MSG_MAP(CFundDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_SEARCH, OnChangeEditSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CODE, OnDblclkListCode)
//	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_BTN_AFUND, IDC_BTN_SFUND, OnClickBtnKind)
	ON_COMMAND_RANGE(IDC_BTN_FSORT01, IDC_BTN_FSORT16, OnClickBtnSort)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFundDlg message handlers

int CALLBACK ListViewCompareFunc4( LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{ 
	struct _ListSort { 
		char*	Code[FD_LEN_CODE];
		char*	Name[FD_LEN_NAME];
	};

	_ListSort *pItem1 = (_ListSort *) lParam1; 
	_ListSort *pItem2 = (_ListSort *) lParam2; 

	int	iCmp=0;

	switch ((int)lParamSort)
	{
	case 0:
		iCmp = lstrcmpi((char*)pItem1->Code, (char*)pItem2->Code); 
		break;
	case 1:
		iCmp = lstrcmpi((char*)pItem1->Name, (char*)pItem2->Name);
		break;
/*	case 2:
		iCmp = lstrcmpi((char*)pItem1->CName, (char*)pItem2->CName);
		break;
*/	}

	if (m_bsort)
	{
		if (iCmp > 0)
			iCmp = -1;
		else if (iCmp < 0)
			iCmp = 1;
	}

	return iCmp;
} 

BOOL CFundDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CtrlFont.DeleteObject();
	m_CtrlFont.CreatePointFontIndirect(&SetFont(9));

	InitCtrl();

	SetBtn();
	OnClickBtnKind(IDC_BTN_AFUND);
	SetDlgPos(m_Point.x, m_Point.y);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFundDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CFundDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	ClearListCtlCode();

	m_CtrlFont.DeleteObject();
}

void CFundDlg::OnChangeEditSearch() 
{
	CString str;
	m_edit_search.GetWindowText(str);

	m_bsort = FALSE;
	ListView_SortItems(m_ListCtlCode, ListViewCompareFunc4,1);
	setSelected(&m_ListCtlCode, findIndex(str, "", str.GetLength()));	
}

void CFundDlg::OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int	nItem;
	CString	code, name;
	POSITION pos = m_ListCtlCode.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			nItem = m_ListCtlCode.GetNextSelectedItem(pos);

			code = m_ListCtlCode.GetItemText(nItem, 0);
			name = m_ListCtlCode.GetItemText(nItem, 1);

			m_rtnData.code = code;
			m_rtnData.name = name;
		}
	}

	OnOK();
}

BOOL CFundDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	switch(wParam)
	{
	case IDC_LIST_CODE:
		{
			switch(((LPNMHDR) lParam)->code)
			{
			case LVN_COLUMNCLICK:
				#define pnm ((LPNMLISTVIEW)lParam)
				switch ((LPARAM)pnm->iSubItem)
				{
				case 0:
					m_bsort = !m_bsort;
					break;
				case 1:
					m_bsort = !m_bsort;
					break;
				}
				ListView_SortItems(pnm->hdr.hwndFrom, ListViewCompareFunc4, (LPARAM)pnm->iSubItem);
				#undef pnm
				break;
			}			
		}
		break;
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CFundDlg::OnClickBtnKind(UINT nID)
{
	for (int ii = IDC_BTN_AFUND; ii <= IDC_BTN_SFUND ; ii++)
	{
		if ((int)nID == ii)
		{
			((CfxButton*)GetDlgItem(ii))->SetBold(TRUE);
			((CfxButton*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, USER_FOCUS_COR);
			((CfxButton*)GetDlgItem(ii))->SetSelected(TRUE);
		}
		else
		{
			((CfxButton*)GetDlgItem(ii))->SetBold(FALSE);
			((CfxButton*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
			((CfxButton*)GetDlgItem(ii))->SetSelected(FALSE);
		}
	}
	
	SetCodeList(nID);
}

void CFundDlg::OnClickBtnSort(UINT nID)
{
	struct _sortInfo {
		char*	str;
		char*	nxstr;
		int	len;
	} sortInfo[] = {
		{"가", "나", 2}, {"나", "다", 2}, {"다", "라", 2}, {"라", "마", 2}, {"마", "바", 2},
		{"바", "사", 2}, {"사", "아", 2}, {"아", "자", 2}, {"자", "차", 2}, {"차", "카", 2},
		{"카", "타", 2}, {"타", "파", 2}, {"파", "하", 2}, {"하", "희희희", 2}, {"A", "Z", 1}, {"1", "0", 1}
	};

	int bSel = nID - IDC_BTN_FSORT01;

	m_bsort = FALSE;
	ListView_SortItems(m_ListCtlCode, ListViewCompareFunc4, 1);
	setSelected(&m_ListCtlCode, findIndex(sortInfo[bSel].str, sortInfo[bSel].nxstr, sortInfo[bSel].len));
}

void CFundDlg::SetDlgPos(int x, int y)
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

CString CFundDlg::GetRtnData(bool dialog/* = true*/, CString code/* = _T("")*/)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	CString rValue = _T("");

	if (dialog)
	{
		if (!m_rtnData.code.IsEmpty() && !m_rtnData.name.IsEmpty())
			rValue.Format("%s\t%s", m_rtnData.code, m_rtnData.name);
	}
	else
	{
		CString	tcode, name;
		int	nSize = (int)pApp->m_arrayFDCode.GetSize();
		for (int ii = 0 ; ii < nSize ; ii++)
		{
			tcode = CString(pApp->m_arrayFDCode[ii]->code, sizeof(pApp->m_arrayFDCode[ii]->code));
			if (code.Compare(tcode) == 0)
			{
				name = CString(pApp->m_arrayFDCode[ii]->hnam, sizeof(pApp->m_arrayFDCode[ii]->hnam));
				rValue.Format("%s\t%s", code, name);
			}
		}
	}
	
	return rValue;
}

void CFundDlg::AddCodeNameArray(_FDCode* fdCode)
{
	_CodeName	CodeName;
	
	CodeName.code = CString(fdCode->code, sizeof(fdCode->code));
	CodeName.name = CString(fdCode->hnam, sizeof(fdCode->hnam));

	CodeName.code.TrimRight(); CodeName.code.TrimRight();
	CodeName.name.TrimRight(); CodeName.name.TrimRight();

	m_CodeNameArray.Add(CodeName);
}

void CFundDlg::SetCodeList(int kind)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int	gubn = 0;
	switch (kind)
	{
	case IDC_BTN_BOND:	gubn = '1';	break;
	case IDC_BTN_BMX:	gubn = '2';	break;
	case IDC_BTN_STK:	gubn = '3';	break;
	case IDC_BTN_SMX:	gubn = '4';	break;
	case IDC_BTN_MMF:	gubn = '5';	break;
	case IDC_BTN_SFUND:	gubn = '6';	break;
	default:
		{
			m_CodeNameArray.RemoveAll();
			for (int ii = 0; ii < pApp->m_arrayFDCode.GetSize(); ii++)
			{
				AddCodeNameArray(pApp->m_arrayFDCode[ii]);
			}
			ClearListCtlCode();
			for (int ii = 0 ; ii < m_CodeNameArray.GetSize() ; ii++)
				AddListCtlCode(ii, m_CodeNameArray[ii].code, m_CodeNameArray[ii].name);
		}

		return;
	}

	m_CodeNameArray.RemoveAll();
	for (int ii = 0; ii < pApp->m_arrayFDCode.GetSize(); ii++)
	{
		if (pApp->m_arrayFDCode[ii]->gubn[0] == gubn)
			AddCodeNameArray(pApp->m_arrayFDCode[ii]);
	}

	ClearListCtlCode();
	for (int ii = 0 ; ii < m_CodeNameArray.GetSize() ; ii++)
		AddListCtlCode(ii, m_CodeNameArray[ii].code, m_CodeNameArray[ii].name);

}

int CFundDlg::findIndex(CString text, CString nxText, int len, int column/* = 1*/)
{
	CString	strTmp;
	CRect	rc;
	CSize	size;

	for (int ii = 0 ; ii < m_ListCtlCode.GetItemCount() ; ii++)
	{
		strTmp = m_ListCtlCode.GetItemText(ii, column);
		if (nxText.IsEmpty())
		{
			if (strncmp(text, strTmp, len) == 0)
			{
				killSelected(&m_ListCtlCode);
				return ii;
			}
		}
		else if (text.Compare(strTmp) <= 0)
		{
			if (nxText.Compare(strTmp) > 0)
			{
				killSelected(&m_ListCtlCode);
				return ii;
			}
		}
	}

	return -1;
}

void CFundDlg::killSelected(CListCtrl* ListCtrl)
{
	POSITION pos = ListCtrl->GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			int	nItem;
			nItem = ListCtrl->GetNextSelectedItem(pos);
			ListCtrl->SetItemState(nItem, NULL, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

void CFundDlg::setSelected(CListCtrl* ListCtrl, int nItem, BOOL scroll/* = TRUE*/)
{
	if (nItem < 0)
	{
		m_rtnData.code = _T("");
		m_rtnData.name = _T("");
		return;
	}

	CRect	rc;
	CSize	size;

	ListCtrl->SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	if (scroll)
	{
		ListCtrl->GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - ListCtrl->GetTopIndex());
		ListCtrl->Scroll(size);
	}

	CString code = m_ListCtlCode.GetItemText(nItem, 0);
	CString	name = m_ListCtlCode.GetItemText(nItem, 1);

	m_rtnData.code = code;
	m_rtnData.name = name;
}

void CFundDlg::SetBtn()
{
	short	shBtnColor = 245;
	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	CString strImg("");
	strImg.Format("%s/%s/", pApp->m_RootPath, "image");

	for (int ii = IDC_BTN_FSORT01 ; ii <= IDC_BTN_SFUND ; ii++)
	{
		//((CButtonST*)GetDlgItem(ii))->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
		//((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
		//((CButtonST*)GetDlgItem(ii))->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
		//((CButtonST*)GetDlgItem(ii))->SetWnd(this);
		((CfxButton*)GetDlgItem(ii))->SetBtnCursor(IDC_CURSOR_HAND);
		((CfxButton*)GetDlgItem(ii))->LoadImg(strImg);
	}
}

void CFundDlg::ClearListCtlCode(int nItem/* = -1*/)
{
	_ListSort*	pListSort = NULL;
	if (nItem < 0)
	{
		int nCount = m_ListCtlCode.GetItemCount();
		for (int ii = 0 ; ii < nCount ; ii++)
		{
			pListSort = (_ListSort*) m_ListCtlCode.GetItemData(ii);
			SAFE_DELETE(pListSort);
		}
		m_ListCtlCode.DeleteAllItems();
	}
	else
	{
		pListSort = (_ListSort*) m_ListCtlCode.GetItemData(nItem);
		SAFE_DELETE(pListSort);
		m_ListCtlCode.DeleteItem(nItem);
	}
}

void CFundDlg::AddListCtlCode(int nItem, CString szCode, CString szName)
{
	szCode.TrimLeft(); szCode.TrimRight(); szName.TrimLeft(); szName.TrimRight();

	_ListSort* pListSort = new _ListSort;
	ZeroMemory(pListSort, sizeof(_ListSort));

	strcpy((char*)pListSort->Code, szCode);
	strcpy((char*)pListSort->Name, szName);
	m_listSort.Add(pListSort);

	int rItem;
	rItem = m_ListCtlCode.InsertItem(LVIF_TEXT|LVIF_PARAM, nItem, szCode
		, NULL, NULL, 0, (LPARAM) pListSort);

	m_ListCtlCode.SetItemText(rItem, 1, szName);
}



void CFundDlg::InitCtrl()
{
	CRect	rc;
	m_ListCtlCode.GetClientRect(&rc);

	m_ListCtlCode.SetFont(&m_CtrlFont);
	m_ListCtlCode.SetBkColor(COR_WHITE);
	m_ListCtlCode.SetTextBkColor(COR_WHITE);
	m_ListCtlCode.SetTextColor(RGB(0,0,0));
	m_ListCtlCode.InsertColumn(1, "펀드코드", LVCFMT_CENTER, 60);
	m_ListCtlCode.InsertColumn(2, "종목명", LVCFMT_CENTER, rc.Width() - 80);
}

LOGFONT CFundDlg::SetFont(int Size)
{
	LOGFONT		Font;

	ZeroMemory(&Font, sizeof(LOGFONT));
	Font.lfHeight		= Size * 10;
	Font.lfCharSet		= DEFAULT_CHARSET;
	Font.lfWeight		= FW_NORMAL;
	Font.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	Font.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	Font.lfQuality		= DEFAULT_QUALITY;
	Font.lfPitchAndFamily	= DEFAULT_PITCH; 
	Font.lfItalic		= false;
	strcpy(Font.lfFaceName, "굴림");

	return Font;
}

BOOL CFundDlg::FindCode(int kind, CString szCode)		// GetName()에서 사용. 
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	_FDCode	*fdcode;
	m_rtnData.code = m_rtnData.name = _T("");

	szCode.TrimRight();
	if (pApp->m_arrayFDCode.GetSize() <= 0)
	{
		return FALSE;
	}

	for(int ii = 0; ii < pApp->m_arrayFDCode.GetSize(); ii++)
	{
		fdcode = pApp->m_arrayFDCode[ii];
		
		if (!szCode.CompareNoCase(CString(fdcode->code, sizeof(fdcode->code))))
		{
			m_rtnData.code = CString(fdcode->code, sizeof(fdcode->code));
			m_rtnData.name = CString(fdcode->hnam, sizeof(fdcode->hnam));
			return TRUE;
		}
	}

	return FALSE;
}

CString CFundDlg::GetCode()
{
	return m_rtnData.code;
}

CString CFundDlg::GetName()
{
	return m_rtnData.name;
}
/*
HBRUSH CFundDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
