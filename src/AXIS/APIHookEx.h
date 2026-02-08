#ifndef _API_HOOK_EX_
#define _API_HOOK_EX_

#include <vector>
#include <string>
#include <algorithm>

#pragma pack(1)

#define C_ERROR         0xFFFFFFFF
#define C_PREFIX        0x00000001
#define C_66            0x00000002
#define C_67            0x00000004
#define C_DATA66        0x00000008
#define C_DATA1         0x00000010
#define C_DATA2         0x00000020
#define C_DATA4         0x00000040
#define C_MEM67         0x00000080
#define C_MEM1          0x00000100
#define C_MEM2          0x00000200
#define C_MEM4          0x00000400
#define C_MODRM         0x00000800
#define C_DATAW0        0x00001000
#define C_FUCKINGTEST   0x00002000
#define C_TABLE_0F      0x00004000

typedef UINT   (WINAPI *FPGetPrivateProfileIntA)          ( LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName );
typedef DWORD  (WINAPI *FPGetPrivateProfileStringA)       ( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName );
typedef BOOL   (WINAPI *FPWritePrivateProfileStringA)     ( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpString, LPCSTR lpFileName );
typedef DWORD  (WINAPI *FPGetPrivateProfileSectionA)      ( LPCSTR lpAppName, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName );
typedef BOOL   (WINAPI *FPWritePrivateProfileSectionA)    ( LPCSTR lpAppName, LPCSTR lpString, LPCSTR lpFileName );
typedef DWORD  (WINAPI *FPGetPrivateProfileSectionNamesA) ( LPSTR lpszReturnBuffer, DWORD nSize, LPCSTR lpFileName );
typedef BOOL   (WINAPI *FPGetPrivateProfileStructA)       ( LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct, UINT uSizeStruct, LPCSTR szFile );
typedef BOOL   (WINAPI *FPWritePrivateProfileStructA)     ( LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct, UINT uSizeStruct, LPCSTR szFile );
typedef BOOL   (WINAPI *FPCopyFileA)                      ( LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists );
typedef BOOL   (WINAPI *FPCreateDirectoryA)               ( LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes );
typedef BOOL   (WINAPI *FPCreateDirectoryExA)             ( LPCSTR lpTemplateDirectory, LPCSTR lpNewDirectory, LPSECURITY_ATTRIBUTES lpSecurityAttributes );
typedef HANDLE (WINAPI *FPCreateFileA)                    ( LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile );
typedef BOOL   (WINAPI *FPDeleteFileA)                    ( LPCSTR lpFileName );
typedef HANDLE (WINAPI *FPFindFirstFileA)                 ( LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData );
typedef DWORD  (WINAPI *FPGetFileAttributesA)             ( LPCSTR lpFileName );
typedef BOOL   (WINAPI *FPGetFileAttributesExA)           ( LPCSTR lpFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation );
typedef BOOL   (WINAPI *FPMoveFileA)                      ( LPCSTR lpExistingFileName, LPCSTR lpNewFileName );
typedef BOOL   (WINAPI *FPMoveFileExA)                    ( LPCSTR lpExistingFileName, LPCSTR lpNewFileName, DWORD dwFlags );
typedef BOOL   (WINAPI *FPRemoveDirectoryA)               ( LPCSTR lpPathName );
typedef DWORD  (WINAPI *FPGetFullPathNameA)               ( LPCSTR lpFileName, DWORD nBufferLength, LPSTR lpBuffer, LPSTR *lpFilePart);

typedef struct _JMPS {
	char	opcode;
	DWORD	RVA;
	char	elses[3];
} JMPS, *PJMPS;

VOID GetInstLenght(PVOID myiptr0, PULONG osizeptr);

UINT   WINAPI AxGetPrivateProfileIntA          ( LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName );                                             
DWORD  WINAPI AxGetPrivateProfileStringA       ( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName ); 
BOOL   WINAPI AxWritePrivateProfileStringA     ( LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpString, LPCSTR lpFileName );                                     
DWORD  WINAPI AxGetPrivateProfileSectionA      ( LPCSTR lpAppName, LPSTR lpReturnedString, DWORD nSize, LPCSTR lpFileName );                                    
BOOL   WINAPI AxWritePrivateProfileSectionA    ( LPCSTR lpAppName, LPCSTR lpString, LPCSTR lpFileName );                                                      
DWORD  WINAPI AxGetPrivateProfileSectionNamesA ( LPSTR lpszReturnBuffer, DWORD nSize, LPCSTR lpFileName );                                                 
BOOL   WINAPI AxGetPrivateProfileStructA       ( LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct, UINT uSizeStruct, LPCSTR szFile );                         
BOOL   WINAPI AxWritePrivateProfileStructA     ( LPCSTR lpszSection, LPCSTR lpszKey, LPVOID lpStruct, UINT uSizeStruct, LPCSTR szFile );                       
BOOL   WINAPI AxCopyFileA                      ( LPCSTR lpExistingFileName, LPCSTR lpNewFileName, BOOL bFailIfExists );
BOOL   WINAPI AxCreateDirectoryA               ( LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes );
BOOL   WINAPI AxCreateDirectoryExA             ( LPCSTR lpTemplateDirectory, LPCSTR lpNewDirectory, LPSECURITY_ATTRIBUTES lpSecurityAttributes );
HANDLE WINAPI AxCreateFileA                    ( LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile );
BOOL   WINAPI AxDeleteFileA                    ( LPCSTR lpFileName );
HANDLE WINAPI AxFindFirstFileA                 ( LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData );
DWORD  WINAPI AxGetFileAttributesA             ( LPCSTR lpFileName );
BOOL   WINAPI AxGetFileAttributesExA           ( LPCSTR lpFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation );
BOOL   WINAPI AxMoveFileA                      ( LPCSTR lpExistingFileName, LPCSTR lpNewFileName );
BOOL   WINAPI AxMoveFileExA                    ( LPCSTR lpExistingFileName, LPCSTR lpNewFileName, DWORD dwFlags );
BOOL   WINAPI AxRemoveDirectoryA               ( LPCSTR lpPathName );
DWORD  WINAPI AxGetFullPathNameA               ( LPCSTR lpFileName, DWORD nBufferLength, LPSTR lpBuffer, LPSTR *lpFilePart);

template <class T>
class HookObject
{
public:
	T fPtr;
	T fOrgFunc, fNewFunc;

public:
	HookObject(T org_function, T new_function) : fPtr(org_function), fNewFunc(new_function), fOrgFunc(NULL) {}
	~HookObject() { if (fOrgFunc) VirtualFree(fOrgFunc, 0x1000, MEM_RELEASE); }
	BOOL Hook()
	{
		DWORD dwSize = 0, dwCollectedSpace = 0;
		
		// Check data
		if (!fPtr) return FALSE;
		
		//Memory for gate code
		fOrgFunc = (T)VirtualAlloc(NULL, 0x1000, MEM_RESERVE|MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if (fOrgFunc==NULL)
			return FALSE;
		
		PBYTE pbInst = (PBYTE)fPtr;
		PBYTE pbGate = (PBYTE)*fOrgFunc;
		
		//need at least 5 bytes for jmp Handler instruction
		while (dwCollectedSpace < 5)
		{
			GetInstLenght((PDWORD)pbInst, &dwSize);
			memcpy((PVOID)pbGate,(PVOID)pbInst,dwSize);
			
			//if current instruction is jmp or call(need correct range)
			if ((*pbGate==0xe8)||(*pbGate==0xe9))
			{
				*((DWORD*)(pbGate+1))+=(DWORD)(pbInst-pbGate);
			}
			pbGate += dwSize;
			pbInst += dwSize;
			dwCollectedSpace += dwSize;
		}
		
		//Make jmp orig_function_end
		JMPS jmps;
		jmps.opcode=(char)0xe9;
		jmps.RVA=(DWORD)fPtr-((DWORD)fOrgFunc+5);
		memcpy((PVOID)&jmps.elses,(PVOID)((DWORD)fPtr+5),3);
		
		//Gate code copying
		memcpy((PVOID)((DWORD)fOrgFunc+dwCollectedSpace),(PVOID)&jmps, sizeof(JMPS));
		
		//Make splice code
		jmps.RVA=(DWORD)fNewFunc-((DWORD)fPtr+5);
		
		DWORD OldProtect;
		VirtualProtect(fPtr, dwCollectedSpace + sizeof(JMPS), PAGE_EXECUTE_READWRITE, &OldProtect);
		
		//Splice it
		PVOID pvPtr = (PVOID)fPtr;
		__asm{
			pusha
				xor		eax,eax
				xor		edx,edx
				mov		ebx,dword ptr [jmps]
				mov		ecx,dword ptr [jmps+4]
				mov		esi,dword ptr [pvPtr]
				lock cmpxchg8b qword ptr [esi]
				lock cmpxchg8b qword ptr [esi]
				popa
		}
		
		VirtualProtect(fPtr, dwCollectedSpace + sizeof(JMPS), OldProtect, &OldProtect);
		
		return TRUE;
	}
	void Unhook()
	{
		PBYTE pSrc = (PBYTE)fOrgFunc;
		PBYTE pDst = (PBYTE)fPtr;
		DWORD OldProtect;
		VirtualProtect(fPtr, sizeof(JMPS), PAGE_EXECUTE_READWRITE, &OldProtect);
		for(int n=0; n<sizeof(JMPS); ++n)
		{
			*pDst = *pSrc;
			++pDst;
			++pSrc;
		}
		VirtualProtect(fPtr, sizeof(JMPS), OldProtect, &OldProtect);
		if (fOrgFunc)
		{
			VirtualFree(fOrgFunc, 0x1000, MEM_RELEASE);
			fOrgFunc = NULL;
		}
	}
};

class CProfileHook
{
public:
	CProfileHook();
	~CProfileHook();

	BOOL IsHook() { return bHook; }
	void StartHook();
	void StopHook();
	void SetInfo(LPCSTR lpHome, LPCSTR lpName);
	CString Convert(LPCSTR path);
	UINT AFXAPI HashDataAXIS(LPCSTR key);
public:
	HookObject<FPGetPrivateProfileIntA         > GetPrivateProfileIntA         ;
	HookObject<FPGetPrivateProfileStringA      > GetPrivateProfileStringA      ;
	HookObject<FPWritePrivateProfileStringA    > WritePrivateProfileStringA    ;
	HookObject<FPGetPrivateProfileSectionA     > GetPrivateProfileSectionA     ;
	HookObject<FPWritePrivateProfileSectionA   > WritePrivateProfileSectionA   ;
	HookObject<FPGetPrivateProfileSectionNamesA> GetPrivateProfileSectionNamesA;
	HookObject<FPGetPrivateProfileStructA      > GetPrivateProfileStructA      ;
	HookObject<FPWritePrivateProfileStructA    > WritePrivateProfileStructA    ;
	HookObject<FPCopyFileA                     > CopyFileA                     ;
	HookObject<FPCreateDirectoryA              > CreateDirectoryA              ;
	HookObject<FPCreateDirectoryExA            > CreateDirectoryExA            ;
	HookObject<FPCreateFileA                   > CreateFileA                   ;
	HookObject<FPDeleteFileA                   > DeleteFileA                   ;
	HookObject<FPFindFirstFileA                > FindFirstFileA                ;
	HookObject<FPGetFileAttributesA            > GetFileAttributesA            ;
	HookObject<FPGetFileAttributesExA          > GetFileAttributesExA          ;
	HookObject<FPMoveFileA                     > MoveFileA                     ;
	HookObject<FPMoveFileExA                   > MoveFileExA                   ;
	HookObject<FPRemoveDirectoryA              > RemoveDirectoryA              ;
	HookObject<FPGetFullPathNameA              > GetFullPathNameA              ;
protected:
	std::vector<std::string>  m_sNames, m_sHashs;
	BOOL bHook;
};

extern CProfileHook gAPIHook;

#endif