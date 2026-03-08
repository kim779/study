// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_test.h"
#include "MapWnd.h"
#include "../../H/interMSG.h"
#include "../../H/axislog.h"
#include "inout.h"
#include "../AXIS/axMsg.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(_param* pParam, CWnd* pWizard )
{
	EnableAutomation();
	m_pWizard = pWizard;
	m_sParamKey = pParam->key;
	m_sParamName = CString(pParam->name,pParam->name.GetLength());
}

CMapWnd::~CMapWnd()
{
}

void CMapWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER, OnMessage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMapWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMapWnd)
	DISP_FUNCTION(CMapWnd, "axlog", axlog, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMapWnd, "IDtoSha", IDtoSha, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CMapWnd, "getroot", getroot, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMapWnd, "openPDF", openPDF, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMapWnd, "Writelog", Writelog, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMapWnd, "HTSEncode", HTSEncode, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMapWnd, "ExcuteFile", ExcuteFile, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CMapWnd, "OpenChrome", dispidOpenChrome, OpenChrome, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMapWnd, "EDGE_lib_Installed", dispidEDGE_lib_Installed, EDGE_lib_Installed, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CMapWnd, "Memo_Search", dispidMemo_Search, Memo_Search, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMapWnd, "Memo_Upload", dispidMemo_Upload, Memo_Upload, VT_EMPTY, VTS_BSTR VTS_BSTR)
	//DISP_PROPERTY_NOTIFY(CMapWnd, "sMemo", dispidsMemo, m_sMemo, OnsMemoChanged, VTS_BSTR)
	DISP_PROPERTY_NOTIFY(CMapWnd, "sMemo", m_sMemo, OnsMemoChanged, VT_BSTR)
	DISP_FUNCTION_ID(CMapWnd, "Memo_Delete", dispidMemo_Delete, Memo_Delete, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMapWnd, "WriteHlog", dispidWriteHlog, WriteHlog, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CMapWnd, "DecHFile", dispidDecHFile, DecHFile, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CMapWnd, "DecFileFromDlg", dispidDecFileFromDlg, DecFileFromDlg, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CMapWnd, "InitShared", dispidInitShared, InitShared, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CMapWnd, "SendSharedMsg", dispidSendSharedMsg, SendSharedMsg, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMapWnd, "GetScreenWidth", dispidGetScreenWidth, GetScreenWidth, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CMapWnd, "GetScreenHeight", dispidGetScreenHeight, GetScreenHeight, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CMapWnd, "GetDPIX", dispidGetDPIX, GetDPIX, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CMapWnd, "GetDPIY", dispidGetDPIY, GetDPIY, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CMapWnd, "GetMonitorX_inch", dispidGetMonitorX_inch, GetMonitorX_inch, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CMapWnd, "GetMonitorY_inch", dispidGetMonitorY_inch, GetMonitorY_inch, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CMapWnd, "DoSomething", dispidDoSomething, DoSomething, VT_EMPTY, VTS_BSTR VTS_BSTR)
	//DISP_PROPERTY_NOTIFY_ID(CMapWnd, "sVal", dispidsVal, m_sVal, OnsValChanged, VTS_BSTR)
	DISP_PROPERTY_NOTIFY(CMapWnd, "sVal", m_sVal, OnsValChanged, VT_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_IMapWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {867CD1F0-86C2-4642-A65D-9036298C4597}
static const IID IID_IMapWnd =
{ 0x867cd1f0, 0x86c2, 0x4642, { 0xa6, 0x5d, 0x90, 0x36, 0x29, 0x8c, 0x45, 0x97 } };

BEGIN_INTERFACE_MAP(CMapWnd, CWnd)
	INTERFACE_PART(CMapWnd, IID_IMapWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers
#define TRKEY_MEMO_BEFORE  117

#include <sstream>
#include <iomanip>
// 헥사 인코딩 함수
std::string ToHex(const std::string input) {
	std::ostringstream oss;
	for (unsigned char c : input) {
		oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
	}
	return oss.str();
}

// 헥사 디코딩 함수
std::string FromHex(const std::string& input) {
	std::string output;
	for (size_t i = 0; i < input.length(); i += 2) {
		std::string byte = input.substr(i, 2);
		char chr = static_cast<char>(std::stoi(byte, nullptr, 16));
		output.push_back(chr);
	}
	return output;
}


long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
	{
		struct	_extTHx* exth = (struct _extTHx*)lParam;
		m_slog.Format("\r\n[memo_cx_log][%-40s][%d][%-35s]--> key=[%s] len=[%d] key =[%d] ",
			__FUNCTION__, __LINE__, "DLL_OUBx", GetMemoTrKeyType(exth->key), exth->size, exth->key);
		m_slog.Trim();
		OutputDebugString(m_slog);


		if (exth->key == TRKEY_MEMO_INSERT || exth->key == TRKEY_MEMO_UPDATE)
		{
			mod_memo* poub = (mod_memo*)exth->data;
			const char chRet = poub->chretc[0];
			CString sEmg{}, stmp;

			m_slog.Format("\r\n[memo_cx_log][%-40s][%d][%-35s]-->  ret=[%c]  msg=[%.64s] key =[%d]",
				__FUNCTION__, __LINE__, GetMemoTrKeyType(exth->key), chRet, poub->chemsg, exth->key);
			m_slog.Trim();
			OutputDebugString(m_slog);

			if (chRet == '1')
			{
				m_slog.Format("\r\n[memo_cx_log][%-40s][%d][%-35s]--> code=[%.16s]  ", __FUNCTION__, __LINE__, "save or update", poub->chcode);
				m_slog.Trim();
				OutputDebugString(m_slog);
			}
			else
			{
				m_slog.Format("\r\n[memo_cx_log][%-40s][%d][%-35s]-->[%s]  ", __FUNCTION__, __LINE__, "save or update",
					exth->key == TRKEY_MEMO_INSERT ? "세이브 실패" : "업데이트 실패");
				OutputDebugString(m_slog);
			}
		}
		else if (exth->key == TRKEY_MEMO_SEARCH)
		{
			mod_memo* poub = (mod_memo*)exth->data;
			const char chRet = poub->chretc[0];
			CString sEmg{}, stmp;

			m_slog.Format("\r\n[memo_cx_log][%-40s][%d][%-35s]-->ret=[%c] len=[%d] msg=[%.64s] ",
				__FUNCTION__, __LINE__, GetMemoTrKeyType(exth->key) + " dll_oubx", chRet, exth->size, poub->chemsg);
			OutputDebugString(m_slog);

			if (chRet == '1')
			{
				m_sMemo.Format("%.15360s", poub->chmemo);
				m_sMemo.Trim();


				m_slog.Format("\r\n[memo_cx_log][%-40s][%d][%-35s]--> code=[%.16s] memo=[%s]",
					__FUNCTION__, __LINE__, GetMemoTrKeyType(exth->key) + " dll_oubx", poub->chcode, CString(poub->chmemo, 64).Trim());
				OutputDebugString(m_slog);

				m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_sParamKey, evOnDblClk/*DblClick*/)),
					(LPARAM)m_sParamName.GetString());
			}
			else
			{

			}
		}
		else if (exth->key == TRKEY_MEMO_CHECK)
		{
			st_mod_SDEmemo* poub = (st_mod_SDEmemo*)exth->data;
			const char chRet = poub->chretc[0];
			CString sEmg{}, stmp, sKey;

			m_slog.Format("\r\n[memo_cx_log][%-40s][%d][%-35s]-->ret=[%c]   msg=[%.64s] ", __FUNCTION__, __LINE__,
				GetMemoTrKeyType(exth->key) + " dll_oubx", chRet, (char*)exth->data);
			OutputDebugString(m_slog);

			sKey.Format("%.16s", poub->chcode);
			sKey.Trim();

			mid_memo* pmid = new mid_memo;
			memset(pmid, 0x00, sizeof(mid_memo));
			char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
			if ((long)pData > 1)
				memcpy(pmid->chusid, pData, strlen(pData));

			memcpy(pmid->chcode, (char*)sKey.GetBuffer(0), sKey.GetLength());
			memcpy(pmid->chmemo, (LPSTR)(LPCTSTR)m_strMemo, m_strMemo.GetLength());

			if (chRet == '1')
			{
				pmid->chgubn[0] = 'U';
				m_slog.Format("\r\n[memo_cx_log][%-40s][%d][%-35s]--> code=[%.16s] 메모 서버 존재 [%.50s]",
					__FUNCTION__, __LINE__, GetMemoTrKeyType(exth->key) + " dll_oubx", poub->chcode, m_strMemo);
				OutputDebugString(m_slog);

				SendMemoTR_Control("pidomemo", (char*)pmid, sizeof(mid_memo), US_KEY, TRKEY_MEMO_UPDATE, sKey);
			}
			else
			{
				pmid->chgubn[0] = 'I';
				m_slog.Format("\r\n[memo_cx_log][%-40s][%d][%-35s]--> code=[%.16s] 메모 서버없어  [%.50s]",
					__FUNCTION__, __LINE__, GetMemoTrKeyType(exth->key) + " dll_oubx", poub->chcode, m_strMemo);
				OutputDebugString(m_slog);

				SendMemoTR_Control("pidomemo", (char*)pmid, sizeof(mid_memo), US_KEY, TRKEY_MEMO_INSERT, sKey);
			}
		}
		else if (exth->key == TRKEY_MEMO_DELETE)
		{
			st_mod_SDEmemo* poub = (st_mod_SDEmemo*)exth->data;
			const char chRet = poub->chretc[0];

			m_slog.Format("\r\n[memo_cx_log][%-40s][%d][%-35s]--> code=[%.16s] ret=[%c]",
				__FUNCTION__, __LINE__, GetMemoTrKeyType(exth->key) + " dll_oubx", poub->chcode, chRet);
			OutputDebugString(m_slog);
		}
	}
	break;
	default:break;
	}
	return 0;
}

void CMapWnd::axlog(LPCTSTR stag, LPCTSTR slog) 
{
	CString stmp;

	stmp.Format("[%s][%s]", stag, slog);
	OutputDebugString(stmp);
}

UINT AFXAPI CMapWnd::HashDataAXIS(LPCSTR key)
{
	UINT nHash = 0;
	while (*key)
		nHash = (nHash << 5) + nHash + *key++;
	return nHash;
}

BSTR CMapWnd::IDtoSha(LPCTSTR sId) 
{
	CString strResult;

	strResult.Format("%08u", HashDataAXIS((LPCSTR)sId));
	return strResult.AllocSysString();
}

BSTR CMapWnd::getroot() 
{
	TCHAR	szTemp[MAX_PATH]={0};
	CString strResult;

	::GetModuleFileName(0, szTemp, sizeof(szTemp));
	strResult = szTemp;
	strResult.Replace("exe\\axis.exe", "");
	return strResult.AllocSysString();
}

void CMapWnd::openPDF(LPCTSTR strPDF) 
{
	CString strpath;
	TCHAR	szTemp[MAX_PATH]={0};

	::GetModuleFileName(0, szTemp, sizeof(szTemp));
	strpath = szTemp;
	strpath.Replace("exe\\axis.exe","");
	strpath += "tab\\";
	strpath += strPDF;

	::ShellExecute(NULL, "open", (LPCTSTR)strpath, NULL, NULL, SW_SHOWNORMAL); 
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_sParamKey, evOnDblClk)), (LPARAM)m_sParamName.GetString());
}

void CMapWnd::Writelog(LPCTSTR spath, LPCTSTR slog) 
{
	TCHAR	szTemp[MAX_PATH]={0};
	CString DefaultFileDataPath;
	CString sroot;

	::GetModuleFileName(0, szTemp, sizeof(szTemp));
	DefaultFileDataPath = szTemp;
	DefaultFileDataPath.Replace("exe\\axis.exe", "");
	sroot.Format("%s", spath);
	sroot.TrimRight();
	if (sroot.IsEmpty())
		DefaultFileDataPath += "tab\\url.ini";

	CFileFind finder;
	if (!finder.FindFile(DefaultFileDataPath))
		WritePrivateProfileString("USER","ENC","1",DefaultFileDataPath);

	TRY 
	{
		FILE *fp = fopen(DefaultFileDataPath, "a+");
		if (!fp) return;
		
		const	CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));
		
		va_list argptr;
		va_start(argptr, slog);
		vfprintf(fp, slog, argptr);
		va_end(argptr);
		fprintf(fp, "\n");
		
		fclose(fp);
	}
	CATCH (CMemoryException, e)
	{
		
	}
	END_CATCH

}

void CMapWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnLButtonUp(nFlags, point);
}

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO: Add your specialized creation code here
	CString sName{};
	m_strHome = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);
	m_strConfig = m_strHome + "\\tab\\EdgeOption.ini";
	sName = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), 0);

	 char readB[256]{};
	 int readL{};

	 if (m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0))
		 GetPrivateProfileString(_T("[InstallPath]"), "custom", "https://go.microsoft.com/fwlink/p/?LinkId=2124703", readB, sizeof(readB), m_strConfig);
	 else
		 GetPrivateProfileString(_T("[InstallPath]"), "staff", "", readB, sizeof(readB), m_strConfig);
	
	 m_strInstall.Format("%s", readB); 
	 m_strInstall.Trim();

	 //m_strInstallName
	 memset(readB, 0x00, 256);
	 GetPrivateProfileString(_T("[Install]"), "File", "MICROSOFTEDGEWEBVIEW2SETUP.EXE", readB, sizeof(readB), m_strConfig);
	 m_strInstallName.Format("%s", readB);
	 m_strInstallName.Trim();

	m_pMainWnd = AfxGetMainWnd();
	if (!m_pMainWnd)
	{
		int readL = 0;
		char readB[1024];
		CString userD;
		userD.Format("%s\\%s\\%s\\%s.ini", m_strHome, "user", sName, sName);
		readL = GetPrivateProfileString("MODE", "main", "", readB, sizeof(readB), userD);

		if (readL <= 0)
			return 0;

		CString sWnd;
		sWnd.Format("%s", readB);
		int ddata = atoi(sWnd);
		HWND hWnd = (HWND)ddata;
		m_pMainWnd = CWnd::FromHandle(hWnd);
	}

	return 0;
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnTimer(nIDEvent);
}

CString Encode(const char* lpszSource, const char* key)
{
	const	int	lenKey = strlen(key);
	const	int	lenSource = strlen(lpszSource);

	if (!lenSource || !lenKey)
		return "";

	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(lenSource + 1);

	for (int ii = 0, idata = 0; ii < lenSource; ii++)
	{
		idata = key[ii % lenKey];
		idata = 0x0f & idata;
		idata =	lpszSource[ii] ^ idata;
		buffer[ii] = (lpszSource[ii] ^ (0x0f & key[ii % lenKey])) - 1;
	}
	buffer[lenSource] = 0x00;

	CString result;
	result = CString(buffer.get());

	/** Decoding test 

	for (int ii = 0; ii < lenSource; ii++)
	{
		buffer[ii] = (result[ii] + 1) ^ (0x0f & key[ii % lenKey]);
	}

	AfxMessageBox(result);
	AfxMessageBox(buffer);
	**/

	buffer.reset();

	return result;
}

BSTR CMapWnd::HTSEncode(LPCTSTR sID, LPCTSTR sKey) 
{
	CString strResult;
	CString strkey;

	strkey.Format("%s", sKey);
	strkey.TrimRight();
	if (strkey.IsEmpty())
		strkey = "ibkis";

	strResult = Encode(sID, strkey);
	return strResult.AllocSysString();
}

void CMapWnd::ExcuteFile(LPCTSTR fullpath) 
{
	::ShellExecute(NULL, "open", fullpath, NULL, NULL, SW_SHOWNORMAL); 
}


void CMapWnd::OpenChrome(BSTR strUrl)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString sUrl;
	sUrl.Format("%s", strUrl);
	ShellExecute(nullptr, "open", "chrome.exe", sUrl, nullptr, SW_SHOWNORMAL);
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}

#include <afxwin.h>
#include <iostream>
#include <windows.h>

BOOL Is64BitWindows()
{
	BOOL bIsWow64 = FALSE;

	// IsWow64Process is not available on all versions of Windows. Use GetProcAddress to get the function pointer.
	typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			// Handle error
		}
	}

	return bIsWow64;
}

BOOL CheckWindowsBitness()
{
	if (Is64BitWindows())
	{
		//AfxMessageBox(_T("This is a 64-bit version of Windows."));
		return TRUE;
	}
	else
	{
		//AfxMessageBox(_T("This is a 32-bit version of Windows."));
		return FALSE;
	}
}

//64ibt
//HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\EdgeUpdate\Clients\{F3017226-FE2A-4295-8BDF-00C3A9A7E4C5}
//HKEY_CURRENT_USER\Software\Microsoft\EdgeUpdate\Clients\{F3017226-FE2A-4295-8BDF-00C3A9A7E4C5}
// 
//32bit
//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\EdgeUpdate\Clients\{F3017226-FE2A-4295-8BDF-00C3A9A7E4C5}
//HKEY_CURRENT_USER\Software\Microsoft\EdgeUpdate\Clients\{F3017226-FE2A-4295-8BDF-00C3A9A7E4C5}

BOOL  CMapWnd::CheckEdgelibInstalled()
{
	BOOL b64bit = CheckWindowsBitness();
	CStringArray arrRegEdgeKey{}, arrRegEdgeSub{};
	if (b64bit)
	{
		arrRegEdgeKey.Add(_T("SOFTWARE\\WOW6432Node\\Microsoft\\EdgeUpdate")); //HKEY_LOCAL_MACHINE
		arrRegEdgeKey.Add(_T("Software\\Microsoft\\EdgeUpdate"));  //HKEY_CURRENT_USER

		arrRegEdgeSub.Add(_T("F3017226-FE2A-4295-8BDF-00C3A9A7E4C5"));
		arrRegEdgeSub.Add(_T("F3017226-FE2A-4295-8BDF-00C3A9A7E4C5"));
	}
	else
	{
		arrRegEdgeKey.Add(_T("SOFTWARE\\Microsoft\\EdgeUpdate")); //HKEY_LOCAL_MACHINE
		arrRegEdgeKey.Add(_T("Software\\Microsoft\\EdgeUpdate"));  //HKEY_CURRENT_USER

		arrRegEdgeSub.Add(_T("F3017226-FE2A-4295-8BDF-00C3A9A7E4C5"));
		arrRegEdgeSub.Add(_T("F3017226-FE2A-4295-8BDF-00C3A9A7E4C5"));
	}

	//Microsoft Edge WebView2 런타임
	BOOL binstalled{};
	CString sProgramName{}, strEdgeEnv{};
	sProgramName = _T("Microsoft Edge WebView2 런타임");

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString sKey, stmp, sval;
	HKEY hKey;
	BOOL bEdgeInstalled{};

	for (int ii = 0; ii < arrRegEdgeKey.GetSize(); ii++)
	{
		sKey = arrRegEdgeKey.GetAt(1);
		OutputDebugString("\r\n[reg] -------------------------------------------------------------");

		stmp.Format("\r\n[reg]--1--  sKey=[%s] [%s]", sKey, ii == 0 ? "LOCAL_MACHINE" : "CURRENT_USER");
		OutputDebugString(stmp);

		if (RegOpenKeyEx(ii == 0 ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER, sKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			char szSubKey[MAX_PATH];
			DWORD dwIndex = 0;
			DWORD dwSize = MAX_PATH;

			while (RegEnumKeyEx(hKey, dwIndex, szSubKey, &dwSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
			{
				HKEY hSubKey;
				if (RegOpenKeyEx(hKey, szSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
				{
					stmp.Format("\r\n\t\t[reg]--2-- szSubKey=[%s], dwindex=[%d]", szSubKey, dwIndex);
					OutputDebugString(stmp);
					char szDisplayName[MAX_PATH];
					DWORD dwSize = sizeof(szDisplayName);

					DWORD dwSubIndex = 0;
					char szSubSubKey[MAX_PATH]{};
					while (RegEnumKeyEx(hSubKey, dwSubIndex, szSubSubKey, &dwSize, nullptr, nullptr, nullptr, nullptr) == ERROR_SUCCESS)
					{
						stmp.Format("\r\n\t\t\t\t[reg]--3-- szSubSubKey=[%s], dwSubIndex=[%d]", szSubSubKey, dwSubIndex);
						OutputDebugString(stmp);
						sval.Format("%s", szSubSubKey);
						sval.Trim();
						if (sval.Find(arrRegEdgeSub.GetAt(ii)) >= 0)
						{
							stmp.Format("\r\n\t\t\t\t\t\t[reg]--4--@@@@@ FOUND @@@@@ szSubSubKey=[%s], dwSubIndex=[%d]", szSubSubKey, dwSubIndex);
							OutputDebugString(stmp);
							return TRUE;
						}
						else
						{
							stmp.Format("\r\n\t\t\t\t\t\t[reg]--4-- !!!NOT FOUND!!! cszSubSubKey=[%s], dwSubIndex=[%d]", szSubSubKey, dwSubIndex);
							OutputDebugString(stmp);
						}
						dwSize = MAX_PATH;
						dwSubIndex++;
					}
				}
				dwSize = MAX_PATH;
				dwIndex++;
			}//while
		}//if
	}//for

	return FALSE;
}

DWORD WINAPI DownloadAndInstallWV2RT(LPVOID lpParameter)
{
	CMapWnd* pwnd = (CMapWnd*)lpParameter;
	int returnCode{};
	HRESULT hr = URLDownloadToFile(NULL, pwnd->m_strInstall, pwnd->m_strInstallName, 0, 0);
	//동기 인듯 하다. 
	if (hr == S_OK)
	{
		//Either Package the WebView2 Bootstrapper with your app or download it using fwlink
		//then invoke install at Runtime

		SHELLEXECUTEINFO shExInfo = { 0 };
		shExInfo.cbSize = sizeof(shExInfo);
		shExInfo.fMask = SEE_MASK_NOASYNC;
		shExInfo.hwnd = 0;
		shExInfo.lpVerb = "runas";
		shExInfo.lpFile = "test.exe";
		shExInfo.lpParameters = " /silent /install";
		shExInfo.lpDirectory = 0;
		shExInfo.nShow = 0;
		shExInfo.hInstApp = 0;

		if (ShellExecuteEx(&shExInfo))
			returnCode = 0;  //Install successful
		else
			returnCode = GetLastError(); //Install failed
	}
	else
	{
		returnCode = GetLastError();
	}

	return returnCode;
}

void CMapWnd::EDGE_lib_Installed()
{
	BOOL bInstalled = CheckEdgelibInstalled();
	CString stmp;
	stmp.Format("[reg] --------EdgeInstalled--------=[%d]", bInstalled);
	OutputDebugString(stmp);

	if (bInstalled)
	{
		int returnCode{};
		if (MessageBox("Microsoft Edge WebView2 설치하시겠습니까?\n", "IBKs", MB_YESNO) == IDYES)
		{
			if (m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0))
			{
				uintptr_t handle = _beginthreadex(0, 0, (_beginthreadex_proc_type)DownloadAndInstallWV2RT, (void*)this, 0, 0);	 //고객이면 다운로드
				WaitForSingleObject((HANDLE)handle, INFINITE);
				
				DWORD exitCode = 0;
				if(GetExitCodeThread((HANDLE)handle, &exitCode))
				{ 
					if (exitCode ==  0)
					{
						const CTime time = CTime::GetCurrentTime();
						m_slog.Format("[EdgeInstall][%s] success", (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));
						WritePrivateProfileString(_T("[InstallResult]"), _T("res"), m_slog,  m_strConfig);
					}
					else
					{
						const CTime time = CTime::GetCurrentTime();
						m_slog.Format("[EdgeInstall][%s] fail err =[%d]", (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "), exitCode);
						WritePrivateProfileString(_T("[InstallResult]"), _T("res"), m_slog, m_strConfig);
					}
				}
			}
			//else
			//{ //직원이면 로컬 다운로드
			//	CString strfile{};
			//	strfile.Format("%s\\%s", m_strHome, "MicrosoftEdgeWebview2Setup.exe");

			//	STARTUPINFO si;
			//	::ZeroMemory(&si, sizeof(STARTUPINFO));
			//	si.cb = sizeof(STARTUPINFO);
			//	PROCESS_INFORMATION pi;

			//	ZeroMemory(&si, sizeof(STARTUPINFO));
			//	ZeroMemory(&si, sizeof(PROCESS_INFORMATION));

			//	si.cb = sizeof(STARTUPINFO);
			//	si.dwFlags = STARTF_USESHOWWINDOW;
			//	si.wShowWindow = SW_SHOWNORMAL;

			//	if (CreateProcess(strfile,
			//		nullptr,
			//		nullptr,
			//		nullptr,
			//		FALSE,
			//		0,
			//		nullptr,
			//		nullptr,
			//		&si,
			//		&pi))
			//	{
			//		WaitForSingleObject(pi.hThread, INFINITE);
			//		CloseHandle(pi.hThread);
			//	}
			//}
		}
	}
}


void CMapWnd::Memo_Search(BSTR sCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	CString strCode;
	strCode.Format("%s", sCode);

	mid_SDEmemo* pmid = new mid_SDEmemo;
	pmid->chgubn[0] = 'S';

	char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
	if ((long)pData > 1)
		memcpy(pmid->chusid, pData, strlen(pData));

	memcpy(pmid->chcode, (char*)strCode.GetBuffer(0), strCode.GetLength());

	SendMemoTR_Control("pidomemo", (char*)pmid, sizeof(mid_SDEmemo), US_KEY, TRKEY_MEMO_SEARCH, strCode);
}

void CMapWnd::SendMemoTR_Control(CString name, char* datb, int datl, char stat, int trkey, CString sKey)
{
	struct _userTH udat {};
	char* szBuf{};
	int	idx = m_sParamName.GetLength();

	CString strtmp;
	switch (trkey)
	{
	case TRKEY_MEMO_INSERT:
		strtmp = "메모추가";
		break;
	case TRKEY_MEMO_SEARCH:
		strtmp = "메모서버조회";
		break;
	case TRKEY_MEMO_UPDATE:
		strtmp = "메모수정 및 업데이트";
		break;
	case TRKEY_MEMO_DELETE:
		strtmp = "메모 삭제";
		break;
	case TRKEY_MEMO_CHECK:
		strtmp = "메모 존재 여부 확인";
		break;
	}

	if (trkey == TRKEY_MEMO_CHECK || trkey == TRKEY_MEMO_DELETE)
	{
		szBuf = new char[sizeof(st_mid_SDEmemo) + sizeof(struct _userTH) + 20];
		CopyMemory(udat.trc, (char*)name.GetString(), name.GetLength());
		udat.key = m_sParamKey;
		udat.stat = stat;

		//// set buffer
		szBuf[0] = trkey;
		idx++;

		CopyMemory(&szBuf[1], (char*)m_sParamName.GetString(), m_sParamName.GetLength());
		szBuf[idx++] = '\t';

		CopyMemory(&szBuf[idx], &udat, L_userTH);
		idx += L_userTH;

		CopyMemory(&szBuf[idx], datb, datl);
	}
	else
	{
		szBuf = new char[sizeof(st_mid_memo) + sizeof(struct _userTH) + 20];
		memset(szBuf, 0x00, sizeof(st_mid_memo) + sizeof(struct _userTH) + 20);
		memset(szBuf, ' ', sizeof(st_mid_memo) + sizeof(struct _userTH));
		CopyMemory(udat.trc, (char*)name.GetString(), name.GetLength());
		udat.key = m_sParamKey;
		udat.stat = stat;

		//// set buffer
		szBuf[0] = trkey;
		idx++;

		CopyMemory(&szBuf[1], (char*)m_sParamName.GetString(), m_sParamName.GetLength());
		szBuf[idx++] = '\t';

		CopyMemory(&szBuf[idx], &udat, L_userTH);
		idx += L_userTH;

		CopyMemory(&szBuf[idx], datb, datl);
	}

	m_slog.Format("\r\n[memo_cx_log][%-40s][%d][%-35s]--> datl=[%d] trkey=[%s] [%s][%.50s]",
		__FUNCTION__, __LINE__, "SendMemoTR_Control",
		datl, GetMemoTrKeyType(trkey), strtmp, datb);
	m_slog.Trim();
	OutputDebugString(m_slog);

	if (m_pWizard->GetSafeHwnd())
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)szBuf);

	delete szBuf;
}


void CMapWnd::checkMemo(CString sKey)
{
	m_slog.Format("\r\n[memo_cx_log][%-40s][%-35s]", __FUNCTION__, sKey);
	m_slog.Trim();

	st_mid_SDEmemo* pmid = new st_mid_SDEmemo;
	pmid->chgubn[0] = 'E';

	char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
	if ((long)pData > 1)
		memcpy(pmid->chusid, pData, strlen(pData));

	memcpy(pmid->chcode, (char*)sKey.GetBuffer(0), sKey.GetLength());
	SendMemoTR_Control("pidomemo", (char*)pmid, sizeof(st_mid_SDEmemo), US_KEY, TRKEY_MEMO_CHECK, sKey);

}

void CMapWnd::Memo_Upload(BSTR sCode, BSTR sMemo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
		m_strMemo.Format("%s", sMemo);
	m_strMemo.Trim();

	m_slog.Format("\r\n[memo_cx_log][%-40s][%d][%-35s]--> sCode=[%s] m_strMemo=[%s]",
		__FUNCTION__, __LINE__, "!!!!!!Memo_Upload!!!!!!",
		sCode, m_strMemo);
	m_slog.Trim();
	OutputDebugString(m_slog);
	CString strcode;
	strcode.Format("%s", sCode);
	checkMemo(strcode);
}


void CMapWnd::OnsMemoChanged()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 속성 처리기 코드를 추가합니다.
}


void CMapWnd::Memo_Delete(BSTR sCode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strcode;
	strcode.Format("%s", sCode);

	mid_SDEmemo* pmid = new mid_SDEmemo;
	pmid->chgubn[0] = 'D';

	char* pData = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
	if ((long)pData > 1)
		memcpy(pmid->chusid, pData, strlen(pData));

	memcpy(pmid->chcode, (char*)strcode.GetBuffer(0), strcode.GetLength());
	SendMemoTR_Control("pidomemo", (char*)pmid, sizeof(st_mid_SDEmemo), US_KEY, TRKEY_MEMO_DELETE, strcode);
}


void CMapWnd::WriteHlog(BSTR sFile, BSTR sSec, BSTR sKey, BSTR sVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString stmp{}, strSec{}, strVal{}, strKey{}, strfile{};
	const	CTime time = CTime::GetCurrentTime();
	CString timeS;
	timeS.Format("%02d/%02d %02d:%02d:%02d", time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

	stmp.Format("%s", sSec);
	stmp.TrimRight();
	if (stmp.MakeUpper() == "IB999920") //파라미터 주문확인창 설정화면(IB999920) 경우 이화면의 부모화면을 섹션값으로 저장한다
	{
		strfile.Format("%s\\tab\\axis.ini", m_strHome);
		stmp.Empty();
		char	wb[512]{};
		
		const DWORD dwRc = GetPrivateProfileString("ParentMap", "IB999920","", wb, sizeof(wb), strfile);
		stmp.Format("%s", wb);
		stmp.TrimRight();
		std::string inputSec(stmp);
		std::string hexEncSec = ToHex(inputSec);
		strSec = hexEncSec.c_str();
	}
	else
	{
		std::string inputSec(stmp);
		std::string hexEncSec = ToHex(inputSec);
		strSec = hexEncSec.c_str();
	}

	stmp.Format("%s", sKey);
	stmp.TrimRight();
	if (stmp.IsEmpty())
	{
		std::string inputKey(timeS);
		std::string hexEncKey = ToHex(inputKey);
		strKey = hexEncKey.c_str();
	}
	else
	{
		stmp.Format("[%s]%s", timeS, sKey);
		std::string inputKey(stmp);
		std::string hexEncKey = ToHex(inputKey);
		strKey = hexEncKey.c_str();
	}

	stmp.Format("%s", sVal);
	std::string inputVal(stmp);  
	std::string hexEncVal = ToHex(inputVal);
	strVal = hexEncVal.c_str();

	CString sName{}, sfile;
	sName = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), 0);

	sfile.Format("%s\\user\\%s\\Crashlog\\Hlog.ini", m_strHome, sName);
	//stmp.Format("%s", sSec);
	WritePrivateProfileString(strSec, strKey, strVal, sfile);
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}


void CMapWnd::DecHFile(BSTR sFile, BSTR sFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString spath{};
	CString sName{};

	sName = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), 0);

	sName.Format("%08u", HashDataAXIS((LPCSTR)sName));
	spath.Format("%s\\user\\%s\\%s\\%s", m_strHome, sName, sFolder, sFile);
	CFile file;
	if (AxStd::isFileExist(spath))
	{
		if (!file.Open(spath, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
			return;

		const int size = gsl::narrow_cast<int>(file.GetLength());
		std::unique_ptr<char[]> m_hcB{};
		m_hcB = std::make_unique<char[]>(size + 1);
		const int len = file.Read(m_hcB.get(), size);
		file.Close();

		m_slog.Format("%s", m_hcB.get());
		m_slog.Replace("[", "5b");
		m_slog.Replace("]", "5d");

		m_slog.Replace("\r", "0d");
		m_slog.Replace("\n", "0a");
		m_slog.Replace("=", "3d");

		std::string decoding((m_slog));
		std::string decodedString = FromHex(decoding);
		CString decodedCString(decodedString.c_str()); 

		spath.Replace(".ini", "_dec.ini");

		FILE* fp;
		fopen_s(&fp, spath, "w");
		if (!fp)
			return;

		const char* cstr = (LPCTSTR)decodedCString;  
		fprintf(fp, "%s\n", cstr);

		fclose(fp);
	}
}


void CMapWnd::DecFileFromDlg()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString sName{}, sfile;
	sName = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), 0);

	sfile.Format("%s\\user\\%s\\Crashlog", m_strHome, sName);

	CFileDialog dlg(TRUE, "ini", NULL, OFN_FILEMUSTEXIST | OFN_LONGNAMES, "INI Files(*.ini)|*.ini|All Files(*.*)|*.*|" , this);

	dlg.m_ofn.lpstrInitialDir = sfile;

	if (dlg.DoModal() == IDOK)
	{
		CString sfilepath = dlg.GetPathName();
		CString sfileName{}, sfolderpath{};

		//sfileName = sfilepath.Mid(sfilepath.ReverseFind(('\\')) + 1);
		//sfolderpath = sfilepath.Left(sfilepath.ReverseFind(('\\')));

		CFile file;
		if (AxStd::isFileExist(sfilepath))
		{
			if (!file.Open(sfilepath, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
				return;

			const int size = gsl::narrow_cast<int>(file.GetLength());
			std::unique_ptr<char[]> m_hcB{};
			m_hcB = std::make_unique<char[]>(size + 1);
			const int len = file.Read(m_hcB.get(), size);
			file.Close();

			m_slog.Format("%s", m_hcB.get());
			m_slog.Replace("[", "5b");
			m_slog.Replace("]", "5d");

			m_slog.Replace("\r", "0d");
			m_slog.Replace("\n", "0a");
			m_slog.Replace("=", "3d");

			std::string decoding((m_slog));
			std::string decodedString = FromHex(decoding);
			CString decodedCString(decodedString.c_str());
			
			decodedCString.Replace("\n", "");
			sfilepath.Replace(".ini", "_dec.ini");

			FILE* fp;
			fopen_s(&fp, sfilepath, "w");
			if (!fp)
				return;

			const char* cstr = (LPCTSTR)decodedCString;
			fprintf(fp, "%s\n", cstr);

			fclose(fp);
		}
	}
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}


void CMapWnd::InitShared()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	strResult.Format("%s", (char*)AfxGetMainWnd()->SendMessage(WM_USER, 0x09));

	m_hKeyShared = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, strResult);

	AfxGetMainWnd()->SendMessage(WM_USER, MMSG_SHARED_REGWND, (LPARAM)this);
}


void CMapWnd::SendSharedMsg(BSTR strVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!m_hKeyShared)
		return;

	CString sVal;
	sVal.Format("%s", strVal);
	AfxGetMainWnd()->SendMessage(WM_USER, MMSG_SHARED_BROADCAST, (LPARAM)(LPCSTR)sVal);
}


BSTR CMapWnd::GetScreenWidth()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	strResult.Format("%d", GetSystemMetrics(SM_CXSCREEN));
	return strResult.AllocSysString();
}


BSTR CMapWnd::GetScreenHeight()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	strResult.Format("%d", GetSystemMetrics(SM_CYSCREEN));
	return strResult.AllocSysString();
}


BSTR CMapWnd::GetDPIX()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	HDC hdc = ::GetDC(m_hWnd);
	strResult.Format("%d", GetDeviceCaps(hdc, LOGPIXELSX));
	return strResult.AllocSysString();
}


BSTR CMapWnd::GetDPIY()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
	HDC hdc = ::GetDC(m_hWnd);
	strResult.Format("%d", GetDeviceCaps(hdc, LOGPIXELSY));
	return strResult.AllocSysString();
}


BSTR CMapWnd::GetMonitorX_inch()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	HDC hdc = ::GetDC(m_hWnd);
	double widthInInches = static_cast<double>(GetSystemMetrics(SM_CXSCREEN)) / GetDeviceCaps(hdc, LOGPIXELSX);
	strResult.Format("%f", widthInInches);
	return strResult.AllocSysString();
}


BSTR CMapWnd::GetMonitorY_inch()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	HDC hdc = ::GetDC(m_hWnd);
	double heightInInches = static_cast<double>(GetSystemMetrics(SM_CYSCREEN)) / GetDeviceCaps(hdc, LOGPIXELSY);
	strResult.Format("%f", heightInInches);
	return strResult.AllocSysString();
}

#include "../../H/jmcode.h"
void CMapWnd::DoSomething(BSTR sDo, BSTR sgubn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_slog.Format("[%s] [%s]", sDo, sgubn);
	
	CString strDo, strgubn;
	strDo.Format("%s", sDo);
	strDo.TrimRight();
	strgubn.Format("%s", sgubn);
	strgubn.TrimRight();
	
	if (strDo == "Get_SFCODE_VAL")
	{
		CFile file;
		CString path;
		CString code{};
		CString gnam{};
		int pid{}, pid2{};
		int ival{};

		path = m_strHome + "\\tab\\sfcode.dat";
		// 선물종목코드
		if (file.Open(path, CFile::modeRead))
		{
			vector<struct sfcode> m_fjcode;
			m_fjcode.resize(gsl::narrow_cast<const int>(file.GetLength() / sizeof(struct sfcode)));
			file.Read(&m_fjcode[0], m_fjcode.size() * sizeof(struct sfcode));
			file.Close();

			for (size_t i = 0; i < m_fjcode.size(); i++)
			{
				CString str(m_fjcode[i].gnam, sizeof(m_fjcode[i].gnam));
				gnam = str;
				gnam.TrimRight();

				char tmp2[3] = {};
				memcpy(tmp2, code.Mid(1, 2), 2);

				if (tmp2[0] >= 'B')
				{
					pid = 61 + (tmp2[0] - 66) * 36;

					if (tmp2[1] >= 'A' && tmp2[1] <= 'Z')
						pid2 = tmp2[1] - 55;
					else
						pid2 = tmp2[1] - 48;

					ival = pid + pid2 + 100;
				}
				else if (tmp2[0] >= '0' && tmp2[0] <= '9')
				{
					pid = (tmp2[0] - 48) * 36;

					if (tmp2[1] >= 'A' && tmp2[1] <= 'Z')
						pid2 = tmp2[1] - 55;
					else
						pid2 = tmp2[1] - 48;

					ival = pid + pid2 + 400;
				}
				else
					ival = atoi(tmp2) + 100;

				code.Format("%d", ival);
				m_sVal.Format("%s|%s|%s", strgubn, code, gnam);

				break;
			}
		}
		else
		{
			code.Format("%d", ival);
			m_sVal.Format("%s|%s|%s", strgubn, "", "");
		}

		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_sParamKey, evOnDblClk/*DblClick*/)),
			(LPARAM)m_sParamName.GetString());

	}
	else if (strDo == "SendMessage_To_Main")
	{

		const	char* dta = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, 4), (LPARAM)"");

		CString stmp;
		m_slog.Format("%s", sgubn);
		m_slog.TrimRight();
		stmp = Parser(m_slog, "\t");
		if(stmp == "modalpop")
			m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(MMSG_POP_MAP, 2), (LPARAM)(LPSTR)(LPCTSTR)m_slog);
		else
			m_pMainWnd->PostMessage(WM_USER, MAKEWPARAM(MMSG_POP_MAP, 0), (LPARAM)(LPSTR)(LPCTSTR)m_slog);
	}
	else if (strDo == "getCODE")
	{
		CString code;
		code.Format("%s", sgubn);
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)code);
		code.Empty();
		m_pWizard->SendMessage(WM_USER, MAKEWPARAM(0x45, 0), (long)(LPCTSTR)code);
	}
	else if (strDo == "getATMCode")
	{
		CString path;
		CFile	file;
		path.Format("%s\\tab\\opcode2.dat", m_strHome);
		struct  ojcodh  OJCodh {};

		if (!file.Open(path, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
		{
			MessageBox("파일[opcode.dat]이 존재하지 않습니다.");
			return ;
		}

		CArray<ojcode, ojcode>		m_arrayOcode;
		int len = file.Read(&OJCodh, sizeof(struct ojcodh));
		int codeN = gsl::narrow_cast<int>((file.GetLength() - len) / sizeof(struct ojcode));
		//CString slog;
		for (int ii = 0; ii < codeN; ii++)
		{
			struct  ojcode OJCode;
			file.Read(&OJCode, sizeof(struct ojcode));
			m_arrayOcode.Add(OJCode);
		} 
		file.Close();

		CString sATMCallCode, sATMPutCode;

		for (int ii = 0; ii < m_arrayOcode.GetSize(); ii++)
		{
			const struct ojcode& oj = m_arrayOcode.GetAt(ii);

			if (oj.atmg != 0x01)   // ATM 행사가만
				continue;

			// call : 인덱스가 높을수록 근월물 → 뒤에서부터 탐색
			for (int m = 10; m >= 0; m--)
			{
				if (oj.call[m].yorn == '1')
				{
					sATMCallCode = CString(oj.call[m].cod2, OCodeLen);
					break;
				}
			}

			// put : 인덱스가 낮을수록 근월물 → 앞에서부터 탐색
			for (int m = 0; m < 11; m++)
			{
				if (oj.put[m].yorn == '1')
				{
					sATMPutCode = CString(oj.put[m].cod2, OCodeLen);
					break;
				}
			}
			break;  // ATM은 1개뿐이므로 찾으면 종료
		}
		
		m_sVal.Format("%s\t%s", sATMCallCode, sATMPutCode);
	}
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}

CString CMapWnd::Parser(CString& srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CMapWnd::OnsValChanged()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 속성 처리기 코드를 추가합니다.
}
