// Properties_Manager.cpp: implementation of the CProperties_Manager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symbol.h"
#include "Properties_Manager.h"

#include "DrawingSymbol.h"

//설정box
#include "Properties.h"
#include "Properties_Figure.h"
#include "Properties_text.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// CProperties_Manager::CProperties_Manager(CDrawingSymbol* pDrawingSymbol,
// 										 CPropertiesData_text* pProperties,
// 										 IChartManager* pMainBlock,
// 										 const CString& strUserPath,
// 										 const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
// 										 HWND p_hOcxWnd)
CProperties_Manager::CProperties_Manager(CDrawingSymbol* pDrawingSymbol,
										 CPropertiesData_text* pProperties,
										 IChartManager* pMainBlock,
										 const CString& strUserPath,
										 const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
										 HWND p_hOcxWnd,
										const CBlockIndex& blockIndex,
										const CString & strFormatX)
// 2011.02.08 by SYS <<
{
	m_pDrawingSymbol = pDrawingSymbol;
	m_pProperties = pProperties;
	m_nDataPeriod = nDataPeriod;
	m_strUserPath = strUserPath;
	
	m_pMainBlock = pMainBlock;
	if( m_pMainBlock) m_pMainBlock->AddRef();

	m_ModifyProperties.SetFillState(pProperties->GetFillState());
	m_ModifyProperties.SetColor(pProperties->GetColor());
	m_ModifyProperties.SetLogFont(pProperties->GetLogFont());
	m_ModifyProperties.SetSizeType(pProperties->GetSizeType());

	m_hOcxWnd = p_hOcxWnd;

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	if(blockIndex.IsAllValueBiggerThanZero())
	{
		m_blockIndex.SetRow(blockIndex.GetRow());
		m_blockIndex.SetColumn(blockIndex.GetColumn());
	}
	m_strFormatX	= strFormatX;
	// 2011.02.08 by SYS <<
}

CProperties_Manager::~CProperties_Manager()
{
	if( m_pMainBlock) m_pMainBlock->Release();
}

bool CProperties_Manager::PopUpPropertiesBox()
{
	ML_SET_LANGUAGE_RES();
	
	//선택된 객체의 색깔과 폰트를 얻어서 띄운다.
	if(m_pDrawingSymbol == NULL)
		return false;

	m_ModifyProperties.SetFillState(m_pDrawingSymbol->GetFillState());
	m_ModifyProperties.SetColor(m_pDrawingSymbol->GetColor());
	m_ModifyProperties.SetLogFont(m_pDrawingSymbol->GetLogFont());
	//sy 2005.04.06
	m_ModifyProperties.SetTextColor(m_pDrawingSymbol->GetTextColor());
	m_ModifyProperties.SetSizeType(m_pDrawingSymbol->GetSizeType());
	m_ModifyProperties.SetText(m_pDrawingSymbol->GetText());

	m_ModifyProperties.SetWeight(m_pDrawingSymbol->GetWeight());
	m_ModifyProperties.SetStyle(m_pDrawingSymbol->GetStyle());
	
	CSymbolBaseData::TOOLTYPE type = m_pDrawingSymbol->GetSymbolType();
	
	if(type == CSymbolBaseData::SYMBOL_SQUARE || type == CSymbolBaseData::SYMBOL_CIRCLE || type == CSymbolBaseData::SYMBOL_TRIANGLE) // 사각도형, 원, 삼각형
		return WhenSymbolIsFigure();
	else if(type == CSymbolBaseData::SYMBOL_TEXT) //text
		return WhenSymbolIsText();
	else
		return WhenSymbol(type);

	return false;
}

bool CProperties_Manager::WhenSymbolIsFigure()
{
	CCoordinate coordStart = m_pDrawingSymbol->GetStartPoint();
	CCoordinate coordEnd = m_pDrawingSymbol->GetEndPoint();

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
//	CProperties_Figure dlg(&m_ModifyProperties,m_pDrawingSymbol->GetSymbolType(),m_strUserPath,m_nDataPeriod,coordStart,coordEnd,CWnd::FromHandle( m_hOcxWnd));
	CProperties_Figure dlg(	m_pMainBlock,
							m_blockIndex,
							m_strFormatX,
							&m_ModifyProperties,
							m_pDrawingSymbol->GetSymbolType(),
							m_strUserPath,
							m_nDataPeriod,
							coordStart,coordEnd,
							CWnd::FromHandle( m_hOcxWnd));
	// 2011.02.08 by SYS <<

	if(dlg.DoModal() == IDOK) 
	{
		m_pDrawingSymbol->SetStartCoord(dlg.GetStartCoord());
		m_pDrawingSymbol->SetEndCoord(dlg.GetEndCoord());
		return SetData();
	}

	return false;	
}

bool CProperties_Manager::WhenSymbolIsText()
{
	CProperties_text dlg(&m_ModifyProperties);
	if(dlg.DoModal() == IDOK) 
	{
		ApplySymbolProperties();
		return SetData();
	}

	return false;
}

bool CProperties_Manager::WhenSymbol(const CSymbolBaseData::TOOLTYPE type)
{
	//sy 2005.04.06
	if(m_pMainBlock == NULL)
		return false;

	CProperties dlg(&m_ModifyProperties, type);
	dlg.SetBackgroundColorInViewer(m_pMainBlock->GetBackgroundColor());
	if(dlg.DoModal() == IDOK)
	{
		ApplySymbolProperties();
		return SetData();
	}

	return false;
}

bool CProperties_Manager::SetData()
{
	if(m_pProperties != NULL && m_pDrawingSymbol != NULL) 
	{
		COLORREF crColor = m_ModifyProperties.GetColor();
		if(m_pDrawingSymbol->GetSymbolType() == CSymbolBaseData::SYMBOL_TEXT)
			crColor = m_ModifyProperties.GetTextColor();
		m_pDrawingSymbol->SetColor(crColor);
		m_pDrawingSymbol->SetStyle(m_ModifyProperties.GetStyle());
		m_pDrawingSymbol->SetWeight(m_ModifyProperties.GetWeight());
		//sy 2005.04.06
		m_pDrawingSymbol->SetLogFontAndTextColor(m_ModifyProperties.GetLogFont(), m_ModifyProperties.GetTextColor());
		m_pDrawingSymbol->SetSizeType(m_ModifyProperties.GetSizeType());
		m_pDrawingSymbol->SetText(m_ModifyProperties.GetText());
		m_pDrawingSymbol->SetFillState(m_ModifyProperties.GetFillState());
		return true;
	}
	return false;
}

//text, 도형에서 설정된 속성값은 기호 Dlg에 적용시키면 안된다..
//기호 Dlg가 뜰대..CPropertiesData의 값을 가지고 뜨기때문에 
//text, 도형에서 설정한 값은 반영하면 안된다.
void CProperties_Manager::ApplySymbolProperties()
{
	COLORREF crColor = m_ModifyProperties.GetColor();
	if(m_pDrawingSymbol != NULL)
	{
		if(m_pDrawingSymbol->GetSymbolType() == CSymbolBaseData::SYMBOL_TEXT)
			crColor = m_ModifyProperties.GetTextColor();
	}
	m_pProperties->SetColor(crColor);
	m_pProperties->SetLogFont(m_ModifyProperties.GetLogFont());
	m_pProperties->SetSizeType(m_ModifyProperties.GetSizeType());
	m_pProperties->SetFillState(m_ModifyProperties.GetFillState());
}
