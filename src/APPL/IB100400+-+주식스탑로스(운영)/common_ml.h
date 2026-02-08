#pragma once


	const char		tabCHAR	= 0x09;

	CString			GetString(char *pChar, int nMinLen);
	BOOL			IsStockAcc(CString sAcc);
	CString			makeAccn(CString accnnum, CString accnname);
	CString			Trim(CString sData);
	int				CInt(CString sData);
	float			CFloat(CString sData);
	CString			CStr(int ival);
	CString			CFStr(double lval, int ifloat=0, bool bsign=false);
	bool			isNum(char c);
	BOOL			isNumber(CString sData);



	CString			RemoveSign(CString sData, CString sign, CString sign2=_T(""));
	CString			RemoveToken(CString sData, char cToken);
	BOOL			FileExist(CString sPath);


	CString			RemoveText(CString sData);
	CString			RemoveToken(CString sData, CString sRemove);
	CString			MakeString(CString sData, CString sFillToken, int ilen);


	bool			ExistFile(CString sPath);
	CString			MakeGroupItemPacket(const CString& code, CString amount = _T(""), CString price = _T(""), CString name = _T(""));

	CString			ParsingString(CString &strData, CString strToken);
	CString			SplitString(CString& strData, CString strToken); //
	int				SplitString(CString strData, CString strToken, CString *sList, int imaxcnt=100);
	int				SplitString(CString strData, CString strToken, CStringArray &astrResult, int imaxcnt=100);

	int				SplitDivString(CString strData, CString *sList, int idivcnt);
	int				SplitString_MAP(CString strData, CString strToken, CMapStringToString &arMapList); //
	int				SplitInt(CString strData, CString strToken, int *iList);
	int				SplitInt(CString strData, CString strToken, CWordArray &arList);

	int				SplitDouble(CString strData, CString strToken,	double *iList, bool bsign=true);
	int				SplitDoubleD(CString strData, CString strToken,	double *iList, double ldifval=1);

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
	CString			MakeNumber(CString sData, bool bsign=false);

	CString			SetComma(CString sData, int ifloatlen=2);
	CString			SetDataFormat(int iType, CString sData, CString sToken=_T(""), int ifloatlen=2);




	LOGFONT			GetLogFont(int fsize, bool bold=false, CString sFont="±¼¸²Ã¼");
	void			PaintGradiantRect(CDC *pDC, const RECT &rect,COLORREF clrFrom, COLORREF clrTo =RGB(255,255,255), 
                       BOOL hori = false, BOOL ascend = true);


