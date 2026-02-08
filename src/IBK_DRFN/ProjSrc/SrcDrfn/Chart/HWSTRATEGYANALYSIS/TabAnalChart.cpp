// TabAnalChart.cpp : implementation file
//

#include "stdafx.h"

#include "TabAnalChart.h"
//#include "InputLeft.h"
#include "../Common_ST/IDataItem.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabAnalChart dialog
const UINT RMSG_UPDATE_DATAINDEX = ::RegisterWindowMessage("RMSG_UPDATE_DATAINDEX");
const UINT RMSG_SELECT_DATAINDEX = ::RegisterWindowMessage("RMSG_SELECT_DATAINDEX");

CTabAnalChart::CTabAnalChart(CWnd* pParent /*=NULL*/)
	: CDlgInterface(CTabAnalChart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabAnalChart)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void CTabAnalChart::DoDataExchange(CDataExchange* pDX)
{
	CDlgInterface::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabAnalChart)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabAnalChart, CDlgInterface)
	//{{AFX_MSG_MAP(CTabAnalChart)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(RMSG_UPDATE_DATAINDEX, OnUpdateDataIndex)
	ON_REGISTERED_MESSAGE(RMSG_SELECT_DATAINDEX, OnSelectDataIndex)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabAnalChart message handlers
void CTabAnalChart::UpdateDlg()
{

	//CIStrategyItem* pStrategyItem = m_pInput->GetStrategyItem();
	//int nCount = pStrategyItem->GetCount();
	//ST_DRAWINFO *pDrawInfo = (ST_DRAWINFO *)pStrategyItem->GetPtrDrawInfo(nCount-1);

	//CISDataItem *pSDataItemDate = (CISDataItem *)pDrawInfo->pPtrArraySDataItem->GetAt(0);
	//CIDataItem *pDataItemOpen = (CIDataItem *)pDrawInfo->pPtrArrayHistoryDataItem->GetAt(0);
	//CIDataItem *pDataItemHigh = (CIDataItem *)pDrawInfo->pPtrArrayHistoryDataItem->GetAt(1);
	//CIDataItem *pDataItemLow = (CIDataItem *)pDrawInfo->pPtrArrayHistoryDataItem->GetAt(2);
	//CIDataItem *pDataItemClose = (CIDataItem *)pDrawInfo->pPtrArrayHistoryDataItem->GetAt(3);
	//CIDataItem *pDataItemVolume = (CIDataItem *)pDrawInfo->pPtrArrayHistoryDataItem->GetAt(4);

	//ST_CHARTHEADER stChartHeader;
	//int nSize = pSDataItemDate->GetSize();
	//ST_CHARTDATA* pStChartData = new ST_CHARTDATA[nSize];

	//

	//memset(pStChartData,0x20,sizeof(ST_CHARTDATA)*nSize);

	//CString strData;
	//ST_CHARTDATA* pStChartDataShifted = pStChartData;
	//for(int nPos=0;nPos<nSize;nPos++)
	//{
	//	strData = pSDataItemDate->GetAt(nPos);
	//	memcpy(pStChartDataShifted->date,(LPCTSTR)strData,strData.GetLength());
	//	strData.Format("%10.2f",pDataItemOpen->GetAt(nPos));
	//	memcpy(pStChartDataShifted->open,(LPCTSTR)strData,strData.GetLength());
	//	strData.Format("%10.2f",pDataItemHigh->GetAt(nPos));
	//	memcpy(pStChartDataShifted->high,(LPCTSTR)strData,strData.GetLength());
	//	strData.Format("%10.2f",pDataItemLow->GetAt(nPos));
	//	memcpy(pStChartDataShifted->low,(LPCTSTR)strData,strData.GetLength());
	//	strData.Format("%10.2f",pDataItemClose->GetAt(nPos));
	//	memcpy(pStChartDataShifted->close,(LPCTSTR)strData,strData.GetLength());
	//	strData.Format("%10.0f",pDataItemVolume->GetAt(nPos));
	//	memcpy(pStChartDataShifted->volume,(LPCTSTR)strData,strData.GetLength());
	//	pStChartDataShifted++;
	//}

	//memset(&stChartHeader,0x20,sizeof(stChartHeader));
	//strData.Format("%5d",nSize);
	//memcpy(stChartHeader.tmp,(LPCTSTR)strData,strData.GetLength());

	//
	//CString strTime = pStrategyItem->GetTime();
	//CString strTimeUnit = strTime.Mid(strTime.GetLength()-2);
	//CString strTimeNum = strTime.Left(strTime.GetLength()-2);
	//long lTimeNum = atol(strTimeNum);
	//CString strBojoMsg;

	//if(strTimeUnit=="ÀÏ")
	//{
	//	strBojoMsg = "0086UM=E@UD=1@RDATET=YYYYMMDD@RESETUSERREAL=0@";
	//}
	//else if(strTimeUnit=="ºÐ")
	//{
	//	strBojoMsg = "0086UM=E@UD=4@RDATET=MMDDHHMM@RESETUSERREAL=0@";
	//}
	//else if(strTimeUnit=="Æ½")
	//{
	//	strBojoMsg = "0086UM=E@UD=4@RDATET=DDHHMMSS@RESETUSERREAL=0@";
	//}
	//else
	//{
	//	ASSERT(0);
	//}

	//memcpy(stChartHeader.bojomsg,(LPCTSTR)strBojoMsg,strBojoMsg.GetLength());

	//long lBufferSize = sizeof(stChartHeader)+sizeof(ST_CHARTDATA)*nSize;
	//char *pBuffer = new char[lBufferSize];
	//char *pBufferShifted = pBuffer;
	//memcpy(pBufferShifted,&stChartHeader,sizeof(stChartHeader));
	//pBufferShifted += sizeof(stChartHeader);
	//memcpy(pBufferShifted,pStChartData,sizeof(ST_CHARTDATA)*nSize);

	//m_Chart.AddPacketAndGraph();
	//m_Chart.SetPacketData(m_Chart.GetPacketName(),(long)pBuffer,lBufferSize,TRUE);
	//m_Chart.SetNOnePageDataCount(nSize+10);
	//m_Chart.ShowInnerToolBar(FALSE);
	//m_Chart.InvokeAddIn("ChartStrategyAddin.dll",2,(long)pDrawInfo);
	//m_Chart.InvokeAddIn("ChartStrategyAddin.dll",3,(long)&m_pInput->m_stDataIndex);
	//m_Chart.InvokeAddIn("ChartStrategyAddin.dll",4,(long)GetSafeHwnd());
	//if(strTimeUnit=="Æ½"&&lTimeNum==1)
	//{
	//	m_Chart.AddChartBlock("¼±Â÷Æ®");
	//}
	//else
	//{
	//	m_Chart.AddChartBlock("ºÀÂ÷Æ®");
	//}
	//delete  pStChartData;
	//delete  pBuffer;
}

BOOL CTabAnalChart::OnInitDialog() 
{
	CDlgInterface::OnInitDialog();
	
	// TODO: Add extra initialization here
//	CRect rect;
//	GetClientRect(rect);

	//GetWindowRect(&rect);	
	//ScreenToClient(rect);
	//rect.DeflateRect(0,0);
	//m_Chart.Create(this,rect,0x764);

	//if(!m_pInput)
	//{
	//	return FALSE;
	//}

	//m_Chart.SetHtsPath(m_pInput->m_strRootPath);
	//m_Chart.SetpBaseDesk(NULL, this->GetSafeHwnd());	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CTabAnalChart::OnUpdateDataIndex( WPARAM wParam, LPARAM lParam)
{
//	m_pInput->AllUpdateDataIndex();
	return 0L;
}

LRESULT CTabAnalChart::OnSelectDataIndex( WPARAM wParam, LPARAM lParam)
{
	//m_pInput->SetListFocus(wParam);
	return 0L;
}

void CTabAnalChart::SetChartFocus(int nFocusIndex)
{
	//m_Chart.InvokeAddIn("ChartStrategyAddin.dll",5,nFocusIndex);
}

void CTabAnalChart::OnSize(UINT nType, int cx, int cy) 
{
	CDlgInterface::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
//	if(m_Chart.GetSafeHwnd())
//	{
//		CRect rect;
//		GetClientRect(rect);
//		rect.DeflateRect(1,1);
//		m_Chart.MoveWindow(rect);
//	}
}

