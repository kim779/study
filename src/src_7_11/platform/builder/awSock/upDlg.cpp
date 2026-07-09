// upDlg.cpp : implementation file
//

#include "stdafx.h"
#include "upDlg.h"
#include "wsock.h"
#include "../mapvar.h"
#include "../mapform.h"
#include "../awWcc/libwcc.h"
#include "../../h/axisrsm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	resC_EXPORT	0x80

/////////////////////////////////////////////////////////////////////////////
// CUpDlg dialog
//
CUpDlg::CUpDlg(CWnd* pParent, struct _mapH* mapH)
	: CDialog(CUpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUpDlg)
	m_nick   = _T("");
	m_object = _T("");
	m_source = _T("");
	m_status = _T("");
	//}}AFX_DATA_INIT

	m_ok    = false;
	m_what  = W_SRC;
	m_count = 0;
	m_index = 0;
	m_offset = 0;
	m_mapH   = mapH;
	m_pFormItem = new CFormItem(m_mapH);

	m_wsock = (CwSock *) NULL;
}

CUpDlg::~CUpDlg()
{
	if (m_wsock)	delete m_wsock;
	if (m_pFormItem)
		delete m_pFormItem;
}

void CUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpDlg)
	DDX_Control(pDX, IDC_RSCLIST, m_list);
	DDX_Control(pDX, IDC_PROGRESS, m_step);
	DDX_Text(pDX, IDC_NICKN, m_nick);
	DDX_Text(pDX, IDC_OBJECTN, m_object);
	DDX_Text(pDX, IDC_SOURCEN, m_source);
	DDX_Text(pDX, IDC_STATUS, m_status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpDlg, CDialog)
	//{{AFX_MSG_MAP(CUpDlg)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STATE, OnState)
	ON_MESSAGE(WM_READDATA, OnReadData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpDlg message handlers

BOOL CUpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_wsock = new CwSock(this);
	goState(stCONNECTING);
	if (!m_wsock->Open())
	{
		MessageBox("Can't Open Socket", "ERROR", MB_OK);
		return TRUE;
        }
	m_pSRH = (struct _rsmH *) &m_wsock->m_sendB[0];
	m_pRRH = (struct _rsmH *) &m_wsock->m_recvB[0];
	goState(stCONNECTED);
	
	m_step.SetRange(0, 100);
	m_source = CString(m_mapH->mapN, L_MAPN);
	if (m_mapH->compile)
		m_object = CString(m_mapH->mapN, L_MAPN);
	/*if (m_mapH->nickN[0] != NULL)
		m_nick   = CString(m_mapH->nickN, L_MAPN);*/
	
	m_source.MakeUpper();
	m_object.MakeUpper();
	m_nick.MakeUpper();

	fillResourceList();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUpDlg::OnCancel() 
{
	goState(stIDLE, " STATUS : Canceled");
	CDialog::OnCancel();
}

void CUpDlg::OnOK() 
{
	if (m_ok)
	{
		m_wsock->Close();
		goState(stIDLE, " STATUS : Exit");
		CDialog::OnOK();
		return;
	}

	if (m_state == stCONNECTED)
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		sendData(true);
	}
}

bool CUpDlg::sendData(bool next)
{
	int	sendL = 0;
	if (next)
	{
		sendL = makeData();
		if (sendL < 0)
			OnCancel();
	}

	if (!m_wsock->Write(sendL + L_rsmH + L_workIO))
		return false;

	updateProgress(m_offset);
        return true;
}

int CUpDlg::makeData()
{
	DWORD	nRead;
	CString	tmps = _T(""), fullName = setMapPath(m_source);
	CString	resN = _T("");
	CButton* export = (CButton*) GetDlgItem(IDC_EXPORT);

	ZeroMemory(m_wsock->m_sendB, MAX_IO);
	ZeroMemory(&m_workIO, L_workIO);
	m_pSRH->dirF = dirF_INB;
	m_workIO.tgub = GB_MP;
	CopyMemory(&m_workIO.mapn, m_source.operator LPCTSTR(), min(m_source.GetLength(), 8));
	tmps.Format("%03d", m_mapH->mapGroup);
	strncpy(&m_workIO.mgub[0], tmps.operator LPCTSTR(), 3);
	strcpy(&m_workIO.mapd[0], &m_mapH->mapdesc[0]);
	tmps = _T("");

	switch (m_what)
	{
	case W_SRC:
		m_pSRH->resK = resK_RSC;
		m_pSRH->resC = resC_ASCII;
		CopyMemory(m_pSRH->resN, (char *) fullName.operator LPCTSTR(), min(fullName.GetLength(), 64));
		wccSetTempPath(tmps, (LPSTR)m_source.operator LPCTSTR());//m_pSRH->resN);
		break;

	case W_BIN:
		m_pSRH->resK = resK_RSC;
		m_pSRH->resC = 0;
		fullName = setMapPath(m_source);
		if (export->GetCheck() == 1)
			m_pSRH->resC = resC_EXPORT;
		CopyMemory(m_pSRH->resN, (char *) fullName.operator LPCTSTR(), min(fullName.GetLength(), 64));
		wccSetTempPath(tmps, (LPSTR)m_source.operator LPCTSTR(), false);
		break;

	case W_RES:
		m_pSRH->resK = resK_RSC;
		m_pSRH->resC = NULL;
		m_list.SetCurSel(m_index);
		m_list.GetText(m_index, resN);
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

	if (!ReadFile(m_file, &m_wsock->m_sendB[L_rsmH + /*(m_offset?0:*/L_workIO], MAX_IO - L_rsmH - /*(m_offset?0:*/L_workIO, &nRead, NULL))
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

	//if (!m_offset)
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

void CUpDlg::updateProgress(int offset)
{
	offset = (offset * 100) / m_size;
	m_step.SetPos(offset);	
}

void CUpDlg::fillResourceList()
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
				{
					m_list.AddString(tmps);
					m_count++;
				}
			}
		}
	}

	for (int ii = 0; ii < m_pFormItem->getFormCount(); ii++)
	{
		if (m_pFormItem->getFormPoint(ii)->kind == FM_AVI || (m_pFormItem->getFormPoint(ii)->properties & PR_IMAGE))
		{
			tmps = m_pFormItem->getFormStr(ii, "str");
			tmps.TrimRight();
			offs = tmps.ReverseFind('\\');
			if (offs != -1)
			{
				CString	res;
				tmps = tmps.Mid(++offs);
				for (int jj = 0; jj < m_list.GetCount(); jj++)
				{
					m_list.GetText(jj, res);
					if (!res.CompareNoCase(tmps))
						break;
				}

				if (jj >= m_list.GetCount())
				{
					dir.Empty();
					wccSetTempPath(dir, (char *)tmps.operator LPCTSTR());
					if (CopyFile(m_pFormItem->getFormStr(ii, "str"), dir, false))
					{
						m_list.AddString(tmps);
						m_count++;
					}
				}
			}

		}
	}
}

void CUpDlg::goState(int state, CString msg)
{
	m_state  = state;
	m_status = " STATUS : ";
	switch (m_state)
	{
	case stADDRESS:
		m_status += "None of IP Address";
		break;
	case stCONNECTING:
		m_status += "Connecting";
		break;
	case stCONNECTED:
		m_status += "Connected";
		break;
	case stCOMPLETE:
		m_status += "Completed work";
		m_ok = true;
		GetDlgItem(IDOK)->SetWindowText("Close");
		SetDlgItemText(IDC_STATUS, m_status);
		updateProgress(m_size);
		m_wsock->Close();
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		EndDialog(IDOK);
		return;
	case stIDLE:
	default:
		m_wsock->Close();
		m_status += "DisConnected";
		break;
	}

	if (!msg.IsEmpty())
		m_status = msg;

	SetDlgItemText(IDC_STATUS, m_status);
}

LONG CUpDlg::OnState(WPARAM wParam, LPARAM lParam)
{
	CString	msg = (char *)lParam;
	goState((int)wParam, msg);
	return 0;
}

LONG CUpDlg::OnReadData(WPARAM wParam, LPARAM lParam)
{
	CString	tmps;

	if (m_state != stCONNECTED)
		return 0;

	int	datL = wccToNUMBER(m_pRRH->datL, sizeof(m_pRRH->datL));
	if (m_pRRH->resK == resK_ERR)
	{
		goState(stIDLE, CString(&m_wsock->m_recvB[L_rsmH], datL));
		return 0;
	}

	if (m_pRRH->resC & resC_ASCII)
	{
		if (m_pRRH->resF == resF_ONLY || m_pRRH->resF == resF_LAS)
		{
			if (m_mapH->compile)
				m_what = W_BIN;
			else
			{
				if (m_count != 0)
					m_what = W_RES;
				else
				{
					goState(stCOMPLETE);
					return 0;
				}
			}
		}
		sendData();
	}
	else
	{
		tmps = _T("");
		/*if (m_pRRH->resK == resK_IMAGE)
		{
			if (m_pRRH->resF == resF_ONLY || m_pRRH->resF == resF_LAS)
			{
				wccSetTempPath(tmps, m_pRRH->resN);
				DeleteFile(tmps);

				if (m_index == (m_count - 1))
				{
					goState(stCOMPLETE);
					return 0;
				}
				else
					m_index++;
			}
		}
		else*/
		{
			if (m_pRRH->resF == resF_ONLY || m_pRRH->resF == resF_LAS)
			{
				if (m_what == W_BIN)
				{
					if (m_count != 0)
						m_what = W_RES;
					else
					{
						goState(stCOMPLETE);
						return 0;
					}
				}
				else
				{
					if (m_count != 0)
						m_what = W_RES;
					else
					{
						goState(stCOMPLETE);
						return 0;
					}
				}
			}
		}
		sendData();
	}

	return 0;
}

CString CUpDlg::setMapPath(CString mapN)
{
	if (mapN.GetLength() != 8)
		return "";
	CString tmp;
	tmp.Format("map/%s/%s/%s", mapN.Left(2), mapN.Left(3), mapN);
	return tmp;
}
