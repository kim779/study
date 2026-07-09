// row.cpp : implementation file
//

#include "stdafx.h"
#include "row.h"
#include "attrdlg.h"
#include "celledit.h"

#include "../../dll/lib/axislib.h"
#include "../awCommon/colordlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	RPR_TYPE	0
#define	RPR_SIZE	1
#define	RPR_ALIGNH	2
#define	RPR_ALIGNC	3
#define	RPR_HTRGB	4
#define	RPR_HPRGB	5
#define	RPR_TRGB	6
#define	RPR_PRGB	7
#define	RPR_ATTR	8

/////////////////////////////////////////////////////////////////////////////
// CRow dialog

CRow::CRow(CWnd* pParent, struct _repR* repR, class CAxisPalette* pal, DWORD attr, bool common)
	: CDialog(CRow::IDD, pParent)
{
	m_common  = common;
	m_pRepR   = repR;
	m_palette = pal;
	m_attr    = attr;
	m_parent  = pParent;

	m_sel   = 0;
	m_first = true;
	CopyMemory(&m_repR, m_pRepR, sizeof(_repR));

	//{{AFX_DATA_INIT(CRow)
	m_float = _T("");
	m_hpRGB = _T("");
	m_htRGB = _T("");
	m_len = _T("");
	m_pRGB = _T("");
	m_head = _T("");
	m_symbol = _T("");
	m_tRGB = _T("");
	m_visible = FALSE;
	m_domino = _T("");
	m_reference = _T("");
	//}}AFX_DATA_INIT
}


void CRow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRow)
	DDX_Text(pDX, IDC_FLOAT, m_float);
	DDV_MaxChars(pDX, m_float, 2);
	DDX_Text(pDX, IDC_HPRGBE, m_hpRGB);
	DDV_MaxChars(pDX, m_hpRGB, 8);
	DDX_Text(pDX, IDC_HTRGBE, m_htRGB);
	DDV_MaxChars(pDX, m_htRGB, 8);
	DDX_Text(pDX, IDC_LENE, m_len);
	DDV_MaxChars(pDX, m_len, 3);
	DDX_Text(pDX, IDC_PRGBE, m_pRGB);
	DDV_MaxChars(pDX, m_pRGB, 8);
	DDX_Text(pDX, IDC_STR, m_head);
	DDV_MaxChars(pDX, m_head, 64);
	DDX_Text(pDX, IDC_SYMBOL, m_symbol);
	DDV_MaxChars(pDX, m_symbol, 16);
	DDX_Text(pDX, IDC_TRGBE, m_tRGB);
	DDV_MaxChars(pDX, m_tRGB, 8);
	DDX_Check(pDX, IDC_VISIBLE, m_visible);
	DDX_Text(pDX, IDC_DOMINO, m_domino);
	DDV_MaxChars(pDX, m_domino, 16);
	DDX_Text(pDX, IDC_REF, m_reference);
	DDV_MaxChars(pDX, m_reference, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRow, CDialog)
	//{{AFX_MSG_MAP(CRow)
	ON_BN_CLICKED(IDC_INSERT, OnInsert)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_EN_CHANGE(IDC_PRGBE, OnChangeColor)
	ON_BN_CLICKED(IDC_DATA, OnEdit)
	ON_BN_CLICKED(IDC_ATTR, OnAttr)
	ON_BN_CLICKED(IDC_HPRGB, OnHPRGB)
	ON_BN_CLICKED(IDC_HTRGB, OnHTRGB)
	ON_BN_CLICKED(IDC_PRGB, OnPRGB)
	ON_BN_CLICKED(IDC_TRGB, OnTRGB)
	ON_CBN_SELCHANGE(IDC_TYPE, OnSelChangeDataType)
	ON_BN_CLICKED(IDC_OPT01, OnRow01)
	ON_BN_CLICKED(IDC_OPT02, OnRow02)
	ON_BN_CLICKED(IDC_OPT03, OnRow03)
	ON_BN_CLICKED(IDC_OPT04, OnRow04)
	ON_BN_CLICKED(IDC_OPT05, OnRow05)
	ON_BN_CLICKED(IDC_OPT06, OnRow06)
	ON_BN_CLICKED(IDC_OPT07, OnRow07)
	ON_BN_CLICKED(IDC_OPT08, OnRow08)
	ON_BN_CLICKED(IDC_OPT09, OnRow09)
	ON_BN_CLICKED(IDC_OPT10, OnRow10)
	ON_BN_CLICKED(IDC_OPT11, OnRow11)
	ON_BN_CLICKED(IDC_OPT12, OnRow12)
	ON_BN_CLICKED(IDC_OPT13, OnRow13)
	ON_BN_CLICKED(IDC_OPT14, OnRow14)
	ON_BN_CLICKED(IDC_OPT15, OnRow15)
	ON_BN_CLICKED(IDC_OPT16, OnRow16)
	ON_BN_CLICKED(IDC_OPT17, OnRow17)
	ON_BN_CLICKED(IDC_OPT18, OnRow18)
	ON_BN_CLICKED(IDC_OPT19, OnRow19)
	ON_BN_CLICKED(IDC_OPT20, OnRow20)
	ON_BN_CLICKED(IDC_OPT21, OnRow21)
	ON_BN_CLICKED(IDC_OPT22, OnRow22)
	ON_BN_CLICKED(IDC_OPT23, OnRow23)
	ON_BN_CLICKED(IDC_OPT24, OnRow24)
	ON_BN_CLICKED(IDC_OPT25, OnRow25)
	ON_BN_CLICKED(IDC_OPT26, OnRow26)
	ON_BN_CLICKED(IDC_OPT27, OnRow27)
	ON_BN_CLICKED(IDC_OPT28, OnRow28)
	ON_BN_CLICKED(IDC_OPT29, OnRow29)
	ON_BN_CLICKED(IDC_OPT30, OnRow30)
	ON_BN_CLICKED(IDC_OPT31, OnRow31)
	ON_BN_CLICKED(IDC_OPT32, OnRow32)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_DOMINOC, OnDominoCheck)
	ON_BN_CLICKED(IDC_REFC, OnReferenceCheck)
	ON_EN_CHANGE(IDC_TRGBE, OnChangeColor)
	ON_BN_CLICKED(IDC_SHIFTL, OnShiftLeft)
	ON_BN_CLICKED(IDC_SHIFTR, OnShiftRight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRow message handlers

BOOL CRow::OnInitDialog() 
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

		m_row[ii].Attach(IDC_OPT01+ii, this, m_palette, ii+1, pRGB, tRGB);
		m_row[ii].Invalidate();
		if (m_common)
			m_row[ii].EnableWindow(FALSE);
	}


	ed = (CEdit *)GetDlgItem(IDC_LENE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_LENS);
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

	ed = (CEdit *)GetDlgItem(IDC_PRGBE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_PRGBS);
	spin->SetRange(0, 255);
	spin->SetBuddy(ed);

	ed = (CEdit *)GetDlgItem(IDC_TRGBE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_TRGBS);
	spin->SetRange(0, 255);
	spin->SetBuddy(ed);

	if (m_common)
		setRow();
	else
		setRow(IDC_OPT01, false);

	UpdateData(FALSE);
	m_first = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRow::OnInsert() 
{
	saveRow();
	for (int ii = MAX_COLUMN-1; ii > m_sel; ii--)
	{
		CopyMemory(&m_repR.cellR[ii], &m_repR.cellR[ii-1], sizeof(m_repR.cellR[ii-1]));
		m_row[ii].m_pRGB = m_row[ii-1].m_pRGB;
		m_row[ii].m_tRGB = m_row[ii-1].m_tRGB;
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
	m_row[m_sel].m_pRGB = m_repR.cellR[m_sel].pRGB;
	m_row[m_sel].m_tRGB = m_repR.cellR[m_sel].tRGB;

	for (ii = 0; ii < MAX_COLUMN; ii++)
		m_row[ii].Invalidate();

	setRow(IDC_OPT01+m_sel, false);
}

void CRow::OnDelete() 
{
	for (int ii = m_sel; ii < MAX_COLUMN-1; ii++)
	{
		CopyMemory(&m_repR.cellR[ii], &m_repR.cellR[ii+1], sizeof(m_repR.cellR[ii+1]));
		m_row[ii].m_pRGB = m_row[ii+1].m_pRGB;
		m_row[ii].m_tRGB = m_row[ii+1].m_tRGB;
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
	m_row[ii].m_pRGB = m_repR.cellR[ii].pRGB;
	m_row[ii].m_tRGB = m_repR.cellR[ii].tRGB;

	for (ii = 0; ii < MAX_COLUMN; ii++)
		m_row[ii].Invalidate();

	setRow(IDC_OPT01+m_sel, false);
}

void CRow::OnChangeColor() 
{
	if (m_first)	return;

	UpdateData(TRUE);
	m_row[m_sel].m_pRGB = atoi(m_pRGB);
	m_row[m_sel].m_tRGB = atoi(m_tRGB);

	if (m_common)
	{
		for (int ii = 0; ii < MAX_COLUMN; ii++)
		{
			m_row[ii].m_pRGB = m_row[m_sel].m_pRGB;
			m_row[ii].m_tRGB = m_row[m_sel].m_tRGB;
			m_row[ii].Invalidate();
		}
	}
	else
		m_row[m_sel].Invalidate();
}

void CRow::OnEdit()
{
	saveRow();
	CCellEdit*	cellEdit;
	cellEdit = new CCellEdit(this, FM_TABLE, m_repR.cellR[m_sel].type, m_repR.cellR[m_sel].isEdit,
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

void CRow::OnAttr() 
{
	saveRow();
	CAttrDlg*	attrDlg;
	attrDlg = new CAttrDlg(this, FM_TABLE, m_repR.cellR[m_sel].type, m_repR.cellR[m_sel].iok,
				m_repR.cellR[m_sel].attr, m_repR.cellR[m_sel].attr2);
	if (attrDlg->DoModal() == IDOK)
	{
		m_repR.cellR[m_sel].attr  = attrDlg->m_attr;
		m_repR.cellR[m_sel].attr2 = attrDlg->m_attr2;
	}
	delete attrDlg;
}

void CRow::OnHPRGB() 
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

void CRow::OnHTRGB() 
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

void CRow::OnPRGB() 
{
	UpdateData(TRUE);
	m_repR.cellR[m_sel].pRGB = atoi(m_pRGB);
	CColorDlg* clDlg = new CColorDlg(this, m_repR.cellR[m_sel].pRGB);
	if (clDlg->DoModal() == IDOK)
	{
		m_pRGB.Format("%d", clDlg->m_color);
		UpdateData(FALSE);
		m_row[m_sel].m_pRGB = clDlg->m_color;
		if (m_common)
		{
			for (int ii = 0; ii < MAX_COLUMN; ii++)
			{
				m_row[ii].m_pRGB = clDlg->m_color;
				m_row[ii].Invalidate();
			}
		}
		else
			m_row[m_sel].Invalidate();
	}

	delete clDlg;
}

void CRow::OnTRGB() 
{
	UpdateData(TRUE);
	m_repR.cellR[m_sel].tRGB = atoi(m_tRGB);
	CColorDlg* clDlg = new CColorDlg(this, m_repR.cellR[m_sel].tRGB);
	if (clDlg->DoModal() == IDOK)
	{
		m_tRGB.Format("%d", clDlg->m_color);
		UpdateData(FALSE);
		m_row[m_sel].m_tRGB = clDlg->m_color;
		if (m_common)
		{
			for (int ii = 0; ii < MAX_COLUMN; ii++)
			{
				m_row[ii].m_tRGB = clDlg->m_color;
				m_row[ii].Invalidate();
			}
		}
		else
			m_row[m_sel].Invalidate();
	}

	delete clDlg;
}

void CRow::OnSelChangeDataType() 
{
	UpdateData(TRUE);
	CComboBox* cb = (CComboBox *)GetDlgItem(IDC_TYPE);
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

	switch (m_repR.cellR[m_sel].type)
	{
	case IO_ALPHA:
	case IO_MIXED:
	case IO_HAN:
		GetDlgItem(IDC_FLOAT)->SetWindowText("");
		GetDlgItem(IDC_FLOAT)->EnableWindow(FALSE);
		m_repR.cellR[m_sel].attr &= ~(FA_COMMA|FA_RIGHT);
	case IO_NUM:
	default:
		if (!m_common)
			GetDlgItem(IDC_FLOAT)->EnableWindow(TRUE);
		break;
	}
}

void CRow::OnRow01()		{ setRow(IDC_OPT01); }
void CRow::OnRow02()		{ setRow(IDC_OPT02); }
void CRow::OnRow03()		{ setRow(IDC_OPT03); }
void CRow::OnRow04()		{ setRow(IDC_OPT04); }
void CRow::OnRow05()		{ setRow(IDC_OPT05); }
void CRow::OnRow06()		{ setRow(IDC_OPT06); }
void CRow::OnRow07()		{ setRow(IDC_OPT07); }
void CRow::OnRow08()		{ setRow(IDC_OPT08); }
void CRow::OnRow09()		{ setRow(IDC_OPT09); }
void CRow::OnRow10()		{ setRow(IDC_OPT10); }
void CRow::OnRow11()		{ setRow(IDC_OPT11); }
void CRow::OnRow12()		{ setRow(IDC_OPT12); }
void CRow::OnRow13()		{ setRow(IDC_OPT13); }
void CRow::OnRow14()		{ setRow(IDC_OPT14); }
void CRow::OnRow15()		{ setRow(IDC_OPT15); }
void CRow::OnRow16()		{ setRow(IDC_OPT16); }
void CRow::OnRow17()		{ setRow(IDC_OPT17); }
void CRow::OnRow18()		{ setRow(IDC_OPT18); }
void CRow::OnRow19()		{ setRow(IDC_OPT19); }
void CRow::OnRow20()		{ setRow(IDC_OPT20); }
void CRow::OnRow21()		{ setRow(IDC_OPT21); }
void CRow::OnRow22()		{ setRow(IDC_OPT22); }
void CRow::OnRow23()		{ setRow(IDC_OPT23); }
void CRow::OnRow24()		{ setRow(IDC_OPT24); }
void CRow::OnRow25()		{ setRow(IDC_OPT25); }
void CRow::OnRow26()		{ setRow(IDC_OPT26); }
void CRow::OnRow27()		{ setRow(IDC_OPT27); }
void CRow::OnRow28()		{ setRow(IDC_OPT28); }
void CRow::OnRow29()		{ setRow(IDC_OPT29); }
void CRow::OnRow30()		{ setRow(IDC_OPT30); }
void CRow::OnRow31()		{ setRow(IDC_OPT31); }
void CRow::OnRow32()		{ setRow(IDC_OPT32); }

void CRow::setRow(int id, bool save)
{
	if (m_common)
		return;

	CString	tmps;
	CComboBox* cb;
	int	sel;

	tmps.Format("ROW %d", id-IDC_OPT01+1);
	GetDlgItem(IDC_NUMBER)->SetWindowText(tmps);
	
	if (save)
		saveRow();

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

	CButton* check = (CButton *) GetDlgItem(IDC_DOMINOC);
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

	m_len.Format("%d",   m_repR.cellR[m_sel].size);
	m_pRGB.Format("%d",  m_repR.cellR[m_sel].pRGB);
	m_tRGB.Format("%d",  m_repR.cellR[m_sel].tRGB);
	m_hpRGB.Format("%d", m_repR.cellR[m_sel].hpRGB);
	m_htRGB.Format("%d", m_repR.cellR[m_sel].htRGB);

	m_visible = (m_repR.cellR[m_sel].properties & PR_VISIBLE) ? TRUE : FALSE;
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
		m_row[sel].SetFocus(false);
		m_row[m_sel].SetFocus();
	}

	UpdateData(FALSE);
	GetDlgItem(IDC_SYMBOL)->SetFocus();
}

void CRow::setRow()
{
	CComboBox*       cb;

	m_len.Format("%d", m_repR.cellR[m_sel].size);
	m_hpRGB.Format("%d", m_repR.cellR[m_sel].hpRGB);
	m_htRGB.Format("%d", m_repR.cellR[m_sel].htRGB);
	m_pRGB.Format("%d", m_repR.cellR[m_sel].pRGB);
	m_tRGB.Format("%d", m_repR.cellR[m_sel].tRGB);

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

	GetDlgItem(IDC_SYMBOL)->EnableWindow(FALSE);
	GetDlgItem(IDC_STR)->EnableWindow(FALSE);
	GetDlgItem(IDC_FLOAT)->EnableWindow(FALSE);
	GetDlgItem(IDC_DATA)->EnableWindow(FALSE);
	GetDlgItem(IDC_VISIBLE)->EnableWindow(FALSE);
	GetDlgItem(IDC_INSERT)->EnableWindow(FALSE);
	GetDlgItem(IDC_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDC_DOMINOC)->EnableWindow(FALSE);
	GetDlgItem(IDC_DOMINO)->EnableWindow(FALSE);
	GetDlgItem(IDC_REFC)->EnableWindow(FALSE);
	GetDlgItem(IDC_REF)->EnableWindow(FALSE);
	GetDlgItem(IDC_SHIFTL)->EnableWindow(FALSE);
	GetDlgItem(IDC_SHIFTR)->EnableWindow(FALSE);
}

void CRow::saveRow()
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

	cb = (CComboBox *)GetDlgItem(IDC_ALIGNH);
	switch (cb->GetCurSel())
	{
	case 1:
		m_repR.cellR[m_sel].alignH = AL_LEFT;	break;
	case 2:
		m_repR.cellR[m_sel].alignH = AL_RIGHT;	break;
	case 0:
	default:
		m_repR.cellR[m_sel].alignH = AL_CENTER;	break;
	}

	cb = (CComboBox *)GetDlgItem(IDC_ALIGND);
	switch (cb->GetCurSel())
	{
	case 0:
		m_repR.cellR[m_sel].alignC = AL_CENTER;	break;
	case 1:
		m_repR.cellR[m_sel].alignC = AL_LEFT;	break;
	case 2:
	default:
		m_repR.cellR[m_sel].alignC = AL_RIGHT;	break;
	}

	if (m_common)
		return;

	strcpy(m_repR.cellR[m_sel].name, m_symbol);
	m_repR.cellR[m_sel].vals = atoi(m_float);
	if (m_repR.cellR[m_sel].vals != 0)
		m_repR.cellR[m_sel].attr |= FA_FLOAT;
	else
		m_repR.cellR[m_sel].attr &= ~FA_FLOAT;

	if (m_attr & GO_XHEAD)
		GetDlgItem(IDC_STR)->EnableWindow(FALSE);
	else
		strcpy(m_repR.cellR[m_sel].head, m_head);

	if (m_visible)
		m_repR.cellR[m_sel].properties |= PR_VISIBLE;
	else
		m_repR.cellR[m_sel].properties &= ~PR_VISIBLE;

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

void CRow::OnOK() 
{
	OnApply();
	CDialog::OnOK();
}

void CRow::OnApply() 
{
	saveRow();
	if (m_common)
	{
		if (m_repR.cellR[m_sel].type != m_pRepR->cellR[m_sel].type)
			applyCommonRow(RPR_TYPE);

		if (m_repR.cellR[m_sel].size != m_pRepR->cellR[m_sel].size)
			applyCommonRow(RPR_SIZE);

		if (m_repR.cellR[m_sel].pRGB != m_pRepR->cellR[m_sel].pRGB)
			applyCommonRow(RPR_PRGB);

		if (m_repR.cellR[m_sel].tRGB != m_pRepR->cellR[m_sel].tRGB)
			applyCommonRow(RPR_TRGB);

		if (m_repR.cellR[m_sel].hpRGB != m_pRepR->cellR[m_sel].hpRGB)
			applyCommonRow(RPR_HPRGB);

		if (m_repR.cellR[m_sel].htRGB != m_pRepR->cellR[m_sel].htRGB)
			applyCommonRow(RPR_HTRGB);

		if (m_repR.cellR[m_sel].attr != m_pRepR->cellR[m_sel].attr
			|| m_repR.cellR[m_sel].attr2 != m_pRepR->cellR[m_sel].attr2)
			applyCommonRow(RPR_ATTR);

		if (m_repR.cellR[m_sel].alignH != m_pRepR->cellR[m_sel].alignH)
			applyCommonRow(RPR_ALIGNH);

		if (m_repR.cellR[m_sel].alignC != m_pRepR->cellR[m_sel].alignC)
			applyCommonRow(RPR_ALIGNC);

		CopyMemory(m_pRepR, &m_repR, sizeof(_repR));
		m_parent->SendMessage(WM_CELL_PAINT);
		return;
	}

	int	rowN = 0;
	for (int ii = 0; ii < MAX_COLUMN; ii++)
	{
		if (m_repR.cellR[ii].name[0] == '\0')
		{
			for (int jj = ii; jj < MAX_COLUMN; jj++)
			{
				if (m_repR.cellR[jj].name[0] != '\0')
				{
					CopyMemory(&m_repR.cellR[ii], &m_repR.cellR[jj], sizeof(m_repR.cellR[jj]));
					rowN++;
					ZeroMemory(&m_repR.cellR[jj], sizeof(m_repR.cellR[jj]));
					break;
				}
			}
		}
		else
			rowN++;
	}
	m_repR.rows = (BYTE) rowN;
	for (ii = rowN; ii < MAX_COLUMN; ii++)
	{
		ZeroMemory(&m_repR.cellR[ii], sizeof(m_repR.cellR[ii]));
		m_repR.cellR[ii].pRGB   = m_repR.cellR[rowN-1].pRGB;
		m_repR.cellR[ii].tRGB   = m_repR.cellR[rowN-1].tRGB;
		m_repR.cellR[ii].hpRGB  = m_repR.cellR[rowN-1].hpRGB;
		m_repR.cellR[ii].htRGB  = m_repR.cellR[rowN-1].htRGB;
		m_repR.cellR[ii].type   = IO_NUM;
		m_repR.cellR[ii].alignH = AL_CENTER;
		m_repR.cellR[ii].alignC = AL_RIGHT;
		m_repR.cellR[ii].attr   = FA_COMMA|FA_RIGHT;
		m_repR.cellR[ii].properties |= PR_VISIBLE;

		m_row[ii].m_pRGB       = m_repR.cellR[ii].pRGB;
		m_row[ii].m_tRGB       = m_repR.cellR[ii].tRGB;
		m_row[ii].Invalidate();
	}

	CopyMemory(m_pRepR, &m_repR, sizeof(_repR));
	m_parent->SendMessage(WM_CELL_PAINT);
}

void CRow::OnDominoCheck() 
{
	CButton* domino = (CButton *)GetDlgItem(IDC_DOMINOC);
	if (domino->GetCheck() == 1)
		GetDlgItem(IDC_DOMINO)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_DOMINO)->EnableWindow(FALSE);
}

void CRow::OnReferenceCheck() 
{
	CButton* ref = (CButton *)GetDlgItem(IDC_REFC);
	if (ref->GetCheck() == 1)
		GetDlgItem(IDC_REF)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_REF)->EnableWindow(FALSE);
}

void CRow::applyCommonRow(int prop)
{
	for (int ii = 0; ii < m_repR.rows; ii++)
	{
		switch (prop)
		{
		case RPR_TYPE:
			m_repR.cellR[ii].type = m_repR.cellR[m_sel].type;	
			break;

		case RPR_SIZE:
			m_repR.cellR[ii].size = m_repR.cellR[m_sel].size;
			break;

		case RPR_ALIGNH:
			m_repR.cellR[ii].alignH = m_repR.cellR[m_sel].alignH;
			break;

		case RPR_ALIGNC:
			m_repR.cellR[ii].alignC = m_repR.cellR[m_sel].alignC;
			break;

		case RPR_HTRGB:
			m_repR.cellR[ii].htRGB = m_repR.cellR[m_sel].htRGB;
			break;

		case RPR_HPRGB:
			m_repR.cellR[ii].hpRGB = m_repR.cellR[m_sel].hpRGB;
			break;

		case RPR_TRGB:
			m_repR.cellR[ii].tRGB = m_repR.cellR[m_sel].tRGB;
			break;

		case RPR_PRGB:
			m_repR.cellR[ii].pRGB = m_repR.cellR[m_sel].pRGB;
			break;

		case RPR_ATTR:
			m_repR.cellR[ii].attr  = m_repR.cellR[m_sel].attr;
			m_repR.cellR[ii].attr2 = m_repR.cellR[m_sel].attr2;
			break;

		default:
			break;
		}
	}
}

void CRow::OnShiftLeft() 
{
	int	sel = m_sel;
	struct	_cellR	cellR;
	
	if (m_sel <= 0)
		return;

	m_sel--;
	CopyMemory(&cellR, &m_repR.cellR[m_sel], sizeof(_cellR));
	CopyMemory(&m_repR.cellR[m_sel], &m_repR.cellR[sel], sizeof(_cellR));
	CopyMemory(&m_repR.cellR[sel], &cellR, sizeof(_cellR));

	m_row[sel].m_pRGB   = m_repR.cellR[sel].pRGB;
	m_row[sel].m_tRGB   = m_repR.cellR[sel].tRGB;
	m_row[m_sel].m_pRGB = m_repR.cellR[m_sel].pRGB;
	m_row[m_sel].m_tRGB = m_repR.cellR[m_sel].tRGB;
	m_row[sel].SetFocus(false);
	m_row[m_sel].SetFocus();

	setRow(IDC_OPT01+m_sel, false);
}

void CRow::OnShiftRight() 
{
	int	sel = m_sel;
	struct	_cellR	cellR;

	if (m_sel >= MAX_COLUMN-1)
		return;

	m_sel++;
	CopyMemory(&cellR, &m_repR.cellR[m_sel], sizeof(_cellR));
	CopyMemory(&m_repR.cellR[m_sel], &m_repR.cellR[sel], sizeof(_cellR));
	CopyMemory(&m_repR.cellR[sel], &cellR, sizeof(_cellR));

	m_row[sel].m_pRGB   = m_repR.cellR[sel].pRGB;
	m_row[sel].m_tRGB   = m_repR.cellR[sel].tRGB;
	m_row[m_sel].m_pRGB = m_repR.cellR[m_sel].pRGB;
	m_row[m_sel].m_tRGB = m_repR.cellR[m_sel].tRGB;
	m_row[sel].SetFocus(false);
	m_row[m_sel].SetFocus();

	setRow(IDC_OPT01+m_sel, false);
}
