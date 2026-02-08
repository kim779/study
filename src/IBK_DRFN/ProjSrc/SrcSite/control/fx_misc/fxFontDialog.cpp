// fxFontDialog.cpp : implementation file
//

#include "stdafx.h"
#include "fxFontDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfxFontDialog dialog

CfxFontDialog::CfxFontDialog(CString home, CWnd* pParent /*=NULL*/)
	: CDialog(CfxFontDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CfxFontDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_home   = home;
	m_faces  = _T("MS Sans Serif");
	m_points = 10;
	m_actual = NTM_REGULAR;
	m_fgColor = 0;	// black
	m_bgColor = 15;	// white
}

CfxFontDialog::CfxFontDialog(CString faces, int point, bool italic, bool bold, DWORD fgColor, DWORD bgColor,
			CString home, CWnd* pParent)
{
	m_home  = home;
	m_faces = faces;
	m_points.Format("%d", point);
	
	m_actual = 0;
	if (italic)
		m_actual |= NTM_ITALIC;
	if (bold)
		m_actual |= NTM_BOLD;
	if (!m_actual)
		m_actual = NTM_REGULAR;

	m_fgColor = fgColor;
	m_bgColor = bgColor;
}

CfxFontDialog::~CfxFontDialog()
{
}

void CfxFontDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CfxFontDialog)
	DDX_Control(pDX, IDC_BCOLOR, m_bgCB);
	DDX_Control(pDX, IDC_FCOLOR, m_fgCB);
	DDX_Control(pDX, IDC_STYLE, m_styleCB);
	DDX_Control(pDX, IDC_POINT, m_sizeCB);
	DDX_Control(pDX, IDC_FACE, m_nameCB);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CfxFontDialog, CDialog)
	//{{AFX_MSG_MAP(CfxFontDialog)
	ON_CBN_EDITUPDATE(IDC_FACE, OnFaceNameEditUpdate)
	ON_CBN_SELCHANGE(IDC_FACE, OnFaceNameSelChange)
	ON_CBN_EDITUPDATE(IDC_POINT, OnPointEditUpdate)
	ON_CBN_SELCHANGE(IDC_POINT, OnPointSelChange)
	ON_CBN_EDITCHANGE(IDC_STYLE, OnStyleEditChange)
	ON_CBN_SELCHANGE(IDC_STYLE, OnStyleSelChange)
	ON_CBN_SELCHANGE(IDC_FCOLOR, OnFgColorSelChange)
	ON_CBN_SELCHANGE(IDC_BCOLOR, OnBgColorSelChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfxFontDialog message handlers

BOOL CfxFontDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_fgCB.SetPalette(m_home);
	m_bgCB.SetPalette(m_home);

	fillFontNames();
	m_fgCB.SetColor(m_fgColor);
	m_bgCB.SetColor(m_bgColor);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CfxFontDialog::OnOK() 
{
	GetParent()->PostMessage(WM_KEYDOWN, (WPARAM)VK_RETURN, 0);
}

void CfxFontDialog::OnCancel() 
{
	GetParent()->PostMessage(WM_KEYDOWN, (WPARAM)VK_ESCAPE, 0);
}

void CfxFontDialog::OnFaceNameEditUpdate() 
{
	CString	str;
	int	entry;

	m_nameCB.GetWindowText(str);
	entry = m_nameCB.FindStringExact(-1, str);
	if (entry != CB_ERR)
	{
		m_nameCB.SetCurSel(entry);
		m_nameCB.SetEditSel(-1, -1);
		m_nameCB.GetLBText(entry, m_faces);
		fillFontSizes();
		GetParent()->PostMessage(WM_USER+112, 0, 0);
	}
}

void CfxFontDialog::OnFaceNameSelChange() 
{
	m_nameCB.GetLBText(m_nameCB.GetCurSel(), m_faces);
	fillFontSizes();
	GetParent()->PostMessage(WM_USER+112, 0, 0);
}

void CfxFontDialog::OnPointEditUpdate() 
{
	int	entry;

	m_sizeCB.GetWindowText(m_points);
	entry = m_sizeCB.FindStringExact(-1, m_points);
	if (entry != CB_ERR)
	{
		m_sizeCB.SetCurSel(entry);
		m_sizeCB.SetEditSel(-1, -1);
		GetParent()->PostMessage(WM_USER+112, 0, 0);
	}
}

void CfxFontDialog::OnPointSelChange() 
{
	int	entry = m_sizeCB.GetCurSel();
	if (entry != CB_ERR)
	{
		m_sizeCB.GetLBText(entry, m_points);
		GetParent()->PostMessage(WM_USER+112, 0, 0);
	}
}

void CfxFontDialog::OnStyleEditChange() 
{
	CString	str;
	int	entry;

	m_styleCB.GetWindowText(str);
	entry = m_styleCB.FindStringExact(-1, str);
	if (entry != CB_ERR)
	{
		m_styleCB.SetCurSel(entry);
		m_styleCB.SetEditSel(-1, -1);
		m_actual = m_styleCB.GetItemData(entry);
		GetParent()->PostMessage(WM_USER+112, 0, 0);
	}
}

void CfxFontDialog::OnStyleSelChange() 
{
	int	entry = m_styleCB.GetCurSel();
	m_actual = m_styleCB.GetItemData(entry);
	GetParent()->PostMessage(WM_USER+112, 0, 0);
}

void CfxFontDialog::OnFgColorSelChange() 
{
	m_fgColor = m_fgCB.GetColor();
	GetParent()->PostMessage(WM_USER+112, 0, 0);
}

void CfxFontDialog::OnBgColorSelChange() 
{
	m_bgColor = m_bgCB.GetColor();
	GetParent()->PostMessage(WM_USER+112, 0, 0);
}

BOOL CfxFontDialog::Create(CWnd* parent)
{
	HINSTANCE hinst = AfxGetResourceHandle();
	AfxSetResourceHandle(::GetModuleHandle("fx_misc.dll"));
	BOOL rc = CDialog::Create(IDD_FONTDLG, parent);
	SetOwner(parent);
	AfxSetResourceHandle(hinst);
	return rc;
}

void CfxFontDialog::SetFont(CString faces, int point, bool italic, bool bold, DWORD fgColor, DWORD bgColor, bool applyNow)
{
	m_faces = faces;

	m_points.Format("%d", point);
	
	m_actual = 0;
	if (italic)
		m_actual |= NTM_ITALIC;
	if (bold)
		m_actual |= NTM_BOLD;
	if (!m_actual)
		m_actual = NTM_REGULAR;

	m_fgColor = fgColor;
	m_bgColor = bgColor;

	if (applyNow)
	{
		int	entry = m_nameCB.FindStringExact(-1, m_faces);
		if (entry != CB_ERR)
		{
			m_nameCB.SetCurSel(entry);
			m_nameCB.SetEditSel(-1, -1);
			m_nameCB.GetLBText(entry, m_faces);
			fillFontSizes();
		}
		else if (m_nameCB.SetCurSel(0) != CB_ERR)
		{
			m_nameCB.GetLBText(0, m_faces);
			fillFontSizes();
		}

		m_fgCB.SetColor(m_fgColor);
		m_bgCB.SetColor(m_bgColor);
	}
}

void CfxFontDialog::GetFont(CString& faces, int& point, bool& italic, bool& bold, DWORD& fgColor, DWORD& bgColor)
{
	faces   = m_faces;
	point   = atoi(m_points);
	italic  = (m_actual & NTM_ITALIC) ? true : false;
	bold    = (m_actual & NTM_BOLD)   ? true : false;

	m_fgColor = m_fgCB.GetColor();
	m_bgColor = m_bgCB.GetColor();

	fgColor = m_fgColor;
	bgColor = m_bgColor;
}

CString CfxFontDialog::GetFontFaceName()
{
	return m_faces;
}

int CfxFontDialog::GetFontSize()
{
	return atoi(m_points);
}

bool CfxFontDialog::IsFontItalic()
{
	return (m_actual & NTM_ITALIC) ? true : false;
}

bool CfxFontDialog::IsFontBold()
{
	return (m_actual & NTM_BOLD) ? true : false;
}

DWORD CfxFontDialog::GetTextColor()
{
	return m_fgColor;
}

DWORD CfxFontDialog::GetBkColor()
{
	return m_bgColor;
}

void CfxFontDialog::Enable(bool eFace, bool eSize, bool eStyle, bool eFG, bool eBK)
{
	m_nameCB.EnableWindow(eFace   ? TRUE : FALSE);
	m_sizeCB.EnableWindow(eSize   ? TRUE : FALSE);
	m_styleCB.EnableWindow(eStyle ? TRUE : FALSE);
	m_fgCB.EnableWindow(eFG ? TRUE : FALSE);
	m_bgCB.EnableWindow(eBK ? TRUE : FALSE);
}

int CALLBACK EnumFontFamiliesCallBack(ENUMLOGFONTEX* lpelf, NEWTEXTMETRICEX* metric, int fontType, LPARAM lParam)
{
	CString	items;

	CfxFontDialog* dlg = (CfxFontDialog *)lParam;
	ASSERT(dlg);

	items = lpelf->elfLogFont.lfFaceName;
	if (items.Find('@') >= 0)
		return 1;

	dlg->m_nameCB.AddFont(&lpelf->elfLogFont, fontType);
	return 1;
}

void CfxFontDialog::fillFontNames()
{
	LOGFONT	lf;
	int	entry;
	CClientDC dc(NULL);
	
	m_nameCB.ResetContent();
	lf.lfCharSet        = DEFAULT_CHARSET;
	lf.lfFaceName[0]    = '\0';
	lf.lfPitchAndFamily = 0;
	EnumFontFamiliesEx(dc.GetSafeHdc(), &lf, (FONTENUMPROC)EnumFontFamiliesCallBack, (LPARAM)this, 0);

	m_style = 0;
	entry = m_nameCB.FindStringExact(-1, m_faces);
	if (entry != CB_ERR)
	{
		m_nameCB.SetCurSel(entry);
		m_nameCB.SetEditSel(-1, -1);
		m_nameCB.GetLBText(entry, m_faces);
		fillFontSizes();
	}
	else if (m_nameCB.SetCurSel(0) != CB_ERR)
	{
		m_nameCB.GetLBText(0, m_faces);
		fillFontSizes();
	}
}

int CALLBACK EnumFontFamiliesCallBack2(ENUMLOGFONTEX* lpelf, NEWTEXTMETRICEX* metric, int fontType, LPARAM lParam)
{
	CfxFontDialog* dlg = (CfxFontDialog *)lParam;
	ASSERT(dlg);

	if (fontType & TRUETYPE_FONTTYPE)
	{
		if (!(metric->ntmTm.ntmFlags & (NTM_BOLD | NTM_ITALIC)))
			dlg->m_style |= NTM_REGULAR;

		if (metric->ntmTm.ntmFlags & NTM_ITALIC)
			dlg->m_style |= NTM_ITALIC;

		if (metric->ntmTm.ntmFlags & NTM_BOLD)
			dlg->m_style |= NTM_BOLD;
	}
	else
	{
		if (fontType & RASTER_FONTTYPE)
		{
			CDC*	dc = dlg->GetDC();
			int	height = metric->ntmTm.tmHeight - metric->ntmTm.tmInternalLeading;
			dlg->m_sizeCB.AddSize(MulDiv(height, 72, dc->GetDeviceCaps(LOGPIXELSY)), height);
			dlg->ReleaseDC(dc);
		}

		if (lpelf->elfLogFont.lfWeight >= FW_BOLD && lpelf->elfLogFont.lfItalic)
			dlg->m_style |= NTM_BOLD | NTM_ITALIC;
		else if (lpelf->elfLogFont.lfWeight >= FW_BOLD)
			dlg->m_style |= NTM_BOLD;
		else if (lpelf->elfLogFont.lfItalic)
			dlg->m_style |= NTM_ITALIC;
		else
			dlg->m_style |= NTM_REGULAR;
	}

	return 1;
}

int _defaultTType[] = { 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48 };
void CfxFontDialog::fillFontSizes()
{
	int	entry;
	LOGFONT	lf;
	CString	str;
	CClientDC dc(NULL);

	m_sizeCB.ResetContent();
	m_styleCB.ResetContent();
	m_style = 0;

	lf.lfCharSet = DEFAULT_CHARSET;
	strcpy(lf.lfFaceName, m_faces);
	lf.lfPitchAndFamily = 0;
	EnumFontFamiliesEx(dc.GetSafeHdc(), &lf, (FONTENUMPROC)EnumFontFamiliesCallBack2, (LPARAM)this, 0);

	if (m_nameCB.GetFontType() != RASTER_FONTTYPE)
	{
		for (int ii = 0; ii < sizeof(_defaultTType)/sizeof(int); ii++)
			m_sizeCB.AddSize(_defaultTType[ii], 0);
	}

	BOOL	bRegular    = (BOOL)(m_style & NTM_REGULAR);
	BOOL	bBold       = (BOOL)(m_style & NTM_BOLD);
	BOOL	bItalic     = (BOOL)(m_style & NTM_ITALIC);
	BOOL	bBoldItalic = (BOOL)((m_style & NTM_BOLD) && (m_style & NTM_ITALIC));

	if (bRegular)
	{
		bBold   = TRUE;
		bItalic = TRUE;
	}
	if (bBold || bItalic)
		bBoldItalic = TRUE;

	if (bRegular)
	{
		str.LoadString(AFX_IDS_REGULAR);
		entry = m_styleCB.AddString(str);
		m_styleCB.SetItemData(entry, (DWORD)NTM_REGULAR);
	}
	if (bBold)
	{
		str.LoadString(AFX_IDS_BOLD);
		entry = m_styleCB.AddString(str);
		m_styleCB.SetItemData(entry, (DWORD)NTM_BOLD);
	}
	if (bItalic)
	{
		str.LoadString(AFX_IDS_ITALIC);
		entry = m_styleCB.AddString(str);
		m_styleCB.SetItemData(entry, (DWORD)NTM_ITALIC);
	}
	if (bBoldItalic)
	{
		str.LoadString(AFX_IDS_BOLDITALIC);
		entry = m_styleCB.AddString(str);
		m_styleCB.SetItemData(entry, (DWORD)NTM_ITALIC|NTM_BOLD);
	}

	if (m_sizeCB.FindString(-1, m_points) != CB_ERR)
	{
		entry = m_sizeCB.SelectString(-1, m_points);
		if (entry == CB_ERR)
			return;
	}
	else
	{
		m_sizeCB.SetCurSel(-1);
		m_sizeCB.SetWindowText(m_points);
	}

	int	count  = m_styleCB.GetCount();
	for (entry = 0; entry < count; entry++)
	{
		if (m_styleCB.GetItemData(entry) == m_actual)
		{
			m_styleCB.SetCurSel(entry);
			break;
		}
	}

	if (entry >= count)
	{
		m_styleCB.SetCurSel(0);
		m_actual = NTM_REGULAR;
	}
}
