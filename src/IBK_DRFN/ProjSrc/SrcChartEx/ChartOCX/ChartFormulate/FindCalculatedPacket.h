// FindCalculatedPacket.h: interface for the CFindCalculatedPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FINDCALCULATEDPACKET_H__414E9A92_9A21_4159_98B5_80B7872F399F__INCLUDED_)
#define AFX_FINDCALCULATEDPACKET_H__414E9A92_9A21_4159_98B5_80B7872F399F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "ChartFormulateData.h"
#include "CalculateData.h"

class CPacketList;
class CFindCalculatedPacket  
{
public:
	//한번이라도 Packet.dll에 계산된 data를 저장한적이 있는지 확인. 있으면 true, 없으면 false
	static bool IsSubGraphIntoPacketList(const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData);

private:
	static void WhenPacketIsIntData(const CString& strSubGraphName, const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData);
	static void WhenPacketIsStringData(const CString& strSubGraphName, const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData);
	static void IsSubGraphIntoPacketListWhenThereisSubGraphName(const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData);
	static void IsSubGraphIntoPacketListbyNEWandINSERTandAPPEND(const CString& strSubGraphName, const CChartFormulateData& chartFormulateData, CCalculateData& CalculateData);

	static bool IsFinishCalulationWhenNew(const CString& strSubGraphName, const int nStandardCount, const int nMyCount, CList<double, double>& ConditionList);
	static bool IsFinishCalulationWhenInsertAppend(const CString &strSubGraphName, const int nStandardCount, const int nMyCount, CList<double, double>& ConditionList);

};

#endif // !defined(AFX_FINDCALCULATEDPACKET_H__414E9A92_9A21_4159_98B5_80B7872F399F__INCLUDED_)
