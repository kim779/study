// Cast.cpp : implementation file
//

#include "stdafx.h"
#include "Wizard.h"
#include "Cast.h"
#include "Guard.h"

#include "../h/axisfire.h"
#include "../h/axisanm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BEGIN_MESSAGE_MAP(CCast, CWnd)
	//{{AFX_MSG_MAP(CCast)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MAPDATA, OnQuote)
END_MESSAGE_MAP()

CCast::CCast(CGuard* guard, CWnd* pWnd, char* home)
{
	m_guard   = guard;
	m_wizard  = pWnd;

	m_smap    = NULL;
	m_msgwnd  = (CWnd *) 0;

	m_running = false;
	m_home    = home;
}

CCast::~CCast()
{
	if (IsWindow(m_smap->m_shm->hwnd[toCast]))
	{
		::SendMessage(m_smap->m_shm->hwnd[toCast], WM_DESTROY, 0, 0);
		WaitForSingleObject(m_smap->m_shm->hwnd[toCast], 3000);
		if (IsWindow(m_smap->m_shm->hwnd[toCast]))
			::PostMessage(m_smap->m_shm->hwnd[toCast], WM_CLOSE, 0, 0);
	}

	if (m_smap != NULL)
	{
		m_smap->endMapping();
		delete m_smap;
	}

	DestroyWindow();
}

LRESULT CCast::OnQuote(WPARAM wparam, LPARAM lparam)
{
	m_sync.Lock();

	int readC = m_smap->readCount();
//TRACE("read count = [%d]\n", readC);
	if (m_guard->GetDataCount() > 0)
	{
		if (readC < maxQUE/2)
		{
			m_sync.Unlock();
			return 0;
		}
	}

	bool	trade;
	CString	code;
	CWordArray	rtmk;
	CMapStringToOb	quote;		// parsed data for MAP
	CMapStringToOb	scroll;	
	POSITION	pos;

	CdataSet*	rts;
	CObArray*	obs;
	char*		pBytes;

	quote.RemoveAll();
	scroll.RemoveAll();
	for (int ii = 0; ii < readC; ii++)
	{
		if (!m_smap->readData(code, &rtmk, pBytes, trade))
			continue;

		// process previous data
		if (trade)
		{
			bool	doing = true;

			if (scroll.Lookup(code, (CObject *&)obs))
			{
				if (obs->GetSize() > 0)
					doing = false;
			}

			if (doing && quote.Lookup(code, (CObject *&)rts))
			{
				if (rts->IsOn())
					m_guard->DoRTM(code, 0, rts, NULL);
				rts->SetOn(false);
			}
		}
		else
		{
			if (scroll.Lookup(code, (CObject *&)obs) && obs->GetSize() > 0)
			{
				if (!quote.Lookup(code, (CObject *&)rts))
					rts = NULL;
				m_guard->DoRTM(code, stat_SCR, rts, obs);
				
				rts->SetOn(false);
				for (int ii = 0; ii < obs->GetSize(); ii++)
					delete obs->GetAt(ii);
				obs->RemoveAll();
			}
		}

		if (trade)
		{
			int	index;
			CdataSet* fms;
			char	*ptr = NULL;
			char	*token, *datb = pBytes;

			if (!scroll.Lookup(code, (CObject *&)obs))
			{
				obs = new CObArray;
				obs->RemoveAll();
			}
			if (!quote.Lookup(code, (CObject *&)fms))
				fms = new CdataSet;

			rts = new CdataSet;
			token = m_guard->strtokx(datb, '\t', &ptr);
			for (int ii = 0; token && ii < rtmk.GetSize(); ii++)
			{
				index = rtmk.GetAt(ii);
				if (!rts->IsOn())
				{
					rts->SetOn();
					fms->SetOn();
				}
				rts->SetAt(index, token);
				fms->SetAt(index, token);
				token = m_guard->strtokx(NULL, '\t', &ptr);
			}

			if (rts->IsOn())
			{
				if (obs->GetSize() > 0)
					obs->InsertAt(0, rts);
				else
					obs->Add(rts);
			}
			else	delete rts;

			quote.SetAt(code, fms);
			scroll.SetAt(code, obs);
		}
		else
		{
			char	*ptr = NULL;
			char	*token, *datb = pBytes;

			if (!quote.Lookup(code, (CObject *&)rts))
				rts = new CdataSet;

			token = m_guard->strtokx(datb, '\t', &ptr);
			for (int ii = 0; token && ii < rtmk.GetSize(); ii++)
			{
				if (!rts->IsOn())
					rts->SetOn();
				rts->SetAt(rtmk.GetAt(ii), token);
				token = m_guard->strtokx(NULL, '\t', &ptr);
			}
			quote.SetAt(code, rts);
		}
	}

	for (pos = scroll.GetStartPosition(); pos; )
	{
		scroll.GetNextAssoc(pos, code, (CObject *&)obs);
		if (obs->GetSize() > 0)
		{
			if (!quote.Lookup(code, (CObject *&)rts))
				rts = NULL;
			m_guard->DoRTM(code, stat_SCR, rts, obs);
			for (int ii = 0; ii < obs->GetSize(); ii++)
				delete obs->GetAt(ii);
			if (rts)
			{
				delete rts;
				quote.RemoveKey(code);
			}
		}
		delete obs;
	}
	scroll.RemoveAll();

	for (pos = quote.GetStartPosition(); pos; )
	{
		quote.GetNextAssoc(pos, code, (CObject *&)rts);
		if (rts->IsOn())
			m_guard->DoRTM(code, 0, rts, NULL);
		delete rts;
	}
	quote.RemoveAll();

	m_sync.Unlock();

	return 0;
}

long checkDispatch(LPVOID lpvoid)
{
	CCast* cast = (CCast *) lpvoid;

	while (cast->m_running)
	{
		if (cast->m_smap->readCount() > 0)
			cast->OnQuote(0, 0);

		Sleep(50);
	}

	return 0;
}

void CCast::LogMsg(DWORD tick, CString data, bool trade)
{
	static int count = 0;

	if (count >= 100) return;

	FILE* debug;
	errno_t	err;
	SYSTEMTIME stime;

	err = fopen_s(&debug, "c:\\debugX2.txt", "a+");
	if (err != 0)
		return;

	GetLocalTime(&stime);
	fprintf(debug, "[%02d:%02d:%02d.%03d][%02d] = tick[%ld], data[%d][%s]\n",
			stime.wHour, stime.wMinute, stime.wSecond, stime.wMilliseconds, count++, tick, trade ? 1 : 0, data);
	fflush(debug);
	fclose(debug);
}

bool CCast::InitialCast()
{
	if (!IsWindow(this->m_hWnd))
	{
		LPCTSTR pstrOwnerClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW, ::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH) ::GetStockObject(WHITE_BRUSH),	AfxGetApp()->LoadStandardIcon(IDI_APPLICATION));
		if (!CWnd::CreateEx(0, pstrOwnerClass, _T(""), WS_POPUP, -1024, -1024, 10, 10, NULL, 0))
			return false;
		SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, WS_EX_TOOLWINDOW);
	}
	
	if (!m_smap)
	{
		m_smap = new CMapping();
		m_smap->initMapping(); m_smap->setHwnd(0, this->m_hWnd);
	}

	m_smap->killProcess();

	CString	argv; argv.Format("%s /p %s", "exeCast", m_home);
	int resp = m_smap->runProcess(m_home, "axiscastx.exe");
	if (resp)
	{
		TRACE("axiscastx runProcess error..[%d]\n", resp);
		return false;
	}

	m_running = true;
//	AfxBeginThread((AFX_THREADPROC)checkDispatch, (LPVOID)this, THREAD_PRIORITY_HIGHEST);
	return true;
}

void CCast::RegisterRTM(int major, int minor, CString codes)
{
	int idx;
	CString code, codelist = _T("");

	m_smap->m_shm->actF[0] = axDetach;
	m_smap->m_shm->actF[1] = major;
	m_smap->m_shm->actF[2] = minor;
	m_smap->sendMsg(toCast);

	while (!codes.IsEmpty())
	{
		if ((idx = codes.Find('\t')) < 0)
		{
			code  = codes;
			codes = _T("");
		}
		else
		{
			code  = codes.Left(idx);
			codes = codes.Mid(idx+1);
		}

		codelist += code + "|";
	}

	m_smap->m_shm->actF[0] = axAttach;
	m_smap->m_shm->actF[1] = major;
	m_smap->m_shm->actF[2] = minor;
	strcpy_s(m_smap->m_shm->actB, sizeof(m_smap->m_shm->actB), (char*)codelist.operator LPCTSTR());
	m_smap->sendMsg(toCast);
}

void CCast::ReloadFMT(CString home)
{
	if (!m_running) return;

	m_smap->m_shm->actF[0] = axReload;
	strcpy_s(m_smap->m_shm->actB, sizeof(m_smap->m_shm->actB), (char*)home.operator LPCTSTR());
	m_smap->sendMsg(toCast);
}

void CCast::FilterRTM(CString filter)
{
	if (!m_running) return;

	m_smap->m_shm->actF[0] = axFilter;
	strcpy_s(m_smap->m_shm->actB, sizeof(m_smap->m_shm->actB), (char*)filter.operator LPCTSTR());
	m_smap->sendMsg(toCast);
}

void CCast::SetInterval(int interval)
{
	if (!m_running)	return;

	CString	text;
	m_smap->m_shm->actF[0] = axInterval;
	text.Format("%d", interval);
	strcpy_s(m_smap->m_shm->actB, sizeof(m_smap->m_shm->actB), (char*)text.operator LPCTSTR());
	m_smap->sendMsg(toCast);
}

void CCast::ExitCast()
{
	if (!m_running) return;

	m_smap->m_shm->actF[0] = axExit;
	m_smap->sendMsg(toCast);
}

bool CCast::IsFoptionCode(CString code)
{
	int codeL = code.GetLength();
	if (codeL <= 0) return false;

	switch (codeL)
	{
	case 3:
	case 6:
		break;
	default:
		if (codeL < 2)	break;

		switch (atoi(code.Left(2)))
		{
		case 10:
		case 40:
			return true;
		case 20:
		case 30:
			return true;
		}
		break;
	}

	return false;
}