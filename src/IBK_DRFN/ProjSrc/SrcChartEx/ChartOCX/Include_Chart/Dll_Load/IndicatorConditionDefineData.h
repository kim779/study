// IndicatorConditionDefineData.h: interface for the CIndicatorConditionDefineData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATORCONDITIONDEFINEDATA_H__29B21D11_A06D_4176_8A1D_23AE5AC0039D__INCLUDED_)
#define AFX_INDICATORCONDITIONDEFINEDATA_H__29B21D11_A06D_4176_8A1D_23AE5AC0039D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include <deque>
#include <list>

using namespace std;

class CIndicatorInfo;
class CGraphTypeData;
class CSubGraphData;
class CSubGraphPartData;
class CGraphColor;

typedef deque<CGraphTypeData*> CGraphTypeDataDeque;
typedef list<CSubGraphPartData*> CSubGraphPartDataList;
typedef CList<double, double> CDoubleList;
typedef CList<CString, CString> CStringDataList;
typedef CList<CGraphColor*, CGraphColor*> CGraphColorPointerList;
typedef CList<CSubGraphData*, CSubGraphData*> CSubGraphDataList;

#endif // !defined(AFX_INDICATORCONDITIONDEFINEDATA_H__29B21D11_A06D_4176_8A1D_23AE5AC0039D__INCLUDED_)
