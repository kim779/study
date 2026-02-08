#include "stdafx.h"
#include "IBXXXX08.h"
#include "dlgHistory.h"
#include "../../h/memDC.h"
#include "../../h/logio.h"
#include "libDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct _grInfo grInfoHistory[] = {
	{ "코드",	60,	"A002",	GVFM_CENTER,	0,		"",	0},
	{ "종목명",	110,	"A003",	GVFM_LEFT,	0,		"",	0},

	{ "설정가",	54,	"A008",	GVFM_RIGHT,	GVAT_COMMA|GVAT_HDRMERGE,	"2|현재가",	0},
	{ "",		16,	"A009",	GVFM_CENTER,	0,		"",	0},
	{ "",		22,	"A010",	GVFM_CENTER,	0,		"",	0},
	{ "설정가",	54,	"A011",	GVFM_RIGHT,	GVAT_COMMA|GVAT_HDRMERGE,	"5|전일대비",	0},
	{ "",		16,	"A012",	GVFM_CENTER,	0,		"",	0},
	{ "",		22,	"A013",	GVFM_CENTER,	0,		"",	0},
	{ "설정가",	54,	"A014",	GVFM_RIGHT,	GVAT_COMMA|GVAT_HDRMERGE,	"8|매도호가",	0},
	{ "",		16,	"A015",	GVFM_CENTER,	0,		"",	0},
	{ "",		22,	"A016",	GVFM_CENTER,	0,		"",	0},
	{ "설정가",	54,	"A017",	GVFM_RIGHT,	GVAT_COMMA|GVAT_HDRMERGE,	"11|매수호가",	0},
	{ "",		16,	"A018",	GVFM_CENTER,	0,		"",	0},
	{ "",		22,	"A019",	GVFM_CENTER,	0,		"",	0},
	{ "설정가",	54,	"A020",	GVFM_RIGHT,	GVAT_HDRMERGE,			"14|등락률",	0},
	{ "",		16,	"A021",	GVFM_CENTER,	0,		"",	0},
	{ "",		22,	"A022",	GVFM_CENTER,	0,		"",	0},
	{ "설정가",	70,	"A023",	GVFM_RIGHT,	GVAT_COMMA|GVAT_HDRMERGE,	"17|거래량",	0},
	{ "",		16,	"A024",	GVFM_CENTER,	0,		"",	0},

	{ "주문",	44,	"A004",	GVFM_CENTER,	GVAT_HDRMERGE,		"19|확인창",	0},
	{ "매매",	44,	"A005",	GVFM_CENTER,	GVAT_HDRMERGE,		"20|관심",	0},
	{ "수량",	46,	"A006",	GVFM_CENTER,	GVAT_HDRMERGE,		"21|소리",	0},
	{ "단가",	60,	"A007",	GVFM_CENTER,	GVAT_HDRMERGE,		"22|주문",	0},

};

enum { IHCOL_CODE = 0, IHCOL_NAME,
	IHCOL_CURR, IHCOL_CURREX, IHCOL_CURREX2, 
	IHCOL_DIFF, IHCOL_DIFFEX, IHCOL_DIFFEX2, 
	IHCOL_MDGA, IHCOL_MDGAEX, IHCOL_MDGAEX2, 
	IHCOL_MSGA, IHCOL_MSGAEX, IHCOL_MSGAEX2, 
	IHCOL_RATE, IHCOL_RATEEX, IHCOL_RATEEX2, 
	IHCOL_GVOL, IHCOL_GVOLEX, 
	IHCOL_WINDOW, IHCOL_INTER, IHCOL_SOUND, IHCOL_JUMUN, 
};

enum { IHCOL_DATE = 1, IHCOL_JMGB = 19, IHCOL_MMGB, IHCOL_JMSU, IHCOL_JMGA };
	

CDlgHistory::CDlgHistory(CWnd* pParent, CWnd* pView)
	: CDialog(CDlgHistory::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgHistory)
	//}}AFX_DATA_INIT

	m_pParent = pParent;
	m_pView = pView;

	initConstruct();
}

void CDlgHistory::initConstruct()
{
	_vHistory.clear();
}

void CDlgHistory::destroy()
{
	_vHistory.clear();
}

void CDlgHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHistory)
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHistory, CDialog)
	//{{AFX_MSG_MAP(CDlgHistory)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDlgHistory::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_clrBk = _GetIndexColor(m_pView, CLRBG);
	m_sHisFile.Format("%s/%s/%s/hiscatch.log", m_root, USRDIR, m_user);
	initGrid();

	setImgBtn(&m_btnCancel, 4, TRUE);

	//loadHistory();
	updateGrid();
	return TRUE;
}

void CDlgHistory::OnPaint() 
{
	CPaintDC dc(this);
	xxx::CMemDC memdc(&dc);
	CRect rc;
	GetClientRect(&rc);
	memdc.SetBkMode(TRANSPARENT);
	rc.SetRect(0, 0, rc.Width(), rc.Height() - 1);
	memdc.FillSolidRect(rc, m_clrBk);
}

void CDlgHistory::OnDestroy() 
{
	destroy();
	CDialog::OnDestroy();
}

void CDlgHistory::SetData(int nrec, char* buf)
{
	_lsgrid *lsgrid = nullptr;
	struct	_hisinfo* hisinfo = nullptr;
	const int bufferSize = 1024;
	for (int ii = 0; ii < nrec; ii++)
	{
		auto &hisinfo = _vHistory.emplace_back(std::make_unique<_hisinfo>());
		lsgrid = (_lsgrid *)buf;
		memcpy(hisinfo.get(), lsgrid->logm, sz_hisinfo);
		buf += sz_lsgrid;
	}
}

void CDlgHistory::initGrid()
{
	CRect rc;
	GetDlgItem(IDC_POSHISTORY)->GetWindowRect(rc);
	ScreenToClient(rc);
	m_pGrid = std::make_unique<CExGrid>();
	m_pGrid->Create(rc, this, IDC_GRHISTORY, GVSC_BOTH, 0, WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL);
	initGrid(m_pGrid.get(), IDC_GRHISTORY);
}

void CDlgHistory::initGrid(CExGrid* pGrid, UINT idc)
{
	_GVITEM gvitem;
	COLORREF gridHead{};
	CFont* pFont = nullptr;
	int ii = 0;
	int colCnt = 0;
	
	gridHead = _GetIndexColor(m_pView, CLRGRHB);

	gvitem.row = 0;
	
	
	gvitem.font = m_lf;
	gvitem.format = GVFM_HEADER;
	gvitem.fgcol = _GetIndexColor(m_pView, CLRGRHF);
	gvitem.bkcol = gridHead;

	switch (idc)
	{
	case IDC_GRHISTORY:
		pGrid->SetGridFocusLine(FALSE, TRUE);
		colCnt = sizeof(grInfoHistory)/sizeof(_grInfo);
		pGrid->Initial(1, colCnt, 1, 0);
		for (ii = 0; ii < colCnt; ii++)
		{
			gvitem.symbol = grInfoHistory[ii].symbols;
			gvitem.text = grInfoHistory[ii].heads;
			gvitem.attr = grInfoHistory[ii].attr;
			gvitem.attr &= ~GVAT_COMMA;
			
			gvitem.misc = grInfoHistory[ii].defaultVal;
			gvitem.col = ii;
			gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR|GVMK_ATTR|GVMK_MISC;
			if (gvitem.attr & GVAT_HDRMERGE)
			{
				gvitem.mask |= GVMK_PARAM;
				gvitem.param = gridHead;
			}
			pGrid->SetItem(&gvitem);
			pGrid->SetColumnWidth(ii, grInfoHistory[ii].width);
		}
		break;
	}
	
	pGrid->SetRowHeight(0, GRIDROW_HEIGHT*2);
	pGrid->SetRowColResize(FALSE, FALSE);
	pGrid->SetBKSelColor(_GetIndexColor(m_pView, CLRGRSEL));
	pGrid->SetGridColor(_GetIndexColor(m_pView, CLRGRLINE));
	pGrid->SetBkColor(_GetIndexColor(m_pView, CLRGRROWB1));
	pGrid->Adjust();
}

bool CDlgHistory::loadHistory()
{
	if (!m_pGrid)	return false;

	m_pGrid->Clear();
	m_pGrid->SetRowCount(1);
	
	CFileFind finder;
	if (!finder.FindFile(m_sHisFile))
		return false;

	CString	codeS;
	struct	_hisinfo* hisinfo = nullptr;

	CFile	fileH(m_sHisFile, CFile::modeRead);
	for (int ii = 0; ii < maxJONGMOK; ii++)
	{
		auto& hisinfo = _vHistory.emplace_back(std::make_unique<_hisinfo>());
		const int readL = fileH.Read(hisinfo.get(), sz_hisinfo);
		if (readL < sz_hisinfo)
			_vHistory.erase((_vHistory.rbegin() + 1).base());
	}
	fileH.Close();
	return true;
}

void CDlgHistory::updateGrid()
{
	int row = 0;
	for (auto& hisinfo : _vHistory)
	{
		row = m_pGrid->GetRowCount();
		insertGrid(m_pGrid.get(), IDC_GRHISTORY, row);
		insertGrid(m_pGrid.get(), IDC_GRHISTORY, row+1);
		updateHistory(row, hisinfo.get());
	}
}

int CDlgHistory::insertGrid(CExGrid* pGrid, UINT idc, int row)
{
	_GVITEM gvitem;
	CString	 tmpS, statS;

	gvitem.font   = m_lf;
	gvitem.mask   = GVMK_FONT|GVMK_TEXT|GVMK_FORMAT|GVMK_PARAM|GVMK_ATTR|GVMK_BKCOLOR|GVMK_FGCOLOR;

	if (row % 4 == 1 || row % 4 == 2)
		gvitem.bkcol = _GetIndexColor(m_pView, CLRGRROWB1);
	else
		gvitem.bkcol = _GetIndexColor(m_pView, CLRGRROWB2);
	gvitem.fgcol  = _GetIndexColor(m_pView, CLRGRROWF);

	row = pGrid->InsertRow(" ", row);
	pGrid->SetRowHeight(row, GRIDROW_HEIGHT);
	gvitem.row    = row;

	int col = 0;
	switch (idc)
	{
	case IDC_GRHISTORY:
		for (col = 0; col < sizeof(grInfoHistory)/sizeof(_grInfo); col++)
		{
			gvitem.text = "";
			gvitem.col = col;
			gvitem.format = grInfoHistory[col].format;
			gvitem.attr = grInfoHistory[col].attr;
			gvitem.attr &= ~GVAT_HDRMERGE;
			gvitem.attr |= GVAT_ONLYROW;
			pGrid->SetItem(&gvitem);
		}
		break;
	}
	return row;
}


void CDlgHistory::updateHistory(int row, _hisinfo *hisinfo)
{
	_codeinfo	*codeinfo = (struct _codeinfo *)&hisinfo->codeinfo;
	CString tmpS, tmpS_;

	tmpS = _GetString(hisinfo->date, sizeof(hisinfo->date));		// 날짜
	tmpS_.Format("%s/%s/%s %s:%s:%s", tmpS.Mid(2, 2), tmpS.Mid(4, 2), tmpS.Mid(6, 2), tmpS.Mid(8, 2), tmpS.Mid(10, 2), tmpS.Mid(12, 2));
	m_pGrid->SetItemText(row+1, IHCOL_DATE, tmpS_);

	tmpS = _GetString(codeinfo->code, sizeof(codeinfo->code));		// 코드
	m_pGrid->SetItemText(row, IHCOL_CODE, tmpS);

	tmpS = _GetString(codeinfo->name, sizeof(codeinfo->name));		// 이름
	m_pGrid->SetItemText(row, IHCOL_NAME, tmpS);

	if (codeinfo->a_kind[0] == ' ')
		codeinfo->a_kind[0] = 0x00;

	if (codeinfo->a_kind[0] & 0x01)						// 확인창
		m_pGrid->SetItemText(row, IHCOL_WINDOW, "Y");
	if (codeinfo->a_kind[0] & 0x02)						// 관심종목신호적용
		m_pGrid->SetItemText(row, IHCOL_INTER, "Y");
	if (codeinfo->a_kind[0] & 0x04)						// 소리적용
		m_pGrid->SetItemText(row, IHCOL_SOUND, "Y");
	if (codeinfo->o_run[0] == '1')						
	{
		m_pGrid->SetItemText(row, IHCOL_JUMUN, "Y");			// 주문확인

		if (codeinfo->o_kind[0] == '1')					// 주문구분
		{
			m_pGrid->SetItemText(row+1, IHCOL_JMGB, "매수");
			m_pGrid->SetItemFgColor(row+1, IHCOL_JMGB, _GetIndexColor(m_pView, CLRMSF));
		}
		else
		{
			m_pGrid->SetItemText(row+1, IHCOL_JMGB, "매도");
			m_pGrid->SetItemFgColor(row+1, IHCOL_JMGB, _GetIndexColor(m_pView, CLRMDF));
		}
		if (codeinfo->o_mmgb[0] == '1')					// 매매구분
			m_pGrid->SetItemText(row+1, IHCOL_MMGB, "시장가");
		else
			m_pGrid->SetItemText(row+1, IHCOL_MMGB, "지정가");

		tmpS = _GetString(codeinfo->o_num, sizeof(codeinfo->o_num));	// 주문수량
		m_pGrid->SetItemText(row+1, IHCOL_JMSU, _GetFormatData(FK_COMMA, tmpS));
		m_pGrid->SetItemFormat(row+1, IHCOL_JMSU, GVFM_RIGHT);

		tmpS = _GetString(codeinfo->o_prc, sizeof(codeinfo->o_prc));	// 주문가
		m_pGrid->SetItemText(row+1, IHCOL_JMGA, _GetFormatData(FK_COMMA, tmpS));
		m_pGrid->SetItemFormat(row+1, IHCOL_JMGA, GVFM_RIGHT);
	}

	for (int ii = 0, col = IHCOL_CURR; ii < 6; ii++)
	{
		if (codeinfo->alarm[ii].kind[0] > '0')
		{
			tmpS = _GetString(codeinfo->alarm[ii].aval, sizeof(codeinfo->alarm[ii].aval));
			m_pGrid->SetItemText(row+1, col, tmpS);

			tmpS = _GetString(hisinfo->gval[ii], sizeof(hisinfo->gval[ii]));
			m_pGrid->SetItemText(row, col, tmpS);
			
			if (hisinfo->chk[ii] == '1')
				tmpS = "T";
			else
				tmpS = "F";
			m_pGrid->SetItemText(row, col+1, tmpS);

			if (!tmpS.IsEmpty())
			{
				// 감시조건 '0':이상, '1':이하
				if (codeinfo->alarm[ii].cond[0] == '1')	
				{
					tmpS = "<=";
				}
				else
				{
					tmpS = ">=";
				}
				m_pGrid->SetItemText(row+1, col+1, tmpS);

				if (ii != 0)
				{
					// 감시연산 '0':AND,  '1':OR
					if (codeinfo->alarm[ii].oper[0] == '1')
					{
						tmpS = "OR";
					}
					else
					{
						tmpS = "AND";
					}
					m_pGrid->SetItemText(row+1, col-1, tmpS);
				}
			}
		}
		col += 3;
	}
}


HBRUSH CDlgHistory::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC1:
		hbr = (HBRUSH)_GetAxBrush(m_pView, m_clrBk)->operator HBRUSH();
		break;
	}
	pDC->SetBkMode(TRANSPARENT);
	return hbr;
}


void CDlgHistory::setImgBtn(CImgBtn *pButton, int nLength, BOOL bImageFit)
{
	CString dirS;
	dirS.Format("%s\\image\\", m_root);

	CString sImgDir = "";
	sImgDir.Format("%s%dbtn", dirS, nLength);

	pButton->m_hBitmap    = _GetAxBitmap(m_pView, sImgDir + ".bmp")->operator HBITMAP();
	pButton->m_hBitmap_dn = _GetAxBitmap(m_pView, sImgDir + "_dn.bmp")->operator HBITMAP();
	pButton->m_hBitmap_hv = _GetAxBitmap(m_pView, sImgDir + "_en.bmp")->operator HBITMAP();
	if (bImageFit)
		pButton->FitImageSize();
}
