// transDlg.cpp : implementation file
//

#include "stdafx.h"
#include "amRm.h"
#include "transDlg.h"

#include "../mSock/wSock.h"
#include "../h/axisvar.h"
#include "../h/axisrsm.h"
#include "../h/axisfire.h"
#include "../h/workio.h"
#include "../h/mapvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransDlg dialog

CTransDlg::CTransDlg(CWnd* pParent, int kind, CString id, CString desc)
	: CDialog(CTransDlg::IDD, pParent)
{
	m_kind = kind;
	m_id.Format("%s", id);
	m_desc.Format("%s", desc);

	m_files.RemoveAll();

	m_state  = sNONE;
	m_socket = (class CwSock *) NULL;

	m_finish = false;
	m_index  = 0;
	m_offset = 0;

	m_sendB  = (char *) NULL;
	m_recvB  = (char *) NULL;

	//{{AFX_DATA_INIT(CTransDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTransDlg::~CTransDlg()
{
	if (m_socket)	delete m_socket;
	if (m_sendB)	delete[] m_sendB;
	if (m_recvB)	delete[] m_recvB;
}

void CTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_ADDR, m_addr);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTransDlg, CDialog)
	//{{AFX_MSG_MAP(CTransDlg)
	ON_BN_CLICKED(IDOK, OnTransmit)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_ADDR, OnAddrFieldChanged)
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_OPENEDSOCK, OnSocketSession)
	ON_MESSAGE(WM_READDATA, OnSocketData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransDlg message handlers

BOOL CTransDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_sendB = new char[szMAX];
	m_recvB = new char[szMAX];

	m_SRH   = (struct _rsmH *)&m_sendB[0];
	m_RRH   = (struct _rsmH *)&m_recvB[0];
	m_saveF = (struct _saveform *)&m_sendB[L_rsmH];

	m_socket = new CwSock(this); m_state = sCREATE;

	int	nLen;
	TCHAR	wb[1024]; GetCurrentDirectory(sizeof(wb), wb);
	CString	path; path.Format("%s/%s", wb, "workshop.ini");

	nLen = GetPrivateProfileString("SERVER", "IP", _T(""), wb, sizeof(wb), path);
//	CopyMemory(m_ips, wb, nLen); m_ips[nLen] = 0x00;

	nLen = GetPrivateProfileString("SERVER", "PORT", _T(""), wb, sizeof(wb), path);
	m_port = atoi(CString(wb, nLen));


	GetDlgItem(IDOK)->EnableWindow(FALSE);
	for (int ii = 0; ii < 4; ii++)
		m_addr.SetFieldRange(ii, 0, 255);
	m_addr.SetFocus();
	m_addr.SetFieldFocus(0);

	m_progress.SetRange(0, 100);
	m_progress.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTransDlg::OnTransmit() 
{
	if (m_finish)
	{
		CDialog::OnOK();
		return;
	}

	BOOL	rc;
	BYTE	addr0, addr1, addr2, addr3;

	m_addr.GetAddress(addr0, addr1, addr2, addr3);
	m_ips.Format("%d.%d.%d.%d", addr0, addr1, addr2, addr3);
	switch (m_state)
	{
	case sCREATE:
		enableSend(false);
		rc = m_socket->Open(m_ips, m_port);
		if (!rc)
		{
			enableSend(true);
			AfxMessageBox("Fail to Connect!", MB_OK|MB_ICONEXCLAMATION);
			return;
		}
		break;

	case sOPEN:
		if (!sendData())
		{
			enableSend(true);
			AfxMessageBox("Fail to Transmit Data.", MB_OK|MB_ICONEXCLAMATION);
			return;
		}
		break;

	default:
		AfxMessageBox("Fail to Connect.", MB_OK|MB_ICONEXCLAMATION);
		break;
	}
}

void CTransDlg::OnCancel() 
{
	close();	
	CDialog::OnCancel();
}

void CTransDlg::OnAddrFieldChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMIPADDRESS* pIpAddress = (NMIPADDRESS *)pNMHDR;
	*pResult = 0;

	GetDlgItem(IDOK)->EnableWindow(!m_addr.IsBlank());
}

LRESULT CTransDlg::OnSocketSession(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case FEV_OPEN:
		if (lParam)
		{
			m_state = sOPEN;
			OnTransmit();
			return 0;
		}
		break;
	case FEV_CLOSE:
		close();
		break;
	}

	return 0;
}

LRESULT CTransDlg::OnSocketData(WPARAM wParam, LPARAM lParam)
{
	if (m_state != sOPEN)
		return 0;

	int	nBytes = LOWORD(wParam);
	char*	pBytes = (char*)lParam;

	CopyMemory(m_recvB, pBytes, nBytes);
	int datL = atoi(CString(m_RRH->datL, sizeof(m_RRH->datL)));
	if (toInt(m_RRH->erno, sizeof(m_RRH->erno)) != 0)
	{
		GetDlgItem(IDC_GUIDE)->SetWindowText(CString(m_RRH->errmsg, sizeof(m_RRH->errmsg)));
		close(); return 0;
	}

	if (m_RRH->resF == resF_ONLY || m_RRH->resF == resF_LAS)
	{
		if (m_index == m_files.GetUpperBound())
		{
			close();
			m_finish = true;
			GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_GUIDE)->SetWindowText("Finish");
			GetDlgItem(IDOK)->SetWindowText("Close");
			updateProgress(m_size);
			return 0;
		}
		else
		{
			m_index++;
			enableSend(false);
		}
	}

	if (!sendData())
		enableSend(true);
	return 0;
}

bool CTransDlg::sendData()
{
	int	sendL;

	sendL = makeData();
	if (sendL < 0)
	{
		OnCancel();
		return false;
	}

	TRACE("fnam = [%s][%c] flag = [%d]\n", m_saveF->fnam, m_saveF->type[0], m_SRH->resF);

	if (!m_socket->Write(m_sendB, sendL))
		return false;

	updateProgress(m_offset);

	if (m_SRH->resF == resF_FIR || m_SRH->resF == resF_MID)
	{
		Sleep(10); 
		if (!sendData()) enableSend(true);
	}

        return true;
}

int CTransDlg::makeData()
{
	DWORD	nRead;
	CString	tmps, tmpx;

	ZeroMemory(m_sendB, szMAX);
	m_SRH->dirF = dirF_INB;
	m_SRH->resK = resK_SRSC;
	m_SRH->resC = resC_ASCII;
	CopyMemory(m_SRH->usid, (char*) m_id.operator LPCSTR(), min(sizeof(m_SRH->usid), m_id.GetLength()));

	tmps = m_files.GetAt(m_index);
	if (!m_file.Open(tmps, CFile::modeRead|CFile::typeBinary))
		return -1;

	m_size = (int)m_file.GetLength();
	if (m_size <= 0)
	{
		m_file.Close();
		return -1;
	}

	m_file.Seek(m_offset, CFile::begin);
	nRead = m_file.Read(&m_sendB[L_rsmH+L_saveform], szMAX-(L_rsmH+L_saveform));
	if (nRead <= 0)
	{
		m_file.Close();
		return 0;
	}

	if (!m_offset)
		m_SRH->resF = resF_FIR;
	else
	{
		if (m_size <= (int)(m_offset + nRead))
			m_SRH->resF = resF_LAS;
		else
			m_SRH->resF = resF_MID;
	}

	m_offset = (int)m_file.Seek(0, CFile::current);
	if (m_offset <= 0)
		m_offset = 0;
	else
	{
		switch (m_SRH->resF)
		{
		case resF_FIR:
			if (m_size <= m_offset)
			{
				m_offset = 0;
				m_SRH->resF = resF_ONLY;
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
	m_file.Close();

	tmpx.Format("%08d", L_saveform+nRead);
	CopyMemory(m_SRH->datL, (char *) tmpx.operator LPCSTR(), sizeof(m_SRH->datL));

	m_saveF->idno[0] = '0';

	int pos = tmps.ReverseFind('\\');
	tmps = tmps.Mid(pos+1);	//tmps = makePath(tmps);
	strcpy_s(m_saveF->fnam, sizeof(m_saveF->fnam), tmps);

	switch (m_kind)
	{
	case kindFORMB:	m_saveF->type[0] = 'B'; break;
	case kindFORMS: m_saveF->type[0] = 'S'; break;
	case kindIMAGE:	m_saveF->type[0] = 'G'; break;
	case kindTRLY:	m_saveF->type[0] = 'T'; break;
	case kindINFO:	m_saveF->type[0] = 'I'; break;
	}

	m_saveF->gubn[0] = '1';
	if (!m_desc.IsEmpty())
		CopyMemory(m_saveF->mdsc, (char *)m_desc.operator LPCSTR(), m_desc.GetLength());

	tmps.Format("%08d", nRead);
	CopyMemory(m_saveF->datl, (char *)tmps.operator LPCTSTR(), sizeof(m_saveF->datl));

	return (L_rsmH+L_saveform+nRead);
}

void CTransDlg::close()
{
	enableSend(true);
	if (m_state == sOPEN)
		m_socket->Close();

	m_state = sCREATE;
}

void CTransDlg::updateProgress(int offset)
{
	offset = (offset * 100) / m_size;
	m_progress.SetPos(offset);	
}

void CTransDlg::enableSend(bool enable)
{
	CString	tmps;

	if (enable)
		tmps = _T("Click [Transmit]");
	else
		tmps.Format("Transmitting...[%s][%d/%d]", m_ips, m_index+1, m_files.GetSize());

	GetDlgItem(IDOK)->EnableWindow(enable);
	GetDlgItem(IDC_GUIDE)->SetWindowText(tmps);
}

CString CTransDlg::makePath(CString name)
{
	CString	path;

	switch (m_kind)
	{
	case kindFORMB:
		path.Format("%s/%s/%s/%s", MAPDIR, name.Left(L_SGID), name.Left(L_TGID), name);
		break;
	case kindFORMS:
		path.Format("%s/%s/%s/%s", MAPDIR, name.Left(L_SGID), name.Left(L_TGID), name);
		break;
	case kindIMAGE:
		path.Format("%s/%s", IMAGEDIR, name);	break;
	case kindTRLY:
		path.Format("%s/%s", TRLYDIR, name);	break;
	case kindINFO:
		path.Format("%s/%s", TABDIR, name);	break;
	default:
		path = name;	break;
	}

	path.MakeUpper();
	return path;
}

int CTransDlg::toInt(char* pBytes, int plen)
{
	CString	string = CString(pBytes, plen);
	return atoi((const char*) string.operator LPCSTR());
}