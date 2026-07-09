// CmmdtDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "AxisCode.h"
#include "CmmdtDlg.h"
#include "afxdialogex.h"

// CCmmdtDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCmmdtDlg, CDialog)

CCmmdtDlg::CCmmdtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCmmdtDlg::IDD, pParent)
	, m_szEditCode(_T(""))
	, m_szEditName(_T(""))
{

}

CCmmdtDlg::CCmmdtDlg(CPoint pt, CWnd* pParent /*=NULL*/)
	: CDialog(CCmmdtDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElwDlg)
	//}}AFX_DATA_INIT

}

CCmmdtDlg::CCmmdtDlg(int kind, int type, CPoint pt,CWnd* pParent /*=NULL*/)
	: CDialog(CCmmdtDlg::IDD, pParent)
{
	m_szEditCode = "";
	m_szEditName = "";
	m_Kind    = kind;
	m_Point = pt;
	m_pCheck = NULL;
	m_szName = "";
	m_szCode = "";
	m_nCountMonth = 0;
}

CCmmdtDlg::~CCmmdtDlg()
{
}

void CCmmdtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ONE, m_BtnOne);
	DDX_Control(pDX, IDC_BUTTON_TWO, m_BtnTwo);
	DDX_Control(pDX, IDC_BUTTON_THREE, m_BtnThree);
	DDX_Control(pDX, IDC_BUTTON_FOUR, m_BtnFour);
	DDX_Control(pDX, IDC_BUTTON_FIVE, m_BtnFive);
	DDX_Control(pDX, IDC_BUTTON_SIX, m_BtnSix);
	DDX_Control(pDX, IDC_BUTTON_SEVEN, m_BtnSeven);
	DDX_Control(pDX, IDC_BUTTON_EIGHT, m_BtnEight);
	DDX_Control(pDX, IDC_COCODE_LIST, m_ListCOCode);
	DDX_Control(pDX, IDC_LIST_CODE, m_ListCtlCode);
	DDX_Text(pDX, IDC_EDIT_CODE, m_szEditCode);
	DDX_Text(pDX, IDC_EDIT_NAME, m_szEditName);
	DDX_Control(pDX, IDC_BUTTON_ONESP, m_BtnOneSP);
	DDX_Control(pDX, IDC_BUTTON_TWOSP, m_BtnTwoSP);
	DDX_Control(pDX, IDC_BUTTON_THREESP, m_BtnThreeSP);
	DDX_Control(pDX, IDC_BUTTON_FOURSP, m_BtnFourSP);
	DDX_Control(pDX, IDC_BUTTON_FIVESP, m_BtnFiveSP);
	DDX_Control(pDX, IDC_BUTTON_SIXSP, m_BtnSixSP);
	DDX_Control(pDX, IDC_BUTTON_SEVENSP, m_BtnSevenSP);
	DDX_Control(pDX, IDC_BUTTON_EIGHTSP, m_BtnEightSP);
}

BEGIN_MESSAGE_MAP(CCmmdtDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_COCODE_LIST, &CCmmdtDlg::OnLbnSelchangeCocodeList)
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_ONE, &CCmmdtDlg::OnBnClickedButtonOne)
	ON_BN_CLICKED(IDC_BUTTON_TWO, &CCmmdtDlg::OnBnClickedButtonTwo)
	ON_BN_CLICKED(IDC_BUTTON_THREE, &CCmmdtDlg::OnBnClickedButtonThree)
	ON_BN_CLICKED(IDC_BUTTON_FOUR, &CCmmdtDlg::OnBnClickedButtonFour)
	ON_BN_CLICKED(IDC_BUTTON_FIVE, &CCmmdtDlg::OnBnClickedButtonFive)
	ON_BN_CLICKED(IDC_BUTTON_SIX, &CCmmdtDlg::OnBnClickedButtonSix)
	ON_BN_CLICKED(IDC_BUTTON_SEVEN, &CCmmdtDlg::OnBnClickedButtonSeven)
	ON_BN_CLICKED(IDC_BUTTON_EIGHT, &CCmmdtDlg::OnBnClickedButtonEight)
	ON_WM_MOUSEHWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_ONESP, &CCmmdtDlg::OnBnClickedButtonOnesp)
	ON_BN_CLICKED(IDC_BUTTON_TWOSP, &CCmmdtDlg::OnBnClickedButtonTwosp)
	ON_BN_CLICKED(IDC_BUTTON_THREESP, &CCmmdtDlg::OnBnClickedButtonThreesp)
	ON_BN_CLICKED(IDC_BUTTON_FOURSP, &CCmmdtDlg::OnBnClickedButtonFoursp)
	ON_BN_CLICKED(IDC_BUTTON_FIVESP, &CCmmdtDlg::OnBnClickedButtonFivesp)
	ON_BN_CLICKED(IDC_BUTTON_SIXSP, &CCmmdtDlg::OnBnClickedButtonSixsp)
	ON_BN_CLICKED(IDC_BUTTON_SEVENSP, &CCmmdtDlg::OnBnClickedButtonSevensp)
	ON_BN_CLICKED(IDC_BUTTON_EIGHTSP, &CCmmdtDlg::OnBnClickedButtonEightsp)
END_MESSAGE_MAP()

// CCmmdtDlg 메시지 처리기입니다.

BOOL CCmmdtDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetDlgPos(m_Point.x, m_Point.y);
	SetCOCodes();
	m_ListCOCode.SetCurSel(0);

	InitBtn();
	SetCOCodeBtn();
	////////////////////////////////////////////////////////////////////////////////////////////////////
	CString configPath;
	char  wb[32];

	configPath.Format("%s\\tab\\config.ini", pApp->m_RootPath);
	DWORD dwRc = GetPrivateProfileString("OPTION", "SORT", "1", wb, sizeof(wb), configPath);

	if (dwRc <= 0) wb[0] = '1';

	m_bSort = atoi(&wb[0]);
	
	CString szTitle;
	szTitle = "상품 선물 코드 안내";
	m_pCheck = new CCheckBox;

	SetDlgPos(m_Point.x, m_Point.y);
	//CRect rc(110 + 17, 42 + (m_Kind!=optionCODE?24:0), 447+17+2, 267);
	CRect rc(163, 130, 501, 271);

	m_pCheck->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_GROUP, rc, this, IDC_WND_CHECK);
	m_pCheck->SetWnd(MMDTDLG);
	m_pCheck->ShowWindow(SW_SHOW);
	m_pCheck->SetWnd(this);

	if (m_Kind != futureCODE)
		m_pCheck->SetCheckCursor(IDC_CURSOR_HAND);

	//((CButton*)GetDlgItem(IDC_BUTTON_NAME))->ShowWindow(SW_HIDE);

	OnButtonCommdt();
	m_pCheck->EnableWindow(FALSE);
	SetWindowText(szTitle);
	//InitCtrl();
	//////////////////////////////////////////////////////////////////////////////////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCmmdtDlg::SetDlgPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	int	scWidth, scHeight;
	scWidth	= GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	//m_BtnOne.Init(bsTEXT|bsCOLOR|bsPUSH, "◀", "", 7,63,7);
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

void CCmmdtDlg::SetCOCodes()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	COCODE item;
	CString strItem("");
	int count = m_ListCOCode.GetCount();

	int idx = 0;
	int ii = 0;
	for (ii = 0; ii < count; ii++)
		m_ListCOCode.DeleteString(0);
	
	for (ii = 0; ii < pApp->m_arrayCOcode.GetSize(); ii++)
	{
		item = pApp->m_arrayCOcode.GetAt(ii);
		idx = item.hnam.Find('F',0);
		if(idx != LB_ERR)
		{
			strItem = item.hnam.Left(idx);
			if (m_ListCOCode.FindString(0, strItem) == LB_ERR && strItem != "미국달러 플렉스 " && strItem != "돈육   ")
				m_ListCOCode.AddString(strItem);
		}

		//if(idx == LB_ERR)
		//{
		//	idx = item.hnam.Find('S',0);
		//	if(idx != LB_ERR)
		//	{
		//		idx += 3;
		//	}
		//}

		//strItem = item.hnam.Left(idx);
		//if (m_ListCOCode.FindString(0, strItem) == LB_ERR)
		//	m_ListCOCode.AddString(strItem);
	}
}

void CCmmdtDlg::InitBtn()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*) AfxGetApp();
	int iID = IDC_BUTTON_ONE;
	CString strImg("");
	strImg.Format("%s/%s/", pApp->m_RootPath, "image");

	for(int i=0; i < BTNLEN; i++)
	{
		m_AryBtn[i] = (CfxButton*)GetDlgItem(iID++);
		m_AryBtn[i]->LoadImg(strImg);
		//m_AryBtn[i]->SetUser();
		//m_AryBtn[i]->ShowWindow(SW_HIDE);
	}

	iID = IDC_BUTTON_ONESP;

	for(int i=0; i < BTNLEN; i++)
	{
		m_AryBtnSP[i] = (CfxButton*)GetDlgItem(iID++);
		m_AryBtnSP[i]->LoadImg(strImg);
		//m_AryBtn[i]->ShowWindow(SW_HIDE);
	}
}
void CCmmdtDlg::SetCOCodeBtnInit()
{
}

void CCmmdtDlg::SetCOCodeBtn()
{
	//InitBtn();
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	CString strItem("");
	CString strBtnText("");
	m_ListCOCode.GetText(m_ListCOCode.GetCurSel(),strItem);
	COCODE item;
	int iChk = -1;
	int iID = 0;
	int iIDSP = 0;
	int iIdx = -1;
	//int 

	for (int ii = 0; ii < pApp->m_arrayCOcode.GetSize(); ii++)
	{
		item = pApp->m_arrayCOcode.GetAt(ii);
		iChk = item.hnam.Find(strItem,0);
		if(iChk != LB_ERR)
		{
			if( iID < BTNLEN )
			{
				iIdx = item.hnam.Find('F',0);
				if(iIdx != LB_ERR)
				{
					strBtnText = item.hnam.Mid(iIdx,item.hnam.GetLength() - iIdx);
					strBtnText.TrimRight();
					strBtnText.TrimLeft();
					m_AryBtn[iID]->SetWindowText(strBtnText);         
					if( m_AryBtn[iID]->IsWindowEnabled() )
						m_AryBtn[iID++]->ShowWindow(SW_SHOW);
				}
				//if( iID == BTNLEN) break;
			}

			if( iIDSP < BTNLEN )
			{
				iIdx = item.hnam.Find('S',0);
				if(iIdx != LB_ERR)
				{
					iIdx += 3;

					strBtnText = item.hnam.Mid(iIdx,item.hnam.GetLength() - iIdx);
					strBtnText.TrimRight();
					strBtnText.TrimLeft();
					m_AryBtnSP[iIDSP]->SetWindowText(strBtnText);         					
					if( m_AryBtnSP[iIDSP]->IsWindowEnabled() )
						m_AryBtnSP[iIDSP++]->ShowWindow(SW_SHOW);
				}
			}
		}
	}

	for(iID; iID < BTNLEN; iID++)
	{
		m_AryBtn[iID]->ShowWindow(SW_HIDE);
	}
	for(iIDSP; iIDSP < BTNLEN; iIDSP++)
	{
		m_AryBtnSP[iIDSP]->ShowWindow(SW_HIDE);
	}
}

void CCmmdtDlg::InitCtrl()
{
	CRect	rc;
	m_ListCtlCode.GetClientRect(&rc);

	//m_ListCtlCode.SetFont(&m_CtrlFont);
	m_ListCtlCode.SetBkColor(COR_WHITE);
	m_ListCtlCode.SetTextBkColor(COR_WHITE);
	m_ListCtlCode.SetTextColor(RGB(0,0,0));
	m_ListCtlCode.InsertColumn(1, "펀드코드", LVCFMT_CENTER, 60);
	m_ListCtlCode.InsertColumn(2, "종목명", LVCFMT_CENTER, rc.Width() - 80);
}

void CCmmdtDlg::OnLbnSelchangeCocodeList()
{
	SetCOCodeBtn();
}

void CCmmdtDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SAFE_DELETE(m_pCheck);
}

BOOL CCmmdtDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL CCmmdtDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::OnCommand(wParam, lParam);
}

void CCmmdtDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnLButtonUp(nFlags, point);
}

void CCmmdtDlg::GetNameNCode(int iBtn)
{
	CString strList("");
	CString strBtn("");
	CString strText("");

	m_ListCOCode.GetCurSel();
	m_ListCOCode.GetText(m_ListCOCode.GetCurSel(),strList);
	m_AryBtn[iBtn]->GetWindowText(strBtn);

	strText = strList + strBtn;
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();

	COCODE item;
	for (int ii = 0; ii < pApp->m_arrayCOcode.GetSize(); ii++)
	{
		item = pApp->m_arrayCOcode.GetAt(ii);

		if(item.hnam.Find(strText) != LB_ERR)
		{
			m_szCode.Format("%s",item.codx);
			m_szName.Format("%s",strText);
			break;
		}
	}
}

void CCmmdtDlg::GetNameNCodeSP(int iBtn)
{
	CString strList("");
	CString strBtn("");
	CString strText("");

	m_ListCOCode.GetCurSel();
	m_ListCOCode.GetText(m_ListCOCode.GetCurSel(),strList);
	strList += "SP ";
	m_AryBtnSP[iBtn]->GetWindowText(strBtn);

	strText = strList + strBtn;
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();

	COCODE item;
	for (int ii = 0; ii < pApp->m_arrayCOcode.GetSize(); ii++)
	{
		item = pApp->m_arrayCOcode.GetAt(ii);

		if(item.hnam.Find(strText) != LB_ERR)
		{
			m_szCode.Format("%s",item.codx);
			m_szName.Format("%s",strText);
			break;
		}
	}
}

void CCmmdtDlg::OnBnClickedButtonOne()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCode(0);
	OnOK();
}


void CCmmdtDlg::OnBnClickedButtonTwo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCode(1);
	OnOK();
}


void CCmmdtDlg::OnBnClickedButtonThree()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCode(2);
	OnOK();
}


void CCmmdtDlg::OnBnClickedButtonFour()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCode(3);
	OnOK();
}


void CCmmdtDlg::OnBnClickedButtonFive()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCode(4);
	OnOK();
}

void CCmmdtDlg::OnBnClickedButtonSix()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCode(5);
	OnOK();
}

void CCmmdtDlg::OnBnClickedButtonSeven()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCode(6);
	OnOK();
}


void CCmmdtDlg::OnBnClickedButtonEight()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCode(7);
	OnOK();
}

void CCmmdtDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 이 기능을 사용하려면 Windows Vista 이상이 있어야 합니다.
	// _WIN32_WINNT 기호는 0x0600보다 크거나 같아야 합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialog::OnMouseHWheel(nFlags, zDelta, pt);
}

void CCmmdtDlg::OnButtonCommdt()
{
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();

	if (m_pCheck->GetSafeHwnd())
		m_pCheck->SetArray();
	m_nCountMonth = 4;

	m_pCheck->SetInit(40);
	m_pCheck->SetScroll(0);

	m_pCheck->RemoveAll();
	m_pCheck->SetCount((int)pApp->m_arrayOcode.GetSize());
}

void CCmmdtDlg::Sort()
{
	m_bSort = !m_bSort;
	//switch (m_Kind)		// 종류에 따라서..
	//{
	//case faoptionCODE:
	//case optionCODE:
	//	OnButtonCommdt();
	//	break;
	//case koptionCODE:
	//	OnDblclkPcodeList();
	//	break;
	//}
	OnButtonCommdt();
}

CArray<CString, CString>* CCmmdtDlg::GetArray()
{ 
	CAxisCodeApp* pApp = (CAxisCodeApp*)AfxGetApp();
	return &pApp->m_ArrayOMonth; 
};


void CCmmdtDlg::OnBnClickedButtonOnesp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCodeSP(0);
	OnOK();
}


void CCmmdtDlg::OnBnClickedButtonTwosp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCodeSP(1);
	OnOK();
}


void CCmmdtDlg::OnBnClickedButtonThreesp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCodeSP(2);
	OnOK();
}


void CCmmdtDlg::OnBnClickedButtonFoursp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCodeSP(3);
	OnOK();
}


void CCmmdtDlg::OnBnClickedButtonFivesp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCodeSP(4);
	OnOK();
}


void CCmmdtDlg::OnBnClickedButtonSixsp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCodeSP(5);
	OnOK();
}


void CCmmdtDlg::OnBnClickedButtonSevensp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCodeSP(6);
	OnOK();
}


void CCmmdtDlg::OnBnClickedButtonEightsp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetNameNCodeSP(7);
	OnOK();
}
