// IndiConfigurationDefineData.h: interface for the CIndiConfigurationDefineData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICONFIGURATIONDEFINEDATA_H__40E83F29_5F96_4C72_98A0_3EDBEEFFC0FC__INCLUDED_)
#define AFX_INDICONFIGURATIONDEFINEDATA_H__40E83F29_5F96_4C72_98A0_3EDBEEFFC0FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIndiConfigurationDefineData  
{
public:
	static const char* GROUPNAME_TOTAL;
	static const char* GROUPNAME_SPECIAL;
	static const char* GROUPNAME_MARKET;
	static const char* GROUPNAME_INVESTOR;

	static const char* INDINAME_DEPOSIT;
	static const char* INDINAME_LOAN;
	static const char* INDINAME_UNCOLLECTED;
	static const char* INDINAME_FOREIGNER_SHARE;
	static const char* INDINAME_FOREIGNER_BUY;
	static const char* INDINAME_FO_UNAPPROVAL;
	static const char* INDINAME_FO_PRICE;

	static bool IsIndicatorNameInStocksDaily(const CString& strIndicatorName);
};

#endif // !defined(AFX_INDICONFIGURATIONDEFINEDATA_H__40E83F29_5F96_4C72_98A0_3EDBEEFFC0FC__INCLUDED_)
