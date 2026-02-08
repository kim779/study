// PrintController.h: interface for the CPrintController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTCONTROLLER_H__91641AAF_625F_4493_8426_A291AE7E2FBE__INCLUDED_)
#define AFX_PRINTCONTROLLER_H__91641AAF_625F_4493_8426_A291AE7E2FBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKTBChartCtrl;
class CPrintController  
{
public:
	void OnPrint(const CRect& rcChartRegion,
				 CKTBChartCtrl* pCtrl);

private:
	void PopUpOsAndMyPrintDlg(const CRect& rcChartRegion,
							  CKTBChartCtrl* pCtrl);
	void PopUpOsPrintDlg(const CRect& rcChartRegion,
						 CKTBChartCtrl* pCtrl);
	void PopUpMyPrintDlg(const CRect& rcChartRegion,
						 CKTBChartCtrl* pCtrl);

	void OnDraw(CDC* pdc, 
				const int nPrintColor, 
				const CRect& rcDestination,
				const CRect& rcChartRegion,
				CKTBChartCtrl* pCtrl);

	void OnPrintbyUserSetDataCountperOnePge(CDC &dcPrinter,
										  const int nPrintColor,
										  const CRect& rcChartRegion,
										  CKTBChartCtrl* pCtrl);
	void OnPrintTotalData(CDC &dcPrinter, 
						  const int nPrintColor,
						  const CRect& rcChartRegion,
						  CKTBChartCtrl* pCtrl);
	void OnPrintCurrentPage(CDC &dcPrinter, 
							const int nPrintColor,
							const CRect& rcChartRegion,
							CKTBChartCtrl* pCtrl);

};

#endif // !defined(AFX_PRINTCONTROLLER_H__91641AAF_625F_4493_8426_A291AE7E2FBE__INCLUDED_)
