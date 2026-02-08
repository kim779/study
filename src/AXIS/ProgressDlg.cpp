// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "ProgressDlg.h"
#include "axmsg.hxx"

#include "../dll/sm/EnBitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ID_PROGRESS	1001
#define	TM_PROGRESS	10000
/////////////////////////////////////////////////////////////////////////////
// CProgressDlg dialog


CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/, BOOL bNew)
	: CDialog(bNew?CProgressDlg::IDD_NEW:CProgressDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressDlg)
	m_current = _T("");
	//}}AFX_DATA_INIT

	m_bitmap = (HBITMAP) NULL;
	m_timer = 0;
	m_bNew = bNew;
}

CProgressDlg::~CProgressDlg()
{
	if (m_bitmap)	DeleteObject(m_bitmap);
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDlg)
	DDX_Control(pDX, IDC_TOTPROGRESS, m_totalCtrl);
	DDX_Control(pDX, IDC_CURPROGRESS, m_currCtrl);
	DDX_Control(pDX, IDC_PROGRESS, m_static);
	DDX_Text(pDX, IDC_FILE, m_current);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CProgressDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProgressDlg message handlers

BOOL CProgressDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_totalCtrl.SetRange(0, 100);
	m_currCtrl.SetRange(0, 100);
	m_currCtrl.SetStep(1);

	m_first   = true;
	m_szTotal = 0.0;
	m_szCurr  = 0.0;
	m_nTotal  = 0;
	m_nCurr   = 0;
	m_rate    = 0;
	
	CFileFind finder;
	CString	imgN;

	imgN.Format("%s\\%s\\DPROGRESS.JPG", Axis::home, IMAGEDIR);
	m_bitmap = CEnBitmap::LoadImageFile(imgN, RGB(255, 255, 255));

	if (!m_bitmap)
	{
		imgN.Format("%s\\%s\\DPROGRESS.BMP", Axis::home, IMAGEDIR);
		m_bitmap = CEnBitmap::LoadImageFile(imgN, RGB(255, 255, 255));
	}

	if (m_bitmap)
	{
		HRGN hWndRgn = DIBToRgn(m_bitmap, Axis::maskColor, FALSE);
		if(hWndRgn)
		{
			SetWindowRgn(hWndRgn,TRUE);
			::DeleteObject(hWndRgn);
		}
	}
	else	visibleButton();

	initButtons();
	m_static.SetBitmap(m_bitmap);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);	// 20070711

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProgressDlg::OnCancel() 
{
	if (GetDlgItem(IDCANCEL)->IsWindowVisible())
		//**MessageBox("다운로드 중 멈추는 현상이 자주일어나는 경우\n\n네트웍에 이상이 있을 수 있습니다. \n\n인터넷 서비스 업체에 문의 해보십시오.", "알림", MB_OK);
		Axis::MessageBox(this, "다운로드 중 멈추는 현상이 자주 일어나는 경우\n\n네트워크에 이상이 있을 수 있습니다.\n\n인터넷 서비스 업체에 문의 해보시기 바랍니다.");
	GetOwner()->PostMessage(WM_STOP, 0, 0);
	EndDialog(IDCANCEL);
}

void CProgressDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	if (bShow)
	{
		SetForegroundWindow();
	}
	else
	{
		KillTimer(m_timer);
	}
}

void CProgressDlg::SetTotal(CString total)
{
	CString	tmps;
	int	pos{};

	m_first = false;
	pos  = total.Find('\t');
	tmps = total.Left(pos);
	m_nTotal = atoi(tmps);
	m_nCurr  = 0;

	tmps = total.Mid(pos+1);
	m_szTotal = atof(tmps);
	m_szCurr  = 0.0;
	tmps.Format(" %d ", m_nCurr);	m_bnENDCNT.SetText(tmps);
	tmps.Format(" %d ", m_nTotal);	m_bnFILECNT.SetText(tmps);
	m_totalCtrl.SetPos(0);
}

void CProgressDlg::SetProgress(CString detail, int rate, BOOL error)
{
	if (m_timer)	KillTimer(m_timer);
	m_timer = SetTimer(ID_PROGRESS, TM_PROGRESS, NULL);

	if (error)
	{
		m_errMsg.SetText(detail);
		return;
	}

	CString tmps;
	if (detail.IsEmpty())
	{
		m_currCtrl.SetPos(100);
		if (rate >= 100 || m_nTotal > 0)
		{
			m_nCurr++;
			tmps.Format(" %d ", m_nCurr);	m_bnENDCNT.SetText(tmps);
			tmps.Format(" %d ", m_nTotal);	m_bnFILECNT.SetText(tmps);
			m_current = detail;
		}
		return;
	}

	if (m_szTotal <= 0)	return;
	int	pos{}, value{};
	double	valuex;

	valuex = 0.0;
	pos    = detail.Find('\t');
	m_current = m_bnFILENAME.GetText();
	if (pos > 0 && detail.GetLength() > 0)
	{
		tmps   = detail.Mid(pos+1);
		detail = detail.Left(pos);
		if (m_current.CompareNoCase(detail) || (!m_current.CompareNoCase(detail) && rate != 100))
		{
			valuex    = atof(tmps);
			m_szCurr += valuex;
			value     = (int)((m_szCurr*100.0)/m_szTotal);
			m_totalCtrl.SetPos(value);

			tmps.Format("( %.f / %.f )", m_szCurr, m_szTotal);
			m_bnTOTSIZE.SetText(tmps);
		}
	}

	m_bnFILENAME.SetText(detail);
	
	if (rate < m_rate)	m_currCtrl.OffsetPos(m_rate-rate);
	else			m_currCtrl.SetPos(m_rate);
	

	if (rate >= 100)
	{
		m_nCurr++;
		tmps.Format(" %d ", m_nCurr);	m_bnENDCNT.SetText(tmps);
		tmps.Format(" %d ", m_nTotal);	m_bnFILECNT.SetText(tmps);
		m_current = detail;
	}
	
	m_rate = rate;
}

void CProgressDlg::visibleButton()
{
	return;
	/*
	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
//	GetDlgItem(IDC_STATIC5)->ShowWindow(SW_SHOW);
*/
}

void CProgressDlg::initButtons()
{
	BOOL bJpg = TRUE;
	CWnd* item{};
	CRect	wRc;
	CString	file, file_;
	CFileFind finder;
	const int	idcX[] = { IDCANCEL, IDC_TOTALSIZE, IDC_TRANSENDFILE, IDC_TOTALFILE, IDC_FILE , IDC_ERRMSG};

	file.Format("%s/%s/DPROGRESS.jpg", Axis::home, IMAGEDIR);
	if (!finder.FindFile(file))
	{
		file.Format("%s/%s/DPROGRESS.bmp", Axis::home, IMAGEDIR);
		bJpg = FALSE;
	}
	file_.Format("%s/%s/DPROGRESS_.jpg", Axis::home, IMAGEDIR);
	if (!finder.FindFile(file_))
		file_.Format("%s/%s/DPROGRESS_.bmp", Axis::home, IMAGEDIR);

	for (int ii = 0; ii < sizeof(idcX) / sizeof(int); ii++)
	{
		item = GetDlgItem(idcX[ii]);
		item->GetWindowRect(&wRc);
		ScreenToClient(&wRc);
		switch (idcX[ii])
		{
		case IDCANCEL:
			VERIFY(m_bnCancel.Attach(idcX[ii], this, wRc, file, file_, file_));
			break;
		case IDC_TOTALSIZE:
			VERIFY(m_bnTOTSIZE.Attach(idcX[ii], this, wRc, file, file_, file_));
			break;
		case IDC_TRANSENDFILE:
			VERIFY(m_bnENDCNT.Attach(idcX[ii], this, wRc, file, file_, file_));
			break;
		case IDC_TOTALFILE:
			VERIFY(m_bnFILECNT.Attach(idcX[ii], this, wRc, file, file_, file_));
			break;
		case IDC_FILE:
			VERIFY(m_bnFILENAME.Attach(idcX[ii], this, wRc, file, file_, file_));
			break;
		case IDC_ERRMSG:
			VERIFY(m_errMsg.Attach(idcX[ii], this, wRc, file, file_, file_));
			break;
		}
	}

	m_bnCancel.SetAlign(DT_CENTER);
	m_bnTOTSIZE.SetAlign(DT_RIGHT);
	m_bnENDCNT.SetAlign(m_bNew?DT_RIGHT:DT_CENTER);
	m_bnFILECNT.SetAlign(m_bNew?DT_RIGHT:DT_CENTER);
	m_bnFILENAME.SetAlign(DT_CENTER);
	m_errMsg.SetAlign(DT_CENTER);

	m_bnTOTSIZE.SetTextColor(m_bNew?RGB(255, 255, 255):RGB(0, 134, 112));
	m_bnENDCNT.SetTextColor(m_bNew?RGB(255, 255, 255):RGB(0, 134, 112));
	m_bnFILECNT.SetTextColor(m_bNew?RGB(255, 255, 255):RGB(0, 134, 112));
	m_bnFILENAME.SetTextColor(m_bNew?RGB(255, 255, 255):RGB(0, 134, 112));
	m_errMsg.SetTextColor(m_bNew?RGB(255, 255, 255):RGB(103, 104, 104));

	if (m_bNew)
	{
		m_totalCtrl.SetImageMode(PK_AQUA);
		m_currCtrl.SetImageMode(PK_AQUA);
		file.Format("%s/%s/AQUAPROGRESS_TOP", Axis::home, IMAGEDIR);
		file_.Format("%s/%s/AQUAPROGRESS_.bmp", Axis::home, IMAGEDIR);
		if (bJpg)
		{
			file.Format("%s/%s/AQUAPROGRESS2_TOP", Axis::home, IMAGEDIR);
			file_.Format("%s/%s/AQUAPROGRESS2_.bmp", Axis::home, IMAGEDIR);
		}
		m_currCtrl.SetImage(file, file_);
		file.Format("%s/%s/AQUAPROGRESS_BOT", Axis::home, IMAGEDIR);
		if (bJpg)
			file.Format("%s/%s/AQUAPROGRESS2_BOT", Axis::home, IMAGEDIR);
		m_totalCtrl.SetImage(file, file_);

		CRect rc;
		m_currCtrl.GetClientRect(&rc);
		m_currCtrl.SetWindowPos(NULL, 0, 0, rc.Width() + 1, rc.Height() + 1, SWP_NOZORDER|SWP_NOMOVE);
		m_totalCtrl.GetClientRect(&rc);
		m_totalCtrl.SetWindowPos(NULL, 0, 0, rc.Width() + 1, rc.Height() + 1, SWP_NOZORDER|SWP_NOMOVE);
	}
	else
	{
		file.Format("%s/%s/DOWNPROGRESS.bmp", Axis::home, IMAGEDIR);
		file_.Format("%s/%s/DOWNPROGRESS_.bmp", Axis::home, IMAGEDIR);
		m_totalCtrl.SetImage(file, file_);
		m_currCtrl.SetImage(file, file_);
	}
	m_errMsg.SetText("컴포넌트 수신중...");
}


void CProgressDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonDown(nFlags, point);
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
}

BOOL CProgressDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)	
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CProgressDlg::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case ID_PROGRESS:
		KillTimer(m_timer);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
