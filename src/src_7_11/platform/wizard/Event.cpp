#include "stdafx.h"
#include "imm.h"
#include "Wizard.h"
#include "Client.h"
#include "Key.h"
#include "Mouse.h"
#include "Tips.h"
#include "Script.h"
#include "Stream.h"
#include "../h/axiserr.h"

LRESULT CALLBACK CallProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CClient* client = (CClient *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (client == NULL)
		return 0;

	static	CString	text;
	CDC	tDC;
	CPoint	point;
	CRect	rect; 
	bool	fBool;
	int	value;
	CScreen* screen;

	if (client->m_focus && client->m_status & S_LOAD && client->isTips())
		client->m_guard->m_tips->RelayEvent(client, msg, lParam);

	switch (msg)
	{
	case WM_PAINT:
		if (!client->m_view->GetUpdateRect(NULL, FALSE))
			break;

		PAINTSTRUCT	ps;
		CDC*		pDC;
 
		pDC = client->m_view->BeginPaint(&ps);

		if ((client->m_type & vtypeMSK) == vtypeSCR)
		{
			pDC->SetMapMode(MM_TEXT);
			point = pDC->GetWindowOrg() + ((CScrollView*)client->m_view)->GetScrollPosition();
			pDC->SetWindowOrg(point);
		}

		client->OnDraw(pDC);
		client->m_view->EndPaint(&ps);
		return TRUE;

	case WM_ERASEBKGND:
		CBrush	*backBrush, *oldBrush;

		if (client->m_bitmap == NULL)
			backBrush = new CBrush(client->m_bkground);
		else
		{
			CBitmap	bitmap;
			backBrush = new CBrush(bitmap.FromHandle(client->m_bitmap));
		}

		tDC.Attach((HDC)wParam);

		// Save old brush 
		tDC.GetClipBox(&rect);
		oldBrush = tDC.SelectObject(backBrush); 

		// Erase the area needed 
		tDC.PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
		tDC.SelectObject(oldBrush); 

		backBrush->DeleteObject();
		delete backBrush;

		tDC.Detach();
		return TRUE;

	case WM_MOUSEWHEEL:
		if (client->m_status & S_LOAD)
		{
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			ScreenToClient(hwnd, &point);
			if (point.x < 0 || point.y < 0)
			{
				::CallWindowProc((LRESULT (__stdcall *)(HWND, UINT, WPARAM, LPARAM))client->m_callproc, hwnd, msg, wParam, lParam);
				client->OnScroll();
				return TRUE;
			}

			client->m_mouse->m_repeat = -1;
			client->m_mouse->SaveMouse(point);
			fBool = client->m_mouse->OnWheel(point, wParam);
			client->m_mouse->RestoreMouse();
			if (!fBool)
			{
				::CallWindowProc((LRESULT (__stdcall *)(HWND, UINT, WPARAM, LPARAM))client->m_callproc, hwnd, msg, wParam, lParam);
				client->OnScroll();
			}
			return TRUE;
		}
		break;

	case WM_SIZE:
		if (client->m_status & S_LOAD)
		{
			if ((client->m_type & vtypeMSK) == vtypeSCR)
				client->OnScroll();
			else
				client->OnResize(LOWORD(lParam), HIWORD(lParam));
		}
		break;

	case WM_VSCROLL:
	case WM_HSCROLL:
		client->OnScroll();
		break;

	case WM_SETCURSOR:
		if (LOWORD(lParam) != HTERROR && HIWORD(lParam) == WM_LBUTTONDOWN)
		{
			if (GetParent((HWND)wParam) == client->m_view->GetSafeHwnd())
				client->OnControl((HWND)wParam);
			else if (GetParent(GetParent((HWND)wParam)) == client->m_view->GetSafeHwnd())
				client->OnControl((HWND)GetParent((HWND)wParam));
		}
		break;

	case WM_SETFOCUS:
		if (GetParent((HWND)wParam) != client->m_view->GetSafeHwnd())
			client->OnFocus(TRUE, (HWND)wParam);
		break;
	
	case WM_KILLFOCUS:
		client->m_view->KillTimer(TM_REPBN);
		client->m_mouse->m_repeat = -1;
		if (GetParent((HWND)wParam) == client->m_view->GetSafeHwnd())
			client->OnControl((HWND)wParam);
		else if (GetParent(GetParent((HWND)wParam)) == client->m_view->GetSafeHwnd())
			client->OnControl((HWND)GetParent((HWND)wParam));
		else
			client->OnFocus(FALSE, (HWND)wParam);
		break;
	
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		if (client->m_status & S_LOAD)
		{
			client->m_view->KillTimer(TM_REPBN);
			client->m_mouse->m_repeat = -1;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			if (client->m_status & S_CHILD || client->OnControl())
			{
				client->m_view->SetFocus();
				break;
			}

			client->m_mouse->SaveMouse(point, true);
			client->m_mouse->OnDown(point, wParam);
			client->m_mouse->RestoreMouse(true);
			if (!(client->m_status & S_CHILD) && !client->m_focus)
				client->m_view->SetFocus();
		}
		break;

	case WM_MOUSEMOVE:
		if (client->m_focus && client->m_status & S_LOAD)
		{
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			client->m_mouse->SaveMouse(point);
			client->m_mouse->OnMove(point, wParam);
			client->m_mouse->RestoreMouse();
		}
		break;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		if (client->m_status & S_LOAD)
		{
			client->m_view->KillTimer(TM_REPBN);
			client->m_mouse->m_repeat = -1;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			client->m_mouse->SaveMouse(point);
			client->m_mouse->OnUp(point, wParam);
			client->m_mouse->RestoreMouse();
		}
		break;

	case WM_LBUTTONDBLCLK:
		if (client->m_status & S_LOAD)
		{
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			client->m_mouse->m_repeat = -1;
			client->m_mouse->SaveMouse(point, true);
			client->m_mouse->OnDblClick(point, wParam);
			client->m_mouse->RestoreMouse(true);
		}
		break;

	case WM_CHAR:
		if (client->m_status & S_LOAD)
			client->m_keyx->OnChar(wParam, lParam);
		break;

	case WM_IME_CHAR:
		if (client->m_status & S_LOAD)
			client->m_keyx->OnIME(wParam, GCS_RESULTSTR);
		break;

	case WM_IME_COMPOSITION:
		if (client->m_status & S_LOAD && !(lParam & GCS_RESULTSTR))
			client->m_keyx->OnIME(wParam, lParam);
		break;

	case WM_KEYDOWN:
		if ((client->m_status & S_LOAD) && (wParam < VK_NUMPAD0 || wParam > VK_DIVIDE))
		{
			if (wParam == VK_TAB)
				lParam = (lParam & KF_UP) ? true : (::GetKeyState(VK_SHIFT) < 0) ? true : false;
			client->m_keyx->OnKey(wParam, lParam);
		}
		break;

	case WM_IME_NOTIFY:
		switch (wParam)
		{
		case IMN_SETCONVERSIONMODE:
		case IMN_SETCOMPOSITIONWINDOW:
			client->ChangeIME(false);
			break;
		default:
			break;
		}
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TM_WAIT:
			client->WaitDone(NULL, true);
			client->m_guard->SetGuide(AE_TIMEOUT, client->m_key);
			break;
		case TM_RTM:
			client->OnClear();
			break;
		case TM_REPBN:
			if (client->m_mouse->m_repeat >= 0 && client->m_mouse->m_repeat < 3)
			{
				client->m_mouse->m_repeat++;
				break;
			}
			if (!client->m_mouse->OnRepeat(client->m_mouse->m_point))
			{
				client->m_mouse->m_repeat = -1;
				client->m_view->KillTimer(TM_REPBN);
			}
			break;
		default:
			if (wParam >= TM_VB)
			{
				client->m_view->KillTimer(wParam);
				if (client->GetAtScreen(screen, (int)wParam - TM_VB))
					client->m_vm->OnTimer(screen);
			}
			else if (wParam >= TM_REPTR && !client->m_stream->m_lock)
			{
				client->m_view->KillTimer(wParam);
				client->OnRepeat((int)wParam - TM_REPTR);
			}
			break;
		}
		break;

	case WM_USER:
		switch (LOWORD(wParam))
		{
		case getPALETTE:
			return client->m_guard->m_palette->GetPaletteRGB((int)lParam);
		case invokeTRx:
			return client->m_guard->Invoke((char *)lParam, HIWORD(wParam), client->m_key);
		case viewDLL:
			return client->m_guard->OpenView(client, (char *)lParam, HIWORD(wParam));
		case procDLL:
			text = (char *)lParam;
			if (!text.IsEmpty())
			{
				int	offs;

				offs = text.Find('\t');
				if (offs == -1)
					offs = text.GetLength();
				client->m_guard->OnProcedure(text.Left(offs), text.Mid(offs+1), "",
							LOBYTE(HIWORD(wParam)), HIBYTE(HIWORD(wParam)));
			}
			return TRUE;
		case accountDLL:
			client->m_guard->AddRegistry((char *)lParam, HIWORD(wParam));
			return TRUE;
		case variantDLL:
			switch (HIWORD(wParam))
			{
			case titleCC:
				client->m_guard->SendAxis(MAKEWPARAM(titleVIEW, client->m_key), lParam);
				break;
			case codeCC:
				client->m_guard->SetCode((char *)lParam);
				break;
			case historyCC:
				client->m_guard->GetCodes((char *)lParam, text, true);
				return (LRESULT)text.operator LPCTSTR();
			case accountCC:
				client->m_guard->GetAcName(text, (BOOL)lParam);
				return (LRESULT)text.operator LPCTSTR();
			case passCC:
				text.Empty();
				client->m_guard->GetAcPass((char *)lParam, text);
				return (LRESULT)text.operator LPCTSTR();
			case triggerCC:
				client->m_guard->OnTrigger(client, (char *)lParam);
				break;
			case guideCC:
				if (!client->m_stream->SetGuide(NULL, (char *)lParam))
					client->m_guard->SetGuide((char *)lParam, client->m_key);
				break;
			case pushCC:
				client->m_guard->PushData((char *)lParam);
				break;
			case popCC:
				text.Empty();
				client->m_guard->PopData((char *)lParam, text);
				return (LRESULT)text.operator LPCTSTR();
			case homeCC:
				text = client->m_guard->m_root;
				return (LRESULT)text.operator LPCTSTR();
			case userCC:
				client->m_guard->GetSignOn(text);
				return (LRESULT)text.operator LPCTSTR();
			case nameCC:
				client->m_guard->GetSignName(text);
				return (LRESULT)text.operator LPCTSTR();
			case orderCC:
				return (client->m_guard->m_term & flagACN) ? false : true;
			case confirmCC:
				value = client->m_guard->m_app->GetProfileInt(ENVIRONMENT, STATUS, 0);
				return (value & stConfirm) ? true : false;
			case whichCC:
				if (!client->m_guard->GetName(codeNAME, (char *)lParam, text, (int&)wParam))
					return client->m_guard->GetName(whichNAME, (char *)lParam, text, (int&)wParam);
				break;
			case prohibitCC:
				return (client->m_guard->m_term & flagXXX) ? true : false;
			case majorCC:
				return client->m_guard->GetKey(client->m_key);
			case minorCC:
				return client->m_key;
			case accountCCx:
				if (client->m_guard->GetCombo((char *)lParam, text, true))
					return (LRESULT)text.operator LPCTSTR();
				return 0;
			case accountCCs:
				client->m_guard->SetCombo((char *)lParam);
				return 0;
			case acnameCC:
				if (client->m_guard->GetAcName((char *)lParam, text))
					return (LRESULT)text.operator LPCTSTR();
				return 0;
			case passCCx:
				text = client->m_guard->GetLoginPass();
				return (LRESULT)text.operator LPCTSTR();
			case systemCC:
				text = client->m_guard->m_app->GetProfileString(WORKSTATION, SITEID, "N");
				if (text.IsEmpty())
					text = _T("N");
				return (LRESULT)text.operator LPCTSTR();
			}
			return TRUE;
		case codeTYPE:
			int	value;
			client->m_guard->GetName(whichTYPE, (char *)lParam, text, value);
			return value;
		case codeDLL:
			point = ((int)lParam < 0) ? CPoint(-1, -1) : CPoint(LOWORD(lParam), HIWORD(lParam));
			if (!client->m_guard->GetCode(LOBYTE(HIWORD(wParam)), text, HIBYTE(HIWORD(wParam)), point))
				text.Empty();
			return (LRESULT)text.operator LPCTSTR();
		case nameDLL:
			if (!client->m_guard->GetName(codeNAME, (char *)lParam, text, (int &)wParam))
			{
				if (!client->m_guard->GetName(whichNAME, (char *)lParam, text, (int&)wParam))
					text.Empty();
			}
			((char *)lParam)[0] = (char)wParam;
			return (LRESULT)text.operator LPCTSTR();
		case columnDLL:
			text = (char *)lParam;
			if (!client->m_guard->SetCells(client->m_mapN, text))
			{
				text.Empty();
				((char *)lParam)[0] = 0;
			}
			else
				((char *)lParam)[0] = 1;
			return (LRESULT)text.operator LPCTSTR();
		case excelDLL:
			text.Format("EX%s", client->m_mapN.Mid(L_SGID, L_SELC));
			client->m_guard->MakeXlsFile((char *)lParam, HIWORD(wParam) ? true : false, text);
			return TRUE;
		case closeDLL:
			client->m_guard->PostAxis(delVIEW, client->m_key, (LPARAM)0);
			return TRUE;
		case sizeDLL:
			client->m_guard->SendAxis(sizeVIEW, client->m_key, lParam);
			return TRUE;
		case formDLL:
			client->SetControl(HIWORD(wParam), (char *)lParam);
			return TRUE;
		case eventDLL:
			if (client->GetAtScreen(screen, LOBYTE(HIWORD(wParam))))
				screen->DoEvent(HIBYTE(HIWORD(wParam)), (char *)lParam);
			return TRUE;
		case autoDLL:
			client->m_guard->SendAxis(MAKEWPARAM(autoVIEW, HIWORD(wParam)), lParam);
			return TRUE;
		case ledgerDLL:
			client->m_guard->Ledger((char *)lParam);
			return TRUE;
		case waitDLL:
			return (client->m_status & S_WAIT);
		case getFONT:
			return (LRESULT)client->m_guard->m_draw->SetFont((struct _fontR *)lParam);
		case getPEN:
			return (LRESULT)client->m_guard->m_draw->SetPen((struct _penR *)lParam);
		case getBRUSH:
			return (LRESULT)client->m_guard->m_draw->SetBrush((COLORREF)lParam);
		case getBITMAP:
			return (LRESULT)client->m_guard->m_draw->SetBitmap((char *)lParam);
		case encPASSx:
			text = (char *)lParam;
			if (!text.IsEmpty())
			{
				int	idx = text.Find('\t');
				if (idx == -1)
					idx = text.GetLength();
				text = client->m_guard->Secure(text.Left(idx), (idx < text.GetLength()) ? text.Mid(idx+1) : _T(""), HIWORD(wParam));
			}
			return (LRESULT)text.operator LPCTSTR();
		case mapDLL:
			if (client->GetAtScreen(screen, HIWORD(wParam)))
				text = CString(screen->m_mapH->mapN, L_MAPN);
			else
				text = client->m_mapN;
			return (LRESULT)text.operator LPCTSTR();
		default:
			wParam = MAKEWPARAM(wParam, client->m_key);
			break;
		}
		break;
	}

	return ::CallWindowProc((LRESULT (__stdcall *)(HWND, UINT, WPARAM, LPARAM))client->m_callproc, hwnd, msg, wParam, lParam);
}
