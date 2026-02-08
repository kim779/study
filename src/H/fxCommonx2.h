#pragma once

#include "axisfire.h"


static BOOL IsExistFile(CString sFilePath)
{
	CFileFind finder;
	if (finder.FindFile(sFilePath))
	{
		finder.Close();
		return TRUE;
	}
	finder.Close();
	return FALSE;
}

static CString parseX(CString &srcstr, CString substr, BOOL bUTF8 = FALSE)
{
	if (bUTF8)
	{
		CString temp = "";
		int nSubStr = substr.GetLength(), 
			nSrcStr = srcstr.GetLength();

		for (int kk = 0; kk < nSrcStr; kk++)
		{
			if (kk + nSubStr < nSrcStr)
			{
				BOOL bOk = TRUE;
				for (int jj = kk; jj < kk + nSubStr; jj++)
				{
					if (srcstr.GetAt(jj) != substr.GetAt(jj - kk))
					{
						bOk = FALSE;
						break;
					}
				}
				if (bOk)
					break;
			}
			temp += srcstr.GetAt(kk);
		}
		srcstr.Delete(0, min(temp.GetLength() + nSubStr, nSrcStr));
		return temp;
	}
	else
	{
		int nFind = srcstr.Find(substr);
		if (nFind == -1)
		{
			CString temp = srcstr;
			srcstr.Empty();
			return temp;
		}
		else
		{
			CString  temp = srcstr.Left(nFind);
			srcstr = srcstr.Mid(nFind + substr.GetLength());
			return temp;
		}
	}
	return "";
}

static CString revParseX(CString &srcstr, char cSubChar)
{
	if (srcstr.ReverseFind(cSubChar) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.ReverseFind(cSubChar));
		srcstr = srcstr.Mid(srcstr.ReverseFind(cSubChar) + 1);
		return temp;
	}
	return "";
}

static CString OneOfParser(CString &srcstr, CString substr, char &cFindChar)
{
	int nIdx = srcstr.FindOneOf(substr);
	if (nIdx == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		cFindChar = NULL;
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(nIdx);
		cFindChar = srcstr.GetAt(nIdx);
		while (temp.Replace("\'", "\'") % 2 != 0)
		{
			if (srcstr.Mid(nIdx + 1).FindOneOf(substr) == -1)
			{
				CString temp = srcstr;
				srcstr.Empty();
				return temp;
			}
			else
				nIdx += srcstr.Mid(nIdx + 1).FindOneOf(substr) + 1;
			temp = srcstr.Left(nIdx);
		}
		srcstr = srcstr.Mid(nIdx + 1);
		srcstr.TrimLeft();	srcstr.TrimRight();
		return temp;
	}
	return "";
}

static CString OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;
	parseX(tmp, sKey);
	tmp = parseX(tmp, "/");
	return tmp;
}

static BOOL OptionParserB(CString sOption, CString sKey)
{
	CString tmp = sOption;
	if (tmp.Find(sKey) >= 0)
		return TRUE;
	return FALSE;
}

static CString GetFontInfo(CString sHome, int nPoint)
{
	CString sInfo, sKey;
	sKey.Format("%02d", nPoint);
	GetPrivateProfileString("FONTENUM", sKey, "", sInfo.GetBuffer(1024), 1024, sHome + "\\tab\\fontinfo.ini");
	sInfo.ReleaseBuffer();
	return sInfo;
}

static CString SetEditMask(CString sSrc, CString sMask)
{
	int nLen = 0;
	CString sRtn = "";

	for (int ii = 0; ii < sMask.GetLength() && sSrc.GetLength() > nLen; ii++)
	{
		if (sMask.GetAt(ii) == '9')
		{
			if (sSrc.GetAt(nLen) >= '0' && sSrc.GetAt(nLen) <= '9')
				sRtn += sSrc.GetAt(nLen++);
		}
		else if (sMask.GetAt(ii) == 'A')
			sRtn += sSrc.GetAt(nLen++);
		else if (sMask.GetAt(ii) == sSrc.GetAt(nLen))
			sRtn += sSrc.GetAt(nLen++);
		else if ((BYTE)sMask.GetAt(ii) > 0x7f && sMask.GetLength() > ii + 1)
			sRtn += sMask.Mid(ii++, 2);
		else
			sRtn += sMask.GetAt(ii);
	}

	if (sMask.GetLength() > ii)
	{
		sMask.Replace("9", "#");
		sMask.Replace("A", "_");
		sRtn += sMask.Mid(ii);
	}

	return sRtn;
}

static int _httoi(const TCHAR *value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	CHexMap HexMap[HexMapL] =
	{
		{'0', 0}, {'1', 1},
		{'2', 2}, {'3', 3},
		{'4', 4}, {'5', 5},
		{'6', 6}, {'7', 7},
		{'8', 8}, {'9', 9},
		{'A', 10}, {'B', 11},
		{'C', 12}, {'D', 13},
		{'E', 14}, {'F', 15}
	};
	TCHAR *mstr = _tcsupr(_tcsdup(value));
	TCHAR *ss = mstr;
	int result = 0;
	if (*ss == '0' && *(ss + 1) == 'X') ss += 2;
	bool firsttime = true;
	while (*ss != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*ss == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		ss++;
		firsttime = false;
	}
	free(mstr);
	return result;
}

static double GetPriceUnit(double price, CString sJCode, bool bPlus)
{
	if (sJCode.IsEmpty())
		return 0;

	double unit = 0;
	if (sJCode[0] == '1')
		return 0.05;
	else
	{
		CString sPrice;
		sPrice.Format("%6.2f", price);
		price = atof(sPrice);

		if (price < 3 || (!bPlus && price == 3))
			return 0.01;
		else
			return 0.05;
	}
}

static CString SetComma(CString src)
{
	CString temp, floatStr;
	int	ii, jj = -1;

	src.TrimLeft();
	if (src.Find('.') >= 0)
	{
		floatStr = src.Mid(src.Find('.'));
		src = src.Left(src.Find('.'));
	}
	else
		floatStr.Empty();
	
	temp = src;
	src.Empty();
	
	for (ii = temp.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (temp.GetAt(0) != '-' && temp.GetAt(0) != '+')
				src = ',' + src;
			else if (ii != 0)
				src = ',' + src;
			
			jj = 0;
		}
		src = temp.GetAt(ii) + src;
	}
	src += floatStr;
	
	return src;
}

static CRect AdjustRect(CRect screenRect)
{
	int nDx = 0, nDy = 0;
	if (screenRect.right > GetSystemMetrics(SM_CXFULLSCREEN))
		nDx = GetSystemMetrics(SM_CXFULLSCREEN) - screenRect.right;
	if (screenRect.bottom > GetSystemMetrics(SM_CYFULLSCREEN))
		nDy = GetSystemMetrics(SM_CYFULLSCREEN) - screenRect.bottom;
	screenRect.OffsetRect(nDx, nDy);
	return screenRect;
}

static int OpenViewA(CWnd *pParent, int type, CString data)
{
	return pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)(LPCTSTR)data);
}

static BOOL SendTRA(CWnd *pParent, CString name, BYTE type, CString data, int key, CString sCtrlName = "")
{
	CString trData = sCtrlName;
	
	struct _userTH udat;
	
	strcpy(udat.trc, name);
	udat.key = key;
	udat.stat = type;

	if (type & US_KEY)
	{
		trData = ((char)LOWORD(key));
		trData += sCtrlName;
		udat.key = (int)HIWORD(key);
	}

	if (!trData.IsEmpty())	// is control
		trData += '\t';

	trData += CString((char *)&udat, L_userTH);
	trData += data;

	return pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, data.GetLength()), (LPARAM)trData.operator LPCTSTR());
}


static CFont* GetAxFontA(CWnd *pParent, CString fName, int point, int style)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point; fontR.italic = false; fontR.bold = 0;

	switch (style)
	{
	case 0: case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)pParent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

static CBitmap* GetAxBitmapA(CWnd *pParent, CString path)
{
	if (!IsExistFile(path))
		path = "";
	return (CBitmap*)pParent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

static CPen* GetAxPenA(CWnd *pParent, COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)pParent->SendMessage(WM_USER, getPEN, (long)&penR);
}

static CBrush* GetAxBrushA(CWnd *pParent, COLORREF clr)
{
	return (CBrush*)pParent->SendMessage(WM_USER, getBRUSH, (long)clr);
}

static COLORREF GetColorA(CWnd *pParent, int color)
{
	if (color & 0x02000000)
		return color;

	return pParent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

static CString VariantA(CWnd *pParent, int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

static CWnd* CreateMapObjectA(CWnd *pParent, CWnd *pMapWnd, CString sMapName, int &nObjKey, CPoint point)
{
	ASSERT(::IsWindow(pParent->m_hWnd));
	ASSERT(::IsWindow(pMapWnd->m_hWnd));

	CWnd *pObjWnd = NULL;
	if (!sMapName.IsEmpty())
	{
		CRect   rc;
		CString sStr;
		long    nSzMap = 0, nType;

		pMapWnd->GetClientRect(&rc);

		sStr = CString(sMapName, 8);
		nSzMap = pParent->SendMessage(WM_USER, sizeMAP, (long)sStr.operator LPCTSTR());

		nType = pParent->SendMessage(WM_USER, typeDLL, (long)sStr.operator LPCTSTR());

		if (nType != vtypeERR)
		{
			pObjWnd = new CWnd();
			pObjWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, LOWORD(nSzMap), HIWORD(nSzMap)), pMapWnd, 0);
			nObjKey = pParent->SendMessage(WM_USER, MAKEWPARAM(attachMAP, nType), (long)pObjWnd);
			sStr = sMapName;
			pParent->SendMessage(WM_USER, MAKEWPARAM(loadMAP, nObjKey), (long)sStr.operator LPCTSTR());
			pObjWnd->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
		}
	}
	return pObjWnd;
}

static void DeleteMapObjectA(CWnd *pParent, CWnd *pObjWnd, int nObjKey)
{
	if (!pObjWnd || !::IsWindow(pObjWnd->m_hWnd))
		return;
	pParent->SendMessage(WM_USER, MAKEWPARAM(detachMAP, nObjKey));
	pObjWnd->DestroyWindow();
	delete pObjWnd;
	pObjWnd = NULL;
}

static wchar_t* utf8tounicode(const unsigned char *utf8, long *pLength = NULL)
{
	int size = 0, index = 0, out_index = 0;
	wchar_t *out;
	unsigned char c;
	
	/* first calculate the size of the target string */
	
	c = utf8[index++];
	
	while(c) {
		if((c & 0x80) == 0) {
			index += 0;
		} else if((c & 0xe0) == 0xe0) {
			index += 2;
		} else {
			index += 1;
		}
		
		size += 1;
		c = utf8[index++];
	}   
	
	out = (unsigned short*)malloc((size + 1) * sizeof(wchar_t));
	
	if (out == NULL)
		return NULL;

	index = 0;
	
	c = utf8[index++];
	
	while(c)
	{
		if((c & 0x80) == 0) {
			out[out_index++] = c;
		} else if((c & 0xe0) == 0xe0) {
			out[out_index] = (c & 0x1F) << 12;
			c = utf8[index++];
			out[out_index] |= (c & 0x3F) << 6;
			c = utf8[index++];
			out[out_index++] |= (c & 0x3F);
		} else {
			out[out_index] = (c & 0x3F) << 6;
			c = utf8[index++];
			out[out_index++] |= (c & 0x3F);
		}
		c = utf8[index++];
	}
	
	out[out_index] = 0;
	if (pLength)
		*pLength = out_index;
	
	return out;
}

static CString UTF2ANSI(CString sUTF, UINT uCP = -1)
{
	CString strResult = _T("");
	wchar_t *unicode;
	int chars, err;

	unicode = utf8tounicode((const unsigned char*)(LPCTSTR)sUTF);

	if (unicode == NULL) 
	{
		TRACE("Out of memory processing string from UTF8 to UNICODE16\n");
		return strResult.AllocSysString();
	}
	
	chars = WideCharToMultiByte((uCP == -1?GetConsoleCP():uCP), WC_COMPOSITECHECK, unicode,
		-1, NULL, 0, NULL, NULL);
	
	if (chars == 0)
	{
		TRACE("Unicode translation error %d\n", GetLastError());
		free(unicode);
		return strResult.AllocSysString();
	}
	
	err = WideCharToMultiByte((uCP == -1?GetConsoleCP():uCP), WC_COMPOSITECHECK, unicode, 
		-1, strResult.GetBuffer(chars), chars, NULL, NULL);
	strResult.ReleaseBuffer();
	
	if (err != chars)
	{
		TRACE("Unicode translation error %d\n", GetLastError());
		free(unicode);
		return strResult.AllocSysString();
		
	}
	
	free(unicode);

	return strResult;
}

static CString PART_UTF2ANSI(CString sData)
{
	CString sRtn = "";
	CString sDel;
	CString sTmp;

	sDel.Format("\t%c", 0x7f);

	while (!sData.IsEmpty())
	{
		sRtn += parseX(sData, sDel);

		if (sData.GetLength() >= 10)
		{
			UINT uLen = atoi(sData.Left(10));
			sData = sData.Mid(10);
			sTmp = UTF2ANSI(CString(sData, uLen));
			sData = sData.Mid(uLen);
			sRtn += "\t" + sTmp;
		}
	}

	return sRtn;
}

#include <afxpriv.h>
static CString convertToUTF8(LPCTSTR str)
{
    USES_CONVERSION;
    const WCHAR* wStr = T2CW(str);

    int nUTF8codeSize = WideCharToMultiByte(CP_UTF8, 0, wStr, -1, NULL, 0, NULL, NULL); 

    CString utf8Str;
    WideCharToMultiByte(CP_UTF8, 0, wStr, -1, utf8Str.GetBuffer(nUTF8codeSize), nUTF8codeSize, 0, 0);
    utf8Str.ReleaseBuffer();

    return utf8Str;
}



static CString convert_Category(CString sCategorys)
{
	CString sRtn;
	while (!sCategorys.IsEmpty())
	{
		CString sCode = parseX(sCategorys, "\t");
		if (sCode.GetLength() >= 10)
		{
			if (sCode.Left(1) == 'X')
			{
				CString sOrgCode = sCode;
				parseX(sOrgCode, (char)0x7f);
				sCode.Empty();
				while (sOrgCode.GetLength() >= 10)
				{
					sCode += sOrgCode.Left(10) + "\t";
					sOrgCode.Delete(0, 10);
				}
				if (!sCode.IsEmpty())
					sCode = sCode.Left(sCode.GetLength() - 1);
			}
			sRtn += sCode + "\t";
		}
		else
			continue;
	}
	return sRtn;
}

static CString token_Path(CString sPath, char cDel = '\\')
{
	CString sFileName;
	if (sPath.ReverseFind(cDel) >= 0)
	{
		sFileName = sPath.Mid(sPath.ReverseFind(cDel) + 1);
		sPath = sPath.Mid(0, sPath.ReverseFind(cDel));
	}
	else
		return sPath;
	return sPath + "\t" + sFileName;
}

static CString ReadFile(CString sFilePath)
{
	CString sRtn = "";
	if (!IsExistFile(sFilePath))
		return "";

	CFile file(sFilePath, CFile::modeRead);
	UINT uRead = 0;

	do
	{
		CString sTmp;
		uRead = file.Read(sTmp.GetBuffer(1024 * 10), 1024 * 10);
		sTmp.ReleaseBuffer();
		if (uRead)
			sRtn += CString(sTmp, uRead);
	} while (uRead);
	
	return sRtn;
}

static void WriteFile(CString sFilePath, CString sData)
{
	CFile file(sFilePath, CFile::modeCreate|CFile::modeWrite);
	if (file.operator HFILE())
	{
		file.Write((LPCTSTR)sData, sData.GetLength());
		file.Close();
	}
}

static CString addNotBrace(CString sData)
{
	CString sTmp = sData;
	int nPos = 0;
	nPos = sTmp.Find("~", nPos);
	while (nPos != -1)
	{
		nPos++;
		if (sTmp.GetLength() <= nPos)
			sTmp += ")";
		else
		{
			while (sTmp.GetLength() > nPos && sTmp.GetAt(nPos) == ' ')
				nPos++;
			BOOL bBrace = FALSE;
			while (sTmp.GetLength() > nPos)
			{
				if (sTmp.GetAt(nPos) == ' ' ||
					sTmp.GetAt(nPos) == '(' ||
					sTmp.GetAt(nPos) == ')' ||
					sTmp.GetAt(nPos) == '&' ||
					sTmp.GetAt(nPos) == '|')
				{
					bBrace = TRUE;
					sTmp.Insert(nPos, ")");
					break;
				}
				nPos++;
			}
			if (!bBrace)
				sTmp += ")";
		}
		nPos = sTmp.Find("~", nPos);
	}
	return sTmp;
}

static CString convertOperations(CString sData)
{
	sData = addNotBrace(sData);
	sData.Replace("&", " and ");
	sData.Replace("|", " or ");
	sData.Replace("~", "( not ");
	return sData;
}

static CSize GetJpgSize(CString sImagePath)
{
	if (!IsExistFile(sImagePath))
		return CSize(-1, -1);
#ifdef _GDIPLUS
	CSize szImg = CSize(-1, -1);
	wchar_t ws[MAX_PATH];
	
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sImagePath, -1, ws, MAX_PATH);
	Image *pImg = Image::FromFile(ws);
	
	if (pImg)
	{
		Status status = pImg->GetLastStatus();
		if (status == Ok)
		{
			szImg.cx = pImg->GetWidth();
			szImg.cy = pImg->GetHeight();
		}
		delete pImg;
	}
	
	return szImg;
#else
	CSize szImg = CSize(-1, -1);
TRY
{
	CFile file_jpg(sImagePath, CFile::modeRead);
	BYTE buf[4];
	int nHeight = 0, nWidth = 0;
	
	file_jpg.Read(&buf[0], 1);
	for (UINT ii = 1; ii < file_jpg.GetLength(); ii++)
	{
		file_jpg.Read(&buf[1], 1);
		if (buf[0] == 0xFF && buf[1] == 0xC0)
		{
			file_jpg.Seek(3, SEEK_CUR);
			file_jpg.Read(buf, 4);
			szImg.cy = (int)buf[0] * 0xff + (int)buf[1];
			szImg.cx = (int)buf[2] * 0xff + (int)buf[3];
			break;
		}
		else if (buf[0] == 0xff && (buf[1] >= 0xe0 && buf[1] <= 0xe1))
		{
			file_jpg.Read(buf, 2);
			file_jpg.Seek((int)buf[0] * 0xff + (int)buf[1] - 2, SEEK_CUR);
			ii += (int)buf[0] * 0xff + (int)buf[1];
			file_jpg.Read(&buf[1], 1);
		}
		buf[0] = buf[1];
	}
	file_jpg.Close();
}
CATCH(CFileException, e )
{
	e->Delete();
}
END_CATCH

	return szImg;
#endif
}

static CString SetSearchTag(CString sSrc, CString sSearchWords, BOOL bColor = FALSE)
{
	if (sSearchWords.IsEmpty())
		return sSrc;
	
	CString sSearch = sSearchWords;
	char cFind;
	sSearch.Replace("\"", "\'");

	while (!sSearch.IsEmpty())
	{
		CString sWord = OneOfParser(sSearch, " ", cFind);
		sWord.Replace("\'", "");
		if (bColor)
			sSrc.Replace(sWord, "<font color=#0033cc><b>" + sWord + "</b></font>");
		else
			sSrc.Replace(sWord, "<b>" + sWord + "</b>");
	}
	return sSrc;
}

static void ConvertSymbolicTag(LPCTSTR source, CString& dest)
{
	struct _tagSymbolTag {
		char tag[10];
		char symbol[3];
	} tagarr[] = {
				{ "&quot;", "\"" },
				{ "&amp; ", "&" },
				{ "&lt;  ", "<" },
				{ "&gt;  ", ">" },
				{ "&nbsp;", " " },
				{ "&iexcl; ", "¢®" },
				{ "&cent;  ", "¡Ë" },
				{ "&pound; ", "¡Ì" },
				{ "&curren;", "¢´" },
				{ "&yen;   ", "¡Í" },
				{ "&brvbar;", "|" },
				{ "&sect;  ", "¡×" },
				{ "&uml;   ", "¡§" },
				{ "&copy;  ", "¨Ï" },
				{ "&ordf;  ", "¨£" },
				{ "&laquo; ", "¡ì" },
				{ "&not;   ", "¡þ" },
				{ "&shy;   ", "¡©" },
				{ "&reg;   ", "¢ç" },
				{ "&macr;  ", "?" },
				{ "&deg;   ", "¡Æ" },
				{ "&plusmn;", "¡¾" },
				{ "&sup2;  ", "©÷" },
				{ "&sup3;  ", "©ø" },
				{ "&acute; ", "¢¥" },
				{ "&micro; ", "¥ì" },
				{ "&para;  ", "¢Ò" },
				{ "&middot;", "¡¤" },
				{ "&cedil; ", "¢¬" },
				{ "&sup1;  ", "©ö" },
				{ "&ordm;  ", "¨¬" },
				{ "&raquo; ", "¡í" },
				{ "&frac14;", "¨ù" },
				{ "&frac12;", "¨ö" },
				{ "&frac34;", "¨ú" },
				{ "&iquest;", "¢¯" },
				{ "&agrave;", "a" },
				{ "&aacute;", "a" },
				{ "&acirc; ", "a" },
				{ "&atilde;", "a" },
				{ "&auml;  ", "a" },
				{ "&aring; ", "a" },
				{ "&aelig; ", "©¡" },
				{ "&ccedil;", "c" },
				{ "&egrave;", "e" },
				{ "&eacute;", "e" },
				{ "&ecirc; ", "e" },
				{ "&euml;  ", "e" },
				{ "&igrave;", "i" },
				{ "&iacute;", "i" },
				{ "&icirc; ", "i" },
				{ "&iuml;  ", "i" },
				{ "&eth;   ", "©£" },
				{ "&ntilde;", "n" },
				{ "&ograve;", "o" },
				{ "&oacute;", "o" },
				{ "&ocirc; ", "o" },
				{ "&otilde;", "o" },
				{ "&ouml;  ", "o" },
				{ "&times; ", "¡¿" },
				{ "&oslash;", "©ª" },
				{ "&ugrave;", "u" },
				{ "&uacute;", "u" },
				{ "&ucirc; ", "u" },
				{ "&uuml;  ", "u" },
				{ "&yacute;", "y" },
				{ "&thorn; ", "©­" },
				{ "&szlig; ", "©¬" },
				{ "&agrave;", "a" },
				{ "&aacute;", "a" },
				{ "&acirc; ", "a" },
				{ "&atilde;", "a" },
				{ "&auml;  ", "a" },
				{ "&aring; ", "a" },
				{ "&aelig; ", "©¡" },
				{ "&ccedil;", "c" },
				{ "&egrave;", "e" },
				{ "&eacute;", "e" },
				{ "&ecirc; ", "e" },
				{ "&euml;  ", "e" },
				{ "&igrave;", "i" },
				{ "&iacute;", "i" },
				{ "&icirc; ", "i" },
				{ "&iuml;  ", "i" },
				{ "&eth;   ", "©£" },
				{ "&ntilde;", "n" },
				{ "&ograve;", "o" },
				{ "&oacute;", "o" },
				{ "&ocirc; ", "o" },
				{ "&otilde;", "o" },
				{ "&ouml;  ", "o" },
				{ "&divide;", "¡À" },
				{ "&oslash;", "©ª" },
				{ "&ugrave;", "u" },
				{ "&uacute;", "u" },
				{ "&ucirc; ", "u" },
				{ "&uuml;  ", "u" },
				{ "&yacute;", "y" },
				{ "&yuml;  ", "y" },
				{ "", ""}
			};
		
	dest = source;
	CString tag;
	for (int i = 0; tagarr[i].tag[0]; i++) {
		tag = tagarr[i].tag;
		tag.TrimRight();
		dest.Replace(tag, tagarr[i].symbol);
	}
		
	int find1, find2, idx = 0;
	while (TRUE) {
		find1 = dest.Find("&#", idx);

		if (find1 < 0) 
			break;

		idx = find1 + 2;

		find2 = dest.Mid(find1).FindOneOf("; ");

		if (find2 < 0) 
			break;
		find2 += find1;

		tag = dest.Mid(idx, find2 - idx);
		dest.Delete(find1, find2 - find1 + 1);

		if (atoi(tag) <= 0x7f)
			dest.Insert(find1, (char)atoi(tag));
			
		idx = find1 + 1;
	}
}

static void OpenBrowser(CString sURL)
{
	IWebBrowser2* pie;
	
	CLSID         clsid;

	HRESULT hr = ::CLSIDFromProgID(OLESTR("InternetExplorer.Application"), &clsid);
	
	hr = ::CoCreateInstance(clsid, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**) &pie);
	
	if (SUCCEEDED(hr) && pie)
	{
		COleVariant url;
		COleVariant vNull((short)NULL);
		url.SetString(_T(sURL), VT_BSTR);
		
		pie->put_Height(600);
		pie->put_Width(800);
		pie->put_MenuBar(TRUE);
		pie->put_ToolBar(TRUE);
		pie->put_AddressBar(TRUE);
		pie->put_StatusBar(TRUE);
		pie->put_Visible(TRUE);
		HWND hBrowserWnd;
		pie->get_HWND((long*)&hBrowserWnd);
		
		pie->Navigate2(&url, &vNull, &vNull, &vNull, &vNull);
		pie->Release();	
		if (hBrowserWnd)
			SetWindowPos(hBrowserWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
	}
}

static CString LoadStringTable(UINT nID)
{
	CString str;
	str.LoadString(nID);
	return str;
}

static void euc2sjis(unsigned char *d, const unsigned char *s, int len)
{
	register int    i;
	unsigned char  *high, *low;
	
	for (i = 0 ; i < len ; ) {
		if (!(*s & 0x80)) {
			*d = *s;
			i++, s++, d++;
		} else if ((*s & 0xFF) == 0x8E) {
			*d = *s;
			i++, s++, d++;
			if (i >= len) break;
			*(d-1) = *s;
			i++, s++;
		} else {
			*d = *s;
			*(d+1) = *(s+1);
			high = d;
			low = d + 1;
			*high = (*high & ~0x80) & 0xff;
			*low = (*low & ~0x80) & 0xff;
			*low += (*high & 1) ? 0x1F : 0x7D;
			if (*low > 0x7E) (*low)++;
			*high = ((*high - 0x21) >> 1) + 0x81;
			if (*high > 0x9F) (*high) += 0x40;
			i += 2, s += 2, d += 2;
			if (i > len) {
				i--, s--, d--;
			}
		}
	}
	*d = '\0';
}

static CTime GetTimeByStr(CString sDateTime)
{
	if (sDateTime.GetLength() < 14)
		return CTime();
	CTime rtnTime(atoi(sDateTime.Left(4)),	// Year
			atoi(sDateTime.Mid(4, 2)),	// Month
			atoi(sDateTime.Mid(6, 2)),	// Day
			atoi(sDateTime.Mid(8, 2)),	// Hour
			atoi(sDateTime.Mid(10, 2)),	// Minute
			atoi(sDateTime.Mid(12, 2)));	// Second
	return rtnTime;
}


#if WINVER >= 0x0500
static CRect GetMonitorRectFromPt_ctrl(CPoint pt)
{
	CRect rcDesktop;
	if (GetDesktopWindow() && IsWindow(GetDesktopWindow()))
		GetWindowRect(GetDesktopWindow(), rcDesktop);
	
	MONITORINFO mi;
	mi.cbSize = sizeof(mi);
	HMONITOR hMonitor = MonitorFromPoint(pt, MONITOR_DEFAULTTOPRIMARY);
	if (GetMonitorInfo(hMonitor, &mi))
		rcDesktop = mi.rcWork;
	return rcDesktop;
}
#endif

