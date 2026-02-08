// ExcelDataController.h: interface for the CExcelDataController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCELDATACONTROLLER_H__89E1DC2C_9C35_466B_8DA9_4DF80F2A7991__INCLUDED_)
#define AFX_EXCELDATACONTROLLER_H__89E1DC2C_9C35_466B_8DA9_4DF80F2A7991__INCLUDED_

#include <afxtempl.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/BlockBaseEnum.h"		// for CChartInfo::CHART_MODE

class CPacketList;
class CPacket;
class CExcelDataController  
{
public:
	CExcelDataController( HWND p_hOcxWnd, CChartInfo::CHART_MODE p_eChartMode);
	virtual ~CExcelDataController();

	void CallExcel(const CString& strTitles,
									const CString& strPacketNames,
									const CString& strPacketNamesPerBlock,
									const CString& strHorzScale,
									const CString& strFilePath,
									const BOOL bRun,
									CPacketList* pPacketList);
private:
	void WriteAllData(const CPoint& ptRowAndCol,
  					  CFile* file);
	void WriteColumnHeader(const CString& strTitle,
						   CFile* file);
	void WriteExcel(const CPoint& ptRowAndCol,
					const CString& strTitle,
					const CString& strFilePath,
					const BOOL bRun);

	void GetAllData(const CString& strTitles,
					const CString& strPacketNames,
					CPacketList* pPakcetList);

	void GetHorzScaleData(const CString& strTitles,
						const CString& strPacketNames,
						const CString& strHorzScale,
						CPacket* pPacketheader,
						CPacketList* pPakcetList);
	CString GetAllTitleData(const int nTitleCount,
							const CString& szTitles,
							const CString& strPacketNamesPerBlock);
	CString GetHorzScaleTitleData(CPacket* pPacketheader);
	BOOL IsAllZeroData(CList<double, double>* datalist);
	CPoint GetRowandColumnCount(const CString& strTitles,
									const CString& strPacketNames,
									const CString& strHorzScale,
									CPacketList* pPacketList);

	CString GetStrValue(HKEY Hkey,CString strkey, CString strsubkey,CString strRValue);
	POSITION FindPosition(const int nIndex) const;

	CList<CString, CString> m_RowDataList;

// (2008/5/14 - Seung-Won, Bae) for Multi-Language of Special Date or Price Unit Text
protected:
	HWND						m_hOcxWnd;

// (2009/9/15 - Seung-Won, Bae) Do not support special time for WorldOn.
protected:
	CChartInfo::CHART_MODE		m_eChartMode;

// (2008/11/27 - Seung-Won, Bae) for Normalized Minute Time Rule candle.
protected:
	int		m_nHeaderDataCount;
};

#endif // !defined(AFX_EXCELDATACONTROLLER_H__89E1DC2C_9C35_466B_8DA9_4DF80F2A7991__INCLUDED_)
