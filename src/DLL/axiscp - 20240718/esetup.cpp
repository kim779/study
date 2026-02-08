// esetup.cpp : implementation file
//

#include "stdafx.h"
#include "esetup.h"
//#include "tmenu.h"
#include "TrSearch.h"
#include "mmsystem.h"

#include "../../axis/axMsg.hxx"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define COLOR_INLINE	RGB(103,170,197)
#define COLOR_OUTLINE	RGB(255,255,255)
/////////////////////////////////////////////////////////////////////////////
// CESetup dialog


CESetup::CESetup(CWnd* pParent /*=NULL*/)
	: CSetupDialog(CESetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CESetup)
	m_Scnt = 0;
	m_idletime = 0;

	m_caption = "사용자환경 설정";

	//}}AFX_DATA_INIT
}

void CESetup::DoDataExchange(CDataExchange* pDX)
{
	CSetupDialog::DoDataExchange(pDX);
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


BEGIN_MESSAGE_MAP(CESetup, CSetupDialog)
	//{{AFX_MSG_MAP(CESetup)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHK_HOTKEY, OnChkHotkey)
	//}}AFX_MSG_MAP
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpincontrol1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpincontrol2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESetup message handlers
void CESetup::OnDeltaposSpincontrol1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	const NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
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
	const NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
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
	const UINT	nRes = LOWORD(wParam);
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
	case IDC_SCREEN_NEW :
		if (((CButton *) GetDlgItem(nRes))->GetCheck() == BST_CHECKED)
		{
			m_screenNew = 1;
		}
		else
		{
			m_screenNew = 0;
		}
		break;
	case IDC_TITLE_CHANGE :
		if (((CButton *) GetDlgItem(nRes))->GetCheck() == BST_CHECKED)	
		{
			m_titleChange = 1;

			GetDlgItem(IDC_EDIT_TITLE)->EnableWindow(TRUE);
		}
		else
		{
			m_titleChange = 0;

			GetDlgItem(IDC_EDIT_TITLE)->EnableWindow(FALSE);
		}
		break;
	case IDC_FLESHONOFF:
		if (((CButton *) GetDlgItem(nRes))->GetCheck() == BST_CHECKED)	m_unflesh = 1;
		else	m_unflesh = 0;
		break;
	case IDC_POPUPPASSDLG:
		if (((CButton *) GetDlgItem(nRes))->GetCheck() == BST_CHECKED)	m_popupAcc = 1;
		else	m_popupAcc = 0;
		break;
	case IDC_START_SCREEN:
		if (((CButton *) GetDlgItem(nRes))->GetCheck() == BST_CHECKED)	m_startscreen = 1;
		else	m_startscreen = 0;
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
	case IDC_MAINTAB:
		if (((CButton *) GetDlgItem(nRes))->GetCheck() == BST_CHECKED)	m_maintab = 1;
		else	m_maintab = 0;
		break;
	case IDC_MSGBOX:
		if (((CButton *) GetDlgItem(nRes))->GetCheck() == BST_CHECKED)	m_msgbox = 1;
		else	m_msgbox = 0;
		break;
	case IDC_CODECTRL1: m_srchtype = 0; break;
	case IDC_CODECTRL2: m_srchtype = 1; break;
	case IDC_CHECK_PB:
		{
			if (((CButton *) GetDlgItem(nRes))->GetCheck() == BST_CHECKED)	m_pbSlide = 1;
			else	m_pbSlide = 0;
		}
		break;
	}

	return CSetupDialog::OnCommand(wParam, lParam);
}

BOOL CESetup::OnInitDialog() 
{
	CSetupDialog::OnInitDialog();
	m_font.CreatePointFont(90, "굴림체");
	m_lEdit.SetLimitText(2);
	m_sEdit.SetLimitText(2);
	m_bmpCombo = Axis::GetBitmap("CONF_COMBO");
	m_bmpSpin  = Axis::GetBitmap("CONF_SPIN");
	load_info();
	
	OnChkHotkey();

	return TRUE;
}

void CESetup::load_info()
{
	CString		file, key;
	char		buf[256]{};
	UINT		nRes{};

	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, SETUPFILE);
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

	m_screenNew = GetPrivateProfileInt("SCREEN", "SCREENNEW", 1, file);
	((CButton *) GetDlgItem(IDC_SCREEN_NEW))->SetCheck(m_screenNew);

	m_titleChange = GetPrivateProfileInt("SCREEN", "TITLECHANGE", 0, file);
	((CButton *) GetDlgItem(IDC_TITLE_CHANGE))->SetCheck(m_titleChange);

	if(m_titleChange)
	{
		GetDlgItem(IDC_EDIT_TITLE)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_TITLE)->EnableWindow(FALSE);
	}

	char buffTitle[1024];
	GetPrivateProfileString("SCREEN", "USERTITLE", "", buffTitle, sizeof(buffTitle)-1, file);
	((CEdit *) GetDlgItem(IDC_EDIT_TITLE))->SetWindowText(buffTitle);

	m_unflesh = GetPrivateProfileInt("SCREEN", "UNFLESH", 0, file);
	((CButton *) GetDlgItem(IDC_FLESHONOFF))->SetCheck(m_unflesh);

	m_startscreen = GetPrivateProfileInt("SCREEN", "STARTSCREEN", 1, file );
	((CButton *) GetDlgItem(IDC_START_SCREEN))->SetCheck(m_startscreen);

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

	m_maintab = GetPrivateProfileInt("SCREEN", "MAINTAB", 1, file);
	((CButton *) GetDlgItem(IDC_MAINTAB))->SetCheck(m_maintab);

	m_msgbox = GetPrivateProfileInt("SCREEN", "MSGBOX", 1, file);
	((CButton *) GetDlgItem(IDC_MSGBOX))->SetCheck(m_msgbox);

	//**int chk = AfxGetMainWnd()->SendMessage(WM_QWAY, MAKEWPARAM(aqGetEPassOmit, 0), 0);
	//**((CButton *) GetDlgItem(IDC_CERTIFY))->SetCheck(chk);

	const BOOL bUseHotkey = GetPrivateProfileInt("HOTKEY", "UseHideHotkey", 0, file);
	((CButton*)GetDlgItem(IDC_CHK_HOTKEY))->SetCheck(bUseHotkey);

	char buff[1024];
	GetPrivateProfileString("HOTKEY", "HideHotkey", "F12", buff, sizeof(buff)-1, file);
	((CComboBox*)GetDlgItem(IDC_CBN_HOTKEY))->SelectString(0, buff);

	m_srchtype = GetPrivateProfileInt("SCREEN", "CODECTRL", 0, file);

	if(m_srchtype == 0)
		((CButton*)GetDlgItem(IDC_CODECTRL1))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_CODECTRL2))->SetCheck(1);

	m_pbSlide = GetPrivateProfileInt("CHOKNTALK", "showslide", 1, file);
	if(m_pbSlide == 0)
		((CButton*)GetDlgItem(IDC_CHECK_PB))->SetCheck(0);
	else
		((CButton*)GetDlgItem(IDC_CHECK_PB))->SetCheck(1);
}

void CESetup::save_info()
{
	CString		file, value;

	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, SETUPFILE);
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

	value.Format("%d", m_screenNew);
	WritePrivateProfileString("SCREEN", "SCREENNEW", value, file);

	value.Format("%d", m_titleChange);
	WritePrivateProfileString("SCREEN", "TITLECHANGE", value, file);

	GetDlgItem(IDC_EDIT_TITLE)->GetWindowText(value);
	WritePrivateProfileString("SCREEN", "USERTITLE", value, file);

	value.Format("%d", m_unflesh);
	WritePrivateProfileString("SCREEN", "UNFLESH", value, file);

	value.Format("%d", m_startscreen);
	WritePrivateProfileString("SCREEN", "STARTSCREEN", value, file);

	if (((CButton *) GetDlgItem(IDC_VSCREENTRIGGER))->GetCheck() == BST_CHECKED)
		WritePrivateProfileString("SCREEN", "VS_REFLECT", "1", file);
	else	WritePrivateProfileString("SCREEN", "VS_REFLECT", "0", file);

	value.Format("%d", m_msgPos);
	WritePrivateProfileString("SCREEN", "MSGPOS", value, file);

	GetDlgItem(IDC_IDLETIME)->GetWindowText(value);
	WritePrivateProfileString("SCREEN", "IDLETIME", value, file);

	value.Format("%d", m_maintab);
	WritePrivateProfileString("SCREEN", "MAINTAB", value, file);

	value.Format("%d", m_msgbox);
	WritePrivateProfileString("SCREEN", "MSGBOX", value, file);

	//**int chk = ((CButton *) GetDlgItem(IDC_CERTIFY))->GetCheck();
	//**AfxGetMainWnd()->SendMessage(WM_QWAY, MAKEWPARAM(aqSetEPassOmit, 0), (LPARAM)chk);

	const BOOL bCheck = (((CButton*)GetDlgItem(IDC_CHK_HOTKEY))->GetCheck());
	WritePrivateProfileString("HOTKEY", "UseHideHotkey", bCheck ? "1" : "0", file);

	CString txt;
	GetDlgItem(IDC_CBN_HOTKEY)->GetWindowText(txt);
	WritePrivateProfileString("HOTKEY", "HideHotkey", txt, file);

	value.Format("%d",m_srchtype);
	WritePrivateProfileString("SCREEN", "CODECTRL", value, file);

	value.Format("%d",m_pbSlide);
	WritePrivateProfileString("CHOKNTALK", "showslide", value, file);
}

void CESetup::load_workspace()
{
	char		wb[256]{};
	DWORD		dw{};
	CString		file, keys, value, wsName;
	CStringArray	ary;
	int ii;  //vc2019

	/* 화면을 선택하지 않을 수 있도록 콤보박스에 공백을 하나 추가한다! 2010.06.14 by warship */
	ary.Add("");

	file.Format("%s\\user\\%s\\%s.ini", Axis::home, Axis::user, Axis::user);
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

		file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, SETUPFILE);
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
	UINT			nRes{};
	CString			name, keys, file, filex;
	CArray <UINT, UINT >	arRes;

	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, SETUPFILE);
	filex.Format("%s\\user\\%s\\%s.ini", Axis::home, Axis::user, Axis::user);
	arRes.Add(IDC_STARTV1);	arRes.Add(IDC_STARTV2);	arRes.Add(IDC_STARTV3);
	arRes.Add(IDC_STARTV4);	arRes.Add(IDC_STARTV5);	arRes.Add(IDC_STARTV6);

	WritePrivateProfileSection("STARTVSMAP", "", filex);
	
	for (int ii = 0, idx = 0; ii < arRes.GetSize(); ii++)
	{
		nRes = arRes.GetAt(ii);
		keys.Format("WS%02d", ii+1);
		idx = ((CComboBox*) GetDlgItem(nRes))->GetCurSel();
		if (idx == CB_ERR)	
		{
			name.Empty();
			WritePrivateProfileString("ETC", keys, name, file);
		}
		else
		{
			((CComboBox*) GetDlgItem(nRes))->GetLBText(idx, name);
			WritePrivateProfileString("ETC", keys, name, file);
		}
CString slog;
		if (!name.IsEmpty())
		{
			CString		tmps;
			char		wb[512]{};
			for (int jj = 1; ; jj++)
			{
				tmps.Format("%03d", jj);
				const DWORD dw = GetPrivateProfileString(name, tmps, "", wb, sizeof(wb), filex);
				if (dw <= 0)	break;
				keys.Format("%d%02d", ii, jj);
slog.Format("[startsave] [%d] key=[%s]  name=[%s]  tmp=[%s] wb=[%s]  \n", ii, keys, name, tmps, wb);
OutputDebugString(slog);
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
	char		buf[256]{};
	UINT		nRes{};

	file.Format("%s\\%s\\%s", Axis::home, MTBLDIR, SETUPFILE);

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

	m_screenNew = GetPrivateProfileInt("SCREEN", "SCREENNEW", 1, file);
	((CButton *) GetDlgItem(IDC_SCREEN_NEW))->SetCheck(m_screenNew);

	m_titleChange = GetPrivateProfileInt("SCREEN", "TITLECHANGE", 0, file);
	((CButton *) GetDlgItem(IDC_TITLE_CHANGE))->SetCheck(m_titleChange);

	m_startscreen = GetPrivateProfileInt("SCREEN", "STARTSCREEN", 1, file );
	((CButton *) GetDlgItem(IDC_START_SCREEN))->SetCheck(m_startscreen);

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

	m_msgbox = GetPrivateProfileInt("SCREEN", "MSGBOX", 1, file);
	((CButton *) GetDlgItem(IDC_MSGBOX))->SetCheck(m_msgbox); 

	((CButton *) GetDlgItem(IDC_CERTIFY))->SetCheck(0);
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

	return CSetupDialog::PreTranslateMessage(pMsg);
}

HBRUSH CESetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CSetupDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	const int ids = ::GetWindowLong(pWnd->m_hWnd, GWL_ID);
	if ( (ids >= IDC_STARTV1) && (ids <= IDC_STARTV6))
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
			pDC->BitBlt(76,0,96,20,&dc,0,0,SRCCOPY);
			dc.DeleteDC();
		}
	}
	if ( ids == IDC_STATIC_1 ) ///*(ids == IDC_SPIN1) || */(ids == IDC_SPIN2))
	{
		/*
		CRect rc;
		//pDC->SetBkMode(TRANSPARENT);
		GetDlgItem(ids)->GetClientRect(rc);
		rc.top+=6;
		rc.right -=20;

		CPen pn_in, pn_out;
		pn_in.CreatePen(PS_SOLID, 1, COLOR_INLINE);
		pn_out.CreatePen(PS_SOLID, 1, COLOR_OUTLINE);
		
		
		//CBrush br;
		
		//br.CreateSolidBrush(GetBkColor(::GetDC(pWnd->m_hWnd)));//GetBkColor(::GetDC(0)));
		pDC->SelectObject(pn_in);
		//pDC->SelectObject(br);
		pDC->MoveTo(rc.left, rc.top); pDC->LineTo(rc.right, rc.top); pDC->LineTo(rc.right, rc.bottom);
		pDC->LineTo(rc.left, rc.bottom); pDC->LineTo(rc.left, rc.top);
		//pDC->Rectangle(rc);
		*/	
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CESetup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	draw_groupBox(&dc, IDC_STATIC_1);
	draw_groupBox(&dc, IDC_STATIC_2);
	draw_groupBox(&dc, IDC_STATIC_3);
	draw_groupBox(&dc, IDC_STATIC_4);
	draw_groupBox(&dc, IDC_STATIC_5);
	draw_groupBox(&dc, IDC_STATIC_6);
	draw_groupBox(&dc, IDC_STATIC_7);
	draw_groupBox(&dc, IDC_STATIC_8);
	draw_groupBox(&dc, IDC_STATIC_9);	
	//draw_groupBox(&dc, IDC_STATIC_10);	

	// Do not call CSetupDialog::OnPaint() for painting messages
}

void CESetup::draw_groupBox(CPaintDC *pdc, int ids)
{
// TODO: Add your message handler code here
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

void CESetup::OnChkHotkey() 
{
	const BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHK_HOTKEY))->GetCheck();
	GetDlgItem(IDC_CBN_HOTKEY)->EnableWindow(bCheck);
}
