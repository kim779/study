// DlgTimeSetup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgTimeSetup.h"
#include "../../inc/IBaseDefine.h"
#include "../../inc/IKSLibMng.h"
#include ".\dlgtimesetup.h"
#define BACK_COLOR RGB(211,223,241)
#define CRBACKGROUND		RGB(177,199,234)
#define DEFAULT_BKGRND_COLOR		RGB(247,248,252)

// CDlgTimeSetup 대화 상자입니다.

UINT CDlgTimeSetup_ID1[] = {IDC_SUB_START0, IDC_SUB_END0, IDC_SUB_STEP0,
							IDC_SUB_START1, IDC_SUB_END1, IDC_SUB_STEP1,
							IDC_SUB_START2, IDC_SUB_END2, IDC_SUB_STEP2,
							IDC_SUB_START3, IDC_SUB_END3, IDC_SUB_STEP3,
							IDC_SUB_START4, IDC_SUB_END4, IDC_SUB_STEP4,
							IDC_SUB_START5, IDC_SUB_END5, IDC_SUB_STEP5,
							IDC_SUB_START6, IDC_SUB_END6, IDC_SUB_STEP6 };
UINT CDlgTimeSetup_ID2[] = {IDC_SUB_TITLE0, IDC_SUB_TITLE1, IDC_SUB_TITLE2, IDC_SUB_TITLE3, IDC_SUB_TITLE4, IDC_SUB_TITLE5, IDC_SUB_TITLE6};


IMPLEMENT_DYNAMIC(CDlgTimeSetup, CDialog)
CDlgTimeSetup::CDlgTimeSetup(CWnd* pParent /*=NULL*/)
	: CEscDialog(CDlgTimeSetup::IDD, pParent)
{
	m_crBk = CRBACKGROUND;
}

CDlgTimeSetup::~CDlgTimeSetup()
{
}

void CDlgTimeSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUB_TITLE0, m_ES_Condition0);
	DDX_Control(pDX, IDC_SUB_TITLE1, m_ES_Condition1);
	DDX_Control(pDX, IDC_SUB_TITLE2, m_ES_Condition2);
	DDX_Control(pDX, IDC_SUB_TITLE3, m_ES_Condition3);
	DDX_Control(pDX, IDC_SUB_TITLE4, m_ES_Condition4);
	DDX_Control(pDX, IDC_SUB_TITLE5, m_ES_Condition5);
	DDX_Control(pDX, IDC_STATIC_START, m_ES_Start);
	DDX_Control(pDX, IDC_STATIC_END, m_ES_End);
	DDX_Control(pDX, IDC_STATIC_INC, m_ES_Inc);
	DDX_Control(pDX, IDC_STATIC_NAME, m_ES_Name);
	DDX_Control(pDX, IDC_SZTITLE, m_ES_szTitle);
}


BEGIN_MESSAGE_MAP(CDlgTimeSetup, CDialog)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgTimeSetup 메시지 처리기입니다.

void CDlgTimeSetup::Reset()
{
	m_ES_szTitle.SetWindowText("");

	int nSizeID1 = sizeof(CDlgTimeSetup_ID1)/sizeof(UINT);
	int nSizeID2 = sizeof(CDlgTimeSetup_ID2)/sizeof(UINT);
	int i = 0;
	for( i=0; i<nSizeID1; i++) 
	{
		SetDlgItemText(CDlgTimeSetup_ID1[i], "");
		GetDlgItem(CDlgTimeSetup_ID1[i])->ShowWindow(SW_HIDE);
	}

	for(i=0; i<nSizeID2; i++) 
	{
		SetDlgItemText(CDlgTimeSetup_ID2[i], "");
		GetDlgItem(CDlgTimeSetup_ID2[i])->ShowWindow(SW_HIDE);
	}

	m_CFGWnd.Reset();
}

int CDlgTimeSetup::LoadCFG(LPCSTR szCFGFile, LPCSTR szSection)
{
	int nCount = m_CFGWnd.ReadCFG(szCFGFile, szSection);

	return nCount;
}

void CDlgTimeSetup::ParseCFG()
{

	m_ES_szTitle.SetWindowText(m_CFGWnd.m_ItemSectionInfo.m_szTilte);
	int nCount = m_CFGWnd.GetCount();
	POSITION pos=m_CFGWnd.m_ItemSectionInfo.m_ItemInfoList.GetHeadPosition();
	ST_VALUEOPTIMAZE st_val;
	m_ValueOptimizerArray.RemoveAll();
	CStatic* Static;
	CString str;
	for(int i=0; i<nCount; i++)
	{
		CCFGScanInfo* pInfo = m_CFGWnd.m_ItemSectionInfo.m_ItemInfoList.GetNext(pos);
		Static = m_ES_Array.GetAt(i);

		st_val.nTitle = pInfo->m_szTilte;
		Static->ShowWindow(SW_SHOW);
		Static->SetWindowText( pInfo->m_szTilte);
		UINT nID = i*3;
		GetDlgItem(CDlgTimeSetup_ID1[nID])->ShowWindow(SW_SHOW);
		SetDlgItemText(CDlgTimeSetup_ID1[nID], pInfo->m_szDefStart);

		GetDlgItem(CDlgTimeSetup_ID1[nID+1])->ShowWindow(SW_SHOW);
		SetDlgItemText(CDlgTimeSetup_ID1[nID+1], pInfo->m_szDefEnd);

		GetDlgItem(CDlgTimeSetup_ID1[nID+2])->ShowWindow(SW_SHOW);
		SetDlgItemText(CDlgTimeSetup_ID1[nID+2], pInfo->m_szDefStep);
		
		GetDlgItemText(CDlgTimeSetup_ID1[nID],str);
		st_val.nStart = atof(str);
		GetDlgItemText(CDlgTimeSetup_ID1[nID+1],str);
		st_val.nEnd = atof(str);
		GetDlgItemText(CDlgTimeSetup_ID1[nID+2],str);
		st_val.nAdv = atof(str);


		//DataType
		st_val.nDataType = pInfo->m_nDataType;
		m_ValueOptimizerArray.Add(st_val);
	}
}


 int CDlgTimeSetup::GetInputData(CCFGSectionInfo *pItemInfoList)
{
	int nCount = m_CFGWnd.GetCount();

	GetDlgItemText(IDC_SZTITLE, pItemInfoList->m_szTilte);

	for(int i=0; i<nCount; i++)
	{
		CCFGScanInfo* pInfo = new CCFGScanInfo;
		SetDlgItemText(CDlgTimeSetup_ID2[i], pInfo->m_szTilte);
		
		UINT nID = i*3;
		GetDlgItemText(CDlgTimeSetup_ID1[nID],   pInfo->m_szDefStart);
		GetDlgItemText(CDlgTimeSetup_ID1[nID+1], pInfo->m_szDefEnd);
		GetDlgItemText(CDlgTimeSetup_ID1[nID+2], pInfo->m_szDefStep);

		pInfo->m_fDefStart = atof(pInfo->m_szDefStart);
		pInfo->m_fDefEnd   = atof(pInfo->m_szDefEnd);
		pInfo->m_fDefStep  = atof(pInfo->m_szDefStep);

		pItemInfoList->m_ItemInfoList.AddTail(pInfo);
	}

	return nCount;
}
void CDlgTimeSetup::SetInputData()
{
	m_ValueOptimizerArray.RemoveAll();
	int nCount = m_CFGWnd.GetCount();
	POSITION pos=m_CFGWnd.m_ItemSectionInfo.m_ItemInfoList.GetHeadPosition();
	ST_VALUEOPTIMAZE st_val;
	CString str;
	CStatic* Static;

	for(int i=0; i<nCount; i++)
	{
		CCFGScanInfo* pInfo = m_CFGWnd.m_ItemSectionInfo.m_ItemInfoList.GetNext(pos);
		Static = m_ES_Array.GetAt(i);
		Static->SetWindowText( pInfo->m_szTilte);
		//SetDlgItemText(CDlgTimeSetup_ID2[i], pInfo->m_szTilte);
		
		UINT nID = i*3;
		GetDlgItemText(CDlgTimeSetup_ID1[nID],   pInfo->m_szDefStart);
		GetDlgItemText(CDlgTimeSetup_ID1[nID+1], pInfo->m_szDefEnd);
		GetDlgItemText(CDlgTimeSetup_ID1[nID+2], pInfo->m_szDefStep);

		GetDlgItemText(CDlgTimeSetup_ID1[nID],str);
		st_val.nStart = atof(str);
		GetDlgItemText(CDlgTimeSetup_ID1[nID+1],str);
		st_val.nEnd = atof(str);
		GetDlgItemText(CDlgTimeSetup_ID1[nID+2],str);
		st_val.nAdv = atof(str);

		//DataType
		st_val.nDataType = pInfo->m_nDataType;
		m_ValueOptimizerArray.Add(st_val);
		
		//delete pInfo;
	}
}
int CDlgTimeSetup::GetCount()
{
	int nCount = m_CFGWnd.GetCount();

	return nCount;
}


BOOL CDlgTimeSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//CCFGWndCreate test;
	//CString szRootDir = (LPCSTR)AfxGetPctrData(10);	// 10 : RootDir
	//CString strUserPath = szRootDir +"\\Data\\STData\\VarOptima.cfg";

	//Reset();
	//LoadCFG(strUserPath, "MACD기준선");
	//ParseCFG();
//	m_ES_Condition0.SetStaticColor(BACK_COLOR,BACK_COLOR,RGB(0,0,0));
//	m_ES_Condition1.SetStaticColor(BACK_COLOR,BACK_COLOR,RGB(0,0,0));
//	m_ES_Condition2.SetStaticColor(BACK_COLOR,BACK_COLOR,RGB(0,0,0));
//	m_ES_Condition3.SetStaticColor(BACK_COLOR,BACK_COLOR,RGB(0,0,0));
//	m_ES_Condition4.SetStaticColor(BACK_COLOR,BACK_COLOR,RGB(0,0,0));
//	m_ES_Condition5.SetStaticColor(BACK_COLOR,BACK_COLOR,RGB(0,0,0));
//	m_ES_Start.SetStaticColor(BACK_COLOR,BACK_COLOR,RGB(0,0,0));
//	m_ES_End.SetStaticColor(BACK_COLOR,BACK_COLOR,RGB(0,0,0));
//	m_ES_Inc.SetStaticColor(BACK_COLOR,BACK_COLOR,RGB(0,0,0));
//	m_ES_Name.SetStaticColor(BACK_COLOR,BACK_COLOR,RGB(0,0,0));
//	m_ES_szTitle.SetStaticColor(BACK_COLOR,BACK_COLOR,RGB(0,0,0));
	
//	IKSLibManager* pKSLibMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
//	if(pKSLibMng)
//		m_crBk = pKSLibMng->Lib_KSLink_GetColor(1);	// 바탕화면 배경
//	else
		m_crBk = DEFAULT_BKGRND_COLOR;
//	m_crBk = DEFAULT_BKGRND_COLOR;
	m_brBkColor.CreateSolidBrush(m_crBk);

	m_ES_Array.Add(&m_ES_Condition0);
	m_ES_Array.Add(&m_ES_Condition1);
	m_ES_Array.Add(&m_ES_Condition2);
	m_ES_Array.Add(&m_ES_Condition3);
	m_ES_Array.Add(&m_ES_Condition4);
	m_ES_Array.Add(&m_ES_Condition5);
	return TRUE;
}

HBRUSH CDlgTimeSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CEscDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		break;
	default:
//		if( pWnd->GetSafeHwnd() == m_ES_szTitle.GetSafeHwnd() ||
//			pWnd->GetSafeHwnd() == m_ES_Condition0.GetSafeHwnd() ||
//			pWnd->GetSafeHwnd() == m_ES_Condition1.GetSafeHwnd() ||
//			pWnd->GetSafeHwnd() == m_ES_Condition2.GetSafeHwnd() ||
//			pWnd->GetSafeHwnd() == m_ES_Condition3.GetSafeHwnd() ||
//			pWnd->GetSafeHwnd() == m_ES_Condition4.GetSafeHwnd() ||
//			pWnd->GetSafeHwnd() == m_ES_Condition5.GetSafeHwnd() ||
//			pWnd->GetSafeHwnd() == m_ES_Start.GetSafeHwnd() ||
//			pWnd->GetSafeHwnd() == m_ES_End.GetSafeHwnd() ||
//			pWnd->GetSafeHwnd() == m_ES_Inc.GetSafeHwnd() ||
//			pWnd->GetSafeHwnd() == m_ES_Name.GetSafeHwnd() )
			pDC->SetBkColor(m_crBk);
		return m_brBkColor;
	}
	return hbr;	
}

BOOL CDlgTimeSetup::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
			return TRUE;
	}

	return CEscDialog::PreTranslateMessage(pMsg);
}
