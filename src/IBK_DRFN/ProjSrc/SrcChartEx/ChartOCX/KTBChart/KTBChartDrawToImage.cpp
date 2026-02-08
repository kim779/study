// KTBChartDrawToImage.cpp: implementation of the DrawToImage Interface
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/DLL_Load/AddInManager.h"		// for CAddInManager
#include "../Include_Chart/DLL_Load/IMetaTable.h"		// for ML_LANGUAGE_RES
#include "KTBChartCtl.h"								// for CKTBChartCtrl
#include "Color.h"										// for CGraphRegionColor
#include "MainBlock.h"									// for CMainBlock
#include "XScaleManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*-------------------------------------------------------------------------------
 - Function    :  OnDrawForPrint
 - Created at  :  2003-10-23   14:32
 - Parameters  :  memoryDC     - 메모리DC
 -                bufferBitmap - 사용하지 않음.
 -                rcSource     - 저장할 이미지의 크기
 -                bScreenColor - ???
 - Return Value:  None
 - Description :  (분석) 차트를 memoryDC에 그린다.
 -------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnDrawForPrint(CDC& memoryDC,
								   CBitmap& bufferBitmap,
								   CRect rcSource,
								   BOOL bScreenColor)
{
	CRect rect(rcSource);
	// (2003.12.23, 배승원) Resource Manager 적용을 개선한다.
	CFont* pOldFont = (CFont*) memoryDC.SelectObject( m_pRscFont);

	OnDrawStockItemCode(memoryDC, rect); // 차트에서의 종목명과 종목이름을 memoryDC에 그린다.

	if(bScreenColor)
		OnDrawUsingScreencolor(memoryDC, bufferBitmap, rect);	// 차트를 memoryDC에 그림
	else
		OnDrawUsingOptimizingcolor(memoryDC, bufferBitmap, rect);	// 간단하게 표현된 색으로 차트를 그림
	
	memoryDC.SelectObject(pOldFont);
}


/*-------------------------------------------------------------------------------
 - Function    :  OnDrawStockItemCode
 - Created at  :  2003-10-23   14:43
 - Analysis    :  종목명과 종목코드를 그린다.
 -------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnDrawStockItemCode(CDC& memoryDC,
										CRect& rect)
{
	int nWritingItemInfoSpace = 0;
	if(!m_strCode.IsEmpty() && !m_strItemName.IsEmpty()) 
		nWritingItemInfoSpace = 20;

	//종목코드, 종목명 쓸 영역
	m_pBackgroundDraw.DrawBackground(&memoryDC,
					CRect(rect.left, rect.top, rect.right, rect.top + 20),	// 그릴 background 영역 
					1,			// border 굵기
					RGB(255,255,255),		// background 색
					RGB(255,255,255),		// block 색
					m_nBorderType);			// border 타입

	if(nWritingItemInfoSpace != 0)
	{
		ML_SET_LANGUAGE_RES();
		CString strFormat;
		strFormat.LoadString( IDS_FORMAT_ITEM_CODE_AND_NAME);
		CString str;
		str.Format( strFormat, m_strCode, m_strItemName);
		memoryDC.SetBkMode(TRANSPARENT);

		SIZE size;
		::GetTextExtentPoint32(memoryDC.m_hDC, "0", 1, &size);

		int ntop = 5;
		int nleft = (rect.Width() - (str.GetLength() * size.cx)) / 2;
		int nright = nleft + (str.GetLength() * size.cx);
		int nbottom = ntop + size.cy;
		
		memoryDC.MoveTo(nleft - 2, ntop - 3);
		memoryDC.LineTo(nleft - 2, nbottom + 1);
		memoryDC.LineTo(nright + 2, nbottom + 1);
		memoryDC.LineTo(nright + 2, ntop - 3);
		memoryDC.LineTo(nleft - 2, ntop - 3); 

		memoryDC.TextOut(nleft, ntop, str);
	}

	rect.top += nWritingItemInfoSpace;	// 종목코드와 종목명을 그린 영역은 이제 제외시키자!
}


/*-------------------------------------------------------------------------------
 - Function    :  OnDrawUsingOptimizingcolor
 - Created at  :  2003-10-23   14:39
 - Analysis    :  차트를 지정된 색(HTS에 표현된 그대로의 색이 아닌)으로 MemoryDC에 그린다.
 -------------------------------------------------------------------------------*/
void CKTBChartCtrl::OnDrawUsingOptimizingcolor(CDC& memoryDC,
											   CBitmap& bufferBitmap,
											   CRect rect)
{
	m_pMainBlock->SetAllBlockRegion(rect);
	m_pMainBlock->SetBackgroundColor(RGB(255,255,255));
	CGraphRegionColor BlockColor(RGB(255,255,255), RGB(255,255,255), m_clrBlockBorder);
	m_pMainBlock->SetGraphRegionColor(BlockColor);
	m_pMainBlock->SetGraphBackgroundGap(0, 0);

	DrawChart( memoryDC, memoryDC, rect);

	//다시 되돌리기..
	m_pMainBlock->SetBackgroundColor(m_clrBackground);
	CGraphRegionColor BlockColor2(m_clrBlock, m_clrLattice, m_clrBlockBorder);
	m_pMainBlock->SetGraphRegionColor(BlockColor2);
	m_pMainBlock->SetGraphBackgroundGap(m_nHorzGap, m_nVertGap);
	// (2004.10.19, 배승원) Block Region 복원은 이용 이후 바로 적용한다.
	m_pMainBlock->SetAllBlockRegion(GetMainBlockRegion());
}

//화면색
void CKTBChartCtrl::OnDrawUsingScreencolor(CDC& memoryDC,
										   CBitmap& bufferBitmap, // 사용하지 않음.
									       CRect rect)
{
	// 차트를 메모리DC에 그린다.
	m_pMainBlock->SetAllBlockRegion(rect);

	DrawChart( memoryDC, memoryDC, rect);

	// (2004.10.19, 배승원) Block Region 복원은 이용 이후 바로 적용한다.
	m_pMainBlock->SetAllBlockRegion(GetMainBlockRegion());
}
