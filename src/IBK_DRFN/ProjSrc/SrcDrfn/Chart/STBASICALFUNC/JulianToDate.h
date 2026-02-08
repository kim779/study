// JulianToDate.h: interface for the CJulianToDate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JULIANTODATE_H__9DAE2CB8_D66B_412D_B182_F5FBE848E622__INCLUDED_)
#define AFX_JULIANTODATE_H__9DAE2CB8_D66B_412D_B182_F5FBE848E622__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CJulianToDate   : public CInnerFunction
{
public:
	CJulianToDate();
	virtual ~CJulianToDate();
	CString GetName(long lType);
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_JULIANTODATE_H__9DAE2CB8_D66B_412D_B182_F5FBE848E622__INCLUDED_)
