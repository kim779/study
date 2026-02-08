
#pragma once

class CMapDlg;

//CMapDlg* g_MapDlgPtr= NULL;

//CMapDlg* GetMapDlg();
CString Parser(CString &srcstr, CString substr);
void AddComma(char* pcData);
LOGFONT GetSimpleFont(int fsize, DWORD weight);
CFont* GetAxFont(int size ,bool bBold,char* name);


#define IDC_CODE 1000
#define _USE_MEMDC
//#define _CUSTOM_DRAW
