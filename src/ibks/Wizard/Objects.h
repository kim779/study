// Objects.h: interface for the CObjects class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTS_H__9EE265B2_76F7_405E_8209_06A8105A90ED__INCLUDED_)
#define AFX_OBJECTS_H__9EE265B2_76F7_405E_8209_06A8105A90ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObjects  
{
public:
	CObjects(CString root);
	virtual ~CObjects();

private:
	CString	m_root;
	CMapStringToPtr	m_objects;

public:
	bool	CreateObject(CString name, CCmdTarget*& target);
};

#endif // !defined(AFX_OBJECTS_H__9EE265B2_76F7_405E_8209_06A8105A90ED__INCLUDED_)
