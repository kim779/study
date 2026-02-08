// AroonDown.h: interface for the CAroonDown class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AROONDOWN_H__0F902A81_BBFA_48BD_B4FC_61B9DCBC9CB4__INCLUDED_)
#define AFX_AROONDOWN_H__0F902A81_BBFA_48BD_B4FC_61B9DCBC9CB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_ST/InnerFunction.h"
class CAroonDown : public CInnerFunction 
{
public:
	CAroonDown();
	virtual ~CAroonDown();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_AROONDOWN_H__0F902A81_BBFA_48BD_B4FC_61B9DCBC9CB4__INCLUDED_)
