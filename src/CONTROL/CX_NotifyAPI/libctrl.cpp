#include "stdafx.h"
#include "libctrl.h"
#include "CX_Notify.h"
#include "ControlWnd.h"
#include "SetDlg.h"
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


__declspec(dllexport) char* WINAPI axPropDlg(char* pStr, char* pMtblPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CSetDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		((CCX_NotifyApp*)AfxGetApp())->m_sSetData = dlg.m_sData;
	}
	
	return (LPSTR)((CCX_NotifyApp*)AfxGetApp())->m_sSetData.GetString(); 
}
