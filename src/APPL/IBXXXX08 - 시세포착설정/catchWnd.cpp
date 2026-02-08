#include "stdafx.h"
#include "IBXXXX08.h"
#include "catchWnd.h"
#include "libDefine.h"
#include "axStatic.h"
#include "../../h/axisfire.h"

#define CONTROLCATCH_CLASSNAME  "CatchPopup"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 그룹그리드
struct _grInfo grInfoAlarm[] = {
	{ "설정내역",	70,	"A001",	GVFM_LEFT,	0,	"",	0},
	{ "설정값",	60,	"A002",	GVFM_RIGHT,	0,	"",	0},
	{ "조건",	36,	"A003",	GVFM_CENTER,	0,	"",	0},
	{ "연산",	36,	"A003",	GVFM_CENTER,	0,	"",	0},
};
enum { IPCOL_KIND = 0, IPCOL_VAL, IPCOL_COND, IPCOL_OPER };

CCatchWnd::CCatchWnd(CWnd *pParent, CWnd *pView)
{
	m_pParent = pParent;
	m_pView = pView;

	initConstruct();
}

CCatchWnd::~CCatchWnd()
{
	
//	destroy();
}

void CCatchWnd::initConstruct()
{
	RegisterWindowClass();	
	m_width = 100;
}

void CCatchWnd::destroy()
{
}


BEGIN_MESSAGE_MAP(CCatchWnd, CWnd)
	//{{AFX_MSG_MAP(CCatchWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CCatchWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

void CCatchWnd::OnDestroy() 
{
	detach();
	if (m_pName)
		m_pName->DestroyWindow();
	CWnd::OnDestroy();
}

void CCatchWnd::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	Hide();
}

void CCatchWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	Hide();
	CWnd::OnLButtonUp(nFlags, point);
}

void CCatchWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	Hide();
	CWnd::OnRButtonDown(nFlags, point);
}

void CCatchWnd::SetData(_codeinfo *codeinfo)
{
	m_codeinfo = codeinfo;
}

void CCatchWnd::Show(CPoint pt)
{
	if (!m_pName || !m_pAlarm || !m_pEtc)	return;

	CString sCode;
	int height = GRIDROW_HEIGHT;
	
	sCode = CString(m_codeinfo->code, sizeof(m_codeinfo->code));
	sCode.TrimRight();
	
	CString titleS, tmpS;
	int row = 0, fkind = FK_NOT, ii = 0;

	m_pName->SetWindowText(_GetString(m_codeinfo->name, sizeof(m_codeinfo->name)));

	COLORREF clrCond = _GetIndexColor(m_pView, CLRGRROWB1);
	const COLORREF clrUp = _GetIndexColor(m_pView, CLRUP);
	const COLORREF clrDn = _GetIndexColor(m_pView, CLRDN);


	m_pAlarm->Clear();
	m_pAlarm->SetRowCount(1);
	row = 1;
	for (ii = 0; ii < MAX_ALARM; ii++)
	{
		if (sCode.GetLength() == 6)		// 주식
		{
			switch (m_codeinfo->alarm[ii].kind[0])
			{
			case '1':	
				titleS = "현재가";	fkind = FK_COMMA;	break;
			case '2':
				titleS = "전일대비";	fkind = FK_COMMA;	break;
			case '3':	
				titleS = "매도호가";	fkind = FK_COMMA;	break;
			case '4':
				titleS = "매수호가";	fkind = FK_COMMA;	break;
			case '5':	
				titleS = "등락률";	fkind = FK_NOT;		break;
			case '6':
				titleS = "거래량";	fkind = FK_COMMA;	break;
			default:
				continue;
			}
		}
		else if (sCode.GetLength() == 8)	// 선물, 옵션
		{
			switch (m_codeinfo->alarm[ii].kind[0])
			{
			case '1':	
				titleS = "현재가";	fkind = FK_FLOAT;	break;
			case '2':
				titleS = "전일대비";	fkind = FK_FLOAT;	break;
			case '3':	
				titleS = "매도호가";	fkind = FK_FLOAT;	break;
			case '4':
				titleS = "매수호가";	fkind = FK_FLOAT;	break;
			case '5':	
				titleS = "등락률";	fkind = FK_NOT;		break;
			case '6':
				titleS = "거래량";	fkind = FK_COMMA;	break;
			default:
				continue;
			}
		}
		insertGrid(m_pAlarm.get(), IDC_GRALARM, row);
		height += GRIDROW_HEIGHT2;
		m_pAlarm->SetItemText(row, IPCOL_KIND, titleS);

		tmpS = _GetString(m_codeinfo->alarm[ii].aval, sizeof(m_codeinfo->alarm[ii].aval));
		tmpS = _GetFormatData(fkind, tmpS);
		m_pAlarm->SetItemText(row, IPCOL_VAL, tmpS);

		if (tmpS.IsEmpty())
		{
			row++;
			continue;
		}
		// 감시조건 '0':이상, '1':이하
		if (m_codeinfo->alarm[ii].cond[0] == '1')
		{
			clrCond = clrDn;
			tmpS = "<=";
		}
		else
		{
			clrCond = clrUp;
			tmpS = ">=";
		}

		m_pAlarm->SetItemText(row, IPCOL_COND, tmpS);
		
		// 감시연산 '0':AND,  '1':OR
		if (ii != 0)
		{
			if (m_codeinfo->alarm[ii].oper[0] == '1')
				tmpS = "OR";
			else
				tmpS = "AND";
			m_pAlarm->SetItemText(row, IPCOL_OPER, tmpS);
		}

		m_pAlarm->SetItemBkColor(row, IPCOL_VAL, clrCond);
		m_pAlarm->SetItemBkColor(row, IPCOL_COND, clrCond);
		m_pAlarm->SetItemBkColor(row, IPCOL_OPER, clrCond);

		row++;
	}

	// 주문확인
	for (ii = 0; ii < MAX_ETCCOL; ii++)
	{
		m_pEtc->SetItemText(ETC_ROW_CDATA, ii, "");
		m_pEtc->SetItemText(ETC_ROW_JDATA, ii, "");
	}

	if (m_codeinfo->a_kind[0] & 0x01)		// 확인창
		m_pEtc->SetItemText(ETC_ROW_CDATA, 0, CONFIRM_MARK);
	if (m_codeinfo->a_kind[0] & 0x02)		// 관심종목신호적용
		m_pEtc->SetItemText(ETC_ROW_CDATA, 1, CONFIRM_MARK);
	if (m_codeinfo->a_kind[0] & 0x04)		// 소리적용
		m_pEtc->SetItemText(ETC_ROW_CDATA, 2, CONFIRM_MARK);
	if (m_codeinfo->o_run[0] == '1')		// 주문창
	{
		m_pEtc->SetItemText(ETC_ROW_CDATA, 3, CONFIRM_MARK);

		if (m_codeinfo->o_kind[0] == '1')	// 매수
		{
			m_pEtc->SetItemText(ETC_ROW_JDATA, ETC_COL_MMGB, "매수");
			m_pEtc->SetItemFgColor(ETC_ROW_JDATA, ETC_COL_MMGB, _GetIndexColor(m_pView, CLRMSF));
		}
		else					// 매도
		{
			m_pEtc->SetItemText(ETC_ROW_JDATA, ETC_COL_MMGB, "매도");
			m_pEtc->SetItemFgColor(ETC_ROW_JDATA, ETC_COL_MMGB, _GetIndexColor(m_pView, CLRMDF));
		}

		if (m_codeinfo->o_mmgb[0] == '1')	// 시장가
			m_pEtc->SetItemText(ETC_ROW_JDATA, ETC_COL_JMGB, "시장가");
		else
			m_pEtc->SetItemText(ETC_ROW_JDATA, ETC_COL_JMGB, "지정가");

		tmpS = _GetString(m_codeinfo->o_num, sizeof(m_codeinfo->o_num));
		m_pEtc->SetItemText(ETC_ROW_JDATA, ETC_COL_NUM, _GetFormatData(FK_COMMA, tmpS));

		tmpS = _GetString(m_codeinfo->o_prc, sizeof(m_codeinfo->o_prc));
		if (sCode.GetLength() == 6)		// 주식
			m_pEtc->SetItemText(ETC_ROW_JDATA, ETC_COL_PRICE, _GetFormatData(FK_COMMA, tmpS));
		else if (sCode.GetLength() == 8)	// 선물, 옵션
			m_pEtc->SetItemText(ETC_ROW_JDATA, ETC_COL_PRICE, _GetFormatData(FK_FLOAT, tmpS));
	}
	CRect alarmRC, etcRC, moveRC;
	alarmRC.SetRect(0, GRIDROW_HEIGHT, m_width, GRIDROW_HEIGHT + height);
	m_pAlarm->MoveWindow(0, GRIDROW_HEIGHT, alarmRC.Width(), alarmRC.Height());
	m_pAlarm->Adjust();

	etcRC.SetRect(0, alarmRC.bottom, m_width, alarmRC.bottom+4*GRIDROW_HEIGHT);
	m_pEtc->MoveWindow(0, etcRC.top, etcRC.Width(), etcRC.Height());
	
	moveRC.SetRect(pt.x, pt.y, pt.x+alarmRC.Width(), pt.y+ etcRC.bottom);

	SetWindowPos(&wndTop,
	       moveRC.left,
	       moveRC.top,
	       moveRC.Width()+2,
	       moveRC.Height()+2,
	       SWP_SHOWWINDOW);
	
	SetFocus();
}

void CCatchWnd::Hide()
{
	m_codeinfo = nullptr;
	ShowWindow(SW_HIDE);
}

BOOL CCatchWnd::CreatePopup()
{
	if (m_hWnd)
	{
		if (detach() == FALSE)
			return FALSE;
	}
	const BOOL bRet = CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_WINDOWEDGE,	// WS_EX_CLIENTEDGE
		       CONTROLCATCH_CLASSNAME,
		       nullptr,
		       WS_BORDER | WS_POPUP,
		       0,
		       0,
		       0,
		       0,
		       nullptr,
		       nullptr,
		       nullptr);
	if (!bRet)	return FALSE;
	initControl();
	return TRUE;
}

void CCatchWnd::RegisterWindowClass()
{
	WNDCLASS  stWndClass{};
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, CONTROLCATCH_CLASSNAME, &stWndClass)))
	{
		stWndClass.style         = CS_SAVEBITS | CS_DBLCLKS;
		stWndClass.lpfnWndProc   = ::DefWindowProc;
		stWndClass.cbClsExtra    = 0;
		stWndClass.cbWndExtra    = 0;
		stWndClass.hInstance     = hInst;
		stWndClass.hIcon         = nullptr;
		stWndClass.hCursor       = LoadCursor(hInst, IDC_ARROW);
		stWndClass.hbrBackground = (HBRUSH)(COLOR_INFOBK + 1);
		stWndClass.lpszMenuName  = nullptr;
		stWndClass.lpszClassName = CONTROLCATCH_CLASSNAME;

		if (!AfxRegisterClass(&stWndClass)) AfxThrowResourceException();
	}
}

//=================================================================================================


void CCatchWnd::setGridItem(int col, _GVITEM *pGvitem, CString text)
{
	pGvitem->text = text;
	pGvitem->col = col;
	pGvitem->format = grInfoAlarm[col].format;
	pGvitem->attr = grInfoAlarm[col].attr;
	m_pAlarm->SetItem(pGvitem);
}

void CCatchWnd::initControl()
{
	m_clrBk = _GetIndexColor(m_pView, CLRBG);

	m_width = 0; 
	for (int ii = 0; ii < sizeof(grInfoAlarm)/sizeof(struct _grInfo); ii++)
		m_width += grInfoAlarm[ii].width;
	
	CRect rc;
	// 종목코드
	m_pName = std::make_unique<CAxStatic>();
	rc.SetRect(0, 0, m_width, GRIDROW_HEIGHT-1);
	m_pName->Create(nullptr, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|SS_CENTER, rc, this, IDC_STCCODE);
	if (m_pBFont)	m_pName->SetFont(m_pBFont);
	m_pName->setColor(_GetIndexColor(m_pView, CLRTITLE), _GetIndexColor(m_pView, CLRGRROWF), _GetIndexColor(m_pView, CLRTITLE));

	// 그룹 grid
	rc.SetRect(0, GRIDROW_HEIGHT, m_width, 100);
	m_pAlarm = std::make_unique < CExGrid>();
	m_pAlarm->Create(rc, this, IDC_GRALARM, GVSC_VERT, 0, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
	initGrid(m_pAlarm.get(), IDC_GRALARM);

	// 기타 grid
	rc.SetRect(0, GRIDROW_HEIGHT+100, m_width, 100);
	m_pEtc = std::make_unique < CExGrid>();
	m_pEtc->Create(rc, this, IDC_GRETC, GVSC_VERT, 0, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
	initGrid(m_pEtc.get(), IDC_GRETC);
}

void CCatchWnd::initGrid(CExGrid* pGrid, int idc)
{
	int ii = 0;
	int colCnt = 0;
	_GVITEM gvitem;

	gvitem.row = 0;
	gvitem.mask = GVMK_TEXT|GVMK_FORMAT|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
	gvitem.mask |= GVMK_ATTR|GVMK_MISC|GVMK_PARAM;
	gvitem.font = m_lf;
	gvitem.format = GVFM_HEADER;
	gvitem.fgcol = _GetIndexColor(m_pView, CLRGRHF);
	gvitem.bkcol = _GetIndexColor(m_pView, CLRGRHB);
	gvitem.attr = 0;

	switch (idc)
	{
	case IDC_GRALARM:
		colCnt = sizeof(grInfoAlarm)/sizeof(_grInfo);
		pGrid->Initial(1, colCnt, 1, 0);
 		for (ii = 0; ii < colCnt; ii++)
		{
 			gvitem.symbol = grInfoAlarm[ii].symbols;
			gvitem.text = grInfoAlarm[ii].heads;
			gvitem.col = ii;
			pGrid->SetColumnWidth(ii, grInfoAlarm[ii].width);
			pGrid->SetItem(&gvitem);
		}
		break;
	case IDC_GRETC:
		{
			int colWidth = 0;
			colCnt = MAX_ETCCOL;
			pGrid->Initial(1, colCnt, 1, 0);
 			for (ii = 0; ii < colCnt; ii++)
			{
 				gvitem.symbol = "";
				gvitem.text = "";
				gvitem.col = ii;
				if (ii == colCnt -1) 
					pGrid->SetColumnWidth(ii, m_width-colWidth+2);
				else
				{
					colWidth += m_width/4;
					pGrid->SetColumnWidth(ii, m_width/4);
				}
				
					
				pGrid->SetItem(&gvitem);
			}
			for (ii = 0; ii < MAX_ETCROW; ii++)
			{
				insertGrid(pGrid, idc, ii+1);
			}
			m_pEtc->SetItemText(ETC_ROW_CTITLE, ETC_COL_CONFIRM, "확인창");
			m_pEtc->SetItemText(ETC_ROW_CTITLE, ETC_COL_INTER, "관심");
			m_pEtc->SetItemText(ETC_ROW_CTITLE, ETC_COL_SOUND, "소리");
			m_pEtc->SetItemText(ETC_ROW_CTITLE, ETC_COL_JUMUN, "주문");

			m_pEtc->SetItemText(ETC_ROW_JTITLE, ETC_COL_MMGB, "주문");
			m_pEtc->SetItemBkColor(ETC_ROW_JTITLE, ETC_COL_MMGB, _GetIndexColor(m_pView, CLRGRHB));
			m_pEtc->SetItemText(ETC_ROW_JTITLE, ETC_COL_JMGB, "매매");
			m_pEtc->SetItemBkColor(ETC_ROW_JTITLE, ETC_COL_JMGB, _GetIndexColor(m_pView, CLRGRHB));
			m_pEtc->SetItemText(ETC_ROW_JTITLE, ETC_COL_NUM, "수량");
			m_pEtc->SetItemBkColor(ETC_ROW_JTITLE, ETC_COL_NUM, _GetIndexColor(m_pView, CLRGRHB));
			m_pEtc->SetItemText(ETC_ROW_JTITLE, ETC_COL_PRICE, "단가");
			m_pEtc->SetItemBkColor(ETC_ROW_JTITLE, ETC_COL_PRICE, _GetIndexColor(m_pView, CLRGRHB));

		}
		break;
	}
	
	pGrid->SetGridFocusLine(FALSE, FALSE);
	pGrid->SetRowHeight(0, GRIDROW_HEIGHT);
	pGrid->SetRowColResize(FALSE, FALSE);
	pGrid->SetGridColor(_GetIndexColor(m_pView, CLRGRLINE));
	pGrid->SetBkColor(_GetIndexColor(m_pView, CLRGRROWB2));
	pGrid->Adjust();
}
int CCatchWnd::insertGrid(CExGrid* pGrid, UINT idc, int row)
{
	_GVITEM gvitem;
	CString	 tmpS, statS;
//	double val = 0;

	gvitem.font   = m_lf;
	gvitem.mask   = GVMK_FONT|GVMK_TEXT|GVMK_FORMAT|GVMK_PARAM|GVMK_ATTR|GVMK_BKCOLOR|GVMK_FGCOLOR;
	gvitem.bkcol = _GetIndexColor(m_pView, CLRGRROWB1);
	gvitem.fgcol  = _GetIndexColor(m_pView, CLRGRROWF);

	row = pGrid->InsertRow(" ", row);
	gvitem.row    = row;

	int col = 0;
	switch (idc)
	{
	case IDC_GRALARM:
		pGrid->SetRowHeight(row, GRIDROW_HEIGHT2);
		for (col = 0; col < sizeof(grInfoAlarm)/sizeof(_grInfo); col++)
		{
			gvitem.text = "";
			gvitem.col = col;
			gvitem.format = grInfoAlarm[col].format;
			gvitem.attr = grInfoAlarm[col].attr;
			pGrid->SetItem(&gvitem);
		}
		break;
	case IDC_GRETC:
		pGrid->SetRowHeight(row, GRIDROW_HEIGHT);
		for (col = 0; col < MAX_ETCCOL; col++)
		{
			gvitem.text = "";
			gvitem.col = col;
			gvitem.format = GVFM_CENTER;
			gvitem.attr = 0;
			pGrid->SetItem(&gvitem);
		}
		break;
	}
	return row;
}

BOOL CCatchWnd::detach()
{
	if (!m_hWnd)
		return TRUE;
	ShowWindow(SW_HIDE);
	m_pParent = nullptr;
	return true;
	
	//return DestroyWindow();
}


