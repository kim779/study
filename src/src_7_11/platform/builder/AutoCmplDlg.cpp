// AutoCmplDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiswork.h"
#include "AutoCmplDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoCmplDlg dialog


CAutoCmplDlg::CAutoCmplDlg(CWnd* pParent /*= NULL*/)
	: CDialog(CAutoCmplDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoCmplDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
}

void CAutoCmplDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoCmplDlg)
	DDX_Control(pDX, IDC_ACLIST, m_aclist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoCmplDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoCmplDlg)
	ON_WM_SIZE()
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoCmplDlg message handlers

void CAutoCmplDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (IsWindow(m_aclist.m_hWnd))
		m_aclist.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);	
}

void CAutoCmplDlg::AddString(CString str, int type)
{
	m_aclist.AddString(str, type);
}

void CAutoCmplDlg::Reset()
{
	m_aclist.ResetContent();
}

int CAutoCmplDlg::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	return MA_NOACTIVATE;	
	return CDialog::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CAutoCmplDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CreateImageList();
	m_aclist.SetImageList(&m_ImageList);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoCmplDlg::CreateImageList()
{
	BOOL	bRetValue = FALSE;
	HICON	hIcon = NULL;

	// Create image list
	bRetValue = m_ImageList.Create(IDB_AUTOCMPLIMG, 12, 1, RGB(192,192,192));
	ASSERT(bRetValue == TRUE);
}

CString CAutoCmplDlg::getSelString()
{
	CString rtValue;
	if (m_aclist.GetCurSel() != LB_ERR)
		m_aclist.GetText(m_aclist.GetCurSel(), rtValue);

	return rtValue;
}

void CAutoCmplDlg::FindString(CString str)
{
	if (!str.IsEmpty())
	{
		int nIndex = m_aclist.SelectString(0, str);
		//if (nIndex != LB_ERR)
			m_aclist.SetCurSel(nIndex);
	}
}

LRESULT CAutoCmplDlg::OnMessage(WPARAM wParam, LPARAM lParam)
{
	m_pParent->PostMessage(WM_KEYDOWN, wParam, 0);
	return 0;
}

LOGFONT CAutoCmplDlg::setFont(int fsize, CString fname, BOOL bBold, BOOL bItalic)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = fsize * 10; 
	
	_tcscpy_s(lf.lfFaceName, fname);   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = (bBold?FW_BLACK:FW_NORMAL);
	lf.lfItalic = bItalic;
	return lf;
}

int CAutoCmplDlg::getWidth()
{
	int nMax = 0;
	for (int ii= 0; ii < m_aclist.GetCount(); ii++)
	{
		CString str;
		m_aclist.GetText(ii, str);
		nMax = max(nMax, str.GetLength());
	}
	return nMax * 8 + 40;
}

BOOL CAutoCmplDlg::IsEmpty()
{
	return m_aclist.GetCount()?FALSE:TRUE;
}
