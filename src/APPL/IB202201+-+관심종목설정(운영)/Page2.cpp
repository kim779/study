// Page2.cpp : implementation file
//

#include "stdafx.h"
#include "IB202201.h"
#include "Page2.h"
#include "sheet.h"
#include "CDlg_Slider.h"

#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "TrSearch1.h"

//#include "../../../dll/sm/TrSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	saveFILE	"interField.ini"
#define	tempFILE	"interField.ini.tmp"

struct	_fieldInfo {
	UINT	stid;
	char*	symb;
} fieldInfo[] = {
	{IDS_GH_SIGN,		"9###"},		// 필수
	{IDS_GH_CODE,		"1301"},		// 필수
	{IDS_GH_NAME,		"1022"},		// 필수
	{IDS_GH_CURR,		"2023"},		// 필수
	{IDS_GH_DIFF,		"2024"},

	{IDS_GH_VOL,		"2027"},		// 거래량
	{IDS_GH_RATE,		"2033"},
	{IDS_GH_TPRC,		"2028"},
	{IDS_GH_ASK,		"3051"},		//2013.02.19 KSJ 2025 --> 3051, 2026 --> 3071 심볼변경 
	{IDS_GH_BID,		"3071"},		//2013.02.19 KSJ 2025 --> 3051, 2026 --> 3071 심볼변경 
	{IDS_GH_ASKSIZE,	"3041"},
	{IDS_GH_BIDSIZE,	"3061"},
	{IDS_GH_TASKSIZE,	"3101"},
	{IDS_GH_TBIDSIZE,	"3106"},
	{IDS_GH_ASKRATE,	"3104"},
	{IDS_GH_BIDRATE,	"3109"},
	{IDS_GH_HOGARATE,	"3107"},
	{IDS_GH_MPRICE,	        "3146"},
	{IDS_GH_MPRICEPER,	"3181"},
	
	
	{IDS_GH_OPEN,		"2029"},
	{IDS_GH_OPEN_P,		"#1##"},
	{IDS_GH_P_OPEN,		"#4##"},
	{IDS_GH_HIGH,		"2030"},
	{IDS_GH_HIGH_P,		"#2##"},
	{IDS_GH_P_HIGH,		"#5##"},
	{IDS_GH_LOW,		"2031"},
	{IDS_GH_LOW_P,		"#3##"},
	{IDS_GH_P_LOW,		"#6##"},
	{IDS_GH_TVOL,		"2032"},
	{IDS_GH_OPENRATEW,	"##1#"},
	{IDS_GH_OPENRATEP,	"##2#"},
	{IDS_GH_HIGHRATEW,	"##3#"},
	{IDS_GH_HIGHRATEP,	"##4#"},
	{IDS_GH_LOWRATEW,	"##5#"},
	{IDS_GH_LOWRATEP,	"##6#"},
	{IDS_GH_PRECURR,	"2934"},
	{IDS_GH_PREVOL,		"2321"},
	{IDS_GH_AVOFS,		"2305"},
	{IDS_GH_CHESTRENGTH,	"2036"},
	{IDS_GH_ROTATION,	"2350"},
	{IDS_GH_FORERATE,	"2204"},
	{IDS_GH_DATEBONG,	"7777"},
	
	{IDS_GH_POSSNUM,	"###1"},
	{IDS_GH_POSSPRC,	"###2"},
	{IDS_GH_POSSPROFIT,	"###3"},
	{IDS_GH_POSSRATE,	"###4"},

	{IDS_GH_CAPITAL,	"1333"},
	{IDS_GH_STOCKCOUNT,	"1334"},
	{IDS_GH_PRETPRC,	"2322"},
	{IDS_GH_UPDNRANGE,	"2342"},
	{IDS_GH_LIMITRANGE,	"2343"},
	{IDS_GH_PRETRATE,	"2403"},
	{IDS_GH_REGLINE2,	"2421"},
	{IDS_GH_REGLINE1,	"2422"},
	{IDS_GH_PIVOT,		"2423"},
	{IDS_GH_SUPLINE1,	"2424"},
	{IDS_GH_SUPLINE2,	"2425"},
	{IDS_GH_MAX52,		"5311"},
	{IDS_GH_MIN52,		"5312"},
	{IDS_GH_MAXYEAR,	"5313"},
	{IDS_GH_MINYEAR,	"5314"},
	{IDS_GH_MAXVOL52,	"5315"},

	{IDS_GH_MINVOL52,	"5316"},
	{IDS_GH_MAXVOLYEAR,	"5317"},
	{IDS_GH_MINVOLYEAR,	"5318"},
	{IDS_GH_MAXDATE52,	"5321"},
	{IDS_GH_MINDATE52,	"5322"},
	{IDS_GH_MAXDATEYEAR,	"5323"},
	{IDS_GH_MINDATEYEAR,	"5324"},
	{IDS_GH_MAXVOLDATE52,	"5325"},
	{IDS_GH_MINVOLDATE52,	"5326"},
	{IDS_GH_MAXVOLDATEYEAR,	"5327"},
	{IDS_GH_MINVOLDATEYEAR,	"5328"},
	{IDS_GH_MAXDIFF52,	"5331"},
	{IDS_GH_MINDIFF52,	"5332"},
	{IDS_GH_MAXDIFFYEAR,	"5333"},
	{IDS_GH_MINDIFFYEAR,	"5334"},
	{IDS_GH_MAXRATE52,	"5341"},
	{IDS_GH_MINRATE52,	"5342"},
	{IDS_GH_MAXRATEYEAR,	"5343"},
	{IDS_GH_MINRATEYEAR,	"5344"},
	{IDS_GH_CONTUPLIMIT,	"5345"},
	{IDS_GH_CONTUP,		"5346"},
	{IDS_GH_CONTDNLIMIT,	"5347"},
	{IDS_GH_CONTDN,		"5348"},
	{IDS_GH_ELWLEFTDATE,	"1842"},
	{IDS_GH_ELWLASTDATE,	"1854"},
	{IDS_GH_ELWCRATE,	"1855"},
	{IDS_GH_EPSFULL,	"5353"},
	{IDS_GH_EPSHALF,	"5354"},
	{IDS_GH_PERFULL,	"5355"},
	{IDS_GH_PERHALF,	"5356"},
	{IDS_GH_GIGOAN,		"2390"},
	{IDS_GH_FOREIGNER,	"2399"},
	{IDS_GH_ABR_1JU,	"5401"},		//khs
	{IDS_GH_ABR_1MON,	"5402"},
	{IDS_GH_ABR_3MON,	"5403"},
	{IDS_GH_ABR_6MON,	"5404"},
	{IDS_GH_ABR_1YEA,	"5405"}


};
const int cnt_fieldInfo = sizeof(fieldInfo) / sizeof(_fieldInfo);

struct _fieldInfo defInfo[] = {
	{IDS_GH_DIFF,		"2024"},
	{IDS_GH_VOL,		"2027"},
	{IDS_GH_ASK,		"3051"},	//2013.02.19 KSJ 2025 --> 3051, 2026 --> 3071 심볼변경 
	{IDS_GH_BID,		"3071"},	//2013.02.19 KSJ 2025 --> 3051, 2026 --> 3071 심볼변경 
	{IDS_GH_ASKSIZE,	"3041"},
	{IDS_GH_BIDSIZE,	"3061"},
	{IDS_GH_TASKSIZE,	"3101"},
	{IDS_GH_TBIDSIZE,	"3106"},
};

#define	deliMITER	";"

#define	currentMAP	"TS0000XX"
#define	chartMAP	"TS0000XX"
#define triggerACT	""

const int keepCOL  = 4;
const int maxLIST  = 18;
const int maxFIELD = 10;
/////////////////////////////////////////////////////////////////////////////
// CPage2 property page
[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CPage2, CPropertyPage)

CPage2::CPage2(CWnd* parent) : CPropertyPage(CPage2::IDD)
	//: _bMemo(FALSE)

{
	//{{AFX_DATA_INIT(CPage2)
	//}}AFX_DATA_INIT

	m_parent = parent;
	m_fields.RemoveAll();

	m_userM = _T("");
	m_dragImage = nullptr;
	m_activePage = false;
	m_activeno = 0;
	m_bSet = FALSE;
	m_bAllBold = false;
	m_strFontSize = "9";
}

CPage2::~CPage2()
{
	if (m_dragImage) delete m_dragImage;
}

void CPage2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage2)
	DDX_Control(pDX, IDC_RADIO101, m_df);
	DDX_Control(pDX, IDC_CBFONTNAME, m_fontname);
	DDX_Control(pDX, IDC_EDITFIELD, m_fieldnm);
	DDX_Control(pDX, IDC_CBFIELD, m_cbfield);
	DDX_Control(pDX, IDC_USERMAP, m_usermap);
	DDX_Control(pDX, IDC_SPIN, m_spin);
	DDX_Control(pDX, IDC_EDITLINE, m_line);
	DDX_Control(pDX, IDC_CBTNREAL, m_cbtReal);
	DDX_Control(pDX, IDC_CBTNF103, m_cbtBK103);
	DDX_Control(pDX, IDC_CBTNF102, m_cbtBK102);
	DDX_Control(pDX, IDC_CBTNF101, m_cbtBK101);
	DDX_Control(pDX, IDC_CBTNBOOKMARK, m_cbtBookmark);
	DDX_Control(pDX, IDC_CBTNBOOKMARKPEN, m_cbtBookmarkPen);
	DDX_Control(pDX, IDC_CBFONTSIZE, m_fontsize);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Radio(pDX, IDC_RADIO_MEMOCLICK, _bMemo);
	//}}AFX_DATA_MAP
	
}


BEGIN_MESSAGE_MAP(CPage2, CPropertyPage)
	//{{AFX_MSG_MAP(CPage2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_MOVEDN, OnMovedn)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, OnBegindragList1)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST2, OnBegindragList2)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_CBN_SELCHANGE(IDC_CBFIELD, OnSelchangeCbfield)
	ON_BN_CLICKED(IDC_CHK_OVERLAP, OnChkOverlap)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SETTING, &CPage2::OnBnClickedBtnSetting)
	ON_COMMAND_RANGE(IDC_RADIO_MEMOCLICK, IDC_RADIO_MEMOOVER, OnRadioMemo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage2 message handlers

BOOL CPage2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");
	m_bSet = FALSE;

	CString userD; 
	userD.Format("%s/%s/%s", m_root, USRDIR, m_name);
	CString saveN; 
	saveN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);
	CString tempN; 
	tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);

	CreateDirectory(userD, nullptr); 
	CopyFile(saveN, tempN, FALSE);
	
	initList(); 
	initField(); 
	initCombo(); 
	initSpin();
	
	if (!loadField(m_activeno) || !loadConfig())
	{
		setDefaultField(m_activeno); 
		setDefaultColor();
	}
	//((CButton *) GetDlgItem(IDC_RADIO301))->SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPage2::OnSetActive() 
{
	m_activePage = true;
	return CPropertyPage::OnSetActive();
}

BOOL CPage2::OnKillActive() 
{
	/*if( m_bSet )
	{
		AfxMessageBox("설정창을 닫아주세요");
		return FALSE;
	}*/

	CWnd* fw = FindWindow(nullptr, "메뉴검색기");
	if (fw != nullptr) fw->SendMessage(WM_CLOSE);

	saveField(m_activeno); 
	saveConfig();	
	return CPropertyPage::OnKillActive();
}

void CPage2::OnSelchangeCbfield() 
{
	saveField(m_activeno);

	m_list2.DeleteAllItems();
	m_activeno = m_cbfield.GetCurSel();
	loadField(m_activeno);
}

BOOL CPage2::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	CButton* button = nullptr;

	switch (LOWORD(wParam))
	{
	case IDC_RADIO101:
		m_line.EnableWindow(FALSE); m_spin.EnableWindow(FALSE);
		((CButton *) GetDlgItem(IDC_RADIO102))->SetCheck(FALSE);
		break;
	case IDC_RADIO102:
		m_line.EnableWindow(TRUE); m_spin.EnableWindow(TRUE);
		((CButton *) GetDlgItem(IDC_RADIO101))->SetCheck(FALSE);
		break;

	case IDC_RADIO301:
	case IDC_RADIO302:
		button = (CButton *) GetDlgItem(IDC_BTNMAP);
		button->EnableWindow(FALSE);
		break;
	case IDC_RADIO304:		// 더블클릭, 사용자지정
		button = (CButton *) GetDlgItem(IDC_BTNMAP);
		button->EnableWindow(TRUE);
		break;

	case IDC_BTNMAP:
		{
//			char readB[128];
			CString	stringTR, stringName;
			CString tempFile; 
			tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);
			CString str(_T(""));
			CTrSearch dlg(this, m_parent);

//			dlg.setPos(GetPos(1));
			if (dlg.DoModal() == IDOK)
			{ 
				stringTR =  dlg.getTR();
				stringName = dlg.getName();
			}
			m_usermap.SetWindowText(stringTR);
			
			m_userM.Format("%s%s%s%s%s", "4", deliMITER, stringTR, deliMITER, stringName);
			WritePrivateProfileString("INTERFIELD", "LINKMAP", m_userM, tempFile);
			
/*			m_bSet = TRUE;

			ZeroMemory(readB, sizeof(readB));
			m_parent->GetParent()->SendMessage(WM_USER + 666, 0, (LPARAM) readB);
			string = CString(readB);
			if (string.GetLength() <= 0) break;

			m_bSet = FALSE;

			stringx = parseX(string, "\t");
			m_usermap.SetWindowText(stringx);

			m_userM.Format("%s%s%s%s%s", "4", deliMITER, stringx, deliMITER, string);
			WritePrivateProfileString("INTERFIELD", "LINKMAP", m_userM, tempFile);
*/
		}

		break;

	case IDC_DEFFIELD:
		setDefaultField(m_activeno);
		break;
	case IDC_BTNDEFAULT:
		setDefaultColor();
		break;
	case IDC_CHK_ALL:
		((CButton *) GetDlgItem(IDC_CHK_NAME))->EnableWindow(m_bAllBold);
		((CButton *) GetDlgItem(IDC_CHK_CURR))->EnableWindow(m_bAllBold);
		m_bAllBold = !m_bAllBold;
		break;
	}
	
	return CPropertyPage::OnCommand(wParam, lParam);
}

void CPage2::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnAdd();
	*pResult = 0;
}

void CPage2::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDel();
	*pResult = 0;
}

void CPage2::OnAdd() 
{
	insertList1toList2();
}

void CPage2::OnDel() 
{
	deleteList2();
}

void CPage2::OnMoveup() 
{
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		GuideMsg(idTOstring(IDS_GUIDE4));
		return;
	}

	const int nitem = m_list2.GetNextSelectedItem(pos);
	if (nitem == 0) return;

	_field	field = m_fields.GetAt(nitem);

	DeleteItemToList2(nitem);
	AppendItemToList2(field.text, field.symb, nitem -1);
	deselcMark(&m_list2); selcMark(&m_list2, nitem-1);
}

void CPage2::OnMovedn() 
{
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		GuideMsg(idTOstring(IDS_GUIDE4));
		return;
	}

	const int nitem = m_list2.GetNextSelectedItem(pos);
//	if (nitem == 0) return;

	_field	field = m_fields.GetAt(nitem);

	DeleteItemToList2(nitem);
	AppendItemToList2(field.text, field.symb, nitem +1);
	deselcMark(&m_list2); selcMark(&m_list2, nitem+1);
}

void CPage2::OnBegindragList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	*pResult = 0;

	if (m_list1.GetSelectedCount() == 0)
		return;

	m_dragData.list = 1;
	m_dragData.item = pNMListView->iItem;

	if (m_dragImage)
		delete m_dragImage;

	CPoint	pt;
	m_dragImage = CreateDragImageSymbol(&pt, &m_list1);

	if (m_dragImage == (CImageList *)nullptr)
		return;

	CPoint	ptStart = pNMListView->ptAction;
	m_list1.ClientToScreen(&ptStart);

	m_dragImage->BeginDrag(0, CPoint(0, 0));
	m_dragImage->DragEnter(GetDesktopWindow(), ptStart);
	SetCapture();
}

void CPage2::OnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	if (m_list2.GetSelectedCount() == 0)
		return;

	m_dragData.list = 2;
	m_dragData.item = pNMListView->iItem;

	if (m_dragImage)
		delete m_dragImage;

	CPoint	pt;
	m_dragImage = CreateDragImageSymbol(&pt, &m_list2);

	if (m_dragImage == (CImageList *)nullptr)
		return;

	CPoint	ptStart = pNMListView->ptAction;
	m_list2.ClientToScreen(&ptStart);

	m_dragImage->BeginDrag(0, CPoint(0, 0));
	m_dragImage->DragEnter(GetDesktopWindow(), ptStart);
	SetCapture();
}

void CPage2::OnEndDrag(CPoint point)
{
	CRect	list1RC, list2RC;

	m_list1.GetClientRect(&list1RC); m_list1.ClientToScreen(&list1RC);
	m_list2.GetClientRect(&list2RC); m_list2.ClientToScreen(&list2RC);

	ClientToScreen(&point);

	if (list1RC.PtInRect(point))
	{
		if (m_dragData.list == 2)
			deleteList2();
	}
	else if (list2RC.PtInRect(point))
	{
		LVHITTESTINFO lvhti;
		m_list2.ScreenToClient(&point);
		lvhti.pt = point;

		int nitem = m_list2.HitTest(&lvhti);
		if (nitem < 0)	nitem = m_list2.GetItemCount();

		if (m_dragData.list == 2)
		{
			_field	field = m_fields.GetAt(m_dragData.item);

			DeleteItemToList2(m_dragData.item);
			AppendItemToList2(field.text, field.symb, nitem);
			deselcMark(&m_list2); selcMark(&m_list2, nitem);
		}
		else if (m_dragData.list == 1)
		{
			insertList1toList2(nitem);
		}
	}
}

void CPage2::OnLButtonUp(UINT nFlags, CPoint point) 
{
	::ReleaseCapture();	

	if (m_dragImage)
	{
		m_dragImage->DragLeave(GetDesktopWindow());
		m_dragImage->EndDrag();
		m_dragImage->DeleteImageList();
		delete m_dragImage;
		m_dragImage = nullptr;

		OnEndDrag(point);
	}

	CPropertyPage::OnLButtonUp(nFlags, point);
}

void CPage2::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect list2RC, labelRC;

	m_list2.GetClientRect(&list2RC);
	m_list2.GetItemRect(0, labelRC, LVIR_LABEL);

	m_list2.ClientToScreen(&list2RC);
	const CRect	topRC	 = CRect(list2RC.left, list2RC.top - 45, list2RC.right, list2RC.top + 5);
	const CRect	bottomRC = CRect(list2RC.left, list2RC.bottom,   list2RC.right, list2RC.bottom + 70);

	if (m_dragImage)
	{
		CPoint	ptMove = point;
		ClientToScreen(&ptMove);

		m_dragImage->DragMove(ptMove);
		
		if (topRC.PtInRect(ptMove))
		{
			SetAutoScroll(&m_list2, -1);
		}
		else if (bottomRC.PtInRect(ptMove))
		{
			SetAutoScroll(&m_list2, +1);
		}
		else if (m_autoScroll)
		{
			CPoint	pt = ptMove;
			LVHITTESTINFO	lvhti;
			m_list2.ScreenToClient(&pt);
			lvhti.pt = pt;

			const int pItem = m_list2.HitTest(&lvhti);

			m_list2.Invalidate();
			
			m_autoScroll = false;
		}
	}	CPropertyPage::OnMouseMove(nFlags, point);
}

void CPage2::initList()
{
	m_list1.DeleteAllItems();
	AddColumn(&m_list1, "1", 0, 15);
	AddColumn(&m_list1, idTOstring(IDS_REGFIELD1), 1, 60);
	m_list1.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	CString sCnt;
	int nItem{};
	int nCnt = 1;
	for (int ii = keepCOL ; ii < cnt_fieldInfo ; ii++)
	{
		sCnt.Format("%d", nCnt);
		nItem = AddItem(&m_list1, sCnt, ii);
		AddItem(&m_list1, idTOstring(fieldInfo[ii].stid), nItem, 1);
		nCnt++;
	}

	m_list2.DeleteAllItems();
	AddColumn(&m_list2, idTOstring(IDS_REGFIELD2), 0, 98);
	m_list2.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}


/* 2019/2/20 
void CPage2::initList()
{
m_list1.DeleteAllItems();
AddColumn(&m_list1, idTOstring(IDS_REGFIELD1), 0, 75);
m_list1.SetExtendedStyle(LVS_EX_FULLROWSELECT);

  for (int ii = keepCOL ; ii < cnt_fieldInfo ; ii++)
		AddItem(&m_list1, idTOstring(fieldInfo[ii].stid), ii);
		
		  m_list2.DeleteAllItems();
		  AddColumn(&m_list2, idTOstring(IDS_REGFIELD2), 0, 98);
		  m_list2.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}
*/

void CPage2::initField()
{
	CString	fieldno;
	for (int ii = 0; ii < maxFIELD; ii++)
	{
		fieldno.Format("%02d", ii);
		m_cbfield.AddString(fieldno);
	}

	m_cbfield.SetCurSel(0);
}

void CPage2::initCombo()
{
	m_fontsize.ResetContent();

	m_fontsize.AddString("8");
	m_fontsize.AddString("9");
	m_fontsize.AddString("10");
	m_fontsize.AddString("11");
	m_fontsize.AddString("12");
	m_fontsize.AddString("13");
	m_fontsize.AddString("14");
	m_fontsize.AddString("15");
	m_fontsize.SetCurSel(1);

	m_fontname.AddString("굴림");
	m_fontname.AddString("굴림체");
	m_fontname.AddString("궁서");
	m_fontname.AddString("궁서체");
	m_fontname.AddString("돋움");
	m_fontname.AddString("돋움체");
	m_fontname.AddString("바탕");
	m_fontname.AddString("바탕체");

	m_fontname.SetCurSel(1);
}

void CPage2::initSpin()
{
	m_spin.SetBuddy(&m_line);

	m_spin.SetRange(1, 10);
	m_line.SetWindowText("1");
}

bool CPage2::loadField(int fno)
{
	CString tempN; 
	tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);

	CFileFind finder; 
	if (!finder.FindFile(tempN)) return false;

	int	readL = 0;
	char	readB[1024];
	CString	string, stringx, fieldno;

	m_fields.RemoveAll(); 
	m_fieldnm.SetWindowText("");
	stringx.Format("필드 %02d", fno);

	fieldno.Format("%02d", fno);
	readL = GetPrivateProfileString("FIELDS", fieldno, stringx, readB, sizeof(readB), tempN);

	if (readL > 0)
	{
		string = CString(readB, readL);
		stringx = parseX(string, deliMITER);		// fields name
		m_fieldnm.SetWindowText(stringx);

		while (!string.IsEmpty())
		{
//			stringx = parseX(string, deliMITER);	// text
//			if (stringx.IsEmpty()) continue;
			stringx = parseX(string, deliMITER);	// symb
			if (stringx.IsEmpty()) continue;

			for (int ii = 0; ii < cnt_fieldInfo; ii++)
			{
				if (fieldInfo[ii].symb == stringx)
				{
					AppendItemToList2(idTOstring(fieldInfo[ii].stid), fieldInfo[ii].symb, m_list2.GetItemCount());
					break;
				}
			}
		}
	}

	return true;
}

bool CPage2::loadConfig()
{
	CString tempN; 
	tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);

	CFileFind finder;
	if (!finder.FindFile(tempN))
		return false;

	int	readL = 0;
	char	readB[1024];
	CString	string, stringx;

	readL = GetPrivateProfileString("INTERFIELD", "BKCOLOR", "", readB, sizeof(readB), tempN);
	if (readL > 0)
	{
		CButton* btn = nullptr;

		if (readB[0] == '1')
		{
			btn = (CButton *) GetDlgItem(IDC_RADIO101); btn->SetCheck(TRUE);
			btn = (CButton *) GetDlgItem(IDC_RADIO102); btn->SetCheck(FALSE);

			m_line.EnableWindow(FALSE); m_spin.EnableWindow(FALSE);
		}
		else if (readB[0] == '2')
		{
			btn = (CButton *) GetDlgItem(IDC_RADIO101); btn->SetCheck(FALSE);
			btn = (CButton *) GetDlgItem(IDC_RADIO102); btn->SetCheck(TRUE);

			m_line.EnableWindow(TRUE); m_spin.EnableWindow(TRUE);
		}
	}

	readL = GetPrivateProfileString("INTERFIELD", "COLOR1", "", readB, sizeof(readB), tempN);
	m_cbtBK101.SetColor(GetAxColor(68));
	if (readL > 0) 
		m_cbtBK101.SetColor(_httoi(readB));
		

	readL = GetPrivateProfileString("INTERFIELD", "COLOR2", "", readB, sizeof(readB), tempN);
	m_cbtBK102.SetColor(GetAxColor(68));
	m_cbtBK103.SetColor(GetAxColor(77));
	if (readL > 0)
	{
		string = CString(readB, readL);

		stringx = parseX(string, deliMITER);
		m_line.SetWindowText(stringx);

		stringx = parseX(string, deliMITER);
		m_cbtBK102.SetColor(_httoi(stringx));
		m_cbtBK103.SetColor(_httoi(string));
	}
	
	readL = GetPrivateProfileString("INTERFIELD", "BOOKMARKCOLOR", "", readB, sizeof(readB), tempN);
	m_cbtBookmark.SetColor(GetAxColor(199));
	if (readL > 0) 
		m_cbtBookmark.SetColor(_httoi(readB));

	readL = GetPrivateProfileString("INTERFIELD", "BOOKMARKPENCOLOR", "", readB, sizeof(readB), tempN);
	if (readL > 0) m_cbtBookmarkPen.SetColor(_httoi(readB));

	readL = GetPrivateProfileString("INTERFIELD", "REALCOLOR", "", readB, sizeof(readB), tempN);
	if (readL > 0) m_cbtReal.SetColor(_httoi(readB));


	readL = GetPrivateProfileString("INTERFIELD", "MEMO", "0", readB, sizeof(readB), tempN);
	if (readL > 0)  _bMemo = atoi(readB) != 0 ? FALSE : TRUE;
	UpdateData(FALSE);


	readL = GetPrivateProfileString("INTERFIELD", "FLASHTYPE", "", readB, sizeof(readB), tempN);
	if (readL > 0)
	{
		CButton* btn1 = (CButton *) GetDlgItem(IDC_RADIO401); btn1->SetCheck(FALSE);
		CButton* btn2 = (CButton *) GetDlgItem(IDC_RADIO402); btn2->SetCheck(FALSE);
		CButton* btn3 = (CButton *) GetDlgItem(IDC_RADIO403); btn3->SetCheck(FALSE);

		switch (char(readB[0]))
		{
		case '1':			// 셀단위 변환
			btn1->SetCheck(TRUE); break;
		case '2':			// 행단위 변환
			btn2->SetCheck(TRUE); break;
		case '0':
		default:
			btn3->SetCheck(TRUE); break;
		}
	}

	readL = GetPrivateProfileString("INTERFIELD", "LINKMAP", "1", readB, sizeof(readB), tempN);
	if (readL > 0)
	{
		CString str = CString(readB, readL);

		stringx = parseX(str, deliMITER);
		if (stringx.GetLength() > 0)
		{
			CButton* btn1 = (CButton *) GetDlgItem(IDC_RADIO301); 
			btn1->SetCheck(FALSE);

			CButton* btn2 = (CButton *) GetDlgItem(IDC_RADIO302); 
			btn2->SetCheck(FALSE);		
			
			CButton* btn4 = (CButton *) GetDlgItem(IDC_RADIO304); 
			btn4->SetCheck(FALSE);

			CButton* mapb = (CButton *) GetDlgItem(IDC_BTNMAP);   
			mapb->EnableWindow(FALSE);

			switch (stringx.GetAt(0))
			{
			case '1':
				btn1->SetCheck(TRUE);	
				break;
			case '2':
				btn2->SetCheck(TRUE);	
				break;
			case '4':
				btn4->SetCheck(TRUE);	
				mapb->EnableWindow(TRUE);
				stringx = parseX(str, deliMITER); 
				m_usermap.SetWindowText(stringx);
				break;
			default:
				btn1->SetCheck(TRUE);
				break;
			}
		}
	}
	else
	{
		CButton *btngg = (CButton *) GetDlgItem(IDC_RADIO301); 
		btngg->SetCheck(TRUE);
	}

	readL = GetPrivateProfileInt("INTERFIELD", "SELACTION", 1, tempN);
	CButton* btn = (CButton *) GetDlgItem(IDC_CHK_CODETRIGGER);
	btn->SetCheck(readL);

	readL = GetPrivateProfileInt("INTERFIELD", "OVERLAP", 0, tempN);
	CButton* btn2 = (CButton *) GetDlgItem(IDC_CHK_OVERLAP);
	btn2->SetCheck(readL);

	//2016.12.09 KSJ 종료시 자동저장 추가
	readL = GetPrivateProfileInt("INTERFIELD", "AUTOSAVE", 0, tempN);
	CButton* btn3 = (CButton *) GetDlgItem(IDC_CHK_AUTOSAVE);
	btn3->SetCheck(readL);

	readL = GetPrivateProfileString("INTERFIELD", "FONTPOINT", "", readB, sizeof(readB), tempN);
	if (readL > 0)
	{
		CString	str;
		str.Format("%s", readB);
		m_fontsize.SelectString(0, str);
		
		m_strFontSize = str;
	}

	// 폰트가 변할때 폭 늘이기
	readL = GetPrivateProfileInt("INTERFIELD", "CHKFONTPOINT", 0, tempN);
	btn = (CButton *) GetDlgItem(IDC_CHK_FONTSIZE);
	
	btn->SetCheck(readL);


	// 폰트종류
	CString	sztmp = _T("굴림체");
	readL = GetPrivateProfileString("INTERFIELD", "FONTNAME", "", readB, sizeof(readB), tempN);
	if (readL > 0)
	{
		sztmp.Format("%s", readB);
	}
	
	m_fontname.SelectString(0, sztmp);		

	// 현재가 굵기 옵션 
	readL = GetPrivateProfileInt("INTERFIELD", "CURRBOLD", 1, tempN);
	btn = (CButton *) GetDlgItem(IDC_CHK_CURR);

	btn->SetCheck(readL);

	// ADD PSH 20079017
	// 종목명 굵기 옵션 
	readL = GetPrivateProfileInt("INTERFIELD", "NAMEBOLD", 1, tempN);
	btn = (CButton *) GetDlgItem(IDC_CHK_NAME);

	btn->SetCheck(readL);
	// END ADD

	// 2012.11.28 KSJ 전체 굵게
	readL = GetPrivateProfileInt("INTERFIELD", "ALLBOLD", 0, tempN);
	btn = (CButton *) GetDlgItem(IDC_CHK_ALL);
	
	btn->SetCheck(readL);

	m_bAllBold = readL ? true:false;
	((CButton *) GetDlgItem(IDC_CHK_NAME))->EnableWindow(!m_bAllBold);
	((CButton *) GetDlgItem(IDC_CHK_CURR))->EnableWindow(!m_bAllBold);
	// END 

	
	return true;
}

void CPage2::saveField(int fno)
{
	CString	string, fieldno, fieldnm = _T("");
	CString tempFile; 
	tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);

	fieldno.Format("%02d", fno); 
	m_fieldnm.GetWindowText(fieldnm);

	_field field; 
	string = fieldnm + deliMITER;
	for (int ii = 0; ii < m_fields.GetSize(); ii++)
	{
		field = m_fields.GetAt(ii);

//		string += field.text + deliMITER;
		string += field.symb + deliMITER;
	}
	WritePrivateProfileString("FIELDS", fieldno, string, tempFile);
}

void CPage2::saveConfig()
{
	CString	string, stringx;
	CString tempFile; 
	tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);

	CButton* btn; 
	btn = (CButton *) GetDlgItem(IDC_RADIO101);
	string = _T("2"); if (btn->GetCheck()) string = _T("1");
	WritePrivateProfileString("INTERFIELD", "BKCOLOR", string, tempFile);

	string.Format("0x%06x", m_cbtBK101.GetColor());
	WritePrivateProfileString("INTERFIELD", "COLOR1", string, tempFile);

	m_line.GetWindowText(stringx); if (atoi(stringx) < 1) stringx = _T("1");
	string.Format("%s%s0x%06x%s0x%06x", stringx, deliMITER, m_cbtBK102.GetColor(), deliMITER, m_cbtBK103.GetColor());
	WritePrivateProfileString("INTERFIELD", "COLOR2", string, tempFile);

	string.Format("0x%06x", m_cbtReal.GetColor());
	WritePrivateProfileString("INTERFIELD", "REALCOLOR", string, tempFile);

	
	string.Format("0x%06x", m_cbtBookmarkPen.GetColor());
	WritePrivateProfileString("INTERFIELD", "BOOKMARKPENCOLOR", string, tempFile);

	string.Format("0x%06x", m_cbtBookmark.GetColor());
	WritePrivateProfileString("INTERFIELD", "BOOKMARKCOLOR", string, tempFile);
	
	CButton* btn1 = (CButton *) GetDlgItem(IDC_RADIO401);
	CButton* btn2 = (CButton *) GetDlgItem(IDC_RADIO402);
	CButton* btn3 = (CButton *) GetDlgItem(IDC_RADIO403);

	string = _T("0");
	if (btn1->GetCheck()) string = _T("1");
	else if (btn2->GetCheck()) string = _T("2");
	else if (btn3->GetCheck()) string = _T("0");
	WritePrivateProfileString("INTERFIELD", "FLASHTYPE", string, tempFile);

	btn1 = (CButton *) GetDlgItem(IDC_RADIO301);
	btn2 = (CButton *) GetDlgItem(IDC_RADIO302);
	btn3 = (CButton *) GetDlgItem(IDC_CHK_CODETRIGGER);

	string = m_userM;
	if (btn1->GetCheck()) string.Format("%s%s%s%s%s", "1", deliMITER, currentMAP, deliMITER, "현재가");
	else if (btn2->GetCheck()) string.Format("%s%s%s%s%s", "2", deliMITER, chartMAP, deliMITER, "챠트");
	
	WritePrivateProfileString("INTERFIELD", "LINKMAP", string, tempFile);
	if (btn3->GetCheck())
		WritePrivateProfileString("INTERFIELD", "SELACTION", "1", tempFile);
	else
		WritePrivateProfileString("INTERFIELD", "SELACTION", "0", tempFile);
	
	CButton* btn4 = (CButton* ) GetDlgItem(IDC_CHK_OVERLAP);

	if(btn4->GetCheck())
		WritePrivateProfileString("INTERFIELD", "OVERLAP", "1", tempFile);
	else
		WritePrivateProfileString("INTERFIELD", "OVERLAP", "0", tempFile);

	CButton* btn5 = (CButton* ) GetDlgItem(IDC_CHK_AUTOSAVE);
	
	if(btn5->GetCheck())
		WritePrivateProfileString("INTERFIELD", "AUTOSAVE", "1", tempFile);
	else
		WritePrivateProfileString("INTERFIELD", "AUTOSAVE", "0", tempFile);

	m_fontsize.GetLBText(m_fontsize.GetCurSel(), string);
	WritePrivateProfileString("INTERFIELD", "FONTPOINT", string, tempFile);

	//2013.03.27 KSJ 늘이기
	btn3 = (CButton*)GetDlgItem(IDC_CHK_FONTSIZE);
	if (btn3->GetCheck())
		WritePrivateProfileString("INTERFIELD", "CHKFONTPOINT", "1", tempFile);
	else	
		WritePrivateProfileString("INTERFIELD", "CHKFONTPOINT", "0", tempFile);

	//늘이기에 체크되어 있고 이전 폰트크기와 지금 폰트크기를 비교하여 다르면 폭을 조절해야함.
	if(m_strFontSize.Compare(string) && btn3->GetCheck())
	{
		CString strTemp;
		strTemp.Format("%d", _ttoi(m_strFontSize) - _ttoi(string));
		WritePrivateProfileString("INTERFIELD", "RESIZE", strTemp, tempFile);
	}
	else
	{
		WritePrivateProfileString("INTERFIELD", "RESIZE", "0", tempFile);
	}
	//KSJ 늘이기 끝

	m_fontname.GetLBText(m_fontname.GetCurSel(), string);
	WritePrivateProfileString("INTERFIELD", "FONTNAME", string, tempFile);

	btn3 = (CButton*)GetDlgItem(IDC_CHK_CURR);
	if (btn3->GetCheck())
		WritePrivateProfileString("INTERFIELD", "CURRBOLD", "1", tempFile);
	else	
		WritePrivateProfileString("INTERFIELD", "CURRBOLD", "0", tempFile);

	// ADD PSH 20070917
	btn3 = (CButton*)GetDlgItem(IDC_CHK_NAME);
	if (btn3->GetCheck())
		WritePrivateProfileString("INTERFIELD", "NAMEBOLD", "1", tempFile);
	else	
		WritePrivateProfileString("INTERFIELD", "NAMEBOLD", "0", tempFile);
	// END ADD

	//2012.11.28 KSJ 전체굵게
	btn3 = (CButton*)GetDlgItem(IDC_CHK_ALL);
	if (btn3->GetCheck())
		WritePrivateProfileString("INTERFIELD", "ALLBOLD", "1", tempFile);
	else	
		WritePrivateProfileString("INTERFIELD", "ALLBOLD", "0", tempFile);
	//END KSJ

	UpdateData(TRUE);
	string.Format("%s", _bMemo ? "0" : "1");
	WritePrivateProfileString("INTERFIELD", "MEMO", string, tempFile);
}

void CPage2::setDefaultField(int fno)
{
	m_list2.DeleteAllItems(); m_fields.RemoveAll();

	CString	fieldnm;
	m_fieldnm.GetWindowText(fieldnm); fieldnm.TrimLeft(); fieldnm.TrimRight();
	if (fieldnm.IsEmpty()) fieldnm.Format("필드 %02d", fno);

	m_cbfield.SetCurSel(fno); 
	m_fieldnm.SetWindowText(fieldnm);
	for (int ii = 0; ii < sizeof(defInfo) / sizeof(_fieldInfo); ii++)
		AppendItemToList2(idTOstring(defInfo[ii].stid), defInfo[ii].symb, m_list2.GetItemCount());
}

void CPage2::setDefaultColor()
{
	CButton* btn;

	btn = (CButton *) GetDlgItem(IDC_RADIO101); btn->SetCheck(FALSE);
	btn = (CButton *) GetDlgItem(IDC_RADIO102); btn->SetCheck(TRUE);

	btn = (CButton *) GetDlgItem(IDC_RADIO301); btn->SetCheck(TRUE);
	btn = (CButton *) GetDlgItem(IDC_RADIO302); btn->SetCheck(FALSE);
	btn = (CButton *) GetDlgItem(IDC_RADIO304); btn->SetCheck(FALSE);

	btn = (CButton *) GetDlgItem(IDC_RADIO401); btn->SetCheck(FALSE);
	btn = (CButton *) GetDlgItem(IDC_RADIO402); btn->SetCheck(FALSE);
	btn = (CButton *) GetDlgItem(IDC_RADIO403); btn->SetCheck(TRUE);

	btn = (CButton *) GetDlgItem(IDC_CHK_CODETRIGGER); btn->SetCheck(TRUE);
	btn = (CButton *) GetDlgItem(IDC_CHK_OVERLAP); btn->SetCheck(FALSE);
	btn = (CButton *) GetDlgItem(IDC_CHK_AUTOSAVE); btn->SetCheck(FALSE);	//2016.12.09 KSJ 화면닫을때 자동저장

	m_line.SetWindowText("1"); 
	
	m_line.EnableWindow(FALSE);
	m_spin.EnableWindow(FALSE);

	m_cbtBK101.SetColor(GetAxColor(68));
	m_cbtBK102.SetColor(GetAxColor(68));
	m_cbtBK103.SetColor(GetAxColor(77));
		
	m_cbtBookmark.SetColor(GetAxColor(199));
	m_cbtBookmarkPen.SetColor(GetAxColor(69));

	m_cbtReal.SetColor(getIndexColor(8));
	m_fontsize.SetCurSel(1);
	m_fontname.SelectString(0, "굴림체");

	btn = (CButton *) GetDlgItem(IDC_CHK_FONTSIZE); btn->SetCheck(FALSE);
	btn = (CButton *) GetDlgItem(IDC_CHK_ALL); btn->SetCheck(FALSE);
	btn = (CButton *) GetDlgItem(IDC_CHK_NAME); btn->SetCheck(TRUE); btn->EnableWindow(TRUE);
	btn = (CButton *) GetDlgItem(IDC_CHK_CURR); btn->SetCheck(TRUE); btn->EnableWindow(TRUE);
}

void CPage2::AppendItemToList2(CString text, CString symb, int nitem)
{
	text.TrimRight();
	AddItem(&m_list2, text, nitem);

	_field	field;

	field.text = text;
	field.symb = symb;

	if (nitem < 0)
		m_fields.Add(field);
	else
		m_fields.InsertAt(nitem, field);
}

void CPage2::DeleteItemToList2(int nitem)
{
	if (m_list2.GetItemCount() > nitem)
		m_list2.DeleteItem(nitem);
	if (m_fields.GetSize() > nitem)
		m_fields.RemoveAt(nitem);
}

void CPage2::CheckField(CString text)
{
	if(text == "수익률")
	{
		GuideMsg("수익률을 확인하시려면 <손익평가>탭에서 보유수량과 매입단가를 입력하여 주시기 바랍니다.");
	}
	
	if(text == "평가손익")
	{
		GuideMsg("평가손익을 확인하시려면 <손익평가>탭에서 보유수량과 매입단가를 입력하여주시기 바랍니다.");
	}
	
// 	CString item; 
// 	for(int i=0 ; i< m_list2.GetItemCount() ; i++)
// 	{
// 		item = m_list2.GetItemText(i, 1);
// 
// 		item.TrimLeft();item.TrimRight();
// 	}

	if(text == "보유수량")
	{
		GuideMsg("평가손익이나 수익률을 확인하시려면 <손익평가>탭에서 보유수량과 매입단가를 입력하여주시기 바랍니다.");
	}
	
	if(text == "매입단가")
	{
		GuideMsg("평가손익이나 수익률을 확인하시려면 <손익평가>탭에서 보유수량과 매입단가를 입력하여주시기 바랍니다.");
	}
}


void CPage2::insertList1toList2(int sitem)
{
	deselcMark(&m_list2);

	int	nitem{};
	CString	text, symb;

	const int	nsel = m_list1.GetSelectedCount();
	POSITION pos = m_list1.GetFirstSelectedItemPosition();

	while (pos)
	{
		nitem = m_list1.GetNextSelectedItem(pos);

		text  = idTOstring(fieldInfo[nitem + keepCOL].stid);

		//필드 체크
		CheckField(text);
			
			symb  = fieldInfo[nitem + keepCOL].symb;
		
		{
			const int	ncnt = m_fields.GetSize();
			bool	bsame = false;
			_field	field;
			for ( int ii = 0 ; ii < ncnt ; ii++ )
			{
				field = m_fields.GetAt(ii);
				if (field.symb.Compare(symb) == 0)
				{
					bsame = true;
					break;
				}
			}
			
			if (bsame)
			{
				if (nsel == 1)
				{
					GuideMsg("이미 존재하는 필드 입니다.");
					break;
				}
				else
					continue;
			}
		}

		if (m_list2.GetItemCount() >= maxLIST)
		{
			GuideMsg(idTOstring(IDS_GUIDE3));
			break;
		}

		if (sitem < 0)
		{
			const int position = m_list2.GetItemCount();
			AppendItemToList2(text, symb, position);

			selcMark(&m_list2, position);
		}
		else	// drag
		{
			AppendItemToList2(text, symb, sitem++);
		}
	}
}

void CPage2::deleteList2()
{
	int nItem = -1;
	POSITION pos{};
	int count = m_list2.GetItemCount();
	for (int ii = 0; ii < count; ii++)
	{
		pos = m_list2.GetFirstSelectedItemPosition();
		if (pos != nullptr)
		{
			nItem = m_list2.GetNextSelectedItem(pos);
			DeleteItemToList2(nItem);
		}
	}

	count = m_list2.GetItemCount();
	if (count > nItem)
		selcMark(&m_list2, nItem, false);
	else
		selcMark(&m_list2, nItem -1, false);
}

void CPage2::selcMark(CListCtrl* listctrl, int nItem, bool scroll /*=true*/)
{
	CRect	rc; CSize size;

	listctrl->SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

	if (scroll)
	{
		listctrl->GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - listctrl->GetTopIndex());
		listctrl->Scroll(size);
	}
}

void CPage2::deselcMark(CListCtrl* listctrl)
{
	int nItem{};
	POSITION pos = listctrl->GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		while (pos)
		{
			nItem = listctrl->GetNextSelectedItem(pos);
			listctrl->SetItemState(nItem, 0, LVIS_SELECTED|LVIS_FOCUSED);
		}
	}
}

CImageList* CPage2::CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl* listctrl)
{
	const int count = listctrl->GetSelectedCount();
	if (count < 1) return nullptr;

	
	CRect itemRC, totalRC; totalRC.SetRectEmpty();
	
	POSITION pos = listctrl->GetFirstSelectedItemPosition();
	if (!pos) return nullptr;

	int item = listctrl->GetNextSelectedItem(pos);
	listctrl->GetItemRect(item, itemRC, LVIR_BOUNDS);
	totalRC = itemRC;
	totalRC.bottom = totalRC.top + totalRC.Height() * count;

	CDC memDC; CClientDC dcClient(this);
	if (!memDC.CreateCompatibleDC(&dcClient))
		return nullptr;

	CBitmap bitmap;
	if (!bitmap.CreateCompatibleBitmap(&dcClient, totalRC.Width(), totalRC.Height()))
		return nullptr;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(0, 0, totalRC.Width(), totalRC.Height(), RGB(255, 255, 255));
	
	auto pCompleteImageList = std::make_unique<CImageList>(); 
	pCompleteImageList->Create(totalRC.Width(), totalRC.Height(), ILC_COLOR|ILC_MASK, 0, 1);

	int index = 0;
	pos = listctrl->GetFirstSelectedItemPosition();
	while (pos)
	{
		item = listctrl->GetNextSelectedItem(pos);
		listctrl->GetItemRect(item, itemRC, LVIR_LABEL);

		CPoint	pt;
		CImageList* pimglstSingle = listctrl->CreateDragImage(item, &pt);
		
		if (pimglstSingle)
		{
			pimglstSingle->Draw(&memDC, 0, CPoint(0, itemRC.Height() * index++), ILD_MASK);
			delete pimglstSingle; pimglstSingle = nullptr;
		}
	}

	memDC.SelectObject(pOldMemDCBitmap);
	pCompleteImageList->Add(&bitmap, RGB(255, 255, 0)); 
	
	if (lpPoint)
	{
		lpPoint->x = itemRC.left;
		lpPoint->y = itemRC.top;
	}

	return pCompleteImageList.release();
}

void CPage2::SetAutoScroll(CListCtrl* listctrl, int direct)
{
	CRect	rc;
	CSize	size;

	listctrl->GetItemRect(0, rc, LVIR_LABEL);

	const int pos = listctrl->GetTopIndex();

	size.cx = 0;
	size.cy = rc.Height() * ((pos + direct) - pos);

	listctrl->Scroll(size);

	if (direct < 0)
		listctrl->Invalidate();

	m_autoScroll = true;
}

void CPage2::OKaction(int igubn)
{
	CString saveN; 
	saveN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);
	CString tempN; 
	tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);
	DeleteFile(saveN); 
	CopyFile(tempN, saveN, FALSE);

	if(igubn == 0)
		CCaction(); //20190423
}

void CPage2::CCaction()
{
	CString tempN; 
	tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);
	DeleteFile(tempN);
}

///////////////////////////////////////////////////////////////////////////////
// 
BOOL CPage2::AddColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) string;
	lvc.cx      = ctrl->GetStringWidth(lvc.pszText) + width;

	return ctrl->InsertColumn(item, &lvc);
}

int CPage2::AddItem(CListCtrl *ctrl, LPCSTR string, int nitem, int nSubItem)
{
	if (nSubItem < 0)
		return ctrl->InsertItem(LVIF_TEXT, nitem, string, 0, 0, 0, 0);
	else
		return ctrl->SetItemText(nitem, nSubItem, string);
}

int CPage2::_httoi(const TCHAR *value)
{
	const struct CHexMap
	{
		TCHAR chr{};
		int value{};
	};
	const int HexMapL = 16;
	const CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};
	TCHAR *mstr = _tcsupr(_tcsdup(value));
	TCHAR *s = mstr;
	int result = 0;
	if (*s == '0' && *(s + 1) == 'X') s += 2;
	bool firsttime = true;
	while (*s != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*s == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		s++;
		firsttime = false;
	}
	[[gsl::suppress(26408)]]
	free(mstr);
	return result;
}

CString CPage2::parseX(CString &srcstr, CString substr)
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

CString CPage2::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

COLORREF CPage2::getIndexColor(int color)
{
	if (color & 0x02000000)
		return color;

	return m_parent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

bool CPage2::GuideMsg(CString msg, bool question)
{
	UINT nType = MB_OK|MB_ICONEXCLAMATION;
	if (question)
		nType = MB_OKCANCEL|MB_ICONEXCLAMATION;

	if (MessageBox(msg, idTOstring(IDS_REGISTERINTER), nType) == IDCANCEL)
		return false;

	return true;
}

CString CPage2::idTOstring(UINT id)
{
	CString	msg;
	if (!msg.LoadString(id))
		msg = _T("");
	return msg;
}


COLORREF CPage2::GetAxColor(int color)
{
	if (color & 0x02000000)
		return color;

	return m_parent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}


BOOL CPage2::IsMenuStatusClosed()
{
	if ( m_bSet == TRUE) 
		return TRUE;
	else
		return FALSE;

}

void CPage2::OnChkOverlap() 
{
	// TODO: Add your control notification handler code here
//	saveConfig();
}


BOOL CPage2::endSendAction() //저 장 버튼 클릭시
{
	CString string;
	string = "OnPortfolio\tok";
	m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);
	m_parent->SendMessage(WM_USER, closeDLL, 0);
	return CPropertyPage::OnKillActive();
}

void CPage2::OnBnClickedBtnSetting()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDlg_Slider dlg;
	dlg.m_root = m_root;
	dlg.m_bCustomer = m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L);
	dlg.DoModal();
}



void  CPage2::OnRadioMemo(UINT id)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (id == IDC_RADIO_MEMOCLICK)
		_bMemo = TRUE;
	else
		_bMemo = FALSE;
}
