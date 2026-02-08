// STD_SMA.h: interface for the CSTD_SMA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STD_SMA_H__478BA8A8_BD5C_4DE0_BEB6_36DBAFAF53BD__INCLUDED_)
#define AFX_STD_SMA_H__478BA8A8_BD5C_4DE0_BEB6_36DBAFAF53BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CSTD_SMA : public CInnerFunction  
{
public:
	CSTD_SMA();
	virtual ~CSTD_SMA();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_STD_SMA_H__478BA8A8_BD5C_4DE0_BEB6_36DBAFAF53BD__INCLUDED_)
