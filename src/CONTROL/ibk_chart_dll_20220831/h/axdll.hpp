#pragma once
/*
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
	DllInvoker(void) : m_hInstance(nullptr) {}
	DllInvoker(LPCTSTR lpFileName) : m_hInstance(nullptr)
	{
		Load(lpFileName);
	}

	DllInvoker(const DllInvoker&) = delete;
	DllInvoker(DllInvoker&& dll) = delete;
	DllInvoker& operator=(const DllInvoker&) = delete;
	DllInvoker& operator=(DllInvoker&& dll) = delete;

	~DllInvoker()
	{
		Release();
	}

	void Release()
	{
		if (m_hInstance)
			::FreeLibrary(m_hInstance);
	}

	void Load(LPCTSTR lpFileName)
	{
	//	ASSERT(nullptr == m_hInstance);

		m_sFilename = lpFileName;
		m_hInstance = ::LoadLibrary(lpFileName);
	}

	BOOL IsLoaded() const
	{
		return m_hInstance != nullptr;
	}

	template <typename _Signature>
	_Signature* Function(LPCSTR lpFunctionName)
	{
		if (!IsLoaded())
		{
			CString sErrorMessage;
			sErrorMessage.Format(_T("%s not loaded."), m_sFilename.GetString());
			AfxMessageBox(sErrorMessage);
			return nullptr;
		}

		_Signature* func = reinterpret_cast<_Signature*>(GetProcAddress(m_hInstance, lpFunctionName));
		if (!func)
		{
			CString sErrorMessage;
			sErrorMessage.Format(_T("Failed to find %s function."), CA2T(lpFunctionName).operator LPSTR());
			AfxMessageBox(sErrorMessage);
			return nullptr;
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

#pragma warning(disable : 26429)
#pragma warning(disable : 26481)
#pragma warning(disable : 26462)
#pragma warning(disable : 26485)
	BOOL EnumDllFunctions(std::vector<CString>& listOfFunctions)
	{
		if (!IsLoaded())
			return FALSE;
		if (m_hInstance != nullptr)
		{
			BYTE* pInstance = (BYTE*)m_hInstance;
			PIMAGE_NT_HEADERS pHeader = (PIMAGE_NT_HEADERS)& pInstance[PIMAGE_DOS_HEADER(pInstance)->e_lfanew];
			PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)& pInstance[pHeader->OptionalHeader.DataDirectory->VirtualAddress];
			DWORD* pFunctions = (DWORD*)& pInstance[pExport->AddressOfNames];

			listOfFunctions.clear();
			for (DWORD i = 0; i < pExport->NumberOfNames; ++i)
				listOfFunctions.push_back((LPCTSTR)CA2T((char*)& pInstance[pFunctions[i]]));

			return TRUE;
		}
		return FALSE;
	}
private:


	CString		m_sFilename;
	HINSTANCE	m_hInstance{};
};