// IntPortion.h: interface for the CIntPortion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTPORTION_H__AEB3C781_03AE_4470_AC04_ADAE0F906E9C__INCLUDED_)
#define AFX_INTPORTION_H__AEB3C781_03AE_4470_AC04_ADAE0F906E9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CIntPortion   : public CInnerFunction
{
public:
	CIntPortion();
	virtual ~CIntPortion();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController ,  long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_INTPORTION_H__AEB3C781_03AE_4470_AC04_ADAE0F906E9C__INCLUDED_)
