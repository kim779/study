#pragma once

#include "../../h/axisfire.h"

class CAxisExt
{
public:
	CAxisExt(CWnd *pParent);

protected:
	CWnd *m_pParent;

public:
	int      Variant(int cmd, int data);
	LPCTSTR  Variant(int cmd, LPCTSTR data);
	CFont*   GetAxFont(LPCTSTR name, int point, bool italic, int bold);
	CBitmap* GetAxBitmap(LPCTSTR path);
	HBITMAP  GetAxHBitmap(LPCTSTR path);
	COLORREF GetIndexColor(int index);
	CPen*    GetAxPen(COLORREF clr, int width, int style);
	CBrush*  GetAxBrush(COLORREF clr);
	BOOL     SendTR(LPCSTR name, byte type, LPCSTR data, int dlen, int key);
public:
	virtual ~CAxisExt();
};

