// MRO.h: interface for the CMRO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MRO_H__FF7E9184_6A38_4CE0_81A5_1F4BE730C04D__INCLUDED_)
#define AFX_MRO_H__FF7E9184_6A38_4CE0_81A5_1F4BE730C04D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"
class CMRO   : public CInnerFunction
{
public:
	CMRO();
	virtual ~CMRO();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_MRO_H__FF7E9184_6A38_4CE0_81A5_1F4BE730C04D__INCLUDED_)
