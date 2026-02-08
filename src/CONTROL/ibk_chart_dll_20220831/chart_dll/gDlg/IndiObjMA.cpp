#include "stdafx.h"
#include "axisgdlg.h"
#include "IndiObjMA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIndiObjMA::CIndiObjMA(CWnd* pParent /*=NULL*/)
	: CDialog(CIndiObjMA::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndiObjMA)
	//}}AFX_DATA_INIT
	m_bEach = false;
}


void CIndiObjMA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndiObjMA)
	DDX_Control(pDX, IDC_STCVALUE, m_stcValue);
	DDX_Control(pDX, IDC_CB_MAVALUE, m_cbValue);
	DDX_Control(pDX, IDC_CB_MATYPE, m_cbType);
	DDX_Control(pDX, IDC_SPIN_PERIOD, m_period);
	DDX_Control(pDX, IDC_CB_WIDTH, m_width);
	DDX_Control(pDX, IDC_CB_KIND, m_kind);
	DDX_Control(pDX, IDC_CB_COLOR, m_color);	
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_ST_INDY, m_stIndi);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndiObjMA, CDialog)
	//{{AFX_MSG_MAP(CIndiObjMA)
	ON_WM_DESTROY()	
	ON_CBN_SELCHANGE(IDC_CB_MATYPE, OnSelchangeCbMatype)
	ON_CBN_SELCHANGE(IDC_CB_MAVALUE, OnSelchangeCbMavalue)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CTRLMSG, OnControls)
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()


void CIndiObjMA::OnDestroy() 
{
	CDialog::OnDestroy();
}

LRESULT CIndiObjMA::OnControls(WPARAM wParam, LPARAM lParam)
{
	int	nCurSel = m_list.GetCurSel();
	COLORREF	color = 0;

	if (nCurSel < 0)
		return 0;

	switch (LOWORD(wParam))
	{
	case IDC_CB_COLOR:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
		{
			color = m_color.GetColor();
			m_pGraph->cInfo.aline[nCurSel].rgbLColor = color;
			m_list.SetColor(m_pGraph->cInfo.aline[nCurSel].rgbLColor);
		}
		break;
	case IDC_CB_WIDTH:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
		{
			m_pGraph->cInfo.aline[nCurSel].btLWidth = m_width.GetLineWidth();
		}
		break;
	case IDC_CB_KIND:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
		{
			m_pGraph->cInfo.aline[nCurSel].btLStyle = m_kind.GetLine();
		}
	case IDC_SPIN_PERIOD:
		if (HIWORD(wParam) == CTRL_EDITCHANGE)
		{
			m_pGraph->cInfo.awValue[nCurSel] = m_period.GetNumber();
			CString	tmpstr, tmpstr1;
			tmpstr.Format("%d MA", (int)m_pGraph->cInfo.awValue[nCurSel]);
			m_list.SetItemText(nCurSel, tmpstr);
			GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(GetDlgCtrlID(), CTRL_VALUECHANGE));
			tmpstr.Format("%s", m_pChart->m_sztitle);
			tmpstr1 = LIB_MakeParen(m_pGraph, m_pIndi);
			tmpstr += tmpstr1;	

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
	case IDC_LIST:
		if (HIWORD(wParam) == CTRL_LISTSELECT)
		{
			int	nCurSel = m_list.GetCurSel();
	
			if (nCurSel < 0)
				return 0;

			m_list.SetFocus();
			SetColorInfo(m_pGraph->cInfo.awValue[nCurSel], &m_pGraph->cInfo.aline[nCurSel]);

			if ((int)lParam)
			{
				
				WORD	opt = 0;
				WORD	flag = 0x01;

				opt = flag << nCurSel;

				if (m_list.GetCheck(nCurSel))
					m_pGraph->cInfo.wCOption |= opt;
				else
					m_pGraph->cInfo.wCOption &= ~opt;	

			}
		}
		break;
	}

	return 0;
}

BOOL CIndiObjMA::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_period.SetMinMax(0);
	m_list.ResetContent();	

	return TRUE;
}

void CIndiObjMA::SetScreen(struct _graph* pgraph, bool bEach, bool bTitle)
{
	if (pgraph->wGKind != GK_PMA)
	{
		m_cbValue.ShowWindow(SW_HIDE);
		m_stcValue.ShowWindow(SW_HIDE);
	}

	CAxisGDlgApp* pApp = (CAxisGDlgApp*)AfxGetApp();
	
	m_pGraph = pgraph;
	m_pIndi = pApp->GetIndiObj(m_pGraph->wGKind);
	m_pChart = pApp->GetChartObj(m_pGraph->wGKind);	
	CString	strTemp, strTemp1;
	strTemp.Format("%s", m_pChart->m_sztitle);
	strTemp1 = LIB_MakeParen(m_pGraph, m_pIndi);
	strTemp += strTemp1;
	m_bEach = bEach;

	if (bTitle)
	{
		if (!m_bEach)
		{
			m_stIndi.SetWindowText(strTemp);
			GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(GetDlgCtrlID(), CTRL_VALUECHANGE));
		}
		else
		{
			m_stIndi.SetWindowText("");
			CString	strTitle;
			GetParent()->GetWindowText(strTitle);
			strTitle += strTemp;
			GetParent()->SetWindowText(strTitle);
		}
	}
	
	this->ShowWindow(SW_SHOW);

	m_list.ResetContent();
	m_list.setCheckList(true);

	WORD wOpt = m_pGraph->cInfo.wCOption;
	CString	title;

	for ( int ii = 0 ; ii < m_pIndi->lcnt ; ii++ )
	{
		title.Format("%d MA", (int)m_pGraph->cInfo.awValue[ii]); 
		m_list.AddColor(title, m_pGraph->cInfo.aline[ii].rgbLColor);
		if (wOpt & 0x01)
			m_list.SetCheck(true, ii);
		else
			m_list.SetCheck(false, ii);
		wOpt = wOpt >> 1;
	}

	m_list.SetCurSel(0);
	m_cbType.ResetContent();
	m_cbValue.ResetContent();

	int iPos = 0;
	DWORD adwValue[] = {0, GC_MAWEIGHT, GC_MAEXP};
	char* apcContent[] = {_SMA_, _WMA_, _EMA_};
	int  iCount = sizeof(apcContent) / sizeof(char*);
	for ( int ii = 0 ; ii < iCount ; ii++ )
	{
		m_cbType.AddString(apcContent[ii]);
		m_cbType.SetItemData(m_cbType.GetCount() - 1, adwValue[ii]);

		if (m_pGraph->wCalOption & adwValue[ii])
			iPos = ii;
	}
	m_cbType.SetCurSel(iPos);

	iPos = 0;
	DWORD	adwValue2[] = {0, GC_MAOPEN, GC_MAHIGH, GC_MALOW, GC_MAMID, GC_MAREP};
	char*	apcContent2[] = {_CCMA_, _OCMA_, _HCMA_, _LCMA_, _MCMA_, _RCMA_};
	iCount = sizeof(apcContent2) / sizeof(char*);
	for ( int ii = 0 ; ii < iCount ; ii++ )
	{
		m_cbValue.AddString(apcContent2[ii]);
		m_cbValue.SetItemData(m_cbValue.GetCount() - 1, adwValue2[ii]);

		if (m_pGraph->wCalOption & adwValue2[ii])
			iPos = ii;
	}
	m_cbValue.SetCurSel(iPos);
	
	m_period.SetMinMax(m_pIndi->basic[0].info.min, m_pIndi->basic[0].info.max);
	SetColorInfo(m_pGraph->cInfo.awValue[0], &m_pGraph->cInfo.aline[0]);
}

void CIndiObjMA::SetColorInfo(float value, struct _line* line)
{
	m_period.SetNumber((int)value);
	m_color.SetColor(line->rgbLColor);
	m_kind.SetLine(line->btLStyle);
	m_width.SetLineWidth(line->btLWidth);
}

void CIndiObjMA::OnSelchangeCbMatype() 
{
	int	nPos = m_cbType.GetCurSel();
	DWORD	param = 0;

	if (nPos < 0)
	{
		nPos = 0;
		m_cbType.SetCurSel(0);
	}

	param = m_cbType.GetItemData(nPos);
	m_pGraph->wCalOption &= ~(GC_MAWEIGHT|GC_MAEXP);
	m_pGraph->wCalOption |= param;
}

void CIndiObjMA::OnSelchangeCbMavalue() 
{
	int	nPos = m_cbValue.GetCurSel();
	DWORD	param = 0;

	if (nPos < 0)
	{
		nPos = 0;
		m_cbValue.SetCurSel(0);
	}

	param = m_cbValue.GetItemData(nPos);
	m_pGraph->wCalOption &= ~(GC_MAOPEN|GC_MAHIGH|GC_MALOW|GC_MAMID|GC_MAREP);
	m_pGraph->wCalOption |= param;
}

LRESULT CIndiObjMA::OnManage(WPARAM wParam, LPARAM lParam)
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
			SetScreen(m_pGraph, (lParam) ? true : false);
		}
		break;
	}

	return 0;
}
