// AroonUp.h: interface for the CAroonUp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AROONUP_H__FD636FF6_2A5A_4A1F_8789_7EBA9DF26D99__INCLUDED_)
#define AFX_AROONUP_H__FD636FF6_2A5A_4A1F_8789_7EBA9DF26D99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_ST/InnerFunction.h"
class CAroonUp : public CInnerFunction 
{
public:
	CAroonUp();
	virtual ~CAroonUp();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_AROONUP_H__FD636FF6_2A5A_4A1F_8789_7EBA9DF26D99__INCLUDED_)
