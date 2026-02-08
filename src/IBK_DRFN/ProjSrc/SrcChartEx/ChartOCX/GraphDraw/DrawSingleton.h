// DrawSingleton.h: interface for the CDrawSingleton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWSINGLETON_H__0F3CD648_46B2_4EBA_B21B_3122BE17D210__INCLUDED_)
#define AFX_DRAWSINGLETON_H__0F3CD648_46B2_4EBA_B21B_3122BE17D210__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BlockBaseData.h"

class CDraw;
class CBarDraw;
class CBongDraw;
class CLineDraw;
class CPieDraw;
class CSpecialDraw;
class CFundDraw;
class CDataViewDraw;
class COptionDraw;
class CSingleBarDraw;
class CPairBarDraw;
class CDrawingGraphData;

class CDrawSingleton  
{
public:
	static CDraw* Find( CGraphBaseData::GRAPHTYPE graphType, CGraphStyle& graphStyle, const CDrawingGraphData& drawingGraphData);// CGraphBaseData::BARSTYLE barStyle);

private:
//	static CBarDraw m_BarDraw;
	static CBongDraw m_BongDraw;
	static CLineDraw m_LineDraw;
	static CPieDraw m_PieDraw;
	static CSpecialDraw m_SpecialDraw;
	static CFundDraw m_FundDraw;
	static CDataViewDraw m_DataViewDraw;
	static COptionDraw m_OptionDraw;
	static CSingleBarDraw m_SingleBarDraw;
	static CPairBarDraw m_PairBarDraw;

};

#endif // !defined(AFX_DRAWSINGLETON_H__0F3CD648_46B2_4EBA_B21B_3122BE17D210__INCLUDED_)
