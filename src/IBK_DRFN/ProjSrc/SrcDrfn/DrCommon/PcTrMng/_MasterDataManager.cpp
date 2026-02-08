#include "stdafx.h"
#include "PcTrMng.h"

//--> For Solomon
#include "../../../SrcSite/include/axisfire.h"	
#include "../../inc/IAUPartnerMng.h"
#include "../../inc/commonDefines.h" // 0024696: 파생상품 코드체계 변경 제도개선
//<-- For Solomon

#define	getCODE			0x45		// char* GetCode(_codeR*)
						// lParam : _codeR*

char *g_pszCFTiesCode[] = { "16599999", "16699999", "16799999", "17599999", "17699999", "17799999" };
char *g_pszCFTiesName[] = { "신3년국채", "신5년국채", "신10년국채", "미국달러", "엔", "유로" };

char *g_pszSFTiesCode[] = { "11199999", "11299999", "11399999", "11499999", "11599999", "11699999",
							"11899999", "11999999", "12199999", "12499999", "13299999", "13699999",
							"13999999", "14099999", "14199999", "14499999", "14599999", "14699999",
							"14899999", "14999999", "15099999", "15199999", "15299999", "15499999",
							"15699999", "15799999" };

char *g_pszSFTiesName[] = { "삼성전자", "SK텔레콤", "POSCO", "KT", "한국전력", "현대차",
							"신한지주", "기아차", "삼성물산", "LG전자", "현대제철", "케이티앤지",
							"현대중공업", "하나지주", "SK이노베이", "우리금융", "LGD", "KB금융",
							"대우증권", "두산인프라", "SK하이닉스", "GS건설", "NHN", "이마트",
							"NAVER", "대한항공" };

//저장루틴--> 지우지말고... 필요시 사용.
// void _DoAlzioSerialize(LPCSTR szFileKey, 
// 					   CStringArray* parCode, CStringArray* parName, CStringArray* parEngName)
// {
// 	CString szLogFile;
// 	szLogFile.Format("%s\\master\\%s.dat", theApp.m_MainInfoMng.GetDataDir(), szFileKey);
// 	
// 	CFile file;
// 	if (file.Open(szLogFile, CFile::typeBinary|CFile::modeRead))
// 		//if (file.Open(szLogFile, CFile::typeBinary|CFile::modeCreate|CFile::modeWrite))
// 	{
// 		CArchive ar(&file, CArchive::load);
// 		//CArchive ar(&file, CArchive::store);
// 		
// 		int nVer = 1;
// 		if (ar.IsStoring())
// 		{	// storing code
// 			ar << nVer;
// 			parCode->Serialize(ar);
// 			parName->Serialize(ar);
// 			parEngName->Serialize(ar);
// 		}
// 		else
// 		{	// loading code
// 			ar >> nVer;
// 			parCode->Serialize(ar);
// 			parName->Serialize(ar);
// 			parEngName->Serialize(ar);
// 		}
// 		ar.Close();
// 		file.Close();
// 	}
// }
//<--저장루틴
// 	CStringArray gFOCode, gFOCodeName;
// 	CMapStringToString gFOCodeMap;
// 	void _gDoReadFOMaster(CStringArray &arrCode, CStringArray &arrName)
// 	{
// 		CString szLogFile;
// 		szLogFile.Format("%s\\master\\%s.dat", theApp.m_MainInfoMng.GetDataDir(), "FOMaster");
// 
// 		if(gFOCode.GetSize()==0)
// 		{
// 			CStdioFile file;
// 			if (file.Open(szLogFile, CFile::typeText|CFile::modeRead))
// 			{
// 				CString rString, rCode, rCodeName;
// 				while(file.ReadString(rString))
// 				{
// 					rCode = rString.Left(10);
// 					rCodeName = rString.Mid(11);
// 
// 					gFOCode.Add(rCode);
// 					gFOCodeName.Add(rCodeName);
// 
// 					gFOCodeMap.SetAt(rCode, rCodeName);
// 				}
// 				file.Close();
// 			}
// 		}
// 
// 		for(int i=0; i<gFOCodeName.GetSize(); i++)
// 		{
// 			arrCode.Add(gFOCode.GetAt(i));
// 			arrName.Add(gFOCodeName.GetAt(i));
// 		}
// 	}
// ----------------------------------------------------------------------
// CMasterDataManager
CMasterDataManager::CMasterDataManager()
{
// 	m_nJ_K_Count = 0;
// 	m_nJ_K_Count_J = 0;
// 	m_nJ_K_Count_K = 0;

//@Solomon090819	m_itemMaster.Load();
//@Solomon090819	m_itemMaster.LoadThemeInfo();

//	m_hWinixWnd = NULL;
	m_hCodeMasterWnd = NULL;
}

CMasterDataManager::~CMasterDataManager()
{
//	UnLoadMASTER_J_K();
	ClearListSTRINGBYTE(&m_RecentCodeTypeList);
	ClearListSTRINGBYTE(&m_RecentMarketTypeList);
	ClearListSTRINGSTRING(&m_RecentCodeNameList);
	ClearListSTRINGSTRING(&m_RecentChxUpCodeList);
}

void CMasterDataManager::ClearListSTRINGBYTE(List_ST_STRINGBYTE* pList)
{
	ST_STRINGBYTE* pItem;
	for(POSITION pos=pList->GetHeadPosition(); pos;)
	{
		pItem = pList->GetNext(pos);
		if(pItem) delete pItem;
	}
	pList->RemoveAll();
}

void CMasterDataManager::ClearListSTRINGSTRING(List_ST_STRINGSTRING* pList)
{
	ST_STRINGSTRING* pItem;
	for(POSITION pos=pList->GetHeadPosition(); pos;)
	{
		pItem = pList->GetNext(pos);
		if(pItem) delete pItem;
	}
	pList->RemoveAll();
}

void CMasterDataManager::SetKeyData(LPCSTR szKey, LPCSTR szData)
{	
}

void CMasterDataManager::SetKeyData(LPCSTR szKey, long   dwData)
{
// 	if(strcmp(szKey, "WinixWnd")==0)
// 	{
// 		m_hWinixWnd = (HWND)dwData;
// 		if(!m_hWinixWnd || !IsWindow(m_hWinixWnd))
// 			m_hWinixWnd = NULL;
// 	}
}

HWND CMasterDataManager::GetHWND()
{
	HWND hWnd = NULL;
	IPartnerSvrManager* pMng = (IPartnerSvrManager*)theApp.m_GateMng.GetInterface(UUID_IPartnerSvrManager);
	if(pMng)
		pMng->GetParentHandle(0, hWnd);
	
	return hWnd;
}
//@Solomon
HWND CMasterDataManager::GetCodeMasterHWND()
{
	if(m_hCodeMasterWnd==NULL)
	{
		HWND hWnd = theApp.m_pPctrWinixLoader->PTM_GetCodeMaster();
		m_hCodeMasterWnd = hWnd;
		return hWnd;
	}
	return m_hCodeMasterWnd;
}

// {{ Code.ocx
long CMasterDataManager::GetBaseCodeTable()
{
	//@solomon return m_CodeOcx.GetBaseCodeTable();
	return NULL;
}

void CMasterDataManager::SetInfo(long _dwInfo)
{
	//@solomonm_CodeOcx.SetInfo(_dwInfo);
}

long CMasterDataManager::ShowCodeTable(LPCTSTR _szCodeKey, const VARIANT& _hWnd)
{
	//@solomon return m_CodeOcx.ShowCodeTable(_szCodeKey, _hWnd);
	return NULL;
}

long CMasterDataManager::ShowModeCodeTable(LPCTSTR _szCodeKey, const VARIANT& _hWnd)
{
	//@solomon return m_CodeOcx.ShowModeCodeTable(_szCodeKey, _hWnd);
	return NULL;
}

void CMasterDataManager::SetDataString(short nKey, LPCTSTR szData)
{
	//@solomon m_CodeOcx.SetDataString(nKey, szData);
}

BOOL CMasterDataManager::InitData(LPCTSTR szRootPath)
{
	//@solomon return m_CodeOcx.InitData(szRootPath);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2006.08.29
// 함수 이름         : CMasterDataManager::GetCodeInfo
// 리턴 타입         : CString
// 리턴 타입 설명    : 
// 파라미터			 :	LPCTSTR _szCodeKey;		// MarketKey
//						short _nFlag;			// * _nFlag 0 : 코드값으로 코드명 조회
												// *		1 : 코드명으로 코드값 조회
												// *		2 : 코드구분 리턴	
//						LPCTSTR _szKey;			// 종목 코드	
// 함수 설명         : 삼성 CodeMaster에서 선물종목마스터 데이터를 얻어옮
///////////////////////////////////////////////////////////////////////////////// 
CString CMasterDataManager::GetCodeInfo(LPCTSTR _szCodeKey, short _nFlag, LPCTSTR _szKey)
{
	switch(_nFlag)
	{
		case 0:
			return GetCodeName(_szKey);
		case 1:
			AfxMessageBox("CMasterDataManager::GetCodeInfo 1 처리안됨");
			return "";
		case 2:
			{
				int nMarketType = GetMarketTypeFromCode(_szKey);
				CString strData;
				strData.Format("%d", nMarketType);
				return strData;
			}
		default:
			break;
	}
	
//@신영제거
//	CString strCode = _szKey;		// 단축코드 (입력) 
//	CString strMarketKey = _szCodeKey;
//	int	nFlag = _nFlag;
//
//	BYTE cType;
//	CString strType;
//
//	switch(nFlag)
//	{
//	// 종목코드를 통해 종목명을 얻으려면
//	// GetJongmokNameFromCode 함수 이용
//	case 0:			// 코드값으로 코드명 조회
//		if(strMarketKey == "UMASTER")		// 업종
//			theApp.m_szTemp = ::dll_GetJongmokName(CODE_ALLUPJONG, strCode);
//		else if(strMarketKey == "PMASTER")	// 옵션
//			theApp.m_szTemp = ::dll_GetJongmokName(CODE_OPTION, strCode);
//		else if(strMarketKey == "FMASTER")	// 선물
//		{
//			CString strName = dll_GetJongmokName(CODE_FUTURE, strCode);
//			if(strName.GetLength()<1)
//			{
//				strName = dll_GetJongmokName(CODE_STARFUT, strCode);
//			}
//			theApp.m_szTemp = strName;
//		}
//		else if(strMarketKey == "FRMASTER")	// 해외
//			theApp.m_szTemp = ::dll_GetJongmokName(CODE_FRMJISU, strCode);
//		else if(strMarketKey == "WMASTER")	// ELW
//			theApp.m_szTemp = ::dll_GetJongmokName(CODE_ELW, strCode);
//		else								// 종목
//			theApp.m_szTemp = ::dll_GetJongmokName(CODE_ALLJONGMOK, strCode);
//		return theApp.m_szTemp;
//		break;
//	// 종목코드를 통해 시장구분을 얻으려면
//	// GetMarketTypeFromCode 함수 이용
//	case 2:		// 코드값으로 코드구분 리턴
//				//	"1"		= 	상장 종목 
//				//	"2"		= 	코스닥 종목
//				//	"3"		= 	프리보드 
//				//	"4"		= 	선물, 스프레드 				
//				//	"5"		= 	옵션 						
//				//	"8"		= 	채권 						
//				//	"9"		= 	수익증권 					
//				//	"A"		= 	ETF코드 					
//				//	"B"		= 	KOSPI 200 업종, KOSPI 100 업종, 장내업종, KRX, KRX업종
//				//	"C"		= 	코스닥 업종, 코스닥 50 		
//				//	"G"		= 	개별주식옵션  				
//				//	"L"		= 	프리보드업종 				
//				//	"M"		= 	스타선물 					
//				//	"N"		=	주식워런트 ELW 	
//		cType = dll_GetJongMarket((LPCTSTR)(LPCSTR)strCode);
//		theApp.m_szTemp = dll_GetChangeMarketCode(cType, false);
//		return theApp.m_szTemp;
//	default:
//		break;
//	}

//@Solomon090819	char cMarketType = m_itemMaster.GetItemType((LPCSTR)_szKey);
//@Solomon090819	switch(cMarketType)
//@Solomon090819	{
//@Solomon090819	case IMT_KOSPI_STOCK:	return "1";
//@Solomon090819	case IMT_KOSDAQ_STOCK:	return "2";
//@Solomon090819	case IMT_ELW:			return "N";
//@Solomon090819	case IMT_KOSPI_UPJONG:	return "B";
//@Solomon090819	case IMT_KOSDAQ_UPJONG:	return "C";
//@Solomon090819	case IMT_KFREEBOARD:	return "3";
//@Solomon090819	case IMT_KFUTURE:
//@Solomon090819	case IMT_FUTURE_SPREAD:	return "4";
//@Solomon090819	case IMT_KOPTION:		return "5";
//@Solomon090819	}

	return "";
}

void CMasterDataManager::SetPosition(LPCTSTR szKey, short nX, short nY)
{
	//@solomon m_CodeOcx.SetPosition(szKey, nX, nY);
}

CString CMasterDataManager::GetMaxHangSaGa()
{
	//@solomon return m_CodeOcx.GetMaxHangSaGa();
	return "";
}

long CMasterDataManager::ShowCodeTableWithPosition(LPCTSTR _szCodeKey, const VARIANT& _hWnd, short nX, short nY)
{
	//@solomon return m_CodeOcx.ShowCodeTableWithPosition(_szCodeKey, _hWnd, nX, nY);
	return 0;
}

BOOL CMasterDataManager::RemoveAllListData()
{
	//@solomon return m_CodeOcx.RemoveAllListData();
	return TRUE;
}

BOOL CMasterDataManager::InsertHistory(short nMaxCount, long nType, LPCTSTR Code, LPCTSTR CodeName)
{
	//@solomon return m_CodeOcx.InsertHistory(nMaxCount, nType, Code, CodeName);
	return TRUE;
}

CString CMasterDataManager::GetHistoryList(short nMaxCount, long nType)
{
	//@solomon return m_CodeOcx.GetHistoryList(nMaxCount, nType);
	return "";
}

long CMasterDataManager::GetGlobalDataTypeList()
{
	//@solomon return m_CodeOcx.GetGlobalDataTypeList();
	return 0;
}

long CMasterDataManager::GetCodePointer(LPCTSTR _szCodeKey)
{
	//@solomon return m_CodeOcx.GetCodePointer(_szCodeKey);
	return 0;
}
// }} Code.ocx


// 주식종목마스터에서 업종에 해당하는 종목구하기
// 리턴되는 형태는 마스터구하는 데이터형
// 	int CMasterDataManager::GetJMaster_Upcode(LPCTSTR szUpcode, long pReturnList)
// 	{
// 		CList<CJMaster, CJMaster>* pRList = (CList<CJMaster, CJMaster>*)pReturnList;
// 		CList<CJMaster, CJMaster>* list = (CList<CJMaster, CJMaster>*)GetCodePointer("JMASTER"); 
// 
// 		int i=0;
// 		for(POSITION pos=list->GetHeadPosition(); pos;)
// 		{
// 			CJMaster data = list->GetNext(pos);
// 			if(data.m_szS.Compare(szUpcode)==0)
// 			{
// 				pRList->AddTail(data); i++;
// 			}
// 		}
// 		return i;
// 	}
// 
// 	// 코스닥종목마스터에서 업종에 해당하는 종목구하기
// 	// 리턴되는 형태는 마스터구하는 데이터형
// 	int CMasterDataManager::GetKMaster_Upcode(LPCTSTR szUpcode, long pReturnList)
// 	{
// 		CList<CKMaster, CKMaster>* pRList = (CList<CKMaster, CKMaster>*)pReturnList;
// 		CList<CKMaster, CKMaster>* list = (CList<CKMaster, CKMaster>*)GetCodePointer("KMASTER"); 
// 
// 		int i=0;
// 		for(POSITION pos=list->GetHeadPosition(); pos;)
// 		{
// 			CKMaster data = list->GetNext(pos);
// 			if(data.m_szUpCodel.Compare(szUpcode)==0)
// 			{
// 				pRList->AddTail(data); i++;
// 			}
// 		}
// 		return i;
// 	}


// BOOL CMasterDataManager::IsKOSPI(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsKosdaq(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsStock(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsFuture(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsSpread(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsStockOption(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsOption(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsCall(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsPut(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsKP200(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::IsKQ50(LPCSTR szCode)
// {
// 	return FALSE;
// }
// 
// BOOL CMasterDataManager::CheckKP200Sect(LPCSTR szCode)
// {
// 	return FALSE;
// }

// int CMasterDataManager::LoadMASTER_J_K()
// {
// 	return m_nJ_K_Count;
// }
// 
// void CMasterDataManager::UnLoadMASTER_J_K()
// {
// 	m_nJ_K_Count = 0;
// 	m_nJ_K_Count_J = 0;
// 	m_nJ_K_Count_K = 0;
// }
// 
// int CMasterDataManager::J_K_JMasterCount()
// {
// 	return m_nJ_K_Count_J;
// }
// 
// int CMasterDataManager::J_K_KMasterCount()
// {
// 	return m_nJ_K_Count_K;
// }
//
// int CMasterDataManager::_SortName(const void* p1, const void* p2)
// {
// 	CJMaster *pChild1 = *(CJMaster**)p1;
// 	CJMaster * pChild2 = *((CJMaster**)p2);
// 
// 	return pChild1->m_szName.Compare(pChild2->m_szName);
// }
// 
// int CMasterDataManager::_SortCode(const void* p1, const void* p2)
// {
// 	CJMaster *pChild1 = *(CJMaster**)p1;
// 	CJMaster * pChild2 = *((CJMaster**)p2);
// 
// 	return pChild1->m_szCode.Compare(pChild2->m_szCode);
// }
// 
// 
// void CMasterDataManager::CodeSort(LPCSTR szMaster, void* pList, int nCount)
// {
// }
// 
// 
// void  CodeSort_JMaster(void* pList, int nCount)
// {
// }

///////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2006.08.28
// 함수 이름         : CMasterDataManager::GetOptionJongMst
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 :	CStringArray* pastrJongCode;	// 옵션 종목 코드 
//						CStringArray* pastrJongName;	// 옵션 종목 명 	
//						CStringArray* pastrJongEngName;	// 옵션 영문 종목 명	
//						CStringArray* pastrEventPrice;	// 행사 가격	
//						CStringArray* pastrFinalYM;		// 만기 년월	
//						CStringArray* pastrValueGubun;	// 가격 구분	
// 함수 설명         : 삼성 CodeMaster에서 옵션종목마스터 데이터를 얻어옮
///////////////////////////////////////////////////////////////////////////////// 
BOOL CMasterDataManager::GetOptionJongMst(CStringArray *&pastrJongCode,	CStringArray *&pastrJongName, 
										  CStringArray *&pastrJongEngName, CStringArray *&pastrEventPrice,	
                                          CStringArray *&pastrFinalYM,	CStringArray *&pastrValueGubun	  )
{
	return GetMasterDataInUpDetails("PMASTER", *pastrJongCode, *pastrJongName, "");
	pastrJongCode->RemoveAll();
	pastrJongName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;

	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_OPTION_K200;
	//codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME | CDTYPE_MONTH | CDTYPE_HSGA;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	//_DoParsingCode(strRet, *pastrJongCode, *pastrJongName, true);
	_DoParsingCodePMaster(strRet, *pastrJongCode, *pastrJongName);

//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_KOPTION, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 		pastrJongCode->Add(str);
// 		str = ppData[nIndx]->szName; str.TrimRight();
// 		pastrJongName->Add(str);
// 	}
//@Solomon090819 <--
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2006.08.28
// 함수 이름         : CMasterDataManager::GetFutureJongMst
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 :	CStringArray* pastrJongCode;	// 종목 코드 
//						CStringArray* pastrJongName;	// 한글 종목 명 	
//						CStringArray* pastrJongEngName;	// 영문 종목 명	
// 함수 설명         : 삼성 CodeMaster에서 선물종목마스터 데이터를 얻어옮
///////////////////////////////////////////////////////////////////////////////// 
BOOL CMasterDataManager::GetFutureJongMst(CStringArray *&pastrJongCode,	CStringArray *&pastrJongName, CStringArray *&pastrJongEngName)
{
	pastrJongCode->RemoveAll();
	pastrJongName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_FUTURE_K200;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	_DoParsingCode(strRet, *pastrJongCode, *pastrJongName);
	pastrJongCode->Add(DEF_CONTINUE_FUT_CODE);
 	pastrJongName->Add(SZDEF_CONTINUE_FUT_CODE);

//@Solomon090819-->
// 	__int64 nType64 = 0;
// 	nType64 |= __int64(1) << KFUTURES_MASTER;
// 	nType64 |= __int64(1) << KOSPI_FUTURE_SPREAD;
// //	nType64 |= __int64(1) << KFUTURES_LINK;
// 
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData2(nType64, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 		pastrJongCode->Add(str);
// 		str = ppData[nIndx]->szName; str.TrimRight();
// 		pastrJongName->Add(str);
// 	}
// 	
// 	pastrJongCode->Add("99999999");
// 	pastrJongName->Add("연결선물");
//@Solomon090819<--
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2006.07.05
// 함수 이름         : CMasterDataManager::GetKSPUpMst
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 :	CStringArray* pastrUpCode;		// 업종 코드 
//						CStringArray* pastrUpName;		// 한글 업종 명 	
//						CStringArray* pastrUpEngName;	// 영문 업종 명	
// 함수 설명         : KSP 업종 마스터
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetKSPUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	if(nOption==0)	codeR.kind = CDKIND_KOSPI;
	else			codeR.kind = CDKIND_INDEX_KOSPI;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2006.07.05
// 함수 이름         : CMasterDataManager::GetKSQUpMst
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 :	CStringArray* pastrUpCode;		// 업종 코드 
//						CStringArray* pastrUpName;		// 한글 업종 명 	
//						CStringArray* pastrUpEngName;	// 영문 업종 명	
// 함수 설명         : 코스닥 업종 마스터
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetKSQUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	if(nOption==0)	codeR.kind = CDKIND_KOSDAQ;
	else			codeR.kind = CDKIND_INDEX_KOSDAQ;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2006.07.05
// 함수 이름         : CMasterDataManager::GetKNXUpMst
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 :	CStringArray* pastrUpCode;		// 업종 코드 
//						CStringArray* pastrUpName;		// 한글 업종 명 	
//						CStringArray* pastrUpEngName;	// 영문 업종 명	
// 함수 설명         : 코넥스 업종 마스터
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetKNXUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	if(nOption==0)	codeR.kind = CDKIND_KNX;
	else			codeR.kind = CDKIND_INDEX_KNX;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2006.09.13
// 함수 이름         : CMasterDataManager::GetFRMst
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 :	CStringArray* pastrCode;		// 해외주요지수 심벌
//						CStringArray* pastrName;		// 한글지수 명 	
// 함수 설명         : 해외주요지수 마스터	CODE_FRMAJORIDX
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetFRMst(CStringArray *&pastrCode, CStringArray *&pastrName)
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2006.09.13
// 함수 이름         : CMasterDataManager::GetFRNationMst
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 :	CStringArray* pastrCode;		// 지수 코드 
//						CStringArray* pastrName;		// 한글 국가 명 	
// 함수 설명         : 해외국가 마스터	CODE_FRNATION
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetFRNationMst(CStringArray *&pastrCode, CStringArray *&pastrName)
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2006.09.13
// 함수 이름         : CMasterDataManager::GetELWMst
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 :	CStringArray* pastrCode;		// 업종 코드 
//						CStringArray* pastrName;		// 한글 업종 명 	
// 함수 설명         : ELW 마스터	CODE_ELW
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetELWMst(CStringArray *&pastrCode, CStringArray *&pastrName)
{
//@신영제거	return dll_GetMasterData(CODE_ELW , *pastrCode, *pastrName);
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2006.09.13
// 함수 이름         : CMasterDataManager::GetGroupMst
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 :	CStringArray* pastrCode;		// 그룹 코드 
//						CStringArray* pastrName;		// 한글 그룹 명 	
// 함수 설명         : ELW 마스터	CODE_ELW
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetGroupMst(CStringArray *&pastrCode, CStringArray *&pastrName, int nOption)
{
	pastrCode->RemoveAll();
	pastrName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_SUBSIDIARY;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrCode, *pastrName);

//@Solomon090819 -->
// 	// 0 : 거래소, 1 : 모두
// 	int nCount = m_itemMaster.GetGroupList2(1, *pastrCode, *pastrName);
// 	if(nCount <= 0)	return FALSE;

//@Solomon090819<--
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2006.07.05
// 함수 이름         : CMasterDataManager::GetKSQUpMst
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 :	CStringArray* pastrUpCode;		// 업종 코드 
//						CStringArray* pastrUpName;		// 한글 업종 명 	
//						CStringArray* pastrUpEngName;	// 영문 업종 명	
// 함수 설명         : KRX 업종 마스터
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetKRXUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	if(nOption==0)	codeR.kind = CDKIND_KRX;
	else			codeR.kind = CDKIND_INDEX_KRX;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2007.01.11
// 함수 이름         : CMasterDataManager::GetStockDetailInfo
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 : CString strCode;		// 종목코드
// 함수 설명         : 종목상세정보
/////////////////////////////////////////////////////////////////////////////////////////////
CString CMasterDataManager::GetStockDetailInfo(CString strCode, int nType)
{
	return "";
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2007.01.25
// 함수 이름         : CMasterDataManager::GetLastCode
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 : CString strMarketCode;		// 시장코드 01:주식
// 함수 설명         : 히스토리 마지막 코드
/////////////////////////////////////////////////////////////////////////////////////////////
CString CMasterDataManager::GetLastCode(CString strMarketCode)
{
	if(atoi(strMarketCode) < 0 || atoi(strMarketCode) > 6)
	{
		strMarketCode = "01";
	}
	
	int nHistoryCount = 0;
	CString strRecentCode;

	return strRecentCode;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2007.04.16
// 함수 이름         : CMasterDataManager::GetK200UpMst
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 :	CStringArray* pastrUpCode;		// 업종 코드 
//						CStringArray* pastrUpName;		// 한글 업종 명 	
//						CStringArray* pastrUpEngName;	// 영문 업종 명	
// 함수 설명         : KSP 업종 마스터
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetK200UpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_INDEX_K200;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_KOSPI200_UPJONG, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 		pastrUpCode->Add(str);
// 		str = ppData[nIndx]->szName; str.TrimRight();
// 		pastrUpName->Add(str);
// 	}
//@Solomon090819 <--
	return TRUE;
}

BOOL CMasterDataManager::GetK100UpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_INDEX_K100;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

//@Solomon090819 -->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_KOSPI100_UPJONG, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 		pastrUpCode->Add(str);
// 		str = ppData[nIndx]->szName; str.TrimRight();
// 		pastrUpName->Add(str);
// 	}

//@Solomon090819<--
	return TRUE;
}

BOOL CMasterDataManager::GetKSTRUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_INDEX_KOSTAR;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

	//@Solomon090819 -->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_KOSTAR_UPJONG, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 		pastrUpCode->Add(str);
// 		str = ppData[nIndx]->szName; str.TrimRight();
// 		pastrUpName->Add(str);
// 	}
	//@Solomon090819 <--

	return TRUE;
}

BOOL CMasterDataManager::GetFREEUpMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	if(nOption==0)
		codeR.kind = CDKIND_FREEBOARD;
 	else
 		codeR.kind = CDKIND_INDEX_FREEBOARD;

	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);
	
	//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_FREEBOARD_UPJONG, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 		pastrUpCode->Add(str);
// 		str = ppData[nIndx]->szName; str.TrimRight();
// 		pastrUpName->Add(str);
// 	}
	//@Solomon090819 <--

	return TRUE;
}

BOOL CMasterDataManager::GetForeignMst(int nKind, CString strCode, CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;

	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	if ((strCode.GetLength() > 0) && strCode.Compare(_T("NONE")))
		memcpy(codeR.code, strCode, min(sizeof(codeR.code), strCode.GetLength()));
	
	codeR.kind = nKind;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	//_DoParsingCodeForeign(strRet, *pastrUpCode, *pastrUpName);
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2010.03.16
// 함수 이름         : CMasterDataManager::GetFXMst
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 :	CStringArray* pastrUpCode;		// 업종 코드 
//						CStringArray* pastrUpName;		// 업종 명 	
// 함수 설명         :  FX 마스터
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMasterDataManager::GetFXMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_GFX_INDEX;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCodeFX(strRet, *pastrUpCode, *pastrUpName);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// 만든 날짜         : 2007.12.24
// 함수 이름         : CMasterDataManager::GetRecentCode
// 리턴 타입         : BOOL
// 리턴 타입 설명    : 
// 파라미터			 :	CString szCode
//						CString& rszCode	
//						int nCodeType
// 함수 설명         :  선물,옵션 종목코드 만기월지났을 경우 확인을 위해서 ...
//						만기월 지나면 FALSE이고 rszCode에 가장 최근월물 리턴.
//						만기월이 안지났으면 TRUE리턴하고 rszCode에 szCode를 그대로 넘겨줌.
/////////////////////////////////////////////////////////////////////////////////////////////
//alzioyes:20110526 연결선물일때 최근월물로 변환하는 루틴 변경.
BOOL CMasterDataManager::GetRecentCode(CString szCode, CString& rszCode, int nCodeType)
{
	if(m_RecentFutureCodeTable.Lookup(szCode, rszCode)==TRUE)
		return TRUE;

	BYTE rCodeType = GetCodeTypeFromCode(szCode);
	rszCode = szCode;

	if(rCodeType==CDRCodeTypeInfo::CODE_KFUTURE) //선물
	{
		CStringArray* pastrJongCode;
		CStringArray* pastrJongName;	// 옵션 종목 명 
		CStringArray* pastrJongEngName;	// 옵션 영문 종목 명
		
		pastrJongCode = new CStringArray;
		pastrJongName = new CStringArray;
		pastrJongEngName = new CStringArray;
		CString strCodeSelected;
		if ( szCode.CompareNoCase("99999999") == 0 )
			GetFutureJongMst(pastrJongCode, pastrJongName, pastrJongEngName);
		else if ( szCode.CompareNoCase("10599999") == 0 )
			GetMiniFutureJongMst(pastrJongCode, pastrJongName, pastrJongEngName);
		
		
		if(pastrJongCode->GetSize()>0)
			rszCode = pastrJongCode->GetAt(0);
		
		delete pastrJongCode;
		delete pastrJongName;
		delete pastrJongEngName;
		
		m_RecentFutureCodeTable.SetAt(szCode, rszCode);
		return TRUE;
	}
	else if(rCodeType==CDRCodeTypeInfo::CODE_FUOPT_COMMODITY)	//상품선물.
	{
/*		CStringArray *pArrCode, *pArrName, *pArrEng;
		pArrCode = new CStringArray;
		pArrName = new CStringArray;
		pArrEng = new CStringArray;
		if (!GetForeignMst(CDKIND_KOFEXLIST, "", pArrCode, pArrName, pArrEng))
			return FALSE;

		for (int i=0; i<pArrCode->GetSize(); i++)
		{
			CStringArray arrDCode, arrDName;
			if (!GetMasterDataInUpDetails("상품선물", arrDCode, arrDName, pArrCode->GetAt(i)))
				continue;

			if (arrDCode.GetSize() > 0 && strncmp(arrDCode.GetAt(0), szCode, 3) == 0)
			{
				rszCode = arrDCode.GetAt(0);
				break;
			}
		}

		pArrCode->RemoveAll();		delete pArrCode;
		pArrName->RemoveAll();		delete pArrName;
		pArrEng->RemoveAll();		delete pArrEng;

		m_RecentFutureCodeTable.SetAt(szCode, rszCode);
*/
	}
	else if(rCodeType==CDRCodeTypeInfo::CODE_STOCKFUT)	//주식선물.
	{
/*
		CStringArray arrCode, arrName;
		if (!GetJFMaster(arrCode, arrName))
			return FALSE;

		for (int i=0; i<arrCode.GetSize(); i++)
		{
			CStringArray arrDCode, arrDName;
			if (!GetMasterDataInUpDetails("JFMASTER", arrDCode, arrDName, arrCode.GetAt(i)))
				continue;

			if (arrDCode.GetSize() > 0 && strncmp(arrDCode.GetAt(0), szCode, 3) == 0)
			{
				rszCode = arrDCode.GetAt(0);
				break;
			}
		}

		m_RecentFutureCodeTable.SetAt(szCode, rszCode);
*/
	}
	
	return TRUE;
	
}

// BOOL CMasterDataManager::GetRecentCode(CString szCode, CString& rszCode, int nCodeType)
// {
// 	rszCode = szCode;
// 
// 	if(nCodeType==IMaster_Future) //선물
// 	{
// 		CStringArray* pastrJongCode;
// 		CStringArray* pastrJongName;	// 옵션 종목 명 
// 		CStringArray* pastrJongEngName;	// 옵션 영문 종목 명
// 
// 		pastrJongCode = new CStringArray;
// 		pastrJongName = new CStringArray;
// 		pastrJongEngName = new CStringArray;
// 		CString strCodeSelected;
// 		GetFutureJongMst(pastrJongCode, pastrJongName, pastrJongEngName);
// 
// 		if(pastrJongCode->GetSize()>0)
// 			rszCode = pastrJongCode->GetAt(0);
// 
// 		delete pastrJongCode;
// 		delete pastrJongName;
// 		delete pastrJongEngName;
// 
// 		return TRUE;
// 	}
// 
// 	return TRUE;
// }

BOOL CMasterDataManager::GetMasterDataInUpDetails(CString _szType, CStringArray &arrCode, CStringArray &arrName, CString strKey)
{
	CString szType = _szType;
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;

	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));

	codeR.type = CDTYPE_CODE | CDTYPE_NAME;

	if (strKey.CompareNoCase(_T("NONE")))
		memcpy(codeR.code, (LPCSTR)strKey, strKey.GetLength());

	BOOL bEachCall = FALSE;

	if(!_szType.CompareNoCase("PMASTER"))
	{
		codeR.kind = CDKIND_OPTION_K200;
		codeR.type = CDTYPE_CODE | CDTYPE_NAME | CDTYPE_MONTH | CDTYPE_HSGA;
		bEachCall = TRUE;
	}
	else if(!_szType.CompareNoCase("MINI_PMASTER"))
	{
		codeR.kind = CDKIND_MINI_OPTION;
		codeR.type = CDTYPE_CODE | CDTYPE_NAME | CDTYPE_MONTH | CDTYPE_HSGA;
		bEachCall = TRUE;
	}
	else if(!_szType.CompareNoCase("WEEKLY_PMASTER"))
	{
		codeR.kind = CDKIND_WEEKLY_OPTION;
		codeR.type = CDTYPE_CODE | CDTYPE_NAME | CDTYPE_MONTH | CDTYPE_HSGA;
		bEachCall = TRUE;
	}
	else if(!_szType.CompareNoCase("JFMASTER"))
	{
		// 2012.2.3 박성경
		//codeR.code[0] = 'A';
		//memcpy(&codeR.code[1], (LPCSTR)strKey, strKey.GetLength());
		memcpy(codeR.code, (LPCSTR)strKey, strKey.GetLength());
		
		codeR.kind = CDKIND_SFUTURE;
	}
	else if(!_szType.CompareNoCase("JPMASTER"))
	{
		codeR.type = CDTYPE_CODE | CDTYPE_NAME | CDTYPE_MONTH | CDTYPE_HSGA;
		codeR.kind = CDKIND_SOPTION;
		bEachCall = TRUE;
	}
	else if (!_szType.CompareNoCase("UMaster"))			codeR.kind = CDKIND_KOSPI;
	else if (!_szType.CompareNoCase("kumASTER"))		codeR.kind = CDKIND_KOSDAQ;
	else if (!_szType.CompareNoCase("TMMASTER"))		codeR.kind = CDKIND_THEME;
	else if (!_szType.CompareNoCase("ETFMASTER"))		codeR.kind = CDKIND_ETF;
//	else if (!_szType.CompareNoCase("FREEJMASTER"))		codeR.kind = CDKIND_FREEBOARD;
	else if (!_szType.CompareNoCase("KRXUMaster"))		codeR.kind = CDKIND_KRX;
	else if (!_szType.CompareNoCase("KNXUMaster"))		codeR.kind = CDKIND_KNX;
	else if (!_szType.CompareNoCase("K200UMaster"))		codeR.kind = CDKIND_INDEX_K200;
	else if (!_szType.CompareNoCase("K100UMaster"))		codeR.kind = CDKIND_INDEX_K100;
	else if (!_szType.CollateNoCase("GRPMASTER"))		codeR.kind = CDKIND_SUBSIDIARY;
	else if (!_szType.CompareNoCase("ELWINDEXMASTER"))	codeR.kind = CDKIND_ELW_BASE;
	else if (!_szType.CompareNoCase("ELWISSUEMASTER"))	codeR.kind = CDKIND_ELW_PB;
	else if (!_szType.CompareNoCase("KOSPI200"))		codeR.kind = CDKIND_KOSPI200;
	else if (!_szType.CompareNoCase("OLDFMASTER"))		return _DoGetOldFutureMaster(arrCode, arrName);
	else if (!_szType.CompareNoCase("ETNMASTER"))		codeR.kind = CDKIND_ETN;
	else
	{
		bEachCall = TRUE;
		if (!_szType.CompareNoCase("FRMASTER"))				codeR.kind = CDKIND_FOREIGN_MAJOR;		// 해외주요지수
		else if (!_szType.CompareNoCase("FRALLMASTER"))		codeR.kind = CDKIND_FOREIGN_ALL;		// 해외전종목
		else if (!_szType.CompareNoCase("해외_지수"))		codeR.kind = CDKIND_FOREIGN;			// 다우30
		else if (!_szType.CompareNoCase("해외_종목"))		codeR.kind = CDKIND_FOREIGN;			// 나스닥100
		else if (!_szType.CompareNoCase("해외_선물"))		codeR.kind = CDKIND_FOREIGN;			// S&P500
		else if (!_szType.CompareNoCase("해외_금리"))		codeR.kind = CDKIND_FOREIGN;			// CME선물
		else if (!_szType.CompareNoCase("해외_국채"))		codeR.kind = CDKIND_FOREIGN;			// 기타선물
		else if (!_szType.CompareNoCase("해외_환율"))		codeR.kind = CDKIND_FOREIGN;			// 국내금리
		else if (!_szType.CompareNoCase("FOFLMASTER"))		codeR.kind = CDKIND_FOINDEX_FO;			// FO지표
		else if (!_szType.CompareNoCase("FOMASTER"))		codeR.kind = CDKIND_FOINDEX_MAJOR;		// FO 주요지수
		else if (!_szType.CompareNoCase("FOFIMASTER"))		codeR.kind = CDKIND_FOINDEX_FI;			// FO F.I지수
		else if (!_szType.CompareNoCase("FOVOLMASTER"))		codeR.kind = CDKIND_FOINDEX_FUTURE;		// FO 선물잔량
		else if (!_szType.CompareNoCase("FOMAKETMASTER"))	codeR.kind = CDKIND_FOINDEX_MARKET;		// FO 시장동향
		else if (!_szType.CompareNoCase("상품선물"))		codeR.kind = CDKIND_KOFEX;				// 국내상품선물
		else if (!_szType.CompareNoCase("COMMODITYMASTER"))	codeR.kind = CDKIND_KOFEX_FUTURE;		// 국내상품선물
		else if (!_szType.CompareNoCase("달러옵션"))		codeR.kind = CDKIND_KOFEX_OPTION;		// 달러(USD)옵션
		else	bEachCall = FALSE;
	}

	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	if (bEachCall==FALSE)
	{
		if(	codeR.kind==CDKIND_KOSPI	||
			codeR.kind==CDKIND_KOSDAQ	||
			codeR.kind==CDKIND_KRX		||
			codeR.kind==CDKIND_KNX			)			
		{
			_DoParsingCode(strRet, arrCode, arrName, false, 1);
		}
		else if (codeR.kind==CDKIND_THEME)
		{
			if(strKey.IsEmpty())	//Sort시키기.
			{
				_DoParsingCode(strRet, arrCode, arrName);
				_DoSortCode(arrCode, arrName);
			}
			else
			{
				_DoParsingCode(strRet, arrCode, arrName, false, 1);
			}
		}
		else
		{
			_DoParsingCode(strRet, arrCode, arrName);
		}
	}
	else
	{
		if (_szType.CompareNoCase("PMASTER")==0)
			_DoParsingCodePMaster(strRet, arrCode, arrName);
		else if (_szType.CompareNoCase("JPMASTER")==0)
			_DoParsingCodeJPMaster(strRet, arrCode, arrName);
		else if (_szType.CompareNoCase("달러옵션")==0)
			_DoParsingCodeDPMaster(strRet, arrCode, arrName);
		else if (_szType.CompareNoCase("MINI_PMASTER") == 0)
			_DoParsingCodeMiniMaster(strRet, arrCode, arrName);
		else if (_szType.CompareNoCase("WEEKLY_PMASTER") == 0)
			_DoParsingCodeWeeklyMaster(strRet, arrCode, arrName);
		else	//	해외
			_DoParsingCode(strRet, arrCode, arrName);
			//_DoParsingCodeForeign(strRet, arrCode, arrName);
	}

	CString strTemp;
	if (arrCode.GetSize() > 0)
		strTemp = arrCode.GetAt(0);

	int nCount, i;
	if (codeR.kind == CDKIND_KOFEX)
	{
		nCount = sizeof(g_pszCFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strncmp(strTemp, g_pszCFTiesCode[i], 3) == 0)
			{
				strTemp = g_pszCFTiesCode[i];
				arrCode.Add(strTemp);

				strTemp.Format(_T("%s 연계선물"), g_pszCFTiesName[i]);
				arrName.Add(strTemp);

				break;
			}
		}
	}
	else if (codeR.kind == CDKIND_SFUTURE)
	{
		nCount = sizeof(g_pszSFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strncmp(strTemp, g_pszSFTiesCode[i], 3) == 0)
			{
				strTemp = g_pszSFTiesCode[i];
				arrCode.Add(strTemp);

				strTemp.Format(_T("%s 연계선물"), g_pszSFTiesName[i]);
				arrName.Add(strTemp);
				break;
			}
		}
	}

	return TRUE;
}

BYTE CMasterDataManager::GetCodeTypeFromCode(LPCSTR strCode)
{
	BYTE rCodeType = 0xFF;
// 	if(strcmp(strCode, DEF_CONTINUE_FUT_CODE)==0)
// 	{
// 		rCodeType = CDRCodeTypeInfo::CODE_KFUTURE;
// 		return rCodeType;
// 	}
	if (strlen(strCode)==8 && strncmp(&strCode[3], "99999", 5)==0)
	{
		if(strcmp(strCode, DEF_CONTINUE_FUT_CODE)==0)		
		{
			rCodeType = CDRCodeTypeInfo::CODE_KFUTURE;
			return rCodeType;
		}
		else if( strcmp(strCode, DEF_CONTINUE_MINI_FUT_CODE) == 0)
		{
			rCodeType = CDRCodeTypeInfo::CODE_KFUTURE;
			return rCodeType;
		}

		int i, nCount = sizeof(g_pszCFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strcmp(strCode, g_pszCFTiesCode[i]) == 0)
				return CDRCodeTypeInfo::CODE_FUOPT_COMMODITY;
		}

		nCount = sizeof(g_pszSFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strcmp(strCode, g_pszSFTiesCode[i]) == 0)
				return CDRCodeTypeInfo::CODE_STOCKFUT;
		}
	}
	
	rCodeType = _DoPreChkCodeSB(&m_RecentCodeTypeList, strCode);
	if(rCodeType != 0xFF )
		return rCodeType;



	HWND hWinixWnd = GetHWND();
	if(!hWinixWnd) return CMasterTypeInfo::STOCK_CHART;

	if(strlen(strCode)==10)	//FO지수
	{
		rCodeType = CDRCodeTypeInfo::CODE_FUOPT_JIPYO;
		_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
		return rCodeType;
	}

	rCodeType = 0xFF;
	//rCodeType = CDRCodeTypeInfo::CODE_KOSPI_STOCK;
	
	int rRet = (int)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(codeTYPE, NULL), (LPARAM)(LPCSTR)strCode);

// KHD : 수정 상품 선물 및 Spread 타입 수정 
	if(rRet < 0) 
	{   
		CString strItemCode = strCode;
		CString strTemp;
		strTemp = strItemCode.Left(1);
		if(IS_SPREAD_CODE_1(strTemp)) // 0024696: 파생상품 코드체계 변경 제도개선
		{
			rRet = spreadType;
			strTemp = strItemCode.Mid(1, 2);
			if(atoi(strTemp) >= 60 && atoi(strTemp) <= 99)
				rRet = cfutureType;
		}
		else if(IS_FUT_CODE_1(strTemp)) // 0024696: 파생상품 코드체계 변경 제도개선
		{
			strTemp = strItemCode.Mid(1, 2);
			if( (atoi(strTemp) >= 10 && atoi(strTemp) <= 59)
				|| ( (strTemp[0] >= 'B' && strTemp[0] <= 'Z')	// 주식선물 제도개선 B0 ~ ZZ 까지
					&& ((strTemp[1] >= '0' && strTemp[1] <= '9') || (strTemp[1] >= 'A' && strTemp[1] <= 'Z')) ) 
				|| ((strTemp[0] >= '0' && strTemp[0] <= '9') && (strTemp[1] >= 'A' && strTemp[1] <= 'Z'))) // 0028557: [7111] 주식선물 신규 상장종목에 대한 종목 선택 불가 현상
				rRet = sfutureType;
			else if(atoi(strTemp) >= 60 && atoi(strTemp) <= 99)
				rRet = cfutureType;
		}
	}
// END
	switch(rRet)
	{
		case kospiType:
		case etfType:
		case elwType:
		case foreignType:	//20100930_JS.Kim 외국주 (화풍집단KDR(950010), 네프로아이티(950030))
		case etnType:
			{
				//return CDRCodeTypeInfo::CODE_KOSPI_STOCK;
				rCodeType = CDRCodeTypeInfo::CODE_KOSPI_STOCK;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case kosdaqType:
			{
				//return CDRCodeTypeInfo::CODE_KOSDAQ_STOCK;
				rCodeType = CDRCodeTypeInfo::CODE_KOSDAQ_STOCK;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case thirdType:
			{
				//return CDRCodeTypeInfo::CODE_ELW;
				rCodeType = CDRCodeTypeInfo::CODE_ELW;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case indexType:		// 업종
			{
				//if(strCode[0]=='0') return CDRCodeTypeInfo::CODE_KOSPI_UPJONG;
				//return CDRCodeTypeInfo::CODE_KOSDAQ_UPJONG;
				if(strCode[0]=='0') rCodeType = CDRCodeTypeInfo::CODE_KOSPI_UPJONG;
				else rCodeType = CDRCodeTypeInfo::CODE_KOSDAQ_UPJONG;

				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}

		case futureType:	// 선물
			{
				//return CDRCodeTypeInfo::CODE_KFUTURE;
				rCodeType = CDRCodeTypeInfo::CODE_KFUTURE;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case spreadType:	// 스프레드
			{
				//return CDRCodeTypeInfo::CODE_FUTURE_SPREAD;
				rCodeType = CDRCodeTypeInfo::CODE_FUTURE_SPREAD;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case callType:		// 콜옵션
		case putType:		// 풋옵션
			{
				//return CDRCodeTypeInfo::CODE_KOPTION;
				rCodeType = CDRCodeTypeInfo::CODE_KOPTION;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}

		case sfutureType:	// 주식선물
			{
				//return CDRCodeTypeInfo::CODE_STOCKFUT;
				rCodeType = CDRCodeTypeInfo::CODE_STOCKFUT;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case koptionType:	// 주식옵션
			{
				//return CDRCodeTypeInfo::CODE_STOCKOPT;
				rCodeType = CDRCodeTypeInfo::CODE_STOCKOPT;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}

		//case foreignType:	// 외국주
		case usType:
			{
				//return CDRCodeTypeInfo::CODE_US_ITEM;
				rCodeType = CDRCodeTypeInfo::CODE_US_ITEM;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				return rCodeType;
			}
		case cfutureType:
		{
				rCodeType = CDRCodeTypeInfo::CODE_FUOPT_COMMODITY;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rCodeType);
				return rCodeType;
		}
		//<<20100316_JS.Kim FX
// 박성경 >>
// 		case gfxType:
// 			{
// 				rCodeType = CDRCodeTypeInfo::CODE_FX;
// 				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
// 				return rCodeType;
// 			}
		//>>
// 박성경 <<

		default:
			//<<JS.Kim_20101019. 신국채 코드 추가
			//if (rRet >= THREE_BONDS && rRet <= MGOLD_FUTURE)
			if (rRet >= THREE_BONDS && rRet <= NEW_TEN_BONDS)
			//>>
			{
				rCodeType = CDRCodeTypeInfo::CODE_FUOPT_COMMODITY;
				_DoAddChkCodeSB(&m_RecentCodeTypeList, RecentCodeTypeList_Max, strCode, rCodeType);
				break;
			}

			if(strstr(strCode, "@") != NULL || strstr(strCode, "$") != NULL)
				rCodeType = CDRCodeTypeInfo::CODE_US_ITEM;

			break;
	}

	return rCodeType;
}

int CMasterDataManager::GetMarketTypeDetail(LPCSTR strCode)
{
	HWND hWinixWnd = GetHWND();
	if (!hWinixWnd) return -1;

	return (int)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(codeTYPE, NULL), (LPARAM)(LPCSTR)strCode);
}

BYTE CMasterDataManager::_DoPreChkCodeSB(List_ST_STRINGBYTE* pListST, LPCSTR strCode)
{
	ST_STRINGBYTE* pItem;
	
	for(POSITION pos=pListST->GetHeadPosition(); pos; )
	{
		pItem = pListST->GetNext(pos);
		if(pItem->m_szKey.Compare(strCode)==0)
			return pItem->m_Value;
	}
	return 0xFF;
}

void CMasterDataManager::_DoAddChkCodeSB(List_ST_STRINGBYTE* pListST, int nMaxCnt, LPCSTR strCode, BYTE bType)
{
	POSITION pos = pListST->GetHeadPosition();
	if(!pos)
	{
		// 		ST_STRINGBYTE* pItem = new ST_STRINGBYTE;
		// 		pItem->m_szKey = strCode;
		// 		pItem->m_Value = bType;
		// 		pListST->AddHead(pItem);
	}
	else
	{
		POSITION hPos=pListST->GetHeadPosition(),
			tPos;

		ST_STRINGBYTE* pItem;
		for(POSITION pos=hPos; pos; )
		{
			tPos = pos;
			pItem = pListST->GetNext(pos);
			if(pItem->m_szKey.Compare(strCode)==0)
			{
				if(tPos==hPos) return;
				else
				{
					pListST->RemoveAt(tPos);
					pListST->AddHead(pItem);
					return;
				}
			}
		}
		
		if(pListST->GetCount()==nMaxCnt)
			pListST->RemoveAt(tPos);
	}
	
	ST_STRINGBYTE* pItem = new ST_STRINGBYTE;
	pItem->m_szKey = strCode;
	pItem->m_Value = bType;
	pListST->AddHead(pItem);
}

BYTE CMasterDataManager::GetMarketTypeFromCode(LPCSTR strCode)
{
	BYTE rMarketType;
	if(strlen(strCode)==8 && strncmp(&strCode[3], "99999", 5)==0)
	{
		if(strcmp(strCode, DEF_CONTINUE_FUT_CODE)==0)		
		{
			rMarketType = CMasterTypeInfo::FUTOPT_CHART;
			return rMarketType;
		}
		else if( strcmp(strCode, DEF_CONTINUE_MINI_FUT_CODE) == 0)
		{
			rMarketType = CMasterTypeInfo::FUTOPT_CHART;
			return rMarketType;
		}

		int i, nCount = sizeof(g_pszCFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strcmp(strCode, g_pszCFTiesCode[i]) == 0)
				return CMasterTypeInfo::COMMODITY_CHART;
		}

		nCount = sizeof(g_pszSFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strcmp(strCode, g_pszSFTiesCode[i]) == 0)
				return CMasterTypeInfo::STOCKFUTOPT_CHART;
		}
	}
	
	rMarketType = _DoPreChkCodeSB(&m_RecentMarketTypeList, strCode);
	if(rMarketType != 0xFF )
		return rMarketType;

	if(strstr(strCode, "@") != NULL || strstr(strCode, "$") != NULL)
	{
		//return CMasterTypeInfo::FOREIGN_CHART;
		rMarketType = CMasterTypeInfo::FOREIGN_CHART;
		_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
		return rMarketType;
	}

	if(strlen(strCode) == 0)//KHD 없는경우
	{
		return CMasterTypeInfo::NONE_CHART;
	}
	if (strlen(strCode) <= 4)
	{
		//return CMasterTypeInfo::UPJONG_CHART;
		rMarketType = CMasterTypeInfo::UPJONG_CHART;
		_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
		return rMarketType;
	}
//@solomon	FO지수를 정상적으로 체크할 수 있어 막음.
/*	if(strlen(strCode)==10)	//FO지수
	{
		//return CMasterTypeInfo::FUOPT_JIPYO_CHART;
		rMarketType = CMasterTypeInfo::FUOPT_JIPYO_CHART;
		_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
		return rMarketType;
	}
*/
	HWND hWinixWnd = GetHWND();
	if(!hWinixWnd) return CMasterTypeInfo::STOCK_CHART;

	CString strItemCode = strCode;
	CString strTempCode = strCode;
	strTempCode.Replace(_T("N.A"), _T(""));
	
	int rRet = (int)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(codeTYPE, NULL), (LPARAM)(LPCSTR)strTempCode);
	if(rRet < 0)
	{
		CString strTemp;
		strTemp = strItemCode.Left(1);
		if(IS_SPREAD_CODE_1(strTemp)) // 0024696: 파생상품 코드체계 변경 제도개선
		{
			rRet = spreadType;
			strTemp = strItemCode.Mid(1, 2);// 주식선물일경우는 스프레드라도 주식선물로 돌려준다.
			if( (atoi(strTemp) >= 10 && atoi(strTemp) <= 59)
				|| ( (strTemp[0] >= 'B' && strTemp[0] <= 'Z')	// 주식선물 제도개선 B0 ~ ZZ 까지
					&& ((strTemp[1] >= '0' && strTemp[1] <= '9') || (strTemp[1] >= 'A' && strTemp[1] <= 'Z')) ) 
				|| ((strTemp[0] >= '0' && strTemp[0] <= '9') && (strTemp[1] >= 'A' && strTemp[1] <= 'Z'))) // 0028557: [7111] 주식선물 신규 상장종목에 대한 종목 선택 불가 현상
				rRet = sfutureType;
			else if(atoi(strTemp) >= 60 && atoi(strTemp) <= 99)
				rRet = cfutureType;
		}
		else if(IS_FUT_CODE_1(strTemp)) // 0024696: 파생상품 코드체계 변경 제도개선
		{
			strTemp = strItemCode.Mid(1, 2);
			if( (atoi(strTemp) >= 10 && atoi(strTemp) <= 59)
				|| ( (strTemp[0] >= 'B' && strTemp[0] <= 'Z')	// 주식선물 제도개선 B0 ~ ZZ 까지
					&& ((strTemp[1] >= '0' && strTemp[1] <= '9') || (strTemp[1] >= 'A' && strTemp[1] <= 'Z')) ) 
				|| ((strTemp[0] >= '0' && strTemp[0] <= '9') && (strTemp[1] >= 'A' && strTemp[1] <= 'Z'))) // 0028557: [7111] 주식선물 신규 상장종목에 대한 종목 선택 불가 현상
				rRet = sfutureType;
			else if(atoi(strTemp) >= 60 && atoi(strTemp) <= 99)
				rRet = cfutureType;
			else if (atoi(strTemp) == 1)
				rRet = futureType;
		}
		else if(strItemCode.Find("COMP")>= 0 || strItemCode.Find("SMBS")>= 0)
		{
			rRet = usType;
		}
	}

	switch(rRet)
	{
		case kospiType:
		case kosdaqType:
		case thirdType:
		case etfType:
		case elwType:
		case foreignType:	//20100930_JS.Kim 외국주 (화풍집단KDR(950010), 네프로아이티(950030))
		case etnType:
			{
				//return CMasterTypeInfo::STOCK_CHART;
				rMarketType = CMasterTypeInfo::STOCK_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}
		case indexType:		// 업종
			{
				//return CMasterTypeInfo::UPJONG_CHART;
				rMarketType = CMasterTypeInfo::UPJONG_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}
		case futureType:	// 선물
		case callType:		// 콜옵션
		case putType:		// 풋옵션
		case spreadType:	// 스프레드
			{
				//return CMasterTypeInfo::FUTOPT_CHART;
				rMarketType = CMasterTypeInfo::FUTOPT_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}
		case koptionType:	// 주식옵션
		case sfutureType:	// 주식선물
			{
				//return CMasterTypeInfo::STOCKFUTOPT_CHART;
				rMarketType = CMasterTypeInfo::STOCKFUTOPT_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}
		//case foreignType:	// 외국주
		case usType:
			{
				//return CMasterTypeInfo::FOREIGN_CHART;
				rMarketType = CMasterTypeInfo::FOREIGN_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}

		//<<20100511_JS.Kim 해외선물
		case gfutureType:
			{
				rMarketType = CMasterTypeInfo::FO_FOREIGN_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}
		//>>

		//<<20100316_JS.Kim FX
// 박성경 >>
// 		case gfxType:
// 			{
// 				rMarketType = CMasterTypeInfo::FX_CHART;
// 				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
// 				return rMarketType;
// 			}
		//>>
// 박성경 <<
		case cfutureType:
			{
				rMarketType = CMasterTypeInfo::COMMODITY_CHART;
				_DoAddChkCodeSB(&m_RecentMarketTypeList, RecentMarketTypeList_Max, strCode, rMarketType);
				return rMarketType;
			}

		default:
			if(strlen(strCode)==8)
			{
				if(IS_CALL_CODE_1(strCode[0]) || IS_PUT_CODE_1(strCode[0])) // 0024696: 파생상품 코드체계 변경 제도개선
				{
					rMarketType = CMasterTypeInfo::FUTOPT_CHART;
					return rMarketType;
				}
			}
			return CMasterTypeInfo::NONE_CHART;
			break;
	}

	return CMasterTypeInfo::STOCK_CHART;
}

// CString CMasterDataManager::GetJongmokNameFromCode(CString strCode)
// {
// 	return "";
// }

BOOL CMasterDataManager::GetRecentCodeNameArray(int nMarketType, CStringArray &arrCode, CStringArray &arrName)
{
	HWND hWinixWnd = GetHWND();
	if(!hWinixWnd) return FALSE;

// 	LPCSTR aSymbol = "1301";
// 	switch(nMarketType)
// 	{
// 		case CMasterTypeInfo::STOCK_CHART: 
// 			break;
// 		case CMasterTypeInfo::UPJONG_CHART:			//UP
// 			aSymbol = "20301"; break;
// 		case CMasterTypeInfo::FUTOPT_CHART:			//선물/옵션
// 			//aSymbol = "focod"; break;
// 			aSymbol = "30301"; break;
// 		case CMasterTypeInfo::STOCKFUTOPT_CHART:	//주식선물/주식옵션
// 			aSymbol = "jfcod"; break;
// 		case CMasterTypeInfo::FOREIGN_CHART:		//해외
// 			aSymbol = "50301"; break;
// 		case CMasterTypeInfo::FUOPT_JIPYO_CHART:	//FO지수는 임의값임.
// 			aSymbol = "60301"; break;
// 		default: return FALSE;
// 	}

	//DrCommon\PartnerMng\_PartnerUtilManager.cpp
	//CPartnerUtilManager::GetSymbolCode(..)루틴 가져옴.
	CString szSymbol;
	switch(nMarketType)
	{
		case CMasterTypeInfo::STOCK_CHART:
			szSymbol = "1301";
			break;
		case CMasterTypeInfo::UPJONG_CHART:
			szSymbol = "20301";
			break;
		case CMasterTypeInfo::COMMODITY_CHART:
			//>>JS.Kim_20101025  상품선물 히스토리 조회 안되어 종목코드 저장 후 다시 로드 안되는 버그 수정
			//szSymbol = "cfcod ";
			szSymbol = "cfcod";
			//<<
			break;
		case CMasterTypeInfo::FUTOPT_CHART:
			szSymbol = "ed_focod";
			break;
		case CMasterTypeInfo::STOCKFUTOPT_CHART:
//			szSymbol = "sfcod";
			szSymbol = "50301";
			break;
		case CMasterTypeInfo::FOREIGN_CHART:
			szSymbol = "50301";
			break;
		case CMasterTypeInfo::FUOPT_JIPYO_CHART:	//091128임의로 셋팅한 값임.
			szSymbol = "109301";
			break;
		case CMasterTypeInfo::FO_FOREIGN_CHART:	//091128임의로 셋팅한 값임.
			szSymbol = "70301";
			break;
		default:
			return FALSE;
	}

	CString strRet;
	if (nMarketType == CMasterTypeInfo::STOCKFUTOPT_CHART)
	{
		CString strPath;
		strPath.Format(_T("%s\\sfcode_history.dat"), theApp.m_MainInfoMng.GetEncUserDir());
		CStdioFile file;
		if (file.Open(strPath, CFile::modeRead|CFile::typeText))
		{
			file.ReadString(strRet);
			file.Close();
		}
	}
	else
	{
		strRet = theApp._WinixVariant(hWinixWnd, historyCC, szSymbol);
	}

	_DoParsingCode2(strRet, arrCode, arrName);

	return TRUE;

//@Solomon090819-->
// 	LPIM_DATA *ppHistory= NULL;
// 	char eType;
// 	int nHisCnt = 0;
// 	__int64 nType64 = 0;
// 	CString str;
// 	nType64 |= __int64(1) << KSTOCK_KOSPI;
// 	nType64 |= __int64(1) << KSTOCK_KOSDAQ;
// 	nType64 |= __int64(1) << KSTOCK_SECTOR;
// 	nType64 |= __int64(1) << KSTOCK_ELW;
// 	nType64 |= __int64(1) << KFREEBOARD;
// 	nType64 |= __int64(1) << KKOSPI_UPJONG;
// 	nType64 |= __int64(1) << KKOSDAQ_UPJONG;
// 	nType64 |= __int64(1) << KSECTOR_UPJONG;
// 	nType64 |= __int64(1) << KFUTURES_MASTER;
// 	nType64 |= __int64(1) << KOPTIONS_MASTER;
// 	nType64 |= __int64(1) << KFUTURE_SBASE;
// 	nType64 |= __int64(1) << KOPTION_SBASE;
// 	nType64 |= __int64(1) << US_JISU;
// 	
// 	// MT_STOCK
// 	if(nMarketType == 0)
// 	{
// 		ppHistory = m_itemMaster.GetCodeNameHistory(nType64, &nHisCnt);
// 		if(nHisCnt <= 0 || ppHistory == NULL)	return FALSE;
// 		
// 		IMD_KOSPI_STOCK* pKospiStock = NULL;
// 		IMD_KOSDAQ_STOCK* pKosdaqStock = NULL;
// 		IMD_ELW* pELW = NULL;
// 		IMD_FREEBOARD* pFreeboard = NULL;
// 		for(int nIndx = 0; nIndx < nHisCnt; ++nIndx)
// 		{
// 			eType = ppHistory[nIndx]->eType;
// 			if(eType == IMT_KOSPI_STOCK)
// 			{
// 				if(pKospiStock = (IMD_KOSPI_STOCK*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pKospiStock->sCode), sizeof(pKospiStock->sCode), pKospiStock->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pKospiStock->sName), sizeof(pKospiStock->sName), pKospiStock->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_KOSDAQ_STOCK)
// 			{
// 				if(pKosdaqStock = (IMD_KOSDAQ_STOCK*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pKosdaqStock->sCode), sizeof(pKosdaqStock->sCode), pKosdaqStock->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pKosdaqStock->sName), sizeof(pKosdaqStock->sName), pKosdaqStock->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_ELW)
// 			{
// 				if(pELW = (IMD_ELW*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pELW->sCode), sizeof(pELW->sCode), pELW->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pELW->sName), sizeof(pELW->sName), pELW->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_KFREEBOARD)
// 			{
// 				if(pFreeboard = (IMD_FREEBOARD*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pFreeboard->sCode), sizeof(pFreeboard->sCode), pFreeboard->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pFreeboard->sName), sizeof(pFreeboard->sName), pFreeboard->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 		}
// 	}
// 
// 	
// 	// MT_UPJONG
// 	if(nMarketType == 1)
// 	{
// 		ppHistory = m_itemMaster.GetCodeNameHistory(nType64, &nHisCnt);
// 		if(nHisCnt <= 0 || ppHistory == NULL)	return FALSE;
// 		
// 		char eType;
// 		IMD_KOSPI_UPJONG* pKospiUpjong = NULL;
// 		IMD_KOSDAQ_UPJONG* pKosdaqUpjong = NULL;
// 		IMD_SECTOR* pSector = NULL;
// 		for(int nIndx = 0; nIndx < nHisCnt; ++nIndx)
// 		{
// 			eType = ppHistory[nIndx]->eType;
// 			if(eType == IMT_KOSPI_UPJONG)
// 			{
// 				if(pKospiUpjong = (IMD_KOSPI_UPJONG*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pKospiUpjong->sCode), sizeof(pKospiUpjong->sCode), pKospiUpjong->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pKospiUpjong->sName), sizeof(pKospiUpjong->sName), pKospiUpjong->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_KOSDAQ_UPJONG)
// 			{
// 				if(pKosdaqUpjong = (IMD_KOSDAQ_UPJONG*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pKosdaqUpjong->sCode), sizeof(pKosdaqUpjong->sCode), pKosdaqUpjong->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pKosdaqUpjong->sName), sizeof(pKosdaqUpjong->sName), pKosdaqUpjong->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_SECTOR)
// 			{
// 				if(pSector = (IMD_SECTOR*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pSector->sCode), sizeof(pSector->sCode), pSector->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pSector->sName), sizeof(pSector->sName), pSector->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 		}
// 	}		
// 
// 
// 	// MT_FUTOPT
// 	if(nMarketType == 2)
// 	{
// 		ppHistory = m_itemMaster.GetCodeNameHistory(nType64, &nHisCnt);
// 		if(nHisCnt <= 0 || ppHistory == NULL)	return FALSE;
// 		
// 		char eType;
// 		IMD_FUTURE* pFuture = NULL;
// 		IMD_OPTION* pOption = NULL;
// 		for(int nIndx = 0; nIndx < nHisCnt; ++nIndx)
// 		{
// 			eType = ppHistory[nIndx]->eType;
// 			if(eType == IMT_KFUTURE)
// 			{
// 				if(pFuture = (IMD_FUTURE*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pFuture->sCode), sizeof(pFuture->sCode), pFuture->sCode); str.TrimRight();
// 					if(str.GetAt(0) == 'K')
// 						str = str.Mid(1);
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pFuture->sName), sizeof(pFuture->sName), pFuture->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_KOPTION)
// 			{
// 				if(pOption = (IMD_OPTION*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pOption->sCode), sizeof(pOption->sCode), pOption->sCode); str.TrimRight();
// 					if(str.GetAt(0) == 'K')
// 						str = str.Mid(1);
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pOption->sName), sizeof(pOption->sName), pOption->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 		}
// 	}
// 
// 
// 	// MT_STOCKFUTOPT
// 	if(nMarketType == 3)	
// 	{
// 		ppHistory = m_itemMaster.GetCodeNameHistory(nType64, &nHisCnt);
// 		if(nHisCnt <= 0 || ppHistory == NULL)	return FALSE;
// 		
// 		char eType;
// 		IMD_KOSPI_SFUTURE* pKospiSFuture = NULL;
// 		IMD_KOSPI_SOPTION* pKospiSOption = NULL;
// 		for(int nIndx = 0; nIndx < nHisCnt; ++nIndx)
// 		{
// 			eType = ppHistory[nIndx]->eType;
// 			if(eType == IMT_FUTURE_SBASE)
// 			{
// 				if(pKospiSFuture = (IMD_KOSPI_SFUTURE*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pKospiSFuture->sCode), sizeof(pKospiSFuture->sCode), pKospiSFuture->sCode); str.TrimRight();
// 					if(str.GetAt(0) == 'K')
// 						str = str.Mid(1);
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pKospiSFuture->sName), sizeof(pKospiSFuture->sName), pKospiSFuture->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 			else if(eType == IMT_OPTION_SBASE)
// 			{
// 				if(pKospiSOption = (IMD_KOSPI_SOPTION*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pKospiSOption->sCode), sizeof(pKospiSOption->sCode), pKospiSOption->sCode); str.TrimRight();
// 					if(str.GetAt(0) == 'K')
// 						str = str.Mid(1);
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pKospiSOption->sName), sizeof(pKospiSOption->sName), pKospiSOption->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 		}
// 	}
// 
// 	
// 	// MT_FOREIGN
// 	if(nMarketType == 4)
// 	{
// 		ppHistory = m_itemMaster.GetCodeNameHistory(nType64, &nHisCnt);
// 		if(nHisCnt <= 0 || ppHistory == NULL)	return FALSE;
// 		
// 		char eType;
// 		IMD_US_JISU* pUSJisu = NULL;
// 		for(int nIndx = 0; nIndx < nHisCnt; ++nIndx)
// 		{
// 			eType = ppHistory[nIndx]->eType;
// 			if(eType == IMT_US_JISU)
// 			{
// 				if(pUSJisu = (IMD_US_JISU*)(ppHistory[nIndx]->pData))
// 				{
// 					str.Format("%*.*s", sizeof(pUSJisu->sCode), sizeof(pUSJisu->sCode), pUSJisu->sCode); str.TrimRight();
// 					arrCode.Add(str);
// 					str.Format("%*.*s", sizeof(pUSJisu->sName), sizeof(pUSJisu->sName), pUSJisu->sName); str.TrimRight();
// 					arrName.Add(str);
// 				}
// 			}
// 		}
// 	}
// 	
//@Solomon090819<--
	return TRUE;
}

void CMasterDataManager::_DoParsingCode(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName, bool bInsert, int nStartIdx)
{
	if(szCodeList.GetLength()==0) return;

// 	szCodeList += "\n";
// 	int nPos1 = 0, nPos2, nOffset=0;
// 	CString szTmp, szCode, szCodeName;
// 
// 	int nDataLen = szCodeList.GetLength();
// 	if(nStartIdx==0)
// 	{
// 		while(nPos1>=0 && nOffset<=nDataLen)
// 		{
// 			nPos1 = szCodeList.Find('\n', nOffset);
// 			if(nPos1<0) break;
// 			
// 			szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
// 			nOffset = (nPos1+1);
// 			
// 			nPos2 = szTmp.Find('\t');
// 			if(nPos2<0) break;
// 			szCode		= szTmp.Left(nPos2);
// 			if(szCode.Left(1) == "A") szCode = szCode.Mid(1);
// 			szCodeName	= szTmp.Mid(nPos2+1);
// 			
// 			szCodeName.TrimLeft();
// 			if(bInsert)
// 			{
// 				arrCode.InsertAt(0, szCode);
// 				arrName.InsertAt(0, szCodeName);
// 			}
// 			else
// 			{
// 				arrCode.Add(szCode);
// 				arrName.Add(szCodeName);
// 			}
// 		}
// 	}
// 	else if(nStartIdx==1)
// 	{
// 		while(nPos1>=0 && nOffset<=nDataLen)
// 		{
// 			nPos1 = szCodeList.Find('\n', nOffset);
// 			if(nPos1<0) break;
// 			
// 			szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
// 			nOffset = (nPos1+1);
// 			
// 			nPos2 = szTmp.Find('\t');
// 			if(nPos2<0) break;
// 			szCode		= szTmp.Left(nPos2);
// 			if(szCode.Left(1) == "A") szCode = szCode.Mid(1);
// 			szCodeName	= szTmp.Mid(nPos2+1);
// //			szCodeName.TrimLeft();
// 			if(szCodeName[0]=='#' ||szCodeName[0]=='&')
// 				szCodeName = szCodeName.Mid(1);
// 
// 			if(bInsert)
// 			{
// 				arrCode.InsertAt(0, szCode);
// 				arrName.InsertAt(0, szCodeName);
// 			}
// 			else
// 			{
// 				arrCode.Add(szCode);
// 				arrName.Add(szCodeName);
// 			}
// 		}
// 	}

	CString strData, strTemp, strCode, strName;
	int nPos;
	strData = szCodeList;
	
	while(TRUE)
	{
		nPos = strData.Find("\t");
		if(nPos == -1)
			break;
		
		strTemp = strData.Mid(0, nPos + 1);
		strTemp.Remove('\t');
		strData = strData.Mid(nPos + 1, strData.GetLength());
		strCode.Format("%s", strTemp);

		nPos = strData.Find("\t");

		strTemp = strData.Mid(0, nPos + 1);
		strTemp.Remove('\t');
		strData = strData.Mid(nPos + 1, strData.GetLength());
		strName.Format("%s", strTemp);

		if (strCode.GetLength() <= 0 || strName.GetLength() <= 0)
			continue;

		arrCode.Add(strCode);
		arrName.Add(strName);
	}
}

void CMasterDataManager::_DoParsingCodePMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;
	
// 	szCodeList += "\n";
// 	int nPos1 = 0, nPos2, nOffset=0;
// 	CString szTmp, szCode, szCodeName;
// 	const int nRequest = 3;
// 	CString szTmpA[nRequest], szMonth;
// 	char cCallPut;
// 
// 	const int nMaxOption = 4;
// 	int	nOptIndex=-1;
// 	CStringArray tmpArrCode[nMaxOption], tmpArrName[nMaxOption];
// 
// 	int nDataLen = szCodeList.GetLength();
// 	while(nPos1>=0 && nOffset<=nDataLen)
// 	{
// 		nPos1 = szCodeList.Find('\n', nOffset);
// 		if(nPos1<0) break;
// 		
// 		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
// 		nOffset = (nPos1+1);
// 		
// 		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
// 		szCode	= szTmp.Left(nPos2);
// 		szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
// 		
// 		for(int i=0; i<nRequest-1; i++)
// 		{
// 			nPos2 = szTmp.Find(' ');	if(nPos2<0) continue;
// 			szTmpA[i] = szTmp.Left(nPos2);
// 			szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
// 		}
// 		
// 		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
// 		szTmpA[nRequest-1] = szTmp.Left(nPos2);	szTmpA[nRequest-1].TrimRight();
// 		
// 		szTmpA[1] = szTmp.Mid(nPos2+1);
// 		
// 		cCallPut = szTmpA[0].GetAt(0);
// 		if(szTmpA[0]=="콜") cCallPut = 'C';
// 		else if(szTmpA[0]=="풋") cCallPut = 'P';
// 
// 		if(szMonth.Compare(szTmpA[1]) !=0 )
// 		{
// 			szMonth = szTmpA[1];
// 			nOptIndex ++;
// 			if(nOptIndex>=nMaxOption) nOptIndex=0;
// 		}
// 
// 		szCodeName.Format("%c %-6.6s %-10.10s", cCallPut, szTmpA[1], szTmpA[2]);
// 
// 		//arrCode.InsertAt(0, szCode);
// 		//arrName.InsertAt(0, szCodeName);
// 		tmpArrCode[nOptIndex].InsertAt(0, szCode);
// 		tmpArrName[nOptIndex].InsertAt(0, szCodeName);
// 	}
// 
// 	//for(int k=nMaxOption-1; k>=0; k--)
// 	for(int k=0; k<nMaxOption; k++)
// 	{
// 		int nCnt = tmpArrName[k].GetSize();
// 		for(int j=0; j<nCnt; j++)
// 		{
// 			arrCode.Add(tmpArrCode[k].GetAt(j));
// 			arrName.Add(tmpArrName[k].GetAt(j));
// 		}
// 	}

	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	const int nRequest = 3;
	CString szTmpA[nRequest], szMonth;
	char cCallPut;
	
	const int nMaxOption = 11;
	int	nOptIndex=-1;
	CStringArray tmpArrCode[nMaxOption], tmpArrName[nMaxOption];
	
	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\n', nOffset);
		if(nPos1<0) break;
		
		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szCode	= szTmp.Left(nPos2);
		szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		
		for(int i=0; i<nRequest-1; i++)
		{
			nPos2 = szTmp.Find(' ');	if(nPos2<0) continue;
			szTmpA[i] = szTmp.Left(nPos2);
			szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		}
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szTmpA[nRequest-1] = szTmp.Left(nPos2);	szTmpA[nRequest-1].TrimRight();
		
		szCodeList = szTmp.Mid(nPos2+1);
		szCodeList += "\n";
		nOffset = 0;
		
		cCallPut = szTmpA[0].GetAt(0);
		if(szTmpA[0]=="콜") cCallPut = 'C';
		else if(szTmpA[0]=="풋") cCallPut = 'P';
		
		if(szMonth.Compare(szTmpA[1]) !=0 )
		{
			szMonth = szTmpA[1];
			nOptIndex ++;
			if(nOptIndex>=nMaxOption) nOptIndex=0;
		}
		
		szCodeName.Format("%c %-6.6s %-10.10s", cCallPut, szTmpA[1], szTmpA[2]);
		szCodeName.TrimRight();
		
		//arrCode.InsertAt(0, szCode);
		//arrName.InsertAt(0, szCodeName);
		tmpArrCode[nOptIndex].InsertAt(0, szCode);
		tmpArrName[nOptIndex].InsertAt(0, szCodeName);
	}
	
	for(int k=0; k<nMaxOption; k++)
	{
		int nCnt = tmpArrName[k].GetSize();
		for(int j=0; j<nCnt; j++)
		{
			arrCode.Add(tmpArrCode[k].GetAt(j));
			arrName.Add(tmpArrName[k].GetAt(j));
		}
	}
}

//@Solomon:주식옵션용.
void CMasterDataManager::_DoParsingCodeJPMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;
	
	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	const int nRequest = 4;
	CString szTmpA[nRequest], szMonth;
	char cCallPut;

	const int nMaxOption = 4;
	int	nOptIndex=-1;
	CStringArray tmpArrCode[nMaxOption], tmpArrName[nMaxOption];

	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\n', nOffset);
		if(nPos1<0) break;
		
		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szCode	= szTmp.Left(nPos2);
		szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();

		for(int i=0; i<nRequest-1; i++)
		{
			nPos2 = szTmp.Find(' ');	if(nPos2<0) continue;
			szTmpA[i] = szTmp.Left(nPos2);
			szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		}

		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szTmpA[nRequest-1] = szTmp.Left(nPos2);

		szTmpA[2] = szTmp.Mid(nPos2+1);

		cCallPut = szTmpA[1].GetAt(0);
		if(szTmpA[1]=="콜") cCallPut = 'C';
		else if(szTmpA[1]=="풋") cCallPut = 'P';

		if(szMonth.Compare(szTmpA[2]) !=0 )
		{
			szMonth = szTmpA[2];
			nOptIndex ++;
		}

		szCodeName.Format("%11s%c 20%-4.4s %-10.10s", " ", cCallPut, szTmpA[2], szTmpA[3]);

		//arrCode.InsertAt(0, szCode);
		//arrName.InsertAt(0, szCodeName);
		tmpArrCode[nOptIndex].InsertAt(0, szCode);
		tmpArrName[nOptIndex].InsertAt(0, szCodeName);
	}

	//for(int k=nMaxOption-1; k>=0; k--)
	for(int k=0; k<nMaxOption; k++)
	{
		int nCnt = tmpArrName[k].GetSize();
		for(int j=0; j<nCnt; j++)
		{
			arrCode.Add(tmpArrCode[k].GetAt(j));
			arrName.Add(tmpArrName[k].GetAt(j));
		}
	}
}


//@Solomon:달러옵션용.
void CMasterDataManager::_DoParsingCodeDPMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;
	
	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	const int nRequest = 4;
	CString szTmpA[nRequest], szMonth;
	char cCallPut;
	
	const int nMaxOption = 4;
	int	nOptIndex=0, nIndex=0;
	CStringArray tmpArrCode[nMaxOption], tmpArrName[nMaxOption];
	CMap<CString, LPCTSTR, int, int> mapMonth;

	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\n', nOffset);
		if(nPos1<0) break;
		
		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szCode	= szTmp.Left(nPos2);
		szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		
		for(int i=0; i<nRequest-1; i++)
		{
			nPos2 = szTmp.Find(' ');	if(nPos2<0) continue;
			szTmpA[i] = szTmp.Left(nPos2);
			szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		}
		
		szTmpA[nRequest-1] = szTmp;

		if(szTmpA[1]=="콜") cCallPut = 'C';
		else if(szTmpA[1]=="풋") cCallPut = 'P';
		
		if (!mapMonth.Lookup(szTmpA[2], nIndex))
		{
			mapMonth.SetAt(szTmpA[2], nOptIndex);
			nIndex = nOptIndex;
			nOptIndex++;
		}

		szCodeName.Format("%11s%c %-6.6s %-10.10s", " ", cCallPut, szTmpA[2], szTmpA[3]);
		
		//arrCode.InsertAt(0, szCode);
		//arrName.InsertAt(0, szCodeName);
		if (nIndex >= nMaxOption)
		{
			break;
		}

		tmpArrCode[nIndex].InsertAt(0, szCode);
		tmpArrName[nIndex].InsertAt(0, szCodeName);
	}
	
	//for(int k=nMaxOption-1; k>=0; k--)
	for(int k=0; k<nMaxOption; k++)
	{
		int nCnt = tmpArrName[k].GetSize();
		for(int j=0; j<nCnt; j++)
		{
			arrCode.Add(tmpArrCode[k].GetAt(j));
			arrName.Add(tmpArrName[k].GetAt(j));
		}
	}
}

void CMasterDataManager::_DoParsingCodeForeign(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;
	
	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	
	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\n', nOffset);
		if(nPos1<0) break;
		
		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find('\t');
		if(nPos2<0) break;
		szCode		= szTmp.Left(nPos2);
		szCodeName	= szTmp.Mid(nPos2+1);
		
		arrCode.Add(szCode);
		szCodeName.TrimLeft();
		arrName.Add(szCodeName);
	}
}

void CMasterDataManager::_DoParsingCode2(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;
	
	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	
	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\t', nOffset);
		if(nPos1<0) break;

		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find(' ');
		if(nPos2<0) break;

		szCode		= szTmp.Left(nPos2);
		szCodeName	= szTmp.Mid(nPos2+1);

		arrCode.Add(szCode);
		szCodeName.TrimLeft();
		arrName.Add(szCodeName);
	}
}

void CMasterDataManager::_DoParsingCodeFX(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName, bool bInsert, int nStartIdx)
{
	if(szCodeList.GetLength()==0) return;

	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;

	int nDataLen = szCodeList.GetLength();
	if(nStartIdx==0)
	{
		while(nPos1>=0 && nOffset<=nDataLen)
		{
			nPos1 = szCodeList.Find('\n', nOffset);
			if(nPos1<0) break;
			
			szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
			nOffset = (nPos1+1);
			
			nPos2 = szTmp.Find('\t');
			if(nPos2<0) break;
			szCode		= szTmp.Left(nPos2);
			szCodeName	= szTmp.Mid(nPos2+1);
			
			szCodeName.TrimLeft();
			if(bInsert)
			{
				arrCode.InsertAt(0, szCode);
				arrName.InsertAt(0, szCodeName);
			}
			else
			{
				arrCode.Add(szCode);
				arrName.Add(szCodeName);
			}
		}
	}
	else if(nStartIdx==1)
	{
		while(nPos1>=0 && nOffset<=nDataLen)
		{
			nPos1 = szCodeList.Find('\n', nOffset);
			if(nPos1<0) break;
			
			szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
			nOffset = (nPos1+1);
			
			nPos2 = szTmp.Find('\t');
			if(nPos2<0) break;
			szCode		= szTmp.Left(nPos2);
			szCodeName	= szTmp.Mid(nPos2+1);
			if(szCodeName[0]=='#' ||szCodeName[0]=='&')
				szCodeName = szCodeName.Mid(1);

			if(bInsert)
			{
				arrCode.InsertAt(0, szCode);
				arrName.InsertAt(0, szCodeName);
			}
			else
			{
				arrCode.Add(szCode);
				arrName.Add(szCodeName);
			}
		}
	}
}

BOOL CMasterDataManager::GetAllStock(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;

	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_ALL;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	_DoParsingCode(strRet, arrCode, arrName);

	return TRUE;
}

BOOL CMasterDataManager::GetStockCodeWithType(int nType, CStringArray &arrCode, CStringArray &arrName)
{
	if(nType=='0')
	{
		GetMasterDataInUpDetails("UMaster", arrCode, arrName, "1001'");
	}
	else if(nType=='1')
	{
		GetMasterDataInUpDetails("kumASTER", arrCode, arrName, "2001'");
	}

	//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(nType, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 	
// 	CString str;
// 	IMD_KOSPI_STOCK* pKospiStock = NULL;
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		if(pKospiStock = (IMD_KOSPI_STOCK*)(ppData[nIndx]->pData))
// 		{
// 			str = ppData[nIndx]->szCode; str.TrimRight();
// 			arrCode.Add(str);
// 			str = ppData[nIndx]->szName; str.TrimRight();
// 			arrName.Add(str);
// 		}
// 	}
	//@Solomon090819<--

	return TRUE;
}

/*
BOOL CMasterDataManager::GetFOFLMaster(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;

	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_FOINDEX_FO_MONTH;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	_DoParsingCode(strRet, arrCode, arrName);

	return TRUE;
}
*/

BOOL CMasterDataManager::GetJFMaster(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_SFUTURE;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, arrCode, arrName);
	
//@Solomon090819
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_FUTURE_SBASE, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	LPIMD_KOSPI_SFUTURE pSFut = NULL;
// 	CMapStringToString mapName;
// 	CString str;
// 	CString strComp;
// 
// 	char* szName = new char[sizeof(pSFut->sunas_name) + 1];
// 
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		if(pSFut = (LPIMD_KOSPI_SFUTURE)(ppData[nIndx]->pData))
// 		{
// 			::memset(szName, 0x00, sizeof(pSFut->sunas_name) + 1);
// 			::memcpy(szName, pSFut->sunas_name, sizeof(pSFut->sunas_name));
// 
// 			str = szName;
// 			str.TrimRight();
// 			if(mapName.Lookup(str, strComp) == FALSE)
// 			{
// 				mapName.SetAt(str, str);
// 				arrCode.Add(str);
// 				arrName.Add(str);
// 			}
// 		}
// 	}
// 
// 	delete [] szName;
// 	mapName.RemoveAll();
	//@Solomon090819<--
	return TRUE;
}

BOOL CMasterDataManager::GetJPMaster(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_SOPTION;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, arrCode, arrName, true);

	return TRUE;
}

BOOL CMasterDataManager::GetWPMaster(CStringArray& arrCode, CStringArray& arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if (!hWinixWnd) return FALSE;

	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_WEEKLY_OPTION;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);

	_DoParsingCode(strRet, arrCode, arrName, true);

	return TRUE;
}

// 	BOOL CMasterDataManager::GetFreeBoardMst(CStringArray &arrCode, CStringArray &arrName)
// 	{
// 		arrCode.RemoveAll();
// 		arrName.RemoveAll();
// 
// 	//@Solomon090819-->
// 	// 	int nCount = 0;
// 	// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_FREEBOARD_UPJONG, &nCount);
// 	// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 	// 
// 	// 	CString str;
// 	// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	// 	{
// 	// 		str = ppData[nIndx]->szCode; str.TrimRight();
// 	// 		arrCode.Add(str);
// 	// 		str = ppData[nIndx]->szName; str.TrimRight();
// 	// 		arrName.Add(str);
// 	// 	}
// 	//@Solomon090819<--
// 
// 		return TRUE;
// 	}

BOOL CMasterDataManager::GetELWIndexMaster(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_ELW_BASE;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, arrCode, arrName);

//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_ELW, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	IMD_ELW* pElw = NULL;
// 	CMapStringToString mapName;
// 	CString str;
// 	CString strComp;
// 
// 	char* szName = new char[sizeof(pElw->sBaseCode1) + 1];
// 
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		if(pElw = (IMD_ELW*)(ppData[nIndx]->pData))
// 		{
// 			::memset(szName, 0x00, sizeof(pElw->sBaseCode1) + 1);
// 			::memcpy(szName, pElw->sBaseCode1, sizeof(pElw->sBaseCode1));
// 
// 			str = m_itemMaster.GetItemName(IMT_STOCK, szName);
// 			if(str.IsEmpty())
// 				str = szName;
// 			str.TrimRight();
// 			if(mapName.Lookup(str, strComp) == FALSE)
// 			{
// 				mapName.SetAt(str, str);
// 				arrCode.Add(str);
// 				arrName.Add(str);
// 			}
// 		}
// 	}
// 
// 	delete [] szName;
// 	mapName.RemoveAll();
//@Solomon090819<--
	return TRUE;
}

BOOL CMasterDataManager::GetELWIndexCode(CString strELWCode, CString& strIndexCode, CString& strIndexName)
{
	strIndexCode.Empty();
	strIndexName.Empty();

//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_ELW, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	CString str;
// 	IMD_ELW* pElw = NULL;
// 
// 	char* szCode = new char[sizeof(pElw->sCode) + 1];
// 
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		if(pElw = (IMD_ELW*)(ppData[nIndx]->pData))
// 		{
// 			::memset(szCode, 0x00, sizeof(pElw->sCode) + 1);
// 			::memcpy(szCode, pElw->sCode, sizeof(pElw->sCode));
// 
// 			str = szCode;
// 			str.TrimRight();
// 
// 			if(str == strELWCode)
// 			{
// 				char* szIndexCode = new char[sizeof(pElw->sBaseCode1) + 1];
// 				::memset(szIndexCode, 0x00, sizeof(pElw->sBaseCode1) + 1);
// 				::memcpy(szIndexCode, pElw->sBaseCode1, sizeof(pElw->sBaseCode1));
// 
// 				strIndexCode = szIndexCode;
// 				strIndexCode.TrimRight();
// 				delete [] szIndexCode;
// 
// 				if(strIndexCode == "KSP200")
// 				{
// 					strIndexCode = "101";
// 					strIndexName = m_itemMaster.GetItemName(IMT_KOSPI200_UPJONG, strIndexCode);
// 				}
// 				else
// 					strIndexName = m_itemMaster.GetItemName(IMT_STOCK, strIndexCode);
// 
// 				break;
// 			}
// 		}
// 	}
// 	delete [] szCode;

//@Solomon090819<--
	return TRUE;
}

BOOL CMasterDataManager::GetELWIssueMaster(CStringArray &arrCode, CStringArray &arrName)
{
	arrCode.RemoveAll();
	arrName.RemoveAll();

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_ELW_PB;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, arrCode, arrName);

//@Solomon090819-->
// 	int nCount = 0;
// 	LPIM_DATA* ppData = m_itemMaster.GetItemListData(IMT_ELW, &nCount);
// 	if(nCount <= 0 || ppData == NULL)	return FALSE;
// 
// 	IMD_ELW* pElw = NULL;
// 	CMapStringToString mapName;
// 	CString str;
// 	CString strComp;
// 	char* szName = new char[sizeof(pElw->sPublishName) + 1];
// 	for(int nIndx = 0; nIndx < nCount; ++nIndx)
// 	{
// 		if(pElw = (IMD_ELW*)(ppData[nIndx]->pData))
// 		{
// 			::memset(szName, 0x00, sizeof(pElw->sPublishName) + 1);
// 			::memcpy(szName, pElw->sPublishName, sizeof(pElw->sPublishName));
// 
// 			str = szName;
// 			str.TrimRight();
// 			if(mapName.Lookup(str, strComp) == FALSE)
// 			{
// 				mapName.SetAt(str, str);
// 				arrCode.Add(str);
// 				arrName.Add(str);
// 			}
// 		}
// 	}
// 	delete [] szName;
// 	mapName.RemoveAll();
//@Solomon090819<--
	return TRUE;
}

CString CMasterDataManager::GetCodeName(LPCTSTR lpszItemCode)
{
//	if(strcmp(lpszItemCode, DEF_CONTINUE_FUT_CODE)==0) 
//		return SZDEF_CONTINUE_FUT_CODE;

	if(strlen(lpszItemCode)==8 && strncmp(&lpszItemCode[3], "99999", 5)==0)
	{
		if (strcmp(lpszItemCode, DEF_CONTINUE_FUT_CODE)==0)
			return SZDEF_CONTINUE_FUT_CODE;
		else if( strcmp(lpszItemCode, DEF_CONTINUE_MINI_FUT_CODE) == 0 )
			return SZDEF_CONTINUE_MINI_FUT_CODE;

		CString strTemp;
		int i, nCount = sizeof(g_pszCFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strcmp(lpszItemCode, g_pszCFTiesCode[i]) == 0)
			{
				strTemp.Format(_T("%s 연계선물"), g_pszCFTiesName[i]);
				return strTemp;
			}
		}

		nCount = sizeof(g_pszSFTiesCode) / sizeof(char*);
		for (i=0; i<nCount; i++)
		{
			if (strcmp(lpszItemCode, g_pszSFTiesCode[i]) == 0)
			{
				strTemp.Format(_T("%s 연계선물"), g_pszSFTiesName[i]);
				return strTemp;
			}
		}
	}

	CString szItemCode = lpszItemCode; //winix버그인지 데이터가 변경되기때문에 이렇게 함.
	// 이상신 (20151123) : (ISSUE NO - 0013117): [7125] 주기버튼 클릭하여 주기 변경하다보면 조회되지않고 HTS 비정상종료 되는 현상
	//			winix 버그가 아니고, 포인터를 넘겨주면.. 함수내에서 또는 넘겨준 곳에서 값을 사용함에 있어 변경이 될수 있음.
	//			하여, (ISSUE NO - 0013117)는 _DoAddChkCodeSS() 함수 내에서 저장하는 값이 포인터로 저장되어 발생하는 버그.
	szItemCode.TrimLeft();szItemCode.TrimRight();

	//20221210 이문수 - 종목코드가 빈문자열인 경우 hWinixWnd핸들에 WM_USER메세지를 SendMessage로 전달시 메모리 깨지면서 CStdDialog::m_strRecvCode에 쓰레기값 들어가서 조회응답처리에 문제발생 >>
	if (szItemCode.IsEmpty())
		return "";
	//20221210 이문수 - 종목코드가 빈문자열인 경우 hWinixWnd핸들에 WM_USER메세지를 SendMessage로 전달시 메모리 깨지면서 CStdDialog::m_strRecvCode에 쓰레기값 들어가서 조회응답처리에 문제발생 <<

	CString strRet;
	BOOL bFind = _DoPreChkCodeSS(&m_RecentCodeNameList, (LPTSTR)(LPCTSTR)szItemCode, strRet);
	if(bFind) return strRet;

	//FO지수 CodeName.
// 	if(strlen(lpszItemCode)==10)
// 	{
// 		CString rValue;
// 		gFOCodeMap.Lookup(lpszItemCode, rValue);
// 		return rValue;
// 	}

	HWND hWinixWnd = GetHWND();
	if(!hWinixWnd) return "";

	strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(nameDLL, codeNAME), (LPARAM)(LPCTSTR)szItemCode);
	if(strRet.IsEmpty())
	{
		HWND hMasterWnd = GetCodeMasterHWND();
		if(!hMasterWnd) return "";
		szItemCode = lpszItemCode;
		strRet = (char*)::SendMessage(hMasterWnd, WM_USER, MAKEWPARAM(codeNAME, NULL), (LPARAM)(LPCTSTR)szItemCode);
	}
	strRet.TrimLeft(); 
	strRet.TrimRight();

/*@solom	20100122	첫번째 문자 특수용도 사용하지 않음.
	BYTE rCodeType = GetCodeTypeFromCode(lpszItemCode);
	if( rCodeType==CDRCodeTypeInfo::CODE_KOSPI_STOCK ||
		rCodeType==CDRCodeTypeInfo::CODE_KOSDAQ_STOCK ||
		rCodeType==CDRCodeTypeInfo::CODE_ELW)
//	if(strRet[0]=='&')
		strRet = strRet.Mid(1); //첫번째는 space또는 &등의 문자가 오고 있으므로. 한 칸 뛰움.
*/
	strRet.Replace("&", "&&");

	if(strRet.GetLength())	_DoAddChkCodeSS(&m_RecentCodeNameList, RecentGetCodeName_Max, (LPTSTR)(LPCTSTR)szItemCode, strRet);

	return strRet;
}

CString CMasterDataManager::GetItemExpCode(LPCTSTR lpszCode)
{
//@Solomon090819	char cMarketType = m_itemMaster.GetItemType(lpszCode);
//@Solomon090819	return m_itemMaster.GetItemExpCode(cMarketType, lpszCode);
	return "";//@Solomon090819
}

BOOL CMasterDataManager::GetThemeGroup(CStringArray& arCode, CStringArray& arName)
{
	CString strCode;
	strCode.Format("%d", CDKIND_THEMELIST);
	GetMasterDataInUpDetails("TMMASTER", arCode, arName, strCode);
//@Solomon090819	int nResult = m_itemMaster.GetThemeGroup(arCode, arName);
//@Solomon090819	return nResult;
	return FALSE; //@Solomon090819
}

BOOL CMasterDataManager::GetThemeItemList(LPCTSTR szThemeCode, CStringArray &arrCode, CStringArray &arrName)
{
	GetMasterDataInUpDetails("TMMASTER", arrCode, arrName, szThemeCode);

//@Solomon090819	int nResult = m_itemMaster.GetThemeItemList2(szThemeCode, arrCode, arrName);	
//@Solomon090819	return nResult;
	return FALSE; //@Solomon090819
}

BOOL CMasterDataManager::_DoPreChkCodeSS(List_ST_STRINGSTRING* pListST, LPCSTR strCode, CString &szCodeName)
{
	if(!pListST) return FALSE;

	ST_STRINGSTRING* pItem;	
	for(POSITION pos=pListST->GetHeadPosition(); pos; )
	{
		pItem = pListST->GetNext(pos);
		if(pItem->m_szKey.Compare(strCode)==0)
		{
			szCodeName = pItem->m_szValue;
			return TRUE;
		}
	}
	return FALSE;
}

void CMasterDataManager::_DoAddChkCodeSS(List_ST_STRINGSTRING* pListST, int nMaxCnt, LPCSTR strCode, LPCSTR szCodeName)
{
	if(strCode==NULL || strlen(strCode)==0) return;

	POSITION pos = pListST->GetHeadPosition();
	if(pos)
	{
		POSITION hPos=pListST->GetHeadPosition(),
			tPos;
		
		ST_STRINGSTRING* pItem;
		for(POSITION pos=hPos; pos; )
		{
			tPos = pos;
			pItem = pListST->GetNext(pos);
			if(pItem->m_szKey.Compare(strCode)==0)
			{
				if(tPos==hPos) return;
				else
				{
					pListST->RemoveAt(tPos);
					pListST->AddHead(pItem);
					return;
				}
			}
		}
		
		if(pListST->GetCount()==nMaxCnt)
			pListST->RemoveAt(tPos);
	}
	
	ST_STRINGSTRING* pItem = new ST_STRINGSTRING;
	pItem->m_szKey = strCode;
	pItem->m_szValue = szCodeName;
	pListST->AddHead(pItem);
}

BOOL CMasterDataManager::GetSiteUpKode(LPCSTR szCode, CString& rCode)
{
	rCode.Empty();

	BOOL bFind = _DoPreChkCodeSS(&m_RecentChxUpCodeList, szCode, rCode);
	if(bFind) return bFind;

	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
//	szCode ="001";
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_INDEX_CODECHANGE;
	codeR.type = CDTYPE_CODE;
	memcpy(codeR.code, szCode, strlen(szCode));
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	if(rCode.IsEmpty())
	{
		rCode = strRet;
		_DoAddChkCodeSS(&m_RecentChxUpCodeList, RecentGetChxUpCode_Max, szCode, rCode);
		return TRUE;
	}

	return FALSE;
}

void CMasterDataManager::_DoClearList()
{
	for(POSITION pos=m_UpKeyList.GetHeadPosition(); pos; )
		delete m_UpKeyList.GetNext(pos);
	m_UpKeyList.RemoveAll();
}

int gFncomparisonFunctionString(const void *a, const void *b) 
{
	return( strcmp( (char *)a, (char *)b) );
}

void CMasterDataManager::_DoSortCode(CStringArray &arrCode, CStringArray &arrName)
{
	int nCount = arrCode.GetSize();
	int nOneSize = 100;
	char* pData = new char[nOneSize*nCount];
	ZeroMemory(pData, nOneSize*nCount);
	char aTmp[100+1]={0,};

	//char** pArray = (char**)pData;

	int nStrLen;
	int i = 0;
	for( i=0; i<nCount; i++)
	{
		sprintf(aTmp, "%s%s", arrName.GetAt(i), arrCode.GetAt(i));

		nStrLen = strlen(aTmp);
		memcpy(&pData[i*nOneSize], aTmp, nStrLen);
		pData[i*nOneSize+nStrLen] = 0;
	}

	//qsort((void *)pArray, nCount, sizeof(pArray[0]), gFncomparisonFunctionString);
	qsort((void *)pData, nCount, nOneSize, gFncomparisonFunctionString);

	arrCode.RemoveAll();
	arrName.RemoveAll();
	CString szCode, szName;
	char* pOneData;
	for(i=0; i<nCount; i++)
	{
		pOneData = &pData[i*nOneSize];
		//szName.Format("%*.*s", nOneSize, nOneSize, pOneData);
		szName = pOneData;
		szCode = szName.Right(3);
		szName = szName.Left(szName.GetLength()-3);

		arrCode.Add(szCode);
		arrName.Add(szName);
	}
}

BOOL CMasterDataManager::_DoGetOldFutureMaster(CStringArray &arrCode, CStringArray &arrName)
{
	char *arrayCode[] = { "101DC000"	,"101E3000"	,"101E6000"	,"101E9000"	,NULL};
	char *arrayName[] = { "F 0912"		,"F 1003"	,"F 1006"	,"F 1009"	,NULL};

	int i=0;
	while(arrayCode[i] != NULL)
	{
		arrCode.Add(arrayCode[i]);
		arrName.Add(arrayName[i]);
		i++;
	}
	
	return TRUE;
}

BOOL CMasterDataManager::GetETNMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();
	
	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_ETN;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);
	
	return TRUE;
}

BOOL CMasterDataManager::GetVFutureMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{	
	pastrUpCode->RemoveAll();
	pastrUpName->RemoveAll();
	
	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_VF;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrUpCode, *pastrUpName);
	return TRUE;
}

BOOL CMasterDataManager::GetSFutureMst(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption)
{
	return TRUE;
}

BOOL CMasterDataManager::GetMiniFutureJongMst(CStringArray *&pastrJongCode,	CStringArray *&pastrJongName, CStringArray *&pastrJongEngName)
{
	pastrJongCode->RemoveAll();
	pastrJongName->RemoveAll();
	
	HWND hWinixWnd = GetCodeMasterHWND();
	if(!hWinixWnd) return FALSE;
	
	struct _codeR codeR;
	ZeroMemory(codeR.code, sizeof(codeR.code));
	codeR.kind = CDKIND_MINI_FUTURE;
	codeR.type = CDTYPE_CODE | CDTYPE_NAME;
	CString strRet = (char*)::SendMessage(hWinixWnd, WM_USER, MAKEWPARAM(getCODE, NULL), (LPARAM)&codeR);
	
	_DoParsingCode(strRet, *pastrJongCode, *pastrJongName);

	if( pastrJongCode->GetSize() > 0 )
	{
		pastrJongCode->Add(DEF_CONTINUE_MINI_FUT_CODE);
		pastrJongName->Add(SZDEF_CONTINUE_MINI_FUT_CODE);
	}
	
	return TRUE;
}

BOOL CMasterDataManager::GetMiniOptionJongMst(CStringArray *&pastrJongCode,	CStringArray *&pastrJongName)
{
	return GetMasterDataInUpDetails("MINI_PMASTER", *pastrJongCode, *pastrJongName, "");
}

BOOL CMasterDataManager::GetWeeklyOptionJongMst(CStringArray *&pastrJongCode,	CStringArray *&pastrJongName)
{
	return GetMasterDataInUpDetails("WEEKLY_PMASTER", *pastrJongCode, *pastrJongName, "");
}

void CMasterDataManager::_DoParsingCodeMiniMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;

	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	const int nRequest = 3;
	CString szTmpA[nRequest], szMonth;
	char cCallPut;
	
	const int nMaxOption = 11;
	int	nOptIndex=-1;
	CStringArray tmpArrCode[nMaxOption], tmpArrName[nMaxOption];
	
	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\n', nOffset);
		if(nPos1<0) break;
		
		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szCode	= szTmp.Left(nPos2);
		szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		
		for(int i=0; i<nRequest-1; i++)
		{
			nPos2 = szTmp.Find(' ');	if(nPos2<0) continue;
			
			int nTemp = 2;
			if( strncmp( szTmp , "M ", nTemp ) == 0 )			// 행사가에 'M'(Mini)가 추가되어 'M'을 자르는 처리
				szTmp = szTmp.Mid(nTemp);
			szTmpA[i] = szTmp.Left(nPos2);
			szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		}
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szTmpA[nRequest-1] = szTmp.Left(nPos2);	szTmpA[nRequest-1].TrimRight();
		
		szCodeList = szTmp.Mid(nPos2+1);
		szCodeList += "\n";
		nOffset = 0;
		
		cCallPut = szTmpA[0].GetAt(0);		
		if(szMonth.Compare(szTmpA[1]) !=0 )
		{
			szMonth = szTmpA[1];
			nOptIndex ++;
			if(nOptIndex>=nMaxOption) nOptIndex=0;
		}
		
		szCodeName.Format("%c %-7.7s %-9.9s", cCallPut, szTmpA[1], szTmpA[2]);
		szCodeName.TrimRight();
		tmpArrCode[nOptIndex].InsertAt(0, szCode);
		tmpArrName[nOptIndex].InsertAt(0, szCodeName);
	}
	
	for(int k=0; k<nMaxOption; k++)
	{
		int nCnt = tmpArrName[k].GetSize();
		for(int j=0; j<nCnt; j++)
		{
			arrCode.Add(tmpArrCode[k].GetAt(j));
			arrName.Add(tmpArrName[k].GetAt(j));
		}
	}
}


void CMasterDataManager::_DoParsingCodeWeeklyMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName)
{
	if(szCodeList.GetLength()==0) return;

	szCodeList += "\n";
	int nPos1 = 0, nPos2, nOffset=0;
	CString szTmp, szCode, szCodeName;
	const int nRequest = 3;
	CString szTmpA[nRequest], szMonth;
	char cCallPut;
	
	const int nMaxOption = 11;
	int	nOptIndex=-1;
	CStringArray tmpArrCode[nMaxOption], tmpArrName[nMaxOption];
	
	int nDataLen = szCodeList.GetLength();
	while(nPos1>=0 && nOffset<=nDataLen)
	{
		nPos1 = szCodeList.Find('\n', nOffset);
		if(nPos1<0) break;
		
		szTmp = szCodeList.Mid(nOffset, nPos1-nOffset);
		nOffset = (nPos1+1);
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szCode	= szTmp.Left(nPos2);
		szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
		
		for(int i=0; i<nRequest-1; i++)
		{
			nPos2 = szTmp.Find(' ');	if(nPos2<0) continue;
			
			int nTemp = 2;
			if (strncmp(szTmp, "M ", nTemp) == 0)
			{
				szTmp = szTmp.Mid(nTemp);
				szTmpA[i] = "M " + szTmp.Left(nPos2);
				szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
			}			
			else
			{
				szTmpA[i] = szTmp.Left(nPos2);
				szTmp = szTmp.Mid(nPos2); szTmp.TrimLeft();
			}
		}
		
		nPos2 = szTmp.Find('\t');	if(nPos2<0) continue;
		szTmpA[nRequest-1] = szTmp.Left(nPos2);	szTmpA[nRequest-1].TrimRight();
		
		szCodeList = szTmp.Mid(nPos2+1);
		szCodeList += "\n";
		nOffset = 0;
		
		cCallPut = szTmpA[0].GetAt(0);		
		if(szMonth.Compare(szTmpA[1]) !=0 )
		{
			szMonth = szTmpA[1];
			nOptIndex ++;
			if(nOptIndex>=nMaxOption) nOptIndex=0;
		}
		
		szCodeName.Format("%s %-7.7s %-9.9s", szTmpA[0], szTmpA[1], szTmpA[2]);
		szCodeName.TrimRight();
		tmpArrCode[nOptIndex].InsertAt(0, szCode);
		tmpArrName[nOptIndex].InsertAt(0, szCodeName);
	}
	
	for(int k=0; k<nMaxOption; k++)
	{
		int nCnt = tmpArrName[k].GetSize();
		for(int j=0; j<nCnt; j++)
		{
			arrCode.Add(tmpArrCode[k].GetAt(j));
			arrName.Add(tmpArrName[k].GetAt(j));
		}
	}
}

