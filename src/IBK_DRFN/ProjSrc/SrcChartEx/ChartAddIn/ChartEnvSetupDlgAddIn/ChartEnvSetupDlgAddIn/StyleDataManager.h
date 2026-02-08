// StyleDataManager.h: interface for the CStyleDataManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STYLEDATAMANAGER_H__7EA9338C_1E62_49BC_B6BD_93B74B5CFA94__INCLUDED_)
#define AFX_STYLEDATAMANAGER_H__7EA9338C_1E62_49BC_B6BD_93B74B5CFA94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Include_AddIn.h"		// for IChartOCX

class CStyleData;
class CStyleDataManager : public CObject  
{
public:
	CStyleDataManager();
	virtual ~CStyleDataManager();

protected:
	static CList<CStyleData*, CStyleData*> m_lstStyleDefault;
	static CList<CStyleData*, CStyleData*> m_lstStyleUser;
public:
	static void		RemoveAll( void);
	// (2007/5/2 - Seung-Won, Bae) Analysis Color Notify Skip Flag
	// (2007/7/3 - Seung-Won, Bae) Vertical & Horizontal Scale Grid Type Notify Skip Flag
	static void		SetStyleDataToOCX( CStyleData &p_styleData, IChartOCX *p_pIChartOCX, BOOL p_bResetSaveCFG = TRUE,
						BOOL p_bSkipAnalysisColor = FALSE, BOOL p_bSkipVerticalScaleGrid = FALSE, BOOL p_bSkipHorizontalScaleGrid = FALSE);		// p_bResetSaveCFG for Skin.
	static void		SetStyleDataToIndex( CStyleData &p_styleData, IChartOCX *p_pIChartOCX, const char *p_szGraphName);
public:
	int									Initialize( IChartOCX *p_pIChartOCX, BOOL p_bDefaultData = FALSE);
	int									GetDataCount( BOOL p_bDefaultData = FALSE);
	int									WriteStyleFile( IChartOCX *p_pIChartOCX);
	BOOL								RemoveStyleData( const char *p_szStyleName, BOOL p_bDefaultData = FALSE);
	BOOL								CopyStyleDataListTo( CList< CStyleData *, CStyleData * > &p_lstStyleDataList, BOOL p_bDefaultData = FALSE);
	BOOL								CopyStyleDataListFrom( CList< CStyleData *, CStyleData * > &p_lstStyleDataList, BOOL p_bDefaultData = FALSE);
	void								AddStyleData( CStyleData *p_pStyleData, BOOL p_bDefaultData = FALSE);
	CStyleData *						GetStyleData( int p_nIndex, BOOL p_bDefaultData = FALSE);
	CStyleData *						GetStyleData( const char *p_szStyleName, BOOL p_bDefaultData = FALSE);
protected:	// Not Used.
	BOOL								RemoveStyleDataIn( const int p_nIndex, BOOL p_bDefaultData = FALSE);
	CList<CStyleData*, CStyleData*> *	GetStyleDataList( BOOL p_bDefaultData = FALSE);

public:
	void	CreateDummyUserStyle( IChartOCX *p_pIChartOCX);
	void	RemoveDummyUserStyle( void);
	void	GetStyleDataFromOCX( CStyleData &p_styleData, IChartOCX *p_pIChartOCX, BOOL p_bWithIndex = TRUE);

public:
	void	DrawSampleImage(	CDC *pDC, const CRect p_rctArea, CStyleData *p_pStyleData, CChartInfo::CHART_MODE p_eChartMode, CFont *p_pTitleFont, HWND p_hOcxWnd, int p_nDrawCount);
	void	DrawScale(			CDC *pDC, CRect rect1, CRect rect2, CRect rect3, CStyleData *p_pStyleData, CFont *p_pTitleFont, CChartInfo::CHART_MODE p_eChartMode, double p_dFactor);
	void	DrawGraph(			CDC *pDC, CRect rect1, CRect rect2, CStyleData *p_pStyleData, CChartInfo::CHART_MODE p_eChartMode);
	void	DrawGraphGrid(		CDC *pDC, CRect rect1, CRect rect2, CStyleData *p_pStyleData);
	void	DrawScaleGrid(		CDC *pDC, CRect rect1, CRect rect2, CStyleData *p_pStyleData, CChartInfo::CHART_MODE p_eChartMode, double p_dFactor);
	void	DrawTitle(			CDC *pDC, CRect rect1, CRect rect2, CStyleData *p_pStyleData, CFont *p_pTitleFont, HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
	void	DrawBong(			CDC *pDC, CRect rect, int (*price)[4], COLORREF upColor, COLORREF downColor, COLORREF equalColor, BOOL bUpFill, BOOL bDownFill, int p_nDrawCount);
	void	DrawBar(			CDC *pDC, CRect rect, int *trade, COLORREF upColor, COLORREF downColor, COLORREF equalColor, BOOL bUpFill, BOOL bDownFill, int p_nDrawCount);
	void	DrawLine(			CDC *pDC, CRect rect, int *ave, COLORREF color, int p_nDrawCount);
	void	DrawRect(			CDC* pDC, CRect rect);
};

#endif // !defined(AFX_STYLEDATAMANAGER_H__7EA9338C_1E62_49BC_B6BD_93B74B5CFA94__INCLUDED_)
