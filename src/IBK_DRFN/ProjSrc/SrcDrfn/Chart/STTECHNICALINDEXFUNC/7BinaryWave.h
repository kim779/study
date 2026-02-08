// 7BinaryWave.h: interface for the C7BinaryWave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_7BINARYWAVE_H__349B711D_976D_40C2_8059_100542908680__INCLUDED_)
#define AFX_7BINARYWAVE_H__349B711D_976D_40C2_8059_100542908680__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class C7BinaryWave  : public CInnerFunction
{
public:
	C7BinaryWave();
	virtual ~C7BinaryWave();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_7BINARYWAVE_H__349B711D_976D_40C2_8059_100542908680__INCLUDED_)
