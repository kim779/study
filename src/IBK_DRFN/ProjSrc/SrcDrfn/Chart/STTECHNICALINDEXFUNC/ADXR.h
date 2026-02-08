// ADXR.h: interface for the CADXR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADXR_H__41632BD2_F04E_4754_8844_15F667C5AADC__INCLUDED_)
#define AFX_ADXR_H__41632BD2_F04E_4754_8844_15F667C5AADC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CADXR  : public CInnerFunction
{
public:
	CADXR();
	virtual ~CADXR();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_ADXR_H__41632BD2_F04E_4754_8844_15F667C5AADC__INCLUDED_)
