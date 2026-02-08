// ChartStyleDataController.h: interface for the CChartStyleDataController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTSTYLEDATACONTROLLER_H__B270CEB9_8386_49D4_886E_9206A6C1A085__INCLUDED_)
#define AFX_CHARTSTYLEDATACONTROLLER_H__B270CEB9_8386_49D4_886E_9206A6C1A085__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EnvironmentData.h"
#include "BlockBaseData.h"

class CMainBlock;
class CIndicatorList;
class CIndicatorInfo;
class CChartStyleDataController  
{
public:
	CChartStyleDataController( CMainBlock* pMainBlock, CIndicatorList* pIndicatorList);
	void GetStyleData_Indicator( CStyleData& styleData) const;
	void SetStyleData_Indicator( const CStyleData& styleData);

protected:
	CMainBlock *		m_pMainBlock;
	CIndicatorList *	m_pIndicatorList;

protected:
	void GetPriceGraphColor( CStyleData& styleData) const;
	void SetPriceGraphColor( const CStyleData& styleData);

protected:
	void GetPriceMAGraphColor( CStyleData& styleData) const;
	void SetPriceMAGraphColor( const CStyleData& styleData);

protected:
	void GetVolumeGraphColor( CStyleData& styleData) const;
	void SetVolumeGraphColor( const CStyleData& styleData);

protected:
	void GetVolumeMAGraphColor( CStyleData& styleData) const;
	void SetVolumeMAGraphColor( const CStyleData& styleData);
};

#endif // !defined(AFX_CHARTSTYLEDATACONTROLLER_H__B270CEB9_8386_49D4_886E_9206A6C1A085__INCLUDED_)
