#include "stdafx.h"
#include "DrChartCtl.h"

BOOL CDrChartCtl::TrimLR( char *&p_szItem, int p_nItemLen)
{
	if( !p_szItem) return FALSE;
	char *szItemEnd = p_szItem + p_nItemLen - 1;
	// 1. Trim Left
	while( *p_szItem && _istspace( *p_szItem)) p_szItem++;	// Cuation! Do not use isspace(), Use _istspace() for MBCS
	// 2. Trim Right
	if( *szItemEnd)
	{
		while( *szItemEnd && _istspace( *szItemEnd)) szItemEnd--;	// Cuation! Do not use isspace(), Use _istspace() for MBCS
		szItemEnd[ 1] = '\0';	// Trim Right
	}
	return TRUE;
}

BOOL CDrChartCtl::Chart_Method( const char *p_szMethodName, const char *p_szParams, char *p_szReturn)
{
	if( !m_pIChartDisp) return FALSE;

	// 1. Get Function DISPID (Check Suppoerted Function)
	DISPID 	dispid = 0L;
	HRESULT hResult = GetIDsOfNames( p_szMethodName, dispid);
	// 1.3 Check Not Suppoerted Function
	if( FAILED( hResult))
	{
		CString strMsg;
		strMsg.Format( "This Method [%s] is not supported in euplChart!\nPlease, Check again in Chart OCX's wrapper class header.",
			p_szMethodName);
		AfxMessageBox( strMsg);
		return FALSE;
	}

	// 2. Make Param String Array
	// 2.1 Copy Original for Parsing
	CString strParam;
	int nParamsLen = strlen( p_szParams);
	char *szParam = strParam.GetBufferSetLength( nParamsLen + 1);		// 1 for NULL of Next Parameter (see 2.2)
	strcpy( szParam, p_szParams);
	// 2.1.5 Prepare Param End Position for Next Param Checker (see 2.2)
	char *szParamEnd = szParam + nParamsLen + 1;
	*szParamEnd = '\0';
	// 2.2 Parse Parameter (Cuation! It is need to TrimLeft & Right for Parameter Type Auto Conversion.)
	CStringArray saParam;
	char *szParamRightTrim = NULL;
	do
	{
		BOOL bIsQuatParam = FALSE;
		char *szNextParam = strchr( szParam, ',');
		if( szNextParam)
		{
			char *szBQuat = strchr( szParam, '"');
			// "" Param
			if(szBQuat >= szParam && szBQuat < szNextParam)
			{
				bIsQuatParam = TRUE;

				szParam = szBQuat + 1;
				char *szEQuat = strchr( szParam, '"');
				szNextParam = szEQuat;
			}
			*szNextParam = '\0';
			szNextParam++;
		}
		else szNextParam = szParamEnd;

		// 2.2.5 Trim Space of Parameter
		TrimLR( szParam, szNextParam - szParam - 1);

		saParam.Add( szParam);	// Set Parameter without checking whether it is empty. Becuase It must be setted in Order!

		if(bIsQuatParam)
		{
			szNextParam = strchr( szNextParam, ',');
			if( szNextParam)
			{
				*szNextParam = '\0';
				szNextParam++;
			}
			else szNextParam = szParamEnd;
		}
		szParam = szNextParam;
	} while( *szParam);			// szParam can not be NULL. but *szParam must be NULL in last Parameter.
	// 2.3 Check Parameter Count
	int nParamCount = saParam.GetSize();
	if( NMPARAM_MAXCOUNT < nParamCount)
	{
		CString strMsg;
		strMsg.Format( "Too many parameters!\nPlease, Check again in [%s(%s)] or Increase Buffer Count [NMPARAM_MAXCOUNT] in DRChartObj.h",
			p_szMethodName, p_szParams);
		AfxMessageBox( strMsg);
		return FALSE;
	}
	// 2.4 Check No Parameter
	if( 1 == nParamCount && saParam[ 0].IsEmpty()) nParamCount = 0;

	// 3. Prepare Parameter & Result Variable
	// 3.1 Basic Preparing
	// 3.1.1 Result Variable
	VARIANT		vRet;
	VariantInit( &vRet);
	vRet.vt = VT_VARIANT;
	// 3.1.2 Parameter Variable
	VARIANTARG	var[ NMPARAM_MAXCOUNT];
	for( int i = 0; i < nParamCount; i++) VariantInit( &var[ i]);
	// 3.1.3 Parameter Info.
	DISPPARAMS	dispparams;
	dispparams.rgvarg = ( VARIANTARG *)var;
	dispparams.cNamedArgs = 0;
	// 3.2 Data Preparing (Cuation! Reverse order!)
	// 3.2.1 Set Param Count
	dispparams.cArgs = nParamCount;
	// 3.2.2 Set Data (Cuation! Reverse order!)
	for( int i = 0; i < nParamCount; i++)
	{
		var[ i].vt = VT_BSTR;
		var[ i].bstrVal = saParam[ nParamCount -1 - i].AllocSysString();	// (Cuation! Reverse order!)
	}

	// 4. Call Chart OCX Method
	UINT		nErr;
	EXCEPINFO	excep;
	hResult = m_pIChartDisp->Invoke( dispid, IID_NULL, GetUserDefaultLCID(), DISPATCH_METHOD,
		&dispparams, (VARIANT FAR*)&vRet, (EXCEPINFO FAR*)&excep, &nErr);
	// 4.1 Check Error
	if( FAILED( hResult))
	{
		// 4.1.1 Make Error Message
		CString strMsg;
		switch( hResult)
		{
		case DISP_E_BADPARAMCOUNT:
		case DISP_E_PARAMNOTOPTIONAL:	strMsg = "Parameter Count Error!";
			break;
		case DISP_E_BADVARTYPE:
		case DISP_E_OVERFLOW:
		case DISP_E_PARAMNOTFOUND:
		case DISP_E_TYPEMISMATCH:	strMsg.Format( "Invalide Data Type of Param%d!", nErr + 1);
			break;
		case DISP_E_EXCEPTION:	strMsg = "Exception Error in Chart OCX!";
			break;
		case DISP_E_MEMBERNOTFOUND:	strMsg = "Method not found in Chart!";
			break;
		default:	strMsg = "Method Invoking Error with Unknown Type!";
			break;
		}
		// 4.1.2 Set Method Sentence
		CString strCheck;
		strCheck.Format( "\nPlease, Check again in [%s(%s)].", p_szMethodName, p_szParams);
		strMsg += strCheck;
		// 4.1.3 Set Error Code
		switch( hResult)
		{
		case DISP_E_BADPARAMCOUNT:		strMsg += "\nError Code : [DISP_E_BADPARAMCOUNT]";	break;
		case DISP_E_PARAMNOTOPTIONAL:	strMsg += "\nError Code : [DISP_E_PARAMNOTOPTIONAL]";	break;
		case DISP_E_BADVARTYPE:			strMsg += "\nError Code : [DISP_E_BADVARTYPE]";	break;
		case DISP_E_OVERFLOW:			strMsg += "\nError Code : [DISP_E_OVERFLOW]";	break;
		case DISP_E_PARAMNOTFOUND:		strMsg += "\nError Code : [DISP_E_PARAMNOTFOUND]";	break;
		case DISP_E_TYPEMISMATCH:		strMsg += "\nError Code : [DISP_E_TYPEMISMATCH]";	break;
		case DISP_E_EXCEPTION:			strMsg += "\nError Code : [DISP_E_EXCEPTION]";	break;
		case DISP_E_MEMBERNOTFOUND:		strMsg += "\nError Code : [DISP_E_MEMBERNOTFOUND]";	break;
		default:
			{
				CString strErrCode;
				strErrCode.Format( "\nError Code : [0x%08X]", hResult);
				strMsg += strErrCode;
				break;
			}
		}
		AfxMessageBox( strMsg);
		return FALSE;
	}

	// 5. Make Return Value with String
	VARIANT		vRetString;
	VariantInit( &vRetString);
	VariantChangeType( &vRetString, &vRet, 0, VT_BSTR);
	CString strResult;
	strResult = vRetString.bstrVal;
	if( NMRETURN_MAXLENGTH < strResult.GetLength())
	{
		CString strMsg;
		strMsg.Format( "Return value of [%s(%s)] is too long.\nIt was trancated in %d by DRChartObj.h",
			p_szMethodName, p_szParams, NMRETURN_MAXLENGTH);
		AfxMessageBox( strMsg);
	}

	strncpy( p_szReturn, strResult, NMRETURN_MAXLENGTH);

	return TRUE;
}

HRESULT	CDrChartCtl::GetIDsOfNames( const char *p_szProperty, DISPID &p_diProperty)
{
	if( !m_pIChartDisp) return -1;
	// 1.1 Make OLECHAR Function Name
	USES_CONVERSION;
	OLECHAR *wszPropertyName = ( OLECHAR *)T2COLE( p_szProperty);
	// 1.2 Get Function DISPID
	return m_pIChartDisp->GetIDsOfNames( IID_NULL, &wszPropertyName, 1, LOCALE_USER_DEFAULT, &p_diProperty);
}

BOOL CDrChartCtl::Chart_PropertyGet( const char *p_szProperty, DISPID p_diProperty, char *p_szReturn)
{
	if( !m_pIChartDisp) return FALSE;
	
	// 3. Prepare Parameter & Result Variable
	int nParamCount = 0;
	// 3.1 Basic Preparing
	// 3.1.1 Result Variable
	VARIANT		vRet;
	VariantInit( &vRet);
	vRet.vt = VT_VARIANT;
	// 3.1.2 Parameter Variable
	VARIANTARG	var[ 1];
	VariantInit( &var[ 0]);
	// 3.1.3 Parameter Info.
	DISPPARAMS	dispparams;
	dispparams.rgvarg = ( VARIANTARG *)var;
	dispparams.cNamedArgs = 0;
	// 3.2 Data Preparing (Cuation! Reverse order!)
	// 3.2.1 Set Param Count
	dispparams.cArgs = nParamCount;
	
	// 4. Call Chart OCX PropertyGet
	UINT		nErr;
	EXCEPINFO	excep;
	HRESULT hResult = m_pIChartDisp->Invoke( p_diProperty, IID_NULL, GetUserDefaultLCID(), DISPATCH_PROPERTYGET,
		&dispparams, (VARIANT FAR*)&vRet, (EXCEPINFO FAR*)&excep, &nErr);
	if( FAILED( hResult))
	{
		CString strMsg;
		strMsg.Format( "The PropertyGet Error of [%s].\nPlease, Check again!", p_szProperty);
		AfxMessageBox( strMsg);
		return FALSE;
	}
	
	// 5. Make Return Value with String
	VARIANT		vRetString;
	VariantInit( &vRetString);
	VariantChangeType( &vRetString, &vRet, 0, VT_BSTR);
	CString strResult;
	strResult = vRetString.bstrVal;
	if( NMRETURN_MAXLENGTH < strResult.GetLength())
	{
		CString strMsg;
		strMsg.Format( "The Property Value [%s] is too long.\nIt was trancated in %d by DRChartObj.h",
			p_szProperty, NMRETURN_MAXLENGTH);
		AfxMessageBox( strMsg);
	}
	strncpy( p_szReturn, strResult, NMRETURN_MAXLENGTH);
	
	return TRUE;
}

#include "./RunChart/AzDispatchDriver.h"
BOOL CDrChartCtl::Chart_PropertySet( const char *p_szProperty, DISPID p_diProperty, const char *p_szValue, const char *p_szValueType)
{
	CAzDispatchDriver azDispatch;
	azDispatch.SetDispatch(m_pIChartDisp);
	
	//	OLECHAR szProperty;
	//	MultiByteToWideChar(CP_ACP, 0, p_szProperty, -1, &szProperty, strlen(p_szProperty));
	VARTYPE varType;
	//>> sizeof(p_szValueType) => strlen(p_szValueType) by Á¤¿ëÈ£ 20090901
	if(memcmp(p_szValueType, "2", strlen(p_szValueType)) == 0) 
	{
		varType = VT_I2;
		azDispatch.FSetProperty(p_diProperty, varType, atoi(p_szValue));
	}
	else if(memcmp(p_szValueType, "3", strlen(p_szValueType)) == 0)
	{
		varType = VT_I4;
		azDispatch.FSetProperty(p_diProperty, varType, atol(p_szValue));
	}
	else if(memcmp(p_szValueType, "8", strlen(p_szValueType)) == 0)
	{
		varType = VT_BSTR;
		azDispatch.FSetProperty(p_diProperty, varType, CString( p_szValue).AllocSysString());
	}
	else if(memcmp(p_szValueType, "11", strlen(p_szValueType)) == 0)
	{
		varType = VT_BOOL;
		azDispatch.FSetProperty(p_diProperty, varType, atoi(p_szValue));
	}
	else
	{
		varType = VT_EMPTY;
		azDispatch.FSetProperty(p_diProperty, varType);
	}
	//<<
	
	return TRUE;
}

CString CDrChartCtl::TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/)
{
	CString strRes = _T("");
	int iCur = iStart;
	iStart = strSrc.Find(pszTokens, iCur);
	if(iStart != -1)
	{
		strRes = strSrc.Mid(iCur, iStart-iCur);
		iStart++;
		return strRes;
	}
	return strRes;
}

