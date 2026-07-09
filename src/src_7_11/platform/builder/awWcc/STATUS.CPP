// status.cpp : implementation file
//

#include "stdafx.h"
#include "awWcc.h"
#include "status.h"
#include "../../h/mapform.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatus dialog


CStatus::CStatus(CString mapN, CWnd* pParent /*=NULL*/)
	: CDialog(CStatus::IDD, pParent)
{
	m_estate = false;
	m_parent = pParent;
	
	//{{AFX_DATA_INIT(CStatus)
	m_mapN  = mapN;
	m_error = _T("0");
	//}}AFX_DATA_INIT
}


void CStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatus)
	DDX_Control(pDX, IDC_STEP, m_step);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_ERROR, m_error);
	DDX_Text(pDX, IDC_MAPN, m_mapN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatus, CDialog)
	//{{AFX_MSG_MAP(CStatus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatus message handlers

BOOL CStatus::OnInitDialog() 
{
	CDialog::OnInitDialog();

	DWORD	dwStyle = m_list.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_list.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);

	m_bitmap[0] = (CStatic *)GetDlgItem(IDC_BMP1);
	m_bitmap[1] = (CStatic *)GetDlgItem(IDC_BMP2);
	m_bitmap[2] = (CStatic *)GetDlgItem(IDC_BMP3);
	m_bitmap[3] = (CStatic *)GetDlgItem(IDC_BMP4);

	m_msg[0] = (CStatic *)GetDlgItem(IDC_MSG1);
	m_msg[1] = (CStatic *)GetDlgItem(IDC_MSG2);
	m_msg[2] = (CStatic *)GetDlgItem(IDC_MSG3);
	m_msg[3] = (CStatic *)GetDlgItem(IDC_MSG4);

	addColumn("Type or Line", 0, 50);
	addColumn("Name",         1, 50);
	addColumn("Event",        2, 70);
	addColumn("Message",      3, 170);
	showMessage();

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatus::OnOK() 
{
	CDialog::OnOK();
}

void CStatus::SetRange(int lower, int upper)
{
	m_step.SetRange(lower, upper);
}

void CStatus::SetStep(int step)
{
	m_step.SetStep(step);
}

void CStatus::StepIt()
{
	m_step.StepIt();
}

void CStatus::SetMessage(int err, int kind, CString name, CString event, int message, CString param, int line)
{
	CString	tmps;

	m_estate = true;
	showMessage(false);

	int	item = m_list.GetItemCount();
	tmps = _T("");
	if (kind == -1)
	{
		if (line > 0)
			tmps.Format("%d", line);
	}
	else
		tmps = getFormKind(kind);
	
	addItem(tmps,  item, 0);
	addItem(name,  item, 1);
	addItem(event, item, 2);
	tmps = getMessage(message, param);
	addItem(tmps,  item, 3);
	m_error.Format("%d", err);
	UpdateData(FALSE);
}

void CStatus::SetTotalError(int error)
{
	m_error.Format("%d", error);
	UpdateData(FALSE);
}

void CStatus::ShowBitmap(int idx, bool show)
{
	if (m_estate)
		show = false;

	if (idx < 0)
	{
		for (int ii = 0; ii < maxMSGs; ii++)
			m_bitmap[ii]->ShowWindow(show ? SW_SHOW : SW_HIDE);
		return;
	}

	m_bitmap[idx]->ShowWindow(show ? SW_SHOW : SW_HIDE);
}

void CStatus::showMessage(bool show)
{
	for (int ii = 0; ii < maxMSGs; ii++)
	{
		m_msg[ii]->ShowWindow(show ? SW_SHOW : SW_HIDE);
		m_bitmap[ii]->ShowWindow(show ? SW_SHOW : SW_HIDE);
	}

	m_list.ShowWindow(show ? SW_HIDE : SW_SHOW);
}

BOOL CStatus::addColumn(LPCTSTR str, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR) str;
	lvc.cx       = m_list.GetStringWidth(lvc.pszText) + width;
	lvc.iSubItem = -1;

	return m_list.InsertColumn(item, &lvc);
}

BOOL CStatus::addItem(CString& str, int item, int subItem)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subItem;
	lvi.pszText  = (LPTSTR) str.operator LPCTSTR();

	if (subItem == 0)
		return m_list.InsertItem(&lvi);
	return m_list.SetItem(&lvi);
}

CString CStatus::getFormKind(int kind)
{
	switch (kind)
	{
	case FM_SYS:
		return _T("SYS");
	case FM_LABEL:
		return _T("LABEL");
	case FM_BOX:
		return _T("BOX");
	case FM_GROUP:
		return _T("GROUP");
	case FM_PANEL:
		return _T("PANEL");
	case FM_BUTTON:
		return _T("BUTTON");
	case FM_EDIT:
		return _T("EDIT");
	case FM_COMBO:
		return _T("COMBO");
	case FM_OUT:
		return _T("OUTPUT");
	case FM_MEMO:
		return _T("MEMO");
	case FM_GRID:
		return _T("GRID");
	case FM_TABLE:
		return _T("TABLE");
	case FM_TREE:
		return _T("TREEVIEW");
	case FM_AVI:
		return _T("AVI");
//	case FM_GRAPH:
//		return _T("GRAPH");
	case FM_OBJECT:
		return _T("OBJECT");
	case FM_TAB:
		return _T("TAB");
	case FM_UTAB:
		return _T("USERTAB");
	case FM_SHEET:
		return _T("SHEET");
	case FM_BROWSER:
		return _T("BROWSER");
	default:
		return _T("");
	}
}

#define	E_TYPE0		0	// only message
#define	E_TYPE1		1	// param + message

const struct	_errList {
	int	etype;
	char	msg[60];
public:
	CString getInfoFile(CString sSection, CString sKey, CString sDefault, CString sPath);
} errList[] = {
	{ E_TYPE0,  "USAGE AXISMAP AXISMAP_FILENAME"},		// 0
	{ E_TYPE0,  "Invalid MAP_FILE NAME"},			// 1
	{ E_TYPE0,  "Can't find MAP_FILE_SRC"},			// 2
	{ E_TYPE1,  "Invalid MAP SYNTAX"},			// 3
	{ E_TYPE0,  "Memory allocation Error"},			// 4
	{ E_TYPE0,  "Can't find LEADING CHARACTER"},		// 5
	{ E_TYPE0,  "Duplicate [BEGIN] keyword."},		// 6
	{ E_TYPE0,  "Unknown Error."},				// 7
	{ E_TYPE0,  "Duplicate [(] keyword."},			// 8
	{ E_TYPE0,  "Duplicate [)] keyword."},			// 9
	{ E_TYPE0,  "[|] is Reserved keyword."},		// 10
	{ E_TYPE0,  "[''] is Reserved keyword."},		// 11
	{ E_TYPE0,  "[""] is Reserved keyword."},		// 12
	{ E_TYPE1,  "Unknown syntax."},				// 13
	{ E_TYPE0,  "Can't create OBJECT FILE"},		// 14
	{ E_TYPE0,  "Insufficient Numeric FIELD"},		// 15
	{ E_TYPE0,  "Invalid FORMAT"},				// 16
	{ E_TYPE1,  "Unknown DATA TYPE"},			// 17
	{ E_TYPE1,  "Unknown SYMBOL"},				// 18
	{ E_TYPE0,  "[BEGIN-END] syntax Error"},		// 19
	{ E_TYPE0,  "[SCRIPT-ENDSCRIPT] syntax Error."},	// 20
	{ E_TYPE0,  "[IF-ENDIF] syntax Error."},		// 21
	{ E_TYPE0,  "[SWITCH-ENDSWITCH] syntax Error."},	// 22
	{ E_TYPE0,  "Duplicate [END] keyword."},		// 23
	{ E_TYPE0,  "Duplicate [SCRIPT] keyword."},		// 24
	{ E_TYPE0,  "Duplicate [DEFAULT] keyword."},		// 25
	{ E_TYPE0,  "Duplictae [IF] keyword."},			// 26
	{ E_TYPE0,  "Duplicate [ELSE] keyword."},		// 27
	{ E_TYPE0,  "Duplicate [ENDIF] keyword."},		// 28
	{ E_TYPE0,  "Duplicate [SWITCH] keyword"},		// 29
	{ E_TYPE0,  "Duplicate [ENDSWITCH] keyword."},		// 30
	{ E_TYPE0,  "[IF-ELSE] syntax Error."},			// 31
	{ E_TYPE0,  "[SWITCH-CASE] syntax Error."},		// 32
	{ E_TYPE0,  "[IF] syntax Error."},			// 33
	{ E_TYPE1,  "operator syntax Error."},			// 34
	{ E_TYPE1,  "Parameter Error of Method."},		// 35
	{ E_TYPE1,  "Data Type Error."},			// 36
	{ E_TYPE0,  "End of condition syntax Error."},		// 37
	{ E_TYPE0,  "brace is written in syntax."},		// 38
	{ E_TYPE0,  "[IF-ELSE-ENDIF] syntax Error."},		// 39
	{ E_TYPE1,  "Duplicate SYMBOL"},			// 40
	{ E_TYPE1,  "Unknown Method."},				// 41
	{ E_TYPE1,  "Duplicate COLUMN SYMBOL"},			// 42
	{ E_TYPE1,  "Form size is out of bounds."},		// 43
	{ E_TYPE1,  "Number of Method Parameters Error."},	// 44
	{ E_TYPE0,  "THIS keyword is reserved."},		// 45
	{ E_TYPE0,  "syntax Error."},				// 46
	{ E_TYPE0,  "GRAPH Information Error...",},		// 47
	{ E_TYPE0,  "Duplicate [PROCEDURE] keyword."},		// 48
	{ E_TYPE0,  "[PROCEDURE-ENDPROC] syntax Error."},	// 49
	{ E_TYPE0,  "User defined procedure Error."},		// 50
	{  -1,       NULL}
}; 

CString CStatus::getMessage(int message, CString param)
{
	if ((message == -1 || errList[message].etype == -1) && !param.IsEmpty())
		return param;

	CString	err = _T("");
	switch (errList[message].etype)
	{
	case E_TYPE0:
		err = CString(errList[message].msg);
		break;
	case E_TYPE1:
		err.Format("[%s]  %s", param, errList[message].msg);
		break;
	default:
		err = param;
		break;
	}

	return err;
}


