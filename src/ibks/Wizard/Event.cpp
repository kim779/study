#include "pch.h"
#include "imm.h"
#include "mmsystem.h"
#include "Wizard.h"
#include "Client.h"
#include "Key.h"
#include "Mouse.h"
#include "Tips.h"
#include "Script.h"
#include "Stream.h"
//#include "OnTimer.h"
#include "../h/axiserr.h"

LRESULT CALLBACK CallProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CClient* client = (CClient *)GetWindowLong(hwnd, GWL_USERDATA);
	if (client == NULL)
		return 0;

	static	CString	text;
	CDC	tDC;
	CPoint	point;
	CRect	rect; 
	bool	fBool;
	int	value;
	CScreen* screen;
	CString slog;

	if (client->m_focus && (client->m_status & S_LOAD) && client->isTips())
		client->m_guard->m_tips->RelayEvent(client, msg, lParam);

	switch (msg)
	{
	case WM_DESTROY:
		if (::IsWindow(client->m_view->GetSafeHwnd()))
			client->OnClose();
		break;

	case WM_PAINT:
		if (!client->m_view->GetUpdateRect(NULL, FALSE))
			break;

		PAINTSTRUCT	ps;
		CDC*		pDC;
 // updateXXX_2022XX
		pDC = client->m_view->BeginPaint(&ps);
		if (pDC == nullptr)
			break;

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
		return TRUE;		// 20120228 IBK 요청

		CBrush	*backBrush, *oldBrush;

		if (client->m_bitmap == NULL)
			backBrush = new CBrush(client->m_bkground);
		else
		{
			CBitmap	bitmap;
			backBrush = new CBrush(bitmap.FromHandle(client->m_bitmap));
		}

// udpateXXX_2022XX
		if (!tDC.Attach((HDC)wParam))
		{
			backBrush->DeleteObject();
			delete backBrush;
			break;
		}

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
// updateXXX_2022XX
			point.x = static_cast<short>(LOWORD(lParam));
			point.y = static_cast<short>(HIWORD(lParam));
			ScreenToClient(hwnd, &point);
			if (point.x < 0 || point.y < 0)
			{
				::CallWindowProc((long (__stdcall *)(HWND, UINT, WPARAM, LPARAM))client->m_callproc, hwnd, msg, wParam, lParam);
				client->OnScroll();
				return TRUE;
			}

			client->m_mouse->m_repeat = -1;
			client->m_mouse->SaveMouse(point);
			fBool = client->m_mouse->OnWheel(point, wParam);
			client->m_mouse->RestoreMouse();
			if (!fBool)
			{
				::CallWindowProc((long (__stdcall *)(HWND, UINT, WPARAM, LPARAM))client->m_callproc, hwnd, msg, wParam, lParam);
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
		{
			
			
			if (client->m_pPrtWnd)
			{
				//slog.Format("[WIZARD][setfocus][Event.cpp %s]<%d> map=[%s] wparam = [%d]  windowID=[%d]", __FUNCTION__, __LINE__, client->m_mapN, (int)wParam, client->m_pPrtWnd->GetDlgCtrlID());
				//OutputDebugString(slog); //m_pPrtWnd
				//if ((int)wParam == client->m_pPrtWnd->GetDlgCtrlID())
					break;;
			}
			client->OnFocus(TRUE, (HWND)wParam);
		}
		break;
	
	case WM_KILLFOCUS:
		{
			if (GetParent((HWND)wParam) == client->m_view->GetSafeHwnd())
				client->OnControl((HWND)wParam);
			else if (GetParent(GetParent((HWND)wParam)) == client->m_view->GetSafeHwnd())
				client->OnControl((HWND)GetParent((HWND)wParam));
			else if (GetParent(GetParent(GetParent((HWND)wParam))) == client->m_view->GetSafeHwnd())
				client->OnControl((HWND)GetParent(GetParent((HWND)wParam)));
			else
			{

				if (client->m_pPrtWnd)
				{
				/*	slog.Format("[WIZARD][killfocus][Event.cpp %s]<%d> map=[%s]   wparam = [%d]  windowID=[%d]", __FUNCTION__, __LINE__, client->m_mapN, (int)wParam, client->m_pPrtWnd->GetDlgCtrlID());
					OutputDebugString(slog);
					if ((int)wParam == client->m_pPrtWnd->GetDlgCtrlID())*/
						break;;
				}
			
				client->OnFocus(FALSE, (HWND)wParam);
			}
	
			if (!client->m_focus)
			{
				client->m_view->KillTimer(TM_REPBN);
				client->m_mouse->m_repeat = -1;
			}
		}
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
				slog.Format("[WIZARD][spin][%s]<%d> 스핀버튼 클릭햇는데 break", __FUNCTION__, __LINE__);
				OutputDebugString(slog);
				break;
			}

slog.Format("[WIZARD][spin][%s]<%d>", __FUNCTION__, __LINE__);
OutputDebugString(slog);

			client->m_mouse->SaveMouse(point, true);
			client->m_mouse->OnDown(point, wParam);
			client->m_mouse->RestoreMouse(true);
//			if (!(client->m_status & S_CHILD) && !client->m_focus)
			if (!(client->m_status & S_CHILD) && (!client->m_focus || GetFocus() != client->m_view->GetSafeHwnd()))
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

	case WM_MOUSELEAVE:
		client->m_mouse->OnCursor(true);
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

	case MM_MCINOTIFY:
		client->OnNotifyMCI(wParam, lParam);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TM_WAIT:
			client->WaitDone(NULL, true);
			client->m_guard->SetGuide(AE_TIMEOUT, client->m_key);
			slog.Format("[wizard] TM_WAIT[%s]", client->m_mapN);
			LOG_OUTP(3, "wizard", __FUNCTION__, slog);
			FileLog(slog, "timeout.ini");
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
			if (wParam >= TM_VBx)
			{
				int	key, id;

				key = (wParam / TM_VBx)-1;
				id  = (wParam % TM_VBx);
				client->m_view->KillTimer(wParam);
				client->m_timers.RemoveKey(wParam);
				if (!(client->m_status & S_ING) && client->GetAtScreen(screen, key))
					client->m_vm->OnTimerX(screen, id);
			}			
			else if (wParam >= TM_VB)
			{
				client->m_view->KillTimer(wParam);
				if (!(client->m_status & S_ING) && client->GetAtScreen(screen, wParam - TM_VB))
					client->m_vm->OnTimer(screen);
			}
			else if (wParam >= TM_REPTR && !client->m_stream->m_lock)
			{
				client->m_view->KillTimer(wParam);
				client->OnRepeat(wParam - TM_REPTR);
			}
			break;
		}
		break;

	case WM_USER:
		switch (LOWORD(wParam))
		{
		case getPALETTE:
			return client->m_guard->m_palette->GetPaletteRGB(lParam);
		case invokeTRx:
		case invokeALLMarketTRx:  //modi nxt
		case invokeNTXMarketTRx:
slog.Format("[WIZARD][SEND][controll dll][Event.cpp %s]<%d> map=[%s] ",__FUNCTION__, __LINE__,  client->m_mapN);
OutputDebugString(slog);
			if (LOWORD(wParam) == invokeTRx)
				return client->m_guard->Invoke((char*)lParam, HIWORD(wParam), client->m_key);
			else if (LOWORD(wParam) == invokeALLMarketTRx)
				return client->m_guard->Invoke((char*)lParam, HIWORD(wParam), client->m_key, 0);
			else if (LOWORD(wParam) == invokeNTXMarketTRx)
				return client->m_guard->Invoke((char*)lParam, HIWORD(wParam), client->m_key, 2);

		case viewDLL:
			return client->m_guard->OpenView(client, (char *)lParam, HIWORD(wParam));
		case procDLL:
			text = (char *)lParam;
			if (!text.IsEmpty())
			{
				int offs = text.Find('\t');
				if (offs == -1)
					offs = text.GetLength();
				client->m_guard->OnProcedure(text.Left(offs), text.Mid(offs+1), "",
							LOBYTE(HIWORD(wParam)), HIBYTE(HIWORD(wParam)));
			}
			return TRUE;
		case mouseDLL:
			client->OnMouse((char *)lParam, HIWORD(wParam));
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
			case codeCCx:
				client->m_guard->RemoveCode((char *)lParam);
				break;
			case historyCC:
				client->m_guard->GetCodes((char *)lParam, text, true);
				return (long)text.operator LPCTSTR();
			case accountCC:
				client->m_guard->GetAcName(text, (BOOL)lParam);
				return (long)text.operator LPCTSTR();
			case passCC:
				text.Empty();
				client->m_guard->GetAcPass((char *)lParam, text);
				return (long)text.operator LPCTSTR();
			case triggerCC:
				client->m_guard->OnTrigger(client, (char *)lParam);
				break;
			case guideCC:
				client->m_guard->SetGuide((char *)lParam, client->m_key);
				break;
			case pushCC:
				client->m_guard->PushData((char *)lParam);
				break;
			case popCC:
				text.Empty();
				client->m_guard->PopData((char *)lParam, text);
				return (long)text.operator LPCTSTR();
			case homeCC:
				text = client->m_guard->m_root;
				return (long)text.operator LPCTSTR();
			case userCC:
				client->m_guard->GetSignOn(text);
				return (long)text.operator LPCTSTR();
			case nameCC:
				client->m_guard->GetSignName(text);
				return (long)text.operator LPCTSTR();
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
			case orderCCx:
				text = client->m_guard->GetLoginData(2);
				return atoi(text);
			case majorCC:
				return client->m_guard->GetKey(client->m_key);
			case minorCC:
				return client->m_key;
			case accountCCx:
				if (client->m_guard->GetCombo((char *)lParam, text, true))
					return (long)text.operator LPCTSTR();
				return 0;
			case accountCCs:
				client->m_guard->SetCombo((char *)lParam);
				return 0;
			case acnameCC:
				if (client->m_guard->GetAcName((char *)lParam, text))
					return (long)text.operator LPCTSTR();
				return 0;
			case systemCC:
				text = client->m_guard->m_app->GetProfileString(WORKSTATION, SITEID, "D");
				if (text.IsEmpty())
					text = _T("D");
				return (long)text.operator LPCTSTR();
			case deptCC:
				text = client->m_guard->GetLoginData(0);
				return (long)text.operator LPCTSTR();
			case dateCC:
				text = client->m_guard->GetLoginData(1);
				return (long)text.operator LPCTSTR();
			case passCCx:
				text = client->m_guard->GetLoginPass();
				return (long)text.operator LPCTSTR();
			case groupCC:
				text = lParam ? client->m_guard->m_future_accs : client->m_guard->m_stock_accs;
				return (long)text.operator LPCTSTR();
			case groupCCx:
				return client->m_group;
			case acinfoCC:
				if (client->m_guard->GetAcName((char *)lParam, text, true))
					return (long)text.operator LPCTSTR();
				return 0;
			case passCA:
				text = client->m_guard->GetCAPass();
				return (long)text.operator LPCTSTR();
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
			return (long)text.operator LPCTSTR();
		case nameDLL:
			if (!client->m_guard->GetName(codeNAME, (char *)lParam, text, (int &)wParam))
			{
				if (!client->m_guard->GetName(whichNAME, (char *)lParam, text, (int&)wParam))
					text.Empty();
			}
			((char *)lParam)[0] = wParam;
			return (long)text.operator LPCTSTR();
		case columnDLL:
			text = (char *)lParam;
			if (!client->m_guard->SetCells(client->m_mapN, text))
			{
				text.Empty();
				((char *)lParam)[0] = 0;
			}
			else
				((char *)lParam)[0] = 1;
			return (long)text.operator LPCTSTR();
		case excelDLL:
			text.Format(_T("EX%s"), LPCTSTR(client->m_mapN.Mid(L_SGID, L_SELC)));
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
		case getFONT:
			return (long)client->m_guard->m_draw->SetFont((struct _fontR *)lParam);
		case getPEN:
			return (long)client->m_guard->m_draw->SetPen((struct _penR *)lParam);
		case getBRUSH:
			return (long)client->m_guard->m_draw->SetBrush((COLORREF)lParam);
		case getCURSOR:
			return (long)client->m_guard->GetCursor((int)lParam);
		case getBITMAP:
			return (long)client->m_guard->m_draw->SetBitmap((char *)lParam);
		case encPASSx:
			text = (char *)lParam;
			if (!text.IsEmpty())
			{
				int	idx = text.Find('\t');
				if (idx == -1)
					idx = text.GetLength();
				text = client->m_guard->Secure(text.Left(idx), (idx < text.GetLength()) ? text.Mid(idx+1) : _T(""), HIWORD(wParam));
			}
			return (long)text.operator LPCTSTR();
		case encSHAx:
			text = (char *)lParam;
			if (!text.IsEmpty())
				text = client->m_guard->Secure2(text);
			return (long)text.operator LPCTSTR();
		case mapDLL:
			if (client->GetAtScreen(screen, HIWORD(wParam)))
				text = CString(screen->m_mapH->mapN, L_MAPN);
			else
				text = client->m_mapN;
			return (long)text.operator LPCTSTR();
		case menuDLL:
			{
				char	wb[32] = { 0, };
				client->m_guard->SendAxis(MAKEWPARAM(menuVIEW, 0), (LPARAM)wb);
				text = CString(wb, min(strlen(wb), sizeof(wb)));
				return (long)text.operator LPCTSTR();
			}
		case linkDLL:
			client->m_guard->SendAxis(MAKEWPARAM(linkVIEW, 0), lParam);
			break;
		case tabsDLL:
			client->SetTabStop(LOBYTE(HIWORD(wParam)), HIBYTE(HIWORD(wParam)), (char *)lParam);
			return TRUE;
		case codeDLLx:
			text = (char *)lParam;
			if (!text.IsEmpty())
			{
				int	idx;
				CString	tmps;

				for (int ii = 0; ii < 3; ii++)
				{
					idx = text.Find('\t');
					if (idx == -1)
					{
						tmps = text;
						text.Empty();
					}
					else
					{
						tmps = text.Left(idx++);
						text = text.Mid(idx);
					}

					switch (ii)
					{
					case 0:
						point.x = tmps.IsEmpty() ? -1 : atoi(tmps);
						break;
					case 1:
						point.y = tmps.IsEmpty() ? -1 : atoi(tmps);
						break;
					case 2:
						text = tmps;
						break;
					}
				}
			}
			if (!client->m_guard->GetCode(LOBYTE(HIWORD(wParam)), text, HIBYTE(HIWORD(wParam)), point, true))
				text.Empty();
			return (long)text.operator LPCTSTR();
		case setRECT:
			client->SetControlRect((void *)lParam);
			break;
		case codeUNIT:
			return client->m_guard->GetCodeInfo(unitINFO, (char *)lParam, text, value);
		case codeINFO:
			client->m_guard->GetCodeInfo(codeINFOx, (char *)lParam, text, value);
			return (long)text.operator LPCTSTR();
		case elwBASIS:
			return client->m_guard->GetCodeInfo(codeELW, (char *)lParam, text, value);
		case listSTOCK:	
			client->m_guard->GetCodeInfo(codeSTOCKS, (char *)lParam, text, value);
			return (long)text.operator LPCTSTR();
		case editMODAL:
			return (long)client->m_guard->Modal(columnED, (char *)lParam);
		case focusDLL:
			return client->SetFocus(LOBYTE(HIWORD(wParam)), HIBYTE(HIWORD(wParam)) ? true : false, (CWnd *)lParam);
		default:
			wParam = MAKEWPARAM(wParam, client->m_key);
			break;
		}
		break;

	case WM_MEASUREITEM:
		if ((int)wParam == 223 || (int)wParam == 226)
			client->MeasureItem((LPMEASUREITEMSTRUCT)lParam, (int)wParam == 223);
		break;

	case WM_DRAWITEM:
		if ((int)wParam == 223 || (int)wParam == 226)
			client->DrawItem((LPDRAWITEMSTRUCT)lParam, (int)wParam == 223);
		break;
	}

// updateXXX_2022XX
	CWnd*	pWnd = CWnd::FromHandlePermanent(hwnd);
	if (!IsWindow(hwnd))
		return 0;
	return ::CallWindowProc((long (__stdcall *)(HWND, UINT, WPARAM, LPARAM))client->m_callproc, hwnd, msg, wParam, lParam);
}
