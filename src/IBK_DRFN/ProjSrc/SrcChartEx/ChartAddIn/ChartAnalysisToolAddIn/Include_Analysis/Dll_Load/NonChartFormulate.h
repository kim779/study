// NonChartFormulate.h: interface for the CNonChartFormulate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NONCHARTFORMULATE_H__7FC10CCA_025C_4A03_9C1C_0269C9C26CF0__INCLUDED_)
#define AFX_NONCHARTFORMULATE_H__7FC10CCA_025C_4A03_9C1C_0269C9C26CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "PacketBase.h"

class CIndicatorList;
class CPacketList;
class CNonChartImplementation;


class AFX_EXT_CLASS CNonChartFormulate  
{
public:
	CNonChartFormulate();
	virtual ~CNonChartFormulate();

	bool GetAutoTrendData(const CCalculateBaseData::CALCULATETYPE eCalType, 
		const CString& strPacketName, CPacketList* pPacketList, 
		CList<CPacket*, CPacket*>* pCalculateForPacketList) const;
	
private:
	CNonChartImplementation* m_pNonChartImplementation;

};

#endif // !defined(AFX_NONCHARTFORMULATE_H__7FC10CCA_025C_4A03_9C1C_0269C9C26CF0__INCLUDED_)
