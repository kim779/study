// CDLG_Notice.cpp: 구현 파일
//
#include "stdafx.h"
#include "axis.h"
#include "axis.h"
#include "CDLG_Notice.h"
#include "afxdialogex.h"


// CDLG_Notice 대화 상자
#define DF_TM_FILE 9999
IMPLEMENT_DYNAMIC(CDLG_Notice, CDialog)

CDLG_Notice::CDLG_Notice(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_NOTICEPOPUP, pParent)
{
	m_strbitmap = "최선집행기준설명서교부팝업";
}

CDLG_Notice::~CDLG_Notice()
{
}

void CDLG_Notice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDLG_Notice, CDialog)
	ON_BN_CLICKED(IDC_CHK1_CONFIRM, &CDLG_Notice::OnBnClickedChk1Confirm)
	ON_BN_CLICKED(IDC_CHK2_CONFIRM, &CDLG_Notice::OnBnClickedChk2Confirm)
	ON_BN_CLICKED(9899, &CDLG_Notice::OnBnClickedBtnDevclose)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CDLG_Notice::OnBnClickedOk)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CDLG_Notice 메시지 처리기
BOOL CDLG_Notice::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(NULL, 0, 0, 450, 600, SWP_NOMOVE | SWP_NOZORDER);
	m_bmpBg = LoadFileBitmap(m_strbitmap);

	CRect rc;

	((CWnd*)GetDlgItem(IDOK))->ShowWindow(SW_HIDE);
	GetDlgItem(IDOK)->GetWindowRect(&rc); ScreenToClient(rc);
	m_btnOk.Create("", WS_CHILD | WS_VISIBLE | BS_BITMAP, rc, this, IDOK);
	m_btnOk.m_hBitmap = Axis::GetBitmap("닫기1")->operator HBITMAP();
	m_btnOk.m_hBitmap_dn = Axis::GetBitmap("닫기1_DN")->operator HBITMAP();
	m_btnOk.m_hBitmap_hv = Axis::GetBitmap("닫기1_EN")->operator HBITMAP();



	((CWnd*)GetDlgItem(IDC_BTN_DEVCLOSE))->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BTN_DEVCLOSE)->GetWindowRect(&rc); ScreenToClient(rc);
	m_btnConfirm.Create("확인", WS_CHILD | WS_VISIBLE | BS_BITMAP, rc, this, 9899);
	m_btnConfirm.m_hBitmap = Axis::GetBitmap("5BTN")->operator HBITMAP();
	m_btnConfirm.m_hBitmap_dn = Axis::GetBitmap("5BTN_DN")->operator HBITMAP();
	m_btnConfirm.m_hBitmap_hv = Axis::GetBitmap("5BTN_EN")->operator HBITMAP();

	CRect recbtn;
	m_btnConfirm.GetWindowRect(recbtn);
	ScreenToClient(recbtn);
	int iwidth = recbtn.Width();
	recbtn.left = (450 / 2) - (iwidth / 2);
	recbtn.right = recbtn.left + iwidth;
	m_btnConfirm.MoveWindow(recbtn);
	m_btnConfirm.ShowWindow(SW_HIDE);

	int gap = 5;
	CRect rec;
	GetClientRect(rec);
	int iDelta{};
	iDelta = (int)rec.Height() / 30;
	int left, top, right, bottom;
	left = iDelta * 2 + gap;
	top = rec.Height() / 2 + iDelta;
	right = rec.Width() - (iDelta * 2) - gap;
	bottom = top + (3 * iDelta) + (2 * gap);

	m_rec1PFD.top = top;
	m_rec1PFD.left = left;
	m_rec1PFD.bottom = bottom;
	m_rec1PFD.right = right;

	m_rec2PFD = m_rec1PFD;
	m_rec2PFD.OffsetRect(0, iDelta / 2 + (iDelta * 3) + gap);


	/*if (m_bDev)
		((CWnd*)GetDlgItem(IDC_BTN_DEVCLOSE))->ShowWindow(SW_SHOW);
	else
		((CWnd*)GetDlgItem(IDC_BTN_DEVCLOSE))->ShowWindow(SW_HIDE);*/

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDLG_Notice::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	if (m_bmpBg)  //image폴더의 파일을 사용하는 경우 
	{
		if ((CWnd*)GetDlgItem(IDC_DBTEST)->GetSafeHwnd())
			(CWnd*)GetDlgItem(IDC_DBTEST)->ShowWindow(SW_HIDE);

		CDC mdc;
		mdc.CreateCompatibleDC(&dc);

		BITMAP bm;
		m_bmpBg->GetBitmap(&bm);

		CBitmap* oldBmp = mdc.SelectObject(m_bmpBg);

		dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &mdc, 0, 0, SRCCOPY);

		mdc.SelectObject(oldBmp);
		mdc.DeleteDC();

		SetWindowPos(&CWnd::wndTopMost, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE);
	}
}

#pragma warning (disable : 26409)
#pragma warning (disable : 26400)
CBitmap* CDLG_Notice::LoadFileBitmap(const char* bmpName)
{
	HBITMAP hBitmap;
	CString fileName;

	CString path(bmpName);
	path.MakeUpper();
	fileName.Format("%s\\image\\%s.bmp", Axis::home, bmpName);

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), fileName,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		CBitmap* bmp = new CBitmap;
		bmp->Attach(hBitmap);
		return bmp;
	}
	else
		TRACE("FAIL!!!!! = %s\n", bmpName);

	return NULL;
}

BOOL CDLG_Notice::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		//case VK_RETURN:
		case VK_ESCAPE:
		{
			CDialog::OnOK();
		//	return TRUE;
		}
		break;
		}

	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDLG_Notice::OnBnClickedChk1Confirm()
{
	m_file1pop = true;

	char	bufA[128]{};
	char	bufB[128]{};
	CString	file, tmps;

	file.Format("%s\\%s\\%s\\userconf.ini", m_sRoot, USRDIR, m_sUser);
	DWORD dwA = GetPrivateProfileString("PDF_CONFIRM", "pop", "0", bufA, sizeof(bufA), file);   //유저폴더 확인
	tmps.Format("%s", bufA);

	if (tmps == "browser")
	{
		tmps = "";
		file.Format("%s\\%s\\AXNXT.ini", m_sRoot, "tab");  //tab 폴더 확인
		DWORD dwB = GetPrivateProfileString("PDF", "trade", "0", bufB, sizeof(bufB), file);  //최선
		tmps.Format("%s", bufB);
		tmps.TrimRight();
		ShellExecute(NULL, _T("open"), tmps, NULL, NULL, SW_SHOWNORMAL);

		bool bchk1 = ((CButton*)GetDlgItem(IDC_CHK1_CONFIRM))->GetCheck();
		bool bchk2 = ((CButton*)GetDlgItem(IDC_CHK2_CONFIRM))->GetCheck();

		if (bchk1 && bchk2)
		{
			KillTimer(DF_TM_FILE);
			AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(0x15, 1), (LPARAM)(LPSTR)(LPCTSTR)m_slog);
			CDialog::OnOK();
			return;
		}
	}
	else
	{
		KillTimer(DF_TM_FILE);
		SetTimer(DF_TM_FILE, 1000, nullptr);
		((CButton*)GetDlgItem(IDC_CHK1_CONFIRM))->SetCheck(FALSE);
		m_slog.Format("%s%s", "IB99996B", "ed_url");
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(0x15, 0), (LPARAM)(LPSTR)(LPCTSTR)m_slog);
	}
}


void CDLG_Notice::OnBnClickedChk2Confirm()
{
	
	m_file2pop = true;

	char	bufA[128]{};
	char	bufB[128]{};
	CString	file, tmps;

	file.Format("%s\\%s\\%s\\userconf.ini", m_sRoot, USRDIR, m_sUser);
	DWORD dwA = GetPrivateProfileString("PDF_CONFIRM", "pop", "0", bufA, sizeof(bufA), file);   //유저폴더 확인

	tmps.Format("%s", bufA);

	if (tmps == "browser")
	{
		tmps = "";
		file.Format("%s\\%s\\AXNXT.ini", m_sRoot, "tab");  //tab 폴더 확인
		DWORD dwB = GetPrivateProfileString("PDF", "best_excution", "0", bufB, sizeof(bufB), file);  //최선
		tmps.Format("%s", bufB);
		tmps.TrimRight();
		ShellExecute(NULL, _T("open"), tmps, NULL, NULL, SW_SHOWNORMAL);

		bool bchk1 = ((CButton*)GetDlgItem(IDC_CHK1_CONFIRM))->GetCheck();
		bool bchk2 = ((CButton*)GetDlgItem(IDC_CHK2_CONFIRM))->GetCheck();

		if (bchk1 && bchk2)
		{
			KillTimer(DF_TM_FILE);
			AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(0x15, 1), (LPARAM)(LPSTR)(LPCTSTR)m_slog);
			CDialog::OnOK();
			return;
		}
	}
	else
	{
		KillTimer(DF_TM_FILE);
		SetTimer(DF_TM_FILE, 1000, nullptr);
		((CButton*)GetDlgItem(IDC_CHK2_CONFIRM))->SetCheck(FALSE);
		m_slog.Format("%s%s", "IB99996C", "ed_url");
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(0x15, 0), (LPARAM)(LPSTR)(LPCTSTR)m_slog);
	}
}

void CDLG_Notice::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case DF_TM_FILE:
		{
			char	bufA[64]{};
			char	bufB[64]{};
			CString	file, tmps;

			file.Format("%s\\%s\\%s\\userconf.ini", m_sRoot, USRDIR, m_sUser);

			DWORD dwA = GetPrivateProfileString("PDF_CONFIRM", "trade", "0", bufA, sizeof(bufA), file);   //매매
			DWORD dwB = GetPrivateProfileString("PDF_CONFIRM", "best_excution", "0", bufB, sizeof(bufB), file);  //최선

			if (strcmp(bufA, "1") == 0 && m_file1pop)
				((CButton*)GetDlgItem(IDC_CHK1_CONFIRM))->SetCheck(TRUE);

			if (strcmp(bufB, "1") == 0 && m_file2pop)
				((CButton*)GetDlgItem(IDC_CHK2_CONFIRM))->SetCheck(TRUE);

			m_slog.Format("[popup]file=[%s]  trade=[%s]  best=[%s]", file, bufA, bufB);
			OutputDebugString(m_slog);

			bool bchk1 = ((CButton*)GetDlgItem(IDC_CHK1_CONFIRM))->GetCheck();
			bool bchk2 = ((CButton*)GetDlgItem(IDC_CHK2_CONFIRM))->GetCheck();

			if (bchk1 && bchk2)
			{
				KillTimer(DF_TM_FILE);
				AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(0x15, 1), (LPARAM)(LPSTR)(LPCTSTR)m_slog);
				CDialog::OnOK();
				return;

			}
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CDLG_Notice::OnBnClickedBtnDevclose()   //확인버튼
{
	bool bchk1 = ((CButton*)GetDlgItem(IDC_CHK1_CONFIRM))->GetCheck();
	bool bchk2 = ((CButton*)GetDlgItem(IDC_CHK2_CONFIRM))->GetCheck();

	if (bchk1 && bchk2)
	{
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(0x15, 1), (LPARAM)(LPSTR)(LPCTSTR)m_slog);
		CDialog::OnOK();
		return;
	}
	else
	{
		CString msg, title;
		title = "최선집행의무 등록 안내";
		msg = "중요 약관/설명서 미확인 시 \r\n3월 4일부터 국내주식 거래,청약이 불가해요.\r\n약관/설명서를 확인하시겠어요?";
		if (MessageBox(msg, title, MB_YESNO) == IDNO)
			CDialog::OnOK();
	}
}

void CDLG_Notice::OnBnClickedOk()
{
	//bool bchk1 = ((CButton*)GetDlgItem(IDC_CHK1_CONFIRM))->GetCheck();
	//bool bchk2 = ((CButton*)GetDlgItem(IDC_CHK2_CONFIRM))->GetCheck();

	//if (!bchk1 || !bchk2)
	//{ 
	CString msg, title;
	title = "최선집행의무 등록 안내";
	//msg = "2025년 3월 4일 부터 거래, 청약을 하기 위해서는\r\n계좌 소유주 본인이 최선집행기준 설명서를 확인해야해요\r\n(고객센터 : 1544-0050)";
	msg = "중요 약관/설명서 미확인 시 \r\n3월 4일부터 국내주식 거래,청약이 불가해요.\r\n약관/설명서를 확인하시겠어요?";
	if (MessageBox(msg, title, MB_YESNO) == IDNO)
		CDialog::OnOK();
	else
		return;
	//}


	CDialog::OnOK();
}


void CDLG_Notice::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_rec1PFD.PtInRect(point))
		OnBnClickedChk1Confirm();
	else if (m_rec2PFD.PtInRect(point))
		OnBnClickedChk2Confirm();
	CDialog::OnLButtonUp(nFlags, point);
}


void CDLG_Notice::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_rec1PFD.PtInRect(point))
		SetCursor(Axis::handCursor);
	else if (m_rec2PFD.PtInRect(point))
		SetCursor(Axis::handCursor);
	CDialog::OnMouseMove(nFlags, point);
}


BOOL CDLG_Notice::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CPoint point;
	GetCursorPos(&point);

	if (m_rec1PFD.PtInRect(point))
		SetCursor(Axis::handCursor);
	else if (m_rec2PFD.PtInRect(point))
		SetCursor(Axis::handCursor);
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
