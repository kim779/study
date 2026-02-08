#include "stdafx.h"
#include "ControlWnd.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CControlWnd *pControlWnd = new CControlWnd();
	pControlWnd->m_pParent = parent;

	pControlWnd->SetParam((struct _param*)pParam);
	pControlWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN, pControlWnd->m_Param.rect, parent, 100);

	return pControlWnd;
}