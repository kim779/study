// DayLow.h: interface for the CDayLow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAYLOW_H__F441738D_47B1_4FB9_B4AD_42CFEE8DE2AD__INCLUDED_)
#define AFX_DAYLOW_H__F441738D_47B1_4FB9_B4AD_42CFEE8DE2AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CDayLow  : public CInnerFunction
{
public:
	CDayLow();
	virtual ~CDayLow();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_DAYLOW_H__F441738D_47B1_4FB9_B4AD_42CFEE8DE2AD__INCLUDED_)
