// ATR.h: interface for the CATR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATR_H__366165B9_FC35_4AE2_8791_95BBFB70FBB9__INCLUDED_)
#define AFX_ATR_H__366165B9_FC35_4AE2_8791_95BBFB70FBB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CATR  : public CInnerFunction
{
public:
	CATR();
	virtual ~CATR();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_ATR_H__366165B9_FC35_4AE2_8791_95BBFB70FBB9__INCLUDED_)
