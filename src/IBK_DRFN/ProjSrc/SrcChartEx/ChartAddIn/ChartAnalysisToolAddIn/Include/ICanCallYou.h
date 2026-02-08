#if !defined(AFX_HANARO_INTERFACE_ICANCALLYOU_H_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_INTERFACE_ICANCALLYOU_H_BY_JUNOK_LEE_98B1A__INCLUDED_

// ICanCallYou.h
DECLARE_INTERFACE(ICanCallYou)
{
	STDMETHOD(DoCallYou)(int nCallType, LONG dwData) PURE;
};

enum ICanCallYouType
{
	ICCYT_changeThick		= 1,	
};

#ifndef MYMETHOD_PROLOGUE
#define MYMETHOD_PROLOGUE(theClass, localClass) \
	theClass* pThis = \
	((theClass*)((BYTE*)this - offsetof(theClass, m_x##localClass))); \
	pThis;
#endif

#endif //AFX_HANARO_INTERFACE_ICANCALLYOU_H_BY_JUNOK_LEE_98B1A__INCLUDED_

