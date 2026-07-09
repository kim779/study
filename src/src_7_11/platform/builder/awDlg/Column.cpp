// column.cpp : implementation file
//

#include "stdafx.h"
#include "column.h"
#include "cbItem.h"
#include "attrdlg.h"
#include "celledit.h"

#include "../awCommon/colordlg.h"
#include "../awCommon/CellItem.h"
#include "../../dll/lib/axislib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CPR_TYPE	0
#define	CPR_MODE	1
#define	CPR_SIZE	2
#define	CPR_WIDTH	3
#define	CPR_ALIGNH	4
#define	CPR_ALIGNC	5
#define	CPR_HTRGB	6
#define	CPR_HPRGB	7
#define	CPR_TRGB	8
#define	CPR_PRGB	9
#define	CPR_ATTR	10

/////////////////////////////////////////////////////////////////////////////
// CColumn dialog

CColumn::CColumn(CWnd* pParent, struct _repR* repR, CAxisPalette* pal, DWORD attr, bool common)
	: CDialog(CColumn::IDD, pParent)
{
	m_pRepR   = repR;
	m_palette = pal;
	m_attr    = attr;
	m_parent  = pParent;
	m_common  = common;

	m_sel = 0;
	m_first = true;
	/*CCellItem *cellItem = new CCellItem(m_pRepR);*/
	CopyMemory(&m_repR, m_pRepR, sizeof(_repR));
	/*m_repR.cellR = NULL;
	for (int ii = 0; ii < */
	
	//{{AFX_DATA_INIT(CColumn)
	m_domino = _T("");
	m_float = _T("");
	m_hpRGB = _T("");
	m_htRGB = _T("");
	m_len = _T("");
	m_multi = FALSE;
	m_pRGB = _T("");
	m_width = _T("");
	m_head = _T("");
	m_symbol = _T("");
	m_tRGB = _T("");
	m_visible = FALSE;
	m_reference = _T("");
	m_sort = FALSE;
	m_check = _T("");
	m_cut = FALSE;
	//}}AFX_DATA_INIT
}

void CColumn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColumn)
	DDX_Text(pDX, IDC_DOMINO, m_domino);
	DDV_MaxChars(pDX, m_domino, 16);
	DDX_Text(pDX, IDC_FLOAT, m_float);
	DDV_MaxChars(pDX, m_float, 2);
	DDX_Text(pDX, IDC_HPRGBE, m_hpRGB);
	DDV_MaxChars(pDX, m_hpRGB, 8);
	DDX_Text(pDX, IDC_HTRGBE, m_htRGB);
	DDV_MaxChars(pDX, m_htRGB, 8);
	DDX_Text(pDX, IDC_LENE, m_len);
	DDV_MaxChars(pDX, m_len, 3);
	DDX_Check(pDX, IDC_MULTI, m_multi);
	DDX_Text(pDX, IDC_PRGBE, m_pRGB);
	DDV_MaxChars(pDX, m_pRGB, 8);
	DDX_Text(pDX, IDC_RIGHT, m_width);
	DDV_MaxChars(pDX, m_width, 4);
	DDX_Text(pDX, IDC_STR, m_head);
	DDV_MaxChars(pDX, m_head, 64);
	DDX_Text(pDX, IDC_SYMBOL, m_symbol);
	DDV_MaxChars(pDX, m_symbol, 16);
	DDX_Text(pDX, IDC_TRGBE, m_tRGB);
	DDV_MaxChars(pDX, m_tRGB, 8);
	DDX_Check(pDX, IDC_VISIBLE, m_visible);
	DDX_Text(pDX, IDC_REF, m_reference);
	DDV_MaxChars(pDX, m_reference, 16);
	DDX_Check(pDX, IDC_SORT, m_sort);
	DDX_Text(pDX, IDC_CHECKSTR, m_check);
	DDX_Check(pDX, IDC_CUT, m_cut);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColumn, CDialog)
	//{{AFX_MSG_MAP(CColumn)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_EN_CHANGE(IDC_PRGBE, OnChangeColor)
	ON_BN_CLICKED(IDC_DOMINOC, OnDominoCheck)
	ON_BN_CLICKED(IDC_REFC, OnReferenceCheck)
	ON_BN_CLICKED(IDC_DATA, OnEdit)
	ON_BN_CLICKED(IDC_ATTR, OnAttr)
	ON_BN_CLICKED(IDC_HPRGB, OnHeadPRGB)
	ON_BN_CLICKED(IDC_HTRGB, OnHeadTRGB)
	ON_BN_CLICKED(IDC_PRGB, OnPRGB)
	ON_BN_CLICKED(IDC_TRGB, OnTRGB)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelChangeDataType)
	ON_CBN_SELCHANGE(IDC_KIND, OnSelChangeKind)
	ON_CBN_SELCHANGE(IDC_MODE, OnSelChangeMode)
	ON_BN_CLICKED(IDC_OPT01, OnColumn01)
	ON_BN_CLICKED(IDC_OPT02, OnColumn02)
	ON_BN_CLICKED(IDC_OPT03, OnColumn03)
	ON_BN_CLICKED(IDC_OPT04, OnColumn04)
	ON_BN_CLICKED(IDC_OPT05, OnColumn05)
	ON_BN_CLICKED(IDC_OPT06, OnColumn06)
	ON_BN_CLICKED(IDC_OPT07, OnColumn07)
	ON_BN_CLICKED(IDC_OPT08, OnColumn08)
	ON_BN_CLICKED(IDC_OPT09, OnColumn09)
	ON_BN_CLICKED(IDC_OPT10, OnColumn10)
	ON_BN_CLICKED(IDC_OPT11, OnColumn11)
	ON_BN_CLICKED(IDC_OPT12, OnColumn12)
	ON_BN_CLICKED(IDC_OPT13, OnColumn13)
	ON_BN_CLICKED(IDC_OPT14, OnColumn14)
	ON_BN_CLICKED(IDC_OPT15, OnColumn15)
	ON_BN_CLICKED(IDC_OPT16, OnColumn16)
	ON_BN_CLICKED(IDC_OPT17, OnColumn17)
	ON_BN_CLICKED(IDC_OPT18, OnColumn18)
	ON_BN_CLICKED(IDC_OPT19, OnColumn19)
	ON_BN_CLICKED(IDC_OPT20, OnColumn20)
	ON_BN_CLICKED(IDC_OPT21, OnColumn21)
	ON_BN_CLICKED(IDC_OPT22, OnColumn22)
	ON_BN_CLICKED(IDC_OPT23, OnColumn23)
	ON_BN_CLICKED(IDC_OPT24, OnColumn24)
	ON_BN_CLICKED(IDC_OPT25, OnColumn25)
	ON_BN_CLICKED(IDC_OPT26, OnColumn26)
	ON_BN_CLICKED(IDC_OPT27, OnColumn27)
	ON_BN_CLICKED(IDC_OPT28, OnColumn28)
	ON_BN_CLICKED(IDC_OPT29, OnColumn29)
	ON_BN_CLICKED(IDC_OPT30, OnColumn30)
	ON_BN_CLICKED(IDC_OPT31, OnColumn31)
	ON_BN_CLICKED(IDC_OPT32, OnColumn32)
	ON_BN_CLICKED(IDC_CDATA, OnData)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_EN_CHANGE(IDC_TRGBE, OnChangeColor)
	ON_BN_CLICKED(IDC_SHIFTL, OnShiftLeft)
	ON_BN_CLICKED(IDC_SHIFTR, OnShiftRight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColumn message handlers

BOOL CColumn::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CEdit*	ed;
	CSpinButtonCtrl* spin;
	int	pRGB, tRGB;

	for (int ii = 0; ii < MAX_COLUMN; ii++)
	{
		if (m_common)
		{
			pRGB = m_repR.cellR[m_sel].pRGB;
			tRGB = m_repR.cellR[m_sel].tRGB;
		}
		else
		{
			pRGB = m_repR.cellR[ii].pRGB;
			tRGB = m_repR.cellR[ii].tRGB;
			if (m_repR.cellR[ii].pRGB == 0 && m_repR.cellR[ii].tRGB == 0)
			{
				pRGB = basPRGB;
				tRGB = 0;
			}
		}

		m_column[ii].Attach(IDC_OPT01+ii, this, m_palette, ii+1, pRGB, tRGB);
		m_column[ii].Invalidate();
		if (m_common)
			m_column[ii].EnableWindow(FALSE);
	}

	if (m_attr & GO_MULTI)
		m_multi = TRUE;

	ed = (CEdit *)GetDlgItem(IDC_LENE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_LENS);
	spin->SetRange(0, 255);
	spin->SetBuddy(ed);

	ed = (CEdit *)GetDlgItem(IDC_PRGBE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_PRGBS);
	spin->SetRange(0, 255);
	spin->SetBuddy(ed);

	ed = (CEdit *)GetDlgItem(IDC_TRGBE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_TRGBS);
	spin->SetRange(0, 255);
	spin->SetBuddy(ed);

	ed = (CEdit *)GetDlgItem(IDC_HPRGBE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_HPRGBS);
	spin->SetRange(0, 255);
	spin->SetBuddy(ed);

	ed = (CEdit *)GetDlgItem(IDC_HTRGBE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_HTRGBS);
	spin->SetRange(0, 255);
	spin->SetBuddy(ed);

	if (m_common)
		setColumn();
	else
		setColumn(IDC_OPT01, false);

	UpdateData(FALSE);
	m_first = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CColumn::OnInsert()
{
	saveColumn();
	for (int ii = MAX_COLUMN-1; ii > m_sel; ii--)
	{
		CopyMemory(&m_repR.cellR[ii], &m_repR.cellR[ii-1], sizeof(m_repR.cellR[ii-1]));
		m_column[ii].m_pRGB = m_column[ii-1].m_pRGB;
		m_column[ii].m_tRGB = m_column[ii-1].m_tRGB;
	}

	ii = MAX_COLUMN-1;
	ZeroMemory(&m_repR.cellR[m_sel], sizeof(m_repR.cellR[m_sel]));
	m_repR.cellR[m_sel].pRGB   = m_repR.cellR[ii].pRGB;
	m_repR.cellR[m_sel].tRGB   = m_repR.cellR[ii].tRGB;
	m_repR.cellR[m_sel].hpRGB  = m_repR.cellR[ii].hpRGB;
	m_repR.cellR[m_sel].htRGB  = m_repR.cellR[ii].htRGB;
	m_repR.cellR[m_sel].type   = IO_NUM;
	m_repR.cellR[m_sel].alignH = AL_CENTER;
	m_repR.cellR[m_sel].alignC = AL_RIGHT;
	m_repR.cellR[m_sel].attr   = FA_COMMA|FA_RIGHT;
	m_repR.cellR[m_sel].properties |= PR_VISIBLE;
	m_column[m_sel].m_pRGB = m_repR.cellR[m_sel].pRGB;
	m_column[m_sel].m_tRGB = m_repR.cellR[m_sel].tRGB;

	for (ii = 0; ii < MAX_COLUMN; ii++)
		m_column[ii].Invalidate();

	setColumn(IDC_OPT01+m_sel, false);
}

void CColumn::OnDelete() 
{
	for (int ii = m_sel; ii < MAX_COLUMN-1; ii++)
	{
		CopyMemory(&m_repR.cellR[ii], &m_repR.cellR[ii+1], sizeof(m_repR.cellR[ii+1]));
		m_column[ii].m_pRGB = m_column[ii+1].m_pRGB;
		m_column[ii].m_tRGB = m_column[ii+1].m_tRGB;
	}

	ZeroMemory(&m_repR.cellR[ii], sizeof(m_repR.cellR[ii]));
	m_repR.cellR[ii].pRGB   = m_repR.cellR[ii-1].pRGB;
	m_repR.cellR[ii].tRGB   = m_repR.cellR[ii-1].tRGB;
	m_repR.cellR[ii].hpRGB  = m_repR.cellR[ii-1].hpRGB;
	m_repR.cellR[ii].htRGB  = m_repR.cellR[ii-1].htRGB;
	m_repR.cellR[ii].type   = IO_NUM;
	m_repR.cellR[ii].alignH = AL_CENTER;
	m_repR.cellR[ii].alignC = AL_RIGHT;
	m_repR.cellR[ii].attr   = FA_COMMA|FA_RIGHT;
	m_repR.cellR[ii].properties |= PR_VISIBLE;
	m_column[ii].m_pRGB = m_repR.cellR[ii].pRGB;
	m_column[ii].m_tRGB = m_repR.cellR[ii].tRGB;

	for (ii = 0; ii < MAX_COLUMN; ii++)
		m_column[ii].Invalidate();

	setColumn(IDC_OPT01+m_sel, false);
}

void CColumn::OnChangeColor() 
{
	if (m_first)	return;

	UpdateData(TRUE);
	m_column[m_sel].m_pRGB = atoi(m_pRGB);
	m_column[m_sel].m_tRGB = atoi(m_tRGB);

	if (m_common)
	{
		for (int ii = 0; ii < MAX_COLUMN; ii++)
		{
			m_column[ii].m_pRGB = m_column[m_sel].m_pRGB;
			m_column[ii].m_tRGB = m_column[m_sel].m_tRGB;
			m_column[ii].Invalidate();
		}
	}
	else
		m_column[m_sel].Invalidate();
}

void CColumn::OnDominoCheck() 
{
	CButton* domino = (CButton *)GetDlgItem(IDC_DOMINOC);
	if (domino->GetCheck() == 1)
		GetDlgItem(IDC_DOMINO)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_DOMINO)->EnableWindow(FALSE);
}

void CColumn::OnReferenceCheck()
{
	CButton* ref = (CButton *)GetDlgItem(IDC_REFC);
	if (ref->GetCheck() == 1)
		GetDlgItem(IDC_REF)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_REF)->EnableWindow(FALSE);
}

void CColumn::OnEdit() 
{
	saveColumn();
	CCellEdit*	cellEdit;
	cellEdit = new CCellEdit(this, FM_GRID, m_repR.cellR[m_sel].type, m_repR.cellR[m_sel].isEdit,
				m_repR.cellR[m_sel].onEdit, m_repR.cellR[m_sel].editS,  m_repR.cellR[m_sel].name,
				m_repR.cellR[m_sel].attr,   m_repR.cellR[m_sel].size);
	if (cellEdit->DoModal() == IDOK)
	{
		m_repR.cellR[m_sel].attr   = cellEdit->m_attr;
		m_repR.cellR[m_sel].isEdit = cellEdit->m_isEdit;
		m_repR.cellR[m_sel].onEdit = cellEdit->m_onEdit;
		strcpy(m_repR.cellR[m_sel].editS, (char *)cellEdit->m_format.operator LPCTSTR());
	}
	delete cellEdit;
}

void CColumn::OnAttr() 
{
	saveColumn();
	CAttrDlg*	attrDlg;
	attrDlg = new CAttrDlg(this, FM_GRID, m_repR.cellR[m_sel].type,	m_repR.cellR[m_sel].iok,
				m_repR.cellR[m_sel].attr, m_repR.cellR[m_sel].attr2);
	if (attrDlg->DoModal() == IDOK)
	{
		m_repR.cellR[m_sel].attr  = attrDlg->m_attr;
		m_repR.cellR[m_sel].attr2 = attrDlg->m_attr2;
	}
	delete attrDlg;
}

void CColumn::OnData() 
{
	UpdateData(TRUE);
	m_repR.cellR[m_sel].size = min(255, atoi(m_len));
	CComboItem	cbItem(this, m_repR.cellR[m_sel].keys, m_repR.cellR[m_sel].size);
	CopyMemory(cbItem.m_disB, m_repR.cellR[m_sel].dat, sizeof(m_repR.cellR[m_sel].dat));
	CopyMemory(cbItem.m_datB, m_repR.cellR[m_sel].str, sizeof(m_repR.cellR[m_sel].str));
	if (cbItem.DoModal() == IDOK)
	{
		m_repR.cellR[m_sel].keys = cbItem.m_count;
		CopyMemory(m_repR.cellR[m_sel].dat, cbItem.m_disB, sizeof(m_repR.cellR[m_sel].dat));
		CopyMemory(m_repR.cellR[m_sel].str, cbItem.m_datB, sizeof(m_repR.cellR[m_sel].str));
		UpdateData(FALSE);
	}
}

void CColumn::OnHeadPRGB() 
{
	UpdateData(TRUE);
	m_repR.cellR[m_sel].hpRGB = atoi(m_hpRGB);
	CColorDlg* clDlg = new CColorDlg(this, m_repR.cellR[m_sel].hpRGB);
	if (clDlg->DoModal() == IDOK)
	{
		m_hpRGB.Format("%d", clDlg->m_color);
		UpdateData(FALSE);
	}

	delete clDlg;
}

void CColumn::OnHeadTRGB() 
{
	UpdateData(TRUE);
	m_repR.cellR[m_sel].htRGB = atoi(m_htRGB);
	CColorDlg* clDlg = new CColorDlg(this, m_repR.cellR[m_sel].htRGB);
	if (clDlg->DoModal() == IDOK)
	{
		m_htRGB.Format("%d", clDlg->m_color);
		UpdateData(FALSE);
	}

	delete clDlg;
}

void CColumn::OnPRGB() 
{
	UpdateData(TRUE);
	m_repR.cellR[m_sel].pRGB = atoi(m_pRGB);
	CColorDlg* clDlg = new CColorDlg(this, m_repR.cellR[m_sel].pRGB);
	if (clDlg->DoModal() == IDOK)
	{
		m_pRGB.Format("%d", clDlg->m_color);
		UpdateData(FALSE);
		m_column[m_sel].m_pRGB = clDlg->m_color;
		if (m_common)
		{
			for (int ii = 0; ii < MAX_COLUMN; ii++)
			{
				m_column[ii].m_pRGB = clDlg->m_color;
				m_column[ii].Invalidate();
			}
		}
		else
			m_column[m_sel].Invalidate();
	}

	delete clDlg;
}

void CColumn::OnTRGB() 
{
	UpdateData(TRUE);
	m_repR.cellR[m_sel].tRGB = atoi(m_tRGB);
	CColorDlg* clDlg = new CColorDlg(this, m_repR.cellR[m_sel].tRGB);
	if (clDlg->DoModal() == IDOK)
	{
		m_tRGB.Format("%d", clDlg->m_color);
		UpdateData(FALSE);
		m_column[m_sel].m_tRGB = clDlg->m_color;
		if (m_common)
		{
			for (int ii = 0; ii < MAX_COLUMN; ii++)
			{
				m_column[ii].m_tRGB = clDlg->m_color;
				m_column[ii].Invalidate();
			}
		}
		else
			m_column[m_sel].Invalidate();
	}

	delete clDlg;
}

void CColumn::OnSelChangeDataType() 
{
	UpdateData(TRUE);
	CComboBox* cb = (CComboBox *)GetDlgItem(IDC_TYPE);
	switch (cb->GetCurSel())
	{
	case 0:
	default:
		m_repR.cellR[m_sel].type = IO_NUM;
		if (!m_common)
			GetDlgItem(IDC_FLOAT)->EnableWindow(TRUE);
		return;
	case 1:
		m_repR.cellR[m_sel].type = IO_ALPHA;	break;
	case 2:
		m_repR.cellR[m_sel].type = IO_HAN;	break;
	case 3:
		m_repR.cellR[m_sel].type = IO_MIXED;	break;
	}
	GetDlgItem(IDC_FLOAT)->SetWindowText("");
	GetDlgItem(IDC_FLOAT)->EnableWindow(FALSE);
	m_repR.cellR[m_sel].attr &= ~(FA_COMMA|FA_RIGHT);
}

void CColumn::OnSelChangeKind()
{
	m_check = _T("");
	m_repR.cellR[m_sel].str[0] = '\0';
	CComboBox* cb = (CComboBox *)GetDlgItem(IDC_KIND);
	switch (cb->GetCurSel())
	{
	case 1:		// IE_COMBO
		m_repR.cellR[m_sel].kind = CK_COMBO;
		GetDlgItem(IDC_CDATA)->EnableWindow(TRUE);
		m_repR.cellR[m_sel].attr &= ~FA_REF;
		GetDlgItem(IDC_REF)->EnableWindow(FALSE);
		GetDlgItem(IDC_REFC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECKSTR)->EnableWindow(FALSE);
		break;
	case 2:		// IE_CHECK
		m_repR.cellR[m_sel].kind = CK_CHECK;
		m_repR.cellR[m_sel].attr &= ~FA_REF;
		GetDlgItem(IDC_CDATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_REF)->EnableWindow(FALSE);
		GetDlgItem(IDC_REFC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECKSTR)->EnableWindow(TRUE);
		break;
	default:
		m_repR.cellR[m_sel].isEdit = IE_NOP;
		GetDlgItem(IDC_CDATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_REF)->EnableWindow(TRUE);
		GetDlgItem(IDC_REFC)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECKSTR)->EnableWindow(FALSE);
		break;
	}
}

void CColumn::OnSelChangeMode()
{
	BOOL	enable = TRUE;

	CComboBox* cb = (CComboBox *)GetDlgItem(IDC_MODE);
	switch (cb->GetCurSel())
	{
	case 0:
		m_repR.cellR[m_sel].iok = EIO_INPUT;	break;
	case 1:
		m_repR.cellR[m_sel].iok = EIO_INOUT;	break;
	case 2:
	default:
		m_repR.cellR[m_sel].iok = EIO_OUTPUT;	break;
	}

	if (m_common)
		return;

	switch (m_repR.cellR[m_sel].iok)
	{
	case EIO_INPUT:
		GetDlgItem(IDC_KIND)->EnableWindow(TRUE);
		enable = FALSE;		break;
	case EIO_OUTPUT:
		GetDlgItem(IDC_KIND)->EnableWindow(FALSE);
		break;
	case EIO_INOUT:
		GetDlgItem(IDC_KIND)->EnableWindow(TRUE);
	case EIO_NOP:
	default:
		break;
	}
	GetDlgItem(IDC_DOMINOC)->EnableWindow(enable);
	GetDlgItem(IDC_DOMINO)->EnableWindow(enable);
	GetDlgItem(IDC_REFC)->EnableWindow(enable);
	GetDlgItem(IDC_REF)->EnableWindow(enable);
}

void CColumn::OnColumn01()	{ setColumn(IDC_OPT01); }
void CColumn::OnColumn02()	{ setColumn(IDC_OPT02); }
void CColumn::OnColumn03()	{ setColumn(IDC_OPT03); }
void CColumn::OnColumn04()	{ setColumn(IDC_OPT04); }
void CColumn::OnColumn05()	{ setColumn(IDC_OPT05); }
void CColumn::OnColumn06()	{ setColumn(IDC_OPT06); }
void CColumn::OnColumn07()	{ setColumn(IDC_OPT07); }
void CColumn::OnColumn08()	{ setColumn(IDC_OPT08); }
void CColumn::OnColumn09()	{ setColumn(IDC_OPT09); }
void CColumn::OnColumn10()	{ setColumn(IDC_OPT10); }
void CColumn::OnColumn11()	{ setColumn(IDC_OPT11); }
void CColumn::OnColumn12()	{ setColumn(IDC_OPT12); }
void CColumn::OnColumn13()	{ setColumn(IDC_OPT13); }
void CColumn::OnColumn14()	{ setColumn(IDC_OPT14); }
void CColumn::OnColumn15()	{ setColumn(IDC_OPT15); }
void CColumn::OnColumn16()	{ setColumn(IDC_OPT16); }
void CColumn::OnColumn17()	{ setColumn(IDC_OPT17); }
void CColumn::OnColumn18()	{ setColumn(IDC_OPT18); }
void CColumn::OnColumn19()	{ setColumn(IDC_OPT19); }
void CColumn::OnColumn20()	{ setColumn(IDC_OPT20); }
void CColumn::OnColumn21()	{ setColumn(IDC_OPT21); }
void CColumn::OnColumn22()	{ setColumn(IDC_OPT22); }
void CColumn::OnColumn23()	{ setColumn(IDC_OPT23); }
void CColumn::OnColumn24()	{ setColumn(IDC_OPT24); }
void CColumn::OnColumn25()	{ setColumn(IDC_OPT25); }
void CColumn::OnColumn26()	{ setColumn(IDC_OPT26); }
void CColumn::OnColumn27()	{ setColumn(IDC_OPT27); }
void CColumn::OnColumn28()	{ setColumn(IDC_OPT28); }
void CColumn::OnColumn29()	{ setColumn(IDC_OPT29); }
void CColumn::OnColumn30()	{ setColumn(IDC_OPT30); }
void CColumn::OnColumn31()	{ setColumn(IDC_OPT31); }
void CColumn::OnColumn32()	{ setColumn(IDC_OPT32); }

void CColumn::setColumn(int id, bool save)
{
	if (m_common)
		return;

	CString	tmps;
	CComboBox* cb;
	int	sel;

	tmps.Format("COLUMN %d", id-IDC_OPT01+1);
	GetDlgItem(IDC_NUMBER)->SetWindowText(tmps);
	
	if (save)
		saveColumn();

	sel   = m_sel;
	m_sel = id - IDC_OPT01;
	m_symbol = CString(m_repR.cellR[m_sel].name);
	if (m_attr & GO_XHEAD)
	{
		GetDlgItem(IDC_STR)->SetWindowText("");
		GetDlgItem(IDC_STR)->EnableWindow(FALSE);
	}
	else
		m_head = CString(m_repR.cellR[m_sel].head);

	cb = (CComboBox *)GetDlgItem(IDC_TYPE);
	switch (m_repR.cellR[m_sel].type)
	{
	case IO_ALPHA:
		cb->SetCurSel(1);	break;
	case IO_HAN:
		cb->SetCurSel(2);	break;
	case IO_MIXED:
		cb->SetCurSel(3);	break;
	case IO_NUM:
	default:
		cb->SetCurSel(0);	break;
	}

	cb = (CComboBox *)GetDlgItem(IDC_ALIGNH);
	switch (m_repR.cellR[m_sel].alignH)
	{
	case AL_LEFT:
		cb->SetCurSel(1);	break;
	case AL_RIGHT:
		cb->SetCurSel(2);	break;
	case AL_CENTER:
	default:
		cb->SetCurSel(0);	break;
	}
	
	cb = (CComboBox *)GetDlgItem(IDC_ALIGND);
	switch (m_repR.cellR[m_sel].alignC)
	{
	case AL_CENTER:
		cb->SetCurSel(0);	break;
	case AL_LEFT:
		cb->SetCurSel(1);	break;
	case AL_RIGHT:
	default:
		cb->SetCurSel(2);	break;
	}
	
	cb = (CComboBox *)GetDlgItem(IDC_MODE);
	switch (m_repR.cellR[m_sel].iok)
	{
	case EIO_INPUT:
		cb->SetCurSel(0);	break;
	case EIO_INOUT:
		cb->SetCurSel(1);	break;
	case EIO_OUTPUT:
	default:
		cb->SetCurSel(2);	break;
	}

	cb = (CComboBox *)GetDlgItem(IDC_KIND);
	switch (m_repR.cellR[m_sel].kind)
	{
	case CK_COMBO:
		cb->SetCurSel(1);
		GetDlgItem(IDC_CDATA)->EnableWindow(TRUE);
		GetDlgItem(IDC_REF)->EnableWindow(FALSE);
		GetDlgItem(IDC_REFC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECKSTR)->EnableWindow(FALSE);
		break;
	case CK_CHECK:
		cb->SetCurSel(2);
		GetDlgItem(IDC_CDATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_REF)->EnableWindow(FALSE);
		GetDlgItem(IDC_REFC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECKSTR)->EnableWindow(TRUE);
		m_check = CString(m_repR.cellR[m_sel].str);
		break;
	default:
		cb->SetCurSel(0);
		GetDlgItem(IDC_CDATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_REF)->EnableWindow(TRUE);
		GetDlgItem(IDC_REFC)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECKSTR)->EnableWindow(FALSE);
		break;
	}

	CButton* check = (CButton *) GetDlgItem(IDC_DOMINOC);
	if (m_repR.cellR[m_sel].iok == EIO_INOUT || m_repR.cellR[m_sel].iok == EIO_OUTPUT)
	{
		check->EnableWindow(TRUE);
		if (m_repR.cellR[m_sel].attr & FA_DOMINO)
		{
			check->SetCheck(1);
			GetDlgItem(IDC_DOMINO)->EnableWindow(TRUE);
			m_domino = CString(m_repR.cellR[m_sel].auxS);
		}
		else
		{
			check->SetCheck(0);
			m_domino = _T("");
			GetDlgItem(IDC_DOMINO)->EnableWindow(FALSE);
		}
	}
	else
	{
		check->EnableWindow(FALSE);
		m_domino = _T("");
		GetDlgItem(IDC_DOMINO)->EnableWindow(FALSE);
	}

	check = (CButton *) GetDlgItem(IDC_REFC);
	check->EnableWindow(TRUE);
	if (m_repR.cellR[m_sel].attr & FA_REF)
	{
		check->SetCheck(1);
		GetDlgItem(IDC_REF)->EnableWindow(TRUE);
		m_reference = CString(m_repR.cellR[m_sel].refS);
	}
	else
	{
		check->SetCheck(0);
		m_reference = _T("");
		GetDlgItem(IDC_REF)->EnableWindow(FALSE);
	}

	m_len.Format("%d", m_repR.cellR[m_sel].size);
	m_pRGB.Format("%d", m_repR.cellR[m_sel].pRGB);
	m_tRGB.Format("%d", m_repR.cellR[m_sel].tRGB);
	m_hpRGB.Format("%d", m_repR.cellR[m_sel].hpRGB);
	m_htRGB.Format("%d", m_repR.cellR[m_sel].htRGB);

	m_visible = (m_repR.cellR[m_sel].properties & PR_VISIBLE) ? TRUE : FALSE;
	m_sort    = (m_repR.cellR[m_sel].properties & PR_SORT)    ? TRUE : FALSE;
	m_cut     = (m_repR.cellR[m_sel].attr & FA_ENUM)          ? TRUE : FALSE;

	m_width.Format("%d", m_repR.cellR[m_sel].width);
	if (m_repR.cellR[m_sel].type == IO_NUM)
	{
		GetDlgItem(IDC_FLOAT)->EnableWindow(TRUE);
		m_float.Format("%d", m_repR.cellR[m_sel].vals);
	}
	else
	{
		GetDlgItem(IDC_FLOAT)->EnableWindow(FALSE);
		m_float.Empty();
	}

	if (sel != m_sel || m_first)
	{
		m_column[sel].SetFocus(false);
		m_column[m_sel].SetFocus();
	}

	UpdateData(FALSE);
	GetDlgItem(IDC_SYMBOL)->SetFocus();
}

void CColumn::setColumn()
{
	CComboBox* cb;

	m_len.Format("%d", m_repR.cellR[m_sel].size);
	m_pRGB.Format("%d", m_repR.cellR[m_sel].pRGB);
	m_tRGB.Format("%d", m_repR.cellR[m_sel].tRGB);
	m_hpRGB.Format("%d", m_repR.cellR[m_sel].hpRGB);
	m_htRGB.Format("%d", m_repR.cellR[m_sel].htRGB);
	m_width.Format("%d", m_repR.cellR[m_sel].width);

	cb = (CComboBox *)GetDlgItem(IDC_TYPE);
	switch (m_repR.cellR[m_sel].type)
	{
	case IO_ALPHA:
		cb->SetCurSel(1);	break;
	case IO_HAN:
		cb->SetCurSel(2);	break;
	case IO_MIXED:
		cb->SetCurSel(3);	break;
	case IO_NUM:
	default:
		cb->SetCurSel(0);	break;
	}

	cb = (CComboBox *)GetDlgItem(IDC_ALIGNH);
	switch (m_repR.cellR[m_sel].alignH)
	{
	case AL_LEFT:
		cb->SetCurSel(1);	break;
	case AL_RIGHT:
		cb->SetCurSel(2);	break;
	case AL_CENTER:
	default:
		cb->SetCurSel(0);	break;
	}

	cb = (CComboBox *)GetDlgItem(IDC_ALIGND);
	switch (m_repR.cellR[m_sel].alignC)
	{
	case AL_CENTER:
		cb->SetCurSel(0);	break;
	case AL_LEFT:
		cb->SetCurSel(1);	break;
	case AL_RIGHT:
	default:
		cb->SetCurSel(2);	break;
	}

	cb = (CComboBox *)GetDlgItem(IDC_MODE);
	switch (m_repR.cellR[m_sel].iok)
	{
	case EIO_INPUT:
		cb->SetCurSel(0);	break;
	case EIO_INOUT:
		cb->SetCurSel(1);	break;
	case EIO_OUTPUT:
	default:
		cb->SetCurSel(2);	break;
	}

	GetDlgItem(IDC_SYMBOL)->EnableWindow(FALSE);
	GetDlgItem(IDC_STR)->EnableWindow(FALSE);
	GetDlgItem(IDC_KIND)->EnableWindow(FALSE);
	GetDlgItem(IDC_CDATA)->EnableWindow(FALSE);
	GetDlgItem(IDC_DOMINOC)->EnableWindow(FALSE);
	GetDlgItem(IDC_DOMINO)->EnableWindow(FALSE);
	GetDlgItem(IDC_REFC)->EnableWindow(FALSE);
	GetDlgItem(IDC_REF)->EnableWindow(FALSE);
	GetDlgItem(IDC_VISIBLE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_FLOAT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECKSTR)->EnableWindow(FALSE);
	GetDlgItem(IDC_DATA)->EnableWindow(FALSE);
	GetDlgItem(IDC_MULTI)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSERT)->EnableWindow(FALSE);
	GetDlgItem(IDC_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SHIFTL)->EnableWindow(FALSE);
	GetDlgItem(IDC_SHIFTR)->EnableWindow(FALSE);
}

void CColumn::saveColumn()
{
	CComboBox* cb;

	UpdateData(TRUE);
	cb = (CComboBox *)GetDlgItem(IDC_TYPE);
	switch (cb->GetCurSel())
	{
	case 1:
		m_repR.cellR[m_sel].type = IO_ALPHA;	break;
	case 2:
		m_repR.cellR[m_sel].type = IO_HAN;	break;
	case 3:
		m_repR.cellR[m_sel].type = IO_MIXED;	break;
	case 0:
	default:
		m_repR.cellR[m_sel].type = IO_NUM;	break;
	}
	m_repR.cellR[m_sel].size = min(255, atoi(m_len));
	m_repR.cellR[m_sel].pRGB = atoi(m_pRGB);
	m_repR.cellR[m_sel].tRGB = atoi(m_tRGB);
	m_repR.cellR[m_sel].hpRGB = atoi(m_hpRGB);
	m_repR.cellR[m_sel].htRGB = atoi(m_htRGB);
	m_repR.cellR[m_sel].width = atoi(m_width);

	cb = (CComboBox *)GetDlgItem(IDC_ALIGNH);
	switch (cb->GetCurSel())
	{
	case 1:
		m_repR.cellR[m_sel].alignH = AL_LEFT;		break;
	case 2:
		m_repR.cellR[m_sel].alignH = AL_RIGHT;		break;
	case 0:
	default:
		m_repR.cellR[m_sel].alignH = AL_CENTER;		break;
	}

	cb = (CComboBox *)GetDlgItem(IDC_ALIGND);
	switch (cb->GetCurSel())
	{
	case 0:
		m_repR.cellR[m_sel].alignC = AL_CENTER;		break;
	case 1:
		m_repR.cellR[m_sel].alignC = AL_LEFT;		break;
	case 2:
	default:
		m_repR.cellR[m_sel].alignC = AL_RIGHT;		break;
	}
	
	cb = (CComboBox *)GetDlgItem(IDC_MODE);
	switch (cb->GetCurSel())
	{
	case 0:
		m_repR.cellR[m_sel].iok = EIO_INPUT;		break;
	case 1:
		m_repR.cellR[m_sel].iok = EIO_INOUT;		break;
	case 2:
	default:
		m_repR.cellR[m_sel].iok = EIO_OUTPUT;		break;
		break;
	}

	if (m_common)
		return;

	strcpy(m_repR.cellR[m_sel].name, m_symbol);
	cb = (CComboBox *)GetDlgItem(IDC_KIND);
	switch (cb->GetCurSel())
	{
	case 1:
		m_repR.cellR[m_sel].kind = CK_COMBO;	break;
	case 2:
		m_repR.cellR[m_sel].kind = CK_CHECK;
		strcpy(m_repR.cellR[m_sel].str, m_check);
		break;
	default:
		break;
	}

	m_repR.cellR[m_sel].vals = atoi(m_float);
	if (m_repR.cellR[m_sel].vals != 0)
		m_repR.cellR[m_sel].attr |= FA_FLOAT;
	else
		m_repR.cellR[m_sel].attr &= ~FA_FLOAT;

	if (!(m_attr & GO_XHEAD))
		strcpy(m_repR.cellR[m_sel].head, m_head);

	int	offs;
	m_repR.cellR[m_sel].headN = 1;
	for (;;)
	{
		offs = m_head.Find('~');
		if (offs != -1)
		{
			m_repR.cellR[m_sel].headN++;
			m_head = m_head.Mid(offs+1);
		}
		break;
	}

	if (m_visible)
		m_repR.cellR[m_sel].properties |= PR_VISIBLE;
	else
		m_repR.cellR[m_sel].properties &= ~PR_VISIBLE;

	if (m_sort)
		m_repR.cellR[m_sel].properties |= PR_SORT;
	else
		m_repR.cellR[m_sel].properties &= ~PR_SORT;

	if (m_cut)
		m_repR.cellR[m_sel].attr |= FA_ENUM;
	else
		m_repR.cellR[m_sel].attr &= ~FA_ENUM;

	CButton* check = (CButton *)GetDlgItem(IDC_DOMINOC);
	if (check->GetCheck() == 1)
	{
		m_repR.cellR[m_sel].attr |= FA_DOMINO;
		strcpy(m_repR.cellR[m_sel].auxS, m_domino);
	}
	else
	{
		m_repR.cellR[m_sel].attr &= ~FA_DOMINO;
		m_repR.cellR[m_sel].auxS[0] = '\0';
	}

	check = (CButton *)GetDlgItem(IDC_REFC);
	if (check->GetCheck() == 1)
	{
		m_repR.cellR[m_sel].attr |= FA_REF;
		strcpy(m_repR.cellR[m_sel].refS, m_reference);
	}
	else
	{
		m_repR.cellR[m_sel].attr &= ~FA_REF;
		m_repR.cellR[m_sel].refS[0] = '\0';
	}
}

void CColumn::OnOK() 
{
	OnApply();
	CDialog::OnOK();
}

void CColumn::OnApply() 
{
	saveColumn();
	if (m_common)
	{
		CCellItem *cellItem = new CCellItem(&m_repR), *cellItemS = new CCellItem(m_pRepR);
		struct _cellR *cellR = cellItem->getCellPoint(m_sel),
			*cellRS = cellItemS->getCellPoint(m_sel);

		if (cellR->type != cellRS->type)
			applyCommonColumn(CPR_TYPE);

		if (cellR->iok != cellRS->iok)
			applyCommonColumn(CPR_MODE);

		if (cellR->size != cellRS->size)
			applyCommonColumn(CPR_SIZE);

		if (cellR->width != cellRS->width)
			applyCommonColumn(CPR_WIDTH);

		if (cellR->pRGB != cellRS->pRGB)
			applyCommonColumn(CPR_PRGB);

		if (cellR->tRGB != cellRS->tRGB)
			applyCommonColumn(CPR_TRGB);

		if (cellR->hpRGB != cellRS->hpRGB)
			applyCommonColumn(CPR_HPRGB);

		if (cellR->htRGB != cellRS->htRGB)
			applyCommonColumn(CPR_HTRGB);

		if (cellR->attr != cellRS->attr
			|| cellR->attr2 != cellRS->attr2)
			applyCommonColumn(CPR_ATTR);

		if (cellR->alignH != cellRS->alignH)
			applyCommonColumn(CPR_ALIGNH);

		if (cellR->alignC != cellRS->alignC)
			applyCommonColumn(CPR_ALIGNC);

		CopyMemory(m_pRepR, &m_repR, sizeof(_repR));
		delete cellItem;
		delete cellItemS;
		m_parent->SendMessage(WM_CELL_PAINT);
		return;
	}

	if (m_multi)
		m_attr |= GO_MULTI;
	else
		m_attr &= ~GO_MULTI;

	int	line, colN;
	line = colN = 0;
	for (int ii = 0; ii < MAX_COLUMN; ii++)
	{
		if (m_repR.cellR[ii].name[0] == '\0')
		{
			for (int jj = ii; jj < MAX_COLUMN; jj++)
			{
				if (m_repR.cellR[jj].name[0] != '\0')
				{
					CopyMemory(&m_repR.cellR[ii], &m_repR.cellR[jj], sizeof(m_repR.cellR[jj]));
					colN++;
					ZeroMemory(&m_repR.cellR[jj], sizeof(m_repR.cellR[jj]));
					m_repR.cellR[ii].lineN = (BYTE)line;
					if (m_attr & GO_MULTI)
					{
						if (m_repR.cellR[ii].attr & FA_WRAP)
							line++;
					}
					break;
				}
			}
		}
		else
		{
			colN++;
			m_repR.cellR[ii].lineN = (BYTE)line;
			if (m_attr & GO_MULTI)
			{
				if (m_repR.cellR[ii].attr & FA_WRAP)
					line++;
			}
		}
	}
	m_repR.cols = (BYTE) colN;
	for (ii = colN; ii < MAX_COLUMN; ii++)
	{
		ZeroMemory(&m_repR.cellR[ii], sizeof(m_repR.cellR[ii]));
		m_repR.cellR[ii].pRGB   = m_repR.cellR[colN-1].pRGB;
		m_repR.cellR[ii].tRGB   = m_repR.cellR[colN-1].tRGB;
		m_repR.cellR[ii].hpRGB  = m_repR.cellR[colN-1].hpRGB;
		m_repR.cellR[ii].htRGB  = m_repR.cellR[colN-1].htRGB;
		m_repR.cellR[ii].type   = IO_NUM;
		m_repR.cellR[ii].alignH = AL_CENTER;
		m_repR.cellR[ii].alignC = AL_RIGHT;
		m_repR.cellR[ii].attr   = FA_COMMA|FA_RIGHT;
		m_repR.cellR[ii].properties |= PR_VISIBLE;

		m_column[ii].m_pRGB     = m_repR.cellR[ii].pRGB;
		m_column[ii].m_tRGB     = m_repR.cellR[ii].tRGB;
		m_column[ii].Invalidate();
	}

	CopyMemory(m_pRepR, &m_repR, sizeof(_repR));
	m_parent->SendMessage(WM_CELL_PAINT);
}

void CColumn::applyCommonColumn(int prop)
{
	for (int ii = 0; ii < m_repR.cols; ii++)
	{
		switch (prop)
		{
		case CPR_TYPE:
			m_repR.cellR[ii].type = m_repR.cellR[m_sel].type;	
			break;

		case CPR_MODE:
			m_repR.cellR[ii].iok = m_repR.cellR[m_sel].iok;
			break;

		case CPR_SIZE:
			m_repR.cellR[ii].size = m_repR.cellR[m_sel].size;
			break;

		case CPR_WIDTH:
			m_repR.cellR[ii].width = m_repR.cellR[m_sel].width;
			break;

		case CPR_ALIGNH:
			m_repR.cellR[ii].alignH = m_repR.cellR[m_sel].alignH;
			break;

		case CPR_ALIGNC:
			m_repR.cellR[ii].alignC = m_repR.cellR[m_sel].alignC;
			break;

		case CPR_HTRGB:
			m_repR.cellR[ii].htRGB = m_repR.cellR[m_sel].htRGB;
			break;

		case CPR_HPRGB:
			m_repR.cellR[ii].hpRGB = m_repR.cellR[m_sel].hpRGB;
			break;

		case CPR_TRGB:
			m_repR.cellR[ii].tRGB = m_repR.cellR[m_sel].tRGB;
			break;

		case CPR_PRGB:
			m_repR.cellR[ii].pRGB = m_repR.cellR[m_sel].pRGB;
			break;

		case CPR_ATTR:
			m_repR.cellR[ii].attr  = m_repR.cellR[m_sel].attr;
			m_repR.cellR[ii].attr2 = m_repR.cellR[m_sel].attr2;
			break;

		default:
			break;
		}
	}
}

void CColumn::OnShiftLeft() 
{
	int	sel = m_sel;
	struct	_cellR	cellR;
	
	if (m_sel <= 0)
		return;

	m_sel--;
	CopyMemory(&cellR, &m_repR.cellR[m_sel], sizeof(_cellR));
	CopyMemory(&m_repR.cellR[m_sel], &m_repR.cellR[sel], sizeof(_cellR));
	CopyMemory(&m_repR.cellR[sel], &cellR, sizeof(_cellR));

	m_column[sel].m_pRGB   = m_repR.cellR[sel].pRGB;
	m_column[sel].m_tRGB   = m_repR.cellR[sel].tRGB;
	m_column[m_sel].m_pRGB = m_repR.cellR[m_sel].pRGB;
	m_column[m_sel].m_tRGB = m_repR.cellR[m_sel].tRGB;
	m_column[sel].SetFocus(false);
	m_column[m_sel].SetFocus();

	setColumn(IDC_OPT01+m_sel, false);
}

void CColumn::OnShiftRight() 
{
	int	sel = m_sel;
	struct	_cellR	cellR;

	if (m_sel >= MAX_COLUMN-1)
		return;

	m_sel++;
	CopyMemory(&cellR, &m_repR.cellR[m_sel], sizeof(_cellR));
	CopyMemory(&m_repR.cellR[m_sel], &m_repR.cellR[sel], sizeof(_cellR));
	CopyMemory(&m_repR.cellR[sel], &cellR, sizeof(_cellR));

	m_column[sel].m_pRGB   = m_repR.cellR[sel].pRGB;
	m_column[sel].m_tRGB   = m_repR.cellR[sel].tRGB;
	m_column[m_sel].m_pRGB = m_repR.cellR[m_sel].pRGB;
	m_column[m_sel].m_tRGB = m_repR.cellR[m_sel].tRGB;
	m_column[sel].SetFocus(false);
	m_column[m_sel].SetFocus();

	setColumn(IDC_OPT01+m_sel, false);
}
