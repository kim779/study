// Copyright (c) Iuri Apollonio 1998
// Use & modify as you want & need, and leave those 3 lines.
// http://www.codeguru.com


// GfxOutBarCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "../resource.h"
#include "GfxOutBarCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ID_GFX_SMALLICON                50000
#define ID_GFX_LARGEICON                50001
#define ID_GFX_RENAMEITEM               50002
#define ID_GFX_REMOVEITEM               50003


//#define IDC_DRAGGING                    137
//#define IDC_HANDCUR                     138
//#define IDC_HANDCUR                     143
#define IDC_NODRAGGING                  139

extern HINSTANCE g_hInstance;
/////////////////////////////////////////////////////////////////////////////
// CGfxOutBarCtrl

IMPLEMENT_DYNCREATE(CGfxOutBarCtrl, CWnd)

CGfxOutBarCtrl::CGfxOutBarCtrl()
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//HINSTANCE hInstResourceClient = AfxGetResourceHandle();
	//AfxSetResourceHandle(g_hInstance);

	crBackGroundColor = RGB(232,226,210);//GetSysColor(COLOR_BACKGROUND);//RGB(0,128,128);//
	crBackGroundColor1 = RGB(232,226,210);
	crTextColor = GetSysColor(COLOR_WINDOW);
	
	
	//cr3dFace = GetSysColor(COLOR_3DFACE);
	cr3dFace = RGB(255,255,0);
	cr3dUnFace = GetSysColor(COLOR_3DFACE);

	crTextColor = RGB(255,255,255);
	crHilightBorder = RGB(255,255,255);//GetSysColor(COLOR_3DHILIGHT);

	crDkShadowBorder = RGB(186, 176, 141);//GetSysColor(COLOR_3DDKSHADOW); //선택시 란인색


	pBlackPen = new CPen(PS_SOLID, 1, RGB(143,155,178));

	iFolderHeight = 21;
	iGap = 0;

	dwFlags = fDragItems|fEditGroups|fEditItems|fRemoveGroups|fRemoveItems|fAddGroups|fAnimation|fSelHighlight;

	iSelFolder = 0;

	iLastFolderHighlighted = -1;
	iLastSelectedFolder = -1;
	
	pLargeImageList = NULL;
	pSmallImageList = NULL;

	iFirstItem = 0;

	xSmallIconLabelOffset = 4;
	yLargeIconLabelOffset = 3;

	ySmallIconSpacing = 10;
	yLargeIconSpacing = 8;

	xLeftMargin = 5;
	yTopMargin  = 5;

	bUpArrow = bDownArrow = false;

	hHandCursor = AfxGetApp()->LoadCursor(IDC_HANDCUR);

	rcUpArrow   = rcDownArrow = CRect(0,0,0,0);

	bUpPressed = bDownPressed = false;

	bLooping = false;

	iLastItemHighlighted = -1;
	bPressedHighlight = false;

	iLastDragItemDraw = -1;

	lAnimationTickCount = 5;

	hDragCursor = AfxGetApp()->LoadCursor(IDC_DRAGGING);
	hNoDragCursor = AfxGetApp()->LoadCursor(IDC_NODRAGGING);


	iLastSel = -1;
	iSelAnimCount = 0;

	iSelAnimTiming = 0;

	m_bGradient = FALSE;
	m_pFolderImageList = NULL;

	//AfxSetResourceHandle(hInstResourceClient);
}

CGfxOutBarCtrl::~CGfxOutBarCtrl()
{
}

BEGIN_MESSAGE_MAP(CGfxOutBarCtrl, CWnd)
	//{{AFX_MSG_MAP(CGfxOutBarCtrl)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_COMMAND(ID_GFX_LARGEICON, OnGfxLargeicon)
	ON_UPDATE_COMMAND_UI(ID_GFX_LARGEICON, OnUpdateGfxLargeicon)
	ON_COMMAND(ID_GFX_SMALLICON, OnGfxSmallicon)
	ON_UPDATE_COMMAND_UI(ID_GFX_SMALLICON, OnUpdateGfxSmallicon)
	ON_COMMAND(ID_GFX_REMOVEITEM, OnGfxRemoveitem)
	ON_UPDATE_COMMAND_UI(ID_GFX_REMOVEITEM, OnUpdateGfxRemoveitem)
	ON_UPDATE_COMMAND_UI(ID_GFX_RENAMEITEM, OnUpdateGfxRenameitem)
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGfxOutBarCtrl message handlers

void CGfxOutBarCtrl::SetFolderText(const int index, const char * text)
{
	ASSERT(index >= 0 && index < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(index);
	if (pbf->cName) 
	{
		delete [] pbf->cName;
		pbf->cName = NULL;
	}
	pbf->cName = new char [lstrlen(text)+1];
	lstrcpy(pbf->cName, text);
}

void CGfxOutBarCtrl::SetItemText(const int index, const char * text)
{
	ASSERT(iSelFolder >= 0 && iSelFolder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
	CBarItem * pi = (CBarItem *) pbf->arItems.GetAt(index);
	if (pi->cItem) 
	{
		delete [] pi->cItem;
		pi->cItem = NULL;
	}
	pi->cItem = new char [lstrlen(text)+1];
	lstrcpy(pi->cItem, text);
}

BOOL CGfxOutBarCtrl::Create(DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, const DWORD dwFlag)
{
	dwFlags = dwFlag;
	BOOL bRet = CWnd::CreateEx(NULL, NULL, NULL, dwStyle, rect, pParentWnd, nID);
//	CWnd::ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	//GFXOUTBARCTRL_CLASSNAME
	return bRet;
}

bool CGfxOutBarCtrl::IsSmallIconView() const
{
	return dwFlags&fSmallIcon;
}

void CGfxOutBarCtrl::SetSmallIconView(const bool bSet)
{
	iFirstItem = 0;

	if (bSet)
	{
		if (!IsSmallIconView()) 
		{
			dwFlags |= fSmallIcon;
			CRect rc;
			GetInsideRect(rc);
			InvalidateRect(rc, false);
		}
	}
	else
	{
		if (IsSmallIconView()) 
		{
			dwFlags &= ~fSmallIcon;
			CRect rc;
			GetInsideRect(rc);
			InvalidateRect(rc, false);
		}
	}
}

DWORD CGfxOutBarCtrl::GetFlag() const
{
	return dwFlags;
}

void CGfxOutBarCtrl::ModifyFlag(const DWORD & dwRemove, const DWORD & dwAdd, const UINT redraw)
{
	if (dwRemove)	dwFlags &= ~(dwRemove);
	if (dwAdd)		dwFlags |= dwAdd;

	if (GetSafeHwnd())
	{
		if (redraw != 0) SetWindowPos(0,0,0,0,0,SWP_NOZORDER|SWP_NOSIZE|SWP_NOMOVE|redraw);
		else Invalidate();
	}
}

int CGfxOutBarCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	LOGFONT lfont;
	::memset(&lfont, 0x00, sizeof(LOGFONT));
	lfont.lfWidth = 0; 
	lfont.lfEscapement = 0; 
	lfont.lfOrientation = 0; 
	lfont.lfWeight = FW_NORMAL; 
	lfont.lfItalic = 0; 
	lfont.lfUnderline = 0; 
	lfont.lfStrikeOut = 0; 
	lfont.lfCharSet = DEFAULT_CHARSET; 
	lfont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
	lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
	lfont.lfQuality = DEFAULT_QUALITY;
	lfont.lfPitchAndFamily = FF_ROMAN; 
	strcpy(lfont.lfFaceName, "굴림체");
	lfont.lfHeight = -12;
	
	if(!m_Font.CreateFontIndirect(&lfont))
	{
		TRACE("%s[%d] : Tool Button용 font 생성 실패!", __FILE__, __LINE__);
		return -1;
	}
	
	return 0;
}

void CGfxOutBarCtrl::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1)
	{
		CPoint pt(GetMessagePos());

		ScreenToClient(&pt);

		CRect rc;
		GetClientRect(&rc);
		if (!(rc.PtInRect(pt)))
		{
			HighlightFolder(-1);
			HighlightItem(-1);
			KillTimer(1);
		}
	}
	else if (nIDEvent == 3 && iLastSel >= 0)
	{
		iSelAnimCount ++;
		if (iSelAnimCount > 10) iSelAnimCount = -1;
		if (iSelAnimCount == 0)
			DrawAnimItem(-1, 1, iLastSel);
		if (iSelAnimCount == 1)
			DrawAnimItem(0, 0, iLastSel);
		if (iSelAnimCount == 2)
			DrawAnimItem(1, 1, iLastSel);
		if (iSelAnimCount == 3)
			DrawAnimItem(0, 0, iLastSel);
	}
	CWnd::OnTimer(nIDEvent);
}

void CGfxOutBarCtrl::DrawAnimItem(const int xoffset, const int yoffset, const int index)
{
	CImageList * ima = GetFolderImageList(iSelFolder, IsSmallIconView());

	CRect rc, irc;
	GetInsideRect(irc);
	GetItemRect(iSelFolder, iLastSel, rc);

	if (iLastSel >= iFirstItem && irc.bottom > rc.bottom && irc.top < rc.top)
	{
		ASSERT(iSelFolder >= 0 && iSelFolder < GetFolderCount());
		CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);

		ASSERT(index >= 0 && index < pbf->GetItemCount());
		CBarItem * pi = (CBarItem *) pbf->arItems.GetAt(index);

		ASSERT(pi && ima);

		CClientDC dc(this);

		if (IsSmallIconView())
		{
			if (ima)
			{
				IMAGEINFO ii;
				ima->GetImageInfo(pi->iImageIndex, &ii);
				CSize szImage = CRect(ii.rcImage).Size();
				CPoint pt;
				pt.x = rc.left + 2;
				pt.y = rc.top + (rc.Height() - szImage.cy) / 2;

				CRect rcBck(pt.x-1, pt.y-1, pt.x + szImage.cx+2, pt.y + szImage.cy+2);
				dc.FillSolidRect(rcBck, crBackGroundColor);

				pt.x += xoffset;
				pt.y += yoffset;

				ima->Draw(&dc, pi->iImageIndex, pt, ILD_NORMAL);

			}
		}
		else
		{
			if (ima)
			{
				IMAGEINFO ii;
				ima->GetImageInfo(pi->iImageIndex, &ii);
				CSize szImage = CRect(ii.rcImage).Size();
				CPoint pt;
				pt.x = rc.left + (rc.Width() - szImage.cx) / 2;
				pt.y = rc.top;

				CRect rcBck(pt.x-1, pt.y-1, pt.x + szImage.cx+2, pt.y + szImage.cy+2);
				dc.FillSolidRect(rcBck, crBackGroundColor);

				pt.x += xoffset;
				pt.y += yoffset;

				ima->Draw(&dc, pi->iImageIndex, pt, ILD_NORMAL);
			}
		}
	}
}

void CGfxOutBarCtrl::OnPaint() 
{
	CRect clientRc;
	GetClientRect(&clientRc);
	CPaintDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, clientRc.Width(), clientRc.Height());
	CBitmap * obmp = memDC.SelectObject(&bmp);

	CDC * pDC = &memDC;

	pDC->FillSolidRect(clientRc, RGB(255,255,255));
	pDC->Draw3dRect(clientRc, RGB(120,120,120), RGB(120,120,120));

	CRect rc;
	GetInsideRect(rc);
	pDC->FillSolidRect(rc, RGB(255,255,255));
	if (!GetFolderChild()) pDC->FillSolidRect(rc, crBackGroundColor);

	int max = arFolder.GetSize();

	CRect frc;
	for (int t = 0; t < max; t++)
	{
		GetFolderRect(t, frc);

		if( t == iSelFolder)
			DrawFolder(pDC, t, frc, TRUE);
		else
			DrawFolder(pDC, t, frc, false);

	}

	//GetFolderRect(iSelFolder, frc);
	//DrawFolder(pDC, iSelFolder, frc, true);

	// FolderCount가 0이상일때만 실행되게 한다. 안그럼 죽어요
	if (GetFolderCount() && !GetFolderChild())
	{
		int f=0,l=0;
		GetVisibleRange(iSelFolder, f,l);
		rcUpArrow.SetRect(0,0,GetSystemMetrics(SM_CXVSCROLL), GetSystemMetrics(SM_CXVSCROLL));
		rcDownArrow = rcUpArrow;
		rcUpArrow.OffsetRect(rc.right - 5 - GetSystemMetrics(SM_CXVSCROLL), rc.top +5);
		rcDownArrow.OffsetRect(rc.right - 5 - GetSystemMetrics(SM_CXVSCROLL), rc.bottom - 5 - GetSystemMetrics(SM_CXVSCROLL));

		if (f > 0)
		{
			if (bUpPressed) pDC->DrawFrameControl(rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP|DFCS_PUSHED);
			else pDC->DrawFrameControl(rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP);
			bUpArrow = true;
			pDC->ExcludeClipRect(rcUpArrow);

			rc.top += (iFolderHeight + iGap);
		}
		else 
			bUpArrow = false;

		if (l < GetItemCount() - 1)
		{
			if (bDownPressed) pDC->DrawFrameControl(rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN|DFCS_PUSHED);
			else pDC->DrawFrameControl(rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN);
			bDownArrow = true;
			pDC->ExcludeClipRect(rcDownArrow);

			rc.bottom -= (iFolderHeight + iGap);
		}
		else bDownArrow = false;

		PaintItems(pDC, iSelFolder, rc);
	}

	dc.BitBlt(clientRc.left, clientRc.top, clientRc.Width(), clientRc.Height(), &memDC, 0,0, SRCCOPY);

	memDC.SelectObject(obmp);

	if (iLastFolderHighlighted >= 0)
	{
		int i = iLastFolderHighlighted;
		iLastFolderHighlighted = -1;
		HighlightFolder(i);
	}
	if (iLastItemHighlighted >= 0)
	{
		int i = iLastItemHighlighted;
		iLastItemHighlighted = -1;
		HighlightItem(i, bPressedHighlight);
	}
}

BOOL CGfxOutBarCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return true;
}

bool CGfxOutBarCtrl::GetFolderRect(const int iIndex, CRect & rect) const
{
	int max = arFolder.GetSize();
	ASSERT(iIndex >= 0 && iIndex < max);

	if (iIndex >= 0 && iIndex < max)
	{
		CRect rc;
		GetClientRect(rc);
		rc.DeflateRect(4,4);
		if (iIndex > iSelFolder)
			rect.SetRect(rc.left, rc.bottom - ((max - iIndex)) * (iFolderHeight+iGap), rc.right, 
			rc.bottom - (max - iIndex - 1) * (iFolderHeight+iGap));
		else 
			rect.SetRect(rc.left, rc.top + iIndex * (iFolderHeight+iGap), rc.right, 
			rc.top + (1 + iIndex) * (iFolderHeight+iGap));
		return true;
	}
	return false;
}

void CGfxOutBarCtrl::GetItemRect(const int iFolder, const int iIndex, CRect & rect)
{
	CRect rc;
	GetInsideRect(rc);

	int top = rc.top;
	CSize sz(0,0);
	int y = 0;
	for (int t = 0; t < iIndex; t++)
	{
		sz = GetItemSize(iFolder, t, ircAll);
		top += sz.cy;

		if (IsSmallIconView()) 
		{
			top += ySmallIconSpacing; 
		}
		else 
		{
			top += yLargeIconSpacing;
		}
		if (t == iFirstItem - 1) y = top - rc.top;
	}
	sz = GetItemSize(iFolder, iIndex, ircAll);
	rect.SetRect(rc.left, top, rc.left + sz.cx, top + sz.cy);

	rect.top -= y;
	rect.bottom -= y;

	rect.left += xLeftMargin;
	rect.top  += yTopMargin;
	rect.bottom += yTopMargin;

	if (!IsSmallIconView()) 
	{
		rect.left = rc.left;
		rect.right = rc.right;
	}
}

void CGfxOutBarCtrl::DrawFolder(CDC * pDC, const int iIdx, CRect rect, const bool bSelected)
{
	//if (iIdx <= 0 ) return ;
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iIdx);

	if (!bSelected)
	{
		if(!m_pFolderImageList)
		{
			CPen pn1(PS_SOLID, 1, crBackGroundColor1);
			CPen * op = pDC->SelectObject(&pn1);
			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.right, rect.top);
			pDC->SelectObject(op);

			rect.top ++;
			pDC->FillSolidRect(rect, RGB(255,255,255));
			rect.left +=2;
			rect.top +=2;
			pDC->FillSolidRect(rect, cr3dUnFace);
			int obk = pDC->SetBkMode(TRANSPARENT);
//			CFont * of = pDC->SelectObject(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
			CFont * of = pDC->SelectObject(&m_Font);
			pDC->SetTextColor(CLR_FONT_DEFAULT);
			pDC->DrawText(CString(pbf->cName), rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			pDC->SetBkMode(obk);
			pDC->SelectObject(of);
		}
		else
		{

//			CPen pn1(PS_SOLID, 1, crBackGroundColor1);
//			CPen * op = pDC->SelectObject(&pn1);
//			pDC->MoveTo(rect.left, rect.top);
//			pDC->LineTo(rect.right, rect.top);
//			pDC->SelectObject(op);

			//rect.top ++;
//			pDC->FillSolidRect(rect, RGB(255,255,255));
			//rect.left +=2;
			//rect.top +=2;
//			pDC->FillSolidRect(rect, cr3dUnFace);
			//2006.09.25 by LYH 검은색 부분 삭제
//			CRect rcTemp, rcTemp2;
//			CRect rc;
//			GetClientRect(rc);
//			rcTemp.top = rect.top;
//			rcTemp.bottom = rc.bottom + 2;
//			rcTemp.left = rect.left;
//			rcTemp.right = rect.right;
//			//2006.12.07 정유진 수정 
//			//LeftBar 사이즈 변경시 이미지 잘림현상 수정
//			//pDC->FillSolidRect(rcTemp, RGB(239,242,247));
////@유진삭제 pDC->FillSolidRect(rcTemp, RGB(208,186,21));
//			pDC->FillSolidRect(rcTemp, RGB(173,173,173));
//			rcTemp.top = rect.top;// + 1 ;
//			pDC->FillSolidRect(rcTemp, RGB(255,255,255));
//			rcTemp2.top = rect.top + 1;// + 2 ;
//			rcTemp2.bottom = rc.bottom + 2;
//			rcTemp2.left = rect.left;
//			rcTemp2.right = rect.right;
//@유진삭제 pDC->FillSolidRect(rcTemp2, RGB(255,253,225));
			pDC->FillSolidRect(rect, crBackGroundColor1);
			CRect rectOutLine(rect);
			if(iIdx == arFolder.GetSize() - 1)
			{
				pDC->Draw3dRect(rect, cr3dFace, cr3dFace);
			}
			else
			{
				rectOutLine.bottom += 1;
				pDC->Draw3dRect(rectOutLine, cr3dFace, cr3dFace);
			}
			//2006.12.07 정유진 수정end
			//end
			
			int obk = pDC->SetBkMode(TRANSPARENT);

//			CPen pn1(PS_SOLID, 1, RGB(173,173,173));
//			CPen* pPnOld = pDC->SelectObject(&pn1);
//			pDC->MoveTo(rect.left, rect.top + iFolderHeight - 1);
//			pDC->LineTo(rect.right, rect.top + iFolderHeight - 1);
//			pDC->SelectObject(pPnOld);

			//2006.12.07 정유진 수정 
			//LeftBar 사이즈 변경시 이미지 잘림현상 수정
			//m_pFolderImageList->Draw(pDC, m_nNormal, CPoint(rect.left, rect.top), ILD_TRANSPARENT);
			m_pFolderImageList->Draw(pDC, m_nNormal, CPoint(rect.left, rect.top), ILD_TRANSPARENT);		
			//2006.12.07 정유진 수정end
//			CFont * of = pDC->SelectObject(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
			CFont * of = pDC->SelectObject(&m_Font);
			pDC->SetTextColor(CLR_FONT_DEFAULT);
			rect.left += 25;
			pDC->DrawText(CString(pbf->cName), rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			pDC->SetBkMode(obk);
			pDC->SelectObject(of);
		}
	}
	else
	{
		if(m_pFolderImageList)
		{
			//2006.09.25 by LYH 검은색 부분 삭제
			CRect rcTemp, rcTemp2;
			CRect rc;
			GetClientRect(rc);
			rc.DeflateRect(4,4);
			rcTemp.top = rect.top;
			rcTemp.bottom = rc.bottom - (arFolder.GetSize() - iSelFolder - 1)*(iFolderHeight+iGap);
			rcTemp.left = rect.left;
			rcTemp.right = rect.right;
			//2006.12.07 정유진 수정 
			//LeftBar 사이즈 변경시 이미지 잘림현상 수정
			//pDC->FillSolidRect(rcTemp, RGB(239,242,247));
//@유진삭제	pDC->FillSolidRect(rcTemp, RGB(208,186,21));
			pDC->FillSolidRect(rcTemp, RGB(173,173,173));
			rcTemp.top = rect.top;// + 1 ;
			pDC->FillSolidRect(rcTemp, RGB(238,239,245));
			rcTemp2.top = rect.top;// + 2 ;
			rcTemp2.bottom = rc.bottom - (arFolder.GetSize() - iSelFolder - 1)*(iFolderHeight+iGap);
			rcTemp2.left = rect.left;
			rcTemp2.right = rect.right;
//@유진삭제 pDC->FillSolidRect(rcTemp2, RGB(255,253,225));
			pDC->FillSolidRect(rcTemp2, RGB(255,246,181));//KHD :선택색 RGB(255,0,0));//crBackGroundColor1);
			pDC->Draw3dRect(rcTemp2, cr3dFace, cr3dFace);
			//2006.12.07 정유진 수정end
			//end

			//rect.top ++;
			int obk = pDC->SetBkMode(TRANSPARENT);
			//2006.12.07 정유진 수정 
			//LeftBar 사이즈 변경시 이미지 잘림현상 수정
			//m_pFolderImageList->Draw(pDC, m_nDown, CPoint(rect.left, rect.top), ILD_TRANSPARENT);
			m_pFolderImageList->Draw(pDC, m_nDown, CPoint(rect.left + 2, rect.top), ILD_TRANSPARENT);
			//2006.12.07 정유진 수정end

//@유진삭제 CPen pn1(PS_SOLID, 1, RGB(208,186,24));
			CPen pn1(PS_SOLID, 1, RGB(173,173,173));
			CPen* pPnOld = pDC->SelectObject(&pn1);
			pDC->MoveTo(rect.left, rect.top + iFolderHeight - 1);
			pDC->LineTo(rect.right, rect.top + iFolderHeight - 1);
			pDC->SelectObject(pPnOld);


//			CFont * of = pDC->SelectObject(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
			CFont * of = pDC->SelectObject(&m_Font);
			//2006.12.11 정유진 수정
			//Leftbar의 font 일반체 -> bold체로 수정
			CFont boldfont;
			LOGFONT lfont;

			memset(&lfont, 0X00,sizeof(LOGFONT));
			of->GetLogFont(&lfont);
			lfont.lfWidth = 0; 
			lfont.lfEscapement = 0; 
			lfont.lfOrientation = 0; 
			lfont.lfWeight = FW_NORMAL; 
			lfont.lfItalic = 0; 
			lfont.lfUnderline = 0; 
			lfont.lfStrikeOut = 0; 
			lfont.lfCharSet = DEFAULT_CHARSET; 
			lfont.lfOutPrecision = OUT_DEFAULT_PRECIS; 
			lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS; 
			lfont.lfQuality = DEFAULT_QUALITY;
			lfont.lfPitchAndFamily = FF_ROMAN; 
			strcpy(lfont.lfFaceName, "굴림체");
			lfont.lfHeight = -12;
			lfont.lfWeight = FW_BOLD;

			boldfont.CreateFontIndirect(&lfont);
			of = pDC->SelectObject(&boldfont);
			//2006.12.11 정유진 수정 END
			pDC->SetTextColor(CLR_FONT_DEFAULT);
			rect.left += 25;
			pDC->DrawText(CString(pbf->cName), rect, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			pDC->SetBkMode(obk);
			pDC->SelectObject(of);
	
		}
		else
		{
			CPen pn1(PS_SOLID, 1, crBackGroundColor1);
			CPen * op = pDC->SelectObject(&pn1);
			pDC->MoveTo(rect.left, rect.top);
			pDC->LineTo(rect.right, rect.top);
			pDC->SelectObject(op);

			//2006.09.25 by LYH 검은색 부분 삭제
			CRect rcTemp;
			CRect rc;
			GetClientRect(rc);
			rc.DeflateRect(4,4);
			rcTemp.top = (iFolderHeight+iGap) * (iSelFolder + 1);
			rcTemp.bottom = rc.bottom - (arFolder.GetSize() - iSelFolder - 1)*(iFolderHeight+iGap);
			rcTemp.left = rect.left;
			rcTemp.right = rect.right;
			pDC->FillSolidRect(rcTemp, RGB(239,242,247));
			//end

			rect.top ++;
			pDC->FillSolidRect(rect, RGB(255,255,255));
			pDC->Draw3dRect(rect, crHilightBorder, crBackGroundColor1);
			rect.InflateRect(-1,-1);
			pDC->FillSolidRect(rect, cr3dFace);
			int obk = pDC->SetBkMode(TRANSPARENT);
//			CFont * of = pDC->SelectObject(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
			CFont * of = pDC->SelectObject(&m_Font);
			pDC->SetTextColor(CLR_FONT_DEFAULT);
			pDC->DrawText(CString(pbf->cName), rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			pDC->SetBkMode(obk);
			pDC->SelectObject(of);

		}
	}
//* 수정 끝 (2003.4.10 이남경) 

}

int CGfxOutBarCtrl::AddFolder(const char * cFolderName, const DWORD exData)
{
	CBarFolder * pbf = new CBarFolder(cFolderName, exData);
	ASSERT(pbf);
	arFolder.Add((void *)pbf);

	return arFolder.GetSize() - 1;
}

CGfxOutBarCtrl::CBarFolder::CBarFolder(const char * name, DWORD exData)
{
	cName = NULL;
	dwData = exData;

	if (name)
	{
		cName = new char[lstrlen(name)+1];
		ASSERT(cName);
		lstrcpy(cName, name);
	}
	pLargeImageList = NULL;
	pSmallImageList = NULL;
	pChild = NULL;
}

CGfxOutBarCtrl::CBarFolder::~CBarFolder()
{
	if (cName) delete [] cName;
	for (int t = 0; t < arItems.GetSize(); t++)
		if (arItems.GetAt(t)) delete (CBarFolder*) arItems.GetAt(t);

	arItems.RemoveAll();
}

void CGfxOutBarCtrl::CBarFolder::CleanUp()
{
	if (cName) delete [] cName;
	for (int t = 0; t < arItems.GetSize(); t++)
		if (arItems.GetAt(t)) delete (CBarFolder*) arItems.GetAt(t);

	arItems.RemoveAll();
}
void CGfxOutBarCtrl::GetInsideRect(CRect & rect) const
{
	GetClientRect(rect);
	rect.DeflateRect(4,4);
	if (arFolder.GetSize() > 0)
	{
		int max = arFolder.GetSize();
		rect.top += iFolderHeight * (iSelFolder + 1);
		if(iSelFolder == arFolder.GetSize() - 1)
			rect.bottom -= (max - iSelFolder - 1)*iFolderHeight + 1;
		else
			rect.bottom -= (max - iSelFolder - 1)*iFolderHeight;
		rect.left += 1;//+ 2;
		rect.right -= 1;

		//rect.top += (iFolderHeight+iGap) * (iSelFolder + 1);
		//rect.bottom -= (max - iSelFolder - 1)*(iFolderHeight+iGap) + 2;
		return;
	}
}

void CGfxOutBarCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
//	int index, ht = HitTestEx(point, index);
//
//	if (ht != htFolder && iLastFolderHighlighted >= 0) HighlightFolder(-1);
//	if (ht != htItem   && iLastItemHighlighted   >= 0) HighlightItem(-1);
//
//	if (ht == htFolder)
//	{
//		HighlightFolder(index);
//		SetTimer(1,100,NULL);
//	}
//	else if (ht == htItem)
//	{
//		HighlightItem(index);
//		SetTimer(1,100,NULL);
//	}

	CWnd::OnMouseMove(nFlags, point);
}

int CGfxOutBarCtrl::HitTestEx(const CPoint & point, int &index)
{
	if (bUpArrow && rcUpArrow.PtInRect(point)) return htUpScroll;
	if (bDownArrow && rcDownArrow.PtInRect(point)) return htDownScroll;

	int max = arFolder.GetSize(), t;
	//max가 1보다 작으면 안됨.
	if (max <1)	return -1;

	CRect rc;
	for (t = 0; t < max; t++)
	{
		GetFolderRect(t, rc);
		if (rc.PtInRect(point)) 
		{
			index = t;
			return htFolder;
		}
	}
	
	GetInsideRect(rc);	

	int f=0,l=0;
	GetVisibleRange(iSelFolder, f,l);

	if (f > 0)
		rc.top += (iFolderHeight+iGap);
	
	if (l < GetItemCount() - 1)
		rc.bottom -= (iFolderHeight+iGap);

	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
	max = pbf->GetItemCount();
	for (t = iFirstItem; t < max; t++)
	{
		CBarItem * pi = (CBarItem *) pbf->arItems.GetAt(t);
		CRect itemRc;
		if (!IsSmallIconView()) 
		{
			GetIconRect(iSelFolder, t, itemRc);
			if (itemRc.PtInRect(point))
			{
				index = t;
				return htItem;
			}
			else if (itemRc.top > rc.bottom) break;
			GetLabelRect(iSelFolder, t, itemRc);
			itemRc.top -= yLargeIconLabelOffset;

			if (itemRc.PtInRect(point))
			{
				index = t;
				return htItem;
			}
			else if (itemRc.top > rc.bottom) break;
		}
		else
		{
			GetItemRect(iSelFolder, t, itemRc);
			if (itemRc.PtInRect(point) && rc.PtInRect(point))
			{
				index = t;
				return htItem;
			}
			else if (itemRc.top > rc.bottom) break;
		}
	}
	return htNothing;
}

void CGfxOutBarCtrl::HighlightFolder(const int index)
{
	CWnd * pf = GetFocus();
	if (pf != NULL && pf != this && IsChild(pf)) return;

	if (iLastFolderHighlighted == index) return;

	if (iLastFolderHighlighted >= 0)
	{
		CRect rc;
		if (GetFolderRect(iLastFolderHighlighted, rc))
		{
			CClientDC dc(this);
			if(iLastFolderHighlighted == iSelFolder)
			{
				CPen pn(PS_SOLID, 1, crBackGroundColor1);
				CPen * op = dc.SelectObject(&pn);
				dc.MoveTo(rc.left, rc.bottom-1);
				dc.LineTo(rc.right-1, rc.bottom-1);
				dc.LineTo(rc.right-1, rc.top);
				dc.SelectObject(op);
			}
			else
			{
				CPen pn(PS_SOLID, 1, cr3dUnFace);
				CPen * op = dc.SelectObject(&pn);
				dc.MoveTo(rc.left, rc.bottom-1);
				dc.LineTo(rc.right-1, rc.bottom-1);
				dc.LineTo(rc.right-1, rc.top);
				dc.SelectObject(op);
			}
			//CPen pn(PS_SOLID, 1, crBackGroundColor1);
			//CPen * op = dc.SelectObject(&pn);
			//dc.MoveTo(rc.left, rc.bottom-1);
			//dc.LineTo(rc.right-1, rc.bottom-1);
			//dc.LineTo(rc.right-1, rc.top);
			//CPen pn1(PS_SOLID, 1, cr3dFace);
			//dc.SelectObject(&pn1);
			//dc.MoveTo(rc.left+1, rc.bottom-2);
			//dc.LineTo(rc.right-2, rc.bottom-2);
			//dc.LineTo(rc.right-2, rc.top+1);
			//dc.SelectObject(op);
		}
	}
	iLastFolderHighlighted = index;
	if (iLastFolderHighlighted >= 0)
	{
		CRect rc;
		if (GetFolderRect(iLastFolderHighlighted, rc))
		{
			CClientDC dc(this);
			CPen pn(PS_SOLID, 1, crDkShadowBorder);
			CPen * op = dc.SelectObject(&pn);
			dc.MoveTo(rc.left, rc.bottom-1);
			dc.LineTo(rc.right-1, rc.bottom-1);
			dc.LineTo(rc.right-1, rc.top);
			//CPen pn1(PS_SOLID, 1, crBackGroundColor1);
			//dc.SelectObject(&pn1);
			//dc.MoveTo(rc.left+1, rc.bottom-2);
			//dc.LineTo(rc.right-2, rc.bottom-2);
			//dc.LineTo(rc.right-2, rc.top+1);
			dc.SelectObject(op);
		}
	}
}

void CGfxOutBarCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (GetFocus() != this) SetFocus();

	int index, ht = HitTestEx(point, index);
	iLastDragItemDrawType = -1;

	CRect inRc;
	GetInsideRect(inRc);

	if (ht == htFolder)
	{
		bool bHigh = true;
		CRect rc;
		GetFolderRect(index, rc);

		if (::GetCapture() == NULL)
		{
			SetCapture();
			ASSERT(this == GetCapture());
			CClientDC dc(this);
			//DrawFolder(&dc, index, rc, true);
			AfxLockTempMaps();
			for (;;)
			{
				MSG msg;
				VERIFY(::GetMessage(&msg, NULL, 0, 0));

				if (CWnd::GetCapture() != this) break;

				switch (msg.message)
				{
				//case WM_MOUSEMOVE:
				//	{
				//		CPoint pt(msg.lParam);
				//		int idx, ht1 = HitTestEx(pt, idx);
				//		if (ht1 == htFolder && idx == index)
				//		{
				//			if (!bHigh)
				//			{
				//				DrawFolder(&dc, index, rc, true);
				//				bHigh = true;
				//			}
				//		}
				//		else
				//		{
				//			if (bHigh)
				//			{
				//				DrawFolder(&dc, index, rc, false);
				//				bHigh = false;
				//			}
				//		}
				//	}
				//	break;

				case WM_LBUTTONUP:
					{
						//if (bHigh)
						//{
						//	DrawFolder(&dc, index, rc, true);
						//	bHigh = false;
						//}
						CPoint pt(msg.lParam);
						int idx, ht1 = HitTestEx(pt, idx);
						if (ht1 == htFolder)// && idx != iSelFolder)
							SetSelFolder(idx);
					}

					goto ExitLoop2;

				case WM_KEYDOWN:	
					if (msg.wParam != VK_ESCAPE) 
						break;

				default:
					DispatchMessage(&msg);
					break;
				}
			}

		ExitLoop2:
			ReleaseCapture();
			AfxUnlockTempMaps(FALSE);
		}
		if (bHigh) InvalidateRect(rc, false);
	}
	else iLastSelectedFolder = -1;

	if (ht == htItem)
	{
		iLastDragItemDraw = -1;
		bool bHigh = true, bDragging = false;
		CRect rc;
		GetItemRect(iSelFolder, index, rc);

		HCURSOR hCur = GetCursor();

		if (::GetCapture() == NULL)
		{
			SetCapture();
			ASSERT(this == GetCapture());
			CClientDC dc(this);
			HighlightItem(index, true);
			AfxLockTempMaps();
			for (;;)
			{
				MSG msg;
				VERIFY(::GetMessage(&msg, NULL, 0, 0));

				if (CWnd::GetCapture() != this) break;

				switch (msg.message)
				{
				//case WM_MOUSEMOVE:
				//	{
				//		CPoint pt(msg.lParam);
				//		int idx, ht1 = HitTestEx(pt, idx);
				//		if (bDragging)
				//		{
				//			if (ht1 == htItem) 
				//			{
				//				DrawDragArrow(&dc, index, idx);
				//				SetCursor(hDragCursor);
				//				hCur = hDragCursor;
				//			}
				//			else 
				//			{
				//				CRect rcItem;
				//				GetItemRect(iSelFolder, GetItemCount() - 1, rcItem);
				//				if (pt.y > rcItem.bottom && pt.y < inRc.bottom)
				//				{
				//					DrawDragArrow(&dc, index, GetItemCount());
				//					SetCursor(hDragCursor);
				//					hCur = hDragCursor;
				//				}
				//				else
				//				{
				//					DrawDragArrow(&dc, index, -1);
				//					SetCursor(hNoDragCursor);
				//					hCur = hNoDragCursor;
				//				}
				//			}
				//		}
				//		else
				//		{
				//			if (ht1 == htItem && idx == index)
				//			{
				//				if (!bHigh)
				//				{
				//					HighlightItem(index, true);
				//					bHigh = true;
				//					bPressedHighlight = true;
				//				}
				//			}
				//			else 
				//			{
				//				if (ht1 == htItem)
				//				{
				//					if (bHigh)
				//					{
				//						HighlightItem(index, false);
				//						bHigh = false;
				//						bPressedHighlight = false;
				//					}
				//				}
				//				else
				//				{
				//					if (dwFlags&fDragItems)
				//					{
				//						HighlightItem(index, true);
				//						bHigh = true;
				//						bDragging = true;

				//						SetCursor(hDragCursor);
				//						hCur = hDragCursor;

				//						bPressedHighlight = true;
				//					}
				//				}
				//			}
				//		}
				//	}
				//	break;

				case WM_SETCURSOR:
					SetCursor(hCur);
					break;

				case WM_LBUTTONUP:
					{
						if (bHigh)
						{
							HighlightItem(-1);
							bHigh = false;
						}
						CPoint pt(msg.lParam);
						int idx, ht1 = HitTestEx(pt, idx);
						if (!bDragging)
						{
							if (ht1 == htItem && idx == index) 
							{
								if (iSelAnimTiming > 0 && index != iLastSel && iLastSel >= 0)
								{
									DrawAnimItem(0, 0, iLastSel);
								}

								if (dwFlags&fSelHighlight && iLastSel >= 0)
								{
									CRect rc;
									GetIconRect(iSelFolder, iLastSel, rc);
									rc.InflateRect(1,1);
									InvalidateRect(rc);
								}

								iLastSel = index;

								if (dwFlags&fSelHighlight && iLastSel >= 0)
								{
									CRect rc;
									GetIconRect(iSelFolder, iLastSel, rc);
									rc.InflateRect(1,1);
									InvalidateRect(rc);
								}

								GetOwner()->SendMessage(WMU_OUTBAR_NOTIFY, NM_OB_ITEMCLICK, idx);
								Invalidate();
							}
						}
						else
						{
							if (ht1 == htItem)
							{
								if (idx != index)
								{
									OUTBAR_INFO ob;
									ob.iDragFrom = iSelFolder;
									ob.iDragTo = index;
									if (GetOwner()->SendMessage(WMU_OUTBAR_NOTIFY, NM_OB_DRAGITEM, (LPARAM) &ob))
									{
										CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
										CBarItem * piFrom = (CBarItem *) pbf->arItems.GetAt(index);
										pbf->arItems.RemoveAt(index);
										if (idx > index) idx --;
										pbf->arItems.InsertAt(idx, piFrom);
									}
								}
							}
							else
							{
								CRect rcItem;
								GetItemRect(iSelFolder, GetItemCount() - 1, rcItem);
								if (pt.y > rcItem.bottom && pt.y < inRc.bottom)
								{
									OUTBAR_INFO ob;
									ob.iDragFrom = iSelFolder;
									ob.iDragTo = index;
									if (GetOwner()->SendMessage(WMU_OUTBAR_NOTIFY, NM_OB_DRAGITEM, (LPARAM) &ob))
									{
										CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
										CBarItem * piFrom = (CBarItem *) pbf->arItems.GetAt(index);
										pbf->arItems.RemoveAt(index);
										pbf->arItems.Add((void *) piFrom);
									}
								}
							}
						}
					}

					goto ExitLoop4;

				default:
					DispatchMessage(&msg);
					break;
				}
			}

		ExitLoop4:
			ReleaseCapture();
			AfxUnlockTempMaps(FALSE);
			if (bDragging) 
			{
				Invalidate();
			}
		}
		bPressedHighlight = false;
		if (bHigh) InvalidateRect(rc, false);
	}
	else iLastItemHighlighted = -1;

	if (ht == htDownScroll)
	{
		bLooping = true;
		bool bHigh = true;

		if (::GetCapture() == NULL)
		{
			SetCapture();
			ASSERT(this == GetCapture());
			CClientDC dc(this);
			dc.DrawFrameControl(rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN|DFCS_PUSHED);
			SetTimer(2,300,NULL);
			AfxLockTempMaps();
			for (;;)
			{
				MSG msg;
				VERIFY(::GetMessage(&msg, NULL, 0, 0));

				if (CWnd::GetCapture() != this) break;

				switch (msg.message)
				{
				//case WM_MOUSEMOVE:
				//	{
				//		CPoint pt(msg.lParam);
				//		if (rcDownArrow.PtInRect(pt))
				//		{
				//			if (bHigh == false)
				//			{
				//				dc.DrawFrameControl(rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN|DFCS_PUSHED);
				//				bHigh = true;
				//				bDownArrow = true;
				//				bDownPressed = true;
				//			}
				//		}
				//		else
				//		{
				//			if (bHigh == true)
				//			{
				//				dc.DrawFrameControl(rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN);
				//				bHigh = false;
				//				bDownArrow = false;
				//				bDownPressed = false;
				//			}
				//		}
				//	}
				//	break;

				case WM_LBUTTONUP:
					{
						if (bHigh)
						{
							dc.DrawFrameControl(rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN);
							bHigh = false;
						}
						bDownArrow = false;
						CPoint pt(msg.lParam);
						if (rcDownArrow.PtInRect(pt))
						{
							CRect itrc;
							GetItemRect(iSelFolder, GetItemCount() - 1, itrc);
							CRect crc;
							GetInsideRect(crc);
							if (itrc.bottom > crc.bottom)
							{
								iFirstItem ++;
								InvalidateRect(crc, true);
							}
						}
					}
					goto ExitLoop3;


				case WM_TIMER:
					{
						if (msg.wParam == 2)
						{
							if (bHigh)
							{
								CPoint pt(msg.pt);
								ScreenToClient(&pt);
								if (rcDownArrow.PtInRect(pt))
								{
									bDownPressed = true;
									CRect itrc;
									GetItemRect(iSelFolder, GetItemCount() - 1, itrc);
									CRect crc;
									GetInsideRect(crc);
									if (itrc.bottom > crc.bottom)
									{
										iFirstItem ++;
										InvalidateRect(crc, true);
									}
									else goto ExitLoop3;
								}
								else bDownPressed = false;
							}
						}
						break;
					}

				case WM_KEYDOWN:	
					if (msg.wParam != VK_ESCAPE) 
						break;

				default:
					DispatchMessage(&msg);
					break;
				}
			}

		ExitLoop3:
			KillTimer(2);
			ReleaseCapture();
			AfxUnlockTempMaps(FALSE);
			bLooping = false;
			bDownPressed = false;
			bDownArrow = false;
			CRect crc;
			GetInsideRect(crc);
			InvalidateRect(crc, true);
		}
	}

	if (ht == htUpScroll)
	{
		bLooping = true;
		bool bHigh = true;

		if (::GetCapture() == NULL)
		{
			SetCapture();
			ASSERT(this == GetCapture());
			CClientDC dc(this);
			dc.DrawFrameControl(rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP|DFCS_PUSHED);
			SetTimer(2,300,NULL);
			AfxLockTempMaps();
			for (;;)
			{
				MSG msg;
				VERIFY(::GetMessage(&msg, NULL, 0, 0));

				if (CWnd::GetCapture() != this) break;

				switch (msg.message)
				{
				//case WM_MOUSEMOVE:
				//	{
				//		CPoint pt(msg.lParam);
				//		if (rcUpArrow.PtInRect(pt))
				//		{
				//			if (bHigh == false)
				//			{
				//				dc.DrawFrameControl(rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP|DFCS_PUSHED);
				//				bHigh = true;
				//				bUpArrow = true;
				//				bUpPressed = true;
				//			}
				//		}
				//		else
				//		{
				//			if (bHigh == true)
				//			{
				//				dc.DrawFrameControl(rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP);
				//				bHigh = false;
				//				bUpArrow = false;
				//				bUpPressed = false;
				//			}
				//		}
				//	}
				//	break;

				case WM_LBUTTONUP:
					{
						if (bHigh)
						{
							dc.DrawFrameControl(rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP);
							bHigh = false;
						}
						bUpArrow = false;
						CPoint pt(msg.lParam);
						if (rcUpArrow.PtInRect(pt))
						{
							if (iFirstItem > 0 ) 
							{
								iFirstItem --;
								CRect crc;
								GetInsideRect(crc);
								InvalidateRect(crc, true);
							}
						}
					}

					goto ExitLoop;

				case WM_TIMER:
					{
						if (msg.wParam == 2)
						{
							if (bHigh)
							{
								CPoint pt(msg.pt);
								ScreenToClient(&pt);
								if (rcUpArrow.PtInRect(pt))
								{
									bUpPressed = true;
									if (iFirstItem > 0) 
									{
										iFirstItem --;
										CRect crc;
										GetInsideRect(crc);
										InvalidateRect(crc, true);
									}
									else goto ExitLoop;
								}
								else bUpPressed = false;
							}
						}
						break;
					}
				case WM_KEYDOWN:	
					if (msg.wParam != VK_ESCAPE) break;

				default:
					DispatchMessage(&msg);
					break;
				}
			}

		ExitLoop:
			KillTimer(2);
			ReleaseCapture();
			AfxUnlockTempMaps(FALSE);
			bLooping = false;
			bUpPressed = false;
			bUpArrow = false;
			CRect crc;
			GetInsideRect(crc);
			InvalidateRect(crc, true);
		}
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}


int CGfxOutBarCtrl::InsertItem(const int folder, const int index, const char * text, const int image, const DWORD exData)
{
	ASSERT(folder >= 0 && folder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(folder);
	return pbf->InsertItem(index, text, image, exData);
}

int CGfxOutBarCtrl::GetItemCount() const
{
	ASSERT(iSelFolder >= 0 && iSelFolder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
	return 	pbf->GetItemCount();
}

int CGfxOutBarCtrl::SetSelFolder(LPCSTR szFolder)
{
	int nCnt = GetFolderCount();
	int i=0;
	CString szText;
	for(; i<nCnt; i++)
	{
		szText = GetFolderText(i);
		if(szText.CompareNoCase(szFolder)==0)
		{
			SetSelFolder(i);
			return i;
		}
	}
	return -1;
}

void CGfxOutBarCtrl::SetSelFolder(const int index)
{
	ASSERT(index >= 0 && index < GetFolderCount());
//	if (index == iSelFolder) return;

	CWnd * pc = GetFolderChild();
	if (pc) pc->ShowWindow(SW_HIDE);

	if (index != iSelFolder)
	{
		if (dwFlags&fAnimation && lAnimationTickCount >= 0) AnimateFolderScroll(iSelFolder, index);
	}

	iSelFolder = index;
	iFirstItem = 0;
	iLastSel = -1;

	pc = GetFolderChild();
	if (pc)
	{
		CRect rc;
		GetInsideRect(rc);

		// bds 트리 콘트롤 위치 수정 (040305)
		//rc.left += 5;

		pc->MoveWindow(rc);
		pc->ShowWindow(SW_SHOW);
	}	
//	if(index!=3)//KHD : 업종일땐 폴더체인지 이벤트를 타지 않게 한다.
		GetParent()->SendMessage(WMU_OUTBAR_NOTIFY, NM_FOLDERCHANGE, (LPARAM) iSelFolder);

	Invalidate();
}
							
int CGfxOutBarCtrl::GetFolderCount() const
{
	return arFolder.GetSize();
}

int CGfxOutBarCtrl::GetSelFolder() const
{
	return iSelFolder;
}

void CGfxOutBarCtrl::RemoveFolder(const int index)
{
	ASSERT(index >= 0 && index < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(index);
	delete pbf;
	arFolder.RemoveAt(index);
	if (iSelFolder >= index) iSelFolder = index - 1;
	if (iSelFolder < 0 && GetFolderCount() > 0) iSelFolder = 0;
	Invalidate();
}

int CGfxOutBarCtrl::CBarFolder::InsertItem(int index, const char * text, const int image, const DWORD exData)
{
	if (index < 0 || index > GetItemCount()) index = GetItemCount();

	int c = GetItemCount();

	CBarItem * pbf = new CBarItem(text, image, exData);
	ASSERT(pbf);
	if (index < GetItemCount()) 
		arItems.InsertAt(index, (void *) pbf);
	else 
		arItems.Add((void *) pbf);

	c = GetItemCount();

	return index;
}

int CGfxOutBarCtrl::GetCountPerPage() const
{
	return 0;
}

CImageList * CGfxOutBarCtrl::SetImageList(CImageList * pImageList, int nImageList)
{
	CImageList * o = NULL;
	if (nImageList == fSmallIcon)
	{
		o = pSmallImageList;
		pSmallImageList = pImageList;
	}
	else if (nImageList == fLargeIcon)
	{
		o = pLargeImageList;
		pLargeImageList = pImageList;
	}
	return o;
}

CImageList * CGfxOutBarCtrl::GetImageList(CImageList * pImageList, int nImageList)
{
	if (nImageList == fSmallIcon) return pSmallImageList;
	else if (nImageList == fLargeIcon) return pLargeImageList;
	return NULL;
}

CImageList * CGfxOutBarCtrl::SetFolderImageList(const int folder, CImageList * pImageList, int nImageList)
{
	ASSERT(folder >= 0 && folder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(folder);
	CImageList * o = NULL;
	if (nImageList == fSmallIcon)
	{
		o = pbf->pSmallImageList;
		pbf->pSmallImageList = pImageList;
	}
	else if (nImageList == fLargeIcon)
	{
		o = pbf->pLargeImageList;
		pbf->pLargeImageList = pImageList;
	}
	return o;
}


CGfxOutBarCtrl::CBarItem::CBarItem(const char * name, const int image, DWORD exData)
{
	cItem = NULL;
	iImageIndex = image;
	dwData = exData;
	if (name)
	{
		cItem = new char[lstrlen(name)+1];
		ASSERT(cItem);
		lstrcpy(cItem, name);
	}
}

CGfxOutBarCtrl::CBarItem::~CBarItem()
{
	if (cItem) delete [] cItem;
}

int CGfxOutBarCtrl::CBarFolder::GetItemCount()
{
	return arItems.GetSize();
}

void CGfxOutBarCtrl::PaintItems(CDC * pDC, const int iFolder, CRect rc)
{
	ASSERT(iFolder >= 0 && iFolder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iFolder);
	int max = pbf->GetItemCount(), t;
//	CRect irc;
//	GetInsideRect(irc);
	int isdc = pDC->SaveDC();
	CRgn itemRegion;
	itemRegion.CreateRectRgnIndirect(&rc);
	pDC->SelectClipRgn(&itemRegion);
	itemRegion.DeleteObject();
	for (t = iFirstItem; t < max; t++)
	{
		CBarItem * pi = (CBarItem *) pbf->arItems.GetAt(t);
		CRect itemRc;
		GetItemRect(iFolder, t, itemRc);
		if (itemRc.top > rc.bottom) break;
		else DrawItem(pDC, iFolder, itemRc, t);
	}
	pDC->RestoreDC(isdc);
}


CSize CGfxOutBarCtrl::GetItemSize(const int iFolder, const int index, const int type)
{
	ASSERT(iFolder >= 0 && iFolder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iFolder);

	ASSERT(index >= 0 && index < pbf->GetItemCount());
	CBarItem * pi = (CBarItem *) pbf->arItems.GetAt(index);

	CSize szImage(0,0);
	CSize szLabel(0,0);
	CSize szAll(0,0);

	if (pi->iImageIndex >= 0)
	{
		if (type != ircLabel)
		{
			CImageList * il = GetFolderImageList(iFolder, IsSmallIconView());
			ASSERT(il);
			if (il)
			{
				IMAGEINFO ii;
				il->GetImageInfo(pi->iImageIndex, &ii);
				szImage = CRect(ii.rcImage).Size();
			}
		}
	}
	if (pi->cItem)
	{
		if (type != ircIcon)
		{
			CClientDC dc(this);
//			CFont * oft = (CFont *) dc.SelectObject(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
			CFont * oft = dc.SelectObject(&m_Font);
			if (IsSmallIconView())
			{
				szLabel = dc.GetTextExtent(pi->cItem, lstrlen(pi->cItem));
			}
			else
			{
				CRect rc;
				GetInsideRect(rc);
				rc.bottom = rc.top;
				dc.DrawText(pi->cItem, lstrlen(pi->cItem), rc, DT_CALCRECT|DT_CENTER|DT_WORDBREAK);
				szLabel = rc.Size();
			}
			dc.SelectObject(oft);
		}
	}
	if (IsSmallIconView())
	{
		if (type == ircIcon) szAll = szImage;
		else if (type == ircLabel) szAll = szLabel;
		else if (type == ircAll) szAll = CSize(szImage.cx + szLabel.cx + xSmallIconLabelOffset, szImage.cy > szLabel.cy ? szImage.cy : szLabel.cy);
	}
	else
	{
		if (type == ircIcon) szAll = szImage;
		else if (type == ircLabel) szAll = szLabel;
		else if (type == ircAll) 
		{
			szAll = CSize(szImage.cx > szLabel.cx ? szImage.cx : szLabel.cx, szLabel.cy + szImage.cy + yLargeIconLabelOffset + yLargeIconSpacing);
		}
	}
	return szAll;
}

CImageList * CGfxOutBarCtrl::GetFolderImageList(const int index, const bool bSmall) const
{
	ASSERT(index >= 0 && index < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(index);
	if (bSmall)
	{
		if (pbf->pSmallImageList) return pbf->pSmallImageList; else return pSmallImageList;
	}
	if (pbf->pLargeImageList) return pbf->pLargeImageList; else return pLargeImageList;
}

void CGfxOutBarCtrl::DrawItem(CDC * pDC, const int iFolder, CRect rc, const int index, const bool bOnlyImage)
{
	CImageList * ima = GetFolderImageList(iFolder, IsSmallIconView());

	ASSERT(iFolder >= 0 && iFolder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iFolder);

	ASSERT(index >= 0 && index < pbf->GetItemCount());
	CBarItem * pi = (CBarItem *) pbf->arItems.GetAt(index);

	ASSERT(pi && ima);

	CFont * oft = NULL;
	COLORREF ocr;
	if(index == iLastSel && iFolder == iSelFolder)
	{
//		oft = (CFont *) pDC->SelectObject(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
		oft = (CFont *) pDC->SelectObject(&m_Font);
		ocr = pDC->SetTextColor(RGB(255, 0, 0));
	}
	else
	{
//		oft = (CFont *) pDC->SelectObject(CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
		oft = (CFont *) pDC->SelectObject(&m_Font);
		ocr = pDC->SetTextColor(crTextColor);
	}

	int obk = pDC->SetBkMode(TRANSPARENT);

	if (IsSmallIconView())
	{
		if (ima)
		{
			IMAGEINFO ii;
			ima->GetImageInfo(pi->iImageIndex, &ii);
			CSize szImage = CRect(ii.rcImage).Size();
			CPoint pt;
			pt.x = rc.left + 2;
			pt.y = rc.top + (rc.Height() - szImage.cy) / 2;
			ima->Draw(pDC, pi->iImageIndex, pt, ILD_NORMAL);

			if (!bOnlyImage)
			{
				rc.left += szImage.cx + xSmallIconLabelOffset;
				pDC->TextOut(rc.left, rc.top, CString(pi->cItem));
			}
			
		}
	}
	else
	{
		if (ima)
		{
			IMAGEINFO ii;
			ima->GetImageInfo(pi->iImageIndex, &ii);
			CSize szImage = CRect(ii.rcImage).Size();
			CPoint pt;
			pt.x = rc.left + (rc.Width() - szImage.cx) / 2;
			pt.y = rc.top;// + (rc.Height() - szImage.cy) / 2;
			ima->Draw(pDC, pi->iImageIndex, pt, ILD_NORMAL);
			if (!bOnlyImage)
			{
				rc.top += szImage.cy + yLargeIconLabelOffset;
				pDC->DrawText(pi->cItem, lstrlen(pi->cItem), rc, DT_CENTER|DT_WORDBREAK);
			}
		}
	}
	
	if (dwFlags&fSelHighlight && iLastSel == index && iLastSel >= 0)
	{
		CRect rc;
		GetIconRect(iSelFolder, iLastSel, rc);
		rc.InflateRect(1,1);
		pDC->Draw3dRect(rc, crDkShadowBorder, cr3dFace);
	}

	pDC->SetTextColor(ocr);
	pDC->SelectObject(oft);
	pDC->SetBkMode(obk);

}
	

BOOL CGfxOutBarCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);
	int index, ht = HitTestEx(pt, index);
	if (ht == htFolder)
	{
		SetCursor(hHandCursor);
		return true;
	}
		
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CGfxOutBarCtrl::GetVisibleRange(const int iFolder, int & first, int & last)
{
	first = iFirstItem;
	CRect rc;
	GetInsideRect(rc);

	if (arFolder.GetSize() <= iFolder)	return;
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iFolder);
	int max = pbf->GetItemCount(), t;
	for (t = iFirstItem; t < max; t++)
	{
		CBarItem * pi = (CBarItem *) pbf->arItems.GetAt(t);
		CRect itemRc;
		GetItemRect(iFolder, t, itemRc);
		if (itemRc.bottom > rc.bottom) 
		{
			last = t - 1;
			break;
		}
		else last = t;
	}
}

void CGfxOutBarCtrl::OnSize(UINT nType, int cx, int cy) 
{
	bUpArrow = bDownArrow = false;
	
	CWnd::OnSize(nType, cx, cy);
	CString szDebug;

	int t, max = GetFolderCount();
	if(max>0)
	{
		CRect rc(0,0, cx, cy);
		//GetInsideRect 호출하는 루틴대신에 그 루틴을 그대로 가져옴.
		//GetInsideRect(rc);
		{
			//GetClientRect(rect);
			rc.DeflateRect(4,4);
			if (arFolder.GetSize() > 0)
			{
				int max = arFolder.GetSize();
				rc.top += iFolderHeight * (iSelFolder + 1);
				{
					szDebug.Format("SelFolder=%d\n", iSelFolder);
					OutputDebugString(szDebug);
				}
				
				if(iSelFolder == arFolder.GetSize() - 1)
					rc.bottom -= (max - iSelFolder - 1)*iFolderHeight + 1;
				else
					rc.bottom -= (max - iSelFolder - 1)*iFolderHeight;

				{
					szDebug.Format("top=%d, bottom=%d\n", rc.top, rc.bottom);
					OutputDebugString(szDebug);
				}

				rc.left += 1;//+ 2;
				rc.right -= 1;    
			}
		}
		{
			szDebug.Format("width=%d, height=%d\n", rc.Width(), rc.Height());
			OutputDebugString(szDebug);
		}
		for (t = 0; t < max; t++)
		{
			CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(t);
			CWnd * pc = GetFolderChild(t);
			if (pc) 
			{
				//pc->SetWindowPos(0, rc.left, rc.top, rc.Width(), rc.Height(), SWP_NOZORDER);
				pc->SetWindowPos(0, 0, 0, rc.Width(), rc.Height(), SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
			}
		}
	}
} 

void CGfxOutBarCtrl::HighlightItem(const int index, const bool bPressed)
{
	CWnd * pf = GetFocus();
	if (pf != NULL && pf != this && IsChild(pf)) return;

	static bool bOldPressed = false;
	if (iLastItemHighlighted == index && bOldPressed == bPressed) return;

	bOldPressed = bPressed;

	CClientDC dc(this);

	CRect irc;
	GetInsideRect(irc);
	int isdc = dc.SaveDC();
	CRgn itemRegion;
	itemRegion.CreateRectRgnIndirect(&irc);
	dc.SelectClipRgn(&itemRegion);
	itemRegion.DeleteObject();

	COLORREF clrOldText = dc.SetTextColor(RGB(255, 0, 0));

	if (iLastItemHighlighted >= 0 && IsValidItem(iLastItemHighlighted))
	{
		CRect rc;
		GetIconRect(iSelFolder, iLastItemHighlighted, rc);
		rc.InflateRect(1,1);
		dc.Draw3dRect(rc, crBackGroundColor, crBackGroundColor);
	}
	if (iSelAnimTiming > 0 && index == iLastSel)
	{
		iLastItemHighlighted = -1;
		return;
	}
	if (dwFlags&fSelHighlight && iLastSel == index)
	{
		iLastItemHighlighted = -1;
		return;
	}

	iLastItemHighlighted = index;
	if (iLastItemHighlighted >= 0 && IsValidItem(iLastItemHighlighted))
	{
		CRect rc;
		GetIconRect(iSelFolder, iLastItemHighlighted, rc);
		rc.InflateRect(1,1);
		if (bPressed)
			dc.Draw3dRect(rc, crDkShadowBorder, cr3dFace);
		else
			dc.Draw3dRect(rc, cr3dFace, crDkShadowBorder);
	}
	dc.RestoreDC(isdc);
}

void CGfxOutBarCtrl::GetIconRect(const int iFolder, const int iIndex, CRect & rect)
{
	CRect rc;
	GetInsideRect(rc);
	int top = rc.top;
	CSize sz(0,0);
	int y = 0;
	int spacing = IsSmallIconView() ? ySmallIconSpacing : yLargeIconSpacing;

	for (int t = 0; t < iIndex; t++)
	{
		sz = GetItemSize(iFolder, t, ircAll);
		top += sz.cy;
		top += spacing;
		if (t == iFirstItem - 1) y = top - rc.top;
	}
	top += spacing;

	sz = GetItemSize(iFolder, iIndex, ircIcon);
	if (IsSmallIconView())
	{
		rect.SetRect(rc.left, top, rc.left + sz.cx, top + sz.cy);
		rect.left += xLeftMargin + 2;
		rect.right += xLeftMargin + 2;
		rect.top -= yTopMargin;
		rect.bottom -= yTopMargin;

		rect.top -= y;
		rect.bottom -= y;
	}
	else
	{
		rect.SetRect(rc.left + (rc.Width() - sz.cx)/2, top, rc.left + (rc.Width() - sz.cx)/2 + sz.cx, top + sz.cy);
		rect.top -= y + 3;
		rect.bottom -= y + 2;
	}
}

void CGfxOutBarCtrl::GetLabelRect(const int iFolder, const int iIndex, CRect & rect)
{
	CRect rc;
	GetInsideRect(rc);
	int top = rc.top;
	CSize sz(0,0);
	int y = 0;
	int spacing = IsSmallIconView() ? ySmallIconSpacing : yLargeIconSpacing;

	for (int t = 0; t < iIndex; t++)
	{
		sz = GetItemSize(iFolder, t, ircAll);
		top += sz.cy;
		top += spacing;
		if (t == iFirstItem - 1) y = top - rc.top;
	}
	top += spacing;

	sz = GetItemSize(iFolder, iIndex, ircAll);

	CSize szLabel = GetItemSize(iFolder, iIndex, ircLabel);

	if (IsSmallIconView())
	{
		rect.SetRect(rc.left, top, rc.left + sz.cx, top + sz.cy);
		rect.left += xLeftMargin + 2;
		rect.right += xLeftMargin + 2;
		rect.top  += yTopMargin - 5;
		rect.bottom += yTopMargin - 5;
	}
	else
	{
		rect.SetRect(rc.left + (rc.Width() - sz.cx)/2, top, rc.left + (rc.Width() - sz.cx)/2 + sz.cx, top + sz.cy);
		rect.top -= y + 3;
		rect.bottom -= y + 2;

		rect.bottom -=  yLargeIconSpacing;
		rect.top    = rect.bottom - szLabel.cy;
	}
}

void CGfxOutBarCtrl::OnGfxLargeicon() 
{
	SetSmallIconView(false);
	Invalidate();	
}

void CGfxOutBarCtrl::OnUpdateGfxLargeicon(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsSmallIconView());
}

void CGfxOutBarCtrl::OnGfxSmallicon() 
{
	SetSmallIconView(true);	
	Invalidate();	
}

void CGfxOutBarCtrl::OnUpdateGfxSmallicon(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!IsSmallIconView());
}

void CGfxOutBarCtrl::OnGfxRemoveitem() 
{
	if (iHitInternal1 == htFolder)
	{
		RemoveFolder(iHitInternal2);
	}
	else if (iHitInternal1 == htItem)
	{
		RemoveItem(iHitInternal2);
	}
}

void CGfxOutBarCtrl::OnUpdateGfxRemoveitem(CCmdUI* pCmdUI) 
{
		
}

void CGfxOutBarCtrl::OnUpdateGfxRenameitem(CCmdUI* pCmdUI) 
{
}

void CGfxOutBarCtrl::RemoveItem(const int index)
{
	ASSERT(iSelFolder >= 0 && iSelFolder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
	ASSERT(index >= 0 && index < pbf->GetItemCount());
	if (IsValidItem(index))
	{
		CBarItem * i = (CBarItem *) pbf->arItems.GetAt(index);
		delete i;

		pbf->arItems.RemoveAt(index);
		CRect rc;
		GetInsideRect(rc);
		InvalidateRect(rc);
	}
}

bool CGfxOutBarCtrl::IsValidItem(const int index) const
{
	ASSERT(iSelFolder >= 0 && iSelFolder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
	return (index >= 0 && index < pbf->GetItemCount());
}

DWORD CGfxOutBarCtrl::GetItemData(const int index) const
{
	ASSERT(iSelFolder >= 0 && iSelFolder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
	if (IsValidItem(index))
	{
		CBarItem * i = (CBarItem *) pbf->arItems.GetAt(index);
		return i->dwData;
	}
	return 0;
}

int CGfxOutBarCtrl::GetItemImage(const int index) const
{
	ASSERT(iSelFolder >= 0 && iSelFolder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
	if (IsValidItem(index))
	{
		CBarItem * i = (CBarItem *) pbf->arItems.GetAt(index);
		return i->iImageIndex;
	}
	return 0;
}

void CGfxOutBarCtrl::SetItemData(const int index, const DWORD dwData)
{
	ASSERT(iSelFolder >= 0 && iSelFolder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
	if (IsValidItem(index))
	{
		CBarItem * i = (CBarItem *) pbf->arItems.GetAt(index);
		i->dwData = dwData;
	}
}

void CGfxOutBarCtrl::SetItemImage(const int index, const int iImage)
{
	ASSERT(iSelFolder >= 0 && iSelFolder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
	if (IsValidItem(index))
	{
		CBarItem * i = (CBarItem *) pbf->arItems.GetAt(index);
		i->iImageIndex = iImage;
	}
}

void CGfxOutBarCtrl::DrawDragArrow(CDC * pDC, const int iFrom, const int iTo)
{
	if (iTo == iLastDragItemDraw) return;
	CRect rc1;
	if (iLastDragItemDraw >= 0)
	{
		GetDragItemRect(iLastDragItemDraw, rc1);
		if (iLastDragItemDrawType == 0) rc1.bottom = rc1.top + 5;
		else if (iLastDragItemDrawType == 1) { rc1.top -= 4; rc1.bottom = rc1.top + 9; }
		else if (iLastDragItemDrawType == 2) { rc1.top -= 4; rc1.bottom = rc1.top + 5; }
		InvalidateRect(rc1, true);
		UpdateWindow();
	}
	iLastDragItemDraw = iTo;

	if (iLastDragItemDraw >= 0)
	{
		iLastDragItemDrawType = GetDragItemRect(iLastDragItemDraw, rc1);

		CPen * op = pDC->SelectObject(pBlackPen);

		pDC->MoveTo(rc1.left, rc1.top);
		pDC->LineTo(rc1.right, rc1.top);

		if (iLastDragItemDrawType != 2)
		{
			pDC->MoveTo(rc1.left, rc1.top+1);
			pDC->LineTo(rc1.left + 7, rc1.top+1);
			pDC->MoveTo(rc1.left, rc1.top+2);
			pDC->LineTo(rc1.left + 5, rc1.top+2);
			pDC->MoveTo(rc1.left, rc1.top+3);
			pDC->LineTo(rc1.left + 3, rc1.top+3);
			pDC->MoveTo(rc1.left, rc1.top+4);
			pDC->LineTo(rc1.left + 1, rc1.top+4);

			pDC->MoveTo(rc1.right-1, rc1.top+1);
			pDC->LineTo(rc1.right - 7, rc1.top+1);
			pDC->MoveTo(rc1.right-1, rc1.top+2);
			pDC->LineTo(rc1.right - 5, rc1.top+2);
			pDC->MoveTo(rc1.right-1, rc1.top+3);
			pDC->LineTo(rc1.right - 3, rc1.top+3);
		}
		if (iLastDragItemDrawType != 0)
		{
			pDC->MoveTo(rc1.left, rc1.top-1);
			pDC->LineTo(rc1.left + 7, rc1.top-1);
			pDC->MoveTo(rc1.left, rc1.top-2);
			pDC->LineTo(rc1.left + 5, rc1.top-2);
			pDC->MoveTo(rc1.left, rc1.top-3);
			pDC->LineTo(rc1.left + 3, rc1.top-3);
			pDC->MoveTo(rc1.left, rc1.top-4);
			pDC->LineTo(rc1.left + 1, rc1.top-4);

			pDC->MoveTo(rc1.right-1, rc1.top-1);
			pDC->LineTo(rc1.right - 7, rc1.top-1);
			pDC->MoveTo(rc1.right-1, rc1.top-2);
			pDC->LineTo(rc1.right - 5, rc1.top-2);
			pDC->MoveTo(rc1.right-1, rc1.top-3);
			pDC->LineTo(rc1.right - 3, rc1.top-3);
			pDC->MoveTo(rc1.right-1, rc1.top-4);
			pDC->LineTo(rc1.right - 1, rc1.top-4);
		}

		pDC->SelectObject(op);
	}
}

int CGfxOutBarCtrl::GetDragItemRect(const int index, CRect & rect)
{
	CRect rc, crc;
	GetInsideRect(crc);
	crc.InflateRect(-2, 0);

	GetItemRect(iSelFolder, index < GetItemCount() ? index : index - 1, rc);
	int line = 0;
	if (index < GetItemCount())
	{
		line = rc.top - 4;
	}
	else
	{
		line = rc.bottom + 6;
	}
	int tpe;
	if (index == 0)
	{
		rect.SetRect(crc.left, line+2, crc.right, line + 7);
		tpe = 0;
	}
	else if (index < GetItemCount())
	{
		rect.SetRect(crc.left, line - 9, crc.right, line);
		tpe = 1;
		if (IsSmallIconView()) 
		{
			rect.top += 8;
			rect.bottom += 8;
		}
	}
	else
	{
		rect.SetRect(crc.left, line, crc.right, line + 5);
		tpe = 2;
	}
	return tpe;
}


void CGfxOutBarCtrl::AnimateFolderScroll(const int iFrom, const int iTo)
{
	ASSERT(iFrom >= 0 && iFrom < GetFolderCount());
	ASSERT(iTo >= 0 && iTo < GetFolderCount());

	CRect rc, rc1, frc, frc1;
	GetInsideRect(rc);
	rc1.SetRect(0,0,rc.Width(),rc.Height());
	GetFolderRect(iTo, frc);
	frc1 = frc;

	CClientDC dc(this);
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bmpFrom, bmpTo;
	bmpFrom.CreateCompatibleBitmap(&dc, rc1.Width(), rc1.Height() + frc.Height() * 2);
	bmpTo.CreateCompatibleBitmap(&dc, rc1.Width(), rc1.Height() + frc.Height() * 2);
	CDC * pDC = &memDC;

	CWnd * pWnd1 = GetFolderChild(iFrom);
	CWnd * pWnd2 = GetFolderChild(iTo);


	CBitmap * obmp = pDC->SelectObject(&bmpFrom);
	if (iTo > iFrom)
	{
		rc1.bottom += frc.Height() * 2;
		// bds 트리 콘트롤 위치 수정 (040305)
		//rc1.left += 5;
		pDC->FillSolidRect(rc1, crBackGroundColor);
		if (pWnd1) 
		{
			BOOL bPrev = pWnd1->ShowWindow(SW_SHOW);
			pWnd1->SendMessage(WM_ERASEBKGND, (WPARAM) pDC->GetSafeHdc(), (LPARAM) 0);
			pWnd1->SendMessage(WM_PAINT, (WPARAM) pDC->GetSafeHdc(), (LPARAM) 0);
			if (!bPrev) pWnd1->ShowWindow(SW_HIDE);
		}
		else PaintItems(pDC, iFrom, rc1);

		pDC->SelectObject(&bmpTo);
		frc.SetRect(0,0,frc.Width(),frc.Height());
		rc1.SetRect(0,frc.Height(),rc.Width(), rc.Height()+frc.Height() * 2);
		pDC->FillSolidRect(rc1, crBackGroundColor);

		if (pWnd2) 
		{
			CPoint ovpt = pDC->SetViewportOrg(0, frc.Height());
			BOOL bPrev = pWnd2->ShowWindow(SW_SHOW);
			pWnd2->SendMessage(WM_ERASEBKGND, (WPARAM) pDC->GetSafeHdc(), (LPARAM) 0);
			pWnd2->SendMessage(WM_PAINT, (WPARAM) pDC->GetSafeHdc(), (LPARAM) 0);
			if (!bPrev) pWnd2->ShowWindow(SW_HIDE);
			pDC->SetViewportOrg(ovpt);
		}
		else PaintItems(pDC, iTo, rc1);
		DrawFolder(pDC, iTo, frc, false);
	}
	else
	{
		frc.SetRect(0,0,frc.Width(),frc.Height());
		rc1.top += frc.Height();
		rc1.bottom += frc.Height() * 2;

		// bds 트리 콘트롤 위치 수정 (040305)
		//rc1.left += 5;

		pDC->FillSolidRect(rc1, crBackGroundColor);
		if (pWnd1) 
		{
			CPoint ovpt = pDC->SetViewportOrg(0, frc.Height());
			BOOL bPrev = pWnd1->ShowWindow(SW_SHOW);
			pWnd1->SendMessage(WM_ERASEBKGND, (WPARAM) pDC->GetSafeHdc(), (LPARAM) 0);
			pWnd1->SendMessage(WM_PAINT, (WPARAM) pDC->GetSafeHdc(), (LPARAM) 0);
			if (!bPrev) pWnd1->ShowWindow(SW_HIDE);
			pDC->SetViewportOrg(ovpt);
		}
		else PaintItems(pDC, iFrom, rc1);

		DrawFolder(pDC, iFrom, frc, false);

		pDC->SelectObject(&bmpTo);
		rc1.SetRect(0,0,rc.Width(), rc.Height() + frc.Height() * 2);
		pDC->FillSolidRect(rc1, crBackGroundColor);

		if (pWnd2) 
		{
			BOOL bPrev = pWnd2->ShowWindow(SW_SHOW);
			pWnd2->SendMessage(WM_ERASEBKGND, (WPARAM) pDC->GetSafeHdc(), (LPARAM) 0);
			pWnd2->SendMessage(WM_PAINT, (WPARAM) pDC->GetSafeHdc(), (LPARAM) 0);
			if (!bPrev) pWnd2->ShowWindow(SW_HIDE);
		}
		else PaintItems(pDC, iTo, rc1);
	}

	if (iTo > iFrom)
	{
		CRect rcFrom, rcTo;
		GetFolderRect(iFrom, rcFrom);
		GetFolderRect(iTo, rcTo);
		int fh = rcFrom.Height();
		for (int y = rcTo.top - fh; y > rcFrom.bottom; y -= fh)
		{
			pDC->SelectObject(&bmpFrom);
			dc.BitBlt(rc.left, rcFrom.bottom + 1, rc.Width(), y - rcFrom.bottom - 1, pDC, 0,fh, SRCCOPY);
			pDC->SelectObject(&bmpTo);
			dc.BitBlt(rc.left, y, rc.Width(), rc.bottom - y + fh, pDC, 0,0, SRCCOPY);
			Sleep(lAnimationTickCount);
		}
	}
	else
	{
		CRect rcFrom, rcTo;
		GetFolderRect(iFrom, rcFrom);
		int fh = rcFrom.Height();
		rcTo.SetRect(rc.left, rc.bottom, rc.right, rc.bottom - fh);
		for (int y = rcFrom.top + 1; y < rcTo.top - fh; y += fh)
		{
			pDC->SelectObject(&bmpTo);
			dc.BitBlt(rc.left, rcFrom.top, rc.Width(), y - rcFrom.top - 1, pDC, 0, fh*2, SRCCOPY);
			pDC->SelectObject(&bmpFrom);
			dc.BitBlt(rc.left, y, rc.Width(), rc.bottom - y, pDC, 0,0, SRCCOPY);
			Sleep(lAnimationTickCount);
		}
	}

	pDC->SelectObject(obmp);
}




CString CGfxOutBarCtrl::GetItemText(const int index)
{
	CString item;

	ASSERT(iSelFolder >= 0 && iSelFolder < GetFolderCount());
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
	if (IsValidItem(index))
	{
		CBarItem * pi = (CBarItem *) pbf->arItems.GetAt(index);
		if (pi->cItem) item = pi->cItem;
	}
	return item;
}

int CGfxOutBarCtrl::AddFolderBar(const char * pFolder, CWnd * pSon, const DWORD exData)
{
	CBarFolder * pbf = new CBarFolder(pFolder, exData);
	ASSERT(pbf);
	pbf->pChild = pSon;

	arFolder.Add((void *)pbf);

	return arFolder.GetSize() - 1;
}

CWnd * CGfxOutBarCtrl::GetFolderChild(int iFolder)
{
	int nFolderCnt = GetFolderCount();
	if (nFolderCnt <= iFolder) return NULL;
	if (nFolderCnt)
	{
		if (iFolder < 0) iFolder = iSelFolder;
		if (nFolderCnt <= iFolder) return NULL;
		CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iFolder);
		return pbf->pChild;
	}
	return NULL;
}

DWORD CGfxOutBarCtrl::GetFolderData(int iFolder)
{
	if (iFolder < 0) iFolder = iSelFolder;
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iFolder);
	return pbf->dwData;
}

void CGfxOutBarCtrl::SetFolderData(int iFolder, DWORD dwData)
{
	if (iFolder < 0) iFolder = iSelFolder;
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iFolder);
	if (pbf)
		pbf->dwData = dwData;
}

CString CGfxOutBarCtrl::GetFolderText(int iFolder)
{
	if (iFolder < 0) iFolder = iSelFolder;
	CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iFolder);
	CString strText;
	strText.Format(_T("%s"), pbf->cName);
	return strText;
}

void CGfxOutBarCtrl::SetAnimSelHighlight(const int iTime)
{
	if (iTime <= 0) KillTimer(3);
	else SetTimer(3, iTime, NULL);
	iSelAnimTiming = iTime;
}

// 폴더 색상의 첫 폴더 색상과 마지막 폴더 색상을 셋팅한다.
void CGfxOutBarCtrl::SetBackColor(BOOL bGradient, COLORREF bgTop, COLORREF bgBottom, COLORREF TextColor, COLORREF sTextColor)
{
	m_bGradient = FALSE;

	crTextColor = TextColor;
	crSelTextColor = sTextColor;
}

void CGfxOutBarCtrl::DeleteAllFolder()
{
	for (int t = 0; t < arFolder.GetSize(); t++)
	{
		if (arFolder.GetAt(t)) delete (CBarFolder*) arFolder.GetAt(t);
	}
	arFolder.RemoveAll();
	iSelFolder = 0;
}

void CGfxOutBarCtrl::OnDestroy() 
{
	if(m_Font.GetSafeHandle())
		m_Font.DeleteObject();

	CWnd::OnDestroy();
	DeleteAllFolder();
	delete pBlackPen;
}

void CGfxOutBarCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (GetFocus() != this) SetFocus();

	int index, ht = HitTestEx(point, index);
	iLastDragItemDrawType = -1;

	CRect inRc;
	GetInsideRect(inRc);

	if (ht == htFolder)
	{
		bool bHigh = true;
		CRect rc;
		GetFolderRect(index, rc);

		if (::GetCapture() == NULL)
		{
			SetCapture();
			ASSERT(this == GetCapture());
			CClientDC dc(this);
			DrawFolder(&dc, index, rc, true);
			AfxLockTempMaps();
			for (;;)
			{
				MSG msg;
				VERIFY(::GetMessage(&msg, NULL, 0, 0));

				if (CWnd::GetCapture() != this) break;

				switch (msg.message)
				{
				//case WM_MOUSEMOVE:
				//	{
				//		CPoint pt(msg.lParam);
				//		int idx, ht1 = HitTestEx(pt, idx);
				//		if (ht1 == htFolder && idx == index)
				//		{
				//			if (!bHigh)
				//			{
				//				DrawFolder(&dc, index, rc, true);
				//				bHigh = true;
				//			}
				//		}
				//		else
				//		{
				//			if (bHigh)
				//			{
				//				DrawFolder(&dc, index, rc, false);
				//				bHigh = false;
				//			}
				//		}
				//	}
				//	break;

				case WM_LBUTTONUP:
					{
						if (bHigh)
						{
							DrawFolder(&dc, index, rc, true);
							bHigh = false;
						}
						CPoint pt(msg.lParam);
						int idx, ht1 = HitTestEx(pt, idx);
						SetSelFolder(idx);
					}

					goto ExitLoop2;

				case WM_KEYDOWN:	
					if (msg.wParam != VK_ESCAPE) 
						break;

				default:
					DispatchMessage(&msg);
					break;
				}
			}

		ExitLoop2:
			ReleaseCapture();
			AfxUnlockTempMaps(FALSE);
		}
		if (bHigh) InvalidateRect(rc, false);
	}
	else iLastSelectedFolder = -1;

	if (ht == htItem)
	{
		iLastDragItemDraw = -1;
		bool bHigh = true, bDragging = false;
		CRect rc;
		GetItemRect(iSelFolder, index, rc);

		HCURSOR hCur = GetCursor();

		if (::GetCapture() == NULL)
		{
			SetCapture();
			ASSERT(this == GetCapture());
			CClientDC dc(this);
			HighlightItem(index, true);
			AfxLockTempMaps();
			for (;;)
			{
				MSG msg;
				VERIFY(::GetMessage(&msg, NULL, 0, 0));

				if (CWnd::GetCapture() != this) break;

				switch (msg.message)
				{
				//case WM_MOUSEMOVE:
				//	{
				//		CPoint pt(msg.lParam);
				//		int idx, ht1 = HitTestEx(pt, idx);
				//		if (bDragging)
				//		{
				//			if (ht1 == htItem) 
				//			{
				//				DrawDragArrow(&dc, index, idx);
				//				SetCursor(hDragCursor);
				//				hCur = hDragCursor;
				//			}
				//			else 
				//			{
				//				CRect rcItem;
				//				GetItemRect(iSelFolder, GetItemCount() - 1, rcItem);
				//				if (pt.y > rcItem.bottom && pt.y < inRc.bottom)
				//				{
				//					DrawDragArrow(&dc, index, GetItemCount());
				//					SetCursor(hDragCursor);
				//					hCur = hDragCursor;
				//				}
				//				else
				//				{
				//					DrawDragArrow(&dc, index, -1);
				//					SetCursor(hNoDragCursor);
				//					hCur = hNoDragCursor;
				//				}
				//			}
				//		}
				//		else
				//		{
				//			if (ht1 == htItem && idx == index)
				//			{
				//				if (!bHigh)
				//				{
				//					HighlightItem(index, true);
				//					bHigh = true;
				//					bPressedHighlight = true;
				//				}
				//			}
				//			else 
				//			{
				//				if (ht1 == htItem)
				//				{
				//					if (bHigh)
				//					{
				//						HighlightItem(index, false);
				//						bHigh = false;
				//						bPressedHighlight = false;
				//					}
				//				}
				//				else
				//				{
				//					if (dwFlags&fDragItems)
				//					{
				//						HighlightItem(index, true);
				//						bHigh = true;
				//						bDragging = true;

				//						SetCursor(hDragCursor);
				//						hCur = hDragCursor;

				//						bPressedHighlight = true;
				//					}
				//				}
				//			}
				//		}
				//	}
				//	break;

				case WM_SETCURSOR:
					SetCursor(hCur);
					break;

				case WM_LBUTTONUP:
					{
						if (bHigh)
						{
							HighlightItem(-1);
							bHigh = false;
						}
						CPoint pt(msg.lParam);
						int idx, ht1 = HitTestEx(pt, idx);
						if (!bDragging)
						{
							if (ht1 == htItem && idx == index) 
							{
								if (iSelAnimTiming > 0 && index != iLastSel && iLastSel >= 0)
								{
									DrawAnimItem(0, 0, iLastSel);
								}

								if (dwFlags&fSelHighlight && iLastSel >= 0)
								{
									CRect rc;
									GetIconRect(iSelFolder, iLastSel, rc);
									rc.InflateRect(1,1);
									InvalidateRect(rc);
								}

								iLastSel = index;

								if (dwFlags&fSelHighlight && iLastSel >= 0)
								{
									CRect rc;
									GetIconRect(iSelFolder, iLastSel, rc);
									rc.InflateRect(1,1);
									InvalidateRect(rc);
								}

								GetOwner()->SendMessage(WMU_OUTBAR_NOTIFY, NM_OB_ITEMDBCLICK, idx);
							}
						}
						else
						{
							if (ht1 == htItem)
							{
								if (idx != index)
								{
									OUTBAR_INFO ob;
									ob.iDragFrom = iSelFolder;
									ob.iDragTo = index;
									if (GetOwner()->SendMessage(WMU_OUTBAR_NOTIFY, NM_OB_DRAGITEM, (LPARAM) &ob))
									{
										CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
										CBarItem * piFrom = (CBarItem *) pbf->arItems.GetAt(index);
										pbf->arItems.RemoveAt(index);
										if (idx > index) idx --;
										pbf->arItems.InsertAt(idx, piFrom);
									}
								}
							}
							else
							{
								CRect rcItem;
								GetItemRect(iSelFolder, GetItemCount() - 1, rcItem);
								if (pt.y > rcItem.bottom && pt.y < inRc.bottom)
								{
									OUTBAR_INFO ob;
									ob.iDragFrom = iSelFolder;
									ob.iDragTo = index;
									if (GetOwner()->SendMessage(WMU_OUTBAR_NOTIFY, NM_OB_DRAGITEM, (LPARAM) &ob))
									{
										CBarFolder * pbf = (CBarFolder *) arFolder.GetAt(iSelFolder);
										CBarItem * piFrom = (CBarItem *) pbf->arItems.GetAt(index);
										pbf->arItems.RemoveAt(index);
										pbf->arItems.Add((void *) piFrom);
									}
								}
							}
						}
					}

					goto ExitLoop4;

				default:
					DispatchMessage(&msg);
					break;
				}
			}

		ExitLoop4:
			ReleaseCapture();
			AfxUnlockTempMaps(FALSE);
			if (bDragging) 
			{
				Invalidate();
			}
		}
		bPressedHighlight = false;
		if (bHigh) InvalidateRect(rc, false);
	}
	else iLastItemHighlighted = -1;

	if (ht == htDownScroll)
	{
		bLooping = true;
		bool bHigh = true;

		if (::GetCapture() == NULL)
		{
			SetCapture();
			ASSERT(this == GetCapture());
			CClientDC dc(this);
			dc.DrawFrameControl(rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN|DFCS_PUSHED);
			SetTimer(2,300,NULL);
			AfxLockTempMaps();
			for (;;)
			{
				MSG msg;
				VERIFY(::GetMessage(&msg, NULL, 0, 0));

				if (CWnd::GetCapture() != this) break;

				switch (msg.message)
				{
				//case WM_MOUSEMOVE:
				//	{
				//		CPoint pt(msg.lParam);
				//		if (rcDownArrow.PtInRect(pt))
				//		{
				//			if (bHigh == false)
				//			{
				//				dc.DrawFrameControl(rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN|DFCS_PUSHED);
				//				bHigh = true;
				//				bDownArrow = true;
				//				bDownPressed = true;
				//			}
				//		}
				//		else
				//		{
				//			if (bHigh == true)
				//			{
				//				dc.DrawFrameControl(rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN);
				//				bHigh = false;
				//				bDownArrow = false;
				//				bDownPressed = false;
				//			}
				//		}
				//	}
				//	break;

				case WM_LBUTTONUP:
					{
						if (bHigh)
						{
							dc.DrawFrameControl(rcDownArrow, DFC_SCROLL, DFCS_SCROLLDOWN);
							bHigh = false;
						}
						bDownArrow = false;
						CPoint pt(msg.lParam);
						if (rcDownArrow.PtInRect(pt))
						{
							CRect itrc;
							GetItemRect(iSelFolder, GetItemCount() - 1, itrc);
							CRect crc;
							GetInsideRect(crc);
							if (itrc.bottom > crc.bottom)
							{
								iFirstItem ++;
								InvalidateRect(crc, true);
							}
						}
					}
					goto ExitLoop3;


				case WM_TIMER:
					{
						if (msg.wParam == 2)
						{
							if (bHigh)
							{
								CPoint pt(msg.pt);
								ScreenToClient(&pt);
								if (rcDownArrow.PtInRect(pt))
								{
									bDownPressed = true;
									CRect itrc;
									GetItemRect(iSelFolder, GetItemCount() - 1, itrc);
									CRect crc;
									GetInsideRect(crc);
									if (itrc.bottom > crc.bottom)
									{
										iFirstItem ++;
										InvalidateRect(crc, true);
									}
									else goto ExitLoop3;
								}
								else bDownPressed = false;
							}
						}
						break;
					}

				case WM_KEYDOWN:	
					if (msg.wParam != VK_ESCAPE) 
						break;

				default:
					DispatchMessage(&msg);
					break;
				}
			}

		ExitLoop3:
			KillTimer(2);
			ReleaseCapture();
			AfxUnlockTempMaps(FALSE);
			bLooping = false;
			bDownPressed = false;
			bDownArrow = false;
			CRect crc;
			GetInsideRect(crc);
			InvalidateRect(crc, true);
		}
	}

	if (ht == htUpScroll)
	{
		bLooping = true;
		bool bHigh = true;

		if (::GetCapture() == NULL)
		{
			SetCapture();
			ASSERT(this == GetCapture());
			CClientDC dc(this);
			dc.DrawFrameControl(rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP|DFCS_PUSHED);
			SetTimer(2,300,NULL);
			AfxLockTempMaps();
			for (;;)
			{
				MSG msg;
				VERIFY(::GetMessage(&msg, NULL, 0, 0));

				if (CWnd::GetCapture() != this) break;

				switch (msg.message)
				{
				//case WM_MOUSEMOVE:
				//	{
				//		CPoint pt(msg.lParam);
				//		if (rcUpArrow.PtInRect(pt))
				//		{
				//			if (bHigh == false)
				//			{
				//				dc.DrawFrameControl(rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP|DFCS_PUSHED);
				//				bHigh = true;
				//				bUpArrow = true;
				//				bUpPressed = true;
				//			}
				//		}
				//		else
				//		{
				//			if (bHigh == true)
				//			{
				//				dc.DrawFrameControl(rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP);
				//				bHigh = false;
				//				bUpArrow = false;
				//				bUpPressed = false;
				//			}
				//		}
				//	}
				//	break;

				case WM_LBUTTONUP:
					{
						if (bHigh)
						{
							dc.DrawFrameControl(rcUpArrow, DFC_SCROLL, DFCS_SCROLLUP);
							bHigh = false;
						}
						bUpArrow = false;
						CPoint pt(msg.lParam);
						if (rcUpArrow.PtInRect(pt))
						{
							if (iFirstItem > 0 ) 
							{
								iFirstItem --;
								CRect crc;
								GetInsideRect(crc);
								InvalidateRect(crc, true);
							}
						}
					}

					goto ExitLoop;

				case WM_TIMER:
					{
						if (msg.wParam == 2)
						{
							if (bHigh)
							{
								CPoint pt(msg.pt);
								ScreenToClient(&pt);
								if (rcUpArrow.PtInRect(pt))
								{
									bUpPressed = true;
									if (iFirstItem > 0) 
									{
										iFirstItem --;
										CRect crc;
										GetInsideRect(crc);
										InvalidateRect(crc, true);
									}
									else goto ExitLoop;
								}
								else bUpPressed = false;
							}
						}
						break;
					}
				case WM_KEYDOWN:	
					if (msg.wParam != VK_ESCAPE) break;

				default:
					DispatchMessage(&msg);
					break;
				}
			}

		ExitLoop:
			KillTimer(2);
			ReleaseCapture();
			AfxUnlockTempMaps(FALSE);
			bLooping = false;
			bUpPressed = false;
			bUpArrow = false;
			CRect crc;
			GetInsideRect(crc);
			InvalidateRect(crc, true);
		}
	}
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CGfxOutBarCtrl::SetFolderImageList(CImageList * pImageList, int nNormal, int nDown, int nOver)
{
	m_pFolderImageList = pImageList;
	m_nNormal = nNormal;
	m_nDown = nDown;
	m_nOver = nOver;
}
