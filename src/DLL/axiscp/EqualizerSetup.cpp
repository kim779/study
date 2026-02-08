// EqualizerSetup.cpp : implementation file
//

#include "stdafx.h"
#include "mmsystem.h"
#include "EqualizerSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int def_vols[] = {80,60,40,20,0};
int def_levs[] = {100, 500, 1000, 3000, 5000};

#define DF_PAINTING 

CEqualizerSetup::CEqualizerSetup(CWnd* pParent /*=NULL*/)
	: CSetupDialog(CEqualizerSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEqualizerSetup)
	m_level1 = _T("");
	m_level2 = _T("");
	m_level3 = _T("");
	m_level4 = _T("");
	m_level5 = _T("");
	//}}AFX_DATA_INIT
	m_caption = "이퀄라이저";

	
	m_sliders[0] = &m_slider1;
	m_sliders[1] = &m_slider2;
	m_sliders[2] = &m_slider3;
	m_sliders[3] = &m_slider4;
	m_sliders[4] = &m_slider5;
	m_eq_lev[0] = &m_level1;
	m_eq_lev[1] = &m_level2;
	m_eq_lev[2] = &m_level3;
	m_eq_lev[3] = &m_level4;
	m_eq_lev[4] = &m_level5;

	const int lev = 0x26, deflev = 0xC0;
	for(int n=0; n<5; ++n)
	{
		m_mdcol[n] = RGB(deflev-(n*lev), deflev-(n*lev), 0xFF);
		m_mscol[n] = RGB(0xFF, deflev-(n*lev), deflev-(n*lev));
	}

	m_def_wav = Axis::home + "\\image\\CHIMES.WAV";
}


void CEqualizerSetup::DoDataExchange(CDataExchange* pDX)
{
	CSetupDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEqualizerSetup)
	DDX_Control(pDX, IDC_CHK_MASU, m_chk_masu);
	DDX_Control(pDX, IDC_CHK_MADO, m_chk_mado);
	DDX_Control(pDX, IDC_CB_MADO, m_cb_mado);
	DDX_Control(pDX, IDC_CB_MASU, m_cb_masu);
	DDX_Control(pDX, IDC_SLIDER5, m_slider5);
	DDX_Control(pDX, IDC_SLIDER4, m_slider4);
	DDX_Control(pDX, IDC_SLIDER3, m_slider3);
	DDX_Control(pDX, IDC_SLIDER2, m_slider2);
	DDX_Control(pDX, IDC_SLIDER1, m_slider1);
	DDX_Control(pDX, IDC_MSEQUAL, m_mspic);
	DDX_Control(pDX, IDC_MDEQUAL, m_mdpic);
	DDX_Text(pDX, IDC_EDIT1, m_level1);
	DDV_MaxChars(pDX, m_level1, 10);
	DDX_Text(pDX, IDC_EDIT2, m_level2);
	DDV_MaxChars(pDX, m_level2, 10);
	DDX_Text(pDX, IDC_EDIT3, m_level3);
	DDV_MaxChars(pDX, m_level3, 10);
	DDX_Text(pDX, IDC_EDIT4, m_level4);
	DDV_MaxChars(pDX, m_level4, 10);
	DDX_Text(pDX, IDC_EDIT5, m_level5);
	DDV_MaxChars(pDX, m_level5, 10);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEqualizerSetup, CSetupDialog)
	//{{AFX_MSG_MAP(CEqualizerSetup)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT1, OnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, OnKillfocusEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT3, OnKillfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT4, OnKillfocusEdit4)
	ON_EN_KILLFOCUS(IDC_EDIT5, OnKillfocusEdit5)
	ON_BN_CLICKED(IDC_CHK_MADO, OnChkMado)
	ON_BN_CLICKED(IDC_CHK_MASU, OnChkMasu)
	ON_EN_SETFOCUS(IDC_EDIT1, OnSetfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT2, OnSetfocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT3, OnSetfocusEdit3)
	ON_EN_SETFOCUS(IDC_EDIT4, OnSetfocusEdit4)
	ON_EN_SETFOCUS(IDC_EDIT5, OnSetfocusEdit5)
	ON_BN_CLICKED(IDC_BTN_F_MADO, OnBtnFMado)
	ON_BN_CLICKED(IDC_BTN_F_MASU, OnBtnFMasu)
	ON_BN_CLICKED(IDC_BTN_L_MADO, OnBtnLMado)
	ON_BN_CLICKED(IDC_BTN_L_MASU, OnBtnLMasu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CEqualizerSetup::OnInitDialog() 
{
	CSetupDialog::OnInitDialog();

	((CButton*)GetDlgItem(IDC_RD_HJ))->SetCheck(TRUE);

	m_cb_masu.SetCurSel(3);
	m_cb_mado.SetCurSel(3);

	this->DefaultSetup();
	this->LoadSetup();

	for(int n=0; n<5; ++n)
	{
		m_sliders[n]->SetRange(0, 100, TRUE);
		m_sliders[n]->SetPos(m_eq_vol[n]);
	}

	OnChkMado();
	OnChkMasu();

	return TRUE;
}

void CEqualizerSetup::DrawEqaulizer(CPaintDC* pdc)
{
	int bottom{}, n{};
	CPen Pen, * oldPen{};
	CRect tRc;
	CWnd *pmdWnd = GetDlgItem(IDC_MDEQUAL);
	CWnd *pmsWnd = GetDlgItem(IDC_MSEQUAL);
	Pen.CreatePen(PS_SOLID, 1, RGB(0xC0,0xC0,0xC0));
	
	if (pmdWnd)
	{	
		pmdWnd->GetWindowRect(tRc);
		ScreenToClient(tRc);
		bottom = tRc.bottom;
		
		for(n=0; n<5; ++n)
		{
			pdc->FillSolidRect(tRc, m_mdcol[n]);
			tRc.bottom = (int)((bottom/100.0)*m_sliders[n]->GetPos());
		}
		tRc.bottom = bottom;
		
		pdc->Draw3dRect(tRc.left, tRc.top, tRc.Width(), tRc.Height(), RGB(0xC0, 0xC0, 0xC0), RGB(0x90, 0x90, 0x90));
		oldPen = pdc->SelectObject(&Pen);
		for(int n=0; n<tRc.Height(); n+=5)
		{
			pdc->MoveTo(tRc.left, n + tRc.top);
			pdc->LineTo(tRc.left + tRc.Width(), n + tRc.top);
		}
		pdc->SelectObject(oldPen);
	}
	
	if (pmsWnd)
	{	
		pmsWnd->GetWindowRect(tRc);
		ScreenToClient(tRc);

		bottom = tRc.bottom;
		for(n=0; n<5; ++n)
		{
			pdc->FillSolidRect(tRc, m_mscol[n]);
			tRc.bottom = (int)((bottom/100.0)*m_sliders[n]->GetPos());
		}
		tRc.bottom = bottom;
		
		pdc->Draw3dRect(tRc.left, tRc.top, tRc.Width(), tRc.Height(), RGB(0xC0, 0xC0, 0xC0), RGB(0x90, 0x90, 0x90));
		oldPen = pdc->SelectObject(&Pen);
		for(int n=0; n<tRc.Height(); n+=5)
		{
			pdc->MoveTo(tRc.left, n + tRc.top);
			pdc->LineTo(tRc.left + tRc.Width(), n + tRc.top);
		}
		pdc->SelectObject(oldPen);
	}
	
	Pen.DeleteObject();
}

void CEqualizerSetup::DrawEqaulizer()
{
	int bottom{}, n{};
	CPen Pen, * oldPen{};
	CRect tRc;
	CWnd* pmdWnd = GetDlgItem(IDC_MDEQUAL);
	CWnd* pmsWnd = GetDlgItem(IDC_MSEQUAL);
	Pen.CreatePen(PS_SOLID, 1, RGB(0xC0, 0xC0, 0xC0));

	if (pmdWnd)
	{
		CClientDC pdc(pmdWnd);

		pmdWnd->GetClientRect(tRc);
		bottom = tRc.bottom;
		for (n = 0; n < 5; ++n)
		{
			pdc.FillSolidRect(tRc, m_mdcol[n]);
			tRc.bottom = (int)((bottom / 100.0) * m_sliders[n]->GetPos());
		}
		tRc.bottom = bottom;

		pdc.Draw3dRect(0, 0, tRc.Width(), tRc.Height(), RGB(0xC0, 0xC0, 0xC0), RGB(0x90, 0x90, 0x90));
		oldPen = pdc.SelectObject(&Pen);
		for (int n = 0; n < tRc.Height(); n += 5)
		{
			pdc.MoveTo(0, n);
			pdc.LineTo(tRc.Width(), n);
		}
		pdc.SelectObject(oldPen);
		pdc.DeleteDC();
	}
	if (pmsWnd)
	{
		CClientDC pdc(pmsWnd);

		pmsWnd->GetClientRect(tRc);
		bottom = tRc.bottom;
		for (n = 0; n < 5; ++n)
		{
			pdc.FillSolidRect(tRc, m_mscol[n]);
			tRc.bottom = (int)((bottom / 100.0) * m_sliders[n]->GetPos());
		}
		tRc.bottom = bottom;

		pdc.Draw3dRect(0, 0, tRc.Width(), tRc.Height(), RGB(0xC0, 0xC0, 0xC0), RGB(0x90, 0x90, 0x90));
		oldPen = pdc.SelectObject(&Pen);
		for (int n = 0; n < tRc.Height(); n += 5)
		{
			pdc.MoveTo(0, n);
			pdc.LineTo(tRc.Width(), n);
		}
		pdc.SelectObject(oldPen);
		pdc.DeleteDC();
	}

	Pen.DeleteObject();
}

void CEqualizerSetup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

#ifdef DF_PAINTING
	DrawEqaulizer(&dc);
#else
	DrawEqaulizer();
#endif
}

void CEqualizerSetup::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int n{};
	CSliderCtrl *pSlider = (CSliderCtrl*)pScrollBar;
	if (SB_THUMBPOSITION!=nSBCode && SB_THUMBTRACK!=nSBCode)
		nPos = pSlider->GetPos();

	for(n=0; n<5; ++n)
	{
		if (m_sliders[n]==pSlider)
		{
			if (n==0)
			{
				if (nPos==100) 
					pSlider->SetPos(99);
				else if (m_sliders[n+1]->GetPos()>=nPos)   //vc2019
					pSlider->SetPos(m_sliders[n+1]->GetPos()+1);
			}
			else if (n>0)
			{
				if (m_sliders[n-1]->GetPos()<= nPos)
					pSlider->SetPos(m_sliders[n-1]->GetPos()-1);
				else if (m_sliders[n+1]->GetPos()>= nPos)
					pSlider->SetPos(m_sliders[n+1]->GetPos()+1);
			}
			break;
		}
	}
	CSetupDialog::OnVScroll(nSBCode, nPos, pScrollBar);

	for(n=0; n<5; ++n)
		m_eq_vol[n] = m_sliders[n]->GetPos();


#ifdef DF_PAINTING
	CWnd* pmdWnd = GetDlgItem(IDC_MDEQUAL);
	CRect tRc;
	pmdWnd->GetWindowRect(tRc);
	ScreenToClient(tRc);
	InvalidateRect(tRc);
	CWnd* pmsWnd = GetDlgItem(IDC_MSEQUAL);
	pmsWnd->GetWindowRect(tRc);
	ScreenToClient(tRc);
	InvalidateRect(tRc);
#else
	DrawEqaulizer();
#endif
}

void CEqualizerSetup::LoadSetup()
{
	CString key, val;
	CProfile prop(pkUserSetup);
	for(int n=0; n<5; ++n)
	{
		m_eq_type = prop.GetInt(PROP_SECTION, "TYPE", 1);
		if (m_eq_type==0)
			m_eq_type = 1;
		
		key.Format("VOL%02d", n+1);
		m_eq_vol[n] = prop.GetInt(PROP_SECTION, key, def_vols[n]);
		
		key.Format("LEV%02d", n+1);
		val.Format("%d", def_levs[n]);
		*m_eq_lev[n] = prop.GetString(PROP_SECTION, key, val);
	}

	((CButton*)GetDlgItem(IDC_CHK_MADO))->SetCheck( prop.GetInt(PROP_SECTION, "MADO_ALARM", 0) );
	((CButton*)GetDlgItem(IDC_CHK_MASU))->SetCheck( prop.GetInt(PROP_SECTION, "MASU_ALARM", 0) );
	((CComboBox*)GetDlgItem(IDC_CB_MADO))->SetCurSel( prop.GetInt(PROP_SECTION, "MADO_ALARM_LEV", 3) );
	((CComboBox*)GetDlgItem(IDC_CB_MASU))->SetCurSel( prop.GetInt(PROP_SECTION, "MASU_ALARM_LEV", 3) );

	val = prop.GetString(PROP_SECTION, "MADO_ALARM_WAV", (LPCSTR)m_def_wav);
	if (val.IsEmpty()) val = m_def_wav;
	((CEdit*)GetDlgItem(IDC_MADO_MUSIC))->SetWindowText( (LPCSTR)val );

	val = prop.GetString(PROP_SECTION, "MASU_ALARM_WAV", (LPCSTR)m_def_wav);
	if (val.IsEmpty()) val = m_def_wav;
	((CEdit*)GetDlgItem(IDC_MASU_MUSIC))->SetWindowText( (LPCSTR)val );

	UpdateData(FALSE);
}

void CEqualizerSetup::DefaultSetup()
{
	int n{};

	m_eq_type = 1;
	for(n=0; n<5; ++n)
		m_eq_lev[n]->Format("%d", def_levs[n]);
	for(n=0; n<5; ++n)
	{
		m_eq_vol[n] = def_vols[n];
		m_sliders[n]->SetPos(def_vols[n]);
	}

	m_chk_mado.SetCheck(FALSE);
	m_chk_masu.SetCheck(FALSE);
	m_cb_mado.SetCurSel(3);
	m_cb_masu.SetCurSel(3);
	
	((CEdit*)GetDlgItem(IDC_MADO_MUSIC))->SetWindowText(m_def_wav);
	((CEdit*)GetDlgItem(IDC_MASU_MUSIC))->SetWindowText(m_def_wav);

	UpdateData(FALSE);
#ifndef DF_PAINTING
	DrawEqaulizer();
#endif
}

void CEqualizerSetup::ApplySetup()
{
	UpdateData();
	CString key, val;
	CProfile prop(pkUserSetup);
	for(int n=0; n<5; ++n)
	{
		prop.Write(PROP_SECTION, "TYPE", m_eq_type);

		key.Format("VOL%02d", n+1);
		prop.Write(PROP_SECTION, key, (int)m_eq_vol[n]);

		key.Format("LEV%02d", n+1);
		prop.Write(PROP_SECTION, key, (LPCSTR)*m_eq_lev[n]);

		key.Format("DCOL%02d", n+1);
		prop.Write(PROP_SECTION, key, (int)m_mdcol[n]);

		key.Format("SCOL%02d", n+1);
		prop.Write(PROP_SECTION, key, (int)m_mscol[n]);
	}

	prop.Write(PROP_SECTION, "MADO_ALARM", ((CButton*)GetDlgItem(IDC_CHK_MADO))->GetCheck() ? 1 : 0);
	prop.Write(PROP_SECTION, "MASU_ALARM", ((CButton*)GetDlgItem(IDC_CHK_MASU))->GetCheck() ? 1 : 0);
	prop.Write(PROP_SECTION, "MADO_ALARM_LEV", ((CComboBox*)GetDlgItem(IDC_CB_MADO))->GetCurSel() );
	prop.Write(PROP_SECTION, "MASU_ALARM_LEV", ((CComboBox*)GetDlgItem(IDC_CB_MASU))->GetCurSel() );

	((CEdit*)GetDlgItem(IDC_MADO_MUSIC))->GetWindowText(val);
	if (val.IsEmpty()) val = m_def_wav;
	prop.Write(PROP_SECTION, "MADO_ALARM_WAV", (LPCSTR)val);

	((CEdit*)GetDlgItem(IDC_MASU_MUSIC))->GetWindowText(val);
	if (val.IsEmpty()) val = m_def_wav;
	prop.Write(PROP_SECTION, "MASU_ALARM_WAV", (LPCSTR)val);
}

void CEqualizerSetup::CheckEdit(int idx, CEdit *pEdit, CEdit *pPrevEdit, CEdit *pNextEdit)
{
	double fval1{}, fval2{};
	CString val;

	pEdit->GetWindowText(val);
	fval1 = atof(val);
	if (pPrevEdit)
	{
		pPrevEdit->GetWindowText(val);
		fval2 = atof(val);
		if (fval1<=fval2)
			pEdit->SetWindowText( (LPCSTR)*m_eq_lev[idx] );
	}
	if (pNextEdit)
	{
		pNextEdit->GetWindowText(val);
		fval2 = atof(val);
		if (fval1>=fval2)
			pEdit->SetWindowText( (LPCSTR)*m_eq_lev[idx] );
	}
	
	GetDlgItem(IDC_EDIT1)->GetWindowText(*m_eq_lev[0]);
	GetDlgItem(IDC_EDIT2)->GetWindowText(*m_eq_lev[1]);
	GetDlgItem(IDC_EDIT3)->GetWindowText(*m_eq_lev[2]);
	GetDlgItem(IDC_EDIT4)->GetWindowText(*m_eq_lev[3]);
	GetDlgItem(IDC_EDIT5)->GetWindowText(*m_eq_lev[4]);

}

void CEqualizerSetup::OnKillfocusEdit1() 
{
	CheckEdit(0, (CEdit*)GetDlgItem(IDC_EDIT1), NULL, (CEdit*)GetDlgItem(IDC_EDIT2));
}

void CEqualizerSetup::OnKillfocusEdit2() 
{
	CheckEdit(1, (CEdit*)GetDlgItem(IDC_EDIT2), (CEdit*)GetDlgItem(IDC_EDIT1), (CEdit*)GetDlgItem(IDC_EDIT3));
}

void CEqualizerSetup::OnKillfocusEdit3() 
{
	CheckEdit(2, (CEdit*)GetDlgItem(IDC_EDIT3), (CEdit*)GetDlgItem(IDC_EDIT2), (CEdit*)GetDlgItem(IDC_EDIT4));
}

void CEqualizerSetup::OnKillfocusEdit4() 
{
	CheckEdit(3, (CEdit*)GetDlgItem(IDC_EDIT4), (CEdit*)GetDlgItem(IDC_EDIT3), (CEdit*)GetDlgItem(IDC_EDIT5));
}

void CEqualizerSetup::OnKillfocusEdit5() 
{
	CheckEdit(4, (CEdit*)GetDlgItem(IDC_EDIT5), (CEdit*)GetDlgItem(IDC_EDIT4), NULL);
}

void CEqualizerSetup::OnChkMado() 
{
	const CButton *pBtn = (CButton*)GetDlgItem(IDC_CHK_MADO);
	const BOOL bChecked = pBtn->GetCheck();
	GetDlgItem(IDC_CB_MADO)->EnableWindow(bChecked);
	GetDlgItem(IDC_MADO_MUSIC)->EnableWindow(bChecked);
	GetDlgItem(IDC_BTN_F_MADO)->EnableWindow(bChecked);
	GetDlgItem(IDC_BTN_L_MADO)->EnableWindow(bChecked);
}

void CEqualizerSetup::OnChkMasu() 
{
	const CButton *pBtn = (CButton*)GetDlgItem(IDC_CHK_MASU);
	const BOOL bChecked = pBtn->GetCheck();
	GetDlgItem(IDC_CB_MASU)->EnableWindow(bChecked);
	GetDlgItem(IDC_MASU_MUSIC)->EnableWindow(bChecked);
	GetDlgItem(IDC_BTN_F_MASU)->EnableWindow(bChecked);
	GetDlgItem(IDC_BTN_L_MASU)->EnableWindow(bChecked);
}

void CEqualizerSetup::OnSetfocusEdit1() 
{
	((CEdit*)GetDlgItem(IDC_EDIT1))->SetSel(0, -1);
}

void CEqualizerSetup::OnSetfocusEdit2() 
{
	((CEdit*)GetDlgItem(IDC_EDIT2))->SetSel(0, -1);
}

void CEqualizerSetup::OnSetfocusEdit3() 
{
	((CEdit*)GetDlgItem(IDC_EDIT3))->SetSel(0, -1);
}

void CEqualizerSetup::OnSetfocusEdit4() 
{
	((CEdit*)GetDlgItem(IDC_EDIT4))->SetSel(0, -1);
}

void CEqualizerSetup::OnSetfocusEdit5() 
{
	((CEdit*)GetDlgItem(IDC_EDIT5))->SetSel(0, -1);
}

void CEqualizerSetup::OnBtnFMado() 
{
	CFileDialog dlg(TRUE, "wav", NULL, OFN_FILEMUSTEXIST|OFN_LONGNAMES, "Wav Files(*.WAV)|*.wav|", this);
	dlg.m_ofn.lpstrInitialDir = Axis::home + "\\image";
	dlg.m_ofn.lpstrTitle = "체결이퀄라이저 - 파일선택";
	if (dlg.DoModal()==IDOK)
		GetDlgItem(IDC_MADO_MUSIC)->SetWindowText( dlg.GetPathName( ) );
	GetDlgItem(IDC_BTN_F_MADO)->SetFocus();
}

void CEqualizerSetup::OnBtnFMasu() 
{
	CFileDialog dlg(TRUE, "wav", NULL, OFN_FILEMUSTEXIST|OFN_LONGNAMES, "Wav Files(*.WAV)|*.wav|", this);
	dlg.m_ofn.lpstrInitialDir = Axis::home + "\\image";
	dlg.m_ofn.lpstrTitle = "체결이퀄라이저 - 파일선택";
	if (dlg.DoModal()==IDOK)
		GetDlgItem(IDC_MASU_MUSIC)->SetWindowText( dlg.GetPathName( ) );
	GetDlgItem(IDC_BTN_F_MASU)->SetFocus();
}

void CEqualizerSetup::OnBtnLMado() 
{
	CString fname;
	((CEdit*)GetDlgItem(IDC_MADO_MUSIC))->GetWindowText(fname);
	PlaySound((LPCSTR)fname, NULL, SND_ASYNC );
}

void CEqualizerSetup::OnBtnLMasu() 
{
	CString fname;
	((CEdit*)GetDlgItem(IDC_MASU_MUSIC))->GetWindowText(fname);
	PlaySound((LPCSTR)fname, NULL, SND_ASYNC );
}

BOOL CEqualizerSetup::PreTranslateMessage(MSG* pMsg) 
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

