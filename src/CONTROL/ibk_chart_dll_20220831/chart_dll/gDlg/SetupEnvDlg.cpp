#include "stdafx.h"
#include "axisgdlg.h"
#include "SetupEnvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSetupEnvDlg::CSetupEnvDlg(CWnd* pMain, char* info, CWnd* pParent /*=NULL*/)
	: CDialog(CSetupEnvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupEnvDlg)		
	//}}AFX_DATA_INIT

	m_pMain = pMain;
	m_pEnv = (struct _envInfo*)info;
}

void CSetupEnvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupEnvDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetupEnvDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupEnvDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CTRLMSG, OnControls)
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()


void CSetupEnvDlg::SetScreen()
{
	COLORREF color[] = {
				m_pEnv->display.rgbBG, 
				m_pEnv->display.rgbEdge, 
				m_pEnv->display.rgbChartBG, 
				m_pEnv->display.rgbChartEdge,
				m_pEnv->display.hline.rgbLColor,
				m_pEnv->display.vline.rgbLColor,
				m_pEnv->display.cline.rgbLColor,
				m_pEnv->display.dline.rgbLColor,
				//m_pEnv->display.toolinfo.lcolor
				m_pEnv->display.toolinfo.rgbLine
				};

	int	ii = 0, nCount = sizeof(color) / sizeof(COLORREF);
	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		m_color[ii].SetColor(color[ii]);		
	}
	
	DWORD	lkind[] = { 
				m_pEnv->display.hline.btLStyle, 
				m_pEnv->display.vline.btLStyle,
				m_pEnv->display.cline.btLStyle,
				m_pEnv->display.dline.btLStyle,
				//m_pEnv->display.toolinfo.btLStyle
				m_pEnv->display.toolinfo.btLineStyle
				};

	nCount = sizeof(lkind) / sizeof(DWORD);
	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		m_kind[ii].SetLine(lkind[ii]);		
	}

	DWORD	btLWidth[] = { 
				m_pEnv->display.hline.btLWidth, 
				m_pEnv->display.vline.btLWidth,
				m_pEnv->display.cline.btLWidth,
				m_pEnv->display.dline.btLWidth,
				//m_pEnv->display.toolinfo.btLWidth
				m_pEnv->display.toolinfo.btLineWidth
				};

	nCount = sizeof(btLWidth) / sizeof(DWORD);
	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		m_width[ii].SetLineWidth(btLWidth[ii]);		
	}

	CWnd*	pWnd = GetDlgItem(IDC_CB_FONT_KIND);
	CString	tmpstr;
	int	nPos = 0;

	tmpstr.Format("%s", m_pEnv->display.font.acFName);	

	if (tmpstr.IsEmpty())
		tmpstr = _T("±¼¸²Ã¼");

	nPos = ((CComboBox*)pWnd)->SelectString(0, tmpstr);
	
	if (nPos < 0)
	{
		tmpstr = _T("±¼¸²Ã¼");
		nPos = ((CComboBox*)pWnd)->SelectString(0, tmpstr);
	}

	// 2006.12.26
	for (ii = 0; ii < 3; ii++)
	{
		memset(&m_pEnv->display.atick[ii].tfont.acFName, 0x00, sizeof(m_pEnv->display.atick[ii].tfont.acFName));
		memcpy(&m_pEnv->display.atick[ii].tfont.acFName, tmpstr,
			(tmpstr.GetLength() > sizeof(m_pEnv->display.atick[ii].tfont.acFName) ? sizeof(m_pEnv->display.atick[ii].tfont.acFName) : tmpstr.GetLength()));
	}


	tmpstr.Format("%d", m_pEnv->display.font.btFPoint);	

	if (tmpstr.IsEmpty())
		tmpstr = _T("9");

	pWnd = GetDlgItem(IDC_CB_FONT_SIZE);

	nPos = ((CComboBox*)pWnd)->SelectString(0, tmpstr);
	
	if (nPos < 0)
	{
		tmpstr = _T("9");
		nPos = ((CComboBox*)pWnd)->SelectString(0, tmpstr);
	}

	// 2006.12.26
	for (ii = 0; ii < 3; ii++)
	{
		m_pEnv->display.atick[ii].tfont.btFPoint = atoi(tmpstr);
		m_pEnv->display.atick[ii].tfont.btFWeight = m_pEnv->display.font.btFWeight;
	}

	if (m_pEnv->display.font.btFWeight)
		((CButton*)GetDlgItem(IDC_CHK_BOLD))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_CHK_BOLD))->SetCheck(0);
	
}

LRESULT CSetupEnvDlg::OnControls(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_CB_EBCOLOR:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.rgbBG = m_color[0].GetColor();
		break;
	case IDC_CB_EFCOLOR:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.rgbEdge = m_color[1].GetColor();
		break;
	case IDC_CB_GBCOLOR:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.rgbChartBG = m_color[2].GetColor();
		break;
	case IDC_CB_GFCOLOR:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.rgbChartEdge = m_color[3].GetColor();
		break;
	case IDC_CB_LINEC1:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.hline.rgbLColor = m_color[4].GetColor();
		break;
	case IDC_CB_LINEC2:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.vline.rgbLColor = m_color[5].GetColor();
		break;
	case IDC_CB_LINEC3:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.cline.rgbLColor = m_color[6].GetColor();
		break;
	case IDC_CB_LINEC4:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.dline.rgbLColor = m_color[7].GetColor();
		break;
	case IDC_CB_LINEC5:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			//m_pEnv->display.toolinfo.lcolor = m_color[8].GetColor();
			m_pEnv->display.toolinfo.rgbLine = m_color[8].GetColor();
		break;
	case IDC_CB_LINEK1:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.hline.btLStyle = m_kind[0].GetLine();
		break;
	case IDC_CB_LINEK2:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.vline.btLStyle = m_kind[1].GetLine();
		break;
	case IDC_CB_LINEK3:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.cline.btLStyle = m_kind[2].GetLine();
		break;
	case IDC_CB_LINEK4:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.dline.btLStyle = m_kind[3].GetLine();
		break;
	case IDC_CB_LINEK5:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			//m_pEnv->display.toolinfo.btLStyle = m_kind[4].GetLine();
			m_pEnv->display.toolinfo.btLineStyle = m_kind[4].GetLine();
		break;
	case IDC_CB_LINEW1:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.hline.btLWidth = m_width[0].GetLineWidth();
		break;
	case IDC_CB_LINEW2:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.vline.btLWidth = m_width[1].GetLineWidth();	
		break;
	case IDC_CB_LINEW3:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.cline.btLWidth = m_width[2].GetLineWidth();
		break;
	case IDC_CB_LINEW4:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			m_pEnv->display.dline.btLWidth = m_width[3].GetLineWidth();
		break;
	case IDC_CB_LINEW5:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
			//m_pEnv->display.toolinfo.btLWidth = m_width[4].GetLineWidth();
			m_pEnv->display.toolinfo.btLineWidth = m_width[4].GetLineWidth();
		break;
	}

	return 0;
}

BOOL CSetupEnvDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD(wParam) == BN_CLICKED)
	{
		switch (LOWORD(wParam))
		{
		case IDC_CHK_BOLD:
			{
				int	weight = 0;
				if (((CButton*)GetDlgItem(IDC_CHK_BOLD))->GetCheck())
					weight = m_pEnv->display.font.btFWeight = 1;
				else
					weight = m_pEnv->display.font.btFWeight = 0;

				for ( int ii = 0 ; ii < 3 ; ii++ )
				{
					m_pEnv->display.atick[ii].tfont.btFWeight = weight;
				}
			}
			break;
		}			
	}
	else if (HIWORD(wParam) == CBN_SELCHANGE)
	{
		CString	tmpstr = _T("");
		CComboBox*	pCombo = NULL;
		int		nPos = 0, ii = 0;
		switch (LOWORD(wParam))
		{
		case IDC_CB_FONT_KIND:
			pCombo = (CComboBox*)GetDlgItem(IDC_CB_FONT_KIND);
			nPos = pCombo->GetCurSel();
			if (nPos >= 0)
				pCombo->GetLBText(nPos, tmpstr);
			memset(m_pEnv->display.font.acFName, 0x00, sizeof(m_pEnv->display.font.acFName));
			memcpy(m_pEnv->display.font.acFName, tmpstr, 
				(tmpstr.GetLength() > sizeof(m_pEnv->display.font.acFName)) ? sizeof(m_pEnv->display.font.acFName) : tmpstr.GetLength());

			for ( ii = 0 ; ii < 3 ; ii++ )
			{
				memset(&m_pEnv->display.atick[ii].tfont.acFName, 0x00, sizeof(m_pEnv->display.atick[ii].tfont.acFName));
				memcpy(&m_pEnv->display.atick[ii].tfont.acFName, tmpstr, 
					(tmpstr.GetLength() > sizeof(m_pEnv->display.atick[ii].tfont.acFName)) ? sizeof(m_pEnv->display.atick[ii].tfont.acFName) : tmpstr.GetLength());
			}
			
			break;
		case IDC_CB_FONT_SIZE:
			pCombo = (CComboBox*)GetDlgItem(IDC_CB_FONT_SIZE);
			nPos = pCombo->GetCurSel();
			if (nPos >= 0)
				pCombo->GetLBText(nPos, tmpstr);
			m_pEnv->display.font.btFPoint = atoi(tmpstr);

			for ( ii = 0 ; ii < 3 ; ii++ )
			{
				m_pEnv->display.atick[ii].tfont.btFPoint = atoi(tmpstr);
			}							
			break;
		}
	}
	return CDialog::OnCommand(wParam, lParam);
}

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

BOOL CSetupEnvDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CClientDC dc(this);

	UINT	colorID[] = {
			IDC_CB_EBCOLOR,
			IDC_CB_EFCOLOR,
			IDC_CB_GBCOLOR,
			IDC_CB_GFCOLOR,
			IDC_CB_LINEC1,
			IDC_CB_LINEC2,
			IDC_CB_LINEC3,
			IDC_CB_LINEC4,
			IDC_CB_LINEC5
			};

	int	ii = 0, nCount = sizeof(m_color) / sizeof(class CColorCtrl);
	for ( ii = 0 ; ii < nCount ; ii++ )
		m_color[ii].SubclassDlgItem(colorID[ii], this);

	UINT	kindID[] = {
				IDC_CB_LINEK1,
				IDC_CB_LINEK2,
				IDC_CB_LINEK3,
				IDC_CB_LINEK4,
				IDC_CB_LINEK5
				};

	nCount = sizeof(kindID) / sizeof(UINT);
	for ( ii = 0 ; ii < nCount ; ii++ )
		m_kind[ii].SubclassDlgItem(kindID[ii], this);

	UINT	widthID[] = { 
				IDC_CB_LINEW1, 
				IDC_CB_LINEW2,
				IDC_CB_LINEW3,
				IDC_CB_LINEW4,
				IDC_CB_LINEW5
				};

	nCount = sizeof(widthID) / sizeof(UINT);	
	for ( ii = 0 ; ii < nCount ; ii++ )
		m_width[ii].SubclassDlgItem(widthID[ii], this);		

	CWnd*	pWnd = GetDlgItem(IDC_CB_FONT_KIND);
	CString	tmpstr;

	EnumFonts(dc, 0,(FONTENUMPROC) EnumFontProc,(LPARAM)pWnd); //Enumerate font
	((CComboBox*)pWnd)->SetCurSel(0);
	
	for ( ii = 0 ; ii < 100 ; ii++ )
	{
		pWnd = GetDlgItem(IDC_CB_FONT_SIZE);
		tmpstr.Format("%d", ii+1);
		((CComboBox*)pWnd)->AddString(tmpstr);
	}

	((CComboBox*)pWnd)->SetCurSel(8);

	return TRUE;
}

LRESULT CSetupEnvDlg::OnManage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case MNG_DEFAULT:
		{
			CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
			struct _envInfo*	pEnv = pApp->GetEnvDefault();

//			if (m_pEnv->wEnvOption & EO_MAP)
//				m_pEnv->display.rgbBG = RGB(220,226,234);
//			else
				m_pEnv->display.rgbBG = pEnv->display.rgbBG;
			m_pEnv->display.rgbEdge = pEnv->display.rgbEdge;
			m_pEnv->display.rgbChartBG = pEnv->display.rgbChartBG; 
			m_pEnv->display.rgbChartEdge = pEnv->display.rgbChartEdge;
			m_pEnv->display.hline.rgbLColor = pEnv->display.hline.rgbLColor;
			m_pEnv->display.vline.rgbLColor = pEnv->display.vline.rgbLColor;
			m_pEnv->display.cline.rgbLColor = pEnv->display.cline.rgbLColor;
			m_pEnv->display.dline.rgbLColor = pEnv->display.dline.rgbLColor;
			//m_pEnv->display.toolinfo.lcolor = pEnv->display.toolinfo.lcolor;
			m_pEnv->display.toolinfo.rgbLine = pEnv->display.toolinfo.rgbLine;

			m_pEnv->display.hline.btLStyle = pEnv->display.hline.btLStyle;
			m_pEnv->display.vline.btLStyle = pEnv->display.vline.btLStyle;
			m_pEnv->display.cline.btLStyle = pEnv->display.cline.btLStyle;
			m_pEnv->display.dline.btLStyle = pEnv->display.dline.btLStyle;
			//m_pEnv->display.toolinfo.btLStyle = pEnv->display.toolinfo.btLStyle;
			m_pEnv->display.toolinfo.btLineStyle = pEnv->display.toolinfo.btLineStyle;

			m_pEnv->display.hline.btLWidth = pEnv->display.hline.btLWidth;
			m_pEnv->display.vline.btLWidth = pEnv->display.vline.btLWidth;
			m_pEnv->display.cline.btLWidth = pEnv->display.cline.btLWidth;
			m_pEnv->display.dline.btLWidth = pEnv->display.dline.btLWidth;
			//m_pEnv->display.toolinfo.btLWidth = pEnv->display.toolinfo.btLWidth;
			m_pEnv->display.toolinfo.btLineWidth = pEnv->display.toolinfo.btLineWidth;

			memset(m_pEnv->display.font.acFName, 0x00, sizeof(m_pEnv->display.font.acFName));
			memcpy(m_pEnv->display.font.acFName, pEnv->display.font.acFName, sizeof(pEnv->display.font.acFName));
			
			m_pEnv->display.font.btFPoint = pEnv->display.font.btFPoint;
			m_pEnv->display.font.btFWeight = pEnv->display.font.btFWeight;
			SetScreen();
		}
		break;
	}

	return 0;
}
