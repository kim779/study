// PnBaseWnd.cpp : implementation file
//

#include "stdafx.h"
#include "AxisGCom.h"
#include "PnBaseWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPnBaseWnd

CPnBaseWnd::CPnBaseWnd(CWnd *pwndView, CWnd* pwndGView, CFont* pFont, int ctrlID, char* info)
{
	m_pApp = (CAxisGComApp *)AfxGetApp();
	m_pwndView = pwndView;
	m_pwndGView = pwndGView;
	m_pFont = pFont;
	m_ctrlID = ctrlID;
	m_arComs.RemoveAll();

	m_bLButton = false;
	m_bCapture = false;
	m_iMoveID = 0;
	m_iSelID = 0;

	m_clrBack = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 64);
	m_clrFrame = PALETTERGB(0, 0, 0);
	m_clrInBack = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), 66);

	m_wdLine = 1;
	m_iMoveID = 0;
}

CPnBaseWnd::~CPnBaseWnd()
{
	DeleteComs();
}


BEGIN_MESSAGE_MAP(CPnBaseWnd, CWnd)
	//{{AFX_MSG_MAP(CPnBaseWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPnBaseWnd message handlers

struct _comp* CPnBaseWnd::SearchCom(CPoint pt)
{
	struct _comp* comp;
	for (int ii = 0; ii < m_arComs.GetSize(); ii++)
	{
		comp = m_arComs.GetAt(ii);
		if (comp->cRC.PtInRect(pt) && comp->bshow)
			return comp;
	}
	return NULL;
}

struct _comp* CPnBaseWnd::SearchCom(int ID)
{
	struct _comp* comp;
	for (int ii = 0; ii < m_arComs.GetSize(); ii++)
	{
		comp = m_arComs.GetAt(ii);
		if (comp->cID == ID)
			return comp;
	}
	
	return NULL;
}

CString CPnBaseWnd::GetPosComment(CPoint pt)
{
	ScreenToClient(&pt);
	CString tmpS;
	struct _comp* comp;
	for (int ii = 0; ii < m_arComs.GetSize(); ii++)
	{
		comp = m_arComs.GetAt(ii);
		if (comp->cRC.PtInRect(pt) && comp->bshow)
		{
			tmpS = CString(comp->comment);
			tmpS.TrimRight();
			return tmpS;
		}
	}	

	return _T("");
}

void CPnBaseWnd::DeleteComs()
{
	struct _comp* comp;
	for (int ii = 0; ii < m_arComs.GetSize(); ii++)
	{
		comp = m_arComs.GetAt(ii);
		LIB_DELETE(comp);
	}
	m_arComs.RemoveAll();	
}

void CPnBaseWnd::InvalidateCom(int idc, int status)
{
	struct _comp *comp = SearchCom(idc);
	InvalidateCom(comp, status);
}

void CPnBaseWnd::InvalidateCom(struct _comp *comp, int status)
{
	if (!comp)
		return;

	comp->status = status;
	InvalidateRect(comp->cRC);
}

void CPnBaseWnd::DrawBackground(CDC* pDC)
{
	CRect wRC;
	GetClientRect(&wRC);
	pDC->FillSolidRect(&wRC, m_clrBack);

	wRC.InflateRect(-1, -3, 0, -2);
	CPen pn(PS_NULL, 1, m_clrBack);
	CBrush *Inbrush = m_pApp->GetBrush(m_pwndView, m_clrInBack);
	
	CPen	*sPen = pDC->SelectObject(&pn);
	CBrush	*sBrush = pDC->SelectObject(Inbrush);
	pDC->RoundRect(wRC,CPoint(5,5));

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
}

void CPnBaseWnd::DrawFrame(CDC* pDC)
{
	CRect rect;
	GetClientRect(rect);
	rect.DeflateRect(1, 1, 1, 1);
	CPen	*fPen = m_pApp->GetPen(m_pwndView, PS_SOLID, 1, m_clrFrame);
	CPen	*sPen = pDC->SelectObject(fPen);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.left, rect.bottom);
	pDC->LineTo(rect.right, rect.bottom);
	pDC->LineTo(rect.right, rect.top);
	pDC->LineTo(rect.left, rect.top);
	pDC->SelectObject(sPen);
}

void CPnBaseWnd::DrawButton(CDC* pDC, int dtype, struct _comp *comp, CImageList* m_pImage)
{
	CRect cRC = comp->cRC;
	CPoint imgPt = CPoint(cRC.left, cRC.top);

	switch (dtype)
	{
	case dtButton:
		if (!m_pImage)
		{
			switch (comp->status)
			{
			case btnDown:
				cRC.DeflateRect(1, 1, 1, 1);
				pDC->DrawFrameControl(&cRC, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
				break;
			case btnMove:
				cRC.DeflateRect(1, 1, 1, 1);
				pDC->DrawEdge(&cRC, BDR_RAISEDINNER, BF_RECT);
				break;
			case btnUp:
			default:
				break;
			}
			cRC.DeflateRect(1, 1, 1, 1);
			CString tmpS = CString(comp->str);
			tmpS.TrimRight();
			int mode = pDC->SetBkMode(TRANSPARENT);
			pDC->DrawText(tmpS, comp->cRC, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			pDC->SetBkMode(mode);
			break;
		}
		switch (comp->status)
		{
		case btnDown:
			m_pImage->Draw(pDC, comp->imgsel, imgPt, ILD_TRANSPARENT);
			break;
		case btnUp:
			m_pImage->Draw(pDC, comp->imggen, imgPt, ILD_TRANSPARENT);
			break;
		case btnMove:
			m_pImage->Draw(pDC, comp->imgmov, imgPt, ILD_TRANSPARENT);
			break;
		case btnDis:
			m_pImage->Draw(pDC, comp->imgdis, imgPt, ILD_TRANSPARENT);
		}
		break;
	case dtColor:
		{
			switch (comp->status)
			{
			case btnDown:
				pDC->DrawFrameControl(&cRC, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
				break;
			case btnMove:
				pDC->DrawFrameControl(&cRC, DFC_BUTTON, DFCS_BUTTONPUSH);
				break;
			case btnUp:
			default:
				break;
			}
			cRC.DeflateRect(2, 2, 2, 2);
			CBrush	*cBrush = m_pApp->GetBrush(m_pwndView, m_crColourBk);
			pDC->FillRect(cRC, cBrush);
		}
		break;
	case dtWidth:
		{
			switch (comp->status)
			{
			case btnDown:
				pDC->DrawFrameControl(&cRC, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
				break;
			case btnMove:
				pDC->DrawFrameControl(&cRC, DFC_BUTTON, DFCS_BUTTONPUSH);
				break;
			case btnUp:
			default:
				break;
			}
			cRC.DeflateRect(2, 2, 2, 2);

			CBrush	*cBrush = m_pApp->GetBrush(m_pwndView, RGB(255, 255, 255));
			pDC->FillRect(cRC, cBrush);

			CPen	*cPen = m_pApp->GetPen(m_pwndView, PS_SOLID, 1, PALETTERGB(0, 0, 0));
			CPen	*sPen = pDC->SelectObject(cPen);
			int hori = cRC.top + (cRC.Height())/2 - m_wdLine/2;
			for (int ii = 0; ii < m_wdLine; ii++)
			{
				pDC->MoveTo(cRC.left+1, hori+ii+1);
				pDC->LineTo(cRC.right-2, hori+ii+1);
			}
			pDC->SelectObject(sPen);
		}
		break;
	case dtEtc:
		{
			switch (comp->status)
			{
			case btnDown:
			case btnMove:
				pDC->DrawFrameControl(&cRC, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);
				break;
			case btnUp:
			default:
				break;
			}
			cRC.DeflateRect(2, 2, 2, 2);
			CBrush	*cBrush = m_pApp->GetBrush(m_pwndView, ::GetSysColor(COLOR_BTNFACE));
			pDC->FillRect(&cRC, cBrush);
			if (m_pImage)
			{
				if (cRC.Width() < 10)
					imgPt.x = cRC.left;
				else
					imgPt.x = cRC.right-szCtrlImgX/2-1;
				imgPt.y = cRC.top;
				m_pImage->Draw(pDC, comp->imgsel, imgPt, ILD_TRANSPARENT);
			}
		}
		break;
	case dtSeparator:
		{
			cRC.DeflateRect(2, 0, 2, 0);

			int height = cRC.top + cRC.Height()/2;

			CPen	*grayPen = m_pApp->GetPen(m_pwndView, PS_SOLID, 1, PALETTERGB(128,128, 128));
			CPen	*whitePen = m_pApp->GetPen(m_pwndView, PS_SOLID, 1, PALETTERGB(255, 255, 255));
			CPen	*sPen = pDC->SelectObject(grayPen);

			pDC->MoveTo(cRC.left, height);
			pDC->LineTo(cRC.right, height);

			pDC->SelectObject(whitePen);
			pDC->MoveTo(cRC.left, height+1);
			pDC->LineTo(cRC.right, height+1);
			
			pDC->SelectObject(sPen);
		}
		break;
	}
}
