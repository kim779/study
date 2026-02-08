// BollBandUp.h: interface for the CBollBandUp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOLLBANDUP_H__62BAD917_9082_4929_B216_D2E695A073C0__INCLUDED_)
#define AFX_BOLLBANDUP_H__62BAD917_9082_4929_B216_D2E695A073C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CBollBandUp  : public CInnerFunction
{
public:
	CBollBandUp();
	virtual ~CBollBandUp();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_BOLLBANDUP_H__62BAD917_9082_4929_B216_D2E695A073C0__INCLUDED_)
