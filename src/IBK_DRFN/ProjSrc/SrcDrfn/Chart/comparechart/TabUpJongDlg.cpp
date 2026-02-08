// TabUpJongDlg.cpp : implementation file
//

#include "stdafx.h"
#include "comparechart.h"
#include "TabUpJongDlg.h"
#include "../../inc/IGateMng.h"
#include "./define/ChartMsgDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabUpJongDlg dialog


CTabUpJongDlg::CTabUpJongDlg(CWnd* pParent /*=NULL*/)
	: CTabDlg(CTabUpJongDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabUpJongDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabUpJongDlg::DoDataExchange(CDataExchange* pDX)
{
	CTabDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabUpJongDlg)
	DDX_Control(pDX, IDC_LIST_UPJONG, m_listUpJongCode);
	DDX_Control(pDX, IDC_CHECK_KOSPI, m_BtnKospi);
	DDX_Control(pDX, IDC_CHECK_K200, m_BtnK200);
	DDX_Control(pDX, IDC_CHECK_KOSDAQ, m_BtnKosdaq);
//	DDX_Control(pDX, IDC_CHECK_KONEX, m_BtnKonex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabUpJongDlg, CTabDlg)
	//{{AFX_MSG_MAP(CTabUpJongDlg)
	ON_BN_CLICKED(IDC_CHECK_KOSPI, OnChkKospi)
	ON_BN_CLICKED(IDC_CHECK_K200, OnChkK200)
	ON_BN_CLICKED(IDC_CHECK_KOSDAQ, OnChkKosdaq)
//	ON_BN_CLICKED(IDC_CHECK_KONEX, OnChkKonex)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabUpJongDlg message handlers

BOOL CTabUpJongDlg::OnInitDialog() 
{
	CTabDlg::OnInitDialog();
	
	int nID = 5;
	IGateManager* pGateMng;	AfxGetIGateManager(pGateMng);
	m_pDataManager = (IMasterDataManagerLast*)pGateMng->GetInterface(nID);
	// TODO: Add extra initialization here

	//m_listUpJongCode.SetDBMgr(m_pDBMgr);
	m_listUpJongCode.SetListDataType(UPJONG);
	m_listUpJongCode.Init_ListCtrl();	
	//m_listUpJongCode.LoadCodeList(m_nOption);

	m_BtnKospi.FP_vSetImage( m_pIL_W46H20, 32, 34, TRUE, "코스피", TRUE);
	m_BtnK200.FP_vSetImage( m_pIL_W46H20, 32, 34, TRUE, "KRX", TRUE);
	m_BtnKosdaq.FP_vSetImage( m_pIL_W46H20, 32, 34, TRUE, "코스닥", TRUE);
//	m_BtnKonex.FP_vSetImage( m_pIL_W46H20, 32, 34, TRUE, "코넥스", TRUE);


	m_BtnKospi.SetCheck(TRUE);
	OnChkKospi();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabUpJongDlg::OnDestroy()
{
	fnRemovelistStockCode();

	CTabDlg::OnDestroy();
}

LRESULT CTabUpJongDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
		case UMSG_ADDITEM_BTNCLICK:
			if(!m_listUpJongCode.m_strCodeNName.GetLength()) return 0l;
			GetParent()->GetParent()->SendMessage(UMSG_ADDITEM, 0, (LPARAM)&(m_listUpJongCode.m_strCodeNName));
			break;				
//		case WMU_SET_KEYCODE:
//			SetKeyCodeChange(wParam, lParam);
//			break;
	//=================================================================
	}
	return CTabDlg::WindowProc(message, wParam, lParam);
}

BOOL CTabUpJongDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
//	if(pMsg->message == WM_KEYDOWN)
//	{
//		if(pMsg->wParam == VK_RETURN	|| 
//		   pMsg->wParam == VK_ESCAPE	) 
//		{
//			return TRUE;
//		}
//	}
//	
//	return CTabDlg::PreTranslateMessage(pMsg);

	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);
	else
		return CTabDlg::PreTranslateMessage(pMsg);
}

void CTabUpJongDlg::InitBaseInfo(int nType)
{
	//if(m_pDBMgr)
	//	m_BaseInfo.InitInfo((void*)((CDrMLib_CDBMgr*)m_pDBMgr)->GetDBLong());
	
//	CRect rc;
//	GetDlgItem(IDC_STATIC_UPCODE)->GetWindowRect(&rc);
//	rc.bottom += 100;
//	ScreenToClient(rc);	
//	GetDlgItem(IDC_STATIC_UPCODE)->ShowWindow(SW_HIDE);	
		
	
//	CString sCode;
//	char szBuf[20];	
//	CString sSearchOption;
//	int nKind = 0;
//	if(nType == UPALL)
//	{
//		nKind = CODE_UPJONG | CODE_K200UPJONG | CODE_KUPJONG;
//		sSearchOption = "CODE_UPJONG | CODE_K200UPJONG | CODE_KUPJONG";
//	}
//	else if(nType == UPKOSPI)
//	{
//		nKind = CODE_UPJONG | CODE_K200UPJONG;
//		sSearchOption = "CODE_UPJONG | CODE_K200UPJONG";
//	}
//	else if(nType == UPKOSDAQ)
//	{
//		nKind = CODE_KUPJONG;
//		sSearchOption = "CODE_KUPJONG";
//	}
//
//	//m_BaseInfo.CreateControl_CodeInput(GetSafeHwnd(), rc, NID_CODEINPUT, nKind);	
//	//m_BaseInfo.CreateControl_UpjongCodeCombo(GetSafeHwnd(), rc, NID_UPJONGINPUT, sSearchOption);
//
//	if( AfxGetMainWnd()->SendMessage(WMU_GET_KEYCODE, (WPARAM)nKind, (LPARAM)szBuf) == FALSE)
//		return;

//	 // 종목 코드 변경
//	 sCode.Format("%s",szBuf);
//	 sCode.TrimLeft();sCode.TrimRight();sCode.Remove(' ');
//	 
//	 //m_BaseInfo.SetTextToCodeInput(sCode);
//	 if(sCode.GetLength())
//	 {
//		m_strCodeFromHanhwa = sCode;		
//	 }
}

void CTabUpJongDlg::SetKeyCodeChange(WPARAM wp, LPARAM lp)
{
	CString strCode;
	strCode = (LPSTR)lp;	

	if(wp == NID_UPJONGINPUT)
	{
		m_listUpJongCode.ChangeCode(strCode);
	}	
}

void CTabUpJongDlg::OnChkK200() 
{
	// TODO: Add your control notification handler code here
	//m_nOption = UPALL;
	//InitBaseInfo(m_nOption);
	//m_listUpJongCode.LoadCodeList(m_nOption);
	
	// TODO: Add your control notification handler code here
	m_BtnKospi.SetCheck(FALSE);	
	m_BtnKosdaq.SetCheck(FALSE);	
//	m_BtnKonex.SetCheck(FALSE);
//	m_listUpJongCode.LoadCodeList(m_nOption);
	m_listUpJongCode.DeleteAllItems();
	fnRemovelistStockCode();

 	CStringArray* arCode;
	CStringArray* arName;
	CStringArray* arEngName;
	arCode = new CStringArray;
	arName = new CStringArray;
	arEngName = new CStringArray;
	int nIndex = 0;

	m_pDataManager->GetKRXUpMst(arCode, arName, arEngName,1);
	for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
	{
		MASTERCODE* pData = new MASTERCODE;
		pData->strCode = arCode->GetAt(nIndex);
		pData->strKor = arName->GetAt(nIndex);
		pData->strKor.TrimRight();
		pData->strEng = _T("");	
		m_listUpJongCode.m_AllArray.Add(pData);
	}
	m_listUpJongCode.SetItemCount(arCode->GetSize());		

	delete arCode;
	delete arName;
	delete arEngName;
}

void CTabUpJongDlg::OnChkKosdaq() 
{
	// TODO: Add your control notification handler code here
	m_BtnKospi.SetCheck(FALSE);	
	m_BtnK200.SetCheck(FALSE);	
//	m_BtnKonex.SetCheck(FALSE);
	//m_listUpJongCode.LoadCodeList(m_nOption);
	m_listUpJongCode.DeleteAllItems();
	fnRemovelistStockCode();
			
 	CStringArray* arCode;
	CStringArray* arName;
	CStringArray* arEngName;
	arCode = new CStringArray;
	arName = new CStringArray;
	arEngName = new CStringArray;
	int nIndex = 0;

	m_pDataManager->GetKSQUpMst(arCode, arName, arEngName,1);
	for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
	{
		MASTERCODE* pData = new MASTERCODE;
		pData->strCode = arCode->GetAt(nIndex);
		pData->strKor = arName->GetAt(nIndex);
		pData->strKor.TrimRight();
		pData->strEng = _T("");	
		m_listUpJongCode.m_AllArray.Add(pData);
	}
	m_listUpJongCode.SetItemCount(arCode->GetSize());		
	
	delete arCode;
	delete arName;
	delete arEngName;
}
/*
void CTabUpJongDlg::OnChkKonex() 
{
	// TODO: Add your control notification handler code here
	m_BtnKospi.SetCheck(FALSE);	
	m_BtnK200.SetCheck(FALSE);	
	m_BtnKosdaq.SetCheck(FALSE);
	//m_listUpJongCode.LoadCodeList(m_nOption);
	m_listUpJongCode.DeleteAllItems();
	fnRemovelistStockCode();
			
 	CStringArray* arCode;
	CStringArray* arName;
	CStringArray* arEngName;
	arCode = new CStringArray;
	arName = new CStringArray;
	arEngName = new CStringArray;
	int nIndex = 0;

	m_pDataManager->GetKNXUpMst(arCode, arName, arEngName,1);
	for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
	{
		MASTERCODE* pData = new MASTERCODE;
		pData->strCode = arCode->GetAt(nIndex);
		pData->strKor = arName->GetAt(nIndex);
		pData->strKor.TrimRight();
		pData->strEng = _T("");	
		m_listUpJongCode.m_AllArray.Add(pData);
	}
	m_listUpJongCode.SetItemCount(arCode->GetSize());		
	
	delete arCode;
	delete arName;
	delete arEngName;
}
*/
void CTabUpJongDlg::OnChkKospi() 
{
	// TODO: Add your control notification handler code here
	m_BtnKosdaq.SetCheck(FALSE);	
	m_BtnK200.SetCheck(FALSE);	
//	m_BtnKonex.SetCheck(FALSE);
//	m_listUpJongCode.LoadCodeList(m_nOption);
	m_listUpJongCode.DeleteAllItems();
	fnRemovelistStockCode();
			
	CStringArray* arCode;
	CStringArray* arName;
	CStringArray* arEngName;
	arCode = new CStringArray;
	arName = new CStringArray;
	arEngName = new CStringArray;
	int nIndex = 0;

	m_pDataManager->GetKSPUpMst(arCode, arName, arEngName,1);
	for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
	{
		MASTERCODE* pData = new MASTERCODE;
		pData->strCode = arCode->GetAt(nIndex);
		pData->strKor = arName->GetAt(nIndex);
		pData->strKor.TrimRight();
		pData->strEng = _T("");	
		m_listUpJongCode.m_AllArray.Add(pData);
	}
	int nKospiCnt;
	nKospiCnt = arCode->GetSize();

// 	m_pDataManager->GetK200UpMst(arCode, arName, arEngName);
// 	for(nIndex=0; nIndex<arCode->GetSize(); nIndex++)
// 	{
// 		MASTERCODE* pData = new MASTERCODE;
// 		pData->strCode = arCode->GetAt(nIndex);
// 		pData->strKor = arName->GetAt(nIndex);
// 		pData->strKor.TrimRight();
// 		pData->strEng = _T("");	
// 		m_listUpJongCode.m_AllArray.Add(pData);
// 	}
// 	nK200Cnt = arCode->GetSize();
	m_listUpJongCode.SetItemCount(nKospiCnt);	
	delete arCode;
	delete arName;
	delete arEngName;
}

void CTabUpJongDlg::OnSize(UINT nType, int cx, int cy) 
{
	CTabDlg::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_listUpJongCode.GetSafeHwnd())
	{
		CRect rect, listRect;
		GetClientRect(rect);

		m_listUpJongCode.GetWindowRect(listRect);
		ScreenToClient(listRect);
		listRect.left = rect.left + 1;
		listRect.right = rect.right;
		listRect.bottom = rect.bottom; 
		m_listUpJongCode.MoveWindow(listRect);
	}	
}

void CTabUpJongDlg::fnRemovelistStockCode()
{
	for( int i = 0; i < m_listUpJongCode.m_AllArray.GetSize(); i++)
		delete m_listUpJongCode.m_AllArray.GetAt(i);
	m_listUpJongCode.m_AllArray.RemoveAll();
}