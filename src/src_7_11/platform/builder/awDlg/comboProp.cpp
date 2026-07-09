// comboProp.cpp : implementation file
//

#include "stdafx.h"
#include "comboProp.h"
#include "cbitem.h"
#include "attrdlg.h"

#include "../mapform.h"
#include "../awCommon/colordlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboProp dialog

CComboProp::CComboProp(CWnd* pParent, struct _mapH* mapH, struct _formR* formR)
	: CDialog(CComboProp::IDD, pParent)
{
	m_parent = pParent;
	m_mapH   = mapH;
	m_formR  = formR;
	CopyMemory(&m_saveR, m_formR, sizeof(_formR));

	m_first = true;
	//{{AFX_DATA_INIT(CComboProp)
	m_bottom = _T("");
	m_count = _T("");
	m_enable = FALSE;
	m_fontN = _T("");
	m_left = _T("");
	m_len = _T("");
	m_pRGB = _T("");
	m_right = _T("");
	m_showhint = FALSE;
	m_symbol = _T("");
	m_top = _T("");
	m_tRGB = _T("");
	m_visible = FALSE;
	m_fPoint = _T("");
	m_showhelp = FALSE;
	m_help = _T("");
	m_xdelete = FALSE;
	m_index = _T("");
	m_valid = _T("");
	m_variant = _T("");
	m_tabskip = FALSE;
	m_code = FALSE;
	//}}AFX_DATA_INIT
}

void CComboProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CComboProp)
	DDX_Text(pDX, IDC_BOTTOM, m_bottom);
	DDV_MaxChars(pDX, m_bottom, 3);
	DDX_Text(pDX, IDC_COUNT, m_count);
	DDV_MaxChars(pDX, m_count, 2);
	DDX_Check(pDX, IDC_ENABLE, m_enable);
	DDX_Text(pDX, IDC_FONTN, m_fontN);
	DDV_MaxChars(pDX, m_fontN, 16);
	DDX_Text(pDX, IDC_LEFT, m_left);
	DDV_MaxChars(pDX, m_left, 4);
	DDX_Text(pDX, IDC_LENE, m_len);
	DDV_MaxChars(pDX, m_len, 3);
	DDX_Text(pDX, IDC_PRGBE, m_pRGB);
	DDV_MaxChars(pDX, m_pRGB, 8);
	DDX_Text(pDX, IDC_RIGHT, m_right);
	DDV_MaxChars(pDX, m_right, 4);
	DDX_Check(pDX, IDC_SHOWHINT, m_showhint);
	DDX_Text(pDX, IDC_SYMBOL, m_symbol);
	DDV_MaxChars(pDX, m_symbol, 16);
	DDX_Text(pDX, IDC_TOP, m_top);
	DDV_MaxChars(pDX, m_top, 3);
	DDX_Text(pDX, IDC_TRGBE, m_tRGB);
	DDV_MaxChars(pDX, m_tRGB, 8);
	DDX_Check(pDX, IDC_VISIBLE, m_visible);
	DDX_Text(pDX, IDC_FONTSZ, m_fPoint);
	DDV_MaxChars(pDX, m_fPoint, 2);
	DDX_Check(pDX, IDC_SHOWHELP, m_showhelp);
	DDX_Text(pDX, IDC_HINT, m_help);
	DDV_MaxChars(pDX, m_help, 64);
	DDX_Check(pDX, IDC_XDELETE, m_xdelete);
	DDX_Text(pDX, IDC_INDEX, m_index);
	DDV_MaxChars(pDX, m_index, 2);
	DDX_Text(pDX, IDC_VALID, m_valid);
	DDV_MaxChars(pDX, m_valid, 2);
	DDX_Text(pDX, IDC_VARIANT, m_variant);
	DDV_MaxChars(pDX, m_variant, 16);
	DDX_Check(pDX, IDC_TABPASS, m_tabskip);
	DDX_Check(pDX, IDC_CODE, m_code);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CComboProp, CDialog)
	//{{AFX_MSG_MAP(CComboProp)
	ON_EN_CHANGE(IDC_LENE, OnChangeLength)
	ON_BN_CLICKED(IDC_FONT, OnFont)
	ON_CBN_SELCHANGE(IDC_FONTS, OnSelChangeFontStyle)
	ON_BN_CLICKED(IDC_PRGB, OnPRGB)
	ON_BN_CLICKED(IDC_TRGB, OnTRGB)
	ON_BN_CLICKED(IDC_ATTR, OnAttr)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_DATA, OnData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboProp message handlers

BOOL CComboProp::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CEdit*	ed;
	CComboBox*	cb;
	CSpinButtonCtrl* spin;

	m_symbol = CString(m_formR->name);
	m_left.Format("%d", m_formR->left);
	m_top.Format("%d", m_formR->top);
	m_right.Format("%d", m_formR->right - m_formR->left);
	m_bottom.Format("%d", m_formR->bottom - m_formR->top);

	m_index.Format("%d", m_formR->checked);
	cb = (CComboBox *)GetDlgItem(IDC_TYPE);
	switch (m_formR->type)
	{
	case IO_NUM:
	default:
		cb->SetCurSel(0);	break;
	case IO_ALPHA:
		cb->SetCurSel(1);	break;
	case IO_HAN:
		cb->SetCurSel(2);	break;
	case IO_MIXED:
		cb->SetCurSel(3);	break;
	}
	cb = (CComboBox *)GetDlgItem(IDC_MODE);
	if (m_formR->iok == EIO_INOUT)
		cb->SetCurSel(1);
	else
		cb->SetCurSel(0);

	ed = (CEdit *)GetDlgItem(IDC_LENE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_LENS);
	spin->SetRange(0, 255);
	spin->SetBuddy(ed);
	m_len.Format("%d", m_formR->size);
	m_valid.Format("%d", HIWORD(m_formR->keys));
	m_count.Format("%d", LOWORD(m_formR->keys));

	cb = (CComboBox *)GetDlgItem(IDC_FONTS);
	cb->SetCurSel(m_formR->fStyle);
	if (m_formR->fStyle == 0)
	{
		GetDlgItem(IDC_FONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_FONTSZ)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_FONT)->EnableWindow(TRUE);
		m_fontN = CString(m_formR->fName);
		m_fPoint.Format("%d", m_formR->fPoint);
	}

	cb = (CComboBox *)GetDlgItem(IDC_CURSOR);
	if (m_formR->initC)
		cb->SetCurSel(0);
	else
		cb->SetCurSel(1);

	ed = (CEdit *)GetDlgItem(IDC_PRGBE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_PRGBS);
	spin->SetBuddy(ed);
	m_pRGB.Format("%d", m_formR->pRGB);

	ed = (CEdit *)GetDlgItem(IDC_TRGBE);
	spin = (CSpinButtonCtrl *)GetDlgItem(IDC_TRGBS);
	spin->SetBuddy(ed);
	m_tRGB.Format("%d", m_formR->tRGB);

	m_help    = CString(m_formR->help);
	m_variant = CString(m_formR->editS);
	m_visible  = (m_formR->properties & PR_VISIBLE) ? TRUE : FALSE;
	m_enable   = (m_formR->properties & PR_ENABLE)  ? TRUE : FALSE;
	m_showhint = (m_formR->properties & PR_HINT)    ? TRUE : FALSE;
	m_showhelp = (m_formR->properties & PR_HELP)    ? TRUE : FALSE;
	m_xdelete  = (m_formR->attr & FA_XDELETE)       ? TRUE : FALSE;
	m_tabskip  = (m_formR->attr & FA_PASS)          ? TRUE : FALSE;
	m_code     = (m_formR->properties & PR_CODE)    ? TRUE : FALSE;  

	UpdateData(FALSE);
	m_first = false;	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CComboProp::OnChangeLength() 
{
	if (m_first)	return;

	UpdateData(TRUE);
	m_formR->size = min(256, atoi(m_len));
}

void CComboProp::OnData() 
{
	UpdateData(TRUE);
	m_formR->size = min(255, atoi(m_len));
	CComboItem	cbItem(this, LOWORD(m_formR->keys), m_formR->size);
	CopyMemory(cbItem.m_disB, m_formR->dat, sizeof(m_formR->dat));
	CopyMemory(cbItem.m_datB, m_formR->str, sizeof(m_formR->str));
	if (cbItem.DoModal() == IDOK)
	{
		m_count.Format("%d", cbItem.m_count);
		m_formR->keys = MAKELONG((WORD)atoi(m_valid), (WORD)cbItem.m_count);
		CopyMemory(m_formR->dat, cbItem.m_disB, sizeof(m_formR->dat));
		CopyMemory(m_formR->str, cbItem.m_datB, sizeof(m_formR->str));
		UpdateData(FALSE);
	}
}

void CComboProp::OnFont() 
{
	CFontDialog	fontD;
	CComboBox*	cb;

	fontD.m_cf.lStructSize = sizeof(CHOOSEFONT);
	strcpy(fontD.m_cf.lpLogFont->lfFaceName, m_formR->fName);
	cb = (CComboBox *)GetDlgItem(IDC_FONTS);
	m_formR->fStyle = (WORD)cb->GetCurSel();
	if (m_formR->fStyle == FS_ITALIC || m_formR->fStyle == FS_IBOLD)
		fontD.m_cf.lpLogFont->lfItalic = TRUE;
	else
		fontD.m_cf.lpLogFont->lfItalic = FALSE;

	if (m_formR->fStyle == FS_BOLD || m_formR->fStyle == FS_IBOLD)
		fontD.m_cf.lpLogFont->lfWeight = FW_BOLD;
	else
		fontD.m_cf.lpLogFont->lfWeight = FW_NORMAL;

	fontD.m_cf.lpLogFont->lfCharSet = DEFAULT_CHARSET;
	fontD.m_cf.hDC = 0;
	fontD.m_cf.Flags  = CF_ENABLEHOOK | CF_FORCEFONTEXIST | CF_SCREENFONTS;
	fontD.m_cf.Flags |= CF_INITTOLOGFONTSTRUCT|CF_NOVERTFONTS;
	fontD.m_cf.rgbColors  = 0;
	fontD.m_cf.iPointSize = m_formR->fPoint;
	fontD.m_cf.nFontType  = SCREEN_FONTTYPE;
	fontD.m_cf.nSizeMin = 0;
	fontD.m_cf.nSizeMax = 0;

	if (fontD.DoModal() == IDOK)
	{
		if (fontD.IsItalic())
		{
			if (fontD.IsBold())
				m_formR->fStyle = FS_IBOLD;
			else
				m_formR->fStyle = FS_ITALIC;
		}
		else
		{
			if (fontD.IsBold())
				m_formR->fStyle = FS_BOLD;
			else
				m_formR->fStyle = FS_NORM;
		}
		strcpy(m_formR->fName, (char *)fontD.GetFaceName().operator LPCTSTR());

		m_formR->fPoint  = fontD.GetSize()/10;
		m_fPoint.Format("%d", m_formR->fPoint);
		m_fontN = CString(m_formR->fName);

		CComboBox* cb = (CComboBox *)GetDlgItem(IDC_FONTS);
		cb->SetCurSel(m_formR->fStyle);
		UpdateData(FALSE);
	}
}

void CComboProp::OnSelChangeFontStyle() 
{
	CComboBox* cb = (CComboBox *)GetDlgItem(IDC_FONTS);
	m_formR->fStyle = (BYTE)cb->GetCurSel();

	if (m_formR->fStyle == 0)
	{
		GetDlgItem(IDC_FONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_FONTSZ)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_FONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_FONTSZ)->EnableWindow(TRUE);
	}
}

void CComboProp::OnPRGB() 
{
	UpdateData(TRUE);
	m_formR->pRGB = atoi(m_pRGB);
	CColorDlg* clDlg = new CColorDlg(this, m_formR->pRGB);
	if (clDlg->DoModal() == IDOK)
	{
		m_pRGB.Format("%d", clDlg->m_color);
		UpdateData(FALSE);
	}

	delete clDlg;
}

void CComboProp::OnTRGB() 
{
	UpdateData(TRUE);
	m_formR->tRGB = atoi(m_tRGB);
	CColorDlg* clDlg = new CColorDlg(this, m_formR->tRGB);
	if (clDlg->DoModal() == IDOK)
	{
		m_tRGB.Format("%d", clDlg->m_color);
		UpdateData(FALSE);
	}

	delete clDlg;
}

void CComboProp::OnAttr() 
{
	CComboBox* cb = (CComboBox *)GetDlgItem(IDC_TYPE);
	m_formR->type = (BYTE)cb->GetCurSel()+1;

	CAttrDlg*	attrDlg;
	attrDlg = new CAttrDlg(this, FM_COMBO, m_formR->type, m_formR->iok, m_formR->attr, m_formR->attr2);
	if (attrDlg->DoModal() == IDOK)
	{
		m_formR->attr  = attrDlg->m_attr;
		m_formR->attr2 = attrDlg->m_attr2;
	}
	delete attrDlg;
}

void CComboProp::OnCancel() 
{
	CopyMemory(m_formR, &m_saveR, sizeof(_formR));
	CDialog::OnCancel();
}

void CComboProp::OnOK() 
{
	OnApply();
	CDialog::OnOK();
}

void CComboProp::OnApply() 
{
	CComboBox* cb;
	CRect	tRc, fRc;

	UpdateData(TRUE);
	m_mapH->modified = true;

	strcpy(m_formR->name, m_symbol);
	tRc.SetRect(m_formR->left, m_formR->top, m_formR->right, m_formR->bottom);
	m_formR->left = atoi(m_left);
	m_formR->top  = atoi(m_top);
	m_formR->right  = m_formR->left + atoi(m_right);
	m_formR->bottom = m_formR->top + atoi(m_bottom);
	fRc.SetRect(m_formR->left, m_formR->top, m_formR->right, m_formR->bottom);

	m_formR->checked = atoi(m_index);
	m_formR->pRGB = atoi(m_pRGB);
	m_formR->tRGB = atoi(m_tRGB);

	cb = (CComboBox *)GetDlgItem(IDC_TYPE);
	switch (cb->GetCurSel())
	{
	case 0:
	default:
		m_formR->type = IO_NUM;		break;
	case 1:
		m_formR->type = IO_ALPHA;	break;
	case 2:
		m_formR->type = IO_HAN;		break;
	case 3:
		m_formR->type = IO_MIXED;	break;
	}

	cb = (CComboBox *)GetDlgItem(IDC_MODE);
	switch (cb->GetCurSel())
	{
	case 0:
	default:
		m_formR->iok = EIO_INPUT;	break;
	case 1:
		m_formR->iok = EIO_INOUT;	break;
	}

	cb = (CComboBox *)GetDlgItem(IDC_FONTS);
	m_formR->fStyle = (WORD)cb->GetCurSel();
	strcpy(m_formR->fName, m_fontN);
	if (m_formR->fName[0] == NULL)
	{
		m_formR->fPoint = 0;
		m_formR->fStyle = FS_NONE;
	}
	else
		m_formR->fPoint = atoi(m_fPoint);

	m_formR->size = min(256, atoi(m_len));
	m_formR->keys = MAKELONG((WORD)atoi(m_count), (WORD)atoi(m_valid));

	cb = (CComboBox *)GetDlgItem(IDC_CURSOR);
	m_formR->initC = (!cb->GetCurSel()) ? true : false;

	strcpy(m_formR->help, m_help);
	strcpy(m_formR->editS, m_variant);
	if (m_visible)
		m_formR->properties |= PR_VISIBLE;
	else
		m_formR->properties &= ~PR_VISIBLE;

	if (m_showhint)
		m_formR->properties |= PR_HINT;
	else
		m_formR->properties &= ~PR_HINT;

	if (m_showhelp)
		m_formR->properties |= PR_HELP;
	else
		m_formR->properties &= ~PR_HELP;

	if (m_enable)
		m_formR->properties |= PR_ENABLE;
	else
		m_formR->properties &= ~PR_ENABLE;

	if (m_xdelete)
		m_formR->attr |= FA_XDELETE;
	else
		m_formR->attr &= ~FA_XDELETE;

	if (m_tabskip)
		m_formR->attr |= FA_PASS;
	else
		m_formR->attr &= ~FA_PASS;

	if (m_code)
		m_formR->properties |= PR_CODE;
	else
		m_formR->properties &= ~PR_CODE;

	if (!fRc.EqualRect(&tRc))
	{
		tRc.left   = min(tRc.left,   fRc.left);
		tRc.top    = min(tRc.top,    fRc.top);
		tRc.right  = max(tRc.right,  fRc.right);
		tRc.bottom = max(tRc.bottom, fRc.bottom);
	}
	CopyMemory(&m_saveR, m_formR, sizeof(_formR));
	m_parent->InvalidateRect(&tRc);
}
