#pragma once

//----------------------------------------------------------
// FIXMEs / TODOs / NOTE macros
//----------------------------------------------------------
#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "

#define NOTE( x )   message( x )
#define FILE_LINE   message( __FILE__LINE__ )
#define TODO( x )   message( __FILE__LINE__"\n"                 \
        " ------------------------------------------------\n"   \
        "|  TODO :   " #x "\n"					\
        " ------------------------------------------------\n"   )  
#define FIXME( x )  message(  __FILE__LINE__"\n"                \
        " ------------------------------------------------\n"   \
        "|  FIXME :  " #x "\n"			                \
        " -------------------------------------------------\n"  )
#define todo( x )   message( __FILE__LINE__" TODO :   " #x "\n" ) 
#define fixme( x )  message( __FILE__LINE__" FIXME:   " #x "\n" ) 


#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETEA(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_DELETEW(p) { if(p) { p->DestroyWindow(); delete (p); (p)=NULL; } }
#define TRIM(s)	s.TrimLeft(); s.TrimRight();


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

// stl string -> CString 
#define CSTRING(t) CString(t.data(), t.length())

#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <Shlwapi.h>
#include <fstream>
#include <iostream>
#include <sstream>

#pragma comment(lib, "Shlwapi.lib")

using namespace std;

namespace AxStd
{

#ifdef _DEBUG
	void AFXAPI Msg(char* fmt, ...);
#endif
	CString AFXAPI FORMAT(const char* formatString, ...);
	CString Parser(CString &srcstr, CString substr);
	int     GetLineCount(CString str);
	int	SplitSub(vector<CString> &vString, CString& srcstr, CString substr);
	int	Split(vector<CString> &vString, CString& srcstr, CString str);
	int	Split(vector<CString> &elems, const string &s, char delim);


	/**
	* Copyright (C) 2013 by Kyung-jin Kim
	* e-mail		: devmachine@naver.com
	*
	*
	* Description	: Dynamic Invoke Dll function Helper Class
	* Created		: Aug-27,2013
	* Version		: DllInvoker v0.91
	*/

	/**
	* Sample code
	try
	{
	DllInvoker kernel32(_T("kernel32.dll"));
	kernel32.Function<void WINAPI(DWORD)>("Sleep")(1000);
	}
	catch (DllInvoker::Exception e)
	{
	TRACE(_T("Invoke Error: %s\n"), e.GetErrorMessage());
	}
	*/
	class DllInvoker
	{
	public:
		DllInvoker(void) : m_hInstance(NULL) {}
		DllInvoker(LPCTSTR lpFileName) : m_hInstance(NULL)
		{
			Load(lpFileName);
		}

		~DllInvoker(void)
		{
			if (m_hInstance)
				::FreeLibrary(m_hInstance);
		}

		void Load(LPCTSTR lpFileName)
		{
			ASSERT(NULL == m_hInstance);

			m_sFilename = lpFileName;
			m_hInstance = ::LoadLibrary(lpFileName);
		}

		BOOL IsLoaded() const
		{
			return m_hInstance != NULL;
		}

		template <typename _Signature>
		_Signature* Function(LPCSTR lpFunctionName)
		{
			if (!IsLoaded())
			{
				CString sErrorMessage;
				sErrorMessage.Format(_T("%s not loaded."), m_sFilename);
		//		throw CException(sErrorMessage);
				AfxMessageBox(sErrorMessage);
			}

			_Signature* func = reinterpret_cast<_Signature*>(GetProcAddress(m_hInstance, lpFunctionName));
			if (!func)
			{
				CString sErrorMessage;
				sErrorMessage.Format(_T("Failed to find %s function."), CA2T(lpFunctionName));
		//		throw CException(sErrorMessage);
				AfxMessageBox(sErrorMessage);
			}
			return func;
		}

		template <typename _Signature>
		_Signature* Function(LPCWSTR lpFunctionName)
		{
			return Function<_Signature>(CW2A(lpFunctionName));
		}

		template <typename _Type>
		_Type* Variable(LPCSTR lpVariableName)
		{
			return Function<_Type>(lpVariableName);
		}

		template <typename _Type>
		_Type* Variable(LPCWSTR lpVariableName)
		{
			return Function<_Type>(lpVariableName);
		}

		BOOL EnumDllFunctions(std::vector<CString>& listOfFunctions)
		{
			if (!IsLoaded())
				return FALSE;

			BYTE* pInstance = (BYTE*)m_hInstance;
			PIMAGE_NT_HEADERS pHeader = (PIMAGE_NT_HEADERS)&pInstance[PIMAGE_DOS_HEADER(pInstance)->e_lfanew];
			PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)&pInstance[pHeader->OptionalHeader.DataDirectory->VirtualAddress];
			DWORD* pFunctions = (DWORD*)&pInstance[pExport->AddressOfNames];

			listOfFunctions.clear();
			for (DWORD i = 0; i < pExport->NumberOfNames; ++i)
				listOfFunctions.push_back((LPCTSTR)CA2T((char*)&pInstance[pFunctions[i]]));

			return TRUE;
		}

	private:
		DllInvoker(const DllInvoker&);
		DllInvoker& operator=(const DllInvoker&);

		CString		m_sFilename;
		HINSTANCE	m_hInstance;
	};


	/************************************************************************************************/
	// Class EzIni
	/************************************************************************************************/
	class EZIni
	{
	public:
		class Exception
		{
		public:
			Exception(DWORD dwErrorCode, LPCTSTR lpErrorMessage)
				: m_dwErrorCode(dwErrorCode), m_sErrorMessage(lpErrorMessage) {}

			CString GetErrorMessage() const { return m_sErrorMessage; }
			DWORD GetErrorCode() const { return m_dwErrorCode; }

		private:
			DWORD m_dwErrorCode;
			CString m_sErrorMessage;
		};

		class Key;
		class Section
		{
			friend class EZIni;

		public:
			// Get Key Object
			Key operator[](LPCSTR lpKeyName);
			Key operator[](LPCWSTR lpKeyName);

			// Ini Section Helper function
			BOOL Exists() const;
			BOOL Delete();
			BOOL EnumKeyNames(std::vector<CString>& listOfKeyNames) const;

		private:
			Section(LPCTSTR lpFileName, LPCTSTR lpSectionName);

		private:
			CString m_sFileName;
			CString m_sSectionName;
		};

		class Key
		{
			friend class EZIni;
			friend class EZIni::Section;

		public:
			// Write Operation
			Key& operator=(int nValue);
			Key& operator=(UINT uValue);
			Key& operator=(INT64 n64Value);
			Key& operator=(UINT64 u64Value);
			Key& operator=(bool bValue);
			Key& operator=(double dValue);
			Key& operator=(LPCTSTR lpValue);
			Key& operator=(POINT ptValue);
			Key& operator=(LPCRECT lpRectValue);
			Key& operator=(const COleDateTime& dtValue);

			// Read Operation
			operator int();
			operator UINT();
			operator INT64();
			operator UINT64();
			operator bool();
			operator double();
			operator CString();
			operator CPoint();
			operator CRect();
			operator COleDateTime();

			// Arithmetic/Logical Operation
			Key& operator++();
			Key& operator--();
			Key& operator+=(int nValue);
			Key& operator-=(int nValue);
			Key& operator*=(int nValue);
			Key& operator/=(int nValue);
			Key& operator%=(int nValue);

			Key& operator+=(UINT uValue);
			Key& operator-=(UINT uValue);
			Key& operator*=(UINT uValue);
			Key& operator/=(UINT uValue);
			Key& operator%=(UINT uValue);
			Key& operator<<=(UINT uValue);
			Key& operator>>=(UINT uValue);
			Key& operator&=(UINT uValue);
			Key& operator|=(UINT uValue);
			Key& operator^=(UINT uValue);

			Key& operator+=(INT64 n64Value);
			Key& operator-=(INT64 n64Value);
			Key& operator*=(INT64 n64Value);
			Key& operator/=(INT64 n64Value);
			Key& operator%=(INT64 n64Value);

			Key& operator+=(UINT64 u64Value);
			Key& operator-=(UINT64 u64Value);
			Key& operator*=(UINT64 u64Value);
			Key& operator/=(UINT64 u64Value);
			Key& operator%=(UINT64 u64Value);
			Key& operator<<=(UINT64 u64Value);
			Key& operator>>=(UINT64 u64Value);
			Key& operator&=(UINT64 u64Value);
			Key& operator|=(UINT64 u64Value);
			Key& operator^=(UINT64 u64Value);

			Key& operator+=(double dValue);
			Key& operator-=(double dValue);
			Key& operator*=(double dValue);
			Key& operator/=(double dValue);

			Key& operator+=(POINT ptValue);
			Key& operator-=(POINT ptValue);
			Key& operator+=(SIZE sizeValue);
			Key& operator-=(SIZE sizeValue);
			Key& operator+=(LPCRECT lpRectValue);
			Key& operator-=(LPCRECT lpRectValue);
			Key& operator+=(COleDateTimeSpan dateSpan);
			Key& operator-=(COleDateTimeSpan dateSpan);

			// Append Operation
			Key& operator+=(LPCTSTR lpText);

			// Set Default Value
			Key& operator<<(int nDefaultValue);
			Key& operator<<(UINT uDefaultValue);
			Key& operator<<(INT64 n64DefaultValue);
			Key& operator<<(UINT64 u64DefaultValue);
			Key& operator<<(bool bDefaultValue);
			Key& operator<<(double dDefaultValue);
			Key& operator<<(LPCTSTR lpDefaultValue);
			Key& operator<<(POINT ptDefaultValue);
			Key& operator<<(LPCRECT lpDefaultValue);
			Key& operator<<(const COleDateTime& dtDefaultValue);

			Key& operator>>(int nDefaultValue);
			Key& operator>>(UINT uDefaultValue);
			Key& operator>>(INT64 n64DefaultValue);
			Key& operator>>(UINT64 u64DefaultValue);
			Key& operator>>(bool bDefaultValue);
			Key& operator>>(double dDefaultValue);
			Key& operator>>(LPCTSTR lpDefaultValue);
			Key& operator>>(POINT ptDefaultValue);
			Key& operator>>(LPCRECT lpDefaultValue);
			Key& operator>>(const COleDateTime& dtDefaultValue);

			// Ini Key Helper function
			BOOL Exists() const;
			BOOL Delete();

		private:
			Key(LPCTSTR lpFileName, LPCTSTR lpSectionName, LPCTSTR lpKeyName);

			template <typename _T>
			void _SetKeyValue(_T value, LPCTSTR lpFormatSpec);
			template <typename _T>
			CString _GetKeyValue(const _T& defaultValue, LPCTSTR lpFormatSpec);

			CString _Point2String(const POINT& ptValue);
			CString _Rect2String(LPCRECT lpRectValue);
			CString _DateTime2String(const COleDateTime& dtValue);

			CPoint _String2Point(LPCTSTR lpValue);
			CRect _String2Rect(LPCTSTR lpValue);
			COleDateTime _String2DateTime(LPCTSTR lpValue);

		private:
			CString m_sFileName;
			CString m_sSectionName;
			CString m_sKeyName;

			CString m_sDefaultValue;
			union
			{
				int m_nDefaultValue;
				UINT m_uDefaultValue;
				INT64 m_n64DefaultValue;
				UINT64 m_u64DefaultValue;
				bool m_bDefaultValue;
				double m_dDefaultValue;
				POINT m_ptDefaultValue;
				RECT m_rcDefaultValue;
				DATE m_dtDefaultValue;
			};
		};

		EZIni(void);
		EZIni(LPCTSTR lpFileName, BOOL bCreateAsUnicode = FALSE);
		~EZIni(void);

		// Get Section Object
		Section operator[](LPCSTR lpSectionName);
		Section operator[](LPCWSTR lpSectionName);

		// Initialize Function
		void SetFileName(LPCTSTR lpFileName, BOOL bCreateAsUnicode = FALSE);

		// Ini Helper Function
		BOOL EnumSectionNames(std::vector<CString>& listOfSectionNames) const;

	private:
		enum { READ_BUFFER_SIZE = 512 };

		static CString _GetErrorMessage();

	private:
		CString m_sFileName;
	};

	template <typename _T>
	void EZIni::Key::_SetKeyValue(_T value, LPCTSTR lpFormatSpec)
	{
		ASSERT(!m_sFileName.IsEmpty() && !m_sSectionName.IsEmpty() && !m_sKeyName.IsEmpty());

		CString sValue;
		sValue.Format(lpFormatSpec, value);
		BOOL bResult = WritePrivateProfileString(m_sSectionName, m_sKeyName, sValue, m_sFileName);
		if (!bResult)
		{
			CString sErrMessage, sFormat;
			sFormat.Format(_T("Failed to write value(%s)."), lpFormatSpec);
			sFormat += _T(" %s");
			sErrMessage.Format(sFormat, value, EZIni::_GetErrorMessage());
			throw EZIni::Exception(GetLastError(), sErrMessage);
		}
	}

	template <typename _T>
	CString EZIni::Key::_GetKeyValue(const _T& defaultValue, LPCTSTR lpFormatSpec)
	{
		CString sDefaultValue;
		sDefaultValue.Format(lpFormatSpec, defaultValue);

		DWORD dwLen = READ_BUFFER_SIZE;
		LPTSTR pBuffer = new TCHAR[dwLen + 1];

		DWORD dwCopied = GetPrivateProfileString(m_sSectionName, m_sKeyName, sDefaultValue, pBuffer, dwLen, m_sFileName);
		while (dwCopied + 1 >= dwLen)
		{
			dwLen += READ_BUFFER_SIZE;
			delete[] pBuffer;
			pBuffer = new TCHAR[dwLen + 1];
			dwCopied = GetPrivateProfileString(m_sSectionName, m_sKeyName, sDefaultValue, pBuffer, dwLen, m_sFileName);
		}

		CString sValue(pBuffer);
		delete[] pBuffer;

		return sValue;
	}


	/**********************************************************************************************************************/
	// Class EZRegistry
	/**********************************************************************************************************************/
	class EZRegistry
	{
	public:
		enum OpenFlags
		{
			modeRead = 0x0001,
			modeWrite = 0x0002,
			modeReadWrite = modeRead | modeWrite,

			wow64Key32 = 0x0010,
			wow64Key64 = 0x0020,

			optVolatile = 0x0100,
			optUseDefaultValue = 0x0200
		};

		class Exception
		{
		public:
			Exception(DWORD dwErrorCode, LPCTSTR lpErrorMessage)
				: m_dwErrorCode(dwErrorCode), m_sErrorMessage(lpErrorMessage) {}

			CString GetErrorMessage() const { return m_sErrorMessage; }
			DWORD GetErrorCode() const { return m_dwErrorCode; }

		private:
			DWORD m_dwErrorCode;
			CString m_sErrorMessage;
		};

		class Binary
		{
		public:
			Binary();
			Binary(BYTE* pData, int nLength);

			Binary(const Binary& other);
			Binary& operator=(const Binary& other);

#if _MSC_VER >= 1600
			Binary(Binary&& other);
			Binary& operator=(Binary&& other);
#endif
			~Binary();

			void SetData(BYTE* pData, int nLength);
			void FillData(int nLength, BYTE byFill = 0);

			BYTE* GetData() const;
			int GetLength() const;

			BOOL IsEmpty() const;
			void Clear();

		private:
			BYTE* m_pData;
			int m_nLength;
		};

		class Value
		{
			friend class EZRegistry;

		public:
			Value(const Value& other);
			Value& operator=(const Value& other);

			// Write Operation
			Value& operator=(int nValue);
			Value& operator=(UINT uValue);
			Value& operator=(INT64 n64Value);
			Value& operator=(UINT64 u64Value);
			Value& operator=(bool bValue);
			Value& operator=(double dValue);
			Value& operator=(LPCTSTR lpValue);
			Value& operator=(POINT ptValue);
			Value& operator=(LPCRECT lpRectValue);
			Value& operator=(const COleDateTime& dtValue);
			Value& operator=(const EZRegistry::Binary& binaryValue);

			// Read Operation
			operator int();
			operator UINT();
			operator INT64();
			operator UINT64();
			operator bool();
			operator double();
			operator CString();
			operator CPoint();
			operator CRect();
			operator COleDateTime();
			operator EZRegistry::Binary();

			// Arithmetic/Logical Operation
			Value& operator++();
			Value& operator--();
			Value& operator+=(int nValue);
			Value& operator-=(int nValue);
			Value& operator*=(int nValue);
			Value& operator/=(int nValue);
			Value& operator%=(int nValue);

			Value& operator+=(UINT uValue);
			Value& operator-=(UINT uValue);
			Value& operator*=(UINT uValue);
			Value& operator/=(UINT uValue);
			Value& operator%=(UINT uValue);
			Value& operator<<=(UINT uValue);
			Value& operator>>=(UINT uValue);
			Value& operator&=(UINT uValue);
			Value& operator|=(UINT uValue);
			Value& operator^=(UINT uValue);

			Value& operator+=(INT64 n64Value);
			Value& operator-=(INT64 n64Value);
			Value& operator*=(INT64 n64Value);
			Value& operator/=(INT64 n64Value);
			Value& operator%=(INT64 n64Value);

			Value& operator+=(UINT64 u64Value);
			Value& operator-=(UINT64 u64Value);
			Value& operator*=(UINT64 u64Value);
			Value& operator/=(UINT64 u64Value);
			Value& operator%=(UINT64 u64Value);
			Value& operator<<=(UINT64 u64Value);
			Value& operator>>=(UINT64 u64Value);
			Value& operator&=(UINT64 u64Value);
			Value& operator|=(UINT64 u64Value);
			Value& operator^=(UINT64 u64Value);

			Value& operator+=(double dValue);
			Value& operator-=(double dValue);
			Value& operator*=(double dValue);
			Value& operator/=(double dValue);

			Value& operator+=(POINT ptValue);
			Value& operator-=(POINT ptValue);
			Value& operator+=(SIZE sizeValue);
			Value& operator-=(SIZE sizeValue);
			Value& operator+=(LPCRECT lpRectValue);
			Value& operator-=(LPCRECT lpRectValue);
			Value& operator+=(COleDateTimeSpan dateSpan);
			Value& operator-=(COleDateTimeSpan dateSpan);

			// Append Operation
			Value& operator+=(LPCTSTR lpText);

			// Set Default Value
			Value& operator<<(int nDefaultValue);
			Value& operator<<(UINT uDefaultValue);
			Value& operator<<(INT64 n64DefaultValue);
			Value& operator<<(UINT64 u64DefaultValue);
			Value& operator<<(bool bDefaultValue);
			Value& operator<<(double dDefaultValue);
			Value& operator<<(LPCTSTR lpDefaultValue);
			Value& operator<<(POINT ptDefaultValue);
			Value& operator<<(LPCRECT lpDefaultValue);
			Value& operator<<(const COleDateTime& dtDefaultValue);

			Value& operator>>(int nDefaultValue);
			Value& operator>>(UINT uDefaultValue);
			Value& operator>>(INT64 n64DefaultValue);
			Value& operator>>(UINT64 u64DefaultValue);
			Value& operator>>(bool bDefaultValue);
			Value& operator>>(double dDefaultValue);
			Value& operator>>(LPCTSTR lpDefaultValue);
			Value& operator>>(POINT ptDefaultValue);
			Value& operator>>(LPCRECT lpDefaultValue);
			Value& operator>>(const COleDateTime& dtDefaultValue);

			// Registry Value Helper Function
			BOOL Exists() const;
			BOOL Delete();

		protected:
			Value(const EZRegistry& rRegistry, LPCTSTR lpValueName);

			void _SetRegValue(DWORD dwType, LPVOID lpValue, int nSize, LPCTSTR lpFormatSpec, ...);
			template <typename _T>
			_T _GetRegValue(const _T& defaultValue, DWORD dwDesiredType, int nDesiredSize);

			template <typename _T>
			void _GetRegQueryValue(_T& value, DWORD dwSize);
			template <>
			void _GetRegQueryValue(CString& value, DWORD dwSize);
			template <>
			void _GetRegQueryValue(EZRegistry::Binary& value, DWORD dwSize);

			DWORD _GetRegValueType() const;
			int _GetRegValueSize() const;

		protected:
			const EZRegistry& m_rRegistry;
			CString m_sValueName;

			CString m_sDefaultValue;
			union
			{
				int m_nDefaultValue;
				UINT m_uDefaultValue;
				INT64 m_n64DefaultValue;
				UINT64 m_u64DefaultValue;
				bool m_bDefaultValue;
				double m_dDefaultValue;
				POINT m_ptDefaultValue;
				RECT m_rcDefaultValue;
				DATE m_dtDefaultValue;
			};
		};

		class EnumeratedValue : public Value
		{
			friend class EZRegistry;

		public:
			EnumeratedValue(const EnumeratedValue& other);
			EnumeratedValue& operator=(const EnumeratedValue& other);

			// Write Operation
			EnumeratedValue& operator=(int nValue);
			EnumeratedValue& operator=(UINT uValue);
			EnumeratedValue& operator=(INT64 n64Value);
			EnumeratedValue& operator=(UINT64 u64Value);
			EnumeratedValue& operator=(bool bValue);
			EnumeratedValue& operator=(double dValue);
			EnumeratedValue& operator=(LPCTSTR lpValue);
			EnumeratedValue& operator=(POINT ptValue);
			EnumeratedValue& operator=(LPCRECT lpRectValue);
			EnumeratedValue& operator=(const COleDateTime& dtValue);

			CString GetName() const { return m_sValueName; }
			DWORD GetType() const { return m_dwType; }
			int GetSize() const { return m_nSize; }

		private:
			EnumeratedValue(const EZRegistry& rRegistry, LPCTSTR lpValueName, DWORD dwType, int nSize);

		private:
			DWORD m_dwType;
			int m_nSize;
		};

		EZRegistry(void);
		EZRegistry(const EZRegistry& baseKey, LPCTSTR lpSubKey, UINT nOpenFlags = modeReadWrite);
		~EZRegistry(void);

		BOOL Open(const EZRegistry& baseKey, LPCTSTR lpSubKey, UINT nOpenFlags = modeReadWrite);
		BOOL IsOpen() const;

		void Close();
		void ClearSubKeyCache();

		// Get Value object
		Value operator[](LPCSTR lpValueName);
		Value operator[](LPCWSTR lpValueName);

		// Get Subkey object
		EZRegistry& operator+(LPCSTR lpSubKey);
		EZRegistry& operator+(LPCWSTR lpSubKey);

		EZRegistry& operator()(LPCSTR lpSubKey);
		EZRegistry& operator()(LPCWSTR lpSubKey);

		// Create Subkey
		EZRegistry& operator+=(LPCSTR lpSubKey);
		EZRegistry& operator+=(LPCWSTR lpSubKey);

		// Registry Key Helper Function
		static BOOL KeyExists(const EZRegistry& baseKey, LPCTSTR lpSubKey);
		BOOL SubKeyExists(LPCTSTR lpSubKey) const;

		BOOL Delete(BOOL bRecursive = TRUE);
		BOOL DeleteSubKey(LPCTSTR lpSubKey, BOOL bRecursive = TRUE);

		BOOL HasSubKeys() const;
		BOOL EnumSubKeys(std::vector<CString>& listOfSubKeys) const;
		BOOL EnumValues(std::vector<EnumeratedValue>& listOfValues) const;

	private:
		EZRegistry(const EZRegistry& other);
		EZRegistry& operator=(const EZRegistry& other);

		EZRegistry(HKEY hKey) : m_hKey(hKey), m_nOpenFlags(0) {}

		LONG _Open(const EZRegistry& baseKey, LPCTSTR lpSubKey, UINT nOpenFlags);
		EZRegistry& _OpenSubKey(LPCTSTR lpSubKey);

		static BOOL _IsPredefinedKey(HKEY hKey);
		static CString _GetErrorMessage(DWORD dwError);

	public:
		static const EZRegistry classesRoot;
		static const EZRegistry currentUser;
		static const EZRegistry localMachine;
		static const EZRegistry users;
		static const EZRegistry currentConfig;

	private:
		HKEY m_hKey;
		UINT m_nOpenFlags;
		std::map<CString, EZRegistry*> m_subKeyMap;
	};


	template <typename _T>
	_T EZRegistry::Value::_GetRegValue(const _T& defaultValue, DWORD dwDesiredType, int nDesiredSize)
	{
		ASSERT(m_rRegistry.m_nOpenFlags & modeRead);

		if (!m_rRegistry.IsOpen() && (m_rRegistry.m_nOpenFlags & optUseDefaultValue))
			return defaultValue;

		_T value;
		DWORD dwType, dwSize;
		LONG nResult;
		CString sErrMessage;

		try
		{
			nResult = ::RegQueryValueEx(m_rRegistry.m_hKey, m_sValueName, NULL, &dwType, NULL, &dwSize);

			if (nResult != ERROR_SUCCESS)
			{
				sErrMessage.Format(_T("Failed to read value. %s"), EZRegistry::_GetErrorMessage(nResult));
				throw EZRegistry::Exception(nResult, sErrMessage);
			}

			if (dwType != dwDesiredType)
			{
				sErrMessage.Format(_T("Failed to read value. Invalid data type(Type: %d, Desired: %d)."), dwType, dwDesiredType);
				throw EZRegistry::Exception(ERROR_DATATYPE_MISMATCH, sErrMessage);
			}

			if (0 == nDesiredSize)
			{
				if (0 == dwSize)
				{
					sErrMessage.Format(_T("Failed to read value. Invalid data size(Size: %d)."), dwSize);
					throw EZRegistry::Exception(ERROR_INCORRECT_SIZE, sErrMessage);
				}
			}
			else
			{
				if (static_cast<int>(dwSize) != nDesiredSize)
				{
					sErrMessage.Format(_T("Failed to read value. Invalid data size(Size: %d, Desired: %d)."), dwSize, nDesiredSize);
					throw EZRegistry::Exception(ERROR_INCORRECT_SIZE, sErrMessage);
				}
			}
			_GetRegQueryValue(value, dwSize);
		}
		catch (EZRegistry::Exception& e)
		{
			if (m_rRegistry.m_nOpenFlags & optUseDefaultValue)
				return defaultValue;

			throw e;
		}

		return value;
	}

	template <typename _T>
	void EZRegistry::Value::_GetRegQueryValue(_T& value, DWORD dwSize)
	{
		LONG nResult;
		CString sErrMessage;

		nResult = ::RegQueryValueEx(m_rRegistry.m_hKey, m_sValueName, NULL, NULL, reinterpret_cast<LPBYTE>(&value), &dwSize);
		if (nResult != ERROR_SUCCESS)
		{
			sErrMessage.Format(_T("Failed to read value. %s"), EZRegistry::_GetErrorMessage(nResult));
			throw EZRegistry::Exception(nResult, sErrMessage);
		}
	}

	template <>
	void EZRegistry::Value::_GetRegQueryValue(CString& value, DWORD dwSize)
	{
		LONG nResult;
		CString sErrMessage;

		BYTE* pData = new BYTE[dwSize];
		ZeroMemory(pData, dwSize);
		nResult = ::RegQueryValueEx(m_rRegistry.m_hKey, m_sValueName, NULL, NULL, pData, &dwSize);
		if (nResult != ERROR_SUCCESS)
		{
			delete[] pData;
			sErrMessage.Format(_T("Failed to read value. %s"), EZRegistry::_GetErrorMessage(nResult));
			throw EZRegistry::Exception(nResult, sErrMessage);
		}

		value = (TCHAR*)pData;
		delete[] pData;
	}

	template <>
	void EZRegistry::Value::_GetRegQueryValue(EZRegistry::Binary& value, DWORD dwSize)
	{
		LONG nResult;
		CString sErrMessage;

		value.FillData(dwSize);
		nResult = ::RegQueryValueEx(m_rRegistry.m_hKey, m_sValueName, NULL, NULL, value.GetData(), &dwSize);
		if (nResult != ERROR_SUCCESS)
		{
			sErrMessage.Format(_T("Failed to read value. %s"), EZRegistry::_GetErrorMessage(nResult));
			throw EZRegistry::Exception(nResult, sErrMessage);
		}
	}

	class ICallback
	{
	public:
		virtual void Run(){};
	};

	template<class T>
	class Callback : public ICallback
	{
	private:
		typedef void(T::*Func)();
	public:
		Callback(T* pOwner, Func pfFunc) : 
	        m_pOwner(pOwner) m_pfFunc(pFunc){};

		virtual void Run()
		{
			(m_pOwner->*m_pfFunc)();
		}
	};

	class CallbackRepository
	{
	private:
		typedef map<DWORD, ICallback*>	 MAP_CALLBACK;
		typedef MAP_CALLBACK::iterator	 MAP_CALLBACK_IT;
		typedef MAP_CALLBACK::value_type MAP_CALLBAKC_PAIR;

	public:
//		virtual ICallbackRepository(){};
		CallbackRepository(){};

	public:
		void Run(DWORD dwKey);
		virtual ~CallbackRepository(){};

	protected:
		MAP_CALLBACK m_mapCallback;
	};

#define CREATE_CALLBACK(CC_KEY, CC_CLASS, CC_FUNC) \
	CallbackRepository::insert(CC_KEY, new Callback<CC_CLASS>(this, &CC_CLASS::CC_FUCN))
}