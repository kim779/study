// ChartToolBtn.cpp : implementation file
//

#include "stdafx.h"
#include "ChartToolBtn.h"
#include "ctrlnotify.h"
#include "..\define\ChartMsgDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChartToolBtn
#define BT_DEF_NORMAL		1
#define BT_DEF_UP			2
#define BT_DEF_DOWN			3

#define BT_XGAP				6	/* 글씨보다 큰 버튼 길이 */
#define BT_YGAP				8   /* 글씨보다 큰 버튼 길이 */

#define BT_GRADIATE_GAP		5	/* 그라데이션 색 갭 */
#define	BT_MENUBTN_GAP	   10	/* 메뉴버튼 size	*/

#define UP_TOGGLE			0
#define DOWN_TOGGLE			1

#define UDM_CHARTBUTTON_CLICK		WM_USER + 602

CChartToolBtn::CChartToolBtn()
{
	m_nBtDown = BT_DEF_NORMAL;
	m_bDowned = FALSE;
	m_strTitle = "";
	m_strTR	= "";
	m_bFixedSz = FALSE;
	m_FixedSz.cx = 0;	
	m_FixedSz.cy = 0;
	m_pBmp = NULL;
	m_bMenuBtn = FALSE;
	m_pUpBmp = NULL;
	m_pDownBmp = NULL;
	m_pNormalBmp = NULL;
	m_bToggle = FALSE;
	m_nCurrToggle = UP_TOGGLE;

	m_pImageList = NULL;
	m_nUp = -1;
	m_nDown = -1;
	m_nNormal = -1;
}

CChartToolBtn::~CChartToolBtn()
{
	DestroyWindow();
}

BEGIN_MESSAGE_MAP(CChartToolBtn, CWnd)
	//{{AFX_MSG_MAP(CChartToolBtn)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChartToolBtn message handlers

CPoint CChartToolBtn::CreateBtn(CWnd *pParent, CFont *pFont, CPoint ptStart, int nID)
{
	m_pParent = pParent;
	m_ptStart = ptStart;
	m_nID = nID;
	m_rcRect = CRect(0,0,0,0);

	if(!Create(NULL, "", WS_VISIBLE | WS_CHILD | WS_TABSTOP, m_rcRect, m_pParent, m_nID))
		return FALSE;

	LOGFONT	lf; ::memset(&lf, 0x00, sizeof(LOGFONT));
	m_pFont = pFont;
	pFont->GetLogFont(&lf);
	m_nFHeight = lf.lfHeight;
	m_nFWidth = lf.lfWidth;

	CalcRect(ptStart, m_strTitle, m_rcRect);
	MoveWindow(&m_rcRect, TRUE);
	InvalidateRect(NULL, TRUE);

	m_ptEnd.x = m_rcRect.right;
	m_ptEnd.y = m_rcRect.top;

	return m_ptEnd;
}

BOOL CChartToolBtn::CreateBtn(CWnd *pParent, CFont *pFont, int nID)
{
	LOGFONT	lf; ::memset(&lf, 0x00, sizeof(LOGFONT));
	m_pFont = pFont;
	pFont->GetLogFont(&lf);
	m_nFHeight = lf.lfHeight;
	m_nFWidth = lf.lfWidth;

	m_pParent = pParent;
	m_nID = nID;
	m_rcRect = CRect(0,0,0,0);

	CPoint	ptStart(0, 0);
	
//	CalcRect(ptStart, m_strTitle, m_rcRect);
	if(!Create(NULL, "", WS_VISIBLE | WS_CHILD | WS_TABSTOP, m_rcRect, m_pParent, m_nID))
		return FALSE;
	
	return TRUE;
}

// 2002. 8. 30 Hedges
void CChartToolBtn::SetValueBtn(CToolOptionInfo::TOOLOPTION tooltype, BOOL bToggle, CBitmap *pNormalBmp, CBitmap *pUpBmp, CBitmap *pDownBmp)
{
	m_ToolOption = tooltype;
	m_bToggle = bToggle;
	m_bFixedSz = TRUE;
	m_pUpBmp = pUpBmp;
	m_pDownBmp = pDownBmp;
	m_pNormalBmp = pNormalBmp;
}

// 2006. 1. 27 LYH
void CChartToolBtn::SetValueBtn(CToolOptionInfo::TOOLOPTION tooltype, BOOL bToggle, CImageList* pImageList, int nNormalIndex, int nUpIndex, int nDownIndex)
{
	m_ToolOption = tooltype;
	m_bToggle = bToggle;
	m_bFixedSz = TRUE;
	m_pImageList = pImageList;
	m_nUp = nUpIndex;
	m_nDown = nDownIndex;
	m_nNormal = nNormalIndex;
}

void CChartToolBtn::SetValueBtn(int nAnalyType, int nMessage, BOOL bToggle, CSize *sz, CSize *szBackBmp, 
								CBitmap *pNormalBmp, CBitmap *pUpBmp, CBitmap *pDownBmp)
{
	m_nAnalyType = nAnalyType;
	m_nMessage = nMessage;
	m_bToggle = bToggle;
	m_strTitle = "";
	m_strTR	= "";
	m_bFixedSz = TRUE;
	if(sz != NULL)
	{
		m_FixedSz.cx = sz->cx;	m_FixedSz.cy = sz->cy;
	}
	m_pBmp = NULL;
	m_bMenuBtn = FALSE;
	if(szBackBmp)
	{
		m_BackBmpSz.cx = szBackBmp->cx;
		m_BackBmpSz.cy = szBackBmp->cy;
		m_pUpBmp = pUpBmp;
		m_pDownBmp = pDownBmp;
		m_pNormalBmp = pNormalBmp;
	}
	else
	{
		m_pUpBmp = NULL;
		m_pDownBmp = NULL;
		m_pNormalBmp = NULL;
	}
}

CPoint CChartToolBtn::Replace(CPoint ptStart)
{
	m_ptStart	= ptStart;
	CalcRect(ptStart, m_strTitle, m_rcRect);

	if(m_bMenuBtn)
		m_rcRect.right = m_rcRect.left + BT_MENUBTN_GAP;

	MoveWindow(&m_rcRect, TRUE);
	m_ptEnd.x = m_rcRect.right;
	m_ptEnd.y = m_rcRect.top;

	return m_ptEnd;
}

CPoint CChartToolBtn::ReplaceSize(CPoint ptStart, BOOL bShow)
{
	if(bShow) 
	{
		m_ptStart	= ptStart;
		CalcRect(ptStart, m_strTitle, m_rcRect);
		MoveWindow(&m_rcRect, TRUE);
	}
	else
	{
		MoveWindow(CRect(0,0,0,0));		// 버튼을 숨김
	}

	m_ptEnd.x = m_rcRect.left;
	m_ptEnd.y = m_rcRect.bottom;

	return m_ptEnd;
}


void CChartToolBtn::DeleteBtn()
{
	if(m_hWnd && IsWindow(m_hWnd))
		DestroyWindow();
}

int CChartToolBtn::GetHeight()
{
	return m_nBtnHeigth;
}

void CChartToolBtn::CalcRect(CPoint ptStart, CString strTitle, CRect &rect)
{
	CSize size;
	int nLen;

	if(m_bFixedSz)
	{
		rect.left = ptStart.x;
		rect.top = ptStart.y;
		rect.right = rect.left + m_FixedSz.cx;
		rect.bottom = rect.top + m_FixedSz.cy;

		m_nBtnWidth = rect.right - rect.left;
		m_nBtnHeigth = rect.bottom - rect.top;

		return;
	}

	nLen = strTitle.GetLength();
	size.cx = nLen * (m_nFWidth);
	size.cy = m_nFHeight;

	rect.left = ptStart.x;
	rect.top = ptStart.y;
	rect.right = ptStart.x + size.cx + BT_XGAP;
	rect.bottom = ptStart.y + size.cy + BT_YGAP;

	m_nBtnWidth = rect.right - rect.left;
	m_nBtnHeigth = rect.bottom - rect.top;
}

void CChartToolBtn::DrawBMP(CDC *pdc)
{
	CRect rectCl, rectBm;
	CDC	dcMem;

	GetClientRect(&rectCl);
	if(rectCl.right < m_BmpSz.cx)
	{
		rectBm.left = 0; 
		rectBm.right = rectCl.right;
	}
	else
	{
		rectBm.left = (rectCl.right - m_BmpSz.cx) / 2;
		rectBm.right = rectBm.left + m_BmpSz.cx;
	}

	if(rectCl.bottom < m_BmpSz.cy)
	{
		rectBm.top = 0;
		rectBm.bottom = rectCl.bottom;
	}
	else
	{
		if(m_strTitle == "")
		{
			rectBm.top = (rectCl.bottom - m_BmpSz.cy) / 2;
			rectBm.bottom = rectBm.top + m_BmpSz.cy;
		}
		else
		{
			rectBm.top = 2;
			rectBm.bottom = 2 + m_BmpSz.cy;
		}
	}

	if(!dcMem.CreateCompatibleDC(pdc))
		return;

	CBitmap *pOldBitmap = dcMem.SelectObject(m_pBmp);
	pdc->StretchBlt(rectBm.left, rectBm.top, rectBm.Width(), rectBm.Height(), &dcMem,
		0,0, m_BmpSz.cx, m_BmpSz.cy, SRCCOPY);
	dcMem.SelectObject(pOldBitmap);
}

void CChartToolBtn::DrawGradation(CDC *pdc) 
{
	//	CPen	pen, *pOldPen;
	//	CRect	rect;
	//	COLORREF ref = RGB(235,235,235);
	//	BYTE	bRed	= GetRValue(ref);
	//	BYTE	bGreen	= GetGValue(ref);
	//	BYTE	bBlue	= GetBValue(ref);
	//	
	//	GetClientRect(&rect);
	//	int nCnt = rect.bottom / 2+1;
	//
	//	for(int i = 0; i < nCnt; i++)
	//	{
	//		bRed	-= BT_GRADIATE_GAP;
	//		bBlue	-= BT_GRADIATE_GAP;
	//		bGreen	-= BT_GRADIATE_GAP;
	//		bBlue	= (bBlue>=255) ? 255:bBlue;
	//		bBlue	= (bBlue<0) ? 0:bBlue;
	//		bRed	= (bBlue>=255) ? 255:bRed;
	//		bRed	= (bBlue<0) ? 0:bRed;
	//		bGreen	= (bGreen>=255) ? 255:bGreen;
	//		bGreen	= (bGreen<0) ? 0:bGreen;
	//
	//		pen.CreatePen(PS_SOLID, 2, RGB(bRed, bGreen, bBlue));
	//		pOldPen = pdc->SelectObject(&pen);
	//
	//		if(m_nBtDown != BT_DEF_DOWN){
	//			pdc->MoveTo(rect.left, rect.top+2*i);
	//			pdc->LineTo(rect.right, rect.top+2*i);
	//		}
	//		else {
	//			pdc->MoveTo(rect.left, rect.bottom-2*i);
	//			pdc->LineTo(rect.right, rect.bottom-2*i);
	//		}
	//		pdc->SelectObject(pOldPen);
	//		pen.DeleteObject();
	//	}
}

void CChartToolBtn::DrawBackBmp(CDC *pdc)
{
	CRect rect;
	GetClientRect(&rect);

	CDC	dcMem;
	if(!dcMem.CreateCompatibleDC(pdc))
		return;

	CBitmap *pOldBitmap;
		
	if(m_nBtDown == BT_DEF_DOWN) // Button-down image
	{
		pOldBitmap = dcMem.SelectObject(m_pDownBmp);
		pdc->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
	}
	else if(m_nBtDown == BT_DEF_UP) // Button-up(normal) image
	{
		if(m_bToggle && m_nCurrToggle == DOWN_TOGGLE)
			pOldBitmap = dcMem.SelectObject(m_pDownBmp);
		else if(m_bToggle && m_nCurrToggle == UP_TOGGLE)
			pOldBitmap = dcMem.SelectObject(m_pNormalBmp);
		else
			pOldBitmap = dcMem.SelectObject(m_pUpBmp);

		pdc->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
	}
	else
	{
		if(m_bToggle && m_nCurrToggle == DOWN_TOGGLE)
			pOldBitmap = dcMem.SelectObject(m_pDownBmp);
		else
			pOldBitmap = dcMem.SelectObject(m_pNormalBmp);

		pdc->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
	}
	dcMem.SelectObject(pOldBitmap);	
}

void CChartToolBtn::DrawBackImageList(CDC *pdc)
{
	CRect rect;
	GetClientRect(&rect);

//	CDC	dcMem;
//	if(!dcMem.CreateCompatibleDC(pdc))
//		return;

//	CBitmap *pOldBitmap;
		pdc->SetBkMode(TRANSPARENT);						// 텍스트의 바탕을 투명하게

		if(m_nBtDown == BT_DEF_DOWN) // Button-down image
		{
			//pOldBitmap = dcMem.SelectObject(m_pDownBmp);
			//pdc->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
			m_pImageList->Draw(pdc,m_nDown, CPoint(0,0), ILD_TRANSPARENT);
		}
		else if(m_nBtDown == BT_DEF_UP) // Button-up(normal) image
		{
			if(m_bToggle && m_nCurrToggle == DOWN_TOGGLE)
				m_pImageList->Draw(pdc,m_nDown, CPoint(0,0), ILD_TRANSPARENT);
				//pOldBitmap = dcMem.SelectObject(m_pDownBmp);
			else if(m_bToggle && m_nCurrToggle == UP_TOGGLE)
				m_pImageList->Draw(pdc,m_nNormal, CPoint(0,0), ILD_TRANSPARENT);
				//pOldBitmap = dcMem.SelectObject(m_pNormalBmp);
			else
				m_pImageList->Draw(pdc,m_nUp, CPoint(0,0), ILD_TRANSPARENT);
				//pOldBitmap = dcMem.SelectObject(m_pUpBmp);

			//pdc->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
		}
		else
		{
			if(m_bToggle && m_nCurrToggle == DOWN_TOGGLE)
				m_pImageList->Draw(pdc,m_nDown, CPoint(0,0), ILD_TRANSPARENT);
				//pOldBitmap = dcMem.SelectObject(m_pDownBmp);
			else
				m_pImageList->Draw(pdc,m_nNormal, CPoint(0,0), ILD_TRANSPARENT);
				//pOldBitmap = dcMem.SelectObject(m_pNormalBmp);

			//pdc->BitBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0, SRCCOPY);
		}
		//dcMem.SelectObject(pOldBitmap);	
}

void CChartToolBtn::CalcText(CPoint *pt, char *cTitle)
{
	int nMaxLen = 0, nCLen = 0, nHi = 0, nLen = 0;
	nLen = strlen(cTitle);
	for(int i = 0; i < nLen; i++)
	{
		nCLen++;
		if(*(cTitle+i) == '\n')
		{
			nCLen = 0;
			nHi++;
		}
		if(nCLen > nMaxLen)
			nMaxLen = nCLen;
	}
	pt->x = nMaxLen;
	pt->y = nHi+1;
}

void CChartToolBtn::DrawRound(CDC *pdc)
{
	CPen pen, *pOldPen;
	CRect rect;
	GetClientRect(&rect);
	CFont *pOldFont = (CFont *)pdc->SelectObject(m_pFont);

	if(m_pUpBmp == NULL || m_bMenuBtn)	// Bitmap Button 이 아니면 Round 를 그린다.
	{
		if(m_nBtDown == BT_DEF_DOWN)			/* Button DOWN */
		{
			pen.CreatePen(PS_SOLID, 2, CLR_FONT_DEFAULT);
			pOldPen = pdc->SelectObject(&pen);
			pdc->MoveTo(rect.right, rect.top);
			pdc->LineTo(rect.left, rect.top);
			pdc->LineTo(rect.left, rect.bottom);
			pdc->SelectObject(pOldPen);
			pen.DeleteObject();

			pen.CreatePen(PS_SOLID, 2, RGB(200,200,200));
			pOldPen = pdc->SelectObject(&pen);
			pdc->MoveTo(rect.right, rect.top);
			pdc->LineTo(rect.right, rect.bottom);
			pdc->LineTo(rect.left, rect.bottom);
			pdc->SelectObject(pOldPen);
			pen.DeleteObject();
		//	rect.left += BT_XGAP /2; //rect.top += BT_YGAP /2;
		//	rect.right -= BT_XGAP /2; //rect.bottom -= BT_YGAP /2;
			rect.left += 1; rect.top += 1; rect.right += 1; rect.bottom += 1;
		}
		else if(m_nBtDown == BT_DEF_UP||((m_nBtDown == BT_DEF_NORMAL)&&!m_bMenuBtn))/* Button MOUSE MOVE (UP) */
		{
			pen.CreatePen(PS_SOLID, 2, CLR_FONT_DEFAULT);
			pOldPen = pdc->SelectObject(&pen);
			pdc->MoveTo(rect.right, rect.top);
			pdc->LineTo(rect.right, rect.bottom);
			pdc->LineTo(rect.left, rect.bottom);
			pdc->SelectObject(pOldPen);
			pen.DeleteObject();

			pen.CreatePen(PS_SOLID, 2, RGB(255,255,255));
			pOldPen = pdc->SelectObject(&pen);
			pdc->MoveTo(rect.right, rect.top);
			pdc->LineTo(rect.left, rect.top);
			pdc->LineTo(rect.left, rect.bottom);
			pdc->SelectObject(pOldPen);
			pen.DeleteObject();
		//	rect.left += BT_XGAP /2; //rect.top += BT_YGAP /2;
		//	rect.right -= BT_XGAP /2; //rect.bottom -= BT_YGAP /2;
		}
		else if(m_nBtDown == BT_DEF_NORMAL)
		{
		//	rect.left += BT_XGAP /2; //rect.top += BT_YGAP /2;
		//	rect.right -= BT_XGAP /2; //rect.bottom -= BT_YGAP /2;
		}
	}

	pdc->SetBkMode(TRANSPARENT);
	if(m_bMenuBtn)
	{
		char cMenu[5] = "";
		cMenu[0] = 0x1F;
		pdc->SetTextColor(RGB(0, 0, 0));
		int nY = rect.Height() / 2 - 8;
		pdc->TextOut(1,nY,cMenu, 1);
//		pdc->DrawText(strBuf, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else if(m_strTitle != "")
	{
		CRect bkRect = rect + CPoint(1, 0);
		if(m_pBmp == NULL)
		{
			if(!m_bFixedSz)
			{
			//	pdc->SetTextColor(RGB(255,255,255));
			//	pdc->DrawText(m_strTitle, &bkRect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				pdc->SetTextColor(RGB(0, 0, 0));
				pdc->DrawText(m_strTitle, &rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			}
			else
			{
				CPoint cp;
				CalcText(&cp, (LPSTR)(LPCTSTR)m_strTitle);
				rect.top += (rect.Height() - (m_nFHeight * cp.y)) / 2;
				bkRect = rect + CPoint(1, 0);
			//	rect.left = (rect.Width() - ((m_nFWidth-2) * cp.x)) / 2;

				// 음영 TEXT
			//	pdc->SetTextColor(RGB(255,255,255));
			//	pdc->DrawText(m_strTitle, &bkRect, DT_CENTER|DT_VCENTER);
				// TEXT
				if(m_nBtDown == BT_DEF_DOWN)
					pdc->SetTextColor(RGB(0,0,160));
				else
					pdc->SetTextColor(RGB(255, 255, 255));
				pdc->DrawText(m_strTitle, &rect, DT_CENTER|DT_VCENTER);
			}
		}
		else
		{
			rect.DeflateRect(0, 2);	bkRect.DeflateRect(0, 2);
			pdc->SetTextColor(RGB(255,255,255));
			pdc->DrawText(m_strTitle, &bkRect, DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
			pdc->SetTextColor(RGB(0, 0, 0));
			pdc->DrawText(m_strTitle, &rect, DT_CENTER|DT_BOTTOM|DT_SINGLELINE);
		}
		pdc->SelectObject(pOldFont);
	}
}


void CChartToolBtn::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if(m_bMenuBtn)
	{
		CRect rect;
		GetClientRect(&rect);
		CBrush brush;
		brush.CreateSolidBrush(RGB(217,217,217));
		dc.FillRect(&rect, &brush);
		brush.DeleteObject();
	}
	else
	{
		if(m_pBmp == NULL)
		{
			if(m_pImageList != NULL)
				DrawBackImageList((CDC *)&dc);
			else if(m_pUpBmp != NULL)
				DrawBackBmp((CDC *)&dc);
			else
				DrawGradation((CDC *)&dc);
		}
		else
		{
			if(m_pImageList != NULL)
				DrawBackImageList((CDC *)&dc);
			else if(m_pUpBmp != NULL)
				DrawBackBmp((CDC *)&dc);
			else
				DrawGradation((CDC *)&dc);

			if(!m_bMenuBtn)
				DrawBMP((CDC *)&dc);
		}
	}

//	DrawRound((CDC *)&dc);
	// Do not call CWnd::OnPaint() for painting messages
}

void CChartToolBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	SetCapture();
	m_nBtDown = BT_DEF_DOWN;
	m_bDowned = TRUE;
	m_nBeState = m_nBtDown;
	InvalidateRect(NULL, TRUE);

	// 부모가 Edit & Label에 설명을 뿌려줌
	if(m_nCurrToggle == UP_TOGGLE)
		GetParent()->SendMessage(UDM_CHARTBUTTON_CLICK, 2, GetDlgCtrlID());
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CChartToolBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	GetWindowRect(&m_rect);
	
	CWnd *wndCapture = GetCapture();
	if(wndCapture == this)	
		ReleaseCapture();

	//CWnd* pMainWnd = AfxGetMainWnd();
	//if(pMainWnd==NULL)
	//	return;

	if(PtInRect(&m_rect, point))
	{
		// message 발생
		if(m_bDowned)
		{
			if(m_bToggle)
			{
				if(m_nCurrToggle == UP_TOGGLE)
				{
					m_nCurrToggle = DOWN_TOGGLE;
					m_nMessage = 1;
					// * 자신이 눌린 상태가 되었음을 부모에게 알린다. 
					// * 부모(CChartToolBar)는 이를 기억하고 있다가 다른 버튼이 눌렸을 때
					// * 이 버튼의 눌린 상태를 해제시킨다. 
					GetParent()->SendMessage(UDM_CHARTBUTTON_CLICK, 1, (LPARAM)this);
				}
				else
				{
					m_nCurrToggle = UP_TOGGLE;
					m_nMessage = 0;
					GetParent()->SendMessage(UDM_CHARTBUTTON_CLICK, 3, (LPARAM)this);
				}
				InvalidateRect(NULL, FALSE);
			}
			else 
			{
				m_nMessage = 1;
				GetParent()->SendMessage(UDM_CHARTBUTTON_CLICK, 3, (LPARAM)this);
			}
			
			// 2002. 8. 30 Hedges
			//CWnd* pMainWnd = AfxGetMainWnd();
			//if(pMainWnd->GetSafeHwnd() && ::IsWindow(pMainWnd->GetSafeHwnd()))
			//{
				NMHDR nmhdr; ::memset(&nmhdr, 0x00, sizeof(NMHDR));
				nmhdr.hwndFrom = m_hWnd;
				nmhdr.idFrom = ADD_CHART_TOOL;
				nmhdr.code = m_nMessage;
				CWnd* pParent = GetParent();
				pParent->SendMessage(RMS_ANALYSIS_FUNC, (WPARAM)m_ToolOption, (LPARAM)&nmhdr);

			//}
		}
	}

	m_bDowned = FALSE;
	m_nBtDown = BT_DEF_NORMAL;
	m_nBeState = m_nBtDown;
	InvalidateRect(NULL, TRUE);
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CChartToolBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	return;
	ClientToScreen(&point);
	CRect rect;
	GetWindowRect(&rect);

	if(PtInRect(&rect, point))
	{
		if(GetCapture() != this)
			SetCapture();
		if(m_bDowned)
			m_nBtDown = BT_DEF_DOWN;
		else
		{
			m_nBtDown = BT_DEF_UP;
		}
	}
	else //if(GetCapture() == this)
	{
		if(GetCapture() == this && !m_bDowned)
		{
			m_nBtDown = BT_DEF_NORMAL;
			ReleaseCapture();
		}
		else if(m_bDowned)
			m_nBtDown = BT_DEF_UP;
		else
		{
			m_nBtDown = BT_DEF_NORMAL;
			ReleaseCapture();
		}
	}
	if(m_nBeState != m_nBtDown)
		Invalidate(TRUE);
	m_nBeState = m_nBtDown;

	CWnd::OnMouseMove(nFlags, point);
}

void CChartToolBtn::SetToggle(BOOL bToggle, int nCurToggle)
{
	m_bToggle = bToggle;
	m_nCurrToggle = nCurToggle;
}

int CChartToolBtn::GetToggle()
{
	if(m_bToggle)
		return m_nCurrToggle;

	return UP_TOGGLE;
}

// * Down상태에 있는 버튼을 해제한다. 
void CChartToolBtn::ReleaseBtn()
{
	m_nCurrToggle = UP_TOGGLE;
	m_nBtDown = BT_DEF_NORMAL;
	m_nBeState = BT_DEF_NORMAL;
	m_bDowned = FALSE;

	InvalidateRect(NULL, TRUE);
}

void CChartToolBtn::SetToggleBtnFromOutside()
{
	m_nBtDown = BT_DEF_DOWN;
	m_bDowned = TRUE;
	m_nBeState = m_nBtDown;

	// 부모가 Edit & Label에 설명을 뿌려줌
	if(m_nCurrToggle == UP_TOGGLE)
		GetParent()->SendMessage(UDM_CHARTBUTTON_CLICK, 2, GetDlgCtrlID());

	CWnd *wndCapture = GetCapture();
	if(wndCapture == this)	
		ReleaseCapture();

	// message 발생
	if(m_bDowned)
	{
		if(m_bToggle)
		{
			if(m_nCurrToggle == UP_TOGGLE)
			{
				m_nCurrToggle = DOWN_TOGGLE;
				m_nMessage = 1;
				// * 자신이 눌린 상태가 되었음을 부모에게 알린다. 
				// * 부모(CChartToolBar)는 이를 기억하고 있다가 다른 버튼이 눌렸을 때
				// * 이 버튼의 눌린 상태를 해제시킨다. 
				GetParent()->SendMessage(UDM_CHARTBUTTON_CLICK, 1, (LPARAM)this);
			}
			else
			{
				m_nCurrToggle = UP_TOGGLE;
				m_nMessage = 0;
				GetParent()->SendMessage(UDM_CHARTBUTTON_CLICK, 3, (LPARAM)this);
			}
			InvalidateRect(NULL, FALSE);
		}
		else 
		{
			m_nMessage = 1;
			GetParent()->SendMessage(UDM_CHARTBUTTON_CLICK, 3, (LPARAM)this);
		}
		
		NMHDR nmhdr; ::memset(&nmhdr, 0x00, sizeof(NMHDR));
		nmhdr.hwndFrom = m_hWnd;
		nmhdr.idFrom = ADD_CHART_TOOL;
		nmhdr.code = m_nMessage;
		CWnd* pParent = GetParent();
		pParent->SendMessage(RMS_ANALYSIS_FUNC, (WPARAM)m_ToolOption, (LPARAM)&nmhdr);
	}

	m_bDowned = FALSE;
	m_nBtDown = BT_DEF_NORMAL;
	m_nBeState = m_nBtDown;
	InvalidateRect(NULL, TRUE);
}