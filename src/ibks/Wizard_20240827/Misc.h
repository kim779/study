// misc.h: interface for the CMapV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MISC_H__69A91B02_8185_11D3_83ED_00105A779B5A__INCLUDED_)
#define AFX_MISC_H__69A91B02_8185_11D3_83ED_00105A779B5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CComboN : public CObject
{
public:
	CComboN();
	virtual ~CComboN();

	CString	m_entry = _T("");
};


class CCaret  
{
public:
	CCaret();
	virtual ~CCaret();

	bool CCaret::operator ==(const CCaret& caret)
	{ return (m_key == caret.m_key && m_idx == caret.m_idx); }
	bool CCaret::operator !=(const CCaret& caret)
	{ return !operator ==(caret); }

	int	m_key = 0;
	int	m_idx = 0;
};


class CCode : public CObject  
{
public:
	CCode();
	virtual ~CCode();

	bool		m_self;
	CStringArray	m_codes;
};


class CpubControl
{
public:
	HINSTANCE	m_dll;
	CCmdTarget*	m_target;
};


#define	maxSYM		1000
class CdataSet : public CObject
{
public:
	CdataSet();
	virtual	~CdataSet();

protected:
	bool	m_on;
	DWORD	m_data[maxSYM];

public:
	bool	IsOn();
	void	SetOn(bool on = true);
	bool	Lookup(int index, CString& data);
	void	SetAt(int index, char* data);
	DWORD	GetData();
};


#endif // !defined(AFX_MISC_H__69A91B02_8185_11D3_83ED_00105A779B5A__INCLUDED_)
