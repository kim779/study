// FilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrmWnd.h"
#include "SubFilterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;
/////////////////////////////////////////////////////////////////////////////
// CSubFilterDlg dialog

CSubFilterDlg::CSubFilterDlg(CWnd* pParent, CRect rePos, CString strType, 
							 CList <SubFilterInfo, SubFilterInfo&>	*plistSubFilterInfo, CWnd* pMainFrmWnd)
							 : CDialog(CSubFilterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubFilterDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
	m_strType = strType;
	m_pwndParent = pParent;
	m_plistSubFilterInfo = plistSubFilterInfo;
	m_rePos = rePos;
	m_pMainFrmWnd = pMainFrmWnd;
}


void CSubFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubFilterDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubFilterDlg, CDialog)
//{{AFX_MSG_MAP(CSubFilterDlg)
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_PAINT()
ON_WM_CTLCOLOR()
ON_WM_DESTROY()
//}}AFX_MSG_MAP

ON_BN_CLICKED(IDC_BTN_MODIFY, OnBtnModify)
ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)

ON_MESSAGE(WM_NOTIFY_GRID, OnNotify_Grid)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubFilterDlg message handlers

BOOL CSubFilterDlg::Create(CWnd* pParentWnd, UINT nID) 
{
	return CDialog::Create(IDD, pParentWnd);
}

int CSubFilterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pMainFrmWnd;
	
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	
	m_brushStatic.CreateSolidBrush(COLOR_DLG_CANVAS);

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	m_btnFilterGroup.Create(m_strType + " 필터", WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_GROUPIN);
	m_btnFilterGroup.SetFont(&m_fontDefault);
	
	m_gridSet.CreateGrid(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_GRID_SET);
	
	m_comboTime.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
		CRect(0, 0, 0, 0), this, IDC_COMBO_TIME);
	m_comboTime.SetFont(&m_fontDefault);
	
	m_ctrlTime.Create(_T("까지"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlTime.SetFont(&m_fontDefault);
	
	m_editVal.Create(WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this, IDC_EDIT_VAL);
	m_editVal.SetFont(&m_fontDefault);
	m_editVal.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	m_ctrlVal.Create(_T("천주 이상"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlVal.SetFont(&m_fontDefault);
	
	m_btnModify.Create(_T("추가/변경"), CRect(0, 0, 0, 0), this, IDC_BTN_MODIFY);
	pMainFrmWnd->SetBtnImg(&m_btnModify, 4);
	
	m_btnDelete.Create(_T("삭제"), CRect(0, 0, 0, 0), this, IDC_BTN_DELETE);
	pMainFrmWnd->SetBtnImg(&m_btnDelete, 4);
	
	m_btnOk.Create(_T("확인"), CRect(0, 0, 0, 0), this, IDC_BTN_OK);
	pMainFrmWnd->SetBtnImg(&m_btnOk, 4);
	
	m_btnCancel.Create(_T("취소"), CRect(0, 0, 0, 0), this, IDC_BTN_CANCEL);
	pMainFrmWnd->SetBtnImg(&m_btnCancel, 4);
	
	m_ctrlInfo.Create(_T("정보"), WS_CHILD | WS_VISIBLE | SS_CENTER, 
		CRect(0, 0, 0, 0), this);
	m_ctrlInfo.SetFont(&m_fontDefault);

	AfxSetResourceHandle(hInstSave);
	
	return 0;
}

void CSubFilterDlg::OnDestroy() 
{
	//	CDialog::OnDestroy();
	
	m_fontDefault.DeleteObject();
	m_brushStatic.DeleteObject();
	
	CDialog::OnDestroy();
}

BOOL CSubFilterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect reClient;
	//GetClientRect(reClient);
	
	reClient.top = m_rePos.top;
	reClient.left = m_rePos.left;
	
	reClient.bottom = reClient.top + 250;
	reClient.right = reClient.left + 335;
	MoveWindow(&reClient);
	
	SetInit();
	SetShow(m_strType, m_plistSubFilterInfo);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSubFilterDlg::SetInit()
{
	CString strTime;
	for (int nIndex = 0; nIndex < 6; nIndex++)
	{
		strTime.Format("%2d 시", nIndex + 10);
		m_comboTime.AddString(strTime);
	}
	m_comboTime.SetCurSel(0);
	
	m_editVal.SetWindowText(0);
	m_gridSet.SetInit("VOL");

	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pMainFrmWnd;

	ST_GRIDCOLOR stGridColor;
	stGridColor.clrGridHeader = pMainFrmWnd->GetAxColor(74);
	stGridColor.clrGridHilight = pMainFrmWnd->GetAxColor(78);
	
	m_gridSet.ChangeGridColor(stGridColor);
}

void CSubFilterDlg::SetShow(CString strType, CList <SubFilterInfo, SubFilterInfo&>	*plistSubFilterInfo)
{
	if (strType == "VOL")
	{
		m_btnFilterGroup.SetWindowText("거래량 필터");
		m_ctrlVal.SetWindowText("천주 이상");
		m_ctrlInfo.SetWindowText("검색 종목의 거래량 필터 조건을 설정합니다.");
		
		SetWindowText("거래량 필터 설정");
	}
	else if (strType == "PRICE")
	{
		m_btnFilterGroup.SetWindowText("거래대금 필터");
		m_ctrlVal.SetWindowText("백만원 이상");
		m_ctrlInfo.SetWindowText("검색 종목의 거래대금 필터 조건을 설정합니다.");
		
		SetWindowText("거래대금 필터 설정");
	}
	
	m_comboTime.SetCurSel(0);
	m_editVal.SetWindowText("");
	m_gridSet.HeaderSetup(strType);
	
	POSITION pos;
	SubFilterInfo	stSubFilterInfo;
	for (int nIndex = 0; nIndex < plistSubFilterInfo->GetCount(); nIndex++)
	{
		pos = plistSubFilterInfo->FindIndex(nIndex);
		stSubFilterInfo = plistSubFilterInfo->GetAt(pos);
		
		m_gridSet.SetAdd_Row(stSubFilterInfo.nTime, stSubFilterInfo.nData);
	}
	m_gridSet.RedrawAll();
	
	ShowWindow(SW_SHOW);
}

void CSubFilterDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect reClient, reBottom, reLeft;
	GetClientRect(reClient);
	
	reClient.top += 5;
	reClient.bottom -= 5;
	reClient.left += 5;
	reClient.right -= 5;
	m_btnFilterGroup.MoveWindow(&reClient);
	
	reClient.top += 20;
	reClient.bottom -= 5;
	reClient.left += 5;
	reClient.right -= 5;
	
	reBottom = reClient;
	
	reBottom.top = reBottom.bottom - 20;
	m_ctrlInfo.MoveWindow(&reBottom);
	
	reBottom.bottom  = reBottom.top - 5; 
	reBottom.top = reClient.top;
	
	reLeft = reBottom;
	
	reLeft.right = reLeft.left + 180;
	m_gridSet.MoveWindow(&reLeft);
	
	reLeft.left = reLeft.right + 5;
	reLeft.right = reBottom.right;
	OnSize_Set(reLeft);
}

void CSubFilterDlg::OnSize_Set(CRect reClient)
{
	CRect reTop, reTemp, reStatic;
	
	reTop = reClient;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 55;
	reTemp.bottom += 100;
	m_comboTime.MoveWindow(&reTemp);
	reTemp.bottom -= 100;
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 60;
	
	reStatic = reTemp;
	reStatic.top += 3;
	reStatic.bottom -= 2;
	m_ctrlTime.MoveWindow(&reStatic);
	
	reTop.top = reTop.bottom + 5;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 55;
	m_editVal.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 3;
	reTemp.right = reClient.right;
	reStatic = reTemp;
	reStatic.top += 3;
	reStatic.bottom -= 2;
	m_ctrlVal.MoveWindow(&reStatic);
	
	reTop.top = reTop.bottom + 70;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 60;
	m_btnModify.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 60;
	m_btnDelete.MoveWindow(&reTemp);
	
	reTop.top = reTop.bottom + 3;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.right = reTemp.left + 60;
	m_btnOk.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 2;
	reTemp.right = reTemp.left + 60;
	m_btnCancel.MoveWindow(&reTemp);
}


void CSubFilterDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pMainFrmWnd;

	CRect reClient;
	GetClientRect(reClient);
	//dc.FillSolidRect(reClient, COLOR_DLG_CANVAS);
	dc.FillSolidRect(reClient, pMainFrmWnd->m_clrMainSkin);
}

HBRUSH CSubFilterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pMainFrmWnd;

		pDC->SetBkMode(TRANSPARENT);
		//hbr = m_brushStatic;
		hbr = pMainFrmWnd->GetAxBrush(pMainFrmWnd->m_clrMainSkin);
	}
	
	return hbr;
}

void CSubFilterDlg::OnBtnModify()
{
	int nIndex = m_comboTime.GetCurSel();
	
	CString strData;
	m_editVal.GetWindowText(strData);
	
	m_gridSet.SetModify_Row(nIndex + 10, atoi(strData));
}

void CSubFilterDlg::OnBtnDelete()
{
	m_gridSet.SetDelete_Row(m_gridSet.m_nLastSelRow);
}

void CSubFilterDlg::OnBtnOk()
{
	m_plistSubFilterInfo->RemoveAll();
	
	SubFilterInfo	stSubFilterInfo;
	for (int nRow = 0; nRow < m_gridSet.GetNumberRows(); nRow++)
	{
		m_gridSet.GetData_Row(nRow, stSubFilterInfo.nTime, stSubFilterInfo.nData);
		m_plistSubFilterInfo->AddTail(stSubFilterInfo);
	}
	
	OnOK();
}

void CSubFilterDlg::OnBtnCancel()
{
	OnCancel();
}

//void CSubFilterDlg::OnNotify_Grid(WPARAM wParam, LPARAM lParam)
LRESULT CSubFilterDlg::OnNotify_Grid(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NM_ITEM_CLK)
	{
		int nRow = (int)lParam;
		int nData1, nData2;
		m_gridSet.GetData_Row(nRow, nData1, nData2);
		
		m_comboTime.SetCurSel(nData1 - 10);
		CString strTemp;
		strTemp.Format("%d", nData2);
		m_editVal.SetWindowText(strTemp);
	}

	return TRUE;
}