#include "stdafx.h"
#include "BinaryMngr.h"
#include "../h/seq.H"

CBinaryMngr::CBinaryMngr(void)
{
	m_ObjScpFile;
	InitBuf();
}

CBinaryMngr::~CBinaryMngr(void)
{
	if (!m_ObjScpFile.GetFilePath().IsEmpty()) m_ObjScpFile.Close();
}

void CBinaryMngr::InitBuf()
{
	m_pos = NULL;
	memset(m_buf, 0, BUF_SIZE);
	m_pos = m_buf;
}

void CBinaryMngr::AddBinaryData(void* dst, int size, BYTE seq)
{
	if (seq)
		*m_pos++ = seq;
	char src = 0x00;
	if (memcmp(dst, &src, 1) != 0)
	{
		memcpy(m_pos, dst, size);
		m_pos += size;
	}
}

void CBinaryMngr::AddTRlayoutData(CString dst)
{
	CString strDest(""), tmp(""), tmp2(""), tmp3(""), strData("");
	strDest.Format("%s", dst);
	while (!strDest.IsEmpty())
	{
		strData += (char)SEQ_FORMAT;
		strData += (char)kindLAYOUT;
		strData += (char)SEQ_DATA;
		tmp = parse(strDest, CString(GB_XML_SEQ));

		tmp2 = parse(tmp, "\t");
		strData += tmp2;
		tmp2 = parse(tmp, CString(GB_XML_INOUT));
		tmp2.Delete(0);
		while (!tmp2.IsEmpty())
		{
			tmp3 = parse(tmp2, CString(GB_XML_SYMBOL));
			tmp3 = tmp3.Left(tmp3.ReverseFind('='));
			strData += (char)SEQ_DATA;
			strData += tmp3;
		}
		strData += (char)SEQ_OUTPUT;
		while (!tmp.IsEmpty())
		{
			tmp2 = parse(tmp, CString(GB_XML_SYMBOL));
			tmp2 = tmp2.Left(tmp2.ReverseFind('='));
			strData += tmp2;
			strData += (char)SEQ_DATA;
		}
		strData.Delete(strData.GetLength() - 1);
	}
	int size = strData.GetLength();
	memcpy(m_pos, strData, size);
	m_pos += size;
}

void CBinaryMngr::AddRTSData(CString dst)
{
	CString strDest(""), tmp(""), tmp2(""), strData("");
	strDest.Format("%s", dst);
	while (!strDest.IsEmpty())
	{
		strData += (char)SEQ_FORMAT;
		strData += (char)kindRTS;
		strData += (char)SEQ_DATA;
		tmp = parse(strDest, CString(GB_XML_SEQ));
		tmp2 = parse(tmp, CString(GB_XML_SYMBOL));
		tmp2 = parse(tmp, "|");
		strData += tmp2;
		while (!tmp.IsEmpty())
		{
			tmp2 = parse(tmp, ",");
			strData += (char)SEQ_DATA;
			strData += tmp2;
		}
	}
	int size = strData.GetLength();
	memcpy(m_pos, strData, size);
	m_pos += size;
}

void CBinaryMngr::AddScreenLayoutData(CString dst)
{
	CString strDest(""), tmp(""), tmp2(""), strData("");
	strDest.Format("%s", dst);

	strData += (char)SEQ_FORMAT;
	strData += (char)kindScreen;
	strData += (char)SEQ_DATA;
	strData += parse(strDest, ",");

	while (!strDest.IsEmpty())
	{
		strData += (char)SEQ_DATA;
		strData += parse(strDest, "^");
	}
	int size = strData.GetLength();
	memcpy(m_pos, strData, size);
	m_pos += size;
}

CString CBinaryMngr::parse(CString &src, CString sub)
{
	CString	tmps;
	int	pos = src.Find(sub);

	if (pos == -1)
	{
		tmps = src;
		src = _T("");
		return tmps;
	}

	tmps = src.Left(pos);
	src = src.Mid(pos + sub.GetLength());
	return tmps;
}


int CBinaryMngr::getSize()
{
	return (m_pos - m_buf);
}

BYTE* CBinaryMngr::getBuf()
{
	return m_buf;
}

void CBinaryMngr::AddMapScript(CString strPath, int kind)
{
	CFile file;
	DWORD iSize;
	BYTE* sbuf;
	DWORD	dwRead = 0;

	if (!file.Open(strPath, CFile::modeRead | CFile::typeBinary))
	{
		return;
	}

	iSize = (DWORD)file.GetLength();
	if (iSize <= 0)
	{
		file.Close();
		return ;
	}
	sbuf = new BYTE[iSize];
	dwRead = file.Read(sbuf, iSize);
	file.Close();
	if (iSize != dwRead)
	{
		delete[] sbuf;
		return ;
	}

	*m_pos++ = SEQ_SCRIPT;
	*m_pos++ = kind;
	*m_pos++ = SEQ_DATA;
	memcpy(m_pos, sbuf, iSize);
	m_pos += iSize;

	delete[] sbuf;
}

void CBinaryMngr::AddObjectScript(CString strPath,int& pos, int size, int kind)
{
	DWORD iSize;
	BYTE* sbuf;
	DWORD	dwRead = 0;

	if (size <= 0) return;

	if (m_ObjScpFile.GetFilePath() != strPath)
	{
		if (!m_ObjScpFile.GetFilePath().IsEmpty()) m_ObjScpFile.Close();
		if (!m_ObjScpFile.Open(strPath, CFile::modeRead | CFile::typeBinary))
		{
			return;
		}
	}

	iSize = (DWORD)m_ObjScpFile.GetLength();
	if (iSize <= 0)
	{
		m_ObjScpFile.Close();
		return;
	}
	sbuf = new BYTE[iSize];
	dwRead = m_ObjScpFile.Read(sbuf, iSize);
	if (iSize != dwRead)
	{
		delete[] sbuf;
		return;
	}

	*m_pos++ = SEQ_SCRIPT;
	*m_pos++ = kind;
	*m_pos++ = SEQ_DATA;
	memcpy(m_pos, sbuf + pos, size);
	m_pos += size;
	pos += size;

	delete[] sbuf;
}