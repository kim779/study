// CrossLine.cpp: implementation of the CCrossLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axisGMain.h"
#include "CrossLine.h"
#include "GrpGrid.h"
#include "ObjMgr.h"

#include "../../h/axisgenv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrossLine::CCrossLine(CWnd *pView, CObjMgr* pObjMgr, bool bVisible)
{
	m_pApp = (CAxisGMainApp *)AfxGetApp();
	m_pView = pView;
	m_pObjMgr = pObjMgr;

	m_curPt = CPoint(0, 0);
	m_prePt = CPoint(0, 0);

	m_bVisable = bVisible;
}

CCrossLine::~CCrossLine()
{

}

void CCrossLine::ResizeCrossLine(CRect rect)
{
	m_DrawRect = rect;
}

void CCrossLine::DrawCrossLine(CDC* pDC)
{
	if (m_curPt.x == 0)
		return;

	int	style = m_pObjMgr->GetEnvInfo()->display.cline.btLStyle;
	int	width = m_pObjMgr->GetEnvInfo()->display.cline.btLWidth;
	COLORREF	color = m_pObjMgr->GetEnvInfo()->display.cline.rgbLColor;
	color = color^m_pObjMgr->GetEnvInfo()->display.rgbChartBG;
	int	bkMode = pDC->SetBkMode(TRANSPARENT);

	CPen	*cPen = m_pApp->GetPen(m_pView, style, width, color);
	CPen	*sPen = pDC->SelectObject(cPen);
	int	sROP = pDC->SetROP2(R2_XORPEN);

	pDC->MoveTo(m_DrawRect.left, m_curPt.y);
	pDC->LineTo(m_DrawRect.right, m_curPt.y);
	pDC->MoveTo(m_curPt.x, m_DrawRect.top);
	pDC->LineTo(m_curPt.x, m_DrawRect.bottom);

	pDC->SetROP2(sROP);
	pDC->SetBkMode(bkMode);
	pDC->SelectObject(sPen);
}

void CCrossLine::ShowCrossLine(CPoint pt)
{
	if (!m_bVisable)
		return;

	CClientDC ClientDC((CWnd*)m_pObjMgr->m_pwndPnChart);

	if (m_prePt.x > 0)
	{
		m_curPt = m_prePt;
		DrawCrossLine(&ClientDC);
	}

	m_curPt = pt;
	DrawCrossLine(&ClientDC);

	m_prePt = m_curPt = pt;
}

void CCrossLine::HideCrossLine(CPoint pt)
{
	if (!m_bVisable)
		return;

	CClientDC ClientDC((CWnd*)m_pObjMgr->m_pwndPnChart);

	if (m_curPt.x > 0)
		DrawCrossLine(&ClientDC);

	m_prePt = m_curPt = CPoint(0, 0);
}
