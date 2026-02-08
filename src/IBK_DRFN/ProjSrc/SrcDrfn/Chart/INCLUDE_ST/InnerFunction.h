// InnerFunction.h: interface for the CInnerFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INNERFUNCTION_H__76D8BCF0_270C_4CB8_9736_07034F0DD303__INCLUDED_)
#define AFX_INNERFUNCTION_H__76D8BCF0_270C_4CB8_9736_07034F0DD303__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/ISTController.h"
#include "../Common_ST/STControllerDef.h"
class CISTController;
#define CInnerFunction CInnerFunction0001
class CInnerFunction0001  
{
public:
	virtual ~CInnerFunction0001() {}
	virtual CString GetName(long lType) = 0;
	virtual CString GetInputs(long lType) = 0;
	virtual double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10) = 0;
};

#endif // !defined(AFX_INNERFUNCTION_H__76D8BCF0_270C_4CB8_9736_07034F0DD303__INCLUDED_)
