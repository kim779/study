// ChartManager.cpp: implementation of the CChartManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "chartguidelineaddin.h"
#include "ChartManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartManager::CChartManager()
{

}

CChartManager::~CChartManager()
{
	m_ChartList.RemoveAll();
}

void CChartManager::Add_ChartPointer(IChartAddIn *pAddIn)
{
	if(pAddIn != NULL)
	{
		IChartAddIn* pChartAddIn = pAddIn;
		pChartAddIn->AddRef(); //이 Inferface를 쓰기위해 AddRef
		m_ChartList.AddTail(pChartAddIn); //차후 전체 갱신을 위해 list에 저장한다.
	}
}

void CChartManager::Refresh_AllChart()
{
	IChartAddIn* pChartAddIn = NULL;
	
	POSITION pos = m_ChartList.GetHeadPosition();
	POSITION pos2 = NULL;
	while(!(pos == NULL))		
	{		
		pChartAddIn = (IChartAddIn*)m_ChartList.GetAt(pos);		
		if(pChartAddIn != NULL)
		{
			if(pChartAddIn->Release())
			{				
				pChartAddIn->InvokeAddIn(-1, 0);//갱신 시키고
				pChartAddIn->AddRef();//다시 Addref					
			}
			else //이미 무효화된 포인터는 제거한다.
			{	
				pos2 = pos;
				m_ChartList.GetNext(pos);
				m_ChartList.RemoveAt(pos2);
				pChartAddIn = NULL;
				continue;
			}
		}
		m_ChartList.GetNext(pos);
	}	
}