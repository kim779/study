// ImageSetup.cpp : implementation file
//

#include "stdafx.h"
#include "ImageSetup.h"

#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageSetup dialog


CImageSetup::CImageSetup(CString home, CString user, CWnd* pParent /*=NULL*/)
	: CDialog(CImageSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageSetup)
	//}}AFX_DATA_INIT
	m_home = home;
	m_user = user;
}


void CImageSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageSetup)
	DDX_Control(pDX, IDC_CB_THEME, m_cbTheme);
	DDX_Control(pDX, IDC_CB_BKG, m_cbColor);
	DDX_Control(pDX, IDC_PICTURE, m_static);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImageSetup, CDialog)
	//{{AFX_MSG_MAP(CImageSetup)
	ON_BN_CLICKED(IDC_FILEFIND, OnFilefind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageSetup message handlers
BOOL CImageSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();

	loadSetup();
	
	return TRUE;
}

void CImageSetup::loadSetup()
{
	CString file, key, section = "Image";
	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, SETUPFILE);

	char	buf[256];
	GetPrivateProfileString(section, "bkFile", "", buf, sizeof(buf), file);
	m_bmpFile = buf;	m_bmpFile.TrimLeft();		m_bmpFile.TrimRight();

	m_cbTheme.SetPalette(m_home);

	if (m_bmpFile.IsEmpty())
		m_static.SetWindowText("지정한 배경 파일이 없습니다.");
	else 
	{
		m_bitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			m_bmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		
		m_static.SetBitmap(m_bitmap);
		
		GetDlgItem(IDC_STATICFILE)->SetWindowText(m_bmpFile);
	}

	int chk = GetPrivateProfileInt(section, "UseImage", 0, file);
	((CButton *)GetDlgItem(IDC_CHK_DISPLAY))->SetCheck(chk);
	
	chk = GetPrivateProfileInt(section, "UseColor", 0, file);
	((CButton *)GetDlgItem(IDC_CHK_COLOR))->SetCheck(chk);

	chk = GetPrivateProfileInt(section, "posImage", 0, file);
	((CComboBox *)GetDlgItem(IDC_CB_POS))->SetCurSel(chk);

	COLORREF color = GetPrivateProfileInt(section, "bkColor", 0, file);
	m_cbColor.SetColor(color);
}

void CImageSetup::ApplySetup()
{
	CString file, value, key, section = "Image";
	COLORREF color;
	int	chk, cursel;
	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, SETUPFILE);

	WritePrivateProfileString(section, "bkFile", m_bmpFile, file);

	chk = ((CButton *)GetDlgItem(IDC_CHK_DISPLAY))->GetCheck();
	value.Format("%d", chk);
	WritePrivateProfileString(section, "useImage", value, file);

	cursel = ((CComboBox *)GetDlgItem(IDC_CB_POS))->GetCurSel();
	value.Format("%d", cursel);
	WritePrivateProfileString(section, "posImage", value, file);

	chk = ((CButton *)GetDlgItem(IDC_CHK_COLOR))->GetCheck();
	value.Format("%d", chk);
	WritePrivateProfileString(section, "useColor", value, file);

	color = m_cbColor.GetColor();
	value.Format("%d", color);
	WritePrivateProfileString(section, "bkColor", value, file);

	m_cbTheme.ChangePalette();
}

void CImageSetup::DefaultSetup()
{
	CString file, key, section = "Image";
	file.Format("%s\\%s\\%s", m_home, MTBLDIR, SETUPFILE);

	char	buf[256];
	GetPrivateProfileString(section, "bkFile", "", buf, sizeof(buf), file);
	m_bmpFile = buf;	m_bmpFile.TrimLeft();		m_bmpFile.TrimRight();

	m_cbTheme.SetCurSel(0);

	if (m_bmpFile.IsEmpty())
		m_static.SetWindowText("지정한 배경 파일이 없습니다.");
	else 
	{
		m_bitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			m_bmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		
		m_static.SetBitmap(m_bitmap);
		
		GetDlgItem(IDC_STATICFILE)->SetWindowText(m_bmpFile);
	}

	int chk = GetPrivateProfileInt(section, "UseImage", 0, file);
	((CButton *)GetDlgItem(IDC_CHK_DISPLAY))->SetCheck(chk);
	
	chk = GetPrivateProfileInt(section, "UseColor", 0, file);
	((CButton *)GetDlgItem(IDC_CHK_COLOR))->SetCheck(chk);

	chk = GetPrivateProfileInt(section, "posImage", 0, file);
	((CComboBox *)GetDlgItem(IDC_CB_POS))->SetCurSel(chk);

	COLORREF color = GetPrivateProfileInt(section, "bkColor", 0, file);
	m_cbColor.SetColor(color);
}

void CImageSetup::OnFilefind() 
{
	CFileDialog	dlg(TRUE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				"바탕화면(*.bmp) |*.bmp||", NULL );
	if (dlg.DoModal() == IDOK)
	{
		CString file = dlg.GetPathName();
		CString ext = file.Right(4);
		ext.MakeLower();
		if (ext != ".bmp")
		{
			::MessageBox(m_hWnd, "BMP 파일만 선택 가능합니다.", "배경화면 설정", MB_OK|MB_ICONWARNING);
			return;
		}

		m_bmpFile = file;
		GetDlgItem(IDC_STATICFILE)->SetWindowText(m_bmpFile);
		m_static.SetWindowText("");

		m_bitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			m_bmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		
		m_static.SetBitmap(m_bitmap);
	}
}

BOOL CImageSetup::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			pMsg->wParam = VK_TAB;
		break;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
