// DownLoad.cpp: implementation of the CDownLoad class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../axisWork.h"
#include "DownLoad.h"
#include "../mapvar.h"
#include "../mainvar.h"
#include "../awSock/wSock.h"
#include "../awWcc/libwcc.h"
#include "../../h/axisvar.h"
#include "../../h/axisrsm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDownLoad::CDownLoad()
{
	m_wsock = NULL;
	m_nPercentage = 0;
}

CDownLoad::~CDownLoad()
{
	if (m_wsock)
	{
		m_wsock->Close();
		delete m_wsock;
		m_wsock = NULL;
	}
}

BEGIN_MESSAGE_MAP(CDownLoad, CWnd)
	//{{AFX_MSG_MAP(CDownLoad)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_READDATA, OnReadData)
	ON_MESSAGE(WM_OPENEDSOCK, OnOpenedSock)
END_MESSAGE_MAP()

LONG CDownLoad::OnReadData(WPARAM wParam, LPARAM lParam)
{
	CString	tmps;
	int	datL;

	tmps = CString(m_RRH->datL, sizeof(m_RRH->datL));
	datL = atoi(tmps);
	if (m_RRH->resK == resK_ERR)
	{
		//MessageBox(&m_wsock->m_recvB[L_rsmH], "ERROR !!", MB_OK);
		finish();
		return 0;
	}

	tmps = _T("");
	switch (m_RRH->resF)
	{
	case resF_FIR:
		SetProgress(30);
		if (!m_file.Open(m_localPath, CFile::modeCreate|CFile::modeWrite))
		{
			AfxMessageBox("File Create Failed");
			finish();
			return 0;
		}
		else 
		{
			m_file.Write(&m_wsock->m_recvB[L_rsmH], datL);
		}
		m_SRH->resK = resK_RSP;
		SetProgress(40);
		sendData();
		break;
	case resF_MID:
		SetProgress(50);
		m_file.Write(&m_wsock->m_recvB[L_rsmH], datL);
		m_SRH->resK = resK_RSP;
		SetProgress(60);
		sendData();
		break;
		
	case resF_LAS:
		SetProgress(80);
		m_file.Write(&m_wsock->m_recvB[L_rsmH], datL);
		m_file.Close();
		SetProgress(100);
		finish();
		break;
		
	case resF_ONLY:
		{
			if (!m_file.Open(m_localPath, CFile::modeCreate|CFile::modeWrite))
				AfxMessageBox("File Create Fail");
			else
			{
				m_file.Write(&m_wsock->m_recvB[L_rsmH], datL);
				m_file.Close();
				finish();
			}
			SetProgress(100);
		}
		break;
	}
	
	return 0;
}

LONG CDownLoad::OnOpenedSock(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
	{
		sendData();
		SetProgress(20);
	}
	else
	{
		m_bResult = false;
		SetProgress(100, 3);
		finish();
	}
	return 0;
}

bool CDownLoad::downLoad(CString svrPath, CString localPath, int kind, bool bEnable)
{
	MSG msg;
	Create(NULL, NULL, WS_CHILD, CRect(0), AfxGetMainWnd(), 2);

	m_bResult = true;
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOACTIVATE);
	if (!initialize())
	{
		m_bResult = false;
		PostMessage(WM_CLOSE);
		return false;
	}

	SetProgress(10, 2);	
	
	if (bEnable)
		AfxGetMainWnd()->EnableWindow(FALSE);
	m_svrPath = svrPath;
	m_localPath = localPath;
	m_kind = kind;
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
	PostMessage(WM_CLOSE);
	return m_bResult;
}

bool CDownLoad::initialize()
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

	m_SRH = (struct _rsmH *) &m_wsock->m_sendB[0];
	m_RRH = (struct _rsmH *) &m_wsock->m_recvB[0];

	return true;
}

void CDownLoad::sendData()
{
	m_SRH->dirF = dirF_OUTB;

	if (m_SRH->resK == resK_RSP)
	{
		m_SRH->resC = m_RRH->resC;
		m_SRH->resF = m_RRH->resF;
	}
	
	switch(m_kind)
	{
	case 1: // template
		if (m_SRH->resK != resK_RSP)
			m_SRH->resK = resK_SYMB;
		break;
	case 2: // map_info
		m_SRH->resK = resK_REQ;
		break;
	case 3: // template_info
		m_SRH->resK = resK_REQ2;
		break;
	case 4: // group_info
		if (m_SRH->resK != resK_RSP)
			m_SRH->resK = resK_REQ3;
		break;
	default:
	case 0:
		if (m_SRH->resK != resK_RSP)
			m_SRH->resK = resK_RSC;
		break;
	}

	if (m_kind <= 1 || m_kind == 4)
		CopyMemory(m_SRH->resN, (char *)m_svrPath.operator LPCTSTR(), min(m_svrPath.GetLength(), 64));
	
	wccToBUFFER(m_SRH->datL, sizeof(m_SRH->datL), 0);

	if (!m_wsock->Write(L_rsmH))
		return;
}

void CDownLoad::finish()
{
	if (m_wsock)
	{
		m_wsock->Close();
		/*delete m_wsock;
		m_wsock = NULL;*/
	}
	m_fDone = true;
	if (IsWindow(GetSafeHwnd()))
		PostMessage(WM_USER);
}

void CDownLoad::SetProgress(int nPercentage, int nType)
{
	// nType 1 : download, 2 : Connect, 3:Disconnected
	for (;m_nPercentage <= nPercentage; m_nPercentage++)
		AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_PROGRESS, MAKELPARAM(m_nPercentage, nType));
}



