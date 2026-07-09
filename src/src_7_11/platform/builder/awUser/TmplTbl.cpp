// TmplTbl.cpp : implementation file
//

#include "stdafx.h"
#include "TmplTbl.h"
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

#define	DN_TMPLFILE	"template"
/////////////////////////////////////////////////////////////////////////////
// CTmplTbl

CTmplTbl::CTmplTbl()
{
	m_wsock = NULL;
	m_request = true;
}

CTmplTbl::~CTmplTbl()
{
	if (m_wsock)
	{
		m_wsock->Close();
		delete m_wsock;
	}
}


BEGIN_MESSAGE_MAP(CTmplTbl, CWnd)
	//{{AFX_MSG_MAP(CTmplTbl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_READDATA, OnReadData)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTmplTbl message handlers

LONG CTmplTbl::OnReadData(WPARAM wParam, LPARAM lParam)
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
		return 0;
	}

	if (m_RRH->resC & resC_ASCII)
	{
		tmps = _T("");
		switch (m_RRH->resF)
		{
		case resF_FIR:
			if (m_request)
                		wccSetTempPath(tmps, DN_TMPLFILE);
			else
                		wccSetTempPath(tmps, "template");//m_RRH->resN);

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
//			finish();
			break;

		case resF_ONLY:
			if (m_request)
                		wccSetTempPath(tmps, DN_TMPLFILE);
			else
                		wccSetTempPath(tmps, "template");//m_RRH->resN);

			if (!m_file.Open(tmps, CFile::modeCreate|CFile::modeWrite))
				AfxMessageBox("File Create Fail");
			else
			{
				m_file.Write(&m_wsock->m_recvB[L_rsmH], datL);
				m_file.Close();
				goState(stCONNECTED);
			}
			break;
		}

		if (m_RRH->resF == resF_FIR || m_RRH->resF == resF_MID)
		{
			m_SRH->resK = resK_RSP;
			sendData(m_request, m_name);
		}
		else
		{
			if (m_request)
				loadFile();
			else
			{
				finish();
				AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_LOADTEMPLATE, MAKELPARAM(m_MousePoint.x, m_MousePoint.y));
			}
		}
	}

	return 0;
}

LONG CTmplTbl::OnState(WPARAM wParam, LPARAM lParam)
{
	CString	msg = (char *)lParam;
	goState((int)wParam, msg);
	return 0;
}	

void CTmplTbl::goState(int state, CString msg)
{
	m_state = state;
	m_status = " STATUS : ";
	switch (m_state)
	{
	case stADDRESS:
		m_status += "IP Address is none.";
		break;
	case stCONNECTING:
		m_status += "Connecting.";
		break;
	case stCONNECTED:
		m_status += "Connected.";
		//EnableWindow(true);
		break;
	case stWAIT:
		m_state = stCONNECTED;
		m_status += "Idle.";
		//EnableWindow(true);
		break;
	case stRECEIVING:
		m_state = stCONNECTED;
		m_status += "Data Downloading.";
		//EnableWindow(false);
		break;
	case stIDLE:
	default:
		m_status += "Disconnected.";
		break;
	}

}

bool CTmplTbl::sendData(bool req, CString name)
{
	m_request = req ? true : false;
	m_SRH->dirF = dirF_OUTB;
	if (req)	// download initial data
	{
		//m_SRH->resK = resK_TMPLINF;
		strcpy_s(m_SRH->resN, 65, DN_TMPLFILE);
	}
	else		// directory or filename
	{
		if (m_SRH->resK != resK_RSP)
		{
			if (!name.IsEmpty())
				m_name = name;
			else if (m_name.IsEmpty())
				return false;
			
			//m_SRH->resK = resK_TMPL;
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

void CTmplTbl::finish()
{
	if (m_wsock)
	{
		m_wsock->Close();
		if (m_wsock)
			delete m_wsock;
		m_wsock = NULL;
	}
}

void CTmplTbl::loadFile()
{
	char twb[6144];
	CString filename, temp;
	wccSetTempPath(filename, DN_TMPLFILE);

	m_groupArr.RemoveAll();
	m_nameArr.RemoveAll();

	GetPrivateProfileString("TEMPLATE", "$NAME", "", twb, 6144, filename);
	temp = twb;
	while (!temp.IsEmpty())
	{
		CString sGroup = Parser(temp, ";"), tmp, sName = "";
		if (sGroup.IsEmpty())
			break;
		m_groupArr.Add(sGroup);
		GetPrivateProfileString("TEMPLATE", sGroup, "", twb, 6144, filename);
		tmp = twb;
		while(!tmp.IsEmpty())
		{
			CString sFName = Parser(tmp, ";"), tmp2;
			if (sFName.IsEmpty())
				break;
			GetPrivateProfileString("TEMPLATE", sFName, "", twb, 6144, filename);
			tmp2 = twb;
			sFName += '\t';
			sFName += Parser(tmp2, ";");
			sName += sFName;
			sName += ';';
		}
		m_nameArr.Add(sName);
	}
	fillCombo(m_pCombo);
	finish();
}

void CTmplTbl::openList(CComboBox *CBO)
{
	m_pCombo = CBO;
	if (m_wsock)
	{
		m_wsock->Close();
		if (m_wsock)
			delete m_wsock;
	}
	m_wsock = new CwSock(this);
	goState(stCONNECTING);
	AfxGetMainWnd()->EnableWindow(FALSE);
	if (!m_wsock->Open())
	{
		AfxGetMainWnd()->EnableWindow(TRUE);
		MessageBox("Can't open Socket", "ERROR", MB_OK);
		if (m_wsock)
		{
			delete m_wsock;
			m_wsock = NULL;
		}
		return;
	}

	AfxGetMainWnd()->EnableWindow(TRUE);
	m_SRH = (struct _rsmH *) &m_wsock->m_sendB[0];
	m_RRH = (struct _rsmH *) &m_wsock->m_recvB[0];

	goState(stCONNECTED);
	if (m_request)
		sendData();
	else
		loadFile();
}

CString CTmplTbl::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CTmplTbl::fillCombo(CComboBox *CBO)
{
	CBO->ResetContent();
	for (int ii = 0; ii < m_groupArr.GetSize(); ii++)
		CBO->SetItemData(CBO->AddString(m_groupArr.GetAt(ii)), ii);
	if (CBO->GetCount())
	{
		CBO->SetCurSel(0);
		GetParent()->PostMessage(WM_COMMAND, MAKEWPARAM(/*ID_WRK_GROUPCBO2*/119, CBN_SELENDOK), (long)this->GetSafeHwnd());
	}
}

void CTmplTbl::fillList(CListCtrl *LC, int idx)
{
	LC->DeleteAllItems();
	CString temp = m_nameArr.GetAt(idx);
	while(!temp.IsEmpty())
	{
		CString tmp = Parser(temp, ";");
		if (tmp.IsEmpty())
			break;
		Parser(tmp, "\t");
		addItem(LC, 0, 0, tmp);
	}
}

BOOL CTmplTbl::addItem(CListCtrl *LC, int item, int subItem, CString &str)
{
	LV_ITEM lvi;

        lvi.mask     = LVIF_TEXT;
        lvi.iItem    = item;
        lvi.iSubItem = subItem;
        lvi.pszText  = (LPTSTR) str.operator LPCTSTR();
        if (subItem == 0)
                return LC->InsertItem(&lvi);

        return LC->SetItem(&lvi);
}

void CTmplTbl::loadTmpl(CPoint pnt)
{
	if (m_wsock)
	{
		m_wsock->Close();
		if (m_wsock)
			delete m_wsock;
	}
	m_wsock = new CwSock(this);
	goState(stCONNECTING);
	AfxGetMainWnd()->EnableWindow(FALSE);
	if (!m_wsock->Open())
	{
		AfxGetMainWnd()->EnableWindow(TRUE);
		MessageBox("Socket을 open하는 중 Error가 발생하였습니다.", "ERROR", MB_OK);
		if (m_wsock)
		{
			delete m_wsock;
			m_wsock = NULL;
		}
		return;
	}

	AfxGetMainWnd()->EnableWindow(TRUE);
	m_SRH = (struct _rsmH *) &m_wsock->m_sendB[0];
	m_RRH = (struct _rsmH *) &m_wsock->m_recvB[0];

	goState(stCONNECTED);
	sendData(false);
	m_MousePoint = pnt;
}

void CTmplTbl::setFileName(int idx, CString name)
{
	CString temp = m_nameArr.GetAt(idx);
	temp = Parser(temp, name + ";");
	if (temp.ReverseFind(';') == -1)
		m_name = Parser(temp, "\t");
	else
		m_name = Parser(temp.Mid(temp.ReverseFind(';') + 1), "\t");
}
