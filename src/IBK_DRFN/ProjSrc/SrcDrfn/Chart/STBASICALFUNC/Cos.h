// Cos.h: interface for the CCos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COS_H__3E030973_398A_412B_B3C2_A331C12786A8__INCLUDED_)
#define AFX_COS_H__3E030973_398A_412B_B3C2_A331C12786A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CCos    : public CInnerFunction
{
public:
	CCos();
	virtual ~CCos();
	CString GetName(long lType);
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName0;//TS
	CString m_strName1;//YES

};

#endif // !defined(AFX_COS_H__3E030973_398A_412B_B3C2_A331C12786A8__INCLUDED_)
