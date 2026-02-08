#if !defined(AFX_DTSPFORMDATA_H__BY_JUNOK_LEE__20051228_57ECF7257A__INCLUDED_)
#define AFX_DTSPFORMDATA_H__BY_JUNOK_LEE__20051228_57ECF7257A__INCLUDED_

// #include "../../_Include/DispFormData.h"

/////////////////////////////////////////////////////////////////////////////
// CDispFormData wrapper class

class CDispFormData : public COleDispatchDriver
{
public:
	CDispFormData();
	
//{{ Dispid 자동처리
public:
	LPDISPATCH m_pdispatch;

	// 1. 변수선언. m_pdID, m_osInterfaceName[]
	DISPID*			m_pdID;
	static OLECHAR *	m_osInterfaceName[];

	// 2. enum 추가, 필요한 것만 리스트업.
	typedef enum {
		// Variable
//		IIx_Rows=0,

		// Method
		IIx_SaveData=0,
		IIx_LoadData,
		IIx_SaveIniData,
		IIx_LoadIniData,
		IIx_SaveStringDataToCfg,
		IIx_SaveIntDataToCfg,
		IIx_LoadIntDataInCfg,
		IIx_GetMapCfgFile,
		IIx_GetUserDataPath,
		IIx_WritePrivateDataString,
		IIx_GetPrivateDataString,
		IIx_WritePrivateDelete,
		IIx_LoadStringDataInCfg,	
	};

	// 3. void InitInterfaceName(IDispatch *pDisp);
	void InitInterfaceName(IDispatch *pDisp);

	// 4.
//	COleDispatchDriver m_dispDriver;

//}} Dispid자동처리
public:
// Attributes

// Operations
	BOOL SaveData(LPCTSTR _szFileName, LPCTSTR _szKey, const VARIANT FAR& _szValue);
	BOOL LoadData(LPCTSTR _szFileName, LPCTSTR _szKey, const VARIANT FAR& _szValue);
	BOOL SaveIniData(LPCTSTR _szFileName, LPCTSTR _szSection, LPCTSTR _szKey, const VARIANT FAR& _szValue);
	BOOL LoadIniData(LPCTSTR _szFileName, LPCTSTR _szSection, LPCTSTR _szKey, VARIANT FAR* _szValue);
	BOOL SaveStringDataToCfg(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue);
	BOOL SaveIntDataToCfg(LPCTSTR szSection, LPCTSTR szKey, short nValue);
	BOOL LoadIntDataInCfg(LPCTSTR szSection, LPCTSTR szKey, short nDefault, VARIANT FAR* nValue);
	CString GetMapCfgFile();
	CString GetUserDataPath(LPCTSTR szKey);
	short WritePrivateDataString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szData, LPCTSTR szFile);
	CString GetPrivateDataString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault, LPCTSTR szFile);
	void WritePrivateDelete(LPCTSTR szSection, LPCTSTR szFile);
	CString LoadStringDataInCfg(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault);


// Implementation
};

#endif // !defined(AFX_DTSPFORMDATA_H__BY_JUNOK_LEE__20051228_57ECF7257A__INCLUDED_)

