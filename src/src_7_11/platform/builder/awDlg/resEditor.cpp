// resEditor.cpp : implementation file
//

#include "stdafx.h"
#include "resEditor.h"

#include "../mapform.h"
#include "../awWcc/libwcc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResEditor dialog

CResEditor::CResEditor(CWnd* pParent, struct _mapH* mapH, struct _formR* formR, CString curdir)
	: CDialog(CResEditor::IDD, pParent)
{
	m_mapH  = mapH;
	m_formR = formR;
	m_curdir = curdir;

	//{{AFX_DATA_INIT(CResEditor)
	m_script = _T("");
	//}}AFX_DATA_INIT

	m_click   = _T("");
	m_dblclk  = _T("");
	m_change  = _T("");
	m_char    = _T("");
}


void CResEditor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResEditor)
	DDX_Control(pDX, IDC_EVENT, m_event);
	DDX_Text(pDX, IDC_SCRIPT, m_script);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResEditor, CDialog)
	//{{AFX_MSG_MAP(CResEditor)
	ON_CBN_SELCHANGE(IDC_EVENT, OnSelChangeEvent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResEditor message handlers

BOOL CResEditor::OnInitDialog() 
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_SYMBOL)->SetWindowText(m_formR->name);
	switch (m_formR->kind)
	{
	case FM_EDIT:
		m_event.AddString("None");
		m_event.AddString("OnChange");
		m_event.AddString("OnDoubleClick");
		m_event.AddString("OnCharChange");
		break;

	case FM_COMBO:
		m_event.AddString("None");
		m_event.AddString("OnChange");
		break;

	case FM_GRID:
		m_event.AddString("None");
		m_event.AddString("OnClick");
		m_event.AddString("OnDoubleClick");
		m_event.AddString("OnChange");
		break;

	case FM_LABEL:
	case FM_OUT:
	case FM_PANEL:
	case FM_BUTTON:
	case FM_TAB:
	case FM_UTAB:
	case FM_TABLE:
		m_event.AddString("None");
		m_event.AddString("OnClick");
		m_event.AddString("OnDoubleClick");
		break;

	case FM_TREE:
	case FM_SHEET:
		m_event.AddString("None");
		m_event.AddString("OnClick");
		break;

	default:
		break;
	}

	if (m_formR->scpN[0] == NULL)
	{
		m_event.SetCurSel(0);
		GetDlgItem(IDC_SCRIPT)->EnableWindow(FALSE);
		return TRUE;
	}

	HANDLE	file = CreateFile(m_formR->scpN, GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
		m_event.SetCurSel(0);
		GetDlgItem(IDC_SCRIPT)->EnableWindow(FALSE);
		return TRUE;
	}

	DWORD	scpSize = GetFileSize(file, NULL);
	if (scpSize == 0xFFFFFFFF)
	{
		CloseHandle(file);
		m_event.SetCurSel(0);
		GetDlgItem(IDC_SCRIPT)->EnableWindow(FALSE);
		return TRUE;
	}

	char*	scpAlloc;
	scpAlloc = new char[scpSize+1];

	DWORD	dwReadLen = 0;
	BOOL	bRc = ReadFile(file, scpAlloc, scpSize, &dwReadLen, NULL);
	CloseHandle(file);
	if (!bRc || scpSize != dwReadLen)
	{
		m_event.SetCurSel(0);
		GetDlgItem(IDC_SCRIPT)->EnableWindow(FALSE);
		delete[] scpAlloc;
		return TRUE;
	}

	if (m_formR->onClick != -1)
		m_click = CString(&scpAlloc[m_formR->onClick], m_formR->onClickL);

	if (m_formR->onDblClk != -1)
		m_dblclk = CString(&scpAlloc[m_formR->onDblClk], m_formR->onDblClkL);

	if (m_formR->onChange != -1)
		m_change = CString(&scpAlloc[m_formR->onChange], m_formR->onChangeL);

	if (m_formR->onChar != -1)
		m_char = CString(&scpAlloc[m_formR->onChar], m_formR->onCharL);

	delete[] scpAlloc;

	bool	set = false;
	if (!set && !m_click.IsEmpty() && m_formR->kind != FM_COMBO && m_formR->kind != FM_EDIT)
	{
		m_event.SetCurSel(1);
		insertToEdit(m_click);
		set = true;
	}

	if (!set && !m_change.IsEmpty()
		&& (m_formR->kind == FM_COMBO || m_formR->kind == FM_EDIT || m_formR->kind == FM_GRID))
	{
		m_event.SetCurSel((m_formR->kind == FM_GRID) ? 3 : 1);
		insertToEdit(m_change);
		set = true;
	}

	if (!set && !m_dblclk.IsEmpty() && m_formR->kind != FM_COMBO)
	{
		m_event.SetCurSel(2);
		insertToEdit(m_dblclk);
		set = true;
	}

	if (!set && !m_char.IsEmpty() && m_formR->kind == FM_EDIT)
	{
		m_event.SetCurSel(4);
		insertToEdit(m_char);
		set = true;
	}

	if (!set)
	{
		m_event.SetCurSel(0);
		GetDlgItem(IDC_SCRIPT)->EnableWindow(FALSE);
	}

	m_prev = m_event.GetCurSel();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CResEditor::OnSelChangeEvent() 
{
	UpdateData(TRUE);

	m_script.TrimRight();
	switch (m_prev)
	{
	case 1:		// click or change
		switch (m_formR->kind)
		{
		case FM_EDIT:
		case FM_COMBO:
			m_formR->onChangeL = m_script.GetLength();
			m_change = m_script;
			break;
		case FM_LABEL:
		case FM_OUT:
		case FM_PANEL:
		case FM_BUTTON:
		case FM_GRID:
		case FM_TAB:
		case FM_UTAB:
		case FM_TREE:
		case FM_TABLE:
		case FM_SHEET:
			m_formR->onClickL = m_script.GetLength();
			m_click = m_script;
			break;
		default:
			break;
		}
		break;

	case 2:		// dblclk
		switch (m_formR->kind)
		{
		case FM_EDIT:
		case FM_LABEL:
		case FM_OUT:
		case FM_PANEL:
		case FM_BUTTON:
		case FM_GRID:
		case FM_TAB:
		case FM_UTAB:
		case FM_TABLE:
			m_formR->onDblClkL = m_script.GetLength();
			m_dblclk = m_script;
			break;
		default:
			break;
		}
		break;

	case 3:		// char or change
		switch (m_formR->kind)
		{
		case FM_EDIT:
			m_formR->onCharL = m_script.GetLength();
			m_char = m_script;
			break;

		case FM_GRID:
			m_formR->onChangeL = m_script.GetLength();
			m_change = m_script;
			break;
		}
		break;
	case 0:
	default:
		break;
	}

	m_prev = m_event.GetCurSel();
	switch (m_prev)
	{
	case 1:		// click or change
		switch (m_formR->kind)
		{
		case FM_EDIT:
		case FM_COMBO:
			GetDlgItem(IDC_SCRIPT)->EnableWindow(TRUE);
			m_script.Empty();
			insertToEdit(m_change);
			break;
		case FM_LABEL:
		case FM_OUT:
		case FM_PANEL:
		case FM_BUTTON:
		case FM_GRID: 
		case FM_TAB:
		case FM_UTAB:
		case FM_TREE:
		case FM_TABLE:
		case FM_SHEET:
			GetDlgItem(IDC_SCRIPT)->EnableWindow(TRUE);
			m_script.Empty();
			insertToEdit(m_click);
			break;
		default:
			break;
		}
		break;

	case 2: 	// dblclk
		switch (m_formR->kind)
		{
		case FM_EDIT:
		case FM_LABEL:
		case FM_OUT:
		case FM_PANEL:
		case FM_BUTTON:
		case FM_GRID: 
		case FM_TAB:
		case FM_UTAB:
		case FM_TABLE:
			GetDlgItem(IDC_SCRIPT)->EnableWindow(TRUE);
			m_script.Empty();
			insertToEdit(m_dblclk);
			break;
		default:
			break;
		}
		break;

	case 3:		// char or change
		switch (m_formR->kind)
		{
		case FM_EDIT:
			GetDlgItem(IDC_SCRIPT)->EnableWindow(TRUE);
			m_script.Empty();
			insertToEdit(m_char);
			break;
		case FM_GRID:
			GetDlgItem(IDC_SCRIPT)->EnableWindow(TRUE);
			m_script.Empty();
			insertToEdit(m_change);
			break;
		}
		break;

	case 0:
	default:
		m_script.Empty();
		GetDlgItem(IDC_SCRIPT)->EnableWindow(FALSE);
		break;
	}

	UpdateData(FALSE);
}

void CResEditor::insertToEdit(CString str)
{
	m_script.Empty();
	for (int ii = 0; ii < str.GetLength(); ii++)
	{
		if (str[ii] == '\n')
		{
			m_script += 0x0d;
			m_script += 0x0a;
		}
		else if (str[ii] == '\0')
			break;
		else
		{
			if ((str[ii] != 0x0d && str[ii] != 0x0a) || str[ii] == '\t')
				m_script += str[ii];
		}
	}

	UpdateData(FALSE);
}

void CResEditor::OnOK() 
{
	UpdateData(TRUE);

	switch (m_prev)
	{
	case 1:		// click or chabge
		switch (m_formR->kind)
		{
		case FM_EDIT:
		case FM_COMBO:
			m_formR->onChangeL = m_script.GetLength();
			m_change = m_script;
			break;
		case FM_PANEL:
		case FM_OUT:
		case FM_LABEL:
		case FM_BUTTON:
		case FM_GRID:
		case FM_TAB:
		case FM_UTAB:
		case FM_TREE:
		case FM_TABLE:
		case FM_SHEET:
			m_formR->onClickL = m_script.GetLength();
			m_click = m_script;
			break;
		default:
			break;
		}
		break;

	case 2:		// dblclk
		switch (m_formR->kind)
		{
		case FM_EDIT:
		case FM_PANEL:
		case FM_OUT:
		case FM_LABEL:
		case FM_BUTTON:
		case FM_GRID:
		case FM_TAB:
		case FM_UTAB:
		case FM_TABLE:
			m_formR->onDblClkL = m_script.GetLength();
			m_dblclk = m_script;
			break;
		default:
			break;
		}
		break;

	case 3:		// char or change
		switch (m_formR->kind)
		{
		case FM_EDIT:
			m_formR->onCharL = m_script.GetLength();
			m_char = m_script;
			break;
		case FM_GRID:
			m_formR->onChangeL = m_script.GetLength();
			m_change = m_script;
			break;
		}
		break;

	case 0:
	default:
		break;
	}


// write file
	if (m_click.IsEmpty() && (m_formR->onClickL == 0)
		&& m_dblclk.IsEmpty() && (m_formR->onDblClkL == 0)
		&& m_change.IsEmpty() && (m_formR->onChangeL == 0)
		&& m_char.IsEmpty() && (m_formR->onCharL == 0))
	{
		DeleteFile(m_formR->scpN);
		CDialog::OnOK();
		return;
	}

	if (m_formR->scpN[0] == NULL)
	{
		char	prefix[L_SGID+1];
		CopyMemory(prefix, m_mapH->mapN, L_SGID);
		prefix[L_SGID] = '\0'; 
		wccGetTempName(m_formR->scpN, prefix);
	}

	HANDLE file = CreateFile(m_formR->scpN, GENERIC_WRITE, FILE_SHARE_WRITE,
				NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
	{
AfxMessageBox("XError [d1]");
		CDialog::OnOK();
		return;
	}

	DWORD	nWritten;
	int	offs = 0;
	if (m_formR->onClickL != 0 && !m_click.IsEmpty())
	{
		nWritten = 0;
		if (WriteFile(file, m_click, m_click.GetLength(), &nWritten, NULL) != 1)
			MessageBox("Write Fail", "", MB_OK);
		else
		{
			m_formR->onClick = offs;
			offs += nWritten;
		}
	}

	if (m_formR->onDblClkL != 0 && !m_dblclk.IsEmpty())
	{
		nWritten = 0;
		if (WriteFile(file, m_dblclk, m_dblclk.GetLength(), &nWritten, NULL) != 1)
			MessageBox("Write Fail", "", MB_OK);
		else
		{
			m_formR->onDblClk = offs;
			offs += nWritten;
		}
	}

	if (m_formR->onChangeL != 0 && !m_change.IsEmpty())
	{
		nWritten = 0;
		if (WriteFile(file, m_change, m_change.GetLength(), &nWritten, NULL) != 1)
			MessageBox("Write Fail", "", MB_OK);
		else
		{
			m_formR->onChange = offs;
			offs += nWritten;
		}
	}

	if (m_formR->onCharL != 0 && !m_char.IsEmpty())
	{
		nWritten = 0;
		if (WriteFile(file, m_char, m_char.GetLength(), &nWritten, NULL) != 1)
			MessageBox("Write Fail", "", MB_OK);
		else
		{
			m_formR->onChar = offs;
			offs += nWritten;
		}
	}

	CloseHandle(file);
	CDialog::OnOK();
}
