// ledgerx.cpp : implementation file
//

#include "stdafx.h"
#include "axislogin.h"
#include "ledgerx.h"
#include "../../h/axis.h"
#include "../../h/axisVar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma	pack(1)
struct	_mapH	{
	BYTE	btFRONT[39];   //47
	char	trxH[32];
	BYTE	btBACK[82];
};
#pragma	pack()
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
	m_iInputlen = 0;
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
	DISP_PROPERTY_EX(CLedger, "anam", _getanam, _setanam, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "snam", _getsnam, _setsnam, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "fnam", _getfnam, _setfnam, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "fncd", _getfncd, _setfncd, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "guid", _getguid, _setguid, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "ctyp", _getctyp, _setctyp, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "eflg", _geteflg, _seteflg, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "tflg", _gettflg, _settflg, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "tdat", _gettdat, _settdat, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "ttim", _Getttim, _Setttim, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "clip", _Getclip, _Setclip, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "rtyp", _Getrtyp, _Setrtyp, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "rcod", _Getrcod, _Setrcod, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "rlcd", _Getrlcd, _Setrlcd, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "rtit", _Getrtit, _Setrtit, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "rbmg", _Getrbmg, _Setrbmg, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "rdmg", _Getrdmg, _Setrdmg, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "usid", _Getusid, _Setusid, VT_BSTR)
	DISP_PROPERTY_EX(CLedger, "crst", _Getcrst, _Setcrst, VT_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_PROPERTY_EX_ID(CLedger, "fill", dispidfill, Getfill, Setfill, VT_BSTR)
	DISP_PROPERTY_EX_ID(CLedger, "tmno", dispidtmno, Gettmno, Settmno, VT_BSTR)
	DISP_PROPERTY_EX_ID(CLedger, "tmps", dispidtmps, Gettmps, Settmps, VT_BSTR)
	DISP_PROPERTY_EX_ID(CLedger, "dept", dispiddept, Getdept, Setdept, VT_BSTR)
	DISP_PROPERTY_EX_ID(CLedger, "brnc", dispidbrnc, Getbrnc, Setbrnc, VT_BSTR)
	DISP_PROPERTY_EX_ID(CLedger, "sale", dispidsale, Getsale, Setsale, VT_BSTR)
	DISP_PROPERTY_EX_ID(CLedger, "agnc", dispidagnc, Getagnc, Setagnc, VT_BSTR)
	DISP_PROPERTY_EX_ID(CLedger, "sfiller", dispidsfiller, Getsfiller, Setsfiller, VT_BSTR)
	DISP_PROPERTY_EX_ID(CLedger, "susid", dispidsusid, Getsusid, Setsusid, VT_BSTR)
	DISP_PROPERTY_EX_ID(CLedger, "menu", dispidmenu, Getmenu, Setmenu, VT_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_ILedger to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {3BF10F40-FDE9-4A44-BA55-56DE5FAEBAC9}
static const IID IID_ILedger =
{ 0x3bf10f40, 0xfde9, 0x4a44, { 0xba, 0x55, 0x56, 0xde, 0x5f, 0xae, 0xba, 0xc9 } };

BEGIN_INTERFACE_MAP(CLedger, CCmdTarget)
	INTERFACE_PART(CLedger, IID_ILedger, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLedger message handlers

BSTR CLedger::_getanam() 
{
	CString	strResult;
	struct	_ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->anam, sizeof(ledgerH->anam));  
	return strResult.AllocSysString();
}

void CLedger::_setanam(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;
	ledgerH = (struct _ledgerH *)m_ledger;

	FillMemory(ledgerH->anam, sizeof(ledgerH->anam), ' ');
	CopyMemory(ledgerH->anam, lpszNewValue, min(sizeof(ledgerH->anam), strlen(lpszNewValue)));
}

BSTR CLedger::_getsnam() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->snam, sizeof(ledgerH->snam));  
	return strResult.AllocSysString();
}

void CLedger::_setsnam(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;
	ledgerH = (struct _ledgerH *)m_ledger;
	
	FillMemory(ledgerH->snam, sizeof(ledgerH->snam), ' ');
	CopyMemory(ledgerH->snam, lpszNewValue, min(sizeof(ledgerH->snam), strlen(lpszNewValue)));
}

BSTR CLedger::_getfnam() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->fnam, sizeof(ledgerH->fnam));
	return strResult.AllocSysString();
}

void CLedger::_setfnam(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;
	ledgerH = (struct _ledgerH *)m_ledger;

	FillMemory(ledgerH->fnam, sizeof(ledgerH->fnam), ' ');  
	CopyMemory(ledgerH->fnam, lpszNewValue, min(sizeof(ledgerH->fnam), strlen(lpszNewValue)));
}

BSTR CLedger::_getfncd() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->fncd, sizeof(ledgerH->fncd));
	return strResult.AllocSysString();
}

void CLedger::_setfncd(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;
	ledgerH = (struct _ledgerH *)m_ledger;

	FillMemory(ledgerH->fncd, sizeof(ledgerH->fncd), ' ');
	CopyMemory(ledgerH->fncd, lpszNewValue, min(sizeof(ledgerH->fncd), strlen(lpszNewValue)));
}

BSTR CLedger::_getguid() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->guid, sizeof(ledgerH->guid));
	return strResult.AllocSysString();
}

void CLedger::_setguid(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;
	ledgerH = (struct _ledgerH *)m_ledger;

	FillMemory(ledgerH->guid, sizeof(ledgerH->guid), ' ');
	CopyMemory(ledgerH->guid, lpszNewValue, min(sizeof(ledgerH->guid), strlen(lpszNewValue)));
}

BSTR CLedger::_getctyp() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->ctyp, sizeof(ledgerH->ctyp));
	return strResult.AllocSysString();
}

void CLedger::_setctyp(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;
	ledgerH = (struct _ledgerH *)m_ledger;

	FillMemory(ledgerH->ctyp, sizeof(ledgerH->ctyp), ' ');
	CopyMemory(ledgerH->ctyp, lpszNewValue, min(sizeof(ledgerH->ctyp), strlen(lpszNewValue)));
}

BSTR CLedger::_geteflg() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->eflg, sizeof(ledgerH->eflg));  
	return strResult.AllocSysString();
}

void CLedger::_seteflg(LPCTSTR lpszNewValue)
{
	struct	_ledgerH* ledgerH;
	ledgerH = (struct _ledgerH *)m_ledger;

	FillMemory(ledgerH->eflg, sizeof(ledgerH->eflg), ' ');
	CopyMemory(ledgerH->eflg, lpszNewValue, min(sizeof(ledgerH->eflg), strlen(lpszNewValue)));
}

BSTR CLedger::_gettflg() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->tflg, sizeof(ledgerH->tflg));  
	return strResult.AllocSysString();
}

void CLedger::_settflg(LPCTSTR lpszNewValue)
{
	struct	_ledgerH* ledgerH;
	ledgerH = (struct _ledgerH *)m_ledger;

	FillMemory(ledgerH->tflg, sizeof(ledgerH->tflg), ' ');
	CopyMemory(ledgerH->tflg, lpszNewValue, min(sizeof(ledgerH->tflg), strlen(lpszNewValue)));
}

BSTR CLedger::_gettdat() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->tdat, sizeof(ledgerH->tdat));  
	return strResult.AllocSysString();
}

void CLedger::_settdat(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;
	ledgerH = (struct _ledgerH *)m_ledger;
	
	FillMemory(ledgerH->tdat, sizeof(ledgerH->tdat), ' ');
	CopyMemory(ledgerH->tdat, lpszNewValue, min(sizeof(ledgerH->tdat), strlen(lpszNewValue)));
}

CString CLedger::GetLedger(int pos, int length ,int inputhlen)
{
	struct	_ledgerH* ledgerH = (struct _ledgerH *)m_ledger;
	m_iInputlen = inputhlen;
	SetLedgerLen();

	int nn = L_ledgerH;
	if (length <= 0)
	{
		pos    = 0;
		length = L_ledgerH;
	}

	if (pos >= 0 && length >= 0 && pos+length <= L_ledgerH)
		m_text = CString(&m_ledger[pos], length);
	else
		m_text.Empty();

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
	case getEtype:
		m_text = _T("-1");			break;
	default:
		break;
	}
	return m_text;
}

void CLedger::SetLedgerLen() 
{
/*	CString tmpS;
	tmpS.Format(_T("%d"), L_ledgerH + m_iInputlen);
	struct _ledgerH* ledgerH = (struct _ledgerH *)m_ledger;
	
	FillMemory(ledgerH->len, sizeof(ledgerH->len), ' '); 
	CopyMemory(ledgerH->len, tmpS, min(sizeof(ledgerH->len), tmpS.GetLength()));
	*/
}

void CLedger::SetLedger(int pos, int length, void* data)
{
	struct _ledgerH* ledgerH = (struct _ledgerH *)m_ledger;

	/* test
	if (length <= 0)
	{
		CString	maps = CString((char *)data, L_MAPN);
		struct _ledgerH* ledgerTmp	= (struct _ledgerH *)data;
		CopyMemory(ledgerH->ecod, ledgerTmp->ecod, sizeof(ledgerTmp->ecod));
		CopyMemory(ledgerH->emsg, ledgerTmp->emsg, sizeof(ledgerTmp->emsg));
		return;
	}

	if ((pos == -1) && (length >= 0))
	{
		CString	maps = CString((char *)data, L_MAPN);
		if (!(length & winK_POPUP) || maps.Mid(L_SGID+L_SELC, 2) == "00")
			CopyMemory(ledgerH->scrn, maps.Mid(L_SGID), sizeof(ledgerH->scrn));
		else
			FillMemory(ledgerH->scrn, sizeof(ledgerH->scrn), ' ');
		return;
	}

	if (pos >= 0 && length >= 0 && pos+length <= L_ledgerH)
		CopyMemory(&m_ledger[pos], data, length);
		*/
}

void CLedger::parseParam(void* param)
{
	struct	_mapH*	mapH;
	struct	_ledgerH* ledgerH;
	CAxisloginApp* app = (CAxisloginApp *)AfxGetApp();

	mapH = (struct _mapH *)param;
	ledgerH = (struct _ledgerH *)m_ledger;

	int	pos;
	CString strTmp = CString(mapH->trxH, sizeof(mapH->trxH));
   
	pos = strTmp.Find(" ");
}

BSTR CLedger::_Getttim() 
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->ttim, sizeof(ledgerH->ttim)); 
	return strResult.AllocSysString();
}

void CLedger::_Setttim(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;
	ledgerH = (struct _ledgerH *)m_ledger;

	FillMemory(ledgerH->ttim, sizeof(ledgerH->ttim), ' '); 
	CopyMemory(ledgerH->ttim, lpszNewValue, min(sizeof(ledgerH->ttim), strlen(lpszNewValue)));
}

BSTR CLedger::_Getclip() 
{
	CString	strResult;
	struct	_ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->clip, sizeof(ledgerH->clip));  
	return strResult.AllocSysString();
}

void CLedger::_Setclip(LPCTSTR lpszNewValue) 
{
	struct	_ledgerH* ledgerH;
	ledgerH = (struct _ledgerH *)m_ledger;

	FillMemory(ledgerH->clip, sizeof(ledgerH->clip), ' ');  
	CopyMemory(ledgerH->clip, lpszNewValue, min(sizeof(ledgerH->clip), strlen(lpszNewValue)));
}

BSTR CLedger::_Getrtyp() 
{
	CString strResult;
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->rtyp, sizeof(ledgerH->rtyp)); 
	return strResult.AllocSysString();
}

void CLedger::_Setrtyp(LPCTSTR lpszNewValue) 
{
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH *)m_ledger;

	FillMemory(ledgerH->rtyp, sizeof(ledgerH->rtyp), ' ');  
	CopyMemory(ledgerH->rtyp, lpszNewValue, min(sizeof(ledgerH->rtyp), strlen(lpszNewValue)));
}

BSTR CLedger::_Getrcod() 
{
	CString strResult;
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->rcod, sizeof(ledgerH->rcod));  
	return strResult.AllocSysString();
}

void CLedger::_Setrcod(LPCTSTR lpszNewValue) 
{
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->rcod, sizeof(ledgerH->rcod), ' '); 
	CopyMemory(ledgerH->rcod, lpszNewValue, min(sizeof(ledgerH->rcod), strlen(lpszNewValue)));
}

BSTR CLedger::_Getrlcd() 
{
	CString strResult;
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->rlcd, sizeof(ledgerH->rlcd));  
	return strResult.AllocSysString();
}

void CLedger::_Setrlcd(LPCTSTR lpszNewValue) 
{
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->rlcd, sizeof(ledgerH->rlcd), ' '); 
	CopyMemory(ledgerH->rlcd, lpszNewValue, min(sizeof(ledgerH->rlcd), strlen(lpszNewValue)));
}

BSTR CLedger::_Getrtit() 
{
	CString strResult;
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->rtit, sizeof(ledgerH->rtit)); 
	return strResult.AllocSysString();
}

void CLedger::_Setrtit(LPCTSTR lpszNewValue) 
{
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->rtit, sizeof(ledgerH->rtit), ' '); 
	CopyMemory(ledgerH->rtit, lpszNewValue, min(sizeof(ledgerH->rtit), strlen(lpszNewValue)));
}

BSTR CLedger::_Getrbmg() 
{
	CString strResult;
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->rbmg, sizeof(ledgerH->rbmg)); 
	return strResult.AllocSysString();
}

void CLedger::_Setrbmg(LPCTSTR lpszNewValue) 
{
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->rbmg, sizeof(ledgerH->rbmg), ' '); 
	CopyMemory(ledgerH->rbmg, lpszNewValue, min(sizeof(ledgerH->rbmg), strlen(lpszNewValue)));
}

BSTR CLedger::_Getrdmg() 
{
	CString strResult;
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->rdmg, sizeof(ledgerH->rdmg));  
	return strResult.AllocSysString();
}

void CLedger::_Setrdmg(LPCTSTR lpszNewValue) 
{
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->rdmg, sizeof(ledgerH->rdmg), ' ');  
	CopyMemory(ledgerH->rdmg, lpszNewValue, min(sizeof(ledgerH->rdmg), strlen(lpszNewValue)));
}

BSTR CLedger::_Getusid() 
{
	CString strResult;
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->usid, sizeof(ledgerH->usid));  
	return strResult.AllocSysString();
}

void CLedger::_Setusid(LPCTSTR lpszNewValue) 
{
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->usid, sizeof(ledgerH->usid), ' ');  
	CopyMemory(ledgerH->usid, lpszNewValue, min(sizeof(ledgerH->usid), strlen(lpszNewValue)));
}

BSTR CLedger::_Getcrst() 
{
	CString strResult;
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->crst, sizeof(ledgerH->crst)); 
	return strResult.AllocSysString();
}

void CLedger::_Setcrst(LPCTSTR lpszNewValue) 
{
	struct _ledgerH* ledgerH;	
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->crst, sizeof(ledgerH->crst), ' ');  
	CopyMemory(ledgerH->crst, lpszNewValue, min(sizeof(ledgerH->crst), strlen(lpszNewValue)));
}


BSTR CLedger::Getfill(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	CString strResult;
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->fill, sizeof(ledgerH->fill)); 
	return strResult.AllocSysString();
}


void CLedger::Setfill(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;
	FillMemory(ledgerH->fill, sizeof(ledgerH->fill), ' ');  
	CopyMemory(ledgerH->fill, newVal, min(sizeof(ledgerH->fill), strlen(newVal)));
}


BSTR CLedger::Gettmno(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->tmno, sizeof(ledgerH->tmno)); 
	return strResult.AllocSysString();
}


void CLedger::Settmno(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->tmno, sizeof(ledgerH->tmno), ' ');  
	CopyMemory(ledgerH->tmno, newVal, min(sizeof(ledgerH->tmno), strlen(newVal)));
	// TODO: 여기에 속성 처리기 코드를 추가합니다.
}


BSTR CLedger::Gettmps(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->tmps, sizeof(ledgerH->tmps)); 
	return strResult.AllocSysString();
}


void CLedger::Settmps(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->tmps, sizeof(ledgerH->tmps), ' ');  
	CopyMemory(ledgerH->tmps, newVal, min(sizeof(ledgerH->tmps), strlen(newVal)));
}


BSTR CLedger::Getdept(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strResult;
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->dept, sizeof(ledgerH->dept)); 
	return strResult.AllocSysString();
}


void CLedger::Setdept(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->dept, sizeof(ledgerH->dept), ' ');  
	CopyMemory(ledgerH->dept, newVal, min(sizeof(ledgerH->dept), strlen(newVal)));
}


BSTR CLedger::Getbrnc(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->brnc, sizeof(ledgerH->brnc)); 
	return strResult.AllocSysString();
}


void CLedger::Setbrnc(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->brnc, sizeof(ledgerH->brnc), ' ');  
	CopyMemory(ledgerH->brnc, newVal, min(sizeof(ledgerH->brnc), strlen(newVal)));
	// TODO: 여기에 속성 처리기 코드를 추가합니다.
}


BSTR CLedger::Getsale(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strResult;
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->sale, sizeof(ledgerH->sale)); 
	return strResult.AllocSysString();
}


void CLedger::Setsale(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->sale, sizeof(ledgerH->sale), ' ');  
	CopyMemory(ledgerH->sale, newVal, min(sizeof(ledgerH->sale), strlen(newVal)));
}


BSTR CLedger::Getagnc(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strResult;
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->agnc, sizeof(ledgerH->agnc)); 
	return strResult.AllocSysString();
}


void CLedger::Setagnc(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->agnc, sizeof(ledgerH->sale), ' ');  
	CopyMemory(ledgerH->agnc, newVal, min(sizeof(ledgerH->agnc), strlen(newVal)));
}


BSTR CLedger::Getsfiller(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strResult;
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->sfill, sizeof(ledgerH->sfill)); 
	return strResult.AllocSysString();
}


void CLedger::Setsfiller(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->sfill, sizeof(ledgerH->sfill), ' ');  
	CopyMemory(ledgerH->sfill, newVal, min(sizeof(ledgerH->sfill), strlen(newVal)));
}


BSTR CLedger::Getsusid(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strResult;
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->susid, sizeof(ledgerH->susid)); 
	return strResult.AllocSysString();
}


void CLedger::Setsusid(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->susid, sizeof(ledgerH->susid), ' ');  
	CopyMemory(ledgerH->susid, newVal, min(sizeof(ledgerH->susid), strlen(newVal)));
}


BSTR CLedger::Getmenu(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strResult;
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->menu, sizeof(ledgerH->menu)); 
	return strResult.AllocSysString();
}


void CLedger::Setmenu(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	struct _ledgerH* ledgerH;
	ledgerH = (struct _ledgerH*)m_ledger;

	FillMemory(ledgerH->menu, sizeof(ledgerH->menu), ' ');  
	CopyMemory(ledgerH->menu, newVal, min(sizeof(ledgerH->menu), strlen(newVal)));
}
