// upload.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "upload.h"
#include "wsock.h"

#include "../h/workio.h"
#include "../h/mapform.H"
#include "../h/mapvar.h"
#include "../awWcc/libwcc.h"
#include "../../h/axisrsm.h"

#define	resC_EXPORT	0x80

// CUpload

IMPLEMENT_DYNAMIC(CUpload, CWnd)

CUpload::CUpload(bool bExport)
{
	m_wsock = NULL;
	m_mapH  = NULL;
	m_done  = true;
	m_offset = 0;
	m_export = bExport;
	m_kind   = W_SRC;
}

CUpload::~CUpload()
{
	if (m_wsock)
	{
		m_wsock->Close();
		delete m_wsock;
		m_wsock = NULL;
	}
}


BEGIN_MESSAGE_MAP(CUpload, CWnd)
	ON_MESSAGE(WM_READDATA, OnReadData)
	ON_MESSAGE(WM_OPENEDSOCK, OnOpenedSock)
END_MESSAGE_MAP()

// CUpload 메시지 처리기입니다.

LRESULT CUpload::OnReadData(WPARAM wParam, LPARAM lParam)
{
	CString	tmps;
	struct	_rsmH*	rsmH = (struct _rsmH *)m_wsock->m_recvB;


	if (rsmH->resK == resK_ERR)
		return 0;

	if (rsmH->resF == resF_ONLY || rsmH->resF == resF_LAS)
	{
		finish();
		return 0;
	}
	sendData();
	return 0;
}

LRESULT CUpload::OnOpenedSock(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
		sendData();
	else
		finish();

	return 0;
}

void CUpload::Upload(CString path, int kind, bool enable, class mapForm *mapForm)
{
	MSG	msg;

	Create(NULL, NULL, WS_CHILD, CRect(0), AfxGetMainWnd(), 2);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE);

	if (!initialize())
	{
		PostMessage(WM_CLOSE);
		return;
	}
	
	if (enable)
		AfxGetMainWnd()->EnableWindow(FALSE);

	m_path = path;
	m_kind = rKIND(kind);
	m_mapH = mapForm;
	m_offset = 0;

	for (m_done = FALSE; !m_done; WaitMessage())
	{
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
	PostMessage(WM_CLOSE);
}

bool CUpload::initialize()
{
	if (m_wsock)
	{
		m_wsock->Close();
		if (m_wsock)
			delete m_wsock;
	}

	m_wsock = new CwSock(this);
	if (!m_wsock->Open())
	{
		MessageBox(_T("Can't open Socket"), _T("ERROR"), MB_OK);
		m_wsock->Close();
		if (m_wsock)
			delete m_wsock;
		m_wsock = NULL;
		return false;
	}

	return true;
}

bool CUpload::sendData()
{
	char	sndb[MAX_IO];
	int	sndl;

	ZeroMemory(sndb, MAX_IO);
	sndl = makeData(sndb);
	if (sndl < 0)
		finish();

	if (!m_wsock->Write(sndb, L_rsmH + L_workIO + sndl))
		return false;

        return true;
}

int CUpload::makeData(char* sndb)
{
	CFile	file;
	ULONGLONG size;
	UINT	readl;
	CString	tmps;
	struct	_rsmH*	rsmH;
	//struct	_rscR*	rscR;
	//struct _workIO* wio;

	rsmH = (struct _rsmH *)sndb;
	//rscR = (struct _rscR *)&sndb[L_rsmH];
	//wio = (struct _workIO *)&sndb[L_rsmH + L_workIO];

	rsmH->dirF = dirF_INB;
	//wio->tgub = GB_MP;
	//CopyMemory(wio->mapn, m_mapH->mapN, min(strlen(m_mapH->mapN), L_MAPN));
	//tmps.Format(_T("%03d"), m_mapH->mapGroup);
	//CopyMemory(wio->mgub, tmps.operator LPCTSTR(), sizeof(wio->mgub)-1);
	//strcpy_s(wio->mapd, sizeof(wio->mapd), m_mapH->mapdesc);

	switch (m_kind)
	{
	case W_SRC:
		rsmH->resK = resK_RSC;
		rsmH->resC = resC_ASCII;
		CopyMemory(rsmH->resN, (char *)m_path.operator LPCTSTR(), min(m_path.GetLength(), sizeof(rsmH->resN)-1));
		wccSetTempPath(tmps, m_mapH->mapN);//m_pSRH->resN);
		break;

	case W_BIN:
		rsmH->resK = resK_RSC;
		rsmH->resC = 0;
		m_path = setMapPath(m_mapH->mapN);
		if (m_export == 1)
			rsmH->resC = resC_EXPORT;
		CopyMemory(rsmH->resN, (char *)m_path.operator LPCTSTR(), min(m_path.GetLength(), sizeof(rsmH->resN)-1));
		wccSetTempPath(tmps, m_mapH->mapN, false);
		break;

	default:
		break;
	}

	if (!file.Open(tmps, CFile::modeRead|CFile::typeBinary))
		return -1;

	size = file.GetLength();
	if (size <= 0)
	{
		file.Close();
		return -1;
	}

	try
	{
		file.Seek(m_offset, CFile::begin);
	}
	catch (CFileException*)
	{
		file.Close();
		return -1;
	}

	readl = file.Read(&sndb[L_rsmH + L_workIO], MAX_IO - L_rsmH - L_workIO);
	file.Close();
	if (readl == 0)
		return 0;

	if (m_offset == 0)
	{
		if (size == readl)
			rsmH->resF = resF_ONLY;
		else
			rsmH->resF = resF_FIR;
	}
	else
	{
		if (size <= (int)(m_offset + readl))
			rsmH->resF = resF_LAS;
		else
		{
			rsmH->resF = resF_MID;
			m_offset += readl;
		}
	}

	tmps.Format(_T("%05d"), readl);
	CopyMemory(rsmH->datL, (char *)tmps.operator LPCSTR(), min(sizeof(rsmH->datL), tmps.GetLength()));
	return readl;
}

void CUpload::finish()
{
	if (m_wsock)
	{
		m_wsock->Close();
		/*delete m_wsock;
		m_wsock = NULL;*/
	}

	if (m_done)
		PostMessage(WM_CLOSE);
	else
	{
		m_done = true;
		if (IsWindow(GetSafeHwnd()))
			PostMessage(WM_USER);
	}
}

CString CUpload::setMapPath(CString mapN)
{
	CString tmps;

	if (mapN.GetLength() != L_MAPN)
		return _T("");

	tmps.Format(_T("map/%s/%s/%s"), mapN.Left(L_SGID), mapN.Left(L_TGID), mapN);
	return tmps;
}
