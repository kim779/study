// SetYAxisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axisgdlg.h"
#include "SetYAxisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static BOOL CALLBACK EnumFontProc (LPLOGFONT lplf, LPTEXTMETRIC lptm, DWORD dwType, LPARAM lpData)	
{	
	CComboBox*	pThis = (CComboBox*)lpData;

	int index = pThis->AddString(lplf->lfFaceName);
	ASSERT(index!=-1);
	
	int maxLen = lptm->tmMaxCharWidth * strlen(lplf->lfFaceName);
	int ret = pThis->SetItemData (index, dwType); 

	ASSERT(ret!=-1);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSetYAxisDlg dialog


CSetYAxisDlg::CSetYAxisDlg(char* pcInfo, char* pcTotalInfo, char* pcEnvInfo, CWnd* pwndView, CWnd* pParent /*=NULL*/)
	: CDialog(CSetYAxisDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetYAxisDlg)
	//}}AFX_DATA_INIT
	
	m_pgraph = (struct _graph*)pcInfo;
	m_pwndView = pwndView;
	m_pEnv = (struct _envInfo*)pcEnvInfo;

	int iMaxCount = ((struct _cntInfo*)pcTotalInfo)->wMainCnt + ((struct _cntInfo*)pcTotalInfo)->wAidCnt;
	pcTotalInfo += SZ_CNTINFO;	
	struct	_graph* pGraph = (struct _graph*)pcTotalInfo;

	CAxisGDlgApp* pApp = (CAxisGDlgApp*)AfxGetApp();

	for ( int ii = 0 ; ii < iMaxCount ; ii++ )
	{
		m_mapInfo.SetAt(pGraph[ii].wGKind, &pGraph[ii]);
	}

}


void CSetYAxisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetYAxisDlg)
	DDX_Control(pDX, IDC_COMBO_EDGE_COLOR, m_colorEdge);
	DDX_Control(pDX, IDC_COMBO_BG_COLOR, m_colorBG);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetYAxisDlg, CDialog)
	//{{AFX_MSG_MAP(CSetYAxisDlg)
	ON_BN_CLICKED(IDC_RADIO_AUTO, OnRadioAuto)
	ON_BN_CLICKED(IDC_RADIO_FIT, OnRadioFit)
	ON_BN_CLICKED(IDC_RADIO_FIX, OnRadioFix)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetYAxisDlg message handlers


BOOL CSetYAxisDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return Init();
	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSetYAxisDlg::Init() 
{
	// YÃà
	CString strTemp;
	strTemp.Format("%d", m_pgraph->yainfo.iFit);
	GetDlgItem(IDC_EDIT_FIT)->SetWindowText(strTemp);
	strTemp.Format("%0.2f", m_pgraph->yainfo.dFix);
	GetDlgItem(IDC_EDIT_FIX)->SetWindowText(strTemp);


	switch (m_pgraph->yainfo.cMode)
	{
	case Y_FIT:	
		((CButton*)GetDlgItem(IDC_RADIO_FIT))->SetCheck(true);
		GetDlgItem(IDC_EDIT_FIX)->EnableWindow(false);
		break;
	case Y_FIX:	
		((CButton*)GetDlgItem(IDC_RADIO_FIX))->SetCheck(true);
		GetDlgItem(IDC_EDIT_FIT)->EnableWindow(false);
		break;
	case Y_AUTO:
	default:
		((CButton*)GetDlgItem(IDC_RADIO_AUTO))->SetCheck(true);
		GetDlgItem(IDC_EDIT_FIT)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_FIX)->EnableWindow(false);
		break;
	}

	// YÃà À§Ä¡
	((CButton*)GetDlgItem(IDC_RADIO_BOTH))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_LEFT))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_RIGHT))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_RADIO_NONE))->SetCheck(0);

	UINT uiID;
	if (m_pEnv->display.dwDspOption & DO_DISPLT)
	{
		if (m_pEnv->display.dwDspOption & DO_DISPRT)
			uiID = IDC_RADIO_BOTH;
		else
			uiID = IDC_RADIO_LEFT;		
	}
	else
	{
		if (m_pEnv->display.dwDspOption & DO_DISPRT)
			uiID = IDC_RADIO_RIGHT;
		else
			uiID = IDC_RADIO_NONE;		
	}

	((CButton*)GetDlgItem(uiID))->SetCheck(1);

	// Color
	m_colorBG.SetColor(m_pEnv->display.rgbBG);
	m_colorEdge.SetColor(m_pEnv->display.rgbChartEdge);

	// font
	CWnd* pWnd = GetDlgItem(IDC_COMBO_FONT);
	CClientDC dc(this);
	EnumFonts(dc, 0,(FONTENUMPROC) EnumFontProc,(LPARAM)pWnd); //Enumerate font
	((CComboBox*)pWnd)->SetCurSel(0);

	strTemp.Format("%s", m_pEnv->display.font.acFName);	

	if (strTemp.IsEmpty())
		strTemp = _T("±¼¸²Ã¼");

	int nPos = ((CComboBox*)pWnd)->SelectString(0, strTemp);
	
	if (nPos < 0)
	{
		strTemp = _T("±¼¸²Ã¼");
		nPos = ((CComboBox*)pWnd)->SelectString(0, strTemp);
	}
	
	// size
	pWnd = GetDlgItem(IDC_COMBO_SIZE);
	for (int ii = 0 ; ii < 100 ; ii++ )
	{
		
		strTemp.Format("%d", ii+1);
		((CComboBox*)pWnd)->AddString(strTemp);
	}

	((CComboBox*)pWnd)->SetCurSel(8);

	strTemp.Format("%d", m_pEnv->display.font.btFPoint);	

	if (strTemp.IsEmpty())
		strTemp = _T("9");


	nPos = ((CComboBox*)pWnd)->SelectString(0, strTemp);
	
	if (nPos < 0)
	{
		strTemp = _T("9");
		nPos = ((CComboBox*)pWnd)->SelectString(0, strTemp);
	}


	// ±½±â
	if (m_pEnv->display.font.btFWeight)
		((CButton*)GetDlgItem(IDC_CHECK_BOLD))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_CHECK_BOLD))->SetCheck(0);


	return TRUE;
}

void CSetYAxisDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (((CButton*)GetDlgItem(IDC_RADIO_AUTO))->GetCheck())
		m_pgraph->yainfo.cMode = Y_AUTO;
	else if (((CButton*)GetDlgItem(IDC_RADIO_FIT))->GetCheck())
		m_pgraph->yainfo.cMode = Y_FIT;
	else if (((CButton*)GetDlgItem(IDC_RADIO_FIX))->GetCheck())
		m_pgraph->yainfo.cMode = Y_FIX;

	CString strTemp;
	GetDlgItem(IDC_EDIT_FIT)->GetWindowText(strTemp);
	m_pgraph->yainfo.iFit = atoi(strTemp);
	if (m_pgraph->yainfo.iFit < 1)
		m_pgraph->yainfo.iFit = 1;
	else if (m_pgraph->yainfo.iFit > 15)
		m_pgraph->yainfo.iFit = 15;

	GetDlgItem(IDC_EDIT_FIX)->GetWindowText(strTemp);
	m_pgraph->yainfo.dFix = atof(strTemp);
	if (m_pgraph->yainfo.dFix < 1)
		m_pgraph->yainfo.dFix = 1;
//	else if (m_pgraph->yainfo.dFix > 1000000)
//		m_pgraph->yainfo.dFix = 1000000;

	struct _graph* pgraphOri;
	if (m_mapInfo.Lookup(m_pgraph->wGKind, pgraphOri))
	{
		CopyMemory(&pgraphOri->yainfo, &m_pgraph->yainfo, SZ_YAXISINFO);
	}

	if (((CButton*)GetDlgItem(IDC_RADIO_LEFT))->GetCheck())
	{
		m_pEnv->display.dwDspOption &= ~DO_DISPRT;
		m_pEnv->display.dwDspOption |= DO_DISPLT;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_RIGHT))->GetCheck())
	{
		m_pEnv->display.dwDspOption &= ~DO_DISPLT;
		m_pEnv->display.dwDspOption |= DO_DISPRT;
	}
	else if (((CButton*)GetDlgItem(IDC_RADIO_BOTH))->GetCheck())
	{
		m_pEnv->display.dwDspOption |= (DO_DISPLT | DO_DISPRT);
	}
	else
	{
		m_pEnv->display.dwDspOption &= ~(DO_DISPLT | DO_DISPRT);
	}

	m_pEnv->display.rgbBG = m_colorBG.GetColor();
	m_pEnv->display.rgbChartEdge = m_colorEdge.GetColor();

	// font
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FONT);
	int iPos = pCombo->GetCurSel();
	if (iPos >= 0)
		pCombo->GetLBText(iPos, strTemp);
	memset(m_pEnv->display.font.acFName, 0x00, sizeof(m_pEnv->display.font.acFName));
	memcpy(m_pEnv->display.font.acFName, strTemp, 
		(strTemp.GetLength() > sizeof(m_pEnv->display.font.acFName)) ? sizeof(m_pEnv->display.font.acFName) : strTemp.GetLength());

	for (int ii = 0 ; ii < 3 ; ii++ )
	{
		memset(&m_pEnv->display.atick[ii].tfont.acFName, 0x00, sizeof(m_pEnv->display.atick[ii].tfont.acFName));
		memcpy(&m_pEnv->display.atick[ii].tfont.acFName, strTemp, 
			(strTemp.GetLength() > sizeof(m_pEnv->display.atick[ii].tfont.acFName)) ? sizeof(m_pEnv->display.atick[ii].tfont.acFName) : strTemp.GetLength());
	}

	// size
	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SIZE);
	iPos = pCombo->GetCurSel();
	if (iPos >= 0)
		pCombo->GetLBText(iPos, strTemp);
	m_pEnv->display.font.btFPoint = atoi(strTemp);

	for ( int ii = 0 ; ii < 3 ; ii++ )
	{
		m_pEnv->display.atick[ii].tfont.btFPoint = atoi(strTemp);
	}							

	// ±½°Ô
	BYTE btWeight = 0;
	if (((CButton*)GetDlgItem(IDC_CHECK_BOLD))->GetCheck())
		btWeight = m_pEnv->display.font.btFWeight = 1;
	else
		btWeight = m_pEnv->display.font.btFWeight = 0;

	for ( int ii = 0 ; ii < 3 ; ii++ )
	{
		m_pEnv->display.atick[ii].tfont.btFWeight = btWeight;
	}
	
	CDialog::OnOK();
}

void CSetYAxisDlg::OnRadioAuto() 
{
	GetDlgItem(IDC_EDIT_FIT)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_FIX)->EnableWindow(false);
}

void CSetYAxisDlg::OnRadioFit() 
{
	GetDlgItem(IDC_EDIT_FIT)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_FIX)->EnableWindow(false);
}

void CSetYAxisDlg::OnRadioFix() 
{
	GetDlgItem(IDC_EDIT_FIX)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_FIT)->EnableWindow(false);
}
