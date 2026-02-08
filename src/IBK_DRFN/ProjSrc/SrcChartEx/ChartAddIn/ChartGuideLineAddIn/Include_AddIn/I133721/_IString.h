// IString.h: interface for the IString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_)
#define AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_AddInVer.h"			// for AddIn Version Define

interface IString : public IUnknown
{
protected:
	virtual ~IString()	{}

protected:
	virtual const char *GetData( void) = 0;

friend class ILPCSTR;
};

// (2006/8/22 - Seung-Won, Bae) String Data Interface Wrapper.
//		Caution! do not make virtual function for sprintf() or CString::Format()
class ILPCSTR
{
protected:
	const char *m_szData;
public:
	ILPCSTR()
	{
		m_szData = NULL;
		m_pIStrData = NULL;
	}
	ILPCSTR( IString *p_pIString)
	{
		m_pIStrData = p_pIString;	// Do not AddRef(). Because It must be Return Value.
		if( m_pIStrData) m_szData = m_pIStrData->GetData();
		else m_szData = NULL;
	}
	~ILPCSTR()
	{
		if( m_pIStrData)
		{
			m_pIStrData->Release();
			m_pIStrData = NULL;
			m_szData = NULL;
		}
	}
protected:
	IString *	m_pIStrData;
public:
	BOOL	IsEmpty( void)
	{
		if( !m_szData) return TRUE;
		return !*m_szData;
	}
public:
	operator const char *() const
	{
		return m_szData;
	}
	const ILPCSTR& operator=( IString *stringSrc)
	{
		if( m_pIStrData) m_pIStrData->Release();
		m_pIStrData = stringSrc;	// Do not AddRef(). Because It must be Return Value.
		if( m_pIStrData) m_szData = m_pIStrData->GetData();
		else m_szData = NULL;
		return *this;
	}
};

#endif // !defined(AFX_ISTRING_H__10782E66_9B95_411B_B7C7_6B76E8D9C12D__INCLUDED_)
