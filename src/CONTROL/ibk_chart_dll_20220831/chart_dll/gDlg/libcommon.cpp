#include "stdafx.h"
#include "libcommon.h"

void LIB_MessageBox(char* fmt, ...)
{
	CString	tmpstr;

	va_list	valist;
	va_start(valist, fmt);
	tmpstr.FormatV(fmt, valist);
	AfxGetMainWnd()->MessageBox(tmpstr, COMPANYNAME);
	va_end(valist);
}

CString LIB_GetComboText(CComboBox* pCombo)
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

CString LIB_GetStrToken(int nIndex, CString data, char* token)
{
	CString retval = _T("");
	char*	dataptr = NULL;
	char*	value = NULL;
	int	nValue = 0;
	int	nLen = 	data.GetLength() + 1;
	int	ii = 0;
	char*	next_token1 = nullptr;
	char*	next_token2 = nullptr;

	if (data.IsEmpty())
		return _T("");

	dataptr = (char*)malloc(nLen);

	if (dataptr == nullptr)
		return _T("");

	ZeroMemory(dataptr, nLen);
	CopyMemory(dataptr, (char*)data.GetString(), nLen - 1);

	retval = value = strtok_s(dataptr, token, &next_token1);

	if ( ii == nIndex )
	{
		free(dataptr);
		return retval;
	}

	while (value)
	{
		ii++;
		retval = value = strtok_s(NULL, token, &next_token2);
		
		if (ii == nIndex)
		{
			free(dataptr);
			return retval;
		}
	}

	free(dataptr);
	return _T("");	
}

CRect	LIB_MoveWindow(CWnd* pWnd, CPoint pt)
{
	if (pWnd == NULL)
		return CRect(0, 0, 0, 0);

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

CString LIB_MakeComma(CString data)
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

CString LIB_MakeParen(struct _graph* pGraph, struct _indiInfo* pIndi)
{
	int	ii = 0; 
	int	nCount = pIndi->bcnt;
	CString	tmpstr = _T(""), tmpstr1 = _T("");
	WORD	opt = pGraph->cInfo.wCOption;
	WORD	flag = 0x01;

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		flag = 0x01 << ii;
		if (pIndi->basic[ii].info.kind == _COMBO_)
		{
			tmpstr1 = _T("");

			for ( int jj = 0 ; jj < pIndi->basic[ii].ccnt ; jj++ )
			{
				if (pIndi->basic[ii].combo[jj].value == pGraph->cInfo.awValue[ii])
					tmpstr1.Format("%s", pIndi->basic[ii].combo[jj].content);
			}				
		}
		else if (pIndi->basic[ii].info.kind == _MACOMBO_ || pIndi->basic[ii].info.kind == _VOLCOMBO_)
		{
			tmpstr1 = _T("");
			int	nPos = 0;

			for ( int jj = 0 ; jj < pIndi->basic[ii].ccnt ; jj++ )
			{
				if (pIndi->basic[ii].combo[jj].value & pGraph->wCalOption)
				{
					nPos = jj;
				}					
			}

			tmpstr1.Format("%s", pIndi->basic[ii].combo[nPos].content);
		}
		else
		{
			if (pGraph->cInfo.awValue[ii] > 0 && (opt & flag))
				tmpstr1.Format("%d", (int)pGraph->cInfo.awValue[ii]);
			else
				tmpstr1 = _T("");
		}
		

		tmpstr += tmpstr1;

		if (!tmpstr1.IsEmpty())
			tmpstr += ",";		
	}

	if (tmpstr.IsEmpty())
		return tmpstr;
		
	tmpstr = "(" + tmpstr + ")";
	tmpstr.Replace(",)", ")");
	return tmpstr;
}

COLORREF LIB_GetReverseColor(COLORREF color)
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

void LIB_ShowHide(CWnd* pParent, UINT shflag, UINT first, ...)
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
