// Properties_Manager.h: interface for the CProperties_Manager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTIES_MANAGER_H__B25D21B0_1CD2_4BA3_980F_50F917515254__INCLUDED_)
#define AFX_PROPERTIES_MANAGER_H__B25D21B0_1CD2_4BA3_980F_50F917515254__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h

#include "BlockBaseData.h"
#include "PropertiesData_text.h"

class CDrawingSymbol;
class CProperties_Manager  
{
public:

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
// 	CProperties_Manager(CDrawingSymbol* pDrawingSymbol,
// 						CPropertiesData_text* pProperties,
// 						IChartManager* pMainBlock,
// 						const CString& strUserPath,
// 						const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
// 						HWND p_hOcxWnd);

	CProperties_Manager(CDrawingSymbol* pDrawingSymbol,
						CPropertiesData_text* pProperties,
						IChartManager* pMainBlock,
						const CString& strUserPath,
						const CScaleBaseData::HORZSCALEDRAWERTYPE& nDataPeriod,
						HWND p_hOcxWnd,
						const CBlockIndex& blockIndex, 
						const CString & strFormatX);

	// 2011.02.08 by SYS <<

	virtual ~CProperties_Manager();

	//설정Box 띄우기
	bool PopUpPropertiesBox();

private:	
	CString m_strUserPath;
	CScaleBaseData::HORZSCALEDRAWERTYPE m_nDataPeriod;
	CDrawingSymbol* m_pDrawingSymbol;
	CPropertiesData_text* m_pProperties;
	IChartManager* m_pMainBlock;
	//수정한 속성값 가지고 있기.
	CPropertiesData_text m_ModifyProperties;

	// 2011.02.08 by SYS >> 분차트 YYYY(년) 추가 : 날짜/시간 14Byte형태 변경 (YYYYMMDDhhmmss)
	CBlockIndex m_blockIndex;
	CString		m_strFormatX;
	// 2011.02.08 by SYS <<


	void ApplySymbolProperties();
	bool SetData();
	bool WhenSymbol(const CSymbolBaseData::TOOLTYPE type);
	bool WhenSymbolIsText();
	bool WhenSymbolIsFigure();

protected:
	HWND	m_hOcxWnd;
};

#endif // !defined(AFX_PROPERTIES_MANAGER_H__B25D21B0_1CD2_4BA3_980F_50F917515254__INCLUDED_)
