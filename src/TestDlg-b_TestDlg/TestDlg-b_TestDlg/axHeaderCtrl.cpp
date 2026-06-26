// SkinHeaderCtrl.cpp : implementation file
//


#include "pch.h"
#include "axHeaderCtrl.h"
#include "resource.h"

const std::string sortUp   = _T("▲ ");
const std::string sortDown = _T("▼ ");

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinHeaderCtrl

CAxHeaderCtrl::CAxHeaderCtrl()
{
	_font.CreatePointFont(90, _T("굴림체"));
	m_hWnd = 0;
}

CAxHeaderCtrl::~CAxHeaderCtrl()
{
	_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CAxHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CSkinHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_MEASUREITEM()
	ON_MESSAGE(HDM_LAYOUT, &CAxHeaderCtrl::OnLayOut)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinHeaderCtrl message handlers

void CAxHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// not use...
}

void CAxHeaderCtrl::OnPaint()
{
	// 리스트 컨트롤 헤더를 그려줌...
	CPaintDC dc(this); // device context for painting
	
	CRect rect, rc;
	GetClientRect(&rect);
		
	dc.FillSolidRect(&rect, _bgclr);
	
	const int nItems = GetItemCount();	// 탭의 갯수 ...
	for(int ii = 0; ii < nItems; ii++)
	{
		TCHAR buf[256]{};
		HD_ITEM hditem1;
		
		hditem1.mask = HDI_TEXT | HDI_FORMAT | HDI_ORDER;
		hditem1.pszText = buf;
		hditem1.cchTextMax = 255;
		GetItem(ii, &hditem1 );		
		GetItemRect(ii, &rect);
		
		const UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_CENTER;

		CString szText(buf);
		szText.Trim();
		CFont *old = dc.SelectObject(&_font);
		dc.SetTextColor(_Txtclr);
		if (ii == _sortColumn)
		{
			if (_bAscending ==  1)
				szText = CString(sortUp.c_str()) + szText;
			else if (_bAscending == -1)
				szText = CString(sortDown.c_str()) + szText;
		}
		dc.DrawText(szText, rect, uFormat);
		dc.SelectObject(old);
	}

}

BOOL CAxHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return false;
	//return CHeaderCtrl::OnEraseBkgnd(pDC);
}


void CAxHeaderCtrl::PreSubclassWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
 	CHeaderCtrl::PreSubclassWindow();
}

afx_msg LRESULT CAxHeaderCtrl::OnLayOut(WPARAM wParam, LPARAM lParam)
{
	LRESULT val = DefWindowProc(HDM_LAYOUT, 0, lParam);

	LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;
	RECT* prc = lphdlayout->prc;
	WINDOWPOS* pwpos = lphdlayout->pwpos;

	prc->top  = _height;
	pwpos->cy = _height;

	return val;
}

void CAxHeaderCtrl::SetSortArrow(int nCol, int Ascending)
{
	_sortColumn = nCol;
	_bAscending = Ascending;
	Invalidate();

}

int CAxHeaderCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CHeaderCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	return 0;
}


void CAxHeaderCtrl::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CHeaderCtrl::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}



