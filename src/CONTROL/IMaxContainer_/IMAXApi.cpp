// IMAXApi.cpp : implementation file
//

#include "stdafx.h"
#include "IMAXContainer.h"
#include "IMAXApi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

pfnimax_SetRootPath		imax_SetRootPath	= nullptr;
pfnimax_Init			imax_Init		= nullptr;
pfnimax_Open			imax_Open		= nullptr;
pfnimax_SetGroup		imax_SetGroup		= nullptr;
pfnimax_ChangeSkin		imax_ChangeSkin		= nullptr;
pfnimax_Close			imax_Close		= nullptr;
pfnimax_Free			imax_Free		= nullptr;
pfnimax_Resize			imax_Resize		= nullptr;
pfnimax_PreTranslateMessage	imax_PreTranslateMessage = nullptr;
pfnimax_GetStockType		imax_GetStockType	= nullptr;
pfnimax_CreateCodeWindow	imax_CreateCodeWindow	= nullptr;
pfnimax_MoveCodeWindow		imax_MoveCodeWindow	= nullptr;
pfnimax_DeleteCodeWindow	imax_DeleteCodeWindow	= nullptr;
pfnimax_ChangeCodeWindow	imax_ChangeCodeWindow	= nullptr;
pfnimax_GetHwnd			imax_GetHwnd		= nullptr;
pfnimax_GetScreen		imax_GetScreen		= nullptr;
pfnimax_GetScreenInfo		imax_GetScreenInfo	= nullptr;
pfnimax_SetEventWindow		imax_SetEventWindow	= nullptr;
pfnimax_SetEventProc		imax_SetEventProc	= nullptr;
pfnimax_GetViewCount		imax_GetViewCount	= nullptr;
pfnimax_GetISIN			imax_GetISIN		= nullptr;
pfnimax_SetAuth		imax_SetAuth = nullptr;

/////////////////////////////////////////////////////////////////////////////
// CIMAXApi

CIMAXApi::CIMAXApi()
{
	g_bLoadImaxGate = FALSE;
}

CIMAXApi::~CIMAXApi()
{
	//FreeImaxGate();
}

////////IMAX/////////////////////////////////////////////////////////////////////////////////////////////////
void CIMAXApi::LoadImaxGate()
{
m_tmpX.Format("LoadImaxGate() call"); dprint();
	try 
	{
		if (g_bLoadImaxGate)
		{
m_tmpX.Format("LoadImaxGate() Already loaded"); dprint();			
			return;
		}

		TCHAR szModulePath[MAX_PATH] = {0,};
		if (::GetModuleFileName(NULL, szModulePath, sizeof(szModulePath)) != 0) 
		{
			*_tcsrchr(szModulePath,_T('\\')) = _T('\0');	// c:/hts (/hts.exe)
		}

		CString strPath;		
		HMODULE hLib = ::GetModuleHandle("IMaxGate.dll");

		if (!hLib)
		{
			strPath.Format(_T("%s\\IMaxGate.dll"), szModulePath);	// c:/hts/IMaxGate.dll  (배포용)
			hLib = ::LoadLibrary(strPath);
		}

m_tmpX.Format("LoadImaxGate() IMaxGate.dll path = [%s]", strPath); dprint();

 		if (hLib)
		{
			imax_SetRootPath	= (pfnimax_SetRootPath)		::GetProcAddress(hLib, "imax_SetRootPath");
			imax_Init		= (pfnimax_Init)		::GetProcAddress(hLib, "imax_Init");
			imax_Open		= (pfnimax_Open)		::GetProcAddress(hLib, "imax_Open");
			imax_SetGroup		= (pfnimax_SetGroup)		::GetProcAddress(hLib, "imax_SetGroup");
			imax_ChangeSkin		= (pfnimax_ChangeSkin)		::GetProcAddress(hLib, "imax_ChangeSkin");
			imax_Close		= (pfnimax_Close)		::GetProcAddress(hLib, "imax_Close");
			imax_Free		= (pfnimax_Free)		::GetProcAddress(hLib, "imax_Free");
			imax_Resize		= (pfnimax_Resize)		::GetProcAddress(hLib, "imax_Resize");
			imax_PreTranslateMessage = (pfnimax_PreTranslateMessage)::GetProcAddress(hLib, "imax_PreTranslateMessage");
			imax_GetStockType	= (pfnimax_GetStockType)	::GetProcAddress(hLib, "imax_GetStockType");
			imax_CreateCodeWindow	= (pfnimax_CreateCodeWindow)	::GetProcAddress(hLib, "imax_CreateCodeWindow");
			imax_MoveCodeWindow	= (pfnimax_MoveCodeWindow)	::GetProcAddress(hLib, "imax_MoveCodeWindow");
			imax_DeleteCodeWindow	= (pfnimax_DeleteCodeWindow)	::GetProcAddress(hLib, "imax_DeleteCodeWindow");
			imax_ChangeCodeWindow	= (pfnimax_ChangeCodeWindow)	::GetProcAddress(hLib, "imax_ChangeCodeWindow");
			imax_GetHwnd		= (pfnimax_GetHwnd)		::GetProcAddress(hLib, "imax_GetHwnd");
			imax_GetScreen		= (pfnimax_GetScreen)		::GetProcAddress(hLib, "imax_GetScreen");
			imax_GetScreenInfo	= (pfnimax_GetScreenInfo)	::GetProcAddress(hLib, "imax_GetScreenInfo");
			imax_SetEventWindow	= (pfnimax_SetEventWindow)	::GetProcAddress(hLib, "imax_SetEventWindow");
			imax_SetEventProc	= (pfnimax_SetEventProc)	::GetProcAddress(hLib, "imax_SetEventProc");
			imax_GetViewCount	= (pfnimax_GetViewCount)	::GetProcAddress(hLib, "imax_GetViewCount");
			imax_GetISIN		= (pfnimax_GetISIN)		::GetProcAddress(hLib, "imax_GetISIN");
			imax_SetAuth = (pfnimax_SetAuth)		::GetProcAddress(hLib, "imax_SetAuth");
			
			g_bLoadImaxGate = TRUE;
			
#if _DEBUG	//DEBUG ONLY	
			// hts.exe 파일이 없으면 한단계 내림
			strPath.Format(_T("%s\\hts.exe"), szModulePath);
			if (::GetFileAttributes(strPath) == -1)
			{
				*_tcsrchr(szModulePath,_T('\\')) = _T('\0');	// c:\\hts
			}
#endif
			// IMAX root 경로 설정
 			SetRootPath(szModulePath);
m_tmpX.Format("LoadImaxGate() IMaxGate.dll loadig ok"); dprint();
 		}
		
	}
	catch(...) {
m_tmpX.Format("LoadImaxGate() IMaxGate.dll loadig error[%d]", GetLastError()); dprint();
	}
}

void CIMAXApi::FreeImaxGate()
{
	HMODULE hLib = ::GetModuleHandle("IMaxGate.dll");
m_tmpX.Format("FreeImaxGate() call"); dprint();
	if (hLib)
	{
		imax_SetRootPath	= nullptr;
		imax_Init		= nullptr;
		imax_Open		= nullptr;
		imax_SetGroup		= nullptr;
		imax_ChangeSkin		= nullptr;
		imax_Close		= nullptr;
		imax_Free		= nullptr;
		imax_Resize		= nullptr;
		imax_PreTranslateMessage = nullptr;
		imax_GetStockType	= nullptr;
		imax_CreateCodeWindow	= nullptr;
		imax_MoveCodeWindow	= nullptr;
		imax_DeleteCodeWindow	= nullptr;
		imax_ChangeCodeWindow	= nullptr;
		imax_GetHwnd		= nullptr;
		imax_GetScreen		= nullptr;
		imax_GetScreenInfo	= nullptr;
		imax_SetEventWindow	= nullptr;
		imax_SetEventProc	= nullptr;
		imax_GetViewCount	= nullptr;
		imax_GetISIN		= nullptr;
		imax_SetAuth = nullptr;
		
		if (g_bLoadImaxGate)
		{
			if (::FreeLibrary(hLib))
			{
				hLib = nullptr;
			}
		}
		g_bLoadImaxGate = FALSE;
	}
}

////////IMAX_API/////////////////////////////////////////////////////////////////////////////////////////////
void CIMAXApi::SetRootPath(char *szRootPath)
{
	if (imax_SetRootPath)
		imax_SetRootPath(szRootPath);
}

int CIMAXApi::IMAXInit(HWND hFrameWnd, char *szLogin, INT nType)
{
m_tmpX.Format("IMAXInit(imax_Init) instance[%p]", imax_Init); dprint();

	if (imax_Init)
		return imax_Init(hFrameWnd, szLogin, nType);
m_tmpX.Format("IMAXInit(imax_Init) error"); dprint();
	return FALSE;
}

HANDLE CIMAXApi::Open(HWND hwnd, char *szScreen, char *szCodeInfo, int nWidth, int nHeight)
{
m_tmpX.Format("Open(imax_Open) instance[%p]hwnd[%p]screen[%s]code[%s]width[%d]height[%d]", imax_Open, hwnd, szScreen, szCodeInfo, nWidth, nHeight); dprint();

	if (imax_Open)
		return imax_Open(hwnd, szScreen, szCodeInfo, nWidth, nHeight);
m_tmpX.Format("Open(imax_Init) error"); dprint();
	return nullptr;
}

void CIMAXApi::SetGroup(HANDLE hView, int nGroupNo)
{
m_tmpX.Format("SetGroup(imax_SetGroup) instance[%p]hView[%p]nGroupNo[%d]", imax_SetGroup, hView, nGroupNo); dprint();
	if (imax_SetGroup)
		imax_SetGroup(hView, nGroupNo);
}

void CIMAXApi::ChangeSkin(int nSkin)
{
m_tmpX.Format("ChangeSkin(imax_ChangeSkin) instance[%p]nSkin[%d]", imax_ChangeSkin, nSkin); dprint();
	if (imax_ChangeSkin)
		imax_ChangeSkin(nSkin);
}

void CIMAXApi::Close(HANDLE hView)
{
	if (imax_Close)
		imax_Close(hView);
}

void CIMAXApi::Free()
{
	if (imax_Free)
		imax_Free();
}

void CIMAXApi::Resize(HANDLE hView ,int cx, int cy)
{
	if (imax_Resize)
		imax_Resize(hView, cx, cy);

}

int CIMAXApi::PreTranslateMessage(HANDLE hView, MSG* pMsg)
{
	if (imax_PreTranslateMessage)
		return imax_PreTranslateMessage(hView, pMsg);

	return FALSE;
}

int CIMAXApi::GetStockType(char *szCodeInfo)
{
	// return >> -1: error , 0:코드정보없음,  1:stock, 2:index, 3:etf

	if (imax_GetStockType)
		return imax_GetStockType(szCodeInfo);

	return 0;
}

HWND CIMAXApi::CreateCodeWindow(HWND hParentWnd, char *szCodeInfo, int nLeft, int nTop, int nWidth, int nHeight, DWORD dwOption)
{
	if (imax_CreateCodeWindow)
		return imax_CreateCodeWindow(hParentWnd, szCodeInfo, nLeft, nTop, nWidth, nHeight, dwOption);

	return nullptr;
}

void CIMAXApi::MoveCodeWindow(HWND hWnd, int nLeft, int nTop, int nWidth, int nHeight)
{
	if (imax_MoveCodeWindow)
		imax_MoveCodeWindow(hWnd, nLeft, nTop, nWidth, nHeight);
}

void CIMAXApi::DeleteCodeWindow(HWND hWnd)
{
	if (imax_DeleteCodeWindow)
		imax_DeleteCodeWindow(hWnd);
}

void CIMAXApi::ChangeCodeWindow(HWND hWnd, char *szCodeInfo)
{
	if (imax_ChangeCodeWindow)
		imax_ChangeCodeWindow(hWnd, szCodeInfo);
}

HWND CIMAXApi::GetHwnd(HANDLE hView)
{
	if (imax_GetHwnd)
		return imax_GetHwnd(hView);
	return nullptr;
}

BOOL CIMAXApi::GetScreenInfo(CString strScr, CString& strImaxNo, int& nScrWidth, int& nScrHeight)
{
	if (imax_GetScreenInfo)
	{
		const	BOOL bRet = imax_GetScreenInfo(strScr.GetBufferSetLength(100), strImaxNo.GetBufferSetLength(100), &nScrWidth, &nScrHeight);
		strScr.ReleaseBuffer();
		strImaxNo.ReleaseBuffer();
		return bRet;
	}
	return FALSE;
}

BOOL CIMAXApi::GetScreen(CString strImaxNo, CString& strScr)
{
	if (imax_GetScreen)
	{
		const	BOOL bRet = imax_GetScreen(strImaxNo.GetBufferSetLength(100), strScr.GetBufferSetLength(100));
		strScr.ReleaseBuffer();
		strImaxNo.ReleaseBuffer();
		return bRet;
	}
	return FALSE;
}

void CIMAXApi::SetEventWindow(HANDLE hView, HWND hRecvWnd)
{
	if (imax_SetEventWindow)
		imax_SetEventWindow(hView, hRecvWnd);
}

void CIMAXApi::SetEventProc(HANDLE hView, IMXEVENTPROC lpProc, LPARAM lParam)
{
	if (imax_SetEventProc)
		imax_SetEventProc(hView, lpProc, lParam);
}


//imax_SetAuth = (pfnimax_SetAuth)		::GetProcAddress(hLib, "imax_SetAuth");
INT CIMAXApi::GetViewCount()
{
	if (imax_GetViewCount)
		return imax_GetViewCount();

	return 0;
}

BOOL CIMAXApi::GetISIN(char *szCodeInfo, CString& strISIN)
{
	if (imax_GetScreen)
	{
		// return >> -1: error , 0:코드정보없음,  1:정상
		const	int	nRet = imax_GetISIN(szCodeInfo, strISIN.GetBufferSetLength(20));
		strISIN.ReleaseBuffer();
		return (nRet == 1 ? TRUE : FALSE);
	}
	return FALSE;
}

void	CIMAXApi::SetAuth(char* sAuth)
{
	if (imax_SetAuth)
		imax_SetAuth(sAuth);
}

void CIMAXApi::dprint()
{
	if (m_tmpX.IsEmpty())
		return;

	m_tmpX.Insert(0, _T("[IMAX:API] : "));

// 필요시 사용할것. 평소엔 주석처리
//	OutputDebugString(m_tmpX);
	m_tmpX.Empty();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////