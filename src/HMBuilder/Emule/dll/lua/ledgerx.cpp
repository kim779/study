// ledgerx.cpp : implementation file
//

#include "stdafx.h"
#include "ledgerx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLedger

IMPLEMENT_DYNCREATE(CLedger, CCmdTarget)

CLedger::CLedger()
{
	EnableAutomation();

	m_size = L_ledgerH;
	m_ledger = new char[L_ledgerH];
	FillMemory(m_ledger, L_ledgerH, ' ');
	parseParam();
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
	//}}AFX_DISPATCH_MAP
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

int CLedger::GetLedger(char* data)
{
	CopyMemory(data, m_ledger, L_ledgerH);
	return L_ledgerH;
}

void CLedger::SetLedger(char* data)
{
	CopyMemory(m_ledger, data, L_ledgerH);
}

LPCTSTR CLedger::GetTime()
{
	CString strResult;
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->time, sizeof(ledgerH->time));

	return strResult.operator LPCTSTR();
}

void CLedger::SetTime(LPCTSTR lpszNewValue)
{
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	FillMemory(ledgerH->time, sizeof(ledgerH->time), ' ');
	CopyMemory(ledgerH->time, lpszNewValue, min(sizeof(ledgerH->time), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetFlag()
{
	CString	strResult;
	struct	_ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->flag, sizeof(ledgerH->flag));
	return strResult.operator LPCTSTR();
}

void CLedger::SetFlag(LPCTSTR lpszNewValue)
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	FillMemory(ledgerH->flag, sizeof(ledgerH->flag), ' ');
	CopyMemory(ledgerH->flag, lpszNewValue, min(sizeof(ledgerH->flag), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetSvcn()
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->svcn, sizeof(ledgerH->svcn));
	return strResult.operator LPCTSTR();
}

void CLedger::SetSvcn(LPCTSTR lpszNewValue)
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	FillMemory(ledgerH->svcn, sizeof(ledgerH->svcn), ' ');
	CopyMemory(ledgerH->svcn, lpszNewValue, min(sizeof(ledgerH->svcn), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetSvid()
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->svid, sizeof(ledgerH->svid));
	return strResult.operator LPCTSTR();
}

void CLedger::SetSvid(LPCTSTR lpszNewValue)
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	FillMemory(ledgerH->svid, sizeof(ledgerH->svid), ' ');
	CopyMemory(ledgerH->svid, lpszNewValue, min(sizeof(ledgerH->svid), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetPgid()
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->pgid, sizeof(ledgerH->pgid));
	return strResult.operator LPCTSTR();
}

void CLedger::SetPgid(LPCTSTR lpszNewValue)
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	FillMemory(ledgerH->pgid, sizeof(ledgerH->pgid), ' ');
	CopyMemory(ledgerH->pgid, lpszNewValue, min(sizeof(ledgerH->pgid), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetUsid()
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->usid, sizeof(ledgerH->usid));
	return strResult.operator LPCTSTR();
}

LPCTSTR CLedger::GetDept()
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->dept, sizeof(ledgerH->dept));
	return strResult.operator LPCTSTR();
}

LPCTSTR CLedger::GetTerm()
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->term, sizeof(ledgerH->term));
	return strResult.operator LPCTSTR();
}

LPCTSTR CLedger::GetUsnm()
{
		CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->usnm, sizeof(ledgerH->usnm));
	return strResult.operator LPCTSTR();
}

LPCTSTR CLedger::GetTrcd()
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->trcd, sizeof(ledgerH->trcd));
	return strResult.operator LPCTSTR();
}

void CLedger::SetTrcd(LPCTSTR lpszNewValue)
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	FillMemory(ledgerH->trcd, sizeof(ledgerH->trcd), ' ');
	CopyMemory(ledgerH->trcd, lpszNewValue, min(sizeof(ledgerH->trcd), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetScrn()
{
	CString strResult;
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->scrn, sizeof(ledgerH->scrn));
	return strResult.operator LPCTSTR();
}

void CLedger::SetScrn(LPCTSTR lpszNewValue)
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	FillMemory(ledgerH->scrn, sizeof(ledgerH->scrn), ' ');
	CopyMemory(ledgerH->scrn, lpszNewValue, min(sizeof(ledgerH->scrn), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetKEnd()
{
	CString	strResult;
	struct	_ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->kend, sizeof(ledgerH->kend));
	return strResult.operator LPCTSTR();
}

void CLedger::SetKEnd(LPCTSTR lpszNewValue)
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	FillMemory(ledgerH->kend, sizeof(ledgerH->kend), ' ');
	CopyMemory(ledgerH->kend, lpszNewValue, min(sizeof(ledgerH->kend), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetFxcp()
{
	CString	strResult;
	struct	_ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH *)m_ledger;
	strResult = CString(ledgerH->fxcp, sizeof(ledgerH->fxcp));
	return strResult.operator LPCTSTR();
}

void CLedger::SetFxcp(LPCTSTR lpszNewValue)
{
	struct	_ledgerH* ledgerH;

	ledgerH = (struct _ledgerH *)m_ledger;
	FillMemory(ledgerH->fxcp, sizeof(ledgerH->fxcp), ' ');
	CopyMemory(ledgerH->fxcp, lpszNewValue, min(sizeof(ledgerH->fxcp), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetIkey()
{
	CString strResult;
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->ikey, sizeof(ledgerH->ikey));
	return strResult.operator LPCTSTR();
}

void CLedger::SetIkey(LPCTSTR lpszNewValue)
{
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	FillMemory(ledgerH->ikey, sizeof(ledgerH->ikey), ' ');
	CopyMemory(ledgerH->ikey, lpszNewValue, min(sizeof(ledgerH->ikey), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetEofd()
{
	CString strResult;
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->eofd, sizeof(ledgerH->eofd));
	return strResult.operator LPCTSTR();
}

void CLedger::SetEofd(LPCTSTR lpszNewValue)
{
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	FillMemory(ledgerH->eofd, sizeof(ledgerH->eofd), ' ');
	CopyMemory(ledgerH->eofd, lpszNewValue, min(sizeof(ledgerH->eofd), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetReqn()
{
	CString strResult;
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->reqn, sizeof(ledgerH->reqn));
	return strResult.operator LPCTSTR();
}

void CLedger::SetReqn(LPCTSTR lpszNewValue)
{
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	FillMemory(ledgerH->reqn, sizeof(ledgerH->reqn), ' ');
	CopyMemory(ledgerH->reqn, lpszNewValue, min(sizeof(ledgerH->reqn), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetECod()
{
	CString strResult;
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->ecod, sizeof(ledgerH->ecod));
	return strResult.operator LPCTSTR();
}

void CLedger::SetECod(LPCTSTR lpszNewValue)
{
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	FillMemory(ledgerH->ecod, sizeof(ledgerH->ecod), ' ');
	CopyMemory(ledgerH->ecod, lpszNewValue, min(sizeof(ledgerH->ecod), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetEMsg()
{
	CString strResult;
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->emsg, sizeof(ledgerH->emsg));
	return strResult.operator LPCTSTR();
}

void CLedger::SetEMsg(LPCTSTR lpszNewValue)
{
	struct _ledgerH* ledgerH;

	ledgerH = (struct _ledgerH*)m_ledger;
	FillMemory(ledgerH->emsg, sizeof(ledgerH->emsg), ' ');
	CopyMemory(ledgerH->emsg, lpszNewValue, min(sizeof(ledgerH->emsg), strlen(lpszNewValue)));
}

LPCTSTR CLedger::GetAccn()
{
	CString strResult;
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH*)m_ledger;
	strResult = CString(ledgerH->accn, sizeof(ledgerH->accn));
	return strResult.operator LPCTSTR();
}

void CLedger::SetAccn(LPCTSTR lpszNewValue)
{
	struct _ledgerH* ledgerH;
	
	ledgerH = (struct _ledgerH*)m_ledger;
	FillMemory(ledgerH->accn, sizeof(ledgerH->accn), ' ');
	CopyMemory(ledgerH->accn, lpszNewValue, min(sizeof(ledgerH->accn), strlen(lpszNewValue)));
}

void CLedger::parseParam()
{

}