// FinterSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FilterSetDlg.h"

#include "FilterSetWnd.h"
#include "MainFrmWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterSetDlg dialog

//#define	DLG_HEIGHT					355;
/*
#define	DLG_HEIGHT					404 - 44;
#define	DLG_WIDTH						600;
*/

#define	DLG_HEIGHT					265;
#define	DLG_WIDTH						295;

CFilterSetDlg::CFilterSetDlg(CWnd* pParent)
: CDialog(CFilterSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterSetDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pwndFilterSet = NULL;
	m_pParent = pParent;
}

void CFilterSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterSetDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterSetDlg, CDialog)
	//{{AFX_MSG_MAP(CFilterSetDlg)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(IDC_BTN_BASE, OnBtnBase)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)

END_MESSAGE_MAP()

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CFilterSetDlg message handlers
int CFilterSetDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->wParam == VK_TAB)
		return 0L;
	
	return CDialog::PreTranslateMessage(pMsg);
}

int CFilterSetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);

	if (m_pwndFilterSet == NULL)
	{
		m_pwndFilterSet = new CFilterSetWnd;
		m_pwndFilterSet->Create(this, IDC_WND_FILTERSET, m_pParent);
		m_pwndFilterSet->SetInit();
	}

	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pParent;

	m_btnBase.Create(_T("기본값"), CRect(0, 0, 0, 0), this, IDC_BTN_BASE);
	pMainFrmWnd->SetBtnImg(&m_btnBase, 4);

	m_btnCancel.Create(_T("취소"), CRect(0, 0, 0, 0), this, IDC_BTN_CANCEL);
	pMainFrmWnd->SetBtnImg(&m_btnCancel, 4);

	m_btnApply.Create(_T("적용"), CRect(0, 0, 0, 0), this, IDC_BTN_APPLY);
	pMainFrmWnd->SetBtnImg(&m_btnApply, 4);

	CRect reClient;
	GetClientRect(reClient);
	reClient.right = reClient.left + (int)DLG_WIDTH;
	reClient.bottom = reClient.top + (int)DLG_HEIGHT;
	MoveWindow(reClient);

	AfxSetResourceHandle(hInstSave);

	return 0;
}

BOOL CFilterSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText("필터 설정");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CFilterSetDlg::OnDestroy() 
{
	//	CDialog::OnDestroy();

	m_fontDefault.DeleteObject();

	if (m_pwndFilterSet != NULL)
	{
		m_pwndFilterSet->DestroyWindow();
		delete m_pwndFilterSet;
		m_pwndFilterSet = NULL;
	}

	CDialog::OnDestroy();
}

void CFilterSetDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CRect reClient, reBottom, reTemp;
	GetClientRect(reClient);

	reBottom = reClient;

	reBottom.bottom -= 5;
	reBottom.top = reBottom.bottom - 20;
	reTemp = reBottom;
	reTemp.left += 5;
	reTemp.right = reTemp.left + 60;
	m_btnBase.MoveWindow(&reTemp);

	reTemp = reBottom;
	reTemp.right -= 5;
	reTemp.left = reTemp.right - 60;
	m_btnCancel.MoveWindow(&reTemp);

	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 60;
	m_btnApply.MoveWindow(&reTemp);

	reBottom.bottom = reBottom.top - 3;
	reBottom.top = reClient.top;

	if (m_pwndFilterSet != NULL)
		m_pwndFilterSet->MoveWindow(&reBottom);
}

void CFilterSetDlg::OnBtnOk()
{
	if (m_pwndFilterSet != NULL)
	{
		if (m_pwndFilterSet->SetInfo_Filter(FALSE) == TRUE)
			OnOK();
	}
}

void CFilterSetDlg::OnBtnBase()
{
	if (m_pwndFilterSet != NULL)
		m_pwndFilterSet->SetInit_Base();
}

void CFilterSetDlg::OnBtnCancel()
{
	OnCancel();
}

void CFilterSetDlg::OnBtnApply()
{
	if (m_pwndFilterSet != NULL)
	{
		if (m_pwndFilterSet->SetInfo_Filter(FALSE) == TRUE)
			OnOK();
	}
}

void CFilterSetDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CMainFrmWnd* pMainFrmWnd = (CMainFrmWnd*)m_pParent;

	CRect reClient;
	GetClientRect(reClient);
	//dc.FillSolidRect(reClient, COLOR_DLG_CANVAS);

	dc.FillSolidRect(reClient, pMainFrmWnd->GetAxColor(66));
}
//control backgroud color
HBRUSH CFilterSetDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		CMainFrmWnd* pMainWnd = (CMainFrmWnd*)m_pParent;

		pDC->SetBkMode(TRANSPARENT);
		hbr = pMainWnd->GetAxBrush(pMainWnd->GetAxColor(66));
	}

	return hbr;
}
