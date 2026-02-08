// MapWnd.cpp : implementation file
//

#include <Afxtempl.h>
#include "stdafx.h"
#include "IB420300.h"
#include "MapWnd.h"
#include "memDC.h"
//#include "../../control/fx_misc/fxStatic.h"
//#include "../../control/fx_misc/miscType.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TRN	"PIBF4203"	//2012.15.05 KSJ PIBO4203 --> PIBF4203

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

#define	DESTROY_WND(wnd) if (wnd) {wnd->DestroyWindow(); delete wnd; wnd = NULL; }
CMapWnd::CMapWnd()
{
	m_pBitmap = NULL;
	m_pPopwnd = NULL;
	m_pParent = NULL;
//	m_pBorder = NULL;
	m_pFont	  = NULL;
	m_pFontB  = NULL;
	m_pKospiBtn = NULL;
	m_pKostarBtn = NULL;
	m_bMouseClicked = FALSE;
	m_nFontSize = 9;
	m_bDraw = FALSE;
	m_listUpper.RemoveAll();
	m_listLower.RemoveAll();

	m_mapItem.RemoveAll();
}

CMapWnd::~CMapWnd()
{
	m_mapItem.RemoveAll();
	//DESTROY_WND(m_pBorder);
	//DESTROY_WND(m_pKospiBtn);
	//DESTROY_WND(m_pKostarBtn);
	//DESTROY_WND(m_pPopwnd);

//	if (m_pBitmap)	delete m_pBitmap;
	if (m_br.m_hObject)	m_br.DeleteObject();

	m_listUpper.RemoveAll();
	m_listLower.RemoveAll();
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_KOSPIBTN, OnKospiClick)
	ON_BN_CLICKED(ID_KOSTARBTN, OnKostarClick)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if(!m_bDraw)
		return;

	xxx::CMemDC	mdc(&dc);
	
//	m_section.Lock();	
	Draw(&mdc);		
//	m_section.Unlock();
/*
	if (m_pBitmap)
	{
		CDC		memDC;
		CBitmap*	pOldBitmap;

		memDC.CreateCompatibleDC(&dc);	
		if (memDC.m_hDC != NULL) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);
		dc.BitBlt(0, 0, m_clientRt.Width(), m_clientRt.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
*/
}

LRESULT CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CString sym, dat, str;
	int pos = 0;
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		CopyMemory(&m_data, (char*)lParam, sizeof(m_data));	// m_data => struct _outbound
		m_bDraw = TRUE;
		makeMapItem();
		drawBitmap();
		
		Invalidate();
		break;
	//case DLL_ALERT:
	//	{
	//		str = CString((char*)lParam);
	//		CString code;
	//		code = str.Mid(0, str.Find('\t'));
	//		str = str.Mid(str.Find('\t') + 1);
	//		
	//		if (code.GetLength() == 4 && code.Compare("RANK") == 0)
	//		{
	//			do
	//			{
	//				sym = str.Mid(pos, str.Find('\t', pos) - pos);
	//				pos = str.Find('\t', pos) + 1;
	//				dat = str.Mid(pos, str.Find('\t', pos) - pos);
	//				pos = str.Find('\t', pos) + 1;
	//				if (sym.Compare("034") == 0 && m_time.Compare(str) != 0)
	//				{
	//					SendTR(TRN, m_pKospiBtn->GetCheck() == 1 ? "1" : "2");
	//					break;
	//				}
	//			}while (pos != 0);
	//		}
	//	}
	//	break;

	case DLL_ALERTx :
		{
			struct _alertR* alertR = (struct _alertR*)lParam;
			CString code = alertR->code;
			if (code.GetLength() == 4 && code.Compare("RANK") == 0)
			{
				for (int i = alertR->size - 1; i > -1; --i)
				{
					DWORD* data = (DWORD*)alertR->ptr[i];
					if (data[34]) { // 체결시간 값 비교처리
						CString val = (char*)data[34];
						if (m_time.Compare(val) != 0) {
							/*TRACE("IBXXXX00 체결시간 : [ %s ] %s\n", code.GetString(), val.GetString());*/
							SendTR(TRN, m_pKospiBtn->GetCheck() == 1 ? "1" : "2");
							break;
						} 
					}
				}
			}
		}
		break;

	case DLL_TRIGGER:
		break;
	case DLL_DOMINO:
		break;
	case DLL_NOTICE:
		break;
	case DLL_SETPAL:
		m_strPal = GetPalette();
		m_crBack = GetIndexColor(clBack);		
		m_crBackIn = GetIndexColor(clPanel);
		if (m_br.m_hObject)	m_br.DeleteObject();
		m_br.CreateSolidBrush(m_crBackIn);
		Invalidate();
		break;
	case DLL_SETFONTx:
		m_nFontSize = (int)HIWORD(wParam);
		m_pFont = RequestFont("굴림체", m_nFontSize);
		m_pFontB = RequestFont("굴림체", m_nFontSize, FALSE, FW_BOLD);
		SetResize(m_nFontSize);
		break;
	}

	return 0;
}

void CMapWnd::SendTR(CString name, CString data)
{
	std::unique_ptr<char[]> pnt = std::make_unique<char[]>(1000);
	
	struct _userTH udat;	
	
	strcpy(udat.trc, name);
	udat.key = 0;
	udat.stat = US_PASS;

	ZeroMemory(pnt.get(), 1000);
	CopyMemory(pnt.get(), &udat, L_userTH);
	CopyMemory(pnt.get() + L_userTH, data.operator LPCTSTR(), data.GetLength());
	
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, data.GetLength()), (LPARAM)pnt.get());
	
	pnt = nullptr;
}

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	//OutputDebugString("4203 START\n");
	m_pFont = RequestFont("굴림체", 9, FALSE, 0);
	m_pFontB = RequestFont("굴림체", 9, FALSE, FW_BOLD);

	m_strRoot = Variant(homeCC, "");
	m_strPal = GetPalette();

//	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(nWidth, nHeight));

/*	m_pBorder = new CfxStatic();
	m_pBorder->Create("", WS_CHILD|WS_VISIBLE|SS_LEFT|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, map_Width, borderH), this);
	m_pBorder->Init(ssGRADIENT, 0, "", "", GetIndexColor(66), GetIndexColor(67),
		GetSysColor(COLOR_BTNTEXT), alLEFT);
*/
	m_crBack = GetIndexColor(clBack);
	m_crBackIn = GetIndexColor(clPanel);
	m_br.CreateSolidBrush(m_crBackIn);

	m_pKospiBtn = std::make_unique<CTransButton>();
	m_pKospiBtn->Create("KOSPI200 지수", WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTORADIOBUTTON, BTN_KOSPI_RECT, this, ID_KOSPIBTN);
	m_pKospiBtn->SetFont(m_pFont);

	m_pKostarBtn = std::make_unique<CTransButton>();
	m_pKostarBtn->Create("STAR 지수", WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_AUTORADIOBUTTON, BTN_KOSTAR_RECT, this, ID_KOSTARBTN);
	m_pKostarBtn->SetFont(m_pFont);
	m_pKostarBtn->ShowWindow(FALSE);

	m_pPopwnd = std::make_unique<CPopWnd>();
	if (!m_pPopwnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 2))
		AfxMessageBox("Popup Window 생성오류!");

	Variant(titleCC, "[4203]KOSPI200 변동비중 현황");
	GetClientRect(&m_clientRt);

	m_pKospiBtn->SetCheck(true);
	OnKospiClick();
	//OutputDebugString("4203 END\n");
	return 0;
}

void CMapWnd::Draw(CDC *pDC)
{
	int xx=0, yy=0;
	CFont		*font=nullptr, *pOldFont=nullptr;
	CRect		rect, rect2;
	CString		temp, temp2;
	CString		codeS, codeS2;
	CRect		codeR, codeR2, saveR, saveR2;
	DWORD		align=0;

	const int nSize = m_nFontSize - 9;
	const int nIncreaseWidth = INCREASE_WIDTH * nSize;
	const int nIncreaseHeight = INCREASE_HEIGHT * nSize;
	

	CString header[5] = {"NO", "종목명", "등락", "변동비중", "시가비중"};
	int	wth[5] = {17, 80, 46, 52, 52};

	for(int i = 0; i < 5; i++)
		wth[i] += nIncreaseWidth;

	font = m_pFont;
	pOldFont = (CFont*)pDC->SelectObject(font);
	
	CRect rc;
	
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc, m_crBack);

	CRect topRc;
	topRc = rc;
	topRc.top += GAP_TOP;
	topRc.left += GAP_SIDE;
	topRc.right -= GAP_SIDE;
	topRc.bottom = TOP_HEIGHT + GAP_TOP + nIncreaseHeight;
	
	pDC->FillSolidRect(&topRc, m_crBackIn);
	DrawRoundBitmap(pDC, topRc, ROUND_PANEL);

	CRect bottomPRc;
	bottomPRc.SetRect(topRc.left, topRc.bottom + GAP_BLOCK, topRc.right, rc.bottom - GAP_BOTTOM);
	pDC->FillSolidRect(&bottomPRc, GetIndexColor(clContents));
	DrawRoundBitmap(pDC, bottomPRc, ROUND_CONTENTS);

	topRc.SetRect(topRc.left + GAP_PANEL, topRc.top, topRc.right - GAP_PANEL, topRc.bottom);
//	pDC->FillSolidRect(&topRc, m_crBackIn);

	const int nTop = topRc.bottom + GAP_BLOCK + GAP_PANEL;
	// 봉
	CRect BongRc;
	BongRc.SetRect(	topRc.left,	nTop, BONG_WIDTH + GAP_SIDE + GAP_PANEL + nIncreaseWidth, bottomPRc.bottom - GAP_PANEL);
	pDC->FillSolidRect(BongRc, GetIndexColor(clTableHBack));
	pDC->Draw3dRect(BongRc, GetIndexColor(clTableLine), GetIndexColor(clTableLine));

	// Draw
	CRect LeftRc, RightRc;
	LeftRc.SetRect(	BongRc.right + 1, nTop, BongRc.right + 1 + TABLE_WIDTH + (5 * nIncreaseWidth),	nTop + HTC + nIncreaseHeight);
	RightRc.SetRect(LeftRc.right, nTop, topRc.right, nTop + HTC + nIncreaseHeight);
	FrameSRect(pDC, LeftRc, GetIndexColor(clTableBuy), GetIndexColor(clTableBuy));		// 상승종목
	FrameSRect(pDC, RightRc, GetIndexColor(clTableSell), GetIndexColor(clTableSell));		// 하락종목

	// 봉
	const int nhalfHeight = (rc.Height() - nTop) / 2;
	m_lowerBound = nTop + nhalfHeight + (int)(atof(CString(m_data.dnyl, 7))) * nhalfHeight / 100;
	m_upperBound = nTop + nhalfHeight - (int)(atof(CString(m_data.upyl, 7))) * nhalfHeight / 100;
	
	if (m_listUpper.GetSize() < 10)
	{
		m_listUpper.Add(m_upperBound);
		m_listLower.Add(m_lowerBound);
	}
	else
	{
		m_listUpper.RemoveAt(0);
		m_listLower.RemoveAt(0);
		m_listUpper.Add(m_upperBound);
		m_listLower.Add(m_lowerBound);
	}

//	TRACE("%d\n", m_listUpper.GetSize);

	if (!m_bMouseClicked) 
	{
		//FrameSRect(&memDC, CRect(5, nTop + map_halfHeight, HTC, nTop + map_halfHeight + (int)(atof(CString(m_data.dnyl, 7))) * map_halfHeight / 100),
		//	RGB(230, 0, 0), RGB(0, 0, 0));	// 하락비중 봉
		//FrameSRect(&memDC, CRect(5, nTop + map_halfHeight - (int)(atof(CString(m_data.upyl, 7))) * map_halfHeight / 100, HTC, nTop + map_halfHeight),
		//	RGB(0, 0, 230), RGB(0, 0, 0));	// 상승비중 봉

		// 봉을 입체감있게 색칠한다
		for (xx = 6; xx <= 32; xx += 3)
		{
			FrameSRect(pDC, CRect(	xx + GAP_PANEL + GAP_SIDE,	nTop + nhalfHeight - (int)(atof(CString(m_data.upyl, 7))) * nhalfHeight / 100 + 1, 
									xx + GAP_PANEL + GAP_SIDE + 4 + nIncreaseWidth, nTop + nhalfHeight), RGB(255 - (xx - 246) * 3, 0, 0), RGB(255 - (xx - 246) * 3, 0, 0));
			FrameSRect(pDC, CRect(	xx + GAP_PANEL + GAP_SIDE, nTop + nhalfHeight, 
									xx + GAP_PANEL + GAP_SIDE + 4 + nIncreaseWidth, nTop + nhalfHeight + (int)(atof(CString(m_data.dnyl, 7))) * nhalfHeight / 100 - GAP_PANEL - GAP_BOTTOM - 1),
									RGB(0, 0, 255 - (xx - 246) * 3), RGB(0, 0, 255 - (xx - 246) * 3));
		}
	}

	m_rcUpRect = CRect(5 + GAP_SIDE, nTop + nhalfHeight - (int)(atof(CString(m_data.upyl, 7))) * nhalfHeight / 100, HTC + GAP_SIDE, nTop + nhalfHeight);
	m_rcDnRect = CRect(5 + GAP_SIDE, nTop + nhalfHeight, HTC + GAP_SIDE, nTop + nhalfHeight + (int)(atof(CString(m_data.dnyl, 7))) * nhalfHeight / 100);

	pDC->SetBkMode(TRANSPARENT);

	for (yy = 0; yy <= 10; yy++)
	{
		const int nRowtop = LeftRc.bottom - 1 + yy * (HGC + nIncreaseHeight);
		const int nRowBottom = LeftRc.bottom - 1 + (yy + 1) * (HGC + nIncreaseHeight);
		rect.SetRect(LeftRc.left, nRowtop, LeftRc.right,	nRowBottom);	// 좌측 테이블 
		rect2.SetRect(RightRc.left,	nRowtop, RightRc.right,	nRowBottom);	// 우측 테이블

		saveR = rect;
		saveR2 = rect2;

		if (yy == 0)
		{			
			pDC->FillSolidRect(rect, GetIndexColor(clHead));
			pDC->FillSolidRect(rect2, GetIndexColor(clHead));
		}
		else
		{
			if (m_data.ucod[yy - 1].giho == '+')
				pDC->FillSolidRect(rect, 
					RGB(255, 245, 245));
			else if (m_data.ucod[yy - 1].giho == '-')
				pDC->FillSolidRect(rect, 
					RGB(245, 245, 255));
			else
				pDC->FillSolidRect(rect, RGB(255, 245, 245));
//				pDC->FillSolidRect(rect, GetIndexColor(75));

			if (m_data.dcod[yy - 1].giho == '+')
				pDC->FillSolidRect(rect2, 
					RGB(255, 245, 245));
			else if (m_data.dcod[yy - 1].giho == '-')
				pDC->FillSolidRect(rect2, 
					RGB(245, 245, 255));
			else
				pDC->FillSolidRect(rect2, RGB(245, 245, 255));
//				pDC->FillSolidRect(rect2, GetIndexColor(75));

		}
		rect.DeflateRect(0, 0, TABLE_WIDTH + 5 * nIncreaseWidth, 0);		// rect 의 가로폭을 0 으로 만듬
		rect2.DeflateRect(0, 0, TABLE_WIDTH + 5 * nIncreaseWidth, 0);
		
		for (xx = 0; xx <= 4; xx++)
		{
			// box line & text
			rect.SetRect(rect.right, rect.top, rect.right + wth[xx], rect.bottom);
			rect2.SetRect(rect2.right, rect2.top, rect2.right + wth[xx], rect2.bottom);
			
			if(yy == 0)
				pDC->SetTextColor(GetIndexColor(clHText));
			else
				pDC->SetTextColor(GetIndexColor(clTableHText));

			align = DT_RIGHT;
			if (yy == 0)
			{
				temp = header[xx];	// temp = 상승, temp2 는 하락 
				temp2 = temp;
				align = DT_CENTER;
			}
			else
			{
				switch(xx)
				{
				case 0:
					temp.Format("%d", yy);
					temp2 = temp;
					align = DT_CENTER;
					break;
				case 1:
					temp = CString(m_data.ucod[yy - 1].cod2, 20);		// 종목명
					temp2 = CString(m_data.dcod[yy - 1].cod2, 20);
					align = DT_LEFT;
					break;
				case 2:
					{
						temp = CString(m_data.ucod[yy - 1].udyl, 7);	// 전일대비 등락
						temp2 = CString(m_data.dcod[yy - 1].udyl, 7);
						temp.TrimRight();
						temp2.TrimRight();
						if (!temp.IsEmpty()) temp = temp.Mid(1);
						if (!temp2.IsEmpty()) temp2 = temp2.Mid(1);
					}
					rect.DeflateRect (0, 0, 3, 0);
					rect2.DeflateRect (0, 0, 3, 0);
					break;
				case 3:
					temp = CString(m_data.ucod[yy - 1].bdyl, 7);		// 변동비중
					temp2 = CString(m_data.dcod[yy - 1].bdyl, 7);
					rect.DeflateRect (0, 0, 3, 0);
					rect2.DeflateRect (0, 0, 3, 0);
					break;
				case 4:
					temp = CString(m_data.ucod[yy - 1].sdyl, 7);		// 시가비중
					temp2 = CString(m_data.dcod[yy - 1].sdyl, 7);
					rect.DeflateRect (0, 0, 3, 0);
					rect2.DeflateRect (0, 0, 3, 0);

					codeS = CString(m_data.ucod[yy - 1].code, sizeof(m_data.ucod[yy - 1].code));		// 상승종목명
					codeS.TrimRight();

					if (m_mapItem.Lookup(codeS, codeR))
					{
						m_mapItem.SetAt(codeS, saveR);
					}

					codeS2 = CString(m_data.dcod[yy - 1].code, sizeof(m_data.dcod[yy - 1].code));		// 하락종목명
					codeS2.TrimRight();

					if (m_mapItem.Lookup(codeS2, codeR2))
					{
						m_mapItem.SetAt(codeS2, saveR2);
					}
					break;
				}
			} 

			temp.TrimRight();	// 투명글자로 만들다
			temp2.TrimRight();
			
			if (xx == 2 && yy != 0)
			{
				for (int ii = strlen(temp); ii < 5; ii++)
					temp = ' ' + temp;
				
				if (m_data.ucod[yy - 1].giho == '+')
				{
					temp = "▲" + temp;
					pDC->SetTextColor(GetIndexColor(clUp));
				}
				else if (m_data.ucod[yy - 1].giho == '-')
				{
					temp = "▼" + temp;
					pDC->SetTextColor(GetIndexColor(clDown));
				}
			}			

			pDC->DrawText(temp, rect, align | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);		// 상승종목 글자 출력

			if (xx == 2 && yy != 0)		// 등락에 부호 추가
			{
				for (int ii = strlen(temp2); ii < 5; ii++)
					temp2 = ' ' + temp2;

				if (m_data.dcod[yy - 1].giho == '+')
				{
					temp2 = "▲" + temp2;
					pDC->SetTextColor(GetIndexColor(clUp));
				}
				else if (m_data.dcod[yy - 1].giho == '-')
				{
					temp2 = "▼" + temp2;
					pDC->SetTextColor(GetIndexColor(clDown));
				}
			}
			pDC->DrawText(temp2, rect2, align | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);	// 하락종목 글자 출력

			if (xx >= 2 && yy != 0)
			{
				rect.DeflateRect(0, 0, -3, 0);
				rect2.DeflateRect(0, 0, -3, 0);
			}
			rect.InflateRect(0, 0, 1, 1);
			rect2.InflateRect(0, 0, 1, 1);

			const COLORREF linecolor = GetIndexColor(clTableLine);
			pDC->Draw3dRect(rect, linecolor, linecolor);//RGB(111, 146, 181), RGB(111, 146, 181));
			pDC->Draw3dRect(rect2, linecolor, linecolor);//RGB(111, 146, 181), RGB(111, 146, 181));

			rect.InflateRect(0, 0, -1, -1);
			rect2.InflateRect(0, 0, -1, -1);
		}
	}
	pDC->SelectObject(pOldFont);

	font = m_pFontB;
	pOldFont = (CFont*)pDC->SelectObject(font);

	CSize sz;
	sz = pDC->GetTextExtent("테스트");

	pDC->SetTextColor(RGB(0, 0, 200));
	temp2.Format("하락종목(변동비중) : %d (%.2f%%)", atoi(CString(m_data.dnum, 7)), atof(CString(m_data.dnyl, 7)));
	pDC->DrawText(temp2, CRect(RightRc.left + GAP_SIDE, RightRc.top, RightRc.right, RightRc.bottom), 
				DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	pDC->SetTextColor(RGB(200, 0, 0));
	temp2.Format("상승종목(변동비중) : %d (%.2f%%)", atoi(CString(m_data.unum, 7)), atof(CString(m_data.upyl, 7)));
	pDC->DrawText(temp2, CRect(LeftRc.left + GAP_SIDE, LeftRc.top, RightRc.right, LeftRc.bottom), 
				DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	pDC->SelectObject(pOldFont);

	font = RequestFont("굴림체", 9, FALSE, FW_BOLD);
	pOldFont = (CFont*)pDC->SelectObject(font);

	if ((int)(atof(CString(m_data.upyl, 7))) != 0)
	{
		temp2.Format("%d", (int)(atof(CString(m_data.upyl, 7))));
		temp2.TrimLeft();
		if (atof(CString(m_data.upyl, 7)) > 15)
		{
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->DrawText(temp2 + "%",
				CRect(	5 + GAP_PANEL + GAP_SIDE, nTop + nhalfHeight - (int)(atof(CString(m_data.upyl, 7))) * nhalfHeight / 100,
						35 + GAP_PANEL + GAP_SIDE + nIncreaseWidth, nTop + nhalfHeight), DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
		else
		{
			pDC->SetTextColor(RGB(255, 0, 0));
			pDC->DrawText(temp2 + "%", 
				CRect(	5 + GAP_PANEL + GAP_SIDE, nTop + nhalfHeight - (int)(atof(CString(m_data.upyl, 7))) * nhalfHeight / 100 - 20,
						35 + GAP_PANEL + GAP_SIDE + nIncreaseWidth, nTop + nhalfHeight - (int)(atof(CString(m_data.upyl, 7))) * nhalfHeight / 100), 
						DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
	}

	if ((int)(atof(CString(m_data.dnyl, 7))) != 0)
	{
		temp2.Format("%d", (int)(atof(CString(m_data.dnyl, 7))));
		temp2.TrimLeft();
		if (atof(CString(m_data.dnyl, 7)) > 15)
		{
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->DrawText(temp2 + "%", 
				CRect(	5 + GAP_PANEL + GAP_SIDE,	nTop + nhalfHeight, 
						35 + GAP_PANEL + GAP_SIDE + nIncreaseWidth, nTop + nhalfHeight + (int)(atof(CString(m_data.dnyl, 7))) * nhalfHeight / 100), 
						DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
		else
		{
			pDC->SetTextColor(RGB(0, 0, 255));
			pDC->DrawText(temp2 + "%", 
				CRect(	5 + GAP_PANEL + GAP_SIDE,	nTop + nhalfHeight + (int)(atof(CString(m_data.dnyl, 7))) * nhalfHeight / 100, 
						35 + GAP_PANEL + GAP_SIDE + nIncreaseWidth,	nTop + nhalfHeight + (int)(atof(CString(m_data.dnyl, 7))) * nhalfHeight / 100 + 20), 
						DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
	}

	CRect GridLrc, GridRrc;
	GridLrc.SetRect(LeftRc.left, LeftRc.top, LeftRc.right, bottomPRc.bottom - GAP_PANEL);
	GridRrc.SetRect(RightRc.left, RightRc.top, RightRc.right, bottomPRc.bottom - GAP_PANEL);
	pDC->Draw3dRect(GridLrc, GetIndexColor(clTableLine), GetIndexColor(clTableLine));
	pDC->Draw3dRect(GridRrc, GetIndexColor(clTableLine), GetIndexColor(clTableLine));

	CPen *sPen = GetPen(GetIndexColor(clTableLine));
	CPen *oldpen;
	oldpen = (CPen*)pDC->SelectObject(sPen);
/*	
	pDC->MoveTo(40, nTop + HTC);
	pDC->LineTo(280, nTop + HTC);
	
//	pDC->MoveTo(40, nTop + 57);
//	pDC->LineTo(280, nTop + 57);

	pDC->MoveTo(280, nTop + HTC);
	pDC->LineTo(520, nTop + HTC);
	
//	pDC->MoveTo(280, nTop + 57);
//	pDC->LineTo(520, nTop + 57);
*/	
	pDC->SelectObject(oldpen);
	pDC->SelectObject(pOldFont);
}

void CMapWnd::drawBitmap()
{
	m_section.Lock();

	CDC	memDC, *pDC = GetDC();
	CBitmap*	pOldBitmap=nullptr;
	
	
	if (m_pBitmap)
	{
//		delete m_pBitmap;
		m_pBitmap = nullptr;
	}

	m_pBitmap = std::make_unique<CBitmap>();
	
	memDC.CreateCompatibleDC(pDC);
	m_pBitmap->CreateCompatibleBitmap(pDC, m_clientRt.Width(), m_clientRt.Height());
	pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap.get());

	Draw(&memDC);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	ReleaseDC(pDC);

	m_section.Unlock();
}

CString CMapWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_strRoot, "tab", "palette.ini");

	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;

	return strPal;
}


void CMapWnd::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
{
	CRect bitRc;
	
	switch(nKind)
	{
	case ROUND_PANEL:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");

		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");

		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");

		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");

		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}

void CMapWnd::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;

	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;

	memDC.CreateCompatibleDC(pDC);

	if (memDC.m_hDC != NULL) {
		memDC.SelectObject(BitMap);
		::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
	}

	DeleteObject(BitMap);
	memDC.DeleteDC();
}

LOGFONT CMapWnd::setFont(int size, bool bold)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = size; 
	
	strcpy(lf.lfFaceName, "굴림체");   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;

	lf.lfWeight = (bold ? FW_BOLD : FW_NORMAL);
	lf.lfItalic = FALSE;
	return lf;
}


void CMapWnd::FrameSRect(CDC *pDC, CRect rt, COLORREF solid, COLORREF frame)
{
	pDC->FillSolidRect(rt, solid);
	pDC->Draw3dRect(rt, frame, frame);
}

COLORREF CMapWnd::GetIndexColor(int index)
{
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index); 
}

CBitmap* CMapWnd::GetAxBitmap(CString path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

void CMapWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_rcDnRect.PtInRect(point) || m_rcUpRect.PtInRect(point))
	{
		m_bMouseClicked = !(m_bMouseClicked);

		//drawBitmap();		Invalidate();
	}

	CString codeS;
	CRect	rect;
	for (POSITION pos = m_mapItem.GetStartPosition(); pos != NULL; )
	{
		m_mapItem.GetNextAssoc(pos, codeS, rect);
		if (rect.PtInRect(point) && !codeS.IsEmpty())
		{
			
			if (codeS.GetAt(0) == 'A' || codeS.GetAt(0) == 'J')
				codeS = codeS.Mid(1);
			TRACE("code[%s]\n", codeS);
			CString triggerS;
			triggerS.Format("%s\t%s", "1301", codeS);
			Variant(triggerCC, triggerS);
			break;
		}
	}
	CWnd::OnLButtonDown(nFlags, point);
}


CString CMapWnd::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

void CMapWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_rcDnRect.PtInRect(point)) {
		m_pPopwnd->SetData(atof(CString(m_data.dnyl, 7)));
		m_pPopwnd->ShowWindow(SW_SHOW);
	}
	else if (m_rcUpRect.PtInRect(point)) {
		m_pPopwnd->SetData(atof(CString(m_data.upyl, 7)));
		m_pPopwnd->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pPopwnd->ShowWindow(SW_HIDE);
	}
	
	m_pPopwnd->MoveWindow(point.x + 30, point.y + 10, 65, 20);
	
	CWnd::OnMouseMove(nFlags, point);
}

CFont* CMapWnd::RequestFont(CString strName, int iPoint, BOOL bItalic, int iBold)
{
	struct	_fontR fontR;

	fontR.name	= (char*)(strName.operator LPCTSTR());
	fontR.point	= iPoint;
	fontR.italic	= (bItalic == TRUE ? true : false);
	fontR.bold	= iBold;

	return (CFont*)m_pParent->SendMessage(WM_USER, (WPARAM)MAKEWORD(getFONT, NULL), (LPARAM)&fontR);
}

CPen* CMapWnd::GetPen(COLORREF color)
{
	_penR	Pen;
	Pen.clr		= color;
	Pen.style	= PS_SOLID;
	Pen.width	= 1;

	return (CPen*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(getPEN, 0), (LPARAM)&Pen);
}

void CMapWnd::OnKospiClick()
{
	SendTR(TRN, "1");
}

void CMapWnd::OnKostarClick()
{
	SendTR(TRN, "2");
}

void CMapWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CPoint pointx, ptclient; GetCursorPos(&pointx);
	ptclient = pointx; ScreenToClient(&ptclient);

	int nRow=0;
	CString code;
	CRect rc1;
	rc1 = CRect(40, borderH + HTC + 1 * HGC, 520, borderH + 45 + 11 * HGC);
	if (rc1.PtInRect(ptclient))
	{
		nRow = (ptclient.y - (borderH + HTC + HGC)) / HGC;
		if (ptclient.x < 280)	code = m_data.ucod[nRow].code;
		else			code = m_data.dcod[nRow].code;
		if (!code.IsEmpty()) code = code.Mid(1);

		m_mouseDat.Format("IB420300\n%d\t%d\n%s\t%s\n", pointx.x, pointx.y, "1301", code);
	}
	else
	{
		m_mouseDat.Format("IB420300\n%d\t%d\n", pointx.x, pointx.y);
	}
	m_pParent->PostMessage(WM_USER, MAKEWPARAM(mouseDLL, 0), (long)m_mouseDat.operator LPCTSTR());
	
	CWnd::OnRButtonDown(nFlags, point);
}

void CMapWnd::makeMapItem()
{
	CString codeS;
	CRect	rect;
	
	rect.SetRectEmpty();
	m_mapItem.RemoveAll();

	for (int ii = 0; ii < sizeof(m_data.ucod)/ sizeof(struct _jcode); ii++)
	{
		// 상승
		codeS = CString(m_data.ucod[ii].code, sizeof(m_data.ucod[ii].code));
		codeS.TrimRight();
		if (!codeS.IsEmpty())
			m_mapItem.SetAt(codeS, rect);

		// 하락
		codeS = CString(m_data.dcod[ii].code, sizeof(m_data.dcod[ii].code));
		codeS.TrimRight();
		if (!codeS.IsEmpty())
			m_mapItem.SetAt(codeS, rect);
	}
}

HBRUSH CMapWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	pDC->SetBkMode(TRANSPARENT);
	return m_br;
//	return hbr;
}

void CMapWnd::SetResize(int nFontSize)
{
	const int nSize = nFontSize - 9;	//font size 9를 100%로 지정하여 8~12까지 표현

	const int nIncreaseWidth = 11 * INCREASE_WIDTH * nSize;
	const int nIncreaseHeight = 13 * INCREASE_HEIGHT * nSize;

	const CRect cWindowRc = MAPWND_RECT;
	const int nWidth = cWindowRc.Width() + nIncreaseWidth;
	const int nHeight = cWindowRc.Height() + nIncreaseHeight;
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(nWidth, nHeight));
	this->MoveWindow(0, 0, nWidth, nHeight);

	CRect btnRc;
	btnRc = BTN_KOSPI_RECT;
	btnRc.right += 10 * nSize;
	btnRc.bottom += nSize;
	m_pKospiBtn->MoveWindow(btnRc);
	m_pKospiBtn->SetFont(m_pFont);

	btnRc = BTN_KOSTAR_RECT;
	btnRc.left += 10 * nSize;
	btnRc.right += 10 * nSize;
	btnRc.bottom += nSize;
	m_pKostarBtn->MoveWindow(btnRc);
	m_pKostarBtn->SetFont(m_pFont);
}

