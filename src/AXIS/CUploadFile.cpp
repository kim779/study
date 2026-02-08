#include "stdafx.h"
#include "CUploadFile.h"

class CMyException : public CException {
public:
	CMyException(const CString& message) : m_message(message) {}
	CString GetErrorMessage() const { return m_message; }
	virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) {
		_tcsncpy_s(lpszError, nMaxError, m_message, _TRUNCATE);
		return TRUE;
	}
private:
	CString m_message;
};

void CUploadFile::WriteLog(LPCSTR log, ...)
{
#if 1
	TRY
	{
		CString slog;
	slog.Format("[AXIS][WriteLog] [%s]\n", log);
	//OutputDebugString(slog);

		FILE* fp;
		fopen_s(&fp, Axis::home + "\\exe\\axis.log", "a+");
		if (!fp) return;

		const CTime time = CTime::GetCurrentTime();
		fprintf(fp, (LPCSTR)time.Format("[%Y-%m-%d %H:%M:%S] "));

		va_list argptr;
		va_start(argptr, log);
		vfprintf(fp, log, argptr);
		va_end(argptr);
		fprintf(fp, "\n");

		fclose(fp);
	}
		CATCH(CMemoryException, e)
	{

	}
	END_CATCH
#endif
}

CUploadFile::CUploadFile(CWnd* wizard)
{
	m_pWizard = wizard;
}

CUploadFile::~CUploadFile()
{

}

BOOL CUploadFile::GetFilesInFolder(CString sFolderPath)
{
	CString filename;
	WIN32_FIND_DATA FindFileData;
	HANDLE		hFind;
	CFile		cfile;
	CString		allfile;

	allfile = sFolderPath;
	//allfile += "\\*.dmp";
	allfile += "\\*.*";
	hFind = FindFirstFile(allfile, &FindFileData);
	while (hFind != INVALID_HANDLE_VALUE)
	{
		if (FindFileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
		{
			filename = FindFileData.cFileName;
			OutputDebugString("\r\n");
			OutputDebugString(filename);
			m_arrFile.Add(sFolderPath + "\\" + filename);
		}
		if (!FindNextFile(hFind, &FindFileData))	break;
	}

	return m_arrFile.GetSize();
}

BOOL CUploadFile::uploadfolder(CString sPath)
{
	m_arrFile.RemoveAll();
	if (!GetFilesInFolder(sPath))
		return FALSE;

	CString spath, tmp;
	spath = m_arrFile.GetAt(0);
	uploadFile(spath);
}

BOOL CUploadFile::uploadFile(CString sFile)
{
	CString  slog;
	if (!m_file.Open(sFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		slog.Format("file open fail    --> uploadFile =[%s]  ", sFile);
		WriteLog(slog);
		m_filename.Empty();
		return FALSE;
	}
	
	slog.Format("uploadFile =[%s]  ", sFile);
	WriteLog(slog);

	m_filename = sFile.Right(sFile.GetLength() - sFile.ReverseFind('\\') - 1);
	m_offset = 0;
	m_size = (int)m_file.GetLength();
	if (m_size == 0)
	{
		m_file.Close();
		return 0;
	}

	//WriteLog(m_filename);

	return sendData();
}

int CUploadFile::sendData()
{
	if (m_offset < 0)
		return -1;
	
	std::unique_ptr<char[]> wb = std::make_unique<char[]>(DF_FILESIZE);
	char*	base64encode = nullptr;
	char	flow = (m_offset == 0) ? DF_FIRST : DF_MIDDLE;
	const	int	datl = (m_offset + DF_FILESIZE > m_size) ? (m_size - m_offset) : DF_FILESIZE;

	if (datl > 0)
	{
		m_file.Seek(m_offset, CFile::begin);
		m_file.Read(wb.get(), datl);

#ifdef DF_BASE64
		datl = Base64Incode((unsigned char*)wb.get(), (unsigned char*&)base64encode, datl);
#endif
	}
	else
	{
		flow = DF_LAST;
	}

	CString tmps;
	std::unique_ptr<char[]> sndb = std::make_unique<char[]>(L_MID);
	st_mid_PMHOUPLD* pmid = (st_mid_PMHOUPLD*)sndb.get();

	pmid->flow = flow;
	CopyMemory(pmid->name, m_filename, m_filename.GetLength());
	tmps.Format("%5d", datl);
	CopyMemory(pmid->dlen, tmps, tmps.GetLength());
#ifdef DF_BASE64
	if (base64encode)
	{
		CopyMemory(pmid->data, base64encode, datl);
		free(base64encode);
	}
#else
	CopyMemory(pmid->data, wb.get(), datl);
#endif
	wb.reset();

	const	int	ret = sendTR("PMHOUPLD", US_KEY, sndb.get(), L_xMID + datl);
	sndb.reset();

	if (flow == DF_LAST)
	{
		m_offset = -1;
		m_size = 0;
		m_filename.Empty();
		m_file.Close();
		DeleteFile(m_arrFile.GetAt(0));
		m_arrFile.RemoveAt(0);
	}
	else
		m_offset += datl;

	return ret;
}

int CUploadFile::sendTR(CString strName, BYTE type, char* datb, int datl)
{
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(datl + L_userTH + 128);
	struct	_userTH	udat {};
	int	idx = 0;

	// udat
	CopyMemory(udat.trc, strName, min(sizeof(udat.trc), strName.GetLength()));
	udat.stat = type;
	udat.key = 233;
	CopyMemory(&pWb[idx], &udat, L_userTH);
	idx += L_userTH;

	// data
	CopyMemory(&pWb[idx], datb, datl);

	int rc{};
	m_pWizard->InvokeHelper(DI_TRX, DISPATCH_METHOD, VT_BOOL, (void*)&rc,
		(BYTE*)(VTS_I4 VTS_I4), (long)pWb.get(), datl);

	pWb.reset();

	return rc;
}

void CUploadFile::ParseOub(WPARAM wParam, LPARAM lParam)
{
	char* pdata = (char*)lParam;
	if (pdata[0] = '1')
	{
		int ret = sendData();
		if (ret == -1 && m_arrFile.GetSize() > 0)
			uploadFile(m_arrFile.GetAt(0));
	}
}
