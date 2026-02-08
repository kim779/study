// WxReport.cpp: implementation of the CWxReport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IB0000A3.h"
#include "WxReport.h"
#include "MainDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWxReport::CWxReport()
{

	m_iBottomMargin = 100;
}

CWxReport::~CWxReport()
{

}

LOGFONT CWxReport::GetLogFont(int fsize, bool bold)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = int( (double)fsize * m_lscaley) ;
	
	strcpy(lf.lfFaceName, "굴림체");   
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	if(bold)
		lf.lfWeight = FW_BOLD;
	else
		lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	return lf;
}

void CWxReport::DrawBox(CDC* pDC, CRect& rect)
{
}
void CWxReport::SetDrawRect(CDC* pDC,  CRect rect)
{
	pDC->Rectangle(rect); 
}

int CWxReport::DrawTitle(CDC* pDC,  CRect rect, CString sTitle, int* irowcnt, Line* stleft, Line* stright)
{
	CRect trect = rect;
	trect.bottom = trect.top + m_iHdrLineHeight;
	UINT	uiFormat = DT_SINGLELINE|DT_VCENTER|DT_LEFT;
	const int ihdrrows = max(irowcnt[0], irowcnt[1]);

	CFont* poldFont = pDC->SelectObject(&m_pTitleFont); 
	
	CSize tsize = pDC->GetTextExtent(sTitle); 
	int itltleft = (trect.Width() / 2 ) - tsize.cx/2 ; 
	if(trect.left > itltleft)
	{
		itltleft += trect.left;
	}
	const int itltbot = trect.top + tsize.cy + 10;
	pDC->DrawText(sTitle, CRect(itltleft,trect.top, itltleft+tsize.cx, itltbot), uiFormat);
	pDC->SelectObject(poldFont); 


	poldFont = pDC->SelectObject(&m_pHdrFont); 
	trect.DeflateRect(2, 0,0,2); 

	const CRect hrect = trect;
	CString sHdrText, sRight[10];
	int i = 0;

	for(int i=0; i<irowcnt[0]; i++)
	{
		pDC->DrawText(stleft[i].sText[0], trect, uiFormat);
		sRight[i] = m_pMain->GetHeaderText(stright[i].sText[1]);
		if( sRight[i].GetLength() >  sHdrText.GetLength() ) sHdrText = sRight[i];
		trect.top = trect.bottom;
		trect.bottom = trect.top + m_iHdrLineHeight;
	}

	//right
	tsize = pDC->GetTextExtent(stright[0].sText[0] + sHdrText); 
	trect = hrect;
	const int igap = 10;
	trect.left = trect.right - tsize.cx - igap;
	uiFormat = DT_SINGLELINE|DT_VCENTER;

	for( i=0; i<irowcnt[0]; i++)
	{
		pDC->DrawText(stright[i].sText[0], trect, uiFormat|DT_LEFT);
		pDC->DrawText(sRight[i], trect, uiFormat|DT_RIGHT);


		trect.top = trect.bottom;
		trect.bottom = trect.top + m_iHdrLineHeight;
	}
	pDC->SelectObject(poldFont);
	DeleteObject(poldFont);

	return m_iLineHeight * ihdrrows;
}

int CWxReport::DrawListGrid(CDC* pDC,  CRect rect, int icolcnt,  int iblrowcnt, Line* stLine, COutDataListArray& arDataList, int& inext)
{
	CFont* poldFont = pDC->SelectObject(&m_pFont); 
	int ipagerows = rect.Height() / m_iLineHeight;
	if(ipagerows % 2 > 0) ipagerows--;

	const int idrawrows = (arDataList.GetSize() - inext) * iblrowcnt + iblrowcnt;
	ipagerows = min(ipagerows, idrawrows);

	const int iColWidth = rect.Width() / icolcnt; 
	const int itw  = rect.Width();
	int icolwlist[MAXCOUNT]{};

	CRect trect = rect;
	trect.DeflateRect(2, 0, 2,0); 
	CRect grect = trect;
	grect.bottom = trect.top + (m_iLineHeight * ipagerows);
	DrawRectangle(pDC, grect);

	icolwlist[0] = iColWidth;
	if( stLine[0].lWidthDif[0] > 0)
		icolwlist[0] = int((double)itw * stLine[0].lWidthDif[0]);

	int iw = trect.left + icolwlist[0];
	int i = 0;
	for(int i=1; i<icolcnt; i++)
	{
		pDC->MoveTo(iw, trect.top);   
		pDC->LineTo(iw, grect.bottom ); 

		if( stLine[0].lWidthDif[i] > 0)
			icolwlist[i] = int((double)itw * stLine[0].lWidthDif[i]);
		else icolwlist[i] = iColWidth;

		iw += icolwlist[i];
	}

	int ih = trect.top + m_iLineHeight;

	for(i=1; i<ipagerows; i++)
	{
		pDC->MoveTo(trect.left, ih);   
		pDC->LineTo(trect.right, ih ); 

		ih += m_iLineHeight;
	}

	const int ileft = trect.left;

	trect.right = ileft+icolwlist[0];
	trect.bottom = trect.top + m_iLineHeight;
	CRect txrect = trect;

	UINT	uiFormat = DT_SINGLELINE|DT_VCENTER|DT_CENTER;
	
	for(int r=0; r<iblrowcnt; r++)
	{
		for(i=0; i<icolcnt; i++)
		{
			txrect.DeflateRect(2,0,1,0 );
			pDC->DrawText(stLine[r].sText[i], txrect, uiFormat);
			trect.left = trect.right;
			trect.right = trect.left+icolwlist[i+1];

			txrect = trect;
		}

		trect.top = trect.bottom;
		trect.bottom = trect.top + m_iLineHeight;

		trect.left = ileft;
		trect.right = ileft+icolwlist[0];
		txrect = trect;
	}

	
	trect.right = ileft+icolwlist[0];
	trect.bottom = trect.top + m_iLineHeight;
	txrect = trect;

	if(arDataList.GetSize() <= 0) return 0;
	int igridrow = iblrowcnt;
	int idx = 0;


	if(arDataList.GetSize() <= inext) inext= 0;
	int iline = iblrowcnt;
	uiFormat = DT_SINGLELINE|DT_VCENTER;
	const int itxtmargin = int( (double)m_pMain->GetGridTextMargin() * m_lscalex);

	for( i=inext; i<arDataList.GetSize(); i++)
	{
		for(int r=0; r<iblrowcnt; r++)
		{
			for(int c=0; c<icolcnt; c++)
			{
				txrect.DeflateRect(itxtmargin,0,itxtmargin,0 );

				idx = CInt( stLine[igridrow].sText[c]);
				pDC->DrawText(arDataList.GetAt(i).strData[idx], txrect, uiFormat|stLine[r].iTextAlign[c] );
				trect.left = trect.right;
				trect.right = trect.left+icolwlist[c+1];

				txrect = trect;
			}

			igridrow++;
			trect.top = trect.bottom;
			trect.bottom = trect.top + m_iLineHeight;

			trect.left = ileft;
			trect.right = ileft+icolwlist[0];
			txrect = trect;
			iline++;
		}

		if(iline >= ipagerows) 
		{
			i++;
			break;
		}
		igridrow = iblrowcnt;
	}

	if(arDataList.GetSize() <= i) inext= 0;
	if(arDataList.GetSize() > i) inext= i;

	pDC->SelectObject(poldFont); 
	DeleteObject(poldFont);
	return trect.bottom;
}


int CWxReport::DrawGrid(CDC* pDC,  CRect rect, int icolcnt, int irowcnt, Line* stLine/*int* iTxtType, CString* sText*/)
{
	const int iColWidth = rect.Width() / icolcnt;
	const int itw  = rect.Width();
	int	icolwlist[MAXCOUNT]{};

	CRect trect = rect;
	const int itop = trect.top;
	trect.DeflateRect(2, 0,2,0); 
	rect.bottom = trect.bottom = itop + (m_iLineHeight * irowcnt);
	DrawRectangle(pDC, trect); 

	icolwlist[0] = iColWidth;
	if( stLine[0].lWidthDif[0] > 0)
		icolwlist[0] = int((double)itw * stLine[0].lWidthDif[0]);

	UINT	uiFormat = DT_SINGLELINE|DT_VCENTER|DT_LEFT;
	int iw = trect.left + icolwlist[0];
	int i = 0;
	for(i=1; i<icolcnt; i++)
	{
		pDC->MoveTo(iw, trect.top);   
		pDC->LineTo(iw, trect.bottom ); 
	
		if( stLine[0].lWidthDif[i] > 0)
			icolwlist[i] = int((double)itw * stLine[0].lWidthDif[i]);
		else icolwlist[i] = iColWidth;

		iw += icolwlist[i];
	}

	int ih = trect.top + m_iLineHeight;
	for(i=1; i<irowcnt; i++)
	{
		pDC->MoveTo(trect.left, ih);   
		pDC->LineTo(trect.right, ih ); 

		ih += m_iLineHeight;
	}

	trect.right = trect.left+icolwlist[0];
	trect.bottom = itop + m_iLineHeight;
	CRect txrect = trect;
	
	const int itxtmargin = int( (double)m_pMain->GetGridTextMargin() * m_lscalex );

	const int ileft = trect.left;
	uiFormat = DT_SINGLELINE|DT_VCENTER;
	for(int ir=0; ir<irowcnt;ir++)
	{
		for(i=0; i<icolcnt; i++)
		{
			txrect.DeflateRect(itxtmargin, 0, itxtmargin, 0 );
			pDC->DrawText(stLine[ir].sText[i], txrect, uiFormat|stLine[ir].iTextAlign[i]);

			trect.left = trect.right;
			trect.right = trect.left+icolwlist[i+1];
			txrect = trect;
		}

		trect.left = ileft;
		trect.right = trect.left+icolwlist[0];
		trect.top = trect.bottom;
		trect.bottom = trect.top + m_iLineHeight;
		txrect = trect;
	}

	return rect.bottom;
}

int CWxReport::DrawTextLine(CDC* pDC, CRect rect, CString sDtText, int iCnt, Line* stLine)
{
	
	CRect trect =rect;
	trect.bottom = rect.top + m_iLineHeight;

	const UINT	uiFormat = DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX   ;
	if(!sDtText.IsEmpty() )
	{
		trect.top = trect.bottom;
		trect.bottom = trect.top + m_iLineHeight;
		trect.DeflateRect(2,0,1,0 );
		pDC->DrawText(sDtText, trect, uiFormat);
	}

	for(int i=0; i<iCnt; i++)
	{
		pDC->DrawText(stLine[i].sText[0], trect, uiFormat);
		trect.top = trect.bottom;
		trect.bottom = trect.top + m_iLineHeight;
	}

	return trect.bottom;
}

int CWxReport::DrawTextMultiLine(CDC* pDC, CRect rect, CString sDtText, int irowcnt, CStringArray& arLines, int& inext)
{
	DrawRectangle(pDC, rect); 
	CRect trect =rect;
	const int itxtmargin = int( (double)m_pMain->GetTextMargin() * m_lscalex );

	trect.DeflateRect(itxtmargin, 0, itxtmargin,0);  
	trect.bottom = rect.top + m_iLineHeight;
	const UINT	uiFormat = DT_SINGLELINE|DT_VCENTER|DT_LEFT;

	CFont* poldFont = pDC->SelectObject(&m_pMultiLineFont); 
	for(int i=inext; i<arLines.GetSize() ; i++)
	{
		if(rect.bottom <= trect.bottom)
		{
			inext = i-1;
			break;
		}

		pDC->DrawText(arLines[i], trect, uiFormat);
		trect.top = trect.bottom;
		trect.bottom = trect.top + m_iLineHeight;
	}

	pDC->SelectObject(poldFont); 
	::DeleteObject(poldFont);
	
	return trect.bottom;
}

#include "FormParser.h"
int CWxReport::DrawPage(int& icontentidx, int ipage, CDC* pDC, CRect rect, int inext)
{
	if(icontentidx < 0) return 0;
	CRect drect = rect; 

	struct	Line			stLines[MAXCOUNT], stRight[10];
 	if(ipage == 0)
	{
		m_pMain->m_clParser.GetTitle(0, stLines);
		m_pMain->m_clParser.GetTitle(1, stRight);
		drect.top += DrawTitle(pDC, drect, m_pMain->m_clParser.m_strTitle, m_pMain->m_clParser.m_iTitleRow, stLines, stRight);
	}

	CStringArray	arLines;
	int				iLineCnt = 0;
	const int idxP = 0;
	int i = 0;
	for(i=icontentidx; i<m_pMain->m_clParser.m_iContentCount; i++)
	{
		switch(m_pMain->m_clParser.GetDrawType(i))
		{
			case SPACEDRAW:
				drect.top += m_iSpaceHeight * m_pMain->m_clParser.GetLineCount(i);
				break;
			case GRIDDRAW:
				m_pMain->m_clParser.GetLine(i, stLines);
				drect.top = DrawGrid(pDC, drect,  m_pMain->m_clParser.GetColCount(i), m_pMain->m_clParser.GetRowCount(i), stLines);// pData.iDataType,  pData.sText); 
				break;

			case LISTDRAW:
				m_pMain->m_clParser.GetLine(i, stLines);
				drect.top = DrawListGrid(pDC, drect,  m_pMain->m_clParser.GetColCount(i), m_pMain->m_clParser.GetRowCount(i), 
					stLines, m_pMain->m_arGridList, inext);
				break;

			case TEXTDRAW:
				m_pMain->m_clParser.GetLine(i, stLines);
				drect.top = DrawTextLine(pDC, drect, m_pMain->m_clParser.GetDTText(i), m_pMain->m_clParser.GetLineCount(i), stLines);
				break;

			case MULTILINETEXTDRAW:
				const int idx = m_pMain->m_clParser.GetMultiLineSubIndex(i);
				iLineCnt = m_pMain->GetMultiLineSze(idx, arLines);
				drect.top = DrawTextMultiLine(pDC, drect, m_pMain->m_clParser.GetDTText(i), iLineCnt, arLines, inext);
				
				break;

		}
	}

	m_pMain->SetProgressPos(drect.top/100); 
	if(inext) icontentidx = i-1 ;
	else icontentidx = i;

	return inext;
}

int CWxReport::GetTotPage(CRect rect)
{
	const int ihdrcnt = max(m_pMain->m_clParser.m_iTitleRow[0], m_pMain->m_clParser.m_iTitleRow[1]);
	const int ih = rect.Height();
	int itotheight = ihdrcnt * m_iHdrLineHeight;

	CString strTemp;

	int ipage = 1;
	int ilblrow = 1, idx=0;
	for(int i=0; i<m_pMain->m_clParser.m_iContentCount; i++)
	{
		switch(m_pMain->m_clParser.GetDrawType(i))
		{
			case SPACEDRAW://space
				itotheight += m_iLineHeight * m_pMain->m_clParser.GetLineCount(i);

// 				strTemp.Format("[KSJ] SPACEDRAW = %d", m_iLineHeight * m_pMain->m_clParser.GetLineCount(i));
// 				OutputDebugString(strTemp);

				break;

			case GRIDDRAW://grid
				itotheight +=m_pMain->m_clParser.GetRowCount(i)* m_iLineHeight;

// 				strTemp.Format("[KSJ] GRIDDRAW = %d", m_pMain->m_clParser.GetRowCount(i)* m_iLineHeight);
// 				OutputDebugString(strTemp);

				break;

			case LISTDRAW://list
				ilblrow = m_pMain->m_clParser.GetRowCount(i);
				itotheight += ilblrow * m_iLineHeight; //lbl
				itotheight += (ilblrow*m_pMain->m_arGridList.GetSize()) * m_iLineHeight; //lbl
				
// 				strTemp.Format("[KSJ] LISTDRAW = %d", ilblrow);
// 				OutputDebugString(strTemp);

				break;

			case TEXTDRAW://text
				itotheight += m_pMain->m_clParser.GetLineCount(i) * m_iLineHeight;

// 				strTemp.Format("[KSJ] TEXTDRAW = %d", m_pMain->m_clParser.GetLineCount(i) * m_iLineHeight);
// 				OutputDebugString(strTemp);

				break;

			case MULTILINETEXTDRAW:
				idx = m_pMain->m_clParser.GetMultiLineSubIndex(i);
				itotheight += m_pMain->GetMultiLineSze(idx) * m_iLineHeight;

// 				strTemp.Format("[KSJ] MULTILINETEXTDRAW = %d", idx);
// 				OutputDebugString(strTemp);

				break;
		}
	}
	//2011.11.14 KSJ
	//0.16이 찾아낸 interval
	//하나넘어간 페이지 4.8414596749463354799141367678626
	//꽉찬페이지 4.8353265869365225390984360625575
	//2015.03.12 KSJ 0.16 더해준는 부분 삭제
	double dPage = (itotheight/(ih*1.0));
	ipage = (int)dPage/1;

// 	strTemp.Format("[KSJ] 11111 dPage = %0.5f, ipage = %d", dPage, ipage);
// 	OutputDebugString(strTemp);

	//2015.03.12 KSJ 페이지당 0.07을 곱해준값을 더해준다.
	dPage = (itotheight/(ih*1.0) + 0.07*ipage);
	ipage = (int)dPage/1;

// 	strTemp.Format("[KSJ] 22222 dPage = %0.5f, ipage = %d", dPage, ipage);
// 	OutputDebugString(strTemp);

	if(itotheight % ih > 0) ipage++;		//여기는 반드시 해줘야함. 다른곳에서도 이쪽을 사용

// 	strTemp.Format("[KSJ] ihdrcnt = %d, m_iLineHeight = %d, Height = %d, Index = %d, itotheight = %d, ipage = %d, dPage = %0.5f", ihdrcnt, m_iLineHeight, ih, i, itotheight, ipage, dPage);
// 	OutputDebugString(strTemp);

	m_pMain->SetProgressRange(itotheight/100, 0); 
	return  ipage; 
}

void CWxReport::DrawBottom(CDC* pDC, CRect rect, int itotpage, int ipage)
{
	CFont* poldFont = pDC->SelectObject(&m_pHdrFont); 
	struct	Line			stLeft[10], stRight[10];
	m_pMain->m_clParser.GetBottom(0, stLeft);
	m_pMain->m_clParser.GetBottom(1, stRight);

	const UINT	uiFormat = DT_SINGLELINE|DT_VCENTER|DT_LEFT;
	const 	int irows = max(m_pMain->m_clParser.m_iBotRow[0], m_pMain->m_clParser.m_iBotRow[1]);

	CRect trect = rect;
	trect.bottom = trect.top + m_iHdrLineHeight;
	
	CSize tsize; 
	const int gap = 10;

	for(int i=0; i<irows; i++)
	{
		if(m_pMain->m_clParser.m_iBotRow[0] > i)
			pDC->DrawText(stLeft[i].sText[0], trect, uiFormat);

		if(m_pMain->m_clParser.m_iBotRow[1] > i)
		{
			if( stRight[i].iDataType[0]  == LBLDATATYPE)
			{
				tsize = pDC->GetTextExtent(stRight[i].sText[0]); 
				pDC->DrawText(stRight[i].sText[0], CRect(trect.right - tsize.cx - gap , trect.top, trect.right, trect.bottom), uiFormat);
			}
			else if( stRight[i].iDataType[0]  == LOGODATATYPE)
			{
				DrawBitmap(stRight[i].sText[0], pDC, CRect(trect.right - tsize.cx - gap, trect.top, trect.right, trect.bottom) );
			}
		}

		trect.top = trect.bottom;
		trect.bottom = trect.top + m_iHdrLineHeight;
	}


	
	CString spage = " 99 / 99 ";
	tsize = pDC->GetTextExtent(spage); 

	trect = rect;
	if(m_pMain->m_clParser.m_iPageNumAlign == LEFTALIGN) //left
	{
		trect.left = rect.left; 
		trect.top += (m_pMain->m_clParser.m_iBotRow[0] * m_iHdrLineHeight);
		
	}

	if(m_pMain->m_clParser.m_iPageNumAlign == CENTERALIGN) 
	{
		const int ibotrow = max(m_pMain->m_clParser.m_iBotRow[0], m_pMain->m_clParser.m_iBotRow[1]);
		trect.top += (ibotrow * m_iHdrLineHeight);
		trect.left = (rect.Width() / 2 ) - tsize.cx/2 ; 
	}

	if(m_pMain->m_clParser.m_iPageNumAlign == RIGHTALIGN) 
	{
		trect.left = rect.right - tsize.cx - gap; 
		trect.top += (m_pMain->m_clParser.m_iBotRow[1] * m_iHdrLineHeight);
	}

	trect.bottom = trect.top + m_iHdrLineHeight;

	if(m_pMain->m_clParser.m_iPageNumType > -1)
	{
		spage = "";
		if(!m_pMain->m_clParser.m_strPageNumToken[0].IsEmpty()) 
			spage = m_pMain->m_clParser.m_strPageNumToken[0];

		CString stmp;
		if(m_pMain->m_clParser.m_iPageNumType == 0) //page/tot
		{
			stmp.Format("%d %s %d",  ipage, m_pMain->m_clParser.m_strPageNumToken[1], itotpage);
			spage += stmp;
		}
		else 
		{
			stmp.Format("%d",  ipage);
			spage += stmp;
		}

		pDC->DrawText(spage, trect, uiFormat);
	}

	pDC->SelectObject(poldFont);
	DeleteObject(poldFont);
}

CSize CWxReport::GetBitmapSize(HBITMAP hBitmap)
{
	if (!hBitmap) return CSize(0, 0);

	BITMAP BM;
	GetObject(hBitmap, sizeof(BITMAP), &BM);
	return CSize((int)BM.bmWidth, (int)BM.bmHeight);
}


HANDLE CWxReport::DDBToDIB(const CBitmap& bitmap, DWORD dwCompression, CPalette* pPal)
{
	BITMAP bm{};
	BITMAPINFOHEADER bi{};
    LPBITMAPINFOHEADER lpbi{};
	DWORD dwLen{};
	HANDLE hDIB{};
	HANDLE handle{};
	HDC hDC{};
	HPALETTE hPal{};

    ASSERT( bitmap.GetSafeHandle() );
    if ( dwCompression == BI_BITFIELDS )
        return NULL;

    hPal = (HPALETTE) pPal->GetSafeHandle();
    if (hPal==NULL)
        hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

    bitmap.GetObject(sizeof(bm),(LPSTR)&bm);
    bi.biSize               = sizeof(BITMAPINFOHEADER);
    bi.biWidth              = bm.bmWidth;
    bi.biHeight             = bm.bmHeight;
    bi.biPlanes             = 1;
    bi.biBitCount           = bm.bmPlanes * bm.bmBitsPixel;
    bi.biCompression        = dwCompression;
    bi.biSizeImage          = 0;
    bi.biXPelsPerMeter      = 0;
    bi.biYPelsPerMeter      = 0;
    bi.biClrUsed            = 0;
    bi.biClrImportant       = 0;

    int nColors = (1 << bi.biBitCount);
    if ( nColors > 256 ) 
        nColors = 0;
    dwLen = bi.biSize + nColors * sizeof(RGBQUAD);
    hDC = ::GetDC(NULL);
    hPal = SelectPalette(hDC,hPal,FALSE);
    RealizePalette(hDC);

    hDIB = GlobalAlloc(GMEM_FIXED,dwLen);
    if (!hDIB)
    {
        SelectPalette(hDC,hPal,FALSE);
        ::ReleaseDC(NULL,hDC);
        return NULL;
    }

    lpbi = (LPBITMAPINFOHEADER)hDIB;
    *lpbi = bi;

    GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 
                  0L, (DWORD)bi.biHeight,
                  (LPBYTE)NULL, (LPBITMAPINFO)lpbi, 
                  (DWORD)DIB_RGB_COLORS);
    bi = *lpbi;

    if (bi.biSizeImage == 0)
    {
        bi.biSizeImage = 
          ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8) 
                                             * bi.biHeight;
        if (dwCompression != BI_RGB)
            bi.biSizeImage = (bi.biSizeImage * 3) / 2;
    }

    dwLen += bi.biSizeImage;
    if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
        hDIB = handle;
    else
    {
        GlobalFree(hDIB);
        SelectPalette(hDC,hPal,FALSE);
        ::ReleaseDC(NULL,hDC);
        return NULL;
    }

    lpbi = (LPBITMAPINFOHEADER)hDIB;

    const BOOL bGotBits = GetDIBits( hDC, 
        (HBITMAP)bitmap.GetSafeHandle(),
        0L,                 // 
        (DWORD)bi.biHeight, // 
        (LPBYTE)lpbi        // 
        + (bi.biSize + nColors * sizeof(RGBQUAD)),
        (LPBITMAPINFO)lpbi, // 
        (DWORD)DIB_RGB_COLORS); // 

    if( !bGotBits )
    {
        GlobalFree(hDIB);

        SelectPalette(hDC,hPal,FALSE);
        ::ReleaseDC(NULL,hDC);
        return NULL;
    }

    SelectPalette(hDC,hPal,FALSE);
    ::ReleaseDC(NULL,hDC);
    return hDIB;
}

void CWxReport::DrawBitmap(CBitmap* hbitmap, CDC* pDC, int ileft, int itop)
{
	CPalette pal;
    HANDLE hDIB=DDBToDIB(*hbitmap, BI_RGB, &pal );


    LPBITMAPINFOHEADER lpbi;
    lpbi = (LPBITMAPINFOHEADER)hDIB;
    int nColors = lpbi->biClrUsed ? 
           lpbi->biClrUsed : 1 << lpbi->biBitCount;

    BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB;
    LPVOID lpDIBBits=NULL;
    if( bmInfo.bmiHeader.biBitCount > 8 )
        lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + 
            bmInfo.bmiHeader.biClrUsed) + 
            ((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 
                                                        3 : 0));
    else
        lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	const int imgw = bmInfo.bmiHeader.biWidth;
	const int imgh = bmInfo.bmiHeader.biHeight;

    HDC hDC=pDC->GetSafeHdc();

    StretchDIBits(
		hDC,           // hDC
		ileft ,                           // DestX
		itop,                           // DestY

		(int)(imgw * m_lscalex) ,            // nDestWidth
		(int)(imgh * m_lscaley) ,                      // nDestHeight

		0,               // SrcX
		0,// SrcY

		imgw,            // wSrcWidth
		imgh,                      // wSrcHeight

		lpDIBBits,                   // lpBits
		&bmInfo,                     // lpBitsInfo
		DIB_RGB_COLORS,              // wUsage
		SRCCOPY	);                    // dwROP
}

void CWxReport::DrawBitmap(CString sImgpath, CDC* pDC, CRect rect)
{
	CBitmap* hbitmap = m_pMain->getBitmap(sImgpath);
	if(!hbitmap) return;

	const CSize size = GetBitmapSize(hbitmap->operator HBITMAP());

	const int imgw = int( (double)size.cx * m_lscalex);
	const int imgh = int( (double)size.cy * m_lscaley );
	const int iw = rect.left + imgw;
	const int ih = rect.top + imgh;

	if(rect.right < iw)
		rect.left = rect.right - imgw;

	DrawBitmap(hbitmap, pDC, rect.left, rect.top  );
}


void CWxReport::DrawBackGround(CDC* pDC, CRect rect, CBitmap* hbitmap)
{
	if(hbitmap == NULL) return;
	const CSize size = GetBitmapSize(hbitmap->operator HBITMAP());
	const int imgw = int( (double) size.cx * m_lscalex );
	const int imgh =  int( (double)size.cy * m_lscaley );

	int idivh = rect.Height() / 2 - imgh/2; 
	if(idivh < 0) idivh = 0;

	const int itop = rect.top + idivh;
	int idivw = rect.Width() /2 - imgw/2; 
	if(idivw < 0) idivw = 0;
	const int ileft = rect.left + idivw;

	DrawBitmap(hbitmap, pDC, ileft, itop);
}

int CWxReport::DrawScreen(CDC* pDC, CRect rect, int idrawpage)
{
	m_pMain->SetPrinterStatus("프린터 정보를 읽고 있습니다");
	m_lscalex = 0.9;
	m_lscaley = 0.9;
	const int iscrnx = rect.right ;
	const int iscrny = rect.bottom;

	m_iLineHeight = int( (double)LINEHEIGHT * m_lscaley) ;
	m_iHdrLineHeight = int( (double)HDRLINEHEIGHT * m_lscaley);

	LOGFONT lf = GetLogFont(10, 0);
	m_pHdrFont.CreateFontIndirect(&lf);

	lf = GetLogFont(12, 0);
	m_pFont.CreateFontIndirect(&lf);

	lf = GetLogFont(10, 0);
	m_pMultiLineFont.CreateFontIndirect(&lf);

	
	lf = GetLogFont(22, 1);
	m_pTitleFont.CreateFontIndirect(&lf);


	m_iSpaceHeight = int( (double)m_pMain->m_clParser.m_iSpaceHeight * m_lscaley) ;

	int inext = 0;
	int ipage = 0;
	m_iBottomMargin = int( (double)m_pMain->m_clParser.m_iMargin[3] * m_lscaley );
	const CRect drect = CRect( rect.left+5,
						rect.top + 5, 
						iscrnx-10, 
						iscrny-m_iBottomMargin);

	const int itotpage = GetTotPage(drect);


	CFont* poldFont = pDC->SelectObject(&m_pFont); 
	const int igap = int(3.0 * m_lscaley);
	const CRect botrect = CRect(drect.left, iscrny - (m_iBottomMargin - igap), drect.right, drect.bottom);
//	botrect.top = iscrny - (m_iBottomMargin-igap);

	int icontentidx = 0;
	CString sMsg = "";
	sMsg.Format("1 / %d ", itotpage); 
	m_pMain->SetPrinterPage(sMsg);

	for(int i=0; i<itotpage;i++)
	{
		SetDrawRect(pDC,  rect);
		sMsg.Format("%d / %d ", i+1, itotpage); 
		m_pMain->SetPrinterPage(sMsg);
		sMsg += " 페이지 인쇄중...";
		m_pMain->SetPrinterStatus(sMsg);
		
		pDC->SetBkMode(TRANSPARENT); 
		DrawBackGround(pDC, drect, m_pMain->m_clParser.m_hBackground);
		inext = DrawPage(icontentidx, ipage++, pDC, drect, inext);
		DrawBottom( pDC, botrect, itotpage, ipage);
		if(idrawpage == i) break;
	}

	pDC->SelectObject(poldFont); 
	::DeleteObject(poldFont);

	
	m_pMain->SetProgressPos(-1);
	return itotpage;
}

void CWxReport::Print()
{
	
	m_pMain->SetPrinterStatus("프린터 정보를 읽고 있습니다");
	PRINTDLG m_pd;
	memset(&m_pd, 0x00, sizeof(m_pd));
	m_pd.lStructSize = (DWORD)sizeof(PRINTDLG);
	m_pd.Flags        = PD_ALLPAGES | PD_NOPAGENUMS |PD_RETURNDEFAULT | PD_RETURNDC;

	//프린트에 대한 정보를 채워주는 부분. 화면에 나타나지 않음.
	if (!PrintDlg(&m_pd)) 
	{
		m_pMain->SetPrinterStatus("프린터를 찾을수 없습니다.");
		return ;
	}

	if( !m_pd.hDevMode || !m_pd.hDevNames)
	{
		m_pMain->SetPrinterStatus("프린터를 찾을수 없습니다.");
		return ;
	}

	//bool bSuccess = false;
	DEVMODE *pDevMode=NULL;
	pDevMode = (DEVMODE *)GlobalLock(m_pd.hDevMode);

	if( !pDevMode )
	{
		m_pMain->SetPrinterStatus("프린터를 찾을수 없습니다.");
		return ;
	}

	if( lstrlen((char*)pDevMode->dmDeviceName) == 0 )
	{
		GlobalUnlock(m_pd.hDevMode);
		m_pMain->SetPrinterStatus("프린터를 찾을수 없습니다.");
		return ;
	}

	//	pDevMode->dmFields |= DM_COPIES | DM_COLLATE;
	pDevMode->dmCopies = 1;
	pDevMode->dmOrientation = m_pMain->m_clParser.m_iPaperMode?DMORIENT_PORTRAIT:DMORIENT_LANDSCAPE;//DMORIENT_LANDSCAPE;//m_bLandScope?DMORIENT_LANDSCAPE:DMORIENT_PORTRAIT;
	pDevMode->dmFields |= DM_ORIENTATION;

	ResetDC(m_pd.hDC, pDevMode);//
	GlobalUnlock(m_pd.hDevMode);

	CDC* pDC = CDC::FromHandle(m_pd.hDC);
	if( !pDC )	return ;

	const int ioffx = GetDeviceCaps(pDC->m_hDC, PHYSICALOFFSETX);//100
	const int ioffy = GetDeviceCaps(pDC->m_hDC, PHYSICALOFFSETY);//100

//m_lscaley, m_lscalex
	m_lscaley = pDC->GetDeviceCaps(LOGPIXELSY) / 100;
	m_lscalex = pDC->GetDeviceCaps(LOGPIXELSX) / 100;

	m_iLineHeight = int( (double)LINEHEIGHT * m_lscaley) ;
	m_iSpaceHeight = int( (double)m_pMain->m_clParser.m_iSpaceHeight * m_lscaley) ;
	m_iHdrLineHeight = int( (double)HDRLINEHEIGHT * m_lscaley);

	LOGFONT lf = GetLogFont(10, 0);
	m_pHdrFont.CreateFontIndirect(&lf);

	lf = GetLogFont(12, 0);
	m_pFont.CreateFontIndirect(&lf);

	lf = GetLogFont(10, 0);
	m_pMultiLineFont.CreateFontIndirect(&lf);

	
	lf = GetLogFont(28, 1);
	m_pTitleFont.CreateFontIndirect(&lf);

	const int ipaperx = pDC->GetDeviceCaps(HORZRES);//용지사이즈 
	const int ipapery = pDC->GetDeviceCaps(VERTRES);// 


    DOCINFO di;
    ZeroMemory(&di,sizeof(DOCINFO));
    di.cbSize = sizeof(DOCINFO);
    di.lpszDocName = m_pMain->GetDocName();
	
			
	if( pDC->StartDoc(&di) < 0 )
	{
		m_pMain->SetPrinterStatus("인쇄를 시작할 수 없습니다.");
		return ;
	}

	CFont* poldFont = pDC->SelectObject(&m_pFont); 
	
	int inext = 0;
	int ipage = 0;
	m_iBottomMargin = int( (double)m_pMain->m_clParser.m_iMargin[3] * m_lscaley );

	const CRect drect = CRect( int( double(m_pMain->m_clParser.m_iMargin[0]) * m_lscalex),
						int( (double)m_pMain->m_clParser.m_iMargin[1] * m_lscaley), 
						int( (double)ipaperx-m_pMain->m_clParser.m_iMargin[2] * m_lscalex), 
						ipapery-m_iBottomMargin);

	const int itotpage = GetTotPage(drect);

	const int igap = int(3.0 * m_lscaley);
	//CRect botrect = drect;
	//	botrect.top = ipapery - (m_iBottomMargin-igap);
	const CRect botrect = CRect(drect.left, ipapery - (m_iBottomMargin - igap), drect.right, drect.bottom);

	int icontentidx = 0;
	CString sMsg = "";
	sMsg.Format("1 / %d ", itotpage); 
	m_pMain->SetPrinterPage(sMsg);

	for(int i=0; i<itotpage;i++)
	{
		sMsg.Format("%d / %d ", i+1, itotpage); 
		m_pMain->SetPrinterPage(sMsg);
		sMsg += " 페이지 인쇄중...";
		m_pMain->SetPrinterStatus(sMsg);
		
		pDC->StartPage();
		pDC->SetBkMode(TRANSPARENT); 
		DrawBackGround(pDC, drect, m_pMain->m_clParser.m_hBackground);
		inext = DrawPage(icontentidx, ipage++, pDC, drect, inext);
		DrawBottom( pDC, botrect, itotpage, ipage);
		pDC->EndPage();
	}

	pDC->EndDoc();
	pDC->SelectObject(poldFont); 
	::DeleteObject(poldFont);

	
	m_pMain->SetProgressPos(-1);
	m_pMain->SetPrinterStatus("인쇄가 완료되었습니다..");
	m_pMain->ending();
}

void CWxReport::DrawRectangle(CDC* pDC, CRect rect)
{
	pDC->MoveTo( rect.left, rect.top);  
	pDC->LineTo( rect.left, rect.bottom);  

	pDC->MoveTo( rect.left, rect.top);  
	pDC->LineTo( rect.right, rect.top);  

	pDC->MoveTo( rect.right, rect.top);  
	pDC->LineTo( rect.right, rect.bottom);  

	pDC->MoveTo( rect.left, rect.bottom);  
	pDC->LineTo( rect.right, rect.bottom);  

}