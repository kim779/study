// CMNG_NXT.cpp: 구현 파일
//

#include "StdAfx.h"
#include "CMNG_NXT.h"
//#include "afxdialogex.h"

// CMNG_NXT 대화 상자

#define FREEMK_OPEN     "881"     //프리마켓 개시
#define FREEMK_CLOSE    "882"     //프리마켓 마감
#define MAINMK_OPEN    "884"     //메인마켓 개시
#define MAINMK_CLOSE   "885"     //메인마켓 마감
#define AFTMK_OPEN		   "888"     //애프터마켓 개시
#define AFTMK_CLOSE	   "889"     //애프터마켓 마감
#define AFTMK_SIGADAN	   "887"     //애프터마켓 시가단일가 개시


//#define MRN_OFF			   "883"     //오전 휴장
//#define AFT_OFF				   "886"     //오후 휴장

CMNG_NXT::CMNG_NXT(CWnd* pParent /*=nullptr*/)
	: CSetupDialog(IDD_MNG_NXT, pParent)
{

}

CMNG_NXT::~CMNG_NXT()
{
}

void CMNG_NXT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMNG_NXT, CSetupDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMNG_NXT 메시지 처리기


void CMNG_NXT::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CSetupDialog::OnPaint()을(를) 호출하지 마십시오.
	draw_groupBox(&dc, IDC_STATIC_NXT);
	draw_groupBox(&dc, IDC_STATIC_NXT2);
	draw_groupBox(&dc, IDC_STATIC_NXT3);
}


BOOL CMNG_NXT::OnInitDialog()
{
	CSetupDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_font.CreatePointFont(90, "굴림체");
	loadInfo();

	m_bitmapBtn = std::make_unique<CBmpButton>();
	CRect rc;
	((CWnd*)GetDlgItem(IDC_STATIC_TOOL))->GetWindowRect(&rc);
	ScreenToClient(rc);

	//rc.OffsetRect(10, 0);

	CString file;
	CString strtooltip, stime1, stime2, stime3, stime4, stime5, stime6, stime7, stime8;
	file.Format("%s\\%s\\%s", Axis::home, TABDIR, "AXNXT.INI");

	char	ssb[32]{};
	DWORD ssL = GetPrivateProfileString("NXT_TIME", "free_start", "", ssb, sizeof(ssb), file);
	stime1.Format("%s", ssb); stime1.TrimRight();
	ssL = GetPrivateProfileString("NXT_TIME", "free_end", "", ssb, sizeof(ssb), file);
	stime2.Format("%s", ssb); stime2.TrimRight();

	ssL = GetPrivateProfileString("NXT_TIME", "main_start", "", ssb, sizeof(ssb), file);
	stime3.Format("%s", ssb); stime3.TrimRight();
	ssL = GetPrivateProfileString("NXT_TIME", "main_end", "", ssb, sizeof(ssb), file);
	stime4.Format("%s", ssb); stime4.TrimRight();

	ssL = GetPrivateProfileString("NXT_TIME", "after_si_start", "", ssb, sizeof(ssb), file);
	stime5.Format("%s", ssb); stime5.TrimRight();
	ssL = GetPrivateProfileString("NXT_TIME", "after_si_end", "", ssb, sizeof(ssb), file);
	stime6.Format("%s", ssb); stime6.TrimRight();

	ssL = GetPrivateProfileString("NXT_TIME", "after_start", "", ssb, sizeof(ssb), file);
	stime7.Format("%s", ssb); stime7.TrimRight();
	ssL = GetPrivateProfileString("NXT_TIME", "after_end", "", ssb, sizeof(ssb), file);
	stime8.Format("%s", ssb); stime8.TrimRight();



	strtooltip.Format("   \nNXT 장운영 시간\n- 프리마켓 : %s~%s\n- 메인마켓 : %s~%s\n- 애프터마켓 시가단일가\n%s~%s\n-애프터마켓 : %s~%s", 
		stime1, stime2, stime3, stime4, stime5, stime6, stime7, stime8);
	if (!m_bitmapBtn->Create(strtooltip, BS_BITMAP, rc, this, 9898))
	{
		//delete m_bitmapBtn;
		m_bitmapBtn.release();
		m_bitmapBtn = NULL;
	}

	
	file.Format("%s\\%s\\%s", Axis::home, IMAGEDIR, "NXTQS.bmp");

	HBITMAP hBmp = (HBITMAP)::LoadImage(
		NULL,
		file,
		IMAGE_BITMAP,
		0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	if (hBmp)
		m_bitmapTooltip.Attach(hBmp);        // CBitmap에 Attach

	m_bitmapBtn->SetImgBitmap(&m_bitmapTooltip, 1);
	//m_bitmapBtn->SetImgBitmap(Axis::GetSkinBitmap("EQUALIZER"), 2);

	//IDC_CHK_MAIN_OPEN
	((CButton*)GetDlgItem(IDC_CHK_MAIN_OPEN))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHK_MAIN_CLOSE))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_CHK_MAIN_OPEN))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHK_MAIN_CLOSE))->SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMNG_NXT::loadInfo()
{
	CString stmp;
	CString file;
	file.Format("%s\\%s\\%s\\mngsetup.ini", Axis::home, USRDIR, Axis::user);

	//유저폴더의  mngsetup.ini 에 nxt 장운영 정보가 있다면 mtbl 의 default를 사용안하고
	//mtbl 의 default를 사용했다면 아래에서 데이터 없어서 mtbl이 유지된다.
	char	ssb[1024 * 4];
	const DWORD ssL = GetPrivateProfileSection("NXT", ssb, sizeof(ssb), file);
	
	CString slog;
	slog.Format("[MNGSETUP][NXT] user ssL = %d ", ssL);
	OutputDebugString(slog);

	if (ssL <= 0)
	{
		DefaultSetup();
		return;
	}

	CString subitem, keys, value, string = CString(ssb, ssL);
	for (; !string.IsEmpty(); )
	{
		int idx = string.Find('\0');
		if (idx == -1)	break;

		subitem = string.Left(idx++);
		string = string.Mid(idx);

		idx = subitem.Find('=');
		if (idx == -1)	continue;

		keys = subitem.Left(idx++);
		value = subitem.Mid(idx);

		setControlValue(keys, value);
		m_mapMng.SetAt(keys, value);
	}

	m_pos = GetPrivateProfileInt("Setup", "Pos", 2, file);
	m_sound = GetPrivateProfileInt("Setup", "Sound", 0, file);

	((CButton*)GetDlgItem(RADIO_NXT_LEFTTOP))->SetCheck(0);
	((CButton*)GetDlgItem(RADIO_NXT_RIGHTTOP))->SetCheck(0);
	((CButton*)GetDlgItem(RADIO_NXT_LEFTBOTTOM))->SetCheck(0);
	((CButton*)GetDlgItem(RADIO_NXT_RIGHTBOTTOM))->SetCheck(0);

	switch (m_pos)
	{
		case 0:
			((CButton*)GetDlgItem(RADIO_NXT_LEFTTOP))->SetCheck(1);
			break;
		case 2:
			((CButton*)GetDlgItem(RADIO_NXT_RIGHTTOP))->SetCheck(1);
			break;
		case 3:
			((CButton*)GetDlgItem(RADIO_NXT_LEFTBOTTOM))->SetCheck(1);
			break;
		case 4:
			((CButton*)GetDlgItem(RADIO_NXT_RIGHTBOTTOM))->SetCheck(1);
			break;
	}

	((CButton*)GetDlgItem(IDC_CHK_SOUNDAVA))->SetCheck(m_sound);
}

void CMNG_NXT::DefaultSetup()
{
	CString file, stmp;
	char	ssb[1024 * 4]{};
	file.Format("%s\\%s\\mngsetup.ini", Axis::home, MTBLDIR);
	memset(ssb, 0x00, 1024 * 4);
	const DWORD ssL = GetPrivateProfileSection("NXT", ssb, sizeof(ssb), file);
	if (ssL <= 0)		return;

	m_mapMng.RemoveAll();

	CString subitem, keys, value, string = CString(ssb, ssL);
	for (; !string.IsEmpty(); )
	{
		int idx = string.Find('\0');
		if (idx == -1)	break;

		subitem = string.Left(idx++);
		string = string.Mid(idx);

		idx = subitem.Find('=');
		if (idx == -1)	continue;

		keys = subitem.Left(idx++);
		value = subitem.Mid(idx);

		setControlValue(keys, value);

		m_mapMng.SetAt(keys, value);
	}

	((CButton*)GetDlgItem(RADIO_NXT_RIGHTTOP))->SetCheck(1);
	((CButton*)GetDlgItem(RADIO_NXT_LEFTTOP))->SetCheck(0);
	((CButton*)GetDlgItem(RADIO_NXT_LEFTBOTTOM))->SetCheck(0);
	((CButton*)GetDlgItem(RADIO_NXT_RIGHTBOTTOM))->SetCheck(0);

	((CButton*)GetDlgItem(IDC_CHK_SOUNDAVA))->SetCheck(FALSE);
}

void CMNG_NXT::setControlValue(CString keys, CString value)
{
	if (keys == FREEMK_OPEN)				//프리마켓 개시
		((CButton*)GetDlgItem(IDC_CHK_FREE_OPEN))->SetCheck(atoi(value));
	else if (keys == FREEMK_CLOSE)     //프리마켓 마감
		((CButton*)GetDlgItem(IDC_CHK_FREE_CLOSE))->SetCheck(atoi(value));
	else if (keys == MAINMK_OPEN)		  //메인마켓 개시	
		((CButton*)GetDlgItem(IDC_CHK_MAIN_OPEN))->SetCheck(atoi(value));
	else if (keys == MAINMK_CLOSE)		  //메인마켓 마감
		((CButton*)GetDlgItem(IDC_CHK_MAIN_CLOSE))->SetCheck(atoi(value));
	else if (keys == AFTMK_OPEN)		//애프터마켓 개시
		((CButton*)GetDlgItem(IDC_CHK_AFT_OPEN))->SetCheck(atoi(value));
	else if (keys == AFTMK_CLOSE)		//애프터마켓 마감
		((CButton*)GetDlgItem(IDC_CHK_AFT_CLOSE))->SetCheck(atoi(value));
	else if (keys == AFTMK_SIGADAN)		//애프터마켓 시가단일가 개시
		((CButton*)GetDlgItem(IDC_CHK_SIGADAN))->SetCheck(atoi(value));
}

void CMNG_NXT::saveInfo()
{
	CString file, keys, value;
	file.Format("%s\\%s\\%s\\mngsetup.ini", Axis::home, USRDIR, Axis::user);

	for (POSITION pos = m_mapMng.GetStartPosition(); pos; )
	{
		m_mapMng.GetNextAssoc(pos, keys, value);

		WritePrivateProfileString("NXT", keys, value, file);
	}

	WritePrivateProfileString("MESSAGE", "INIT", "4", file);

	if(((CButton*)GetDlgItem(RADIO_NXT_LEFTTOP))->GetCheck())
		m_pos = 0;
	else if(((CButton*)GetDlgItem(RADIO_NXT_RIGHTTOP))->GetCheck())
		m_pos = 2;
	else if(((CButton*)GetDlgItem(RADIO_NXT_LEFTBOTTOM))->GetCheck())
		m_pos = 3;
	else if(((CButton*)GetDlgItem(RADIO_NXT_RIGHTBOTTOM))->GetCheck())
		m_pos = 4;

	WritePrivateProfileString("Setup", "Pos", Format("%d", m_pos), file);

	m_sound = ((CButton*)GetDlgItem(IDC_CHK_SOUNDAVA))->GetCheck();
	WritePrivateProfileString("Setup", "Sound", Format("%d", m_sound), file);
}

void CMNG_NXT::ApplySetup()
{
	saveInfo();
}

BOOL CMNG_NXT::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	const UINT	nRes = LOWORD(wParam);
	CString value;
	int	chk = 0;

	UINT controlID{};
	value.Empty();

	chk = ((CButton*)GetDlgItem(nRes))->GetCheck();

	if (nRes == IDC_CHK_FREE_OPEN)				//프리마켓 개시
		value.Format("%s", FREEMK_OPEN);
	else if (nRes == IDC_CHK_FREE_CLOSE)     //프리마켓 마감
		value.Format("%s", FREEMK_CLOSE);
	else if (nRes == IDC_CHK_MAIN_OPEN)		  //메인마켓 개시	
		value.Format("%s", MAINMK_OPEN);
	else if (nRes == IDC_CHK_MAIN_CLOSE)		  //메인마켓 마감
		value.Format("%s", MAINMK_CLOSE);
	else if (nRes == IDC_CHK_AFT_OPEN)		//애프터마켓 개시
		value.Format("%s", AFTMK_OPEN);
	else if (nRes == IDC_CHK_AFT_CLOSE)		//애프터마켓 마감
		value.Format("%s", AFTMK_CLOSE);
	else if (nRes == IDC_CHK_SIGADAN)		//애프터마켓 마감
		value.Format("%s", AFTMK_SIGADAN);

	if (!value.IsEmpty())
	{
		if (chk)
			m_mapMng.SetAt(value, "1");
		else
			m_mapMng.SetAt(value, "0");
	}

	if (nRes == RADIO_NXT_LEFTTOP)
	{
		((CButton*)GetDlgItem(RADIO_NXT_RIGHTTOP))->SetCheck(0);
		((CButton*)GetDlgItem(RADIO_NXT_LEFTBOTTOM))->SetCheck(0);
		((CButton*)GetDlgItem(RADIO_NXT_RIGHTBOTTOM))->SetCheck(0);
	}
	else if (nRes == RADIO_NXT_RIGHTTOP)
	{
		((CButton*)GetDlgItem(RADIO_NXT_LEFTTOP))->SetCheck(0);
		((CButton*)GetDlgItem(RADIO_NXT_LEFTBOTTOM))->SetCheck(0);
		((CButton*)GetDlgItem(RADIO_NXT_RIGHTBOTTOM))->SetCheck(0);
	}
	else if (nRes == RADIO_NXT_LEFTBOTTOM)
	{
		((CButton*)GetDlgItem(RADIO_NXT_LEFTTOP))->SetCheck(0);
		((CButton*)GetDlgItem(RADIO_NXT_RIGHTTOP))->SetCheck(0);
		((CButton*)GetDlgItem(RADIO_NXT_RIGHTBOTTOM))->SetCheck(0);
	}
	else if (nRes == RADIO_NXT_RIGHTBOTTOM)
	{
		((CButton*)GetDlgItem(RADIO_NXT_LEFTTOP))->SetCheck(0);
		((CButton*)GetDlgItem(RADIO_NXT_RIGHTTOP))->SetCheck(0);
		((CButton*)GetDlgItem(RADIO_NXT_LEFTBOTTOM))->SetCheck(0);
	}


	return CSetupDialog::OnCommand(wParam, lParam);
}

void CMNG_NXT::draw_groupBox(CPaintDC* pdc, int ids)
{
	pdc->SelectObject(m_font);
	CRect rc, out, pout;
	GetDlgItem(ids)->GetClientRect(rc);
	GetDlgItem(ids)->GetWindowRect(out);
	GetWindowRect(pout);
	out.InflateRect(pout);

	rc.top += 6;
	//rc.right -=20;
	CPen pn_in, pn_out;
	pn_in.CreatePen(PS_SOLID, 1, Axis::GetColor(38));
	pn_out.CreatePen(PS_SOLID, 1, Axis::GetColor(76));
	rc.OffsetRect(out.left, out.top);
	rc.OffsetRect(1, 1);

	pdc->SelectObject(pn_out);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);
	rc.OffsetRect(-1, -1);
	pdc->SelectObject(pn_in);
	pdc->MoveTo(rc.left, rc.top); pdc->LineTo(rc.right, rc.top); pdc->LineTo(rc.right, rc.bottom);
	pdc->LineTo(rc.left, rc.bottom); pdc->LineTo(rc.left, rc.top);

	pdc->SetBkColor(Axis::dialogBgColor);
	CString data;
	GetDlgItem(ids)->GetWindowText(data);
	pdc->TextOut(rc.left + 7, rc.top - 5, data);
}

void CMNG_NXT::Loadsetup()
{
	loadInfo();
}