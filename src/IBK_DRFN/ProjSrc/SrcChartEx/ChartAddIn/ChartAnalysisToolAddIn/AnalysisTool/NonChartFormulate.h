// NonChartFormulate.h: interface for the CNonChartFormulate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NONCHARTFORMULATE_H__7FC10CCA_025C_4A03_9C1C_0269C9C26CF0__INCLUDED_)
#define AFX_NONCHARTFORMULATE_H__7FC10CCA_025C_4A03_9C1C_0269C9C26CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../Include_Analysis/Include_AddIn.h"				// for Ixxxxxx.h
#include "BlockBaseEnum.h"					// for CCalculateBaseData

class CIndicatorList;
class CNonChartImplementation;

#ifndef AFX_EXT_CLASS_ANALYSISTOOL
	#ifdef _ANALYSISTOOL_EXPORT								// in MainAnalysisTool
		#define AFX_EXT_CLASS_ANALYSISTOOL	AFX_CLASS_EXPORT
	#else													// in Other, To Link
		#define AFX_EXT_CLASS_ANALYSISTOOL	AFX_CLASS_IMPORT
	#endif
#endif

class AFX_EXT_CLASS_ANALYSISTOOL CNonChartFormulate  
{
public:
	CNonChartFormulate();
	virtual ~CNonChartFormulate();

	bool GetAutoTrendData(const CCalculateBaseData::CALCULATETYPE eCalType, 
		const CString& strPacketName, IPacketManager* pPacketList, 
		CList<IPacket*, IPacket*>* pCalculateForPacketList) const;
	
private:
	CNonChartImplementation* m_pNonChartImplementation;

};

#endif // !defined(AFX_NONCHARTFORMULATE_H__7FC10CCA_025C_4A03_9C1C_0269C9C26CF0__INCLUDED_)
