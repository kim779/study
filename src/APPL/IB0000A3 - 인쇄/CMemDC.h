#pragma once


//#include <windows.h>
////////////////////////////////////////////////////////////////////////////////
// CMemDC - memory DC
////////////////////////////////////////////////////////////////////////////////


class CMemDC : public CDC
{
private:
	BOOL		m_bMemDC{};
	CBitmap		m_MemBitmap{};
	CBitmap*	m_pOldBitmap{};
	CDC*		m_pDC{};
	CRect		m_rect{};

public:
	CMemDC(CDC* pDC, const CRect* pRect = NULL) : CDC()
	{
		ASSERT(pDC != NULL);

		m_pDC = pDC;
		m_bMemDC = !pDC->IsPrinting();

		if (pRect == NULL)
			pDC->GetClipBox(&m_rect);
		else
			m_rect = *pRect;

		if (m_bMemDC)
		{
			CreateCompatibleDC(pDC);

			pDC->LPtoDP(&m_rect);
			m_MemBitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_pOldBitmap = SelectObject(&m_MemBitmap);
			SetMapMode(pDC->GetMapMode());
			SetWindowExt(pDC->GetWindowExt());
			SetViewportExt(pDC->GetViewportExt());
			pDC->DPtoLP(&m_rect);

			// set window origin
			SetWindowOrg(m_rect.left, m_rect.top);
		}
		else
		{
			m_bPrinting = pDC->m_bPrinting;
			m_hDC		= pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}

		// fill background
		CWnd* pWnd = pDC->GetWindow();
		HWND hWnd = NULL;
		if (pWnd != NULL)
		{
			hWnd = pWnd->m_hWnd;

			if (hWnd != NULL)
			{
				if (GetClassLong(hWnd, GCW_ATOM) == 32770)
				{
					FillSolidRect(m_rect, GetSysColor(COLOR_BTNFACE));
				}
				else
				{
					DWORD ulpBrush = GetClassLong(hWnd, GCL_HBRBACKGROUND);
					if (ulpBrush)
					{
						// fill with class background brush
						FillRect(m_rect, CBrush::FromHandle((HBRUSH)ulpBrush));
					}
					else
						hWnd = NULL;
				}
			}
		}
		if (hWnd == NULL)
			FillSolidRect(m_rect, pDC->GetBkColor());
	}

	~CMemDC()
	{
		if (m_bMemDC)
		{
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
						  this, m_rect.left, m_rect.top, SRCCOPY);

			m_pDC->SelectObject(m_pOldBitmap);
		}
		else
		{
			m_hDC = m_hAttribDC = NULL;
		}
	}

	CMemDC* operator->() { return this; }

	operator CMemDC*() { return this; }
};
