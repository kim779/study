// CDlg_Top.cpp: 구현 파일
//

#include "pch.h"
#include "TestDLL.h"
#include "afxdialogex.h"
#include "CDlg_Top.h"


// CDlg_Top 대화 상자

IMPLEMENT_DYNAMIC(CDlg_Top, CDialogEx)

CDlg_Top::CDlg_Top(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_TOP, pParent)
{

}

CDlg_Top::~CDlg_Top()
{
	CString fileName;
}

void CDlg_Top::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_chkTest);
	DDX_Control(pDX, IDC_BTN_TEST, m_pBtnTest);
}


BEGIN_MESSAGE_MAP(CDlg_Top, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlg_Top 메시지 처리기
CBitmap* CDlg_Top::LoadFileBitmap(const char* bmpName)
{
	HBITMAP hBitmap;
	CString fileName;

	CString path(bmpName);
	path.MakeUpper();
	fileName.Format("%s\\image\\%s.bmp", m_path, bmpName);

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), fileName,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		CBitmap* bmp = new CBitmap;
		bmp->Attach(hBitmap);

		m_slog.Format("[7141] bmp = [%x]", hBitmap);


		OutputDebugString(m_slog);
		return bmp;
	}
	else
		TRACE("FAIL!!!!! = %s\n", bmpName);



	return NULL;
}

BOOL CDlg_Top::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_bmpBg = LoadFileBitmap("최선집행기준설명서교부팝업");
	m_chkTest.DrawTransparent(TRUE);

	CRect rec;
	m_pBtnTest.GetWindowRect(rec);

	m_pbtntest = new CMCButton();
	ScreenToClient(rec);
	rec.OffsetRect(rec.Width() + 10, 0);
	rec.InflateRect(rec.Width() / 2, rec.Height() / 2);
	m_pbtntest->Create("한글은?", WS_VISIBLE | WS_CHILD, rec, this, 9898);
	m_pbtntest->DrawTransparent(TRUE);

	//(HBITMAP)LoadImageW(NULL, filePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//m_pbtntest->ModifyStyle(0, BS_DEFPUSHBUTTON);
	//m_chkTest.ModifyStyle(0, BS_CHECKBOX);   //BS_DEFPUSHBUTTON

	//SetBitmaps
	CString fileNameIN, fileNameOUT;
	fileNameIN.Format("%s\\image\\%s.bmp", m_path, "BTN_APPLICATION_1");
	fileNameOUT.Format("%s\\image\\%s.bmp", m_path, "BTN_APPLICATION_2");
	m_pbtntest->SetBitmaps(fileNameIN, RGB(255,255,255), fileNameOUT, RGB(255, 255, 255));
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlg_Top::InitBtn()
{

}

void CDlg_Top::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (m_bmpBg)  //image폴더의 파일을 사용하는 경우 
	{
		CDC mdc;
		mdc.CreateCompatibleDC(&dc);

		BITMAP bm;
		m_bmpBg->GetBitmap(&bm);

		CBitmap* oldBmp = mdc.SelectObject(m_bmpBg);

		int ix = bm.bmWidth;
		int iy = bm.bmHeight;
		//// 새 DPI에 맞춰 크기 조정
	/*	bm.bmWidth = MulDiv(bm.bmWidth, m_xdpi, 96);
		bm.bmHeight = MulDiv(bm.bmHeight, m_ydpi, 96);*/

		//dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &mdc, 0, 0, SRCCOPY);

		dc.StretchBlt(0, 0, bm.bmWidth, bm.bmHeight,
			&mdc, 0, 0, ix, iy, SRCCOPY);

		mdc.SelectObject(oldBmp);
		mdc.DeleteDC();

		GetDlgItem(IDC_CHECK1)->Invalidate();

		//SetWindowPos(&CWnd::wndTopMost, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE);
	}
}


HBRUSH CDlg_Top::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	const int nCtrlID = pWnd->GetDlgCtrlID();
	if (nCtrlID == IDC_CHECK1)
	{
		pDC->SetBkMode(TRANSPARENT); // 배경 투명 설정
		return (HBRUSH)GetStockObject(NULL_BRUSH); // 배경을 투명하게 설정  
	}
	return hbr;
}


BOOL CDlg_Top::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}
