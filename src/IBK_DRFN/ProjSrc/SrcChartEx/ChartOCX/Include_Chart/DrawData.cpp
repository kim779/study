#include "StdAfx.h"
#include "DrawData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//20081114 이문수 >>
#define COMPILE_MULTIMON_STUBS
#include "multimon.h"

typedef CArray < CRect, CRect & > CRectArray;

BOOL CALLBACK MonitorEnumProc( HMONITOR hMonitor,  // handle to display monitor
							  HDC hdcMonitor,     // handle to monitor-appropriate device context
							  LPRECT lprcMonitor, // pointer to monitor intersection rectangle
							  LPARAM dwData       // data passed from EnumDisplayMonitors
							  )
{
	CRect rect = *lprcMonitor;
	
	if(dwData)
	{
		CRectArray * pRectArray = (CRectArray *)dwData;
		pRectArray->Add(rect);
	}
	return TRUE;
}


BOOL GetMonitorMinMaxX(int & nMinX, int & nMaxX)
{
	int i, nSumMonitorWidth = 0;
	CRectArray MonitorRectArray;
	BOOL bResult = EnumDisplayMonitors ( NULL, NULL, MonitorEnumProc, (long)&MonitorRectArray);
	if(bResult)
	{
		for( i = 0 ; i < MonitorRectArray.GetSize(); i++)
		{
			CRect rectMonitor = MonitorRectArray.GetAt(i);
			if(i == 0)
			{
				nMinX = rectMonitor.left;
				nMaxX = rectMonitor.right;
			}
			else
			{
				if(nMinX > rectMonitor.left)	nMinX = rectMonitor.left;
				if(nMaxX < rectMonitor.right)	nMaxX = rectMonitor.right;
			}
		}
	}
	return bResult;
}
//20081114 이문수 <<



CDrawBaseDataManager::~CDrawBaseDataManager()
{
	while(!m_DrawDataList.IsEmpty())
		delete m_DrawDataList.RemoveHead();

}



CDrawTickDataManager::CDrawTickDataManager()
{
	m_nBase = 0;
	m_nType = TICK_DRAW_DATA_TYPE;	

	m_nPolyLowSize		= 0;
	m_nPolyHighSize		= 0;
	m_nLineLowSize		= 0;
	m_nLineHighSize		= 0;	
	m_nPolyHighRealSize = 0;
	m_nPolyLowRealSize  = 0;

	m_pLineHigh = NULL;
	m_pLineLow  = NULL;
	m_pPolyHigh = NULL;
	m_pPolyLow  = NULL;
}

CDrawTickDataManager::CDrawTickDataManager(int nSize, int nBase)
{
	m_nBase = nBase;
	m_nType = TICK_DRAW_DATA_TYPE;
	
	m_nPolyLowSize		= nSize;
	m_nPolyHighSize		= nSize;
	m_nLineLowSize		= nSize;
	m_nLineHighSize		= nSize;	
	m_nPolyHighRealSize = 0;
	m_nPolyLowRealSize  = 0;	

	m_pLineHigh = new POINT[m_nLineHighSize];
	m_pLineLow  = new POINT[m_nLineLowSize];
	m_pPolyHigh = new DWORD[m_nPolyHighSize];
	m_pPolyLow  = new DWORD[m_nPolyLowSize];
}

CDrawTickDataManager::~CDrawTickDataManager()
{
	if(m_pLineHigh) delete [] m_pLineHigh;
	if(m_pLineLow)	delete [] m_pLineLow;
	if(m_pPolyHigh) delete [] m_pPolyHigh;
	if(m_pPolyLow)	delete [] m_pPolyLow;

	while(!m_DrawDataList.IsEmpty())
		delete m_DrawDataList.RemoveHead();

}

BOOL CDrawTickDataManager::AllocBuffer(int nSize)
{
	FreeBuffer();

	m_nPolyLowSize	= m_nPolyHighSize =m_nLineLowSize	=m_nLineHighSize	= nSize;	

	m_pLineHigh = new POINT[m_nLineHighSize];
	m_pLineLow  = new POINT[m_nLineLowSize];
	m_pPolyHigh = new DWORD[m_nPolyHighSize];
	m_pPolyLow  = new DWORD[m_nPolyLowSize];

	return TRUE;
}

BOOL CDrawTickDataManager::FreeBuffer()
{
	if(m_pLineHigh) delete [] m_pLineHigh;
	if(m_pLineLow)	delete [] m_pLineLow;
	if(m_pPolyHigh) delete [] m_pPolyHigh;
	if(m_pPolyLow)	delete [] m_pPolyLow;
	return TRUE;
}

POSITION CDrawTickDataManager::DrawDataMoveToHead()
{
	return m_DrawDataList.GetHeadPosition();
}


POSITION CDrawTickDataManager::DrawDataMoveToNext(POSITION pos)
{
	if(!pos) return NULL;
	m_DrawDataList.GetNext(pos);
	return pos;
}


POSITION CDrawTickDataManager::DrawDataMoveToPrev(POSITION pos)
{
	if(!pos) return NULL;
	m_DrawDataList.GetPrev(pos);
	return pos;
}


POSITION CDrawTickDataManager::DrawDataMoveToTail()
{
	return m_DrawDataList.GetTailPosition();
}


POSITION CDrawTickDataManager::AppendDrawData(int nLeft, int nCenter, int nRight)
{
	CDrawTickData * pDrawTickData	= new CDrawTickData(nLeft, nCenter, nRight);
	POSITION pos= m_DrawDataList.AddTail(pDrawTickData);
	int nSize	= m_DrawDataList.GetCount();
	if(nSize < m_nLineHighSize)
	{
		int nIndex = (nSize - 1) * 4;

		pDrawTickData->m_pHighHigh = &m_pLineHigh[nIndex + 1];//고
		pDrawTickData->m_pHighLow  = &m_pLineHigh[nIndex + 2];//저

		pDrawTickData->m_pLowHigh  = &m_pLineLow [nIndex + 1];//고
		pDrawTickData->m_pLowLow   = &m_pLineLow [nIndex + 2];//저
	}
	return pos;
}


BOOL CDrawTickDataManager::AddDrawDataItem(POSITION pos, int nDrawDataItem)
{
	CDrawTickData * pDrawTickData = (CDrawTickData *)m_DrawDataList.GetAt(pos);
	return FALSE;
}


BOOL CDrawTickDataManager::DeleteFirstDrawDataItem(POSITION pos)
{
	return FALSE;
}

//////////////////////////////////////////////////////
//CDrawSimpleLineDataManager

CDrawSimpleLineDataManager::CDrawSimpleLineDataManager()
{
	m_nType = SIMPLE_LINE_DRAW_DATA_TYPE;	

	m_nLineSize	= 0;	

	m_pLine = NULL;	
}

CDrawSimpleLineDataManager::CDrawSimpleLineDataManager(int nSize)
{
	m_nType = SIMPLE_LINE_DRAW_DATA_TYPE;
	
	m_nLineSize	= nSize;	

	m_pLine = new POINT[m_nLineSize];	
}

CDrawSimpleLineDataManager::~CDrawSimpleLineDataManager()
{
	if(m_pLine) delete [] m_pLine;
	
	while(!m_DrawDataList.IsEmpty())
		delete m_DrawDataList.RemoveHead();
}

BOOL CDrawSimpleLineDataManager::AllocBuffer(int nSize)
{
	FreeBuffer();
	m_nLineSize	= nSize;
	m_pLine = new POINT[m_nLineSize];	
	return TRUE;
}

BOOL CDrawSimpleLineDataManager::FreeBuffer()
{
	if(m_pLine) delete [] m_pLine;	
	return TRUE;
}


/////////////////////////////////////////////////////////////
//CDrawSimpleBarDataManager

CDrawSimpleBarDataManager::CDrawSimpleBarDataManager()
{
	m_nType = SIMPLE_BAR_DRAW_DATA_TYPE;	

	m_posDTIEndClose = NULL;

	m_posDTIEndOpen  = NULL;

	m_posDTIEndVolume= NULL;

	m_posDTIStartClose=NULL;

	m_posDTIStartOpen =NULL;

	m_posDTIStartVolume=NULL;

}


CDrawSimpleBarDataManager::~CDrawSimpleBarDataManager()
{
	while(!m_DrawDataList.IsEmpty())
		delete m_DrawDataList.RemoveHead();
}

//20081129 이문수 <<