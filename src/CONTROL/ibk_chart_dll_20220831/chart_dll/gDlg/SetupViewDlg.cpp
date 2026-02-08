#include "stdafx.h"
#include "axisgdlg.h"
#include "SetupViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSetupViewDlg::CSetupViewDlg(CWnd* pMain, char* info, CWnd* pParent /*=NULL*/)
	: CDialog(CSetupViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupViewDlg)
	//}}AFX_DATA_INIT

	m_pMain = pMain;
	m_pEnv = (struct _envInfo*)info;
}


void CSetupViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupViewDlg)
	DDX_Control(pDX, IDC_ST_MARGIN_RIGHT, m_spinRight);
	DDX_Control(pDX, IDC_ST_MARGIN_LEFT, m_spinLeft);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupViewDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupViewDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CTRLMSG, OnControls)
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupViewDlg message handlers

void CSetupViewDlg::SetScreen()
{
	UINT	ctrlID[] = {
				IDC_CHK_DS_CURRENT,
				IDC_CHK_DS_MOD,
				IDC_CHK_DS_LOG,
				IDC_CHK_DS_REVERSE,
				IDC_CHK_DS_MINMAX,
				IDC_CHK_DS_LOCK,
				IDC_CHK_DS_PCURRENT,
				IDC_CHK_DS_ECURRENT,
				IDC_CHK_DS_STARTMAX,
				IDC_CHK_DS_VIEWGRID
				};
	CWnd*	pWnd = NULL;
	int	flags[] = {
				DO_CURRENT,
				DO_MOD,
				DO_LOG,
				DO_REVERSE,
				DO_MINMAX,
				DO_LOCK,
				DO_PCURRENT,
				DO_ECURRENT,
				DO_STARTMAX,
				DO_VIEWGRID
				};


	int	ii = 0, nCount = sizeof(ctrlID) / sizeof(UINT);

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		pWnd = GetDlgItem(ctrlID[ii]);

		if (m_pEnv->display.dwDspOption & flags[ii])
			((CButton*)pWnd)->SetCheck(1);
		else
			((CButton*)pWnd)->SetCheck(0);
	}

	pWnd = GetDlgItem(IDC_CHK_DS_VIEWTIP);
	if (m_pEnv->display.dwDspOption & DO_NOVIEWTIP)
		((CButton *)pWnd)->SetCheck(0);
	else
		((CButton *)pWnd)->SetCheck(1);

	pWnd = GetDlgItem(IDC_CHK_DS_VIEWSISE);
	struct _pninfo	*pninfo = NULL;
	WORD info = 0;
	for (ii = 0; ii < PANEL_CNT; ii++)
	{
		if (m_pEnv->panel[ii].btPnID == 0)
			continue;

		info = m_pEnv->panel[ii].wPnOption;
		if (info & PO_USE && m_pEnv->panel[ii].btPnID == PN_SISE)
		{
			if (info & PO_SHOW)
				((CButton *)pWnd)->SetCheck(1);
			else
				((CButton *)pWnd)->SetCheck(0);

			break;
		}
	}


	m_spinLeft.SetMinMax(0, 50);
	m_spinLeft.SetNumber(m_pEnv->display.wLMargin);
	m_spinRight.SetMinMax(0, 50);
	m_spinRight.SetNumber(m_pEnv->display.wRMargin);

	UINT	raID = 0;

	((CButton*)GetDlgItem(IDC_RA_BOTH))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RA_LEFT))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RA_RIGHT))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RA_NONE))->SetCheck(0);

	if (m_pEnv->display.dwDspOption & DO_DISPLT)
	{
		if (m_pEnv->display.dwDspOption & DO_DISPRT)
			raID = IDC_RA_BOTH;
		else
			raID = IDC_RA_LEFT;		
	}
	else
	{
		if (m_pEnv->display.dwDspOption & DO_DISPRT)
			raID = IDC_RA_RIGHT;
		else
			raID = IDC_RA_NONE;		
	}

	((CButton*)GetDlgItem(raID))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_RA_SCREEN))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RA_TOTAL))->SetCheck(0);

	if (m_pEnv->display.dwDspOption & DO_SCREEN)
		raID = IDC_RA_SCREEN;
	else
		raID = IDC_RA_TOTAL;

	((CButton*)GetDlgItem(raID))->SetCheck(1);

	// 버튼 다운시 수치조회
	if (m_pEnv->display.dwDspOption & DO_LBTNDNSHOW)
		((CButton*)GetDlgItem(IDC_CHK_DS_VIEWGRID_BUTTON_DOWN))->SetCheck(1);

	if (!((CButton*)GetDlgItem(IDC_CHK_DS_VIEWGRID))->GetCheck())
		GetDlgItem(IDC_CHK_DS_VIEWGRID_BUTTON_DOWN)->EnableWindow(false);
	else
		GetDlgItem(IDC_CHK_DS_VIEWGRID_BUTTON_DOWN)->EnableWindow(true);

}

LRESULT CSetupViewDlg::OnControls(WPARAM wParam, LPARAM lParam)
{
	
	switch (LOWORD(wParam))
	{
	case IDC_ST_MARGIN_LEFT:
		if (HIWORD(wParam) == CTRL_EDITCHANGE)
			m_pEnv->display.wLMargin = m_spinLeft.GetNumber();
		break;
	case IDC_ST_MARGIN_RIGHT:
		if (HIWORD(wParam) == CTRL_EDITCHANGE)
			m_pEnv->display.wRMargin = m_spinRight.GetNumber();
		break;
	}

	return 0;
}

BOOL CSetupViewDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD(wParam) == BN_CLICKED)
	{
		CWnd* pWnd = GetDlgItem(LOWORD(wParam));
		if (pWnd == NULL)
			return TRUE;

		int iFlag = 0;

		switch (LOWORD(wParam))
		{
		case IDC_CHK_DS_CURRENT:
			iFlag = DO_CURRENT;
			break;
		case IDC_CHK_DS_MOD:
			iFlag = DO_MOD;
			break;
		case IDC_CHK_DS_LOG:
			iFlag = DO_LOG;
			break;
		case IDC_CHK_DS_REVERSE:
			iFlag = DO_REVERSE;
			break;
		case IDC_CHK_DS_MINMAX:
			iFlag = DO_MINMAX;
			break;
		case IDC_CHK_DS_LOCK:
			iFlag = DO_LOCK;
			break;	
		case IDC_CHK_DS_PCURRENT:
			iFlag = DO_PCURRENT;
			break;
		case IDC_CHK_DS_ECURRENT:
			iFlag = DO_ECURRENT;
			break;
		case IDC_CHK_DS_STARTMAX:
			iFlag = DO_STARTMAX;
			break;
		case IDC_CHK_DS_VIEWGRID:
			iFlag = DO_VIEWGRID;
			break;
		case IDC_CHK_DS_VIEWTIP:
			{
				if (((CButton*)pWnd)->GetCheck())
					m_pEnv->display.dwDspOption &= ~DO_NOVIEWTIP;
				else
					m_pEnv->display.dwDspOption |= DO_NOVIEWTIP;
			}
			break;
		case IDC_CHK_DS_VIEWSISE:
			{
				struct _pninfo	*pninfo = NULL;
				WORD info = 0;
				for (int ii = 0; ii < PANEL_CNT; ii++)
				{
					if (m_pEnv->panel[ii].btPnID == 0)
						continue;

					info = m_pEnv->panel[ii].wPnOption;
					if (info & PO_USE && m_pEnv->panel[ii].btPnID == PN_SISE)
					{
						if (((CButton*)pWnd)->GetCheck())
							m_pEnv->panel[ii].wPnOption |= PO_SHOW;
						else
							m_pEnv->panel[ii].wPnOption &= ~PO_SHOW;

						break;
					}
				}
			}
			break;
		case IDC_RA_LEFT:
			m_pEnv->display.dwDspOption &= ~DO_DISPRT;
			m_pEnv->display.dwDspOption |= DO_DISPLT;
			break;
		case IDC_RA_RIGHT:
			m_pEnv->display.dwDspOption &= ~DO_DISPLT;
			m_pEnv->display.dwDspOption |= DO_DISPRT;
			break;
		case IDC_RA_BOTH:
			m_pEnv->display.dwDspOption |= (DO_DISPLT | DO_DISPRT);
			break;
		case IDC_RA_NONE:
			m_pEnv->display.dwDspOption &= ~(DO_DISPLT | DO_DISPRT);
			break;
		case IDC_RA_SCREEN:
			m_pEnv->display.dwDspOption |= DO_SCREEN;
			break;
		case IDC_RA_TOTAL:
			m_pEnv->display.dwDspOption &= ~DO_SCREEN;
			break;
		// 버튼 다운시 수치조회
		case IDC_CHK_DS_VIEWGRID_BUTTON_DOWN:
			iFlag = DO_LBTNDNSHOW;
			break;
		}

		if (iFlag)
		{
			if (((CButton*)pWnd)->GetCheck())
			{
				m_pEnv->display.dwDspOption |= iFlag;
				if (iFlag == DO_VIEWGRID)
					GetDlgItem(IDC_CHK_DS_VIEWGRID_BUTTON_DOWN)->EnableWindow(true);
			}
			else
			{
				m_pEnv->display.dwDspOption &= ~iFlag;
				if (iFlag == DO_VIEWGRID)
					GetDlgItem(IDC_CHK_DS_VIEWGRID_BUTTON_DOWN)->EnableWindow(false);
			}
		}	
	}
	return CDialog::OnCommand(wParam, lParam);
}

BOOL CSetupViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_spinLeft.ModifyStyle(ES_RIGHT, ES_CENTER);
	m_spinRight.ModifyStyle(ES_RIGHT, ES_CENTER);

	if (m_pEnv->wEnvOption & EO_MAP)
	{
		CWnd *pWnd = GetDlgItem(IDC_CHK_DS_STARTMAX);
		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_CHK_DS_VIEWGRID);
		pWnd->ShowWindow(SW_HIDE);
//		pWnd = GetDlgItem(IDC_CHK_DS_VIEWTIP);
//		pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_CHK_DS_VIEWSISE);
		pWnd->ShowWindow(SW_HIDE);
	}

	return TRUE; 
}

LRESULT CSetupViewDlg::OnManage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case MNG_DEFAULT:
		{
			CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
			struct _envInfo*	pEnv = pApp->GetEnvDefault();
			
			m_pEnv->display.wLMargin = pEnv->display.wLMargin;
			m_pEnv->display.wRMargin = pEnv->display.wRMargin;
			m_pEnv->display.dwDspOption = pEnv->display.dwDspOption;
			if (m_pEnv->wEnvOption & EO_MAP)
				m_pEnv->display.dwDspOption = DO_SCREEN | DO_DISPRT | DO_TKVGRID | DO_TKHGRID | DO_MOD | DO_LOCK | DO_CURRENT | DO_USEGRID;

			((CButton*)GetDlgItem(IDC_CHK_DS_VIEWGRID_BUTTON_DOWN))->SetCheck(false);
			SetScreen();
		}
		break;
	}

	return 0;
}
