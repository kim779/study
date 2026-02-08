// DrTabCtrl.cpp : implementation file
//

/************************************************/
/*																		          */
/*							DR Tab Control							    */
/*                                              */
/*																		          */
/*							작성일 : 2006. 07 ~							*/
/*						작성자 : 우동우 (Doori co.)		    */
/*																		          */
/************************************************/

#include "stdafx.h"
#include "DrTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrTabCtrl
CDrTabCtrl::CDrTabCtrl()
{
	m_crlSelText = RGB(0, 0, 0);
	m_crlUnSelText = RGB(0, 0, 0);
}

CDrTabCtrl::~CDrTabCtrl()
{
	m_fontBold.DeleteObject();
	m_fontDefault.DeleteObject();
	
	m_penSelectLine.DeleteObject();
	m_penUnSelectLine.DeleteObject();
	m_penOutLine.DeleteObject();
	m_penOutLine_In.DeleteObject();
}

BEGIN_MESSAGE_MAP(CDrTabCtrl, CTabCtrl)
//{{AFX_MSG_MAP(CDrTabCtrl)
ON_WM_CREATE()
ON_WM_SHOWWINDOW()
ON_WM_PAINT()
ON_WM_ERASEBKGND()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrTabCtrl message handlers

BOOL CDrTabCtrl::Create(int nTabPos, CWnd* pParentWnd, UINT nID) 
{
	BOOL bResult = FALSE;
	DWORD dwStyle = TCS_TABS | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
	
	if (nTabPos == 0)   //# Normal
	{
		
	}
	else if (nTabPos == 1)   //# Left
		dwStyle |= TCS_VERTICAL;
	bResult = CTabCtrl::Create(dwStyle, CRect(0, 0, 0, 0), pParentWnd, nID);
	
	m_nTabPos = nTabPos;
	
	SetColorType(0);
	SetFonts(600, 400);
	
	return bResult;
}


// #include "../../../Contents/inc/ISkinColorMng.h"
// #include "../../../Contents/inc/IKSLibMng.h"

void CDrTabCtrl::SetColorType(int nType)
{
	
// 	IKSLibManager*	pKSMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager, 1);	
// 	if(!pKSMng) return;
// 
// 
// 	COLORREF clrOutLine = pKSMng->Lib_KSLink_GetColor(98);
// 	
// 	COLORREF clrOutLine_In = pKSMng->Lib_KSLink_GetColor(99);
// 	COLORREF clrSelectOutLine_In = pKSMng->Lib_KSLink_GetColor(99);
// 	
// 	m_clrBoard	=m_clrBK = pKSMng->Lib_KSLink_GetColor(1);
// 	m_crlSel = 	 pKSMng->Lib_KSLink_GetColor(100);
// 	m_crlUnSel = pKSMng->Lib_KSLink_GetColor(104);
// 	m_crlSelText = pKSMng->Lib_KSLink_GetColor(101);
// 	m_crlUnSelText =pKSMng->Lib_KSLink_GetColor(105);
// 	
// 	m_penUnSelectLine.CreatePen(PS_SOLID, 1, clrOutLine);
// 	m_penSelectLine.CreatePen(PS_SOLID, 1, clrOutLine);
// 	
// 	m_penOutLine.CreatePen(PS_SOLID, 1 , clrOutLine);
// 	m_penOutLine_In.CreatePen(PS_SOLID, 1 , clrOutLine_In);
// 	m_penSelectOutLine_In.CreatePen(PS_SOLID, 1 , clrSelectOutLine_In);
	
}

int CDrTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1) return -1;
	
	
	return 0;
}

void CDrTabCtrl::PreSubclassWindow() 
{	
	CTabCtrl::PreSubclassWindow();
	//ModifyStyle(0, TCS_OWNERDRAWFIXED | TCS_SINGLELINE);
}

void CDrTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	
	//Set current font equal to default menu font.
	
	char buf[255];
	int nCurItem = lpDrawItemStruct->itemID;
	
	int nTabIndex = lpDrawItemStruct->itemID;
	if (nTabIndex < 0) 
		return;
	BOOL bSelected = (nTabIndex == GetCurSel());
	
	TCITEM tcitem;
	//Get text of item.
	tcitem.mask=TCIF_TEXT;
	tcitem.pszText =buf;
	tcitem.cchTextMax=sizeof(buf);
	GetItem(nCurItem, &tcitem);
	
	CRect rect=lpDrawItemStruct->rcItem;
	rect.OffsetRect(1, 5);
	
	/*
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
    */
	if (bSelected == TRUE)
    {
		dc.SetTextColor(m_crlSelText);
		dc.SelectObject(m_fontBold);
		dc.SetBkColor(m_crlSel);
    }
    else
    {
		dc.SetTextColor(m_crlUnSelText);
		dc.SelectObject(m_fontDefault);
		dc.SetBkColor(m_crlUnSel);
    }

	dc.SetBkMode(TRANSPARENT);
    if (m_nTabPos == 0)
		::DrawText(lpDrawItemStruct->hDC, buf, -1, &rect, DT_CENTER);
    else if (m_nTabPos == 1)
		::DrawText(lpDrawItemStruct->hDC, buf, -1, &rect, DT_CENTER | DT_VCENTER);
	
	//}	
	dc.Detach();
	dc.DeleteDC();
}

/*
void CDrTabCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{	
CRect rect = lpDrawItemStruct->rcItem;
int nTabIndex = lpDrawItemStruct->itemID;
if (nTabIndex < 0) 
return;
BOOL bSelected = (nTabIndex == GetCurSel());

  char label[64];
  TC_ITEM tci;
  tci.mask = TCIF_TEXT | TCIF_IMAGE;
  tci.pszText = label;     
  tci.cchTextMax = 63;
  
	if (!GetItem(nTabIndex, &tci)) 
	return;
	
	  CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	  if (!pDC) return;
	  int nSavedDC = pDC->SaveDC();
	  
		// For some bizarre reason the rcItem you get extends above the actual
		// drawing area. We have to workaround this "feature".
		
		  /*
		  rect.top += ::GetSystemMetrics(SM_CYEDGE);
		  if (m_lfEscapement == 900)
		  {
		  rect.bottom -= ::GetSystemMetrics(SM_CYEDGE);
		  rect.right += ::GetSystemMetrics(SM_CXEDGE);
		  }
		  */
		  
		  /*	
		  
			pDC->SetBkMode(TRANSPARENT);
			if (bSelected) 
			pDC->FillSolidRect(rect, m_crlSel);
			else
			{
			//rect.bottom += 2;
			pDC->FillSolidRect(rect, m_crlUnSel);
			//rect.bottom -= 2;
			}
			
			  // Draw image
			  CImageList* pImageList = GetImageList();
			  if (pImageList && tci.iImage >= 0) 
			  {
			  rect.left += pDC->GetTextExtent(_T(" ")).cx;		// Margin
			  
				// Get height of image so we 
				IMAGEINFO info;
				pImageList->GetImageInfo(tci.iImage, &info);
				CRect ImageRect(info.rcImage);
				int nYpos = rect.top;
				
				  pImageList->Draw(pDC, tci.iImage, CPoint(rect.left, nYpos), ILD_TRANSPARENT);
				  rect.left += ImageRect.Width();
				  }
				  
					if (bSelected) 
					{
					pDC->SetTextColor(m_crlSelText);
					pDC->SelectObject(&m_fontBold);
					
					  if (m_lfEscapement == 900)
					  {
					  rect.right += ::GetSystemMetrics(SM_CXEDGE) * 6;
					  pDC->DrawText(label, rect, DT_SINGLELINE | DT_BOTTOM | DT_CENTER);
					  }
					  else
					  {
					  rect.top += ::GetSystemMetrics(SM_CYEDGE);
					  pDC->DrawText(label, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					  }
					  }
					  else 
					  {
					  if (m_lfEscapement == 900)
					  rect.right += ::GetSystemMetrics(SM_CXEDGE) * 6;
					  
						pDC->SetTextColor(m_crlUnSelText);
						pDC->SelectObject(&m_fontDefault);
						pDC->DrawText(label, rect, DT_SINGLELINE | DT_BOTTOM | DT_CENTER);
						}
						
						  pDC->RestoreDC(nSavedDC);
}
*/
						  
/////////////////////////////////////////////////////////////////////////////
// CDrTabCtrl operations
						  
void CDrTabCtrl::SetTabColor(COLORREF crlBoard, COLORREF crlSel, COLORREF crlUnSel, COLORREF crlSelGraDown, COLORREF crlUnSelGraDown, COLORREF crlBody)
{
	m_clrBK = crlBoard;
	m_crlSel = crlSel;
	m_crlUnSel = crlUnSel;  
	m_clrBoard = crlBody;
	
	m_crlSelGraDown   = crlSelGraDown;
	m_crlUnSelGraDown = crlUnSelGraDown;
}

void CDrTabCtrl::SetBKColor(COLORREF crlBK)
{
	m_clrBK = crlBK;
}

void CDrTabCtrl::SetTextColor(COLORREF crlSel, COLORREF crlUnSel)
{
	m_crlSelText = crlSel;
	m_crlUnSelText = crlUnSel;
}

void CDrTabCtrl::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CTabCtrl::OnShowWindow(bShow, nStatus);
	
}

void CDrTabCtrl::SetFonts(int nSelWeight, int nUnselWeight)
{
	m_fontBold.DeleteObject();
	m_fontDefault.DeleteObject();
	
	// Get the current font
	LOGFONT lFont;
// 							  CFont *pFont = GetFont();
// 							  if (pFont)
// 								  pFont->GetLogFont(&lFont);
// 							  else 
// 							  {
// 								  NONCLIENTMETRICS ncm;
// 								  ncm.cbSize = sizeof(NONCLIENTMETRICS);
// 								  VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
// 								  lFont = ncm.lfMessageFont; 
// 							  }
	lFont.lfHeight	=	-12;    
	lFont.lfWidth	=	0;    
	lFont.lfEscapement=	0;    
	lFont.lfOrientation=	0; 
	lFont.lfWeight	=	FW_NORMAL;
	lFont.lfItalic	=	0;    
	lFont.lfUnderline	=	0;        
	lFont.lfStrikeOut	=	0;     
	lFont.lfCharSet	=	DEFAULT_CHARSET;
	lFont.lfOutPrecision	=	OUT_DEFAULT_PRECIS;        
	lFont.lfClipPrecision	=	CLIP_DEFAULT_PRECIS;     
	lFont.lfQuality	=	DEFAULT_QUALITY;        
	lFont.lfPitchAndFamily=	FF_ROMAN;        
	strcpy( lFont.lfFaceName, (const char*)_T("돋움체") );
	
	m_lfEscapement = lFont.lfEscapement;
	if (lFont.lfEscapement == 900)
		lFont.lfHeight -= 1;    
	
	// Create the "Selected" font
	lFont.lfWeight = nSelWeight;
	m_fontBold.CreateFontIndirect(&lFont);
	
	// Create the "Unselected" font
	lFont.lfWeight = nUnselWeight;
	m_fontDefault.CreateFontIndirect(&lFont);
	
	SetFont((nSelWeight > nUnselWeight)? &m_fontBold : &m_fontDefault);
}

void CDrTabCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rePaint, rcItem, reClient;
	GetClientRect(&reClient);
	
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, reClient.Width(), reClient.Height());
	CBitmap* pOldbitmap = MemDC.SelectObject(&bitmap);
	
	MemDC.FillSolidRect(&reClient, m_clrBK);
	
	HPEN hOldPen;
	hOldPen = (HPEN)::GetCurrentObject(dc.m_hDC, OBJ_PEN);
	int nCurSel = GetCurSel();
	
	{
		if (m_nTabPos == 0)
			OnPaint_DefType(&MemDC, reClient);
		else if (m_nTabPos == 1)
			OnPaint_LeftType(&MemDC, reClient);
	}
	dc.BitBlt(0, 0, reClient.Width(), reClient.Height(), &MemDC, 0, 0, SRCCOPY);
	
	::SelectObject(dc.m_hDC, hOldPen);
	MemDC.SelectObject(pOldbitmap);
	MemDC.DeleteDC();
}

BOOL CDrTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CDrTabCtrl::OnPaint_DefType(CPaintDC *pDC, CRect rePaint) 
{
  int nCurSel = GetCurSel();
  
  CRect reItem, reItem_Paint;
  int nCount = GetItemCount();
  
  CPen *pOldLinePen; 
  int nGap = 2;
  for (int nItem = 0; nItem < nCount; nItem++)
  {
	  GetItemRect(nItem, &reItem);
	  
	  reItem.right-=3;
	  reItem.top-=1;
	  reItem.bottom+=1;
	  if (reItem_Paint.IntersectRect(&reItem, &rePaint))
	  {
		  reItem_Paint = reItem;
		  
		  if (nItem != nCurSel)
		  {
			  reItem_Paint.top+=1;
			  reItem_Paint.left+=2;
			  pDC->FillSolidRect(&reItem_Paint, m_crlUnSel);    //# UnCurSel Color!
		  }
		  else
		  {
			  reItem_Paint.top+=1;
			  reItem_Paint.bottom+=1;
			  reItem_Paint.left+=1;
			  pDC->FillSolidRect(&reItem_Paint, m_crlSel);
		  }
		  
		  if (nItem != nCurSel)
		  {
			  pOldLinePen = pDC->SelectObject(&m_penUnSelectLine);
			  
			  pDC->MoveTo(reItem.left , reItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.
			  pDC->LineTo(reItem.left-4, reItem.bottom);
			  
			  pDC->MoveTo(reItem.right , reItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.	
			  pDC->LineTo(reItem.left , reItem.bottom );			// 바닥 외곽선 라인 그린다.				
			  pDC->LineTo(reItem.left, reItem.top + nGap);		// 좌측 외곽선 라인 그린다.				
		  }
		  else
		  {
			  pOldLinePen = pDC->SelectObject(&m_penSelectLine);
			  
			  pDC->MoveTo(reItem.left, reItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.
			  pDC->LineTo(reItem.left-4, reItem.bottom);
			  
			  pDC->MoveTo(reItem.left , reItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.	
			  pDC->LineTo(reItem.left, reItem.top + nGap);		// 좌측 외곽선 라인 그린다.		
		  }
		  
		  
		  
		  pDC->LineTo(reItem.left+1,  reItem.top + nGap);		// 좌측 상단 모서리 둥글게1
		  pDC->LineTo(reItem.left+1, reItem.top +1);	// 좌측 상단 모서리 둥글게2
		  pDC->LineTo(reItem.left+2, reItem.top +1);	// 좌측 상단 모서리 둥글게3
		  pDC->LineTo(reItem.left+nGap + 1, reItem.top);				// 좌측 상단 모서리 둥글게4
		  
		  
		  pDC->LineTo(reItem.right - nGap - 1, reItem.top);			// 상단 외곽선 그린다.
		  
		  pDC->LineTo(reItem.right - nGap,		reItem.top + nGap - 1);	// 우측 상단 모서리 둥글게1
		  pDC->LineTo(reItem.right - nGap + 1,	reItem.top + nGap - 1);	// 우측 상단 모서리 둥글게2
		  pDC->LineTo(reItem.right - nGap + 1,  reItem.top + nGap);		// 우측 상단 모서리 둥글게3
		  pDC->LineTo(reItem.right , reItem.top + nGap);					// 우측 상단 모서리 둥글게4
		  
		  pDC->LineTo(reItem.right , reItem.bottom );	// 우측 외곽선 라인 그린다.
		  
		  
		  // 안쪽테두리
		  //if(nItem != nCurSel)
		  //{
		  pDC->SelectObject(&m_penOutLine_In);
		  
		  pDC->MoveTo(reItem.left+1, reItem.top + 3);		// 좌측선
		  pDC->LineTo(reItem.left+1, reItem.bottom);
		  
		  pDC->MoveTo(reItem.left+1, reItem.top + 3);		// 좌측 상단 모서리 둥글게1
		  pDC->LineTo(reItem.left+3, reItem.top + 1);		// 좌측 상단 모서리 둥글게2
		  
		  pDC->MoveTo(reItem.left+1, reItem.top + 4);
		  pDC->LineTo(reItem.left+4, reItem.top + 1);
		  
		  pDC->MoveTo(reItem.left+3, reItem.top + 1);		// 상단 라인
		  pDC->LineTo(reItem.right-2, reItem.top+ 1);
		  //}				
		  
		  pDC->SelectObject(pOldLinePen);
		  
		  DRAWITEMSTRUCT drawItemStruct;
		  drawItemStruct.rcItem  =reItem;
		  drawItemStruct.hDC = pDC->GetSafeHdc();
		  drawItemStruct.itemID = nItem;
		  DrawItem(&drawItemStruct);
	  }
  }
  
  pOldLinePen = pDC->SelectObject(&m_penOutLine);
  
  CRect reClient;
  GetClientRect(&reClient);
  reClient.top = reItem.bottom;
  reClient.right -= 1;
  reClient.bottom -= 2;
  
  pDC->MoveTo(reClient.left, reClient.top);
  pDC->LineTo(reClient.right, reClient.top);
  pDC->LineTo(reClient.right, reClient.bottom);
  pDC->LineTo(reClient.left, reClient.bottom);
  pDC->LineTo(reClient.left, reClient.top);
  
  if (m_clrBK != m_clrBoard)
  {
	  reClient.top += 1;
	  reClient.bottom -= 1;
	  reClient.left += 1;
	  reClient.right -= 1;
	  pDC->FillSolidRect(&reClient, m_clrBoard);
  }
  
  pDC->SelectObject(pOldLinePen);
}

void CDrTabCtrl::OnPaint_LeftType(CPaintDC *pDC, CRect rePaint) 
{
	int nCurSel = GetCurSel();
	
	CRect reItem, reItem_Paint;
	int nCount = GetItemCount();
	
	CPen *pOldLinePen; 
	int nGap = 2;
	for (int nItem = 0; nItem < nCount; nItem++)
	{
		GetItemRect(nItem, &reItem);
		
		/*
		reItem.right-=3;
		reItem.top-=1;
		reItem.bottom+=1;
		*/
		
		reItem.bottom-=3;
		reItem.left-=1;
		reItem.right+=1;
		if (reItem_Paint.IntersectRect(&reItem, &rePaint))
		{
			reItem_Paint = reItem;
			
			if (nItem != nCurSel)
			{
			/*
			reItem_Paint.top+=1;
			reItem_Paint.left+=2;
				*/
				reItem_Paint.left+=1;
				reItem_Paint.top+=2;
				pDC->FillSolidRect(&reItem_Paint, m_crlUnSel);    //# UnCurSel Color!
			}
			else
			{
			/*
			reItem_Paint.top+=1;
			reItem_Paint.bottom+=1;
			reItem_Paint.left+=1;
				*/
				reItem_Paint.left+=1;
				reItem_Paint.right+=1;
				reItem_Paint.top+=1;
				
				pDC->FillSolidRect(&reItem_Paint, m_crlSel);
			}
			
			if (nItem != nCurSel)
				pOldLinePen = pDC->SelectObject(&m_penUnSelectLine);
			else
				pOldLinePen = pDC->SelectObject(&m_penSelectLine);
			
			pDC->MoveTo(reItem.right, reItem.top - 4);			// 탭 우하단으로 Drawing Point  옮김.
			pDC->LineTo(reItem.right, reItem.bottom);
			
			//pDC->MoveTo(reItem.left, reItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.	
			pDC->LineTo(reItem.left + nGap - 1, reItem.bottom);		// 좌측 외곽선 라인 그린다.	
			
																	/*
																	CPen penTemp;
																	penTemp.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
																	pDC->SelectObject(&penTemp);
																	
																	  pDC->MoveTo(reItem.left, reItem.top);
																	  pDC->LineTo(reItem.right, reItem.top);
																	  pDC->LineTo(reItem.right, reItem.bottom);
																	  pDC->LineTo(reItem.left, reItem.bottom);
																	  pDC->LineTo(reItem.left, reItem.top);
			*/
			
			pDC->LineTo(reItem.left, reItem.bottom - 1);	// 우측 상단 모서리 둥글게1
			pDC->LineTo(reItem.left, reItem.bottom - nGap);	// 우측 상단 모서리 둥글게2
			//pDC->LineTo(reItem.left - 1,  reItem.top + nGap);		// 우측 상단 모서리 둥글게3
			pDC->LineTo(reItem.left, reItem.top + nGap);					// 우측 상단 모서리 둥글게4
			
			//      pDC->LineTo(reItem.left, reItem.top + nGap);
			
			pDC->LineTo(reItem.left+1, reItem.top + nGap);		// 좌측 상단 모서리 둥글게1
			pDC->LineTo(reItem.left+1, reItem.top +1);	      // 좌측 상단 모서리 둥글게2
			pDC->LineTo(reItem.left+2, reItem.top +1);	      // 좌측 상단 모서리 둥글게3
			pDC->LineTo(reItem.left + nGap + 1, reItem.top);		// 좌측 상단 모서리 둥글게4
			
			pDC->LineTo(reItem.right, reItem.top);
			
			
			pDC->SelectObject(&m_penOutLine_In);
			
			pDC->MoveTo(reItem.left+1, reItem.top + 3);		// 좌측선
			pDC->LineTo(reItem.left+1, reItem.bottom);
			
			pDC->MoveTo(reItem.left+1, reItem.top + 3);		// 좌측 상단 모서리 둥글게1
			pDC->LineTo(reItem.left+3, reItem.top + 1);		// 좌측 상단 모서리 둥글게2
			
			pDC->MoveTo(reItem.left+1, reItem.top + 4);
			pDC->LineTo(reItem.left+4, reItem.top + 1);
			
			pDC->MoveTo(reItem.left+3, reItem.top + 1);		// 상단 라인
			pDC->LineTo(reItem.right, reItem.top+ 1);
			
			pDC->SelectObject(pOldLinePen);
            
			DRAWITEMSTRUCT drawItemStruct;
			drawItemStruct.rcItem  =reItem;
			drawItemStruct.hDC = pDC->GetSafeHdc();
			drawItemStruct.itemID = nItem;
			DrawItem(&drawItemStruct);
		}
	}
	
	
	pOldLinePen = pDC->SelectObject(&m_penOutLine);
	
	CRect reClient;
	GetClientRect(&reClient);
	reClient.left = reItem.right;
	reClient.right -= 1;
	reClient.bottom -= 2;
    
	pDC->MoveTo(reClient.left, reClient.top);
	pDC->LineTo(reClient.right, reClient.top);
	pDC->LineTo(reClient.right, reClient.bottom);
	pDC->LineTo(reClient.left, reClient.bottom);
	pDC->LineTo(reClient.left, reClient.top);
	
	if (m_clrBK != m_clrBoard)
	{
		reClient.top += 1;
		reClient.bottom -= 1;
		reClient.left += 1;
		reClient.right -= 1;
		pDC->FillSolidRect(&reClient, m_clrBoard);
	}
	
	pDC->SelectObject(pOldLinePen);
}


void CDrTabCtrl::OnPaint_DefType(CDC *pDC, CRect rePaint) 
{
	int nCurSel = GetCurSel();
	
	CRect reItem, reItem_Paint;
	int nCount = GetItemCount();
	
	CPen *pOldLinePen; 
	int nGap = 0;
	for (int nItem = 0; nItem < nCount; nItem++)
	{
		GetItemRect(nItem, &reItem);

	//	reItem.right-=3;
		reItem.left-=2;
		reItem.top-=1;
//		reItem.bottom+=1;		
		if (reItem_Paint.IntersectRect(&reItem, &rePaint))
		{
			if(nItem > 0)
				reItem.left += 4;
			pDC->FillSolidRect(reItem.left, reItem.top, 2, reItem.Height(), m_clrBoard);
			reItem_Paint = reItem;
			
			if (nItem != nCurSel)
			{
				reItem_Paint.bottom+=1;

				int RSValue, GSValue, BSValue;
				RSValue = GetRValue(m_crlUnSel);
				GSValue = GetGValue(m_crlUnSel);
				BSValue = GetBValue(m_crlUnSel);
				int RFValue, GFValue, BFValue;
				RFValue = GetRValue(m_crlUnSelGraDown);
				GFValue = GetGValue(m_crlUnSelGraDown);
				BFValue = GetBValue(m_crlUnSelGraDown);  
				
				TRIVERTEX        vert[2] ;
				GRADIENT_RECT    gRect;
				vert [0] .x      = reItem_Paint.left;
				vert [0] .y      = reItem_Paint.top;
				vert [0] .Red    = RSValue*16*16;
				vert [0] .Green  = GSValue*16*16;
				vert [0] .Blue   = BSValue*16*16;
				vert [0] .Alpha  = 0x0000;
				
				vert [1] .x      = reItem_Paint.right;
				vert [1] .y      = reItem_Paint.bottom;
				vert [1] .Red    = RFValue*16*16;
				vert [1] .Green  = GFValue*16*16;
				vert [1] .Blue   = BFValue*16*16;
				vert [1] .Alpha  = 0x0000;
				
				gRect.UpperLeft  = 1;
				gRect.LowerRight = 0;
//20100218 김한두 >> NH 포팅
			//	GradientFill(pDC->m_hDC, vert, 2, &gRect,1,GRADIENT_FILL_RECT_V);
//20100218 김한두 <<
				reItem_Paint.top+=1;
				reItem_Paint.left+=2;	
				reItem_Paint.bottom+=1;
				pDC->FillSolidRect(&reItem_Paint, m_crlUnSel);    //# UnCurSel Color!
			}
			else
			{
				reItem_Paint.bottom+=1;

				int RSValue, GSValue, BSValue;
				RSValue = GetRValue(m_crlSel);
				GSValue = GetGValue(m_crlSel);
				BSValue = GetBValue(m_crlSel);
				int RFValue, GFValue, BFValue;
				RFValue = GetRValue(m_crlSelGraDown);
				GFValue = GetGValue(m_crlSelGraDown);
				BFValue = GetBValue(m_crlSelGraDown);  
				
				TRIVERTEX        vert[2] ;
				GRADIENT_RECT    gRect;
				vert [0] .x      = reItem_Paint.left;
				vert [0] .y      = reItem_Paint.top;
				vert [0] .Red    = RSValue*16*16;
				vert [0] .Green  = GSValue*16*16;
				vert [0] .Blue   = BSValue*16*16;
				vert [0] .Alpha  = 0x0000;
				
				vert [1] .x      = reItem_Paint.right;
				vert [1] .y      = reItem_Paint.bottom;
				vert [1] .Red    = RFValue*16*16;
				vert [1] .Green  = GFValue*16*16;
				vert [1] .Blue   = BFValue*16*16;
				vert [1] .Alpha  = 0x0000;
				
				gRect.UpperLeft  = 1;
				gRect.LowerRight = 0;
		//		GradientFill(pDC->m_hDC, vert, 2, &gRect,1,GRADIENT_FILL_RECT_V);
				pDC->FillSolidRect(reItem_Paint.left, reItem_Paint.top + reItem_Paint.Height()/2 + 1, reItem_Paint.Width(), reItem_Paint.Height()/2,m_crlSelGraDown); 

				reItem_Paint.top+=1;	
				reItem_Paint.left+=1;
				reItem_Paint.bottom+=1;
				pDC->FillSolidRect(&reItem_Paint, m_crlSel);
			}
			
			if (nItem != nCurSel)
			{
				pOldLinePen = pDC->SelectObject(&m_penUnSelectLine);
				
			//	pDC->MoveTo(reItem.left , reItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.
			//	pDC->LineTo(reItem.left-4, reItem.bottom);
				
				pDC->MoveTo(reItem.right , reItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.	
				pDC->MoveTo(reItem.left , reItem.bottom );			// 바닥 외곽선 라인 그린다.				
				pDC->LineTo(reItem.left , reItem.top + nGap);		// 좌측 외곽선 라인 그린다.	
				
				pDC->MoveTo(reItem.right , reItem.bottom );			// 바닥 외곽선 라인 그린다.				
				pDC->LineTo(reItem.right , reItem.top + nGap);
				
			}
			else
			{
				pOldLinePen = pDC->SelectObject(&m_penSelectLine);
				
// 				pDC->MoveTo(reItem.left, reItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.
// 				pDC->LineTo(reItem.left-4, reItem.bottom);
				
				pDC->MoveTo(reItem.left , reItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.	
				pDC->LineTo(reItem.left, reItem.top + nGap);		// 좌측 외곽선 라인 그린다.		
		
				pDC->MoveTo(reItem.right , reItem.bottom );			// 바닥 외곽선 라인 그린다.				
				pDC->LineTo(reItem.right , reItem.top + nGap);
			}
			
			
			
// 			pDC->LineTo(reItem.left+1,  reItem.top + nGap);		// 좌측 상단 모서리 둥글게1
// 			pDC->LineTo(reItem.left+1, reItem.top +1);	// 좌측 상단 모서리 둥글게2
// 			pDC->LineTo(reItem.left+2, reItem.top +1);	// 좌측 상단 모서리 둥글게3
// 			pDC->LineTo(reItem.left+nGap + 1, reItem.top);				// 좌측 상단 모서리 둥글게4
// 			
// 			pDC->LineTo(reItem.right - nGap - 1, reItem.top);			// 상단 외곽선 그린다.

			pDC->LineTo(reItem.left,  reItem.top);		
			pDC->LineTo(reItem.right, reItem.top);			// 상단 외곽선 그린다.
			
// 			pDC->LineTo(reItem.right - nGap,		reItem.top + nGap - 1);	// 우측 상단 모서리 둥글게1
// 			pDC->LineTo(reItem.right - nGap + 1,	reItem.top + nGap - 1);	// 우측 상단 모서리 둥글게2
// 			pDC->LineTo(reItem.right - nGap + 1,  reItem.top + nGap);		// 우측 상단 모서리 둥글게3
// 			pDC->LineTo(reItem.right , reItem.top + nGap);					// 우측 상단 모서리 둥글게4
			
			pDC->LineTo(reItem.right, reItem.top );
			pDC->LineTo(reItem.right, reItem.bottom );	// 우측 외곽선 라인 그린다.
			
			
			// 안쪽테두리
			//if(nItem != nCurSel)
			//{
// 			if(nItem != nCurSel)
// 				pDC->SelectObject(&m_penOutLine_In);
// 			else
// 				pDC->SelectObject(&m_penSelectOutLine_In);
			
// 			pDC->MoveTo(reItem.left+1, reItem.top + 3);		// 좌측선
// 			pDC->LineTo(reItem.left+1, reItem.bottom);
// 			
// 			pDC->MoveTo(reItem.left+1, reItem.top + 3);		// 좌측 상단 모서리 둥글게1
// 			pDC->LineTo(reItem.left+3, reItem.top + 1);		// 좌측 상단 모서리 둥글게2
			pDC->MoveTo(reItem.left, reItem.top);
			pDC->LineTo(reItem.left, reItem.bottom);

			
// 			pDC->MoveTo(reItem.left+1, reItem.top + 4);
// 			pDC->LineTo(reItem.left+4, reItem.top + 1);
// 			
// 			pDC->MoveTo(reItem.left +3, reItem.top + 1);		// 상단 라인
// 			pDC->LineTo(reItem.right-4, reItem.top+ 1);
			pDC->MoveTo(reItem.left, reItem.top);
 			pDC->LineTo(reItem.left, reItem.top);

			//우측선
			if(nCurSel != nCount-1)
			{			
// 				pDC->MoveTo(reItem.right-4, reItem.top +1);
// 				pDC->LineTo(reItem.right-2, reItem.top +3);
// 
// 				pDC->MoveTo(reItem.right-5, reItem.top +1);
// 				pDC->LineTo(reItem.right-2, reItem.top +4);
// 
// 				pDC->MoveTo(reItem.right-2, reItem.top +3);
// 				pDC->LineTo(reItem.right-2, reItem.bottom);

				pDC->MoveTo(reItem.right, reItem.top );
				pDC->LineTo(reItem.right, reItem.bottom);
			}
			else
			{
// 				pDC->MoveTo(reItem.right-4, reItem.top +1);
// 				pDC->LineTo(reItem.right-1, reItem.top +3);
// 
// 				pDC->MoveTo(reItem.right-5, reItem.top +1);
// 				pDC->LineTo(reItem.right-1, reItem.top +4);
// 
// 				pDC->MoveTo(reItem.right-1, reItem.top +3);
// 				pDC->LineTo(reItem.right-1, reItem.bottom);
				pDC->MoveTo(reItem.right, reItem.top );
				pDC->LineTo(reItem.right, reItem.bottom);
			}

			//}				
			
			pDC->SelectObject(pOldLinePen);
            
			DRAWITEMSTRUCT drawItemStruct;
			drawItemStruct.rcItem  =reItem;
			drawItemStruct.hDC = pDC->GetSafeHdc();
			drawItemStruct.itemID = nItem;
			DrawItem(&drawItemStruct);
		}
	}
    
//	pOldLinePen = pDC->SelectObject(&m_penOutLine);

//	CPen pen(PS_SOLID, 1, m_crlSel);


	pOldLinePen = pDC->SelectObject(&m_penUnSelectLine);
	
	CRect reClient;
	GetClientRect(&reClient);
	reClient.top = reItem.bottom+1;
	reClient.right -= 1;
	reClient.bottom -= 2;
	if(reClient.bottom >= reItem.bottom)
	{
		pDC->MoveTo(reClient.left, reClient.top);
		pDC->LineTo(reClient.right, reClient.top);
		pDC->LineTo(reClient.right, reClient.bottom);
		pDC->LineTo(reClient.left, reClient.bottom);
		pDC->LineTo(reClient.left, reClient.top);
		
		if (m_clrBK != m_clrBoard)
		{
			reClient.top += 1;
			reClient.bottom -= 1;
			reClient.left += 1;
			reClient.right -= 1;
			pDC->FillSolidRect(&reClient, m_clrBoard);
		}
	}
	pDC->SelectObject(pOldLinePen);
}

void CDrTabCtrl::OnPaint_LeftType(CDC *pDC, CRect rePaint) 
{
	int nCurSel = GetCurSel();
	
	CRect reItem, reItem_Paint;
	int nCount = GetItemCount();
	
	CPen *pOldLinePen; 
	int nGap = 2;
	for (int nItem = 0; nItem < nCount; nItem++)
	{
		GetItemRect(nItem, &reItem);	
		
		reItem.bottom-=3;
		reItem.left-=1;
		reItem.right+=1;
		if (reItem_Paint.IntersectRect(&reItem, &rePaint))
		{
			reItem_Paint = reItem;
			
			if (nItem != nCurSel)
			{	
				reItem_Paint.left+=1;
				reItem_Paint.top+=2;
				pDC->FillSolidRect(&reItem_Paint, m_crlUnSel);    //# UnCurSel Color!
			}
			else
			{
				reItem_Paint.left+=1;
				reItem_Paint.right+=1;
				reItem_Paint.top+=1;
				
				pDC->FillSolidRect(&reItem_Paint, m_crlSel);
			}
			
			if (nItem != nCurSel)
				pOldLinePen = pDC->SelectObject(&m_penUnSelectLine);
			else
				pOldLinePen = pDC->SelectObject(&m_penSelectLine);
			
			pDC->MoveTo(reItem.right, reItem.top - 4);			// 탭 우하단으로 Drawing Point  옮김.
			pDC->LineTo(reItem.right, reItem.bottom);
			
			//pDC->MoveTo(reItem.left, reItem.bottom);			// 탭 우하단으로 Drawing Point  옮김.	
			pDC->LineTo(reItem.left + nGap - 1, reItem.bottom);		// 좌측 외곽선 라인 그린다.	
			
																	/*
																	CPen penTemp;
																	penTemp.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
																	pDC->SelectObject(&penTemp);
																	
																	  pDC->MoveTo(reItem.left, reItem.top);
																	  pDC->LineTo(reItem.right, reItem.top);
																	  pDC->LineTo(reItem.right, reItem.bottom);
																	  pDC->LineTo(reItem.left, reItem.bottom);
																	  pDC->LineTo(reItem.left, reItem.top);
			*/
			
			pDC->LineTo(reItem.left, reItem.bottom - 1);	// 우측 상단 모서리 둥글게1
			pDC->LineTo(reItem.left, reItem.bottom - nGap);	// 우측 상단 모서리 둥글게2
			//pDC->LineTo(reItem.left - 1,  reItem.top + nGap);		// 우측 상단 모서리 둥글게3
			pDC->LineTo(reItem.left, reItem.top + nGap);					// 우측 상단 모서리 둥글게4
			
			//      pDC->LineTo(reItem.left, reItem.top + nGap);
			
			pDC->LineTo(reItem.left+1, reItem.top + nGap);		// 좌측 상단 모서리 둥글게1
			pDC->LineTo(reItem.left+1, reItem.top +1);	      // 좌측 상단 모서리 둥글게2
			pDC->LineTo(reItem.left+2, reItem.top +1);	      // 좌측 상단 모서리 둥글게3
			pDC->LineTo(reItem.left + nGap + 1, reItem.top);		// 좌측 상단 모서리 둥글게4
			
			pDC->LineTo(reItem.right, reItem.top);
			
			
			pDC->SelectObject(&m_penOutLine_In);
			
			pDC->MoveTo(reItem.left+1, reItem.top + 3);		// 좌측선
			pDC->LineTo(reItem.left+1, reItem.bottom);
			
			pDC->MoveTo(reItem.left+1, reItem.top + 3);		// 좌측 상단 모서리 둥글게1
			pDC->LineTo(reItem.left+3, reItem.top + 1);		// 좌측 상단 모서리 둥글게2
			
			pDC->MoveTo(reItem.left+1, reItem.top + 4);
			pDC->LineTo(reItem.left+4, reItem.top + 1);
			
			pDC->MoveTo(reItem.left+3, reItem.top + 1);		// 상단 라인
			pDC->LineTo(reItem.right, reItem.top+ 1);
			
			pDC->SelectObject(pOldLinePen);
            
			DRAWITEMSTRUCT drawItemStruct;
			drawItemStruct.rcItem  =reItem;
			drawItemStruct.hDC = pDC->GetSafeHdc();
			drawItemStruct.itemID = nItem;
			DrawItem(&drawItemStruct);
		}
	}
	
	
	pOldLinePen = pDC->SelectObject(&m_penOutLine);
	
	CRect reClient;
	GetClientRect(&reClient);
	reClient.left = reItem.right;
	reClient.right -= 1;
	reClient.bottom -= 2;
	
	if(reClient.bottom >= reItem.bottom)
	{ 
		pDC->MoveTo(reClient.left, reClient.top);
		pDC->LineTo(reClient.right, reClient.top);
		pDC->LineTo(reClient.right, reClient.bottom);
		pDC->LineTo(reClient.left, reClient.bottom);
		pDC->LineTo(reClient.left, reClient.top);
		
		if (m_clrBK != m_clrBoard)
		{
			reClient.top += 1;
			reClient.bottom -= 1;
			reClient.left += 1;
			reClient.right -= 1;
			pDC->FillSolidRect(&reClient, m_clrBoard);
		}
	}
	
	pDC->SelectObject(pOldLinePen);
}
