// MakeChildDlg.h: interface for the CMakeChildDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAKECHILDDLG_H__9B1864A9_71D7_4B42_8043_29D0223E5CC3__INCLUDED_)
#define AFX_MAKECHILDDLG_H__9B1864A9_71D7_4B42_8043_29D0223E5CC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <afxtempl.h>

#include "Include_AddIn.h"		// for IGraphInfo

#include "BaseData.h"

class CBaseDialog;
class CGraphSetting;
class CSetting_All_Indicator;
class CMakeChildDlg  
{
public:
	CWnd *m_pTopWnd;
	
	void Initial( IChartOCX *p_pIChartOCX, IGraphInfo *p_pIGraphInfo, CWnd* pParent, CBaseDialog *&pChildDlg, CWnd *pTopWnd = NULL);
	void SetItemText(const CString& strIndicatorName, CButton& First, CButton& second);
	
private:
	void SetDlg_Variables(CBaseData::TABITEMTYPE texttype, CWnd* pParent, CBaseDialog *&pChildDlg);
	void SetDlg_BaseLine(CBaseData::TABITEMTYPE texttype, CWnd* pParent, CBaseDialog *&pChildDlg);
	
	void SetDlg_Price(CBaseData::TABITEMTYPE texttype, IChartOCX *p_pIChartOCX, CWnd* pParent, CBaseDialog *&pChildDlg);
	void SetDlg_Volume(CBaseData::TABITEMTYPE texttype, IChartOCX *p_pIChartOCX, CWnd* pParent, CBaseDialog *&pChildDlg);

	void SetDlg_PriceMA(CBaseData::TABITEMTYPE texttype, IChartOCX *p_pIChartOCX, CWnd* pParent, CBaseDialog *&pChildDlg);
	void SetDlg_VolumeMA(CBaseData::TABITEMTYPE texttype, IChartOCX *p_pIChartOCX, CWnd* pParent, CBaseDialog *&pChildDlg);
	
	// Sang-Woo, Cho		2005 / 11 / 10
	// 일목균형표 추가
	// Begin
	void SetDlg_GlanceBalance( CBaseData::TABITEMTYPE texttype, IChartOCX *p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg);
	// End
	
	// RENKO, P&F Chart
	void SetDlg_Special_Type_A( CBaseData::TABITEMTYPE texttype, IChartOCX *p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg);
	void SetDlg_Special_Type_B( CBaseData::TABITEMTYPE texttype, IChartOCX *p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg);
	void SetDlg_Special_Type_C( CBaseData::TABITEMTYPE texttype, IChartOCX *p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg);
	void SetDlg_Special_Type_D( CBaseData::TABITEMTYPE texttype, IChartOCX *p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg);
	void SetDlg_Special_Type_E( CBaseData::TABITEMTYPE texttype, IChartOCX *p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg);	// Sang-Woo, Cho		2006 / 04 / 19	대기매물 설정 분리
	void SetDlg_Common_Type_A( CBaseData::TABITEMTYPE texttype, IChartOCX *p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg);
	void SetDlg_Common_Type_B( CBaseData::TABITEMTYPE texttype, IChartOCX *p_pIChartOCX, CWnd *pParent, CBaseDialog *&pChildDlg);
};

#endif // !defined(AFX_MAKECHILDDLG_H__9B1864A9_71D7_4B42_8043_29D0223E5CC3__INCLUDED_)
