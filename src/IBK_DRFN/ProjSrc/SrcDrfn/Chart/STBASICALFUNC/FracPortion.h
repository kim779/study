// FracPortion.h: interface for the CFracPortion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRACPORTION_H__1B103537_AB70_410A_8AEC_D71AFEB81BC6__INCLUDED_)
#define AFX_FRACPORTION_H__1B103537_AB70_410A_8AEC_D71AFEB81BC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CFracPortion  : public CInnerFunction
{
public:
	CFracPortion();
	virtual ~CFracPortion();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType,  const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_FRACPORTION_H__1B103537_AB70_410A_8AEC_D71AFEB81BC6__INCLUDED_)
