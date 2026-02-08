	ON_WM_TIMER()// ExitD.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "ExitD.h"
#include "../dll/sm/EnBitmap.h"

#define IDT_BANNER_EXIT		1008
#define IDC_SHAPE1		1001

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExitD dialog
#pragma warning(disable : 26400)
#pragma warning(disable : 26409)

CExitD::CExitD(CWnd* pParent /*=NULL*/, BOOL bNew)
: CDialog(bNew?CExitD::IDD_NEW:CExitD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExitD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bitmap  = NULL;
	m_bitmapEvent = (HBITMAP) NULL;
	m_bNew    = bNew;

	m_btnOK = m_btnCancel = NULL;
	m_bmpEvent = NULL;

	CString PathConf;
	PathConf.Format("%s\\%s\\CERT.INI", Axis::home, "tab");
	
	m_bUseNewLogin = GetPrivateProfileInt("CERTLOGIN","USE",0,PathConf);
}

CExitD::~CExitD()
{
	m_font.DeleteObject();
	if (m_bitmap)
		DeleteObject(m_bitmap);
	if (m_bitmapEvent)
		DeleteObject(m_bitmapEvent);
	if (m_bmpEvent)
	{
		m_bmpEvent->DestroyWindow();
		delete m_bmpEvent;
	}
}

void CExitD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExitD)
	DDX_Control(pDX, IDC_DBITMAP, m_static);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExitD, CDialog)
	//{{AFX_MSG_MAP(CExitD)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExitD message handlers
BOOL CExitD::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString	fontName;
	fontName.LoadString(ST_FONTNAME_DEFAULT2);

	m_fontBold.CreateFont(12, 0, 0, 0, FW_BOLD, FALSE,
			false, false, HANGEUL_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, fontName);

	m_font.CreatePointFont(90, fontName);
	
	if(m_bUseNewLogin)
	{
		if (Axis::isCustomer)
			m_bitmap = Axis::GetBitmap("NEW_LOGOUT_BG_BAGIC_CLOUDE");
		else
			m_bitmap = Axis::GetBitmap("NEW_STAFF_LOGOUT_BG_BAGIC_CLOUDE");
		//test
		/*
		HBITMAP hBitmap;
		CString bmpName, fileName;
		bmpName = "NEW_LOGOUT_BG_BAGIC"; 
		CString path(bmpName);
		path.MakeUpper();
		if (path.Find(".BMP") < 0)
			fileName.Format("%s\\image\\%s.bmp", Axis::home, bmpName);
		else
			fileName = path;

		hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), fileName,
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if (hBitmap)
		{
			//TRACE("Loaded File Bitmap NAME = %s\n", bmpName);
			TRACE("SUCCESS !!!!! = %s\n", bmpName);
			CBitmap* bmp = new CBitmap;
			bmp->Attach(hBitmap);
			m_bitmap = bmp;
		}
		//test
		*/
	}
	else
	{
		if (Axis::isCustomer)
			m_bitmap = Axis::GetBitmap("LOGOUT_BG_BAGIC");
		else
			m_bitmap = Axis::GetBitmap("STAFF_LOGOUT_BG_BAGIC");
	}
	ASSERT(m_bitmap);

	BITMAP bm;
	m_bitmap->GetBitmap(&bm);
	SetWindowPos(NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE | SWP_NOZORDER);
	
	HRGN hWndRgn = DIBToRgn((HBITMAP)m_bitmap->m_hObject, Axis::maskColor, FALSE);
	if(hWndRgn)
	{
		SetWindowRgn(hWndRgn,TRUE);
		::DeleteObject(hWndRgn);
	}
	
	if(m_bUseNewLogin)
	{
		m_btnOK = CreateBmpButton(IDOK, IDB_NEW_LOGOUT_BTN_OK);
		m_btnCancel = CreateBmpButton(IDCANCEL, IDB_NEW_LOGOUT_BTN_CANCEL);
		
		const POINT shapePosition[MAX_SHAPE_EX -2] =
		{
			{ 649, 1 }, { 649, 67 }, { 649, 134 }, { 649, 201 }, {  649, 268 }, { 649, 335 }
		};
		
		for (int i = 0; i < MAX_SHAPE_EX -2; i++)
		{
			m_shapeButtons[i] = CreateShapeButton(IDC_SHAPE1 + i, Format("NEW_LINK_%02d", i + 1),
				CRect(shapePosition[i].x, shapePosition[i].y, 0, 0));
			m_shapeButtons[i]->ShowWindow(SW_HIDE);   //cloude HTS종료 DLG화면 내부 버튼 삭제
		}
		
		m_shapeButtons[MAX_SHAPE_EX -2] = CreateShapeButton(IDC_SHAPE1 + MAX_SHAPE_EX -2, Format("NEW_LINK_%02d", MAX_SHAPE_EX -1),
				CRect(300, 374, 0, 0));

		m_shapeButtons[MAX_SHAPE_EX -2]->ShowWindow(SW_HIDE);  //고객센터 링크 제거
		
	}
	else
	{
		m_btnOK = CreateBmpButton(IDOK, IDB_LOGOUT_BTN_OK);
		m_btnCancel = CreateBmpButton(IDCANCEL, IDB_LOGOUT_BTN_CANCEL);
		
		POINT shapePosition[MAX_SHAPE_EX] =
		{
			//{ 208, 108 }, { 289, 108 }, { 369, 108 }, { 210, 291 }, {  297, 291 }, { 356, 291 }, {37, 309}
			{ 208, 111 }, { 268, 111 }, { 389, 111 }, { 210, 291 }, {  297, 291 }, { 356, 291 }, {37, 305}, {326, 111}
		};
		if( Axis::isCustomer )
		{
			
			for( int i = 0; i < MAX_SHAPE_EX; i++ )
				if ((i==0) || (i==1) || (i==2) || (i==7))
					shapePosition[i].y -= 7;
		}
		for (int i = 0; i < MAX_SHAPE_EX; i++)
		{
			if (i==5 )
			{
				m_shapeButtons[i] = CreateShapeButton(IDC_SHAPE1 + i, Format("LINK_%02d", i + 1),
					CRect(shapePosition[i].x, (Axis::isCustomer)? shapePosition[i].y: shapePosition[i].y, 0, 0));
			}
			else
			{
				m_shapeButtons[i] = CreateShapeButton(IDC_SHAPE1 + i, Format("LINK_%02d", i + 1),
					CRect(shapePosition[i].x, (Axis::isCustomer)? shapePosition[i].y: shapePosition[i].y, 0, 0));
			}
			
		}
	}	
	
	ASSERT(m_btnOK && m_btnCancel);

	if(m_bUseNewLogin)
	{
		m_btnOK->SetWindowPos(NULL, 338, 276, 259, 39, SWP_NOSIZE | SWP_NOZORDER);
		m_btnCancel->SetWindowPos(NULL, 603, 276, 56, 39, SWP_NOSIZE | SWP_NOZORDER);
	}
	else
	{
		m_btnOK->SetWindowPos(NULL,  228, 248, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_btnCancel->SetWindowPos(NULL, 329, 248, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

SYSTEMTIME CExitD::GetUseTime()
{
	SYSTEMTIME time{};
	int	day{}, hour{}, minute{}, second{};

	day = m_etime.wDay - m_stime.wDay;
	hour = m_etime.wHour - m_stime.wHour;
	minute = m_etime.wMinute - m_stime.wMinute;
	second = m_etime.wSecond - m_stime.wSecond;

	if (second < 0)
	{
		second += 60;
		minute--;
	}
	if (minute < 0)
	{
		minute += 60;
		hour--;
	}
	if (hour < 0)
	{
		hour += 24;
		day--;
	}
	if (day > 0)
		hour += day*24;
	time.wDay = day;
	time.wHour = hour;
	time.wMinute = minute;
	time.wSecond = second;

	return time;
}

void CExitD::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case IDT_BANNER_EXIT:
		RotateBanner();
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CExitD::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}

CBmpButton* CExitD::CreateBmpButton(UINT buttonID, UINT buttonBitmap)
{
	CBmpButton* btn = new CBmpButton;

	btn->SubclassWindow(GetDlgItem(buttonID)->m_hWnd);
	btn->SetImgBitmap(Axis::GetBitmap(buttonBitmap), 2);
	return btn;
}

void CExitD::OnDestroy()
{
	KillTimer(IDT_BANNER_EXIT);
	DELETE_WND(m_btnOK);
	DELETE_WND(m_btnCancel);
	ReleaseShapeButton();
	CDialog::OnDestroy();
}

void CExitD::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;

	GetClientRect(&rc);
	
	ASSERT(m_bitmap);

	CDC mdc;
	mdc.CreateCompatibleDC(&dc);
	CBitmap* oldBmp = mdc.SelectObject(m_bitmap);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
	mdc.SelectObject(oldBmp);
	
	SYSTEMTIME time{};
	CString text;

	GetLocalTime(&m_etime);
	CRect r;

	if(m_bUseNewLogin)
	{
		CRect rect(370, 90, 540, 275);

		r.CopyRect(rect);
	}
	else
	{
		CRect rect(120+100, 100+25, 310+130, 220+11+35);

		r.CopyRect(rect);
	}

	dc.SetBkMode(TRANSPARENT);

	CFont* oldFont = dc.SelectObject(&m_font);

	dc.SelectObject(&m_fontBold);
	dc.DrawText(CString("접속시작:"), r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	dc.SelectObject(&m_font);
	dc.DrawText(Format("%02d시 %02d분 %02d초", m_stime.wHour, m_stime.wMinute, m_stime.wSecond), 
		r, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	r.OffsetRect(0, 18);

	dc.SelectObject(&m_fontBold);
	dc.DrawText(CString("접속종료:"), r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	dc.SelectObject(&m_font);
	dc.DrawText(Format("%02d시 %02d분 %02d초", m_etime.wHour, m_etime.wMinute, m_etime.wSecond), 
		r, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	r.OffsetRect(0, 18);

	time = GetUseTime();
	dc.SetTextColor(RGB(253, 79, 2));
	dc.SelectObject(&m_fontBold);
	dc.DrawText(CString("접속시간:"), r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	dc.DrawText(Format("%d시간 %02d분 %02d초", time.wHour, time.wMinute, time.wSecond), 
		r, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	dc.SelectObject(oldFont);
	
}

void CExitD::SetEventBitmap(int index)
{
	m_currEvent = index - 1;
	if (!m_bmpEvent)
	{
		m_bmpEvent = new CStatic;
		
		const CRect rc(0, 0, 175, 220);
		//rc.OffsetRect(420, 48);
		//rc.OffsetRect(28, 100);
		
		m_bmpEvent->Create("", m_static.GetStyle(), rc, this, IDC_STATIC);
		m_bmpEvent->SetWindowPos(&m_static, 27, 72, 0, 0, SWP_NOSIZE );
	}
	
	if (m_bitmapEvent)
	{
		DeleteObject(m_bitmapEvent);
		m_bitmapEvent = NULL;
	}
	
	CString fileName;
	fileName.Format("%s\\%s\\banner_%02d.bmp", Axis::home, IMAGEDIR, index);
	
	m_bitmapEvent = CEnBitmap::LoadImageFile(fileName, RGB(255, 255, 255));
	
	if (m_bitmapEvent)
	{
		m_bmpEvent->SetBitmap(m_bitmapEvent);
		m_bmpEvent->ShowWindow(SW_SHOW);
	}
	else
	{
		m_bmpEvent->ShowWindow(SW_HIDE);
	}
	
	{
		static BOOL bFirst = TRUE;
		KillTimer(IDT_BANNER_EXIT);
		
		SetTimer(IDT_BANNER_EXIT, 2000, NULL);
		//bFirst = FALSE;
	}
}



void CExitD::RotateBanner()
{
	if (m_currEvent == -1)
		return;
	
	m_currEvent++;
	if (m_currEvent >= 3)
		m_currEvent = 0;
	SetEventBitmap(m_currEvent + 1);
/*	
	for (int i = IDC_EVENT1; i < IDC_EVENT1 + 3; i++)
		GetDlgItem(i)->SendMessage(BM_SETCHECK);
	
	CWnd* btn = GetDlgItem(IDC_EVENT1 + m_currEvent);
	if (btn)
		btn->SendMessage(BM_SETCHECK, TRUE);
*/
}

CShapeButton* CExitD::CreateShapeButton(UINT buttonID, const char* bmpFileName, CRect rc)
{
	CShapeButton* btn = new CShapeButton;

	btn->Create("", WS_CHILD | WS_VISIBLE, rc, this, buttonID);
	btn->SetImgBitmap(bmpFileName, 2);

	return btn;
}

void CExitD::ReleaseShapeButton()
{
	if(m_bUseNewLogin)
	{
		for (int i = 0; i < MAX_SHAPE_EX -1; i++)
		{
			if (m_shapeButtons[i])
			{
				m_shapeButtons[i]->DestroyWindow();
				delete m_shapeButtons[i];
				m_shapeButtons[i] = NULL;
			}
		}
	}
	else
	{
		for (int i = 0; i < MAX_SHAPE_EX; i++)
		{
			if (m_shapeButtons[i])
			{
				m_shapeButtons[i]->DestroyWindow();
				delete m_shapeButtons[i];
				m_shapeButtons[i] = NULL;
			}
		}
	}
}


void CExitD::OnShapeButton(int index)
{
	if(m_bUseNewLogin)
	{
		const char* const urls[MAX_SHAPE_EX -1] =
		{
			"https://www.ibks.com/customer/quickstart/IOS00002_01.jsp",	// 처음이세요?
				"https://www.ibks.com/member/idsearch/associate.jsp",		// ID분실
				"https://www.ibks.com/member/pwcancel/pwretry.jsp",	//비밀번호분실
				"www.ibks.com/customer/certificate/cert_main.jsp",	// 인증센터
				"http://113366.com/ibks",								// 온라인도우미
				"http://www.ibks.com/customer/stipulation/info02_attendedview.do?seq=36&qs_flag=N",					// 비상주문  www.ibks.com/wts/index_view.do
				"http://www.ibks.com/customer/dealing/ars.jsp"			// 고객지원센터
		};
		
		if (index >= 0 && index < MAX_SHAPE_EX -1)
		{
			if (strlen(urls[index]) > 0)
				ShellExecute(NULL, _T("open"), urls[index], NULL,NULL, SW_SHOWNORMAL);
			else
				Axis::MessageBox(this, "준비중");
		}
	}
	else
	{
		const char* const urls[MAX_SHAPE_EX] =
		{
			"www.ibks.com/customer/stock/branchAccount.jsp",	// 처음이세요?
			"https://www.ibks.com/member/idsearch/associate.jsp",		// ID/비밀번호분실
			"www.ibks.com/customer/certificate/cert_main.jsp",	// 인증센터
			"http://113366.com/ibks",								// 온라인도우미
			"http://www.ibks.com/customer/stipulation/info02_attendedview.do?seq=36&qs_flag=N",					// 비상주문  www.ibks.com/wts/index_view.do
			"http://www.ibks.com/company/branch/branch.jsp",				// 영업점안내
			"http://www.ibks.com/customer/dealing/ars.jsp",				// 고객지원센터
			"https://www.ibks.com/member/pwcancel/pwretry.jsp"
		};

		if (index >= 0 && index < MAX_SHAPE_EX)
		{
			if (strlen(urls[index]) > 0)
				ShellExecute(NULL, _T("open"), urls[index], NULL,NULL, SW_SHOWNORMAL);
			else
				Axis::MessageBox(this, "준비중");
		}
	}
}

#include "axMsg.hxx"
BOOL CExitD::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	CString		file, keys;
	const UINT		cmd = LOWORD(wParam);
	const UINT		event = HIWORD(wParam);

	if (event != BN_CLICKED)
		return CDialog::OnCommand(wParam, lParam);

	if (cmd == IDOK)
	{
		/*bool bret = GetParent()->SendMessage(WM_AXIS, MAKEWPARAM(axUPLODING_LOG, 0));
		if (!bret)
			return TRUE;*/
	}

	if(m_bUseNewLogin)
	{
		if (cmd >= IDC_SHAPE1 && cmd < IDC_SHAPE1 + MAX_SHAPE_EX -1)
		{
			OnShapeButton(cmd - IDC_SHAPE1);
			return TRUE;
		}
	}
	else
	{
		if (cmd >= IDC_SHAPE1 && cmd < IDC_SHAPE1 + MAX_SHAPE_EX)
		{
			OnShapeButton(cmd - IDC_SHAPE1);
			return TRUE;
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}

#pragma warning(default : 26400)
#pragma warning(default : 26409)


BOOL CExitD::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		bool bret = GetParent()->SendMessage(WM_AXIS, MAKEWPARAM(axUPLODING_LOG, 0));
		if (!bret)
			return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
