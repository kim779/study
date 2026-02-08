// IWGraphInfoManager.h: interface for the CIWGraphInfoManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWGRAPHINFOMANAGER_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_)
#define AFX_IWGRAPHINFOMANAGER_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../Include_Chart/Dll_Load/IndicatorList.h"		// for CIndicatorList
#include "../_IWUnknown.h"
#include "_IGraphInfoManager.h"
#include "_IChartOCX.h"											// for IChartOCX

// (2004.10.13, 배승원) IndicatorList의 Interface Wrapper Class를 정의한다.
class CIWGraphInfoManager : public CIWUnknown, public IGraphInfoManager
{
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IGraphInfoManager, CIWGraphInfoManager, CIndicatorList, m_pIndicatorList)

// [04/10/14] Graph Info를 조회하는 Interface를 제공한다.
protected:
	// [04/10/14] Graph의 6색상을 조회하는 Interface를 제공한다.
	virtual BOOL	GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6]);
	
	// 종찬(05/07/14) 지표 설정값을 제공한다.
	virtual CList<double, double>* GetCommonConditionList(const char* pcszGraphName);
	virtual CList<double, double>* GetConditionList(const char* pcszGraphName, const char* pcszSubGraphName);
	virtual CList<double, double>* GetConditionList(const char* pcszGraphName, const int nSubGraphIndex);
	
// [05/07/15] Getting Graph Info Interface
protected:
	virtual IGraphInfo *		GetGraphInfo( const char *p_szGraphName);
};

#endif // !defined(AFX_IWGRAPHINFOMANAGER_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_)
