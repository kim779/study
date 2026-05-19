// LayoutUpload.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "LayoutUpload.h"
#include "afxdialogex.h"

#include "../h/mapvar.h"
#include "../h/mapform.h"
#include "../h/workio.h"
#include "../awWcc/libwcc.h"
#include "../awSock/wsock.h"
#include "../../h/axisrsm.h"
#include "../awTool/awObjectLoad.h"

// CLayoutUpload 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLayoutUpload, CDialogEx)
CLayoutUpload::CLayoutUpload(CWnd* pParent)
: CDialogEx(CLayoutUpload::IDD, pParent)
{
}

CLayoutUpload::CLayoutUpload(CWnd* pParent, CString userID, bool isMap)
: CDialogEx(CLayoutUpload::IDD, pParent)
{
	m_isMap = isMap;

	m_object = _T("");
	m_source = _T("");
	m_path = _T("");

	m_done = false;
	m_count = 0;
	m_index = 0;
	m_offset = 0;
	m_sock = (CwSock *)NULL;
	m_userID = userID;
}

CLayoutUpload::~CLayoutUpload()
{
}

void CLayoutUpload::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_step);
}


BEGIN_MESSAGE_MAP(CLayoutUpload, CDialogEx)
	ON_MESSAGE(WM_OPENEDSOCK, &CLayoutUpload::OnOpenedSock)
	ON_MESSAGE(WM_READDATA, &CLayoutUpload::OnReadData)
	ON_MESSAGE(WM_STATE, &CLayoutUpload::OnState)
	ON_BN_CLICKED(IDOK, &CLayoutUpload::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CLayoutUpload::OnBnClickedButtonUpload)
	ON_BN_CLICKED(IDC_BUTTON_PATH, &CLayoutUpload::OnBnClickedButtonPath)
END_MESSAGE_MAP()


// CLayoutUpload 메시지 처리기입니다.

BOOL CLayoutUpload::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sock = new CwSock(this);
	goState(stCONNECTING);
	if (!m_sock->Open())
	{
		MessageBox(_T("Can't Open Socket"), _T("ERROR"), MB_OK);
		return TRUE;
	}
	m_step.SetRange(0, 100);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CLayoutUpload::OnOK()
{
	if (m_done)
	{
		m_sock->Close();
		CDialogEx::OnOK();
		return;
	}

	if (m_state == stCONNECTED)
	{
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		sendData(true);
	}
}

void CLayoutUpload::OnCancel()
{
	goState(stIDLE, _T(" STATUS : Canceled"));
	CDialogEx::OnCancel();
}

LRESULT CLayoutUpload::OnOpenedSock(WPARAM wParam, LPARAM lParam)
{
	if (!lParam)
		OnCancel();
	else
		goState(stCONNECTED);
	return 0;
}


LRESULT CLayoutUpload::OnReadData(WPARAM wParam, LPARAM lParam)
{
	if (m_state != stCONNECTED)
		return 0;

	CString	tmps;
	int	datl;
	struct	_rsmH*	rsmH;

	rsmH = (struct _rsmH *)m_sock->m_recvB;
	datl = atoi(CString(rsmH->datL, sizeof(rsmH->datL)));

	if (rsmH->rcod != '0')
	{
		CString strErr;
		char* pBuffer = (char*)rsmH + L_rsmH;
		for (size_t ii = 0; ii < 80; ii++)
		{
			strErr.AppendFormat("%c", pBuffer[ii]);
			if (pBuffer[ii] == NULL)
				break;
		}
		MessageBox(strErr, NULL, MB_TOPMOST);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		return 0;
	}

	if (rsmH->resK == resK_ERR)
	{
		return 0;
	}

	if (rsmH->resC & resC_ASCII)
	{
		if (rsmH->resF == resF_ONLY || rsmH->resF == resF_LAS)
		{
			if (!m_isMap)
			{
				goState(stCOMPLETE);
				return 0;
			}

			if (m_count == 0)
			{
				goState(stCOMPLETE);
				return 0;
			}
		}
		sendData();
	}
	else
	{
		tmps = _T("");
		if (rsmH->resF == resF_ONLY || rsmH->resF == resF_LAS)
		{
			if (m_count == 0)
			{
				goState(stCOMPLETE);
				return 0;
			}
		}
	}
	sendData();
	return 0;
}

LRESULT CLayoutUpload::OnState(WPARAM wParam, LPARAM lParam)
{
	goState((int)wParam, (char *)lParam);
	return 0;
}

bool CLayoutUpload::sendData(bool next)
{
	char	sndb[MAX_IO];
	int	sndl = 0;

	ZeroMemory(sndb, MAX_IO);
	if (next)
	{
		sndl = makeData(sndb);
		if (sndl < 0)
		{
			OnCancel();
			return false;
		}
		else if (sndl == 0)
		{
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			return false;
		}
	}
	if (!m_sock->Write(sndb, L_rsmH + L_saversc + sndl))
		return false;

	updateProgress(m_offset);
	return true;
}

int CLayoutUpload::makeData(char* sndb)
{
	CFile	file;
	ULONGLONG size;
	UINT	readl;
	CString	tmps, fullN;
	struct	_rsmH*	rsmH;
	struct i_saversc* saversc;

	rsmH = (struct _rsmH *)sndb;
	saversc = (struct i_saversc *)&sndb[L_rsmH];

	rsmH->dirF = dirF_INB;
	CString userID = m_userID;
	CString ver1(""), ver2(""), ver3("");

	rsmH->mode = 'W';
	CString strCmt("");
	CopyMemory(saversc->mdsc, (char *)strCmt.operator LPCTSTR(), min(strCmt.GetLength(), sizeof(saversc->mdsc) - 1));

	CopyMemory(saversc->ver1, (char *)ver1.operator LPCTSTR(), min(ver1.GetLength(), sizeof(saversc->ver1) - 1));
	CopyMemory(saversc->ver2, (char *)ver2.operator LPCTSTR(), min(ver2.GetLength(), sizeof(saversc->ver2) - 1));
	CopyMemory(saversc->ver3, (char *)ver3.operator LPCTSTR(), min(ver3.GetLength(), sizeof(saversc->ver3) - 1));

	CopyMemory(rsmH->usid, (char *)userID.operator LPCTSTR(), min(userID.GetLength(), sizeof(rsmH->usid) - 1));
	rsmH->resK = resk_TRX;
	rsmH->resC = resC_ASCII;
	fullN = setTRPath(m_source);
	CopyMemory(rsmH->resN, (char *)fullN.operator LPCTSTR(), min(fullN.GetLength(), sizeof(rsmH->resN) - 1));
	//wccSetTempPath(tmps, (LPSTR)m_source.operator LPCTSTR());
	saversc->type[0] = 'T';

	if (!file.Open(m_path, CFile::modeRead | CFile::typeBinary))
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

	readl = file.Read(&sndb[L_rsmH + L_saversc - sizeof(saversc->file)], MAX_IO - L_rsmH - L_saversc - sizeof(saversc->file));
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

void CLayoutUpload::updateProgress(int offset)
{
	offset = (offset * 100) / m_size;
	m_step.SetPos(offset);
}

CString CLayoutUpload::setTRPath(CString trName)
{
	CString	tmps;

	tmps.Format(_T("trlayout/%s"), trName);
	return tmps;
}

void CLayoutUpload::goState(int state, CString msg)
{
	m_state = state;
	switch (m_state)
	{
	case stADDRESS:
	case stCONNECTING:
	case stCONNECTED:
		break;

	case stCOMPLETE:
		GetDlgItem(IDOK)->SetWindowText(_T("Close"));
		updateProgress(m_size);
		m_sock->Close();
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_done = true;
		PostMessage(WM_COMMAND, IDOK, (LPARAM)this->m_hWnd);
		break;

	case stIDLE:
	default:
		m_sock->Close();
		break;
	}
}

void CLayoutUpload::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

void CLayoutUpload::OnBnClickedButtonUpload()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	sendData(true);
}

void CLayoutUpload::OnBnClickedButtonPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char szFilter[] = "All Files(*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal())
	{
		m_source = dlg.GetFileName();
		m_path = dlg.GetPathName();
		GetDlgItem(IDC_EDIT_PATH)->SetWindowText(m_path);
	}
}