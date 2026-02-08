// CRegKeywordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegKeywordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegKeywordDlg dialog
extern HINSTANCE g_hInstance;
const		LPCTSTR DATA_POINT[] = { "+3", "+2", "+1", "0", "-1", "-2", "-3" };

CRegKeywordDlg::CRegKeywordDlg(CWnd* pParent, CList <KeywordData, KeywordData&>	*plistUserData)
: CDialog(CRegKeywordDlg::IDD, pParent)
{
	m_plistUserData = plistUserData;
	
	//{{AFX_DATA_INIT(CRegKeywordDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CRegKeywordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegKeywordDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRegKeywordDlg, CDialog)
//{{AFX_MSG_MAP(CRegKeywordDlg)
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_CTLCOLOR()
ON_WM_PAINT()
ON_WM_DESTROY()
//}}AFX_MSG_MAP

ON_BN_CLICKED(IDC_BTN_REGOK, OnBtnOk)
ON_BN_CLICKED(IDC_BTN_REGCANCEL, OnBtnCancel)

ON_CBN_SELCHANGE(IDC_COMBO_REGPOINT, OnChangeRegPoint)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegKeywordDlg message handlers

BOOL CRegKeywordDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect reClient;
	GetWindowRect(reClient);
	
	reClient.bottom = reClient.top + 150;
	reClient.right = reClient.left + 200;
	MoveWindow(&reClient);
	
	SetInit();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRegKeywordDlg::SetInit()
{
	m_editS.SetFocus();
	
	for (int nIndex = 0; nIndex < _countof(DATA_POINT); nIndex++)
		m_comboPoint.AddString(DATA_POINT[nIndex]);
	
	m_comboPoint.SetCurSel(3);
	
	m_saSetData.RemoveAll();
}

int CRegKeywordDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	
	m_brushStatic.CreateSolidBrush(COLOR_DLG_CANVAS);

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
		
	m_btnGroup.Create(_T("상황문"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 
		CRect(0, 0, 0, 0), this, IDC_BTN_REGGROUP);
	m_btnGroup.SetFont(&m_fontDefault);
	
	m_ctrlS.Create(_T("주어"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlS.SetFont(&m_fontDefault);
	
	m_editS.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		CRect(0, 0, 0, 0), this, IDC_EDIT_REGS);
	m_editS.SetFont(&m_fontDefault);
	m_editS.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	m_ctrlV.Create(_T("서술어"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlV.SetFont(&m_fontDefault);
	
	m_editV.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		CRect(0, 0, 0, 0), this, IDC_EDIT_REGV);
	m_editV.SetFont(&m_fontDefault);
	m_editV.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	
	m_ctrlPoint.Create(_T("강 도"), WS_CHILD | WS_VISIBLE | SS_LEFT, 
		CRect(0, 0, 0, 0), this);
	m_ctrlPoint.SetFont(&m_fontDefault);
	
	m_comboPoint.Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | WS_TABSTOP,
		CRect(0, 0, 0, 0), this, IDC_COMBO_REGPOINT);
	m_comboPoint.SetFont(&m_fontDefault);
	
	m_btnOk.Create(_T("등록"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 
		CRect(0, 0, 0, 0), this, IDC_BTN_REGOK);
	m_btnOk.SetFont(&m_fontDefault);
	m_btnOk.SetBtnColor((int)STYLE_SET);
	
	m_btnCancel.Create(_T("취소"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP, 
		CRect(0, 0, 0, 0), this, IDC_BTN_REGCANCEL);
	m_btnCancel.SetFont(&m_fontDefault);
	m_btnCancel.SetBtnColor((int)STYLE_SET);

	AfxSetResourceHandle(hInstSave);
	
	return 0;
}

void CRegKeywordDlg::OnDestroy() 
{
	//	CDialog::OnDestroy();
	
	m_fontDefault.DeleteObject();
	m_brushStatic.DeleteObject();
	
	CDialog::OnDestroy();
}

void CRegKeywordDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect reClient, reTop, reLine, reTemp;
	GetClientRect(reClient);
	
	reTop = reClient;
	reTop.left += 5;
	reTop.right -= 5;
	reTop.top += 5;
	reTop.bottom = reTop.top + 90;
	m_btnGroup.MoveWindow(&reTop);
	
	reLine = reTop;
	reLine.left += 10;
	reLine.right -= 10;
	
	reLine.top += 18;
	reLine.bottom = reLine.top + 18;
	
	reTemp = reLine;
	reTemp.right = reTemp.left + 60;
	m_ctrlS.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 5;
	reTemp.right = reLine.right;
	m_editS.MoveWindow(&reTemp);
	
	reLine.top = reLine.bottom + 5;
	reLine.bottom = reLine.top + 18;
	
	reTemp = reLine;
	reTemp.right = reTemp.left + 60;
	m_ctrlV.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 5;
	reTemp.right = reLine.right;
	m_editV.MoveWindow(&reTemp);
	
	
	reLine.top = reLine.bottom + 5;
	reLine.bottom = reLine.top + 18;
	
	reTemp = reLine;
	reTemp.right = reTemp.left + 60;
	m_ctrlPoint.MoveWindow(&reTemp);
	
	reTemp.left = reTemp.right + 5;
	reTemp.right = reTemp.left + 80;
	reTemp.bottom += 100;
	m_comboPoint.MoveWindow(&reTemp);
	
	reTop.top = reTop.bottom + 5;
	reTop.bottom = reTop.top + 20;
	
	reTemp = reTop;
	reTemp.left = reTemp.right - 45;
	m_btnCancel.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 1;
	reTemp.left = reTemp.right - 45;
	m_btnOk.MoveWindow(&reTemp);
}

HBRUSH CRegKeywordDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brushStatic;
	}
	
	return hbr;
}

void CRegKeywordDlg::OnPaint() 
{
	CPaintDC dc(this);
	
	CRect reClient;
	GetClientRect(reClient);
	dc.FillSolidRect(reClient, COLOR_DLG_CANVAS);
}

void CRegKeywordDlg::OnBtnOk()
{
	CString strName_S, strName_V, strPoint;
	
	m_editS.GetWindowText(strName_S);
	m_editV.GetWindowText(strName_V);
	
	if (strName_S.IsEmpty() == TRUE)
		return;
	
	POSITION pos;
	KeywordData		stKeywordData;
	
	BOOL bAddFlag = TRUE;
	for (int nIndex = 0; nIndex < m_plistUserData->GetCount(); nIndex++)
	{
		pos = m_plistUserData->FindIndex(nIndex);
		stKeywordData = m_plistUserData->GetAt(pos);
		
		if ((stKeywordData.nType == 1) &&
			(stKeywordData.strName_S == strName_S) &&
			(stKeywordData.strName_V == strName_V))
		{
			bAddFlag = FALSE;
			break;
		}
	}
	
	if (bAddFlag == TRUE)
	{
		m_saSetData.Add(strName_S);
		m_saSetData.Add(strName_V);
		
		m_comboPoint.GetLBText(m_comboPoint.GetCurSel(), strPoint);
		m_saSetData.Add(strPoint);
		
		OnOK();
	}
	else
	{
		
// 		g_iMetaTable.ShowMessageBox(m_hWnd, "이미 등록된 상황문입니다.", MB_OK);
// 		m_editS.SetFocus();
		
	}
}

void CRegKeywordDlg::OnBtnCancel()
{
	OnCancel();
}

void CRegKeywordDlg::OnChangeRegPoint()
{
	
}

BOOL CRegKeywordDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			CString strS;
			m_editS.GetWindowText(strS);
			if (strS.IsEmpty() == FALSE)
				OnBtnOk();
		}
		else if (pMsg->wParam == VK_ESCAPE)
			OnBtnCancel();
		
		return FALSE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
