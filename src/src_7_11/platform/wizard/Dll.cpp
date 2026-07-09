// Dll.cpp: implementation of the CDll class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Wizard.h"
#include "Dll.h"
#include "../h/axisvar.h"
#include "../h/axisrsm.h"
#include "../h/axiserr.h"
#include "../h/axstring.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LRESULT CALLBACK DllProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CDll* dll = (CDll *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (dll == NULL)
		return 0;

	static	CString	text;
	CPoint	pt;
	int	value;

	switch (msg)
	{
	case WM_ERASEBKGND:
		return TRUE;
	case WM_SETFOCUS:
		if (dll->m_status & S_LOAD)
			dll->m_dll->SetFocus();
		break;
	case WM_SIZE:
		if (dll->m_status & S_LOAD)
		{
			CRect	rect;
			switch (dll->m_type & vtypeMSK)
			{
			case vtypeSCR:
			case vtypeFIX:
				break;
			default:
				dll->m_view->GetClientRect(rect);
				dll->m_dll->MoveWindow(rect);
				break;
			}
		}
		break;

	case WM_USER:
		switch (LOWORD(wParam))
		{
		case getPALETTE:
			return dll->m_guard->m_palette->GetPaletteRGB((int)lParam);
		case invokeTRx:
			if (dll->m_guard->Write((char *)lParam, HIWORD(wParam), dll->m_key))
			{
				dll->OnUnit(((struct _userTH *)lParam)->key);
				return TRUE;
			}
			return FALSE;
		case viewDLL:
			return dll->m_guard->OpenView(dll, (char *)lParam, HIWORD(wParam));
		case dropDLL:
			if (HIWORD(wParam))
				dll->m_status |= S_DROPME;
			return dll->Register((HWND)lParam);
		case dragDLL:
			dll->Register(dll->m_dll->GetSafeHwnd(), true);
			dll->m_guard->OnDrag(dll->m_key, (char *)lParam);
			return TRUE;
		case revokeDLL:
			dll->Revoke((HWND)lParam);
			return TRUE;
		case alertDLLx:
			dll->xAlert((int)lParam);
			return TRUE;
		case clipDLL:
			if (lParam == NULL)
			{
				dll->m_guard->GetClipboard(text);
				return (LRESULT)(char *)text.operator LPCTSTR();
			}
			dll->m_guard->SetClipboard((char *)lParam);
			return TRUE;

		case procDLL:
			text = (char *)lParam;
			if (!text.IsEmpty())
			{
				int	offs;

				offs = text.Find('\t');
				if (offs == -1)
					offs = text.GetLength();
				dll->m_guard->OnProcedure(text.Left(offs), text.Mid(offs+1), "",
							LOBYTE(HIWORD(wParam)), HIBYTE(HIWORD(wParam)));
			}
			return TRUE;

		case mouseDLL:
			dll->OnMouse((char *)lParam, HIWORD(wParam));
			return TRUE;

		case accountDLL:
			dll->m_guard->AddRegistry((char *)lParam, HIWORD(wParam));
			return TRUE;

		case variantDLL:
			switch (HIWORD(wParam))
			{
			case titleCC:
				dll->m_guard->SendAxis(MAKEWPARAM(titleVIEW, dll->m_key), lParam);
				break;
			case codeCC:
				dll->m_guard->SetCode((char *)lParam);
				break;
			case historyCC:
				dll->m_guard->GetCodes((char *)lParam, text, true);
				return (LRESULT)text.operator LPCTSTR();
			case accountCC:
				dll->m_guard->GetAcName(text, (BOOL)lParam);
				return (LRESULT)text.operator LPCTSTR();
			case passCC:
				text.Empty();
				dll->m_guard->GetAcPass((char *)lParam, text);
				return (LRESULT)text.operator LPCTSTR();
			case triggerCC:
				dll->m_guard->OnTrigger(dll, (char *)lParam);
				break;
			case guideCC:
				dll->m_guard->SetGuide((char *)lParam, dll->m_key);
				break;
			case pushCC:
				dll->m_guard->PushData((char *)lParam);
				break;
			case popCC:
				text.Empty();
				dll->m_guard->PopData((char *)lParam, text);
				return (LRESULT)text.operator LPCTSTR();
			case homeCC:
				text = dll->m_guard->m_root;
				return (LRESULT)text.operator LPCTSTR();
			case userCC:
				dll->m_guard->GetSignOn(text);
				return (LRESULT)text.operator LPCTSTR();
			case nameCC:
				dll->m_guard->GetSignName(text);
				return (LRESULT)text.operator LPCTSTR();
			case orderCC:
				return (dll->m_guard->m_term & flagACN) ? false : true;
			case confirmCC:
				value = dll->m_guard->m_app->GetProfileInt(ENVIRONMENT, STATUS, 0);
				return (value & stConfirm) ? true : false;
			case whichCC:
				if (!dll->m_guard->GetName(codeNAME, (char *)lParam, text, (int&)wParam))
					return dll->m_guard->GetName(whichNAME, (char *)lParam, text, (int&)wParam);
				break;
			case prohibitCC:
				return (dll->m_guard->m_term & flagXXX) ? true : false;
			case majorCC:
				return dll->m_guard->GetKey(dll->m_key);
			case minorCC:
				return dll->m_key;
			case accountCCx:
				if (dll->m_guard->GetCombo((char *)lParam, text, true))
					return (LRESULT)text.operator LPCTSTR();
				return 0;
			case accountCCs:
				dll->m_guard->SetCombo((char *)lParam);
				return 0;
			case acnameCC:
				if (dll->m_guard->GetAcName((char *)lParam, text))
					return (LRESULT)text.operator LPCTSTR();
				return 0;
			case passCCx:
				text = dll->m_guard->GetLoginPass();
				return (LRESULT)text.operator LPCTSTR();
			case systemCC:
				text = dll->m_guard->m_app->GetProfileString(WORKSTATION, SITEID, "N");
				if (text.IsEmpty())
					text = _T("N");
				return (LRESULT)text.operator LPCTSTR();
			}
			return TRUE;
		case codeTYPE:
			dll->m_guard->GetName(whichTYPE, (char *)lParam, text, value);
			return value;
		case codeDLL:
			pt = ((int)lParam < 0) ? CPoint(-1, -1) : CPoint(LOWORD(lParam), HIWORD(lParam));
			if (!dll->m_guard->GetCode(LOBYTE(HIWORD(wParam)), text, HIBYTE(HIWORD(wParam)), pt))
				text.Empty();
			return (LRESULT)text.operator LPCTSTR();
		case nameDLL:
			if (!dll->m_guard->GetName(codeNAME, (char *)lParam, text, (int &)wParam))
			{
				if (!dll->m_guard->GetName(whichNAME, (char *)lParam, text, (int&)wParam))
					text.Empty();
			}
			((char *)lParam)[0] = (char)wParam;
			return (LRESULT)text.operator LPCTSTR();
		case columnDLL:
			text = (char *)lParam;
			if (!dll->m_guard->SetCells(dll->m_mapN, text))
			{
				text.Empty();
				((char *)lParam)[0] = 0;
			}
			else
				((char *)lParam)[0] = 1;
			return (LRESULT)text.operator LPCTSTR();
		case excelDLL:
			text.Format("EX%s", dll->m_mapN.Mid(L_SGID, L_SELC));
			dll->m_guard->MakeXlsFile((char *)lParam, HIWORD(wParam) ? true : false, text);
			return TRUE;
		case closeDLL:
			dll->m_guard->PostAxis(delVIEW, dll->m_key, (LPARAM)0);
			return TRUE;
		case sizeDLL:
			dll->m_guard->SendAxis(sizeVIEW, dll->m_key, lParam);
			return TRUE;
		case formDLL:
		case eventDLL:
			return TRUE;
		case autoDLL:
			dll->m_guard->SendAxis(MAKEWPARAM(autoVIEW, HIWORD(wParam)), lParam);
			return TRUE;
		case sizeMAP:
			return dll->GetSize((char *)lParam);
		case attachMAP:
			return dll->Attach((CWnd *)lParam, HIWORD(wParam));
		case detachMAP:
			dll->Detach(HIWORD(wParam));
			return TRUE;
		case loadMAP:
			return dll->Load(HIWORD(wParam), (char *)lParam);
		case dominoMAP:
			dll->Domino(HIWORD(wParam), (char *)lParam);
			return TRUE;
		case ledgerDLL:
			dll->m_guard->Ledger((char *)lParam);
			return TRUE;
		case typeDLL:
			return dll->GetViewType((char *)lParam);
		case waitDLL:
			return dll->IsWait(HIWORD(wParam));
		case getFONT:
			return (LRESULT)dll->m_guard->m_draw->SetFont((struct _fontR *)lParam);
		case getPEN:
			return (LRESULT)dll->m_guard->m_draw->SetPen((struct _penR *)lParam);
		case getBRUSH:
			return (LRESULT)dll->m_guard->m_draw->SetBrush((COLORREF)lParam);
		case getBITMAP:
			return (LRESULT)dll->m_guard->m_draw->SetBitmap((char *)lParam);
		case encPASSx:
			text = (char *)lParam;
			if (!text.IsEmpty())
			{
				int	idx = text.Find('\t');
				if (idx == -1)
					idx = text.GetLength();
				text = dll->m_guard->Secure(text.Left(idx), (idx < text.GetLength()) ? text.Mid(idx+1) : _T(""), HIWORD(wParam));
			}
			return (LRESULT)text.operator LPCTSTR();
		case mapDLL:
			text = dll->m_mapN;
			return (LRESULT)text.operator LPCTSTR();

		case regCODE:
			if (dll->m_guard->m_cast && dll->m_guard->m_cast->IsRunning())
				dll->m_guard->m_cast->RegisterRTM(dll->m_guard->GetKey(dll->m_key), dll->m_key, (char *) lParam);
			return TRUE;

		case remainDLL:
			dll->m_guard->OnNotice((char *)lParam, strlen((char *)lParam), false);
			break;

		default:
			wParam = MAKEWPARAM(wParam, dll->m_key);
			break;
		}
		break;
	}

	return ::CallWindowProc((LRESULT (__stdcall *)(HWND, UINT, WPARAM, LPARAM))dll->m_callproc, hwnd, msg, wParam, lParam);
}

CDll::CDll(CGuard* guard, CWnd* view, int key, int type) : CWorks(guard, view, key, type)
{
	m_dll      = NULL;
	m_instance = NULL;

	HANDLE instance = (HANDLE)GetWindowLongPtr(m_view->GetSafeHwnd(), GWLP_HINSTANCE);
	FARPROC	dllproc = (FARPROC)MakeProcInstance(DllProc, instance);
	m_callproc      = (FARPROC) SetWindowLongPtr(m_view->GetSafeHwnd(), GWLP_WNDPROC, (LONG_PTR)dllproc);
	m_screens.RemoveAll();
}

CDll::~CDll()
{
	WORD	key;
	CWnd*	pWnd;

	Free();
	xAlert();

	for (POSITION pos = m_screens.GetStartPosition(); pos; )
	{
		m_screens.GetNextAssoc(pos, (WORD&)key, (void *&)pWnd);
		Detach(key);
	}
	m_screens.RemoveAll();
}

bool CDll::Attach(CString maps, bool only, bool fix)
{
	if (maps.GetLength() < L_MAPN)
		return false;

	CString	name, domino;
	char	tmapN[L_MAPN+1];
	CWnd*	dll;
	HINSTANCE instance;
	CWnd*	(APIENTRY* axCreate)(CWnd*);
	HWND	(APIENTRY* axCreateEx)(HWND);

	domino = maps.Mid(L_MAPN);
	maps   = maps.Left(L_MAPN);
	CopyMemory(tmapN, (char *)maps.operator LPCSTR(), L_MAPN);
	tmapN[L_MAPN] = '\0';

	bool	equal = false;
	bool	isvirtual = m_guard->GetVirtualTR(tmapN);
	if (isvirtual && !m_mapN.IsEmpty())
	{
		char	twb[L_MAPN+1];

		CopyMemory(twb, (char *)m_mapN.operator LPCSTR(), L_MAPN);
		twb[L_MAPN] = '\0';
		if (m_guard->GetVirtualTR(twb))
			equal = true;
	}

	if (!equal)
	{
		name.Format("%s\\%s\\%s.dll", m_guard->m_root, DEVDIR, tmapN);
		instance = AfxLoadLibrary(name);
		if (instance == NULL)
			return false;

		xAlert();
		m_mapN = maps;

		axCreate = (CWnd* (APIENTRY*)(CWnd*))GetProcAddress(instance, _T("axCreate"));
		if (axCreate == NULL)
		{
			axCreateEx = (HWND (APIENTRY*)(HWND))GetProcAddress(instance, _T("axCreateEx"));
			if (axCreateEx == NULL)
			{
				AfxFreeLibrary(instance);
				return false;
			}

			HWND hWnd = (*axCreateEx)(m_view->GetSafeHwnd());
			if (hWnd == NULL)
			{
				AfxFreeLibrary(instance);
				return false;
			}
			dll = new CWnd();
			dll->Attach(hWnd);
		}
		else
		{
			dll = (*axCreate)(m_view);
			if (dll == NULL)
			{
				AfxFreeLibrary(instance);
				return false;
			}
		}

		Free();
		m_dll      = dll;
		m_mapN     = maps;
		m_instance = instance;
		m_status  |= S_LOAD;
		if (fix)
			m_status |= S_FIX;
	}
	else
		xAlert();

	if (isvirtual)
		m_dll->SendMessage(WM_USER, MAKEWPARAM(DLL_SELECT, 0), (LPARAM)maps.Mid(L_SGID, L_SELC).operator LPCTSTR());

	if (!only || isvirtual)
	{
		if (domino.IsEmpty() && !equal)
			m_guard->GetCode(domino);
		m_dll->SendMessage(WM_USER, MAKEWPARAM(DLL_DOMINO, 0), (LPARAM)domino.operator LPCTSTR());
	}
	SetFont(m_font, m_guard->m_resize);
	m_dll->SendMessage(WM_USER, MAKEWPARAM(DLL_SETFCB, (m_guard->m_status & WS_FCB) ? true : false), m_guard->m_flash);

/*	if (!m_mapN.CompareNoCase("et050100"))
	{
		struct	_axisH*	axisH;
		int	nBytes = 65 * 1024;
		char*	pBytes = new char[L_axisH + nBytes];

		axisH = (struct _axisH *)pBytes;
		ZeroMemory(pBytes, L_axisH+nBytes);
		axisH->msgK = msgK_AXIS;
		axisH->winK = this->m_key;
		axisH->unit = 0;
		FillMemory(&pBytes[L_axisH], nBytes, 'A');

		OnAxis(axisH, &pBytes[L_axisH], nBytes);
	}
	*/
	return true;
}

void CDll::OnAxis(struct _axisH* axisH, char* pBytes, int nBytes)
{
	struct _auxH*	auxH;
	CString	guide = _T("");

	if (axisH->stat & statAUX)
	{
		auxH = (struct _auxH *)pBytes;
		nBytes -= L_auxH;
		if (nBytes < 0)
			return;
		pBytes += L_auxH;
		nBytes -= auxH->datL;
		if (nBytes < 0)
			return;

		guide = CString(pBytes, auxH->datL);
		pBytes += auxH->datL;
	}

	if (nBytes > 0)
	{
		if (nBytes < 64 * 1024)
			m_dll->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_OUB, axisH->unit), nBytes), (LPARAM)pBytes);
		else
		{
		//	m_dll->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_OUBx, nBytes >> 16), nBytes), (LPARAM)pBytes);
			char*	datb;
			struct	_extTH* extTH;

			datb = new char[L_extTH+nBytes];
			extTH = (struct _extTH *)datb;
			extTH->key = axisH->unit;
			extTH->size = nBytes;
			CopyMemory(&datb[L_extTH], pBytes, nBytes);
			m_dll->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_OUBx, 0), 0), (LPARAM)datb);
			delete[] datb;			
		}
	}

	if (guide.IsEmpty())
		m_guard->SetGuide(AE_CLEAR, m_key);
	else
	{
//		if (auxH->type != typeFM || !m_dll->SendMessage(WM_USER, MAKEWPARAM(DLL_GUIDE, 0), (LPARAM)guide.operator LPCTSTR()))
		if (auxH->type != typeFM && !m_dll->SendMessage(WM_USER, MAKEWPARAM(DLL_GUIDE, 0), (LPARAM)guide.operator LPCTSTR()))
			m_guard->SetGuide(auxH->type, guide, m_key);
	}
	m_guard->PostAxis(waitPAN, m_key, false);
}

void CDll::OnTrigger(CString name, CString text, int key)
{
	if (m_status & S_WAIT)
		return;

	name += '\t';
	name += text;
	m_dll->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_TRIGGER, key), false), (LPARAM)name.operator LPCTSTR());
}

void CDll::OnTrigger(CString data)
{
	if (m_status & S_WAIT)
		return;

	m_dll->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_TRIGGER, -1), false), (LPARAM)data.operator LPCTSTR());
}

void CDll::OnProcedure(CString proc, CString param, int key)
{
	CWnd*	pWnd;
	CWorks*	works;

	if (m_screens.Lookup(key, (void *&)pWnd) && m_guard->GetClient(key, works))
		works->OnProcedure(proc, param);
	else
	{
		proc += '\t';
		proc += param;
		m_dll->SendMessage(WM_USER, MAKEWPARAM(DLL_TRIGGER, true), (LPARAM)proc.operator LPCTSTR());
	}
}

CString CDll::OnNotice(CString text)
{
	if (m_status & S_LOAD)
		return (char *)m_dll->SendMessage(WM_USER, MAKEWPARAM(DLL_NOTICE, 0), (LPARAM)text.operator LPCTSTR());
	return _T("");
}

void CDll::OnDomino(CString domino)
{
	if (m_status & S_LOAD)
		m_dll->SendMessage(WM_USER, MAKEWPARAM(DLL_DOMINO, 0), (LPARAM)domino.operator LPCTSTR());
}

LRESULT CDll::OnInvoke(int wParam, int lParam)
{
	if (m_status & S_LOAD)
		return m_dll->SendMessage(WM_USER, MAKEWPARAM(DLL_INVOKE, wParam), (LPARAM)lParam);
	return 0;
}

void CDll::OnDrop(CPoint point, CString text, CWnd* pWnd)
{
	CString	tmps;
	int	key;

	if (m_status & S_WAIT)
		return;

	key = text.GetAt(0);
	text.Delete(0);
	if (m_status & S_DROPME && pWnd != NULL)
	{
		pWnd->ScreenToClient(&point);
		tmps.Format("%d\t%d\n%s", point.x, point.y, text);
		pWnd->SendMessage(WM_USER, MAKEWPARAM(DLL_DROP, MAKEWORD(key, m_key)), (LPARAM)tmps.operator LPCTSTR());
	}
	else
	{
		m_dll->ScreenToClient(&point);
		tmps.Format("%d\t%d\n%s", point.x, point.y, text);
		m_dll->SendMessage(WM_USER, MAKEWPARAM(DLL_DROP, MAKEWORD(key, m_key)), (LPARAM)tmps.operator LPCTSTR());
	}
}

void CDll::SetPalette(bool force)
{
	m_dll->PostMessage(WM_USER, MAKEWPARAM(DLL_SETPAL, 0));
}

void CDll::SetFCB(bool blink, int flash)
{
	m_dll->PostMessage(WM_USER, MAKEWPARAM(DLL_SETFCB, blink ? true : false), flash);
}

void CDll::Free()
{
	m_status &= S_DROP;

	if (m_instance)
	{
		if (m_dll)
		{
			if (IsWindow(m_dll->GetSafeHwnd()))
				m_dll->SendMessage(WM_CLOSE);
			m_dll->Detach();
			delete m_dll;
		}
		AfxFreeLibrary(m_instance);
	}
}

int CDll::GetSize(CString maps)
{
	int	size;

	m_guard->WorkView(maps.Left(L_MAPN), &size);
	return size;
}

int CDll::Attach(CWnd* pWnd, int type)
{
	int	key;

	if (type == 0)
		type = vtypeWND|vtypeNRM;
	key = m_guard->Attach(pWnd, type, WK_NORM);
	if (key != WK_NONE)
		m_screens.SetAt(key, pWnd);
	return key;
}

void CDll::Detach(int key)
{
	CWnd*	pWnd;

	if (m_screens.Lookup(key, (void *&)pWnd))
	{
		m_screens.RemoveKey(key);
		m_guard->Detach(key);
	}
}

BOOL CDll::Load(int key, CString maps)
{
	CWnd*	pWnd;
	CWorks*	works;
	int	size;

	if (!m_screens.Lookup(key, (void *&)pWnd) || !m_guard->GetClient(key, works))
		return FALSE;

	m_guard->WorkView(maps.Left(L_MAPN), &size);
	works->m_rect.SetRect(0, 0, LOWORD(size), HIWORD(size));
	works->m_group = m_group;
	if (works->Attach(maps, false, true))
	{
		works->m_status |= S_WORKS;
		pWnd->SetFocus();
		return TRUE;
	}
	works->m_status |= S_WORKS;
	return FALSE;
}

void CDll::Domino(int key, CString data)
{
	CWnd*	pWnd;
	CWorks*	works;
	CString	name;
	int	idx;

	if (m_status & (S_WAIT|S_TRIX|S_XTRI))
		return;

	if ((idx = data.Find('\t')) == -1)
		return;

	m_status |= S_TRIX;
	name = data.Left(idx++);
	data = data.Mid(idx);

	if (key == WK_NONE)
	{
		for (POSITION pos = m_screens.GetStartPosition(); pos; )
		{
			m_screens.GetNextAssoc(pos, (WORD&)key, (void *&)pWnd);
			if (m_guard->GetClient(key, works))
				works->OnTrigger(name, data, -1);
		}
	}
	else if (m_screens.Lookup(key, (void *&)pWnd) && m_guard->GetClient(key, works))
		works->OnTrigger(name, data, -1);
	m_status &= ~S_TRIX;
}

BOOL CDll::SetFont(int point, bool resize, bool attach)
{
	if (!resize || !(m_status & S_FIX))
	{
		m_font = point;
		m_dll->SendMessage(WM_USER, MAKEWPARAM(resize ? DLL_SETFONTx : DLL_SETFONT, point));
	}
	return TRUE;
}

void CDll::SetGID(int group)
{
	CWnd*	pWnd;
	CWorks*	works;
	WORD	key;

	m_group = group;
	for (POSITION pos = m_screens.GetStartPosition(); pos; )
	{
		m_screens.GetNextAssoc(pos, (WORD&)key, (void *&)pWnd);
		if (m_guard->GetClient(key, works))
			works->m_group = group;
	}
}

int CDll::GetViewType(CString mapN)
{
	int	size;
	return m_guard->WorkView(mapN, &size);
}

BOOL CDll::IsWait(int key)
{
	CWorks*	works;

	if (m_guard->GetClient(key, works))
		return (works->m_status & S_WAIT);

	return (m_status & S_WAIT);
}

bool CDll::isTrigger(int key, CString name, CString text)
{
	CWnd*	pWnd;
	CWorks*	works;
	WORD	keys;

	if (!m_screens.Lookup(key, (void *&)pWnd))
		return false;

	if (m_status & (S_WAIT|S_TRIX|S_XTRI))
		return true;

	m_status |= S_TRIX;
	for (POSITION pos = m_screens.GetStartPosition(); pos; )
	{
		m_screens.GetNextAssoc(pos, (WORD&)keys, (void *&)pWnd);
		if (keys != key && m_guard->GetClient(keys, works))
			works->OnTrigger(name, text, -1);
	}
	OnTrigger(name, text, 0);

	m_status &= ~S_TRIX;
	return true;
}

bool CDll::isYours(int key)
{
	CWnd*	pWnd;
	CWorks*	works;

	if (!m_screens.Lookup(key, (void *&)pWnd) || !m_guard->GetClient(key, works))
		return false;
	return true;
}

void CDll::xAlert(int unit)
{
	if (m_units.GetCount() <= 0)
		return;

	CString	text;
	BYTE	key, tmpv;

	if (unit != -1)
	{
		if (m_units.Lookup(unit, key))
		{
			text = (TCHAR)unit;
			m_units.RemoveKey(unit);
			m_guard->Write(msgK_MAPX, "", (char *)text.operator LPCTSTR(), text.GetLength(), m_key);
		}
		return;
	}


	text.Empty();
	for (POSITION pos = m_units.GetStartPosition(); pos; )
	{
		m_units.GetNextAssoc(pos, key, tmpv);
		text += key;
	}
	m_units.RemoveAll();
	m_guard->Write(msgK_MAPX, "", (char *)text.operator LPCTSTR(), text.GetLength(), m_key);
}

void CDll::OnMouse(CString infos, BOOL left)
{
	int	offs;
	CString	text;
	static	CString	maps;
	CPoint	point = CPoint(0, 0);
	bool	grid  = false;

	for (int ii = 0; ii < 3; ii++)
	{
		offs = infos.Find('\n');
		if (offs != -1 && ii != 2)
		{
			text  = infos.Left(offs);
			infos = infos.Mid(offs+1);
		}
		else
			text = infos;

		switch (ii)
		{
		case 0:		// mapN
			offs = text.Find('\t');
			if (offs != -1)
			{
				maps = text.Left(offs);
				grid = true;
			}
			else	maps = text;
			break;
		case 1:		// point
			offs = text.Find('\t');
			if (offs != -1)
			{
				point.x = atoi(text.Left(offs));
				point.y = atoi(text.Mid(offs+1));
			}
			break;
		case 2:		// data
			text += '\n';	break;
		}
	}

	if (maps.IsEmpty())
		return;

	int	index;
	char	*ptr, wb[1024];;
	CStringArray	items;
	CString	string, path, mapN;

	items.RemoveAll();
	CMenu *popM = new CMenu();
	popM->CreatePopupMenu();
	m_guard->GetSignName(string);
	path.Format("%s\\%s\\%s\\%s", m_guard->m_root, USRDIR, string, maps.Left(L_SELC));

	index = -1;
	DWORD	rc = GetPrivateProfileSection(maps, wb, sizeof(wb), path);
	if (rc <= 0)
	{
		path.Format("%s\\%s\\%s", m_guard->m_root, MTBLDIR, maps.Left(L_SELC));
		rc = GetPrivateProfileSection(maps, wb, sizeof(wb), path);
	}
	ptr = wb;
	for ( ; rc > 0; ptr++, rc--)
	{
		string = ptr;
		ptr   += string.GetLength();
		rc    -= string.GetLength();
		if (string.GetLength() <= L_MAPN)
		{
			if (string.Find('-') == 0)
				popM->AppendMenu(MF_SEPARATOR, 0);
			continue;
		}
		mapN   = string.Left(L_MAPN);
		string = string.Mid(L_MAPN);
		offs   = string.Find('=');
		if (offs != -1)
			string = string.Mid(offs+1);

		index = (int)items.Add(mapN);
		popM->AppendMenu(MF_STRING|MF_ENABLED, menuBASE+index, string);
	}

	if (index >= 0 && m_guard->GetString(AS_LINK, string))
	{
		index++;
		popM->AppendMenu(MF_SEPARATOR, 0);
		popM->AppendMenu(MF_STRING|MF_ENABLED, menuLINK, string);
	}

	if (grid && m_guard->GetString(AS_2XLS, string))
	{
		if (++index > 0)
			popM->AppendMenu(MF_SEPARATOR, 0);
		popM->AppendMenu(MF_STRING|MF_ENABLED, menuXLS, string);
	}

	if (index < 0)
	{
		delete popM;
		return;
	}

	index = popM->TrackPopupMenu(TPM_LEFTALIGN|TPM_RETURNCMD, point.x, point.y, m_dll);
	delete popM;

	if (index >= menuBASE)
	{
		index -= menuBASE;
		if (index < items.GetSize())
		{
			mapN = items.GetAt(index);
			mapN += (m_guard->m_status & WS_LINK) ? "/t/d" : "/s/t/d";
			mapN += text;
			m_guard->OpenView(this, mapN, typeVIEW);
		}
		items.RemoveAll();
		return;
	}
	items.RemoveAll();

	switch (index)
	{
	case menuLINK:
		m_guard->PostAxis(MAKEWPARAM(linkVIEW, 0), (LPARAM)maps.operator LPCTSTR());
		break;
	case menuXLS:
		m_dll->SendMessage(WM_USER, MAKEWPARAM(DLL_EXCEL, 0), (LPARAM)0);
		break;
	}
}

void CDll::OnAlert(void* data)
{
	if (m_status & S_LOAD)
		m_dll->SendMessage(WM_USER, MAKEWPARAM(DLL_ALERTx, 0), (LPARAM)data);
}