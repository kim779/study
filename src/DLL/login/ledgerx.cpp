// ledgerx.cpp : implementation file
//

#include "stdafx.h"
#include "axislogin.h"
#include "ledgerx.h"
#include "../../h/mapform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLedger

IMPLEMENT_DYNCREATE(CLedger, CCmdTarget)

CLedger::CLedger(void* data)
{
	EnableAutomation();

	m_ledger = new char[L_ledgerH];
	FillMemory(m_ledger, L_ledgerH, ' ');
	parseParam((char *)data);

	m_text.Empty();
}

CLedger::~CLedger()
{
	delete[] m_ledger;
}


void CLedger::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CLedger, CCmdTarget)
	//{{AFX_MSG_MAP(CLedger)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLedger, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CLedger)
	DISP_PROPERTY_EX(CLedger, "Type", _getType, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Svcd", _getSvcd, _setSvcd, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Cono", _getCono, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Usid", _getUsid, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Cogb", _getCogb, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Puip", _getPuip, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Pcip", _getPcip, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Dept", _getDept, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Brno", _getBrno, _setBrno, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Lang", _getLang, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Time", _getTime, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Ecod", _getEcod, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Eknd", _getEknd, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Fkey", _getFkey, _setFkey, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Rcnt", _getRcnt, _setRcnt, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Next", _getNext, _setNext, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Nkey", _getNkey, _setNkey, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Hsiz", _getHsiz, _setHsiz, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Odrf", _getOdrf, _setOdrf, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Mkty", _getMkty, _setMkty, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Svrm", _getSvrm, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Eact", _getEact, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Emsg", _getEmsg, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Skip", _getSkip, _setSkip, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Term", _getTerm, SetNotSupported, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "Epwd", _getEpwd, _setEpwd, VT_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ILedger to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {F26FD729-BC91-46EF-B4D2-6AA9FAAEB017}
static const IID IID_ILedger =
{ 0xf26fd729, 0xbc91, 0x46ef, { 0xb4, 0xd2, 0x6a, 0xa9, 0xfa, 0xae, 0xb0, 0x17 } };


BEGIN_INTERFACE_MAP(CLedger, CCmdTarget)
	INTERFACE_PART(CLedger, IID_ILedger, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLedger message handlers

BSTR CLedger::_getType() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->type, sizeof(ledgerH->type));
	return strResult.AllocSysString();
}

BSTR CLedger::_getSvcd() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->svcd, sizeof(ledgerH->svcd));
	return strResult.AllocSysString();
}

void CLedger::_setSvcd(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	CopyMemory(ledgerH->svcd, lpszNewValue, min(sizeof(ledgerH->svcd), strlen(lpszNewValue)));
}

BSTR CLedger::_getCono() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->cono, sizeof(ledgerH->cono));
	return strResult.AllocSysString();
}

BSTR CLedger::_getUsid() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->usid, sizeof(ledgerH->usid));
	return strResult.AllocSysString();
}

BSTR CLedger::_getCogb() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->cogb, sizeof(ledgerH->cogb));
	return strResult.AllocSysString();
}

BSTR CLedger::_getPuip() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->puip, sizeof(ledgerH->puip));
	return strResult.AllocSysString();
}

BSTR CLedger::_getPcip() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->puip, sizeof(ledgerH->puip));
	return strResult.AllocSysString();
}

BSTR CLedger::_getDept() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->dept, sizeof(ledgerH->dept));
	return strResult.AllocSysString();
}

BSTR CLedger::_getBrno() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->brno, sizeof(ledgerH->brno));
	return strResult.AllocSysString();
}

void CLedger::_setBrno(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	CopyMemory(ledgerH->brno, lpszNewValue, min(sizeof(ledgerH->brno), strlen(lpszNewValue)));
}

BSTR CLedger::_getTerm() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->term, sizeof(ledgerH->term));
	return strResult.AllocSysString();
}

BSTR CLedger::_getLang() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->lang, sizeof(ledgerH->lang));
	return strResult.AllocSysString();
}

BSTR CLedger::_getTime() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->time, sizeof(ledgerH->time));
	return strResult.AllocSysString();
}

BSTR CLedger::_getEcod() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->ecod, sizeof(ledgerH->ecod));
	return strResult.AllocSysString();
}

BSTR CLedger::_getEknd() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->eknd, sizeof(ledgerH->eknd));
	return strResult.AllocSysString();
}

BSTR CLedger::_getFkey() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->fkey, sizeof(ledgerH->fkey));
	return strResult.AllocSysString();
}

void CLedger::_setFkey(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	CopyMemory(ledgerH->fkey, lpszNewValue, min(sizeof(ledgerH->fkey), strlen(lpszNewValue)));
}

BSTR CLedger::_getRcnt() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->rcnt, sizeof(ledgerH->rcnt));
	return strResult.AllocSysString();
}

void CLedger::_setRcnt(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	CopyMemory(ledgerH->rcnt, lpszNewValue, min(sizeof(ledgerH->rcnt), strlen(lpszNewValue)));
}

BSTR CLedger::_getNext() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->next, sizeof(ledgerH->next));
	return strResult.AllocSysString();
}

void CLedger::_setNext(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	CopyMemory(ledgerH->next, lpszNewValue, min(sizeof(ledgerH->next), strlen(lpszNewValue)));
}

BSTR CLedger::_getNkey() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->nkey, sizeof(ledgerH->nkey));
	return strResult.AllocSysString();
}

void CLedger::_setNkey(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	CopyMemory(ledgerH->nkey, lpszNewValue, min(sizeof(ledgerH->nkey), strlen(lpszNewValue)));
}

BSTR CLedger::_getOdrf() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->odrf, sizeof(ledgerH->odrf));
	return strResult.AllocSysString();
}

void CLedger::_setOdrf(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	CopyMemory(ledgerH->odrf, lpszNewValue, min(sizeof(ledgerH->odrf), strlen(lpszNewValue)));
}

BSTR CLedger::_getHsiz() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->hsiz, sizeof(ledgerH->hsiz));
	return strResult.AllocSysString();
}

void CLedger::_setHsiz(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	CopyMemory(ledgerH->hsiz, lpszNewValue, min(sizeof(ledgerH->hsiz), strlen(lpszNewValue)));
}

BSTR CLedger::_getMkty() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->mkty, sizeof(ledgerH->mkty));
	return strResult.AllocSysString();
}

void CLedger::_setMkty(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	CopyMemory(ledgerH->mkty, lpszNewValue, min(sizeof(ledgerH->mkty), strlen(lpszNewValue)));
}

BSTR CLedger::_getSvrm() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->svrm, sizeof(ledgerH->svrm));
	return strResult.AllocSysString();
}

BSTR CLedger::_getEact() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->eact, sizeof(ledgerH->eact));
	return strResult.AllocSysString();
}

BSTR CLedger::_getEmsg() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->emsg, sizeof(ledgerH->emsg));
	return strResult.AllocSysString();
}

BSTR CLedger::_getSkip() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->skip, sizeof(ledgerH->skip));
	return strResult.AllocSysString();
}

void CLedger::_setSkip(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	CopyMemory(ledgerH->skip, lpszNewValue, min(sizeof(ledgerH->skip), strlen(lpszNewValue)));
}

BSTR CLedger::_getEpwd() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->epwd, sizeof(ledgerH->epwd));
	return strResult.AllocSysString();
}

void CLedger::_setEpwd(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	CopyMemory(ledgerH->epwd, lpszNewValue, min(sizeof(ledgerH->epwd), strlen(lpszNewValue)));
}

void CLedger::LedgerEx(void* data)
{
//	struct	_ledgerH* ledgerH = (struct _ledgerH *)m_ledger;
//	CopyMemory(ledgerH->screen, data, sizeof(ledgerH->screen));
}

CString CLedger::GetLedger(int pos, int length)
{
	struct	_ledgerH* ledgerH = (struct _ledgerH *)m_ledger;

	if (length <= 0)
	{
		pos    = 0;
		length = L_ledgerH;
	}

	m_text.Empty();
	if (pos >= 0 && length >= 0 && pos+length <= L_ledgerH)
		m_text = CString(&m_ledger[pos], length);

	return m_text;
}

CString CLedger::GetLedgerEx(int id, void* data)
{
	struct	_ledgerH* ledgerH;
	
	if (data == NULL)
		ledgerH = (struct _ledgerH *)m_ledger;
	else
		ledgerH = (struct _ledgerH *)data;

	m_text.Empty();
	switch (id)
	{
	case getOK:
		m_text = _T("1");			break;
	case getSIZE:
		m_text.Format("%d", L_ledgerH);		break;
	case getMsg:
		m_text = CString(ledgerH->emsg, sizeof(ledgerH->emsg));	break;
	case getEtype:
		m_text = CString(ledgerH->eact, sizeof(ledgerH->eact));	break;
	case getEcode:
		m_text = CString(ledgerH->ecod, sizeof(ledgerH->ecod));	break;
//	case getTimeR:
//		m_text = CString(ledgerH->time_r, sizeof(ledgerH->time_r));	break;
	}

	CString slog;
	slog.Format("[LEDGER] id = [%d] m_text =[%s] ", id, m_text);
	OutputDebugString(slog);
	return m_text;
}

void CLedger::SetLedger(int pos, int length, void* data)
{
	CString slog;
	slog.Format("[LEDGER] SetLedger  length =[%d]  data =[%.30s]", length, data);
	OutputDebugString(slog);

	if (length <= 0)
	{
		//test
		//return;
		pos    = 0;
		length = L_ledgerH;
	
		
	}

	if (pos >= 0 && length >= 0 && pos+length <= L_ledgerH)
		CopyMemory(&m_ledger[pos], data, length);
}

void CLedger::parseParam(void* param)
{
	struct	_mapH*	mapH;
	struct	_ledgerH* ledgerH;
	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();

	mapH = (struct _mapH *)param;
	ledgerH = (struct _ledgerH *)m_ledger;
	CopyMemory(ledgerH->svcd, &mapH->trxH[3], sizeof(ledgerH->svcd));
	CopyMemory(ledgerH->usid, app->m_user, min(app->m_user.GetLength(), sizeof(ledgerH->usid)));
	CopyMemory(ledgerH->pcip, app->m_host, min(app->m_host.GetLength(), sizeof(ledgerH->pcip)));
	CopyMemory(ledgerH->brno, app->m_brno, min(app->m_brno.GetLength(), sizeof(ledgerH->brno)));
	CopyMemory(ledgerH->cogb, app->m_cogb, min(app->m_cogb.GetLength(), sizeof(ledgerH->cogb)));
	CopyMemory(ledgerH->rcnt, "0000", sizeof(ledgerH->rcnt));
	ledgerH->fkey[0] = mapH->trxH[2];
	ledgerH->mkty[0] = mapH->trxH[0];
	ledgerH->odrf[0] = mapH->trxH[1];
}
