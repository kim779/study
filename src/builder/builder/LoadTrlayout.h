#pragma once
#include "StdAfx.h"

struct struInputData
{
	CString strName;
	CString strType;
	CString strLen;
};

struct struOutputData
{
	CString strName;
	CString strType;
	CString strLen;
};

struct struGridData
{
	int row;
	int col;
};

class CLoadTrlayout
{
public:
	CLoadTrlayout();
	~CLoadTrlayout();
public:
	void loadTrLayoutFile(CString path);
	void MakeInputObject(CString strData);
	void MakeOutputObject(CString strData);
	CString	Parser(CString &srcstr, CString substr);
	CString UTF8ToAnsi(char* pszUTF8);
	void	setSymbol(CString inout, CString trsymbol/*, bool grid = false, bool desc = false, CString grids = _T("")*/);
	void setMapH();

	void AddTrLayout(CString inout, CString strName, CString tmps);

	void CLoadTrlayout::SetInputData(CString strInputData);
public:
	//CArray<struInputData, struInputData> m_arryInputData;
	//CArray<struOutputData, struOutputData> m_arryOutputData;
	//CArray<struGridData, struGridData> m_arryGridData;

	CArray	<CString, CString> m_in;
	CArray	<CString, CString> m_out;
	CMapStringToString m_trMap;

	CArray	<CString, CString> m_aryTRLayout;
	CString m_strInOutChange;
	CString m_trName;
};