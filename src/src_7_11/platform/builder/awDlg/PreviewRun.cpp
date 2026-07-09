// PreviewRun.cpp : implementation file
//

#include "stdafx.h"
#include "PreviewRun.h"
#include "../../h/axisfire.h"
#include "../mainvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define GAPX	GetSystemMetrics(SM_CXFRAME) * 2 + GetSystemMetrics(SM_CXBORDER) * 2
#define GAPY	GetSystemMetrics(SM_CYFRAME) * 2 + GetSystemMetrics(SM_CYBORDER) * 2 + GetSystemMetrics(SM_CYCAPTION)
/////////////////////////////////////////////////////////////////////////////
// CPreviewRun dialog


CPreviewRun::CPreviewRun(CWnd* pParent /*=NULL*/)
	: CDialog(CPreviewRun::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreviewRun)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pView = NULL;
}


void CPreviewRun::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreviewRun)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreviewRun, CDialog)
	//{{AFX_MSG_MAP(CPreviewRun)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CPreviewRun message handlers

void CPreviewRun::OnClose() 
{
	m_pWizardCtrl->InvokeHelper(DI_DETACH, DISPATCH_METHOD, VT_EMPTY, (void *)NULL,
					(BYTE *)(VTS_I4), m_key);
	if (m_pView)
		delete m_pView;
	CDialog::OnClose();
}

void CPreviewRun::OnStart()
{
	if (!m_pWizardCtrl)
		return;
	long size;
				
	m_wndType = vtypeERR;
	m_pWizardCtrl->InvokeHelper(DI_FORMI, DISPATCH_METHOD, VT_I4, (void*)&m_wndType, (BYTE*)(VTS_BSTR VTS_I4), m_mapName.operator LPCTSTR(), &size);
	SetWindowPos(NULL, 0, 0, LOWORD(size) + GAPX, HIWORD(size) + GAPY, SWP_NOZORDER|SWP_NOMOVE);
	if (m_wndType == vtypeERR)
		AfxMessageBox("wndType Error!");
	
	m_pWizardCtrl->InvokeHelper(DI_ATTACH, DISPATCH_METHOD, VT_I4, (void*)&m_key, (BYTE*)(VTS_I4 VTS_I4 VTS_I4), (long)m_pView, m_wndType, 0);
	
	if (m_key)
	{
		BOOL bretval;
		m_pWizardCtrl->InvokeHelper(DI_FORMS, DISPATCH_METHOD, VT_BOOL, (void*)&bretval, (BYTE*)(VTS_I4 VTS_BSTR VTS_I4 VTS_BOOL), m_key, m_mapName.operator LPCTSTR(), size, false);
	}
}

BOOL CPreviewRun::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//m_pView = new CWnd();
	//m_pView->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 0);

	SetWindowText(m_sCaption);
	OnStart();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPreviewRun::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (m_pView)
		m_pView->SetWindowPos(0, 0, 0, cx, cy, SWP_NOMOVE|SWP_NOZORDER);	
}
