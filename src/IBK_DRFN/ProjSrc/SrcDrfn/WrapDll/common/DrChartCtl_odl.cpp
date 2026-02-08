#include "stdafx.h"
#include "DrChartCtl.h"

const AFX_EVENTMAP CDrChartCtl::eventMap = { NULL, NULL };

const AFX_EVENTMAP* CDrChartCtl::GetEventMap() const
{
	return &eventMap;
}

const AFX_EVENTMAP_ENTRY* CDrChartCtl::GetEventMapEntry(
													 LPCTSTR pszName,
													 DISPID* pDispid) const
{
	ASSERT(pszName != NULL);
	ASSERT(pDispid != NULL);
	
	const AFX_EVENTMAP* pEventMap = GetEventMap();
	const AFX_EVENTMAP_ENTRY* pEntry;
	DISPID dispid = MAKELONG(1, 0);
	
	while (pEventMap != NULL)
	{
		pEntry = pEventMap->lpEntries;
		
		// Scan entries in this event map
		
		if (pEntry != NULL)
		{
			while (pEntry->pszName != NULL)
			{
				if (_tcscmp(pEntry->pszName, pszName) == 0)
				{
					if (pEntry->dispid != DISPID_UNKNOWN)
						dispid = pEntry->dispid;
					
					*pDispid = dispid;
					return pEntry;
				}
				
				++pEntry;
				++dispid;
			}
		}
		
		// If we didn't find it, go to the base class's event map
#if !defined(_AFXDLL)		//tour2k
		pEventMap = pEventMap->lpBaseEventMap;
#endif
		dispid = MAKELONG(1, HIWORD(dispid)+1);
	}
	
	// If we reach here, the event isn't supported
	
	return NULL;
}

void CDrChartCtl::FireEventV(DISPID dispid, BYTE* pbParams,
						  va_list argList)
{
	COleDispatchDriver driver;
	
	POSITION pos = m_xEventConnPt.GetStartPosition();
	LPDISPATCH pDispatch;
	while (pos != NULL)
	{
		pDispatch = (LPDISPATCH)m_xEventConnPt.GetNextConnection(pos);
		ASSERT(pDispatch != NULL);
		driver.AttachDispatch(pDispatch, FALSE);
		TRY
			driver.InvokeHelperV(dispid, DISPATCH_METHOD, VT_EMPTY, NULL,
			pbParams, argList);
		END_TRY
			driver.DetachDispatch();
	}
}

void AFX_CDECL CDrChartCtl::FireEvent(DISPID dispid, BYTE* pbParams, ...)
{
	va_list argList;
	va_start(argList, pbParams);
	FireEventV(dispid, pbParams, argList);
	va_end(argList);
}

void CDrChartCtl::XEventConnPt::OnAdvise(BOOL bAdvise)
{
	METHOD_PROLOGUE_EX(CDrChartCtl, EventConnPt)
		//	pThis->OnEventAdvise(bAdvise);
		//	TRACE("Event conn pt advise\n");
}

REFIID CDrChartCtl::XEventConnPt::GetIID()
{
	METHOD_PROLOGUE_EX_(CDrChartCtl, EventConnPt)
		if (pThis->m_piidEvents != NULL)
			return *(pThis->m_piidEvents);
		else
			return GUID_NULL;
}

LPUNKNOWN CDrChartCtl::XEventConnPt::QuerySinkInterface(LPUNKNOWN pUnkSink)
{
	METHOD_PROLOGUE_EX_(CDrChartCtl, EventConnPt)
		
		// First, QI for control-specific IID; failing that, QI for IDispatch
		LPUNKNOWN pUnkReturn = NULL;
	if (FAILED(pUnkSink->QueryInterface(*(pThis->m_piidEvents),
		reinterpret_cast<void**>(&pUnkReturn))))
	{
		pUnkSink->QueryInterface(IID_IDispatch,
			reinterpret_cast<void**>(&pUnkReturn));
	}
	return pUnkReturn;
}

LPCONNECTIONPOINT CDrChartCtl::GetConnectionHook(REFIID iid)
{
	if ((m_piidEvents != NULL) && IsEqualIID(iid, *m_piidEvents))
		return (LPCONNECTIONPOINT)((char*)&m_xEventConnPt +
		offsetof(CConnectionPoint, m_xConnPt));
	else
		return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CBaseCtl::GetExtraConnectionPoints - override the connection map

BOOL CDrChartCtl::GetExtraConnectionPoints(CPtrArray* pConnPoints)
{
	pConnPoints->Add((char*)&m_xEventConnPt +
		offsetof(CConnectionPoint, m_xConnPt));
	return TRUE;
}
