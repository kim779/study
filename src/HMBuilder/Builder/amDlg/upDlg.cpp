// upDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "upDlg.h"
#include "afxdialogex.h"

#include "../h/mapvar.h"
#include "../h/mapform.h"
#include "../h/workio.h"
#include "../amCom/formItem.h"
#include "../h/axisrsm.h"
//#include "../awWcc64/libwcc.h"
//#include "../awSock64/wsock.h"

#define	resC_EXPORT	0x80

// CUpDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUpDlg, CDialogEx)

CUpDlg::CUpDlg(CWnd* pParent, struct _mapH* mapH, bool isMap)
	: CDialogEx(CUpDlg::IDD, pParent)
{
	m_pParent = pParent;
	m_mapH  = mapH;
	m_isMap = isMap;

	m_nick = _T("");
	m_object = _T("");
	m_source = _T("");

	m_done  = false;
	m_what  = W_SRC;
	m_count = 0;
	m_index = 0;
	m_offset = 0;
	m_sock = NULL;

	m_ver1 = _T("");
	m_ver2 = _T("");
	m_gubn = _T("");
	m_desc = _T("");
}

CUpDlg::~CUpDlg()
{
	//if (m_sock)
	//	delete m_sock;
}

void CUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SOURCEN, m_source);
}


BEGIN_MESSAGE_MAP(CUpDlg, CDialogEx)
	ON_MESSAGE(WM_OPENEDSOCK, &CUpDlg::OnOpenedSock)
	ON_MESSAGE(WM_READDATA, &CUpDlg::OnReadData)
	ON_MESSAGE(WM_STATE, &CUpDlg::OnState)
END_MESSAGE_MAP()


// CUpDlg 메시지 처리기입니다.

BOOL CUpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
		
	((CEdit*)GetDlgItem(IDC_SOURCEN))->SetWindowTextA(m_mapH->mapN);
		
	m_source = CString(m_mapH->mapN, L_MAPN);
	m_source.MakeUpper();
	((CButton*)GetDlgItem(IDC_RA_TEMP))->SetCheck(1);

	((CEdit*)GetDlgItem(IDC_RICH_DESC))->LimitText(500);
	((CEdit*)GetDlgItem(IDC_EDIT_VER1))->LimitText(4);
	((CEdit*)GetDlgItem(IDC_EDIT_VER2))->LimitText(4);

	UpdateData(FALSE);
	
	return TRUE;
}


void CUpDlg::OnOK()
{
	CDialogEx::OnOK();

	//strMsg.Format("%s/%s/%d/%s/", strver1, strver2, iGubn, strDesc);
		
	//m_pParent->SendMessage(WM_UPLOAD, (WPARAM)(LPCTSTR)strMsg, (LPARAM)m_mapH);

	if (((CButton*)GetDlgItem(IDC_RA_TEMP))->GetCheck())
		m_gubn = _T("0");
	else
		m_gubn = _T("1");

	((CEdit*)GetDlgItem(IDC_EDIT_VER1))->GetWindowTextA(m_ver1);
	((CEdit*)GetDlgItem(IDC_EDIT_VER2))->GetWindowTextA(m_ver2);
	((CEdit*)GetDlgItem(IDC_RICH_DESC))->GetWindowTextA(m_desc);

	m_strTotData.Format("%s/%s/%s/%s/", m_gubn, m_ver1, m_ver2, m_desc);

	CDialogEx::OnOK();

}


void CUpDlg::OnCancel()
{
	//goState(stIDLE, _T(" STATUS : Canceled"));
	CDialogEx::OnCancel();
}


LRESULT CUpDlg::OnOpenedSock(WPARAM wParam, LPARAM lParam)
{
	/*if (!lParam)
		OnCancel();
	else
		goState(stCONNECTED);*/
	return 0;
}


LRESULT CUpDlg::OnReadData(WPARAM wParam, LPARAM lParam)
{
	/*if (m_state != stCONNECTED)
		return 0;

	CString	tmps;
	int	datl;
	struct	_rsmH*	rsmH;

	rsmH = (struct _rsmH *)m_sock->m_recvB;
	datl = atoi(CString(rsmH->dlen, sizeof(rsmH->dlen)));
	if (rsmH->resK == resK_ERR)
	{
		goState(stIDLE, CString(&m_sock->m_recvB[L_rsmH], datl));
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
		if (rsmH->resF == resF_ONLY || rsmH->resF == resF_LAS)
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
	sendData();*/
	return 0;
}


LRESULT CUpDlg::OnState(WPARAM wParam, LPARAM lParam)
{
	//goState((int)wParam, (char *)lParam);
	return 0;
}

bool CUpDlg::sendData(bool next)
{
	/*char	sndb[MAX_IO];
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
	}
	if (!m_sock->Write(sndb, L_rsmH + L_workIO + sndl))
		return false;

	updateProgress(m_offset);*/
        return true;
}

int CUpDlg::makeData(char* sndb)
{
	CFile	file;
//	ULONGLONG size;
	UINT	readl = 0;
	/*CString	tmps, fullN;
	struct	_rsmH*	rsmH;
	struct	_workIO* wio;

	rsmH = (struct _rsmH *)sndb;
	wio  = (struct _workIO *)&sndb[L_rsmH];

	rsmH->dirF = dirF_INB;
	wio->tgub  = m_isMap ? GB_MP : GB_TM;
	CopyMemory(wio->mapn, m_source.operator LPCTSTR(), min(m_source.GetLength(), L_MAPN));
	tmps.Format(_T("%03d"), m_mapH->mapGroup);
	CopyMemory(wio->mgub, tmps.operator LPCTSTR(), sizeof(wio->mgub)-1);
	strcpy_s(wio->mapd, sizeof(wio->mapd), m_mapH->mapdesc);

	switch (m_what)
	{
	case W_SRC:
		rsmH->resK = resK_RSC;
		rsmH->resC = resC_ASCII;
		fullN = setMapPath(m_source);
		CopyMemory(rsmH->resN, (char *)fullN.operator LPCTSTR(), min(fullN.GetLength(), sizeof(rsmH->resN)-1));
		wccSetTempPath(tmps, (LPSTR)m_source.operator LPCTSTR());
		break;

	case W_BIN:
		rsmH->resK = resK_RSC;
		rsmH->resC = 0;
		fullN = setMapPath(m_source);
		if (((CButton*)GetDlgItem(IDC_EXPORT))->GetCheck() == 1)
			rsmH->resC = resC_EXPORT;
		CopyMemory(rsmH->resN, (char *)fullN.operator LPCTSTR(), min(fullN.GetLength(), sizeof(rsmH->resN)-1));
		wccSetTempPath(tmps, (LPSTR)m_source.operator LPCTSTR(), false);
		break;

	case W_RES:
		rsmH->resK = resK_RSC;
		rsmH->resC = NULL;
		m_list.SetCurSel(m_index);
		m_list.GetText(m_index, tmps);
		CopyMemory(rsmH->resN, (char *)tmps.operator LPCTSTR(), min(tmps.GetLength(), sizeof(rsmH->resN)-1));
		wccSetTempPath(tmps, rsmH->resN);
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
	CopyMemory(rsmH->dlen, (char *)tmps.operator LPCSTR(), min(sizeof(rsmH->dlen), tmps.GetLength()));*/
	return readl;
}

void CUpDlg::updateProgress(int offset)
{
	//offset = (offset * 100) / m_size;
	//m_step.SetPos(offset);
}

void CUpDlg::fillResourceList()
{
	/*CString	tmps, dir;
	int	jj, offs;
	CFormItem	formItem(m_mapH);

	if (m_mapH->imageK == IK_BMP)
	{
		if (m_mapH->imageV[0] != _T('\0'))
		{
			tmps = CString(m_mapH->imageV);
			tmps.TrimRight();
			offs = tmps.ReverseFind(_T('\\'));
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

	for (int ii = 0; ii < formItem.GetCount(); ii++)
	{
		if (formItem.GetForm(ii)->kind == FM_AVI || (formItem.GetForm(ii)->properties & PR_IMAGE))
		{
			tmps = formItem.GetFormStr(ii, _T("str"));
			tmps.TrimRight();
			offs = tmps.ReverseFind(_T('\\'));
			if (offs != -1)
			{
				CString	res;

				tmps = tmps.Mid(++offs);
				for (jj = 0; jj < m_list.GetCount(); jj++)
				{
					m_list.GetText(jj, res);
					if (!res.CompareNoCase(tmps))
						break;
				}

				if (jj >= m_list.GetCount())
				{
					dir.Empty();
					wccSetTempPath(dir, (char *)tmps.operator LPCTSTR());
					if (CopyFile(formItem.GetFormStr(ii, _T("str")), dir, false))
					{
						m_list.AddString(tmps);
						m_count++;
					}
				}
			}

		}
	}*/
}

CString CUpDlg::setMapPath(CString mapN)
{
	CString	tmps;
	/*if (mapN.GetLength() != L_MAPN)
		return "";

	tmps.Format(_T("map\\%s\\%s\\%s"), mapN.Left(L_SGID), mapN.Left(L_TGID), mapN);*/
	return tmps;
}

void CUpDlg::goState(int state, CString msg)
{
	/*m_state  = state;
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
	}*/
}


CString CUpDlg::GetDescription(void)
{
	CString strDesc;
	((CEdit*)GetDlgItem(IDC_RICH_DESC))->GetWindowTextA(strDesc);
	return strDesc;
}


int CUpDlg::GetGubn(void)
{
	if (((CButton*)GetDlgItem(IDC_RA_TEMP))->GetCheck())
		return 0;
	else
		return 1;
}


CString CUpDlg::GetVersion(int type)
{
	CString strver1;
	CString strver2;
	((CEdit*)GetDlgItem(IDC_EDIT_VER1))->GetWindowTextA(strver1);
	((CEdit*)GetDlgItem(IDC_EDIT_VER2))->GetWindowTextA(strver2);

	switch (type)
	{
	case 1:
		return strver1;
	case 2:
		return strver2;
	case 3:
		return strver1 + strver2;
	}

	return _T("");
}
