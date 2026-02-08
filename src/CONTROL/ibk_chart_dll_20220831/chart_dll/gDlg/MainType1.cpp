#include "stdafx.h"
#include "axisGDlg.h"
#include "MainType1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMainType1::CMainType1(CWnd* pParent /*=NULL*/)
	: CDialog(CMainType1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainType1)
	//}}AFX_DATA_INIT
}

void CMainType1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainType1)
	DDX_Control(pDX, IDC_SPIN5, m_spin5);
	DDX_Control(pDX, IDC_SPIN4, m_spin4);
	DDX_Control(pDX, IDC_SPIN3, m_spin3);
	DDX_Control(pDX, IDC_CB_COLOR, m_color);
	DDX_Control(pDX, IDC_CB_VALUE, m_cbValue);
	DDX_Control(pDX, IDC_SPIN2, m_spin2);
	DDX_Control(pDX, IDC_SPIN1, m_spin1);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_CB_WIDTH, m_width);
	DDX_Control(pDX, IDC_CB_KIND, m_kind);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainType1, CDialog)
	//{{AFX_MSG_MAP(CMainType1)
	ON_MESSAGE(WM_CTRLMSG, OnControls)
	ON_CBN_SELCHANGE(IDC_CB_VALUE, OnSelchangeCbValue)
	ON_BN_CLICKED(IDC_CHK_FILL, OnChkFill)
	ON_CBN_SELCHANGE(IDC_COMBO_UNION, OnSelchangeComboUnion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMainType1::SetScreen(struct _envInfo* penvinfo, struct _graph* pGraph)
{
	CWnd* pWnd = NULL;

	m_pGraph = pGraph;
	int iGKind = m_pGraph->wGKind;
	int iIndex = 0;
	
	m_list.ResetContent();	
			
	LIB_ShowHide(this, SW_HIDE, 
		IDC_ST_CONTENT1, IDC_ST_CONTENT2, IDC_ST_CONTENT3, IDC_ST_CONTENT4, IDC_ST_CONTENT5, 
		IDC_SPIN1, IDC_SPIN2, IDC_SPIN3, IDC_SPIN4, IDC_SPIN5,
		IDC_CHK_FILL, IDC_CB_VALUE, IDC_ST_KIND,
		IDC_ST_WIDTH, IDC_CB_KIND, IDC_CB_WIDTH, -1);
	
	switch (iGKind)
	{
	case GK_CDV:
	case GK_EQV:
		{
			m_list.AddColor("상승", m_pGraph->cInfo.aline[1].rgbLColor);
			m_list.AddColor("하락", m_pGraph->cInfo.aline[2].rgbLColor);
			GetDlgItem(IDC_CHK_FILL)->ShowWindow(SW_SHOW);
		}
		break;
	case GK_THREE:
	case GK_RENKO:
		{
			m_list.AddColor("상승", m_pGraph->cInfo.aline[0].rgbLColor);
			m_list.AddColor("하락", m_pGraph->cInfo.aline[1].rgbLColor);			
			GetDlgItem(IDC_CHK_FILL)->ShowWindow(SW_SHOW);			
		}
		break;	
	case GK_POLE:
	case GK_JPN:
	case GK_BAR:
		{			
			m_list.AddColor("상승", m_pGraph->cInfo.aline[1].rgbLColor);
			m_list.AddColor("하락", m_pGraph->cInfo.aline[2].rgbLColor);
			m_list.AddColor("틱차트", m_pGraph->cInfo.aline[0].rgbLColor);

			if (penvinfo->datainfo.btIndex == GI_TICK && penvinfo->datainfo.wTGap == 1)
				iIndex = 2;
		}
		break;
	case GK_KAGI:
		{
			m_list.AddColor("상승", m_pGraph->cInfo.aline[0].rgbLColor);
			m_list.AddColor("하락", m_pGraph->cInfo.aline[1].rgbLColor);		
		}
		break;
	case GK_LIN:
		{
			LIB_ShowHide(this, SW_SHOW, IDC_ST_WIDTH, IDC_CB_WIDTH, IDC_ST_KIND, IDC_CB_KIND, -1);
			m_list.AddColor("라인", m_pGraph->cInfo.aline[0].rgbLColor);			
		}
		break;		
	case GK_PV:
	case GK_CLOCK:
		{
			LIB_ShowHide(this, SW_SHOW, IDC_ST_WIDTH, IDC_CB_WIDTH, IDC_ST_KIND, 
				IDC_CB_KIND, IDC_ST_CONTENT1, IDC_SPIN1, -1);

			CString	tmpstr;
			
			for (int ii = 0 ; ii < 5 ; ii++ )
			{
				tmpstr.Format("색상%d", ii + 1);
				m_list.AddColor(tmpstr, m_pGraph->cInfo.aline[ii].rgbLColor);
			}
			
			GetDlgItem(IDC_ST_CONTENT1)->SetWindowText("기간");
			m_spin1.SetNumber((int)m_pGraph->cInfo.awValue[0]);
		}
		break;	
	case GK_PNF:
		{
			LIB_ShowHide(this, SW_SHOW, IDC_ST_CONTENT1, IDC_SPIN1, -1);
			m_list.AddColor("상승", m_pGraph->cInfo.aline[0].rgbLColor);
			m_list.AddColor("하락", m_pGraph->cInfo.aline[1].rgbLColor);

			GetDlgItem(IDC_ST_CONTENT1)->SetWindowText("칸수");
			m_spin1.SetNumber((int)m_pGraph->cInfo.awValue[0]);
		}
		break;
	case GK_BALANCE:
		{
			LIB_ShowHide(this, SW_SHOW, IDC_ST_WIDTH, IDC_CB_WIDTH, IDC_ST_KIND, IDC_CB_KIND, 				
				IDC_ST_CONTENT1, IDC_ST_CONTENT2, IDC_ST_CONTENT3, IDC_ST_CONTENT4, IDC_ST_CONTENT5,
				IDC_SPIN1, IDC_SPIN2,  IDC_SPIN3,  IDC_SPIN4,  IDC_SPIN5, -1);

			m_list.AddColor("전환선", m_pGraph->cInfo.aline[0].rgbLColor);
			m_list.AddColor("기준선", m_pGraph->cInfo.aline[1].rgbLColor);
			m_list.AddColor("후행스팬", m_pGraph->cInfo.aline[2].rgbLColor);
			m_list.AddColor("선행스팬1", m_pGraph->cInfo.aline[3].rgbLColor);
			m_list.AddColor("선행스팬2", m_pGraph->cInfo.aline[4].rgbLColor);
			m_list.AddColor("상승구름", m_pGraph->cInfo.aline[5].rgbLColor);
			m_list.AddColor("하강구름", m_pGraph->cInfo.aline[6].rgbLColor);

			GetDlgItem(IDC_ST_CONTENT1)->SetWindowText("전환");
			GetDlgItem(IDC_ST_CONTENT3)->SetWindowText("기준");
			GetDlgItem(IDC_ST_CONTENT5)->SetWindowText("후행");
			GetDlgItem(IDC_ST_CONTENT2)->SetWindowText("선행1");
			GetDlgItem(IDC_ST_CONTENT4)->SetWindowText("선행2");

			m_spin1.SetNumber(m_pGraph->cInfo.awValue[0]);
			m_spin3.SetNumber(m_pGraph->cInfo.awValue[1]);
			if (m_pGraph->cInfo.awValue[2] > 0)
				m_spin5.SetNumber(m_pGraph->cInfo.awValue[2]);
			else
				m_spin5.SetNumber(26);
			if (m_pGraph->cInfo.awValue[3] > 0)
				m_spin2.SetNumber(m_pGraph->cInfo.awValue[3]);
			else
				m_spin2.SetNumber(26);
			if (m_pGraph->cInfo.awValue[4] > 0)
				m_spin4.SetNumber(m_pGraph->cInfo.awValue[4]);
			else
				m_spin4.SetNumber(52);
		}
		break;
	case GK_AVOL:
		{
			LIB_ShowHide(this, SW_SHOW, IDC_ST_CONTENT1, IDC_SPIN1, IDC_ST_CONTENT2, IDC_CB_VALUE, -1);
			m_list.AddColor("매물대", m_pGraph->cInfo.aline[0].rgbLColor);
			//m_list.AddColor("현재가", m_pGraph->cInfo.aline[1].lcolor);
			//m_list.AddColor("최고가", m_pGraph->cInfo.aline[2].lcolor);
			GetDlgItem(IDC_ST_CONTENT1)->SetWindowText("매물대");
			GetDlgItem(IDC_ST_CONTENT2)->SetWindowText("표시");

			m_spin1.SetNumber((int)m_pGraph->cInfo.awValue[0]);
			
			CComboBox*	pCombo = (CComboBox*)GetDlgItem(IDC_CB_VALUE);

			pCombo->ResetContent();
			pCombo->AddString("누적량");
			pCombo->AddString("퍼센트");
			pCombo->AddString("모두");
			pCombo->AddString("표시안함");

			if (m_pGraph->cInfo.awValue[1] == 3)
				pCombo->SelectString(0, "표시안함");
			else if (m_pGraph->cInfo.awValue[1] == 2)
				pCombo->SelectString(0, "모두");
			else if (m_pGraph->cInfo.awValue[1] == 1)
				pCombo->SelectString(0, "퍼센트");
			else
				pCombo->SelectString(0, "누적량");
		}
		break;
	
	}
	m_list.SetCurSel(iIndex);		

	// Union
	struct _unionChart {
		int	iGKind;
		char	acName[20];
	};
	struct _unionChart aUnion[] = {	{0,		"적용안함"},
					{GK_AVOL,	"매물차트"},
					{GK_PNF,	"P&F차트"},
					{GK_SWING,	"스윙차트"},
					{GK_KAGI,	"Kagi차트"},
					{GK_RENKO,	"Renko차트"},
					{GK_THREE,	"삼선전환도"},
					{GK_GUKKI,	"꺾은선차트"},
	};


	iIndex = penvinfo->datainfo.btIndex;
	int iUnit = 0;
	switch (penvinfo->datainfo.btUnit)
	{
	case GU_FUTURE:
	case GU_OPTION:
	case GU_COFUT:
	case GU_COOPT:
		iUnit = GU_FUTURE-GU_CODE;
		break;
	case GU_CODE:
	case GU_INDEX:	
		iUnit = penvinfo->datainfo.btUnit - GU_CODE;
		break;
	case GU_FOREIGN:
		iUnit = 3;
		break;
	default:
		break;
	}

	m_wUnionKind = penvinfo->datainfo.aaDayInfo[iUnit][iIndex - GI_DAY].wUnionGKind;


	int iSelCombo = 0;
	CComboBox* pcbUnion = (CComboBox*)GetDlgItem(IDC_COMBO_UNION);
	for (int ii = 0; ii < sizeof(aUnion) / sizeof(_unionChart); ii++)
	{
		pcbUnion->AddString(aUnion[ii].acName);
		pcbUnion->SetItemData(ii, aUnion[ii].iGKind);

		if ((int)m_wUnionKind == aUnion[ii].iGKind)
		{
			iSelCombo = ii;
			GetParent()->PostMessage(WM_MANAGE, MNG_UNION_PREVIEW, m_wUnionKind);
		}
	}

	pcbUnion->SetCurSel(iSelCombo);
	

	switch (iGKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_LIN:
	case GK_BALANCE:
	case GK_POLE:
//	case GK_AVOL:
//	case GK_EQV:
//	case GK_CDV:
		pcbUnion->EnableWindow(TRUE);
		break;

	default:
		pcbUnion->EnableWindow(FALSE);
		break;
	}
	

	
	SendMessage(WM_CTRLMSG, MAKEWPARAM(m_list.GetDlgCtrlID(), CTRL_LISTSELECT));
}
/*
BOOL CMainType1::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	return TRUE;
}
*/
LRESULT CMainType1::OnControls(WPARAM wParam, LPARAM lParam)
{
	WORD	key = LOWORD(wParam);
	int	nCurSel = m_list.GetCurSel();
	int	nKind = 0;

	switch (key)
	{
	case IDC_CB_COLOR:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
		{
			switch (m_pGraph->wGKind)
			{
			case GK_JPN:
			case GK_POLE:
			case GK_BAR:
				if (nCurSel == 2)
					nKind = GK_LIN;
			case GK_EQV:
			case GK_CDV:				
				m_pGraph->cInfo.aline[(nCurSel + 1) % 3].rgbLColor = m_color.GetColor();
				m_list.SetColor(m_pGraph->cInfo.aline[(nCurSel + 1) % 3].rgbLColor, nCurSel);
				break;
			default:
				m_pGraph->cInfo.aline[nCurSel].rgbLColor = m_color.GetColor();
				m_list.SetColor(m_pGraph->cInfo.aline[nCurSel].rgbLColor, nCurSel);
			}
			GetParent()->SendMessage(WM_MANAGE, MNG_REDRAW, nKind);			
		}
		break;
	case IDC_CB_KIND:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
		{
			switch (m_pGraph->wGKind)
			{
			case GK_JPN:
			case GK_POLE:
			case GK_BAR:
				if (nCurSel == 2)
					nKind = GK_LIN;
				m_pGraph->cInfo.aline[(nCurSel + 1) % 3].btLStyle = m_kind.GetLine();
				break;
			default:
				m_pGraph->cInfo.aline[nCurSel].btLStyle = m_kind.GetLine();
				break;
			}
			GetParent()->SendMessage(WM_MANAGE, MNG_REDRAW, nKind);
		}
		break;
	case IDC_CB_WIDTH:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
		{
			switch (m_pGraph->wGKind)
			{
			case GK_JPN:
			case GK_POLE:
			case GK_BAR:
				if (nCurSel == 2)
					nKind = GK_LIN;
				m_pGraph->cInfo.aline[(nCurSel + 1) % 3].btLWidth = m_width.GetLineWidth();
				break;
			default:
				m_pGraph->cInfo.aline[nCurSel].btLWidth = m_width.GetLineWidth();
				break;
			}
			GetParent()->SendMessage(WM_MANAGE, MNG_REDRAW, nKind);
		}
		break;
	case IDC_SPIN1:
		switch (m_pGraph->wGKind)
		{
		case GK_BALANCE:
		case GK_PNF:
		case GK_AVOL:
		case GK_PV:
		case GK_CLOCK:
			{
				m_pGraph->cInfo.awValue[0] = m_spin1.GetNumber();

				if (m_pGraph->wGKind == GK_AVOL)
					GetParent()->SendMessage(WM_MANAGE, MNG_REDRAW);
			}
			break;
		default:
			break;
		}
		break;
	case IDC_SPIN2:
		if (m_pGraph->wGKind != GK_BALANCE)	
			break;

		m_pGraph->cInfo.awValue[3] = m_spin2.GetNumber();
		break;
	case IDC_SPIN3:
		if (m_pGraph->wGKind != GK_BALANCE)	
			break;

		m_pGraph->cInfo.awValue[1] = m_spin3.GetNumber();
		break;
	case IDC_SPIN4:
		if (m_pGraph->wGKind != GK_BALANCE)	
			break;

		m_pGraph->cInfo.awValue[4] = m_spin4.GetNumber();
		break;
	case IDC_SPIN5:
		if (m_pGraph->wGKind != GK_BALANCE)	
			break;

		m_pGraph->cInfo.awValue[2] = m_spin5.GetNumber();
		break;
	case IDC_LIST:
		{	
			if (HIWORD(wParam) == CTRL_LISTSELECT)
			{
				
				int	nKind = 0;
				switch (m_pGraph->wGKind)
				{							
				case GK_CDV:
				case GK_EQV:
					{
						CButton* pButton = (CButton*)GetDlgItem(IDC_CHK_FILL);

						m_color.SetColor(m_pGraph->cInfo.aline[nCurSel + 1].rgbLColor);

						if (!nCurSel)
						{						
							if (m_pGraph->dwDraOption & GO_FIILUPBONG)
								pButton->SetCheck(1);
							else
								pButton->SetCheck(0);
						}
						else
						{
							if (m_pGraph->dwDraOption & GO_FIILDNBONG)
								pButton->SetCheck(1);
							else
								pButton->SetCheck(0);
						}
					}
					break;
				case GK_THREE:
				case GK_RENKO:
					{
						CButton* pButton = (CButton*)GetDlgItem(IDC_CHK_FILL);

						m_color.SetColor(m_pGraph->cInfo.aline[nCurSel].rgbLColor);

						if (!nCurSel)
						{						
							if (m_pGraph->dwDraOption & GO_FIILUPBONG)
								pButton->SetCheck(1);
							else
								pButton->SetCheck(0);
						}
						else
						{
							if (m_pGraph->dwDraOption & GO_FIILDNBONG)
								pButton->SetCheck(1);
							else
								pButton->SetCheck(0);
						}
					}
					break;
				case GK_JPN:					
				case GK_POLE:
				case GK_BAR:	
					{
						if (nCurSel == 2)
							nKind = GK_LIN;
						
						CButton* pButton = (CButton*)GetDlgItem(IDC_CHK_FILL);
						pButton->ShowWindow(SW_HIDE);
								
						if (nCurSel == 2)
						{
							LIB_ShowHide(this, SW_SHOW, IDC_ST_KIND, IDC_CB_KIND, IDC_ST_WIDTH, IDC_CB_WIDTH, -1);
							m_kind.SetLine(m_pGraph->cInfo.aline[(nCurSel + 1) % 3].btLStyle);
							m_width.SetLineWidth(m_pGraph->cInfo.aline[(nCurSel + 1) % 3].btLWidth);
						}
						else
						{
							LIB_ShowHide(this, SW_HIDE, IDC_ST_KIND, IDC_CB_KIND, IDC_ST_WIDTH, IDC_CB_WIDTH, -1);

							if (m_pGraph->wGKind == GK_JPN)
							{
								pButton->ShowWindow(SW_SHOW);
								if (!nCurSel)
								{						
									if (m_pGraph->dwDraOption & GO_FIILUPBONG)
										pButton->SetCheck(1);
									else
										pButton->SetCheck(0);
								}
								else
								{
									if (m_pGraph->dwDraOption & GO_FIILDNBONG)
										pButton->SetCheck(1);
									else
										pButton->SetCheck(0);
								}
							}								
						}

						m_color.SetColor(m_pGraph->cInfo.aline[(nCurSel + 1) % 3].rgbLColor);
											
					}
					break;
				case GK_PNF:
				case GK_KAGI:
				case GK_AVOL:
					m_color.SetColor(m_pGraph->cInfo.aline[nCurSel].rgbLColor);
					break;
				case GK_CLOCK:
				case GK_PV:				
				case GK_LIN:
					m_color.SetColor(m_pGraph->cInfo.aline[nCurSel].rgbLColor);
					m_kind.SetLine(m_pGraph->cInfo.aline[nCurSel].btLStyle);
					m_width.SetLineWidth(m_pGraph->cInfo.aline[nCurSel].btLWidth);
					break;
				case GK_BALANCE:
					m_color.SetColor(m_pGraph->cInfo.aline[nCurSel].rgbLColor);

					if (nCurSel < 5)
					{
						LIB_ShowHide(this, SW_SHOW, IDC_CB_WIDTH, IDC_ST_WIDTH, IDC_CB_KIND, IDC_ST_KIND);
						
						m_kind.SetLine(m_pGraph->cInfo.aline[nCurSel].btLStyle);
						m_width.SetLineWidth(m_pGraph->cInfo.aline[nCurSel].btLWidth);
					}
					else
					{
						LIB_ShowHide(this, SW_HIDE, IDC_CB_WIDTH, IDC_ST_WIDTH, IDC_CB_KIND, IDC_ST_KIND);
					}
					break;
				default:
					return 0;
				}
											
				GetParent()->SendMessage(WM_MANAGE, MNG_REDRAW, nKind);
			}
		}
		break;
	}

	return 0;
}

void CMainType1::OnSelchangeCbValue() 
{
	if (m_pGraph->wGKind == GK_AVOL)
	{
		int	nCurSel = m_cbValue.GetCurSel();
	
		if (nCurSel < 0)
			return;

		m_pGraph->cInfo.awValue[1] = nCurSel;
		GetParent()->SendMessage(WM_MANAGE, MNG_REDRAW);
	}	
}

void CMainType1::OnChkFill() 
{
	CButton*	pWnd = (CButton*)GetDlgItem(IDC_CHK_FILL);
	int		nCurSel = m_list.GetCurSel();
	
	if (pWnd->IsWindowVisible())
	{
		if (pWnd->GetCheck())
		{
			if (nCurSel == 0)
				m_pGraph->dwDraOption |= GO_FIILUPBONG;
			else
				m_pGraph->dwDraOption |= GO_FIILDNBONG;
		}
		else
		{
			if (nCurSel == 0)
				m_pGraph->dwDraOption &= ~GO_FIILUPBONG;
			else
				m_pGraph->dwDraOption &= ~GO_FIILDNBONG;
		}

		GetParent()->SendMessage(WM_MANAGE, MNG_REDRAW);		
	}	
}


void CMainType1::OnSelchangeComboUnion() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pcbUnion = (CComboBox*)GetDlgItem(IDC_COMBO_UNION);
	m_wUnionKind = (WORD)pcbUnion->GetItemData(pcbUnion->GetCurSel());

	GetParent()->PostMessage(WM_MANAGE, MNG_UNION_PREVIEW, m_wUnionKind);
}
