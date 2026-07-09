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

#pragma once
#include <vector>

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

	class Exception
	{
	public:
		Exception(LPCTSTR lpErrorMessage) : m_sErrorMessage(lpErrorMessage) {}
		CString GetErrorMessage() const { return m_sErrorMessage; }

	private:
		CString m_sErrorMessage;
	};

	template <typename _Signature>
	_Signature* Function(LPCSTR lpFunctionName)
	{
		if (!IsLoaded())
		{
			CString sErrorMessage;
			sErrorMessage.Format(_T("%s not loaded."), m_sFilename);
			throw Exception(sErrorMessage);
		}

		_Signature* func = reinterpret_cast<_Signature*>(GetProcAddress(m_hInstance, lpFunctionName));
		if (!func)
		{
			CString sErrorMessage;
			sErrorMessage.Format(_T("Failed to find %s function."), CA2T(lpFunctionName));
			throw Exception(sErrorMessage);
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

	CString m_sFilename;
	HINSTANCE m_hInstance;
};