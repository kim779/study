// NProgressCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "NProgressCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNProgressCtrl

CNProgressCtrl::CNProgressCtrl()
{
	m_nPos	 = 0;
	m_nStep	 = 10;
	m_nLower = 0;
	m_nUpper = 100;
	CalcRange();
}

CNProgressCtrl::~CNProgressCtrl()
{
	if (m_img.GetSafeHandle())	m_img.DeleteObject();
	if (m_back.GetSafeHandle())	m_back.DeleteObject();
}


BEGIN_MESSAGE_MAP(CNProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CNProgressCtrl)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNProgressCtrl message handlers

void CNProgressCtrl::OnPaint() 
{
	if (!m_img.GetSafeHandle())
	{
		CProgressCtrl::OnPaint();
		return;
	}

	CPaintDC	dc(this); // device context for painting
	CDC		memDC;

	if (!memDC.CreateCompatibleDC(&dc))
	{
		CProgressCtrl::OnPaint();
		return;
	}

	CRect	cRc;
	CBitmap	bmp, *pbmp;

	GetClientRect(cRc);
	bmp.CreateCompatibleBitmap(&dc, cRc.Width(), cRc.Height());
	pbmp = (CBitmap *) memDC.SelectObject(&bmp);

	drawBackground(&memDC);
	drawProgress(&memDC);

	dc.BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pbmp);
	memDC.DeleteDC();
}

void CNProgressCtrl::SetImage(CString imgName, CString back)
{
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), imgName,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)	m_img.Attach(hBitmap);

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), back,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)	m_back.Attach(hBitmap);

	if (m_img.GetSafeHandle() || m_back.GetSafeHandle())
		ModifyStyleEx(WS_EX_STATICEDGE, 0, SWP_DRAWFRAME);
}

void CNProgressCtrl::SetRange(int nLower, int nUpper)
{
	m_nLower = nLower;
	m_nUpper = nUpper;
	CalcRange();

	CProgressCtrl::SetRange(nLower, nUpper);
}

int CNProgressCtrl::SetStep(int nStep)
{
	m_nStep = nStep;
	return CProgressCtrl::SetStep(nStep);
}

int CNProgressCtrl::SetPos(int nPos)
{
	// Bound checking
	if (nPos < m_nLower) nPos = m_nLower;
	if (nPos > m_nUpper) nPos = m_nUpper;

	m_nPos = nPos;
	return CProgressCtrl::SetPos(nPos);
}

int CNProgressCtrl::StepIt()
{
	m_nPos += m_nStep;

	// Bound checking
	if (m_nPos > m_nUpper) m_nPos = m_nLower + (m_nPos - m_nUpper);

	return CProgressCtrl::StepIt();
}

void CNProgressCtrl::CalcRange()
{
	m_nRange = abs(m_nUpper - m_nLower);
	// Avoid divide by zero
	if (m_nRange == 0) m_nRange = 1;
} // End of CalcRange

void CNProgressCtrl::drawBackground(CDC* pDC)
{
	CDC	 memDC;
	CRect	 cRc;

	GetClientRect(cRc);
	if (m_back.GetSafeHandle())
	{
		if (memDC.CreateCompatibleDC(pDC))
		{
			CBitmap* oldbuffBitmap = memDC.SelectObject(&m_back);
			if (oldbuffBitmap != (CBitmap *) 0)
			{
				pDC->BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
				memDC.SelectObject(oldbuffBitmap);
			}
			memDC.DeleteDC();
		}
	}
	
}

// only vetical progress
void CNProgressCtrl::drawProgress(CDC* pDC)
{
	CDC	 memDC;
	CRect	 cRc;

	GetClientRect(cRc);
#if 0
	// Calculate control's percentage to draw
	int	rate = (int)((100.0/m_nRange)*(abs(m_nLower)+m_nPos));
	float	f = ((float)(cRc.Height())/100)*rate;

	if ((cRc.bottom - (int)f) < cRc.top)
		cRc.top += 1;
	else	cRc.top = cRc.bottom - (int)f;
	
	if (m_img.GetSafeHandle())
	{
		if (memDC.CreateCompatibleDC(pDC))
		{
			CBitmap* oldbuffBitmap = memDC.SelectObject(&m_img);
			if (oldbuffBitmap != (CBitmap *) 0)
			{
				pDC->BitBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), &memDC, 0, cRc.top, SRCCOPY);
				memDC.SelectObject(oldbuffBitmap);
			}
			memDC.DeleteDC();
		}
	}
#else
	int	rate = (int)((100.0/m_nRange)*(abs(m_nLower)+m_nPos));
	float	f = ((float)(cRc.Width())/100)*rate;

	if ((cRc.left + (int)f) > cRc.right)
		cRc.right -= 1;
	else	
		cRc.right = cRc.left + (int)f;
	
	if (m_img.GetSafeHandle())
	{
		if (memDC.CreateCompatibleDC(pDC))
		{
			CBitmap* oldbuffBitmap = memDC.SelectObject(&m_img);
			if (oldbuffBitmap != (CBitmap *) 0)
			{
				pDC->BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
				memDC.SelectObject(oldbuffBitmap);
			}
			memDC.DeleteDC();
		}
	}
#endif
}
