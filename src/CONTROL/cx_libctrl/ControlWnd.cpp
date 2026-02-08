// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_libctrl.h"
#include "ControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_pParent = nullptr;
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_FUNCTION(CControlWnd, "IsIPInRange", IsIPInRange, VT_I4, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {B8C8D88E-AF56-4222-AB73-564082462F4C}
static const IID IID_IControlWnd =
{ 0xb8c8d88e, 0xaf56, 0x4222, { 0xab, 0x73, 0x56, 0x40, 0x82, 0x46, 0x2f, 0x4c } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers
#define TRKEY_GRIDNEW		0x21	// 그리드 새로조회
#define TRKEY_MARKETTIME    0x01


int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	return 0;
}

void CControlWnd::sendTR(CString name, CString data, BYTE type, int key, CString ctrlName, int uniqueKey)
{
	char	wb[1024]{};
	struct	_userTH udat {};
	int	idx = 0;

	// key & control name
	if (name == "bo2211991")
	{
		udat.stat |= US_KEY;
		wb[idx++] = TRKEY_GRIDNEW;
	}
	CopyMemory(&wb[idx], ctrlName, ctrlName.GetLength());
	idx += ctrlName.GetLength();
	wb[idx++] = '\t';

	// userTH
	strcpy(udat.trc, name);
	udat.key = key;  //test
	udat.stat = type;
	CopyMemory(&wb[idx], &udat, L_userTH);
	idx += L_userTH;

	// data
	CopyMemory(&wb[idx], data, data.GetLength());
	
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, data.GetLength()), (LPARAM)wb);
}


void CControlWnd::SetParam(_param *pParam)
{
	m_param.key = pParam->key;
	m_param.name = CString(pParam->name, pParam->name.GetLength());
	m_param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point = pParam->point;
	m_param.style = pParam->style;
	m_param.options = CString(pParam->options, pParam->options.GetLength());
	m_param.options.Delete(0,2);
}

LONG CControlWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		{
			struct _extTHx* extTH = (struct _extTHx*)lParam;
		}
		break;
	}

	return 0;
}

CString CControlWnd::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;
	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
}

CString CControlWnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}

	CString  temp = srcstr.Left(srcstr.Find(substr));
	srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	return temp;
}
 

unsigned int CControlWnd::IPToUInt(CString ip) 
{
	int	a = 0, b = 0, c = 0, d = 0;
	unsigned int addr = 0;

	if (sscanf(ip, "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
		return 0;

	addr  = a << 24;	//11000
	addr |= b << 16;	//10000
	addr |= c << 8;		//1000
	addr |= d;

	return addr;
}

long CControlWnd::IsIPInRange(LPCTSTR sIP, LPCTSTR sNetWork)
{
	const	unsigned int ip_addr = IPToUInt(sIP);
	const	unsigned int network_addr = IPToUInt(sNetWork);
	const	unsigned int mask_addr = IPToUInt("255.255.0.0");

	const	unsigned int net_lower = (network_addr & mask_addr);
	const	unsigned int net_upper = (net_lower | (~mask_addr));

	if (ip_addr >= net_lower && ip_addr <= net_upper)
		return true;

	return false;
}

void CControlWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//sendTR("bo221199", "1", US_OOP, m_param.key, m_param.name, TRKEY_MARKETTIME);

	sendTR("bo221199", "1", US_ENC, m_param.key, m_param.name, TRKEY_MARKETTIME);
	CWnd::OnLButtonDown(nFlags, point);
}
