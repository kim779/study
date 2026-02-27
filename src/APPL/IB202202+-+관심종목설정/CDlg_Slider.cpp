// CDlg_Slider.cpp: 구현 파일
//

#include "StdAfx.h"
#include "IB202201.h"
#include "CDlg_Slider.h"
#include "afxdialogex.h"


// CDlg_Slider 대화 상자

IMPLEMENT_DYNAMIC(CDlg_Slider, CDialog)

#define DF_IMAX_RATE 6000 
#define DF_IMIN_RATE  300
#define DF_IMAX_POS 100
#define DF_IMIN_POS 30

CDlg_Slider::CDlg_Slider(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_SLIDER, pParent)
{

}

CDlg_Slider::~CDlg_Slider()
{
}

void CDlg_Slider::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slider);
}


BEGIN_MESSAGE_MAP(CDlg_Slider, CDialog)
	ON_BN_CLICKED(IDOK, &CDlg_Slider::OnBnClickedOk)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlg_Slider::OnCustomdrawSlider1)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDlg_Slider 메시지 처리기
unsigned int CDlg_Slider::IPToUInt(CString ip)
{
	int a{}, b{}, c{}, d{};
	unsigned int addr = 0;

	if (sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
		return 0;

	addr = a << 24;
	addr |= b << 16;
	addr |= c << 8;
	addr |= d;
	return addr;
}

CString CDlg_Slider::CheckIP()
{
	char szHostName[64] = { 0 };
	CString m_ip;
	CString tmps, ipAddr;

	::gethostname(szHostName, sizeof(szHostName));

	if (lstrcmp(szHostName, "") != 0)
	{
		HOSTENT FAR* lphostent = ::gethostbyname(szHostName);

		ipAddr = "";
		for (int ii = 0; lphostent; ii++)
		{
			if (!lphostent->h_addr_list[ii])
				break;
			sprintf(szHostName, "%u.%u.%u.%u",
				0xff & lphostent->h_addr_list[ii][0],
				0xff & lphostent->h_addr_list[ii][1],
				0xff & lphostent->h_addr_list[ii][2],
				0xff & lphostent->h_addr_list[ii][3]);
			ipAddr = szHostName;
		}
	}
	return ipAddr;
}

bool CDlg_Slider::isIPInRange(CString ip, CString network_s, CString network_e)
{
	const unsigned int ip_addr = IPToUInt(ip);
	const unsigned int net_lower = IPToUInt(network_s);
	const unsigned int net_upper = IPToUInt(network_e);

	if (ip_addr >= net_lower &&
		ip_addr <= net_upper)
		return true;

	return false;
}

void CDlg_Slider::LoadRate()
{
	CString filePath, strRate, userip;
	filePath.Format("%s/%s/InterOption.ini", m_root, "tab");

	userip = CheckIP();

	int iTime = 800;
	int ipos;
	if (!m_bCustomer) //직원용
	{
		iTime = GetPrivateProfileInt("STAFF", "cTIME", 300, filePath);
		ipos = ((iTime - DF_IMIN_RATE) / ((DF_IMAX_RATE - DF_IMIN_RATE) / (DF_IMAX_POS - DF_IMIN_POS))) + DF_IMIN_POS;
		m_slider.SetPos(ipos);
		return;
	}

	//고객용 HTS
	int icount = 0;
	iTime = GetPrivateProfileInt("CUSTOMER", "cTIME", 300, filePath);
	icount = GetPrivateProfileInt("IPLIST", "COUNT", 0, filePath);
	if (icount == 0)
	{
		ipos = ((iTime - DF_IMIN_RATE) / ((DF_IMAX_RATE - DF_IMIN_RATE) / (DF_IMAX_POS - DF_IMIN_POS))) + DF_IMIN_POS;
		m_slider.SetPos(ipos);
	}
	else
	{
		BOOL binip = false;
		char buf[1024]{};
		CString siplist, eiplist, sip, eip;
		for (int ii = 0; ii < icount; ii++)
		{
			siplist.Format("IP00%d_STT", ii + 1);
			eiplist.Format("IP00%d_END", ii + 1);

			memset(buf, 0x00, sizeof(buf));
			GetPrivateProfileString("IPLIST", siplist, "", buf, sizeof(buf), filePath);
			sip.Format("%s", buf);

			memset(buf, 0x00, sizeof(buf));
			GetPrivateProfileString("IPLIST", eiplist, "", buf, sizeof(buf), filePath);
			eip.Format("%s", buf);

			binip = isIPInRange(userip, sip, eip); //고객용 HTS 이지만 ip대역이 직원용피씨
			if (binip)
			{
				iTime = GetPrivateProfileInt("SCUSTOMER", "cTIME", 300, filePath);
				ipos = ((iTime - DF_IMIN_RATE) / ((DF_IMAX_RATE - DF_IMIN_RATE) / (DF_IMAX_POS - DF_IMIN_POS))) + DF_IMIN_POS;
				m_slider.SetPos(ipos);
				return;
			}
		}
	}

	//고객용이지만 IP가 직원용 대역이 아닌경우 (대부분 고객)
	// slog.Format("[2022]TIMER 고객용일반 ip=[%s] iTime=[%d]", userip, iTime);
	ipos = ((iTime - DF_IMIN_RATE) / ((DF_IMAX_RATE - DF_IMIN_RATE) / (DF_IMAX_POS - DF_IMIN_POS))) + DF_IMIN_POS;
	m_slider.SetPos(ipos);
}

void CDlg_Slider::SaveRate()
{
	int isliderPos = (((DF_IMAX_RATE - DF_IMIN_RATE) / (DF_IMAX_POS - DF_IMIN_POS)) * (m_slider.GetPos() - DF_IMIN_POS)) + DF_IMIN_RATE;
	int ipos = ((isliderPos - DF_IMIN_RATE) / ((DF_IMAX_RATE - DF_IMIN_RATE) / (DF_IMAX_POS - DF_IMIN_POS))) + DF_IMIN_POS;

	CString filePath, strRate, userip;
	filePath.Format("%s/%s/InterOption.ini", m_root, "tab");
	strRate.Format("%d", isliderPos);

	if (!m_bCustomer)
	{
		WritePrivateProfileString("STAFF", "cTIME", strRate, filePath);
		return;
	}

	int icount = 0;
	icount = GetPrivateProfileInt("IPLIST", "COUNT", 0, filePath);
	userip = CheckIP();

	BOOL binip = false;
	char buf[1024]{};
	CString siplist, eiplist, sip, eip;
	for (int ii = 0; ii < icount; ii++)
	{
		siplist.Format("IP00%d_STT", ii + 1);
		eiplist.Format("IP00%d_END", ii + 1);

		memset(buf, 0x00, sizeof(buf));
		GetPrivateProfileString("IPLIST", siplist, "", buf, sizeof(buf), filePath);
		sip.Format("%s", buf);

		memset(buf, 0x00, sizeof(buf));
		GetPrivateProfileString("IPLIST", eiplist, "", buf, sizeof(buf), filePath);
		eip.Format("%s", buf);

		binip = isIPInRange(userip, sip, eip); //고객용 HTS 이지만 ip대역이 직원용피씨
		if (binip)
		{
			WritePrivateProfileString("SCUSTOMER", "cTIME", strRate, filePath);
			// slog.Format("[2022]TIMER 고객용직원IP대역 ip=[%s]  iTime=[%d]", userip, iTime);
			return;
		}
	}

	WritePrivateProfileString("CUSTOMER", "cTIME", strRate, filePath);
}

void CDlg_Slider::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SaveRate();
	CDialog::OnOK();
}


void CDlg_Slider::OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//*pResult = 0;
	NMCUSTOMDRAW* pDraw = reinterpret_cast<NMCUSTOMDRAW*>(pNMHDR);
	switch (pDraw->dwDrawStage) {
	case CDDS_PREPAINT:
	{

		*pResult = CDRF_NOTIFYITEMDRAW; // 아이템 그리기 알림을 받겠다고 알림
	}
	break;

	case CDDS_ITEMPREPAINT:
	{
		CDC* pDC = CDC::FromHandle(pDraw->hdc);
		CRect rcItem = pDraw->rc;
		rcItem.DeflateRect(3, 3); // 내부 영역 크기 조절
		HWND hwnd = pDraw->hdr.hwndFrom;

		CString slog;
		slog.Format("\r\n[%x] [%d %d %d %d]", hwnd, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
		//	OutputDebugString(slog);

			// 내부 색칠
		if (pDraw->dwItemSpec == 3)
		{
			//CBrush brush;
			//brush.CreateSolidBrush(RGB(255, 0, 0));

			//int numSteps = 20; // 그라데이션 단계 수
			//int rStep = (0 - 255) / numSteps;
			//int gStep = (0 - 255) / numSteps;
			//int bStep = (255 - 0) / numSteps;

			//for (int i = numSteps + 1; i > 1; i--) {
			//	CRect currentRect(rcItem);
			//	currentRect.left += rcItem.right - (i * (rcItem.Width() / numSteps));
			//	currentRect.right = currentRect.left + (rcItem.Width() / numSteps);

			//	COLORREF fillColor = RGB(255, i * gStep, 255 - i * bStep);
			//	brush.DeleteObject();
			//	brush.CreateSolidBrush(fillColor);

			//	pDC->FillRect(&currentRect, &brush);
			//}

			//((CWnd*)GetDlgItem(IDC_EDIT_SLIDER))->SetWindowText(slog);
			pDC->FillSolidRect(rcItem, RGB(195, 195, 195));
		}
		else if (pDraw->dwItemSpec == 2)
		{
			pDC->FillSolidRect(rcItem, RGB(0, 122, 217));
			//((CWnd*)GetDlgItem(IDC_EDIT_SLIDER2))->SetWindowText(slog);
		}

		*pResult = CDRF_SKIPDEFAULT; // 기본 그리기 작업 건너뛰기
	}
	break;

	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
	{

		*pResult = CDRF_SKIPDEFAULT; // 기본 그리기 작업 건너뛰기
	}
	break;
	}
}


void CDlg_Slider::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	m_slider.SendMessage(NM_CUSTOMDRAW, 0, 0);
}


BOOL CDlg_Slider::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_slider.SetRange(30, 100, TRUE);
	LoadRate();

	if (CWnd* pUnit = GetDlgItem(IDC_STATIC_UNIT))
	{
		CRect rc;
		pUnit->GetWindowRect(&rc);
		ScreenToClient(&rc);

		// 1~2px 아래로 이동 (원하면 2로 바꿔라)
		const int offsetY = 0;
		rc.OffsetRect(0, offsetY);

		pUnit->MoveWindow(rc);
	}

	const int timeMs = PosToTimeMs(m_slider.GetPos());
	//m_iTime = timeMs;
	UpdateTimeText(timeMs);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

int CDlg_Slider::PosToTimeMs(int pos) const
{
	// pos 범위 보정
	if (pos < DF_IMIN_POS) pos = DF_IMIN_POS;
	if (pos > DF_IMAX_POS) pos = DF_IMAX_POS;

	const int posRange = (DF_IMAX_POS - DF_IMIN_POS);   // 70
	const int timeRange = (DF_IMAX_RATE - DF_IMIN_RATE);  // 5700

	// 정수 연산 반올림(중간값 더하기)
	const int timeMs = DF_IMIN_RATE + ((timeRange * (pos - DF_IMIN_POS) + posRange / 2) / posRange);
	return timeMs;
}

void CDlg_Slider::UpdateTimeText(int timeMs)
{
	CString s;
	if (timeMs <= DF_IMIN_RATE) // 300ms면 "실시간"
	{
		s = " 실시간";
		if (CWnd* p = GetDlgItem(IDC_STATIC_UNIT))
			p->SetWindowText("");
	}
	else
	{
		// 1) 소수 1자리 초 표기 (예: 800ms -> 0.8초, 1500ms -> 1.5초)
		const double sec = static_cast<double>(timeMs) / 1000.0;
		s.Format("%.1f", sec);
		if (CWnd* p = GetDlgItem(IDC_STATIC_UNIT))
			p->SetWindowText("초");
	}

	if (CWnd* p = GetDlgItem(IDC_STATIC_TIME))
		p->SetWindowText(s);

	// 디버그 로그 (필요하면 주석 해제/유지)
	// AxStd::_Msg("[%s] Slider pos=%d => time=%dms (%s)", __FUNCTION__, m_slider.GetPos(), timeMs, (LPCTSTR)s);
}

void CDlg_Slider::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar && pScrollBar->GetSafeHwnd() == m_slider.GetSafeHwnd())
	{
		const int pos = m_slider.GetPos();
		const int timeMs = PosToTimeMs(pos);

		// 현재 선택값을 멤버에 반영 (OK 누르면 SaveRate에서 다시 계산하지만,
		// 화면/로직에서 바로 쓰려면 여기서 갱신해두는 게 맞음)
		//m_iTime = timeMs;

		UpdateTimeText(timeMs);
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}