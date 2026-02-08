// TRIX.h: interface for the CTRIX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIX_H__E924F742_F862_4060_9B63_07B18ED82AE5__INCLUDED_)
#define AFX_TRIX_H__E924F742_F862_4060_9B63_07B18ED82AE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CTRIX  : public CInnerFunction
{
public:
	CTRIX();
	virtual ~CTRIX();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_TRIX_H__E924F742_F862_4060_9B63_07B18ED82AE5__INCLUDED_)
