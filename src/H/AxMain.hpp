// *****************************************************************************
// (C) COPYRIGHT Winix Information Inc. 2000
// All Rights Reserved
// Licensed Materials - Property of WINIX
//
// This program contains proprietary information of Winix Information.
// All embodying confidential information, ideas and expressions can't be
// reproceduced, or transmitted in any form or by any means, electronic,
// mechanical, or otherwise without the written permission of Winix Information.
//  
//  작성자        :  김정식
//  요약          :  표준 라이브러리 사용및 공통함수 정리
//  Components   :   AxStd.hpp	AXIS common 
//  Rev. History :
//  		  Ver	Date	Description
//		-------	-------	------------------------------------------------
//		 01.00	2019-01	Initial version
// *****************************************************************************


#pragma once

//----------------------------------------------------------
// FIXMEs / TODOs / NOTE macros
//----------------------------------------------------------
#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "

#define NOTE( x )   TRACE( x )
#define FILE_LINE   TRACE( __FILE__LINE__ "\n")
#define TODO( x )   TRACE( __FILE__LINE__"\n"                 \
        " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"   \
        "|  TODO :   " #x "\n"					\
        " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"   )  
#define FIXME( x )  TRACE(  __FILE__LINE__"\n"                \
        " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"   \
        "|  FIXME :  " #x "\n"			                \
        " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"  )
#define todo( x )   TRACE( __FILE__LINE__" TODO :   " #x "\n" ) 
#define fixme( x )  TRACE( __FILE__LINE__" FIXME:   " #x "\n" ) 
#define _LINE _T("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------")
#define XMSG( x )  TRACE(  __FILE__LINE__"\n"                 \
        " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"   \
	"|  " __FUNCTION__ "\n"\
	"|  XMSG :  " #x "\n"			                \
        " ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"  )

// 조건문 매크로
#define IS_TRUE(c)              (c)
#define IS_FALSE(c)             (!(c))

#define IS_EXIST(c)             (c)
#define IS_NOT_EXIST(c)         (!(c))

#define IS_VALID(c)             (c)
#define IS_INVALID(c)           (!(c))

#define IS_SUCCEEDED(c)         (c)
#define IS_FAILED(c)            (!(c))

// 포인터 매크로
#define IS_NULL(p)              (!(p))
#define IS_NOT_NULL(p)          (p)

#define IS_ZERO(n)              (!(n))
#define IS_NOT_ZERO(n)          (n)

// 범위 검사 매크로
#define IS_WITHIN(min,max,expr)         (((min)<=(expr))&&((max)>=(expr)))
#define IS_WITHOUT(min,max,expr)        (((min)>(expr))||((max)<(expr)))
#define IS_WITHINx(min,max,expr)         (((min)<(expr))&&((max)>(expr)))
#define IS_WITHOUTx(min,max,expr)        (((min)>=(expr))||((max)<=(expr)))

#define XSTRING(obj) CString(obj, sizeof(obj)).Trim()
// stl string -> CString 
#define CSTRING(t)	CString(t.data(), t.length())
#define MAX_DATA	1024 * 64
#define AxNULL		{ 0, }

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif


#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  if(p){ delete p; p = nullptr;                     }
#define SAFE_DELETEA(p) if(p){ delete[] p; p = nullptr;                   }
#define SAFE_DELETEW(p) if(p){ p->DestroyWindow(); }
#define SAFE_RELEASE(p) if(p){ p->Release(); p = nullptr; }
#endif

// 최근 C++ 에서 사용을 자제 하길 요청...
// 명시적일 경우에만... 사용
template <class T, class U>
T ax_cast(U && u) noexcept
{
	return reinterpret_cast<T>(std::forward<U>(u));
}


#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <Shlwapi.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <nb30.h>
#include <Iphlpapi.h>
#include <locale>
#include <codecvt>
#include <memory>
#include <string>
#include <functional>
#include <stack>
#include <gsl/gsl>
#include <mutex>
#include <thread>
#include <any>
#include <optional>
#include <variant>
#include "ppl.h"
#include <random>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <future>
#include <queue>
#include <thread>
#include <VersionHelpers.h>
#include <execution>

//using namespace std;
using namespace concurrency;

#pragma warning(disable : 26814)	// gap을 컴파일 시간 계산
#pragma warning(disable : 4003)		// 매크로호출 인수 부족
#pragma warning(disable : 26486)        // 함수에 잘못된 포인터 전달
#pragma warning(disable : 26489)        // 잘못된 포인터 반환
#pragma warning(disable : 4003)	        // 매크로 호출시 인수 부족
#pragma warning(disable : 26434)	// 버추얼 함수가 부모함수를 숨김
#pragma warning(disable : 26490)	// reinterpret_cast 를 사용하지 말것
#pragma warning(disable : 26485)	// 배열에서 포인터로 감소 (CString(char[15])) 사용할때 경고 발생
#pragma warning(disable : 26487)	// 잘못된 포인터를 반환하지 마세요.
#pragma warning(disable : 26481)	// 포인터 연산대신  span을 사용하세요.
#pragma warning(disable : 26812)	// enum  대신 enum 클래스를 사용하세요.
#pragma warning(disable : 26433)	// override 표시
#pragma warning(disable : 26493)	// c 스타일 캐스트를 사용하지 마세요.
#pragma warning(disable : 6284)		// format 사용시...
#pragma warning(disable : 26444)	// 파라미터로 사라지는 객체 사용금지
#pragma warning(disable : 6340)		// format 사용시...
#pragma warning(disable : 26414)	// 기존함수가 virtual 함수를 가립니다.
#pragma warning(disable : 6031)		// 반환값이 무시되었습니다.
#pragma warning(disable : 26440)	// virtual funciton
#pragma warning(disable : 6258)		// TerminateThread를 사용하면 스레드가 제대로 정리되지 않습니다.
#pragma warning(disable : 28251)
#pragma warning(disable : 26426)
#pragma warning(disable : 26455)
#pragma warning(disable : 26451)
#pragma warning(disable : 26454)
#pragma warning(disable : 4996)
#pragma warning(disable : 26446)	// gsl::at
#pragma warning(disable : 26482)	// 배열 범위 체크
#pragma warning(disable : 26461)
#pragma warning(disable : 26462)
#pragma warning(disable : 26429)
//#pragma warning(disable : 26402)

namespace AxStd
{
	typedef enum
	{
		WIN_XP, WIN_VISTA, WIN7, WIN8, WIN10, UNKNOWN
	} PLATFORM;

	template <class Function>
	__int64 time_call(Function&& f)
	{	
		__int64 begin = ::GetTickCount64();
		f();
		return ::GetTickCount64() - begin;
	}

	template<typename T>
        class Singleton
	{
	protected:
		Singleton() = default;
		Singleton(const Singleton<T>& src) = delete;
		Singleton<T>& operator=(const Singleton<T>& rhs) = delete;
	private:
		static std::unique_ptr<T> _instance;
		static std::once_flag     _once;
	public:
#pragma warning(disable:26409)
		static T& Instance()
		{
			std::call_once(_once, [] {
				_instance.reset(new T());
			});
			return *_instance.get();
		}
#pragma warning(default:26409)
		static T& get()
		{
			return Instance();
		}
	};

	template<typename T>
	std::unique_ptr<T> Singleton<T>::_instance;
	template<typename T>
	std::once_flag Singleton<T>::_once;


#pragma warning(disable : 26492)
#pragma warning(disable : 26477)

	inline void AFXAPI _Msg(TCHAR* fmt = _T(""), ...)
	{
#ifdef _DEBUG
		CString	tmpstr, str;

		va_list	valist;
		va_start(valist, fmt);
		tmpstr.FormatV(fmt, valist);
		va_end(valist);

		if (tmpstr == _T(""))
		{
			str = _LINE;
		}
		else
		{
			const CTime time = CTime::GetCurrentTime();
			CString sTime;
			const clock_t end = clock();;
			
			
//			sTime.Format(_T("[%02d%02d-%02d:%02d:%02d] "), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
//			sTime.Format(_T("[%ld] "), end - g_begin);
//			g_begin = end;
			str = _T("<Debug> ") + tmpstr;
		}
		OutputDebugString(str + _T("\n"));
		/*
			ofstream dFile(_T("c:\\Debug.log"), std::ios::app);
			dFile << str << endl;
			dFile.close();
		*/
#endif
	};

	inline void AFXAPI _Msg(CString sMsg)
	{
		_Msg("%s", sMsg);
	}
	
	inline CString AFXAPI FORMAT(const TCHAR* formatString, ...)
	{
		CString s;
		va_list	valist;

		va_start(valist, formatString);
		s.FormatV(formatString, valist);
		va_end(valist);
		return s;
	};

#pragma warning(default : 26492)
#pragma warning(default : 26477)
	inline CString Parser(CString &src, LPCTSTR sub = _T("\t"))
	{
		CString tmps;
		const int pos = src.Find(sub);
		
		if (pos == -1)
		{
			tmps = src;
			src.Empty();
			return tmps;
		}

		tmps = src.Left(pos);
		src = src.Mid(pos + _tcslen(sub));
		return tmps;
	}

	inline int SplitSub(std::vector<CString> &elems, CString& srcstr, CString substr)
	{
		while (!srcstr.IsEmpty())
			elems.emplace_back(Parser(srcstr, substr));

		return elems.size();
	}

	inline int Split(std::vector<CString> &elems, const CString &s, TCHAR delim)
	{
		std::stringstream ss(s.GetString());
		std::string item;
		while (getline(ss, item, delim))
			elems.emplace_back(CSTRING(item));

		return elems.size();
	}

	inline int GetMacAddr(const char* ipaddr, gsl::not_null<char*> data)
	{
		int returnL = 12;
		CString	ip = _T("");
		CString ip2 = _T("");
		CString	checkIP;
		FillMemory(data, returnL, ' '); data.get()[returnL] = 0x00;
		ip.Format(_T("%s"), CString(ipaddr));
		ip.TrimLeft(), ip.TrimRight();

		int n = ip.ReverseFind('.');
		ip = ip.Left(n);

		n = ip.ReverseFind('.');
		ip2 = ip.Left(n);

		IP_ADAPTER_INFO AdapterInfo[16]{};
		DWORD dwBufLen = sizeof(AdapterInfo);
		const DWORD dwStatus = GetAdaptersInfo(AdapterInfo, &dwBufLen);
		if (dwStatus == ERROR_SUCCESS)
		{
			for (int ii = 0; ii < 16; ii++)
			{
				checkIP = (((gsl::at(AdapterInfo, ii)).IpAddressList).IpAddress).String;
				checkIP.TrimLeft(), checkIP.TrimRight();
				if (false && checkIP.Find(ip) == -1 && checkIP.Find(ip2) == -1)
					continue;


				sprintf_s(data, returnL, "%.2X%.2X%.2X%.2X%.2X%.2X",
					gsl::at(AdapterInfo, ii).Address[0],
					gsl::at(AdapterInfo, ii).Address[1],
					gsl::at(AdapterInfo, ii).Address[2],
					gsl::at(AdapterInfo, ii).Address[3],
					gsl::at(AdapterInfo, ii).Address[4],
					gsl::at(AdapterInfo, ii).Address[5]);
				break;
			}
		}
		else
		{
			sprintf_s(data, returnL, "NOT FOUND   ");
		}

		return returnL;
	}

	//inline CString Utf8toWide(const char* pBuf, int len)
	//{
	//	CString sData;
	//	len = ::MultiByteToWideChar(CP_UTF8, 0, pBuf, len, nullptr, 0);
	//	::MultiByteToWideChar(CP_UTF8, 0, pBuf, -1, sData.GetBuffer(len), len);
	//	sData.ReleaseBuffer();
	//	return sData; 
	//};

	//// Return  Data 길이 / 0을 반환시 오류  버퍼 사이즈 확인 바람.
	//inline int WidetoAnsi(CString sIn, char* pOut, int len) noexcept
	//{
	//	return WideCharToMultiByte(CP_ACP, 0, sIn, -1, pOut, len, nullptr, nullptr);
	//};

	//// Return  Data 길이 / 0을 반환시 오류  버퍼 사이즈 확인 바람.
	//inline int WidetoUTF8(CString sIn, char* pOut, int len) noexcept
	//{
	//	return WideCharToMultiByte(CP_UTF8, 0, sIn, -1, pOut, len, nullptr, nullptr);
	//};

	//// 사용해 본적이 없음 검증 필요
	//inline CString AnsitoWide(const char* pBuf, int len)
	//{
	//	CString sData;
	//	len = ::MultiByteToWideChar(CP_UTF8, 0, pBuf, len, nullptr, 0);
	//	::MultiByteToWideChar(CP_ACP, 0, pBuf, -1, sData.GetBuffer(len), len);
	//	sData.ReleaseBuffer();
	//	return sData;
	//};

	//// Return  Data 길이 / 0을 반환시 오류  버퍼 사이즈 확인 바람.
	//inline int AnsiToUTF8(CString InputStr, char* pbuffer, int len) noexcept
	//{
	//	return WideCharToMultiByte(CP_UTF8, 0, InputStr, InputStr.GetLength(), pbuffer, len, nullptr, nullptr);
	//}

	/*
	inline bool isDirExist(const CString& path)
	{
		// PathIsDirectory() <-- this use
		DWORD attr = GetFileAttributes(path);
		return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
	}
	

	inline bool isFileExist(const CString& path)
	{
		// PathFileExists() <-- this use
		DWORD attr = GetFileAttributes(path);
		return (attr != INVALID_FILE_ATTRIBUTES);
	}
	*/


	inline int AFXAPI SplitString(CStringArray& arr, LPCTSTR lpszFullString, LPCTSTR lpszSep)
	{
		if (lpszFullString == nullptr)     return FALSE;
		CString str;
		const int SepLen = lstrlen(lpszSep);
		LPCTSTR lpchEnd = lpszFullString;
		while (lpchEnd != nullptr)
		{
			lpchEnd = _tcsstr(lpszFullString, lpszSep);
			const int nLen = (lpchEnd == nullptr) ? lstrlen(lpszFullString) : (int)(lpchEnd - lpszFullString);
			ASSERT(nLen >= 0);
			str = CString(lpszFullString, nLen);
			lpszFullString += SepLen + nLen;       // point past the separator
			arr.Add(str);
		}
		return arr.GetSize();
	};

	
	/// 특정 윈도우 폴더 fullpath
	inline CString getWinPath(REFKNOWNFOLDERID rfid)	// Knownsfolders.h 참조  FOLDERID_RoamingAppData
	{
		CString sPath;
		PWSTR path;
		const HRESULT hr = SHGetKnownFolderPath(rfid , 0, nullptr, &path);
		if (SUCCEEDED(hr)) {
			sPath = path;
			CoTaskMemFree(path);	
		}
		return sPath;
	}
	
	static int DPI_x(int x)
	{
		int iValue = 0;
		HWND hwnd = CWnd::GetDesktopWindow()->m_hWnd;
		HDC hdc = ::GetDC(hwnd);

		const int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
		::ReleaseDC(hwnd, hdc);
		
		iValue = MulDiv(x, dpiX, 96);
		return iValue;
	}

	static int DPI_y(int y)
	{
		int iValue = 0;
		HWND hwnd = CWnd::GetDesktopWindow()->m_hWnd;
		HDC hdc = ::GetDC(hwnd);
		const int dpiY = GetDeviceCaps(hdc, LOGPIXELSY);
		::ReleaseDC(hwnd, hdc);
		
		iValue = MulDiv(y, dpiY, 96);	
		return iValue;
	}

	static int DPI_Rx(int x)
	{
		double dValue = 0;
		int irate = 0;
		HWND hwnd = CWnd::GetDesktopWindow()->m_hWnd;
		HDC hdc = ::GetDC(hwnd);
		
		const int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
		::ReleaseDC(hwnd, hdc);
		
		irate = dpiX * 100 / 96;
		dValue = x * 100 / irate;
		return (int)dValue;
	}

	struct AxDeleter
	{
		template<typename T>
		void operator () (T* p) noexcept
		{
			delete[] p;
		}
	};

	static CString getErrMsg()
	{
		CString sMsg;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), sMsg.GetBuffer(256), 0, nullptr);
		sMsg.ReleaseBuffer();
		sMsg.TrimRight(_T(" \t\r\n"));
		return sMsg;
	}

	static PLATFORM GetPlatform()
	{	
		if (IsWindows10OrGreater())
			return UNKNOWN;
		if (IsWindows8OrGreater())
			return WIN10;
		if (IsWindows7OrGreater())
			return WIN8;
		if (IsWindowsVistaOrGreater())
			return WIN7;
		if (IsWindowsXPSP3OrGreater())
			return WIN_VISTA;
		if (IsWindowsXPOrGreater())
			return WIN_XP;
		return UNKNOWN;
	}



	namespace base64{
		#define CRYPT_STRING_BASE64HEADER		0x00000000
		#define CRYPT_STRING_BASE64			0x00000001
		#define CRYPT_STRING_BINARY			0x00000002
		#define CRYPT_STRING_BASE64REQUESTHEADER	0x00000003
		#define CRYPT_STRING_HEX			0x00000004
		#define CRYPT_STRING_HEXASCII			0x00000005
		#define CRYPT_STRING_BASE64_ANY			0x00000006
		#define CRYPT_STRING_ANY			0x00000007

		#define CRYPT_STRING_HEX_ANY			0x00000008
		#define CRYPT_STRING_BASE64X509CRLHEADER	0x00000009
		#define CRYPT_STRING_HEXADDR			0x0000000a
		#define CRYPT_STRING_HEXASCIIADDR		0x0000000b
		#define CRYPT_STRING_HEXRAW			0x0000000c
		#define CRYPT_STRING_STRICT			0x20000000

		#define CRYPT_STRING_NOCRLF			0x40000000
		#define CRYPT_STRING_NOCR			0x80000000



		static const char MimeBase64[] = {
				'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
				'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
				'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
				'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
				'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
				'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
				'w', 'x', 'y', 'z', '0', '1', '2', '3',
				'4', '5', '6', '7', '8', '9', '+', '/'
		};

		static int DecodeMimeBase64[256] = {
				-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 00-0F */
				-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 10-1F */
				-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,  /* 20-2F */
				52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,  /* 30-3F */
				-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  /* 40-4F */
				15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,  /* 50-5F */
				-1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  /* 60-6F */
				41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,  /* 70-7F */
				-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 80-8F */
				-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 90-9F */
				-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* A0-AF */
				-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* B0-BF */
				-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* C0-CF */
				-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* D0-DF */
				-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* E0-EF */
				-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1   /* F0-FF */
		};

		typedef union{
			struct{
				unsigned char c1,c2,c3;
			};
			struct{
				unsigned int e1:6,e2:6,e3:6,e4:6;
			};
		} BF;

		static int  ib_chk_endian() {
			const int x = 1;
			if(*(char *)&x == 1) /* 하위 비트 부터 값이 채워짐 */
				return 0;
			else
				return 1;
		}

		static int ib_base64e(gsl::not_null<const char*> src, gsl::not_null<char*> result, int length)
		{
			int i{}, j = 0;
			BF temp{};
			
			if(ib_chk_endian() == 0){ // little endian(intel)
				for(i = 0 ; i < length ; i = i+3, j = j+4){
					temp.c3 = src.get()[i];
					if((i+1) >= length) temp.c2 = 0x00;
					else temp.c2 = src.get()[i+1];
					if((i+2) >= length) temp.c1 = 0x00;
					else temp.c1 = src.get()[i+2];
					
					result.get()[j]   = gsl::at(MimeBase64, temp.e4);
					result.get()[j+1] = gsl::at(MimeBase64, temp.e3);
					result.get()[j+2] = gsl::at(MimeBase64, temp.e2);
					result.get()[j+3] = gsl::at(MimeBase64, temp.e1);
					
					if((i+1) >= length) result.get()[j+2] = '=';
					if((i+2) >= length) result.get()[j+3] = '=';
				}
			} else { // big endian(sun)
				for(i = 0 ; i < length ; i = i+3, j = j+4){
					temp.c1 = src.get()[i];
					if((i+1) >= length) temp.c2 = 0x00;
					else temp.c2 = src.get()[i+1];
					if((i+2) >= length) temp.c3 = 0x00;
					else temp.c3 = src.get()[i+2];
					
					result.get()[j]   = gsl::at(MimeBase64, temp.e4);
					result.get()[j+1] = gsl::at(MimeBase64, temp.e3);
					result.get()[j+2] = gsl::at(MimeBase64, temp.e2);
					result.get()[j+3] = gsl::at(MimeBase64, temp.e1);
					
					if((i+1) >= length) result.get()[j+2] = '=';
					if((i+2) >= length) result.get()[j+3] = '=';
				}
			}
			
			return j;
		}

		static void ib_base64d(gsl::not_null<const char*>src, gsl::not_null<char*> result, gsl::not_null<int*> length)
		{
			int i{}, j = 0, blank = 0;
			const int src_length = strlen(src);
			BF temp{};
						
			if(ib_chk_endian() == 0){ // little endian(intel)
				for(i = 0 ; i < src_length ; i = i+4, j = j+3){
					temp.e4 = gsl::at(DecodeMimeBase64, src.get()[i]);
					temp.e3 = gsl::at(DecodeMimeBase64, src.get()[i+1]);
					if(src.get()[i+2] == '='){
						temp.e2 = 0x00;
						blank++;
					} else temp.e2 = gsl::at(DecodeMimeBase64, src.get()[i+2]);
					if(src.get()[i+3] == '='){
						temp.e1 = 0x00;
						blank++;
					} else temp.e1 = gsl::at(DecodeMimeBase64, src.get()[i+3]);
					
					result.get()[j]   = temp.c3;
					result.get()[j+1] = temp.c2;
					result.get()[j+2] = temp.c1;
				}
			} else { // big endian(sun)
				for(i = 0 ; i < src_length ; i = i+4, j = j+3){
					temp.e4 = gsl::at(DecodeMimeBase64, src.get()[i]);
					temp.e3 = gsl::at(DecodeMimeBase64, src.get()[i+1]);
					if(src.get()[i+2] == '='){
						temp.e2 = 0x00;
						blank++;
					} else temp.e2 = gsl::at(DecodeMimeBase64, src.get()[i+2]);
					if(src.get()[i+3] == '='){
						temp.e1 = 0x00;
						blank++;
					} else temp.e1 = gsl::at(DecodeMimeBase64, src.get()[i+3]);
					
					result.get()[j]   = temp.c1;
					result.get()[j+1] = temp.c2;
					result.get()[j+2] = temp.c3;
				}
			}
			*length = j-blank;
		}

		static int ToBase64Crypto( const BYTE* pSrc, int nLenSrc, char* pDst, int nLenDst )
		{
			DWORD nLenOut = nLenDst;
			const BOOL fRet= CryptBinaryToString((const BYTE*)pSrc, 
				nLenSrc, 
				CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, 
				pDst, 
				&nLenOut
				);
			if (!fRet) nLenOut=0;  // failed
			return( nLenOut );
		}

		static int FromBase64Crypto( const BYTE* pSrc, int nLenSrc, char* pDst, int nLenDst )
		{
			DWORD nLenOut = nLenDst;
			const BOOL fRet= CryptStringToBinary((LPCSTR)pSrc, 
				nLenSrc, CRYPT_STRING_BASE64, 
				(BYTE*)pDst, 
				&nLenOut, 
				nullptr,        // pdwSkip (not needed)
				nullptr         // pdwFlags (not needed)
				);
			if (!fRet) nLenOut=0;  // failed
			return( nLenOut );
		}
	}
}


namespace AxPool {
	
	template <int N>
	class AxThreadPool {
	public:
		AxThreadPool<N>()
			: _stop(false) {
			//_vThreads.reserve(num);
			for (size_t ii = 0; ii < N; ++ii)
				_vThreads[ii] = thread([this]() { run(); });
		}
		~AxThreadPool()
		{
			_stop = true;
			_cond.notify_all();
			for (auto& t : _vThreads)
				t.join();
		}

		template <class Function, class... Args>
		std::future<std::invoke_result_t<Function, Args...>> addJob(Function && f, Args && ... args)
		{
			if (_stop) {
				throw std::runtime_error("ThreadPool 사용 중지됨");
			}

			using return_type = std::invoke_result_t<Function, Args...>;
			auto job = make_shared<std::packaged_task<return_type()>>(bind(std::forward<F>(f), std::forward<Args>(args)...));
			std::future<return_type> job_result_future = job->get_future();
			{
				std::lock_guard lock(_xxx);
				_qJobs.push([job]() { (*job)(); });
			}
			_cond.notify_one();
			return job_result_future;
		}

	private:
		bool _stop;
		std::array<std::thread, N> _vThreads;
		std::queue<std::function<void()>> _qJobs;
		std::condition_variable _cond;
		std::mutex _xxx;

		void run()
		{
			while (true) {
				unique_lock xxx_lock(_xxx);
				_cond.wait(xxx_lock, [this]() { return !this->_qJobs.empty() || _stop; });
				if (_stop && this->_qJobs.empty()) {
					return;
				}

				function<void()> job = std::move(_qJobs.front());
				_qJobs.pop();
				xxx_lock.unlock();

				job();
			}
		}
	};

}  // namespace ThreadPool





/*
// 사용 예시
int work(int t, int id) {
	printf("%d start \n", id);
	std::this_thread::sleep_for(std::chrono::seconds(t));
	printf("%d end after %ds\n", id, t);
	return t + id;
}

int main() {
	AxThreadPool pool(3);

	std::vector<std::future<int>> futures;
	for (int i = 0; i < 10; i++) {
		futures.emplace_back(pool.EnqueueJob(work, i % 3 + 1, i));
	}
	for (auto& f : futures) {
		printf("result : %d \n", f.get());
	}
}
*/