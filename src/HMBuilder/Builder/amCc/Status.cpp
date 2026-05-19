// Status.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "amCc.h"
#include "Status.h"
#include "afxdialogex.h"


// CStatus 대화 상자입니다.

IMPLEMENT_DYNAMIC(CStatus, CDialogEx)

CStatus::CStatus(CString mapN, CWnd* pParent /*=NULL*/)
	: CDialogEx(CStatus::IDD, pParent)
{

	m_error = _T("");
	m_mapN = _T("");

	m_mapN  = mapN;
	m_error = _T("0");
}

CStatus::~CStatus()
{
}

void CStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STEP, m_step);
	DDX_Text(pDX, IDC_ERROR, m_error);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_MAPN, m_mapN);
}


BEGIN_MESSAGE_MAP(CStatus, CDialogEx)
END_MESSAGE_MAP()


// CStatus 메시지 처리기입니다.


BOOL CStatus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	case FM_LABEL:
		return _T("LABEL");
	case FM_BOX:
		return _T("BOX");
	case FM_GROUP:
		return _T("GROUP");
	case FM_IMAGEVIEW:
		return _T("IMAGEVW");
	case FM_BUTTON:
		return _T("BUTTON");
	case FM_CHECK:
		return _T("CHECK");
	case FM_RADIO:
		return _T("RADIO");
	case FM_EDIT:
		return _T("EDIT");
	case FM_COMBO:
		return _T("COMBO");
	case FM_OUT:
		return _T("OUTPUT");
	case FM_GRID:
		return _T("GRID");
	case FM_GRIDEX:
		return _T("GRIDEX");
	case FM_TABLE:
		return _T("TABLE");
	case FM_OBJECT:
		return _T("OBJECT");
	case FM_TAB:
		return _T("TAB");
	case FM_BROWSER:
		return _T("BROWSER");
	case FM_CONTROL:
		return _T("CONTROL");
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
	{ E_TYPE0,  _T("USAGE AXISMAP AXISMAP_FILENAME")},		// 0
	{ E_TYPE0,  _T("MAP_FILE NAME이 유효하지 않습니다")},		// 1
	{ E_TYPE0,  _T("MAP_FILE_SRC가 존재하지 않습니다")},		// 2
	{ E_TYPE1,  _T("MAP SYNTAX가 유효하지 않습니다")},		// 3
	{ E_TYPE0,  _T("메모리를 할당 할 수 없습니다")},		// 4
	{ E_TYPE0,  _T("LEADING CHARACTER가 없습니다")},		// 5
	{ E_TYPE0,  _T("[BEGIN] 구분자가 중복되었습니다")},		// 6
	{ E_TYPE0,  _T("알 수 없는 에러입니다")},			// 7
	{ E_TYPE0,  _T("[(] 구분자가 중복되었습니다")},			// 8
	{ E_TYPE0,  _T("[)] 구분자가 중복되었습니다")},			// 9
	{ E_TYPE0,  _T("[|] 구분자는 사용할수없습니다")},		// 10
	{ E_TYPE0,  _T("[''] 구분자는 사용할수없습니다")},		// 11
	{ E_TYPE0,  _T("[""] 구분자는 사용할수없습니다")},		// 12
	{ E_TYPE1,  _T("알 수 없는 문자(열)입니다")},			// 13
	{ E_TYPE0,  _T("OBJECT FILE을 생성할수 없습니다")},		// 14
	{ E_TYPE0,  _T("숫자 FIELD가 부족합니다")},			// 15
	{ E_TYPE0,  _T("사용할 수 없는 FORMAT입니다")},			// 16
	{ E_TYPE1,  _T("알수 없는 DATA TYPE입니다")},			// 17
	{ E_TYPE1,  _T("알수 없는 SYMBOL입니다")},			// 18
	{ E_TYPE0,  _T("[BEGIN-END] 구문 에러입니다.")},			// 19
	{ E_TYPE0,  _T("[SCRIPT-ENDSCRIPT] 구문 에러입니다.")},		// 20
	{ E_TYPE0,  _T("[IF-ENDIF] 구문 에러입니다.")},			// 21
	{ E_TYPE0,  _T("[SWITCH-ENDSWITCH] 구문 에러입니다.")},		// 22
	{ E_TYPE0,  _T("[END] 구분자가 중복되었습니다")},		// 23
	{ E_TYPE0,  _T("[SCRIPT] 구분자가 중복되었습니다")},		// 24
	{ E_TYPE0,  _T("[DEFAULT] 구분자가 중복되었습니다")},		// 25
	{ E_TYPE0,  _T("[IF] 구분자가 중복되었습니다")},		// 26
	{ E_TYPE0,  _T("[ELSE] 구분자가 중복되었습니다")},		// 27
	{ E_TYPE0,  _T("[ENDIF] 구분자가 중복되었습니다")},		// 28
	{ E_TYPE0,  _T("[SWITCH] 구분자가 중복되었습니다")},		// 29
	{ E_TYPE0,  _T("[ENDSWITCH] 구분자가 중복되었습니다")},		// 30
	{ E_TYPE0,  _T("[IF-ELSE] 구문 에러입니다.")},			// 31
	{ E_TYPE0,  _T("[SWITCH-CASE] 구문 에러입니다.")},		// 32
	{ E_TYPE0,  _T("[IF] 조건문 에러입니다.")},			// 33
	{ E_TYPE1,  _T("연산자 에러입니다.")},				// 34
	{ E_TYPE1,  _T("Method의 Parameter 오류입니다.")},		// 35
	{ E_TYPE1,  _T("Data Type 오류입니다.")},			// 36
	{ E_TYPE0,  _T("조건문 종료 오류입니다.")},			// 37
	{ E_TYPE0,  _T("괄호()의 수가 일치하지 않습니다.")},		// 38
	{ E_TYPE0,  _T("[IF-ELSE-ENDIF] 구문 에러입니다.")},		// 39
	{ E_TYPE1,  _T("SYMBOL이 중복되었습니다")},			// 40
	{ E_TYPE1,  _T("해당 FORM에 정의되지 않은 Method입니다.")},	// 41
	{ E_TYPE1,  _T("COLUMN SYMBOL이 중복되었습니다")},		// 42
	{ E_TYPE1,  _T("해당 Form이 화면 영역을 Over하였습니다")},	// 43
	{ E_TYPE1,  _T("Method의 Parameter 갯수 오류입니다.")},		// 44
	{ E_TYPE0,  _T("THIS를 사용할 수 없습니다.")},			// 45
	{ E_TYPE0,  _T("구문 에러입니다.")},				// 46
	{ E_TYPE0,  _T("GRAPH 정보설정 에러..."),},			// 47
	{ E_TYPE0,  _T("[PROCEDURE] 구분자가 중복되었습니다")},		// 48
	{ E_TYPE0,  _T("[PROCEDURE-ENDPROC] 구문 에러입니다.")},		// 49
	{ E_TYPE0,  _T("사용자 함수호출 에러입니다.")},			// 50
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
