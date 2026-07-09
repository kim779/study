// fxTab.cpp : implementation file
//

#include "stdafx.h"
#include "fxTab.h"
#include "memdc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TAB_GAP		3
#define	AL_TOP		1
#define	AL_LEFT		2
#define	AL_RIGHT	3
#define	AL_BOTTOM	4
#define	clBOTTOM	RGB(100, 100, 100)
#define	clWHITE		RGB(255, 255, 255)
#define	IMAGEDIR	"Image"
#define	TABDIR		"Tab"
#define PALETTEFILE	"PALETTE.INI"
#define IMGSEL_L	"L_S"
#define	IMGSEL_M	"S"
#define	IMGSEL_R	"R_S"
#define IMG_L		"L_"
#define	IMG_M		""
#define	IMG_R		"R_"
#define	IMGSPACE	"B_"
#define IMG_FILE	"TAB.BMP"

const int bmp_selL = 0, bmp_selM = 1, bmp_selR = 2, bmp_norL = 3, bmp_norM = 4, bmp_norR = 5, bmp_sp = 6;

/////////////////////////////////////////////////////////////////////////////
// CfxTab

CfxTab::CfxTab()
{
	m_bkColor   = RGB(255, 255, 255);
	m_backColor = RGB(255, 255, 255);
	m_frColor   = RGB(0, 0, 0);
	m_selectedbColor = RGB(190, 190, 190);
	m_selectedfColor = RGB(255, 255, 255);
	m_textUpDown= -2;

	m_pFont = NULL;
	m_selfLoad = false;

	m_bmpSelL = m_bmpSelM = m_bmpSelR = m_bmpL = m_bmpM = m_bmpR = m_bmpSpace = NULL;
	for (int ii = 0; ii <= bmp_sp; ii++)
		m_pics[ii] = NULL;
}

CfxTab::~CfxTab()
{
}


BEGIN_MESSAGE_MAP(CfxTab, CTabCtrl)
	//{{AFX_MSG_MAP(CfxTab)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfxTab message handlers

BOOL CfxTab::Create(CRect rect, CWnd* parent, UINT nID)
{
	DWORD dwStyle = WS_CHILD|WS_VISIBLE;
	return CTabCtrl::Create(dwStyle, rect, parent, nID);
}

LOGFONT CfxTab::getLogFont(int fsize, DWORD weight)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = fsize * 10; 
	
	strcpy(lf.lfFaceName, "굴림체");   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = weight;
	lf.lfItalic = FALSE;
	return lf;
}

void CfxTab::DrawItem(LPDRAWITEMSTRUCT lpdis)
{
}

void CfxTab::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;
	if (redraw) RedrawWindow();
}

void CfxTab::SetColor(COLORREF fcolor, COLORREF bcolor, COLORREF sfcolor, COLORREF sbcolor, COLORREF backColor)
{
	m_frColor = fcolor;
	m_bkColor = bcolor;
	m_selectedfColor = sfcolor;
	m_selectedbColor = sbcolor;
	m_backColor = backColor;
}

void CfxTab::SetStyle(DWORD style, bool redraw)
{
	m_style = style;
	if (m_style == tsIMAGE)
	{
		SetMinTabWidth(5);
		SetPadding(CSize(6, 0));
		SetItemSize(CSize(5, 25));
	} 
	else
	{

	}

	if (redraw)
		Invalidate();
}

int CfxTab::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CfxTab::OnDestroy() 
{
	CTabCtrl::OnDestroy();
	ClearImage();
}

BOOL CfxTab::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
}

void CfxTab::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC MemDC(&dc);
	OnDraw(&MemDC);
}

void CfxTab::OnDraw(CDC *pDC)
{
	switch (m_style)
	{
	case tsNORMAL: drawNormal(pDC);
		break;
	case tsIMAGE: drawImage(pDC);
		break;
	}
}


void CfxTab::draw3dRoundRect(CDC *dc, CRect rc, int align, int depth, COLORREF borderColor)
{
	CRect tRc = rc;
	CBrush	Brush, *pOldBrush;
	CPen	Pen, *pOldPen;

	Brush.CreateSolidBrush(clWHITE);
	Pen.CreatePen(PS_SOLID, 0, clBOTTOM);
	pOldPen = (CPen*)dc->SelectObject(&Pen);
	pOldBrush = (CBrush*)dc->SelectObject(&Brush);

	dc->RoundRect(rc, CPoint(depth, depth));
	
	switch(align)
	{
	case AL_TOP:
		{
			dc->SelectObject(pOldBrush);
			dc->SelectObject(pOldPen);
			Pen.DeleteObject();
			Brush.DeleteObject();
			Brush.CreateSolidBrush(borderColor);
			pOldBrush = (CBrush*)dc->SelectObject(&Brush);
			Pen.CreatePen(PS_SOLID, 0, borderColor);
			pOldPen = (CPen*)dc->SelectObject(&Pen);

			tRc.DeflateRect(2, 2, 1, 1);
			dc->RoundRect(tRc, CPoint(depth - 2, depth - 2));
			tRc = rc;
			tRc.right = tRc.left + 2;
			tRc.top = tRc.bottom - depth;
			draw2Rect(dc, tRc, clBOTTOM, clWHITE);
			tRc = rc;
			tRc.left = tRc.right - 2;
			tRc.top = tRc.bottom - depth;
			draw2Rect(dc, tRc, clWHITE, clBOTTOM);
			tRc = rc;
			tRc.top = tRc.bottom - depth;
			tRc.DeflateRect(2, 0, 1, 0);
			dc->FillSolidRect(tRc, borderColor);
		}
		break;
	}
	dc->SelectObject(pOldBrush);
	dc->SelectObject(pOldPen);
	Pen.DeleteObject();
	Brush.DeleteObject();
}

void CfxTab::draw2Rect(CDC *dc, CRect rc, COLORREF color1, COLORREF color2)
{
	if (rc.Width() == 2)
	{
		CRect tRc = rc;
		dc->FillSolidRect(tRc, color1);
		tRc.left++;
		dc->FillSolidRect(tRc, color2);
	}
	else
	{
		CRect tRc = rc;
		dc->FillSolidRect(tRc, color1);
		tRc.top++;
		dc->FillSolidRect(tRc, color2);
	}
}

void CfxTab::SetImage(HBITMAP hselectL, HBITMAP hSelectM, HBITMAP hSelectR, HBITMAP hL, HBITMAP hM, HBITMAP hR, HBITMAP hSpace)
{
	m_pics[bmp_selL] = hselectL;
	m_pics[bmp_selM] = hSelectM;
	m_pics[bmp_selR] = hSelectR;
	m_pics[bmp_norL] = hL;
	m_pics[bmp_norM] = hM;
	m_pics[bmp_norR] = hR;
	m_pics[bmp_sp]   = hSpace;
}

void CfxTab::SetImage(CString imageDir, CString selectL, CString selectM, CString selectR, CString left, CString middle, CString right, CString space)
{
	ClearImage();
	m_selfLoad = true;

	CString path;
	path.Format("%s\\%s", imageDir, selectL);	m_bmpSelL = CreateBmp(path);
	path.Format("%s\\%s",   imageDir, selectM);	m_bmpSelM = CreateBmp(path);
	path.Format("%s\\%s", imageDir, selectR);	m_bmpSelR = CreateBmp(path);
	path.Format("%s\\%s",  imageDir, left);		m_bmpL = CreateBmp(path);
	path.Format("%s\\%s",    imageDir, middle);	m_bmpM = CreateBmp(path);
	path.Format("%s\\%s",  imageDir, right);	m_bmpR = CreateBmp(path);
	path.Format("%s\\%s",  imageDir, space);	m_bmpSpace = CreateBmp(path);

	SetImage(m_bmpSelL->operator HBITMAP(), m_bmpSelM->operator HBITMAP(), m_bmpSelR->operator HBITMAP(), 
		m_bmpL->operator HBITMAP(), m_bmpM->operator HBITMAP(), m_bmpR->operator HBITMAP(), m_bmpSpace->operator HBITMAP());	
}

void CfxTab::LoadTabImage(CString rootDir, bool bTopTab)
{
	CString path, prefix;
	CString selL, selM, selR, left, middle, right, space;
	path.Format("%s\\%s\\%s", rootDir, TABDIR, PALETTEFILE); 
		
	CFileFind find;
	if (!find.FindFile(path)) return;

	char dat[256];
	GetPrivateProfileString("General", "Palette", "Blue", dat, sizeof(dat), path);
	prefix = dat;
	
	path.Format("%s\\%s", rootDir, IMAGEDIR);
	if (bTopTab == true)
	{
		selL.Format("%s_%s%s", prefix, IMGSEL_L, IMG_FILE);
		selM.Format("%s_%s%s", prefix, IMGSEL_M, IMG_FILE);
		selR.Format("%s_%s%s", prefix, IMGSEL_R, IMG_FILE);
		left.Format("%s_%s%s", prefix, IMG_L, IMG_FILE);
		middle.Format("%s_%s%s", prefix, IMG_M, IMG_FILE);
		right.Format("%s_%s%s", prefix, IMG_R, IMG_FILE);
		space.Format("%s_%s%s", prefix, IMGSPACE, IMG_FILE);
	}
	else
	{
		selL.Format("%s%s%s", IMGSEL_L, prefix, IMG_FILE);
		selM.Format("%s%s%s", IMGSEL_M, prefix, IMG_FILE);
		selR.Format("%s%s%s", IMGSEL_R, prefix, IMG_FILE);
		left.Format("%s%s%s", IMG_L, prefix, IMG_FILE);
		middle.Format("%s%s%s", IMG_M, prefix, IMG_FILE);
		right.Format("%s%s%s", IMG_R, prefix, IMG_FILE);
		space.Format("%s%s%s", IMGSPACE, prefix, IMG_FILE);
	}

	SetImage(path, selL, selM, selR, left, middle, right, space);
	Invalidate();
}

CBitmap* CfxTab::CreateBmp( LPCTSTR sBMPFile/*, CPalette *pPal */)
{
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead) )
		return NULL;

	BITMAPFILEHEADER bmfHeader;

	// Read file header
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return NULL;

	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return NULL;

	// Get length of the remainder of the file and allocate memory
	DWORD nPackedDIBLen = file.GetLength() - sizeof(BITMAPFILEHEADER);
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
		return NULL;

	// Read the remainder of the bitmap file.
	if (file.ReadHuge((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
	{
		::GlobalFree(hDIB);
		file.Close();
		return NULL;
	}
	file.Close();

	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;

	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed :
						1 << bmiHeader.biBitCount;

	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) +
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	CClientDC dc(NULL);

	HBITMAP hBmp = CreateDIBitmap( dc.m_hDC, &bmiHeader, CBM_INIT, lpDIBBits, &bmInfo, DIB_RGB_COLORS);
	CBitmap *bitmap;
	bitmap = new CBitmap();
	bitmap->Attach( hBmp );

	::GlobalFree(hDIB);
	return bitmap;
}

int CfxTab::bmpCopy(CDC *dc, CDC *memDC, int bmpidx, int posX, int posTo, int height)
{
	HBITMAP pbmp = NULL;
	BITMAP	bm;
	int	bmH = height;

	pbmp = (HBITMAP)memDC->SelectObject(m_pics[bmpidx]);
	GetObject(m_pics[bmpidx], sizeof(bm), &bm);
	if (bmH == -1)
		bmH = bm.bmHeight;

	if (posTo == -1)
	{
		dc->StretchBlt(posX, 0, bm.bmWidth, bmH, memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		posX += bm.bmWidth;
	} 
	else if (posTo > 0)
	{
		dc->StretchBlt(posX, 0, posTo - posX + 1, bmH, memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		posX = posTo;		
	}
	memDC->SelectObject(pbmp);

	return posX;
}

int CfxTab::getBmpWidth(int bmpidx)
{
	BITMAP bm;
	GetObject(m_pics[bmpidx], sizeof(bm), &bm);
	return bm.bmWidth;
}

void CfxTab::ClearImage()
{
	if (!m_selfLoad)
		return;

	if (m_bmpSelL)	{ m_bmpSelL->DeleteObject(); delete m_bmpSelL; m_bmpSelL = NULL; };
	if (m_bmpSelM)	{ m_bmpSelM->DeleteObject(); delete m_bmpSelM; m_bmpSelM = NULL; };
	if (m_bmpSelR)	{ m_bmpSelR->DeleteObject(); delete m_bmpSelR; m_bmpSelR = NULL; };
	if (m_bmpL)	{ m_bmpL->DeleteObject(); delete m_bmpL; m_bmpL = NULL; };
	if (m_bmpM)	{ m_bmpM->DeleteObject(); delete m_bmpM; m_bmpM = NULL; };
	if (m_bmpR)	{ m_bmpR->DeleteObject(); delete m_bmpR; m_bmpR = NULL; };
	if (m_bmpSpace)	{ m_bmpSpace->DeleteObject(); delete m_bmpSpace; m_bmpSpace = NULL; };
}

void CfxTab::drawNormal(CDC *pDC)
{
	char        szTabText[100];
	TC_ITEM     tci;
	CRect rt, tRc1, tBRc;
	CFont	font, *pOldFont;
	bool	bSelect = false;
	int	tabHeight = 0;
 
	GetClientRect(&rt);
	pDC->FillSolidRect(&rt, m_backColor);

	for (int ii = 0; ii < GetItemCount(); ii++)
	{
		tci.mask        = TCIF_TEXT;
		tci.pszText     = szTabText;
		tci.cchTextMax  = sizeof(szTabText) - 1;
		GetItem(ii, &tci);
		
		bSelect = GetCurSel() == ii;

		// set font
		LOGFONT lf;
		if (m_pFont)
			m_pFont->GetLogFont(&lf);
		else
		{ 
			if (GetParent()->GetFont())
				GetParent()->GetFont()->GetLogFont(&lf);
			else
			{
				lf = getLogFont(9, NULL);
				lf.lfHeight = -12;
			}
		}

		lf.lfWeight = (bSelect ? FW_SEMIBOLD : FW_NORMAL);
		font.CreateFontIndirect(&lf);//, &dc);
		pOldFont = (CFont*)pDC->SelectObject(&font);
		
		GetItemRect(ii, &rt);
		if (!ii)	// border 판을 먼저그린다.
		{
			GetClientRect(&tBRc);
			tBRc.DeflateRect(0, rt.bottom - 1, 0, 0);
			pDC->Draw3dRect(&tBRc, clBOTTOM, clBOTTOM);
			tBRc.DeflateRect(1, 1, 1, 1);
			pDC->Draw3dRect(tBRc, clWHITE, clBOTTOM);
		}
		GetItemRect(ii, &rt);
		if (bSelect)
			rt.InflateRect(2, 2, 0, 1);
		else
			rt.InflateRect(0, 0, 0, 1);
		tRc1 = rt;
		tRc1.DeflateRect(0, 0, 0, 1);
		int bkMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF color = pDC->SetTextColor(m_selectedfColor);
		if (bSelect)
		{
			draw3dRoundRect(pDC, tRc1, AL_TOP, 5, m_bkColor);
			pDC->SetTextColor(m_selectedfColor);
		}
		else
		{
			draw3dRoundRect(pDC, tRc1, AL_TOP, 5, m_bkColor);
			tRc1 = rt;
			tRc1.top = tRc1.bottom - 2;
			pDC->FillSolidRect(&tRc1, clBOTTOM);
			pDC->SetTextColor(m_frColor);
		}
		tRc1 = rt;
		tRc1.top = tRc1.bottom - 1;

		if (bSelect)
		{
			tRc1.DeflateRect(2, 0, 0, 0);
			pDC->FillSolidRect(&tRc1, m_selectedbColor);
		}
		else
			pDC->FillSolidRect(&tRc1, clWHITE);

		tRc1 = rt;
		tRc1.DeflateRect(2, (bSelect?2:3), 1, 1);
		if (bSelect)
			pDC->FillSolidRect(&tRc1, m_selectedbColor);
		
		pDC->DrawText(tci.pszText, lstrlen(tci.pszText), tRc1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		pDC->SetBkMode(bkMode);
		pDC->SetTextColor(color);
		pDC->SelectObject(pOldFont);
		font.DeleteObject();
	}
	tBRc.DeflateRect(1, 1, 1, 1);
	pDC->FillSolidRect(&tBRc, m_selectedbColor);	
}

void CfxTab::drawImage(CDC* pDC)
{
	char        szTabText[100];
	TC_ITEM     tci;
	CRect rt, rtItem, rcText;
	CFont	font, *pOldFont;
	bool	bSelect = false;
	int	nCheck, posX;
	CDC	memDC;
	BITMAP	bm;

	GetClientRect(&rt);
	pDC->FillSolidRect(&rt, m_backColor);
	int bkMode = pDC->SetBkMode(TRANSPARENT);

	LOGFONT lf;
	if (m_pFont)
		m_pFont->GetLogFont(&lf);
	else
	{ 
		if (GetParent()->GetFont())
			GetParent()->GetFont()->GetLogFont(&lf);
		else
		{
			lf = getLogFont(9, NULL);
			lf.lfHeight = -12;
		}
	}

	posX = 1;
	nCheck = GetCurSel();
	memDC.CreateCompatibleDC(pDC);
	for (int ii = 0; ii < GetItemCount(); ii++)
	{
		tci.mask        = TCIF_TEXT;
		tci.pszText     = szTabText;
		tci.cchTextMax  = sizeof(szTabText) - 1;
		GetItem(ii, &tci);
		
		bSelect = nCheck == ii;

		// Set font
		lf.lfWeight = (bSelect ? FW_SEMIBOLD : FW_NORMAL);
		font.CreateFontIndirect(&lf);
		pOldFont = (CFont*)pDC->SelectObject(&font);
		
		GetItemRect(ii, &rtItem);

		COLORREF color;
		if (rtItem.left > 0)
		{
			// Draw Image
			if (bSelect)
			{
				posX = bmpCopy(pDC, &memDC, bmp_selL, posX -1, -1, rt.Height());
				posX = bmpCopy(pDC, &memDC, bmp_selM, posX, rtItem.right - getBmpWidth(bmp_selR)/* + 1*/, rt.Height());
				posX = bmpCopy(pDC, &memDC, bmp_selR, posX -1, -1, rt.Height());
				//posX--;

				color = pDC->SetTextColor(m_selectedfColor);
			} 
			else
			{
				posX = bmpCopy(pDC, &memDC, bmp_norL, posX, -1, rt.Height());
				if (nCheck == ii + 1)
				{
					posX = bmpCopy(pDC, &memDC, bmp_norM, posX, rtItem.right, rt.Height());
				} 
				else
				{
					posX = bmpCopy(pDC, &memDC, bmp_norM, posX, rtItem.right - getBmpWidth(bmp_selR)/* + 1*/, rt.Height());
					//if (ii != GetItemCount() -1)
						posX = bmpCopy(pDC, &memDC, bmp_norR, posX -1, -1, rt.Height());
				}

				color = pDC->SetTextColor(m_frColor);
			}
			rcText = rtItem;
			rcText.OffsetRect(CPoint(0, m_textUpDown));
			pDC->DrawText(tci.pszText, lstrlen(tci.pszText), CRect(rcText.left, rcText.top, rcText.right, rcText.bottom), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			pDC->SetTextColor(color);
		}
		else
		{
			if (rtItem.right > 0)
			{

				if (bSelect)
				{
					GetObject(m_pics[bmp_selR], sizeof(bm), &bm);
					posX = bmpCopy(pDC, &memDC, bmp_selR, rtItem.right - bm.bmWidth, -1, rt.Height());
				}
				else
				{
					GetObject(m_pics[bmp_norR], sizeof(bm), &bm);
					posX = bmpCopy(pDC, &memDC, bmp_norR, rtItem.right - bm.bmWidth, -1, rt.Height());
				}
			}
		}

		pDC->SelectObject(pOldFont);
		font.DeleteObject();
	}
	posX = bmpCopy(pDC, &memDC, bmp_sp, posX, rt.right, rt.Height());

	pDC->SetBkMode(bkMode);
	memDC.DeleteDC();
}
