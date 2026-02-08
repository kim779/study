// DataListViewMngWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Define.h"
#include "DataListViewMngWnd.h"

#include "ChartDataListViewerAddIn.h"
#include "ChartDataListViewerAddInImp.h"

#include "DlgDataListViewer.h"				// for CDlgDataListViewer
#include "../Include_Chart/Dll_Load/IMetaTable.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataListViewMngWnd

CDataListViewMngWnd::CDataListViewMngWnd()
{
	m_strClassName = AfxRegisterWndClass(CS_DBLCLKS);
	m_pDataListViewer = NULL;
	m_bIsUpdateReady  = FALSE;
}

CDataListViewMngWnd::~CDataListViewMngWnd()
{
}


BEGIN_MESSAGE_MAP(CDataListViewMngWnd, CWnd)
	//{{AFX_MSG_MAP(CDataListViewMngWnd)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDataListViewMngWnd message handlers

BOOL CDataListViewMngWnd::Create(CWnd* pParentWnd, UINT nID) 
{
	return CWnd::Create(m_strClassName, "", WS_CHILD, CRect(0,0,0,0), pParentWnd, nID);
}

void CDataListViewMngWnd::OnDestroy() 
{
	if( m_pDataListViewer)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		//AfxSetResourceHandle(ML_LANGUAGE_RES);

		DestroyDataListViewer();		 
	}

	CWnd::OnDestroy();	
	
}

void CDataListViewMngWnd::CreateDataListView()
{
	CRect rctPrePosition;
	rctPrePosition.SetRectEmpty();
	if( m_pDataListViewer && IsWindow(m_pDataListViewer->GetSafeHwnd()))
		m_pDataListViewer->GetWindowRect( &rctPrePosition);

	DestroyDataListViewer();	

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	CString szTitle, szPacketNames, szPacketNames2, strHorzScale;
//	CSubGraphViewDataList pViewDataList;
//	CViewData viewData;
//	// 20081028 박동관 >>
//	//m_pMainBlock->GetViewDataListInAllBlock(viewData);
//	m_pMainBlock->GetViewDataListInAllBlock(m_strRQ, viewData);
//	// 20081028 박동관 <<
//	if(viewData.GetColumnViewDataCount() <= 0)
//		return;
//
//	for(int i = 0; i < m_pMainBlock->GetColumnCount(); i++){
//		for(int j = 0; j < viewData.GetRowViewDataCount(i); j++){
//			CRowViewData& rowViewData = viewData.GetAt(i, j);
//			//nami 9.2
//			//현재는 가로스케일을 하나만 보이게 하고 있다.
//			CString strData = rowViewData.GetHorzScaleName();
//			
//			if(!strData.IsEmpty()) 
//				strHorzScale = strData;
//			//nami end
//			for(int k = 0 ; k < rowViewData.GetSubGraphViewDataCount(); k++){
//				POSITION listPos = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().FindIndex(k);
//				if(listPos){
//					CSubGraphViewData pSubGraph = rowViewData.GetSubGraphViewDataList().GetSubGraphViewDataList().GetNext(listPos);
//					szTitle += pSubGraph.GetTitle() + ";" ;
//					szPacketNames += pSubGraph.GetPacketNames() + ";" ;
//					szPacketNames2 += pSubGraph.GetPacketNames() ;
//				}
//			}
//		}
//	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	IChartManager *	pIChartManager	= m_pAddInImp->GetIChartManager();
	if(!pIChartManager) return;
	IChartOCX *		pIChartOCX		= m_pAddInImp->GetIChartOCX();
	if(!pIChartOCX)		return;
	IPacketManager *pIPacketManager = m_pAddInImp->GetIPacketManager();
	if(!pIPacketManager)return;

	ILPCSTR szTitle, szPacketNames; 
	CString	strTitle, strPacketNames, strTitleList, strPacketNameList, strPacketNameListPerBlock, strRQList, strRQ;
	ILPCSTR szSelectedRQ = pIChartOCX->GetCurrentRQ();
	strRQ = szSelectedRQ;
	POSITION psnViewData = pIChartManager->GetSubGraphViewData_First( strRQ);
	ILPCSTR szHorzScale = pIChartManager->GetSubGraphViewData_HorzScaleNameString( psnViewData);
	CString strHorzScale( szHorzScale);
	while( psnViewData)
	{
		pIChartManager->GetSubGraphViewData_TitleAndPacketNameString( psnViewData, szTitle, szPacketNames);
		strTitle = szTitle;
		strPacketNames = szPacketNames;
		strTitleList += strTitle + ";";
		strPacketNameList += strPacketNames;
		strPacketNameListPerBlock += strPacketNames + ";";
		
		int nNextPacketIndex = strPacketNames.Find( ',', 0);
		nNextPacketIndex = strPacketNames.Find( ',', nNextPacketIndex+1);
		while(nNextPacketIndex>=0)
		{
			nNextPacketIndex = strPacketNames.Find( ',', nNextPacketIndex+1);
			strRQList += strRQ + ";";
		}
		strRQList += strRQ + ";";

		pIChartManager->GetSubGraphViewData_Next( psnViewData);
	}
	
	ILPCSTR szUserPath = pIChartOCX->GetUserFilePathString();
	m_pDataListViewer = new CDlgDataListViewer( this, strTitleList, strHorzScale, strPacketNameListPerBlock, 
		strPacketNameList, szUserPath, strRQList, FALSE, m_pAddInImp, pIChartOCX->GetOcxHwnd(), pIChartOCX->GetChartMode());

	m_pDataListViewer->SetPacketListAddr( pIPacketManager);
	m_pDataListViewer->Create( IDD_DATALIST_VIEWER, this);
	if( !rctPrePosition.IsRectEmpty()) 
		m_pDataListViewer->SetWindowPos(NULL, rctPrePosition.left, rctPrePosition.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	m_pDataListViewer->ShowWindow( SW_SHOW);

	m_bIsUpdateReady = TRUE;
}

void CDataListViewMngWnd::DestroyDataListViewer()
{
	if(m_pDataListViewer)
	{
		if(IsWindow(m_pDataListViewer->GetSafeHwnd()))
			m_pDataListViewer->DestroyWindow();
		delete m_pDataListViewer;
		m_pDataListViewer = NULL;
	}
}

void CDataListViewMngWnd::RealUpdate()
{
	if(m_pDataListViewer && IsWindow(m_pDataListViewer->GetSafeHwnd()))
	{
		if(IsUpdateReady())
			m_pDataListViewer->RealUpdate();
	}
}

void CDataListViewMngWnd::SetAddInImp(CChartDataListViewerAddInImp *	pAddInImp)
{
	m_pAddInImp = pAddInImp;
}

CChartDataListViewerAddInImp *	CDataListViewMngWnd::GetAddInImp()
{
	return m_pAddInImp;
}

BOOL CDataListViewMngWnd::IsUpdateReady()
{
	return m_bIsUpdateReady;
}

void CDataListViewMngWnd::Update()
{
	m_bIsUpdateReady = FALSE;
	PostMessage(UDM_UPDATE);
}

LRESULT CDataListViewMngWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == UDM_DATALISTVIEWER_CLOSE)	
	{
		DestroyDataListViewer();		
		return 1L;
	}
	else if(message == UDM_UPDATE)
	{
		if(!IsUpdateReady())
		{
			CreateDataListView();
		}
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}
