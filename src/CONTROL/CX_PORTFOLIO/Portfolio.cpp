// Portfolio.cpp : implementation file
//

#include "stdafx.h"
#include "CX_PORTFOLIO.h"
#include "Portfolio.h"
#include "dataio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TR_IGROUP	1	// 관심종목 그룹
#define	TR_IDATA	2	// 관심종목 그룹의 종목

/////////////////////////////////////////////////////////////////////////////
// CPortfolio

CPortfolio::CPortfolio(CWnd* parent, struct _param* param)
{
	EnableAutomation();

	m_parent = parent;
	m_name = CString(param->name);
	m_key = param->key;

	m_datb = nullptr;
	m_datl = 0;
}

CPortfolio::~CPortfolio()
{
	if (m_datb)
		m_datb.reset();
}

void CPortfolio::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CPortfolio, CWnd)
	//{{AFX_MSG_MAP(CPortfolio)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CPortfolio, CWnd)
	//{{AFX_DISPATCH_MAP(CPortfolio)
	DISP_FUNCTION(CPortfolio, "SetUserName", _SetUserName, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CPortfolio, "LoadGroupNames", _LoadGroupNames, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CPortfolio, "LoadCodeList", _LoadCodeList, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CPortfolio, "GetGroupCnt", _GetGroupCnt, VT_I4, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IPortfolio to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {A2BB4FBC-4EBF-49A4-98E2-92B86C56B91F}
static const IID IID_IPortfolio =
{ 0xa2bb4fbc, 0x4ebf, 0x49a4, { 0x98, 0xe2, 0x92, 0xb8, 0x6c, 0x56, 0xb9, 0x1f } };

BEGIN_INTERFACE_MAP(CPortfolio, CWnd)
	INTERFACE_PART(CPortfolio, IID_IPortfolio, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortfolio message handlers

long CPortfolio::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		if (m_datb)
			m_datb.reset();

		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		m_trkey = exth->key;
		m_datl = exth->size;
		m_datb = std::make_unique<char[]>(m_datl);
		CopyMemory(m_datb.get(), exth->data, m_datl);
		m_done = TRUE;
		break;

	default:break;
	}
	return 0;
}

void CPortfolio::_SetUserName(LPCTSTR sUser)
{
	m_user = sUser;
}

BSTR CPortfolio::_LoadGroupNames()
{
	CString strResult;
	struct	_updn	updn {};

	FillMemory(&updn, sz_updn, ' ');
	CopyMemory(updn.uinfo.gubn, "MY", sizeof(updn.uinfo.gubn));
	updn.uinfo.dirt[0] = 'U';
	updn.uinfo.cont[0] = 'g';
	CopyMemory(updn.uinfo.nblc, "00001", sizeof(updn.uinfo.nblc));
	updn.uinfo.retc[0] = 'U';

	if (!sendTR("PIDOMYST", (char*)&updn, sz_updn, TR_IGROUP))
		return strResult.AllocSysString();

	if (m_datb != nullptr && m_datl > 0)
	{
		struct	_gfoldex* group = (struct _gfoldex*)m_datb.get();
		const	int	cnt = atoi(CString(group->nrec, sizeof(group->nrec)));
		if (cnt > 0)
		{
			CString	tmps, tmpx;
			for (int ii = 0, ival = 0; ii < cnt; ii++)
			{
				ival = atoi(CString(group->glist[ii].ogrs, sizeof(group->glist[ii].ogrs)));
				tmps = CString(group->glist[ii].gnam, sizeof(group->glist[ii].gnam));
				tmps.Trim();
				if (tmps.IsEmpty())
					continue;

				tmpx.Format("%02d %s\t", ival, tmps);
				strResult = tmpx + strResult;
			}
		}
	}

	return strResult.AllocSysString();
}

BSTR CPortfolio::_LoadCodeList(LPCTSTR sGroup)
{
	CString strResult;
	struct	_updn	updn {};
	CString tmps;

	FillMemory(&updn, sz_updn, ' ');
	CopyMemory(updn.uinfo.gubn, "MY", sizeof(updn.uinfo.gubn));
	updn.uinfo.dirt[0] = 'U';
	updn.uinfo.cont[0] = 'j';
	CopyMemory(updn.uinfo.nblc, "00001", sizeof(updn.uinfo.nblc));
	updn.uinfo.retc[0] = 'U';

	tmps.Format("%02d", atoi(sGroup));
	CopyMemory(updn.ginfo.gnox, tmps, sizeof(updn.ginfo.gnox));

	if (!sendTR("PIDOMYST", (char*)&updn, sz_updn, TR_IDATA))
		return strResult.AllocSysString();

	if (m_datb != nullptr && m_datl >= sz_ginfo)
	{
		struct	_ginfo* group = (struct _ginfo*)m_datb.get();
		const	int	cnt = atoi(CString(group->jrec, sizeof(group->jrec)));
		if (cnt > 0)
		{
			CString	tmps;
			struct	_jinfo* jinfo = (struct _jinfo*)&m_datb[sz_ginfo];

			for (int ii = 0; ii < cnt; ii++, jinfo++)
			{
				tmps = CString(jinfo->code, sizeof(jinfo->code));
				tmps.Trim();
				strResult += tmps;
				strResult += '\t';
			}
		}
	}

	return strResult.AllocSysString();
}

long CPortfolio::_GetGroupCnt()
{
	CString strResult;
	int	count = 0;
	struct	_updn	updn {};

	FillMemory(&updn, sz_updn, ' ');
	CopyMemory(updn.uinfo.gubn, "MY", sizeof(updn.uinfo.gubn));
	updn.uinfo.dirt[0] = 'U';
	updn.uinfo.cont[0] = 'g';
	CopyMemory(updn.uinfo.nblc, "00001", sizeof(updn.uinfo.nblc));
	updn.uinfo.retc[0] = 'U';

	sendTR("PIDOMYST", (char*)&updn, sz_updn, TR_IGROUP);
	if (m_datb != nullptr && m_datl > 0)
	{
		struct	_gfoldex* group = (struct _gfoldex*)m_datb.get();
		const	int	cnt = atoi(CString(group->nrec, sizeof(group->nrec)));
		if (cnt <= 0)
			return count;

		CString	tmps, tmpx;
		for (int ii = 0, ival = 0; ii < cnt; ii++)
		{
			ival = atoi(CString(group->glist[ii].ogrs, sizeof(group->glist[ii].ogrs)));
			tmps = CString(group->glist[ii].gnam, sizeof(group->glist[ii].gnam));
			tmps.Trim();
			if (tmps.IsEmpty())
				continue;

			count++;
		}
	}

	return count;
}

bool CPortfolio::sendTR(CString trc, char* datb, int datl, int key)
{
	MSG	msg{};
	struct _userTH	udat {};
	int	index = 0;
	std::unique_ptr<TCHAR[]> buff = std::make_unique<TCHAR[]>(L_userTH + datl + 128);

	// key & control name
	buff[index++] = key;
	CopyMemory(&buff[index], m_name, m_name.GetLength());
	index += m_name.GetLength();
	buff[index++] = '\t';

	// userTH
	CopyMemory(udat.trc, trc, trc.GetLength());
	udat.stat = US_KEY;
	udat.key  = m_key;
	CopyMemory(&buff[index], &udat, L_userTH);
	index += L_userTH;

	// data
	CopyMemory(&buff[index], datb, datl);

	m_done = FALSE;
	m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)buff.get());
	buff.reset();

	ULONGLONG elapse = GetTickCount64();
	for ( ; !m_done; )
	{
		if (GetTickCount64() - elapse > 3000)
		{
			m_done = TRUE;
			return false;
		}

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return true;
}