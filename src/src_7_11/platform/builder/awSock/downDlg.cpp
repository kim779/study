// downDlg.cpp : implementation file
//

#include "stdafx.h"
#include "downDlg.h"
#include "wsock.h"
#include "../mapform.h"
#include "../mainfrm.h"
#include "../childfrm.h"
#include "../awWcc/libwcc.h"
#include "../../h/axisrsm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDownDlg dialog
//
CDownDlg::CDownDlg(CMainFrame* frame, CString dir,  CString path, int child, bool request)
	: CDialog(CDownDlg::IDD, frame)
{
	//{{AFX_DATA_INIT(CDownDlg)
	m_status = _T("");
	m_name = _T("");
	//}}AFX_DATA_INIT

	m_count   = 0;
	m_index   = 0;
	m_dir     = dir;
	m_child   = child;
	m_state   = stIDLE;
	m_enable  = true;
	m_finish  = false;
	m_request = request;
	m_mainFRM = frame;

	m_files.RemoveAll();
	m_types.RemoveAll();
	m_arMAP.RemoveAll();
	m_arUOB.RemoveAll();

	m_path = _T("");
	m_type = R_NONE;
	m_item = (HTREEITEM) NULL;
	int	offs = path.Find(':');
	if (offs != -1)
	{
		m_path = path.Left(offs);
		if (!m_path.CompareNoCase(MAPEN))
			m_type = R_MAP;
		else if (!m_path.CompareNoCase(UOBEN))
			m_type = R_UOB;

		if (m_type != R_NONE)
			m_path = path.Mid(offs+1);
	}

	m_wsock = (CwSock *) NULL;
}

CDownDlg::~CDownDlg()
{
	if (m_wsock)
		delete m_wsock;

	removeAll();
}

void CDownDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownDlg)
	DDX_Text(pDX, IDC_STATUS, m_status);
	DDX_Text(pDX, IDC_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownDlg, CDialog)
	//{{AFX_MSG_MAP(CDownDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblClkTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_LIST, OnSelChangedTree)
	ON_EN_SETFOCUS(IDC_NAME, OnSetFocusName)
	ON_BN_CLICKED(IDC_REQ, OnReq)
	ON_BN_CLICKED(IDC_KIND1, OnMAP)
	ON_BN_CLICKED(IDC_KIND2, OnUSER)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STATE, OnState)
	ON_MESSAGE(WM_READDATA, OnReadData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownDlg message handlers
//
BOOL CDownDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	VERIFY(m_tree.Attach(IDC_LIST, this));

	CImageList	imglist;
	CBitmap		bitmap;

	bitmap.LoadBitmap(IDB_TREE);
	imglist.Create(16, 16, ILC_COLORDDB|ILC_MASK, 5, 1);
	imglist.Add(&bitmap, RGB(255, 0, 255));
	imglist.SetBkColor(CLR_NONE);
	m_tree.SetImageList(&imglist, TVSIL_NORMAL);
	imglist.Detach();
	bitmap.Detach();

	m_kind[0] = (CButton *) GetDlgItem(IDC_KIND1);
	m_kind[1] = (CButton *) GetDlgItem(IDC_KIND2);
	switch (m_type)
	{
	case R_MAP:
		m_kind[0]->SetCheck(1);	break;
	case R_UOB:
		m_kind[1]->SetCheck(1);	break;
	case R_NONE:
	default:
		m_kind[0]->SetCheck(0);
		m_kind[1]->SetCheck(0);	break;
	}

	removeAll();

	CString tmps;
	m_pDownLoad = new CDownLoad();
	wccSetTempPath(tmps, DN_FILE);

	m_pDownLoad->downLoad("", tmps, (m_bTemplate?3:2));
	delete m_pDownLoad;
	loadFile();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDownDlg::OnDblClkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_count = m_tree.GetSelectedItemCount();
	if (m_count == 0 || m_type == R_NONE  || m_name.IsEmpty())
	{
		*pResult = 0;
		return;
	}
	m_index = 0;
	sendData(false);
	*pResult = 0;
}

void CDownDlg::OnSelChangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if (pNMTreeView->itemNew.hItem == (HTREEITEM) NULL)
		return;

	m_type = m_tree.GetItemData(pNMTreeView->itemNew.hItem);
	switch (m_type)
	{
	case R_MAP:
		m_kind[0]->SetCheck(1);
		m_kind[1]->SetCheck(0);
		break;
		
	case R_UOB:
		m_kind[0]->SetCheck(0);
		m_kind[1]->SetCheck(1);
		break;

	default:
		m_kind[0]->SetCheck(0);
		m_kind[1]->SetCheck(0);
		break;
	}

	if (m_type != R_NONE)
		m_name = m_tree.GetItemText(pNMTreeView->itemNew.hItem);
	UpdateData(FALSE);
	*pResult = 0;
}

void CDownDlg::OnReq() 
{
	sendData();
}

void CDownDlg::OnSetFocusName() 
{
	if (m_type == R_NONE)
	{
		m_kind[0]->SetCheck(1);
		m_type = R_MAP;
	}

	m_tree.ClearSelection();
}

void CDownDlg::OnCancel() 
{
	m_wsock->Close();
	CDialog::OnCancel();
}

void CDownDlg::OnOK() 
{
	if (m_finish)
	{
		Sleep(500);
		EndDialog(IDOK);
		return;
	}

	UpdateData(TRUE);
	m_count = m_tree.GetSelectedItemCount();
	if (((m_count == 0 || m_index >= m_count) && m_name.IsEmpty()) || m_type == R_NONE)
		return;

	m_index = 0;
	sendData(false);
}

void CDownDlg::loadFile()
{
	CString	dir = m_dir + "\\" + DN_FILE;
	struct _mapTree	mapTree;
	m_treeArr.RemoveAll();
	
	TRY
	{
		CFile file(dir, CFile::modeRead);
		UINT nBytesRead;
		do
		{
			nBytesRead = file.Read(&mapTree, sizeof(mapTree));
			if (nBytesRead == sizeof(mapTree))
				m_treeArr.Add(mapTree);
		}while((int)nBytesRead);
		file.Close();
	}CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	for (int ii = 0; ii < m_treeArr.GetSize(); ii++)
	{
		insert(m_treeArr.GetAt(ii).mapn, ii);
	}
	SortChildren(NULL);

	GetDlgItem(IDC_NAME)->SetFocus();
}

void CDownDlg::load(int type, CString dir)
{
	char	wb[4*1024];
	CString	tmps;
	CString	section;
	CStringArray arr;

	CfileSET*	fSET;
	CvirtualSET*	vSET;
	CsubSET*	sSET;

	switch (type)
	{
	case R_MAP:
		section = MAPEN;	break;
	case R_UOB:
		section = UOBEN;	break;
	case R_NONE:
	default:
		return;
	}

	DWORD	dwRc = GetPrivateProfileSection(section, wb, sizeof(wb), dir);
	if (!dwRc)
		return;

	parse(wb, dwRc, arr);
	for (int ii = 0; ii < arr.GetSize(); ii++)
	{
		tmps = arr.GetAt(ii);
		tmps.TrimLeft(); tmps.TrimRight();
		if (tmps.IsEmpty())
			continue;

		dwRc = GetPrivateProfileSection(tmps, wb, sizeof(wb), dir);
		if (!dwRc)
			continue;

		sSET = new CsubSET;
		sSET->m_name = tmps;

		CStringArray	sarr;
		parse(wb, dwRc, sarr, sSET->m_name);
		for (int jj = 0; jj < sarr.GetSize(); jj++)
		{
			tmps = sarr.GetAt(jj);
			tmps.TrimLeft(); tmps.TrimRight();
			if (tmps.IsEmpty())
				continue;

			dwRc = GetPrivateProfileSection(tmps, wb, sizeof(wb), dir);
			if (!dwRc)
				continue;

			vSET = new CvirtualSET;
			vSET->m_name = tmps;

			CStringArray	varr;
			parse(wb, dwRc, varr, vSET->m_name);
			for (int kk = 0; kk < varr.GetSize(); kk++)
			{
				tmps = varr.GetAt(kk);
				tmps.TrimLeft(); tmps.TrimRight();
				if (tmps.IsEmpty())
					continue;

				dwRc = GetPrivateProfileSection(tmps, wb, sizeof(wb), dir);
				if (!dwRc)
					continue;

				fSET = new CfileSET;
				fSET->m_name = tmps;

				CStringArray	farr;
				parse(wb, dwRc, farr, fSET->m_name);
				for (int ll = 0; ll < farr.GetSize(); ll++)
				{
					tmps = farr.GetAt(ll);
					tmps.TrimLeft(); tmps.TrimRight();
					if (tmps.IsEmpty())
						continue;

					fSET->m_arFILE.Add(tmps);
				}
				vSET->m_arVIR.Add(fSET);
			}
			sSET->m_arSUB.Add(vSET);
		}
		if (type == R_MAP)
			m_arMAP.Add(sSET);
		else
			m_arUOB.Add(sSET);
	}
}

void CDownDlg::parse(char* buf, int bufL, CStringArray& arr, CString section, char ch)
{
	CString	tmps;

	arr.RemoveAll();
	for ( ; bufL > 0; )
	{
		tmps  = buf;
		buf  += tmps.GetLength();
		bufL -= tmps.GetLength();
		buf++; bufL--;

		if (ch != NULL)
		{
			int pos = tmps.Find(ch);
			if (pos >= 0)
				tmps = tmps.Left(pos);
		}
		if (!section.IsEmpty())
			tmps.Insert(0, section);
		arr.Add(tmps);
	}
}

void CDownDlg::insert(int type, HTREEITEM hitem)
{
	HTREEITEM	mitem, sitem, vitem, fitem;
	CsubSET*	sSET;
	CvirtualSET*	vSET;
	CfileSET*	fSET;
	CString		tmps;
	int		count, index;

	if (type == R_MAP)
	{
		count = m_arMAP.GetSize();
		index = 2;		// bitmap index
	}
	else
	{
		count = m_arUOB.GetSize();
		index = 3;
	}

	for (int ii = 0; ii < count; ii++)
	{
		if (type == R_MAP)
			sSET = m_arMAP.GetAt(ii);
		else
			sSET = m_arUOB.GetAt(ii);

		mitem = m_tree.InsertItem(sSET->m_name, 0, 1, hitem);
		m_tree.SetItemData(mitem, R_NONE);
		for (int jj = 0; jj < sSET->m_arSUB.GetSize(); jj++)
		{
			vSET  = sSET->m_arSUB.GetAt(jj);
			sitem = m_tree.InsertItem(vSET->m_name, 0, 1, mitem);
			m_tree.SetItemData(sitem, R_NONE);
			for (int kk = 0; kk < vSET->m_arVIR.GetSize(); kk++)
			{
				fSET  = vSET->m_arVIR.GetAt(kk);
				vitem = m_tree.InsertItem(fSET->m_name, 0, 1, sitem);
				m_tree.SetItemData(vitem, R_NONE);
				for (int mm = 0; mm < fSET->m_arFILE.GetSize(); mm++)
				{
					tmps = fSET->m_arFILE.GetAt(mm);
					fitem = m_tree.InsertItem(tmps, index, index, vitem);
					if (m_type == type && m_path == tmps)
						m_item = fitem;
					m_tree.SetItemData(fitem, type);
				}
			}
		}
	}
}

HTREEITEM CDownDlg::getSelectedItem(int index)
{
	if (m_tree.m_arItem.GetSize() <= 0 || index > m_tree.m_arItem.GetUpperBound())
		return (HTREEITEM) NULL;

	HTREEITEM hitem = m_tree.m_arItem.GetAt(index);
	int	type = m_tree.GetItemData(hitem);
	if (m_tree.m_arItem.GetUpperBound() > 1 && (!(type == R_MAP || type == R_UOB)))
		return (HTREEITEM) NULL;

	return hitem;
}

bool CDownDlg::sendData(bool req)
{
	HTREEITEM	hitem;

	m_request = req ? true : false;
	m_SRH->dirF = dirF_OUTB;
	if (req)	// download initial data
		m_SRH->resK = resK_REQ;
	else		// directory or filename
	{
		if (m_SRH->resK != resK_RSP)
		{
			hitem = getSelectedItem(m_index);
			if (hitem != (HTREEITEM)NULL)
			{
				m_type = m_tree.GetItemData(hitem);
				m_name = m_tree.GetItemText(hitem);
			}
			if (m_name.IsEmpty())
			{
				finish(false);
				return false;
			}

			/*switch (m_type)
			{
			case R_MAP:
				m_SRH->resK = resK_MAP;		break;
			case R_UOB:
				m_SRH->resK = resK_UOB;		break;
			default:
				finish(false);
				return false;
			}*/

			CChildFrame* child;
			for (int ii = 0; ii < m_mainFRM->m_children.GetSize(); ii++)
			{
				child = m_mainFRM->m_children.GetAt(ii);
				if (child == (CChildFrame *)NULL)
				{
					continue;
				}

				if (!m_name.CompareNoCase(child->m_mapH.mapN))
				{
					CString	tmps;
					if (m_type == R_MAP)
						tmps.Format("\n[%s] 같은 화면이 편집중입니다.\n\n", m_name);
					else
						tmps.Format("\n[%s] 같은 사용자 객체가 편집중입니다.\n\n", m_name);
					MessageBox(tmps, "AXIS Builder", MB_OK);

					finish(false);
					return false;
				}
			}
		}
		else
		{
			m_SRH->resC = m_RRH->resC;
			m_SRH->resF = m_RRH->resF;
		}
		CopyMemory(m_SRH->resN, (char *)m_name.operator LPCTSTR(), m_name.GetLength());
	}

	wccToBUFFER(m_SRH->datL, sizeof(m_SRH->datL), 0);
	m_enable  = false;
	if (!m_wsock->Write(L_rsmH))
	{
		m_enable = true;
		return false;
	}

	return true;
}

void CDownDlg::finish(bool add)
{
	if (m_request)
		return;

	if (add)
	{
		switch (m_type)
		{
		case R_MAP:
			m_path.Format("sform:%s", m_name);	break;
		case R_UOB:
			m_path.Format("uform:%s", m_name);	break;
		default:
			break;
		}
		m_files.Add(m_name);
		m_types.Add((BYTE)m_type);
	}
	m_index++;
	if (m_index < m_count)
	{
		sendData(false);
		return;
	}

	m_finish = true;
	m_wsock->Close();
	OnOK();
}

void CDownDlg::removeAll()
{
	CfileSET*	fileSET;
	CvirtualSET*	virSET;
	CsubSET*	subSET;

	for (int ii = 0; ii < m_arMAP.GetSize(); ii++)
	{
		subSET = m_arMAP.GetAt(ii);
		for (int jj = 0; jj < subSET->m_arSUB.GetSize(); jj++)
		{
			virSET = subSET->m_arSUB.GetAt(jj);
			for (int kk = 0; kk < virSET->m_arVIR.GetSize(); kk++)
			{
				fileSET = virSET->m_arVIR.GetAt(kk);
				fileSET->m_arFILE.RemoveAll();
				delete fileSET;
			}
			virSET->m_arVIR.RemoveAll();
			delete virSET;
		}
		subSET->m_arSUB.RemoveAll();
		delete subSET;
	}
	m_arMAP.RemoveAll();

	for (ii = 0; ii < m_arUOB.GetSize(); ii++)
	{
		subSET = m_arUOB.GetAt(ii);
		for (int jj = 0; jj < subSET->m_arSUB.GetSize(); jj++)
		{
			virSET = subSET->m_arSUB.GetAt(jj);
			for (int kk = 0; kk < virSET->m_arVIR.GetSize(); kk++)
			{
				fileSET = virSET->m_arVIR.GetAt(kk);
				fileSET->m_arFILE.RemoveAll();
				delete fileSET;
			}
			virSET->m_arVIR.RemoveAll();
			delete virSET;
		}
		subSET->m_arSUB.RemoveAll();
		delete subSET;
	}
	m_arUOB.RemoveAll();

	m_files.RemoveAll();
	m_types.RemoveAll();
}

void CDownDlg::goState(int state, CString msg)
{
	m_state = state;
	m_status = " STATUS : ";
	switch (m_state)
	{
	case stADDRESS:
		m_status += "IP 주소가 입력되지 않았습니다.";
		break;
	case stCONNECTING:
		m_status += "연결중입니다.";
		break;
	case stCONNECTED:
		m_status += "연결되었습니다.";
		m_tree.EnableWindow(true);
		GetDlgItem(IDOK)->EnableWindow(true);
		GetDlgItem(IDC_REQ)->EnableWindow(true);
		GetDlgItem(IDC_NAME)->EnableWindow(true);
		break;
	case stWAIT:
		m_state = stCONNECTED;
		m_status += "항목을 선택한 후 확인을 누르시오.";
		m_tree.EnableWindow(true);
		GetDlgItem(IDOK)->EnableWindow(true);
		GetDlgItem(IDC_REQ)->EnableWindow(true);
		GetDlgItem(IDC_NAME)->EnableWindow(true);
		break;
	case stRECEIVING:
		m_state = stCONNECTED;
		m_status += "Data를 Download 받고 있습니다.";
		m_tree.EnableWindow(false);
		GetDlgItem(IDOK)->EnableWindow(false);
		GetDlgItem(IDC_REQ)->EnableWindow(false);
		GetDlgItem(IDC_NAME)->EnableWindow(false);
		break;
	case stIDLE:
	default:
		m_status += "연결이 해제되었습니다.";
		break;
	}

	if (!msg.IsEmpty())
		m_status = msg;

	SetDlgItemText(IDC_STATUS, m_status);
}

void CDownDlg::OnMAP() 
{
	m_type = R_MAP;
}

void CDownDlg::OnUSER() 
{
	m_type = R_UOB;
}

LONG CDownDlg::OnState(WPARAM wParam, LPARAM lParam)
{
	CString	msg = (char *)lParam;
	goState((int)wParam, msg);
	return 0;
}

LONG CDownDlg::OnReadData(WPARAM wParam, LPARAM lParam)
{
	CString	tmps;
	int	datL;

	if (m_state != stCONNECTED)
		return 0;

	m_enable = true;
	tmps = CString(m_RRH->datL, sizeof(m_RRH->datL));
	datL = atoi(tmps);
	if (m_RRH->resK == resK_ERR)
	{
		MessageBox(&m_wsock->m_recvB[L_rsmH], "ERROR !!", MB_OK);
//		goState(stIDLE);
		return 0;
	}

	/*switch (m_RRH->resK)
	{
	case resK_MAP:
		m_type = R_MAP;		break;
	case resK_UOB:
		m_type = R_UOB;		break;
	default:
		break;
	}*/

	if (m_RRH->resC & resC_ASCII)
	{
		tmps = _T("");
		switch (m_RRH->resF)
		{
		case resF_FIR:
			if (m_request)
                		wccSetTempPath(tmps, DN_FILE);
			else
                		wccSetTempPath(tmps, m_RRH->resN);

			if (!m_file.Open(tmps, CFile::modeCreate|CFile::modeWrite))
				AfxMessageBox("File Create Fail");
    			else 
			{
				m_file.Write(&m_wsock->m_recvB[L_rsmH], datL);
				goState(stRECEIVING);
    			}
			break;

		case resF_MID:
			m_file.Write(&m_wsock->m_recvB[L_rsmH], datL);
			goState(stRECEIVING);
			break;

		case resF_LAS:
			m_file.Write(&m_wsock->m_recvB[L_rsmH], datL);
			m_file.Close();
			goState(stCONNECTED);
			finish();
			break;

		case resF_ONLY:
			if (m_request)
                		wccSetTempPath(tmps, DN_FILE);
			else
                		wccSetTempPath(tmps, m_RRH->resN);

			if (!m_file.Open(tmps, CFile::modeCreate|CFile::modeWrite))
				AfxMessageBox("File Create Fail");
			else
			{
				m_file.Write(&m_wsock->m_recvB[L_rsmH], datL);
				m_file.Close();
				goState(stCONNECTED);
				finish();
			}
			break;
		}

		if (m_RRH->resF == resF_FIR || m_RRH->resF == resF_MID)
		{
			m_SRH->resK = resK_RSP;
			sendData(m_request ? true : false);
		}
		else
		{
			if (m_request)
				loadFile();
		}
	}

	return 0;
}

bool CDownDlg::checkDuplicate(CString fileN, int type)
{
	CChildFrame* child;
	for (int ii = 0; ii < m_mainFRM->m_children.GetSize(); ii++)
	{
		child = m_mainFRM->m_children.GetAt(ii);
		if (child == (CChildFrame *)NULL)
		{
			continue;
		}

		if (!fileN.CompareNoCase(child->m_mapH.mapN))
			return true;
	}
	return false;
}
