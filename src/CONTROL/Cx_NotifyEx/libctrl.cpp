#include "stdafx.h"
#include "libctrl.h"
#include "CX_NotifyEx.h"
#include "ControlWnd.h"
#include "../../h/axisfire.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CControlWnd *m_pControlWnd = new CControlWnd();
	m_pControlWnd->m_pParent = parent;
	
	m_pControlWnd->SetParam((struct _param*)pParam);
	m_pControlWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, m_pControlWnd->m_Param.rect, parent, 100);
	return m_pControlWnd;
}
