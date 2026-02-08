// Page4.cpp : implementation file
//

#include "stdafx.h"
#include "IB202201.h"
#include "Page4.h"
#include "page1.h"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "ioformat.h"
#include "MapWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	fieldFILE	"interField2.ini"
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
	m_inters.clear();
	m_ActiveInters.clear();
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
	const int nItem = loadList(m_activegno);

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
	m_grid = std::make_unique<CfxGrid>(&lf); 
	m_grid->Create(clientRC, this, idGRID, GVSC_VERT);

	initGrid();

	if (m_activegno != -1)
	{
//		loadingInterest(m_activegno);
//		loadGrid(); 
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(dnGROUPCODE, 0), m_activegno);
		selcMark(&m_list1, nItem);
		loadConfig();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
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
			_bAll = true;
			
		}
		else
		{
			btn->SetCheck(FALSE);
			_bAll = false;
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

		selcMark(&m_list1, m_activegno - 1);
	//	loadingInterest(m_activegno);
	//	loadGrid();
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(dnGROUPCODE, 0), m_activegno);
	}

	m_activePage = true;

	return CPropertyPage::OnSetActive();
}

BOOL CPage4::OnKillActive() 
{	
	if (m_activegno != -1)
	{
		saveConfig();
		if(m_inters.size() > 0)
			savingInterest(m_activegno);

		CheckGroupAllSave();
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
//			CheckGroupAllSave();
		}

		m_activegno = gno; 
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(dnGROUPCODE, 0), m_activegno);
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
			
			if (index < 0 || index >= gsl::narrow_cast<int>(m_inters.size()))
				break;
			auto& pinter = m_inters.at(index);
			string.Trim();
			switch (nmgv->col)
			{
			case colXNUM:
				pinter->xnum = string;
				break;
			case colXPRC:
				pinter->xprc = string;
				break;
			default:
				return CPropertyPage::OnNotify(wParam, lParam, pResult);
			}
//XMSG(필요없음 삭제);
//			m_inters.SetAt(index, pinter);
//			if (m_page->GetActiveGroup() == m_activegno)
//				m_page->SetArray(m_inters);
		}
		break;
	default:
		break;
	}

	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}


void CPage4::OnBtnsave() 
{
// XMSG(사용안함);
	if (m_activegno != -1)
	{
		m_bIsEdit = true;
		//savingInterest(m_activegno);
		//CheckGroupAllSave();

		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upACTION, 0), 0);
	}
}

void CPage4::OKaction()
{
//	DWORD	fLen = 0; 
	CFileFind finder;
	CString	filePath, fileTemp;

	CString saveN; 
	saveN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, fieldFILE);
	CString tempN; 
	tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempfieldFILE);
	DeleteFile(saveN); 
	CopyFile(tempN, saveN, FALSE);

	CCaction();
}

void CPage4::CCaction()
{ 
//	CString tempN; tempN.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempFILE);
//	DeleteFile(tempN);

	CString tempInter; 
	tempInter.Format("%s/%s/%s/%s", m_root, USRDIR, m_name, tempfieldFILE);
	DeleteFile(tempInter);

//	DWORD	fLen = 0;
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
	int nItem = 0;
	for (int ii = 0; ii < 100; ii++)
	{
		nItem = AddItem(&m_list1, m_page->m_manageGroup[ii][3], ii);
		m_list1.SetItemData(nItem, atoi(m_page->m_manageGroup[ii][0]));
	}
	return nItem;	
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

	//int	width = 0;
	GVITEM	gvitem;

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
	for (int ii = 0; ii < gsl::narrow_cast<int>(m_inters.size()); ii++)
	{
		auto& pinter = m_inters.at(ii);
//		m_grid->InsertRow("");
		m_grid->SetItemText(ii+1, colCODE, pinter->code);
		if (pinter->gubn == ROW_COMMENT)
		{
			UINT attr = m_grid->GetItemAttr(ii + 1, colXNUM) & GVAT_HIDDEN;
			m_grid->SetItemAttr(ii + 1, colXNUM, attr);
			attr = m_grid->GetItemAttr(ii + 1, colXPRC) & GVAT_HIDDEN;
			m_grid->SetItemAttr(ii + 1, colXPRC, attr);
			m_grid->SetItemText(ii + 1, colNAME, pinter->name);
		}
		else
		{
			if (!pinter->code.IsEmpty())
			{
				m_grid->SetItemText(ii + 1, colNAME, pinter->name);
				m_grid->SetItemText(ii + 1, colXNUM, pinter->xnum);
				m_grid->SetItemText(ii + 1, colXPRC, pinter->xprc);
			}
		}
		
	}
}

void CPage4::savingInterest(int gno)
{

}

//모든 그룹에 동일 적용 체크 여부
void CPage4::CheckGroupAllSave()
{	
//xxxxx
//XMSG(서버 TR로 수정 예정);
	CButton* btn = (CButton *) GetDlgItem(IDC_CHKSAVEALL);
	if(btn->GetCheck())
	{
		_bAll = true;
	}
	else
	{
		_bAll = false;
	}
}

void CPage4::clearInterest()
{
	m_inters.clear();
}

void CPage4::clearActiveInterest()
{
	m_ActiveInters.clear();
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
	CString	filePath;

	if (temp)
		filePath.Format("%s/%s/%s/portfolio.i%02d.tmp", m_root, USRDIR, m_name, gno);
	else
		filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_name, gno);

	CFileFind finder;
	if (!finder.FindFile(filePath))
		return false;

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
	//clearInterest();
	//struct	_inters*	pinters = nullptr;

	//for ( int ii = 0 ; ii < maxJONGMOK ; ii++ )
	//{
	//	pinters = new struct _inters;
	//	memset(pinters, 0x00, sz_inters);
	//	memcpy(pinters, arInters.GetAt(ii), sz_inters);
	//	m_inters.Add(pinters);
	//}
}

void CPage4::OnBtnjango() 
{
	// TODO: Add your control notification handler code here
	CString string;
	string.Format("IB120100 /p5 /S/d%s", "");

	m_parent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUP), (LPARAM)(LPCTSTR)string);
}

int CPage4::GetUploadData(int gno, CString& name, char* datB)
{
	gsl::span<struct _jinfo> spanjinfo = gsl::span<struct _jinfo>((struct _jinfo*)datB, maxJONGMOK);
	int ii = 0;
	for_each(m_inters.begin(), m_inters.end(), [&](const auto& item) {
		auto& info = spanjinfo.at(ii++);
		FillMemory(&info, sizeof(struct	_jinfo), ' ');

		info.gubn[0] = item->gubn;
		CopyMemory(info.code, item->code, item->code.GetLength());
		if (item->gubn == ROW_COMMENT)
			CopyMemory(info.xprc, item->name, strlen(item->name));
		else
		{
			CopyMemory(info.xprc, item->xprc, item->xprc.GetLength());
			CopyMemory(info.xnum, item->xnum, item->xnum.GetLength());
		}
		CopyMemory(info.bookrgb, item->bookrgb, item->bookrgb.GetLength());
	});
	return m_inters.size();
}


BOOL CPage4::endSendAction() //저 장 버튼 클릭시
{
	CString	gname;
	if (m_activegno != -1)
	{
		//gname = GetGroupName(m_activegno);
		//savingGroupFile(m_activegno, gname);

		//gname = m_list1.GetItemText(m_activegno - 1, 0);

		//서버에 전송
		CWnd* wnd = GetParent()->GetParent();
		wnd->SendMessage(TOmapWnd, MAKEWPARAM(upEndACTION, m_activegno - 1), 0);
	}	
	return CPropertyPage::OnKillActive();
}

void CPage4::receiveOub(int key, CString data)
{
	switch (key)
	{
	case dnGROUPCODE:
	{
		m_inters.clear();
		const CString skey = data.Left(2);
		const CString sGroupName = data.Mid(2, 20);
		const CString sCount = data.Mid(22, 4);

		data = data.Mid(26);

		CString stmp;
		const gsl::span<_jinfo> codelist((_jinfo*)data.GetString(), atoi(sCount));
		for_each(codelist.begin(), codelist.end(), [&](auto item) {
			auto& inter = m_inters.emplace_back(std::make_unique<_intersx>());
			inter->copy(&item);
			if (inter->gubn == ROW_COMMENT)
			{
				stmp.Format("%s", inter->xprc);
				stmp.Trim();
				inter->name = stmp;
			}
			else
				inter->name = m_page->GetCodeName(inter->code);

			inter->bookrgb = CString(item.bookrgb, sizeof(item.bookrgb)).TrimRight();
		});

		m_grid->MessageToGrid(WM_VSCROLL, SB_TOP);
		loadGrid();
	}
	break;
	default:
		break;
	}
}