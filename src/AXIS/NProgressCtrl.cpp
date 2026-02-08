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
	m_nKind  = PK_NORMAL;
	CalcRange();
}

CNProgressCtrl::~CNProgressCtrl()
{
	for (int ii = 0; ii < 5; ii++)
		if (m_img[ii].GetSafeHandle())
			m_img[ii].DeleteObject();
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
	if (!m_img[0].GetSafeHandle())
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
	switch (m_nKind)
	{
	case PK_NORMAL:
		{
			HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), imgName,
				IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			if (hBitmap)	m_img[0].Attach(hBitmap);
			
			hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), back,
				IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			if (hBitmap)	m_back.Attach(hBitmap);
		}
		break;
	case PK_AQUA:
		{
			HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), imgName + "_left.bmp",
				IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			if (hBitmap)	m_img[0].Attach(hBitmap);

			hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), imgName + "_mid.bmp",
				IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			if (hBitmap)	m_img[1].Attach(hBitmap);

			hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), imgName + "_right.bmp",
				IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			if (hBitmap)	m_img[2].Attach(hBitmap);

			hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), imgName + "_right_end.bmp",
				IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			if (hBitmap)	m_img[3].Attach(hBitmap);
			
			hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), back,
				IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
			if (hBitmap)	m_back.Attach(hBitmap);
		}
		break;
	}

	if (m_img[0].GetSafeHandle() || m_back.GetSafeHandle())
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
				if (m_nKind == PK_AQUA)
				{
					BITMAP bm;
					m_back.GetBitmap(&bm);
					pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
				}
				else
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

	if (m_nKind == PK_NORMAL)
	{
		// Calculate control's percentage to draw
		const int	rate = (int)((100.0/m_nRange)*(abs(m_nLower)+m_nPos));
		const float	f = ((float)(cRc.Height())/100)*rate;
		
		if ((cRc.bottom - (int)f) < cRc.top)
			cRc.top += 1;
		else	cRc.top = cRc.bottom - (int)f;
		
		if (m_img[0].GetSafeHandle())
		{
			if (memDC.CreateCompatibleDC(pDC))
			{
				CBitmap* oldbuffBitmap = memDC.SelectObject(&m_img[0]);
				if (oldbuffBitmap != (CBitmap *) 0)
				{
					pDC->BitBlt(cRc.left, cRc.top, cRc.Width(), cRc.Height(), &memDC, 0, cRc.top, SRCCOPY);
					memDC.SelectObject(oldbuffBitmap);
				}
				memDC.DeleteDC();
			}
		}
	}
	else if (m_nKind == PK_AQUA)
	{
		// Calculate control's percentage to draw
		const int	rate = (int)((100.0/m_nRange)*(abs(m_nLower)+m_nPos));
		const float	f = ((float)(cRc.Width())/100)*rate;
		cRc.left++;
		if ((cRc.left + (int)f) > cRc.right)
			cRc.right -= 1;
		else	cRc.right = cRc.left + (int)f;

		if (m_img[0].GetSafeHandle() && m_img[1].GetSafeHandle() &&
			m_img[2].GetSafeHandle() && m_img[3].GetSafeHandle())
		{
			if (memDC.CreateCompatibleDC(pDC))
			{
				int nLeft = cRc.left;
				BITMAP bm{};
				CBitmap* oldbuffBitmap = memDC.SelectObject(&m_img[0]);	// 처음
				m_img[0].GetBitmap(&bm);
				if (oldbuffBitmap != (CBitmap *) 0)
				{
					pDC->BitBlt(nLeft, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
					memDC.SelectObject(oldbuffBitmap);
				}
				memDC.DeleteDC();
				nLeft += bm.bmWidth;

				if (cRc.Width() > 15 && memDC.CreateCompatibleDC(pDC))
				{
					oldbuffBitmap = memDC.SelectObject(&m_img[1]);	// 중간
					m_img[1].GetBitmap(&bm);
					if (oldbuffBitmap != (CBitmap *) 0)
					{
						pDC->StretchBlt(nLeft, 0, cRc.Width() - (nLeft - cRc.left) - 9, bm.bmHeight, &memDC, 0, 0, 1, bm.bmHeight, SRCCOPY);
						memDC.SelectObject(oldbuffBitmap);
					}
					memDC.DeleteDC();
					nLeft += cRc.Width() - (nLeft - cRc.left) - 9;
				}

				if (memDC.CreateCompatibleDC(pDC))
				{
					if (rate < 99)
					{
						oldbuffBitmap = memDC.SelectObject(&m_img[2]);	// 중간의 끝부분
						m_img[2].GetBitmap(&bm);
						if (oldbuffBitmap != (CBitmap *) 0)
						{
							pDC->BitBlt(nLeft, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
							memDC.SelectObject(oldbuffBitmap);
						}
					}
					else
					{
						oldbuffBitmap = memDC.SelectObject(&m_img[3]);	// Last
						m_img[3].GetBitmap(&bm);
						if (oldbuffBitmap != (CBitmap *) 0)
						{
							pDC->BitBlt(nLeft, 0, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
							memDC.SelectObject(oldbuffBitmap);
						}
						
					}
					memDC.DeleteDC();
				}
			}
		}
	}
}

void CNProgressCtrl::SetImageMode(int nKind)
{
	m_nKind = nKind;
}
