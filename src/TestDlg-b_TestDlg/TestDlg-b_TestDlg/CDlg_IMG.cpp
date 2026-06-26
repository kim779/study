// CDlg_IMG.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlg_IMG.h"
#include "afxdialogex.h"


// CDlg_IMG 대화 상자

IMPLEMENT_DYNAMIC(CDlg_IMG, CDialogEx)

CDlg_IMG::CDlg_IMG(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_IMG, pParent)
{

}

CDlg_IMG::~CDlg_IMG()
{
	GdiplusShutdown(gdiplusToken);
}

void CDlg_IMG::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_IMG, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg_IMG::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CDlg_IMG::OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlg_IMG 메시지 처리기


void CDlg_IMG::OnBnClickedButton1()
{

}


void CDlg_IMG::OnBnClickedOk()
{

	CDialogEx::OnOK();
}
void DrawBitmapWithGDIPlus(Graphics* pGraphics, HBITMAP hBmp, int x, int y, int width, int height)
{
	Bitmap bmp(hBmp, NULL);

	// Set high-quality interpolation mode for better scaling.
	pGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);

	// Draw the image scaled.
	pGraphics->DrawImage(&bmp, x, y, width, height);
}
#define	wIMAGE		20
void CDlg_IMG::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	CRect rec1, rec2, rec3;
	((CWnd*)GetDlgItem(IDC_ST_IMG1))->GetWindowRect(rec1);
	ScreenToClient(rec1);
	((CWnd*)GetDlgItem(IDC_ST_IMG2))->GetWindowRect(rec2);
	ScreenToClient(rec2);
	//((CWnd*)GetDlgItem(IDC_ST_IMG3))->GetWindowRect(rec3);
	//ScreenToClient(rec3);
	rec3 = rec2;
	rec3.OffsetRect(rec2.Width(), 0);
	//{
	//	CDC memDC;

	//	CBitmap* pOldBmp = nullptr;
	//	BITMAP bmpInfo;
	//	memDC.CreateCompatibleDC(&dc);

	//	m_pBltBmp->GetBitmap(&bmpInfo);
	//	if (memDC.m_hDC != NULL)
	//	{
	//		pOldBmp = (CBitmap*)memDC.SelectObject(m_pBltBmp);
	//		::TransparentBlt(dc.m_hDC, rec1.left, rec1.top, rec1.Width(), rec1.Height(), memDC.m_hDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, RGB(255, 0, 255));
	//	}

	//	if (pOldBmp)
	//		memDC.SelectObject(pOldBmp);


	//	dc.SetStretchBltMode(HALFTONE);
	//	m_pStrechBmp->GetBitmap(&bmpInfo);
	//	if (memDC.m_hDC != NULL)
	//	{
	//		pOldBmp = (CBitmap*)memDC.SelectObject(m_pStrechBmp);
	//		dc.StretchBlt(rec2.left, rec2.top, rec2.Width(), rec2.Height(),
	//			&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);
	//	}

	//	if (pOldBmp)
	//		memDC.SelectObject(pOldBmp);

	//

	//	Graphics graphics(dc.GetSafeHdc());
	//	// BMP 파일 로드
	//	if (m_hBmp)
	//	{
	//		// 원하는 위치와 크기에 이미지를 그립니다.
	//		DrawBitmapWithGDIPlus(&graphics, m_hBmp, rec3.left, rec3.top, rec3.Width(), rec3.Height());
	//		
	//		// 리소스 해제
	//		::DeleteObject(m_hBmp);
	//		memDC.DeleteDC();
	//	}
	//}
	
	CRect rcback;
	GetClientRect(rcback);
	dc.FillSolidRect(rcback, RGB(255,0,0));

	//투명
	CString sImageDir;  //ARROW.BMP
	sImageDir = "F:\\util\\HTS\\IBK투자증권 HTS\\image\\apple.bmp";

	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	CDC memDC, maskDC;
	memDC.CreateCompatibleDC(&dc);
	

	CBitmap* pBitmap = CBitmap::FromHandle(BitMap);
	BITMAP bitmap;
	pBitmap->GetBitmap(&bitmap);


	// 원본 비트맵 선택
	CBitmap* pOldBitmap = memDC.SelectObject(pBitmap);

	::TransparentBlt(dc.m_hDC, 0, 0, wIMAGE, wIMAGE, memDC.m_hDC, 0, 0, wIMAGE, wIMAGE, RGB(255, 0, 255));
	memDC.SelectObject(pOldBitmap);
}


BOOL CDlg_IMG::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//_mbitmaps = std::make_shared<MBITMAP>();
	bool key = true;
	CString strKey = "F:\\util\\HTS\\IBK투자증권 HTS\\image\\BG_BLUE.BMP";
	bool bStaff{};
	auto [it, bret] = _mbitmaps.emplace(std::move(std::make_pair(strKey, std::make_tuple(
		std::make_shared<CBitmap>(),  true, bStaff))));

   bool bval = std::get<1>(it->second);


   for_each(_mbitmaps.begin(), _mbitmaps.end(), [&](const auto& item) {

	   });

   
	for_each(_mbitmaps.begin(), _mbitmaps.end(), [&](const auto& item) {
		auto at = std::get<1>(item.second);
		if (std::get<1>(item.second))
		{
			CString str;
			str.Format("%s_%s", "blue", item.first);

			if (std::get<2>(item.second))
				str = "STAFF_" + str;

		
		}
		});



	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	CRect rec;
	((CWnd*)GetDlgItem(IDC_ST_IMG1))->ShowWindow(SW_HIDE);
	((CWnd*)GetDlgItem(IDC_ST_IMG2))->ShowWindow(SW_HIDE);
	//((CWnd*)GetDlgItem(IDC_ST_IMG3))->ShowWindow(SW_HIDE);

	CString m_strimagepath;
	m_strimagepath = "F:\\util\\HTS\\IBK투자증권 HTS\\image\\event01.bmp";
	HBITMAP hBitmap, hbitmap;

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_strimagepath,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
		m_pBltBmp = CBitmap::FromHandle(hBitmap);

	hbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_strimagepath,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hbitmap)
		m_pStrechBmp = CBitmap::FromHandle(hbitmap);

	m_hBmp  = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
		_T("F:\\util\\HTS\\IBK투자증권 HTS\\image\\event01.bmp"),
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE);

	m_strimagepath = "F:\\util\\HTS\\IBK투자증권 HTS\\image\\apple.bmp";
	HBITMAP htramsbitmap;
	htramsbitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_strimagepath,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hbitmap)
		m_pBitTrans = CBitmap::FromHandle(htramsbitmap);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDlg_IMG::OnEraseBkgnd(CDC* pDC)
{

	//return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}
