// BinaryWave.h: interface for the CBinaryWave class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINARYWAVE_H__74D2DC3E_761F_4B77_8AB3_0E43202B2A6E__INCLUDED_)
#define AFX_BINARYWAVE_H__74D2DC3E_761F_4B77_8AB3_0E43202B2A6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_ST/InnerFunction.h"
class CBinaryWave : public CInnerFunction 
{
public:
	CBinaryWave();
	virtual ~CBinaryWave();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;
};

#endif // !defined(AFX_BINARYWAVE_H__74D2DC3E_761F_4B77_8AB3_0E43202B2A6E__INCLUDED_)
