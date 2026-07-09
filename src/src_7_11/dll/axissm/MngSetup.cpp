// MngSetup.cpp : implementation file
//

#include "stdafx.h"
#include "MngSetup.h"
#include "smdefine.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMngSetup dialog


CMngSetup::CMngSetup(CString home, CWnd* pParent /*=NULL*/)
	: CDialog(CMngSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngSetup)
	//}}AFX_DATA_INIT
	m_home = home;
	CWinApp* app = AfxGetApp();
	m_user = app->GetProfileString(WORKSTATION, SIGNONID);
	int pos = m_user.Find('|');
	if (pos < 0)
		m_user = _T("Anonymous");

	else	
		m_user = m_user.Mid(pos+1);

	if (m_user.IsEmpty())
		m_user = _T("Anonymous");

	m_mapMng.RemoveAll();

	m_bApply = false;
}


void CMngSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngSetup)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngSetup, CDialog)
	//{{AFX_MSG_MAP(CMngSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngSetup message handlers

BOOL CMngSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	loadInfo();
	
	return TRUE;  
}

void CMngSetup::loadInfo()
{
	CString file;
	file.Format("%s\\%s\\%s\\mngsetup.ini", m_home, USRDIR, m_user);

	char	ssb[1024*4];
	DWORD ssL = GetPrivateProfileSection("Manage", ssb, sizeof(ssb), file);
	if (ssL <= 0)	
		return;

	CString subitem, keys, value, string = CString(ssb, ssL);
	for (; !string.IsEmpty(); )
	{
		int idx = string.Find('\0');
		if (idx == -1)	break;

		subitem  = string.Left(idx++);
		string = string.Mid(idx);

		idx = subitem.Find('=');
		if (idx == -1)	continue;

		keys = subitem.Left(idx++);
		value = subitem.Mid(idx);

		m_mapMng.SetAt(keys, value);
		setControlValue(keys, value);
	}

	m_pos = GetPrivateProfileInt("Setup", "Pos", 0, file);
	int	nRes;
	if (m_pos == 0)		nRes = RADIO_UPPERLEFT;
	else if (m_pos == 1)	nRes = RADIO_UPPERRIGHT;
	else if (m_pos == 2)	nRes = RADIO_BOTTOMLEFT;
	else if (m_pos == 3)	nRes = RADIO_BOTTOMRIGHT;
	((CButton *)GetDlgItem(nRes))->SetCheck(1);

	m_popup = GetPrivateProfileInt("Setup", "Popup", 0, file);
	((CButton *)GetDlgItem(IDC_POPUP))->SetCheck(m_popup);

	m_sound = GetPrivateProfileInt("Setup", "Sound", 0, file);
	((CButton *)GetDlgItem(IDC_SOUND))->SetCheck(m_sound);
}

void CMngSetup::setControlValue(CString keys, CString value)
{
	int nRes;
	switch (atoi(keys))
	{
	case 610000:
		nRes = KOSPI_BEFORE_HOGA;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 110000:
		nRes = KOSPI_OPEN;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 110600:
		nRes = KOSPI_OPEN_10M;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 110300:
		nRes = KOSPI_OPEN_5M;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 110060:
		nRes = KOSPI_OPEN_1M;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 110010:
		nRes = KOSPI_OPEN_10S;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 710000:
		nRes = KOSPI_AFTER_HOGA;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 210000:
		nRes = KOSPI_CLOSE;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 210300:
		nRes = KOSPI_CLOSE_5M;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 210060:
		nRes = KOSPI_CLOSE_1M;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 210010:
		nRes = KOSPI_CLOSE_10S;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 100000:
		nRes = KOSPI_OPEN_CIRCUIT;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 200000:
		nRes = KOSPI_CLOSE_CIRCUIT;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 310000:
		nRes = KOSPI_OPEN_TIME;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 410000:
		nRes = KOSPI_CLOSE_TIME;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 510000:
		nRes = KOSPI_CLOSE_TIME_ONE;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 2610000:
		nRes = FUTURE_BEFORE_HOGA;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 2110000:
		nRes = FUTURE_OPEN;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 2110600:
		nRes = FUTURE_OPEN_10M;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 2110300:
		nRes = FUTURE_OPEN_5M;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 2110060:
		nRes = FUTURE_OPEN_1M;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 2110010:
		nRes = FUTURE_OPEN_10S;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 2710000:
		nRes = FUTURE_AFTER_HOGA;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 2210000:
		nRes = FUTURE_CLOSE;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 2100000:
		nRes = FUTURE_OPEN_CIRCUIT;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	case 2200000:
		nRes = FUTURE_CLOSE_CIRCUIT;
		((CButton *)GetDlgItem(nRes))->SetCheck(atoi(value));
		break;
	}
}

void CMngSetup::saveInfo()
{
	CString file, keys, value;
	file.Format("%s\\%s\\%s\\mngsetup.ini", m_home, USRDIR, m_user);

	for (POSITION pos = m_mapMng.GetStartPosition(); pos; )
	{
		m_mapMng.GetNextAssoc(pos, keys, value);
		
		WritePrivateProfileString("Manage", keys, value, file);
	}
	
	value.Format("%d", m_pos);
	WritePrivateProfileString("Setup", "Pos", value, file);

	value.Format("%d", m_popup);
	WritePrivateProfileString("Setup", "Popup", value, file);

	value.Format("%d", m_sound);
	WritePrivateProfileString("Setup", "Sound", value, file);
}
/*
{ 'S', {{ 47,  "GUBN"}, { 600, "BELLCODE"    }, { 601, "KEYVALUE"    },
               { 23,  "메시지" },
047	1	600	000	601	2110600	023	선물옵션 장개시10 분전"}
XJANG	047	1	600	000	601	110300	023	주식 장개시5 분전"}
KEYVALUE : XJANG 고정

GUBN : 0:장운영정보, 1:알림
BELLCODE : wave 파일

KEYVALUE :								*/
/* janinfo.keyv = XYZnnnn						*/
/* X									*/
/*	0:주식시장      1:KOSDAQ시장    2:선물시장      3:옵션시장      */
/*      4:하이일드펀드  5:프리보드      6:채권          7:주식옵션      */
/* Y									*/
/*	if z == 0  (장운영정보)						*/
/*	1:C.B 발동	2:C.B 해제					*/
/*	if z == 1  (알림메시지)						*/
/*	1:장개시전      2:장마감        3:시간외종가    4:시간외단일가  */
/*      5:시간외단일가 매매종료         6:장전동시호가  7:장후동시호가  */
/* Z    0:cont[40]      1:xmsg[60]                                      */
/* nnnn seconds (장개시전과 장마감전 인경우에만 사용)                   */

BOOL CMngSetup::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	UINT	nRes = LOWORD(wParam);
	CString value, x, y, z, nnnn;
	int	chk = 0;

	switch (nRes)
	{
	case KOSPI_BEFORE_HOGA:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "6";	z = "1";	nnnn = "0000";
		break;
	case KOSPI_OPEN:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "1";	z = "1";	nnnn = "0000";
		break;
	case KOSPI_OPEN_10M:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "1";	z = "1";	nnnn = "0600";
		break;
	case KOSPI_OPEN_5M:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "1";	z = "1";	nnnn = "0300";
		break;
	case KOSPI_OPEN_1M:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "1";	z = "1";	nnnn = "0060";
		break;
	case KOSPI_OPEN_10S:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "1";	z = "1";	nnnn = "0010";
		break;
	case KOSPI_AFTER_HOGA:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "7";	z = "1";	nnnn = "0000";
		break;
	case KOSPI_CLOSE:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "2";	z = "1";	nnnn = "0000";
		break;
	case KOSPI_CLOSE_5M:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "2";	z = "1";	nnnn = "0300";
		break;
	case KOSPI_CLOSE_1M:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "2";	z = "1";	nnnn = "0060";
		break;
	case KOSPI_CLOSE_10S:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "2";	z = "1";	nnnn = "0010";
		break;
	case KOSPI_OPEN_CIRCUIT:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "1";	z = "0";	nnnn = "0000";
		break;
	case KOSPI_CLOSE_CIRCUIT:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "2";	z = "0";	nnnn = "0000";
		break;
	case KOSPI_OPEN_TIME:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "3";	z = "1";	nnnn = "0000";
		break;
	case KOSPI_CLOSE_TIME:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "4";	z = "1";	nnnn = "0000";
		break;
	case KOSPI_CLOSE_TIME_ONE:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "0";	y = "5";	z = "1";	nnnn = "0000";
		break;

	case FUTURE_BEFORE_HOGA:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "2";	y = "6";	z = "1";	nnnn = "0000";
		break;
	case FUTURE_OPEN:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "2";	y = "1";	z = "1";	nnnn = "0000";
		break;
	case FUTURE_OPEN_10M:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "2";	y = "1";	z = "1";	nnnn = "0600";
		break;
	case FUTURE_OPEN_5M:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "2";	y = "1";	z = "1";	nnnn = "0300";
		break;
	case FUTURE_OPEN_1M:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "2";	y = "1";	z = "1";	nnnn = "0060";
		break;
	case FUTURE_OPEN_10S:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "2";	y = "1";	z = "1";	nnnn = "0010";
		break;
	case FUTURE_AFTER_HOGA:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "2";	y = "7";	z = "1";	nnnn = "0000";
		break;
	case FUTURE_CLOSE:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "2";	y = "2";	z = "1";	nnnn = "0000";
		break;
	case FUTURE_OPEN_CIRCUIT:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "2";	y = "1";	z = "0";	nnnn = "0000";
		break;
	case FUTURE_CLOSE_CIRCUIT:
		m_bApply = true;
		chk = ((CButton *)GetDlgItem(nRes))->GetCheck();
		x = "2";	y = "2";	z = "0";	nnnn = "0000";
		break;
	case RADIO_UPPERLEFT:
		m_bApply = true;
		m_pos = 0;	
		break;
	case RADIO_UPPERRIGHT:
		m_bApply = true;
		m_pos = 1;
		break;
	case RADIO_BOTTOMLEFT:
		m_bApply = true;
		m_pos = 2;	
		break;
	case RADIO_BOTTOMRIGHT:
		m_bApply = true;
		m_pos = 3;
		break;
	case IDC_POPUP:
		m_bApply = true;
		m_popup = ((CButton *)GetDlgItem(nRes))->GetCheck();
		break;
	case IDC_SOUND:
		m_bApply = true;
		m_sound = ((CButton *)GetDlgItem(nRes))->GetCheck();
		break;
	}

	if (!x.IsEmpty())
	{
		value.Format("%s%s%s%s", x, y, z, nnnn);
		if (chk)	m_mapMng.SetAt(value, "1");
		else		m_mapMng.SetAt(value, "0");
	}
	
	if (m_bApply)		GetParent()->SendMessage(WM_SETUP, (WPARAM)this, 0);

	return CDialog::OnCommand(wParam, lParam);
}

void CMngSetup::ApplySetup()
{
	saveInfo();
}

void CMngSetup::DefaultSetup()
{
	CString file;
	file.Format("%s\\%s\\mngsetup.ini", m_home, MTBLDIR);

	char	ssb[1024*4];
	DWORD ssL = GetPrivateProfileSection("Manage", ssb, sizeof(ssb), file);
	if (ssL <= 0)		return;

	m_mapMng.RemoveAll();

	CString subitem, keys, value, string = CString(ssb, ssL);
	for (; !string.IsEmpty(); )
	{
		int idx = string.Find('\0');
		if (idx == -1)	break;

		subitem  = string.Left(idx++);
		string = string.Mid(idx);

		idx = subitem.Find('=');
		if (idx == -1)	continue;

		keys = subitem.Left(idx++);
		value = subitem.Mid(idx);

		m_mapMng.SetAt(keys, value);
		setControlValue(keys, value);
	}

	m_pos = GetPrivateProfileInt("Setup", "Pos", 0, file);
	((CButton *)GetDlgItem(RADIO_UPPERLEFT))->SetCheck(0);
	((CButton *)GetDlgItem(RADIO_UPPERRIGHT))->SetCheck(0);
	((CButton *)GetDlgItem(RADIO_BOTTOMLEFT))->SetCheck(0);
	((CButton *)GetDlgItem(RADIO_BOTTOMRIGHT))->SetCheck(0);
	int	nRes;
	if (m_pos == 0)		nRes = RADIO_UPPERLEFT;
	else if (m_pos == 1)	nRes = RADIO_UPPERRIGHT;
	else if (m_pos == 2)	nRes = RADIO_BOTTOMLEFT;
	else if (m_pos == 3)	nRes = RADIO_BOTTOMRIGHT;
	((CButton *)GetDlgItem(nRes))->SetCheck(1);

	m_popup = GetPrivateProfileInt("Setup", "Popup", 0, file);
	((CButton *)GetDlgItem(IDC_POPUP))->SetCheck(m_popup);

	m_sound = GetPrivateProfileInt("Setup", "Sound", 0, file);
	((CButton *)GetDlgItem(IDC_SOUND))->SetCheck(m_sound);
}

BOOL CMngSetup::PreTranslateMessage(MSG* pMsg) 
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
