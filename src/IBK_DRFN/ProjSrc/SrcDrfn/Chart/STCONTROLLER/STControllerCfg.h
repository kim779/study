// STControllerCfg.h: interface for the CSTControllerCfg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STCONTROLLERCFG_H__5778CC22_3028_466C_8BB2_6B8F3CB40DA0__INCLUDED_)
#define AFX_STCONTROLLERCFG_H__5778CC22_3028_466C_8BB2_6B8F3CB40DA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "STController.h"
#include "STControllerApp.h"
#include "../Include_ST/STConfigureDlgLoader.h"

class CDataItem;
class CMemManager;
class CSTControllerCfg  : public CSTController
{
public:
	CSTControllerCfg(CSTControllerApp *pTheApp);
	virtual ~CSTControllerCfg();
	short Execute(LPCTSTR lpInputData);
	long Stop() {return TRUE;}
	BOOL DrawGraph() {return TRUE;}

	// Quote Data	
	double GetOpenData(double dReverseIndex) {return 0;}
	double GetHighData(double dReverseIndex) {return 0;}
	double GetLowData(double dReverseIndex) {return 0;}
	double GetCloseData(double dReverseIndex) {return 0;}
	double GetVolumeData(double dReverseIndex) {return 0;}
	double GetAmountData(double dReverseIndex) {return 0;}

	// Config
	void SetConfig();
	void SetConfigOnWizard(CRect* pRect);
	void WriteConfig();
	void DeleteConfig();

	BOOL OnReceiveQuoteData();
private:
	CSTConfigureDlgLoader m_STConfigureDlgLoader;
};

#endif // !defined(AFX_STCONTROLLERCFG_H__5778CC22_3028_466C_8BB2_6B8F3CB40DA0__INCLUDED_)
