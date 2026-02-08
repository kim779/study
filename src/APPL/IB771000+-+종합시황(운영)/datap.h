#pragma once

	const char		tabCHAR	= 0x09;

	CString			GetString(char *pChar, int nMinLen);
	BOOL			IsStockAcc(CString sAcc);
	CString			makeAccn(CString accnnum, CString accnname);
	CString			Trim(CString sData);
	int				CInt(CString sData);
	CString			CStr(int ival);
	bool			isNum(char c);
	BOOL			isNumber(CString sData);



	CString			RemoveSign(CString sData, CString sign, CString sign2=_T(""));
	BOOL			FileExist(CString sPath);


	CString			RemoveText(CString sData);
	CString			MakeString(CString sData, CString sFillToken, int ilen);


	bool			ExistFile(CString sPath);
	CString			MakeGroupItemPacket(const CString& code, CString amount = _T(""), CString price = _T(""), CString name = _T(""));

	CString			ParsingString(CString &strData, CString strToken);
	CString			SplitString(CString& strData, CString strToken); //
	int				SplitString(CString strData, CString strToken, CString *sList, int imaxcnt=100);
	int				SplitString(CString strData, CString strToken, CStringArray &astrResult, int imaxcnt=100);

	int				SplitString_MAP(CString strData, CString strToken, CMapStringToString &arMapList); //
	int				SplitInt(CString strData, CString strToken, int *iList);
	int				SplitInt(CString strData, CString strToken, CWordArray &arList);

	int				SplitFloat(CString strData, CString strToken,	double *iList, bool bsign=true);
	CString			GetFindString(CString sData, CString sKey, CString sDefault=_T(""));
	CString			GetFindOptionString(CString sData, CString sKey, CString sEndChar);


	
	int				_httoi(const TCHAR *value);

	CString			GetConfigFileString(CString strFile, CString strSection, CString strKey, CString strDefault=_T(""));
	COLORREF		GetConfigFileColor(CString strFile, CString strSection, CString strKey, CString strDefault=_T(""));
	int				GetConfigFileInt(CString strFile, CString strSection, CString strKey, int idefault);
	int				GetProfileKeyList(CString strFile, LPCTSTR  strAppName, CStringArray& arList);

	void			WriteConfigFileInt(CString strFile, CString strSection, CString strKey, int ival);
	void			WriteConfigFileUInt(CString strFile, CString strSection, CString strKey, UINT nval);

	CString			ResourceIDLoadString(UINT nID);
	CSize			GetTextSize(CDC* pDC, CString sText);
	CString			filltoken(CString strData, int ilen, CString strToken, int itype=0/*0right, 1-left*/);

//#endif