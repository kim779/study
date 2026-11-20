#pragma once
#include <windows.h>
#include <string>

class IAstxMkd8A;   // ASTx SDK keyboard-protect interface (ASTx/stsdkmkd25.h)

// Single entry point for AhnLab ASTx in the host program.
// Uses astxManager.dll when Load() succeeds, otherwise calls the ASTx SDK (STSDKEX.DLL) directly.
// The path is decided once by Load() and never changed in the process.
// UI and policy (messages, install restart, force close) stay in the caller.
// The DLL is not freed on destruction: SDK may still be shutting down at process exit.
class AstxClient
{
public:
	enum : DWORD
	{
		FUNC_PB = 0x00000004,
		FUNC_FW = 0x00000080,
		FUNC_PCSCAN = 0x00008000,
		RESULT_OK = 0x00000000,
		RESULT_NOT_INSTALLED = 0x32A4100F,
	};

	static const char* ErrStr(DWORD code);   // Korean message (CP949) for an SDK result code

	typedef int(__stdcall* EVENT_CB)(long code, void* param, long paramSize);
	typedef int(__stdcall* AK_CB)(long code, LPARAM param);

	bool  Load(char* log = NULL, int logCap = 0);   // log: optional result message
	bool  IsLoaded() const { return m_h != NULL; }  // true = astxManager.dll path

	BOOL  IsInstalled();
	DWORD InstalledCode();                 // same meaning as STSDKEX_IsInstalled()
	DWORD Install(HWND parent);
	DWORD Init(const char* customerId);
	void  Uninit();                        // AK stop -> stop all funcs -> SDK uninit
	BOOL  IsInitialized();
	DWORD Start(DWORD funcMask);           // STSDKEX_FUNC_CODE_* OR-ed; already-started / priority-rule -> success
	DWORD Stop(DWORD funcMask);            // already-stopped -> success
	DWORD IsRunning(DWORD funcCode, const char* customerId = NULL);   // 0 = running; customerId used before Init
	DWORD AkStart();
	void  AkStop();
	DWORD AkProtectEdit(HWND edit);

private:
	// common
	std::string m_customer;

	// astxManager.dll path
	bool    m_tried = false;
	HMODULE m_h = NULL;

	BOOL(*m_IsInstalled)(void) = NULL;
	DWORD(*m_Install)(HWND) = NULL;
	DWORD(*m_Init)(const char*, EVENT_CB) = NULL;
	void        (*m_Uninit)(void) = NULL;
	BOOL(*m_IsInitialized)(void) = NULL;
	DWORD(*m_Start)(DWORD) = NULL;
	DWORD(*m_Stop)(DWORD) = NULL;
	DWORD(*m_IsRunning)(DWORD) = NULL;
	DWORD(*m_AkStart)(AK_CB) = NULL;
	void        (*m_AkStop)(void) = NULL;
	DWORD(*m_AkProtectEdit)(HWND) = NULL;
	const char* (*m_ErrStr)(DWORD) = NULL;

	// built-in path (direct SDK)
	bool        m_init = false;
	IAstxMkd8A* m_ak = NULL;
};
