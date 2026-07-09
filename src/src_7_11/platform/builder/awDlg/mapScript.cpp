// mapScript.cpp : implementation file
//

#include "stdafx.h"
#include "mapScript.h"

#include "../mapform.h"
#include "../awWcc/libwcc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapScript property page

IMPLEMENT_DYNCREATE(CMapScript, CPropertyPage)

CMapScript::CMapScript(struct _mapH* mapH, CString curdir)
	: CPropertyPage(CMapScript::IDD)
{
	m_mapH   = mapH;
	m_curdir = curdir;

	//{{AFX_DATA_INIT(CMapScript)
	m_script = _T("");
	//}}AFX_DATA_INIT
}

CMapScript::~CMapScript()
{
}

void CMapScript::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapScript)
	DDX_Control(pDX, IDC_EVENT, m_event);
	DDX_Text(pDX, IDC_SCRIPT, m_script);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapScript, CPropertyPage)
	//{{AFX_MSG_MAP(CMapScript)
	ON_CBN_SELCHANGE(IDC_EVENT, OnSelChangeEvent)
	ON_EN_CHANGE(IDC_SCRIPT, OnChangeScript)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapScript message handlers

BOOL CMapScript::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	bool	set = false;

	if (!set && m_mapH->onStart)
	{
		m_event.SetCurSel(1);
		setScript(m_mapH->onStartN);
		set = true;
	}

	if (!set && m_mapH->onSend)
	{
		m_event.SetCurSel(2);
		setScript(m_mapH->onSendN);
		set = true;
	}

	if (!set && m_mapH->onReceive)
	{
		m_event.SetCurSel(3);
		setScript(m_mapH->onReceiveN);
		set = true;
	}

	if (!set && m_mapH->onAlert)
	{
		m_event.SetCurSel(4);
		setScript(m_mapH->onAlertN);
		set = true;
	}

	if (!set && m_mapH->onKey)
	{
		m_event.SetCurSel(5);
		setScript(m_mapH->onKeyN);
		set = true;
	}

	if (!set && m_mapH->onTrigger)
	{
		m_event.SetCurSel(6);
		setScript(m_mapH->onTriggerN);
		set = true;
	}

	if (!set)
	{
		m_event.SetCurSel(0);
		GetDlgItem(IDC_SCRIPT)->EnableWindow(FALSE);
		m_mapH->onStart   = false;
		m_mapH->onSend    = false;
		m_mapH->onReceive = false;
		m_mapH->onKey     = false;
		m_mapH->onTrigger = false;
	}
	else
		GetDlgItem(IDC_SCRIPT)->EnableWindow(TRUE);

	m_prev = m_event.GetCurSel();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMapScript::OnSelChangeEvent() 
{
	saveScript();

  	m_prev = m_event.GetCurSel();
	GetDlgItem(IDC_SCRIPT)->EnableWindow(TRUE);
	m_script.Empty();

	switch (m_prev)
	{
	case 1:
		setScript(m_mapH->onStartN);	break;
	case 2:
		setScript(m_mapH->onSendN);	break;
	case 3:
		setScript(m_mapH->onReceiveN);	break;
	case 4:
		setScript(m_mapH->onKeyN);	break;
	case 5:
		setScript(m_mapH->onTriggerN);	break;
	default:
		GetDlgItem(IDC_SCRIPT)->EnableWindow(FALSE);
		break;
	}
	UpdateData(FALSE);
}

void CMapScript::OnChangeScript() 
{
	SetModified();	
}

BOOL CMapScript::OnApply() 
{
	saveScript();
	CancelToClose();

	return CPropertyPage::OnApply();
}

void CMapScript::setScript(char* dir)
{
	HANDLE	file;
	DWORD	dwRead = 0;
	DWORD	scpSize;
	char*	sbuf;

	file = CreateFile(dir, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
		return;

	scpSize = GetFileSize(file, NULL);
	if (scpSize == 0xFFFFFFFF)
	{
		CloseHandle(file);
		return;
	}
	sbuf = new char[scpSize + 1];
	BOOL  rc = ReadFile(file, sbuf, scpSize, &dwRead, NULL);
	sbuf[scpSize] = '\0';
	CloseHandle(file);
	if (!rc || scpSize != dwRead)
	{
		delete[] sbuf;
		return;
	}

	insertToEdit(sbuf);
	delete[] sbuf;
}

void CMapScript::insertToEdit(char* buf)
{
	int	len = 0;
	CEdit*	edit;

	m_script.Empty();
	edit = (CEdit *)GetDlgItem(IDC_SCRIPT);
	for (int ii = 0;; ii++, buf++)
	{
		if (*buf == '\n')
		{
			m_script += 0x0d;
			m_script += 0x0a;
		}
		else if (*buf == NULL)
			break;
		else
		{
			if ((*buf != 0x0d && *buf != 0x0a) || *buf == '\t')
				m_script += *buf;
		}
	}
	UpdateData(FALSE);
}

bool CMapScript::saveScript()
{
	UpdateData(TRUE);

	HANDLE	file;
	DWORD	nData = 0;
	CString	dir = _T("");

	m_script.TrimRight();
	switch (m_prev)
	{
	case 1:		// onStart
		if (m_script.GetLength() <= 0)
		{
			if (m_mapH->onStart)
				DeleteFile(m_mapH->onStartN);
			m_mapH->onStartN[0] = '\0';
			m_mapH->onStart = false;
			break;
		}
		if (m_mapH->onStartN[0] == '\0')
		{
			char	prefix[L_SGID+1];
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0';
			wccGetTempName(m_mapH->onStartN, prefix);
		}
		dir = m_mapH->onStartN;
		m_mapH->onStart = true;
		break;

	case 2:		// onSend
		if (m_script.GetLength() <= 0)
		{
			if (m_mapH->onSend)
				DeleteFile(m_mapH->onSendN);
			m_mapH->onSendN[0] = '\0';
			m_mapH->onSend = false;
			break;
		}
		if (m_mapH->onSendN[0] == '\0')
		{
			char	prefix[L_SGID+1];
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			wccGetTempName(m_mapH->onSendN, prefix);
		}
		dir = m_mapH->onSendN;
		m_mapH->onSend = true;
		break;

	case 3:		// onReceive
		if (m_script.GetLength() <= 0)
		{
			if (m_mapH->onReceive)
				DeleteFile(m_mapH->onReceiveN);
			m_mapH->onReceiveN[0] = '\0';
			m_mapH->onReceive = false;
			break;
		}
		if (m_mapH->onReceiveN[0] == '\0')
		{
			char	prefix[L_SGID+1];
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			wccGetTempName(m_mapH->onReceiveN, prefix);
		}
		dir = m_mapH->onReceiveN;
		m_mapH->onReceive = true;
		break;

	case 4:		// onKey
		if (m_script.GetLength() <= 0)
		{
			if (m_mapH->onKey)
				DeleteFile(m_mapH->onKeyN);
			m_mapH->onKeyN[0] = '\0';
			m_mapH->onKey = false;
			break;
		}
		if (m_mapH->onKeyN[0] == '\0')
		{
			char	prefix[L_SGID+1];
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0'; 
			wccGetTempName(m_mapH->onKeyN, prefix);
		}
		dir = m_mapH->onKeyN;
		m_mapH->onKey = true;
		break;

	case 5:		// onTrigger
		if (m_script.GetLength() <= 0)
		{
			if (m_mapH->onTrigger)
				DeleteFile(m_mapH->onTriggerN);
			m_mapH->onTriggerN[0] = '\0';
			m_mapH->onTrigger = false;
			break;
		}
		if (m_mapH->onTriggerN[0] == '\0')
		{
			char	prefix[L_SGID+1];
			CopyMemory(prefix, m_mapH->mapN, L_SGID);
			prefix[L_SGID] = '\0';
			wccGetTempName(m_mapH->onTriggerN, prefix);
		}
		dir = m_mapH->onTriggerN;
		m_mapH->onTrigger = true;
		break;

	default:
		break;
	}

	if (m_script.GetLength() > 0)
	{
		file = CreateFile(dir, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
AfxMessageBox("XError [d2]");
			return false;
		}
		else
		{
			if (!WriteFile(file, (char *)m_script.operator LPCTSTR(), m_script.GetLength(), &nData, NULL))
				AfxMessageBox("XError [d3]");
			CloseHandle(file);
		}
	}

	return true;
}
