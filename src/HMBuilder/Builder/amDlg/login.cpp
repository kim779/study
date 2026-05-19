// login.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "login.h"
#include "afxdialogex.h"

#include "loginSetDlg.h"
#include "../h/mapvar.H"
#include "../h/mainvar.h"
#include "../h/axisvar.h"

// CLogin 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogin::IDD, pParent)
{
	m_offline = FALSE;
	m_pass = _T("");
	m_user = _T("");

	m_errs = _T("");
	m_bitmapSz = CSize(0, 0);
}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_OFFLINECHK, m_offline);
	DDX_Text(pDX, IDC_PASSEDIT, m_pass);
	DDX_Control(pDX, IDC_SETBTN, m_setBN);
	DDX_Text(pDX, IDC_USERIDEDIT, m_user);
	DDX_Control(pDX, IDCANCEL, m_cancelBN);
	//  DDX_Control(pDX, IDOK, m_okBN);
	DDX_Control(pDX, IDOK, m_connectBN);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SETBTN, &CLogin::OnSetup)
END_MESSAGE_MAP()


// CLogin 메시지 처리기입니다.


BOOL CLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (GetPrivateProfileInt((LPCTSTR)ENVIRONMENT, _T("OFFLINE"), 0, m_currentPath + _T("\\workshop.ini")))
		m_offline = FALSE;
	else
		m_offline = TRUE;

	UpdateData(FALSE);
	
	m_setBN.SetWindowPos(NULL, 84, m_bitmapSz.cy - 105, 51, 24, SWP_NOZORDER);
	m_connectBN.SetWindowPos(NULL, 25, m_bitmapSz.cy - 105, 59, 24, SWP_NOZORDER);
	m_cancelBN.SetWindowPos(NULL, m_bitmapSz.cx - 20, 7, 10, 10, SWP_NOZORDER);

	GetDlgItem(IDC_OFFLINECHK)->SetWindowPos(NULL, 142, m_bitmapSz.cy - 127, 12, 12, SWP_NOZORDER);
	GetDlgItem(IDC_USERIDEDIT)->SetWindowPos(NULL, 29, m_bitmapSz.cy - 152, 104, 17, SWP_NOZORDER);
	GetDlgItem(IDC_PASSEDIT)->SetWindowPos(NULL, 29, m_bitmapSz.cy - 127, 104, 17, SWP_NOZORDER);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLogin::OnOK()
{
	UpdateData();
	if (!m_user.IsEmpty() && m_pass.IsEmpty())
	{
		GetDlgItem(IDC_PASSEDIT)->SetFocus();
			return;
	}
	m_parent->SendMessage(WM_LOGIN, IDOK);
	GetDlgItem(IDCANCEL)->SetFocus();
//	CDialogEx::OnOK();
}


void CLogin::OnCancel()
{
	m_parent->SendMessage(WM_LOGIN, IDCANCEL);	
//	CDialogEx::OnCancel();
}


int CLogin::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_bitmap.LoadBitmap(IDB_LOGINBMP))
		AfxMessageBox(_T("Error Fail to load the Login Image!!"));

	if (m_bitmap.operator HBITMAP())
	{
		m_bitmapSz = bitmapSize(m_bitmap.operator HBITMAP());
		SetWindowPos(NULL, 0, 0, m_bitmapSz.cx, m_bitmapSz.cy, SWP_NOMOVE | SWP_NOZORDER);
	}

	m_setBN.LoadBitmaps(IDB_SETUPBTN, IDB_SETUPBTN_DN, IDB_SETUPBTN_EN);
	m_connectBN.LoadBitmaps(IDB_CONNECTBTN, IDB_CONNECTBTN_DN, IDB_CONNECTBTN_EN);
	m_cancelBN.LoadBitmaps(IDB_CLOSEBTN, IDB_CLOSEBTN_DN, IDB_CLOSEBTN_EN);
	
	m_brush.CreateSolidBrush(RGB(235, 235, 235));
	CenterWindow();

	return 0;
}


HBRUSH CLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
		pDC->SetBkColor(RGB(235, 235, 235));
		hbr = (HBRUSH) m_brush.operator HBRUSH();
		break;
	}
	return hbr;
}


void CLogin::OnDestroy()
{
	if (m_bitmap.operator HBITMAP())
		m_bitmap.DeleteObject();
	CDialogEx::OnDestroy();
}


void CLogin::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_OFFLINECHK)->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(5, 5));
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CLogin::OnLButtonUp(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_OFFLINECHK)->SendMessage(WM_LBUTTONUP, MK_LBUTTON, MAKELONG(5, 5));
	CDialogEx::OnLButtonUp(nFlags, point);
}


LRESULT CLogin::OnNcHitTest(CPoint point)
{
	CRect	rc;
	UINT	nHitTest = (UINT)CDialogEx::OnNcHitTest(point);

	rc = CRect(m_bitmapSz.cx - 98, m_bitmapSz.cy - 114, m_bitmapSz.cx - 60, m_bitmapSz.cy - 106);
	ClientToScreen(&rc);
	if (nHitTest == HTCLIENT && !rc.PtInRect(point))
		nHitTest = HTCAPTION;

	return nHitTest;

}


void CLogin::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_bitmap.operator HBITMAP())
	{
		CRect	rc;
		CFont*	pOldFont;

		drawBitmap(&dc);
		dc.SetBkMode(TRANSPARENT);

		m_connectBN.GetWindowRect(&rc);
		ScreenToClient(&rc);

		pOldFont = dc.SelectObject(GetFont());
		dc.SetTextColor(RGB(0, 0, 0));
		dc.DrawText(m_errs, CRect(rc.left, rc.bottom + 10, rc.left + 200, rc.bottom + 30), DT_LEFT|DT_VCENTER);
		dc.SelectObject(pOldFont);
	}
}


void CLogin::OnSetup()
{
	CLoginSetDlg	dlg(m_currentPath);
	dlg.DoModal();
}

void CLogin::SetParentW(CWnd* pParent)
{
	m_parent = pParent;
}

void CLogin::SetMessage(CString sMsg)
{
	m_errs = sMsg;
	Invalidate();
}

void CLogin::EnableLogin(bool bEnable)
{
	GetDlgItem(IDC_USERIDEDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_PASSEDIT)->EnableWindow(bEnable);
	GetDlgItem(IDOK)->EnableWindow(bEnable);
	GetDlgItem(IDC_SETBTN)->EnableWindow(bEnable);
	GetDlgItem(IDC_OFFLINECHK)->EnableWindow(bEnable);
}

CSize CLogin::bitmapSize(HBITMAP hBitmap)
{
	if (!hBitmap)
		return CSize(0, 0);

	BITMAP	BM;

	GetObject(hBitmap, sizeof(BITMAP), &BM);
	return CSize((int)BM.bmWidth, (int)BM.bmHeight);
}

void CLogin::drawBitmap(CDC *pDC)
{
	CRect	rc;

	GetClientRect(&rc);
	if (m_bitmap.operator HBITMAP())
	{
		CDC	memDC;
		
		memDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBitmap = memDC.SelectObject(&m_bitmap);
		pDC->BitBlt(rc.left, rc.top, rc.right, rc.bottom, &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
}