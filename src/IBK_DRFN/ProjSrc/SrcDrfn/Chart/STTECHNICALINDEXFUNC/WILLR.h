// WILLR.h: interface for the CWILLR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WILLR_H__9163654D_1DEA_4F09_B485_151FF6C15AC6__INCLUDED_)
#define AFX_WILLR_H__9163654D_1DEA_4F09_B485_151FF6C15AC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CWILLR  : public CInnerFunction
{
public:
	CWILLR();
	virtual ~CWILLR();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_WILLR_H__9163654D_1DEA_4F09_B485_151FF6C15AC6__INCLUDED_)
