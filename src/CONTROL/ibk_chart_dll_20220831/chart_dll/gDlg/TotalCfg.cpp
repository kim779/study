#include "stdafx.h"
#include "axisgdlg.h"
#include "TotalCfg.h"
#include "SetupViewDlg.h"
#include "SetupEnvDlg.h"
#include "ScreenCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTotalCfg::CTotalCfg(CWnd* pMain, COLORREF tkCOLOR, char* envinfo, char* ginfo, char* scrinfo, CWnd* pParent)
	: CDialog(CTotalCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTotalCfg)
	//}}AFX_DATA_INIT

	m_pMain = pMain;
	m_pInput[0] = scrinfo;
	m_pInput[1] = envinfo;
	m_pInput[2] = ginfo;
	m_pOutput = NULL;

	memset(&m_tmpEnv, 0x00, SZ_ENVINFO);
	memcpy(&m_tmpEnv, m_pInput[1], SZ_ENVINFO);

	struct _cntInfo*	pGraph = (struct _cntInfo*)ginfo;
	m_nSize = SZ_CNTINFO + (pGraph->wMainCnt + pGraph->wAidCnt) * SZ_GRAPH;
	
	m_pgraphinfo = new char[m_nSize];
	memset(m_pgraphinfo, 0x00, m_nSize);
	memcpy(m_pgraphinfo, pGraph, m_nSize);

	m_nCurSel = -1;
	m_pChild = NULL;
	m_tkCOLOR = tkCOLOR;
}


void CTotalCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTotalCfg)
	DDX_Control(pDX, IDC_TAB, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTotalCfg, CDialog)
	//{{AFX_MSG_MAP(CTotalCfg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_BT_DEFAULT, OnBtDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTotalCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_tab.InsertItem(0, "지표설정");
	m_tab.InsertItem(1, "정보표시");
	m_tab.InsertItem(2, "색상");

	CWnd*	pWnd = GetDlgItem(IDC_ST_FRAME);
	pWnd->GetWindowRect(m_rcFrame);
	ScreenToClient(m_rcFrame);
	setSel(0);	
	return TRUE;
}

void CTotalCfg::setSel(int nIndex)
{
	if (nIndex == m_nCurSel)
		return;
	else
		m_nCurSel = nIndex;	

	if (LIB_IsWndAlive(m_pChild))
	{
		LIB_DeleteWnd(m_pChild);
	}
		
	switch (nIndex)
	{
	case 0:
		//pMain, tkCOLOR, envinfo, ginfo, scrinfo
		if (m_pOutput)
			m_pInput[0] = m_pOutput;

		m_pChild = new CScreenCfg(m_pMain, m_tkCOLOR, (char*)&m_tmpEnv, m_pgraphinfo, m_pInput[0], &m_pOutput);
		((CScreenCfg*)m_pChild)->Create(CScreenCfg::IDD, this);	
		break;
	case 1:
		m_pChild = new CSetupViewDlg(m_pMain, (char*)&m_tmpEnv);
		((CSetupViewDlg*)m_pChild)->Create(CSetupViewDlg::IDD, this);	
		((CSetupViewDlg*)m_pChild)->SetScreen();
		break;
	case 2:
		m_pChild = new CSetupEnvDlg(m_pMain, (char*)&m_tmpEnv);
		((CSetupEnvDlg*)m_pChild)->Create(CSetupEnvDlg::IDD, this);	
		((CSetupEnvDlg*)m_pChild)->SetScreen();
		break;	
	}

	if (m_pChild)
	{
		m_pChild->ShowWindow(SW_HIDE);
		m_pChild->MoveWindow(m_rcFrame);
		m_pChild->ShowWindow(SW_SHOW);			
		m_tab.SetCurSel(nIndex);		
	}

	m_tab.SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void CTotalCfg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int	nCurSel = m_tab.GetCurSel();
	
	if (nCurSel < 0)
		return;

	setSel(nCurSel);
	*pResult = 0;
}

void CTotalCfg::OnDestroy() 
{
	CDialog::OnDestroy();

	if (LIB_IsWndAlive(m_pChild))
		LIB_DeleteWnd(m_pChild);

	delete [] m_pgraphinfo;
	m_pgraphinfo = NULL;
}

void CTotalCfg::OnOk() 
{
	if (LIB_IsWndAlive(m_pChild))
		LIB_DeleteWnd(m_pChild);
	
	memcpy(m_pInput[1], &m_tmpEnv, SZ_ENVINFO);
	memcpy(m_pInput[2], m_pgraphinfo, m_nSize);
	CDialog::OnOK();
}

void CTotalCfg::OnBtDefault() 
{
	int	nMode = AfxGetMainWnd()->MessageBox("설정값을 모두 초기화 합니다\n계속하시겠습니까?", "알림", MB_YESNO);

	if (nMode == IDYES)
	{
		if (LIB_IsWndAlive(m_pChild))
			m_pChild->SendMessage(WM_MANAGE, MNG_DEFAULT);	
	}
}
