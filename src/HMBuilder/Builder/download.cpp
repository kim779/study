// download.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "wSock.h"
#include "download.h"
#include "h/mapvar.h"
#include "h/mainvar.h"
#include "h/workio.h"
#include "h/axisrsm.h"

//#include "../awWcc64/libwcc.h"
//#include "../../h/axisvar.h"

IMPLEMENT_DYNCREATE(CDownload, CWnd)

// CDownload
CDownload::CDownload()
{
	m_sock       = NULL;
	m_svrPath    = _T("");
	m_localPath  = _T("");
	m_done       = false;
	m_alive      = false;
	m_kind       = 0;
	m_idno	     = 0;
	m_percentage = 0;

}

CDownload::~CDownload()
{
	if (m_sock)
	{
		m_sock->Close();
		m_sock->DestroyWindow();
		delete m_sock;
	}
}


BEGIN_MESSAGE_MAP(CDownload, CWnd)
	ON_MESSAGE(WM_READDATA, OnReadData)
	ON_MESSAGE(WM_OPENEDSOCK, OnOpenedSock)
//	ON_WM_CREATE()
ON_WM_CREATE()
END_MESSAGE_MAP()

// CDownload 메시지 처리기입니다.

LRESULT CDownload::OnReadData(WPARAM wParam, LPARAM lParam)
{
	CFile	file;
	CString	tmps;
	int	datl = wParam;

	if (datl < L_rsmH)
		return 0;

	struct	_rsmH*	rcvH;
	rcvH = (struct _rsmH *)lParam;
	m_sMsg = CString(rcvH->errmsg);
	TRIM(m_sMsg);

	if (atoi(rcvH->erno) > 0)
	{
//		m_sMsg = CString(rcvH->errmsg);
		TRIM(m_sMsg);
		AfxMessageBox(m_sMsg);
		finish(true);
		return 0;
	}

	if (!m_sMsg.IsEmpty())
		AfxMessageBox(m_sMsg);

	char* pBuffer = (char*)rcvH + L_rsmH;
	
	switch (m_trkind)
	{
	case TRGB_FORMSOURCE:
		pBuffer += L_downform;
		datl -= (L_downform + L_rsmH);
		break;
	case TRGB_FORMSAVE:
		break;
	case TRGB_FORMCHECK:
		break;
	default:
		datl -=	L_rsmH;
		break;
	}
	
	switch (rcvH->resF)
	{
	case resF_FIR:
		setProgress(30);
		if (!file.Open(m_localPath, CFile::modeCreate|CFile::modeWrite))
		{
			AfxMessageBox(_T("download_FIR : File Create Failed"));
			finish();
			return 0;
		}
		else 
			file.Write(pBuffer, datl);	
		file.Close();
		setProgress(40);
		break;

	case resF_MID:
		setProgress(50);
		if (!file.Open(m_localPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		{
			AfxMessageBox(_T("download_MID : File Create Failed"));
			finish();
			return 0;
		}
		file.SeekToEnd();		
		file.Write(pBuffer, datl);
		file.Close();
		setProgress(60);
		break;
		
	case resF_LAS:
		setProgress(80);
		if (!file.Open(m_localPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite))
		{
			AfxMessageBox(_T("download_LAS : File Create Failed"));
			finish();
			return 0;
		}
		file.SeekToEnd();
		file.Write(pBuffer, datl);
		file.Close();
		setProgress(100);
		finish();
		break;
	case resF_ONLY:
		if (!file.Open(m_localPath, CFile::modeCreate|CFile::modeWrite))
			AfxMessageBox(_T("download_ONLY : File Create Failed"));
		else
		{
			file.Write(pBuffer, datl);
			file.Close();
			finish();
		}
		setProgress(100);
		break;
	default:
		finish();
		break;
	}
	
	return 0;
}

LRESULT CDownload::OnOpenedSock(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
	{
		m_alive = true;
	}
	else
	{
		setProgress(100, 3);
		finish(true);
	}
	return 0;
}

bool CDownload::Download(CString usid, CString formname, CString svrPath, CString localPath, int trkind, int kind, int id, bool enable)
{
	MSG	msg;
	bool	first = true;
	setProgress(10, 2);	
	
	//if (enable)
	//	AfxGetMainWnd()->EnableWindow(FALSE);
	m_svrPath	= svrPath;
	m_localPath	= localPath;
	m_formname	= formname;
	m_usid		= usid;
	m_trkind	= trkind;
	m_kind		= kind;
	m_idno		= id;
	
	for (m_done = false; !m_done; WaitMessage())
	{
		if (m_sock->m_alive && first)
		{
			first = false;
			sendData();
			setProgress(20);
		}
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))

		{
			if (msg.message == WM_USER && msg.hwnd == this->GetSafeHwnd())
				break;

			if (GetSafeHwnd() && !IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (m_done)
			break;
	}

	//if (enable)
	//	AfxGetMainWnd()->EnableWindow();

	return true;
}

bool CDownload::initialize()
{
	if (m_sock)
	{
		m_sock->Close();
		m_sock->DestroyWindow();
		delete m_sock;
	}

	m_sock = new CwSock(this);

	if (!m_sock->Open())
	{
		MessageBox(_T("Can't open Socket"), _T("ERROR"), MB_OK);
		m_sock->Close();
		m_sock->DestroyWindow();
		delete m_sock;
		return false;
	}

	//m_rcvH = (struct _rsmH *)m_sock->m_recvB;
	return true;
}

void CDownload::finish(bool close)
{
	if (close && m_sock)
		m_sock->Close();

	m_done = true;
	if (IsWindow(GetSafeHwnd()))
	{
		PostMessage(WM_USER);

		if (m_trkind != TRGB_FORMSOURCE)
			GetParent()->PostMessage(WM_USER, MAKEWPARAM(56, m_trkind));
	}
}


void CDownload::sendData()
{
	CString	tmps;
	int	len = 0;
	char*	sndb = NULL;
	struct _rsmH*	rsmH;

	// 
	CString	gubn = FORMAT("%d", m_kind);
	CString usid = m_usid;
	CString idno = FORMAT("%d", m_idno);
	CString fname = m_formname;		
	m_idno = 0;

	switch (m_trkind)
	{
	case TRGB_FORMALL:
		{
			// map tree
			len = L_rsmH + 1;
			sndb = new char[len];
			FillMemory(sndb, len, ' ');
			rsmH = (struct _rsmH*)sndb;

			rsmH->dirF = dirF_OUTB;
			rsmH->resC = resC_ASCII;
			rsmH->resF = resF_ONLY;
			rsmH->resK = resK_LRSCL;   

			CopyMemory(rsmH->usid, (char*)usid.operator LPCSTR(), min(sizeof(rsmH->usid), usid.GetLength()));
			tmps.Format(_T("0"));
			CopyMemory(rsmH->datL, (char*)tmps.operator LPCSTR(), min(sizeof(rsmH->datL), tmps.GetLength()));

		}
		break;
	case TRGB_TRLAYOUT:
		{
			// trlayout

			len = L_rsmH + 1;
			sndb = new char[len];
			FillMemory(sndb, len, ' ');
			rsmH = (struct _rsmH*)sndb;

			rsmH->dirF = dirF_OUTB;
			rsmH->resC = resC_ASCII;
			rsmH->resF = resF_ONLY;
			rsmH->resK = resk_TRL;

			CopyMemory(rsmH->usid, (char*)usid.operator LPCSTR(), min(sizeof(rsmH->usid), usid.GetLength()));
			tmps.Format(_T("0"));
			CopyMemory(rsmH->datL, (char*)tmps.operator LPCSTR(), min(sizeof(rsmH->datL), tmps.GetLength()));
		}
		break;
	case TRGB_FORMSOURCE:
		{
			// Resource Down
			struct _workIO* wio;
			gubn.Format("%d", m_kind);

			len = L_rsmH + L_workIO + 1;
			sndb = new char[len];
			FillMemory(sndb, len, ' ');

			rsmH = (struct _rsmH*)sndb;
			wio = (struct _workIO*)(sndb + L_rsmH);

			rsmH->dirF = dirF_OUTB;
			rsmH->resC = resC_ASCII;
			rsmH->resF = resF_ONLY;
			rsmH->resK = resK_LRSC;

			CopyMemory(rsmH->usid, (char *)usid.operator LPCSTR(), min(sizeof(rsmH->usid), usid.GetLength()));
			tmps.Format(_T("%08d"), L_workIO);
			CopyMemory(rsmH->datL, (char*)tmps.operator LPCSTR(), min(sizeof(rsmH->datL), tmps.GetLength()));
			CopyMemory(wio->idno, (char*)idno.operator LPCSTR(), min(sizeof(wio->idno), idno.GetLength()));
			CopyMemory(wio->fnam,(char*)fname.operator LPCSTR(), min(sizeof(wio->fnam), fname.GetLength()));
			CopyMemory(wio->gubn, (char*)gubn.operator LPCSTR(), min(sizeof(wio->gubn), gubn.GetLength()));		
		}
		break;
	case TRGB_FORMSAVE:
		break;
	case TRGB_FORMCHECK:
		{
			struct _formcheck* wio;
			len = L_rsmH + L_formcheck + 1;
			sndb = new char[len];
			FillMemory(sndb, len, ' ');

			rsmH = (struct _rsmH *)sndb;
			wio = (struct _formcheck*)(sndb + L_rsmH);

			rsmH->dirF = dirF_OUTB;
			rsmH->resC = resC_ASCII;
			rsmH->resF = resF_ONLY;
			rsmH->resK = resK_CRSC;

			CopyMemory(rsmH->usid, (char *)usid.operator LPCSTR(), min(sizeof(rsmH->usid), usid.GetLength()));
			tmps.Format(_T("%08d"), L_formcheck);
			CopyMemory(rsmH->datL, (char *)tmps.operator LPCSTR(), min(sizeof(rsmH->datL), tmps.GetLength()));
			CopyMemory(wio->idno, (char*)idno.operator LPCSTR(), min(sizeof(wio->idno), idno.GetLength()));
			CopyMemory(wio->fnam, (char*)fname.operator LPCSTR(), min(sizeof(wio->fnam), fname.GetLength()));

		}
		break;
	case TRGB_FORMHISTORY:
		{
			// map history
			struct _formhistorylist* wio;

			len = L_rsmH + L_formhistorylist + 1;
			sndb = new char[len];
			FillMemory(sndb, len, ' ');

			rsmH = (struct _rsmH *)sndb;
			wio = (struct _formhistorylist*)(sndb + L_rsmH);

			rsmH->dirF = dirF_OUTB;
			rsmH->resC = resC_ASCII;
			rsmH->resF = resF_ONLY;
			rsmH->resK = resK_LRSCH;

			CopyMemory(rsmH->usid, (char *)usid.operator LPCSTR(), min(sizeof(rsmH->usid), usid.GetLength()));
			tmps.Format(_T("%08d"), L_formhistorylist);
			CopyMemory(rsmH->datL, (char*)tmps.operator LPCSTR(), min(sizeof(rsmH->datL), tmps.GetLength()));
			CopyMemory(wio->fnam,(char*)m_formname.operator LPCSTR(), min(sizeof(wio->fnam), m_formname.GetLength()));

		}
		break;
	}

	if (sndb != NULL)
	{
		if (!m_sock->Write(sndb, len))
		{
			delete [] sndb;
			return;
		}
		delete [] sndb;
	}
}

void CDownload::setProgress(int percentage, int type )
{
	// nType 1 : download
	//       2 : connect
	//       3 : disconnected
	for ( ;m_percentage <= percentage; m_percentage++)
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_PROGRESS, MAKELPARAM(m_percentage, type));
}

int CDownload::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	initialize();
	return 0;
}
