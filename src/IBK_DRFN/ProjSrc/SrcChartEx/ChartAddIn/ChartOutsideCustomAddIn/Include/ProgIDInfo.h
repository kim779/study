// ProgIDInfo.h: interface for the CProgIDInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROGIDINFO_H__E53E14DB_B03D_4A22_A69C_6322EE4BCAC6__INCLUDED_)
#define AFX_PROGIDINFO_H__E53E14DB_B03D_4A22_A69C_6322EE4BCAC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <objbase.h>	// for CLSIDFromProgID()
#include <afxpriv.h>	// for USES_CONVERSION

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.08.05
//
//	목적	:	ProgID에 대한 정보를 구하는 Object이다.
//
//	내용	:	1. 크지 않은 기능으로 Header Library로 구현한다.
//				2. 특히 Object로 관리되는 것이 없으므로, Static Member Routine을 이용하는 것으로 한다.
///////////////////////////////////////////////////////////////////////////////
class CProgIDInfo : public CObject  
{
public:
	CProgIDInfo();
	virtual ~CProgIDInfo();

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.08.05
//
//	목적	:	ProgID와 ClassID를 변환하는 Routine을 제공한다.
//
//	ERROR	:	[CLSIDFromProgID]	0,	Success, S_OK in ::CLSIDFromProgID()
//									-1,	CO_E_CLASSSTRING in ::CLSIDFromProgID()
//									-2, REGDB_E_WRITEREGDB in ::CLSIDFromProgID()
//									-3, Other Error in ::CLSIDFromProgID()
//				[ProgIDFromCLSID]	0,	Success, S_OK in ::ProgIDFromCLSID()
//									-1,	REGDB_E_CLASSNOTREG in ::ProgIDFromCLSID()
//									-2, REGDB_E_READREGDB in ::ProgIDFromCLSID()
//									-3, Other Error in ::ProgIDFromCLSID()
///////////////////////////////////////////////////////////////////////////////
public:
	static long CLSIDFromProgID( const char *p_szProgID, LPCLSID p_pClsID)
	{
		USES_CONVERSION;
		LPCOLESTR szOleProgID = T2COLE( p_szProgID);
		HRESULT hResult = ::CLSIDFromProgID( szOleProgID, p_pClsID);
		if( hResult == S_OK) return 0;
		else if( hResult == CO_E_CLASSSTRING) return -1;
		else if( hResult == REGDB_E_WRITEREGDB) return -2;
		return -3;
	}
	static long ProgIDFromCLSID( const CLSID p_clsID, CString &p_strProgID)
	{
		LPOLESTR oleProgID;
		HRESULT hResult = ::ProgIDFromCLSID( p_clsID, &oleProgID);
		if( hResult == REGDB_E_CLASSNOTREG) return -1;
		else if( hResult == REGDB_E_READREGDB) return -2;
		else if( hResult != S_OK) return -3;
		USES_CONVERSION;
		p_strProgID = OLE2T( oleProgID);
		CoTaskMemFree( oleProgID);
		return 0;
	}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.08.05
//
//	목적	:	ClassID를 String으로 상호 변환하는 Routine을 제공한다.
//
//	ERROR	:	[StringFromCLSID]	0,	Success, S_OK in ::StringFromCLSID()
//									-1,	Error in ::StringFromCLSID()
//				[CLSIDFromString]	0,	Success, NOERROR in ::CLSIDFromString()
//									-1,	CO_E_CLASSTRING in ::CLSIDFromString()
//									-2, REGDB_E_WRITEREGDB in ::CLSIDFromString()
//									-3, Other Error in ::CLSIDFromString()
///////////////////////////////////////////////////////////////////////////////
public:
	static long StringFromCLSID( const CLSID p_clsID, CString &p_strClsID)
	{
		LPOLESTR oleClsID;
		HRESULT hResult = ::StringFromCLSID( p_clsID, &oleClsID);
		if( hResult != S_OK) return -1;
		USES_CONVERSION;
		p_strClsID = OLE2T( oleClsID);
		CoTaskMemFree( oleClsID);
		return 0;
	}
	static long CLSIDFromString( const char *p_szClsID, LPCLSID p_pClsID)
	{
		USES_CONVERSION;
		LPOLESTR szOleClsID = T2OLE( p_szClsID);
		HRESULT hResult = ::CLSIDFromString( szOleClsID, p_pClsID);
		if( hResult == NOERROR) return 0;
		else if( hResult == CO_E_CLASSSTRING) return -1;
		else if( hResult == REGDB_E_WRITEREGDB) return -2;
		return -3;
	}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.08.05
//
//	목적	:	ClassID String과 ProgID를 상호 변환하는 Routine을 제공한다.
//
//	ERROR	:	[StringCLSIDFromProgID]		0,		Success
//											-X1,	Error in CProgIDInfo::CLSIDFromProgID()
//											-X2,	Error in CProgIDInfo::StringFromCLSID()
//				[ProgIDFromStringCLSID]		0,		Success
//											-X1,	Error in CProgIDInfo::CLSIDFromString()
//											-X2,	Error in CProgIDInfo::ProgIDFromCLSID()
///////////////////////////////////////////////////////////////////////////////
public:
	static long StringCLSIDFromProgID( const char *p_szProgID, CString &p_strClsID)
	{
		// 1. 먼저 CLSID를 구한다.
		CLSID clsID;
		LONG lResult = CLSIDFromProgID( p_szProgID, &clsID);
		if( lResult < 0) return lResult * 10 - 1;
		// 2. String CLSID를 구한다.
		lResult = StringFromCLSID( clsID, p_strClsID);
		if( lResult < 0) return lResult * 10 - 2;
		return 0;
	}
	static long ProgIDFromStringCLSID( const char *p_szClsID, CString &p_strProgID)
	{
		// 1. 먼저 CLSID를 구한다.
		CLSID clsID;
		LONG lResult = CLSIDFromString( p_szClsID, &clsID);
		if( lResult < 0) return lResult * 10 - 1;
		// 2. ProgID를 구한다.
		lResult = ProgIDFromCLSID( clsID, p_strProgID);
		if( lResult < 0) return lResult * 10 - 2;
		return 0;
	}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.08.05
//
//	목적	:	String ClassID로 CLSID에 해당하는 Module의 경로(COM Server Path)를 구한다.
//
//	내용	:	1. ProgID, CLSID로도 구할 수 있도록 지원한다.
//
//	ERROR	:	[ComServerFromStringCLSID]	0,		Success
//											-1,		Error in RegOpenKeyEx() (No Inproc/Local Server Key ?)
//											-2,		No ComServer Value
//											-3,		ComServer Path has No EXT.
//											-4,		There is no ComServer File.
//				[ComServerFromCLSID]		0,		Success
//											-X1,	Error in CProgIDInfo::StringFromCLSID()
//											-X2,	Error in CProgIDInfo::ComServerFromStringCLSID()
//				[ComServerFromProgID]		0,		Success
//											-XX1,	Error in CProgIDInfo::StringCLSIDFromProgID()
//											-X2,	Error in CProgIDInfo::ComServerFromStringCLSID()
///////////////////////////////////////////////////////////////////////////////
public:
	static long ComServerFromStringCLSID( const char *p_szClsID, CString &p_strComServer)
	{
		// 0. Key String을 구한다.
		CString strSubKey;
		char *szSubKey = strSubKey.GetBufferSetLength( 6 + 38 + 15);
		strcpy( szSubKey, "CLSID\\");
		strcpy( szSubKey + 6, p_szClsID);
		strcpy( szSubKey + 6 + 38, "\\InprocServer32");
		// 1. Key를 Open한다.
		HKEY hKey;
		if( RegOpenKeyEx( HKEY_CLASSES_ROOT, szSubKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
		{
			// 1.1 InprocServer32가 아니라면 LocalServer32로 확인한다.
			strcpy( szSubKey + 6 + 38, "\\LocalServer32");
			if( RegOpenKeyEx( HKEY_CLASSES_ROOT, szSubKey, 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
				return -1;
		}
		// 2. ComServer의 경로를 정보를 구한다.(Default Value이다.)
		DWORD dwDataType, dwReceivedSize = _MAX_PATH;
		CString strShortServerPath;
		char *szShortServerPath = strShortServerPath.GetBufferSetLength( dwReceivedSize);
		RegQueryValueEx( hKey, NULL, 0, &dwDataType, ( unsigned char *)szShortServerPath, &dwReceivedSize);
		// 3. Key를 닫는다.
		RegCloseKey( hKey);
		// 4. 구해진 Com Server Path를 확인한다.
		if( !*szShortServerPath) return -2;
		// 5. Module 실행 Option을 제거한다.
		char *szExtension = strrchr( szShortServerPath, '.');
		if( !szExtension) return -3;
		szExtension[ 4] = '\0';
		// 6. Long File Path를 구한다.
		char *szLongServerPath = p_strComServer.GetBuffer( _MAX_PATH);
		DWORD dwLongPath = GetLongPathName( szShortServerPath, szLongServerPath, _MAX_PATH);
		p_strComServer.ReleaseBuffer();
		if( !dwLongPath) return -4;
		// 7. Success를 Return한다.
		return 0;
	}
	static long ComServerFromCLSID( const CLSID p_clsID, CString &p_strComServer)
	{
		// 1. 먼저 String CLSID를 구한다.
		CString strClsID;
		long lResult = StringFromCLSID( p_clsID, strClsID);
		if( lResult < 0) return lResult * 10 - 1;
		// 2. String CLSID로 Com Server를 구한다.
		lResult = ComServerFromStringCLSID( strClsID, p_strComServer);
		if( lResult < 0) return lResult * 10 - 2;
		return 0;
	}
	static long ComServerFromProgID( const char *p_szProgID, CString &p_strComServer)
	{
		// 1. 먼저 String CLSID를 구한다.
		CString strClsID;
		long lResult = StringCLSIDFromProgID( p_szProgID, strClsID);
		if( lResult < 0) return lResult * 10 - 1;
		// 2. String CLSID로 Com Server를 구한다.
		lResult = ComServerFromStringCLSID( strClsID, p_strComServer);
		if( lResult < 0) return lResult * 10 - 2;
		return 0;
	}

///////////////////////////////////////////////////////////////////////////////
//	작업자	:	배승원(Seung-Won, Bae)
//	E-Mail	:	radox@dooriic.co.kr
//	일자	:	2003.08.05
//
//	목적	:	ProgID의 Vender Part만을 이용하여 해당하는 Module의 경로(Com Server Path)를 구한다.
//
//	내용	:	1. ProgID는 <Vender>.<Component>.<Version>으로 구성되어 있다.
//				2. 같은 <Vender>내의 <Component>들은 하나의 Module로 구성된다는 가정하에
//					주어진 <Vender>의 ProgID를 하나 검색하여 (Registry로부터) ComServer를 구하도록 한다.
//
//	ERROR	:	[ProgIDFromVender]			0,		Success
//											-1,		Error in RegQueryInfoKey() (No HKEY_CLASSS_ROOT Key ?)
//											-2,		ProgID of Vender Not Found in HKEY_CLASSS_ROOT
//				[ComServerFromVender]		0,		Success
//											-X1,	Error in CProgIDInfo::ProgIDFromVender()
//											-XXX2,	Error in CProgIDInfo::ComServerFromProgID()
//
//	수정	:	(2003.08.06) HKEY_CLASSS_ROOT내에서 ProgID의 검색을 Sequence Search에서 Binary Search로 변경한다.
//					1. 기본 Variable
//						1.1 B(Base)		: 2진 검색을 진행하기 위한 기본 Index이다.
//											(ex, B=5이면 5이하의 Index에는 검색 대상이 없다는 것이다.)
//						1.2 N(Elements)	: 2진 검색을 진행할 남은 대상 Element의 갯수이다.
//											특히 B(Base) Index의 이상(B 포함) N개의 것을 말한다.
//					2. 기본 Algorithm
//						2.1 기본적인 Algorithm은 전통적인 Binary Search를 기본으로 한다.
//						2.2 그러나 전통적인 방법과같이 B(Base) Index를 2분할 폭으로 이동하면서 비교후 다음 2분할 폭의 방향을 결정하지 않고,
//							상향 2분할 폭을 조정하면서 폭내에 검색대상이 없는 경우에만 B(Base) Index는 상향 이동하는 방법을 택한다.
//						2.3 예를 들어 0 ~ 19까지의 20개 Index가 있는 경우, 12.5를 검색하는 경우를 예로 비교해 보면 다음과 같다.
//										Classical Binary Search			Precheck Binary Search
//							Step 0		B=10, N=10						B=0, N=20
//							Step 1		Compare B (10), B=15, N=4		Compare B+N/2 (10), B=10, N=9
//							Step 2		Compare B (15), B=13, N=2		Compare B+N/2 (15), B=10, N=4
//							Step 3		Compare B (13), B=12, N=1		Compare B+N/2 (13), B=10, N=1
//							Step 4		Compare B (12), N=0, Not Found	Compare B+N/2 (12), B=13, N=0 Not Found
//						2.4 예에서와 같이 B값의 변화를 보면 사전 검사방식은 B가 확인된 경우에만 상향조정되는 방법임을 알 수 있다.
//					3. Algorithm
//						3.1 B=0, N=(Element 개수)
//						3.2 N!=0인 동안 다음을 반복한다.
//							3.2.0 Compare (B + N / 2)
//							3.2.1 =0														-> Found, Exit
//							3.2.2 <0, N=N/2
//								(N은 다음 검색 Element의 갯수로, 2분할 한다.)
//							3.2.3 >0, B=N/2+1, N=N/2-1+(N%2)
//								(B는 다음 검색 대상의 첫 Index를 가리키고,
//								 특히 N은 산출치에서 좀전에 비교한 Element 1개를 제외하고,
//								 또한 이전 N이 홀수인 경우의 2분할 나머지를 추가한다.)
//						3.3 N!=0이면 검색 실패이다.											-> Not Found, Exit
///////////////////////////////////////////////////////////////////////////////
public:
	static long ProgIDFromVender( const char *p_szVender, CString &p_strProgID)
	{
		// 1. Class Root Registry Key를 Open한다.
		//	(HKEY_CLASSES_ROOT의 SubKey를 Handling하는 것으로 이미 HKEY_CLASSES_ROOT는 Open되어 있다.)
		// 2. Sub Key의 길이를 확인한다.
		DWORD dwSubKeyCount, dwSubKeyMaxLen;
		if( RegQueryInfoKey( HKEY_CLASSES_ROOT, NULL, NULL, NULL, &dwSubKeyCount, &dwSubKeyMaxLen, NULL, 
			NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) return -1;
		// 2.1 NULL Character를 감안하여 1를 추가한다.
		dwSubKeyMaxLen++;
		// 3. Sequence Search로 해당 Vender의 ProgID를 검색한다.
		// 3.0 SubKey Name를 받을 Buffer를 확보한다.
		char *szSubKeyName = p_strProgID.GetBuffer( dwSubKeyMaxLen);
		// 3.1 Vender Name을 구한다.
		//	(Full Vender Name이 되도록 '.'를 붙이고, 비교가 용이하도록 대문자로 변환한다.)
		CString strVenderP( p_szVender);	// Vender Name with '.'
		strVenderP += ".";
		strVenderP.MakeUpper();
		// 3.2 각 Sub Key를 구하여 확인한다.
		LONG lResult;
		DWORD dwSubKeySize;
		FILETIME ftLastWriteTime;
// (2003.08.06) HKEY_CLASSS_ROOT내에서 ProgID의 검색을 Sequence Search에서 Binary Search로 변경한다.
//		// 3.2.0 Sequence Search를 진행한다.
//		for( DWORD i = 0, dwResult = ERROR_SUCCESS; dwResult == ERROR_SUCCESS; i++) 
//		{ 
//			// 3.2.1 dwSubKeySize는 I/O 겸용이므로 매번 초기화한다.
//			dwSubKeySize = dwSubKeyMaxLen;
//			// 3.2.2 Sub Key를 구한다.
//			lResult = RegEnumKeyEx( HKEY_CLASSES_ROOT, i, szSubKeyName, &dwSubKeySize, NULL, NULL, NULL, &ftLastWriteTime); 
//			ASSERT( dwSubKeySize < dwSubKeyMaxLen);
//			if( lResult != ERROR_SUCCESS) break;
//			// 3.2.3 이번 차례의 Sub Key를 검색 대상과 비교한다.
//			// 3.2.3.1 대문자의 비교가 되도록, Upper Case로 변환한다.
//			strupr( szSubKeyName);
//			// 3.2.3.2 Key를 비교한다.
//			if( strcmp( strVenderP, szSubKeyName) <= 0) break;
//		}
		// 3.2.0 Binary Search를 진행한다.
		// 3.2.1 B=0, N=(Element 개수)
		int nB = 0, nN = dwSubKeyCount, nCompare, nVenderNameLen = strVenderP.GetLength();
		// 3.2.2 N!=0인 동안 다음을 반복한다.
		while( nN != 0)
		{
			// 3.2.2.0 Compare (B + N / 2)
			// 3.2.2.1 dwSubKeySize는 I/O 겸용이므로 매번 초기화한다.
			dwSubKeySize = dwSubKeyMaxLen;
			// 3.2.2.2 Sub Key를 구한다.
			lResult = RegEnumKeyEx( HKEY_CLASSES_ROOT, nB + nN / 2, szSubKeyName, &dwSubKeySize, NULL, NULL, NULL, &ftLastWriteTime); 
			ASSERT( dwSubKeySize < dwSubKeyMaxLen);
			if( lResult != ERROR_SUCCESS) break;
			// 3.2.2.3 이번 차례의 Sub Key를 검색 대상과 비교한다.
			// 3.2.2.3.1 대문자의 비교가 되도록, Upper Case로 변환한다.
			strupr( szSubKeyName);
			// 3.2.2.3.2 Key를 비교한다.
			nCompare = strncmp( strVenderP, szSubKeyName, nVenderNameLen);
			// 3.2.2.1 =0	-> Found, Exit
			if( nCompare == 0) break;
			// 3.2.2.2 <0, N=N/2
			//	(N은 다음 검색 Element의 갯수로, 2분할 한다.)
			if( nCompare < 0) nN = nN / 2;
			// 3.2.2.3 >0, B=B+N/2+1, N=N/2-1+(N%2)
			//	(B는 다음 검색 대상의 첫 Index를 가리키고,
			//	 특히 N은 산출치에서 좀전에 비교한 Element 1개를 제외하고,
			//	 또한 이전 N이 홀수인 경우의 2분할 나머지를 추가한다.)
			else
			{
				nB = nB + nN / 2 + 1;
				nN = nN / 2 - 1 + ( nN % 2);
			}
// [DEBUG] 검색되고 비교되는 순서를 확인할 수 있도록 MessageBox로 출력하여 본다.
//			CString strTraceMsg;
//			strTraceMsg.Format( "ComparedProgID=%s, Base=%d, N=%d", szSubKeyName, nB, nN);
//			AfxMessageBox( strTraceMsg);
		}
		// 3.3 Class Root Registry Key를 닫는다.
		//	(HKEY_CLASSES_ROOT의 SubKey를 Handling하는 것으로 이미 HKEY_CLASSES_ROOT는 Open되어 있다.)
		// 4. ProgID를 Return한다.
		p_strProgID.ReleaseBuffer();
		// 4.1 검색 실패의 경우를 처리한다.
		if( lResult != ERROR_SUCCESS || strncmp( p_strProgID, strVenderP, strVenderP.GetLength()))
		{
			p_strProgID.Empty();
			return -2;
		}
		// 4.2 검색 성공의 경우를 Return한다.
		return 0;
	}
	static long ComServerFromVender( const char *p_szVender, CString &p_strComServer)
	{
		// 0. 먼저 ProgID를 구한다.
		CString strProgID;
		long lResult = ProgIDFromVender( p_szVender, strProgID);
		if( lResult < 0) return lResult * 10 - 1;
		// 1. ProgID로 Com Server를 구한다.
		lResult = ComServerFromProgID( strProgID, p_strComServer);
		if( lResult < 0) return lResult * 10 - 2;
		return 0;
	}
};

#endif // !defined(AFX_PROGIDINFO_H__E53E14DB_B03D_4A22_A69C_6322EE4BCAC6__INCLUDED_)
