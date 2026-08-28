// MngSetup.cpp : implementation file
//

#include "stdafx.h"
#include "MngSetup.h"
#include "cpdefine.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define COLOR_INLINE	RGB(103,170,197)
#define COLOR_OUTLINE	RGB(255,255,255)

#define SIG_KRX_FREE_OPEN    854
#define SIG_KRX_FREE_CLOSE    855
#define SIG_KRX_AFTER_OPEN   856
#define SIG_KRX_AFTER_CLOSE  857
/////////////////////////////////////////////////////////////////////////////
// CMngSetup dialog
CMngSetup::CMngSetup(CWnd* pParent /*=NULL*/)
	: CSetupDialog(CMngSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMngSetup)
	//}}AFX_DATA_INIT

	m_caption = "장운영정보 설정";
	m_mapMng.RemoveAll();

	InitSignalControlMap();
}


void CMngSetup::DoDataExchange(CDataExchange* pDX)
{
	CSetupDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMngSetup)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMngSetup, CSetupDialog)
	//{{AFX_MSG_MAP(CMngSetup)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMngSetup message handlers

BOOL CMngSetup::OnInitDialog() 
{
	CSetupDialog::OnInitDialog();
	m_font.CreatePointFont(90, "굴림체");
	loadInfo();

	((CButton*)GetDlgItem(KOSPI_CLOSE_TIME_ONE))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(KOBA_ELW_CLOSE))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(KOSPI_CLOSE_TIME))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(KOSPI_OPEN_10S))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(KOSPI_CLOSE_10S))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(KOSPI_FREE))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(KOSPI_FREE_END))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(FUTURE_OPEN_10S))->ShowWindow(SW_HIDE);

	
	
	
	return TRUE;  
}

void CMngSetup::loadInfo()
{
	m_mapMng.RemoveAll();

	CString userFile, mtblFile;

	userFile.Format("%s\\%s\\%s\\mngsetup.ini",
		Axis::home, USRDIR, Axis::user);

	mtblFile.Format("%s\\%s\\mngsetup.ini",
		Axis::home, MTBLDIR);

	bool userExist =
		(GetFileAttributes(userFile) != INVALID_FILE_ATTRIBUTES);

	int userVer = 0;
	int mtblVer = GetPrivateProfileInt("MESSAGE", "INIT", 0, mtblFile);

	if (userExist)
		userVer = GetPrivateProfileInt("MESSAGE", "INIT", 0, userFile);

	CString useFile;

	if (!userExist)
	{
		CopyFile(mtblFile, userFile, FALSE);
		useFile = userFile;
	}
	else if (mtblVer > userVer)
	{
		WritePrivateProfileString("Manage", Format("%d", SIG_KRX_FREE_OPEN), "1", userFile);
		WritePrivateProfileString("Manage", Format("%d", SIG_KRX_AFTER_OPEN), "1", userFile);
		WritePrivateProfileString("Manage", Format("%d", SIG_KRX_AFTER_CLOSE), "1", userFile);

		CString ver;
		ver.Format("%d", mtblVer);
		WritePrivateProfileString("MESSAGE", "INIT", ver, userFile);

		useFile = userFile;
	}
	else
	{
		useFile = userFile;
	}

	// =========================
	// Manage 로딩
	// =========================
	TCHAR buf[4096]{};
	DWORD len = GetPrivateProfileSection("Manage", buf, 4096, useFile);

	if (len > 0)
	{
		TCHAR* p = buf;

		while (*p)
		{
			CString line = p;

			int eq = line.Find('=');
			if (eq > 0)
			{
				CString key = line.Left(eq);
				CString val = line.Mid(eq + 1);

				int semi = val.Find(';');
				if (semi >= 0)
					val = val.Left(semi);

				key.TrimRight();
				val.TrimRight();

				m_mapMng.SetAt(key, val);
				setControlValue(key, val);
			}

			p += _tcslen(p) + 1;
		}
	}

	// Setup 로딩
	m_pos = GetPrivateProfileInt("Setup", "Pos", 2, useFile);
	m_popup = GetPrivateProfileInt("Setup", "Popup", 0, useFile);
	m_sound = GetPrivateProfileInt("Setup", "Sound", 0, useFile);

	int nRes = -1;
	switch (m_pos)
	{
		case 0:
			nRes = RADIO_UPPERLEFT;
			break;
		case 2:
			nRes = RADIO_UPPERRIGHT;
			break;
		case 3:
			nRes = RADIO_BOTTOMLEFT;
			break;
		case 4:
			nRes = RADIO_BOTTOMRIGHT;
			break;
	}
	if (nRes != -1)
	{
		((CButton*)GetDlgItem(RADIO_UPPERLEFT))->SetCheck(FALSE);
		((CButton*)GetDlgItem(RADIO_UPPERRIGHT))->SetCheck(FALSE);
		((CButton*)GetDlgItem(RADIO_BOTTOMLEFT))->SetCheck(FALSE);
		((CButton*)GetDlgItem(RADIO_BOTTOMRIGHT))->SetCheck(FALSE);
		((CButton*)GetDlgItem(nRes))->SetCheck(TRUE);
	}

	((CButton*)GetDlgItem(IDC_POPUP))->SetCheck(m_popup);
	((CButton*)GetDlgItem(IDC_SOUND))->SetCheck(m_sound);

	showlog();
}

void CMngSetup::setControlValue(CString keys, CString value)
{
	UINT controlID{};
	int key = atoi(keys);

	if (m_signalMap.Lookup(key, controlID))
	{
		((CButton*)GetDlgItem(controlID))->SetCheck(atoi(value));
	}
}

void CMngSetup::saveInfo()
{
	CString userFile, mtblFile;

	userFile.Format("%s\\%s\\%s\\mngsetup.ini",
		Axis::home, USRDIR, Axis::user);

	mtblFile.Format("%s\\%s\\mngsetup.ini",
		Axis::home, MTBLDIR);

	// =========================
	// Manage 저장
	// =========================
	for (POSITION pos = m_mapMng.GetStartPosition(); pos;)
	{
		CString key, val;
		m_mapMng.GetNextAssoc(pos, key, val);
		WritePrivateProfileString("Manage", key, val, userFile);
	}
	
	// =========================
	// Setup 저장
	// =========================
	WritePrivateProfileString("Setup", "Pos", Format("%d", m_pos), userFile);
	WritePrivateProfileString("Setup", "Popup", Format("%d", m_popup), userFile);
	WritePrivateProfileString("Setup", "Sound", Format("%d", m_sound), userFile);

	// =========================
	// INIT version 동기화 (핵심)
	// =========================
	int mtblVer = GetPrivateProfileInt("MESSAGE", "INIT", 0, mtblFile);

	CString ver;
	ver.Format("%d", mtblVer);

	WritePrivateProfileString("MESSAGE", "INIT", ver, userFile);
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
	const UINT	nRes = LOWORD(wParam);
	CString value;
	int	chk = 0;

	int signal{};
	UINT controlID{};
	value.Empty();

	switch (nRes)
	{
	case RADIO_UPPERLEFT:
		m_pos = 0;	
		break;
	case RADIO_UPPERRIGHT:
		m_pos = 2;
		break;
	case RADIO_BOTTOMLEFT:
		m_pos = 3;	
		break;
	case RADIO_BOTTOMRIGHT:
		m_pos = 4;
		break;
	case IDC_POPUP:
		m_popup = ((CButton *)GetDlgItem(nRes))->GetCheck();
		break;
	case IDC_SOUND:
		m_sound = ((CButton *)GetDlgItem(nRes))->GetCheck();
		break;
	default:
		for (POSITION pos = m_signalMap.GetStartPosition(); pos; )
		{
			m_signalMap.GetNextAssoc(pos, signal, controlID);
		
			if (nRes == controlID)
			{
				chk = ((CButton*)GetDlgItem(nRes))->GetCheck();
				value.Format("%d", signal);
				break;
			}
		}
		break;
	}

	if (!value.IsEmpty())
	{
		if (chk)	m_mapMng.SetAt(value, "1");
		else		m_mapMng.SetAt(value, "0");
	}
	
	return CSetupDialog::OnCommand(wParam, lParam);
}

void CMngSetup::ApplySetup()
{
	saveInfo();
}

void CMngSetup::DefaultSetup()
{
	CString userFile, mtblFile;

	userFile.Format("%s\\%s\\%s\\mngsetup.ini",
		Axis::home, USRDIR, Axis::user);

	mtblFile.Format("%s\\%s\\mngsetup.ini",
		Axis::home, MTBLDIR);

	// =========================
	// mtbl Manage 로딩
	// =========================
	m_mapMng.RemoveAll();

	TCHAR buf[4096]{};
	DWORD len = GetPrivateProfileSection("Manage", buf, 4096, mtblFile);

	if (len > 0)
	{
		TCHAR* p = buf;

		while (*p)
		{
			CString line = p;

			int eq = line.Find('=');
			if (eq > 0)
			{
				CString key = line.Left(eq);
				CString val = line.Mid(eq + 1);

				// 주석 제거
				int semi = val.Find(';');
				if (semi >= 0)
					val = val.Left(semi);

				key.TrimRight();
				val.TrimRight();

				m_mapMng.SetAt(key, val);
				setControlValue(key, val);
			}

			p += _tcslen(p) + 1;
		}
	}

	// =========================
	// Setup 로딩 (mtbl 기준)
	// =========================
	m_pos = GetPrivateProfileInt("Setup", "Pos", 2, mtblFile);
	m_popup = GetPrivateProfileInt("Setup", "Popup", 0, mtblFile);
	m_sound = GetPrivateProfileInt("Setup", "Sound", 0, mtblFile);

	((CButton*)GetDlgItem(IDC_POPUP))->SetCheck(m_popup);
	((CButton*)GetDlgItem(IDC_SOUND))->SetCheck(m_sound);

	int nRes = -1;
	switch (m_pos)
	{
	case 0: nRes = RADIO_UPPERLEFT; break;
	case 2: nRes = RADIO_UPPERRIGHT; break;
	case 3: nRes = RADIO_BOTTOMLEFT; break;
	case 4: nRes = RADIO_BOTTOMRIGHT; break;
	}

	if (nRes != -1)
	{
		((CButton*)GetDlgItem(RADIO_UPPERLEFT))->SetCheck(FALSE);
		((CButton*)GetDlgItem(RADIO_UPPERRIGHT))->SetCheck(FALSE);
		((CButton*)GetDlgItem(RADIO_BOTTOMLEFT))->SetCheck(FALSE);
		((CButton*)GetDlgItem(RADIO_BOTTOMRIGHT))->SetCheck(FALSE);
		((CButton*)GetDlgItem(nRes))->SetCheck(TRUE);
	}

	// =========================
	// user 파일로 저장
	// =========================
	saveInfo();
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

	return CSetupDialog::PreTranslateMessage(pMsg);
}

void CMngSetup::InitSignalControlMap()
{
	m_signalMap.RemoveAll();

	struct Item { int sig; UINT cid; };

	static const Item items[] =
	{
		{851, KOSPI_BEFORE_HOGA},
		{801, KOSPI_OPEN},
		{21,  KOSPI_OPEN_10M},
		{26,  KOSPI_OPEN_5M},
		{30,  KOSPI_OPEN_1M},
		{35,  KOSPI_OPEN_10S},
		{852, KOSPI_AFTER_HOGA},
		{809, KOSPI_CLOSE},
		{126, KOSPI_CLOSE_5M},
		{130, KOSPI_CLOSE_1M},
		{135, KOSPI_CLOSE_10S},
		{804, KOSPI_OPEN_TIME},
		{853, KOSPI_CLOSE_TIME},
		{806, KOSPI_CLOSE_TIME_ONE},
		{846, KOBA_ELW_CLOSE},
		{871, KOSPI_OPEN_SIDECAR},
		{872, KOSPI_CLOSE_SIDECAR},
		{817, KOSPI_OPEN_CIRCUIT},
		{818, KOSPI_CLOSE_CIRCUIT},
		{SIG_KRX_FREE_OPEN, KOSPI_FREE},
		{SIG_KRX_FREE_CLOSE, KOSPI_FREE_END},
		{SIG_KRX_AFTER_OPEN, KOSPI_AFTER_START},
		{SIG_KRX_AFTER_CLOSE, KOSPI_AFTER_END},


		{56,  FUTURE_OPEN_5M},
		{60,  FUTURE_OPEN_1M},
		{65,  FUTURE_OPEN_10S},
		{862, FUTURE_AFTER_HOGA},
		{863, FUTURE_CLOSE},
		{837, FUTURE_OPEN_CIRCUIT},
		{838, FUTURE_CLOSE_CIRCUIT},
	};

	for (int i = 0; i < (int)(sizeof(items) / sizeof(items[0])); ++i)
		m_signalMap.SetAt(items[i].sig, items[i].cid);
}
//void CMngSetup::InitSignalControlMap()
//{
//	int oldsignals[] = { 610000,  110000 , 110600 , 110300,  110060, 110010, 710000, 210000,  210300 , 210060 ,
//								  210010, 100000, 200000, 310000, 410000,  510000 , 2610000,  2110000 , 2110600 ,2110300, 2110060,
//								  2110010 , 2710000 , 2210000 , 2100000 , 2200000 ,601196, 0 };
//
//	int signals[] = {
//	851, //610000,		//장전 동시호가 개시
//	801, //110000,		//장개시
//	21, //110600,		//장개시 10분전
//	26, //110300,		//장개시 5분전
//	30, //110060,		//장개시 1분전
//	35, //110010,		//장개시 10초전
//	852, //710000,       //장후 동시호개 개시
//	809, //210000,		//장마감
//	126, //210300,		//장마감 5분전
//	130, //210060,		//장마감 1분전
//	135, //210010,		//장마감 10초전
//	817, //100000,		//서킷브레이커발동
//	818, //200000,		//서킷브레이커해제
//	804, //310000,		//시간외종가 매매개시
//	853, //410000,		//시간외종가 매매종료, 시간외단일가 매매개시 
//	806, //510000,		//시간외단일가 매매종료
//	832, //2610000,		//선물/옵션 장전 동시호가 개시
//	861, //2110000,		//선물/옵션 장개시
//	2110600,		//선물/옵션 장개시 10분전
//	56, //2110300,		//선물/옵션 장개시 5분전
//	60, //2110060,		//선물/옵션 장개시 1분전
//	65, //2110010,		//선물/옵션 장개시 10초전
//	862, //2710000,		//선물/옵션 장후 동시호가 개시
//	863, //2210000,		//선물/옵션 장마감
//	837, //2100000,		//선물/옵션 서킷브레이커발동
//	838, //2200000,		//선물/옵션 서킷브레이커해제
//	846, //601196,		//KOBA ELW 조기종료
//	871,//					//사이드카발동
//	872,//					//사이드카해제    //29개
//	0
//	};
//	const UINT const controls[] = {
//		KOSPI_BEFORE_HOGA, 	//장전 동시호개 개시
//		KOSPI_OPEN,
//		KOSPI_OPEN_10M,
//		KOSPI_OPEN_5M,
//		KOSPI_OPEN_1M,
//		KOSPI_OPEN_10S,
//		KOSPI_AFTER_HOGA,
//		KOSPI_CLOSE,
//		KOSPI_CLOSE_5M,
//		KOSPI_CLOSE_1M,
//		KOSPI_CLOSE_10S,
//		KOSPI_OPEN_CIRCUIT,
//		KOSPI_CLOSE_CIRCUIT,
//		KOSPI_OPEN_TIME,
//		KOSPI_CLOSE_TIME,
//		KOSPI_CLOSE_TIME_ONE,
//		FUTURE_BEFORE_HOGA,
//		FUTURE_OPEN,
//		FUTURE_OPEN_10M,
//		FUTURE_OPEN_5M,
//		FUTURE_OPEN_1M,
//		FUTURE_OPEN_10S,
//		FUTURE_AFTER_HOGA,
//		FUTURE_CLOSE,
//		FUTURE_OPEN_CIRCUIT,
//		FUTURE_CLOSE_CIRCUIT,
//		KOBA_ELW_CLOSE,
//		KOSPI_OPEN_SIDECAR,
//		KOSPI_CLOSE_SIDECAR,   //29
//		0
//	};
//
//	for (int i = 0; signals[i]; i++)
//	{
//		m_signalMap.SetAt(signals[i], controls[i]);
//	}
//
//	int isize = sizeof(oldsignals) / sizeof(int);
//	CString sOri, sNew;                 //ref 기존심벌 신규변환을 위한
//	for (int ii = 0; ii < isize; ii++)
//	{
//		sOri.Format("%07d", oldsignals[ii]);
//		sNew.Format("%d", signals[ii]);
//		m_mapSymkey.SetAt(sOri, sNew);
//	}
//
//	isize = sizeof(controls) / sizeof(int);
//	for (int ii = 0; ii < isize; ii++)
//	{
//		sNew.Format("%d", signals[ii]);
//		m_mapSymkey.SetAt(sNew, sNew);
//	}
//}

void CMngSetup::draw_groupBox(CPaintDC *pdc, int ids)
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

void CMngSetup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	draw_groupBox(&dc, IDC_STATIC_10);
	draw_groupBox(&dc, IDC_STATIC_11);
	draw_groupBox(&dc, IDC_STATIC_12);
	draw_groupBox(&dc, IDC_STATIC_13);
	// Do not call CSetupDialog::OnPaint() for painting messages
}


void CMngSetup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case 1818:
	{
		((CButton*)GetDlgItem(KOSPI_OPEN))->SetCheck(atoi("1"));
		KillTimer(nIDEvent);
	}
	break;
	}
	CSetupDialog::OnTimer(nIDEvent);
}

void CMngSetup::Loadsetup()
{
	loadInfo();
}

