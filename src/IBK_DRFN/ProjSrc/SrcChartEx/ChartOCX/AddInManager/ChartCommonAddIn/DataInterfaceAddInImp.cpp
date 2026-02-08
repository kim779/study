// DataInterfaceAddInImp.cpp: implementation of the CDataInterfaceAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "DataInterfaceAddInImp.h"

#include "../../Include_Chart/Dll_Load/IMetaTable.h"		// for _MTEXT()
#include "../Include_AddIn/I000000/_IString.h"				// for ILPCSTR
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IPacket.h"				// for IPacket
#include "../Include_AddIn/I000000/_IPacketManager.h"		// for IPacketManager
#include "../Include_AddIn/I000000/_IDoubleList.h"			// for IDoubleList

#include "IWDblList.h"										// for CIWDblList

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataInterfaceAddInImp::CDataInterfaceAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{

}

CDataInterfaceAddInImp::~CDataInterfaceAddInImp()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CDataInterfaceAddInImp::m_strAddInItemName		= "PACKET_QUERYER";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CDataInterfaceAddInImp)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// [05/07/08] AddIn 전용 Interface를 제공한다.
//		*. 주의. 반듯이 전용 Interface Pointer로 Converting후 IUnknown의 Pointer로 변환하여 전달한다.
//		*. 또한 다음의 _IWRAPPER_IUNKNOWN_INTERFACE도 정의한다.
//////////////////////////////////////////////////////////////////////
IUnknown *CDataInterfaceAddInImp::GetAddInCustomInterface( void)
{
	IUnknown *pIUnknown = ( IPacketQueryer *)this;
	pIUnknown->AddRef();
	return pIUnknown;
}


//////////////////////////////////////////////////////////////////////
// (2007/3/28 - Seung-Won, Bae) Suppoert Double Value List of Packet
//////////////////////////////////////////////////////////////////////
IDblList *CDataInterfaceAddInImp::GetDoubleList( const char *p_szChartName2, const char *p_szSubGraphName2, const int p_nPacketIndex)
{
	CString p_szChartName = _STOM2( p_szChartName2);
	CString p_szSubGraphName = _STOM3( p_szSubGraphName2);

	IPacket *pPacket = NULL;
	if( !strcmp( p_szChartName, _MTEXT( C0_DATE_TIME))) pPacket = m_pIDefaultPacketManager->GetPacket( _MTEXT( C0_DATE_TIME));
	else
	{
		int nR = 0, nC = 0, nV = 0, nG = 0;
		ILPCSTR szGraphName = m_pIChartManager->FindFirstIndicator( p_szChartName, nR, nC, nV, nG, "DEFAULT");
		if( szGraphName.IsEmpty()) return NULL;

		ILPCSTR szPacketName = m_pIChartManager->GetSubGraphPacketNameString( szGraphName, p_szSubGraphName, p_nPacketIndex);
		if( szPacketName.IsEmpty()) return NULL;
		
		pPacket = m_pIDefaultPacketManager->GetPacket( szPacketName);
	}
	if( !pPacket) return NULL;

	IDoubleList *pDataList = pPacket->GetDataList();
	pPacket->Release();
	if( !pDataList) return NULL;

	CList<double, double> *pDblList = new CList<double, double>;
	IDblList *pIDblList = CIWDblList::CreateObject( pDblList, TRUE);
	double dData = 0.0;
	POSITION posDoubleData = pDataList->GetHeadPosition();
	while( posDoubleData)
	{
		pDataList->GetNext( posDoubleData, dData);
		pIDblList->AddTail( dData);
	}
	pDataList->Release();

	// (2007/3/29 - Seung-Won, Bae) Manage Data for Temporary Packet Data.
	if( !strcmp( p_szChartName, _MTEXT( C2_BUY_SELL_OVERHANGING_SUPPLY))
		&& !strcmp( p_szSubGraphName, _MTEXT( C2_BUY_SELL_OVERHANGING_SUPPLY)))
	{
		double dData = 0.0;
		if( 1 == p_nPacketIndex)
		{
			// (2007/4/10 - Seung-Won, Bae) Check Data Count.
			if( 0 < pIDblList->GetCount()) pIDblList->RemoveHead( dData);
		}
		else
		{
			// (2007/4/10 - Seung-Won, Bae) Check Data Count.
			if( 1 < pIDblList->GetCount()) 
			{
				pIDblList->RemoveTail( dData);
				pIDblList->RemoveTail( dData);
			}
		}
	}

	return pIDblList;
}
