#include "stdafx.h"
#include "IB0000X1.h"
#include "SisePopup.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
 
CSisePopup::CSisePopup(CWnd *pParent) : CAxisExt(pParent)
{
	m_offsetY = 0;
	m_crBack = COLOR_DEF_POPUP_BACK;
	m_crFlash = COLOR_DEF_POPUP_FLASH;
	m_data = NULL;
	m_fields = NULL;
	DrawPrice = NULL;
	DrawDiff = NULL;
	m_bTracking = FALSE;
	m_bLBDown = FALSE;
	m_pOwner = NULL;
	m_pEdit = NULL;
	m_search = NULL;
	m_fields = NULL;
	m_bSimpleColor = TRUE;
	m_bNews = FALSE;
	m_nType = SISE_MODE;
	m_crSimple = 0;
	m_transparent = false;
	m_bFlash = false;
	m_type = CT_NONE;
}

CSisePopup::~CSisePopup()
{
}


BEGIN_MESSAGE_MAP(CSisePopup, CWnd)
	//{{AFX_MSG_MAP(CSisePopup)
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ENTERSIZEMOVE, OnEnterSizeMove)
	ON_MESSAGE(WM_EXITSIZEMOVE,  OnExitSizeMove)
	ON_MESSAGE(WX_EDIT_KEYPRESS, OnEditKeyPress)
	ON_MESSAGE(WX_EDIT_KILLFOCUS, OnEditKillFocus)
	ON_MESSAGE(WX_CHANGE_CODE, OnChangeCode)
	ON_CBN_SELCHANGE(IDC_CB_NEWS,OnSelChangeNews)
END_MESSAGE_MAP()

void CSisePopup::UpdateData( CMapStringToString *pd )
{
	m_data = pd;
	
	if(m_bNews == FALSE)
	{
		CDC *pDC = GetDC();
		Draw(pDC);
		ReleaseDC(pDC);
	}
}


BOOL CSisePopup::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CWnd::PreCreateWindow(cs);
}

void CSisePopup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if (!m_bmpBack.GetSafeHandle()) return;

	CDC mdc;
	mdc.CreateCompatibleDC(&dc);
	
	CBitmap *pOldBmp = mdc.SelectObject(&m_bmpBack);

// 	if(m_nType == SISE_MODE)
// 		Draw(&mdc);
// 	else
// 	{
// 		CString colName = "뉴 스";
// 
// 		CRect *pRC;
// 		pRC = &m_rects.ElementAt(4);
// 
// 		DrawName(&mdc,pRC,"",colName);
// 	}
	Draw(&mdc);

	dc.BitBlt(0, 0, m_rcBody.Width(), m_rcBody.Height(), &mdc, 0, 0, SRCCOPY);
	mdc.SelectObject(pOldBmp);

	mdc.DeleteDC();
}

void CSisePopup::Draw( CDC *pDC )
{
	CFont* pOldFont{};

	if(m_nType == SISE_MODE)
	{
		if (!m_fields || !m_data) return;
		if (m_fields->GetSize()==0) return;
		
		pOldFont = pDC->SelectObject(m_pFont);

		DWORD tick = GetTickCount();
		CString key, val;
		CRect* pRC{};
		const DWORD fcnt = m_fields->GetSize();

		if (m_bSimpleColor)
		{
			if (m_data->Lookup("033", val))
			{
				if (val.GetLength()==0) 
				{
					m_crSimple = m_crNormal;
				}
				else
				{
					const TCHAR sign = val.GetAt(0);
					switch(sign)
					{
					case '+': m_crSimple = m_crPlus;	break;
					case '-': m_crSimple = m_crMinus;	break;
					default : m_crSimple = m_crNormal;	break;
					}
				}
			}
			else m_crSimple = m_crNormal;
		}

		if(m_code.GetAt(0) == 'H')
		{
			//pDC->SetBkColor(RGB(255, 255, 255));

			CRect rc;
			GetClientRect(rc);

			CBrush	brush(RGB(255, 255, 255));
			
			pDC->FillRect(rc,&brush);

			CPen	cPen(PS_SOLID, 1, RGB(128,128,128));
			
			CPen* oldPen = pDC->SelectObject(&cPen);

			pDC->Rectangle(rc);
			
			brush.DeleteObject();

			//pDC->SelectObject(oldPen);
		}

		for(DWORD n=0; n<fcnt; ++n)
		{
			struct FIELD_INFO *fi = m_fields->GetAt(n);

			pRC = &m_rects.ElementAt(n);

			if (!m_data->Lookup(fi->rsym, val)) continue;

			if (m_hist.ElementAt(n)!=val)
			{
				m_flashs[n] = TRUE && m_bFlash;
				m_ticks[n] = tick;
				m_hist[n] = val;
			}
			
			if(m_code.GetAt(0) != 'H')
			{
				pDC->SetBkColor(m_crBack);
				pDC->FillSolidRect(m_frects[n], m_flashs[n] ? m_crFlash : m_crBack);
			}

			if (strcmp(fi->symb, "1022") == 0)
			{
				DrawName(pDC, pRC, fi->rsym, val);
			}
			else if (strcmp(fi->symb, "2023")==0 || strcmp(fi->symb, "2029")==0 || strcmp(fi->symb, "2030")==0 || strcmp(fi->symb, "2031")==0 )
			{
				if (DrawPrice)
					(this->*DrawPrice)(pDC, pRC, fi->rsym, val);
			}
			else if (strcmp(fi->symb, "2027")==0 || strcmp(fi->symb, "3101")==0 || strcmp(fi->symb, "3106")==0)
			{
				DrawVolumn(pDC, pRC, fi->rsym, val);
			}
			else if (strcmp(fi->symb, "2024")==0)
			{
				if (DrawDiff)
					(this->*DrawDiff)(pDC, pRC, fi->rsym, val);
			}
			else if (strcmp(fi->symb, "2033")==0)
			{
				DrawRate(pDC, pRC, fi->rsym, val);
			}
		}

		if(m_bNews)
		{
			pRC = &m_rects.ElementAt(4);

			GetClientRect(m_rectNews);
			//m_rectNews.top = pRc->top;
			m_rectNews.left = pRC->right;
			//m_rectNews.bottom = pRc->bottom;
			//m_rectNews.right = pRc->right;
			
			DrawNews(pDC,m_rectNews);
		}
	}
	else
	{
		pOldFont = pDC->SelectObject(m_pFont);

		CString colName = "뉴 스";
		
		CRect rect,rectName;
		
		GetClientRect(rect);

		CRect *pRC;
		pRC = &m_rects.ElementAt(4);

		pDC->SetBkColor(m_crBack);
		pDC->FillSolidRect(m_frects[4], m_flashs[4] ? m_crFlash : m_crBack);

		rectName = rect;

		rectName.top = pRC->top;
		rectName.right = pRC->right - 30;
		rectName.bottom = pRC->bottom;
		
		DrawName(pDC,&rectName,"",colName);
		
		rect.top = pRC->top;
		rect.left = pRC->right - 30;
		rect.bottom = pRC->bottom;

		m_rectNews = rect;

		pDC->FillSolidRect(rect, m_crBack);

		DrawNews(pDC,rect);
	}

	pDC->SelectObject(pOldFont);
}

void CSisePopup::OnSize(UINT nType, int cx, int cy) 
{
	GetClientRect(&m_rcBody);
	if (m_rcBody.Width() && m_rcBody.Height())
		MakeBackground();

	if(m_nType == NEWS_MODE)
	{
		CRect rect;
		
		GetClientRect(rect);
		
		CRect *pRC;
		pRC = &m_rects.ElementAt(4);
		
		rect.left = pRC->right - 30;
		rect.bottom = rect.bottom * 20;

		m_cbNews.MoveWindow(rect,FALSE);
	}

	CWnd::OnSize(nType, cx, cy);
}

void CSisePopup::MakeBackground()
{
	CDC mdc, *pDC = GetDC();
	CBitmap* pOldBmp{};
	
	if (m_bmpBack.GetSafeHandle()) 
		m_bmpBack.DeleteObject();

	CFont* pOldFont = pDC->SelectObject(m_pFont);
	const CSize sz = pDC->GetTextExtent("A1가");
	m_offsetY = (m_rcBody.Height()-sz.cy)/2;
	pDC->SelectObject(pOldFont);

	mdc.CreateCompatibleDC(pDC);
	m_bmpBack.CreateCompatibleBitmap(pDC, m_rcBody.Width(), m_rcBody.Height());
	
	pOldBmp = (CBitmap*)mdc.SelectObject(&m_bmpBack);
	
	mdc.FillSolidRect(0, 0, m_rcBody.Width(), m_rcBody.Height(), m_crBack);
	
	mdc.SelectObject(pOldBmp);
	mdc.DeleteDC();
	ReleaseDC(pDC);	
}

int CSisePopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetFont(m_pFont);

	//SetWindowLong(m_hWnd,GWL_EXSTYLE,WS_EX_NOACTIVATE);
	
	return 0;
}

void CSisePopup::SetCode( LPCSTR code )
{
	const int code_len = strlen(code);

	if      (code_len==6)      { m_type = CT_STOCK;  DrawPrice = &CSisePopup::DrawSPrice; DrawDiff = &CSisePopup::DrawSDiff; }
	else if (code_len==3)      { m_type = CT_JISU;   DrawPrice = &CSisePopup::DrawJPrice; DrawDiff = &CSisePopup::DrawJDiff; }
	else if (code_len==8)
	{
		if      (code[0]=='4') { m_type = CT_SPREAD; DrawPrice = &CSisePopup::DrawFPrice; DrawDiff = &CSisePopup::DrawFDiff; }
		else if (code[0]=='1') { m_type = CT_FUTURE; DrawPrice = &CSisePopup::DrawFPrice; DrawDiff = &CSisePopup::DrawFDiff; }
		else if (code[0]=='2') { m_type = CT_OPTION; DrawPrice = &CSisePopup::DrawFPrice; DrawDiff = &CSisePopup::DrawFDiff; }
		else if (code[0]=='3') { m_type = CT_OPTION; DrawPrice = &CSisePopup::DrawFPrice; DrawDiff = &CSisePopup::DrawFDiff; }
		else                   { m_type = CT_NONE;   DrawPrice = &CSisePopup::DrawFPrice; DrawDiff = &CSisePopup::DrawFDiff; }
	}
	else                       { m_type = CT_NONE;   DrawPrice = &CSisePopup::DrawSPrice; DrawDiff = &CSisePopup::DrawSDiff; }
	m_code = code;
}

CString CSisePopup::ApplyComma( CString &dat )
{
//	if ( dat.Find('.')==-1 ) 
	return "";		
}

void CSisePopup::DrawSPrice(CDC *pDC, CRect *pRc, LPCSTR key, CString &val)
{
	if (val.GetLength()==0)	return;

	const TCHAR sign = val.GetAt(0);
	switch(sign)
	{
	case '+': 
		pDC->SetTextColor(m_crPlus); 
		val.Delete(0);
		break;
	case '-': 
		pDC->SetTextColor(m_crMinus); 
		val.Delete(0);
		break;
	default : 
		pDC->SetTextColor(m_crNormal);
		break;
	}

	val = Str2CommaStr((LPSTR)(LPCSTR)val);
	
	pDC->DrawText( val, pRc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
}

void CSisePopup::DrawFPrice(CDC *pDC, CRect *pRc, LPCSTR key, CString &val)
{
	if (val.GetLength()==0) return;

	CString strJgga;
	const TCHAR sign = val.GetAt(0);
	
	if (m_type==CT_SPREAD && m_data->Lookup("934", strJgga))
	{
		// 스프레드 시/고/저가는 좀 다르네!
		if (sign=='+' || sign=='-') val.Delete(0);

		const double jgga = atof(strJgga);
		const double curr = atof(val);

		if (curr>jgga)		pDC->SetTextColor(m_crPlus);
		else if (curr<jgga)	pDC->SetTextColor(m_crMinus); 
		else				pDC->SetTextColor(m_crNormal);
	}
	else
	{
		switch(sign)
		{
		case '+': 
			pDC->SetTextColor(m_crPlus); 
			val.Delete(0);
			break;
		case '-': 
			pDC->SetTextColor(m_crMinus); 
			val.Delete(0);
			break;
		default : 
			pDC->SetTextColor(m_crNormal);
			break;
		}
	}
	pDC->DrawText( val, pRc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
}

void CSisePopup::DrawJPrice(CDC *pDC, CRect *pRc, LPCSTR key, CString &val)
{
	if (val.GetLength()==0) return;

	const TCHAR sign = val.GetAt(0);
	switch(sign)
	{
	case '+': 
		pDC->SetTextColor(m_crPlus); 
		val.Delete(0);
		break;
	case '-': 
		pDC->SetTextColor(m_crMinus); 
		val.Delete(0);
		break;
	default : 
		pDC->SetTextColor(m_crNormal);
		break;
	}	
	pDC->DrawText( val, pRc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
}

void CSisePopup::DrawSDiff(CDC *pDC, CRect *pRc, LPCSTR key, CString &val)
{
	if (val.GetLength()==0) return;
	if (val=="0") return;

	const TCHAR sign = val.GetAt(0);
	CString strSign;
	switch(sign)
	{
	case '1':
		pDC->SetTextColor(m_crPlus); 
		strSign = "↑";
		break;

	case '4':
		pDC->SetTextColor(m_crMinus); 
		strSign = "↓";
		break;
		
	case '2':
		pDC->SetTextColor(m_crPlus);
		strSign = "▲";
		break;
		
	case '5':
		pDC->SetTextColor(m_crMinus); 
		strSign = "▼";
		break;

	case '3':
	default :
		pDC->SetTextColor(m_crNormal);
		strSign = " ";
		break;
	}

	val.Delete(0);
	pDC->DrawText( Str2CommaStr((LPSTR)(LPCSTR)val), pRc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT );
	pDC->DrawText( strSign, pRc, DT_SINGLELINE|DT_VCENTER|DT_LEFT );
}

void CSisePopup::DrawFDiff(CDC *pDC, CRect *pRc, LPCSTR key, CString &val)
{
	if (val.GetLength()==0) return;
	if (val=="0.00") return;
	
	const TCHAR sign = val.GetAt(0);
	CString strSign;
	switch(sign)
	{
	case '+':
		pDC->SetTextColor(m_crPlus);
		strSign = "▲";
		val.Delete(0);
		break;
		
	case '-':
		pDC->SetTextColor(m_crMinus); 
		strSign = "▼";
		val.Delete(0);
		break;
		
	default :
		pDC->SetTextColor(m_crNormal);
		strSign = " ";
		break;
	}
	pDC->DrawText( val, pRc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT );
	pDC->DrawText( strSign, pRc, DT_SINGLELINE|DT_VCENTER|DT_LEFT );
}

void CSisePopup::DrawJDiff(CDC *pDC, CRect *pRc, LPCSTR key, CString &val)
{
	DrawFDiff(pDC, pRc, key, val);
}

void CSisePopup::DrawRate(CDC *pDC, CRect *pRc, LPCSTR key, const CString &val)
{
	if (val.GetLength()==0) return;
	if (val=="0") return;
	if (val=="0.00") return;
	
	if (m_bSimpleColor)
	{
		pDC->SetTextColor(m_crSimple);
	}
	else
	{
		const TCHAR sign = val.GetAt(0);
		switch(sign)
		{
		case '+':
			pDC->SetTextColor(m_crPlus);
			break;
			
		case '-':
			pDC->SetTextColor(m_crMinus); 
			break;
			
		default :
			pDC->SetTextColor(m_crNormal);
			break;
		}
	}
	pDC->DrawText( val+"%", pRc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT );
}

void CSisePopup::DrawVolumn(CDC *pDC, CRect *pRc, LPCSTR key, const CString &val)
{
	if (val.GetLength()==0 || val=="0") return;
	pDC->SetTextColor(m_bSimpleColor ? m_crSimple : m_crNormal);
	pDC->DrawText( Str2CommaStr((LPSTR)(LPCSTR)val), pRc, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);
}

void CSisePopup::DrawName( CDC *pDC, CRect *pRc, LPCSTR key, const CString &val)
{
	if(m_nType == SISE_MODE)
		pDC->SetTextColor(m_bSimpleColor ? m_crSimple : m_crNormal);
	else
		pDC->SetTextColor(RGB(0,0,0));

	m_name = val;

	pDC->DrawText( val, pRc, DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX );
}

void CSisePopup::DrawNews(CDC* pDC, CRect rect)
{
	//OutputDebugString("Draw NEWS\n");
//	CPoint	pt[4];
	CRgn	rgn;
	COLORREF color = pDC->SetTextColor(RGB(255, 255, 255));

	const int	nmode = pDC->SetBkMode(TRANSPARENT);
	
// 	pt[0] = rect.TopLeft();	
// 	pt[1] = CPoint(rect.left, rect.bottom);
// 	pt[2] = rect.BottomRight();
// 	pt[3] = CPoint(rect.right, rect.top);
// 	rgn.DeleteObject();
	CBrush	brush(RGB(128, 128, 255));

	if(m_nType == SISE_MODE)
	{
		//rgn.CreatePolygonRgn(pt, 4, ALTERNATE);
		//pDC->FillRgn(&rgn, &brush);
		pDC->FillRect(rect,&brush);
	}
	else
	{
		color = pDC->SetTextColor(RGB(0,0,0));
	}
	//rect.OffsetRect(3, 2);
	pDC->DrawText(m_newsTiTle, rect, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);
	pDC->SetBkMode(nmode);
	pDC->SetTextColor(color);
	//rgn.DeleteObject();
	brush.DeleteObject();
}

void CSisePopup::SetFields( CArray<struct FIELD_INFO*, struct FIELD_INFO*> *pa, VIEW_TYPE vtype )
{
	m_fields = pa;
	m_flashs.RemoveAll();
	m_ticks.RemoveAll();
	m_hist.RemoveAll();
	m_rects.RemoveAll();
	m_frects.RemoveAll();

	int n=0;
	CRect rc;
	int x_sum = 0, y_sum = 0;

	for(n=0; n<pa->GetSize(); ++n) 
	{
		m_flashs.Add(FALSE);
		m_ticks.Add(0xFFFFFFFF);
		m_hist.Add("");

		switch(vtype)
		{
		case VT_VERTICAL  : 
			rc.SetRect(0, y_sum, MAX_FIELD_WIDTH, y_sum+(pa->ElementAt(n)->width ? POPUP_HEIGHT : 0));
			y_sum += pa->ElementAt(n)->width ? POPUP_HEIGHT : 0;
			break;
		case VT_HORIZONTAL: 
		default: 
			rc.SetRect(x_sum, 0, x_sum+pa->ElementAt(n)->width, POPUP_HEIGHT);
			x_sum += pa->ElementAt(n)->width;
			break;
		}
		
		m_frects.Add(rc);
		rc.DeflateRect(CSize(3,0));
		m_rects.Add(rc);
		rc.DeflateRect(CSize(-3,0));
	}	
}

LRESULT CSisePopup::OnNcHitTest(CPoint point) 
{
	const UINT nHit = CWnd::OnNcHitTest(point);
	
	if (nHit==HTCLIENT) return HTCAPTION;
	else                return nHit;	
}

BOOL CSisePopup::PreTranslateMessage(MSG* pMsg) 
{
	return CWnd::PreTranslateMessage(pMsg);
}

void CSisePopup::HoverWindow()
{
	CLayer::SetTransparent(this, 100);
}

void CSisePopup::LeaveWindow()
{
	CLayer::SetTransparent(this, m_transparent, &m_crBack);
}

void CSisePopup::SetTransparent( DWORD tp )
{
	m_transparent = tp;
	CLayer::SetTransparent(this, tp, &m_crBack);
}

void CSisePopup::UpdateFlash( DWORD tick )
{
	const int nSize = m_flashs.GetSize();
	for(int n=0; n<nSize; ++n)
	{
		if (m_flashs[n])
		{
			if ((tick-m_ticks[n])>200)
			{
				CDC *pDC = GetDC();
				CFont *pOldFont = pDC->SelectObject(m_pFont);
				
				struct FIELD_INFO *fi = m_fields->ElementAt(n);
				CRect *pRC = &m_rects.ElementAt(n);
				CString val = m_hist.ElementAt(n);

				pDC->FillSolidRect(m_frects[n], m_crBack);
				if (strcmp(fi->symb, "1022") == 0)
				{
					DrawName(pDC, pRC, fi->rsym, val);
				}
				else if (strcmp(fi->symb, "2023")==0 || strcmp(fi->symb, "2029")==0 || strcmp(fi->symb, "2030")==0 || strcmp(fi->symb, "2031")==0 )
				{
					if (DrawPrice)
						(this->*DrawPrice)(pDC, pRC, fi->rsym, val);
				}
				else if (strcmp(fi->symb, "2027")==0 || strcmp(fi->symb, "3101")==0 || strcmp(fi->symb, "3106")==0)
				{
					DrawVolumn(pDC, pRC, fi->rsym, val);
				}
				else if (strcmp(fi->symb, "2024")==0)
				{
					if (DrawDiff)
						(this->*DrawDiff)(pDC, pRC, fi->rsym, val);
				}
				else if (strcmp(fi->symb, "2033")==0)
				{
					DrawRate(pDC, pRC, fi->rsym, val);
				}

				pDC->SelectObject(pOldFont);
				ReleaseDC(pDC);
				m_flashs[n] = FALSE;
			}
		}
	}
}

void CSisePopup::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonDown(nFlags, point);
}

void CSisePopup::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	if(m_code.GetAt(0) == 'H')
		return;

	CMenu menu, almenu[4], intermenu;
	if (menu.CreatePopupMenu())
	{
		m_pOwner->SendMessage(WX_SET_TRACKING, (WPARAM)0, (LPARAM)FALSE);

		for(int idx=0; idx<4; ++idx)
		{
			if (almenu[idx].CreatePopupMenu())
			{
				for(int n=0; n<m_fields->GetSize(); ++n)
				{
					// 2001~20XX -> 오름차순
					// 3001~30XX -> 내림차순
					if (m_fields->ElementAt(n)->sort)
						almenu[idx].AppendMenu(MF_STRING, 2000+(1000*idx)+n, m_fields->ElementAt(n)->name);
				}
			}
		}

		if(intermenu.CreatePopupMenu())
		{
			for(int n=0; n<((CMapWnd*)m_pOwner)->m_gpnm.GetSize(); ++n)
			{
				CString intername = ((CMapWnd*)m_pOwner)->m_gpnm[n];
				
// 				CString s;
// 				s.Format("INTERST : %s\n",intername);
// 				OutputDebugString(s);

	 			intermenu.AppendMenu(MF_STRING,6000+n,((CMapWnd*)m_pOwner)->m_gpnm[n]);
			}
		}
		
// 		menu.AppendMenu(MF_POPUP, (UINT)almenu[0].m_hMenu, "가로정렬(오름차순)");
// 		menu.AppendMenu(MF_POPUP, (UINT)almenu[1].m_hMenu, "가로정렬(내림차순)");
// 		menu.AppendMenu(MF_POPUP, (UINT)almenu[2].m_hMenu, "세로정렬(오름차순)");
// 		menu.AppendMenu(MF_POPUP, (UINT)almenu[3].m_hMenu, "세로정렬(내림차순)");
		menu.AppendMenu(MF_POPUP, (UINT)intermenu.m_hMenu, "관심종목");
		menu.AppendMenu(MF_STRING, 1080, "뉴스창 보기");
		menu.AppendMenu(MF_MENUBARBREAK);
		menu.AppendMenu(MF_STRING, 1098, "종목추가");
		menu.AppendMenu(MF_POPUP, (UINT)almenu[2].m_hMenu, "세로정렬(오름차순)");
		menu.AppendMenu(MF_POPUP, (UINT)almenu[3].m_hMenu, "세로정렬(내림차순)");
		menu.AppendMenu(MF_MENUBARBREAK);
		menu.AppendMenu(MF_STRING, 1097, "설정창 보기");
		if(((CMapWnd*)m_pOwner)->m_bMiniMode)
		{
			menu.AppendMenu(MF_STRING, 1081, "Hot Trading 전환");
		}
		menu.AppendMenu(MF_MENUBARBREAK);
		menu.AppendMenu(MF_STRING, 1099, m_name + " 닫기");

		if(!((CMapWnd*)m_pOwner)->m_bMiniMode)
			menu.AppendMenu(MF_STRING, 1100, "모두 닫기");

		if(((CMapWnd*)m_pOwner)->m_bMiniMode)
			menu.AppendMenu(MF_STRING, 1101, "HTS 종료");

		CString path;
		path.Format("%s\\image\\TITLE_BROWN.BMP", Variant(homeCC,""));

		HBITMAP hBitmap;

		hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), path, 
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		std::unique_ptr<CBitmap> bmp = std::make_unique<CBitmap>();
		bmp->Attach(hBitmap);

		menu.AppendMenu(MF_OWNERDRAW | MF_BYCOMMAND, 9000, bmp.get());
		
		const int mnuId = menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, point.x, point.y, this, CRect(0,0,100,100));
		
		m_pOwner->SendMessage(WX_SET_TRACKING, (WPARAM)0, (LPARAM)TRUE);

		if      (mnuId==1098) m_pOwner->PostMessage(WX_ADD_NEWCODE, (WPARAM)0, (LPARAM)this);
		else if (mnuId==1099) m_pOwner->PostMessage(WX_CLOSE_WINDOW, (WPARAM)0, (LPARAM)this);
		else if (mnuId==1080) m_pOwner->PostMessage(WX_NEWSVIEW, (WPARAM)0, (LPARAM)this);
		else if (mnuId==1081) m_pOwner->PostMessage(WX_HOTTRADE, (WPARAM)0, (LPARAM)this);
		else if (mnuId==1100) m_pOwner->PostMessage(WX_CLOSE_WINDOW, (WPARAM)0, (LPARAM)NULL);
		else if (mnuId>=2000 && mnuId<3000) m_pOwner->PostMessage(WX_ALIGN_H_UPPER, (WPARAM)mnuId-2000, (LPARAM)this);
		else if (mnuId>=3000 && mnuId<4000) m_pOwner->PostMessage(WX_ALIGN_H_LOWER, (WPARAM)mnuId-3000, (LPARAM)this);
		else if (mnuId>=4000 && mnuId<5000) m_pOwner->PostMessage(WX_ALIGN_V_UPPER, (WPARAM)mnuId-4000, (LPARAM)this);
		else if (mnuId>=5000 && mnuId<6000) m_pOwner->PostMessage(WX_ALIGN_V_LOWER, (WPARAM)mnuId-5000, (LPARAM)this);
		else if (mnuId==1097) m_pOwner->PostMessage(WX_VIEW_PROPERTY, (WPARAM)0, (LPARAM)this);
		else if (mnuId==1101) m_pOwner->PostMessage(WD_HTSCLOSEFORMINI, (WPARAM)0, (LPARAM)0);
		else if (mnuId>=6000 && mnuId<7000) m_pOwner->PostMessage(WX_INTEREST, (WPARAM)mnuId-6000, (LPARAM)this);
		
		almenu[0].DestroyMenu();
		almenu[1].DestroyMenu();
		menu.DestroyMenu();
	}

	if(m_nType == NEWS_MODE)
	{
		m_cbNews.ShowWindow(SW_HIDE);
	}
		
	CWnd::OnNcRButtonDown(nHitTest, point);
}

LRESULT CSisePopup::OnExitSizeMove( WPARAM wParam, LPARAM lParam )
{
	CRect rc;
	GetWindowRect(&rc);
	m_pOwner->PostMessage(WX_LASTPOS_UPDATE, rc.left, rc.top);
	return m_pOwner->SendMessage(WX_SET_TRACKING, (WPARAM)0, (LPARAM)TRUE);
}

LRESULT CSisePopup::OnEnterSizeMove( WPARAM wParam, LPARAM lParam )
{
	return m_pOwner->SendMessage(WX_SET_TRACKING, (WPARAM)0, (LPARAM)FALSE);
}

void CSisePopup::SetFlash( BOOL bFlash )
{
	m_bFlash = bFlash;	
}

void CSisePopup::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	//OutputDebugString("DBLCLICK\n");
	if(m_nType == SISE_MODE)
	{
		if (m_fields->GetSize()<4) return;
		if ( strcmp(m_fields->ElementAt(4)->rsym, "022") != 0 ) return;

		EditCode();
	}
	else
	{
		m_cbNews.ShowWindow(SW_SHOW);
	}

	CWnd::OnNcLButtonDblClk(nHitTest, point);
}

void CSisePopup::OnDestroy() 
{
	if (m_pEdit!=NULL) { 
		m_pEdit->DestroyWindow(); 
		m_pEdit=nullptr; 
	}
	CWnd::OnDestroy();	
}

void CSisePopup::Search(CString text)
{
	if (text.IsEmpty()) return;

	TRACE("-------------------" + text + "-------------------\n");

	int size{};	
	CString key;
	CStringArray* obj = NULL;
	CStringArray rCode, rHnam;
	
	key.Format("%.*s", IsDBCSLeadByte(text[0]) ? 2 : 1, (LPCSTR)text);
	if (!m_search->Lookup(key, obj))
	{
		key.MakeUpper();
		if (!m_search->Lookup(key, obj))
		{
			obj = NULL;
			return;
		}
	}

	m_sCodes.RemoveAll();
	m_sNames.RemoveAll();

	text.MakeUpper();

	if (obj!=NULL)
	{
		size = obj->GetSize();
		for(int n=0; n<size; ++n)
		{
			CString sdat = obj->ElementAt(n);
			sdat.MakeUpper();
			if (sdat.Find(text, 0)>=0)
			{
				CString name, code;
				const CString &data = obj->ElementAt(n);
				const int pos = data.Find('|');
				const int len = data.GetLength();
				const TCHAR gubn = data.GetAt( len-1 );
				if (gubn=='@')	// name|code
				{
					name = data.Left(pos);
					code = data.Mid(pos+1, len-pos-2);
				}
				else if (gubn=='#')	// code|name
				{
					code = data.Left(pos);
					name = data.Mid(pos+1, len-pos-2);
				}
				m_sCodes.Add(code);
				m_sNames.Add(name);
				TRACE("[%s][%s]\n", code, name);
			}
		}
	}
	else if (text.GetLength()>1)
	{
		CString key;
		POSITION pos = m_search->GetStartPosition();
		while(pos)
		{
			m_search->GetNextAssoc(pos, key, obj);
			if (!obj) continue;
			size = obj->GetSize();
			for(int n=0; n<size; ++n)
			{
				CString sdat = obj->ElementAt(n);

				if (sdat.GetAt(sdat.GetLength()-1)=='#') continue;	// code|name 형식은 삭제

				sdat.MakeUpper();
				if (sdat.Find(text, 0)>=0)
				{
					CString name, code;
					const CString &data = obj->ElementAt(n);
					const int pos = data.Find('|');
					const int len = data.GetLength();
					const TCHAR gubn = data.GetAt( len-1 );
					if (gubn=='@')	// name|code
					{
						name = data.Left(pos);
						code = data.Mid(pos+1, len-pos-2);
					}
					else if (gubn=='#')	// code|name
					{
						code = data.Left(pos);
						name = data.Mid(pos+1, len-pos-2);
					}
					m_sCodes.Add(code);
					m_sNames.Add(name);
					TRACE("[%s][%s]\n", code, name);
				}
			}
		}
	}

	if (m_sCodes.GetSize()==0) return;
	m_pEdit->SetCodeList(&m_sCodes, &m_sNames);
}

LRESULT CSisePopup::OnEditKeyPress( WPARAM wParam, LPARAM lParam )
{
	if (!m_search) return 0;
	if (!m_pEdit) return 0;
	if (!m_pEdit->GetSafeHwnd()) return 0;

	LPCSTR text = (LPCSTR)lParam;

	if (m_oldSearch!=text)
	{
		Search(text);
		m_oldSearch = text;
	}

	return 0;
}

LRESULT CSisePopup::OnEditKillFocus( WPARAM wParam, LPARAM lParam )
{
	return 0;
}

LRESULT CSisePopup::OnChangeCode( WPARAM wParam, LPARAM lParam )
{
	LPCSTR code = (LPCSTR)wParam;
	LPCSTR name = (LPCSTR)lParam;
	
	const LRESULT ret = m_pOwner->SendMessage(WX_CHANGE_CODE, (WPARAM)code, (LPARAM)this);
	if (ret==0)
	{
		m_pEdit->ShowWindow(SW_HIDE);
	}
	else
	{
		m_pEdit->SetSel(0, -1);
	}


	return 0;
}

void CSisePopup::EditCode()
{
	if (m_pEdit==NULL)
	{
		m_pEdit = std::make_unique<CFindEdit>();
		m_pEdit->Create(ES_LEFT|WS_CHILD|WS_VISIBLE, m_frects[4], this, IDC_CODESEARCH);
		m_pEdit->SetFont(m_pFont);
	}
	
	CRect *pRc = &m_rects[4];
	m_pEdit->SetWindowPos(0, pRc->left, pRc->top, pRc->Width(), pRc->Height(), SWP_SHOWWINDOW);
	
	m_pEdit->SetWindowText("");
	m_pEdit->SetFocus();
}

void CSisePopup::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState==WA_ACTIVE && m_pOwner)
	{
		CRect rc;
		GetWindowRect(&rc);
		m_pOwner->PostMessage(WX_LASTPOS_UPDATE, rc.left, rc.top);
	}
}

void CSisePopup::SetBkColor( COLORREF rgb )
{
	m_crBack = rgb;
	SetTransparent(m_transparent);
	Invalidate(TRUE);
}

void CSisePopup::SetFlashColor( COLORREF rgb )
{
	m_crFlash = rgb;	
	Invalidate(TRUE);
}

void CSisePopup::UpdateNews( CString title,CString strValue )
{
	if(m_nType == SISE_MODE)
	{
		m_newsTiTle = title;
		m_newsData = strValue;
		m_bNews = TRUE;
		//OutputDebugString("UPDATE NEWS\n");
		CDC *pDC = GetDC();
		Draw(pDC);
		ReleaseDC(pDC);
	}
	else
	{
		if(m_cbNews)
		{
			title.TrimLeft();
			title.TrimRight();

			m_newsTiTle = title;
			m_newsData = strValue;

			m_cbNews.InsertString(0,title);
			m_cbNews.SetCurSel(0);

			if(m_cbNews.GetCount() > 20)
			{
				CString last = (CString)(LPCTSTR)m_cbNews.GetItemData(20);

				m_mapNewsHis.RemoveKey(last);
				m_cbNews.DeleteString(20);
			}

			m_mapNewsHis.SetAt(title,strValue);
		}

		Invalidate(TRUE);
	}
}

void CSisePopup::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bNews)
	{
		//OutputDebugString("MOUSE MOVE\n");
		if(m_rectNews.PtInRect(point))
		{
			//OutputDebugString("NEWXX CALL\n");
			CString str;
			str.Format("IBNEWSXX /t /p5 /d %s", m_newsData);
			SetPopup(str,FALSE);

			m_bNews = FALSE;
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CSisePopup::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	ScreenToClient(&point);
	if(m_bNews)
	{
		CString s;
		//s.Format("TOP : %d	LEFT : %d	BOTTOM : %d	RIGHT : %d\n",m_rectNews.top,m_rectNews.left,m_rectNews.bottom,m_rectNews.right);
		//OutputDebugString(s);
		//s.Format("POINT X : %d	Y: %d\n",point.x,point.y);
		//OutputDebugString(s);
		if(m_rectNews.PtInRect(point))
		{
			//OutputDebugString("NEWXX CALL\n");
			CString str;
			str.Format("IBNEWSXX /t /p5 /d %s", m_newsData);
			SetPopup(str,FALSE);
			
			m_bNews = FALSE;

			Invalidate();
		}
	}
	
	CWnd::OnNcMouseMove(nHitTest, point);
}

void CSisePopup::SetViewType( SISE_TYPE type )
{
	m_nType = type;

	CRect rect;

	GetClientRect(rect);
	
	CRect *pRC;
	pRC = &m_rects.ElementAt(4);

	rect.left = pRC->right - 30;
	rect.bottom = rect.bottom * 20;

	m_cbNews.Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|CBS_SIMPLE, rect, this, IDC_CB_NEWS);
	m_cbNews.SetFont(m_pFont);

	m_cbNews.ShowWindow(SW_HIDE);
}

void CSisePopup::DoDataExchange(CDataExchange* pDX) 
{	
	CWnd::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSisePopup)
	DDX_Control(pDX,IDC_CB_NEWS,m_cbNews);
	//}}AFX_DATA_MAP
}

void CSisePopup::OnSelChangeNews()
{
	//OutputDebugString("OnSelChangeNews\n");
	CString str,strData,key;

	m_cbNews.GetWindowText(key);
	//OutputDebugString(key);
	key.TrimRight();
	key.TrimLeft();

	m_mapNewsHis.Lookup(key,strData);
	//OutputDebugString(strData);
	str.Format("IBNEWSXX /t /p5 /d %s", strData);
	SetPopup(str,FALSE);
	//OutputDebugString("OnSelChangeEnd\n");
}

void CSisePopup::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	if(m_nType == NEWS_MODE)
	{
		if(m_cbNews)
		{
			m_cbNews.ShowWindow(SW_HIDE);
		}
	}
	
	CWnd::OnNcLButtonUp(nHitTest, point);
}

void CSisePopup::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	if(m_nType == NEWS_MODE)
	{
		ScreenToClient(&point);
				CString s;
		// 		s.Format("POINT X : %d	Y : %d\n",point.x,point.y);
		// 		OutputDebugString(s);
		// 		s.Format("rect X : %d	Y : %d\n",m_rectNews.top,m_rectNews.left);
		// 		OutputDebugString(s);
		
		if(m_rectNews.PtInRect(point))
		{
			CString strData,str;
			
			m_mapNewsHis.Lookup(m_newsTiTle,strData);
			
			str.Format("IBNEWSXX /t /p5 /d %s", strData);
			SetPopup(str,FALSE);
		}

// 		s.Format("TITLE : [%s]\n",m_newsTiTle);
// 		OutputDebugString(s);
// 		
// 		CString key,val;
// 		POSITION pos = m_mapNewsHis.GetStartPosition();
// 		while(pos!=NULL)
// 		{
// 			m_mapNewsHis.GetNextAssoc(pos, key, val);
// 			
// 			s.Format("KEY : [%s]\n",key);
// 			OutputDebugString(s);
// 			s.Format("VALUE : [%s]\n",val);
// 			OutputDebugString(s);
// 		}
	}
	
	CWnd::OnNcLButtonDown(nHitTest, point);
}

void CSisePopup::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
 	CDC * pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	const CRect rect = lpDrawItemStruct->rcItem;
	//CBitmap* bitmap = (CBitmap*)lpDrawItemStruct->itemData;
// 	
//	pDC->FillSolidRect(0,0,100,30, RGB(255,255,0));

	CString path;
	path.Format("%s\\image\\TTILE_BROWN.BMP", Variant(homeCC,""));

	//OutputDebugString(path);
	
	BITMAP hBitmap{};
	std::unique_ptr<CBitmap> bmp = std::make_unique<CBitmap>();
	
	bmp->m_hObject = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), path, 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	bmp->GetBitmap(&hBitmap);
 
	CDC mdc;
	mdc.CreateCompatibleDC(pDC);
	CBitmap* oldBmp = mdc.SelectObject(bmp.get());
	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &mdc, 0, 0, SRCCOPY);
	mdc.SelectObject(oldBmp);
	mdc.DeleteDC();
	
	CWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
