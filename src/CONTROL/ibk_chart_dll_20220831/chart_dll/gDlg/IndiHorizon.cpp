#include "stdafx.h"
#include "axisgdlg.h"
#include "IndiHorizon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIndiHorizon::CIndiHorizon(CWnd* pParent /*=NULL*/)
	: CDialog(CIndiHorizon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndiHorizon)
	//}}AFX_DATA_INIT
}

void CIndiHorizon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndiHorizon)
	DDX_Control(pDX, IDC_COMBO_OVER, m_cbFillOver);
	DDX_Control(pDX, IDC_EDIT, m_input);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_CB_WIDTH, m_width);
	DDX_Control(pDX, IDC_CB_KIND, m_kind);
	DDX_Control(pDX, IDC_CB_COLOR, m_color);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIndiHorizon, CDialog)
	//{{AFX_MSG_MAP(CIndiHorizon)
	ON_BN_CLICKED(IDC_BT_ADD, OnClickedBtAdd)
	ON_BN_CLICKED(IDC_BT_MODIFY, OnClickedBtModify)
	ON_BN_CLICKED(IDC_BT_DEL, OnClickedBtDel)
	ON_BN_CLICKED(IDC_BT_DELALL, OnClickedBtDelall)
	ON_BN_CLICKED(IDC_CHK_GO_USERLINE, OnClickedChkGoUserline)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_CTRLMSG, OnControls)
	ON_CBN_SELCHANGE(IDC_COMBO_OVER, OnSelchangeComboOver)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CIndiHorizon::SetScreen(struct _graph* pGraph)
{
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
	m_pGraph = pGraph;
	m_pIndi = pApp->GetIndiObj(m_pGraph->wGKind);


	m_arListinfo.RemoveAll();
	m_list.ResetContent();

	

//	struct	_line	line;
//	line.lcolor = RGB(255, 0, 0);
//	line.btLStyle = PS_SOLID;
//	line.btLWidth = 1;
//	SetInfo(&line, 0);

	WORD wOpt = m_pGraph->tInfo.wCOption;
	for ( int ii = 0 ; ii < MAX_ULINE ; ii++ )
	{
		if (!m_pGraph->tInfo.aline[ii].btLWidth)
			continue;
		
		struct _listtemp* pList = new struct _listtemp;
		CString strTemp = MakeString((double)m_pGraph->tInfo.afValue[ii]);
		pList->value = m_pGraph->tInfo.afValue[ii];
		pList->btFillOver = m_pGraph->tInfo.abtFillOver[ii];
		memcpy(&pList->line, &m_pGraph->tInfo.aline[ii], SZ_LINE);
		
		m_list.AddColor(strTemp, m_pGraph->tInfo.aline[ii].rgbLColor);
		m_arListinfo.Add(pList);

		if (wOpt & 0x01)
			m_list.SetCheck(true, ii);
		else
			m_list.SetCheck(false, ii);
		wOpt = wOpt >> 1;
	}

	if (m_list.GetCount())
	{
		m_list.SetCurSel(0);
		struct _listtemp* plt = m_arListinfo.GetAt(0);
		SetInfo(plt);

		if (m_list.GetCount() >= MAX_ULINE)
			GetDlgItem(IDC_BT_ADD)->EnableWindow(FALSE);
		else
			GetDlgItem(IDC_BT_ADD)->EnableWindow(TRUE);
	}	

	CButton* pButton = (CButton*)GetDlgItem(IDC_CHK_GO_USERLINE);
	pButton->SetCheck(((m_pGraph->dwDraOption & GO_USERLINE) ? 0 : 1));

	if ((m_pGraph->dwDraOption & GO_FILLOVER) != GO_FILLOVER)
		m_cbFillOver.EnableWindow(false);
}

void CIndiHorizon::OnClickedBtAdd() 
{
	CString		temp;
	CString		tmpstr;
	m_input.GetWindowText(temp);

	if (temp.IsEmpty() || m_list.GetCount() >= MAX_ULINE)
		return;

	CString	chkStr;
	for (int ii = 0; ii < m_list.GetCount(); ii++)
	{
		m_list.GetText(ii, chkStr);
		if (atof(chkStr) == atof(temp))
			return;
	}
	
	tmpstr = MakeString(temp);	
	m_list.AddColor(tmpstr, RGB(255, 0, 0));
	m_input.SetWindowText(tmpstr);
	struct _listtemp*	pList = NULL;
	pList = new struct _listtemp;
	pList->value = (float)atof(tmpstr);
	pList->line.rgbLColor = RGB(255, 0, 0);
	pList->line.btLStyle = PS_SOLID;
	pList->line.btLWidth = 1;
	pList->btFillOver = 0;
	m_arListinfo.Add(pList);
	int	nCount = m_list.GetCount();
	m_list.SetCheck(true, nCount - 1);
	m_list.SetCurSel(nCount - 1);
	SetInfo(pList);
	m_input.SetSel(0, -1);
	m_input.SetFocus();

	SaveAll();
	if (m_list.GetCount() >= MAX_ULINE)
		GetDlgItem(IDC_BT_ADD)->EnableWindow(FALSE);
}

void CIndiHorizon::OnClickedBtModify()
{
	int	nCurSel = m_list.GetCurSel();

	if (nCurSel < 0)
		return;

	CString		temp;
	CString		tmpstr;
	m_input.GetWindowText(temp);

	if (temp.IsEmpty())
		return;	
	
	tmpstr = MakeString(temp);
	m_list.GetText(nCurSel, temp);

	if (temp == tmpstr)
		return;
	
	struct	_listtemp*	pList = NULL;
	pList = m_arListinfo.GetAt(nCurSel);
	pList->value = (float)atof(tmpstr);
	m_list.SetItemText(nCurSel, tmpstr);
	m_input.SetSel(0, -1);
	m_input.SetFocus();

	SaveAll();
}

void CIndiHorizon::OnClickedBtDel()
{
	int	 nIndex = m_list.GetCurSel();

	if (nIndex < 0)
		return;

	m_arListinfo.RemoveAt(nIndex);
	m_list.DeleteString(nIndex);

	if (m_list.GetCount())
	{
		m_list.SetCurSel(0);
		struct	_listtemp*	pList = NULL;
		pList = m_arListinfo.GetAt(0);
		SetInfo(pList);
		CString	tmpstr;
		tmpstr = MakeString((double)pList->value);
		m_input.SetWindowText(tmpstr);
	}
	else
	{
		struct	_listtemp	lt;
		lt.line.rgbLColor = RGB(255, 0, 0);
		lt.line.btLStyle = PS_SOLID;
		lt.line.btLWidth = 1;
		lt.btFillOver = 0;
		SetInfo(&lt);
		m_input.SetWindowText("");
	}

	SaveAll();
	if (m_list.GetCount() < MAX_ULINE)
		GetDlgItem(IDC_BT_ADD)->EnableWindow(TRUE);
}

void CIndiHorizon::OnClickedBtDelall()
{
	m_list.ResetContent();
	m_arListinfo.RemoveAll();

	struct _listtemp lt;
	lt.line.rgbLColor = RGB(255, 0, 0);
	lt.line.btLStyle = PS_SOLID;
	lt.line.btLWidth = 1;
	lt.btFillOver = 0;
	SetInfo(&lt);
	m_input.SetWindowText("");

	SaveAll();
	GetDlgItem(IDC_BT_ADD)->EnableWindow(TRUE);
}

BOOL CIndiHorizon::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_list.ResetContent();
	m_arListinfo.RemoveAll();
	m_list.setCheckList(true);
	m_cbFillOver.AddString("일반선");
	m_cbFillOver.AddString("과매수선");
	m_cbFillOver.AddString("과매도선");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
void CIndiHorizon::SetInfo(struct _line* pline, BYTE btFillOver)
{
	m_color.SetColor(pline->lcolor);
	m_kind.SetLine(pline->btLStyle);
	m_width.SetLineWidth(pline->btLWidth);
	m_cbFillOver.SetCurSel(btFillOver);
}
*/
void CIndiHorizon::SetInfo(struct _listtemp* plt)
{
	m_color.SetColor(plt->line.rgbLColor);
	m_kind.SetLine(plt->line.btLStyle);
	m_width.SetLineWidth(plt->line.btLWidth);
	m_cbFillOver.SetCurSel(plt->btFillOver);
}

void CIndiHorizon::SetCheckBit(int nIndex, bool bCheck)
{
	int	nCount = m_list.GetCount();

	if (nIndex >= nCount || nIndex < 0)
		return;

	WORD	opt = 0;
	WORD	flag = 0x01;

	opt = flag << nIndex;

	if (bCheck)
		m_pGraph->tInfo.wCOption |= opt;
	else
		m_pGraph->tInfo.wCOption &= ~opt;	
}

void CIndiHorizon::RefreshCheck()
{
	int	nCount = m_list.GetCount();	
	WORD	opt = 0x01;

	m_pGraph->tInfo.wCOption = 0;

	for ( int ii = 0 ; ii < nCount ; ii++ )
	{
		opt = 0x01;

		if (m_list.GetCheck(ii))
		{
			opt = opt << ii;
			m_pGraph->tInfo.wCOption |= opt;
		}
		else
		{
			opt = opt << ii;
			m_pGraph->tInfo.wCOption &= ~opt;
		}
	}
}

void CIndiHorizon::SaveAll()
{
	COLORREF rgbOverBuy = m_pGraph->tInfo.rgbOverBuy;
	COLORREF rgbOverSell = m_pGraph->tInfo.rgbOverSell;
	memset(&m_pGraph->tInfo, 0x00, SZ_TICKINFO);
	m_pGraph->tInfo.rgbOverBuy = rgbOverBuy;
	m_pGraph->tInfo.rgbOverSell = rgbOverSell;

	RefreshCheck();
	int	ii = 0, nCount = m_arListinfo.GetSize();
	struct	_listtemp*	pLine = NULL;

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		pLine = m_arListinfo.GetAt(ii);
		m_pGraph->tInfo.afValue[ii] = pLine->value;
		memcpy(&m_pGraph->tInfo.aline[ii], &pLine->line, SZ_LINE);

		m_pGraph->tInfo.abtFillOver[ii] = pLine->btFillOver;
	}
}

void CIndiHorizon::OnDestroy() 
{
	CDialog::OnDestroy();
}

LRESULT CIndiHorizon::OnControls(WPARAM wParam, LPARAM lParam)
{	
	if (HIWORD(wParam) == CTRL_COMBOSELECT || HIWORD(wParam) == CTRL_LISTSELECT)
	{
		int nCurSel = m_list.GetCurSel();
		if (nCurSel < 0) return 0;

		struct _listtemp* pList = m_arListinfo.GetAt(nCurSel);

		switch (LOWORD(wParam))
		{
		case IDC_CB_COLOR:
			pList->line.rgbLColor = m_color.GetColor();
			m_list.SetColor(m_color.GetColor(), nCurSel);
			SaveAll();
			break;
		case IDC_CB_WIDTH:
			pList->line.btLWidth = m_width.GetLineWidth();
			SaveAll();
			break;
		case IDC_CB_KIND:
			pList->line.btLStyle = m_kind.GetLine();
			SaveAll();
			break;
		case IDC_LIST:
			{
				SetInfo(pList);
				CString	tmpstr;

				tmpstr = MakeString((double)pList->value);
				m_input.SetWindowText(tmpstr);
				m_input.SetSel(0, -1);
				m_input.SetFocus();
			}
			break;
		}
	}

	return 0;
}

BOOL CIndiHorizon::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{		
		CWnd*	pWnd = this->GetFocus();
				
		if (pMsg->wParam == VK_RETURN)
		{
			if (pWnd)
			{
				if (pWnd->m_hWnd == m_input.m_hWnd)
				{
					OnClickedBtAdd();
					return TRUE;
				}
			}

			return TRUE;
		}				
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

void CIndiHorizon::OnClickedChkGoUserline() 
{
	CButton*	pButton = (CButton*)GetDlgItem(IDC_CHK_GO_USERLINE);	

	if (pButton->GetCheck())
		m_pGraph->dwDraOption &= ~GO_USERLINE;
	else
		m_pGraph->dwDraOption |= GO_USERLINE;
}

CString CIndiHorizon::MakeString(CString szString)
{
	return MakeString(atof(szString));
}

CString CIndiHorizon::MakeString(double	fVal)
{
	CString	szOutput, szLeft, szRight;
	int	nPos = 0;

	szOutput.Format("%.*lf", m_pIndi->digit, fVal);

	if (m_pIndi->digit)
	{
		nPos = szOutput.Find(".", 0);
		szLeft = szOutput.Left(nPos);
		szRight = szOutput.Mid(nPos + 1);
		szRight.MakeReverse();
		nPos = atoi(szRight);
		szRight.Format("%d", nPos);
		szRight.MakeReverse();
		szOutput = szLeft;

		if (nPos)
			szOutput = szOutput + "." + szRight;		
	}

	return szOutput;
}

void CIndiHorizon::OnSelchangeComboOver() 
{
	// TODO: Add your control notification handler code here
	struct _listtemp* pList = m_arListinfo.GetAt(m_list.GetCurSel());
	pList->btFillOver = m_cbFillOver.GetCurSel();	
	SaveAll();
}
