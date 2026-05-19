// Script.cpp: implementation of the CScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Client.h"
#include "Script.h"
#include "Stream.h"
#include "Key.h"

#include "../h/axiserr.h"
#include "../dll/lua/scriptEngine.h"

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
}

CScript::~CScript()
{

}

bool CScript::OnDeclaretion(CScreen* screen)
{
	CString	procs = getProcName(evDeclaretion);
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);	
	if (!sData.IsEmpty())	
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnStart(CScreen* screen)
{
	CString sData = LuaDefine();
	CString	procs = getProcName(evStart);	
	sData += CString(screen->m_mSCRIPT[string(procs)]);	
	if (!sData.IsEmpty())	
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnFocus(CScreen* screen)
{
	CString	procs = getProcName(evFocus);
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())	
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnClose(CScreen* screen)
{
	CString	procs = getProcName(evClose);
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnReceive(CScreen* screen)
{
	CString	procs = getProcName(evReceive);
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnSend(CScreen* screen)
{
	CString	procs = getProcName(evSend);
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnKey(CScreen* screen, CamBase* form)
{
	CString	procs = getProcName(evKey, form ? form->GetSymbolName() : _T(""));
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		screen->m_client->m_vms.button = MS_NB;
		screen->m_client->m_vms.scroll = SB_NS;
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnService(class CScreen* screen, char* data, int datL)
{
	CString	procs = getProcName(evService);
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnApprove(class CScreen* screen, int key, char* data, int datL)
{
	CString	procs = getProcName(evApprove);
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnDevice(CScreen* screen)
{
	CString	procs = getProcName(evDevice);
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnTimer(class CScreen* screen)
{
	CString procs = getProcName(evTimer);
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnSelect(class CScreen* screen, CString maps)
{
	CString	procs = getProcName(evSelect);
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnFile(class CScreen* screen, BOOL upload, char* data, int datL)
{
	CString	procs = getProcName(evFile);
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnAlert(class CScreen* screen, CString code, CString data, int stat)
{
	return true;
	CString	procs = getProcName(evAlert);
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnClick(CScreen* screen, CamBase* form, bool byKey)
{
	CString	procs = getProcName(evClick, form->GetSymbolName());
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	
	if (!sData.IsEmpty())	
	{
		m_script = byKey;
		screen->m_client->m_vms.key    = 0;
		screen->m_client->m_vms.shift  = SS_NS;
		screen->m_client->m_vms.button = MS_NB;

		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnDblClick(CScreen* screen, CamBase* form)
{
	CString	procs = getProcName(evDblClk, form->GetSymbolName());
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		screen->m_client->m_vms.key    = 0;
		screen->m_client->m_vms.shift  = SS_NS;
		screen->m_client->m_vms.button = MS_NB;
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnChange(CScreen* screen, CamBase* form, bool byKey)
{
	CString	procs = getProcName(evChange, form->GetSymbolName());
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = byKey;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnMouseDown(CScreen* screen, CamBase* form)
{
	CString	procs = getProcName(evMouseDn, form->GetSymbolName());
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		screen->m_client->m_vms.key    = 0;
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnMouseUp(CScreen* screen, CamBase* form)
{
	CString	procs = getProcName(evMouseUp, form->GetSymbolName());
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		screen->m_client->m_vms.key    = 0;
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnCharChange(CScreen* screen, CamBase* form)
{
	CString	procs = getProcName(evChar, form->GetSymbolName());
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		initialize(screen);
		return screen->m_vbe->DoProcedure(sData);
	}
	return true;
}

bool CScript::OnProcedure(CScreen* screen, CString procs)
{
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		return screen->m_vbe->DoProcedure(sData);
	}
	return false;
}

bool CScript::Procedure(class CScreen* screen, CString procs, CString data, int count)
{
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
	{
		m_script = false;
		return screen->m_vbe->DoProcedure(sData);
	}
	return false;
}

bool CScript::IsAvailable(CScreen* screen, int event, CamBase* form)
{
	if(form == NULL)
		return false;

	CString	procs = getProcName(event, form->GetSymbolName());
	CString sData = CString(screen->m_mSCRIPT[string(procs)]);
	if (!sData.IsEmpty())
		return true;

	return false;
	//return screen->m_vbe->IsAvailable(getProcName(event, form ? form->GetSymbolName() : _T("")));
}

CString CScript::getProcName(int event, CString symbol)
{
	CString	procs;

	switch (event)
	{
	case evDeclaretion:
		procs = _T("_am_ondeclaration_am_");	break;
	case evStart:
		//procs = _T("AX_SUB_OnStart_AX_");	break;
		procs = _T("_am_onload_am_");		break;
	case evFocus:
		procs = _T("AX_SUB_OnFocus_AX_");	break;
	case evClose:
//		procs = _T("AX_SUB_OnClose_AX_");	break;
		procs = _T("_am_onclose_am_");		break;
	case evSend:
//		procs = _T("AX_SUB_OnSend_AX_");	break;
		procs = _T("_am_onsend_am_");		break;
	case evReceive:
//		procs = _T("AX_SUB_OnReceive_AX_");	break;
		procs = _T("_am_onrecv_am_");	break;
	case evKey:
		if (symbol.IsEmpty())
			procs = _T("AX_SUB_OnKey_AX_");
		else
			procs.Format(_T("AX_%s_OnKey_AX_"), symbol);
		break;
	case evService:
		procs = _T("AX_SUB_OnService_AX_");	break;
	case evApprove:
		procs = _T("AX_SUB_OnApprove_AX_");	break;
	case evDevice:
//		procs = _T("AX_SUB_OnDevice_AX_");	break;
		procs = _T("_am_ondeviec_am_");		break;

	case evTimer:
//		procs = _T("AX_SUB_OnTimer_AX_");	break;
		procs = _T("_am_ontimer_am_");		break;
	case evSelect:
		procs = _T("AX_SUB_OnSelect_AX_");	break;
	case evFile:
		procs = _T("AX_SUB_OnFile_AX_");	break;
	case evAlert:
//		procs = _T("AX_SUB_OnAlert_AX_");	break;
		procs = _T("_am_onalert_am_");		break;
	default:
		if (!symbol.IsEmpty())
		{
			switch (event)
			{
			case evClick:
				procs.Format(_T("_am_onclick_am_%s"), symbol);		break;		
			case evChange:
				procs.Format(_T("_am_onchange_am_%s"), symbol);	break;
			case evDblClk:
				procs.Format(_T("_am_onlongtap_am_%s"), symbol);	break;
			case evChar:
				procs.Format(_T("_am_onchar_am_%s"), symbol);	break;
			case evMouseDn:
			case evMouseUp:
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

CString CScript::LuaDefine()
{
	CString text;
	text =	"NORMALVIEW = 0\n";
	text +=	"POPUPVIEW  = 1\n";
	text += "DIALOGVIEW = 2\n";
	text += "USERPOS = -2\n";
	text += "FULLPOS = -1\n";
	text += "CENTERPOS = 0\n";
	text += "LEFTPOS = 1\n";
	text += "RIGHTPOS = 2\n";
	text += "TOPPOS = 4\n";
	text += "BOTTOMPOS = 8\n";
	text += "XCENTERPOS = 16\n";
	text += "YCENTERPOS = 32\n";
	text += "BUILDERFORM = 1\n";
	text += "CUSTOMFORM = 2\n";
	text += "METRICSWIDTH = 1\n";
	text += "METRICSHEIGHT = 2\n";
	text += "INTRIGGER   = 1\n";
	text += "OUTTRIGGER  = 2\n";
	text += "ALLTRIGGER  = 3\n";
	text += "OKMSGBOX    = 1\n";
	text += "YESNOMSGBOX = 2\n";
	text += "FADEANIMATION  = 1\n";
	text += "ZOOMANIMATION  = 2\n";
	text += "SLIDEANIMATION = 3\n";
	text += "DROPANIMATION  = 4\n";
	text += "FLIPANIMATION  = 5\n";
	text += "CURLANIMATION  = 6\n";
	text += "ROTATEANIMATION = 7\n";
	text += "NONEEFFECT = 0\n";
	text += "BOUNCEEFFECT = 1\n";
	text += "OVERSHOOTEFFECT = 2\n";
	text += "ACCELERATEEFFECT = 3\n";
	text += "DECELERATEEFFECT = 4\n";
	text +=	"PORTRAIT = 0\n";
	text +=	"LANDSCAPE = 1\n";
	text += "INITGRID = 0\n";
	text += "ADDTOP = 1\n";
	text += "ADDBOTTOM = 2\n";
	return text;
}