// DateToJulian.h: interface for the CDateToJulian class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATETOJULIAN_H__EA63C89F_9CD9_4AA6_B3D2_42F535177408__INCLUDED_)
#define AFX_DATETOJULIAN_H__EA63C89F_9CD9_4AA6_B3D2_42F535177408__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CDateToJulian   : public CInnerFunction
{
public:
	CDateToJulian();
	virtual ~CDateToJulian();
	CString GetName(long lType);
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_DATETOJULIAN_H__EA63C89F_9CD9_4AA6_B3D2_42F535177408__INCLUDED_)
