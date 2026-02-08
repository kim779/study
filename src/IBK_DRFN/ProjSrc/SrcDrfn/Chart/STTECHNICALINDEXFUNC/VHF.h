// VHF.h: interface for the CVHF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VHF_H__6E6FFE7A_863A_49A6_88F3_7E9312FD6DB2__INCLUDED_)
#define AFX_VHF_H__6E6FFE7A_863A_49A6_88F3_7E9312FD6DB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Include_ST/InnerFunction.h"

class CVHF   : public CInnerFunction
{
public:
	CVHF();
	virtual ~CVHF();
	CString GetName(long lType) {return m_strName;}
	CString GetInputs(long lType);
	double Func(CISTController *pIController , long lType, const VARIANT FAR& varArg1, const VARIANT FAR& varArg2, const VARIANT FAR& varArg3, const VARIANT FAR& varArg4, const VARIANT FAR& varArg5, const VARIANT FAR& varArg6, const VARIANT FAR& varArg7, const VARIANT FAR& varArg8, const VARIANT FAR& varArg9, const VARIANT FAR& varArg10);
private:
	CString m_strName;

};

#endif // !defined(AFX_VHF_H__6E6FFE7A_863A_49A6_88F3_7E9312FD6DB2__INCLUDED_)
