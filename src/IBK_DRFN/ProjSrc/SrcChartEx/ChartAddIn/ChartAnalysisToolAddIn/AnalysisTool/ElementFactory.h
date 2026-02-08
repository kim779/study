// ElementFactory.h: interface for the CElementFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTFACTORY_H__09F61C32_2F7B_4B4D_AD4C_C3905D3A0FF8__INCLUDED_)
#define AFX_ELEMENTFACTORY_H__09F61C32_2F7B_4B4D_AD4C_C3905D3A0FF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Analysis/BlockBaseEnum2.h"		// for CAnalysis
#include "BlockBaseData.h"
#include "AnalysisData.h"

class CElement;
class CTextPosition;
class CTextShowPosition;
class CTextHorzPosition;
class CTextVertPosition;
class CTextHorzVertPosition;
class CElementCommonEnvironment;

class CElementFactory
{
public:
	static CElement* Copy(CElement* pElement);

	static CElement* Make(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point, 
		const CTextPosition* pPosition, const CElementCommonEnvironment& environment);
	static CElement* Make(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point1, const CCoordinate& point2, 
		const CTextPosition* pPosition, const CElementCommonEnvironment& environment,double dChannel=0.0);
	static CElement* Make(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point1, const CCoordinate& point2, const CCoordinate& point3, 
		const CTextPosition* pPosition, const CElementCommonEnvironment& environment);
	static CElement* Make(const CAnalysis::TOOLTYPE eToolType,int nPointsCount,const CPoints* pPointsList,const CElementCommonEnvironment& environment);
	static CElement* Make(const CAnalysis::TOOLTYPE eToolType,const CDequeList<CCoordinate>& pointList,const CElementCommonEnvironment& environment);
	
	static void Delete(CElement* pElement);

private:
	static CElement* Make_HorzVertText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point, const CTextHorzVertPosition* pPosition, const CElementCommonEnvironment& environment);
	static CElement* Make_VertText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point, const CTextVertPosition* pPosition, const CElementCommonEnvironment& environment);
	static CElement* Make_HorzText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point, const CTextHorzPosition* pPosition, const CElementCommonEnvironment& environment);

	static CElement* Make_ShowText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point,const CTextShowPosition* pPosition, const CElementCommonEnvironment& environment);
	static CElement* Make_ShowText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point1, const CCoordinate& point2, const CTextShowPosition* pPosition, const CElementCommonEnvironment& environment);
	static CElement* Make_HorzText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point1, const CCoordinate& point2, const CTextHorzPosition* pPosition, const CElementCommonEnvironment& environment);
	static CElement* Make_NoText(const CAnalysis::TOOLTYPE eToolType, const CCoordinate& point1, const CCoordinate& point2, const CElementCommonEnvironment& environment,double dChannel);
};

#endif // !defined(AFX_ELEMENTFACTORY_H__09F61C32_2F7B_4B4D_AD4C_C3905D3A0FF8__INCLUDED_)
