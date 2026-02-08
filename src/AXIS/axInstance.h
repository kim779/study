// axInstance.h: interface for the CAxInstance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXINSTANCE_H__9C695125_1172_11D6_8AC5_00001CD7F9BE__INCLUDED_)
#define AFX_AXINSTANCE_H__9C695125_1172_11D6_8AC5_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAxInstance  
{
public:
	CAxInstance(CString regkey);
	virtual ~CAxInstance();

private:
	CString	m_regkey;
	CString	m_uniqueClsName;
	bool	m_classRegistered;
	HANDLE	m_hMutex;

public:
	bool	IsFirstInstance();
	bool	RegisterClass(HINSTANCE hInst, UINT uMainID);
	void	SetClass(CREATESTRUCT& cs);
	void	UnsetClass(HINSTANCE hInst);
	bool	AddDocTemplate();
};

#endif // !defined(AFX_AXINSTANCE_H__9C695125_1172_11D6_8AC5_00001CD7F9BE__INCLUDED_)
