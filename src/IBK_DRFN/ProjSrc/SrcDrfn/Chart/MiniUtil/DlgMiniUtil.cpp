// DlgMiniUtil.cpp : implementation file
//

#include "stdafx.h"
#include "MiniUtil.h"
#include "DlgMiniUtil.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CIRCLEBUTTON		2021 // IDC_STATIC_UTIL1		주기버튼
#define COMBO_DIVISION1		2011 // IDC_COMBO_DIVISION1		구분
#define COMBO_PERIOD1		2051 // IDC_COMBO_PERIOD1		기간
#define EDIT_CIRCLE1		2071 // IDC_EDIT_CIRCLE1		주기
#define SPIN1				2061 // IDC_SPIN1				주기 스핀컨트롤
#define COMBO_THEDAY		2081 //IDC_COMBO_THEDAY1		정체, 당일

#define MAXCOUNT	9   //총 열갯수

#define DIVISION	0
#define PERIOD		1

#define COLOR_MAIN_CANVAS		RGB(233, 233, 233)
#define COLOR_GRID				RGB(225, 225, 225)


const LPCSTR	NAME_DIVISION[] = { _T("주식"), _T("종합"), _T("선물"), _T("스타") };
const LPCSTR	NAME_PERIOD[]	  = { _T("틱"), _T("분"), _T("일"), _T("주"), _T("월") };
const LPCSTR	DEFAULT_PROPERTYINFO = "1틱;0;0;1;0;,1분;0;1;1;0;,3분;0;1;3;0;,5분;0;1;5;0;,10분;0;1;10;0;,30분;0;1;30;0;,일;0;2;1;1;,선1;2;0;1;0;,종1;1;0;1;0;,";

/////////////////////////////////////////////////////////////////////////////
// CDlgMiniUtil dialog


//CDlgMiniUtil::CDlgMiniUtil(long lArMiniSetupInfo, CWnd* pParent )
//	: CDialog(CDlgMiniUtil::IDD, pParent)
//{
//	//{{AFX_DATA_INIT(CDlgMiniUtil)
//		// NOTE: the ClassWizard will add member initialization here
//	//}}AFX_DATA_INIT
//	m_pArMiniSetupInfo = (CMiniSetupInfoArray*)lArMiniSetupInfo;
//
//
//	//m_pArMiniSetupInfo = NULL;
//}
CDlgMiniUtil::CDlgMiniUtil(CString strPropertyInfo, CWnd* pParent )
	: CDialog(CDlgMiniUtil::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMiniUtil)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strPropertyInfo = strPropertyInfo;
	if(strPropertyInfo.IsEmpty())
		m_strPropertyInfo = DEFAULT_PROPERTYINFO;
}


void CDlgMiniUtil::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMiniUtil)
	DDX_Control(pDX, IDC_LINE1, m_Line01);
	DDX_Control(pDX, IDC_LINE2, m_Line02);
	DDX_Control(pDX, IDC_LINE3, m_Line03);
	DDX_Control(pDX, IDC_LINE4, m_Line04);
	DDX_Control(pDX, IDC_LINE5, m_Line05);
	DDX_Control(pDX, IDC_LINE6, m_Line06);
	DDX_Control(pDX, IDC_LINE7, m_Line07);
	DDX_Control(pDX, IDC_LINE8, m_Line08);
	DDX_Control(pDX, IDC_LINE9, m_Line09);
	DDX_Control(pDX, IDC_LINE10, m_Line10);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgMiniUtil, CDialog)
	//{{AFX_MSG_MAP(CDlgMiniUtil)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(CBN_SELCHANGE, COMBO_DIVISION1, COMBO_DIVISION1 + 8, OnSelChangeComboDivision )
	ON_CONTROL_RANGE(CBN_SELCHANGE, COMBO_PERIOD1, COMBO_PERIOD1 + 8, OnSelChangeComboDivision )
	ON_CONTROL_RANGE(EN_UPDATE, EDIT_CIRCLE1, EDIT_CIRCLE1 + 8, OnEditCircleChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMiniUtil message handlers

BOOL CDlgMiniUtil::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_brushStatic.CreateSolidBrush(COLOR_MAIN_CANVAS);

	m_strDivision = "";

	SetPropertyInfo();
	SetInit();	

	GetDlgItem(CIRCLEBUTTON)->ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	ModifyLineStyle();
	return TRUE;       
}

void CDlgMiniUtil::ModifyLineStyle()
{
	int nIDCnt = 10;
	for(int i=0; i<nIDCnt; i++)
	{
		GetDlgItem(IDC_LINE1+i)->SetWindowText("");
	}
}

CString CDlgMiniUtil::TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/)
{
	CString strRes = _T("");
	int iCur = iStart;
	iCur = strSrc.Find(pszTokens, iStart);
	if(iCur != -1)	// tokenize 할 문자가 없을 경우에는 "" 리턴
	{
		strRes = strSrc.Mid(iStart, iCur-iStart);
		iCur++;
		iStart = iCur;
	}
	return strRes;
}

void CDlgMiniUtil::SetPropertyInfo()
{
	// String으로부터 MiniSetupInfo 구성
	CString strInfo;
	int nStart = 0;
	CMiniSetupInfo* pMiniSetupInfo = NULL;
	while(1)
	{
		strInfo = TokenizeEx(m_strPropertyInfo, ",", nStart);
		if(strInfo == "") break;
		
		pMiniSetupInfo = new CMiniSetupInfo;
		if(pMiniSetupInfo)
		{
			pMiniSetupInfo->ConvertStringToSetupInfo(strInfo);
			m_arrMiniSetupInfo.Add(pMiniSetupInfo);
		}
	}
}

CString CDlgMiniUtil::GetPropertyInfo()
{
	// MiniSetupInfo으로부터 String 구성
	CMiniSetupInfo* pMiniSetupInfo = NULL;
	m_strPropertyInfoOut.Empty();
	int nCount = m_arrMiniSetupInfo.GetSize();
	for(int nIndx = 0; nIndx < nCount; ++nIndx)
	{
		if(pMiniSetupInfo = m_arrMiniSetupInfo.GetAt(nIndx))
		{
			m_strPropertyInfoOut += pMiniSetupInfo->ConvertSetupInfoToString();
		}
	}

	return m_strPropertyInfoOut;
}

void CDlgMiniUtil::SetInit()
{
	// String으로부터 MiniSetupInfo 구성
	if(GetInfo() == FALSE)
		SetDefault();

	m_bLoad = TRUE;
}

void CDlgMiniUtil::SetDefault()
{

	//구분 초기화
	CComboBox *pCombo;
	int nIndex = 0;
	for( nIndex = COMBO_DIVISION1; nIndex <= COMBO_DIVISION1 + 8; nIndex++)
	{		
		pCombo = (CComboBox*)GetDlgItem(nIndex);
		pCombo->SetCurSel(0);
	}
	//기간 초기화
	for(nIndex = COMBO_PERIOD1; nIndex <= COMBO_PERIOD1 + 8; nIndex++)
	{		
		pCombo = (CComboBox*)GetDlgItem(nIndex);
		pCombo->SetCurSel(2);
	}
	//주기 초기화
	for(nIndex = CIRCLEBUTTON; nIndex <= CIRCLEBUTTON + 8; nIndex++)
	{
		GetDlgItem(nIndex)->SetWindowText("load");
	}
	//스핀버튼 설정
	CSpinButtonCtrl *pSpin;
	for(nIndex = SPIN1; nIndex <= SPIN1 + 8; nIndex++)
	{
		pSpin = (CSpinButtonCtrl*)GetDlgItem(nIndex);
		pSpin->SetRange(0, 100);
	}
	//당일, 전체 초기화
	for(nIndex = COMBO_THEDAY; nIndex <= COMBO_THEDAY + 8; nIndex++)
	{		
		pCombo = (CComboBox*)GetDlgItem(nIndex);
		pCombo->SetCurSel(0);
	}
}


void CDlgMiniUtil::VerifyCircleButton(UINT nID, int nComboNum)
{
	CString strFront = "";          //앞 문자
	CString strBack = "";			//뒷 문자
	CString strDivision = "";		//문자 조합

	int nDivisionID = 0;        //구분
	int nPeriodID = 0;			//기간
	int nCircleButtonID = 0;	//주기버튼(STATIC)
	int nThedayID = 0;			//당일, 전체
	int nEditCircleID = 0;		//주기 에디트박스
	int nSpinID = 0;			//주기 스핀컨트롤

	// 구분, 기간에 따른 같은 라인의 컨트롤 ID검색
	//nComboNum #0:구분 #1:기간
	if(nComboNum == DIVISION)
	{
		nDivisionID = nID;
		nPeriodID = COMBO_PERIOD1 + ( nID - COMBO_DIVISION1 );
		nCircleButtonID = CIRCLEBUTTON + ( nID - COMBO_DIVISION1 );
		nThedayID = COMBO_THEDAY + ( nID - COMBO_DIVISION1 );
		nEditCircleID = EDIT_CIRCLE1 + ( nID - COMBO_DIVISION1 );
		nSpinID = SPIN1 + ( nID - COMBO_DIVISION1 );
	}
	else if(nComboNum == PERIOD)
	{
		nDivisionID = COMBO_DIVISION1 + (nID - COMBO_PERIOD1);
		nPeriodID = nID;
		nCircleButtonID = CIRCLEBUTTON + ( nID - COMBO_PERIOD1 );
		nThedayID = COMBO_THEDAY + ( nID - COMBO_PERIOD1 );
		nEditCircleID = EDIT_CIRCLE1 + ( nID - COMBO_PERIOD1 );
		nSpinID = SPIN1 + ( nID - COMBO_PERIOD1 );
	}

	CComboBox *pCombo;
	pCombo = (CComboBox*)GetDlgItem(nDivisionID);
	pCombo->GetLBText(pCombo->GetCurSel(), strFront);

	pCombo = (CComboBox*)GetDlgItem(nPeriodID);
	pCombo->GetLBText(pCombo->GetCurSel(), strBack);
	
	//기간이 틱, 분 
	if(strBack == (CString)NAME_PERIOD[0] || strBack == (CString)NAME_PERIOD[1])
	{
		ShowControl(nEditCircleID, nSpinID, nThedayID);	
		if(strFront == (CString)NAME_DIVISION[0])
		{
			GetDlgItem(nEditCircleID)->GetWindowText(strFront);
			GetDlgItem(nPeriodID)->GetWindowText(strBack);
		}
		else
		{
			GetDlgItem(nDivisionID)->GetWindowText(strFront);
			GetDlgItem(nEditCircleID)->GetWindowText(strBack);
			strFront = strFront.Left(2);
		}
		m_strDivision = strFront + strBack;
		GetDlgItem(nCircleButtonID)->SetWindowText(m_strDivision);		
		
	}
	else //기간이 일, 주, 월
	{
		if(strFront == (CString)NAME_DIVISION[0])
			strFront = "";

		m_strDivision = strFront.Left(2) + strBack;
		GetDlgItem(nCircleButtonID)->SetWindowText(m_strDivision);

		HideControl(nEditCircleID, nSpinID, nThedayID);		
	}	
}

void CDlgMiniUtil::ShowControl(int nEditCircleID, int nSpinID, int nThedayID)
{
	GetDlgItem(nEditCircleID)->ShowWindow(SW_SHOW);
	GetDlgItem(nSpinID)->ShowWindow(SW_SHOW);
	GetDlgItem(nThedayID)->ShowWindow(SW_SHOW);
}
void CDlgMiniUtil::ShowControl(int nID)
{
	int nCircleButtonID = 0;	//주기버튼(STATIC)
	int nDivisionID = 0;        //구분
	int nPeriodID = 0;			//기간	
	int nThedayID = 0;			//당일, 전체
	int nEditCircleID = 0;		//주기 에디트박스
	int nSpinID = 0;			//주기 스핀컨트롤

	nDivisionID = COMBO_DIVISION1 + (nID - COMBO_PERIOD1);
	nPeriodID = nID;
	nCircleButtonID = CIRCLEBUTTON + ( nID - COMBO_PERIOD1 );
	nThedayID = COMBO_THEDAY + ( nID - COMBO_PERIOD1 );
	nEditCircleID = EDIT_CIRCLE1 + ( nID - COMBO_PERIOD1 );
	nSpinID = SPIN1 + ( nID - COMBO_PERIOD1 );

	GetDlgItem(nEditCircleID)->ShowWindow(SW_SHOW);
	GetDlgItem(nSpinID)->ShowWindow(SW_SHOW);
	GetDlgItem(nThedayID)->ShowWindow(SW_SHOW);
	
}

void CDlgMiniUtil::HideControl(int nEditCircleID, int nSpinID, int nThedayID)
{
	GetDlgItem(nEditCircleID)->ShowWindow(SW_HIDE);
	GetDlgItem(nSpinID)->ShowWindow(SW_HIDE);
	GetDlgItem(nThedayID)->ShowWindow(SW_HIDE);
}

void CDlgMiniUtil::HideControl(int nID)
{
	int nDivisionID = 0;        //구분
	int nPeriodID = 0;			//기간
	int nCircleButtonID = 0;	//주기버튼(STATIC)
	int nThedayID = 0;			//당일, 전체
	int nEditCircleID = 0;		//주기 에디트박스
	int nSpinID = 0;			//주기 스핀컨트롤

	nDivisionID = COMBO_DIVISION1 + (nID - COMBO_PERIOD1);
	nPeriodID = nID;
	nCircleButtonID = CIRCLEBUTTON + ( nID - COMBO_PERIOD1 );
	nThedayID = COMBO_THEDAY + ( nID - COMBO_PERIOD1 );
	nEditCircleID = EDIT_CIRCLE1 + ( nID - COMBO_PERIOD1 );
	nSpinID = SPIN1 + ( nID - COMBO_PERIOD1 );

	GetDlgItem(nEditCircleID)->ShowWindow(SW_HIDE);
	GetDlgItem(nSpinID)->ShowWindow(SW_HIDE);
	GetDlgItem(nThedayID)->ShowWindow(SW_HIDE);
}

void CDlgMiniUtil::OnSelChangeComboDivision(UINT nID)
{	
	int nComboNum = DIVISION;
	CString strChangeCombo;

	GetDlgItem(nID)->GetWindowText(strChangeCombo);

	CComboBox *pCombo;
	pCombo = (CComboBox*)GetDlgItem(nID);
	pCombo->GetLBText(pCombo->GetCurSel(), strChangeCombo);


	if(nID >= COMBO_DIVISION1 && nID <= COMBO_DIVISION1 + 8)
		nComboNum = DIVISION;
	else if(nID >= COMBO_PERIOD1 && nID <= COMBO_PERIOD1 + 8)
		nComboNum = PERIOD;

	VerifyCircleButton(nID, nComboNum);
}

void CDlgMiniUtil::OnEditCircleChange(UINT nID)
{
	if(m_bLoad == TRUE)
	{
		int nDivisionID = 0, nPeriodID = 0;
		int nEditCircleID = 0, nCircleButtonID = 0;
		CString strDivision;
		CString strFront, strBack;

		nDivisionID = COMBO_DIVISION1 + ( nID - EDIT_CIRCLE1 );
		nPeriodID = COMBO_PERIOD1 + ( nID - EDIT_CIRCLE1 );
		nEditCircleID = nID;
		nCircleButtonID = CIRCLEBUTTON + ( nID - EDIT_CIRCLE1 );

		CComboBox *pCombo;
		pCombo = (CComboBox*)GetDlgItem(nDivisionID);
		pCombo->GetLBText(pCombo->GetCurSel(), strDivision);	

		if(strDivision == (CString)NAME_DIVISION[0])
		{			
			GetDlgItem(nEditCircleID)->GetWindowText(strFront);
			GetDlgItem(nPeriodID)->GetWindowText(strBack);
		}
		else
		{
			strFront = strDivision.Left(2);
			GetDlgItem(nEditCircleID)->GetWindowText(strBack);
		}
		m_strDivision = strFront + strBack;
		GetDlgItem(nCircleButtonID)->SetWindowText(m_strDivision);
	}
	
}

void CDlgMiniUtil::SetInfo()
{
//	m_arrMiniSetupInfo.RemoveAll();	

	CComboBox* pCombo;	
	CMiniSetupInfo *pMiniSetupInfo;

	CString strTitle, strPeriod;
	
	for(int nIndex = 0; nIndex < MAXCOUNT  ; nIndex++)
	{
		pMiniSetupInfo = m_arrMiniSetupInfo.GetAt(nIndex);
	//	 pMiniSetupInfo = new CMiniSetupInfo();		
	
		GetDlgItem((int)CIRCLEBUTTON + nIndex )->GetWindowText(strTitle);
		pMiniSetupInfo->m_strTitle = strTitle;
		
		pCombo = (CComboBox*)GetDlgItem((int)COMBO_DIVISION1 + nIndex );
		pMiniSetupInfo->m_nMarketType = pCombo->GetCurSel();

		pCombo = (CComboBox*)GetDlgItem((int)COMBO_PERIOD1 + nIndex );
		pMiniSetupInfo->m_nTimeType = pCombo->GetCurSel();

		GetDlgItem((int)EDIT_CIRCLE1 + nIndex )->GetWindowText(strPeriod);
		pMiniSetupInfo->m_nPeriod = atoi(strPeriod);

		pCombo = (CComboBox*)GetDlgItem((int)COMBO_THEDAY + nIndex );
		pMiniSetupInfo->m_nDataOption = pCombo->GetCurSel();

		m_arrMiniSetupInfo.SetAt(nIndex, pMiniSetupInfo);
	}
}

BOOL CDlgMiniUtil::GetInfo()
{
	BOOL bResult = FALSE;

	CComboBox *pCombo;
	CEdit *pEdit;
	CSpinButtonCtrl *pSpin;

	CString strCircle, strTitle;
	CMiniSetupInfo *pMiniSetupInfo;
	pMiniSetupInfo = NULL;


	for(int nIndex = 0; nIndex < MAXCOUNT ; nIndex++)
	{
		 if(m_arrMiniSetupInfo.GetSize() != 0)
			 pMiniSetupInfo = m_arrMiniSetupInfo.GetAt(nIndex);
		 else 
			 return bResult;

		pCombo = (CComboBox*)GetDlgItem(CIRCLEBUTTON + nIndex );			
		pCombo->SetWindowText(pMiniSetupInfo->m_strTitle);
		 
		pCombo = (CComboBox*)GetDlgItem(COMBO_DIVISION1 + nIndex );		
		pCombo->SetCurSel(pMiniSetupInfo->m_nMarketType);

		pCombo = (CComboBox*)GetDlgItem(COMBO_PERIOD1 + nIndex );		
		pCombo->SetCurSel(pMiniSetupInfo->m_nTimeType);

		if(pMiniSetupInfo->m_nTimeType > 1)
			HideControl(COMBO_PERIOD1 + nIndex);
		else
			ShowControl(COMBO_PERIOD1 + nIndex);
		

		pEdit = (CEdit*)GetDlgItem(EDIT_CIRCLE1 + nIndex );	
		strCircle.Format("%d", pMiniSetupInfo->m_nPeriod);
		pEdit->SetWindowText(strCircle);

		pSpin = (CSpinButtonCtrl*)GetDlgItem(SPIN1 + nIndex );		
		pSpin->SetRange(0, 100);

		pCombo = (CComboBox*)GetDlgItem(COMBO_THEDAY + nIndex );		
		pCombo->SetCurSel(pMiniSetupInfo->m_nDataOption);		
	}

	bResult = TRUE;
	return bResult;
}

void CDlgMiniUtil::OnDestroy()
{
	RemoveAllArray();
	CDialog::OnDestroy();
}

void CDlgMiniUtil::OnBtnApply() 
{
	SetInfo();
	GetPropertyInfo();

	RemoveAllArray();
	OnOK();	
}

void CDlgMiniUtil::OnBtnCancel() 
{
	RemoveAllArray();
	OnCancel();
	
}

void CDlgMiniUtil::DrawLine(CDC* pDC)
{
	return;
	//CDC *pDC = GetDC(); 
	CRect rect; 
	GetWindowRect(rect); //그림판의 사각영역 설정
	ScreenToClient(rect);

	CDC BufferDC;                      //메모리 DC
	BufferDC.CreateCompatibleDC(pDC);  //화면 DC와 호환

	CBitmap bmpBuffer;				   //비트맵 객체	
	bmpBuffer.CreateCompatibleBitmap(pDC, rect.Width()-2, rect.Height()-2);
	CBitmap *pOldBitmap = (CBitmap*)BufferDC.SelectObject(&bmpBuffer);//비트맵 영역을 메모리 DC에 선택
//그리기
	CPen pen, *pOldPen;					//펜 설정
	pen.CreatePen(PS_SOLID, 1, RGB(130, 130, 130));
	BufferDC.SelectObject(&pen);
	pOldPen = (CPen*)BufferDC.SelectObject(&pen);


	GetClientRect(rect);
	CBrush br;
	br.CreateSolidBrush(COLOR_MAIN_CANVAS);
	BufferDC.SelectObject(br);
	BufferDC.Rectangle(rect); //바탕 화면 색깔 지정
	BufferDC.SetBkMode(TRANSPARENT);

	CRect rtStart, rtEnd;

	GetDlgItem(IDC_STATIC_HEAD1)->GetWindowRect(rtStart);
	ScreenToClient(rtStart);
	GetDlgItem(CIRCLEBUTTON + 8)->GetWindowRect(rtEnd);
	ScreenToClient(rtEnd);
	BufferDC.MoveTo(rtStart.left-1, rtStart.top);
	BufferDC.LineTo(rtEnd.left-1, rtEnd.bottom + 3);
	BufferDC.MoveTo(rtStart.right+1, rtStart.top);
	BufferDC.LineTo(rtEnd.right+1, rtEnd.bottom +3 );


	GetDlgItem(IDC_STATIC_HEAD2)->GetWindowRect(rtStart);
	ScreenToClient(rtStart);
	GetDlgItem(COMBO_DIVISION1 + 8)->GetWindowRect(rtEnd);
	ScreenToClient(rtEnd);
	BufferDC.MoveTo(rtStart.right+1, rtStart.top);
	BufferDC.LineTo(rtEnd.right+1, rtEnd.bottom );


	GetDlgItem(IDC_STATIC_HEAD3)->GetWindowRect(rtStart);
	ScreenToClient(rtStart);
	GetDlgItem(COMBO_PERIOD1 + 8)->GetWindowRect(rtEnd);
	ScreenToClient(rtEnd);
	BufferDC.MoveTo(rtStart.right+1, rtStart.top);
	BufferDC.LineTo(rtEnd.right+1, rtEnd.bottom );


	GetDlgItem(IDC_STATIC_HEAD4)->GetWindowRect(rtStart);
	ScreenToClient(rtStart);
	GetDlgItem(SPIN1 + 8)->GetWindowRect(rtEnd);
	ScreenToClient(rtEnd);
	BufferDC.MoveTo(rtStart.right+1, rtStart.top);
	BufferDC.LineTo(rtEnd.right , rtEnd.bottom );

	GetDlgItem(IDC_STATIC_HEAD5)->GetWindowRect(rtStart);
	ScreenToClient(rtStart);
	GetDlgItem(COMBO_THEDAY + 8)->GetWindowRect(rtEnd);
	ScreenToClient(rtEnd);
	BufferDC.MoveTo(rtStart.right+1, rtStart.top - 1);
	BufferDC.LineTo(rtEnd.right+1, rtEnd.bottom + 2);

	////////////
	GetDlgItem(IDC_STATIC_HEAD1)->GetWindowRect(rtStart);
	ScreenToClient(rtStart);
	GetDlgItem(IDC_STATIC_HEAD5)->GetWindowRect(rtEnd);
	ScreenToClient(rtEnd);
	BufferDC.MoveTo(rtStart.left - 1, rtStart.top - 1);
	BufferDC.LineTo(rtEnd.right + 1, rtEnd.top - 1);


	for(int nIndex = 0; nIndex < 9; nIndex+=2)
	{
		if(nIndex == 8)
		{
			GetDlgItem(CIRCLEBUTTON + nIndex)->GetWindowRect(rtStart);
			ScreenToClient(rtStart);
			GetDlgItem(COMBO_THEDAY + nIndex)->GetWindowRect(rtEnd);
			ScreenToClient(rtEnd);
			BufferDC.MoveTo(rtStart.left - 1, rtStart.top - 1);
			BufferDC.LineTo(rtEnd.right + 1, rtStart.top - 1 );
			BufferDC.MoveTo(rtStart.left - 1, rtStart.bottom + 3);
			BufferDC.LineTo(rtEnd.right + 1, rtStart.bottom + 3);
		}
		else
		{
			GetDlgItem(CIRCLEBUTTON + nIndex)->GetWindowRect(rtStart);
			ScreenToClient(rtStart);
			GetDlgItem(COMBO_THEDAY + nIndex)->GetWindowRect(rtEnd);
			ScreenToClient(rtEnd);
			BufferDC.MoveTo(rtStart.left - 1, rtStart.top - 1);
			BufferDC.LineTo(rtEnd.right + 1, rtStart.top - 1 );
			BufferDC.MoveTo(rtStart.left - 1, rtStart.bottom + 1);
			BufferDC.LineTo(rtEnd.right + 1, rtStart.bottom + 1);
		}		
	}
	pDC->BitBlt(0,0,rect.Width(), rect.Height(), &BufferDC, 0, 0, SRCCOPY);
	BufferDC.SelectObject(pOldPen);
	BufferDC.SelectObject(pOldBitmap);
	ReleaseDC(pDC);
}

BOOL CDlgMiniUtil::OnEraseBkgnd(CDC* pDC) 
{
	CBrush BackBrush(COLOR_MAIN_CANVAS);

	CRect rect, groupRect;
	pDC->GetClipBox(rect);
	pDC->FillRect(&rect, &BackBrush);

DrawLine(pDC);
	
	return TRUE;
}

HBRUSH CDlgMiniUtil::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC ) 
	{
		pDC->SetBkColor(COLOR_MAIN_CANVAS);
		pDC->SetBkMode(TRANSPARENT);		
		hbr = m_brushStatic;
	}

	return hbr;
}

void CDlgMiniUtil::RemoveAllArray()
{
	CMiniSetupInfo* pMiniSetupInfo = NULL;
	int nCount = m_arrMiniSetupInfo.GetSize();
	for(int nIndx = 0; nIndx < nCount; ++nIndx)
	{
		if(pMiniSetupInfo = m_arrMiniSetupInfo.GetAt(nIndx))
		{
			delete pMiniSetupInfo;
			pMiniSetupInfo = NULL;
		}
	}
	m_arrMiniSetupInfo.RemoveAll();
}