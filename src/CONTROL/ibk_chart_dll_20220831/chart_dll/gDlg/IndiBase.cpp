#include "stdafx.h"
#include "axisgdlg.h"
#include "IndiBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CIndiBase::CIndiBase(CWnd* pParent /*=NULL*/)
	: CDialog(CIndiBase::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndiBase)
	//}}AFX_DATA_INIT
	m_bBig = false;
}


void CIndiBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndiBase)	
	DDX_Control(pDX, IDC_COLOR7, m_color7);
	DDX_Control(pDX, IDC_COLOR6, m_color6);
	DDX_Control(pDX, IDC_COLOR5, m_color5);
	DDX_Control(pDX, IDC_COLOR4, m_color4);
	DDX_Control(pDX, IDC_COLOR3, m_color3);
	DDX_Control(pDX, IDC_COLOR2, m_color2);
	DDX_Control(pDX, IDC_COLOR1, m_color1);
	DDX_Control(pDX, IDC_SPIN_CONTENT7, m_spin7);
	DDX_Control(pDX, IDC_SPIN_CONTENT6, m_spin6);
	DDX_Control(pDX, IDC_SPIN_CONTENT5, m_spin5);
	DDX_Control(pDX, IDC_SPIN_CONTENT4, m_spin4);
	DDX_Control(pDX, IDC_SPIN_CONTENT3, m_spin3);
	DDX_Control(pDX, IDC_SPIN_CONTENT2, m_spin2);
	DDX_Control(pDX, IDC_SPIN_CONTENT1, m_spin1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIndiBase, CDialog)
	//{{AFX_MSG_MAP(CIndiBase)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_CTRLMSG, OnControls)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CIndiBase::OnDestroy() 
{
	CDialog::OnDestroy();
	for ( int ii = 0 ; ii < 7 ; ii++ )
	{
		m_stContent[ii].Detach();
		m_cbContent[ii].Detach();
	}
}

BOOL CIndiBase::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UINT	ctrlID[4][7] = {
				{
					IDC_ST_CONTENT1, 
					IDC_ST_CONTENT2, 
					IDC_ST_CONTENT3, 
					IDC_ST_CONTENT4, 
					IDC_ST_CONTENT5, 
					IDC_ST_CONTENT6, 
					IDC_ST_CONTENT7 
				},
				{
					IDC_CB_CONTENT1, 
					IDC_CB_CONTENT2, 
					IDC_CB_CONTENT3, 
					IDC_CB_CONTENT4, 
					IDC_CB_CONTENT5, 
					IDC_CB_CONTENT6, 
					IDC_CB_CONTENT7 
				},
				{
					IDC_SPIN_CONTENT1, 
					IDC_SPIN_CONTENT2, 
					IDC_SPIN_CONTENT3, 
					IDC_SPIN_CONTENT4, 
					IDC_SPIN_CONTENT5, 
					IDC_SPIN_CONTENT6, 
					IDC_SPIN_CONTENT7
				},
				{
					IDC_COLOR1, 
					IDC_COLOR2, 
					IDC_COLOR3, 
					IDC_COLOR4, 
					IDC_COLOR5, 
					IDC_COLOR6, 
					IDC_COLOR7
				}
	};

	CWnd*	pWnd = NULL;
	
	for ( int ii = 0 ; ii < 7 ; ii++ )
	{
		pWnd = GetDlgItem(ctrlID[0][ii]);
		m_stContent[ii].Attach(pWnd->m_hWnd);
		pWnd = GetDlgItem(ctrlID[1][ii]);
		m_cbContent[ii].Attach(pWnd->m_hWnd);
		pWnd = GetDlgItem(ctrlID[2][ii]);
		m_spinContent[ii] = (CSpinCtrl*)pWnd;
		pWnd = GetDlgItem(ctrlID[3][ii]);	// added by schbang 2006.03.22
		m_apcolor[ii] = (CColorCtrl*)pWnd;
	}
	
	return TRUE;
}

void CIndiBase::SetScreen(struct _indiInfo* pIndi, struct _graph* pGraph)
{
	m_pIndi = pIndi;
	m_pGraph = pGraph;

	int	ii = 0, jj = 0;

	for ( ii = 0 ; ii < 7 ; ii++ )
	{
		if ( ii < m_pIndi->bcnt)
		{
			m_stContent[ii].ShowWindow(SW_SHOW);
			m_stContent[ii].SetWindowText(m_pIndi->basic[ii].info.title);
			
			switch (m_pIndi->basic[ii].info.kind)
			{
			case _COMBO_:
				{
					m_cbContent[ii].ResetContent();
					int	nPos = 0;

					for ( jj = 0 ; jj < m_pIndi->basic[ii].ccnt ; jj++ )
					{
						m_cbContent[ii].AddString(m_pIndi->basic[ii].combo[jj].content);
						m_cbContent[ii].SetItemData(m_cbContent[ii].GetCount() - 1, m_pIndi->basic[ii].combo[jj].value);
						if ((int)m_pGraph->cInfo.awValue[ii] == m_pIndi->basic[ii].combo[jj].value)
							nPos = jj;
					}

					m_cbContent[ii].SetCurSel(nPos);
					m_cbContent[ii].ShowWindow(SW_SHOW);			
					m_spinContent[ii]->ShowWindow(SW_HIDE);
					m_apcolor[ii]->ShowWindow(SW_HIDE);
				}
				break;
			case _MACOMBO_:
				{
					m_cbContent[ii].ResetContent();
					int	nPos = 0;

					for ( jj = 0 ; jj < m_pIndi->basic[ii].ccnt ; jj++ )
					{
						m_cbContent[ii].AddString(m_pIndi->basic[ii].combo[jj].content);
						m_cbContent[ii].SetItemData(m_cbContent[ii].GetCount() - 1, m_pIndi->basic[ii].combo[jj].value);

						if (m_pGraph->wCalOption & m_pIndi->basic[ii].combo[jj].value)
							nPos = jj;
					}

					m_cbContent[ii].SetCurSel(nPos);
					m_cbContent[ii].ShowWindow(SW_SHOW);			
					m_spinContent[ii]->ShowWindow(SW_HIDE);
					m_apcolor[ii]->ShowWindow(SW_HIDE);
				}
				break;
			case _VOLCOMBO_:
				{
					m_cbContent[ii].ResetContent();
					int	nPos = 0;

					for ( jj = 0 ; jj < m_pIndi->basic[ii].ccnt ; jj++ )
					{
						m_cbContent[ii].AddString(m_pIndi->basic[ii].combo[jj].content);
						m_cbContent[ii].SetItemData(m_cbContent[ii].GetCount() - 1, m_pIndi->basic[ii].combo[jj].value);

						if (m_pGraph->wCalOption & m_pIndi->basic[ii].combo[jj].value)
							nPos = jj;
					}

					m_cbContent[ii].SetCurSel(nPos);
					m_cbContent[ii].ShowWindow(SW_SHOW);			
					m_spinContent[ii]->ShowWindow(SW_HIDE);
					m_apcolor[ii]->ShowWindow(SW_HIDE);

					if (ii == 0 && !m_bBig)
					{
						int	nScroll = GetSystemMetrics(SM_CXVSCROLL);
						CRect	rect;
						m_cbContent[ii].GetWindowRect(rect);
						rect.left -= (nScroll * 4) / 3;
						ScreenToClient(rect);
						m_cbContent[ii].MoveWindow(rect);
						nScroll = rect.left - 2;
						m_stContent[ii].GetWindowRect(rect);
						ScreenToClient(rect);
						rect.right = nScroll;
						m_stContent[ii].MoveWindow(rect);
						m_bBig = true;
					}
				}
				break;
			case _SPIN_:
				{
					m_cbContent[ii].ShowWindow(SW_HIDE);
					m_spinContent[ii]->SetMinMax(m_pIndi->basic[ii].info.min, m_pIndi->basic[ii].info.max);
					m_spinContent[ii]->SetNumber((int)m_pGraph->cInfo.awValue[ii]);
					m_spinContent[ii]->ShowWindow(SW_SHOW);	
					m_apcolor[ii]->ShowWindow(SW_HIDE);
				}
				break;
			case _COLOR_:	// added by schbang 2006.03.22
				m_cbContent[ii].ShowWindow(SW_HIDE);
				m_spinContent[ii]->ShowWindow(SW_HIDE);
				m_apcolor[ii]->ShowWindow(SW_SHOW);
				if (strcmp(m_pIndi->basic[ii].info.title, _OVERBUY_) == 0)
					m_apcolor[ii]->SetColor(m_pGraph->tInfo.rgbOverBuy);
				else if (strcmp(m_pIndi->basic[ii].info.title, _OVERSELL_) == 0)
					m_apcolor[ii]->SetColor(m_pGraph->tInfo.rgbOverSell);
				break;
			}
		}
		else
		{
			m_stContent[ii].ShowWindow(SW_HIDE);
			m_cbContent[ii].ShowWindow(SW_HIDE);
			m_spinContent[ii]->ShowWindow(SW_HIDE);
			m_apcolor[ii]->ShowWindow(SW_HIDE);
		}
	}
}

BOOL CIndiBase::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD(wParam) == CBN_SELCHANGE)
	{
		CComboBox* pCombo = NULL;
		WORD*	value = NULL;
		pCombo = (CComboBox*)GetDlgItem(LOWORD(wParam));
		int	param = pCombo->GetItemData(pCombo->GetCurSel());

		switch (LOWORD(wParam))
		{
		case IDC_CB_CONTENT1:
			if (m_pIndi->basic[0].info.kind == _MACOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_MAWEIGHT|GC_MAEXP);
				m_pGraph->wCalOption |= param;
			}
			else if (m_pIndi->basic[0].info.kind == _VOLCOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_VOLPRC|GC_VOLVOL|GC_VOLFPR);
				m_pGraph->wCalOption |= param;
			}
			else
				value = &m_pGraph->cInfo.awValue[0];
			break;
		case IDC_CB_CONTENT2:
			if (m_pIndi->basic[1].info.kind == _MACOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_MAWEIGHT|GC_MAEXP);
				m_pGraph->wCalOption |= param;
			}
			else if (m_pIndi->basic[1].info.kind == _VOLCOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_VOLPRC|GC_VOLVOL|GC_VOLFPR);
				m_pGraph->wCalOption |= param;
			}
			else
				value = &m_pGraph->cInfo.awValue[1];
			break;
		case IDC_CB_CONTENT3:
			if (m_pIndi->basic[2].info.kind == _MACOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_MAWEIGHT|GC_MAEXP);
				m_pGraph->wCalOption |= param;
			}
			else if (m_pIndi->basic[2].info.kind == _VOLCOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_VOLPRC|GC_VOLVOL|GC_VOLFPR);
				m_pGraph->wCalOption |= param;
			}
			else
				value = &m_pGraph->cInfo.awValue[2];
			break;
		case IDC_CB_CONTENT4:
			if (m_pIndi->basic[3].info.kind == _MACOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_MAWEIGHT|GC_MAEXP);
				m_pGraph->wCalOption |= param;
			}
			else if (m_pIndi->basic[3].info.kind == _VOLCOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_VOLPRC|GC_VOLVOL|GC_VOLFPR);
				m_pGraph->wCalOption |= param;
			}
			else
				value = &m_pGraph->cInfo.awValue[3];
			break;
		case IDC_CB_CONTENT5:
			if (m_pIndi->basic[4].info.kind == _MACOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_MAWEIGHT|GC_MAEXP);
				m_pGraph->wCalOption |= param;
			}
			else if (m_pIndi->basic[4].info.kind == _VOLCOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_VOLPRC|GC_VOLVOL|GC_VOLFPR);
				m_pGraph->wCalOption |= param;
			}
			else
				value = &m_pGraph->cInfo.awValue[4];
			break;
		case IDC_CB_CONTENT6:
			if (m_pIndi->basic[5].info.kind == _MACOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_MAWEIGHT|GC_MAEXP);
				m_pGraph->wCalOption |= param;
			}
			else if (m_pIndi->basic[5].info.kind == _VOLCOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_VOLPRC|GC_VOLVOL|GC_VOLFPR);
				m_pGraph->wCalOption |= param;
			}
			else
				value = &m_pGraph->cInfo.awValue[5];
			break;
		case IDC_CB_CONTENT7:
			if (m_pIndi->basic[6].info.kind == _MACOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_MAWEIGHT|GC_MAEXP);
				m_pGraph->wCalOption |= param;
			}
			else if (m_pIndi->basic[6].info.kind == _VOLCOMBO_)
			{
				m_pGraph->wCalOption &= ~(GC_VOLPRC|GC_VOLVOL|GC_VOLFPR);
				m_pGraph->wCalOption |= param;
			}
			else
				value = &m_pGraph->cInfo.awValue[6];
			break;
		}

		if (value)
			(*value) = pCombo->GetCurSel();

		GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(GetDlgCtrlID(), CTRL_VALUECHANGE));
	}
			
	return CDialog::OnCommand(wParam, lParam);
}

LRESULT CIndiBase::OnControls(WPARAM wParam, LPARAM lParam)
{	
	if (HIWORD(wParam) == CTRL_EDITCHANGE)
	{
		int	nIndex = 0;

		switch (LOWORD(wParam))
		{
		case IDC_SPIN_CONTENT1:
			nIndex = 0;
			break;
		case IDC_SPIN_CONTENT2:
			nIndex = 1;
			break;
		case IDC_SPIN_CONTENT3:
			nIndex = 2;
			break;
		case IDC_SPIN_CONTENT4:
			nIndex = 3;
			break;
		case IDC_SPIN_CONTENT5:
			nIndex = 4;
			break;
		case IDC_SPIN_CONTENT6:
			nIndex = 5;
			break;
		case IDC_SPIN_CONTENT7:
			nIndex = 6;
			break;
		}

		m_pGraph->cInfo.awValue[nIndex] = m_spinContent[nIndex]->GetNumber();
		GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(GetDlgCtrlID(), CTRL_VALUECHANGE));
	}

	if (HIWORD(wParam) == CTRL_COMBOSELECT)
	{
		switch (LOWORD(wParam))
		{
		case IDC_COLOR1:
			if (strcmp(m_pIndi->basic[0].info.title, _OVERBUY_) == 0)
				m_pGraph->tInfo.rgbOverBuy = m_color1.GetColor();
			else if (strcmp(m_pIndi->basic[0].info.title, _OVERSELL_) == 0)
				m_pGraph->tInfo.rgbOverSell = m_color1.GetColor();
			break;
		case IDC_COLOR2:
			if (strcmp(m_pIndi->basic[1].info.title, _OVERBUY_) == 0)
				m_pGraph->tInfo.rgbOverBuy = m_color2.GetColor();
			else if (strcmp(m_pIndi->basic[1].info.title, _OVERSELL_) == 0)
				m_pGraph->tInfo.rgbOverSell = m_color2.GetColor();
			break;
		case IDC_COLOR3:
			if (strcmp(m_pIndi->basic[2].info.title, _OVERBUY_) == 0)
				m_pGraph->tInfo.rgbOverBuy = m_color3.GetColor();
			else if (strcmp(m_pIndi->basic[2].info.title, _OVERSELL_) == 0)
				m_pGraph->tInfo.rgbOverSell = m_color3.GetColor();
			break;
		case IDC_COLOR4:
			if (strcmp(m_pIndi->basic[3].info.title, _OVERBUY_) == 0)
				m_pGraph->tInfo.rgbOverBuy = m_color4.GetColor();
			else if (strcmp(m_pIndi->basic[3].info.title, _OVERSELL_) == 0)
				m_pGraph->tInfo.rgbOverSell = m_color4.GetColor();
			break;
		case IDC_COLOR5:
			if (strcmp(m_pIndi->basic[4].info.title, _OVERBUY_) == 0)
				m_pGraph->tInfo.rgbOverBuy = m_color5.GetColor();
			else if (strcmp(m_pIndi->basic[4].info.title, _OVERSELL_) == 0)
				m_pGraph->tInfo.rgbOverSell = m_color5.GetColor();
			break;
		case IDC_COLOR6:
			if (strcmp(m_pIndi->basic[5].info.title, _OVERBUY_) == 0)
				m_pGraph->tInfo.rgbOverBuy = m_color6.GetColor();
			else if (strcmp(m_pIndi->basic[5].info.title, _OVERSELL_) == 0)
				m_pGraph->tInfo.rgbOverSell = m_color6.GetColor();
			break;
		case IDC_COLOR7:
			if (strcmp(m_pIndi->basic[6].info.title, _OVERBUY_) == 0)
				m_pGraph->tInfo.rgbOverBuy = m_color7.GetColor();
			else if (strcmp(m_pIndi->basic[6].info.title, _OVERSELL_) == 0)
				m_pGraph->tInfo.rgbOverSell = m_color7.GetColor();
			break;
		}
		
//		GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(GetDlgCtrlID(), CTRL_COLORCHANGE), (LPARAM)m_pLine->lcolor);
	}

	return 0;
}


