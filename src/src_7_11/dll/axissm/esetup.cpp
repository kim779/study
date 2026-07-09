// esetup.cpp : implementation file
//

#include "stdafx.h"
#include "esetup.h"
#include "tmenu.h"
#include "TrSearch.h"
#include "mmsystem.h"

#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESetup dialog


CESetup::CESetup(CString home, class CTMenu* menu, CWnd* pParent /*=NULL*/)
	: CSetupDialog(CESetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CESetup)
	m_Scnt = 0;
	m_idletime = 0;
	//}}AFX_DATA_INIT
	m_home = home;
	m_menu = menu;

	CWinApp* app = AfxGetApp();
	m_user = app->GetProfileString(WORKSTATION, SIGNONID);
	int pos = m_user.Find('|');
	if (pos < 0)
		m_user = _T("Anonymous");
	else	
		m_user = m_user.Mid(pos+1);

	if (m_user.IsEmpty())
		m_user = _T("Anonymous");

	m_caption = "사용자환경 설정";
}

void CESetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CESetup)
	DDX_Control(pDX, IDC_SPIN2, m_spin2);
	DDX_Control(pDX, IDC_SPIN1, m_spin1);
	DDX_Control(pDX, IDC_IDLETIME, m_lEdit);
	DDX_Control(pDX, IDC_SCREEN_CNT, m_sEdit);
	DDX_Text(pDX, IDC_SCREEN_CNT, m_Scnt);
	DDV_MinMaxUInt(pDX, m_Scnt, 0, 99);
	DDX_Text(pDX, IDC_IDLETIME, m_idletime);
	DDV_MinMaxUInt(pDX, m_idletime, 0, 99);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CESetup, CDialog)
	//{{AFX_MSG_MAP(CESetup)
	ON_BN_CLICKED(IDC_FILEFIND, OnFilefind)
	ON_BN_CLICKED(IDC_PLAYSOUND, OnPlaysound)
	//}}AFX_MSG_MAP
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpincontrol1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpincontrol2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESetup message handlers
void CESetup::OnDeltaposSpincontrol1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString string;
	m_sEdit.GetWindowText(string);
	m_Scnt = atoi(string);
	if (pNMUpDown->iDelta > 0)
		m_Scnt -= 1;
	else	m_Scnt += 1;
	if (m_Scnt < 0) m_Scnt = 0;
	if (m_Scnt > 99) m_Scnt = 99;

	string.Format("%d", m_Scnt);
	m_sEdit.SetWindowText(string);
	*pResult = 0;
}

void CESetup::OnDeltaposSpincontrol2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	CString string;
	m_lEdit.GetWindowText(string);
	m_idletime = atoi(string);
	if (pNMUpDown->iDelta > 0)
		m_idletime -= 1;
	else	m_idletime += 1;
	if (m_idletime < 0) m_idletime = 0;
	if (m_idletime > 99) m_idletime = 99;

	string.Format("%d", m_idletime);
	m_lEdit.SetWindowText(string);
	*pResult = 0;
}

BOOL CESetup::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	UINT	nRes = LOWORD(wParam);
	switch (nRes)
	{
	case IDC_8N:		m_childCnt = 0;	break;
	case IDC_16N:		m_childCnt = 1;	break;
	case IDC_24N:		m_childCnt = 2;	break;
	case IDC_32N:		m_childCnt = 3;	break;
	case IDC_USERCOUNT:	m_childCnt = 4;	break;
	case IDC_MAPNO:		m_openMap = 0;	break;
	case IDC_LASTMAP:	m_openMap = 1;	break;
	case IDC_USERMAP:	m_openMap = 2;	break;
	case IDC_FLESHONOFF:
		if (((CButton *) GetDlgItem(nRes))->GetCheck() == BST_CHECKED)	m_unflesh = 1;
		else	m_unflesh = 0;
		break;
	case IDC_POPUPPASSDLG:
		if (((CButton *) GetDlgItem(nRes))->GetCheck() == BST_CHECKED)	m_popupAcc = 1;
		else	m_popupAcc = 0;
		break;
	case IDC_POSDEFAULT:	m_childPos = 0;	break;
	case IDC_POSUP:		m_childPos = 1;	break;
	case IDC_POSCENTER:	m_childPos = 2;	break;
	case IDC_MSG_CENTER:	m_msgPos = 0;	break;
	case IDC_MSG_TOP:	m_msgPos = 1;	break;
	case IDC_MSG_BOTTOM:	m_msgPos = 2;	break;
	case IDC_VSCLS1:	((CComboBox*) GetDlgItem(IDC_STARTV1))->SetCurSel(-1);	break;
	case IDC_VSCLS2:	((CComboBox*) GetDlgItem(IDC_STARTV2))->SetCurSel(-1);	break;
	case IDC_VSCLS3:	((CComboBox*) GetDlgItem(IDC_STARTV3))->SetCurSel(-1);	break;
	case IDC_VSCLS4:	((CComboBox*) GetDlgItem(IDC_STARTV4))->SetCurSel(-1);	break;
	case IDC_VSCLS5:	((CComboBox*) GetDlgItem(IDC_STARTV5))->SetCurSel(-1);	break;
	case IDC_VSCLS6:	((CComboBox*) GetDlgItem(IDC_STARTV6))->SetCurSel(-1);	break;
	case IDC_8F:		m_fontN = 8;	break;
	case IDC_9F:		m_fontN = 9;	break;
	case IDC_10F:		m_fontN = 10;	break;
	case IDC_11F:		m_fontN = 11;	break;
	case IDC_12F:		m_fontN = 12;	break;
	case IDC_LINKBAR:
		if (((CButton *) GetDlgItem(nRes))->GetCheck() == BST_CHECKED)	m_linkbar = 1;
		else	m_linkbar = 0;
		break;
	case IDC_MAINTAB:
		if (((CButton *) GetDlgItem(nRes))->GetCheck() == BST_CHECKED)	m_maintab = 1;
		else	m_maintab = 0;
		break;
	}

	return CDialog::OnCommand(wParam, lParam);
}

BOOL CESetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_lEdit.SetLimitText(2);
	m_sEdit.SetLimitText(2);
	load_info();
	return TRUE;
}

void CESetup::load_info()
{
	CString		file, key;
	char		buf[256];
	UINT		nRes;

	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, SETUPFILE);
	m_openMap = GetPrivateProfileInt("SCREEN", "STARTMAP", 0, file);
	switch (m_openMap)
	{
	default:
	case 0:	nRes = IDC_MAPNO;	break;
	case 1: nRes = IDC_LASTMAP;	break;
	case 2: nRes = IDC_USERMAP;	break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(1);
	load_workspace();

	m_childCnt = GetPrivateProfileInt("ETC", "SELECT", 1, file);
	switch (m_childCnt)
	{
	case 0:	nRes = IDC_8N;		break;
	case 1:	nRes = IDC_16N;		break;
	case 2: nRes = IDC_24N;		break;
	case 3:	nRes = IDC_32N;		break;
	case 4:
	default:nRes = IDC_USERCOUNT;	break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(1);

	GetPrivateProfileString("ETC", "04", "24", buf, sizeof(buf), file);
	GetDlgItem(IDC_SCREEN_CNT)->SetWindowText(buf);

	m_childPos = GetPrivateProfileInt("SCREEN", "CHILDPOS", 0, file);
	switch (m_childPos)
	{
	default:
	case 0:	nRes = IDC_POSDEFAULT;	break;
	case 1: nRes = IDC_POSUP;	break;
	case 2: nRes = IDC_POSCENTER;	break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(1);

	m_fontN = GetPrivateProfileInt("SCREEN", "FONTSIZE", 9, file);

	switch (m_fontN)
	{
	case  8:	nRes = IDC_8F;		break;
	default:
	case  9:	nRes = IDC_9F;		break;
	case 10:	nRes = IDC_10F;		break;
	case 11:	nRes = IDC_11F;		break;
	case 12:	nRes = IDC_12F;		break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(1);

	m_popupAcc = GetPrivateProfileInt("SCREEN", "POPUPACC", 1, file);
	((CButton *) GetDlgItem(IDC_POPUPPASSDLG))->SetCheck(m_popupAcc);

	m_unflesh = GetPrivateProfileInt("SCREEN", "UNFLESH", 0, file);
	((CButton *) GetDlgItem(IDC_FLESHONOFF))->SetCheck(m_unflesh);

	if (GetPrivateProfileInt("SCREEN", "VS_REFLECT", 1, file))
		((CButton *) GetDlgItem(IDC_VSCREENTRIGGER))->SetCheck(1);
	
	m_msgPos = GetPrivateProfileInt("SCREEN", "MSGPOS", 0, file);
	switch (m_msgPos)
	{
	default:
	case 0:	nRes = IDC_MSG_CENTER;	break;
	case 1: nRes = IDC_MSG_TOP;	break;
	case 2: nRes = IDC_MSG_BOTTOM;	break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(1);

	m_idletime = GetPrivateProfileInt("SCREEN", "IDLETIME", 0, file);
	key.Format("%d", m_idletime);
	GetDlgItem(IDC_IDLETIME)->SetWindowText(key);

	m_linkbar = GetPrivateProfileInt("SCREEN", "LINKBAR", 0, file);
	((CButton *) GetDlgItem(IDC_LINKBAR))->SetCheck(m_linkbar);

	m_maintab = GetPrivateProfileInt("SCREEN", "MAINTAB", 1, file);
	((CButton *) GetDlgItem(IDC_MAINTAB))->SetCheck(m_maintab);

	CWinApp*	app = AfxGetApp();
	if (app->GetProfileInt(ENVIRONMENT, "keyprotect", 1))
		((CButton *) GetDlgItem(IDC_RUNKEYPROTECT))->SetCheck(1);

	if (app->GetProfileInt(INFORMATION, "use_prelude", 1))
		((CButton *) GetDlgItem(IDC_PRELUDE))->SetCheck(1);

	file = app->GetProfileString(INFORMATION, "prelude");
	GetDlgItem(IDC_STATICFILE)->SetWindowText(file);
}

void CESetup::save_info()
{
	CString		file, value;

	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, SETUPFILE);
	value.Format("%d", m_openMap);
	WritePrivateProfileString("SCREEN", "STARTMAP", value, file);
	save_workspace();
	
	switch (m_childCnt)
	{
	case 0: case 1: case 2:	case 3:	case 4:
		value.Format("%d", m_childCnt);	break;
	default:value = _T("1");		break;
	}

	WritePrivateProfileString("ETC", "SELECT", value, file);
	GetDlgItem(IDC_SCREEN_CNT)->GetWindowText(value);
	WritePrivateProfileString("ETC", "00", "08", file);
	WritePrivateProfileString("ETC", "01", "16", file);
	WritePrivateProfileString("ETC", "02", "24", file);
	WritePrivateProfileString("ETC", "03", "32", file);
	WritePrivateProfileString("ETC", "04", value, file);

	value.Format("%d", m_childPos);
	WritePrivateProfileString("SCREEN", "CHILDPOS", value, file);

	value.Format("%d", m_fontN);
	WritePrivateProfileString("SCREEN", "FONTSIZE", value, file);

	value.Format("%d", m_popupAcc);
	WritePrivateProfileString("SCREEN", "POPUPACC", value, file);

	value.Format("%d", m_unflesh);
	WritePrivateProfileString("SCREEN", "UNFLESH", value, file);

	if (((CButton *) GetDlgItem(IDC_VSCREENTRIGGER))->GetCheck() == BST_CHECKED)
		WritePrivateProfileString("SCREEN", "VS_REFLECT", "1", file);
	else	WritePrivateProfileString("SCREEN", "VS_REFLECT", "0", file);

	value.Format("%d", m_msgPos);
	WritePrivateProfileString("SCREEN", "MSGPOS", value, file);

	GetDlgItem(IDC_IDLETIME)->GetWindowText(value);
	WritePrivateProfileString("SCREEN", "IDLETIME", value, file);

	value.Format("%d", m_linkbar);
	WritePrivateProfileString("SCREEN", "LINKBAR", value, file);

	value.Format("%d", m_maintab);
	WritePrivateProfileString("SCREEN", "MAINTAB", value, file);

	CWinApp*	app = AfxGetApp();
	if (((CButton *) GetDlgItem(IDC_RUNKEYPROTECT))->GetCheck() == BST_CHECKED)
		app->WriteProfileInt(ENVIRONMENT, "keyprotect", 1);
	else
	{
		::MessageBox(m_hWnd, "키보드 보안 프로그램을 실행하지 않는 경우\n키보드 해킹에 의한 개인정보 유출의 위험이\n있습니다."
				, "키보드 보안경고",  MB_ICONWARNING);
		app->WriteProfileInt(ENVIRONMENT, "keyprotect", 0);
	}

	if (((CButton *) GetDlgItem(IDC_PRELUDE))->GetCheck() == BST_CHECKED)
		app->WriteProfileInt(INFORMATION, "use_prelude", 1);
	else
		app->WriteProfileInt(INFORMATION, "use_prelude", 0);
	GetDlgItem(IDC_STATICFILE)->GetWindowText(value);
	app->WriteProfileString(INFORMATION, "prelude", value);
}

void CESetup::load_workspace()
{
	int ii = 0;
	char		wb[256];
	DWORD		dw;
	CString		file, keys, value, wsName;
	CStringArray	ary;

	file.Format("%s\\user\\%s\\%s.ini", m_home, m_user, m_user);
	for (ii = 1; ; ii++)
	{
		keys.Format("%02d", ii);
		dw = GetPrivateProfileString("ROOT", keys, "", wb, sizeof(wb), file);
		if (dw <= 0)	break;

		ary.Add(wb);
	}

	if (ary.GetSize())
	{
		CArray <UINT, UINT > arRes;

		file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, SETUPFILE);
		arRes.Add(IDC_STARTV1);	arRes.Add(IDC_STARTV2);	arRes.Add(IDC_STARTV3);
		arRes.Add(IDC_STARTV4);	arRes.Add(IDC_STARTV5);	arRes.Add(IDC_STARTV6);

		for (ii = 0; ii < ary.GetSize(); ii++)
		{
			keys = ary.GetAt(ii);
			for (int jj = 0; jj < arRes.GetSize(); jj++)
				((CComboBox*) GetDlgItem(arRes.GetAt(jj)))->AddString(keys);
		}

		for (ii = 0; ii < arRes.GetSize(); ii++)
		{
			keys.Format("WS%02d", ii+1);
			dw = GetPrivateProfileString("ETC", keys, "", wb, sizeof(wb), file);
			if (dw <= 0)	continue;	wsName = wb;
			
			for (int jj = 0; jj < ary.GetSize(); jj++)
			{
				keys = ary.GetAt(jj);
				if (wsName.CollateNoCase(keys)) continue;
				((CComboBox*) GetDlgItem(arRes.GetAt(ii)))->SetCurSel(jj);
				break;
			}
		}

		ary.RemoveAll();
		arRes.RemoveAll();
	}
}

void CESetup::save_workspace()
{
	UINT			nRes;
	CString			name, keys, file, filex;
	CArray <UINT, UINT >	arRes;

	file.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, SETUPFILE);
	filex.Format("%s\\user\\%s\\%s.ini", m_home, m_user, m_user);
	arRes.Add(IDC_STARTV1);	arRes.Add(IDC_STARTV2);	arRes.Add(IDC_STARTV3);
	arRes.Add(IDC_STARTV4);	arRes.Add(IDC_STARTV5);	arRes.Add(IDC_STARTV6);

	WritePrivateProfileSection("STARTVSMAP", "", filex);

	for (int ii = 0, idx; ii < arRes.GetSize(); ii++)
	{
		nRes = arRes.GetAt(ii);
		keys.Format("WS%02d", ii+1);
		idx = ((CComboBox*) GetDlgItem(nRes))->GetCurSel();
		if (idx == CB_ERR)	continue;
		((CComboBox*) GetDlgItem(nRes))->GetLBText(idx, name);
		WritePrivateProfileString("ETC", keys, name, file);

		if (!name.IsEmpty())
		{
			CString		tmps;
			char		wb[512];
			for (int jj = 1; ; jj++)
			{
				tmps.Format("%03d", jj);
				DWORD dw = GetPrivateProfileString(name, tmps, "", wb, sizeof(wb), filex);
				if (dw <= 0)	break;
				keys.Format("%d%02d", ii, jj);
				WritePrivateProfileString("STARTVSMAP", keys, wb, filex);
			}
		}
	}
	arRes.RemoveAll();
}

void CESetup::ApplySetup()
{
	save_info();
}

void CESetup::DefaultSetup()
{
	CString		file, wave, key;
	char		buf[256];
	UINT		nRes;

	file.Format("%s\\%s\\%s", m_home, MTBLDIR, SETUPFILE);

	m_openMap = GetPrivateProfileInt("SCREEN", "STARTMAP", 0, file);
	((CButton *) GetDlgItem(IDC_MAPNO))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_LASTMAP))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_USERMAP))->SetCheck(0);

	switch (m_openMap)
	{
	default:
	case 0:	nRes = IDC_MAPNO;	break;
	case 1: nRes = IDC_LASTMAP;	break;
	case 2: nRes = IDC_USERMAP;	break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(1);

	((CComboBox*) GetDlgItem(IDC_STARTV1))->SetCurSel(-1);
	((CComboBox*) GetDlgItem(IDC_STARTV2))->SetCurSel(-1);
	((CComboBox*) GetDlgItem(IDC_STARTV3))->SetCurSel(-1);
	((CComboBox*) GetDlgItem(IDC_STARTV4))->SetCurSel(-1);
	((CComboBox*) GetDlgItem(IDC_STARTV5))->SetCurSel(-1);
	((CComboBox*) GetDlgItem(IDC_STARTV6))->SetCurSel(-1);
	
	m_childCnt = GetPrivateProfileInt("ETC", "SELECT", 1, file);
	((CButton *) GetDlgItem(IDC_8N))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_16N))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_24N))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_32N))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_USERCOUNT))->SetCheck(0);
	GetPrivateProfileString("ETC", "04", "24", buf, sizeof(buf), file);
	GetDlgItem(IDC_SCREEN_CNT)->SetWindowText(buf);

	switch (m_childCnt)
	{
	case 0:	nRes = IDC_8N;		break;
	case 1:	nRes = IDC_16N;		break;
	case 2: nRes = IDC_24N;		break;
	case 3:	nRes = IDC_32N;		break;
	case 4:
	default:nRes = IDC_USERCOUNT;	break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(1);

	m_childPos = GetPrivateProfileInt("SCREEN", "CHILDPOS", 0, file);
	((CButton *) GetDlgItem(IDC_POSDEFAULT))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_POSUP))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_POSCENTER))->SetCheck(0);

	switch (m_childPos)
	{
	default:
	case 0:	nRes = IDC_POSDEFAULT;	break;
	case 1: nRes = IDC_POSUP;	break;
	case 2: nRes = IDC_POSCENTER;	break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(1);

	m_fontN = GetPrivateProfileInt("SCREEN", "FONTSIZE", 9, file);
	((CButton *) GetDlgItem(IDC_8F))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_9F))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_10F))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_11F))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_12F))->SetCheck(0);

	switch (m_fontN)
	{
	case  8:	nRes = IDC_8F;		break;
	default:
	case  9:	nRes = IDC_9F;		break;
	case 10:	nRes = IDC_10F;		break;
	case 11:	nRes = IDC_11F;		break;
	case 12:	nRes = IDC_12F;		break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(1);

	m_popupAcc = GetPrivateProfileInt("SCREEN", "POPUPACC", 1, file);
	((CButton *) GetDlgItem(IDC_POPUPPASSDLG))->SetCheck(m_popupAcc);

	m_unflesh = GetPrivateProfileInt("SCREEN", "UNFLESH", 0, file);
	((CButton *) GetDlgItem(IDC_FLESHONOFF))->SetCheck(m_unflesh);

	if (GetPrivateProfileInt("SCREEN", "VS_REFLECT", 1, file))
		((CButton *) GetDlgItem(IDC_VSCREENTRIGGER))->SetCheck(1);
	else
		((CButton *) GetDlgItem(IDC_VSCREENTRIGGER))->SetCheck(0);
	
	m_msgPos = GetPrivateProfileInt("SCREEN", "MSGPOS", 0, file);
	((CButton *) GetDlgItem(IDC_MSG_CENTER))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_MSG_TOP))->SetCheck(0);
	((CButton *) GetDlgItem(IDC_MSG_BOTTOM))->SetCheck(0);

	m_linkbar = GetPrivateProfileInt("SCREEN", "LINKBAR", 0, file);
	((CButton *) GetDlgItem(IDC_LINKBAR))->SetCheck(m_linkbar);

	m_maintab = GetPrivateProfileInt("SCREEN", "MAINTAB", 1, file);
	((CButton *) GetDlgItem(IDC_MAINTAB))->SetCheck(m_maintab);

	switch (m_msgPos)
	{
	default:
	case 0:	nRes = IDC_MSG_CENTER;	break;
	case 1: nRes = IDC_MSG_TOP;	break;
	case 2: nRes = IDC_MSG_BOTTOM;	break;
	}
	((CButton *) GetDlgItem(nRes))->SetCheck(1);

	m_idletime = GetPrivateProfileInt("SCREEN", "IDLETIME", 0, file);
	key.Format("%d", m_idletime);
	GetDlgItem(IDC_IDLETIME)->SetWindowText(key);

	((CButton *) GetDlgItem(IDC_RUNKEYPROTECT))->SetCheck(1);
	((CButton *) GetDlgItem(IDC_PRELUDE))->SetCheck(1);
	wave.Format("%s\\%s\\%s", m_home, IMAGEDIR, "푸르덴셜투자증권.WAV");
	GetDlgItem(IDC_STATICFILE)->SetWindowText(wave);
}

void CESetup::OnFilefind() 
{
	CString file;
	GetDlgItem(IDC_STATICFILE)->GetWindowText(file);
	CFileDialog	dlg(TRUE, "*.wav", file, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_EXPLORER,
		"Sound file(*.wav) |*.wav||", NULL );
	if (dlg.DoModal() == IDOK)
		GetDlgItem(IDC_STATICFILE)->SetWindowText(dlg.GetPathName());
}

void CESetup::OnPlaysound() 
{
	CString file;
	GetDlgItem(IDC_STATICFILE)->GetWindowText(file);
	
	if (file.IsEmpty())
		MessageBeep(MB_OK);
	else
	{
		sndPlaySound(NULL, SND_NODEFAULT|SND_ASYNC);
		sndPlaySound(file, SND_NODEFAULT|SND_ASYNC);
	}
}

BOOL CESetup::PreTranslateMessage(MSG* pMsg) 
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
