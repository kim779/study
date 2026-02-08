// Pane.cpp : implementation file
//

#include "stdafx.h"
#include "IB100000.h"
#include "Pane.h"
#include "usetup.h"

#include "base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <afxtempl.h>

#define	BHEIGTH		25
#define	BWIDTH		66
#define GAP		2
#define	EXPANDW		30

#define LT_COLOR	RGB(171, 168, 153)
#define RB_COLOR	RGB(115, 113, 100)

#define DIRECT_NONE	0
#define DIRECT_LEFT	1
#define DIRECT_RIGHT	2

#define SPACE	3
/////////////////////////////////////////////////////////////////////////////
// CPane

CPane::CPane(CWnd* parent, CBase* base, CString home, int type, int pos)
{
	m_parent = parent;
	m_base = base;
	m_home = home;
	m_pos = pos;
	m_type = type;

	m_iTabBottom = 0;
	m_iHSpace = m_iVSpace  = SPACE;
	m_pExpendRect = CRect(0, 0, 0, 0);
	m_bTabClick = m_bExpendClick = FALSE;
	m_arTabList.RemoveAll();
	m_iSelectTab = -1;
	m_iTabExBtnSize = EXPANDW;
	Initialize();
}

CPane::~CPane()
{
}


BEGIN_MESSAGE_MAP(CPane, CWnd)
	//{{AFX_MSG_MAP(CPane)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPane message handlers

void CPane::Initialize(int type, int pos)
{
	m_pos = pos;
	m_type = type;
	Initialize();
}

void CPane::Initialize()
{	
	//**m_exM = EX_NONE;
	m_iTop = 1;

	switch(m_pos)
	{
		case 0:
			m_iLeft = 1;
			m_iHSpace = SPACE;
			break;
		case 1:
			m_iLeft = SPACE+1;
			m_iHSpace = SPACE;
			break;

		case 2:
			m_iTop = SPACE;
			m_iLeft = 1;
			m_iHSpace = m_iVSpace = SPACE+SPACE;
			break;
		case 3:
			m_iTop = SPACE;
			m_iLeft = 1;
			m_iVSpace = SPACE+SPACE;
			m_iHSpace = SPACE;
			break;

	}

	m_iTabBottom =  BHEIGTH + m_iTop +  SPACE; 
	m_screen = NULL;

	m_cur = 0;
	m_key = 0;

	_fontR font;
	font.name = "굴림";
	font.point = 9;
	font.italic = false;
	font.bold = FW_NORMAL;
	m_font = (CFont *) m_parent->SendMessage(WM_USER, MAKEWPARAM(getFONT, 0), (long) &font);

	if (GetType() == TYPE_3)
	{
		m_stat[0] = STAT_HIDE;
		m_stat[1] = STAT_HIDE;
	}
	else	
	{
		m_stat[0] = STAT_NONE;
		m_stat[1] = STAT_NONE;
	}

	m_rect		= CRect(0, 0, 0, 0);
	m_dRc		= CRect(0, 0, 0, 0);

	m_bColor	= m_base->GetSkinColor(60); // 화면바탕
	m_sColor	= m_base->GetSkinColor(0);
	m_btnColor	= m_base->GetSkinColor(7);
	m_sbtnColor	= m_base->GetSkinColor(33);
	m_nosize	= false;

	m_index = 0;


	m_TabLineColor = m_base->GetIndexColor(TAB_BORDER_INDEX);
	/*
	CString sImgPath;
	sImgPath.Format("%s\\image\\axpanel0_lt.bmp",  m_base->m_home);
	m_hRoundBitmap[0] =  ((CBitmap*)m_parent->SendMessage(WM_USER, getBITMAP, (LPARAM)sImgPath.operator LPCTSTR()))->operator HBITMAP();

	sImgPath.Format("%s\\image\\axpanel0_rt.bmp",  m_base->m_home);
	m_hRoundBitmap[1] =  ((CBitmap*)m_parent->SendMessage(WM_USER, getBITMAP, (LPARAM)sImgPath.operator LPCTSTR()))->operator HBITMAP();
	
	sImgPath.Format("%s\\image\\axpanel0_lb.bmp",  m_base->m_home);
	m_hRoundBitmap[2] =  ((CBitmap*)m_parent->SendMessage(WM_USER, getBITMAP, (LPARAM)sImgPath.operator LPCTSTR()))->operator HBITMAP();
	
	sImgPath.Format("%s\\image\\axpanel0_rb.bmp",  m_base->m_home);
	m_hRoundBitmap[3] =  ((CBitmap*)m_parent->SendMessage(WM_USER, getBITMAP, (LPARAM)sImgPath.operator LPCTSTR()))->operator HBITMAP();
*/
	LoadRoundBitmap();
}

void CPane::ChangePallete()
{
	m_TabLineColor = m_base->GetIndexColor(TAB_BORDER_INDEX);
	LoadRoundBitmap();
	Invalidate();
}

void CPane::LoadRoundBitmap()
{
	CString sFile;
	sFile.Format("%s\\tab\\PALETTE.INI", m_base->m_home);

	CString sPalette = _T("");
	int readL = 0; 
	char readB[1024]; 
	readL = GetPrivateProfileString(_T("General"), _T("Palette"), _T("Blue"), readB, sizeof(readB), sFile);
	if (readL > 0)	sPalette.Format("%s", readB);

	CString sImage;
	sImage.Format("%s\\Image\\axpanel%s1_",  m_base->m_home, sPalette);
	for(int i=0; i<4; i++)
	{
		if(m_hRoundBitmap[i])
			DeleteObject(m_hRoundBitmap[i]);
	}

	CIB100000App* pApp = (CIB100000App*)AfxGetApp(); 
	m_hRoundBitmap[0] = m_hRoundBitmap[4] = (HBITMAP)::LoadImage( pApp->m_hInstance , sImage+"lt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[1] = m_hRoundBitmap[5] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"rt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[2] = m_hRoundBitmap[6] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"lb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[3] = m_hRoundBitmap[7] = (HBITMAP)::LoadImage(pApp->m_hInstance, sImage+"rb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );

}

void CPane::Release()
{
	if (IsWindow(m_parent->m_hWnd))
	{
		m_parent->SendMessage(WM_USER, MAKEWPARAM(detachMAP, m_key), 0);
	}
	if (m_screen)
	{
		m_screen->DestroyWindow();
		//delete m_screen;
		m_screen = nullptr;
	}
	m_list.RemoveAll();
}

void CPane::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(!m_base) return;

	CRect rc, rc2;
	GetClientRect(rc);
	if( m_list.GetSize() <= 0) 
		dc.FillSolidRect(rc, m_base->m_contentsbgColor); 

	const COLORREF color[] = {RGB(255, 0,0), RGB(255, 0,255), RGB(0, 0,255), RGB(0, 255,0)};
	dc.FillSolidRect(rc, m_base->m_bgColor); 

	CPen linepen(PS_SOLID, 1, m_base->m_contentslineColor);
	CBrush brush(m_base->m_contentsbgColor);
	
	CPen* pOldPen = dc.SelectObject(&linepen); 
	CBrush* pOldBrush = dc.SelectObject(&brush); 

	const int ipoint = 3;
	rc2 = rc;
	if(m_pos == 0)
	{
		rc2.DeflateRect(0,0,0,0); 
	}
	else if(m_pos == 1)
	{
		rc2.DeflateRect(SPACE,0,0,0); 
	}
	else if(m_pos == 2)
	{
		m_iHSpace = m_iVSpace = SPACE+SPACE;
		rc2.DeflateRect(0,SPACE,SPACE,SPACE); 
	}
	else if(m_pos == 3)
	{
		rc2.DeflateRect(0,SPACE,0,SPACE); 
	}
	else 
		rc2.DeflateRect(SPACE,SPACE,SPACE,SPACE); 


	dc.Rectangle(rc2); 
	dc.SelectObject(pOldPen); 
	dc.SelectObject(pOldBrush); 
	DeleteObject(pOldPen);
	DeleteObject(pOldBrush);

	
	DrawCornerRoundBitmap(&dc, rc2);
	DrawButton(&dc); //==>AX TAB
}
void CPane::DrawButton(CDC* pDC)
{
	if( m_list.GetSize() <= 0) return;
	if (!pDC) return;
//	m_pExpendRect.SetRectEmpty();
//	CClientDC	dc(this);
//	if (!pDC)	pDC = &dc;
	CRect	cRc, bRc, eRc, xRc;
	CString	str;
	const int itop = m_iTop + SPACE;
	const int ileft = m_iLeft + SPACE;

	GetClientRect(cRc);
	cRc.left = ileft;
	m_iTabBottom = cRc.bottom = BHEIGTH + itop; 

	eRc.CopyRect(&cRc);
	eRc.right = cRc.right - m_iHSpace;
	const int iright = cRc.right - m_iHSpace - SPACE;
	switch (GetType())
	{
		default:
		case TYPE_1:
			eRc.left = eRc.right - EXPANDW*2;
			break;
	}

	pDC->SetBkColor(m_base->GetSkinColor(64));
	pDC->SetBkMode(TRANSPARENT);
	CFont*	font = pDC->SelectObject(m_font);

	bool expand = false;
	int ilinestart = 100;


	const int itabspace = 5;
	m_iexLastIdx = m_list.GetSize();
	m_arTabList.RemoveAll();
	for (int jj = m_index, ii = 0 ; ii < m_list.GetSize(); ii++, jj++)
	{
		if (expand)	break;

		bRc.SetRect(BWIDTH*ii+itabspace, itop, BWIDTH*(ii+1), cRc.bottom);
		if(jj == m_index) bRc.left = ileft;
		bRc.right += GAP+1;

		if (bRc.right >= eRc.left)
		{
			m_iexLastIdx = ii;
			expand = true;
			eRc.left -= ((int)(EXPANDW/2))*2 -1;
//			break;
		}

		if (m_list.GetSize() < jj+1)
		{
		//	xRc.CopyRect(&bRc);
		//	pDC->FillSolidRect(xRc, GetSysColor(COLOR_BTNFACE));
			break;
		}

		
		str = GetShortName(m_list.GetAt(jj));
		if (m_base->ImageReady())
		{

	//		if(bRc.right > iright) bRc.right = iright;
		//	if(!expand)
			{
				m_base->DrawButton(pDC, bRc, jj == m_cur, str);
				if (jj == m_cur)
					ilinestart = bRc.right ;
				m_arTabList.Add(TabList(bRc, jj) );
			}
		//	pDC->Rectangle(bRc); 
		}
		else
		{
			if (jj == m_cur)
			{
				xRc.CopyRect(&bRc);
				pDC->FillSolidRect(xRc, m_sbtnColor);
				

				const COLORREF color = pDC->SetTextColor(RGB(255, 255, 255));
				pDC->Draw3dRect(xRc, RB_COLOR, RB_COLOR);
				bRc.OffsetRect(1, 1);
				pDC->DrawText(str, bRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				pDC->SetTextColor(color);
			}
			else
			{
				xRc.CopyRect(&bRc);
				pDC->FillSolidRect(xRc, m_btnColor);
				pDC->Draw3dRect(xRc, RB_COLOR, RB_COLOR);
				xRc.left++;	xRc.top++;
				pDC->Draw3dRect(xRc, RGB(255, 255, 255), RB_COLOR);
				bRc.OffsetRect(1, 1);
				pDC->DrawText(str, bRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			}
		}
	}
	pDC->SelectObject(font);

	CPen linepen1(PS_SOLID, 1, m_TabLineColor);
	CPen* pOldPen = pDC->SelectObject(&linepen1); 

	if(ilinestart > BWIDTH + SPACE)
	{
		pDC->MoveTo(ileft,  m_iTabBottom);
		pDC->LineTo(ilinestart-BWIDTH+GAP ,  m_iTabBottom);
	}

	pDC->MoveTo(ilinestart,  m_iTabBottom);
	pDC->LineTo(cRc.right-m_iHSpace ,  m_iTabBottom);
	
	pDC->SelectObject(pOldPen); 
	DeleteObject(pOldPen);

	if(expand)
	{
	//	eRc = cRc;
		eRc.top = itop;
		eRc.bottom = m_iTabBottom;

//		eRc.left = eRc.right - EXPANDW - 1 + SPACE;
		eRc.right = cRc.right - m_iHSpace;
		eRc.left = eRc.right -  EXPANDW;

//		eRc.left = cRc.right -  EXPANDW;
//		eRc.right = eRc.left + EXPANDW;

		CRect rc1 = bRc;
		if(m_pos == 2)
		{
			
			rc1.left = cRc.right -	SPACE;
			rc1.right = cRc.right;
			rc1.top = SPACE;
			rc1.bottom = m_iTabBottom + 1;
			pDC->FillSolidRect(rc1, m_base->m_bgColor); 

//			eRc.left = rc1.left -  EXPANDW;
//			eRc.right = eRc.left + EXPANDW;
		}

		
	//	pDC->Rectangle(m_pExpendRect); 

		m_pExpendRect.CopyRect(&eRc);
		rc1 = eRc;
		rc1.left = eRc.right - 1;
		rc1.right = cRc.right;
		pDC->FillSolidRect(rc1, m_base->m_contentsbgColor); 
		DrawExpand(eRc, EXSTAT_NONE);
	//	m_pExpendRect.right = m_pExpendRect.left + m_iTabExBtnSize;
	}
	else m_iTabExBtnSize = 0;
}

void CPane::DrawExpand(CRect eRc, UINT expandStat)
{
	CDC*	pDC = GetDC();

	//if (expand)
	{
		m_iTabExBtnSize = m_base->DrawExpand(pDC, eRc, expandStat);
		m_iTabExBtnSize = max(m_iTabExBtnSize, EXPANDW);
	}
	ReleaseDC(pDC);
}

void CPane::RedrawTabButton()
{
	CRect rc;
	GetClientRect(&rc);
	rc.bottom = m_iTabBottom+1;
	InvalidateRect(rc);
}

void CPane::OnLButtonDown(UINT nFlags, CPoint point) 
{
	bool bexec = false;
	if( m_iTabExBtnSize > 0 && m_pExpendRect.PtInRect(point) )
	{
		const int iw = m_iTabExBtnSize/ 2;
		
		CRect rcL = m_pExpendRect;
		rcL.right = rcL.left + iw;
		CRect rcR = m_pExpendRect;
		rcR.left = rcL.right+1; 
		rcR.right = rcR.left + iw;

		
		if (rcL.PtInRect(point))
		{
			DrawExpand(m_pExpendRect, EXSTAT_LEFT_DOWN);
			if (m_index > 0)
			{
				m_index--;
				RedrawTabButton();
				
			}
			bexec = true;
			m_bExpendClick = TRUE;
		}
		else 
		{
			if (rcR.PtInRect(point))
			{
				DrawExpand(m_pExpendRect, EXSTAT_RIGHT_DOWN);
				if (m_index + 1 <= m_list.GetSize() - m_iexLastIdx)
//				if (m_index  < m_list.GetSize())
				{
					m_index++;
					RedrawTabButton();
				}

				bexec = true;
				m_bExpendClick = TRUE;
			}
		}
	}
	else
	{
		CString ss;
		for(int i=0; i<m_arTabList.GetSize(); i++)
		{
			if (m_arTabList.GetAt(i).rc.PtInRect(point))
			{
				m_bTabClick = TRUE;
				bexec = true;
				m_cur = m_arTabList.GetAt(i).iTab;

				
				RedrawTabButton();
				if(m_iSelectTab != m_cur)
					SelectTabbutton(m_cur);
//				else
//					AfxMessageBox( m_list.GetAt(m_iSelectTab) );
				break;
			}
			else
			{
		//		ss.Format("OnLButtonDown:%d-%d-%d-%d",  point.x, point.y, m_arTabList.GetAt(i).rc.left, m_arTabList.GetAt(i).rc.top);
		//		AfxMessageBox(ss);
			}
		}

		if(!bexec)
		{
//			ss.Format("OnLButtonDown:%d",  m_arTabList.GetSize());
//			AfxMessageBox(ss);
		}
	}

	if(bexec)
	{
	//	SetCapture();
	}

}

void CPane::OnLButtonUp(UINT nFlags, CPoint point) 
{
//	if( GetCapture())
	{
	//	ReleaseCapture();
		if(m_bExpendClick)
		{
			DrawExpand(m_pExpendRect, EXSTAT_NONE);
			m_bExpendClick = FALSE;
		}

		if(m_bTabClick)
		{
			m_bTabClick = FALSE;
			
		}
	}
}

void CPane::SelectTabbutton(int index, bool bInit)
{
	m_iSelectTab = index;
	if(m_list.GetSize() <= index || index < 0) return;
	CString mapN = m_list.GetAt(index);

	//맵사이즈가 윈도우사이즈에 맞춰서 로드되지않음==>맵윈도우 종료후 재생성 필요--씨발
	if(m_screen)
	{
		m_parent->SendMessage(WM_USER, MAKEWPARAM(detachMAP, m_key), 0);
		m_screen->DestroyWindow();
		//delete m_screen;
		m_screen = nullptr;
	}

	CRect rc{};
	GetClientRect(&rc);
	const int cx = rc.Width();
	const int cy = rc.Height(); 
	const int itop = m_iTabBottom + 1;
	const int ileft = m_iLeft + SPACE;

	//m_screen = new CWnd();
	m_screen = std::make_unique<CWnd>();

	if (!m_screen->Create(NULL, "SCREEN", WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, (int)m_screen.get()))
	{
		return;
	}

	int type = m_parent->SendMessage(WM_USER, MAKEWPARAM(typeDLL, 0), (long) mapN.operator LPCTSTR());
	if( !(type & vtypeNRM))
		type = type | vtypeNRM;
	

	m_key = m_parent->SendMessage(WM_USER, MAKEWPARAM(attachMAP, type), (long) m_screen.get());

	const BOOL ret = m_parent->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_key), (long) mapN.operator LPCTSTR());
	//멥 리사이즈---Create 할때 보낸 사이즈로 사이즈 안잡힘=======띠용
	m_screen->MoveWindow(ileft, itop, cx-ileft - m_iHSpace, cy - itop - m_iVSpace);

	if( !bInit)
	{
//		CString ss;
//		ss.Format("%d[%s] %d * %d",  type, mapN, cx-ileft - m_iHSpace, cy - itop - m_iVSpace);
//		AfxMessageBox(ss);
	}

//	m_screen->RedrawWindow();
//	m_screen->Invalidate();
//	RedrawTabButton();
	m_screen->SetFocus();
	SetTimer(TIMER_WAIT, 20, NULL);
	SetIndex();
}

void CPane::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_screen)
	{
		/*
		CRect	cRc;
		CString	mapN = m_list.GetAt(m_cur);
		int size = m_parent->SendMessage(WM_USER, MAKEWPARAM(sizeMAP, 0), (long)(char *) mapN.operator LPCTSTR());
		int type = m_parent->SendMessage(WM_USER, MAKEWPARAM(typeDLL, 0), (long)(char *) mapN.operator LPCTSTR());
		//m_screen->MoveWindow(0, BHEIGTH+GAP, cRc.Width(), cRc.Height() - (BHEIGTH+GAP));
		GetClientRect(cRc);
		int	cx, cy, vwTYPE;
		cx = cRc.Width();
		cy = cRc.Height() - (BHEIGTH+GAP);
		switch (type)
		{
		default:
			vwTYPE = vtypeMSK & type;
			if (vwTYPE != vtypeRSZ && vwTYPE != vtypeREX)
			{
				if (cx > LOWORD(size))	cx = LOWORD(size);
				if (cy > HIWORD(size))	cy = HIWORD(size);
				break;
			}
			else
				if (cy > 600)
					cy -= (BHEIGTH + GAP + 1);
		case vtypeNRM:
			break;
		}
		*/
//CString	mapN = m_list.GetAt(m_cur);
//		int size = m_parent->SendMessage(WM_USER, MAKEWPARAM(sizeMAP, 0), (long)(char *) mapN.operator LPCTSTR());
//		CString ss;
//		ss.Format("%d", HIWORD(size));
//		AfxMessageBox(ss);

	//	if (!m_nosize)
		{
			const int itop = m_iTabBottom + 1;
			const int ileft = m_iLeft + SPACE;
			m_screen->MoveWindow(ileft, itop, cx-ileft - m_iHSpace, cy-itop - m_iVSpace, FALSE);
		}

	//m_screen->MoveWindow(0, BHEIGTH, cx, cy-BHEIGTH);

		SetIndex();
	}
}

void CPane::SetIndex()
{

	CRect	cRc, bRc, eRc;

	GetClientRect(cRc);
	cRc.bottom = BHEIGTH+GAP; 
	bRc.CopyRect(&cRc);	bRc.DeflateRect(1, 1);
	eRc.CopyRect(&cRc);	eRc.DeflateRect(0, GAP);
	eRc.OffsetRect(-GAP, 0);

	switch (GetType())
	{
	default:
	case TYPE_1:
		eRc.left = eRc.right - EXPANDW*2;
		break;
	}

	int ii = 0;
	for (ii = 0; ii < 2; ii++)
	{
		if (m_stat[ii] != STAT_HIDE)
			continue;
		eRc.left += EXPANDW;
	}

	int	matchN{};
	bool	expand = false;
	for (ii = 0; ii < m_list.GetSize(); ii++)
	{
		bRc.SetRect(BWIDTH*ii, 0, BWIDTH*(ii+1), cRc.bottom);
		bRc.right += GAP+1;
		bRc.DeflateRect(GAP, GAP);
		if (bRc.right < eRc.left)
			continue;
		matchN = ii;
		expand = true;
		eRc.left -= ((int)(EXPANDW/2))*2 -1;
		break;
	}

	if (expand)
	{
		const int gap = m_list.GetSize() - matchN;
		if (m_index > gap)	m_index = gap;
	}
	else
		m_index = 0;
}



void CPane::DrawDirect(CRect eRc, int dir, bool push)
{
	return;
	CDC*	pDC = GetDC();
	CRect	xRc{};
	CFont*	font = pDC->SelectObject(m_font);

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillSolidRect(eRc, GetSysColor(COLOR_BTNFACE));

	xRc.CopyRect(&eRc);
	pDC->Draw3dRect(xRc, RB_COLOR, RB_COLOR);
	xRc.left++;	xRc.top++;

	if (push)
	{
		xRc.right--;	xRc.bottom--;
		pDC->Draw3dRect(xRc, RGB(148, 146, 131), RGB(255, 255, 255));
	}
	else	pDC->Draw3dRect(xRc, RGB(255, 255, 255), RB_COLOR);

	const CPoint	cPoint = eRc.CenterPoint();
	if (dir == DIRECT_LEFT)
	{
		pDC->MoveTo(cPoint.x-1, cPoint.y-1);
		pDC->LineTo(cPoint.x-2, cPoint.y-1);

		pDC->MoveTo(cPoint.x, cPoint.y - 2);
		pDC->LineTo(cPoint.x, cPoint.y + 1);

		pDC->MoveTo(cPoint.x+1, cPoint.y-3);
		pDC->LineTo(cPoint.x+1, cPoint.y+2);
	}
	else
	{
		pDC->MoveTo(cPoint.x - 1, cPoint.y - 3);
		pDC->LineTo(cPoint.x - 1, cPoint.y + 2);

		pDC->MoveTo(cPoint.x, cPoint.y - 2);
		pDC->LineTo(cPoint.x, cPoint.y + 1);

		pDC->MoveTo(cPoint.x-1, cPoint.y-1);
		pDC->LineTo(cPoint.x+2, cPoint.y-1);
	}
	pDC->SelectObject(font);
	ReleaseDC(pDC);
}

BYTE CPane::GetType()
{
	return m_type;
}

void CPane::LoadScreen()
{
	if (m_list.GetSize() <= m_cur || m_cur < 0)
		return;

//	CString	mapN = m_list.GetAt(m_cur);
	SelectTabbutton(m_cur, true);

/*
	CString	mapN = m_list.GetAt(m_cur);
	CRect	cRc;
	GetClientRect(cRc);
	int imapWidth = cRc.Width();
	int imapHeight = cRc.Height() -  BHEIGTH - GAP;

	if (!m_screen)
	{
		m_screen = new CWnd();//CScreen();
		if (!m_screen->Create(NULL, "SCREEN", WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS, CRect(0), this, (int)m_screen))
		{
			delete m_screen;
			m_screen = NULL;
			m_key = 0;
			return;
		}
//		m_screen->MoveWindow(0, BHEIGTH+GAP, cRc.Width(), cRc.Height() - (BHEIGTH+GAP));
	//	m_screen->MoveWindow(0, BHEIGTH+GAP, imapWidth, imapHeight);
	}

	int type = m_parent->SendMessage(WM_USER, MAKEWPARAM(typeDLL, 0), (long)(char *) mapN.operator LPCTSTR());
	int size = m_parent->SendMessage(WM_USER, MAKEWPARAM(sizeMAP, 0), (long)(char *) mapN.operator LPCTSTR());
	int cx = cRc.Width();
	int cy = cRc.Height() - (BHEIGTH+GAP);

	if ((type & vtypeMSK) != vtypeRSZ)
	{
		if (cx > LOWORD(size))	cx = LOWORD(size);
		if (cy > HIWORD(size))	cy = HIWORD(size);
	}

//	m_screen->MoveWindow(0, BHEIGTH+GAP, cx -1, cy -1);

	if (m_key > 0)		
		m_parent->SendMessage(WM_USER, MAKEWPARAM(detachMAP, m_key), 0);

	if( !(type & vtypeNRM))
		type = type | vtypeNRM;
	m_key = m_parent->SendMessage(WM_USER, MAKEWPARAM(attachMAP, type), (long) m_screen);

	if (m_screen)	
		m_screen->SetFocus();

	
	m_parent->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_key), (long)(char *) mapN.operator LPCTSTR());

	cx = cRc.Width();
	cy = cRc.Height();
	int itop = m_iTabBottom + 1;
	int ileft = m_iLeft + SPACE;
	m_screen->MoveWindow(ileft, itop, cx-ileft - m_iHSpace, cy - itop - m_iVSpace);

	//m_screen->MoveWindow(0, 0, imapWidth, imapHeight);
	if (m_screen)
	{
		m_nosize = false;
		m_screen->RedrawWindow();
	}
	*/
}

void CPane::SaveLog(CString sData)
{
	CString sFile;
	CFile pFile;

	sFile = "e:\\1000List.log";
	

	if (pFile.Open(sFile, CFile::modeReadWrite))
	{
		const ULONGLONG pos = pFile.SeekToEnd();
		pFile.Seek((long) pos, CFile::end);
	}
	else
	{
		if(!pFile.Open(sFile, CFile::modeCreate|CFile::modeReadWrite)) return;
	}

	CString strTime, strWrite;
	//SYSTEMTIME	tm;
//	GetLocalTime(&tm);

	strWrite.Format("%s\r\n", sData);

	pFile.Write(strWrite, strWrite.GetLength() );
	pFile.Close(); 
}

void CPane::LoadScreen(CString mapN)
{}

void CPane::SetSkinColor(COLORREF color, bool draw)
{
	m_bColor	= m_base->GetSkinColor(64); // 화면바탕
	m_sColor	= m_base->GetSkinColor(0);
	m_btnColor	= m_base->GetSkinColor(7);
	m_sbtnColor	= m_base->GetSkinColor(33);

	if (draw)	
		RedrawTabButton();
//		DrawButton();
}

void CPane::SetScreen(CString dat)
{
	int	pos{};
	CString	str;

	while (!dat.IsEmpty())
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			dat.TrimLeft();
			dat.TrimRight();
			m_list.Add(dat);
			break;
		}

		str = dat.Left(pos);
		str.TrimLeft();
		str.TrimRight();
		m_list.Add(str);
		dat = dat.Mid(pos+1);
	}
}

CString	CPane::GetShortName(CString mapN)
{
	char	buf[256];
	CString	str, file, sname;

	file.Format("%s\\tab\\종합list.ini", m_home);
	DWORD dw = GetPrivateProfileString("STOCK", mapN, "", buf, sizeof(buf), file);
	if (dw <= 0)
	{
		dw = GetPrivateProfileString("FOPTION", mapN, "", buf, sizeof(buf), file);
		if (dw <= 0)	return mapN.Mid(2, 4);
	}
	
	sname = buf;
	int pos = sname.Find(';');
	if (pos == -1)	
		return mapN.Mid(2, 4);

	sname = sname.Mid(pos+1);
	sname.TrimLeft(); sname.TrimRight();

	if (sname.IsEmpty())	
		return mapN.Mid(2, 4);

	pos = sname.Find(';');
	if (pos > 0)
		return sname.Left(pos);
	else
		return sname;
}

void CPane::ChangeFont(int pointfont, CString fontName)
{
//	m_parent->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_key), (long) mapN.operator LPCTSTR());
//	m_pAccountCtrl->SendMessage(WM_USER, MAKEWPARAM(DLL_SETFONT, pointfont), (LPARAM)lszname);

	_fontR font;

	font.name = "굴림";

	if (!fontName.IsEmpty())
		sprintf(font.name, fontName, fontName.GetLength());

	font.point = pointfont;
	font.italic = false;
	font.bold = FW_NORMAL;
	m_font = (CFont *) m_parent->SendMessage(WM_USER, MAKEWPARAM(getFONT, 0), (long) &font);

	RedrawTabButton();

	if (m_screen)	
		m_screen->RedrawWindow();
}

void CPane::ChangeSize(int cx, int cy)
{
	if (m_screen)
	{
		m_nosize = true;
		m_screen->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE|SWP_SHOWWINDOW);
	}
}

CSize CPane::GetMapInfo()
{
	CString	mapN = m_list.GetAt(m_cur);
	const int	size = m_parent->SendMessage(WM_USER, MAKEWPARAM(sizeMAP, 0),
				(long)(char *) mapN.operator LPCTSTR());
	return CSize(LOWORD(size), HIWORD(size));

}

bool CPane::IsExpand(CPoint point, CRect eRc)
{
	CRect	bRc;

	bRc.CopyRect(&eRc);
	if (m_index > 0)
	{
		bRc.right = bRc.left + EXPANDW/2;
		if (bRc.PtInRect(point))
			return true;
		bRc.OffsetRect(EXPANDW/2-1, 0);
		bRc.right = bRc.left + EXPANDW/2;
		if (bRc.PtInRect(point))
			return true;
		bRc.OffsetRect(EXPANDW/2, 0);
	}

	bRc.right = bRc.left + EXPANDW;
	if (m_stat[0] != STAT_HIDE)
	{
		if (bRc.PtInRect(point))
			return true;
		bRc.OffsetRect(EXPANDW, 0);
	}
	
	if (m_stat[1] != STAT_HIDE)
	{
		if (bRc.PtInRect(point))
			return true;
	}
	return false;
}
/*
LRESULT CPane::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_SYSCOMMAND)
	{
		if (wParam == SC_MAXIMIZE)
		{
			AfxMessageBox("a");
			return TRUE;
		}
	}	
	
	return CWnd::WindowProc(message, wParam, lParam);
}
*/
void CPane::OnDestroy() 
{
	Release();	
	CWnd::OnDestroy();
}



void CPane::DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap)
{
	HDC hDC = ::CreateCompatibleDC (NULL);
	SelectObject (hDC, hBitmap); //icon
//	::BitBlt( pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(),	hDC, 0, 0, SRCCOPY);
	::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);

	::DeleteDC (hDC);
}

void CPane::DrawCornerRoundBitmap(CDC* pDC, CRect rc)
{
	const int iPoint = 5;
//	DrawBitmap(pDC, CRect(rc.left, rc.top, rc.left+iPoint, rc.top +iPoint), m_hRoundBitmap[0]);
//	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.top, rc.right, rc.top +iPoint), m_hRoundBitmap[1]);
//	DrawBitmap(pDC, CRect(rc.left, rc.bottom-iPoint, rc.left+iPoint, rc.bottom), m_hRoundBitmap[2]);
//	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.bottom-iPoint, rc.right, rc.bottom), m_hRoundBitmap[3]);

	DrawBitmap(pDC, CRect(rc.left, rc.top, rc.left+iPoint, rc.top +iPoint), m_hRoundBitmap[0]);
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.top, rc.right, rc.top +iPoint), m_hRoundBitmap[1]);
	DrawBitmap(pDC, CRect(rc.left, rc.bottom-iPoint, rc.left+iPoint, rc.bottom), m_hRoundBitmap[2]);
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.bottom-iPoint, rc.right, rc.bottom), m_hRoundBitmap[3]);
}

void CPane::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIMER_WAIT)
	{
		KillTimer(nIDEvent);

		CString sKey=_T("LoadMapKey"), sData;
		sData.Format("%d", m_key);
		m_base->SendDataToMap( m_key, sKey, sData);
	}

	CWnd::OnTimer(nIDEvent);
}

