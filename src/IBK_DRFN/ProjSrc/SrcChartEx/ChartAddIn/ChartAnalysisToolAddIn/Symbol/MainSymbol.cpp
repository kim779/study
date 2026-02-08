// MainSymbol.cpp: implementation of the CMainSymbol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Symbol.h"
#include "MainSymbol.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "MainSymbolImplementation.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMainSymbol::CMainSymbol():
	m_Implementation(NULL)
{
	m_Implementation = new CMainSymbolImplementation();
}

CMainSymbol::~CMainSymbol()
{
	delete m_Implementation;
}

// ChartObject.dll 주소
void CMainSymbol::SetMainBlock_Object(IChartManager* pMainBlock,IChartOCX *p_pIChartOCX)
{
	m_Implementation->SetMainBlock_Object(pMainBlock,p_pIChartOCX);
}

void CMainSymbol::CreateSymbolDlg()
{
	m_Implementation->CreateSymbolDlg();
}

BOOL CMainSymbol::IsExistBuletDlg()
{
	return m_Implementation->IsExistBuletDlg();
}

void CMainSymbol::DeleteSymbolDlg()
{
	m_Implementation->DeleteSymbolDlg();
}

void CMainSymbol::SetCurrentSymbolTool(const CSymbol::SYMBOLTOOL symbolType)
{
	m_Implementation->SetCurrentSymbolTool(symbolType);
}

void CMainSymbol::SetShapeDrawingType(const COLORREF& crLine,const int& nStyle,const int& nWeight,const bool& bFill)
{
	m_Implementation->SetShapeDrawingType(crLine,nStyle,nWeight,bFill);
}

bool CMainSymbol::OnLButtonDown(CDC* pDC, const CPoint &point)
{
	return m_Implementation->OnLButtonDown(pDC, point);
}

bool CMainSymbol::OnLButtonUp(CDC* pDC, const CPoint &point)
{
	return m_Implementation->OnLButtonUp(pDC, point);
}

bool CMainSymbol::OnMouseMove(CDC* pDC,const UINT &nFlags, const CPoint &point)
{
	return m_Implementation->OnMouseMove(pDC,nFlags, point);
}

bool CMainSymbol::OnRButtonUp(const CPoint &point)
{
	return m_Implementation->OnRButtonUp(point);
}

bool CMainSymbol::OnLButtonDbClk(CDC* pDC, const CPoint &point)
{
	return m_Implementation->OnLButtonDbClk(pDC, point);
}

void CMainSymbol::OnDraw(CDC* pDC)
{
	m_Implementation->OnDraw(pDC);
}	

// block 추가시 elementSet을 추가하기.
bool CMainSymbol::InsertBlockIndex(const CBlockIndex& insertBlockIndex)
{
	return m_Implementation->InsertBlockIndex(insertBlockIndex);	
}

void CMainSymbol::SetModifiedBlockIndexs(const CBlockIndex &sourceBlockIndex, const CBlockIndex &TargetBlockIndex)
{
	m_Implementation->SetModifiedBlockIndexs(sourceBlockIndex, TargetBlockIndex);
}

// element 삭제
void CMainSymbol::DeleteCurrentSymbols()
{
	m_Implementation->DeleteCurrentSymbols(); 
}

// element 삭제, 맨마지막에 생성된 객체를 삭제
void CMainSymbol::DeleteLastSymbol()
{
	m_Implementation->DeleteLastSymbol(); 
}

bool CMainSymbol::DeleteSymbolPos(const long lAddress)
{
	return m_Implementation->DeleteSymbolPos(lAddress); 
}

bool CMainSymbol::UndoElement(const long lAddress)
{
	return m_Implementation->UndoElement(lAddress);
}

bool CMainSymbol::RedoElement(const long lAddress)
{
	return m_Implementation->RedoElement(lAddress);
}

void CMainSymbol::DeleteAllSymbols()
{
	m_Implementation->DeleteAllSymbols(); 
}

void CMainSymbol::DeleteAllSymbolsInBlock(const CBlockIndex& blockIndex)
{
	m_Implementation->DeleteAllSymbolsInBlock(blockIndex); 
}

bool CMainSymbol::OnRButtonMenu(const CMenuType::ELEMENTMENU menuType, const WPARAM wParam)
{
	return m_Implementation->OnRButtonMenu(menuType, wParam); 
}

void CMainSymbol::OnSetColor(const COLORREF& color)
{
	m_Implementation->OnSetColor(color); 
}

CString CMainSymbol::GetSavedSymbolsData()
{
	return m_Implementation->GetSavedSymbolsData(); 
}

void CMainSymbol::BuildSymbolsFromString(const CString& Data)
{
	m_Implementation->BuildSymbolsFromString(Data); 
}

void CMainSymbol::SetFilePath(const CString& path)
{
	m_Implementation->SetFilePath(path); 
}

void CMainSymbol::InitializeBlock(CDC *pDC, CWnd *pParentWnd)
{
	m_Implementation->InitializeBlock(pDC, pParentWnd); 
}

CSymbol::SYMBOLTOOL CMainSymbol::GetToolType()
{
	return m_Implementation->GetToolType(); 	
}

CSymbol::SYMBOLTOOL CMainSymbol::GetSelectedToolType()
{
	return m_Implementation->GetSelectedToolType(); 	
}

void CMainSymbol::SetElementCopy()
{
	m_Implementation->SetElementCopy();
}

CRect CMainSymbol::GetDrawingTotalRegion(CPoint point)
{
	return m_Implementation->GetDrawingTotalRegion(point);
}

void CMainSymbol::DrawSymbolTool(CDC *pDC)
{
	m_Implementation->DrawSymbolTool(pDC);
}

void CMainSymbol::ChangeAllSymbolsColor(const COLORREF& crLine,const COLORREF& crText)
{
	m_Implementation->ChangeAllSymbolsColor(crLine,crText);
}

// 20081007 JS.Kim	사용자시간대 선택
void CMainSymbol::RecalcSymbolXPoint(const char* szName, int nTimeDiff)
{
	m_Implementation->RecalcSymbolXPoint(szName, nTimeDiff); 
}

// 2011.02.07 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
void CMainSymbol::SetXScaleDrawType(const CScaleBaseData::HORZSCALEDRAWERTYPE eScaleType)
{
	m_Implementation->SetXScaleDrawType(eScaleType); 
}
// 2011.02.07 by SYS <<
