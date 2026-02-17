// Script.cpp: implementation of the CScript class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "Wizard.h"
#include "Client.h"
#include "Script.h"
#include "Stream.h"
#include "Key.h"

#include "../h/axiserr.h"
#include "../dll/vbs/scriptEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScript::CScript(CGuard* guard)
{
	m_guard = guard;// ? guard : ((CWizardApp *)AfxGetApp())->GetGuard();
	m_client = (CClient*)0;
	m_screen = (CScreen*)0;

	m_script = false;
}

CScript::~CScript()
{

}

bool CScript::OnStart(CScreen* screen)
{
	CString	procs = getProcName(evStart);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnFocus(CScreen* screen)
{
	CString	procs = getProcName(evFocus);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnClose(CScreen* screen)
{
	CString	procs = getProcName(evClose);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnReceive(CScreen* screen, CString trs)
{
	CString	procs = getProcName(evReceive);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs, (WPARAM)0, (LPARAM)(char *)trs.operator LPCTSTR());
	}
	return true;
}

bool CScript::OnSend(CScreen* screen)
{
	CString	procs = getProcName(evSend);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnKey(CScreen* screen, CfmBase* form)
{
	CString	procs = getProcName(evKey, form ? form->GetSymbolName() : _T(""));
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		screen->m_client->m_vms.button = MS_NB;
		screen->m_client->m_vms.scroll = SB_NS;
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnService(class CScreen* screen, char* data, int datL)
{
	CString	procs = getProcName(evService);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs, (WPARAM)datL, (LPARAM)CString(data, datL).operator LPCTSTR());
	}
	return true;
}

bool CScript::OnApprove(class CScreen* screen, int key, char* data, int datL)
{
	CString	procs = getProcName(evApprove);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs, (WPARAM)datL, (LPARAM)CString(data, datL).operator LPCTSTR(), key);
	}
	return true;
}

bool CScript::OnDevice(CScreen* screen)
{
	CString	procs = getProcName(evDevice);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnTimer(class CScreen* screen)
{
	CString procs = getProcName(evTimer);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnTimerX(class CScreen* screen, int id)
{
	CString	procs = getProcName(evTimerX);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs, (WPARAM)id);
	}
	return true;
}

bool CScript::OnSelect(class CScreen* screen, CString maps)
{
	CString	procs = getProcName(evSelect);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs, (WPARAM)0, (LPARAM)(char *)maps.operator LPCTSTR());
	}
	return true;
}

bool CScript::OnFile(class CScreen* screen, BOOL upload, char* data, int datL)
{
	CString	procs = getProcName(evFile);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs, (WPARAM)datL, (LPARAM)CString(data, datL).operator LPCTSTR(), upload);
	}
	return true;
}

bool CScript::OnAlert(class CScreen* screen, CString code, CString data, int stat)
{
return true;
	CString	procs = getProcName(evAlert);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs, (WPARAM)(char *)code.operator LPCTSTR(),
							(LPARAM)(char *)data.operator LPCTSTR(), stat);
	}
	return true;
}

bool CScript::OnSize(class CScreen* screen)
{
	CString procs = getProcName(evSize);
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnClick(CScreen* screen, CfmBase* form, bool byKey)
{
	CString	procs = getProcName(evClick, form->GetSymbolName());
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = byKey;
		screen->m_client->m_vms.key    = 0;
		screen->m_client->m_vms.shift  = SS_NS;
		screen->m_client->m_vms.button = MS_NB;
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnDblClick(CScreen* screen, CfmBase* form)
{
	CString	procs = getProcName(evDblClk, form->GetSymbolName());
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		screen->m_client->m_vms.key    = 0;
		screen->m_client->m_vms.shift  = SS_NS;
		screen->m_client->m_vms.button = MS_NB;
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnChange(CScreen* screen, CfmBase* form, bool byKey)
{
	CString	procs = getProcName(evChange, form->GetSymbolName());
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = byKey;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnMouseDown(CScreen* screen, CfmBase* form)
{
	CString	procs = getProcName(evMouseDn, form->GetSymbolName());
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		screen->m_client->m_vms.key    = 0;
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnMouseUp(CScreen* screen, CfmBase* form)
{
	CString	procs = getProcName(evMouseUp, form->GetSymbolName());
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		screen->m_client->m_vms.key    = 0;
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnCharChange(CScreen* screen, CfmBase* form)
{
	CString	procs = getProcName(evChar, form->GetSymbolName());
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnSetFocus(CScreen* screen, CfmBase* form)
{
	CString	procs = getProcName(evSetFocus, form->GetSymbolName());
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnMouseEnter(CScreen* screen, CfmBase* form)
{
	CString procs = getProcName(evMouseEnter, form->GetSymbolName());
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnMouseLeave(CScreen* screen, CfmBase* form)
{
	CString	procs = getProcName(evMouseLeave, form->GetSymbolName());
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(procs);
	}
	return true;
}

bool CScript::OnProcedure(CScreen* screen, CString procs)
{
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		return screen->m_vbe->DoProcedure(procs);
	}
	return false;
}

bool CScript::Procedure(class CScreen* screen, CString procs, CString data, int count)
{
	if (screen->m_vbe->IsAvailable(procs))
	{
		m_script = false;
		return screen->m_vbe->DoProcedure(procs, data, count);
	}
	return false;
}

bool CScript::IsAvailable(CScreen* screen, int event, CfmBase* form)
{
	return screen->m_vbe->IsAvailable(getProcName(event, form ? form->GetSymbolName() : _T("")));
}

CString CScript::getProcName(int event, CString symbol)
{
	CString	procs;

	switch (event)
	{
	case evStart:
		procs = _T("AX_SUB_OnStart_AX_");	break;
	case evFocus:
		procs = _T("AX_SUB_OnFocus_AX_");	break;
	case evClose:
		procs = _T("AX_SUB_OnClose_AX_");	break;
	case evSend:
		procs = _T("AX_SUB_OnSend_AX_");	break;
	case evReceive:
		procs = _T("AX_SUB_OnReceive_AX_");	break;
	case evKey:
		if (symbol.IsEmpty())
			procs = _T("AX_SUB_OnKey_AX_");
		else
			procs.Format(_T("AX_%s_OnKey_AX_"), symbol.GetString());
		break;
	case evService:
		procs = _T("AX_SUB_OnService_AX_");	break;
	case evApprove:
		procs = _T("AX_SUB_OnApprove_AX_");	break;
	case evDevice:
		procs = _T("AX_SUB_OnDevice_AX_");	break;
	case evTimer:
		procs = _T("AX_SUB_OnTimer_AX_");	break;
	case evTimerX:
		procs = _T("AX_SUB_OnTimerX_AX_");	break;
	case evSelect:
		procs = _T("AX_SUB_OnSelect_AX_");	break;
	case evFile:
		procs = _T("AX_SUB_OnFile_AX_");	break;
	case evAlert:
		procs = _T("AX_SUB_OnAlert_AX_");	break;
	case evSize:
		procs = _T("AX_SUB_OnSize_AX_");	break;
	default:
		if (!symbol.IsEmpty())
		{
			switch (event)
			{
			case evClick:
				procs.Format(_T("AX_%s_OnClick_AX_"), symbol.GetString());		break;
			case evChange:
				procs.Format(_T("AX_%s_OnChange_AX_"), symbol.GetString());		break;
			case evDblClk:
				procs.Format(_T("AX_%s_OnDblClick_AX_"), symbol.GetString());	break;
			case evMouseDn:
				procs.Format(_T("AX_%s_OnMouseDown_AX_"), symbol.GetString());	break;
			case evMouseUp:
				procs.Format(_T("AX_%s_OnMouseUp_AX_"), symbol.GetString());	break;
			case evChar:
				procs.Format(_T("AX_%s_OnCharChange_AX_"), symbol.GetString());	break;
			case evSetFocus:
				procs.Format(_T("AX_%s_OnSetFocus_AX_"), symbol.GetString());	break;
			case evMouseEnter:
				procs.Format(_T("AX_%s_OnMouseEnter_AX_"), symbol.GetString());	break;
			case evMouseLeave:
				procs.Format(_T("AX_%s_OnMouseLeave_AX_"), symbol.GetString());	break;
			default:
				procs = _T("");		break;
			}
			break;
		}
		procs = _T("");
		break;
	}
	return procs;
}

void CScript::initialize(CScreen* screen)
{
	screen->m_client->m_vms.key    = 0;
	screen->m_client->m_vms.shift  = SS_NS;
	screen->m_client->m_vms.button = MS_NB;
	screen->m_client->m_vms.scroll = SB_NS;
}
