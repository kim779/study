// Clbox.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "Clbox.h"


// Clbox

IMPLEMENT_DYNAMIC(Clbox, CListBox)

Clbox::Clbox()
{

}

Clbox::~Clbox()
{
}


BEGIN_MESSAGE_MAP(Clbox, CListBox)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// Clbox 메시지 처리기




void Clbox::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CListBox::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


void Clbox::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CListBox::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}


void Clbox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	CDC	dc;

	if (!dc.Attach(lpDrawItemStruct->hDC))
		return;

	CFont* font = dc.SelectObject(&m_font);
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		dc.SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, GetSysColor(COLOR_HIGHLIGHT));
	}
	else
	{
		dc.SetTextColor(RGB(255,0,0));
		dc.SetBkColor(RGB(255,255,255));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255,0,255));
	}

	if (lpDrawItemStruct->itemID != -1)
	{
		CString	text;
		CRect	rc;

		rc = lpDrawItemStruct->rcItem;
		GetText(lpDrawItemStruct->itemID, text);

		dc.SetBkMode(TRANSPARENT);
		rc.left += 3;
		text.Replace("&", "&&");
		dc.DrawText(text, rc, DT_VCENTER | DT_SINGLELINE);
	}

	dc.SelectObject(font);
	dc.Detach();
}


void Clbox::MeasureItem(LPMEASUREITEMSTRUCT pMeasureItemStruct)
{
	if (pMeasureItemStruct->CtlType == ODT_LISTBOX)
		pMeasureItemStruct->itemHeight = 20;
}


int Clbox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	LOGFONT	lf;

	lf.lfHeight = 9 * 10;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(lf.lfFaceName, LF_FACESIZE, "굴림체");

	m_font.DeleteObject();
	m_font.CreatePointFontIndirect(&lf);

	SetFont(&m_font);
	return 0;
}
