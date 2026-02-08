// ChartCtrl.h: interface for the CChartCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTCTRL_H__73F56DA4_A289_4CE8_BA01_F6F2E83DF866__INCLUDED_)
#define AFX_CHARTCTRL_H__73F56DA4_A289_4CE8_BA01_F6F2E83DF866__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for IMetaTable

class CCodePage;
class CMetaTable;
class CChartCtrl : public CObject
{
public:
	CChartCtrl( HWND p_hOcxWnd, IMetaTable::E_LANGUAGEID p_eLanguageID);
	virtual ~CChartCtrl();

protected:
	HWND							m_hOcxWnd;
	IMetaTable::E_LANGUAGEID		m_eLanguageID;
	BOOL							m_bDesignMode;
	CCodePage *						m_pLocalCodePage;
	CString							m_strLocalLanguageTable;
	CString							m_strLocalLanguageTableRetrieveBuffer;
public:
	HWND							GetOcxHwnd( void)		{	return m_hOcxWnd;	}

public:
	void						CreateLocalLanguageTable( const char *p_szLocalLanguageTable, BOOL p_bDesignMode);
	const char *				GetLocalLanguageTable(	void);
	IMetaTable::E_LANGUAGEID	GetLanguageID( void)	{	return m_eLanguageID;	}
	const char *				GetLanguageText(		IMetaTable::E_CODEPAGE p_eCodePage, const char *p_szMetaText);

public:
	// *. Convert LText to MText
	//		Caution! p_szLanguageText can be MText already.
	//					or p_szLanguageText can be created already.
	const char *				CreateLocalMetaText(	const char *p_szLanguageText);

	// *. Convert LText to MText
	//		Caution! p_szMetaText can be created already or not.
	BOOL						SetLocalMetaText(		const char *p_szMetaText, const char *p_szLanguageText);

	// *. Convert LText to MText
	//		Caution! p_szLanguageText can be MText already.
	const char *				GetMetaText(			IMetaTable::E_CODEPAGE p_eCodePage, const char *p_szLanguageText);

	// *. Convert LText to MText
	//		Caution! p_szOtherMOrLText can be MText already.
	const char *				GetMetaTextForScript(	IMetaTable::E_CODEPAGE p_eCodePage, const char *p_szOtherMOrLText);
};

#endif // !defined(AFX_CHARTCTRL_H__73F56DA4_A289_4CE8_BA01_F6F2E83DF866__INCLUDED_)
