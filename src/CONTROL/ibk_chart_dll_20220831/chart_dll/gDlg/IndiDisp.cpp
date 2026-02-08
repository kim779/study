#include "stdafx.h"
#include "axisgdlg.h"
#include "IndiDisp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIndiDisp::CIndiDisp(CWnd* pParent /*=NULL*/)
	: CDialog(CIndiDisp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIndiDisp)
	//}}AFX_DATA_INIT
}


void CIndiDisp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIndiDisp)
	DDX_Control(pDX, IDC_CHECK1, m_check);
	DDX_Control(pDX, IDC_ST_COLOR, m_stColor);
	DDX_Control(pDX, IDC_ST_WIDTH, m_stWidth);
	DDX_Control(pDX, IDC_ST_KIND, m_stKind);
	DDX_Control(pDX, IDC_CB_WIDTH, m_width);
	DDX_Control(pDX, IDC_CB_KIND, m_kind);
	DDX_Control(pDX, IDC_CB_COLOR, m_color);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIndiDisp, CDialog)
	//{{AFX_MSG_MAP(CIndiDisp)
	ON_MESSAGE(WM_CTRLMSG, OnControls)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CIndiDisp::SetScreen(char kind, struct _line* line)
{
	m_pLine = line;

	m_check.ShowWindow(SW_HIDE);
	
	switch (kind)
	{
	case _CWK_:
		m_color.ShowWindow(SW_SHOW);
		m_stColor.ShowWindow(SW_SHOW);
		m_width.ShowWindow(SW_SHOW);
		m_stWidth.ShowWindow(SW_SHOW);
		m_kind.ShowWindow(SW_SHOW);
		m_stKind.ShowWindow(SW_SHOW);
		break;
	case _C_:
		m_color.ShowWindow(SW_SHOW);
		m_stColor.ShowWindow(SW_SHOW);
		m_width.ShowWindow(SW_HIDE);
		m_stWidth.ShowWindow(SW_HIDE);
		m_kind.ShowWindow(SW_HIDE);		
		m_stKind.ShowWindow(SW_HIDE);
		break;
	case _W_:
		m_color.ShowWindow(SW_HIDE);
		m_stColor.ShowWindow(SW_HIDE);
		m_width.ShowWindow(SW_SHOW);
		m_stWidth.ShowWindow(SW_SHOW);
		m_kind.ShowWindow(SW_HIDE);		
		m_stKind.ShowWindow(SW_HIDE);
		break;
	case _K_:
		m_color.ShowWindow(SW_HIDE);
		m_stColor.ShowWindow(SW_HIDE);
		m_width.ShowWindow(SW_HIDE);
		m_stWidth.ShowWindow(SW_HIDE);
		m_kind.ShowWindow(SW_SHOW);
		m_stKind.ShowWindow(SW_SHOW);		
		break;
	}

	m_color.SetColor(line->rgbLColor);	
	m_width.SetLineWidth(line->btLWidth);
	m_kind.SetLine(line->btLStyle);	
}

void CIndiDisp::SetScreen(struct _line* line, CString title, DWORD* opt, DWORD value)
{
	m_pLine = line;
	m_doption = opt;
	m_value = value;

	m_stColor.ShowWindow(SW_SHOW);
	m_color.ShowWindow(SW_SHOW);
	m_check.ShowWindow(SW_SHOW);
	m_check.SetWindowText(title);
	m_stKind.ShowWindow(SW_HIDE);
	m_kind.ShowWindow(SW_HIDE);
	m_stWidth.ShowWindow(SW_HIDE);
	m_width.ShowWindow(SW_HIDE);

	if ((*m_doption) & m_value)
		m_check.SetCheck(0);
	else
		m_check.SetCheck(1);
	
	m_color.SetColor(line->rgbLColor);	
}

LRESULT CIndiDisp::OnControls(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDC_CB_COLOR:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
		{
			m_pLine->rgbLColor = m_color.GetColor();
			GetParent()->SendMessage(WM_CTRLMSG, MAKEWPARAM(GetDlgCtrlID(), CTRL_COLORCHANGE), (LPARAM)m_pLine->rgbLColor);
		}
		break;
	case IDC_CB_WIDTH:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
		{
			m_pLine->btLWidth = m_width.GetLineWidth();
		}
		break;
	case IDC_CB_KIND:
		if (HIWORD(wParam) == CTRL_COMBOSELECT)
		{
			m_pLine->btLStyle = m_kind.GetLine();
		}
		break;
	}

	return 0;
}

void CIndiDisp::OnCheck1() 
{
	if (!m_check.GetCheck())
		(*m_doption) |= m_value;
	else
		(*m_doption) &= ~m_value;
}
