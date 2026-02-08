// ATan.h: interface for the CATan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATAN_H__71237897_B631_4219_BE2F_4A3CA366ED49__INCLUDED_)
#define AFX_ATAN_H__71237897_B631_4219_BE2F_4A3CA366ED49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CATan   : public CInnerFunction
{
public:
	CATan();
	virtual ~CATan();
	CString GetName(long lType);
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName0;//TS
	CString m_strName1;//YES

};

#endif // !defined(AFX_ATAN_H__71237897_B631_4219_BE2F_4A3CA366ED49__INCLUDED_)
