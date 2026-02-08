// PortPolioDelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"

#include "PortPolioDelDlg.h"
#include "MainFrmWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DLG_WIDTH       150
#define DLG_HEIGHT      123

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CPortPolioDelDlg dialog

CPortPolioDelDlg::CPortPolioDelDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPortPolioDelDlg::IDD, pParent)
{
	m_pMainWnd = pParent;
	
	//{{AFX_DATA_INIT(CPortPolioDelDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPortPolioDelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortPolioDelDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortPolioDelDlg, CDialog)
//{{AFX_MSG_MAP(CPortPolioDelDlg)
ON_WM_DESTROY()
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_CHAR()
ON_WM_PAINT()
//}}AFX_MSG_MAP

ON_LBN_SELCHANGE(IDC_PORTDEL_DLGLISTBOX, OnPortDelCtrlSelChange)
ON_BN_CLICKED(IDC_BTN_PORTDEL_OK, OnDeleteBtnClicked)
ON_BN_CLICKED(IDC_BTN_PORTDEL_CANCEL, OnCancelBtnClicked)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortPolioDelDlg message handlers

BOOL CPortPolioDelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	POSITION pos = m_peditPortfolio->m_listPort.GetHeadPosition();
	CString  strData;
	for (int nIndex = 0; nIndex < m_peditPortfolio->m_listPort.GetCount(); nIndex++) 
	{
		PortPolio stPort = (PortPolio)(m_peditPortfolio->m_listPort.GetNext(pos));
		strData.Format( "%s-%s", stPort.strPortNumber, stPort.strPortName);
		m_chklistPortDel.AddString(strData);
	}
	
	CRect rePosition;
	rePosition.left = m_rePortDel.right - DLG_WIDTH;
	rePosition.right = m_rePortDel.right;
	rePosition.bottom = m_rePortDel.top;
	rePosition.top = m_rePortDel.top - DLG_HEIGHT;
	
	MoveWindow(rePosition);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPortPolioDelDlg::OnDestroy() 
{
	//	CDialog::OnDestroy();
	
	m_fontDefault.DeleteObject();
	CDialog::OnDestroy();
}

int CPortPolioDelDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);

	CConditionMakeWnd* pMakeWnd = (CConditionMakeWnd*)m_pMainWnd;
		
	m_chklistPortDel.Create(WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_HASSTRINGS,
		CRect(0, 0, 0, 0), this, IDC_PORTDEL_DLGLISTBOX);
	m_chklistPortDel.SetFont(&m_fontDefault);
	
	COLORREF crBg = COLOR_BUTTON;
	COLORREF crText = COLOR_BLACK;
	
	m_btnDelete.Create(_T("삭제"), CRect(0, 0, 0, 0), this, IDC_BTN_PORTDEL_OK);
	pMakeWnd->SetBtnImg(&m_btnDelete, 2);
	
	m_btnCancel.Create(_T("취소"), CRect(0, 0, 0, 0), this, IDC_BTN_PORTDEL_CANCEL);
	pMakeWnd->SetBtnImg(&m_btnCancel, 2);

	AfxSetResourceHandle(hInstSave);
	
	return 0;
}

void CPortPolioDelDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect reClient, reBottom, reTemp;
	GetClientRect(reClient);
	
	reClient.top += 3;
	reClient.bottom -= 3;
	reClient.left += 3;
	reClient.right -= 3;
	
	reBottom = reClient;
	reBottom.top = reClient.bottom - 20;
	
	reTemp = reBottom;
	reTemp.left = reTemp.right - 36;
	m_btnCancel.MoveWindow(&reTemp);
	
	reTemp.right = reTemp.left - 2;
	reTemp.left = reTemp.right - 36;
	m_btnDelete.MoveWindow(&reTemp);
	
	reBottom.bottom = reBottom.top - 2;
	reBottom.top = reClient.top;
	m_chklistPortDel.MoveWindow(&reBottom);
}

void CPortPolioDelDlg::OnDeleteBtnClicked()
{
	for (int nIndex = 0; nIndex < m_chklistPortDel.GetCount(); nIndex++)
	{
		if (m_chklistPortDel.GetCheck(nIndex) == TRUE)
		{
			CString strPortNumber;
			m_chklistPortDel.GetText(nIndex, strPortNumber);
			int nFind = strPortNumber.Find('-');
			
			strPortNumber = strPortNumber.Left(nFind);
			m_peditPortfolio->DeletePortData(strPortNumber);
			if (m_peditPortfolio->GetPortDataCount() == 0)
				m_peditPortfolio->DeleteAll();
		}
	}
	
	OnOK();
}

void CPortPolioDelDlg::OnCancelBtnClicked()
{
	OnOK();
}

void CPortPolioDelDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE)
		OnOK();
	
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CPortPolioDelDlg::OnPortDelCtrlSelChange()
{
	int nSelectIndex = m_chklistPortDel.GetCurSel();
	
	POSITION posIndex;
	BOOL bHave = FALSE;
	int nIndex, nCount;
	nCount = m_listCheckInfo.GetCount();
	if (nCount > 0)
	{
		for (nIndex = 0; nIndex < nCount; nIndex++)
		{
			posIndex = m_listCheckInfo.FindIndex(nIndex);
			int nFindIndex = m_listCheckInfo.GetAt(posIndex);
			
			if (nSelectIndex == nFindIndex)
			{
				m_listCheckInfo.RemoveAt(posIndex);
				bHave = TRUE;
				break;
			}
		}
	}
	
	if (bHave == FALSE)
		m_listCheckInfo.AddTail(nSelectIndex);
	
	for (nIndex = 0; nIndex < m_chklistPortDel.GetCount(); nIndex++)
		m_chklistPortDel.SetCheck(nSelectIndex, FALSE);
	
	for (nIndex = 0; nIndex < m_listCheckInfo.GetCount(); nIndex++)
	{
		posIndex = m_listCheckInfo.FindIndex(nIndex);
		nSelectIndex = m_listCheckInfo.GetAt(posIndex);
		m_chklistPortDel.SetCheck(nSelectIndex, TRUE);
	}
	
	m_chklistPortDel.Invalidate();
}

void CPortPolioDelDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rtMainWnd;
	GetClientRect(rtMainWnd);
	
	dc.FillSolidRect(rtMainWnd, COLOR_DLG_CANVAS);	
}
