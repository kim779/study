#include "stdafx.h"
#include "PcTrMng.h"
#include "../../inc/IAUChartUtilMng.h"

/////////////////////////////////////////////////////////////////////////////
// CAUChartUtilManager
CAUChartUtilManager::CAUChartUtilManager()
{
	m_pMChartDll = NULL;

	for(int i=0; i<ID_IAUChartUtilManager_Max; i++)
		m_ChartMngID[i]=0;
}

CAUChartUtilManager::~CAUChartUtilManager()
{
	if(m_pMChartDll)
	{
		delete m_pMChartDll;
		m_pMChartDll = NULL;
	}
}

IChartFloatDlg* CAUChartUtilManager::AdviseChartFloatDlg()
{
	if(!m_pMChartDll) 
	{
		m_pMChartDll = new CMChartUtilLoadLib;
	}
	if(m_pMChartDll)
		return (IChartFloatDlg*)m_pMChartDll->UtilFDlgAdvise();

	return NULL;
}

void CAUChartUtilManager::UnAdviseChartFloatDlg(IChartFloatDlg* dwKey)
{
	if(m_pMChartDll) 
	{
		m_pMChartDll->UtilFDlgUnAdvise((LONG)dwKey);
	}
}
