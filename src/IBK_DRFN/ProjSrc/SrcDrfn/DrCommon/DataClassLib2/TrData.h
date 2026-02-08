/**
	@file		unitdata.h
	@brief		클라이언트에서 사용하는 데이터형
	@author		sashia
	@date		2009/08/13
*/

#if !defined(AFX_UNITDATA_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
#define AFX_UNITDATA_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma	pack(1)

#define		VALUE_TYPE_CHAR		0x0001
#define		VALUE_TYPE_INT		0x0002
#define		VALUE_TYPE_BOOL		0x0003
#define		VALUE_TYPE_LONG		0x0004
#define		VALUE_TYPE_DOUBLE	0x0005
#define		VALUE_TYPE_STRING	0x0006

//#define		VALUE_TYPE_COUNTER	0x1000

/**
	@class		CBaseValue
	@brief		모든 데이터형의 Base
	@author		sashia
	@date		2009/08/13
*/
class CBaseValue
{
public:
	CBaseValue();
	virtual ~CBaseValue();

public:
	virtual void	SetValue(void *pValue, int nSize)=0;
	virtual int		GetValue(void **pValue)=0;

	virtual void	SetstrValue(CString strData)=0;
	virtual CString	GetstrValue()=0;
	virtual void	SetbValue(BOOL bValue)=0;
	virtual BOOL	GetbValue()=0;
	virtual void	SetnValue(int nValue)=0;
	virtual int		GetnValue()=0;
	virtual void	SetlValue(long lValue)=0;
	virtual long	GetlValue()=0;
	virtual void	SetdValue(double dValue)=0;
	virtual double	GetdValue()=0;
};

/**
	@class		CCharValue
	@brief		Char Data형.
	@author		sashia
	@date		2009/08/13
*/
class CCharValue : public CBaseValue
{
public:
	CCharValue();
	CCharValue(CCharValue *pItem);
	virtual ~CCharValue();

	void	ResetValue(int nSize);

	char	*m_pszValue;
	int		m_nSize;

public:
	void	Copy(const CCharValue& item);
	virtual CCharValue& operator = (const CCharValue& item);

public:
	virtual void	SetValue(void *pValue, int nSize=-1);
	virtual int		GetValue(void **pValue);

	virtual void	SetstrValue(CString strData);
	virtual CString	GetstrValue();
	virtual void	SetbValue(BOOL bValue);
	virtual BOOL	GetbValue();
	virtual void	SetnValue(int nValue);
	virtual int		GetnValue();
	virtual void	SetlValue(long lValue);
	virtual long	GetlValue();
	virtual void	SetdValue(double dValue);
	virtual double	GetdValue();
};

/**
	@class		CStringValue
	@brief		String Data형.
	@author		sashia
	@date		2009/08/13
*/
class CStringValue : public CBaseValue
{
public:
	CStringValue();
	CStringValue(CStringValue *pItem);
	virtual ~CStringValue();

	CString		m_strValue;

public:
	void	Copy(const CStringValue& item);
	virtual CStringValue& operator = (const CStringValue& item);

public:
	virtual void	SetValue(void *pValue, int nSize=0)	{ m_strValue.Format(_T("%s"), (char*)pValue);	};
	virtual int		GetValue(void **pValue)				{ *pValue = (LPVOID)m_strValue.GetBuffer(0);	return m_strValue.GetLength();	};

	virtual void	SetstrValue(CString strData)	{ m_strValue = strData; };
	virtual CString	GetstrValue()					{ return m_strValue;	};
	virtual void	SetbValue(BOOL bValue)		{ m_strValue.Format(_T("%d"), (int)bValue); };
	virtual BOOL	GetbValue()					{ return atoi(m_strValue);	};
	virtual void	SetnValue(int nValue)		{ m_strValue.Format(_T("%d"), (int)nValue); };
	virtual int		GetnValue()					{ return atoi(m_strValue);	};
	virtual void	SetlValue(long lValue)		{ m_strValue.Format(_T("%l"), (long)lValue); };
	virtual long	GetlValue()					{ return atol(m_strValue);	};
	virtual void	SetdValue(double dValue)	{ m_strValue.Format(_T("%f"), (double)dValue); };
	virtual double	GetdValue()					{ return atof(m_strValue);	};
};

/**
	@class		CIntValue
	@brief		Integer Data형.
	@author		sashia
	@date		2009/08/13
*/
class CIntValue : public CBaseValue
{
public:
	CIntValue();
	CIntValue(CIntValue *pItem);
	virtual ~CIntValue();

	int		m_nValue;

public:
	void	Copy(const CIntValue& item);
	virtual CIntValue& operator = (const CIntValue& item);

public:
	virtual void	SetValue(void *pValue, int nSize=sizeof(int))	{ m_nValue = *(int*)pValue;	};
	virtual int		GetValue(void **pValue)	{ *pValue = (void*)&m_nValue; return sizeof(int);	};

	virtual void	SetstrValue(CString strData)	{ m_nValue = atoi(strData);	};
	virtual CString	GetstrValue() {
		CString strData; strData.Format(_T("%d"), m_nValue); return strData;
	};
	virtual void	SetbValue(BOOL bValue)		{ m_nValue = (int)bValue;	};
	virtual BOOL	GetbValue()					{ return (BOOL)m_nValue;	};
	virtual void	SetnValue(int nValue)		{ m_nValue = nValue;		};
	virtual int		GetnValue()					{ return m_nValue;			};
	virtual void	SetlValue(long lValue)		{ m_nValue = (int)lValue;	};
	virtual long	GetlValue()					{ return (long)m_nValue;	};
	virtual void	SetdValue(double dValue)	{ m_nValue = (int)dValue;	};
	virtual double	GetdValue()					{ return (double)m_nValue;	};
};

/**
	@class		CBoolValue
	@brief		BOOL Data형.
	@author		sashia
	@date		2009/08/13
*/
class CBoolValue : public CBaseValue
{
public:
	CBoolValue();
	CBoolValue(CBoolValue *pItem);
	virtual ~CBoolValue();

	BOOL	m_bValue;

public:
	void	Copy(const CBoolValue& item);
	virtual CBoolValue& operator = (const CBoolValue& item);

public:
	virtual void	SetValue(void *pValue, int nSize=sizeof(BOOL))	{ m_bValue = *(BOOL*)pValue;	};
	virtual int		GetValue(void **pValue)	{ *pValue = (void*)&m_bValue; return sizeof(BOOL);	};

	virtual void	SetstrValue(CString strData)	{ m_bValue = atoi(strData);	};
	virtual CString	GetstrValue()	{ return (m_bValue)?_T("TRUE"):_T("FALSE");	};
	virtual void	SetbValue(BOOL bValue)		{ m_bValue = bValue;		};
	virtual BOOL	GetbValue()					{ return m_bValue;			};
	virtual void	SetnValue(int nValue)		{ m_bValue = (BOOL)nValue;	};
	virtual int		GetnValue()					{ return (int)m_bValue;		};
	virtual void	SetlValue(long lValue)		{ m_bValue = (BOOL)lValue;	};
	virtual long	GetlValue()					{ return (long)m_bValue;	};
	virtual void	SetdValue(double dValue)	{ m_bValue = (BOOL)dValue;	};
	virtual double	GetdValue()					{ return (double)m_bValue;	};
};

/**
	@class		CLongValue
	@brief		Long Data형.
	@author		sashia
	@date		2009/08/13
*/
class CLongValue : public CBaseValue
{
public:
	CLongValue();
	CLongValue(CLongValue *pItem);
	virtual ~CLongValue();

	long	m_lValue;

public:
	void	Copy(const CLongValue& item);
	virtual CLongValue& operator = (const CLongValue& item);

public:
	virtual void	SetValue(void *pValue, int nSize=sizeof(long))	{ m_lValue = *(long*)pValue;	};
	virtual int		GetValue(void **pValue)	{ *pValue = (void*)&m_lValue; return sizeof(long);	};

	virtual void	SetstrValue(CString strData)	{ m_lValue = atol(strData);		};
	virtual CString	GetstrValue() {
		CString strData; strData.Format(_T("%ld"), m_lValue); return strData;
	};
	virtual void	SetbValue(BOOL bValue)		{ m_lValue = (long)bValue;	};
	virtual BOOL	GetbValue()					{ return (BOOL)m_lValue;	};
	virtual void	SetnValue(int nValue)		{ m_lValue = (long)nValue;	};
	virtual int		GetnValue()					{ return (int)m_lValue;		};
	virtual void	SetlValue(long lValue)		{ m_lValue = lValue;		};
	virtual long	GetlValue()					{ return m_lValue;			};
	virtual void	SetdValue(double dValue)	{ m_lValue = (long)dValue;	};
	virtual double	GetdValue()					{ return (double)m_lValue;	};
};

/**
	@class		CDoubleValue
	@brief		Double Data형.
	@author		sashia
	@date		2009/08/13
*/
class CDoubleValue : public CBaseValue
{
public:
	CDoubleValue();
	CDoubleValue(CDoubleValue *pItem);
	virtual ~CDoubleValue();

	double	m_dValue;

public:
	void	Copy(const CDoubleValue& item);
	virtual CDoubleValue& operator = (const CDoubleValue& item);

public:
	virtual void	SetValue(void *pValue, int nSize=sizeof(double))	{ m_dValue = *(double*)pValue;	};
	virtual int		GetValue(void **pValue)	{ *pValue = (void*)&m_dValue; return sizeof(double);	};

	virtual void	SetstrValue(CString strData)	{ m_dValue = atof(strData);		};
	virtual CString	GetstrValue() {
		CString strData; strData.Format(_T("%f"), m_dValue); return strData;
	};
	virtual void	SetbValue(BOOL bValue)		{ m_dValue = (double)bValue;	};
	virtual BOOL	GetbValue()					{ return (BOOL)m_dValue;		};
	virtual void	SetnValue(int nValue)		{ m_dValue = (double)nValue;	};
	virtual int		GetnValue()					{ return (int)m_dValue;			};
	virtual void	SetlValue(long lValue)		{ m_dValue = (double)lValue;	};
	virtual long	GetlValue()					{ return (long)m_dValue;		};
	virtual void	SetdValue(double dValue)	{ m_dValue = dValue;			};
	virtual double	GetdValue()					{ return m_dValue;				};
};

typedef	CArray<CBaseValue*, CBaseValue*>	CArrValue;

#pragma	pack()

#endif // !defined(AFX_UNITDATA_H__16F42BCA_D645_4B9B_978C_E7AC60B1F611__INCLUDED_)
