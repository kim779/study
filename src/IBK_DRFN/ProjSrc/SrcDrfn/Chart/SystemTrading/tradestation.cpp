#include "stdafx.h"
#include "SystemTradingView.h"
#include "SystemTradingDoc.h"
#include "MainFrm.h"
#include "../Include_ST/DeclareDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static BOOL IsStsKeyword(LPCTSTR pszChars, int nLength)
{
	LPSTR szUpperChar = ::strdup(pszChars);
	::strupr(szUpperChar);
	
	for (int L = 0; s_apszStsKeywordList[L] != NULL; L ++)
	{
		if (strncmp(s_apszStsKeywordList[L], szUpperChar, nLength) == 0
				&& s_apszStsKeywordList[L][nLength] == 0)
		{
			::free(szUpperChar);
			return TRUE;
		}
	}

	::free(szUpperChar);
	return FALSE;
}

static BOOL IsStsNumber(LPCTSTR pszChars, int nLength)
{
	if (nLength > 2 && pszChars[0] == '0' && pszChars[1] == 'x')
	{
		for (int I = 2; I < nLength; I++)
		{
			if (isdigit(pszChars[I]) || (pszChars[I] >= 'A' && pszChars[I] <= 'F') ||
										(pszChars[I] >= 'a' && pszChars[I] <= 'f'))
				continue;
			return FALSE;
		}
		return TRUE;
	}
	if (::IsDBCSLeadByte(pszChars[0]) || !isdigit(pszChars[0]))
		return FALSE;
	for (int I = 1; I < nLength; I++)
	{
		if (! isdigit(pszChars[I]) && pszChars[I] != '+' &&
			pszChars[I] != '-' && pszChars[I] != '.' && pszChars[I] != 'e' &&
			pszChars[I] != 'E')
			return FALSE;
	}
	return TRUE;
}

#define DEFINE_BLOCK(pos, colorindex)	\
	ASSERT((pos) >= 0 && (pos) <= nLength);\
	if (pBuf != NULL)\
	{\
		if (nActualItems == 0 || pBuf[nActualItems - 1].m_nCharPos <= (pos)){\
		pBuf[nActualItems].m_nCharPos = (pos);\
		pBuf[nActualItems].m_nColorIndex = (colorindex);\
		nActualItems ++;}\
	}

#define COOKIE_COMMENT			0x0001
#define COOKIE_PREPROCESSOR		0x0002
#define COOKIE_EXT_COMMENT		0x0004
#define COOKIE_STRING			0x0008
#define COOKIE_CHAR				0x0010

DWORD CSystemTradingView::ParseLineSTS(DWORD dwCookie, int nLineIndex, TEXTBLOCK *pBuf, int &nActualItems)
{
	int nLength = GetLineLength(nLineIndex);
	if (nLength <= 0)
		return dwCookie & COOKIE_EXT_COMMENT;

	LPCTSTR pszChars    = GetLineChars(nLineIndex);
	BOOL bFirstChar     = (dwCookie & ~COOKIE_EXT_COMMENT) == 0;
	BOOL bRedefineBlock = TRUE;
	BOOL bDecIndex  = FALSE;
	int nIdentBegin = -1;
	int I = 0;
	for ( I = 0; ; I++)
	{
		if (bRedefineBlock)
		{
			int nPos = I;
			if (bDecIndex)
			{
				nPos--;
				// ojtaso 추가
				// 한글 예약어 컬러를 위한 추가작업
				if(::IsDBCSLeadByte(pszChars[nPos]) == TRUE && nPos >= 0)
					nPos--;
			}
			if (dwCookie & (COOKIE_COMMENT | COOKIE_EXT_COMMENT))
			{
				DEFINE_BLOCK(nPos, COLORINDEX_COMMENT);
			}
			else
			if (dwCookie & (COOKIE_CHAR | COOKIE_STRING))
			{
				DEFINE_BLOCK(nPos, COLORINDEX_STRING);
			}
			else
			if (dwCookie & COOKIE_PREPROCESSOR)
			{
				DEFINE_BLOCK(nPos, COLORINDEX_PREPROCESSOR);
			}
			else
			{
				DEFINE_BLOCK(nPos, COLORINDEX_NORMALTEXT);
			}
			bRedefineBlock = FALSE;
			bDecIndex      = FALSE;
		}

		if (I == nLength)
			break;

		if(::IsDBCSLeadByte(pszChars[I]) == TRUE)
		{
			// ojtaso 추가
			// 한글 예약어 컬러를 위한 추가작업
			if(nIdentBegin == -1)
				nIdentBegin = I;

			if(nIdentBegin >= 0)
			{
				if (IsStsKeyword(pszChars + nIdentBegin, I - nIdentBegin))
				{
					DEFINE_BLOCK(nIdentBegin, COLORINDEX_KEYWORD);
					bRedefineBlock = TRUE;
					bDecIndex = TRUE;
				}
				else
				{
					UINT nFileType = ((CSystemTradingDoc*)GetDocument())->GetParseFileType();
					if(nFileType == FT_STRATEGY || nFileType == FT_USERFUNCTION)
					{
						CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
						CSystemTradingDoc* pDoc = (CSystemTradingDoc*)GetDocument();
						if(pMainFrame->IsInnerFunctionKeyword(const_cast<LPSTR>(pszChars) + nIdentBegin, I - nIdentBegin, pDoc->GetScriptType()))
						{
							DEFINE_BLOCK(nIdentBegin, COLORINDEX_KEYWORD);
							bRedefineBlock = TRUE;
							bDecIndex = TRUE;
						}
						else
						{
							int nFunc = pMainFrame->IsUserFunctionKeyword(const_cast<LPSTR>(pszChars) + nIdentBegin, I - nIdentBegin);
							if(nFunc == 1)
							{
								DEFINE_BLOCK(nIdentBegin, COLORINDEX_STSUSERFUNC);
								bRedefineBlock = TRUE;
								bDecIndex = TRUE;
							}
							else if(nFunc == -1)
							{
								DEFINE_BLOCK(nIdentBegin, COLORINDEX_STSUSERFUNC_NOTCERT);
								bRedefineBlock = TRUE;
								bDecIndex = TRUE;
							}
						}
					}
				}
			}
			
			bFirstChar = FALSE;
			I++;

			continue;
		}

		if (dwCookie & COOKIE_COMMENT)
		{
			DEFINE_BLOCK(I, COLORINDEX_COMMENT);
			dwCookie |= COOKIE_COMMENT;
			break;
		}

		//	String constant "...."
		if (dwCookie & COOKIE_STRING)
		{
			if (pszChars[I] == '"' && (I == 0 || pszChars[I - 1] != '\\'))
			{
				dwCookie &= ~COOKIE_STRING;
				bRedefineBlock = TRUE;
			}
			continue;
		}

		//	Char constant '..'
		if (dwCookie & COOKIE_CHAR)
		{
			if (pszChars[I] == '\'' && (I == 0 || pszChars[I - 1] != '\\'))
			{
				dwCookie &= ~COOKIE_CHAR;
				bRedefineBlock = TRUE;
			}
			continue;
		}

		//	Extended comment /*....*/
		if (dwCookie & COOKIE_EXT_COMMENT)
		{
			if (I > 0 && pszChars[I] == '/' && pszChars[I - 1] == '*')
			{
				dwCookie &= ~COOKIE_EXT_COMMENT;
				bRedefineBlock = TRUE;
			}
			else if (I > 0 && pszChars[I] == ')' && pszChars[I - 1] == '*')
			{
				dwCookie &= ~COOKIE_EXT_COMMENT;
				bRedefineBlock = TRUE;
			}
			else if (pszChars[I] == '}')
			{
				dwCookie &= ~COOKIE_EXT_COMMENT;
				bRedefineBlock = TRUE;
			}
			continue;
		}

		if (I > 0 && pszChars[I] == '/' && pszChars[I - 1] == '/')
		{
			DEFINE_BLOCK(I - 1, COLORINDEX_COMMENT);
			dwCookie |= COOKIE_COMMENT;
			break;
		}

		//	Preprocessor directive #....
		if (dwCookie & COOKIE_PREPROCESSOR)
		{
			if (I > 0 && pszChars[I] == '*' && pszChars[I - 1] == '/')
			{
				DEFINE_BLOCK(I - 1, COLORINDEX_COMMENT);
				dwCookie |= COOKIE_EXT_COMMENT;
			}
			else if (I > 0 && pszChars[I] == '*' && pszChars[I - 1] == '(')
			{
				DEFINE_BLOCK(I - 1, COLORINDEX_COMMENT);
				dwCookie |= COOKIE_EXT_COMMENT;
			}
			else if (pszChars[I] == '{')
			{
				DEFINE_BLOCK(I, COLORINDEX_COMMENT);
				dwCookie |= COOKIE_EXT_COMMENT;
			}
			continue;
		}

		//	Normal text
		if (pszChars[I] == '"')
		{
			DEFINE_BLOCK(I, COLORINDEX_STRING);
			dwCookie |= COOKIE_STRING;
			continue;
		}
		if (pszChars[I] == '\'')
		{
			DEFINE_BLOCK(I, COLORINDEX_STRING);
			dwCookie |= COOKIE_CHAR;
			continue;
		}
		if (I > 0 && pszChars[I] == '*' && pszChars[I - 1] == '/')
		{
			DEFINE_BLOCK(I - 1, COLORINDEX_COMMENT);
			dwCookie |= COOKIE_EXT_COMMENT;
			continue;
		}
		if (I > 0 && pszChars[I] == '*' && pszChars[I - 1] == '(')
		{
			DEFINE_BLOCK(I - 1, COLORINDEX_COMMENT);
			dwCookie |= COOKIE_EXT_COMMENT;
			continue;
		}
		if (pszChars[I] == '{')
		{
			DEFINE_BLOCK(I, COLORINDEX_COMMENT);
			dwCookie |= COOKIE_EXT_COMMENT;
			continue;
		}

		if (bFirstChar)
		{
			if (pszChars[I] == '#')
			{
				DEFINE_BLOCK(I, COLORINDEX_PREPROCESSOR);
				dwCookie |= COOKIE_PREPROCESSOR;
				continue;
			}
			if (! isspace(pszChars[I]))
				bFirstChar = FALSE;
		}

		if (pBuf == NULL)
			continue;	//	We don't need to extract keywords,
						//	for faster parsing skip the rest of loop

		if (isalnum(pszChars[I]) || pszChars[I] == '_' || pszChars[I] == '.')
		{
			if (nIdentBegin == -1)
				nIdentBegin = I;
		}
		else
		{
			if (nIdentBegin >= 0)
			{
				if (IsStsKeyword(pszChars + nIdentBegin, I - nIdentBegin))
				{
					DEFINE_BLOCK(nIdentBegin, COLORINDEX_KEYWORD);
				}
				else if (IsStsNumber(pszChars + nIdentBegin, I - nIdentBegin))
				{
					DEFINE_BLOCK(nIdentBegin, COLORINDEX_NUMBER);
				}
				else
				{
					UINT nFileType = ((CSystemTradingDoc*)GetDocument())->GetParseFileType();
					if(nFileType == FT_STRATEGY || nFileType == FT_USERFUNCTION)
					{
						CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
						CSystemTradingDoc* pDoc = (CSystemTradingDoc*)GetDocument();
						if(pMainFrame->IsInnerFunctionKeyword(const_cast<LPSTR>(pszChars) + nIdentBegin, I - nIdentBegin, pDoc->GetScriptType()))
						{
							DEFINE_BLOCK(nIdentBegin, COLORINDEX_KEYWORD);
						}
						else
						{
							int nFunc = pMainFrame->IsUserFunctionKeyword(const_cast<LPSTR>(pszChars) + nIdentBegin, I - nIdentBegin);
							if(nFunc == 1)
							{
								DEFINE_BLOCK(nIdentBegin, COLORINDEX_STSUSERFUNC);
							}
							else if(nFunc == -1)
							{
								DEFINE_BLOCK(nIdentBegin, COLORINDEX_STSUSERFUNC_NOTCERT);
							}
						}
					}
				}

				bRedefineBlock = TRUE;
				bDecIndex = TRUE;
				nIdentBegin = -1;
			}
		}
	}

	if (nIdentBegin >= 0)
	{
		if (IsStsKeyword(pszChars + nIdentBegin, I - nIdentBegin))
		{
			DEFINE_BLOCK(nIdentBegin, COLORINDEX_KEYWORD);
		}
		else if (IsStsNumber(pszChars + nIdentBegin, I - nIdentBegin))
		{
			DEFINE_BLOCK(nIdentBegin, COLORINDEX_NUMBER);
		}
		else
		{
			UINT nFileType = ((CSystemTradingDoc*)GetDocument())->GetParseFileType();
			if(nFileType == FT_STRATEGY || nFileType == FT_USERFUNCTION)
			{
				CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
				CSystemTradingDoc* pDoc = (CSystemTradingDoc*)GetDocument();
				if(pMainFrame->IsInnerFunctionKeyword(const_cast<LPSTR>(pszChars) + nIdentBegin, I - nIdentBegin, pDoc->GetScriptType()))
				{
					DEFINE_BLOCK(nIdentBegin, COLORINDEX_KEYWORD);
				}
				else
				{
					int nFunc = pMainFrame->IsUserFunctionKeyword(const_cast<LPSTR>(pszChars) + nIdentBegin, I - nIdentBegin);
					if(nFunc == 1)
					{
						DEFINE_BLOCK(nIdentBegin, COLORINDEX_STSUSERFUNC);
					}
					else if(nFunc == -1)
					{
						DEFINE_BLOCK(nIdentBegin, COLORINDEX_STSUSERFUNC_NOTCERT);
					}
				}
			}
		}
	}

	if (pszChars[nLength - 1] != '\\')
		dwCookie &= COOKIE_EXT_COMMENT;
	return dwCookie;
}
