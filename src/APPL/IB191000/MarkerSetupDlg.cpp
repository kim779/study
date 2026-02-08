// MarkerSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB191000.h"
#include "MarkerSetupDlg.h"

#include "../../h/axisfire.h"
#include "../../h/axisvar.h"

#ifndef _INC_SHLWAPI
	#include <shlwapi.h>
	#pragma comment(lib, "Shlwapi.lib")
	//#pragma message("Automatically linking with Shlwapi.lib")
#endif // _INC_SHLWAPI

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarkerSetupDlg dialog


CMarkerSetupDlg::CMarkerSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMarkerSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarkerSetupDlg)
	//}}AFX_DATA_INIT

	m_hLib = NULL;
	m_pWnd = NULL;

	m_clrMarkerBKG	= RGB(240, 240, 240);
	m_clrMarkerTXT	= RGB(142, 142, 142);
	m_bShadow	= TRUE;
	m_bAllApply = TRUE;
}


void CMarkerSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarkerSetupDlg)
	DDX_Control(pDX, IDC_TEXT_SHADOW, m_cbShadow);
	DDX_Control(pDX, IDC_CHECKBOX_ALLAPPLY, m_cbAllApply);
/*@@Zeta
	DDX_Control(pDX, IDC_TEXT_COLOR, m_ctlTxtColor);
	DDX_Control(pDX, IDC_BKG_COLOR, m_ctlBkgColor);
*/
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarkerSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CMarkerSetupDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMarkerSetupDlg::LoadCtrlModule()
{
	HMODULE hModule = NULL;
	char szMain[MAX_PATH] = {0,};

	char szModule[MAX_PATH]  = {0,};
	char szBaseDll[MAX_PATH] = {0,};

	::GetModuleFileName(NULL, szMain, sizeof(szMain));
	::PathAppend(szMain, "../..");
	
	CString strFile;
	strFile.Format("%s/%s/%s", szMain, DEVDIR, "Dx_ColorPicker.dll");
	
	m_hLib = LoadLibrary(strFile);
}

void CMarkerSetupDlg::FreeCtrlModule()
{
	if (NULL != m_hLib)	FreeLibrary(m_hLib);
}

typedef CWnd* (*fpaxCreate)(CWnd *parent, void* pParam);

/////////////////////////////////////////////////////////////////////////////
// CMarkerSetupDlg message handlers

BOOL CMarkerSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	/*
	LoadCtrlModule();

	fpaxCreate fn;
	fn = (fpaxCreate)::GetProcAddress(m_hLib, "axCreate");

	struct _param param;
	param.rect = CRect(0, 0, 100, 20);
	param.options = "/s IB202200 /k IB202200_BK";
	
	if (fn) 
	{
		m_pWnd = fn(this, &param);
	}
	*/

/*@@Zeta
	m_ctlBkgColor.SetColor(m_clrMarkerBKG);
	m_ctlTxtColor.SetColor(m_clrMarkerTXT);
*/
  m_cbShadow.SetCheck(m_bShadow);
	m_cbAllApply.SetCheck(m_bAllApply);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMarkerSetupDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_hLib)	FreeCtrlModule();

	if (m_pWnd)	delete m_pWnd;
}

void CMarkerSetupDlg::OnOK() 
{
/*@@Zeta
	m_clrMarkerBKG = m_ctlBkgColor.GetColor();
	m_clrMarkerTXT = m_ctlTxtColor.GetColor();
*/
	m_bShadow = m_cbShadow.GetCheck();
	m_bAllApply = m_cbAllApply.GetCheck();
	
	CDialog::OnOK();
}
