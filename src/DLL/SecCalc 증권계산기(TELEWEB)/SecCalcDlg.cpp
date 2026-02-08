// SecCalcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SecCalc.h"
#include "SecCalcDlg.h"
#include "MyTab.h"
#include "OrderableDlg.h"
#include "TradeDlg.h"
#include "ProfitDlg.h"
#include "MyTab.h"
#include "misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSecCalcDlg dialog
#define	WND_CLEAR(wnd) if (wnd){ wnd->DestroyWindow(); delete wnd; wnd = NULL;	}

CSecCalcDlg::CSecCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSecCalcDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSecCalcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pPage1 = NULL;
	m_pPage2 = NULL;
	m_pPage3 = NULL;
	m_pTabCtrl = NULL;
}

void CSecCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSecCalcDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSecCalcDlg, CDialog)
	//{{AFX_MSG_MAP(CSecCalcDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CHILD, OnChild)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSecCalcDlg message handlers

BOOL CSecCalcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
//	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CRect	rc;
	GetClientRect(&rc);
	rc.DeflateRect(5,5);

	CreateControls(rc);
	SelectPage(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSecCalcDlg::OnDestroy() 
{
	WND_CLEAR(m_pTabCtrl);
	WND_CLEAR(m_pPage1);
	WND_CLEAR(m_pPage2);
	WND_CLEAR(m_pPage3);

	CDialog::OnDestroy();
}

long CSecCalcDlg::OnChild(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case TAB_SELCHANGE:
		SelectPage(lParam);
		break;
	}

	return 0;
}

void CSecCalcDlg::CreateControls(CRect rc)
{
	m_strRoot = GetRoot();
	m_strRoot = "C:\\Program Files\\IBKS_STAFF\\IBK hot Trading(직원용)\\";  //test

	m_strPal = GetPalette();

	m_pTabCtrl = new CMyTab();
	m_pTabCtrl->Create(rc, this, 1111);

	m_pTabCtrl->LoadTabImage(m_strRoot, false);
	m_pTabCtrl->SetColor(NULL, NULL, NULL, NULL, clContents);
	m_pTabCtrl->SetBorderColor(GetIndexColor(clTabBorder));

	m_pPage1 = new COrderableDlg(this, m_pTabCtrl);
	m_pPage1->Create(m_pPage1->IDD);

	m_pPage2 = new CTradeDlg(this, m_pTabCtrl);
	m_pPage2->Create(m_pPage2->IDD);

	m_pPage3 = new CProfitDlg(this, m_pTabCtrl);
	m_pPage3->Create(m_pPage3->IDD);

	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = _T("주문가능수량");	m_pTabCtrl->InsertItem(0, &tcItem);
	tcItem.pszText = _T("매매정산금액");	m_pTabCtrl->InsertItem(1, &tcItem);
	tcItem.pszText = _T("평가손익계산");	m_pTabCtrl->InsertItem(2, &tcItem);

	m_pTabCtrl->SetFont(GetFont());

	CRect	trc;
	m_pTabCtrl->GetItemRect(0, &trc);
	rc.top += trc.Height();
	rc.DeflateRect(5,3);
	m_pPage1->MoveWindow(rc);
	m_pPage2->MoveWindow(rc);
	m_pPage3->MoveWindow(rc);
}

CString CSecCalcDlg::GetRoot()
{
	char	pPath[128];
	GetCurrentDirectory(128, pPath);

	CString strPath;
	strPath.Format("%s", pPath);
	int a = strPath.ReverseFind('\\');

	strPath = strPath.Mid(0, a);
	strPath += "\\";

	return strPath;
}

CString CSecCalcDlg::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_strRoot, "tab", "palette.ini");

	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;

	return strPal;
}

COLORREF CSecCalcDlg::GetIndexColor(int index)
{
	char buff[24];
	CString strRGB;
	CString file;
	file.Format("%s\\%s\\%s", m_strRoot, "tab", "palette.ini");
	CString strIndex;
	strIndex.Format("RGB%03d", index);

	GetPrivateProfileString(m_strPal, strIndex, "", buff, sizeof(buff), file);
	strRGB = buff;
	
	int r, g, b;
	r = atoi(Parser(strRGB, ","));
	g = atoi(Parser(strRGB, ","));
	b = atoi(strRGB);
	
	COLORREF color = RGB(r, g, b);
	return color;
}

CString CSecCalcDlg::Parser(CString &srcstr, CString substr)
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


void CSecCalcDlg::SelectPage(int index)
{
	if (!m_pPage1 || !m_pPage2 || !m_pPage3)
		return;

	switch (index)
	{
	case 0:
		m_pPage1->ShowWindow(SW_SHOW);
		m_pPage2->ShowWindow(SW_HIDE);
		m_pPage3->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pPage1->ShowWindow(SW_HIDE);
		m_pPage2->ShowWindow(SW_SHOW);
		m_pPage3->ShowWindow(SW_HIDE);		
		break;
	case 2:
		m_pPage1->ShowWindow(SW_HIDE);
		m_pPage2->ShowWindow(SW_HIDE);
		m_pPage3->ShowWindow(SW_SHOW);		
		break;
	}
}

void CSecCalcDlg::ChangeFee()
{
	if (m_pPage1 && m_pPage1->GetSafeHwnd())
	{
		m_pPage1->OnChangeFee();
	}
	if (m_pPage2 && m_pPage2->GetSafeHwnd())
	{
		m_pPage2->OnChangeFee();
	}
	if (m_pPage3 && m_pPage3->GetSafeHwnd())
	{
		m_pPage3->OnChangeFee();
	}
}

BOOL CSecCalcDlg::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_CHAR:	break;
	case WM_KEYDOWN:
		if (pMsg->wParam == VK_RETURN)
			pMsg->wParam = VK_TAB;
		else if (pMsg->wParam == VK_ESCAPE)
			return TRUE;

		break;
	case WM_KEYUP:	break;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CSecCalcDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect cRc;
	GetClientRect(cRc);
	
	dc.FillSolidRect(cRc, GetIndexColor(clBack));

	cRc.DeflateRect(1, 1, 1, 3);
	dc.FillSolidRect(cRc, clContents);

	SetBitmap();
	DrawRoundBitmap(&dc, cRc, m_hBmpRoundPanel);
}

void CSecCalcDlg::SetBitmap()
{
	CBitmap* BmpRoundPanel[4];
	
	CString strPath;

	strPath.Format("%s%s\\%s", m_strRoot, "image", "axpanel" + m_strPal + "0_lt.bmp");
	BmpRoundPanel[0] = CreateBmp(strPath);

	strPath.Format("%s%s\\%s", m_strRoot, "image", "axpanel" + m_strPal + "0_rt.bmp");
	BmpRoundPanel[1] = CreateBmp(strPath);

	strPath.Format("%s%s\\%s", m_strRoot, "image", "axpanel" + m_strPal + "0_lb.bmp");
	BmpRoundPanel[2] = CreateBmp(strPath);
	
	strPath.Format("%s%s\\%s", m_strRoot, "image", "axpanel" + m_strPal + "0_rb.bmp");
	BmpRoundPanel[3] = CreateBmp(strPath);

	for (int i = 0; i < 4; i++)
		m_hBmpRoundPanel[i] = BmpRoundPanel[i]->operator HBITMAP();

	for (i = 0; i < 4; i++)
		DeleteObject(BmpRoundPanel[i]);
}

CBitmap* CSecCalcDlg::CreateBmp(LPCTSTR sBMPFile)
{
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead) )
		return NULL;

	BITMAPFILEHEADER bmfHeader;

	// Read file header
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return NULL;

	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return NULL;

	// Get length of the remainder of the file and allocate memory
	DWORD nPackedDIBLen = file.GetLength() - sizeof(BITMAPFILEHEADER);
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
		return NULL;

	// Read the remainder of the bitmap file.
	if (file.Read((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )  //test
	{
		::GlobalFree(hDIB);
		return NULL;
	}

	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;

	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed :
						1 << bmiHeader.biBitCount;

	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) +
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	CClientDC dc(NULL);

	HBITMAP hBmp = CreateDIBitmap( dc.m_hDC, &bmiHeader, CBM_INIT, lpDIBBits, &bmInfo, DIB_RGB_COLORS);
	CBitmap *bitmap;
	bitmap = new CBitmap();
	bitmap->Attach( hBmp );

	::GlobalFree(hDIB);
	return bitmap;
}


void CSecCalcDlg::DrawRoundBitmap(CDC *dc, CRect rc, HBITMAP *hBitmap)
{
	CRect cRc;
	//Round left + top
	cRc.SetRect(rc.left, rc.top, 
				rc.left + 5, rc.top + 5);
	DrawBitmap(dc, cRc, hBitmap[0], true);

	//Round right + top
	cRc.SetRect(rc.right - 5, rc.top, 
				rc.right, rc.top + 5);
	DrawBitmap(dc, cRc, hBitmap[1], true);
	
	//Round left + bottom
	cRc.SetRect(rc.left, rc.bottom - 5, 
				rc.left + 5, rc.bottom);
	DrawBitmap(dc, cRc, hBitmap[2], true);

	//Round right + top
	cRc.SetRect(rc.right - 5, rc.bottom - 5, 
				rc.right, rc.bottom);
	DrawBitmap(dc, cRc, hBitmap[3], true);
}

#pragma comment (lib, "Msimg32.lib")
void CSecCalcDlg::DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap, bool bTrans)
{
	CDC		memDC;
	CBitmap		*pOldBitmap, *pBitmap;
	
	pBitmap = CBitmap::FromHandle(hBitmap);
	
	memDC.CreateCompatibleDC(pDC);	
	if (memDC.m_hDC != NULL) 
		pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
	
	if(bTrans)
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	else
		::BitBlt( pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(),	memDC.m_hDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}



