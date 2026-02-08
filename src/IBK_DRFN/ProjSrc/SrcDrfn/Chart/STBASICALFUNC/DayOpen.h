// DayOpen.h: interface for the CDayOpen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAYOPEN_H__6185DFF0_BDE2_43FC_934D_DA52210F027E__INCLUDED_)
#define AFX_DAYOPEN_H__6185DFF0_BDE2_43FC_934D_DA52210F027E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CDayOpen  : public CInnerFunction
{
public:
	CDayOpen();
	virtual ~CDayOpen();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_DAYOPEN_H__6185DFF0_BDE2_43FC_934D_DA52210F027E__INCLUDED_)
