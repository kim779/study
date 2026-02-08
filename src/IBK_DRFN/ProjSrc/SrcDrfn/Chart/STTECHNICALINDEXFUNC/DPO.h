// DPO.h: interface for the CDPO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DPO_H__E7B2C742_C217_4B35_92B0_0AFE9C32F297__INCLUDED_)
#define AFX_DPO_H__E7B2C742_C217_4B35_92B0_0AFE9C32F297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CDPO  : public CInnerFunction
{
public:
	CDPO();
	virtual ~CDPO();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_DPO_H__E7B2C742_C217_4B35_92B0_0AFE9C32F297__INCLUDED_)
