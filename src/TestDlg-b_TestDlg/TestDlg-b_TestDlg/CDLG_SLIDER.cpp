// CDLG_SLIDER.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDLG_SLIDER.h"
#include "afxdialogex.h"


// CDLG_SLIDER 대화 상자

IMPLEMENT_DYNAMIC(CDLG_SLIDER, CDialog)

#define DF_IMAX_RATE 3000
#define DF_IMIN_RATE  300
#define DF_IMAX_POS 100
#define DF_IMIN_POS 30


CDLG_SLIDER::CDLG_SLIDER(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_SLIDER, pParent)
{

}

CDLG_SLIDER::~CDLG_SLIDER()
{
}

void CDLG_SLIDER::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_RATE, m_sliders);
	DDX_Control(pDX, IDC_COMBO1, m_cb);
}


BEGIN_MESSAGE_MAP(CDLG_SLIDER, CDialog)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_RATE, &CDLG_SLIDER::OnNMCustomdrawSliderRate)
	ON_BN_CLICKED(IDOK, &CDLG_SLIDER::OnBnClickedOk)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDLG_SLIDER::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CDLG_SLIDER 메시지 처리기


void CDLG_SLIDER::OnNMCustomdrawSliderRate(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//*pResult = 0;

	NMCUSTOMDRAW* pDraw = reinterpret_cast<NMCUSTOMDRAW*>(pNMHDR);
	switch (pDraw->dwDrawStage) {
	case CDDS_PREPAINT:
	{
		
		*pResult = CDRF_NOTIFYITEMDRAW; // 아이템 그리기 알림을 받겠다고 알림
	}
		break;

	case CDDS_ITEMPREPAINT:
	{
		CDC* pDC = CDC::FromHandle(pDraw->hdc);
		CRect rcItem = pDraw->rc;
		rcItem.DeflateRect(3, 3); // 내부 영역 크기 조절
		HWND hwnd = pDraw->hdr.hwndFrom;

		CString slog;
		slog.Format("\r\n[%x] [%d %d %d %d]", hwnd, rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
	//	OutputDebugString(slog);
		
		// 내부 색칠
		//if (rcItem.Width() > 50)
		if (pDraw->dwItemSpec == 3)
		{
			//CBrush brush;
			//brush.CreateSolidBrush(RGB(255, 0, 0));

			//int numSteps = 20; // 그라데이션 단계 수
			//int rStep = (0 - 255) / numSteps;
			//int gStep = (0 - 255) / numSteps;
			//int bStep = (255 - 0) / numSteps;

			//	for (int i = numSteps + 1; i > 1; i--) {
			//		CRect currentRect(rcItem);
			//		currentRect.left += rcItem.right - ( i * (rcItem.Width() / numSteps));
			//		currentRect.right = currentRect.left + (rcItem.Width() / numSteps);

			//		COLORREF fillColor = RGB(255, i * gStep, 255 - i * bStep);
			//		brush.DeleteObject();
			//		brush.CreateSolidBrush(fillColor);

			//		pDC->FillRect(&currentRect, &brush);
			//	}

			/*for (int i = 0 ; i < numSteps; ++i) {
				CRect currentRect(rcItem);
				currentRect.left += i * (rcItem.Width() / numSteps);
				currentRect.right = currentRect.left + (rcItem.Width() / numSteps);

				COLORREF fillColor = RGB(255, i * gStep, 255 - i * bStep);
				brush.DeleteObject();
				brush.CreateSolidBrush(fillColor);

				pDC->FillRect(&currentRect, &brush);
			}*/
			
			((CWnd*)GetDlgItem(IDC_EDIT_SLIDER))->SetWindowText(slog);
			pDC->FillSolidRect(rcItem, RGB(195, 195, 195));
		}
		else if(pDraw->dwItemSpec == 2)
		{
			pDC->FillSolidRect(rcItem, RGB(0, 122, 217));
			((CWnd*)GetDlgItem(IDC_EDIT_SLIDER2))->SetWindowText(slog);
		}

		*pResult = CDRF_SKIPDEFAULT; // 기본 그리기 작업 건너뛰기
	}
	//	*pResult = CDRF_NOTIFYSUBITEMDRAW; // 서브 아이템 그리기 알림을 받겠다고 알림
		break;

	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
	{
		
		*pResult = CDRF_SKIPDEFAULT; // 기본 그리기 작업 건너뛰기
	}
	break;
	}
}


BOOL CDLG_SLIDER::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_sliders.SetRange(30, 100, TRUE);
	m_cb.AddString("test");
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDLG_SLIDER::OnBnClickedOk()
{
	CDialog::OnOK();
}

/*
#define DF_IMAX_RATE 3000
#define DF_IMIN_RATE  300
#define DF_IMAX_POS 100
#define DF_IMIN_POS 30
*/

void CDLG_SLIDER::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CString slog;
	CSliderCtrl* pslider = (CSliderCtrl*)pScrollBar;
	if (pslider == &m_sliders)
	{
		int isliderPos = (((DF_IMAX_RATE - DF_IMIN_RATE) / (DF_IMAX_POS - DF_IMIN_POS)) * (m_sliders.GetPos() - DF_IMIN_POS)) + DF_IMIN_RATE;
		int ipos = ((isliderPos - DF_IMIN_RATE) / ((DF_IMAX_RATE - DF_IMIN_RATE) / (DF_IMAX_POS - DF_IMIN_POS))) + DF_IMIN_POS;
		slog.Format("\r\n  [%d][%d]    [%d]", isliderPos , ipos, m_sliders.GetPos());
		((CWnd*)GetDlgItem(IDC_EDIT_SLIDER3))->SetWindowText(slog);
	/*	if (m_sliders.GetPos() > 86 && m_sliders.GetPos() <= 100)
			m_sliders.SetPos(100);
		else if (m_sliders.GetPos() > 72 && m_sliders.GetPos() <= 86)
			m_sliders.SetPos(86);
		else if (m_sliders.GetPos() > 56 && m_sliders.GetPos() <= 72)
			m_sliders.SetPos(72);
		else if (m_sliders.GetPos() > 44 && m_sliders.GetPos() <= 56)
			m_sliders.SetPos(56);
		else if (m_sliders.GetPos() > 30 && m_sliders.GetPos() <= 44)
			m_sliders.SetPos(44);*/
		OutputDebugString(slog);
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDLG_SLIDER::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	
	//NMHDR nmhdr;
	//nmhdr.code = NM_CUSTOMDRAW;
	//nmhdr.hwndFrom = m_sliders.GetSafeHwnd(); // 슬라이더 컨트롤의 핸들
	//nmhdr.idFrom = m_sliders.GetDlgCtrlID(); // 슬라이더 컨트롤의 ID

	//NMCUSTOMDRAW nmCustomDraw;
	//nmCustomDraw.hdr = nmhdr;
	//nmCustomDraw.dwDrawStage = CDDS_ITEMPREPAINT | CDDS_SUBITEM;
	////LRESULT lResult = m_sliders.SendMessage(WM_NOTIFY, nmCustomDraw.hdr.idFrom, (LPARAM)&nmCustomDraw);
	//LRESULT lResult = m_sliders.SendMessage(NM_CUSTOMDRAW, nmCustomDraw.hdr.idFrom, (LPARAM)&nmCustomDraw);
	m_sliders.SendMessage(NM_CUSTOMDRAW, 0, 0);

}



BOOL CDLG_SLIDER::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::OnNotify(wParam, lParam, pResult);
}


void CDLG_SLIDER::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
