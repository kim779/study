#include "stdafx.h"
#include "CX_IB220001.h"
#include "ControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int iHeads[] = {52, 52, 55, 49, 49, 52, 49, 49, 55, 52, 52};
int iSHeads[] = {51, 51, 51, 55, 49, 52, 49, 55, 51, 51, 51};
LPCSTR strHeads[] = {"증감", "미결제", "거래량", "대비", "현재가", "행사가", "현재가", "대비", "거래량", "미결제", "증감", NULL};
LPCSTR strSHeads[] = {"저가", "고가", "시가", "거래량", "현재가", "행사가", "현재가", "거래량", "시가", "고가", "저가", NULL};

#define TRG	"110301"
#define S2I(x)	Str2Int(x, sizeof(x))
#define S2D(x)	Str2Double(x, sizeof(x))

CControlWnd::CControlWnd(CWnd* pParent, _param* pParam)
: CAxisExt(pParent)
{
	EnableAutomation();

	m_rect = pParam->rect;
	m_hsga = "";
	m_SelRow = m_HsgaRow = -1;
	m_DrawQueue = 0;
	m_nType = 0;
	m_nPredict = 0;
	m_pGrid = nullptr;
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
//	ON_MESSAGE(WMX_LAZYDRAW, OnLazyDraw)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY(CControlWnd, "code", m_code, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "hsga", m_hsga, OnHsgaChanged, VT_BSTR)
	DISP_FUNCTION(CControlWnd, "SetMapHandle", SetMapHandle, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetMapHandle", GetMapHandle, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetPredict", SetPredict, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CControlWnd, "SetType", SetType, VT_EMPTY, VTS_I2)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {D97F5D83-BE90-4D41-A79B-FD27744A03DF}
static const IID IID_IControlWnd =
{ 0xd97f5d83, 0xbe90, 0x4d41, { 0xa7, 0x9b, 0xfd, 0x27, 0x74, 0x4a, 0x3, 0xdf } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

LRESULT CControlWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		ParseOUB(exth->data, exth->size);
		break;

	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!
		ParseOUB((LPCSTR)lParam, HIWORD(wParam));
		break;

	case DLL_ALERTx:
		struct _alertR* alertR;
		alertR = (struct _alertR*)lParam;
		ParseAlert(alertR);
		break;

	case DLL_SETPAL:
		ChangeTheme();
		break;

	case DLL_INB:
	case DLL_TRIGGER:
	case DLL_NOTICE:
	case DLL_SETFONT:
	case DLL_SETFONTx:
	case DLL_GUIDE:
	case DLL_DOMINO:
	case DLL_AXIS:
	case DLL_INVOKE:
	default:
		break;
	}
	return 0;
}

void CControlWnd::SetMapHandle(LPCTSTR sHandle) 
{
	m_sMapHandle = sHandle;
}

BSTR CControlWnd::GetMapHandle() 
{
	CString strResult;
	strResult = m_sMapHandle;
	
	return strResult.AllocSysString();
}

void CControlWnd::ParseOUB(LPCSTR dat, int len)
{
	if (!m_pGrid || !m_pGrid->GetSafeHwnd())
		return;

	pibo4013_mod *mod = (pibo4013_mod*)dat;
	const	int	nRows = Str2Int(mod->grec, sizeof(mod->grec));

	LOGFONT lf;
	m_pFontB->GetLogFont(&lf);

	m_pGrid->SetRowCount(nRows+1);

	const	COLORREF row_fg = GetIndexColor(GRID_TEXT_CLR);
	COLORREF row_bg = 0;
	for (int row = 1; row < m_pGrid->GetRowCount(); ++row)
	{
		row_bg = GetIndexColor(row%2 ? GRID_1ST_BG : GRID_2ND_BG);
		for (int col = 0; col < m_pGrid->GetColumnCount(); ++col)
		{
			m_pGrid->SetItemFgColour(row, col, row_fg);
			m_pGrid->SetItemBkColour(row, col, row_bg);
			m_pGrid->SetItemText(row, col, "");

			if (col == C_HSGA)
				m_pGrid->SetItemFormat(row, col, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
			else	m_pGrid->SetItemFormat(row, col, DT_SINGLELINE|DT_VCENTER|DT_RIGHT);

			if (col == CC_CURR || col == C_HSGA || col ==  CP_CURR)
				m_pGrid->SetItemFont(row, col, &lf);
		}
	}

	ClearData();
	pibo4013_grid *pg = (pibo4013_grid*)&mod->grid[0];
	std::shared_ptr<OPTION_DATA> od = nullptr;

	for (int row = 1; row <= nRows; ++row, ++pg)
	{
		// Call
		od = std::make_shared<OPTION_DATA>();
		CopyMemory(od->code, pg->c_rtcd, sizeof(pg->c_rtcd));
		od->set_mdif (S2I(pg->c_mdif));
		od->set_mgjv(S2I(pg->c_mgjv));
		od->set_gvol(S2I(pg->c_gvol));
		od->set_diff(Str2Double(&pg->c_diff[1], sizeof(pg->c_diff)-1));
		if (pg->c_diff[0]=='5' || pg->c_diff[0]=='4' || pg->c_diff[0]=='-')
			od->set_diff(-od->diff);

		od->set_curr(S2D(pg->c_curr));
		od->set_gjga(fabs(fabs(od->curr) - od->diff));
		od->set_ycurr(S2D(pg->c_ycur));
		od->set_ycuvl(S2I(pg->c_ycvl));  //0422
		od->set_hsga(S2D(pg->hang));
		od->set_siga(S2D(pg->c_siga));
		od->set_koga(S2D(pg->c_koga));
		od->set_jega(S2D(pg->c_jega));
		od->grid_row = row;
		m_data.SetAt(od->code, od);

		// Put
		od = std::make_shared<OPTION_DATA>();
		CopyMemory(od->code, pg->p_rtcd, sizeof(pg->p_rtcd));
		od->set_mdif (S2I(pg->p_mdif));
		od->set_mgjv(S2I(pg->p_mgjv));
		od->set_gvol(S2I(pg->p_gvol));

		od->set_diff(Str2Double(&pg->p_diff[1], sizeof(pg->p_diff)-1));
		if (pg->p_diff[0]=='5' || pg->p_diff[0]=='4' || pg->p_diff[0]=='-')
			od->set_diff(-od->diff);

		od->set_curr(S2D(pg->p_curr));
		od->set_gjga(fabs(fabs(od->curr) - od->diff));
		od->set_ycurr(S2D(pg->p_ycur));
		od->set_ycuvl(S2I(pg->p_ycvl));  //0422
		od->set_hsga(S2D(pg->hang));
		od->set_siga(S2D(pg->p_siga));
		od->set_koga(S2D(pg->p_koga));
		od->set_jega(S2D(pg->p_jega));
		od->grid_row = row;
		m_data.SetAt(od->code, od);
	}

	OnSize(0,0,0);
	DrawCode(nullptr, nullptr);
	m_pGrid->Refresh();
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_rcOrg.SetRect(0, 0, SCREEN_CX, SCREEN_CY);

	m_pGrid = std::make_unique<CGridCtrl>();
	if (!m_pGrid->Create(m_rect, this, IDC_GRID, WS_CHILD|WS_VISIBLE|WS_BORDER))
	{
		m_pGrid.reset();
		return -1;
	}

	m_pGrid->SetFont(m_pFont);
	m_pGrid->EnableSelection(FALSE);
	m_pGrid->SetFrameFocusCell(FALSE);
	m_pGrid->SetTrackFocusCell(FALSE);
	//m_pGrid->SetColumnResize(FALSE);
	m_pGrid->EnableToolTips(TRUE);
	//m_pGrid->SetDefCellMargin(2);
	m_pGrid->SetDefCellHeight(GRID_CELL_HEIGHT);
	m_pGrid->SetRowResize(FALSE);
	m_pGrid->SetBkColor(m_crBodyBk);
	m_pGrid->SetEditable(FALSE);
	m_pGrid->SetDoubleBuffering(TRUE);
	m_pGrid->SetGridLines(GVL_VERT);
	m_pGrid->SetGridLineColor(RGB(0xAE, 0xB8, 0xC6));
	m_pGrid->SetGridBorderColor(RGB(0xAE, 0xB8, 0xC6));
	m_pGrid->SetRowCount(GRID_VISIBLE_ROWS+1);	// rows + header
	m_pGrid->SetColumnCount(GRID_COLS);
	m_pGrid->SetFixedRowCount(1);
	m_pGrid->SetFixedColumnCount(0);

	const	COLORREF head_bg = GetIndexColor(GRID_HEAD_CLR);
	const	COLORREF head_fg = GetIndexColor(GRID_HEAD_TEXT_CLR);
	for (int col = 0; col <  m_pGrid->GetColumnCount(); col++)
	{
		if (strHeads[col])
			m_pGrid->SetItemText(0, col, strHeads[col]);
		m_pGrid->SetColumnWidth(col, iHeads[col]);
		m_pGrid->SetItemFormat(0, col, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		m_pGrid->SetItemFgColour(0, col, head_fg);
		m_pGrid->SetItemBkColour(0, col, head_bg);
	}

	const	COLORREF row_fg = GetIndexColor(GRID_TEXT_CLR);
	COLORREF row_bg = 0;
	for (int row = 1; row <  m_pGrid->GetRowCount(); row++)
	{
		row_bg = GetIndexColor(row%2 ? GRID_1ST_BG : GRID_2ND_BG);
		for (int col = 0; col < m_pGrid->GetColumnCount(); ++col)
		{
			m_pGrid->SetItemFgColour(row, col, row_fg);
			m_pGrid->SetItemBkColour(row, col, row_bg);
		}
	}
	
	SetTimer(150, DRAWQUEUE_TIMER, NULL);
	return 0;
}

void CControlWnd::OnDestroy() 
{
	ClearData();
	if (m_pGrid)
		m_pGrid.reset();

	CWnd::OnDestroy();
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (!m_pGrid || !m_pGrid->GetSafeHwnd())
		return;

	CRect	rc;
	double	rate_x = 0, rate_y = 0;

	GetClientRect(&rc);
	m_pGrid->MoveWindow(&rc, FALSE);
	
	if (rc.Height())
	{
		rate_y = rc.Height() / (double)m_rcOrg.Height();
		const	int	n_cell_height = (int)(GRID_CELL_HEIGHT * rate_y);
		for (int row = 0; row < m_pGrid->GetRowCount(); ++row)
			m_pGrid->SetRowHeight(row, n_cell_height);
	}

	if (rc.Width())
	{
		int	twidth = 0, dx = 0, col = 0;
		rate_x = (rc.Width()-GRID_SBAR_WIDTH) / (double)(m_rcOrg.Width()-GRID_SBAR_WIDTH);
		for (col = CC_MDIF; col <= CC_CURR; ++col)
		{
			dx = (int)(iHeads[col] * rate_x);
			twidth += dx;
			m_pGrid->SetColumnWidth(col, dx);
		}
		for (col = CP_CURR; col <= CP_MDIF; ++col)
		{
			dx = (int)(iHeads[col] * rate_x);
			twidth += dx;
			m_pGrid->SetColumnWidth(col, dx);
		}
		m_pGrid->SetColumnWidth(C_HSGA, rc.Width()-GRID_SBAR_WIDTH-twidth);
	}

	if (rc.Width() && rc.Height())
	{
		LOGFONT lf{};
		const	double	rate = (rate_x > rate_y) ? rate_y : rate_x;

		GetAxFont("굴림체", (int)(9.49*rate), false, false)->GetLogFont(&lf);
		for (int row=0; row<m_pGrid->GetRowCount(); ++row)
		{
			for (int col = 0; col < m_pGrid->GetColumnCount(); ++col)
			{
				if (row>0 && (col==CC_CURR || col==C_HSGA || col==CP_CURR))
				{
					lf.lfWeight = FW_BOLD;
					m_pGrid->SetItemFont(row, col, &lf);
				}
				else
				{
					lf.lfWeight = FW_NORMAL;
					m_pGrid->SetItemFont(row, col, &lf);
				}
			}
		}
	}
	
}

void CControlWnd::ClearData()
{
	CString	key;
	std::shared_ptr<OPTION_DATA> val = nullptr;
	POSITION pos = m_data.GetStartPosition();

	while (pos)
	{
		m_data.GetNextAssoc(pos, key, val);
		val.reset();
	}
	m_data.RemoveAll();
}

void CControlWnd::DrawCode(std::shared_ptr<OPTION_DATA> od, CDC *pdc)
{
	BOOL	bRelease = FALSE;
	if (!pdc) 
	{
		bRelease = TRUE;
		pdc = m_pGrid->GetDC();
	}

	if (od == nullptr)
	{
		CString key;
		POSITION pos = m_data.GetStartPosition();
		while (pos)
		{
			m_data.GetNextAssoc(pos, key, od);
			DrawCode(od, pdc);
		}
		if (bRelease)
			m_pGrid->ReleaseDC(pdc);
		return;
	}

	char	buff[64]{};
	const	int	row = od->grid_row;

	m_pGrid->beginDrawHolding();
	if (m_nType == 0)
	{
		if (od->is_call())
		{
			if (od->is_mdif ())
			{
				m_pGrid->SetItemFgColour(row, CC_MDIF, GetColor(od->mdif));
				m_pGrid->SetItemText(row, CC_MDIF, Int2CommaStr(od->mdif));
				m_pGrid->RedrawCell(row, CC_MDIF, pdc);
			}
			if (od->is_mgjv())
			{
				m_pGrid->SetItemText(row, CC_MGJV, Int2CommaStr(od->mgjv));
				m_pGrid->RedrawCell(row, CC_MGJV, pdc);
			}
			if (od->is_gvol())
			{
				m_pGrid->SetItemText(row, CC_GVOL, Int2CommaStr(od->gvol));
				m_pGrid->RedrawCell(row, CC_GVOL, pdc);
			}
			if (od->is_diff())
			{
				m_pGrid->SetItemFgColour(row, CC_DIFF, GetColor(od->diff));
				sprintf(buff, "%.2f", fabs(od->diff));
				m_pGrid->SetItemText(row, CC_DIFF, (od->diff != 0.0) ? buff : "");
				m_pGrid->RedrawCell(row, CC_DIFF, pdc);
			}

			if (m_nPredict == 1)
			{
				if (od->is_ycurr())
				{
					m_pGrid->SetItemFgColour(row, CC_CURR, GetColor(od->diff));
					sprintf(buff, "%.2f", fabs(od->ycurr));
					m_pGrid->SetItemText(row, CC_CURR, (od->ycurr != 0.0) ? buff : "");
					m_pGrid->RedrawCell(row, CC_CURR, pdc);

					if (od->is_ycuvl())  //0422 1
					{
						m_pGrid->SetItemFgColour(row, CC_GVOL, GetColor(od->diff));
						sprintf(buff, "%d", (od->ycuvl));
						/*CString stmp;
						stmp.Format("[2200] type = 0 call %d %s", od->ycuvl, buff);
						OutputDebugString(stmp);*/
						m_pGrid->SetItemText(row, CC_GVOL, (od->ycuvl != 0) ? buff : "");
						m_pGrid->RedrawCell(row, CC_GVOL, pdc);
					}
				}
			}
			else
			{
				if (od->is_curr())
				{
					m_pGrid->SetItemFgColour(row, CC_CURR, GetColor(od->diff));
					sprintf(buff, "%.2f", fabs(od->curr));
					m_pGrid->SetItemText(row, CC_CURR, (od->curr != 0.0) ? buff : "");
					m_pGrid->RedrawCell(row, CC_CURR, pdc);
				}
			}

			if (od->is_hsga())
			{
				sprintf(buff, "%.2f", od->hsga);
				m_pGrid->SetItemText(row, C_HSGA, buff);
				m_pGrid->RedrawCell(row, C_HSGA, pdc);
			}
		}
		else
		{
			if (od->is_mdif ())
			{
				m_pGrid->SetItemFgColour(row, CP_MDIF, GetColor(od->mdif));
				m_pGrid->SetItemText(row, CP_MDIF, Int2CommaStr(od->mdif));
				m_pGrid->RedrawCell(row, CP_MDIF, pdc);
			}
			if (od->is_mgjv())
			{
				m_pGrid->SetItemText(row, CP_MGJV, Int2CommaStr(od->mgjv));
				m_pGrid->RedrawCell(row, CP_MGJV, pdc);
			}
			if (od->is_gvol())
			{
				m_pGrid->SetItemText(row, CP_GVOL, Int2CommaStr(od->gvol));
				m_pGrid->RedrawCell(row, CP_GVOL, pdc);
			}
			if (od->is_diff())
			{
				m_pGrid->SetItemFgColour(row, CP_DIFF, GetColor(od->diff));
				sprintf(buff, "%.2f", fabs(od->diff));
				m_pGrid->SetItemText(row, CP_DIFF, (od->diff != 0.0) ? buff : "");
				m_pGrid->RedrawCell(row, CP_DIFF, pdc);
			}

			if (m_nPredict == 1)
			{
				if (od->is_ycurr())
				{
					m_pGrid->SetItemFgColour(row, CP_CURR, GetColor(od->diff));
					sprintf(buff, "%.2f", fabs(od->ycurr));
					m_pGrid->SetItemText(row, CP_CURR, (od->ycurr != 0.0) ? buff : "");
					m_pGrid->RedrawCell(row, CP_CURR, pdc);

					if (od->is_ycuvl()) //0422 1
					{
						m_pGrid->SetItemFgColour(row, CP_GVOL, GetColor(od->diff));
						sprintf(buff, "%d", (od->ycuvl));
					/*	CString stmp;
						stmp.Format("[2200] type = 0 put  %d %s", od->ycuvl, buff);
						OutputDebugString(stmp);*/
						m_pGrid->SetItemText(row, CP_GVOL, (od->ycuvl != 0) ? buff : "");
						m_pGrid->RedrawCell(row, CP_GVOL, pdc);
					}
				}
			}
			else
			{
				if (od->is_curr())
				{
					m_pGrid->SetItemFgColour(row, CP_CURR, GetColor(od->diff));
					sprintf(buff, "%.2f", fabs(od->curr));
					m_pGrid->SetItemText(row, CP_CURR, (od->curr != 0.0) ? buff : "");
					m_pGrid->RedrawCell(row, CP_CURR, pdc);
				}
			}
		}
	}
	else if (m_nType == 1)
	{
		if (od->is_call())
		{
			if (od->is_jega())
			{
				m_pGrid->SetItemFgColour(row, OC_JEGA, GetColor(od->gjga,od->jega));
				sprintf(buff, "%.2f", fabs(od->jega));
				m_pGrid->SetItemText(row, OC_JEGA, (od->jega != 0.0) ? buff : "");
				m_pGrid->RedrawCell(row, OC_JEGA, pdc);
			}
			if (od->is_koga())
			{
				m_pGrid->SetItemFgColour(row, OC_KOGA, GetColor(od->gjga,od->koga));
				sprintf(buff, "%.2f", fabs(od->koga));
				m_pGrid->SetItemText(row, OC_KOGA, (od->koga != 0.0) ? buff : "");
				m_pGrid->RedrawCell(row, OC_KOGA, pdc);
			}
			if (od->is_siga())
			{
				m_pGrid->SetItemFgColour(row, OC_SIGA, GetColor(od->gjga,od->siga));
				sprintf(buff, "%.2f", fabs(od->siga));
				m_pGrid->SetItemText(row, OC_SIGA, (od->siga != 0.0) ? buff : "");
				m_pGrid->RedrawCell(row, OC_SIGA, pdc);
			}
			if (od->is_gvol())
			{
				m_pGrid->SetItemText(row, OC_GVOL, Int2CommaStr(od->gvol));
				m_pGrid->RedrawCell(row, OC_GVOL, pdc);
			}

			if (m_nPredict == 1)
			{
				if (od->is_ycurr())
				{
					m_pGrid->SetItemFgColour(row, OC_CURR, GetColor(od->diff));
					sprintf(buff, "%.2f", fabs(od->ycurr));
					m_pGrid->SetItemText(row, OC_CURR, (od->ycurr != 0.0) ? buff : "");
					m_pGrid->RedrawCell(row, OC_CURR, pdc);

					if (od->is_ycuvl())  //0422 1
					{
						m_pGrid->SetItemFgColour(row, OC_GVOL, GetColor(od->diff));
						sprintf(buff, "%d", (od->ycuvl));
					/*	CString stmp;
						stmp.Format("[2200] type = 1 call  %d %s", od->ycuvl, buff);
						OutputDebugString(stmp);*/
						m_pGrid->SetItemText(row, OC_GVOL, (od->ycuvl != 0) ? buff : "");
						m_pGrid->RedrawCell(row, OC_GVOL, pdc);
					}
				}
			}
			else
			{
				if (od->is_curr())
				{
					m_pGrid->SetItemFgColour(row, OC_CURR, GetColor(od->diff));
					sprintf(buff, "%.2f", fabs(od->curr));
					m_pGrid->SetItemText(row, OC_CURR, (od->curr != 0.0) ? buff : "");
					m_pGrid->RedrawCell(row, OC_CURR, pdc);
				}
			}

			if (od->is_hsga())
			{
				sprintf(buff, "%.2f", od->hsga);
				m_pGrid->SetItemText(row, O_HSGA, buff);
				m_pGrid->RedrawCell(row, O_HSGA, pdc);
			}
		}
		else
		{
			if (od->is_jega())
			{
				m_pGrid->SetItemFgColour(row, OP_JEGA, GetColor(od->gjga,od->jega));
				sprintf(buff, "%.2f", fabs(od->jega));
				m_pGrid->SetItemText(row, OP_JEGA, (od->jega != 0.0) ? buff : "");
				m_pGrid->RedrawCell(row, OP_JEGA, pdc);
			}
			if (od->is_koga())
			{
				m_pGrid->SetItemFgColour(row, OP_KOGA, GetColor(od->gjga,od->koga));
				sprintf(buff, "%.2f", fabs(od->koga));
				m_pGrid->SetItemText(row, OP_KOGA, (od->koga != 0.0) ? buff : "");
				m_pGrid->RedrawCell(row, OP_KOGA, pdc);
			}
			if (od->is_siga())
			{
				m_pGrid->SetItemFgColour(row, OP_SIGA, GetColor(od->gjga,od->siga));
				sprintf(buff, "%.2f", fabs(od->siga));
				m_pGrid->SetItemText(row, OP_SIGA, (od->siga != 0.0) ? buff : "");
				m_pGrid->RedrawCell(row, OP_SIGA, pdc);
			}
			if (od->is_gvol())
			{
				m_pGrid->SetItemText(row, OP_GVOL, Int2CommaStr(od->gvol));
				m_pGrid->RedrawCell(row, OP_GVOL, pdc);
			}
			
			if (m_nPredict == 1)
			{
				if (od->is_ycurr())
				{
					m_pGrid->SetItemFgColour(row, OP_CURR, GetColor(od->diff));
					sprintf(buff, "%.2f", fabs(od->ycurr));
					m_pGrid->SetItemText(row, OP_CURR, (od->ycurr != 0.0) ? buff : "");
					m_pGrid->RedrawCell(row, OP_CURR, pdc);

					if (od->is_ycuvl())  //0422 1
					{
						m_pGrid->SetItemFgColour(row, OP_GVOL, GetColor(od->diff));
						sprintf(buff, "%d", od->ycuvl);
						/*CString stmp;
						stmp.Format("[2200] type = 1 put  %d %s", od->ycuvl, buff);
						OutputDebugString(stmp);*/
						m_pGrid->SetItemText(row, OP_GVOL, (od->ycuvl != 0) ? buff : "");
						m_pGrid->RedrawCell(row, OP_GVOL, pdc);
					}
				}
			}
			else
			{
				if (od->is_curr())
				{
					m_pGrid->SetItemFgColour(row, OP_CURR, GetColor(od->diff));
					sprintf(buff, "%.2f", fabs(od->curr));
					m_pGrid->SetItemText(row, OP_CURR, (od->curr != 0.0) ? buff : "");
					m_pGrid->RedrawCell(row, OP_CURR, pdc);
				}
			}
		}
	}
	m_pGrid->endDrawHolding();
	if (bRelease)
		m_pGrid->ReleaseDC(pdc);
}

void CControlWnd::ParseAlert(struct _alertR* alertR)
{
	CString key;
	std::shared_ptr<OPTION_DATA> pod = nullptr;
	
	// Key check
	key = alertR->code;
	if (!m_data.Lookup(key, pod))
		return;
	
	CString	str;
	BOOL	bUpdate = FALSE;
	DWORD*	data = nullptr;

	for (int ii = alertR->size-1; ii >= 0; ii--)
	{
		data = (DWORD*)alertR->ptr[ii];
		if (data[34])
			bUpdate = TRUE;
		
		if (data[206])
		{
			str = (char*)data[206];
			pod->set_mdif(Str2Int(str));
		}

		if (data[201])
		{
			str = (char*)data[201];
			pod->set_mgjv(Str2Int(str));
		}

		if (data[29])
		{
			str = (char*)data[29];
			pod->set_siga(S2D(str));
		}

		if (data[30])
		{
			str = (char*)data[30];
			pod->set_koga(S2D(str));
		}

		if (data[31])
		{
			str = (char*)data[31];
			pod->set_jega(S2D(str));
		}

		if (m_nPredict == 0)
		{
			if (data[27])
			{
				str = (char*)data[27];
				pod->set_gvol(Str2Int(str));
			}

			if (data[24])
			{
				str = (char*)data[24];
				pod->set_diff(Str2Double(str));
			}

			if (data[23])
			{
				str = (char*)data[23];
				pod->set_curr(fabs(Str2Double(str)));
			}
		}

		if (m_nPredict == 1)
		{
			if (data[115])
			{
				str = (char*)data[115];
				pod->set_diff(Str2Double(str));
				bUpdate = TRUE;
			}

			if (data[111])
			{
				str = (char*)data[111];
				pod->set_ycurr(fabs(Str2Double(str)));
				bUpdate = TRUE;
			}

			if (data[112])  //0422
			{
				str = (char*)data[112];
				pod->set_ycuvl(Str2Int(str));
				bUpdate = TRUE;
			}
		}
	}
	
	if (m_code == pod->code)	
	{
		if (!bUpdate)
			return;

		DrawCode(pod);
	}
	else
	{
		if (!bUpdate)
			return;

		++m_DrawQueue;
		if (m_DrawQueue>DRAWQUEUE_CNT)
		{
			m_qdata.SetAt(pod->code, pod);
		}
		else
		{
			DrawCode(pod);
		}
	}
}
#if 0
LRESULT CControlWnd::OnLazyDraw(WPARAM wParam, LPARAM lParam)
{
	OPTION_DATA *pod = (OPTION_DATA*)lParam;
	--pod->drawcnt;
	DrawCode(pod);
	return 0;
}
#endif

BOOL CControlWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	GV_DISPINFO *pdi = (GV_DISPINFO *)lParam;
	CString pdat; 
	OPTION_DATA *pd = GetData(pdi->item.row, pdi->item.col);

	if (pdi->hdr.code == NM_CLICK && pdi->item.row > 0)
	{
		if (m_SelRow != pdi->item.row)
		{
			const	COLORREF old_clr = (m_SelRow%2) ? GetIndexColor(GRID_1ST_BG) : GetIndexColor(GRID_2ND_BG);
			const	COLORREF new_clr = GetIndexColor(GRID_FOCUS_BG);
			
			for (int col = 0; col < m_pGrid->GetColumnCount(); ++col)
			{
				if (m_SelRow==m_HsgaRow && col==C_HSGA)
					m_pGrid->SetItemBkColour(m_SelRow, col, m_crLine);
				else
					m_pGrid->SetItemBkColour(m_SelRow, col, old_clr);
				
				if (pdi->item.row==m_HsgaRow && col==C_HSGA)
					m_pGrid->SetItemBkColour(pdi->item.row, col, m_crLine);
				else
					m_pGrid->SetItemBkColour(pdi->item.row, col, new_clr);

				m_pGrid->RedrawRow(m_SelRow);
				m_pGrid->RedrawRow(pdi->item.row);
			}
			
			m_SelRow = pdi->item.row;
		}

		if (!pd)
			return FALSE;

		m_code = pd->code;
		CString tmpCode = pd->code;
		if (tmpCode.GetAt(0) == 'X')
		{
			tmpCode = tmpCode.Mid(1);
		}

		pdat.Format("SetFOCode\t%s,%s", tmpCode, m_sMapHandle);
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)pdat.GetString());
	}

	if (pdi->hdr.code == NM_CLICK && pdi->item.row == 0 && m_HsgaRow > 0)
	{
		OnHsgaChanged();
	}

	if (pdi->hdr.code == NM_RCLICK)
	{
		CPoint	pt;
		CString strCode;

		GetCursorPos(&pt);
		//2012.11.09 - dkkim
		//오른쪽 클릭후 팝업 메뉴 누를때 코드에 X가 붙어서 가는거 방지.
		strCode = pd->code;
		if (strCode.GetAt(0) == 'X')
		{
			strCode = strCode.Mid(1);
		}

// 		if (pd)
// 			pdat.Format("IB220000\n%d\t%d\ned_focod\t%s\n40301\t%s", pt.x, pt.y, pd->code, pd->code);
// 		else
// 			pdat.Format("IB220000\n%d\t%d\n", pt.x, pt.y);
		if (pd)
			pdat.Format("IB220000\n%d\t%d\ned_focod\t%s\n40301\t%s", pt.x, pt.y, strCode, strCode);
		else
			pdat.Format("IB220000\n%d\t%d\n", pt.x, pt.y);

		m_pParent->PostMessage(WM_USER, MAKEWPARAM(mouseDLL, FALSE), (LPARAM)(LPCTSTR)pdat);
	}
	
	return CWnd::OnNotify(wParam, lParam, pResult);
}

OPTION_DATA* CControlWnd::GetData(int row, int col)
{
	if (row <= 0)
		return nullptr;

	const	BOOL	bCall = col<C_HSGA;
	const	BOOL	bPut  = col>C_HSGA;
	CString key;
	std::shared_ptr<OPTION_DATA> val = nullptr;
	POSITION pos = m_data.GetStartPosition();

	while (pos)
	{
		m_data.GetNextAssoc(pos, key, val);
		if (val->grid_row == row)
		{
		    if (val->is_call() && bCall && !bPut)
				return val.get();
			if (val->is_put() && !bCall && bPut)
				return val.get();
		}
	}
	return nullptr;
}

void CControlWnd::OnHsgaChanged() 
{
	const	double hsga = atof(m_hsga);

	for (int row = 1; row < m_pGrid->GetRowCount(); ++row)
	{
		if (hsga == atof(m_pGrid->GetItemText(row, C_HSGA)))
		{
			CRect	rc;

			m_HsgaRow = row;
			m_pGrid->SetItemBkColour(row, C_HSGA, m_crLine);
			m_pGrid->RedrawCell(row, C_HSGA);

			GetClientRect(&rc);
			m_pGrid->SetScrollPos(SB_VERT, (row)*m_pGrid->GetRowHeight(row) - rc.Height()/2, TRUE);
			m_pGrid->PostMessage(WM_VSCROLL, SB_THUMBPOSITION, 0);
			break;
		}
	}
}

void CControlWnd::ChangeTheme()
{
	if (!m_pGrid || !m_pGrid->GetSafeHwnd())
		return;

	const	COLORREF head_bg = GetIndexColor(GRID_HEAD_CLR);
	const	COLORREF head_fg = GetIndexColor(GRID_HEAD_TEXT_CLR);
	for (int col = 0; col < m_pGrid->GetColumnCount(); ++col)
	{
		if (strHeads[col])
			m_pGrid->SetItemText(0, col, strHeads[col]);
		m_pGrid->SetColumnWidth(col, iHeads[col]);
		m_pGrid->SetItemFormat(0, col, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		m_pGrid->SetItemFgColour(0, col, head_fg);
		m_pGrid->SetItemBkColour(0, col, head_bg);
	}
	m_pGrid->Refresh();
}

void CControlWnd::OnTimer(UINT nIDEvent) 
{
	if (m_qdata.IsEmpty())
		return;

	CDC*	pdc = m_pGrid->GetDC();
	if (pdc)
	{
		CString	key;
		std::shared_ptr<OPTION_DATA> val = nullptr;
		POSITION pos = m_qdata.GetStartPosition();

		while (pos)
		{
			m_qdata.GetNextAssoc(pos, key, val);
			DrawCode(val);
		}

		m_pGrid->ReleaseDC(pdc);
	}

	m_qdata.RemoveAll();
	m_DrawQueue = 0;

	CWnd::OnTimer(nIDEvent);
}

void CControlWnd::SetPredict(short nIndex) 
{
	m_nPredict = nIndex;

	if (m_nType == 0)
	{
		if (m_nPredict == 0)
		{
			m_pGrid->SetItemText(0, 2, strHeads[2]);
			m_pGrid->SetItemText(0, 3, strHeads[3]);
			m_pGrid->SetItemText(0, 4, strHeads[4]);
			m_pGrid->SetItemText(0, 6, strHeads[6]);
			m_pGrid->SetItemText(0, 7, strHeads[7]);
			m_pGrid->SetItemText(0, 8, strHeads[8]);
		}
		else
		{
			m_pGrid->SetItemText(0, 2, "예상량");
			m_pGrid->SetItemText(0, 3, "대비");
			m_pGrid->SetItemText(0, 4, "예상가");
			m_pGrid->SetItemText(0, 6, "예상가");
			m_pGrid->SetItemText(0, 7, "대비");
			m_pGrid->SetItemText(0, 8, "예상량");
		}
	}
	else if (m_nType == 1)
	{
		if (m_nPredict == 0)
		{
			m_pGrid->SetItemText(0, 3, strSHeads[3]);
			m_pGrid->SetItemText(0, 4, strSHeads[4]);
			m_pGrid->SetItemText(0, 6, strSHeads[6]);
			m_pGrid->SetItemText(0, 7, strSHeads[7]);
		}
		else
		{
			m_pGrid->SetItemText(0, 3, "예상량");
			m_pGrid->SetItemText(0, 4, "예상가");
			m_pGrid->SetItemText(0, 6, "예상가");
			m_pGrid->SetItemText(0, 7, "예상량");
		}
	}

	m_pGrid->Refresh();
}

void CControlWnd::SetType(short nType) 
{
	m_nType = nType;

	if (m_nType == 0)
	{
		for (int col = 0; col < m_pGrid->GetColumnCount(); ++col)
		{
			if (strHeads[col])
				m_pGrid->SetItemText(0, col, strHeads[col]);
			m_pGrid->SetColumnWidth(col, iHeads[col]);
		}

		if (m_nPredict == 0)
		{
			m_pGrid->SetItemText(0, 2, strHeads[2]);
			m_pGrid->SetItemText(0, 3, strHeads[3]);
			m_pGrid->SetItemText(0, 4, strHeads[4]);
			m_pGrid->SetItemText(0, 6, strHeads[6]);
			m_pGrid->SetItemText(0, 7, strHeads[7]);
			m_pGrid->SetItemText(0, 8, strHeads[8]);
		}
		else
		{
			m_pGrid->SetItemText(0, 2, "예상량");
			m_pGrid->SetItemText(0, 3, "대비");
			m_pGrid->SetItemText(0, 4, "예상가");
			m_pGrid->SetItemText(0, 6, "예상가");
			m_pGrid->SetItemText(0, 7, "대비");
			m_pGrid->SetItemText(0, 8, "예상량");
		}
	}
	else if (m_nType == 1)
	{
		for (int col = 0; col < m_pGrid->GetColumnCount(); ++col)
		{
			if (strSHeads[col])
				m_pGrid->SetItemText(0, col, strSHeads[col]);
			m_pGrid->SetColumnWidth(col, iSHeads[col]);
		}

		if (m_nPredict == 0)
		{
			m_pGrid->SetItemText(0, 3, strSHeads[3]);
			m_pGrid->SetItemText(0, 4, strSHeads[4]);
			m_pGrid->SetItemText(0, 6, strSHeads[6]);
			m_pGrid->SetItemText(0, 7, strSHeads[7]);
		}
		else
		{
			m_pGrid->SetItemText(0, 3, "예상량");
			m_pGrid->SetItemText(0, 4, "예상가");
			m_pGrid->SetItemText(0, 6, "예상가");
			m_pGrid->SetItemText(0, 7, "예상량");
		}
	}

	m_pGrid->Refresh();
}
