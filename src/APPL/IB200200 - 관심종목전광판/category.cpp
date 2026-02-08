// category.cpp : implementation file
//

#include "stdafx.h"
#include "IB200200.h"
#include "category.h"

#include "../../h/axisvar.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// IControlWnd properties

/////////////////////////////////////////////////////////////////////////////
// IControlWnd properties

CString IControlWnd::GetJCodes()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void IControlWnd::SetJCodes(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// IControlWnd operations

CString IControlWnd::GetCurCode()
{
	CString result;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString IControlWnd::GetPreCode(LPCTSTR code)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		code);
	return result;
}

CString IControlWnd::GetNextCode(LPCTSTR code)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		code);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// CCategory

CCategory::CCategory(CWnd* sendW, int sendK)
{
	m_sendW = sendW;
	m_sendK = sendK;

	m_saveK = -1;

	m_hInst = NULL;
	m_Wcontrol = NULL;
	m_Icontrol = NULL;
}

CCategory::~CCategory()
{
}


BEGIN_MESSAGE_MAP(CCategory, CWnd)
	//{{AFX_MSG_MAP(CCategory)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER,    OnSend)
	ON_MESSAGE(WM_USER+99, OnRecv)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCategory message handlers
BOOL CCategory::CreateCategory(CWnd* parent, CRect rect, CString root)
{
	m_parent = parent;
	m_root   = root;

	return CWnd::Create(NULL, "category_window", WS_CHILD|WS_VISIBLE, rect, parent, 5001);
}

int CCategory::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	AfxEnableControlContainer();

	struct	_param	param;
	CString	path; path.Format("%s/%s/%s", m_root, DEVDIR, "cx_category.DLL");

	param.name	= "cx_category";
	param.fonts	= "±¼¸²";
	param.point	= 9;
	param.style	= 0;
	param.tRGB	= 232;
	param.pRGB	= 192;
	param.options = "/k1/icategory.bmp/x0/c100";
	GetClientRect(&param.rect);

	m_hInst = LoadLibrary(path);
	if (!m_hInst)
	{
		TRACE("cx_category control load error....\n");
		return -1;
	}

	axCreate = (CWnd* (APIENTRY *)(CWnd*, void*)) GetProcAddress(m_hInst, "axCreateEx");
	if (axCreate == NULL)
	{
		TRACE("cx_category axCreate proc error...\n");
		return -1;
	}

	m_Wcontrol = axCreate(this, (void *) &param);
	m_Icontrol = std::make_unique<IControlWnd>(m_Wcontrol->GetIDispatch(TRUE));
	
	return 0;
}

void CCategory::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_Wcontrol != NULL)
	{
		m_Wcontrol->DestroyWindow();  delete m_Wcontrol;
		m_Icontrol->DetachDispatch(); m_Icontrol = nullptr;
	}
	if (m_hInst != NULL) FreeLibrary(m_hInst);
}

void CCategory::OnClose() 
{
	if (m_Wcontrol != NULL)
	{
		m_Wcontrol->DestroyWindow(); 
		delete m_Wcontrol; 
		m_Icontrol = nullptr;
	}
	if (m_hInst != NULL) FreeLibrary(m_hInst);
	
	DestroyWindow(); CWnd::OnClose();
}

LONG CCategory::OnSend(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case invokeTRx:
		{
			int	pos = -1;
			const int nBytes = HIWORD(wParam);
			char*	  pBytes = (char *)lParam;
			CString	 string;


			string  = CString((char *)lParam, L_userTH + HIWORD(wParam));
			pos = string.Find("\t");
			if (pos == -1)
			{
				TRACE("invokeTRx...send error...\n");
				return 0;
			}
			pos = string.Mid(1).Find('\t'); pos++;

			string = string.Left(pos++);
			pBytes += pos;

			struct	_userTH* uTH = (struct _userTH *)pBytes;

			m_saveK = -1;
			if (uTH->stat & US_KEY)
				m_saveK = string.GetAt(0);
			uTH->key = m_sendK;

			m_sendW->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nBytes), (LPARAM)pBytes);
		}
		break;
	case variantDLL:
	case getPALETTE:
	case getBITMAP:
	case getFONT:
	case getPEN:
		return m_sendW->SendMessage(WM_USER, wParam, lParam);

	case formDLL:
		m_parent->SendMessage(WM_TOPARENT, MAKEWPARAM(cpCATEGROY, m_sendK), (LPARAM)0);
		break;
	case eventDLL:
		{
			CString	string = category_GetJCodes();
			m_parent->SendMessage(WM_TOPARENT, MAKEWPARAM(cpCATEGROY, m_sendK), (LPARAM)(LPCTSTR)string);
		}
		break;

	default:
		TRACE("OnSend = [0x%x][%d]\n", LOWORD(wParam), HIWORD(wParam));
		break;
	}

	return 0;
}

LONG CCategory::OnRecv(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		{
			WPARAM wp = wParam;
			if (m_saveK != -1)
				wp = MAKEWPARAM(MAKEWORD(LOBYTE(LOWORD(wParam)), m_saveK), HIWORD(wParam));

			m_Wcontrol->SendMessage(WM_USER, wp, lParam);
		}
		break;
//	case DLL_ALERT:
	case DLL_ALERTx:
	case DLL_SETPAL:
	case DLL_TRIGGER:
	case DLL_DOMINO:
		m_Wcontrol->SendMessage(WM_USER, wParam, lParam);
		break;
	}

	return 0;
}

CString CCategory::category_GetJCodes()
{
	CString	codes = _T("");
	if (m_Icontrol == NULL)
		return codes;

	return m_Icontrol->GetJCodes();


/*
	USES_CONVERSION;

	if (m_Wcontrol == NULL)
		return _T("");

	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);

	if (pDisp == NULL)
		return _T("");

	VARIANT var;
	VariantInit(&var);
	var.vt = VT_BSTR;

	HRESULT hr = CComDispatchDriver::GetProperty(pDisp, 0x4, &var);	

	return OLE2T(var.bstrVal);
*/
}

void CCategory::category_SetJCodes(CString codes)
{
	if (m_Icontrol == NULL)
		return;

	m_Icontrol->SetJCodes(codes);
}
