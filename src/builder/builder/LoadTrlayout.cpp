#include "LoadTrlayout.h"
#include "h/mainvar.h"
#include "h/seq.H"

CLoadTrlayout::CLoadTrlayout()
{
	m_strInOutChange = _T("");
	m_trName = _T("");
}

CLoadTrlayout::~CLoadTrlayout()
{
	//m_arryInputData.RemoveAll();
	//m_arryOutputData.RemoveAll();
	m_aryTRLayout.RemoveAll();

	m_in.RemoveAll();
	m_out.RemoveAll();
}

void CLoadTrlayout::loadTrLayoutFile(CString path)
{
	CFile file;
	CString text;
	//path = _T("C:\\BMT\\trlayout\\PTBO0104"); //테스트용
	if (!file.Open(path, CFile::modeRead))
		return;

	UINT	nRead, size = (UINT)file.GetLength();
	char*	p = new char[size + 1];

	nRead = file.Read(p, size);
	if (size != nRead)
	{
		file.Close();
		delete[] p;
		return;
	}
	file.Close();
	p[size] = '\0';

	text = UTF8ToAnsi(p);

	// remove garbage
	if (text.Find("TR=") != -1)
		text = text.Mid(text.Find("TR="));

	CString strData, tmps;
	m_in.RemoveAll();
	m_out.RemoveAll();
	while (!text.IsEmpty())
	{
		strData = Parser(text, _T("\n"));
		strData.Trim();
		tmps = Parser(strData, _T("="));
		if (!tmps.CompareNoCase(_T("TR")))
			m_trName = strData;

		else if (!tmps.CompareNoCase(_T("DESCRIPTION")) || !tmps.CompareNoCase(_T("PATH")))
			continue;

		else if (!tmps.CompareNoCase(_T("FORMAT")))
			TRACE("%s\n", strData);
		else if (!tmps.CompareNoCase(_T("in")))
		{
			//m_in.Add(strData);
			//CString tmps;
			//while (!strData.IsEmpty())
			//{
			//	tmps = Parser(strData, _T(":"));
			//	setSymbol(_T("I"), strData);
			//}
			SetInputData(strData);
		}
		else if (!tmps.CompareNoCase(_T("OUT")))
			m_out.Add(strData);
	}

	m_strInOutChange = _T("");
	//for (int ii = 0; ii < m_in.GetCount(); ii++)
	//{
	//	text = m_in.GetAt(ii);
	//	MakeInputObject(text);
	//}

//	for (int ii = 0; ii < m_out.GetCount(); ii++)
//	{
//		text = m_out.GetAt(ii);
//		makeOut(text);
//	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	if (mapData.Lookup("TR", strData))
//		TRACE("%s\n", strData);
//	if (mapData.Lookup("PATH", strData))
//		TRACE("%s\n", strData);
//	if (mapData.Lookup("FORMAT", strData))
//		TRACE("%s\n", strData);
//	if (mapData.Lookup("IN", strData))
//	{
//		MakeInputObject(strData);
//	}
//	if (mapData.Lookup("OUT", strData))
//		MakeOutputObject(strData);
}

void CLoadTrlayout::MakeInputObject(CString strData)
{
	CString tmps;
	while (!strData.IsEmpty())
	{
		tmps = Parser(strData, _T(","));
		setSymbol(_T("I"), tmps);
	}
	//CString strInput("");
	//CString strName(""), strType(""), strLen("");
	//m_arryInputData.RemoveAll();
	//while (!strData.IsEmpty())
	//{
	//	struInputData struData;
	//	strInput = Parser(strData, ",");

	//	struData.strName = Parser(strInput, ":");
	//	struData.strType = Parser(strInput, ":");
	//	struData.strLen = Parser(strInput, ":");

	//	m_arryInputData.Add(struData);
	//}
}

void CLoadTrlayout::MakeOutputObject(CString strData)
{
	CString tmps;
	while (!strData.IsEmpty())
	{
		tmps = Parser(strData, _T(","));
		setSymbol(_T("O"), tmps);
	}
	//CString strInput("");
	//CString strName(""), strType(""), strLen("");
	//m_arryOutputData.RemoveAll();
	//while (!strData.IsEmpty())
	//{
	//	strInput = Parser(strData, ",");
	//	if (strInput.GetAt(0) == '$')
	//	{
	//		struGridData struData;
	//		Parser(strInput, ":");
	//		struData.row = atoi(Parser(strInput, ":"));
	//		//struData.strType = Parser(strInput, ":");
	//		//struData.strLen = Parser(strInput, ":");
	//		Parser(strInput, ":");
	//		struData.col = 0;
	//		while (strInput.Find("CHAR") != -1)
	//		{
	//			Parser(strInput, "CHAR");
	//			struData.col++;
	//		}
	//		m_arryGridData.Add(struData);
	//	}
	//	else
	//	{
	//		struOutputData struData;
	//		struData.strName = Parser(strInput, ":");
	//		struData.strType = Parser(strInput, ":");
	//		struData.strLen = Parser(strInput, ":");

	//		m_arryOutputData.Add(struData);
	//	}
	//}
}

CString CLoadTrlayout::Parser(CString &srcstr, CString substr)
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
	return _T("");
}

CString CLoadTrlayout::UTF8ToAnsi(char* pszUTF8)
{
	int	nU8StrLen, nBStrLen, nAStrLen;
	BSTR	bstr;
	CString	str;
	char*	pszANSI;

	nU8StrLen = lstrlen(pszUTF8);
	nBStrLen = MultiByteToWideChar(CP_UTF8, 0, pszUTF8, nU8StrLen + 1, NULL, NULL);
	bstr = SysAllocStringLen(NULL, nBStrLen);
	MultiByteToWideChar(CP_UTF8, 0, pszUTF8, nU8StrLen + 1, bstr, nBStrLen);

	nAStrLen = WideCharToMultiByte(CP_ACP, 0, bstr, -1, NULL, 0, NULL, NULL);
	pszANSI = new char[nAStrLen];
	WideCharToMultiByte(CP_ACP, 0, bstr, -1, pszANSI, nAStrLen, NULL, NULL);
	str = (CString)bstr;
	SysFreeString(bstr);
	delete[] pszANSI;

	return str;
}

void CLoadTrlayout::SetInputData(CString strInputData)
{
	CString tmp;
	//bool bgrid = false;

	//if (!strInputData.IsEmpty())
	//{
	//	if (strInputData.GetAt(0) == '$')
	//	{
	//		bgrid = true;
	//		//bskip = true;
	//		//trsymbol = trsymbol.Mid(1);
	//		//tmp = trsymbol;
	//		//tmps = Parser(tmp, _T(":"));
	//		//gridsymbol = tmps;
	//	}
	//	else if (strInputData.GetAt(0) == '@')
	//	{
	//		bgrid = true;
	//		//bgridDesc = true;
	//		//bskip = true;
	//		//trsymbol = trsymbol.Mid(1);
	//		//tmp = trsymbol;
	//		//tmps = Parser(tmp, _T(":"));
	//		//gridsymbol = tmps;
	//	}
	//}
	while (!strInputData.IsEmpty())
	{
		tmp = Parser(strInputData, _T(","));

		m_in.Add(tmp);
	}
}

void CLoadTrlayout::setSymbol(CString inout, CString trsymbol/*, bool grid, bool gridDesc, CString grids*/)
{
	CString tmp, tmps, gridsymbol;
	CString strData;
	bool	bgrid = false, bgridDesc = false;
	bool	bskip = false;
	bool	bgridch = false;

	if (!trsymbol.IsEmpty())
	{
		if (trsymbol.GetAt(0) == '$')
		{
			bgrid = true;
			bskip = true;
			trsymbol = trsymbol.Mid(1);
			tmp = trsymbol;
			tmps = Parser(tmp, _T(":"));
			gridsymbol = tmps;
		}
		else if (trsymbol.GetAt(0) == '@')
		{
			bgrid = true;
			bgridDesc = true;
			bskip = true;
			trsymbol = trsymbol.Mid(1);
			tmp = trsymbol;
			tmps = Parser(tmp, _T(":"));
			gridsymbol = tmps;
		}

		//if (grid)
		//{
		//	bgrid = true;
		//	if (gridDesc)
		//		bgridDesc = true;
		//	gridsymbol = grids;
		//}
	}


	while (!trsymbol.IsEmpty())
	{
		tmp = Parser(trsymbol, _T(":"));
		if (bgrid && tmp.CompareNoCase(gridsymbol))
		{
			int len = gridsymbol.GetLength() * 2 + 2;
			CString tmpx;
			for (int ii = 0; ii < len; ii++)
				tmpx += _T(" ");
			tmps.Format(_T("%s-%s"), tmpx, tmp);
			tmp = tmps;
			bgridch = true;
		}

		if (bgrid)
		{
			if (bgridch)
				tmps.Format(_T("%s"), tmp);
			else
				tmps.Format(_T("+%s"), tmp);
			//list->AddItem(m_row, col, +tmps);
		}
		else
			//list->AddItem(m_row, col, tmp);

		tmp = Parser(trsymbol, _T(":"));
		//list->AddItem(m_row, col, tmp);

		if (bskip)
		{
			if (bgridDesc)
			{
				tmp = Parser(trsymbol, _T(":"));
				//list->AddItem(m_row, descCol, tmp);
			}
			bskip = false;
			//list->AddItem(m_row, 4, inout);
			continue;
		}

		tmp = Parser(trsymbol, _T(":"));
		//list->AddItem(m_row, col, tmp);

		//list->AddItem(m_row, col, inout);

		if (!bgrid || bgridDesc)
		{
			tmp = Parser(trsymbol, _T(":"));
			//list->AddItem(m_row, descCol, tmp);
		}

		if (bgrid)
		{
			//setSymbol(list, inout, trsymbol, bgrid, bgridDesc, gridsymbol);
			trsymbol = trsymbol.Mid(trsymbol.GetLength());
		}

		m_in.Add(strData);
	}
}

void CLoadTrlayout::setMapH()
{
	CString strData(""), tmps("");
	strData = m_trName;
	strData += '\t';
	for (int ii = 0; ii < m_aryTRLayout.GetCount(); ii++)
	{
		tmps = m_aryTRLayout.GetAt(ii);
		strData += tmps;
	}

	strData += "#^";
	AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_DATASET, 0), (LPARAM)strData.operator LPCSTR());
}

void CLoadTrlayout::AddTrLayout(CString inout, CString strName, CString tmps)
{
	CString tmp, strData(""), strSeq("#"), strObject, strText, strC_GRID, strGridText("");
	CString strN, strFormat, strItem, strSym;
	bool bGridText = false;
	char seqGrid = NULL;

	if (m_strInOutChange.IsEmpty())
		m_strInOutChange = inout;

	if (m_strInOutChange != inout)
	{
		m_strInOutChange = inout;
		strSeq = "*";
	}
	while (!strName.IsEmpty())
	{
		strObject = Parser(strName, _T(","));
		if (tmps.GetAt(0) == '$' || tmps.GetAt(0) == '@')
		{
			seqGrid = tmps.GetAt(0);
			tmps.Delete(0);
			strC_GRID = Parser(tmps, _T(":"));
			Parser(tmps, _T(":"));
			if (seqGrid == '$')
			{
				bGridText = false;
			}
			else
			{
				bGridText = true;
				Parser(tmps, _T(":"));
			}
			
			strN = Parser(strObject, _T("\t"));
			while (!tmps.IsEmpty())
			{
				strSym = Parser(tmps, _T(":"));
				Parser(tmps, _T(":"));
				Parser(tmps, _T(":"));
				if (bGridText) strGridText = Parser(tmps, _T(":"));

				strItem.Format("%s.%s", strN, Parser(strObject, _T("\t")));
				strFormat.Format("%s$%s=%s.%s=%s", strSeq, strItem, strC_GRID, strSym, strGridText);
				strData += strFormat;
				if (!tmps.IsEmpty() && strSeq != "#") strSeq = "#";
			}
		}
		else
		{
			tmp = Parser(tmps, _T(":"));
			Parser(tmps, _T(":"));
			Parser(tmps, _T(":"));
			strText = Parser(tmps, _T(":"));
			strData.Format("%s%s=%s=%s", strSeq, strObject, tmp, strText);
		}
		m_aryTRLayout.Add(strData);
		if (!strName.IsEmpty() && strSeq != "#") strSeq = "#";
	}
}