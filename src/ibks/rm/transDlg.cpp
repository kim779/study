// transDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axisrm.h"
#include "transDlg.h"

#include "../h/axisvar.h"
#include "../h/axisrsm.h"
#include "../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransDlg dialog

CTransDlg::CTransDlg(CWnd* pParent, int kind)
	: CDialog(CTransDlg::IDD, pParent)
{
	m_kind = kind;

	m_files.RemoveAll();

	m_state  = sNONE;
	m_socket = (CWnd *) NULL;

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
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CTransDlg, CDialog)
	ON_EVENT(CTransDlg, (UINT)-1, (UINT)1, OnFire, VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransDlg message handlers

BOOL CTransDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_sendB = new char[szMAX];
	m_recvB = new char[szMAX];
	m_SRH   = (struct _rsmH *)&m_sendB[0];
	m_RRH   = (struct _rsmH *)&m_recvB[0];

	m_socket = new CWnd();
	if (m_socket->CreateControl("AxisSock.SockCtrl.IBK2019", NULL, WS_CHILD, CRect(0,0,0,0), this, -1))
		m_state = sCREATE;

	GetDlgItem(IDOK)->EnableWindow(FALSE);
	for (int ii = 0; ii < 4; ii++)
		m_addr.SetFieldRange(ii, 0, 255);
	m_addr.SetFocus();
	m_addr.SetFieldFocus(0);

	m_progress.SetRange(0, 100);
	m_progress.SetPos(0);

	// TODO: Add extra initialization here
	
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
		rc = FALSE;
		enableSend(false);
		m_socket->InvokeHelper(DI_OPEN, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_BSTR VTS_BSTR VTS_I4), "", m_ips, verPORT);
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

int CTransDlg::OnFire(int type, char* pBytes, int nBytes)
{
	CString	tmps;

	switch (type)
	{
	case FEV_OPEN:
		if (!pBytes)
		{
			BOOL	rc = FALSE;
			m_socket->InvokeHelper(DI_AXIS, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
							(BYTE *)(VTS_I4 VTS_I4), NULL, modeWS);
			if (rc)
			{
				m_state = sOPEN;
				OnTransmit();
				return 0;
			}
			return 0;
		}

	case FEV_ERROR:
		tmps = CString(pBytes, nBytes);
		enableSend(true);
		AfxMessageBox(tmps, MB_OK|MB_ICONEXCLAMATION);
		return 0;

	case FEV_AXIS:
		if (m_state != sOPEN)
			return 0;
		break;
	}

	int	datL;
	
	CopyMemory(m_recvB, pBytes, nBytes);
	datL = atoi(CString(m_RRH->datL, sizeof(m_RRH->datL)));
	if (m_RRH->resK == resK_ERR)
	{
		GetDlgItem(IDC_GUIDE)->SetWindowText(CString(&m_recvB[L_rsmH], datL));
		close();
		return 0;
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

	BOOL	rc = FALSE;
	m_socket->InvokeHelper(DI_WRITE, DISPATCH_METHOD, VT_BOOL, (void *)&rc,
					(BYTE *)(VTS_I4 VTS_I4), m_sendB, sendL+L_rsmH);
	if (!rc)
		return false;

	updateProgress(m_offset);
        return true;
}

int CTransDlg::makeData()
{
	DWORD	nRead;
	CString	tmps, tmpx;

	ZeroMemory(m_sendB, szMAX);
	m_SRH->dirF = dirF_INB;
	m_SRH->resC = resC_EXT;
	m_SRH->rsvB[0] = rsvB_NEW;
	m_SRH->resK = (m_kind == rsAXIS) ? resK_AXIS : resK_RSC;

	tmps = m_files.GetAt(m_index);
	if (!m_file.Open(tmps, CFile::modeRead|CFile::typeBinary))
		return -1;

	m_size = (int) m_file.GetLength();
	if (m_size <= 0)
	{
		m_file.Close();
		return -1;
	}

	m_file.Seek(m_offset, CFile::begin);
	nRead = m_file.Read(&m_sendB[L_rsmH], szMAX-L_rsmH);
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

	m_offset = (int) m_file.Seek(0, CFile::current);
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

	int	pos = tmps.ReverseFind('\\');
	tmps = tmps.Mid(pos+1);
	tmps = makePath(tmps);
	strcpy_s(m_SRH->resN, sizeof(m_SRH->resN), tmps);

	tmps.Format("%05d", nRead);
	CopyMemory(m_SRH->datL, (char *)tmps.operator LPCTSTR(), sizeof(m_SRH->datL));

	return nRead;
}

void CTransDlg::close()
{
	enableSend(true);
	if (m_state == sOPEN)
		m_socket->InvokeHelper(DI_CLOSE, DISPATCH_METHOD, VT_EMPTY, (void *)NULL, (BYTE *)NULL);

	m_state = sNONE;
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
	int	idx;

	switch (m_kind)
	{
	case rsDEV:
		path.Format("%s/%s", DEVDIR, name);	break;
	case rsTAB:
		path.Format("%s/%s", TABDIR, name);	break;
	case rsIMAGE:
		path.Format("%s/%s", IMAGEDIR, name);	break;
	case rsMTBL:
		path.Format("%s/%s", MTBLDIR, name);	break;
	case rsHELP:
		path.Format("%s/%s", HELPDIR, name);	break;
	case rsMAP:
		path.Format("%s/%s/%s/%s", MAPDIR, name.Left(L_SGID), name.Left(L_TGID), name);
		break;
	case rsEXE:
		path.Format("%s/%s", RUNDIR, name);	break;
	case rsROP:
		path.Format("%s/%s", ROPDIR, name);	break;
	case rsMENU:
		idx = name.ReverseFind('.');
		if (idx != -1)
			name = name.Left(idx);
		path.Format("menu/%s", name);
		path.MakeUpper();
		path += ".menu";
		return path;
	case rsGEX:
		path.Format("%s/%s", GEXDIR, name);	break;
	case rsAXIS:
	default:
		path = name;	break;
	}

	path.MakeUpper();
	return path;
}
