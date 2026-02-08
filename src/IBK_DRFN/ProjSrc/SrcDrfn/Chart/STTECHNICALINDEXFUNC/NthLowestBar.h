// NthLowestBar.h: interface for the CNthLowestBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NTHLOWESTBAR_H__B8963AD6_C11A_4D96_ADA2_B5525B93926A__INCLUDED_)
#define AFX_NTHLOWESTBAR_H__B8963AD6_C11A_4D96_ADA2_B5525B93926A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CNthLowestBar  : public CInnerFunction
{
public:
	CNthLowestBar();
	virtual ~CNthLowestBar();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_NTHLOWESTBAR_H__B8963AD6_C11A_4D96_ADA2_B5525B93926A__INCLUDED_)
