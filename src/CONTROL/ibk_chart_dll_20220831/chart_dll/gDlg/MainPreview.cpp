#include "stdafx.h"
#include "axisGDlg.h"
#include "MainPreview.h"
#include "MainType1.h"
#include "PreviewWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMainPreview::CMainPreview(COLORREF tkCOLOR, char* envinfo, CWnd* pParent /*=NULL*/)
	: CDialog(CMainPreview::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainPreview)
	//}}AFX_DATA_INIT

	m_pwndChild = NULL;
	m_pPreviewWnd = NULL;
	m_pcEnv = envinfo;
	m_rgbTick = tkCOLOR;
}


void CMainPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainPreview)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainPreview, CDialog)
	//{{AFX_MSG_MAP(CMainPreview)
	ON_WM_DESTROY()	
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()


BOOL CMainPreview::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
	CRect	rect;
	CWnd*	pWnd = GetDlgItem(IDC_ST_CONFIG);
	
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);

	if (m_pwndChild == NULL)
	{
		m_pwndChild = new CMainType1();
		((CMainType1*)m_pwndChild)->Create(CMainType1::IDD, this);
		m_pwndChild->ShowWindow(SW_HIDE);
		m_pwndChild->MoveWindow(rect);
		m_pwndChild->ShowWindow(SW_SHOW);
	}
	
	pWnd = GetDlgItem(IDC_ST_PREVIEW);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);	

	if (m_pPreviewWnd == NULL)
	{
		m_pPreviewWnd = new CPreviewWnd;
		m_pPreviewWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect, this, 0);		
	}
	return TRUE;
}

void CMainPreview::OnDestroy() 
{
	CDialog::OnDestroy();

	if (LIB_IsWndAlive(m_pwndChild))
		LIB_DeleteWnd(m_pwndChild);

	if (LIB_IsWndAlive(m_pPreviewWnd))
		LIB_DeleteWnd(m_pPreviewWnd);
}

void CMainPreview::SetScreen(struct _graph* pBong, struct _graph* pGraph, bool bEach, bool bTitle)
{
	CWnd*	pWnd = GetDlgItem(IDC_CHARTNAME);
	int	nKind = pGraph->wGKind;
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
	CString	title;

	title = pApp->GetTitle(nKind);
	
	if (title.Find("&") >= 0)
		title.Replace("&", "&&");

	if (bTitle)
	{
		if (bEach)
		{
			pWnd->SetWindowText("");
			CString	tmpstr;
			GetParent()->GetWindowText(tmpstr);
			tmpstr += title;
			GetParent()->SetWindowText(tmpstr);
		}
		else
			pWnd->SetWindowText(title);
	}
	
	m_pGraph = pGraph;
	m_pBong = pBong;

	pWnd = GetFocus();

//	OnManage(MAKEWPARAM(MNG_REDRAW, 0), 0);		
	((CMainType1*)m_pwndChild)->SetScreen((struct _envInfo*)m_pcEnv, pGraph);

	if (LIB_IsWndAlive(pWnd))
		pWnd->SetFocus();

	
}

void CMainPreview::SetChartName(CString chartname)
{
	CWnd* pWnd = GetDlgItem(IDC_CHARTNAME);
	pWnd->SetWindowText(chartname);
}

LRESULT CMainPreview::OnManage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case MNG_REDRAW:		
		m_pPreviewWnd->ShowGraph(m_pBong, m_rgbTick, (struct _envInfo*)m_pcEnv, m_pGraph, (int)lParam);
		break;
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
			SetScreen(m_pBong, m_pGraph, (lParam) ? true : false);			
		}
		break;

	case MNG_UNION_PREVIEW:
		m_pPreviewWnd->SetUnion((WORD)lParam);
	}
	
	return 0;
}

WORD CMainPreview::GetUnion()
{

	if (!m_pwndChild)
		return NULL;

	return ((CMainType1*)m_pwndChild)->GetUnion();
}