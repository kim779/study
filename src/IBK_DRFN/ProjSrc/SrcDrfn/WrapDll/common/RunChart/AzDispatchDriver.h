#if !defined(AFX_AZDISPATCHDRIVER_H__95FF7BB7_E274_4DAA_93EF_F33B5631FB7B__INCLUDED_)
#define AFX_AZDISPATCHDRIVER_H__95FF7BB7_E274_4DAA_93EF_F33B5631FB7B__INCLUDED_
//  By Junok Lee, 2003.03.05
// --------------------------------------------------------------------
//	설명 : LPSTR 또는 LPCSTR, LPCTSTR 형를 OLECHAR*형으로 처리하는 루틴
//		WCHAR szuText[80]; // Special treatment for ASCII client
//		LPCSTR szKey="OcxDLL0";
//		::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szKey, -1, szuText, sizeof(szuText));
//		BSTR bstrTemp = ::SysAllocString(szuText);
//		::SysFreeString(bstrTemp);
//
//	mfc의 OLEDisp2.cpp 파일참조
//		void COleDispatchDriver::InvokeHelperV(.....)
//
// --------------------------------------------------------------------
/*	Sample Code
/ --------------------------------------------------------------------
  설명 : 메소드를 호출하는 루틴, Property접근은 각함수의 설명 부분에 포함되어있음.
BOOL YOUR_CLASS_NAME::AddToScriptEngine(LPCSTR szDLLKeyName)
{
	LPDISPATCH pDLLDispatch = m_pCrInfo->m_pDispatch;
	IDispatch* pDeskDispatch = m_pBaseDesk->GetUnknown();

	OLECHAR* szMethod = L"AddUserScriptItem";
	long result=0;
	BYTE parms[] =
		VTS_BSTR VTS_DISPATCH;

	CAzDispatchDriver aDriver(pDeskDispatch);
	aDriver.FInvokeHelper(szMethod, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szDLLKeyName, pDLLDispatch);

	return (BOOL)result;
}

void YOURCLASSNAME::AddToScriptEngine(LPDISPATCH pTargetDisp)
{
	OLECHAR* szMethod = L"AddUserScriptItem";
	long result=0;
	BYTE parms[] =
		VTS_BSTR VTS_DISPATCH;

	CAzDispatchDriver aDriver(pTargetDisp);
	DISPID dispid;
	aDriver.FGetDispid(szMethod, dispid);

	struct
	{
		char *Name;
		LPDISPATCH pDisp;
	} mYData[] = { {"Main", 0},
	{ "PrimLib", 0 } };

	for(int i=0; i<2; i++)
	{
		aDriver.FInvokeHelper(dispid, DISPATCH_METHOD, VT_I4, (void*)&result, parms, 
			mYData[i].Name, mYData[i].pDisp);
	}
//      Method로 직접호출할 경우의 예
//	CAzDispatchDriver aDriver(pDeskDispatch);
//	aDriver.FInvokeHelper(szMethod, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szDLLKeyName, pDLLDispatch);
}


	속성 접근 예제
  	IUnknown *pUnknown = m_pBaseDesk->GetCtrlUnknown("HtcOutput0");
	if(pUnknown)
	{
		LPCTSTR szText = "test";	
		CAzDispatchDriver azDriver(pUnknown);
		azDriver.FSetProperty(DISPID_TEXT, VT_BSTR, szText);
	}
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AzDispatchDriver.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAzDispatchDriver document
#define VT_MFCFORCEPUTREF   0x8000  // force DISPATCH_PROPERTYPUTREF

class CAzDispatchDriver
{
public:
	CAzDispatchDriver()
	{
		m_pDispatch=NULL;
	}	

	CAzDispatchDriver(IUnknown* pUnknown)
	{
		IDispatch *pDisp;
		pUnknown->QueryInterface(IID_IDispatch, (void**)&pDisp);
		pDisp->Release();

		m_pDispatch=pDisp;
		m_DispatchDriver.AttachDispatch(m_pDispatch);
	}

	CAzDispatchDriver(IDispatch* pDispatch)
	{
		m_pDispatch=pDispatch;
		m_DispatchDriver.AttachDispatch(m_pDispatch);
	}

	virtual ~CAzDispatchDriver()
	{
		if(m_pDispatch) m_DispatchDriver.DetachDispatch();
	}

// Attributes
protected:
	LPDISPATCH m_pDispatch;
	COleDispatchDriver	m_DispatchDriver;

public:

// Operations
	void SetDispatch(IDispatch* pDispatch)
	{
		m_pDispatch=pDispatch;
		m_DispatchDriver.AttachDispatch(m_pDispatch);
	}

	IDispatch* GetDispatch()
	{
		return m_pDispatch;
	}

public:

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/3/7
	// 함수 이름          : FGetDispid
	// 리턴 타입          : 	HRESULT
	// 리턴 타입 설명     : 
	// 파라미터
	//		OLECHAR* szKeyName  : 
	//		DISPID &dispid   : 
	// 함수 설명          : DISPID를 구한다.
	///////////////////////////////////////////////////////////////////////////////
	HRESULT FGetDispid(OLECHAR* szKeyName, DISPID &dispid)
	{
		if(m_pDispatch==0)	return S_FALSE;
		HRESULT hr = m_pDispatch->GetIDsOfNames(IID_NULL, &szKeyName, 1, LOCALE_USER_DEFAULT, &dispid);
		return hr;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/3/7
	// 함수 이름          : FInvokeHelper
	// 리턴 타입          : 	void
	// 리턴 타입 설명     : 
	// 파라미터
	//		OLECHAR* szMethod  : 
	//		WORD wFlags  : 
	//		VARTYPE vtRet  : 
	//				void* pvRet  : 
	//		const BYTE* pbParamInfo  : 
	//		...   : 
	//		  : 
	// 함수 설명          : szMethod 이름으로 Function(Method)를 호출한다.
	///////////////////////////////////////////////////////////////////////////////
	void FInvokeHelper(OLECHAR* szMethod, WORD wFlags, VARTYPE vtRet,
		void* pvRet, const BYTE* pbParamInfo, ...)
	{
		DISPID		dwDispID = 0L;
		HRESULT hr = FGetDispid(szMethod, dwDispID);
		if(SUCCEEDED(hr))
		{
			va_list argList;
			va_start(argList, pbParamInfo);
			FInvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo,
				argList);
			va_end(argList);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/3/7
	// 함수 이름          : FInvokeHelper
	// 리턴 타입          : 	void
	// 리턴 타입 설명     : 
	// 파라미터
	//		DISPID dwDispID  : 
	//		WORD wFlags  : 
	//		VARTYPE vtRet  : 
	//				void* pvRet  : 
	//		const BYTE* pbParamInfo  : 
	//		...   : 
	// 함수 설명          : DISPID로 Function(Method)를 호출한다.
	///////////////////////////////////////////////////////////////////////////////
	void FInvokeHelper(DISPID dwDispID, WORD wFlags, VARTYPE vtRet,
		void* pvRet, const BYTE* pbParamInfo, ...)
	{
		va_list argList;
		va_start(argList, pbParamInfo);
		FInvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo,
			argList);
		va_end(argList);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/3/7
	// 함수 이름          : FGetProperty
	// 리턴 타입          : 	void
	// 리턴 타입 설명     : 
	// 파라미터
	//		OLECHAR* szProperty  : 
	//		VARTYPE vtProp  : 
	//		void* pvProp   : 
	// 함수 설명          : szProperty 이름으로 속성값을 덛어온다.
	//	Sample
	//OLECHAR* wszFnName = L"SAppRegKey";
	//CString result;
	//FGetProperty(wszFnName, VT_BSTR, (void*)&result);
	///////////////////////////////////////////////////////////////////////////////
	void FGetProperty(OLECHAR* szProperty, VARTYPE vtProp, void* pvProp)
	{
		DISPID		dwDispID = 0L;
		HRESULT hr = FGetDispid(szProperty, dwDispID);

		FGetProperty(dwDispID, vtProp, pvProp);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/3/7
	// 함수 이름          : FGetProperty
	// 리턴 타입          : 	void
	// 리턴 타입 설명     : 
	// 파라미터
	//		DISPID dwDispID  : 
	//			VARTYPE vtProp  : 
	//		void* pvProp   : 
	// 함수 설명          : DISPID로 속성값을 덛어온다.
	///////////////////////////////////////////////////////////////////////////////
	void FGetProperty(DISPID dwDispID,	VARTYPE vtProp, void* pvProp)
	{
		FInvokeHelper(dwDispID, DISPATCH_PROPERTYGET,vtProp, pvProp, NULL);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/3/7
	// 함수 이름          : FSetProperty
	// 리턴 타입          : 	void
	// 리턴 타입 설명     : 
	// 파라미터
	//		OLECHAR* szProperty  : 
	//		VARTYPE vtProp  : 
	//		...   : 
	// 함수 설명          : szProperty 이름으로 속성값을 세팅한다.
	//	Sample
	//LPCSTR propVal = "DeskPath";
	//CString result;
	//FSetProperty(wszFnName, VT_BSTR, propVal);
	///////////////////////////////////////////////////////////////////////////////
	void FSetProperty(OLECHAR* szProperty, VARTYPE vtProp, ...)
	{
		DISPID		dwDispID = 0L;
		HRESULT hr = FGetDispid(szProperty, dwDispID);

		va_list argList;    // really only one arg, but...
		va_start(argList, vtProp);
		FSetPropertyV(dwDispID, vtProp, argList);
		va_end(argList);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/3/7
	// 함수 이름          : FSetProperty
	// 리턴 타입          : 	void
	// 리턴 타입 설명     : 
	// 파라미터
	//		DISPID dwDispID  : 
	//		VARTYPE vtProp  : 
	//		...   : 
	// 함수 설명          : DISPID로 속성값을 세팅한다.
	///////////////////////////////////////////////////////////////////////////////
	void FSetProperty(DISPID dwDispID, VARTYPE vtProp, ...)
	{
		va_list argList;    // really only one arg, but...
		va_start(argList, vtProp);
		FSetPropertyV(dwDispID, vtProp, argList);
		va_end(argList);
	}

public:
	// 아래 부분은 내부적으로 콜되는 함수 이므로 직접호출 할 필요가 있을 경우에만 사용한다.
	// void FSetPropertyV(DISPID dwDispID, VARTYPE vtProp, va_list argList)
	// void FInvokeHelperV(DISPID dwDispID, WORD wFlags, VARTYPE vtRet, void* pvRet, const BYTE* pbParamInfo, va_list argList);

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/3/7
	// 함수 이름          : FSetPropertyV
	// 리턴 타입          : void
	// 파라미터
	//		DISPID dwDispID  : 
	//		VARTYPE vtProp  : 
	//		va_list argList   : 
	// 함수 설명          : 속성값을 세팅하는 루틴
	///////////////////////////////////////////////////////////////////////////////
	void FSetPropertyV(DISPID dwDispID, VARTYPE vtProp, va_list argList)
	{
		BYTE rgbParams[2];
		if (vtProp & VT_BYREF)
		{
			vtProp &= ~VT_BYREF;
			vtProp |= VT_MFCBYREF;
		}
		
		#if !defined(_UNICODE) && !defined(OLE2ANSI)
				if (vtProp == VT_BSTR)
					vtProp = VT_BSTRA;
		#endif

		WORD wFlags;
		if (vtProp & VT_MFCFORCEPUTREF)
		{
			wFlags = DISPATCH_PROPERTYPUTREF;
			vtProp &= ~VT_MFCFORCEPUTREF;
		}
		else
		{
			if (vtProp == VT_DISPATCH)
				wFlags = DISPATCH_PROPERTYPUTREF;
			else
				wFlags = DISPATCH_PROPERTYPUT;
		}
		
		rgbParams[0] = (BYTE)vtProp;
		rgbParams[1] = 0;
		FInvokeHelperV(dwDispID, wFlags, VT_EMPTY, NULL, rgbParams, argList);
	}

	///////////////////////////////////////////////////////////////////////////////
	// 만든이             : 이준옥(Jun-Ok, Lee)
	// Email              : alzioyes@hotmail.com
	// 만든 날짜          : 2003/3/7
	// 함수 이름          : FInvokeHelperV
	// 리턴 타입          : 	void
	// 리턴 타입 설명     : 
	// 파라미터
	//		DISPID dwDispID  : 
	//		WORD wFlags  : 
	//				VARTYPE vtRet  : 
	//		void* pvRet  : 
	//		const BYTE* pbParamInfo  : 
	//		va_list argList   : 
	// 함수 설명          : 실제적인 IDispatch의 Invoke를 호출하는 루틴
	///////////////////////////////////////////////////////////////////////////////
	void FInvokeHelperV(DISPID dwDispID, WORD wFlags,
		VARTYPE vtRet, void* pvRet, const BYTE* pbParamInfo, va_list argList)
	{
		if(m_pDispatch==0) return;

		if(!m_pDispatch)	m_DispatchDriver.AttachDispatch(m_pDispatch);
		
		m_DispatchDriver.InvokeHelperV(dwDispID, wFlags, vtRet, pvRet, pbParamInfo, argList);
	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AZDISPATCHDRIVER_H__95FF7BB7_E274_4DAA_93EF_F33B5631FB7B__INCLUDED_)
