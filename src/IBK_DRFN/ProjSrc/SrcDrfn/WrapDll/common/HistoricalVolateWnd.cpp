// HistoricalVolateWnd.cpp : implementation file
//

#include "stdafx.h"
#include "HistoricalVolateWnd.h"
#include "NetworkDefine.h"
#include "BaseNetworkModule.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistoricalVolateWnd

CHistoricalVolateWnd::CHistoricalVolateWnd()
{
	RegisterWndClass(AfxGetApp()->m_hInstance);

	m_pFONetwork	= NULL;

	m_bSuccess		= TRUE;
	
	//20080304 이문수 >>
	//m_pPastDataOfUnderlyingAssets = NULL;	
	m_pNetworkWnd		= NULL;
	//20080304 이문수 <<
}

//20080304 이문수 >>
CHistoricalVolateWnd::CHistoricalVolateWnd(CWnd * pNetworkWnd)
{
	RegisterWndClass(AfxGetApp()->m_hInstance);

	m_pFONetwork	= NULL;

	m_bSuccess		= TRUE;
	
	//20080304 이문수 >>
	//m_pPastDataOfUnderlyingAssets = NULL;
	m_pNetworkWnd		= pNetworkWnd;	
	//20080304 이문수 <<
}
//20080304 이문수 <<

CHistoricalVolateWnd::~CHistoricalVolateWnd()
{
}


BEGIN_MESSAGE_MAP(CHistoricalVolateWnd, CWnd)
	//{{AFX_MSG_MAP(CHistoricalVolateWnd)
	ON_WM_CREATE()
	ON_WM_COPYDATA()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RECEIVE_BASEDATA,     OnReceiveBaseData)
	ON_MESSAGE(WM_FO_RECEIVE_DATA,		OnFOReceiveData)
	ON_MESSAGE(WM_USER,					OnUser)
	ON_MESSAGE(WMM_GET_THIS_WND_PTR,	OnGetThisWndPtr)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHistoricalVolateWnd message handlers
BOOL CHistoricalVolateWnd::RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.lpszClassName	= "__HISTORICAL_WND__";
	wc.hInstance		= hInstance;
	wc.lpfnWndProc		= ::DefWindowProc;
	wc.hCursor			= ::LoadCursor(NULL,IDC_ARROW);
	wc.hIcon			= 0;
	wc.lpszMenuName		= NULL;	
	wc.hbrBackground	= (HBRUSH)::GetStockObject(WHITE_BRUSH);	
	wc.style			= CS_GLOBALCLASS | CS_DBLCLKS;
	wc.cbClsExtra		= 0;
	////////////////////////////////////////
	wc.cbWndExtra		= sizeof(HWND);

	return (::RegisterClass(&wc) != 0);
}

//20080304 이문수 >>
void CHistoricalVolateWnd::SetNetworkWnd(CWnd * pNetworkWnd)
{
	m_pNetworkWnd = pNetworkWnd;
}
//20080304 이문수 <<

BOOL CHistoricalVolateWnd::CreateWnd(int nID, CWnd * pParentWnd)
{
	return Create("__HISTORICAL_WND__", "HISTORICAL_WND", WS_CHILD , CRect(0,0,0,0), pParentWnd, nID);
}

int CHistoricalVolateWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	

	//20080304 이문수 >>
	CWnd * pNetworkWnd = this;
	if(m_pNetworkWnd) pNetworkWnd = m_pNetworkWnd;	
	m_pFONetwork = new CFONetworkModule(this, pNetworkWnd);
	//m_pFONetwork = new CFONetworkModule(this);
	//20080304 이문수 <<
	
	m_StrategyCalc.SetBaseNetworkModule(m_pFONetwork->m_pBaseWnd);

	return 0;
}

long CHistoricalVolateWnd::OnReceiveBaseData(WPARAM wParam, LPARAM lParam)
{	
	//CStringArray strCodeArray;
	CDataArray DataArray;
	int i,nSize= m_pFONetwork->m_pBaseWnd->m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyingInfo = m_pFONetwork->m_pBaseWnd->m_UnderlyingAssetsInfoArray.GetAt(i);
		CDataSet * pNewSet = new CDataSet;


		//20080327 이문수 >>
		pNewSet->m_strCode = pUnderlyingInfo->m_strShortCode;
		//if(pUnderlyingInfo->m_strUnderlyingAssetsCode.GetLength() >= 3)
		//	pNewSet->m_strCode = pUnderlyingInfo->m_strUnderlyingAssetsCode.Mid(1,2);
		//else
		//	pNewSet->m_strCode = pUnderlyingInfo->m_strUnderlyingAssetsCode;
		//20080327 이문수 <<
		
		DataArray.Add(pNewSet);		
	}

	//20080304 이문수 >>
	//m_pFONetwork->RequestPriceProgressData(strCodeArray, NULL, NULL,300, 'I');
	m_pFONetwork->RequestIVDate(DataArray);

	for(i = 0 ; i < DataArray.GetSize(); i++) delete DataArray.GetAt(i);
	//20080304 이문수 <<

	return 0L;
}

long CHistoricalVolateWnd::OnFOReceiveData(WPARAM wParam, LPARAM lParam)
{
	if(!lParam)
	{
		m_bSuccess = FALSE;
		return 0L;
	}

	if(wParam == IV_DATE_INFO)
	{		
		int i;
		for(i = 0 ; i < m_IVItemArray.GetSize(); i++)delete m_IVItemArray.GetAt(i);
		m_IVItemArray.RemoveAll();

		CIV4ItemArray * pIVItemArray = (CIV4ItemArray *)lParam;
		for(i = 0 ; i < pIVItemArray->GetSize(); i++)
		{
			CIV4ItemSet * pIVSet	= pIVItemArray->GetAt(i);
			CIV4ItemSet * pNewIVSet = new CIV4ItemSet;
			*pNewIVSet				= *pIVSet;
			m_IVItemArray.Add(pNewIVSet);
		}

		m_bSuccess = TRUE;
	}
	return 0L;
}

void CHistoricalVolateWnd::OnDestroy() 
{
	CWnd::OnDestroy();

	int i;
	for( i = 0 ; i < m_IVItemArray.GetSize(); i++)
		delete m_IVItemArray.GetAt(i);
	m_IVItemArray.RemoveAll();

	if(m_pFONetwork) delete m_pFONetwork;
}


double CHistoricalVolateWnd::CalcHisVolate(CString strUnderlyingAssetsCode, int nCount)
{
	int nItemCount = m_IVItemArray.GetSize();
	if(nItemCount <= 0) return -1;
	
	int nTradeDays, i, nSize = m_pFONetwork->m_pBaseWnd->m_BaseDataSet.m_TradeDateInfoArray.GetSize();
	
	if(nSize <= 0)
		nTradeDays = 245;
	else
	{	
		CTradeDateInfo * pTradeDateInfo = m_pFONetwork->m_pBaseWnd->m_BaseDataSet.m_TradeDateInfoArray.GetAt(0);
		nTradeDays = pTradeDateInfo->m_nTradeDate;
	}
	
	for(i = 0 ; i < nItemCount ; i++)
	{
		CIV4ItemSet * pIVItemSet = m_IVItemArray.GetAt(i);
		if(pIVItemSet->m_strCode == strUnderlyingAssetsCode)
		{
			int j,nDateCount = pIVItemSet->m_IVDateArray.GetSize();
			
			if(nDateCount < nCount + 1) return -1;

			double * pPastDataArray = new double[nCount + 1];
			for(j = 0 ;j <= nCount + 1 && j < pIVItemSet->m_IVDateArray.GetSize(); j++)
			{
				CIV4DateSet * pDateSet = pIVItemSet->m_IVDateArray.GetAt(j);
				if(j >= 1)
					pPastDataArray[j-1] = atof(pDateSet->m_strClose);				
			}
			double dHisVolate = m_StrategyCalc.CalcHisVolate(pPastDataArray, nCount + 1, nCount, nTradeDays);
			delete [] pPastDataArray;
			return dHisVolate;
		}
	}
	return -1;
}

//20080317 이문수 >>
BOOL CHistoricalVolateWnd::GetCloseData(CString strUnderlyingAssetsCode, CArray < double, double > * pCloseDataArray)
{
	if(!m_pFONetwork || !m_pFONetwork->m_pBaseWnd) return FALSE;

	CString strUnderlyingShortCode = m_pFONetwork->m_pBaseWnd->GetUnderlyingAssetsShortCode(strUnderlyingAssetsCode);
	
	int i, nItemCount = m_IVItemArray.GetSize();
	if(nItemCount <= 0) return FALSE;	
	
	for(i = 0 ; i < nItemCount ; i++)
	{
		CIV4ItemSet * pIVItemSet = m_IVItemArray.GetAt(i);
		if(pIVItemSet->m_strCode == strUnderlyingShortCode)
		{
			int j,nDateCount = pIVItemSet->m_IVDateArray.GetSize();
			
			if(nDateCount <= 0) return FALSE;			
			
			for(j = 0 ;j < nDateCount ; j++)
			{
				CIV4DateSet * pDateSet = pIVItemSet->m_IVDateArray.GetAt(j);				
				pCloseDataArray->Add( atof(pDateSet->m_strClose) );
			}
			
			return TRUE;
		}
	}
	return FALSE;
}
//20080317 이문수 <<

//20080304 이문수 >>
LONG CHistoricalVolateWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	BYTE bt = LOBYTE(LOWORD(wParam));
	switch(bt)
	{
	case DLL_OUB:					// Tr수신
		m_pFONetwork->ReceiveData(wParam, lParam);
		break;

	case DLL_ALERT:					// 리얼 수신
		break;		
	case DLL_SETPAL:
		break;
	case DLL_TRIGGER:		// 공유(화면 떠있을 때)
		break;
	case DLL_DOMINO:		// 화면 열리면서 데이터 공유 받을 때
		break;
	case DLL_GUIDE:
		return 1;
	case DLL_SETFONTx:
		break;
	}

	return ret;
}

long CHistoricalVolateWnd::OnGetThisWndPtr(WPARAM wParam, LPARAM lParam)
{
	return (long)this;
}
//20080304 이문수 <<