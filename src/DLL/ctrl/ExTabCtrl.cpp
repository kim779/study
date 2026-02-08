#include "stdafx.h"
#include "ExTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExTabCtrl::CExTabCtrl()
{
	//m_clrUnSel = CLR_UNSEL;
	//m_clrSel = CLR_SEL;
	m_clrUnSel = GetSysColor(COLOR_3DFACE);
	m_clrSel = GetSysColor(COLOR_3DFACE);
	m_clrFontUnSel = CLR_FONTUNSEL;
	m_clrFontSel = CLR_FONTSEL;
	m_clrBack = GetSysColor(COLOR_3DFACE);
	m_pImageList = NULL;
}

CExTabCtrl::~CExTabCtrl()
{
	if (m_pImageList)
	{
		m_pImageList->DeleteImageList();
		delete m_pImageList;
	}
}


BEGIN_MESSAGE_MAP(CExTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CExTabCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExTabCtrl::InitTab(CString* tName, CDialog** pDlg, UINT* idcX, int cnt)
{
	for (int ii = 0; ii < cnt; ii++)
	{
		InsertItem(ii, tName[ii], ii);
		ASSERT(pDlg[ii]);
		m_pPage[ii] = pDlg[ii];
		m_pPage[ii]->Create(idcX[ii], this);
		m_pPage[ii]->ShowWindow(SW_HIDE);
	}
	m_nIndex = 0;
}

void CExTabCtrl::ChangeTab(int nIndex)
{
	m_pPage[m_nIndex]->EnableWindow(FALSE);
	m_pPage[m_nIndex]->ShowWindow(SW_HIDE);
	m_nIndex = nIndex;
	m_pPage[m_nIndex]->EnableWindow(TRUE);
	m_pPage[m_nIndex]->ShowWindow(SW_SHOW);
	resizeTab();
}

void CExTabCtrl::resizeTab()
{
	CRect tabRect, itemRect;
	int nX{}, nY{}, nXc{}, nYc{};

	GetClientRect(&tabRect);
	GetItemRect(0, &itemRect);

	nX = itemRect.left;
	nY = itemRect.bottom+1;
	nXc = tabRect.right-itemRect.left-1;
	nYc = tabRect.bottom-nY-1;
	m_pPage[m_nIndex]->SetWindowPos(&wndTop, nX, nY, nXc, nYc, SWP_SHOWWINDOW);
}

void CExTabCtrl::SetColours(COLORREF clrUnSel, COLORREF clrSel, COLORREF clrBack)
{
	m_clrUnSel = clrUnSel;
	m_clrSel = clrSel;
	m_clrBack = clrBack;
	
	Invalidate();
}

void CExTabCtrl::SetOwnerDraw()
{
	ModifyStyle(0, TCS_OWNERDRAWFIXED);
	Invalidate();
}

void CExTabCtrl::PreSubclassWindow() 
{
	CTabCtrl::PreSubclassWindow();

	//ModifyStyle(0, TCS_OWNERDRAWFIXED);
}


void CExTabCtrl::InitTabImage(CBitmap& bm)
{
	CBitmap& bitmap = bm;
	

	m_pImageList = new CImageList();
	m_pImageList->Create(16, 16, ILC_COLORDDB | ILC_MASK, 4, 1); 
	m_pImageList->Add(&bitmap, RGB(255, 0, 255)); 
	SetImageList(m_pImageList); 
}

void CExTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	const int nTabIndex = lpDrawItemStruct->itemID;
	if (nTabIndex < 0) 
		return;
	const BOOL bSelected = (nTabIndex == GetCurSel());
	
	char label[64]{};
	TC_ITEM tci;
	tci.mask = TCIF_TEXT |TCIF_IMAGE | TCIF_PARAM;
	tci.pszText = label;     
	tci.cchTextMax = 63; 
	if (!GetItem(nTabIndex, &tci)) 
		return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	if (!pDC) 
		return;
	const int nSavedDC = pDC->SaveDC();
	
	// tab 바탕색
	pDC->SetBkMode(TRANSPARENT);

	CRect bRect, wRect, rRect;
	GetClientRect(&wRect);
	GetItemRect((UINT)(GetItemCount()-1), bRect);
	rRect = newRect(bRect.right, wRect.right, 0, bRect.bottom);
	pDC->FillSolidRect(rRect, m_clrBack);
	
	CRect iRect, rect;
	GetItemRect(lpDrawItemStruct->itemID, iRect);
	rect = lpDrawItemStruct->rcItem;

	if (bSelected)
	{
		rRect = newRect(iRect.left, iRect.right, 0, rect.top + 1);
		pDC->FillSolidRect(rRect, m_clrBack);
		pDC->FillSolidRect(rect, m_clrSel);
	}
	else
	{
		rRect = newRect(iRect.left, iRect.right, 0, rect.top + 1);
		pDC->FillSolidRect(rRect, m_clrBack);
		rRect = newRect(iRect.left, iRect.right, rect.bottom, rect.bottom + 5);
		pDC->FillSolidRect(rRect, m_clrUnSel);

		if (nTabIndex == 0)
		{
			rRect = newRect(0, rect.left, 0, iRect.bottom);
			pDC->FillSolidRect(rRect, m_clrBack);
		}

		pDC->FillSolidRect(rect, m_clrUnSel);
	}
	

	IMAGEINFO info{};
	CRect ImageRect;
	// image
	if (m_pImageList) 
	{
		rect.left += pDC->GetTextExtent(_T(" ")).cx;

		m_pImageList->GetImageInfo(tci.iImage, &info);
		ImageRect = info.rcImage;
		const int nYpos = rect.top + 2;

		m_pImageList->Draw(pDC, tci.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);
		rect.left += ImageRect.Width();
	}

	// tab 글자색
	if (bSelected) 
	{
		pDC->SetTextColor(m_clrFontSel);
		rect.top -= ::GetSystemMetrics(SM_CYEDGE);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	} 
	else 
	{
		pDC->SetTextColor(m_clrFontUnSel);
		pDC->DrawText(label, rect, DT_SINGLELINE|DT_BOTTOM|DT_CENTER);
	}

	pDC->RestoreDC(nSavedDC);
}


CRect CExTabCtrl::newRect(int left, int right, int top, int bottom) 
{
	CRect	rRect;

	rRect.left = left;
	rRect.right = right;
	rRect.top = top;
	rRect.bottom = bottom;

	return rRect;
}
