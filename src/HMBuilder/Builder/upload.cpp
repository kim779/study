// upload.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "upload.h"
#include "wsock.h"

#include "h/workio.h"
#include "h/mapform.H"
#include "h/mapvar.h"
#include "h/axisrsm.h"
#include "amCc/LibBuild.h"

#define	resC_EXPORT	0x80

// CUpload

CUpload::CUpload(bool bExport)
{
	m_wsock = NULL;
	m_mapH  = NULL;
	m_done  = true;
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
	struct _rsmH* prsm = (struct _rsmH*)lParam;

	if (atoi(prsm->erno) > 0)
	{
		finish();
		return 0;
	}

	finish();
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

void CUpload::FormUploadSetting(CString strFormInfo)
{
	int iFind = strFormInfo.Find("/");
	m_gubn = atoi(strFormInfo.Left(iFind++));
	strFormInfo = strFormInfo.Mid(iFind);
	
	iFind = strFormInfo.Find("/");
	m_ver1 = strFormInfo.Left(iFind++);
	strFormInfo = strFormInfo.Mid(iFind);

	iFind = strFormInfo.Find("/");
	m_ver2 = strFormInfo.Left(iFind++);
	strFormInfo = strFormInfo.Mid(iFind);

	iFind = strFormInfo.Find("/");
	m_desc = strFormInfo.Left(iFind++);
}

void CUpload::Upload(CString usid, CString path, int trkind, int kind, bool enable, struct _mapH *mapH)
{
	MSG	msg;

	Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), AfxGetMainWnd(), 2);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE);

	if (!initialize())
	{
		PostMessage(WM_CLOSE);
		return;
	}
	
	if (enable)
		AfxGetMainWnd()->EnableWindow(FALSE);

	m_mapH = mapH;
	m_path = path;
	m_kind = rKIND(trkind);
//	m_trkind = kind;
	m_mapH = mapH;
	m_usid = usid;	
	//m_gubn  = kind;

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
	if (_sendData() == -1)
	{
//		finish();	
		return false;
	}
//	finish();	
        return true;
}

int CUpload::_sendData()
{
	CFile	file;
	ULONGLONG size;
	UINT	readl;
	CString	tmps, path, fname, desc;
	CString sRoot, ss;
//	int	len;
	struct	_rsmH*	rsmH;
	struct	_saveform* wio;
//	char	sndb[MAX_IO];
	char*	sndb;

	int	max_size = MAX_IO - L_rsmH - L_saveform;
	int	idx = 0;
	fname = CString(m_mapH->mapN);
	desc = CString(m_mapH->mapdesc);
	sndb = new char[MAX_IO];
	TRIM(fname);
	TRIM(desc);


	rsmH = (struct _rsmH*)sndb;
	ZeroMemory(rsmH, L_rsmH);
	
	wio  = (struct _saveform*)&sndb[L_rsmH];
	rsmH->dirF = dirF_INB;
	ZeroMemory(wio, L_saveform);

	switch (m_kind)
	{
	case W_SRC:
		rsmH->resK = resK_SRSC;
		rsmH->resC = resC_ASCII;
		rsmH->resF = resF_ONLY;
		wio->type[0] = 'S';
		fname += ".XML";
		sRoot = CString(m_mapH->fullN); TRIM(sRoot);
		path.Format("%s\\exe\\tmp\\%s", m_path, fname);
		break;

	case W_BIN:
		rsmH->resK = resK_SRSC;
		rsmH->resC = resC_ASCII;
		rsmH->resF = resF_ONLY;
		wio->type[0] = 'B';
		idx = fname.Find(".XML");	
		if (idx != -1)
			fname = fname.Left(idx);
		path.Format("%s\\exe\\tmp\\%s", m_path, fname);	
		break;

	default:
		break;
	}

	CopyMemory(rsmH->usid, m_usid, min(m_usid.GetLength(), sizeof(rsmH->usid)));
	CopyMemory(wio->fnam, fname, min(fname.GetLength(), sizeof(wio->fnam)));
	CopyMemory(wio->desc, desc, min(desc.GetLength(), sizeof(wio->desc)));

	tmps.Format("0");
	CopyMemory(wio->idno,  (char *)tmps.operator LPCTSTR(), min(tmps.GetLength(), sizeof(wio->type)));	  
	CopyMemory(wio->ver1,  (char *)m_ver1.operator LPCTSTR(), min(tmps.GetLength(), sizeof(wio->ver1)));
	CopyMemory(wio->ver2,  (char *)m_ver2.operator LPCTSTR(), min(tmps.GetLength(), sizeof(wio->ver2)));
	tmps.Format("%d", m_gubn);
	CopyMemory(wio->gubn,  (char *)tmps.operator LPCTSTR(), min(tmps.GetLength(), sizeof(wio->gubn)));
	CopyMemory(wio->mdsc, (char *)m_desc.operator LPCTSTR(), min(m_desc.GetLength(), sizeof(wio->mdsc)));

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return -1;

	size = file.GetLength();
	if (size <= 0)
	{
		file.Close();
		return -1;
	}

	for(int offset = 0; true; )
	{
		readl = file.Read(&sndb[L_rsmH + L_saveform], max_size);
		if (offset == 0)
		{
			if (size == readl)
				rsmH->resF = resF_ONLY;
			else
			{
				rsmH->resF = resF_FIR;
				offset += readl;
			}
		}
		else
		{
			if (size <= (int)(offset + readl))
			{
				offset = 0;
				rsmH->resF = resF_LAS;
			}
			else
			{
				rsmH->resF = resF_MID;
				offset += readl;
			}
		}

		tmps.Format(_T("%08d"), L_saveform + readl);
		CopyMemory(rsmH->datL, (char *)tmps.operator LPCSTR(), min(sizeof(rsmH->datL), tmps.GetLength()));
		tmps.Format(_T("%08d"), readl);
		CopyMemory(wio->datl, (char *)tmps.operator LPCSTR(), min(sizeof(wio->datl), tmps.GetLength()));

		if (!m_wsock->Write(sndb, L_rsmH + L_saveform + readl))
		{
			file.Close();
			return -1;
		}
		if (offset == 0)
			break;

		try
		{
			file.Seek(offset, CFile::begin);
		}
		catch (CFileException*)
		{
			file.Close();
			return -1;
		}

	}
	file.Close();

	delete [] sndb;
	return 0;
}

void CUpload::finish()
{
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
