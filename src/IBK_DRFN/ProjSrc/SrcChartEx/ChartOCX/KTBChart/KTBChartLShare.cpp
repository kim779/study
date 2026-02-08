// KTBChartLShare.cpp: implementation of the Local Share Code
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"

#include "../Include_Chart/DLL_Load/AddInManager.h"	// for CAddInManager
#include "KTBChartCtl.h"							// for CKTBChartCtrl
#include "LocalShareInfoList.h"						// for CLocalShareInfo
#include "LSharedData.h"							// for LPST_LSHAREDATA

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// (2004.11.29, 배승원, 분석) SetAllProperties2()에서 호출되어, 설정된 공유 설정 String인
//		m_strLocalSharedList를 m_ListLShare로 구성하는 작업을 처리한다.
//		특히 이때, 초기 공유값 확인과 공유 변경 Notify를 수신을 요청토록 한다.
BOOL CKTBChartCtrl::InitLSharedData()
{
	// 0. 먼저 Base Desk Pointer를 확인한다.
	if( !m_pBaseDesk) return FALSE;
	if( !m_plLocalShareInfoList) return FALSE;

	// 1. Local 공유 Manager의 Interface Pointer를 구한다.
	// (2004.11.29, 배승원) 변경된 공유 전달 방식이 Message 방식을 이용하면서,
	//		이전의 Interface 통보 방식은 이용되지 않고 있다.
//	m_pILSharedData = ( ILSharedData *)m_pBaseDesk->GetLSharedData();
//	if( !m_pILSharedData) return FALSE;

	// 2. 공유 설정 Data String을 확인한다.
	if( m_strLocalSharedList.IsEmpty()) return TRUE;

	// 3. 각 공유 설정 Item을 추출하여 m_ListLShare로 관리한다.
	// 3.0 먼저 공유 설정 Data String을 복사한다.
	CString strLocalShareSetting;
	char *szLocalShareSetting = strLocalShareSetting.GetBufferSetLength( m_strLocalSharedList.GetLength());
	strcpy( szLocalShareSetting, m_strLocalSharedList);
	while( szLocalShareSetting && *szLocalShareSetting)
	{
		// 3.1 다음 공유 설정 Item을 찾아 분리한다.
		char *szNextLShareItem = strchr( szLocalShareSetting, ';');
		if( szNextLShareItem) *szNextLShareItem++ = '\0';

		// 3.2 공유 설정 Item의 마지막 Byte를 추출하여 Code로 관리한다.
		// 3.2.1 공유 설정 Item String의 길이를 구한다.
		int nItemLen = strlen( szLocalShareSetting);
		// 3.2.2 공유 설정 Item String은 2 Byte 이상이어야 한다.
		if( 2 <= nItemLen)
		{
			// 3.2.3 공유 설정 Item의 설정 Code를 구한다.
			//	(마지막 Byte에 Ascii 숫자 Character로 되어 있다.)
			int nSettingCode = szLocalShareSetting[ nItemLen - 1] - '0';
			// 3.2.4 설정 Code를 Clear시켜 정확한 공유명을 확인할 수 있도록 한다.
			szLocalShareSetting[ nItemLen - 1] = '\0';

			// 3.3 공유 설정정보를 Local Share Info List에 등록한다.
			CLocalShareInfo *pLocalShareInfo = m_plLocalShareInfoList->AddLocalShareInfo( szLocalShareSetting, nSettingCode);
			if( !pLocalShareInfo) MessageBox( CString( "Mulitple Local Share Setting!\n\nLocal Share Name : ") + szLocalShareSetting, "Chart OCX");
			// (2004.11.29, 배승원) 변경된 공유 전달 방식이 Message 방식을 이용하면서,
			//		이전의 Interface 통보 방식은 이용되지 않고 있다.
//			else
//			{
//				// 3.4 공유 설정 Item에 Start 공유와 Get 공유를 처리한다.
//				if( m_pILSharedData)
//				{
//					if( pLocalShareInfo->m_bStart) 
//					{
//						CString strLSharedData;
//						if( !m_pILSharedData->GetData( szLocalShareSetting, strLSharedData)) strLSharedData.Empty();
//						pLocalShareInfo->m_strLocalSharedData = strLSharedData;
//					}
//					if( pLocalShareInfo->m_bGet)
//						m_pILSharedData->DAdvise( &m_xLAdviseSink, szLocalShareSetting);
//				}
//			}
		}

		// 3.5 다음 공유 설정 Item을 처리할 수 있도록 한다.
		szLocalShareSetting = szNextLShareItem;
	}

	return TRUE;
}

// Local Shared Data Receiving Routine
LRESULT CKTBChartCtrl::Rmsg_LSharedChaned(WPARAM wParam, LPARAM lParam)
{
	if( !m_plLocalShareInfoList) return 0L;

	LPST_LSHAREDATA pSHD = (LPST_LSHAREDATA)wParam;
	if( !pSHD) return 0L;

	CString szShareName = pSHD->m_szShareName;
	CString szData;
	if( pSHD->m_nType == LSDT_DWORD) szData.Format("%ld", pSHD->m_dwShareData);
	else szData = pSHD->m_szShareData;

	LRESULT lResult = m_plLocalShareInfoList->SetLocalSharedData( szShareName, szData);

	// (2005/11/21 - Seung-Won, Bae) Notify Local Shared Data Changed to AddIn
	if( m_pAddInManager) m_pAddInManager->OnLSharedDataChange( szShareName, szData);

	return lResult;
}

// (2004.11.29, 배승원) 변경된 공유 전달 방식이 Message 방식을 이용하면서,
//		이전의 Interface 통보 방식은 이용되지 않고 있다.
//BEGIN_INTERFACE_MAP(CKTBChartCtrl, COleControl)
//  INTERFACE_PART(CKTBChartCtrl, IID_ILAdviseSink, LAdviseSink)
//END_INTERFACE_MAP()
//STDMETHODIMP_(ULONG) CKTBChartCtrl::XLAdviseSink::AddRef()
//{
//	METHOD_PROLOGUE(CKTBChartCtrl, LAdviseSink)
//	ULONG nResult = (ULONG)pThis->ExternalAddRef();
//	return nResult;
//}
//STDMETHODIMP_(ULONG) CKTBChartCtrl::XLAdviseSink::Release()
//{
//	METHOD_PROLOGUE(CKTBChartCtrl, LAdviseSink)
//	ULONG nResult = (ULONG)pThis->ExternalRelease();
//	return nResult;
//}
//STDMETHODIMP CKTBChartCtrl::XLAdviseSink::QueryInterface(REFIID iid, LPVOID* ppvObj)
//{
//	METHOD_PROLOGUE(CKTBChartCtrl, LAdviseSink)
//	HRESULT hr = (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
//	return hr;
//}
//STDMETHODIMP CKTBChartCtrl::XLAdviseSink::OnDataChange(LPCSTR szSharedName, LPCSTR szData)
//{
//	METHOD_PROLOGUE(CKTBChartCtrl, LAdviseSink)
//	return pThis->ParedShareDataChange(szSharedName, szData);
//}
