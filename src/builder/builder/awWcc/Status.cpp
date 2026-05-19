// Status.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "awWcc.h"
#include "Status.h"
#include "afxdialogex.h"


// CStatus 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStatus, CDialogEx)

CStatus::CStatus(CString mapN, CWnd* pParent /*=NULL*/)
	: CDialogEx(CStatus::IDD, pParent)
{
	m_mapN = mapN;

	m_estate = false;
	m_parent = pParent;
}

CStatus::~CStatus()
{
}

void CStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STEP, m_step);
	DDX_Control(pDX, IDC_LIST, m_list);
}


BEGIN_MESSAGE_MAP(CStatus, CDialogEx)
END_MESSAGE_MAP()


// CStatus 메시지 처리기입니다.


BOOL CStatus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	DWORD	dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_list.SetExtendedStyle(dwStyle);

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

	GetDlgItem(IDC_MAPN)->SetWindowTextA(m_mapN);
	GetDlgItem(IDC_ERROR)->SetWindowTextA(_T("0"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CStatus::OnOK()
{
	CDialogEx::OnOK();
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

	tmps.Format("%d", err);
	GetDlgItem(IDC_ERROR)->SetWindowTextA(tmps);
}

void CStatus::SetTotalError(int error)
{
	CString	tmps;

	tmps.Format("%d", error);
	GetDlgItem(IDC_ERROR)->SetWindowTextA(tmps);
}

void CStatus::ShowBitmap(int idx, bool show)
{
	if (m_estate)
		show = false;

	if (idx < 0)
	{
		for (int ii = 0; ii < nMSG; ii++)
			m_bitmap[ii]->ShowWindow(show ? SW_SHOW : SW_HIDE);
		return;
	}

	m_bitmap[idx]->ShowWindow(show ? SW_SHOW : SW_HIDE);
}

void CStatus::showMessage(bool show)
{
	for (int ii = 0; ii < nMSG; ii++)
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
	case FM_GRIDEX:
		return _T("GRIDEX");
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
} errList[] = {
	{ E_TYPE0,  "Memory allocation Error"},			// 4	0
	{ E_TYPE0,  "Duplicate [BEGIN] keyword."},		// 6	1
	{ E_TYPE0,  "Unknown Error."},				// 7	2
	{ E_TYPE0,  "Duplicate [(] keyword."},			// 8	3
	{ E_TYPE0,  "Duplicate [)] keyword."},			// 9	4
	{ E_TYPE0,  "[''] is Reserved keyword."},		// 11	5
	{ E_TYPE1,  "Unknown syntax."},				// 13	6
	{ E_TYPE0,  "Can't create OBJECT FILE"},		// 14	7
	{ E_TYPE0,  "Invalid FORMAT"},				// 16	8
	{ E_TYPE1,  "Unknown DATA TYPE"},			// 17	9
	{ E_TYPE1,  "Unknown SYMBOL"},				// 18	10
	{ E_TYPE0,  "[BEGIN-END] syntax Error"},		// 19	11
	{ E_TYPE1,  "Data Type Error."},			// 36	12
	{ E_TYPE1,  "Duplicate SYMBOL"},			// 40	13
	{ E_TYPE1,  "Duplicate COLUMN SYMBOL"},			// 42	14
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