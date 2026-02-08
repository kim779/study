// Page4.cpp : implementation file
//

#include "stdafx.h"
#include "IB202201.h"
#include "Page4.h"
#include "page1.h"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "ioformat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	saveFILE	"portfolio.ini"
#define	tempFILE	"portfolio.ini.tmp"
#define	fieldFILE		"interField2.ini"
#define	tempfieldFILE	"interField2.ini.tmp"
const int idGRID	= 1000;
const int rowH		= 18;

const int colCODE = 0, colNAME = 1, colXNUM = 2, colXPRC = 3;
/////////////////////////////////////////////////////////////////////////////
// CPage4 property page
[[gsl::suppress(26409)]]
IMPLEMENT_DYNCREATE(CPage4, CPropertyPage)

CPage4::CPage4(CWnd* parent) : CPropertyPage(CPage4::IDD)
{
	//{{AFX_DATA_INIT(CPage4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_parent = parent;
	m_activegno = -1;
	m_inters.RemoveAll();
	m_ActiveInters.RemoveAll();
	m_activePage = false;

	m_grid = nullptr;
	m_rowC = maxJONGMOK + 1;
	m_colC = 4;
	m_bIsEdit = false;
}

CPage4::~CPage4()
{
	clearInterest();
	clearActiveInterest();
	if (m_grid  != nullptr) delete m_grid;
}

void CPage4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage4)
	DDX_Control(pDX, IDC_LIST1, m_list1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage4, CPropertyPage)
	//{{AFX_MSG_MAP(CPage4)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_BN_CLICKED(IDC_BTNSAVE, OnBtnsave)
	ON_BN_CLICKED(IDC_BTNJANGO, OnBtnjango)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage4 message handlers

BOOL CPage4::OnInitDialog() 
{
#ifdef DF_SEARCH
	CPropertyPage::OnInitDialog();

	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");
	m_font = getAxFont(idTOstring(IDS_DEFAULTFONT));

	CString userD;
	userD.Format("%s/%s/%s", m_root, USRDIR, m_name);
	CString saveN;
	saveN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, fieldFILE);
	CString tempN;
	tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempfieldFILE);

	CreateDirectory(userD, nullptr);
	CopyFile(saveN, tempN, FALSE);

	m_activegno = m_page->GetActiveGroup();
	initList();

	const int	nItem = loadList(m_activegno);

	const int gap2 = 2, offsetGH = 36;

	CRect rect, clientRC;
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_GROUPBOX);
	pWnd->GetWindowRect(&rect); ScreenToClient(&rect);

	clientRC.left = rect.left + gap2;
	clientRC.right = rect.right - gap2;
	clientRC.top = rect.top + offsetGH;
	clientRC.bottom = rect.bottom - gap2;

	LOGFONT lf;
	m_font->GetLogFont(&lf);
	lf.lfHeight = 90;
	m_grid = new CfxGrid(&lf);
	m_grid->Create(clientRC, this, idGRID, GVSC_VERT);

	initGrid();

	if (m_activegno != -1)
	{
		loadingInterest(m_activegno);  //수정완료
		selcMark(&m_list1, nItem);
		loadConfig();
	}

	if (IsWin8())
		SetSize();

	return TRUE;
#else
	CPropertyPage::OnInitDialog();
	
	m_root = Variant(homeCC, "");
	m_name = Variant(nameCC, "");
	m_font = getAxFont(idTOstring(IDS_DEFAULTFONT));

	CString userD; 
	userD.Format("%s/%s/%s", m_root, USRDIR, m_name);
	CString saveN; 
	saveN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, fieldFILE);
	CString tempN; 
	tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempfieldFILE);
	
	CreateDirectory(userD, nullptr); 
	CopyFile(saveN, tempN, FALSE);

	m_activegno = m_page->GetActiveGroup();
	initList(); 
	const int	nItem = loadList(m_activegno);

	const int gap2 = 2, offsetGH = 36;

	CRect rect, clientRC;
	CWnd* pWnd = (CWnd *)GetDlgItem(IDC_GROUPBOX);
	pWnd->GetWindowRect(&rect); ScreenToClient(&rect);

	clientRC.left  = rect.left + gap2;
	clientRC.right = rect.right - gap2;
	clientRC.top   = rect.top + offsetGH;
	clientRC.bottom= rect.bottom - gap2;

	LOGFONT lf; 
	m_font->GetLogFont(&lf); 
	lf.lfHeight = 90;
	m_grid = new CfxGrid(&lf); 
	m_grid->Create(clientRC, this, idGRID, GVSC_VERT);

	initGrid();

	if (m_activegno != -1)
	{
		loadingInterest(m_activegno); //수정완료
		loadGrid(); 
		selcMark(&m_list1, nItem);
		loadConfig();
	}

	if(IsWin8())
		SetSize();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
#endif
}

bool CPage4::loadConfig()
{
	CString tempN; 
	tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, fieldFILE);
	
	CFileFind finder;
	if (!finder.FindFile(tempN))
		return false;
	
	int	readL = 0;
	char	readB[1024];
	CString	string, stringx;
	
	readL = GetPrivateProfileString("INTERFIELD", "CHKALLSAVE", "", readB, sizeof(readB), tempN);
	if(readL)
	{
		CButton* btn = nullptr;

		btn = (CButton *)GetDlgItem(IDC_CHKSAVEALL);
		if(readB[0] == '1')
		{
			btn->SetCheck(TRUE);
		}
		else
		{
			btn->SetCheck(FALSE);
		}
	}
	
	return true;
}

bool CPage4::saveConfig()
{
	CString	string, stringx;
	CString tempFile; 
	tempFile.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempfieldFILE);

	CButton* btn; 
	btn = (CButton *) GetDlgItem(IDC_CHKSAVEALL);
	string = _T("0"); 
	if (btn->GetCheck()) 
		string = _T("1");

	WritePrivateProfileString("INTERFIELD", "CHKALLSAVE", string, tempFile);
	
	return true;
}

BOOL CPage4::OnSetActive() 
{
	CWnd* wnd = GetParent()->GetParent();
	 wnd->SendMessage(WM_MSG, MAKEWPARAM(MSG_SET_ACTIVE_PAGE, 0), (LPARAM)4);

	m_activegno = m_page->GetActiveGroup();
	if (m_activePage && m_activegno != -1)
	{
		m_list1.DeleteAllItems();
		const int	nItem = loadList(m_activegno);
		CRect	rc; CSize size;

		m_list1.SetItemState(nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
		m_list1.GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - m_list1.GetTopIndex());
		m_list1.Scroll(size);
		loadingInterest(m_activegno);  //수정완료
		//loadGrid();
	}

	m_activePage = true;

	return CPropertyPage::OnSetActive();
}

BOOL CPage4::OnKillActive() 
{
	if (m_activegno != -1)
	{
		saveConfig();
		//if(m_inters.GetSize() > 0)
		//{
		//	savingInterest(m_activegno);
		//}
		//CheckGroupAllSave();
	}
	return CPropertyPage::OnKillActive();
}

void CPage4::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int	nitem{}, gno{};
	CString	name;
	POSITION pos = m_list1.GetFirstSelectedItemPosition();

	if (pos)
	{
		nitem = m_list1.GetNextSelectedItem(pos);

		name = m_list1.GetItemText(nitem, 0);
		gno  = m_list1.GetItemData(nitem);

		if (m_activegno != -1) 
		{	
			savingInterest(m_activegno);
		}

		m_activegno = gno; 
		loadingInterest(m_activegno);   //수정완료

		m_grid->MessageToGrid(WM_VSCROLL, SB_TOP); 
		//loadGrid();
	}

	*pResult = 0;
}

BOOL CPage4::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NM_GVITEM* nmgv = (NM_GVITEM*) lParam;
	if (wParam != idGRID)
		return CPropertyPage::OnNotify(wParam, lParam, pResult);

	switch (nmgv->hdr.code)
	{
	case GVNM_ENDEDIT: case GVNM_ENDTABKEY: case GVNM_ENDRETURN:
		{
			m_bIsEdit = true;
			CString	string = m_grid->GetItemText(nmgv->row, nmgv->col);
			const int	index = nmgv->row - 1;
			
			if (index < 0 || index >= m_inters.GetSize())
				break;

			_inters* pinter = m_inters.GetAt(index);
			string.TrimLeft(), string.TrimRight();
			switch (nmgv->col)
			{
			case colXNUM:
				memset(pinter->xnum, 0x00, sizeof(pinter->xnum));
				memcpy(pinter->xnum, string.operator LPCTSTR(), min(string.GetLength(), 10));
				break;
			case colXPRC:
				memset(pinter->xprc, 0x00, sizeof(pinter->xprc));
				memcpy(pinter->xprc, string.operator LPCTSTR(), min(string.GetLength(), 10));
				break;
			default:
				return CPropertyPage::OnNotify(wParam, lParam, pResult);
			}

			m_inters.SetAt(index, pinter);
			if (m_page->GetActiveGroup() == m_activegno)
				m_page->SetArray(m_inters);
		}
		break;
	default:
		break;
	}

	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}


void CPage4::OnBtnsave() 
{
	if (m_activegno != -1)
	{
		m_bIsEdit = true;
		savingInterest(m_activegno);
		//CheckGroupAllSave();
	}
}

void CPage4::OKaction()
{
	CFileFind finder;
	CString	filePath, fileTemp;

	CString saveN; 
	saveN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, fieldFILE);
	CString tempN; 
	tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempfieldFILE);
	DeleteFile(saveN); 
	CopyFile(tempN, saveN, FALSE);

	//filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);
	//fileTemp.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);

	//if (finder.FindFile(fileTemp))
	//{
	//	DeleteFile(filePath); 
	//	CopyFile(fileTemp, filePath, FALSE);
	//}

	//for (int ii = 0; ii < maxGROUP; ii++)
	//{
	//	filePath.Format("%s/%s/%s/portfolio.i%02d",     m_root, USRDIR, m_name, ii);
	//	fileTemp.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, ii);

	//	if (!finder.FindFile(fileTemp))
	//		continue;

	//	CopyFile(fileTemp, filePath, FALSE);		
	//}

	CCaction();

}

void CPage4::CCaction()
{ 
	//CString tempN; tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);
	//DeleteFile(tempN);

	//CString tempInter; 
	//tempInter.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempfieldFILE);
	//DeleteFile(tempInter);

	//CFileFind finder;
	//CString	fileTemp;
	//for (int ii = 0; ii < maxGROUP; ii++)
	//{
	//	fileTemp.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, ii);

	//	if (!finder.FindFile(fileTemp))
	//		continue;

	//	DeleteFile(fileTemp);
	//}
}

void CPage4::initList()
{
	AddColumn(&m_list1, "", 0, 0);
	m_list1.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

int CPage4::loadList(int param)
{
#ifdef DF_SEARCH
	CString string, stmp, gname;
	CWnd* wnd = GetParent()->GetParent();
	memset(m_pdata, 0x00, 1024 * 4);
	const bool iret = wnd->SendMessage(WM_MSG, MAKEWPARAM(MSG_GET_GROUPARR, 0), (LPARAM)m_pdata);
	if (iret == 0)
		return -1;

	int	ipos = 0;
	int ret = 0;
	string.Format("%s", m_pdata);
	int val{};
	while (!string.IsEmpty())
	{
		stmp = parseX(string, "\t");
		val = atoi(parseX(stmp, ";"));
		gname = stmp;
		if (param < 0)
			if (m_activegno == -1) m_activegno = val;

		const int nItem = AddItem(&m_list1, gname, ipos++);
		if (val == param)
			ret = nItem;
		m_list1.SetItemData(nItem, val);
	}

	return ret;
#else
	int	ret = 0;
	int readL = 0; char readB[512]; //관심그룹 많을때 256->512
	CString	filePath, string, gno, gname;

	filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, saveFILE);

	readL = GetPrivateProfileString(_T("GROUPORDER"), "00", "", readB, sizeof(readB), filePath);
	if (readL <= 0)	return ret;

	string = CString(readB, readL);

	int	ipos = 0;
	int	val = 0;
	while (!string.IsEmpty())
	{
		gno = parseX(string, ";");
		if (gno.IsEmpty() || gno.GetLength() != 2)
			break;

		val = atoi(gno);
		if (param < 0)
		{
			if (m_activegno == -1) m_activegno = val;
		}
		
		readL = GetPrivateProfileString("GROUPNAME", gno, "", readB, sizeof(readB), filePath);
		gname = CString(readB, readL);

		if (ExistFile(val))
		{
			const int nItem = AddItem(&m_list1, gname, ipos++);
			if (val == param)
				ret = nItem;	
			m_list1.SetItemData(nItem, val);
		}
	}

	return ret;
#endif
}

void CPage4::initGrid()
{
	const struct	_gridHdr {
		UINT	stid;		// string table id
		UINT	width;		// fixed width
		UINT	attr;		// fixed attribute

		UINT	format;		// nonfixed format
		UINT	attrx;		// nonfixed attr
	} gridHdr[] = {
		{IDS_GH_CODE,		90,	GVAT_TEXT,	GVFM_CENTER,	0},			// 0
		{IDS_GH_NAME,		150,		0,	GVFM_LEFT,	0},			// 1
		{IDS_GH_POSSNUM,	100,		0,	GVFM_RIGHT,	GVAT_COMMA|GVAT_EDIT},	// 2
		{IDS_GH_POSSPRC,	100,		0,	GVFM_RIGHT,	GVAT_COMMA|GVAT_EDIT},	// 3
	};


	m_grid->Initial(m_rowC, m_colC, 1, 0);

	LOGFONT lf; m_font->GetLogFont(&lf); lf.lfHeight = 90;

//	int	width = 0;
	GVITEM	gvitem;;

 	// column header setting
	for (int ii = 0; ii < m_colC; ii++)
	{
		gvitem.row   = 0;
		gvitem.col   = ii;
		gvitem.state = 0;
		gvitem.mask  = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		gvitem.format= GVFM_HEADER;
		gvitem.fgcol = PALETTERGB(  0,   0,   0);
		gvitem.bkcol = PALETTERGB(205, 205, 255);

		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

		gvitem.text  = idTOstring(gridHdr[ii].stid);
		gvitem.attr  = gridHdr[ii].attr;

		m_grid->SetItem(&gvitem);
		m_grid->SetColumnWidth(ii, gridHdr[ii].width);
	}

	for (int ii = 0; ii < m_colC; ii++)
	{
		gvitem.state = 0;
		gvitem.mask  = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR;

		gvitem.format= gridHdr[ii].format;
		gvitem.attr  = gridHdr[ii].attrx;
		gvitem.fgcol = PALETTERGB(  0,   0,   0);
		gvitem.bkcol = PALETTERGB(255, 255, 255);

		CopyMemory(&gvitem.font, &lf, sizeof(LOGFONT));

		m_grid->SetColumnItems(ii, &gvitem);
	}

	for (int ii = 0; ii < m_grid->GetRowCount(); ii++)
		m_grid->SetRowHeight(ii, rowH);

	m_grid->SetRowColResize(FALSE, TRUE);
	m_grid->SetGridFocusLine(TRUE, TRUE);
	m_grid->SetGridColor(PALETTERGB(192, 192, 192));
	m_grid->Adjust();
}

void CPage4::loadGrid()
{
	m_grid->Clear();

	_inters* pinter = nullptr;
	for (int ii = 0; ii < m_inters.GetSize(); ii++)
	{
		pinter = m_inters.GetAt(ii);

		m_grid->SetItemText(ii+1, colCODE, chTOstring(pinter->code, sizeof(pinter->code)));

		if (pinter->gubn[0] == ROW_COMMENT)
		{
			UINT attr = m_grid->GetItemAttr(ii + 1, colXNUM) & GVAT_HIDDEN;
			m_grid->SetItemAttr(ii + 1, colXNUM, attr);
			attr = m_grid->GetItemAttr(ii + 1, colXPRC) & GVAT_HIDDEN;
			m_grid->SetItemAttr(ii + 1, colXPRC, attr);
		}

		if (strlen(pinter->code))
		{
			m_grid->SetItemText(ii+1, colNAME, chTOstring(pinter->name, sizeof(pinter->name)));
			m_grid->SetItemText(ii+1, colXNUM, chTOstring(pinter->xnum, sizeof(pinter->xnum)));
			m_grid->SetItemText(ii+1, colXPRC, chTOstring(pinter->xprc, sizeof(pinter->xprc)));
		}
	}
}

int	CPage4::loadingActiveInterest(int gno)
{
	return 0;
}

int CPage4::loadingInterest(int gno)
{
#ifdef DF_SEARCH
	CWnd* wnd = GetParent()->GetParent();
	char* pdata = (char*)wnd->SendMessage(WM_MSG, MSG_SEARCH_GROUPCODE_PAGE4, gno);
	m_page->loadingInterest(gno);
	return 0;
#else
	clearInterest();

	CString	filePath, fileBook;
	if (ExistFile(gno))
		filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);
	else
		return 0;
	
	fileBook.Format("%s/%s/%s/bookmark.i%02d", m_root, USRDIR, m_name, gno);
	bool    isFile = true;
	UINT	readL{};
	struct	_inters* pinter{};
	struct  _bookmarkinfo* bInfo{};

	CFile	fileH(filePath, CFile::modeRead);
	CFile	fileB;

	if( !fileB.Open(fileBook, CFile::modeRead) )
	{
		isFile = false;		//파일 없을경우
	}

	for (int ii = 0; ii < maxJONGMOK; ii++)
	{
		pinter = (_inters *) new char[sz_inters];
		ZeroMemory(pinter, sz_inters);

		readL = fileH.Read(pinter, sz_inters);
		if (readL < sz_inters)
		{
			delete pinter;
			break;
		}
		
		if (pinter->code[0] == 'm')
		{

			if(isFile == true)
			{
				bInfo = (_bookmarkinfo *) new char[sizeof(_bookmarkinfo)];
				ZeroMemory(bInfo, sizeof(_bookmarkinfo));
				
				readL = fileB.Read(bInfo, sizeof(_bookmarkinfo));

				if(readL < sizeof(_bookmarkinfo))
				{
					delete bInfo;
				}
				else
				{
					CString temp = CString((pinter->code));
					temp = temp.Left(12);
					temp.TrimRight();
					CString temp2 = CString(bInfo->code);
					temp2.TrimRight();
					
					if(strcmp(temp, temp2) == 0)
					{
						CopyMemory(pinter->name, bInfo->name, sizeof(bInfo->name));
					
						pinter->bookmark[0] = bInfo->bookmark[0] == '1' ? '1':'0';//2015.04.03 KSJ 1이아니면 0으로 해준다.
					}
				}
			}
		
		}
		else
		{
			strcpy(pinter->name, m_page->GetCodeName(pinter->code));
		}
		
		m_inters.Add(pinter);
	}

	fileH.Close();
	return m_inters.GetSize();
#endif
}

void CPage4::savingInterest(int gno)
{
	//m_bIsEdit = true;    
	//
	//CString	filePath;
	//filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);

	//struct	_inters* pinter{};

	//CFile	fileH(filePath, CFile::modeWrite|CFile::modeCreate);

	//if (fileH.m_hFile == CFile::hFileNull) return;

	//for (int ii = 0; ii < m_inters.GetSize(); ii++)
	//{
	//	pinter = m_inters.GetAt(ii);
	//	fileH.Write(pinter, sizeof(_inters));
	//}
	//fileH.Close();
}

//모든 그룹에 동일 적용 체크 여부
void CPage4::CheckGroupAllSave()
{
	CButton* btn = (CButton *) GetDlgItem(IDC_CHKSAVEALL);
	if(btn->GetCheck())
	{
		savingSameCodeInGroup();
		savingAllGroup();
	}
}

void CPage4::savingAllGroup()
{	
	int count = 0;
	CString code, qty, price;
	bool result = false;
	_inters* pinter = nullptr;

	const int activeCount = loadingActiveInterest(m_activegno);

	for(int i=1 ; i<=MAXGROUP ; i++)
	{
		if(m_activegno == i)
			continue;

		count = loadingInterest(i);

		if(count == 0)
			continue;
		
		for (int j = 0; j < m_ActiveInters.GetSize(); j++)
		{
			pinter = m_ActiveInters.GetAt(j);

			code = CString(pinter->code, sizeof(pinter->code));
			qty = CString(pinter->xnum, sizeof(pinter->xnum));
			price = CString(pinter->xprc, sizeof(pinter->xprc));

			code.TrimLeft();code.TrimRight();
			qty.TrimLeft();qty.TrimRight();
			price.TrimLeft();price.TrimRight();

			if(code == "")
				continue;

			result = findingCodeInGroup(code, qty, price);
		}

		if(count != 0)
			savingInterest(i);
	}

}


void CPage4::savingSameCodeInGroup()
{
	_inters* pinter = nullptr;
	_inters* pinterComp = nullptr;
	CString code, qty, price;
	CString jCode, jQty, jPrice;
	
	for (int ii = 0; ii < m_inters.GetSize(); ii++)
	{
		pinter = m_inters.GetAt(ii);
		
		code= pinter->code;
		code = code.Left(12);
		code.TrimLeft();code.TrimRight();
		qty = CString(pinter->xnum, sizeof(pinter->xnum));
		price = CString(pinter->xprc, sizeof(pinter->xprc));
		qty.TrimLeft();qty.TrimRight();
		price.TrimLeft();price.TrimRight();	
		
		if(!qty.IsEmpty() || !price.IsEmpty())
		{
			for(int jj=0 ; jj < m_inters.GetSize() ; jj++)
			{
				if(ii == jj)
					continue;

				pinterComp = m_inters.GetAt(jj);
				jCode = pinterComp->code;
				jCode = jCode.Left(12);
				jQty = CString(pinterComp->xnum, sizeof(pinterComp->xnum));
				jPrice = CString(pinterComp->xprc, sizeof(pinterComp->xprc));	

				jCode.TrimLeft();jCode.TrimRight();
				jQty.TrimLeft();jQty.TrimRight();
				jPrice.TrimLeft();jPrice.TrimRight();

				if(code == jCode)
				{
					if(jQty == "")
					{
						CopyMemory(pinterComp->xnum, qty, min(sizeof(pinterComp->xnum), qty.GetLength()) );
					}

					if(jPrice == "")
					{
						CopyMemory(pinterComp->xprc, price, min(sizeof(pinterComp->xprc), price.GetLength()) );
					}
					
					m_inters.SetAt(jj, pinterComp);
				}
				
			}
		}
	}

	if(m_inters.GetSize() > 0)
		savingInterest(m_activegno);

}

//그리드에 입력된 종목이 그룹에 있는지 검색해서 
bool CPage4::findingCodeInGroup(CString code, CString qty, CString price)
{
	_inters* pinter = nullptr;
	CString codeInMemory;
	bool result = false;
	
	for (int ii = 0; ii < m_inters.GetSize(); ii++)
	{
		pinter = m_inters.GetAt(ii);
		
		codeInMemory = pinter->code;
		codeInMemory = codeInMemory.Left(12);
		codeInMemory.TrimLeft();codeInMemory.TrimRight();
		
		if(codeInMemory == "")
			continue;

		if (codeInMemory == code)
		{
			ZeroMemory(pinter->xnum, sizeof(pinter->xnum));
			ZeroMemory(pinter->xprc, sizeof(pinter->xprc));
			CopyMemory(pinter->xnum, qty, min(sizeof(pinter->xnum), qty.GetLength()));
			CopyMemory(pinter->xprc, price, min(sizeof(pinter->xprc), price.GetLength()));

			result = true;
		}
	}

	return result;
}


void CPage4::clearInterest()
{
	for (int ii = 0; ii < m_inters.GetSize(); ii++)
		delete m_inters.GetAt(ii);
	m_inters.RemoveAll();
}

void CPage4::clearActiveInterest()
{
	for (int ii = 0; ii < m_ActiveInters.GetSize(); ii++)
		delete m_ActiveInters.GetAt(ii);
	m_ActiveInters.RemoveAll();
}

void CPage4::selcMark(CListCtrl* listctrl, int nItem, bool scroll /*=true*/)
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

void CPage4::deselcMark(CListCtrl* listctrl)
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

//////////////////////////////////////////////////////////////////////////////////////////////
//
BOOL CPage4::AddColumn(CListCtrl *ctrl, LPCTSTR string, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR) string;
	lvc.cx      = ctrl->GetStringWidth(lvc.pszText) + width;

	return ctrl->InsertColumn(item, &lvc);
}

int CPage4::AddItem(CListCtrl *ctrl, LPCSTR string, int nItem, int nSubItem)
{
	if (nSubItem < 0)
		return ctrl->InsertItem(LVIF_TEXT, nItem, string, 0, 0, 0, 0);
	else
		return ctrl->SetItemText(nItem, nSubItem, string);
}

bool CPage4::ExistFile(int gno, bool temp /*=false*/)
{
	//CString	filePath;

	//if (temp)
	//	filePath.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, gno);
	//else
	//	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);

	//CFileFind finder;
	//if (!finder.FindFile(filePath))
	//	return false;

	return true;
}

CString CPage4::parseX(CString &srcstr, CString substr)
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

CFont* CPage4::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point; fontR.italic = false; fontR.bold = 0;

	switch (style)
	{
	case 0: case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_parent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CString CPage4::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

CString CPage4::idTOstring(UINT id)
{
	CString	msg;
	if (!msg.LoadString(id))
		msg = _T("");
	return msg;
}

CString CPage4::chTOstring(char* chB, int chL)
{
	CString string = CString(chB, chL);
	string.TrimRight(); return string;
}

void CPage4::SetOther(class CPage1* page)
{
	m_page = page; 
}


int CPage4::GetActiveGroup()
{
	return m_activegno;
}

void CPage4::SetArray(CArray<struct _inters*, struct _inters*>& arInters)
{
	clearInterest();
	struct	_inters*	pinters = nullptr;

	for ( int ii = 0 ; ii < maxJONGMOK ; ii++ )
	{
		pinters = new struct _inters;
		memset(pinters, 0x00, sz_inters);
		memcpy(pinters, arInters.GetAt(ii), sz_inters);
		m_inters.Add(pinters);
	}
}

void CPage4::OnBtnjango() 
{
	// TODO: Add your control notification handler code here
	CString string;
	string.Format("IB120100 /p5 /S/d%s", "");

	m_parent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUP), (LPARAM)(LPCTSTR)string);
}

bool CPage4::IsWin8()
{
	bool bReturn = false;
	//2013.01.07 KSJ win8에서는 사이즈를 366으로 해준다.
	OSVERSIONINFOEX osvi;
	osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFOA);
	GetVersionExA((LPOSVERSIONINFO)&osvi);
	
	if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
	{
		bReturn = true;
	}
	//KSJ
	return bReturn;
}

void CPage4::SetSize()
{
	constexpr int x = 5;
	constexpr int y = 5;

	((CStatic *)GetDlgItem(IDC_LIST1_STATIC))->MoveWindow(x, y, 162, 330);
	((CListCtrl *)GetDlgItem(IDC_LIST1))->MoveWindow(x+6, y+18, 150, 307);	
	((CStatic *)GetDlgItem(IDC_GROUPBOX))->MoveWindow(x+170, y, 467, 330);	
	((CStatic *)GetDlgItem(IDC_CHKSAVEALL))->MoveWindow(x+176, y+18, 410, 15);
	m_grid->MoveWindow(x+174, y+37, 457, 288);
}

void CPage4::SendMsgToPage(int igubn, CString sdata)
{
	switch (igubn)
	{
	case MSG_MTP_CPL_SEARCH_GROUPCODE:
	{
		CString sResult;
		CWnd* wnd = GetParent()->GetParent();
		memset(m_pdata, 0x00, 1024 * 4);
		BOOL ret = wnd->SendMessage(WM_MSG, MAKEWPARAM(MSG_GET_GROUPCODE, atoi(sdata)), (LPARAM)m_pdata);
		if (ret == FALSE)
			return;

		clearInterest();
	
		UINT	readL{};
		struct	_inters* pinter{};
		struct  _bookmarkinfo* bInfo{};

		CString stmp;
		CString strseq = CString(m_pdata, 2);
		CString strgname = CString(&m_pdata[2], 20);
		int count = atoi(CString(&m_pdata[22], 4));
		int parseL = 26;

		struct _jinfo* jinfo{};
		for (int ii = 0; ii < count; ii++)
		{
			pinter = (_inters*) new char[sz_inters];
			ZeroMemory(pinter, sz_inters);

			jinfo = (struct _jinfo*)&m_pdata[parseL];

			memcpy(pinter->gubn, jinfo->gubn, sizeof(jinfo->gubn));		// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
			memcpy(pinter->code, jinfo->code, sizeof(jinfo->code));			// 종목코드[12]

			if (pinter->gubn[0] == ROW_COMMENT)
			{
				stmp.Format("%s", jinfo->xprc);
				stmp = stmp.Left(sizeof(jinfo->xprc) + sizeof(jinfo->xprc));
				stmp.Trim();
				strcpy(pinter->name, (LPSTR)(LPCTSTR)stmp);
			}
			else
			{
				memcpy(pinter->xprc, jinfo->xprc, sizeof(jinfo->xprc));				// 보유단가[10]
				memcpy(pinter->xnum, jinfo->xnum, sizeof(jinfo->xnum));  	     // 보유수량[10]
				strcpy(pinter->name, m_page->GetCodeName(jinfo->code));
			}

			m_inters.Add(pinter);

			parseL += sizeof(struct _jinfo);
		}  //for
		loadGrid();
	}
	break;  //MSG_MTP_CPL_SEARCH_GROUPCODE


	}
}

CString CPage4::GetGridData()
{
	CString code, price, qty;
	CString sResult, stmp;
	sResult.Empty();
	for (int ii = 1; ii < m_grid->GetRowCount(); ii++)
	{
		code = m_grid->GetItemText(ii, colCODE); code.TrimRight();
		if (code.GetLength() <= 0)
			break;
		qty = m_grid->GetItemText(ii, colXNUM); qty.TrimRight(); qty.TrimLeft();
		price = m_grid->GetItemText(ii, colXPRC); price.TrimRight(); price.TrimLeft();
		stmp.Format("%s\t%s\t%s", code, qty, price);
		sResult += stmp;
		sResult += "\n";
	}
	return sResult;
}

BOOL CPage4::endSendAction()
{
	CString	gname;
	if (m_activegno != -1)
	{
		gname = GetGroupName(m_activegno);  //endSendAc

		//서버에 전송
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEndACTION, m_activegno - 1), 4);
	}
	//else if (GetGroupCount() == 0)	//2015.04.09 KSJ 선택된 그룹이 없고 그룹카운트가 0 이라면 전체삭제된것으로 파악하고 창을 닫는다.
	//{
	//	CString string;
	//	string = "OnPortfolio\tok";
	//	m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR)string);
	//	m_parent->SendMessage(WM_USER, closeDLL, 0);
	//}

	return CPropertyPage::OnKillActive();
}

CString	CPage4::GetGroupName(int gno)
{
	CString string, stmp;
	CWnd* wnd = GetParent()->GetParent();
	memset(m_pdata, 0x00, 1024 * 4);
	const bool ret = wnd->SendMessage(WM_MSG, MAKEWPARAM(MSG_GET_GROUPARR, 0), (LPARAM)m_pdata);
	if (ret == 0)
		return "";

	string.Format("%s", m_pdata);
	int igroup{};
	while (!string.IsEmpty())
	{
		stmp = parseX(string, "\t");
		igroup = atoi(parseX(stmp, ";"));
		if (gno == igroup)
			return stmp;
	}

	return "";
}
int CPage4::GetUploadData(int gno, CString& name, char* datB)
{
	CWnd* wnd = GetParent()->GetParent();
	
	name = GetGroupName(gno);  //GetUploadData
	CheckPage4();
	int datL = 0;
	struct	_inters* pinter{};
	struct	_jinfo* jinfo{};
	for (int ii = 0; ii < m_inters.GetSize(); ii++)
	{
		pinter = m_inters.GetAt(ii);
		jinfo = (struct _jinfo*)&datB[datL];
		datL += sz_jinfo;

		memset(jinfo->gubn, ' ', sizeof(jinfo->gubn));
		memset(jinfo->code, ' ', sizeof(jinfo->code));
		memset(jinfo->xprc, ' ', sizeof(jinfo->xprc));
		memset(jinfo->xnum, ' ', sizeof(jinfo->xnum));
		memset(jinfo->bookrgb, ' ', sizeof(jinfo->bookrgb));

		jinfo->gubn[0] = pinter->gubn[0];
		CopyMemory(jinfo->code, pinter->code, strlen(pinter->code));

		if (pinter->gubn[0] == ROW_COMMENT)
			CopyMemory(jinfo->xprc, pinter->name, strlen(pinter->name));
		else
		{
			CopyMemory(jinfo->xprc, pinter->xprc, strlen(pinter->xprc));
			CopyMemory(jinfo->xnum, pinter->xnum, strlen(pinter->xnum));
		}
		CopyMemory(jinfo->bookrgb, pinter->bookrgb, sizeof(pinter->bookrgb));  //test IO

		//if (jinfo->gubn[0] == ROW_BOOKMARK)   //test IO
		//{//HTS에서 북마크를 신규로 적용해서 MTS에서 색상을 내려받은게 없다면  디폴트 01
		//	if (CString(pinter->bookrgb, 2).IsEmpty())
		//	{
		//		CopyMemory(jinfo->bookrgb, "01", sizeof(jinfo->bookrgb));
		//	}
		//}

	}

	return m_inters.GetSize();
}

void CPage4::CheckPage4()
{
	CString sData;
	sData = GetGridData();

	if (sData.IsEmpty())
		return;

	CString stmp, stemp, scode, sprc, sval;
	int icnt = 0;
	struct	_inters* pinter{};
	while (!sData.IsEmpty())
	{
		stmp = parseX(sData, "\n");
		scode = parseX(stmp, "\t");		 scode.TrimRight(); scode.TrimLeft();
		sval = parseX(stmp, "\t");			 sprc.TrimRight(); sprc.TrimLeft();
		sprc = parseX(stmp, "\t");		 sval.TrimRight(); sval.TrimLeft();

		pinter = m_inters.GetAt(icnt);
		stemp.Format("%.12s", pinter->code); stemp.TrimRight();
		if (stemp == scode)
		{
			if (!sprc.IsEmpty())
				CopyMemory(pinter->xprc, sprc, sprc.GetLength());
			if (!sval.IsEmpty())
				CopyMemory(pinter->xnum, sval, sval.GetLength());
		}
	}
}