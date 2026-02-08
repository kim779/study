// SystemList.h: interface for the CSystemList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMLIST_H__4D097C6E_10E9_11D6_9374_0050FC28B229__INCLUDED_)
#define AFX_SYSTEMLIST_H__4D097C6E_10E9_11D6_9374_0050FC28B229__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CSystemInfo;
class CSystemCfg;
class CFormula;
class COptimizeVar;
class CSystemListImplementation;

class AFX_EXT_CLASS CSystemList  
{
public:
	CSystemList(const CString& strFilePath, const CString& strUserFilePath);
	virtual ~CSystemList();

	CString GetFilePath() const;
	CString GetUserFilePath() const;
	CList<CString, CString>* GetSystemNameList();
	CSystemCfg* GetSystemCfg();
	CSystemInfo* GetSystem(const CString& strName);
	CSystemInfo* GetDefaultSystem(const CString& strName);
	CTypedPtrList<CObList, COptimizeVar*>* GetOptimizeVarList(const CString& strName);

	void SetFilePath(const CString& strFilePath);
	void SetUserFilePath(const CString& strUserFilePath);
	void SaveUserSystemInfoAndCfg();
	bool CreateUserConfigFile();
	void SaveSectionOfSystemInfo(const CString& strSection);
	void SaveSystemCfg();

private:
	CSystemListImplementation* m_pSystemListImpl;
};

#endif // !defined(AFX_SYSTEMLIST_H__4D097C6E_10E9_11D6_9374_0050FC28B229__INCLUDED_)
