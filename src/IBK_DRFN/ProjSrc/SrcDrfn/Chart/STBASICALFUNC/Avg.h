// Avg.h: interface for the CAvg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVG_H__2EFA50BC_19C0_4F9C_9920_C3C7B21E3CF3__INCLUDED_)
#define AFX_AVG_H__2EFA50BC_19C0_4F9C_9920_C3C7B21E3CF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CAvg  : public CInnerFunction
{
public:
	CAvg();
	virtual ~CAvg();
	CString GetName(long lType);
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName0;//TS
	CString m_strName1;//YES

};

#endif // !defined(AFX_AVG_H__2EFA50BC_19C0_4F9C_9920_C3C7B21E3CF3__INCLUDED_)
