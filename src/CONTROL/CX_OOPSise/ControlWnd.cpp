// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_OOPSise.h"
#include "ControlWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define		P_TAB		'\t'	// 0x09
#define		P_NEW		'\n'	// 0x0a


#define		C_BACKIDX		181
#define		C_LBLBACKIDX		77
#define		C_MEDOBACKIDX		82
#define		C_MESUBACKIDX		85
#define		C_LBLBACKIDX2		78
#define		C_LINEIDX		43

#define		C_DATAFGIDX		69
#define		C_LBLFGIDX		130

static	int	signtxtColorIdx[] = {94, 95};
static	int	txtAlign[] = {DT_LEFT, DT_CENTER, DT_RIGHT};


/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	m_strRtsSymbol = "1021";
	m_bRecv = FALSE;
	m_pFont = m_pBoldFont = nullptr;
	m_iSymbolCnt = 0;
	m_iVOutLine = 0;
	m_iHOutLine = 1;

	m_strDateSplit = "/";
	m_strTimeSplit = ":";
	EnableAutomation();
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
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_FUNCTION(CControlWnd, "Send", Send, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetValue", GetValue, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Clear", Clear, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {A9BEF83E-7126-47B4-868D-FFF950DBA237}
static const IID IID_IControlWnd =
{ 0xa9bef83e, 0x7126, 0x47b4, { 0x86, 0x8d, 0xff, 0xf9, 0x50, 0xdb, 0xa2, 0x37 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers
COLORREF CControlWnd::getAxColor(int color)
{
	if (color & 0x02000000)
		return color;
	return SendAxisMessage(MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

HCURSOR CControlWnd::getAxCursor(int ikind)
{
	return (HCURSOR)SendAxisMessage(getCURSOR, (LPARAM)ikind);
}

CFont* CControlWnd::getAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct	_fontR	fontR {};

	fontR.name = (LPSTR)fName.GetString();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)SendAxisMessage(getFONT, (LPARAM)&fontR);
}


CFont* CControlWnd::getAxFont(struct _fontR* font)
{
	return getAxFont(font->name, font->point, (font->bold == FW_BOLD) ? true : false, font->italic);
}

HBITMAP CControlWnd::getBitmap(CString path, int idir)
{
//	if (idir != 0) path = m_strImagePath + path;

	path = m_strImagePath + path;
	CBitmap* pBitmap = (CBitmap*)SendAxisMessage(getBITMAP, (LPARAM)path.GetString());
	return pBitmap->operator HBITMAP();
}

CPen* CControlWnd::getAxPen(COLORREF clr, int width, int style)
{
	struct	_penR	penR {};

	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*) SendAxisMessage(getPEN, (LPARAM)&penR);
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char* dta = (char*)SendAxisMessage(MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = CString(dta);
	
	return retvalue;
}

LRESULT CControlWnd::SendAxisMessage(WPARAM wParam, LPARAM lParam)
{
	return m_pMainWindow->SendMessage(WM_USER, wParam, lParam);
}


LONG CControlWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		struct	_extTHx* exth;
		exth = (struct _extTHx*)lParam;
		ParseRecvData(CString(exth->data, exth->size));
		break;
	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!!
		ParseRecvData((char*)lParam);
		break;
	case DLL_ALERTx:
		struct _alertR* alertR;
		alertR = (struct _alertR*)lParam;
		ParseRealData(alertR);
		break;
	case DLL_ALERT:	// NOT USED. Use DLL_ALERTx !!!
		break;
	case DLL_SETPAL:
		Invalidate(FALSE);
		break;

	case DLL_SETFONT:
	case DLL_SETFONTx:
		{
			if (m_pFont)
				DeleteObject(m_pFont);
			if (m_pBoldFont)
				DeleteObject(m_pBoldFont);

			m_pFont = getAxFont(CString((char*)lParam), HIWORD(wParam));
			m_pBoldFont = getAxFont(CString((char*)lParam), HIWORD(wParam), true);
			Invalidate(FALSE);
		}
 		break;

	case DLL_TRIGGER: //설정확인도 여기서
		break;
	case DLL_DOMINO:
		break;
	case DLL_GUIDE:
		return 1;
	default:break;
	}

	return 0;
}

#if 0
void CControlWnd::ParseRealData(CString sData)
{
	if (m_iSymbolCnt <= 0 || !m_bRecv || m_arDrawList.GetSize() <= 0)
		return;
	
	CString sRealCode = SplitString(sData, P_TAB);
	sRealCode.Trim();
	if (sRealCode.IsEmpty() || m_strRealCode != sRealCode)
		return;
	
	CMapStringToString arMapList;
	CString sval, spos;
	DrawList plist;
	CStringArray	arRealList, arRowList;

	int	irow = 0, icol = 0, ifind = 0;
	SplitString(sData, P_NEW, arRowList);
	for (int r = arRowList.GetSize()-1; r >= 0; r--)
	{
		if (SplitString(arRowList[r], P_TAB, arRealList) > 0)
		{
			for (int ii = 0; ii < arRealList.GetSize(); ii+=2)
			{
				if (m_arRealCodeMap.Lookup(arRealList[ii], spos))
				{
					ifind = spos.Find(",");
					if (ifind < 0) continue;

					irow = CInt(spos.Left(ifind));
					spos = spos.Right(spos.GetLength() - ifind - 1);  
					icol = CInt(spos);

					plist = m_arDrawList.GetAt(irow);
					sval = arRealList[ii+1];
					sval.Trim();
					plist.stext[icol] = SetDataFormat(sval, plist.iDataFormat[icol], plist.iorgfgcolor[icol], plist.ifgcolor[icol], plist.sType[icol]);

				//	plist.stext[jj] =  sval;
					m_arDrawList.SetAt(irow, plist);
					InvalidateRect(plist.rc[icol]); 

				}
			}
		}
	}
}
#endif

void CControlWnd::ParseRealData(struct _alertR* alertR)
{
	if (m_iSymbolCnt <= 0 || !m_bRecv || m_arDrawList.GetSize() <= 0)
		return;
	
	CString sRealCode = alertR->code;
	if (sRealCode.IsEmpty() || m_strRealCode != sRealCode)
		return;
	
	CString sval;
	DrawList plist;
	int	irow = 0, icol = 0;
	int	isym = 0;
	DWORD	xpos = 0;
	DWORD*	data = (DWORD*)alertR->ptr[0];

	for (POSITION pos = m_arRealCodeMap.GetStartPosition(); pos; )
	{
		m_arRealCodeMap.GetNextAssoc(pos, isym, xpos);
		if (!data[isym])
			continue;

		irow = LOWORD(xpos);
		icol = HIWORD(xpos);
		if (irow >= m_arDrawList.GetSize() ||  icol >= MAX_DATA)
			continue;

		plist = m_arDrawList.GetAt(irow);
		sval = (char*)data[isym];
		sval.Trim();
		plist.stext[icol] = SetDataFormat(sval, plist.iDataFormat[icol], plist.iorgfgcolor[icol], plist.ifgcolor[icol], plist.sType[icol]);
		
	//	plist.stext[jj] =  sval;
		m_arDrawList.SetAt(irow, plist);
		InvalidateRect(plist.rc[icol]); 
	}
}

CString CControlWnd::SetDataFormat(CString sData, int iDataFormat, int iOrgFgColor, int& iFgcolor, CString& stype)
{
	//txtformat 0-lbl, 1-+/-comma, 2comma, 3:+/-signcolor, 4:signcolor
	//5-diff, 6-yyyy-mm-dd, 7-yy-mm-dd, 8-HH:MM:SS, 9-MM:SS, 10-signcolortext
	//11-+↑, -↓ comma/signcolor
	//12-+▲, -▼ comma/signcolor
	//13:signcolor, -표시

	iFgcolor = iOrgFgColor;
	if (iDataFormat < 5)
	{
		if (sData.GetLength() > 1)
		{
			if (sData.Left(1) == "0" && sData.FindOneOf("+/") > 0)
				sData = sData.Right(sData.GetLength() -1);  
		}
	}
	
	CString sResult = sData;
	CString sTypes[] = { "", "↑", "▲", "", "↓", "▼" };
	int	iType = 0;

	stype = _T("");
	sData.TrimLeft();
	switch (iDataFormat)
	{
	case 1:
		sResult = SetComma(sData);
		break;
	case 2:
		sResult = SetComma(sData);
		if (sResult.FindOneOf("+/-") == 0)
			sResult = sResult.Right(sResult.GetLength() - 1);  
		break;
	case 3:
		sResult = SetComma(sData);
		if (sResult.FindOneOf("+/-") == 0)
		{
			if (sResult.Left(1) == "+")
				iFgcolor = signtxtColorIdx[0];
			else	iFgcolor = signtxtColorIdx[1];
		}
		break;

	case 4:
		sResult = SetComma(sData);
		if (sResult.FindOneOf("+/-") == 0)
		{
			if (sResult.Left(1) == "+")
				iFgcolor = signtxtColorIdx[0];
			else	iFgcolor = signtxtColorIdx[1];
			sResult = sResult.Right(sResult.GetLength() - 1);  
		}
		break;

	case 5:
		iType = CInt(sResult.Left(1));
		if (sResult.GetLength() <= 1) 
			break;

		sResult = SetComma(sResult.Right(sResult.GetLength() - 1));
		if (sResult != "0")
		{
			if (iType > 0 && iType < 3)
				iFgcolor = signtxtColorIdx[0];
			if (iType > 3)
				iFgcolor = signtxtColorIdx[1];
		}
		stype = sTypes[iType];
		//1-↑
		//2-▲
		//5-▼
		//4-↓
		break;

	case 6://yyyy-mm-dd
		if (sData.GetLength() > 7)
			sResult = sData.Left(4) + m_strDateSplit + sData.Mid(4, 2) + m_strDateSplit + sData.Right(2);   
		else	sResult = sData;
		break;

	case 7://yy-mm-dd
		if (sData.GetLength() > 7)
			sResult = sData.Mid(2,2) + m_strDateSplit + sData.Mid(4, 2) + m_strDateSplit + sData.Right(2);   
		else	sResult = sData;
		break;

	case 8://8-HH:MM:SS, 9-MM:SS, 10-signcolortext
		if (sData.GetLength() > 5)
			sResult = sData.Left(2) + m_strTimeSplit + sData.Mid(2, 2) + m_strTimeSplit + sData.Right(2);   
		else	sResult = sData;
		break;
	case 9:
		if (sData.GetLength() > 5)
			sResult = sData.Mid(2, 2) + m_strTimeSplit + sData.Right(2);   
		else	sResult = sData;
		break;

	case 10:
		if (sResult.FindOneOf("+/-") == 0)
		{
			if (sResult.Left(1) == "+")
				iFgcolor = signtxtColorIdx[0];
			else	iFgcolor = signtxtColorIdx[1];
			sResult = sResult.Right(sResult.GetLength() - 1);  
		}
		break;

	case 11:
	case 12:
		sResult = SetComma(sData);
		if (sResult.FindOneOf("+/-") == 0)
		{
			const	int	isign = 12 - iDataFormat;
			stype = sTypes[2-isign];
		//	sTypes[] = {"", "↑", "▲", "", "↓", "▼"};
			if (sResult.Left(1) == "+")
				iFgcolor = signtxtColorIdx[0];
			else 
			{
				iFgcolor = signtxtColorIdx[1];
				stype = sTypes[5-isign];
			}
			sResult = SetComma(sResult.Right(sResult.GetLength() - 1));
		}
		break;

	case 13:
		sResult = SetComma(sData);
		if (sResult.FindOneOf("+/-") == 0)
		{
			if (sResult.Left(1) == "+") 
			{
				iFgcolor = signtxtColorIdx[0];
				sResult = sResult.Right(sResult.GetLength() - 1);  
			}
			else	iFgcolor = signtxtColorIdx[1];
		}
		break;

	default:break;
	}

	return sResult;
}

void CControlWnd::ParseRecvData(CString sData)
{
	m_arRealCodeMap.RemoveAll();
	if (m_iSymbolCnt <= 0)
		return;

	CStringArray astrResult;
	if (SplitString(sData, P_TAB, astrResult) <= 0)
		return;

	m_strRealCode = astrResult[0];
	m_strRealCode.Trim();

	int	idx = 1; //1021-0
	DrawList plist;
	DWORD	xpos = 0;

	for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
	{
		plist = m_arDrawList.GetAt(ii);
		for (int jj = 0; jj < plist.icol; jj++)
		{
			if (astrResult.GetSize() <= idx)
				break;

			plist.sType[jj] = _T("");
			if (plist.iType [jj])
			{
				xpos = (DWORD)MAKELONG(ii, jj);
				m_arRealCodeMap.SetAt(plist.srtssymbol[jj], xpos); 
				astrResult[idx].Trim();
				plist.stext[jj] = SetDataFormat(astrResult[idx++], plist.iDataFormat[jj], 
										plist.iorgfgcolor[jj], plist.ifgcolor[jj], 
										plist.sType[jj]);
			}
		}

		m_arDrawList.SetAt(ii, plist);
	}

	Invalidate(FALSE);
	m_bRecv = TRUE;

}

void CControlWnd::SetTotValue(BOOL bRedraw)
{

}

void CControlWnd::SetParam(struct _param* param, CWnd *parent)
{
	m_pMainWindow	= parent;
	m_key		= param->key;
	m_strParamName	= CString(param->name, param->name.GetLength());
	m_pFont		= getAxFont(_T("굴림체"), 9);
	m_pBoldFont	= getAxFont(_T("굴림체"), 9, true);

	m_clrBgColor	= getAxColor(C_BACKIDX);
	m_iRowHeight	= 20;
	CString sopt	= param->options;
	m_arDrawList.RemoveAll();
/*
[CX_OOPSise]
symbols|1|현재가;2023;거래량;2027;\n대비;2024;거래대금;2028;\n||s
types|1|0;1;0;1;\n0;1;0;1;\n||t
widths|1|0;1;0;1;\n0;1;0;1;\n||w
vlines|1|0;1;0;1;\n0;1;0;1;\n||v
hlines|1|0;1;0;1;\n0;1;0;1;\n||h
txtformats|1|0;1;0;1;\n0;1;0;1;\n||f
FGColor|1|0;1;0;1;\n0;1;0;1;\n||fg
FontDefault/Bold|1|0;1;0;1;\n0;1;0;1;\n||fb
BgColor|1|0;1;0;1;\n0;1;0;1;\n||bg
txtAlign|1|0;1;0;1;\n0;1;0;1;\n||ta
line|2|00001002|75|line
*/
	m_strCodeType = "1301";
	if (!sopt.IsEmpty())
	{
		CString sList[10];

		m_strCodeType = GetFindOptionString(sopt, "c", "/");
		if (m_strCodeType.IsEmpty())
			m_strCodeType = "1301";

		m_strRtsSymbol = GetFindOptionString(sopt, "r", "/");
		if (m_strRtsSymbol.IsEmpty())
			m_strRtsSymbol = "1021";
		sList[0] = GetFindOptionString(sopt, "s", "/");
		sList[1] = GetFindOptionString(sopt, "t", "/");
		sList[2] = GetFindOptionString(sopt, "w", "/");
		sList[3] = GetFindOptionString(sopt, "v", "/");
		sList[4] = GetFindOptionString(sopt, "h", "/");
		sList[5] = GetFindOptionString(sopt, "f", "/");
		sList[6] = GetFindOptionString(sopt, "fg", "/");
		sList[7] = GetFindOptionString(sopt, "ta", "/");
		sList[8] = GetFindOptionString(sopt, "fb", "/");
		sList[9] = GetFindOptionString(sopt, "bg", "/");

		m_strDateSplit = GetFindOptionString(sopt, "d1", "/");
		m_strTimeSplit = GetFindOptionString(sopt, "t1", "/");

		m_iLineColorIdx = CInt(GetFindOptionString(sopt, "line", "/"));
		CStringArray	arList[10];

		for (int ii = 0; ii < 10; ii++)
			SplitString(sList[ii], "|", arList[ii]);

		for (int ii = 0; ii < arList[0].GetSize(); ii++)
		{
			m_arDrawList.Add(DrawList(arList[0].GetAt(ii),  arList[1].GetAt(ii), arList[2].GetAt(ii),
						arList[3].GetAt(ii), arList[4].GetAt(ii), arList[5].GetAt(ii), arList[6].GetAt(ii),
						arList[7].GetAt(ii), arList[8].GetAt(ii), arList[9].GetAt(ii)));
		}
	}

	if (m_strDateSplit.IsEmpty())
		m_strDateSplit = "/";
	if (m_strTimeSplit.IsEmpty())
		m_strTimeSplit = ":";

	/*
	sList[0] = "현재가;2023;거래량;2027;\n대비;2024;거래대금;2028;\n"; //sym
	sList[1] = "0;1;0;1;\n0;1;0;1;\n";//type
	sList[2] = "3;2;3;2;\n3;2;2;3;\n";//width
	sList[3] = "1;0;1;0;\n1;0;1;0;\n";//vline
	sList[4] = "0;0;0;0;\n0;0;0;0;\n";//hline
	sList[5] = "0;2;0;2;\n0;5;0;2;\n";	//txtformat 0-lbl, 1-+/-comma, 2-comma, 3-+/-signcolor, 4-signcolor
	//5-diff, 6-yyyy-mm-dd, 7-yy-mm-dd, 8-HH:MM:SS, 9-MM:SS, 10-signcolortext
	sList[6] = "69;69;69;69;\n 69;69;69;69;\n";
	sList[7] = "0;2;0;2;\n 0;2;0;2;\n"; //0-left,1-center,right
	sList[8] = "0;1;0;0;\n 0;1;0;0;\n"; //default?bold
	sList[9] = "96;68;96;68;\n 96;77;96;77;\n"; //backcoloridx
	m_iLineColorIdx = 75;
	*/
}


int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CControlWnd::ResizePanelBox()
{
	CRect	rc;

	GetClientRect(&rc);

	const	int	irows = m_arDrawList.GetSize();
	m_iRowHeight = rc.Height() / irows ;
	const	int	iWidth = rc.Width();
	int	ileft = 0, itop = 0, iTmp = 0, iright = 0;
	DrawList pList;

	for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
	{
		pList = m_arDrawList.GetAt(ii);
		ileft = 0;
		for (int jj = 0; jj < pList.icol; jj++)
		{
			pList.rc[jj].SetRectEmpty();
			if (pList.lWidthR[jj] > 0)
			{
				iTmp = (int)((float)iWidth * (pList.lWidthR[jj] / 100.f));
				iright = ileft + iTmp;
				if (pList.icol == jj+1)
					iright = iWidth;
				pList.rc[jj] = CRect(ileft, itop, iright, itop + m_iRowHeight);
				ileft += iTmp;
			}
		}

		m_arDrawList.SetAt(ii, pList);
		itop += m_iRowHeight;
	}
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC	backdc;
	CBitmap bitmap,  *pOldBitmap;
	CRect	rectClient;

	GetClientRect(rectClient);
	backdc.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rectClient.right, rectClient.bottom);
	pOldBitmap = (CBitmap*)backdc.SelectObject(&bitmap);
	backdc.SetBkMode(TRANSPARENT);
	//m_clrBgColor
	backdc.FillSolidRect(rectClient, m_clrBgColor); 
	
	Draw(&backdc, rectClient);
	dc.BitBlt(rectClient.left , rectClient.top , rectClient.right , rectClient.bottom , &backdc, 0,0, SRCCOPY);
	bitmap.DeleteObject(); 
	backdc.SelectObject(pOldBitmap);
	DeleteObject(pOldBitmap);
	ReleaseDC(&backdc);	
}

int CControlWnd::GetTextWidth(CDC* pDC, CString sText)
{
	return pDC->GetTextExtent(sText).cx;
}

void CControlWnd::Draw(CDC* pDC, CRect rcClient)
{
	DrawList plist;
	CRect	rc, rc1;
	CFont* pOldFont = nullptr;
	CPen	pen(PS_SOLID, 1, getAxColor(m_iLineColorIdx));
	CPen*	pOldPen = pDC->SelectObject(&pen); 
	CString sTxt;
	int	itxtw = 0;

	for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
	{
		plist = m_arDrawList.GetAt(ii);
		for (int jj = 0; jj < plist.icol; jj++)
		{
			rc = plist.rc[jj];
			pDC->FillSolidRect(rc, getAxColor(plist.ibgcolor[jj]));

			rc.DeflateRect(1,0,0,0);
			if (!plist.iHLine[jj])
				rc.DeflateRect(0,-1,0,0);
			else	rc.DeflateRect(0,1,0,0);
			if (!plist.iVLine[jj])
				rc.InflateRect(0,0,1,0);

//			pDC->FillSolidRect(rc, getAxColor(plist.ibgcolor[jj]));

/*			if (plist.iVLine[jj])
			{
				pDC->MoveTo(plist.rc[jj].right, plist.rc[jj].top); 
				pDC->LineTo(plist.rc[jj].right, plist.rc[jj].bottom);
			}

			if (plist.iHLine[jj])
			{
				pDC->MoveTo(plist.rc[jj].left, plist.rc[jj].bottom); 
				pDC->LineTo(plist.rc[jj].right, plist.rc[jj].bottom);
				
			}
*/
			rc = plist.rc[jj];
			rc.DeflateRect(2,2,2,1);

			pDC->SetTextColor(getAxColor(plist.ifgcolor[jj]));  

			if (plist.iFontBold[jj])
				pOldFont = pDC->SelectObject(m_pBoldFont);	
			else	pOldFont = pDC->SelectObject(m_pFont);	

			if (plist.iDataFormat[jj] == 5 || plist.iDataFormat[jj] == 11 || plist.iDataFormat[jj] == 12)
			{
				const	int	iw = GetTextWidth(pDC, plist.stext[jj]);
				const	int	iw2 = GetTextWidth(pDC, plist.sType[jj]);

				rc1 = rc;
				rc1.left = rc.right - iw - iw2 - 20;//
				rc1.right = rc1.left + iw2;
				pDC->DrawText(plist.sType[jj] , rc1, DT_LEFT|DT_SINGLELINE|DT_VCENTER);

				rc.left = rc1.right+20;
			}

			sTxt = plist.stext[jj];
			if (plist.iType[jj] == 2) 
			{
			//	sTxt +=  plist.suffixes[jj]; //접미
				itxtw = GetTextWidth(pDC, plist.suffixes[jj]);
				pDC->DrawText(plist.suffixes[jj], rc, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);  
				rc.right -= (itxtw+2);
			}

			if (plist.iType[jj] == 3)
				sTxt = plist.suffixesA[jj] + sTxt + plist.suffixes[jj];
			pDC->DrawText(sTxt , rc, txtAlign[ plist.iTxtAlign[jj]  ]|DT_SINGLELINE|DT_VCENTER);  
			if (pOldFont)
				pDC->SelectObject(pOldFont);
		}
	}

	for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
	{
		plist = m_arDrawList.GetAt(ii);
		for (int jj = 0; jj < plist.icol; jj++)
		{
			if (plist.iVLine[jj])
			{
				pDC->MoveTo(plist.rc[jj].right, plist.rc[jj].top); 
				pDC->LineTo(plist.rc[jj].right, plist.rc[jj].bottom);
			}

			if (plist.iHLine[jj])
			{
				pDC->MoveTo(plist.rc[jj].left, plist.rc[jj].bottom); 
				pDC->LineTo(plist.rc[jj].right, plist.rc[jj].bottom);
				
			}
		}
	}

	if (pOldFont)
		pDC->SelectObject(pOldFont); 

	if (m_iHOutLine)
	{
		pDC->MoveTo(rcClient.left, rcClient.top);
		pDC->LineTo(rcClient.right, rcClient.top); 

		pDC->MoveTo(rcClient.left, rcClient.bottom-1);
		pDC->LineTo(rcClient.right, rcClient.bottom-1); 
	}

	if (m_iVOutLine)
	{
		pDC->MoveTo(rcClient.left, rcClient.top);
		pDC->LineTo(rcClient.left, rcClient.bottom-1); 

		pDC->MoveTo(rcClient.right, rcClient.top);
		pDC->LineTo(rcClient.right, rcClient.bottom-1); 
	}

	pDC->SelectObject(pOldPen); 
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	ResizePanelBox();
}


void CControlWnd::SendTR(CString sCode)
{
	DrawList plist;
	CString	text, code, stmp, sval;

	stmp.Format("%s%c%s\t", m_strCodeType, 0x7f, sCode);
	stmp += m_strRtsSymbol;
	stmp += '\t';

	m_iSymbolCnt = 0;
	for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
	{
		plist = m_arDrawList.GetAt(ii);
		for (int jj = 0; jj < plist.icol; jj++)
		{
			if (plist.iType[jj])
			{
				sval = plist.symbol[jj];
				sval.Trim();
				if (sval.IsEmpty())
					continue;
				
				stmp += sval;
				stmp += '\t';
				m_iSymbolCnt++;
			}
		}
	}
	if (m_iSymbolCnt <= 0)
		return;

	text.Format("%s%c%c", stmp, P_NEW, P_TAB); 
	sendTR("pooppoop", text, US_OOP);
}

void CControlWnd::sendTR(CString trCode, CString sdatB, int istat)
{
	const	int	datL = sdatB.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(datL + L_userTH + 128);
	struct _userTH udat {};

	// control name
	int	idx = m_strParamName.GetLength();
	CopyMemory(pWb.get(), m_strParamName, idx);
	pWb[idx++] = '\t';

	// _userTH
	CopyMemory(udat.trc, trCode, min(sizeof(udat.trc), trCode.GetLength()));
	udat.key = m_key;
	udat.stat = istat;
	CopyMemory(&pWb[idx], &udat, L_userTH);
	idx += L_userTH;

	// data
	CopyMemory(&pWb[idx], sdatB, datL);

	SendAxisMessage(MAKEWPARAM(invokeTRx, datL), (LPARAM)pWb.get());
	pWb.reset();
}

void CControlWnd::Send(LPCTSTR sCode) 
{
	// TODO: Add your dispatch handler code here
	if (m_strCode == CString(sCode) || strlen(sCode) == 0)
		return;

	SendTR(sCode);
	m_strCode = CString(sCode);
}

void CControlWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnLButtonDown(nFlags, point);
}

void CControlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnMouseMove(nFlags, point);
}

BOOL CControlWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BSTR CControlWnd::GetValue(LPCTSTR sCode) 
{
	CString strResult;
	DrawList plist;

	// TODO: Add your dispatch handler code here
	for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
	{
		plist = m_arDrawList.GetAt(ii);
		for (int jj = 0; jj < plist.icol; jj++)
		{
			if (plist.iType[jj]) 
			{
				if (plist.symbol[jj]  == CString(sCode))
				{
					strResult = plist.stext[jj]; 
					break;
				}
			}
		}
	}

	return strResult.AllocSysString();
}

void CControlWnd::Clear() 
{
	DrawList plist;

	for (int ii = 0; ii < m_arDrawList.GetSize(); ii++)
	{
		plist = m_arDrawList.GetAt(ii);
		for (int jj = 0; jj < plist.icol; jj++)
		{
			if (plist.iType[jj]) 
			{
				plist.stext[jj];
			}
		}

		m_arDrawList.SetAt(ii, plist);
	}
	Invalidate(FALSE);
}
