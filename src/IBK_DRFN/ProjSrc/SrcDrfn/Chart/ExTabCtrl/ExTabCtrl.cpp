// ExTabCtrl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ExTabCtrl.h"
#include "../../inc/ExTabCtrl.h"
#include "../../inc/ISkinColorMng.h"

#include "MemberDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define			BACK_COLOR		RGB(245, 245, 245)
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CExTabCtrlApp

BEGIN_MESSAGE_MAP(CExTabCtrlApp, CWinApp)
	//{{AFX_MSG_MAP(CExTabCtrlApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExTabCtrlApp construction

CExTabCtrlApp::CExTabCtrlApp()
{
	m_pCurMember = NULL;
}

long CExTabCtrlApp::ActionControl(LPVOID lpAction)
{
	LPST_ACTIONCONTROL lpActionControl = (LPST_ACTIONCONTROL)lpAction;
	return 0L;
}

void CExTabCtrlApp::ExitTabCtrl(LPVOID lpPointer)
{
	m_pCurMember = (CMember*)lpPointer;
	delete m_pCurMember;
	m_pCurMember = NULL;
}

LPVOID CExTabCtrlApp::InitTabCtrlDraw(LPVOID lpPointer, BOOL bUseDefSkin/* = FALSE*/)
{
	HWND hMainWnd;
	AfxGetHanaroMainWnd(hMainWnd);

	CMember* pMember = new CMemberDraw(lpPointer, (ISkinColorManager*)::SendMessage(hMainWnd, RMSG_INTERFACE, 6, 0), bUseDefSkin);
	return pMember;
}

void CExTabCtrlApp::PreSubclassWindow() 
{	
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	pMember->m_pTabCtrl->CTabCtrl::PreSubclassWindow();

	NONCLIENTMETRICS nclim;
	nclim.cbSize=sizeof(NONCLIENTMETRICS);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
							sizeof(NONCLIENTMETRICS),
							&nclim,0);
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

	pMember->m_hMenuFont= ::CreateFontIndirect(&lfont);
	
	lfont.lfWeight = FW_BOLD; 
	pMember->m_hBoldFont= ::CreateFontIndirect(&lfont);
	
	//Create pens for drawing CLBTabCtrl
	COLORREF clr = ::GetSysColor(COLOR_3DHILIGHT);	
	pMember->m_hPenLight = ::CreatePen(PS_SOLID,1,pMember->m_clrNormal);
	clr=::GetSysColor(COLOR_3DDKSHADOW);
	pMember->m_hPenShadow = ::CreatePen(PS_SOLID,1,pMember->m_clrLine);
	clr=::GetSysColor(COLOR_3DSHADOW);
	pMember->m_hPenItemShadow = ::CreatePen(PS_SOLID,1,RGB(253, 205, 1));

	//Look for msctls_ukpdown32 control and if it is present, modify its size,
	//position and subclass it to CLBSpinButtonCtrl, which have autodisabling
	//arrow buttons.
	//The msctls_updown32 control exists when CLBTabCtrl is not in stacked mode.
	CWnd* pWnd = pMember->m_pTabCtrl->GetWindow(GW_CHILD);
	while(pWnd)
	{
		char buf[]="msctls_updown32";
		int nRet=::GetClassName(pWnd->m_hWnd,buf,sizeof(buf)/sizeof (buf[0]));
		if(nRet && strcmp(buf,"msctls_updown32"))
		{
			pWnd = pWnd->GetWindow(GW_HWNDNEXT);
		}
		else
		{
			//The msctls_updown32 control is found.
			//
			pWnd->GetWindowRect(&pMember->m_rectUpDn);
			pMember->m_pTabCtrl->ScreenToClient(&pMember->m_rectUpDn);

			//Update size and position of msctls_updown32 control
			pMember->m_rectUpDn.DeflateRect(3,2);
			pMember->m_rectUpDn.OffsetRect(3,5);
			pWnd->MoveWindow(&pMember->m_rectUpDn);
			
			//Subclass common msctls_updown32 control to
			//my CLBSpinButtonCtrl, which have autodisabling arrow buttons.
//			pMember->m_Spin.SubclassWindow(pWnd->m_hWnd);

			pWnd=0;
		}
	}

	pMember->m_nActiveItem=-1;
	SetDefaultItemRect();
}

void CExTabCtrlApp::SetDefaultItemRect()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	CRect rctItem;
	pMember->m_pTabCtrl->GetItemRect(0,&rctItem);
	pMember->m_pTabCtrl->GetClientRect(&pMember->m_rctPage);
	pMember->m_rctPage.top=rctItem.bottom;
}

BOOL CExTabCtrlApp::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CExTabCtrlApp::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);

	//Set current font equal to default menu font.
	dc.SelectObject(pMember->m_hMenuFont);
	char buf[255];
	int nCurItem = lpDrawItemStruct->itemID;

	TCITEM tcitem;
	//Get text of item.
	tcitem.mask=TCIF_TEXT;
	tcitem.pszText =buf;
	tcitem.cchTextMax=sizeof(buf);
	pMember->m_pTabCtrl->GetItem(nCurItem,&tcitem);
	
	CRect rect=lpDrawItemStruct->rcItem;
	rect.OffsetRect(1,4);

	// Draw Item text.
	if(!pMember->m_rectUpDn.IsRectEmpty() && 
			lpDrawItemStruct->rcItem.right >= pMember->m_rectUpDn.left)
	{ 
		//The current item is cut on the right side.
		if(nCurItem == pMember->m_nActiveItem)
		{
			//If current item is active, then we should decrease its width
			//as so, the length of the visible text should be the same as when
			//this tab will be inactive. 
			if(pMember->m_nActiveItem!=(int)FindLeftDownItem(pMember))
					rect.left+=3;
			TCHITTESTINFO thi;
			thi.pt=CPoint(lpDrawItemStruct->rcItem.right+5,
						  lpDrawItemStruct->rcItem.bottom-4);
			int nPos = pMember->m_pTabCtrl->HitTest(&thi);
			if(nPos>=0)
				rect.right-=3;
		}

		//Decrease the length of the visible text, since
		//the right side is cut.
		rect.right=pMember->m_rectUpDn.left-SHIFT_FROM_CUT_TO_SPIN-1;
		rect.left+=4;
		::DrawText(lpDrawItemStruct->hDC,buf,-1,&rect,DT_LEFT);
	}
	else
	{
		int nOldBM = dc.SetBkMode(TRANSPARENT);
		if(nCurItem != pMember->m_nActiveItem)
		{
			dc.SetTextColor(RGB(0, 0, 0));//20090504 SJ_KIM Tab Text Color
//			dc.SetBkColor(pMember->m_clrNormal);
			::DrawText(lpDrawItemStruct->hDC,buf,-1,&rect,DT_CENTER);
		}
		else
		{
			dc.SetTextColor(RGB(255, 255, 255)); //20090504 SJ_KIM Tab Text Color
//			dc.SetBkColor(pMember->m_clrSelect);
			dc.SelectObject(pMember->m_hBoldFont);
			::DrawText(lpDrawItemStruct->hDC,buf,-1,&rect,DT_CENTER);
			dc.SelectObject(pMember->m_hMenuFont);
		}
		dc.SetBkMode(nOldBM);
	}	
	dc.Detach();
}

void CExTabCtrlApp::InsertItem(int nItem, LPSTR lpszItem, UINT nTab/* = EXTAB_NORMAL*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	int nSize = pMember->m_pTabCtrl->GetItemCount();
	if(nSize < nItem)
		nItem = nSize;
	
	TCITEM item;
	item.mask = TCIF_TEXT|TCIF_PARAM;
	item.lParam = nTab;
	item.pszText = lpszItem;

	pMember->m_pTabCtrl->CTabCtrl::InsertItem(nItem, &item);

	pMember->m_nActiveItem=nItem;
	SetDefaultItemRect();
}

void CExTabCtrlApp::InsertItem(int nItem, LPSTR lpszItem, int nImage, UINT nTab/* = EXTAB_NORMAL*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	int nSize = pMember->m_pTabCtrl->GetItemCount();
	if(nSize < nItem)
		nItem = nSize;
	
	TCITEM item;
	item.mask = TCIF_TEXT|TCIF_PARAM;
	item.lParam = nTab;
	item.pszText = lpszItem;
	item.iImage = nImage;

	pMember->m_pTabCtrl->CTabCtrl::InsertItem(nItem, &item);

	pMember->m_nActiveItem=nItem;
	SetDefaultItemRect();
}

void CExTabCtrlApp::InsertItem(int nItem, TCITEM* pTabCtrlItem, UINT nTab/* = EXTAB_NORMAL*/)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	int nSize = pMember->m_pTabCtrl->GetItemCount();
	if(nSize < nItem)
		nItem = nSize;
	
	if(!pTabCtrlItem->lParam)
		pTabCtrlItem->lParam = nTab;

	pMember->m_pTabCtrl->CTabCtrl::InsertItem(nItem, pTabCtrlItem);

	pMember->m_nActiveItem=nItem;
	SetDefaultItemRect();
}

void CExTabCtrlApp::OnMouseMove(UINT nFlags, CPoint point) 
{
	return;
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	if(pMember->m_bAutoRaising)
	{
		if(!pMember->m_bOverTab )
		{
		//	We are here if the mouse  just entered the CLBTabCtrl
		//
		//	Let's use _TrackMouseEvent API to be notified when the mouse leaves.
		//	This API tries to use the window manager's
		//	implementation of TrackMouseEvent if it is present,
		//	otherwise it emulates.

			pMember->m_bOverTab=true;
			TRACKMOUSEEVENT stTRACKMOUSEEVENT;
			stTRACKMOUSEEVENT.cbSize = sizeof(stTRACKMOUSEEVENT);
			stTRACKMOUSEEVENT.hwndTrack=pMember->m_pTabCtrl->m_hWnd;
			stTRACKMOUSEEVENT.dwFlags=TME_LEAVE;
			_TrackMouseEvent(&stTRACKMOUSEEVENT);
		}
		CRect rectItem(0,0,0,0);

		//	Here we shall get mouse position for the last dispatched message and
		//	test if mouse is over any tab item.
		TCHITTESTINFO thi;
		thi.pt=point;
		int nItem=pMember->m_pTabCtrl->HitTest(&thi);
		if(nItem>=0)
		{
			if(nItem!=pMember->m_nPrevPointedItem )
			{
			//	Mouse is over new inactive item.
				if(pMember->m_nPrevPointedItem>=0 && pMember->m_nPrevPointedItem!=pMember->m_nActiveItem)
				{
				//	Redraw the item, over which mouse was on the previous step.
				//	We redraw it as normal (unraised).
					  
					pMember->m_pTabCtrl->GetItemRect(pMember->m_nPrevPointedItem,&rectItem);
					rectItem.top-=FLOAT_ITEM_HEIGHT_SHIFT;


					// If the right side of item overlaps the up-down control,
					// we have to exclude the up-down control from update region
					// to reduce flickering.
					if(!pMember->m_rectUpDn.IsRectEmpty() && rectItem.right >= pMember->m_rectUpDn.left)
						rectItem.right= pMember->m_rectUpDn.left;

					pMember->m_pTabCtrl->InvalidateRect(&rectItem);
					pMember->m_pTabCtrl->UpdateWindow();
				}
				if(nItem!=pMember->m_nActiveItem)
				 {
				 //Redraw the item, over which mouse is now.
				 //We redraw it as raised.
					pMember->m_pTabCtrl->GetClientRect(&rectItem);
					pMember->m_pTabCtrl->ValidateRect(&rectItem);
					pMember->m_pTabCtrl->GetItemRect(nItem,&rectItem);
					pMember->m_bFloatPaint=true;
					rectItem.top-=FLOAT_ITEM_HEIGHT_SHIFT;


					// If the right side of item overlaps the up-down control,
					// we have to exclude the up-down control from update region
					// to reduce flickering
					if(!pMember->m_rectUpDn.IsRectEmpty() && rectItem.right >= pMember->m_rectUpDn.left)
						rectItem.right= pMember->m_rectUpDn.left;

					pMember->m_pTabCtrl->InvalidateRect(&rectItem);
					pMember->m_pTabCtrl->UpdateWindow();
				 }
				pMember->m_nPrevPointedItem=nItem;
			}
		}
	}
	
	pMember->m_pTabCtrl->CTabCtrl::OnMouseMove(nFlags, point);
}

void CExTabCtrlApp::OnPaint(CDC* pDC)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	CRect rcPnt,rcItem,rcActive,rectCli;
	CDC dc;
	CBitmap bmp;
	pMember->m_pTabCtrl->GetClientRect(&rectCli);

	//To reduce flickering we are drawing to memory DC,
	//and then BitBlting result to the screen.
	//So we have to create compatible memory DC and select bitmap into it.
	dc.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC,rectCli.Width(),rectCli.Height());
	CBitmap *pOldBitmap;
	pOldBitmap=dc.SelectObject(&bmp);

	//Copy update rect to rcPnt 
	rcPnt=((CPaintDC*)pDC)->m_ps.rcPaint;
//	COLORREF clrFill=::GetSysColor(COLOR_3DFACE);
//	COLORREF clrFill=pMember->m_clrBackground;
	COLORREF clrFill=pMember->m_clrTabBkgrnd;
	//@Solomon COLORREF clrFill = RGB(193,202,217);	//@Solomon

	//As so I bypassed WM_ERASEBCKGND, do its job here 
	dc.FillSolidRect(&rcPnt,clrFill);

	HPEN hOldPen;
	DRAWITEMSTRUCT drawItemStruct;

	hOldPen = (HPEN)::GetCurrentObject(dc.m_hDC,OBJ_PEN);
	
	//Active item should be wider then other items.
	//So here we calculate and apply the width difference.
	pMember->m_pTabCtrl->GetItemRect(pMember->m_nActiveItem,&rcActive);
	if(pMember->m_nActiveItem!=(int)FindLeftDownItem(pMember)) // if acive item is not leftmost,
		rcActive.left-=3;					   // then move to the left its left side.
	TCHITTESTINFO thi;
	thi.pt=CPoint(rcActive.right+5,rcActive.bottom-4);
	int nPos = pMember->m_pTabCtrl->HitTest(&thi);
	if(nPos>=0)				// if acive item is not rightmost,
		rcActive.right+=3;	// then move to the right its left side.

	//Active item should be higher then other items.
	//So here we  apply the height difference.
	rcActive.top-=ACTIVE_ITEM_HEIGHT_SHIFT;

	COLORREF clrSelect = pMember->m_clrSelect;
	//clrSelect = RGB(124,147,184);	//@Solomon
	BOOL bInvalidateRect = FALSE;

	TCITEM item;
	item.mask=TCIF_PARAM;
	pMember->m_pTabCtrl->GetItem(pMember->m_nActiveItem, &item);

	int nColor = item.lParam;
	pMember->m_pTabCtrl->GetItem(pMember->m_nPrevPointedItem, &item);
	if((nColor != item.lParam) || pMember->m_nPrevPointedItem < 0)
		pMember->m_bInvalidatePage = TRUE;
	else
		pMember->m_bInvalidatePage = FALSE;

	if(nColor != EXTAB_NORMAL)
	{
		switch(nColor)
		{
		case EXTAB_SELL:
			clrSelect = pMember->m_pISkinColorManager->GetColor(39);
			break;
		case EXTAB_BUY:
			clrSelect = pMember->m_pISkinColorManager->GetColor(45);
			break;
		case EXTAB_MODIFY:
			clrSelect = pMember->m_pISkinColorManager->GetColor(51);
			break;
//		case EXTAB_02:		// 영희과장님스탈
//			clrSelect = RGB(191, 222, 255);		// RGB(239, 242, 247);
//			break;			
		case EXTAB_CANCEL:
			clrSelect = pMember->m_pISkinColorManager->GetColor(57);
			break;
		}
	}

	if (pMember->m_bFloatPaint )
	{	
		//
		//The mouse is  moving over inactive item - raise it
		//
		pMember->m_bFloatPaint=false;
		//Get the mouse position for the last dispatched message and
		// find index of item, over which the mouse is(was).
		TCHITTESTINFO tci;
		tci.pt=CPoint(::GetMessagePos());
		pMember->m_pTabCtrl->ScreenToClient(&tci.pt);
		int nItemFloat=pMember->m_pTabCtrl->HitTest(&tci);
		if(nItemFloat>=0)
		{
			pMember->m_pTabCtrl->GetItemRect(nItemFloat,&rcItem);
			rcItem.DeflateRect(ITEM_DEFLATE,2);
			rcItem.top-=FLOAT_ITEM_HEIGHT_SHIFT;
			dc.FillSolidRect(&rcItem, pMember->m_clrNormal);
		
			//Draw item's text,bitmap,...
			drawItemStruct.rcItem=rcItem;
			drawItemStruct.hDC=dc.GetSafeHdc();
			drawItemStruct.itemID=nItemFloat;
			DrawItem(&drawItemStruct);

			//Draw item's rect
			//HINT_ACTIVATE_RIGHT_SIDE is a flag indicating, that the
			//right side of rect should be drawn as for active item (darker).
			DrawItemRect(dc,rcItem,HINT_ACTIVATE_RIGHT_SIDE);
		}
	}
	else
	{
		//If we are here, then  complex repaint is required.
		CRect rectTemp;
		int nCount = pMember->m_pTabCtrl->GetItemCount();

		//{{ 2006. 09. 19 by LYH 삼성 탭 디자인 적용
		COLORREF highlightColor;		
		//탭의 내곽선(하일라이트색) 그려주기
		highlightColor = RGB(255,255,255);

		CPen ActiveTabLinePen;
		CPen *oldhighlightPen, highlightPen;
		ActiveTabLinePen.CreatePen(	PS_SOLID,1 , RGB(120,120,120)/*m_pColorMap->GetColor(nTabLineIndex)*/);
		highlightPen.CreatePen(PS_SOLID, 1 , highlightColor);
		int nGap = 2;
		for(int i=0;i< nCount;i++)
		{
			//Let's draw all inactive  items which intersects invalidated
			//rect (rcPaint).
			pMember->m_pTabCtrl->GetItemRect(i,&rcItem);
//			rcItem.right-=3;
//			rcItem.top-=1;
//			rcItem.bottom+=1;
			if (rectTemp.IntersectRect(&rcItem,&rcPnt))
			{
				rectTemp = rcItem;

				COLORREF Start, End;
				if(i!=pMember->m_nActiveItem)
				{
					if(!pMember->m_bGradation)
					{
						rectTemp.top+=1;
						rectTemp.left+=2;
						dc.FillSolidRect(&rectTemp, pMember->m_clrNormal);
						//dc.FillSolidRect(&rectTemp, RGB(234, 234, 234));	//@Solomon
					}
					else
					{
						CRect rect(rectTemp);
						CRect backup = rect;
						int r,g,b;//각 3가지 색상의 값의 차이..
						float rStep, gStep, bStep;//각 3가지 값들 각각의 색상값의 차이..
						float fStep;//최대 색상값 차이로 나누어서 한번에 이동할 옵셋..

						Start = pMember->m_clrTabNorGDStt;
						End   = pMember->m_clrTabNorGDEnd;
						//Start = RGB(234, 234, 234);	//@Solomon 
						//End   = RGB(235, 235, 235);	//@Solomon 
						
						//각 생상 차이를 구한다..
						r = (GetRValue(End) - GetRValue(Start));
						g = (GetGValue(End) - GetGValue(Start));
						b = (GetBValue(End) - GetBValue(Start));
						
						//가장 큰차이가 나는것을 구한다..
						int Max = max( abs(r), max(abs(g), abs(b)) );
						
						//그릴 사각형을 차이가 나는것 만큼.. 분할할 길이를 구한다..
						fStep = (float)rect.Height()/(float)Max;     // 상-하 Gradation 
						
						//각 색상별 단계 차이값..
						rStep = r/(float)Max;
						gStep = g/(float)Max;
						bStep = b/(float)Max;
						
						//r,g,b를 시작 색으로 재설정...
						r = GetRValue(Start);
						g = GetGValue(Start);
						b = GetBValue(Start);
						
						//색깔이 변해야 하는 횟수만큼.. 반복 그린다.
						for(int iOnBand=0 ; iOnBand <Max; iOnBand++)  // 상-하 Gradation 
						{
							::SetRect( &rect, rect.left, int(fStep*iOnBand) + backup.top ,
								rect.right, backup.bottom );  
							dc.FillSolidRect( &rect,  RGB( r + rStep*iOnBand, g + gStep*iOnBand, b + bStep*iOnBand ) );
						}
					}
				}
				else
				{
					if(!pMember->m_bGradation)
					{
						rectTemp.top+=1;
						rectTemp.bottom+=1;
						rectTemp.left+=1;
						dc.FillSolidRect(&rectTemp, clrSelect);
					}
					else
					{
						CRect rect(rectTemp);
						CRect backup = rect;
						int r,g,b;//각 3가지 색상의 값의 차이..
						float rStep, gStep, bStep;//각 3가지 값들 각각의 색상값의 차이..
						float fStep;//최대 색상값 차이로 나누어서 한번에 이동할 옵셋..
						
						Start = pMember->m_clrTabSelGDStt;
						End   = pMember->m_clrTabSelGDEnd;
						//Start = RGB(125, 148, 185);
						//End   = RGB(124, 147, 184);
						
						//각 생상 차이를 구한다..
						r = (GetRValue(End) - GetRValue(Start));
						g = (GetGValue(End) - GetGValue(Start));
						b = (GetBValue(End) - GetBValue(Start));
						
						//가장 큰차이가 나는것을 구한다..
						int Max = max( abs(r), max(abs(g), abs(b)) );
						
						//그릴 사각형을 차이가 나는것 만큼.. 분할할 길이를 구한다..
						fStep = (float)rect.Height()/(float)Max;     // 상-하 Gradation 
						
						//각 색상별 단계 차이값..
						rStep = r/(float)Max;
						gStep = g/(float)Max;
						bStep = b/(float)Max;
						
						//r,g,b를 시작 색으로 재설정...
						r = GetRValue(Start);
						g = GetGValue(Start);
						b = GetBValue(Start);
						
						//색깔이 변해야 하는 횟수만큼.. 반복 그린다.
						for(int iOnBand=0 ; iOnBand <Max; iOnBand++)  // 상-하 Gradation 
						{
							::SetRect( &rect, rect.left, int(fStep*iOnBand) + backup.top ,
								rect.right, backup.bottom );  
							dc.FillSolidRect( &rect,  RGB( r + rStep*iOnBand, g + gStep*iOnBand, b + bStep*iOnBand ) );
						}
					}
				}

				oldhighlightPen = dc.SelectObject(&ActiveTabLinePen);
				dc.MoveTo(rcItem.right , rcItem.bottom);
				dc.LineTo(rcItem.left, rcItem.bottom);
				dc.LineTo(rcItem.left, rcItem.top + nGap);
//				if(i!=pMember->m_nActiveItem)
//				{
//					dc.MoveTo(rcItem.left , rcItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.
//					dc.LineTo(rcItem.left-4, rcItem.bottom);
//
//					dc.MoveTo(rcItem.right , rcItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.	
//					dc.LineTo(rcItem.left , rcItem.bottom );			// 바닥 외곽선 라인 그린다.				
//					dc.LineTo(rcItem.left, rcItem.top + nGap);		// 좌측 외곽선 라인 그린다.				
//				}
//				else
//				{
//					dc.MoveTo(rcItem.left , rcItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.
//					dc.LineTo(rcItem.left-4, rcItem.bottom);
//
//					dc.MoveTo(rcItem.left , rcItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.	
//					dc.LineTo(rcItem.left, rcItem.top + nGap);		// 좌측 외곽선 라인 그린다.		
//				}
//
//
//				dc.LineTo(rcItem.left+1,  rcItem.top + nGap);		// 좌측 상단 모서리 둥글게1
//				dc.LineTo(rcItem.left+1, rcItem.top +1);	// 좌측 상단 모서리 둥글게2
//				dc.LineTo(rcItem.left+2, rcItem.top +1);	// 좌측 상단 모서리 둥글게3
//				dc.LineTo(rcItem.left+nGap + 1, rcItem.top);				// 좌측 상단 모서리 둥글게4
				dc.LineTo(rcItem.left, rcItem.top);

			
//				dc.LineTo(rcItem.right - nGap - 1, rcItem.top);			// 상단 외곽선 그린다.
//				dc.LineTo(rcItem.right - nGap,		rcItem.top + nGap - 1);	// 우측 상단 모서리 둥글게1
//				dc.LineTo(rcItem.right - nGap + 1,	rcItem.top + nGap - 1);	// 우측 상단 모서리 둥글게2
//				dc.LineTo(rcItem.right - nGap + 1,  rcItem.top + nGap);		// 우측 상단 모서리 둥글게3
//				dc.LineTo(rcItem.right , rcItem.top + nGap);					// 우측 상단 모서리 둥글게4
				dc.LineTo(rcItem.right,		rcItem.top);		// 좌측 상단 모서리 둥글게1

				dc.LineTo(rcItem.right , rcItem.bottom );	// 우측 외곽선 라인 그린다.
				
				// 안쪽테두리
				if(i!=pMember->m_nActiveItem)
				{
					dc.SelectObject(&highlightPen);

//					dc.MoveTo(rcItem.left+1, rcItem.top + 3);		// 좌측선
//					dc.LineTo(rcItem.left+1, rcItem.bottom);
//
//					dc.MoveTo(rcItem.left+1, rcItem.top + 3);		// 좌측 상단 모서리 둥글게1
//					dc.LineTo(rcItem.left+3, rcItem.top + 1);		// 좌측 상단 모서리 둥글게2
//
//					dc.MoveTo(rcItem.left+1, rcItem.top + 4);
//					dc.LineTo(rcItem.left+4, rcItem.top + 1);
//
//					dc.MoveTo(rcItem.left+3, rcItem.top + 1);		// 상단 라인
//					dc.LineTo(rcItem.right-2, rcItem.top+ 1);
//
//					dc.MoveTo(rcItem.left+1, rcItem.top + 1);
//					dc.LineTo(rcItem.left+1, rcItem.bottom);
//
//					dc.MoveTo(rcItem.left+1, rcItem.top + 1);
//					dc.LineTo(rcItem.right-2, rcItem.top + 1);
				}				
				dc.SelectObject(oldhighlightPen);

				//Draw item's text,bitmap,...
				drawItemStruct.rcItem=rcItem;
				drawItemStruct.hDC=dc.GetSafeHdc();
				drawItemStruct.itemID=i;
				DrawItem(&drawItemStruct);
			//	DrawItemRect(dc,rcItem);
			}
		}
		oldhighlightPen = dc.SelectObject(&ActiveTabLinePen);
		dc.MoveTo(rcItem.right, rcItem.bottom);		// 상단 라인
		dc.LineTo(rcPnt.right, rcItem.bottom);
		dc.SelectObject(oldhighlightPen);

		ActiveTabLinePen.DeleteObject();
		highlightPen.DeleteObject();
	}
	//}} 2006. 09. 19 by LYH 삼성 탭 디자인 적용

	//Copy drawing from memory device context to the screen.
	pDC->BitBlt(0,0,rectCli.Width(),rectCli.Height(),&dc,0,0,SRCCOPY);

	//Restore device context defaults.
	::SelectObject(dc.m_hDC,hOldPen);
	dc.SelectObject(pOldBitmap);
	bmp.DeleteObject();
	dc.DeleteDC();

// 2006.09.05 by LYH 깜빡거림 심해 막음	
//	CWnd* pChild = pMember->m_pTabCtrl->GetWindow(GW_CHILD);
//	while(pChild)
//	{
//		if(pChild->IsWindowVisible())
//			pChild->Invalidate();
//
//		pChild = pMember->m_pTabCtrl->GetWindow(GW_HWNDNEXT);
//	}
}

void CExTabCtrlApp::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
//	return;
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	if(nSBCode == SB_THUMBPOSITION)
	{
		pMember->m_pTabCtrl->CTabCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
		//	If the current position of CLBSpinButtonCtrl has changed,
		//	we have to repaint area near it, since it could have
		//	traces of rightmost item.
		CRect rctNearSpin;
		pMember->m_pTabCtrl->GetClientRect(&rctNearSpin);
		rctNearSpin.right = pMember->m_rectUpDn.left;
		rctNearSpin.bottom = pMember->m_rectUpDn.bottom+2;
		rctNearSpin.left=pMember->m_rectUpDn.left-2*SHIFT_FROM_CUT_TO_SPIN;

		pMember->m_pTabCtrl->InvalidateRect(&rctNearSpin);
		pMember->m_pTabCtrl->UpdateWindow();
	}
}

BOOL CExTabCtrlApp::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	pMember->m_nPrevPointedItem=-1;
	*pResult = 0;

	CRect rctUpdate;
	pMember->m_pTabCtrl->GetClientRect(&rctUpdate);

	//	We will set visible bits back on here (see comments in OnSelchanging),
	//	since now we know which item became active and can redraw CLBTabCtrl.
	pMember->m_pTabCtrl->ValidateRect(&rctUpdate);//Clean-up any pending WM_PAINTs
	DWORD dwStyle = ::GetWindowLong(pMember->m_pTabCtrl->m_hWnd,GWL_STYLE);
	if ((dwStyle & WS_VISIBLE) == 0)
        ::SetWindowLong(pMember->m_pTabCtrl->m_hWnd, GWL_STYLE, (dwStyle | WS_VISIBLE));//switch on WS_VISIBLE
	
	pMember->m_nActiveItem=pMember->m_pTabCtrl->GetCurSel();
//	if(::IsWindow(pMember->m_Spin.m_hWnd))
//	{
//		//As so the CLBSpinButtonCtrl lives within CLBTabCtrl,we have to
//		// update its (CLBSpinButtonCtrl) state when tab order has changed
//		UINT nLeftItem=FindLeftDownItem(pMember);
//		NMUPDOWN nmUpDn;
//		nmUpDn.iDelta=0;
//		nmUpDn.iPos=nLeftItem;
//		nmUpDn.hdr.code=UDN_DELTAPOS;
//		nmUpDn.hdr.hwndFrom=pMember->m_Spin.m_hWnd;
//		nmUpDn.hdr.idFrom=::GetDlgCtrlID(pMember->m_Spin.m_hWnd);
//
//		::SendMessage(::GetParent(pMember->m_Spin.m_hWnd),
//							WM_NOTIFY,(WPARAM)nmUpDn.hdr.idFrom,
//							(LPARAM)&nmUpDn);
//	}

	//if(!pMember->m_bInvalidatePage)
		rctUpdate.bottom=pMember->m_rctPage.top+2;
	rctUpdate.top=0;

	// If the right side of update region overlaps the up-down control,
	// we have to exclude the up-down control from update region
	// to reduce flickering
	if(!pMember->m_rectUpDn.IsRectEmpty())
	{
		rctUpdate.right= pMember->m_rectUpDn.left;
	}
	//Let's repaint tabs,since the active tab has changed
	pMember->m_pTabCtrl->InvalidateRect(&rctUpdate,FALSE);
	//pMember->m_pTabCtrl->UpdateWindow();

//	POSITION pos = pMember->m_listForgCtrl.GetHeadPosition();
//	while(pos)
//	{
//		CWnd* pControl = pMember->m_listForgCtrl.GetNext(pos);
//		pControl->RedrawWindow();
//	}

	return FALSE;
}

BOOL CExTabCtrlApp::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	CRect rctUpdate;
	pMember->m_pTabCtrl->GetClientRect(&rctUpdate);
	pMember->m_pTabCtrl->ValidateRect(&rctUpdate); //Clean-up any pending WM_PAINTs
	
	DWORD dwStyle = ::GetWindowLong(pMember->m_pTabCtrl->m_hWnd,GWL_STYLE);
	if (dwStyle & WS_VISIBLE)
     ::SetWindowLong(pMember->m_pTabCtrl->m_hWnd, GWL_STYLE, (dwStyle & ~ WS_VISIBLE));//switch off WS_VISIBLE

	*pResult = 0;
	return FALSE;
}

LRESULT CExTabCtrlApp::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	
	pMember->m_bOverTab=false;
	if(pMember->m_nPrevPointedItem>=0)
	{
		if (pMember->m_nPrevPointedItem!=pMember->m_nActiveItem)
		{
			//If we are here,then mouse leaved the raised item, so
			//we have to redraw it as normal (unraised).

			CRect rectItem;
			pMember->m_pTabCtrl->GetItemRect(pMember->m_nPrevPointedItem,&rectItem);
			rectItem.top-=FLOAT_ITEM_HEIGHT_SHIFT;


			//If the right side of item overlaps the up-down control,
			//we have to exclude the up-down control from update region
			//to reduce flickering
			if(!pMember->m_rectUpDn.IsRectEmpty() && rectItem.right >= pMember->m_rectUpDn.left)
				rectItem.right = pMember->m_rectUpDn.left;

			pMember->m_pTabCtrl->InvalidateRect(&rectItem);
			pMember->m_pTabCtrl->UpdateWindow();
		}
		pMember->m_nPrevPointedItem=-1;
	}

	return 0;
}

void CExTabCtrlApp::OnKillFocus(CWnd* pNewWnd) 
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	
	pMember->m_bHasFocus	= false;
	pMember->m_pTabCtrl->CTabCtrl::OnKillFocus(pNewWnd);
}

void CExTabCtrlApp::OnSetFocus(CWnd* pOldWnd) 
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	
	pMember->m_bHasFocus	= true;
	pMember->m_pTabCtrl->CTabCtrl::OnSetFocus(pOldWnd);
}

LRESULT CExTabCtrlApp::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	
	if(message != TCM_SETCURSEL)
		return pMember->m_pTabCtrl->CTabCtrl::WindowProc(message, wParam, lParam);
	else if(message == WM_COMMAND)
	{
		CWnd *pOwner = pMember->m_pTabCtrl->GetOwner();
		if (pOwner && ::IsWindow(pOwner->m_hWnd))
			return pOwner->SendMessage(message, wParam, lParam);
		else
			return 0;
	}	
	else
	{	//	We need to handle this message to alter CLBTabCtrl state,
		//	which may change due to user keyboard input ('Control-Tab')
		//	or because of direct SendMessage.
		//
		//	After CLBTabCtrl control handles TCM_SETCURSEL message,  
		//	the system will draw the tab control, by sending to it 
		//	WM_PAINT/WM_ERASEBKGND messages.
		//	Since we perform actual redraw in  TCN_SELCHANGE handler ,
		//	we have to avoid these WM_PAINT/WM_ERASEBKGND messages 
		//	to get rid of flickering.
		//
		//	Since up-down control do not receive WM_PAINT/WM_ERASEBKGND messages 
		//	if it is invisible, so we temporary  set the appropriate visible bits
		//	off and the control thinks it is visible even though it is not.
		//	We shall set visible bits back on in TCN_SELCHANGE handler, when
		//	it is good time to redraw CLBTabCtrl.

		DWORD dwStyle = ::GetWindowLong(pMember->m_pTabCtrl->m_hWnd,GWL_STYLE);
		if (dwStyle & WS_VISIBLE)
		    ::SetWindowLong(pMember->m_pTabCtrl->m_hWnd, GWL_STYLE, (dwStyle & ~ WS_VISIBLE));
	
		LRESULT nResult = pMember->m_pTabCtrl->CTabCtrl::WindowProc(message, wParam, lParam);
		
		//Now let's meet with TCN_SELCHANGE message
		NMHDR nmhdr;
		nmhdr.hwndFrom = pMember->m_pTabCtrl->m_hWnd;
		nmhdr.idFrom = ::GetDlgCtrlID(pMember->m_pTabCtrl->m_hWnd);
		nmhdr.code = TCN_SELCHANGE;

		::SendMessage(::GetParent(pMember->m_pTabCtrl->m_hWnd),WM_NOTIFY,
						WPARAM(nmhdr.idFrom),LPARAM(&nmhdr));
		return nResult;
	}
}

void CExTabCtrlApp::DrawFocus(CDC &dc)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	//	Helper function
	//	Draws a focus rect over active item

		CRect rctItem;
		pMember->m_pTabCtrl->GetItemRect(pMember->m_nActiveItem,&rctItem);
		if(rctItem.left >=0)
		{
			if(pMember->m_nActiveItem!=(int)FindLeftDownItem(pMember))
				rctItem.left-=3;
			TCHITTESTINFO thi;
			thi.pt=CPoint(rctItem.right+5,rctItem.bottom-4);
			int nPos = pMember->m_pTabCtrl->HitTest(&thi);
			if(nPos>=0)
				rctItem.right+=3;

			if(!pMember->m_rectUpDn.IsRectEmpty()&& rctItem.right >= pMember->m_rectUpDn.left)
			{
				rctItem.right = pMember->m_rectUpDn.left-SHIFT_FROM_CUT_TO_SPIN;
				rctItem.right-=2;
				rctItem.left+=3;
			}
			else
				rctItem.DeflateRect(3,0);

			rctItem.top+=1;
			dc.DrawFocusRect(&rctItem);
		}
}

void CExTabCtrlApp::DrawItemRect(CDC &dc, CRect &rcItem,int nHint)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	//Let's draw items rect.
	rcItem.bottom-=1;
	if(rcItem.left>0)
	{
		bool bDoNotCut=true;

		::SelectObject(dc.m_hDC,pMember->m_hPenShadow);
		
		if(nHint & HINT_ACTIVE_ITEM)
		{
			dc.MoveTo(rcItem.left,rcItem.bottom+2);
			dc.LineTo(rcItem.left,rcItem.top+1);
			dc.LineTo(rcItem.left+1,rcItem.top);
		}
		else
		{
			dc.MoveTo(rcItem.left,rcItem.bottom);
			dc.LineTo(rcItem.left,rcItem.top+2);
			dc.LineTo(rcItem.left+2,rcItem.top);
		}
		
		if(!pMember->m_rectUpDn.IsRectEmpty() && rcItem.right >= pMember->m_rectUpDn.left)
		{
			//	CLBTabCtrl is not in stacked mode and current item overlaps
			//	msctls_updown32 control. So let's decrease current item's  
			//	width.
			rcItem.right=pMember->m_rectUpDn.left-SHIFT_FROM_CUT_TO_SPIN;
			dc.LineTo(rcItem.right,rcItem.top);
			bDoNotCut=false;
		}
		else
			dc.LineTo(rcItem.right-2,rcItem.top);
		
/*
		if(!nHint)
			::SelectObject(dc.m_hDC,pMember->m_hPenShadow);
		else
		{
			if(nHint & HINT_ACTIVATE_RIGHT_SIDE)
			{
				::SelectObject(dc.m_hDC,pMember->m_hPenShadow);
			}
		}
*/
		if(bDoNotCut)
		{
			if(nHint & HINT_ACTIVE_ITEM)
			{
				dc.LineTo(rcItem.right,rcItem.top+1);
				dc.LineTo(rcItem.right,rcItem.bottom+2);
			}
			else
			{
				dc.LineTo(rcItem.right,rcItem.top+2);
				dc.LineTo(rcItem.right,rcItem.bottom+1);
			}
		}
		else
		{
			//	CLBTabCtrl is not in stacked mode and current item overlaps
			//	msctls_updown32 control. So let's draw current item's rect 
			//	as cut.
			int nYPos=rcItem.top+2;

			nYPos+=rcItem.Height()/5;
			rcItem.right+=1;
			if(nYPos < rcItem.bottom)
				dc.LineTo(rcItem.right,nYPos);

			nYPos+=rcItem.Height()/3;
			rcItem.right-=2;
			if(nYPos < rcItem.bottom)
				dc.LineTo(rcItem.right,nYPos);

			nYPos+=rcItem.Height()/5;
			rcItem.right+=1;
			if(nYPos < rcItem.bottom)
				dc.LineTo(rcItem.right,nYPos);

			nYPos+=rcItem.Height()/5;
			rcItem.right+=1;
			if(nYPos < rcItem.bottom)
				dc.LineTo(rcItem.right,nYPos);
			if(nHint & HINT_ACTIVE_ITEM)
			{
				dc.LineTo(rcItem.right,rcItem.bottom+2);
			}
			else
				dc.LineTo(rcItem.right,rcItem.bottom+1);

		}
	}
}

void CExTabCtrlApp::DrawItemInRect(CDC &dc, CRect &rcItem)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	//Let's draw items rect.
//	rcItem.bottom-=1;
	rcItem.DeflateRect(1, 1, 1, 0);

	::SelectObject(dc.m_hDC,pMember->m_hPenLight);
	if(rcItem.left>0)
	{
		bool bDoNotCut=true;

		dc.MoveTo(rcItem.left-2,rcItem.bottom+2);
		dc.LineTo(rcItem.left,rcItem.bottom+2);
		dc.LineTo(rcItem.left,rcItem.top);
		dc.LineTo(rcItem.left,rcItem.top);

		if(!pMember->m_rectUpDn.IsRectEmpty() && rcItem.right >= pMember->m_rectUpDn.left)
		{
			//	CLBTabCtrl is not in stacked mode and current item overlaps
			//	msctls_updown32 control. So let's decrease current item's  
			//	width.
			rcItem.right=pMember->m_rectUpDn.left-SHIFT_FROM_CUT_TO_SPIN;
			dc.LineTo(rcItem.right,rcItem.top);
			bDoNotCut=false;
		}
		else
			dc.LineTo(rcItem.right-2,rcItem.top);
		
		if(bDoNotCut)
		{
			dc.LineTo(rcItem.right,rcItem.top);
			dc.LineTo(rcItem.right,rcItem.bottom+2);
			dc.LineTo(rcItem.right+1,rcItem.bottom+2);
		}
		else
		{
			//	CLBTabCtrl is not in stacked mode and current item overlaps
			//	msctls_updown32 control. So let's draw current item's rect 
			//	as cut.
			int nYPos=rcItem.top+2;

			nYPos+=rcItem.Height()/5;
			rcItem.right+=1;
			if(nYPos < rcItem.bottom)
				dc.LineTo(rcItem.right,nYPos);

			nYPos+=rcItem.Height()/3;
			rcItem.right-=2;
			if(nYPos < rcItem.bottom)
				dc.LineTo(rcItem.right,nYPos);

			nYPos+=rcItem.Height()/5;
			rcItem.right+=1;
			if(nYPos < rcItem.bottom)
				dc.LineTo(rcItem.right,nYPos);

			nYPos+=rcItem.Height()/5;
			rcItem.right+=1;
			if(nYPos < rcItem.bottom)
				dc.LineTo(rcItem.right,nYPos);

			dc.LineTo(rcItem.right,rcItem.bottom+2);
			dc.LineTo(rcItem.right+1,rcItem.bottom+2);
		}
	}

	dc.MoveTo(rcItem.right - 5,rcItem.top + 1);
	dc.LineTo(rcItem.right,rcItem.top + 6);
	
	::SelectObject(dc.m_hDC,pMember->m_hPenItemShadow);
	dc.MoveTo(rcItem.right - 4,rcItem.top + 1);
	dc.LineTo(rcItem.right - 1,rcItem.top + 1);
	dc.LineTo(rcItem.right - 1,rcItem.top + 4);
	dc.LineTo(rcItem.right - 3,rcItem.top + 2);
	dc.LineTo(rcItem.right - 2,rcItem.top + 1);
	dc.LineTo(rcItem.right - 2,rcItem.top + 3);
}

UINT CExTabCtrlApp::FindLeftDownItem(CMemberDraw* pMember) const
{
	//Helper function
	//Finds leftmost down item.

	CRect rctLeft(100000,0,0,0),rctCur;
	int nCount = pMember->m_pTabCtrl->GetItemCount()-1;
	UINT nItem=0;
	for(int i=0;i<nCount;i++)
	{
		pMember->m_pTabCtrl->GetItemRect(i,&rctCur);
		if(rctCur.left > 0 && rctCur.left <= rctLeft.left)
		{
			if(rctCur.bottom > rctLeft.bottom)
			{
				rctLeft=rctCur;
				nItem=i;	
			}
		}
	}
	return nItem;
}

bool CExTabCtrlApp::SetAutoRaising(bool bOn)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	// User should have an opportunity to swith on/off the 
	// autoraising feature of the CLBTabCtrl control.

	bool bPrev = pMember->m_bAutoRaising;
	pMember->m_bAutoRaising = bOn;
	return bPrev;
}

void CExTabCtrlApp::AddChildControl(CWnd* pControl)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->AddForeCtrl(pControl);
}

void CExTabCtrlApp::RemoveChildControl(CWnd* pControl)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->RemoveForeCtrl(pControl);
}

void CExTabCtrlApp::SetTabColor(COLORREF clrBkgrnd, COLORREF clrActive, COLORREF clrInactive)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->SetTabColor(clrBkgrnd, clrActive, clrInactive);
}

void CExTabCtrlApp::SetSTTabColor(long lSTTabColor)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->SetSTTabColor(lSTTabColor);
}

void CExTabCtrlApp::OnSize(UINT nType, int cx, int cy)
{
	SetDefaultItemRect();
	
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_pTabCtrl->CTabCtrl::OnSize(nType, cx, cy);
}
/////////////////////////////////////////////////////////////////////////////
// CExTabCtrlApp operations

/////////////////////////////////////////////////////////////////////////////
// The one and only CExTabCtrlAppApp object

CExTabCtrlApp theApp;
extern "C" __declspec(dllexport) long Ex_ActionControl(LPVOID lpPointer, LPVOID lpAction)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	long lResult = theApp.ActionControl(lpAction);

	return lResult;
}

extern "C" __declspec(dllexport) void Ex_ExitTabCtrl(LPVOID lpPointer)
{
	theApp.ExitTabCtrl(lpPointer);
}

extern "C" __declspec(dllexport) LPVOID Ex_InitTabCtrlDraw(LPVOID lpPointer)
{
	return theApp.InitTabCtrlDraw(lpPointer);
}

extern "C" __declspec(dllexport) LPVOID Ex_InitTabCtrlDraw2(LPVOID lpPointer, BOOL bUseDefSkin)
{
	return theApp.InitTabCtrlDraw(lpPointer, bUseDefSkin);
}

extern "C" __declspec(dllexport) void Ex_PreSubclassWindow(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.PreSubclassWindow();
}

extern "C" __declspec(dllexport) BOOL Ex_OnEraseBkgnd(LPVOID lpPointer, CDC* pDC)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.OnEraseBkgnd(pDC);

	return bResult;
}

extern "C" __declspec(dllexport) void Ex_DrawItem(LPVOID lpPointer, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.DrawItem(lpDrawItemStruct);
}

extern "C" __declspec(dllexport) void Ex_InsertItem1(LPVOID lpPointer, int nItem, LPSTR lpszItem, UINT nTab)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.InsertItem(nItem, lpszItem, nTab);
}

extern "C" __declspec(dllexport) void Ex_InsertItem2(LPVOID lpPointer, int nItem, LPSTR lpszItem, int nImage, UINT nTab)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.InsertItem(nItem, lpszItem, nImage, nTab);
}

extern "C" __declspec(dllexport) void Ex_InsertItem3(LPVOID lpPointer, int nItem, TCITEM* pTabCtrlItem, UINT nTab)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.InsertItem(nItem, pTabCtrlItem, nTab);
}

extern "C" __declspec(dllexport) void Ex_OnMouseMove(LPVOID lpPointer, UINT nFlags, CPoint point)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnMouseMove(nFlags, point);
}

extern "C" __declspec(dllexport) void Ex_OnPaint(LPVOID lpPointer, CDC* pDC)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnPaint(pDC);
}

extern "C" __declspec(dllexport) void Ex_OnHScroll(LPVOID lpPointer, UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnHScroll(nSBCode, nPos, pScrollBar);
}

extern "C" __declspec(dllexport) BOOL Ex_OnSelchange(LPVOID lpPointer, NMHDR* pNMHDR, LRESULT* pResult)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.OnSelchange(pNMHDR, pResult);

	return bResult;
}

extern "C" __declspec(dllexport) BOOL Ex_OnSelchanging(LPVOID lpPointer, NMHDR* pNMHDR, LRESULT* pResult)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.OnSelchanging(pNMHDR, pResult);

	return bResult;
}

extern "C" __declspec(dllexport) LRESULT Ex_OnMouseLeave(LPVOID lpPointer, WPARAM wParam, LPARAM lParam)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	LRESULT lResult = theApp.OnMouseLeave(wParam, lParam);

	return lResult;
}

extern "C" __declspec(dllexport) void Ex_OnKillFocus(LPVOID lpPointer, CWnd* pNewWnd)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnKillFocus(pNewWnd);
}

extern "C" __declspec(dllexport) void Ex_OnSetFocus(LPVOID lpPointer, CWnd* pOldWnd)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnSetFocus(pOldWnd);
}

extern "C" __declspec(dllexport) LRESULT Ex_WindowProc(LPVOID lpPointer, UINT message, WPARAM wParam, LPARAM lParam)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	LRESULT lResult = theApp.WindowProc(message, wParam, lParam);

	return lResult;
}

extern "C" __declspec(dllexport) void Ex_AddChildControl(LPVOID lpPointer, CWnd* pControl)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.AddChildControl(pControl);
}

extern "C" __declspec(dllexport) void Ex_RemoveChildControl(LPVOID lpPointer, CWnd* pControl)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.RemoveChildControl(pControl);
}

extern "C" __declspec(dllexport) void Ex_SetTabColor(LPVOID lpPointer, COLORREF clrBkgrnd, COLORREF clrActive, COLORREF clrInactive)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetTabColor(clrBkgrnd, clrActive, clrInactive);
}

extern "C" __declspec(dllexport) void Ex_SetSTTabColor(LPVOID lpPointer, long lSTTabColor)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetSTTabColor(lSTTabColor);
}

extern "C" __declspec(dllexport) void Ex_OnSize(LPVOID lpPointer, UINT nType, int cx, int cy)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnSize(nType, cx, cy);
}
