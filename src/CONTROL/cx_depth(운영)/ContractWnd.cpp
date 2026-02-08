// ContractWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Depth.h"
#include "ContractWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FUTURE	1
#define OPTION	2
#define STOCK	3
const UINT IDC_GRID = 1003;
const BYTE CLR_NORMAL = 69;
const BYTE CLR_UP	= 94;
const BYTE CLR_DOWN   = 95;
const BYTE CLR_LINE	= 75;
const BYTE CLR_GRID1ST	= 68;	// grid 1st back
const BYTE CLR_GRID2ST	= 77;	// grid 2st back


/////////////////////////////////////////////////////////////////////////////
// CContractWnd

CContractWnd::CContractWnd(CWnd *pWizard, CWnd *pParent)
{
	m_pApp = (CCX_DepthApp*)AfxGetApp();

	m_pWizard = pWizard;
	m_pParent = pParent;

	m_display = 10;
	m_bAlert  = false;

	m_digit = 0;
	m_half = 0;
	m_max = 30;
	m_grid = NULL;
	m_pFont = NULL;
}

CContractWnd::~CContractWnd()
{
}


BEGIN_MESSAGE_MAP(CContractWnd, CWnd)
	//{{AFX_MSG_MAP(CContractWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CContractWnd message handlers
int CContractWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pApp = (CCX_DepthApp*)AfxGetApp();

	m_grid = std::make_unique<CContractGrid>(m_pWizard, this);
	m_grid->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 101);
	m_grid->PaletteGrid(m_pApp->GetColor(m_pWizard, CLR_UP), m_pApp->GetColor(m_pWizard, CLR_DOWN), 
		m_pApp->GetColor(m_pWizard, CLR_NORMAL), m_pApp->GetColor(m_pWizard, CLR_LINE), 
		m_pApp->GetColor(m_pWizard, CLR_GRID1ST), m_pApp->GetColor(m_pWizard, CLR_GRID2ST));

	m_grid->SetDisplay(m_display);

	m_BasicSize = CSize(14,14);
	
	return 0;
}

void CContractWnd::OnDestroy() 
{
	if (m_grid)
		m_grid.reset();
	
	CWnd::OnDestroy();
}

void CContractWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	ResizeToFit();
}

void CContractWnd::Init(int DisplayCnt, int half)
{
	m_grid->Init(DisplayCnt);
	m_half = half;
}

void CContractWnd::Clear() 
{
	m_bAlert = false;
	m_grid->Clear();
}

void CContractWnd::Dispatch(char *pData, int digit)
{
	Clear();
	m_digit = digit;

	const	struct _EW_data	*ew = (_EW_data *)pData;
	int	count = atoi(CString(ew->nContract, sizeof(ew->nContract)));
	int	cp = 0, cc = 0;
	CString	sPrice, sContract;

	for (int ii = 0; ii < count; ii++)
	{
		sPrice = CString(ew->contract[ii].price, sizeof(ew->contract[ii].price));
		sContract = CString(ew->contract[ii].contract, sizeof(ew->contract[ii].contract));

		cp = CommaModify(sPrice, m_digit);
		cc = CommaModify(sContract, 0);

		if (sPrice.GetLength() > 7) sPrice.Remove(',');
		m_grid->AddData(sPrice, sContract, cp, cc);
	}
	m_grid->AutoUpdate();
	m_bAlert = true;
}

void CContractWnd::Alert(CString sRts)
{
	if (!m_bAlert) 
		return;

	CString str = sRts;
	CStringList list;
	int	find = 0, index = 0;

	while (TRUE)
	{
		index = str.Find('\n', find);
		if (index < 0)
			break;

		list.AddHead(str.Mid(find, index - find));
		find = index + 1;
	}

	if (find < str.GetLength() - 1)
		list.AddHead(str);

	CString	sPrice, sVolume;
	for (POSITION pos = list.GetHeadPosition(); pos; )
	{
		str = list.GetNext(pos);
		WriteAlert(str, sPrice, sVolume);
	}
}

void CContractWnd::Alert(struct _alertR* alertR)
{
	if (!m_bAlert) 
		return;
	
	CString	sPrice, sVolume;
	DWORD*	data = nullptr;

	for (int ii = alertR->size - 1; ii >= 0; ii--)
	{
		data = (DWORD*)alertR->ptr[ii];
		WriteAlert(data);
	}
}
void CContractWnd::Font(CFont *pFont)
{
	m_pFont = pFont;

	CDC* pDC = GetDC();
	if (pDC) 
	{
		CFont* pOldFont = pDC->SelectObject(m_pFont);
		m_BasicSize = pDC->GetOutputTextExtent(_T("9"));
		pDC->SelectObject(pOldFont);
	}
	ReleaseDC(pDC);
	ResizeToFit();

	m_grid->SetFontx(m_pFont);
}

void CContractWnd::Palette()
{
	CCX_DepthApp	*pApp = (CCX_DepthApp*)AfxGetApp();
	m_grid->PaletteGrid(pApp->GetColor(m_pWizard, CLR_UP), pApp->GetColor(m_pWizard, CLR_DOWN), 
		pApp->GetColor(m_pWizard, CLR_NORMAL), pApp->GetColor(m_pWizard, CLR_LINE),
		m_pApp->GetColor(m_pWizard, CLR_GRID1ST), m_pApp->GetColor(m_pWizard, CLR_GRID2ST));
	m_grid->Invalidate();
}

void CContractWnd::ChangeDisplay(int DisplayCnt)
{
	m_display = DisplayCnt;
	m_grid->ChangeDisplay(m_display);
}

void CContractWnd::WriteAlert(CString str, CString &sPrice, CString &sVolume)
{
	CString sym, dat, sRow;
	CString nonsetl, nonDaebi;
	CString price, volume, contract;

	while (!str.IsEmpty())
	{
		sym = m_pApp->Parser(str, "\t");
		dat = m_pApp->Parser(str, "\t");
		dat.Trim();
		if (!dat.Compare("0"))
			dat.Empty();

		switch (atoi(sym))
		{
		case 23: //현재가
			price = dat; 
			price.TrimLeft();
			break;
		case 27: //거래량
			volume = dat;
			break;
		case 32: //체결량
			contract = dat;
			contract.TrimLeft();
			break;
		default:
			continue;
		}
	}

	sPrice = price;
	sVolume = volume;

	int	cp = 0, cc = 0;
	if (!price.IsEmpty() && !contract.IsEmpty())
	{
		cp = CommaModify(price, m_digit);
		cc = CommaModify(contract, 0);
		m_grid->InsertData(price, contract, cp, cc);
	}
}

void CContractWnd::WriteAlert(DWORD* data)
{
	CString sym, dat, sRow;
	CString nonsetl, nonDaebi;
	CString price, volume, contract;

	if (data[23])
	{
		price = (char*)data[23];
		price.TrimLeft();
	}

	if (data[27])
	{
		volume = (char*)data[27];
	}

	if (data[32])
	{
		contract = (char*)data[32];
		contract.TrimLeft();
	}

	int	cp = 0, cc = 0;
	if (!price.IsEmpty() && !contract.IsEmpty())
	{
		cp = CommaModify(price, m_digit);
		cc = CommaModify(contract, 0);

		if (price.GetLength() > 7) price.Remove(',');

		m_grid->InsertData(price, contract, cp, cc);
	}
}

void CContractWnd::ResizeToFit()
{
	if (!m_grid)
		return;

	CRect	RC, gridRC, scrRC;
	int	scrWidth = 16;

	GetClientRect(RC);
	if (m_pFont)
	{
		CDC*	pDC = GetDC();
		if (pDC) 
		{
			CFont*	pOldFont = pDC->SelectObject(m_pFont);
			const	CSize	sz = pDC->GetOutputTextExtent(_T("9"));
			pDC->SelectObject(pOldFont);

			scrWidth = m_BasicSize.cx + 10;
			if (m_BasicSize.cx > 0 && sz.cx > m_BasicSize.cx)
				scrWidth = m_BasicSize.cx + 10 + (sz.cx - m_BasicSize.cx)*2;
		}
		ReleaseDC(pDC);
	}

	gridRC = scrRC = RC;
	gridRC.right -= scrWidth;
	scrRC.left = gridRC.right;
	RC.left += 1;

	const	int	half = 1 + (RC.Width() * m_half) / 100;
	m_grid->SetHalfLineInfo(half);
	m_grid->MoveWindow(gridRC, TRUE);
	m_grid->m_pScroll->MoveWindow(scrRC, TRUE);
}

int CContractWnd::CommaModify(CString &str, int digit)
{
	int	clrIndex = clr_NormalIndex;
	switch (str[0])
	{
	case '+':
		clrIndex = clr_UpIndex; 
		str.Delete(0);
		break;
	case '-':
		clrIndex = clr_DnIndex;
		str.Delete(0);
		break;
	}

	m_pApp->VariableComma(str, atof(str), digit);

	return clrIndex;
}
