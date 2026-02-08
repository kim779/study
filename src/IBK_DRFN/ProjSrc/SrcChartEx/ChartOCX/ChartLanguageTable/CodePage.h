// CodePage.h: interface for the CCodePage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODEPAGE_H__BC43A001_9E8F_426F_AE5E_67C609716B97__INCLUDED_)
#define AFX_CODEPAGE_H__BC43A001_9E8F_426F_AE5E_67C609716B97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>										// for CTypedPtrMap
#include "../Include_Chart/Dll_Load/IMetaTable.h"			// for IMetaTable

typedef const char *( LanguageTexts[ IMetaTable::LANGUAGE_ID_END]);
typedef CTypedPtrMap < CMapStringToPtr, CString, const char *> CMapLanguageToMetaBase;

class CMapLanguageToMeta : public CMapLanguageToMetaBase
{
public:
	void	SetAt( CString strKey, const char *p_szValue)
			{	CMapStringToPtr::SetAt( strKey, ( void *)p_szValue);	}
};

class CCodePage  
{
public:
	CCodePage( int p_nCodePageID);
	virtual ~CCodePage();

protected:
	int															m_nCodePageID;
	CTypedPtrMap < CMapStringToPtr, CString, LanguageTexts *>	m_mapMetaToLanguage;
	CMapLanguageToMeta											m_mapLanguageToMeta[ IMetaTable::LANGUAGE_ID_END];
	CStringList													m_slTempText;
	CMapLanguageToMeta											m_mapOtherMetaText;
protected:
	BOOL		AddMetaData( long p_lMetaCode, const char *p_szMetaText, const char *p_szOtherMetaText, LanguageTexts *p_pLanguageTexts);

public:
	BOOL			LoadMetaTable(			char *p_szStaticMetaTableBuffer);
	BOOL			GetMetaTableString(		CString &p_strTableString);
	BOOL			SetLocalMetaText(		const char *p_szMetaText, IMetaTable::E_LANGUAGEID p_eLanguageID, const char *p_szLanguageText);
	const char *	GetLanguageText(		IMetaTable::E_LANGUAGEID p_eLanguageID, const char *p_szMetaText);
	int				GetMetaTextCount(		void);
	BOOL			GetAllMetaTexts(	const	char **p_szMetaTexts);

public:
	// *. Convert LText to MText
	//		Caution! p_szLanguageText can be MText already.
	//					or p_szLanguageText can be created already.
	const char *	CreateMetaText(			IMetaTable::E_LANGUAGEID p_eLanguageID, const char *p_szLanguageText);

	// *. Convert LText to MText
	//		Caution! p_szLanguageText can be MText already.
	const char *	GetMetaText(			IMetaTable::E_LANGUAGEID p_eLanguageID, const char *p_szLanguageText);
	
	// *. Convert old MText to new MText
	//		Caution! p_szOtherMetaText can be new MText already.
	BOOL			GetMetaTextFromOther(	const char *p_szOtherMetaText, const char *&p_szMetaText);
	
	// *. Convert old MText or LText to new MText
	//		Caution! p_szOtherMOrLText can be new MText already.
	const char *	GetMetaTextForScript(	IMetaTable::E_LANGUAGEID p_eLanguageID, const char *p_szOtherMOrLText);

	// (2008/3/24 - Seung-Won, Bae) Support the re-directed Meta.
	const char *	GetReDirectedMetaText( const char *p_szMetaText);
};

#endif // !defined(AFX_CODEPAGE_H__BC43A001_9E8F_426F_AE5E_67C609716B97__INCLUDED_)
