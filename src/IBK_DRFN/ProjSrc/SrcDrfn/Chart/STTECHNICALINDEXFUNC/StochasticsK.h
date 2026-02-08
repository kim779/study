// StochasticsK.h: interface for the CStochasticsK class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOCHASTICSK_H__16A53FBF_3F2C_49DF_96A9_30F758A70A51__INCLUDED_)
#define AFX_STOCHASTICSK_H__16A53FBF_3F2C_49DF_96A9_30F758A70A51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

/*  StochasticsK -> StoSlowK  */
class CStochasticsK   : public CInnerFunction	
{
public:
	CStochasticsK();
	virtual ~CStochasticsK();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;


};

#endif // !defined(AFX_STOCHASTICSK_H__16A53FBF_3F2C_49DF_96A9_30F758A70A51__INCLUDED_)
