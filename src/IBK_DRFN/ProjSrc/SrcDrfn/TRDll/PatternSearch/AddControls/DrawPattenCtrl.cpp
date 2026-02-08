// DrawPattenCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "DrawPattenCtrl.h"

#include "math.h"
#define _WIN32_WINNT 0x0400

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/************************************************************************/
/*                                                                      */
/*                          Patten Draw Control                         */
/*                                                                      */
/*                                                                      */
/*                            작성일 : 2007. 12~                        */
/*                       작성자 : 우동우 (DRfn co.)                     */
/*                                                                      */
/************************************************************************/

//#define FILE_VERSION_DRAWDATA		2
//# 2008.02.27

#define COLOR_CANVAS				    RGB(255, 255, 255)
#define COLOR_LINE_GRID					RGB(192, 192, 192)
#define COLOR_LINE							RGB(255, 0, 0)
#define COLOR_LINE_MOVE					RGB(0, 0, 255)
#define COLOR_LINE_GUIDE				RGB(0, 0, 125)

#define COLOR_LINE_TEMP				RGB(0, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// CDrawPattenCtrl

CDrawPattenCtrl::CDrawPattenCtrl()
{
	m_pwndMsgTarget = NULL;

	m_nOffset_CanvasGap = 4;
	m_bEnable = TRUE;

	m_brushCanvas.CreateSolidBrush(COLOR_CANVAS);
	m_hPen_Draw = CreatePen(PS_SOLID, 2, COLOR_LINE);
	m_hPen_Move= CreatePen(PS_SOLID, 2, COLOR_LINE_MOVE);
	m_hPen_Guide = CreatePen(PS_SOLID, 1, COLOR_LINE_GUIDE);
	m_hPen_Grid = CreatePen(PS_SOLID, 1, COLOR_LINE_GRID);
	m_hPen_Canvas = CreatePen(PS_SOLID, 1, COLOR_CANVAS);


	m_nXGridCnt = (int)MAXCNT_XPOSITION;
	m_nState_LMouse = 0;
	
	m_nInterval_X = 0;
	m_nInterval_Y = 0;

	m_nDrawXPos_Min = 0;
	m_nDrawXPos_Max = 0;
	m_nDrawYPos_Min = 0;
	m_nDrawYPos_Max = 0;
	
	m_nPosX_Start = 0;

	m_paDrawData = NULL;
	m_paDrawData_ReDraw = NULL;

	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	GETFONT(logFont, 9, 200, m_fontSmall);
}

CDrawPattenCtrl::~CDrawPattenCtrl()
{
	m_brushCanvas.DeleteObject();
	m_fontDefault.DeleteObject();
	m_fontSmall.DeleteObject();

	::DeleteObject(m_hPen_Draw);
	::DeleteObject(m_hPen_Move);
	::DeleteObject(m_hPen_Guide);
	::DeleteObject(m_hPen_Grid);
	::DeleteObject(m_hPen_Canvas);
}

BEGIN_MESSAGE_MAP(CDrawPattenCtrl, CWnd)
	//{{AFX_MSG_MAP(CDrawPattenCtrl)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDrawPattenCtrl message handlers

BOOL CDrawPattenCtrl::Create(CWnd* pParentWnd, UINT nID)
{
	BOOL bResult = FALSE;

	bResult = CWnd::Create(_T("STATIC"), _T("Draw Patten Control"), 
		WS_CHILD | WS_VISIBLE /*| WS_BORDER*/ | WS_CLIPSIBLINGS | SS_NOTIFY, 
		CRect(0, 0, 100, 100), pParentWnd, nID);
  
	m_pwndMsgTarget = pParentWnd;
	
	if (m_paDrawData == NULL)
		m_paDrawData = new CPtrArray;

	if (m_paDrawData_ReDraw == NULL)
		m_paDrawData_ReDraw = new CPtrArray;

	return bResult;
}

void CDrawPattenCtrl::OnDestroy() 
{
	SetDelData(m_paDrawData);
	SetDelData(m_paDrawData_ReDraw);
	
	if (m_paDrawData != NULL)
		delete m_paDrawData;

	if (m_paDrawData_ReDraw != NULL)
		delete m_paDrawData_ReDraw;
	
	CWnd::OnDestroy();
}

void CDrawPattenCtrl::SetData_Clear()
{
	SetDelData(m_paDrawData);
//	SetDelData(m_paDrawData_ReDraw);

	for (int nIndex = 0; nIndex < (int)MAXCNT_XPOSITION; nIndex++)
		m_nDrawPosition[nIndex] = -1;
}

void CDrawPattenCtrl::SetData_XGridCnt(int nData)
{
	m_nXGridCnt = nData;

	m_nState_LMouse = 0;
	SetData_ReSize(m_paDrawData);

	AdjustJumpPoint(m_paDrawData);

	Invalidate();
}
	
void CDrawPattenCtrl::SetInit()
{
	m_nState_LMouse = 0;
	SetData_Clear();
	
	Invalidate();
}

void CDrawPattenCtrl::OnPaint() 
{
	CPaintDC dc(this);

	CRect reClient;
	GetClientRect(reClient);
	if (m_nState_LMouse == 0)
	{
		SetDraw_Canvas(dc.m_hDC, reClient, COLOR_CANVAS);
		SetDraw_Grid(dc.m_hDC);
	}
	else
		SetDraw_Canvas(dc.m_hDC, reClient, COLOR_CANVAS);
	
	if (m_nState_LMouse == 1)
	{
		SetDraw_Line(dc.m_hDC, m_paDrawData_ReDraw, 1);
		SetDraw_GuideLine(dc.m_hDC);
	}
	
	SetDraw_Line(dc.m_hDC, m_paDrawData, 0);
}

void CDrawPattenCtrl::SetDraw_All(HDC hDC)
{
	CRect reClient;
	GetClientRect(reClient);
	if (m_nState_LMouse == 0)
	{
		SetDraw_Canvas(hDC, reClient, COLOR_CANVAS);
		SetDraw_Grid(hDC);
	}
	else
		SetDraw_Canvas(hDC, reClient, COLOR_CANVAS);
	
	if (m_nState_LMouse == 1)
	{
		SetDraw_Line(hDC, m_paDrawData_ReDraw, 1);
		SetDraw_GuideLine(hDC);
	}
	
	SetDraw_Line(hDC, m_paDrawData, 0);
}

void CDrawPattenCtrl::SetDraw_Canvas(HDC hDC, CRect reClient, COLORREF crColor)
{
	CRect reBorder(reClient);
		
	reBorder.DeflateRect(1, 1);
	FillSolidRect(hDC, reBorder.left, reBorder.top, 
		reBorder.Width(), reBorder.Height(), crColor);
	
	Draw3dRect(hDC, reBorder.left, reBorder.top, 
		reBorder.Width(), reBorder.Height(), 1, 
		GetSysColor(COLOR_BTNHIGHLIGHT), GetSysColor(COLOR_BTNSHADOW)); 

	reBorder.InflateRect(1, 1);
	Draw3dRect(hDC, reBorder.left, reBorder.top, 
		reBorder.Width(), reBorder.Height(), 1, 
		GetSysColor (COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHIGHLIGHT));
	
	if (m_nState_LMouse == 1)
	{
		HPEN hOldPen;
		hOldPen = (HPEN)SelectObject(hDC, m_hPen_Grid);
			
		MoveToEx(hDC, m_reCanvas.left, m_reCanvas.top, NULL);
		LineTo(hDC, m_reCanvas.right, m_reCanvas.top);

		MoveToEx(hDC, m_reCanvas.right, m_reCanvas.top, NULL);
		LineTo(hDC, m_reCanvas.right, m_reCanvas.bottom);

		MoveToEx(hDC, m_reCanvas.right, m_reCanvas.bottom, NULL);
		LineTo(hDC, m_reCanvas.left, m_reCanvas.bottom);

		MoveToEx(hDC, m_reCanvas.left, m_reCanvas.bottom, NULL);
		LineTo(hDC, m_reCanvas.left, m_reCanvas.top);

		SelectObject (hDC, hOldPen);
	}
}

void CDrawPattenCtrl::FillSolidRect(HDC hDC, int x, int y, int cx, int cy, COLORREF clr)
{
	ASSERT (hDC != NULL);
	SetBkColor(hDC, clr);
	CRect rect(x, y, x + cx, y + cy);
	ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
}

void CDrawPattenCtrl::Draw3dRect(HDC hDC, int x, int y, int cx, int cy, int nLH,
	COLORREF clrTopLeft, COLORREF clrBottomRight)
{
	FillSolidRect(hDC, x, y, cx - nLH, nLH, clrTopLeft);
	FillSolidRect(hDC, x, y, nLH, cy - nLH, clrTopLeft);
	FillSolidRect(hDC, x + cx, y, -nLH, cy, clrBottomRight);
	FillSolidRect(hDC, x, y + cy, cx, -nLH, clrBottomRight);
}

void CDrawPattenCtrl::SetDraw_Grid(HDC hDC)
{
	int nOffsetX, nOffsetY;
	nOffsetX = m_reCanvas.left;
	nOffsetY = m_reCanvas.top;
  
	HPEN hOldPen;
	hOldPen = (HPEN)SelectObject (hDC, m_hPen_Grid);

	int xScaleDispGap = 10;
	if(m_nXGridCnt > 60) xScaleDispGap = 20;

	CString strMark;
	int nIndex, nX, nY, nLenScale = 0;
	HFONT* pOldFont = (HFONT*)SelectObject(hDC, m_fontSmall.m_hObject);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(120,120,120));
	for (nIndex = m_nXGridCnt; nIndex >= 0; nIndex--)
	{
		nX = m_nInterval_X * nIndex + nOffsetX;
		if((m_nXGridCnt - nIndex)%xScaleDispGap == 0) nLenScale = 3;
		else nLenScale = 0;

		MoveToEx(hDC, nX, m_reCanvas.bottom + nLenScale, NULL);
		LineTo (hDC, nX, m_reCanvas.top);

		if((m_nXGridCnt - nIndex)%xScaleDispGap == 0)
		{
			strMark.Format("%d", m_nXGridCnt - nIndex);
			nX = nX - 5*(strMark.GetLength()-1);
			if(nX <= m_reCanvas.left) nX = m_reCanvas.left - 2;
			else if( nX+ 5 > m_reCanvas.right) nX = m_reCanvas.right - 2;
			TextOut(hDC, nX, m_reCanvas.bottom + nLenScale,strMark, strMark.GetLength());
		}
	}
	SelectObject(hDC, pOldFont);

	int nYGridCnt;
	nYGridCnt = m_nXGridCnt * 2 / 3;
	for (nIndex = 0; nIndex <= nYGridCnt; nIndex++)
	{
		nY = m_nInterval_Y * nIndex + nOffsetY;
		MoveToEx(hDC, m_reCanvas.left, nY, NULL);
		LineTo (hDC, m_reCanvas.right, nY);
	}

	CRect rcText = m_reCanvas;
	rcText.top = rcText.bottom - 12;
	HFONT* pOldFontNormal = (HFONT*)::SelectObject(hDC, m_fontDefault.m_hObject);
	SetTextColor(hDC, RGB(120,120,120));
	DrawText(hDC, "마우스로 끌어서 추세를 그리세요", 32, rcText, DT_SINGLELINE | DT_VCENTER |DT_RIGHT);
	::SelectObject(hDC, pOldFontNormal);
	SelectObject (hDC, hOldPen);
}

void CDrawPattenCtrl::SetDraw_GuideLine(HDC hDC)
{
	if ((m_paDrawData_ReDraw == NULL) || (m_paDrawData_ReDraw->GetSize() == 0))
		return;

	int nOffsetX, nOffsetY;
	nOffsetX = m_reCanvas.left;
	nOffsetY = m_reCanvas.top;

	Position *pptData;

	//# Guide Line
	HPEN hOldPen;
	hOldPen = (HPEN)SelectObject (hDC, m_hPen_Guide);

	pptData = (Position*)m_paDrawData_ReDraw->GetAt(0);
	MoveToEx(hDC, (int)pptData->x + nOffsetX, m_reCanvas.top, NULL);
	LineTo(hDC, (int)pptData->x + nOffsetX, m_reCanvas.bottom);
	
	pptData = (Position*)m_paDrawData_ReDraw->GetAt(m_paDrawData_ReDraw->GetSize() - 1);
	MoveToEx(hDC, (int)pptData->x + nOffsetX, m_reCanvas.top, NULL);
	LineTo(hDC, (int)pptData->x + nOffsetX, m_reCanvas.bottom);
	
	SelectObject(hDC, hOldPen);
}

void CDrawPattenCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	SetData_ReSize(m_paDrawData);
	Invalidate();
}

void CDrawPattenCtrl::SetData_ReSize(CPtrArray *paData)
{
	CRect reClient, reCanvas;
	GetClientRect(reClient);
	reClient.bottom -= 8; // by jeoyoho

	reCanvas = GetSize_Canvas(reClient);
	int nOffsetX, nOffsetY;
	nOffsetX = (reClient.Width() - reCanvas.Width()) / 2;
	nOffsetY = (reClient.Height() - reCanvas.Height()) / 2;
	
	reCanvas.left += nOffsetX;
	reCanvas.right += nOffsetX;
	reCanvas.top += nOffsetY;
	reCanvas.bottom += nOffsetY;

	if ((paData == NULL) || (paData->GetSize() == 0))
	{
		m_reCanvas = reCanvas;
		return;
	}
	
	if(reCanvas.Height() < 20 || reCanvas.Width() < 60) return;

	double dScaleX, dScaleY;
	dScaleX = (reCanvas.Width() - m_reCanvas.Width()) / (double)m_reCanvas.Width();
	dScaleY = (reCanvas.Height() - m_reCanvas.Height()) / (double)m_reCanvas.Height();

	Position *pstPosition;
	for (int nIndex = 0; nIndex < paData->GetSize(); nIndex++)
	{
		pstPosition = (Position*)paData->GetAt(nIndex);

		pstPosition->x += (float)(pstPosition->x * dScaleX);
		if (pstPosition->x < 0)
			pstPosition->x = 0;

		if ((float)reCanvas.Width() < pstPosition->x)
			pstPosition->x = (float)reCanvas.Width();

		pstPosition->y += (float)(pstPosition->y * dScaleY);
		if (pstPosition->y < 0)
			pstPosition->y = 0;

		if ((float)reCanvas.Height() < pstPosition->y)
			pstPosition->y = (float)reCanvas.Height();

		paData->SetAt(nIndex, pstPosition);
	}

	m_reCanvas = reCanvas;
}

CRect CDrawPattenCtrl::GetSize_Canvas(CRect reClient)
{
	int nWidth, nHeight, nXGridCnt, nYGridCnt;
	nWidth = reClient.right - reClient.left - m_nOffset_CanvasGap * 2;
	nHeight = reClient.bottom - reClient.top - m_nOffset_CanvasGap * 2;

	nXGridCnt = m_nXGridCnt;

	div_t div_result;
	div_result = div(nWidth, nXGridCnt);

	reClient.right = nWidth - div_result.rem;
	m_nInterval_X = div_result.quot;

	nYGridCnt = nXGridCnt * 2 / 3;
	div_result = div(nHeight, nYGridCnt);

	reClient.bottom = nHeight - div_result.rem;
	m_nInterval_Y = div_result.quot;

	return reClient;
}

BOOL CDrawPattenCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if (m_bEnable == FALSE)
		return FALSE;

	int nOffsetX, nOffsetY;
	nOffsetX = m_reCanvas.left;
	nOffsetY = m_reCanvas.top;

	CString strMsg;
	CPoint ptData;
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		ptData = (CPoint)pMsg->lParam;

		m_nPosX_Start = ptData.x - nOffsetX;
		// 기존 리스트 삭제
		if(m_nState_LMouse==0)
		{
			SetDelData(m_paDrawData);
			SetDelData(m_paDrawData_ReDraw);
		}

		m_nState_LMouse = 1;
		Invalidate();
		m_pwndMsgTarget->PostMessage(WM_NOTIFY_DRAWPATTEN, (WPARAM)NM_LBUTTONDOWN, (LPARAM)NULL);
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		ptData = (CPoint)pMsg->lParam;
		if ((pMsg->wParam == MK_LBUTTON) && (m_nState_LMouse == 1))
		{
			Position stPos;
			stPos.x = (float)ptData.x - nOffsetX;
			stPos.y = (float)ptData.y - nOffsetY;
			if (GetCheck_CanvasArea(stPos) == FALSE)
			{
				SetDraw_Data(TRUE);			
				return FALSE;
			}
			
			SetAddData(m_paDrawData_ReDraw, stPos);
			Invalidate();
		}
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		ptData = (CPoint)pMsg->lParam;

		if (m_nState_LMouse == 1)
			SetDraw_Data(TRUE);
	}
	
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CDrawPattenCtrl::GetCheck_CanvasArea(Position stPosition)
{
	BOOL bResult = TRUE;

	int nOffsetX, nOffsetY;
	nOffsetX = m_reCanvas.left;
	nOffsetY = m_reCanvas.top;
	
	if (((int)stPosition.x <= m_reCanvas.left - nOffsetX) || ((int)stPosition.x >= m_reCanvas.right - nOffsetX) ||
		((int)stPosition.y <= m_reCanvas.top - nOffsetY) || ((int)stPosition.y >= m_reCanvas.bottom - nOffsetY))
		bResult = FALSE;

	return bResult;
}

void CDrawPattenCtrl::SetDraw_Data(BOOL bDraw)
{
	if (m_paDrawData_ReDraw->GetSize() <= 2 )
	{
//		SetDelData(m_paDrawData);
//		SetDelData(m_paDrawData_ReDraw);
		m_nState_LMouse = 0;
		Invalidate();
		return;
	}
	
	int nOffsetX, nOffsetY;
	nOffsetX = m_reCanvas.left;
	nOffsetY = m_reCanvas.top;
	
	BOOL bReverse_XData = FALSE;
	Position *pptData, *pptData_Copy;
	int nIndex, nMaxX, nMinX;
	pptData = (Position*)m_paDrawData_ReDraw->GetAt(m_paDrawData_ReDraw->GetSize() - 1);
	if (m_nPosX_Start > pptData->x)
		bReverse_XData = TRUE;
	
	if (m_paDrawData->GetSize() == 0)
	{
		for (nIndex = 0; nIndex < m_paDrawData_ReDraw->GetSize(); nIndex++)
		{
			if (bReverse_XData == TRUE)
				pptData = (Position*)m_paDrawData_ReDraw->GetAt(m_paDrawData_ReDraw->GetSize() - 1 - nIndex);
			else
				pptData = (Position*)m_paDrawData_ReDraw->GetAt(nIndex);
			
			pptData_Copy = new Position;
			pptData_Copy->x = pptData->x;
			pptData_Copy->y = pptData->y;
			m_paDrawData->Add(pptData_Copy);
		}
	}
	else 
	{
		nMaxX = m_reCanvas.left - nOffsetX;
		nMinX = m_reCanvas.right - nOffsetX;
		for (nIndex = 0; nIndex < m_paDrawData_ReDraw->GetSize(); nIndex++)
		{
			pptData = (Position*)m_paDrawData_ReDraw->GetAt(nIndex);
			if (pptData->x > nMaxX)
				nMaxX = (int)pptData->x;
			
			if (pptData->x < nMinX)
				nMinX = (int)pptData->x;
		}
		
		CPtrArray paBefor, paAfter;
		for (nIndex = 0; nIndex < m_paDrawData->GetSize(); nIndex++)
		{
			pptData = (Position*)m_paDrawData->GetAt(nIndex);
			
			pptData_Copy = new Position;
			pptData_Copy->x = pptData->x;
			pptData_Copy->y = pptData->y;
			if (nMinX >= (int)pptData->x)
				paBefor.Add(pptData_Copy);
			else if ((int)pptData->x >= nMaxX)
				paAfter.Add(pptData_Copy);
		}
		SetDelData(m_paDrawData);
		
		for (nIndex = 0; nIndex < paBefor.GetSize(); nIndex++)
		{
			pptData = (Position*)paBefor.GetAt(nIndex);
			
			pptData_Copy = new Position;
			pptData_Copy->x = pptData->x;
			pptData_Copy->y = pptData->y;
			m_paDrawData->Add(pptData_Copy);
		}

		for (nIndex = 0; nIndex < m_paDrawData_ReDraw->GetSize(); nIndex++)
		{
			if (bReverse_XData == TRUE)
				pptData = (Position*)m_paDrawData_ReDraw->GetAt(m_paDrawData_ReDraw->GetSize() - 1 - nIndex);
			else
				pptData = (Position*)m_paDrawData_ReDraw->GetAt(nIndex);
			pptData_Copy = new Position;
			pptData_Copy->x = pptData->x;
			pptData_Copy->y = pptData->y;
			m_paDrawData->Add(pptData_Copy);
		}
		
		for (nIndex = 0; nIndex < paAfter.GetSize(); nIndex++)
		{
			pptData = (Position*)paAfter.GetAt(nIndex);
			pptData_Copy = new Position;
			pptData_Copy->x = pptData->x;
			pptData_Copy->y = pptData->y;
			m_paDrawData->Add(pptData_Copy);
		}
		
		SetDelData(&paBefor);
		SetDelData(&paAfter);
	}
	SetDelData(m_paDrawData_ReDraw);
	
	if (bDraw == TRUE)
	{
		m_nState_LMouse = 0;
		Invalidate();
		
		m_pwndMsgTarget->SendMessage(WM_NOTIFY_DRAWPATTEN, (WPARAM)NM_LBUTTONUP, (LPARAM)NULL);
	}
}

void CDrawPattenCtrl::SetAddData(CPtrArray *paData, Position stPosition, BOOL bCheck /*= TRUE*/)
{
	if (paData == NULL)
		return;

	int nIndex = 0, nOffsetX, nOffsetY;
	nOffsetX = 0;
	nOffsetY = 0;

	Position *pptData;
	if (bCheck == TRUE)
	{
		nOffsetX = m_reCanvas.left;
		nOffsetY = m_reCanvas.top;

		BOOL bFlag_Delete = FALSE;
		if (m_nPosX_Start == stPosition.x)
		{
			SetDelData(paData);
		//	paData->RemoveAll();
		}
		else if (paData->GetSize() > 0)
		{
			for (nIndex = 0; nIndex < paData->GetSize(); nIndex++)
			{
				pptData = (Position*)paData->GetAt(nIndex);
				if ((m_nPosX_Start < stPosition.x) && (stPosition.x < pptData->x))
				{
					bFlag_Delete = TRUE;
					break;
				}
				else if ((m_nPosX_Start > stPosition.x) && (stPosition.x > pptData->x))
				{
					bFlag_Delete = TRUE;
					break;
				}
			}
		}
		
		nIndex = paData->GetSize() - nIndex;
		if ((nIndex > 0) && (bFlag_Delete == TRUE))
		{
			while (nIndex > 0)
			{
				pptData = (Position*)paData->GetAt(paData->GetSize() - 1);
				delete pptData;
				pptData = NULL;
				
			
				paData->RemoveAt(paData->GetSize() - 1);
				paData->FreeExtra();
				nIndex--;
			}
		}
	}

	pptData = new Position;
	pptData->x = stPosition.x;
	pptData->y = stPosition.y;

	paData->Add(pptData);

	AdjustJumpPoint(paData);
}

void CDrawPattenCtrl::AdjustJumpPoint(CPtrArray *paData)
{
	if ((paData == NULL) || (paData->GetSize() < 2))
		return;

	int nUnitGap, nInterval_X ;
	nUnitGap = m_nXGridCnt / 20;

	nInterval_X = m_nInterval_X;

	int nIndex, nOffsetX, nOffsetY;
	nOffsetX = m_reCanvas.left;
	nOffsetY = m_reCanvas.top;


	Position *pstPosition, *pstPosition_Befor;
	BOOL bStart = TRUE, bFind = TRUE;
	while(bFind)
	{
		bFind = FALSE;
		bStart = TRUE;

		for (nIndex = 0; nIndex < paData->GetSize(); nIndex++)
		{
			pstPosition = (Position*)paData->GetAt(nIndex);

			if (pstPosition == NULL)
			{
				continue;
			}

			if (bStart == TRUE)
			{
				pstPosition_Befor = pstPosition;
				bStart = FALSE;
			}
			else
			{
				if (abs((int)(pstPosition_Befor->x - pstPosition->x)) >= nInterval_X * nUnitGap)
				{
					int nXGap = int(pstPosition->x - pstPosition_Befor->x);
					int nYGap = int(pstPosition_Befor->y - pstPosition->y);
					BOOL bHDir = nXGap > 0;
					BOOL bVDir  = nYGap > 0;

					bFind = TRUE;

					if(nInterval_X == 0)
					{
						bFind = FALSE;
						break;
					}

					int nCnt = abs(nXGap)/(nInterval_X * nUnitGap*2/3);
					if(nCnt == 0)
					{
						bFind = FALSE;
						break;
					}

					int x, y;
					float yGap = (float)(nYGap/nCnt);
					yGap = (float)abs(yGap);

					for(int i = 0; i < nCnt; i++)
					{
						if(bHDir)
						{
							x = int(pstPosition_Befor->x + nInterval_X * nUnitGap *i*2/3 + nInterval_X * nUnitGap*2/3);
						}
						else
						{
							x = int(pstPosition_Befor->x - nInterval_X * nUnitGap *i*2/3 - nInterval_X * nUnitGap*2/3);
						}

						if(bVDir)
						{
							y = int(pstPosition_Befor->y - yGap*i - yGap/2);
						}
						else
						{
							y = int(pstPosition_Befor->y + yGap*i +yGap/2);
						}


						Position *pt = new Position;
						pt->x = (float)x;
						pt->y = (float)y;

						paData->InsertAt(nIndex + i, pt);
					}

					break;
				}

				pstPosition_Befor = pstPosition;
			}
		}
	}
}

void CDrawPattenCtrl::SetDelData(CPtrArray *paData)
{
	Position *pptData;
	for (int nIndex = 0; nIndex < paData->GetSize(); nIndex++)
	{
		pptData = (Position*)paData->GetAt(nIndex);
		if (pptData != NULL)
		{
			delete pptData;
			pptData = NULL;
		}
	}

	paData->RemoveAll();
	paData->FreeExtra();
}

void CDrawPattenCtrl::SetDraw_Line(HDC hDC, CPtrArray *paData, int nModeDraw)
{
	if ((paData == NULL) || (paData->GetSize() == 0))
		return;

	int nUnitGap;
	nUnitGap = m_nXGridCnt / 20;

	int nIndex, nOffsetX, nOffsetY;
	nOffsetX = m_reCanvas.left;
	nOffsetY = m_reCanvas.top;

	HPEN hOldPen;
	if (nModeDraw == 1)
		hOldPen = (HPEN)SelectObject (hDC, m_hPen_Move);
	else
		hOldPen = (HPEN)SelectObject (hDC, m_hPen_Draw);
	
	Position *pstPosition, *pstPosition_Befor;
	BOOL bStart = TRUE;
	for (nIndex = 0; nIndex < paData->GetSize(); nIndex++)
	{
		pstPosition = (Position*)paData->GetAt(nIndex);

		if (pstPosition == NULL)
			continue;

		if (bStart == TRUE)
		{
			MoveToEx(hDC, (int)pstPosition->x + nOffsetX, (int)pstPosition->y + nOffsetY, NULL);
			pstPosition_Befor = pstPosition;
			bStart = FALSE;
		}
		else
		{
			if (abs((int)(pstPosition_Befor->x - pstPosition->x)) < m_nInterval_X * nUnitGap)
				LineTo (hDC, (int)pstPosition->x + nOffsetX, (int)pstPosition->y + nOffsetY);
			MoveToEx(hDC, (int)pstPosition->x + nOffsetX, (int)pstPosition->y + nOffsetY, NULL);

			pstPosition_Befor = pstPosition;
		}
	}
	
	SelectObject (hDC, hOldPen);
}

void CDrawPattenCtrl::SetCalCulationData(CPtrArray *paData)
{
	if(m_nInterval_X == 0)
		return;

	int nIndex, nOffsetX, nOffsetY;
	nOffsetX = m_reCanvas.left;
	nOffsetY = m_reCanvas.top;
	
	m_nDrawXPos_Min = m_reCanvas.right;
	m_nDrawXPos_Max = m_reCanvas.left;
	m_nDrawYPos_Min = m_reCanvas.bottom;
	m_nDrawYPos_Max = m_reCanvas.top;

	for (nIndex = 0; nIndex < (int)MAXCNT_XPOSITION; nIndex++)
		m_nDrawPosition[nIndex] = -1;

	div_t div_result;
	Position *pstPosition;
	int nPosX, nPosY;
	for (nIndex = 0; nIndex < paData->GetSize(); nIndex++)
	{
		pstPosition = (Position*)paData->GetAt(nIndex);

		div_result = div((int)(pstPosition->x + nOffsetX), m_nInterval_X);
		nPosX = div_result.quot;
		if ((nPosX == m_nXGridCnt - 1) && (div_result.rem > (int)(m_nInterval_X / 2)))
			nPosX++;
		else if ((nPosX == 0) && (div_result.rem > (int)(m_nInterval_X / 2)))
			nPosX = 1;

		if (nPosX >= (int)MAXCNT_XPOSITION)
			continue;

		if (m_nDrawPosition[nPosX] == -1)
			nPosY = (int)pstPosition->y;
		else
			nPosY = (m_nDrawPosition[nPosX] + (int)pstPosition->y) / 2;
		m_nDrawPosition[nPosX] = nPosY;

		if (m_nDrawXPos_Min > nPosX)
			m_nDrawXPos_Min = nPosX;

		if (m_nDrawXPos_Max < nPosX)
			m_nDrawXPos_Max = nPosX;

		if (m_nDrawYPos_Max < nPosY)
			m_nDrawYPos_Max = nPosY;

		if (m_nDrawYPos_Min > nPosY)
			m_nDrawYPos_Min = nPosY;
	}
}

BOOL CDrawPattenCtrl::SetSave_DrawData(CString strFile)
{
	CString strFullPath;
	if (strFile.IsEmpty() == TRUE)
		::MessageBox(m_hWnd, "저장할 데이터 파일명이 존재하지 않습니다.", PROGRAM_TITLE, MB_OK);
	else
		strFullPath = strFile;

	CFile clsFile;
	if(!clsFile.Open(strFullPath, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite)) 
		return FALSE;
	CArchive ar(&clsFile, CArchive::store);
	
	int nVersion = (int)FILE_VERSION_DRAWDATA;
	ar << nVersion;

	ar << m_paDrawData->GetSize();

	ar << m_reCanvas.top;
	ar << m_reCanvas.bottom;
	ar << m_reCanvas.left;
	ar << m_reCanvas.right;

	Position *pptData;
	for (int nIndex = 0; nIndex < m_paDrawData->GetSize(); nIndex++)
	{
		pptData = (Position*)m_paDrawData->GetAt(nIndex);
		
		ar << pptData->x;
		ar << pptData->y;
	}
	
	ar.Close();
	clsFile.Close();

	return TRUE;
}

BOOL CDrawPattenCtrl::GetLoad_DrawData(CString strFile)
{	
	CString strFullPath;

	if (strFile.IsEmpty() == TRUE)
		::MessageBox(m_hWnd, "입력한 데이터 파일이 존재하지 않습니다.", PROGRAM_TITLE, MB_OK);
	else
		strFullPath = strFile;

	SetData_Clear();

	CFile clsFile;
	if (clsFile.Open(strFullPath, CFile::typeBinary | CFile::modeRead))
	{
		LPSTR lpBuff = NULL;
		DWORD nLen = (DWORD)clsFile.GetLength();
		if(nLen == 0) return FALSE;

		lpBuff = new char[nLen];
		memset(lpBuff, 0, nLen);
		clsFile.Read(lpBuff, nLen);
		clsFile.Close();

		CMemFile memFile;//tour2k = CMemFile(nLen);
		memFile.Attach((unsigned char*)lpBuff, nLen);

		CArchive ar(&memFile, CArchive::load);

		int nVersion;
		ar >> nVersion;
		if (nVersion != (int)FILE_VERSION_DRAWDATA)
		{
			delete[] lpBuff;
			::MessageBox(m_hWnd, "데이터 파일의 버전이 다릅니다.", PROGRAM_TITLE, MB_OK);
			return FALSE;
		}
		
		int nDataCnt;
		ar >> nDataCnt;

		ar >> m_reCanvas.top;
		ar >> m_reCanvas.bottom;
		ar >> m_reCanvas.left;
		ar >> m_reCanvas.right;

		if (nVersion == 3)
		{
			Position stPosition;
			for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
			{
				ar >> stPosition.x;
				ar >> stPosition.y;
			
				SetAddData(m_paDrawData, stPosition, FALSE);
			}
		}
		
		ar.Close();
		memFile.Detach();
		if (lpBuff) 
			delete[] lpBuff;

		SetData_ReSize(m_paDrawData);

		Invalidate();
		return TRUE;
	}

	::MessageBox(m_hWnd, strFile + " 이 존재하지 않습니다.", PROGRAM_TITLE, MB_OK);
	return FALSE;
}

BOOL CDrawPattenCtrl::LoadPatternData4Thumbnail(CString strFile, CRect& rcCanvas, CPtrArray *paDrawData)
{	
	CString strFullPath = strFile;

	CFile clsFile;
	if (clsFile.Open(strFullPath, CFile::typeBinary | CFile::modeRead))
	{
		LPSTR lpBuff = NULL;
		DWORD nLen = (DWORD)clsFile.GetLength();
		if(nLen == 0) return FALSE;

		lpBuff = new char[nLen];
		memset(lpBuff, 0, nLen);
		clsFile.Read(lpBuff, nLen);
		clsFile.Close();

		CMemFile memFile;//tour2k = CMemFile(nLen);
		memFile.Attach((unsigned char*)lpBuff, nLen);

		CArchive ar(&memFile, CArchive::load);

		int nVersion;
		ar >> nVersion;
		if (nVersion != (int)FILE_VERSION_DRAWDATA)
		{
			delete[] lpBuff;
			::MessageBox(m_hWnd, "데이터 파일의 버전이 다릅니다.", PROGRAM_TITLE, MB_OK);
			return FALSE;
		}
		
		int nDataCnt;
		ar >> nDataCnt;

		ar >> rcCanvas.top;
		ar >> rcCanvas.bottom;
		ar >> rcCanvas.left;
		ar >> rcCanvas.right;

		Position stPosition;
		for (int nIndex = 0; nIndex < nDataCnt; nIndex++)
		{
			ar >> stPosition.x;
			ar >> stPosition.y;
		
			if(stPosition.x == 0 && stPosition.y == 0)
				continue;

			Position* pptData = new Position;
			pptData->x = stPosition.x;
			pptData->y = stPosition.y;

			paDrawData->Add(pptData);
		}
		
		ar.Close();
		memFile.Detach();
		if (lpBuff) 
			delete[] lpBuff;

		return TRUE;
	}

	return FALSE;
}


int CDrawPattenCtrl::GetData_DrawPacket(long *pDrawData)
{
	int nResult = 0;

	SetCalCulationData(m_paDrawData);

	int nHeight;
	nHeight = m_reCanvas.bottom - m_reCanvas.top;

	double dData, dData_YMax, dData_YMin;
	dData_YMin = m_nDrawYPos_Min / (double)nHeight * 100.0;
	dData_YMax = m_nDrawYPos_Max / (double)nHeight * 100.0;

	for (int nIndex = 0; nIndex < (int)m_nXGridCnt; nIndex++)
	{
		if (m_nDrawPosition[nIndex] == -1)
			pDrawData[nIndex] = -999;
		else
		{
			dData = m_nDrawPosition[nIndex] / (double)nHeight * 100.0;
			pDrawData[nIndex] = (int)(100 - (dData - dData_YMin) * 100 / (dData_YMax - dData_YMin));
			nResult++;
		}
	}
	
	return nResult;
}

BOOL CDrawPattenCtrl::GetCheck_Sidle()
{
	BOOL bResult = FALSE;
	
	if ((m_nDrawYPos_Max - m_nDrawYPos_Min) < m_nInterval_Y * 2)
		bResult = TRUE;

	return bResult;
}

void CDrawPattenCtrl::SetEnable(BOOL bEnable)
{
	m_bEnable = bEnable;
}

LRESULT CDrawPattenCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_LBUTTONDOWN || message == WM_MOUSEMOVE || message == WM_LBUTTONUP)
	{
		if (m_bEnable == FALSE)
			return FALSE;
		
		int nOffsetX, nOffsetY;
		nOffsetX = m_reCanvas.left;
		nOffsetY = m_reCanvas.top;
		
		CString strMsg;
		CPoint ptData;
		
		switch (message)
		{
		case WM_LBUTTONDOWN:
			ptData = (CPoint)lParam;
			m_nPosX_Start = ptData.x - nOffsetX;
			// 기존 리스트 삭제
			if (m_nState_LMouse==0)
			{
				SetDelData(m_paDrawData);
				SetDelData(m_paDrawData_ReDraw);
			}
			
			m_nState_LMouse = 1;
			Invalidate();
			m_pwndMsgTarget->PostMessage(WM_NOTIFY_DRAWPATTEN, (WPARAM)NM_LBUTTONDOWN, (LPARAM)NULL);
			
			break;
		case WM_MOUSEMOVE:
			ptData = (CPoint)lParam;
			if ((wParam == MK_LBUTTON) && (m_nState_LMouse == 1))
			{
				Position stPos;
				stPos.x = (float)ptData.x - nOffsetX;
				stPos.y = (float)ptData.y - nOffsetY;
				if (GetCheck_CanvasArea(stPos) == FALSE)
				{
					SetDraw_Data(TRUE);			
					return FALSE;
				}
				
				SetAddData(m_paDrawData_ReDraw, stPos);
				Invalidate();
			}
			break;
		case WM_LBUTTONUP:
			ptData = (CPoint)lParam;
			
			if (m_nState_LMouse == 1)
				SetDraw_Data(TRUE);
			
			break;
		}
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}