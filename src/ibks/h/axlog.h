// axlog.h - unified debug log helper, category on/off via DF_LOG_* defines.
// Shared across Wizard.dll, axisvbs.dll, axSock.ocx and AXIS.exe (inline, header-only - no link dependency).
#pragma once
#include <stdarg.h>
#include <stdio.h>

// Module tag shown as "[TAG][category]...". Define this before #include-ing
// axlog.h in a given .cpp to override; existing Wizard/axisvbs files that
// don't define it keep the original "WIZARD" tag unchanged.
#ifndef AXLOG_MODULE_TAG
#define AXLOG_MODULE_TAG "WIZARD"
#endif

#define DF_LOG_INIT
#define DF_LOG_EVENT
#define DF_LOG_DATA
//#define DF_LOG_RTM
#define DF_LOG_SCRIPT
//#define DF_LOG_FILEPATCH
//#define DF_LOG_LOGIN
//#define DF_LOG_SOCK_SEND
//#define DF_LOG_SOCK_RECEIVE
//#define DF_LOG_CERTIFY
// OFF by default: axisform.dll's CfmEdit::Draw/UpdateData fire on every repaint/keystroke, very high volume.
// #define DF_LOG_AXISFORM

enum axLogCat { LOG_INIT, LOG_EVENT, LOG_DATA, LOG_RTM, LOG_SCRIPT, LOG_AXISFORM, LOG_FILEPATCH, LOG_LOGIN, LOG_SOCK_SEND, LOG_SOCK_RECEIVE, LOG_CERTIFY };

inline bool axLogOn(axLogCat cat)
{
	switch (cat)
	{
#ifdef DF_LOG_INIT
	case LOG_INIT:   return true;
#endif
#ifdef DF_LOG_EVENT
	case LOG_EVENT:  return true;
#endif
#ifdef DF_LOG_DATA
	case LOG_DATA:   return true;
#endif
#ifdef DF_LOG_RTM
	case LOG_RTM:    return true;
#endif
#ifdef DF_LOG_SCRIPT
	case LOG_SCRIPT: return true;
#endif
#ifdef DF_LOG_AXISFORM
	case LOG_AXISFORM: return true;
#endif
#ifdef DF_LOG_FILEPATCH
	case LOG_FILEPATCH: return true;
#endif
#ifdef DF_LOG_LOGIN
	case LOG_LOGIN:  return true;
#endif
#ifdef DF_LOG_SOCK_SEND
	case LOG_SOCK_SEND:     return true;
#endif
#ifdef DF_LOG_SOCK_RECEIVE
	case LOG_SOCK_RECEIVE: return true;
#endif
#ifdef DF_LOG_CERTIFY
	case LOG_CERTIFY: return true;
#endif
	default: return false;
	}
}

inline void axlogImpl(axLogCat cat, const char* func, int line, LPCTSTR fmt, ...)
{
	if (!axLogOn(cat)) return;

	// Category names padded to the width of the longest one (SOCK_RECEIVE=12) so
	// "[TAG][xxx]" is always the same length regardless of category.
	static const char* catNames[] = {
		"INIT", "EVENT", "DATA", "RTM", "SCRIPT", "AXISFORM", "FILEPATCH", "LOGIN", "SOCK_SEND", "SOCK_RECEIVE", "CERTIFY"
	};

	va_list args;
	va_start(args, fmt);
	CString msg;
	msg.FormatV(fmt, args);
	va_end(args);

	CString funcLine;
	funcLine.Format("[%s:%d]", func, line);

	// funcLine is left-justified into a fixed field so the actual message
	// lines up in the same column regardless of function/class name length.
	// Longer entries just overflow the field without truncation or misalignment
	// of their own message - only later, shorter lines stay aligned.
	CString line_out;
	line_out.Format("[%s][%-12s] %-42s %s\n", AXLOG_MODULE_TAG, catNames[cat], (LPCTSTR)funcLine, (LPCTSTR)msg);

	OutputDebugString(line_out);
}

// Macro so __FUNCTION__/__LINE__ expand at the call site, not inside axlogImpl.
#define axlog(cat, ...) axlogImpl(cat, __FUNCTION__, __LINE__, __VA_ARGS__)

// axDiagLog(dir, sfile, fmt, ...) - persistent file logger for rare/hard-to-reproduce
// investigations (crash races, timing-dependent bugs). Unlike axlog() above, this is NOT
// gated by any DF_LOG_* define and does not go through OutputDebugString/DebugView - it
// always appends straight to a file, so it survives even when nobody happened to have
// DebugView open at the moment the rare condition hit.
// dir: caller-supplied base directory - axlog.h has no opinion on directory conventions
//      since those differ per project (e.g. AXIS's Axis::home+"\\user\\"+Axis::user+"\\Crashlog",
//      Wizard's m_guard->m_root-based path). Pass "" (empty) to default to the running exe's
//      own folder (GetModuleFileName), for callers that don't have/need a project-specific dir.
// sfile: log file name within that directory.
inline void axDiagLogImpl(LPCTSTR dir, LPCTSTR sfile, const char* func, int line, LPCTSTR fmt, ...)
{
	TRY
	{
		CString path;
		if (dir && dir[0])
		{
			path.Format("%s\\%s", dir, sfile);
		}
		else
		{
			TCHAR exePath[MAX_PATH];
			GetModuleFileName(NULL, exePath, MAX_PATH);
			CString exeDir(exePath);
			int pos = exeDir.ReverseFind('\\');
			if (pos != -1) exeDir = exeDir.Left(pos);
			path.Format("%s\\%s", (LPCTSTR)exeDir, sfile);
		}

		FILE* fp;
		fopen_s(&fp, path, "a+");
		if (!fp) return;

		va_list args;
		va_start(args, fmt);
		CString msg;
		msg.FormatV(fmt, args);
		va_end(args);

		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));
		fprintf(fp, "[%s:%d] %s\n", func, line, (LPCSTR)msg);

		fclose(fp);
	}
	CATCH(CMemoryException, e)
	{
	}
	END_CATCH
}

#define axDiagLog(dir, sfile, ...) axDiagLogImpl(dir, sfile, __FUNCTION__, __LINE__, __VA_ARGS__)

// axlogShow(cat, fmt, ...) - same content/formatting as axlog() above, but also appended to a
// small floating, non-blocking window instead of only OutputDebugString/DebugView. Use this at
// call sites where you need to *see* a value live while the screen keeps running - unlike
// AfxMessageBox()/MessageBox(), this never blocks the message loop (no modal dialog), so it
// can't stall a live trading screen the way a stray debug messagebox could.
// One shared window per DLL/module (this header is header-only, so each module that includes
// it gets its own independent window/static state - same "no link dependency" design as the
// rest of this file); closing it via the titlebar X just hides it, it reappears on the next
// axlogShow() call so history isn't lost. UI-thread only, same as every other axlog() call site.
namespace axlogShowDetail
{
	inline LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_SIZE:
		{
			HWND hEdit = ::GetWindow(hWnd, GW_CHILD);
			if (hEdit)
			{
				RECT rc;
				::GetClientRect(hWnd, &rc);
				::MoveWindow(hEdit, 0, 0, rc.right, rc.bottom, TRUE);
			}
			return 0;
		}
		case WM_CLOSE:
		{}
			// hide, don't destroy - keeps the accumulated log text around for next time
			//::ShowWindow(hWnd, SW_HIDE);
			//return 0;
		}
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}

	inline HWND EnsureWindow()
	{
		static HWND s_hWnd = NULL;
		if (::IsWindow(s_hWnd))
			return s_hWnd;

		static bool s_bRegistered = false;
		if (!s_bRegistered)
		{
			WNDCLASS wc = { 0 };
			wc.lpfnWndProc = WndProc;
			wc.hInstance = AfxGetInstanceHandle();
			wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
			wc.lpszClassName = _T("AxLogShowWnd");
			::RegisterClass(&wc);
			s_bRegistered = true;
		}

		s_hWnd = ::CreateWindowEx(WS_EX_TOPMOST, _T("AxLogShowWnd"), _T("axlogShow"),
			WS_POPUPWINDOW | WS_CAPTION | WS_THICKFRAME,
			CW_USEDEFAULT, CW_USEDEFAULT, 700, 350, NULL, NULL, AfxGetInstanceHandle(), NULL);
		if (!s_hWnd)
			return NULL;

		::CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL,
			0, 0, 700, 350, s_hWnd, NULL, AfxGetInstanceHandle(), NULL);

		return s_hWnd;
	}

	inline void Append(LPCTSTR text)
	{
		HWND hWnd = EnsureWindow();
		if (!hWnd) return;

		if (!::IsWindowVisible(hWnd))
			::ShowWindow(hWnd, SW_SHOWNOACTIVATE);	// never steals focus from whatever's in front

		HWND hEdit = ::GetWindow(hWnd, GW_CHILD);
		if (!hEdit) return;

		// cap growth so a long-running screen doesn't slowly balloon this window's memory
		if (::GetWindowTextLength(hEdit) > 200000)
			::SetWindowText(hEdit, _T(""));

		const int len = ::GetWindowTextLength(hEdit);
		::SendMessage(hEdit, EM_SETSEL, (WPARAM)len, (LPARAM)len);
		::SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)(LPCTSTR)text);
		::SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)_T("\r\n"));
		::SendMessage(hEdit, EM_SCROLLCARET, 0, 0);
	}
}

inline void axlogShowImpl(axLogCat cat, const char* func, int line, LPCTSTR fmt, ...)
{
	if (!axLogOn(cat)) return;

	static const char* catNames[] = {
		"INIT", "EVENT", "DATA", "RTM", "SCRIPT", "AXISFORM", "FILEPATCH", "LOGIN", "SOCK_SEND", "SOCK_RECEIVE", "CERTIFY"
	};

	va_list args;
	va_start(args, fmt);
	CString msg;
	msg.FormatV(fmt, args);
	va_end(args);

	CString funcLine;
	funcLine.Format("[%s:%d]", func, line);

	CString line_out;
	line_out.Format("[%s][%-12s] %-42s %s", AXLOG_MODULE_TAG, catNames[cat], (LPCTSTR)funcLine, (LPCTSTR)msg);

	OutputDebugString(line_out + "\n");
	axlogShowDetail::Append(line_out);
}

#define axlogShow(cat, ...) axlogShowImpl(cat, __FUNCTION__, __LINE__, __VA_ARGS__)
