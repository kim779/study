// UnBond.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TB900100.h"
#include "UnBond.h"
#include "afxdialogex.h"
#include "MapWnd.h"
#include "XLEzAutomation.h"

#define TAB_HEADER_HEIGHT		24

#pragma warning(disable:4996)
// CUnBond 대화 상자입니다.
#define GAP 3
#define TM_START 9999
#define TM_LOAD 8888
#define TM_TITLE 7777
#define CHECKDOWNLOAD 9999

#define LISTCNT 500
IMPLEMENT_DYNAMIC(CUnBond, CDialogEx)

CUnBond* pMain;
/*
typedef struct _listItem { 
	char*	Code[12];
	char*	Name[100];
	char*	val[10];
	char*	man[10];
}LISTITEM;
*/

typedef struct _listItem { 

	char*	Code[12];     //종목코드
	char*	idct[6];      //지표구분
	char*	mtry[6];      //만기구분
	char*	man[10];      //만기일
	char*	itrt[10];      //금리
	char*	intertype[6];      //이자유형
	char*	minfore[10];     //민평4
	char*	minthree[10];     //민평3
	char*	credit[6];     //신용등급
	char*	Name[100];    //종목명
	char*	val[10];		//발행일
}LISTITEM;


int CALLBACK ListViewCompareFunc1(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort) 
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
		case 11:
		tmpS.Format(_T("%.10s"), (char*)pItem1->val);tmpS.Replace("-", "");
		tempS.Format(_T("%.10s"), (char*)pItem2->val);tempS.Replace("-", "");
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


CUnBond::CUnBond(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUnBond::IDD, pParent)
{
	m_sort = FALSE;
	m_pMain = NULL;
	m_pfont = NULL;
	m_pPanel = NULL;
	m_bAllcheck = false;

	m_arrayCJCode.RemoveAll();
	m_arrayGukCode.RemoveAll();
	m_arrayTongCode.RemoveAll();
	m_arrayTukCode.RemoveAll();
	m_arrayJiCode.RemoveAll();
	m_arrayGumCode.RemoveAll();
	m_arrayHeuiCode.RemoveAll();
	m_arrayDangCode.RemoveAll();
	m_arrayBalCode.RemoveAll();
	m_arrlist.RemoveAll();
	m_iSelectRow = -1;
	m_bDonwlading = false;
}

CUnBond::~CUnBond()
{
	TRACE(_T("constructor cunbond"));

	m_bDraging = FALSE;
	if(m_pPanel)
	{
		delete m_pPanel;
		m_pPanel = NULL;
	}
}

void CUnBond::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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
	DDX_Control(pDX, IDC_BTN_LOOK, m_btnLook);
}


BEGIN_MESSAGE_MAP(CUnBond, CDialogEx)
	ON_WM_DESTROY()
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
	ON_MESSAGE(WM_BTNMSG, OnMessage)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_LOOK, &CUnBond::OnBnClickedBtnLook)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK1, &CUnBond::OnBnClickedCheck1)
	ON_NOTIFY(HDN_ITEMSTATEICONCLICK, 0, &CUnBond::OnItemstateiconclickListUnbond)
	ON_NOTIFY(HDN_ITEMSTATEICONCLICK, 0, &CUnBond::OnItemstateiconclickListAllbond)

	ON_NOTIFY(NM_CLICK, IDC_LIST_ALLBOND, &CUnBond::OnClickListAllbond)
	ON_NOTIFY(NM_CLICK, IDC_LIST_UNBOND, &CUnBond::OnClickListUnbond)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_ALLBOND, &CUnBond::OnCustomdrawListAllbond)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_UNBOND, &CUnBond::OnCustomdrawListUnbond)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_ALLBOND, &CUnBond::OnRclickListAllbond)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_UNBOND, &CUnBond::OnRclickListUnbond)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_1Val, &CUnBond::OnDatetimechangeDate1val)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_2MAN, &CUnBond::OnDatetimechangeDate2man)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_2VAL, &CUnBond::OnDatetimechangeDate2val)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_1MAN, &CUnBond::OnDatetimechangeDate1man)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CUnBond 메시지 처리기입니다.
LRESULT CUnBond::OnMessage(WPARAM wParam, LPARAM lParam)
{
	if(m_bDonwlading)
			return -1;
	switch(wParam)
	{
		case btnAll	:  
			{
				OnBnClickedBtnAll();
				SetHeaderCheck(0);
			}
			break;
		case btnGuk:  
			{
				OnBnClickedBtnGuk();
				SetHeaderCheck(0);
			}
			break;
		case btnGum:  
			{
				OnBnClickedBtnGum();
				SetHeaderCheck(0);
			}
			break;
		case btnHeui:  
			{
				OnBnClickedBtnHeui();
				SetHeaderCheck(0);
			}
			break;
		case btnJi: 
			{
				OnBnClickedBtnJi();
				SetHeaderCheck(0);
			}
			break;
		case btnTong:  
			{
				OnBnClickedBtnTong();
				SetHeaderCheck(0);
			}
			break;
		case btnTuk:
			{
				OnBnClickedBtnTuk();
				SetHeaderCheck(0);
			}
			break;
		case btnBal:  
			{
				OnBnClickedBtnBal();
				SetHeaderCheck(0);
			}
			break;
		case btnDang:    
			{
				OnBnClickedBtnDang();
				SetHeaderCheck(0);
			}
			break;
		case btnApply: 
			SearchData(true);
			break;
		case btnleft: 
			{
				CString tmpS;
				CListCtrl* plist = GetVisibleList();
				CWnd *pMain = AfxGetMainWnd();
				for(int ii = 0 ; ii < plist->GetItemCount() ; ii++)
				{
					if(plist->GetCheck(ii))
					{
						if(pMain != NULL)
						{
							tmpS += plist->GetItemText(ii, 10).TrimRight().TrimLeft();
							tmpS += _T("\t");
							plist->SetCheck(ii,false);
						}
					}
				}
				pMain->SendMessage(WM_USER + 500, axADDINTERSET,  (LPARAM)tmpS.operator LPCSTR());
			}
			break;
		case btnright: 
			{
				CWnd *pMain = AfxGetMainWnd();
				if(pMain != NULL)
					pMain->SendMessage(WM_USER + 500, axDELINTERSET,  0);
			}
			break;
		case btnlook: 
			OnBnClickedBtnApply();
			break;
	}

	switch(wParam)
	{
		case btnAll	:  
		case btnGuk:  
		case btnGum:     
		case btnHeui:  
		case btnJi: 
		case btnTong:     
		case btnTuk:
		case btnBal:  
		case btnDang:    
			{
				//m_typecode
				CString strVal, strFilePath, strUserPath;
				strUserPath.Format(_T("%s\\user\\%s"), m_sRoot, m_sUser);
				strFilePath.Format(_T("%s\\%s"), strUserPath, _T("userinterest.ini"));
				strVal.Format(_T("%d"), wParam);
				WritePrivateProfileString(_T("GROUPINFO"), _T("lastbtn"), (LPCTSTR)strVal, (LPCTSTR)strFilePath);
			}
			break;
	}
	return 0;
}

void CUnBond::Setdatapointer(char* pdata)
{
	//memset(m_pcode, 0, 50);
	m_pcode = pdata;
}

void CUnBond::OnDblclkListAllbond(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int item = m_AllList.GetSelectionMark();
	
	CString tmpS;
	CWnd *pMain = AfxGetMainWnd();
	if(pMain != NULL)
	{
		tmpS.Format(_T("%s\t%s\t%s\t%s"), m_AllList.GetItemText(item, 1),
										m_AllList.GetItemText(item, 10),
										m_AllList.GetItemText(item, 8),
										m_AllList.GetItemText(item, 4));
		pMain->SendMessage(WM_USER + 500, axADDINTERSET,  (LPARAM)tmpS.operator LPCSTR());
		m_AllList.SetCheck(item, false);
	}

	*pResult = 0;
}


void CUnBond::OnDblclkListUnbond(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	int item = m_UnBListCtrl.GetSelectionMark();

	CString tmpS;
	CWnd *pMain = AfxGetMainWnd();
	if(pMain != NULL)
	{
		tmpS.Format(_T("%s\t%s\t%s\t%s"), m_UnBListCtrl.GetItemText(item, 1),
										m_UnBListCtrl.GetItemText(item, 10),
										m_UnBListCtrl.GetItemText(item, 8),
										m_UnBListCtrl.GetItemText(item, 4));
		pMain->SendMessage(WM_USER + 500, axADDINTERSET,  (LPARAM)tmpS.operator LPCSTR());
		m_UnBListCtrl.SetCheck(item, false);
	}
	
	*pResult = 0;
}


void CUnBond::OnDestroy()
{
//	CSkinDialog::OnDestroy();
	CDialogEx::OnDestroy();
}


BOOL CUnBond::OnInitDialog()
{
	//CSkinDialog::OnInitDialog();
	CDialogEx::OnInitDialog();
	CString tmpS;
	m_typecode = -1;
	m_bINTERMODE = FALSE;
	// TODO:  Add extra initialization here
	SetDlgPos(m_Point.x, m_Point.y);

	CMapWnd* pMapWnd = (CMapWnd*)GetParent();
	m_sRoot = pMapWnd->GetRoot();

	//버튼 초기화
	InitBtn();

	// List 초기화
	CString str(_T(""));
	InitList();

	SetDate(&m_Date1Val,_T("20090101"));
	SetDate(&m_Date1Man,_T("200901230"));

	m_AllList.ShowWindow(SW_HIDE);
	LoadCJCode();

	m_UnBListCtrl.GetClientRect(&m_rcList);

	m_pCtlBrush = new CBrush();
	m_pCtlBrush->CreateSolidBrush(RGB(249,249,249));

	m_pBrush = new CBrush();
	m_pBrush->CreateSolidBrush(RGB(255,255,255));

	CWnd *pMain = AfxGetMainWnd();
	if(pMain != NULL)
		pMain->SendMessage(WM_USER + 500, 86,  (LPARAM)this);

	SetTimer(TM_START, 500, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CUnBond::SetDate(CDateTimeCtrl* pDate, CString strDate)
{
	COleDateTime odt;

	if (strDate.GetLength() == 8)
		odt.SetDate(atoi((LPCTSTR)strDate.Left(4)), atoi((LPCTSTR)strDate.Mid(4, 2)), atoi((LPCTSTR)strDate.Right(2)));
	else
	{
		SYSTEMTIME tm;
		GetSystemTime(&tm);
		odt.SetDate(tm.wYear, tm.wMonth, tm.wDay);
	}

	if (odt.m_status == COleDateTime::invalid)
		return;

	pDate->SetTime(odt);
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
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();

	CRect rc, tmpRc, rclist;
	GetClientRect(&rc);
	
	tmpRc.left = rc.left + 3;
	tmpRc.top = rc.top + 3;
	tmpRc.right = tmpRc.left + rc.right - 9;
	tmpRc.bottom = tmpRc.top + 56;

	m_pPanel = new CPanel;
	m_pPanel->m_pFont = m_pfont;
	if (!m_pPanel->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS|WS_CLIPCHILDREN, tmpRc, this, 9898))
		delete m_pPanel;

	m_btnAll.set_ToolTipText(_T("전체"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_down");
	m_btnAll.set_Image(imgC, imgO, imgD);
	m_btnAll.SetItemFont(m_pfont);
	m_btnAll.m_fcolor = RGB(0, 0, 0);
	m_btnAll.m_text = _T("전체");
	m_btnAll.m_id = btnAll;

	m_btnGuk.set_ToolTipText(_T("국채"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_down");
	m_btnGuk.set_Image(imgC, imgO, imgD);
	m_btnGuk.SetItemFont(m_pfont);
	m_btnGuk.m_fcolor = RGB(0, 0, 0);
	m_btnGuk.m_text = _T("국채");
	m_btnGuk.m_id = btnGuk;

	m_btnGum.set_ToolTipText(_T("금융채"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_down");
	m_btnGum.set_Image(imgC, imgO, imgD);
	m_btnGum.SetItemFont(m_pfont);
	m_btnGum.m_fcolor = RGB(0, 0, 0);
	m_btnGum.m_text = _T("금융채");
	m_btnGum.m_id = btnGum;

	m_btnHeui.set_ToolTipText(_T("회사채"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_down");
	m_btnHeui.set_Image(imgC, imgO, imgD);
	m_btnHeui.SetItemFont(m_pfont);
	m_btnHeui.m_fcolor = RGB(0, 0, 0);
	m_btnHeui.m_text = _T("회사채");
	m_btnHeui.m_id = btnHeui;

	m_btnJi.set_ToolTipText(_T("지방채"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_down");
	m_btnJi.set_Image(imgC, imgO, imgD);
	m_btnJi.SetItemFont(m_pfont);
	m_btnJi.m_fcolor = RGB(0, 0, 0);
	m_btnJi.m_text = _T("지방채");
	m_btnJi.m_id = btnJi ;

	m_btnTong.set_ToolTipText(_T("통안채"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_down");
	m_btnTong.set_Image(imgC, imgO, imgD);
	m_btnTong.SetItemFont(m_pfont);
	m_btnTong.m_fcolor = RGB(0, 0, 0);
	m_btnTong.m_text = _T("통안채");
	m_btnTong.m_id = btnTong;

	m_btnTuk.set_ToolTipText(_T("특수채"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_down");
	m_btnTuk.set_Image(imgC, imgO, imgD);
	m_btnTuk.SetItemFont(m_pfont);
	m_btnTuk.m_fcolor = RGB(0, 0, 0);
	m_btnTuk.m_text = _T("특수채");
	m_btnTuk.m_id = btnTuk;

	m_btnBal.set_ToolTipText(_T("발행예정종목"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_down");
	m_btnBal.set_Image(imgC, imgO, imgD);
	m_btnBal.SetItemFont(m_pfont);
	m_btnBal.m_fcolor = RGB(0, 0, 0);
	m_btnBal.m_text = _T("발행예정종목");
	m_btnBal.m_id = btnBal;

	m_btnDang.set_ToolTipText(_T("당일발행물"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_65_down");
	m_btnDang.set_Image(imgC, imgO, imgD);
	m_btnDang.SetItemFont(m_pfont);
	m_btnDang.m_fcolor = RGB(0, 0, 0);
	m_btnDang.m_text = _T("당일발행물");
	m_btnDang.m_id = btnDang;

	m_btnApply.set_ToolTipText(_T("조회"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_Act_50");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_Act_50_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN_Act_50_DN");
	m_btnApply.set_Image(imgC, imgO, imgD);
	m_btnApply.SetItemFont(m_pfont);
	m_btnApply.m_fcolor = RGB(249, 249, 249);
	m_btnApply.m_text = _T("조회");
	m_btnApply.m_id = btnApply;   

	m_btnLook.set_ToolTipText(_T("조회"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-lookup");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-lookup_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "BTN-lookup_DN");
	m_btnLook.set_Image(imgC, imgO, imgD);
	m_btnLook.SetItemFont(m_pfont);
	m_btnLook.m_fcolor = RGB(0, 0, 0);
	m_btnLook.m_text = _T("");
	m_btnLook.m_id = btnlook;

	m_btnleft.set_ToolTipText(_T("등록"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "btn_left");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "btn_left_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "btn_left_DN");
	m_btnleft.set_Image(imgC, imgO, imgD);
	m_btnleft.SetItemFont(m_pfont);
	m_btnleft.m_fcolor = RGB(0, 0, 0);
	m_btnleft.m_text = _T("");
	m_btnleft.m_id = btnleft;
	m_btnleft.m_kind = 1;

	m_btnright.set_ToolTipText(_T("삭제"));
	imgC.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "btn_right");
	imgO.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "btn_right_EN");
	imgD.Format("%s\\%s\\%s.bmp", pMapWnd->GetRoot(), _T("image"), "btn_right_DN");
	m_btnright.set_Image(imgC, imgO, imgD);
	m_btnright.SetItemFont(m_pfont);
	m_btnright.m_fcolor = RGB(0, 0, 0);
	m_btnright.m_text = _T("");
	m_btnright.m_id = btnright;
	m_btnleft.m_kind = 1;
}

void CUnBond::FilAllList_thread()
{
	
}

void CUnBond::FillListdata_thread(int igubn)
{
	
}

void CUnBond::OnBnClickedBtnGuk()
{
	m_typecode = GUKCODE;
	m_szIntergroup.Empty();
	SetButtonPress(m_typecode);
	FillListByGubn(m_typecode);
	//m_btnGuk.m_bPress = true;
}

void CUnBond::OnBnClickedBtnTong()
{
	m_typecode = TONGCODE;
	m_szIntergroup.Empty();
	SetButtonPress(m_typecode);
	FillListByGubn(m_typecode);
}

void CUnBond::OnBnClickedBtnTuk()
{
	m_typecode = TUKCODE;
	m_szIntergroup.Empty();
	SetButtonPress(m_typecode);
	FillListByGubn(m_typecode);
}

void CUnBond::OnBnClickedBtnJi()
{
	m_typecode = JICODE;
	m_szIntergroup.Empty();
	SetButtonPress(m_typecode);
	FillListByGubn(m_typecode);
}

void CUnBond::OnBnClickedBtnGum()
{
	m_typecode = GUMCODE;
	m_szIntergroup.Empty();
	SetButtonPress(m_typecode);
	FillListByGubn(m_typecode);
}

void CUnBond::OnBnClickedBtnHeui()
{
	m_typecode = WHECODE;
	m_szIntergroup.Empty();
	SetButtonPress(m_typecode);
	FillListByGubn(m_typecode);
}

void CUnBond::OnBnClickedBtnDang()
{
	m_typecode = DANGCODE;
	m_szIntergroup.Empty();
	SetButtonPress(m_typecode);
	FillListByGubn(m_typecode);
}

void CUnBond::OnBnClickedBtnBal()
{
	m_typecode = BALCODE;
	m_szIntergroup.Empty();
	SetButtonPress(m_typecode);
	FillListByGubn(m_typecode);
}

void CUnBond::FillListByGubn(int igubn)
{
	SearchData();
}

void CUnBond::AllListSet()
{
	SearchData();
}

BOOL CUnBond::LoadCJCode()
{
	CString path, tmps;
	int codeN;
	struct CJcode code;
	CJCODE* cjCode;
	CJCODESTR* cjstrCode = NULL;
	CFile file;
	
	path = m_sRoot + "\\tab\\" + _T("IssBondMast.txt");   
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return false;
	
	int ilen = (int)file.GetLength();
	codeN = (int)file.GetLength() / sizeof(struct CJcode);
	
	CString strtemp, tmpS;
	m_arrayCJCode.RemoveAll();
	for (int ii = 0; ii < codeN; ii++)
	{
		cjCode = new CJCODE;
		cjstrCode = new CJCODESTR;

		memset(cjCode, 0, L_CJcode);
		
		file.Read(&code, sizeof(struct CJcode));
		
		memcpy(cjCode->code, code.code, 12);
		tmps = CString(code.name, 100);
		tmps.TrimRight();
		if(tmps.Find(_T("ELS")) >= 0 )
			continue;
		memcpy(cjCode->name, tmps, tmps.GetLength());
		memcpy(cjCode->kind, code.kind, 4);
		memcpy(cjCode->idct, code.idct, 6);
		tmps = CString(code.idct, 6);
		memcpy(cjCode->valday, code.valday, 10);
		memcpy(cjCode->mtday, code.mtday, 10);
		memcpy(cjCode->mtry, code.mtry, 6);
		memcpy(cjCode->itrt, code.itrt, 10);
		memcpy(cjCode->intertype, code.intertype, 6);
		memcpy(cjCode->credit, code.credit, 6);
		memcpy(cjCode->minfore, code.minfore, 10);
		memcpy(cjCode->minthree, code.minthree, 10);
		tmps = CString(code.update, 14);
		memcpy(cjCode->update, tmps, 14);
		
		m_arrayCJCode.Add(cjCode);

		cjstrCode->code = CString(cjCode->code, 12);
		cjstrCode->name = CString(cjCode->name, 100);
		cjstrCode->kind = CString(cjCode->kind, 4);
		cjstrCode->idct = CString(cjCode->idct, 6);
		cjstrCode->valday = CString(cjCode->valday, 10);
		cjstrCode->mtday = CString(cjCode->mtday, 10);
		cjstrCode->mtry = CString(cjCode->mtry, 6);
		cjstrCode->itrt = CString(cjCode->itrt, 10);
		cjstrCode->intertype = CString(cjCode->intertype, 6);
		cjstrCode->credit = CString(cjCode->credit, 6);
		cjstrCode->minfore = CString(cjCode->minfore, 10);
		cjstrCode->minthree = CString(cjCode->minthree, 10);
		cjstrCode->update = CString(cjCode->update, 14);

		m_arrlist.Add(cjstrCode);

		switch(atoi(cjstrCode->kind))
		{
			case 1000:  //국채
				{
					m_arrayGukCode.Add(cjCode);
					m_arrGuklist.Add(cjstrCode);
				}
				break;
			case 4000:  //지방
				{
					m_arrayJiCode.Add(cjCode);
					m_arrJilist.Add(cjstrCode);
				}
				break;
			case 3000:  //특수
				{
					m_arrayTukCode.Add(cjCode);
					m_arrTuklist.Add(cjstrCode);
				}
				break;
			case 2000:  //통안
				{
					m_arrayTongCode.Add(cjCode);
					m_arrTonglist.Add(cjstrCode);
				}
				break;
			case 5000:  //금융
				{
					m_arrayGumCode.Add(cjCode);
					m_arrGumlist.Add(cjstrCode);
				}
				break;
			case 6000:  //회사
				{
					m_arrayHeuiCode.Add(cjCode);
					m_arrHeuilist.Add(cjstrCode);
				}
				break;
		}
	}

	file.Close();

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
			
			ListView_SortItems((HWND)pnm->hdr.hwndFrom, ListViewCompareFunc1, (LPARAM)pnm->iSubItem);
 			
 			#undef pnm

			break;
 		}
	}
	
//	return CSkinDialog::OnNotify(wParam, lParam, pResult);  
	return CDialogEx::OnNotify(wParam, lParam, pResult); 
}


BOOL CUnBond::FindCode(CString szCode)
{
	/*
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	int nCount = (int)pApp->m_arrayUBCode.GetSize();
	CUNBCODE *code;

	for (int ii = 0; ii < nCount; ii++)
	{
		code = pApp->m_arrayUBCode.GetAt(ii);

		if (szCode == code->code)
		{
			SetName(code->name);
			return TRUE;
		}
	}
	*/
	return FALSE;
}

void CUnBond::Set_KS_HanMode()
{
	/*
	HIMC himc = ImmGetContext(GetSafeHwnd());
	ImmSetConversionStatus(himc, IME_CMODE_NATIVE, IME_SMODE_CONVERSATION);
	ImmReleaseContext(GetSafeHwnd(), himc); 
	*/
}

void CUnBond::InitList()
{
	CMapWnd* pMapWnd = (CMapWnd*)GetParent();

	HDITEM hditem = {0,};
	CString strFilePath(_T(""));
	strFilePath.Format(_T("%s\\image\\%s"), m_sRoot, _T("AXCHECK.BMP"));

	m_cbimagelist.Create(12,12, ILC_COLORDDB|ILC_MASK, 20, 2);
	m_cbimagelist.Add(pMapWnd->GetBitmap(strFilePath), RGB(255,255,255));

	strFilePath.Format(_T("%s\\image\\%s"), m_sRoot, _T("AXCHECK_C.BMP"));
	m_cbimagelist.Add(pMapWnd->GetBitmap(strFilePath), RGB(255,255,255));
	
	CRect rc;
	m_UnBListCtrl.GetClientRect(&rc);
	m_UnBListCtrl.SetExtendedStyle(m_UnBListCtrl.GetExtendedStyle() | LVS_EX_CHECKBOXES |  LVS_EX_FULLROWSELECT );
	m_UnBListCtrl.InsertColumn(0,_T(" "), LVCFMT_LEFT, 18);
	m_UnBListCtrl.InsertColumn(1, _T("           종목명"), LVCFMT_LEFT, 155);
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

	DWORD dwHeaderStyle = ::GetWindowLong(ListView_GetHeader(m_UnBListCtrl), GWL_STYLE);
	dwHeaderStyle |= HDS_CHECKBOXES;
	::SetWindowLong(ListView_GetHeader(m_UnBListCtrl), GWL_STYLE, dwHeaderStyle);

	hditem.mask = HDI_FORMAT;
	Header_GetItem(ListView_GetHeader(m_UnBListCtrl), 0, &hditem);
	hditem.fmt |= HDF_CHECKBOX;
	//hditem.fmt |= HDF_CENTER;
	Header_SetItem(ListView_GetHeader(m_UnBListCtrl), 0, &hditem);

	//m_AllList
	m_AllList.GetClientRect(&rc);
	m_AllList.SetExtendedStyle(m_UnBListCtrl.GetExtendedStyle() | LVS_EX_CHECKBOXES |  LVS_EX_FULLROWSELECT );
	m_AllList.InsertColumn(0,_T(" "), LVCFMT_LEFT, 18);
	m_AllList.InsertColumn(1, _T("           종목명"), LVCFMT_LEFT, 155);
	m_AllList.InsertColumn(2, _T(" 지표"), LVCFMT_LEFT, 45);
	m_AllList.InsertColumn(3, _T("만기 "), LVCFMT_RIGHT, 45);
	m_AllList.InsertColumn(4, _T("만기일"), LVCFMT_CENTER, 80);
	m_AllList.InsertColumn(5, _T("금리"), LVCFMT_CENTER, 60);
	m_AllList.InsertColumn(6, _T("이자유형"), LVCFMT_CENTER, 60);
	m_AllList.InsertColumn(7, _T("민평(3사)"), LVCFMT_CENTER, 70);
	m_AllList.InsertColumn(8, _T("민평(4사)"), LVCFMT_CENTER, 70);
	m_AllList.InsertColumn(9, _T("신용등급"), LVCFMT_CENTER,70);
	m_AllList.InsertColumn(10, _T("종목코드"), LVCFMT_CENTER, 100);
	m_AllList.InsertColumn(11, _T("발행일"), LVCFMT_CENTER, 100);
	m_AllList.InsertColumn(12, _T("업데이트시각"), LVCFMT_CENTER, 0);

	dwHeaderStyle = ::GetWindowLong(ListView_GetHeader(m_AllList), GWL_STYLE);
	dwHeaderStyle |= HDS_CHECKBOXES;
	::SetWindowLong(ListView_GetHeader(m_AllList), GWL_STYLE, dwHeaderStyle);

	hditem.mask = HDI_FORMAT;
	Header_GetItem(ListView_GetHeader(m_AllList), 0, &hditem);
	hditem.fmt |= HDF_CHECKBOX;
	//hditem.fmt |= HDF_CENTER;
	Header_SetItem(ListView_GetHeader(m_AllList), 0, &hditem);
	
	//m_InterList
	m_InterList.GetClientRect(&rc);
	m_InterList.SetExtendedStyle(m_InterList.GetExtendedStyle() |  LVS_EX_FULLROWSELECT);
	m_InterList.InsertColumn(0,_T(" "), LVCFMT_CENTER, rc.Width());
	m_InterList.ShowWindow(SW_HIDE);

	m_sRoot = pMapWnd->GetRoot();
	m_sUser = pMapWnd->GetName();

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
	CString strDFilePath(_T(""));
	m_mapGroupCodemap.RemoveAll();
	m_arrGroup.RemoveAll();
	
	strUserPath.Format(_T("%s\\user\\%s"), m_sRoot, m_sUser);

	CFile file;
	CFileException ex;
	CStringArray* pArr = NULL;

	strFilePath.Format(_T("%s\\%s"), strUserPath, _T("userinterest.ini"));
	strDFilePath.Format(_T("%s\\%s"), strUserPath, _T("defaultinterest.ini"));

	int nGrpCnt = GetPrivateProfileInt(_T("GROUPINFO"), _T("COUNT"), 0, (LPCTSTR)strFilePath);
	CString sGroup(_T(""));
	CString sGrpData(_T(""));
	CString strGroup(_T(""));
	CString tmpS(_T(""));

	m_InterList.DeleteAllItems();

	for ( int i = 0 ; i < nGrpCnt ; i ++ )
	{
		sGroup.Format(_T("%d"), i);

		GetPrivateProfileString(_T("GROUPINFO"), _T("last"), _T(""), sGrpData.GetBufferSetLength(1024), 1024, (LPCTSTR)strFilePath);
		sGrpData.ReleaseBuffer();

		_TRIMDATA(sGrpData);
		if(sGrpData.IsEmpty())
			continue;

		strGroup = Parser(sGrpData, _T(";"));
		strGroup = sGrpData;

		AddGroup(strGroup);

		LoadFile(strGroup);
	}

	char		buffer[128];
	GetPrivateProfileString(_T("GROUPINFO"), _T("lastbtn"), _T(""), buffer, sizeof(buffer), (LPCTSTR)strFilePath);
	tmpS.Format(_T("%s"), buffer);
	tmpS.TrimRight();
	tmpS.TrimLeft();
	if(tmpS.GetLength() > 0)
		m_slastbtn = tmpS;
	else
		m_slastbtn.Empty();

	ReflashSelectList();
}

void CUnBond::ReflashSelectList()
{
	CArray<CCodeData, CCodeData>* ptrArr = NULL;
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	CStringArray arr;
	CString	 strkey, tmpS, strtemp;
	POSITION pos = 0;
	
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
	/*
	CRect rc;
	GetClientRect(&rc);

	SIZE size = Global.GetBitmapSize(IDX_BMP_BTN_BASE);

	int x=0, y=0, cx=0, cy=0;

	x = 20;
	y = 20;
	cx = rc.Width() - 20*2;
	cy = rc.Height() - 20 - 10*2 - size.cy;
	m_list.MoveWindow(x, y, cx, cy);
	ResizeListColumn(m_list, colWidthList);

	x = rc.Width() - 20 - size.cx;
	y = rc.Height() - 10 - size.cy;
	m_btnCancel.MoveWindow(x, y, size.cx, size.cy, TRUE);
	m_btnCancel.Invalidate();
	
	x -= size.cx + 10;
	m_btnOk.MoveWindow(x, y, size.cx, size.cy, TRUE);
	m_btnOk.Invalidate();
	*/
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
	pDC->FillSolidRect(rc, RGB(255,255,255));	
	return TRUE;
#else
	return TRUE;
#endif
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CUnBond::OnBnClickedBtnApply()
{
	
}


BOOL CUnBond::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam  == VK_RETURN)
		{
		//	SearchData();	
		}
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


void CUnBond::FillCodelistByGroup(CString strgroup)
{
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	CString	 strkey, tmpS;
	POSITION pos;
	m_bWorking = true;
	int i = 0;
	CListCtrl* plist = GetVisibleList();
	plist->DeleteAllItems();
	if(m_mapGroupCodemap.Lookup(strgroup, pmap))
	{
		int ii = 0;
		CCodeData cd;
		CJCODESTR* cjCode = NULL;
	
		for (pos = pmap->GetStartPosition(); pos;)
		{	
			pmap->GetNextAssoc(pos, strkey, cd);
			cjCode = GetCodeData(cd.strCode);
			if(cjCode != NULL)
			{
				LISTITEM* pitem;
				plist->InsertItem(ii, _T(""));
				plist->SetItemText(ii, 1, cjCode->name);
				plist->SetItemText(ii, 2, cjCode->idct);
				plist->SetItemText(ii, 3, cjCode->mtry);
				plist->SetItemText(ii, 4, cjCode->mtday);
				plist->SetItemText(ii, 5, cjCode->itrt);

				plist->SetItemText(ii, 6, cjCode->intertype);
	
				plist->SetItemText(ii, 7, cjCode->minthree);	
				plist->SetItemText(ii, 8, cjCode->minfore);
				plist->SetItemText(ii, 9, cjCode->credit);
				plist->SetItemText(ii, 10, cjCode->code);
				plist->SetItemText(ii, 11, cjCode->valday);
				plist->SetItemText(ii, 12, cjCode->update);

				pitem = new LISTITEM;
				strncpy((char*)pitem->Code, cjCode->code,cjCode->code.GetLength());
				strncpy((char*)pitem->idct, cjCode->idct,min(cjCode->code.GetLength(), sizeof(pitem->idct)/sizeof(char*)));  //지표구분  
				strncpy((char*)pitem->mtry, cjCode->mtry,min(cjCode->mtry.TrimLeft().TrimRight().GetLength(), sizeof(pitem->mtry)/sizeof(char*)));  //만기구분
				strncpy((char*)pitem->man, cjCode->mtday, min(cjCode->mtday.TrimLeft().TrimRight().GetLength(), sizeof(pitem->man)/sizeof(char*))); //만기일
				strncpy((char*)pitem->itrt, cjCode->itrt, min(cjCode->itrt.TrimLeft().TrimRight().GetLength(), sizeof(pitem->itrt)/sizeof(char*)));  //금리
				strncpy((char*)pitem->intertype, cjCode->intertype, min(cjCode->intertype.TrimLeft().TrimRight().GetLength(), sizeof(pitem->intertype)/sizeof(char*)));  //이자유형
				strncpy((char*)pitem->minfore, cjCode->minfore, min(cjCode->minfore.TrimLeft().TrimRight().GetLength(), sizeof(pitem->minfore)/sizeof(char*)));  //민평4
				strncpy((char*)pitem->minthree, cjCode->minthree, min(cjCode->minthree.TrimLeft().TrimRight().GetLength(), sizeof(pitem->minthree)/sizeof(char*)));  //민평3
				strncpy((char*)pitem->credit, cjCode->credit, min(cjCode->credit.TrimLeft().TrimRight().GetLength(), sizeof(pitem->credit)/sizeof(char*)));  //신용등급
				strncpy((char*)pitem->Name, cjCode->name,min(cjCode->name.TrimLeft().TrimRight().GetLength(), sizeof(pitem->Name)/sizeof(char*)));	  //종목명
				strncpy((char*)pitem->val, cjCode->valday,min(cjCode->valday.TrimLeft().TrimRight().GetLength(), sizeof(pitem->val)/sizeof(char*)));	  //발행일
				plist->SetItemData(ii, (LPARAM)pitem);
			}
		}
	}
	m_bWorking = false;
}

CJCODESTR* CUnBond::GetCodeData(CString strcode)
{
	CJCODESTR* cjCode = NULL;
	
	for(int ii = 0 ; ii < m_arrlist.GetSize() ; ii++)
	{
		cjCode = m_arrlist.GetAt(ii);
		if(cjCode->code == strcode)
			return cjCode;
	}
	return NULL;
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
	if(m_bDonwlading)  //종목리스트에 insert 할때는 막는다
		return;

	m_iSelectRow = -1;
	Invalidate();

	CString tmpS, tempS;
	CString strSearch(_T(""));

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
	if(((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
		bstrip = true;

	int icnt = 0;

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
	CArray<CJCODESTR*, CJCODESTR*>	editArr;  //조회버튼을 눌렀을때 에디트의 값을 포함하는 것만 넣어준다
	CArray<CJCODESTR*, CJCODESTR*>	Arr;

	switch(m_typecode)
	{
		case GUKCODE:   //국채
			Arr.Copy(m_arrGuklist);
			break;
		case JICODE:   //지방
			Arr.Copy(m_arrJilist);
			break;
		case TUKCODE:    //특수
			Arr.Copy(m_arrTuklist);
			break;
		case TONGCODE:   //통안
			Arr.Copy(m_arrTonglist);
			break;
		case GUMCODE:   //금융
			Arr.Copy(m_arrGumlist);
			break;
		case WHECODE:   //회사
			Arr.Copy(m_arrHeuilist);
			break;
		case ALLCODE:
			Arr.Copy(m_arrlist);
			break;
		case DANGCODE:
			{
				CString strtmps, tmpS;
				CTime ctstart, ctend;
				ctstart = CTime::GetCurrentTime();
				strtmps.Format(_T("%d-%02d-%02d\n"), ctstart.GetYear(), ctstart.GetMonth(), ctstart.GetDay());
				for(int ii = 0 ; ii < m_arrlist.GetSize(); ii++)
				{
					CJCode = m_arrlist.GetAt(ii);
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
				for(int ii = 0 ; ii < m_arrlist.GetSize(); ii++)
				{
					CJCode = m_arrlist.GetAt(ii);
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
		/*
		if(bstrip)
		{
			//스트립채권은 국고채에서만 있다. 따라서 전체나 국고채가 아닌 상태에서는 그냥 없다
			if(m_typecode != GUKCODE && m_typecode != ALLCODE)   
			{
				GetVisibleList()->DeleteAllItems();
				return;
			}
		}
		*/

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
		CJCODESTR* CJCode = NULL;
		bool bCode = false;

		if(strSearch.Left(2) == "KR" && strSearch.GetLength() == 12)
			bCode = true;

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
	//	for(int ii = 0 ; ii < tmpArr.GetSize(); ii++)
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
			strncpy((char*)pitem->val, CJCode->valday,min(CJCode->valday.TrimLeft().TrimRight().GetLength(), sizeof(pitem->val)/sizeof(char*)));	  //발행일
			plist->SetItemData(ii, (LPARAM)pitem);
		}

		if(tmpArr.GetSize() > LISTCNT)
		{
			CMapWnd* pMapWnd = (CMapWnd*)GetParent();
			m_title = "종목로드중";
			m_bDonwlading = true;
			pMapWnd->SetTitle(m_title);
			m_cpArr.Copy(tmpArr);
			m_iListcnt = LISTCNT;
			m_iTotalCnt = tmpArr.GetCount();

			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
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
	//SearchData(arrtemp);

	CString strSearch(_T(""));
	m_Search.GetWindowText(strSearch);

	//strSearch.MakeUpper();			// 2009/08/31 KJH ADD - UPPERCASE

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
	/*
	BOOL bNumeric = IsNumeric((LPCTSTR)strSearch.Left(3));
	BOOL bFind = FALSE;
	bool isHan = false;

	char* test = (LPSTR)strSearch.operator LPCTSTR();
	CString tmpS = _T("");

	TRACE(tmpS);
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

	plist->SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	plist->SetSelectionMark(nIndex);

	CRect rcItem;
	plist->GetItemRect(0, &rcItem, LVIR_LABEL);
	CSize sz(0, rcItem.Height() * (nIndex - plist->GetTopIndex()));
				
	plist->Scroll(sz);

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

	/*
	if(m_ckval.GetCheck())
	{
		CListCtrl* plist = GetVisibleList();
		m_sort = false;
		ListView_SortItems(plist->GetSafeHwnd(), ListViewCompareFunc1, (LPARAM)11);
	}
	*/
}


void CUnBond::OnBnClickedChkMan()
{
	SearchData();
	OnChangeEditCode();

	/*
	if(m_ckman.GetCheck())
	{
		CListCtrl* plist = GetVisibleList();
		m_sort = false;
		ListView_SortItems(plist->GetSafeHwnd(), ListViewCompareFunc1, (LPARAM)4);
	}
	*/
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
												m_UnBListCtrl.GetItemText(ii, 8),
												m_UnBListCtrl.GetItemText(ii, 5),
												m_UnBListCtrl.GetItemText(ii, 11));
				m_pMain->SendMessage(WM_USER + 500, axADDINTERSET,  (LPARAM)tmpS.operator LPCSTR());
				m_UnBListCtrl.SetCheck(ii, false);
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
	//if(!m_bINTERMODE)
	//	MoveAllLeft();
	//else
	//	MoveAllRight();
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

	switch(type)
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

void CUnBond::OnBnClickedBtnAll()
{
	m_typecode = ALLCODE;
	m_szIntergroup.Empty();
	//FillListdata_thread(m_typecode);
	SetButtonPress(m_typecode);
	AllListSet();
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
	
	//if (nCtlColor == CTLCOLOR_STATIC)
	{
		switch (pWnd->GetDlgCtrlID())
		{
			case IDC_STATIC_INTER:
				{
					pDC->SetBkMode(TRANSPARENT);
					pDC->SetTextColor(RGB(0, 0, 0));
					hbr = m_pBrush->operator HBRUSH();
				}
				break;
			case IDC_STATIC:
			case IDC_CHK_VAL:
			case IDC_CHK_MAN:	
			case IDC_STATIC_STRIP:
			case IDC_STATIC_MAN:
			case IDC_STATIC_BAL:
				//if(pWnd->GetDlgCtrlID() == IDC_STATIC)		
				pDC->SetBkMode(TRANSPARENT);
				pDC->SetTextColor(RGB(75, 96, 139));
				pDC->SetBkColor(RGB(249,249,249));
				hbr = m_pCtlBrush->operator HBRUSH();
				break;
		}
	}
	return hbr;
}


void CUnBond::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	//IDC_BTN_BAL
	/*
	CRect rc;
	((CWnd*)GetDlgItem(IDC_BTN_BAL))->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.OffsetRect(0, 27);
	rc.bottom -= 3;
	rc.right -= 1;
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(183,187,209));

	CPen *pOldPen = dc.SelectObject(&pen);
	dc.MoveTo(rc.left, rc.top);
	dc.LineTo(rc.right, rc.top);

	dc.MoveTo(rc.left, rc.top);
	dc.LineTo(rc.left, rc.bottom);

	dc.MoveTo(rc.left, rc.bottom);
	dc.LineTo(rc.right, rc.bottom);

	dc.MoveTo(rc.right, rc.bottom);
	dc.LineTo(rc.right, rc.top);

	dc.SelectObject(pOldPen);
	*/
}


void CUnBond::OnBnClickedBtnLook()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
}


void CUnBond::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TM_START:
		{
			KillTimer(TM_START);
			if(m_slastbtn.IsEmpty())
				OnBnClickedBtnGuk();
			else
			{
				switch(atoi(m_slastbtn))
				{
					case btnAll:
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
				strncpy((char*)pitem->val, CJCode->valday,min(CJCode->valday.TrimLeft().TrimRight().GetLength(), sizeof(pitem->val)/sizeof(char*)));	  //발행일
				plist->SetItemData(ii, (LPARAM)pitem);
			}

			if(blast)
			{
				KillTimer(TM_LOAD);
				KillTimer(TM_TITLE);
				int irowcnt = plist->GetItemCount();
				if(m_iTotalCnt < plist->GetItemCount())
				{
				//	for(int ii = m_iTotalCnt ;  ii < irowcnt ; ii++)
				//		plist->DeleteItem(ii);
				}
				irowcnt = plist->GetItemCount();
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				CMapWnd* pMapWnd = (CMapWnd*)GetParent();
				pMapWnd->SetTitle("종목검색");
				m_bDonwlading = false;
			}
		}
		break;
	case TM_TITLE:
		{
			CMapWnd* pMapWnd = (CMapWnd*)GetParent();
			m_title += ".";
			if(m_title == "종목로드중.....")
				m_title = "종목로드중";
			/*
			int icnt = m_title.ReverseFind('중');

			if(icnt == 3)
				m_title = "종목검색";
			else
				m_title += ".";

			if(m_title == "종목검색....")
				m_title = "종목검색.";
				*/
			pMapWnd->SetTitle(m_title);
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CUnBond::OnBnClickedCheck1()
{
	SearchData();
	OnChangeEditCode();
}

void CUnBond::SetHeaderCheck(BOOL bFlag)
{
	HDITEM hditem = {0,};
	DWORD dwHeaderStyle = ::GetWindowLong(ListView_GetHeader(m_AllList.IsWindowVisible() == TRUE ? m_AllList : m_UnBListCtrl), GWL_STYLE);
	dwHeaderStyle |= HDS_CHECKBOXES;

	hditem.mask = HDI_FORMAT;
	Header_GetItem(ListView_GetHeader(m_AllList.IsWindowVisible() == TRUE ? m_AllList : m_UnBListCtrl), 0, &hditem);

	if(bFlag)
		hditem.fmt |= HDF_CHECKED;
	else
		hditem.fmt &= ~HDF_CHECKED;
	
	Header_SetItem(ListView_GetHeader(m_AllList.IsWindowVisible() == TRUE ? m_AllList : m_UnBListCtrl), 0, &hditem);
}

void CUnBond::OnItemstateiconclickListUnbond(NMHDR *pNMHDR, LRESULT *pResult)
{
	CListCtrl* plist = GetVisibleList();

	if(m_bAllcheck)
	{
		for(int ii = 0 ; ii < plist->GetItemCount() ; ii++)
			m_UnBListCtrl.SetCheck(ii, FALSE);
		m_bAllcheck = false;
	}
	else
	{
		for(int ii = 0 ; ii < plist->GetItemCount() ; ii++)
			m_UnBListCtrl.SetCheck(ii, TRUE);
		m_bAllcheck = true;
	}
	
	*pResult = 0;
}

void CUnBond::OnItemstateiconclickListAllbond(NMHDR *pNMHDR, LRESULT *pResult)
{
	CListCtrl* plist = GetVisibleList();

	if(m_bAllcheck)
	{
		for(int ii = 0 ; ii < plist->GetItemCount() ; ii++)
			m_AllList.SetCheck(ii, FALSE);
		m_bAllcheck = false;
	}
	else
	{
		for(int ii = 0 ; ii < plist->GetItemCount() ; ii++)
			m_AllList.SetCheck(ii, TRUE);
		m_bAllcheck = true;
	}

	*pResult = 0;
}

void CUnBond::OnClickListAllbond(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CListCtrl* plist = GetVisibleList();

	if(pNMItemActivate->iSubItem != 0)
		m_AllList.SetCheck(pNMItemActivate->iItem, !m_AllList.GetCheck(pNMItemActivate->iItem));

	int ipredata = m_iSelectRow;
	m_iSelectRow = -1;
	if(ipredata != m_iSelectRow)
		m_AllList.Invalidate();

	BOOL bShift = (::GetKeyState(VK_SHIFT) & 0x8000);
	if(bShift)
	{
		for(int ii = 0 ; ii < m_UnBListCtrl.GetItemCount() ; ii++)
		{
			if(m_AllList.GetItemState(ii, LVIS_SELECTED) == LVIS_SELECTED)
				m_AllList.SetCheck(ii, true);
		}
	}

	*pResult = 0;
}

void CUnBond::OnClickListUnbond(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CListCtrl* plist = GetVisibleList();
	
	if(pNMItemActivate->iSubItem != 0)
		m_UnBListCtrl.SetCheck(pNMItemActivate->iItem, !m_UnBListCtrl.GetCheck(pNMItemActivate->iItem));

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

void CUnBond::OnDatetimechangeDate1val(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(((CButton*)GetDlgItem(IDC_CHK_VAL))->GetCheck())
		SearchData();
	*pResult = 0;
}


void CUnBond::OnDatetimechangeDate2man(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(((CButton*)GetDlgItem(IDC_CHK_MAN))->GetCheck())
		SearchData();
	*pResult = 0;
}


void CUnBond::OnDatetimechangeDate2val(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(((CButton*)GetDlgItem(IDC_CHK_VAL))->GetCheck())
		SearchData();
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
