// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PartnerMng.h"

//--> For Solomon
#include "../../../SrcSite/include/axisfire.h"	
#include "../../../SrcSite/include/axisgenv.h"
//#include "../../inc/IAUPartnerMng.h"
//<-- For Solomon

#include "../../inc/IDataUtilMng.h"
#include "../../inc/IMainInfoMng.h"

//---------------------------------------------------------------------------------------
//
//				class CPartnerUtilManager
//
//---------------------------------------------------------------------------------------
CPartnerUtilManager::CPartnerUtilManager()
{
}

LPCSTR CPartnerUtilManager::_DoGetRootDir()
{
	if(m_szRootDir.GetLength()==0)
	{
		IMainInfoManager* pMIMng = (IMainInfoManager*)theApp.DoGetPctrInterface(UUID_IMainInfoManager);
		if(pMIMng)
			m_szRootDir = pMIMng->GetRootDir();
	}
	return (LPCSTR)m_szRootDir;
}

LRESULT CPartnerUtilManager::DoWinixVariant(HWND hWnd, int comm, CString data)
{
	if(!hWnd) hWnd = _GetSiteHandle();

	// 	CString retvalue = _T("");
	// 	char* dta = (char*)::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	// 	
	// 	if ((long)dta > 1)
	// 		retvalue.Format("%s", dta);
	// 	
	// 	return retvalue;
	
	return ::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
}


BOOL CPartnerUtilManager::OpenScreen(HWND hWnd, LPCSTR szScreenNo)
{
	if(!hWnd) hWnd = _GetSiteHandle();

	LRESULT dwRet =0;
	if(strlen(szScreenNo)==4)
	{
		CString szLink;
		szLink.Format("IB%s00", szScreenNo);
		dwRet = ::SendMessage(hWnd, WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCSTR)szLink);
	}
	else
	{
		dwRet = ::SendMessage(hWnd, WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCSTR)szScreenNo);
	}

	return TRUE;
}

BOOL CPartnerUtilManager::GetSymbolCode(LPCSTR szCode, CString& szSymbol)
{
	if(!m_pMasterMng)
	{
		m_pMasterMng = (IMasterDataManagerLast*)theApp.DoGetPctrInterface(UUID_IMasterDataManager);
	}
	if(m_pMasterMng)
	{
		char nCodeType = m_pMasterMng->GetMarketTypeFromCode(szCode);
		return GetSymbolCode(nCodeType, szSymbol);
	}
	return FALSE;
}

BOOL CPartnerUtilManager::IsCodeSymbol(LPCSTR szSymbol)
{
	if(	strcmp(szSymbol, "1301")==0    ||
		strcmp(szSymbol, "ecod")==0    ||	//ELW				
		strcmp(szSymbol, "jfcod")==0   ||
		strcmp(szSymbol, "cfcod")==0   ||		// 상품선물.JS.Kim_20101019
		strcmp(szSymbol, "50301")==0   ||
		strcmp(szSymbol, "109301")==0  ||
		strcmp(szSymbol, "gscode")==0  || //해외 			
		strcmp(szSymbol, "gicode")==0  || //업종
		strcmp(szSymbol, "20301")==0   || //업종
		strcmp(szSymbol, "ed_fcode")==0|| //선물옵션
		strcmp(szSymbol, "30301")==0   || //선물옵션
		strcmp(szSymbol, "40301")==0   || //선물옵션
		strcmp(szSymbol, "ed_fcod")==0 || //상품선물 
		strcmp(szSymbol, "100301")==0)    //상품선물
		return TRUE;

	return FALSE;
}

BOOL CPartnerUtilManager::GetSymbolCode(BYTE cChartMarketType, CString& szSymbol)
{
	//\DrCommon\PcTrMng\_MasterDataManager.cpp
	//CMasterDataManager::GetRecentCodeNameArray(..)에서도 GetSymbolCode(..)루틴 가져다 쓰고있음.
	szSymbol.Empty();

	switch(cChartMarketType)
	{
	case CMasterTypeInfo::STOCK_CHART:
		szSymbol = "1301";
		break;
	case CMasterTypeInfo::UPJONG_CHART:
		szSymbol = "20301";
		break;
	case CMasterTypeInfo::COMMODITY_CHART:
		szSymbol = "cfcod";
		break;
	case CMasterTypeInfo::FUTOPT_CHART:
		szSymbol = "30301";
		break;
	case CMasterTypeInfo::STOCKFUTOPT_CHART:
		szSymbol = "100301";
		break;
	case CMasterTypeInfo::FOREIGN_CHART:
		szSymbol = "50301";
		break;
	case CMasterTypeInfo::FUOPT_JIPYO_CHART:	//091128임의로 셋팅한 값임.
		szSymbol = "109301";
		break;
	case CMasterTypeInfo::FO_FOREIGN_CHART:
		szSymbol += "70301";
		break;
	case CMasterTypeInfo::FX_CHART:
		szSymbol += "80301";
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//>>JS.Kim_20101025  리얼 요청하는 MarketType코드와 History 구하는 코드가 다르다고 하여 리얼용 함수를 따로 만듬
BOOL CPartnerUtilManager::GetRealSymbolCode(BYTE cChartMarketType, CString& szSymbol)
{
	//\DrCommon\PcTrMng\_MasterDataManager.cpp
	//CMasterDataManager::GetRecentCodeNameArray(..)에서도 GetSymbolCode(..)루틴 가져다 쓰고있음.
	szSymbol.Empty();

	switch(cChartMarketType)
	{
	case CMasterTypeInfo::STOCK_CHART:
		szSymbol = "1301";
		break;
	case CMasterTypeInfo::UPJONG_CHART:
		szSymbol = "20301";
		break;
	case CMasterTypeInfo::COMMODITY_CHART:
	case CMasterTypeInfo::STOCKFUTOPT_CHART:
	case CMasterTypeInfo::FUTOPT_CHART:
		szSymbol = "30301";
		break;
	case CMasterTypeInfo::FOREIGN_CHART:
		szSymbol = "50301";
		break;
	case CMasterTypeInfo::FUOPT_JIPYO_CHART:	//091128임의로 셋팅한 값임.
		szSymbol = "109301";
		break;
	case CMasterTypeInfo::FO_FOREIGN_CHART:
		szSymbol += "70301";
		break;
	case CMasterTypeInfo::FX_CHART:
		szSymbol += "80301";
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
//<<

BOOL CPartnerUtilManager::AddToHistory(HWND hWnd, LPCSTR _szCode)
{
	// 같은 Code가 반복적으로 들어오고 있어서 추가함.
	if(m_szLastHistoryCode.Compare(_szCode)==0) return TRUE;

	//if(strcmp(_szCode, "10199999")==0) return TRUE;
	if(strcmp(&_szCode[3], "99999")==0) return TRUE;	//alzioyes:연결선물일때는 history추가하지 않는다.

	CString szSymbol;
	GetSymbolCode(_szCode, szSymbol);

	//091217 30301로 통일한다고 해서 수정함.
	if(szSymbol=="30301") 
	{
		szSymbol="ed_focod";
	}
	CString szData;
	szData.Format("%s\t%s", szSymbol, _szCode);
	
	::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, codeCC), (LPARAM)(LPCSTR)szData);
	m_szLastHistoryCode = _szCode;

	return TRUE;
}

BOOL CPartnerUtilManager::GetCodeInfoFromShared(CString szData, CString& szSymbol, CString& szCode)
{
	int nPos = szData.Find('\t');
	if(nPos<0) return FALSE;

	szSymbol = szData.Left(nPos);
	szCode = szData.Mid(nPos+1);
	szCode.TrimRight();
	return TRUE;
}

BOOL CPartnerUtilManager::GetChartMarketFromSymbol(BYTE &cChartMarketType, LPCSTR szSymbol)
{
	cChartMarketType = CMasterTypeInfo::NONE_CHART;

	if(strcmp(szSymbol, "1301")==0) cChartMarketType = CMasterTypeInfo::STOCK_CHART;
	else if(strcmp(szSymbol, "1301")==0) cChartMarketType = CMasterTypeInfo::STOCK_CHART;
	else if(strcmp(szSymbol, "1301")==0) cChartMarketType = CMasterTypeInfo::STOCK_CHART;

	return TRUE;
}

BOOL CPartnerUtilManager::GetCaptionTitle(HWND hWnd, LPCSTR szScreenNo)
{
	//@100204-alzioyes. winix에서 처리한다고 해서 DRFN에서는 처리안하는 것으로 수정함.
	return TRUE;
// 	if(!IsWindow(hWnd)) FALSE;
// 
// 	char* pHomeCC = (char*)::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);
// 	CString szCfg;
// 	szCfg.Format("%s\\%s", pHomeCC, "\\Hanaro\\PctrMng.cfg");
// 	char aTitle[MAX_PATH] = {0,};
// 
// 	if(::GetPrivateProfileString("Title", szScreenNo, "", aTitle, MAX_PATH, szCfg)>0)
// 	{
// 		//DoWinixVariant(hWnd, titleCC, aTitle);
// 		::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, titleCC), (LPARAM)aTitle);
// 		return TRUE;
// 	}
// 	return FALSE;
}

BOOL CPartnerUtilManager::ShareCode(HWND hWnd, LPCSTR szShareCode)
{
	if(!hWnd) hWnd = _GetSiteHandle();

	CString szSymbol;
	if(!GetSymbolCode(szShareCode, szSymbol)) return FALSE;

	//091217 30301로 통일한다고 해서 수정함.
	//if(szSymbol=="30301") szSymbol="focod";

	CString szParam;

	szParam.Format("%s\t%s", szSymbol, szShareCode);
	DoWinixVariant(hWnd, triggerCC, szParam);

	if(szSymbol.Compare("1301")==0)
	{
		int rRet = (int)::SendMessage(hWnd, WM_USER, MAKEWPARAM(codeTYPE, NULL), (LPARAM)(LPCSTR)szShareCode);
		if(rRet==elwType)	szSymbol = "ecod";

		szParam.Format("%s\t%s", szSymbol, szShareCode);
		DoWinixVariant(hWnd, triggerCC, szParam);
	}

	return TRUE;
}

/* example: ShareCode(..)
//@Solomon-->
#include "../../inc/IAUPartnerMng.h"

	IPartnerSvrManager* pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
	if(pPtSvrMng)
	{
		IPartnerUtilManager* pPtUtilMng = (IPartnerUtilManager*)pPtSvrMng->GetInerInterface(LUID_IPartnerUtilManager);
		if(pPtUtilMng)
		{
			pPtUtilMng->ShareCode(theApp.m_hPlatform, "005930");
		}
	}
//@Solomon<--
*/

BOOL CPartnerUtilManager::_IsFileExist(LPCSTR szFileName)
{
	CFileFind fileFind;
	return fileFind.FindFile(szFileName);
}

int	CPartnerUtilManager::_GetIniSectList(LPCSTR szIniPath, LPCSTR szSect, CStringList& list)
{
	int nAddCnt=0;
	
	DWORD nSize = 4096;
	char *pReturnedStrng=(char*)malloc(nSize);
	
	DWORD lReads = ::GetPrivateProfileSection(szSect, pReturnedStrng, nSize, szIniPath);
	if(lReads<0)
	{
		return 0;
	}

	IDataUtilManager* pDUMng = (IDataUtilManager*)theApp.DoGetPctrInterface(UUID_IDataUtilManager);

	int nStart=0, nEnd=0;
	int nStrSize=0;
	char aLine[MAX_PATH], aKey[100];//, aValue[MAX_PATH];
	CString szAddData;
	for(int i=0; i<(int)lReads; i++)
	{
		if(pReturnedStrng[i] == 0x00)
		{
			memcpy(aLine, &pReturnedStrng[nStart], i-nStart+1);	//Null포함.
			nStart = i+1;
			
			pDUMng->GetWord(aKey, aLine, '=');

			//SetEnvironmentVariable(aKey, aLine);
			szAddData.Format("%8.8s%s", aKey, aLine);
			list.AddTail(szAddData);
			nAddCnt++;
		}
	}
	
	free(pReturnedStrng);
	return nAddCnt;
}

BOOL CPartnerUtilManager::GetLinkScreenInfo(LPCSTR szScrrenNo, CStringList& szKeyList)
{
	if(szScrrenNo==NULL) return FALSE;

	int nScreenLen = strlen(szScrrenNo);

	char aScreen[5], aFullScreen[8+1];
	if(strlen(szScrrenNo)==4)
	{
		sprintf(aScreen, "SI%c%c", szScrrenNo[0], szScrrenNo[1]);
		sprintf(aFullScreen, "SI%s00", szScrrenNo);
	}
	else if(strlen(szScrrenNo)==8)
	{
		sprintf(aScreen, "SI%c%c", szScrrenNo[2], szScrrenNo[3]);
		sprintf(aFullScreen, "%s", szScrrenNo);
	}
	else return FALSE;

	IMainInfoManager* pMIMng = (IMainInfoManager*)theApp.DoGetPctrInterface(UUID_IMainInfoManager);

	CString szUser, szData;
	szUser.Format("%s\\%s", pMIMng->GetEncUserDir(), aScreen);
	szData.Format("%s\\%s\\%s", pMIMng->GetRootDir() , "\\mtbl", aScreen);

	LPCSTR szINI = szData;
	if(_IsFileExist(szUser))	szINI = szUser;

	return (BOOL)_GetIniSectList(szINI, aFullScreen, szKeyList);
}


BOOL CPartnerUtilManager::GetSiteUpKode(LPCSTR szCode, CString& rCode)
{
	if(!m_pMasterMng)
	{
		m_pMasterMng = (IMasterDataManagerLast*)theApp.DoGetPctrInterface(UUID_IMasterDataManager);
	}
	if(m_pMasterMng)
	{
		return m_pMasterMng->GetSiteUpKode(szCode, rCode);
	}
	return FALSE;
}

HWND CPartnerUtilManager::_GetSiteHandle()
{
	HWND hWnd = NULL;
	IPartnerSvrManager* pMng = (IPartnerSvrManager*)theApp.DoGetPctrInterface(UUID_IPartnerSvrManager);
	if(pMng)
		pMng->GetParentHandle(0, hWnd);

	return hWnd;
};

HBITMAP CPartnerUtilManager::GetBitmap(HWND hWnd, CString strPath)
{
	if(!hWnd) hWnd = _GetSiteHandle();

	CBitmap*	pBitmap = (CBitmap*)::SendMessage(hWnd, WM_USER, getBITMAP, (LPARAM)strPath.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

CBrush* CPartnerUtilManager::GetBrush(HWND hWnd, COLORREF clr)
{
	if(!hWnd) hWnd = _GetSiteHandle();

	return (CBrush*)::SendMessage(hWnd, WM_USER, getBRUSH, (LPARAM)clr);
}

CFont* CPartnerUtilManager::GetAxFont(HWND hWnd, CString fName, int point, bool bBold, bool bItalic)
{
	if(!hWnd) hWnd = _GetSiteHandle();

	struct _fontR fontR;
	
	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)::SendMessage(hWnd, WM_USER, getFONT, (long)&fontR);
}

CString CPartnerUtilManager::Variant(HWND hWnd, int nComm, CString strData)
{
	if(!hWnd) hWnd = _GetSiteHandle();

	CString strResult = _T("");
	char* szData = (char*)::SendMessage(hWnd, WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);
	
	if ((long)szData > 1)
		strResult.Format("%s", szData);
	
	return strResult;
}

COLORREF CPartnerUtilManager::GetAxColor(HWND hWnd, int nColor)
{
	if(!hWnd) hWnd = _GetSiteHandle();

	if (nColor & 0x02000000)
		return nColor;
	
	return ::SendMessage(hWnd, WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)nColor);
}

int CPartnerUtilManager::DoPlatformMenu(HWND hWnd, CWnd* pWnd, CPoint point, LPCSTR szScrrenNo)
{
	//mouseDLL사용해도 된다고 하는데. 그럼 Menu선택시 처리는?

	CStringList szKeyList;
	GetLinkScreenInfo(szScrrenNo, szKeyList);

	CMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING,(UINT)99, "연결화면등록");

	CString rMenuString;
	for(int i=0; i<szKeyList.GetCount(); i++)
	{
		rMenuString = szKeyList.GetAt(szKeyList.FindIndex(i));
		menu.AppendMenu(MF_STRING,(UINT)100+i, rMenuString.Mid(8));
	}
	
	UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
	int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, point.x, point.y, pWnd);
	menu.DestroyMenu();
	if(!nRetValue) return 0;

	if(nRetValue==99)
	{
		char aFullScreen[8+1];
		if(strlen(szScrrenNo)==4)
		{
			sprintf(aFullScreen, "SI%s00", szScrrenNo);
		}
		else if(strlen(szScrrenNo)==8)
		{
			sprintf(aFullScreen, "%s", szScrrenNo);
		}
		::SendMessage(hWnd, WM_USER, MAKEWPARAM(linkDLL, 0), (LPARAM)aFullScreen);
	}
	else
	{
		rMenuString = szKeyList.GetAt(szKeyList.FindIndex(nRetValue-100));
		rMenuString = rMenuString.Left(8);

		OpenScreen(hWnd, rMenuString);
	}

	return 1;
}


BOOL CPartnerUtilManager::GetLinkScreenOpen(HWND hWnd, LPCSTR szScrrenNo, int nIndex, BOOL bFront)
{
	if(nIndex==0)
	{
		char aFullScreen[8+1];
		if(strlen(szScrrenNo)==4)
		{
			sprintf(aFullScreen, "SI%s00", szScrrenNo);
		}
		else if(strlen(szScrrenNo)==8)
		{
			sprintf(aFullScreen, "%s", szScrrenNo);
		}
		::SendMessage(hWnd, WM_USER, MAKEWPARAM(linkDLL, 0), (LPARAM)aFullScreen);
		return TRUE;
	}

	CStringList szKeyList;
	GetLinkScreenInfo(szScrrenNo, szKeyList);
	if(szKeyList.GetCount()>0)
	{
		POSITION pos = NULL;
		int nRIndex = (nIndex-1);
		if(!bFront)
			nRIndex = szKeyList.GetCount()-nIndex;

		pos = szKeyList.FindIndex(nRIndex);
		if(pos)
		{
			CString szScreenNo;
			szScreenNo = szKeyList.GetAt(pos);
			szScreenNo = szScreenNo.Left(8);
			return OpenScreen(hWnd, szScreenNo);
		}
	}
	return FALSE;
}

CString CPartnerUtilManager::GetCurrentPalette()
{
	CString szPalette;
	szPalette.Format("%s\\tab\\PALETTE.INI", _DoGetRootDir());
	char aRetStr[20+1]={0,};
	::GetPrivateProfileString("General", "Palette", "Blue", aRetStr, 20, szPalette);

	m_szCurPalette = aRetStr;
	return aRetStr;
}

BOOL CPartnerUtilManager::GetTabColor(CString szPalette, int nIdx, COLORREF &clrStart, COLORREF &clrEnd)
{
	if(szPalette.IsEmpty())
	{
		if(m_szCurPalette.IsEmpty())
			GetCurrentPalette();
		szPalette = m_szCurPalette;
	}
	switch(nIdx)
	{
	case 0: //Tab Select색상.
		if(szPalette.CompareNoCase("Blue")==0)
		{
			clrStart = RGB(124+1, 147+1, 184+1);
			clrEnd = RGB(124, 147, 184);	
		}
		else if(szPalette.CompareNoCase("Cyan")==0)
		{
			clrStart = RGB( 91+1, 162+1, 188+1);
			clrEnd = RGB( 91, 162, 188);	
		}
		else if(szPalette.CompareNoCase("Violet")==0)
		{
			clrStart = RGB(128+1, 127+1, 181+1);
			clrEnd = RGB(128, 127, 181);	
		}
		else if(szPalette.CompareNoCase("Green")==0)
		{
			clrStart = RGB(133+1, 183+1, 135+1);
			clrEnd = RGB(133, 183, 135);
		}
		else if(szPalette.CompareNoCase("Brown")==0)
		{
			clrStart = RGB(192+1, 160+1, 141+1);
			clrEnd = RGB(192, 160, 141);
		}
		else if(szPalette.CompareNoCase("Gray")==0)
		{
			clrStart = RGB(157+1, 163+1, 174+1);
			clrEnd = RGB(157, 163, 174);	
		}
		return TRUE;
	case 1: //Tab UnSelect색상.
		{
			clrStart = RGB(244,244,244);
			clrEnd = RGB(242,242,242);	
		}
		break;
	}

	return FALSE;
}

#include "../../Chart/COMMONTR/TRDefine.h"
BOOL CPartnerUtilManager::GetTRCodeFromCode(BYTE cChartMarketType, LPCSTR szCode, CString &szTRCode)
{
	szTRCode=QUERY_STRJONGMOK;

	switch(cChartMarketType)
	{
		case CMasterTypeInfo::STOCK_CHART:
			szTRCode=QUERY_STRJONGMOK;	return TRUE;
		case CMasterTypeInfo::UPJONG_CHART:
			szTRCode=QUERY_STRUPJONG;	return TRUE;
		case CMasterTypeInfo::FUTOPT_CHART:
		case CMasterTypeInfo::COMMODITY_CHART:
			szTRCode=QUERY_STRFUTOPT;	return TRUE;
		case CMasterTypeInfo::STOCKFUTOPT_CHART:
			szTRCode=QUERY_STRSTOCKFUTOPT; return TRUE;
		case CMasterTypeInfo::FOREIGN_CHART:
			szTRCode=QUERY_STRFOREIGN;	return TRUE;
		case CMasterTypeInfo::FUOPT_JIPYO_CHART:
			szTRCode=QUERY_STRFOJIPYO;	return TRUE;
		case CMasterTypeInfo::FO_FOREIGN_CHART:
			szTRCode=QUERY_STRFOFOREIGN;	return TRUE;
		case CMasterTypeInfo::FX_CHART:
			szTRCode=QUERY_STRFX;		return TRUE;
		default: break;
	}
	return FALSE;
}