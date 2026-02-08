// FOMultiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FOMultiSel.h"
#include "FOMultiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define STANDARDNUM	11
/////////////////////////////////////////////////////////////////////////////
// CFOMultiDlg dialog


CFOMultiDlg::CFOMultiDlg(LPCTSTR root, CWnd* pParent /*=NULL*/)
	: CDialog(CFOMultiDlg::IDD, pParent), m_root(root)
{
	//{{AFX_DATA_INIT(CFOMultiDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_igubn = K200;
	m_oidx  = 0;
}


void CFOMultiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFOMultiDlg)
	DDX_Control(pDX, IDC_BTN_MINI_KOSPI200, m_ctlButtonMiniKospi200);
	DDX_Control(pDX, IDC_BTN_KOSPI200, m_ctlButtonKospi200);
	DDX_Control(pDX, IDC_BTN_WEEK, m_ctlButtonWeek);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFOMultiDlg, CDialog)
	//{{AFX_MSG_MAP(CFOMultiDlg)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_BN_CLICKED(IDC_RADIO_MON1, OnRadioMon1)
	ON_BN_CLICKED(IDC_RADIO_MON2, OnRadioMon2)
	ON_BN_CLICKED(IDC_RADIO_MON3, OnRadioMon3)
	ON_BN_CLICKED(IDC_RADIO_MON4, OnRadioMon4)
	ON_BN_CLICKED(IDC_RADIO_MON5, OnRadioMon5)
	ON_BN_CLICKED(IDC_RADIO_MON6, OnRadioMon6)
	ON_BN_CLICKED(IDC_RADIO_MON7, OnRadioMon7)
	ON_BN_CLICKED(IDC_RADIO_MON8, OnRadioMon8)
	ON_BN_CLICKED(IDC_RADIO_MON9, OnRadioMon9)
	ON_BN_CLICKED(IDC_RADIO_MON10, OnRadioMon10)
	ON_BN_CLICKED(IDC_RADIO_MON11, OnRadioMon11)
	ON_BN_CLICKED(IDC_BTN_KOSPI200, OnButtonKospi200)
	ON_BN_CLICKED(IDC_BTN_MINI_KOSPI200, OnButtonMiniKospi200)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_BTN_WEEK, OnBtnWeek)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVNM_LMOUSEUP,	IDC_FGRID, OnFGridLeftUp)
	ON_NOTIFY(GVNM_ENDEDIT,		IDC_FGRID, OnFGridCellChanged)
	ON_NOTIFY(GVNM_SELCHANGED,	IDC_FGRID, OnFGridCellChanged)
	ON_NOTIFY(GVNM_ENDRETURN,	IDC_FGRID, OnFGridCellChanged)
	ON_NOTIFY(GVNM_ENDTABKEY,	IDC_FGRID, OnFGridCellChanged)
	ON_NOTIFY(GVNM_LMOUSEUP,	IDC_OGRID, OnOGridLeftUp)
	ON_NOTIFY(GVNM_ENDEDIT,		IDC_OGRID, OnOGridCellChanged)
	ON_NOTIFY(GVNM_SELCHANGED,	IDC_OGRID, OnOGridCellChanged)
	ON_NOTIFY(GVNM_ENDRETURN,	IDC_OGRID, OnOGridCellChanged)
	ON_NOTIFY(GVNM_ENDTABKEY,	IDC_OGRID, OnOGridCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFOMultiDlg message handlers

void CFOMultiDlg::loadOJCode()
{
	m_month.clear();
	m_ojcodes.clear();

	CFile	file;
	CString path;
	
	path = m_root + "\\tab\\opcode.dat";
	
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{	
		MessageBox("파일이 존재하지 않습니다.");
		return;
	}

	int	size = sizeof(struct ojcodh);
	int	len = (int)file.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(len+1);
	const	struct	ojcodh*	pOJCodh = (struct ojcodh *)pWb.get();
	struct	ojcode*	pOJCode = (struct ojcode*)&pWb[size];

	if (file.Read(pWb.get(), len) != len)
	{
		file.Close();
		pWb.reset();
		return;
	}
	file.Close();

	for (int jj = 0; jj < STANDARDNUM; jj++)
	{
		m_month.push_back(CString(pOJCodh->cjym[jj], 6).Right(4));
	}
	
	size = (len - size) / sizeof(struct ojcode);	
	len  = sizeof(struct ojcode);
	for (int ii = 0; ii < size; ii++, pOJCode++)
	{
		m_ojcodes.push_back(*pOJCode);
	}
	std::reverse(m_ojcodes.begin(), m_ojcodes.end());
	pWb.reset();
}

void CFOMultiDlg::loadWOJCode()
{
	m_month.clear();
	m_ojcodes.clear();
	
	CFile	file;
	CString path;
	
	path = m_root + "\\tab\\wocode.dat";
	
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{	
		MessageBox("파일이 존재하지 않습니다.");
		return;
	}

	int	size = sizeof(struct ojcodh);
	int	len  = (int)file.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(len + 1);
	const	struct	ojcodh*	pOJCodh = (struct ojcodh*)pWb.get();
	struct	ojcode*	pOJCode = (struct ojcode*)&pWb[size];

	if (file.Read(pWb.get(), len) != len)
	{
		file.Close();
		pWb.reset();
		return;
	}
	file.Close();

	for (int jj = 0; jj < STANDARDNUM; jj++)
	{
		m_month.push_back(CString(pOJCodh->cjym[jj], 6).Right(4));
	}

	size = (len - size) / sizeof(struct ojcode);
	len  = sizeof(struct ojcode);
	for (int ii = 0; ii < size; ii++, pOJCode++)
	{
		m_ojcodes.push_back(*pOJCode);
	}
	std::reverse(m_ojcodes.begin(), m_ojcodes.end());
	pWb.reset();
}

void CFOMultiDlg::loadFJCode()
{
	m_fjcodes.clear();

	CFile	file;
	CString path;

	path = m_root + "\\tab\\fjcode.dat";
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{
		MessageBox("파일이 존재하지 않습니다.");
		return;
	}

	int	size = (int)file.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);

	if (file.Read(pWb.get(), size) != size)
	{
		file.Close();
		pWb.reset();
		return;
	}
	file.Close();

	FCODE	fcode{};
	struct  fjcode*  FJCode = (struct fjcode *)pWb.get();
	size /= sizeof(struct fjcode);
	for (int ii = 0; ii < size; ii++, FJCode++)
	{
		fcode.code = CString(FJCode->cod2, FCodeLen);
		fcode.name = CString(FJCode->hnam, FNameLen);
		fcode.mchk = FJCode->mchk;
		m_fjcodes.push_back(fcode);
	}
	pWb.reset();
}

void CFOMultiDlg::loadMOCode()
{
	m_month.clear();
	m_ojcodes.clear();
	
	CFile	file;
	CString path;
	
	path = m_root + "\\tab\\mocode.dat";
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{	
		MessageBox("파일이 존재하지 않습니다.");
		return;
	}

	int	size = sizeof(struct ojcodh);
	int	len  = (int)file.GetLength();
	std::unique_ptr<char[]>	pWb = std::make_unique<char[]>(len+1);
	const	struct	ojcodh*	pOJCodh = (struct ojcodh *)pWb.get();
	struct	ojcode* pOJCode = (struct ojcode*)&pWb[size];;

	if (file.Read(pWb.get(), len) != len)
	{
		file.Close();
		pWb.reset();
		return;
	}
	file.Close();

	CString	code;
	for (int jj = 0; jj < STANDARDNUM; jj++)
	{
		code = CString(pOJCodh->cjym[jj], 6).Right(4);
		if (code == "0000")	//2015.07.06 KSJ 0000월물이면 뺀다
			continue;

		m_month.push_back(code);
	}

	size = (len - size) / sizeof(struct ojcode);
	len  = sizeof(struct ojcode);
	for (int ii = 0; ii < size; ii++, pOJCode++)
	{
		m_ojcodes.push_back(*pOJCode);
	}
	std::reverse(m_ojcodes.begin(), m_ojcodes.end());
	pWb.reset();
}

void CFOMultiDlg::loadMFCode()
{
	m_fjcodes.clear();
	
	CFile	file;
	CString path;

	path = m_root + "\\tab\\mfcode.dat";
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
	{
		MessageBox("파일이 존재하지 않습니다.");
		return;
	}

	int	size = (int)file.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(size + 1);

	if (file.Read(pWb.get(), size) != size)
	{
		file.Close();
		pWb.reset();
		return;
	}
	file.Close();

	FCODE	fcode{};
	struct  fjcode*  FJCode = (struct fjcode *)pWb.get();
	size /= sizeof(struct fjcode);
	for (int ii = 0; ii < size; ii++, FJCode++)
	{
		fcode.code = CString(FJCode->cod2, FCodeLen);
		fcode.name = CString(FJCode->hnam, FNameLen);
		fcode.mchk = FJCode->mchk;
		m_fjcodes.push_back(fcode);
	}
	pWb.reset();
}

int CFOMultiDlg::GetOptionCount(int midx)
{
	int	cnt = 0;
	for (int ii = 0; ii < (int)m_ojcodes.size(); ii++)
	{
		cnt += (m_ojcodes[ii].put[midx].yorn == '1' ) ? 1 : 0;
	}
	return cnt;
}

void CFOMultiDlg::InitFGrid()
{
	m_fgrid.Clear();

	if (!::IsWindow(m_fgrid.GetSafeHwnd()))
	{
		CRect stRect, gridRect;
		
		GetDlgItem(IDC_STATIC_F)->GetWindowRect(stRect);
		ScreenToClient(stRect);
		
		gridRect.top	= stRect.bottom+ 5;
		gridRect.bottom	= stRect.top + 128;
		gridRect.left	= stRect.left;
		gridRect.right	= stRect.right;
		m_fgrid.Create(gridRect, this, IDC_FGRID, GVSC_VERT, GVDD_NONE);

	}
	
	GVITEM	item{};
	LOGFONT	lf{};
	const	int	iColumns = 6;
	const	COLORREF defBgCol = RGB(0xE3,0xE3,0xE3);
	const	COLORREF defFgCol = RGB(0x00,0x00,0x00);
	const	COLORREF colWhite = RGB(0xFF,0xFF,0xFF);
	const	COLORREF defBlue  = RGB(0x02,0x01,0xFE);
	const	COLORREF defRed   = RGB(0xFF,0x00,0x00);
	
	if (m_igubn == K200MINI)	//2015.07.08 KSJ  미니 코스피200 에서는 한칸이 적다.
		m_fgrid.Initial(7, iColumns, 1, 0);
	else 
		m_fgrid.Initial(8, iColumns, 1, 0);

	
	GetFont()->GetLogFont(&lf);
	lf.lfHeight = 90;
	const	int	cWidth[] = {30, 57, 120, 40, 40, 44};
	const	int	cAttr[] = { GVAT_CHECK, GVAT_TEXT, GVAT_TEXT, GVAT_TEXT, GVAT_TEXT, GVAT_EDIT|GVAT_COMMA};
	const	int	cFormat[] = { GVFM_CENTER, GVFM_LEFT, GVFM_LEFT, GVFM_CENTER, GVFM_CENTER, GVFM_RIGHT};
	const	char*	const	cCaption[] = { "선택", "코드", "종목명", "매도", "매수", "수량"};
	const	COLORREF cBkColors[] = { defBgCol, defBgCol, defBgCol, defBlue, defRed, defBgCol };
	COLORREF cFgColors[] = { defFgCol, defFgCol, defFgCol, colWhite, colWhite, defFgCol };
	
	// Header Setting
	for (int ii = 0; ii < iColumns; ii++)
	{
		item.state = 0;
		item.row = 0;
		item.col = ii;
		item.state = 0;
		item.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		item.format = GVFM_HEADER;
		item.attr = 0;
		CopyMemory(&item.font, &lf, sizeof(LOGFONT));
		item.fgcol = cFgColors[ii];
		item.bkcol = cBkColors[ii];
		item.text = cCaption[ii];	
		m_fgrid.SetColumnWidth(ii, cWidth[ii]);
		m_fgrid.SetItem(&item);
	}
	
	// Column Setting
	cFgColors[3] = defBlue;
	cFgColors[4] = defRed;
	for (int ii = 0; ii < iColumns; ii++)
	{
		item.state = 0;
		item.mask = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR;
		item.format = cFormat[ii];
		item.attr = cAttr[ii];
		item.fgcol = cFgColors[ii];
		item.bkcol = colWhite;
		m_fgrid.SetBKSelColor(colWhite);
		m_fgrid.SetFGSelColor(defFgCol);
		m_fgrid.SetColumnItems(ii, &item);
	}
	
	// Data Setting
	for (int ii = 0; ii < m_fgrid.GetRowCount(); ii++)
	{
		m_fgrid.SetRowHeight(ii, 20);
		if (ii > 0)
		{
			m_fgrid.SetItemText(ii, 1, m_fjcodes[ii-1].code);
			m_fgrid.SetItemText(ii, 2, m_fjcodes[ii-1].name);
			m_fgrid.SetItemText(ii, 3, CHK_MARK);
		}
	}
	
	m_fgrid.SetRowColResize(FALSE, FALSE);
	m_fgrid.SetGridFocusLine(FALSE, FALSE);
	m_fgrid.Adjust();
}

void CFOMultiDlg::InitOGrid(int midx)
{
// 	m_ogrid.Clear();
//	if (m_igubn == K200WEEK)
//		midx = 5;

	if (!::IsWindow(m_ogrid.GetSafeHwnd()))
	{
		CRect stRect, stRect2, dlgRect, gridRect;
		GetClientRect(dlgRect);
		GetDlgItem(IDC_STATIC_O)->GetWindowRect(stRect);
		ScreenToClient(stRect);
		GetDlgItem(IDC_STATIC_MON)->GetWindowRect(stRect2);
		ScreenToClient(stRect2);
		
		gridRect.top	= stRect2.bottom + 28;
		gridRect.bottom	= dlgRect.bottom - 40;
		gridRect.left	= stRect.left;
		gridRect.right	= stRect.right;
		m_ogrid.Create(gridRect, this, IDC_OGRID, GVSC_VERT, GVDD_NONE);
	}
	
	GVITEM	item{};
	LOGFONT	lf{};
	const	int	iColumns = 11;
	const	COLORREF defBgCol = RGB(0xE3,0xE3,0xE3);
	const	COLORREF defFgCol = RGB(0x00,0x00,0x00);
	const	COLORREF colWhite = RGB(0xFF,0xFF,0xFF);
	const	COLORREF defBlue  = RGB(0x02,0x01,0xFE);
	const	COLORREF defRed   = RGB(0xFF,0x00,0x00);
	
	m_ogrid.Initial(GetOptionCount(midx)+1, iColumns, 1, 0);
	GetFont()->GetLogFont(&lf);
	lf.lfHeight = 90;

	const	int	cWidth[] = {30, 55, 30, 30, 0, 45, 0, 30, 30, 55, 30};
	const	int	cAttr[] = {	GVAT_CHECK, GVAT_EDIT|GVAT_COMMA, GVAT_TEXT, GVAT_TEXT,		// left
					GVAT_HIDDEN|GVAT_TEXT, GVAT_TEXT, GVAT_HIDDEN|GVAT_TEXT,	// code, amt, code
					GVAT_TEXT, GVAT_TEXT, GVAT_EDIT|GVAT_COMMA, GVAT_CHECK};	// right
	const	int	cFormat[] = {	GVFM_CENTER, GVFM_RIGHT, GVFM_CENTER, GVFM_CENTER,
						GVFM_CENTER, GVFM_CENTER, GVFM_CENTER,
						GVFM_CENTER, GVFM_CENTER, GVFM_RIGHT, GVFM_CENTER };
	const	char*	const	cCaption[] = {"선택", "수량", "매도", "매수", "종목코드", "행사가", "종목코드", "매수", "매도", "수량", "선택"};
	const	COLORREF cBkColors[] = { defBgCol, defBgCol, defBlue, defRed, defBgCol, defBgCol, defBgCol, defRed, defBlue, defBgCol, defBgCol };
	COLORREF cFgColors[] = { defFgCol, defFgCol, colWhite, colWhite, defFgCol, defFgCol, defFgCol, colWhite, colWhite, defFgCol, defFgCol };
	const	COLORREF cAtmColor = RGB(0xB4,0xFF,0xFF);

	// Header Setting
	for (int ii = 0; ii < iColumns; ii++)
	{
		item.state = 0;
		item.row = 0;
		item.col = ii;
		item.state = 0;
		item.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_MISC;
		item.format = GVFM_HEADER;
		item.attr = 0;
		CopyMemory(&item.font, &lf, sizeof(LOGFONT));
		item.fgcol = cFgColors[ii];
		item.bkcol = cBkColors[ii];
		item.text = cCaption[ii];	
		m_ogrid.SetColumnWidth(ii, cWidth[ii]);
		m_ogrid.SetItem(&item);
	}
	
	// Column Setting
	cFgColors[2] = cFgColors[8] = defBlue;
	cFgColors[3] = cFgColors[7] = defRed;
	cFgColors[4] = cFgColors[6] = colWhite;
	cFgColors[5] = defFgCol;
	for (int ii = 0; ii < iColumns; ii++)
	{
		item.state = 0;
		item.mask = GVMK_FORMAT|GVMK_FONT|GVMK_ATTR|GVMK_FGCOLOR|GVMK_BKCOLOR;
		item.format = cFormat[ii];
		item.attr = cAttr[ii];
		item.fgcol = cFgColors[ii];
		if (ii != 5)
			item.bkcol = colWhite;
		else
			item.bkcol = cAtmColor;
		m_ogrid.SetBKSelColor(colWhite);
		m_ogrid.SetFGSelColor(defFgCol);
		m_ogrid.SetColumnItems(ii, &item);
	}
	
	// Data Setting
	for (int ii = 0; ii < m_ogrid.GetRowCount(); ii++)
		m_ogrid.SetRowHeight(ii, 20);

	CString	code, price;
	for (int ii = 0, gidx = 1; ii < (int)m_ojcodes.size(); ii++)
	{
		if (m_ojcodes[ii].put[midx].yorn=='1')
		{
			price = CString(m_ojcodes[ii].price, OPriceLen);
			price.Insert(3,".");

			code = CString(m_ojcodes[ii].call[STANDARDNUM - midx - 1].cod2, OCodeLen - 1); code.TrimRight();
			m_ogrid.SetItemText(gidx, 4, code);
			m_ogrid.SetItemText(gidx, 5, (LPCTSTR)price);
			code = CString(m_ojcodes[ii].put[midx].cod2, OCodeLen -1); code.TrimRight();
			m_ogrid.SetItemText(gidx, 6, code);
			m_ogrid.SetItemText(gidx, 2, CHK_MARK);
			m_ogrid.SetItemText(gidx, 8, CHK_MARK);

			if (m_ojcodes[ii].atmg==1)
			{
				m_ogrid.SetItemBkColor(gidx, 5, RGB(0xFF,0xFF,0xB4), true);
				m_ogrid.SetSelectRow(gidx+4);
			}
			else
				m_ogrid.SetItemBkColor(gidx, 5, defBgCol, true);
			++gidx;
		}
	}
	
	m_ogrid.SetRowColResize(FALSE, FALSE);
	m_ogrid.SetGridFocusLine(FALSE, FALSE);
	m_ogrid.Adjust();
	
	m_ogrid.Refresh();
}

void CFOMultiDlg::InitData()
{
	CString stmp;

	m_ctlButtonKospi200.SetFlat(TRUE);
	m_ctlButtonKospi200.m_bNoFocus = TRUE;
	m_ctlButtonKospi200.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
	m_ctlButtonKospi200.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COR_BACK);
	m_ctlButtonKospi200.SetColor(CButtonST::BTNST_COLOR_BK_IN, COR_BACK);
	
	//2015.06.30 KSJ 미니 코스피 200 추가
	m_ctlButtonMiniKospi200.SetFlat(TRUE);
	m_ctlButtonMiniKospi200.m_bNoFocus = TRUE;
	m_ctlButtonMiniKospi200.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
	m_ctlButtonMiniKospi200.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COR_BACK);
	m_ctlButtonMiniKospi200.SetColor(CButtonST::BTNST_COLOR_BK_IN, COR_BACK);
	

	m_ctlButtonWeek.SetFlat(TRUE);
	m_ctlButtonWeek.m_bNoFocus = TRUE;
	m_ctlButtonWeek.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COR_BACK);
	m_ctlButtonWeek.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COR_BACK);
	m_ctlButtonWeek.SetColor(CButtonST::BTNST_COLOR_BK_IN, COR_BACK);


	if (m_igubn == K200MINI)
	{
		loadMFCode();
		loadMOCode();
		m_ctlButtonKospi200.SetSelected(FALSE);
		m_ctlButtonWeek.SetSelected(FALSE);
		m_ctlButtonMiniKospi200.SetSelected(TRUE);
	}
	else if (m_igubn == K200)
	{
		loadFJCode();
		loadOJCode();
		m_ctlButtonKospi200.SetSelected(TRUE);
		m_ctlButtonWeek.SetSelected(FALSE);
		m_ctlButtonMiniKospi200.SetSelected(FALSE);
	}
	else if (m_igubn == K200WEEK)
	{
		loadFJCode();
		loadWOJCode();
		m_ctlButtonKospi200.SetSelected(FALSE);
		m_ctlButtonWeek.SetSelected(TRUE);
		m_ctlButtonMiniKospi200.SetSelected(FALSE);
	}

	/* Radio-button setting */

	for (int ii = 0; ii < 11; ii++)
	{
		GetDlgItem(IDC_RADIO_MON1 + ii)->ShowWindow(FALSE);
		((CButton*)GetDlgItem(IDC_RADIO_MON1 + ii))->SetCheck(FALSE);
	}

	for (int ii = 0; ii < (int)m_month.size(); ii++)  //weekly
	{
		if (m_month[m_month.size()-(ii+1)] != "0000")
		{
			GetDlgItem(IDC_RADIO_MON1 + ii)->SetWindowText(m_month[m_month.size()-(ii+1)]+"월물");
			GetDlgItem(IDC_RADIO_MON1 + ii)->ShowWindow(TRUE);
		}
	}

	((CButton*)GetDlgItem(IDC_RADIO_MON1))->SetCheck(TRUE);

	/* grid setting */
	InitFGrid();
	InitOGrid(0);
}

BOOL CFOMultiDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitData();
	return TRUE;
}

void CFOMultiDlg::OnOk() 
{
	CDialog::OnOK();
}

void CFOMultiDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CFOMultiDlg::OnFGridLeftUp(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *item = (NM_GVITEM*)pNMHDR;
	int	row = 0;

	if (item->row == 0)	// Header Click
	{
		switch (item->col)
		{
		case 3:
			for (row = 1; row < m_fgrid.GetRowCount(); ++row)
			{
				m_fgrid.SetItemText(row, 3, CHK_MARK);
				m_fgrid.SetItemText(row, 4, _T(""));
			}
			break;
		case 4:
			for (row = 1; row < m_fgrid.GetRowCount(); ++row)
			{
				m_fgrid.SetItemText(row, 3, _T(""));
				m_fgrid.SetItemText(row, 4, CHK_MARK);
			}
			break;
		}
	}
	else if (item->row > 0)
	{
		switch (item->col)
		{
		case 3:
			m_fgrid.SetItemText(item->row, 3, CHK_MARK);
			m_fgrid.SetItemText(item->row, 4, _T(""));
			break;
		case 4:
			m_fgrid.SetItemText(item->row, 3, _T(""));
			m_fgrid.SetItemText(item->row, 4, CHK_MARK);
			break;
		case 5:	// 수량 선택
			m_fgrid.SetFocusCellEdit(item->row, item->col, true);
			break;
		}
	}
	//*pResult = 0;
}

void CFOMultiDlg::OnFGridCellChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *item = (NM_GVITEM*)pNMHDR;

	if (item->row > 0 && item->col == 5)
	{
		CString val = m_fgrid.GetItemText(item->row, item->col);
		if (atoi((LPCSTR)val) > 0)
			m_fgrid.SetItemParam(item->row, 0, 1);
		else
			m_fgrid.SetItemText(item->row, item->col, "");
	}
	//*pResult = 0;
}

void CFOMultiDlg::OnOGridLeftUp(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *item = (NM_GVITEM*)pNMHDR;
	int	row = 0;

	if (item->row == 0)	// Header Click
	{
		switch (item->col)
		{
		case 0:	//2013.07.24 KSJ 선택 클릭시 전체선택되도록
			for (row=1; row<m_ogrid.GetRowCount(); ++row)
			{
				m_ogrid.SetItemParam(row, 0, !m_ogrid.GetItemParam(row, 0));
			}
			break;

		case 2:
			for (row=1; row<m_ogrid.GetRowCount(); ++row)
			{
				m_ogrid.SetItemText(row, 2, CHK_MARK);
				m_ogrid.SetItemText(row, 3, _T(""));
			}
			break;
		case 3:
			for (row=1; row<m_ogrid.GetRowCount(); ++row)
			{
				m_ogrid.SetItemText(row, 2, _T(""));
				m_ogrid.SetItemText(row, 3, CHK_MARK);
			}
			break;
		case 7:
			for (row=1; row<m_ogrid.GetRowCount(); ++row)
			{
				m_ogrid.SetItemText(row, 7, CHK_MARK);
				m_ogrid.SetItemText(row, 8, _T(""));
			}
			break;
		case 8:
			for (row=1; row<m_ogrid.GetRowCount(); ++row)
			{
				m_ogrid.SetItemText(row, 7, _T(""));
				m_ogrid.SetItemText(row, 8, CHK_MARK);
			}
			break;
		case 10:	//2013.07.24 KSJ 선택 클릭시 전체선택되도록
			for (row=1; row<m_ogrid.GetRowCount(); ++row)
			{
				m_ogrid.SetItemParam(row, 10, !m_ogrid.GetItemParam(row, 10));
			}
			break;

		}
	}
	else if (item->row>0)
	{
		switch (item->col)
		{
		case 2:
			m_ogrid.SetItemText(item->row, 2, CHK_MARK);
			m_ogrid.SetItemText(item->row, 3, _T(""));
			break;
		case 3:
			m_ogrid.SetItemText(item->row, 2, _T(""));
			m_ogrid.SetItemText(item->row, 3, CHK_MARK);
			break;
		case 7:
			m_ogrid.SetItemText(item->row, 7, CHK_MARK);
			m_ogrid.SetItemText(item->row, 8, _T(""));
			break;
		case 8:
			m_ogrid.SetItemText(item->row, 7, _T(""));
			m_ogrid.SetItemText(item->row, 8, CHK_MARK);
			break;
		case 1:	// 수량 선택
		case 9:
			m_ogrid.SetFocusCellEdit(item->row, item->col, true);
			break;
		}
	}
	*pResult = 0;
}

void CFOMultiDlg::OnOGridCellChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *item = (NM_GVITEM*)pNMHDR;
	
	if (item->row > 0 && (item->col == 1 || item->col == 9))
	{
		CString val = m_ogrid.GetItemText(item->row, item->col);
		if (atoi((LPCSTR)val) > 0)
		{
			switch (item->col)
			{
			case 1: m_ogrid.SetItemParam(item->row, 0, 1); break;
			case 9: m_ogrid.SetItemParam(item->row, 10, 1); break;
			}
		}
		else
			m_ogrid.SetItemText(item->row, item->col, "");
	}
	*pResult = 0;
}

void CFOMultiDlg::OnRadioMon1() 
{
	if (m_oidx!=0)
		InitOGrid((m_oidx=0));
}

void CFOMultiDlg::OnRadioMon2() 
{
	if (m_oidx!=1)
		InitOGrid((m_oidx=1));
}

void CFOMultiDlg::OnRadioMon3() 
{
	if (m_oidx!=2)
		InitOGrid((m_oidx=2));
}

void CFOMultiDlg::OnRadioMon4() 
{
	if (m_oidx!=3)
		InitOGrid((m_oidx=3));
}

void CFOMultiDlg::OnRadioMon5() 
{
	if (m_oidx!=4)
		InitOGrid((m_oidx=4));
}

void CFOMultiDlg::OnRadioMon6() 
{
	if (m_oidx!=5)
		InitOGrid((m_oidx=5));
}

void CFOMultiDlg::OnRadioMon7() 
{
	if (m_oidx!=6)
		InitOGrid((m_oidx=6));
}

void CFOMultiDlg::OnRadioMon8() 
{
	if (m_oidx!=7)
		InitOGrid((m_oidx=7));
}

void CFOMultiDlg::OnRadioMon9() 
{
	if (m_oidx!=8)
		InitOGrid((m_oidx=8));
}

void CFOMultiDlg::OnRadioMon10() 
{
	if (m_oidx!=9)
		InitOGrid((m_oidx=9));
}

void CFOMultiDlg::OnRadioMon11() 
{
	if (m_oidx!=10)
		InitOGrid((m_oidx=10));
}

CString CFOMultiDlg::GetData()
{
	int row;
	CString data = "";
	
	for (row=1; row<m_fgrid.GetRowCount(); ++row)
	{
		if (m_fgrid.GetItemParam(row, 0)==1)	// check box
		{
			if (m_fgrid.IsNumeric(m_fgrid.GetItemText(row, 5)))	// jqty edit
			{
				data += m_fgrid.GetItemText(row, 1) + DEL_COLUMN;
				data += (m_fgrid.GetItemText(row, 4) == CHK_MARK) ? "2" : "1";
				data += DEL_COLUMN;
				data += m_fgrid.GetItemText(row, 5) + DEL_ROW;
			}
		}
	}

	for (row=1; row<m_ogrid.GetRowCount(); ++row)
	{
		// call
		if (m_ogrid.GetItemParam(row, 0)==1)	// check box
		{
			if (m_ogrid.IsNumeric(m_ogrid.GetItemText(row, 1)))	// jqty edit
			{
				data += m_ogrid.GetItemText(row, 4) + DEL_COLUMN;
				data += (m_ogrid.GetItemText(row, 2) == CHK_MARK) ? "1" : "2";
				data += DEL_COLUMN;
				data += m_ogrid.GetItemText(row, 1) + DEL_ROW;
			}
		}
		// put
		if (m_ogrid.GetItemParam(row, 10)==1)	// check box
		{
			if (m_ogrid.IsNumeric(m_ogrid.GetItemText(row, 9)))	// jqty edit
			{
				data += m_ogrid.GetItemText(row, 6) + DEL_COLUMN;
				data += (m_ogrid.GetItemText(row, 7) == CHK_MARK) ? "2" : "1";
				data += DEL_COLUMN;
				data += m_ogrid.GetItemText(row, 9) + DEL_ROW;
			}
		}
	}
	data.TrimRight(DEL_ROW);

	return data;
}

void CFOMultiDlg::OnButtonKospi200() 
{
	// TODO: Add your control notification handler code here
	m_igubn = K200;
	InitData();
}

void CFOMultiDlg::OnButtonMiniKospi200() 
{
	// TODO: Add your control notification handler code here
	m_igubn = K200MINI;
	InitData();
}

void CFOMultiDlg::OnBtnWeek() 
{
	// TODO: Add your control notification handler code here
	m_igubn = K200WEEK;
	InitData();
}
