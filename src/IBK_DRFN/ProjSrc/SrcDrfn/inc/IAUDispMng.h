#if !defined(AFX_HANARO_DISP_MANAGER__HEADER_73649_BY_JUNOK_LEE_98B1A__INCLUDED_)
#define AFX_HANARO_DISP_MANAGER__HEADER_73649_BY_JUNOK_LEE_98B1A__INCLUDED_

// #include "../../_Include/OAUPlatformMng.h"
#include "IBaseDefine.h"

DECLARE_INTERFACE(IAUDispManager)
{
	//
	// ¿¹. LPUNKNOWN pUnKnown = m_pBaseDesk->GetCtrlUnknown(FORM_OPENMANAGER);
	// ¿¹. LPUNKNOWN pUnKnown = m_pBaseDesk->GetCtrlUnknown(FORM_DATAMAN);
	STDMETHOD_(LONG ,Advise)(LPUNKNOWN pUnKnown) PURE;
	STDMETHOD_(void, UnAdvise)(LONG dwKey) PURE;
};

#endif // AFX_HANARO_DISP_MANAGER__HEADER_73649_BY_JUNOK_LEE_98B1A__INCLUDED_
