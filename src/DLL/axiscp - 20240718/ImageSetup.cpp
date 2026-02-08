// ImageSetup.cpp : implementation file
//

#include "stdafx.h"
#include "ImageSetup.h"
#include "ThemePreviewDlg.h"

#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define COLOR_INLINE	RGB(103,170,197)
#define COLOR_OUTLINE	RGB(255,255,255)
/////////////////////////////////////////////////////////////////////////////
// CImageSetup dialog


CImageSetup::CImageSetup(CWnd* pParent /*=NULL*/)
	: CSetupDialog(CImageSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageSetup)
	//}}AFX_DATA_INIT
	m_caption = "배경 / 테마 설정";
}


void CImageSetup::DoDataExchange(CDataExchange* pDX)
{
	CSetupDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageSetup)
	DDX_Control(pDX, IDC_CB_THEME, m_cbTheme);
	DDX_Control(pDX, IDC_CB_BKG, m_cbColor);
	DDX_Control(pDX, IDC_PICTURE, m_static);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImageSetup, CSetupDialog)
	//{{AFX_MSG_MAP(CImageSetup)
	ON_BN_CLICKED(IDC_FILEFIND, OnFilefind)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_CB_BKG, OnSelchangeCbBkg)
	ON_CBN_SELCHANGE(IDC_CB_POS, OnSelchangeCbPos)
	ON_BN_CLICKED(IDC_CHK_COLOR, OnChkColor)
	ON_BN_CLICKED(IDC_CHK_DISPLAY, OnChkDisplay)
	ON_BN_CLICKED(IDC_THEMEPREVIEW, OnThemepreview)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageSetup message handlers
BOOL CImageSetup::OnInitDialog() 
{
	CSetupDialog::OnInitDialog();
	m_bmpCombo = Axis::GetBitmap("CONF_COMBO");
	m_bmpCombo_N = Axis::GetBitmap("CONF_COMBO_N");
	loadSetup();
	m_font.CreatePointFont(90, "굴림체");
	
	return TRUE;
}

const char* section = "Image";

void CImageSetup::loadSetup()
{
	CString fileName = Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, SETUPFILE);

	if (IsFileExist(fileName))
	{
		CProfile profile(fileName);
		if (profile.GetString(section, "Stamp") != "")
		{
			Load(fileName);
			return;
		}
	}

	DefaultSetup();
}

void CImageSetup::DefaultSetup()
{
	Load(Format("%s\\%s\\%s", Axis::home, MTBLDIR, SETUPFILE));
}

void CImageSetup::ApplySetup()
{
	UpdateData();
	Save(Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, SETUPFILE));
}

void CImageSetup::Load(const char* file)
{
	CProfile profile(file);
	//AfxMessageBox(file);
	m_bmpFile = profile.GetString(section, "bkFile");
	Trim(m_bmpFile);
	
	//m_cbTheme.SetPalette(Axis::home);
	//m_cbTheme.SetP

	CProfile p(pkPalette);
	CString str = p.GetString(GENERALSN,"Palette");
	str.MakeUpper();

	if( str == "BLUE" )
	{
		m_cbTheme.SetCurSel( 0 );
	}
	else if( str == "GRAY" )
	{
		m_cbTheme.SetCurSel( 4 );
	}
	else if( str == "BROWN" )
	{
		m_cbTheme.SetCurSel( 2 );
	}
	else if( str == "VIOLET" )
	{
		m_cbTheme.SetCurSel( 3 );
	}
	else if( str == "GREEN" )
	{
		m_cbTheme.SetCurSel( 1 );
	}



	
	if (m_bmpFile.IsEmpty())
	{
		m_static.SetWindowText("지정한 배경 파일이 없습니다.");
	}
	else 
	{
		m_picture.Load(m_bmpFile);	
		Invalidate(FALSE);

		m_bitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			m_bmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		m_static.SetBitmap(m_bitmap);
		GetDlgItem(IDC_STATICFILE)->SetWindowText(m_bmpFile);
	}

	((CButton*)GetDlgItem(IDC_CHK_DISPLAY))->SetCheck(profile.GetInt(section, "UseImage"));
	((CButton*)GetDlgItem(IDC_CHK_COLOR))->SetCheck(profile.GetInt(section, "UseColor"));
	((CComboBox*)GetDlgItem(IDC_CB_POS))->SetCurSel(profile.GetInt(section, "posImage"));

	m_cbColor.SetColor((COLORREF)profile.GetInt(section, "bkColor"));
}

void CImageSetup::Save(const char* file)
{
	CProfile profile(file);

	profile.Write(section, "bkFile", m_bmpFile);
	profile.Write(section, "useImage", ((CButton*)GetDlgItem(IDC_CHK_DISPLAY))->GetCheck());
	profile.Write(section, "posImage", ((CComboBox*)GetDlgItem(IDC_CB_POS))->GetCurSel());
	profile.Write(section, "useColor", ((CButton*)GetDlgItem(IDC_CHK_COLOR))->GetCheck());
	profile.Write(section, "bkColor", (int)m_cbColor.GetColor());
	profile.Write(section, "stamp", CTime::GetCurrentTime().Format("%Y.%m.%d"));

	m_cbTheme.ChangePalette();

	CString str = m_cbTheme.m_strSkinName;

	CProfile profile1(pkPalette);	
	profile1.Write(GENERALSN, "Palette", str);
}

void CImageSetup::OnFilefind() 
{
	CFileDialog	dlg(TRUE, "*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
				"Image Files(*.bmp;*.jpg;*.gif)|*.bmp;*.jpg;*.gif||", NULL );
	if (dlg.DoModal() == IDOK)
	{
		CString file = dlg.GetPathName();
		CString ext = file.Right(4);
		ext.MakeLower();
		if (ext == ".bmp" || ext == ".jpg" || ext == ".gif" )
		{
			m_bmpFile = file;
			m_picture.Load(m_bmpFile);	Invalidate(FALSE);

			GetDlgItem(IDC_STATICFILE)->SetWindowText(m_bmpFile);
			m_static.SetWindowText("");
			m_bitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
				m_bmpFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			m_static.SetBitmap(m_bitmap);
			Invalidate();
		}
		else
		{
			::MessageBox(m_hWnd, "이미지 파일만 선택 가능합니다.", "배경화면 설정", MB_OK|MB_ICONWARNING);
			return;
		}
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
	
	return CSetupDialog::PreTranslateMessage(pMsg);
}

void CImageSetup::OnPaint() 
{
	CPaintDC dc(this); 

	CRect rc;
	m_static.GetWindowRect(rc);
	ScreenToClient(rc);

	if (((CButton*)GetDlgItem(IDC_CHK_COLOR))->GetCheck())
	{
		CBrush br;
		br.CreateSolidBrush(m_cbColor.GetColor());
		
		CBrush *oldBr = dc.SelectObject(&br);

		dc.PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);
		dc.SelectObject(oldBr);
	}

	if (((CButton*)GetDlgItem(IDC_CHK_DISPLAY))->GetCheck() && !m_bmpFile.IsEmpty())
	{
		const int cursel = ((CComboBox *)GetDlgItem(IDC_CB_POS))->GetCurSel();
		m_picture.Draw(dc.m_hDC, rc, cursel);
	}
	draw_groupBox(&dc, IDC_STATIC_16);
	draw_groupBox(&dc, IDC_STATIC_17);
	draw_groupBox(&dc, IDC_STATIC_18);
}

void CImageSetup::OnSelchangeCbBkg() 
{
	Invalidate();
}

void CImageSetup::OnSelchangeCbPos() 
{
	Invalidate();
}

void CImageSetup::OnChkColor() 
{
	Invalidate();
}

void CImageSetup::OnChkDisplay() 
{
	Invalidate();
}

void CImageSetup::OnThemepreview() 
{
	CThemePreviewDlg dlg(m_cbTheme.GetCurSel());
	if (dlg.DoModal() == IDOK)
	{
		m_cbTheme.SetCurSel(dlg.GetIndex());
	}
}

void CImageSetup::draw_groupBox(CPaintDC *pdc, int ids)
{
	pdc->SelectObject(m_font);
	CRect rc, out, pout;
	GetDlgItem(ids)->GetClientRect(rc);
	GetDlgItem(ids)->GetWindowRect(out);
	GetWindowRect(pout);
	out.InflateRect(pout);
	/*
	CString tmp;
	tmp.Format("[%d]",out.left);
	AfxMessageBox(tmp);
	*/
	rc.top+=6;
	//rc.right -=20;
	CPen pn_in, pn_out;
	pn_in.CreatePen(PS_SOLID, 1, Axis::GetColor(38));
	pn_out.CreatePen(PS_SOLID, 1, Axis::GetColor(76));
	
	rc.OffsetRect(out.left, out.top);	
	rc.OffsetRect(1,1);

	pdc->SelectObject(pn_out);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);
	rc.OffsetRect(-1,-1);
	pdc->SelectObject(pn_in);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);
	//pdc->SelectObject(pdc->GetCurrentBrush());
	//Axis::dialogBgColor
	//pdc->SelectObject(&Axis::dialogBgBrush);
	pdc->SetBkColor(Axis::dialogBgColor);
	CString data;
	GetDlgItem(ids)->GetWindowText(data);
	pdc->TextOut(rc.left+7, rc.top-5, data);
}

HBRUSH CImageSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CSetupDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO: Change any attributes of the DC here
	const int ids = ::GetWindowLong(pWnd->m_hWnd, GWL_ID);
	if ( (ids == IDC_CB_POS) || (ids == IDC_CB_BKG) || (ids == IDC_CB_THEME))
	{
		if(((CComboBox *) GetDlgItem(ids))->IsWindowEnabled())
		{
			if (m_bmpCombo)
			{
				CDC dc; 
				dc.CreateCompatibleDC(pDC);
				dc.SelectObject(m_bmpCombo);
				CRect rc;
				CPen pn; 
				pn.CreatePen(PS_SOLID, 1, RGB(125,125,125));				
				pDC->SelectObject(&pn);
				((CComboBox *) GetDlgItem(ids))->GetClientRect(rc);
				pDC->Rectangle(rc);
				//pDC->BitBlt(rc.right-19,0,rc.right,20,&dc,0,0,19,20,SRCCOPY);
				pDC->StretchBlt(rc.right-20,0,19,rc.bottom,&dc,0,0,19,20,SRCCOPY);
				dc.DeleteDC();
			}
		}else
		{
			if (m_bmpCombo_N)
			{
				CDC dc; 
				dc.CreateCompatibleDC(pDC);
				dc.SelectObject(m_bmpCombo_N);
				CRect rc;
				CPen pn; pn.CreatePen(PS_SOLID, 1, RGB(125,125,125));
				pDC->SelectObject(pn);
				((CComboBox *) GetDlgItem(ids))->GetClientRect(rc);
				pDC->Rectangle(rc);
				//pDC->BitBlt(rc.right-19,0,rc.right,20,&dc,0,0,SRCCOPY);
				pDC->StretchBlt(rc.right-20,0,19,rc.bottom,&dc,0,0,19,20,SRCCOPY);
				dc.DeleteDC();
			}		
		}
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
