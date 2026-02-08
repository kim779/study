// ChartManager.h: interface for the CChartManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTMANAGER_H__077F33D9_D428_4561_8D83_61BF64597186__INCLUDED_)
#define AFX_CHARTMANAGER_H__077F33D9_D428_4561_8D83_61BF64597186__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Include_AddIn.h"

#ifndef __CHART_MANAGER_H
#define __CHART_MANAGER_H
#endif
class CChartManager  
{
public:
	void Add_ChartPointer(IChartAddIn* pAddIn);
	void Refresh_AllChart();
	CChartManager();
	virtual ~CChartManager();

private:	
	CPtrList  m_ChartList;
};

#endif // !defined(AFX_CHARTMANAGER_H__077F33D9_D428_4561_8D83_61BF64597186__INCLUDED_)
