#include "pch.h"
#include "CUploadFile.h"
#include "h/axisfire.h"


_param  m_param;
class CMyException : public CException {
public:
	CMyException(const CString& message) : m_message(message) {}
	CString GetErrorMessage() const { 
		return m_message; 
	}
	virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL) {
		_tcsncpy_s(lpszError, nMaxError, m_message, _TRUNCATE);
		return TRUE;
	}
private:
	CString m_message;
};

CUploadFile::CUploadFile()
{
	
}

CUploadFile::~CUploadFile()
{

}

BOOL CUploadFile::upload(LPCTSTR sPath)
{
	CString spath, tmp;

	spath = CString(sPath);
	spath.TrimRight();

	try
	{
		char* pdata{};
		memcpy(pdata, "123", 3);
		if (spath.CompareNoCase(m_filename) || spath.IsEmpty() || m_strFileName.IsEmpty())
		{
			m_filename.Empty();
			m_strFileName.Empty();
			throw new CMyException(_T("This is a custom exception"));
		}
	}
	catch (CMyException* ex)
	{
		CString str;
		str = ex->GetErrorMessage();
		AfxMessageBox("why doesn't come?");
		return false;
	}
	

	
	spath = m_strFileName;

	m_file.Close();
	if (!m_file.Open(spath, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone))
	{
		m_strFileName.Empty();
		m_filename.Empty();
		return FALSE;
	}

	m_offset = 0;
	m_size = (int)m_file.GetLength();
	if (m_size == 0)
	{
		m_file.Close();
		return 0;
	}

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
		flow = DF_LAST;


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
		m_offset = -1;
	else
		m_offset += datl;

	return ret;
}

int CUploadFile::sendTR(CString strName, BYTE type, char* datb, int datl)
{
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(datl + L_userTH + 128);
	struct	_userTH	udat {};
	int	idx = 0;

	// key & control name
	pWb[idx++] = 1;
	CopyMemory(&pWb[idx], m_param.name, m_param.name.GetLength());
	idx += m_param.name.GetLength();
	pWb[idx++] = '\t';

	// udat
	CopyMemory(udat.trc, strName, min(sizeof(udat.trc), strName.GetLength()));
	udat.stat = type;
	udat.key = m_param.key;
	CopyMemory(&pWb[idx], &udat, L_userTH);
	idx += L_userTH;

	// data
	CopyMemory(&pWb[idx], datb, datl);

//	const	int	ret = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)pWb.get());
	pWb.reset();

	//return ret;
	return 0;
}
