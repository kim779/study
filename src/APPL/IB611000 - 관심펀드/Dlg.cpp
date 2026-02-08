// Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB611000.h"
#include "Dlg.h"
#include "NewGroup.h"
#include "EditGroup.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	WM_EDITGROUP (WM_USER + 200)
/////////////////////////////////////////////////////////////////////////////
// CDlg dialog
#define	saveFILE	"FDportfolio.ini"
#define	tempFILE	"FDportfolio.ini.tmp"

#define	HANGULMSK	0x80
#define	ChosungS	0xA4A1	// ㄱ
#define	ChosungE	0xA4BE	// ㅎ


BOOL	m_listsort;

int CALLBACK ListViewCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)			// 리스트 소팅
{ 
	_listitem *pItem1 = (_listitem *) lParam1; 
	_listitem *pItem2 = (_listitem *) lParam2; 
	
	int	compare=0;
	CString code1, code2;
	CString	name1, name2;
	CString rt06_1, rt06_2;
	CString rate1, rate2;
	CString idtp1, idtp2;
	CString sdat1, sdat2;
	CString rknm1, rknm2;
	CString shgb1, shgb2;	 
	
	double _rt06_1{}, _rt06_2{};
	double _rate1{}, _rate2{};


	switch ((int)lParamSort)
	{
	case 0:
		compare = lstrcmpi((char*)pItem1->code, (char*)pItem2->code); 
		break;
	case 1:
		name1 = CString((char*)pItem1->name); 
		name2 = CString((char*)pItem2->name); 
		compare = lstrcmpi(name1, name2);
		break;
	case 2:
		rt06_1 = CString((char*)pItem1->rt06); 
		rt06_2 = CString((char*)pItem2->rt06);

		_rt06_1 = atof(rt06_1);
		_rt06_2 = atof(rt06_2);

		if(_rt06_1 > _rt06_2)
			compare = 11;
		else
			compare = -11;
		//compare = strcmp(rt06_1, rt06_2);
		break;
	case 3:
		rate1 = CString((char*)pItem1->rate); 
		rate2 = CString((char*)pItem2->rate);
		_rate1 = atof(rate1);
		_rate2 = atof(rate2);
		
		if(_rate1 > _rate2)
			compare = 11;
		else
			compare = -11;
		//compare = strcmp(rt06_1, rt06_2);
		break;
	case 4:
		idtp1 = CString((char*)pItem1->idtp); 
		idtp2 = CString((char*)pItem2->idtp); 
		compare = lstrcmpi(idtp1, idtp2);
		break;
	case 5:
// 		sdat1 = CString((char*)pItem1->sdat); 
// 		sdat2 = CString((char*)pItem2->sdat); 
// 		compare = lstrcmpi(sdat1, sdat2);
		compare = lstrcmpi((char*)pItem1->sdat, (char*)pItem2->sdat); 
		break;
	case 6:
		rknm1 = CString((char*)pItem1->rknm); 
		rknm2 = CString((char*)pItem2->rknm); 
		compare = lstrcmpi(rknm1, rknm2);
		break;
	case 7:
		shgb1 = CString((char*)pItem1->shgb); 
		shgb2 = CString((char*)pItem2->shgb); 
		compare = lstrcmpi(shgb1, shgb2);
		break;
	}
	
	if (m_listsort)
	{
		if (compare > 0)
			compare = -1;
		else if (compare < 0)
			compare = 1;
	}
	
	return compare;
}


CDlg::CDlg(CWnd* pParent /*=NULL*/, CWnd *main)	: CDialog(CDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg)
	//}}AFX_DATA_INIT

	m_parent = pParent;
	m_main = main;

	m_inters.RemoveAll();
	m_list2item.RemoveAll();
	m_listitem.RemoveAll();
	m_listitem2.RemoveAll();
	m_grouplist.RemoveAll();
	m_Newgrouplist.RemoveAll();
	m_searchlist.RemoveAll();
	m_searchlist2.RemoveAll();
	m_dragImage = NULL;
	m_autoScroll = false;

	m_activegno = -1;
	m_selindex = -1;
	m_selectGroup = 0;
	m_nPrevList2 = -1;
	m_bIsEdit = FALSE;
	m_isDuplicateCode = FALSE;
	m_activePage = false;
	m_bAddAll = true;
	m_fdtypeno = 0;
	m_gubnno = 0;
	
	m_newGroup = FALSE;

	m_clickadd = FALSE;
	m_clickdel = FALSE;
	m_up = FALSE;
	m_down = FALSE;
	m_gdnt = 0;
	m_dlginit = TRUE;
	m_delcnt = 0;

	m_confirm = 0;
	comboselect = FALSE;
	editboxselect = FALSE;
	m_ok = FALSE;
	m_createGrp = FALSE;

	m_bSearch = false;
}

CDlg::~CDlg()
{
	
	
}

void CDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
 

	//{{AFX_DATA_MAP(CDlg)
	DDX_Control(pDX, IDC_ERRORMSG, m_errormsg);
	DDX_Control(pDX, IDC_SETRT06, m_setrt06);
	DDX_Control(pDX, IDC_SETRATE, m_setrate);
	DDX_Control(pDX, IDC_RATECHECK, m_ratecheck);
	DDX_Control(pDX, IDC_RT06CHECK, m_rt06check);
	DDX_Control(pDX, IDC_FDTYPE, m_fdtype);
	DDX_Control(pDX, IDC_GUBN, m_gubn);
	DDX_Control(pDX, IDC_IVGRADE, m_ivgrade);
	DDX_Control(pDX, IDC_INPUT, m_search);
	DDX_Control(pDX, IDC_GROUPSELECT, m_groupselect);
	DDX_Control(pDX, IDC_LIST2, m_list2);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg, CDialog)
	//{{AFX_MSG_MAP(CDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_ALLADD, OnAlladd)
	ON_BN_CLICKED(IDC_ALLDELETE, OnAlldelete)
	ON_BN_CLICKED(IDC_LINEADD, OnLineadd)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_NEWGROUP, OnNewgroup)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_GROUPEDIT, OnGroupedit)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST1, OnColumnclickList1)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, OnBegindragList1)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST2, OnBegindragList2)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, OnColumnclickList2)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_CBN_SELCHANGE(IDC_GROUPSELECT, OnSelchangeGroupselect)
	ON_CBN_SELCHANGE(IDC_FDTYPE, OnSelchangeFdtype)
	ON_CBN_SELCHANGE(IDC_GUBN, OnSelchangeGubn)
	ON_CBN_SELCHANGE(IDC_IVGRADE, OnSelchangeIvgrade)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SEARCH, OnSearch)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_EDITX, OnEditXMsg)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_EDITGROUP, OnEditGroup)
	//ON_NOTIFY_REFLECT ( NM_CUSTOMDRAW, OnCustomDrawList )
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, OnCustomDrawList)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST2, OnCustomDrawList2)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg message handlers
long CDlg::OnEditXMsg(WPARAM wParam, LPARAM lParam)					// 펀드 검색시 에디터 박스에 키가 입력 되었을때
{
	CString	string;
	switch ((int) wParam)
	{
	case wpBLANK:
		break;
	case wpENTER:		
		OnSearch();
		break;
	case wpSEARCH1:
		break;
	case wpSEARCH2:		
		break;
	}
	
	return 0;
}

long CDlg::OnEditGroup(WPARAM wParam, LPARAM lParam)
{
	CString	string;
	switch ((int) wParam)
	{
	case DELETEGROUP:
		SendQueryDeleteGroup((char*)lParam, HIWORD(wParam));
		//SendTR("pidomyfd", (CString)lParam, FUNDDELETEGROUP, 0);
		break;
	case UPDATEGROUP:
		SendQueryUpdateGroup((char*)lParam, HIWORD(wParam));
		break;
	}

	return 0;
}

void CDlg::SendQueryDeleteGroup(char* datB, int datL)
{
	char *buf = datB;

	SendTR("pidomyfd", buf, FUNDDELETEGROUP, 0);
}

void CDlg::SendQueryUpdateGroup(char* datB, int datL)
{
	char *buf = datB;

	SendTR("pidomyfd", buf, FUNDUPDATEGROUP, 0);	
}

LONG CDlg::OnMessage(WPARAM wParam, LPARAM lParam)
{
	//	int tets= wParam;
	const int key = HIBYTE(LOWORD(wParam));
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:		// Wizard request TR Header
		// return char*
		break;
	case DLL_OUB:		// Received Data
		// HIBYTE(LOWORD(WPARAM)) : key, HIWORD(WPARAM) : DATA size
		// HIBYTE(LOWORD(WPARAM)) : DATA size for MSB 8bits (control FORM OUB)
		switch (key)
		{
		case FUNDSEARCH:
			{
// 				CEditGroup *editGroup;
// 				editGroup->SendMessage(WM_USER, GROUPINIT); 
				ParsingData((char *)lParam, HIWORD(wParam));
				break;
			}

		case FUNDNEWGROUP:
			ParsingDataNGroup((char *)lParam, HIWORD(wParam));
			break;
		case FUNDCREATEGROUP:
			ParsingCreateNGroup((char *)lParam, HIWORD(wParam));
			break;
		case FUNDLIST:
			ParsingGroupList((char *)lParam, HIWORD(wParam));
			break;
		case FUNDDELETEGROUP:
			ParsingDeleteGroup((char *)lParam, HIWORD(wParam));
			break;
		case FUNDBACKUP:
			ParsingBackup((char *)lParam, HIWORD(wParam));
			break;
		case FUNDRESTORE:
			ParsingRestore((char *)lParam, HIWORD(wParam));
			break;
		case FUNDUPDATEGROUP:

			ParsingCreateNGroup((char *)lParam, HIWORD(wParam));			// 임시로 사용
			break;
		}
// 		ParsingData((char *)lParam, HIWORD(wParam));
// 		break;

	}

	return TRUE;
}

/************************************************************************/
/* TRcode("pihomyfd")를 서버로 전송하여 받은 데이터를 파싱한다.			*/
/* 파싱된 데이터를 토대로 간접투자대상 콤보박스를 세팅한다.				*/
/************************************************************************/
void CDlg::ParsingData(char* datB, int datL)
{
	struct item1
	{
		char type[1];
		char data[40];
	};
	
	struct mod1
	{
		char nrec[4];
		char ltcd[4];

		struct item1 item1[MYFD_DEFMAXCNT];
	}*mod1{};

	char *buf = datB;
	mod1 = (struct mod1*)buf;
	CString nrec, ltcd, type, data;
	CString gno, gnm;

	std::unique_ptr<_grouplist> grouplist{};
	///_grouplist* grouplist{};
	
	m_fdtype.ResetContent();
	m_gubn.ResetContent();
	m_ivgrade.ResetContent();

	int ncnt = 0;
	const int nsize = sizeof(struct item1);
	int idx = 0;
	int gubncnt = 0;
	int ivgd = 0;

	nrec.Format("%.*s", sizeof(mod1->nrec), mod1->nrec);
//	m_gcnt = atoi(nrec);
	
	ncnt = atoi(nrec);

	ltcd.Format("%.*s", sizeof(mod1->ltcd), mod1->ltcd);
	ltcd.TrimLeft();
	//IH::Parser(ltcd, "-");

	//m_main->SendMessage(WM_MESSAGE, GETLASTPAGE, 0);
	m_grouplist.RemoveAll();
	
	if(m_dlginit)
		m_selectGroup = atoi(ltcd);
	else
		m_selectGroup = m_groupselect.GetCount()-1;
	
	TRACE("m_selectGroup = %d\n", m_selectGroup);
	for(int ii = 0; ii < ncnt; ii++)
	{
		type.Format("%.*s", sizeof(mod1->item1[ii].type), mod1->item1[ii].type);
		type.TrimLeft();

		data.Format("%.*s", sizeof(mod1->item1[ii].data), mod1->item1[ii].data);
		data.TrimRight();

		if(strcmp(type, "0") == 0)
		{
			idx = m_fdtype.AddString(data);
			m_fdtype.SetItemData(idx, ii);
		}
		else if(strcmp(type, "1") == 0)
		{
			idx = m_gubn.AddString(data);
			m_gubn.SetItemData(idx, gubncnt);
			gubncnt++;
		}
		else if(strcmp(type, "2") == 0)
		{
			idx = m_ivgrade.AddString(data);
			m_ivgrade.SetItemData(idx, ivgd);
			ivgd++;
		}	
		else if(strcmp(type, "3") == 0)
		{
//			grouplist = new _grouplist();
			grouplist = std::make_unique< _grouplist >();

			gno = data.Mid(0, 4);
			const int len = strlen(data);
			gnm = data.Mid(4, len);
			
			strcpy(grouplist->gno, gno);
			strcpy(grouplist->gnm, gnm);
			
			m_grouplist.Add(grouplist.release());
		}
	}

	initCombo();									
	loadingInterest(m_selectGroup);				// 초기 리스트 2에 보여줄 관심종목은 파일을 바로 로드하기 때문에 1을 더해준다

	m_fdtype.SetCurSel(0);
	m_gubn.SetCurSel(0);
	m_ivgrade.SetCurSel(0);
}

void CDlg::ParsingDataNGroup(char* datB, int datL)
{
	struct _mod1* mod1{};
	char *buf = datB;
	mod1 = (struct _mod1*)buf;
	CString rest, ercd;

	rest.Format("%.*s", sizeof(mod1->rest), mod1->rest);

	ercd.Format("%.*s", sizeof(mod1->ercd), mod1->ercd);

	if(strcmp(rest, "N") == 0)
		AfxMessageBox("관심펀드가 서버에 저장되지 않았습니다." "Error CODE : " + ercd);

// 	if(m_ok == TRUE)
// 		DestroyWindow();
}

BOOL CDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_root = Variant(homeCC);
	m_name = Variant(nameCC);
	
	QueryBackup();					// 현재 관심펀드 상태 
	QueryExcelSise();

	initList();

 	LoadFile();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlg::QueryBackup()
{
	CString strData, grcd;
	char sendB[1]{};
	struct _mid5* mid5 = (struct _mid5 *)&sendB[0];
	
//	FillMemory(&mid5, sizeof(struct _mid5), ' ');
//	FillMemory(sendB, sizeof(sendB), ' ');
	
	sendB[0] = 'b';

//	CopyMemory(sendB, "b", sizeof(mid5->gubn));
	
	strData = "b";

	SendTR("pidomyfd", strData, FUNDBACKUP, 0);
}

void CDlg::QueryRestore()							// esc 및 취소 클릭시 처음 상태 복구
{
	CString strData, grcd;
	char sendB[1]{};
	struct _mid5* mid5 = (struct _mid5 *)&sendB[0];
	
	FillMemory(&mid5, sizeof(struct _mid5), ' ');
	FillMemory(sendB, sizeof(sendB), ' ');
	
	CopyMemory(sendB, "r", sizeof(mid5->gubn));
	
	//strData = CString((char *)&mid2, sizeof(struct __mid2));
	//TRACE("Main data : %s\n", strData);
	//	SendTR("pihomyfd", strData, FUNDINIT, 0);
	SendTR("pidomyfd", sendB, FUNDRESTORE, 0);
}
void CDlg::QueryExcelSise()							
{
	CString strData;
	struct _4mid1 mid1;
	ZeroMemory(&mid1, sizeof(struct _4mid1));
	FillMemory(&mid1, sizeof(struct _4mid1), ' ');
	
	//	CopyMemory(&mid1.gubn, "", sizeof(mid1.gubn));
	//	CopyMemory(&mid1.nrce, "000", sizeof(mid1.nrce));
	
	// 	memset(&mid1, ' ', sizeof(mid1));
	// 	strncpy(mid1.grcd, "AAAAAAAA", 8);
	
	strncpy(mid1.gubn, "A", 1);
	strData = CString((char *)&mid1, sizeof(struct _4mid1));
	
	/*SendTR("pihomyfd", (char*)&mid1, FUNDSEARCH, 0);*/
	SendTR("pihomyfd", strData, FUNDSEARCH, 0);
}

LRESULT CDlg::SendTR(CString strCode, CString strData, int iKey, int iStat)
{
	std::unique_ptr<char[]>pcDataBuffer = std::make_unique<char[]>(L_userTH + strData.GetLength());

//	ZeroMemory(pcDataBuffer.get(), sizeof(pcDataBuffer));
	struct	_userTH* puserTH;
	puserTH = (struct _userTH *)pcDataBuffer.get();
	
	strcpy(puserTH->trc, strCode.operator LPCTSTR());
	puserTH->key = iKey;
	puserTH->stat = iStat;
	
	CopyMemory(&pcDataBuffer.get()[L_userTH], strData.GetBuffer(), strData.GetLength());
		
	const LRESULT lResult = m_parent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), (LPARAM) pcDataBuffer.get());
	
	pcDataBuffer = nullptr;

	return lResult;
}

void CDlg::LoadFile()
{
	_ITEM01 Item01{};
	CString fdFile;
	//int i =0;
	CString code, name, rt06, rate, idtp, sdat, rknm, shgb, lnfd;
	int next = 120;


//	std::unique_ptr<_listitem> listitem{};
	_listitem* listitem{};
	//float frt06, frate;

	CopyMemory(&Item01, "", sizeof(Item01));
	
	fdFile.Format("%s/%s/ffcode.dat", m_root, TABDIR);

	CFile file(fdFile, CFile::modeRead);

	file.SeekToBegin();
 
	const int codeN = (int)(file.GetLength() / sizeof(Item01));

	for(int ii = 0; ii < codeN; ii++)
	{	
		listitem = new _listitem();

//		listitem = std::make_unique<_listitem>();

		file.Read(&Item01, sizeof(Item01));
	
		code = CString(Item01.code, 12);
		code.TrimRight();
		name = CString(Item01.name, 40);
		name.TrimRight();
		rt06 = CString(Item01.rt06, 9);
		rt06.TrimLeft();
		rate = CString(Item01.rate, 9);
		rate.TrimLeft();
		idtp = CString(Item01.idtp, 20);
		idtp.TrimRight();
		sdat = CString(Item01.sdat, 8);
		rknm = CString(Item01.rknm, 20);
		rknm.TrimRight();
		shgb = CString(Item01.shgb, 1); 
		lnfd = CString(Item01.lnfd, 1); 
		
		if(strcmp(shgb, "1") == 0)
			shgb = "인터넷거래가능";
		else if(strcmp(shgb, "0") == 0)
			shgb = "인터넷거래불가능";		
			
		sdat.Insert(4,"/");
		sdat.Insert(7,"/");
		
		strcpy(listitem->code, code);
		strcpy(listitem->name, name);
		strcpy(listitem->rt06, rt06);
		strcpy(listitem->rate, rate);
		strcpy(listitem->idtp, idtp);
		strcpy(listitem->sdat, sdat);
		strcpy(listitem->rknm, rknm);
		strcpy(listitem->shgb, shgb);

		m_listitem.Add(listitem);
		m_searchlist.Add(listitem);
		AppendItemToList1(0, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem);

		file.Seek(next, CFile::begin);
		next += 120;
	}

	//2017.01.18 KSJ 전부 넣고나서 소팅
	ListView_SortItems(m_list1, ListViewCompareFunc, 0);

	UpdateData();

 	file.Close();
}

void CDlg::loadConfig()
{
	CString tempN; 
	
	tempN.Format("%s/%s/%s/interField.ini.tmp", m_root, USRDIR, m_name);
	
	CFileFind finder;
	if (!finder.FindFile(tempN))
	{
		tempN.Format("%s/%s/%s/interField.ini", m_root, USRDIR, m_name);
		if (!finder.FindFile(tempN))
		{
			return;
		}
	}
	
	int	readL = 0;
	
	readL = GetPrivateProfileInt("INTERFIELD", "OVERLAP", 0, tempN);
	m_isDuplicateCode = readL;
}

int CDlg::loadingInterest(int gno)
{
	SendInterestData(gno);
// 	CString	filePath, fileBook;
// 
// 	filePath.Format("%s/%s/%s/FDportfolio.i%02d", m_root, USRDIR, m_name, gno);
// 	
// 	FILE *fp;
// 	char code[12], char name[40];
// 
// 	int count = 0;
// 	
// 	CopyMemory(code, "", sizeof(code));
// 	CopyMemory(name, "", sizeof(name));
// 
// 	
// 	if((fp = fopen(filePath, "r")) == NULL)
// 		return 0;
// 	
// 	fseek(fp, 0, SEEK_SET);
// 	
// 	
// 	while(!feof(fp))
// 	{
// 		fscanf(fp, "%s\t%s\n", &code, &name);
// 		
// 		int test = strcmp(code, "");
// 		if(test == 0)
// 			break;
// 		else
// 		{
// 		//	TRACE("file output : %s\t%s\n", code, name );
// 		//	listcount = m_list2.GetItemCount();
// 		
// // 			int nIndex = m_list2.InsertItem(count, code, 1);						
// // 			m_list2.SetItemText(nIndex, 1, name);
// 			AppendItemToList2(0, code, name);
// 		}
// 	}
// 	
// 
// 	fclose(fp);
	return 0;

}

void CDlg::AppendItemToList2(int ipos, CString code, CString name, CString rt06, CString rate, CString idtp, CString sdat, CString rknm, CString shgb, _listitem *pListItem)
{
	int nItem = 0;

	_listitem* listitem{};
	
	name.TrimRight();
	
	nItem = AddItem(&m_list2, code, ipos);
	AddItem(&m_list2, name, nItem, 1);
	AddItem(&m_list2, rt06, nItem, 2);
	AddItem(&m_list2, rate, nItem, 3);
	AddItem(&m_list2, idtp, nItem, 4);
	AddItem(&m_list2, sdat, nItem, 5);
	AddItem(&m_list2, rknm, nItem, 6);
	AddItem(&m_list2, shgb, nItem, 7);
	
	if (pListItem)
	{
		m_list2.SetItemData(nItem, (LPARAM) pListItem);
	}
	else	// 처음 종류를 선택할때만 추가된다.
	{
		listitem = new _listitem();

		strcpy(listitem->code, code);
		strcpy(listitem->name, name);
		strcpy(listitem->rt06, rt06);
		strcpy(listitem->rate, rate);
		strcpy(listitem->idtp, idtp);
		strcpy(listitem->sdat, sdat);
		strcpy(listitem->rknm, rknm);
		strcpy(listitem->shgb, shgb);

		m_listitem2.Add(listitem);
		m_list2.SetItemData(nItem, (LPARAM) listitem);
	}
}

void CDlg::initList()
{
	AddColumn(&m_list1, "펀드코드", 0, 24);
	AddColumn(&m_list1, "펀드명", 1, 228);
	AddColumn(&m_list1, "6개월수익률", 2, 12, LVCFMT_RIGHT);
	AddColumn(&m_list1, "누적수익률", 3, 12, LVCFMT_RIGHT);
	AddColumn(&m_list1, "간접투자유형", 4, 20);
	AddColumn(&m_list1, "설정일", 5, 37);
	AddColumn(&m_list1, "투자등급명", 6, 20); 
	AddColumn(&m_list1, "조회구분", 7, 70);
	m_list1.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	AddColumn(&m_list2, "펀드코드", 0, 24);
	AddColumn(&m_list2, "펀드명", 1, 228);
	AddColumn(&m_list2, "6개월수익률", 2, 12, LVCFMT_RIGHT);
	AddColumn(&m_list2, "누적수익률", 3, 12, LVCFMT_RIGHT);
	AddColumn(&m_list2, "간접투자유형", 4, 20);
	AddColumn(&m_list2, "설정일", 5, 37);
	AddColumn(&m_list2, "투자등급명", 6, 20); 
	AddColumn(&m_list2, "조회구분", 7, 70);
	m_list2.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

BOOL CDlg::AddColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width, int textfmt)
{
	LV_COLUMN lvc;
	
	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT ;
	lvc.fmt  = textfmt;
	lvc.pszText = (LPTSTR) string;
	lvc.cx      = ctrl->GetStringWidth(lvc.pszText) + width;
	
	return ctrl->InsertColumn(item, &lvc);
}

void CDlg::initCombo()
{
	int	idx=0;	
//	int	readL = 0;
//	char	readB[256] = _T("");
	CString	string, gno, gname, filePath;
	int groupno=0;
	
	m_groupselect.ResetContent();
	
	const int ncnt = m_grouplist.GetSize();
	_grouplist* grouplist{};
	
	for(int ii = 0; ii < ncnt; ii++)
	{
		grouplist = m_grouplist.GetAt(ii);
		
		gno = CString(grouplist->gno, sizeof(grouplist->gno));
		gname = CString(grouplist->gnm, sizeof(grouplist->gnm));
		// 		strcpy(gno, grouplist->fdcd);
		// 		strcpy(gname, grouplist->fdnm);
		groupno = atoi(gno);
		if(!gname.IsEmpty())
		{
			idx = m_groupselect.AddString(gname);
			m_groupselect.SetItemData(idx, atoi(gno));
		}
	}

	if (m_activegno == -1 && m_groupselect.GetCount() > 0)
	{
		if (m_selectGroup < 0 )
		{
			idx = m_groupselect.SetCurSel(0);
		//	loadingInterest(idx);
		}
		else
		{
			idx = m_groupselect.SetCurSel(m_selectGroup);//BUFFET 0
//			loadingInterest(idx);
		}
		m_activegno = m_groupselect.GetItemData(idx);
	}
	else
	{
		m_groupselect.SetCurSel(ncnt-1);
	}
}

CString CDlg::parseX(CString &srcstr, CString substr)
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

CString CDlg::Variant(int nComm, CString strData)
{
	CString strRet(_T(""));
	
	char* pData = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);
	
	if ((long)pData > 1)
		strRet = pData;
	
	return strRet;
}

int CDlg::AddItem(CListCtrl *ctrl, LPCSTR string, int nItem, int nSubItem)
{
	if (nSubItem < 0) // code
		return ctrl->InsertItem(LVIF_TEXT, nItem, string, NULL, NULL, 0, 0);
	else		  // name
		return ctrl->SetItemText(nItem, nSubItem, string);
}

void CDlg::OnAdd() 
{
	if (m_activegno == -1)
	{
		m_errormsg.SetWindowText("선택된 그룹이 없습니다. 새그룹을 등록하세요");
		return;
	}

	const int nCount = m_list1.GetItemCount();
	int listcount = 0;
	CString str = "추가시킬 항목을 선택하십시오.";
	CString code, name, rt06, rate, idtp, sdat, rknm, shgb;
	POSITION pos = m_list1.GetFirstSelectedItemPosition();

	if(pos == 0)
		m_errormsg.SetWindowText(str);	
	
	for(int ii=0; ii < nCount; ii++)
	{
		if(m_list1.GetItemState(ii, LVIS_SELECTED) != 0)		// 리스트에서 아이템이 선택되어 있는지 확인
		{
			code = m_list1.GetItemText(ii, 0);					// 리스트의 값을 얻어온다
			name = m_list1.GetItemText(ii, 1);
			rt06 = m_list1.GetItemText(ii, 2);
			rate = m_list1.GetItemText(ii, 3);
			idtp = m_list1.GetItemText(ii, 4);
			sdat = m_list1.GetItemText(ii, 5);
			rknm = m_list1.GetItemText(ii, 6);
			shgb = m_list1.GetItemText(ii, 7);
			
			listcount = m_list2.GetItemCount();
			
			if(existCodeInList2(code))
			{
				continue;
			}

			if(listcount >= MYFD_MAXFUND )
			{
				m_errormsg.SetWindowText("최대로 저장할 수 있는 종목개수를 초과하였습니다.");
				break;
			}
			else
			{
// 				int nIndex = m_list2.InsertItem(listcount, code, 1);						
// 				m_list2.SetItemText(nIndex, 1, name);
				AppendItemToList2(ii, code, name, rt06, rate, idtp, sdat, rknm, shgb);
			}
		}
	}

	const int num = m_groupselect.GetCurSel();
	m_clickadd = TRUE;
}

void CDlg::OnDelete() 
{
	if (m_activegno == -1)
	{
		m_errormsg.SetWindowText("선택된 그룹이 없습니다. 새그룹을 등록하세요");
		return;
	}
	
	int nCount = m_list2.GetItemCount();
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	int ii = 0;
	int jj = 0;
	
	_listitem* listitem{};
	const char fdcd[12]{};

	_rmlist[MYFD_MAXCNT-1] = -1;
	rmlist[MYFD_MAXCNT-1] = -1;

	if(pos == 0)
		m_errormsg.SetWindowText("삭제시킬 항목을 선택하십시오.");

// 	for(; nCount >= 0; nCount--)
// 	{
// 	//	listitem = m_listitem2.GetAt(ii);
// 		if(m_list2.GetItemState(nCount, LVIS_SELECTED) != 0)
// 		{
// // 			fdcd = listitem->code;
// // 			FillMemory(m_afcd[ii], sizeof(m_afcd[ii]), ' ');
// // 			CopyMemory(m_afcd[ii], listitem->code,  strlen(listitem->code));
// 			m_list2.DeleteItem(nCount);	
// 	
// 		}
// 		ii++;
// 	}
// 	int test = m_listitem2.GetSize();
	for(; nCount >= 0; nCount--)										// 리스트에서 삭제부분
	{
		if(m_list2.GetItemState(nCount, LVIS_SELECTED) != 0)
		{
// 			CopyMemory(fdcd, listitem->code, sizeof(listitem->code));
 			
			m_list2.DeleteItem(nCount);	
			_rmlist[jj] = nCount;
			jj++;
			m_gdnt++;
			/*m_listitem2.RemoveAt(jj);*/
		}
	//	test = m_listitem2.GetSize();
	}

	const int cntlist = m_listitem2.GetSize();
// 
	for(jj = cntlist-1, ii = 0; jj >= 0; jj--, ii++)
	{
		rmlist[jj] = _rmlist[ii];
	}

	for(ii = 0, jj =0; ii < cntlist; ii++, jj++)
	{
		listitem = m_listitem2.GetAt(jj);
		
		if(rmlist[ii] > 0)
		{
			FillMemory(m_afcd[m_delcnt], sizeof(m_afcd[m_delcnt]), ' ');
 			CopyMemory(m_afcd[m_delcnt], listitem->code,  strlen(listitem->code));
			//delete m_listitem2.GetAt(jj);		
			m_listitem2.RemoveAt(jj);
			jj--;
			m_delcnt++;
		}
		//int test = m_listitem2.GetSize();
	}

	//const int num = m_groupselect.GetCurSel();
	m_clickdel = TRUE;
}

void CDlg::OnAlladd() 
{
	const int nCount = m_list1.GetItemCount();
	CString code, name, rt06, rate, idtp, sdat, rknm, shgb;
	int listcount = 0;

	if (m_activegno == -1)
	{
		m_errormsg.SetWindowText("선택된 그룹이 없습니다. 새그룹을 등록하세요");
		return;
	}

	for(int ii=0; ii < nCount; ii++)
	{
		code = m_list1.GetItemText(ii, 0);					// 리스트의 값을 얻어온다
		name = m_list1.GetItemText(ii, 1);
		rt06 = m_list1.GetItemText(ii, 2);
		rate = m_list1.GetItemText(ii, 3);
		idtp = m_list1.GetItemText(ii, 4);
		sdat = m_list1.GetItemText(ii, 5);
		rknm = m_list1.GetItemText(ii, 6);
		shgb = m_list1.GetItemText(ii, 7);
			
		listcount = m_list2.GetItemCount();
		
		if(existCodeInList2(code))
		{
			continue;
		}
		if(listcount >= MYFD_MAXFUND)
		{
			m_errormsg.SetWindowText("최대로 저장할 수 있는 종목개수를 초과하였습니다.");
			break;
		}
		else
		{
			const int nIndex = m_list2.InsertItem(listcount, code, 1);						
			m_list2.SetItemText(nIndex, 1, name);
			m_list2.SetItemText(nIndex, 2, rt06);
			m_list2.SetItemText(nIndex, 3, rate);
			m_list2.SetItemText(nIndex, 4, idtp);
			m_list2.SetItemText(nIndex, 5, sdat);
			m_list2.SetItemText(nIndex, 6, rknm);
			m_list2.SetItemText(nIndex, 7, shgb);
		}	
	}

	//int num = m_groupselect.GetCurSel();
	m_clickadd = TRUE;
}

void CDlg::OnAlldelete() 
{
	if (m_activegno == -1)
	{
		m_errormsg.SetWindowText("선택된 그룹이 없습니다. 새그룹을 등록하세요");
		return;
	}
	
	m_bIsEdit = TRUE;
	
	const int ncnt = m_list2.GetItemCount();
	CString code;

	for(int ii=0; ii < ncnt; ii++)
	{
		code = m_list2.GetItemText(ii, 0);	
		
		FillMemory(m_afcd[ii], sizeof(m_afcd[ii]), ' ');
 		CopyMemory(m_afcd[ii], code,  strlen(code));
	}

	m_list2.DeleteAllItems();	

	const int num = m_groupselect.GetCurSel();
	m_clickdel = TRUE;
	m_gdnt = ncnt;
}

void CDlg::OnLineadd() 
{
	POSITION   pos{};
	int listcount{};
	const int nCount = m_list2.GetItemCount();

	pos = m_list2.GetFirstSelectedItemPosition();

	if(pos == 0)
	{
		listcount = m_list2.GetItemCount();
		
		if(listcount >= MYFD_MAXFUND)
			m_errormsg.SetWindowText("최대로 저장할 수 있는 종목개수를 초과하였습니다.");
		else
		{
			const int nIndex = m_list2.InsertItem(nCount, "", 1);						
			m_list2.SetItemText(nIndex, 1, "");
		}
	}
	else
	{
		while(pos)
		{
			const int   nSelItem = m_list2.GetNextSelectedItem(pos);
			
			listcount = m_list2.GetItemCount();
			
			if(listcount >= MYFD_MAXFUND)
			{
				m_errormsg.SetWindowText("최대로 저장할 수 있는 종목개수를 초과하였습니다.");
				break;
			}
			else
			{
				const int nIndex = m_list2.InsertItem(nSelItem, "", 1);						
				m_list2.SetItemText(nIndex, 1, "");
			}
		}
	}

//	const int num = m_groupselect.GetCurSel();
	m_clickadd = TRUE;
}

void CDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnAdd();

	*pResult = 0;
}

void CDlg::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDelete();

	*pResult = 0;
}

void CDlg::OnDown() 
{
	int	nItem{}, data{};
	CString code, name, rt06, rate, idtp, sdat, rknm, shgb;
	CString	gname, gno;
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = m_list2.GetNextSelectedItem(pos);
		if (nItem != m_list2.GetItemCount())
		{
			code = m_list2.GetItemText(nItem, 0);
			name = m_list2.GetItemText(nItem, 1);
			rt06 = m_list2.GetItemText(nItem, 2);
			rate = m_list2.GetItemText(nItem, 3);
			idtp = m_list2.GetItemText(nItem, 4);
			sdat = m_list2.GetItemText(nItem, 5);
			rknm = m_list2.GetItemText(nItem, 6);
			shgb = m_list2.GetItemText(nItem, 7);

			data = m_list2.GetItemData(nItem);

			m_list2.DeleteItem(nItem);
			
			nItem = addItem(code, nItem+1);
			AddItem(&m_list2, name, nItem, 1);
			AddItem(&m_list2, rt06, nItem, 2);
			AddItem(&m_list2, rate, nItem, 3);
			AddItem(&m_list2, idtp, nItem, 4);
			AddItem(&m_list2, sdat, nItem, 5);
			AddItem(&m_list2, rknm, nItem, 6);
			AddItem(&m_list2, shgb, nItem, 7);
			

			m_list2.SetItemData(nItem, data);
			
			
			KillSelected(&m_list2);
			SetSelected(&m_list2, nItem, FALSE);
			
			m_selindex = nItem;
		}
	}	
	
	m_down = TRUE;
}

void CDlg::OnUp() 
{
	int	nItem{}, data{};
	CString code, name, rt06, rate, idtp, sdat, rknm, shgb;
	CString	gname, gno;
	POSITION pos = m_list2.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = m_list2.GetNextSelectedItem(pos);
		if (nItem != 0)
		{
			code = m_list2.GetItemText(nItem, 0);
			name = m_list2.GetItemText(nItem, 1);
			rt06 = m_list2.GetItemText(nItem, 2);
			rate = m_list2.GetItemText(nItem, 3);
			idtp = m_list2.GetItemText(nItem, 4);
			sdat = m_list2.GetItemText(nItem, 5);
			rknm = m_list2.GetItemText(nItem, 6);
			shgb = m_list2.GetItemText(nItem, 7);
			data = m_list2.GetItemData(nItem);
			m_list2.DeleteItem(nItem);
			
			
			nItem = addItem(code, nItem - 1);
			AddItem(&m_list2, name, nItem, 1);
			AddItem(&m_list2, rt06, nItem, 2);
			AddItem(&m_list2, rate, nItem, 3);
			AddItem(&m_list2, idtp, nItem, 4);
			AddItem(&m_list2, sdat, nItem, 5);
			AddItem(&m_list2, rknm, nItem, 6);
			AddItem(&m_list2, shgb, nItem, 7);

			m_list2.SetItemData(nItem, data);
			
			KillSelected(&m_list2);
			SetSelected(&m_list2, nItem, FALSE);
			
			m_selindex = nItem;
		}
	}
	else
	{
		GuideMsg(idTOstring(IDS_GUIDE2));
		return;
	}
	
	m_up = TRUE;
}

int CDlg::addItem(const CString &strItem, int nItem)
{
	LV_ITEM lvi;
	
	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = nItem;
	lvi.iSubItem = 0;
	lvi.pszText  = (LPTSTR) strItem.operator LPCTSTR();
	
	return m_list2.InsertItem(&lvi);
}

void CDlg::KillSelected(CListCtrl *ListCtrl)
{
	int nItem{};
	POSITION pos = ListCtrl->GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			nItem = ListCtrl->GetNextSelectedItem(pos);
			ListCtrl->SetItemState(nItem, NULL, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

void CDlg::SetSelected(CListCtrl *ListCtrl, int nItem, BOOL scroll/* = TRUE */)
{
	CRect	rc;
	CSize	size;
	
	ListCtrl->SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	
	if (scroll)
	{
		ListCtrl->GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - ListCtrl->GetTopIndex());
		ListCtrl->Scroll(size);
	}
}

void CDlg::OnBegindragList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW * pNMListView = (NM_LISTVIEW *) pNMHDR;

	*pResult = 0;

	if (m_list1.GetSelectedCount() == 0)
		return;
	
	m_dragData.list = 1;
	m_dragData.item = pNMListView->iItem;
	
	if (m_dragImage)
		delete m_dragImage;
	
	CPoint	pt;
	m_dragImage = CreateDragImageSymbol(&pt, &m_list1);
	
	if (m_dragImage == (CImageList *)NULL)
		return;
	
	CPoint	ptStart = pNMListView->ptAction;
	m_list1.ClientToScreen(&ptStart);
	
	m_dragImage->BeginDrag(0, CPoint(0, 0));
	m_dragImage->DragEnter(GetDesktopWindow(), ptStart);
	SetCapture();
}

CImageList* CDlg::CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl* listctrl)
{
	const int count = listctrl->GetSelectedCount();
	if (count < 1) return NULL;
	
	CRect itemRC, totalRC; totalRC.SetRectEmpty();
	
	POSITION pos = listctrl->GetFirstSelectedItemPosition();
	if (!pos) return NULL;
	
	int item = listctrl->GetNextSelectedItem(pos);
	listctrl->GetItemRect(item, itemRC, LVIR_BOUNDS);
	totalRC = itemRC;
	totalRC.bottom = totalRC.top + totalRC.Height() * count;
	
	CDC memDC; CClientDC dcClient(this);
	if (!memDC.CreateCompatibleDC(&dcClient))
		return NULL;
	
	CBitmap bitmap;
	if (!bitmap.CreateCompatibleBitmap(&dcClient, totalRC.Width(), totalRC.Height()))
		return NULL;
	
	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(0, 0, totalRC.Width(), totalRC.Height(), RGB(255, 255, 255));
	
	
	std::unique_ptr<CImageList> pCompleteImageList = std::make_unique<CImageList>();
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
			delete pimglstSingle; pimglstSingle = NULL;
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

void CDlg::OnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;
	
	if (m_list2.GetSelectedCount() == 0)
		return;
	
	m_dragData.list = 2;
	m_dragData.item = pNMListView->iItem;
	m_nItem = pNMListView->iItem;

	if (m_dragImage)
		delete m_dragImage;
	
	CPoint	pt;
	m_dragImage = CreateDragImageSymbol(&pt, &m_list2);
	
	if (m_dragImage == (CImageList *)NULL)
		return;
	
	CPoint	ptStart = pNMListView->ptAction;
	m_list2.ClientToScreen(&ptStart);
	
	m_dragImage->BeginDrag(0, CPoint(0, 0));
	m_dragImage->DragEnter(GetDesktopWindow(), ptStart);
	SetCapture();
}

void CDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	::ReleaseCapture();	
	
	if (m_dragImage)
	{
		m_dragImage->DragLeave(GetDesktopWindow());
		m_dragImage->EndDrag();
		m_dragImage->DeleteImageList();
		delete m_dragImage;
		m_dragImage = NULL;
		
		OnEndDrag(point);
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CDlg::OnEndDrag(CPoint point)
{
	CRect	list1RC, list2RC;
	
	CString code, name, rt06, rate, idtp, sdat, rknm, shgb;

	m_list1.GetClientRect(&list1RC); 
	m_list1.ClientToScreen(&list1RC);
	m_list2.GetClientRect(&list2RC); 
	m_list2.ClientToScreen(&list2RC);
	
	ClientToScreen(&point);
	m_bIsEdit = TRUE;	
	
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
		
		int nItem = m_list2.HitTest(&lvhti);
		
		if (nItem < 0)
			nItem = m_list2.GetItemCount() -1;
		
		if (m_dragData.list == 2)
		{
			code = m_list2.GetItemText(m_nItem, 0);
			name = m_list2.GetItemText(m_nItem, 1);
			rt06 = m_list2.GetItemText(m_nItem, 2);
			rate = m_list2.GetItemText(m_nItem, 3);
			idtp = m_list2.GetItemText(m_nItem, 4);
			sdat = m_list2.GetItemText(m_nItem, 5);
			rknm = m_list2.GetItemText(m_nItem, 6);
			shgb = m_list2.GetItemText(m_nItem, 7);

			const DWORD dData = m_list2.GetItemData(m_nItem);
			m_list2.DeleteItem(m_nItem);
			
			nItem = addItem(code, nItem);
			AddItem(&m_list2, name, nItem, 1);
			AddItem(&m_list2, rt06, nItem, 2);
			AddItem(&m_list2, rate, nItem, 3);
			AddItem(&m_list2, idtp, nItem, 4);
			AddItem(&m_list2, sdat, nItem, 5);
			AddItem(&m_list2, rknm, nItem, 6);
			AddItem(&m_list2, shgb, nItem, 7);

			//AppendItemToList2(nItem, code, name, rt06, rate, idtp, sdat, rknm, shgb);
			m_list2.SetItemData(nItem, dData);
			
			KillSelected(&m_list2);
			SetSelected(&m_list2, nItem, FALSE);

			m_up = TRUE;
			m_down = TRUE;
		}
		else if (m_dragData.list == 1)
		{	
			insertList1toList2(nItem);
			m_clickadd = TRUE;
		}
	}
}

void CDlg::deleteList2()
{
	if (m_activegno == -1)
	{
		m_errormsg.SetWindowText("선택된 그룹이 없습니다. 새그룹을 등록하세요");
		return;
	}
	
	m_bIsEdit = TRUE;
	
	int nItem = -1;
	POSITION pos{};
	int count = m_list2.GetItemCount();
	for (int ii = 0; ii < count; ii++)
	{
		pos = m_list2.GetFirstSelectedItemPosition();
		if (pos != NULL)
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

void CDlg::DeleteItemToList2(int ipos)
{
	if (m_list2.GetItemCount() > ipos)
		m_list2.DeleteItem(ipos);
	
/*
	if (m_inters.GetSize() > ipos)
	{
		delete m_inters.GetAt(ipos);
		m_inters.RemoveAt(ipos);
	}
*/
	
	if (m_list2item.GetSize() > ipos)
	{
		delete m_list2item.GetAt(ipos);
		m_list2item.RemoveAt(ipos);
	}
}

CString CDlg::GetCodeName(CString code)
{
	code = code.Left(12);
	code.TrimRight();
	code.TrimLeft();
//	code.Remove(0);
	
//	int codeL = code.GetLength();
	const int codeL = strlen(code);

	return _T("");
}

void CDlg::SetListColumnText(CListCtrl* pList, int nCol, int nAsc)
{
	CHeaderCtrl* pCtrl = pList->GetHeaderCtrl();
	
	if (NULL != pCtrl)
	{
		CString strText;
		
		switch(nCol) 
		{
		case 0:
			strText = "펀드코드";
			break;
		case 1:
			strText = "펀드명";
			break;
		case 2:
			strText = "6개월수익률";
			break;
		case 3:
			strText = "누적수익률";
			break;
		case 4:
			strText = "간접투자유형";
			break;
		case 5:
			strText = "설정일";
			break;
		case 6:
			strText = "투자등급명";
			break;
		case 7:
			strText = "조회구분";
			break;
		}
		
		switch (nAsc) {
		case 0:
			{
				strText += " ▼";
			}
			break;
		case 1:
			{
				strText += " ▲";
			}
			break;
		}
		
		HDITEM hdItem;
		hdItem.mask = HDI_TEXT;
		hdItem.pszText = strText.GetBuffer(0);
		
		pCtrl->SetItem(nCol, &hdItem);
	}
}

void CDlg::deselcMark(CListCtrl* listctrl)
{
	int nItem{};
	POSITION pos = listctrl->GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			nItem = listctrl->GetNextSelectedItem(pos);
			listctrl->SetItemState(nItem, NULL, LVIS_SELECTED|LVIS_FOCUSED);
		}
	}
}

void CDlg::selcMark(CListCtrl* listctrl, int nItem, bool scroll /*=true*/)
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

void CDlg::insertList1toList2(int sitem)
{
	CString	gname;
	int listcount{};
	if (m_activegno == -1)
	{
		m_errormsg.SetWindowText("선택된 그룹이 없습니다. 새그룹을 등록하세요");
		return;
	}
	
	m_bIsEdit = TRUE;
	
	deselcMark(&m_list2);
	
	int	nitem{};
	CString	code, name, rt06, rate, idnm, stdt, ivnm, scgm;
	
	POSITION pos = m_list1.GetFirstSelectedItemPosition();
	
	while (pos)
	{
		nitem = m_list1.GetNextSelectedItem(pos);
		
		code = m_list1.GetItemText(nitem, 0);
		name = m_list1.GetItemText(nitem, 1);
		rt06 = m_list1.GetItemText(nitem, 2);
		rate = m_list1.GetItemText(nitem, 3);
		idnm = m_list1.GetItemText(nitem, 4);
		stdt = m_list1.GetItemText(nitem, 5);
		ivnm = m_list1.GetItemText(nitem, 6);
		scgm = m_list1.GetItemText(nitem, 7);

		listcount = m_list2.GetItemCount();

		if (existCodeInList2(code))
		{	
			//			GuideMsg("동일한 종목이 관심그룹 내에 존재합니다.");
			continue;
		}
		
		if(listcount >= MYFD_MAXFUND )
		{
			m_errormsg.SetWindowText("최대로 저장할 수 있는 종목개수를 초과하였습니다.");
			break;
		}
		else
		{
// 			int nIndex = m_list2.InsertItem(sitem+1, code, 1);						
// 			m_list2.SetItemText(nIndex, 1, name);
// 			m_list2.SetItemText(nIndex, 2, rt06);
// 			m_list2.SetItemText(nIndex, 3, rate);
// 			m_list2.SetItemText(nIndex, 4, idnm);
// 			m_list2.SetItemText(nIndex, 5, stdt);
// 			m_list2.SetItemText(nIndex, 6, ivnm);
// 			m_list2.SetItemText(nIndex, 7, scgm);			
			AppendItemToList2(nitem, code, name, rt06, rate, idnm, stdt, ivnm, scgm);
		}
	}

	InitEdit();
}

void CDlg::InitEdit()
{
	m_search.SetWindowText("");
}

bool CDlg::existCodeInList2(CString code)
{
	CString	chkcode, chkname;
	
	if(m_isDuplicateCode == true)	// 종목 중복등록 허용
	{
		
	}
	else
	{
		for (int ii = 0 ; ii < m_list2.GetItemCount() ; ii++)
		{
			chkcode = m_list2.GetItemText(ii, 0);
			chkname = m_list2.GetItemText(ii, 1);
			chkcode.TrimRight();
			chkcode.TrimLeft();
			
			if (code.Compare(chkcode) == 0)
			{
				m_errormsg.SetWindowText(chkname + " 종목이 관심그룹 내에 존재합니다. 중복해서 관심펀드를 추가할 수 없습니다.");
				return true;
			}
		}
	}
	
	return false;
}


void CDlg::GuideMsg(CString msg)
{
	Variant(guideCC, msg);
}

bool CDlg::GuideMsg(CString msg, bool question)
{
	UINT nType = MB_OK|MB_ICONEXCLAMATION;
	if (question)
		nType = MB_OKCANCEL|MB_ICONEXCLAMATION;
	
	if (MessageBox(msg, idTOstring(IDS_REGISTERINTER), nType) == IDCANCEL)
		return false;
	
	return true;
}

CString CDlg::idTOstring(UINT id)
{
	CString	msg;
	if (!msg.LoadString(id))
		msg = _T("");
	return msg;
}

BOOL CDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::OnNotify(wParam, lParam, pResult);
}

void CDlg::OnNewgroup() 
{
	CString	gname, gno;
	_grouplist* newGroupList{};
	int ii = 0;
	
	m_bIsEdit = TRUE;
	
	if (m_activegno != -1)
	{
		gname = GetGroupName(m_activegno);
		savingGroupFile(m_activegno, gname);
	}
	
	//int cnt = m_groupselect.GetCount();
	
	const int cnt = m_grouplist.GetSize();
		
	gname.Format("그룹%03d", cnt+1);
	CNewGroup newGroup(this, 0, gname);
	
	if (newGroup.DoModal() == IDOK)
	{
		m_newGroup = TRUE;

		std::vector<std::unique_ptr<_grouplist>> _vNewgrouplist;
		auto& _newGroup = _vNewgrouplist.emplace_back(std::make_unique<_grouplist>());
		newGroupList = _newGroup.get();
//		newGroupList = new _grouplist();
		gname = newGroup.m_defName;
		SendNewGroup(m_selectGroup);
		if (savingGroupOrder(gname))
		{
			OnAlldelete();
	
			SendCreateGroup(gname);
			
			savingGroupFile(m_activegno, gname);
		
			gno.Format("%04d", cnt);
			
			CopyMemory(newGroupList->gno, gno, strlen(gno));
			CopyMemory(newGroupList->gnm, gname, strlen(gname));

			m_Newgrouplist.Add(newGroupList);
			ii++;
			m_dlginit = FALSE;
		}
		m_createGrp = TRUE;
		QueryExcelSise();
	}
}

void CDlg::SendCreateGroup(CString gnm)
{
	CString strData;
	CString gname;
	struct _mid2 mid2;

	ZeroMemory(&mid2, sizeof(struct _mid2));

	//gname = GetGroupName(gno);

	CopyMemory(mid2.gubn, "i", sizeof(mid2.gubn));
	FillMemory(&mid2.grnm, sizeof(mid2.grnm), ' ');
	CopyMemory(mid2.grnm, gnm, strlen(gnm));

	strData = CString((char *)&mid2, sizeof(struct _mid2));
//	TRACE("Create group : %s\n", strData);

//	SendTR("pidomyfd", strData, FUNDCREATEGROUP, 0);
	SendTR("pidomyfd", strData, FUNDCREATEGROUP, 0);
}

void CDlg::SendNewGroup(int gno)
{
	CString strData;
	CString grcd;
	CString data;
	struct _mid1 mid1 {};
	_listitem* listitem{};

	int ncnt = m_list2.GetItemCount();
	const int combocnt = m_groupselect.GetCurSel();
	
	if((m_clickdel == TRUE && m_clickadd == FALSE) && ncnt > m_gdnt)			// 펀드 목록에서 삭제한 목록이 적을 경우 "d"로 보냄
	{
		ncnt = m_gdnt;
		ZeroMemory(&mid1, sizeof(struct _mid1));
		strData.Format("%03d", ncnt);
		grcd.Format("%04d", gno);

		CopyMemory(mid1.gubn, "D", 1);
		CopyMemory(mid1.nrec, strData, strlen(strData));
		CopyMemory(mid1.grcd, grcd, strlen(grcd));

		for(int ii = 0; ii < ncnt ; ii++)
		{
			//listitem = m_listitem2.GetAt(ii);
			FillMemory(&mid1.afcd[ii], sizeof(mid1.afcd[ii]), ' ');
			CopyMemory(mid1.afcd[ii], m_afcd[ii], strlen(mid1.afcd[ii]));
		}

		strData = CString((char *)&mid1, sizeof(struct _mid1));
		
 		SendTR("pidomyfd", strData, FUNDNEWGROUP, 0);
	}
	else if(m_clickadd == TRUE || m_clickdel == FALSE || ncnt < m_gdnt)
	{
		ZeroMemory(&mid1, sizeof(struct _mid1));
		strData.Format("%03d", ncnt);
		grcd.Format("%04d", gno);
		
		CopyMemory(mid1.gubn, "U", 1);
		CopyMemory(mid1.nrec, strData, strlen(strData));
		CopyMemory(mid1.grcd, grcd, strlen(grcd));
		
		for(int ii = 0; ii < m_list2.GetItemCount() ; ii++)
		{
			//listitem = m_listitem2.GetAt(ii);
			data = m_list2.GetItemText(ii, 0);		
			FillMemory(&mid1.afcd[ii], sizeof(mid1.afcd[ii]), ' ');
			CopyMemory(mid1.afcd[ii], data, min(sizeof(mid1.afcd[ii]), strlen(data)));

			//
			if (ii >= 99) 
				break;
		}
		strData = CString((char *)&mid1, sizeof(struct _mid1));
	
		SendTR("pidomyfd", strData, FUNDNEWGROUP, 0);
	}
}


CString	CDlg::GetGroupName(int gno)
{
	_grouplist* grouplist{};
	int ncnt = 0;
	CString groupNo;
	CString gnm;
	ncnt = m_grouplist.GetSize();
	
	for(int ii = 0; ii < ncnt ; ii++)
	{
		grouplist = m_grouplist.GetAt(ii);
		
		groupNo.Format("%04d", gno);

		if(strncmp(groupNo, grouplist->gno, 4) == 0)
		{
			break;
		}
	}
	
	return gnm;
}

void CDlg::savingGroupFile(int gno, CString gname)
{
	CString tempN; 
	tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);
	
	CString	section; 
	section.Format("%02d", gno);
	WritePrivateProfileString("GROUPNAME", section, gname, tempN);
	WritePrivateProfileString(NULL, NULL, NULL, tempN);
	
	savingInterest(gno);
}

void CDlg::savingInterest(int gno)
{
	
	CString	filePath, fileBook;
	filePath.Format("%s/%s/%s/FDportfolio.i%02d.tmp", m_root, USRDIR, m_name, gno);
	
	CString code, name;

	CFile	file;
	if (!file.Open(filePath, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
	{
		AfxMessageBox("파일을 생성할 수  없습니다.");
		return;
	}
	else
	{

//		CString strData(_T(""));
//		DWORD dwRead{};
//		int nLength = 0;
		char buff[256]{};

		const int nCount = m_list2.GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			code = m_list2.GetItemText(i, 0);
			name = m_list2.GetItemText(i, 1);

			char* nullcode = "            ";

			sprintf(buff, "%s\t%s\n", code.GetString(), (char*)name.GetString());
			file.Write(buff, sizeof(buff));
		}

		file.Close();
	}

/*
	FILE *fp{};

	if((fp = fopen(filePath, "w")) == NULL)
		AfxMessageBox("파일을 생성할 수  없습니다.");

	const int nCount = m_list2.GetItemCount();
	
	for(int i = 0 ; i < nCount; i++)
	{
		code = m_list2.GetItemText(i, 0);
 		name = m_list2.GetItemText(i, 1);

		char *nullcode = "            ";

		if (fp != NULL)
			fprintf(fp, "%s\t%s\n", code.GetString() , (char*)name.GetString());
	}

	if (fp != NULL)
		fclose(fp);
*/
}

bool CDlg::savingGroupOrder(CString gname)
{
	int	ttL = 0;
	char	ttB[1024];
	CString	strOrder = _T(""), string;
	
	CString filePath; 
	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);
	ttL = GetPrivateProfileString(_T("GROUPORDER"), "00", "", ttB, sizeof(ttB), filePath);
	if (ttL > 0) strOrder = CString(ttB, ttL);
	
	int ii = 0;
	for (ii = 1; !strOrder.IsEmpty() && ii < maxGROUP; ii++)
	{
		string.Format("%02d", ii);
		if (strOrder.Find(string) == -1)
			break;
	}
	if (ii >= maxGROUP)
		return false;
	
	if (!gname.IsEmpty())
	{
		const int idx = m_groupselect.AddString(gname);
		m_groupselect.SetItemData(idx, ii);
		
		string.Format("%02d;", ii); strOrder += string;
		WritePrivateProfileString(_T("GROUPORDER"), "00", strOrder, filePath);
		WritePrivateProfileString(NULL, NULL, NULL, filePath);
		
		m_groupselect.SetCurSel(idx); m_activegno = ii;
		return true;
	}
	
	return false;
}

void CDlg::OnGroupedit() 
{
	if (m_activegno != -1)
	{
		CString gname = GetGroupName(m_activegno);
		savingGroupFile(m_activegno, gname);
	}
	
	m_bIsEdit = TRUE;
	
	_grouplist* grouplist[100]{};
	
	CString data = "";
	CString gno[100], gnm[100];
	int grouplistsize = 0;
	
	grouplistsize = m_grouplist.GetSize();

	for(int ii =0; ii < m_grouplist.GetSize(); ii++)
	{
		grouplist[ii] = m_grouplist.GetAt(ii);
// 		gno[ii] = CString((char*)&grouplist->gno, sizeof(grouplist->gno));	
// 		gnm[ii] = CString((char*)&grouplist->gnm, sizeof(grouplist->gnm));
// 		data = data + gno[ii]+gnm[ii];
	}

	CEditGroup editGroup(this, m_main, m_root, m_name, grouplist, grouplistsize);
	//CString tcode;
	
	if (editGroup.DoModal() == IDOK)
	{
		CString	string, filePath;
		
		m_activegno = -1; 
	//initCombo();
//		QueryExcelSise();
		
		
		//int count = loadingInterest(m_activegno);
		
		m_list2.DeleteAllItems();
		m_listitem.RemoveAll();

// 		int nItem; _inters* pinter;
// 		for (int ii = 0; ii < count; ii++)
// 		{
// 			pinter = m_inters.GetAt(ii);
// 			
// 			// MODIFY PSH 20070917
// 			/*
// 			nItem = AddItem(&m_list2, pinter->code, ii);
// 			if (strlen(pinter->code))
// 			AddItem(&m_list2, pinter->name, nItem, 1);
// 			
// 			*/
// 			if (pinter->code[0] == 'm')
// 			{
// 				nItem = AddItem(&m_list2, "", ii);
// 				
// 				if (strlen(pinter->code))
// 					AddItem(&m_list2, pinter->name, nItem, 1);
// 			}
// 			else
// 			{
// 				tcode = pinter->code;
// 				tcode = tcode.Mid(0,12);
// 				tcode.TrimRight();
// 				nItem = AddItem(&m_list2, tcode, ii);
// 				if (strlen(pinter->code))
// 					AddItem(&m_list2, GetCodeName(tcode), nItem, 1);
// 			}
// 			
// 			_listitem* listitem = new _listitem();
// 			listitem->pInter = pinter;
// 
// 			strcpy(listitem->code, pinter->code);
// 			strcpy(listitem->name, pinter->name);
// 			
// 			m_list2item.Add(listitem);
// 			m_list2.SetItemData(nItem, (LPARAM) listitem);
// 		}
		//updateList2sum();
	}
}

void CDlg::ClearList2item()
{
	for (int ii = 0; ii < m_list2item.GetSize(); ii++)
		delete m_list2item.GetAt(ii);
	m_list2item.RemoveAll();
}


void CDlg::OnColumnclickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if (NULL != pNMListView)
	{
		switch(pNMListView->iSubItem) 
		{
		case 0:
			{
				if (0 != m_nPrevCol)
				{
					m_bCol0Asc = FALSE;
				}
				
				m_bCol0Asc = !m_bCol0Asc;
				m_listsort = !m_bCol0Asc;
				
				ListView_SortItems(m_list1, ListViewCompareFunc, 0);
				
				SetListColumnText(&m_list1, 0, m_bCol0Asc);
				SetListColumnText(&m_list1, 1, -1);
				SetListColumnText(&m_list1, 2, -1);
				SetListColumnText(&m_list1, 3, -1);
				SetListColumnText(&m_list1, 4, -1);
				SetListColumnText(&m_list1, 5, -1);
				SetListColumnText(&m_list1, 6, -1);
				SetListColumnText(&m_list1, 7, -1);
			}
			break;
		case 1:
			{
				if (1 != m_nPrevCol)
				{
					m_bCol1Asc = FALSE;
				}
				
				m_bCol1Asc = !m_bCol1Asc;
				m_listsort = !m_bCol1Asc;
				
				ListView_SortItems(m_list1, ListViewCompareFunc, 1);
				
				SetListColumnText(&m_list1, 1, m_bCol1Asc);
				SetListColumnText(&m_list1, 0, -1);
				SetListColumnText(&m_list1, 2, -1);
				SetListColumnText(&m_list1, 3, -1);
				SetListColumnText(&m_list1, 4, -1);
				SetListColumnText(&m_list1, 5, -1);
				SetListColumnText(&m_list1, 6, -1);
				SetListColumnText(&m_list1, 7, -1);
			}
			break;
		case 2:
			{
				if (2 != m_nPrevCol)
				{
					m_bCol2Asc = FALSE;
				}
				
				m_bCol2Asc = !m_bCol2Asc;
				m_listsort = !m_bCol2Asc;
				
				ListView_SortItems(m_list1, ListViewCompareFunc, 2);
				
				SetListColumnText(&m_list1, 2, m_bCol2Asc);
				SetListColumnText(&m_list1, 0, -1);
				SetListColumnText(&m_list1, 1, -1);
				SetListColumnText(&m_list1, 3, -1);
				SetListColumnText(&m_list1, 4, -1);
				SetListColumnText(&m_list1, 5, -1);
				SetListColumnText(&m_list1, 6, -1);
				SetListColumnText(&m_list1, 7, -1);
				
			}
			break;
		case 3:
			{
				if (3 != m_nPrevCol)
				{
					m_bCol3Asc = FALSE;
				}
				
				m_bCol3Asc = !m_bCol3Asc;
				m_listsort = !m_bCol3Asc;
				
				ListView_SortItems(m_list1, ListViewCompareFunc, 3);
				
				SetListColumnText(&m_list1, 3, m_bCol3Asc);
				SetListColumnText(&m_list1, 0, -1);
				SetListColumnText(&m_list1, 1, -1);
				SetListColumnText(&m_list1, 2, -1);
				SetListColumnText(&m_list1, 4, -1);
				SetListColumnText(&m_list1, 5, -1);
				SetListColumnText(&m_list1, 6, -1);
				SetListColumnText(&m_list1, 7, -1);
			}
			break;
		case 4:
			{
				if (4 != m_nPrevCol)
				{
					m_bCol4Asc = FALSE;
				}
				
				m_bCol4Asc = !m_bCol4Asc;
				m_listsort = !m_bCol4Asc;
				
				ListView_SortItems(m_list1, ListViewCompareFunc, 4);
				
				SetListColumnText(&m_list1, 4, m_bCol4Asc);
				SetListColumnText(&m_list1, 0, -1);
				SetListColumnText(&m_list1, 1, -1);
				SetListColumnText(&m_list1, 2, -1);
				SetListColumnText(&m_list1, 3, -1);
				SetListColumnText(&m_list1, 5, -1);
				SetListColumnText(&m_list1, 6, -1);
				SetListColumnText(&m_list1, 7, -1);
			}
			break;
		case 5:
			{
				if (5 != m_nPrevCol)
				{
					m_bCol5Asc = FALSE;
				}
				
				m_bCol5Asc = !m_bCol5Asc;
				m_listsort = !m_bCol5Asc;
				
				ListView_SortItems(m_list1, ListViewCompareFunc, 5);
				
				SetListColumnText(&m_list1, 5, m_bCol5Asc);
				SetListColumnText(&m_list1, 0, -1);
				SetListColumnText(&m_list1, 1, -1);
				SetListColumnText(&m_list1, 2, -1);
				SetListColumnText(&m_list1, 4, -1);
				SetListColumnText(&m_list1, 3, -1);
				SetListColumnText(&m_list1, 6, -1);
				SetListColumnText(&m_list1, 7, -1);
			}
			break;
		case 6:
			{
				if (6 != m_nPrevCol)
				{
					m_bCol6Asc = FALSE;
				}
				
				m_bCol6Asc = !m_bCol6Asc;
				m_listsort = !m_bCol6Asc;
				
				ListView_SortItems(m_list1, ListViewCompareFunc, 6);
				
				SetListColumnText(&m_list1, 6, m_bCol6Asc);
				SetListColumnText(&m_list1, 0, -1);
				SetListColumnText(&m_list1, 1, -1);
				SetListColumnText(&m_list1, 2, -1);
				SetListColumnText(&m_list1, 4, -1);
				SetListColumnText(&m_list1, 5, -1);
				SetListColumnText(&m_list1, 3, -1);
				SetListColumnText(&m_list1, 7, -1);
			}
			break;
		case 7:
			{
				if (7 != m_nPrevCol)
				{
					m_bCol7Asc = FALSE;
				}
				
				m_bCol7Asc = !m_bCol7Asc;
				m_listsort = !m_bCol7Asc;
				
				ListView_SortItems(m_list1, ListViewCompareFunc, 7);
				
				SetListColumnText(&m_list1, 7, m_bCol7Asc);
				SetListColumnText(&m_list1, 0, -1);
				SetListColumnText(&m_list1, 1, -1);
				SetListColumnText(&m_list1, 2, -1);
				SetListColumnText(&m_list1, 4, -1);
				SetListColumnText(&m_list1, 5, -1);
				SetListColumnText(&m_list1, 6, -1);
				SetListColumnText(&m_list1, 3, -1);
			}
			break;
		} 
		
		m_nPrevCol = pNMListView->iSubItem;
	}
	
	*pResult = 0;
}

void CDlg::OnColumnclickList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	if (NULL != pNMListView)
	{
		switch(pNMListView->iSubItem) 
		{
		case 0:
			{
				if (0 != m_nPrevCol)
				{
					m_bCol0Asc = FALSE;
				}
				
				m_bCol0Asc = !m_bCol0Asc;
				m_listsort = !m_bCol0Asc;
				
				ListView_SortItems(m_list2, ListViewCompareFunc, 0);
				
				SetListColumnText(&m_list2, 0, m_bCol0Asc);
				SetListColumnText(&m_list2, 1, -1);
				SetListColumnText(&m_list2, 2, -1);
				SetListColumnText(&m_list2, 3, -1);
				SetListColumnText(&m_list2, 4, -1);
				SetListColumnText(&m_list2, 5, -1);
				SetListColumnText(&m_list2, 6, -1);
				SetListColumnText(&m_list2, 7, -1);
			}
			break;
		case 1:
			{
				if (1 != m_nPrevCol)
				{
					m_bCol1Asc = FALSE;
				}
				
				m_bCol1Asc = !m_bCol1Asc;
				m_listsort = !m_bCol1Asc;
				
				ListView_SortItems(m_list2, ListViewCompareFunc, 1);
				
				SetListColumnText(&m_list2, 1, m_bCol1Asc);
				SetListColumnText(&m_list2, 0, -1);
				SetListColumnText(&m_list2, 2, -1);
				SetListColumnText(&m_list2, 3, -1);
				SetListColumnText(&m_list2, 4, -1);
				SetListColumnText(&m_list2, 5, -1);
				SetListColumnText(&m_list2, 6, -1);
				SetListColumnText(&m_list2, 7, -1);
			}
			break;
		case 2:
			{
				if (2 != m_nPrevCol)
				{
					m_bCol2Asc = FALSE;
				}
				
				m_bCol2Asc = !m_bCol2Asc;
				m_listsort = !m_bCol2Asc;
				
				ListView_SortItems(m_list2, ListViewCompareFunc, 2);
				
				SetListColumnText(&m_list2, 2, m_bCol2Asc);
				SetListColumnText(&m_list2, 0, -1);
				SetListColumnText(&m_list2, 1, -1);
				SetListColumnText(&m_list2, 3, -1);
				SetListColumnText(&m_list2, 4, -1);
				SetListColumnText(&m_list2, 5, -1);
				SetListColumnText(&m_list2, 6, -1);
				SetListColumnText(&m_list2, 7, -1);
			}
			break;
		case 3:
			{
				if (3 != m_nPrevCol)
				{
					m_bCol3Asc = FALSE;
				}
				
				m_bCol3Asc = !m_bCol3Asc;
				m_listsort = !m_bCol3Asc;
				
				ListView_SortItems(m_list2, ListViewCompareFunc, 3);
				
				SetListColumnText(&m_list2, 3, m_bCol3Asc);
				SetListColumnText(&m_list2, 0, -1);
				SetListColumnText(&m_list2, 1, -1);
				SetListColumnText(&m_list2, 2, -1);
				SetListColumnText(&m_list2, 4, -1);
				SetListColumnText(&m_list2, 5, -1);
				SetListColumnText(&m_list2, 6, -1);
				SetListColumnText(&m_list2, 7, -1);
			}
			break;
		case 4:
			{
				if (4 != m_nPrevCol)
				{
					m_bCol4Asc = FALSE;
				}
				
				m_bCol4Asc = !m_bCol4Asc;
				m_listsort = !m_bCol4Asc;
				
				ListView_SortItems(m_list2, ListViewCompareFunc, 4);
				
				SetListColumnText(&m_list2, 4, m_bCol4Asc);
				SetListColumnText(&m_list2, 0, -1);
				SetListColumnText(&m_list2, 1, -1);
				SetListColumnText(&m_list2, 2, -1);
				SetListColumnText(&m_list2, 3, -1);
				SetListColumnText(&m_list2, 5, -1);
				SetListColumnText(&m_list2, 6, -1);
				SetListColumnText(&m_list2, 7, -1);
			}
			break;
		case 5:
			{
				if (5 != m_nPrevCol)
				{
					m_bCol5Asc = FALSE;
				}
				
				m_bCol5Asc = !m_bCol5Asc;
				m_listsort = !m_bCol5Asc;
				
				ListView_SortItems(m_list2, ListViewCompareFunc, 5);
				
				SetListColumnText(&m_list2, 5, m_bCol5Asc);
				SetListColumnText(&m_list2, 0, -1);
				SetListColumnText(&m_list2, 1, -1);
				SetListColumnText(&m_list2, 2, -1);
				SetListColumnText(&m_list2, 4, -1);
				SetListColumnText(&m_list2, 3, -1);
				SetListColumnText(&m_list2, 6, -1);
				SetListColumnText(&m_list2, 7, -1);
			}
			break;
		case 6:
			{
				if (6 != m_nPrevCol)
				{
					m_bCol6Asc = FALSE;
				}
				
				m_bCol6Asc = !m_bCol6Asc;
				m_listsort = !m_bCol6Asc;
				
				ListView_SortItems(m_list2, ListViewCompareFunc, 6);
				
				SetListColumnText(&m_list2, 6, m_bCol6Asc);
				SetListColumnText(&m_list2, 0, -1);
				SetListColumnText(&m_list2, 1, -1);
				SetListColumnText(&m_list2, 2, -1);
				SetListColumnText(&m_list2, 4, -1);
				SetListColumnText(&m_list2, 5, -1);
				SetListColumnText(&m_list2, 3, -1);
				SetListColumnText(&m_list2, 7, -1);
			}
			break;
		case 7:
			{
				if (7 != m_nPrevCol)
				{
					m_bCol7Asc = FALSE;
				}
				
				m_bCol7Asc = !m_bCol7Asc;
				m_listsort = !m_bCol7Asc;
				
				ListView_SortItems(m_list2, ListViewCompareFunc, 7);
				
				SetListColumnText(&m_list2, 7, m_bCol7Asc);
				SetListColumnText(&m_list2, 0, -1);
				SetListColumnText(&m_list2, 1, -1);
				SetListColumnText(&m_list2, 2, -1);
				SetListColumnText(&m_list2, 4, -1);
				SetListColumnText(&m_list2, 5, -1);
				SetListColumnText(&m_list2, 6, -1);
				SetListColumnText(&m_list2, 3, -1);
			}
			break;
		} 
		
		m_nPrevCol = pNMListView->iSubItem;
	}
	
	m_up = TRUE;
	*pResult = 0;
}

void CDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect	list2RC, labelRC;
	
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
			
			//int pItem = m_list2.HitTest(&lvhti);
			
			m_list2.Invalidate();
			
			m_autoScroll = false;
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CDlg::SetAutoScroll(CListCtrl* listctrl, int direct)
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

void CDlg::OnOk() 
{
// //	OnKillActive();
	CString gname;

	if(m_activegno != -1)
	{
		gname = GetGroupName(m_activegno);
		savingGroupFile(m_activegno, gname);
	}
	
	if(m_clickadd == TRUE || m_clickdel == TRUE || m_up == TRUE || m_down == TRUE)
		SendNewGroup(m_selectGroup);
	
	m_confirm = 1;

	CDialog::OnOK();
}


void CDlg::OnSelchangeGroupselect() 
{
	CString	gname;
	int gno{};

	if (m_activegno != -1)
	{
		gname = GetGroupName(m_activegno);
		savingGroupFile(m_activegno, gname);	
	}
	
	TRACE("m_selectGroup: %d\n", m_selectGroup);
	SendNewGroup(m_selectGroup);
	
	m_list2.DeleteAllItems();
	//m_listitem.RemoveAll();
	//ClearList2item();
	
	gno = m_groupselect.GetCurSel();
	m_list2item.RemoveAll();
	m_activegno = m_groupselect.GetItemData(m_groupselect.GetCurSel());
	const int count = loadingInterest(m_activegno);
	const int nBreakCnt = 0;
	CString tcode;
	//int nItem;
	//_inters* pinter;
	//2017.01.18 KSJ count가 0이므로 여긴 안탄다..

/*
	for (int ii = 0; ii < count; ii++)
	{
		pinter = m_inters.GetAt(ii);
		
		// MODIFY PSH 20070917
		/ *
		nItem = AddItem(&m_list2, pinter->code, ii);
		if (strlen(pinter->code))
		AddItem(&m_list2, GetCodeName(pinter->code), nItem, 1);
		* /
		if (pinter->code[0] == 'm')
		{
			nItem = AddItem(&m_list2, "", ii);
			
			if (strlen(pinter->code))
				AddItem(&m_list2, pinter->name, nItem, 1);
		}
		/ *else if (0 == GetCodeType(pinter->code) && 0 != strlen(pinter->name))
		{
		nBreakCnt++;
		memset(pinter->code, 0x00, sizeof(pinter->code));
		memset(pinter->name, 0x00, sizeof(pinter->name));
	}* /
		else
		{
			tcode = pinter->code;
			tcode = tcode.Mid(0,12);
			tcode.TrimRight();
			nItem = AddItem(&m_list2, tcode, ii);
			if (strlen(pinter->code))
				AddItem(&m_list2, GetCodeName(tcode), nItem, 1);
		}
		
		_listitem* listitem = new _listitem();
		listitem->pInter = pinter;
		strcpy(listitem->code, tcode);
		strcpy(listitem->name, GetCodeName(tcode));
		
		m_list2item.Add(listitem);
		m_list2.SetItemData(nItem, (LPARAM) listitem);

		// END MODIFY
	}*/

	m_selectGroup = m_groupselect.GetCurSel();
	
}

void CDlg::searchAction(int column)
{
	CString	string; 
	m_search.GetWindowText(string);
	m_list1.UpdateData(false);
	if (column == 0)	// number search
	{
		if (!m_bAddAll)				// 검색시 모두 지웠을때
		{
			CString	code, name, rt06, rate, idtp, sdat, rknm, shgb;
			_listitem* listitem{};
			int	position = 0;
			m_list1.DeleteAllItems();
			
			for (int ii = 0; ii < m_searchlist2.GetSize(); ii++)
			{
				listitem = m_searchlist2.GetAt(ii);
				
				name = CString(listitem->name, sizeof(listitem->name));
				code = CString(listitem->code, sizeof(listitem->code));
				rt06 = CString(listitem->rt06, sizeof(listitem->rt06));
				rate = CString(listitem->rate, sizeof(listitem->rate));
				idtp = CString(listitem->idtp, sizeof(listitem->idtp));
				sdat = CString(listitem->sdat, sizeof(listitem->sdat));
				rknm = CString(listitem->rknm, sizeof(listitem->rknm));
				shgb = CString(listitem->shgb, sizeof(listitem->shgb));
				
				AppendItemToList1(position, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem);
				
				position += 1;
			}

			m_bAddAll = true;
		}
		
		if (m_list1.GetItemCount() > 0)
		{
			int	idx = -1;
			if (string.GetLength() > 0)
			{
				m_listsort = false;
				ListView_SortItems(m_list1, ListViewCompareFunc, 0);
				
				deselcMark(&m_list1);
				
				idx = findMatchIndex(string, column);
				if (idx >= 0) selcMark(&m_list1, idx);
			}
			else
			{
				deselcMark(&m_list1);
				selcMark(&m_list1, 0);
			}
		}
	}
	else			// name search
	{
		m_bAddAll = SearchData(0, string, m_bAddAll);
		if (m_list1.GetItemCount() > 0)
		{
			m_listsort = false;
			ListView_SortItems(m_list1, ListViewCompareFunc, 0);
			
			deselcMark(&m_list1);
			selcMark(&m_list1, 0);
		}
	}
	m_list1.UpdateData(true);
}

void CDlg::AppendItemToList1(int ipos, CString code, CString name,  _listitem *pListItem)
{
	int nItem = 0;
	gsl::owner<_listitem*> listitem{};
	
	name.TrimRight();
	
	nItem = AddItem(&m_list1, code, ipos);
	AddItem(&m_list1, name, nItem, 1);
	
	if (pListItem)
	{
		m_list1.SetItemData(nItem, (LPARAM) pListItem);
	}
	else	// 처음 종류를 선택할때만 추가된다.
	{
		[[gsl::suppress(26409)]]
		listitem = new _listitem();
		
		CopyMemory(listitem->code, code, strlen(code));
		CopyMemory(listitem->name, name, strlen(name));

		m_listitem.Add(listitem);
		m_list1.SetItemData(nItem, (LPARAM) listitem);
	}
}

void CDlg::AppendItemToList1(int ipos, CString code, CString name, CString rt06, CString rate, CString idtp, CString sdat, CString rknm, CString shgb, _listitem *pListItem, BOOL serarch)
{
	int nItem = 0;
	
	_listitem* listitem{};
	
	name.TrimRight();

	if(serarch == TRUE)
	{
		listitem = new _listitem();
		
		strcpy(listitem->code, code);
		strcpy(listitem->name, name);
		strcpy(listitem->rt06, rt06);
		strcpy(listitem->rate, rate);
		strcpy(listitem->idtp, idtp);
		strcpy(listitem->sdat, sdat);
		strcpy(listitem->rknm, rknm);
		strcpy(listitem->shgb, shgb);
		m_searchlist2.Add(listitem);

		if(m_bSearch)	return;	//2017.01.20 KSJ OnSearch에서 여기로 올경우는 그리드에 넣지 않아도됨.
	}
	
	nItem = AddItem(&m_list1, code, ipos);
	AddItem(&m_list1, name, nItem, 1);
	AddItem(&m_list1, rt06, nItem, 2);
	AddItem(&m_list1, rate, nItem, 3);
	AddItem(&m_list1, idtp, nItem, 4);
	AddItem(&m_list1, sdat, nItem, 5);
	AddItem(&m_list1, rknm, nItem, 6);
	AddItem(&m_list1, shgb, nItem, 7);
	
	if (pListItem)
	{
		m_list1.SetItemData(nItem, (LPARAM) pListItem);
	}
	else	// 처음 종류를 선택할때만 추가된다.
	{
		listitem = new _listitem();
		CopyMemory(listitem->code, code, strlen(code));
		CopyMemory(listitem->name, name, strlen(name));
		m_listitem.Add(listitem);
		m_list1.SetItemData(nItem, (LPARAM) listitem);
	}
}

int CDlg::findMatchIndex(CString text, int column)
{
	CString	string;
	const int	textL = text.GetLength();
	for (int ii = 0; ii < m_list1.GetItemCount(); ii++)
	{
		string = m_list1.GetItemText(ii, column);
		if (column == 1) string = string.Mid(1);
		
		if (!strncmp(text, string, textL))
			return ii;
	}
	return -1;
}

bool CDlg::SearchData(int kind, CString sName, bool bAddAll)
{
	bool result;
	result = SearchJongmok(sName, bAddAll);
	
	if(result == false)
		return SearchWord(sName, bAddAll);

	return result;
}

bool CDlg::SearchJongmok(CString sName, bool bAddAll)
{
	CString	code, name, rt06, rate, idtp, sdat, rknm, shgb;
	_listitem* listitem{};
	CArray <_listitem*, _listitem*> searchlist;
	searchlist.RemoveAll();
	int	position = 0;
	bool bsearch = false;

	editboxselect = TRUE;
	//int test = m_searchlist2.GetSize();

// 	if(comboselect == FALSE && editboxselect == TRUE)
// 	{
// 		searchlist.Copy(m_listitem);						// 아무것도 선택 안했을때는 전체 데이터를 이용
// 	}
// 	else
// 	{
		searchlist.Copy(m_searchlist2);						// 콤보 박스에서 조건 검색시 선택된 조건에서 검색
// 	}
	
	if (sName.IsEmpty())
	{
		if (!bAddAll)
		{
			m_list1.DeleteAllItems();
			for (int ii = 0; ii < searchlist.GetSize(); ii++)
			{
				listitem = searchlist.GetAt(ii);

				name = CString(listitem->name, sizeof(listitem->name));
				code = CString(listitem->code, sizeof(listitem->code));
				rt06 = CString(listitem->rt06, sizeof(listitem->rt06));
				rate = CString(listitem->rate, sizeof(listitem->rate));
				idtp = CString(listitem->idtp, sizeof(listitem->idtp));
				sdat = CString(listitem->sdat, sizeof(listitem->sdat));
				rknm = CString(listitem->rknm, sizeof(listitem->rknm));
				shgb = CString(listitem->shgb, sizeof(listitem->shgb));
				
				AppendItemToList1(position, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem);
				
				position += 1;
			}
		}
	
		return true;
	}

	m_list1.DeleteAllItems();

	char	cName{};
	const int	sLen = sName.GetLength();
	if (sLen == 1)
	{	// 일반검색적용(길이만큼 맞는것만 적용)
		const int	sLen = sName.GetLength();
		for (int ii = 0; ii < searchlist.GetSize(); ii++)
		{
			listitem = searchlist.GetAt(ii);

			name = CString(listitem->name, sizeof(listitem->name));
			if (strncmp(sName, name, sLen) != 0)
				continue;

			code = CString(listitem->code, sizeof(listitem->code));
			AppendItemToList1(position, code, name, listitem);
			position += 1;

			bsearch = true;
		}

		if(bsearch)	return true;
		else		return false;
	}
	else
	{	// 한글 검색
		WORD	wHangul{}, wHangul2{}, wStart{}, wEnd{};
		CPtrArray	arSearch;
		arSearch.RemoveAll();
		int ii = 0;

		cName = sName.GetAt(ii);
		if (cName & HANGULMSK)	// 한글일땐 2Byte
		{
			wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
			if (IsChosung(wHangul, wStart, wEnd))
			{	// 초성 비교
				for (int jj = 0; jj < searchlist.GetSize(); jj++)
				{
					listitem = searchlist.GetAt(jj);
					name = CString(listitem->name, sizeof(listitem->name));
					//앞공백제거
					name.TrimLeft();

					if (name.GetLength() < 2)	continue;
					wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
					if (wStart <= wHangul2 && wHangul2 <= wEnd)	
						arSearch.Add(listitem);
				}
			}
			else
			{	// 한글 비교

				for (int jj = 0; jj < searchlist.GetSize(); jj++)
				{
					listitem = searchlist.GetAt(jj);
					name = CString(listitem->name, sizeof(listitem->name));
					//앞공백제거
					name.TrimLeft();

					if (name.GetLength() < 2)	
						continue;
					wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
					if (wHangul == wHangul2)	
						arSearch.Add(listitem);
				}
			}
			ii += 2;
		}
		else			// 그외는 1Byte
		{
			for (int jj = 0; jj < searchlist.GetSize(); jj++)
			{
				listitem = searchlist.GetAt(jj);
				name = CString(listitem->name, sizeof(listitem->name));

				if (name.GetLength() < 1)	
					continue;
				if (cName == name.GetAt(ii))	
					arSearch.Add(listitem);
			}
			ii += 1;
		}

		for (; ii < sLen && arSearch.GetSize();)
		{
			cName = sName.GetAt(ii);
			if (cName & HANGULMSK)	// 한글일땐 2Byte
			{
				wHangul = MakeHangul(sName.GetAt(ii), sName.GetAt(ii+1));
				if (IsChosung(wHangul, wStart, wEnd))
				{	// 초성 비교
					for (int jj = arSearch.GetUpperBound(); jj >= 0; jj--)
					{
						listitem = (_listitem*)arSearch.GetAt(jj);
						name = CString(listitem->name, sizeof(listitem->name));
						//앞공백제거
						name.TrimLeft();

						if (name.GetLength() < ii+2)	
							arSearch.RemoveAt(jj);;
						wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
						if (wStart > wHangul2 || wHangul2 > wEnd)	
							arSearch.RemoveAt(jj);
					}
				}
				else
				{	// 한글 비교

					for (int jj = arSearch.GetUpperBound(); jj >= 0; jj--)
					{
						listitem = (_listitem*)arSearch.GetAt(jj);
						name = CString(listitem->name, sizeof(listitem->name));
						//앞공백제거
						name.TrimLeft();

						if (name.GetLength() < ii+2)	
							arSearch.RemoveAt(jj);
						wHangul2 = MakeHangul(name.GetAt(ii), name.GetAt(ii+1));
						if (wHangul != wHangul2)
							arSearch.RemoveAt(jj);
					}
				}
				ii += 2;
			}
			else			// 그외는 1Byte
			{
				for (int jj = arSearch.GetUpperBound(); jj >= 0; jj--)
				{
					listitem = (_listitem*)arSearch.GetAt(jj);
					name = CString(listitem->name, sizeof(listitem->name));

					if (name.GetLength() < ii+1)	
						arSearch.RemoveAt(jj);
					if (cName != name.GetAt(ii))	
						arSearch.RemoveAt(jj);
				}
				ii += 1;
			}
		}

		for (ii = 0; ii < arSearch.GetSize(); ii++)
		{
			listitem = (_listitem*)arSearch.GetAt(ii);

			name = CString(listitem->name, sizeof(listitem->name));
			code = CString(listitem->code, sizeof(listitem->code));
			rt06 = CString(listitem->rt06, sizeof(listitem->rt06));
			rate = CString(listitem->rate, sizeof(listitem->rate));
			idtp = CString(listitem->idtp, sizeof(listitem->idtp));
			sdat = CString(listitem->sdat, sizeof(listitem->sdat));
			rknm = CString(listitem->rknm, sizeof(listitem->rknm));
			shgb = CString(listitem->shgb, sizeof(listitem->shgb));

			AppendItemToList1(position, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem);
			position += 1;
			bsearch = true;
		}

		if(bsearch)	return true;
		else		return false;
	}

	return false;
}

bool CDlg::SearchWord(CString sName, bool bAddAll)
{
	CString	code, name, rt06, rate, idtp, sdat, rknm, shgb;
	_listitem* listitem{};
	CArray <_listitem*, _listitem*> searchlist;
	searchlist.RemoveAll();
	int	position = 0;
		
	editboxselect = TRUE;
	
// 	if(comboselect == FALSE && editboxselect == TRUE)
// 	{
// 		searchlist.Copy(m_listitem);						// 아무것도 선택 안했을때는 전체 데이터를 이용
// 	}
// 	else
// 	{
		searchlist.Copy(m_searchlist2);						// 콤보 박스에서 조건 검색시 선택된 조건에서 검색
// 	}
	
	if (sName.IsEmpty())
	{	// add all
		if (!bAddAll)
		{
			m_list1.DeleteAllItems();
			for (int ii = 0; ii < searchlist.GetSize(); ii++)
			{
				listitem = searchlist.GetAt(ii);
				
				name = CString(listitem->name, sizeof(listitem->name));
				code = CString(listitem->code, sizeof(listitem->code));
				rt06 = CString(listitem->rt06, sizeof(listitem->rt06));
				rate = CString(listitem->rate, sizeof(listitem->rate));
				idtp = CString(listitem->idtp, sizeof(listitem->idtp));
				sdat = CString(listitem->sdat, sizeof(listitem->sdat));
				rknm = CString(listitem->rknm, sizeof(listitem->rknm));
				shgb = CString(listitem->shgb, sizeof(listitem->shgb));
				
				AppendItemToList1(position, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem);
				
				position += 1;
			}
		}
		return true;
	}
	
	m_list1.DeleteAllItems();
	
	CPtrArray	arSearch;
	arSearch.RemoveAll();

	for (int ii = 0; ii < searchlist.GetSize(); ii++)
	{
		listitem = (_listitem*)searchlist.GetAt(ii);
		name = CString(listitem->name, sizeof(listitem->name));
		//if (hjc->ssgb != jmELW)		
		//	name = name.Mid(1);
		name.MakeUpper();
		sName.MakeUpper();
		if (name.Find(sName) >= 0)	
			arSearch.Add(listitem);
	}
	
	for (int ii = 0; ii < arSearch.GetSize(); ii++)
	{
		listitem = (_listitem*)arSearch.GetAt(ii);
		
		//name = CString(listitem->name, sizeof(listitem->name));
		//if (hjc->ssgb != jmELW)	
		//name = name.Mid(1);
		
		name = CString(listitem->name, sizeof(listitem->name));
		code = CString(listitem->code, sizeof(listitem->code));
		rt06 = CString(listitem->rt06, sizeof(listitem->rt06));
		rate = CString(listitem->rate, sizeof(listitem->rate));
		idtp = CString(listitem->idtp, sizeof(listitem->idtp));
		sdat = CString(listitem->sdat, sizeof(listitem->sdat));
		rknm = CString(listitem->rknm, sizeof(listitem->rknm));
		shgb = CString(listitem->shgb, sizeof(listitem->shgb));

		AppendItemToList1(position, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem);
		
		position += 1;
	}
	
	return false;
}

WORD CDlg::MakeHangul(char cChar1, char cChar2)
{	// 2Byte문자로부터 한글을 맹글어주라~~
	const WORD	wHangul = (cChar1<<8) | (0x00FF & cChar2);
	return wHangul;
}

bool CDlg::IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd)
{	// 초성만 가진 한글인지~~
	if (ChosungS > wHangul || wHangul > ChosungE)
		return false;
	
	const WORD	pChosung[19] = 
	{
		0xA4A1, 0xA4A2, 0xA4A4, 0xA4A7, 0xA4A8, 0xA4A9, 0xA4B1,	// ㄱ,ㄲ,ㄴ,ㄷ,ㄸ,ㄹ,ㅁ
			0xA4B2, 0xA4B3, 0xA4B5, 0xA4B6, 0xA4B7, 0xA4B8, 0xA4B9,	// ㅂ,ㅃ,ㅅ,ㅆ,ㅇ,ㅈ,ㅉ
			0xA4BA, 0xA4BB, 0xA4BC, 0xA4BD, 0xA4BE			// ㅊ,ㅋ,ㅌ,ㅍ,ㅎ
	};
	
	const WORD	pHangulStart[19] = 
	{
		0xB0A1, 0xB1EE, 0xB3AA, 0xB4D9, 0xB5FB, 0xB6F3, 0xB8B6,	// 가,까,나,다,따,라,마
			0xB9D9, 0xBAFC, 0xBBE7, 0xBDCE, 0xBEC6, 0xC0DA, 0xC2A5,	// 바,빠,사,싸,아,자,짜
			0xC2F7, 0xC4AB, 0xC5B8, 0xC6C4, 0xC7CF			// 차,카,타,파,하
	};
	const WORD	pHangulEnd[19] = 
	{
		0xB1ED, 0xB3A9, 0xB4D8, 0xB5FA, 0xB6F2, 0xB8B5,		
			0xB9D8, 0xBAFB, 0xBBE6, 0xBDCD, 0xBEC5, 0xC0D9, 0xC2A4,	
			0xC2F6, 0xC4AA, 0xC5B7, 0xC6C3, 0xC7CE, 0xC8FE			
	};
	
	int ii = 0;
	for (ii = 0; ii < 19; ii++)
	{
		if (wHangul == pChosung[ii])
		{
			wStart = pHangulStart[ii];
			wEnd = pHangulEnd[ii];
			return true;
		}			
	}
	
	wStart = pHangulStart[ii] - 1;
	wEnd = pHangulEnd[ii] + 1;
	
	return true;
}

void CDlg::OnSelchangeFdtype()					// 간접투자 대상 콤보 변경 시
{
	CString fdtypename, gubnname, ivgradename;
	CString _rt06, _rate;
	int iState{}, iStaterate{};
	double rt06{}, rate{};
	
	iState = m_rt06check.GetCheck();
	iStaterate = m_ratecheck.GetCheck();
	
	m_fdtype.GetLBText(m_fdtype.GetCurSel(), fdtypename);
	m_gubn.GetLBText(m_gubn.GetCurSel(), gubnname);
	m_ivgrade.GetLBText(m_ivgrade.GetCurSel(), ivgradename);
	
	if(iState == BST_CHECKED && iStaterate == BST_CHECKED)
	{
		m_setrt06.GetWindowText(_rt06);
		m_setrate.GetWindowText(_rate);
		rt06 = atof(_rt06);
		rate = atof(_rate);
		
		LoadList1(fdtypename, gubnname, ivgradename, (float)rt06, (float)rate);
	}
	else if(iState == BST_CHECKED)
	{
		m_setrt06.GetWindowText(_rt06);
		rt06 = atof(_rt06);
		
		LoadList1(fdtypename, gubnname, ivgradename, (float)rt06);
	}
	else if(iStaterate == BST_CHECKED)
	{
		m_setrate.GetWindowText(_rate);
		rate = atof(_rate);
		
		LoadList1(fdtypename, gubnname, ivgradename, (float)rt06, (float)rate);
	}
	else
	{
		LoadList1(fdtypename, gubnname, ivgradename);
	}

	//2017.01.18 KSJ 전부 넣고나서 소팅
	ListView_SortItems(m_list1, ListViewCompareFunc, 0);
}

void CDlg::LoadList1(CString fdtype, CString gubn, CString ivgrade, float _rt06, float _rate)			// 콤보박스 조건 검색
{
	m_list1.DeleteAllItems();
	_listitem* listitem{};
	CString code, name, rt06, rate, idtp, sdat, rknm, shgb;
	double rate06{}, accumrate{};
	int iState{}, iStaterate{};

	comboselect = TRUE;

	iState = m_rt06check.GetCheck();
	iStaterate = m_ratecheck.GetCheck();

	if(comboselect == TRUE && editboxselect == TRUE)
	{
		//m_search.SetWindowText("");
		m_searchlist2.RemoveAll();
		m_searchlist.Copy(m_listitem);
	}
	else
	{
		m_searchlist2.RemoveAll();
		m_searchlist.Copy(m_listitem);
	}

	//int test = m_searchlist.GetSize();
	if(strcmp(fdtype, "전체") == 0 && strcmp(gubn, "전체") == 0 && strcmp(ivgrade, "전체")==0)
	{
		for(int ii =0; ii < m_searchlist.GetSize(); ii++)
		{
			listitem = m_searchlist.GetAt(ii);
			
			name = CString(listitem->name, sizeof(listitem->name));
			code = CString(listitem->code, sizeof(listitem->code));
			rt06 = CString(listitem->rt06, sizeof(listitem->rt06));
			rate = CString(listitem->rate, sizeof(listitem->rate));
			idtp = CString(listitem->idtp, sizeof(listitem->idtp));
			sdat = CString(listitem->sdat, sizeof(listitem->sdat));
			rknm = CString(listitem->rknm, sizeof(listitem->rknm));
			shgb = CString(listitem->shgb, sizeof(listitem->shgb));
			
			rate06 = atof(rt06);
			accumrate = atof(rate);

			if((iState == BST_UNCHECKED ||rate06 >= _rt06) && (iStaterate == BST_UNCHECKED || accumrate >= _rate))
				AppendItemToList1(0, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem, TRUE);
		}
	}
	else if(strcmp(ivgrade, "전체") == 0 && strcmp(gubn, "전체") == 0)
	{
		for(int ii =0; ii < m_searchlist.GetSize(); ii++)
		{
			listitem = m_searchlist.GetAt(ii);
			
			name = CString(listitem->name, sizeof(listitem->name));
			code = CString(listitem->code, sizeof(listitem->code));
			rt06 = CString(listitem->rt06, sizeof(listitem->rt06));
			rate = CString(listitem->rate, sizeof(listitem->rate));
			idtp = CString(listitem->idtp, sizeof(listitem->idtp));
			sdat = CString(listitem->sdat, sizeof(listitem->sdat));
			rknm = CString(listitem->rknm, sizeof(listitem->rknm));
			shgb = CString(listitem->shgb, sizeof(listitem->shgb));
			
			rate06 = atof(rt06);
			accumrate = atof(rate);

			if(strcmp(fdtype, idtp) == 0 && (iState == BST_UNCHECKED ||rate06 >= _rt06) && (iStaterate == BST_UNCHECKED || accumrate >= _rate))
			{
				AppendItemToList1(0, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem, TRUE);
			}
		}
	}
	else if(strcmp(fdtype, "전체") == 0 && strcmp(ivgrade, "전체") == 0)
	{
		for(int ii =0; ii < m_searchlist.GetSize(); ii++)
		{
			listitem = m_searchlist.GetAt(ii);
			
			name = CString(listitem->name, sizeof(listitem->name));
			code = CString(listitem->code, sizeof(listitem->code));
			rt06 = CString(listitem->rt06, sizeof(listitem->rt06));
			rate = CString(listitem->rate, sizeof(listitem->rate));
			idtp = CString(listitem->idtp, sizeof(listitem->idtp));
			sdat = CString(listitem->sdat, sizeof(listitem->sdat));
			rknm = CString(listitem->rknm, sizeof(listitem->rknm));
			shgb = CString(listitem->shgb, sizeof(listitem->shgb));
			
			rate06 = atof(rt06);
			accumrate = atof(rate);

			if(strcmp(gubn, shgb) == 0 && (iState == BST_UNCHECKED ||rate06 >= _rt06) && (iStaterate == BST_UNCHECKED || accumrate >= _rate))
			{
				AppendItemToList1(0, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem, TRUE);
			}
		}
	}
	else if(strcmp(fdtype, "전체") == 0 && strcmp(gubn, "전체") == 0)
	{
		for(int ii =0; ii < m_searchlist.GetSize(); ii++)
		{
			listitem = m_searchlist.GetAt(ii);
			
			name = CString(listitem->name, sizeof(listitem->name));
			code = CString(listitem->code, sizeof(listitem->code));
			rt06 = CString(listitem->rt06, sizeof(listitem->rt06));
			rate = CString(listitem->rate, sizeof(listitem->rate));
			idtp = CString(listitem->idtp, sizeof(listitem->idtp));
			sdat = CString(listitem->sdat, sizeof(listitem->sdat));
			rknm = CString(listitem->rknm, sizeof(listitem->rknm));
			shgb = CString(listitem->shgb, sizeof(listitem->shgb));
			
			rate06 = atof(rt06);
			accumrate = atof(rate);	

			if(strcmp(ivgrade, rknm) == 0 && (iState == BST_UNCHECKED ||rate06 >= _rt06) && (iStaterate == BST_UNCHECKED || accumrate >= _rate))
			{
				AppendItemToList1(0, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem, TRUE);
			}
		}
	}
	else if(strcmp(fdtype, "전체") == 0)
	{
		for(int ii =0; ii < m_searchlist.GetSize(); ii++)
		{
			listitem = m_searchlist.GetAt(ii);

			name = CString(listitem->name, sizeof(listitem->name));
			code = CString(listitem->code, sizeof(listitem->code));
			rt06 = CString(listitem->rt06, sizeof(listitem->rt06));
			rate = CString(listitem->rate, sizeof(listitem->rate));
			idtp = CString(listitem->idtp, sizeof(listitem->idtp));
			sdat = CString(listitem->sdat, sizeof(listitem->sdat));
			rknm = CString(listitem->rknm, sizeof(listitem->rknm));
			shgb = CString(listitem->shgb, sizeof(listitem->shgb));
			
			rate06 = atof(rt06);
			accumrate = atof(rate);

			if(strcmp(gubn, shgb) == 0 && strcmp(ivgrade, rknm) == 0 && (iState == BST_UNCHECKED ||rate06 >= _rt06) && (iStaterate == BST_UNCHECKED || accumrate >= _rate))
			{
				AppendItemToList1(0, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem, TRUE);
			}
		}
	}
	else if(strcmp(gubn, "전체") == 0)
	{
		for(int ii =0; ii < m_searchlist.GetSize(); ii++)
		{
			listitem = m_searchlist.GetAt(ii);
			
			name = CString(listitem->name, sizeof(listitem->name));
			code = CString(listitem->code, sizeof(listitem->code));
			rt06 = CString(listitem->rt06, sizeof(listitem->rt06));
			rate = CString(listitem->rate, sizeof(listitem->rate));
			idtp = CString(listitem->idtp, sizeof(listitem->idtp));
			sdat = CString(listitem->sdat, sizeof(listitem->sdat));
			rknm = CString(listitem->rknm, sizeof(listitem->rknm));
			shgb = CString(listitem->shgb, sizeof(listitem->shgb));
			
			rate06 = atof(rt06);
			accumrate = atof(rate);

			if(strcmp(fdtype, idtp) == 0 && strcmp(ivgrade, rknm) == 0 && (iState == BST_UNCHECKED ||rate06 >= _rt06) && (iStaterate == BST_UNCHECKED || accumrate >= _rate))
			{
				AppendItemToList1(0, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem, TRUE);
			}
		}
	}
	else if(strcmp(ivgrade, "전체") == 0)
	{
		for(int ii =0; ii < m_searchlist.GetSize(); ii++)
		{
			listitem = m_searchlist.GetAt(ii);
			
			name = CString(listitem->name, sizeof(listitem->name));
			code = CString(listitem->code, sizeof(listitem->code));
			rt06 = CString(listitem->rt06, sizeof(listitem->rt06));
			rate = CString(listitem->rate, sizeof(listitem->rate));
			idtp = CString(listitem->idtp, sizeof(listitem->idtp));
			sdat = CString(listitem->sdat, sizeof(listitem->sdat));
			rknm = CString(listitem->rknm, sizeof(listitem->rknm));
			shgb = CString(listitem->shgb, sizeof(listitem->shgb));
			
			rate06 = atof(rt06);
			accumrate = atof(rate);

			if(strcmp(fdtype, idtp) == 0 && strcmp(gubn, shgb) == 0 && (iState == BST_UNCHECKED ||rate06 >= _rt06) && (iStaterate == BST_UNCHECKED || accumrate >= _rate))
			{
				AppendItemToList1(0, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem, TRUE);
			}
		}
	}
	else
	{
		for(int ii =0; ii < m_searchlist.GetSize(); ii++)
		{
			listitem = m_searchlist.GetAt(ii);
			
			name = CString(listitem->name, sizeof(listitem->name));
			code = CString(listitem->code, sizeof(listitem->code));
			rt06 = CString(listitem->rt06, sizeof(listitem->rt06));
			rate = CString(listitem->rate, sizeof(listitem->rate));
			idtp = CString(listitem->idtp, sizeof(listitem->idtp));
			sdat = CString(listitem->sdat, sizeof(listitem->sdat));
			rknm = CString(listitem->rknm, sizeof(listitem->rknm));
			shgb = CString(listitem->shgb, sizeof(listitem->shgb));
			
			if(strcmp(fdtype, idtp) == 0 && strcmp(gubn, shgb) == 0 && strcmp(ivgrade, rknm) == 0 && (iState == BST_UNCHECKED ||rate06 >= _rt06) && (iStaterate == BST_UNCHECKED || accumrate >= _rate))
			{
				AppendItemToList1(0, code, name, rt06, rate, idtp, sdat, rknm, shgb, listitem, TRUE);
			}
		}
	}
}

CString	CDlg::getFDtypeName(int num)
{
	CString name;
	
	switch (num)
	{
	case 0:
		name = "전체";
		break;
	case 1:
		name = "MMF";
		break;
	case 2:
		name = "부동산간접";
		break;
	case 3:
		name = "재간접";
		break;
	case 4:
		name = "주식형";
		break;
	case 5:
		name = "주식혼합형";
		break;
	case 6:
		name = "채권형";
		break;
	case 7:
		name = "채권혼합형";
		break;
	case 8:
		name = "특별간접";
		break;
	case 9:
		name = "파생상품간접";
		break;
	}

	return name;	
}

CString	CDlg::getGubnName(int num)
{
	CString name;

	switch(num)
	{
	case 0:
		name = "전체";
		break;
	case 1:
		name = "인터넷거래가능";
		break;
	case 2:
		name = "인터넷거래불가능";
		break;
	}

	return name;
}

CString	CDlg::getIVGradeName(int num)
{
	CString name;
	
	switch (num)
	{
	case 0:
		name = "전체";
		break;
	case 1:
		name = "공격투자형";
		break;
	case 2:
		name = "안정추구형";
		break;
	case 3:
		name = "안정형";
		break;
	case 4:
		name = "위험중립형";
		break;
	case 5:
		name = "적극투자형";
		break;
	}
	
	return name;	
}

void CDlg::OnSelchangeGubn() 
{
	m_bSearch = false;
	OnSelchangeFdtype();
}

void CDlg::OnSelchangeIvgrade() 
{
	m_bSearch = false;
	OnSelchangeFdtype();
}

void CDlg::OnCancel() 
{
// 	int ncnt = 0;
// 	_grouplist *grouplist;
// 	int gno=0;
// 
// 	ncnt = m_Newgrouplist.GetSize();
// 	
// 	for(int ii=0; ii < ncnt; ii++)
// 	{
// 		grouplist = m_Newgrouplist.GetAt(ii);
// 		gno = atoi(grouplist->gno);
// 		SendDelNewGroup(gno);
// 	}
	QueryRestore();
	
	m_confirm = 2;
	//OnDestroy();

//	DestroyWindow();
	CDialog::OnCancel();
}

void CDlg::ParsingCreateNGroup(char* datB, int datL)
{
	char *buf = datB;
	struct _mod2 *mod2;
	mod2 = (struct _mod2*)buf;
	CString rest, ercd, nrce;

	rest.Format("%.*s", sizeof(mod2->rest), mod2->rest);
	
	ercd.Format("%.*s", sizeof(mod2->ercd), mod2->ercd);

	nrce.Format("%.*s", sizeof(mod2->nrec), mod2->nrec);

	QueryExcelSise();
}

void CDlg::SendInterestData(int gno)
{
	CString strData, grcd;
	struct __mid2 mid2;
	ZeroMemory(&mid2, sizeof(struct __mid2));
	FillMemory(&mid2, sizeof(struct __mid2), ' ');
	
	grcd.Format("%04d", gno);
	
	CopyMemory(mid2.gubn, "B", 1);
	CopyMemory(mid2.grcd, grcd, sizeof(grcd));
	
	strData = CString((char *)&mid2, sizeof(struct __mid2));
//	TRACE("Main data : %s\n", strData);
	//	SendTR("pihomyfd", strData, FUNDINIT, 0);
	SendTR("pihomyfd", (char *)&mid2, FUNDLIST, 0);
}

void CDlg::ParsingGroupList(char* datB, int datL)
{
	struct item2
	{
		char fdcd[12];			/*펀드코드*/
		char fdnm[40];			/*펀드명*/
		char rt06[9];			/*6개월수익률*/			
		char rate[9];			/*기간수익률*/
		char idnm[40];			/*간접투자유형*/
		char stdt[8];			/*설정일*/
		char ivnm[40];			/*투자등급명*/
		char scgm[20];			/*조회구분명*/
	};

	struct mod2
	{
		char nrce[4];			/*펀드 갯수*/
		struct item2 list[MYFD_MAXCNT];
	}*mod2{};

	
	char *buf = datB;
	CString nrce;
	CString fdcd, fdnm, rt06, rate, idnm, stdt, ivnm, scgm;
	int combocnt = m_groupselect.GetCurSel();
	int ncnt{};
	m_list2.DeleteAllItems();
	m_list2item.RemoveAll();
	m_listitem2.RemoveAll();
	mod2 = (struct mod2*)buf;
	
	nrce.Format("%.*s", sizeof(mod2->nrce), mod2->nrce);
	ncnt = atoi(nrce);
	m_gcnt[combocnt] = ncnt;

	for(int ii = 0; ii < ncnt; ii++)
	{
		//fdcd.Format("%.*s", sizeof(mod2->list[ii].fdcd), mod2->list[ii].fdcd);
		fdcd = CString(mod2->list[ii].fdcd, sizeof(mod2->list[ii].fdcd));
		fdcd.TrimRight();
				
		fdnm = CString(mod2->list[ii].fdnm, sizeof(mod2->list[ii].fdnm));
		fdnm.TrimRight();
		
		rt06 = CString(mod2->list[ii].rt06, sizeof(mod2->list[ii].rt06));
		rt06.TrimLeft();

		rate = CString(mod2->list[ii].rate, sizeof(mod2->list[ii].rate));
		rate.TrimLeft();

		idnm = CString(mod2->list[ii].idnm, sizeof(mod2->list[ii].idnm));
		idnm.TrimRight();

		stdt = CString(mod2->list[ii].stdt, sizeof(mod2->list[ii].stdt));
		stdt.Insert(4, "/");
		stdt.Insert(7, "/");
		
		ivnm = CString(mod2->list[ii].ivnm, sizeof(mod2->list[ii].ivnm));
		ivnm.TrimRight();

		scgm = CString(mod2->list[ii].scgm, sizeof(mod2->list[ii].scgm));
		scgm.TrimRight();

		AppendItemToList2(ii, fdcd, fdnm, rt06, rate, idnm, stdt, ivnm, scgm);
	}	
	
//	UpdateData();
}


void CDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
//  	DeleteListItem();
//  	DeleteList2Item();	
// 	DeleteGroupList();
//  
	if (m_dragImage) 
		delete m_dragImage;

	m_inters.RemoveAll();
	m_list2item.RemoveAll();
	m_listitem.RemoveAll();
	m_listitem2.RemoveAll();
	m_grouplist.RemoveAll();
	m_Newgrouplist.RemoveAll();
	m_searchlist.RemoveAll();
	m_searchlist2.RemoveAll();

	if(m_confirm == 0)
		QueryRestore();

	const int lastpage = m_groupselect.GetCurSel();
	m_main->SendMessage(WM_MESSAGE, DLGEXIT, lastpage);
}

void CDlg::DeleteGroupList()
{
	for(int ii = 0; ii < m_grouplist.GetSize(); ii++)
	{
		delete m_grouplist.GetAt(ii);
		m_grouplist.RemoveAt(ii);
	}
}

void CDlg::DeleteList2Item()
{
	for(int ii = 0; ii < m_list2item.GetSize(); ii++)
	{
		delete m_list2item.GetAt(ii);
		m_list2item.RemoveAt(ii);
	}
}

void CDlg::DeleteListItem()
{
	for(int ii = 0; ii < m_listitem.GetSize(); ii++)
	{
		delete m_listitem.GetAt(ii);
		m_listitem.RemoveAt(ii);
	}
}

void CDlg::SendDelNewGroup(int gno)
{
	CString strData;
	CString gname;
	struct _mid4 mid4 {};
	CString nrce;
	CString groupno;
	const int ncnt = 1;
	

	ZeroMemory(&mid4, sizeof(struct _mid4));

	groupno.Format("%04d", gno);

	CopyMemory(mid4.gubn, "d", sizeof(mid4.gubn));
	
	CopyMemory(mid4.agcd, groupno, sizeof(mid4.agcd));
	
	nrce.Format("%03d", ncnt);
	CopyMemory(mid4.nrce, nrce, sizeof(mid4.nrce));

	strData = CString((char *)&mid4, sizeof(struct _mid4));
//	TRACE("Create group : %s\n", strData);
	
	//	SendTR("pidomyfd", strData, FUNDCREATEGROUP, 0);
	SendTR("pidomyfd", strData, FUNDDELETEGROUP, 0);
}

void CDlg::ParsingDeleteGroup(char* datB, int datL)
{
	struct _mod2* mod2{};

	char *buf = datB;
	
	CString ngrd, ogrd, grnm;
	CString rest, ercd, nrec;
	
	mod2 = (struct _mod2*)buf;
	
// 	nrce.Format("%.*s", sizeof(mod2->nrce), mod2->nrce);
// 	ncnt = atoi(nrce);
// 	m_gcnt[combocnt] = ncnt;
// 	
// 	for(int ii = 0; ii < ncnt; ii++)
// 	{
// 		fdcd.Format("%.*s", sizeof(mod2->list[ii].fdcd), mod2->list[ii].fdcd);
// 		fdcd.TrimRight();
// 		
// 		fdnm.Format("%.*s", sizeof(mod2->list[ii].fdnm), mod2->list[ii].fdnm);
// 		fdnm.TrimRight();
// 		
// 		AppendItemToList2(ii, fdcd, fdnm);
// 	}	
}

void CDlg::ParsingBackup(char* datB, int datL)
{
	struct _modB3* mod3{};
	
	char *buf = datB;
	mod3 = (struct _modB3*)buf;

	CString rest(mod3->rest, sizeof(mod3->rest));
	CString ercd(mod3->ercd, sizeof(mod3->ercd));

	
// 	rest.Format("%.s", sizeof(mod3->rest), mod3->rest);
// 	ercd.Format("%.s", sizeof(mod3->ercd), mod3->ercd);

	if(strcmp(rest, "N") == 0)
		m_errormsg.SetWindowText("데이터가 백업되지 않았습니다." + ercd);
}

void CDlg::ParsingRestore(char* datB, int datL)
{
	struct _modB3* mod3{};
	
	char *buf = datB;
	mod3 = (struct _modB3*)buf;
	
	CString rest(mod3->rest, sizeof(mod3->rest));
	CString ercd(mod3->ercd, sizeof(mod3->ercd));
	
// 	rest.Format("%.s", sizeof(mod3->rest), mod3->rest);
// 	ercd.Format("%.s", sizeof(mod3->ercd), mod3->ercd);
	
	if(strcmp(rest, "N") == 0)
		AfxMessageBox("데이터가 복구되지 않았습니다." + ercd);
	
	//DestroyWindow();
}

void CDlg::OnSearch() 
{
	CString string;
	m_search.GetWindowText(string);
	CString temp = string.GetAt(0);

	//2017.01.20 KSJ 검색창이 빈값이면 전체검색
	m_bSearch = true;

	if(string.GetLength() == 0)	m_bSearch = false;

	OnSelchangeFdtype();
	
	if(atoi(temp) >= 5 && atoi(temp) < 9)
	{
		searchAction(0);	// char search by kwon
	}
	else if(!string.IsEmpty())
	{
		searchAction(1);	// char search by kwon
	}
}

void CDlg::OnCustomDrawList ( NMHDR* pNMHDR, LRESULT* pResult )					// 리스트 데이터 글자색 변경
{
	//This code based on Michael Dunn's excellent article on
	//list control custom draw at http://www.codeproject.com/listctrl/lvcustomdraw.asp
	
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
    // Take the default processing unless we set this to something else below.
    *pResult = CDRF_DODEFAULT;
	
    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        // This is the notification message for an item.  We'll request
        // notifications before each subitem's prepaint stage.
		
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		COLORREF clrNewTextColor{};
        
		const int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
		
		CString strTemp = m_list1.GetItemText(nItem,pLVCD->iSubItem);
		double rt_06{}, rate{};
		
		switch(pLVCD->iSubItem)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			rt_06 = atof(strTemp);
			if(rt_06 > 0)
				clrNewTextColor = RGB(255, 0, 0);
			else if(rt_06 <0)
				clrNewTextColor = RGB(0, 0, 255);
			else
				clrNewTextColor = RGB(0, 0, 0);
			pLVCD->clrText = clrNewTextColor;
			break;
		case 3:
			rate = atof(strTemp);
			if(rate > 0)
				clrNewTextColor = RGB(255, 0, 0);
			else if(rate <0)
				clrNewTextColor = RGB(0, 0, 255);
			else
				clrNewTextColor = RGB(0, 0, 0);
			
			pLVCD->clrText = clrNewTextColor;
			break;
		case 4:
			clrNewTextColor = RGB(0, 0, 0);
			pLVCD->clrText = clrNewTextColor;
			break;
		}
        *pResult = CDRF_DODEFAULT;        
	}
} 

void CDlg::OnCustomDrawList2( NMHDR* pNMHDR, LRESULT* pResult )				
{
	//This code based on Michael Dunn's excellent article on
	//list control custom draw at http://www.codeproject.com/listctrl/lvcustomdraw.asp
	
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
    // Take the default processing unless we set this to something else below.
    *pResult = CDRF_DODEFAULT;
	
    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
        // This is the notification message for an item.  We'll request
        // notifications before each subitem's prepaint stage.
		
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
    else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		COLORREF clrNewTextColor{};
        
		const int nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec ); //row
		
		CString strTemp = m_list2.GetItemText(nItem,pLVCD->iSubItem);
		double rt_06{}, rate{};

		switch(pLVCD->iSubItem)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			rt_06 = atof(strTemp);
			if(rt_06 > 0)
				clrNewTextColor = RGB(255, 0, 0);
			else if(rt_06 <0)
				clrNewTextColor = RGB(0, 0, 255);
			else
				clrNewTextColor = RGB(0, 0, 0);
			pLVCD->clrText = clrNewTextColor;
			break;
		case 3:
			rate = atof(strTemp);
			if(rate > 0)
				clrNewTextColor = RGB(255, 0, 0);
			else if(rate <0)
				clrNewTextColor = RGB(0, 0, 255);
			else
				clrNewTextColor = RGB(0, 0, 0);
			
			pLVCD->clrText = clrNewTextColor;
			break;
		case 4:
			clrNewTextColor = RGB(0, 0, 0);
			pLVCD->clrText = clrNewTextColor;
			break;
		}
        *pResult = CDRF_DODEFAULT;        
	}
} 

BOOL CDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			CDialog::OnCancel();
			break;
		case VK_RETURN:
			if(pMsg->hwnd == GetDlgItem(IDC_SETRT06)->GetSafeHwnd())
			{
				if(m_rt06check.GetCheck() != BST_CHECKED)
					m_errormsg.SetWindowText("6개월 수익률 체크박스를 체크해 주세요.");
				else
					CDlg::OnSearch();
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_SETRATE)->GetSafeHwnd())
			{
				if(m_ratecheck.GetCheck() != BST_CHECKED)
					m_errormsg.SetWindowText("누적수익률 체크박스를 체크해 주세요.");
				else
					CDlg::OnSearch();
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_INPUT)->GetSafeHwnd())
			{
				CDlg::OnSearch();
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_ADD)->GetSafeHwnd())
			{
				OnAdd();
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_LIST1)->GetSafeHwnd())
			{
				OnAdd();
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_ALLDELETE)->GetSafeHwnd())
			{
				OnAlldelete();
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_UP)->GetSafeHwnd())
			{
				OnUp();
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_DOWN)->GetSafeHwnd())
			{
				OnDown();
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_NEWGROUP)->GetSafeHwnd())
			{
				OnNewgroup();
			}
			else if(pMsg->hwnd == GetDlgItem(IDC_GROUPEDIT)->GetSafeHwnd())
			{
				OnGroupedit();
			}
			return TRUE;													// 다이얼고그에서 엔터키 입력시 다이얼로그가 종료되기 때문에 return true를 통해 방지
			break;
		case VK_DELETE:
			if(pMsg->hwnd == GetDlgItem(IDC_LIST2)->GetSafeHwnd())
			{
				OnDelete();
			}
			break;
		case VK_INSERT:
			if(pMsg->hwnd == GetDlgItem(IDC_LIST2)->GetSafeHwnd())
			{
				OnAdd();
			}
			break;
		}		
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
