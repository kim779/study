#include "stdafx.h"
#include "libctrl.h"
#include "CX_SIGNAL.h"
#include "MainWnd.h"
#include "../../h/axisfire.h"

__declspec(dllexport) CWnd* WINAPI axCreate(CWnd *parent, void* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	MainWnd *m_pControlWnd = new MainWnd();
	m_pControlWnd->m_pParent = parent;
	 
	m_pControlWnd->SetParam((struct _param*)pParam);
	m_pControlWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, m_pControlWnd->m_param.rect, parent, 100);
	return m_pControlWnd;
}

