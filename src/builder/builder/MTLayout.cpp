// MTLayout.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisBuilder.h"
#include "MTLayout.h"
#include "afxdialogex.h"
#include "h/mainvar.h"

// CMTLayout 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMTLayout, CDialogEx)

CMTLayout::CMTLayout(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMTLayout::IDD, pParent)
{
	m_iSel = 0;
	m_iStart = 0;
	m_iMax = 0;
	InitPreview();
}

CMTLayout::~CMTLayout()
{
	m_aryWnd.RemoveAll();
}

void CMTLayout::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC_PREVIEW1, m_picPreview1);
	DDX_Control(pDX, IDC_PIC_PREVIEW2, m_picPreview2);
	DDX_Control(pDX, IDC_PIC_PREVIEW3, m_picPreview3);
}


BEGIN_MESSAGE_MAP(CMTLayout, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_NEXTP, &CMTLayout::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PRE, &CMTLayout::OnBnClickedButtonPre)
	ON_MESSAGE(WM_OBJMSG, &CMTLayout::OnObjMsg)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CMTLayout::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CMTLayout 메시지 처리기입니다.

void CMTLayout::Init()
{
	m_aryWnd.RemoveAll();
	CWnd *pWnd = NULL;
	for (int ii = 0; ii < PREVIEW_CNT; ii++)
	{
		pWnd = GetDlgItem(IDC_PIC_PREVIEW1 + ii);
		m_aryWnd.Add(pWnd);
	}
}

BOOL CMTLayout::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Init();
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_MAPPREVIEW, 0);

	CWnd *pWnd = GetDlgItem(IDC_PIC_PREVIEW1);
	pWnd->GetWindowRect(m_rc);
	ScreenToClient(m_rc);
	m_rc.top -= 1;
	m_rc.left -= 1;

	GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
	if (m_aryWnd.GetCount() < m_iMax)
		GetDlgItem(IDC_BUTTON_NEXTP)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_BUTTON_NEXTP)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMTLayout::InitPreview()
{
	for (int ii = 0; ii < PREVIEW_CNT; ii++)
	{
		m_clr[ii] = NULL;
		m_img[ii] = _T("");
	}
}

void CMTLayout::SetBkClr(int idx, COLORREF clr)
{
	m_clr[idx] = clr;
}

void CMTLayout::SetBkImg(int idx, CString strImg)
{
	m_img[idx] = strImg;
}

void CMTLayout::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CWnd *pWnd = NULL;
	CRect rc, UpdateRc;
	UpdateRc.CopyRect(m_rc);
	UpdateRc.InflateRect(1, 1);
	InvalidateRect(UpdateRc);
	for (int ii = 0; ii < m_aryWnd.GetCount(); ii++)
	{
		pWnd = m_aryWnd.GetAt(ii);
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		if (rc.PtInRect(point))
		{
			m_iSel = ii;
			m_rc.CopyRect(rc);
			m_rc.top -= 1;
			m_rc.left -= 1;
			break;
		}
	}
	UpdateRc.CopyRect(m_rc);
	UpdateRc.InflateRect(1, 1);
	InvalidateRect(UpdateRc);
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CMTLayout::OnBnClickedButtonNext()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iStart += m_aryWnd.GetCount();
	m_iStart = m_iStart > m_iMax ? (m_iMax - 1) : m_iStart;
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_MAPPREVIEW, m_iStart);

	GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(TRUE);
	if ((m_iStart + m_aryWnd.GetCount()) >= m_iMax)
		GetDlgItem(IDC_BUTTON_NEXTP)->EnableWindow(FALSE);
	Invalidate();
}


void CMTLayout::OnBnClickedButtonPre()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iStart -= m_aryWnd.GetCount();
	m_iStart = m_iStart < 0 ? 0 : m_iStart;
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_MAPPREVIEW, m_iStart);

	GetDlgItem(IDC_BUTTON_NEXTP)->EnableWindow(TRUE);
	if (m_iStart == 0)
		GetDlgItem(IDC_BUTTON_PRE)->EnableWindow(FALSE);
	Invalidate();
}

void CMTLayout::SetMaxCnt(int iMax)
{
	m_iMax = iMax;
	if (m_aryWnd.GetCount() < m_iMax)
		GetDlgItem(IDC_BUTTON_NEXTP)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_BUTTON_NEXTP)->EnableWindow(FALSE);
}

LRESULT CMTLayout::OnObjMsg(WPARAM wp, LPARAM lp) // 오브젝트 이벤트 후킹
{
	CPoint pt;
	switch (LOWORD(wp))
	{
	case WM_LBUTTONDOWN:
		pt.x = LOWORD(lp);
		pt.y = HIWORD(lp);
		OnLButtonDown(0, pt);
		break;
	}

	return 0;
}

void CMTLayout::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

	CWnd *pWnd = NULL;
	CRect rc, UpdateRc;

	for (int ii = 0; ii < m_aryWnd.GetCount(); ii++)
	{
		if (m_clr[ii] != NULL)
		{
			CBrush m_edit_bk_brush;
			m_edit_bk_brush.CreateSolidBrush(m_clr[ii]);
			pWnd = m_aryWnd.GetAt(ii);
			pWnd->GetWindowRect(rc);
			ScreenToClient(rc);
			dc.FillRect(rc, &m_edit_bk_brush);
			m_edit_bk_brush.DeleteObject();
		}
		else if (m_img[ii] != _T(""))
		{
			pWnd = m_aryWnd.GetAt(ii);
			pWnd->GetWindowRect(rc);
			ScreenToClient(rc);

			HBITMAP hImgBack = (HBITMAP)LoadImage(NULL, m_img[ii], IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			CBitmap bitmap;
			CBrush* brush = new CBrush(bitmap.FromHandle(hImgBack));
			CBrush* oldBrush = dc.SelectObject(brush);
			dc.PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);
			dc.SelectObject(oldBrush);
			DeleteObject(hImgBack);
			delete brush;
		}
	}

	dc.MoveTo(m_rc.left, m_rc.top);
	dc.LineTo(m_rc.right + 1, m_rc.top);

	dc.MoveTo(m_rc.right, m_rc.top);
	dc.LineTo(m_rc.right, m_rc.bottom + 1);

	dc.MoveTo(m_rc.left, m_rc.bottom);
	dc.LineTo(m_rc.right, m_rc.bottom);

	dc.MoveTo(m_rc.left, m_rc.top);
	dc.LineTo(m_rc.left, m_rc.bottom + 1);

	//dc.FrameRect(m_rc, WHITE_BRUSH);
}

BOOL CMTLayout::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// 키보드가 눌렸을때.
	if (pMsg->message == WM_KEYDOWN)
	{

		if (pMsg->wParam == VK_DELETE)
		{
			PreviewDelete();
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CMTLayout::PreviewDelete()
{
	if (m_iMax <= m_iStart + m_iSel)
		return;

	AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_DELPREVIEW, m_iStart), m_iSel);

	CWnd *pWnd = NULL;
	CRect rc, UpdateRc;
	for (int ii = 0; ii < m_aryWnd.GetCount(); ii++)
	{
		pWnd = m_aryWnd.GetAt(ii);
		pWnd->GetWindowRect(rc);
		ScreenToClient(rc);
		rc.InflateRect(1, 1);
		InvalidateRect(rc);
	}
}

void CMTLayout::OnBnClickedButtonDel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	PreviewDelete();
}