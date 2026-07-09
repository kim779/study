// XecureCtl.cpp : Implementation of the CXecureCtrl ActiveX Control class.

#include "stdafx.h"
#include "Xecure.h"
#include "XecureCtl.h"
#include "XecurePpg.h"

/*
#include "Xecure/issacweb_common.h"
#include "Xecure/issacweb_client.h"
#pragma	comment(lib, "Xecure/IssacWebCSClient")
#pragma	message("Automatically linking with Xecure library")
*/


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	L_key	16

IMPLEMENT_DYNCREATE(CXecureCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CXecureCtrl, COleControl)
	//{{AFX_MSG_MAP(CXecureCtrl)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CXecureCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CXecureCtrl)
	DISP_FUNCTION(CXecureCtrl, "Encrypt", Encrypt, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CXecureCtrl, "Decrypt", Decrypt, VT_BOOL, VTS_I4 VTS_I4)
	DISP_FUNCTION(CXecureCtrl, "Xecure", Xecure, VT_I4, VTS_I4 VTS_I4)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CXecureCtrl, COleControl)
	//{{AFX_EVENT_MAP(CXecureCtrl)
	// NOTE - ClassWizard will add and remove event map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CXecureCtrl, 1)
	PROPPAGEID(CXecurePropPage::guid)
END_PROPPAGEIDS(CXecureCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CXecureCtrl, "AxisXecure.XecureCtrl.AX2010",
	0x66ac43f3, 0x2ced, 0x46f1, 0xb6, 0x47, 0x8d, 0x5, 0xe7, 0xb6, 0x6b, 0x2f)
/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CXecureCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DXecure =
		{ 0x66ac43f4, 0x2ced, 0x46f1, { 0xb6, 0x47, 0x8d, 0x5, 0xe7, 0xb6, 0x6b, 0x2f } };

const IID BASED_CODE IID_DXecureEvents =
		{ 0x66ac43f5, 0x2ced, 0x46f1, { 0xb6, 0x47, 0x8d, 0x5, 0xe7, 0xb6, 0x6b, 0x2f } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwXecureOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CXecureCtrl, IDS_XECURE, _dwXecureOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CXecureCtrl::CXecureCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CXecureCtrl

BOOL CXecureCtrl::CXecureCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_XECURE,
			IDB_XECURE,
			afxRegApartmentThreading,
			_dwXecureOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CXecureCtrl::CXecureCtrl - Constructor

CXecureCtrl::CXecureCtrl()
{
	InitializeIIDs(&IID_DXecure, &IID_DXecureEvents);

	m_enc  = encHELLO;
	ZeroMemory(&m_key, sizeof(LEA_KEY));
	srand(time(NULL));
}


/////////////////////////////////////////////////////////////////////////////
// CXecureCtrl::~CXecureCtrl - Destructor

CXecureCtrl::~CXecureCtrl()
{
}


/////////////////////////////////////////////////////////////////////////////
// CXecureCtrl::OnDraw - Drawing function

void CXecureCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{

}


/////////////////////////////////////////////////////////////////////////////
// CXecureCtrl::DoPropExchange - Persistence support

void CXecureCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
}


/////////////////////////////////////////////////////////////////////////////
// CXecureCtrl::OnResetState - Reset control to default state

void CXecureCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}


/////////////////////////////////////////////////////////////////////////////
// CXecureCtrl message handlers

BOOL CXecureCtrl::Encrypt(long pBytes, long nBytes) 
{
	if (m_enc != encRUN)
		return FALSE;

	m_nBytes = *(int *)nBytes;
	long pdata = pBytes;
	unsigned char* pEnc = nullptr;

	if ((m_nBytes % BLOCK_SIZE) != 0)
	{
		int paddinglength = BLOCK_SIZE - (m_nBytes % BLOCK_SIZE);
		int total = m_nBytes + paddinglength;
		pEnc = new unsigned char[total];		
		ZeroMemory(pEnc, total);
		CopyMemory(pEnc, (unsigned char*)pBytes, m_nBytes);		
		pEnc[total - 1] = paddinglength;
		pdata = (LONG)pEnc;
		m_nBytes = total;
	}

	lea_ecb_enc((unsigned char*)m_pBytes, (unsigned char*)pdata, m_nBytes, &m_key);

	if (pEnc)
		delete[] pEnc;

	if (m_nBytes < 0)
	{
		m_nBytes = 0;
		return FALSE;
	}

	*(int *)nBytes = m_nBytes;
	CopyMemory((void *)pBytes, m_pBytes, m_nBytes);
	return TRUE;
}

BOOL CXecureCtrl::Decrypt(long pBytes, long nBytes)
{
	if (m_enc != encRUN)
		return FALSE;

	m_nBytes = *(int *)nBytes;
	lea_ecb_dec((unsigned char*)m_pBytes, (unsigned char*)pBytes, m_nBytes, &m_key);

	unsigned char clen = m_pBytes[m_nBytes - 1];
	if ((clen > 0) && (clen < 16))
		m_nBytes -= clen;

	if (m_nBytes < 0)
	{
		m_nBytes = 0;
		return FALSE;
	}

	*(int *)nBytes = m_nBytes;
	CopyMemory((void *)pBytes, m_pBytes, m_nBytes);
	return TRUE;
}

long CXecureCtrl::Xecure(long pBytes, long nBytes) 
{
	m_nBytes = *(int *)nBytes;
	*(int *)nBytes = -1;

	switch (m_enc)
	{
	case encHELLO:
		// Randomkey Create
		ZeroMemory(m_pBytes, sizeof(m_pBytes));
		m_nBytes = 0;

		// make Client Key
		if (makeHalfkey((unsigned char*)m_pBytes))
			m_nBytes = BLOCK_SIZE / 2;
				
		if (m_nBytes < 0)
			return 0;
	
		m_pBytes[m_nBytes] = '\0';
		m_enc = encOK;
		*(int *)nBytes = m_nBytes;
		return (long)m_pBytes;
	case encOK:
		unsigned char skey[BLOCK_SIZE / 2];
		unsigned char mkey[BLOCK_SIZE];
		ZeroMemory(skey, BLOCK_SIZE / 2);
		ZeroMemory(mkey, BLOCK_SIZE);
		// ReceiveKey from Server key [8bytes]
		CopyMemory(skey, (char*)pBytes, BLOCK_SIZE/2);
		
		// Create Masterkey 16bytes
		for (int ii = 0; ii < BLOCK_SIZE / 2; ii++)
		{
			mkey[ii * 2]     = m_pBytes[ii];
			mkey[ii * 2 + 1] = skey[ii];
		}
		lea_set_key(&m_key, mkey, BLOCK_SIZE);
		ZeroMemory(m_pBytes, sizeof(m_pBytes));
		ZeroMemory(skey, BLOCK_SIZE / 2);
		ZeroMemory(mkey, BLOCK_SIZE);
		m_enc = encRUN;
		*(int *)nBytes = 0;
		return (long)0;
	case encRUN:
		break;
	default:
		break;
	}

	*(int *)nBytes = m_nBytes;
	return (long)m_pBytes;
}

bool CXecureCtrl::makeHalfkey(unsigned char* pAppkey)
{
	ASSERT(pAppkey);
	if (pAppkey == nullptr)
		return false;

	for (int ii = 0; ii < (BLOCK_SIZE / 2); ii++)
		*(pAppkey++) = (rand() % 94) + 0x21;
	return true;
}