// Alarm.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "Alarm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarm dialog
#define DHEIGHT 150
CAlarm::CAlarm(CString home, CWnd* pParent /*=NULL*/)
	: CDialog(CAlarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlarm)
	m_msg = _T("");
	//}}AFX_DATA_INIT
	m_home = home;
}

CAlarm::~CAlarm()
{
	if (m_bitmap)	DeleteObject(m_bitmap);
}

void CAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarm)
	DDX_Control(pDX, IDC_NOTICESKIN, m_skin);
	DDX_Text(pDX, IDC_MSG, m_msg);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MSG, m_edit);
}


BEGIN_MESSAGE_MAP(CAlarm, CDialog)
	//{{AFX_MSG_MAP(CAlarm)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &CAlarm::OnBnClickedOk)
	ON_WM_SIZE()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarm message handlers

BOOL CAlarm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);

	m_pCtlBrush =  new CBrush();
	m_pCtlBrush->CreateSolidBrush(RGB(249,249,249));

	CString	imgN;
	imgN.Format("%s\\image\\noticeskin.bmp", m_home);
	m_bitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			imgN,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	
	m_skin.SetBitmap(m_bitmap);
	/*
	CRect r;
    // 먼저 여백을 부여하고자 하는 에디트 컨트롤의 기본 여백정보를 읽어옵니다.
	m_edit.GetRect(r);
    r.top += 10;
    r.bottom -= 10;
    // 변경된 정보를 해당 에디트 컨트롤에 적용합니다.
    m_edit.SetRect(r);
	*/
	GetDlgItem(IDC_MSG)->SetFocus();
	//SetTimer(9999,100,NULL);
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlarm::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect	wRc;

	GetDlgItem(IDC_CAPTION)->GetWindowRect(wRc);
	ScreenToClient(&wRc);
	
	if (wRc.PtInRect(point))
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	else
	{
		GetDlgItem(IDOK)->GetWindowRect(wRc);
		ScreenToClient(&wRc);
		CPoint pt;
		::GetCursorPos(&pt);
		ScreenToClient(&pt);
		if(wRc.PtInRect(pt))
			PostMessage(WM_CLOSE);
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}


void CAlarm::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CAlarm::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}


void CAlarm::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//IDC_MSG
	//if(((CWnd*)GetDlgItem(IDC_MSG))->GetSafeHwnd())
	{
		((CWnd*)GetDlgItem(IDC_MSG))->MoveWindow(0,0,cx/2 - 10, cy - 10);
	}
}


void CAlarm::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnRButtonUp(nFlags, point);
}


void CAlarm::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
		case 9999:
			{
				CStringArray arr;
				CString strresult, tmps;
				while(1)
				{
					arr.Add(Parser(m_msg, _T("\\n")));
					if(m_msg.GetLength() == 0)
						break;
				}

				for(int ii = 0 ; ii < arr.GetSize(); ii++)
				{
					tmps = _T(" ") + arr.GetAt(ii);
					tmps += _T("\r\n");
					strresult += tmps;
				}
				((CWnd*)GetDlgItem(IDC_MSG))->SetWindowText(strresult);

				CRect rc;
				GetClientRect(rc);

				if(arr.GetSize() > 9)
					SetWindowPos (NULL, 0, 0, rc.Width(), rc.Height() + (arr.GetSize() - 8) * 10, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
				else
					SetWindowPos (NULL, 0, 0, rc.Width(), rc.Height(), SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

				GetClientRect(rc);
				((CWnd*)GetDlgItem(IDC_MSG))->MoveWindow(10,40,rc.Width() - 20, rc.Height() - 50);
				KillTimer(9999);

				UpdateData();
			}
			break;
	}
	CDialog::OnTimer(nIDEvent);
}

CString CAlarm::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

HBRUSH CAlarm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_MSG:
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(RGB(249,249,249));
			hbr = m_pCtlBrush->operator HBRUSH();
		break;
	}
	return hbr;
}
