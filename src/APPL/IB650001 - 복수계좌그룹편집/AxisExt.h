#if !defined(AFX_AXISEXT_H__B65C2E7A_1A7A_4871_A94D_49D7BC0EF414__INCLUDED_)
#define AFX_AXISEXT_H__B65C2E7A_1A7A_4871_A94D_49D7BC0EF414__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include "common.h"
#include "../../h/axisfire.h"
#include "../../h/ledger.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxStatic.h"

class CfxEditEx : public CfxEdit {
public:
	CfxEditEx(CFont *pFont=NULL) : CfxEdit(pFont) {}
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message==WM_NCPAINT)
		{
			CRect rc;
			GetClientRect(&rc);
			rc.DeflateRect(CSize(-1,-1));
			CDC *pDC = GetDC();
			pDC->FillSolidRect(rc, RGB(0x7d, 0x7d, 0x7d));
			ReleaseDC(pDC);
			return 0;
		}
		return CEdit::WindowProc(message, wParam, lParam);
	}
};

class CfxStaticEx : public CfxStatic {
public:
	CfxStaticEx(CFont *pFont=NULL) : CfxStatic(pFont) {}
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message==WM_NCPAINT)
		{
			CRect rc;
			GetClientRect(&rc);
			rc.DeflateRect(CSize(-1,-1));
			CDC *pDC = GetDC();
			pDC->FillSolidRect(rc, RGB(0x7d, 0x7d, 0x7d));
			ReleaseDC(pDC);
			return 0;
		}
		return CfxStatic::WindowProc(message, wParam, lParam);
	}
};

#define S2I(x)	(Str2Int(x, sizeof(x)))
#define S2D(x)	(Str2Double(x, sizeof(x)))
#define F2S(x)	(CString(x, sizeof(x)))

class CAxisExt
{
public:
	CAxisExt(CWnd *pParent);
	void AxInit();

protected:
	CWnd *m_pParent;
	CFont *m_pFont, *m_pFontB;
	COLORREF m_crMinus, m_crPlus, m_crNormal, m_crBk, m_crBodyBk, m_crFocus, m_crWhite, m_crLine, m_crMadoBk, m_crMasuBk, m_crBlack;
	CString m_usid;

public:
	int      Variant(int cmd, int data);
	LPCTSTR  Variant(int cmd, LPCTSTR data);
	CFont*   GetAxFont(LPCTSTR name, int point, bool italic, int bold);
	CBitmap* GetAxBitmap(LPCTSTR path);
	HBITMAP  GetAxHBitmap(LPCTSTR path);
	COLORREF GetIndexColor(int index);
	CPen*    GetAxPen(COLORREF clr, int width, int style);
	CBrush*  GetAxBrush(COLORREF clr);
	BOOL     SendTR(LPCSTR name, BYTE type, LPCSTR data, int dlen, int key);
	void     SetView(LPCSTR option);
	void     SetModal(LPCSTR option);
	void     SetTrigger(LPCSTR data);
	struct _ledgerH* GetLedger(struct _ledgerH *ledger);

	int      Str2Int(LPCSTR data, int dlen);
	int      Str2Int(CString &data) { return Str2Int((LPCSTR)data, data.GetLength()); }
	double   Str2Double(LPCSTR data, int dlen);
	double   Str2Double(CString &data) { return Str2Double((LPCSTR)data, data.GetLength()); }

	void     ParseString(LPCSTR st, LPCSTR ed, CHAR del, vector<StringProxy> &retval);
	void     ParseString(StringProxy &sp, CHAR del, vector<StringProxy> &retval);
public:
	virtual ~CAxisExt();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISEXT_H__B65C2E7A_1A7A_4871_A94D_49D7BC0EF414__INCLUDED_)
