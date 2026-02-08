// ChartCtrlMap.h: interface for the CChartCtrlMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTCTRLMAP_H__2C231D5D_926E_4FE0_957E_0E74B2072D77__INCLUDED_)
#define AFX_CHARTCTRLMAP_H__2C231D5D_926E_4FE0_957E_0E74B2072D77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>			// for CTypedPtrList

class CChartCtrl;
typedef CTypedPtrMap< CMapPtrToPtr, HWND, CChartCtrl *> CChartCtrlMapBase;

class CChartCtrlMap : public CChartCtrlMapBase
{
public:
	CChartCtrlMap();
	virtual ~CChartCtrlMap();

};

#endif // !defined(AFX_CHARTCTRLMAP_H__2C231D5D_926E_4FE0_957E_0E74B2072D77__INCLUDED_)
