// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "cx_notify.h"
#include "../libDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IControlWnd properties

CString IControlWnd::GetFlag()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void IControlWnd::SetFlag(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString IControlWnd::GetDataList()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void IControlWnd::SetDataList(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// IControlWnd operations

void IControlWnd::Clear()
{
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void IControlWnd::SendX(LPCTSTR sAccn, LPCTSTR sPswd, LPCTSTR sPosition)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sAccn, sPswd, sPosition);
}

void IControlWnd::SendGroup(LPCTSTR userS, LPCTSTR gIDS, LPCTSTR logPassS, LPCTSTR codeS)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 userS, gIDS, logPassS, codeS);
}

/////////////////////////////////////////////////////////////////////////////
// CNotify

CNotify::CNotify(CWnd* sendW, int sendK)
{
	m_sendW = sendW;
	m_sendK = sendK;

	//m_saveK = -1;

	m_hInst = NULL;
	m_Wcontrol = NULL;
	m_Icontrol = NULL;
	m_bAll = false;
}

CNotify::~CNotify()
{
}


BEGIN_MESSAGE_MAP(CNotify, CWnd)
	//{{AFX_MSG_MAP(CNotify)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnSend)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNotify message handlers
BOOL CNotify::CreateNotify(CWnd* parent, CRect rect, CString root, bool bAll)
{
	m_parent = parent;
	m_root   = root;
	m_bAll = bAll;

	return CWnd::Create(NULL, "Notify_window", WS_CHILD|WS_VISIBLE, rect, parent, 5001);
}

int CNotify::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	AfxEnableControlContainer();

	struct	_param	param;	
	CString	path; path.Format("%s/%s/%s", m_root, DEVDIR, "cx_NotifyEx.DLL");

	param.key	= m_sendK;
	param.name	= NOTIFYNAME;
	param.fonts	= "굴림";
	param.point	= 9;
	param.style	= 0;
	param.tRGB	= 232;
	param.pRGB	= 192;
	
	param.options = "/edit00|01|02|03|04|05|06|07|08|09|10|13|14|15|";
	
	GetClientRect(&param.rect);

	m_hInst = LoadLibrary(path);
	if (!m_hInst)
	{
		TRACE("cx_Notify control load error....\n");
		return -1;
	}

	axCreate = (CWnd* (APIENTRY *)(CWnd*, void*)) GetProcAddress(m_hInst, "axCreate");
	if (axCreate == NULL)
	{
		TRACE("cx_Notify axCreate proc error...\n");
		return -1;
	}

	m_Wcontrol = axCreate(this, (void *) &param);
	m_Icontrol = new IControlWnd(m_Wcontrol->GetIDispatch(TRUE));
	
	return 0;
}

void CNotify::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_Wcontrol != NULL)
	{
		m_Wcontrol->DestroyWindow();  delete m_Wcontrol;	m_Wcontrol = NULL;
		m_Icontrol->DetachDispatch(); delete m_Icontrol;	m_Icontrol = NULL;
	}
	if (m_hInst != NULL) FreeLibrary(m_hInst);
}

void CNotify::OnClose() 
{
	if (m_Wcontrol != NULL)
	{
		m_Wcontrol->DestroyWindow(); delete m_Wcontrol; delete m_Icontrol;
	}
	if (m_hInst != NULL) FreeLibrary(m_hInst);
	
	DestroyWindow(); CWnd::OnClose();
}

LONG CNotify::OnSend(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case invokeTRx:
		{
			CString name = NOTIFYNAME;
			CString sendS = CString((char *)lParam, HIWORD(wParam) + L_userTH + name.GetLength());
			_Parser(sendS, "\t");
			return m_sendW->SendMessage(WM_USER, wParam, (LPARAM)(LPCTSTR)sendS);
		}
		break;
	case variantDLL:
	case getPALETTE:
	case getBITMAP:
	case getFONT:
	case getPEN:
	case encPASSx:
	case ledgerDLL:
		return m_sendW->SendMessage(WM_USER, wParam, lParam);
	case formDLL:
		m_parent->SendMessage(WM_LOCAL, MAKEWPARAM(EVT_NOTIFYRESULT, 0), lParam);	// 조회결과 result
		break;
	case eventDLL:
		{
			if (m_Icontrol && m_parent)
			{
				// count \t code \t mmgb \t jgvl \t mygy \n ......
				CString	dat = m_Icontrol->GetDataList();
				if (m_Icontrol->GetFlag() == "A")
					m_parent->SendMessage(WM_LOCAL, MAKEWPARAM(EVT_NOTIFYALL, dat.GetLength()), (LPARAM)(LPCTSTR)dat);	// nrec \t data
				else
					m_parent->SendMessage(WM_LOCAL, MAKEWPARAM(EVT_NOTIFYEACH, dat.GetLength()), (LPARAM)(LPCTSTR)dat);	// data
			}
		}
		break;
	default:
	//	TRACE("OnSend = [0x%x][%d]\n", LOWORD(wParam), HIWORD(wParam));
		break;
	}

	return 0;
}

void CNotify::SendJango(CString acc, CString pswd, CString sPosition /*="0"*/)
{
	m_bAll = false;
	if (m_Icontrol == NULL)
		return;
	
	if (pswd.IsEmpty())
		return;
	m_Icontrol->SendX(acc, pswd, sPosition);
}

void CNotify::SendJango2(CString userS, CString gIDS, CString logPassS, CString codeS)
{
	m_bAll = true;
	if (m_Icontrol == NULL)
		return;
	if (logPassS.IsEmpty())
		return;
	m_Icontrol->SendGroup(userS, gIDS, logPassS, codeS);
}

void CNotify::Clear()
{
	if (m_Icontrol)	m_Icontrol->Clear();
}


void CNotify::SubSendMessage(WPARAM wParam, LPARAM lParam)
{
	if (m_Wcontrol)
	{
		m_Wcontrol->SendMessage(WM_USER, wParam, lParam);
	}
}

