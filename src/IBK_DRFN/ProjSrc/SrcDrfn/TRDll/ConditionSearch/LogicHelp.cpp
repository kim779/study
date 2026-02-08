// LogicHelp.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LogicHelp.h"
#include "MainFrmWnd.h"

extern HINSTANCE g_hInstance;
// CLogicHelp 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogicHelp, CDialog)

CLogicHelp::CLogicHelp(CWnd* pParent /*=NULL*/)
	: CDialog(CLogicHelp::IDD, pParent)
{
	m_pWndMainFrm = pParent;

	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 12, 600, m_fontBold);
}

CLogicHelp::~CLogicHelp()
{
	m_brushStatic.DeleteObject();
}

void CLogicHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	/*DDX_Control(pDX, IDC_LIST1, m_ctrlList);*/
	/*DDX_Control(pDX, IDC_EDIT1, m_ctrlHelpText);*/
}


BEGIN_MESSAGE_MAP(CLogicHelp, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	ON_EN_CHANGE(IDC_EDIT1, OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, OnBtnOk)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()

	ON_WM_CREATE()
//	ON_WM_CREATE()
END_MESSAGE_MAP()


// CLogicHelp 메시지 처리기입니다.

void CLogicHelp::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*ChangeHelpText(m_ctrlList.GetCurSel());*/
}

void CLogicHelp::ChangeHelpText(int nSel)
{
	/*m_ctrlHelpText.SetWindowText((LPCTSTR)m_arrHelpText.GetAt(nSel));*/
	Invalidate();
}

void CLogicHelp::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

BOOL CLogicHelp::OnInitDialog()
{
	CDialog::OnInitDialog();

	CConditionMakeWnd* pMakeWnd = (CConditionMakeWnd*)m_pWndMainFrm;
	CMainFrmWnd* pMainWnd = (CMainFrmWnd*)pMakeWnd->m_pwndMain;
	m_clrMainSkin = pMainWnd->GetAxColor(66);

	m_brushStatic.CreateSolidBrush(m_clrMainSkin);

	m_btnOk.Create(_T("닫기"), CRect(0, 0, 0, 0), this, IDOK);
	pMakeWnd->SetBtnImg(&m_btnOk, 4);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
// 	m_ctrlList.AddString("1. 조건식");
// 	m_ctrlList.AddString("2. 조건식 그룹지정");
// 	m_ctrlList.AddString("3. 조합");
// 	m_ctrlList.AddString("4. 관심그룹");
// 	m_ctrlList.AddString("5. 지표식, A..z");

	CString szHelpText, strTemp;




// 	szHelpText.Format("\r\n%s\r\n\r\n%s\r\n\r\n%s\r\n", 
// 		"[조건식]",
// 		"조건실 필드에는 추가한 조건들이 약어 A, B, C 등으로 표시됩니다.",
// 		"각 약어들 사이의 and//or을 더블 클릭하면, 조합방법을 변경할 수 있습니다."
// 		);
// 	m_arrHelpText.Add(szHelpText);
// 
// 	szHelpText.Format("\r\n%s\r\n\r\n%s\r\n",
// 		"[조건식 그룹지정]",
// 		"마우스를 이용하여, 괄호(그룹)로 묶을 영역들을 드래그 합니다."
// 		);
// 	m_arrHelpText.Add(szHelpText);
// 
// 	szHelpText.Format("\r\n%s\r\n\r\n%s\r\n",
// 		"[조합]",
// 		"일반검색 조건식은 and//or의 복합조건으로 조합 가능하지만, 순차검색 조건식은 시간개념이므로 and조건으로만 조합 가능합니다."
// 		);
// 	m_arrHelpText.Add(szHelpText);
// 
// 	szHelpText.Format("\r\n%s\r\n\r\n%s\r\n\r\n%s\r\n",
// 		"[관심그룹]",
// 		"관심그룹을 지정하는 경우, 작성한 조건식은 지정한 관심종목그룹 내 종목 안에서 검색합니다.",
// 		"※ 관심그룹대상 조건식은 and//or의 복합조건으로 조회 불가능하며 and나 or조건으로만 조회 가능합니다.");
// 	m_arrHelpText.Add(szHelpText);
// 
// 	szHelpText.Format("\r\n%s\r\n\r\n%s\r\n",
// 		"[지표식] A..z",
// 		"한 조건식에서 등록 가능한 최대 조건 수는 48개입니다."
// 		);
// 	m_arrHelpText.Add(szHelpText);
	strTemp.Format("\r\n%s", "1. 조건식");
	/*m_ctrlHelpText.SetFont(&m_fontBold);*/
	/*m_ctrlHelpText.SetWindowText(strTemp);*/
	/*int nTemp = m_ctrlHelpText.GetWindowTextLength();*/
// 	m_ctrlHelpText.SetSel(0, nTemp);
// 	m_ctrlHelpText.SetSelectionCharFormat()


// 	strTemp.Format("\r\n%s\r\n%s\r\n\r\n", 
// 		"조건식 필드에는 추가한 조건들이 약어 A, B, C 등으로 표시됩니다.",
// 		"각 약어들 사이의 and/or을 더블 클릭하면, 조합방법을 변경할 수 있습니다."
// 		);
// 	m_ctrlHelpText.SetFont(&m_fontDefault, FALSE);
// 	int nLength = m_ctrlHelpText.GetWindowTextLength();
// 	m_ctrlHelpText.SetSel(nLength, nLength);
// 	m_ctrlHelpText.ReplaceSel(strTemp);

// 	szHelpText.Format("%s", strTemp);
// 	m_ctrlHelpText.SetWindowText(szHelpText);
// 	
// 	strTemp.Format("%s\r\n%s\r\n\r\n",
// 		"2. 조건식 그룹지정",
// 		"마우스를 이용하여, 괄호(그룹)로 묶을 영역들을 드래그 합니다."
// 		);
// 	szHelpText.Format("%s%s", szHelpText, strTemp);
// 	m_ctrlHelpText.SetWindowText(szHelpText);
// 	
// 	strTemp.Format("%s\r\n%s\r\n\r\n",
// 		"3. 조합",
// 		"일반검색 조건식은 and//or의 복합조건으로 조합 가능하지만, 순차검색 조건식은 시간개념이므로 and조건으로만 조합 가능합니다."
// 		);
// 	szHelpText.Format("%s%s", szHelpText, strTemp);
// 	m_ctrlHelpText.SetWindowText(szHelpText);
// 	
// 	strTemp.Format("%s\r\n%s\r\n%s\r\n\r\n",
// 		"4. 관심그룹",
// 		"관심그룹을 지정하는 경우, 작성한 조건식은 지정한 관심종목그룹 내 종목 안에서 검색합니다.",
// 		"※ 관심그룹대상 조건식은 and//or의 복합조건으로 조회 불가능하며 and나 or조건으로만 조회 가능합니다.");
// 	szHelpText.Format("%s%s", szHelpText, strTemp);
// 	m_ctrlHelpText.SetWindowText(szHelpText);
// 	
// 	strTemp.Format("%s\r\n%s",
// 		"5. 지표식, A..z",
// 		"한 조건식에서 등록 가능한 최대 조건 수는 25개입니다."
// 		);
// 	szHelpText.Format("%s%s", szHelpText, strTemp);
// 
// 	m_ctrlList.SetCurSel(0);
// 	m_ctrlList.ShowWindow(SW_HIDE);
// 	//m_ctrlHelpText.SetWindowText((LPCTSTR)m_arrHelpText.GetAt(0));
// 	m_ctrlHelpText.SetFont(&m_fontBold);
// 	m_ctrlHelpText.SetWindowText(szHelpText);
	
	Invalidate();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLogicHelp::OnBtnOk()
{
	OnOK();
}

void CLogicHelp::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect reClient;
	GetClientRect(reClient);
	dc.FillSolidRect(reClient, m_clrMainSkin);
}

HBRUSH CLogicHelp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
// 	if(pWnd->m_hWnd == m_ctrlList.m_hWnd)
// 	{
// 		pDC->SetBkMode(TRANSPARENT);
// 		pDC->SetBkColor(m_clrMainSkin);
// 		hbr = m_brushStatic;
// 	}
	
	return hbr;
}

void CLogicHelp::OnSize()
{
	CRect reClient, reLeft;
	GetClientRect(reClient);
	
	reClient.left += 5;
	reClient.top += 5;
	reClient.right -= 5;
	reClient.bottom -= 30;

	m_stcBkg.MoveWindow(reClient);

	reClient.left += 8;
	reClient.top += 8;
	reClient.right -= 8;
	
	reClient.bottom = reClient.top + 14;
	m_stcTitle1.MoveWindow(reClient);
	
	reClient.top = reClient.bottom;
	reClient.bottom = reClient.top + 14;
	m_stcInfo1_1.MoveWindow(reClient);

	reClient.top = reClient.bottom;
	reClient.bottom = reClient.top + 14;
	m_stcInfo1_2.MoveWindow(reClient);

	reClient.top = reClient.bottom + 5;
	reClient.bottom = reClient.top + 14;
	m_stcTitle2.MoveWindow(reClient);

	reClient.top = reClient.bottom;
	reClient.bottom = reClient.top + 14;
	m_stcInfo2_1.MoveWindow(reClient);

	reClient.top = reClient.bottom + 5;
	reClient.bottom = reClient.top + 14;
	m_stcTitle3.MoveWindow(reClient);

	reClient.top = reClient.bottom;
	reClient.bottom = reClient.top + 14;
	m_stcInfo3_1.MoveWindow(reClient);

	reClient.top = reClient.bottom+ 5;
	reClient.bottom = reClient.top + 14;
	m_stcTitle4.MoveWindow(reClient);

	reClient.top = reClient.bottom;
	reClient.bottom = reClient.top + 14;
	m_stcInfo4_1.MoveWindow(reClient);

	reClient.top = reClient.bottom + 5;
	reClient.bottom = reClient.top + 14;
	m_stcTitle5.MoveWindow(reClient);

	reClient.top = reClient.bottom;
	reClient.bottom = reClient.top + 14;
	m_stcInfo5_1.MoveWindow(reClient);

// 	reLeft = reClient;
// 	
// 	reLeft.top = reClient.bottom - 25;
// 	reLeft.bottom = reLeft.top + 20;
// 	
// 	reLeft.right = reClient.right - 5;
// 	reLeft.left = reLeft.right - 60;
// 	m_btnOk.MoveWindow(&reLeft);
// 	m_btnOk.ShowWindow(SW_HIDE);
	
	/*m_ctrlHelpText.ShowWindow(SW_HIDE);*/
	Invalidate();
}

int CLogicHelp::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) 
		return -1;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	CRect rtNULL(0, 0, 0, 0);
	
	m_stcBkg.Create(_T(""), WS_CHILD | WS_VISIBLE | WS_BORDER, rtNULL, this);
	m_stcBkg.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	m_stcTitle1.Create(_T("1. 조건식"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcTitle1.SetFont(&m_fontBold);
	m_stcTitle2.Create(_T("2. 조건식 그룹지정"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcTitle2.SetFont(&m_fontBold);
	m_stcTitle3.Create(_T("3. 조합"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcTitle3.SetFont(&m_fontBold);
	m_stcTitle4.Create(_T("4. 관심그룹"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcTitle4.SetFont(&m_fontBold);
	m_stcTitle5.Create(_T("5. 지표식, A..z"), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcTitle5.SetFont(&m_fontBold);

	m_stcInfo1_1.Create(_T("조건식 필드에는 추가한 조건들이 약어 A, B, C 등으로 표시됩니다."), 
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo1_1.SetFont(&m_fontDefault);
	
	m_stcInfo1_2.Create(_T("각 약어들 사이의 and/or을 더블 클릭하면, 조합방법을 변경할 수 있습니다."), 
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo1_2.SetFont(&m_fontDefault);
	
	m_stcInfo2_1.Create(_T("마우스를 이용하여, 괄호(그룹)로 묶을 영역들을 드래그 합니다."), 
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo2_1.SetFont(&m_fontDefault);
	
	m_stcInfo3_1.Create(_T("일반검색 조건식은 and/or의 복합조건으로 조합 가능하지만, 순차검색 조건식은 시간개념이므로 and조건으로만 조합 가능합니다."), 
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo3_1.SetFont(&m_fontDefault);
	
	m_stcInfo4_1.Create(_T("관심그룹을 지정하는 경우, 작성한 조건식은 지정한 관심종목그룹 내 종목 안에서 검색합니다."),
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo4_1.SetFont(&m_fontDefault);
	
	m_stcInfo4_2.Create(_T("※ 관심그룹대상 조건식은 and/or의 복합조건으로 조회 불가능하며 and나 or조건으로만 조회 가능합니다."), 
		WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo4_2.SetFont(&m_fontDefault);
	
	m_stcInfo5_1.Create(_T("한 조건식에서 등록 가능한 최대 조건 수는 25개입니다."), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, rtNULL, this);
	m_stcInfo5_1.SetFont(&m_fontDefault);

	AfxSetResourceHandle(hInstSave);

	OnSize();

	return 0;
}