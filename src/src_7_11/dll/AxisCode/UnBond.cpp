// UnBond.cpp : implementation file
//

#include "stdafx.h"
#include "AxisCode.h"
#include "UnBond.h"
#include "afxdialogex.h"
#include "XLEzAutomation.h"
#include "SEdit.h"

#define TAB_HEADER_HEIGHT		24
							
const TCHAR *lpszHeader[] = {			_T("종목명"),			// 0
								_T("지표(구분)"),			// 1
								_T("만기(구분)"),		// 2
								_T("만기일"),	// 3
								_T("금리"),	// 4
								_T("종목코드"),	// 5
								NULL};	

const int colWidthList[] = {				25,					// 0
								15,					// 1
								15,					// 2
								15,
								10,
								0};					// 3

#define GAP 2
// CUnBond dialog
#define LISTCNT 500
IMPLEMENT_DYNAMIC(CUnBond, CDialogEx)

#define btnAll	1
#define btnGuk	2
#define btnGum		3
#define btnHeui	4
#define btnJi  5
#define btnTong		6
#define btnTuk    7
#define btnBal     8
#define btnDang   9
#define btnApply   10
#define btnleft   11
#define btnright   12
#define btnlook   13
#define btnExcel   14

#define axBONDON 90
#define axBONDOFF 91
#define axINTERSETON 92
#define axINTERSETOFF 93
#define axADDINTERSET 94
#define axDELINTERSET 89
#define axRELOADINTER 88

#define TM_START 9999
#define TM_LOAD 8888
#define TM_TITLE 7777

#define CHECKDOWNLOAD 9999

CUnBond* pMain;
DWORD WINAPI ListThread(LPVOID parm_data)
{
	ListThreadData *p_data = (ListThreadData *)parm_data;
	CAxisCodeApp* pApp = (CAxisCodeApp*)p_data->papp;
	CString strtemp;
	CJCODE*  CJCode = NULL;
	CJALLCODE	ncode;
	CXListCtrl* list = NULL;

	pMain = (CUnBond*)p_data->pmain;

	CString strtmps;
	CTime ctstart, ctend;
	ctstart = CTime::GetCurrentTime();
	strtmps.Format(_T("%d %d %d\n"), ctstart.GetHour(), ctstart.GetMinute(), ctstart.GetSecond());
	TRACE(strtmps);

	//pMain->SetWindowText(_T("채권..  종목로드중입니다"));
	pMain->m_UnBListCtrl.DeleteAllItems();
	int iroof = p_data->igubn == 0 ?  p_data->icnt :p_data->igubn  * 100;
	
	pMain->m_section.Lock();
	pMain->m_arrlist.RemoveAll();
	for (int ii = 0; ii < iroof ; ii++)
	{
		bool bSet = false;
		
		
	}
	pMain->m_section.Unlock();

	ctend = CTime::GetCurrentTime();
	strtmps.Format(_T("%d %d %d\n"), ctend.GetHour(), ctend.GetMinute(), ctend.GetSecond());
	TRACE(strtmps);
	pMain->deletethread();
	return 1;
}

void CUnBond::AllListSet()
{
	SearchData();
}

DWORD WINAPI AllListThread(LPVOID parm_data)
{
	ListThreadData *p_data = (ListThreadData *)parm_data;
	CAxisCodeApp* pApp = (CAxisCodeApp*)p_data->papp;
	CString strtemp;
	CJCODE*  CJCode = NULL;
	CJALLCODE	ncode;
	
	pMain = (CUnBond*)p_data->pmain;

	CString strtmps;
	CTime ctstart, ctend;
	ctstart = CTime::GetCurrentTime();
	strtmps.Format(_T("%d %d %d\n"), ctstart.GetHour(), ctstart.GetMinute(), ctstart.GetSecond());
	TRACE(strtmps);

	pMain->SetWindowText(_T("채권..  종목로드중입니다"));
	pMain->m_AllList.DeleteAllItems();
	pMain->m_Search.EnableWindow(FALSE);
	int iroof = p_data->icnt;
	pMain->m_arrlist.RemoveAll();
	pMain->m_section.Lock();
	for (int ii = 0; ii < iroof ; ii++)
	{
		bool bSet = false;
		
	}
	pMain->SetWindowText(_T("채권"));
	ctend = CTime::GetCurrentTime();
	strtmps.Format(_T("%d %d %d\n"), ctend.GetHour(), ctend.GetMinute(), ctend.GetSecond());
	TRACE(strtmps);
	pMain->m_Search.EnableWindow(TRUE);
	
	return 1;
}

int CALLBACK ListViewCompareFunc2(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
{ 
	CString tmpS, tempS;
	_listItem *pItem1 = (_listItem *) lParam1; 
	_listItem *pItem2 = (_listItem *) lParam2; 
	
	int	iCmp=0;
	switch ((int)lParamSort)
	{
		case 1:  //종목명
			iCmp = lstrcmpi((char*)pItem1->Name, (char*)pItem2->Name);
			break;
		case 2:  //지표
			iCmp = lstrcmpi((char*)pItem1->idct, (char*)pItem2->idct);
			break;
		case 3:  //만기
			tmpS.Format(_T("%.6s"), (char*)pItem1->mtry);
			tempS.Format(_T("%.6s"), (char*)pItem2->mtry);
			iCmp = atof(tmpS) > atof(tempS) ? 1: -1;
			break;
		case 4:  //만기일
			tmpS.Format(_T("%.10s"), (char*)pItem1->man);tmpS.Replace("-", "");
			tempS.Format(_T("%.10s"), (char*)pItem2->man);tempS.Replace("-", "");
			iCmp = atoi(tmpS) > atoi(tempS) ? 1: -1;
			break;
		case 5:  //금리
			iCmp = lstrcmpi((char*)pItem1->itrt, (char*)pItem2->itrt);
			break;
		case 6:  //이자유형
			iCmp = lstrcmpi((char*)pItem1->intertype, (char*)pItem2->intertype);
			break;
		case 8:  //민평4
			tmpS.Format(_T("%.10s"), (char*)pItem1->minfore);
			tmpS.TrimLeft();
			tmpS.TrimRight();
			tempS.Format(_T("%.10s"), (char*)pItem2->minfore);
			tempS.TrimLeft();
			tempS.TrimRight();
			iCmp = atof(tmpS) > atof(tempS) ? 1: -1;
			break;
		case 7:  //민평3
			tmpS.Format(_T("%.10s"), (char*)pItem1->minthree);
			tmpS.TrimLeft();
			tmpS.TrimRight();
			tempS.Format(_T("%.10s"), (char*)pItem2->minthree);
			tempS.TrimLeft();
			tempS.TrimRight();
			iCmp = atof(tmpS) > atof(tempS) ? 1: -1;
			break;
		case 9:  //신용등급
			iCmp = lstrcmpi((char*)pItem1->credit, (char*)pItem2->credit);
			break;
		case 10: //종목코드
			iCmp = lstrcmpi((char*)pItem1->Code, (char*)pItem2->Code); 
			break;
		case 11: //발행일
			tmpS.Format(_T("%.10s"), (char*)pItem1->valday);tmpS.Replace("-", "");
			tempS.Format(_T("%.10s"), (char*)pItem2->valday);tempS.Replace("-", "");
			iCmp = atoi(tmpS) > atoi(tempS) ? 1: -1;
			break;
	}

	if (m_sort)
	{
		if (iCmp > 0)
			iCmp = -1;
		else if (iCmp < 0)
			iCmp = 1;
	}
	
	return iCmp;
}
CUnBond::CUnBond(CWnd* pParent)
	: CDialogEx(CUnBond::IDD, pParent)
//	: CSkinDialog(CUnBond::IDD, pParent)  
{
	m_bDraging = FALSE;
	m_pfont = NULL;
	m_iSelectRow = -1;
	m_bDonwlading = false;
}

CUnBond::CUnBond(CPoint pt, CWnd* pParent /*=NULL*/)
	: CDialogEx(CUnBond::IDD, pParent)
//	: CSkinDialog(CUnBond::IDD, pParent)
{
	m_Point = pt;
	m_sort = FALSE;
	m_pMain = NULL;
	m_pfont = NULL;
	m_bDonwlading = false;
}

CUnBond::~CUnBond()
{
}

void CUnBond::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UNBOND, m_UnBListCtrl);
	DDX_Control(pDX, IDC_BTN_GUK, m_btnGuk);
	DDX_Control(pDX, IDC_BTN_GUM, m_btnGum);
	DDX_Control(pDX, IDC_BTN_HEUI, m_btnHeui);
	DDX_Control(pDX, IDC_BTN_JI, m_btnJi);
	DDX_Control(pDX, IDC_BTN_TONG, m_btnTong);
	DDX_Control(pDX, IDC_BTN_TUK, m_btnTuk);
	DDX_Control(pDX, IDC_BTN_BAL, m_btnBal);
	DDX_Control(pDX, IDC_BTN_DANG, m_btnDang);
	DDX_Control(pDX, IDC_BTN_APPLY, m_btnApply);
	DDX_Control(pDX, IDC_BTN_ALL, m_btnAll);
	DDX_Control(pDX, IDC_LIST1, m_InterList);
	DDX_Control(pDX, IDC_EDIT_CODE, m_Search);
	DDX_Control(pDX, IDC_DATE_1Val, m_Date1Val);
	DDX_Control(pDX, IDC_DATE_2VAL, m_Date2Val);
	DDX_Control(pDX, IDC_DATE_1MAN, m_Date1Man);
	DDX_Control(pDX, IDC_DATE_2MAN, m_Date2Man);
	DDX_Control(pDX, IDC_BTN_LEFT, m_btnleft);
	DDX_Control(pDX, IDC_BTN_RIGHT, m_btnright);
	DDX_Control(pDX, IDC_LIST_ALLBOND, m_AllList);
	DDX_Control(pDX, IDC_CHK_MAN, m_ckman);
	DDX_Control(pDX, IDC_CHK_VAL, m_ckval);
	DDX_Control(pDX, IDC_BTN_EXCEL, m_btnexcel);
}


//BEGIN_MESSAGE_MAP(CUnBond, CSkinDialog)
BEGIN_MESSAGE_MAP(CUnBond, CDialogEx)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_EDIT_UNBOND_FIND, &CUnBond::OnChangeEditUnbondFind)
	ON_MESSAGE(WM_ENTERSIZEMOVE, OnEnterSizeMove)
	ON_MESSAGE(WM_EXITSIZEMOVE, OnExitSizeMove)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_APPLY, &CUnBond::OnBnClickedBtnApply)
	ON_MESSAGE(WM_USER + 999, OnAXIS)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CUnBond::OnClickList1)
	ON_EN_CHANGE(IDC_EDIT_CODE, &CUnBond::OnChangeEditCode)
	ON_BN_CLICKED(IDC_CHK_VAL, &CUnBond::OnBnClickedChkVal)
	ON_BN_CLICKED(IDC_CHK_MAN, &CUnBond::OnBnClickedChkMan)
	ON_BN_CLICKED(IDC_BTN_LEFT, &CUnBond::OnBnClickedBtnLeft)
	ON_BN_CLICKED(IDC_BTN_RIGHT, &CUnBond::OnBnClickedBtnRight)
	ON_BN_CLICKED(IDC_BTN_ALL, &CUnBond::OnBnClickedBtnAll)
	ON_BN_CLICKED(IDC_BTN_GUK, &CUnBond::OnBnClickedBtnGuk)
	ON_BN_CLICKED(IDC_BTN_TONG, &CUnBond::OnBnClickedBtnTong)
	ON_BN_CLICKED(IDC_BTN_TUK, &CUnBond::OnBnClickedBtnTuk)
	ON_BN_CLICKED(IDC_BTN_JI, &CUnBond::OnBnClickedBtnJi)
	ON_BN_CLICKED(IDC_BTN_GUM, &CUnBond::OnBnClickedBtnGum)
	ON_BN_CLICKED(IDC_BTN_HEUI, &CUnBond::OnBnClickedBtnHeui)
	ON_BN_CLICKED(IDC_BTN_DANG, &CUnBond::OnBnClickedBtnDang)
	ON_BN_CLICKED(IDC_BTN_BAL, &CUnBond::OnBnClickedBtnBal)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ALLBOND, &CUnBond::OnDblclkListAllbond)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_UNBOND, &CUnBond::OnDblclkListUnbond)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_STRIP, &CUnBond::OnBnClickedCheckStrip)
	ON_MESSAGE(WM_BTNMSG, OnMessage)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_UNBOND, &CUnBond::OnCustomdrawListUnbond)
	ON_NOTIFY(NM_CLICK, IDC_LIST_UNBOND, &CUnBond::OnClickListUnbond)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_ALLBOND, &CUnBond::OnCustomdrawListAllbond)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ALLBOND, &CUnBond::OnClickListAllbond)
	ON_BN_CLICKED(IDC_BTN_EXCEL, &CUnBond::OnBnClickedBtnExcel)
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ALLBOND, &CUnBond::OnRclickListAllbond)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_UNBOND, &CUnBond::OnRclickListUnbond)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_1MAN, &CUnBond::OnDatetimechangeDate1man)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_1Val, &CUnBond::OnDatetimechangeDate1val)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_2MAN, &CUnBond::OnDatetimechangeDate2man)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_2VAL, &CUnBond::OnDatetimechangeDate2val)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CUnBond message handlers


void CUnBond::Setdatapointer(char* pdata)
{
	//memset(m_pcode, 0, 50);
	m_pcode = pdata;
}

void CUnBond::OnDblclkListAllbond(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int item = m_AllList.GetSelectionMark();

	CString tmpS;

	m_szCode = m_szName = "";
	m_szCode = m_AllList.GetItemText(item, 10);
	m_szName = m_AllList.GetItemText(item, 1);
	OnOK();	

	*pResult = 0;
}


void CUnBond::OnDblclkListUnbond(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int item = m_UnBListCtrl.GetSelectionMark();

	CString tmpS;

	if(!m_bINTERMODE)
	{
		m_szCode = m_szName = "";

		int item = m_UnBListCtrl.GetSelectionMark();
	
		m_szCode = m_UnBListCtrl.GetItemText(item, 10);
		m_szName = m_UnBListCtrl.GetItemText(item, 1);
		OnOK();	
	}
	else
	{
		if(m_pMain != NULL)
		{
			tmpS.Format(_T("%s\t%s\t%s\t%s"), m_UnBListCtrl.GetItemText(item, 1),
											m_UnBListCtrl.GetItemText(item, 7),
											m_UnBListCtrl.GetItemText(item, 5),
											m_UnBListCtrl.GetItemText(item, 11));
			m_pMain->SendMessage(WM_USER + 500, axADDINTERSET,  (LPARAM)tmpS.operator LPCSTR());
		}
	}
	*pResult = 0;
}


void CUnBond::OnDestroy()
{
//	CSkinDialog::OnDestroy();
	CDialogEx::OnDestroy();
}

void CUnBond::OnChangeEditUnbondFind()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString strName;
	//m_gListsort = FALSE;
	//ListView_SortItems(m_UnBListCtrl.GetSafeHwnd(), ListViewCompareFunc2, 1);

	GetDlgItem(IDC_EDIT_UNBOND_FIND)->GetWindowText(strName);
	strName.TrimRight();
	int n = strName.GetLength();

}

LRESULT CUnBond::OnMessage(WPARAM wParam, LPARAM lParam)
{
	if(m_bDonwlading)
		return -1;

	switch(wParam)
	{
		case btnAll	:  
			OnBnClickedBtnAll();
			break;
		case btnGuk:  
			OnBnClickedBtnGuk();
			break;
		case btnGum:     
			OnBnClickedBtnGum();
			break;
		case btnHeui:  
			OnBnClickedBtnHeui();
			break;
		case btnJi: 
			OnBnClickedBtnJi();
			break;
		case btnTong:     
			OnBnClickedBtnTong();
			break;
		case btnTuk:
			OnBnClickedBtnTuk();
			break;
		case btnBal:  
			OnBnClickedBtnBal();
			break;
		case btnDang:    
			OnBnClickedBtnDang();
			break;
		case btnApply: 
			SearchData(true);
			break;
		case btnlook: 
			OnBnClickedBtnApply();
		//	m_btnApply.m_push = false;
			break;
		case btnExcel: 
			{
			
			}
		//	OnBnClickedBtnExcel();
		//	m_btnexcel.m_push = false;
			break;
	}

	switch(m_typecode)
	{
		case ALLCODE:
			m_btnAll.m_push = true;
			m_btnGuk.m_push = false;
			m_btnGum.m_push = false;
			m_btnHeui.m_push = false;
			m_btnJi.m_push = false;
			m_btnTong.m_push = false;
			m_btnTuk.m_push = false;
			m_btnBal.m_push = false;
			m_btnDang.m_push = false;
			break;
		case GUKCODE:
			m_btnAll.m_push = false;
			m_btnGuk.m_push = true;
			m_btnGum.m_push = false;
			m_btnHeui.m_push = false;
			m_btnJi.m_push = false;
			m_btnTong.m_push = false;
			m_btnTuk.m_push = false;
			m_btnBal.m_push = false;
			m_btnDang.m_push = false;
			break;
		case TONGCODE:
			m_btnAll.m_push = false;
			m_btnGuk.m_push = false;
			m_btnGum.m_push = false;
			m_btnHeui.m_push = false;
			m_btnJi.m_push = false;
			m_btnTong.m_push = true;
			m_btnTuk.m_push = false;
			m_btnBal.m_push = false;
			m_btnDang.m_push = false;
			break;
		case TUKCODE:
			m_btnAll.m_push = false;
			m_btnGuk.m_push = false;
			m_btnGum.m_push = false;
			m_btnHeui.m_push = false;
			m_btnJi.m_push = false;
			m_btnTong.m_push = false;
			m_btnTuk.m_push = true;
			m_btnBal.m_push = false;
			m_btnDang.m_push = false;
			break;
		case JICODE:
			m_btnAll.m_push = false;
			m_btnGuk.m_push = false;
			m_btnGum.m_push = false;
			m_btnHeui.m_push = false;
			m_btnJi.m_push = true;
			m_btnTong.m_push = false;
			m_btnTuk.m_push = false;
			m_btnBal.m_push = false;
			m_btnDang.m_push = false;
			break;
		case GUMCODE:
			m_btnAll.m_push = false;
			m_btnGuk.m_push = false;
			m_btnGum.m_push = true;
			m_btnHeui.m_push = false;
			m_btnJi.m_push = false;
			m_btnTong.m_push = false;
			m_btnTuk.m_push = false;
			m_btnBal.m_push = false;
			m_btnDang.m_push = false;	
			break;
		case WHECODE:
			m_btnAll.m_push = false;
			m_btnGuk.m_push = false;
			m_btnGum.m_push = false;
			m_btnHeui.m_push = true;
			m_btnJi.m_push = false;
			m_btnTong.m_push = false;
			m_btnTuk.m_push = false;
			m_btnBal.m_push = false;
			m_btnDang.m_push = false;	
			break;	
		case DANGCODE:
			m_btnAll.m_push = false;
			m_btnGuk.m_push = false;
			m_btnGum.m_push = false;
			m_btnHeui.m_push = false;
			m_btnJi.m_push = false;
			m_btnTong.m_push = false;
			m_btnTuk.m_push = false;
			m_btnBal.m_push = false;
			m_btnDang.m_push = true;
			break;
		case BALCODE:
			m_btnAll.m_push = false;
			m_btnGuk.m_push = false;
			m_btnGum.m_push = false;
			m_btnHeui.m_push = false;
			m_btnJi.m_push = false;
			m_btnTong.m_push = false;
			m_btnTuk.m_push = false;
			m_btnBal.m_push = true;
			m_btnDang.m_push = false;
			break;
	}
	return 0;
}

BOOL CUnBond::OnInitDialog()
{
	//CSkinDialog::OnInitDialog();
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	m_typecode = -1;
	m_bINTERMODE = FALSE;
	// TODO:  Add extra initialization here
	SetDlgPos(m_Point.x, m_Point.y);
	m_sRoot = ((CAxisCodeApp*)AfxGetApp())->m_RootPath;
	// Image List
	m_pImageList = new CImageList();		
	if(!m_pImageList->Create( 16, 16, ILC_COLOR32, 0, 1))
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
	else 
	{
		m_pImageList->Add(Global.GetIcon(IDX_ICON_HOME,ICON16));
		m_pImageList->Add(Global.GetIcon(IDX_ICON_FAVORITES,ICON16));
		m_pImageList->Add(Global.GetIcon(IDX_ICON_INTERNET,ICON16));
		m_pImageList->Add(Global.GetIcon(IDX_ICON_SETTING,ICON16));
	}

	//버튼 초기화
	InitBtn();

	// List 초기화
	CString str(_T(""));
	InitInterList();

	m_AllList.ShowWindow(SW_HIDE);
	LoadCJCode();
	m_UnBListCtrl.GetClientRect(&m_rcList);
	Reposition();

	m_pCtlBrush =  new CBrush();
	m_pCtlBrush->CreateSolidBrush(RGB(249,249,249));
	
	m_pInterBrusl=  new CBrush();
	m_pInterBrusl->CreateSolidBrush(RGB(252,255,255));

	SetTimer(TM_START, 500, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
void CUnBond::WriteLastbtn()
{
	switch(m_typecode)
	{
		case ALLCODE	:  
		case GUKCODE:  
		case TONGCODE:     
		case TUKCODE:  
		case JICODE:
		case GUMCODE: 
		case WHECODE:     
		case DANGCODE:
		case BALCODE:  
		case INTERCODE:    
			{
				//m_typecode
				CString strVal, strFilePath, strUserPath;
				strUserPath.Format(_T("%s\\user\\%s"), m_sRoot, m_sUser);
				strFilePath.Format(_T("%s\\%s"), strUserPath, _T("userinterest.ini"));
				strVal.Format(_T("%d"), m_typecode);
				WritePrivateProfileString(_T("GROUPINFO"), _T("lastbtn_dlg"), (LPCTSTR)strVal, (LPCTSTR)strFilePath);
			}
			break;
	}
	return ;

}
void CUnBond::RepositionBtn(CNButton* pbtn, CNButton* pupbtn)
{
	CRect tmpRc, rc;
	pbtn->GetWindowRect(tmpRc);
	tmpRc.right = tmpRc.left + 80;

	if(pupbtn == NULL)
		tmpRc.bottom = tmpRc.top + 25;
	else
	{
		pupbtn->GetWindowRect(rc);
		tmpRc.top = rc.bottom + GAP;
		tmpRc.bottom = tmpRc.top + 25;
	}

	ScreenToClient(tmpRc);
	pbtn->MoveWindow(tmpRc);
}

void CUnBond::InitBtn()
{
	
	CString	fontName;
	fontName = _T("굴림");
	m_pfont = new CFont;
	m_pfont->CreateFont(	12,            // nHeight
				0,                         // nWidth
				0,                         // nEscapement
				0,                         // nOrientation
				FW_NORMAL,	           // nWeight
				FALSE,                     // bItalic
				FALSE,                     // bUnderline
				0,                         // cStrikeOut
				DEFAULT_CHARSET,           // nCharSet
				OUT_DEFAULT_PRECIS,        // nOutPrecision
				CLIP_DEFAULT_PRECIS,       // nClipPrecision
				DEFAULT_QUALITY,           // nQuality
				DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				fontName);       

	CString imgC, imgO, imgD;
	
	CRect rc, tmpRc, rclist;
	GetClientRect(&rc);
	
	tmpRc.left = rc.left + 3;
	tmpRc.top = rc.top + 3;
	tmpRc.right = tmpRc.left + rc.right - 9;
	tmpRc.bottom = tmpRc.top + 52;

	m_pPanel = new CPanel;
	m_pPanel->m_pFont = m_pfont;
	if (!m_pPanel->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, tmpRc, this, 9898))
		delete m_pPanel;

	m_btnAll.set_ToolTipText(_T("전체"));
	imgC.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_down");
	m_btnAll.set_Image(imgC, imgO, imgD);
	m_btnAll.SetItemFont(m_pfont);
	m_btnAll.m_fcolor = RGB(0, 0, 0);
	m_btnAll.m_text = _T("전체");
	m_btnAll.m_id = btnAll;
	RepositionBtn(&m_btnAll, NULL);

	m_btnGuk.set_ToolTipText(_T("국채"));
	imgC.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_down");
	m_btnGuk.set_Image(imgC, imgO, imgD);
	m_btnGuk.SetItemFont(m_pfont);
	m_btnGuk.m_fcolor = RGB(0, 0, 0);
	m_btnGuk.m_text = _T("국채");
	m_btnGuk.m_id = btnGuk;
	RepositionBtn(&m_btnGuk, &m_btnAll);

	m_btnTong.set_ToolTipText(_T("통안채"));
	imgC.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_down");
	m_btnTong.set_Image(imgC, imgO, imgD);
	m_btnTong.SetItemFont(m_pfont);
	m_btnTong.m_fcolor = RGB(0, 0, 0);
	m_btnTong.m_text = _T("통안채");
	m_btnTong.m_id = btnTong;
	RepositionBtn(&m_btnTong, &m_btnGuk);

	m_btnGum.set_ToolTipText(_T("금융채"));
	imgC.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_down");
	m_btnGum.set_Image(imgC, imgO, imgD);
	m_btnGum.SetItemFont(m_pfont);
	m_btnGum.m_fcolor = RGB(0, 0, 0);
	m_btnGum.m_text = _T("금융채");
	m_btnGum.m_id = btnGum;
	RepositionBtn(&m_btnGum, &m_btnTong);

	m_btnTuk.set_ToolTipText(_T("특수채"));
	imgC.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_down");
	m_btnTuk.set_Image(imgC, imgO, imgD);
	m_btnTuk.SetItemFont(m_pfont);
	m_btnTuk.m_fcolor = RGB(0, 0, 0);
	m_btnTuk.m_text = _T("특수채");
	m_btnTuk.m_id = btnTuk;
	RepositionBtn(&m_btnTuk, &m_btnGum);

	m_btnJi.set_ToolTipText(_T("지방채"));
	imgC.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_down");
	m_btnJi.set_Image(imgC, imgO, imgD);
	m_btnJi.SetItemFont(m_pfont);
	m_btnJi.m_fcolor = RGB(0, 0, 0);
	m_btnJi.m_text = _T("지방채");
	m_btnJi.m_id = btnJi ;
	RepositionBtn(&m_btnJi, &m_btnTuk);
	
	m_btnHeui.set_ToolTipText(_T("회사채"));
	imgC.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_down");
	m_btnHeui.set_Image(imgC, imgO, imgD);
	m_btnHeui.SetItemFont(m_pfont);
	m_btnHeui.m_fcolor = RGB(0, 0, 0);
	m_btnHeui.m_text = _T("회사채");
	m_btnHeui.m_id = btnHeui;
	RepositionBtn(&m_btnHeui, &m_btnJi);

	m_btnBal.set_ToolTipText(_T("발행예정종목"));
	imgC.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_down");
	m_btnBal.set_Image(imgC, imgO, imgD);
	m_btnBal.SetItemFont(m_pfont);
	m_btnBal.m_fcolor = RGB(0, 0, 0);
	m_btnBal.m_text = _T("발행예정종목");
	m_btnBal.m_id = btnBal;
	RepositionBtn(&m_btnBal, &m_btnHeui);

	m_btnDang.set_ToolTipText(_T("당일발행물"));
	imgC.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_65_down");
	m_btnDang.set_Image(imgC, imgO, imgD);
	m_btnDang.SetItemFont(m_pfont);
	m_btnDang.m_fcolor = RGB(0, 0, 0);
	m_btnDang.m_text = _T("당일발행물");
	m_btnDang.m_id = btnDang;
	RepositionBtn(&m_btnDang, &m_btnBal);

	m_btnApply.set_ToolTipText(_T("조회"));
	imgC.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_Act_50");
	imgO.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_Act_50_EN");
	imgD.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_Act_50_DN");
	m_btnApply.set_Image(imgC, imgO, imgD);
	m_btnApply.SetItemFont(m_pfont);
	m_btnApply.m_fcolor = RGB(249, 249, 249);
	m_btnApply.m_text = _T("조회");
	m_btnApply.m_id = btnApply;   
	m_btnApply.m_kind = 1;

	m_btnexcel.set_ToolTipText(_T("엑셀다운"));
	imgC.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_Act_50");
	imgO.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_Act_50_EN");
	imgD.Format("%s\\%s\\%s.bmp", m_sRoot, _T("image"), "BTN_Act_50_DN");
	m_btnexcel.set_Image(imgC, imgO, imgD);
	m_btnexcel.SetItemFont(m_pfont);
	m_btnexcel.m_fcolor = RGB(249, 249, 249);
	m_btnexcel.m_text = _T("엑셀다운");
	m_btnexcel.m_id = btnExcel;  
	m_btnexcel.m_kind = 1;
	m_btnexcel.ShowWindow(SW_HIDE);
}

void CUnBond::FilAllList_thread()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int codeN = (int)pApp->m_arrayCJCode.GetSize();
	int idata = 0;

	m_thread_data.icnt = codeN;
	m_thread_data.papp = pApp;
	m_thread_data.pmain = this;
	m_thread_data.igubn = ALLCODE;
	h_Allthread = CreateThread(NULL, 0, AllListThread, (LPVOID)&m_thread_data, 0, NULL);
	SetThreadPriority(h_Allthread, THREAD_PRIORITY_HIGHEST);
}

void CUnBond::FillListdata_thread(int igubn)
{
	if(h_thread != NULL)
		return;

	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int codeN = (int)pApp->m_arrayCJCode.GetSize();
	int idata = 0;

	m_thread_data.icnt = codeN;
	m_thread_data.papp = pApp;
	m_thread_data.pmain = this;
	m_thread_data.igubn = igubn;
	h_thread = CreateThread(NULL, 0, ListThread, (LPVOID)&m_thread_data, 0, NULL);
	SetThreadPriority(h_thread, THREAD_PRIORITY_HIGHEST);
}

BOOL CUnBond::LoadCJCode()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
//	int	codeN;

	m_arrlist.Copy(pApp->m_arrlist);
	return TRUE;
}

void CUnBond::SetDlgPos(int x, int y)
{
	if(x < 0 || y < 0)
		return;

	int scWidth, scHeight;
	scWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CRect rc;
	GetClientRect(&rc);

	int gabX = scWidth - (x - rc.Width());
	int	gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(NULL, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}


BOOL CUnBond::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	static int row;
	if(wParam == IDC_LIST_UNBOND || wParam == IDC_LIST_ALLBOND)
	{
		switch(((LPNMHDR) lParam)->code)
 		{
 			case LVN_COLUMNCLICK:
 			#define pnm ((LPNMLISTVIEW)lParam)
			switch ((LPARAM)pnm->iSubItem)
			{
 				case 1:
 					m_sort = !m_sort;
 					break;
 				case 7:
 					m_sort = !m_sort;
					break;
				case 4:
 					m_sort = !m_sort;
					break;
				case 11:
 					m_sort = !m_sort;
					break;
				default:
					m_sort = !m_sort;
					break;
 			}
			
			ListView_SortItems((HWND)pnm->hdr.hwndFrom, ListViewCompareFunc2, (LPARAM)pnm->iSubItem);
 			
 			#undef pnm

			break;
 		}
	}

	return CDialogEx::OnNotify(wParam, lParam, pResult); 
}


BOOL CUnBond::FindCode(CString szCode)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int nCount = (int)pApp->m_arrayCJCode.GetSize();
	CJcode *code;
	CString tmps;
	
	for (int ii = 0; ii < nCount; ii++)
	{
		code = pApp->m_arrayCJCode.GetAt(ii);

		tmps.Format(_T("%.12s"), code->code);
		szCode.MakeUpper();
		if (szCode == tmps)
		{
			SetName(code->name);
			return TRUE;
		}
	}

	return FALSE;
}

void CUnBond::Set_KS_HanMode()
{
	HIMC himc = ImmGetContext(GetSafeHwnd());
	ImmSetConversionStatus(himc, IME_CMODE_NATIVE, IME_SMODE_CONVERSATION);
	ImmReleaseContext(GetSafeHwnd(), himc); 
}


void CUnBond::InitList(CXListCtrl &list, const TCHAR**pszTitle)
{
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
	list.SetRowHeight(15);
	// 리스트 폰트 설정
	list.SetTextFont( Global.GetFont(IDX_FONT_MEDIUM) );
	// 언더라인 컬러 설정
	list.SetColorUnderLine( RGB(240,240,240) );

	// 헤더 높이 설정
	list.m_HeaderCtrl.SetHeight(30);
	// 헤더 폰트 설정
	list.m_HeaderCtrl.SetTextFont( Global.GetFont(IDX_FONT_SMALL) );
}

void CUnBond::InitInterList()
{
	CRect rc;
	//m_UnBListCtrl
	m_UnBListCtrl.GetClientRect(&rc);
	m_UnBListCtrl.SetExtendedStyle(m_UnBListCtrl.GetExtendedStyle() | LVS_EX_CHECKBOXES |  LVS_EX_FULLROWSELECT);
	m_UnBListCtrl.InsertColumn(0,_T(" "), LVCFMT_CENTER, 0);
	m_UnBListCtrl.InsertColumn(1, _T("             종목명"), LVCFMT_LEFT, 155);
	m_UnBListCtrl.InsertColumn(2, _T(" 지표"), LVCFMT_LEFT, 45);
	m_UnBListCtrl.InsertColumn(3, _T("만기 "), LVCFMT_RIGHT, 45);
	m_UnBListCtrl.InsertColumn(4, _T("만기일"), LVCFMT_CENTER, 80);
	m_UnBListCtrl.InsertColumn(5, _T("금리"), LVCFMT_CENTER, 60);
	m_UnBListCtrl.InsertColumn(6, _T("이자유형"), LVCFMT_CENTER, 60);
	m_UnBListCtrl.InsertColumn(7, _T("민평(3사)"), LVCFMT_CENTER, 70);
	m_UnBListCtrl.InsertColumn(8, _T("민평(4사)"), LVCFMT_CENTER, 70);
	m_UnBListCtrl.InsertColumn(9, _T("신용등급"), LVCFMT_CENTER,70);
	m_UnBListCtrl.InsertColumn(10, _T("종목코드"), LVCFMT_CENTER, 100);
	m_UnBListCtrl.InsertColumn(11, _T("발행일"), LVCFMT_CENTER, 100);
	m_UnBListCtrl.InsertColumn(12, _T("업데이트시각"), LVCFMT_CENTER, 0);

	//m_AllList
	m_AllList.GetClientRect(&rc);
	m_AllList.SetExtendedStyle(m_UnBListCtrl.GetExtendedStyle() | LVS_EX_CHECKBOXES |  LVS_EX_FULLROWSELECT);
	m_AllList.InsertColumn(0,_T(" "), LVCFMT_CENTER, 0);
	m_AllList.InsertColumn(1, _T("             종목명"), LVCFMT_LEFT, 155);
	m_AllList.InsertColumn(2, _T(" 지표"), LVCFMT_LEFT, 45);
	m_AllList.InsertColumn(3, _T("만기 "), LVCFMT_RIGHT, 45);
	m_AllList.InsertColumn(4, _T("만기일"), LVCFMT_CENTER, 80);
	m_AllList.InsertColumn(5, _T("금리"), LVCFMT_CENTER, 50);
	m_AllList.InsertColumn(6, _T("이자유형"), LVCFMT_CENTER, 60);
	m_AllList.InsertColumn(7, _T("민평(3사)"), LVCFMT_CENTER, 70);
	m_AllList.InsertColumn(8, _T("민평(4사)"), LVCFMT_CENTER, 70);
	m_AllList.InsertColumn(9, _T("신용등급"), LVCFMT_CENTER,70);
	m_AllList.InsertColumn(10, _T("종목코드"), LVCFMT_CENTER, 100);
	m_AllList.InsertColumn(11, _T("발행일"), LVCFMT_CENTER, 100);
	m_UnBListCtrl.InsertColumn(12, _T("업데이트시각"), LVCFMT_CENTER, 0);

	//m_InterList
	m_InterList.GetClientRect(&rc);
	m_InterList.SetExtendedStyle(m_InterList.GetExtendedStyle() |  LVS_EX_FULLROWSELECT);
	m_InterList.InsertColumn(0,_T(" "), LVCFMT_CENTER, rc.Width());

	m_sRoot = ((CAxisCodeApp*)AfxGetApp())->m_RootPath;
	m_sUser = ((CAxisCodeApp*)AfxGetApp())->m_Name;

	LoadGroupOfFile();
}


CString CUnBond::Parser(CString &strSrc, CString strSub /* = _T("\t") */)
{
	if (strSrc.Find(strSub) == -1)
	{
		CString temp = strSrc;
		strSrc.Empty();
			return temp;
	}
	else
	{
		CString  strTemp = strSrc.Left(strSrc.Find(strSub));
		strSrc = strSrc.Mid(strSrc.Find(strSub) + strSub.GetLength());
		return strTemp;
	}

	return _T("");
}


void CUnBond::LoadGroupOfFile()
{
	h_thread = NULL;
	m_bWorking = false;
	CString strUserPath(_T(""));
	CString strFilePath(_T(""));
	m_mapGroupCodemap.RemoveAll();
	m_arrGroup.RemoveAll();
	
	strUserPath.Format(_T("%s\\user\\%s"), m_sRoot, m_sUser);

	CFile file;
	CFileException ex;
	CStringArray* pArr = NULL;

	strFilePath.Format(_T("%s\\%s"), strUserPath, _T("userinterest.ini"));

	int nGrpCnt = GetPrivateProfileInt(_T("GROUPINFO"), _T("COUNT"), 0, (LPCTSTR)strFilePath);

	CString sGroup(_T(""));
	CString sGrpData(_T(""));
	CString strGroup(_T(""));

	m_InterList.DeleteAllItems();
	
	for ( int i = 0 ; i < nGrpCnt ; i ++ )
	{
		sGroup.Format(_T("%d"), i);

		GetPrivateProfileString(_T("GROUPINFO"), _T(sGroup), _T(""), sGrpData.GetBufferSetLength(1024), 1024, (LPCTSTR)strFilePath);
		sGrpData.ReleaseBuffer();

		_TRIMDATA(sGrpData);
		if(sGrpData.IsEmpty())
			continue;

		strGroup = Parser(sGrpData, _T(";"));
		strGroup = sGrpData;

		AddGroup(strGroup);

		LoadFile(strGroup);
	}

	ReflashSelectList();
}

void CUnBond::ReflashSelectList()
{
	CArray<CCodeData, CCodeData>* ptrArr = NULL;
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	CStringArray arr;
	CString	 strkey, tmpS, strtemp;
	//POSITION pos, tmpos;
	
	m_InterList.DeleteAllItems();
	for(int ii = 0 ; ii < m_arrGroup.GetSize() ; ii++)
		arr.Add(m_arrGroup.GetAt(ii));
			
	for(int ii = 0 ; ii < arr.GetCount() ; ii++)
	{
		strkey = arr.GetAt(ii);
		int iItem = m_InterList.GetItemCount();
		m_InterList.InsertItem(iItem, arr.GetAt(ii));	
	}
		
}

void CUnBond::LoadFile(CString strgroup)
{
	CString strUserPath(_T(""));
	CString strFilePath(_T(""));


	strUserPath.Format("%s\\user\\%s\\%s", m_sRoot, m_sUser, strgroup);

	CString strData(_T(""));

	CFile file;

	CFileException ex;

	int nTry = 0;
	
	while(1)
	{
		if (file.Open(strUserPath, CFile::modeRead | CFile::shareDenyWrite, &ex))
		{
			int nFileSize = (int)file.GetLength();

			char* buffer = new char[nFileSize + 1];

			if ( nFileSize > 0 )
			{
				ZeroMemory(buffer, nFileSize + 1);

				FillMemory(buffer, nFileSize + 1, 0L);

				file.Read(buffer, nFileSize + 1);

				strData.Format("%s", buffer);
			}

			delete []buffer;

			file.Close();

			break;
		}
		else
		{
			if (nTry >= 10)
				break;

			Sleep(10);

			nTry ++;
		}
	}

	CString sTmp(_T(""));

	int ncnt = 0;

	ncnt = 0;

	CString strCode(_T(""));

	CString strCodeName(_T(""));

	CString strMangi(_T(""));

	CString strsuik(_T(""));

	CString strName(_T(""));

	CString strCodeData(_T(""));

	CString stmp(_T(""));

	CString strCount(_T(""));

	for ( int i = 0 ; i < 80 ; i ++ )
	{
		if (strData.IsEmpty())
			break;

		strCodeData = Parser(strData, _T("\n"));
		strCode = Parser(strCodeData, _T("\t"));
		strName = Parser(strCodeData, _T("\t"));
		strsuik = Parser(strCodeData, _T("\t"));
		strMangi = Parser(strCodeData, _T("\t"));
		_TRIMDATA(strCode);

		stmp.Format(_T("%s"), strCode);
		strCodeName = _T("");
		AddListOfCodeMap(strgroup, stmp, strName, strsuik, strMangi);
		ncnt++;
	}

}

void CUnBond::AddListOfCodeMap(CString strgroup, CString sCode, CString sName, CString strsuik, CString strMangi)
{
	CArray<CCodeData, CCodeData>* ptrArr = NULL;
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	CString	 strkey;
	POSITION pos;

	for (pos = m_mapGroupCodemap.GetStartPosition(); pos; )
	{
		m_mapGroupCodemap.GetNextAssoc(pos, strkey, pmap);
		if(strkey == strgroup)
		{
			CCodeData cd;
			cd.strCode = sCode;
			cd.strName = sName;
			cd.strsuik = strsuik;
			cd.strMangi = strMangi;
			pmap->SetAt(sCode, cd);
		}
	}
}

void CUnBond::addlistGroup(CString strgroup)
{
	CString tmpS;
	int iItem = m_InterList.GetItemCount();
	tmpS.Format(_T("%d"), iItem);

	m_InterList.InsertItem(0, strgroup);
}


void CUnBond::AddGroup(CString strGroup)
{
	CString strIndex(_T(""));
	
	addlistGroup(strGroup);
	m_arrGroup.Add(strGroup);

	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	if(!m_mapGroupCodemap.Lookup(strGroup, pmap))
	{
		pmap =  new CMap<CString, LPCTSTR, CCodeData, CCodeData>;
		m_mapGroupCodemap.SetAt(strGroup, pmap);
	}
}


void CUnBond::RelocationControls()
{
	
}

void CUnBond::ResizeListColumn(CXListCtrl &list, const int *arrWidth)
{
	SCROLLINFO si;
	CRect rc;
	list.GetWindowRect(&rc);
	int nMargin=0;

	//스크롤 유무에 따른 마진 추가
	list.GetScrollInfo(SB_VERT, &si);
	if(si.nPage && si.nPage <= (UINT)si.nMax)
		nMargin = ::GetSystemMetrics(SM_CXVSCROLL);

	//보더 유무에 따른 마진 추가
	DWORD dwExStyle = list.GetExStyle();
	if(dwExStyle & WS_EX_STATICEDGE || dwExStyle & WS_EX_CLIENTEDGE)
		nMargin += ::GetSystemMetrics(SM_CXEDGE)*2;

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


LRESULT CUnBond::OnEnterSizeMove(WPARAM wParam, LPARAM lParam)
{
	/*
	if( !m_bDraging )
	{
		m_bDraging = TRUE;
		::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
		Global.SetLayeredWindowAttributes(m_hWnd, 0, 176, LWA_ALPHA);
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	}
	*/
	return 0L;
}


LRESULT CUnBond::OnExitSizeMove(WPARAM wParam, LPARAM lParam)
{
	/*
	if( m_bDraging )
	{
		m_bDraging = FALSE;
		::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	}
	*/
	 return 0L;
}


BOOL CUnBond::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
#if 1
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(rc, Global.GetRGB(IDX_RGB_BACKGROUND));	
	return TRUE;
#else
	return TRUE;
#endif
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CUnBond::OnBnClickedBtnApply()
{
	SearchData();
}


BOOL CUnBond::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_CLOSE)
	{
		CString strdata;
		return -1;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


LRESULT CUnBond::OnAXIS(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case axBONDON:
		{
			m_bINTERMODE = TRUE;
			m_pMain = (CWnd*)lParam;
			Reposition();
		}
		break;
	case axBONDOFF:
			m_bINTERMODE = FALSE;
		break;
	case axRELOADINTER:
		{
			LoadGroupOfFile();
		}
		break;
	}

	return 1;
}

void CUnBond::PreSubclassWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CDialogEx::PreSubclassWindow();
}


void CUnBond::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_UnBListCtrl
	CString tmpS;
	tmpS.Format(_T("click list %s\n"), m_InterList.GetItemText(pNMItemActivate->iItem, 0));
	//m_pMain->SendMessage(WM_USER + 670, 0,  (LPARAM)tmpS.operator LPCSTR());
	if(h_thread != NULL && !m_bWorking)
		return;
	
	tmpS = m_InterList.GetItemText(pNMItemActivate->iItem, 0);
	FillCodelistByGroup(tmpS);
	m_typecode = INTERCODE;
	m_szIntergroup = tmpS;

	//m_pMain->SendMessage(WM_USER + 670, 0,  (LPARAM)m_szIntergroup.operator LPCSTR());
	SetButtonPress(INTERCODE);
	*pResult = 0;
}

bool CUnBond::GetCodeData(CString strcode, CJcode *code)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();

	int icnt = pApp->m_arrayCJCode.GetSize();

	CJcode *pcode;
	CString tmps;
	
	for (int ii = 0; ii < icnt; ii++)
	{
		pcode = pApp->m_arrayCJCode.GetAt(ii);

		tmps.Format(_T("%.12s"), pcode->code);
		strcode.MakeUpper();
		if (strcode == tmps.MakeUpper())
		{
			memcpy((struct CJcode*)code, pcode, sizeof(struct CJcode));
			return true;
		}
	}

	return false;
}

CString CUnBond::GetCodeData(CString strcode)
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();

	int icnt = pApp->m_arrayCJCode.GetSize();

	CJcode *code;
	CString tmps;
	
	for (int ii = 0; ii < icnt; ii++)
	{
		code = pApp->m_arrayCJCode.GetAt(ii);

		tmps.Format(_T("%.12s"), code->code);
		strcode.MakeUpper();
		if (strcode == tmps.MakeUpper())
		{
			
		}
	}

	return _T("");
}

void CUnBond::FillCodelistByGroup(CString strgroup)
{
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	CString	 strkey, tmpS;
	POSITION pos;
	m_bWorking = true;
	int i = 0;
	if(m_mapGroupCodemap.Lookup(strgroup, pmap))
	{
		m_UnBListCtrl.DeleteAllItems();
		int iItem = m_UnBListCtrl.GetItemCount();
		tmpS.Format(_T("%d"), iItem);
		int ii = 0;
		CCodeData cd;
		CJCODESTR* cjcode = NULL;

		for (pos = pmap->GetStartPosition(); pos;)
		{
			cjcode = new CJCODESTR;
			pmap->GetNextAssoc(pos, strkey, cd);
			m_UnBListCtrl.InsertItem(ii,   _T(""));

			m_UnBListCtrl.SetItemText(ii, 1, cd.strName);    //종목명
			cjcode->name =  cd.strName;

			CJcode *pcode = new CJcode;
			memset(pcode, 0x00, sizeof(struct CJcode));
			if(!GetCodeData(cd.strCode.TrimRight(),pcode))
				break;

			m_UnBListCtrl.SetItemText(ii, 2, pcode->idct);   //지표
			cjcode->mtry = cd.strMangi;

			m_UnBListCtrl.SetItemText(ii, 3, pcode->mtry);  //만기
			cjcode->mtry = cd.strMangi;

			m_UnBListCtrl.SetItemText(ii, 4, pcode->mtday);   //만기일
			cjcode->idct.Format(_T("%s"), pcode->mtday);

			m_UnBListCtrl.SetItemText(ii, 5, pcode->itrt);    //금리
			cjcode->itrt.Format(_T("%s"), pcode->itrt);

			m_UnBListCtrl.SetItemText(ii, 6, pcode->intertype);  //이자유형
			cjcode->intertype.Format(_T("%s"),pcode->intertype);

			m_UnBListCtrl.SetItemText(ii, 7, pcode->minthree);   //민평3
			cjcode->code.Format(_T("%s"),pcode->minfore);

			m_UnBListCtrl.SetItemText(ii, 8, pcode->minfore); //민평4
			cjcode->credit.Format(_T("%s"),pcode->minthree);

			m_UnBListCtrl.SetItemText(ii, 9, pcode->credit);   //신용등급
			cjcode->minfore.Format(_T("%s"),pcode->credit);

			m_UnBListCtrl.SetItemText(ii, 10, pcode->code);   //종목코드
			cjcode->minthree.Format(_T("%s"),pcode->code);

			m_UnBListCtrl.SetItemText(ii, 11,pcode->valday);  //발행일
			cjcode->valday.Format(_T("%s"),pcode->valday);

			m_UnBListCtrl.SetItemText(ii, 12,pcode->update);  //업데이트 
			cjcode->update.Format(_T("%s"),pcode->update);

			m_arrlist.Add(cjcode);
			ii++;
		}
	}
	m_bWorking = false;
}

bool CUnBond::checkstrip(bool bstrip, CJCODESTR* CJCode)
{
	
	switch(m_typecode)
	{
		case TONGCODE:
		case TUKCODE:
		case JICODE:
		case GUMCODE:
		case WHECODE:
	        return true;
			break;
	}

	if(bstrip) //스트립체크 했을때는 원금/이자 들어간것은 true
	{
		/*
		if(CJCode->kind == "1000")  //국고채
		{
			if(CJCode->name.Find(_T("원금")) >= 0 || CJCode->name.Find(_T("이자")) >= 0)
				return true;
			else 
				return false;
		}
		*/
		return true;
	}
	else //스트립체크 안했을때는 원금/이자 들어간것은 false
	{	
	//	if(CJCode->kind == "1000")  //국고채
		{
			if(CJCode->name.Find(_T("원금")) >= 0 || CJCode->name.Find(_T("이자")) >= 0)
				return false;
			else 
				return true;
		}
	}
}

void CUnBond::SearchData(bool beditdata)
{
	if(m_bDonwlading)
		return;

	m_iSelectRow = -1;
	Invalidate();

	CString tmpS, strSearch;

	bool bbalchk = false;
	bool bmanchk = false;
	bool bstrip = false;

	//발행 체크 여부
	if(((CButton*)GetDlgItem(IDC_CHK_VAL))->GetCheck())
		bbalchk = true;
	//만기 체크 여부
	if(((CButton*)GetDlgItem(IDC_CHK_MAN))->GetCheck())
		bmanchk = true;
	//스트립 체크 여부
	if(((CButton*)GetDlgItem(IDC_CHECK_STRIP))->GetCheck())
		bstrip = true;

	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();

	int icnt = pApp->m_arrayCJCode.GetSize();

	COleDateTime odt;
	
	m_Date1Val.GetTime(odt);
	tmpS.Format("%04d%02d%02d", odt.GetYear(), odt.GetMonth(), odt.GetDay());
	int ivals = atoi(tmpS);

	m_Date2Val.GetTime(odt);
	tmpS.Format("%04d%02d%02d", odt.GetYear(), odt.GetMonth(), odt.GetDay());
	int ivale = atoi(tmpS);

	m_Date1Man.GetTime(odt);
	tmpS.Format("%04d%02d%02d", odt.GetYear(), odt.GetMonth(), odt.GetDay());
	int imans = atoi(tmpS);

	m_Date2Man.GetTime(odt);
	tmpS.Format("%04d%02d%02d", odt.GetYear(), odt.GetMonth(), odt.GetDay());
	int imane = atoi(tmpS);


	CJCODESTR* CJCode = NULL;
	CListCtrl* plist = GetVisibleList();
	CArray<CJCODESTR*, CJCODESTR*>	tmpArr;
	CArray<CJCODESTR*, CJCODESTR*>	editArr;
	CArray<CJCODESTR*, CJCODESTR*>	Arr;

	switch(m_typecode)
	{
		case GUKCODE:   //국채
			Arr.Copy(pApp->m_arrGuklist);
			break;
		case JICODE:   //지방
			Arr.Copy(pApp->m_arrJilist);	
			break;
		case TUKCODE:    //특수
			Arr.Copy(pApp->m_arrTuklist);
			break;
		case TONGCODE:   //통안
			Arr.Copy(pApp->m_arrTonglist);
			break;
		case GUMCODE:   //금융
			Arr.Copy(pApp->m_arrGumlist);
		   break;
		case WHECODE:   //회사
			Arr.Copy(pApp->m_arrHeuilist);
			break;
		case ALLCODE:
			Arr.Copy(pApp->m_arrlist);
			break;
		case DANGCODE:
			{
				CString strtmps, tmpS;
				CTime ctstart, ctend;
				ctstart = CTime::GetCurrentTime();
				strtmps.Format(_T("%d-%02d-%02d\n"), ctstart.GetYear(), ctstart.GetMonth(), ctstart.GetDay());
				for(int ii = 0 ; ii < pApp->m_arrlist.GetSize(); ii++)
				{
					CJCode = pApp->m_arrlist.GetAt(ii);
					tmpS = CJCode->valday;
					tmpS.Replace("-","");
					strtmps.Replace("-","");
					if(atoi(tmpS) == atoi(strtmps))
						Arr.Add(CJCode);
				}
			}
			break;
		case BALCODE:
			{
				CString strtmps, tmpS;
				CTime ctstart;
				ctstart = CTime::GetCurrentTime();
				tmpS.Format(_T("%d%02d%02d\n"), ctstart.GetYear(), ctstart.GetMonth(), ctstart.GetDay());
				for(int ii = 0 ; ii < pApp->m_arrlist.GetSize(); ii++)
				{
					CJCode = pApp->m_arrlist.GetAt(ii);
					strtmps = CJCode->valday;
					strtmps.Replace("-","");
					tmpS.Replace(_T("-"),_T(""));
					if(atoi(strtmps) > atoi(tmpS))
						Arr.Add(CJCode);
				}
			}
			break;
	}

	for(int ii = 0 ; ii < Arr.GetSize() ; ii++)
	{
		CJCode = Arr.GetAt(ii);
		if(bbalchk && bmanchk)   //발행, 만기 전부선택
		{
			tmpS = CJCode->valday;
			tmpS.Replace("-", "");
			if(atoi(tmpS) >  ivals &&  atoi(tmpS) < ivale)   //발행일
			{
				tmpS = CJCode->mtday;
				tmpS.Replace("-", "");
				if(atoi(tmpS) > imans && atoi(tmpS) < imane )  //만기
				{
					if(checkstrip(bstrip, CJCode))			  //스트립
						tmpArr.Add(CJCode);
				}
			}
		}
		else if(bbalchk && !bmanchk)   //발행일만 선택
		{
			tmpS = CJCode->valday;
			tmpS.Replace("-", "");
			if(atoi(tmpS) > ivals &&  atoi(tmpS) < ivale)  
			{
				if(checkstrip(bstrip, CJCode))
					tmpArr.Add(CJCode);
			}
		}
		else if(!bbalchk && bmanchk)  //만기만 선택
		{
			tmpS = CJCode->mtday;
			tmpS.Replace("-", "");
			if(atoi(tmpS) > imans &&  atoi(tmpS) < imane)   
			{
				if(checkstrip(bstrip, CJCode))
					tmpArr.Add(CJCode);
			}
		}
		else if(!bbalchk && !bmanchk && bstrip)  //발행일 만기선택 안하고 스트림선택 
		{
			if(checkstrip(bstrip, CJCode))
				tmpArr.Add(CJCode);
		}
		else if(!bbalchk && !bmanchk && !bstrip)  //셋다 안선택
		{
			if(checkstrip(bstrip, CJCode))
				tmpArr.Add(CJCode);
		}
	}

	m_Search.GetWindowText(strSearch);
	if(beditdata || (!beditdata && strSearch.GetLength() > 0))
	{
		editArr.RemoveAll();
		bool bCode = false;

		if(strSearch.Left(2) == "KR" && strSearch.GetLength() == 12)
			bCode = true;

		CJCODESTR* CJCode = NULL;
		for(int ii = 0; ii < tmpArr.GetSize() ;ii++)
		{  
			CJCode = tmpArr.GetAt(ii);
			if(bCode)
			{
				if(CJCode->code.Find(strSearch) >= 0)
					editArr.Add(CJCode);
			}
			else
			{
				if(CJCode->name.Find(strSearch) >= 0)
					editArr.Add(CJCode);
			}
		}
		FillListByArr(GetVisibleList(), editArr);
	}
	else
		FillListByArr(GetVisibleList(), tmpArr);

}

void CUnBond::FillListByArr(CListCtrl* plist, CArray<CJCODESTR*, CJCODESTR*>&	tmpArr)
{
	CJCODESTR* CJCode = NULL;

	//if(tmpArr.GetSize() < LISTCNT)
	//	plist->DeleteAllItems();
	plist->DeleteAllItems();

	if(tmpArr.GetSize() > 0)
	{	
		for(int ii = 0 ; ii < min(tmpArr.GetSize(), LISTCNT) ; ii++)
	//	for(int ii = 0 ; ii < tmpArr.GetCount() ; ii++)
		{
			LISTITEM* pitem;
			CJCode = tmpArr.GetAt(ii);
			plist->InsertItem(ii, _T(""));
			plist->SetItemText(ii, 1, CJCode->name);
			plist->SetItemText(ii, 2, CJCode->idct);
			plist->SetItemText(ii, 3, CJCode->mtry);
			plist->SetItemText(ii, 4, CJCode->mtday);
			plist->SetItemText(ii, 5, CJCode->itrt);
			plist->SetItemText(ii, 6, CJCode->intertype);
			plist->SetItemText(ii, 7, CJCode->minthree);	
			plist->SetItemText(ii, 8, CJCode->minfore);
			plist->SetItemText(ii, 9, CJCode->credit);
			plist->SetItemText(ii, 10, CJCode->code);
			plist->SetItemText(ii, 11, CJCode->valday);
			plist->SetItemText(ii, 12, CJCode->update);

			pitem = new LISTITEM;
			strncpy((char*)pitem->Code, CJCode->code,CJCode->code.GetLength());
			strncpy((char*)pitem->idct, CJCode->idct,min(CJCode->code.GetLength(), sizeof(pitem->idct)/sizeof(char*)));  //지표구분  
			strncpy((char*)pitem->mtry, CJCode->mtry,min(CJCode->mtry.TrimLeft().TrimRight().GetLength(), sizeof(pitem->mtry)/sizeof(char*)));  //만기구분
			strncpy((char*)pitem->man, CJCode->mtday, min(CJCode->mtday.TrimLeft().TrimRight().GetLength(), sizeof(pitem->man)/sizeof(char*))); //만기일
			strncpy((char*)pitem->itrt, CJCode->itrt, min(CJCode->itrt.TrimLeft().TrimRight().GetLength(), sizeof(pitem->itrt)/sizeof(char*)));  //금리
			strncpy((char*)pitem->intertype, CJCode->intertype, min(CJCode->intertype.TrimLeft().TrimRight().GetLength(), sizeof(pitem->intertype)/sizeof(char*)));  //이자유형
			strncpy((char*)pitem->minfore, CJCode->minfore, min(CJCode->minfore.TrimLeft().TrimRight().GetLength(), sizeof(pitem->minfore)/sizeof(char*)));  //민평4
			strncpy((char*)pitem->minthree, CJCode->minthree, min(CJCode->minthree.TrimLeft().TrimRight().GetLength(), sizeof(pitem->minthree)/sizeof(char*)));  //민평3
			strncpy((char*)pitem->credit, CJCode->credit, min(CJCode->credit.TrimLeft().TrimRight().GetLength(), sizeof(pitem->credit)/sizeof(char*)));  //신용등급
			strncpy((char*)pitem->Name, CJCode->name,min(CJCode->name.TrimLeft().TrimRight().GetLength(), sizeof(pitem->Name)/sizeof(char*)));	  //종목명
			strncpy((char*)pitem->valday, CJCode->valday,min(CJCode->valday.TrimLeft().TrimRight().GetLength(), sizeof(pitem->valday)/sizeof(char*)));	  //발행일
			plist->SetItemData(ii, (LPARAM)pitem);
		}

		if(tmpArr.GetSize() > LISTCNT)
		{
			m_title = "종목로드중";
			m_bDonwlading = true;
			SetWindowText(m_title);
			m_cpArr.Copy(tmpArr);
			m_iListcnt = LISTCNT;
			m_iTotalCnt = tmpArr.GetCount();
			SetTimer(TM_LOAD, 200, NULL);
			SetTimer(TM_TITLE, 1000, NULL);
		}
	}
}

void CUnBond::OnChangeEditCode()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	CArray<CJCODE*, CJCODE*>*	arrtemp = new CArray<CJCODE*, CJCODE*>;
	
	CString strSearch(_T(""));
	m_Search.GetWindowText(strSearch);

	strSearch.MakeUpper();			// 2009/08/31 KJH ADD - UPPERCASE

	CString strSrc(_T(""));
	int nIndex = -1;

	bool bbalchk = false;
	bool bmanchk = false;

	//발행 체크 여부
	if(((CButton*)GetDlgItem(IDC_CHK_VAL))->GetCheck())
		bbalchk = true;
	//만기 체크 여부
	if(((CButton*)GetDlgItem(IDC_CHK_MAN))->GetCheck())
		bmanchk = true;

	if (strSearch.IsEmpty() && !bbalchk && !bmanchk)
		return;

	BOOL bNumeric = IsNumeric((LPCTSTR)strSearch.Left(3));
	BOOL bFind = FALSE;
	bool isHan = true;

	char* test = (LPSTR)strSearch.operator LPCTSTR();
	CString tmpS = _T("");
	/*
	for (int jj = 0; jj < strSearch.GetLength(); jj++)
	{
		if (test[jj] & 0x80)
			isHan = true;
	}

	if(strSearch.GetLength() >= 2)
	{
		if(strSearch.Left(2) == _T("KT"))
			isHan = false;
	}
	*/

	_TRIMDATA(strSearch);
	int nLen = DeleteSpace((LPSTR)(LPCTSTR)strSearch, strSearch.GetLength());
	
	CListCtrl* plist = GetVisibleList();
	
	{
		int nCount = plist->GetItemCount();
	
		for (int i = 0; i < nCount; i++)
		{
		//	strSrc = plist->GetItemText(i, isHan ? 1 : 7);  //1 종목명 7 종목코드
			strSrc = plist->GetItemText(i, 1);

			if (strSrc.IsEmpty())
				continue;
		
		//	if (!bNumeric)
		//		DeleteSpace((LPSTR)(LPCTSTR)strSrc, HNameLen);
		
			strSrc = strSrc.Left(min(strSrc.GetLength(), strSearch.GetLength()));
			if (strSrc == strSearch)
			{
				nIndex = i;
				break;
			}
		}
	}
	
	if (nIndex == -1)
			return;
	
	m_iSelectRow = nIndex;
	POSITION pos = plist->GetFirstSelectedItemPosition();

	while (pos)
	{
		int nItem = plist->GetNextSelectedItem(pos);
		plist->SetItemState(nItem, 0, LVIS_SELECTED);
	}

	CRect rcItem;
	plist->GetItemRect(0, &rcItem, LVIR_LABEL);
	CSize sz(0, rcItem.Height() * (nIndex - plist->GetTopIndex()));
				
	plist->Scroll(sz);

	plist->SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	plist->SetSelectionMark(nIndex);

	if(strSearch.GetLength() == 0)
	{
		m_iSelectRow = -1;
		Invalidate();
	}
}


int CUnBond::DeleteSpace(char* pBuff, int nLen)
{
	char szTemp[128];

	int j = 0;

	for (int i = 0; i < nLen; i++)
	{
		if (pBuff[i] != 0x20)
			szTemp[j++] = pBuff[i];
	}

	CopyMemory(pBuff, szTemp, j);
	pBuff[j] = 0x00;

	return j;
}


BOOL CUnBond::IsNumeric(CString strVal)
{
	char* pLast = NULL;
	char* pNext = NULL;

	if (strVal.IsEmpty())
		return FALSE;

	pLast = (LPSTR)(LPCTSTR)strVal + strVal.GetLength();
	strtol((LPCTSTR)strVal, &pNext, 10);

	return (pNext == pLast);
}

void CUnBond::OnBnClickedChkVal()
{
	SearchData();
	OnChangeEditCode();
}


void CUnBond::OnBnClickedChkMan()
{
	SearchData();
	OnChangeEditCode();
}

BOOL CUnBond::IsExistInBalDate(CString strdata)
{
	return TRUE;
}

BOOL CUnBond::IsExistInManDate(CString strdata)
{
	return TRUE;
}


void CUnBond::OnBnClickedBtnLeft()
{
	CString tmpS;
	for(int ii = 0 ; ii < m_UnBListCtrl.GetItemCount() ; ii++)
	{
		if(m_UnBListCtrl.GetCheck(ii))
		{
			if(m_pMain != NULL)
			{
				tmpS.Format(_T("%s\t%s\t%s\t%s"), m_UnBListCtrl.GetItemText(ii, 1),
												m_UnBListCtrl.GetItemText(ii, 7),
												m_UnBListCtrl.GetItemText(ii, 5),
												m_UnBListCtrl.GetItemText(ii, 11));
				m_pMain->SendMessage(WM_USER + 500, axADDINTERSET,  (LPARAM)tmpS.operator LPCSTR());
			}
		}
	}
	
}


void CUnBond::OnBnClickedBtnRight()
{
	
	if(m_pMain != NULL)
		m_pMain->SendMessage(WM_USER + 500, axDELINTERSET,  0);
}

void CUnBond::AllItemRedraw()
{
	((CWnd*)GetDlgItem(IDC_BTN_ALL))->Invalidate();
	
	((CWnd*)GetDlgItem(IDC_BTN_GUK))->Invalidate();

	((CWnd*)GetDlgItem(IDC_BTN_TONG))->Invalidate();

	((CWnd*)GetDlgItem(IDC_BTN_TUK))->Invalidate();

	((CWnd*)GetDlgItem(IDC_BTN_JI))->Invalidate();

	((CWnd*)GetDlgItem(IDC_BTN_GUM))->Invalidate();

	((CWnd*)GetDlgItem(IDC_BTN_HEUI))->Invalidate();
	
	((CWnd*)GetDlgItem(IDC_BTN_DANG))->Invalidate();

	((CWnd*)GetDlgItem(IDC_BTN_BAL))->Invalidate();

	((CWnd*)GetDlgItem(IDC_STATIC))->Invalidate();
	
	((CWnd*)GetDlgItem(IDC_EDIT_CODE))->Invalidate();

	((CWnd*)GetDlgItem(IDC_CHK_VAL))->Invalidate();
	
	((CWnd*)GetDlgItem(IDC_DATE_1Val))->Invalidate();

	((CWnd*)GetDlgItem(IDC_DATE_2VAL))->Invalidate();

	((CWnd*)GetDlgItem(IDC_CHK_MAN))->Invalidate();

	((CWnd*)GetDlgItem(IDC_DATE_1MAN))->Invalidate();

	((CWnd*)GetDlgItem(IDC_DATE_2MAN))->Invalidate();

	((CWnd*)GetDlgItem(IDC_BTN_APPLY))->Invalidate();

	m_InterList.Invalidate();
		
	m_UnBListCtrl.Invalidate();

	m_AllList.Invalidate();
}

void CUnBond::MoveAllLeft() 
{
	return;
	CRect tmprc,rc, temprc;

	GetWindowRect(&tmprc);
	MoveWindow(tmprc.left, tmprc.top, 740, tmprc.Height());
	
	m_btnleft.ShowWindow(SW_HIDE);
	m_btnright.ShowWindow(SW_HIDE);

	m_btnleft.GetClientRect(&rc);
	m_btnAll.GetClientRect(&temprc);

	int x;	
	x = (GAP * 2) +  temprc.Width();

	m_btnleft.GetClientRect(&rc);
	m_btnAll.GetClientRect(&temprc);

	x = 0;
	m_btnleft.GetClientRect(&rc);
	x = GAP;

	((CWnd*)GetDlgItem(IDC_BTN_ALL))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_BTN_ALL))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_GUK))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_TONG))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_TUK))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_JI))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_GUM))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_HEUI))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_DANG))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_BAL))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());


	((CWnd*)GetDlgItem(IDC_BTN_BAL))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	m_InterList.GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	m_InterList.MoveWindow(x, rc.bottom + GAP, tmprc.Width(), tmprc.Height());
	m_UnBListCtrl.SetColumnWidth(0, 0);
	m_AllList.SetColumnWidth(0, 0);

	x = (GAP * 2) +  temprc.Width();
	m_UnBListCtrl.MoveWindow(x, 36, m_rcList.Width(), m_rcList.Height());
	m_UnBListCtrl.Invalidate();
	m_AllList.MoveWindow(x, 36, m_rcList.Width(), m_rcList.Height());
	m_AllList.Invalidate();
	
	x = 0;
	x = GAP;
	((CWnd*)GetDlgItem(IDC_STATIC))->GetWindowRect(&tmprc);
	ScreenToClient(&tmprc);
	((CWnd*)GetDlgItem(IDC_STATIC))->MoveWindow(x, tmprc.top,  tmprc.Width(), tmprc.Height());

	((CWnd*)GetDlgItem(IDC_STATIC))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_EDIT_CODE))->GetWindowRect(&tmprc);
	ScreenToClient(&tmprc);
	((CWnd*)GetDlgItem(IDC_EDIT_CODE))->MoveWindow(rc.right, tmprc.top, tmprc.Width(), tmprc.Height());
	
	((CWnd*)GetDlgItem(IDC_EDIT_CODE))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_CHK_VAL))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_CHK_VAL))->MoveWindow(rc.right, tmprc.top, tmprc.Width(), tmprc.Height());

	((CWnd*)GetDlgItem(IDC_CHK_VAL))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_DATE_1Val))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_DATE_1Val))->MoveWindow(rc.right, tmprc.top,  tmprc.Width(), tmprc.Height());

	((CWnd*)GetDlgItem(IDC_DATE_1Val))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_DATE_2VAL))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_DATE_2VAL))->MoveWindow(rc.right, tmprc.top, tmprc.Width(), tmprc.Height());

	((CWnd*)GetDlgItem(IDC_DATE_2VAL))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_CHK_MAN))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_CHK_MAN))->MoveWindow(rc.right, tmprc.top,  tmprc.Width(), tmprc.Height());

	((CWnd*)GetDlgItem(IDC_CHK_MAN))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_DATE_1MAN))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_DATE_1MAN))->MoveWindow(rc.right, tmprc.top, tmprc.Width(), tmprc.Height());

	((CWnd*)GetDlgItem(IDC_DATE_1MAN))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_DATE_2MAN))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_DATE_2MAN))->MoveWindow(rc.right, tmprc.top, tmprc.Width(), tmprc.Height());

	((CWnd*)GetDlgItem(IDC_DATE_2MAN))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_BTN_APPLY))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_BTN_APPLY))->MoveWindow(rc.right, tmprc.top, tmprc.Width(), tmprc.Height());
	AllItemRedraw();
	
	return;
}

void CUnBond::Reposition()
{
	MoveAllLeft();
}

void CUnBond::MoveAllRight()
{
	CRect tmprc, rc, temprc;

	GetWindowRect(&tmprc);
	MoveWindow(tmprc.left, tmprc.top,  770, tmprc.Height());
	
	m_btnleft.ShowWindow(SW_SHOW);
	m_btnright.ShowWindow(SW_SHOW);

	m_btnleft.GetClientRect(&rc);
	m_btnAll.GetClientRect(&temprc);

	int x = 0;
	m_btnleft.GetClientRect(&rc);
	x = (GAP * 2) + rc.Width();

	((CWnd*)GetDlgItem(IDC_BTN_ALL))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_BTN_ALL))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_GUK))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_TONG))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_TUK))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_JI))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_GUM))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_HEUI))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_DANG))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	tmprc.OffsetRect(0, tmprc.Height() + GAP);
	((CWnd*)GetDlgItem(IDC_BTN_BAL))->MoveWindow(x, tmprc.top, tmprc.Width(), tmprc.Height());

	x = 0;
	x = (GAP * 2) + rc.Width();
	((CWnd*)GetDlgItem(IDC_STATIC))->GetWindowRect(&tmprc);
	ScreenToClient(&tmprc);
	((CWnd*)GetDlgItem(IDC_STATIC))->MoveWindow(x, tmprc.top,  tmprc.Width(), tmprc.Height());

	((CWnd*)GetDlgItem(IDC_STATIC))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_EDIT_CODE))->GetWindowRect(&tmprc);
	ScreenToClient(&tmprc);
	((CWnd*)GetDlgItem(IDC_EDIT_CODE))->MoveWindow(rc.right, tmprc.top, tmprc.Width(), tmprc.Height());
	((CWnd*)GetDlgItem(IDC_EDIT_CODE))->SetWindowText(_T(""));


	((CWnd*)GetDlgItem(IDC_EDIT_CODE))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_CHK_VAL))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_CHK_VAL))->MoveWindow(rc.right, tmprc.top, tmprc.Width(), tmprc.Height());


	((CWnd*)GetDlgItem(IDC_CHK_VAL))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_DATE_1Val))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_DATE_1Val))->MoveWindow(rc.right, tmprc.top,  tmprc.Width(), tmprc.Height());


	((CWnd*)GetDlgItem(IDC_DATE_1Val))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_DATE_2VAL))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_DATE_2VAL))->MoveWindow(rc.right, tmprc.top, tmprc.Width(), tmprc.Height());

	((CWnd*)GetDlgItem(IDC_DATE_2VAL))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_CHK_MAN))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_CHK_MAN))->MoveWindow(rc.right, tmprc.top,  tmprc.Width(), tmprc.Height());

	((CWnd*)GetDlgItem(IDC_CHK_MAN))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_DATE_1MAN))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_DATE_1MAN))->MoveWindow(rc.right, tmprc.top, tmprc.Width(), tmprc.Height());

	((CWnd*)GetDlgItem(IDC_DATE_1MAN))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_DATE_2MAN))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_DATE_2MAN))->MoveWindow(rc.right, tmprc.top, tmprc.Width(), tmprc.Height());

	((CWnd*)GetDlgItem(IDC_DATE_2MAN))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.right += GAP;
	((CWnd*)GetDlgItem(IDC_BTN_APPLY))->GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	((CWnd*)GetDlgItem(IDC_BTN_APPLY))->MoveWindow(rc.right, tmprc.top, tmprc.Width(), tmprc.Height());

	x = 0;
	m_btnleft.GetClientRect(&rc);
	x = (GAP * 2) + rc.Width();

	((CWnd*)GetDlgItem(IDC_BTN_BAL))->GetWindowRect(&temprc);
	ScreenToClient(&temprc);
	m_InterList.GetWindowRect(&tmprc);
	ScreenToClient(tmprc);
	m_InterList.MoveWindow(x, temprc.bottom + GAP, tmprc.Width(), tmprc.Height());
	
	x = (GAP * 3) + rc.Width() + temprc.Width();	
	m_UnBListCtrl.ShowWindow(SW_HIDE);
	m_UnBListCtrl.MoveWindow(x, 36, m_rcList.Width(), m_rcList.Height());
	m_UnBListCtrl.ShowWindow(SW_SHOW);
	m_UnBListCtrl.SetColumnWidth(0, 18);

	m_AllList.ShowWindow(SW_HIDE);
	m_AllList.MoveWindow(x, 36, m_rcList.Width(), m_rcList.Height());
	m_AllList.SetColumnWidth(0, 18);
	AllItemRedraw();
}

void CUnBond::SetButtonPress(int type)
{
	if(type == ALLCODE)
	{
		m_AllList.ShowWindow(SW_SHOW);
		m_UnBListCtrl.ShowWindow(SW_HIDE);
	}
	else
	{
		m_AllList.ShowWindow(SW_HIDE);
		m_UnBListCtrl.ShowWindow(SW_SHOW);
	}

	  //test
	if(type == ALLCODE)
	{
		m_btnAll.m_push = true;
		m_btnGuk.m_push = false;
		m_btnGum.m_push = false;
		m_btnHeui.m_push = false;
		m_btnJi.m_push = false;
		m_btnTong.m_push = false;
		m_btnTuk.m_push = false;
		m_btnBal.m_push = false;
		m_btnDang.m_push = false;
	}
	else if(type == GUKCODE)
	{
		m_btnAll.m_push = false;
		m_btnGuk.m_push = true;
		m_btnGum.m_push = false;
		m_btnHeui.m_push = false;
		m_btnJi.m_push = false;
		m_btnTong.m_push = false;
		m_btnTuk.m_push = false;
		m_btnBal.m_push = false;
		m_btnDang.m_push = false;

	}
	else if(type == TONGCODE)
	{
		m_btnAll.m_push = false;
		m_btnGuk.m_push = false;
		m_btnGum.m_push = false;
		m_btnHeui.m_push = false;
		m_btnJi.m_push = false;
		m_btnTong.m_push = true;
		m_btnTuk.m_push = false;
		m_btnBal.m_push = false;
		m_btnDang.m_push = false;
	}
	else if(type == TUKCODE)
	{
		m_btnAll.m_push = false;
		m_btnGuk.m_push = false;
		m_btnGum.m_push = false;
		m_btnHeui.m_push = false;
		m_btnJi.m_push = false;
		m_btnTong.m_push = false;
		m_btnTuk.m_push = true;
		m_btnBal.m_push = false;
		m_btnDang.m_push = false;
	}
	else if(type == JICODE)
	{
		m_btnAll.m_push = false;
		m_btnGuk.m_push = false;
		m_btnGum.m_push = false;
		m_btnHeui.m_push = false;
		m_btnJi.m_push = true;
		m_btnTong.m_push = false;
		m_btnTuk.m_push = false;
		m_btnBal.m_push = false;
		m_btnDang.m_push = false;
	}
	else if(type == GUMCODE)
	{
		m_btnAll.m_push = false;
		m_btnGuk.m_push = false;
		m_btnGum.m_push = true;
		m_btnHeui.m_push = false;
		m_btnJi.m_push = false;
		m_btnTong.m_push = false;
		m_btnTuk.m_push = false;
		m_btnBal.m_push = false;
		m_btnDang.m_push = false;
	}
	else if(type == WHECODE)
	{
		m_btnAll.m_push = false;
		m_btnGuk.m_push = false;
		m_btnGum.m_push = false;
		m_btnHeui.m_push = true;
		m_btnJi.m_push = false;
		m_btnTong.m_push = false;
		m_btnTuk.m_push = false;
		m_btnBal.m_push = false;
		m_btnDang.m_push = false;
	}
	else if(type == DANGCODE)
	{
		m_btnAll.m_push = false;
		m_btnGuk.m_push = false;
		m_btnGum.m_push = false;
		m_btnHeui.m_push = false;
		m_btnJi.m_push = false;
		m_btnTong.m_push = false;
		m_btnTuk.m_push = false;
		m_btnBal.m_push = false;
		m_btnDang.m_push = true;
	}
	else if(type == BALCODE)
	{
		m_btnAll.m_push = false;
		m_btnGuk.m_push = false;
		m_btnGum.m_push = false;
		m_btnHeui.m_push = false;
		m_btnJi.m_push = false;
		m_btnTong.m_push = false;
		m_btnTuk.m_push = false;
		m_btnBal.m_push = true;
		m_btnDang.m_push = false;
	}
	else if(type == INTERCODE)
	{
		m_btnAll.m_push = false;
		m_btnGuk.m_push = false;
		m_btnGum.m_push = false;
		m_btnHeui.m_push = false;
		m_btnJi.m_push = false;
		m_btnTong.m_push = false;
		m_btnTuk.m_push = false;
		m_btnBal.m_push = false;
		m_btnDang.m_push = false;
	}
	

	m_btnAll.Invalidate();
	m_btnGuk.Invalidate();
	m_btnGum.Invalidate();
	m_btnHeui.Invalidate();
	m_btnJi.Invalidate();
	m_btnTong.Invalidate();
	m_btnTuk.Invalidate();
	m_btnBal.Invalidate();
	m_btnDang.Invalidate();
}

void CUnBond::FillListByGubn(int igubn)
{
	/*
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	CJCODE*  CJCode;
	struct CJcode code;

	CArray<CJCODESTR*, CJCODESTR*> tmparr;
	switch(igubn)
	{
		case GUKCODE:   //국채
			tmparr.Copy(pApp->m_arrGuklist);
		break;
		case JICODE:   //지방
			tmparr.Copy(pApp->m_arrJilist);
		break;
		case TUKCODE:    //특수
			tmparr.Copy(pApp->m_arrTuklist);
		break;
		case TONGCODE:   //통안
			tmparr.Copy(pApp->m_arrTonglist);
		break;
		case GUMCODE:   //금융
			tmparr.Copy(pApp->m_arrGumlist);
		break;
		case WHECODE:   //회사
			tmparr.Copy(pApp->m_arrHeuilist);
		break;
	}
	*/
	SearchData();
}

void CUnBond::OnBnClickedBtnAll()
{
	m_typecode = ALLCODE;
	SetButtonPress(m_typecode);
	m_szIntergroup.Empty();
	AllListSet();
	WriteLastbtn();
}

void CUnBond::OnBnClickedBtnGuk()
{
	m_typecode = GUKCODE;
	SetButtonPress(m_typecode);
	m_szIntergroup.Empty();
	FillListByGubn(m_typecode);
	WriteLastbtn();
}

void CUnBond::OnBnClickedBtnTong()
{
	m_typecode = TONGCODE;
	SetButtonPress(m_typecode);
	m_szIntergroup.Empty();
	FillListByGubn(m_typecode);
	WriteLastbtn();
}


void CUnBond::OnBnClickedBtnTuk()
{
	m_typecode = TUKCODE;
	SetButtonPress(m_typecode);
	m_szIntergroup.Empty();
	FillListByGubn(m_typecode);
	WriteLastbtn();
}


void CUnBond::OnBnClickedBtnJi()
{
	m_typecode = JICODE;
	SetButtonPress(m_typecode);
	m_szIntergroup.Empty();
	FillListByGubn(m_typecode);
	WriteLastbtn();
}


void CUnBond::OnBnClickedBtnGum()
{
	m_typecode = GUMCODE;
	SetButtonPress(m_typecode);
	m_szIntergroup.Empty();
	FillListByGubn(m_typecode);
	WriteLastbtn();
}


void CUnBond::OnBnClickedBtnHeui()
{
	m_typecode = WHECODE;
	SetButtonPress(m_typecode);
	m_szIntergroup.Empty();
	FillListByGubn(m_typecode);
	WriteLastbtn();
}


void CUnBond::OnBnClickedBtnDang()
{
	m_typecode = DANGCODE;
	SetButtonPress(m_typecode);
	m_szIntergroup.Empty();
	FillListByGubn(m_typecode);
	WriteLastbtn();
}


void CUnBond::OnBnClickedBtnBal()
{
	m_typecode = BALCODE;
	SetButtonPress(m_typecode);
	m_szIntergroup.Empty();
	FillListByGubn(m_typecode);
	WriteLastbtn();
}

CListCtrl* CUnBond::GetVisibleList()
{
	if(m_AllList.IsWindowVisible())
		return &m_AllList;
	else
		return &m_UnBListCtrl;
}

HBRUSH CUnBond::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_STATIC_INTER:
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(75, 96, 139));
			pDC->SetBkColor(RGB(255,255,255));
			hbr = m_pInterBrusl->operator HBRUSH();
			break;
		case IDC_STATIC:
		case IDC_CHK_VAL:
		case IDC_CHK_MAN:	
		case IDC_CHECK_STRIP:
		case IDC_STATIC_BAL:
		case IDC_STATIC_MAN:
		case IDC_STATIC_STRIP:
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(75, 96, 139));
			pDC->SetBkColor(RGB(249,249,249));
			hbr = m_pCtlBrush->operator HBRUSH();
			break;
	}
	
	return hbr;
}

void CUnBond::OnBnClickedCheckStrip()
{
	SearchData();
	OnChangeEditCode();
}


void CUnBond::OnCustomdrawListUnbond(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	
	if(pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = (LRESULT)CDRF_NOTIFYITEMDRAW;
		return;
	}

	if(pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
	{
		if(pNMCD->dwItemSpec == m_iSelectRow)
		{
			NMLVCUSTOMDRAW *pDraw = (NMLVCUSTOMDRAW*)(pNMHDR);
			pDraw->clrText = 0xffffff;
			pDraw->clrTextBk = RGB(51,153,255);
			*pResult = (LRESULT)CDRF_NOTIFYITEMDRAW;
			return;
		}
		else
		{
			NMLVCUSTOMDRAW *pDraw = (NMLVCUSTOMDRAW*)(pNMHDR);
			pDraw->clrText = RGB(0,0,0);
			pDraw->clrTextBk = RGB(255,255,255);
			*pResult = (LRESULT)CDRF_NOTIFYITEMDRAW;
			return;
		}
	}

	*pResult = 0;
}


void CUnBond::OnClickListUnbond(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int ipredata = m_iSelectRow;
	m_iSelectRow = -1;
	if(ipredata != m_iSelectRow)
		m_UnBListCtrl.Invalidate();

	BOOL bShift = (::GetKeyState(VK_SHIFT) & 0x8000);
	if(bShift)
	{
		for(int ii = 0 ; ii < m_UnBListCtrl.GetItemCount() ; ii++)
		{
			if(m_UnBListCtrl.GetItemState(ii, LVIS_SELECTED) == LVIS_SELECTED)
				m_UnBListCtrl.SetCheck(ii, true);
		}
	}
	*pResult = 0;
}


void CUnBond::OnCustomdrawListAllbond(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if(pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = (LRESULT)CDRF_NOTIFYITEMDRAW;
		return;
	}

	if(pNMCD->dwDrawStage == CDDS_ITEMPREPAINT)
	{
		if(pNMCD->dwItemSpec == m_iSelectRow)
		{
			NMLVCUSTOMDRAW *pDraw = (NMLVCUSTOMDRAW*)(pNMHDR);
			pDraw->clrText = 0xffffff;
			pDraw->clrTextBk = RGB(51,153,255);
			*pResult = (LRESULT)CDRF_NOTIFYITEMDRAW;
			return;
		}
		else
		{
			NMLVCUSTOMDRAW *pDraw = (NMLVCUSTOMDRAW*)(pNMHDR);
			pDraw->clrText = RGB(0,0,0);
			pDraw->clrTextBk = RGB(255,255,255);
			*pResult = (LRESULT)CDRF_NOTIFYITEMDRAW;
			return;
		}
	}

	*pResult = 0;
}

void CUnBond::OnClickListAllbond(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int ipredata = m_iSelectRow;
	m_iSelectRow = -1;
	if(ipredata != m_iSelectRow)
		m_AllList.Invalidate();

	BOOL bShift = (::GetKeyState(VK_SHIFT) & 0x8000);
	if(bShift)
	{
		for(int ii = 0 ; ii < m_AllList.GetItemCount() ; ii++)
		{
			if(m_AllList.GetItemState(ii, LVIS_SELECTED) == LVIS_SELECTED)
				m_AllList.SetCheck(ii, true);
		}
	}
	*pResult = 0;
}

CString CUnBond::GetExcelTile()
{
	CString strReuslt;
	switch(m_typecode)
	{
		case ALLCODE:
			strReuslt = _T("전체");
			break;
		case GUKCODE:
			strReuslt = _T("국채");
			break;
		case TONGCODE:
			strReuslt = _T("통안채");
			break;
		case TUKCODE:
			strReuslt = _T("특수채");
			break;
		case JICODE:
			strReuslt = _T("지방채");
			break;
		case GUMCODE:
			strReuslt = _T("금융채");
			break;
		case WHECODE:
			strReuslt = _T("회사채");
			break;	
		case DANGCODE:
			strReuslt = _T("당일발행물");
			break;
		case BALCODE:
			strReuslt = _T("발행예정종목");
			break;
	}

	CString tmps;
	m_Search.GetWindowText(tmps);
	if(!tmps.IsEmpty())
	{
		strReuslt += _T("(");
		strReuslt += tmps;
		strReuslt += _T(")");
	}
	return strReuslt;
}

void CUnBond::OnBnClickedBtnExcel()
{
	CListCtrl* plist = GetVisibleList();
	if (plist->GetItemCount() <= 0)
		 return;

	CTime timeNow = CTime::GetCurrentTime();
	CString strDateTime;
	strDateTime = timeNow.Format(_T("%Y-%m-%d_%H%M%S"));
    CString strFileName;
	strFileName.Format("%s_%s", GetExcelTile(),strDateTime);

	CFileDialog fdlg(FALSE, "xlsx", strFileName+".xlsx", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR, "xlsx 파일 (*.xlsx)|*.xlsx", NULL);
	if (fdlg.DoModal() != IDOK) 
		return;

	CXLEzAutomation xls(FALSE);  
	int rowOffset = 5;	 
	int colOffset = 0;

	int rowCount = plist->GetItemCount();
	CHeaderCtrl* pHeaderCtrl = plist->GetHeaderCtrl();
	int colCount = pHeaderCtrl->GetItemCount();

	CString strItemText;
	for (int rowIdx = 0; rowIdx < rowCount; rowIdx++)
	 {
		 for (int colIdx = 0; colIdx < colCount; colIdx++)
		 {
			 strItemText = plist->GetItemText(rowIdx, colIdx);
			 xls.SetCellValue(colIdx + colOffset, rowIdx + rowOffset, strItemText);
		 }
	 }
	
	
	xls.SaveFileAs(fdlg.GetPathName());
	xls.ReleaseExcel();
}


void CUnBond::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
		case TM_START:
			{
				KillTimer(TM_START);
				CString tmpS;
				CString strUserPath(_T(""));
				CString strFilePath(_T(""));
				char		buffer[128];
				strUserPath.Format(_T("%s\\user\\%s"), m_sRoot, m_sUser);
				strFilePath.Format(_T("%s\\%s"), strUserPath, _T("userinterest.ini"));

				GetPrivateProfileString(_T("GROUPINFO"), _T("lastbtn_dlg"), _T(""), buffer, sizeof(buffer), (LPCTSTR)strFilePath);
				tmpS.Format(_T("%s"), buffer);
				tmpS.TrimRight();
				tmpS.TrimLeft();
				if(tmpS.GetLength() > 0)
					m_slastbtn = tmpS;
				else
					m_slastbtn.Empty();

				if(m_slastbtn.IsEmpty())
					OnBnClickedBtnGuk();
				else
				{
					switch(atoi(m_slastbtn))
					{
						case ALLCODE:
							OnBnClickedBtnAll();
							break;
						case GUKCODE:
							OnBnClickedBtnGuk();
							break;
						case GUMCODE:
							OnBnClickedBtnGum();
							break;
						case WHECODE:
							OnBnClickedBtnHeui();
							break;
						case JICODE:
							OnBnClickedBtnJi();
							break;
						case TONGCODE:
							OnBnClickedBtnTong();
							break;
						case TUKCODE:
							OnBnClickedBtnTuk();
							break;
						case BALCODE:
							OnBnClickedBtnBal();
							break;
						case DANGCODE:
							OnBnClickedBtnDang();
						break;
					}
				}
			}
			break;
		case TM_LOAD:
		{
			CListCtrl* plist = GetVisibleList();
			CJCODESTR* CJCode = NULL;
			bool blast = false;
			int ilast , iStartCnt;
			iStartCnt = m_iListcnt;
			if(m_iListcnt + LISTCNT > m_cpArr.GetSize())
			{
				  ilast = m_cpArr.GetSize();
				  blast = true;
			}
			else
			{
				m_iListcnt += LISTCNT;
				ilast = m_iListcnt;
			}

			for(int ii = iStartCnt ; ii < ilast ; ii++)
			{	
				LISTITEM* pitem;
				CJCode = m_cpArr.GetAt(ii);
				plist->InsertItem(ii, _T(""));
				plist->SetItemText(ii, 1, CJCode->name);
				plist->SetItemText(ii, 2, CJCode->idct);
				plist->SetItemText(ii, 3, CJCode->mtry);
				plist->SetItemText(ii, 4, CJCode->mtday);
				plist->SetItemText(ii, 5, CJCode->itrt);
				plist->SetItemText(ii, 6, CJCode->intertype);
				plist->SetItemText(ii, 7, CJCode->minthree);	
				plist->SetItemText(ii, 8, CJCode->minfore);
				plist->SetItemText(ii, 9, CJCode->credit);
				plist->SetItemText(ii, 10, CJCode->code);
				plist->SetItemText(ii, 11, CJCode->valday);
				plist->SetItemText(ii, 12, CJCode->update);

				pitem = new LISTITEM;
				strncpy((char*)pitem->Code, CJCode->code,CJCode->code.GetLength());
				strncpy((char*)pitem->idct, CJCode->idct,min(CJCode->code.GetLength(), sizeof(pitem->idct)/sizeof(char*)));  //지표구분  
				strncpy((char*)pitem->mtry, CJCode->mtry,min(CJCode->mtry.TrimLeft().TrimRight().GetLength(), sizeof(pitem->mtry)/sizeof(char*)));  //만기구분
				strncpy((char*)pitem->man, CJCode->mtday, min(CJCode->mtday.TrimLeft().TrimRight().GetLength(), sizeof(pitem->man)/sizeof(char*))); //만기일
				strncpy((char*)pitem->itrt, CJCode->itrt, min(CJCode->itrt.TrimLeft().TrimRight().GetLength(), sizeof(pitem->itrt)/sizeof(char*)));  //금리
				strncpy((char*)pitem->intertype, CJCode->intertype, min(CJCode->intertype.TrimLeft().TrimRight().GetLength(), sizeof(pitem->intertype)/sizeof(char*)));  //이자유형
				strncpy((char*)pitem->minfore, CJCode->minfore, min(CJCode->minfore.TrimLeft().TrimRight().GetLength(), sizeof(pitem->minfore)/sizeof(char*)));  //민평4
				strncpy((char*)pitem->minthree, CJCode->minthree, min(CJCode->minthree.TrimLeft().TrimRight().GetLength(), sizeof(pitem->minthree)/sizeof(char*)));  //민평3
				strncpy((char*)pitem->credit, CJCode->credit, min(CJCode->credit.TrimLeft().TrimRight().GetLength(), sizeof(pitem->credit)/sizeof(char*)));  //신용등급
				strncpy((char*)pitem->Name, CJCode->name,min(CJCode->name.TrimLeft().TrimRight().GetLength(), sizeof(pitem->Name)/sizeof(char*)));	  //종목명
				strncpy((char*)pitem->valday, CJCode->valday,min(CJCode->valday.TrimLeft().TrimRight().GetLength(), sizeof(pitem->valday)/sizeof(char*)));	  //발행일
				plist->SetItemData(ii, (LPARAM)pitem);
			}

			if(blast)
			{
				KillTimer(TM_LOAD);
				KillTimer(TM_TITLE);

				if(m_iTotalCnt < plist->GetItemCount())
				{
				//	for(int ii = m_iTotalCnt ;  ii < plist->GetItemCount() ; ii++)
				//		plist->DeleteItem(ii);
				}
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				SetWindowText("종목검색");
				m_bDonwlading = false;
			}
		}
		break;
		case TM_TITLE:
		{
			m_title += ".";
			if(m_title == "종목로드중.....")
				m_title = "종목로드중";
	
			SetWindowText(m_title);
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CUnBond::MakePopup()
{
	CMenu	menu;
	CPoint	pt;
	GetCursorPos(&pt);

	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING, 1000, "엑셀 전송");
	int menuID = menu.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, pt.x, pt.y, this);
	switch(menuID)
	{
		case 1000:
			OnBnClickedBtnExcel();
		break;
	}
}

void CUnBond::OnRclickListAllbond(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MakePopup();
	*pResult = 0;
}


void CUnBond::OnRclickListUnbond(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MakePopup();
	*pResult = 0;
}

void CUnBond::OnDatetimechangeDate1man(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(((CButton*)GetDlgItem(IDC_CHK_MAN))->GetCheck())
		SearchData();
	*pResult = 0;
}


void CUnBond::OnDatetimechangeDate1val(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	if(((CButton*)GetDlgItem(IDC_CHK_VAL))->GetCheck())
		SearchData();
	*pResult = 0;
}


void CUnBond::OnDatetimechangeDate2man(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	if(((CButton*)GetDlgItem(IDC_CHK_MAN))->GetCheck())
		SearchData();
	*pResult = 0;
}


void CUnBond::OnDatetimechangeDate2val(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	if(((CButton*)GetDlgItem(IDC_CHK_VAL))->GetCheck())
		SearchData();
	*pResult = 0;
}


BOOL CUnBond::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(m_bDonwlading)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		return TRUE;	
	}
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}
