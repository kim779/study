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
#define IS_WITHINx(min,max,expr)        (((min)<(expr))&&((max)>(expr)))
#define IS_WITHOUTx(min,max,expr)       (((min)>=(expr))||((max)<=(expr)))

#ifndef SAFE_WAIT
#define SAFE_WAIT(size, parr) while (size != parr->GetSize()){ size = parr->GetSize(); Sleep(1); }
#endif

#include <execution>
#include <vector>
#include <map>
#include <unordered_map>
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
#include <sal.h>
#include <VersionHelpers.h>
#include <axdll.hpp>
#include <set>
#include <any>
#include <array>
#include <type_traits>
#include <concurrent_vector.h>
#include <filesystem>



#pragma comment(lib, "crypt32.lib")
#pragma comment(lib, "advapi32")
#include <wincrypt.h>

using namespace std;
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
#pragma warning(disable : 26482)
#pragma warning(disable : 26446)
#pragma warning(disable : 26461)
#pragma warning(disable : 26429)
#pragma warning(disable : 4996)

typedef BOOL(WINAPI* RtlGetVersion_FUNC) (OSVERSIONINFOEXW*);
static  ULONGLONG g_begin;


namespace AxStd
{
#pragma warning(disable : 26492)
#pragma warning(disable : 26477)
	inline void AFXAPI _Msg(const TCHAR* fmt = _T(""), ...)
	{
		CString	tmpstr, str;

		va_list	valist;
		va_start(valist, fmt);
		tmpstr.FormatV(fmt, valist);
		va_end(valist);
		const ULONGLONG end = GetTickCount64();

		std::async(launch::deferred, [&](){	
			if (tmpstr == _T(""))
			{
				str = CString(_T("<Debug>")) + CString(_LINE);
				g_begin = end;
			}
			else
			{
				//const CTime time = CTime::GetCurrentTime();
				CString sTime;

				sTime.Format(_T("[%lld ms]"), gsl::narrow_cast<LONG64>(end - g_begin));
				str = _T("<Debug>") + sTime + tmpstr;
			}

			if ((end - g_begin) > 10)
				OutputDebugString(str + _T("\n"));
		
			/*
				ofstream dFile(_T("c:\\Debug.log"), std::ios::app);
				dFile << str << endl;
				dFile.close();
			*/
		}).get();
	};

	template <class _Fty, class... _Types>
	void time_call(_Fty&& func, _Types&&... args)
	{
		INT64 start = GetTickCount64();
		func(_STD forward<_Types>(args)...);        
		_Msg("Elapsed time: [%ld] milliseconds", GetTickCount64() - start);
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

	// inline CString AFXAPI FORMAT(const WCHAR* formatString, ...)
	// {
	// 	CStringW s;
	// 	va_list	valist;

	// 	va_start(valist, formatString);
	// 	s.FormatV(formatString, valist);
	// 	va_end(valist);
	// 	return s;
	// };

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

	inline int SplitSub(vector<CString> &elems, CString& srcstr, CString substr)
	{
		while (!srcstr.IsEmpty())
			elems.push_back(Parser(srcstr, substr));

		return elems.size();
	}

	inline int Split(vector<CString> &elems, const wstring &s, WCHAR delim)
	{
		wstringstream ss(s);
		wstring item;
		while (getline(ss, item, delim))
			elems.push_back(CString(item.c_str()));

		return elems.size();
	}

	inline void xxCopy(gsl::span<char> org, CString src)
	{
		CopyMemory(org.data(), src.GetString(), min(org.size(), src.GetLength()));
	}

	inline BOOL GetVersion(OSVERSIONINFOEX* os) {
		HMODULE hMod{};
		RtlGetVersion_FUNC func{};

		OSVERSIONINFOEXW o{};
		OSVERSIONINFOEXW* osw = &o;

		hMod = LoadLibrary(TEXT("ntdll.dll"));
		if (hMod)
		{
			func = (RtlGetVersion_FUNC)GetProcAddress(hMod, "RtlGetVersion");
			if (func == 0)
			{
				FreeLibrary(hMod);
				return FALSE;
			}
			ZeroMemory(osw, sizeof(*osw));
			osw->dwOSVersionInfoSize = sizeof(*osw);
			func(osw);

			os->dwBuildNumber = osw->dwBuildNumber;
			os->dwMajorVersion = osw->dwMajorVersion;
			os->dwMinorVersion = osw->dwMinorVersion;
			os->dwPlatformId = osw->dwPlatformId;
			os->dwOSVersionInfoSize = sizeof(*os);
		}
		else
			return FALSE;
		FreeLibrary(hMod);
		return  TRUE;
	}

	inline BOOL isDirExist(const CString& path)
	{
		return PathIsDirectory(path);
	}
	
	inline BOOL isFileExist(const CString& path)
	{
		return PathFileExists(path);
	}
	
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
	
	static CString getErrMsg()
	{	
		// OSVERSIONINFOEXA   VersionInformation{};
		// VersionInformation.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		// VersionInformation.dwMajorVersion = 6;
		// VersionInformation.dwMinorVersion = 2;
		// DWORD              dwTypeMask = VER_MAJORVERSION | VER_MINORVERSION;
		// DWORDLONG          dwlConditionMask = 0;
		
		// VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
		// VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
		
		// if(!VerifyVersionInfo(&VersionInformation, dwTypeMask, dwlConditionMask))
		// {
		// 	MessageBoxs(윈도우8을 지원하지 않습니다.);
		// }


		CString sMsg;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), sMsg.GetBuffer(256), 0, nullptr);
		sMsg.ReleaseBuffer();
		sMsg.TrimRight(_T(" \t\r\n"));
		return sMsg;
	}

	inline void addComma(CString &data)
	{
		CString tmps, sRight;
		int pos{}, length{};

		sRight = data;
		pos  = sRight.Find('.') + 1;
		tmps = Parser(sRight, _T("."));
		length = tmps.GetLength();
		if (length < 4)
			return;
			
		data.Empty();
		for (int ii = 0; ii < length; ii++)
		{
			if ((length - ii) % 3 == 0 && ii != 0)
				data += ',';
			data += tmps.GetAt(ii);
		}

		if (!sRight.IsEmpty())
			data += _T(".") + sRight;
	}
	
	// type : 0 는 코드 내림차순
	//      : 1 은 이름 내림차순
	inline void getArray(vector<std::pair<CString, CString>>& vArr, int type)
	{
		DllInvoker codedll("AxisCode.dll");	
		if (codedll.IsLoaded())
		{
			auto func = codedll.Function<char* WINAPI(int)>("getArray");
			if (func)
			{
				std::map<CString, std::pair<CString, int>>* map =
					(std::map<CString, std::pair<CString, int>>*) func(0);

				for_each(map->begin(), map->end(), [&vArr](const auto item) {
					vArr.emplace_back(std::make_pair(item.first, item.second.first));
					});

				if (type == 1)
				{
					std::sort(vArr.begin(), vArr.end(), [](const auto item1, const auto item2) {
						return (item1.second.Compare(item2.second) < 0);
					});
				}
			}
		}
	}

	inline CString getName(CString sCode)
	{
		DllInvoker codedll("AxisCode.dll");
		if (codedll.IsLoaded())
		{
			auto func = codedll.Function<const char* WINAPI(const char*)>("getName");
			if (func)
				return (char*)func(sCode.GetString());
		}
		return "";
	}

	inline bool IsGetNxt(CString sCode)
	{
		DllInvoker codedll("AxisCode.dll");
		if (codedll.IsLoaded())
		{
			auto func = codedll.Function<const bool WINAPI(const char*)>("IsNxt");
			if (func)
				return func(sCode.GetString());
		}
		return false;
	}

	inline CString getCode(CString sName)
	{
		DllInvoker codedll("AxisCode.dll");
		if (codedll.IsLoaded())
		{
			auto func = codedll.Function<const char* WINAPI(const char*)>("getCode");
			if (func)
				return (char*)func(sName.GetString());
		}
		return "";
	}

	// #include <atlstr.h>
	// wstring sUni    =  CA2W("멀티바이트 -> 유니코드");
	// string  sMulti  =  CW2A("유니코드 -> 멀티바이트");
	// string  strUTF8 =  CW2A("유니코드 -> UTF8", CP_UTF8); 

	struct AxDeleter
	{
		template<typename T>
		void operator () (T* p) noexcept
		{
			delete[] p;
		}
	};

	// 안전하게 멤버함수 호출
	template<typename T, typename _Fty, typename... _ArgTypes>
	std::invoke_result_t<_Fty, T*, _ArgTypes...> safe_call(T* obj, _Fty func, _ArgTypes&&... args)
	{
		if (obj && obj->GetSafeHwnd())
			return (obj->*func)(std::forward<_ArgTypes>(args)...);
	}

	template <class _Fty, class... _ArgTypes>
	_NODISCARD future<_Invoke_result_t<decay_t<_Fty>, decay_t<_ArgTypes>...>> async(_Fty&& _Fnarg, _ArgTypes&&... _Args) {
	// manages a callable object launched with default policy
		return _STD async(launch::async, _STD forward<_Fty>(_Fnarg), _STD forward<_ArgTypes>(_Args)...);
	}

namespace AxPool {
	
	template <int N>
	class AxThreadPool {
	public:
		AxThreadPool<N>() : _stop(false) {
			static_assert(N > 0,    "N must be greater than 0");
			static_assert(N < 1001, "N must be less than 1001");	
			for (size_t ii = 0; ii < N; ++ii)
				_vThreads[ii] = std::make_unique<std::thread>([this]() { run(); });
		}
		~AxThreadPool()
		{
			_stop = true;
			_cond.notify_all();
			for (auto& t : _vThreads)
				t->join();
		}

		void Stop() {
			_stop = true;
			_cond.notify_all();
		}

		int getrunningCount() const {
			return gsl::narrow_cast<int>(_info.size());
		}

		template <class _Fty, class... _ArgTypes>
		_NODISCARD std::future<std::_Invoke_result_t<decay_t<_Fty>, decay_t<_ArgTypes>...>> addJob(_Fty&& _Fnarg, _ArgTypes&&... _Args)
		{
			// manages a callable object launched with supplied policy
			using _Ret = std::_Invoke_result_t<std::decay_t<_Fty>, std::decay_t<_ArgTypes>...>;
			auto ptask = std::make_shared<std::packaged_task<_Ret()>>(std::_Fake_no_copy_callable_adapter<_Fty, _ArgTypes...>(_STD forward<_Fty>(_Fnarg), _STD forward<_ArgTypes>(_Args)...));

			{
				std::lock_guard<std::mutex> lock(_xxx);
				_qJobs.emplace([ptask]() {
					(*ptask)();
				});
			}
			_cond.notify_one();
			return ptask->get_future();
		}

	private:
		bool _stop;
		std::array<std::unique_ptr<std::thread>, N> _vThreads;
		std::queue<std::function<void()>> _qJobs;
		std::condition_variable _cond;
		std::mutex _xxx;
		std::set<UINT64> _info;;


		void run()
		{
			while (true) {
				unique_lock xxx_lock(_xxx);
				_info.erase(GetCurrentThreadId());
				_cond.wait(xxx_lock, [this]() { 		
					return !this->_qJobs.empty() || _stop; 
				});
				if (_stop && _qJobs.empty()) {
					xxx_lock.unlock();
					return;
				}
				if (_qJobs.empty()) {
					xxx_lock.unlock();
					continue;
				}
					
				_info.insert(GetCurrentThreadId());				
				function<void()> job = std::move(_qJobs.front());
				_qJobs.pop();
				xxx_lock.unlock();
				job();
				
			}
		}
	};

	class AxMemoryPool 
	{
	private:
		std::queue<std::unique_ptr<char[]>> pool;
		std::mutex poolMutex;
	
	public:
		std::unique_ptr<char[]> Acquire(size_t size) {
			std::lock_guard<std::mutex> lock(poolMutex);
			if (!pool.empty() /*&& /* 크기 검사 */) {
				auto ptr = std::move(pool.front());
				pool.pop();
				return ptr;
			}
			return std::make_unique<char[]>(size);
		}
	
		void Release(std::unique_ptr<char[]> ptr) {
			std::lock_guard<std::mutex> lock(poolMutex);
			pool.push(std::move(ptr));
		}
	};


}  // namespace ThreadPool

//encription!!!!  AES256
#ifndef DF_ENCAES
#define DF_ENCAES
inline CString Base64Encode(const BYTE* pData, DWORD dwDataLen)
{
	static const char* base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	CString base64String;
	int padding = 0;

	for (DWORD i = 0; i < dwDataLen; i += 3)
	{
		DWORD n = pData[i] << 16;

		if (i + 1 < dwDataLen)
		{
			n |= pData[i + 1] << 8;
		}
		else
		{
			padding++;
		}

		if (i + 2 < dwDataLen)
		{
			n |= pData[i + 2];
		}
		else
		{
			padding++;
		}

		base64String.AppendChar(base64Chars[(n >> 18) & 63]);
		base64String.AppendChar(base64Chars[(n >> 12) & 63]);

		if (padding < 2) base64String.AppendChar(base64Chars[(n >> 6) & 63]);
		else base64String.AppendChar('=');

		if (padding < 1) base64String.AppendChar(base64Chars[n & 63]);
		else base64String.AppendChar('=');
	}

	return base64String;
}

inline bool Base64Decode(const CString& base64String, BYTE** ppData, DWORD& outLen)
{
	static const int decodingTable[256] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
		-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
		-1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	};

	// Base64 문자열 길이 확인
	int inputLen = base64String.GetLength();
	if (inputLen % 4 != 0) return false; // Base64는 항상 4의 배수 길이여야 함

	// 패딩 확인 ('=' 문자의 개수)
	int padding = 0;
	if (inputLen >= 2 && base64String[inputLen - 1] == '=')
		padding++;
	if (inputLen >= 3 && base64String[inputLen - 2] == '=')
		padding++;

	// 디코딩된 데이터 길이 계산
	outLen = (inputLen / 4) * 3 - padding;
	*ppData = new BYTE[outLen];

	int buffer = 0;
	int bufferBits = 0;
	int dataIndex = 0;

	for (int i = 0; i < inputLen; i++)
	{
		int decoded = decodingTable[(unsigned char)base64String[i]];
		if (decoded == -1) continue; // 유효하지 않은 Base64 문자 무시

		buffer = (buffer << 6) | decoded;
		bufferBits += 6;

		if (bufferBits >= 8)
		{
			bufferBits -= 8;
			(*ppData)[dataIndex++] = (BYTE)((buffer >> bufferBits) & 0xFF);
		}
	}

	return true;
}

inline BOOL APIENTRY axENCAES(char* src, CString& strENC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HCRYPTPROV hCryptProv{};
	HCRYPTHASH hHash{};
	HCRYPTKEY hKey{};
	CString m_slog{};

	if (CryptAcquireContext(&hCryptProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0) == FALSE) {
		m_slog.Format("[CX_SecureDataEngine][ENC] CryptAcquireContext error =  %x", GetLastError());
		OutputDebugString(m_slog);

		if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_NEWKEYSET) == FALSE) {
			m_slog.Format("[CX_SecureDataEngine][ENC] CryptAcquireContext error  %x", GetLastError());
			OutputDebugString(m_slog);
			return FALSE;
		}
	}

	const DWORD AES_KEY_LENGTH = 16;
	struct {
		BLOBHEADER hdr;
		DWORD cbKeySize;
		BYTE rgbKeyData[AES_KEY_LENGTH];
	} keyBlob;

	keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
	keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
	keyBlob.hdr.reserved = 0;
	keyBlob.hdr.aiKeyAlg = CALG_AES_128;
	keyBlob.cbKeySize = AES_KEY_LENGTH;
	CopyMemory(keyBlob.rgbKeyData, "", AES_KEY_LENGTH);

	if (CryptImportKey(hCryptProv, (BYTE*)&keyBlob, sizeof(keyBlob), 0, 0, &hKey) == FALSE) {
		m_slog.Format("[CX_SecureDataEngine][ENC]   CryptImportKey error ");
		OutputDebugString(m_slog);
		//AfxMessageBox(m_slog);
		return FALSE;
	}

	DWORD dwMode = CRYPT_MODE_ECB;
	if (CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&dwMode, 0) == FALSE) {
		m_slog.Format("[CX_SecureDataEngine][ENC]   CryptSetKeyParam1 error ");
		OutputDebugString(m_slog);
		//AfxMessageBox(m_slog);
		return FALSE;
	}

	dwMode = PKCS5_PADDING;
	if (CryptSetKeyParam(hKey, KP_PADDING, (BYTE*)&dwMode, 0) == FALSE) {
		m_slog.Format("[CX_SecureDataEngine][ENC]  CryptSetKeyParam2 error ");
		OutputDebugString(m_slog);
		//AfxMessageBox(m_slog);
		return FALSE;
	}

	CString plainID;
	plainID.Format("%s", src);

	BYTE pData[128];
	DWORD dwStrLen = plainID.GetLength();
	DWORD dwPadding = 16 - (dwStrLen & 0xF);
	DWORD dwDataLen = dwStrLen;
	DWORD dwBufferLen = dwStrLen + dwPadding;

	memset(pData, 0, sizeof(pData));
	CopyMemory(pData, (LPSTR)(LPCTSTR)plainID, dwStrLen);

	BOOL result = CryptEncrypt(hKey, 0, TRUE, 0, pData, &dwDataLen, dwBufferLen);
	if (!result) {
		OutputDebugString(_T("[CX_SecureDataEngine][ENC]CryptEncrypt 실패!"));
		return FALSE;
	}

	// Base64 인코딩
	strENC = Base64Encode((BYTE*)pData, dwBufferLen);
	CString debugMsg;
	debugMsg.Format(_T("\r\n[CX_SecureDataEngine][ENC]  ---------암호화성공-----------------------------[%d][%s]"), strENC.GetLength(), strENC);
	OutputDebugString(debugMsg);


	return TRUE;
}

inline BOOL APIENTRY axDECAES(char* sEncBase64, CString& sDEC)
{
	HCRYPTPROV hCryptProv{};
	HCRYPTHASH hHash{};
	HCRYPTKEY hKey{};
	CString m_slog{};

	sDEC.Empty();
	m_slog.Format("[CX_SecureDataEngine][DEC]  axDECAES start   len= [%d]  sEncBase64=[%s]", strlen(sEncBase64), sEncBase64);
	OutputDebugString(m_slog);

	if (CryptAcquireContext(&hCryptProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0) == FALSE) {
		m_slog.Format("[CX_SecureDataEngine][DEC]   %x", GetLastError());
		OutputDebugString(m_slog);
		//AfxMessageBox(m_slog);

		if (CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_AES, CRYPT_NEWKEYSET) == FALSE) {
			m_slog.Format("2 %x", GetLastError());
			OutputDebugString(m_slog);
			m_slog.Format("[CX_SecureDataEngine][DEC]    CryptAcquireContext error ");
			OutputDebugString(m_slog);
			//AfxMessageBox(m_slog);
			return FALSE;
		}
	}

	const DWORD AES_KEY_LENGTH = 16;
	struct {
		BLOBHEADER hdr;
		DWORD cbKeySize;
		BYTE rgbKeyData[AES_KEY_LENGTH];
	} keyBlob;

	keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
	keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
	keyBlob.hdr.reserved = 0;
	keyBlob.hdr.aiKeyAlg = CALG_AES_128;
	keyBlob.cbKeySize = AES_KEY_LENGTH;
	CopyMemory(keyBlob.rgbKeyData, "", AES_KEY_LENGTH);

	if (CryptImportKey(hCryptProv, (BYTE*)&keyBlob, sizeof(keyBlob), 0, 0, &hKey) == FALSE) {
		m_slog.Format("[CX_SecureDataEngine][DEC]  CryptImportKey error ");
		OutputDebugString(m_slog);
		//AfxMessageBox(m_slog);
		return FALSE;
	}

	DWORD dwMode = CRYPT_MODE_ECB;
	if (CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&dwMode, 0) == FALSE) {
		m_slog.Format("[CX_SecureDataEngine][DEC]   CryptSetKeyParam1 error ");
		OutputDebugString(m_slog);
		//AfxMessageBox(m_slog);
		return FALSE;
	}

	dwMode = PKCS5_PADDING;
	if (CryptSetKeyParam(hKey, KP_PADDING, (BYTE*)&dwMode, 0) == FALSE) {
		m_slog.Format("[CX_SecureDataEngine][DEC]  CryptSetKeyParam2 error ");
		OutputDebugString(m_slog);
		//AfxMessageBox(m_slog);
		return FALSE;
	}

	// Base64 디코딩 및 복호화
	BYTE* pDecodedData = nullptr;
	DWORD decodedLen = 0;

	bool bDecodeSuccess = Base64Decode(sEncBase64, &pDecodedData, decodedLen);
	if (!bDecodeSuccess) {
		m_slog.Format("[CX_SecureDataEngine][DEC]   Base64 디코딩 실패!  [%s] len=[%d] decodedLen=[%d]", sEncBase64, strlen(sEncBase64), decodedLen);
		OutputDebugString(m_slog);
		return FALSE;
	}


	// CryptDecrypt를 위한 준비
	DWORD outLen = decodedLen;
	BOOL result = CryptDecrypt(hKey, 0, TRUE, 0, pDecodedData, &outLen);
	if (!result) {
		OutputDebugString(_T("[CX_SecureDataEngine][DEC] CryptDecrypt 실패!"));
		delete[] pDecodedData;
		return FALSE;
	}

	// 복호화된 데이터 출력
	pDecodedData[outLen] = '\0'; // Null-terminate
	sDEC.Format("%s", pDecodedData);

	m_slog.Format(_T("\r\n[CX_SecureDataEngine][DEC] ---------복호화성공-----------------------------[%d][%s] "), sDEC.GetLength(), sDEC);
	OutputDebugString(m_slog);

	return TRUE;
}
#endif //
}

