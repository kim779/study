// 123456Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "AxisCode.h"
#include "CommodityDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct month_code {
	char m[2];
	int month;
} month_code[] =
{
	{"1",1},
	{"2",2},
	{"3",3},
	{"4",4},
	{"5",5},
	{"6",6},
	{"7",7},
	{"8",8},
	{"9",9},
	{"A",10},
	{"B",11},
	{"C",12},
	{"0",-1}	
};

struct year_code {
	char y[2];
	int year;
} year_code[] = 
{
	{"0",2000},
	{"1",2001},
	{"2",2002},
	{"3",2003},
	{"4",2004},
	{"5",2005},
	{"A",2006},
	{"B",2007},
	{"C",2008},
	{"D",2009},
	{"E",2010},
	{"F",2011},
	{"G",2012},
	{"H",2013},
	{"J",2014},
	{"K",2015},
	{"L",2016},
	{"M",2017},
	{"N",2018},
	{"P",2019},
	{"Q",2020},
	{"R",2021},
	{"S",2022},
	{"T",2023},
	{"V",2024},
	{"W",2025},
	{"6",2026},
	{"7",2027},
	{"8",2028},
	{"9",2029},	
	{"",-1}
};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

/////////////////////////////////////////////////////////////////////////////
// CCommodityDlg dialog

CCommodityDlg::CCommodityDlg(CWnd* pParent /*=nullptr*/, CPoint pt )
	: CDialog(CCommodityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommodityDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pt = pt;
	m_bSpreadMode = FALSE;
}

void CCommodityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommodityDlg)
	DDX_Control(pDX, IDC_BTN_OPTION, m_btnOption);
	DDX_Control(pDX, IDC_LIST3, m_Olist);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_LIST2, m_spdList);
	DDX_Control(pDX, IDC_BTN_MONEY, m_btnMoney);
	DDX_Control(pDX, IDC_BTN_GUMRI, m_btnGumri);
	DDX_Control(pDX, IDC_BTN_COM, m_btnCommodity);
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCommodityDlg, CDialog)
	//{{AFX_MSG_MAP(CCommodityDlg)	
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MEASUREITEM()
	ON_BN_CLICKED(IDC_BTN_GUMRI, OnBtnGumri)
	ON_BN_CLICKED(IDC_BTN_MONEY, OnBtnMoney)
	ON_BN_CLICKED(IDC_BTN_COM, OnBtnCom)
	ON_WM_DESTROY()	
	ON_LBN_SELCHANGE(IDC_LIST2, OnSelchangeList2)
	ON_LBN_DBLCLK(IDC_LIST2, OnDblclkList2)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BTN_OPTION, OnBtnOption)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_BLCLICK,OnListBDClick)
	ON_MESSAGE(WM_CLICK,OnListClick)
	ON_MESSAGE(WM_SELECT_OPTION,OnSelectOption)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommodityDlg message handlers

void CCommodityDlg::SetDlgPos(int x, int y)
{
	if (x < 0 || y < 0)
		return;

	int	scWidth{}, scHeight{};
	scWidth	= GetSystemMetrics(SM_CXFULLSCREEN), scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

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

BOOL CCommodityDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_Olist.MakeImageList();

	m_btnCommodity.SetButtonStyle(BS_AUTORADIOBUTTON|BS_PUSHLIKE);
	m_btnGumri.SetButtonStyle(BS_AUTORADIOBUTTON|BS_PUSHLIKE);
	m_btnMoney.SetButtonStyle(BS_AUTORADIOBUTTON|BS_PUSHLIKE);
	m_btnOption.SetButtonStyle(BS_AUTORADIOBUTTON|BS_PUSHLIKE);

	LoadCCodeFromFile();

	m_btnGumri.SetCheck( BST_CHECKED );
	OnBtnGumri();

	m_bk.CreateSolidBrush( RGB(240,240,240) );

	m_1.CreateSolidBrush( RGB(225,225,225) );
	m_2.CreateSolidBrush( RGB(245,209,204) );
	m_3.CreateSolidBrush( RGB(200,222,243) );

	SetDlgPos( m_pt.x, m_pt.y );		

	m_btnOption.ShowWindow(SW_HIDE);

	if( m_bSpreadMode == TRUE )
	{
		SetWindowText("상품종목안내-스프레드");
		m_btnOption.EnableWindow( FALSE );
	}
	else
		SetWindowText("상품종목안내");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCommodityDlg::SetSpreadMode( BOOL b )
{
	m_bSpreadMode = b;
}

BOOL CCommodityDlg::FindOCode( CString strCode )
{
	POSITION pos = m_cocodeList.GetHeadPosition();
	while( pos )
	{
		const shared_ptr<cocode_in> p = m_cocodeList.GetNext( pos );
		if( p->code == strCode )
		{
			m_strName = p->hname;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CCommodityDlg::FindCode( CString str )
{
	str.TrimRight();
	for( int i = 0; i < m_codeGroup.GetSize(); i++ )
	{
		std::shared_ptr<ccode_group> pg = m_codeGroup.GetAt(i);

		POSITION pos = pg->list.GetHeadPosition();
		while( pos )
		{
			const std::shared_ptr<ccode_in> pin = pg->list.GetNext( pos );
			if( pin->code == str )
			{
				m_strName = pin->hname;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CCommodityDlg::LoadCOCodeFromFile( void )
{
	struct code15 code15 {};

	CString strPath;
	strPath = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\cocode.dat";

	CFile file;
	if (!file.Open(strPath, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{
	//	MessageBox("상품옵션 파일이 존재하지 않습니다.");
		return FALSE;
	}

	const int iCount = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct code15));

	for (int i = 0; i < iCount; i++)
	{
		file.Read(&code15, sizeof(struct code15));

		if( CString(code15.codx) == "99999999" )
			break; 
		std::shared_ptr<cocode_in> pin = std::make_shared<cocode_in>();
		m_cocodeList.AddTail(pin);

		CString year, month;

		year = CString(code15.mont).Mid(0,2);
		month = CString(code15.mont).Mid(2,2);		

		pin->code = CString(code15.codx,8);
		pin->year = 2000+atoi(year);
		pin->month = atoi(month);


		CString strName;
		strName = CString( code15.hnam,50 );
		strName.TrimRight();
		pin->hname = strName;

		strName = CString( code15.enam,50 );
		strName.TrimRight();
		pin->ename = strName;

		if( pin->code.GetAt(0) == '2' || pin->code.GetAt(0) == 'B')  //파생상품 코드개편
			pin->bCall = TRUE;
		else
			pin->bCall = FALSE;

		const int price = atoi( code15.hsga );
		pin->price = gsl::narrow_cast<float>(price);
		
	}

	file.Close();

	return TRUE;
}

BOOL CCommodityDlg::LoadCCodeFromFile()
{
	struct ccode ccode {};
	std::shared_ptr<ccode_in> pin{};
	CString stmp;

	CString strPath;

	strPath = ((CAxisCodeApp*)AfxGetApp())->m_root + "\\tab\\cfcode.dat";

	CFile file;
	if (!file.Open(strPath, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{
		MessageBox("파일[cfcode.dat]이 존재하지 않습니다.");
		return FALSE;
	}
								// 데이터의 갯수...
	const int iCount = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct pjcode));

	for (int i = 0; i < iCount; i++)
	{
		file.Read(&ccode, sizeof(struct ccode));
		
		if( CString(ccode.codx) == "99999999" )
			break; 

		pin = std::make_shared<ccode_in>();
		pin->code = CString(ccode.codx,8);

		if (pin->code.Find("A75") >= 0)
			TRACE("eset");

		int mode = 0;  //파생상품 코드개편 
		if (pin->code.Left(1) == "A")
		{
			stmp.Format("1%s", pin->code.Mid(1, 2));
			mode = atoi(stmp);
		}
		else if (pin->code.Left(1) == "D")
		{
			stmp.Format("4%s", pin->code.Mid(1, 2));
			mode = atoi(stmp);
		}
		else
		    mode = atoi(pin->code.Left(3));
		
		pin->year = GetYear( pin->code.Mid(3,1) );
		pin->month = GetMonth( pin->code.Mid(4,1) );
		pin->mode = mode;

		pin->ename = CString(ccode.enam,50);
		pin->hname = CString(ccode.hnam,50);

		if( (pin->year < 0 || pin->month < 0) ||
			(mode > 500 || mode <= 0)
			)
		{
			//delete pin;
			pin.reset();
			continue;
		}
		
		AddGroup( pin );
	}

	return TRUE;
}

void CCommodityDlg::AddGroup(std::shared_ptr<ccode_in> pin )
{
	std::shared_ptr<ccode_group> pg{};

	for( int i = 0; i < m_codeGroup.GetSize(); i++ )
	{
		pg = m_codeGroup.GetAt(i);
		if( pg->mode == pin->mode )
		{
			AddMonth( pin, pg );
			return;
		}
	}

	pg = std::make_shared<ccode_group>();
	pg->mode = pin->mode;
	m_codeGroup.Add( pg );
	AddMonth( pin, pg );	
}

void CCommodityDlg::AddMonth(std::shared_ptr<ccode_in> pin, std::weak_ptr<ccode_group> pg )
{
	POSITION pos = pg.lock()->list.GetHeadPosition();
	while( pos )
	{
		const std::shared_ptr<ccode_in> p = pg.lock()->list.GetNext( pos );
		if( p->year == pin->year )
		{
			if( p->month > pin->month )
			{
				pg.lock()->list.InsertBefore(pos, pin);
				return;
			}
		}		
	}
	
	pg.lock()->list.AddTail( pin );	
}

int CCommodityDlg::GetYear(CString str )
{
	for( int i=0; ; i++ )
	{
		if( year_code[i].year < 0 )
			break;

		if( year_code[i].y == str )
			return year_code[i].year;
	}

	return -1;
}

int CCommodityDlg::GetMonth(CString str )
{
	for( int i=0;;i++ )
	{
		if( month_code[i].month < 0 )
			break;
		if( month_code[i].m == str )
			return month_code[i].month;
	}
	return -1;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCommodityDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		const int cxIcon = GetSystemMetrics(SM_CXICON);
		const int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		const int x = (rect.Width() - cxIcon + 1) / 2;
		const int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rc;
		GetClientRect( &rc );

		CPaintDC dc(this);
		dc.FillRect( &rc, &CBrush(RGB(240,240,240)) );

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCommodityDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCommodityDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	if( nIDCtl == IDC_LIST1 )
	{
		lpMeasureItemStruct->itemHeight = 20;
	}
	
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CCommodityDlg::AddList(std::weak_ptr<ccode_group> pg )
{
	const int iCount = m_list.GetItemCount();
	int iSub = 1;

// 	m_list.m_mapYear.RemoveAll();	//2016.06.22 KSJ 맵지워줌

	m_list.InsertItem( iCount, "" );
 
	std::shared_ptr<ccode_in> pin = pg.lock()->list.GetHead();
	if (pin == nullptr || pg.lock()->list.GetCount() == 0)
	{
		return;
	}

//	CString strHead = " " + pin->hname.Left( pin->hname.Find('F',0) ) ;  //20191129 상품선물 스프레드추가
	CString strHead = " " + pin->hname.Left( pin->hname.Find('F',0) > 0 ? pin->hname.Find('F',0) : pin->hname.Find("SP",0) ) ;
	m_list.SetItem( iCount, 0, LVIF_TEXT, strHead, 0, 0, 0, 0 );
	m_list.SetItemData( iCount, (DWORD)pg.lock().get());

	POSITION pos = pg.lock()->list.GetHeadPosition();
	while( pos )
	{
		pin = pg.lock()->list.GetNext( pos );

		CString strMonth, strYear, strKey;
		strMonth.Format("%d월",pin->month);
		strYear.Format("%d년 ", pin->year);
		strKey.Format("%d%d", iCount, iSub);

		m_list.m_mapYear.SetAt(strKey, strYear + strMonth);	//2016.06.22 KSJ 맵에 저장

		m_list.SetItem( iCount, iSub, LVIF_TEXT, strMonth, 0, 0, 0, 0);

		iSub++;
	}
}

void CCommodityDlg::DeleteSpread( void )
{
	m_spdList.ResetContent();

	m_edit1.SetWindowText("");
	m_edit2.SetWindowText("");
}

void CCommodityDlg::DeleteAll( void )
{
	m_list.DeleteAllItems();
	m_Olist.DeleteAllItems();

	DeleteSpread();
	

	int iCount = m_list.GetHeaderCtrl()->GetItemCount();
	for( int i = iCount-1; i >= 0; i-- )
	{
		m_list.DeleteColumn(i);
	}

	iCount = m_Olist.GetHeaderCtrl()->GetItemCount();
	for(int i = iCount-1; i >=0; i-- )
	{
		m_Olist.DeleteColumn(i);
	}
}

void CCommodityDlg::SetOption( CString strCode )
{
	CList<int,int> arrCol;
	CList<float,float> arrRow;
	CList<float,float> arrTemp;		

	CArray<cocode_in, cocode_in> arrlist;

	// 가격 추려내기 
	POSITION pos = m_cocodeList.GetHeadPosition();
	while( pos )
	{
		BOOL bFind = FALSE;
		const shared_ptr<cocode_in> p = m_cocodeList.GetNext( pos );
		POSITION pos2 = arrTemp.GetHeadPosition();
		while( pos2 )
		{
			const float f = arrTemp.GetNext( pos2 );
			if( p->price == f )
			{
				bFind = TRUE;
				break;
			}
		}

		if( bFind == FALSE )
			arrTemp.AddTail( p->price );
	}

	// 가격 sort
	pos = arrTemp.GetHeadPosition();
	while( pos ) 
	{
		const float f = arrTemp.GetNext(pos);
		POSITION pos2 = arrRow.GetHeadPosition();
		BOOL bFind = FALSE;
		while( pos2 )
		{
			POSITION posOld = pos2;
			const float tf = arrRow.GetNext(pos2);
			if( tf > f )
			{
				arrRow.InsertBefore( posOld, f );
				bFind = TRUE;
				break;
			}
		}

		if( bFind == FALSE )
			arrRow.AddTail( f );
	}	

	// 년월 추려내기 
	pos = m_cocodeList.GetHeadPosition();
	while( pos )
	{
		BOOL bFind = FALSE;
		const shared_ptr<cocode_in> p = m_cocodeList.GetNext( pos );
		POSITION pos2 = arrCol.GetHeadPosition();
		while( pos2 )
		{
			const int i = arrCol.GetNext( pos2 );
			if( p->year*100+p->month == i )
			{
				bFind = TRUE;
				break;
			}
		}

		if( bFind == FALSE )
			arrCol.AddTail( p->year*100+p->month );
	}

	// Column 생성 Call
	int idx = 0;
	pos = arrCol.GetTailPosition();
	while( pos )
	{
		const int i = arrCol.GetPrev( pos );

		CString str;
		str.Format("%d",i%100);

		m_Olist.InsertColumn(idx,str,HEADER_CALL,LVCFMT_CENTER,28);
		idx++;	

		cocode_in info;
		info.month = i%100;
		info.year = i/100;
		info.bCall = TRUE;
		arrlist.Add( info );
	}	

	m_Olist.InsertColumn(idx,"행사가격",HEADER_NORMAL,LVCFMT_CENTER,60);
	idx++;

	// Column 생성 put
	pos = arrCol.GetHeadPosition();
	while( pos )
	{
		const int i = arrCol.GetNext( pos );

		CString str;
		str.Format("%d",i%100);

		m_Olist.InsertColumn(idx,str,HEADER_PUT,LVCFMT_CENTER,28);
		idx++;

		cocode_in info;
		info.month = i%100;
		info.year = i/100;
		info.bCall = FALSE;
		arrlist.Add( info );
	}

	const int iCount = arrlist.GetSize();
	const int iRow = arrRow.GetCount();
	for( int i = 1; i < iRow; i++ )
	{
		for( int j = 0; j < iCount; j++ )
		{
			cocode_in info = arrlist.GetAt( j );
			arrlist.Add( info );
		}		
	}

	// row 가격 추가 
	int	iStart = 0;
	idx = 0;
	pos = arrRow.GetHeadPosition();
	while( pos )
	{
		const float a = arrRow.GetNext( pos );

		CString str;
		str.Format("%.0f",a);

		m_Olist.InsertItem(idx,"",0);			

		m_Olist.SetItemText( idx, iCount/2, str);
		int j = 0;
		for(j = iStart; j < iCount+iStart; j++ )
		{
			cocode_in info = arrlist.GetAt( j%iCount );
			info.price = a;
			arrlist.SetAt( j, info );
		}

		iStart = j;
		idx++;	
	}	

	// code 셋팅 
	pos = m_cocodeList.GetHeadPosition();
	while( pos )
	{
		const shared_ptr<cocode_in> p = m_cocodeList.GetNext( pos );
		for( int i = 0; i < arrlist.GetSize(); i++ )
		{
			cocode_in info = arrlist.GetAt( i );
			if( info.month == p->month &&
				info.year == p->year &&
				info.bCall == p->bCall &&
				info.price == p->price )
			{
				int col = i % iCount;
				const int row = i / iCount;

				if( col > iCount/2-1 )
					col++;

				m_Olist.SetItemText(row,col,p->code);				
			}
		}
	}
}

void CCommodityDlg::SetList(int iMin, int iMax, int igubn)
{
	//20191129 상품선물 스프레드추가
	CString stmp;
	std::shared_ptr<ccode_group> SPgp;

	std::shared_ptr<ccode_group>gp_65, gp_66, gp_67, gp_70{};
	std::shared_ptr<ccode_group>gp_75, gp_76, gp_77, gp_78;

	int iColCount = 0;

	CArray<std::weak_ptr<ccode_group>, std::weak_ptr<ccode_group>> list;
	CArray<std::weak_ptr<ccode_group>, std::weak_ptr<ccode_group>> tmplist;
	//3년 65, 5년 66, 10년 67, 3년-10년 68, 30년 70
	//미국달러 75, 앤 76, 유로 77, 위안 78
	for( int i = 0; i < m_codeGroup.GetSize(); i++ )
	{
		std::shared_ptr<ccode_group> gp = m_codeGroup.GetAt(i);

		//20191129 상품선물 스프레드추가
		stmp.Format("%d", gp->mode);
		stmp = stmp.Right(2);

		if( gp->mode >= iMin && gp->mode <= iMax )
		{
			if( gp->list.GetCount() > iColCount )
				iColCount = gp->list.GetCount();

			//list.Add( gp );  //test
			if (igubn == GUMRI)
			{
				if (stmp == "65") gp_65 = gp;
				if (stmp == "66") gp_66 = gp;
				if (stmp == "67") gp_67 = gp;
				if (stmp == "70") gp_70 = gp;
			}
			else
			{
				if (stmp == "75") gp_75 = gp;
				if (stmp == "76") gp_76 = gp;
				if (stmp == "77") gp_77 = gp;
				if (stmp == "78") gp_78 = gp;
			}
		
		}

		if(stmp == "68")
		{
			if( gp->list.GetCount() > iColCount )
				iColCount = gp->list.GetCount();
			
			SPgp = gp;
		}
	}

	if (igubn == GUMRI)
	{
		list.Add(gp_65);
		list.Add(gp_66);
		list.Add(gp_67);
	}
	else
	{
		list.Add(gp_75);
		list.Add(gp_76);
		list.Add(gp_77);
		list.Add(gp_78);
	}

	if (m_btnGumri.GetCheck() && SPgp != nullptr)
	{
		if(gp_70 != nullptr)
			list.Add(gp_70);
		list.Add(SPgp);
	}

	m_list.InsertColumn(0,"이름", LVCFMT_LEFT, 100 );
	
	for(int j = 1; j <= iColCount; j++ )
	{
		m_list.InsertColumn(j,"", LVCFMT_CENTER, 30);
	}

	for(int k = 0; k < list.GetSize(); k++ )
	{
		std::weak_ptr<ccode_group> pg = list.GetAt(k);
		AddList( pg );
	}
}

void CCommodityDlg::OnBtnGumri() 
{
	// TODO: Add your control notification handler code here

	DeleteAll();	
	ShowOption( FALSE );
	
	SetList( 160, 174, GUMRI);
}

void CCommodityDlg::OnBtnMoney() 
{
	// TODO: Add your control notification handler code here
	DeleteAll();	
	ShowOption( FALSE );
	
	SetList( 175, 184, MONEY);
	
}

void CCommodityDlg::OnBtnCom() 
{
	// TODO: Add your control notification handler code here
	DeleteAll();	
	ShowOption( FALSE );
	
	SetList( 185, 199 );
	
}

void CCommodityDlg::DeleteCCode() // 코드 메모리 정리 
{
	//for( int i = 0; i < m_codeGroup.GetSize(); i++ )
	//{
	//	ccode_group* pg = m_codeGroup.GetAt( i );

	//	POSITION pos = pg->list.GetHeadPosition();
	//	while( pos )
	//	{
	//		std::shared_ptr<ccode_in> pin = pg->list.GetNext( pos );
	//		delete pin;
	//	}

	//	delete pg;
	//}

	m_codeGroup.RemoveAll();
}

void CCommodityDlg::DeleteCOCode()
{
	//POSITION pos = m_cocodeList.GetHeadPosition();
	//while( pos )
	//{
	//	cocode_in* p = m_cocodeList.GetNext( pos );
	//	delete p;
	//}	
	m_cocodeList.RemoveAll();
}

void CCommodityDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here

	DeleteCCode();
	DeleteCOCode();	
}

BOOL CCommodityDlg::CheckSelect(CString& code, CString& name ) // 어떤 선물이 클릭 되었는지 
{
	const CPoint pt = m_list.GetSelect();

	if( pt.x == 0 ) 
		return FALSE;

	ccode_group* pg = (ccode_group*)m_list.GetItemData( pt.y );
	if( pg )
	{
		if( pg->list.GetCount() > pt.x-1 && pt.x != 0 )
		{
			int index = 0;
			POSITION pos = pg->list.GetHeadPosition();
			while( pos )
			{ 
				std::shared_ptr<ccode_in> pin = pg->list.GetNext( pos );
				if( index == pt.x-1 )
				{
					code = pin->code;					
					name = pin->hname;

					return TRUE;
				}
				index++;
			}			
		}
	}

	return FALSE;
}

std::weak_ptr<ccode_in> CCommodityDlg::GetSelect( void )
{
	const CPoint pt = m_list.GetSelect();

	if( pt.x == 0 ) 
		return weak_ptr<ccode_in>();

	ccode_group* pg = (ccode_group*)m_list.GetItemData( pt.y );
	if( pg )
	{
		if( pg->list.GetCount() > pt.x-1 && pt.x != 0 )
		{
			int index = 0;
			POSITION pos = pg->list.GetHeadPosition();
			while( pos )
			{ 
				auto pin = pg->list.GetNext( pos );
				if( index == pt.x-1 )
				{
					return pin;
				}
				index++;
			}			
		}
	}

	return weak_ptr<ccode_in>();
}

void CCommodityDlg::SetSpread(std::weak_ptr<ccode_in> pin )
{
	DeleteSpread();
	int irowmax = m_list.GetItemCount();  //irowmax가 5-> 30년 추가, 4-> 30년 없음
	for( int i = 0; i < m_codeGroup.GetSize(); i++ )
	{
		std::shared_ptr<ccode_group> gp = m_codeGroup.GetAt( i );
		if( gp->mode < 400 )
			continue;

		const CPoint pt = m_list.GetSelect();
		if(gp->mode == 468 && (pt.y == irowmax - 1) && m_btnGumri.GetCheck()) //금리이며 3-10년국채 선택시
		{
			POSITION pos = gp->list.GetHeadPosition();
			if(pos)
			{
				std::shared_ptr<ccode_in> pin = gp->list.GetNext(pos);
				if(pt.x == 2)
					pin = gp->list.GetNext(pos);

				const int idx = m_spdList.AddString( pin->code );
				m_spdList.SetItemData( idx, (DWORD)pin.get() );
				return;
			}
		}
		else if (gp->mode == 470 && (pt.y == irowmax - 2) && m_btnGumri.GetCheck()) //금리이며 3-30년국채 선택시
		{
			POSITION pos = gp->list.GetHeadPosition();
			if (pos)
			{
				std::shared_ptr<ccode_in> pin = gp->list.GetNext(pos);

				POSITION pos = gp->list.GetHeadPosition();
				while (pos)
				{
					const std::shared_ptr<ccode_in> pin = gp->list.GetNext(pos);
					const int idx = m_spdList.AddString(pin->code);
					OutputDebugString("\r\n" + pin->code);
					m_spdList.SetItemData(idx, (DWORD)pin.get());
				}
				return;
			}
		}
		else if( pin.lock()->mode-100 != gp->mode-400 )
			continue;

		POSITION pos = gp->list.GetHeadPosition();
		while( pos )
		{
			const std::shared_ptr<ccode_in> pin = gp->list.GetNext(pos);
			const int idx = m_spdList.AddString( pin->code );
			OutputDebugString("\r\n" + pin->code);
			m_spdList.SetItemData( idx, (DWORD)pin.get() );
		}
	}
}

LRESULT CCommodityDlg::OnListBDClick( WPARAM wParam, LPARAM lParam ) // code 더블클릭
{
	if( m_bSpreadMode )
		return 0;

	CString code, name;
	if( CheckSelect( code, name ) )
	{
		m_strCode = code;
		m_strName = name;

		OnOK();
	}
	else
		DeleteSpread();
	
	return 0;
}

LRESULT CCommodityDlg::OnListClick(WPARAM wParam, LPARAM lParam ) // code 클릭 
{
	std::weak_ptr<ccode_in> pin = GetSelect();
	if( pin.lock() )
	{
		SetSpread( pin );
	}
	else
		DeleteSpread();		
	
	return 0;
}

LRESULT CCommodityDlg::OnSelectOption( WPARAM wParam, LPARAM lParam )
{
	CString strCode = m_Olist.GetItemText( wParam, lParam );

	m_strCode = strCode;
	m_strName = GetOptionName(strCode);	
	OnOK();

	return 0;
}

CString CCommodityDlg::GetOptionName( CString strCode )
{
	POSITION pos = m_cocodeList.GetHeadPosition();
	while( pos )
	{
		const shared_ptr<cocode_in> p = m_cocodeList.GetNext( pos );
		if( p->code == strCode )
			return p->hname;
	}

	return "";
}


void CCommodityDlg::OnSelchangeList2() // 스프레드 선택 변경 
{
	// TODO: Add your control notification handler code here
	const int nIndex = m_spdList.GetCurSel();
	if( nIndex != LB_ERR )
	{
		const ccode_in* pin = (ccode_in*)m_spdList.GetItemData( nIndex );
		const CPoint pt = m_list.GetSelect();
		int irowmax = m_list.GetItemCount();
	
		if((pt.y == irowmax - 1) && m_btnGumri.GetCheck())
		{//마지막줄 선택 3-10년
			CString str1, str2;

			CString sname;
			sname = pin->ename;
			sname.TrimRight();
			sname = sname.Right(6);

			str1 = pin->code.Mid(3,2);
			str2 = pin->code.Mid(5,2);  
			
			CString str3, str4;
			if (atoi(sname.Mid(2, 2)) >= 26)
				str3.Format("A%02s%s000", "65", str1);   //65 3년
			else
				str3.Format("1%02s%s000","65",str1);   //65 3년

			if (atoi(sname.Mid(2, 2)) >= 26)
				str4.Format("A%02s%s000","67",str1);   //67 10년
			else
				str4.Format("1%02s%s000", "67", str1);   //67 10년
			
			m_edit1.SetWindowText( str4 ); 
			m_edit2.SetWindowText( str3 ); 
			
			SwapControl( 1 );			
		}
		else if ((pt.y == irowmax - 2) && (irowmax == 5) && m_btnGumri.GetCheck())
		{//30년만을 위함이다 현재 3, 5,10, 3-10 에서   3, 5,10,30, 3-10  이렇게 5개로 늘었을때만 -> 30년 스프레드 처리해주는 부분
			CString str1, str2;

			CString sname;
			sname = pin->ename;
			sname.TrimRight();
			sname = sname.Right(6);

			str1 = pin->code.Mid(3, 2);
			str2 = pin->code.Mid(5, 2);
			int val = atoi(sname.Mid(2, 2));
			CString str3, str4;
			if (atoi(sname.Mid(2, 2)) >= 26)
				str3.Format("A%02s%s000", "70", str1);   
			else
				str3.Format("1%02s%s000", "70", str1);  

			if (atoi(sname.Mid(2, 2)) >= 26)
				str4.Format("A%02s%s000", "70", str2);   
			else
				str4.Format("1%02s%s000", "70", str2);  

			m_edit1.SetWindowText(str4);
			m_edit2.SetWindowText(str3);

			SwapControl(1);
		}
		else if( pin )
		{	
			CString str1, str2;

			str1 = pin->code.Mid(3,2);
			str2 = pin->code.Mid(5,2);  

			CString str3, str4;
			CString sname;
			sname = pin->ename;
			sname.TrimRight();
			sname = sname.Right(9);

			int ival = atoi(sname.Mid(0, 2));
			ival = atoi(sname.Mid(5, 2));

			if(atoi(sname.Mid(0, 2)) >= 26)
				str3.Format("A%02d%s000", pin->mode - 400, str1);
			else
				str3.Format("1%02d%s000", pin->mode - 400, str1);

			if (atoi(sname.Mid(5, 2)) >= 26)
				str4.Format("A%02d%s000",pin->mode - 400, str2);
			else
				str4.Format("1%02d%s000", pin->mode - 400, str2);

			m_edit1.SetWindowText( str4 ); // 원 월물 
			m_edit2.SetWindowText( str3 ); // 근 월물 

			const int type = pin->mode-400;
			if( type > 70 ) // 통화, 상품은 반대 			
				SwapControl( 2 );			
			else			
				SwapControl( 1 );			
		}	
	}	
}

void CCommodityDlg::ShowOption(BOOL bShow )
{
	if( bShow )
	{
		m_list.ShowWindow( SW_HIDE );
		m_Olist.ShowWindow( SW_SHOW );
	}
	else
	{
		m_list.ShowWindow( SW_SHOW );
		m_Olist.ShowWindow( SW_HIDE );
	}	
}

void CCommodityDlg::SwapControl( int b )
{
	CRect rc1, rc2, rc3, rc4;

	GetDlgItem( IDC_STATIC1 )->GetWindowRect( &rc1 );
	GetDlgItem( IDC_STATIC2 )->GetWindowRect( &rc2 );
	GetDlgItem( IDC_STATIC3 )->GetWindowRect( &rc3 );
	GetDlgItem( IDC_STATIC4 )->GetWindowRect( &rc4 );

	ScreenToClient( &rc1 );
	ScreenToClient( &rc2 );
	ScreenToClient( &rc3 );
	ScreenToClient( &rc4 );

	int c{};

	if( rc1.top > rc3.top ) // 통화 선물 
		c = 1;
	else // 금리 
		c = 2; 

	if( b == c )
		return ;

	if( b == 1 ) // 금리 
	{
		GetDlgItem( IDC_STATIC1 )->SetWindowPos( nullptr, rc3.left,rc3.top,rc3.Width(),rc3.Height(),
			SWP_NOZORDER|SWP_NOSIZE );
		GetDlgItem( IDC_STATIC2 )->SetWindowPos( nullptr, rc4.left,rc4.top,rc4.Width(),rc4.Height(),
			SWP_NOZORDER|SWP_NOSIZE );
		GetDlgItem( IDC_STATIC3 )->SetWindowPos( nullptr, rc1.left,rc1.top,rc1.Width(),rc1.Height(),
			SWP_NOZORDER|SWP_NOSIZE );
		GetDlgItem( IDC_STATIC4 )->SetWindowPos( nullptr, rc2.left,rc2.top,rc2.Width(),rc2.Height(),
			SWP_NOZORDER|SWP_NOSIZE );
	}
	else // 통화 
	{
		GetDlgItem( IDC_STATIC1 )->SetWindowPos( nullptr, rc3.left,rc3.top,rc3.Width(),rc3.Height(),
			SWP_NOZORDER|SWP_NOSIZE );
		GetDlgItem( IDC_STATIC2 )->SetWindowPos( nullptr, rc4.left,rc4.top,rc4.Width(),rc4.Height(),
			SWP_NOZORDER|SWP_NOSIZE );
		GetDlgItem( IDC_STATIC3 )->SetWindowPos( nullptr, rc1.left,rc1.top,rc1.Width(),rc1.Height(),
			SWP_NOZORDER|SWP_NOSIZE );
		GetDlgItem( IDC_STATIC4 )->SetWindowPos( nullptr, rc2.left,rc2.top,rc2.Width(),rc2.Height(),
			SWP_NOZORDER|SWP_NOSIZE );
	}
}

void CCommodityDlg::OnDblclkList2() // 스프레드 더블클릭
{
	// TODO: Add your control notification handler code here
	const int nIndex = m_spdList.GetCurSel();
	if( nIndex != LB_ERR )
	{		
		const ccode_in* pin = (ccode_in*)m_spdList.GetItemData( nIndex );
		if( pin )
		{
			m_strCode = pin->code;
			m_strName = pin->hname;

			OnOK();				 
		}	
	}
}

HBRUSH CCommodityDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	const HBRUSH__* hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetBkColor( RGB(240,240,240) );

	// TODO: Change any attributes of the DC here
	if( pWnd->GetDlgCtrlID() == IDC_STATIC1 )	
		pDC->SetTextColor( RGB(255,0,0) );
	else if( pWnd->GetDlgCtrlID() == IDC_STATIC2 )
		pDC->SetTextColor( RGB(0,0,255) );
	else if( pWnd->GetDlgCtrlID() == IDC_STATIC3 )
		pDC->SetTextColor( RGB(0,0,255) );
	else if( pWnd->GetDlgCtrlID() == IDC_STATIC4 )
		pDC->SetTextColor( RGB(255,0,0) );

	const int ctlId = pWnd->GetDlgCtrlID();
	
	if( ctlId == IDC_STATIC_1 || ctlId == IDC_EDIT1 || ctlId == IDC_EDIT2 )
	{
		pDC->SetBkColor(RGB(225,225,225));
		return m_1;
	}
	else if( ctlId == IDC_STATIC_2 || ctlId == IDC_STATIC1 || ctlId == IDC_STATIC3 )
	{
		pDC->SetBkColor(RGB(245,209,204));
		return m_2;
	}
	else if( ctlId == IDC_STATIC_3 || ctlId == IDC_STATIC2 || ctlId == IDC_STATIC4 )
	{
		pDC->SetBkColor(RGB(200,222,243));
		return m_3;
	}
	else
		return m_bk;
}

void CCommodityDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here

	const int nIndex = m_spdList.GetCurSel();
	if( nIndex != LB_ERR )		OnDblclkList2();	//2015.12.14 KSJ 확인버튼 눌렀을때 스프레드가 선택되어 있으면 스프레드로 바꿔준다.
	else						OnListBDClick(0,0);
}

void CCommodityDlg::OnBtnOption() 
{
	// TODO: Add your control notification handler code here
	DeleteAll();
	ShowOption( TRUE );

	// Load

	SetOption("");	
}
