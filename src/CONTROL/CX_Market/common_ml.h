#ifndef _COMMON_ML_H
#define _COMMON_ML_H

int	CInt(CString sData);
bool	isNum(char c);
BOOL	isNumber(CString sData);

CString	SplitString(CString& strData, CString strToken);
int	SplitString(CString strData, CString strToken, CStringArray &astrResult, int imaxcnt=100);
int	SplitDivString(CString strData, CString *sList, int idivcnt);

CString	GetFindOptionString(CString sData, CString sKey, CString sEndChar);

CString	SetComma(CString sData, int ifloatlen=2);

#endif