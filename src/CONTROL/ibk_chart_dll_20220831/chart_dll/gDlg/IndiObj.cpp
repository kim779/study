#include "stdafx.h"
#include "axisgdlg.h"
#include "IndiObj.h"
#include "IndiBase.h"
#include "IndiHorizon.h"
#include "IndiDisp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIndiObj::CIndiObj(CWnd* pParent /*=NULL*/)
	: CDialog(CIndiObj::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndiObj)
	//}}AFX_DATA_INIT
	m_pBaseDlg = NULL;
	m_pHoriDlg = NULL;
	m_pDispDlg = NULL;

	m_bEach = false;
	m_bDestroy = false;

}


void CIndiObj::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndiObj)
	DDX_Control(pDX, IDC_ST_INDY, m_stIndi);
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndiObj, CDialog)
	//{{AFX_MSG_MAP(CIndiObj)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RA_BASIC, OnClickedRaBasic)
	ON_BN_CLICKED(IDC_RA_HORIZONLINE, OnClickedRaHorizonline)
	ON_BN_CLICKED(IDC_RA_LINE, OnClickedRaLine)
	ON_LBN_SETFOCUS(IDC_LIST, OnSetFocusList)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CTRLMSG, OnControls)
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()

void CIndiObj::OnClickedRaBasic() 
{
	DelAllDlg(m_pBaseDlg);	
	m_list.SetCurSel(-1);

	if (m_pBaseDlg == NULL)
	{
		m_pBaseDlg = new CIndiBase;
		m_pBaseDlg->Create(CIndiBase::IDD, this);
		m_pBaseDlg->ShowWindow(SW_HIDE);
		m_pBaseDlg->MoveWindow(m_rcFrame);
		m_pBaseDlg->ShowWindow(SW_SHOW);
	}

	m_pBaseDlg->SetScreen(m_pIndi, m_pGraph);
	GetDlgItem(IDC_RA_BASIC)->SetFocus();
}

void CIndiObj::OnClickedRaHorizonline() 
{
	DelAllDlg(m_pHoriDlg);	
	m_list.SetCurSel(-1);

	if (m_pHoriDlg == NULL)
	{
		m_pHoriDlg = new CIndiHorizon;
		m_pHoriDlg->Create(CIndiHorizon::IDD, this);
		m_pHoriDlg->ShowWindow(SW_HIDE);
		m_pHoriDlg->MoveWindow(m_rcFrame);
		m_pHoriDlg->ShowWindow(SW_SHOW);
	}

	m_pHoriDlg->SetScreen(m_pGraph);
	GetDlgItem(IDC_RA_HORIZONLINE)->SetFocus();
}

void CIndiObj::OnClickedRaLine() 
{
	if (m_bDestroy)
		return;

	DelAllDlg(m_pDispDlg);

	m_list.SetCurSel(0);
	OnControls(MAKEWPARAM(m_list.GetDlgCtrlID(), CTRL_LISTSELECT), 0);
}

void CIndiObj::DelAllDlg(CWnd* pWnd)
{
	if (pWnd != (CWnd*)m_pBaseDlg)
	{
		if (LIB_IsWndAlive(m_pBaseDlg))
			LIB_DeleteWnd(m_pBaseDlg);
	}

	if (pWnd != (CWnd*)m_pHoriDlg)
	{
		if (LIB_IsWndAlive(m_pHoriDlg))
			LIB_DeleteWnd(m_pHoriDlg);
	}

	if (pWnd != (CWnd*)m_pDispDlg)
	{
		if (LIB_IsWndAlive(m_pDispDlg))
			LIB_DeleteWnd(m_pDispDlg);
	}
}

BOOL CIndiObj::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CWnd*	pWnd = GetDlgItem(IDC_ST_DLG);

	pWnd->GetWindowRect(m_rcFrame);
	ScreenToClient(m_rcFrame);	
	m_list.ResetContent();

	return TRUE;
}

void CIndiObj::OnDestroy() 
{
	m_bDestroy = true;
	CDialog::OnDestroy();
	DelAllDlg();
}

UINT CIndiObj::CheckRadio(int nRadio)
{
	UINT	ctrlID[] = {IDC_RA_BASIC, IDC_RA_HORIZONLINE, IDC_RA_LINE};
	int	nCount = sizeof(ctrlID) / sizeof(UINT);
	UINT	id = 0;
	int	ii = 0;
	CWnd*	pWnd = NULL;	

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		pWnd = GetDlgItem(ctrlID[ii]);
		((CButton*)pWnd)->SetCheck(0);
	}

	if (nRadio > nCount)
		return 0;


	if (nRadio > 0)
	{
		id = ctrlID[nRadio - 1];
		pWnd = GetDlgItem(id);
		((CButton*)pWnd)->SetCheck(1);
		
		switch(nRadio - 1)
		{
		case 0:
			OnClickedRaBasic();
			break;
		case 1:
			OnClickedRaHorizonline();
			break;
		case 2:
			OnClickedRaLine();
			break;
		}
	}

	return id;
}

void CIndiObj::CreateColor(char kind, struct _line* line)
{

	if (m_pDispDlg == NULL)
	{
		m_pDispDlg = new CIndiDisp;
		m_pDispDlg->Create(CIndiDisp::IDD, this);
		m_pDispDlg->ShowWindow(SW_HIDE);	
		m_pDispDlg->MoveWindow(m_rcFrame);
		m_pDispDlg->ShowWindow(SW_SHOW);		
	}

	int	nCurSel = m_list.GetCurSel();

	bool	bCheck = (m_pGraph->wGKind == GK_VOL || m_pGraph->wGKind == GK_AMT) ? true : false;
	
	if (bCheck)
		m_pDispDlg->SetScreen(line, "채움", &m_pGraph->dwDraOption, GO_UNFILLVOLUME);
	else
		m_pDispDlg->SetScreen(kind, line);
	m_list.SetFocus();
}

void CIndiObj::SetScreen(struct _graph* pGraph, bool bEach, bool bTitle)
{
	bool bBase = false;
	CWnd* pWnd = NULL;	
	UINT ctrlID[3] = {IDC_RA_BASIC, IDC_RA_HORIZONLINE, IDC_RA_LINE};
	CString	tmpstr, tmpstr1;
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();	

	m_pGraph = pGraph;
	m_pIndi = pApp->GetIndiObj(m_pGraph->wGKind);
	m_pChart = pApp->GetChartObj(m_pGraph->wGKind);
	
	tmpstr.Format("%s", m_pChart->m_sztitle);
	tmpstr1 = LIB_MakeParen(m_pGraph, m_pIndi);
	tmpstr += tmpstr1;

	m_bEach = bEach;

	if (bTitle)
	{
		if (!m_bEach)
			m_stIndi.SetWindowText(tmpstr);
		else
		{
			m_stIndi.SetWindowText("");
			CString	title;
			GetParent()->GetWindowText(title);
			int	nPos = title.Find("-", 0);

			if (nPos < 0)
				title = _T("지표설정 - ");
			else
			{
				title = title.Left(nPos + 2);
			}

			title += tmpstr;
			GetParent()->SetWindowText(title);
		}
	}

	this->ShowWindow(SW_SHOW);

	if (m_pIndi->bcnt)
	{
		CheckRadio(1);	
		bBase = true;		
		pWnd = GetDlgItem(ctrlID[0]);	
		pWnd->EnableWindow(TRUE);
	}
	else
	{
		pWnd = GetDlgItem(ctrlID[0]);	
		pWnd->EnableWindow(FALSE);
	}

	pWnd = GetDlgItem(ctrlID[1]);

	if (m_pIndi->hori)
	{
		pWnd->EnableWindow(TRUE);
	}
	else
	{
		pWnd->EnableWindow(FALSE);
	}

	m_list.ResetContent();

	for ( int ii = 0 ; ii < m_pIndi->lcnt ; ii++ )
	{
		m_list.AddColor(m_pIndi->line[ii].title, m_pGraph->cInfo.aline[ii].rgbLColor);
	}

	pWnd = GetDlgItem(ctrlID[2]);

	if (!bBase)
	{
		CheckRadio(3);		
	}
}

LRESULT CIndiObj::OnControls(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_LIST:
		if (HIWORD(wParam) == CTRL_LISTSELECT)
		{
			int	nCurSel = m_list.GetCurSel();
		
			if (nCurSel < 0)
			{
				if (!m_list.GetCount())
					return 0;
				nCurSel = 0;
				m_list.SetCurSel(nCurSel);
			}
			
			CreateColor(m_pIndi->line[nCurSel].kind, &m_pGraph->cInfo.aline[nCurSel]);			
		}
		break;
	default:
		{
			switch (HIWORD(wParam))
			{
			case CTRL_COLORCHANGE:
				m_list.SetColor(lParam);		
				break;
			case CTRL_VALUECHANGE:
				{
					CString	tmpstr, tmpstr1;
					
					tmpstr.Format("%s", m_pChart->m_sztitle);
					tmpstr1 = LIB_MakeParen(m_pGraph, m_pIndi);
					tmpstr += tmpstr1;

					GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(GetDlgCtrlID(), CTRL_VALUECHANGE));
				
					if (!m_bEach)
						m_stIndi.SetWindowText(tmpstr);
					else
					{						
						CString	title;
						int	nPos = 0;
						GetParent()->GetWindowText(title);
						nPos = title.Find("-");
						title = title.Left(nPos + 2);
						title += tmpstr;
						GetParent()->SetWindowText(title);
					}
				}
				break;
			}
		}
	}

	return 0;
}

void CIndiObj::OnSetFocusList() 
{
	CheckRadio(4);
	DelAllDlg(m_pDispDlg);
	((CButton*)GetDlgItem(IDC_RA_LINE))->SetCheck(1);
}

LRESULT CIndiObj::OnManage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case MNG_DEFAULT:
		{
			CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
			struct	_graph*	pDefault = NULL;

			pDefault = pApp->GetDefault(m_pGraph->wGKind);
			
			if (pDefault == NULL)
				return 0;

			m_pGraph->wCalOption = pDefault->wCalOption;
			m_pGraph->dwDraOption = pDefault->dwDraOption;
			memcpy(&m_pGraph->cInfo, &pDefault->cInfo, SZ_CHARTINFO);
			memcpy(&m_pGraph->tInfo, &pDefault->tInfo, SZ_TICKINFO);
			SetScreen(m_pGraph, (lParam) ? true : false);
			
		}
		break;
	}

	return 0;
}

BOOL CIndiObj::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT)
			return TRUE;	
	}
	return CDialog::PreTranslateMessage(pMsg);
}
