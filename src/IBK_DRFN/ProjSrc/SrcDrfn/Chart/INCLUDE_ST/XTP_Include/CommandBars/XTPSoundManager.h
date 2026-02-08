// XTPSoundManager.h : interface for the CXTPSoundManager class.
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// ©2004 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO 
// BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED 
// WRITTEN CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS 
// OUTLINED IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT.  CODEJOCK SOFTWARE 
// GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE 
// THIS SOFTWARE ON A SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

//:Ignore
#if !defined(__XTPSOUNDMANAGER_H__)
#define __XTPSOUNDMANAGER_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore


//:Ignore
// Internal class.

enum XTPSoundManagerState
{
	xtpSoundNone,
	xtpSoundMenuCommand,
	xtpSoundMenuPopup,
	xtpSoundMenuTerminate,
};

class _XTP_EXT_CLASS CXTPSoundManager
{
	friend class CXTPSoundManager* XTPSoundManager();

	CXTPSoundManager(void);
public:

	~CXTPSoundManager(void);

	void PlaySystemSound(XTPSoundManagerState state);
	void EnableSystemSounds(BOOL bEnable);

	void StopThread();
private:
	void StartThread();
	static DWORD WINAPI SoundThreadProc(LPVOID);
	
private:
	XTPSoundManagerState m_soundState;
	BOOL m_bSystemSounds;
	HANDLE m_hThread;

	static CXTPSoundManager s_managerInstance;
	
private:
};


//:End Ignore

AFX_INLINE CXTPSoundManager* XTPSoundManager() { 
	return &CXTPSoundManager::s_managerInstance; 
}


#endif //#if !defined(__XTPSOUNDMANAGER_H__)