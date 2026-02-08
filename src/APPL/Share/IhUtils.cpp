// IhUtils.cpp: implementation of the IhUtils class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IhUtils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#include <math.h>
IH::IH()
{

}

IH::~IH()
{

}

CSize IH::GetStringSize(CWnd* pWnd, CString string)
{
	CSize	size;
	CDC*	pDC = pWnd->GetDC();
	size = pDC->GetTextExtent(string);
	pWnd->ReleaseDC(pDC);
	return size;
}

int IH::GetStringWidth(CWnd* pWnd, CString string)
{
	return IH::GetStringSize(pWnd, string).cx;
}

int IH::GetStringHeight(CWnd* pWnd, CString string)
{
	return IH::GetStringSize(pWnd, string).cy;
}

CSize IH::GetStringSize(CWnd* pWnd, CFont* pFont, CString string)
{
	CSize	size;
	CDC*	pDC = pWnd->GetDC();
	CFont*	oldFont = (CFont*)pDC->SelectObject(pFont);
	size = pDC->GetTextExtent(string);
	pDC->SelectObject(oldFont);
	pWnd->ReleaseDC(pDC);
	return size;
}

int IH::GetStringWidth(CWnd* pWnd, CFont* pFont, CString string)
{
	return IH::GetStringSize(pWnd, pFont, string).cx;
}

int IH::GetStringHeight(CWnd* pWnd, CFont* pFont, CString string)
{
	return IH::GetStringSize(pWnd, pFont, string).cy;
}


void IH::CreateFont(CFont& font, int height, CString fname, int weight)
{
	bool	bItalic = false;

	LOGFONT	logfont;
	logfont.lfHeight         = height * 10;
	logfont.lfWidth          = 0;
	logfont.lfEscapement     = 0;
	logfont.lfOrientation    = 0;
	logfont.lfWeight         = weight;
	logfont.lfItalic         = false;
	logfont.lfUnderline      = false;
	logfont.lfStrikeOut      = false;
	logfont.lfCharSet        = HANGUL_CHARSET;
	logfont.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality        = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy(logfont.lfFaceName, fname);
	font.CreatePointFontIndirect(&logfont);
}

void IH::CreatePointFont(CFont& font, CString fname, int fpoint)
{
	font.CreatePointFont(fpoint * 10, fname);
}

void IH::MessageBox(char* fmt, ...)
{
	CString	tmpstr;

	va_list	valist;
	va_start(valist, fmt);
	tmpstr.FormatV(fmt, valist);
	AfxMessageBox(tmpstr);
	va_end(valist);
}

CString IH::GetComboText(CComboBox* pCombo)
{
	if (pCombo == NULL)
		return "";

	int	nPos = CB_ERR;	
	CString	tmpstr;

	nPos = pCombo->GetCurSel();

	if (nPos == CB_ERR)
	{
		return "";
	}
	
	pCombo->GetLBText(nPos, tmpstr);
	return tmpstr;
}

CString IH::StrToken(int nIndex, CString data, char* token)
{
	CString retval = _T("");
	char*	dataptr = NULL;
	char*	value = NULL;
	int	nValue = 0;
	int	nLen = 	data.GetLength() + 1;
	int	ii = 0;

	if (data.IsEmpty())
		return "";

	dataptr = (char*)malloc(nLen);
	memset(dataptr, 0x00, nLen);
	memcpy(dataptr, data, nLen - 1);

	retval = value = strtok(dataptr, token);

	if ( ii == nIndex )
	{
		free(dataptr);
		return retval;
	}

	while (value)
	{
		ii++;
		retval = value = strtok(NULL, token);
		
		if (ii == nIndex)
		{
			free(dataptr);
			return retval;
		}
	}

	free(dataptr);

	return "";	
}

CRect	IH::MoveWindow(CWnd* pWnd, CPoint pt)
{
	if (pWnd == NULL)
		return NULL;

	CRect	rect;
	CSize	size;	

	pWnd->GetWindowRect(rect);
	size.cx = rect.Width();
	size.cy = rect.Height();

	rect.left = pt.x;
	rect.right = pt.x + size.cx;
	rect.top = pt.y;
	rect.bottom = rect.top + size.cy;

	pWnd->MoveWindow(rect);
	return rect;
}

CString IH::MakeComma(CString data)
{

	int	val = 0, ii = 0;
	CString	tmpstr = _T("");

	val = atoi((char*)(LPSTR)(LPCTSTR)data);

	if (val < 1000)
	{
		data.Format("%d", val);
		return data;
	}

	val = data.GetLength() - 1;

	for ( ii = 0 ; ii < data.GetLength() ; ii++ ) 
	{
		tmpstr = data.GetAt(val - ii) + tmpstr;

		if ((ii + 1) % 3 == 0)
			tmpstr = "," + tmpstr;
	}

	if (tmpstr.GetAt(0) == ',')
		tmpstr = tmpstr.Mid(1);
	
	return tmpstr;
}

COLORREF IH::GetReverseColor(COLORREF color)
{
	int red = GetRValue(color);
	int green = GetGValue(color);
	int  blue = GetBValue(color);

	if ((red == green && green == blue)
		&& (red > 120 && red < 140) 
		&& (green > 120 && green < 140)
		&& (blue > 120 && blue < 140))
	{
		red = red - 20;
		green = green - 20;
		blue = blue - 20;
	}

	return RGB(255-red, 255-green, 255-blue);
}

void IH::ShowHide(CWnd* pParent, UINT shflag, UINT first, ...)
{
	if (pParent == NULL)
		return;

	CWnd*	pWnd = NULL;
	int count = 0;
	UINT	ctrlID = first;
	va_list marker;

	if ((int)ctrlID <= 0)
		return;

	va_start( marker, first );

	
	while((int)ctrlID > 0)
	{		
		pWnd = pParent->GetDlgItem(ctrlID);

		if (pWnd == NULL)
			break;

		pWnd->ShowWindow(shflag);		
		ctrlID = va_arg( marker, UINT);	
		count++;
	}

	va_end( marker );              /* Reset variable arguments.      */
}


double IH::CharToDouble(char* data, int len)
{
	CString sztmp = CString(data, len);
	double ret = atof(sztmp);
	return ret;
}

double IH::CharToDouble(CString data)
{
	double ret = atof(data);
	return ret;
}

UINT IH::SetCheckInfo(CWnd* pWnd, UINT id, UINT option, UINT value)
{
	UINT	ret = value; 
	CButton*	pButton = (CButton*)pWnd->GetDlgItem(id);
	if (pButton->GetCheck())
		ret |= option;
	else
		ret &= ~option;
	return ret;
}

CString IH::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void IH::DrawBox(CDC* pDC, CRect rect, COLORREF clrLine, int opt, COLORREF clrInner, bool bFill)
{
	CPen	pen(PS_SOLID, 1, clrLine);
	CPen*	oldpen = pDC->SelectObject(&pen);
	
	if (bFill)
		pDC->FillSolidRect(rect, clrInner);
	
	if (opt & BOX_LEFT)
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(CPoint(rect.left, rect.bottom));
	}

	if (opt & BOX_RIGHT)
	{
		pDC->MoveTo(CPoint(rect.right - 1, rect.top));
		pDC->LineTo(CPoint(rect.right - 1, rect.bottom));
	}

	if (opt & BOX_TOP)
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(CPoint(rect.right, rect.top));
	}

	if (opt & BOX_BOTTOM)
	{
		pDC->MoveTo(rect.left, rect.bottom);
		pDC->LineTo(CPoint(rect.right, rect.bottom));
	}
	
	pDC->SelectObject(oldpen);
	pen.DeleteObject();
}

void IH::DrawText(CDC* pDC, CRect rect, CString string, COLORREF color, int option)
{
	COLORREF ocolor = pDC->SetTextColor(color);
	int	nMode = pDC->SetBkMode(TRANSPARENT);
	int	opt  = DT_SINGLELINE|DT_VCENTER | option;

	pDC->DrawText(string, rect, opt);
	pDC->SetBkMode(nMode);
	pDC->SetTextColor(ocolor);
}

void IH::HEXDUMP(char* msg, char* buf, int len, bool bFile)
{
	int ii, jj, kk;
	char    lbuf[80];
	time_t  c_clock;
	struct  tm *tm;

	
	memset(lbuf, 0x00, sizeof(lbuf));
	c_clock = time(0);
	tm = localtime(&c_clock);

	FILE*	fp = NULL;
	if (bFile)
	{
		CString	filename;
		filename.Format("c:\\dump%02d%02d%02d.txt", tm->tm_hour, tm->tm_min, tm->tm_sec);
		fp = fopen(filename, "w+");
	}

	if (!bFile)
	{
		TRACE("%02d:%02d:%02d  %s (%d)\n",
		tm->tm_hour, tm->tm_min, tm->tm_sec, msg, len);
	}
	else
	{
		fprintf(fp, "%02d:%02d:%02d  %s (%d)\n",
		tm->tm_hour, tm->tm_min, tm->tm_sec, msg, len);
	}

	for (ii = 0; ii < len; )
	{
		kk = ii;

		if (!bFile)
			TRACE("%04d   ", ii);
		else
			fprintf(fp, "%04d   ", ii);
		
		for (jj = 0; jj < 15 && ii < len; jj++, ii++)
		{
			if (!bFile)
				TRACE("%02X ", buf[ii]);
			else
				fprintf(fp, "%02X ", buf[ii]);
		}
		
		for (; jj < 15; jj++)
		{
			if (!bFile)
				TRACE("   ", buf[ii]);
			else
				fprintf(fp, "   ", buf[ii]);
		}
		
		if (!bFile)
			TRACE("  ");
		else
			fprintf(fp, "  ");
		
		for (ii = kk, jj = 0; jj < 15 && ii < len; jj++, ii++)
		{
			if (buf[ii] < ' ' || buf[ii] >= 0x80)
			{
				if (!bFile)
					TRACE(".");
				else
					fprintf(fp, ".");
			}
			else    
			{
				if (!bFile)
					TRACE("%c", buf[ii]);
				else
					fprintf(fp, "%c", buf[ii]);
			}
		}
		
		if (!bFile)
			TRACE("\n");
		else
			fprintf(fp, "\n");
	}
	if (len > 0)	
	{
		if (!bFile)
			TRACE("\n");
		else
			fprintf(fp, "\n");
	}

	if (bFile)
		fclose(fp);
}

double IH::Sum(CArray<double*, double*>* pArray, int nday, int nIndex)
{
	if (pArray == NULL)
		return UNUSED_VALUE;

	int	ncnt = pArray->GetSize();

	if (nday <= 0 || nday > ncnt)
		return UNUSED_VALUE;

	if (nIndex < nday - 1)
		return UNUSED_VALUE;

	double	sum = 0;
	double	val = 0;

	for ( int ii = 0 ; ii < nday ; ii++ )
	{
		val = *(pArray->GetAt(nIndex - ii));
		sum += val;
	}

	return sum;
}

double IH::CalculateSMA(CArray<double*, double*>* pArray, int nday, int nIndex)
{
	double	val = IH::Sum(pArray, nday, nIndex);

	if (val == UNUSED_VALUE)
		return UNUSED_VALUE;
	
	val = val / (double)nday;
	return val;
}

int IH::CalculateSMA(CArray<double, double>& pOutput, CArray<double*, double*>* pArray, int nday)
{
	pOutput.RemoveAll();
	if (nday == 0)
		return 0;

	if (pArray == NULL)
		return 0;

	int	ncnt = pArray->GetSize();
	int	ii = 0 ; 
	double	val = 0;

	if (ncnt < nday)
	{
		for ( ii = 0 ; ii < ncnt ; ii++ )
		{
			pOutput.Add(UNUSED_VALUE);
		}

		return 0;
	}

	for ( ii = 0 ; ii < nday - 1 ; ii++ )
	{
		pOutput.Add(UNUSED_VALUE);
	}
	
	val = 0;
	for ( ii = nday - 1 ; ii < ncnt ; ii++ )
	{
		if (ii == nday - 1)
		{
			val = IH::Sum(pArray, nday, ii);
		}
		else
		{
			val = val + *pArray->GetAt(ii) - *pArray->GetAt(ii - nday);
		}
	
		pOutput.Add(val/(double)nday);
	}
	return 0;
}

// 선물/옵션 가격을 지정한 Tick 만큼 가감 처리하여 돌려주는 함수

double IH::CalcFutureOptionJiSu(double val, int nTick, BOOL bOption)
{
	long int nJiSu = (long int)val;
	int nSign = nTick > 0? 1: -1;

	// 옵션
	if(bOption == TRUE)
	{
		for(int _i=0; _i<abs(nTick); _i++)
		{
			if((0 <= nJiSu) && (nJiSu <= 299))
			{
				nJiSu += (1 * nSign);
			}
			else if(nJiSu >= 300)
			{
				nJiSu += (5 * nSign);
			}
		}
	}
	// 선물
	else 
	{
		for(int _i=0; _i<abs(nTick); _i++)
		{
			nJiSu += (5 * nSign);
		}
	}

	return nJiSu;
}

// 주식(KOSPI, KOSDAQ) 가격을 지정한 Tick 만큼 가감 처리하여 돌려주는 함수
int IH::CalcStockPrice(LPCTSTR pszPrice, int nTick, BOOL bKospi)
{
	int nSign = nTick < 0? -1: 1;
	int nCount = abs(nTick);
	int nPrice = atoi(pszPrice);

	if (bKospi)	// KOSPI 주식
	{
		for(int _i=0; _i<nCount; _i++)
		{
			if (nPrice < 5000)	
				nPrice += (5 * nSign);
			else if (nPrice < 10000)
				nPrice += (10 * nSign);
			else if (nPrice < 50000)
				nPrice += (50 * nSign);
			else if (nPrice < 100000)
				nPrice += (100 * nSign);
			else if (nPrice < 500000)
				nPrice += (500 * nSign);
			else if (nPrice >= 500000)
				nPrice += (1000 * nSign);
		}
	}
	else		// KOSDAQ 주식
	{
		for(int _i=0; _i<nCount; _i++)
		{
			if (nPrice < 5000)
				nPrice += (5 * nSign);
			else if (nPrice < 10000)
				nPrice += (10 * nSign);
			else if (nPrice < 50000)
				nPrice += (50 * nSign);
			else if (nPrice >= 50000)
				nPrice += (100 * nSign);
		}
	}

	return nPrice;
}


BOOL IH::IsFileExist(CString filename)   //file_name은 path를 포함한 모든 경로
{
     CFileFind   finder;

     if(!finder.FindFile(filename))
            return FALSE;

     return TRUE;
}

int IH::GetUnitInList(CMapStringToString& mapList, CString dat, CString deli, CString start, CString end)
{
	int	ret = 0;
	int	nFind = 0;
	CString	szUnit = _T("");
	mapList.RemoveAll();

	if (dat.IsEmpty() || deli.IsEmpty())
		return 0;

	if (!end.IsEmpty())
	{
		nFind = dat.Find(end);
		if (nFind >= 0)
			dat = dat.Left(nFind);
		else
			return 0;
	}

	if (!start.IsEmpty())
	{
		nFind = dat.Find(start);
		if (nFind >= 0)
			dat = dat.Mid(nFind + 1);
		else
			return 0;
	}

	while (!dat.IsEmpty())
	{
		szUnit = IH::Parser(dat, deli);
		if (!szUnit.IsEmpty())
			mapList.SetAt(szUnit, "");
	}

	ret = mapList.GetCount();
	return ret;
}

LONG IH::Main(CWnd* pMain, WORD kind, WORD key, LPARAM param)
{
	if (pMain)
		return pMain->SendMessage((WM_USER + 1000), MAKEWPARAM(kind, key), param);
	return 0;
}

void IH::DrawDivideH(CDC* pDC, CRect rect)
{
	pDC->FillSolidRect(rect, RGB(192, 192, 192));
	CPen	penW(PS_SOLID, 1, RGB(255, 255, 255)), penB(PS_SOLID, 1, RGB(128, 128, 128));
	CPen*	oldpen = NULL;

	oldpen = pDC->SelectObject(&penW);
	pDC->MoveTo(rect.TopLeft());
	pDC->LineTo(rect.left, rect.bottom);
	pDC->SelectObject(oldpen);
	penW.DeleteObject();

	oldpen = pDC->SelectObject(&penB);
	pDC->MoveTo(rect.right, rect.top);
	pDC->LineTo(rect.BottomRight());
	pDC->SelectObject(oldpen);
	penB.DeleteObject();
}

void IH::DrawDivideV(CDC* pDC, CRect rect)
{
	pDC->FillSolidRect(rect, RGB(192, 192, 192));
	CPen	penW(PS_SOLID, 1, RGB(255, 255, 255)), penB(PS_SOLID, 1, RGB(128, 128, 128));
	CPen*	oldpen = NULL;

	oldpen = pDC->SelectObject(&penW);
	pDC->MoveTo(rect.TopLeft());
	pDC->LineTo(rect.right, rect.top);
	pDC->SelectObject(oldpen);
	penW.DeleteObject();

	oldpen = pDC->SelectObject(&penB);
	pDC->MoveTo(rect.left, rect.bottom);
	pDC->LineTo(rect.BottomRight());
	pDC->SelectObject(oldpen);
	penB.DeleteObject();
}

void IH::PrintLog(CString filename, char* fmt, ...)
{
	CString	tmpstr;
	FILE*	fp = NULL;

	fp = fopen(filename, "a+");
	if (!fp)
		return;
	va_list	valist;
	va_start(valist, fmt);
	tmpstr.FormatV(fmt, valist);
	fprintf(fp, "%s", tmpstr);
	fclose(fp);	
	va_end(valist);	
}

void IH::PrintTickCount(int& start, char* fmt, ...)
{
	int val = GetTickCount();
	CString	tmpstr;
	va_list	valist;
	va_start(valist, fmt);
	tmpstr.FormatV(fmt, valist);
	va_end(valist);	
	TRACE("[%d] [%s]\n", val - start, tmpstr);
	start = val;
}

double IH::TOfabs(CString string)
{
	double dval = 0;
	if (string.IsEmpty()) return dval;
	string.Replace("++", "+");
	string.Replace("--", "-");

	dval = fabs(atof(string));
	return dval;
}

double IH::TOf(CString string)
{
	double dval = 0;
	if (string.IsEmpty()) return dval;
	string.Replace("++", "+");
	string.Replace("--", "-");
	
	dval = atof(string);
	return dval;
}

CString IH::idTOstring(UINT id)
{
	CString	msg;
	if (!msg.LoadString(id))
		msg = _T("");
	return msg;
}

CString	IH::charTOstring(char* datB, int datL)
{
	CString	string = CString(datB, datL);
	string.TrimLeft(); string.TrimRight();
	return string;
}

int IH::ParsingOption(CMapStringToString& fms, CString stream, CString token)
{
	fms.RemoveAll();
	CString	sym, dat;
	IH::Parser(stream, token);
	int	nToken = token.GetLength();
	if (nToken <= 0)
		return 0;

	while(stream.GetLength())
	{
		sym = stream.Left(nToken);
		stream = stream.Mid(nToken);
		dat = IH::Parser(stream, token);
		fms.SetAt(sym, dat);
	}

	return fms.GetCount();
}

//** macho add start 2007.07.27

// ** static functions
void LIB_DeleteWnd(CWnd* pWnd)
{
	if (pWnd)
	{
		if (::IsWindow(pWnd->m_hWnd))
			pWnd->DestroyWindow();
		delete pWnd;
		//pWnd = NULL;
	}
}

BOOL LIB_IsWndAlive(CWnd* pWnd)
{
	if (pWnd)
		return ::IsWindow(pWnd->m_hWnd);
	else
		return FALSE;
}
//** macho add end 2007.07.27


//** macho add
#ifndef kospiType
	#define kospiType		1			// if kind == codeType, *type return (kospiType ~ indexType)
	#define kosdaqType		2
	#define	thirdType		3			
	#define	mufundType		4			// 뮤추얼펀드
	#define	reitsType		5			// 리츠
	#define etfType			6			// ETF
	#define	sinjuType		7			// 신주인수권
	#define	hyfundType		8			// 하일리펀드
	#define	elwType			9			// elw
	#define	foreignType		10			// 외국주
	#define	futureType		11			// 선물
	#define	callType		12			// 콜옵션
	#define	putType			13			// 풋옵션
	#define	koptionType		15			// 주식옵션
	#define	sfutureType		16			// 주식선물
	#define	indexType		20			// 업종
#endif // kospiType

int IH::GetCodeType(CWnd* view, CString& code)
{
	int codeL = code.GetLength();
	if (codeL <= 0) return 0;

	int result = view->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)code);

	switch (result)
	{
	case kospiType:				// kospi code
	case kosdaqType:			// kosdaq code
		return	kospiCODE;;
	case elwType:				// ELW
		return	elwCODE;
	case futureType:
		if (code[0] == '1' || code[0] == '4')
			return	futureCODE;		// 선물
		else
			return  optionCODE;		// 옵션
	case callType:				// call option
	case putType:				// put  option
		return  optionCODE;
	case indexType:				// 업종
		return	indexCODE;
	case thirdType:				// 제3시장
		return	thirdCODE;
	case sinjuType:				// 신주인수권
		return	sinjuCODE;
	case etfType:
		return  etfCODE;
	case mufundType:			// 뮤츄얼펀드
	case reitsType:				// 리츠
	case hyfundType:			// 하이일드펀드
	default:
		break;
	}

	return 0;
}
