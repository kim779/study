// CodePage.cpp: implementation of the CCodePage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartLanguageTable.h"
#include "CodePage.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// for IMetaTable
#include "MetaTable.h"									// for CMetaTable

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCodePage::CCodePage( int p_nCodePageID)
{
	m_nCodePageID = p_nCodePageID;
}

CCodePage::~CCodePage()
{
	CString strMetaText;
	LanguageTexts *pLanguageTexts = NULL;
	POSITION psnMeta = m_mapMetaToLanguage.GetStartPosition();
	while( psnMeta)
	{
		m_mapMetaToLanguage.GetNextAssoc( psnMeta, strMetaText, pLanguageTexts);
		if( pLanguageTexts) delete [] pLanguageTexts;
	}
	m_mapMetaToLanguage.RemoveAll();
}

BOOL CCodePage::AddMetaData( long p_lMetaCode, const char *p_szMetaText, const char *p_szOtherMetaText, LanguageTexts *p_pLanguageTexts)
{
	if( !p_szMetaText || !p_szOtherMetaText || !p_pLanguageTexts) return TRUE;
	if( !*p_szMetaText || !*p_szOtherMetaText) return TRUE;

	LanguageTexts *pLanguageTexts = NULL;
	if( m_mapMetaToLanguage.Lookup( p_szMetaText, pLanguageTexts))
	{
		CString strMsg;
		strMsg.Format( "Meta Text Collision Error!\nCode Page : [%d]\nMeta Text : [%s]", m_nCodePageID, p_szMetaText);
		AfxMessageBox( strMsg);
		return FALSE;
	}

	const char *szMetaText = NULL;
	if( m_mapOtherMetaText.Lookup( p_szOtherMetaText, szMetaText))
	{
		CString strMsg;
		strMsg.Format( "Other Meta Text Collision Error!\nCode Page : [%d]\nOther Meta Text : [%s]", m_nCodePageID, p_szOtherMetaText);
		AfxMessageBox( strMsg);
		return FALSE;
	}

	int i = 0;
	for( i = IMetaTable::ENGLISH; i < IMetaTable::LANGUAGE_ID_END; i++)
	{
		if( ( *p_pLanguageTexts)[ i] == NULL) break;

		if( m_mapLanguageToMeta[ i].Lookup( ( *p_pLanguageTexts)[ i], szMetaText))
		{
			CString strLanguageName;
			switch( i)
			{
				case IMetaTable::ENGLISH:	strLanguageName = "English";
											break;
				case IMetaTable::KOREAN:	strLanguageName = "Korean";
											break;
//				case IMetaTable::TAIWAN:	strLanguageName = "Taiwan";
//											break;
			}
			CString strMsg;
			strMsg.Format( "%s Text Collision Error!\nCode Page : [%d]\nText : [%s]", strLanguageName, m_nCodePageID, ( *p_pLanguageTexts)[ i]);
			AfxMessageBox( strMsg);
			return FALSE;
		}
	}

	pLanguageTexts = ( LanguageTexts *)new LanguageTexts;
	memcpy( pLanguageTexts, p_pLanguageTexts, sizeof( LanguageTexts));
	m_mapMetaToLanguage.SetAt( p_szMetaText, pLanguageTexts);

	m_mapOtherMetaText.SetAt( p_szOtherMetaText, p_szMetaText);

	for( i = IMetaTable::ENGLISH; i < IMetaTable::LANGUAGE_ID_END; i++)
	{
		if( ( *p_pLanguageTexts)[ i] == NULL) break;
		m_mapLanguageToMeta[ i].SetAt( ( *p_pLanguageTexts)[ i], p_szMetaText);
	}

	if( IMetaTable::PACKET_NAME <= p_lMetaCode && p_lMetaCode < IMetaTable::META_CODE_END)
		if( CMetaTable::m_szMetaTexts[ p_lMetaCode] == CMetaTable::m_szNullString)
			CMetaTable::m_szMetaTexts[ p_lMetaCode] = p_szMetaText;

	return TRUE;
}

BOOL CCodePage::LoadMetaTable( char *p_szStaticMetaTableBuffer)
{
	if( !p_szStaticMetaTableBuffer) return FALSE;
	if( !*p_szStaticMetaTableBuffer) return FALSE;

	char *szNext = p_szStaticMetaTableBuffer;
	char *szNextLine = NULL;
	char *szComment = NULL;
	char *szNextItem = NULL;
	long lMetaCode = 0;
	char *szMetaText = NULL;
	int nLanguageID = 0;
	char *( szLanguageTexts[ IMetaTable::LANGUAGE_ID_END + 1]);
	char *szOtherMetaText = NULL;
	while( *szNext)
	{
		// 1. Get Line
		szNextLine = strstr( szNext, "\r\n");
		if( szNextLine)
		{
			szNextLine++;
			*szNextLine = '\0';
		}

		// 2. Skip Comment Line
		if( *szNext != '#')
		{
			// 3. Delete Comment
			szComment = strchr( szNext, '#');
			if( szComment) *szComment = '\0';

			// 4. Get Meta Code
			szNextItem = strchr( szNext, ';');
			if( szNextItem)
			{
				*szNextItem = '\0';
				if( *szNext) lMetaCode = atol( szNext);
				else lMetaCode = -1;

				// 5. Get Meta Text
				szNext = szNextItem;
				szNext++;
				szNextItem = strchr( szNext, ';');
				if( szNextItem)
				{
					*szNextItem = '\0';
					szMetaText = szNext;

					szNext = szNextItem;
					szNext++;
					szNextItem = strchr( szNext, ';');
					if( szNextItem)
					{
						*szNextItem = '\0';
						szOtherMetaText = szNext;

						// 6. Get Language Text
						nLanguageID = 0;
						while( szNextItem && nLanguageID < IMetaTable::LANGUAGE_ID_END)
						{
							szNext = szNextItem;
							szNext++;
							szNextItem = strchr( szNext, ';');
							if( !szNextItem) break;

							*szNextItem = '\0';
							szLanguageTexts[ nLanguageID++] = szNext;
						}

						if( 0 < nLanguageID)
						{
							szLanguageTexts[ nLanguageID] = NULL;

							if( !AddMetaData( lMetaCode, szMetaText, szOtherMetaText, ( LanguageTexts *)&szLanguageTexts)) return FALSE;
						}
					}
				}
			}
		}

		szNext = szNextLine;
		if( !szNext) break;
		szNext++;
	}

	return TRUE;
}

BOOL CCodePage::GetMetaTableString( CString &p_strTableString)
{
	int i = 0;
	long lTableStringLen = 0;
	CString strMetaText;
	LanguageTexts *pLanguageTexts = NULL;
	POSITION psnMeta = m_mapMetaToLanguage.GetStartPosition();
	while( psnMeta)
	{
		m_mapMetaToLanguage.GetNextAssoc( psnMeta, strMetaText, pLanguageTexts);
		lTableStringLen++;													// ';'
		lTableStringLen += strMetaText.GetLength();							// MetaText
		lTableStringLen++;													// ';'
		lTableStringLen += strMetaText.GetLength();							// MetaText (for Other MetaText)
		lTableStringLen++;													// ';'
		for( i = IMetaTable::ENGLISH; i < IMetaTable::LANGUAGE_ID_END; i++)
		{
			lTableStringLen += strlen( ( *pLanguageTexts)[ i]);				// Language(n)
			lTableStringLen++;												// ';'
		}
		lTableStringLen += 2;												// '\r\n'
	}

	char *szTableString = p_strTableString.GetBufferSetLength( lTableStringLen);
	psnMeta = m_mapMetaToLanguage.GetStartPosition();
	while( psnMeta)
	{
		m_mapMetaToLanguage.GetNextAssoc( psnMeta, strMetaText, pLanguageTexts);
		*szTableString = ';';												// ';'
		szTableString++;
		strcpy( szTableString, strMetaText);								// MetaText
		szTableString += strMetaText.GetLength();
		*szTableString = ';';												// ';'
		szTableString++;
		strcpy( szTableString, strMetaText);								// MetaText (for Other MetaText)
		szTableString += strMetaText.GetLength();
		*szTableString = ';';												// ';'
		szTableString++;
		for( i = IMetaTable::ENGLISH; i < IMetaTable::LANGUAGE_ID_END; i++)
		{
			strcpy( szTableString, ( *pLanguageTexts)[ i]);					// Language(n)
			szTableString += strlen( ( *pLanguageTexts)[ i]);
			*szTableString = ';';											// ';'
			szTableString++;
		}
		*szTableString = '\r';												// '\r'
		szTableString++;
		*szTableString = '\n';												// '\n'
		szTableString++;
	}

	return TRUE;
}

BOOL CCodePage::SetLocalMetaText( const char *p_szMetaText, IMetaTable::E_LANGUAGEID p_eLanguageID, const char *p_szLanguageText)
{
	if( p_eLanguageID < IMetaTable::ENGLISH || IMetaTable::LANGUAGE_ID_END <= p_eLanguageID) return FALSE;

	const char *szLanguageText = NULL;
	POSITION psnTempText = m_slTempText.AddTail( p_szLanguageText);
	CString &strLanguageText = m_slTempText.GetAt( psnTempText);
	szLanguageText = strLanguageText;

	const char *szMetaText = NULL;
	LanguageTexts *pLanguageTexts = NULL;
	m_mapMetaToLanguage.Lookup( p_szMetaText, pLanguageTexts);
	if( pLanguageTexts)
	{
		if( !m_mapLanguageToMeta[ p_eLanguageID].Lookup( ( *pLanguageTexts)[ p_eLanguageID], szMetaText)) return FALSE;
		
		m_mapLanguageToMeta[ p_eLanguageID].RemoveKey( ( *pLanguageTexts)[ p_eLanguageID]);

		( *pLanguageTexts)[ p_eLanguageID] = szLanguageText;
		m_mapLanguageToMeta[ p_eLanguageID].SetAt( ( *pLanguageTexts)[ p_eLanguageID], szMetaText);
	}
	else
	{
		psnTempText = m_slTempText.AddTail( p_szMetaText);
		CString &strMetaText = m_slTempText.GetAt( psnTempText);
		szMetaText = strMetaText;

		pLanguageTexts = ( LanguageTexts *)new LanguageTexts;
		m_mapMetaToLanguage.SetAt( szMetaText, pLanguageTexts);
		for( int i = IMetaTable::ENGLISH; i < IMetaTable::LANGUAGE_ID_END; i++)
		{
			( *pLanguageTexts)[ i] = szLanguageText;
			m_mapLanguageToMeta[ i].SetAt( ( *pLanguageTexts)[ i], szMetaText);
		}
	}

	return TRUE;
}

const char *CCodePage::GetLanguageText( IMetaTable::E_LANGUAGEID p_eLanguageID, const char *p_szMetaText)
{
	if( p_eLanguageID < IMetaTable::ENGLISH || IMetaTable::LANGUAGE_ID_END <= p_eLanguageID) return NULL;
	
	LanguageTexts *pLanguageTexts = NULL;
	m_mapMetaToLanguage.Lookup( p_szMetaText, pLanguageTexts);
	if( !pLanguageTexts)
	{
		return NULL;
	}
	
	return ( *pLanguageTexts)[ p_eLanguageID];
}

int CCodePage::GetMetaTextCount( void)
{
	return m_mapLanguageToMeta[ 0].GetCount();
}

BOOL CCodePage::GetAllMetaTexts( const char **p_szMetaTexts)
{
	if( !p_szMetaTexts) return FALSE;
	
	int i = 0;
	CString strLanguageText;
	POSITION psn = m_mapLanguageToMeta[ 0].GetStartPosition();
	while( psn) m_mapLanguageToMeta[ 0].GetNextAssoc( psn, strLanguageText, p_szMetaTexts[ i++]);
	return TRUE;
}

// *. Convert LText to MText
//		Caution! p_szLanguageText can be MText already.
//					or p_szLanguageText can be created already.
const char *CCodePage::CreateMetaText( IMetaTable::E_LANGUAGEID p_eLanguageID, const char *p_szLanguageText)
{
	if( p_eLanguageID < IMetaTable::ENGLISH || IMetaTable::LANGUAGE_ID_END <= p_eLanguageID) return NULL;

	POSITION psnTempText = m_slTempText.AddTail( p_szLanguageText);
	CString &strLanguageText = m_slTempText.GetAt( psnTempText);
	const char *szLanguageText = strLanguageText;

	CString strTempMetaText;
	LanguageTexts *pLanguageTexts = NULL;
	const char *szMetaText = p_szLanguageText;

	// (2008/3/27 - Seung-Won, Bae) Do not use isdigit(). (It did not work in some system)
	while( *szMetaText && '0' <= *szMetaText && *szMetaText <= '9') szMetaText++;
	if( !*szMetaText) return p_szLanguageText;

	int nLen = strlen( p_szLanguageText), j = 0;
	char *szTempMetaText = strTempMetaText.GetBuffer( nLen * 2 + 3);
	unsigned char c1 = 0, c2 = 0;
	szTempMetaText[ j++] = '_';
	szTempMetaText[ j++] = 'M';

	int i = 0;
	for( i = 0; i < nLen; i++)
	{
		c1 = p_szLanguageText[ i];
		if( c1 < 0x80) szTempMetaText[ j++]	= c1;
		else
		{
			c2						= '0' + ( c1 >> 4);
			szTempMetaText[ j++]	= ( '9' < c2 ? c2 - '9' - 1 + 'A' : c2);
			c2						= '0' + ( c1 & 0x0F);
			szTempMetaText[ j++]	= ( '9' < c2 ? c2 - '9' - 1 + 'A' : c2);
		}
	}
	szTempMetaText[ j++]		= '_';
	szTempMetaText[ j++]		= '\0';
	strTempMetaText.ReleaseBuffer();

	if( m_mapMetaToLanguage.Lookup( strTempMetaText, pLanguageTexts))
	{
		m_mapLanguageToMeta[ p_eLanguageID].Lookup( ( *pLanguageTexts)[ p_eLanguageID], szMetaText);
		return szMetaText;
	}

	psnTempText = m_slTempText.AddTail( strTempMetaText);
	CString &strMetaText = m_slTempText.GetAt( psnTempText);
	szMetaText = strMetaText;

	pLanguageTexts = ( LanguageTexts *)new LanguageTexts;
	m_mapMetaToLanguage.SetAt( szMetaText, pLanguageTexts);
	for( i = IMetaTable::ENGLISH; i < IMetaTable::LANGUAGE_ID_END; i++)
	{
		( *pLanguageTexts)[ i] = szLanguageText;
		m_mapLanguageToMeta[ i].SetAt( ( *pLanguageTexts)[ i], szMetaText);
	}

	return szMetaText;
}

// *. Convert LText to MText
//		Caution! p_szLanguageText can be MText already.
// (2008/3/25 - Seung-Won, Bae) Use GetReDirectedMetaText()
const char *CCodePage::GetMetaText( IMetaTable::E_LANGUAGEID p_eLanguageID, const char *p_szLanguageText)
{
	if( p_eLanguageID < IMetaTable::ENGLISH || IMetaTable::LANGUAGE_ID_END <= p_eLanguageID) return NULL;

	// (2008/2/5 - Seung-Won, Bae) Check Current Meta First for Meta Conversion.
	LanguageTexts *pLanguageTexts = NULL;
	if( m_mapMetaToLanguage.Lookup( p_szLanguageText, pLanguageTexts))
		p_szLanguageText = ( *pLanguageTexts)[ p_eLanguageID];

	const char *szMetaText = NULL;
	if( !m_mapLanguageToMeta[ p_eLanguageID].Lookup( p_szLanguageText, szMetaText)) szMetaText = NULL;

	if( szMetaText) szMetaText = GetReDirectedMetaText( szMetaText);

	return szMetaText;
}

// *. Convert old MText to new MText
//		Caution! p_szOtherMetaText can be new MText already.
// (2008/3/25 - Seung-Won, Bae) Use GetReDirectedMetaText()
BOOL CCodePage::GetMetaTextFromOther( const char *p_szOtherMetaText, const char *&p_szMetaText)
{
	p_szMetaText = NULL;

	// (2008/2/5 - Seung-Won, Bae) Check Current Meta First for Meta Conversion.
	LanguageTexts *pLanguageTexts = NULL;
	if( m_mapMetaToLanguage.Lookup( p_szOtherMetaText, pLanguageTexts))
		p_szMetaText = p_szOtherMetaText;

	if( !p_szMetaText)
		if( !m_mapOtherMetaText.Lookup( p_szOtherMetaText, p_szMetaText))
			p_szMetaText = NULL;

	if( p_szMetaText) p_szMetaText = GetReDirectedMetaText( p_szMetaText);

	return TRUE;
}

// *. Convert old MText or LText to new MText
//		Caution! p_szOtherMOrLText can be new MText already.
// (2008/3/25 - Seung-Won, Bae) Use GetReDirectedMetaText()
const char *CCodePage::GetMetaTextForScript( IMetaTable::E_LANGUAGEID p_eLanguageID, const char *p_szOtherMOrLText)
{
	if( p_eLanguageID < IMetaTable::ENGLISH || IMetaTable::LANGUAGE_ID_END <= p_eLanguageID) return NULL;

	const char *szMetaText = NULL;
	if( !GetMetaTextFromOther( p_szOtherMOrLText, szMetaText)) szMetaText = NULL;
	
	if( !szMetaText)
		if( !m_mapLanguageToMeta[ p_eLanguageID].Lookup( p_szOtherMOrLText, szMetaText))
			szMetaText = NULL;

	if( szMetaText) szMetaText = GetReDirectedMetaText( szMetaText);

	return szMetaText;
}

// (2008/3/24 - Seung-Won, Bae) Support the re-directed Meta.
const char *CCodePage::GetReDirectedMetaText( const char *p_szMetaText)
{
	CString strMetaText( p_szMetaText);
	int nMetaLength = strMetaText.GetLength();
	if( nMetaLength < 7) return p_szMetaText;				// "_M?_$?_"
	int nCopyIdIndex = strMetaText.Find( "_$");
	if( nCopyIdIndex < 0) return p_szMetaText;
	strMetaText = strMetaText.Left( nCopyIdIndex) + "_";
	const char *szNewMetaText = GetMetaText( IMetaTable::ENGLISH, strMetaText);
	if( szNewMetaText) return szNewMetaText;
	return p_szMetaText;
}