// BaseNetworkModule.cpp : implementation file
//

#include "stdafx.h"
#include "networkdefine.h"
#include "BaseNetworkModule.h"
#include "NetworkModule.h"
#include "FONetworkModule.h"
#include "global.h"
#include "CommonFrameWorkWnd.h"
#include "exportfunction.h"
#include <math.h>
#include "ClosingNoticeDlg.h"
#include "../../h/axisfire.h"
#include "../../control/fx_misc/miscType.h"

//#include "SetupSheet.h"
#include "../../h/jmcode.h"	// LJP 20080130

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseNetworkModule
#define	TIMER		60000

#define	TEST_TIMER			1
#define	TEST_TIMER_INTERVAL	30000

#define WM_MANAGE	(WM_USER + 1001)
#define DLG_SET_DATA	99999

#define	OPCODE_LEN	8

CBaseNetworkModule::CBaseNetworkModule(CWnd* pParent)
{
	RegisterWndClass(AfxGetApp()->m_hInstance);

	m_pNetwork = NULL;

	m_bReceiveBaseData = FALSE;

	m_bEnableNetwork = TRUE;

	m_nTradeTimeType = 1;//장종료시간 여부.(1-장중, 2-장종료)

	m_pCommonFrameWork = NULL;

	m_pClosingDlg = NULL;

	m_pHistoricalWnd = NULL;

	m_pFrameWnd = pParent;

	m_strATM = "";

	m_pPastDataOfUnderlyingAssets = NULL;

	m_nBizDayCount = 0;

	m_hVolateSelDlg = NULL;

	m_strKP200Time  = "";//장전에는 데이터가 안들어오는 여부 확인

	//20080314 이문수 >>
	m_dTimeLoss      = 0.;
	//20080314 이문수 <<
}

CBaseNetworkModule::~CBaseNetworkModule()
{
}


BEGIN_MESSAGE_MAP(CBaseNetworkModule, CWnd)
	//{{AFX_MSG_MAP(CBaseNetworkModule)
	ON_WM_CREATE()
	ON_WM_COPYDATA()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_DIALOGDESTORYED,	OnDialogDestroyed)	
	ON_MESSAGE(WM_CLOSE_BASENETWORKMODULE,	OnCloseBaseNetworkModule)	
	ON_MESSAGE(WM_SHOW_CLOSE_DLG,	OnShowCloseDlg)
	ON_MESSAGE(WMM_GET_THIS_WND_PTR,OnGetBaseNetworkModule)	
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

//	ON_MESSAGE(WMU_RTM_PACKET,		OnReceiveRTM)

long CBaseNetworkModule::OnGetBaseNetworkModule(WPARAM wParam, LPARAM lParam)
{
	return (long)this;
}

/////////////////////////////////////////////////////////////////////////////
// CBaseNetworkModule message handlers
BOOL CBaseNetworkModule::RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.lpszClassName	= "HADT_BASE_NETWORK_MODULE"; // 하나대투 베이스 네트윅 모듈
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

BOOL CBaseNetworkModule::CreateBaseNetworkWnd(int nID, CWnd * pParentWnd)
{
	return Create("HADT_BASE_NETWORK_MODULE","HADT_FO_NETWORK_WND", WS_CAPTION|WS_CHILD |WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS ,
		CRect(0,0,0,0), pParentWnd, nID);
}

BOOL CBaseNetworkModule::RequestBaseData()
{
	if(!m_pNetwork) return FALSE;

	m_strRqCodeArray.RemoveAll();

	m_pNetwork->RequestBaseData();

	return TRUE;
}

int CBaseNetworkModule::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	::SetFactorialArray();

	// LJP 20080130 Start -->
	m_strHomeDir = Variant(homeCC);					// client root 디렉토리
	m_strUser	 = Variant(nameCC);	
	m_strId		 = Variant(userCC);

	ReadFutCode();
	ReadOptCode();
	ReadSOptCode();

	SortSOptItem();	

	m_BaseDataSet.m_pRTDataArray = &m_RTDataArray;

	m_pNetwork = new CNetworkModule(this, m_pFrameWnd);
	m_pNetwork->SetMode(BASE_NETWORK_MODULE_MODE);

	m_pCommonFrameWork = new CCommonFrameWorkWnd;
	m_pCommonFrameWork->SetAppPath(m_strHomeDir, m_strUser);
	m_pCommonFrameWork->CreateWnd(10000, this);

	RequestBaseData();


	//TEST
#ifdef __TEST__
	SetTimer(TEST_TIMER, TEST_TIMER_INTERVAL, NULL);
#endif
	return 0;
}

//20080316 이문수 >>
BOOL CBaseNetworkModule::RequestData(CString strCode, HWND hRqWnd)
{
	if(!m_pNetwork || !m_pNetwork->m_pKeyManager) return FALSE;


	{
		m_pNetwork->m_bAutoMakeKey = FALSE;
		m_pNetwork->m_nKey = m_pNetwork->m_pKeyManager->GetKey(strCode);
		
		for(POSITION pos = m_KeyToWndList.GetHeadPosition(); pos ; )
		{
			CKeyToWndInfo Info = m_KeyToWndList.GetNext(pos);
			if(Info.m_nKey == m_pNetwork->m_nKey)
			{
				for(int i = 0 ; i < Info.m_hWndArray.GetSize(); i++ )
				{
					HWND hWnd = (HWND)Info.m_hWndArray.GetAt(i);
					if(hWnd == hRqWnd) //이전 요청을 아직 못받은 것으로 판단하고 실패.
						return FALSE;
				}	

				//키 중복은 있지만 윈도우핸들 중복은 없는 경우.
				Info.m_hWndArray.Add((UINT)hRqWnd);
				break;
			}
		}

		//키 중복이 없는 경우.
		if(!pos)
		{		
			CKeyToWndInfo Info(m_pNetwork->m_nKey, hRqWnd);
			m_KeyToWndList.AddTail(Info);
		}
	}

	int nResult = m_pNetwork->RequestData(strCode);	
	m_pNetwork->m_bAutoMakeKey = TRUE;
	return nResult;
}
//20080316 이문수 <<

BOOL CBaseNetworkModule::RequestData()
{
	if(!m_pNetwork) return FALSE;	
	if(m_strRqCodeArray.GetSize() <= 0) return FALSE;

	CString strCode = m_strRqCodeArray.GetAt(0);

	int nCodeType = m_pNetwork->GetCodeType(strCode); // 종목코드종류 구분
	
	//20080316 이문수 >>
	/*
	if(nCodeType != KP200_CODE && nCodeType != KOSPI_CODE && m_pNetwork->m_pKeyManager)
	{
		m_pNetwork->m_bAutoMakeKey = FALSE;
		m_pNetwork->m_nKey = m_pNetwork->m_pKeyManager->GetKey(strCode);
		
		for(POSITION pos = m_KeyToWndList.GetHeadPosition(); pos ; )
		{
			CKeyToWndInfo Info = m_KeyToWndList.GetNext(pos);
			if(Info.m_nKey == m_pNetwork->m_nKey)
			{
				for(int i = 0 ; i < Info.m_hWndArray.GetSize(); i++ )
				{
					HWND hWnd = (HWND)Info.m_hWndArray.GetAt(i);
					if(hWnd == GetSafeHwnd()) //이전 요청을 아직 못받은 것으로 판단하고 실패.
						return FALSE;
				}	

				//키 중복은 있지만 윈도우핸들 중복은 없는 경우.
				Info.m_hWndArray.Add((UINT)GetSafeHwnd());
				break;
			}
		}

		//키 중복이 없는 경우.
		if(!pos)
		{		
			CKeyToWndInfo Info(m_pNetwork->m_nKey, GetSafeHwnd());
			m_KeyToWndList.AddTail(Info);
		}
	}
	int nResult = m_pNetwork->RequestData(strCode);	
	m_pNetwork->m_bAutoMakeKey = TRUE;
	return nResult;
	*/
	return m_pNetwork->RequestData(strCode);	
	//20080316 이문수 <<
}

BOOL CBaseNetworkModule::RequestKP200Data()
{
	if(!m_pNetwork) return FALSE;
	return m_pNetwork->RequestKP200Data();
}

BOOL CBaseNetworkModule::ReceiveAverageIV(WPARAM wParam, LPARAM lParam)
{
// 	CAverageIV AveIV;
// 	if(m_pNetwork->ReceiveData(wParam, lParam, AveIV))
// 	{
// 		m_AverageIV = AveIV;				
// 		m_bEnableNetwork = TRUE;
// 		
// 		if(atoi(m_AverageIV.m_strType) != 2) 
// 			SetTimer(0, TIMER, NULL);
// 	}
// 	else
// 	{
// 		m_bEnableNetwork = TRUE;
// 		SetTimer(0, TIMER, NULL);
// 	}
	return TRUE;
}

BOOL CBaseNetworkModule::ReceiveData(WPARAM wParam, LPARAM lParam)
{
	if(!m_pNetwork) return FALSE;
	if(m_strRqCodeArray.GetSize() <= 0) return FALSE;
	CString strCode = m_strRqCodeArray.GetAt(0);

	CDataSet * pDataSet = new CDataSet;
	pDataSet->m_strCode = strCode;
	if(!m_pNetwork->ReceiveData(wParam, lParam, pDataSet))
	{
		delete pDataSet;
		return FALSE;
	}
	m_RTDataArray.Add(pDataSet);
	m_strRqCodeArray.RemoveAt(0);

	return TRUE;
}

BOOL CBaseNetworkModule::RqItem(BOOL bStart)
{
	if(bStart) // Kospi, Kp200, 선물 4종목, 주식기초자산 종목 시세 요청
	{
		int i;
		
		CString strKP    = KOSPICODE;//kospi
		m_strRqCodeArray.Add(strKP);

		CString strKP200 = KP200CODE; //kp200
		m_strRqCodeArray.Add(strKP200);

		for(i = 0 ; i < min(4, m_FutureMonthInfoArray.GetSize()); i++) //선물 4종목
		{
			CItemDataInfo * pInfo = m_FutureMonthInfoArray.GetAt(i);
			m_strRqCodeArray.Add(pInfo->m_strCode);
		}
		for(i = 0 ; i < m_UnderlyingAssetsInfoArray.GetSize(); i++) //주식 종목들.
		{
			CUnderlyingAssetsInfo * pUnder = m_UnderlyingAssetsInfoArray.GetAt(i);
			if(pUnder->m_strUnderlyingAssetsCode != KP200CODE)
				m_strRqCodeArray.Add(pUnder->m_strUnderlyingAssetsCode);
		}

		return RequestData();		
	}
	else
	{	
		if(m_strRqCodeArray.GetSize() > 0)
		{
			return RequestData();
		}
		else			
			return  FALSE;
	}
}

void CBaseNetworkModule::AddRTData()
{
	if(!m_pNetwork) return;
	m_pNetwork->AddRTData(m_RTDataArray);	
}

void CBaseNetworkModule::SetWnd(CWnd * pWnd)
{
	if(m_bReceiveBaseData)
	{	
		pWnd->PostMessage(WM_RECEIVE_BASEDATA, 0, (LPARAM) &m_BaseDataSet);
		pWnd->PostMessage(WM_DISPLAY_TODAY, 0, (LPARAM)&m_BaseDataSet);
	}
	else
		m_WndArray.Add(pWnd);
}

BOOL CBaseNetworkModule::ReadFutCode()
{
	if (m_FutureMonthInfoArray.GetSize() > 0)
		return true;

	char buf[1024];
	int	res;
	DWORD	dw;
	int	bufsize;
	struct	fjcode	*pFjCode;
	
	CString path;
	path.Format("%s\\%s\\%s", m_strHomeDir, "tab", "fjcode.dat");

	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		::MessageBox(NULL, "선물 화일을 읽을 수 없습니다!", "", MB_OK|MB_ICONERROR);
		CloseHandle(fh);
		return false;
	}
	
	bufsize = sizeof(struct fjcode);
	
	for (int ii = 0; ; ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;

		pFjCode = (struct fjcode *)buf;

		CItemDataInfo * pFutureInfo = new CItemDataInfo;
		pFutureInfo->m_strCode = CString(pFjCode->cod2, FCodeLen);
		pFutureInfo->m_strCode.TrimRight();
		pFutureInfo->m_strName = CString(pFjCode->hnam, FNameLen);
		pFutureInfo->m_strName.TrimRight();

		m_FutureMonthInfoArray.Add(pFutureInfo);
	}

	CloseHandle(fh);

	return TRUE;
}


BOOL CBaseNetworkModule::ReadETFCode()
{
	CStdioFile file;
	CString strFile1= m_strHomeDir + "\\table\\etfstcdk.dat";
	if(!file.Open(strFile1, CFile::modeRead))
		return FALSE;

	int nPos;
	CString strLine;
	CString strMonthInfo;
	CString strCode, strName, strStrike;

	while(file.ReadString(strLine))
	{
		if((nPos = strLine.Find("=")) == -1) continue;
		strCode = strLine.Left(nPos - 1);
		strLine = strLine.Mid(nPos + 1);
		
		if((nPos = strLine.Find("=")) == -1) continue;
		strName = strLine.Left(nPos - 1);
		
		strName.TrimLeft(); strName.TrimRight();
		strCode.TrimLeft(); strCode.TrimRight();

		CItemDataInfo * pItem= new CItemDataInfo;
		pItem->m_strCode = strCode;
		pItem->m_strName = strName;

		m_ETFItemInfoArray.Add(pItem);
	}
	file.Close();

	return TRUE;
}


BOOL CBaseNetworkModule::ReadSOptCode()
{
	CFile	file;
	struct  pjcode  PJCode;
	CString path;
	path.Format("%s\\%s\\%s", m_strHomeDir, "tab", "pjcode.dat");

	CUnderlyingAssetsInfo * pUnderlyingAssetsInfo = NULL;

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
		AfxMessageBox("주식옵션 파일을 읽을 수 없습니다.");
		return false;
	}

// struct pjcode {
// 	char	gubn;		// 1:KOSPI, 2:KOSDAQ
// 	char	tjgb[2];	// 거래대상종목구분(기초자산코드)
// 	char	snam[20];	// 거래대상종목명
// 
// 	char	codx[8];	// 단축코드
// 	char	hnam[30];	// 한글명
// 	char	enam[30];	// 영문명
// 	char	gcod[12];	// 기초자산종목코드
// 	char	mont[4];	// 월물
// 	char	atmx;		// ATM종목여부 1:ATM종목
// 	char	hsga[8];	// 행사가
// 	char	mchk;		// 매매유형허용	// 0x01:시장가허용	// 0x02:조건부지정가허용// 0x04:최유리지정가허용
// };

	CString strUnderlyingCode, strCurAsset;
	CUnderlyingAssetsInfo * pCurUnderlyingInfo;

	CString strName, strCode, strStrike, strMonthInfo;
	int nCnt = file.GetLength() / sizeof(struct pjcode);
	for(int ii = 0; ii < nCnt; ii++)
	{
		file.Read(&PJCode, sizeof(struct pjcode));

		strUnderlyingCode = CString(PJCode.tjgb, sizeof(PJCode.tjgb));// 기초자산Short코드

		if(strCurAsset.Compare(strUnderlyingCode) != 0) // 기초자산이 변경된 경우만
		{
			strName = CString(PJCode.snam, sizeof(PJCode.snam)); // 기초자산이름(주식명)
			strCode = CString(PJCode.gcod, sizeof(PJCode.gcod)); // 기초자산코드
			strName.TrimLeft(); strName.TrimRight();
			strCode.TrimLeft(); strCode.TrimRight();
			strCode = strCode.Mid(3, 6);

			CUnderlyingAssetsInfo * pUnderlyingInfo = new CUnderlyingAssetsInfo;
			pUnderlyingInfo->m_strShortCode = strUnderlyingCode;
			pUnderlyingInfo->m_strUnderlyingAssetsCode = strCode;
			pUnderlyingInfo->m_strUnderlyingAssetsName = strName;

			m_UnderlyingAssetsInfoArray.Add(pUnderlyingInfo);

			strCurAsset = strUnderlyingCode; // 현재 기초자산코드 변경
			pCurUnderlyingInfo = pUnderlyingInfo; // 현재기초자산의 포인터 변경
		}

		strCode = CString(PJCode.codx, sizeof(PJCode.codx)); //종목코드
		strName = CString(PJCode.hnam, sizeof(PJCode.hnam)); //종목명
		strName.TrimLeft(); strName.TrimRight();
		strStrike = CString(PJCode.hsga, sizeof(PJCode.hsga)); //행사가
		strStrike.TrimLeft();strStrike.TrimRight();
// 			strTradeUnit = strLine.Right(3);
// 			strTradeUnit.TrimLeft(); strTradeUnit.TrimRight();
		strMonthInfo = CString(PJCode.mont, sizeof(PJCode.mont)); //월물

		CItemDataInfo * pOptionInfo = new CItemDataInfo;
		pOptionInfo->m_strCode = strCode;
		pOptionInfo->m_strName = strName;
		pOptionInfo->m_strStrike=strStrike;
		
		if(pCurUnderlyingInfo->m_strShortCode == strUnderlyingCode)
		{
			//거래단위...=> 주식현재가 조회시 받아서 셋팅할 것이다.
			//pCurUnderlyingInfo->m_strTradeUnit = strTradeUnit;

			for(int j = 0 ; j < pCurUnderlyingInfo->m_MonthInfoArray.GetSize(); j++)
			{
				COptionMonthInfo * pMonthInfo = pCurUnderlyingInfo->m_MonthInfoArray.GetAt(j);
				if(pMonthInfo->m_strYearMonth == strMonthInfo)
				{
					if(strCode.Left(1) == "2")
						pMonthInfo->m_CallOptionInfoArray.Add(pOptionInfo);
					else
						pMonthInfo->m_PutOptionInfoArray.Add(pOptionInfo);	
					break;
				}
			}

			// 못찾았을 경우 처리
			if(j == pCurUnderlyingInfo->m_MonthInfoArray.GetSize())
			{
				COptionMonthInfo * pMonthInfo = new COptionMonthInfo;
				pMonthInfo->m_strYearMonth = strMonthInfo;
				if(strCode.Left(1) == "2")
					pMonthInfo->m_CallOptionInfoArray.Add(pOptionInfo);
				else
					pMonthInfo->m_PutOptionInfoArray.Add(pOptionInfo);

				pCurUnderlyingInfo->m_MonthInfoArray.Add(pMonthInfo);
			}
		}
		else
			delete pOptionInfo;
	}

	return TRUE;
}

BOOL CBaseNetworkModule::ReadOptCode()
{
	int	codeN, len;
	float fPrice;
	CFile	file;
	struct  ojcodh  OJCodh;
	struct  ojcode  OJCode;
	CString path;
	path.Format("%s\\%s\\%s", m_strHomeDir, "tab", "opcode.dat");

	CUnderlyingAssetsInfo * pUnderlyingAssetsInfo = NULL;
	COptionMonthInfo * pOptionMonth[4];

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
		AfxMessageBox("선물/옵션 파일을 읽을 수 없습니다.");
		return false;
	}

	len = file.Read(&OJCodh, sizeof(struct ojcodh));
	codeN = (file.GetLength() - len) / sizeof(struct ojcode);

	for (int ii = 0; ii < 4; ii++)
	{
		pOptionMonth[ii] = new COptionMonthInfo;
		pOptionMonth[ii]->m_strYearMonth = CString(OJCodh.pjym[ii], sizeof(OJCodh.pjym[ii]));
	}

	CString	sPrice, strCode, strATM;
	BOOL bFull = FALSE;
	
	for (ii = 0; ii < codeN; ii++)
	{
		file.Read(&OJCode, sizeof(struct ojcode));
		sPrice = CString(OJCode.price, sizeof(OJCode.price));
		fPrice = float(atof(sPrice) / 100.0);
		sPrice.Format("%.1f", fPrice);
		strATM = OJCode.atmg;
		
		int nCnt = 0;

		for(int i = 0; i < 4; i++)
		{	
			strCode = CString(OJCode.call[3-i].cod2, OPCODE_LEN);
			if(strCode == "")
				continue;

			if(!bFull) 
			{
				nCnt++;
				pOptionMonth[i]->m_strShortYearMonth = strCode.Mid(3,2);
				if(nCnt == 4)
					bFull = TRUE;
			}

			CItemDataInfo * pOptionInfo = new CItemDataInfo;
			
			pOptionInfo->m_strCode = strCode;
			pOptionInfo->m_strName = CString(OJCode.call[3-i].hnam, ONameLen);
			pOptionInfo->m_strStrike = sPrice;
			pOptionMonth[i]->m_CallOptionInfoArray.Add(pOptionInfo);

			CItemDataInfo * pOptionInfo2 = new CItemDataInfo;
			pOptionInfo2->m_strCode = CString(OJCode.put[i].cod2, OPCODE_LEN);
			pOptionInfo2->m_strName = CString(OJCode.put[i].hnam, ONameLen);
			pOptionInfo2->m_strStrike = sPrice;
			pOptionMonth[i]->m_PutOptionInfoArray.Add(pOptionInfo2);
		}
	}
	
	if(!pUnderlyingAssetsInfo)
	{
		pUnderlyingAssetsInfo = new CUnderlyingAssetsInfo;
		pUnderlyingAssetsInfo->m_strShortCode = "01";
		pUnderlyingAssetsInfo->m_strUnderlyingAssetsCode = "101";//KP200CODE;
		pUnderlyingAssetsInfo->m_strUnderlyingAssetsName = "KP200";
		m_UnderlyingAssetsInfoArray.Add(pUnderlyingAssetsInfo);
	}			
	pUnderlyingAssetsInfo->m_MonthInfoArray.Add(pOptionMonth[0]);
	pUnderlyingAssetsInfo->m_MonthInfoArray.Add(pOptionMonth[1]);
	pUnderlyingAssetsInfo->m_MonthInfoArray.Add(pOptionMonth[2]);
	pUnderlyingAssetsInfo->m_MonthInfoArray.Add(pOptionMonth[3]);

	file.Close();
	
	return TRUE;
}

void CBaseNetworkModule::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if(m_pNetwork)
	{
		m_pNetwork->DelAllData();	
		delete m_pNetwork;
		m_pNetwork = NULL;
	}

	if(m_pCommonFrameWork)
	{
		if(IsWindow(m_pCommonFrameWork->GetSafeHwnd()))
			m_pCommonFrameWork->DestroyWindow();
		delete m_pCommonFrameWork;
		m_pCommonFrameWork = NULL;
	}

	if(m_pHistoricalWnd)
	{
		if(IsWindow(m_pHistoricalWnd->GetSafeHwnd()))
			m_pHistoricalWnd->DestroyWindow();
		delete m_pHistoricalWnd;
		m_pHistoricalWnd = NULL;
	}

	if(m_pPastDataOfUnderlyingAssets) delete m_pPastDataOfUnderlyingAssets;
	
	int i;
	for(i = 0 ; i < m_UnderlyingAssetsInfoArray.GetSize() ; i++)	delete m_UnderlyingAssetsInfoArray.GetAt(i);
	for(i = 0 ; i < m_FutureMonthInfoArray.GetSize(); i++)			delete m_FutureMonthInfoArray.GetAt(i);	
	for(i = 0 ; i < m_RTDataArray.GetSize(); i++)					delete m_RTDataArray.GetAt(i);	
	for(i = 0 ; i < m_ETFItemInfoArray.GetSize(); i++)				delete m_ETFItemInfoArray.GetAt(i);

	KillTimer(0);
}

void CBaseNetworkModule::AddFONetwork(CFONetworkModule * pFONetwork)
{
	m_FONetworkArray.Add(pFONetwork);
	if(m_bReceiveBaseData && pFONetwork && pFONetwork->m_pParentWnd)
	{
		pFONetwork->m_pParentWnd->PostMessage(WM_RECEIVE_BASEDATA, 0, (LPARAM) &m_BaseDataSet);
		pFONetwork->m_pParentWnd->PostMessage(WM_DISPLAY_TODAY, 0, (LPARAM) &m_BaseDataSet);
	}
}

void CBaseNetworkModule::DelFONetwork(CFONetworkModule * pFONetwork)
{
	int i, nSize = m_FONetworkArray.GetSize();

	for(i = 0 ; i < nSize ; i++)
	{
		CFONetworkModule * pNetwork = m_FONetworkArray.GetAt(i);
		if(pNetwork == pFONetwork)
		{
			m_FONetworkArray.RemoveAt(i);
			return;
		}
	}
}

CString CBaseNetworkModule::GetUnderlyingAssetsPrice(CString strUnderlyingAssetsCode)
{
//20080316 이문수 >>	
	CString strUnderlyingPrice, strUnderlyingPrice1;
	int i, nSize = m_RTDataArray.GetSize();
	
	if(strUnderlyingAssetsCode == KP200CODE)
	{
		int nFindCount = 0;
		CString strFutureCode = GetRecentFuture();
		for(i = 0 ; i < nSize ; i++)
		{
			CDataSet * pDataSet = m_RTDataArray.GetAt(i);
			if(pDataSet->m_strCode == strUnderlyingAssetsCode)
			{//kp200
				nFindCount++;
				strUnderlyingPrice = pDataSet->m_strCurPrice;
			}
			else if(pDataSet->m_strCode == strFutureCode)
			{//최근월선물
				nFindCount++;
				strUnderlyingPrice1 = pDataSet->m_strCurPrice;
			}
			if(nFindCount >= 2) break;
		}
		
		//최근월옵션의 배당액지수와 잔존일 정보얻기
		CCodeDataSet * pCodeInfo = NULL;
		if(m_BaseDataSet.m_CodeDataArray.GetSize() >= 5)
		{
			pCodeInfo = m_BaseDataSet.m_CodeDataArray.GetAt(4);
		}
		if(!pCodeInfo)
			return "";

		CCommonFrameWorkWnd * pCommon = (CCommonFrameWorkWnd *)GetCommonFrameWork();
		switch(pCommon->m_pTheorySet->m_nBasicPrice)
		{
		case 0: //kp200 - 배당액지수
			{
				strUnderlyingPrice.Format("%.2f", atof(strUnderlyingPrice) / 100 - atof(pCodeInfo->m_strIndex));				
			}
			break;
		case 1: //선물최근월
			{
				strUnderlyingPrice = ::ConvertIntToPoint(strUnderlyingPrice1, 2);
			}
			break;
		case 2: //자동계산
			{
				strUnderlyingPrice.Format( "%.2f", atof(strUnderlyingPrice1) / 100 * 
					exp(-1 * (atof(m_BaseDataSet.m_strCDRate) / 100) * (atof(pCodeInfo->m_strRemainDay) / 365)) );				
			}
			break;
		default: //가중
			{	strUnderlyingPrice.Format("%.2f",( atof(strUnderlyingPrice) * (pCommon->m_pTheorySet->m_nKOSPI200 / 100.) + 
					atof(strUnderlyingPrice1) * (pCommon->m_pTheorySet->m_nFuture / 100.) ) / 100.);				
			}
			break;
		}
	}
	else
	{
		for(i = 0 ; i < nSize ; i++)
		{
			CDataSet * pDataSet = m_RTDataArray.GetAt(i);
			if(pDataSet->m_strCode == strUnderlyingAssetsCode)
			{
				CCommonFrameWorkWnd* pWnd = (CCommonFrameWorkWnd*)GetCommonFrameWork();
				if(pWnd && pWnd->m_pEtcSet->m_bApplyPredictPrice && !pDataSet->m_strPredictPrice.IsEmpty())
					strUnderlyingPrice.Format("%d", atoi(pDataSet->m_strPredictPrice));
				else if(pDataSet->m_nRTDataType != RT_EXP_DATA_TYPE)
					strUnderlyingPrice.Format("%d", atoi(pDataSet->m_strCurPrice));
				break;
			}
		}
	}
	return strUnderlyingPrice;	
//20080316 이문수 <<
}

CString CBaseNetworkModule::GetUnderlyingAssetsPriceForMargin(CString strUnderlyingAssetsCode)
{
	CString strUnderlyingPrice, strUnderlyingPrice1;
	int i, nSize = m_RTDataArray.GetSize();
	
	for(i = 0 ; i < nSize ; i++)
	{
		CDataSet * pDataSet = m_RTDataArray.GetAt(i);
		if(pDataSet->m_strCode == strUnderlyingAssetsCode)
		{
			if(strUnderlyingAssetsCode == KP200CODE) 			
				strUnderlyingPrice = ::ConvertIntToPoint(pDataSet->m_strCurPrice2, 2);
			else 
				strUnderlyingPrice.Format("%d", atoi(pDataSet->m_strCurPrice));
			break;
		}
	}	
	return strUnderlyingPrice;	
}

CString CBaseNetworkModule::GetUnderlyingAssetsShortCode(CString strUnderlyingAssetsCode)
{
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyingInfo = m_UnderlyingAssetsInfoArray.GetAt(i);
		if(pUnderlyingInfo->m_strUnderlyingAssetsCode == strUnderlyingAssetsCode)
			return pUnderlyingInfo->m_strShortCode;
	}
	return "";
}

//입력된 종목의 기초자산 정보를 추출해 준다.
CUnderlyingAssetsInfo * CBaseNetworkModule::GetUnderlyingAssetsInfo(CString strCode)
{
	int nLength = strCode.GetLength();
	if(nLength < 3) return NULL;
	int nItemType = GetItemType(strCode);
	
	if(nItemType == EXP_ETF_TYPE || nItemType == EXP_STOCK_TYPE)
	{//주식
		int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CUnderlyingAssetsInfo * pUnderlyingInfo = m_UnderlyingAssetsInfoArray.GetAt(i);
			if(pUnderlyingInfo->m_strUnderlyingAssetsCode == strCode)
				return pUnderlyingInfo;
		}
	}
	else
	{
		int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
		for(i = 0 ; i < nSize ; i++)
		{
			CUnderlyingAssetsInfo * pUnderlyingInfo = m_UnderlyingAssetsInfoArray.GetAt(i);
			if(pUnderlyingInfo->m_strShortCode == strCode.Mid(1,2))
				return pUnderlyingInfo;
		}
	}
	return NULL;
}

CString CBaseNetworkModule::GetUnderlyingAssetsLastPrice(CString strUnderlyingAssetsCode)
{
	CString strUnderlyingLastPrice;

	int i, nSize = m_RTDataArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CDataSet * pDataSet = m_RTDataArray.GetAt(i);
		if(pDataSet->m_strCode == strUnderlyingAssetsCode)
		{
			if(pDataSet->m_strCode == KP200CODE)
			{
				strUnderlyingLastPrice = pDataSet->m_strPrevPrice2;
				strUnderlyingLastPrice = ::ConvertIntToPoint(strUnderlyingLastPrice, 2);
			}
			else//주식인 경우.
				strUnderlyingLastPrice.Format("%d", atoi(pDataSet->m_strLastPrice));

			break;
		}
	}
	return strUnderlyingLastPrice;
}

CDataSet * CBaseNetworkModule::GetDataSetOfUnderlyingAssets(CString strUnderlyingAssetsCode)
{	
	int i, nSize = m_RTDataArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CDataSet * pDataSet = m_RTDataArray.GetAt(i);
		if(pDataSet->m_strCode == strUnderlyingAssetsCode)
		{
			return pDataSet;
		}
	}
	return NULL;;
}

int CBaseNetworkModule::GetTradeUnit(CString strCode)
{
	int nItemType = GetItemType(strCode);

	if(nItemType == EXP_ETF_TYPE || nItemType == EXP_STOCK_TYPE)
		return 1;
	else
	{
		if(strCode.Left(1) == "1")
		{
			return 500000;
		}
		else if(strCode.Left(3) == "201" || strCode.Left(3) == "301")
		{
			return 100000;
		}
		else if(strCode.Left(1) == "2" || strCode.Left(1) == "3")
		{
			//20080327 이문수 >>
			int i,nSize = m_BaseDataSet.m_VolateInfoArray.GetSize();
			for(i = 0 ; i < nSize ; i++)
			{
				CVolateInfo * pVolateInfo = 
					m_BaseDataSet.m_VolateInfoArray.GetAt(i);
				if(pVolateInfo->m_strUnderlyingAssetsShortCode == strCode.Mid(1,2))
				{
					return atoi(pVolateInfo->m_strTradeUnit);
				}
			}
			//20080327 이문수 <<
			return 1;
		}
	}
	
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////
//선물 , 옵션, 주식은 이것으로 얻을 수 없다.
//주식옵션의 기초자산 별로 거래단위를 얻으려고 할 경우만 사용...
//////////////////////////////////////////////////////////////////////////////////////////
int CBaseNetworkModule::GetTradeUnitByUnderlyingAssetsCode(CString strUnderlyingAssetsCode)
{
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyInfo = 
			m_UnderlyingAssetsInfoArray.GetAt(i);
		if(pUnderlyInfo->m_strUnderlyingAssetsCode == strUnderlyingAssetsCode)
		{
			return atoi(pUnderlyInfo->m_strTradeUnit);
		}
	}		
	return 1;
}

CString CBaseNetworkModule::GetATM(CString strUnderlyingAssetsCode, CString strYM)
{
	CString strATM = "", strTemp;
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyingAssetsInfo = 
			m_UnderlyingAssetsInfoArray.GetAt(i);

		if(pUnderlyingAssetsInfo->m_strUnderlyingAssetsCode == strUnderlyingAssetsCode)
		{
			int j,nSize1 = m_RTDataArray.GetSize();
			for(j = 0 ; j  < nSize1; j++)
			{
				CDataSet * pDataSet = m_RTDataArray.GetAt(j);
				if(pDataSet->m_strCode == strUnderlyingAssetsCode)
				{
					double dUnderlyingAssetsPrice;
					double dTerm;
					//
					if(strUnderlyingAssetsCode == KP200CODE)
					{
						CString strCurPrice = ::ConvertIntToPoint(pDataSet->m_strCurPrice2, 2);
						dUnderlyingAssetsPrice = atof(strCurPrice);
					}
					else
						dUnderlyingAssetsPrice = atof(pDataSet->m_strCurPrice);

					int k, nSize2 = pUnderlyingAssetsInfo->m_MonthInfoArray.GetSize();
					for(k = 0 ; k < nSize2 ; k++)
					{
						COptionMonthInfo * pMonthInfo = pUnderlyingAssetsInfo->m_MonthInfoArray.GetAt(k);
						if(pMonthInfo->m_strYearMonth.GetLength() > 4)
							strTemp = pMonthInfo->m_strYearMonth.Mid(3);
						else
							strTemp = pMonthInfo->m_strYearMonth.Mid(1);		// 주식옵션 일 때
//						if(pMonthInfo->m_strYearMonth.Mid(3) == strYM)
						if(strTemp == strYM)
						{
							int l, nSize3 = pMonthInfo->m_CallOptionInfoArray.GetSize();
							for(l = 0 ; l < nSize3 ; l++)
							{
								CItemDataInfo * pInfo = pMonthInfo->m_CallOptionInfoArray.GetAt(l);
								double dDifference = fabs(atof(pInfo->m_strStrike) - dUnderlyingAssetsPrice);
								if(l == 0)
									dTerm = dDifference;
								else
								{
									if(dTerm > dDifference)
										dTerm = dDifference;
									else
									{
										if(l > 0) l--;
										CItemDataInfo * pData = pMonthInfo->m_CallOptionInfoArray.GetAt(l);
										return pData->m_strStrike;
									}
								}
							}
							if(l == nSize3)
							{
								CItemDataInfo * pData = 
									pMonthInfo->m_CallOptionInfoArray.GetAt(pMonthInfo->m_CallOptionInfoArray.GetSize() - 1);
								return pData->m_strStrike;
							}							
						}
					}
					return strATM;
				}
			}
			return strATM;
		}
	}
	return strATM;
}

void CBaseNetworkModule::SortSOptItem()
{
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyingInfo = 
			m_UnderlyingAssetsInfoArray.GetAt(i);

		int j, nSize1 = pUnderlyingInfo->m_MonthInfoArray.GetSize();
		for(j = 0 ; j < nSize1; j++)
		{
			COptionMonthInfo * pMonthInfo = pUnderlyingInfo->m_MonthInfoArray.GetAt(j);			
			Sort(pMonthInfo->m_CallOptionInfoArray);
			Sort(pMonthInfo->m_PutOptionInfoArray);
		}
	}
}

void CBaseNetworkModule::Sort(CItemInfoArray & ItemInfoArray)
{
	int i,j,nSize = ItemInfoArray.GetSize();
	for(i = nSize - 1 ; i >= 0 ; i--)
	{
		for(j = 0; j < i ; j++)
		{
			CItemDataInfo * pItem = ItemInfoArray.GetAt(j);
			CItemDataInfo * pNext = ItemInfoArray.GetAt(j+1);
			if(atof(pItem->m_strStrike) > atof(pNext->m_strStrike))
			{
				CItemDataInfo Temp;
				Temp		= *pItem;
				*pItem		= *pNext;
				*pNext		= Temp;
			}
		}
	}
}

//배당액지수 얻기.
CString CBaseNetworkModule::GetIndex(CString strCode)
{
	int nType = GetItemType(strCode);
	if(nType != EXP_FUTURE_TYPE && nType != EXP_KP200_CALL_OPT_TYPE && nType != EXP_KP200_PUT_OPT_TYPE &&
		nType != EXP_STOCK_CALL_OPT_TYPE && nType != EXP_STOCK_PUT_OPT_TYPE)
		return "";

	int i,nSize = m_BaseDataSet.m_CodeDataArray.GetSize();
	for(i = 0 ; i < nSize; i++)
	{	
		CCodeDataSet * pCode = m_BaseDataSet.m_CodeDataArray.GetAt(i);
	
		if(nType == EXP_FUTURE_TYPE)
		{
			if(pCode->m_strCodeInfo == strCode.Left(5))			
				return pCode->m_strIndex;			
		}
		else
		{
			if(pCode->m_strCodeInfo.GetLength() == 5 &&					// 5자리
					pCode->m_strCodeInfo.Left(1) != "1" &&				// 선물이 아니고
					pCode->m_strCodeInfo.Mid(1,4) == strCode.Mid(1,4)	// 201C6, 301C6 -> 01C6 추출비교
				)
				return pCode->m_strIndex;
		}
	}

	return "";
}

CString CBaseNetworkModule::GetRemainDay(CString strCode)
{
	int nType = GetItemType(strCode);
	if(nType != EXP_FUTURE_TYPE && nType != EXP_KP200_CALL_OPT_TYPE && nType != EXP_KP200_PUT_OPT_TYPE && 
		nType != EXP_STOCK_CALL_OPT_TYPE && nType != EXP_STOCK_PUT_OPT_TYPE)
		return "";

	CString strRemainDay;
	int i,nSize = m_BaseDataSet.m_CodeDataArray.GetSize();
	for(i = 0 ; i < nSize; i++)
	{	
		CCodeDataSet * pCode = m_BaseDataSet.m_CodeDataArray.GetAt(i);
	
		if(nType == EXP_FUTURE_TYPE)
		{
			if(pCode->m_strCodeInfo == strCode.Left(5))			
			{
				//20080314 이문수 >>
				// 0 : 거래일 , 1:영업일
				if(m_pCommonFrameWork->m_pTheorySet->m_nRemainDay == 0)
				{
					strRemainDay = pCode->m_strWorkRemainDay;
					break;
				}
				else
				{
					strRemainDay = pCode->m_strRemainDay;
					break;
				}
			}
		}
		else
		{
			if(pCode->m_strCodeInfo.Mid(1,4) == strCode.Mid(1,4))
			{
				if(m_pCommonFrameWork->m_pTheorySet->m_nRemainDay == 0)
				{
					strRemainDay = pCode->m_strWorkRemainDay;
					break;
				}
				else
				{
					strRemainDay = pCode->m_strRemainDay;
					break;
				}
			}
			//20080314 이문수 <<
		}
	}

	if(m_pCommonFrameWork->m_pTheorySet->m_bTimeValue)
	{
		double dTotalRemainDay = atof(strRemainDay) - m_dTimeLoss;
		strRemainDay.Format("%.2f", dTotalRemainDay);
	}
	return strRemainDay;	
}

//20080325 이문수 >>
CString CBaseNetworkModule::GetRemainDayForMargin(CString strCode)
{
	int nType = GetItemType(strCode);
	if(nType != EXP_FUTURE_TYPE && nType != EXP_KP200_CALL_OPT_TYPE && nType != EXP_KP200_PUT_OPT_TYPE && 
		nType != EXP_STOCK_CALL_OPT_TYPE && nType != EXP_STOCK_PUT_OPT_TYPE)
		return "";

	CString strRemainDay;
	int i,nSize = m_BaseDataSet.m_CodeDataArray.GetSize();
	for(i = 0 ; i < nSize; i++)
	{	
		CCodeDataSet * pCode = m_BaseDataSet.m_CodeDataArray.GetAt(i);
		
		if(pCode->m_strCodeInfo == strCode.Left(5))			
		{
			//달력일수
			strRemainDay = pCode->m_strRemainDay;
			return strRemainDay;
		}
	}
	return strRemainDay;	
}
//20080325 이문수 <<

//20080314 이문수 >>
double  CBaseNetworkModule::CalcTimeLoss()
{
	if(m_pCommonFrameWork->m_pTheorySet->m_bTimeValue)
	{
		CTime t1, t2, tCurTime;
		CTimeSpan timeSpan1, timeSpan2;
		CString strFromTime , strToTime;
		if(m_pCommonFrameWork->m_pTheorySet->m_strFromTime.GetLength() < 4)
			t1 = CTime(2000, 1, 1, 9, 0, 0);
		else
			t1 = CTime(2000, 1, 1, 
					atoi(m_pCommonFrameWork->m_pTheorySet->m_strFromTime.Left(2)),
					atoi(m_pCommonFrameWork->m_pTheorySet->m_strFromTime.Mid(2,2)), 0);


		if(m_pCommonFrameWork->m_pTheorySet->m_strToTime.GetLength() < 4)
			t2 = CTime(2000, 1, 1, 15, 15, 0);
		else
			t2 = CTime(2000, 1, 1, 
				atoi(m_pCommonFrameWork->m_pTheorySet->m_strToTime.Left(2)),
				atoi(m_pCommonFrameWork->m_pTheorySet->m_strToTime.Mid(2,2)), 0);

		tCurTime = CTime(2000, 1, 1, 0, 0, 0);

		//20080324 이문수 >>
		if(m_strKP200Time == "장마감")
			tCurTime = t2;
		else
			tCurTime = CTime(2000, 1, 1, atoi(m_strKP200Time.Left(2)), atoi(m_strKP200Time.Mid(2,2)), 0);
		//20080324 이문수 <<

		if(t1 >= t2 || t1 >= tCurTime)
			return 0.;	
		
		timeSpan1 = t2			- t1;
		timeSpan2 = tCurTime	- t1;

		m_dTimeLoss = (timeSpan2.GetTotalSeconds() / (double)timeSpan1.GetTotalSeconds());		
	}
	else
		m_dTimeLoss = 0;

	return m_dTimeLoss;
}
//20080314 이문수 <<

CString CBaseNetworkModule::GetRecentFuture()
{
	if(m_FutureMonthInfoArray.GetSize() > 0)
	{
		CItemDataInfo * pItem = m_FutureMonthInfoArray.GetAt(0);
		return pItem->m_strCode;
	}
	return "";
}

void CBaseNetworkModule::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 0 && m_bEnableNetwork && m_FONetworkArray.GetSize() > 0)
	{
		RequestAverageIV();
		KillTimer(0);
	}
	
#ifdef __TEST__
	else if(nIDEvent == TEST_TIMER)
	{
		for(int i = 0 ; i < 20000 ; i++)
		{		
			for(int j = 0 ; j < 25; j++)m_pNetwork->RequestAverageIV("99");
			Sleep(1000);
		}
	}
#endif

	CWnd::OnTimer(nIDEvent);
}

void CBaseNetworkModule::RequestAverageIV()
{
	m_bEnableNetwork = FALSE;
	m_pNetwork->RequestAverageIV("99");//전체기초자산에 대해서 요청...
}

//옵션 종목코드를 넣어준다.
CString CBaseNetworkModule::GetTotalAverageIV(CString strCode)
{
	int nItemType = GetItemType(strCode);

	if(nItemType == EXP_ETF_TYPE || nItemType == EXP_STOCK_TYPE) return "";

	int i;
	char cType = strCode.GetAt(0);
	for(i = 0 ; i < m_AverageIV.m_AverageIVArray.GetSize(); i++)
	{
		CAverageIVSet * pAveIV = m_AverageIV.m_AverageIVArray.GetAt(i);
		if(pAveIV->m_strUnderlyingAssetsCode == strCode.Mid(1,2))
		{		
			if(cType == '2')		return pAveIV->m_strCallAverageIV;
			else if(cType == '3')	return pAveIV->m_strPutAverageIV;
			return "";
		}
	}	
	return "";
}

CString CBaseNetworkModule::GetKOSPIPrice()
{
	for(int i = 0 ; i < m_RTDataArray.GetSize(); i++)
	{
		CDataSet * pDataSet = m_RTDataArray.GetAt(i);
		if(pDataSet->m_strCode == KOSPICODE)
		{
			return pDataSet->m_strCurPrice;
		}
	}
	return "";
}
// [4/10/2003] m_FONetworkArray에 등록된 모든 윈도를 닫고, 자신도 삭제
void CBaseNetworkModule::DestroyWndSelf()
{
	int nSize = m_FONetworkArray.GetSize();

	// 등록된 윈도우를 모두 종료
	CWnd* pMainWnd = AfxGetMainWnd();
	for(int i = nSize - 1 ; i >= 0; i--)
	{
		CFONetworkModule* pFModule = m_FONetworkArray.GetAt(i);
		CFrameWnd *pWnd = pFModule->m_pParentWnd->GetParentFrame();

		HWND hA = pWnd->GetSafeHwnd();
		HWND hB = pMainWnd->GetSafeHwnd();

		if(pWnd && pMainWnd && hA != hB)
		{
			pWnd->PostMessage(WM_CLOSE);
		}
	}
}

// [4/8/2003] > by jeoyoho, 자동 종료 알림 대화상자 닫힘
// WM_DIALOGDESTORYED메시지 처리(from CCloseNoticeDlg)
long CBaseNetworkModule::OnDialogDestroyed(WPARAM wParam, LPARAM lParam)
{
	DestroyWndSelf();

	return 0L;
} 
// [4/10/2003] > by jeoyoho, 모든 창이 닫히면 자동 종료
// WM_CLOSE_BASENETWORKMODULE메시지 처리(from CCommonFrameWorkWnd)
long CBaseNetworkModule::OnCloseBaseNetworkModule(WPARAM wParam, LPARAM lParam)
{
	DestroyWndSelf();

	// 자신을 삭제하도록 
	PostMessage(WM_CLOSE);

	return 0L;
}

void CBaseNetworkModule::SetHistoricalWnd(CWnd * pHistoricalWnd)
{
	m_pHistoricalWnd = pHistoricalWnd;
}

double CBaseNetworkModule::GetOptIVByUnderlyingAssetsCode(CString strUnderlyingAssetsCode)
{
	CString strShortCode;
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyingAssetsInfo = m_UnderlyingAssetsInfoArray.GetAt(i);
		if(pUnderlyingAssetsInfo->m_strUnderlyingAssetsCode == strUnderlyingAssetsCode)
		{
			strShortCode = pUnderlyingAssetsInfo->m_strShortCode;
			break;
		}
	}
	if(strShortCode.IsEmpty()) return 0;
	
	for(i = 0 ; i < m_BaseDataSet.m_VolateInfoArray.GetSize() ; i++)
	{
		CVolateInfo * pOpt = m_BaseDataSet.m_VolateInfoArray.GetAt(i);
		if(strShortCode == pOpt->m_strUnderlyingAssetsShortCode)
			return atof(pOpt->m_strIV);
	}	
	return 0.;
}

CVolateInfo * CBaseNetworkModule::GetVolateInfoByUnderlyingAssetsCode(CString strUnderlyingAssetsCode)
{
	CString strShortCode;
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize; i++)
	{
		CUnderlyingAssetsInfo * pUnderlyingAssetsInfo = m_UnderlyingAssetsInfoArray.GetAt(i);
		if(pUnderlyingAssetsInfo->m_strUnderlyingAssetsCode == strUnderlyingAssetsCode)
		{
			strShortCode = pUnderlyingAssetsInfo->m_strShortCode;
			break;
		}
	}
	if(strShortCode.IsEmpty()) return 0;

	for(i = 0 ; i < m_BaseDataSet.m_VolateInfoArray.GetSize(); i++)
	{
		CVolateInfo * pVol = m_BaseDataSet.m_VolateInfoArray.GetAt(i);
		if(strShortCode == pVol->m_strUnderlyingAssetsShortCode)
			return pVol;			
	}
	return NULL;
}

long CBaseNetworkModule::OnShowCloseDlg(WPARAM wParam, LPARAM lParam)
{
	if(m_pClosingDlg && IsWindow(m_pClosingDlg->GetSafeHwnd()))
	{
		m_pClosingDlg->ShowWindow(SW_SHOW);
	}
	return 0L;
}

//종목코드를 입력해서 변동성 정보 구조체를 얻어온다.
CVolateInfo * CBaseNetworkModule::GetVolateInfoByItemCode(CString strCode)
{
	int nLength = strCode.GetLength();
	if(nLength < 3) return NULL;

	CUnderlyingAssetsInfo * pUnderlyingInfo = GetUnderlyingAssetsInfo(strCode);
	if(!pUnderlyingInfo) return NULL;

	CString strShortCode = pUnderlyingInfo->m_strShortCode;

	int i,nSize = m_BaseDataSet.m_VolateInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++)
	{
		CVolateInfo * pVolate = m_BaseDataSet.m_VolateInfoArray.GetAt(i);
		if(pVolate->m_strUnderlyingAssetsShortCode == strShortCode)
		{
			return pVolate;
		}
	}	
	return NULL;
}


void CBaseNetworkModule::GetOptionYearMonthArray(CString strAsset, CStringArray *pstrYearMonthArray)
{
	int i,nSize = m_UnderlyingAssetsInfoArray.GetSize();
	for(i = 0 ; i < nSize ; i++) 
	{		
		CUnderlyingAssetsInfo * pUnderlying = m_UnderlyingAssetsInfoArray.GetAt(i);
		if(pUnderlying) 
		{
			int nMonthCnt = pUnderlying->m_MonthInfoArray.GetSize();
			if(strAsset == pUnderlying->m_strUnderlyingAssetsCode && nMonthCnt > 0) 
			{
				for(int j=0; j<nMonthCnt; j++) {
					COptionMonthInfo* pOptMonthInfo = pUnderlying->m_MonthInfoArray.GetAt(j);
					if(pOptMonthInfo && pOptMonthInfo->m_CallOptionInfoArray.GetSize() > 0) 
					{
						CItemDataInfo* pItemInfo1 = pOptMonthInfo->m_CallOptionInfoArray.GetAt(0);
						if(pItemInfo1->m_strCode.GetLength() >= 9) 
						{
							CString strMonth = "0" + pItemInfo1->m_strCode.Mid(3,3);
							pstrYearMonthArray->Add(strMonth);
						}
					}
				}
			}
		}
	}
}

LONG CBaseNetworkModule::OnUser(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	BYTE bt = LOBYTE(LOWORD(wParam));
	switch(bt)
	{
	case DLL_OUB:					// Tr수신
		ReceiveDataPIDO(wParam, lParam);
		break;

	case DLL_ALERT:					// 리얼 수신
		{
			int i,j;
			char* pData = (char*)lParam;

			if(!m_pNetwork)	return 0L;
			CDataArray	ReceiveArray;
			
			// 파싱해서 ReceiveArray 에 채워서 받음
			m_pNetwork->ReceiveRTData(wParam, lParam, m_RTDataArray, ReceiveArray);

			if(ReceiveArray.GetSize() > 0)
			{
				for(j= 0 ; j < ReceiveArray.GetSize(); j++)
				{
					CDataSet * pDataSet = ReceiveArray.GetAt(j);

					//20080314 이문수 >>
					//시간가치 반영
					if(pDataSet->m_strCode == KP200CODE)
						m_strKP200Time = pDataSet->m_strTime;
					//20080314 이문수 <<

					//예상체결가가 들어온 것을 받지 않는 것으로 셋팅되어 있는 경우.
					if(pDataSet->m_nRTDataType == RT_EXP_DATA_TYPE && !m_pCommonFrameWork->m_pEtcSet->m_bApplyPredictPrice)
						continue;

					int nSize = m_FONetworkArray.GetSize();
					for(i = 0 ; i < nSize ; i++)
					{
						CFONetworkModule * pNetwork = m_FONetworkArray.GetAt(i);
						if(pNetwork->m_pParentWnd && IsWindow(pNetwork->m_pParentWnd->GetSafeHwnd()))
							pNetwork->m_pParentWnd->SendMessage(WM_RECEIVE_RT_DATA, (WPARAM)BASEDATA_TYPE, (LPARAM)pDataSet);
					}	
				}
			}

			break;
		}
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

LONG CBaseNetworkModule::ReceiveDataPIDO(WPARAM wParam, LPARAM lParam)
{
	LONG ret = 0;
	int nKind = (HIBYTE(LOWORD(wParam)));

	if(!m_pNetwork) return FALSE;

	switch(nKind)
	{
		case TR_BASEDATA:			// 기초데이터
			m_pNetwork->ReceiveBaseData(wParam, lParam);

			// 조회 데이터 목록을 만들어서 배열의 첫 종목부터 조회
			RqItem(TRUE); // m_strRqCodeArray 배열의 종목을 차례료 요청한다.
			break;

		case TR_AVERAGEIV:
			{
				CAverageIV AveIV;

				if(m_pNetwork->ReceiveData(wParam, lParam, AveIV))
				{
					m_AverageIV = AveIV;				
					m_bEnableNetwork = TRUE;
					
					if(atoi(m_AverageIV.m_strType) != 2) 
						SetTimer(0, TIMER, NULL);
				}
				else
				{
					m_bEnableNetwork = TRUE;
					SetTimer(0, TIMER, NULL);
				}
			}
			break;
		case TR_AVERAGETODAYIV:
			{
				m_pNetwork->ReceiveData(wParam, lParam, m_stTodayVI);
				m_bEnableNetwork = TRUE;

				if(m_hVolateSelDlg)
				{
					::SendMessage(m_hVolateSelDlg, WM_AVERAGETODAYIV, 0, 0);
					m_hVolateSelDlg = NULL;
				}
			}
			break;
		case TR_OPTION_MONTH:
			{
				COptionMonthInfo OptionMonthInfo;
			
				int nResult;
				if((nResult = m_pNetwork->ReceiveData(wParam, lParam, &OptionMonthInfo)) == SUCCESS)
				{
					int nSize = m_FONetworkArray.GetSize();
					for(int i = 0; i < nSize; ++i)
					{
						CFONetworkModule* pFoNetWorkModlue = (CFONetworkModule*)m_FONetworkArray.GetAt(i);
						if(pFoNetWorkModlue->IsCommUseBaseModule())
							pFoNetWorkModlue->m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)MONTH_OPT_INFO, (LPARAM)&OptionMonthInfo);
					}	
				}
			}
			break;
		case TR_SOPTION_MONTH:
			{
				COptionMonthInfo OptionMonthInfo;
				
				int nResult;
				if((nResult = m_pNetwork->ReceiveDataSOptionMonth(wParam, lParam, &OptionMonthInfo)) == SUCCESS)
				{
					int nSize = m_FONetworkArray.GetSize();
					for(int i = 0; i < nSize; ++i)
					{
						CFONetworkModule* pFoNetWorkModlue = (CFONetworkModule*)m_FONetworkArray.GetAt(i);
						if(pFoNetWorkModlue->IsCommUseBaseModule())
							pFoNetWorkModlue->m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)MONTH_OPT_INFO, (LPARAM)&OptionMonthInfo);
					}	
				}
			}
			break;
		case TR_DELTA_HEDGE_INFO:
			{
				TR_15204_O stDeltaHedge;

				int nResult;
				if((nResult = m_pNetwork->ReceiveData(wParam, lParam, &stDeltaHedge)) == SUCCESS)
				{
					int nLen = HIWORD(wParam);
					char* szDeltaHedgeInfo = new char[nLen + 1];
					memset(szDeltaHedgeInfo, NULL, nLen + 1);

					memcpy(szDeltaHedgeInfo, (char*)lParam, nLen);

					int nSize = m_FONetworkArray.GetSize();
					for(int i = 0; i < nSize; ++i)
					{
						CFONetworkModule* pFoNetWorkModlue = (CFONetworkModule*)m_FONetworkArray.GetAt(i);
						if(pFoNetWorkModlue->IsCommUseBaseModule())
							pFoNetWorkModlue->m_pParentWnd->SendMessage(WM_FO_RECEIVE_DATA, (WPARAM)DELTA_HEDGE_INFO, (LPARAM)szDeltaHedgeInfo);
					}

					delete szDeltaHedgeInfo;
				}
			}
			break;
		default:
			if(nKind == TR_KOSPI || nKind == TR_KP200 || (nKind >= 50 && nKind <= 255))
			{
				//20080316 이문수 >>
				if(nKind >= 50 && nKind <= 255 && m_KeyToWndList.GetCount() > 0)
				{				
					BOOL bMyData = FALSE;
					POSITION pos, pos1;
					for(pos = m_KeyToWndList.GetHeadPosition(); (pos1 = pos) ; )
					{
						CKeyToWndInfo Info = m_KeyToWndList.GetNext(pos);
						if(Info.m_nKey == nKind)
						{
							for(int _i = 0; _i < Info.m_hWndArray.GetSize(); _i++ )
							{
								HWND hWnd = (HWND)Info.m_hWndArray.GetAt(_i);
								if(hWnd == GetSafeHwnd())
									bMyData = TRUE;
								else
									::SendMessage(hWnd, WM_USER, wParam, lParam);
							}
							m_KeyToWndList.RemoveAt(pos1);
							break;
						}
					}
					if(!bMyData) return FALSE;
				}
				//20080316 이문수 <<

				if(m_strRqCodeArray.GetSize() <= 0) return FALSE;

				CString strCode = m_strRqCodeArray.GetAt(0);

				CDataSet * pDataSet = new CDataSet;
				pDataSet->m_strCode = strCode;
				
				// 수신 데이터 파싱
				m_pNetwork->ReceiveData(wParam, lParam, pDataSet);

				// 리얼 요청된 종목 배열에 넣음
				m_RTDataArray.Add(pDataSet);
				// 조회된 종목을 배열에서 삭제
				m_strRqCodeArray.RemoveAt(0);

				// 주식인 경우 주문단위를 셋팅해 준다.(정용호 추가:주식옵션파일에 기초자산인 주식의 주문단위정보가 없었다.)
				int nItemType = GetItemType(strCode);	
				if(nItemType == EXP_STOCK_TYPE)
				{
					CUnderlyingAssetsInfo * pAssetInfo = GetUnderlyingAssetsInfo(strCode);
					pAssetInfo->m_strTradeUnit = pDataSet->m_strReserved;
				}

				//20080314 이문수 >>
				//잔존일 시간가치 반영을 위해 2초마다 들어오는 kp200의 시간데이터를 저장해 놓는다.
				else if(nItemType == EXP_KP200)
				{
					m_strKP200Time = pDataSet->m_strTime;
				}
				//20080314 이문수 <<
	
				// 필요한 종목의 시세를 다 받았으면 
				if(m_strRqCodeArray.GetSize() == 0)
				{
					m_bReceiveBaseData = TRUE;

					if(m_RTDataArray.GetSize() > 0)	AddRTData();

					m_strRqCodeArray.RemoveAll();
					int i , nSize = m_FONetworkArray.GetSize();
					for(i = 0 ; i < nSize ; i++)
					{
						CFONetworkModule * pFONetwork = m_FONetworkArray.GetAt(i);
						if(pFONetwork->m_pParentWnd && IsWindow(pFONetwork->m_pParentWnd->GetSafeHwnd()))
						{			
							pFONetwork->m_pParentWnd->PostMessage(WM_RECEIVE_BASEDATA, 0, (LPARAM)&m_BaseDataSet);
							pFONetwork->m_pParentWnd->PostMessage(WM_DISPLAY_TODAY, 0, (LPARAM)&m_BaseDataSet);
						}
					}

					//////////////////////////////////////////////////////////////////////////
					RequestAverageIV();
					//////////////////////////////////////////////////////////////////////////

					//20080324 이문수 >>
					CalcTimeLoss();
					//20080324 이문수 <<
				}
				else
					RqItem(); // 다음 종목 요청
			}
			break;
	}

	return ret;
}

CString CBaseNetworkModule::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pFrameWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue.Format("%s", dta);
	
	return retvalue;
}

void CBaseNetworkModule::RequestTodayAverageIV(HWND hVolSelWnd, CString strShortCode)
{
	m_hVolateSelDlg = hVolSelWnd;

	int nDayFlag;
	if(m_pCommonFrameWork->m_pTheorySet->m_nRemainDay == 0)
		nDayFlag = (m_pCommonFrameWork->m_pTheorySet->m_bTimeValue) ? 2 : 0;
	else
		nDayFlag = (m_pCommonFrameWork->m_pTheorySet->m_bTimeValue) ? 3 : 1;

	CString strInput;
	strInput.Format("%2.2s%c%3.3s%3.3s%4.4s%4.4s%4.4s",
					strShortCode,										// 기초자산코드(2)
					nDayFlag,											// 영업일구분(1)
					"000",												// 잔존일수(3)		
					m_nBizDayCount,										// 총영업일수(4)	
					m_pCommonFrameWork->m_pTheorySet->m_strFromTime,	// 시작시간(4)
					m_pCommonFrameWork->m_pTheorySet->m_strToTime,		// 종료시간(4)
					"    ");											// 현재시간(4)

	m_bEnableNetwork = FALSE;
	m_pNetwork->RequestTodayAverageIV(strInput);
}


BOOL CBaseNetworkModule::IsApplyPredictPrice()
{
	CCommonFrameWorkWnd * pCommon = (CCommonFrameWorkWnd *)GetCommonFrameWork();
	
	if(pCommon != NULL)
		return pCommon->m_pEtcSet->m_bApplyPredictPrice;
		
	return FALSE;
}