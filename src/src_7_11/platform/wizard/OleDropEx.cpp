// OleDropEx.cpp: implementation of the COleDropEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Wizard.h"
#include "OleDropEx.h"
#include "Works.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COleDropEx::COleDropEx()
{
	m_AddRefCount = 1;

	OleInitialize(NULL);
	CoLockObjectExternal(this, TRUE, 0);
}

COleDropEx::~COleDropEx()
{
	m_works.RemoveAll();
	CoLockObjectExternal(this, FALSE, 0);
	OleUninitialize();
}

HRESULT COleDropEx::QueryInterface(REFIID riid, void **ppvObject) 
{
	if(IsEqualGUID(riid, IID_IDropTarget) || IsEqualGUID(riid, IID_IUnknown)) 
	{
		this->AddRef();
		*ppvObject = this;
		return S_OK;
	}
	return E_FAIL;
}

ULONG COleDropEx::AddRef(void) 
{
	return ++m_AddRefCount;
}

ULONG COleDropEx::Release(void) 
{
	if(--m_AddRefCount <= 0)
		delete this;

	return m_AddRefCount;
}

HRESULT COleDropEx::DragEnter(IDataObject *pObject, unsigned long state, _POINTL mouse, unsigned long *pDrop)
{
	if (pObject == NULL)
		return E_FAIL;

	*pDrop = DROPEFFECT_COPY;
	return S_OK;
}

HRESULT COleDropEx::DragOver(unsigned long state, _POINTL mouse, unsigned long *pDrop)
{
	*pDrop = DROPEFFECT_COPY;
	return S_OK;
}

HRESULT COleDropEx::DragLeave()
{
	return S_OK;
}

HRESULT _stdcall COleDropEx::Drop(IDataObject *pObject, unsigned long state, _POINTL mouse, unsigned long *pDrop)
{
	if (pObject == NULL)
		return E_FAIL;

	*pDrop = DROPEFFECT_COPY;

	FORMATETC	format;
	STGMEDIUM	medium;

	ZeroMemory(&format, sizeof(FORMATETC));
	ZeroMemory(&medium, sizeof(STGMEDIUM));

	format.cfFormat = CF_TEXT;
	format.dwAspect = DVASPECT_CONTENT;
	format.lindex   = -1;
	format.tymed    = TYMED_HGLOBAL;

	HRESULT	hr = pObject->GetData(&format, &medium);
	if (FAILED(hr))
		return hr;

	CPoint	point;
	CWnd*	pWnd;

	point.x = mouse.x;
	point.y = mouse.y;
	pWnd = CWnd::WindowFromPoint(point);

	if (pWnd != NULL)
	{
		CWorks*	works = NULL;
		pWnd->ClientToScreen(&point);

		if (!m_works.Lookup(pWnd->GetSafeHwnd(), works))
		{
			pWnd = pWnd->GetParent();
			if (pWnd != NULL)
				m_works.Lookup(pWnd->GetSafeHwnd(), works);
		}
		if (works)
		{
			CString text = (char *)::GlobalLock(medium.hGlobal);
			pWnd->ScreenToClient(&point);
			::GlobalUnlock(medium.hGlobal);
			if (works->m_status & S_DROP)
				works->OnDrop(point, text, pWnd);
		}
	}

	if (medium.pUnkForRelease != NULL)
		medium.pUnkForRelease->Release();

	return S_OK;
}

BOOL COleDropEx::Register(CWorks* works, HWND hWnd)
{
	if (hWnd == NULL)
		return E_FAIL;

	HRESULT hr = ::RegisterDragDrop(hWnd, this);
	if (SUCCEEDED(hr))
	{
		AddRef();
		m_works.SetAt(hWnd, works);
		return TRUE;
	}
	return FALSE;
}

void COleDropEx::Revoke(HWND hWnd)
{
	if (hWnd != NULL)
	{
		m_works.RemoveKey(hWnd);
		RevokeDragDrop(hWnd);
		Release();
	}
}
