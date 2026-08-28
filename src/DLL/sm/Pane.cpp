// Pane.cpp : implementation file
//

#include "stdafx.h"
#include "Pane.h"
#include "smdefine.h"
#include "resource.h"
//#include "eticker.h"
#include "NewsHistory.h"
#include "MenuXP.h"
#include "SetTickPopup.h"
#include "../../h/interSt.h"

#include "../../axis/axMsg.hxx"
#include "../../h/axisvar.h"

#include "../../h/interMSG.h"
#include "ioformat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NEWSGAP		1

#define TM_INDEX	7001
#define TM_NEWS		7002
#define TM_UPDOWN	7003
#define TM_INTER	7004
#define TM_NEWNEWS	7005
#define TM_INDEX_TUJA	 7006

#define TMI_INDEX	3000
#define TMI_NEWS	5000
#define TMI_UPDOWN	10000
#define TMI_INTER	3000
#define TMI_INDEX_TUJA	15000

#define ID_TOOLTIP  1

#define AUX_TEXT	1
#define AUX_GRAPH	2

#define COLW		450
#define ROWH		200
#define RMAX		20
#define FGAP		10
// ups and downs color
#define COLOR_GB	RGB(255, 255, 255)
#define COLOR_SH	RGB(245, 137, 154)
#define COLOR_SS	RGB(245, 214, 137)
#define COLOR_BH	RGB(235, 137, 245)
#define COLOR_HL	RGB(137, 240, 245)
#define COLOR_HH	RGB(137, 185, 245)

#define COLOR_BACK	RGB(255, 255, 255)

#define COLOR_UP    59
#define COLOR_DOWN  60
#define COLOR_BOHAP 76
#define COLOR_TEXT  130


#define BASE_UPDOWN	1000
#define BASE_INTER	2000

//#define COLOR_PANETEXT		RGB( 81,  85,  96)
#define COLOR_PANETEXT		RGB( 255,  255,  255)
#define COLOR_PANESHADOW	RGB(252, 253, 255)
#define COLOR_PANEBOX		RGB(181, 181, 181)

#define PANETITLEWIDTH		" DOWJONES"

#define MAX_NEWSCNT			100

#include "../../axis/axcolor.h"

/////////////////////////////////////////////////////////////////////////////
// CPane

CPane::CPane(bool (*axiscall)(int, WPARAM, LPARAM), CBitmap* bkImage)
{
	m_axiscall	= axiscall;
	m_bkIMG		= bkImage;
	m_nTitleCx	= 0;
	
	m_pen.CreatePen(PS_SOLID, 0, COLOR_PANEBOX);
	m_bitmap = Axis::GetBitmap(IDB_TICKLIST);
	m_idx_back = Axis::GetBitmap("TK_INDEX_BACK");
	m_news_back = Axis::GetBitmap("TK_NEWS_BACK");

	m_bNew = false;
	m_bTurn = false;
	m_down = false;
	m_bReset = false;
	m_kind = TKIND_INDEX;
	m_select = 0;

	m_clrAlarm = 0;
	m_bNoAlarm = false;
	m_index = -1;
	m_aux = -1;	
	m_ToolTip = NULL;

	CString	fontName;

	fontName.LoadString(STSM_FONTNAME_DEFAULT);

	m_font.CreatePointFont(90, fontName);

	m_bFont.CreateFont(	   12,                        // nHeight
			   0,                         // nWidth
			   0,                         // nEscapement
			   0,                         // nOrientation
			   FW_SEMIBOLD,	              // nWeight
			   FALSE,                     // bItalic
			   FALSE,                     // bUnderline
			   0,                         // cStrikeOut
			   DEFAULT_CHARSET,              // nCharSet
			   OUT_DEFAULT_PRECIS,        // nOutPrecision
			   CLIP_DEFAULT_PRECIS,       // nClipPrecision
			   DEFAULT_QUALITY,           // nQuality
			   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			   "굴림");                 // lpszFacename


	m_lineColor = COLOR_GB;
// 	const COLORREF bgcolors[5] = {
// 		RGB(249, 60, 76), RGB(243, 187, 81), RGB(182, 228, 104), RGB(105, 222, 229), RGB(106, 158, 242)
// 	};
	const COLORREF bgcolors[5] = {
		RGB(195, 78, 88), RGB(247, 150, 70), RGB(155, 187, 89), RGB(75, 172, 198), RGB(79, 129, 189)
	};
// 	const COLORREF fgcolors[5] = {
// 		RGB(255, 255, 255), RGB(72, 36, 8), RGB(22, 53, 2), RGB(0, 34, 79), RGB(255, 255, 255)
// 	};
	const COLORREF fgcolors[5] = {
		RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255)
	};

	for (int i = 0; i < 5; i++)
	{
		m_bgColor[i] = bgcolors[i];
		m_fgColor[i] = fgcolors[i];
	}


	//m_eTicker = NULL;
	m_newHistory = NULL;
	MakeExpectSymbolTable();

	m_hCursorHand = ::LoadCursor(NULL, MAKEINTRESOURCE(32649));
	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);

	m_bAppendedNews = FALSE;
	
	//m_wButtonTextAlign = DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS|DT_NOPREFIX;
	m_wButtonTextAlign = DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
	LoadImage();
	
}

CPane::~CPane()
{
	if (m_hCursorHand)
		DestroyCursor(m_hCursorHand);
	m_font.DeleteObject();
	m_bFont.DeleteObject();
	m_pen.DeleteObject();

	m_arItem.RemoveAll();
	m_arNewsKey.RemoveAll();
	m_mapData.RemoveAll();
	m_arSymData.RemoveAll();
	m_arIndexbySym.RemoveAll();
	m_arSymbyIndex.RemoveAll();
	
	for (int ii = 0; ii < m_arInter.GetSize(); ii++)
		delete m_arInter.GetAt(ii);

	m_arInter.RemoveAll();
	m_arInterByCode.RemoveAll();
#ifndef DF_USESTL_PANE
	if (m_newHistory)	delete m_newHistory;
#endif
}


BEGIN_MESSAGE_MAP(CPane, CWnd)
	//{{AFX_MSG_MAP(CPane)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_MEASUREITEM()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
	ON_MESSAGE(WM_MSG_MAINTOMAP, OnMsgFromMain)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPane message handlers
void CPane::MakeExpectSymbolTable()
{
	// 20070608
	const char* const sym[] = {"K5001", "K0001", "K2001", "KQSTR", "KQ001", "KQ044", "KQ047"};
	const char* const name[] = {"KRX(예)", "KP(예)", "K200(예)", "QSTR(예)", "KQ(예)", "KQ벤처(예)", "KQ150(예)"};
	const int	ncnt = sizeof(sym)/sizeof(char*);
	
	for (int ii = 0 ; ii < ncnt ; ii++ )
	{
		m_mapExpectSymbol.SetAt(sym[ii], name[ii]);
	}
}

CString CPane::GetExpectSymbolName(CString sym)
{
	CString	ret = _T("");
	if (sym.IsEmpty())
		return _T("");

	m_mapExpectSymbol.Lookup(sym, ret);
	return ret;
}

void CPane::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	cRc;
	GetClientRect(cRc);
	
	CDC	memDC;
	if (!memDC.CreateCompatibleDC(&dc))
	{
		drawBackground(&dc);
		DrawData(&dc);
	}
	else
	{
		CBitmap	bmp, *pbmp;
		bmp.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
		pbmp = (CBitmap *) memDC.SelectObject(&bmp);

		drawBackground(&memDC);
		DrawData(&memDC);

		dc.BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pbmp);
		memDC.DeleteDC();
	}
}

void CPane::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CString		text, tmps, data;
	int		index{};

	const char* szIndexMap = "IB290300";
	const char* szFutureMap = "IB400100";
	const char* szProgramMap = "IB295100";
	const char* szInvestMap = "IB290800";
	const char* szForiegnMap = "";
	const char* szNewsMap = "IBNEWSXX";//**/"IB771000";

	CString indexMap(szIndexMap);

	switch (m_kind)
	{
	case TKIND_INDEX:
		{
			if (point.x < m_nTitleCx)
			{
				if (SetTickPopup())
					return;
			}
		}
		index = m_arItem.GetAt(m_index);

		switch (index)
		{
		default:// kindex
		case 1:	data = indexMap + "20301\t999";	break;
		case 2:	data = indexMap + "20301\t001";	break;
		case 3: data = indexMap + "20301\t101";	break;
		case 4:	data = indexMap + "20301\t305";	break;
		case 5: data = indexMap + "20301\t201";	break;
		case 6:	data = indexMap + "20301\t244";	break;	

		// future index
		case 9:case 10:case 11:case 12:case 33:
			{
				CString symbolS = _T(""), indexS;
				indexS.Format("%d", index);
				if (m_arSymbyIndex.Lookup(indexS, symbolS))
				{
					data.Format("%s30301\t%s", szFutureMap, symbolS);
				}
				else
				{
					data = szFutureMap;
				}
			}
			break;
		// program
		case 13:	data = CString(szProgramMap) + "UPGB\t1";	break;
		case 14:	data = CString(szProgramMap) + "UPGB\t2";	break;			
		// invest
		case 15: case 16:case 17:case 18:case 19: 	
			data = szInvestMap;
			break;
		case 20:	data = "IB781100";					break;
		case 21:	data = "IB781500symb\tCME$ND";		break;
		case 22:	data = "IB781500symb\tCME$NQ";		break;
		case 23:	data = "IB781100";					break;	
		case 24:	data = "IB781500symb\tCME$SP";		break;	
		case 25:	data = "IB781500symb\tCME$ES";		break;
		case 26:	data = "IB781100";					break;	
		case 27:	data = "IB781500symb\tNII@NI225";	break;
		case 28:	data = "IB781500symb\tTWS@TI01";	break;
		case 29:	data = "IB781500symb\tSHS@000002";	break;
		case 30:	data = "IB781500symb\tHSI@HSCI";	break;
		case 35:	data = "IB785100symb\tUSDKRWSMBS";	break;
		case 38:	data = "IB780300symb\tSHS@000003";	break;
		case 39:	data = "IB780300symb\tSZS@399108";	break;
		case 40:	data = "IB780300symb\tINI@BSE30";	break;
		case 41:	data = "IB780300symb\tTWS@TI01";	break;
		case 42:	data = "IB780300symb\tVNI@VIDX";	break;
		case 43:	data = "IB780300symb\tUSI@SOXX";	break;
		case 44:	data = "IB780300symb\tMXI@INMEX";	break;
		case 45:	data = "IB780300symb\tBRI@BVSP";	break;
		case 46:	data = "IB780300symb\tLNS@FTSE100";	break;
		case 47:	data = "IB780300symb\tXTR@DAX30";	break;
		case 48:	data = "IB780300symb\tPAS@CAC40";	break;
		case 49:	data = "IB780300symb\tRUI@RTSI";	break;
		case 50:	data = "IB780300symb\tSHS@000001";	break;
		// fn index
			
			/**
		case 20:	
		case 21:	data = "DH150500symb\tCME$ND";		break;
		case 22:	data = "DH150500symb\tCME$NQ";		break;
		case 23:	data = "DH150100";	break;
		case 24:	data = "DH150500symb\tCME$SP";		break;
		case 25:	data = "DH150500symb\tCME$ES";		break;
		case 26:	data = "DH150100";	break;
		case 27:	data = "DH150500symb\tNII@NI225";	break;
		case 28:	data = "DH150500symb\tTWS@TI01";	break;
		case 29:	data = "DH150500symb\tSHS@000002";	break;
		case 30:	data = "DH150500symb\tHSI@HSCI";	break;
		case 31:	data = "DH150200";			break;
		case 35:	data = "DH150500symb\tUSDKRWSMBS";	break;
		**/
		}
		if (!data.IsEmpty())
			(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(const char *)data);
		break;
	case TKIND_NEWS:
		if (m_arRect.PtInRect(point))
		{
			SetCapture();
			CClientDC	dc(this);
			DrawAr(&dc, true);
		}
		else
		{
		 	//** 하단티커 클릭시에도 뉴스본문팝업 실행하는 것으로 변경함에 따라 수정. 
			data = szNewsMap; 

			if (m_mapData.Lookup(SNEW_TITLE, text))
			{
				tmps.Format("%s\t%s", SNEW_TITLE, text);
				data += tmps;
			}

			if (m_mapData.Lookup(SNEW_JCODE, text))
			{
				if (!text.IsEmpty())
				{
					tmps.Format("\n1%s\t%s", SNEW_JCODE, text);
					data += tmps;
				}
			}

			if (m_mapData.Lookup(SNEW_KEY, text))
			{
				tmps.Format("\n%s\t%s", SNEW_KEY, text);
				data += tmps;
				
				(*m_axiscall)(AXI_CHANGEPOPUP, 0, (LPARAM)(const char *)data);
			}
		}
		NewNews(false);
		Invalidate(FALSE);
		break;
	case TKIND_UPDOWN:
		{
			if (point.x < m_nTitleCx)
			{
				CMenuXP		menu;
				menu.CreatePopupMenu();
				menu.AppendMenuX(MF_STRING, UPDN_KOSPI, "코스피");
				menu.AppendMenuX(MF_STRING, UPDN_KOSDAQ, "코스닥");
				menu.AppendMenuX(MF_STRING, UPDN_KOSPI + UPDN_KOSDAQ, "자동");
				menu.AppendMenuX(MF_SEPARATOR);
				menu.AppendMenuX(MF_STRING, 999, "상세설정");
				
				if (m_arItem.GetSize() == 1)
				{
					if (m_arItem.GetAt(0) == UPDN_KOSPI)
						menu.CheckMenuItem(UPDN_KOSPI, MF_CHECKED|MF_BYCOMMAND);
					if (m_arItem.GetAt(0) == UPDN_KOSDAQ)
						menu.CheckMenuItem(UPDN_KOSDAQ, MF_CHECKED|MF_BYCOMMAND);
				}
				else if (m_arItem.GetSize() == 2)
					menu.CheckMenuItem(UPDN_KOSPI + UPDN_KOSDAQ, MF_CHECKED|MF_BYCOMMAND);
				
				ClientToScreen(&point);
				const int nRtn = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, point.x, point.y, this);
				if (nRtn == 999)
				{
					(*m_axiscall)(AXI_TICKSETUP, 0, 0);
					return;
				}
				//지수티커 변동시 UPDN_KOSPI,UPDN_KOSDAQ 값을 변동된 값으로 변경 필요 
				switch (nRtn)
				{
				case UPDN_KOSPI:
					m_select = 0;
				case UPDN_KOSDAQ:
					m_arItem.RemoveAll();
					m_arItem.Add(nRtn);
					
					if (nRtn == UPDN_KOSDAQ)
						m_select = 1;
					break;
				case UPDN_KOSPI + UPDN_KOSDAQ:
					m_arItem.RemoveAll();
					m_arItem.Add(UPDN_KOSPI);
					m_arItem.Add(UPDN_KOSDAQ);
					m_select = 2;
					break;
				}
				
				save();
				ReSetPaneInfo();
				// 전체 재조회.. ㅜㅜ;
				(*m_axiscall)(AXI_SENDTICKINFO, 0, 0);
				break;
			}
		}
		data = GetdataUPDN(point);
		//MessageBox(data);
		(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(const char *)data);
		break;
	case TKIND_INTER:
		{
			if (m_index < 0 || m_index >= m_arInter.GetSize())
				break;
			const _inter* inter = m_arInter.GetAt(m_index);
			data.Format("IB2022001301\t%s", inter->Code.Mid(1));
			(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(const char *)data);
		}
		break;
		
	}
	//OutputDebugString("PANE MOUSE DOWN\n");
	CWnd::OnLButtonDown(nFlags, point);
}

void CPane::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_kind == TKIND_NEWS)
	{
		ReleaseCapture();
		if (m_arRect.PtInRect(point))
		{
			if (!m_newHistory)
			{

#ifdef DF_USESTL_PANE
				m_newHistory = std::make_unique<CNewsHistory>(m_axiscall);
#else
				m_newHistory = new CNewsHistory(m_axiscall);
#endif
				
				if (m_newHistory->Create(IDD_NEWHISTORY))
				{
					m_newHistory->SetData(m_arHistoryNews);
					CRect	pRc, wRc;
					GetWindowRect(pRc);
					wRc.SetRect(pRc.left, pRc.top - ROWH, pRc.right, pRc.top);
					if (wRc.top < 0)	wRc.OffsetRect(0, abs(wRc.top));

					wRc.left = wRc.right - COLW;

					if(pRc.left < wRc.left)
					{
						wRc.left = pRc.left;
						wRc.right = wRc.left + COLW;
					}

					m_newHistory->SetWindowPos(&wndTopMost, wRc.left, wRc.top, wRc.Width(), wRc.Height(), SWP_SHOWWINDOW);

					//OutputDebugString("CREATE NEWSHISTORY\n");
				}
				else
				{
#ifndef DF_USESTL_PANE
					delete m_newHistory;	
#endif	
					m_newHistory = NULL;
					//OutputDebugString("DELETE NEWSHISTORY\n");
				}
			}
			else
			{
				m_newHistory->SetFocus();
				m_newHistory->SetData(m_arHistoryNews);
				CRect	pRc, wRc;
				GetWindowRect(pRc);
				wRc.SetRect(pRc.left, pRc.top - ROWH, pRc.right, pRc.top);
				if (wRc.top < 0)	wRc.OffsetRect(0, abs(wRc.top));
				wRc.left = wRc.right - COLW;

				if(pRc.left < wRc.left)
				{
					wRc.left = pRc.left;
					wRc.right = wRc.left + COLW;
				}

				m_newHistory->SetWindowPos(&wndTopMost, wRc.left, wRc.top, wRc.Width(), wRc.Height(), SWP_SHOWWINDOW);

				//OutputDebugString("SET NEWSHISTORY\n");
			}
		}
		InvalidateRect(m_arRect);
	}
	CWnd::OnLButtonUp(nFlags, point);
}


void CPane::OnTimer(UINT nIDEvent) 
{
	CString slog;
	switch (nIDEvent)
	{
	case TM_INDEX_TUJA:
	{
		HWND m_hMain = AfxGetMainWnd()->m_hWnd;
		::SendMessage(m_hMain, WM_INTERMSG, MAKEWPARAM(MMSG_TIME_INVESTTUJA, 0), 0);
	}
	break;
	case TM_INDEX:
	case TM_UPDOWN:
		m_index++;

		if (m_index >= m_arItem.GetSize())
			m_index = 0;
		if (m_index >= 0 && m_index < m_arSym.GetSize())
		{
			CString	sym, dat, keys;

			keys.Format("%d", m_arItem.GetAt(m_index));
			m_arSymbyIndex.Lookup(keys, sym);
			m_arSymData.Lookup(sym, dat);
			SetData(dat, TRUE);
		}
		else	SetData("");
		Invalidate();
		break;
	case TM_INTER:
		m_index++;
		if (m_index >= m_arInter.GetSize())
			m_index = 0;

		Invalidate();
		break;
	case TM_NEWNEWS:
		{
			if (m_bNew)
			{
				if (m_bTurn)
					m_nCount--;

				m_bTurn = (m_bTurn) ? false : true;
				
				if (!m_nCount)
					NewNews(false);
				
				Invalidate(FALSE);				
			}
		}
		break;
	default:
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

void CPane::drawBackground(CDC* pDC)
{
	CDC	 memDC;
	CRect	 cRc;

	GetClientRect(cRc);
	if (m_bkIMG && m_bkIMG->GetSafeHandle())
	{
		if (memDC.CreateCompatibleDC(pDC))
		{
			CBitmap* oldbuffBitmap = memDC.SelectObject(m_bkIMG);
			if (oldbuffBitmap != (CBitmap *) 0)
			{
				BITMAP	 bm;
				m_bkIMG->GetBitmap(&bm);
				//pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), 
				//		&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
				
				pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), 
							&memDC, 0, HORZFIT_TOP, bm.bmWidth,
							bm.bmHeight-(HORZFIT_TOP+HORZFIT_BOTTOM), SRCCOPY);
				memDC.SelectObject(oldbuffBitmap);
			}
			memDC.DeleteDC();
		}
	}
	else
	{
		pDC->SetBkColor(COLOR_BACK);
		pDC->ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);
	}
}

void CPane::DrawData(CDC* pDC/*= NULL*/)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;
	if (m_kind != TKIND_NEWS) drawDataBG(pDC, 0);
	else drawDataBG(pDC, 1);

	switch (m_kind)
	{
	case TKIND_INDEX:	Draw_Index(pDC);	break;
	case TKIND_NEWS:	Draw_News(pDC);		break;
	case TKIND_UPDOWN:	Draw_Updn(pDC);		break;
	case TKIND_INTER:	Draw_Inters(pDC);	break;
	default:return;
	}
}

void CPane::DrawAr(CDC* pDC, bool bDown)
{
	if (!m_bitmap || !m_bitmap->m_hObject)
		return;
	
	CRect	cRc;
	BITMAP	bm;

	GetClientRect(cRc);
	m_bitmap->GetBitmap(&bm);

	cRc.left = 0;//cRc.right - bm.bmWidth / 3;
	cRc.top    = 0;//(cRc.Height() - bm.bmHeight) / 2;
	cRc.bottom = cRc.top + bm.bmHeight;
	cRc.right  = bm.bmWidth/3;
	m_arRect.CopyRect(&cRc);

	CDC	memDC;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap* bitmap = memDC.SelectObject(m_bitmap);
		pDC->BitBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), &memDC, 
			bDown ? bm.bmWidth / 3 * 2: 0, 0, SRCCOPY);
		memDC.SelectObject(bitmap);
		memDC.DeleteDC();
	}
}

void CPane::Draw_Index(CDC* pDC)
{
	if (m_index < 0 ||m_index >= m_arItem.GetSize())//m_mapData.GetCount() <= 0)
		return;

	CClientDC	dc(this);

	if (!pDC)	
		pDC = &dc;
	
	pDC->SetBkMode(TRANSPARENT);

	const int	key = m_arItem.GetAt(m_index);

	const int index = GetIndexByKey(key);

	if (index == -1) return;

	switch (key)
	{
	default:
	case 1:case 2:case 3:case 4:case 5:case 6:	// kindex
	case 9:case 10:case 11:case 12:case 32:case 33:			// future index
		Draw_KIndex(pDC, index);
		break;
	case 13:case 14:				// program
		Draw_Prog(pDC, index);
		break;
	case 15:case 16:case 17:case 18:case 19:	// invest
		Draw_Invest(pDC, index);
		break;
 	}
}

void CPane::Draw_KIndex(CDC* pDC, int key)
{
	CRect	cRc, tRc;
	CString	text, keys, value, sym, tmps;
	CFont*	font = pDC->SelectObject(&m_font);
	int	cx = m_nTitleCx = GetButtonWidth(pDC);

	DrawButton(pDC, CRect(0, 0, cx, m_szButton.cy));
	BOOL	bExpect = FALSE;
	//TRACE("Index: %s",m_mapData);
	if (m_mapData.Lookup("600", text))
	{
		if (atoi(text) == 0)		// 0:예상, 1:장중
		{
			if(!text.IsEmpty())
				bExpect = TRUE;
		}
	}

	if (bExpect)
	{
		keys.Format("%d", key);
		m_arSymbyIndex.Lookup(keys, sym);
		text.Format("%s", GetExpectSymbolName(sym));
		if (text.IsEmpty())
		{	
			CString tmp = GetIndexSName(key);
			tmp.TrimLeft(); tmp.TrimRight();
			if (tmp == "KOSPI200")
			{
				tmp = "K200";
			}
			else if (tmp == "KOSDAQ")
			{
				tmp = "KQ";
			}

			text.Format("%s(예)", tmp);
			
		}
	}
	else
		text.Format("%s", GetIndexSName(key));
	text.Replace("&&", "&");
	GetClientRect(cRc);

	tRc.SetRect(cRc.left+2, cRc.top, cRc.left + cx, cRc.bottom);
	
	tRc.OffsetRect(0, 2);

	/** 티커 종목명 표시 **/
	COLORREF color = pDC->SetTextColor(COLOR_PANESHADOW);
	//pDC->DrawText(text, tRc, m_wButtonTextAlign);///**DT_LEFT**/DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	tRc.OffsetRect(-1, -1);
	
	pDC->SetTextColor(COLOR_PANETEXT);
	pDC->SelectObject(&m_bFont);
	pDC->DrawText(text, tRc, m_wButtonTextAlign);///**DT_LEFT**/DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	pDC->SelectObject(&m_font);
	if (key==28) // BASIS 라면 따로 드로잉 처리
	{
		if (m_mapData.Lookup(BASIS, value))	// BASIS
		{
			switch (GetSign(value))
			{
			case '+':	pDC->SetTextColor(GetColor(AxColor::up));	break;//COLOR_UP));//
			case '-':	pDC->SetTextColor(GetColor(AxColor::down));	break;//COLOR_DOWN));//
			default:	pDC->SetTextColor(GetColor(AxColor::bohap));	break;
			}
			
			text.Format("%sB", commaE(value, false));
			cx = pDC->GetTextExtent(" +99.99%").cx;
			tRc.left = tRc.right;
			tRc.right = tRc.left + cx;
			
			
			pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

			color = pDC->SetTextColor(GetColor(97));
			text.Format("%s", " [선]");
			cx = pDC->GetTextExtent(" [선]").cx;
			tRc.left = tRc.right;
			tRc.right = tRc.left + cx;
			pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

			if (m_mapData.Lookup(S_CUR, value))	// 현재가
			{
				switch (GetSign(value))
				{
				case '+':
					color = pDC->SetTextColor(GetColor(AxColor::up));//COLOR_UP));//
					sym.LoadString(ST_SYMBOL_UP2);
					break;
				case '-':
					color = pDC->SetTextColor(GetColor(AxColor::down));//COLOR_DOWN));//
					sym.LoadString(ST_SYMBOL_DOWN2);
					break;
				default:
					color = pDC->SetTextColor(GetColor(AxColor::bohap));//COLOR_BOHAP));
					break;
				}
				
				text.Format("%s", commaE(value));
				cx = pDC->GetTextExtent("99,999.99").cx;
				tRc.left = tRc.right;
				tRc.right = tRc.left + cx;
				
				pDC->DrawText(text, tRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			}
			else
			{
				tRc.left = tRc.right;
				tRc.right = cRc.right;
				
				//		pDC->SetTextColor(GetColor(AxColor::up));
				//		pDC->DrawText(" No match symbol........", tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
				//		pDC->SetTextColor(color);
				pDC->SelectObject(font);
				return;
			}
			
			color = pDC->SetTextColor(GetColor(97));
			text.Format("%s", "[현]");
			cx = pDC->GetTextExtent("[현]").cx;
			tRc.left = tRc.right;
			tRc.right = tRc.left + cx;
			pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

			if (m_mapData.Lookup(S_KP200, value))	// 현재가
			{
				switch (GetSign(value))
				{
				case '+':
					color = pDC->SetTextColor(GetColor(AxColor::up));//COLOR_UP));//
					sym.LoadString(ST_SYMBOL_UP2);
					break;
				case '-':
					color = pDC->SetTextColor(GetColor(AxColor::down));//COLOR_DOWN));//
					sym.LoadString(ST_SYMBOL_DOWN2);
					break;
				default:
					color = pDC->SetTextColor(GetColor(AxColor::bohap));//COLOR_BOHAP));
					break;
				}
				
				text.Format("%s", commaE(value));
				cx = pDC->GetTextExtent("99,999.99").cx;
				tRc.left = tRc.right;
				tRc.right = tRc.left + cx;
				
				pDC->DrawText(text, tRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			}
			else
			{
				tRc.left = tRc.right;
				tRc.right = cRc.right;
				
				//		pDC->SetTextColor(GetColor(AxColor::up));
				//		pDC->DrawText(" No match symbol........", tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
				//		pDC->SetTextColor(color);
				pDC->SelectObject(font);
				return;
			}
			return;
		}
	}

	if (m_mapData.Lookup(S_CUR, value))	// 현재가
	{
		switch (GetSign(value))
		{
		case '+':
			color = pDC->SetTextColor(GetColor(AxColor::up));//COLOR_UP));//
			sym.LoadString(ST_SYMBOL_UP2);
			break;
		case '-':
			color = pDC->SetTextColor(GetColor(AxColor::down));//COLOR_DOWN));//
			sym.LoadString(ST_SYMBOL_DOWN2);
			break;
		default:
			color = pDC->SetTextColor(GetColor(AxColor::bohap));//COLOR_BOHAP));
			break;
		}
		
		text.Format("%s", commaE(value));
		cx = pDC->GetTextExtent("99,999.99").cx;
		tRc.left = tRc.right;
		tRc.right = tRc.left + cx;

		pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	}
	else
	{
		tRc.left = tRc.right;
		tRc.right = cRc.right;

//		pDC->SetTextColor(GetColor(AxColor::up));
//		pDC->DrawText(" No match symbol........", tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
//		pDC->SetTextColor(color);
		pDC->SelectObject(font);
		return;
	}

	if (m_mapData.Lookup(S_DB, value))	// 대비
	{
		text.Format("%s", getDBSign(value));
		cx = pDC->GetTextExtent("  ▲").cx;
		tRc.left = tRc.right;
		tRc.right = tRc.left + cx;

		pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);

		cx = pDC->GetTextExtent(" 9999.99").cx;
		tRc.left = tRc.right;
		tRc.right = tRc.left + cx;

		pDC->DrawText(commaE(value), tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
	}

	switch (key)
	{
	case 9:case 10:case 11:case 12://case 28:
		if (m_mapData.Lookup(BASIS, value))	// BASIS
		{
			switch (GetSign(value))
			{
			case '+':	pDC->SetTextColor(GetColor(AxColor::up));	break;//COLOR_UP));//
			case '-':	pDC->SetTextColor(GetColor(AxColor::down));	break;//COLOR_DOWN));//
			default:	pDC->SetTextColor(GetColor(AxColor::bohap));	break;
			}

			text.Format("%sB", commaE(value, false));
			cx = pDC->GetTextExtent(" +99.99%").cx;
			tRc.left = tRc.right;
			tRc.right = tRc.left + cx;

			
			pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		}
		break;
	default:
		if (m_mapData.Lookup(S_RATE, value))	// 등락률
		{
			text.Format("%s%%", value);
			cx = pDC->GetTextExtent(" +99.99%").cx;
			tRc.left = tRc.right;
			tRc.right = tRc.left + cx;

			pDC->DrawText(text, tRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		}
		break;
	}

	tRc.right += 4;

// 	CString s;
// 	s.Format("AUX [%d]\n",m_aux);
// 	OutputDebugString(s);
	if (m_aux == 1 || m_aux == 2)
	{
		CString sSym;

		if(m_aux == 1)
			sSym = S_NVOL;
		else if(m_aux == 2)
			sSym = S_PVOL;

// 		s.Format("AUX SYM [%s]\n",sSym);
// 		OutputDebugString(s);

		if (m_mapData.Lookup(sSym, value))	// 거래량
		{
// 			s.Format("SYM VALUE [%s]\n",value);
// 			OutputDebugString(s);
			if(value != "0")
			{

				text.Format("%s", commaE(value));
				cx = pDC->GetTextExtent(text).cx;
				tRc.left = tRc.right;
				tRc.right = tRc.left + cx;

				pDC->SetTextColor(GetColor(COLOR_TEXT));
				pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			}
		}
	}
	else
	{
		for (int ii = 0; ii < m_arSign[key].GetSize(); ii++)
		{
			switch (atoi(m_arSign[key].GetAt(ii)))
			{
			case 0:
				text = S_NONE;
				color = pDC->SetTextColor(GetColor(AxColor::bohap));
				break;
			case 1:
				text = S_SKofDay;
				pDC->SetTextColor(GetColor(AxColor::up));//COLOR_UP));//
				break;
			case 2:
				text = S_SJofDay;
				color = pDC->SetTextColor(GetColor(AxColor::down));//COLOR_DOWN);//
				break;
			case 3:
				pDC->SetTextColor(GetColor(AxColor::up));//COLOR_UP));//
				text = S_SSofPrev;
				break;
			case 4:
				text = S_HLofPrev;
				color = pDC->SetTextColor(GetColor(AxColor::down));//COLOR_DOWN));//
				break;
			default:
				continue;
			}

			cx = pDC->GetTextExtent(text).cx;
			tRc.left = tRc.right + 1;
			tRc.right = tRc.left + cx;
			pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
	}

	pDC->SetTextColor(color);
	pDC->SelectObject(font);
}

void CPane::Draw_Prog(CDC* pDC, int key)
{
	CRect	cRc, tRc;
	CString	text, keys, value, sym, kindS;
	CFont*	font = pDC->SelectObject(&m_font);
	int	cx = m_nTitleCx = GetButtonWidth(pDC);
	DrawButton(pDC, CRect(0, 0, cx, m_szButton.cy));

	text.Format("%s", GetIndexSName(key));

	GetClientRect(cRc);
	tRc.SetRect(cRc.left, cRc.top, cRc.left + cx, cRc.bottom);
	tRc.OffsetRect(0, 2);

	COLORREF color = pDC->SetTextColor(COLOR_PANESHADOW);
	//pDC->DrawText(text, tRc, m_wButtonTextAlign);//DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	tRc.OffsetRect(-1, -1);
	pDC->SetTextColor(COLOR_PANETEXT);
	pDC->SelectObject(&m_bFont);
	pDC->DrawText(text, tRc, m_wButtonTextAlign);//DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	pDC->SelectObject(&m_font);
/*	cRc.left = tRc.right;
	pDC->Draw3dRect(cRc, COLOR_PANEBOX, COLOR_PANEBOX);
	cRc.DeflateRect(1, 1);
	pDC->FillSolidRect(cRc, COLOR_BACK);
*/
	CStringArray	ary;
	if (false)	// vol
	{
		ary.Add(PROG_VOLALL);
		ary.Add(PROG_VOLCHA);
		ary.Add(PROG_VOLBI);
	}
	else		// amt
	{
		ary.Add(PROG_AMTALL);
		ary.Add(PROG_AMTCHA);
		ary.Add(PROG_AMTBI);
	}

	for (int ii = 0; ii < ary.GetSize(); ii++)
	{
		sym = ary.GetAt(ii);
		switch (ii)
		{
		case 0:	kindS = _T("[전]");	break;
		case 1: kindS = _T("[차]");	break;
		case 2: kindS = _T("[비]");	break;
		}

		color = pDC->SetTextColor(GetColor(COLOR_TEXT));
		text.Format("%s", kindS);
		cx = pDC->GetTextExtent(text).cx;
		tRc.left = tRc.right + FGAP/2;
		tRc.right = tRc.left + cx;

		pDC->DrawText(text, tRc, m_wButtonTextAlign);//DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		if (m_mapData.Lookup(sym, value))
		{
			switch (GetSign(value))
			{
			case '+':
				color = pDC->SetTextColor(GetColor(AxColor::up));//COLOR_UP));//
				sym.LoadString(ST_SYMBOL_UP2);
				break;
			case '-':
				color = pDC->SetTextColor(GetColor(AxColor::down));//COLOR_DOWN));//
				sym.LoadString(ST_SYMBOL_DOWN2);
				break;
			default:
				color = pDC->SetTextColor(GetColor(AxColor::bohap));
				break;
			}
			
			text.Format("%s", commaE(value));
			cx = pDC->GetTextExtent(text).cx;
			tRc.left = tRc.right + FGAP/2;
			tRc.right = tRc.left + cx;

			pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
		else
		{
			text.Format(" %s .... ", kindS);
			tRc.left = tRc.right + FGAP/2;
			cx = pDC->GetTextExtent(text).cx;
			tRc.right = tRc.left + cx;

			pDC->SetTextColor(GetColor(AxColor::bohap));//));
			pDC->DrawText(text, tRc, m_wButtonTextAlign);//DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
	}

	pDC->SetTextColor(color);
	pDC->SelectObject(font);
}

void CPane::Draw_Invest(CDC* pDC, int key)
{
	CRect	cRc, tRc;
	CString	text, keys, value, sym, dat, kindS;
	CFont*	font = pDC->SelectObject(&m_font);
	int	cx = m_nTitleCx = GetButtonWidth(pDC);
	DrawButton(pDC, CRect(0, 0, cx, m_szButton.cy));

	text.Format("%s", GetIndexSName(key));

	GetClientRect(cRc);
	tRc.SetRect(cRc.left, cRc.top, cRc.left + cx, cRc.bottom);
	tRc.OffsetRect(0, 2);

	COLORREF color = pDC->SetTextColor(COLOR_PANESHADOW);
	//pDC->DrawText(text, tRc, m_wButtonTextAlign);//DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	tRc.OffsetRect(-1, -1);
	pDC->SetTextColor(COLOR_PANETEXT);
	pDC->SelectObject(&m_bFont);
	pDC->DrawText(text, tRc, m_wButtonTextAlign);//DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	pDC->SelectObject(&m_font);
/*	cRc.left = tRc.right;
	pDC->Draw3dRect(cRc, COLOR_PANEBOX, COLOR_PANEBOX);
	cRc.DeflateRect(1, 1);
	pDC->FillSolidRect(cRc, COLOR_BACK);
*/
	CString sExpSym;

	sExpSym = INVEST_MSAMOUNT;

	CStringArray	ary;
	switch (GetTicKey(key))
	{
	case 15:	// KOSPI
		ary.Add("TJ01090001");	// 외극인
		ary.Add("TJ01000001");	// 기관
		ary.Add("TJ01080001");	// 개인
		break;
	case 16:	// KOSDAQ
		ary.Add("TJ01099001");	// 외극인
		ary.Add("TJ01009001");	// 기관
		ary.Add("TJ01089001");	// 개인
		break;
	case 17:	// FUTURE
		ary.Add("TJ02090000");	// 외극인
		ary.Add("TJ02000000");	// 기관
		ary.Add("TJ02080000");	// 개인
		sExpSym = INVEST_MSVOL;
		break;
	case 18:	// CALL OPTION
		ary.Add("TJ05090000");	// 외극인
		ary.Add("TJ05000000");	// 기관
		ary.Add("TJ05080000");	// 개인
		break;
	case 19:	// PUT OPTION
		ary.Add("TJ06090000");	// 외극인
		ary.Add("TJ06000000");	// 기관
		ary.Add("TJ06080000");	// 개인
		break;
	default:
		pDC->SetTextColor(color);
		pDC->SelectObject(font);
		return;
	}

	CMapStringToString	fms;
	for (int ii = 0; ii < ary.GetSize(); ii++)
	{
		sym = ary.GetAt(ii);
		if (!m_arSymData.Lookup(sym, dat))
			continue;

		parseDat(dat, fms);
		switch (ii)
		{
		case 0: kindS = _T("[외]");	break;
		case 1: kindS = _T("[기]");	break;
		case 2:	kindS = _T("[개]");	break;
		}

		color = pDC->SetTextColor(GetColor(COLOR_TEXT));////GetColor(COLOR_BOHAP));
		text.Format("%s", kindS);
		cx = pDC->GetTextExtent(text).cx;
		tRc.left = tRc.right + FGAP/2;
		tRc.right = tRc.left + cx;

		pDC->DrawText(text, tRc,m_wButtonTextAlign);//_DT_LEFT|DT_VCENTER|DT_SINGLELINE);

		if (fms.Lookup(sExpSym/*INVEST_MSVOL*/, dat))
		{
			switch (GetSign(dat))
			{
			case '+':
				color = pDC->SetTextColor(GetColor(AxColor::up));//COLOR_UP));//
				sym.LoadString(ST_SYMBOL_UP2);
				break;
			case '-':
				color = pDC->SetTextColor(GetColor(AxColor::down));//COLOR_DOWN));//
				sym.LoadString(ST_SYMBOL_DOWN2);
				break;
			default:
				color = pDC->SetTextColor(GetColor(AxColor::bohap));
				break;
			}
			
			text.Format("%s", commaE(dat));
			cx = pDC->GetTextExtent(text).cx;
			tRc.left = tRc.right + FGAP/2;
			tRc.right = tRc.left + cx;

			pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}
		else
		{
			tRc.left = tRc.right;
			tRc.right = cRc.right;

			pDC->SetTextColor(GetColor(AxColor::up));
			text.Format(" %s .... ", kindS);
			pDC->DrawText(text, tRc, m_wButtonTextAlign);//DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		}

		fms.RemoveAll();
	}

	pDC->SetTextColor(color);
	pDC->SelectObject(font);
}


void CPane::Draw_News(CDC* pDC)
{
	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;

	CString	text;
	CRect	cRc;

	GetClientRect(cRc);
	pDC->SetBkMode(TRANSPARENT);

	if (m_mapData.Lookup(SNEW_TITLE, text))
	{
		BITMAP	bm;

		m_bitmap->GetBitmap(&bm);
		cRc.left += bm.bmWidth/3;
		//cRc.right -= bm.bmWidth/2;
		CFont*	font = pDC->SelectObject(&m_font);
		if (!m_bNoAlarm && m_bNew && m_bTurn)
		{
			const CSize		size = GetStringSize(pDC, text + " ");
			const COLORREF	clr = pDC->SetTextColor(GetReverseColor(m_clrAlarm));
			CRect	rect = cRc;
			rect.right = rect.left + size.cx;
			pDC->FillSolidRect(rect, m_clrAlarm);
			pDC->DrawText(" " + text, cRc, m_wButtonTextAlign);
			pDC->SetTextColor(clr);
		}
		else
			pDC->DrawText(" " + text, cRc, m_wButtonTextAlign);
		pDC->SelectObject(font);

		DrawAr(pDC);
	}
}

void CPane::Draw_Updn(CDC* pDC)
{
	if (m_index < 0)
		return;

	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;
	pDC->SetBkMode(TRANSPARENT);

	int		width{}, cx{};
	CRect		cRc, dRc[RMAX];
	CString		text;
	COLORREF	color{};

	GetClientRect(cRc);
	CPen*  pen  = pDC->SelectObject(&m_pen);
	CFont* font = pDC->SelectObject(&m_font);
	cx = m_nTitleCx = GetButtonWidth(pDC);
	DrawButton(pDC, CRect(0, 0, cx, m_szButton.cy));
	//티커 지수 변경시 KOSPI,KOSDAQ 앞에 인덱스가 추가되면 위치가 바뀌기 때문에 UPDN_KOSPI,UPDN_KOSDAQ의 값이 변경되나 KOSPI는 7,KOSDAQ은 8로 서버에 고정되어 있어 이에 맞게 수정 필요
	const int key = m_arItem.GetAt(m_index) + 1;
	//int key = m_arItem.GetAt(m_index);
	
	text.Format("%s", GetIndexSName(key));

// 	CString s;
// 	s.Format("M_INDEX [%d] KEY [%d] TITLE [%s]\n",m_index,key,text);
// 	OutputDebugString(s);

	const int gap = 3;
	switch (m_aux)
	{
	case AUX_TEXT:
		{
			/**
			for (int ii = 5, idx = 0; ii >= 0; ii--, idx++)
			{
				if (!ii)
					dRc[ii].SetRect(cRc.left + gap, cRc.top, dRc[ii+1].left + 1, cRc.bottom);
				else
					dRc[ii].SetRect(cRc.right - width*(idx+1) + gap, cRc.top,
							cRc.right - width*idx + 1, cRc.bottom);
			}
			**/
			dRc[0].SetRect(cRc.left + gap, cRc.top, cRc.left + gap + cx, cRc.bottom);
			//cRc.top += 1;
			//cRc.bottom -= 1;
			width = (cRc.Width() - cx - gap) / 5;
			for (int ii = 1; ii <= 5; ii++)
			{
				dRc[ii].SetRect(cRc.left + gap + cx + (ii-1) * width, cRc.top, 
								cRc.left + gap + cx + (ii-1) * width + width, cRc.bottom);
			}

			dRc[0].OffsetRect(-gap, 2);
			color = pDC->SetTextColor(COLOR_PANESHADOW);
			//pDC->DrawText(text, dRc[0], m_wButtonTextAlign);//DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			dRc[0].OffsetRect(-1, -1);
			pDC->SetTextColor(COLOR_PANETEXT);
			pDC->SelectObject(&m_bFont);
			pDC->DrawText(text, dRc[0], m_wButtonTextAlign);//DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			pDC->SelectObject(&m_font);
			text.Empty();
			pDC->SetTextColor(GetColor(23));
			const UINT nFlag = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			dRc[1].OffsetRect(-3, 0);
			dRc[1].right += 3;
			dRc[5].right = cRc.right;
			for (int ii = 1; ii <= 5; ii++)
			{
				//pDC->Rectangle(dRc[ii]); //테두리 처리 안함.	
				
				//dRc[ii].DeflateRect(1, 1);
				pDC->FillSolidRect(dRc[ii], m_bgColor[ii - 1]);
				//dRc[ii].InflateRect(1, 1);	
				dRc[ii].OffsetRect(0, 1);
				
				pDC->SetTextColor(m_fgColor[ii - 1]);
				switch (ii)
				{
				case 1:	
					m_mapData.Lookup(S_SH, text);
					break;
				case 2:	
					m_mapData.Lookup(S_SS, text);
					break;
				case 3:	
					m_mapData.Lookup(S_BH, text);
					break;
				case 4:	
					m_mapData.Lookup(S_HL, text);
					break;
				case 5:	
					m_mapData.Lookup(S_HH, text);
					break;
				}
				
				pDC->DrawText(text, dRc[ii], nFlag);
				text.Empty();
			}
			pDC->SetTextColor(color);
		}
		break;
	case AUX_GRAPH:
		{
			if (m_mapData.GetCount() <= 0)
				break;
			
			int	total = 0, value{}, w{};
			CArray < int, int > ary, aryW;

			dRc[0].SetRect(cRc.left, cRc.top, cRc.left + cx + 1, cRc.bottom);
			dRc[0].OffsetRect(0, 2);
			color = pDC->SetTextColor(COLOR_PANESHADOW);
			//pDC->DrawText(text, dRc[0], m_wButtonTextAlign);//DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			dRc[0].OffsetRect(-1, -1);
			pDC->SetTextColor(COLOR_PANETEXT);
			pDC->SelectObject(&m_bFont);
			pDC->DrawText(text, dRc[0], m_wButtonTextAlign);//DT_LEFT|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			pDC->SelectObject(&m_font);
			if (!m_mapData.Lookup(S_SH, text))
			{
				pDC->SetTextColor(color);
				break;
			}

			value = atoi(text);	ary.Add(value);	total += value;
			m_mapData.Lookup(S_SS, text);	value = atoi(text);	ary.Add(value);	total += value;
			m_mapData.Lookup(S_BH, text);	value = atoi(text);	ary.Add(value);	total += value;
			m_mapData.Lookup(S_HL, text);	value = atoi(text);	ary.Add(value);	total += value;
			m_mapData.Lookup(S_HH, text);	value = atoi(text);	ary.Add(value);	total += value;

			width = cRc.Width() - cx;
			for (int ii = ary.GetSize() - 1; ii >= 0; ii--)
			{
				value = ary.GetAt(ii);
				if (value)
				{
					w = (value*width)/total;
					if (!w)	w = 1;	// fit

				}
				else	w = 0;
				aryW.InsertAt(0, w);
			}

			total = 0;
			for (int ii = ary.GetSize(); ii >= 0; ii--)
			{
				if (ii)
				{
					value = aryW.GetAt(ii-1);
					if (!value)
					{
						if (ii == ary.GetSize())
						{
							dRc[ii].SetRect(cRc.right, cRc.top,
								cRc.right, cRc.bottom);
						}
						else
						{
							dRc[ii].SetRect(dRc[ii+1].left, cRc.top,
								dRc[ii+1].left, cRc.bottom);
						}
					}
					else
					{
						dRc[ii].SetRect(cRc.right - total - value, cRc.top,
								cRc.right - total +1, cRc.bottom);
					}
					total += value;
				}
				else
					dRc[ii].SetRect(cRc.left, cRc.top, dRc[ii+1].left, cRc.bottom);
			}

			for (int ii = 1; ii <= 5; ii++)
			{
				dRc[ii].DeflateRect(0, 1);
				pDC->FillSolidRect(dRc[ii], m_bgColor[ii - 1]);
			}

			dRc[0].SetRect(dRc[1].left, cRc.top, cRc.right, cRc.bottom);
			pDC->Draw3dRect(dRc[0], COLOR_PANEBOX, COLOR_PANEBOX);
			pDC->SetTextColor(color);
		}
		break;
	}

	pDC->SelectObject(font);
	pDC->SelectObject(pen);
}

void CPane::Draw_Inters(CDC* pDC)
{
	if (m_arInterByCode.GetCount() < 1)
		return;
	if (m_index < 0)	m_index = 0;

	CClientDC	dc(this);
	if (!pDC)	pDC = &dc;
	pDC->SetBkMode(TRANSPARENT);

	CString	text, keys, sym;
	CRect	cRc, tRc;

	GetClientRect(cRc);
	pDC->SetBkMode(TRANSPARENT);
// draw border	
//	pDC->Draw3dRect(cRc, COLOR_PANEBOX, COLOR_PANEBOX);
///	cRc.DeflateRect(1, 1);
//	pDC->FillSolidRect(cRc, COLOR_BACK);

// draw data
	const _inter* inter = m_arInter.GetAt(m_index);
	if (!AfxIsValidAddress(inter, sizeof(_inter), TRUE))
		return;
	text.Format("%s", inter->Name);
	if (inter->Name.IsEmpty())	return;

	CFont*	font = pDC->SelectObject(&m_font);

	GetClientRect(cRc);
	cRc.OffsetRect(0, 1);
	int	cx = m_szButton.cx;//pDC->GetTextExtent(text).cx;
	tRc.SetRect(cRc.left, cRc.top, cRc.left + cx, cRc.bottom);
	
	text.Replace("&", "&&");
	pDC->SetTextColor(COLOR_PANETEXT);
	pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	COLORREF color{};
	switch (GetSign(inter->curr))
	{
	case '+':
		color = pDC->SetTextColor(GetColor(AxColor::up));//COLOR_UP));//
		sym.LoadString(ST_SYMBOL_UP2);
		break;
	case '-':
		color = pDC->SetTextColor(GetColor(AxColor::down));//COLOR_DOWN));//
		sym.LoadString(ST_SYMBOL_DOWN2);
		break;
	default:
		color = pDC->SetTextColor(GetColor(AxColor::bohap));//));
		break;
	}
	
	text = commaE(inter->curr);
	cx = pDC->GetTextExtent(text).cx;
	tRc.left = tRc.right+2;
	tRc.right = tRc.left + cx;
	pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);

	text.Format("  %s", dbE(inter->diff));
	cx = pDC->GetTextExtent(text).cx;
	tRc.left = tRc.right;
	tRc.right = tRc.left + cx;

	pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SetTextColor(GetColor(COLOR_TEXT));

	text.Format("  %s", commaE(inter->gvol));
	cx = pDC->GetTextExtent(text).cx;
	tRc.left = tRc.right;
	tRc.right = tRc.left + cx;
	pDC->DrawText(text, tRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	pDC->SelectObject(font);
}

void CPane::SetPaneInfo(int index)
{
	int	ii{}, idx{};
	DWORD	dwTemp = 0;
	CString	section, keys, value;

	CProfile profile(pkAxisTicker);

	m_arSymData.RemoveAll();
	section.Format("BAR_INFO_%02d", index);
	m_id = index;
	m_kind = profile.GetInt(section, "INDEXK", TKIND_INDEX);
	JudgeAppendedNews();

	switch (m_kind)
	{
	default:
	case TKIND_INDEX:
		for (ii = 0; ; ii++)
		{
			keys.Format("%03d", ii);
			value = profile.GetString(section, keys);

			if (value.IsEmpty())
				break;

			idx = GetTicKey(atoi(value));
			m_arItem.Add(idx);
		}

		if (m_arItem.GetSize() <= 0)	
			m_arItem.Add(2);

		m_aux = profile.GetInt(section, szAux);

		break;
	case TKIND_NEWS:
		m_arItem.Add(NEWS_BASE);
		for (ii = 0; ; ii++)
		{
			keys.Format("%02d", ii);
			value = profile.GetString(section, keys);
			if (value.IsEmpty())
				break;
			idx = atoi(value);
			m_arNewsKey.Add(idx);
//CString slog;
//slog.Format("[news] file=[%s] valse=[%s]", profile.GetFileName(), value);
//OutputDebugString(slog);
		}
		if (m_arNewsKey.GetSize() <= 0)	
			m_arNewsKey.Add(1);/*시황*/

		m_aux = profile.GetInt(section, szAux);
		dwTemp = profile.GetInt(section, szSelect, RGB(255, 255, 166));
		m_bNoAlarm = FALSE;
		m_clrAlarm = RGB(255, 255, 166);
		if (IsSetting(dwTemp))
		{
			m_bNoAlarm = IsNoUse(dwTemp);
			m_clrAlarm = getColor(dwTemp);
		}
		break;
	case TKIND_UPDOWN:
		ii = profile.GetInt(section, szSelect);
		switch (ii)
		{
		case 0:case 1:
			m_arItem.Add(UPDN_KOSPI+ii);	break;
		case 2:	
			m_arItem.Add(UPDN_KOSPI);
			m_arItem.Add(UPDN_KOSDAQ);	break;
		default:m_arItem.Add(UPDN_KOSPI);	break;
		}
		m_aux = profile.GetInt(section, szAux);
		break;
	case TKIND_INTER:
		m_select = profile.GetInt(section, szSelect);  //선택한 그룹 번호 
		loadInter(section);
		break;
		{
			m_aux = profile.GetInt(section, szSelect, -1);
			if (m_aux == -1)	break;
			value.Format("%02d", m_aux);
			loadInter(value);
		}
		break;
	}

	if (m_arItem.GetSize())
		m_index = 0;
	else	m_index = -1;
}

void CPane::JudgeAppendedNews()
{
	CString	section, key, value;
	int	nKind{}, ii{}, jj{}, nLastKey{}, max{};

	CProfile profile(pkAxisTicker);
	const int ver = profile.GetInt(szGeneral, "news_version");
	if (ver <= 1)
	{
		section.Format("BAR_INFO_%02d", m_id);
		nKind = profile.GetInt(section, "INDEXK", TKIND_INDEX);
		if (nKind == TKIND_NEWS)
		{
			int maxKey{};
			CString sKey{};

			for (int ii = 0; ; ii++)
			{
				sKey.Format("%02d", ii);
				const int dw = profile.GetInt(section, sKey);
				if (dw > maxKey)
					maxKey = dw;
				if (dw > 0)
					nLastKey = ii;
				else
					break;
			}

			if (maxKey > 2)
			{
				key.Format("%02d", nLastKey + 1);
				profile.Write(section, key, "15");
				profile.Write(szGeneral, "news_version", "2");
			}
		}
	}
}

int CPane::FindLastNewsKey(CString section)
{
	CString sKey;
	int	nLastKey = 0;
	
	CProfile profile(pkAxisTicker);
	for (int ii = 0; ; ii++)
	{
		sKey.Format("%02d", ii);
		const DWORD dw = profile.GetInt(section, sKey);
		if (dw > 0)	nLastKey = ii;
		else		break;
	}
	nLastKey++;

	return nLastKey;
}

void CPane::ReSetPaneInfo()
{
	int	ii{};
	CString	section, keys, value;

	m_arSymData.RemoveAll();
	m_arItem.RemoveAll();
	m_arNewsKey.RemoveAll();
	m_mapData.RemoveAll();
	m_arSym.RemoveAll();

	CProfile profile(pkAxisTicker);
	section.Format("BAR_INFO_%02d", m_id);

	switch (m_kind)
	{
	default:
	case TKIND_INDEX:
		for (ii = 0; ; ii++)
		{
			keys.Format("%03d", ii);
			value = profile.GetString(section, keys);
			/*
			if (value == "28")
			{
				continue;
			}
			*/
			if (value.IsEmpty())
				break;
// 			CString s;
// 			s.Format("RESETPANEINFO : [%s]\n",value);
// 			OutputDebugString(s);
			m_arItem.Add(GetTicKey(atoi(value)));
		}
		if (m_arItem.GetSize() <= 0)	
			m_arItem.Add(2);
		m_aux = profile.GetInt(section, szAux);
		break;
	case TKIND_NEWS:
		m_arItem.Add(NEWS_BASE);
		for (ii = 0; ; ii++)
		{
			keys.Format("%02d", ii);
			value = profile.GetString(section, keys);
			if (value.IsEmpty())
				break;
			m_arNewsKey.Add(atoi(value));
		}
		if (m_arNewsKey.GetSize() <= 0)	
			m_arNewsKey.Add(2);/*공시*/
		break;
	case TKIND_UPDOWN:
		ii = profile.GetInt(section, szSelect);
		switch (ii)
		{
		case 0:case 1:
			m_arItem.Add(UPDN_KOSPI+ii);	break;
		case 2:	
			m_arItem.Add(UPDN_KOSPI);
			m_arItem.Add(UPDN_KOSDAQ);	break;
		default:m_arItem.Add(UPDN_KOSPI);	break;
		}
		m_aux = profile.GetInt(section, szAux, 1);
		break;
	case TKIND_INTER:
		m_select = profile.GetInt(section, szSelect);
		loadInter(section);
		break;
		{
			m_aux = profile.GetInt(section, szSelect, -1);
			if (m_aux == -1)	break;
			value.Format("%02d", m_aux);
			loadInter(value);
		}
		break;
	}

	m_bReset = true;
	if (m_arItem.GetSize())
		m_index = 0;
	else	m_index = -1;
}

void CPane::SetFmx(bool setting, int kind, int key, CString symbol, CString dat)
{
	if (!setting)
	{
		if (m_index >= m_arItem.GetSize())
			m_index = 0;
		if (m_index >= 0 && m_index < m_arSym.GetSize())
		{
			CString	sym, dat;
			sym = m_arSym.GetAt(m_index);
			m_arSymData.Lookup(sym, dat);
			//TRACE("\nSetData: %s|%s\n", sym, dat);
			SetData(dat);
		}
		Invalidate();

		switch (m_kind)
		{
		case TKIND_INDEX:
			if (m_arSym.GetSize() > 1)
			{
				SetTimer(TM_INDEX, TMI_INDEX, NULL);
				//	KillTimer(TM_INDEX_TUJA);
				//	SetTimer(TM_INDEX_TUJA, TMI_INDEX_TUJA, NULL);
			}
			break;
		case TKIND_UPDOWN:
			if (m_arSym.GetSize() > 1)
				SetTimer(TM_UPDOWN, TMI_UPDOWN, NULL);
			break;
		case TKIND_INTER:
			if (m_arSym.GetSize() > 1)
				SetTimer(TM_INTER, TMI_INTER, NULL);
			break;
		}

		return;
	}

// 	s.Format("SETTING : %d\n",setting);
// 	OutputDebugString(s);

	if (m_kind != kind)	return;
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		const int	sKey = m_arItem.GetAt(ii);

// 		CString s;
// 		s.Format("Tick KEY [%d] [%d]\n",sKey,key);
// 		OutputDebugString(s);
		if (sKey != key)	continue;

		CString	oldDat, keys, value, tmps;
		
		CMapStringToString	newfms, oldfms;
		if (m_arSymData.Lookup(symbol, oldDat))
		{

			parseDat(dat, newfms);
			parseDat(oldDat, oldfms);

			if (oldfms.GetCount())
			{
				for (POSITION pos = newfms.GetStartPosition(); pos; )
				{
					newfms.GetNextAssoc(pos, keys, value);
					oldfms.SetAt(keys, value);
				}

				dat.Empty();
				for (POSITION pos = oldfms.GetStartPosition(); pos; )
				{
					oldfms.GetNextAssoc(pos, keys, value);
					if (dat.IsEmpty())
						dat.Format("%s\t%s", keys, value);
					else
					{
						tmps = dat;
						dat.Format("%s\t%s\t%s", tmps, keys, value);
					}
				}
			}

			newfms.RemoveAll();
			oldfms.RemoveAll();
		}

		if (m_kind == TKIND_NEWS)
		{
			m_arSymData.SetAt(symbol, dat);  //TKIND_NEWS
			parseDat(dat, newfms);
			if (newfms.Lookup(SNEW_INFO, value))
			{
				int	newsKey{};
				for (int jj = 0; jj < m_arNewsKey.GetSize(); jj++)
				{
					newsKey = m_arNewsKey.GetAt(jj);
					MatchNewsIndex(newsKey);
					if (newsKey == atoi(value))
					{
						SetData(dat);
						m_arSymData.SetAt(symbol, dat);  //TKIND_NEWS
						m_arHistoryNews.InsertAt(0, dat);
						if(m_arHistoryNews.GetSize() > MAX_NEWSCNT) 
							m_arHistoryNews.RemoveAt(MAX_NEWSCNT);
						break;
					}
				}

			}
	
			newfms.RemoveAll();
		}
		else
		{
			setSign(sKey, dat);
			symbol = replaceKey(symbol);
			m_arSymData.SetAt(symbol, dat);  
			tmps.Format("%d", key);
			m_arIndexbySym.SetAt(symbol, tmps);
			m_arSymbyIndex.SetAt(tmps, symbol);
			m_arSym.Add(symbol);

			CString slog;
			slog.Format("\r\n-----  SetFmx m_id=[%d]  cnt=[%d] symbol=[%s]  key=[%s] --------\r\n ", m_id, m_arSymData.GetCount(), symbol, dat);
		//	OutputDebugString(slog);
		}
		break;
	}
}

CString CPane::replaceKey(CString symbol)
{
	if (!symbol.IsEmpty() && (symbol.GetAt(0) == 'X' || symbol.GetAt(0) == 'x'))
	{
		CString	sym[] = {"X5001", "X0001", "X2001", "XQSTR", "XQ001", "XQ044", "XQ047"};
		CString	str = _T("");
		for (int ii = 0 ; ii < 7 ; ii++ )
		{
			str = sym[ii];
			if (str == symbol)
			{
				str = str.Mid(1);
				str = "K" + str;
TRACE("symbol before[%s] after[%s]\n", symbol, str);
				return str;
			}
		}
	}
	return symbol;
}

void CPane::SetEFmx(bool setting, int kind, int key, CString symbol, CString dat)
{
	if (!m_bReset)	return;
	if (!setting)
	{
		m_bReset = false;
		if (m_index >= m_arItem.GetSize())
			m_index = 0;
		if (m_index >= 0 && m_index < m_arSym.GetSize())
		{
			CString	sym, dat;
			sym = m_arSym.GetAt(m_index);
			if (m_arSymData.Lookup(sym, dat))
				SetData(dat);
			
		}
		Invalidate();

		switch (m_kind)
		{
		case TKIND_INDEX:
			if (m_arSym.GetSize() > 1)
				SetTimer(TM_INDEX, TMI_INDEX, NULL);
			break;
		case TKIND_UPDOWN:
			if (m_arSym.GetSize() > 1)
				SetTimer(TM_UPDOWN, TMI_UPDOWN, NULL);
			break;
		case TKIND_INTER:
			if (m_arSym.GetSize() > 1)
				SetTimer(TM_INTER, TMI_INTER, NULL);
			break;
		}

		return;
	}

	if (m_kind != kind)	return;
	for (int ii = 0; ii < m_arItem.GetSize(); ii++)
	{
		const int	sKey = m_arItem.GetAt(ii);
		if (sKey != key)	continue;

		CString	oldDat, keys, value, tmps;
		if (!m_arSymData.Lookup(symbol, oldDat))
		{
			CMapStringToString	newfms, oldfms;

			parseDat(dat, newfms);
			parseDat(oldDat, oldfms);

			if (oldfms.GetCount())
			{
				for (POSITION pos = newfms.GetStartPosition(); pos; )
				{
					newfms.GetNextAssoc(pos, keys, value);
					oldfms.SetAt(keys, value);
				}

				dat.Empty();
				for (POSITION pos = oldfms.GetStartPosition(); pos; )
				{
					oldfms.GetNextAssoc(pos, keys, value);
					if (dat.IsEmpty())
						dat.Format("%s\t%s", keys, value);
					else
					{
						tmps = dat;
						dat.Format("%s\t%s\t%s", tmps, keys, value);
					}
				}
			}
		}

		m_arSymData.SetAt(symbol, dat);  //안쓰는듯
		m_arSym.Add(symbol);
		break;
	}
}

void CPane::SetInter(char* dat, bool bRotate/*=true*/)
{
	if (m_kind != TKIND_INTER)
		return;
	
	struct	_iGrid {
		char	code[12];		 /* 종목코드			 */
		char    hnam[20];                /* 종목명                       */
		char    curr[8];                 /* 현재가                       */
		char    diff[6];                 /* 전일대비      (9999V99)      */
		char    gvol[12];                /* 거래량                       */
	};
	CString	code, name, curr, diff, gvol;

	const _iGrid*	grid = (_iGrid *) dat;
	code = CString(grid->code, sizeof(grid->code));	code.TrimRight(); 
	code = code.Mid(1);
	name = CString(grid->hnam, sizeof(grid->hnam));	name.TrimRight();
	curr = CString(grid->curr, sizeof(grid->curr));	curr.TrimRight();
	diff = CString(grid->diff, sizeof(grid->diff));	diff.TrimLeft();
	gvol = CString(grid->gvol, sizeof(grid->gvol));	gvol.TrimLeft();

	_inter*	inter = NULL;
	m_arInterByCode.Lookup(code, (CObject *&) inter);
	if (!inter)	return;
	inter->Name = name;
	inter->curr = curr;
	inter->diff = diff;
	inter->gvol = gvol;

	Invalidate(FALSE);
	if (bRotate && m_arInterByCode.GetCount() > 1)
	{
		m_index = 0;
		SetTimer(TM_INTER, TMI_INTER, NULL);
	}
}

void CPane::UpdatePaneData(int kind, CString dat)
{
	if (kind != m_kind)
		return;
	
	SetData(dat);
	Invalidate();
}
//#define DF_USE_MINI 1
void CPane::ProcessRTS(CString symbol, DWORD* data)
{
	/*CString stmp;
	stmp.Format("-- [%s] [%s] \r\n\ ", symbol, data );
	OutputDebugString(stmp);*/

	int const arrtick[4][20] = { {0,23,24,27,28,29,30,31,32,33,34,207,326,361,501,502,503,600},  //지수
									{0,14,15,16,22,41,42,44,45,46,47,48,49,301},           //뉴스
									{0,34,251,252,253,254,255,256},                        //등락 
									{23,24,27 } };                                                       //관심
	CString		keys, value, tmps, dat;
	CMapStringToString	newfms, oldfms;
	//int const arTick[] = { 0, 14, 15, 16, 22, 23, 24, 27, 28, 29, 30, 31, 32, 33, 34, 41, 42, 44, 45, 46, 47, 48, 49, 301, 207, 251,  252,  253, 254, 255, 256, 326,
	//	333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 361, 501, 502, 503, 600 };
	

	if (!m_arSymData.Lookup(symbol, value))	
		return;
	
	if (m_index == -1)	
		return;

	parseDat(value, oldfms);

	if (m_arIndexbySym.Lookup(symbol, tmps))
		setSign(atoi(tmps), dat);

	/*stmp.Format("---------------------------------------------- \n");
	//OutputDebugString(stmp);
	stmp.Format("--------------------- m_kind=[%d]  data=[%s] \n", m_kind, (char*)data[0]);
	//OutputDebugString(stmp);*/

	if (m_kind != TKIND_NEWS && oldfms.GetCount())
	{
#ifdef DF_USE_MINI
		for (int jj = 0; jj < sizeof(arTick) / sizeof(int); jj++)
		{
			if (jj == 0)
			{
				keys.Format("%03d", arTick[0]);
				oldfms.SetAt(keys, (char*)data[arTick[jj]]);
			}
			else if (arTick[jj])
			{
				keys.Format("%03d", arTick[jj]);

				//	stmp.Format("--------------------- m_kind=[%d]  jj=[%d] symbol=[%s] rtssym=[%s] data=[%s] \n", m_kind, jj, symbol, keys, (char*)data[arrtick[m_kind][jj]]);
				//	OutputDebugString(stmp);
				oldfms.SetAt(keys, (char*)data[arTick[jj]]);
			}
		}
#else		
		
		for (int jj=0;jj<1000;jj++)
		{
			if(data[jj])
			{
				keys.Format("%03d",jj);

			//	stmp.Format("--------------------- m_kind=[%d]  symbol=[%s] rtssym=[%s] data=[%s] \n", m_kind, symbol, keys, (char*)data[jj]);
			//	OutputDebugString(stmp);

				oldfms.SetAt(keys, (char*)data[jj]);
			}
		}
#endif

		for (POSITION pos = oldfms.GetStartPosition(); pos; )
		{
			oldfms.GetNextAssoc(pos, keys, value);
			if (dat.IsEmpty())
				dat.Format("%s\t%s", keys, value);
			else
			{
				tmps = dat;
				dat.Format("%s\t%s\t%s", tmps, keys, value);
			}
		}
	}
	else if(m_kind == TKIND_NEWS)
	{
#ifdef DF_USE_MINI
		const int isize = sizeof(arTick) / sizeof(int);
		for (int jj = 0; jj < sizeof(arTick) / sizeof(int); jj++)
		{
			if (jj == 0)
			{
				keys.Format("%03d", arTick[0]);
				oldfms.SetAt(keys, (char*)data[arTick[jj]]);
			}
			else if (arTick[jj])
			{
				keys.Format("%03d", arTick[jj]);

				//	stmp.Format("--------------------- m_kind=[%d]  jj=[%d] symbol=[%s] rtssym=[%s] data=[%s] \n", m_kind, jj, symbol, keys, (char*)data[arrtick[m_kind][jj]]);
				//	OutputDebugString(stmp);
				newfms.SetAt(keys, (char*)data[arTick[jj]]);
			}
		}
#else
		for (int jj=0;jj<1000;jj++)
		{
			if(data[jj])
			{
				keys.Format("%03d",jj);
	
		//		stmp.Format("---------------------m_kind=[%d]   symbol=[%s] rtssym=[%s] dat=[%s] \n",m_kind, symbol, keys, (char*)data[jj] );
		//		OutputDebugString(stmp);

				newfms.SetAt(keys, (char*)data[jj]);
			}
		}
#endif
		for (POSITION pos = newfms.GetStartPosition(); pos; )
		{
			newfms.GetNextAssoc(pos, keys, value);
			if (dat.IsEmpty())
				dat.Format("%s\t%s", keys, value);
			else
			{
				tmps = dat;
				dat.Format("%s\t%s\t%s", tmps, keys, value);
			}
		}
	}

	//CString stmp;
	//stmp.Format("%s", (char*)data[0]);
	//if (m_kind == TKIND_INDEX)
	//{
	//	if (stmp == "J")
	//		m_arSymData.SetAt(symbol, dat);
	//}
	//else if (m_kind == TKIND_UPDOWN)
	//{
	//	if (stmp == "U")
	//		m_arSymData.SetAt(symbol, dat);
	//}
	//else
	//	m_arSymData.SetAt(symbol, dat);
	

	m_arSymData.SetAt(symbol, dat);

	if (m_kind == TKIND_INTER)
	{
		_inter* inter;
		if (m_arInterByCode.Lookup(symbol, (CObject *&) inter))
		{
			
		//	stmp.Format("---------------------m_kind=[%d]   symbol=[%s] dat=[%s]  [%s] [%s] [%s]\n", m_kind, symbol,  (char*)data[0] , (char*)data[23], (char*)data[24], (char*)data[27]);
		 //   OutputDebugString(stmp);

			if(data[23])
				inter->curr = (char*)data[23];
			if(data[24])
				inter->diff = (char*)data[24];
			if(data[27])
				inter->gvol = (char*)data[27];

			if (!m_arSym.GetAt(m_index).Compare(symbol))
 				Invalidate();
		}
	}
	else if (m_kind == TKIND_NEWS)
	{
		if (data[41])
		{
			value = (char*)data[41];

			int	newsKey{};

//stmp.Format("[\n [news] ----------------- ProcessRTS m_arNewsKey size = [%d]\n", m_arNewsKey.GetSize());
//OutputDebugString(stmp);

			for (int jj = m_arNewsKey.GetSize()-1; jj > -1 ; jj--)
			{
				newsKey = m_arNewsKey.GetAt(jj);
		
				MatchNewsIndex(newsKey);

//slog.Format("[news]  newsKey  = [%d]  [%s]  [%s]\n", newsKey, value, dat);
//if(atoi(value) == 15 || atoi(value) == 16)
//	OutputDebugString(slog);
			
				if (newsKey == atoi(value))
				{
					SetData(dat);
					Invalidate();
					m_arHistoryNews.InsertAt(0, dat);
					if(m_arHistoryNews.GetSize() > MAX_NEWSCNT) 
							m_arHistoryNews.RemoveAt(MAX_NEWSCNT);
					NewNews(true);
					break;
				}
			}
		}
	}
	else if (m_index >= 0 && m_index < m_arSym.GetSize())
	{
	//	stmp.Format("m_index = [%d] m_arSym = [%d] \n", m_index, m_arSym.GetSize());
	//	OutputDebugString(stmp);
		value = m_arSym.GetAt(m_index);
		if (!value.Compare(symbol)) 
		{
			SetData(dat);
			Invalidate();
		}
	}
	newfms.RemoveAll();
	oldfms.RemoveAll();


//	stmp.Format("---------------------------------------------- \n");
//	OutputDebugString(stmp);
}

CString CPane::commaE(CString value, bool remove)
{
	CString		tmps, gb, tail;
	int		pos{};

	value.TrimLeft();	value.TrimRight();
	if (value.IsEmpty())	return _T("");

	if (value.Left(1) == "+" || value.Left(1) == "-")
	{
		gb = value.Left(1);
		value = value.Mid(1);
	}
	
	pos = value.Find('.');
	if (pos != -1)
	{
		tail = value.Mid(pos);
		value = value.Left(pos);
	}

	if (remove)	gb = _T("");
	if (value.GetLength() < 3)
		return gb + value + tail;

	for (int ii = value.GetLength()-1, jj = 0; ii >= 0; jj++)
	{
		if (jj != 0 && jj % 3 == 0)
			tmps.Insert(0, ',');
		tmps.Insert(0, value.GetAt(ii));
		ii--;
	}
	return gb + tmps + tail;
}

COLORREF CPane::GetColor(int index)
{
	return Axis::GetColor(index);
	/**
	int		pos;
	CString		tmpS, dat, section, key;
	CStringArray	ary;

	CProfile profile(pkPalette);
	section = profile.GetString(GENERALSN, szPalette, szPalette);

	key.Format("RGB%03d", index);
	dat = profile.GetString(section, key);
	if (dat.IsEmpty())
	{
		if (!section.Compare(szPalette))
			return RGB(215, 215, 215);
		section = szPalette;
		dat = profile.GetString(section, key);
		if (dat.IsEmpty())	
			return RGB(215, 215, 215);
	}

	for (; !dat.IsEmpty();)
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			ary.Add(dat);
			break;
		}
		tmpS = dat.Left(pos++);
		dat = dat.Mid(pos);
		ary.Add(tmpS);
	}

	if (ary.GetSize() < 3)
		return RGB(215, 215, 215);
	return RGB(atoi(ary.GetAt(0)), atoi(ary.GetAt(1)), atoi(ary.GetAt(2)));
	**/
}

void CPane::SetData(CString dat, BOOL bTimer)
{
	//OutputDebugString("SETDATA START\n");

	CString stmp, sym, sdata, value;
	int pos{};
	stmp = dat;

	if (((m_kind == TKIND_INDEX) || (m_kind == TKIND_UPDOWN)) && !bTimer)
	{
		while (!stmp.IsEmpty())
		{
			pos = stmp.Find('\t');
			if (pos == -1)	break;
			sym = stmp.Left(pos++);
			stmp = stmp.Mid(pos);

			pos = stmp.Find('\t');
			if (pos == -1)
			{
				sdata = stmp;
				stmp.Empty();
			}
			else
			{
				sdata = stmp.Left(pos++);
				stmp = stmp.Mid(pos);
			}
			// J = KRX100, KOSPI, KOSPI200, KOSDAQ, KOSDAQ벤처, KQ150, KRX SRI
			// K = 최근월, 차근월, KP최근월, KP차근월, KP최근월 Basis
			// W = KOSPI 프로그램, KOSDAQ 프로그램
			// M = KOSPI투자자(금액), KOSDAQ투자자(금액), 선물투자자(수량), 콜투자자(금액), 풋투자자(금액)
			if (sym == "000")
			{
				if (m_kind == TKIND_INDEX)
				{
					if (sdata != "J" && sdata != "W" && sdata != "X" && sdata != "M" && sdata != "K")  //업종 , 프로그램, 예상, 투자자별  업종 매매
						return;
				}
				else if (m_kind == TKIND_UPDOWN)
				{
					if (sdata != "U")
						return;
				}
				break;
			}
		}
	}

	m_mapData.RemoveAll();
	
	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;
		sym = dat.Left(pos++);
		dat = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat = dat.Mid(pos);
		}
		
		value.TrimRight();
		m_mapData.SetAt(sym, value);  //real
	}
}

void CPane::SetData(DWORD* data)
{
	CString sym, value;
	
	m_mapData.RemoveAll();

	for(int i=1;i<1000;i++)
	{
		if(data[i])
		{
			sym.Format("%03d",i);
// 			CString s;
// 			s.Format("XXXXX : %s\n",sym);
// 			OutputDebugString(s);
			value = (char*)data[i];
			m_mapData.SetAt(sym, value);  //사용안함
		}
	}
}

void CPane::parseDat(CString dat, CMapStringToString& ary)
{
	CString sym, value;
	int	pos{};
//	OutputDebugString(dat + "\r\n");
	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;
		sym = dat.Left(pos++);
		dat = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat = dat.Mid(pos);
		}

		ary.SetAt(sym, value);
	}
}

CString CPane::dbE(CString value)
{
	CString		retS, sign;

	if (value.IsEmpty())	return _T("");

	const char ch = value.GetAt(0);
	value = value.Mid(1);
	value.TrimLeft();

	switch (ch)
	{
	case '1':	// 상한
		sign.LoadString(ST_SYMBOL_UP1);	break;// = "↑";	break;
	case '2':	// 상승
	case '+':
		sign.LoadString(ST_SYMBOL_UP2);	break;//sign = "▲";	break;
	default:
	case '3':	// 보합
		sign = "";	break;
	case '4':	// 하한
		sign.LoadString(ST_SYMBOL_DOWN1);	break;//sign = "↓";	break;
	case '5':	// 하락
	case '-':
		sign.LoadString(ST_SYMBOL_DOWN2);	break;//sign = "▼";	break;
	}

	value = commaE(value);
	retS.Format("%s %6s", sign, value);
	return retS;
}

CString CPane::getDBSign(CString value)
{
	CString		retS, sign;

	if (value.IsEmpty())	return _T("");

	const char ch = value.GetAt(0);
	value = value.Mid(1);
	value.TrimLeft();

	switch (ch)
	{
	case '1':	// 상한
		sign.LoadString(ST_SYMBOL_UP1);	break;// = "↑";	break;
	case '2':	// 상승
	case '+':
		sign.LoadString(ST_SYMBOL_UP2);	break;//sign = "▲";	break;
	default:
	case '3':	// 보합
		sign = "";	break;
	case '4':	// 하한
		sign.LoadString(ST_SYMBOL_DOWN1);	break;//sign = "↓";	break;
	case '5':	// 하락
	case '-':
		sign.LoadString(ST_SYMBOL_DOWN2);	break;//sign = "▼";	break;
	}

	return sign;
}

// 0 - 보합, 1 - 상승, 2 - 하락
char CPane::GetSign(CString dat)
{
	if (dat.Find('+') != -1)return '+';
	if (dat.Find('-') != -1)return '-';
	return ' ';
}

void CPane::loadInter(CString section)
{
	CString stmp;
	CProfile profile(pkAxisTicker);
	stmp = profile.GetString(section, "SELECT", "-1");

	if (atoi(stmp) < 0)
		return;

	HWND m_hMain = AfxGetMainWnd()->m_hWnd;
	CString strHwnd;
	strHwnd.Format("%d\t%02d", this->m_hWnd, atoi(stmp) + 1);
	::SendMessage(m_hMain, WM_INTERMSG, MAKEWPARAM(MMSG_SEARCH_GROUPCODE, 0), (LPARAM)(LPSTR)(LPCTSTR)strHwnd);

	for (int ii = 0; ii < m_arInter.GetSize(); ii++)
		delete m_arInter.GetAt(ii);
	m_arInter.RemoveAll();
	m_arInterByCode.RemoveAll();
}

int CPane::GetPosY(int value, int max)
{
	CRect	cRc;
	
	GetClientRect(cRc);
	cRc.DeflateRect(0, 2);
	int	posy = cRc.bottom;
	double	inc = 0.0;
	const double valueByPixel = (double)max / (double)cRc.Height();
	if (!value)	return posy;

	for (;;)
	{
		if (max <= value)
		{
			posy = cRc.top;
			break;
		}

		if (inc >= value)
			break;
		posy--;
		inc += valueByPixel;
	}

	return posy;
}

CString CPane::GetIndexSName(int idx)
{
// 	const char* index[] = {
// 		"KRX100",		"KOSPI",		"KOSPI200",		"KOSTAR",		"KOSDAQ",
// 		"KQ 벤처",		"KP 최근월",	"KP 차근월",	
// 		
// 		"KOSPI",		"KOSDAQ",		"KOSPI",		"KOSDAQ",
// 		"선물",			"콜옵션",		"풋옵션",
// 
// 		"NASDAQ",		"NA100선물",	"Mini N100",	"S&&P500",		"S&&P500선물",
// 		"Mini SP500",	"DOWJONES",
// 
// 		"NIKKEI",		"대만",			"ASSHARE",		"항셍",			"HSCE", "KRX SRI", "BASIS",
// 
// 		"상해B",	"심천B",	"BOMBAY",	"대만가권",	"HCMC",	"반도체",
// 		"INMEX",	"BOVESPA",	"FTSE100",	"Xetra",	"CAC40",	"RTSI", "상해종합"
// 	};
	//티커지수 추가시 고쳐야 되는 부분
	const char* const index[] = {
		"KRX100",		"KOSPI",		"KOSPI200",		"KQ150",/*"KOSTAR",*/		"KOSDAQ",
			"KQ 벤처",		"KP 최근월",	"KP 차근월",	
			
			"KOSPI",		"KOSDAQ",		"KOSPI",		"KOSDAQ",
			"선물",			"콜옵션",		"풋옵션",
			
			"KRX SRI", "BASIS"
	};

	if (idx >= 0 && idx < ArraySize(index))
		return " " + CString(index[idx]);
	else
		return "";
}

void CPane::saveinfo(int selectN)
{
	CProfile profile(pkAxisTicker);
	profile.Write(Format("BAR_INFO_%02d", m_id), szSelect, Format("%d", selectN));
}

BOOL CPane::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
}

void CPane::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
/*	if (!GetImgRect().PtInRect(point))
	{
		CString	mapName, code;
		switch (m_kind)
		{
		case TKIND_INDEX:
			if (m_index >= 0 && m_index < m_arSym.GetSize())
			{
				code = m_arSym.GetAt(m_index);
				if (code.GetLength() == 3)
					mapName.Format("TG018200%s\t%s\n", "21011", code);
				else if (code.GetLength() == 5)
					mapName.Format("TG030100%s\t%s\n", "31011", code);
				else	mapName = _T("TG018200");
			}
			else	mapName = _T("TG018200");
			(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *) mapName.operator LPCTSTR());
			break;
		case TKIND_NEWS:	(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM) "TG067100");	break;
		case TKIND_UPDOWN:	(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM) "TG011500");	break;
		case TKIND_INTER:
			mapName.Format("TG074500%s\t%d\n", "INTERIDX", m_aux);
			(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)(char *) mapName.operator LPCTSTR());
			break;
		}
	}*/
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CPane::setSign(int key, CString dat)
{
	if (m_kind != TKIND_INDEX)
		return;

	CMapStringToString	fms;
	CString			value;

	parseDat(dat, fms);
	if (fms.Lookup(NEWKJ, value))
	{
		m_arSign[key].InsertAt(0, value);
		while (m_arSign[key].GetSize() > 5)
			m_arSign[key].RemoveAt(m_arSign[key].GetSize()-1);
	}
}

void CPane::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CMenuXP		menu;
	menu.CreatePopupMenu();
	switch (m_kind)
	{
	case TKIND_INDEX:
		if (SetTickPopup())
			break;
		break;	
	case TKIND_NEWS:
		{
		const int	idx = 1;
		int index{};
			for (int i = 0; i < Axis::GetNewsCount(); i++)
			{
				menu.AppendMenuX(MF_STRING, i + 1, Axis::GetNewsName(i));
			}
			menu.AppendMenuX(MF_SEPARATOR);
			menu.AppendMenuX(MF_STRING, 999, "상세설정");
			for (int ii = 0; ii < m_arNewsKey.GetSize(); ii++)
			{
				index = m_arNewsKey.GetAt(ii);
				menu.CheckMenuItem(index, MF_CHECKED|MF_BYCOMMAND);
			}
			ClientToScreen(&point);
			const int nRtn = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, point.x, point.y, this);
			if (nRtn == 999)
			{
				(*m_axiscall)(AXI_TICKSETUP, 0, 0);
			}
			else if (nRtn > 0)
			{
				if (menu.GetMenuState(nRtn, MF_BYCOMMAND) & MF_CHECKED)
				{
					for (int ii = 0; ii < m_arNewsKey.GetSize(); ii++)
					{
						index = m_arNewsKey.GetAt(ii);
						if (index == nRtn)
							m_arNewsKey.RemoveAt(ii);
					}
				}
				else
				{
					int nInsert = m_arNewsKey.GetSize();
					for (int ii = 0; ii < m_arNewsKey.GetSize(); ii++)
					{
						index = m_arNewsKey.GetAt(ii);
						if (index > nRtn)
						{
							nInsert = ii;
							break;
						}
					}
					m_arNewsKey.InsertAt(nInsert, nRtn);
				}
			
				save();
				ReSetPaneInfo();
				// 전체 재조회.. ㅜㅜ;
				(*m_axiscall)(AXI_SENDTICKINFO, 0, 0);
			}
		}
		break;
	case TKIND_UPDOWN:
		{
			menu.AppendMenuX(MF_STRING, UPDN_KOSPI, "코스피");
			menu.AppendMenuX(MF_STRING, UPDN_KOSDAQ, "코스닥");
			menu.AppendMenuX(MF_STRING, UPDN_KOSPI + UPDN_KOSDAQ, "자동");
			menu.AppendMenuX(MF_SEPARATOR);
			menu.AppendMenuX(MF_STRING, 999, "상세설정");

			if (m_arItem.GetSize() == 1)
			{
				if (m_arItem.GetAt(0) == UPDN_KOSPI)
					menu.CheckMenuItem(UPDN_KOSPI, MF_CHECKED|MF_BYCOMMAND);
				if (m_arItem.GetAt(0) == UPDN_KOSDAQ)
					menu.CheckMenuItem(UPDN_KOSDAQ, MF_CHECKED|MF_BYCOMMAND);
			}
			else if (m_arItem.GetSize() == 2)
				menu.CheckMenuItem(UPDN_KOSPI + UPDN_KOSDAQ, MF_CHECKED|MF_BYCOMMAND);

			ClientToScreen(&point);
			const int nRtn = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, point.x, point.y, this);
			if (nRtn == 999)
			{
				(*m_axiscall)(AXI_TICKSETUP, 0, 0);
				break;
			}

			switch (nRtn)
			{
			case UPDN_KOSPI:
				m_select = 0;
			case UPDN_KOSDAQ:
				m_arItem.RemoveAll();
				m_arItem.Add(nRtn);
				
				if (nRtn == UPDN_KOSDAQ)
					m_select = 1;
				break;
			case UPDN_KOSPI + UPDN_KOSDAQ:
				m_arItem.RemoveAll();
				m_arItem.Add(UPDN_KOSPI);
				m_arItem.Add(UPDN_KOSDAQ);
				m_select = 2;
				break;
			}
			
			save();
			ReSetPaneInfo();
			// 전체 재조회.. ㅜㅜ;
			(*m_axiscall)(AXI_SENDTICKINFO, 0, 0);
		}
		break;
	case TKIND_INTER:
		{
			CStringArray arGroup;
			CStringArray arKey;
			LoadInterGroup(arGroup);
		/*	for (int ii = 0; ii < arGroup.GetSize(); ii++)
			{
				CString sGroup = arGroup.GetAt(ii);
				int pos = sGroup.Find(':');
				if (pos == -1)	break;
				arKey.Add(sGroup.Left(pos++));
				sGroup = sGroup.Mid(pos);

				menu.AppendMenuX(MF_STRING, ii + 1, sGroup);
			}
			menu.AppendMenuX(MF_SEPARATOR);
			menu.AppendMenuX(MF_STRING, 999, "상세설정");
			if ((int)m_select < arGroup.GetSize())
				menu.CheckMenuItem(m_select + 1, MF_CHECKED|MF_BYCOMMAND);
			else
				menu.CheckMenuItem(1, MF_CHECKED|MF_BYCOMMAND);
			ClientToScreen(&point);
			const int nRtn = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, point.x, point.y, this);

			if (nRtn == 999)
			{
				(*m_axiscall)(AXI_TICKSETUP, 0, 0);
			}
			else if (nRtn > 0 && (nRtn - 1) != (int)m_select)
			{
				CString section;
				section.Format("BAR_INFO_%02d", m_id);
				m_select = nRtn - 1;
				save();
			
				ReSetPaneInfo();
			
				(*m_axiscall)(AXI_SENDTICKINFO, 0, 0);
			}*/
		}
		break;
		
	}
	//(*m_axiscall)(AXI_TICKSETUP, 0, 0);
//	CWnd::OnRButtonDown(nFlags, point);
}



//기존 해외 지수
// const int indexKey[] = {
// 	1, 2, 3, 4, 5, 6, 9, 10, 13, 14, 15, 16, 17, 18, 19, 20,
// 		21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33
// };
//새로운 해외 지수
// const int indexKey[] = {
// 	1, 2, 3, 4, 5, 6, 9, 10, 13, 14, 15, 16, 17, 18, 19, 20,
// 		21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50
// };
//티커지수 추가시 고쳐야 되는 부분
const int indexKey[] = {
	1, 2, 3, 4,/*4,*/ 5, 6, 9, 10, 13, 14, 15, 16, 17, 18, 19, 32, 33
};

const int indexKeyCount = ArraySize(indexKey);

int CPane::GetTicKey(int index)
{
	if (index >= 0 && index < indexKeyCount)
		return indexKey[index];
	else
		return 0;
}

int CPane::GetIndexByKey(int key)
{
	//if(key == 50)
	if(key == 33)
	{
		CString t;
		t = "0";
	}

	for (int i = 0; i < indexKeyCount; i++)
	{
		if (indexKey[i] == key)
			return i;
	}

	return -1;
}

CString CPane::GetdataUPDN(CPoint pt)
{
	CString	dats, tmps, text, chSym, kSym = "001";
	if (m_kind != TKIND_UPDOWN)
		return "";

	CClientDC	dc(this);
	int		index = -1, width{}, cx{};
	CRect		cRc, dRc[RMAX];

	GetClientRect(cRc);
	CFont* font = dc.SelectObject(&m_font);
	cx = m_nTitleCx = dc.GetTextExtent(PANETITLEWIDTH).cx;

	if (m_arItem.GetAt(m_index) != 7)	
		kSym = "201";

	if (m_aux == AUX_TEXT)
	{
		width = (cRc.Width() - cx)/5;
		for (int ii = 5, idx = 0; ii >= 0; ii--, idx++)
		{
			if (!ii)
				dRc[ii].SetRect(cRc.left, cRc.top, dRc[ii+1].left + 1, cRc.bottom);
			else
				dRc[ii].SetRect(cRc.right - width*(idx+1), cRc.top,
						cRc.right - width*idx + 1, cRc.bottom);
		}

		for (int ii = 1; ii < 6; ii++)
		{
			if (dRc[ii].PtInRect(pt))
			{
				index = ii;
				break;
			}
		}
	}
	else
	{
		int	total = 0, value{}, w{};
		CArray < int, int > ary, aryW;
		if (m_mapData.Lookup(S_SH, text))
		{
			value = atoi(text);	ary.Add(value);	total += value;
			m_mapData.Lookup(S_SS, text);	value = atoi(text);	ary.Add(value);	total += value;
			m_mapData.Lookup(S_BH, text);	value = atoi(text);	ary.Add(value);	total += value;
			m_mapData.Lookup(S_HL, text);	value = atoi(text);	ary.Add(value);	total += value;
			m_mapData.Lookup(S_HH, text);	value = atoi(text);	ary.Add(value);	total += value;

			width = cRc.Width() - cx;
			for (int ii = ary.GetSize() - 1; ii >= 0; ii--)
			{
				value = ary.GetAt(ii);
				if (value)
				{
					w = (value*width)/total;
					if (!w)	w = 1;	// fit

				}
				else	w = 0;
				aryW.InsertAt(0, w);
			}

			total = 0;
			for (int ii = ary.GetSize(); ii >= 0; ii--)
			{
				if (ii)
				{
					value = aryW.GetAt(ii-1);
					if (!value)
					{
						if (ii == ary.GetSize())
						{
							dRc[ii].SetRect(cRc.right, cRc.top,
								cRc.right, cRc.bottom);
						}
						else
						{
							dRc[ii].SetRect(dRc[ii+1].left, cRc.top,
								dRc[ii+1].left, cRc.bottom);
						}
					}
					else
					{
						dRc[ii].SetRect(cRc.right - total - value, cRc.top,
								cRc.right - total +1, cRc.bottom);
					}
					total += value;
				}
				else
					dRc[ii].SetRect(cRc.left, cRc.top, dRc[ii+1].left, cRc.bottom);
			}

			for (int ii = 1; ii < 6; ii++)
			{
				if (dRc[ii].PtInRect(pt))
				{
					index = ii;
					break;
				}
			}

			ary.RemoveAll();
			aryW.RemoveAll();
		}
	}
	dc.SelectObject(font);

	dats.Format("IB210300200901\t%s\n", kSym);
	
	for (int ii = 1; index != -1 && ii < 6; ii++)
	{
		chSym.Format("%d", ii);
		if (ii == 4)	chSym.Format("%d", ii+1);
		if (ii == 5)	chSym.Format("%d", ii-1);
		tmps.Format("20040%s\t%s\n", chSym, index == ii ? "1" : "0");
		dats += tmps;
	}
	dats += "main_E\t1\n";
	//MessageBox(dats);

	return dats;
}

void CPane::OnDestroy() 
{
	if (m_ToolTip)
	{
		if (m_ToolTip->m_hWnd)
		{
			//m_ToolTip->RelayEvent(NULL);
			m_ToolTip->Activate(FALSE);
			m_ToolTip->DelTool(this);
		}
#ifndef DF_USESTL_PANE
		delete m_ToolTip;
#endif
		m_ToolTip = NULL;
	}
	CWnd::OnDestroy();
	
	NewNews(false);	
}

void CPane::NewNews(bool bOn)
{
	if (m_bNoAlarm)
		return;
	m_bNew = bOn;

	if (bOn)
	{
		SetTimer(TM_NEWNEWS, 500, NULL);
		m_nCount = 2;
	}
	else
	{
		m_nCount = 0;
		KillTimer(TM_NEWNEWS);		
	}
}

////////////////////////////////////////////////////////////////
// 2006.11.02 틱커의 뉴스의 색상 변경 옵션 추가

BOOL CPane::IsSetting(DWORD value)
{
	const BOOL	bRet = (value & 0xF0000000) >> 28;
	return bRet;
}

BOOL CPane::IsNoUse(DWORD value)
{
	const BOOL	bRet = (value & 0x0F000000) >> 24;
	return bRet;
}

COLORREF CPane::getColor(DWORD value)
{
	return (value & 0x00FFFFFF);
}

COLORREF CPane::GetReverseColor(COLORREF color)
{
	int red = GetRValue(color);
	int green = GetGValue(color);
	int  blue = GetBValue(color);

	if ((red == green && green == blue)
		&& (red > 120 && red < 140) 
		&& (green > 120 && green < 140)
		&& (blue > 120 && blue < 140))
	{
		red = red - 20;
		green = green - 20;
		blue = blue - 20;
	}

	return RGB(255-red, 255-green, 255-blue);
}

CSize CPane::GetStringSize(CDC* pDC, CString string)
{
	CSize	size;
	size = pDC->GetTextExtent(string);
	return size;
}

////////////////////////////////////////////////////////////////

void CPane::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	HMENU hMenu = AfxGetThreadState()->m_hTrackingMenu;
	CMenu	*pMenu = CMenu::FromHandle(hMenu);
	pMenu->MeasureItem(lpMeasureItemStruct);	
	//CWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CPane::save()
{
	CProfile profile(pkAxisTicker);
	CString section, keys, value;
	section.Format("BAR_INFO_%02d", m_id);

	profile.WriteSection(section, "");
	profile.Write(section, "INDEXK", Format("%d", m_kind));

	switch (m_kind)
	{
	case TKIND_INDEX:
		{
			if (m_arItem.GetSize() <= 0)	
				m_arItem.Add(1);

			for (int kk = 0; kk < m_arItem.GetSize(); kk++)
			{
				keys.Format("%03d", kk);
				value.Format("%d",  GetIndexByKey(m_arItem.GetAt(kk)));
				profile.Write(section, keys, value);
			}

			keys = _T(szAux);
			value.Format("%d", m_aux);
			profile.Write(section, keys, value);
		}
		break;
	case TKIND_NEWS:
		{
			if (m_arNewsKey.GetSize() <= 0)	m_arNewsKey.Add(2);
			for (int kk = 0; kk < m_arNewsKey.GetSize(); kk++)
			{
				keys.Format("%02d", kk);
				value.Format("%d",  m_arNewsKey.GetAt(kk));
				profile.Write(section, keys, value);
			}
			keys = _T(szAux);
			value.Format("%d", m_aux);
			profile.Write(section, keys, value);
			
			// 뉴스 깜박임 색상 옵션 추가
			keys = _T(szSelect);
			value.Format("%d", makeOption(m_bNoAlarm, m_clrAlarm));
			profile.Write(section, keys, value);
		}
		break;
	case TKIND_UPDOWN:
		{
			keys = _T(szSelect);
			value.Format("%d", m_select);
			profile.Write(section, keys, value);
			
			keys = _T(szAux);
			value.Format("%d", m_aux);
			profile.Write(section, keys, value);
		}
		break;
	case TKIND_INTER:
		{
			keys = _T(szSelect);
			value.Format("%d", m_select);
			profile.Write(section, keys, value);
		//	writeInterCode(section, m_select);
		}
		break;
	default:
		//AfxMessageBox("Tick infomation save Error!");
		break;
	}
}

DWORD CPane::makeOption(BOOL bUse, COLORREF color)
{
	const DWORD	flag = 0x01000000;
	DWORD	ret =  0x10000000 | (DWORD)color;
	if (bUse)
		ret = ret | flag;
	return ret;
}

void CPane::LoadInterGroup(CStringArray &arGroup)
{
	HWND m_hMain = AfxGetMainWnd()->m_hWnd;
	CString stemp;
	CString strHwnd;
	strHwnd.Format("%d", this->m_hWnd);
	::SendMessage(m_hMain, WM_INTERMSG, MAKEWPARAM(MMSG_SEARCH_INTERGROUP, 0),  (LPARAM)strHwnd.operator LPCSTR());
	

	//CString keys, gName;
	//CProfile profile(pkPortfolio);

	//gName = profile.GetString("GROUPORDER", "00");
	//if (gName.IsEmpty())
	//	return;
	//
	//arGroup.RemoveAll();
	//while (!gName.IsEmpty())
	//{
	//	int pos = gName.Find(';');
	//	if (pos == -1)	break;
	//	keys = gName.Left(pos++);
	//	gName = gName.Mid(pos);
	//	arGroup.Add(keys);
	//}

	//CString group;
	//for (int ii = 0; ii < arGroup.GetSize(); ii++)
	//{
	//	keys = arGroup.GetAt(ii);
	//	group = profile.GetString("GROUPNAME", keys);
	//	if (group.IsEmpty())
	//		break;
	//	arGroup.SetAt(ii, keys + ":" + group);
	//}
}

void CPane::writeInterCode(CString section, int groupN)
{
	//if (groupN < 0)	return;
	//CString		file, ifile, sKey = GetGroupKeyByIndex(groupN);
	//CFile		fileH;

	//if (sKey.IsEmpty())
	//	return;

	//CProfile profile(pkAxisTicker);
	//ifile = GetProfileFileName(pkPortfolio);

	//ifile.Delete(ifile.GetLength() - 2, 2);
	//ifile += sKey;

 // 	if (!fileH.Open(ifile, CFile::modeRead|CFile::typeBinary))
	//{
	//	CString	msg;
	//	msg.Format("관심종목 파일을 찾을수 없습니다.\n[%s]", ifile);
	//	::MessageBox(m_hWnd, msg, "파일오류", MB_ICONASTERISK);
	//	return;
	//}

	//int	idx = 0;
	//struct _inters	inter;
	//while (fileH.Read(&inter, sizeof(inter)))
	//{
	//	char	jmCode [16];
	//	sprintf(jmCode, "%s", CString(inter.code, sizeof(inter.code)));
	//
	//	CString keys, jCode = jmCode;	jCode.TrimRight();
	//	if (jCode.IsEmpty())
	//		continue;

	//	jCode = "A" + jCode;
	//	keys.Format("%02d", idx++);
	//	profile.Write(section, keys, jCode);
 //     	}
	//fileH.Close();
}

CString CPane::GetGroupKeyByIndex(int nIndex)
{
	CString keys, firstKey, gName;

	CProfile profile(pkPortfolio);
	gName = profile.GetString("GROUPORDER", "00");
	if (gName.IsEmpty())
		return "";

	firstKey = "";

	while (!gName.IsEmpty())
	{
		int pos = gName.Find(';');
		if (pos == -1)	break;
		keys = gName.Left(pos++);
		gName = gName.Mid(pos);
		if (firstKey.IsEmpty())
			firstKey = keys;
		if (--nIndex < 0)
			return keys;
	}
	return firstKey;
}

LONG CPane::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case CTRL_SETTICKPOPUP:
		{
			switch (HIWORD(wParam))
			{
			case 0:
				{
					CSetTickPopup *pDlg = (CSetTickPopup *)lParam;
					m_arItem.RemoveAll();
					
					for (int ii = 0; ii < indexKeyCount; ii++)
					{
						if (pDlg->GetCheck(GetTicKey(ii)))
							m_arItem.Add(GetTicKey(ii));
					}
					
					save();
					ReSetPaneInfo();
					// 전체 재조회.. ㅜㅜ;
					(*m_axiscall)(AXI_SENDTICKINFO, 0, 0);
				}
				break;
			case 1:
				{
					(*m_axiscall)(AXI_TICKSETUP, 0, 0);
				}
				break;
			}
		}
		break;
	}
	return 0;
}

BOOL CPane::SetTickPopup()
{
#ifdef DF_USESTL_PANE
	std::unique_ptr<CSetTickPopup> pDlg = std::make_unique<CSetTickPopup>(this);
#else
	CSetTickPopup* pDlg = new CSetTickPopup(this);
#endif

	pDlg->m_point = CPoint(0, -4);
	ClientToScreen(&pDlg->m_point);
	if (pDlg->Create(IDD_SETTICK_POPUP))
	{
		for (int ii = 0; ii < m_arItem.GetSize(); ii++)
		{
			const int index = m_arItem.GetAt(ii);
			pDlg->SetCheck(index, TRUE);
		}
		
		pDlg->ShowWindow(SW_SHOW);
		pDlg->SetFocus();
		return TRUE;
	}
	return FALSE;
}

BOOL CPane::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	switch (nHitTest)
	{
	case HTCLIENT:
		SetCursor(m_hCursor);
		return TRUE;
	case HTHSCROLL:
	case HTVSCROLL:
		break;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CPane::OnMouseMove(UINT nFlags, CPoint point) 
{
	switch (m_kind)
	{
	case TKIND_INDEX:
	case TKIND_UPDOWN:
		{
			if (point.x < m_nTitleCx)
				m_hCursor = m_hCursorHand;
			else
				m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		}
		break;
	case TKIND_NEWS:
		{

			if (m_arRect.PtInRect(point))
			{
				//AfxMessageBox("asdf");
				set_ToolTipText("최근목록보기",true);
				if (!m_ToolTip || !IsWindow(m_ToolTip->GetSafeHwnd()))
					break;
				MSG	msg;
				
				msg.hwnd = m_hWnd;
				msg.message = WM_MOUSEMOVE;
				msg.wParam = 0;
				msg.lParam = 0;
				msg.pt = point;
			
				m_ToolTip->RelayEvent(&msg);
				

			}
		}
		break;
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CPane::MatchNewsIndex(int &index)
{
	// 7번은 reserved
	
	switch (index)
	{
		case 7:		index = 8;	break;	// 연합뉴스
		case 8:		index = 9;	break;	// 매일경제
		case 9:		index = 10;	break;	// 이투데이
		case 10:	index = 11;	break;	// 뉴스핌
		case 11:	index = 12;	break;	// 아시아 경제
		case 12:    index = 13; break;  // 헤럴드 경제
		case 13:    index = 15; break;  // IR GO
		case 14:    index = 16; break;  // 컨슈머타임즈
		case 15:    index = 17; break;  //modi  20240704 뉴스웨이 추가
	}
}

void CPane::DrawButton(CDC* pDC, CRect rc)
{
	if (!m_imgButton || !m_imgButton->m_hObject)
		return;

	CDC mdc;

	mdc.CreateCompatibleDC(pDC);
	CBitmap* oldBmp = mdc.SelectObject(&m_imgButton[0]);

	pDC->BitBlt(rc.left, rc.top, m_szButton.cx, m_szButton.cy, &mdc, 0, 0, SRCCOPY);

	mdc.SelectObject(oldBmp);
}

void CPane::LoadImage()
{
	m_imgButton = Axis::GetBitmap("TICKER");

	if (m_imgButton && m_imgButton->m_hObject)
	{
		BITMAP bm;
		m_imgButton->GetBitmap(&bm);
		m_szButton = CSize(bm.bmWidth / 3, bm.bmHeight);
	}
	else
	{
		m_szButton = CSize(0, 0);
	}
}

int CPane::GetButtonWidth(CDC* pDC)
{
	if (m_szButton.cx > 0)
		return m_szButton.cx;
	else
		return pDC->GetTextExtent(PANETITLEWIDTH).cx;
}

int CPane::SplitString(CString strData, CString strToken, CStringArray &astrResult, int imaxcnt)
{
	if(strToken == "") return 0;
	
	astrResult.RemoveAll();
	int iFind = 0;
	while(1)
	{
		iFind = strData.Find(strToken); 
		if(iFind < 0 ) 
		{
			if(!strData.IsEmpty() )astrResult.Add(strData);
			break;
		}
		
		astrResult.Add( strData.Left(iFind) ); 
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength() );  
		if(strData == "")break;
		if(imaxcnt < astrResult.GetSize() ) break;
	}
	
	return astrResult.GetSize(); 
}

void CPane::set_ToolTipText(CString text, BOOL bActivate)
{
	if (text.IsEmpty())	return;

	InitToolTip();
	if (m_ToolTip->GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		
		::GetWindowRect(m_hWnd, rectBtn);
		CString ddd; ddd.Format("%d, %d, %d, %d", rectBtn.left, rectBtn.right, rectBtn.top, rectBtn.bottom);
		//AfxMessageBox(ddd);
		m_ToolTip->AddTool(this, text);//, rectBtn, ID_TOOLTIP);
		/*
		if (!m_ToolTip->AddTool(this, text, rectBtn, ID_TOOLTIP))
		{
			AfxMessageBox("Fail");
		}else
		{
			AfxMessageBox(ddd);
		}
		*/
	}
	
	m_ToolTip->UpdateTipText(text, this);//, ID_TOOLTIP);
	m_ToolTip->Activate(bActivate);

	
}

void CPane::InitToolTip()
{
	if (!m_ToolTip)
	{
#ifdef DF_USESTL_PANE
		m_ToolTip = std::make_unique<CToolTipCtrl>();
#else
		m_ToolTip = new CToolTipCtrl();
#endif
	
		m_ToolTip->Create(GetDesktopWindow(), TTS_ALWAYSTIP);
		m_ToolTip->Activate(TRUE);
	}
}

void CPane::drawDataBG(CDC* pDC, int tp)
{
	if (tp==0)
	{
		CDC	 memDC;
		CRect	 cRc;
		const int	cx = m_nTitleCx = GetButtonWidth(pDC);
		GetClientRect(cRc);
		if (m_idx_back && m_idx_back->GetSafeHandle())
		{
			if (memDC.CreateCompatibleDC(pDC))
			{
				CBitmap* oldbuffBitmap = memDC.SelectObject(m_idx_back);
				if (oldbuffBitmap != (CBitmap *) 0)
				{
					BITMAP	 bm;
					m_idx_back->GetBitmap(&bm);
					//pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), 
					//		&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
					
					pDC->StretchBlt(cRc.left+cx, cRc.top, cRc.Width(), cRc.Height(), 
								&memDC, 0, HORZFIT_TOP, bm.bmWidth,
								bm.bmHeight-(HORZFIT_TOP+HORZFIT_BOTTOM), SRCCOPY);
					memDC.SelectObject(oldbuffBitmap);
				}
				memDC.DeleteDC();
			}
		}
		else
		{
			pDC->SetBkColor(COLOR_BACK);
			pDC->ExtTextOut(cRc.left+cx, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);
		}
	}
	else
	{
		CDC	 memDC;
		CRect	 cRc;
		//int	cx = m_nTitleCx = 18;
		GetClientRect(cRc);
		if (m_news_back && m_news_back->GetSafeHandle())
		{
			if (memDC.CreateCompatibleDC(pDC))
			{
				CBitmap* oldbuffBitmap = memDC.SelectObject(m_news_back);
				if (oldbuffBitmap != (CBitmap *) 0)
				{
					BITMAP	 bm;
					m_news_back->GetBitmap(&bm);
					//pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), 
					//		&memDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
					
					pDC->StretchBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), 
								&memDC, 0, HORZFIT_TOP, bm.bmWidth,
								bm.bmHeight-(HORZFIT_TOP+HORZFIT_BOTTOM), SRCCOPY);
					memDC.SelectObject(oldbuffBitmap);
				}
				memDC.DeleteDC();
			}
		}
		else
		{
			pDC->SetBkColor(COLOR_BACK);
			pDC->ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);
		}
	}
}

void CPane::SendPiboSise()
{
		if (m_arSymData.GetCount())
		{
			CString strHwnd;
			CString stmp;
			const	int	L_code = 12;
			const	int	codeCnt = 5;/* 2 - key, 3 - code count */
			CString	codes;
			int idx, len;
			const int	cnt = m_arSymData.GetCount();
			const int	sndL = codeCnt + L_code * cnt + 1;
			char* sndB = new char[sndL];
	
			memset(sndB, ' ', sndL);
			len = idx = codeCnt;
			sprintf(sndB, "%05d", cnt);
			strHwnd.Format("%d\t%s", this->m_hWnd, sndB);
			for (POSITION pos = m_arSymData.GetStartPosition(); pos; )
			{
				m_arSymData.GetNextAssoc(pos, codes, stmp);

			//	sprintf(&sndB[idx], "%s", (LPSTR)(LPCTSTR)codes);
				codes.Format("%12s", codes);
				idx += L_code;
				strHwnd += codes;
			}
	//		sendTR("PIBOSISE", sndB, idx, US_PASS, 'I');

			HWND m_hMain = AfxGetMainWnd()->m_hWnd;
			
			
			::SendMessage(m_hMain, WM_INTERMSG, MAKEWPARAM(MMSG_SEARCH_SISE, idx), (LPARAM)(LPSTR)(LPCTSTR)strHwnd);

		//	delete[] sndB;
		}
}//MMSG_SEARCH_SISE

LRESULT CPane::OnMsgFromMain(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case MMSG_SEARCH_SISE:
		{
			char* pdata = (char*)lParam;
		
			struct	_grid {
				char	code[12];		 /* 종목코드			 */
				char    hnam[20];                /* 종목명                       */
				char    curr[8];                 /* 현재가                       */
				char    diff[6];                 /* 전일대비      (9999V99)      */
				char    gvol[12];                /* 거래량                       */
			};

			struct  _interMod {
				char	keyf[2];		// ticker id
				char	nrec[3];		// count jcode
				struct	_grid	grid[1];	// data
			};

			CString		code, name, price, diff, gvol;
			_interMod* interMod = (struct _interMod*)pdata;
			int	cnt = atoi(CString(interMod->nrec, sizeof(interMod->nrec)));
			for (int ii = 0; ii < cnt; ii++)
			{
				code = CString(interMod->grid[ii].code, sizeof(interMod->grid[ii].code));	code.TrimRight();
				name = CString(interMod->grid[ii].hnam, sizeof(interMod->grid[ii].hnam));	name.TrimRight();
				price = CString(interMod->grid[ii].curr, sizeof(interMod->grid[ii].curr));	price.TrimRight();
				diff = CString(interMod->grid[ii].diff, sizeof(interMod->grid[ii].diff));	diff.TrimLeft();
				gvol = CString(interMod->grid[ii].gvol, sizeof(interMod->grid[ii].gvol));	gvol.TrimLeft();
				SetInter((char*)&interMod->grid[ii]);
			}
		}
		break;
		case MMSG_SEARCH_GROUPCODE:
		{
			char* pdata = (char*)lParam;

			CString stmp, sRet, code;
			CString strseq = CString(pdata, 2);
			CString strgname = CString(&pdata[2], 20);
			int cnt = atoi(CString(&pdata[22], 4));
			int parseL = 26;

			struct _jinfo* jinfo{};
			char* pCode;
			pCode = new char[sizeof(jinfo->code) + 1];
			sRet.Empty();

			if (cnt <= 0)
				return 0;

			for (int ii = 0; ii < cnt; ii++)
			{
				jinfo = (struct _jinfo*)&pdata[parseL];

				memset(pCode, 0x00, sizeof(jinfo->code) + 1);
				memcpy(pCode, jinfo->code, sizeof(jinfo->code));  // 종목코드[12]

				code.Format("%s", pCode);
				code.TrimRight();
			
#ifdef DF_USESTL_PANE
				auto inter = std::make_unique<_inter>();
				inter->Code = code;
				m_arInter.Add(inter.get());
				m_arInterByCode.SetAt(inter->Code, (CObject*)inter.get());
#else
				_inter* inter = new _inter;
				inter->Code = code;
				m_arInter.Add(inter);
				m_arInterByCode.SetAt(inter->Code, (CObject*)inter);
#endif
				m_arSymData.SetAt(inter->Code, "x");
				m_arSym.Add(inter->Code);

				parseL += sizeof(struct _jinfo);
			}
			SendPiboSise();
		}
		break;
		case MMSG_SEARCH_INTERGROUP:
		{
			m_arrInterGroup.RemoveAll();
		
			CString stmp, seq, sgname, keys;
			char* pdata = (char*)lParam;
			int cnt = atoi(CString(pdata, 4));
			if (cnt == 0) return 0;

			for (int ii = 0; ii < cnt; ii++)
			{
				keys.Format("%02d", ii);
				m_arrInterGroup.Add(keys);
			}

			int parseL = 4;
			struct _grSearch
			{
				char ngrs[2];
				char grseq[2];
				char gname[30];
			};

			struct _grSearch* stgr;
			char* pSeq, * pgame;
			pSeq = new char[sizeof(stgr->grseq) + 1];
			pgame = new char[sizeof(stgr->gname) + 1];

			for (int ii = 0; ii < cnt; ii++)
			{
				stgr = (struct _grSearch*)&pdata[parseL];
				memset(pSeq, 0x00, sizeof(stgr->grseq) + 1);
				memcpy(pSeq, stgr->grseq, sizeof(stgr->grseq));
				seq.Format("%s", pSeq);

				memset(pgame, 0x00, sizeof(stgr->gname) + 1);
				memcpy(pgame, stgr->gname, sizeof(stgr->gname));
				sgname.Format("%s", pgame);
				sgname.TrimRight();

				parseL += sizeof(struct _grSearch);

				m_arrInterGroup.SetAt(ii, sgname);
			}
		
			delete[] pSeq; pSeq = nullptr;
			delete[] pgame; pgame = nullptr;

			/////////////////////////////////////////////
			CPoint point;
			GetCursorPos(&point);
			CMenuXP		menu;
			menu.CreatePopupMenu();
			CStringArray arKey;
			for (int ii = 0; ii < m_arrInterGroup.GetSize(); ii++)
			{
				CString sGroup = m_arrInterGroup.GetAt(ii);
				menu.AppendMenuX(MF_STRING, ii + 1, sGroup);
			}
			menu.AppendMenuX(MF_SEPARATOR);
			menu.AppendMenuX(MF_STRING, 999, "상세설정");
			if ((int)m_select < m_arrInterGroup.GetSize())
				menu.CheckMenuItem(m_select + 1, MF_CHECKED | MF_BYCOMMAND);
			else
				menu.CheckMenuItem(1, MF_CHECKED | MF_BYCOMMAND);

			const int nRtn = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, point.x, point.y, this);

			if (nRtn == 999)
			{
				(*m_axiscall)(AXI_TICKSETUP, 0, 0);
			}
			else if (nRtn > 0 && (nRtn - 1) != (int)m_select)
			{
				CString section;
				section.Format("BAR_INFO_%02d", m_id);
				m_select = nRtn - 1;
				save();
				loadInter(section);
			//	ReSetPaneInfo();
			}
		}
		break;
	}

	return 0;
}