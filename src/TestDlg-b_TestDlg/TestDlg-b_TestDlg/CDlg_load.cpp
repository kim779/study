// CDlg_load.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlg_load.h"
#include "afxdialogex.h"


// CDlg_load 대화 상자

IMPLEMENT_DYNAMIC(CDlg_load, CDialogEx)

CDlg_load::CDlg_load(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_LOAD, pParent)
{

}

CDlg_load::~CDlg_load()
{
}

void CDlg_load::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_load, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlg_load::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &CDlg_load::OnBnClickedCheck1)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlg_load 메시지 처리기


BOOL CDlg_load::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	/*CWnd* (APIENTRY * axCreate)(CWnd*, void*);

	CString spath;
	spath.Format("%s", "D:\\IBKS_TEST\\dev\\CX_WEBEDGE.dll");
	HINSTANCE instance = AfxLoadLibrary(spath);
	int ierr = GetLastError();
	ierr = GetLastError();

	axCreate = (CWnd * (APIENTRY*)(CWnd*, void*))GetProcAddress(instance, _T("axCreate"));

	CWnd* pwnd;
	pwnd = (*axCreate)(this, nullptr);*/
	m_bmpBg = LoadFileBitmap("");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlg_load::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CDlg_load::OnBnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


BOOL CDlg_load::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}


HBRUSH CDlg_load::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	const int nCtrlID = pWnd->GetDlgCtrlID();
	if (nCtrlID == IDC_CHECK1)
	{
		OutputDebugString("[transparent]!!!!!!");
		pDC->SetBkMode(TRANSPARENT); // 배경 투명 설정
		//pDC->SelectObject(m_chbrush);
		return (HBRUSH)GetStockObject(NULL_BRUSH); // 배경을 투명하게 설정
	}
	return hbr;

}


void CDlg_load::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
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
		//bm.bmWidth = MulDiv(bm.bmWidth, m_xdpi, 96);
		//bm.bmHeight = MulDiv(bm.bmHeight, m_ydpi, 96);

		//dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &mdc, 0, 0, SRCCOPY);

		dc.StretchBlt(0, 0, bm.bmWidth, bm.bmHeight,
			&mdc, 0, 0, ix, iy, SRCCOPY);

		mdc.SelectObject(oldBmp);
		mdc.DeleteDC();



		//SetWindowPos(&CWnd::wndTopMost, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE);
	}
}

CBitmap* CDlg_load::LoadFileBitmap(const char* bmpName)
{
	HBITMAP hBitmap;
	CString fileName;

	CString path(bmpName);
	path.MakeUpper();
	fileName.Format("F:\\util\\HTS\\IBK투자증권 HTS\\image\\%s.bmp", "최선집행기준설명서교부팝업");

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