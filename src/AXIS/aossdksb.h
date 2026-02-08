/*
 * (C) Copyright 2005-2010. AhnLab, Inc.
 *
 * Any part of this source code can not be copied with
 * any method without prior written permission from
 * the author or authorized person.
 *
 * @version			$Revision: 1918 $
 * 
 */

#ifndef	__aossdksb_h__
#define	__aossdksb_h__

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////
// Activate Option

#define SBSDK_ACTIVATE_SHILD					0x00000001L	
#define SBSDK_ACTIVATE_ANTIDEBUG				0x00000002L
#define	SBSDK_ACTIVATE_ANTIHTMLINJECTION		0x00000004L

//////////////////////////////////////////////////////////////
// SDK Extension Option

#define	SBSDK_EXOPTION_EXCEPTPROCESS			0x00000001L
#define	SBSDK_EXOPTION_PROTECTPROCESS_INS		0x00000002L
#define	SBSDK_EXOPTION_PROTECTPROCESS_DEL		0x00000003L
#define SBSDK_EXOPTION_EXCEPTIONPROCESS_INS		0x00000004L
#define SBSDK_EXOPTION_EXCEPTIONPROCESS_DEL		0x00000005L

//////////////////////////////////////////////////////////////
// SB Status Option

#define	SBSDK_STATUS_EXOPTION					0x00000001L


//////////////////////////////////////////////////////////////
// Error Code 

// Success
#define SBSDK_ERROR_SUCCESS						0L
// Initialization failed
#define SBSDK_ERROR_INITIALIZE_FAILED			0xE9000001L
// Unitialization failed
#define SBSDK_ERROR_UNINITIALIZE_FAILED			0xE6000002L
// Already been initialized
#define SBSDK_ERROR_ALREADY_INITIALIZED			0xE9000003L
// Invalid argument
#define SBSDK_ERROR_BAD_PARAMETER				0xE9000004L
// Debugger has been detected
#define SBSDK_ERROR_DETECT_DEBUG				0xE9000005L
// Running in compatibility mode
#define SBSDK_ERROR_COMPATIBILITY_MODE			0xE9000006L
// Execption process setting failed.
#define SBSDK_ERROR_EXCEPTION_PS_FAILED			0xE9000007L
// Need to initialize with SBSDK_ACTIVATE_SHILD 
#define SBSDK_ERROR_NEED_ACTIVATESHIELD			0xE9000008L



//////////////////////////////////////////////////////////////
// CallBack Code			 

// Driver has failed to run
#define SBSDK_CALLBACK_DRIVERFAILED				0x010301			
// Debugger has been detected
#define SBSDK_CALLBACK_DETECT_DEBUG				0x010302
// In memory of attempted unauthorized access
#define SBSDK_CALLBACK_ABNORMAL_MEMORY_ACCESS	0x010601
// Malicious code has been blocked
#define SBSDK_CALLBACK_BLACKLIST				0x020001

//////////////////////////////////////////////////////////////
// Callback Function 

typedef struct 
{
	ULONG		ulPID;
	ULONG		uPadding;
	CHAR		szProcessName[1024];
} ST_SBSDK_PROCESS_PATH, *PST_SBSDK_PROCESS_PATH;

typedef int (__stdcall* PFN_SBSDK_Callback)(long lCode, void* pParam, long lParamSize);

//////////////////////////////////////////////////////////////
// This class is exported from the sbsdk.dll

class IAosSB
{
public:
	virtual DWORD _stdcall Initialize(DWORD dwOption, PFN_SBSDK_Callback fnCallback) = 0;
	virtual DWORD _stdcall Uninitialize() = 0;
	virtual DWORD _stdcall ExOption(DWORD dwOption, LPCSTR lpszValue) = 0;
};

typedef IAosSB* (_stdcall *PFN_SBSDK_GetObject)();

#ifdef __cplusplus
}
#endif

#endif /* #ifndef __aossdksb_h__ */
