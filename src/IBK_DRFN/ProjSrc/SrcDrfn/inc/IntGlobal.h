// IntGlobal1.h: interface for the CIntGlobal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTGLOBAL1_H__E53E1D47_6CD3_4E59_A458_7C5FB70D6D18__INCLUDED_)
#define AFX_INTGLOBAL1_H__E53E1D47_6CD3_4E59_A458_7C5FB70D6D18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CIntGlobal  
{
public:
	CIntGlobal();
	virtual ~CIntGlobal();

};

class CGManagerObj
{
public:
	DWORD m_nKey;
public:
	virtual ~CGManagerObj(){};
	virtual void CloseAll(){};

public:
	CGManagerObj(){m_nKey = 0xFFFF;};
	DWORD GetKey() { return m_nKey;}
	void SetKey(DWORD nKey){
		m_nKey = nKey;
	}
};

class CMyPropety 
{
public :
	virtual int Save(){ return -1;}
	virtual void SetManager(void* pDataMng){
	}
};

#ifdef _INTLOOK
#define MAX_CUR_GROUP 1
#endif
#ifdef _BLINK
#define MAX_CUR_GROUP 2
#endif
#ifdef _INTERESTDLL
#define MAX_CUR_GROUP 3
#endif
#ifdef _INTMAIN
#define MAX_CUR_GROUP 3
#endif
#endif // !defined(AFX_INTGLOBAL1_H__E53E1D47_6CD3_4E59_A458_7C5FB70D6D18__INCLUDED_)
