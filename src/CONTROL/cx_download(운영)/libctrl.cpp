#include "stdafx.h"
#include "libctrl.h"
#include "ControlWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CControlWnd *m_pControlWnd = new CControlWnd((_param*)pParam, parent);		
	m_pControlWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, 
		((_param*)pParam)->rect, parent, 100);

	return m_pControlWnd;
}