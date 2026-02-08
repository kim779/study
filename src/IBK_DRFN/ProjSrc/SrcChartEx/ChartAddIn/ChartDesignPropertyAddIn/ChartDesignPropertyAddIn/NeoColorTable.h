#ifndef _NEOCOLOR_TABLE
#define _NEOCOLOR_TABLE

extern COLORREF RGB_Color_Table[];


class CColorTableMgr
{
public:
	CColorTableMgr();
	~CColorTableMgr();
	static COLORREF GetColor(int nIndex);
	static int GetColorIndex(COLORREF color);
	static CString GetColorText(int nIndex);
};


#endif //_NEOCOLOR_TABLE