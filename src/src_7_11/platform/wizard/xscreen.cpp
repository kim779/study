// xscreen.cpp : implementation file
//

#include "stdafx.h"
#include "Wizard.h"
#include "xscreen.h"
#include "screen.h"
#include "client.h"
#include "script.h"
#include "stream.h"
#include "mouse.h"

#include "../dll/form/fmgrid.h"
#include "../dll/form/fmctrl.h"
#include "../dll/vbs/scriptEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxScreen

IMPLEMENT_DYNCREATE(CxScreen, CCmdTarget)

CxScreen::CxScreen(CScreen* screen)
{
	EnableAutomation();
	m_screen  = screen;
	m_guard   = m_screen->m_guard;
	m_return  = TRUE;

	m_pBytes  = NULL;
	m_service = svREADY;
}

CxScreen::~CxScreen()
{
	if (m_pBytes)
	{
		delete [] m_pBytes;
		m_pBytes = NULL;
	}
}


void CxScreen::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CxScreen, CCmdTarget)
	//{{AFX_MSG_MAP(CxScreen)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CxScreen, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CxScreen)
	DISP_PROPERTY_EX(CxScreen, "Key", _getKey, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CxScreen, "OnSend", _getOnSend, _setOnSend, VT_BOOL)
	DISP_PROPERTY_EX(CxScreen, "Wait", _getWait, _setWait, VT_BOOL)
	DISP_PROPERTY_EX(CxScreen, "Major", _getMajor, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CxScreen, "Minor", _getMinor, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CxScreen, "Certify", _getCertify, _setCertify, VT_BOOL)
	DISP_PROPERTY_EX(CxScreen, "Trigger", GetNotSupported, _setTrigger, VT_BOOL)
	DISP_FUNCTION(CxScreen, "ClearAll", _ClearAll, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CxScreen, "ChangeTR", _ChangeTR, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CxScreen, "ChangePath", _ChangePath, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CxScreen, "SetLedger", _SetLedger, VT_EMPTY, VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CxScreen, "GetLedger", _GetLedger, VT_BSTR, VTS_I4 VTS_I4)
	DISP_FUNCTION(CxScreen, "ChangeMap", _ChangeMap, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CxScreen, "SetMessage", _SetMessage, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CxScreen, "SetMessage2", _SetMessage2, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CxScreen, "CreateWindow", _CreateWindow, VT_EMPTY, VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CxScreen, "Send", _Send, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CxScreen, "RSend", _RSend, VT_EMPTY, VTS_I4)
	DISP_FUNCTION(CxScreen, "CloseWindow", _CloseWindow, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CxScreen, "GetMapName", _GetMapName, VT_BSTR, VTS_I4)
	DISP_FUNCTION(CxScreen, "IgnoreChange", _IgnoreChange, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CxScreen, "Object", _Object, VT_DISPATCH, VTS_BSTR)
	DISP_FUNCTION(CxScreen, "GetCode", _GetCode, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_I4)
	DISP_FUNCTION(CxScreen, "GetName", _GetName, VT_BSTR, VTS_I4 VTS_BSTR VTS_I4)
	DISP_FUNCTION(CxScreen, "Service", _Service, VT_BOOL, VTS_BSTR VTS_BSTR VTS_I4 VTS_I4)
	DISP_FUNCTION(CxScreen, "Proc", _Proc, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CxScreen, "SetCaption", _SetCaption, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CxScreen, "Approve", _Approve, VT_BOOL, VTS_I4 VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4)
	DISP_FUNCTION(CxScreen, "PopupMenu", _PopupMenu, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CxScreen, "SetFont", _SetFont, VT_EMPTY, VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CxScreen, "CreateObject", _CreateObject, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CxScreen, "SetSize", _SetSize, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION(CxScreen, "Print", _Print, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CxScreen, "SetTimer", _SetTimer, VT_EMPTY, VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CxScreen, "SetTimerX", _SetTimerX, VT_EMPTY, VTS_I2 VTS_I4 VTS_BOOL)
	DISP_FUNCTION(CxScreen, "GetCodeType", _GetCodeType, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CxScreen, "UploadFile", _UploadFile, VT_I4, VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR VTS_I4)
	DISP_FUNCTION(CxScreen, "DownloadFile", _DownloadFile, VT_BOOL, VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR)
	DISP_FUNCTION(CxScreen, "IsCodeType", _IsCodeType, VT_BOOL, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CxScreen, "GetOrderType", _GetOrderType, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CxScreen, "GetCaption", _GetCaption, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CxScreen, "ServiceEx", _ServiceEx, VT_BSTR, VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CxScreen, "ProcEx", _ProcEx, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_I4)
	DISP_FUNCTION(CxScreen, "ReloadAccn", _ReloadAccn, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CxScreen, "RegisterRTM", _RegisterRTM, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IxScreen to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {892925C0-9BED-4662-9932-DB3E6ECB329B}
static const IID IID_IxScreen =
{ 0x892925c0, 0x9bed, 0x4662, { 0x99, 0x32, 0xdb, 0x3e, 0x6e, 0xcb, 0x32, 0x9b } };

BEGIN_INTERFACE_MAP(CxScreen, CCmdTarget)
	INTERFACE_PART(CxScreen, IID_IxScreen, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CxScreen message handlers

long CxScreen::_getKey() 
{
	return m_screen->m_client->m_vms.key;
}

BOOL CxScreen::_getOnSend() 
{
	return m_return;
}

void CxScreen::_setOnSend(BOOL bNewValue) 
{
	m_return  = bNewValue;
}

BOOL CxScreen::_getWait() 
{
	return (m_screen->m_state & waitSN);
}

void CxScreen::_setWait(BOOL bNewValue) 
{
	if (bNewValue)
		m_screen->m_client->WaitState(m_screen, false);
	else
		m_screen->m_client->WaitDone(m_screen, false);
}

long CxScreen::_getMajor() 
{
	return m_screen->m_client->m_key;
}

long CxScreen::_getMinor() 
{
	return m_screen->m_key;
}

BOOL CxScreen::_getCertify() 
{
	return (m_screen->m_mapH->options & OP_CERTIFY);
}

void CxScreen::_setCertify(BOOL bNewValue) 
{
	if (bNewValue)
		m_screen->m_mapH->options |= OP_CERTIFY;
	else
		m_screen->m_mapH->options &= ~OP_CERTIFY;
}

void CxScreen::_setTrigger(BOOL bNewValue) 
{
	if (bNewValue)	
		m_screen->m_client->m_status &= ~S_XTRI;
	else
		m_screen->m_client->m_status |= S_XTRI;
}

void CxScreen::_ClearAll(long dest, long type) 
{
	CScreen* screen;
	int	value;

	switch (dest)
	{
	case targetMAIN:
		if (m_screen->m_client->GetAtScreen(screen))
		{
			screen->ClearForm(type);
			break;
		}
	case targetTHIS:
		m_screen->ClearForm(type);
		break;
	case targetALL:
		for (value = 0; value < m_screen->m_client->m_magic; value++)
		{
			if (m_screen->m_client->GetAtScreen(screen, value))
				screen->ClearForm(type);
		}
	default:
		break;
	}
}

void CxScreen::_ChangeTR(LPCTSTR trN) 
{
	CString	str = trN;

	if (!str.IsEmpty())
		m_screen->ChangeTR((char *)str.operator LPCTSTR());
}

void CxScreen::_ChangePath(LPCTSTR path) 
{
	CString	str = path;
	struct	_toPath {
		char	strName[8];
		BYTE	defValue;
	} toPath[] = {
			{"AXIS",	TP_AXIS},	{"BP1",		TP_AXIS+1},
			{"BP2",		TP_AXIS+2},	{"BP3",		TP_AXIS+3},
			{"BP4",		TP_AXIS+4},	{"BP5",		TP_AXIS+5},
			{"BP6",		TP_AXIS+6},	{"BP7",		TP_AXIS+7},
			{"FEP",		TP_FEP}
	};

	for (int ii = 0; ii < sizeof(toPath)/sizeof(struct _toPath); ii++)
	{
		if (!str.CompareNoCase(toPath[ii].strName))
		{
			m_screen->ChangeTR(toPath[ii].defValue);
			break;
		}
	}
}

void CxScreen::_SetLedger(long pos, long length, LPCTSTR data) 
{
	m_screen->SetLedger(pos, length, (char *)data);
}

BSTR CxScreen::_GetLedger(long pos, long length) 
{
	CString strResult;

	strResult = m_screen->GetLedger(pos, length);
	return strResult.AllocSysString();
}

void CxScreen::_ChangeMap(LPCTSTR mapN) 
{
	CString	str = mapN;
	if (!str.IsEmpty())
	{
		m_guard->m_dragbys = str;
		m_guard->PostAxis(renVIEW, m_screen->m_client->m_key, (LPARAM)m_guard->m_dragbys.operator LPCTSTR());
	}
}

void CxScreen::_SetMessage(LPCTSTR msg) 
{
	m_guard->m_status |= WS_GUIDE;
	m_guard->SetGuide(CString(msg), m_screen->m_client->m_key);
}

void CxScreen::_SetMessage2(long level, LPCTSTR msg) 
{
	m_guard->SetGuide(msg, m_screen->m_client->m_key, level);
}

void CxScreen::_CreateWindow(long type, LPCTSTR param, LPCTSTR data) 
{
	CString	text = param;

	if (!text.IsEmpty())
	{
		int	idx;

		if ((idx = text.Find("/d")) != -1)
		{
			int	value;
			CfmBase* form;
			CString	domino, names, codes, tmps;

			domino.Empty();
			tmps = data;
			while (!tmps.IsEmpty())
			{
				value = tmps.Find('\n');
				if (value != -1)
				{
					names = tmps.Left(value++);
					tmps  = tmps.Mid(value);
				}
				else
				{
					names = tmps;
					tmps.Empty();
				}

				value = names.Find('\t');
				if (value != -1)
				{
					domino += names;
					domino += '\n';
				}
				else
				{
					names.TrimLeft();
					names.TrimRight();
					if (m_screen->FindForm(names, form))
					{
						form->ReadData(codes);
						domino += names;	domino += '\t';
						domino += codes;	domino += '\n';
					}
				}
			}

			if (domino.IsEmpty())
				text.Delete(idx, 2);
			else
				text.Insert(idx+2, domino);
		}
		m_guard->OpenView(m_screen->m_client, text, type);
	}
}

void CxScreen::_Send(long target) 
{
	if (m_screen->m_client->m_vm->m_script)
		return;		// ignore dup

	CScreen* screen;
	switch (target)
	{
	case targetMAIN:
		if (m_screen->m_client->GetAtScreen(screen))
			break;
	case targetTHIS:
		screen = m_screen;
		break;
	case targetALL:
		m_screen->m_client->m_stream->InStream();
	default:
		return;		// invalid script
	}

	m_screen->m_client->m_stream->InStream(screen);
}

void CxScreen::_RSend(long target) 
{
	if (m_screen->m_client->m_vm->m_script)
		return;		// ignore dup

	CScreen* screen;
	switch (target)
	{
	case targetMAIN:
		if (m_screen->m_client->GetAtScreen(screen))
			break;
	case targetTHIS:
		screen = m_screen;
		break;
	case targetALL:
		m_screen->m_client->m_stream->InStream();
	default:
		return;		// invalid script
	}
	m_screen->m_client->OnSend(screen);
}

void CxScreen::_CloseWindow() 
{
	m_guard->PostAxis(delVIEW, m_screen->m_client->m_key, (LPARAM)0);
}

BSTR CxScreen::_GetMapName(long target) 
{
	CString strResult = _T("");
	CScreen* screen;

	switch (target)
	{
	case targetMAIN:
	default:
		if (m_screen->m_client->GetAtScreen(screen))
			break;
	case targetTHIS:
		screen = m_screen;
		break;
	}

	if (screen)
		strResult = CString(screen->m_mapH->mapN, L_MAPN);

	return strResult.AllocSysString();
}

void CxScreen::_IgnoreChange(BOOL ignore) 
{
	if (ignore)
		m_screen->m_client->m_status |= S_IGNORE;
	else
		m_screen->m_client->m_status &= ~S_IGNORE;
}

LPDISPATCH CxScreen::_Object(LPCTSTR forms) 
{
	int	value;
	CString	str, name;
	CScreen* screen;
	struct	_depths {
		char	strN[32];
		BYTE	depth;
	} depths[] = {
		{ "PARENT.PARENT.PARENT.",	dpGREAT  },
		{ "PARENT.PARENT.",		dpGRAND  },
		{ "PARENT.",			dpPARENT },
		{ "MAIN.",			dpMAIN   }
	};

	value  = dpCURRENT;
	screen = m_screen;
	str    = name = forms;
	str.MakeUpper();
	name.Replace(".", "_");
	for (int ii = 0; ii < sizeof(depths)/sizeof(struct _depths); ii++)
	{
		if (str.Find(depths[ii].strN) == 0)
		{
			value = depths[ii].depth;
			str   = str.Mid((int)strlen(depths[ii].strN));
			break;
		}
	}

	if (m_screen->m_client->FindScreen(value, str, screen))
	{
		value = str.ReverseFind('.');
		if (value != -1)
			str = str.Mid(value+1);

		CfmBase* form;
		if (screen->FindForm(str, form))
		{
			if (form->m_form->kind == FM_CONTROL)
			{
				CWnd* ctrl = ((CfmCtrl *)form)->m_ctrl;
				if (ctrl)
				{
					m_screen->m_vbe->AddObject(name, ctrl);
					return ctrl->GetIDispatch(TRUE);
				}
			}
			else
			{
				m_screen->m_vbe->AddObject(name, form);
				return form->GetIDispatch(TRUE);
			}
		}
	}

	return NULL;
}

BOOL CxScreen::_GetCode(long kind, LPCTSTR codeSymbol, LPCTSTR nameSymbol, long type) 
{
	CString	str;
	CfmBase* form;
	CRect	rect;
	bool	istrue = false;

	switch (kind)
	{
	case indexCODE:
	case futureCODE:
		istrue = true;
		break;
	default:
		break;
	}

	rect.SetRectEmpty();
	if (m_screen->m_client->m_mouse->m_mouse.m_idx >= 0)
	{
		form = m_screen->GetAtForm(m_screen->m_client->m_mouse->m_mouse.m_idx);
		rect = form->GetRect();
		if (form->m_form->kind == FM_GRID)
			rect.bottom = rect.top;
	}
	m_screen->m_client->m_view->ClientToScreen(&rect);
	if (m_guard->GetCode(kind, str, type, CPoint(rect.left, rect.bottom+1)))
		return dialogAction(istrue, str, codeSymbol, nameSymbol);

	return FALSE;
}

BSTR CxScreen::_GetName(long kind, LPCTSTR code, long type) 
{
	int	value;
	CString strResult, str;

	str = code;
	strResult = _T("");
	if (!str.IsEmpty())
		m_guard->GetName(kind, str, strResult, value);

	return strResult.AllocSysString();
}

long CxScreen::_GetCodeType(LPCTSTR code) 
{
	int	type;
	CString	str, name;
	
	str = code;
	if (!str.IsEmpty())
	{
		m_guard->GetName(whichTYPE, str, name, type);
		return type;
	}

	return 0;
}

BOOL CxScreen::_Service(LPCTSTR trN, LPCTSTR data, long length, long mode)
{
	if (m_screen->m_client->m_vm->m_script)
		return FALSE;		// ignore dup

	return m_guard->Service(m_screen, trN, (char *)data, length, mode);
}

void CxScreen::_Proc(LPCTSTR procs, LPCTSTR data) 
{
	int	value;
	CString	string, name;
	CScreen* screen;
	struct	_depths {
		char	strN[32];
		BYTE	depth;
	} depths[] = {
		{ "PARENT.PARENT.PARENT.",	dpGREAT  },
		{ "PARENT.PARENT.",		dpGRAND  },
		{ "PARENT.",			dpPARENT },
		{ "MAIN.",			dpMAIN   }
	};

	string = procs;
	value = string.ReverseFind('.');
	if (value != -1)
		name = string.Mid(value+1);
	else
	{
		name = string;
		string.Empty();
	}

	value  = dpCURRENT;
	screen = m_screen;
	string.MakeUpper();
	string.Replace(".PROC.", ".");
	for (int ii = 0; ii < sizeof(depths)/sizeof(struct _depths); ii++)
	{
		if (string.Find(depths[ii].strN) == 0)
		{
			value  = depths[ii].depth;
			string = string.Mid((int)strlen(depths[ii].strN));
			break;
		}
	}

	if (m_screen->m_client->FindScreen(value, string, screen))
		screen->m_client->m_vm->Procedure(screen, name, data);
}

void CxScreen::_ProcEx(LPCTSTR procs, LPCTSTR data, long count) 
{
	int	value;
	CString	string, name;
	CScreen* screen;
	struct	_depths {
		char	strN[32];
		BYTE	depth;
	} depths[] = {
		{ "PARENT.PARENT.PARENT.",	dpGREAT  },
		{ "PARENT.PARENT.",		dpGRAND  },
		{ "PARENT.",			dpPARENT },
		{ "MAIN.",			dpMAIN   }
	};

	string = procs;
	value = string.ReverseFind('.');
	if (value != -1)
		name = string.Mid(value+1);
	else
	{
		name = string;
		string.Empty();
	}

	value  = dpCURRENT;
	screen = m_screen;
	string.MakeUpper();
	for (int ii = 0; ii < sizeof(depths)/sizeof(struct _depths); ii++)
	{
		if (string.Find(depths[ii].strN) == 0)
		{
			value  = depths[ii].depth;
			string = string.Mid((int)strlen(depths[ii].strN));
			break;
		}
	}

	if (m_screen->m_client->FindScreen(value, string, screen))
		screen->m_client->m_vm->Procedure(screen, name, data, count);
}

void CxScreen::_SetCaption(LPCTSTR caption) 
{
	m_guard->SendAxis(MAKEWPARAM(titleVIEW, m_screen->m_client->m_key), (WPARAM)caption);
}

BOOL CxScreen::_Approve(long key, LPCTSTR ip, LPCTSTR map, LPCTSTR data, long length) 
{
	return m_guard->Approve(m_screen, key, ip, map, (char *)data, length);
}

BSTR CxScreen::_PopupMenu(LPCTSTR menus, LPCTSTR symbol) 
{
	CMenu*	popM;
	CString	text, string;
	CString strResult = _T("");
	CStringArray ids;
	int	value, base = 20000;

	popM = new CMenu();
	popM->CreatePopupMenu();
	text = CString(menus);
	ids.RemoveAll();
	for (int ii = 0 ; !text.IsEmpty(); ii++)
	{
		value = text.Find('\t');
		if (value != -1)
		{
			string = text.Left(value);
			text   = text.Mid(value+1);
		}
		else
		{
			string = text;
			text.Empty();
		}

		value = string.Find('|');
		if (value == -1 || !string.Find('-'))
		{
			ids.Add(_T(""));
			popM->AppendMenu(MF_SEPARATOR, 0);
		}
		else
		{
			ids.Add(string.Mid(value+1));
			popM->AppendMenu(MF_STRING|MF_ENABLED, base+ii, string.Left(value)); 
		}
	}

	if (ids.GetSize() > 0)
	{
		CPoint	pt;

		text = CString(symbol);
		if (!text.IsEmpty())
		{
			CRect	rect;
			CfmBase* form;

			if (m_screen->FindForm(text, form))
			{
				rect = form->GetRect();
				pt = CPoint(rect.left, rect.bottom);
			}
		}
		else
			pt = m_screen->m_cursor;

		m_screen->m_client->m_view->ClientToScreen(&pt);
		value  = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, pt.x, pt.y, m_screen->m_client->m_view);
		if (value >= base)
			strResult = ids.GetAt(value-base);
	}

	delete popM;
	return strResult.AllocSysString();
}

void CxScreen::_SetFont(long point, LPCTSTR name) 
{
	CScreen* screen;

	for (int key = 0; key < m_screen->m_client->m_magic; key++)
	{
		if (m_screen->m_client->GetAtScreen(screen, key))
			screen->SetFont(m_guard->m_resize, point, -1, name);
	}
}

BOOL CxScreen::_CreateObject(LPCTSTR name) 
{
	CCmdTarget*	target;

	if (m_guard->CreateObject(name, target))
	{
		m_screen->m_vbe->AddObject(name, target);
		return TRUE;
	}
	return FALSE;
}

void CxScreen::_SetSize(long width, long height) 
{
	width  = (long)(width * m_screen->m_client->m_hR);
	height = (long)(height * m_screen->m_client->m_vR);
	m_screen->m_client->m_guard->SendAxis(sizeVIEW, m_screen->m_client->m_key, MAKELPARAM(width, height));
}

void CxScreen::_Print(LPCTSTR text) 
{
	m_screen->m_client->m_guard->SendAxis(printVIEW, m_screen->m_client->m_key, (LPARAM)text);
}

void CxScreen::_SetTimer(long interval, BOOL main)
{
	if (main)
	{
		m_screen->m_view->KillTimer(TM_VB);
		m_screen->m_view->SetTimer(TM_VB, interval*100, NULL);
	}
	else
	{
		m_screen->m_view->KillTimer(TM_VB+m_screen->m_key);
		m_screen->m_view->SetTimer(TM_VB+m_screen->m_key, interval*100, NULL);
	}
}

void CxScreen::_SetTimerX(short id, long interval, BOOL main)
{
	int	tid;

	if (main)
		id += TM_VBx;
	else
		id += (TM_VBx * (m_screen->m_key+1));

	tid = id;
	m_screen->m_view->KillTimer(id);
	m_screen->m_view->SetTimer(id, interval, NULL);
	m_screen->m_client->m_timers.SetAt(id, tid);
}

long CxScreen::_UploadFile(LPCTSTR trN, LPCTSTR data, long length, long mode, LPCTSTR fileN, long offset) 
{
	return m_guard->UploadFile(m_screen, trN, (char *)data, length, mode, (char *)fileN, offset);
}

BOOL CxScreen::_DownloadFile(LPCTSTR trN, LPCTSTR data, long length, long mode, LPCTSTR fileN) 
{
	return m_guard->DownloadFile(m_screen, trN, (char *)data, length, mode, fileN);
}

BOOL CxScreen::_IsCodeType(LPCTSTR code, long type) 
{
	CString	str;

	str = code;
	return m_guard->GetCode(isCODEtype, str, type, CPoint(0,0), true);
}

long CxScreen::_GetOrderType(LPCTSTR code) 
{
	int	type;
	CString	str, name;
	
	str = code;
	if (!str.IsEmpty())
	{
		m_guard->GetName(whichORDER, str, name, type);
		return type;
	}
	return 0;
}

BSTR CxScreen::_GetCaption() 
{
	CString strResult;

	m_screen->m_view->GetParent()->GetWindowText(strResult);
	return strResult.AllocSysString();
}

BSTR CxScreen::_ServiceEx(LPCTSTR trN, LPCTSTR data, long length, long mode, long timeout) 
{
	CString strResult = _T("");

	switch (m_service)
	{
	case svREADY:
	case svTIMEOUT:
		if (_Service(trN, data, length, mode | US_PASS) && timeout)
		{
			m_screen->m_client->WaitState(m_screen, false);
			m_service = svWAIT;
			break;
		}
		m_service = svREADY;
	default:
		return strResult.AllocSysString();
	}

	MSG	msg;
	HWND	hWnd;
	DWORD	elapse;

	elapse = GetTickCount();
	hWnd   = m_screen->m_view->GetSafeHwnd();

	while (m_service != svDONE)
	{
		if (!::IsWindow(hWnd) || (timeout > 0 && GetTickCount() - elapse > (DWORD)timeout))
		{
			m_service = svTIMEOUT;
			return strResult.AllocSysString();
		}

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			switch (msg.message)
			{
			case WM_CHAR:
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_MOUSEMOVE:
			case WM_MOUSEWHEEL:
				if (hWnd == msg.hwnd)
					break;
			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
			default:
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				break;
			}
		}
	}

	if (m_pBytes)
	{
		strResult = CString(m_pBytes, m_nBytes);
		delete [] m_pBytes;
		m_pBytes  = NULL;
		m_service = svREADY;
	}
	return strResult.AllocSysString();
}

bool CxScreen::OnService(char* pBytes, int nBytes)
{
	if (pBytes == NULL)
		return (m_service == svWAIT) ? true : false;

	switch (m_service)
	{
	case svTIMEOUT:
		m_service = svREADY;
		return true;
	case svWAIT:
		break;
	default:
		return false;
	}

	if (m_pBytes)
		delete [] m_pBytes;

	m_nBytes = nBytes;
	m_pBytes = new char[nBytes];
	CopyMemory(m_pBytes, pBytes, nBytes);
	m_service = svDONE;
	m_screen->m_client->WaitDone(m_screen, false, true);
	return true;
}

BOOL CxScreen::dialogAction(bool isKOSPI, CString data, CString code, CString name)
{
	if (!m_screen)
		return FALSE;

	int	value;
	BOOL	result = FALSE;
	CString	tmps = _T("");

	value = data.Find('\t'); 
	if (value < 0)
	{
		tmps = data;
		data.Empty();
	}
	else
	{
		tmps = data.Left(value);
		data = data.Mid(value+1);
	}

	CfmBase	*fmCode, *fmName;
	if (m_screen->FindForm(code, fmCode))
	{
		tmps.TrimRight();
		value = tmps.GetLength();
		if (isKOSPI && fmCode->m_form->size)
		{
			value = min(value, fmCode->m_form->size);
			tmps  = tmps.Right(value);
		}
		fmCode->WriteData(tmps);
		if (m_screen->m_client->m_status & S_IGNORE)
			fmCode->IsChanged();
		result = TRUE;
	}

	value = data.Find('\t'); 
	if (value < 0)
	{
		tmps = data;
		data.Empty();
	}
	else
	{
		tmps = data.Left(value);
		data = data.Mid(value+1);
	}

	if (m_screen->FindForm(name, fmName))
	{
		tmps.TrimRight();
		fmName->WriteData(tmps);
	}

	return result;
}

bool CxScreen::findForm(int parent, CString name, CfmBase*& form)
{
	CScreen* screen = m_screen;
	if (!m_screen->m_client->FindScreen(parent, name, screen))
		return false;
	return screen->FindForm(name, form);
}

// updateX_20080225_for eu2s
void CxScreen::_ReloadAccn(LPCTSTR name) 
{
	CString	str = name;
	if (!str.IsEmpty())
	{
		m_multiUse = str;
		m_guard->PostAxis(reloadACCN, m_screen->m_client->m_key, (LPARAM)m_multiUse.operator LPCTSTR());
	}
}

void CxScreen::_RegisterRTM(LPCTSTR codes) 
{
	if (m_guard->m_cast && m_guard->m_cast->IsRunning())
		m_guard->m_cast->RegisterRTM(m_screen->m_client->m_key, m_screen->m_key, codes);
}
