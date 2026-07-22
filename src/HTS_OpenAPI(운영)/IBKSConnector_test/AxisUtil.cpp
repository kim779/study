#include "stdafx.h"
#include <winsock2.h>
#include <Iphlpapi.h>
#include "AxisUtil.h"


void WriteLog_1(LPCSTR log, ...)
{
#if 1
	TRY
	{
		CString slog;
		slog.Format("[AXIS][WriteLog] [%s]\n", log);
		OutputDebugString(slog);

		/*FILE* fp;
		fopen_s(&fp, "d:\\openapi.log", "a+");
		if (!fp) return;

		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));

		va_list argptr;
		va_start(argptr, log);
		vfprintf(fp, log, argptr);
		va_end(argptr);
		fprintf(fp, "\n");

		fclose(fp);*/
	}
		CATCH(CMemoryException, e)
	{

	}
	END_CATCH
#endif
}

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "IpHlpApi.lib")

//LPCSTR OCX[] = { "axWizard.ocx", "axSock.ocx", "axComCtl.ocx", "axXecure.ocx", "axCertify.ocx", 0 };
LPCSTR OCX[] = { "axWizard.ocx", "axSock.ocx", "axXecure.ocx", "axCertify.ocx", 0 };

bool RegisterOCX( CString path )
{
 	HINSTANCE hInst = LoadLibraryEx(path, NULL, LOAD_LIBRARY_AS_DATAFILE);	//2014.02.06 VISTA이상 버전에서는 Ex를 써야 라이브러리가 로딩된다.
//	HINSTANCE hInst = LoadLibrary(path);
	if (!hInst)		
		return false;

	HRESULT hr = S_FALSE;
	FARPROC func = (FARPROC)GetProcAddress(hInst, "DllRegisterServer");
	if (func) {
		hr = func();	
	}

	FreeLibrary(hInst);
	return SUCCEEDED(hr);
}

bool UnregisterOCX( CString path )
{
	// 	HINSTANCE hInst = LoadLibraryEx(path, NULL, LOAD_LIBRARY_AS_DATAFILE);	//2014.02.06 VISTA이상 버전에서는 Ex를 써야 라이브러리가 로딩된다.
	HINSTANCE hInst = LoadLibrary(path);
	if (!hInst) return false;

	HRESULT hr = S_FALSE;
	FARPROC func = (FARPROC)GetProcAddress(hInst, "DllUnregisterServer");
	if (func) {
		hr = func();	
	}
	FreeLibrary(hInst);
	return SUCCEEDED(hr);
}

bool SetAxisRegistry( CString key, CString root )
{
	HKEY hKey;
	DWORD dwDisp;
	CString sRegKey;

	sRegKey.Format(_T("Software\\%s\\AXIS Workstation V04.00\\Environment"), key);

	WriteLog_1("SetAxisRegistry  " + sRegKey);

	if ( RegCreateKeyEx(HKEY_CURRENT_USER, sRegKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &dwDisp) ) return false;

	if ( RegSetValueEx(hKey, _T("RootDir"), 0, REG_SZ, (LPBYTE)(LPCTSTR)root, root.GetLength()) ) 
	{
		RegCloseKey(hKey);
		return false;
	}
	RegCloseKey(hKey);
	WriteLog_1("SetAxisRegistry  root=" + root);
	return true;
}

CString GetGlbIP( CString usid, CString uspwd, CString uscpw )
{
	CString sIP, sGlbModule, sGlbProc;
	sGlbModule = _T("axisglb.dll");
	sGlbProc = _T("axGetGLB");

	typedef long (WINAPI *GLBFUNC)(LPCSTR, LPCSTR, int, LPCSTR, int, bool);

	HMODULE hInst = LoadLibrary(GetRootPath() + "\\" + sGlbModule);
	if (!hInst) return sIP;

	GLBFUNC glb_func = (GLBFUNC)GetProcAddress(hInst, sGlbProc);
	if (!glb_func) 
	{
		FreeLibrary(hInst);
		return sIP;
	}

	typedef struct {
		char user[12];
		char pass[8];
		char dats[10];
		char cpas[30];
		char uips[15];
		char madr[16];
		char cNull;
	} axglbM;
	
	typedef struct {
		char result;  // R: ok  X: failed (ip는 준다) ===> X, R 이외의 플래그일 경우 추후 메시지 처리 가능.
		char ip[32];
		char cNull;
	} axglbRcv; 
	
	axglbM mid;
	axglbRcv mod;

	//axlogon_mid mid;
	//axlogon_mod mod;

	memset(&mid, ' ', sizeof(mid));
	memset(&mod, ' ', sizeof(mod));

	memcpy(mid.user, usid,  usid.GetLength());
	memcpy(mid.pass, uspwd, uspwd.GetLength());
	memcpy(mid.cpas, uscpw, uscpw.GetLength());

	mid.dats[3]	= 'Y';

	//MessageBox(0, GetRootPath() + "\\..\\tab", "", MB_OK);
	glb_func( (LPCSTR)(GetRootPath() + "\\..\\tab"), (char*)&mid, sizeof(mid), (char*)&mod, sizeof(mod), true);
	sIP = CString(mod.ip, sizeof(mod.ip));

// 	OutputDebugString("[KSJ] [" + sIP + "]");

	FreeLibrary(hInst);

	sIP.TrimLeft(); 
	sIP.TrimRight();
	return sIP;
}

CString GetMacAddress( CString ip )
{
	CString sMacAddr;

	IP_ADAPTER_INFO AdapterInfo[16];
	DWORD dwBufLen = sizeof(AdapterInfo);
	DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
	if (dwStatus==ERROR_SUCCESS)
	{
		for(int n=0; n<16; ++n)
		{
			CString strIpAddr = AdapterInfo[n].IpAddressList.IpAddress.String;

			if (ip!=strIpAddr) continue;
			sMacAddr.Format( _T("%.2X%.2X%.2X%.2X%.2X%.2X"),
				AdapterInfo[n].Address[0], AdapterInfo[n].Address[1],
				AdapterInfo[n].Address[2], AdapterInfo[n].Address[3],
				AdapterInfo[n].Address[4], AdapterInfo[n].Address[5] );
			break;
		}
	}

	//2014.04.25 KSJ 혹시 Mac 주소를 못가져 오면 000000000000 으로 세팅해준다.
	if(sMacAddr.GetLength() != 12)
		sMacAddr = _T("000000000000");

	return sMacAddr;
}

CString GetLocalIP( CString ip, int port)
{
	CString sIP = "";
	char szHostName[64] = {0};
	WSADATA wsaData;

	if (WSAStartup(WINSOCK_VERSION, &wsaData)==0)
	{
		::gethostname(szHostName, sizeof(szHostName));
		if(lstrcmp(szHostName, "") != 0)
		{			
			int nSize = sizeof(SOCKADDR_IN);
			SOCKET sock;		
			sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sock != INVALID_SOCKET)
			{
				SOCKADDR_IN sin;
				sin.sin_family = AF_INET;
				sin.sin_addr.s_addr = inet_addr(ip);
				sin.sin_port = htons(port);
				if (connect(sock, (SOCKADDR*)&sin, nSize)==0)
				{
					IN_ADDR in;
					getsockname(sock, (PSOCKADDR)&sin, &nSize);
					closesocket(sock);
					CopyMemory(&in, &sin.sin_addr.s_addr, sizeof(IN_ADDR));
					sIP.Format(_T("%s"), inet_ntoa(in));
				}
				else
				{
					HOSTENT FAR* lphostent = ::gethostbyname(szHostName);
					for (int ii = 0; lphostent; ii++)
					{
						if (!lphostent->h_addr_list[ii]) break;
						sIP.Format("%u.%u.%u.%u",
							0xff & lphostent->h_addr_list[ii][0],
							0xff & lphostent->h_addr_list[ii][1],
							0xff & lphostent->h_addr_list[ii][2],
							0xff & lphostent->h_addr_list[ii][3]);
				}
				}
			}
		}
		WSACleanup();
	}
	return sIP;
}

CString GetRootPath()
{
	CString slog;
	static CString modPath;
	if (modPath.IsEmpty())
	{
		TCHAR path[MAX_PATH], fpath[MAX_PATH];
		GetModuleFileName(::AfxGetInstanceHandle(), path,  sizeof(path));
#if 1
		GetLongPathName(path, fpath, MAX_PATH);
		modPath = fpath;

		slog.Format("[openapi] GetRootPath modPath =[%s]", modPath);
		OutputDebugString(slog);

#else
		modPath = path;
#endif
		//modPath = modPath.Mid(0, modPath.ReverseFind(_T('\\')));
		modPath = modPath.Mid(0, modPath.ReverseFind(_T('\\')));
	}
	return modPath;
}