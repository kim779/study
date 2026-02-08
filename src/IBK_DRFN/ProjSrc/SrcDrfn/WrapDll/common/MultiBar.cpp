// MultiBar.cpp : implementation file
//

#include "stdafx.h"
//#include "wdrchart.h"
#include "MultiBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiBar

CMultiBar::CMultiBar() : m_bHover(FALSE), m_bTracking(FALSE)
{
	m_crBuy.CreateSolidBrush(RGB(255,0,0));
	m_crSell.CreateSolidBrush(RGB(0,0,255));
//	m_crBack.CreateSolidBrush(RGB(255,255,255));
//	m_crScale.CreateSolidBrush(RGB(192,192,192));
	m_crScalePen.CreatePen(PS_SOLID,1,RGB(88,89,95));

	//BOOL bRet = m_textFont.CreateFont(9,0,0,0, FW_NORMAL ,0, 0, 0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "굴림체");
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(LOGFONT));       // zero out structure
	
	logFont.lfHeight = 12;                      // request a 12-pixel-height font
	logFont.lfWeight = FW_NORMAL;
	logFont.lfCharSet = 0x81;				   // 한글 Character Set
	strcpy(logFont.lfFaceName, "굴림체");
	VERIFY(m_textFont.CreateFontIndirect(&logFont));  // create the font

	m_Manager.SetOwner(this);
}

CMultiBar::~CMultiBar()
{
	m_crBuy.DeleteObject();
	m_crSell.DeleteObject();
	m_crScalePen.DeleteObject();

	m_textFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMultiBar, CButton)
	//{{AFX_MSG_MAP(CMultiBar)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
// 	ON_WM_ERASEBKGND()


/////////////////////////////////////////////////////////////////////////////
// CMultiBar message handlers

int CMultiBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	return 0;
}

void CMultiBar::OnDestroy() 
{
	CButton::OnDestroy();
	
	// TODO: Add your message handler code here	
}

BOOL CMultiBar::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_ToolTip.m_hWnd)
		m_ToolTip.RelayEvent(pMsg);
	
	return CButton::PreTranslateMessage(pMsg);
}

// BOOL CMultiBar::OnEraseBkgnd(CDC* pDC) 
// {
// 	// TODO: Add your message handler code here and/or call default
// 	
// 	return CButton::OnEraseBkgnd(pDC);
// }
// 
void CMultiBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC* pDC = &dc;

	CRect rect, rcClient;
	GetClientRect(&rect);
	rcClient = rect;

	pDC->FillSolidRect(rect, RGB(238, 238, 238));

	rect.left += (m_Manager.m_ChartInfo.m_YTextMargin);
	rect.bottom -= (m_Manager.m_ChartInfo.m_XTextMargin);
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	
	// TODO: Add your message handler code here
	m_Manager.PrepareDraw(pDC, &rect, &rcClient);
	DrawTitle(&dc, &rect);

	// Do not call CButton::OnPaint() for painting messages
	LIST_CXScalInfo listXScale;
	if(m_Manager.GetCount()>0)
	{
		DrawBar(&dc, listXScale);

		pDC->SetBkMode(TRANSPARENT);

		DrawScale(&dc, listXScale);

		CRect yScale(rcClient);
		yScale.right = yScale.left+m_Manager.m_ChartInfo.m_YTextMargin;
		yScale.top += m_Manager.m_ChartInfo.m_nMarginTop;
		yScale.bottom = yScale.bottom - m_Manager.m_ChartInfo.m_XTextMargin;

		DoDrawYScale(pDC, yScale, m_Manager.m_ChartInfo.m_lMin, m_Manager.m_ChartInfo.m_lMax);
	}
	for(POSITION pos=listXScale.GetHeadPosition(); pos;) delete listXScale.GetNext(pos);

	DrawLine(&dc, &rect, &rcClient);
}

void CMultiBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);		
	}
	
	CButton::OnMouseMove(nFlags, point);
}

LRESULT CMultiBar::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover=TRUE;
	Invalidate();
	
 	if (m_ToolTip.m_hWnd)
 		if (m_ToolTip.GetToolCount() != 0) 
 			m_ToolTip.Activate(TRUE);

	return 0;
}


LRESULT CMultiBar::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;
	Invalidate();
	
 	if (m_ToolTip.m_hWnd)
 		if (m_ToolTip.GetToolCount() != 0) 
 			m_ToolTip.Activate(FALSE);
		
		return 0;
}


LRESULT CMultiBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( (message>=WM_MOUSEFIRST && message<=WM_MOUSELAST) && message!=WM_MOUSEMOVE)
		return 0L;
	
	return CButton::WindowProc(message, wParam, lParam);
}

void CMultiBar::SetPacketData(LPBYTE pPacketData, long nLen)
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create(this);
		m_ToolTip.Activate(FALSE);
	}

	m_Manager.SetTooltipInit(&m_ToolTip);

	char aSize[5+1];
	memcpy(aSize, pPacketData, 5); aSize[5]=0;
	int nStart = 5 + atoi(aSize);
	m_Manager.SetPacketData((char*)(pPacketData+nStart), nLen-nStart);
	Invalidate();
}

void CMultiBar::DrawTitle(CDC* pDC, CRect* pRect)
{
	CRect textRc(*pRect);


	//매도/매수
	int nOldMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF ordColor = pDC->GetTextColor();

	CSize sizeText = pDC->GetTextExtent("매도");
	HGDIOBJ hOld = pDC->SelectObject(m_textFont.GetSafeHandle());

	UINT nFormat=DT_SINGLELINE;

	textRc.left += 15; textRc.right=textRc.left+sizeText.cx;
	textRc.top +=5;
	textRc.bottom = textRc.top + 20;

	pDC->FillSolidRect(CRect(textRc.left-5, textRc.top+2, textRc.left-2, textRc.top+10),
		RGB(0, 50, 255));

	pDC->SetTextColor(RGB(0,50,255));
	pDC->DrawText("매도", textRc, nFormat);
	
	textRc.left=textRc.right;
	textRc.right=textRc.left+sizeText.cx;
	pDC->SetTextColor(RGB(255,0,0));
	pDC->DrawText("매수", textRc, nFormat);
	pDC->SelectObject(hOld);

	pDC->SetBkMode(nOldMode);
	pDC->SetTextColor(ordColor);

}

void CMultiBar::DrawBar(CDC* pDC, LIST_CXScalInfo& listXScale)
{
	m_Manager.DrawBar(pDC, listXScale);
}

void CMultiBar::DrawScale(CDC* pDC, LIST_CXScalInfo &listXScale)
{
//	HGDIOBJ hOldPen = pDC->SelectObject(m_crScalePen);
	HGDIOBJ hOld = pDC->SelectObject(m_textFont.GetSafeHandle());
	
	//pDC->SetBkColor(RGB(238, 238, 238));
	pDC->SetTextColor(RGB(0,0,0));

	UINT nFormat=DT_VCENTER|DT_WORDBREAK;
	for(POSITION pos=listXScale.GetHeadPosition(); pos; )
	{
		CXScalInfo* pItem = listXScale.GetNext(pos);
		if(pItem)
		{
 			pDC->DrawText(pItem->m_szText, pItem->m_rc, nFormat);
		}
	}

	pDC->SelectObject(hOld);
}

void CMultiBar::DrawLine(CDC* pDC, CRect* pRect, CRect* prcClient)
{
	HGDIOBJ hOldPen = pDC->SelectObject(m_crScalePen);
	//pDC->Rectangle(rect);
	pDC->MoveTo(pRect->left	, pRect->top); 
	pDC->LineTo(pRect->right	, pRect->top);
	pDC->LineTo(pRect->right	, pRect->bottom);
	pDC->LineTo(pRect->left	, pRect->bottom);
	pDC->LineTo(pRect->left	, pRect->top);
	
	//pDC->Rectangle(rcClient);
	pDC->MoveTo(prcClient->left	, prcClient->top); 
	pDC->LineTo(prcClient->right	, prcClient->top);
	pDC->LineTo(prcClient->right	, prcClient->bottom);
	pDC->LineTo(prcClient->left	, prcClient->bottom);
	pDC->LineTo(prcClient->left	, prcClient->top);
	
	pDC->SelectObject(hOldPen);
}

#define		SCALY_A1	2
#define		SCALY_A2	5
void CMultiBar::DoDrawYScale(CDC* pDC, CRect rc, long nMin, long nMax)
{
	HGDIOBJ hOldPen = pDC->SelectObject(m_crScalePen);
	HGDIOBJ hOld = pDC->SelectObject(m_textFont.GetSafeHandle());

	//pDC->SetBkColor(RGB(238, 238, 238));
	pDC->SetTextColor(RGB(0,0,0));

	int nTHeight = rc.Height();
	int nGap = 5;

	int nScaleCnt = nTHeight/nGap;
	long nMinMaxs = nMax;// - nMin;
	long nMinMax = nMinMaxs/nScaleCnt;

	POINT sPt, ePt;
	long nDispVal;

	{
		CString szDebug;
		szDebug.Format("Max = %ld, Min = %ld\n", nMax, nMin);
		OutputDebugString(szDebug);
	}

	for(int i=1; i<nScaleCnt; i++)
	{
		sPt.x=rc.right-SCALY_A1;
		if( (i%5)==0 )
		{
			sPt.x=rc.right-SCALY_A2;
			nDispVal = (nMinMax*i);
			if(nDispVal<100)
			{
				//nDispVal /= 10;
				//nDispVal = (nDispVal*10);
			}
			else if(nDispVal<1000)
			{
				nDispVal /= 10;
				nDispVal = (nDispVal*10);
			}
			else if(nDispVal<10000)
			{				
				nDispVal /= 100;
				nDispVal = (nDispVal*100);
			}
			else
			{
				nDispVal /= 1000;
				nDispVal = (nDispVal*1000);
			}
		}
		sPt.y=rc.bottom-(nGap*i);

		ePt.x=rc.right;
		ePt.y=sPt.y;

		pDC->MoveTo(sPt);
		pDC->LineTo(ePt);

		if( (i%5)==0 )
		{
			CString szDebug;
			szDebug.Format("nDispVal = %ld\n", nDispVal);
			OutputDebugString(szDebug);

			CString szValue;
			szValue.Format("%ld", nDispVal);
			szValue = GetComma(szValue);

			UINT nFormat=DT_SINGLELINE|DT_RIGHT;

			CSize size = pDC->GetTextExtent(szValue);

			CRect textRc;
			int x1,x2, y1, y2;
			x1=ePt.x-size.cx-1-SCALY_A2-1;
			x2=ePt.x-SCALY_A2-1;
			y1=(ePt.y-size.cy/2);
			y2=(ePt.y+size.cy/2)+1;
			textRc.SetRect(x1, y1, x2, y2);

			pDC->DrawText(szValue, textRc, nFormat);
		}

	}
	pDC->SelectObject(hOldPen);
	pDC->SelectObject(hOld);
}

CString CMultiBar::GetComma(LPCSTR pData)
{
	CString szRet=_T("");
	CString szTmp = pData;
//	szTmp.TrimLeft();
//	szTmp.TrimRight();
	
	if(szTmp.IsEmpty())	return "" ;
	
	szTmp.MakeReverse();
	int	nLen = szTmp.GetLength();	
	szRet.Empty();
	for(int i=0;   i<nLen;   i++) 
	{
		if(i!=0 && !(i%3))	szRet += ',';
		szRet += szTmp[i];
	}
	szRet.MakeReverse();	
	return szRet;
	
}

void CMultiBar::SetToolTipText(CRect* pRect, CString sTip)
{
	if (sTip.IsEmpty())
		sTip = "MultiBar";
	
	if (m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create(this);
		m_ToolTip.Activate(FALSE);
	}
	
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, sTip, rectBtn, 100);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText(sTip, this, 1);
}

/////////////////////////////////////////////////////////////////////////////
// CMBarChartInfo
CMBarChartInfo::CMBarChartInfo()
{
	DoClear();
}

void CMBarChartInfo::DoClear()
{
	m_nXDivCnt=13;
	m_nYDivCnt=25;
	m_lMin=0;
	m_lMax=0;
	m_nItemCnt=0;

	m_nMarginTop=10;
	m_nMarginLR = 1;

	m_YTextMargin=50;
	m_XTextMargin=55;
	m_YTopMargin=20;
}

/////////////////////////////////////////////////////////////////////////////
// CMBarItmManager
CMBarItmManager::CMBarItmManager()
{
	m_pToolTip = NULL;
}

void CMBarItmManager::ClearList()
{
	for(POSITION pos=m_List.GetHeadPosition(); pos;)
	{
		delete m_List.GetNext(pos);
	}
	m_List.RemoveAll();
}

void CMBarItmManager::PrepareDraw(CDC* pDC, CRect* pDrawRect, CRect* pChartRect)
{
	m_TextExtent = pDC->GetTextExtent("매");
	m_pDrawRect = pDrawRect;
	m_pChartRect = pChartRect;
}

void CMBarItmManager::SetTooltipInit(CToolTipCtrl* pToolTip)
{
	if(!m_pToolTip) m_pToolTip = pToolTip;	
	_DoClearTooltip();
}

void CMBarItmManager::_DoClearTooltip()
{
	if(m_pToolTip && m_pToolTip->GetSafeHwnd())
	{
		CWnd* pWnd = m_pOwner;
		if(pWnd && pWnd->GetSafeHwnd())
		{
			for(int i=0; i<m_List.GetCount(); i++)
			{
				m_pToolTip->DelTool(pWnd, 100+(i*2));
				m_pToolTip->DelTool(pWnd, 100+(i*2)+1);
			}
		}
	}
}

void CMBarItmManager::SetPacketData(char* pPacketData, long nLen)
{
	ClearList();

	int nOneItem = (20+12+12);
	int nCnt = nLen/nOneItem;
	
	long nMin=0, nMax=0;
	for(int i=0; i<nCnt; i++)
	{
		CMBarItem* pItem = new CMBarItem;
		pItem->SetItem(&pPacketData[i*nOneItem]);
		m_List.AddTail(pItem);

		if(i==0)
		{
			nMin = pItem->GetMin();
			nMax = pItem->GetMax();
		}
		else
		{
			nMin = min(nMin, pItem->GetMin());
			nMax = max(nMax, pItem->GetMax());
		}
	}
	m_ChartInfo.m_lMin = nMin;
	m_ChartInfo.m_lMax = nMax;
	m_ChartInfo.m_nItemCnt = m_List.GetCount();

	if(m_ChartInfo.m_nItemCnt>0) _DoCalcScale();
}

void CMBarItmManager::_DoCalcScale()
{
	int nXDivUnit = 25;
	int nXDivUnits_5=5, nXDivUnits_6=6;
	{
		long dYUnit_1 = m_ChartInfo.m_lMax/nXDivUnit;
		dYUnit_1 /= 100;
		dYUnit_1 *= 100;

		long dYUnit_5 = (dYUnit_1*nXDivUnits_5);

		long xIdealVal = dYUnit_5*nXDivUnits_5;

		int yDivCnt = nXDivUnit;
		if(xIdealVal<m_ChartInfo.m_lMax)
		{
			//@100210-alzioyes. 장전 계산시에 dYUnit_1값이 0이 발생해서 수정함.
			if(dYUnit_1==0) yDivCnt=1;
			else yDivCnt = (m_ChartInfo.m_lMax/dYUnit_1)+1;
		}

		m_ChartInfo.m_nYDivCnt = yDivCnt;
	}
}


void CMBarItmManager::DrawBar(CDC* pDC, LIST_CXScalInfo& listXScale)
{
	int nBars = m_List.GetCount();
	if(nBars==0) return;

	CString szToolText;
	CWnd* pParent = m_pOwner;
	int nToolID=100;

	int x, y, width, height;

	x = m_ChartInfo.m_YTextMargin,
	y = m_pDrawRect->bottom;

	//width = m_pDrawRect->Width()/(nBars*2);
	int width2 = m_pDrawRect->Width()/(nBars);
	int nDiff = m_pDrawRect->Width() -(width2*nBars);
	width = (width2/2);
	int nTipDiff = width2%2;

	CRect rc;
	int nDAHeight = GetDrawAllHeight();

	//Line
//	rc.SetRect(x+2, y-height, x+width, y);
//	pDC->MoveTo(x+2, rc.bottom+1);
//	pDC->LineTo(x+2, rc.bottom+5);

	CSize size=pDC->GetTextExtent("매");
	for(POSITION pos=m_List.GetHeadPosition(); pos;)
	{
		CMBarItem* pItem = m_List.GetNext(pos);

		double aVal = (double)pItem->m_ValueA/(double)m_ChartInfo.m_lMax;
		height = (int)(aVal*nDAHeight);
		rc.SetRect(x+2, y-height, x+width, y);
		pDC->FillSolidRect(&rc, RGB(0, 50, 255));
		x += (width+nTipDiff);
		szToolText.Format("%ld", pItem->m_ValueA);
		if(szToolText.GetLength()>=4)
			szToolText.Insert(szToolText.GetLength()-3, ",");
		m_pToolTip->AddTool(pParent, szToolText, rc, nToolID++);
		//m_pToolTip->UpdateTipText(szToolText, pParent, nToolID++);

		//Line
		pDC->MoveTo(rc.left, rc.bottom+1);
		pDC->LineTo(rc.left, rc.bottom+5);

		{
			CXScalInfo* pXItem = new CXScalInfo;
			//CRect textRc(rc.left, rc.bottom+6, rc.left+size.cx, rc.bottom+6+(size.cy*szValue.GetLength()));
			pXItem->m_szText = pItem->m_szItem;
			pXItem->m_rc.SetRect(rc.left+5, rc.bottom+6, rc.left+size.cx, rc.bottom+6+(size.cy*pItem->m_szItem.GetLength()));
			listXScale.AddTail(pXItem);

// 			CString szValue;
// 			szValue="외국인";
// 
// 			UINT nFormat=DT_VCENTER|DT_WORDBREAK;
// 			pDC->DrawText(szValue, textRc, nFormat);
		}

		//height = (int)(100*((nDAHeight/m_ChartInfo.m_lMax)*pItem->m_ValueB));
		aVal = (double)pItem->m_ValueB/(double)m_ChartInfo.m_lMax;
		height = (int)(aVal*nDAHeight);
		rc.SetRect(x, y-height, x+width-2, y);
		pDC->FillSolidRect(&rc, RGB(255, 0, 0));
		x += (width);
		szToolText.Format("%ld", pItem->m_ValueA);
		if(szToolText.GetLength()>=4)
			szToolText.Insert(szToolText.GetLength()-3, ",");
		m_pToolTip->AddTool(pParent, szToolText, rc, nToolID++);
		//m_pToolTip->UpdateTipText(szToolText, pParent, nToolID++);

		if(nDiff>0)
		{
			x += 1;
			nDiff--;
		}
	}
}

