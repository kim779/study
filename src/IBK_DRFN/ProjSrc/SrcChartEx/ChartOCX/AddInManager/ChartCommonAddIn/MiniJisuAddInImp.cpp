// MiniJisuAddInImp.cpp: implementation of the CMiniJisuAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "MiniJisuAddInImp.h"

#include "../../Include_Chart/Dll_Load/IMetaTable.h"				// for _MTEXT()
#include "../Include_AddIn/AddInCommand.h"							// for EAI_DRDS_PACKET_END
#include "../Include_AddIn/I000000/_IChartManager.h"				// for IChartManager
#include "../Include_AddIn/I000000/_IBlock.h"						// for IBlock
#include "../Include_AddIn/I000000/_IPacketManager.h"				// for IPacketManager
#include "../Include_AddIn/I000000/_IPacket.h"						// for IPacket
#include "../Include_AddIn/I000000/_IString.h"						// for ILPCSTR
#include "../Include_AddIn/I000000/_IChartOCX.h"					// for IChartOCX

#define UDM_SEND_DATALIST				WM_USER + 611


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMiniJisuAddInImp::CMiniJisuAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	//미니지수 유기태 8.22
	m_bSendDataList2Parent = false;
	m_bMiniStartDrag = false;
}

CMiniJisuAddInImp::~CMiniJisuAddInImp()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CMiniJisuAddInImp::m_strAddInItemName = "MINIJISU";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CMiniJisuAddInImp)
	ONADDINEVENT( OnMouseMove)
	ONADDINEVENT( OnLButtonDown)
	ONADDINEVENT( OnLButtonUp)
END_ADDIN_EVENT_MAP()


//////////////////////////////////////////////////////////////////////
// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
//		*. 또한 다음의 _IWRAPPER_IUNKNOWN_INTERFACE도 정의한다.
//////////////////////////////////////////////////////////////////////
IUnknown *CMiniJisuAddInImp::GetAddInCustomInterface( void)
{
	IUnknown *pIUnknown = ( IUnknown *)( IMiniJisu *)this;
	pIUnknown->AddRef();
	return pIUnknown;
}

//////////////////////////////////////////////////////////////////////
// (2006/3/23 - Seung-Won, Bae) MiniJisu Interface
//////////////////////////////////////////////////////////////////////
void CMiniJisuAddInImp::SendToChartMiniJisu(LPCTSTR szCommand, LPCTSTR szParam1, LPCTSTR szParam2, LPCTSTR szParam3)
{
	CString Command = szCommand;
	CString Param1, Param2, Param3;
	if(Command == "Receive_DataList"){
		Param1 = szParam1;
		m_bSendDataList2Parent = (Param1 == "1") ? true : false;
		m_bMiniStartDrag = FALSE;
		if(m_bSendDataList2Parent)
		{
			CString strPacketData = GetPacketDataForMiniJisu(-1, -1);
			m_pChartCWnd->GetParent()->SendMessage(UDM_SEND_DATALIST, 0, (LPARAM)(LPSTR)(LPCSTR)strPacketData);
		}
	}
	else if(Command == "Graph_Scale Color")
	{
		m_pIChartOCX->SetStyleValueGraphBGColor(	atol( szParam1));
		m_pIChartOCX->SetStyleValueScaleBGColor(	atol( szParam2));
		m_pIChartOCX->SetStyleValueScaleTextColor(	atol( szParam3));
	}
	else if(Command == "ScalePosition")
	{
		Param1 = szParam1;
		CScaleBaseData::VERTSCALEPOSITION nPosition = (CScaleBaseData::VERTSCALEPOSITION)atoi(Param1);
		m_pIChartManager->SetAllVertScalePosition(nPosition);
	}
}

//2003.07.28 nykim 미니지수(dll)에서 데이터보기를 하기위한 전용루틴
//sy 2003.7.30.
CString CMiniJisuAddInImp::GetPacketDataForMiniJisu(short nPoint_x, short nPoint_y) 
{
	CPoint point(nPoint_x, nPoint_y);
	// 1. 먼저 Block으로부터 선택된 Data Index를 구한다.
	// 1.1 주어진 Point에 해당하는 Block의 Graph Region을 구한다.
	// (2004.06.14, 배승원) Block 영역의 Height에, Graph의 Width 영역을 Input 영역으로 확인한다.
	// (2006/6/16 - Seung-Won, Bae) Use Block Inteface
	int nRow, nColumn;
	if( !m_pIChartManager->GetBlockIndex( point, nRow, nColumn)) return "";
	IBlock *pIBlock = m_pIChartManager->GetBlock( nRow, nColumn);
	if( !pIBlock) return "";
	CRect rctBlockRegion, rctNoFrameAndMargin;
	pIBlock->GetGraphRegions( rctBlockRegion, rctNoFrameAndMargin);
	CRect rctGraphRegion = pIBlock->GetGraphDrwingRegion();
	pIBlock->Release();
	pIBlock = NULL;
	rctBlockRegion.left = rctGraphRegion.left;
	rctBlockRegion.right = rctGraphRegion.right;
	if( !rctBlockRegion.PtInRect( point)) return "";
	// 1.2 현재 화면에 보이는 Data의 Start/End Index를 구한다.
	int nStartIndex, nEndIndex;
	if( !m_pIChartOCX->GetViewStartEndIndex( nStartIndex, nEndIndex)) return "";
	// 1.3 지정된 Mouse Point의 Data Index를 구한다.
	int nDataIndex = nStartIndex + ( ( point.x - rctBlockRegion.left) * ( nEndIndex - nStartIndex + 1)) / rctBlockRegion.Width();

	if(nDataIndex < 0 && nPoint_x <0 && nPoint_y <0) nDataIndex = -9999;

	return GetPacketDataForMiniJisuByIndex( nDataIndex);
}

CString CMiniJisuAddInImp::GetPacketDataForMiniJisuByIndex(int nDataIndex) 
{
	int nMiniIndex= nDataIndex;
	BOOL bResult = false;
	CString strAllData;
	POSITION psnViewData = m_pIChartManager->GetSubGraphViewData_First( "DEFAULT");
	while( psnViewData)
	{
		double dDate = 0.0;
		ILPCSTR szTitle = m_pIChartManager->GetSubGraphViewData_TitleString( psnViewData);

		IPacket* pIPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
		if( pIPacket != NULL)
		{
			//미니지수 유기태 8.22
			if(!m_bMiniStartDrag && m_bSendDataList2Parent && nDataIndex == -9999)
				nMiniIndex = pIPacket->GetDataCount() - 1;
			pIPacket->GetData( nMiniIndex, dDate);
			pIPacket->Release();
		}
		 
		double dPrice = 0.0;
		pIPacket = m_pIDefaultPacketManager->GetPacket( szTitle);
		if( pIPacket != NULL)
		{
			pIPacket->GetData( nMiniIndex, dPrice);
			pIPacket->Release();
		}

		double dPrevPrice = 0.0;
		pIPacket = m_pIDefaultPacketManager->GetPacket( CString( szTitle) + "_" + _MTEXT( C6_PREVIOUS_DAY_COMPARISON));
		if( pIPacket != NULL)
		{
			pIPacket->GetData( nMiniIndex, dPrevPrice);
			pIPacket->Release();
		}

		double dRate = 0.0;
		pIPacket = m_pIDefaultPacketManager->GetPacket( CString( szTitle) + "_" + _MTEXT( C6_CHANGE_RATE));
		if( pIPacket != NULL)
		{
			pIPacket->GetData( nMiniIndex, dRate);
			pIPacket->Release();
		}

		double dVolume = 0.0;
		pIPacket = m_pIDefaultPacketManager->GetPacket( CString( szTitle) + "_" + _MTEXT( C3_VOLUME));
		if( pIPacket != NULL)
		{
			pIPacket->GetData( nMiniIndex, dVolume);
			pIPacket->Release();
		}

		CString strItem;
		// (2004.02.04, 배승원) Packet Data가 없는 곳에 Mouse Point가 위치하여
		//		nMiniIndex가 Packet Data 수를 넘어서는 Exception을 처리한다.
		//		Packet Data가 없는 경우 다른 Graph에도 마찬가지이므로,
		//			이곳에서는 Output String을 구성하지만 않도록 하고,
		//			최종 처리시 Empty String인 경우 Message도 날리지 않도록 한다.
		if( dDate != ERRORVALUE)
		{
			if(dPrice != ERRORVALUE)
			{
				// (2004.03.16, 배승원) dPrice는 있는데, dPrevPrice, dRate, dVolume가 오류인 경우를 처리한다.
				//		속도를 위하여 개별로 처리하지 않고, dPrevPrice가 없는 경우는 dRate, dVolume도 없는 것으로 처리한다.
				if( dPrevPrice != ERRORVALUE)
					strItem.Format("%-50.50s%12.0f%12.2f%12.2f%12.2f%12.0f;", szTitle, dDate, dPrice, dPrevPrice, dRate, dVolume);
				else strItem.Format("%-50.50s%12.0f%12.2f%8s%8s%8s;", szTitle, dDate, dPrice, "N/A", "N/A", "N/A");
			}
			else
				strItem.Format("%-50.50s%8.0f%8s%8s%8s%8s;", szTitle, dDate,"N/A","N/A","N/A","N/A");
		}

		strAllData += strItem;
		
		bResult = true;

		m_pIChartManager->GetSubGraphViewData_Next( psnViewData);
	}

	return strAllData;
}


//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CMiniJisuAddInImp::OnMouseMove( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID, const HBITMAP p_hbmpMainOSB)	// (2006/11/5 - Seung-Won, Bae) Support MainOSB
{
	// 미니지수
	if( m_bSendDataList2Parent && m_bMiniStartDrag)
	{
		// (2004.02.04, 배승원) Packet Data가 없는 곳에 Mouse Point가 위치하여
		//			nMiniIndex가 Packet Data 수를 넘어서는 Exception을 처리한다.
		//		Packet Data가 없는 경우 Output String을 구성하지만 않도록 하고,
		//			최종 처리시 Empty String인 경우 Message도 날리지 않도록 한다.
		CString strPacketData = GetPacketDataForMiniJisu((short)point.x, (short)point.y);
		if( !strPacketData.IsEmpty()) m_pChartCWnd->GetParent()->SendMessage(UDM_SEND_DATALIST, 0, (LPARAM)(LPSTR)(LPCSTR)strPacketData);
	}
	return FALSE;	// for Cross Line
}
BOOL CMiniJisuAddInImp::OnLButtonDown( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	///////////////////////////////////////////// BEGIN
	//미니지수 유기태 08/22
	if(m_bSendDataList2Parent) m_bMiniStartDrag = TRUE;
	//////////////////////////////////////////////////////// END
	return FALSE;	// for Cross Line
}
BOOL CMiniJisuAddInImp::OnLButtonUp( HDC p_hDC, UINT nFlags, POINT &point, const DWORD p_dwMouseActionID)
{
	//미니지수 유기태 08/22
	 m_bMiniStartDrag = FALSE;
	if(m_bSendDataList2Parent)
	{
		 CString strPacketData = GetPacketDataForMiniJisu(-1, -1);
		 m_pChartCWnd->GetParent()->SendMessage(UDM_SEND_DATALIST, 0, (LPARAM)(LPSTR)(LPCSTR)strPacketData);
	}
	return FALSE;	// for Cross Line
}
