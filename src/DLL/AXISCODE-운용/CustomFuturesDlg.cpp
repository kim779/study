// CustomFuturesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CustomFuturesDlg.h"
#include "AxisCode.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STANDARDNUM		11


template <class T, class PT> class CQArray : public CArray <T, PT>
{
public:
	void QuickSort(BOOL bAscending = TRUE);
};

template <class T, class TP> void CQArray<T,TP>::QuickSort(BOOL bAscending/* = TRUE*/)
{
	if (GetSize() > 1)
	{
		::QuickSort(GetData(), GetSize(), bAscending);
	}
}

template <class T> BOOL QuickSort(T *pArr, int iSize, BOOL bAscending = TRUE)
{
	BOOL rc = TRUE;

	if (iSize > 1) 
	{
		try 
		{
			const int low = 0, high = iSize - 1;

			QuickSortRecursive(pArr, low, high, bAscending);
		} 
		catch (...) 
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}
	} 
	else 
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}

	return rc;
}

template <class T> void QuickSortRecursive(T *pArr, int low, int high, BOOL bAscending)
{
	int ii{}, jj{};
	T str;
	T TTemp;

	ii = high;
	jj = low;

	str = pArr[((int) ((low+high) / 2))];

	do {
		if (bAscending) 
		{
			while (pArr[jj].Compare(str) < 0) jj++;
			while (pArr[ii].Compare(str) > 0) ii--;
		}
		else
		{
			while (pArr[jj].Compare(str) > 0) jj++;
			while (pArr[ii].Compare(str) < 0) ii--;
		}
		if (ii >= jj) 
		{
			if (ii != jj) 
			{
				TTemp = pArr[ii];
				pArr[ii] = pArr[jj];
				pArr[jj] = TTemp;
			}
			ii--;
			jj++;
		}
	} while (jj <= ii);

	if (low < ii) QuickSortRecursive(pArr, low, ii, bAscending);
	if (jj < high) QuickSortRecursive(pArr, jj, high, bAscending);

}
/////////////////////////////////////////////////////////////////////////////
// CCustomFuturesDlg dialog


CCustomFuturesDlg::CCustomFuturesDlg(int kind, CPoint pt, int type, CString szcode /*= 0*/, CWnd* pParent /*=nullptr*/)
	: CDialog(CCustomFuturesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomFuturesDlg)
	m_szEditCode = _T("");
	m_szEditName = _T("");
	m_kindList = 0;
	//}}AFX_DATA_INIT

	m_szCode = _T("");
	const int len = szcode.GetLength();
	if (len)
	{
		if (szcode.GetAt(0) == '@')
		{
			szcode = szcode.Mid(1);
			switch (atoi(szcode))
			{
			case CC_FUTURE:
				m_szCode.Format("101");
				break;
			case CC_KOSTAR:
				m_szCode.Format("103");
				break;
			case CC_SFUTURE:
				m_szCode.Format("1XX");
				break;
			case CC_KOPTION:
				m_szCode.Format("2XX");
				break;
			}
		}
		else if (len >= 3)
		{
			switch (szcode.GetAt(0))
			{
			case '1':
			case '2':
			case '3':
			case '4':
				szcode.TrimLeft();
				szcode.TrimRight();

				if (atoi(szcode.Left(3)) == 0)
					szcode = _T("");
				
				m_szCode.Format("%s", szcode);			
				break;
			}			
		}
	}
	
	m_pCheck = nullptr;		
	RemoveAllData();

	m_Kind = kind;
	m_type = type;
	m_point = pt;
	m_nCountMonth = 0;
	m_nBorder = 0;
	m_type = CorrectType(type);
	m_szFindText = _T("");
	m_nSavePos = -1;
	m_bFind = false;
}	


void CCustomFuturesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomFuturesDlg)
	DDX_Control(pDX, IDC_BUTTON_SPMONTH9, m_ctlButtonSPMonth9);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH6, m_ctlButtonSPMonth6);
	DDX_Control(pDX, IDC_BUTTON_SPMONTH3, m_ctlButtonSPMonth3);
	DDX_Control(pDX, IDC_BUTTON_KOSTAR, m_ctlButtonKostar);
	DDX_Control(pDX, IDC_PCODE_LIST, m_plistbox);
	DDX_Control(pDX, IDC_EDIT_NAME, m_ctlEditName);
	DDX_Control(pDX, IDC_EDIT_CODE, m_ctlEditCode);
	DDX_Control(pDX, IDC_BUTTON_MONTH12, m_ctlButtonMonth12);
	DDX_Control(pDX, IDC_BUTTON_MONTH3, m_ctlButtonMonth3);
	DDX_Control(pDX, IDC_BUTTON_MONTH6, m_ctlButtonMonth6);
	DDX_Control(pDX, IDC_BUTTON_MONTH9, m_ctlButtonMonth9);
	DDX_Control(pDX, IDC_BUTTON_NAME, m_ctlButtonName);
	DDX_Control(pDX, IDC_BUTTON_KOSPI200, m_ctlButtonKospi);
	DDX_Text(pDX, IDC_EDIT_CODE, m_szEditCode);
	DDX_Text(pDX, IDC_EDIT_NAME, m_szEditName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomFuturesDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomFuturesDlg)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTON_KOSPI200, OnButtonKospi200)
	ON_LBN_SELCHANGE(IDC_PCODE_LIST, OnSelchangePcodeList)
	ON_BN_CLICKED(IDC_BUTTON_KOSTAR, OnButtonKostar)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH3, OnButtonSpmonth3)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH6, OnButtonSpmonth6)
	ON_BN_CLICKED(IDC_BUTTON_SPMONTH9, OnButtonSpmonth9)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_MONTH3, OnButtonMonth3)
	ON_BN_CLICKED(IDC_BUTTON_MONTH6, OnButtonMonth6)
	ON_BN_CLICKED(IDC_BUTTON_MONTH9, OnButtonMonth9)
	ON_BN_CLICKED(IDC_BUTTON_MONTH12, OnButtonMonth12)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomFuturesDlg message handlers

void CCustomFuturesDlg::RemoveAllData()
{
	m_arrayFcode.RemoveAll();
	m_arrayOcode.RemoveAll();	
	m_arrayPcode.RemoveAll();
	m_arraycall.RemoveAll();
	m_arrayput.RemoveAll();
	m_arraySfCode.RemoveAll();
	m_ListPcode.RemoveAll();
}
void CCustomFuturesDlg::CreateChild()
{
	m_pCheck = std::make_unique<CCustomCheckBox>();
	
	CString szTitle;
	m_rcCheck.SetRect(110, 42, 447, 267);		

	m_pCheck->Create(nullptr, nullptr, WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_GROUP, m_rcCheck, this, IDC_WND_CHECK);
	m_pCheck->ShowWindow(SW_SHOW);
	m_pCheck->SetWnd(this);
}

void CCustomFuturesDlg::init()
{
	SetDlgPos(m_point.x, m_point.y);
	m_ctlEditCode.SetLimitText(6);
	SetButton();	

	m_ctlButtonName.SetFlat(TRUE);
	m_ctlButtonName.m_bNoFocus = TRUE;
	m_ctlButtonName.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
	m_ctlButtonName.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COR_BACK);
	m_ctlButtonName.SetColor(CButtonST::BTNST_COLOR_BK_IN, COR_BACK);

	SetWindowText(MakeTitle());
	OrderEnable();
}

BOOL CCustomFuturesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CreateChild();	
	init();
	loadfile();	
	SetTimer(1000, 1, nullptr);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomFuturesDlg::SetDlgPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	int	scWidth{}, scHeight{};
	scWidth	 = GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CAxisCodeApp* pApp = (CAxisCodeApp *)AfxGetApp();
	const int virScrX = pApp->GetVirtualScreenX();
	if (virScrX > 0)
		scWidth = virScrX;

	CRect	rc;
	GetClientRect(&rc);

	const int	gabX = scWidth - (x + rc.Width());
	const int	gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(nullptr, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
}

void CCustomFuturesDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_arrayFcode.RemoveAll();
	m_arrayOcode.RemoveAll();
	m_arrayPcode.RemoveAll();
	m_arraycall.RemoveAll();
	m_arrayput.RemoveAll();
	m_ListPcode.RemoveAll();

	KillTimer(1000);
}

BOOL CCustomFuturesDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	CRect rc(0, 41, 321, 225);

	if (m_pCheck->IsWindowEnabled())
	{
		if(zDelta > 0)
		{
			// 위 처리...
			if (m_pCheck->GetInit() < 40)
			{
				m_pCheck->SetInit(m_pCheck->GetInit() + 20); 
				m_pCheck->SetScroll(m_pCheck->GetScroll() - 20);
			}
			
			m_pCheck->InvalidateRect(rc);
			m_pCheck->SetScrollPos32(SB_VERT, m_pCheck->GetScroll());
			
		}
		else
		{
			// 아래 처리....
			if (m_pCheck->GetInit() > -20*((m_pCheck->GetCount()-1)-10))
			{
				m_pCheck->SetInit(m_pCheck->GetInit() - 20); 
				m_pCheck->SetScroll(m_pCheck->GetScroll() + 20);
			}
			
			
			m_pCheck->InvalidateRect(rc);
			m_pCheck->SetScrollPos32(SB_VERT, m_pCheck->GetScroll());
			
		}
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CCustomFuturesDlg::HideButton(BOOL flag)
{
	for (int ii = IDC_BUTTON_MONTH3; ii < IDC_BUTTON_MONTH12; ii++)
	{
		if (flag)
			((CButton*) GetDlgItem(ii))->ShowWindow(SW_HIDE);
		else
			((CButton*) GetDlgItem(ii))->ShowWindow(SW_SHOW);
	}
}



void CCustomFuturesDlg::OnButtonKospi200() 
{
	Select(CC_FUTURE);
}

void CCustomFuturesDlg::OnButtonKostar() 
{
	Select(CC_KOSTAR);
}


void CCustomFuturesDlg::OnButtonSpmonth3() 
{
	CString	str;
	m_ctlButtonMonth3.GetWindowText(str);
	if (str.IsEmpty())
		return;

	if (str.GetAt(0) == 'S')
	{
		CString	code = _T(""), name = _T("");
		if (GetCurrentSFCODE(code, name, 4))
		{
			MonthInit();
			SetCode(code);
			SetName(name);
		}
	}
	else
	{
		bool	bKospi = false;
		if (str.Find("스타") < 0)
			bKospi = true;
		FCODE fcode = m_arrayFcode.GetAt(4 + (!bKospi?7:0));

		MonthInit();
		SetCode(fcode.code);
		SetName(fcode.name);
		UpdateData(FALSE);	
	}

	CDialog::OnOK();
}

void CCustomFuturesDlg::OnButtonSpmonth6() 
{
	CString	str;
	m_ctlButtonMonth3.GetWindowText(str);
	if (str.IsEmpty())
		return;

	if (str.GetAt(0) == 'S')
	{
		CString	code = _T(""), name = _T("");
		if (GetCurrentSFCODE(code, name, 5))
		{
			MonthInit();
			SetCode(code);
			SetName(name);
		}
	}
	else
	{
		bool	bKospi = false;
		if (str.Find("스타") < 0)
			bKospi = true;
		FCODE fcode = m_arrayFcode.GetAt(5 + (!bKospi?7:0));

		MonthInit();
		SetCode(fcode.code);
		SetName(fcode.name);
		UpdateData(FALSE);	
	}	

	CDialog::OnOK();
}

void CCustomFuturesDlg::OnButtonSpmonth9() 
{
	CString	str;
	m_ctlButtonMonth3.GetWindowText(str);
	if (str.IsEmpty())
		return;

	if (str.GetAt(0) == 'S')
	{
		CString	code = _T(""), name = _T("");
		if (GetCurrentSFCODE(code, name, 6))
		{
			MonthInit();
			SetCode(code);
			SetName(name);
		}
	}
	else
	{
		bool	bKospi = false;
		if (str.Find("스타") < 0)
			bKospi = true;
		FCODE fcode = m_arrayFcode.GetAt(6 + (!bKospi?7:0));

		MonthInit();
		SetCode(fcode.code);
		SetName(fcode.name);
		UpdateData(FALSE);	
	}

	CDialog::OnOK();
}


void CCustomFuturesDlg::ButtonText(CString szName, CString szCode)
{
	if (m_Kind == optionCODE)
	{
		szCode = _T("");
		m_ctlButtonName.ShowWindow(SW_SHOW);
	}
	else
		szCode = "(" + szCode + ")" + " 기초자산";
	
	m_ctlButtonName.SetWindowText(szName + szCode);
}



void CCustomFuturesDlg::OnButtonClose() 
{
	CDialog::OnOK();
}

BOOL CCustomFuturesDlg::loadPjCode()			//현물 코드 종목...
{
	if (!(m_type & CC_KOPTION))
		return FALSE;
	if (m_arrayPcode.GetSize() > 0)
		return FALSE;

	int		codeN{};
	CFile		file;
	struct  pjcode  PJCode {};
	PCODE		pcode{};
	CString		path;
	CMapStringToPtr pcodemap;
	CMapStringToString sfcodemap;
	CQArray	<CString, CString>	qKindArr;
  	
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\pjcode.dat";

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{
		//MessageBox("파일[pjcode.dat]이 존재하지 않습니다.");
		return FALSE;
	}
								// 데이터의 갯수...
	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct pjcode));

	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&PJCode, sizeof(struct pjcode));
		
		pcode.kind = GetString(PJCode.tjgb, 2);
		pcode.name = GetString(PJCode.snam, 20);
		pcode.name.TrimRight();
		pcode.code = GetString(PJCode.codx, PCodeLen);
		pcode.hnam = GetString(PJCode.hnam, sizeof(PJCode.hnam)); pcode.hnam.TrimRight();
		pcode.month = GetString(PJCode.mont, 4);
		pcode.price = GetString(PJCode.hsga, 8);
		pcode.gcod  = GetString(PJCode.gcod, 12);
		pcode.mchk = PJCode.mchk;
		
		if (PJCode.atmx == 1)
			pcode.atm = TRUE;
		else
			pcode.atm = FALSE;

		pcodemap.SetAt(pcode.kind, (void*)m_arrayPcode.Add(pcode));
	}
	file.Close();

	POSITION pos = pcodemap.GetStartPosition();
	CString sKind;
	int value{};
	CString	sztmp = _T("");
	if (m_plistbox.m_hWnd && IsWindow(m_plistbox.m_hWnd))
	{	
		
		if (!(m_type & CC_SFUTURE))
			m_plistbox.ResetContent();
		else
		{
			const int ncnt = m_plistbox.GetCount();
			m_plistbox.SetItemData(m_nBorder = m_plistbox.AddString("=========="), -1);
									
			for (int  ii = 0 ; ii < m_nBorder ; ii++ )
			{
				m_plistbox.GetText(ii, sztmp);
				sfcodemap.SetAt(sztmp, "");
			}
		}
		while (pos)
		{
			pcodemap.GetNextAssoc(pos, sKind, (void*&)value);
			qKindArr.Add(sKind);
		}

		qKindArr.QuickSort();
		
		for (int ii = 0; ii < qKindArr.GetSize(); ii++)
		{
			sKind = qKindArr.GetAt(ii);
			if (pcodemap.Lookup(sKind, (void*&)value))
			{
				if (m_type && CC_SFUTURE)
				{
					CString	sztmp2 ;
					sztmp = m_arrayPcode.GetAt(value).name;
					if (sfcodemap.Lookup(sztmp, sztmp2))
						continue;
				}
				m_plistbox.SetItemData(m_plistbox.AddString(m_arrayPcode.GetAt(value).name), value);
			}
		}
	}	
	return TRUE;
}

BOOL CCustomFuturesDlg::loadFjCode()
{
	if (m_arrayFcode.GetSize() > 0)
		return FALSE;
	
	int	codeN{};

	CFile	file;
	struct  fjcode  FJCode {};
	FCODE fcode;
	CString path;
  	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\fjcode.dat";
	
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{
		MessageBox("파일[fjcode.dat]이 존재하지 않습니다.");
		return FALSE;
	}
								// 데이터의 갯수...
	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct fjcode));

	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&FJCode, sizeof(struct fjcode));
		
		fcode.code = CString(FJCode.cod2, FCodeLen);
		fcode.name = CString(FJCode.hnam, FNameLen);
		fcode.mchk = FJCode.mchk;
		
		m_arrayFcode.Add(fcode);
	}

	file.Close();
	return TRUE;
}

BOOL CCustomFuturesDlg::loadOjCode()
{									// 옵션 코드 종목...	
	if (m_arrayOcode.GetSize() > 0)
		return FALSE;
	
	int	codeN{}, len{};

	CFile	file;
	struct  ojcodh  OJCodh{};
	

	CString path;
  	
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\opcode.dat";

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{	
		//MessageBox("파일[opcode.dat]이 존재하지 않습니다.");
		return FALSE;
	}
	len = file.Read(&OJCodh, sizeof(struct ojcodh));

	for (int jj = 0; jj < STANDARDNUM; jj++)
	{
		CString str = CString(OJCodh.cjym[jj], 6).Right(4);
		m_ArrayMonth.Add(str);
	}

	if (m_pCheck->GetSafeHwnd())
		m_pCheck->SetArray();
	
	m_ArrayMonth.RemoveAll();

	codeN = gsl::narrow_cast<int>((file.GetLength() - len) / sizeof(struct ojcode));

	for (int ii = 0; ii < codeN; ii++)
	{
		struct  ojcode  OJCode;
		file.Read(&OJCode, sizeof(struct ojcode));
		m_arrayOcode.Add(OJCode);
	}

	file.Close();
	return TRUE;
}

void CCustomFuturesDlg::Standard(CString szCode, CString codx)		// 콜옵션 풋옵션..
{
	PCODE pcode;

	loadPjCode();

	for (int ii = 0; ii < m_arrayPcode.GetSize(); ii++)
	{
		pcode = m_arrayPcode.GetAt(ii);
	
		if (!pcode.kind.Compare(szCode))
		{
			if (pcode.code.Left(1).Compare("2") == 0)	// 콜
				m_arraycall.Add(pcode);
			if (pcode.code.Left(1).Compare("3") == 0)	// 풋
				m_arrayput.Add(pcode);
		}
	}

	// 이후에...
	SearchPrice();
	SearchMonth();
}

void CCustomFuturesDlg::SearchPrice()
{
	PCode pcode, temp;
	POSITION pos{};

	for (int ii= 0; ii < m_arraycall.GetSize(); ii++)
	{
		pcode = m_arraycall.GetAt(ii);

		if (m_ListPcode.IsEmpty())
			m_ListPcode.AddHead(pcode);
		
		if (!FindPrice(pcode.price))
			AddList(pcode);			
			
	}

	pos = m_ListPcode.GetHeadPosition();
	
	for (int ii = 0; ii < m_ListPcode.GetCount(); ii++)
	{
		pcode = m_ListPcode.GetAt(pos);
		m_ArrayPrice.Add(pcode.price);
		m_ListPcode.GetNext(pos);
	}
	
	m_ListPcode.RemoveAll();

}

BOOL CCustomFuturesDlg::FindPrice(CString szCompare)
{
	if(m_ListPcode.IsEmpty())
		return FALSE;

	PCode pcode;
	POSITION pos = m_ListPcode.GetHeadPosition();

	for (int ii = 0; ii < m_ListPcode.GetCount(); ii++)
	{
		pcode = m_ListPcode.GetAt(pos);
		if (pcode.price == szCompare)
			return TRUE;
		m_ListPcode.GetNext(pos);
	}

	return FALSE;
}

void CCustomFuturesDlg::AddList(const PCODE &pcode)
{
	POSITION pos = m_ListPcode.GetHeadPosition();
	PCODE temp;

	for (int ii = 0; ii < m_ListPcode.GetCount(); ii++)
	{
		temp = m_ListPcode.GetAt(pos);
		
		if (pcode.price.Compare(temp.price) > 0)
		{
			m_ListPcode.AddHead(pcode);
			return;
		}
		
		m_ListPcode.GetNext(pos);
	}
}

void CCustomFuturesDlg::SearchMonth()
{
	PCode pcode, temp;
	CString szTemp;
	
	POSITION pos{};

	for (int ii= 0; ii < m_arraycall.GetSize(); ii++)
	{
		pcode = m_arraycall.GetAt(ii);

		if (m_ListMonth.IsEmpty())
			m_ListMonth.AddHead(pcode.month);
		
		if (!FindMonth(pcode.month))
			m_ListMonth.AddHead(pcode.month);	
	}

	pos = m_ListMonth.GetHeadPosition();
	
	for (int ii = 0; ii < m_ListMonth.GetCount(); ii++)
	{
		szTemp = m_ListMonth.GetAt(pos);
		m_ArrayMonth.Add(szTemp);
		m_ListMonth.GetNext(pos);
	}
	
	m_nCountMonth = m_ArrayMonth.GetSize();

	m_ListMonth.RemoveAll();

}

BOOL CCustomFuturesDlg::FindMonth(CString szMon)
{
	if(m_ListMonth.IsEmpty())
		return FALSE;

	CString sztemp;
	POSITION pos = m_ListMonth.GetHeadPosition();

	for (int ii = 0; ii < m_ListMonth.GetCount(); ii++)
	{
		sztemp = m_ListMonth.GetAt(pos);
		if (sztemp == szMon)
			return TRUE;
		m_ListMonth.GetNext(pos);
	}

	return FALSE;
}

_DATA CCustomFuturesDlg::FindCall(CString szMonth, CString szPrice)
{
	_DATA data;
	PCODE code;
	
	for (int ii = 0; ii < m_arraycall.GetSize(); ii++)
	{
		code = m_arraycall.GetAt(ii);
		if (code.month == szMonth && code.price == szPrice)
		{
			data.flag = TRUE;
			data.code = code.code;
//			code.name.TrimRight();
//	             	data.name.Format("%s %s %s",code.name, code.hnam, code.month);
			data.name = code.hnam;
			data.atm = code.atm;

			return data;
		}
		else
		{
			data.flag = FALSE;
			data.code = "";
			data.name = "";
		}

	}

	return data;

}

_DATA CCustomFuturesDlg::FindPut(CString szMonth, CString szPrice)
{
	_DATA data;
	PCODE code;
	
	for (int ii = 0; ii < m_arrayput.GetSize(); ii++)
	{
		code = m_arrayput.GetAt(ii);
		if (code.month == szMonth && code.price == szPrice)
		{
			data.flag = TRUE;
			data.code = code.code;
//			code.name.TrimRight();
//			data.name.Format("%s %s %s",code.name, code.hnam, code.month);
			data.name = code.hnam;
		
			return data;
		}
		else
		{
			data.flag = FALSE;
			data.code = "";
			data.name = "";
		}
	}

	return data;
}

void CCustomFuturesDlg::FindEnd()
{
	m_pCheck->SetArray();
	m_ArrayMonth.RemoveAll();
	m_ArrayPrice.RemoveAll();
	m_arraycall.RemoveAll();
	m_arrayput.RemoveAll();
}

void CCustomFuturesDlg::CountData(CString code, CString codx)		// 데이터 처리..
{
	Standard(code, codx);

	_DATA data;
	const __POSITION* pos = m_ListMonth.GetHeadPosition();
	CString szMonth, szPrice;
	
	m_pCheck->RemoveAll();
	m_pCheck->SetCount(m_ArrayPrice.GetSize());
	
	// 열 ... call 쪽...
	for (int ii = 0; ii < STANDARDNUM; ii++)
	{
		szMonth = "";
		for (int jj = 0; jj < m_ArrayPrice.GetSize(); jj++) // 행...
		{
			if (m_ArrayMonth.GetSize() > ii)
				szMonth = m_ArrayMonth.GetAt(ii);
			szPrice = m_ArrayPrice.GetAt(jj);
			
			data = FindCall(szMonth, szPrice);
			m_pCheck->AddData(jj, ii, data);
			
			data.code = "";
			data.flag = FALSE;
			szPrice.TrimLeft();
			//				data.name = szPrice;
			data.name = CommaModify(szPrice);
			m_pCheck->AddData(jj, STANDARDNUM, data);
			
			if (data.atm == 1)		// atm..
				m_nAtm = jj;
		}
	}
	
	// 열 ... put 쪽...
	for (int kk = 0; kk < STANDARDNUM; kk++)
	{
		szMonth = "";
		for (int jj = 0; jj < m_ArrayPrice.GetSize(); jj++) // 행...
		{
			if (m_ArrayMonth.GetSize() > kk)
				szMonth = m_ArrayMonth.GetAt(kk);
			szPrice = m_ArrayPrice.GetAt(jj);
			data = FindPut(szMonth, szPrice);
			m_pCheck->AddData(jj, STANDARDNUM + kk + 1, data);	
		}
	}
	
	m_pCheck->Invalidate();
	FindEnd();
}

CString CCustomFuturesDlg::CommaModify(CString string)
{
	CString stripData = string;
	stripData.TrimLeft(); stripData.TrimRight();

	if (stripData.GetLength() <= 3)
		return stripData;

	char dChar = 0x00;
	switch (stripData[0])
	{
	case '+':
		dChar = '+'; stripData.Delete(0);
		break;
	case '-':
		dChar = '-'; stripData.Delete(0);
		break;
	}

	stripData.MakeReverse();
	string = _T("");
	for (int ii = 0; ii < stripData.GetLength(); ii++)
	{
		if ((ii != 0) && ((ii % 3) == 0))
			string += ',';

		string += stripData[ii];
	}

	string.MakeReverse();
	if (dChar != 0x00) string.Insert(0, dChar);
	return string;
}

void CCustomFuturesDlg::DisableButton(long lExcept)
{
	m_plistbox.EnableWindow(FALSE);
}

void CCustomFuturesDlg::SetButton()
{
	SetButton(&m_ctlButtonMonth12);

	SetButton(&m_ctlButtonMonth3);

	SetButton(&m_ctlButtonMonth6);

	SetButton(&m_ctlButtonMonth9);

	SetButton(&m_ctlButtonSPMonth3);

	SetButton(&m_ctlButtonSPMonth6);

	SetButton(&m_ctlButtonSPMonth9);

	SetButton(&m_ctlButtonKostar);

	SetButton(&m_ctlButtonKospi);
}

void CCustomFuturesDlg::SetButton(CButtonST *pButton)
{
	const short	shBtnColor = 245;
	pButton->SetFlat(FALSE);
	pButton->OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	pButton->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
	pButton->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, USER_FOCUS_COR);
	pButton->SetBtnCursor(IDC_CURSOR_HAND);
	pButton->SetBold(FALSE);
	pButton->SetSelected(FALSE);
}

void CCustomFuturesDlg::InitBtn()
{
	m_ctlButtonKospi.SetBold(FALSE);
	m_ctlButtonKostar.SetBold(FALSE);	
}

void CCustomFuturesDlg::MonthInit()
{
	m_ctlButtonMonth12.SetBold(FALSE);
	m_ctlButtonMonth9.SetBold(FALSE);
	m_ctlButtonMonth6.SetBold(FALSE);
	m_ctlButtonMonth3.SetBold(FALSE);
}


BOOL CCustomFuturesDlg::FindCode(int kind, CString szCode)		// GetName()에서 사용. 
{
	FCODE	fcode{};
	ojcode	OJcode{};
	PCODE	pcode{};
	SFCODE	sfcode{};

	szCode.TrimRight();

	switch(kind)
	{
	case whichTYPE:
		{
			m_jongmuk = 0;
			
			if (m_arrayFcode.GetSize() > 0)
			{
				for(int ii = 0; ii < m_arrayFcode.GetSize(); ii++)
				{
					fcode = m_arrayFcode[ii];
					if (szCode.CompareNoCase(fcode.code) == 0) // fcode.code == szCode)
					{
						fcode.name.TrimRight();
						SetName(fcode.name);
						m_arrayFcode.RemoveAll();

						m_jongmuk = futureType;
						return TRUE;
					}
				}
			}

			if (m_arrayOcode.GetSize() > 0)
			{
				for (int ii = 0; ii < m_arrayOcode.GetSize(); ii++)
				{
					OJcode = m_arrayOcode.GetAt(ii);
					
					for (int jj = 0; jj < STANDARDNUM ; jj++)
					{
						if (szCode.CompareNoCase(OJcode.call[jj].cod2) == 0)  // == szCode)
						{
							SetName(OJcode.call[jj].hnam);
							m_arrayOcode.RemoveAll();
							m_jongmuk = callType;
							return TRUE;
						}

						if (szCode.CollateNoCase(OJcode.put[jj].cod2) == 0) // == szCode)
						{
							SetName(OJcode.put[jj].hnam);
							m_arrayOcode.RemoveAll();
							m_jongmuk = putType;
							return TRUE;
						}
					}
				
				}
			}

			if (m_arraySfCode.GetSize() > 0)
			{
				for ( int ii = 0 ; ii < m_arraySfCode.GetSize() ; ii++ )
				{
					sfcode = m_arraySfCode.GetAt(ii);
					if (szCode.CompareNoCase(sfcode.codx) == 0) // == szCode)
					{
						sfcode.hnam.TrimRight();
						CString temp =  sfcode.hnam;
						SetName(temp);
						m_arraySfCode.RemoveAll();
						m_jongmuk = sfutureType;
						return TRUE;
					}
				}
			}

			if (m_arrayPcode.GetSize() > 0)
			{

				for (int ii = 0; ii < m_arrayPcode.GetSize(); ii++)
				{
					pcode = m_arrayPcode.GetAt(ii);

					if (szCode.CompareNoCase(pcode.code) == 0) // == szCode)
					{
						pcode.name.TrimRight();
						CString temp =  pcode.hnam;
						SetName(temp);
						m_arrayPcode.RemoveAll();
						m_jongmuk = koptionType;
						return TRUE;
					}
				}
			}
		}
		break;
	case futureNAME:
		{
			if (m_arrayFcode.GetSize() < 0)
				return FALSE;

			for(int ii = 0; ii < m_arrayFcode.GetSize(); ii++)
			{
				fcode = m_arrayFcode[ii];
				if (szCode.CompareNoCase(fcode.code) == 0) // fcode.code == szCode)
				{
					fcode.name.TrimRight();
					SetName(fcode.name);
					m_arrayFcode.RemoveAll();
					return TRUE;
				}
			}
		}
		break;
	case optionNAME:
		{
			if (m_arrayOcode.GetSize() < 0)
				return FALSE;

			for (int ii = 0; ii < m_arrayOcode.GetSize(); ii++)
			{
				OJcode = m_arrayOcode.GetAt(ii);
				
				for (int jj = 0; jj < STANDARDNUM ; jj++)
				{
					if (szCode.CompareNoCase(OJcode.call[jj].cod2) == 0)  // == szCode)
					{
						SetName(OJcode.call[jj].hnam);
						m_arrayOcode.RemoveAll();
						return TRUE;
					}

					if (szCode.CollateNoCase(OJcode.put[jj].cod2) == 0) // == szCode)
					{
						SetName(OJcode.put[jj].hnam);
						m_arrayOcode.RemoveAll();
						return TRUE;
					}
				}
			
			}
		}
		break;
	case sfutureNAME:
		{
			if (m_arraySfCode.GetSize() < 0)
				return FALSE;

			for (int ii = 0; ii < m_arraySfCode.GetSize(); ii++)
			{
				sfcode = m_arraySfCode.GetAt(ii);
				
				if (szCode.CompareNoCase(sfcode.codx) == 0) // == szCode)
				{
					sfcode.hnam.TrimRight();
					CString temp =  sfcode.hnam;
					SetName(temp);
					m_arraySfCode.RemoveAll();
					return TRUE;
				}
			
			}
		}

		break;
	case koptionNAME:
		{
			if (m_arrayPcode.GetSize() <=  0)
				return FALSE;

			for (int ii = 0; ii < m_arrayPcode.GetSize(); ii++)
			{
				pcode = m_arrayPcode.GetAt(ii);

				if (szCode.CompareNoCase(pcode.code) == 0) // == szCode)
				{
					pcode.name.TrimRight();
//					CString temp =  pcode.name+ " " + pcode.hnam + " " + pcode.month;
					CString temp =  pcode.hnam;
					SetName(temp);
					m_arrayPcode.RemoveAll();
					return TRUE;
				}
			}

		}
		break;
	case getCODEfuture:
		{
			if (m_arrayFcode.GetSize() < 0)
				return FALSE;

			fcode = m_arrayFcode[0];
			
			//	SetName(fcode.name);
				SetCode(fcode.code);

				m_arrayFcode.RemoveAll();
				return TRUE;
		}
		break;
	case getCODEcall:
		{
			if (m_arrayOcode.GetSize() < 0)
				return FALSE;
			
			for (int ii = 0; ii < m_arrayOcode.GetSize(); ii++)
			{
				OJcode = m_arrayOcode.GetAt(ii);
				
				if (OJcode.atmg == 1)
				{
				//	SetName(OJcode.call[3].hnam);
					SetCode(OJcode.call[3].cod2);
					m_arrayOcode.RemoveAll();
					return TRUE;
				}
			
			}

		}
		break;
	case getCODEput:
		{
			if (m_arrayOcode.GetSize() < 0)
				return FALSE;
			
			for (int ii = 0; ii < m_arrayOcode.GetSize(); ii++)
			{
				OJcode = m_arrayOcode.GetAt(ii);
				
				if (OJcode.atmg == 1)
				{	
				//	SetName(OJcode.put[0].hnam);
					SetCode(OJcode.put[0].cod2);
					m_arrayOcode.RemoveAll();
					return TRUE;
				}
			}

		}
		break;
	case basicCODE:
		{
			const TCHAR	ch1 = szCode.GetAt(0);
			const TCHAR	ch2 = szCode.GetAt(1);
			CString	tjgb = szCode.Mid(1, 2);

			switch (ch1)
			{
			case '1':
			case '4':
			case 'A':  //파생상품 코드개편
			case 'D':
				{
					if (ch2 != '0')
					{
						loadSfCode();
						
						for ( int ii = 0 ; ii < m_arraySfCode.GetSize() ; ii++ )
						{
							sfcode = m_arraySfCode.GetAt(ii);
							if (tjgb.CompareNoCase(sfcode.tjgb) == 0)
							{
								m_szBCode = GetShortCode(sfcode.gcod);
								m_szBName = sfcode.gnam;
								return TRUE;
							}
						}
					}
				}
				break;
			case '2':
			case '3':
			case 'B':  //파생상품 코드개편
			case 'C':
				{
					if (ch2 != '0')
					{
						loadPjCode();
						for ( int ii = 0 ; ii < m_arrayPcode.GetSize() ; ii++ )
						{
							pcode = m_arrayPcode.GetAt(ii);
							if (tjgb.CompareNoCase(pcode.kind) == 0)
							{
								m_szBCode = GetShortCode(pcode.gcod);
								m_szBName = pcode.name;
								return TRUE;
							}
						}
					}
				}
				break;
			}		
		}
	}
	
	return FALSE;
}



bool CCustomFuturesDlg::GetCodeOrderable(CString code, int* type)
{
	bool	rValue = false;

	loadFjCode();
	m_nBorder = 0;
	loadSfCode();
	loadPjCode();
	loadOjCode();
	

	if (code.IsEmpty())
		return false;

	CString comp;
	CString kind = code.Left(2);
	switch (atoi(kind))
	{
	case 10: // 선물
	case 40: // 선물스프레드
		{
			FCODE	fcode;
			for (int ii = 0 ; ii < m_arrayFcode.GetSize() ; ii++)
			{
				fcode = m_arrayFcode.GetAt(ii);
				if (fcode.code.Compare(code) == 0)
				{
					*type = fcode.mchk;
					rValue = true;
					break;
				}
			}
		}
		break;
	case 20: // 콜옵션
		{
		ojcode	ojcode{};
			for (int ii = 0 ; ii < m_arrayOcode.GetSize() ; ii++)
			{
				ojcode = m_arrayOcode.GetAt(ii);
				for (int jj = 0 ; jj < STANDARDNUM ; jj++)
				{
					if (ojcode.call[jj].yorn == '1')
					{
						comp = CString(ojcode.call[jj].cod2, OCodeLen);
						if (comp.Compare(code) == 0)
						{
							*type = ojcode.call[jj].mchk;
							rValue = true;
							break;
						}
					}
				}

				if (rValue)
					break;
			}
		}
		break;
	case 30: // 풋옵션
		{ 
			ojcode	ojcode{};
			for (int ii = 0 ; ii < m_arrayOcode.GetSize() ; ii++)
			{
				ojcode = m_arrayOcode.GetAt(ii);
				for (int jj = 0 ; jj < STANDARDNUM ; jj++)
				{
					if (ojcode.put[jj].yorn == '1')
					{
						comp = CString(ojcode.put[jj].cod2, OCodeLen);
						if (comp.Compare(code) == 0)
						{
							*type = ojcode.put[jj].mchk;
							rValue = true;
							break;
						}
					}
				}

				if (rValue)
					break;
			}
		}
		break;
	default:
		{  //2024.10.31  주식선물 관련 추가
			if ("10" <= kind && kind <= "59" || "B0" <= kind && kind <= "ZZ" || "0A" <= kind && kind <= "0Z" || "1A" <= kind && kind <= "1Z" || "2A" <= kind && kind <= "2Z"
				|| "3A" <= kind && kind <= "3Z" || "4A" <= kind && kind <= "4Z" || "5A" <= kind && kind <= "5Z" || "6A" <= kind && kind <= "6Z" || "7A" <= kind && kind <= "7Z"
				|| "8A" <= kind && kind <= "8Z" || "9A" <= kind && kind <= "9Z")
			{
		/*		CString m_slog;
				m_slog.Format("[axiscode][SF][%s]<%d> code=[%s]", __FUNCTION__, __LINE__, code);
				OutputDebugString(m_slog);*/

				SFCODE	sfcode;
				for (int ii = 0 ; ii < m_arraySfCode.GetSize() ; ii++)
				{
					sfcode = m_arraySfCode.GetAt(ii);
					if (sfcode.codx.Compare(code) == 0)
					{
						*type = sfcode.mchk;
						rValue = true;
						break;
					}
				}
			}
// 			else if ((nval >= 21 && nval < 30) || (nval >= 31 && nval < 40)) // 주식옵션
// 			{
// 				PCODE	pcode;
// 				for (int ii = 0 ; ii < m_arrayPcode.GetSize() ; ii++)
// 				{
// 					pcode = m_arrayPcode.GetAt(ii);
// 					if (pcode.code.Compare(code) == 0)
// 					{
// 						*type = pcode.mchk;
// 						rValue = true;
// 						break;
// 					}
// 				}
// 			}	
		}	
	}

	return rValue;
}

void CCustomFuturesDlg::ClickList() 
{
	int nItem = m_plistbox.GetCurSel();
	if (nItem == LB_ERR)
		return;

	if (nItem == m_nBorder)
	{
		nItem = 0;
		m_plistbox.SetCurSel(0);
	}
	
	if (nItem < m_nBorder)
		Select(CC_SFUTURE);
	else
		Select(CC_KOPTION);		
}

void CCustomFuturesDlg::OnSelchangePcodeList() 
{
	ClickList();
}

CString CCustomFuturesDlg::GetString(char *pChar, int nMinLen)
{
	return CString(pChar, min(nMinLen, (int)strlen(pChar)));
}


void CCustomFuturesDlg::SetFutureCodes(BOOL bKostar, CString codx)
{
	int btnCnt = 0;
	for (int jj = 0; jj < m_arrayFcode.GetSize(); jj++)
	{
		FCODE fcode = m_arrayFcode.GetAt(jj);
		CString temp = CString(fcode.name), code;
		temp.TrimRight();
		code = m_arrayFcode[jj].code;
		code.TrimLeft(); code.TrimRight();
		if (bKostar)
			if (code.GetLength() <= 3 || code[2] != '3')
				continue;
		if (btnCnt > 7)
			break;

		if (code.IsEmpty())
			continue;
		if (code[0] == '4' || code[0] == 'D')  //상품선물 코드개편
			((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + btnCnt))->SetWindowText(code);
		else
		{
			if (bKostar)
				temp = "스타 " + temp.Right(4);
			else
				temp = "선물 " + temp.Right(4);
			((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + btnCnt))->SetWindowText(temp);			
		}
		btnCnt++;
	}	
}

/////////////////////////////////////////////////////
// 2006.11.14 이인호
/////////////////////////////////////////////////////

BOOL CCustomFuturesDlg::loadSfCode()
{
	return TRUE;	// 현재 주식 선물 없음
	if (m_arraySfCode.GetSize() > 0)
		return FALSE;
	
	int	codeN = 0, size = 0;

	CFile	file;
	struct  sfcode  sfread {};
	SFCODE		sfFirst{};
	SFCODE		sfsave{};
	int		nFirst = 0;
	CString path, sztmp = _T("");
	CMapStringToPtr			pcodemap;
	CArray<SFCODE, SFCODE>	arSFcode;
	CQArray	<CString, CString>	qKindArr;
	
  	
	path = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\sfcode.dat";

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{	
 		MessageBox("파일[sfcode.dat]이 존재하지 않습니다.");
		return FALSE;
	}

	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct sfcode));
	size = sizeof(struct sfcode);
	int	nUnit = 0;
	m_mapSFCode.RemoveAll();
	m_arraySfCode.RemoveAll();

	for (int ii = 0; ii < codeN; ii++)
	{
		nUnit = nUnit % 7;
		file.Read(&sfread, size);
		sfsave.codx.Format("%.*s", sizeof(sfread.codx), sfread.codx);
		sfsave.codx.TrimRight();

		sfsave.hnam.Format("%.*s", sizeof(sfread.hnam), sfread.hnam);
		sfsave.hnam.TrimRight();

		sfsave.enam.Format("%.*s", sizeof(sfread.enam), sfread.enam);
		sfsave.enam.TrimRight();

		sfsave.tjgb.Format("%.*s", sizeof(sfread.tjgb), sfread.tjgb);
		sfsave.tjgb.TrimRight();

		sfsave.mchk = sfread.mchk;
		
		sfsave.gcod.Format("%.*s", sizeof(sfread.gcod), sfread.gcod);
		sfsave.gcod.TrimRight();

		sfsave.gnam.Format("%.*s", sizeof(sfread.gnam), sfread.gnam);	
		sfsave.gnam.TrimRight();
		arSFcode.Add(sfsave);
		
		if (nUnit == 0)
		{
			nFirst = ii;
			sfFirst = sfsave;			
		}

		if (nUnit == 6)
		{
			const int ncnt = arSFcode.GetSize();
			for ( int jj = 0 ; jj < ncnt ; jj++ )
			{
				pcodemap.SetAt(arSFcode.GetAt(jj).tjgb, (void*)m_arraySfCode.Add(arSFcode.GetAt(jj)));	
			}

			sztmp.Format("%d", nFirst);
			m_mapSFCode.SetAt(sfFirst.tjgb, sztmp);	
			arSFcode.RemoveAll();
		}

		nUnit++;
	}

	file.Close();

	POSITION pos = pcodemap.GetStartPosition();
	CString sKind;
	int value{};

	if (m_plistbox.m_hWnd && IsWindow(m_plistbox.m_hWnd))
	{
		m_plistbox.ResetContent();
		while (pos)
		{
			pcodemap.GetNextAssoc(pos, sKind, (void*&)value);
			qKindArr.Add(sKind);
		}

		qKindArr.QuickSort();
		
		for (int ii = 0; ii < qKindArr.GetSize(); ii++)
		{
			sKind = qKindArr.GetAt(ii);
			if (pcodemap.Lookup(sKind, (void*&)value))
				m_plistbox.SetItemData(m_plistbox.AddString(m_arraySfCode.GetAt(value).gnam), value);
		}

		m_nBorder = m_plistbox.GetCount();
	}
	
	return TRUE;

}

CString CCustomFuturesDlg::GetShortCode(CString code)
{
	CString	ret = _T(code);
	const int len = code.GetLength();

	if (len >= 9)
		ret = code.Mid(3, 6);

	return ret;
}

void CCustomFuturesDlg::SetSFCodes(int nIdx, CString codx)
{
	CString	kind = m_arraySfCode.GetAt(nIdx).tjgb;
	int nIndex = 0;
	CString	sztmp = _T("");

	m_mapSFCode.Lookup(kind, sztmp);
	nIndex = atoi(sztmp);

	for (int jj = 0; jj < 7 ; jj++)
	{
		SFCODE sfcode = m_arraySfCode.GetAt(jj + nIndex);
		CString temp = CString(sfcode.hnam), code;
		temp.TrimRight();
		code = sfcode.codx;
		code.TrimLeft(); code.TrimRight();

		if (code.IsEmpty())
			continue;

		if (code[0] == '4' || code[0] == 'D')  //상품선물 코드개편
			((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + jj))->SetWindowText(code);
		else
		{
			temp = "S선물 " + temp.Right(4);
			((CButton*)GetDlgItem(IDC_BUTTON_MONTH3 + jj))->SetWindowText(temp);
		}	
	}
}

BOOL CCustomFuturesDlg::GetCurrentSFCODE(CString& code, CString& name, int nIndex)
{	
	int	nPos = m_plistbox.GetCurSel();
	if (nPos == LB_ERR)
		return FALSE;

	const int	nval = m_plistbox.GetItemData(nPos);
	SFCODE	sfcode = m_arraySfCode.GetAt(nval);
	CString	kind = sfcode.tjgb;
	CString	sztmp;
	m_mapSFCode.Lookup(kind, sztmp);
	nPos = atoi(sztmp);
	sfcode = m_arraySfCode.GetAt(nPos + nIndex);
	code = sfcode.codx;
	name = sfcode.hnam;
	return TRUE;
}


LRESULT CCustomFuturesDlg::OnUser(WPARAM wParam, LPARAM lParam)
{
	CString	sztmp = _T("");
	if (wParam == IDC_EDIT_CODE)
	{
		m_ctlEditCode.GetWindowText(sztmp);
		m_ctlEditCode.SetWindowText("");
		const int	nCount = m_plistbox.GetCount();
		int	nval = 0;
		int	nFind = -1;
		CString	scod = _T("");

		for ( int ii = 0 ; ii < nCount ; ii++ )
		{
			nval = m_plistbox.GetItemData(ii);
			if (ii < m_nBorder)
			{
				scod = GetShortCode(m_arraySfCode.GetAt(nval).gcod);
				if (scod.CompareNoCase(sztmp) == 0)
				{
					nFind = ii;
					break;
				}
			}
			else if (ii > m_nBorder)
			{
				scod = GetShortCode(m_arrayPcode.GetAt(nval).gcod);
				if (scod.CompareNoCase(sztmp) == 0)
				{
					nFind = ii;
					break;
				}
			}
		}

		if (nFind >= 0)
		{
			m_plistbox.SetCurSel(nFind);			
			ClickList();
		}
	}
	
	if (wParam == IDC_EDIT_NAME)
	{
		m_ctlEditName.GetWindowText(sztmp);
		
		if (sztmp.IsEmpty())
			return 0;

		bool	bSame = false;
		if (m_szFindText.Compare(sztmp) == 0)
		{
			bSame = true;
			m_nSavePos = ++m_nSavePos % m_arFindIndex.GetSize();	// 돌면서 데이터를 찾기위하여
		}
		else
		{
			m_nSavePos = -1;
			m_szFindText = _T("");
			m_arFindIndex.RemoveAll();
		}

		const int nLen = sztmp.GetLength();
		int	nList = 0;
		const int nCount = m_plistbox.GetCount();
		CString	snam = _T("");
		int	nFind = -1;

		if (bSame)
		{
			nFind = m_arFindIndex.GetAt(m_nSavePos);

		}
		else
		{
			for ( int ii = 0 ; ii < nCount ; ii++ )
			{
				m_plistbox.GetText(ii, snam);
				nList = snam.GetLength();
			/*	if (nList == nLen)
				{
					if (snam.CompareNoCase(sztmp) == 0)
					{
						nFind = ii;
						m_szFindText = sztmp;
						m_nSavePos = 0;
						m_arFindIndex.RemoveAll();
						m_arFindIndex.Add(ii);
						break;
					}
				}
				else*/
				if (nList >= nLen)
				{
					snam = snam.Left(nLen);

					if (snam.CompareNoCase(sztmp) == 0)
					{
						if (nFind < 0)
						{
							m_szFindText = sztmp;
							m_nSavePos = 0;
						}

						nFind = ii;						
						m_arFindIndex.Add(nFind);
					}
				}
			}
			
			if (m_nSavePos >= 0)
				nFind = m_arFindIndex.GetAt(m_nSavePos);
		}

		if (nFind >= 0)
		{
			m_plistbox.SetCurSel(nFind);
			m_bFind = true;
			ClickList();
		}
	}
	return 0;
}

void CCustomFuturesDlg::OnOk() 
{
	
}


int CCustomFuturesDlg::CorrectType(int type)
{
	if (type & CC_OPTION)
	{
		if (!(type & CC_FUTURE))
			return CC_OPTION;
	}

	if (type & CC_KOPTION)
	{
		if (!(type & (CC_FUTURE | CC_KOSTAR | CC_SFUTURE)))
			return CC_KOPTION;
	}

	if (type & CC_SPREAD)
	{
		if (!(type & (CC_FUTURE | CC_KOSTAR | CC_SFUTURE)))
			return CC_SPREAD|CC_FUTURE;
	}
	
	if (type == 0 || !(type & CC_FULL))
	{
		type = CC_FULL;
	}

	return type ;
}

CString CCustomFuturesDlg::MakeTitle()
{
	CString	sztitle = _T("");
	if (m_type == CC_OPTION)
	{
		sztitle = "옵션 코드안내";
		return sztitle;
	}

	if (m_type == CC_KOPTION)
	{
		sztitle = "주식옵션 코드안내";
		return sztitle;
	}

	if (m_type & CC_FUTURE)
	{
		sztitle = "선물";

		if (m_type & CC_OPTION)
		{
			sztitle += "/옵션";
		}

		if (m_type & CC_KOSTAR)
		{
			sztitle += "/스타지수";
		}

		if (m_type & CC_SFUTURE)
		{
			sztitle += "/주식선물";
		}

		if (m_type & CC_KOPTION)
		{
			sztitle += "/주식옵션";
		}
	}
	else
	{
		if (m_type & CC_KOSTAR)
		{
			sztitle = "스타지수";

			if (m_type & CC_SFUTURE)
			{
				sztitle += "/주식선물";
			}

			if (m_type & CC_KOPTION)
			{
				sztitle += "/주식옵션";
			}
		}
		else
		{			
			if (m_type & CC_SFUTURE)
			{
				sztitle = "주식선물";

				if (m_type & CC_KOPTION)
				{
					sztitle += "/주식옵션";
				}
			}				
		}
	}

	if (!sztitle.IsEmpty())
	{
		sztitle += " 코드안내";
	}

	return sztitle;
}

void CCustomFuturesDlg::loadfile()
{
	
	if (m_type & (CC_FUTURE|CC_SPREAD|CC_KOSTAR))
		loadFjCode();
	
	if (m_type & CC_OPTION)
		loadOjCode();
	if (m_type & CC_SFUTURE)
		loadSfCode();
	if (m_type & CC_KOPTION)
		loadPjCode();	
}

void CCustomFuturesDlg::Select(WORD kind, CString code)
{
	switch (kind)
	{
	case CC_FUTURE:
		selectFUTURE(code);
		break;
	case CC_KOSTAR:
		selectKOSTA(code);
		break;
	case CC_SFUTURE:
		selectSFUTURE(code);
		break;
	case CC_KOPTION:
		selectKOPTION(code);
		break;
	case CC_OPTION:	
		if (m_type & CC_FUTURE)
			selectFUTURE(code);
		else
			selectOPTION(code);	
	}
}

void CCustomFuturesDlg::Select(CString code)
{
	int	kind = 0;
	if (!code.IsEmpty())
	{
		kind = findKind(code);		
	}	
	else
	{
		if (m_type & (CC_FUTURE|CC_KOSTAR|CC_SFUTURE))
			FutureMode();
		else
			OptionMode();		
	}

	if (!(kind & m_type))
		kind = findFirstKind();

	Select(kind, code);
}

void CCustomFuturesDlg::selectFUTURE(CString code)
{
	FutureMode();
	UnPushed();
	SetTitle("KOSPI200");
	m_ctlButtonKostar.SetSelected(FALSE);	
	m_ctlButtonKospi.SetSelected(TRUE);		
	m_ctlButtonKospi.SetFocus();
	SetFutureCodes(FALSE, code);
	m_ctlEditCode.EnableWindow(FALSE);
	m_ctlEditName.EnableWindow(FALSE);

	if (!(m_type & CC_OPTION))
	{
		DrawEmptyCheck();
		return;
	}

	setOption();	
}

void CCustomFuturesDlg::selectKOSTA(CString code)
{
	FutureMode();
	UnPushed();
	SetTitle("스타지수");
	m_ctlButtonKospi.SetSelected(FALSE);
	m_ctlButtonKostar.SetSelected(TRUE);
	m_ctlButtonKostar.SetFocus();
	SetFutureCodes(TRUE, code);
	m_nCountMonth = 4;
	m_ctlEditCode.EnableWindow(FALSE);
	m_ctlEditName.EnableWindow(FALSE);

	DrawEmptyCheck();
}


void CCustomFuturesDlg::selectSFUTURE(CString code)
{
	FutureMode();
	UnPushed();	

	int	nPos = 0, nItem = 0;
	CString	sCode = _T("");
	m_ctlEditCode.EnableWindow(TRUE);
	m_ctlEditName.EnableWindow(TRUE);	

	if (code.IsEmpty())
	{
		nPos = m_plistbox.GetCurSel();
		if (nPos < 0)
			nPos = 0;
	}
	else
	{
		code = code.Mid(1, 2);
		for ( int ii = 0 ; ii < m_nBorder ; ii++ )
		{
			nItem = m_plistbox.GetItemData(ii);
			if (code.CompareNoCase(m_arraySfCode.GetAt(nItem).tjgb) == 0)
			{
				nPos = ii;
				break;
			}
		}
	}

	CString	str = _T("");
	m_plistbox.SetCurSel(nPos);	
	m_plistbox.GetText(nPos, str);
	SetTitle(str);		
	nItem = m_plistbox.GetItemData(nPos);
	SetSFCodes(nItem, code);

	if (m_arraySfCode.GetSize() <= nItem)
	{
		if (m_bFind)
		{
			m_ctlEditName.SetSel(0, -1);
			m_ctlEditName.SetFocus();
		}
		else
			m_plistbox.SetFocus();
		m_bFind = false;
		return;
	}

	if (m_type & CC_KOPTION)
	{
		m_pCheck->SetInit(40);
		m_pCheck->SetScroll(0);
		m_pCheck->SetEmpty(FALSE);
		m_pCheck->EnableWindow(TRUE);
		m_pCheck->SetCheckCursor(IDC_CURSOR_HAND);
		CountData(m_arraySfCode.GetAt(nItem).tjgb, m_arraySfCode.GetAt(nItem).codx);			
	}
	else
		DrawEmptyCheck();

	if (m_bFind)
	{
		m_ctlEditName.SetSel(0, -1);
		m_ctlEditName.SetFocus();
	}
	else
		m_plistbox.SetFocus();
	m_bFind = false;
	
}

void CCustomFuturesDlg::selectKOPTION(CString code)
{
	UnPushed();
	m_plistbox.SetFocus();
	int	nPos = 0, nItem = 0;
	CString	sCode = _T(""), oldcode;
	oldcode.Format("%s", code);
	m_ctlEditCode.EnableWindow(TRUE);
	m_ctlEditName.EnableWindow(TRUE);

	if (code.IsEmpty())
	{
		nPos = m_plistbox.GetCurSel();
		if (nPos < 0)
			nPos = 0;
	}
	else
	{
		code = code.Mid(1, 2);
		const int ncnt = m_plistbox.GetCount();
		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{
			nItem = m_plistbox.GetItemData(ii);
			if (nItem >= 0)
			{
				CString	str1 = m_arrayPcode.GetAt(nItem).kind;
				if (code.CompareNoCase(str1) == 0)
				{	
					nPos = ii;
					break;
				}
			}
		}
	}

	if (nPos < m_nBorder && (m_type & CC_SFUTURE))
	{
		selectSFUTURE(oldcode);
		return;
	}
	else
		OptionMode();

	CString	str = _T("");
	m_plistbox.SetCurSel(nPos);	
	m_plistbox.GetText(nPos, str);
	SetTitle(str);		
	nItem = m_plistbox.GetItemData(nPos);
	
	m_pCheck->SetInit(40);
	m_pCheck->SetScroll(0);

	if (m_arrayPcode.GetSize() <= nItem)
	{
		if (m_bFind)
		{
			m_ctlEditName.SetSel(0, -1);
			m_ctlEditName.SetFocus();
		}
		else
			m_plistbox.SetFocus();
		m_bFind = false;
		return;
	}
		
	sCode = GetShortCode(m_arrayPcode.GetAt(nItem).gcod);	
	CountData(m_arrayPcode.GetAt(nItem).kind, m_arrayPcode.GetAt(nItem).code);
	m_ctlButtonName.ShowWindow(SW_SHOW);
	ButtonText(m_arrayPcode.GetAt(nItem).name, sCode);
	m_pCheck->SetEmpty(FALSE);
	m_pCheck->EnableWindow(TRUE);
	m_pCheck->SetCheckCursor(IDC_CURSOR_HAND);
	m_pCheck->ShowRow(m_nAtm);

	if (m_bFind)
	{
		m_ctlEditName.SetSel(0, -1);
		m_ctlEditName.SetFocus();
	}
	else
		m_plistbox.SetFocus();
	m_bFind = false;
	
}

void CCustomFuturesDlg::selectOPTION(CString code)
{
	OptionMode();
	UnPushed();
	m_ctlEditCode.EnableWindow(FALSE);
	m_ctlEditName.EnableWindow(FALSE);

	SetTitle("KOSPI200 지수옵션");
	// setOption();
	m_ctlButtonName.SetFocus();
	m_ctlButtonName.SetWindowText("KOSPI200 지수옵션");
	setOption();
}

WORD CCustomFuturesDlg::findKind(const CString& codx)
{
	WORD	ret = 0;
	if (codx.IsEmpty())
		return ret;
	
	const TCHAR fCH = codx.GetAt(0);
	const TCHAR sCH = codx.GetAt(1);
	const TCHAR oCH = codx.GetAt(2);

	switch (fCH)
	{
	case '1':
	case '4':
	case 'A': //파생상품 코드개편
	case 'D':
		if (sCH == '0')	// 지수선물
		{
			if (oCH == '1')
				ret = CC_FUTURE;
			else
				ret = CC_KOSTAR;
		}
		else		// 주식선물
			ret = CC_SFUTURE;
		break;
	case '2':
	case '3':
	case 'B':  //파생상품 코드개편
	case 'C':
		if (sCH == '0')	// 지수옵션
			ret = CC_OPTION;
		else		// 주식옵션
			ret = CC_KOPTION;		
		break;
	}

	return ret;
}

WORD CCustomFuturesDlg::findFirstKind()
{
	if (m_type == CC_OPTION)
		return CC_OPTION;

	if (m_type == CC_KOPTION)
		return CC_KOPTION;

	if (m_type & CC_FUTURE)
		return CC_FUTURE;
	if (m_type & CC_KOSTAR)
		return CC_KOSTAR;
	if (m_type & CC_SFUTURE)
		return CC_SFUTURE;	
	return 0;
}

void CCustomFuturesDlg::FutureMode()
{
	const UINT idFuture[4] = {IDC_BUTTON_MONTH3, IDC_BUTTON_MONTH6, IDC_BUTTON_MONTH9, IDC_BUTTON_MONTH12};
	const UINT idSpread[3] = {IDC_BUTTON_SPMONTH3, IDC_BUTTON_SPMONTH6, IDC_BUTTON_SPMONTH9};
	CRect	checkRC = m_rcCheck;
	const int	nCmd = SW_SHOW;

	for ( int ii = 0 ; ii < 4 ; ii++ )
	{
		((CButton*)GetDlgItem(idFuture[ii]))->ShowWindow(nCmd);	
	}
	
	if ((m_type & CC_SPREAD))
	{
		checkRC.top += 24;	
		for (int ii = 0 ; ii < 3 ; ii++ )
		{
			((CButton*)GetDlgItem(idSpread[ii]))->ShowWindow(nCmd);
		}
	}

	m_ctlButtonName.ShowWindow(SW_HIDE);
	m_pCheck->SetWindowPos(nullptr, checkRC.left, checkRC.top, checkRC.Width(), checkRC.Height(), SWP_NOZORDER);
}

void CCustomFuturesDlg::OptionMode()
{
	const UINT	idFuture[4] = {IDC_BUTTON_MONTH3, IDC_BUTTON_MONTH6, IDC_BUTTON_MONTH9, IDC_BUTTON_MONTH12};
	const UINT	idSpread[3] = {IDC_BUTTON_SPMONTH3, IDC_BUTTON_SPMONTH6, IDC_BUTTON_SPMONTH9};
	const CRect	checkRC = m_rcCheck;
	const int	nCmd = SW_HIDE;

	for ( int ii = 0 ; ii < 4 ; ii++ )
	{
		((CButton*)GetDlgItem(idFuture[ii]))->ShowWindow(nCmd);	
	}
	
	for (int ii = 0 ; ii < 3 ; ii++ )
	{
		((CButton*)GetDlgItem(idSpread[ii]))->ShowWindow(nCmd);
	}

	m_ctlButtonName.ShowWindow(SW_SHOW);
	m_pCheck->SetWindowPos(nullptr, checkRC.left, checkRC.top, checkRC.Width(), checkRC.Height(), SWP_NOZORDER);
}


void CCustomFuturesDlg::SetTitle(CString title)
{
	GetDlgItem(IDC_ST_NAME)->SetWindowText(title);
}

void CCustomFuturesDlg::OrderEnable()
{
	// m_plistbox주식선물과 주식옵션의 플래그가 없다면 필요가 없다.
	m_plistbox.EnableWindow(FALSE);

	// 선물/코스타가 없다면 사용불가
	if (!(m_type & CC_FUTURE))
		m_ctlButtonKospi.EnableWindow(FALSE);

	if (!(m_type & CC_KOSTAR))
		m_ctlButtonKostar.EnableWindow(FALSE);
	
	if (m_type & (CC_SFUTURE|CC_KOPTION))
		m_plistbox.EnableWindow(TRUE);
	else
	{
		m_ctlEditCode.EnableWindow(FALSE);
		m_ctlEditName.EnableWindow(FALSE);
	}
	
}

void CCustomFuturesDlg::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == 1000)
	{
		KillTimer(1000);
		Select(m_szCode);
	}
	CDialog::OnTimer(nIDEvent);
}

void CCustomFuturesDlg::OnButtonMonth3() 
{
	// TODO: Add your control notification handler code here
	CString	str;
	m_ctlButtonMonth3.GetWindowText(str);
	if (str.IsEmpty())
		return;

	if (str.GetAt(0) == 'S')
	{
		CString	code = _T(""), name = _T("");
		if (GetCurrentSFCODE(code, name, 0))
		{
			MonthInit();
			SetCode(code);
			SetName(name);
		}
	}
	else
	{		
		bool	bKospi = false;
		if (str.Find("스타") < 0)
			bKospi = true;
		FCODE fcode = m_arrayFcode.GetAt(0 + (!bKospi?7:0));

		MonthInit();
		SetCode(fcode.code);
		SetName(fcode.name);
	}

	CDialog::OnOK();
}

void CCustomFuturesDlg::OnButtonMonth6() 
{
	// TODO: Add your control notification handler code here
	CString	str;
	m_ctlButtonMonth6.GetWindowText(str);
	if (str.IsEmpty())
		return;

	if (str.GetAt(0) == 'S')
	{
		CString	code = _T(""), name = _T("");
		if (GetCurrentSFCODE(code, name, 1))
		{
			MonthInit();
			SetCode(code);
			SetName(name);
		}
	}
	else
	{
		bool	bKospi = false;
		if (str.Find("스타") < 0)
			bKospi = true;
		FCODE fcode = m_arrayFcode.GetAt(1 + (!bKospi?7:0));

		MonthInit();
		SetCode(fcode.code);
		SetName(fcode.name);
		UpdateData(FALSE);
	}

	CDialog::OnOK();
}

void CCustomFuturesDlg::OnButtonMonth9() 
{
	// TODO: Add your control notification handler code here
	CString	str;
	m_ctlButtonMonth9.GetWindowText(str);
	if (str.IsEmpty())
		return;

	if (str.GetAt(0) == 'S')
	{
		CString	code = _T(""), name = _T("");
		if (GetCurrentSFCODE(code, name, 2))
		{
			MonthInit();
			SetCode(code);
			SetName(name);
		}
	}
	else
	{
		bool	bKospi = false;
		if (str.Find("스타") < 0)
			bKospi = true;
		FCODE fcode = m_arrayFcode.GetAt(2 + (!bKospi?7:0));

		MonthInit();
		SetCode(fcode.code);
		SetName(fcode.name);
		UpdateData(FALSE);
	}

	CDialog::OnOK();
}

void CCustomFuturesDlg::OnButtonMonth12() 
{
	CString	str;
	m_ctlButtonMonth12.GetWindowText(str);
	if (str.IsEmpty())
		return;

	if (str.GetAt(0) == 'S')
	{
		CString	code = _T(""), name = _T("");
		if (GetCurrentSFCODE(code, name, 3))
		{
			MonthInit();
			SetCode(code);
			SetName(name);
		}
	}
	else
	{
		bool	bKospi = false;
		if (str.Find("스타") < 0)
			bKospi = true;

		FCODE fcode = m_arrayFcode.GetAt(3 + (!bKospi?7:0));

		MonthInit();
		SetCode(fcode.code);
		SetName(fcode.name);
		UpdateData(FALSE);
	}

	CDialog::OnOK();
}


void CCustomFuturesDlg::DrawEmptyCheck()
{
	m_pCheck->SetInit(40);
	m_pCheck->SetScroll(0);
	m_pCheck->RemoveAll();
	m_pCheck->SetCount(40);
	m_pCheck->SetEmpty(TRUE);
	m_pCheck->EnableWindow(FALSE);
	m_pCheck->SetCheckCursor(NULL);
	m_pCheck->Invalidate(FALSE);	
}

void CCustomFuturesDlg::setOption()
{
	_DATA data{};
	CString szTemp;
	int col = 0, row = 0;
	
	m_nCountMonth = m_ArrayMonth.GetSize();
// 	CString strTemp;
// 	strTemp.Format("\n\n[KSJ] CountMonth[%d]\n", m_nCountMonth);

	m_pCheck->SetInit(40);
	m_pCheck->SetScroll(0);	
	m_pCheck->RemoveAll();
	m_pCheck->SetCount(m_arrayOcode.GetSize());
	m_pCheck->SetEmpty(FALSE);
	m_pCheck->EnableWindow(TRUE);
	m_pCheck->SetCheckCursor(IDC_CURSOR_HAND);
	

	// CheckBox에 데이터 맞물리기...
	for (int ii = m_arrayOcode.GetSize() - 1; ii >= 0 ; ii--)
	{
		const ojcode OJCode = m_arrayOcode.GetAt(ii);
		
		if (OJCode.atmg == 1)	// ATM 구하기 ...
			m_nAtm = abs(ii - (m_arrayOcode.GetSize() - 1));

		int jj = 0;
		// call... 데이터 
		for (jj = 0; jj < GetCountMonth(); jj++)
		{
			if (OJCode.call[jj].yorn == '0')		
				data.flag = FALSE;
			else					
			{	
				data.flag = TRUE;
				data.code = OJCode.call[jj].cod2;
				szTemp = OJCode.call[jj].hnam;
				szTemp = szTemp.Left(18);
				szTemp.TrimRight();
				data.name = szTemp;
				
				if (OJCode.atmg == 1)
					data.atm = TRUE;
				else
					data.atm = FALSE;
			}
			m_pCheck->AddData(row, jj, data);	// 데이터 삽입..
			
			data.code = "";
			data.name = "";
			data.flag = FALSE;
		}
		
		szTemp = "";
		data.name = CString(OJCode.price, OPriceLen);

		float	val = (float)atol(data.name);
			val /= 100;
			data.name.Format("%3.1f", val);

		m_pCheck->AddData(row, jj, data);
		data.name = "";
		
		//put... 데이터 
		for (jj = 12; jj < 23; jj++)
		{
			col = jj - 12;
			if (OJCode.put[col].yorn == '0')		
			{	
				data.flag = FALSE;
				data.code = "";
				data.name = "";
			}
			else					
			{
				data.flag = TRUE;
				data.code = OJCode.put[col].cod2;
				szTemp = OJCode.put[col].hnam;
				szTemp = szTemp.Left(18);
				szTemp.TrimRight();
				data.name = szTemp;	
			}
			m_pCheck->AddData(row, jj, data);	// 데이터 삽입..

		}
	
		data.name = "";
		data.code = "";
		data.flag = FALSE;
		data.atm = FALSE;
		++row;
	}
	m_pCheck->ShowRow(m_nAtm);
}

void CCustomFuturesDlg::UnPushed()
{
	SetButton(&m_ctlButtonKospi);
	SetButton(&m_ctlButtonKostar);
}
