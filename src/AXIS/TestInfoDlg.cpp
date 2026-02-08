// TestInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "TestInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestInfoDlg dialog


CTestInfoDlg::CTestInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strurl = "";
	m_strbitmap = "EVENT07";
	m_bRscMode = true;
}


void CTestInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestInfoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CTestInfoDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestInfoDlg message handlers

void CTestInfoDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(m_bmpBg && m_bRscMode == false)  //image폴더의 파일을 사용하는 경우 
	{//
		if((CWnd*)GetDlgItem(IDC_DBTEST)->GetSafeHwnd())    
			(CWnd*)GetDlgItem(IDC_DBTEST)->ShowWindow(SW_HIDE);

		CDC mdc;
		mdc.CreateCompatibleDC(&dc);
		
		BITMAP bm;
		m_bmpBg->GetBitmap(&bm);
		
		CBitmap* oldBmp = mdc.SelectObject(m_bmpBg);
		
		dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &mdc, 0, 0, SRCCOPY);
		
		mdc.SelectObject(oldBmp);
		mdc.DeleteDC();
		
		SetWindowPos(&CWnd::wndTopMost, 0,0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE);
/*
		CRect rc, tmprc, tgrc;
		GetClientRect(rc);
		
		((CWnd*)GetDlgItem(IDOK))->GetClientRect(tmprc);
		
		tgrc.left = rc.right - tmprc.Width() - 10;
		tgrc.top = rc.bottom - tmprc.Height() - 5;
		tgrc.right = tgrc.left + tmprc.Width();
		tgrc.bottom = tgrc.left + tmprc.Height();
		
		((CWnd*)GetDlgItem(IDOK))->MoveWindow(tgrc);
		*/
	}
	else  //res폴더 파일로 하는경우
		SetWindowPos(&CWnd::wndTopMost, 0,0, 400, 492, SWP_NOMOVE);
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CTestInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			{
				CDialog::OnOK();
				return TRUE;
			}
			break;
		}
		
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CTestInfoDlg::OnInitDialog()   //IDOK
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CWnd*)GetDlgItem(IDOK))->ShowWindow(SW_HIDE);
	m_bmpBg = LoadFileBitmap(m_strbitmap);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTestInfoDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);
}

void CTestInfoDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString slog;
	slog.Format("%d %d", point.x, point.y);
	CRect rc;
	GetClientRect(&rc);
	rc.SetRect(rc.right - 40, 15, rc.right - 10, rc.top + 50);
	if(rc.PtInRect(point))
	{
		OnOK();
	}
	CDialog::OnLButtonUp(nFlags, point);
}
#pragma warning (disable : 26409)
#pragma warning (disable : 26400)
CBitmap* CTestInfoDlg::LoadFileBitmap(const char* bmpName )
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
#pragma warning (default : 26409)
#pragma warning (default : 26400)

void CTestInfoDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CTestInfoDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnOK();
}

void CTestInfoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if((CWnd*)GetDlgItem(IDOK)->GetSafeHwnd())
	{
		CRect rc,  tgrc;
		const CRect tmprc;
		GetClientRect(rc);
		tgrc.left = rc.right - 70;
		tgrc.top = rc.bottom - 25;
		tgrc.right = tgrc.left + 60;
		tgrc.bottom = rc.bottom - 2;
		((CWnd*)GetDlgItem(IDOK))->MoveWindow(tgrc);
		((CWnd*)GetDlgItem(IDOK))->Invalidate();
	}
}
