#if !defined(AFX_SYSTEMTRADERHELPER_H__8F08A0CC_11C5_4766_AAD5_2FCD816D48BC__INCLUDED_)
#define AFX_SYSTEMTRADERHELPER_H__8F08A0CC_11C5_4766_AAD5_2FCD816D48BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LoadDll.h"
#include "PacketList.h"
#include "IndicatorList.h"
#include "SystemList.h"

//////////////////////////////////////////////////////////////////////////////
typedef int (WINAPI *fpShowSystemTrader)(CWnd*);
typedef int (WINAPI *fpSetInitialData)(CPacketList* packetList, CIndicatorList* IndicatorList, CSystemList* systemList, LPCTSTR pcszFlag);
typedef int (WINAPI *fpShowDayTrading)(CWnd*);
typedef int (WINAPI *fpSetInitialDayTrading)(CPacketList* packetList, CIndicatorList* IndicatorList, CSystemList* systemList);
typedef int (WINAPI *fpRealDayTrading)(CWnd*);
typedef int (WINAPI *fpShowSystemTradeIndicator)(CWnd*);
typedef int (WINAPI *fpDeleteDayTrading)();
typedef int (WINAPI *fpDayTradeChangeData)();

class CSystemTraderHelper : public CLoadDllLib
{
public:
	fpShowSystemTrader			m_pfpShowSystemTrader;
	fpSetInitialData			m_pfpSetInitialData;
	fpShowDayTrading			m_pfpShowDayTrading;
	fpSetInitialDayTrading		m_pfpSetInitialDayTrading;
	fpRealDayTrading			m_pfpRealDayTrading;
	fpShowSystemTradeIndicator  m_pfpShowSystemTradeIndicator;
	fpDeleteDayTrading          m_pfpDeleteDayTrading;
	fpDayTradeChangeData        m_pfpDayTradeChangeData;

	BOOL					m_delAuto;

	CSystemTraderHelper(LPCSTR _szDll)
	{
		m_delAuto = TRUE;
		if(_szDll)
			m_szDllName.Format("%s%s", _szDll,"SystemTrader.dll");
		else
			m_szDllName = _T("SystemTrader.dll");
		m_hLib = NULL;
		MakeNull(m_pfpShowSystemTrader);
		MakeNull(m_pfpSetInitialData);

		MakeNull(m_pfpShowDayTrading);
		MakeNull(m_pfpShowSystemTradeIndicator);
		MakeNull(m_pfpSetInitialDayTrading);
		MakeNull(m_pfpDeleteDayTrading);
		MakeNull(m_pfpDayTradeChangeData);
		LoadLib();
	}

	CSystemTraderHelper(BOOL deleteAuto)
	{
		m_szDllName = _T("SystemTrader.dll");
		m_hLib = NULL;
		MakeNull(m_pfpShowSystemTrader);
		MakeNull(m_pfpShowSystemTradeIndicator);
		MakeNull(m_pfpSetInitialData);

		MakeNull(m_pfpShowDayTrading);
		MakeNull(m_pfpSetInitialDayTrading);
		MakeNull(m_pfpDeleteDayTrading);
		MakeNull(m_pfpDayTradeChangeData);
		LoadLib();

		m_delAuto = deleteAuto;
	}

	~CSystemTraderHelper()
	{
	}

	int ShowSystemTrader(CWnd *pwnd)
	{
		if(!m_hLib) 
			return NULL;

		m_pfpShowSystemTrader = (fpShowSystemTrader)::GetProcAddress(m_hLib, "ShowSystemTrader");
		if(m_pfpShowSystemTrader==NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return NULL;
		}

		int nRet = m_pfpShowSystemTrader(pwnd);
		MAKESTATE(LDS_OK, LDR_READY);
		return nRet;
	}

	int ShowDayTrading(CWnd *pwnd)
	{
		if(!m_hLib) 
			return NULL;

		m_pfpShowDayTrading = (fpShowDayTrading)::GetProcAddress(m_hLib, "ShowDayTrading");
		if(m_pfpShowDayTrading==NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return NULL;
		}

		int nRet = m_pfpShowDayTrading(pwnd);
		MAKESTATE(LDS_OK, LDR_READY);
		return nRet;
	}
	int RealDayTrading(CWnd *pwnd)
	{
		if(!m_hLib)
			return NULL;
		m_pfpRealDayTrading = (fpRealDayTrading)::GetProcAddress(m_hLib, "RealDayTrading");
		if(m_pfpRealDayTrading == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return NULL;
		}
		int nRet = m_pfpRealDayTrading(pwnd);
		MAKESTATE(LDS_OK, LDR_READY);
		return nRet;
	}
	int DeleteDayTrading()
	{
		if(!m_hLib)
			return NULL;
		m_pfpDeleteDayTrading = (fpDeleteDayTrading)::GetProcAddress(m_hLib, "DeleteDayTrading");
		if(m_pfpDeleteDayTrading == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return NULL;
		}
		int nRet = m_pfpDeleteDayTrading();
		MAKESTATE(LDS_OK, LDR_READY);
		return nRet;
	}
	int DayTradeChangeData()
	{
		if(!m_hLib)
			return NULL;
		m_pfpDayTradeChangeData = (fpDayTradeChangeData)::GetProcAddress(m_hLib, "DayTradeChangeData");
		if(m_pfpDayTradeChangeData == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return NULL;
		}
		int nRet = m_pfpDayTradeChangeData();
		MAKESTATE(LDS_OK, LDR_READY);
		return nRet;
	}
	int SetInitialData(CPacketList* packetList, CIndicatorList* IndicatorList, CSystemList* systemList, LPCTSTR pcszFlag)
	{
		if(!m_hLib) 
			return NULL;

		m_pfpSetInitialData	= (fpSetInitialData)::GetProcAddress(m_hLib, "SetInitialData");
		if(m_pfpSetInitialData==NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return NULL;
		}

		int nCheck = m_pfpSetInitialData(packetList, IndicatorList, systemList, pcszFlag);
		MAKESTATE(LDS_OK, LDR_READY);
		return nCheck;
	}

	int SetInitialDayTrading(CPacketList* packetList, CIndicatorList* IndicatorList, CSystemList* systemList)
	{
		if(!m_hLib) 
			return NULL;

		m_pfpSetInitialDayTrading	= (fpSetInitialDayTrading)::GetProcAddress(m_hLib, "SetInitialDayTrading");
		if(m_pfpSetInitialDayTrading==NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return NULL;
		}

		int nCheck = m_pfpSetInitialDayTrading(packetList, IndicatorList, systemList);
		MAKESTATE(LDS_OK, LDR_READY);
		return nCheck;
	}
	int ShowSystemTradeIndicator(CWnd *pwnd)
	{
		if(!m_hLib) 
			return NULL;

		m_pfpShowSystemTradeIndicator = (fpShowSystemTradeIndicator)::GetProcAddress(m_hLib, "ShowSystemTradeIndicator");
		if(m_pfpShowSystemTradeIndicator==NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return NULL;
		}

		int nRet = m_pfpShowSystemTradeIndicator(pwnd);
		MAKESTATE(LDS_OK, LDR_READY);
		return nRet;
	}
};

#endif // !defined(AFX_CHARTPROPPAGEHELPER_H__8F08A0CC_11C5_4766_AAD5_2FCD816D48BC__INCLUDED_)
