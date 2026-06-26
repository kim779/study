// CDlg_chart.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlg_chart.h"
#include "afxdialogex.h"


// CDlg_chart 대화 상자

IMPLEMENT_DYNAMIC(CDlg_chart, CDialog)

CDlg_chart::CDlg_chart(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_CHART, pParent)
{

}

CDlg_chart::~CDlg_chart()
{
}

void CDlg_chart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_chart, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg_chart::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDlg_chart 메시지 처리기


BOOL CDlg_chart::OnInitDialog()
{
	CDialog::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

CFont* CDlg_chart::getfont(CString fname, int ipoint, bool bBold)
{
	CString	keys = _T("");
	CFont* font = nullptr;

	keys.Format(_T("%s:%d:%c:%d"), fname, ipoint, 'i', bBold);
	if (m_pointFontObs.Lookup(keys, (CObject*&)font))
	{
		LOGFONT	logfont;

		font = new CFont();
		logfont.lfHeight = ipoint * 10 + (ipoint < 9 ? 5 : 0);
		logfont.lfWidth = 0;
		logfont.lfEscapement = 0;
		logfont.lfOrientation = 0;
		logfont.lfWeight = bBold;
		logfont.lfItalic = false;
		logfont.lfUnderline = false;
		logfont.lfStrikeOut = false;
		logfont.lfCharSet = HANGUL_CHARSET;
		logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logfont.lfQuality = DEFAULT_QUALITY;
		logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
		strcpy_s(logfont.lfFaceName, LF_FACESIZE, (LPSTR)(LPCTSTR)fname);

		font->CreatePointFontIndirect(&logfont);
		m_pointFontObs.SetAt(keys, font);
	}
	return font;
}

CPen* CDlg_chart::getpen(int style, int width, COLORREF clr)
{
	_penR rPen;
	rPen.clr = clr;
	rPen.width = width;
	rPen.style = style;

	CString	keys = _T("");
	CPen* pen = nullptr;

	keys.Format(_T("%d:%d:%d"), rPen.clr, rPen.width, rPen.style);
	if (!m_penObs.Lookup(keys, (CObject*&)pen))
	{
		pen = new CPen();
		pen->CreatePen(rPen.style, rPen.width, rPen.clr);
		m_penObs.SetAt(keys, pen);
	}
	return pen;

}

void CDlg_chart::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.

	CPen* pPen = getpen(PS_DASH, 1, RGB(255,0,0));
	CFont* tkFont = getfont("굴림체", 10, false);
	CFont* poldfont;

	CRect rect;
	GetClientRect(rect);
	rect.DeflateRect(5, 5);
	dc.Rectangle(rect);

	CPen* poldPen = dc.SelectObject(pPen);
	dc.MoveTo(rect.left + 10, rect.top);
	dc.LineTo(rect.left + 10, rect.bottom - DF_HEIGHT_DISPLAYTEXT);
	dc.SelectObject(poldPen);

	poldfont = dc.SelectObject(tkFont);
	COLORREF sColor = dc.SetTextColor(RGB(255, 0, 255));
	CRect rec;
	rec.left = rect.left + 5;
	rec.top = rect.bottom - 20;
	rec.right = rect.left + 100;
	rec.bottom = rect.bottom -5 ;
	//dc.Rectangle(rec);
	//rec.DeflateRect(1, 1);
	dc.DrawText("test", rec, DT_SINGLELINE | DT_VCENTER | DT_LEFT);


	dc.SelectObject(poldfont);
	dc.SetTextColor(sColor);

}

//#define DF_FIGURE 1
void CDlg_chart::OnBnClickedButton1()
{
#ifdef DF_FIGURE
	//cliping 도형
	CRgn rgn;
	rgn.CreateRectRgn(10, 10, 100, 100);

	CDC* pDC = GetDC();
	pDC->SelectClipRgn(&rgn);

	// Draw some graphics that will be clipped by the region
	pDC->Rectangle(0, 0, 50, 50);
	pDC->Ellipse(25, 25, 75, 75);

	// Clean up
	ReleaseDC(pDC);
	DeleteObject(rgn);

#else //윈도우 RGN
	HRGN hrgn = ::CreateEllipticRgn(0, 0, 150, 150);
	SetWindowRgn( hrgn, TRUE);
	DeleteObject(hrgn);


#endif
}
