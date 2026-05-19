// download.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "download.h"
#include "wSock.h"

#include "../h/mapvar.h"
#include "../h/mainvar.h"
#include "../awWcc/libwcc.h"
#include "../h/workio.h"
//#include "../../h/axisvar.h"

// CDownload

IMPLEMENT_DYNAMIC(CDownload, CWnd)

CDownload::CDownload()
{
	m_sock       = NULL;
	m_svrPath    = _T("");
	m_localPath  = _T("");
	m_done       = false;
	m_alive      = false;
	m_kind       = 0;
	m_percentage = 0;
	m_SearchData = NULL;
	m_iSize = 0;
	m_fileType = 'S';

	ZeroMemory(&m_sndH, L_rsmH);
	setHistoryData();
}

CDownload::~CDownload()
{
	if (m_sock)
	{
		m_sock->Close();
		delete m_sock;
		m_sock = NULL;
	}
	if (m_SearchData)
	{
		delete m_SearchData;
		m_SearchData = NULL;
	}
}


BEGIN_MESSAGE_MAP(CDownload, CWnd)
	ON_MESSAGE(WM_READDATA, OnReadData)
	ON_MESSAGE(WM_OPENEDSOCK, OnOpenedSock)
END_MESSAGE_MAP()

// CDownload 메시지 처리기입니다.

LRESULT CDownload::OnReadData(WPARAM wParam, LPARAM lParam)
{
//////////////////////////////////////////////////////
	//datl = wParam;
	//int	rcod = 0;
	CString strErr("");

	//if (datl < L_rsmH)
	//	return 0;

	//struct	_rsmH*	rcvH;
	//rcvH = (struct _rsmH *)lParam;

	if (m_rcvH->resK == resk_HISTORY ||
		m_rcvH->resK == resk_CHKOUT || m_rcvH->resK == resk_SEARCH || m_rcvH->resK == resK_ERR)
	{
		if (m_rcvH->rcod != '0')
		{
			char* pBuffer = (char*)m_rcvH + L_rsmH;
			strErr = _T("Server Error msg:");
			for (size_t ii = 0; ii < 80; ii++)
			{
				strErr.AppendFormat("%c", pBuffer[ii]);
				if (pBuffer[ii] == NULL)
					break;
			}
			if (m_trkind < 1 || m_trkind > 4) MessageBox(strErr, "Socket Download Error", MB_OK | MB_TOPMOST);
			finish(true);
			return 0;
		}
		else
		{
			if (m_rcvH->resK == resk_CHKOUT)
			{
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_CHECKOUT, (LPARAM)m_formname.operator LPCSTR());
				finish(true);
				return 0;
			}
		}
	}

	if (m_rcvH->resK == resK_ERR)
	{
		finish(true);
		return 0;
	}

	//if (rcvH->resK == resk_HISTORY)
	//{
	//	//struct o_loadhistrsc* histrsc;
	//	//histrsc = (struct o_loadhistrsc *)(lParam + L_rsmH);
	//	//if (rcvH->rcod != '0')
	//	//{
	//	//	TRACE("%s\n", histrsc->mesg);
	//	//	return 0;
	//	//}
	//	//else
	//	//{
	//	//	TRACE("success\n");
	//	//}
	//	//AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_BUILDERLOGIN, atoi(&rcvH->rcod)), (LPARAM)strErr.operator LPCSTR());
	//}
	//else if (rcvH->resK == resk_CHKOUT)
	//{
	//	if (rcvH->rcod != '0')
	//	{
	//		char* pBuffer = (char*)rcvH + L_rsmH;
	//		for (size_t ii = 0; ii < 80; ii++)
	//		{
	//			strErr.AppendFormat("%c", pBuffer[ii]);
	//			if (pBuffer[ii] == NULL)
	//				break;
	//		}
	//		return 0;
	//	}
	//	else
	//	{
	//		TRACE("success\n");
	//		return 0;
	//	}
	//}
///////////////////////////////////////////////////////
	CFile	file;
	CString	tmps;
	int	datl;

	datl = atoi(CString(m_rcvH->datL, sizeof(m_rcvH->datL)));
	switch (m_rcvH->resF)
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
			file.Write(&m_sock->m_recvB[L_rsmH], datl);
		file.Close();
		m_sndH.resK = resK_RSP;
		setProgress(40);
		sendData();
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
		file.Write(&m_sock->m_recvB[L_rsmH], datl);
		file.Close();
		m_sndH.resK = resK_RSP;
		setProgress(60);
		sendData();
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
		file.Write(&m_sock->m_recvB[L_rsmH], datl);
		file.Close();
		setProgress(100);
		finish();
		break;
		
	case resF_ONLY:
		if (!file.Open(m_localPath, CFile::modeCreate|CFile::modeWrite))
			AfxMessageBox(_T("download_ONLY : File Create Failed"));
		else
		{
			file.Write(&m_sock->m_recvB[L_rsmH], datl);
			file.Close();
			finish();
		}
		setProgress(100);
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

bool CDownload::Create()
{
	CWnd::Create(NULL, NULL, WS_CHILD, CRect(0), AfxGetMainWnd(), 2);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE);

	if (!initialize())
	{
		PostMessage(WM_CLOSE);
		return false;
	}
	return true;
}
//bool CDownload::Download(CString usid, CString formname, CString svrPath, CString localPath, int trkind, int kind, int id, bool enable)
//bool CDownload::Download(CString svrPath, CString localPath, int kind, bool enable)
bool CDownload::Download(CString usid, CString formname, CString svrPath, CString localPath, int trkind, int kind, int id, bool enable, char fileType)
{
	MSG	msg;
	bool	first = true;

	setProgress(10, 2);	
	if (enable)
		AfxGetMainWnd()->EnableWindow(FALSE);
	m_svrPath = svrPath;
	m_localPath = localPath;
	m_formname = formname;
	m_usid = usid;
	m_trkind = trkind;
	m_kind = kind;
	m_fileType = fileType;
	if (id != -1)
	{
		char tmp[9] = { 0, };

		sprintf_s(tmp, sizeof(tmp), _T("%08d"), id);
		memcpy(m_seqn, tmp, sizeof(m_seqn));
	}

	for (m_done = false; !m_done; WaitMessage())
	{
		if (m_alive && first)
		{
			first = false;
			sendData();
			setProgress(20);
		}

		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_USER && msg.hwnd == this->GetSafeHwnd())
				break;

			if (!IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (m_done)
			break;
	}

	if (enable)
		AfxGetMainWnd()->EnableWindow();
//	PostMessage(WM_CLOSE);
	return true;
}

bool CDownload::initialize()
{
	if (m_sock)
	{
		m_sock->Close();
		if (m_sock)
			delete m_sock;
	}

	m_sock = new CwSock(this);
	if (!m_sock->Open())
	{
		MessageBox(_T("Can't open Socket"), _T("ERROR"), MB_OK);
		
		m_sock->Close();
		if (m_sock)
			delete m_sock;
		m_sock = NULL;
		return false;
	}

	m_rcvH = (struct _rsmH *)m_sock->m_recvB;
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

		if (m_trkind == TRGB_FORMHISTORY || m_trkind == TRGB_FORMSEARCH)
			GetParent()->PostMessage(WM_USER + 132);
	}
}

void CDownload::sendData()
{
	CString	tmps;
	int	len = 0;
	char*	sndb = NULL;
	struct _rsmH*	rsmH;
	struct i_history* historyI;

	CString usid = m_usid;

	m_sndH.dirF = dirF_OUTB;
	CopyMemory(m_sndH.usid, (char *)usid.operator LPCSTR(), min(sizeof(m_sndH.usid), usid.GetLength()));
	if (m_sndH.resK == resK_RSP)
	{
		m_sndH.resC = m_rcvH->resC;
		m_sndH.resF = m_rcvH->resF;
	}

	switch (m_trkind)
	{
	case 1: // template
		if (m_sndH.resK != resK_RSP)
		{
			len = L_rsmH;
			sndb = new char[len];
			FillMemory(sndb, len, ' ');
			rsmH = (struct _rsmH*)sndb;
			CopyMemory(rsmH, &m_sndH, L_rsmH);
			rsmH->resK = resK_SYMB;
		}
		break;
	case 2: // map_info
		len = L_rsmH;
		sndb = new char[len];
		FillMemory(sndb, len, ' ');
		rsmH = (struct _rsmH*)sndb;
		CopyMemory(rsmH, &m_sndH, L_rsmH);
		rsmH->resK = resK_REQ;
		rsmH->resF = resF_ONLY;
		break;
	case 3: // template_info
		len = L_rsmH;
		sndb = new char[len];
		FillMemory(sndb, len, ' ');
		rsmH = (struct _rsmH*)sndb;
		CopyMemory(rsmH, &m_sndH, L_rsmH);
		rsmH->resK = resK_REQ2;
		break;
	case 4: // group_info
		if (m_sndH.resK != resK_RSP)
		{
			len = L_rsmH;
			sndb = new char[len];
			FillMemory(sndb, len, ' ');
			rsmH = (struct _rsmH*)sndb;
			CopyMemory(rsmH, &m_sndH, L_rsmH);
			rsmH->resK = resK_REQ3;
		}
		break;
	case 5: // trlayout_info
		len = L_rsmH;
		sndb = new char[len];
		FillMemory(sndb, len, ' ');
		rsmH = (struct _rsmH*)sndb;
		CopyMemory(rsmH, &m_sndH, L_rsmH);
		rsmH->resK = resk_REQT;
		rsmH->resF = resF_ONLY;
		break;
	case TRGB_FORMCHECK:
		len = L_rsmH + 1;
		sndb = new char[len];
		FillMemory(sndb, len, ' ');
		rsmH = (struct _rsmH*)sndb;
		CopyMemory(rsmH, &m_sndH, L_rsmH);
		rsmH->resK = resk_CHKOUT;
		sndb[L_rsmH] = 'S';
		CopyMemory(rsmH->resN, (char *)m_formname.operator LPCTSTR(), min(m_formname.GetLength(), 128));
		break;
	case TRGB_FORMHISTORY:
		len = L_rsmH + L_history;
		sndb = new char[len];
		FillMemory(sndb, len, ' ');
		rsmH = (struct _rsmH*)sndb;
		CopyMemory(rsmH, &m_sndH, L_rsmH);
		rsmH->resK = resk_HISTORY;
		CopyMemory(rsmH->resN, (char *)m_formname.operator LPCTSTR(), min(m_formname.GetLength(), 128));
		historyI = (struct i_history*)&sndb[L_rsmH];
		historyI->rscType = m_Type;
		historyI->dirt = m_dirt;
		CopyMemory(historyI->seqn, m_seqn, sizeof(historyI->seqn));
		break;
	case TRGB_FORMSEARCH:
		len = L_rsmH + 1 + (L_condition * m_iSize);
		sndb = new char[len];
		FillMemory(sndb, len, ' ');
		rsmH = (struct _rsmH*)sndb;
		CopyMemory(rsmH, &m_sndH, L_rsmH);
		rsmH->resK = resk_SEARCH;
		rsmH->resF = resF_ONLY;
		sndb[L_rsmH] = 'S';
		memcpy(&sndb[L_rsmH + 1], m_SearchData, L_condition * m_iSize);
		break;
	default:
		if (m_sndH.resK != resK_RSP)
		{
			len = L_rsmH + 1 + sizeof(m_seqn);
			sndb = new char[len];
			FillMemory(sndb, len, ' ');
			rsmH = (struct _rsmH*)sndb;
			CopyMemory(rsmH, &m_sndH, L_rsmH);
			rsmH->resK = resK_RSC;
			rsmH->mode = (char)m_kind;
			sndb[L_rsmH] = m_fileType;
			CopyMemory(&sndb[L_rsmH + 1], m_seqn, sizeof(m_seqn));
		}
		break;
	}

	if (m_trkind <= 1 || m_trkind == 4)
		CopyMemory(rsmH->resN, (char *)m_svrPath.operator LPCTSTR(), min(m_svrPath.GetLength(), 128));

	FillMemory(rsmH->datL, sizeof(rsmH->datL), '0');
	CopyMemory(&m_sndH, rsmH, L_rsmH);
	if (!m_sock->Write((char *)rsmH, len))
		return;
}

void CDownload::setProgress(int percentage, int type )
{
	// nType 1 : download
	//       2 : connect
	//       3 : disconnected
	for ( ;m_percentage <= percentage; m_percentage++)
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_PROGRESS, MAKELPARAM(m_percentage, type));
}

void CDownload::setHistoryData(char Type, char dirt, char* seqn)
{
	m_Type = Type;
	m_dirt = dirt;
	FillMemory(m_seqn, sizeof(m_seqn), ' ');
	if (seqn != NULL)
	{ 
		char tmp[9] = { 0, };

		sprintf_s(tmp, sizeof(tmp), _T("%08d"), atoi(seqn));
		memcpy(m_seqn, tmp, sizeof(m_seqn));
	}
}

void CDownload::setSearchData(const CStringArray& arr)
{
	if (m_SearchData)
	{
		delete m_SearchData;
		m_SearchData = NULL;
	}
	m_iSize = arr.GetSize();
	m_SearchData = (char*)malloc(L_condition * m_iSize);
	FillMemory(m_SearchData, L_condition * m_iSize, ' ');
	CString tmp;
	for (int nIndex = 0; nIndex < m_iSize; nIndex++)
	{
		tmp = arr.GetAt(nIndex);
		memcpy(&m_SearchData[L_condition*nIndex], tmp, tmp.GetLength());
	}
}