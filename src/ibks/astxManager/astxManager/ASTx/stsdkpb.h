#ifndef __STSDKPB_H_
#define __STSDKPB_H_


//////////////////////////////////////////////////////////////
// Activate Option

#define PBSDK_ACTIVATE_PROTECT_MEMORY			0x00000001L	
#define PBSDK_ACTIVATE_ANTIDEBUG				0x00000002L
#define	PBSDK_ACTIVATE_ANTIHTMLINJECTION		0x00000004L
#define	PBSDK_ACTIVATE_BLOCK_REMOTE				0x00000008L

//////////////////////////////////////////////////////////////
// SDK Extension Option

#define	PBSDK_EXOPTION_EXCEPTPROCESS			0x00000001L
#define	PBSDK_EXOPTION_PROTECTPROCESS_INS		0x00000002L
#define	PBSDK_EXOPTION_PROTECTPROCESS_DEL		0x00000003L
#define	PBSDK_EXOPTION_EXCEPTIONPROCESS_INS		0x00000004L
#define	PBSDK_EXOPTION_EXCEPTIONPROCESS_DEL		0x00000005L


//////////////////////////////////////////////////////////////
// PB Status Option

#define	PBSDK_STATUS_EXOPTION					0x00000001L


//////////////////////////////////////////////////////////////
// Error Code 

// Success
#define PBSDK_ERROR_SUCCESS						0L
// Initialization failed
#define PBSDK_ERROR_INITIALIZE_FAILED			0xE9000001L
// Unitialization failed
#define PBSDK_ERROR_UNINITIALIZE_FAILED			0xE6000002L
// Already been initialized
#define PBSDK_ERROR_ALREADY_INITIALIZED			0xE9000003L
// Invalid argument
#define PBSDK_ERROR_BAD_PARAMETER				0xE9000004L
// Debugger has been detected
#define PBSDK_ERROR_DETECT_DEBUG				0xE9000005L
// Running in compatibility mode
#define PBSDK_ERROR_COMPATIBILITY_MODE			0xE9000006L
// Execption process setting failed.
#define PBSDK_ERROR_EXCEPTION_PS_FAILED			0xE9000007L
// Need to initialize with PBSDK_ACTIVATE_SHILD 
#define PBSDK_ERROR_NEED_ACTIVATESHIELD			0xE9000008L


//////////////////////////////////////////////////////////////
// CallBack Code			 

// // Driver has failed to run
// #define PBSDK_CALLBACK_DRIVERFAILED				0x010301
// Debugger has been detected
#define PBSDK_CALLBACK_DETECT_DEBUG				0x010302
// In memory of attempted unauthorized access
#define PBSDK_CALLBACK_ABNORMAL_MEMORY_ACCESS	0x010601
// Malicious code has been blocked
// #define PBSDK_CALLBACK_BLACKLIST				0x020001
// // Attempting remote access has been detected
#define PBSDK_CALLBACK_REMOTE_DETECT			0x020007
// Attempting remote access has been blocked
#define PBSDK_CALLBACK_REMOTE_BLOCKD			0x020008


#define HSB_MAXPATH				1024
typedef struct 
{
	ULONG		ulPID;
	ULONG		uPadding;
	CHAR		szProcessName[HSB_MAXPATH];
} ST_PBSDK_PROCESS_PATH, *PST_PBSDK_PROCESS_PATH;


/*****************************************************************************
 *
 *	PB Interface
 *
 ***************************************************************************/

typedef int (__stdcall* PFN_PBSDK_Callback)(long lCode, void* pParam, long lParamSize);

class IAstxPb
{
public:
	virtual DWORD _stdcall Initialize() = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall StartA(LPCSTR szCustomerID, LPCSTR szCustomPolicyURL) = 0;
	virtual DWORD _stdcall StartW(LPCWSTR szCustomerID, LPCWSTR szCustomPolicyURL) = 0;
	virtual DWORD _stdcall StopA(LPCSTR szCustomerID) = 0;
	virtual DWORD _stdcall StopW(LPCWSTR szCustomerID) = 0;
 	virtual DWORD _stdcall SetActivateSubFunc(DWORD dwSubFunc) = 0;
 	virtual DWORD _stdcall SetEventCallback(PFN_PBSDK_Callback fnCallback) = 0;
	virtual DWORD _stdcall SetExOptionA(DWORD dwOption, LPCSTR lpszValue) = 0;
	virtual DWORD _stdcall SetExOptionW(DWORD dwOption, LPCWSTR lpszValue) = 0;
	virtual BOOL  _stdcall IsRunning() = 0;

};

#endif // __STSDKPB_H_
