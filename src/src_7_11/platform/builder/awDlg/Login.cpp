// Login.cpp : implementation file
//

#include "stdafx.h"
#include "Login.h"
#include "../../h/axisvar.h"
#include "../mainvar.h"
#include "LoginSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogin dialog


CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogin)
	m_offlineChk = FALSE;
	m_sPass = _T("");
	m_sUser = _T("");
	//}}AFX_DATA_INIT
	m_bitmapSz = CSize(0, 0);
	m_sErr.Empty();
}


void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogin)
	DDX_Control(pDX, IDOK, m_connectBtn);
	DDX_Control(pDX, IDCANCEL, m_cancelBtn);
	DDX_Control(pDX, IDC_SETBTN, m_setBtn);
	DDX_Check(pDX, IDC_OFFLINECHK, m_offlineChk);
	DDX_Text(pDX, IDC_PASSEDIT, m_sPass);
	DDX_Text(pDX, IDC_USERIDEDIT, m_sUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
	//{{AFX_MSG_MAP(CLogin)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_SETBTN, OnSetbtn)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogin message handlers

BOOL CLogin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (GetPrivateProfileInt(ENVIRONMENT, "OFFLINE", 0, m_currentPath + "\\workshop.ini"))
		m_offlineChk = FALSE;
	else
		m_offlineChk = TRUE;

	UpdateData(FALSE);
	
	m_setBtn.SetWindowPos(NULL, m_bitmapSz.cx - 155, m_bitmapSz.cy - 99, 51, 24, SWP_NOZORDER);
	m_connectBtn.SetWindowPos(NULL, m_bitmapSz.cx - 213, m_bitmapSz.cy - 99, 59, 24, SWP_NOZORDER);
	m_cancelBtn.SetWindowPos(NULL, m_bitmapSz.cx - 20, 7, 10, 10, SWP_NOZORDER);

	GetDlgItem(IDC_OFFLINECHK)->SetWindowPos(NULL, m_bitmapSz.cx - 103, m_bitmapSz.cy - 118, 12, 12, SWP_NOZORDER);
	GetDlgItem(IDC_USERIDEDIT)->SetWindowPos(NULL, m_bitmapSz.cx - 209, m_bitmapSz.cy - 140, 100, 14, SWP_NOZORDER);
	GetDlgItem(IDC_PASSEDIT)->SetWindowPos(NULL, m_bitmapSz.cx - 209, m_bitmapSz.cy - 120, 100, 14, SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogin::OnOK() 
{
	UpdateData();
	if (!m_sUser.IsEmpty() && m_sPass.IsEmpty())
	{
		GetDlgItem(IDC_PASSEDIT)->SetFocus();
			return;
	}
	m_pParent->SendMessage(WM_USER + 1, IDOK);
	GetDlgItem(IDCANCEL)->SetFocus();
	//CDialog::OnOK();
}

LRESULT CLogin::OnNcHitTest(CPoint point)
{
	UINT nHitTest = CWnd::OnNcHitTest(point);

	CRect rc = CRect(m_bitmapSz.cx - 98, m_bitmapSz.cy - 114, m_bitmapSz.cx - 60, m_bitmapSz.cy - 106);
	ClientToScreen(&rc);
	if (nHitTest == HTCLIENT && !rc.PtInRect(point))
		nHitTest = HTCAPTION;

	return nHitTest;
}

void CLogin::OnSetbtn() 
{
	CLoginSetDlg	Dlg;
	Dlg.m_currentPath = m_currentPath;
	Dlg.DoModal();
}

void CLogin::setParent(CWnd* pParent)
{
	m_pParent = pParent;
}

void CLogin::OnCancel() 
{
	m_pParent->SendMessage(WM_USER + 1, IDCANCEL);	
	//CDialog::OnCancel();
}

void CLogin::setMessage(CString sMsg)
{
	//SetWindowText("ขั " + sMsg);
	m_sErr = sMsg;
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

void CLogin::OnDestroy() 
{
	if (m_Bitmap.operator HBITMAP())
		m_Bitmap.DeleteObject();
	CDialog::OnDestroy();	
}

void CLogin::SetFileImage(CString sFName)
{
	/*CFileFind finder;
	if (finder.FindFile(sFName))
	{
		m_hBitmap = (HBITMAP)::LoadImage(AfxGetResourceHandle(), sFName, IMAGE_BITMAP,
						 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
		m_bitmapSz = BitmapSize(m_hBitmap);
	}*/
}

void CLogin::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (m_Bitmap.operator HBITMAP())
	{
		drawBitmap(&dc);
		dc.SetBkMode(TRANSPARENT);

		CRect rc;
		m_connectBtn.GetWindowRect(&rc);
		ScreenToClient(&rc);

		CFont *pOldFont;
		pOldFont = dc.SelectObject(GetFont());
		dc.SetTextColor(RGB(0, 0, 0));
		dc.DrawText(m_sErr, CRect(rc.left, rc.bottom + 10, rc.left + 200, rc.bottom + 30), DT_LEFT|DT_VCENTER);
		dc.SelectObject(pOldFont);
	}
}

void CLogin::drawBitmap(CDC *pDC)
{
	CRect rc;
	GetClientRect(&rc);

	if (m_Bitmap.operator HBITMAP())
	{
		CDC		memDC;
		
		memDC.CreateCompatibleDC(pDC);
		CBitmap *pOldBitmap = memDC.SelectObject(&m_Bitmap);
		
		pDC->BitBlt(rc.left, rc.top, rc.right, rc.bottom, &memDC, 0, 0, SRCCOPY);
		
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
}

CSize CLogin::BitmapSize(HBITMAP hBitmap)
{
	if (!hBitmap)
		return CSize(0, 0);
	BITMAP BM;
	GetObject(hBitmap, sizeof(BITMAP), &BM);
	return CSize((int)BM.bmWidth, (int)BM.bmHeight);
}

int CLogin::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_Bitmap.LoadBitmap(IDB_LOGINBMP))
		AfxMessageBox("Error Fail to load the Login Image!!");

	if (m_Bitmap.operator HBITMAP())
	{
		m_bitmapSz = BitmapSize(m_Bitmap.operator HBITMAP());
		SetWindowPos(NULL, 0, 0, m_bitmapSz.cx, m_bitmapSz.cy, SWP_NOMOVE | SWP_NOZORDER);
	}

	m_setBtn.LoadBitmaps(IDB_SETUPBTN, IDB_SETUPBTN_DN, IDB_SETUPBTN_EN);
	m_connectBtn.LoadBitmaps(IDB_CONNECTBTN, IDB_CONNECTBTN_DN, IDB_CONNECTBTN_EN);
	m_cancelBtn.LoadBitmaps(IDB_CLOSEBTN, IDB_CLOSEBTN_DN, IDB_CLOSEBTN_EN);
	
	m_brush.CreateSolidBrush(RGB(235, 235, 235));
	CenterWindow();
	return 0;
}

void CLogin::OnLButtonDown(UINT nFlags, CPoint point) 
{
	GetDlgItem(IDC_OFFLINECHK)->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(5, 5));
	CDialog::OnLButtonDown(nFlags, point);
}

void CLogin::OnLButtonUp(UINT nFlags, CPoint point) 
{
	GetDlgItem(IDC_OFFLINECHK)->SendMessage(WM_LBUTTONUP, MK_LBUTTON, MAKELONG(5, 5));
	CDialog::OnLButtonUp(nFlags, point);
}

HBRUSH CLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT:
		pDC->SetBkColor(RGB(235, 235, 235));
		hbr = (HBRUSH) m_brush.operator HBRUSH();
		break;
	}
	return hbr;
}
