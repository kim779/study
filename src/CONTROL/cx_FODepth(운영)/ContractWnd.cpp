// ContractWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FODepth.h"
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
	m_pWizard = pWizard;
	m_pParent = pParent;
	m_pApp = nullptr;

	m_display = 10;
	m_bAlert  = false;
	m_digit = 0;
	m_max = 30;
	m_half = 0;
	m_grid = nullptr;
	m_pFont = nullptr;
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
	
	m_pApp = (CCX_FODepthApp*)AfxGetApp();

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

	struct _EW_data	*ew = (_EW_data *)pData;
	int	count = atoi(CString(ew->nContract, sizeof(ew->nContract)));
	int	cp = 0, cc = 0;
	CString	sPrice, sContract;
	char	sign = 0;

	for (int ii = 0; ii < count; ii++)
	{
		sPrice = CString(ew->contract[ii].price, sizeof(ew->contract[ii].price));
		sContract = CString(ew->contract[ii].contract, sizeof(ew->contract[ii].contract));
		sign = sPrice.GetAt(0);
		if (sign != '+' && sign != '-')
			sPrice.Insert(0, ' ');

		cp = CommaModify(sPrice, m_digit);
		cc = CommaModify(sContract, 0);

		if ((sPrice == "0.00" || sPrice == "0") && sContract == "0")
			continue;

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
	for (POSITION pos = list.GetHeadPosition(); pos;)
	{
		str = list.GetNext(pos);
		WriteAlert(str, sPrice, sVolume);
		
	}
}

void CContractWnd::Alert(struct _alertR* alertR)
{
	if (!m_bAlert) 
		return;
	
	DWORD*	data = nullptr;
	
	for (int ii = alertR->size-1; ii >= 0; ii--)
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

	m_grid->SetFontx(m_pFont);
}

void CContractWnd::Palette()
{
	CCX_FODepthApp	*pApp = (CCX_FODepthApp*)AfxGetApp();
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
	CString price = "", volume = "", contract = "";

	while (!str.IsEmpty())
	{
		sym = m_pApp->Parser(str, "\t");
		dat = m_pApp->Parser(str, "\t");
		//dat.TrimLeft();
		dat.TrimRight();
		if (!dat.Compare("0"))
			dat.Empty();

		switch (atoi(sym))
		{
		case 23: //현재가
			price = dat; 
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
	if (!price.IsEmpty() && !contract.IsEmpty())
	{
		// 2010.09.02 상품(금,미니금,돈육의 경우 m_digit = 0, pow = 1. 이어야함)
		const	int	cp = CommaModify(price, m_digit, (float)((m_digit > 0) ? 100. : 1.));
		const	int	cc = CommaModify(contract, 0);
		m_grid->InsertData(price, contract, cp, cc);
	}
}

void CContractWnd::WriteAlert(DWORD* data)
{
	CString price, volume, contract;

	if (data[23])
	{
		price = (char*)data[23];
		//price.TrimLeft();
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
	
	if (!price.IsEmpty() && !contract.IsEmpty())
	{
		const	int	cp = CommaModify(price, m_digit, (float)((m_digit>0) ? 100. : 1.));
		const	int	cc = CommaModify(contract, 0);
		m_grid->InsertData(price, contract, cp, cc);
	}
}

void CContractWnd::ResizeToFit()
{
	if (!m_grid)
		return;

	int	scrWidth = 16;
	CRect RC, gridRC, scrRC;

	GetClientRect(RC);
	if (m_pFont)
	{
		CDC* pDC = GetDC();
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

int CContractWnd::CommaModify(CString &str, int digit, float Pow)
{
	int	clrIndex = clr_NormalIndex;

	switch (str[0])
	{
	case '1':
	case '2':
	case '+':
		clrIndex = clr_UpIndex; 
		str.Delete(0);
		break;
	case '4':
	case '5':
	case '-':
		clrIndex = clr_DnIndex;
		str.Delete(0);
		break;
	case '0':	//2014.06.09 KSJ 0이 올때는 atof에서 0이 되므로 삭제해줌.
	case '3':	//2014.06.09 KSJ 0이 올때는 atof에서 0이 되므로 삭제해줌.
	case ' ':
		str.Delete(0);
		break;
	}
	
	m_pApp->VariableComma(str, atof(str)*Pow, digit);
	
	return clrIndex;
}