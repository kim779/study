// StochasticsK_Raw.h: interface for the CStochasticsK_Raw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STOCHASTICSK_RAW_H__ECE2519B_6427_4787_BF09_CE5C8FC3FCB5__INCLUDED_)
#define AFX_STOCHASTICSK_RAW_H__ECE2519B_6427_4787_BF09_CE5C8FC3FCB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CStochasticsK_Raw   : public CInnerFunction
{
public:
	CStochasticsK_Raw();
	virtual ~CStochasticsK_Raw();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_STOCHASTICSK_RAW_H__ECE2519B_6427_4787_BF09_CE5C8FC3FCB5__INCLUDED_)
