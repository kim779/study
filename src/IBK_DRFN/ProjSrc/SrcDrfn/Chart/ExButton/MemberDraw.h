// MemberDraw.h: interface for the CMemberDraw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBERDRAW_H__B0F05FF4_6E99_44CA_8487_01349F69CE7C__INCLUDED_)
#define AFX_MEMBERDRAW_H__B0F05FF4_6E99_44CA_8487_01349F69CE7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../inc/_COMMON/Member.h"

class CMemberDraw : public CMember  
{
public:
	CMemberDraw(LPVOID lpControl, ISkinColorManager* pISkinColorManager, BOOL bUseDefSkin);
	virtual ~CMemberDraw();

	void		SetDefaultColor();
	void		SetAccentColor();
	void		SetTabColor();
	void		SetBuyColor();
	void		SetSellColor();
	void		SetModifyColor();
	void		SetCancelColor();
	
	void		SetSkinColor();

	void		UseToggleMode(BOOL bUse) {m_bUseToggleMode = bUse;}
	BOOL		GetToggleState() {return m_bPushed;}
	void		SetToggleState(BOOL bPushed) {m_bPushed = bPushed;}
	
/*
	void		SetMemberIndex(UINT nOutLine, UINT nOutLineTopLeft, UINT nInLineTopLeft, UINT nInLineBottomRight, UINT nOutLineBottomRight, UINT nBackground, UINT nText
							, UINT nOutLineDis, UINT nOutLineTopLeftDis, UINT nInLineTopLeftDis, UINT nInLineBottomRightDis, UINT nOutLineBottomRightDis, UINT nBackgroundDis, UINT nTextDis);
	void		SetMemberDraw(LPCTSTR lpszText, COLORREF clrOutLine, COLORREF clrOutLineTopLeft, COLORREF clrInLineTopLeft, COLORREF clrInLineBottomRight, COLORREF clrOutLineBottomRight, COLORREF clrBackground, COLORREF clrText
							, COLORREF clrOutLineDis, COLORREF clrOutLineTopLeftDis, COLORREF clrInLineTopLeftDis, COLORREF clrInLineBottomRightDis, COLORREF clrOutLineBottomRightDis, COLORREF clrBackgroundDis, COLORREF clrTextDis);
*/

	void		SetText(LPCTSTR lpszText);
	LPCTSTR		GetText();

	FpChangeSkinNotify	GetNotifySkinTableFunction();
	
	LPSTR		m_lpszText;

	COLORREF	m_clrOutLine;				// 바깥 테두리
	COLORREF	m_clrOutLineTopLeft;		// 좌측 상단 바깥 라인
	COLORREF	m_clrInLineTopLeft;			// 좌측 상단 안쪽 라인
	COLORREF	m_clrInLineBottomRight;		// 우측 안쪽 라인
	COLORREF	m_clrOutLineBottomRight;	// 우측 바깥 라인
	COLORREF	m_clrBackground;			// 바탕
	COLORREF	m_clrText;					// 기본글꼴색

	COLORREF	m_clrOutLineOver;			// 바깥 테두리
	COLORREF	m_clrOutLineTopLeftOver;	// 좌측 상단 바깥 라인
	COLORREF	m_clrInLineTopLeftOver;		// 좌측 상단 안쪽 라인
	COLORREF	m_clrInLineBottomRightOver;	// 우측 안쪽 라인
	COLORREF	m_clrOutLineBottomRightOver;// 우측 바깥 라인
	COLORREF	m_clrBackgroundOver;		// 바탕
	COLORREF	m_clrTextOver;				// 기본글꼴색

	COLORREF	m_clrOutLineDown;			// 바깥 테두리
	COLORREF	m_clrOutLineTopLeftDown;	// 좌측 상단 바깥 라인
	COLORREF	m_clrInLineTopLeftDown;		// 좌측 상단 안쪽 라인
	COLORREF	m_clrInLineBottomRightDown;	// 우측 안쪽 라인
	COLORREF	m_clrOutLineBottomRightDown;// 우측 바깥 라인
	COLORREF	m_clrBackgroundDown;		// 바탕
	COLORREF	m_clrTextDown;				// 기본글꼴색

	COLORREF	m_clrOutLineDis;			// 바깥 테두리
	COLORREF	m_clrOutLineTopLeftDis;		// 좌측 상단 바깥 라인
	COLORREF	m_clrInLineTopLeftDis;		// 좌측 상단 안쪽 라인
	COLORREF	m_clrInLineBottomRightDis;	// 우측 안쪽 라인
	COLORREF	m_clrOutLineBottomRightDis;	// 우측 바깥 라인
	COLORREF	m_clrBackgroundDis;			// 바탕
	COLORREF	m_clrTextDis;				// 기본글꼴색

	UINT		m_nOutLine;
	UINT		m_nOutLineTopLeft;
	UINT		m_nInLineTopLeft;
	UINT		m_nInLineBottomRight;
	UINT		m_nOutLineBottomRight;
	UINT		m_nBackground;
	UINT		m_nText;

	UINT		m_nOutLineOver;
	UINT		m_nOutLineTopLeftOver;
	UINT		m_nInLineTopLeftOver;
	UINT		m_nInLineBottomRightOver;
	UINT		m_nOutLineBottomRightOver;
	UINT		m_nBackgroundOver;
	UINT		m_nTextOver;

	UINT		m_nOutLineDown;
	UINT		m_nOutLineTopLeftDown;
	UINT		m_nInLineTopLeftDown;
	UINT		m_nInLineBottomRightDown;
	UINT		m_nOutLineBottomRightDown;
	UINT		m_nBackgroundDown;
	UINT		m_nTextDown;

	UINT		m_nOutLineDis;
	UINT		m_nOutLineTopLeftDis;
	UINT		m_nInLineTopLeftDis;
	UINT		m_nInLineBottomRightDis;
	UINT		m_nOutLineBottomRightDis;
	UINT		m_nBackgroundDis;
	UINT		m_nTextDis;

    HICON    m_hIcon;
    HICON    m_hIconPushed;
    CSize    m_sizeImage;

	UINT	m_nIconAlign;
	
	BOOL	m_bUseToggleMode;
	BOOL	m_bPushed;

	CList<LPVOID, LPVOID>	m_listToggleGroup;

protected:
    class CButton_Friendly : public CButton
    {
		friend class CExButtonApp; 
    };

public:
	CButton_Friendly*		m_pButton;
};

#endif // !defined(AFX_MEMBERDRAW_H__B0F05FF4_6E99_44CA_8487_01349F69CE7C__INCLUDED_)
