// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

CSignalManager::CSignalManager()
{
	m_hSignalLib = NULL;

	m_fpSetLibOpen = NULL;
	m_fpSetLibClose = NULL;
	m_fpGetSignalData = NULL;
	m_fpSetSignalData = NULL;
	m_fpGetFilterData = NULL;
	m_fpSetFilterData = NULL;
	m_fpConvertRealData = NULL;
}

//# DRDS에서 Signal DLL을로드한 후 세팅하는 경우
BOOL CSignalManager::SetLibHandle(HMODULE hLib)
{
	m_hSignalLib = hLib;

	if(m_hSignalLib)
		LoadProcAddress();

	return TRUE;
}

void CSignalManager::LoadProcAddress()
{
	if(!m_hSignalLib) return;

	m_fpSetLibOpen = (FpSetLibOpen)::GetProcAddress(m_hSignalLib, "_DLL_SetLibOpen");
	m_fpSetLibClose = (FpSetLibClose)::GetProcAddress(m_hSignalLib, "_DLL_SetLibClose");
	m_fpGetSignalData = (FpGetSignalData)::GetProcAddress(m_hSignalLib, "_DLL_GetSignalData");
	m_fpSetSignalData = (FpSetSignalData)::GetProcAddress(m_hSignalLib, "_DLL_SetSignalData");
	m_fpGetFilterData = (FpGetFilterData)::GetProcAddress(m_hSignalLib, "_DLL_GetFilterData");
	m_fpSetFilterData = (FpSetFilterData)::GetProcAddress(m_hSignalLib, "_DLL_SetFilterData");
	m_fpConvertRealData = (FpConvertRealData)::GetProcAddress(m_hSignalLib, "ConvertRealData");
}

//# Open / Close! => 반드시 Open 후 Colse!
int CSignalManager::_DLL_SetLibOpen(CWnd* pwndMain)
{
	if(!m_fpSetLibOpen) return 0;

	return m_fpSetLibOpen(pwndMain);
}

void CSignalManager::_DLL_SetLibClose(void)
{
	if(!m_fpSetLibClose) return;

	m_fpSetLibClose();
}

//# 신호 설정 관련 함수!
void CSignalManager::_DLL_GetSignalData(CString strType, CMapStringToPtr &mapSignalSet)
{
	if(!m_fpGetSignalData) return;

	m_fpGetSignalData(strType, mapSignalSet);
}

void CSignalManager::_DLL_SetSignalData(CString strType, CMapStringToPtr *pmapSignalSet)
{
	if(!m_hSignalLib) return;

	m_fpSetSignalData(strType, pmapSignalSet);
}

void CSignalManager::_DLL_GetFilterData(long& stFilterInfo)
{
	if(!m_fpGetFilterData) return;

	m_fpGetFilterData(stFilterInfo);
}

void CSignalManager::_DLL_SetFilterData(long* pstFilterInfo)
{
	if(!m_fpSetFilterData) return;

	m_fpSetFilterData(pstFilterInfo);
}

//# DRDS 외부 함수
int CSignalManager::ConvertRealData(int nPacketName, LPSTR &sRealData)
{
	if(!m_fpConvertRealData) return 0;

	return m_fpConvertRealData(nPacketName, sRealData);
}
