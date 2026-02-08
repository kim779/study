// MemberDraw.h: interface for the CMemberDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBERDRAW_H__B770805C_FB4D_4814_8AFD_FE7F7ECE0D21__INCLUDED_)
#define AFX_MEMBERDRAW_H__B770805C_FB4D_4814_8AFD_FE7F7ECE0D21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../inc/_COMMON/Member.h"
//#include "LBSpinButtonCtrl.h"

#define ITEM_DEFLATE 1
#define HINT_ACTIVATE_RIGHT_SIDE 1
#define HINT_ACTIVE_ITEM 2
#define FLOAT_ITEM_HEIGHT_SHIFT 3
#define ACTIVE_ITEM_HEIGHT_SHIFT 2
#define SHIFT_FROM_CUT_TO_SPIN 4

typedef struct {
	COLORREF clrTabSelText;
	COLORREF clrTabNorText;
	COLORREF clrTabBkgrnd;
	COLORREF clrTabSelGDStt;
	COLORREF clrTabSelGDEnd;
	COLORREF clrTabNorGDStt;
	COLORREF clrTabNorGDEnd;
} STTabColor;

class CMemberDraw : public CMember  
{
public:
	CMemberDraw(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin);
	virtual ~CMemberDraw();

	void SetDefualtSkin();
	void SetSkinColor();

	void AddForeCtrl(CWnd* pCtrl);
	void RemoveForeCtrl(CWnd* pCtrl);
	void SetTabColor(COLORREF clrBkgrnd, COLORREF clrActive, COLORREF clrInactive);
	void SetSTTabColor(long lSTTabColor);

	COLORREF m_clrNormal;
	COLORREF m_clrText;
	COLORREF m_clrSelect;
	COLORREF m_clrLine;
	COLORREF m_clrBackground;

	BOOL m_bGradation;
	COLORREF m_clrTabSelText;
	COLORREF m_clrTabNorText;
	COLORREF m_clrTabBkgrnd;
	COLORREF m_clrTabSelGDStt;
	COLORREF m_clrTabSelGDEnd;
	COLORREF m_clrTabNorGDStt;
	COLORREF m_clrTabNorGDEnd;

	UINT m_nNormal;
	UINT m_nText;
	UINT m_nSelect;
	UINT m_nLine;
	UINT m_nBackground;

	bool m_bFloatPaint;
	bool m_bOverTab;
	int m_nActiveItem;
	short m_nPrevPointedItem;
//	CLBSpinButtonCtrl m_Spin;
	CRect m_rctPage;
	CRect m_rectUpDn;
	HPEN m_hPenShadow;
	HPEN m_hPenLight;
	HPEN m_hPenItemShadow;
	HFONT m_hMenuFont;
	HFONT m_hBoldFont;
	bool m_bAutoRaising;
	bool m_bHasFocus;
	
	BOOL m_bInvalidatePage;
	CList<CWnd*, CWnd*>	m_listForgCtrl;

protected:
    class CTabCtrl_Friendly : public CTabCtrl
    {
		friend class CExTabCtrlApp; 
    };

public:
	CTabCtrl_Friendly*		m_pTabCtrl;
};

#endif // !defined(AFX_MEMBERDRAW_H__B770805C_FB4D_4814_8AFD_FE7F7ECE0D21__INCLUDED_)
