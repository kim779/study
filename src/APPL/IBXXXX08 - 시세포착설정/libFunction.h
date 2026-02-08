
#pragma once

enum { CK_NOT = 0, CK_KOSPI, CK_KOSDAQ, CK_FUTURE, CK_OPTION };
enum { FK_NOT = 0, FK_COMMA, FK_FLOAT, FK_FLOATx, FK_REMOVE };

CBitmap*	_GetAxBitmap(CWnd* m_pView, CString path);
CBrush*		_GetAxBrush(CWnd* m_pView, COLORREF clr);
COLORREF	_GetIndexColor(CWnd* m_pView, int index);
CString		_GetString(char* datB, int datL);
CString		_GetFormatData(int fkind, CString str);
