// DrawingSymbolRow.cpp: implementation of the CDrawingSymbolRow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "DrawingSymbolRow.h"

#include <Math.h>				// for HUGE_VAL

#include "../Include_Analysis/CoordinateCalculatorData.h"

//기호 객체
#include "DrawingSymbol.h"
#include "DrawingSymbolFactory.h"
//좌표 관리
#include "CoordinateCalculator.h"
//Block 영역 관리
#include "Region_AllBlock.h"

#include "Conversion.h"
#include "BlockBaseData.h"
//화일 저장하기 위한 String만들기
#include "ElementSaverBuilder.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDrawingSymbolRow::CDrawingSymbolRow()
{
	m_Symbol_List.RemoveAll();
}

CDrawingSymbolRow::~CDrawingSymbolRow()
{
	DeleteAllDrawingSymbol();
}

CDrawingSymbol* CDrawingSymbolRow::Add(CDrawingSymbol* pSymbol)
{
	m_Symbol_List.AddTail(pSymbol);
	return pSymbol;
}

CDrawingSymbol* CDrawingSymbolRow::Find(const CPoint &point, const int nColumn, const int nRow, IChartManager* pMainBlock)
{
	CRegion_AllBlock drawingRegions;
	drawingRegions.SetDrawingRegions(pMainBlock);

	CCoordinateCalculatorData calData( pMainBlock);
	calData.GetCoordinateCalculatorData( point);

	POSITION pos = m_Symbol_List.GetHeadPosition();
	while(pos) {
		CDrawingSymbol* pSymbol = m_Symbol_List.GetNext(pos);
		if(pSymbol == NULL)
			return NULL;

		if(pSymbol->IsSelected(GetGraphRegion(nColumn, nRow, drawingRegions), point, calData))
			return pSymbol; 
	}
	return NULL;
}

void CDrawingSymbolRow::OnDraw(CDC* pDC, IChartManager* pMainBlock, CDrawingSymbol *pSelected, const int nColumn, const int nRow, const CRegion_AllBlock& drawingRegions)
{
	CCoordinateCalculatorData calData( pMainBlock);
	calData.GetCoordinateCalculatorData( CBlockIndex(nRow, nColumn));

	// (2007/5/10 - Seung-Won, Bae) Check No Y Scale Min/Max.
	if( -HUGE_VAL == calData.GetDisplayDataMax_ScaleType() || -HUGE_VAL == calData.GetDisplayDataMin_ScaleType()) return;

	CRect rcRegion = GetGraphRegion(nColumn, nRow, drawingRegions);
	POSITION pos = m_Symbol_List.GetHeadPosition();
	while(pos) {
		CDrawingSymbol* pSymbol = m_Symbol_List.GetNext(pos);
		if(pSymbol == NULL)
			return;
		pSymbol->Draw(pDC, rcRegion, calData);
		if(pSymbol == pSelected)
			pSymbol->DrawSelectedMark(pDC, rcRegion, calData);
	}
}

long CDrawingSymbolRow::CopyElement(CDrawingSymbol *pSelected)
{
	long lAddress = 0;
	POSITION pos = m_Symbol_List.GetHeadPosition();
	while(pos) 
	{
		CDrawingSymbol* pSymbol = m_Symbol_List.GetNext(pos);
		if(pSymbol == NULL)
			return lAddress;

		if(pSymbol == pSelected)
		{
			CDrawingSymbol *pNewDrawingSymbol = CDrawingSymbolFactory::Copy(pSelected);
			lAddress = (long)pNewDrawingSymbol;
			m_Symbol_List.AddTail(pNewDrawingSymbol);			
			break;
		}
	}
	return lAddress;
}

long CDrawingSymbolRow::MoveElement(CDrawingSymbol *pSelected)
{
	long lAddress = 0;
	POSITION pos = m_Symbol_List.GetHeadPosition();
	while(pos) 
	{
		CDrawingSymbol* pSymbol = m_Symbol_List.GetNext(pos);
		if(pSymbol == NULL)
			return lAddress;

		if(pSymbol == pSelected)
		{
			lAddress = (long)pSelected;			
			break;
		}
	}

	if(lAddress == 0)
		m_Symbol_List.AddTail(pSelected);
	return lAddress;
}

CDrawingSymbol* CDrawingSymbolRow::GetTail()
{
	return m_Symbol_List.GetTail();
}

bool CDrawingSymbolRow::DeleteCurrentDrawingSymbol(const CDrawingSymbol* pDrawingSymbol)
{
	if(m_Symbol_List.GetCount() <= 0)
		return false;

	POSITION pos = m_Symbol_List.GetHeadPosition();
	while(pos) {
		POSITION tmp = pos;
		CDrawingSymbol* pSymbol = m_Symbol_List.GetNext(pos);
		if(pSymbol == pDrawingSymbol) {
			CDrawingSymbolFactory::Remove(pSymbol);
			//delete pSymbol;
			m_Symbol_List.RemoveAt(tmp);
			return true;
		}
	}
	return false;
}

bool CDrawingSymbolRow::DeleteCurrentDrawingSymbol(const long lAddressSymbol,BOOL bListOnly)
{
	if(m_Symbol_List.GetCount() <= 0)
		return false;

	long lTempAddress = 0;
	POSITION pos = m_Symbol_List.GetHeadPosition();
	while(pos) 
	{
		POSITION tmp = pos;
		CDrawingSymbol* pSymbol = m_Symbol_List.GetNext(pos);
		lTempAddress = (long)pSymbol;
		if(lTempAddress == lAddressSymbol) 
		{
			if(bListOnly == FALSE)
				CDrawingSymbolFactory::Remove(pSymbol);
			//delete pSymbol;
			m_Symbol_List.RemoveAt(tmp);
			return true;
		}
	}
	return false;
}

int CDrawingSymbolRow::GetCount()
{
	return m_Symbol_List.GetCount();
}

CRect CDrawingSymbolRow::GetGraphRegion(const int nColumn, const int nRow, const CRegion_AllBlock& drawingRegions)
{
	return drawingRegions.GetBlockRegion(nColumn, nRow);
}

void CDrawingSymbolRow::DeleteAllDrawingSymbol()
{
	if(m_Symbol_List.GetCount() <= 0)
		return;

	POSITION pos = m_Symbol_List.GetHeadPosition();
	while(pos) {
		CDrawingSymbol* pSymbol = m_Symbol_List.GetNext(pos);
		if(pSymbol != NULL)
			CDrawingSymbolFactory::Remove(pSymbol);
			//delete pSymbol;
	}
	m_Symbol_List.RemoveAll();
}

CString CDrawingSymbolRow::GetSavedSymbolsData(const COrder &order)
{
	CElementSaverBuilder save;
	return save.GetSavedSymbolsData(order, &m_Symbol_List);
}

CDrawingSymbol* CDrawingSymbolRow::GetAt(const int nIndex)
{
	if(nIndex < 0 )
		return NULL;

	return m_Symbol_List.GetAt(m_Symbol_List.FindIndex(nIndex));
}

void CDrawingSymbolRow::GetElementList(CList<CDrawingSymbol*, CDrawingSymbol*>& elementList)
{
	elementList.RemoveAll();

	for(int nSymbol = 0;  nSymbol < m_Symbol_List.GetCount();  nSymbol++) 
		elementList.AddTail(m_Symbol_List.GetAt(m_Symbol_List.FindIndex(nSymbol)));
}

void CDrawingSymbolRow::ChangeAllSymbolsColor(const COLORREF& penColor, const COLORREF& textColor)
{
	POSITION pos = m_Symbol_List.GetHeadPosition();
	while(pos) {
		CDrawingSymbol* pSymbol = m_Symbol_List.GetNext(pos);
		if(pSymbol != NULL)
		{
			pSymbol->SetColor(penColor);
			pSymbol->SetTextColor(textColor);
		}
	}
}

bool CDrawingSymbolRow::IsInSameSymbol(const CDrawingSymbol* pSymbol) const
{
	POSITION pos = m_Symbol_List.GetHeadPosition();
	while(pos)
	{
		if(pSymbol == m_Symbol_List.GetNext(pos))
			return true;
	}
	return false;
}