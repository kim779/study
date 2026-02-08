// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_ContractX.h"
#include "ControlWnd.h"
#include "GridWnd.h"
#include "ctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FUTURE	1
#define OPTION	2
#define STOCK	3

/////////////////////////////////////////////////////////////////////////////
// CControlWnd


CControlWnd::CControlWnd(CWnd *pParent, struct _param *pParam)
{
	EnableAutomation();

	m_pParent = pParent;
	m_display = 10;
	setParam(pParam);
	if (m_display <= 0)
		m_display = 10;

	m_bWait   = false;
	m_bAlert  = false;

	m_max = 30;
	m_grid = nullptr;
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
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_NOTIFY(CControlWnd, "JCode", m_jCode, OnJCodeChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "visible", m_visible, OnVisibleChanged, VT_BOOL)
	DISP_FUNCTION(CControlWnd, "Clear", Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SendData", SendData, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetRect", _SetRect, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {C0713FB8-CEFD-4D4E-A3D0-56EF5848BFB9}
static const IID IID_IControlWnd =
{ 0xc0713fb8, 0xcefd, 0x4d4e, { 0xa3, 0xd0, 0x56, 0xef, 0x58, 0x48, 0xbf, 0xb9 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

// event
long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		{
			struct	_extTHx* exth = (struct _extTHx*)lParam;
			CString str = CString(exth->data, exth->size);
			CString sCode = Parser(str, "\t");
			CString	price, gvol;
			int	idx = 0;
			struct	_gridHo* gridHo = nullptr;

			DoWait(false);
			m_sRTSCode = Parser(str, "\t");
			idx = sCode.GetLength() + m_sRTSCode.GetLength() + 2;	// 2 tab

			gridHo = (struct _gridHo*)&exth->data[idx];
			str = CString(&exth->data[idx + sizeof(struct _gridHo)]);
			while (!str.IsEmpty())
			{
				if (str.Find("\n") == -1)
					break;

				gvol = Parser(str, "\n");
				price = Parser(gvol, "\t");

				if (m_gubn == STOCK)
					price = CommaModify(price);
				gvol = CommaModify(gvol);

				m_grid->AddData(price, gvol);
			}
			m_grid->AutoUpdate();
			//SetTimer(1000, 100, NULL);
			m_bAlert = true;
		}
		break;

	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!
		{
			CString str = CString((char*)lParam, HIWORD(wParam));
			CString sCode = Parser(str, "\t");
			struct	_gridHo	gridHo {};

			DoWait(false);
			m_sRTSCode = Parser(str, "\t");
			lParam += sCode.GetLength() + m_sRTSCode.GetLength() + 2;
			CopyMemory(&gridHo, (char*)(lParam), sizeof(struct _gridHo));
			str = CString((char*)(lParam + sizeof(struct _gridHo)));

			CString	price, gvol;
			while (!str.IsEmpty())
			{
				if (str.Find("\n") == -1)
					break;

				gvol = Parser(str, "\n");
				price  = Parser(gvol, "\t");

				if (m_gubn == STOCK)
					price = CommaModify(price);
				gvol = CommaModify(gvol);

				m_grid->AddData(price, gvol);
			}
			m_grid->AutoUpdate();
			//SetTimer(1000, 100, NULL);
			m_bAlert = true;
		}
		break;

	case DLL_ALERTx:
		if (m_bAlert)
			OnAlertx((struct _alertR*)lParam);
		break;

	case DLL_ALERT:
		/*
		if (!m_bAlert) 
		{
			return 0;
		}
		OnAlert((const char*)lParam);
		*/
		break;

	case DLL_SETFONT:
	case DLL_SETFONTx:
		{
			CString	sFont = (char*)lParam;
			sFont.TrimLeft();
			if (!sFont.IsEmpty())
				m_param.fonts = sFont;
			m_param.point = HIWORD(wParam);

			m_pFont = ((CCX_ContractXApp*)AfxGetApp())->GetFont(m_pParent, m_param.point, m_param.fonts, 0, false);
			m_grid->SetFontx(m_pFont);
			ResizeToFit();
		}
		break;
	default:
		break;
	}
	return 0;
}

void CControlWnd::OnTimer(UINT nIDEvent) 
{
/*
	static int	cnt = 0;
	static int	prc = 100000;
	static int	vol = 10000;
	CString	str;

	cnt++;
	prc = 10000 + (cnt/10)*10 + cnt%10;
	vol = 1000  + (cnt/10)*10 + cnt%10;

	CString	sgubn1 = "", sgubn2 = "";
	if (cnt%3)	sgubn1 = "+";
	if (cnt%4)	sgubn2 = "+";
	if (cnt%5)	sgubn1 = "-";
	if (cnt%7)	sgubn2 = "-";

	str.Format("%s\t023\t%s%d\t032\t%s%d\t", m_sRTSCode, sgubn1, prc, sgubn2, vol);
	SendMessage(WM_USER, DLL_ALERT, long(str.operator LPCTSTR()));
*/
	CWnd::OnTimer(nIDEvent);
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	const BYTE COLOR_NORMAL = 69;
	const BYTE COLOR_UP	= 94;
	const BYTE COLOR_DOWN   = 95;
	const BYTE COLOR_LINE	= 75;

	CCX_ContractXApp* pApp = (CCX_ContractXApp*)AfxGetApp();
	m_pBasicFont = pApp->GetFont(m_pParent, 9, m_param.fonts, 0, false);
	m_pFont = pApp->GetFont(m_pParent, m_param.point, m_param.fonts, 0, false);

	m_grid = std::make_unique<CGridWnd>(m_pParent, this);
	m_grid->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, 101);
	m_grid->SetTextColor(pApp->GetColor(m_pParent, COLOR_UP), pApp->GetColor(m_pParent, COLOR_DOWN), 
		pApp->GetColor(m_pParent, COLOR_NORMAL), pApp->GetColor(m_pParent, COLOR_LINE));

	m_grid->SetDisplay(m_display);
	m_grid->SetColInfo(m_col1, m_col2);
	m_grid->SetFontx(m_pFont);

	m_BasicSize = CSize(0,0);
	if (m_pBasicFont)
	{
		CDC*	pDC = GetDC();
		if (pDC) 
		{
			CFont* pOldFont = pDC->SelectObject(m_pBasicFont);
			m_BasicSize = pDC->GetOutputTextExtent(_T("9"));
			pDC->SelectObject(pOldFont);
		}
		ReleaseDC(pDC);
	}

	return 0;
}

void CControlWnd::OnDestroy() 
{
	if (m_grid)
		m_grid.reset();
	
	CWnd::OnDestroy();
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	ResizeToFit();	
}

/////////////////////////////////////////////////////////////////////////////
// CControlWnd Automation

void CControlWnd::OnJCodeChanged() 
{
	//KillTimer(1000);
}

void CControlWnd::OnVisibleChanged() 
{
	ShowWindow(m_visible);
}

void CControlWnd::Clear() 
{
	m_bAlert = false;

	m_grid->Clear();
}

void CControlWnd::SendData() 
{
	if (IsWait() || m_jCode.IsEmpty())
		return;

	Clear();
	DoWait();

	CString trData;
	struct	_gridHi	gridHi;

	FillMemory(&gridHi, sizeof(gridHi), ' ');
	trData.Format("%02d", m_rows);
	CopyMemory(gridHi.visible, trData, 2);
	CopyMemory(gridHi.rows, "0200", 4);
	gridHi.type = '1';
	gridHi.dir  = '1';
	gridHi.sort = '0';
	gridHi.key = '4';

	CString sTmp;
	switch (m_gubn) 
	{
	case FUTURE:
		sTmp = "30301%c%s\t30301\t30021\t34309%c%d\t$34310%c%s%s\t";
		trData.Format(sTmp, 
			0x7f, m_jCode, 
			0x7f, 0,
			0x7f, CString((char*)&gridHi, sizeof(gridHi)), "34023\n34032\n");
		break;
	case OPTION:
		sTmp = "40301%c%s\t40301\t40021\t44309%c%d\t$44310%c%s%s\t";
		trData.Format(sTmp, 
			0x7f, m_jCode, 
			0x7f, 0,
			0x7f, CString((char*)&gridHi, sizeof(gridHi)), "44023\n44032\n");
		break;
	case STOCK:
		sTmp = "1301%c%s\t1301\t1021\t11309%c%d\t$11310%c%s%s\t";
		trData.Format(sTmp, 
			0x7f, m_jCode, 
			0x7f, 0,
			0x7f, CString((char*)&gridHi, sizeof(gridHi)), "11023\n11032\n");
		break;
	default:
		return;
	}

	SendTR("pooppoop", trData, US_OOP, m_param.key);
}

/////////////////////////////////////////////////////////////////////////////
// CControlWnd Function

void CControlWnd::Init()
{
}

void CControlWnd::setParam(struct _param *pParam)
{
	CString	keys, text, tmps;

	m_param.key   = pParam->key;
	m_param.name  = CString(pParam->name, pParam->name.GetLength());
	m_param.rect  = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point = pParam->point;
	m_param.style = pParam->style;
	m_param.tRGB  = pParam->tRGB;
	m_param.pRGB  = pParam->pRGB;
	m_param.options = CString(pParam->options, pParam->options.GetLength());

	tmps = _T("/ ");
	keys = _T("qwerdm");
	for (int ii = 0, idx = 0, pos = 0; ii < keys.GetLength(); ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = m_param.options.Find(tmps);
		if (idx < 0)
			continue;

		idx += 2;
		pos = m_param.options.Find('/', idx);

		text = (pos < 0) ? m_param.options.Mid(idx) : m_param.options.Mid(idx, pos-idx);
		text.TrimLeft();

		switch (keys[ii])
		{
		case 'q':
			m_gubn = atoi(text);
			break;
		case 'w':
			m_col1 = atoi(text)-1;
			break;
		case 'e':
			m_col2 = atoi(text);
			m_col2 -= 4;
			break;
		case 'r':
			m_rows = atoi(text);
			m_display = m_rows;
			break;
		case 'd':
			m_display = atoi(text);
			break;
		case 'm':
			m_max = atoi(text);
			break;
		}
	}

	if (m_param.fonts.IsEmpty())		m_param.fonts = "굴림";
//	if (m_param.point < 5)			m_param.point = 9;
}

void CControlWnd::OnAlertx(struct _alertR* alertR)
{
	if (m_sRTSCode.CompareNoCase(alertR->code))
		return;

	CString price, gvol;
	DWORD*	data = nullptr;

	for (int ii = alertR->size - 1; ii >= 0; ii--)
	{
		data = (DWORD *)alertR->ptr[ii];
		
		price = gvol = "";
		if (data[23])	// 현재가
		{
			price = (char *)data[23];
			price.TrimLeft();
			if (atoi(price) == 0)
				continue;
		}
		if (data[27])	//거래량
		{
			gvol = (char *)data[27];
			gvol.TrimLeft();
			if (atoi(gvol) == 0)
				continue;
		}

		if (!price.IsEmpty() && !gvol.IsEmpty())
		{
			if (m_gubn == STOCK)
				price = CommaModify(price);
			gvol = CommaModify(gvol);
			m_grid->InsertData(price, gvol);
		}
	}
}

void CControlWnd::OnAlert(const char* param)
{
	CString str(param);

	if (!Parser(str, "\t").CompareNoCase(m_sRTSCode))
	{
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

		for (POSITION pos = list.GetHeadPosition(); pos; )
		{
			str = list.GetNext(pos);
			WriteAlert(str);
		}
	}
}

void CControlWnd::WriteAlert(CString str)
{
	CString sym, dat;
	CString nonsetl, nonDaebi;
	CString price = "", gvol = "", qty = "";

	while (!str.IsEmpty())
	{
		sym = Parser(str, "\t");
		dat = Parser(str, "\t");
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
			qty = dat;
			break;
		case 32: //체결량
			gvol = dat;
			gvol.TrimLeft();
			break;
		default:
			continue;
		}
	}

	if (!price.IsEmpty() && !gvol.IsEmpty())
	{
		if (m_gubn == STOCK)
			price = CommaModify(price);
		gvol = CommaModify(gvol);
		m_grid->InsertData(price, gvol);
	}
}

void CControlWnd::SendTR(CString name, CString data, BYTE type, int key)
{
	const	int	datl = data.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(datl + L_userTH + 128);
	struct	_userTH	udat {};
	int	idx = m_param.name.GetLength();

	// control name
	CopyMemory(pWb.get(), m_param.name, idx);
	pWb[idx++] = '\t';

	// userTH
	CopyMemory(udat.trc, name, min(sizeof(udat.trc), name.GetLength()));
	udat.key = key;
	udat.stat = type;
	CopyMemory(&pWb[idx], (char*)&udat, L_userTH);
	idx += L_userTH;

	// data
	CopyMemory(&pWb[idx], data, datl);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)pWb.get());
}

CString CControlWnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}

	CString  temp = srcstr.Left(srcstr.Find(substr));
	srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	return temp;
}

void CControlWnd::DoWait(bool bWait)
{
	m_bWait = bWait;
}

bool CControlWnd::IsWait()
{
	return m_bWait;
}

CString CControlWnd::CommaModify(CString string)
{
	CString stripData = string;

	stripData.Trim();
	if (stripData.GetLength() <= 3)
		return stripData;

	if (stripData.Find('.') != -1)
		return stripData;

	char	dChar = 0x00;
	switch (stripData[0])
	{
	case '+':
		dChar = '+';
		stripData.Delete(0);
		break;
	case '-':
		dChar = '-';
		stripData.Delete(0);
		break;
	}

	stripData.MakeReverse();
	string = _T("");
	for (int ii = 0; ii < stripData.GetLength(); ii++)
	{
		if ((ii != 0) && ((ii % 3) == 0))
			string += ',';

		string += stripData[ii];
	}
	string.MakeReverse();
	if (dChar != 0x00)
		string.Insert(0, dChar);

	return string;
}

void CControlWnd::ResizeToFit()
{
	if (!m_grid)
		return;

	CRect	RC, scrRC;
	int	scrWidth = 16;

	GetClientRect(RC);
	if (m_pFont)
	{
		CDC*	pDC = GetDC();
		if (pDC) 
		{
			CFont* pOldFont = pDC->SelectObject(m_pFont);
			const	CSize	sz = pDC->GetOutputTextExtent(_T("9"));
			pDC->SelectObject(pOldFont);

			scrWidth = m_BasicSize.cx + 10;
			if (m_BasicSize.cx > 0 && sz.cx > m_BasicSize.cx)
				scrWidth = m_BasicSize.cx + 10 + (sz.cx - m_BasicSize.cx)*2;
		}
		ReleaseDC(pDC);
	}

	scrRC = RC;
	RC.right -= scrWidth;
	scrRC.left = RC.right;
	m_grid->MoveWindow(RC, TRUE);
	m_grid->m_pScroll->MoveWindow(scrRC, TRUE);
}

void CControlWnd::_SetRect(short left, short top, short right, short bottom) 
{
	struct	_rectR	rc {};

	rc.key    = m_param.key;
	rc.ctrl   = (long)this;
	rc.left   = left;
	rc.top    = top;
	rc.right  = right;
	rc.bottom = bottom;

	m_pParent->SendMessage(WM_USER, setRECT, (LPARAM)&rc);
}