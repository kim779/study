// FuturesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AxisCode.h"
#include "FuturesDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFuturesDlg dialog

CFuturesDlg::CFuturesDlg(int kind, int type, CPoint pt,CWnd* pParent /*=NULL*/)
	: CDialog(CFuturesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFuturesDlg)
	m_szEditCode = _T("");
	m_szEditName = _T("");
	m_kindList = 0;
	m_nGubn = -1;
	//}}AFX_DATA_INIT

	m_pCheck = NULL;		
	
	m_arraycall.RemoveAll();
	m_arrayput.RemoveAll();
	m_ListPcode.RemoveAll();

	m_Kind    = kind;
	m_Type    = type;
	m_Point   = pt;
	m_nStep   = futureCODE;
	m_bKostar = FALSE;
	m_Spread  = 0;
	m_bSort   = TRUE;
	m_nCountMonth = 0;
}	


void CFuturesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFuturesDlg)
	DDX_Control(pDX, IDC_BUTTON_JFUTURE, m_ctlButtonJFuture);
	DDX_Control(pDX, IDC_LIST_CODE, m_codeList);
	DDX_Control(pDX, IDC_BUTTON_OPMONTH4, m_ctlButtonOPMonth4);
	DDX_Control(pDX, IDC_BUTTON_OPMONTH3, m_ctlButtonOPMonth3);
	DDX_Control(pDX, IDC_BUTTON_OPMONTH2, m_ctlButtonOPMonth2);
	DDX_Control(pDX, IDC_BUTTON_OPMONTH1, m_ctlButtonOPMonth1);
	DDX_Control(pDX, IDC_BUTTON_KOSTAR, m_ctlButtonKostar);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH9, m_ctlButtonSPMonth9);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH6, m_ctlButtonSPMonth6);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH3, m_ctlButtonSPMonth3);
	DDX_Control(pDX, IDC_PCODE_LIST, m_plistbox);
	DDX_Control(pDX, IDC_EDIT_NAME, m_ctlEditName);
	DDX_Control(pDX, IDC_EDIT_CODE, m_ctlEditCode);
	DDX_Control(pDX, IDC_BUTTON_MONTH12, m_ctlButtonMonth12);
	DDX_Control(pDX, IDC_BUTTON_MONTH3, m_ctlButtonMonth3);
	DDX_Control(pDX, IDC_BUTTON_MONTH6, m_ctlButtonMonth6);
	DDX_Control(pDX, IDC_BUTTON_MONTH9, m_ctlButtonMonth9);
	DDX_Control(pDX, IDC_BUTTON_NAME, m_ctlButtonName);
	DDX_Control(pDX, IDC_BUTTON_KOSPI200, m_ctlButtonKospi);
	DDX_Text(pDX, IDC_EDIT_CODE, m_szEditCode);
	DDX_Text(pDX, IDC_EDIT_NAME, m_szEditName);
	DDX_Radio(pDX, IDC_PRICE_RDO, m_kindList);
	DDX_Radio(pDX, IDC_RADIO_FUTURE, m_nGubn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFuturesDlg, CDialog)
	//{{AFX_MSG_MAP(CFuturesDlg)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_KOSPI200, OnButtonKospi200)
	ON_BN_CLICKED(IDC_BUTTON_MONTH12, OnButtonMonth12)
	ON_BN_CLICKED(IDC_BUTTON_MONTH9, OnButtonMonth9)
	ON_BN_CLICKED(IDC_BUTTON_MONTH6, OnButtonMonth6)
	ON_BN_CLICKED(IDC_BUTTON_MONTH3, OnButtonMonth3)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH9, OnButtonSPMonth9)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH6, OnButtonSPMonth6)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH3, OnButtonSPMonth3)
	ON_BN_CLICKED(IDC_BUTTON_NAME, OnButtonName)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_MONTH9, OnDoubleclickedButtonMonth9)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_MONTH6, OnDoubleclickedButtonMonth6)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_MONTH3, OnDoubleclickedButtonMonth3)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_MONTH12, OnDoubleclickedButtonMonth12)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_SPMONTH9, OnDoubleclickedButtonSPMonth9)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_SPMONTH6, OnDoubleclickedButtonSPMonth6)
	ON_BN_DOUBLECLICKED(IDC_BUTTON_SPMONTH3, OnDoubleclickedButtonSPMonth3)
	ON_WM_PAINT()
	ON_LBN_DBLCLK(IDC_PCODE_LIST, OnDblclkPcodeList)
	ON_LBN_SELCHANGE(IDC_PCODE_LIST, OnSelchangePcodeList)
	ON_BN_CLICKED(IDC_CODE_RDO, OnPOPTIONRdo)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CODE, OnClickListCode)
	ON_BN_CLICKED(IDC_BUTTON_KOSTAR, OnButtonKostar)
	ON_BN_CLICKED(IDC_BUTTON_JFUTURE, OnButtonJfuture)
	ON_BN_CLICKED(IDC_RADIO_FUTURE, OnRadioFuture)
	ON_BN_CLICKED(IDC_RADIO_SPREAD, OnRadioSpread)
	ON_BN_CLICKED(IDC_BUTTON_OPMONTH1, OnButtonOPMonth1)
	ON_BN_CLICKED(IDC_BUTTON_OPMONTH2, OnButtonOPMonth2)
	ON_BN_CLICKED(IDC_BUTTON_OPMONTH3, OnButtonOPMonth3)
	ON_BN_CLICKED(IDC_BUTTON_OPMONTH4, OnButtonOPMonth4)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CODE, OnDblclkListCode)
	ON_BN_CLICKED(IDC_PRICE_RDO, OnPOPTIONRdo)
//	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFuturesDlg message handlers

BOOL CFuturesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();

	CString configPath;
	char  wb[32];

	configPath.Format("%s\\tab\\config.ini", pApp->m_RootPath);
	DWORD dwRc = GetPrivateProfileString("OPTION", "SORT", "1", wb, sizeof(wb), configPath);

	CString strImg("");
	strImg.Format("%s/%s/", pApp->m_RootPath, "image");

	if (dwRc <= 0) wb[0] = '1';

	m_bSort = atoi(&wb[0]);
	
	CString szTitle;
	m_pCheck = new CCheckBox;
	
	SetDlgPos(m_Point.x, m_Point.y);
	CRect rc(110, 42 + (m_Kind!=optionCODE?24:0), 447+2, 267);
	
	m_pCheck->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_GROUP, rc, this, IDC_WND_CHECK);
	m_pCheck->ShowWindow(SW_SHOW);
	m_pCheck->SetWnd(this);

	((CButton*)GetDlgItem(IDC_BUTTON_NAME))->ShowWindow(SW_HIDE);
	
	if (m_Kind != futureCODE)
		m_pCheck->SetCheckCursor(IDC_CURSOR_HAND);
	
	SetPjCodes();
	SetButton();
	//SetButton(&m_ctlButtonKospi);
	//m_ctlButtonKospi.LoadImg(strImg);

	SetFutureCodes();

	SetSFCodes();

	m_codeList.GetClientRect(&rc);

	m_codeList.SetFont(GetFont());
	m_codeList.SetBkColor(COR_WHITE);
	m_codeList.SetTextBkColor(COR_WHITE);
	m_codeList.SetTextColor(RGB(0,0,0));
	m_codeList.InsertColumn(1, "종목코드", LVCFMT_LEFT, 80);
	m_codeList.InsertColumn(2, "종목명", LVCFMT_LEFT, rc.Width() - 100);

	if (m_Kind == optionCODE)
	{
		CRect rc;
		m_ctlButtonName.GetWindowRect(&rc);
		ScreenToClient(&rc);
		rc.OffsetRect(0, -25);
		m_ctlButtonName.SetWindowPos(NULL, rc.left, rc.top, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
	}

	GetDlgItem(IDC_RADIO_FUTURE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_SPREAD)->ShowWindow(SW_HIDE);

	switch (m_Kind)		// 종류에 따라서..
	{
	case foptionCODE:
		szTitle = "선물/옵션 코드 안내";
		if (m_Type & 1)
		{
			m_ctlButtonKostar.EnableWindow(FALSE);
			OnButtonKospi200();
		}
		else if (m_Type & 2)
		{
			OnButtonKostar();
			m_ctlButtonKospi.EnableWindow(FALSE);
		}
		else
			OnButtonKospi200();
		
		DisableSpread(FALSE);
		break;
	case futureCODE:
		szTitle = "선물 코드 안내";
		if (m_Type & 1)
		{
			m_ctlButtonKostar.EnableWindow(FALSE);
			OnButtonKospi200();
		}
		else if (m_Type & 2)
		{
			OnButtonKostar();
			m_ctlButtonKospi.EnableWindow(FALSE);
		}
		else
			OnButtonKospi200();

		if (!(m_Type & 4))
			DisableSpread(FALSE);
		break;
	case optionCODE:
		szTitle = "옵션 코드 안내";
		m_nStep = optionCODE;
		DisableFuture();
		OnButtonKospi200();
		break;  
	case koptionCODE:
		m_nStep = koptionCODE;
		DisableFuture();
		m_ctlButtonJFuture.EnableWindow(TRUE);
		ShowJOption(TRUE);
		OnPOPTIONRdo();
		m_plistbox.EnableWindow();
		m_plistbox.SetCurSel(0);
		OnDblclkPcodeList();
		szTitle = "주식옵션 코드 안내";
		break;
	case faoptionCODE: // 전부
		if (m_Type & 1)
		{
			m_ctlButtonKostar.EnableWindow(FALSE);
			OnButtonKospi200();
		}
		else if (m_Type & 2)
		{
			OnButtonKostar();
			m_ctlButtonKospi.EnableWindow(FALSE);
		}
		else
			OnButtonKospi200();
		szTitle = "코드 안내";
		break;
	case spreadCODE:
		szTitle = "스프레드";
		if (m_Type & 1)
		{
			m_ctlButtonKostar.EnableWindow(FALSE);
			OnButtonKospi200();
		}
		else if (m_Type & 2)
		{
			OnButtonKostar();
			m_ctlButtonKospi.EnableWindow(FALSE);
		}
		else
			OnButtonKospi200();
		break;
	case kfutureCODE:
		{
			szTitle = "주식선물 코드 안내";
			OnButtonKospi200();
			m_plistbox.EnableWindow();
			m_plistbox.SetCurSel(0);

			m_ctlButtonKospi.EnableWindow(FALSE);
			m_ctlButtonJFuture.EnableWindow(FALSE);
			m_ctlButtonKostar.EnableWindow(FALSE);

			OnButtonJfuture();
		}
		break;

	}

	SetWindowText(szTitle);

	m_ctlButtonName.SetFlat(TRUE);
	//m_ctlButtonName.SetBold(FALSE);
	m_ctlButtonName.m_bNoFocus = TRUE;
	m_ctlButtonName.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COR_BACK);
	m_ctlButtonName.SetColor(CButtonST::BTNST_COLOR_BK_IN, COR_BACK);
	m_ctlButtonName.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);

	//m_ctlButtonJOption.EnableWindow(FALSE);




	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CFuturesDlg::SetDlgPos(int x, int y)
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

void CFuturesDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	SAFE_DELETE(m_pCheck);
	m_arraycall.RemoveAll();
	m_arrayput.RemoveAll();
	m_ListPcode.RemoveAll();

}

BOOL CFuturesDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	CRect rc(0,41,321,225);

	if (m_pCheck->IsWindowEnabled())
	{
		if(zDelta > 0)
		{
			// 위 처리...
			if (m_pCheck->GetInit() < 40)
			{
				m_pCheck->SetInit(m_pCheck->GetInit() + 20); 
				m_pCheck->SetScroll(m_pCheck->GetScroll() - 20);
			}
			
			
			m_pCheck->InvalidateRect(rc);
			m_pCheck->SetScrollPos32(SB_VERT, m_pCheck->GetScroll());
			
		}
		else
		{
			// 아래 처리....
			if (m_pCheck->GetInit() > -20*((m_pCheck->GetCount())-10))
			{
				m_pCheck->SetInit(m_pCheck->GetInit() - 20); 
				m_pCheck->SetScroll(m_pCheck->GetScroll() + 20);
			}
			
			
			m_pCheck->InvalidateRect(rc);
			m_pCheck->SetScrollPos32(SB_VERT, m_pCheck->GetScroll());
			
		}
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CFuturesDlg::OnButtonKospi200() 
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	if (m_Kind != optionCODE)
		m_nStep = futureCODE;

	m_bKostar = FALSE;
	Invalidate();
	AdjustControls();
	SetFutureCodes();
	
	ojcode OJCode;
	DATA data;
	CString szTemp;
	int col = 0, row = 0;
	
	if (m_pCheck->GetSafeHwnd())
		m_pCheck->SetArray();
	m_nCountMonth = 4;

	m_pCheck->SetInit(40);
	m_pCheck->SetScroll(0);
	
	((CButton*)GetDlgItem(IDC_BUTTON_NAME))->ShowWindow(SW_HIDE);

	SetButton();
	
	m_ctlButtonKospi.SetSelected(TRUE);
	m_ctlButtonKostar.SetSelected(FALSE);
	m_ctlButtonJFuture.SetSelected(FALSE);

	if (m_Kind == optionCODE)
	{
		ButtonText("KOSPI200 지수옵션", _T(""));
	}

// 	if (m_Kind != faoptionCODE)
// 		DisableButton();

	m_pCheck->RemoveAll();
	m_pCheck->SetCount((int)pApp->m_arrayOcode.GetSize());
	pApp->m_arrayOcode.QuickSort(m_bSort);

	// CheckBox에 데이터 맞물리기...
	for (int ii = 0; ii < pApp->m_arrayOcode.GetSize(); ii++)
	{
		OJCode = pApp->m_arrayOcode.GetAt(ii);
		
		if (OJCode.atmg == 1)	// ATM 구하기 ...
			m_nAtm = abs(ii);// - (pApp->m_arrayOcode.GetSize() - 1));

		// call... 데이터 
		int jj  = 0;
		for (jj = 0; jj < GetCountMonth(); jj++)
		{
			if (OJCode.call[jj].yorn == 48)		
				data.flag = FALSE;
			else					
			{	
				data.flag = TRUE;
				data.code = OJCode.call[jj].cod2;
				szTemp = OJCode.call[jj].hnam;
				szTemp = szTemp.Left(18);
				szTemp.TrimRight();
				data.name = szTemp;
				
				if (OJCode.atmg == 1)
					data.atm = TRUE;
				else
					data.atm = FALSE;
			}
			m_pCheck->AddData(row, jj, data);	// 데이터 삽입..
			
			data.code = "";
			data.name = "";
			data.flag = FALSE;
		}
		
		szTemp = "";
		data.name = CString(OJCode.price, OPriceLen);

		float	val = (float)atol(data.name);
			val /= 100;
			data.name.Format("%3.1f", val);

		m_pCheck->AddData(row, jj, data);
		data.name = "";
		
		//put... 데이터 
		for (jj = 5; jj < 9; jj++)
		{
			col = jj - 5;
			if (OJCode.put[col].yorn == 48)		
			{	
				data.flag = FALSE;
				data.code = "";
				data.name = "";
			}
			else					
			{
				data.flag = TRUE;
				data.code = OJCode.put[col].cod2;
				szTemp = OJCode.put[col].hnam;
				szTemp = szTemp.Left(18);
				szTemp.TrimRight();
				data.name = szTemp;
			}
			m_pCheck->AddData(row, jj, data);	// 데이터 삽입..

		}
	
		data.name = "";
		data.code = "";
		data.flag = FALSE;
		data.atm = FALSE;
			
		++row;
	}
	
	m_pCheck->ShowRow(m_nAtm);
}

void CFuturesDlg::OnButtonKostar() 
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();

	m_nStep = futureCODE;
	m_bKostar = TRUE;
	Invalidate();
	AdjustControls();
	SetFutureCodes();

	ojcode OJCode;
	DATA data;
	CString szTemp;
	int col = 0, row = 0;
	
	if (m_pCheck->GetSafeHwnd())
		m_pCheck->SetArray();
	m_nCountMonth = 4;

	m_pCheck->SetInit(40);
	m_pCheck->SetScroll(0);
	
	((CButton*)GetDlgItem(IDC_BUTTON_NAME))->ShowWindow(SW_HIDE);

	SetButton();

	m_ctlButtonKostar.SetSelected(TRUE);
	m_ctlButtonKospi.SetSelected(FALSE);
	m_ctlButtonJFuture.SetSelected(FALSE);
	
	if (m_Kind == optionCODE)
	{
		ButtonText("KOSPI200 지수옵션", _T(""));
	}

// 	if (m_Kind != faoptionCODE)
// 		DisableButton();

	m_pCheck->RemoveAll();
	m_pCheck->SetCount((int)pApp->m_arrayOcode.GetSize());

	// CheckBox에 데이터 맞물리기...
	for (int ii = 0; ii < pApp->m_arrayOcode.GetSize() ; ii++)
	{
		OJCode = pApp->m_arrayOcode.GetAt(ii);
		
		if (OJCode.atmg == 1)	// ATM 구하기 ...
			m_nAtm = abs(ii);// - (pApp->m_arrayOcode.GetSize() - 1));

		// call... 데이터 
		int jj = 0;
		for (jj = 0; jj < GetCountMonth(); jj++)
		{
			if (OJCode.call[jj].yorn == 48)		
				data.flag = FALSE;
			else					
			{	
				data.flag = TRUE;
				data.code = OJCode.call[jj].cod2;
				szTemp = OJCode.call[jj].hnam;
				szTemp = szTemp.Left(18);
				szTemp.TrimRight();
				data.name = szTemp;
				
				if (OJCode.atmg == 1)
					data.atm = TRUE;
				else
					data.atm = FALSE;
			}
			m_pCheck->AddData(row, jj, data);	// 데이터 삽입..
			
			data.code = "";
			data.name = "";
			data.flag = FALSE;
		}
		
		szTemp = "";
		data.name = CString(OJCode.price, OPriceLen);

		float	val = (float)atol(data.name);
			val /= 100;
			data.name.Format("%3.1f", val);

		m_pCheck->AddData(row, jj, data);
		data.name = "";
		
		//put... 데이터 
		for (jj = 5; jj < 9; jj++)
		{
			col = jj - 5;
			if (OJCode.put[col].yorn == 48)		
			{	
				data.flag = FALSE;
				data.code = "";
				data.name = "";
			}
			else					
			{
				data.flag = TRUE;
				data.code = OJCode.put[col].cod2;
				szTemp = OJCode.put[col].hnam;
				szTemp = szTemp.Left(18);
				szTemp.TrimRight();
				data.name = szTemp;	
			}
			m_pCheck->AddData(row, jj, data);	// 데이터 삽입..

		}
	
		data.name = "";
		data.code = "";
		data.flag = FALSE;
		data.atm = FALSE;
			
		++row;
	}
	m_pCheck->ShowRow(m_nAtm);
}

void CFuturesDlg::OnButtonMonth3()		// 선물 선택시..
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	FCODE fcode;
	if (m_nStep == kfutureCODE)
		fcode = pApp->m_arraySFcode.GetAt(m_nSFSet[0]);
	else
		fcode = pApp->m_arrayFcode.GetAt(0 + (m_bKostar?7:0));

	MonthInit();
	SetCode(fcode.code);
	SetName(fcode.name);
	UpdateData(FALSE);

	OnOK();
}

void CFuturesDlg::OnButtonMonth6()		// 선물 선택시..
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	FCODE fcode;
	if (m_nStep == kfutureCODE)
		fcode = pApp->m_arraySFcode.GetAt(m_nSFSet[1]);
	else
		fcode = pApp->m_arrayFcode.GetAt(1 + (m_bKostar?7:0));

	MonthInit();
	SetCode(fcode.code);
	SetName(fcode.name);
	UpdateData(FALSE);

	OnOK();
}

void CFuturesDlg::OnButtonMonth9() 
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	FCODE fcode;
	if (m_nStep == kfutureCODE)
		fcode = pApp->m_arraySFcode.GetAt(m_nSFSet[2]);
	else
		fcode = pApp->m_arrayFcode.GetAt(2 + (m_bKostar?7:0));

	MonthInit();
	SetCode(fcode.code);
	SetName(fcode.name);
	UpdateData(FALSE);

	OnOK();
}

void CFuturesDlg::OnButtonSPMonth3()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	FCODE fcode;

	if (m_nStep == kfutureCODE)
		fcode = pApp->m_arraySFcode.GetAt(m_nSFSet[4]);
	else
		fcode = pApp->m_arrayFcode.GetAt(4 + (m_bKostar?7:0));

	MonthInit();
	SetCode(fcode.code);
	SetName(fcode.name);
	UpdateData(FALSE);

	OnOK();
}

void CFuturesDlg::OnButtonSPMonth6()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	FCODE fcode;

	if (m_nStep == kfutureCODE)
		fcode = pApp->m_arraySFcode.GetAt(m_nSFSet[5]);
	else
		fcode = pApp->m_arrayFcode.GetAt(5 + (m_bKostar?7:0));

	MonthInit();
	SetCode(fcode.code);
	SetName(fcode.name);
	UpdateData(FALSE);

	OnOK();
}

void CFuturesDlg::OnButtonSPMonth9() 
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	FCODE fcode;

	if (m_nStep == kfutureCODE)
		fcode = pApp->m_arraySFcode.GetAt(m_nSFSet[6]);
	else
		fcode = pApp->m_arrayFcode.GetAt(6 + (m_bKostar?7:0));

	MonthInit();
	SetCode(fcode.code);
	SetName(fcode.name);
	UpdateData(FALSE);

	OnOK();
}

void CFuturesDlg::OnButtonMonth12()		// 선물 선택시..
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	FCODE fcode;

	if (m_nStep == kfutureCODE)
		fcode = pApp->m_arraySFcode.GetAt(m_nSFSet[3]);
	else
		fcode = pApp->m_arrayFcode.GetAt(3 + (m_bKostar?7:0));

	MonthInit();
	SetCode(fcode.code);
	SetName(fcode.name);
	UpdateData(FALSE);

	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonMonth3() 
{
	OnButtonMonth3();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonMonth6() 
{
	OnButtonMonth6();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonMonth9() 
{
	OnButtonMonth9();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonSPMonth3() 
{
	OnButtonSPMonth3();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonSPMonth6() 
{
	OnButtonSPMonth6();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonSPMonth9() 
{
	OnButtonSPMonth9();
	OnOK();
}

void CFuturesDlg::OnDoubleclickedButtonMonth12() 
{
	OnButtonMonth12();
	OnOK();
}

void CFuturesDlg::ButtonText(CString szName, CString szCode)
{
	if (m_Kind == optionCODE)
		szCode = _T("");
	else
		szCode = "(" + szCode + ")" + " 기초자산";
	
	m_ctlButtonName.ShowWindow(SW_SHOW);
	m_ctlButtonName.SetWindowText(szName+szCode);
}

void CFuturesDlg::OnButtonName() 
{
	m_szEditCode = m_szBCode;
	m_szEditName = m_szBName;
	
	UpdateData(FALSE);
}

void CFuturesDlg::OnButtonClose() 
{
	OnOK();
}

BOOL CFuturesDlg::SetPjCodes()			//주식옵션.. 코드 종목...
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int				codeN;
	PCODE			pcode;
	CMapStringToPtr pcodemap;
	CQArray	<CString, CString>	qKindArr;
	
	// 데이터의 갯수...
	codeN = (int)pApp->m_arrayPcode.GetSize();
	
	int ii = 0;
	for (ii = 0; ii < codeN; ii++)
	{
		pcode = pApp->m_arrayPcode.GetAt(ii);
		pcodemap.SetAt(pcode.kind, (void*)ii);
	}

	POSITION pos = pcodemap.GetStartPosition();
	CString sKind;
	int value;

	if (m_plistbox.m_hWnd && IsWindow(m_plistbox.m_hWnd))
	{
		m_plistbox.ResetContent();
		while (pos)
		{
			pcodemap.GetNextAssoc(pos, sKind, (void*&)value);
			qKindArr.Add(sKind);
		}

		qKindArr.QuickSort();
		
		for (ii = 0; ii < qKindArr.GetSize(); ii++)
		{
			sKind = qKindArr.GetAt(ii);
			if (pcodemap.Lookup(sKind, (void*&)value))
				m_plistbox.SetItemData(m_plistbox.AddString(pApp->m_arrayPcode.GetAt(value).name), value);
		}
	}
	return TRUE;
}


void CFuturesDlg::Standard(CString szCode)		// 콜옵션 풋옵션..
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	PCODE pcode;
	SetPjCodes();

	for (int ii = 0; ii < pApp->m_arrayPcode.GetSize(); ii++)
	{
		pcode = pApp->m_arrayPcode.GetAt(ii);
	
		if (!pcode.kind.Compare(szCode))
		{
			if (pcode.code.Left(1).Compare("2") == 0)	// 콜
				m_arraycall.Add(pcode);
			if (pcode.code.Left(1).Compare("3") == 0)	// 풋
				m_arrayput.Add(pcode);
		}
	}

	// 이후에...
	SearchPrice();
	SearchMonth();
}

void CFuturesDlg::SearchPrice()
{
	PCode pcode, temp;
	POSITION pos; 

	for (int ii= 0; ii < m_arraycall.GetSize(); ii++)
	{
		pcode = m_arraycall.GetAt(ii);

		if (m_ListPcode.IsEmpty())
			m_ListPcode.AddHead(pcode);
		
		if (!FindPrice(pcode.price))
			AddList(pcode);					
	}

	pos = m_ListPcode.GetHeadPosition();
	INT_PTR ii = 0;
	for (ii = 0; ii < m_ListPcode.GetCount(); ii++)
	{
		pcode = m_ListPcode.GetAt(pos);
		m_ArrayPrice.InsertAt(0, pcode.price);
		m_ListPcode.GetNext(pos);
	}
	
	m_ListPcode.RemoveAll();
}

void CFuturesDlg::SearchMonth()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	PCode pcode, temp;
	CString szTemp;
	
	POSITION pos; 
	
	INT_PTR ii = 0;
	for (ii= 0; ii < m_arraycall.GetSize(); ii++)
	{
		pcode = m_arraycall.GetAt(ii);
		
		if (m_ListMonth.IsEmpty())
			m_ListMonth.AddHead(pcode.month);
		
		if (!FindMonth(pcode.month))
			m_ListMonth.AddHead(pcode.month);	
	}
	
	pos = m_ListMonth.GetHeadPosition();
	
	for (ii = 0; ii < m_ListMonth.GetCount(); ii++)
	{
		szTemp = m_ListMonth.GetAt(pos);
		m_ListMonth.GetNext(pos);
		if (ii < 4)
			GetDlgItem(IDC_BUTTON_OPMONTH4 - (int)ii)->SetWindowText(szTemp.Right(2) + "월");
	}
	
	m_nCountMonth = (int)pApp->m_ArrayOMonth.GetSize();
	m_ListMonth.RemoveAll();

}

BOOL CFuturesDlg::FindPrice(CString szCompare)
{
	if(m_ListPcode.IsEmpty())
		return FALSE;

	PCode pcode;
	POSITION pos = m_ListPcode.GetHeadPosition();

	for (int ii = 0; ii < m_ListPcode.GetCount(); ii++)
	{
		pcode = m_ListPcode.GetAt(pos);
		if (pcode.price == szCompare)
			return TRUE;
		m_ListPcode.GetNext(pos);
	}

	return FALSE;
}

void CFuturesDlg::AddList(PCODE &pcode)
{
	POSITION pos = m_ListPcode.GetHeadPosition();
	PCODE temp;

		for (int ii = 0; ii < m_ListPcode.GetCount(); ii++)
		{
			temp = m_ListPcode.GetAt(pos);
			
			if (pcode.price.Compare(temp.price) > 0)
			{
				m_ListPcode.AddHead(pcode);
				return;
			}
			
			m_ListPcode.GetNext(pos);
		}
}

BOOL CFuturesDlg::FindMonth(CString szMon)
{
	if(m_ListMonth.IsEmpty())
		return FALSE;

	CString sztemp;
	POSITION pos = m_ListMonth.GetHeadPosition();

	for (int ii = 0; ii < m_ListMonth.GetCount(); ii++)
	{
		sztemp = m_ListMonth.GetAt(pos);
		if (sztemp == szMon)
			return TRUE;
		m_ListMonth.GetNext(pos);
	}

	return FALSE;
}

DATA CFuturesDlg::FindCall(CString szMonth, CString szPrice)
{
	DATA data;
	PCODE code;
	
		for (int ii = 0; ii < m_arraycall.GetSize(); ii++)
		{
			code = m_arraycall.GetAt(ii);
			if (code.month == szMonth && code.price == szPrice)
			{
				data.flag = TRUE;
				data.code = code.code;
				data.name = code.hnam;
				data.atm  = code.atm;
	
				return data;
			}
			else
			{
				data.flag = FALSE;
				data.code = "";
				data.name = "";
				data.atm  = 0;
			}

		}

	return data;

}

DATA CFuturesDlg::FindPut(CString szMonth, CString szPrice)
{
	DATA data;
	PCODE code;
	
		for (int ii = 0; ii < m_arrayput.GetSize(); ii++)
		{
			code = m_arrayput.GetAt(ii);
			if (code.month == szMonth && code.price == szPrice)
			{
				data.flag = TRUE;
				data.code = code.code;
				data.name = code.hnam;
				data.atm  = code.atm;
			
				return data;
			}
			else
			{
				data.flag = FALSE;
				data.code = "";
				data.name = "";
				data.atm  = 0;
			}
		}

	return data;
}

void CFuturesDlg::FindEnd()
{
	m_pCheck->SetArray();
	m_ArrayPrice.RemoveAll();
	m_arraycall.RemoveAll();
	m_arrayput.RemoveAll();
}

void CFuturesDlg::CountData(CString code)		// 데이터 처리..
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	Standard(code);

	DATA data;
	CString szMonth, szPrice;
	
	m_pCheck->RemoveAll();
	m_pCheck->SetCount((int)m_ArrayPrice.GetSize());
	
	//sort...
	m_ArrayPrice.QuickSort(m_bSort);


	// 열 ... call 쪽...
	for (int ii = 0; ii < 4; ii++)
	{
		szMonth = "";
		for (int jj = 0; jj < m_ArrayPrice.GetSize(); jj++) // 행...
		{
			if (pApp->m_ArrayOMonth.GetSize() > ii)
				szMonth = pApp->m_ArrayOMonth.GetAt(ii);
			szPrice = m_ArrayPrice.GetAt(jj);
			TRACE(szPrice + "\n");
			data = FindCall(szMonth, szPrice);
			m_pCheck->AddData(jj, ii, data);
			
			data.code = "";
			data.flag = FALSE;
			szPrice.TrimLeft();
			
			data.name = CommaModify(szPrice);
			m_pCheck->AddData(jj, 4, data);
			
			if (data.atm == 1)		// atm..
				m_nAtm = jj;
		}
	}
	
	// 열 ... put 쪽...
	for (int kk = 0; kk < 4; kk++)
	{
		szMonth = "";
		for (int jj = 0; jj < m_ArrayPrice.GetSize(); jj++) // 행...
		{
			if (pApp->m_ArrayOMonth.GetSize() > kk)
				szMonth = pApp->m_ArrayOMonth.GetAt(kk);
			szPrice = m_ArrayPrice.GetAt(jj);
			data = FindPut(szMonth, szPrice);
			m_pCheck->AddData(jj, 8 - kk, data);	
		}
	}
	
	m_pCheck->Invalidate();
	FindEnd();
}

CString CFuturesDlg::CommaModify(CString string)
{
	CString stripData = string;
	stripData.TrimLeft(); stripData.TrimRight();

	if (stripData.GetLength() <= 3)
		return stripData;

	char dChar = 0x00;
	switch (stripData[0])
	{
	case '+':
		dChar = '+'; stripData.Delete(0);
		break;
	case '-':
		dChar = '-'; stripData.Delete(0);
		break;
	}

	stripData.MakeReverse();
	string = _T("");
	for (int ii = 0; ii < stripData.GetLength(); ii++)
	{
		if ((ii != 0) && ((ii % 3) == 0))
			string += ',';

		string += stripData[ii];
	}

	string.MakeReverse();
	if (dChar != 0x00) string.Insert(0, dChar);
	return string;
}

void CFuturesDlg::DisableButton()
{
	GetDlgItem(IDC_BUTTON_JFUTURE)->EnableWindow(FALSE);
	m_plistbox.EnableWindow(FALSE);
}

void CFuturesDlg::SetButton()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	CString strImg("");
	strImg.Format("%s/%s/", pApp->m_RootPath, "image");

	//SetButton(&m_ctlButtonMonth12);
	//SetButton(&m_ctlButtonMonth3);
	//SetButton(&m_ctlButtonMonth6);
	//SetButton(&m_ctlButtonMonth9);
	//SetButton(&m_ctlButtonSPMonth3);
	//SetButton(&m_ctlButtonSPMonth6);
	//SetButton(&m_ctlButtonSPMonth9);
	//SetButton(&m_ctlButtonKostar);
	//SetButton(&m_ctlButtonJFuture);
	//SetButton(&m_ctlButtonKospi);
	//SetButton(&m_ctlButtonOPMonth1);
	SetButton(&m_ctlButtonOPMonth2);
	SetButton(&m_ctlButtonOPMonth3);
	SetButton(&m_ctlButtonOPMonth4);

	m_ctlButtonMonth3.LoadImg(strImg);
	m_ctlButtonMonth6.LoadImg(strImg);
	m_ctlButtonMonth9.LoadImg(strImg);
	m_ctlButtonMonth12.LoadImg(strImg);
	m_ctlButtonSPMonth3.LoadImg(strImg);
	m_ctlButtonSPMonth6.LoadImg(strImg);
	m_ctlButtonSPMonth9.LoadImg(strImg);
	m_ctlButtonKostar.LoadImg(strImg);
	m_ctlButtonJFuture.LoadImg(strImg);
	m_ctlButtonKospi.LoadImg(strImg);
	m_ctlButtonOPMonth1.LoadImg(strImg);

	//m_ctlButtonClose.SetBtnCursor(IDC_CURSOR_HAND);
}

void CFuturesDlg::SetButton(CButtonST *pButton)
{
	short	shBtnColor = 245;
	pButton->SetFlat(FALSE);
	pButton->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	pButton->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
	pButton->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
	pButton->SetBtnCursor(IDC_CURSOR_HAND);
	pButton->SetBold(FALSE);
}


void CFuturesDlg::InitBtn()
{
	m_ctlButtonKospi.SetBold(FALSE);
	m_ctlButtonKostar.SetBold(FALSE);
	m_ctlButtonJFuture.SetBold(FALSE);
}

void CFuturesDlg::MonthInit()
{
	//m_ctlButtonMonth12.SetBold(FALSE);
	//m_ctlButtonMonth9.SetBold(FALSE);
	//m_ctlButtonMonth6.SetBold(FALSE);
	//m_ctlButtonMonth3.SetBold(FALSE);
}

void CFuturesDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	
	CRect rc;//(305, 17, 90, 35);
	m_ctlButtonSPMonth9.GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.OffsetRect(rc.Width() + 7, 0);
	//rc.DeflateRect(0, 2, 0, 3);

	CString szTitle;

	m_Tool.SetDC(&dc);
	m_Tool.FrameRect2(rc, RGB(240,240,240), RGB(164,164,164), RGB(116,116,116), TRUE);

	switch (m_nStep)
	{
	case foptionCODE:
		szTitle = "선물 옵션";
		break;
	case futureCODE:
		if (m_bKostar)
			szTitle = "KOSTAR";
		else
			szTitle = "KOSPI200";
		break;
	case optionCODE:
		szTitle = "옵 션";
		break;  
	case koptionCODE:
		szTitle = "주식옵션";
		break;
	case faoptionCODE:
		szTitle = "선물 옵션";
		break;
	case spreadCODE:
		szTitle = "스프레드";
		break;
	case kfutureCODE:
		szTitle = "주식선물";
		break;
	}

	m_Tool.WriteText(szTitle, rc);
	
	CPen	pen(PS_SOLID, 1, RGB(128,128,128)), *oldpen;

	oldpen = dc.SelectObject(&pen);

	//dc.MoveTo(17, 68);
	//dc.LineTo(88, 68);
	//dc.MoveTo(17, 146);
	//dc.LineTo(88, 146);
	//dc.MoveTo(17, 248);
	//dc.LineTo(88, 248);
////////
	
	dc.SelectObject(oldpen);
	pen.DeleteObject();
	
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CFuturesDlg::FindCode(int kind, CString szCode)		// GetName()에서 사용. 
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	FCODE	fcode;
	ojcode	OJcode;
	PCODE	pcode;

	szCode.TrimRight();

	switch(kind)
	{
	case futureNAME:
		{
			if (pApp->m_arrayFcode.GetSize() < 0)
				return FALSE;

			for(int ii = 0; ii < pApp->m_arrayFcode.GetSize(); ii++)
			{
				fcode = pApp->m_arrayFcode[ii];
				if (szCode.CompareNoCase(fcode.code) == 0) // fcode.code == szCode)
				{
					fcode.name.TrimRight();
					SetName(fcode.name);
					return TRUE;
				}
			}

		}
		break;
	case optionNAME:
		{
			if (pApp->m_arrayOcode.GetSize() < 0)
				return FALSE;

			for (int ii = 0; ii < pApp->m_arrayOcode.GetSize(); ii++)
			{
				OJcode = pApp->m_arrayOcode.GetAt(ii);
				
				for (int jj = 0; jj < 4 ; jj++)
				{
					if (szCode.CompareNoCase(OJcode.call[jj].cod2) == 0)  // == szCode)
					{
						SetName(OJcode.call[jj].hnam);
						return TRUE;
					}

					if (szCode.CollateNoCase(OJcode.put[jj].cod2) == 0) // == szCode)
					{
						SetName(OJcode.put[jj].hnam);
						return TRUE;
					}
				}
			
			}
		}
		break;
	case koptionNAME:
		{
			if (pApp->m_arrayPcode.GetSize() <=  0)
				pApp->m_arrayPcode.GetSize();

			for (int ii = 0; ii < pApp->m_arrayPcode.GetSize(); ii++)
			{
				pcode = pApp->m_arrayPcode.GetAt(ii);

				if (szCode.CompareNoCase(pcode.code) == 0) // == szCode)
				{
					pcode.name.TrimRight();
//					CString temp =  pcode.name+ " " + pcode.hnam + " " + pcode.month;
					CString temp =  pcode.hnam;
					SetName(temp);
					return TRUE;
				}
			}

		}
		break;
	case getCODEfuture:
		{
			if (pApp->m_arrayFcode.GetSize() < 0)
				return FALSE;

			fcode = pApp->m_arrayFcode[0];
			
			//	SetName(fcode.name);
				SetCode(fcode.code);
				return TRUE;
		}
		break;
	case getCODEcall:
		{
			if (pApp->m_arrayOcode.GetSize() < 0)
				return FALSE;
			
			for (int ii = 0; ii < pApp->m_arrayOcode.GetSize(); ii++)
			{
				OJcode = pApp->m_arrayOcode.GetAt(ii);
				
				if (OJcode.atmg == 1)
				{
				//	SetName(OJcode.call[3].hnam);
					SetCode(OJcode.call[3].cod2);
					return TRUE;
				}
			
			}

		}
		break;
	case getCODEput:
		{
			if (pApp->m_arrayOcode.GetSize() < 0)
				return FALSE;
			
			for (int ii = 0; ii < pApp->m_arrayOcode.GetSize(); ii++)
			{
				OJcode = pApp->m_arrayOcode.GetAt(ii);
				
				if (OJcode.atmg == 1)
				{	
				//	SetName(OJcode.put[0].hnam);
					SetCode(OJcode.put[0].cod2);
					return TRUE;
				}
			}

		}
		break;
	case kfutureNAME:
		{
			if (pApp->m_arraySFcode.GetSize() < 0)
				return FALSE;

			for(int ii = 0; ii < pApp->m_arraySFcode.GetSize(); ii++)
			{
				fcode = pApp->m_arraySFcode[ii];
				if (szCode.CompareNoCase(fcode.code) == 0)
				{
					fcode.name.TrimRight();
					SetName(fcode.name);
					return TRUE;
				}
			}
		}

	}
	
	return FALSE;
}

void CFuturesDlg::DisableFuture()
{
	for (int ii = IDC_BUTTON_MONTH3 ; ii <= IDC_BUTTON_SPMONTH9 ; ii++)
		((CButton*)GetDlgItem(ii))->ShowWindow(SW_HIDE);

	m_ctlButtonKospi.EnableWindow(FALSE);
	m_ctlButtonKostar.EnableWindow(FALSE);
	m_plistbox.EnableWindow(FALSE);
}

bool CFuturesDlg::GetCodeOrderable(CString code, int* type)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	bool	rValue = false;

	SetPjCodes();
	if (m_pCheck->GetSafeHwnd())
		m_pCheck->SetArray();

	if (code.IsEmpty())
		return false;

	CString comp;
	CString kind = code.Left(2);
	switch (atoi(kind))
	{
	case 10: // 선물
	case 40: // 선물스프레드
		{
			FCODE	fcode;
			for (int ii = 0 ; ii < pApp->m_arrayFcode.GetSize() ; ii++)
			{
				fcode = pApp->m_arrayFcode.GetAt(ii);
				if (fcode.code.Compare(code) == 0)
				{
					*type = fcode.mchk;
					rValue = true;
					break;
				}
			}
		}
		break;
	case 20: // 콜옵션
		{
			ojcode	ojcode;
			for (int ii = 0 ; ii < pApp->m_arrayOcode.GetSize() ; ii++)
			{
				ojcode = pApp->m_arrayOcode.GetAt(ii);
				for (int jj = 0 ; jj < 4 ; jj++)
				{
					if (ojcode.call[jj].yorn == '1')
					{
						comp = CString(ojcode.call[jj].cod2, OCodeLen);
						if (comp.Compare(code) == 0)
						{
							*type = ojcode.call[jj].mchk;
							rValue = true;
							break;
						}
					}
				}

				if (rValue)
					break;
			}
		}
		break;
	case 30: // 풋옵션
		{ 
			ojcode	ojcode;
			for (int ii = 0 ; ii < pApp->m_arrayOcode.GetSize() ; ii++)
			{
				ojcode = pApp->m_arrayOcode.GetAt(ii);
				for (int jj = 0 ; jj < 4 ; jj++)
				{
					if (ojcode.put[jj].yorn == '1')
					{
						comp = CString(ojcode.put[jj].cod2, OCodeLen);
						if (comp.Compare(code) == 0)
						{
							*type = ojcode.put[jj].mchk;
							rValue = true;
							break;
						}
					}
				}

				if (rValue)
					break;
			}
		}
		break;
	case 21: // 주식옵션 콜
	case 31: // 주식옵션 풋
		{
			PCODE	pcode;
			for (int ii = 0 ; ii < pApp->m_arrayPcode.GetSize() ; ii++)
			{
				pcode = pApp->m_arrayPcode.GetAt(ii);
				if (pcode.code.Compare(code) == 0)
				{
					*type = pcode.mchk;
					rValue = true;
					break;
				}
			}
		}
		break;
	}

	return rValue;
}

void CFuturesDlg::OnDblclkPcodeList() 
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int nItem = m_plistbox.GetCurSel();
	if (nItem == LB_ERR)
		return;
	int nIdx = (int)m_plistbox.GetItemData(nItem);

	switch (m_nStep)
	{
	case koptionCODE:
		{
			if (pApp->m_arrayPcode.GetSize() <= nIdx)
				return;

			m_pCheck->SetInit(40);
			m_pCheck->SetScroll(0);

			CString sCode = pApp->m_arrayPcode.GetAt(nIdx).gcod;
			if (sCode.GetLength() >= 9)
				sCode = sCode.Mid(3, 6);

			CountData(pApp->m_arrayPcode.GetAt(nIdx).kind);
			OnButtonOPMonth1();
			ButtonText(pApp->m_arrayPcode.GetAt(nIdx).name, sCode);
			m_pCheck->ShowRow(m_nAtm);

		}
		break;
	case kfutureCODE:
		{
			m_nSFSet[0]= 0;
			m_nSFSet[1]= 0;
			m_nSFSet[2]= 0;
			m_nSFSet[3]= 0;
			m_nSFSet[4]= 0;
			m_nSFSet[5]= 0;
			m_nSFSet[6]= 0;

			int btnCnt = 0;

			CString str;
			m_plistbox.GetText(nItem, str);
			m_codeList.DeleteAllItems();
			
			for (int ii = 0; ii < pApp->m_arraySFcode.GetSize(); ii++)
			{
				FCODE fcode = pApp->m_arraySFcode.GetAt(ii);
				
				if (fcode.gnam.Compare(str) == 0)
				{
					CString temp = CString(fcode.name), code;
					temp.TrimRight();
					code = pApp->m_arraySFcode[ii].code;
					code.TrimLeft(); code.TrimRight();

					if (btnCnt > 7)
						break;
					//if (!code.IsEmpty() && code[0] == '4')
					if (!code.IsEmpty())
					{
						((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + btnCnt))->SetWindowText(code);
						m_nSFSet[btnCnt] = ii;
					}
// 					else
// 					{
// 						pos = temp.Find('F');
// 						temp = temp.Mid(pos, 8);
// 						//temp = "선물 " + temp.Right(4);
// 						((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + btnCnt))->SetWindowText(temp);
// 					}
					btnCnt++;
				}
			}
// 			CString str;
// 			m_plistbox.GetText(nItem, str);
// 			m_codeList.DeleteAllItems();
// 			
// 			for (int ii = 0; ii < pApp->m_arrayFcode.GetSize(); ii++)
// 			{
// 				FCODE fcode = pApp->m_arrayFcode.GetAt(ii);
// 				
// 				if (fcode.gnam.Compare(str) == 0)
// 				{
// 					if (m_Spread == 0 && fcode.code[0] == '1')
// 						AddListCtlCode(m_codeList.GetItemCount(), fcode.code, fcode.name);
// 					else if (m_Spread == 1 && fcode.code[0] == '4')
// 						AddListCtlCode(m_codeList.GetItemCount(), fcode.code, fcode.name);
// 				}
// 				else if (str.Compare("전체")==0)
// 				{
// 					if (m_Spread == 0 && fcode.code[0] == '1')
// 						AddListCtlCode(m_codeList.GetItemCount(), fcode.code, fcode.name);
// 					else if (m_Spread == 1 && fcode.code[0] == '4')
// 						AddListCtlCode(m_codeList.GetItemCount(), fcode.code, fcode.name);
// 				}
// 			}
		}
		break;
	}
}

void CFuturesDlg::OnSelchangePcodeList() 
{
	OnDblclkPcodeList();
}

CString CFuturesDlg::GetString(char *pChar, int nMinLen)
{
	return CString(pChar, min(nMinLen, (int)strlen(pChar)));
}

void CFuturesDlg::HideFutureBtns(BOOL bHide)
{
	GetDlgItem(IDC_BUTTON_MONTH3)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_MONTH6)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_MONTH9)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_MONTH12)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_SPMONTH3)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_SPMONTH6)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_BUTTON_SPMONTH9)->ShowWindow(bHide?SW_HIDE:SW_SHOW);
}

void CFuturesDlg::OnPOPTIONRdo() 
{
	UpdateData();
	if (m_kindList == 0)
	{
		m_pCheck->ShowWindow(SW_SHOW);
		m_codeList.ShowWindow(SW_HIDE);
		m_ctlButtonOPMonth1.EnableWindow(FALSE);
		m_ctlButtonOPMonth2.EnableWindow(FALSE);
		m_ctlButtonOPMonth3.EnableWindow(FALSE);
		m_ctlButtonOPMonth4.EnableWindow(FALSE);
	}
	else
	{
		m_pCheck->ShowWindow(SW_HIDE);
		m_codeList.ShowWindow(SW_SHOW);
		m_ctlButtonOPMonth1.EnableWindow();
		m_ctlButtonOPMonth2.EnableWindow();
		m_ctlButtonOPMonth3.EnableWindow();
		m_ctlButtonOPMonth4.EnableWindow();
		OnButtonOPMonth1();
	}
}

void CFuturesDlg::ShowJOption(BOOL bShow)
{
	UINT uShow = bShow?SW_SHOW:SW_HIDE;
	m_ctlButtonOPMonth1.ShowWindow(uShow);
	m_ctlButtonOPMonth2.ShowWindow(uShow);
	m_ctlButtonOPMonth3.ShowWindow(uShow);
	m_ctlButtonOPMonth4.ShowWindow(uShow);
	GetDlgItem(IDC_PRICE_RDO)->ShowWindow(uShow);
	GetDlgItem(IDC_CODE_RDO)->ShowWindow(uShow);
}

void CFuturesDlg::OnButtonOPMonth1()
{
	SetCodeList(3);
}

void CFuturesDlg::OnButtonOPMonth2()
{
	SetCodeList(2);
}

void CFuturesDlg::OnButtonOPMonth3()
{
	SetCodeList(1);
}

void CFuturesDlg::OnButtonOPMonth4()
{
	SetCodeList(0);
}

void CFuturesDlg::SetCodeList(int nMonth)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int nItem = m_plistbox.GetCurSel();
	if (nItem == LB_ERR)
		return;

	int nIdx = (int)m_plistbox.GetItemData(nItem);

	if (pApp->m_arrayPcode.GetSize() <= nIdx)
		return;
	Standard(pApp->m_arrayPcode.GetAt(nIdx).kind);

	if (pApp->m_ArrayOMonth.GetSize() <= nMonth)
		return;
	
	m_codeList.DeleteAllItems();
	DATA data;
	CString szMonth, szPrice;
	szMonth = pApp->m_ArrayOMonth.GetAt(nMonth);
	m_codeList.SetAtmCallIndex(-1);
	m_codeList.SetAtmPutIndex(-1);

	CMapStringToString codemap;
	CQArray	<CString, CString>	qCodeArr;

	int jj = 0;
	for (jj = 0; jj < m_ArrayPrice.GetSize(); jj++) // 행...
	{
		szPrice = m_ArrayPrice.GetAt(jj);
		
		data.code = "";
		data.flag = FALSE;
		data = FindCall(szMonth, szPrice);
		if (!data.code.IsEmpty())
		{
			CString sData;
			sData.Format("%s\t%d", data.name, data.atm);
			codemap.SetAt(data.code, sData);
			qCodeArr.Add(data.code);
		}
	}

	qCodeArr.QuickSort();
	while (qCodeArr.GetSize())
	{
		CString sData;
		if (codemap.Lookup(qCodeArr.GetAt(0), sData))
		{
			int rItem = AddListCtlCode(m_codeList.GetItemCount(), qCodeArr.GetAt(0), Parser(sData, "\t"));
			if (atoi(sData) == 1)		// atm..
				m_codeList.SetAtmCallIndex(rItem);
			qCodeArr.RemoveAt(0);
		}
	}

	codemap.RemoveAll();

	for (jj = 0; jj < m_ArrayPrice.GetSize(); jj++) // 행...
	{
		szPrice = m_ArrayPrice.GetAt(jj);
		
		data.code = "";
		data.flag = FALSE;
		data = FindPut(szMonth, szPrice);
		if (!data.code.IsEmpty())
		{
			CString sData;
			sData.Format("%s\t%d", data.name, data.atm);
			codemap.SetAt(data.code, sData);
			qCodeArr.Add(data.code);
		}
	}
	
	qCodeArr.QuickSort();
	while (qCodeArr.GetSize())
	{
		CString sData;
		if (codemap.Lookup(qCodeArr.GetAt(0), sData))
		{
			int rItem = AddListCtlCode(m_codeList.GetItemCount(), qCodeArr.GetAt(0), Parser(sData, "\t"));
			if (atoi(sData) == 1)		// atm..
				m_codeList.SetAtmPutIndex(rItem);
			qCodeArr.RemoveAt(0);
		}
	}

	FindEnd();
}

int CFuturesDlg::AddListCtlCode(int nItem, CString szCode, CString szName)
{
	szCode.TrimLeft(); szCode.TrimRight(); szName.TrimLeft(); szName.TrimRight();

	int rItem;
	rItem = m_codeList.InsertItem(LVIF_TEXT|LVIF_PARAM, nItem, szCode
		, NULL, NULL, 0, (LPARAM) NULL);

	m_codeList.SetItemText(rItem, 1, szName);
	return rItem;
}

void CFuturesDlg::OnDblclkListCode(NMHDR *pNMHDR, LRESULT *pResult)
{
	int	nItem;
	CString	code, name;
	POSITION pos = m_codeList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			nItem = m_codeList.GetNextSelectedItem(pos);

			code = m_codeList.GetItemText(nItem, 0);
			name = m_codeList.GetItemText(nItem, 1);

			SetCode(code);
			SetName(name);
			
			UpdateData(FALSE);
		}
	}

	*pResult = 0;
	OnOK();
}

void CFuturesDlg::OnClickListCode(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int	nItem;
	CString	code, name;
	POSITION pos = m_codeList.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			nItem = m_codeList.GetNextSelectedItem(pos);

			code = m_codeList.GetItemText(nItem, 0);
			name = m_codeList.GetItemText(nItem, 1);

			SetCode(code);
			SetName(name);
			
			UpdateData(FALSE);
		}
	}
	*pResult = 0;
}

void CFuturesDlg::SetFutureCodes()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int btnCnt = 0;

	if(m_Kind == kfutureCODE)
	{
		// do it...
		return;
	}
	else
	{
		for (int jj = 0; jj < pApp->m_arrayFcode.GetSize(); jj++)
		{
			FCODE fcode = pApp->m_arrayFcode.GetAt(jj);
			CString temp = CString(fcode.name), code;
			temp.TrimRight();
			code = pApp->m_arrayFcode[jj].code;
			code.TrimLeft(); code.TrimRight();
			if (m_bKostar)
				if (code.GetLength() <= 3 || code[2] != '3')
					continue;
			if (btnCnt > 7)
				break;
			if (!code.IsEmpty() && code[0] == '4')
				((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + btnCnt))->SetWindowText(code);
			else
			{
				if (m_bKostar)
					temp = "스타 " + temp.Right(4);
				else
					temp = "선물 " + temp.Right(4);
				((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + btnCnt))->SetWindowText(temp);
			}
			btnCnt++;
		}
	}


	if (m_Kind == spreadCODE)
	{
		GetDlgItem(IDC_BUTTON_MONTH3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MONTH6)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MONTH9)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_MONTH12)->EnableWindow(FALSE);
	}
}

void CFuturesDlg::AdjustControls()
{
	//CRect checkRC(110, 42 + (m_nStep!=optionCODE?24:0), 447, 267);
	CRect checkRC(110, 42 + (m_nStep!=optionCODE?24:0), 447+2, 287);
	GetDlgItem(IDC_RADIO_FUTURE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_SPREAD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CODE1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC_NAME1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_CODE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_NAME)->ShowWindow(SW_SHOW);
	m_pCheck->ShowWindow(SW_SHOW);

	switch (m_nStep)
	{
	case futureCODE:
		HideFutureBtns(FALSE);
		// 주식옵션에서 future code로 넘어갈때
		UpdateData();
		m_codeList.ShowWindow(SW_HIDE);
		if (m_kindList == 1)
		{
			m_kindList = 0;
			UpdateData(FALSE);
			OnPOPTIONRdo();
		}

		ShowJOption(FALSE);
		m_plistbox.EnableWindow(FALSE);
		m_pCheck->SetWindowPos(NULL, checkRC.left, checkRC.top, checkRC.Width(), checkRC.Height(), SWP_NOZORDER);
		if (m_bKostar || m_Kind == spreadCODE)	// kostar
			m_pCheck->EnableWindow(FALSE);
		else
			m_pCheck->EnableWindow();
		break;
	case koptionCODE:
		HideFutureBtns();
		ShowJOption(TRUE);
		m_pCheck->EnableWindow();
		m_pCheck->SetWindowPos(NULL, checkRC.left, checkRC.top, checkRC.Width(), checkRC.Height(), SWP_NOZORDER);
		m_codeList.ShowWindow(SW_HIDE);
		m_codeList.SetWindowPos(NULL, checkRC.left, checkRC.top, checkRC.Width(), checkRC.Height(), SWP_NOZORDER);

		break;
	case optionCODE:
		HideFutureBtns();
		m_pCheck->EnableWindow();
		m_pCheck->SetWindowPos(NULL, checkRC.left, checkRC.top, checkRC.Width(), checkRC.Height(), SWP_NOZORDER);
		m_codeList.ShowWindow(SW_HIDE);
		break;
	case kfutureCODE:
		{
 			m_plistbox.EnableWindow();
 			m_plistbox.SetCurSel(0);

			HideFutureBtns(FALSE);
			// 주식옵션에서 future code로 넘어갈때
			UpdateData();
			m_codeList.ShowWindow(SW_HIDE);
			if (m_kindList == 1)
			{
				m_kindList = 0;
				UpdateData(FALSE);
				OnPOPTIONRdo();
			}

			ShowJOption(FALSE);
			
			m_pCheck->EnableWindow(FALSE);

// 			DisableSpread(FALSE);
// 			HideFutureBtns();
// 			GetDlgItem(IDC_RADIO_FUTURE)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_RADIO_SPREAD)->ShowWindow(SW_SHOW);
// 			m_pCheck->ShowWindow(SW_HIDE);
// 			m_codeList.ShowWindow(SW_SHOW);
// 			m_codeList.SetWindowPos(NULL, checkRC.left, checkRC.top - 25, checkRC.Width(), checkRC.Height() + 50, SWP_NOZORDER);
// 			m_plistbox.EnableWindow();
// 			m_plistbox.SetCurSel(0);
// 			ShowJOption(FALSE);
// 			((CButton*)GetDlgItem(IDC_BUTTON_NAME))->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_CODE1)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_STATIC_NAME1)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_EDIT_CODE)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_EDIT_NAME)->ShowWindow(SW_HIDE);
// 			m_codeList.SetAtmCallIndex(-1);
// 			m_codeList.SetAtmPutIndex(-1);
// 
// 			m_nGubn = 0;
// 			UpdateData(FALSE);
		}
		break;

	}
}

void CFuturesDlg::DisableSpread(BOOL bflag)
{
	GetDlgItem(IDC_BUTTON_SPMONTH3)->EnableWindow(bflag);
	GetDlgItem(IDC_BUTTON_SPMONTH6)->EnableWindow(bflag);
	GetDlgItem(IDC_BUTTON_SPMONTH9)->EnableWindow(bflag);
}

CString CFuturesDlg::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CFuturesDlg::OnButtonJfuture() 
{
	// TODO: Add your control notification handler code here
	// 주식선물...
	
	m_nStep = kfutureCODE;
	Invalidate();
	AdjustControls();
	
	((CButton*)GetDlgItem(IDC_BUTTON_NAME))->ShowWindow(SW_HIDE);

	SetButton();
	
	m_ctlButtonKospi.SetSelected(FALSE);
	m_ctlButtonKostar.SetSelected(FALSE);
	m_ctlButtonJFuture.SetSelected(TRUE);

	CString szTemp;
	int col = 0, row = 0;

	SetSFCodes();

	m_plistbox.SetCurSel(0);
	OnDblclkPcodeList();
// 
// 	m_nStep = kfutureCODE;
// 	AdjustControls();
// 
// 	Invalidate();
// 
// 	m_ctlButtonJFuture.EnableWindow(TRUE);	
// 	m_ctlButtonKostar.SetSelected(FALSE);
// 	m_ctlButtonKospi.SetSelected(FALSE);
// 	m_ctlButtonJFuture.SetSelected(TRUE);
// 	
// 	//LoadSFCode();
// 	m_plistbox.SetCurSel(0);
// 	OnDblclkPcodeList();

}

void CFuturesDlg::SetSFCodes()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	FCODE item;
	int count = m_plistbox.GetCount();
	int ii = 0;

	for (ii = 0; ii < count; ii++)
		m_plistbox.DeleteString(0);

	for (ii = 0; ii < pApp->m_arraySFcode.GetSize(); ii++)
	{
		item = pApp->m_arraySFcode.GetAt(ii);
		if (m_plistbox.FindString(0, item.gnam) == LB_ERR)
			m_plistbox.AddString(item.gnam);
	}
}

void CFuturesDlg::OnRadioFuture()
{
	m_Spread = 0;
	OnDblclkPcodeList();
}

void CFuturesDlg::OnRadioSpread()
{
	m_Spread = 1;
	OnDblclkPcodeList();
}

void CFuturesDlg::Sort()
{
	m_bSort = !m_bSort;
	switch (m_Kind)		// 종류에 따라서..
	{
	case faoptionCODE:
	case optionCODE:
		OnButtonKospi200();
		break;
	case koptionCODE:
		OnDblclkPcodeList();
		break;
	}
}

CArray<CString, CString>* CFuturesDlg::GetArray()
{ 
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	return &pApp->m_ArrayOMonth; 
};

/*
HBRUSH CFuturesDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
