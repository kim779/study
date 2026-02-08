// SystemInfo.h: interface for the CSystemInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMINFO_H__4D097C6D_10E9_11D6_9374_0050FC28B229__INCLUDED_)
#define AFX_SYSTEMINFO_H__4D097C6D_10E9_11D6_9374_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>

class CFormula;
class COptimizeVar;
class CSystemInfoImplementation;

class AFX_EXT_CLASS CSystemInfo : public CObject  
{
public:
	CFormula* GetFormula();
	CTypedPtrList<CObList, COptimizeVar*>* GetOptimizeVarList();
	CString GetName();
	CString GetSystemName();
	CString GetExplain();
	CString GetType();	

	CSystemInfo(const CString&  Name, 
				const CString&  SystemName, 
				const CString&  Explain,
				const CString&  Formula,
				const CString&  Type,
				const CString&  Opt);
	virtual ~CSystemInfo();

private:
	CSystemInfoImplementation* m_pSystemInfoImpl;
};

#endif // !defined(AFX_SYSTEMINFO_H__4D097C6D_10E9_11D6_9374_0050FC28B229__INCLUDED_)
