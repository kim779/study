#include "stdafx.h"
#include "PcTrMng.h"
#include "EtcUtil.h"

CString CSystemInfo::GetPcIP()
{
//@Solomon090819-->
// 	WORD wVersionRequested;
// 	WSADATA wsaData;
// 	char name[255];
// 	CString ip;
// 	PHOSTENT hostinfo;
// 	wVersionRequested = MAKEWORD( 2, 0 );
// 
// 	if(WSAStartup(wVersionRequested, &wsaData) == 0)
// 	{
// 		if(gethostname(name, 255) == 0 && (hostinfo = gethostbyname(name)) != NULL)
// 		{
// 			ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
// 			WSACleanup( );
// 			return ip;
// 		}
// 
// 		WSACleanup( );
// 	} 
// 
// 	ip = RegGetIP();
// 
// 	return ip;

	return "127.0.0.1"; //@Solomon090819
}

// 레지스트에서 ip주소를 얻는다
CString CSystemInfo::RegGetIP()
{
	int i = 0;
	CString ip, strPath, strKey;

	strPath = "System\\CurrentControlSet\\Services\\Class\\NetTrans";
	CRegistryManager RegMng;
	while( i <= 8)
	{
		strKey.Format("%s\\%04d", strPath, i);
		
		ip = RegMng.GetStrValue(HKEY_LOCAL_MACHINE, strKey, "IPAddress", " ");
		
		if( ip.CompareNoCase(" ") == 0 || ip.CompareNoCase("0.0.0.0") == 0 ||
			ip.CompareNoCase("127.0.0.1") == 0) 
			i++;
		else  
			break;
	}

	return ip;
}
