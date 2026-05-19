// upDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "upDlg.h"
#include "afxdialogex.h"

#include "../h/mapvar.h"
#include "../h/mapform.h"
#include "../h/workio.h"
#include "../awWcc/libwcc.h"
#include "../awSock/wsock.h"
#include "../../h/axisrsm.h"
#include "../awTool/awObjectLoad.h"
#include "DiffViewerDlg.h"
#include "../h/mainvar.h"

#define	resC_EXPORT	0x80

// CUpDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUpDlg, CDialogEx)

CUpDlg::CUpDlg(CWnd* pParent, class mapForm* mapForm, CString userID, bool isMap)
	: CDialogEx(CUpDlg::IDD, pParent)
{
	m_mapH  = mapForm;
	m_isMap = isMap;

	m_object = _T("");
	m_source = _T("");

	m_done  = false;
	m_what  = W_SRC;
	m_count = 0;
	m_index = 0;
	m_offset = 0;
	m_sock = (CwSock *) NULL;
	m_userID = userID;
	m_sRoot = _T("");
}

CUpDlg::~CUpDlg()
{
	if (m_sock)
		delete m_sock;
}

void CUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_OBJECTN, m_object);
	DDX_Control(pDX, IDC_PROGRESS, m_step);
	DDX_Text(pDX, IDC_SOURCEN, m_source);
	DDX_Control(pDX, IDC_CHECK_TMP, m_CheckTmp);
	DDX_Control(pDX, IDC_REDIT_COMMENT, m_rEditCmt);
}


BEGIN_MESSAGE_MAP(CUpDlg, CDialogEx)
	ON_MESSAGE(WM_OPENEDSOCK, &CUpDlg::OnOpenedSock)
	ON_MESSAGE(WM_READDATA, &CUpDlg::OnReadData)
	ON_MESSAGE(WM_STATE, &CUpDlg::OnState)
	ON_BN_CLICKED(IDC_CHECK_TMP, &CUpDlg::OnBnClickedCheckTmp)
	ON_BN_CLICKED(IDC_BUTTON1_DIFF, &CUpDlg::OnBnClickedButton1Diff)
END_MESSAGE_MAP()


// CUpDlg 메시지 처리기입니다.

BOOL CUpDlg::OnInitDialog()
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
	m_source = CString(m_mapH->mapN, L_MAPN);
	if (m_isMap && m_mapH->compile)
		m_object = CString(m_mapH->mapN, L_MAPN);
	
	m_source.MakeUpper();
	m_object.MakeUpper();

	((CEdit*)GetDlgItem(IDC_REDIT_COMMENT))->LimitText(500);
	((CEdit*)GetDlgItem(IDC_EDIT_VER1))->LimitText(4);
	((CEdit*)GetDlgItem(IDC_EDIT_VER2))->LimitText(4);
	((CEdit*)GetDlgItem(IDC_EDIT_VER3))->LimitText(4);

	CRect rc;
	int iVGap = 6;
	((CEdit*)GetDlgItem(IDC_EDIT_VER1))->GetRect(rc);
	rc.top += iVGap;
	rc.bottom -= iVGap;
	((CEdit*)GetDlgItem(IDC_EDIT_VER1))->SetRect(rc);
	
	((CEdit*)GetDlgItem(IDC_EDIT_VER2))->GetRect(rc);
	rc.top += iVGap;
	rc.bottom -= iVGap;
	((CEdit*)GetDlgItem(IDC_EDIT_VER2))->SetRect(rc);

	((CEdit*)GetDlgItem(IDC_EDIT_VER3))->GetRect(rc);
	rc.top += iVGap;
	rc.bottom -= iVGap;
	((CEdit*)GetDlgItem(IDC_EDIT_VER3))->SetRect(rc);

	m_CheckTmp.SetCheck(1);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUpDlg::OnOK()
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


void CUpDlg::OnCancel()
{
	goState(stIDLE, _T(" STATUS : Canceled"));
	CDialogEx::OnCancel();
}


LRESULT CUpDlg::OnOpenedSock(WPARAM wParam, LPARAM lParam)
{
	if (!lParam)
		OnCancel();
	else
		goState(stCONNECTED);
	return 0;
}


LRESULT CUpDlg::OnReadData(WPARAM wParam, LPARAM lParam)
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
	sendData();
	return 0;
}


LRESULT CUpDlg::OnState(WPARAM wParam, LPARAM lParam)
{
	goState((int)wParam, (char *)lParam);
	return 0;
}

bool CUpDlg::sendData(bool next)
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
		else if(sndl == 0)
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

int CUpDlg::makeData(char* sndb)
{
	CFile	file;
	ULONGLONG size;
	UINT	readl;
	CString	tmps, fullN;
	struct	_rsmH*	rsmH;
	//struct	_workIO* wio;
	struct i_saversc* saversc;
	rsmH = (struct _rsmH *)sndb;
	//wio  = (struct _workIO *)&sndb[L_rsmH];
	saversc = (struct i_saversc *)&sndb[L_rsmH];
	rsmH->dirF = dirF_INB;
	//wio->tgub  = m_isMap ? GB_MP : GB_TM;
	//CopyMemory(wio->mapn, m_source.operator LPCTSTR(), min(m_source.GetLength(), L_MAPN));
	//tmps.Format(_T("%03d"), m_mapH->mapGroup);
	//CopyMemory(wio->mgub, tmps.operator LPCTSTR(), sizeof(wio->mgub)-1);
	//strcpy_s(wio->mapd, sizeof(wio->mapd), m_mapH->mapdesc);
	CString userID = m_userID;
	CString ver1, ver2, ver3;
	((CEdit*)GetDlgItem(IDC_EDIT_VER1))->GetWindowText(ver1);
	((CEdit*)GetDlgItem(IDC_EDIT_VER2))->GetWindowText(ver2);
	((CEdit*)GetDlgItem(IDC_EDIT_VER3))->GetWindowText(ver3);
	if (m_CheckTmp.GetCheck() == 1)
	{
		rsmH->mode = 'T';
	}
	else
	{
		rsmH->mode = 'W';
		CString strCmt;
		m_rEditCmt.GetWindowText(strCmt);
		if (strCmt == _T("") || ver1 == _T("") || ver2 == _T("") || ver3 == _T(""))
		{
			MessageBox("Please enter the version and comment.");
			return 0;
		}
		CopyMemory(saversc->mdsc, (char *)strCmt.operator LPCTSTR(), min(strCmt.GetLength(), sizeof(saversc->mdsc) - 1));
	}
	CopyMemory(saversc->ver1, (char *)ver1.operator LPCTSTR(), min(ver1.GetLength(), sizeof(saversc->ver1) - 1));
	CopyMemory(saversc->ver2, (char *)ver2.operator LPCTSTR(), min(ver2.GetLength(), sizeof(saversc->ver2) - 1));
	CopyMemory(saversc->ver3, (char *)ver3.operator LPCTSTR(), min(ver3.GetLength(), sizeof(saversc->ver3) - 1));
	CopyMemory(rsmH->usid, (char *)userID.operator LPCTSTR(), min(userID.GetLength(), sizeof(rsmH->usid) - 1));
	switch (m_what)
	{
	case W_SRC:
		rsmH->resK = resK_RSC;
		rsmH->resC = resC_ASCII;
		fullN = setMapPath(m_source);
		CopyMemory(rsmH->resN, (char *)fullN.operator LPCTSTR(), min(fullN.GetLength(), sizeof(rsmH->resN)-1));
		wccSetTempPath(tmps, (LPSTR)m_source.operator LPCTSTR());
		tmps += _T(".xml");
		saversc->type[0] = 'S';
		break;
	case W_BIN:
		rsmH->resK = resK_RSC;
		rsmH->resC = 0;
		fullN = setMapPath(m_source);
		if (((CButton*)GetDlgItem(IDC_EXPORT))->GetCheck() == 1)
			rsmH->resC = resC_EXPORT;
		CopyMemory(rsmH->resN, (char *)fullN.operator LPCTSTR(), min(fullN.GetLength(), sizeof(rsmH->resN)-1));
		wccSetTempPath(tmps, (LPSTR)m_source.operator LPCTSTR(), false);
		saversc->type[0] = 'B';
		break;
	//case W_RES:
	//	rsmH->resK = resK_RSC;
	//	rsmH->resC = NULL;
	//	m_list.SetCurSel(m_index);
	//	m_list.GetText(m_index, tmps);
	//	CopyMemory(rsmH->resN, (char *)tmps.operator LPCTSTR(), min(tmps.GetLength(), sizeof(rsmH->resN)-1));
	//	wccSetTempPath(tmps, rsmH->resN);
	//	break;
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

void CUpDlg::updateProgress(int offset)
{
	offset = (offset * 100) / m_size;
	m_step.SetPos(offset);
}

CString CUpDlg::setMapPath(CString mapN)
{
	if (mapN.GetLength() != L_MAPN)
		return "";

	CString	tmps;

	tmps.Format(_T("map/%s/%s/%s"), mapN.Left(L_SGID), mapN.Left(L_TGID), mapN);
	return tmps;
}

void CUpDlg::goState(int state, CString msg)
{
	m_state  = state;
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

void CUpDlg::OnBnClickedCheckTmp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_CheckTmp.GetCheck() == 1)
	{
		m_rEditCmt.EnableWindow(FALSE);
	}
	else
	{
		m_rEditCmt.EnableWindow(TRUE);
	}
}

void CUpDlg::OnBnClickedButton1Diff()
{
	AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_SHOWDIFF, 0), -1);
}
