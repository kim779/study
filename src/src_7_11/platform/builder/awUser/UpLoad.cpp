// UpLoad.cpp : implementation file
//

#include "stdafx.h"
#include "UpLoad.h"
#include "../mapvar.h"
#include "../mainvar.h"
#include "../awSock/wSock.h"
#include "../awWcc/libwcc.h"
#include "../../h/axisvar.h"
#include "../../h/axisrsm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	resC_EXPORT	0x80
/////////////////////////////////////////////////////////////////////////////
// CUpLoad

CUpLoad::CUpLoad()
{
	m_wsock = NULL;
	m_mapH = NULL;
	m_fDone = true;
	m_offset = 0;
	m_bExport = false;
}

CUpLoad::~CUpLoad()
{
	if (m_wsock)
	{
		m_wsock->Close();
		delete m_wsock;
		m_wsock = NULL;
	}
}


BEGIN_MESSAGE_MAP(CUpLoad, CWnd)
	//{{AFX_MSG_MAP(CUpLoad)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_READDATA, OnReadData)
	ON_MESSAGE(WM_OPENEDSOCK, OnOpenedSock)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CUpLoad message handlers

void CUpLoad::upLoad(CString localPath, CString svrPath, int kind, bool bEnable, _mapH *mapH)
{
	MSG msg;
	Create(NULL, NULL, WS_CHILD, CRect(0), AfxGetMainWnd(), 2);

	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE);
	if (!initialize())
	{
		PostMessage(WM_CLOSE);
		return;
	}
	
	if (bEnable)
		AfxGetMainWnd()->EnableWindow(FALSE);
	m_svrPath = svrPath;
	m_localPath = localPath;
	m_kind = kind;
	m_mapH = mapH;
	//sendData();

	for(m_fDone = FALSE; !m_fDone; WaitMessage())
	{
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_USER && msg.hwnd == this->GetSafeHwnd())
				break;

			if (!IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (m_fDone)
			break;
	}

	if (bEnable)
		AfxGetMainWnd()->EnableWindow();
	TRACE("Closed\n");
	PostMessage(WM_CLOSE);
}

LONG CUpLoad::OnReadData(WPARAM wParam, LPARAM lParam)
{
	CString	tmps;

	/*if (m_state != stCONNECTED)
		return 0;*/

	int	datL = wccToNUMBER(m_pRRH->datL, sizeof(m_pRRH->datL));
	if (m_pRRH->resK == resK_ERR)
		return 0;

	if (m_pRRH->resF == resF_ONLY || m_pRRH->resF == resF_LAS)
	{
		finish();
		return 0;
	}
	sendData();
	return 0;
}

LONG CUpLoad::OnOpenedSock(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
		sendData();
	else
		finish();
	return 0;
}

CString CUpLoad::setMapPath(CString mapN)
{
	if (mapN.GetLength() != 8)
		return "";
	CString tmp;
	tmp.Format("map/%s/%s/%s", mapN.Left(2), mapN.Left(3), mapN);
	return tmp;
}

bool CUpLoad::initialize()
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
		MessageBox("Can't open Socket", "ERROR", MB_OK);
		
		m_wsock->Close();
		if (m_wsock)
			delete m_wsock;
		m_wsock = NULL;
		return false;
	}

	m_pSRH = (struct _rsmH *) &m_wsock->m_sendB[0];
	m_pRRH = (struct _rsmH *) &m_wsock->m_recvB[0];

	return true;
}

void CUpLoad::fillResourceList()
{
	CString	tmps, dir;
	int	offs;

	if (m_mapH->imageK == IK_BMP)
	{
		if (m_mapH->imageV[0] != '\0')
		{
			tmps = CString(m_mapH->imageV);
			tmps.TrimRight();
			offs = tmps.ReverseFind('\\');
			if (offs != -1)
			{
				dir.Empty();
				tmps = tmps.Mid(++offs);
				wccSetTempPath(dir, (char *)tmps.operator LPCTSTR());
				if (CopyFile(m_mapH->imageV, tmps, false))
					m_status = "File not found.";
			}
		}
	}

	CFormItem formItem(m_mapH);
	for (int ii = 0; ii < formItem.getFormCount(); ii++)
	{
		if (formItem.getFormPoint(ii)->kind == FM_AVI || (formItem.getFormPoint(ii)->properties & PR_IMAGE))
		{
			tmps = formItem.getFormStr(ii, "str");
			tmps.TrimRight();
			offs = tmps.ReverseFind('\\');
			if (offs != -1)
			{
				dir.Empty();
				wccSetTempPath(dir, (char *)tmps.operator LPCTSTR());
				if (CopyFile(formItem.getFormStr(ii, "str"), dir, false))
					m_status = "File not found!";
			}
		}
	}
}

int CUpLoad::makeData()
{
	DWORD	nRead;
	CString	tmps = _T("");
	CString	resN = _T("");

	ZeroMemory(m_wsock->m_sendB, MAX_IO);
	ZeroMemory(&m_workIO, L_workIO);
	m_pSRH->dirF = dirF_INB;
	m_workIO.tgub = (m_kind==3?GB_TM:GB_MP);
	CopyMemory(&m_workIO.mapn, m_mapH->mapN, min(strlen(m_mapH->mapN), 8));
	sprintf_s(&m_workIO.mgub[0], 3,"%03d", m_mapH->mapGroup);
	strcpy_s(&m_workIO.mapd[0], 20, &m_mapH->mapdesc[0]);
	tmps = _T("");

	switch (m_kind)
	{
	case W_SRC:
		m_pSRH->resK = resK_RSC;
		m_pSRH->resC = resC_ASCII;
		CopyMemory(m_pSRH->resN, (char *) m_svrPath.operator LPCTSTR(), min(m_svrPath.GetLength(), 64));
		wccSetTempPath(tmps, m_mapH->mapN);//m_pSRH->resN);
		break;

	case W_BIN:
		m_pSRH->resK = resK_RSC;
		m_pSRH->resC = 0;
		m_svrPath = setMapPath(m_mapH->mapN);
		if (m_bExport == 1)
			m_pSRH->resC = resC_EXPORT;
		CopyMemory(m_pSRH->resN, (char *) m_svrPath.operator LPCTSTR(), min(m_svrPath.GetLength(), 64));
		wccSetTempPath(tmps, m_mapH->mapN, false);
		break;

	case W_RES:
		m_pSRH->resK = resK_RSC;
		m_pSRH->resC = NULL;
		CopyMemory(m_pSRH->resN, (char *) resN.operator LPCTSTR(), resN.GetLength());
		wccSetTempPath(tmps, m_pSRH->resN);
		break;

	default:
		break;
	}

	m_file = CreateFile(tmps, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (m_file == INVALID_HANDLE_VALUE)
		return -1;

	m_size = GetFileSize(m_file, NULL);
	if (m_size == 0xFFFFFFFF)
	{
		CloseHandle(m_file);
		return -1;
	}

	if (SetFilePointer(m_file, m_offset, NULL, FILE_BEGIN) == 0xFFFFFFFF)
	{
		CloseHandle(m_file);
		return -1;
	}

	if (!ReadFile(m_file, &m_wsock->m_sendB[L_rsmH + L_workIO], MAX_IO - L_rsmH - L_workIO, &nRead, NULL))
	{
		CloseHandle(m_file);
		return -1;
	}
	else
	{
		if (nRead == 0)
		{
			CloseHandle(m_file);
			return 0;
		}
	}

	CopyMemory(&m_wsock->m_sendB[L_rsmH], &m_workIO, L_workIO);
	if (!m_offset)
		m_pSRH->resF = resF_FIR;
	else
	{
		if (m_size <= (int)(m_offset + nRead))
			m_pSRH->resF = resF_LAS;
		else
			m_pSRH->resF = resF_MID;
	}

	m_offset = SetFilePointer(m_file, 0L, NULL, FILE_CURRENT);
	if (m_offset == 0xFFFFFFFF)
		m_offset = 0;
	else
	{
		switch (m_pSRH->resF)
		{
		case resF_FIR:
			if (m_size <= m_offset)
			{
				m_offset = 0;
				m_pSRH->resF = resF_ONLY;
			}
			break;
		case resF_MID:
			break;
		case resF_LAS:
		default:
			m_offset = 0;
			break;
		}
	}	

	CloseHandle(m_file);
	wccToBUFFER(m_pSRH->datL, sizeof(m_pSRH->datL), nRead);

	return nRead;
}

bool CUpLoad::sendData()
{
	int	sendL = 0;

	sendL = makeData();
	if (sendL < 0)
		finish();

	if (!m_wsock->Write(sendL + L_rsmH + L_workIO))
		return false;

        return true;
}


void CUpLoad::finish()
{
	if (m_wsock)
	{
		m_wsock->Close();
		/*delete m_wsock;
		m_wsock = NULL;*/
	}
	if (m_fDone)
		PostMessage(WM_CLOSE);
	else
	{
		m_fDone = true;
		if (IsWindow(GetSafeHwnd()))
			PostMessage(WM_USER);
	}
}
