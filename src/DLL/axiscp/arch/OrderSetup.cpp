// OrderSetup.cpp : implementation file
//

#include "stdafx.h"
#include "OrderSetup.h"
#include "../../h/axisvar.h"

#include "mmsystem.h"

#define	SAVEFILE	"ordcfg.ini"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma	comment(lib, "Winmm.lib")
/////////////////////////////////////////////////////////////////////////////
// COrderSetup dialog


COrderSetup::COrderSetup(CString home, CString user, CWnd* pParent /*=NULL*/)
	: CSetupDialog(home, user, COrderSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(COrderSetup)
	m_ckcount = 0;
	m_autohide = 0;
	//}}AFX_DATA_INIT
	m_caption = "주문/체결 설정";
}

void COrderSetup::DoDataExchange(CDataExchange* pDX)
{
	CSetupDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COrderSetup)
	DDX_Text(pDX, IDC_CHECOUNT, m_ckcount);
	DDV_MinMaxUInt(pDX, m_ckcount, 0, 99);
	DDX_Text(pDX, IDC_HIDECOUNT, m_autohide);
	DDV_MinMaxUInt(pDX, m_autohide, 0, 99);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COrderSetup, CSetupDialog)
	//{{AFX_MSG_MAP(COrderSetup)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_NOTIFY(UDN_DELTAPOS, IDC_CHESPIN, OnCheSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_HIDESPIN, OnHideSpin)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderSetup message handlers
void COrderSetup::OnCheSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(TRUE);
	if (pNMUpDown->iDelta > 0)
		m_ckcount -= 1;
	else	m_ckcount += 1;
	if (m_ckcount < 0) m_ckcount = 0;
	if (m_ckcount > 99) m_ckcount = 99;
	UpdateData(FALSE);
	*pResult = 0;
}

void COrderSetup::OnHideSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	UpdateData(TRUE);
	if (pNMUpDown->iDelta > 0)
	{
		if (m_autohide != 0)
			m_autohide -= 1;
	}
	else	m_autohide += 1;
	if (m_autohide < 0) m_autohide = 0;
	if (m_autohide > 99) m_autohide = 99;
	UpdateData(FALSE);
	*pResult = 0;
}

BOOL COrderSetup::OnInitDialog() 
{
	CSetupDialog::OnInitDialog();

	((CEdit *)GetDlgItem(IDC_CHECOUNT))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_HIDECOUNT))->LimitText(2);
	loadInfo();
	GetDlgItem(IDC_ST_CONCLUSION)->GetWindowRect(m_rect);
	m_rect.DeflateRect(0, 6, 0, 2);
	ScreenToClient(m_rect);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COrderSetup::loadInfo()
{
	CString		file, string;
	UINT		nRes;
	CWinApp*	app = AfxGetApp();

	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, SAVEFILE);

	int value = GetPrivateProfileInt("Common", "CK_POS", 5, file);
	switch (value)
	{
	default:
	case 5:	nRes = IDC_POSRB;	break;
	case 4:	nRes = IDC_POSMOUSE;	break;
	case 3:	nRes = IDC_POSLB;	break;
	case 2:	nRes = IDC_POSRT;	break;
	case 1:	nRes = IDC_POSCENTER;	break;
	case 0:	nRes = IDC_POSLT;	break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(BST_CHECKED);

	value = GetPrivateProfileInt("Common", "CK_RTF", CON_POPUP|CON_SOUND, file);
	if (value & CON_POPUP)
	{
		((CButton *) GetDlgItem(IDC_POPUPWINDOW))->SetCheck(BST_CHECKED);
		EnableConclusion(TRUE);
	}
	else
	{
		EnableConclusion(FALSE);
	}

	if (value & CON_MSN)	((CButton *) GetDlgItem(IDC_MSNWINDOW))->SetCheck(BST_CHECKED);
	if (value & CON_SOUND)
	{
		((CButton *) GetDlgItem(IDC_APPLY_SOUND))->SetCheck(BST_CHECKED);
		EnableSound(TRUE);
	}
	else
		EnableSound(FALSE);

	m_ckcount  = GetPrivateProfileInt("Common", "CK_HISTORY", 0, file);
	m_autohide = GetPrivateProfileInt("Common", "CK_AUTOHIDE", 0, file);

	if (GetPrivateProfileInt("Common", "SOUNDON_MDORDER", 1, file))
		((CButton *) GetDlgItem(IDC_MDORDER))->SetCheck(1);

	if (GetPrivateProfileInt("Common", "SOUNDON_MSORDER", 1, file))
		((CButton *) GetDlgItem(IDC_MSORDER))->SetCheck(1);
	
	if (GetPrivateProfileInt("Common", "SOUNDON_JJORDER", 1, file))
		((CButton *) GetDlgItem(IDC_JJORDER))->SetCheck(1);
	
	if (GetPrivateProfileInt("Common", "SOUNDON_CSORDER", 1, file))
		((CButton *) GetDlgItem(IDC_CSORDER))->SetCheck(1);
	
	if (GetPrivateProfileInt("Common", "SOUNDON_MDCON", 1, file))
		((CButton *) GetDlgItem(IDC_MDCON))->SetCheck(1);
	
	if (GetPrivateProfileInt("Common", "SOUNDON_MSCON", 1, file))
		((CButton *) GetDlgItem(IDC_MSCON))->SetCheck(1);

	if (GetPrivateProfileInt("Common", "SOUNDON_REFUSAL", 1, file))
		((CButton *) GetDlgItem(IDC_REFUSAL))->SetCheck(1);
	
	m_waveF[IDX_MDORDER].Format("%s\\image\\매도주문접수.wav", m_home);
	m_waveF[IDX_MSORDER].Format("%s\\image\\매수주문접수.wav", m_home);
	m_waveF[IDX_JJORDER].Format("%s\\image\\정정주문접수.wav", m_home);
	m_waveF[IDX_CSORDER].Format("%s\\image\\취소주문접수.wav", m_home);
	m_waveF[IDX_MDCON].Format("%s\\image\\매도주문체결.wav", m_home);
	m_waveF[IDX_MSCON].Format("%s\\image\\매수주문체결.wav", m_home);
	m_waveF[IDX_REFUSAL].Format("%s\\image\\주문거부.wav", m_home);

	char	buf[1024];
	DWORD	dw = GetPrivateProfileString("Common", "SOUNDFILE_MDORDER", "", buf, sizeof(buf), file);
	if (dw > 0)	m_waveF[IDX_MDORDER] = buf;

	dw = GetPrivateProfileString("Common", "SOUNDFILE_MSORDER", "", buf, sizeof(buf), file);
	if (dw > 0)	m_waveF[IDX_MSORDER] = buf;

	dw = GetPrivateProfileString("Common", "SOUNDFILE_JJORDER", "", buf, sizeof(buf), file);
	if (dw > 0)	m_waveF[IDX_JJORDER] = buf;

	dw = GetPrivateProfileString("Common", "SOUNDFILE_CSORDER", "", buf, sizeof(buf), file);
	if (dw > 0)	m_waveF[IDX_CSORDER] = buf;

	dw = GetPrivateProfileString("Common", "SOUNDFILE_MDCON", "", buf, sizeof(buf), file);
	if (dw > 0)	m_waveF[IDX_MDCON] = buf;

	dw = GetPrivateProfileString("Common", "SOUNDFILE_MSCON", "", buf, sizeof(buf), file);
	if (dw > 0)	m_waveF[IDX_MSCON] = buf;

	dw = GetPrivateProfileString("Common", "SOUNDFILE_REFUSAL", "", buf, sizeof(buf), file);
	if (dw > 0)	m_waveF[IDX_REFUSAL] = buf;

	GetDlgItem(IDC_MDORDER_FILENAME)->SetWindowText(m_waveF[IDX_MDORDER]);
	GetDlgItem(IDC_MSORDER_FILENAME)->SetWindowText(m_waveF[IDX_MSORDER]);
	GetDlgItem(IDC_JJORDER_FILENAME)->SetWindowText(m_waveF[IDX_JJORDER]);
	GetDlgItem(IDC_CSORDER_FILENAME)->SetWindowText(m_waveF[IDX_CSORDER]);
	GetDlgItem(IDC_MDCON_FILENAME)->SetWindowText(m_waveF[IDX_MDCON]);
	GetDlgItem(IDC_MSCON_FILENAME)->SetWindowText(m_waveF[IDX_MSCON]);
	GetDlgItem(IDC_REFU_FILENAME)->SetWindowText(m_waveF[IDX_REFUSAL]);

	UpdateData(FALSE);
}

void COrderSetup::saveInfo()
{
	UpdateData(TRUE);
	CString		file, str;
	CWinApp*	app = AfxGetApp();
	
	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, SAVEFILE);

	DWORD	rtfK = 0;
	if (((CButton *) GetDlgItem(IDC_POPUPWINDOW))->GetCheck() == BST_CHECKED)	rtfK = CON_POPUP;
	if (((CButton *) GetDlgItem(IDC_MSNWINDOW))->GetCheck() == BST_CHECKED)		rtfK |= CON_MSN;
	if (((CButton *) GetDlgItem(IDC_APPLY_SOUND))->GetCheck() == BST_CHECKED)	rtfK |= CON_SOUND;
	str.Format("%d", rtfK);	WritePrivateProfileString("Common", "CK_RTF", str, file);

	UINT	nRes;
	for (int ii = 0; ii < 6; ii++)
	{
		switch (ii)
		{
		default:
		case 5:	nRes = IDC_POSRB;	break;
		case 4:	nRes = IDC_POSMOUSE;	break;
		case 3:	nRes = IDC_POSLB;	break;
		case 2:	nRes = IDC_POSRT;	break;
		case 1:	nRes = IDC_POSCENTER;	break;
		case 0:	nRes = IDC_POSLT;	break;
		}
		
		if (((CButton *) GetDlgItem(nRes))->GetCheck() != BST_CHECKED)
			continue;
		str.Format("%d", ii);
		WritePrivateProfileString("Common", "CK_POS", str, file);
		break;
	}

	str.Format("%d", m_ckcount);
	WritePrivateProfileString("Common", "CK_HISTORY", str, file);
	str.Format("%d", m_autohide);
	WritePrivateProfileString("Common", "CK_AUTOHIDE", str, file);

	if (((CButton *) GetDlgItem(IDC_MDORDER))->GetCheck() == BST_CHECKED)
		WritePrivateProfileString("Common", "SOUNDON_MDORDER", "1", file);
	else	WritePrivateProfileString("Common", "SOUNDON_MDORDER", "0", file);

	if (((CButton *) GetDlgItem(IDC_MSORDER))->GetCheck() == BST_CHECKED)
		WritePrivateProfileString("Common", "SOUNDON_MSORDER", "1", file);
	else	WritePrivateProfileString("Common", "SOUNDON_MSORDER", "0", file);

	if (((CButton *) GetDlgItem(IDC_JJORDER))->GetCheck() == BST_CHECKED)
		WritePrivateProfileString("Common", "SOUNDON_JJORDER", "1", file);
	else	WritePrivateProfileString("Common", "SOUNDON_JJORDER", "0", file);

	if (((CButton *) GetDlgItem(IDC_CSORDER))->GetCheck() == BST_CHECKED)
		WritePrivateProfileString("Common", "SOUNDON_CSORDER", "1", file);
	else	WritePrivateProfileString("Common", "SOUNDON_CSORDER", "0", file);

	if (((CButton *) GetDlgItem(IDC_MDCON))->GetCheck() == BST_CHECKED)
		WritePrivateProfileString("Common", "SOUNDON_MDCON", "1", file);
	else	WritePrivateProfileString("Common", "SOUNDON_MDCON", "0", file);

	if (((CButton *) GetDlgItem(IDC_MSCON))->GetCheck() == BST_CHECKED)
		WritePrivateProfileString("Common", "SOUNDON_MSCON", "1", file);
	else	WritePrivateProfileString("Common", "SOUNDON_MSCON", "0", file);

	if (((CButton *) GetDlgItem(IDC_REFUSAL))->GetCheck() == BST_CHECKED)
		WritePrivateProfileString("Common", "SOUNDON_REFUSAL", "1", file);
	else	WritePrivateProfileString("Common", "SOUNDON_REFUSAL", "0", file);

	WritePrivateProfileString("Common", "SOUNDFILE_MDORDER", m_waveF[IDX_MDORDER], file);
	WritePrivateProfileString("Common", "SOUNDFILE_MSORDER", m_waveF[IDX_MSORDER], file);
	WritePrivateProfileString("Common", "SOUNDFILE_JJORDER", m_waveF[IDX_JJORDER], file);
	WritePrivateProfileString("Common", "SOUNDFILE_CSORDER", m_waveF[IDX_CSORDER], file);
	WritePrivateProfileString("Common", "SOUNDFILE_MDCON", m_waveF[IDX_MDCON], file);
	WritePrivateProfileString("Common", "SOUNDFILE_MSCON", m_waveF[IDX_MSCON], file);
	WritePrivateProfileString("Common", "SOUNDFILE_REFUSAL", m_waveF[IDX_REFUSAL], file);
}

BOOL COrderSetup::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int	idx = -1;
	CString	string;
	switch (LOWORD(wParam))
	{
	case IDC_REFU_SEARCH:idx++;
	case IDC_MSCON_SEARCH:	idx++;
	case IDC_MDCON_SEARCH:	idx++;
	case IDC_CSORDER_SEARCH:idx++;
	case IDC_JJORDER_SEARCH:idx++;
	case IDC_MSORDER_SEARCH:idx++;
	case IDC_MDORDER_SEARCH:idx++;
		SearchFile(idx);
		break;
	case IDC_REFU_PLAY:	idx++;
	case IDC_MSCON_PLAY:	idx++;
	case IDC_MDCON_PLAY:	idx++;
	case IDC_CSORDER_PLAY:	idx++;
	case IDC_JJORDER_PLAY:	idx++;
	case IDC_MSORDER_PLAY:	idx++;
	case IDC_MDORDER_PLAY:	idx++;
		Soundplay(idx);
		break;
	case IDC_APPLY_SOUND:
		if (((CButton *) GetDlgItem(IDC_APPLY_SOUND))->GetCheck())
			EnableSound(TRUE);
		else
			EnableSound(FALSE);
		break;
	case IDC_POPUPWINDOW:
		if (((CButton*)GetDlgItem(IDC_POPUPWINDOW))->GetCheck())
			EnableConclusion(TRUE);
		else
			EnableConclusion(FALSE);
		break;
	case IDOK:	saveInfo();	break;
	default:break;
	}
	return CSetupDialog::OnCommand(wParam, lParam);
}

void COrderSetup::SearchFile(int idx)
{
	if (idx < 0 || idx >= MAXSOUND)
		return;

	CFileDialog	dlg(TRUE, "*.wav", m_waveF[idx], 
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER,
			"Sound file(*.wav) |*.wav||", NULL );
	if (dlg.DoModal() == IDOK)
	{
		m_waveF[idx] = dlg.GetPathName();
		UINT nRes;
		switch (idx)
		{
		case 0:	nRes = IDC_MDORDER_FILENAME;	break;
		case 1:	nRes = IDC_MSORDER_FILENAME;	break;
		case 2:	nRes = IDC_JJORDER_FILENAME;	break;
		case 3:	nRes = IDC_CSORDER_FILENAME;	break;
		case 4:	nRes = IDC_MDCON_FILENAME;	break;
		case 5:	nRes = IDC_MSCON_FILENAME;	break;
		case 6:	nRes = IDC_REFU_FILENAME;	break;
		default:	return;
		}
		GetDlgItem(nRes)->SetWindowText(m_waveF[idx]);
	}
}

void COrderSetup::Soundplay(int idx)
{
	if (m_waveF[idx].IsEmpty())
		MessageBeep(MB_OK);
	else
	{
		sndPlaySound(NULL, SND_NODEFAULT|SND_ASYNC);
		sndPlaySound(m_waveF[idx], SND_NODEFAULT|SND_ASYNC);
	}
}


void COrderSetup::EnableConclusion(BOOL bEnable)
{
	CWnd*	pWnd = NULL;
	
	// 체결 그룹 박스
//	GetDlgItem(IDC_ST_CONCLUSION)->EnableWindow(bEnable);
	UINT	ids[] = {
			IDC_ST_POSITION,
			IDC_POSLT,
			IDC_POSCENTER,
			IDC_POSRT,
			IDC_POSLB,
			IDC_POSMOUSE,
			IDC_POSRB,
			IDC_ST_HISTORY,
			IDC_CHECOUNT,
			IDC_CHESPIN,
			IDC_ST_HISTORY2,
			IDC_ST_AUTOHIDE,
			IDC_HIDECOUNT,
			IDC_HIDESPIN,
			IDC_ST_AUTOHIDE2,
//			IDC_ST_SOUND,
		};
	
	int	ncnt = sizeof(ids)/sizeof(UINT);

	for ( int  ii = 0 ; ii < ncnt ; ii++ )
	{
		pWnd = GetDlgItem(ids[ii]);
		pWnd->EnableWindow(bEnable);
	}
}

void COrderSetup::EnableSound(BOOL bEnable)
{
	CWnd*	pWnd = NULL;
	
	// 체결 그룹 박스
//	GetDlgItem(IDC_ST_CONCLUSION)->EnableWindow(bEnable);
	UINT	ids[] = {
			IDC_MDORDER,
			IDC_MDORDER_FILENAME,
			IDC_MDORDER_SEARCH,
			IDC_MDORDER_PLAY,
			IDC_MSORDER,
			IDC_MSORDER_FILENAME,
			IDC_MSORDER_SEARCH,
			IDC_MSORDER_PLAY,
			IDC_JJORDER,
			IDC_JJORDER_FILENAME,
			IDC_JJORDER_SEARCH,
			IDC_JJORDER_PLAY,
			IDC_CSORDER,
			IDC_CSORDER_FILENAME,
			IDC_CSORDER_SEARCH,
			IDC_CSORDER_PLAY,
			IDC_MDCON,
			IDC_MDCON_FILENAME,
			IDC_MDCON_SEARCH,
			IDC_MDCON_PLAY,
			IDC_MSCON,
			IDC_MSCON_FILENAME,
			IDC_MSCON_SEARCH,
			IDC_MSCON_PLAY,
			IDC_REFUSAL,
			IDC_REFU_FILENAME,
			IDC_REFU_SEARCH,
			IDC_REFU_PLAY
		};
	
	int	ncnt = sizeof(ids)/sizeof(UINT);

	for ( int  ii = 0 ; ii < ncnt ; ii++ )
	{
		pWnd = GetDlgItem(ids[ii]);
		pWnd->EnableWindow(bEnable);
	}
}

void COrderSetup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	DrawBox(&dc, m_rect, RGB(128, 128, 128), BOX_TOP|BOX_LEFT);
	CRect	rect;
	rect = m_rect;
	rect.OffsetRect(1, 1);
	DrawBox(&dc, rect, RGB(255, 255, 255), BOX_TOP|BOX_LEFT);
	DrawBox(&dc, m_rect, RGB(128, 128, 128), BOX_BOTTOM|BOX_RIGHT);
	DrawBox(&dc, rect, RGB(255, 255, 255), BOX_BOTTOM|BOX_RIGHT);
	
	// Do not call CSetupDialog::OnPaint() for painting messages
}

void COrderSetup::DrawBox(CDC* pDC, CRect rect, COLORREF clrLine, int opt, COLORREF clrInner, bool bFill)
{
	CPen	pen(PS_SOLID, 1, clrLine);
	CPen*	oldpen = pDC->SelectObject(&pen);
	
	if (bFill)
		pDC->FillSolidRect(rect, clrInner);
	
	if (opt & BOX_LEFT)
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(CPoint(rect.left, rect.bottom));
	}

	if (opt & BOX_RIGHT)
	{
		pDC->MoveTo(CPoint(rect.right - 1, rect.top));
		pDC->LineTo(CPoint(rect.right - 1, rect.bottom));
	}

	if (opt & BOX_TOP)
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(CPoint(rect.right, rect.top));
	}

	if (opt & BOX_BOTTOM)
	{
		pDC->MoveTo(rect.left, rect.bottom);
		pDC->LineTo(CPoint(rect.right, rect.bottom));
	}
	
	pDC->SelectObject(oldpen);
	pen.DeleteObject();
}

void COrderSetup::ApplySetup()
{
	saveInfo();
}

void COrderSetup::DefaultSetup()
{
	CString		file, string;
	UINT		nRes;
	CWinApp*	app = AfxGetApp();

	file.Format("%s\\%s\\%s", m_home, MTBLDIR, SAVEFILE);

	int value = GetPrivateProfileInt("Common", "CK_POS", 5, file);
	for (int ii = 0; ii < 6; ii++)
	{
		switch (ii)
		{
		case 5:	nRes = IDC_POSRB;	break;
		case 4:	nRes = IDC_POSMOUSE;	break;
		case 3:	nRes = IDC_POSLB;	break;
		case 2:	nRes = IDC_POSRT;	break;
		case 1:	nRes = IDC_POSCENTER;	break;
		case 0:	nRes = IDC_POSLT;	break;
		}

		if (value == ii)
			((CButton *) GetDlgItem(nRes))->SetCheck(BST_CHECKED);
		else	((CButton *) GetDlgItem(nRes))->SetCheck(BST_UNCHECKED);
	}

	value = GetPrivateProfileInt("Common", "CK_RTF", CON_POPUP|CON_SOUND, file);
	if (value & CON_POPUP)
	{
		((CButton *) GetDlgItem(IDC_POPUPWINDOW))->SetCheck(BST_CHECKED);
		EnableConclusion(TRUE);
	}
	else
	{
		((CButton *) GetDlgItem(IDC_POPUPWINDOW))->SetCheck(BST_UNCHECKED);
		EnableConclusion(FALSE);
	}

	if (value & CON_MSN)	
		((CButton *) GetDlgItem(IDC_MSNWINDOW))->SetCheck(BST_CHECKED);
	else	((CButton *) GetDlgItem(IDC_MSNWINDOW))->SetCheck(BST_UNCHECKED);

	if (value & CON_SOUND)
	{
		((CButton *) GetDlgItem(IDC_APPLY_SOUND))->SetCheck(BST_CHECKED);
		EnableSound(TRUE);
	}
	else
	{
		((CButton *) GetDlgItem(IDC_APPLY_SOUND))->SetCheck(BST_UNCHECKED);
		EnableSound(FALSE);
	}

	m_ckcount  = GetPrivateProfileInt("Common", "CK_HISTORY", 0, file);
	m_autohide = GetPrivateProfileInt("Common", "CK_AUTOHIDE", 0, file);

	if (GetPrivateProfileInt("Common", "SOUNDON_MDORDER", 1, file))
		((CButton *) GetDlgItem(IDC_MDORDER))->SetCheck(1);
	else	((CButton *) GetDlgItem(IDC_MDORDER))->SetCheck(0);

	if (GetPrivateProfileInt("Common", "SOUNDON_MSORDER", 1, file))
		((CButton *) GetDlgItem(IDC_MSORDER))->SetCheck(1);
	else	((CButton *) GetDlgItem(IDC_MSORDER))->SetCheck(0);
	
	if (GetPrivateProfileInt("Common", "SOUNDON_JJORDER", 1, file))
		((CButton *) GetDlgItem(IDC_JJORDER))->SetCheck(1);
	else	((CButton *) GetDlgItem(IDC_JJORDER))->SetCheck(0);
	
	if (GetPrivateProfileInt("Common", "SOUNDON_CSORDER", 1, file))
		((CButton *) GetDlgItem(IDC_CSORDER))->SetCheck(1);
	else	((CButton *) GetDlgItem(IDC_CSORDER))->SetCheck(0);
	
	if (GetPrivateProfileInt("Common", "SOUNDON_MDCON", 1, file))
		((CButton *) GetDlgItem(IDC_MDCON))->SetCheck(1);
	else	((CButton *) GetDlgItem(IDC_MDCON))->SetCheck(0);
	
	if (GetPrivateProfileInt("Common", "SOUNDON_MSCON", 1, file))
		((CButton *) GetDlgItem(IDC_MSCON))->SetCheck(1);
	else	((CButton *) GetDlgItem(IDC_MSCON))->SetCheck(0);

	if (GetPrivateProfileInt("Common", "SOUNDON_REFUSAL", 1, file))
		((CButton *) GetDlgItem(IDC_REFUSAL))->SetCheck(1);
	else	((CButton *) GetDlgItem(IDC_REFUSAL))->SetCheck(0);
	
	m_waveF[IDX_MDORDER].Format("%s\\image\\매도주문접수.wav", m_home);
	m_waveF[IDX_MSORDER].Format("%s\\image\\매수주문접수.wav", m_home);
	m_waveF[IDX_JJORDER].Format("%s\\image\\정정주문접수.wav", m_home);
	m_waveF[IDX_CSORDER].Format("%s\\image\\취소주문접수.wav", m_home);
	m_waveF[IDX_MDCON].Format("%s\\image\\매도주문체결.wav", m_home);
	m_waveF[IDX_MSCON].Format("%s\\image\\매수주문체결.wav", m_home);
	m_waveF[IDX_REFUSAL].Format("%s\\image\\주문거부.wav", m_home);

	GetDlgItem(IDC_MDORDER_FILENAME)->SetWindowText(m_waveF[IDX_MDORDER]);
	GetDlgItem(IDC_MSORDER_FILENAME)->SetWindowText(m_waveF[IDX_MSORDER]);
	GetDlgItem(IDC_JJORDER_FILENAME)->SetWindowText(m_waveF[IDX_JJORDER]);
	GetDlgItem(IDC_CSORDER_FILENAME)->SetWindowText(m_waveF[IDX_CSORDER]);
	GetDlgItem(IDC_MDCON_FILENAME)->SetWindowText(m_waveF[IDX_MDCON]);
	GetDlgItem(IDC_MSCON_FILENAME)->SetWindowText(m_waveF[IDX_MSCON]);
	GetDlgItem(IDC_REFU_FILENAME)->SetWindowText(m_waveF[IDX_REFUSAL]);

	UpdateData(FALSE);
}

BOOL COrderSetup::PreTranslateMessage(MSG* pMsg) 
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
