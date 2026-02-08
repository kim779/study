// MACD.h: interface for the CMACD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MACD_H__F56449C0_FD95_434C_AEB9_C8DF77E04C10__INCLUDED_)
#define AFX_MACD_H__F56449C0_FD95_434C_AEB9_C8DF77E04C10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CMACD  : public CInnerFunction
{
public:
	CMACD();
	virtual ~CMACD();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_MACD_H__F56449C0_FD95_434C_AEB9_C8DF77E04C10__INCLUDED_)
