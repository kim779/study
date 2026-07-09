// token.cpp: implementation of the CCompile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "libwcc.h"
#include "compile.h"

int CCompile::mapGlobalTokenParsing(CMapToken* token, int line, WORD *type)
{
	CString	sP;
	CString	tmp;
	int	tN;		

	tN = 0;
	sP    = m_text[line].text;

	m_tokFlag = 0;
	for (int ii = 0; ii < sP.GetLength(); ii++)
	{
		switch (sP.GetAt(ii))
		{
		case ' ':
		case '\t':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += sP.GetAt(ii);
				break;
			}

			if (m_tokFlag & TOKF_HOS)
			{
				if (tmp.GetLength())
					tmp += sP.GetAt(ii);
			}
			break;

		case '(':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += sP.GetAt(ii);
				break;
			}

			if (m_tokFlag & TOKF_HOS)
			{
				setMessage(line, 8);
				return(-1);
			}
			else
				m_tokFlag |= TOKF_HOS;

			if (tmp.GetLength())
			{
				token[tN].name = tmp;
				token[tN].len = tmp.GetLength();
				tmp.Empty(); tN++;
			}
			break;

		case ')':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp  += sP.GetAt(ii);
				break;
			}

			if (m_tokFlag & TOKF_HOE)
			{
				setMessage(line, 9);
				return(-1);
			}
			else if (m_tokFlag & TOKF_HOS)
			{
				m_tokFlag &= ~TOKF_HOS;
				m_tokFlag &= ~TOKF_HOE;

				if (tmp.GetLength())
				{
					token[tN].name = tmp;
					token[tN].len = tmp.GetLength();
					tmp.Empty(); tN++;
				}
			}
			else
			{
				m_tokFlag |= TOKF_HOE;
			}
			break;

		case '\"':
			if (!(m_tokFlag & TOKF_DQS))
			{
				m_tokFlag |= TOKF_DQS;
				break;
			}

			m_tokFlag &= ~TOKF_DQS;

			token[tN].name = tmp;
			token[tN].len = tmp.GetLength();
			token[tN].type = TOK_FILE;
			tmp.Empty(); tN++;
			break;

		case ',':
		case '|':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += sP.GetAt(ii);
				break;
			}

			if ((m_tokFlag & TOKF_HOS) && tmp.GetLength())
			{
				token[tN].name = tmp;
				token[tN].len = tmp.GetLength();
				tmp.Empty(); tN++;
			}
			break;

		default:
			tmp += sP.GetAt(ii);
			break;
		}
	}

	if (m_tokFlag & TOKF_MSK)
	{
		setMessage(line, 7);
		return(-1);
	}
	*type = TYPE_GLOBAL;
	return(tN);
}

int CCompile::mapFormTokenParsing(CMapToken* token, int line, WORD *type)
{
	CString	sP;
	CString tmp = "";
	int	tN;

	tN = 0;
	sP    = m_text[line].text;

	m_tokFlag = 0;
	for (int ii = 0; ii < sP.GetLength(); ii++)
	{
		switch (sP.GetAt(ii))
		{
		case ' ':
		case '\t':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += sP.GetAt(ii);
				break;
			}

			if (!tmp.IsEmpty())
			{
				token[tN].name = tmp;
				token[tN].len = tmp.GetLength();
				token[tN].type = TOK_NONE;
				tmp.Empty(); tN++;
			}
			break;

		case '(':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += sP.GetAt(ii);
				break;
			}

			if (m_tokFlag & TOKF_HOS)
			{
				setMessage(line, 8);
				return(-1);
			}
			else
				m_tokFlag |= TOKF_HOS;

			if (!tmp.IsEmpty())
			{
				token[tN].name = tmp;
				token[tN].len = tmp.GetLength();
				token[tN].type = TOK_NONE;
				tmp.Empty(); tN++;
			}
			break;

		case ')':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += sP.GetAt(ii);
				break;
			}

			if (m_tokFlag & TOKF_HOE)
			{
				setMessage(line, 9);
				return(-1);
			}
			else if (m_tokFlag & TOKF_HOS)
			{
				m_tokFlag &= ~TOKF_HOS;
				m_tokFlag &= ~TOKF_HOE;

				if (!tmp.IsEmpty())
				{
					token[tN].name = tmp;
					token[tN].len  = tmp.GetLength();
					token[tN].type = TOK_NUM;
					tmp.Empty(); tN++;
				}
			}
			else
				m_tokFlag |= TOKF_HOE;
			break;

		case '|':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += sP.GetAt(ii);
				break;
			}

			if (m_tokFlag & TOKF_HOS)
				token[tN].type = TOK_NUM;
			else
				token[tN].type = TOK_NONE;

			if (!tmp.IsEmpty())
			{
				token[tN].name = tmp;
				token[tN].len = tmp.GetLength();
				tmp.Empty(); tN++;
			}
			break;

		case ',':
			if ((m_tokFlag & TOKF_QUS) || (m_tokFlag & TOKF_DQS))
			{
				tmp += sP.GetAt(ii);
				break;
			}

			if (m_tokFlag & TOKF_HOS)
				token[tN].type = TOK_NUM;
			else
				token[tN].type = TOK_NONE;

			if (!tmp.IsEmpty())
			{
				token[tN].name = tmp;
				token[tN].len = tmp.GetLength();
				tmp.Empty(); tN++;
			}
			break;

		case '\'':				// 0x27
			if (!(m_tokFlag & TOKF_QUS))
			{
				m_tokFlag |= TOKF_QUS;
				break;
			}

			m_tokFlag &= ~TOKF_QUS;

			if (!tmp.IsEmpty())
			{
				token[tN].name = tmp;
				token[tN].len = tmp.GetLength();
				token[tN].type = TOK_TEXT;
				tmp.Empty(); tN++;
			}
			else
				setMessage(line, 11);
			break;

		case '\"':				// 0x22
			if (!(m_tokFlag & TOKF_DQS))
			{
				m_tokFlag |= TOKF_DQS;
				break;
			}

			m_tokFlag &= ~TOKF_DQS;

			token[tN].name = tmp;
			token[tN].len = tmp.GetLength();
			token[tN].type = TOK_FILE;
			tmp.Empty(); tN++;
			break;

		default:
			tmp += sP.GetAt(ii);
			break;
		}
	}

	if (!tmp.IsEmpty())
	{
		token[tN].name = tmp;
		token[tN].len = tmp.GetLength();
		token[tN].type = TOK_NONE;
		tN++;
	}

	*type = m_text[line].type;
	return(tN);
}

int CCompile::mapResourceTokenParsing(CMapToken* token, int line, WORD *type)
{
	CString tmp;
	CString	str;
	int	tN;
	int	lineL;

	tN = 0;
	str    = CString(m_text[line].text);
	str.TrimRight();
	lineL = strlen(m_text[line].text);

	m_paren   = 0;
	m_tokFlag = 0;
	
	if (str.Find("_AW_SCRIPT_AW_(") == 0)
	{
		token[tN].name = "_AW_SCRIPT_AW_";
		token[tN].len = 14;
		token[tN].type = TOK_OPER;
		tN++;
		
		str = str.Mid(15, str.Find(")") - 15);
		token[tN].name = str;
		token[tN].len = str.GetLength();
		token[tN].type = TOK_NONE;
		m_tokFlag = 0;
		return (tN);
	}
	
	token[tN].name = str;
	token[tN].len = str.GetLength();
	token[tN++].type = TOK_NONE;

	*type = TYPE_RESOURCE;
	return(tN);
}
