// GrpWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Cx_MiniChart.h"
#include "GrpWnd.h"
#include "GrpData.h"
#include "memDC.h"
#include "GrpLine.h"
#include "ToolTip.h"
#include "CrossLine.h"
#include "JisuConfigDlg.h"
#include "event.h"
#include "../../h/axisgenv.h"
#include "../../h/axisgwin.h"
#include "MainWnd.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGrpWnd
#define	TIMER_PID	10000
#define	TIMER_PGAP	250
#define	TIMER_ID	10001
#define	TIMER_GAP	1000
#define	TIMER_TR	10002

CGrpWnd::CGrpWnd(CWnd *pParent, CWnd* pMainWnd, char* mj, CParam* pParam)
{
	EnableAutomation();

 	m_pApp = (CCx_MiniChartApp *)AfxGetApp();

	SetParam(pParam);

	CopyMemory(&m_mj, mj, sz_MINIJISU);
	m_pView = pParent;
	m_pMainWnd = pMainWnd;

	m_pFont = nullptr;
	m_fPoint = 9;
	m_fName.Format("굴림");
	
	m_TRIndex = 0;

	m_GrpCnt = 0;
	m_pGrpData = nullptr;
	for (int ii = 0; ii < MAX_JISU; ii++)
		m_pGrpInfo[ii] = nullptr;

	m_tRGB = RGB(233, 233, 233);
	m_pRGB = RGB(255,255,255);
	m_fRGB = RGB(240,240,240);
	m_tkInfo.tkColor[ctkLEFT] = m_tRGB;
	m_tkInfo.tkColor[ctkRIGHT] = m_tRGB;
	m_tkInfo.tkColor[ctkBOTTOM] = m_tRGB;

	m_TickView = 0;
	if (m_mj.tick & TK_LEFT)	m_TickView |= CDT_LEFT;
	if (m_mj.tick & TK_RIGHT)	m_TickView |= CDT_RIGHT;

	m_pToolTip = std::make_unique<CToolTip>(m_pView, this, pMainWnd);
	m_pCrossLine = std::make_unique<CCrossLine>(m_pView, this, pMainWnd);
	m_PosData = _T("");
	m_timerID = 0;

	//ZeroMemory(m_pRcv, sizeof(m_pRcv));
	ZeroMemory(m_szRcv, sizeof(m_szRcv));	
	m_pBitmap = ((CMainWnd*)m_pMainWnd)->GetBitmap(((CMainWnd*)m_pMainWnd)->GetImgPath() + _T("환경설정.bmp"));
}

CGrpWnd::~CGrpWnd()
{
	for (auto& item : m_pRcv)
		item.reset();
}

BEGIN_MESSAGE_MAP(CGrpWnd, CWnd)
	//{{AFX_MSG_MAP(CGrpWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_TIMER()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CGrpWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CGrpWnd)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGrpWnd message handlers
LONG CGrpWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	const LONG	ret = 0;
	struct _extTHx* exTHx = (struct _extTHx*)lParam;

	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:	// set data from server
		{
			m_szRcv[m_TRIndex] = exTHx->size;
			if (m_szRcv[m_TRIndex])
			{
				m_pRcv[m_TRIndex] = std::make_unique<char[]>(m_szRcv[m_TRIndex]);
				CopyMemory(m_pRcv[m_TRIndex].get(), exTHx->data, m_szRcv[m_TRIndex]);
			}

			if (++m_TRIndex < MAX_JISU)
			{
				RequestData(m_TRIndex);
			}
			else
			{
				DispatchData();

				for (int ii = 0; ii < m_TRIndex; ii++)
				{
					m_szRcv[ii] = 0;
					m_pRcv[ii].reset();
				}
			}
		}
		break;
	case DLL_INB:	// get data from out
		break;
	case DLL_ALERTx:	// realtime data
		RealTimeData((struct _alertR*)lParam);
		break;
	case DLL_SETPAL:
		break;
	case DLL_SETFONT:
		{
			m_fPoint = HIWORD(wParam);
			CString fName = (char *)lParam;
			if (!fName.IsEmpty())
				m_fName.Format("%s", fName);

			m_pFont = ((CMainWnd*)m_pMainWnd)->GetFont(m_fPoint, m_fName);

			Resize();
			Invalidate();
		}
		break;
	}

	return ret;
}


void CGrpWnd::SetParam(CParam *pParam)
{
	m_param.key = pParam->key;
	m_param.name = CString(pParam->name, pParam->name.GetLength());
	m_param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point = pParam->point;
	m_param.style = pParam->style;
	// 	m_clrForeColor = m_Param.tRGB = pParam->tRGB;
	// 	m_clrBackColor = m_Param.pRGB = pParam->pRGB;
	m_param.options = CString(pParam->options, pParam->options.GetLength());
}



int CGrpWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pToolTip->Create();
	m_pToolTip->ShowWindow(SW_HIDE);

	m_pFont = ((CMainWnd*)m_pMainWnd)->GetFont(m_fPoint, m_fName);

	m_TRIndex = 0;
//	RequestData(m_TRIndex);
//	SetTimer(TIMER_TR, 4, nullptr);
	return 0;
}

void CGrpWnd::init()
{
	m_TRIndex = 0;
	RequestData(m_TRIndex);
}

void CGrpWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CMemDC	mdc(&dc);
	DrawGraph(&mdc);
}

void CGrpWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx == 0 || cy == 0)
		return;

	Resize();
}

void CGrpWnd::OnDestroy() 
{
	if (m_pToolTip)
		m_pToolTip->DestroyWindow();
	
	RemoveComponent();
	
	CWnd::OnDestroy();
}

void CGrpWnd::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TIMER_PID:
		KillTimer(nIDEvent);
		if (IsPtInGrp())
		{
			CPoint	pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);
			CString str = GetTipStr(pt);

			if (m_PosData == str)
				MouseMove(true, pt);
		}
		break;
	case TIMER_ID:
		if (!IsPtInGrp())
		{
			KillTimer(nIDEvent);
			m_pToolTip->ShowWindow(SW_HIDE);
			m_pCrossLine->HideCrossLine();
			m_timerID = 0;
		}
		break;
	case TIMER_TR:
		KillTimer(nIDEvent);
		RequestData(m_TRIndex);
		break;
	}

	CWnd::OnTimer(nIDEvent);
}

void CGrpWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CString	str = GetTipStr(point);
	if (m_PosData != str)
	{
		m_pToolTip->ShowWindow(SW_HIDE);
		m_PosData.Format("%s", str);
		KillTimer(TIMER_PID);
		SetTimer(TIMER_PID, TIMER_PGAP, nullptr);
	}

	MouseMove(false, point, true);
	
	CWnd::OnMouseMove(nFlags, point);
}

void CGrpWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_btnRect.PtInRect(point))
		Config(point);

	CWnd::OnLButtonDown(nFlags, point);
}

void CGrpWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	Config(point);

	CWnd::OnRButtonDown(nFlags, point);
}

void CGrpWnd::Config(CPoint point)
{
	if (m_timerID > 0)
		KillTimer(m_timerID);
	m_pToolTip->ShowWindow(SW_HIDE);
	m_pCrossLine->HideCrossLine(point);

	CRect rcBtn(m_btnRect);
	ClientToScreen(rcBtn);
	CJisuConfigDlg	dlg(&m_mj, rcBtn);
	if (dlg.DoModal() == IDCANCEL)
		return;

	((CMainWnd*)m_pMainWnd)->SaveCondition(&m_mj);

	m_TickView = 0;
	if (m_mj.tick & TK_LEFT)	m_TickView |= CDT_LEFT;
	if (m_mj.tick & TK_RIGHT)	m_TickView |= CDT_RIGHT;

	Resize();
	Invalidate();

	m_TRIndex = 0;
	RequestData(m_TRIndex);
}

// private member function
int CGrpWnd::DispatchData()
{
	RemoveComponent();

	struct _dataH* dataH{};
	char	token[32]{};
	char	pArgument[MAX_JISU][3][16]{};
	int	pCount[MAX_JISU]{};
	int	MaxIndex = 0;
	int	MaxCnt = 0;	
	for (int ii = 0; ii < MAX_JISU; ii++)
	{
		int dPos = 16;
		for (int jj = 0; jj < 3; jj++)
		{
			const int tLen = CGrpData::GetToken(&m_pRcv[ii][dPos], token);
			dPos += tLen;
			switch (jj)
			{
			case 0:
				sprintf(pArgument[ii][jj], "%s", token);
				break;
			case 1:	case 2:
				sprintf(pArgument[ii][jj], "%d", int(fabs(atof(token)*100)));
			}
		}

		dataH = (_dataH*)&m_pRcv[ii][dPos];
		const int count = CGrpData::CharToInt(dataH->count, sizeof(dataH->count));
		if (MaxCnt < count)
		{
			MaxCnt = count;
			MaxIndex = ii;
		}

		m_pIndex[ii] = dPos + sz_DATAH;
		pCount[ii] = count;
	}

	m_pGrpData = std::make_unique<CGrpData>(this);
	m_pGrpData->AttachGraphData(MaxIndex, true, MaxCnt, 0, &m_pRcv[MaxIndex][m_pIndex[MaxIndex]],
		"", "", "");
	for (int ii = 0; ii < MAX_JISU; ii++)
	{
		m_pGrpData->AttachGraphData(ii, false, MaxCnt, MaxCnt - pCount[ii], 
			&m_pRcv[ii][m_pIndex[ii]], pArgument[ii][0], 
			pArgument[ii][1], pArgument[ii][2]);
	}

	m_pCrossLine = std::make_unique<CCrossLine>(m_pView, this, m_pMainWnd, m_pRGB, RGB(180,77,77));

	AssignGraph();
	Resize();
	Invalidate();

	return 0;
}

void CGrpWnd::RealTimeData(struct _alertR* xxx)
{
	if (m_GrpCnt <= 0)	return;

	const CString code = xxx->code;
	DWORD* data = (DWORD*)xxx->ptr[0];

	for (int ii = 0; ii < MAX_JISU; ii++)
	{
		if (m_pGrpData->m_pCode[ii] != code)
			continue;

		const int searchSym = atoi(m_pGrpData->m_pSymbol[ii]);
		CString	rval = _T("");
		CString	ctim = _T("");
	

		if (data[atoi(R_CTIM)])
			ctim = (char*)data[atoi(R_CTIM)];

		if (data[searchSym])
			rval = (char*)data[searchSym];

		if (rval.IsEmpty() || ctim.IsEmpty())
			continue;

		m_pGrpData->UpdateRTM(ii, rval, ctim);
		m_pGrpInfo[ii]->IsChangeMinMax();
	}
	Invalidate();
}


bool CGrpWnd::AssignGraph()
{
	for (int ii = 0; ii < MAX_JISU; ii++)
	{
		m_pGrpInfo[m_GrpCnt] = std::make_unique<CGrpLine>(m_pView, m_pMainWnd, this, ii);
		m_GrpCnt++;
	}

	return true;
}

void CGrpWnd::RemoveComponent()
{
	m_pGrpData.reset();

	if (m_GrpCnt)
	{
		for (int ii = 0; ii < m_GrpCnt; ii++)
			m_pGrpInfo[ii].reset();
	}
	m_GrpCnt = 0;
}

void CGrpWnd::DrawGraph(CDC *pDC)
{
	DrawEmpty(pDC);
	if (m_GrpCnt == 0)
	{
		return;
	}

	m_tkPoint = CPoint(0, 0);
	m_Max = INT_MIN;
	m_Min = INT_MAX;
	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		if (!m_mj.jisu[ii].view)
			continue;

		m_pGrpInfo[ii]->CalculateMinMax();
		double	dMin, dMax;
		m_pGrpInfo[ii]->GetMinMaxValue(dMin, dMax);

		if (dMin == INT_MAX || dMax == INT_MIN)
			continue;

		if (m_Max < dMax)	m_Max = dMax;
		if (m_Min > dMin)	m_Min = dMin;
	}

	int	tindex = 0;
	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		if (m_mj.jisu[ii].view)
		{
			if (tindex == 0)
			{
				m_pGrpInfo[ii]->DrawTickBottom(pDC);
				if (m_TickView & CDT_LEFT)
					m_pGrpInfo[ii]->DrawTickHorizon(pDC, m_Max, m_Min, ctkLEFT);
				if (m_TickView & CDT_RIGHT)
					m_pGrpInfo[ii]->DrawTickHorizon(pDC, m_Max, m_Min, ctkRIGHT);
				m_pGrpInfo[ii]->DrawZeroLine(pDC, m_Max, m_Min);
			}
			m_pGrpInfo[ii]->DrawGraph(pDC);
			tindex++;
		}
	}

	if (m_pBitmap)
	{
		CDC		memDC;
		CBitmap* pSBitmap{};

		BITMAP stBitmapInfo;
		m_pBitmap->GetBitmap(&stBitmapInfo);

		memDC.CreateCompatibleDC(pDC);	
		if (memDC.m_hDC != nullptr) 
			pSBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);
		pDC->StretchBlt(m_btnRect.left, m_btnRect.top, m_btnRect.Width(), m_btnRect.Height(), &memDC, 0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, SRCCOPY);
		memDC.SelectObject(pSBitmap);
		memDC.DeleteDC();
	}
	else
	{
		pDC->FillSolidRect(m_btnRect, RGB(196,196,196));
		pDC->DrawText("설정", strlen("설정"), m_btnRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	if (m_pCrossLine)	m_pCrossLine->DrawCrossLine(pDC);

}

void CGrpWnd::DrawEmpty(CDC *pDC)
{
	CBrush	*pBrush = ((CMainWnd*)m_pMainWnd)->GetBrush(m_fRGB);
	CBrush	*tBrush = ((CMainWnd*)m_pMainWnd)->GetBrush(m_pRGB);
	CBrush	*fBrush = ((CMainWnd*)m_pMainWnd)->GetBrush(RGB(195,196,197));

	pDC->FillRect(&m_ObjRect, pBrush);
	pDC->FillRect(&m_GrpRect, tBrush);
	pDC->FrameRect(&m_GrpRect, fBrush);
}

void CGrpWnd::Resize()
{
	GetClientRect(&m_ObjRect);

	CDC *pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(m_pFont);
	const CSize	sz = pDC->GetOutputTextExtent("-99.99");

	m_tkInfo.tkRect[ctkBOTTOM] = m_ObjRect;
	m_tkInfo.tkRect[ctkBOTTOM].top = m_tkInfo.tkRect[ctkBOTTOM].bottom - sz.cy - 5;
	m_GrpRect = m_ObjRect;
	m_GrpRect.top += 3;
	m_GrpRect.bottom = m_tkInfo.tkRect[ctkBOTTOM].top;
	if ((m_TickView & CDT_LEFT) && (m_TickView & CDT_RIGHT))
	{
		m_GrpRect.left = m_ObjRect.left + sz.cx;
		m_GrpRect.right = m_ObjRect.right - sz.cx;
	}
	else if (m_TickView & CDT_LEFT)
	{
		m_GrpRect.left = m_ObjRect.left + sz.cx;
		m_GrpRect.right = m_ObjRect.right - 5;
	}
	else if (m_TickView & CDT_RIGHT)
	{
		m_GrpRect.left = m_ObjRect.left + 5;
		m_GrpRect.right = m_ObjRect.right - sz.cx;
	}
	else
	{
		m_GrpRect.left = m_ObjRect.left + 5;
		m_GrpRect.right = m_ObjRect.right - 5;
	}
	m_tkInfo.tkRect[ctkBOTTOM].left = m_GrpRect.left;
	m_tkInfo.tkRect[ctkBOTTOM].right = m_GrpRect.right;

	m_tkInfo.tkRect[ctkLEFT].SetRect(m_ObjRect.left, m_GrpRect.top, m_GrpRect.left, m_GrpRect.bottom);
	m_tkInfo.tkRect[ctkRIGHT].SetRect(m_GrpRect.right, m_GrpRect.top, m_ObjRect.right, m_GrpRect.bottom);

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		if (m_pGrpInfo[ii])
		{
			m_pGrpInfo[ii]->Resize();
		}
	}

	m_pCrossLine->Resize(m_GrpRect);

	m_btnRect.SetRect(m_GrpRect.right-20, m_GrpRect.top, m_GrpRect.right, m_GrpRect.top+20);
}

int CGrpWnd::GetPalette(int index)
{
	return m_pView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

long CGrpWnd::RequestData(int index)
{
	m_pRcv[index].reset();		

	char	inputStr[128]{};
	const int	inputLen = GetHeaderInfo(index, inputStr);

	if (inputLen == 0)
		return 0;

	char	gDataH[256];
	ZeroMemory(gDataH, 256);
	const int	dataHLen = GetDataInfo(index, gDataH);

	if (dataHLen == 0)
		return 0;

	const int	TRLen = inputLen + dataHLen;


	auto temp = std::make_unique<char[]>(TRLen);
	CopyMemory(&temp[0], inputStr, inputLen);
	CopyMemory(&temp[inputLen], gDataH, dataHLen);
	
	((CMainWnd*)m_pMainWnd)->SendTR("gooppoop", CString(temp.get(), TRLen), US_OOP, m_param.key, m_param.name);

	return 0;
}

int CGrpWnd::GetHeaderInfo(int index, char *Header)
{
	if (index >= MAX_JISU)
		return 0;

	CString	symbolS = _T("");
	CString	codeS = _T("");
	CString	AllcodeS = _T("");
	char	allCode[32]{};
	const int	len = strlen(m_mj.jisu[index].code);

	for (int ii = 0; ii < MAX_JISU; ii++)
	{
		if (strlen(m_mj.jisu[ii].code) == 4)
			sprintf(&allCode[ii*4], "%s", m_mj.jisu[ii].code);
		else
			sprintf(&allCode[ii*4], "%s", "    ");
	}

	switch (GetUnit(m_mj.jisu[index].code))
	{
	case CDU_FUTURE:
		codeS.Format("%s", MIF_CODE);
		AllcodeS.Format("%s", MIF_ALLCODE);
		symbolS.Format("%s\t%s\t%s\t", OF_CODE, OF_SIGA, OF_JJGA);	// rts code, 당일시가, 전일종가
		break;
	case CDU_UPJONG:
		codeS.Format("%s", II_CODE);
		AllcodeS.Format("%s", MII_ALLCODE);
		symbolS.Format("%s\t%s\t%s\t", OI_CODE, OI_SIGA, OI_JJGA);	// rts code, 당일시가, 전일종가
		break;
	/*
	case CDU_KOFEX:
		codeS.Format("%s", "51303");
		AllcodeS.Format("%s", MIQ_ALLCODE);
		symbolS.Format("%s\t%s\t%s\t", OQ_CODE, OQ_SIGA, OQ_JJGA);	// rts code, 당일시가, 전일종가

		sprintf(Header, "%s%c%s\t%s%c%s\t%s", codeS, 0x7F, "1", 
			AllcodeS, 0x7F, allCode, symbolS);

		return strlen(Header);
	*/
	default:
		return 0;
	}

	//sprintf(Header, "%s%c%s\t%s%c%s\t%s", codeS.GetString(), 0x7F, m_mj.jisu[index].code, 
	//	AllcodeS.GetString(), 0x7F, allCode, symbolS.GetString());

	if (m_strMarket == "KRX")
	{
		sprintf(Header, "%s%c%s\t%s%c%s\t%s%c%s\t%s", codeS.GetString(), 0x7F, m_mj.jisu[index].code,
			AllcodeS.GetString(), 0x7F, allCode, "1777", 0x7F, "1", symbolS.GetString());
	}
	else if(m_strMarket == "NXT")
	{
		sprintf(Header, "%s%c%s\t%s%c%s\t%s%c%s\t%s", codeS.GetString(), 0x7F, m_mj.jisu[index].code,
			AllcodeS.GetString(), 0x7F, allCode, "1777", 0x7F, "2", symbolS.GetString());
	}
	else if (m_strMarket == "통합")
	{
		sprintf(Header, "%s%c%s\t%s%c%s\t%s%c%s\t%s", codeS.GetString(), 0x7F, m_mj.jisu[index].code,
			AllcodeS.GetString(), 0x7F, allCode, "1777", 0x7F, "3", symbolS.GetString());
	}
	

	return strlen(Header);
}

int CGrpWnd::GetDataInfo(int index, char *Header)
{
/*	struct	_dataH {	// upload/download data header
		char		count[6];	// # of date(일자수)
		char		dummy[6];	// dummy data number
		unsigned char	dkind;		// data pattern kind	// not use
		unsigned char	dkey;		// data key
		char		pday[8];	// 기준일자, yyyymmdd
		char		dunit;		// data Index
		unsigned char	dindex;		// 일주월...
		char		lgap[4];	// n분, n틱
		char		ltic[4];	// last tick count
		unsigned char	option1;	// extra option
		unsigned char	option2;	// extra option
		char		rcode[16];	// code symbol
		
		char		ikey[1];	// key action
		// '0':Enter or Other
		// '1':PgUp	'2':PgDn
		// '3':ScrollUp	'4':ScrollDn
		// '5':First or End
		// '6':Please sort
		char		xpos;		// next gubn
		char		page[4];	// current page number
		char		save[80];	// grid inout save field
	};
*/

	CString	temp;
	struct _dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
		
	int GrpHLen = 0;
	temp.Format("%06d", 450);
	CopyMemory(Datah.count, temp, temp.GetLength());
	Datah.dkey = index + 0x30;

	const CTime	time = CTime::GetCurrentTime();
	temp.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());
	CopyMemory(Datah.pday, temp, temp.GetLength());

	Datah.dindex = GI_MINUTE;
	Datah.dunit = GetUnit(m_mj.jisu[index].code);
	temp.Format("%0*d", sizeof(Datah.lgap), 1);
	CopyMemory(Datah.lgap, temp, temp.GetLength() );

	CString	dataSymbol, outSymbol;
	MakeDataSymbol(Datah.dunit, dataSymbol, outSymbol);

	GrpHLen = dataSymbol.GetLength() + outSymbol.GetLength() + sz_DATAH;

	CopyMemory(Header, dataSymbol, dataSymbol.GetLength());
	CopyMemory(&Header[dataSymbol.GetLength()], (char *)&Datah, sz_DATAH);
	CopyMemory(&Header[dataSymbol.GetLength() + sz_DATAH], outSymbol, outSymbol.GetLength());

	return GrpHLen;
}

int CGrpWnd::GetUnit(char *code)
{
	const int	len = strlen(code);
	int	unit = CDU_FUTURE;

	if (len >= 3)	
		unit = CDU_UPJONG;
	//else if (code[0] == '0')
	//	unit = CDU_KOFEX;

	return unit;
}

void CGrpWnd::MakeDataSymbol(int unit, CString &dataSymbol, CString &outSymbol)
{
	switch (unit)
	{	
	case CDU_UPJONG:
		dataSymbol.Format("?%s%c", MII_SYM, 0x7F);
		outSymbol.Format("%s\n%s\n\t", MDI_TIME, MDI_JGGA);
		break;
	case CDU_FUTURE:
		dataSymbol.Format("?%s%c", MIF_SYM, 0x7F);
		outSymbol.Format("%s\n%s\n\t", MDF_TIME, MDF_JGGA);
		break;
	/*
	case CDU_KOFEX:
		dataSymbol.Format("?%s%c", MIQ_SYM, 0x7F);
		outSymbol.Format("%s\n%s\n\t", MDQ_TIME, MDQ_JGGA);
		break;
	*/
	}
}

BOOL CGrpWnd::IsPtInGrp()
{
	POINT	pt;
	CRect	rect;

	::GetCursorPos(&pt);
	GetWindowRect(rect);

	CWnd* pWnd = CWnd::WindowFromPoint(pt);
	if (pWnd != this)
		return FALSE;

	return (rect.PtInRect(pt));
}

void CGrpWnd::TipMouseMove(CPoint point)
{
	ScreenToClient(&point);

	MouseMove(true, point);
}

void CGrpWnd::MouseMove(bool bIsTipEvent, CPoint point, bool bCrossOnly)
{
	if (!m_pToolTip || !m_pCrossLine)
		return;

	if (!m_GrpRect.PtInRect(point))
	{
		m_pToolTip->ShowWindow(SW_HIDE);
		m_pCrossLine->HideCrossLine(point);
		return;
	}

	if (m_GrpCnt < 1)
	{
		m_pCrossLine->HideCrossLine(point);
		m_pToolTip->ShowWindow(SW_HIDE);
		return;
	}

	m_pCrossLine->ShowCrossLine(point);

	if (bCrossOnly)
	{
		if (m_timerID > 0)
			KillTimer(m_timerID);

		m_timerID = TIMER_ID;
		SetTimer(m_timerID, TIMER_GAP, nullptr);
		return;
	}

	if (m_PosData.IsEmpty())
	{
		m_pToolTip->ShowWindow(SW_HIDE);
		return;
	}

	if (!bIsTipEvent)
	{
		if (m_pToolTip->IsWindowVisible())
			return;
	}

	CPoint	pt = point;
	ClientToScreen(&pt);
	pt.x += 10;
	pt.y += 10;

	CRect	rect;
	CSize	size;	

	rect = m_pToolTip->SetData(m_PosData);

	size.cx = rect.Width();
	size.cy = rect.Height();

	rect.left = pt.x;
	rect.right = pt.x + size.cx;
	rect.top = pt.y;
	rect.bottom = rect.top + size.cy;

	CRect rcWnd;
	GetWindowRect(rcWnd);

	m_pToolTip->SetWindowPos(&wndTop, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOACTIVATE);

	if (m_pToolTip->IsWindowVisible())
		m_pToolTip->Invalidate(FALSE);
	else
		m_pToolTip->ShowWindow(SW_SHOWNOACTIVATE);

	if (m_timerID > 0)
		KillTimer(m_timerID);

	m_timerID = TIMER_ID;
	SetTimer(m_timerID, TIMER_GAP, nullptr);
}

CString CGrpWnd::GetTipStr(CPoint point)
{
	if (m_GrpCnt <= 0)
		return _T("");

	CString stmp;
	CString dispStr = m_pGrpInfo[0]->GetDisplayPosHead(point);
	for (int ii = 0; ii < m_GrpCnt; ii++)
	{
		if (!m_mj.jisu[ii].view)	continue;

		stmp = m_pGrpInfo[ii]->GetDisplayPosData(point);
		if (!stmp.IsEmpty())
			dispStr = dispStr + "\n" + stmp;
	}

	return dispStr;
}

