#pragma once

#define DEFAULT_TXT_SIZE	9

class SMemDC : public CDC  
{
	public:
	SMemDC::SMemDC(CDC* pDC)
	{
		ASSERT(pDC != NULL);
		m_pDC = pDC;
		
		Width = 0;
		Height = 0;

		Pen = OldPen = NULL;
		OldPen = NULL;
		Brush = OldBrush = NULL;
		m_pBitmap = m_pOldBitmap = NULL;
		Font = OldFont = NULL;

		//m_pDC->GetClipBox(&m_rc);
		CWnd *pWnd = CWnd::GetDesktopWindow();
		pWnd->GetWindowRect(&m_rc);

		CreateCompatibleDC(m_pDC);
		SetMapMode(pDC->GetMapMode());

		m_pBitmap = new CBitmap;

		CBitmap bmp;
		bmp.CreateCompatibleBitmap(m_pDC, 1, 1);

		HBITMAP hbm = (HBITMAP)bmp;
		BITMAP bm;	
		GetObject(hbm,sizeof(bm),&bm);

		BOOL r = m_pBitmap->CreateBitmap( m_rc.Width(), m_rc.Height(), bm.bmPlanes, bm.bmBitsPixel, NULL);
		bmp.DeleteObject();

		if(!r)
		{
			delete m_pBitmap;
			m_pBitmap = NULL;
			m_bValid = FALSE;
			TRACE("비트맵 생성에 실패하였습니다. - %s(%d)\n", __FILE__, __LINE__);
		}
		else
		{
			
			Width = m_rc.Width();
			Height = m_rc.Height();

			m_pOldBitmap = SelectObject(m_pBitmap);

	//		Pen = new CPen(PS_SOLID, 1, RGB(0,0,0));
	//		OldPen = (CPen *) SelectObject(Pen);	

			Brush = new CBrush(RGB(255,255,255));
			OldBrush = (CBrush *) SelectObject(Brush);

			// LOGFONT lf;
			memset(&LFont, 0, sizeof(LOGFONT));		// clear out structure
			LFont.lfHeight = -MulDiv(12, GetDeviceCaps(LOGPIXELSY), 72);  // request a 12-pixel-size font
			LFont.lfWeight = FW_NORMAL;					// fsNormal, fsBold
			LFont.lfCharSet = HANGUL_CHARSET;
			strcpy(LFont.lfFaceName, "바탕체");		// 굴림체
			Font = new CFont();
			Font->CreateFontIndirect(&LFont);	// create the font

			// Convert the HFONT to CFont*.
			// Font = CFont::FromHandle(HFont);
			OldFont = (CFont *) SelectObject(Font);

			m_bValid = TRUE;
		}
	}

	void	SMemDC::clear()
	{
		if(!m_bValid) return;
		
		if (Pen && Pen->GetSafeHandle())
		{
			Pen->DeleteObject();
			delete Pen;
		}

		if (Brush && Brush->GetSafeHandle())
		{
			Brush->DeleteObject();
			delete Brush;
		}
		if (Font && Font->GetSafeHandle())
		{
			Font->DeleteObject();
			delete Font;
		}


		if (m_pBitmap && m_pBitmap->GetSafeHandle()) 
		{
			m_pBitmap->DeleteObject();
			delete m_pBitmap;
		}

		m_bValid = FALSE;

	}

	void SetBitmapFont(CString sFont, int fontWeight=FW_NORMAL, int fontSize=DEFAULT_TXT_SIZE)
	{
		// LOGFONT lf;
		if(!GetSafeHdc())
			return;

		LPTSTR fontName;
		fontName = (LPTSTR) sFont.GetBuffer(sFont.GetLength());

		memset(&LFont, 0, sizeof(LOGFONT));			// 
		LFont.lfHeight = -MulDiv(fontSize, GetDeviceCaps(LOGPIXELSY), 72);  // 12-pixel-height font
		LFont.lfWeight = fontWeight;					// fsNormal, fsBold
		LFont.lfCharSet = HANGUL_CHARSET;
		strcpy(LFont.lfFaceName, fontName);			// 

		CFont *pNewFont = new CFont();
		CFont *pOldFont;
		pNewFont->CreateFontIndirect(&LFont);	// create the font

		// Convert the HFONT to CFont*.
		// Font = CFont::FromHandle(HFont);
		pOldFont = SelectObject(pNewFont);
		if(pOldFont)
		{
			if (Font && Font->GetSafeHandle())
			{
				Font->DeleteObject();
				delete Font;
				Font = NULL;
			}
			Font = pNewFont;
		}
		else
		{
			if (pNewFont && pNewFont->GetSafeHandle())
			{
				pNewFont->DeleteObject();
				delete pNewFont;
				pNewFont = NULL;
			}
		}

		// Convert the HFONT to CFont*.
		// pNewFont = CFont::FromHandle(hfont);
		// SelectObject(pNewFont);
		// Font = pNewFont;
	}

	SMemDC::~SMemDC()
	{
		clear();
	}

public:
	CPen *Pen, *OldPen;
	CBrush *Brush, *OldBrush;
	CFont *Font, *OldFont;
	LOGFONT LFont;
	int Width, Height;

public:
    CDC*     m_pDC;        
    CRect    m_rc;         // Rectangle of drawing area.
    CBitmap* m_pBitmap;     // Offscreen bitmap
    CBitmap* m_pOldBitmap; // Original GDI object
	BOOL	 m_bValid;	   // flag for autodraw in dtor
};




