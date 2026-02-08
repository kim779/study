// Pow.h: interface for the CPow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POW_H__3B3B9EF9_B0E3_4C52_A6C8_2E8F1E661AE3__INCLUDED_)
#define AFX_POW_H__3B3B9EF9_B0E3_4C52_A6C8_2E8F1E661AE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CPow  : public CInnerFunction
{
public:
	CPow();
	virtual ~CPow();
	CString GetName(long lType);
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName0;//TS
	CString m_strName1;//YES


};

#endif // !defined(AFX_POW_H__3B3B9EF9_B0E3_4C52_A6C8_2E8F1E661AE3__INCLUDED_)
