// misc.cpp: implementation of the CCompile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "libwcc.h"
#include "Compile.h"
#include "misc.hxx"
#include "../mainvar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CCompile::SetMapFilePath(CString& dir, char* fileN, bool source)
{
	if (source)
		dir += "\\src\\";
	else
		dir += "\\obj\\";

	dir += fileN;
}

void CCompile::SetTempPath(CString& dir, char* fileN, bool source)
{
	dir  = m_curdir;

	dir += "\\";
	dir += fileN;
	if (!source)
		dir += ".ax";
}

#define	CELL_INDICATOR	"COLINFO"
int CCompile::mapColumnAnalyze(int tLine)
{
	int	offs, cellN = 0;
	CString	tmps;
	
	for (int ii = 0; ii < tLine; ii++)
	{
		if (m_text[ii].type != TYPE_FORM)
			continue;
 
		if (!strlen(m_text[ii].text))
			continue;
	
		tmps = CString(m_text[ii].text);
		for (int jj = 0; jj < CELLSYNTAX; jj++)
		{
			if (tmps.Find(cellSyntax[jj].token) == 0)
			{
				tmps = tmps.Mid(strlen(cellSyntax[jj].token));
				for (int kk = 0; ; kk++)
				{
					offs = tmps.Find(CELL_INDICATOR);
					if (offs == -1)
						break;
					cellN++;
					tmps = tmps.Mid(offs);
					tmps = tmps.Mid(strlen(CELL_INDICATOR));
				}
			}
		}
	}

	if (mapColMemAlloc(cellN) < 0)
	{
		setMessage(-1, 4);
		mapMemoryFree();
		return(-1);
	}

	return 0;
}

int CCompile::mapSourceFileLoad(char* szPath, CWnd* hwnd, CString srcData)
{
	HANDLE	hFile;
	BOOL	fBool, bResource = false;
	DWORD	Offsets, NumOfBytes;
	int	NumOfLines;
	char	*szName, *text, *next;
	int	verNew, conLine;
	WORD	fType;
	int	tLine, pLine;

	if (srcData.IsEmpty())
	{
		szName = strrchr((char *)szPath, '\\');
		szName++;
		
		CopyMemory(m_srcN, szName, L_MAPN);
		m_srcN[L_MAPN] = 0x00;
	}
	else
		strcpy_s(m_srcN, "BookMark");

	if (m_status)
	{
		delete m_status;
		m_status = NULL;
	}

	if (hwnd != NULL && hwnd != (CWnd*)1)
	{
		m_status = new CStatus(m_srcN, hwnd);
		m_status->Create(CStatus::IDD);
		m_status->ShowWindow(SW_SHOW);
	}

	if (srcData.IsEmpty())
	{
		hFile = CreateFile((char *)szPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return -1;
		
		NumOfBytes = GetFileSize(hFile, &Offsets);
		if (NumOfBytes == 0xffffffff || Offsets)
		{
			CloseHandle(hFile);
			return -1;
		}
	}
	else
		NumOfBytes = srcData.GetLength();

	if (m_textB)
		delete[] m_textB;
	if (m_textB2)
		delete[] m_textB2;

	m_textB = new char[NumOfBytes * 2 + 1];
	m_textB[NumOfBytes] = '\0';
	m_strR = &m_textB[NumOfBytes+1];

	m_textB2 = new char[NumOfBytes + 1];
	m_textB2[NumOfBytes] = '\0';
	m_scriptR = &m_textB2[0];

	if (srcData.IsEmpty())
	{
		fBool = ReadFile(hFile, m_textB, NumOfBytes, &Offsets, NULL);
		CloseHandle(hFile);
		if (!fBool || NumOfBytes != Offsets)
			return -1;
		ConvertString(&m_textB[0], NumOfBytes);
	}
	else
		CopyMemory(m_textB, srcData.operator LPCTSTR(), NumOfBytes);

	NumOfLines = 0;
	for (int ii = 0; ii < (int)NumOfBytes; ii++)
	{
		switch (m_textB[ii])
		{
		case '\t':
			if (hwnd == (CWnd *)NULL)
				break;
		case '\r':
		case '\0':
			m_textB[ii] = ' ';
			break;
		case '\n':
			NumOfLines++;
			m_textB[ii] = '\0';
			break;
		default:
			break;
		}
	}

	if (NumOfLines >= MAX_TEXT_LINE)
		return -1;

	for (int ii = 0; ii < NumOfLines; ii++)
	{
		if (m_text[ii].text)
		{
			m_text[ii].text = NULL;
			m_text[ii].type = TYPE_NOTDEF;
		}
	}

	next   = m_textB;
	pLine  = 0;
	verNew = -1; conLine = 0;

	int ii = 0;
	for (tLine = 0; tLine < NumOfLines; tLine++)
	{
		text = next;
		for (ii = 0; ii < (int)strlen(text); ii++)
		{
			switch (text[ii])
			{
			case ' ':
			case '\t':
				continue;
			case '/':
				if (hwnd != (CWnd *)NULL)
				{
					if (text[ii+1] != '/')
						break;

					FillMemory(&text[ii], strlen(text)-ii, ' ');
					continue;
				}
			default:
				break;
			}
			break;
		}
		next = &text[strlen(text)+1];

		char* rawText = text;	// preserve original line (with leading whitespace) so Python indentation survives

		if (hwnd != (CWnd *)NULL)
		{
			if (ii >= (int)strlen(text))
				continue;			// skip blank line
			text = &text[ii];
		}

		if (verNew == -1)
			verNew = (*text == '&') ? 0 : 1;

		if (!verNew)
		{
			conLine = 0;
			if (*text == '&')
			{
				text++;
				rawText = text;
			}
			else if (m_text[pLine].text)
				conLine = 1;
		}

		if (conLine)
		{
			conLine = strlen(m_text[pLine].text);
			m_text[pLine].text[conLine] = ' ';
			CopyMemory(&m_text[pLine].text[conLine + 1], text, strlen(text) + 1);
		}
		else
		{
			pLine = tLine;
			m_text[pLine].text = rawText;
		}

		if (strncmp(m_text[pLine].text, "RESOURCE DIVISION", 17) == 0)
			bResource = true;

		conLine = 0;
		for (ii = strlen(m_text[pLine].text)-1; ii >= 0; ii--)
		{
			switch (m_text[pLine].text[ii])
			{
			case '&':
				if (bResource)
					break;
				conLine = 1;
				m_text[pLine].text[ii] = '\0';
				break;
			case ' ':
				if (hwnd != (CWnd *)NULL)
				{
					m_text[pLine].text[ii] = '\0';
					continue;
				}
				else if (ii == 0)
					break;
				continue;
			default:
				break;
			}
			break;
		}
	}

	fType = TYPE_NOTDEF;
	for (int ii = 0; ii < tLine; ii++)
	{
		if (!m_text[ii].text || !strlen(m_text[ii].text))
			continue;

		for (int jj = 0; jj < MAPSYNTAX; jj++)
		{
			if (fType & TYPE_BEGIN)
				m_text[ii].type = (fType & TYPE_SYNMSK);

			CString sText = CString(m_text[ii].text, strlen(m_text[ii].text));
			sText.TrimLeft();

			if (CompareString(LOCALE_SYSTEM_DEFAULT, 0,
				/*(char *)m_text[ii].text*/sText, strlen(mapSyntax[jj].token),
				(char *)mapSyntax[jj].token, strlen(mapSyntax[jj].token)) == 2)
			{
				if (mapSyntax[jj].flag & TYPE_COMMENT)
				{
					m_text[ii].type = TYPE_COMMENT;
					break;
				}

				if (mapSyntax[jj].flag & TYPE_BEGIN)
				{
					if (fType & TYPE_BEGIN)
					{
						m_text[ii].type = TYPE_NOTDEF;
						setMessage(ii, 6);
					}
				}

				if (mapSyntax[jj].flag & TYPE_END)
				{
					if (fType == TYPE_NOTDEF)
					{
						m_text[ii].type = TYPE_NOTDEF;
						setMessage(ii, 19);
					}
					fType = TYPE_NOTDEF;
					m_text[ii].type = fType;
					break;
				}

				fType |= mapSyntax[jj].flag;
				break;
			}
		}		
	}
	return (tLine);
}

int CCompile::mapBuildObjectFile(char *szPath, CString nickN)
{
	HANDLE		hFile;
	OFSTRUCT	oFile;	
	char		szName[16];
	BOOL		wRc;
	DWORD		cbWriteLen;
	DWORD		sOffs;

	if (m_status)
		m_status->ShowBitmap(2, true);

	if (m_err)
		return(-1);

	if (!nickN.IsEmpty())
		CopyMemory(m_mapH.mapN, (char *)nickN.operator LPCTSTR(), L_MAPN);
	else
	{
		CopyMemory(szName, m_mapH.mapN, L_MAPN);
		szName[L_MAPN] = 0x00;
	}

	oFile.cBytes = sizeof(OFSTRUCT);
	oFile.fFixedDisk = 1;
	strcpy_s(oFile.szPathName, szPath);

	hFile = (HANDLE)OpenFile(szPath, &oFile, OF_CREATE|OF_SHARE_COMPAT|OF_SHARE_EXCLUSIVE);
	if (hFile == (HANDLE)HFILE_ERROR)
	{
		setMessage(-1, 14);
		return(-1);
	}
	
	m_mapH.formN = m_fN;

	sOffs = sizeof(struct _mapH);
	m_mapH.formR = sOffs;
	
	sOffs += sizeof(struct _formR) * m_mapH.formN;
	m_mapH.cellR = sOffs;

	sOffs += sizeof(struct _cellR) * (m_repN+1);
	m_mapH.pageR = sOffs;

	sOffs += sizeof(struct _pageR) * m_pageN; // page °¹¼ö maybe tab °ü·Ã
	m_mapH.valueR = sOffs;

	sOffs += sizeof(DWORD) * m_valueN; // value offset
	m_mapH.scriptR = sOffs;

	sOffs += m_scriptN; // script size
	m_mapH.strR = sOffs;

	{
		CString dbg;
		dbg.Format("[AWWCC][DEBUG] mapBuildObjectFile: m_scriptN=%d, scriptR=%lu, strR=%lu\n", m_scriptN, m_mapH.scriptR, m_mapH.strR);
		OutputDebugString(dbg);
	}

	if (m_fN == 0)
		m_mapH.formR   = NOVALUE;
	if (m_repN == -1)
		m_mapH.cellR   = NOVALUE;
	if (m_pageN == 0)
		m_mapH.pageR = NOVALUE;
	if (m_valueN == 0)
		m_mapH.valueR = NOVALUE;
	if (m_scriptN == 0)
		m_mapH.scriptR = NOVALUE;
	if (m_strR[0] == 0x00 && m_strN == 0)
		m_mapH.strR    = NOVALUE;

	
	cbWriteLen = 0;
	wRc = WriteFile(hFile, &m_mapH, sizeof(struct _mapH), &cbWriteLen, NULL);
	if (wRc == FALSE)
	{
		setMessage(-1, 14);
		CloseHandle(hFile);
		return(-1);
	}

	cbWriteLen = 0;
	wRc = WriteFile(hFile, m_formR, sizeof(struct _formR) * m_mapH.formN, &cbWriteLen, NULL);
	if (wRc == FALSE)
	{
		setMessage(-1, 14);
		CloseHandle(hFile);
		return(-1);
	}

	cbWriteLen = 0;
	wRc = WriteFile(hFile, m_cellR, sizeof(struct _cellR) * (m_repN+1), &cbWriteLen, NULL);
	if (wRc == FALSE)
	{
		setMessage(-1, 14);
		CloseHandle(hFile);
		return(-1);
	}

	cbWriteLen = 0;
	wRc = WriteFile(hFile, m_pageR, sizeof(struct _pageR) * m_pageN, &cbWriteLen, NULL);
	if (wRc == FALSE)
	{
		setMessage(-1, 14);
		CloseHandle(hFile);
		return(-1);
	}

	cbWriteLen = 0;
	wRc = WriteFile(hFile, m_valueR, sizeof(DWORD) * m_valueN, &cbWriteLen, NULL);
	if (wRc == FALSE)
	{
		setMessage(-1, 14);
		CloseHandle(hFile);
		return(-1);
	}


	cbWriteLen = 0;
	wRc = WriteFile(hFile, m_scriptR, m_scriptN, &cbWriteLen, NULL);
	if (wRc == FALSE)
	{
		setMessage(-1, 14);
		CloseHandle(hFile);
		return(-1);
	}

	cbWriteLen = 0;
	wRc = WriteFile(hFile, m_strR, m_strN, &cbWriteLen, NULL);
	if (wRc == FALSE)
	{
		setMessage(-1, 14);
		CloseHandle(hFile);
		return(-1);
	}

	CloseHandle(hFile);
	if (nickN.IsEmpty() && m_status)
		m_status->ShowBitmap(3, true);

	return(0);
}

void CCompile::ConvertString(char *src, int len)
{
	CFileFind finder;
	CString curdir;
	char twb[2048];

	GetCurrentDirectory(2048, twb);
	curdir = twb;
	GetPrivateProfileString(ENVIRONMENT, "CONVERTFILE", "", twb, sizeof(twb), curdir + "\\workshop.ini");
	curdir = twb;

	if (m_curdir.IsEmpty() || !finder.FindFile(curdir))
		return;

	CString txt = src, sConvert = getFileData(curdir);

	while(!sConvert.IsEmpty())
	{
		CString sDes, sSrc = "";
		
		sDes = Parser(sConvert, "\n");
		sSrc = Parser(sDes, ";");
		txt.Replace(sSrc, sDes);
		if (sSrc.GetLength() == 4 && sSrc.GetAt(0) < 0 && sSrc.GetAt(2) < 0)
		{
			txt.Replace(sSrc.Mid(0, 2) + " " + sSrc.Mid(2, 2), sDes);
			txt.Replace(sSrc.Mid(0, 2) + "  " + sSrc.Mid(2, 2), sDes);
		}
		else if (sSrc.GetLength() == 6 && sSrc.GetAt(0) < 0 && sSrc.GetAt(2) < 0 && sSrc.GetAt(4) < 0)
		{
			txt.Replace(sSrc.Mid(0, 2) + " " + sSrc.Mid(2, 2) + " " + sSrc.Mid(4, 2), sDes);
			txt.Replace(sSrc.Mid(0, 2) + "  " + sSrc.Mid(2, 2) + "  " + sSrc.Mid(4, 2), sDes);
		}
	}
	CopyMemory(src, txt.operator LPCTSTR(), len);
}

CString CCompile::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

CString CCompile::getFileData(CString sPath)
{
	CString sRtn = "";
	char *pbuffer;	// [64000];
	pbuffer = new char[64000];
	ZeroMemory(pbuffer, 64000);

	UINT nBytesRead = 0;

	TRY
	{
		CFile file(sPath, CFile::modeRead);
		do
		{ 
			nBytesRead = file.Read(pbuffer, sizeof(pbuffer));
			if (nBytesRead)
			{
				sRtn += pbuffer;
			}
		}while((int)nBytesRead);
		file.Close();
		
	}CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
		delete pbuffer;
		return "";
	}
	END_CATCH
	delete pbuffer;
	
	return sRtn;
}
