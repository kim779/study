// NonChartImplementation.h: interface for the CNonChartImplementation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NONCHARTIMPLEMENTATION_H__88A94EFE_987F_40F4_8C2B_53CE86D03763__INCLUDED_)
#define AFX_NONCHARTIMPLEMENTATION_H__88A94EFE_987F_40F4_8C2B_53CE86D03763__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h

#include "BlockBaseEnum.h"				// for CCalculateBaseData

class CNonChartImplementation  
{
public:
	CNonChartImplementation();
	virtual ~CNonChartImplementation();

	bool	GetAutoTrendData(const CCalculateBaseData::CALCULATETYPE eCalType, const CString& strPacketName, IPacketManager * pPacketList,
				CList<IPacket*, IPacket*>* pCalculateForPacketList); 

private:
/*
	bool Calculate(const CChartFormulateData& chartFormulateData,
		CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo,
		CList<CPacket*, CPacket*>* pCalculatePacketList);
	bool Calculate(const CChartFormulateData& chartFormulateData,
		CGraphCalculateData* pGraphCalculateData, CIndicatorInfo* pIndicatorInfo);
	bool AddCalculatePacketList(CGraphCalculateData* pGraphCalculateData,
		CList<CPacket*, CPacket*>* pCalculatePacketList);
	CPoint GetDataPoint(const CChartFormulateData& chartFormulateData,
		CGraphCalculateData* pGraphCalculateData);
	void GetDataPointWhenREALAPPEND(CPacketList* pPacketList,
		CGraphCalculateData* pGraphCalculateData, CPoint& point);
	void GetChartFormulateData(const CCalculateBaseData::CALCULATETYPE eCalType, 
		const CString& strFunctionName, CPacketList* pPacketList, 
		CChartFormulateData& chartFormulateData) const;

private:
	CCalculateData m_CalculateData;
	CIndicatorSingleton m_indiSingleton;
	CIndicator* m_Indicator;
*/
};

#endif // !defined(AFX_NONCHARTIMPLEMENTATION_H__88A94EFE_987F_40F4_8C2B_53CE86D03763__INCLUDED_)
