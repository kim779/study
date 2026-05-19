
#include "stdafx.h"
#include "awObjectLoad.h"
#include "../h/mapform.h"
#include "../ChildView.h"

#include "../h/fmProperties.h"
#include "../h/iForm.h"
#include "../h/fmObject.h"
#include "../h/mainvar.h"

#define WM_OBJMSG	WM_USER + 100
LRESULT CALLBACK ControlProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

CfmObject*	(APIENTRY *createObject)(CfmProperties*, CiForm*, CWnd*);

CObjectLoad::CObjectLoad(CformProp* fmProp, CiForm* iForm, CChildView* view)
{
	m_view = view;
	m_fmProp = fmProp;
	m_piForm = iForm;
	m_hInstDll = NULL;
	m_fmObj = NULL;
	m_strLibName = _T("");
	m_wndObj = NULL;
	m_callproc = NULL;
	m_bHook = true;
}

CObjectLoad::~CObjectLoad()
{
	if (m_wndObj->GetSafeHwnd())
	{
		if (m_callproc)
			SetWindowLong(m_wndObj->GetSafeHwnd(), GWL_WNDPROC, (LONG)m_callproc);
		//if (IsWindow(m_BtnObj->GetSafeHwnd()))
		//{
		//	m_BtnObj->SendMessage(WM_CLOSE);
		//	m_BtnObj->Detach();
		//}
		m_wndObj->SendMessage(WM_CLOSE);
	}

	if (m_fmObj) {
		delete m_fmObj;
	}
	if (m_hInstDll) FreeLibrary(m_hInstDll);
	if (m_fmProp) delete m_fmProp;
}

CString CObjectLoad::GetLibName(char kind)
{
	CString strLibName("");
	switch(kind)
	{
	case fmOBJECT:
		strLibName.Format("%s\\ctmobj\\%s",
			(CString)(char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0),
			m_fmProp->strings);
		break;
	case fmLABEL:
		strLibName = _T("axLabel.dll");
		break;
	case fmBOX:
		strLibName = _T("axBox.dll");
		break;
	case fmGROUP:
		strLibName = _T("axGroup.dll");
		break;
	case fmIMVIEW:
		strLibName = _T("axIMView.dll");
		break;
	case fmBUTTON:
		strLibName = _T("axButton.dll");
		break;
	case fmRADIO:
		strLibName = _T("axRadio.dll");
		break;
	case fmCHECK:
		strLibName = _T("axCheck.dll");
		break;
	case fmEDIT:
		strLibName = _T("axEdit.dll");
		break;
	case fmCOMBO:
		strLibName = _T("axCombo.dll");
		break;
	case fmOUTPUT:
		strLibName = _T("axOutput.dll");
		break;
	case fmMEMO:
		strLibName = _T("axMemo.dll");
		break;
	case fmGRID:
		strLibName = _T("axGrid.dll");
		break;
	case fmGRIDEX:
		strLibName = _T("axGridEx.dll");
		break;
	case fmTABLE:
		strLibName = _T("axTable.dll");
		break;
	case fmTREE:
		strLibName = _T("axTree.dll");
		break;
	case fmFORM:
		strLibName = _T("axForm.dll");
		break;
	case fmTAB:
		strLibName = _T("axTab.dll");
		break;
	case fmBROWSER:
		strLibName = _T("axBrowser.dll");
		break;
		
	}
	return strLibName;
}

bool CObjectLoad::LoadLib(char	kind)
{
	if (m_hInstDll) return false;

	m_strLibName = GetLibName(kind);
	m_hInstDll = LoadLibrary(m_strLibName);
	if (!m_hInstDll)
	{
		CString strErr("");
		strErr.Format(_T("%s load fail!!\n"), m_strLibName);
		MessageBox(NULL, strErr, "AXIS", MB_OK | MB_TOPMOST);
		return false;
	}

	createObject = (CfmObject* (APIENTRY *)(CfmProperties*, CiForm*, CWnd*)) GetProcAddress(m_hInstDll, "createObject");
	if (createObject == NULL)
	{
		CString strErr("");
		strErr.Format(_T("%s createObject load fail!!\n"), m_strLibName);
		MessageBox(NULL, strErr, "AXIS", MB_OK | MB_TOPMOST);
		return false;
	}
	return true;
}

void CObjectLoad::SetLastPoint(CPoint point)
{
	m_fmProp->rect.bottom = point.y;
	m_fmProp->rect.right = point.x;
	m_fmProp->rect.NormalizeRect();
}

CRect CObjectLoad::GetRect()
{
	return m_fmProp->rect;
}

void CObjectLoad::LoadProc()
{
	m_fmObj = createObject((CfmProperties*)m_fmProp, m_piForm, m_view);
	//////////////////////////////LoadProcEnd();
	if(m_fmObj) m_wndObj = m_fmObj->GetWnd();
	if (m_wndObj)
	{
		SetWindowLong(m_wndObj->GetSafeHwnd(), GWL_USERDATA, long(this));
		HANDLE instance = (HANDLE)GetWindowLong(m_wndObj->GetSafeHwnd(), GWL_HINSTANCE);
		FARPROC	callproc = (FARPROC)MakeProcInstance(ControlProc, instance);
		m_callproc = (FARPROC)SetWindowLong(m_wndObj->GetSafeHwnd(), GWL_WNDPROC, (LONG)callproc);
	}

	if (m_fmProp->kind == fmOBJECT)
	{
		int idx = m_fmProp->iCtmIdx;
		CString strProp = GetProperties();
		CString strEvent = GetEvent();
		CString strMethod = GetMethod();
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_SETCTMPROP, idx), (LPARAM)strProp.operator LPCSTR());
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_SETCTMEVENT, idx), (LPARAM)strEvent.operator LPCSTR());
		AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_SETCTMFUNC, idx), (LPARAM)strMethod.operator LPCSTR());
	}
}

CfmObject* CObjectLoad::getObject()
{
	return m_fmObj;
};

CWnd* CObjectLoad::getWnd()
{
	CWnd* ob = NULL;
	if(m_fmObj) ob = m_fmObj->GetWnd();
	return ob;
};

void CObjectLoad::SetHooking(bool bHook)
{
	m_bHook = bHook;
}

bool CObjectLoad::GetHooking()
{
	return m_bHook;
}

CString CObjectLoad::GetProperties()
{
	CString strProperties("");
	if (m_fmObj) strProperties = m_fmObj->GetProperties();
	return strProperties;
}

CString CObjectLoad::GetMethod()
{
	CString strMethod("");
	if (m_fmObj) strMethod = m_fmObj->GetMethod();
	return strMethod;
}

CString CObjectLoad::GetEvent()
{
	CString strEvent("");
	if (m_fmObj) strEvent = m_fmObj->GetEvent();
	return strEvent;
}

LRESULT CALLBACK ControlProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CObjectLoad* ctrl = (CObjectLoad *)GetWindowLong(hwnd, GWL_USERDATA);
	if (ctrl == NULL)
		return 0;

	bool bHook = ctrl->GetHooking();
	CRect rc;
	rc = ctrl->GetRect();
	WORD lw, hw;
	lw = LOWORD(lParam) + (WORD)rc.left;
	hw = HIWORD(lParam) + (WORD)rc.top;

	switch (msg)
	{
	case WM_MOUSEMOVE:
		ctrl->m_view->SendMessage(WM_OBJMSG, MAKEWPARAM(WM_MOUSEMOVE, 0), MAKELPARAM(lw, hw));
		if (!ctrl->getProp()->block || !(ctrl->getProp()->kind == fmGRID || ctrl->getProp()->kind == fmGRIDEX || ctrl->getProp()->kind == fmTABLE) ) 
			return true;
		break;
	}

	if (bHook)
	{
		switch (msg)
		{
		case WM_TIMER:
			break;
		case WM_SHOWWINDOW:
			break;
		case WM_KEYDOWN:
			break;
		case WM_RBUTTONDOWN:
			ctrl->m_view->SendMessage(WM_OBJMSG, MAKEWPARAM(WM_RBUTTONDOWN, 0), MAKELPARAM(lw, hw));
			return true;
		case WM_RBUTTONUP:
			ctrl->m_view->SendMessage(WM_OBJMSG, MAKEWPARAM(WM_RBUTTONUP, 0), MAKELPARAM(lw, hw));
			return true;
		case WM_RBUTTONDBLCLK:
			return true;
		case WM_LBUTTONDOWN:
			ctrl->m_view->SendMessage(WM_OBJMSG, MAKEWPARAM(WM_LBUTTONDOWN, 0), MAKELPARAM(lw, hw));
			return true;
		case WM_LBUTTONUP:
			ctrl->m_view->SendMessage(WM_OBJMSG, MAKEWPARAM(WM_LBUTTONUP, 0), MAKELPARAM(lw, hw));
			return true;
		case WM_LBUTTONDBLCLK:
			return true;
		}
	}

	return ::CallWindowProc((long(__stdcall *)(HWND, UINT, WPARAM, LPARAM))ctrl->m_callproc, hwnd, msg, wParam, lParam);
}
