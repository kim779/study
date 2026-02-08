// StockPriceDiscretionHelper.h: interface for the CStockPriceDeiscretionHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOCKPRICEDISCRETIONHELPER_H__F6EB31E2_10A7_11D6_9374_0050FC28B229__INCLUDED_)
#define AFX_STOCKPRICEDISCRETIONHELPER_H__F6EB31E2_10A7_11D6_9374_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "LoadDll.h"
//////////////////////////////////////////////////////////////////////////////

class CPacketList;
typedef void (WINAPI *fpLoadStockPriceDiscretion)(CWnd*, CPacketList*);

class CStockPriceDiscretionHelper : public CLoadDllLib  
{
public:
	fpLoadStockPriceDiscretion m_fpLoadStockPriceDiscretion;

	BOOL m_delAuto;
	CStockPriceDiscretionHelper(LPCSTR _szDll)
	{
		m_delAuto = TRUE;
		if(_szDll)
			m_szDllName.Format("%s%s", _szDll,"StockPriceDiscretion.dll");
		else
			m_szDllName = _T("StockPriceDiscretion.dll");
		m_hLib = NULL;
		MakeNull(m_fpLoadStockPriceDiscretion);
		LoadLib();
	}

	CStockPriceDiscretionHelper(BOOL deleteAuto)
	{
		m_delAuto = TRUE;
		m_szDllName = _T("StockPriceDiscretion.dll");
		m_hLib = NULL;
		MakeNull(m_fpLoadStockPriceDiscretion);
		LoadLib();

		m_delAuto = deleteAuto;
	}

	CStockPriceDiscretionHelper()
	{
		m_delAuto = TRUE;
		m_szDllName = _T("StockPriceDiscretion.dll");
		m_hLib = NULL;
		MakeNull(m_fpLoadStockPriceDiscretion);
		LoadLib();
	}

	~CStockPriceDiscretionHelper()
	{
	}

	int LoadStockPriceDiscretion(CWnd* pOcxWnd, CPacketList * pPacketList)
	{
		if(!m_hLib) 
			return m_bState;//CS_LOADERROR;

		m_fpLoadStockPriceDiscretion = (fpLoadStockPriceDiscretion)::GetProcAddress(m_hLib, "LoadStockPriceDiscretion");
		if(m_fpLoadStockPriceDiscretion == NULL){
			MAKESTATE(LDS_FAIL, LDR_GETPROCADDRESS);
			return GetState();//CS_LOADERROR;
		}
		m_fpLoadStockPriceDiscretion(pOcxWnd, pPacketList);
		MAKESTATE(LDS_OK, LDR_READY);
		return GetState();//nRet;
	}

};

#endif // !defined(AFX_STOCKPRICEDISCRETIONHELPER_H__F6EB31E2_10A7_11D6_9374_0050FC28B229__INCLUDED_)
