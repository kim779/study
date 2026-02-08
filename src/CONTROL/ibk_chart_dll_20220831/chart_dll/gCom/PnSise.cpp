// PnSise.cpp : implementation file
//

#include "stdafx.h"
#include "AxisGCom.h"
#include "PnSise.h"

#include "../MxTrace.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPnSise
const COLORREF	clrRED	= PALETTERGB(255, 0, 0);
const COLORREF	clrBLUE	= PALETTERGB(0, 0, 255);
const COLORREF	clrBLACK= PALETTERGB(0, 0, 0);
const COLORREF	clrWHITE= PALETTERGB(255, 255, 255);

#define	timeID	1000

CPnSise::CPnSise(CWnd *pView, CWnd* pParent, CFont* pFont, int ctrlID, char* info)
: CPnBaseWnd(pView, pParent, pFont, ctrlID, info)
{
	changeUnit(((_envInfo*)info)->datainfo.btUnit);

	m_arDisp.RemoveAll();

	SetPaletteColor();

	m_stock = _T("");

	m_FlashIndex = 8;

	m_pNMFont = NULL;
	m_pBDFont = NULL;
	m_fPoint = 9;
	m_fName.Format("굴림");

	//** macho add
	m_time.Empty();
}

CPnSise::~CPnSise()
{
	deleteComs();
}


BEGIN_MESSAGE_MAP(CPnSise, CPnBaseWnd)
	//{{AFX_MSG_MAP(CPnSise)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(GEV_SISE, OnSiseEvent)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPnSise message handlers

LRESULT CPnSise::OnSiseEvent(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case siseChgPalette:
		{
			SetPaletteColor();
			Invalidate();
		}
		break;
	case SISE_TR_DATA:
		{
			// debug : 20090915
			//CString strDebug;
			//strDebug.Format("CPnSise::OnSiseEvent() SISE_TR_DATA start");
			//AfxMessageBox(strDebug);

			struct _trData* pTD = (struct _trData*)lParam;
			if (pTD->iLen[0] == 0)
			{
				ClearSise();
				return 0;
			}
			pTD->iSiseLen[0] =  ReceiveData(pTD->pcData[0], pTD->iLen[0]);
		}
		break;
	case siseRTMData:
		realtime((char*)lParam);
		break;
	case siseUnit:
		changeUnit(lParam);
		break;
	case siseData:
		switch (HIWORD(wParam))
		{
		case siseCurr:
			wsprintf((char*)lParam, _T("%s\t%s\t%s\t%s\t%s"),
				LPCTSTR(m_curr), LPCTSTR(m_udyl), LPCTSTR(m_jjga), LPCTSTR(m_sahg), LPCTSTR(m_hahg));
			break;
		case siseCode:
			return (long)m_code.operator LPCTSTR();
		case siseName:
			return (long)m_name.operator LPCTSTR();
		case siseDate:
			wsprintf((char*)lParam, _T("%s%s"), LPCTSTR(m_date), LPCTSTR(m_start));
			break;
		case sisePivot:
			wsprintf((char*)lParam, _T("%s\t%s\t%s"), LPCTSTR(m_sahg), LPCTSTR(m_hahg), LPCTSTR(m_jjga));
			break;
		case siseStock:
			return (long)m_stock.operator LPCTSTR();
		case siseClear:
			ClearSise();
			break;
		}
		break;
	case siseFlash:
		m_bFlash = (HIWORD(wParam) == 1)? true:false;
		m_FlashIndex = lParam;

		if (m_bFlash)	m_clrFlash = m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), m_FlashIndex);
		else		m_clrFlash = clrWHITE;

		Invalidate();
		break;
	case siseFont:
		{
			m_fPoint = HIWORD(wParam);
			char*	fname = (char*)lParam;

			if (fname && strlen(fname))
				m_fName.Format("%s", fname);

			m_pNMFont = m_pApp->GetFont(m_pwndView, m_fPoint, m_fName);
			m_pBDFont = m_pApp->GetFont(m_pwndView, m_fPoint, m_fName, 1);
			Invalidate();
		}
		break;
	case siseChgEnv:
		m_clrBack = lParam;
		Invalidate();
		break;
	}

	return TRUE;
}

int CPnSise::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pBDFont = m_pApp->GetFont(m_pwndView, m_fPoint, m_fName, FW_BOLD);

	initComs();

	return 0;
}

void CPnSise::OnPaint() 
{
	CPaintDC dc(this);

	CRect	rc;
	GetClientRect(&rc);
	if (rc.Height() < 5)
		return;

	if (rc.Height() > 30) 
	{
		DrawSise(dc);
		return;
	}

	CFont	*pFont = m_pFont;
	if (m_pNMFont)	pFont = m_pNMFont;

	CBrush	*bgBrush = m_pApp->GetBrush(m_pwndView, m_clrWindow);
	CBrush	*bgBrush2 = m_pApp->GetBrush(m_pwndView, m_clrHeader);
	CBrush	*sBrush = dc.SelectObject(bgBrush);
	
	dc.FillRect(&rc, bgBrush);

	CPen	*wPen = m_pApp->GetPen(m_pwndView, PS_SOLID, 1, clrWHITE); 
	CPen	*fPen = m_pApp->GetPen(m_pwndView, PS_SOLID, 1, m_clrBorder);
	CPen	*bPen = m_pApp->GetPen(m_pwndView, PS_SOLID, 1, m_clrHeader);
	CPen	*sPen = dc.SelectObject(fPen);

	COLORREF sColor = dc.SetTextColor(m_clrWindow);
	COLORREF sbkColor = dc.SetBkColor(m_clrWindow);
	int	sMode = dc.SetBkMode(TRANSPARENT);
	CFont	*sFont = dc.SelectObject(pFont);

	CPoint		pPt[5];
	_comdisp	*com;
	COLORREF	color;
	CBrush		*cBrush;
	int		end = valMYGL;

	if (m_unit == GU_CODE || m_unit == GU_INDEX)	end = valJEGA;
	for (int ii = valCODE; ii <= end; ii++)
	{
		com = m_arDisp.GetAt(ii);
		if (com == NULL)	continue;
		
		switch (com->dispID)
		{
		case nameVOL:
		case nameSIGA:
		case nameKOGA:
		case nameJEGA:
		case nameMYGL:
			dc.SetTextColor(com->fgcolor);

			rc = com->drawRC;
			pPt[0] = com->drawRC.TopLeft();
			pPt[0].x += 2;	pPt[1].x += 2;
			pPt[2] = com->drawRC.BottomRight();
			pPt[2].x -= 2;
			pPt[1] = CPoint(pPt[0].x, pPt[2].y);
			pPt[3] = CPoint(pPt[2].x, pPt[0].y);
			dc.SelectObject(bPen);
			dc.SelectObject(bgBrush2);
			dc.Polygon(pPt, 4);
			
			rc.OffsetRect(0, 2);
			dc.DrawText(com->dispStr, &rc, com->fstyle);
			break;
		case valCODE:
			dc.SelectObject(m_pBDFont);

			if (com->bFlash)	color = m_clrFlash;
			else			color = m_clrWindow;
			cBrush = m_pApp->GetBrush(m_pwndView, color);
			dc.SetTextColor(com->fgcolor);

			rc = com->drawRC;
			dc.FillRect(&rc, cBrush);

			rc.OffsetRect(0, 2);
			dc.DrawText(com->dispStr, &rc, com->fstyle);
			pPt[0] = pPt[4] = com->drawRC.TopLeft();
			pPt[2] = com->drawRC.BottomRight();
			pPt[1] = CPoint(pPt[0].x, pPt[2].y);
			pPt[3] = CPoint(pPt[2].x, pPt[0].y);

			dc.SelectObject(fPen);
			dc.Polyline(pPt, 5);

			dc.SelectObject(pFont);
			break;
		default:
			if (com->bFlash)	color = m_clrFlash;
			else			color = m_clrWindow;
			cBrush = m_pApp->GetBrush(m_pwndView, color);
			dc.SetTextColor(com->fgcolor);

			rc = com->drawRC;
			dc.FillRect(&rc, cBrush);

			rc.OffsetRect(0, 2);
			dc.DrawText(com->dispStr, &rc, com->fstyle);
			pPt[0] = pPt[4] = com->drawRC.TopLeft();
			pPt[2] = com->drawRC.BottomRight();
			pPt[1] = CPoint(pPt[0].x, pPt[2].y);
			pPt[3] = CPoint(pPt[2].x, pPt[0].y);

			dc.SelectObject(fPen);
			dc.Polyline(pPt, 5);
			break;
		}
	}

	dc.SetTextColor(sColor);
	dc.SetBkMode(sMode);
	dc.SelectObject(sFont);
	dc.SelectObject(sPen);
	dc.SelectObject(sBrush);
}

void CPnSise::changeUnit(int unit)
{
	m_unit = unit;

	// 20090918 : CHANGE : 상품 선물 시세 데이터 처리 추가
	//if (m_unit == GU_CODE || m_unit == GU_COFUT || m_unit == GU_COOPT)
	if (m_unit == GU_CODE || m_unit == GU_COFUT || m_unit == GU_COOPT || m_unit == GU_FFUTURE)
		m_bCode = true;
	else
		m_bCode = false;

	if (m_arDisp.GetSize() <= 0)
		return;

	_comdisp *com = m_arDisp.GetAt(nameVOL);
	switch (m_unit)
	{
	case GU_CODE:	case GU_COFUT:	case GU_COOPT:
		com->dispStr.Format("거래량");
		break;
	case GU_INDEX:
		com->dispStr.Format("거래량(천)");
		break;
	default:
		com->dispStr.Format("거래량");
		break;
	}

	com = m_arDisp.GetAt(nameMYGL);
	switch (m_unit)
	{
	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
	case GU_CFUTURE: // 20090917 : ADD
	case GU_FFUTURE: // 20090917 : ADD
		com->dispStr.Format("미결제");
		break;
	case GU_FOREIGN:
		com->dispStr.Format("수신(국내)");
		break;
	}
}

void CPnSise::ClearSise()
{
	_comdisp *com;
	for (int ii = 0; ii < m_arDisp.GetSize(); ii++)
	{
		com = m_arDisp.GetAt(ii);
		if (com == NULL)
			continue;

		switch (com->dispID)
		{
		case valCODE:
		case valCURR:	
		case valDIFF:
		case valUDYL:
		case valVOL:
		case valSIGA:
		case valKOGA:
		case valJEGA:
		case valMYGL:
		case valMDHG:
		case valMSHG:
			com->dispStr = _T("");
			break;
		}
	}

	m_time = m_name = _T("");

	Invalidate();
}

// 시간,이름,현재가,대비,등락율,거래량,매도호가,매수호가,시가,고가,저가,
// 코드,최근영업일자,상한가,하한가,전일종가,주식수
int CPnSise::ReceiveData(char* pcData, int iLen)
{
	int	index = 0;
	char	token[64];
	CString	tmpS;
	_comdisp* com = NULL;
	m_name = m_time = _T("");

	for (int ii = 0; ii < 11; ii++)
	{
		int tLen = GetToken(&pcData[index], token);
		index += tLen;

		switch (ii)
		{
		case 0:	// 시간
			m_time = &token[tLen - 7];
			if (m_unit == GU_FOREIGN && strlen(token) >= 0)
			{
				com = m_arDisp.GetAt(valMYGL);
				com->dispStr.Format("%.2s:%.2s:%.2s", token, &token[2], &token[4]);
			}
			break;
		case 1:	// 종목명
			com = m_arDisp.GetAt(valCODE);
			com->dispStr.Format("%s", token);
			com->dispStr.TrimRight(" ");
			m_name.Format("%s", token);
			m_name.TrimRight(" ");
			break;
		case 2:	// 현재가
			com = m_arDisp.GetAt(valCURR);
			
			if (token[0] == '+' || token[0] == '-')
			{
				com->fgcolor = parseSymbol(token[0]);
				tmpS = editComma(atof(&token[1]), valCURR);
			}
			else
			{
				com->fgcolor = RGB(0,0,0);
				tmpS = editComma(atof(token), com->dispID);
			}
			com->dispStr.Format("%s", tmpS);
			m_curr.Format("%s", token);
			break;
		case 3:	// 대비
			com = m_arDisp.GetAt(valDIFF);
			{
				CString symbol, sign;
				com->fgcolor = parseSymbol(token[0], symbol, sign);
				tmpS = editComma(atof(&token[1]), com->dispID);
				com->dispStr.Format("%s %s", symbol, tmpS);

				m_diffSym.Format("%s", symbol);
				m_diffVal.Format("%s", tmpS);
			}
			break;
		case 4:	// 등락
			com = m_arDisp.GetAt(valUDYL);
			com->fgcolor = parseSymbol(token[0]);
			com->dispStr.Format("%s", token);
			com->dispStr.Remove('+');
			m_udyl.Format("%s", token);
			m_udyl.Remove('+'); 
			break;
		case 5:	// 거래량
			com = m_arDisp.GetAt(valVOL);
			tmpS = editComma(atof(token), com->dispID);
			com->dispStr.Format("%s", tmpS);
			m_volume = tmpS;
			break;
		case 6:	// 매도호가
			com = m_arDisp.GetAt(valMDHG);
			if (token[0] == '+' || token[0] == '-')
			{
				com->fgcolor = parseSymbol(token[0]);
				tmpS = editComma(atof(&token[1]), valCURR);
			}
			else
			{
				com->fgcolor = RGB(0,0,0);
				tmpS = editComma(atof(token), com->dispID);
			}

			com->dispStr.Format("%s", tmpS);
			if (atof(com->dispStr) == 0)
				com->dispStr = "-";
			
			break;
		case 7:	// 매수호가
			com = m_arDisp.GetAt(valMSHG);
			if (token[0] == '+' || token[0] == '-')
			{
				com->fgcolor = parseSymbol(token[0]);
				tmpS = editComma(atof(&token[1]), valCURR);
			}
			else
			{
				com->fgcolor = RGB(0,0,0);
				tmpS = editComma(atof(token), com->dispID);
			}

			com->dispStr.Format("%s", tmpS);
			if (atof(com->dispStr) == 0)
				com->dispStr = "-";

			break;
		case 8:	// 시가
			com = m_arDisp.GetAt(valSIGA);
			if (token[0] == '+' || token[0] == '-')
			{
				com->fgcolor = parseSymbol(token[0]);
				tmpS = editComma(atof(&token[1]), com->dispID);
			}
			else
			{
				com->fgcolor = RGB(0,0,0);
				tmpS = editComma(atof(token), com->dispID);
			}
			com->dispStr.Format("%s", tmpS);
			break;
		case 9:	// 고가
			com = m_arDisp.GetAt(valKOGA);
			if (token[0] == '+' || token[0] == '-')
			{
				com->fgcolor = parseSymbol(token[0]);
				tmpS = editComma(atof(&token[1]), com->dispID);
			}
			else
			{
				com->fgcolor = RGB(0,0,0);
				tmpS = editComma(atof(token), com->dispID);
			}
			com->dispStr.Format("%s", tmpS);
			break;
		case 10: // 저가
			com = m_arDisp.GetAt(valJEGA);
			if (token[0] == '+' || token[0] == '-')
			{
				com->fgcolor = parseSymbol(token[0]);
				tmpS = editComma(atof(&token[1]), com->dispID);
			}
			else
			{
				com->fgcolor = RGB(0,0,0);
				tmpS = editComma(atof(token), com->dispID);
			}
			com->dispStr.Format("%s", tmpS);
			break;
		case 11: // 미결제
			com = m_arDisp.GetAt(valMYGL);
			if (token[0] == '+' || token[0] == '-')
			{
				com->fgcolor = parseSymbol(token[0]);
				tmpS = editComma(atof(&token[1]), com->dispID);
			}
			else
			{
				com->fgcolor = RGB(0,0,0);
				tmpS = editComma(atof(token), com->dispID);
			}
			com->dispStr.Format("%s", tmpS);
			break;
		}
	}

	Invalidate();

	for (int ii = 0; ii < 7; ii++)
	{
		int tLen = GetToken(&pcData[index], token);
		index += tLen;

		switch (ii)
		{
		case 0:	m_code.Format("%s", token);	break;
		case 1:	m_date.Format("%s", token);	break;
		case 2:	m_sahg.Format("%s", token);	break;
		case 3:	m_hahg.Format("%s", token);	break;
		case 4:	m_jjga.Format("%s", token);	break;
		case 5:	m_stock.Format("%s", token);	break;
		case 6:	m_start.Format("%s", token);	break;
		}
	}

	if (m_unit == GU_CODE)
	{
		int tLen = GetToken(&pcData[index], token);
		index += tLen;

		com = m_arDisp.GetAt(valCODE);
		com->dispStr += CString(token, strlen(token));
	}

	m_pwndGView->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, setCodeInfo), 0);

	if (m_name.IsEmpty())
		return 0;

	return index;
}

// 시간,현재가,대비,등락율,거래량,매도호가,매수호가,시가,고가,저가
void CPnSise::realtime(char *pData)
{
	int	index = 0;
	char	token[64];
	CString	tmpS, tmpS2;
	_comdisp* com = NULL;

	bool	bCurrChange = false;
	int	pID[10] = { 0, valCURR, valDIFF, valUDYL, valVOL, valMDHG, valMSHG, valSIGA, valKOGA, valJEGA};
	for (int ii = 0; ii < 10; ii++)
	{
		com = m_arDisp.GetAt(pID[ii]);
		int tLen = GetToken(&pData[index], token);
		index += tLen;

		if (token[0] == 0x00)	
			continue;

		if (ii == 0)
		{
			m_time = token;
			continue;
		}

		switch (com->dispID)
		{
		case valCURR:
			{
				if (token[0] == '+' || token[0] == '-')
				{
					com->fgcolor = parseSymbol(token[0]);
					tmpS = editComma(atof(&token[1]), valCURR);
				}
				else
				{
					com->fgcolor = RGB(0,0,0);
					tmpS = editComma(atof(token), valCURR);
				}
			}
			
			if( tmpS != com->dispStr)
			{
				com->bFlash = true;
				InvalidateRect(&com->drawRC);
				KillTimer(com->timer);
				SetTimer(com->timer, 1000, NULL);

				bCurrChange = true;
				com->dispStr.Format("%s", tmpS);
				m_curr.Format("%s", token);
			}
			break;
		case valDIFF:
			{
				CString symbol, sign;
				com->fgcolor = parseSymbol(token[0], symbol, sign);
				tmpS = editComma(atof(&token[1]), com->dispID);
				tmpS2.Format("%s %s", symbol, tmpS);

				if( tmpS2 != com->dispStr)
				{
					com->bFlash = true;
					InvalidateRect(&com->drawRC);
					KillTimer(com->timer);
					SetTimer(com->timer, 1000, NULL);
				}
				com->dispStr.Format("%s", tmpS2);
			}
			break;
		case valUDYL:
			com->fgcolor = parseSymbol(token[0]);
			tmpS.Format("%s", token);

			if( tmpS != com->dispStr)
			{
				com->bFlash = true;
				InvalidateRect(&com->drawRC);
				KillTimer(com->timer);
				SetTimer(com->timer, 1000, NULL);
			}
			com->dispStr.Format("%s", tmpS);
			com->dispStr.Remove('+');
			m_udyl.Format("%s", token);
			m_udyl.Remove('+');
			break;
		case valVOL:
			tmpS = editComma(atof(token), com->dispID);

			if( tmpS != com->dispStr)
			{
				com->bFlash = true;
				InvalidateRect(&com->drawRC);
				KillTimer(com->timer);
				SetTimer(com->timer, 1000, NULL);
			}
			com->dispStr.Format("%s", tmpS);
			m_volume = tmpS;
			break;
		case valMDHG:
		case valMSHG:
		case valSIGA:
		case valKOGA:
		case valJEGA:
			{
				if (token[0] == '+' || token[0] == '-')
				{
					com->fgcolor = parseSymbol(token[0]);
					tmpS = editComma(atof(&token[1]), com->dispID);
				}
				else
				{
					com->fgcolor = RGB(0,0,0);
					tmpS = editComma(atof(token), com->dispID);
				}
			}

			if( tmpS != com->dispStr)
			{
				com->bFlash = true;
				InvalidateRect(&com->drawRC);
				KillTimer(com->timer);
				SetTimer(com->timer, 1000, NULL);
			}
			com->dispStr.Format("%s", tmpS);
			break;
		default:
			break;
		}
	}

	if (bCurrChange)
	{
		_comdisp* curcom = m_arDisp.GetAt(valCURR);	// curr
		CString	sTmp, sCurr = curcom->dispStr;
		sCurr.Remove(',');
		switch (m_unit)
		{
		case GU_INDEX:
			com = m_arDisp.GetAt(valSIGA);	// siga
			if (atof(com->dispStr) == 0.0)
			{
				com->dispStr.Format("%s", curcom->dispStr);
				com->fgcolor = curcom->fgcolor;
				com->bFlash = true;
				InvalidateRect(&com->drawRC);
				KillTimer(com->timer);
				SetTimer(com->timer, 1000, NULL);
			}

			com = m_arDisp.GetAt(valKOGA);	// koga
			sTmp = com->dispStr;	sTmp.Remove(',');
			if (atof(sTmp) == 0.0 || 
				atof(sTmp) < atof(sCurr))
			{
				com->dispStr.Format("%s", curcom->dispStr);
				com->fgcolor = curcom->fgcolor;
				com->bFlash = true;
				InvalidateRect(&com->drawRC);
				KillTimer(com->timer);
				SetTimer(com->timer, 1000, NULL);
			}
			
			com = m_arDisp.GetAt(valJEGA);	// jega
			sTmp = com->dispStr;	sTmp.Remove(',');
			if (atof(sTmp) == 0.0 ||
				atof(sTmp) > atof(sCurr))
			{
				com->dispStr.Format("%s", curcom->dispStr);
				com->fgcolor = curcom->fgcolor;
				com->bFlash = true;
				InvalidateRect(&com->drawRC);
				KillTimer(com->timer);
				SetTimer(com->timer, 1000, NULL);
			}
			break;
		}
	}

	Invalidate();
}

int CPnSise::GetToken(char *pData, char token[])
{
	int	idx = 0;
	ZeroMemory(token, sizeof(token));
	for(;;)
	{
		switch (pData[idx])
		{
		case '\t':
			token[idx++] = 0x00;
			return idx;
		case '\n':
			token[idx++] = 0x00;
			return idx;
		default:
			token[idx] = pData[idx];
			idx++;
			break;
		}
	}

	return 0;
}

void CPnSise::initComs()
{
	_comdisp *com = NULL;

	CFont	*pFont = m_pFont;
	if (m_pNMFont)	pFont = m_pNMFont;

	CFont	*cFont = m_pApp->GetFont(m_pwndView, m_fPoint, m_fName);

	CDC *pDC = GetDC();
	CFont *sFont = pDC->SelectObject(cFont);

	CSize szName1 = pDC->GetOutputTextExtent(" 가 ");
	CSize szName4 = pDC->GetOutputTextExtent("거래량(천)");
	CSize szPrc = pDC->GetOutputTextExtent(" 9,999,999 ");
	CSize szDiff = pDC->GetOutputTextExtent(" ▲999,999 "); // 대비
	CSize szUdyl = pDC->GetOutputTextExtent(" +99.99% "); // 등락율
	CSize szVol = pDC->GetOutputTextExtent(" 999,999,999 ");

	pDC->SelectObject(m_pBDFont);
	CSize szName = pDC->GetOutputTextExtent("가나다라마바사아자차카타");
	
	pDC->SelectObject(sFont);
	ReleaseDC(pDC);

	int	x = 5;
	int	y = 2;
	int	end = HEIGHT_SISE - y;
	for (int ii = valCODE; ii <= valMYGL; ii++)
	{
		com = new _comdisp();

		com->dispID = ii;
		com->dispStr = _T("");
		com->fstyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
		com->drawRC = CRect(0, 0, 0, 0);
		com->fgcolor = m_clrText;
		com->bFlash = false;
		com->bShow = true;
		com->timer = timeID + ii;
		
		switch (ii)
		{
		case valCODE:
			com->drawRC = CRect(x, y, x+szName.cx, end);
			x += szName.cx;
			break;
		case valCURR:	
			com->drawRC = CRect(x, y, x+szPrc.cx, end);
			x += szPrc.cx;
			break;
		case valDIFF:
			com->drawRC = CRect(x, y, x+szDiff.cx, end);
			x += szDiff.cx;
			break;
		case valUDYL:
			com->drawRC = CRect(x, y, x+szUdyl.cx, end);
			x += szUdyl.cx;
			break;
		case nameVOL:
			if (m_unit == GU_INDEX)
				com->dispStr = _T("거래량(천)");
			else
				com->dispStr = _T("거래량");

			x += 10;	//x += 15;
			com->drawRC = CRect(x, y, x+szName4.cx, end);
			x += szName4.cx;
			break;
		case valVOL:
			com->drawRC = CRect(x, y, x+szVol.cx, end);
			x += szVol.cx;
			break;

			//** macho add start
		case nameMDHG:
			com->dispStr = "매도호가";
			x += szPrc.cx;
			break;
		case valMDHG:
			x += szPrc.cx;
			break;
		case nameMSHG:
			com->dispStr = "매수호가";
			x += szPrc.cx;
			break;
		case valMSHG:
			x += szPrc.cx;
			break;
			//** macho add end
		case nameSIGA:
			com->dispStr = _T("시");
			com->drawRC = CRect(x, y, x+szName1.cx, end);
			x += szName1.cx;
			break;
		case valSIGA:
			com->drawRC = CRect(x, y, x+szPrc.cx, end);
			x += szPrc.cx;
			break;
		case nameKOGA:
			com->dispStr = _T("고");
			com->drawRC = CRect(x, y, x+szName1.cx, end);
			x += szName1.cx;
			break;
		case valKOGA:
			com->drawRC = CRect(x, y, x+szPrc.cx, end);
			x += szPrc.cx;
			break;
		case nameJEGA:
			com->dispStr = _T("저");
			com->drawRC = CRect(x, y, x+szName1.cx, end);
			x += szName1.cx;
			break;
		case valJEGA:
			com->drawRC = CRect(x, y, x+szPrc.cx, end);
			x += szPrc.cx;
			break;
		case nameMYGL:
			if (m_unit == GU_FOREIGN)
				com->dispStr = _T("수신(국내)");
			else
				com->dispStr = _T("미결제");
			com->drawRC = CRect(x, y, x+szName4.cx, end);
			x += szName4.cx;
			break;
		case valMYGL:
			com->drawRC = CRect(x, y, x+szVol.cx, end);
			x += szVol.cx;
			break;
		default:
			LIB_DELETE(com);
			continue;
		}

		m_arDisp.Add(com);
	}
}

void CPnSise::deleteComs()
{
	_comdisp* com = NULL;
	for (int ii = 0; ii < m_arDisp.GetSize(); ii++)
	{
		com = m_arDisp.GetAt(ii);
		LIB_DELETE(com);
	}
	m_arDisp.RemoveAll();
}

COLORREF CPnSise::parseSymbol(char sign, CString& symbolS, CString& signS)
{
	COLORREF color;
	switch (sign)
	{
	case 1:
	case '1':
		color = clrRED;
		symbolS = "↑";
		signS = "+";
		break;
	case 2:
	case '2':
	case '+':
		color = clrRED;
		symbolS = "▲";
		signS = "+";
		break;
	case 4:
	case '4':
		color = clrBLUE;
		symbolS = "↓";
		signS = "-";
		break;
	case 5:
	case '5':
	case '-':
		color = clrBLUE;
		symbolS = "▼";
		signS = "-";
		break;
	default:
		color = clrBLACK;
		symbolS = " ";
		signS = " ";
		break;
	}

	return color;
}

COLORREF CPnSise::parseSymbol(char sign)
{
	COLORREF color;
	switch (sign)
	{
	case 1:
	case '1':
		color = clrRED;
		break;
	case 2:
	case '2':
	case '+':
		color = clrRED;
		break;
	case 4:
	case '4':
		color = clrBLUE;
		break;
	case 5:
	case '5':
	case '-':
		color = clrBLUE;
		break;
	default:
		color = clrBLACK;
		break;
	}

	return color;
}

CString CPnSise::editComma(double dVal, int id)
{
	CString tmpS = _T("");
	if (dVal == 0.0)
	{
		if (m_bCode)
			tmpS = _T("0");
		else
			tmpS = _T("0.00");
		return tmpS;
	}

	CString dotRight;
	

	bool bMinus = (dVal < 0) ? true : false;
	double val = (dVal > 0) ? dVal:-dVal;
	
	CString tmp;
	
	if (m_bCode)
		tmp.Format("%.0f", val);
	else
		tmp.Format("%.2f", val);

	int find = tmp.Find('.');
	if (find > 0)  {
		dotRight = tmp.Right(tmp.GetLength() - (find));
		tmp = tmp.Left(find);
	}
	else {
		dotRight.Empty();
	}

	int idx = 0, jj = 1;
	for (int ii = tmp.GetLength() - 1; ii >= 0 ;ii--, jj++)
	{
		tmpS.Insert(0, tmp.GetAt(ii));
		if (jj % 3 == 0 && ii > 0)
			tmpS.Insert(0, ",");
	}
	if (bMinus)
		tmpS = "-" + tmpS;

	tmpS += dotRight;
	return tmpS;
}

void CPnSise::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPnBaseWnd::OnLButtonDown(nFlags, point);
}

void CPnSise::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent < timeID)
	{
		CPnBaseWnd::OnTimer(nIDEvent);
		return;
	}

	KillTimer(nIDEvent);

	return;

	int	idx = nIDEvent - timeID;
	if (idx >= m_arDisp.GetSize())
	{
		CPnBaseWnd::OnTimer(nIDEvent);
		return;
	}

	_comdisp* com = m_arDisp.GetAt(idx);
	com->bFlash = false;
	InvalidateRect(&com->drawRC);

	CPnBaseWnd::OnTimer(nIDEvent);
}

void CPnSise::OnDestroy() 
{
	CPnBaseWnd::OnDestroy();
}


//** macho add


// function DrawSise is Drawing Stock Information routine for 2 line Box.
enum { cpTime = 0, cpName, cpCurr, cpDiff, cpUpDown, cpVolume, cpSellHoga, cpBuyHoga, cpOpen, cpHigh, cpLow };
const int widths[] = 
{
	33, 144, 62, 62, 45, 70, 62, 62, 62, 62, 62
};
const char* heads[] = 
{
	"시간", "종목명", "현재가", "대비", "등락율", "거래량", "매도호가", "매수호가", "시가", "고가", "저가" 
};

int	 CPnSise::GetTotalWidth()
{
	int sum = 0;
	for (int i = cpTime; i <= cpLow; i++) {
		sum += widths[i];
	}

	return sum;
}

CRect CPnSise::GetCellRect(int index, BOOL head)
{
	CRect rc(m_rcTable);
	
	for (int i = 0; i < index; i++) 
		rc.left += widths[i];

	rc.right = rc.left + widths[index];

	if (head)
		rc.bottom = rc.top + m_rcTable.Height() / 2;
	else
		rc.top = rc.bottom - m_rcTable.Height() / 2 + 1;

	
	return rc;
}


void CPnSise::DrawSise(CDC& dc)
{
	CRect rc;
	GetClientRect(rc);
	m_rcClient = rc;
	int totalWidth = GetTotalWidth();
	
	m_rcTable = CRect(3, 3, 3 + totalWidth, 3 + rc.Height() - 6);
	
	CFont	*pFont = m_pNMFont ? m_pNMFont: m_pFont;

	CBrush *brBack = m_pApp->GetBrush(m_pwndView, m_clrWindow);
	CBrush *brHead = m_pApp->GetBrush(m_pwndView, m_clrHeader);

	CPen *penWhite = m_pApp->GetPen(m_pwndView, PS_SOLID, 1, clrWHITE);
	CPen *penFrame = m_pApp->GetPen(m_pwndView, PS_SOLID, 1, m_clrBorder);
	CPen *penHead  = m_pApp->GetPen(m_pwndView, PS_SOLID, 1, m_clrHeader);

	CDC mdc;
	mdc.CreateCompatibleDC(&dc);
	CBitmap bmp;
	
	bmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	CBitmap	*sBmp = mdc.SelectObject(&bmp);
	CFont	*sFont = mdc.SelectObject(pFont);
	int	sMode = mdc.SetBkMode(TRANSPARENT);

	mdc.FillRect(rc, brBack);

	CBrush	*sBrush = mdc.SelectObject(brBack);
	CPen	*sPen = mdc.SelectObject(penFrame);
	mdc.Rectangle(m_rcTable);

	CRect rcItem, rcValue;
	rcItem = GetCellRect(0, TRUE);
	rc = CRect(m_rcTable.left, rcItem.top, m_rcTable.right, rcItem.bottom);

	rcItem.top ++;
	mdc.SelectObject(brHead);
	mdc.Rectangle(rc);

	COLORREF clrValue;
	CString  strValue;
	WORD align;
	int dispID;
	_comdisp* disp;
	
	for (int i = cpTime; i <= cpLow; i++) 
	{
		rcItem.right = rcItem.left + widths[i];

		if (i < cpLow) 
		{
			mdc.MoveTo(rcItem.right, m_rcTable.top);
			mdc.LineTo(rcItem.right, m_rcTable.bottom);
		}

		mdc.SetTextColor(clrWHITE);
		mdc.DrawText(CString(heads[i]), rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		rcItem.left += widths[i];

		rcValue = GetCellRect(i, FALSE);
	
		dispID = -1;

		
		switch (i) 
		{
		case cpTime:
			if (m_time.GetLength() >= 4)
			{
				if (m_time == "888800")
					strValue = "외종료";
				else if (m_time == "999900")
					strValue = "장종료";
				else 
					strValue = m_time.Left(2) + ":" + m_time.Mid(2, 2);
			}
			clrValue = 0;
			align = DT_CENTER;
			break;
		case cpName:
			strValue = m_name;
			clrValue = 0;
			align = DT_CENTER;
			break;
		case cpCurr:
			dispID = valCURR;
			break;
		case cpDiff:
			dispID = valDIFF;
			break;
		case cpUpDown:
			dispID = valUDYL;
			break;
		case cpVolume:
			//dispID = valVOL;
			strValue = m_volume;
			if (strValue.Find('.') > 0)
				strValue.Delete(strValue.GetLength() - 3, 3);
			clrValue = 0;
			align = DT_RIGHT;
			break;
		case cpSellHoga:
			dispID = valMDHG;
			break;
		case cpBuyHoga:
			dispID = valMSHG;
			break;
		case cpOpen:
			dispID = valSIGA;
			break;
		case cpHigh:
			dispID = valKOGA;
			break;
		case cpLow:
			dispID = valJEGA;
			break;
		default:
			strValue = "-";
			clrValue = clrWHITE;
			align = DT_CENTER;
			break;
		}

		if (dispID >= 0) 
		{
			disp = FindDispValue(dispID);
			strValue = disp->dispStr;
			clrValue = GetColor(disp->fgcolor);
			align = DT_RIGHT;
		}
		if (align == DT_RIGHT)
			rcValue.right -= 2;
		mdc.SetTextColor(clrValue);
		if (i == cpName)
		{
			CFont	*oldFont = mdc.SelectObject(m_pBDFont);
			mdc.DrawText(strValue, rcValue, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | align);
			mdc.SelectObject(oldFont);
		}
		else
		{
			mdc.DrawText(strValue, rcValue, DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX | align);
		}
	}

	dc.BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), &mdc, 0, 0, SRCCOPY);

	mdc.SelectObject(sBmp);
	mdc.SelectObject(sFont);
	mdc.SetBkMode(sMode);
	mdc.SelectObject(sBrush);
	mdc.SelectObject(sPen);

	return;

}

COLORREF CPnSise::GetColor(int index)
{
	switch (index)
	{
	case clrRED:	return m_clrUp;
	case clrBLUE:	return m_clrDown;
	}

	return m_clrText;
}

_comdisp* CPnSise::FindDispValue(int id) 
{
	_comdisp* disp;
	for (int i = 0; i < m_arDisp.GetSize(); i++) 
	{
		disp = m_arDisp.GetAt(i);
		if (disp->dispID == id)
			return disp;
	}

	return NULL;
}

void CPnSise::SetPaletteColor()
{
	const int piHeader = 74, piBorder = 75, piWindow = 68, piUp = 94, piDown = 95, piFlash = 8, piText = 97;

	m_clrText = GetPalette(piText);
	m_clrUp   = GetPalette(piUp);
	m_clrDown = GetPalette(piDown);
	m_clrHeader = GetPalette(piHeader);
	m_clrFlash = GetPalette(piFlash);
	m_clrBorder = GetPalette(piBorder);
	m_clrWindow = GetPalette(piWindow);
}

COLORREF CPnSise::GetPalette(int index)
{
	return m_pwndGView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getPalette), index);
}
