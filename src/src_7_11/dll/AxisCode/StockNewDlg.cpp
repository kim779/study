// StockNewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "StockNewDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStockNewDlg dialog


CStockNewDlg::CStockNewDlg(CPoint pt, int nType, CWnd* pParent /*=NULL*/)
	: CDialog(CStockNewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStockNewDlg)
	//}}AFX_DATA_INIT

	m_arrayNCode.RemoveAll();
	m_ListCtrl.SetHeaderUse();

	m_Point = pt;
	m_nType = nType;
	m_nStep = m_nType;


	// nType => 1:신주,수익만선택, 2:ELW만선택
	// nStep => 0:전체, 2:ELW, 3:신주, 4:수익
}


void CStockNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStockNewDlg)
	DDX_Control(pDX, IDC_SUIK_BTN, m_suikBtn);
	DDX_Control(pDX, IDC_NEWSTOCK_BTN, m_newStockBtn);
	DDX_Control(pDX, IDC_ELW_BTN, m_elwBtn);
	DDX_Control(pDX, IDC_ALLCODE_BTN, m_allCodeBtn);
	DDX_Control(pDX, IDC_LIST_NEW, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStockNewDlg, CDialog)
	//{{AFX_MSG_MAP(CStockNewDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_NEW, OnDblclkListNew)
	ON_WM_PAINT()
//	ON_WM_CTLCOLOR()
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_ALLCODE_BTN, OnAllcodeBtn)
	ON_BN_CLICKED(IDC_ELW_BTN, OnElwBtn)
	ON_BN_CLICKED(IDC_NEWSTOCK_BTN, OnNewstockBtn)
	ON_BN_CLICKED(IDC_SUIK_BTN, OnSuikBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStockNewDlg message handlers

BOOL CStockNewDlg::loadJCode()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int	codeN;

	struct  hjcode*  HJCode;
	NCODE	ncode;

	if (m_arrayNCode.GetSize() > 0)		
		return FALSE;
	codeN = (int)pApp->m_arrayHJCode.GetSize();

	for (int ii = 0; ii < codeN; ii++)
	{
		bool bSet = false;
		HJCode = pApp->m_arrayHJCode[ii];

//		if (m_nStep == 0 && (HJCode->ssgb == jmSINJU || HJCode->ssgb == jmSINJS || HJCode->ssgb == jmELW))
//			bSet = true;
		if (m_nStep == 0 && (HJCode->ssgb == jmSINJU || HJCode->ssgb == jmSINJS))
			bSet = true;

		if ((m_nStep == 1 || m_nStep == 3 || m_nStep == 4) && (HJCode->ssgb == jmSINJU || HJCode->ssgb == jmSINJS))
			bSet = true;

//		if (m_nStep == 2 && HJCode->ssgb == jmELW)
//			bSet = true;

		if (bSet)
		{
			ncode.name = CString(HJCode->hnam, HNameLen);
			ncode.code = CString(HJCode->code, HCodeLen);
			ncode.fill.Format("%c", HJCode->k200);

			switch (HJCode->ssgb)
			{
			case jmSINJU:
				ncode.kind = "증권";		break;
//			case jmELW:
//				ncode.kind = "ELW";		break;
			default:
				ncode.kind = "증서";		break;
			}

			if (ncode.code.GetLength() && ncode.code.GetAt(0) == 'F')
			{
				if (m_nStep == 2 || m_nStep == 3)
					continue;
				ncode.kind = "수익증권";
			}
			else if (HJCode->ssgb == jmSINJU && m_nStep == 4)
				continue;

			m_arrayNCode.Add(ncode);
		}
	}

	return TRUE;
}

void CStockNewDlg::AddItem(int row, CString szCode, CString szName, CString szTcode, CString kind)
{
	m_ListCtrl.InsertItem(row, "");
	m_ListCtrl.SetItemText(row, 0, szCode);
	m_ListCtrl.SetItemText(row, 1, szName);
	m_ListCtrl.SetItemText(row, 2, szTcode);
	m_ListCtrl.SetItemText(row, 3, kind);
}

void CStockNewDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	EndDialog(1);
}

BOOL CStockNewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetDlgPos(m_Point.x, m_Point.y);

	loadJCode();
	SetButton();

	m_ListCtrl.SetBkColor(RGB(237,237,237));
	m_ListCtrl.SetTextBkColor(RGB(237,237,237));
	m_ListCtrl.SetTextColor(RGB(0,0,0));

	m_ListCtrl.InsertColumn(1, "종목코드", LVCFMT_LEFT, 70);
	m_ListCtrl.InsertColumn(2, "한글종목명", LVCFMT_LEFT, 140);
	m_ListCtrl.InsertColumn(3, "표준코드", LVCFMT_LEFT, 0);
	m_ListCtrl.InsertColumn(4, "구분", LVCFMT_LEFT, 0);

	CountData();

	if (m_nType == 1)
	{
		m_ListCtrl.SetFocus();
		return FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStockNewDlg::SetDlgPos(int x, int y)
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

BOOL CStockNewDlg::CountData()
{
	NCODE ncode;
	int ct = (int)m_arrayNCode.GetSize();
	CString tcode;
	
	m_ListCtrl.DeleteAllItems();

	if (ct <= 0)	return FALSE;

	for (int ii = 0; ii < ct ; ii++)
	{
		ncode = m_arrayNCode[ii];
		tcode = ncode.code.Mid(1);
		tcode = "KRA" + tcode + ncode.fill;
		AddItem(ii, ncode.code, ncode.name, tcode, ncode.kind);
	}
	
	if (m_ListCtrl.GetItemCount() > 18)
		m_ListCtrl.SetColumnWidth(4, 40);

	return TRUE;
}

void CStockNewDlg::OnDblclkListNew(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int item = m_ListCtrl.GetSelectionMark();

		m_szCode = m_ListCtrl.GetItemText(item, 0);
		m_szName = m_ListCtrl.GetItemText(item, 1);

		OnOK();
		
	*pResult = 0;
}

void CStockNewDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	/*CRect rc(6, 8, 361, 247);

	m_Tool.SetDC(&dc);

	m_Tool.DrawCell(rc);*/
}

/*
HBRUSH CStockNewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CStockNewDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
}

CString CStockNewDlg::GetName(char* code)
{
	CString rValue = _T("");

	for (int ii = 0 ; ii < m_arrayNCode.GetSize() ; ii++)
	{
		if (m_arrayNCode[ii].code.Compare(code) == 0)
		{
			rValue = m_arrayNCode[ii].name;
			break;
		}
	}

	return rValue;
}

void CStockNewDlg::OnAllcodeBtn() 
{
	m_nStep = 0;
	m_arrayNCode.RemoveAll();
	loadJCode();
	CountData();
}

void CStockNewDlg::OnElwBtn() 
{
	m_nStep = 2;
	m_arrayNCode.RemoveAll();
	loadJCode();
	CountData();
}

void CStockNewDlg::OnNewstockBtn() 
{
	m_nStep = 3;
	m_arrayNCode.RemoveAll();
	loadJCode();
	CountData();
}

void CStockNewDlg::OnSuikBtn() 
{
	m_nStep = 4;
	m_arrayNCode.RemoveAll();
	loadJCode();
	CountData();
}

void CStockNewDlg::SetButton(CButtonST *pButton)
{
	short	shBtnColor = 245;
	pButton->SetFlat(FALSE);
	pButton->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	pButton->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
	pButton->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
	pButton->SetBtnCursor(IDC_CURSOR_HAND);
	pButton->SetBold(FALSE);
}

void CStockNewDlg::SetButton()
{
	//SetButton(&m_allCodeBtn);
	//SetButton(&m_elwBtn);
	//SetButton(&m_newStockBtn);
	//SetButton(&m_suikBtn);
	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	CString strImg("");
	strImg.Format("%s/%s/", pApp->m_RootPath, "image");

	m_allCodeBtn.LoadImg(strImg);
	m_elwBtn.LoadImg(strImg);
	m_newStockBtn.LoadImg(strImg);
	m_suikBtn.LoadImg(strImg);

	switch (m_nType)
	{
	case 1:		// 수익,신주 선택, 전체선택안함
		m_allCodeBtn.EnableWindow(FALSE);
		m_elwBtn.EnableWindow(FALSE);
		break;
	case 2:		// ELW 선택, 전체 선택안함
		m_allCodeBtn.EnableWindow(FALSE);
		m_newStockBtn.EnableWindow(FALSE);
		m_suikBtn.EnableWindow(FALSE);
		break;
	}
}
